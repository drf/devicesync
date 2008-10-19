/*
 * devicesyncview.cpp
 *
 * Copyright (C) 2008 Dario Freddi <drf54321@gmail.com>
 */
#include "devicesyncview.h"
#include "settings.h"

#include <klocale.h>
#include <QtGui/QLabel>

DeviceSyncView::DeviceSyncView(QWidget *)
{
    ui_devicesyncview_base.setupUi(this);
    setAutoFillBackground(true);
    ui_devicesyncview_base.moveRightButton->setIcon(KIcon("arrow-right"));
    ui_devicesyncview_base.moveLeftButton->setIcon(KIcon("arrow-left"));
}

DeviceSyncView::~DeviceSyncView()
{

}

void DeviceSyncView::addDevice(const QString &name, const QVariant &data)
{
    ui_devicesyncview_base.leftDeviceBox->addItem(name, data);
    ui_devicesyncview_base.rightDeviceBox->addItem(name, data);
}

#include "devicesyncview.moc"
