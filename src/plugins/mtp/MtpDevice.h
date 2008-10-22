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

#ifndef MTPDEVICE_H
#define MTPDEVICE_H

#include "AbstractDevice.h"

#include <libmtp.h>
#include <threadweaver/Job.h>

class QAbstractItemModel;
class QStandardItemModel;

class MtpDevice : public AbstractDevice
{
    Q_OBJECT

public:
    MtpDevice(const QString &udi, QObject *parent = 0);
    virtual ~MtpDevice();

    void connectDevice();
    void disconnectDevice();

    QAbstractItemModel * getFileModel();
    QString getPathForCurrentIndex(const QModelIndex &index);

    bool iterateRawDevices(int numrawdevices, LIBMTP_raw_device_t* rawdevices, const QString &serial);

public slots:
    int sendFileToDevice(const QString &fromPath, const QString &toPath);
    int sendFileToDeviceFromByteArray(const QByteArray &file, const QString &toPath);

    int getFileFromDevice(const QString &path, const QString &toPath);
    int getByteArrayFromDeviceFile(const QString &path);

private slots:
    void transferSuccessful(ThreadWeaver::Job*);

private:
    QAbstractItemModel *m_model;
    QString m_udi;
    bool m_success;
    LIBMTP_mtpdevice_t *m_device;
};

class LibMtpCallbacks : public QObject
{
    Q_OBJECT

    public:
        static LibMtpCallbacks* instance() { return s_instance ? s_instance : new LibMtpCallbacks(); }

        LibMtpCallbacks() { s_instance = this; };

        void setActionPercentage(int percentage);

    signals:
        void actionPercentageChanged(int);

    private:
        static LibMtpCallbacks* s_instance;
};

class WorkerThread : public ThreadWeaver::Job
{
    Q_OBJECT
public:
    WorkerThread(int numrawdevices, LIBMTP_raw_device_t* rawdevices, const QString &serial, MtpDevice* handler);
    virtual ~WorkerThread();

    virtual bool success() const;

protected:
    virtual void run();

private:
    bool m_success;
    int m_numrawdevices;
    LIBMTP_raw_device_t* m_rawdevices;
    QString m_serial;
    MtpDevice *m_handler;
};

class CreateModelThread : public ThreadWeaver::Job
{
    Q_OBJECT
public:
    CreateModelThread(LIBMTP_mtpdevice_t *device, MtpDevice *handler);
    virtual ~CreateModelThread();

    virtual bool success() const;

protected:
    virtual void run();

private:
    void iterateChildren(LIBMTP_folder_t *parentfolder);
    void iterateSiblings(LIBMTP_folder_t *parentfolder);

signals:
    void modelCreated(QStandardItemModel *model);

private:
    int m_success;
    LIBMTP_mtpdevice_t* m_device;
    MtpDevice *m_handler;
    QStandardItemModel *m_model;
};


class SendTrackThread : public ThreadWeaver::Job
{
    Q_OBJECT
public:
    SendTrackThread(LIBMTP_mtpdevice_t *device, QString name, LIBMTP_track_t *trackmeta,
                    LIBMTP_progressfunc_t cb, MtpDevice *parent);
    virtual ~SendTrackThread();

    virtual bool success() const;

protected:
    virtual void run();

private:
    int m_success;
    LIBMTP_mtpdevice_t* m_device;
    QString m_name;
    LIBMTP_track_t *m_trackmeta;
    LIBMTP_progressfunc_t m_callback;
    MtpDevice *m_parent;
};

class SendFileThread : public ThreadWeaver::Job
{
    Q_OBJECT
public:
    SendFileThread(LIBMTP_mtpdevice_t *device, QString name, LIBMTP_file_t *file,
                    LIBMTP_progressfunc_t cb, MtpDevice *parent);
    virtual ~SendFileThread();

    virtual bool success() const;

protected:
    virtual void run();

private:
    int m_success;
    LIBMTP_mtpdevice_t* m_device;
    QString m_name;
    LIBMTP_file_t *m_file;
    LIBMTP_progressfunc_t m_callback;
    MtpDevice *m_parent;
};

#endif /* MTPDEVICE_H */
