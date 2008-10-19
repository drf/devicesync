/*
 * AbstractDeviceInterface.h
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#ifndef ABSTRACTDEVICEINTERFACE_H
#define ABSTRACTDEVICEINTERFACE_H

#include <QObject>

class AbstractDeviceInterface : public QObject
{
    public:
        AbstractDeviceInterface();
        virtual ~AbstractDeviceInterface();

        virtual void init() = 0;

    public slots:
        virtual QAbstractItemModel * getFileList() = 0;
};

#endif /* ABSTRACTDEVICEINTERFACE_H */
