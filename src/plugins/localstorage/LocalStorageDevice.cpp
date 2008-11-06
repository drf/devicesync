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

#include "LocalStorageDevice.h"

#include <QFileSystemModel>

#include <kio/copyjob.h>
#include <kio/deletejob.h>
#include <kio/netaccess.h>

#include <QTimer>

LocalStorageDevice::LocalStorageDevice(AbstractDeviceInterface *parent)
        : AbstractDevice(parent),
        m_model(0)
{
    setName("Local Storage");
    setIcon("drive-harddisk");
}

LocalStorageDevice::~LocalStorageDevice()
{
    // TODO Auto-generated destructor stub
}

void LocalStorageDevice::createFolder(const QString &name, const QString &inPath)
{
    KUrl path = KUrl::fromPath(inPath);
    path.addPath(name);

    KIO::SimpleJob *job = KIO::mkdir(path);

    KIO::NetAccess::synchronousRun(job, 0);
}

void LocalStorageDevice::reloadModel()
{
    if (m_model) {
        m_model->deleteLater();
    }

    m_model = new QFileSystemModel();
    m_model->setRootPath("/");

    setModel(m_model);
}

void LocalStorageDevice::connectDevice()
{

}

void LocalStorageDevice::disconnectDevice()
{
    m_model->deleteLater();
}

QString LocalStorageDevice::getPathForCurrentIndex(const QModelIndex &index)
{
    return m_model->filePath(index);
}

int LocalStorageDevice::sendFileToDevice(const QString &fromPath, const QString &toPath)
{
    int token = getNextTransferToken();

    KJob *job = KIO::copy(KUrl::fromPath(fromPath), KUrl::fromPath(toPath));
    job->setProperty("transfer_token", token);

    connect(job, SIGNAL(copyingDone(KIO::Job*, const KUrl&, const KUrl&, time_t, bool, bool)),
            this, SLOT(sendToDeviceDone(KIO::Job*, const KUrl&, const KUrl&, time_t, bool, bool)));

    return token;
}

int LocalStorageDevice::sendFileToDeviceFromByteArray(const QByteArray &, const QString &)
{
    return -1;
}

int LocalStorageDevice::getFileFromDevice(const QString &path, const QString &toPath)
{
    Q_UNUSED(toPath)

    m_token = getNextTransferToken();
    m_url = KUrl::fromPath(path);

    QTimer::singleShot(50, this, SLOT(sendDoneConfirmation()));

    return m_token;

    /*emit fileCopiedToDevice(token, KUrl::fromPath(path).pathOrUrl());

    KJob *job = KIO::copy(KUrl::fromPath(path), KUrl::fromPath(toPath), KIO::HideProgressInfo | KIO::Overwrite);
    job->setProperty("transfer_token", token);

    connect(job, SIGNAL(copyingDone(KIO::Job*, const KUrl&, const KUrl&, time_t, bool, bool)),
            this, SLOT(getFromDeviceDone(KIO::Job*, const KUrl&, const KUrl&, time_t, bool, bool)));*/


}

int LocalStorageDevice::removePath(const QString &path)
{
    KJob *job = KIO::del(KUrl::fromPath(path), KIO::HideProgressInfo);
    int token = getNextTransferToken();
    job->setProperty("transfer_token", token);
    job->setProperty("path_removed", path);

    connect(job, SIGNAL(result(KJob*)), this, SLOT(removalDone(KJob*)));

    return token;
}

void LocalStorageDevice::removalDone(KJob *job)
{
    emit pathRemovedFromDevice(job->property("transfer_token").toInt(),
                               job->property("path_removed").toString());
}

void LocalStorageDevice::sendDoneConfirmation()
{
    emit fileCopiedFromDevice(m_token, m_url.pathOrUrl());
}

void LocalStorageDevice::sendToDeviceDone(KIO::Job *job, const KUrl&, const KUrl &to, time_t, bool, bool)
{
    emit fileCopiedToDevice(job->property("transfer_token").toInt(), to.pathOrUrl());
}

void LocalStorageDevice::getFromDeviceDone(KIO::Job *job, const KUrl&, const KUrl &to, time_t, bool, bool)
{
    emit fileCopiedFromDevice(job->property("transfer_token").toInt(), to.pathOrUrl());
}

int LocalStorageDevice::getByteArrayFromDeviceFile(const QString &)
{
    return -1;
}

#include "LocalStorageDevice.moc"
