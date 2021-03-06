/***************************************************************************
 *   Copyright (C) 2008 by Dario Freddi <drf@kdemod.ath.cx>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "MtpDevice.h"

#include <KDebug>
#include <KUrl>
#include <KIcon>

#include <QStandardItemModel>
#include <QFile>

#include <threadweaver/ThreadWeaver.h>

#include <solid/device.h>
#include <solid/portablemediaplayer.h>

#include <kio/job.h>
#include <kio/jobclasses.h>
#include <kio/netaccess.h>

#include <taglib/taglib.h>
#include <taglib/mpegfile.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>

#include <klocalizedstring.h>

LibMtpCallbacks* LibMtpCallbacks::s_instance = 0;

int mtp_transfer_callback(uint64_t const sent, uint64_t const total, void const *const data)
{
    Q_UNUSED(data)

    float percent = (float)sent / (float)total;
    int realpercent = percent * 100;

    LibMtpCallbacks::instance()->setActionPercentage(realpercent);

    return 0;
}

class MtpDevice::Private
{
public:
    Private(const QString &u) : model(0), udi(u) {}

    QAbstractItemModel *model;
    QString udi;
    bool success;
    LIBMTP_mtpdevice_t *device;
};

MtpDevice::MtpDevice(const QString &udi, AbstractDeviceInterface *parent)
        : AbstractDevice(parent),
        d(new Private(udi))
{
    connect(LibMtpCallbacks::instance(), SIGNAL(actionPercentageChanged(int)),
            this, SIGNAL(actionProgressChanged(int)));

    if (!udi.isEmpty()) {
        Solid::PortableMediaPlayer* pmp = Solid::Device(d->udi).as<Solid::PortableMediaPlayer>();
        QString serial = pmp->driverHandle("mtp").toString();

        setName(i18n("Mtp Device (serial %1)", serial));
    } else {
        setName(i18n("Mtp Device (raw)"));
    }

    setIcon("multimedia-player");
}

MtpDevice::~MtpDevice()
{
    delete d;
}

void MtpDevice::reloadModel()
{
    kDebug() << "Creating model";

    ThreadWeaver::Job * thread = new CreateModelThread(d->device, this);
    ThreadWeaver::Weaver::instance()->enqueue(thread);
}

void MtpDevice::createFolder(const QString &name, const QString &inPath)
{
    LIBMTP_Create_Folder(d->device, qstrdup(name.toUtf8()), inPath.toInt(), 0);
}

void MtpDevice::renameObject(const QString &path, const QString &newName)
{
    LIBMTP_file_t *files = LIBMTP_Get_Filelisting_With_Callback(d->device, 0, 0);

    while (files) {
        if (files->item_id == (uint32_t)path.toInt()) {
            LIBMTP_Set_File_Name(d->device, files, qstrdup(newName.toUtf8()));
            return;
        }

        files = files->next;
    }

    LIBMTP_folder_t *folders = LIBMTP_Get_Folder_List(d->device);

    while (folders) {
        if (folders->folder_id == (uint32_t)path.toInt()) {
            LIBMTP_Set_Folder_Name(d->device, folders, qstrdup(newName.toUtf8()));
            return;
        }

        folders = folders->sibling;
    }
}

void MtpDevice::connectDevice()
{
    QString serial;

    if (!d->udi.isEmpty()) {
        Solid::PortableMediaPlayer* pmp = Solid::Device(d->udi).as<Solid::PortableMediaPlayer>();
        serial = pmp->driverHandle("mtp").toString();
    } else {
        serial.clear();
    }

    LIBMTP_raw_device_t * rawdevices;
    int numrawdevices;
    LIBMTP_error_number_t err;

    // get list of raw devices
    kDebug() << "Getting list of raw devices";
    err = LIBMTP_Detect_Raw_Devices(&rawdevices, &numrawdevices);

    kDebug() << "Error is: " << err;

    switch (err) {
    case LIBMTP_ERROR_NO_DEVICE_ATTACHED:
        kDebug() << "No raw devices found";
        d->success = false;
        break;

    case LIBMTP_ERROR_CONNECTING:
        kDebug() << "Detect: There has been an error connecting. Exiting";
        d->success = false;
        break;

    case LIBMTP_ERROR_MEMORY_ALLOCATION:
        kDebug() << "Detect: Encountered a Memory Allocation Error. Exiting";
        d->success = false;
        break;

    case LIBMTP_ERROR_NONE: {
        d->success = true;
        break;
    }

    default:
        kDebug() << "Unhandled mtp error";
        d->success = false;
        break;
    }

    if (d->success) {
        kDebug() << "Got mtp list, connecting to device using thread";
        ThreadWeaver::Weaver::instance()->enqueue(new WorkerThread(numrawdevices, rawdevices, serial, this));
    } else {
        free(rawdevices);
    }
}

void MtpDevice::connectionSuccessful()
{
    setName(QString("%1 (%2)").arg(LIBMTP_Get_Modelname(d->device)).arg(LIBMTP_Get_Friendlyname(d->device)));
    emit deviceConnected(this);
}

bool MtpDevice::iterateRawDevices(int numrawdevices, LIBMTP_raw_device_t* rawdevices, const QString &serial)
{

    bool success;

    LIBMTP_mtpdevice_t *device = 0;
    // test raw device for connectability
    for (int i = 0; i < numrawdevices; i++) {

        kDebug() << "Opening raw device number: " << (i + 1);
        device = LIBMTP_Open_Raw_Device(&rawdevices[i]);
        if (device == NULL) {
            kDebug() << "Unable to open raw device: " << (i + 1);
            success = false;
            LIBMTP_Release_Device(device);
            continue;
        }

        if (!serial.isEmpty()) {
            kDebug() << "Testing serial number";

            QString mtpSerial = QString::fromUtf8(LIBMTP_Get_Serialnumber(device));
            if (!mtpSerial.contains(serial)) {
                kDebug() << "Wrong device, going to next";
                kDebug() << "Expected: " << serial << " but got: " << mtpSerial;
                success = false;
                LIBMTP_Release_Device(device);
                continue;
            }
        }

        kDebug() << "Correct device found";
        success = true;
        break;
    }

    d->device = device;

    if (d->device == 0) {
        // TODO: error protection
        success = false;
        free(rawdevices);

    }

    if (serial.isEmpty()) {
        kDebug() << "Serial is: " << QString::fromUtf8(LIBMTP_Get_Serialnumber(d->device));
    } else {
        kDebug() << "Serial is: " << serial;
    }

    kDebug() << "Success is: " << (success ? "true" : "false");

    return success;
}

void MtpDevice::disconnectDevice()
{
    LIBMTP_Release_Device(d->device);
}

void MtpDevice::modelCreated(QStandardItemModel *model)
{
    d->model = model;
    setModel(d->model);
}

QString MtpDevice::getPathForCurrentIndex(const QModelIndex &index)
{
    return d->model->data(index, 40).toString();
}

void MtpDevice::transferSuccessful(ThreadWeaver::Job *job)
{
    emit fileCopiedToDevice(job->property("ds_transfer_token").toInt(), job->property("ds_filename").toString());
    job->deleteLater();
}

void MtpDevice::deletionSuccessful(ThreadWeaver::Job *job)
{
    emit pathRemovedFromDevice(job->property("ds_transfer_token").toInt(), job->property("ds_filename").toString());
    job->deleteLater();
}

int MtpDevice::sendFileToDevice(const QString &fromPath, const QString &toPath)
{
    KUrl url = KUrl::fromPath(fromPath);
    int token = getNextTransferToken();

    if (fromPath.contains(".mp3")) {
        TagLib::MPEG::File *file = new TagLib::MPEG::File(TagLib::FileName(url.path().toUtf8()));

        LIBMTP_track_t *trackmeta = LIBMTP_new_track_t();
        trackmeta->filetype = LIBMTP_FILETYPE_MP3;
        trackmeta->title = qstrdup(file->tag()->title().toCString());
        trackmeta->album = qstrdup(file->tag()->album().toCString());
        trackmeta->artist = qstrdup(file->tag()->artist().toCString());
        trackmeta->genre = qstrdup(file->tag()->genre().toCString());
        trackmeta->tracknumber = file->tag()->track();
        trackmeta->filesize = file->length();
        trackmeta->filename = qstrdup(url.fileName().toUtf8());
        trackmeta->duration = file->audioProperties()->length() * 1000;
        trackmeta->bitrate = file->audioProperties()->bitrate();
        trackmeta->samplerate = file->audioProperties()->sampleRate();

        ThreadWeaver::Job * thread = new SendTrackThread(d->device, qstrdup(url.path().toUtf8()),
                trackmeta, mtp_transfer_callback, this);
        thread->setProperty("ds_transfer_token", token);
        thread->setProperty("ds_filename", url.fileName());
        ThreadWeaver::Weaver::instance()->enqueue(thread);

        return token;
    } else {
        LIBMTP_file_t *file = LIBMTP_new_file_t();
        QFile qfile(url.path().toUtf8());

        KIO::MimetypeJob *job = KIO::mimetype(url, KIO::HideProgressInfo);

        KIO::NetAccess::synchronousRun(job, 0);

        QString mimetype = job->mimetype();

        job->deleteLater();

        if (mimetype == "video/x-msvideo" || mimetype == "video/msvideo") {
            kDebug() << "Setting AVI Video";
            file->filetype = LIBMTP_FILETYPE_AVI;
        } else if (mimetype == "video/x-mpeg" || mimetype == "video/mpeg") {
            kDebug() << "Setting MPEG Video";
            file->filetype = LIBMTP_FILETYPE_MPEG;
        } else if (mimetype == "image/jpeg") {
            kDebug() << "Setting JPEG Image";
            file->filetype = LIBMTP_FILETYPE_JPEG;
        } else if (mimetype == "image/jp2") {
            kDebug() << "Setting JP2 Image";
            file->filetype = LIBMTP_FILETYPE_JP2;
        } else if (mimetype == "image/png") {
            kDebug() << "Setting PNG Image";
            file->filetype = LIBMTP_FILETYPE_PNG;
        } else {
            file->filetype = LIBMTP_FILETYPE_UNDEF_VIDEO;
        }

        file->filename = qstrdup(url.fileName().toUtf8());
        file->filesize = qfile.size();
        file->parent_id = toPath.toInt();

        ThreadWeaver::Job * thread = new SendFileThread(d->device, qstrdup(url.path().toUtf8()),
                file, mtp_transfer_callback, this);
        thread->setProperty("ds_transfer_token", token);
        thread->setProperty("ds_filename", url.fileName());
        ThreadWeaver::Weaver::instance()->enqueue(thread);

        return token;
    }

    return -1;
}

int MtpDevice::sendFileToDeviceFromByteArray(const QByteArray &, const QString &)
{

    return -1;
}

int MtpDevice::getFileFromDevice(const QString &, const QString &)
{
    // TODO
    return -1;
}

int MtpDevice::getByteArrayFromDeviceFile(const QString &)
{
    return -1;
}

int MtpDevice::removePath(const QString &path)
{
    int token = getNextTransferToken();

    ThreadWeaver::Job * thread = new DeleteObjectThread(d->device, path.toInt(), this);
    thread->setProperty("ds_transfer_token", token);
    ThreadWeaver::Weaver::instance()->enqueue(thread);

    return token;
}

/////

void LibMtpCallbacks::setActionPercentage(int percentage)
{
    emit actionPercentageChanged(percentage);
}

////

WorkerThread::WorkerThread(int numrawdevices, LIBMTP_raw_device_t* rawdevices, const QString &serial, MtpDevice* handler)
        : ThreadWeaver::Job()
        , m_success(false)
        , m_numrawdevices(numrawdevices)
        , m_rawdevices(rawdevices)
        , m_serial(serial)
        , m_handler(handler)
{
    connect(this, SIGNAL(failed(ThreadWeaver::Job*)), m_handler, SLOT(slotDeviceMatchFailed(ThreadWeaver::Job*)));
    connect(this, SIGNAL(done(ThreadWeaver::Job*)), m_handler, SLOT(connectionSuccessful()));
    connect(this, SIGNAL(done(ThreadWeaver::Job*)), this, SLOT(deleteLater()));
}

WorkerThread::~WorkerThread()
{
    //nothing to do
}

bool WorkerThread::success() const
{
    return m_success;
}

void WorkerThread::run()
{
    m_success = m_handler->iterateRawDevices(m_numrawdevices, m_rawdevices, m_serial);
}

////

CreateModelThread::CreateModelThread(LIBMTP_mtpdevice_t *device, MtpDevice* handler)
        : ThreadWeaver::Job()
        , m_success(false)
        , m_device(device)
        , m_handler(handler)
        , m_model(new QStandardItemModel())
{
    connect(this, SIGNAL(modelCreated(QStandardItemModel*)), m_handler, SLOT(modelCreated(QStandardItemModel*)));
    connect(this, SIGNAL(done(ThreadWeaver::Job*)), this, SLOT(deleteLater()));
}

CreateModelThread::~CreateModelThread()
{
    //nothing to do
}

bool CreateModelThread::success() const
{
    return m_success;
}

void CreateModelThread::run()
{
    LIBMTP_file_t *mtpfiles = LIBMTP_Get_Filelisting_With_Callback(m_device, 0, 0);
    LIBMTP_folder_t *folders = LIBMTP_Get_Folder_List(m_device);

    iterateSiblings(folders);
    populateFiles(mtpfiles);

    kDebug() << "Model created";
    emit modelCreated(m_model);
}

void CreateModelThread::populateFiles(LIBMTP_file_t *files)
{
    while (files) {
        QStandardItem *itm = new QStandardItem();

        itm->setText(files->filename);
        itm->setIcon(KIcon("application-octet-stream"));
        itm->setData(files->item_id, 40);

        if (m_itemMap.contains(files->parent_id)) {
            m_itemMap[files->parent_id]->appendRow(itm);
        } else {
            m_model->invisibleRootItem()->appendRow(itm);
        }

        files = files->next;
    }
}

void CreateModelThread::iterateChildren(LIBMTP_folder_t *parentfolder)
{
    LIBMTP_folder_t *folder = parentfolder->child;

    while (folder) {
        QStandardItem *itm = new QStandardItem();

        kDebug() << "New folder detected:" << folder->name << " id: " << folder->folder_id;

        itm->setText(folder->name);
        itm->setIcon(KIcon("folder"));
        itm->setData(folder->folder_id, 40);
        m_itemMap[folder->folder_id] = itm;

        m_itemMap[folder->parent_id]->appendRow(itm);

        iterateChildren(folder);
        folder = folder->sibling;
    }
}

void CreateModelThread::iterateSiblings(LIBMTP_folder_t *parentfolder)
{
    LIBMTP_folder_t *folder = parentfolder;

    while (folder) {
        QStandardItem *itm = new QStandardItem();

        kDebug() << "New folder detected:" << folder->name << " id: " << folder->folder_id;

        itm->setText(folder->name);
        itm->setIcon(KIcon("folder"));
        itm->setData(folder->folder_id, 40);
        m_model->invisibleRootItem()->appendRow(itm);
        m_itemMap[folder->folder_id] = itm;
        iterateChildren(folder);
        folder = folder->sibling;
    }
}

//

SendTrackThread::SendTrackThread(LIBMTP_mtpdevice_t *device, QString name, LIBMTP_track_t *trackmeta,
                                 LIBMTP_progressfunc_t cb, MtpDevice *parent)
        : ThreadWeaver::Job()
        , m_success(0)
        , m_device(device)
        , m_name(name)
        , m_trackmeta(trackmeta)
        , m_callback(cb)
        , m_parent(parent)
{
    connect(this, SIGNAL(failed(ThreadWeaver::Job*)), m_parent, SLOT(transferFailed(ThreadWeaver::Job*)));
    connect(this, SIGNAL(done(ThreadWeaver::Job*)), m_parent, SLOT(transferSuccessful(ThreadWeaver::Job*)));
}

SendTrackThread::~SendTrackThread()
{
    //nothing to do
}

bool SendTrackThread::success() const
{
    return m_success;
}

void SendTrackThread::run()
{
    m_success = LIBMTP_Send_Track_From_File(m_device, qstrdup(m_name.toUtf8()), m_trackmeta,
                                            m_callback, m_parent);
}

//

SendFileThread::SendFileThread(LIBMTP_mtpdevice_t *device, QString name, LIBMTP_file_t *file,
                               LIBMTP_progressfunc_t cb, MtpDevice *parent)
        : ThreadWeaver::Job()
        , m_success(0)
        , m_device(device)
        , m_name(name)
        , m_file(file)
        , m_callback(cb)
        , m_parent(parent)
{
    connect(this, SIGNAL(failed(ThreadWeaver::Job*)), m_parent, SLOT(transferFailed(ThreadWeaver::Job*)));
    connect(this, SIGNAL(done(ThreadWeaver::Job*)), m_parent, SLOT(transferSuccessful(ThreadWeaver::Job*)));
}

SendFileThread::~SendFileThread()
{
    //nothing to do
}

bool SendFileThread::success() const
{
    return m_success;
}

void SendFileThread::run()
{
    m_success = LIBMTP_Send_File_From_File(m_device, qstrdup(m_name.toUtf8()), m_file,
                                           m_callback, m_parent);
    kDebug() << m_success;
}

//

DeleteObjectThread::DeleteObjectThread(LIBMTP_mtpdevice_t *device, uint32_t object, MtpDevice *parent)
        : ThreadWeaver::Job()
        , m_success(0)
        , m_device(device)
        , m_object(object)
        , m_parent(parent)
{
    connect(this, SIGNAL(failed(ThreadWeaver::Job*)), m_parent, SLOT(deletionFailed(ThreadWeaver::Job*)));
    connect(this, SIGNAL(done(ThreadWeaver::Job*)), m_parent, SLOT(deletionSuccessful(ThreadWeaver::Job*)));
}

DeleteObjectThread::~DeleteObjectThread()
{
    //nothing to do
}

bool DeleteObjectThread::success() const
{
    return m_success;
}

void DeleteObjectThread::run()
{
    m_folders = LIBMTP_Get_Folder_List(m_device);

    removeObjectRecursively(m_object, true);

    m_success = true;

    kDebug() << m_success;
}

void DeleteObjectThread::removeObjectRecursively(uint32_t object, bool main)
{
    LIBMTP_folder_t *folder = LIBMTP_Find_Folder(m_folders, object);

    if (folder == NULL) {
        // Not a folder, just delete it and we are done

        if (main) {
            LIBMTP_file_t *files = LIBMTP_Get_Filelisting_With_Callback(m_device, 0, 0);

            while (files) {
                if (files->item_id == object) {
                    setProperty("ds_filename", QString(files->filename));
                    break;
                }

                files = files->next;
            }
        }

        LIBMTP_Delete_Object(m_device, object);
    } else {

        if (main) {
            setProperty("ds_filename", QString(folder->name));
        }

        // First of all, let's destroy all files inside the directory.

        LIBMTP_file_t *files = LIBMTP_Get_Filelisting_With_Callback(m_device, 0, 0);

        while (files) {
            if (files->parent_id == object) {
                LIBMTP_Delete_Object(m_device, files->item_id);
            }

            files = files->next;
        }

        // And now, let's recurse again if there are some folders

        LIBMTP_folder_t *child = folder->child;

        while (child != NULL) {
            removeObjectRecursively(child->folder_id);
            child = child->sibling;
        }

        // Last but not least, let's remove the folder itself.

        LIBMTP_Delete_Object(m_device, object);
    }
}

#include "MtpDevice.moc"
