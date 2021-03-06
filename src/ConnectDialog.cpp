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

#include <QMovie>
#include <QTimer>

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
    connect(ui.manualButton, SIGNAL(clicked()), this, SLOT(manualConnection()));
    connect(ui.scanButton, SIGNAL(clicked()), this, SLOT(rescanDevices()));
    connect(ui.connectButton, SIGNAL(clicked()), this, SLOT(connectDevice()));

    setMainWidget(m_widget);
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);
    show();

    refreshList();
}

ConnectDialog::~ConnectDialog()
{
}

int ConnectDialog::getNextId()
{
    int id = 0;

    foreach(int i, m_devices.keys()) {
        if (i >= id) {
            id = i + 1;
        }
    }

    return id;
}

void ConnectDialog::refreshList()
{
    ui.listWidget->clear();

    foreach(AbstractDevice *device, m_handler->getAvailableDisconnectedDevices()) {
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
    connect(device, SIGNAL(deviceConnected(AbstractDevice*)), this, SLOT(deviceConnected(AbstractDevice*)));

    device->interface()->connectDevice(device);

    m_widget->deleteLater();

    m_widget = new QWidget();
    setMainWidget(m_widget);

    QVBoxLayout *ly = new QVBoxLayout();
    QLabel *text = new QLabel();
    QLabel *icon = new QLabel();

    text->setText(i18n("Please wait, connecting %1...", device->name()));
    text->setAlignment(Qt::AlignHCenter);
    QMovie *movie = KIconLoader::global()->loadMovie("process-working", KIconLoader::Dialog);
    icon->setMovie(movie);

    if (movie) {
        movie->start();
    }

    icon->setAlignment(Qt::AlignHCenter);

    ly->addStretch();
    ly->addWidget(text);
    ly->addWidget(icon);
    ly->addStretch();

    m_widget->setLayout(ly);
}

void ConnectDialog::deviceConnected(AbstractDevice *device)
{
    m_widget->deleteLater();

    m_widget = new QWidget();
    setMainWidget(m_widget);

    QVBoxLayout *ly = new QVBoxLayout();
    QLabel *text = new QLabel();

    text->setText(i18n("%1 was successfully connected!", device->name()));
    text->setAlignment(Qt::AlignHCenter);

    ly->addStretch();
    ly->addWidget(text);
    ly->addStretch();

    m_widget->setLayout(ly);

    QTimer::singleShot(2000, this, SLOT(deleteLater()));
}

void ConnectDialog::manualConnection()
{
    m_widget->deleteLater();
    m_widget = new QWidget();

    force_ui.setupUi(m_widget);

    force_ui.continueButton->setIcon(KIcon("dialog-ok-apply"));

    force_ui.deviceBox->clear();

    foreach(AbstractDeviceInterface *iface, m_handler->getAvailableInterfaces()) {
        force_ui.deviceBox->addItem(iface->name());
    }

    connect(force_ui.continueButton, SIGNAL(clicked()), this, SLOT(forceConnection()));

    setMainWidget(m_widget);
}

void ConnectDialog::forceConnection()
{
    AbstractDeviceInterface *iface = 0;

    foreach(AbstractDeviceInterface *ent, m_handler->getAvailableInterfaces()) {
        if (ent->name() == force_ui.deviceBox->currentText()) {
            iface = ent;
        }
    }

    if (!iface) {
        return;
    }

    AbstractDevice *device = iface->forceDeviceConnection();
    connect(device, SIGNAL(deviceConnected(AbstractDevice*)), this, SLOT(deviceConnected(AbstractDevice*)));

    m_widget->deleteLater();

    m_widget = new QWidget();
    setMainWidget(m_widget);

    QVBoxLayout *ly = new QVBoxLayout();
    QLabel *text = new QLabel();
    QLabel *icon = new QLabel();

    text->setText(i18n("Please wait, connecting %1...", device->name()));
    text->setAlignment(Qt::AlignHCenter);
    QMovie *movie = KIconLoader::global()->loadMovie("process-working", KIconLoader::Dialog);
    icon->setMovie(movie);

    if (movie) {
        movie->start();
    }

    icon->setAlignment(Qt::AlignHCenter);

    ly->addStretch();
    ly->addWidget(text);
    ly->addWidget(icon);
    ly->addStretch();

    m_widget->setLayout(ly);
}

#include "ConnectDialog.moc"
