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

#include "QueueManager.h"

#include <QMap>
#include <KDebug>

class QueueManager::Private
{
public:
    Private() {
    }
    ~Private() {}

    QueueItem::List itemList;
    QueueItem::List::iterator iterator;
    QMap<int, QueueItem*> tokenActions;
};

QueueManager::QueueManager(QObject *parent)
        : QObject(parent),
        d(new Private())
{

}

QueueManager::~QueueManager()
{
    delete d;
}

int QueueManager::addJobToQueue(QueueItem::Action action, AbstractDevice *in, const QString &inpath,
                                AbstractDevice *out, const QString &outpath)
{
    QueueItem *item = new QueueItem;

    item->action = action;
    item->in_device = in;
    item->in_path = inpath;
    item->out_device = out;
    item->out_path = outpath;
    item->jobId = generateItemId();

    d->itemList.append(item);

    kDebug() << "Added to queue: copy" << inpath << "to" << outpath << ", job ID: " << item->jobId;

    return item->jobId;
}

int QueueManager::generateItemId()
{
    int new_id = 0;

    foreach(QueueItem *itm, d->itemList) {
        if (itm->jobId >= new_id) {
            new_id = itm->jobId + 1;
        }
    }

    return new_id;
}

void QueueManager::removeJobFromQueue(int jobId)
{
    foreach(QueueItem *itm, d->itemList) {
        if (itm->jobId == jobId) {
            d->itemList.removeOne(itm);
            delete itm;
        }
    }
}

void QueueManager::clearQueue()
{
    while (!d->itemList.isEmpty()) {
        QueueItem *itm = d->itemList.takeFirst();
        delete itm;
    }
}

void QueueManager::processQueue()
{
    d->iterator = d->itemList.begin();

    processNextQueueItem();
}

void QueueManager::processNextQueueItem()
{
    if (d->iterator == d->itemList.end()) {
        kDebug() << "Queue ended";
        clearQueue();
        return;
    }

    kDebug() << "Processing next queue item";

    int token;

    switch ((*d->iterator)->action) {
    case QueueItem::Copy:
    case QueueItem::Move:
        token = (*d->iterator)->in_device->getFileFromDevice((*d->iterator)->in_path, "/tmp");

        d->tokenActions[token] = (*d->iterator);

        connect((*d->iterator)->in_device, SIGNAL(fileCopiedFromDevice(int, const QString&)),
                this, SLOT(fileCopiedFromDevice(int, const QString&)));
        break;
    default:
        break;
    }

}

void QueueManager::fileCopiedFromDevice(int token, const QString &filePath)
{
    int new_token;

    switch (d->tokenActions[token]->action) {
    case QueueItem::Copy:
        disconnect(d->tokenActions[token]->in_device, SIGNAL(fileCopiedFromDevice(int, const QString&)),
                   this, SLOT(fileCopiedFromDevice(int, const QString&)));

        new_token = d->tokenActions[token]->out_device->sendFileToDevice(filePath, d->tokenActions[token]->out_path);

        d->tokenActions[new_token] = d->tokenActions[token];

        connect(d->tokenActions[new_token]->out_device, SIGNAL(fileCopiedToDevice(int, const QString&)),
                this, SLOT(fileCopiedToDevice(int, const QString&)));
        break;
    case QueueItem::Move:
        disconnect(d->tokenActions[token]->in_device, SIGNAL(fileCopiedFromDevice(int, const QString&)),
                   this, SLOT(fileCopiedFromDevice(int, const QString&)));

        //d->tokenActions[token]->in_device->removeFile(d->tokenActions[token]->in_path);

        new_token = d->tokenActions[token]->out_device->sendFileToDevice(filePath, d->tokenActions[token]->out_path);

        d->tokenActions[new_token] = d->tokenActions[token];

        connect(d->tokenActions[new_token]->out_device, SIGNAL(fileCopiedToDevice(int, const QString&)),
                this, SLOT(fileCopiedToDevice(int, const QString&)));
        break;
    default:
        break;
    }
}

void QueueManager::fileCopiedToDevice(int token, const QString &filePath)
{
    int new_token;

    switch (d->tokenActions[token]->action) {
    case QueueItem::Copy:
        kDebug() << "Copy completed!!";
        ++d->iterator;
        processNextQueueItem();
    default:
        break;
    }
}

#include "QueueManager.moc"
