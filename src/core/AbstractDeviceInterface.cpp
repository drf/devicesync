/***************************************************************************
 *   Copyright (C) 2008 by Dario Freddi <drf@kdemod.ath.cx>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

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

void AbstractDeviceInterface::addDevice(AbstractDevice *device, const QString &udi)
{
    foreach(DeviceContainer *ent, getAllDevices()) {
        if (device == ent->device) {
            return;
        }
    }

    DeviceContainer *cont = new DeviceContainer();

    cont->device = device;
    cont->status = DeviceContainer::Disconnected;
    cont->udi = udi;

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
            cont->device->connectDevice();
            cont->status = DeviceContainer::Connected;
        }
    }

    emit deviceConnected(device);
}

void AbstractDeviceInterface::disconnectDevice(AbstractDevice *device)
{
    foreach(DeviceContainer *cont, d->m_devices) {
        if (cont->device == device) {
            cont->device->disconnectDevice();
            cont->status = DeviceContainer::Disconnected;
        }
    }

    emit deviceDisconnected(device);
}

#include "AbstractDeviceInterface.moc"
