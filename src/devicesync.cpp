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
#include "devicesyncview.h"
#include "settings.h"

#include "AbstractDeviceInterface.h"
#include "QueueManager.h"

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

class DeviceSync::Private
{
public:
    Private() {
    }
    ~Private() {}


    DeviceSyncView *view;
    QList<KService::Ptr> services;
    AbstractDeviceInterface::List interfaces;
    QueueManager *manager;
};

DeviceSync::DeviceSync()
        : KXmlGuiWindow(),
        d(new Private())
{
    d->view = new DeviceSyncView(this);
    d->manager = new QueueManager(this);

    // accept dnd
    setAcceptDrops(true);

    // tell the KXmlGuiWindow that this is indeed the main widget
    setCentralWidget(d->view);

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
    delete d;
}

QueueManager * DeviceSync::queueManager()
{
    return d->manager;
}

void DeviceSync::setupActions()
{
    KStandardAction::quit(qApp, SLOT(closeAllWindows()), actionCollection());

    KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());

    KAction *connectDevice = new KAction(KIcon("network-connect"), i18n("&Connect Device..."), this);
    KAction *processQueue = new KAction(KIcon("network-connect"), i18n("Process Q&ueue"), this);

    connect(processQueue, SIGNAL(triggered()), this, SLOT(processQueue()));

    actionCollection()->addAction("connect_device", connectDevice);
    actionCollection()->addAction("process_queue", processQueue);
}

void DeviceSync::processQueue()
{
    queueManager()->processQueue();
}

void DeviceSync::optionsPreferences()
{
    // The preference dialog is derived from prefs_base.ui
    //
    // compare the names of the widgets in the .ui file
    // to the names of the variables in the .kcfg file
    //avoid to have 2 dialogs shown
    if (KConfigDialog::showDialog("settings"))  {
        return;
    }
    KConfigDialog *dialog = new KConfigDialog(this, "settings", Settings::self());
    QWidget *generalSettingsDlg = new QWidget;
    ui_prefs_base.setupUi(generalSettingsDlg);
    dialog->addPage(generalSettingsDlg, i18n("General Settings"), "package_setting");

    KPluginSelector *pluginSelector = new KPluginSelector();
    KService::List offers = KServiceTypeTrader::self()->query("Devicesync/Plugin");

    pluginSelector->addPlugins(KPluginInfo::fromServices(offers), KPluginSelector::ReadConfigFile,
                               i18n("Plugins"), "Service", KGlobal::config());

    pluginSelector->load();

    KPageWidgetItem *page = dialog->addPage(pluginSelector, i18n("Plugins"), "plugin_setting");
    page->setIcon(KIcon("preferences-plugin"));

    dialog->exec();

    pluginSelector->save();

    dialog->deleteLater();
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
        d->services.append(offer);

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

                connect(section, SIGNAL(deviceConnected(AbstractDevice*)), this, SLOT(deviceConnected(AbstractDevice*)));
                connect(section, SIGNAL(deviceDisconnected(AbstractDevice*)), this, SLOT(deviceDisconnected(AbstractDevice*)));
                connect(section, SIGNAL(newDeviceRegistered(AbstractDevice*)), this, SLOT(newDeviceRegistered(AbstractDevice*)));
                connect(section, SIGNAL(deviceRemoved(AbstractDevice*)), this, SLOT(deviceRemoved(AbstractDevice*)));

                d->interfaces.append(section);
                section->init();
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

void DeviceSync::newDeviceRegistered(AbstractDevice *device)
{
    kDebug() << "A new device has been registered:" << device->name();
}

void DeviceSync::deviceConnected(AbstractDevice *device)
{
    kDebug() << "A new device has been connected:" << device->name();
    d->view->addDevice(device->name(), device->name());
}

void DeviceSync::deviceDisconnected(AbstractDevice *device)
{
    kDebug() << "The following device has been removed:" << device->name();
    d->view->removeDevice(device->name());
}

void DeviceSync::deviceRemoved(AbstractDevice *device)
{
    kDebug() << "The following device is no longer available:" << device->name();
}

AbstractDevice * DeviceSync::getConnectedDeviceByName(const QString &name)
{
    foreach(AbstractDeviceInterface *ent, d->interfaces) {
        foreach(AbstractDevice *device, ent->getConnectedDevices()) {
            if (device->name() == name) {
                return device;
            }
        }
    }
}

#include "devicesync.moc"
