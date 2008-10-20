/*
 * AbstractDevice.h
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#ifndef ABSTRACTDEVICE_H
#define ABSTRACTDEVICE_H

#include <QObject>
#include <QList>

#include <kdemacros.h>

class QAbstractItemModel;

class KDE_EXPORT AbstractDevice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)

public:
    typedef QList<AbstractDevice*> List;

    AbstractDevice(QObject *parent = 0);
    virtual ~AbstractDevice();

    QString name();

    virtual QAbstractItemModel *getFileModel() = 0;

public slots:
    virtual int sendFileToDevice(const QString &fromPath, const QString &toPath) = 0;
    virtual int sendFileToDeviceFromByteArray(const QByteArray &file, const QString &toPath) = 0;

    virtual int getFileFromDevice(const QString &path, const QString &toPath) = 0;
    virtual int getByteArrayFromDeviceFile(const QString &path) = 0;

protected:
    void setName(const QString &name);
    int getNextTransferToken();

signals:
    void fileCopiedToDevice(int,const QString&);
    void fileCopiedFromDevice(int,const QString&);

private:
    class Private;
    Private *d;
};

class KDE_EXPORT DeviceContainer
{
public:
    typedef QList<DeviceContainer*> List;

    enum Status {
        Connected,
        Disconnected
    };

    AbstractDevice *device;
    Status status;
};

#endif /* ABSTRACTDEVICE_H_ */
