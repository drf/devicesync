/*
 * LocalStorage.h
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#ifndef LOCALSTORAGE_H_
#define LOCALSTORAGE_H_

#include "AbstractDeviceInterface.h"

#include <QVariant>

class LocalStorage : public AbstractDeviceInterface
{
    public:
        LocalStorage(QObject *parent, const QVariantList&);
        virtual ~LocalStorage();

        virtual void init();

    public slots:
        void startWatching();
        void stopWatching();

        void connectDevice(const QString &uuid);
        void disconnectDevice(const QString &uuid);

        AbstractDevice::List getConnectedDevices();
        DeviceContainer::List getAllDevices();
};

#endif /* LOCALSTORAGE_H_ */
