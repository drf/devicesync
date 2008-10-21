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

#ifndef PROGRESSINTERFACE_H
#define PROGRESSINTERFACE_H

#include <QObject>

#include "QueueManager.h"

class ProgressInterface : public QObject
{
    Q_OBJECT

public:
    enum Action {
        Getting,
        Sending,
        Deleting,
        Renaming
    };

    ProgressInterface(QObject *parent = 0);
    virtual ~ProgressInterface();

public slots:
    void setItems(QueueItem::List list);
    void setCurrentItem(QueueItem *item);

    void setCurrentItemProgress(int percent);
    void setCurrentItemAction(Action action);

signals:
    void currentItemChanged(QueueItem *item);
    void totalProgressChanged(int percent);
    void currentItemProgressChanged(Action action, int percent);

private:
    class Private;
    Private *d;
};

#endif /* PROGRESSINTERFACE_H */
