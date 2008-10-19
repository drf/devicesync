#include "devicesync.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>

static const char description[] =
    I18N_NOOP("A KDE 4 Application");

static const char version[] = "0.1";

int main(int argc, char **argv)
{
    KAboutData about("devicesync", 0, ki18n("DeviceSync"), version, ki18n(description),
                     KAboutData::License_GPL, ki18n("(C) 2008 Dario Freddi"), KLocalizedString(), 0, "drf54321@gmail.com");
    about.addAuthor( ki18n("Dario Freddi"), KLocalizedString("Maintainer"), "drf54321@gmail.com" );
    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("+[URL]", ki18n( "Document to open" ));
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    DeviceSync *widget = new DeviceSync;

    // see if we are starting with session management
    if (app.isSessionRestored())
    {
        RESTORE(DeviceSync);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0)
        {
            //devicesync *widget = new devicesync;
            widget->show();
        }
        else
        {
            int i = 0;
            for (; i < args->count(); i++)
            {
                //devicesync *widget = new devicesync;
                widget->show();
            }
        }
        args->clear();
    }

    return app.exec();
}
