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

    connect(job, SIGNAL(copyingDone(KIO::Job*, const KUrl&, const KUrl&, time_t, bool, bool)),
            this, SLOT(sendToDeviceDone(KIO::Job*, const KUrl&, const KUrl&, time_t, bool, bool)));

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

    connect(job, SIGNAL(copyingDone(KIO::Job*, const KUrl&, const KUrl&, time_t, bool, bool)),
            this, SLOT(getFromDeviceDone(KIO::Job*, const KUrl&, const KUrl&, time_t, bool, bool)));

    return token;
}

void LocalStorageDevice::sendToDeviceDone(KIO::Job *job, const KUrl&, const KUrl &to, time_t, bool, bool)
{
    emit fileCopiedToDevice(job->property("transfer_token").toInt(), to.pathOrUrl());
}

void LocalStorageDevice::getFromDeviceDone(KIO::Job *job, const KUrl&, const KUrl &to, time_t, bool, bool)
{
    emit fileCopiedFromDevice(job->property("transfer_token").toInt(), to.pathOrUrl());
}

int LocalStorageDevice::getByteArrayFromDeviceFile(const QString &path)
{
    return -1;
}

#include "LocalStorageDevice.moc"
