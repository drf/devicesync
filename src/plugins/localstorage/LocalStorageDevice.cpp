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
