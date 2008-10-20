/*
 * LocalStorageDevice.h
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#ifndef LOCALSTORAGEDEVICE_H
#define LOCALSTORAGEDEVICE_H

#include "AbstractDevice.h"

class QFileSystemModel;

class LocalStorageDevice : public AbstractDevice
{
public:
    LocalStorageDevice();
    virtual ~LocalStorageDevice();

    QAbstractItemModel * getFileModel();

private:
    QFileSystemModel *m_model;
};

#endif /* LOCALSTORAGEDEVICE_H_ */
