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

#include "MtpDevice.h"

MtpDevice::MtpDevice()
{
    // TODO Auto-generated constructor stub

}

MtpDevice::~MtpDevice()
{
    // TODO Auto-generated destructor stub
}

QAbstractItemModel *MtpDevice::getFileModel()
{
}

QString MtpDevice::getPathForCurrentIndex(const QModelIndex &index)
{
}

int MtpDevice::sendFileToDevice(const QString &fromPath, const QString &toPath)
{
}

int MtpDevice::sendFileToDeviceFromByteArray(const QByteArray &file, const QString &toPath)
{
    return -1;
}

int MtpDevice::getFileFromDevice(const QString &path, const QString &toPath)
{
}

int MtpDevice::getByteArrayFromDeviceFile(const QString &path)
{
    return -1;
}

#include "MtpDevice.moc"
