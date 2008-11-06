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

#ifndef DEVICESYNC_H
#define DEVICESYNC_H


#include <kxmlguiwindow.h>
#include <KService>

#include "AbstractDevice.h"
#include "AbstractDeviceInterface.h"

#include "ui_prefs_base.h"

class DeviceSyncView;
class QPrinter;
class KToggleAction;
class KUrl;
class QueueManager;

/**
 * This class serves as the main window for DeviceSync.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author Dario Freddi <drf54321@gmail.com>
 * @version 0.1
 */
class DeviceSync : public KXmlGuiWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    DeviceSync();

    /**
     * Default Destructor
     */
    virtual ~DeviceSync();

    AbstractDevice *getConnectedDeviceByName(const QString &name);
    AbstractDevice::List getAvailableDisconnectedDevices();
    AbstractDeviceInterface::List getAvailableInterfaces();

    QueueManager *queueManager();

public slots:
    void requestDeviceScan();

private slots:
    void optionsPreferences();
    void loadAllPlugins();

    void deviceConnected(AbstractDevice *device);
    void deviceDisconnected(AbstractDevice *device);
    void newDeviceRegistered(AbstractDevice *device);
    void deviceRemoved(AbstractDevice *device);

    void connectDevice();
    void connectAllDevices();

    void processQueue();

    void disconnectAllDevices();

private:
    void setupActions();
    AbstractDeviceInterface * createPluginFromService(const KService::Ptr &service);

private:
    Ui::prefs_base ui_prefs_base ;

    class Private;
    Private *d;
};

#endif // _DEVICESYNC_H_
