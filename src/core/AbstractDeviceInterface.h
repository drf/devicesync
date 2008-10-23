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

#ifndef ABSTRACTDEVICEINTERFACE_H
#define ABSTRACTDEVICEINTERFACE_H

#include <kdemacros.h>
#include <KPluginFactory>
#include <KPluginLoader>

#include "AbstractDevice.h"

#define DEVICESYNC_PLUGIN_EXPORT( c ) \
    K_PLUGIN_FACTORY( DeviceSyncFactory, registerPlugin< c >(); ) \
    K_EXPORT_PLUGIN( DeviceSyncFactory("c") )

class KDE_EXPORT AbstractDeviceInterface : public QObject
{
    Q_OBJECT

public:
    typedef QList<AbstractDeviceInterface*> List;

    AbstractDeviceInterface(QObject *parent = 0);
    virtual ~AbstractDeviceInterface();

    virtual void init() = 0;

    virtual void scan() = 0;

public slots:
    virtual void startWatching() = 0;
    virtual void stopWatching() = 0;

    AbstractDevice::List getConnectedDevices();
    DeviceContainer::List getAllDevices();
    void connectDevice(AbstractDevice *device);
    void disconnectDevice(AbstractDevice *device);

protected slots:
    void addDevice(AbstractDevice *device, const QString &udi = QString());
    void removeDevice(AbstractDevice *device);

signals:
    void deviceConnected(AbstractDevice *device);
    void deviceDisconnected(AbstractDevice *device);
    void newDeviceRegistered(AbstractDevice *device);
    void deviceRemoved(AbstractDevice *device);

private:
    class Private;
    Private *d;
};

#endif /* ABSTRACTDEVICEINTERFACE_H */
