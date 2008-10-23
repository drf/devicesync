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

#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include "ui_connectdialog.h"

#include <KDialog>

#include "AbstractDevice.h"

class DeviceSync;

class ConnectDialog : public KDialog
{
    Q_OBJECT

    public:
        explicit ConnectDialog(DeviceSync *handler, QWidget *parent = 0);
        virtual ~ConnectDialog();

    private slots:
        void refreshList();
        void rescanDevices();

    private:
        QWidget *m_widget;
        Ui_connectDialog ui;
        DeviceSync *m_handler;
};

Q_DECLARE_METATYPE(AbstractDevice*)

#endif /* CONNECTDIALOG_H */
