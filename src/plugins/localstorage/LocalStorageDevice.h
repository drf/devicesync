/*
 * LocalStorageDevice.h
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#ifndef LOCALSTORAGEDEVICE_H
#define LOCALSTORAGEDEVICE_H

#include "AbstractDevice.h"
#include <kio/job.h>

class QFileSystemModel;
class KJob;

class LocalStorageDevice : public AbstractDevice
{
    Q_OBJECT

public:
    LocalStorageDevice();
    virtual ~LocalStorageDevice();

    QAbstractItemModel * getFileModel();
    QString getPathForCurrentIndex(const QModelIndex &index);

public slots:
    int sendFileToDevice(const QString &fromPath, const QString &toPath);
    int sendFileToDeviceFromByteArray(const QByteArray &file, const QString &toPath);

    int getFileFromDevice(const QString &path, const QString &toPath);
    int getByteArrayFromDeviceFile(const QString &path);

private slots:
    void sendToDeviceDone(KIO::Job*,const KUrl&,const KUrl&,time_t,bool,bool);
    void getFromDeviceDone(KIO::Job*,const KUrl&,const KUrl&,time_t,bool,bool);

private:
    QFileSystemModel *m_model;
};

#endif /* LOCALSTORAGEDEVICE_H_ */
