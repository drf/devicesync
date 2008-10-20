/*
 * QueueManager.cpp
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#include "QueueManager.h"

class QueueManager::Private
{
public:
    Private()
    {
    }
    ~Private() {}

    QueueItem::List itemList;
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

    return item->jobId;
}

int QueueManager::generateItemId()
{
    int new_id = 0;

    foreach (QueueItem *itm, d->itemList)
    {
        if (itm->jobId >= new_id) {
            new_id = itm->jobId + 1;
        }
    }

    return new_id;
}

void QueueManager::removeJobFromQueue(int jobId)
{
    foreach (QueueItem *itm, d->itemList)
    {
        if (itm->jobId == jobId)
        {
            d->itemList.removeOne(itm);
            delete itm;
        }
    }
}

void QueueManager::clearQueue()
{
    while (!d->itemList.isEmpty())
    {
        QueueItem *itm = d->itemList.takeFirst();
        delete itm;
    }
}

#include "QueueManager.moc"
