/*
 * devicesync.h
 *
 * Copyright (C) 2008 Dario Freddi <drf54321@gmail.com>
 */
#ifndef DEVICESYNC_H
#define DEVICESYNC_H


#include <kxmlguiwindow.h>
#include <KService>

#include "ui_prefs_base.h"

class DeviceSyncView;
class QPrinter;
class KToggleAction;
class KUrl;
class AbstractDevice;
class AbstractDeviceInterface;
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

    QueueManager *queueManager();

private slots:
    void optionsPreferences();
    bool loadAllPlugins();

    void deviceConnected(AbstractDevice *device);
    void deviceDisconnected(AbstractDevice *device);
    void newDeviceRegistered(AbstractDevice *device);
    void deviceRemoved(AbstractDevice *device);

private:
    void setupActions();
    AbstractDeviceInterface * createPluginFromService(const KService::Ptr &service);

private:
    Ui::prefs_base ui_prefs_base ;

    class Private;
    Private *d;
};

#endif // _DEVICESYNC_H_
