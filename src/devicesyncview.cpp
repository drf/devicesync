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
#include <QMenu>

#include <KDialog>
#include <KLineEdit>

DeviceSyncView::DeviceSyncView(DeviceSync *parent)
        : m_core(parent),
        m_leftDevice(0),
        m_rightDevice(0)
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
    connect(ui_devicesyncview_base.listWidget, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showQueueWidgetContextMenu()));
    connect(ui_devicesyncview_base.leftTreeView, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showLeftViewContextMenu()));
    connect(ui_devicesyncview_base.rightTreeView, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showRightViewContextMenu()));
}

DeviceSyncView::~DeviceSyncView()
{

}

void DeviceSyncView::showQueueWidgetContextMenu()
{
    if (ui_devicesyncview_base.listWidget->selectedItems().isEmpty())
        return;

    QMenu *menu = new QMenu(this);

    QAction *removeAction = menu->addAction(KIcon("edit-delete"),
                                            i18n("&Remove from Queue"));
    connect(removeAction, SIGNAL(triggered()), SLOT(removeSelectedActions()));

    menu->addSeparator();

    QAction *clearAction = menu->addAction(KIcon("edit-clear"),
                                           i18n("&Clear Queue"));
    connect(clearAction, SIGNAL(triggered()), SLOT(clearQueue()));

    menu->popup(QCursor::pos());
}

void DeviceSyncView::addDevice(const KIcon &icon, const QString &name, const QVariant &data)
{
    ui_devicesyncview_base.leftDeviceBox->addItem(icon, name, data);
    ui_devicesyncview_base.rightDeviceBox->addItem(icon, name, data);
}

void DeviceSyncView::removeDevice(const QVariant &data)
{
    ui_devicesyncview_base.leftDeviceBox->removeItem(ui_devicesyncview_base.leftDeviceBox->findData(data));
    ui_devicesyncview_base.rightDeviceBox->removeItem(ui_devicesyncview_base.rightDeviceBox->findData(data));
}

void DeviceSyncView::leftDeviceChanged(const QString &name)
{
    if (m_leftDevice) {
        disconnect(m_leftDevice, SIGNAL(modelChanged(QAbstractItemModel*, AbstractDevice*)),
                   this, SLOT(setLeftModel(QAbstractItemModel*, AbstractDevice*)));
    }

    kDebug() << "Loading new model";
    AbstractDevice *device = m_core->getConnectedDeviceByName(name);
    m_leftDevice = device;
    connect(m_leftDevice, SIGNAL(modelChanged(QAbstractItemModel*, AbstractDevice*)),
            this, SLOT(setLeftModel(QAbstractItemModel*, AbstractDevice*)));
    m_leftDevice->reloadModel();
}

void DeviceSyncView::setLeftModel(QAbstractItemModel *, AbstractDevice *device)
{
    ui_devicesyncview_base.leftTreeView->setModel(device->model());
}

void DeviceSyncView::rightDeviceChanged(const QString &name)
{
    if (m_rightDevice) {
        disconnect(m_rightDevice, SIGNAL(modelChanged(QAbstractItemModel*, AbstractDevice*)),
                   this, SLOT(setRightModel(QAbstractItemModel*, AbstractDevice*)));
    }

    kDebug() << "Loading new model";
    AbstractDevice *device = m_core->getConnectedDeviceByName(name);
    ui_devicesyncview_base.rightTreeView->setModel(device->model());
    m_rightDevice = device;
    connect(m_rightDevice, SIGNAL(modelChanged(QAbstractItemModel*, AbstractDevice*)),
            this, SLOT(setRightModel(QAbstractItemModel*, AbstractDevice*)));
    m_rightDevice->reloadModel();
}

void DeviceSyncView::setRightModel(QAbstractItemModel *, AbstractDevice *device)
{
    ui_devicesyncview_base.rightTreeView->setModel(device->model());
}

void DeviceSyncView::addToQueueFromLeft()
{
    QStringList paths;
    foreach(const QModelIndex &index, ui_devicesyncview_base.leftTreeView->selectionModel()->selectedIndexes()) {
        if (paths.contains(m_leftDevice->getPathForCurrentIndex(index))) {
            continue;
        }

        paths.append(m_leftDevice->getPathForCurrentIndex(index));

        int token =
            m_core->queueManager()->addJobToQueue(QueueItem::Copy, m_leftDevice,
                                                  m_leftDevice->getPathForCurrentIndex(index),
                                                  m_rightDevice,
                                                  m_rightDevice->getPathForCurrentIndex(ui_devicesyncview_base.rightTreeView->currentIndex()));

        QListWidgetItem *itm = new QListWidgetItem(KIcon("edit-copy"), i18n("Copy %1 from %2 to %3 in %4",
                m_leftDevice->getPathForCurrentIndex(index),
                m_leftDevice->name(), m_rightDevice->name(),
                m_rightDevice->getPathForCurrentIndex(ui_devicesyncview_base.rightTreeView->currentIndex())),
                ui_devicesyncview_base.listWidget);

        itm->setData(40, token);
    }
}

void DeviceSyncView::addToQueueFromRight()
{
    QStringList paths;
    foreach(const QModelIndex &index, ui_devicesyncview_base.rightTreeView->selectionModel()->selectedIndexes()) {
        if (paths.contains(m_rightDevice->getPathForCurrentIndex(index))) {
            continue;
        }

        paths.append(m_leftDevice->getPathForCurrentIndex(index));

        int token =
            m_core->queueManager()->addJobToQueue(QueueItem::Copy, m_rightDevice,
                                                  m_rightDevice->getPathForCurrentIndex(index),
                                                  m_leftDevice,
                                                  m_leftDevice->getPathForCurrentIndex(ui_devicesyncview_base.leftTreeView->currentIndex()));

        QListWidgetItem *itm = new QListWidgetItem(KIcon("edit-copy"), i18n("Copy %1 from %2 to %3 in %4",
                m_rightDevice->getPathForCurrentIndex(index),
                m_rightDevice->name(), m_leftDevice->name(),
                m_leftDevice->getPathForCurrentIndex(ui_devicesyncview_base.leftTreeView->currentIndex())),
                ui_devicesyncview_base.listWidget);

        itm->setData(40, token);
    }
}

void DeviceSyncView::removeSelectedActions()
{
    foreach(QListWidgetItem *itm, ui_devicesyncview_base.listWidget->selectedItems()) {
        m_core->queueManager()->removeJobFromQueue(itm->data(40).toInt());
        delete itm;
    }
}

void DeviceSyncView::clearQueue()
{
    m_core->queueManager()->clearQueue();
    ui_devicesyncview_base.listWidget->clear();
}

void DeviceSyncView::showLeftViewContextMenu()
{
    if (ui_devicesyncview_base.leftTreeView->selectionModel()->selectedIndexes().isEmpty()) {
        return;
    }

    QMenu *menu = new QMenu(this);

    QAction *removeAction = menu->addAction(KIcon("folder-new"),
                                            i18n("Create &Folder"));
    connect(removeAction, SIGNAL(triggered()), SLOT(createNewFolderLeft()));

    menu->addSeparator();

    QAction *clearAction = menu->addAction(KIcon("edit-delete"),
                                           i18n("&Delete"));
    connect(clearAction, SIGNAL(triggered()), SLOT(deleteFileLeft()));

    menu->popup(QCursor::pos());
}

void DeviceSyncView::showRightViewContextMenu()
{
    if (ui_devicesyncview_base.rightTreeView->selectionModel()->selectedIndexes().isEmpty()) {
        return;
    }

    QMenu *menu = new QMenu(this);

    QAction *removeAction = menu->addAction(KIcon("folder-new"),
                                            i18n("Create &Folder"));
    connect(removeAction, SIGNAL(triggered()), SLOT(createNewFolderRight()));

    menu->addSeparator();

    QAction *clearAction = menu->addAction(KIcon("edit-delete"),
                                           i18n("&Delete File"));
    connect(clearAction, SIGNAL(triggered()), SLOT(deleteFileRight()));

    menu->popup(QCursor::pos());
}

QString DeviceSyncView::getNewFolderNameDialog()
{
    KDialog *dlog = new KDialog(this);
    QWidget *wg = new QWidget();
    QLabel *lb = new QLabel(i18n("Please enter the name for the new folder:"));
    KLineEdit *ed = new KLineEdit();
    QVBoxLayout *ly = new QVBoxLayout();

    ly->addWidget(lb);
    ly->addWidget(ed);

    wg->setLayout(ly);

    dlog->setMainWidget(wg);

    dlog->setModal(true);

    if (dlog->exec() == KDialog::Accepted) {
        return ed->text();
    } else {
        return QString();
    }
}

void DeviceSyncView::createNewFolderLeft()
{
    QString name = getNewFolderNameDialog();

    if (name.isEmpty()) {
        return;
    }

    m_leftDevice->createFolder(name,
                               m_leftDevice->getPathForCurrentIndex(ui_devicesyncview_base.leftTreeView->selectionModel()->selectedIndexes().at(0)));

    m_leftDevice->reloadModel();
}

void DeviceSyncView::createNewFolderRight()
{
    QString name = getNewFolderNameDialog();

    if (name.isEmpty()) {
        return;
    }

    m_rightDevice->createFolder(name,
                                m_rightDevice->getPathForCurrentIndex(ui_devicesyncview_base.rightTreeView->selectionModel()->selectedIndexes().at(0)));

    m_rightDevice->reloadModel();
}

void DeviceSyncView::deleteFileLeft()
{
    QStringList paths;
    foreach(const QModelIndex &index, ui_devicesyncview_base.leftTreeView->selectionModel()->selectedIndexes()) {
        if (paths.contains(m_leftDevice->getPathForCurrentIndex(index))) {
            continue;
        }

        paths.append(m_leftDevice->getPathForCurrentIndex(index));

        int token =
            m_core->queueManager()->addJobToQueue(QueueItem::Delete, m_leftDevice,
                                                  m_leftDevice->getPathForCurrentIndex(index));

        QListWidgetItem *itm = new QListWidgetItem(KIcon("edit-copy"), i18n("Delete %1 from %2",
                m_leftDevice->getPathForCurrentIndex(index),
                m_leftDevice->name()),
                ui_devicesyncview_base.listWidget);

        itm->setData(40, token);
    }
}

void DeviceSyncView::deleteFileRight()
{
    QStringList paths;
    foreach(const QModelIndex &index, ui_devicesyncview_base.rightTreeView->selectionModel()->selectedIndexes()) {
        if (paths.contains(m_rightDevice->getPathForCurrentIndex(index))) {
            continue;
        }

        paths.append(m_rightDevice->getPathForCurrentIndex(index));

        int token =
            m_core->queueManager()->addJobToQueue(QueueItem::Delete, m_rightDevice,
                                                  m_rightDevice->getPathForCurrentIndex(index));

        QListWidgetItem *itm = new QListWidgetItem(KIcon("edit-copy"), i18n("Delete %1 from %2",
                m_rightDevice->getPathForCurrentIndex(index),
                m_rightDevice->name()),
                ui_devicesyncview_base.listWidget);

        itm->setData(40, token);
    }
}

void DeviceSyncView::disconnectAll()
{
    disconnect(ui_devicesyncview_base.leftDeviceBox, SIGNAL(currentIndexChanged(const QString&)),
               this, SLOT(leftDeviceChanged(const QString&)));
    disconnect(ui_devicesyncview_base.rightDeviceBox, SIGNAL(currentIndexChanged(const QString&)),
               this, SLOT(rightDeviceChanged(const QString&)));
}

#include "devicesyncview.moc"
