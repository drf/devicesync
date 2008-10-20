/*
 * AbstractDevice.cpp
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#include "AbstractDevice.h"

class AbstractDevice::Private
{
public:
    Private()
     : token(0)
    {
    }
    ~Private() {}


    QString name;
    int token;
};

AbstractDevice::AbstractDevice(QObject *parent)
        : QObject(parent),
        d(new Private())
{
}

AbstractDevice::~AbstractDevice()
{
    delete d;
}

void AbstractDevice::setName(const QString &name)
{
    d->name = name;
}

QString AbstractDevice::name()
{
    return d->name;
}

int AbstractDevice::getNextTransferToken()
{
    int token = d->token;
    ++d->token;

    return token;
}

#include "AbstractDevice.moc"
