/*
 * AbstractDevice.h
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#ifndef ABSTRACTDEVICE_H
#define ABSTRACTDEVICE_H

#include <QList>

class AbstractDevice
{
    public:
        typedef QList<AbstractDevice*> List;

        AbstractDevice();
        virtual ~AbstractDevice();
};

class DeviceContainer
{
    public:
        typedef QList<DeviceContainer*> List;

        enum Status {
            Connected,
            Disconnected
        };

        AbstractDevice *device;
        Status status;
};

Q_DECLARE_METATYPE(AbstractDevice)

#endif /* ABSTRACTDEVICE_H_ */
