/*
 * LocalStorageDevice.cpp
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#include "LocalStorageDevice.h"

#include <QFileSystemModel>
#include <kio/job.h>
#include <kio/copyjob.h>

LocalStorageDevice::LocalStorageDevice()
{
    setName("Local Storage");
    m_model = new QFileSystemModel();
    m_model->setRootPath("/");
}

LocalStorageDevice::~LocalStorageDevice()
{
    // TODO Auto-generated destructor stub
}

QAbstractItemModel *LocalStorageDevice::getFileModel()
{
    return m_model;
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

    return token;
}

int LocalStorageDevice::sendFileToDeviceFromByteArray(const QByteArray &file, const QString &toPath)
{
    return -1;
}

int LocalStorageDevice::getFileFromDevice(const QString &path, const QString &toPath)
{
    int token = getNextTransferToken();

    KJob *job = KIO::copy(KUrl::fromPath(path), KUrl::fromPath(toPath));
    job->setProperty("transfer_token", token);

    return token;
}

int LocalStorageDevice::getByteArrayFromDeviceFile(const QString &path)
{
    return -1;
}

#include "LocalStorageDevice.moc"
