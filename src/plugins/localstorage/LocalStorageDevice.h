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

#ifndef LOCALSTORAGEDEVICE_H
#define LOCALSTORAGEDEVICE_H

#include "AbstractDevice.h"
#include <kio/job.h>

class QFileSystemModel;
class KJob;

class LocalStorageDevice : public AbstractDevice
{
    Q_OBJECT

public:
    explicit LocalStorageDevice(AbstractDeviceInterface *parent);
    virtual ~LocalStorageDevice();

    void connectDevice();
    void disconnectDevice();

    QString getPathForCurrentIndex(const QModelIndex &index);

    void reloadModel();

public slots:
    int sendFileToDevice(const QString &fromPath, const QString &toPath);
    int sendFileToDeviceFromByteArray(const QByteArray &file, const QString &toPath);

    int getFileFromDevice(const QString &path, const QString &toPath);
    int getByteArrayFromDeviceFile(const QString &path);

    void createFolder(const QString &name, const QString &inPath);
    int removePath(const QString &path);

private slots:
    void sendToDeviceDone(KIO::Job*, const KUrl&, const KUrl&, time_t, bool, bool);
    void getFromDeviceDone(KIO::Job*, const KUrl&, const KUrl&, time_t, bool, bool);
    void removalDone(KIO::Job*);

    void sendDoneConfirmation();

private:
    QFileSystemModel *m_model;
    int m_token;
    KUrl m_url;
};

#endif /* LOCALSTORAGEDEVICE_H_ */
