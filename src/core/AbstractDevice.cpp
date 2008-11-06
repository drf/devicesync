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

#include "AbstractDevice.h"

#include "AbstractDeviceInterface.h"

#include <KIcon>

class AbstractDevice::Private
{
public:
    Private()
            : token(0) {}

    ~Private() {}


    QString name;
    QString icon;
    AbstractDeviceInterface *interface;
    QAbstractItemModel *model;
    int token;
};

AbstractDevice::AbstractDevice(AbstractDeviceInterface *parent)
        : QObject(parent),
        d(new Private())
{
    d->interface = parent;
}

AbstractDevice::~AbstractDevice()
{
    delete d;
}

void AbstractDevice::setName(const QString &name)
{
    d->name = name;
}

void AbstractDevice::setIcon(const QString &name)
{
    d->icon = name;
}

QString AbstractDevice::name() const
{
    return d->name;
}

AbstractDeviceInterface *AbstractDevice::interface() {
    return d->interface;
}

QString AbstractDevice::iconName() const
{
    return d->icon;
}

KIcon AbstractDevice::icon() const
{
    return KIcon(d->icon);
}

void AbstractDevice::setModel(QAbstractItemModel *model)
{
    d->model = model;
    emit modelChanged(d->model, this);
}

QAbstractItemModel * AbstractDevice::model()
{
    return d->model;
}

int AbstractDevice::getNextTransferToken()
{
    int token = d->token;
    ++d->token;

    return token;
}

#include "AbstractDevice.moc"
