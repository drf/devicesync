/*
 * QueueManager.h
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#ifndef QUEUEMANAGER_H
#define QUEUEMANAGER_H

#include "AbstractDevice.h"

class QueueItem
{
    public:
        typedef QList<QueueItem*> List;

        enum Action {
            Copy,
            Move,
            Delete,
            Rename
        };

        Action action;
        AbstractDevice *in_device;
        AbstractDevice *out_device;
        QString in_path;
        QString out_path;
        int jobId;
};

class QueueManager : public QObject
{
    Q_OBJECT

    public:
        QueueManager(QObject *parent = 0);
        virtual ~QueueManager();

    public slots:
        int addJobToQueue(QueueItem::Action action, AbstractDevice *in, const QString &inpath,
                AbstractDevice *out = 0, const QString &outpath = QString());
        void removeJobFromQueue(int jobId);

        void clearQueue();

    private:
        int generateItemId();

    private:
        class Private;
        Private *d;
};

#endif /* QUEUEMANAGER_H */
