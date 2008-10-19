/*
 * LocalStorage.h
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#ifndef LOCALSTORAGE_H_
#define LOCALSTORAGE_H_

#include "AbstractDeviceInterface.h"

class LocalStorage : public AbstractDeviceInterface
{
    public:
        LocalStorage(QObject *parent, const QVariantList&);
        virtual ~LocalStorage();

        void init();

    public slots:
        QAbstractItemModel *getFileList();
};

#endif /* LOCALSTORAGE_H_ */
