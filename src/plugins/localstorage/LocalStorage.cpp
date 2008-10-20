/*
 * LocalStorage.cpp
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#include "LocalStorage.h"
#include "LocalStorageDevice.h"

DEVICESYNC_PLUGIN_EXPORT(LocalStorage)

LocalStorage::LocalStorage(QObject *parent, const QVariantList&)
{

}

LocalStorage::~LocalStorage()
{
}

void LocalStorage::init()
{
    DeviceContainer *cont = new DeviceContainer();
    LocalStorageDevice *device = new LocalStorageDevice();

    cont->device = device;
    cont->status = DeviceContainer::Connected;

    m_devices.append(cont);

    emit deviceConnected(device);
}

void LocalStorage::startWatching()
{

}

void LocalStorage::stopWatching()
{

}

void LocalStorage::connectDevice(const QString &uuid)
{

}

void LocalStorage::disconnectDevice(const QString &uuid)
{

}

AbstractDevice::List LocalStorage::getConnectedDevices()
{

}

DeviceContainer::List LocalStorage::getAllDevices()
{

}

#include "LocalStorage.moc"
