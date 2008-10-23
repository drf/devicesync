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

#ifndef ABSTRACTDEVICE_H
#define ABSTRACTDEVICE_H

#include <QObject>
#include <QList>
#include <QModelIndex>

#include <kdemacros.h>

class QAbstractItemModel;

class KDE_EXPORT AbstractDevice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)

public:
    typedef QList<AbstractDevice*> List;

    AbstractDevice(QObject *parent = 0);
    virtual ~AbstractDevice();

    virtual void connectDevice() = 0;
    virtual void disconnectDevice() = 0;

    QString name();

    virtual QAbstractItemModel *getFileModel() = 0;
    virtual QString getPathForCurrentIndex(const QModelIndex &index) = 0;

public slots:
    virtual int sendFileToDevice(const QString &fromPath, const QString &toPath) = 0;
    virtual int sendFileToDeviceFromByteArray(const QByteArray &file, const QString &toPath) = 0;

    virtual int getFileFromDevice(const QString &path, const QString &toPath) = 0;
    virtual int getByteArrayFromDeviceFile(const QString &path) = 0;

protected:
    void setName(const QString &name);
    int getNextTransferToken();

signals:
    void fileCopiedToDevice(int, const QString&);
    void fileCopiedFromDevice(int, const QString&);

    void actionProgressChanged(int);

    void modelChanged(QAbstractItemModel *, AbstractDevice *);

    void deviceConnected(AbstractDevice *device);

private:
    class Private;
    Private *d;
};

class KDE_EXPORT DeviceContainer
{
public:
    typedef QList<DeviceContainer*> List;

    enum Status {
        Connected,
        Disconnected
    };

    AbstractDevice *device;
    Status status;
    QString udi;
};

#endif /* ABSTRACTDEVICE_H_ */
