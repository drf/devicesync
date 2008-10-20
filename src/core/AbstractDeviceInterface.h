/*
 * AbstractDeviceInterface.h
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

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

public slots:
    virtual void startWatching() = 0;
    virtual void stopWatching() = 0;

    AbstractDevice::List getConnectedDevices();
    DeviceContainer::List getAllDevices();

protected slots:
    void addDevice(AbstractDevice *device);
    void removeDevice(AbstractDevice *device);
    void connectDevice(AbstractDevice *device);
    void disconnectDevice(AbstractDevice *device);

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
