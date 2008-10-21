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

#include "ProgressDialog.h"

#include "QueueManager.h"

#include <KDebug>

ProgressDialog::ProgressDialog(ProgressInterface *iface, QWidget *parent)
        : KDialog(parent),
        m_interface(iface)
{
    setButtons(KDialog::None);

    m_widget = new QWidget();
    ui.setupUi(m_widget);

    setMainWidget(m_widget);
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);
    show();

    ui.totalBar->setRange(0, 100);
    ui.currentBar->setRange(0, 100);

    connect(m_interface, SIGNAL(totalProgressChanged(int)), this, SLOT(totalProgressChanged(int)));
    connect(m_interface, SIGNAL(currentItemChanged(QueueItem*)), this, SLOT(currentItemChanged(QueueItem*)));
    connect(m_interface, SIGNAL(currentItemProgressChanged(ProgressInterface::Action, int)),
            this, SLOT(currentItemProgressChanged(ProgressInterface::Action, int)));
}

ProgressDialog::~ProgressDialog()
{
    // TODO Auto-generated destructor stub
}

void ProgressDialog::totalProgressChanged(int percent)
{
    kDebug() << "Setting total percent";
    ui.totalBar->setValue(percent);
}

void ProgressDialog::currentItemChanged(QueueItem *item)
{
    switch (item->action) {
    case QueueItem::Copy:
        ui.currentLabel->setText(i18n("Copying %1 to %2...", KUrl::fromPath(item->in_path).fileName(),
                                      item->out_device->name()));
        break;
    default:
        break;
    }
}

void ProgressDialog::currentItemProgressChanged(ProgressInterface::Action action, int percent)
{
    Q_UNUSED(action)

    ui.currentBar->setValue(percent);
}

#include "ProgressDialog.moc"
