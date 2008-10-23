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

#include "MtpInterface.h"

#include "MtpDevice.h"

#include <KDebug>

#include <libmtp.h>

//solid specific includes
#include <solid/devicenotifier.h>
#include <solid/device.h>
#include <solid/storageaccess.h>
#include <solid/storagedrive.h>
#include <solid/portablemediaplayer.h>

DEVICESYNC_PLUGIN_EXPORT(MtpInterface)

MtpInterface::MtpInterface(QObject *parent, const QVariantList&)
        : AbstractDeviceInterface(parent)
{
}

MtpInterface::~MtpInterface()
{
}

void MtpInterface::init()
{
    kDebug() << "Initializing MTP stuff";
    LIBMTP_Init();
}

void MtpInterface::scan()
{
    foreach(const Solid::Device &device, Solid::Device::listFromType(Solid::DeviceInterface::PortableMediaPlayer)) {
        const Solid::PortableMediaPlayer *pmp = device.as<Solid::PortableMediaPlayer>();

        foreach(QString protocol, pmp->supportedProtocols()) {
            if (protocol == "mtp") {
                kDebug() << "MTP device detected!";

                MtpDevice *mtpdevice = new MtpDevice(device.udi(), this);

                addDevice(mtpdevice);
            }
        }
    }
}

bool MtpInterface::isMtp(const QString &udi)
{
    Solid::Device device;

    device = Solid::Device(udi);
    if (!device.is<Solid::PortableMediaPlayer>()) {
        return false;
    }

    Solid::PortableMediaPlayer *pmp = device.as<Solid::PortableMediaPlayer>();

    foreach(QString protocol, pmp->supportedProtocols()) {
        if (protocol == "mtp") {
            kDebug() << "MTP device detected!";
            return true;
        }
    }

    return false;
}

void MtpInterface::startWatching()
{
    kDebug() << "Watching for Mtp devices";

    connect(Solid::DeviceNotifier::instance(), SIGNAL(deviceAdded(const QString &)),
            this, SLOT(deviceAdded(const QString &)));
    connect(Solid::DeviceNotifier::instance(), SIGNAL(deviceRemoved(const QString &)),
            this, SLOT(deviceRemoved(const QString &)));
}

void MtpInterface::stopWatching()
{
    disconnect(Solid::DeviceNotifier::instance(), SIGNAL(deviceAdded(const QString &)),
               this, SLOT(deviceAdded(const QString &)));
    disconnect(Solid::DeviceNotifier::instance(), SIGNAL(deviceRemoved(const QString &)),
               this, SLOT(deviceRemoved(const QString &)));
}

void MtpInterface::deviceAdded(const QString &udi)
{
    kDebug() << "Added a device";

    if (isMtp(udi)) {
        MtpDevice *device = new MtpDevice(udi, this);

        addDevice(device);
    }
}

void MtpInterface::deviceRemoved(const QString &udi)
{
    kDebug() << "Removed a device";

    if (isMtp(udi)) {
        kDebug() << "Removing an MTP device!";
        foreach(DeviceContainer *cont, getAllDevices()) {
            if (cont->udi == udi) {
                kDebug() << "Removing a managed MTP device!";
                removeDevice(cont->device);
            }
        }
    }
}

#include "MtpInterface.moc"
