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

#ifndef ABSTRACTDEVICEINTERFACE_H
#define ABSTRACTDEVICEINTERFACE_H

#include <kdemacros.h>
#include <KPluginFactory>
#include <KPluginLoader>

#include "AbstractDevice.h"

/**
 * Convenience macros to export a devicesync plugin. Suppose you created the plugin
 * FooDevice, and its interface is named FooDeviceInterface. In FooDeviceInterface you
 * will have to put the following line:
 *
 * @code
 * DEVICESYNC_PLUGIN_EXPORT(FooDeviceInterface)
 * @endcode
 *
 * The rest will be done for you.
 */
#define DEVICESYNC_PLUGIN_EXPORT( c ) \
    K_PLUGIN_FACTORY( DeviceSyncFactory, registerPlugin< c >(); ) \
    K_EXPORT_PLUGIN( DeviceSyncFactory("c") )

/**
 * The device interface is the main component of the plugin. It takes care of
 * detecting available devices, managing them, and keeping the core up-to-date.
 *
 * Through a deviceinterface, devicesync core is able to get available devices,
 * connected devices and more. It is the connector between the core and the devices.
 *
 * You have to reimplement just few functions in this interface, since almost
 * all the hard work has already been done for you, thanks to abstraction in devices.
 * The only thing you have to do here is reimplement initialization and device detection.
 *
 * Please note that Solid has an entire framework able to detect device hotplugging, and
 * device recognition too. It is multiplatform, scalable and extremely fast, so please
 * consider taking advantage of Solid while writing your plugin, because it's probably worth it.
 * Look at the MTP interface code for an example of Solid usage.
 *
 * @author Dario Freddi
 */
class KDE_EXPORT AbstractDeviceInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)

public:
    /**
     * @internal
     *
     * A list of AbstractDeviceInterface pointers
     */
    typedef QList<AbstractDeviceInterface*> List;

    AbstractDeviceInterface(QObject *parent = 0);
    virtual ~AbstractDeviceInterface();

    /**
     * This function is called only once when the interface gets initialized.
     * Reimplement it if you need to do something special when the interface
     * is activated for the first time (e.g.: loading libraries, etc)
     */
    virtual void init();

    /**
     * Scans the PC for available devices.
     *
     * You have to reimplement this function. When it gets called, the interface should
     * search newly connected devices and add them to the device list.
     *
     * You can use Solid for device recognition, it makes your work definitely easier.
     *
     * @see addDevice
     */
    virtual void scan() = 0;

    /**
     * Returns the name for this interface.
     *
     * @see setName
     */
    QString name() const;

public slots:
    /**
     * Starts watching for device hotplugging.
     *
     * You have to reimplement this function. When it is called, your interface should
     * be able to detect and add devices as they get plugged in (aka hotplugging).
     *
     * Solid can really help you here, though, if your interface does not support device
     * hotplugging, simply do not reimplement this function.
     *
     * @see stopWatching
     */
    virtual void startWatching();
    /**
     * Stops watching for device hotplugging.
     *
     * You have to reimplement this function. When it is called, your interface should
     * stop detecting devices as they get plugged in (aka hotplugging).
     *
     * Solid can really help you here, though, if your interface does not support device
     * hotplugging, simply do not reimplement this function.
     *
     * @see startWatching
     */
    virtual void stopWatching();

    /**
     * Forces a connection to the first available device
     *
     * You have to reimplement this function. When it is called, your interface should
     * attempt to connect to the first available device, and return a reference to it
     * if there was actually an available device, or 0 if no devices were found.
     *
     * @returns a reference to the newly added device or 0 if no device was found
     *
     * @see addDevice
     * @see connectDevice
     */
    virtual AbstractDevice *forceDeviceConnection() = 0;

    /**
     * Returns a list of connected devices managed by this interface
     *
     * @returns a list of connected devices managed by this interface
     */
    AbstractDevice::List getConnectedDevices();
    /**
     * Returns the list of devices currently managed by this interface, both connected and disconnected.
     */
    DeviceContainer::List getAllDevices();

    /**
     * Connects a device. This function calls connectDevice() on the device, updates the
     * associated container, and takes care of informing the GUI when the connection is
     * established. Whenever you need to connect a device, you always need to do it through
     * the interface, and not by calling the connectDevice() method of the device itself.
     *
     * @param device the device a connection was requested for
     */
    void connectDevice(AbstractDevice *device);
    /**
     * Disconnects a device. This function calls disconnectDevice() on the device, updates the
     * associated container, and takes care of informing the GUI . Whenever you need to disconnect
     * a device, you always need to do it through the interface, and not by calling the
     * disconnectDevice() method of the device itself.
     *
     * @param device the device a disconnection was requested for
     */
    void disconnectDevice(AbstractDevice *device);

protected slots:
    /**
     * Adds a device to the list of managed devices.
     *
     * You have to call this function everytime a device is detected through
     * scanning, hotplugging or forced connection. A container will be created for it
     * and the interface will be able to manage it after this call has been made.
     *
     * @param device the new device
     * @param udi the new device's udi
     *
     * @see scan
     * @see startWatching
     * @see forceDeviceConnection
     */
    void addDevice(AbstractDevice *device, const QString &udi = QString());
    /**
     * Removes a device from the list of managed devices.
     *
     * You have to call this function everytime a device is removed.
     * The associated container and all memory will be freed, and the device
     * will no longer be available for managing
     *
     * @warning: after calling this function, the device pointer becomes invalid
     *
     * @param device the device to remove
     */
    void removeDevice(AbstractDevice *device);

protected:
    /**
     * Sets the name for this interface. This name will be displayed in the
     * interface, so be sure to set this name.
     *
     * It usually should be the generic name of devices this interface is managing: for
     * example, if you're managing devices "Foo", you eventually want to set the name
     * for the interface to "Foo Devices". And you probably want to call this function in
     * init(), too.
     *
     * @param newName the name for this interface
     *
     * @see name
     * @see init
     */
    void setName(const QString &newName);

signals:
    void deviceConnected(AbstractDevice *device);
    void deviceDisconnected(AbstractDevice *device);
    void newDeviceRegistered(AbstractDevice *device);
    void deviceRemoved(AbstractDevice *device);

private:
    class Private;
    Private *d;
};

#endif /* ABSTRACTDEVICEINTERFACE_H */
