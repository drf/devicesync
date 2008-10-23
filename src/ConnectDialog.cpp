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

#include "ConnectDialog.h"

#include <KDebug>

#include "AbstractDeviceInterface.h"

#include "devicesync.h"

ConnectDialog::ConnectDialog(DeviceSync *handler, QWidget *parent)
 : KDialog(parent),
 m_handler(handler)
{
    setButtons(KDialog::None);

    m_widget = new QWidget();
    ui.setupUi(m_widget);

    ui.cancelButton->setIcon(KIcon("dialog-cancel"));
    ui.connectButton->setIcon(KIcon("network-connect"));
    ui.scanButton->setIcon(KIcon("system-search"));

    connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(deleteLater()));
    connect(ui.scanButton, SIGNAL(clicked()), this, SLOT(rescanDevices()));
    connect(ui.connectButton, SIGNAL(clicked()), this, SLOT(connectDevice()));

    setMainWidget(m_widget);
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);
    show();


}

ConnectDialog::~ConnectDialog()
{
}

int ConnectDialog::getNextId()
{
    int id = 0;

    foreach (int i, m_devices.keys()) {
        if (i >= id) {
            id = i + 1;
        }
    }

    return id;
}

void ConnectDialog::refreshList()
{
    ui.listWidget->clear();

    foreach (AbstractDevice *device, m_handler->getAvailableDisconnectedDevices()) {
        QListWidgetItem *itm = new QListWidgetItem(ui.listWidget);
        int id = getNextId();

        itm->setText(device->name());
        itm->setIcon(device->icon());
        itm->setData(40, id);

        m_devices[id] = device;
    }
}

void ConnectDialog::rescanDevices()
{
    m_handler->requestDeviceScan();

    refreshList();
}

void ConnectDialog::connectDevice()
{
    AbstractDevice *device = m_devices[ui.listWidget->currentItem()->data(40).toInt()];
    device->interface()->connectDevice(device);
}

#include "ConnectDialog.moc"
