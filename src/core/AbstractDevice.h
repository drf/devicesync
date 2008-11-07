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
 *
 * AbstractDevice is completely asynchronous and makes heavy use of signals and slots,
 * so you will have to emit some signals in your reimplementation to notify the core about
 * events. There are no threads in DeviceSync, though you will probably have to use threading
 * in your device plugin if that kind of device does not support asynchronous transfer.
 * It is strongly suggested to use ThreadWeaver for both performance and ease of use:
 * you can see an example of ThreadWeaver usage in the MTP plugin.
 */

class KDE_EXPORT AbstractDevice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString icon READ iconName WRITE setIcon)
    Q_PROPERTY(QAbstractItemModel* model READ model WRITE setModel)

public:
    /**
     * @internal
     * A list of AbstractDevice Pointers.
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
     *
     * @see deviceConnected
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
    QString name() const;
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
    KIcon icon() const;
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
    QString iconName() const;

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

    /**
     * Returns the model for representing file/folder view of the device
     *
     * @returns the current model for this device
     * @see setModel
     */
    QAbstractItemModel *model();

    /**
     * Returns the path associated to the given index.
     *
     * You have to reimplement this function. It is critical to return
     * a path representation that you will be able to use later. The path
     * you will return here will be the path passed in transfer functions.
     *
     * @param index the index of the current model
     * @returns the path associated to index
     *
     * @see sendFileToDevice
     * @see getFileFromDevice
     * @see model
     * @see setModel
     */
    virtual QString getPathForCurrentIndex(const QModelIndex &index) = 0;

    /**
     * Requests reloading the model.
     *
     * You have to reimplement this function. Just load a new model and
     * set it through @setModel
     *
     * @see setModel
     */
    virtual void reloadModel() = 0;

public slots:
    /**
     * Transfers a file to the device
     *
     * You have to reimplement this function. This function should return a
     * valid transfer token for tracking the operation. You can get one simply
     * by calling getNextTransferToken(). Make sure to keep track of the token yourself
     * since you will need them later.
     *
     * DeviceSync is completely asynchronous. This means this function should just
     * initialize the transfer and return the token immediately. It is strongly
     * advised to implement file transfer with threads.
     *
     * Once the transfer is complete, you need to emit fileCopiedToDevice(). This is
     * critical, since the core needs to be notified about transfer status. Failure
     * in doing this will result in a non working plugin.
     *
     * @param fromPath the path of the file that should be copied onto the device. It is a standard path on the local disk
     * @param toPath the path where the file should be copied. It is a path you returned in getPathForCurrentIndex()
     * @returns a transfer token associated to the newly created transfer
     *
     * @see getNextTransferToken
     * @see fileCopiedToDevice
     * @see getPathForCurrentIndex
     */
    virtual int sendFileToDevice(const QString &fromPath, const QString &toPath) = 0;
    virtual int sendFileToDeviceFromByteArray(const QByteArray &file, const QString &toPath) = 0;

    /**
     * Gets a file from the device
     *
     * You have to reimplement this function. This function should return a
     * valid transfer token for tracking the operation. You can get one simply
     * by calling getNextTransferToken(). Make sure to keep track of the token yourself
     * since you will need them later.
     *
     * DeviceSync is completely asynchronous. This means this function should just
     * initialize the transfer and return the token immediately. It is strongly
     * advised to implement file transfer with threads.
     *
     * Once the transfer is complete, you need to emit fileCopiedFromDevice(). This is
     * critical, since the core needs to be notified about transfer status. Failure
     * in doing this will result in a non working plugin.
     *
     * @param fromPath the path of the file we want to get. It is a path you
     *                 returned in getPathForCurrentIndex()
     * @param toPath the path where the file should be copied.
     *               It is a standard path on the local disk
     * @returns a transfer token associated to the newly created transfer
     *
     * @see getNextTransferToken
     * @see fileCopiedFromDevice
     * @see getPathForCurrentIndex
     */
    virtual int getFileFromDevice(const QString &path, const QString &toPath) = 0;
    virtual int getByteArrayFromDeviceFile(const QString &path) = 0;

    /**
     * Creates a folder into the device
     *
     * You have to reimplement this function. This is function is non-queued and
     * non asynchronous. You just have to create the folder and return right after that.
     * This might change in the future, though.
     *
     * @param name the name of the new folder
     * @param inPath the path where the folder should be created.
     *               It is a path returned from getPathForCurrentIndex
     *
     * @see getPathForCurrentIndex
     */
    virtual void createFolder(const QString &name, const QString &inPath) = 0;
    /**
     * Renames an object in the device
     *
     * You have to reimplement this function. This is function is non-queued and
     * non asynchronous. You just have to rename the given object and return right
     * after that. The object could be a file or a folder
     *
     * @param path the path of the object to rename.
     *             It is a path returned from getPathForCurrentIndex
     * @param newName the new name for the object
     *
     * @see getPathForCurrentIndex
     */
    virtual void renameObject(const QString &path, const QString &newName) = 0;

    /**
     * Removes an object from the device
     *
     * You have to reimplement this function. This function should return a
     * valid transfer token for tracking the operation. You can get one simply
     * by calling getNextTransferToken(). Make sure to keep track of the token yourself
     * since you will need them later.
     *
     * DeviceSync is completely asynchronous. This means this function should just
     * initialize the deletion and return the token immediately. It is strongly
     * advised to implement deletion with threads.
     *
     * Once the deletion is complete, you need to emit pathRemovedFromDevice(). This is
     * critical, since the core needs to be notified about transfer status. Failure
     * in doing this will result in a non working plugin.
     *
     * Please note that you need to add recursion if the requested path is a folder.
     *
     * @param fromPath the path we want to remove. It is a path you
     *                 returned in getPathForCurrentIndex()
     * @returns a transfer token associated to the newly created transfer
     *
     * @see getNextTransferToken
     * @see fileCopiedFromDevice
     * @see getPathForCurrentIndex
     */
    virtual int removePath(const QString &path) = 0;

protected:
    /**
     * Sets the name for this device. This name will be displayed in
     * the interface, so be sure to set it at least once.
     *
     * @param name the name for the device
     *
     * @see name
     */
    void setName(const QString &name);
    /**
     * Sets the icon for this device. This icon will be displayed in
     * the interface, so be sure to set it at least once.
     *
     * @param icon the icon for the device
     *
     * @see icon
     */
    void setIcon(const QString &name);
    /**
     * Sets the file model for this interface. The model passed in this
     * function will be the one displayed in the interface for navigating
     * inside the device. You basically have to create a model containing
     * the disk structure of the device and set it through this function when
     * requested, for example, by reloadModel()
     *
     * @param model the new model to be shown
     *
     * @see reloadModel
     */
    void setModel(QAbstractItemModel *model);

    /**
     * Returns a valid, unique transfer token.
     *
     * This function is granted to return a valid transfer token for a single
     * operation. You need to call this when you're starting a new operation
     * and you need to keep track of it, aka always. The returned token will be
     * useful both for the main interface to keep track of your job, and for you
     * to make some action on it. So please be sure to track all your jobs through
     * their token.
     *
     * @returns a unique valid transfer token
     *
     * @see sendFileFromDevice
     * @see getFileFromDevice
     * @see removePath
     */
    int getNextTransferToken();

signals:
    /**
     * You have to emit this signal when a copy operation to the device finishes.
     * It is strictly compulsory to emit it when the operation finishes, since
     * the core keeps track of progress and handles the queue through these signals.
     *
     * @param token the token associated with the completed transfer
     * @param path the path this transfer was referring to
     *
     * @see sendFileToDevice
     */
    void fileCopiedToDevice(int token, const QString &path);
    /**
     * You have to emit this signal when a copy operation from the device finishes.
     * It is strictly compulsory to emit it when the operation finishes, since
     * the core keeps track of progress and handles the queue through these signals.
     *
     * @param token the token associated with the completed transfer
     * @param path the path this transfer was referring to
     *
     * @see getFileFromDevice
     */
    void fileCopiedFromDevice(int token, const QString &path);
    /**
     * You have to emit this signal when a removal operation on the device finishes.
     * It is strictly compulsory to emit it when the operation finishes, since
     * the core keeps track of progress and handles the queue through these signals.
     *
     * @param token the token associated with the completed deletion
     * @param path the path this deletion was referring to
     *
     * @see removePath
     */
    void pathRemovedFromDevice(int token, const QString &path);

    /**
     * You have to emit this signal when the current action progress
     * changes. It is not compulsory to do it, though it is advised, since
     * it allows to show more detailed information in the interface.
     *
     */
    void actionProgressChanged(int percentage);

    /**
     * Emitted when the model gets changed. Do not emit this signal manually:
     * it is already done for you when you call setModel()!
     *
     * @see setModel
     */
    void modelChanged(QAbstractItemModel *, AbstractDevice *);

    /**
     * Emitted when the device gets connected. You have to emit
     * this signal when the connection operation finishes and the device
     * is available, passing this as a parameter.
     *
     * @param device the connected device, just use "this" here
     *
     * @see connectDevice
     */
    void deviceConnected(AbstractDevice *device);

private:
    class Private;
    Private *d;
};

/**
 * DeviceContainer is a convenience class used to represent a device and some
 * of its properties. Since accessing directly the device class could be dangerous sometimes
 * (eg. underlying object no longer exists) or can be rather slow, DeviceContainer
 * handles the job of making information about a device reachable in a simple way.
 *
 * This class has no methods: it is simply a container (surprise, huh?), and you will
 * have to deal with it very few times
 */
class KDE_EXPORT DeviceContainer
{
public:
    /**
     * @internal
     * A list of DeviceContainer Pointers.
     */
    typedef QList<DeviceContainer*> List;

    /**
     * This enum represents the current status of the device
     */
    enum Status {
        /** The device is connected and ready to be used */
        Connected,
        /** The device exists, but is not connected yet */
        Disconnected
    };

    /** The device this container has */
    AbstractDevice *device;
    /** Current status of the device */
    Status status;
    /** UDI of the device */
    QString udi;
};

#endif /* ABSTRACTDEVICE_H_ */
