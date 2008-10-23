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

#include "devicesync.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>

static const char description[] =
    I18N_NOOP("An application that lets you transfer data between devices");

static const char version[] = "0.1";

int main(int argc, char **argv)
{
    KAboutData about("devicesync", 0, ki18n("DeviceSync"), version, ki18n(description),
                     KAboutData::License_GPL, ki18n("(C) 2008 Dario Freddi"), KLocalizedString(), 0, "drf54321@gmail.com");
    about.addAuthor(ki18n("Dario Freddi"), ki18n("Maintainer"), "drf54321@gmail.com");
    KCmdLineArgs::init(argc, argv, &about);

    new KComponentData(about);

    KCmdLineOptions options;
    options.add("+[URL]", ki18n("Document to open"));
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    DeviceSync *widget = new DeviceSync;

    // see if we are starting with session management
    if (app.isSessionRestored()) {
        RESTORE(DeviceSync);
    } else {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0) {
            //devicesync *widget = new devicesync;
            widget->show();
        } else {
            int i = 0;
            for (; i < args->count(); i++) {
                //devicesync *widget = new devicesync;
                widget->show();
            }
        }
        args->clear();
    }

    return app.exec();
}
