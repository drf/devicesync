/*
 * devicesync.h
 *
 * Copyright (C) 2008 Dario Freddi <drf54321@gmail.com>
 */
#ifndef DEVICESYNC_H
#define DEVICESYNC_H


#include <kxmlguiwindow.h>

#include "ui_prefs_base.h"

class DeviceSyncView;
class QPrinter;
class KToggleAction;
class KUrl;

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

private slots:
    void fileNew();
    void optionsPreferences();

private:
    void setupActions();

private:
    Ui::prefs_base ui_prefs_base ;
    DeviceSyncView *m_view;

    QPrinter   *m_printer;
    KToggleAction *m_toolbarAction;
    KToggleAction *m_statusbarAction;
};

#endif // _DEVICESYNC_H_
