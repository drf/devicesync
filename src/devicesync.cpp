/*
 * devicesync.cpp
 *
 * Copyright (C) 2008 Dario Freddi <drf54321@gmail.com>
 */
#include "devicesync.h"
#include "devicesyncview.h"
#include "settings.h"

#include <QtGui/QDropEvent>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>

#include <kconfigdialog.h>
#include <kstatusbar.h>

#include <kaction.h>
#include <kactioncollection.h>
#include <kstandardaction.h>

#include <KServiceTypeTrader>
#include <KPluginSelector>
#include <KPluginInfo>

#include <KDE/KLocale>

DeviceSync::DeviceSync()
    : KXmlGuiWindow(),
      m_view(new DeviceSyncView(this))
{
    // accept dnd
    setAcceptDrops(true);

    // tell the KXmlGuiWindow that this is indeed the main widget
    setCentralWidget(m_view);

    // then, setup our actions
    setupActions();

    loadAllPlugins();

    // add a status bar
    statusBar()->show();

    // a call to KXmlGuiWindow::setupGUI() populates the GUI
    // with actions, using KXMLGUI.
    // It also applies the saved mainwindow settings, if any, and ask the
    // mainwindow to automatically save settings if changed: window size,
    // toolbar position, icon size, etc.
    setupGUI();
}

DeviceSync::~DeviceSync()
{
}

void DeviceSync::setupActions()
{
    KStandardAction::quit(qApp, SLOT(closeAllWindows()), actionCollection());

    KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());

    KAction *connectDevice = new KAction(KIcon("network-connect"), i18n("&Connect Device..."), this);
    actionCollection()->addAction("connect_device", connectDevice);

}

void DeviceSync::optionsPreferences()
{
    // The preference dialog is derived from prefs_base.ui
    //
    // compare the names of the widgets in the .ui file
    // to the names of the variables in the .kcfg file
    //avoid to have 2 dialogs shown
    if ( KConfigDialog::showDialog( "settings" ) )  {
        return;
    }
    KConfigDialog *dialog = new KConfigDialog(this, "settings", Settings::self());
    QWidget *generalSettingsDlg = new QWidget;
    ui_prefs_base.setupUi(generalSettingsDlg);
    dialog->addPage(generalSettingsDlg, i18n("General"), "package_setting");
    connect(dialog, SIGNAL(settingsChanged(QString)), m_view, SLOT(settingsChanged()));
    dialog->setAttribute( Qt::WA_DeleteOnClose );
    dialog->show();
}

bool DeviceSync::loadAllPlugins()
{
    KService::List offers;

    //Static plugins
    offers = KServiceTypeTrader::self()->query("Devicesync/Plugin");

    bool loaded = false;

    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup conf(config, "Plugins");

    for (int i = 0; i < offers.size(); i++) {
        KService::Ptr offer = offers[i];
        m_services.append(offer);

        AbstractDeviceInterface * section;

        KPluginInfo description(offer);
        description.load(conf);

        bool selected = description.isPluginEnabled();

        if (selected) {
            if ((section = createPluginFromService(offer)) != 0) {
                kDebug() << "# Plugin " + description.name() + " found";
                kDebug() << "# Version: " + description.version();
                kDebug() << "# Description: " + description.comment() + description.icon();
                kDebug() << "# Author: " + description.author();
                kDebug() << "# Website: " + description.website();
            } else {
                kDebug() << "Error loading Devicesync plugin ("
                << (offers[i])->library() << ")" << endl;
            }
        } else {
            kDebug() << "# Plugin " + description.name() + " found, however it's not activated, skipping...";
            continue;
        }
    }
}

AbstractDeviceInterface * DeviceSync::createPluginFromService(const KService::Ptr &service)
{
    //try to load the specified library
    KPluginFactory *factory = KPluginLoader(service->library()).factory();

    if (!factory) {
        kError(5001) << "KPluginFactory could not load the plugin:" << service->library();
        return 0;
    }

    QObject * plugin = factory->create< AbstractDeviceInterface >();

    return qobject_cast<AbstractDeviceInterface *>(plugin);
}

void DeviceSync::deviceConnected(AbstractDevice *device)
{
    m_view->addDevice(device->name(), device->name());
}

#include "devicesync.moc"
