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
    LocalStorageDevice *device = new LocalStorageDevice();

    addDevice(device);
    connectDevice(device);
}

void LocalStorage::startWatching()
{

}

void LocalStorage::stopWatching()
{

}

#include "LocalStorage.moc"
