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

#include <KDebug>

#include <threadweaver/ThreadWeaver.h>

#include <solid/device.h>
#include <solid/portablemediaplayer.h>

MtpDevice::MtpDevice(const QString &udi, QObject *parent)
 : AbstractDevice(parent),
 m_udi(udi)
{
}

MtpDevice::~MtpDevice()
{
    // TODO Auto-generated destructor stub
}

void MtpDevice::connectDevice()
{
    Solid::PortableMediaPlayer* pmp = Solid::Device( m_udi ).as<Solid::PortableMediaPlayer>();
    QString serial = pmp->driverHandle( "mtp" ).toString();
    setName("Mtp Device");

    LIBMTP_raw_device_t * rawdevices;
    int numrawdevices;
    LIBMTP_error_number_t err;

    kDebug() << "Initializing MTP stuff";
    LIBMTP_Init();

    // get list of raw devices
    kDebug() << "Getting list of raw devices";
    err = LIBMTP_Detect_Raw_Devices(&rawdevices, &numrawdevices);

    kDebug() << "Error is: " << err;

    switch(err) {
        case LIBMTP_ERROR_NO_DEVICE_ATTACHED:
            fprintf(stdout, "   No raw devices found.\n");
            m_success = false;
            break;

        case LIBMTP_ERROR_CONNECTING:
            fprintf(stderr, "Detect: There has been an error connecting. Exiting\n");
            m_success = false;
            break;

        case LIBMTP_ERROR_MEMORY_ALLOCATION:
            fprintf(stderr, "Detect: Encountered a Memory Allocation Error. Exiting\n");
            m_success = false;
            break;

        case LIBMTP_ERROR_NONE:
        {
            m_success = true;
            break;
        }

        default:
            kDebug() << "Unhandled mtp error";
            m_success = false;
            break;
    }

    if( m_success )
    {
        kDebug() << "Got mtp list, connecting to device using thread";
        ThreadWeaver::Weaver::instance()->enqueue( new WorkerThread( numrawdevices, rawdevices, serial, this ) );
    }
    else
    {
        free( rawdevices );
    }
}

bool MtpDevice::iterateRawDevices( int numrawdevices, LIBMTP_raw_device_t* rawdevices, const QString &serial )
{

    bool success;

    LIBMTP_mtpdevice_t *device;
            // test raw device for connectability
    for(int i = 0; i < numrawdevices; i++)
    {

        kDebug() << "Opening raw device number: " << (i+1);
        device = LIBMTP_Open_Raw_Device(&rawdevices[i]);
        if (device == NULL) {
            kDebug() << "Unable to open raw device: " << (i+1);
            success = false;
            LIBMTP_Release_Device( device );
            continue;
        }

        kDebug() << "Testing serial number";

        QString mtpSerial = QString::fromUtf8( LIBMTP_Get_Serialnumber( device ) );
        if( !mtpSerial.contains(serial) )
        {
            kDebug() << "Wrong device, going to next";
            kDebug() << "Expected: " << serial << " but got: " << mtpSerial;
            success = false;
            LIBMTP_Release_Device( device );
            continue;
        }

        kDebug() << "Correct device found";
        success = true;
        break;
    }

    m_device = device;

    if( m_device == 0 ) {
        // TODO: error protection
        success = false;
        free( rawdevices );

    }

    //QString serial = QString::fromUtf8( LIBMTP_Get_Serialnumber( m_device ) );

    kDebug() << "Serial is: " << serial;

    kDebug() << "Success is: " << ( success ? "true" : "false" );

    return success;
}

void MtpDevice::disconnectDevice()
{

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

/////

WorkerThread::WorkerThread( int numrawdevices, LIBMTP_raw_device_t* rawdevices, const QString &serial, MtpDevice* handler )
    : ThreadWeaver::Job()
    , m_success( false )
    , m_numrawdevices( numrawdevices )
    , m_rawdevices( rawdevices )
    , m_serial( serial )
    , m_handler( handler )
{
    connect( this, SIGNAL( failed( ThreadWeaver::Job* ) ), m_handler, SLOT( slotDeviceMatchFailed( ThreadWeaver::Job* ) ) );
    connect( this, SIGNAL( done( ThreadWeaver::Job* ) ), m_handler, SLOT( slotDeviceMatchSucceeded( ThreadWeaver::Job* ) ) );
    connect( this, SIGNAL( done( ThreadWeaver::Job* ) ), this, SLOT( deleteLater() ) );
}

WorkerThread::~WorkerThread()
{
    //nothing to do
}

bool WorkerThread::success() const
{
    return m_success;
}

void WorkerThread::run()
{
    m_success = m_handler->iterateRawDevices( m_numrawdevices, m_rawdevices, m_serial );
}

#include "MtpDevice.moc"
