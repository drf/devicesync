/*
 * LocalStorageDevice.cpp
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#include "LocalStorageDevice.h"

#include <QFileSystemModel>

LocalStorageDevice::LocalStorageDevice()
{
    setName("Local Storage");
}

LocalStorageDevice::~LocalStorageDevice()
{
    // TODO Auto-generated destructor stub
}

QAbstractItemModel *LocalStorageDevice::getFileModel()
{
    return new QFileSystemModel();
}
