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

#include "ProgressInterface.h"

class ProgressInterface::Private
{
public:
    Private() {
    }
    ~Private() {}

    QueueItem::List itemList;
    QueueItem *current;
    ProgressInterface::Action action;
    int action_percent;
};

ProgressInterface::ProgressInterface(QObject *parent)
        : QObject(parent),
        d(new Private())
{
}

ProgressInterface::~ProgressInterface()
{
}

void ProgressInterface::setCurrentItem(QueueItem *item)
{
    d->current = item;

    emit currentItemChanged(item);

    int pos = 1;

    for (QueueItem::List::iterator i = d->itemList.begin(); i != d->itemList.end(); ++i) {
        if (*i == item) {
            emit totalProgressChanged((int)((float)((float)pos / (float)d->itemList.count()) * 100));
        }
    }
}

void ProgressInterface::setCurrentItemProgress(int percent)
{
    d->action_percent = percent;
    emit currentItemProgressChanged(d->action, d->action_percent);
}

void ProgressInterface::setCurrentItemAction(Action action)
{
    d->action = action;
    d->action_percent = 0;
    emit currentItemProgressChanged(d->action, d->action_percent);
}

void ProgressInterface::setItems(QueueItem::List list)
{
    d->itemList = list;
    emit totalProgressChanged(0);
}

#include "ProgressInterface.moc"
