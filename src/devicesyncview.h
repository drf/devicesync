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

#ifndef DEVICESYNCVIEW_H
#define DEVICESYNCVIEW_H

#include <QtGui/QWidget>

#include "devicesync.h"

#include "ui_devicesyncview_base.h"

class QPainter;
class KUrl;
class KIcon;

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

    void addDevice(const KIcon &icon, const QString &name, const QVariant &data);
    void removeDevice(const QVariant &data);

    void disconnectAll();

private slots:
    void leftDeviceChanged(const QString &name);
    void rightDeviceChanged(const QString &name);

    void addToQueueFromLeft();
    void addToQueueFromRight();

    void showQueueWidgetContextMenu();
    void removeSelectedActions();

    void clearQueue();
    void clearQueueView();

    void setLeftModel(QAbstractItemModel *model, AbstractDevice *device);
    void setRightModel(QAbstractItemModel *model, AbstractDevice *device);

    void showLeftViewContextMenu();
    void showRightViewContextMenu();

    void createNewFolderLeft();
    void createNewFolderRight();

    void deleteFileLeft();
    void deleteFileRight();

    void renameObjectLeft();
    void renameObjectRight();

private:
    QString getNewFolderNameDialog();
    QString getRenameDialog(const QString &oldName);

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
