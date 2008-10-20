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

class KDE_EXPORT AbstractDevice
{
    Q_PROPERTY(QString name READ name WRITE setName)

public:
    typedef QList<AbstractDevice*> List;

    AbstractDevice();
    virtual ~AbstractDevice();

    QString name();

    virtual QAbstractItemModel *getFileModel() = 0;

protected:
    void setName(const QString &name);

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
