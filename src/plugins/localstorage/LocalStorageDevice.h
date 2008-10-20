/*
 * LocalStorageDevice.h
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#ifndef LOCALSTORAGEDEVICE_H
#define LOCALSTORAGEDEVICE_H

#include "AbstractDevice.h"

class LocalStorageDevice : public AbstractDevice
{
    public:
        LocalStorageDevice();
        virtual ~LocalStorageDevice();

        QAbstractItemModel * getFileModel();
};

#endif /* LOCALSTORAGEDEVICE_H_ */
