/*
 * devicesyncview.cpp
 *
 * Copyright (C) 2008 Dario Freddi <drf54321@gmail.com>
 */
#include "devicesyncview.h"
#include "settings.h"

#include <klocale.h>
#include <QtGui/QLabel>

DeviceSyncView::DeviceSyncView(DeviceSync *parent)
        : m_core(parent)
{
    ui_devicesyncview_base.setupUi(this);
    setAutoFillBackground(true);
    ui_devicesyncview_base.moveRightButton->setIcon(KIcon("arrow-right"));
    ui_devicesyncview_base.moveLeftButton->setIcon(KIcon("arrow-left"));

    connect(ui_devicesyncview_base.leftDeviceBox, SIGNAL(currentIndexChanged(const QString&)),
            SLOT(leftDeviceChanged(const QString&)));
    connect(ui_devicesyncview_base.rightDeviceBox, SIGNAL(currentIndexChanged(const QString&)),
            SLOT(rightDeviceChanged(const QString&)));
}

DeviceSyncView::~DeviceSyncView()
{

}

void DeviceSyncView::addDevice(const QString &name, const QVariant &data)
{
    ui_devicesyncview_base.leftDeviceBox->addItem(name, data);
    ui_devicesyncview_base.rightDeviceBox->addItem(name, data);
}

void DeviceSyncView::removeDevice(const QVariant &data)
{
    ui_devicesyncview_base.leftDeviceBox->removeItem(ui_devicesyncview_base.leftDeviceBox->findData(data));
    ui_devicesyncview_base.rightDeviceBox->removeItem(ui_devicesyncview_base.rightDeviceBox->findData(data));
}

void DeviceSyncView::leftDeviceChanged(const QString &name)
{
    kDebug() << "Loading new model";
    AbstractDevice *device = m_core->getConnectedDeviceByName(name);
    ui_devicesyncview_base.leftTreeView->setModel(device->getFileModel());
}

void DeviceSyncView::rightDeviceChanged(const QString &name)
{
    kDebug() << "Loading new model";
    AbstractDevice *device = m_core->getConnectedDeviceByName(name);
    ui_devicesyncview_base.rightTreeView->setModel(device->getFileModel());
}

#include "devicesyncview.moc"
