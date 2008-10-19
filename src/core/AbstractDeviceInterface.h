/*
 * AbstractDeviceInterface.h
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#ifndef ABSTRACTDEVICEINTERFACE_H
#define ABSTRACTDEVICEINTERFACE_H

#include <QObject>
#include <kdemacros.h>
#include <KPluginFactory>
#include <KPluginLoader>

#define DEVICESYNC_PLUGIN_EXPORT( c ) \
    K_PLUGIN_FACTORY( DeviceSyncFactory, registerPlugin< c >(); ) \
    K_EXPORT_PLUGIN( DeviceSyncFactory("c") )

class KDE_EXPORT AbstractDeviceInterface : public QObject
{
    public:
        AbstractDeviceInterface();
        virtual ~AbstractDeviceInterface();

        virtual void init() = 0;

    public slots:
        virtual void startWatching() = 0;
        virtual void stopWatching() = 0;

        virtual void connectDevice(const QString &uuid) = 0;
        virtual void disconnectDevice(const QString &uuid) = 0;

        virtual void getConnectedDevices() = 0;
        virtual void getAllDevices() = 0;

    signals:
        void deviceConnected();
        void deviceDisconnected();
        void newDeviceAvailable();
};

#endif /* ABSTRACTDEVICEINTERFACE_H */
