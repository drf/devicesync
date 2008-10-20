/*
 * AbstractDeviceInterface.cpp
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#include "AbstractDeviceInterface.h"

class AbstractDeviceInterface::Private
{
public:
    Private() {
    }
    ~Private() {}


    DeviceContainer::List m_devices;
};

AbstractDeviceInterface::AbstractDeviceInterface(QObject *parent)
        : QObject(parent),
        d(new Private())
{
}

AbstractDeviceInterface::~AbstractDeviceInterface()
{
    delete d;
}

AbstractDevice::List AbstractDeviceInterface::getConnectedDevices()
{
    AbstractDevice::List retlist;

    foreach(DeviceContainer *ent, d->m_devices) {
        if (ent->status == DeviceContainer::Connected) {
            retlist.append(ent->device);
        }
    }

    return retlist;
}

DeviceContainer::List AbstractDeviceInterface::getAllDevices()
{
    return d->m_devices;
}

void AbstractDeviceInterface::addDevice(AbstractDevice *device)
{
    DeviceContainer *cont = new DeviceContainer();

    cont->device = device;
    cont->status = DeviceContainer::Disconnected;

    d->m_devices.append(cont);

    emit newDeviceRegistered(device);
}

void AbstractDeviceInterface::removeDevice(AbstractDevice *device)
{
    foreach(DeviceContainer *cont, d->m_devices) {
        if (cont->device == device) {
            d->m_devices.removeOne(cont);
            cont->device->deleteLater();
            delete cont;
        }
    }

    emit deviceRemoved(device);
}

void AbstractDeviceInterface::connectDevice(AbstractDevice *device)
{
    foreach(DeviceContainer *cont, d->m_devices) {
        if (cont->device == device) {
            cont->status = DeviceContainer::Connected;
        }
    }

    emit deviceConnected(device);
}

void AbstractDeviceInterface::disconnectDevice(AbstractDevice *device)
{
    foreach(DeviceContainer *cont, d->m_devices) {
        if (cont->device == device) {
            cont->status = DeviceContainer::Disconnected;
        }
    }

    emit deviceDisconnected(device);
}

#include "AbstractDeviceInterface.moc"
