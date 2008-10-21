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

    ProgressInterface *progressInterface();

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
