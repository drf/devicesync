/*
 * devicesyncview.h
 *
 * Copyright (C) 2007 Dario Freddi <drf54321@gmail.com>
 */
#ifndef DEVICESYNCVIEW_H
#define DEVICESYNCVIEW_H

#include <QtGui/QWidget>

#include "devicesync.h"

#include "ui_devicesyncview_base.h"

class QPainter;
class KUrl;

/**
 * This is the main view class for DeviceSync.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 * @short Main view
 * @author Dario Freddi <drf54321@gmail.com>
 * @version 0.1
 */

class DeviceSyncView : public QWidget, public Ui::devicesyncview_base
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    DeviceSyncView(DeviceSync *parent);

    /**
     * Destructor
     */
    virtual ~DeviceSyncView();

    void addDevice(const QString &name, const QVariant &data);
    void removeDevice(const QVariant &data);

private slots:
    void leftDeviceChanged(const QString &name);
    void rightDeviceChanged(const QString &name);

    void addToQueueFromLeft();
    void addToQueueFromRight();

private:
    Ui::devicesyncview_base ui_devicesyncview_base;
    DeviceSync *m_core;
    AbstractDevice *m_leftDevice;
    AbstractDevice *m_rightDevice;

signals:
    /**
     * Use this signal to change the content of the statusbar
     */
    void signalChangeStatusbar(const QString& text);

    /**
     * Use this signal to change the content of the caption
     */
    void signalChangeCaption(const QString& text);

};

#endif // DeviceSyncVIEW_H
