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

#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <KDialog>
#include "ui_progressdialog.h"

#include "ProgressInterface.h"

class QueueItem;

class ProgressDialog : public KDialog
{
    Q_OBJECT

public:
    ProgressDialog(ProgressInterface *iface, QWidget *parent = 0);
    virtual ~ProgressDialog();

private slots:
    void currentItemChanged(QueueItem *item);
    void totalProgressChanged(int percent);
    void currentItemProgressChanged(ProgressInterface::Action action, int percent);

private:
    QWidget *m_widget;
    Ui_progressDialog ui;
    ProgressInterface *m_interface;
};

#endif /* PROGRESSDIALOG_H */
