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

#ifndef ABSTRACTDEVICE_H
#define ABSTRACTDEVICE_H

#include <QObject>
#include <QList>
#include <QModelIndex>

#include <kdemacros.h>

class QAbstractItemModel;
class KIcon;
class AbstractDeviceInterface;

/**
 * AbstractDevice represents a single device instance. When writing a plugin,
 * you have to reimplement the virtual methods of this class.
 *
 * AbstractDevice and AbstractDeviceInterface already encapsulate a huge part of
 * the logic. Mainly, the functions you will have to implement will be mostly
 * device-specific. Though, please make sure of reading the whole documentation
 * for the virtual functions, since some times you are requested to do some specific
 * operations inside them.
 */
class KDE_EXPORT AbstractDevice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString icon READ iconName WRITE setIcon)

public:
    /**
     * @internal
     * A list of AbstractDevicePointers.
     */
    typedef QList<AbstractDevice*> List;

    /**
     * Constructor of the class.
     *
     * Passing the parent interface is mandatory, and it is already done for you.
     * Please make sure of calling the constructor for AbstractDevice in your
     * reimplementation, since it initializes some important stuff.
     *
     * @param parent the device interface that created this device
     */
    explicit AbstractDevice(AbstractDeviceInterface *parent);
    virtual ~AbstractDevice();

    /**
     * Connects the device
     *
     * Calling this function will connect the device. You have to reimplement this
     * in your plugin.
     * When your device gets connected, please make sure you emit the signal @deviceConnected.
     * Failure in doing this will lead to undefined behavior in the core
     */
    virtual void connectDevice() = 0;
    /**
     * Disconnects the device
     *
     * Calling this function will disconnect the device. You have to reimplement this
     * in your plugin.
     */
    virtual void disconnectDevice() = 0;

    /**
     * Returns the name of the device.
     *
     * You can get the name of the device through this function.
     *
     * @returns the name of the device
     *
     * @see setName
     */
    QString name();
    /**
     * Returns the icon of the device.
     *
     * You can get the KIcon for this device through this function.
     *
     * @returns a KIcon for this device
     *
     * @see setIcon
     * @see iconName
     */
    KIcon icon();
    /**
     * Returns the icon name of the device.
     *
     * You can get the icon name for this device through this function.
     *
     * @returns the icon name for this device
     *
     * @see setIcon
     * @see icon
     */
    QString iconName();

    /**
     * Returns the parent interface of the device.
     *
     * You can access the parent interface of the device through this
     * function. You generally should not feel the need of accessing the
     * interface from the device, unless you have some particular needs.
     *
     * @returns the parent interface for this device
     */
    AbstractDeviceInterface *interface();

    void setModel(QAbstractItemModel *model);
    QAbstractItemModel *model();

    virtual QString getPathForCurrentIndex(const QModelIndex &index) = 0;

    virtual void reloadModel() = 0;

public slots:
    virtual int sendFileToDevice(const QString &fromPath, const QString &toPath) = 0;
    virtual int sendFileToDeviceFromByteArray(const QByteArray &file, const QString &toPath) = 0;

    virtual int getFileFromDevice(const QString &path, const QString &toPath) = 0;
    virtual int getByteArrayFromDeviceFile(const QString &path) = 0;

    virtual void createFolder(const QString &name, const QString &inPath) = 0;

protected:
    void setName(const QString &name);
    void setIcon(const QString &name);
    int getNextTransferToken();

signals:
    void fileCopiedToDevice(int, const QString&);
    void fileCopiedFromDevice(int, const QString&);

    void actionProgressChanged(int);

    void modelChanged(QAbstractItemModel *, AbstractDevice *);

    void deviceConnected(AbstractDevice *device);

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
    QString udi;
};

#endif /* ABSTRACTDEVICE_H_ */
