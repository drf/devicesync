/*
 * QueueManager.h
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#ifndef QUEUEMANAGER_H
#define QUEUEMANAGER_H

#include "AbstractDevice.h"

#include <kdemacros.h>

class KDE_EXPORT QueueItem
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

class KDE_EXPORT QueueManager : public QObject
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

    void processQueue();

private slots:
    void processNextQueueItem();
    void fileCopiedFromDevice(int token, const QString &path);
    void fileCopiedToDevice(int token, const QString &filePath);

private:
    int generateItemId();

private:
    class Private;
    Private *d;
};

#endif /* QUEUEMANAGER_H */
