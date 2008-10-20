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

#include "devicesyncview.h"
#include "settings.h"

#include "AbstractDeviceInterface.h"
#include "QueueManager.h"

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
    connect(ui_devicesyncview_base.moveRightButton, SIGNAL(clicked()),
            this, SLOT(addToQueueFromLeft()));
    connect(ui_devicesyncview_base.moveLeftButton, SIGNAL(clicked()),
            this, SLOT(addToQueueFromRight()));
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
    m_leftDevice = device;
}

void DeviceSyncView::rightDeviceChanged(const QString &name)
{
    kDebug() << "Loading new model";
    AbstractDevice *device = m_core->getConnectedDeviceByName(name);
    ui_devicesyncview_base.rightTreeView->setModel(device->getFileModel());
    m_rightDevice = device;
}

void DeviceSyncView::addToQueueFromLeft()
{
    m_core->queueManager()->addJobToQueue(QueueItem::Copy, m_leftDevice,
                                          m_leftDevice->getPathForCurrentIndex(ui_devicesyncview_base.leftTreeView->currentIndex()),
                                          m_rightDevice,
                                          m_rightDevice->getPathForCurrentIndex(ui_devicesyncview_base.rightTreeView->currentIndex()));
}

void DeviceSyncView::addToQueueFromRight()
{
    m_core->queueManager()->addJobToQueue(QueueItem::Copy, m_rightDevice,
                                          m_rightDevice->getPathForCurrentIndex(ui_devicesyncview_base.rightTreeView->currentIndex()),
                                          m_leftDevice,
                                          m_leftDevice->getPathForCurrentIndex(ui_devicesyncview_base.leftTreeView->currentIndex()));
}

#include "devicesyncview.moc"
