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

#ifndef QUEUEMANAGER_H
#define QUEUEMANAGER_H

#include "AbstractDevice.h"

#include <kdemacros.h>

class ProgressInterface;

/**
 * This class provides a representation of an item in the Queue.
 *
 * It is just a container, and you will have to handle it read-only, in
 * case you want to create your own frontend.
 */
class KDE_EXPORT QueueItem
{
public:
    /**
     * @internal
     *
     * A list of QueueItem pointers
     */
    typedef QList<QueueItem*> List;

    /**
     * Represents the action this item will be doing
     */
    enum Action {
        /** Copying a file */
        Copy,
        /** Moving a file */
        Move,
        /* Deleting an object */
        Delete,
        /* Renaming an object */
        Rename
    };

    /** The action of this item */
    Action action;
    /** The input device */
    AbstractDevice *in_device;
    /** The output device */
    AbstractDevice *out_device;
    /* The input path */
    QString in_path;
    /* The output path */
    QString out_path;
    /* The job token associated with this item */
    int jobId;
};

/**
 * Queue manager handles queue in devicesync. It is device agnostic
 * (it can handle an infinite number of jobs/devices in a single queue),
 * job agnostic (it can handle an infinite type of jobs), and completely
 * asynchronous.
 *
 * This handler can be used to set up and perform a queue pretty easily,
 * and has builtin support to show detailed progress.
 *
 * @author Dario Freddi
 */
class KDE_EXPORT QueueManager : public QObject
{
    Q_OBJECT

public:
    QueueManager(QObject *parent = 0);
    virtual ~QueueManager();

    /**
     * Returns the progress interface for the queue. Through the
     * progress interface you can monitor progress of this queue.
     *
     * @returns the progress interface
     */
    ProgressInterface *progressInterface();
    /**
     * Returns all items in queue
     *
     * @returns all items in queue
     */
    QueueItem::List currentQueue();

public slots:
    /**
     * Adds a new job to the queue.
     *
     * Jobs are tracked through their own token too. This function returns a
     * valid job token if adding the item succeeded, or -1 if adding the item
     * to queue failed.
     *
     * If the operation is done on a single device, such as folder creation or
     * removal, just omit out and outpath
     *
     * @param action the action of the job
     * @param in the input device (for example, in a copy operation, the device files will be copied from)
     * @param inpath the input path
     * @param out the output device (for example, in a copy operation, the device files will be copied to)
     * @param outpath the output path
     *
     * @returns a valid job token on success, -1 on failure
     */
    int addJobToQueue(QueueItem::Action action, AbstractDevice *in, const QString &inpath,
                      AbstractDevice *out = 0, const QString &outpath = QString());
    /**
     * Removes a job from the current queue
     *
     * @param jobId the job token associated to the job you want to remove
     */
    void removeJobFromQueue(int jobId);

    /**
     * Clears all the queue
     */
    void clearQueue();

    /**
     * Starts processing the current queue
     */
    void processQueue();

private slots:
    void processNextQueueItem();
    void fileCopiedFromDevice(int token, const QString &path);
    void fileCopiedToDevice(int token, const QString &filePath);
    void pathRemovedFromDevice(int token, const QString &path);

Q_SIGNALS:
    void queueCleared();

private:
    int generateItemId();

private:
    class Private;
    Private *d;
};

#endif /* QUEUEMANAGER_H */
