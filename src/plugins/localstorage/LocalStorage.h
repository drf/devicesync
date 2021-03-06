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

#ifndef LOCALSTORAGE_H
#define LOCALSTORAGE_H

#include "AbstractDeviceInterface.h"

#include <QVariant>

class LocalStorage : public AbstractDeviceInterface
{
    Q_OBJECT

public:
    LocalStorage(QObject *parent, const QVariantList&);
    virtual ~LocalStorage();

    void init();

    void scan() {};

public slots:
    void startWatching();
    void stopWatching();

    AbstractDevice *forceDeviceConnection() {
        return 0;
    };
};

#endif /* LOCALSTORAGE_H */
