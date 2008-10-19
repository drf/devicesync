/*
 * LocalStorage.cpp
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#include "LocalStorage.h"

DEVICESYNC_PLUGIN_EXPORT(LocalStorage)

LocalStorage::LocalStorage(QObject *parent, const QVariantList&)
{

}

LocalStorage::~LocalStorage()
{
}

void LocalStorage::init()
{

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
