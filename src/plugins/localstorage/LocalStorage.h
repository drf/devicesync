/*
 * LocalStorage.h
 *
 *  Created on: 20-ott-2008
 *      Author: drf
 */

#ifndef LOCALSTORAGE_H_
#define LOCALSTORAGE_H_

#include "AbstractDeviceInterface.h"

#include <QVariant>

class LocalStorage : public AbstractDeviceInterface
{
    Q_OBJECT

public:
    LocalStorage(QObject *parent, const QVariantList&);
    virtual ~LocalStorage();

    virtual void init();

public slots:
    void startWatching();
    void stopWatching();
};

#endif /* LOCALSTORAGE_H_ */
