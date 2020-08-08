#include "ahaSoundInterface.h"
#include "ahaCommonUtility.h"

#define DEFAULT_SAMPLE_RATE 44100

CSoundInterface::CSoundInterface()
{
    //we only need to initialize the timer here.
    m_OpenedAudioSource = ENone;
    m_pAudioRouter = new GSTAudioRouterDBus("org.lge.GSTAudioRouterDBus", "/",
                                            QDBusConnection::sessionBus(), 0);
    m_nCurrentSampleRate_Front = DEFAULT_SAMPLE_RATE;
    m_nCurrentSampleRate_Rear = DEFAULT_SAMPLE_RATE;
    m_nOpenedSampleRate = 0;

    m_nVehicleType = 1;

    LOG_INIT_V2("Aha");
}

CSoundInterface::~CSoundInterface()
{
    m_OpenedAudioSource = ENone;
    if(m_pAudioRouter)
    {
        delete m_pAudioRouter;
        m_pAudioRouter = NULL;
    }
}

void CSoundInterface::setVehicleType(int nVehicleType)
{
    m_nVehicleType = nVehicleType;
}

void CSoundInterface::resetSampleRate(EIPodPorts nCurrIPodPortNum)
{
    if(nCurrIPodPortNum == eIPOD_PORT_1)
    {
        m_nCurrentSampleRate_Front = DEFAULT_SAMPLE_RATE;
    }
    else if (nCurrIPodPortNum == eIPOD_PORT_2)
    {
        m_nCurrentSampleRate_Rear = DEFAULT_SAMPLE_RATE;
    }
}

void CSoundInterface::changeSampleRate(EIPodPorts nCurrIPodPortNum, int nSampleRate)
{
    TAN_PRINTF("CSoundInterface::changeSampleRate %d\n ", nSampleRate);

    if(nCurrIPodPortNum == eIPOD_PORT_1)
    {
        m_nCurrentSampleRate_Front = nSampleRate;
    }
    else if (nCurrIPodPortNum == eIPOD_PORT_2)
    {
        m_nCurrentSampleRate_Rear = nSampleRate;
    }

    EIPodPorts nOpenedPort;
    nOpenedPort = AudioSourceToPort(m_OpenedAudioSource);
    if(m_OpenedAudioSource != ENone && nOpenedPort == nCurrIPodPortNum)
    {
        //The sound for this port already opened, but the frequency has changed.
        //Need to repoen the sound.
        TAN_PRINTF("Reopening the sound\n");
        openSoundChannel(nOpenedPort);
    }
}

void CSoundInterface::openSoundChannel(EIPodPorts nCurrIPodPortNum)
{
    int nSampleRate = DEFAULT_SAMPLE_RATE;
    QString strSoundSource;
    EAudioSource eAudioSource;

    if(nCurrIPodPortNum == eIPOD_PORT_1)
    {
        nSampleRate = m_nCurrentSampleRate_Front;
    }
    else if(nCurrIPodPortNum == eIPOD_PORT_2)
    {
        nSampleRate = m_nCurrentSampleRate_Rear;
    }
    QString strSampleRate = QString::number(nSampleRate);


    FindAudioDevice(nCurrIPodPortNum, strSoundSource);
    eAudioSource = PortToAudioSource(nCurrIPodPortNum);

    if(m_OpenedAudioSource != eAudioSource || m_nOpenedSampleRate != nSampleRate)
    {
        TAN_PRINTF("### AquireAudioSource: OK   strSampleRate = %s \n", strSampleRate.toUtf8().data());
        LOG_HIGH<< "### AquireAudioSource: OK   nSampleRate =" << nSampleRate << LOG_ENDL;

        m_pAudioRouter->AquireAudioSource(strSoundSource, strSampleRate, eAudioSource);
        m_OpenedAudioSource = eAudioSource;
        m_nOpenedSampleRate = nSampleRate;
    }
    else
    {
        TAN_PRINTF("### AquireAudioSource: ALREADY ACQUIRED \n");
        LOG_HIGH<< "### AquireAudioSource: ALREADY ACQUIRED" <<  LOG_ENDL;
    }
}

/*
*********************************************
// updateSampleRate : read the sample rate from the file
// added by Ryu - 2013.07.01
***********************************************
*/
void CSoundInterface::updateSampleRate(EIPodPorts nCurrIPodPortNum)
{
    LOG_HIGH<< "CSoundInterface::updateSampleRate  nCurrIPodPortNum ->" << nCurrIPodPortNum << LOG_ENDL;
    TAN_PRINTF("CSoundInterface::updateSampleRate\n");

    QFile sampleRateFile;
    QString strFilePath;
    QString strSampleRate;
    int nSampleRate = DEFAULT_SAMPLE_RATE;

    if(nCurrIPodPortNum == eIPOD_PORT_1)
    {
        strFilePath = "/app/data/AppMedia/share/sampleRate_1.txt";
    }
    else if (nCurrIPodPortNum == eIPOD_PORT_2)
    {
        strFilePath = "/app/data/AppMedia/share/sampleRate_2.txt";
    }
    else
    {
        TAN_PRINTF("\n Error - Not eIPOD_PORT_1 / 2  \n");
        return;
    }


    if(!QFile::exists(strFilePath))
    {
        TAN_PRINTF("First read - %s  not exist!!!! then Retry!!! \n", strFilePath.toUtf8().data());
        LOG_HIGH<< "First read" << strFilePath << " not exist!!!! then Retry!!!" << LOG_ENDL;

        sleep(2);   // time sleep for the writing the sample-rate by ipodcontroller
        if(!QFile::exists(strFilePath))
        {
            TAN_PRINTF("Second read - %s  not exist!!!! \n", strFilePath.toUtf8().data());
            LOG_HIGH<< "Second read" << strFilePath << " not exist!!!!" << LOG_ENDL;
            return;
        }
    }

    sampleRateFile.setFileName(strFilePath);
    if(sampleRateFile.open(QIODevice::ReadOnly))
    {
        QTextStream textStream (&sampleRateFile);
        textStream >> strSampleRate;
        nSampleRate  = strSampleRate.toInt();
        sampleRateFile.close();

        LOG_HIGH<< "Success!! UpdateSampleRate from the file. nSampleRate = " << nSampleRate << LOG_ENDL;
        TAN_PRINTF("Success!! UpdateSampleRate from the file. nSampleRate = %d\n ", nSampleRate);
    }
    else
    {
        TAN_PRINTF("Error - do not open the sampleRate file!!!");
        LOG_HIGH<< "Error - do not open the sampleRate file!!!" << LOG_ENDL;
    }

    if(nCurrIPodPortNum == eIPOD_PORT_1)
    {
       m_nCurrentSampleRate_Front = nSampleRate;
    }
    else if (nCurrIPodPortNum == eIPOD_PORT_2)
    {
        m_nCurrentSampleRate_Rear = nSampleRate;
    }

    return;
}

EAudioSource CSoundInterface::PortToAudioSource(EIPodPorts nCurrIPodPortNum)
{
    EAudioSource eAudioSource;
    switch(nCurrIPodPortNum)
    {
    case eIPOD_PORT_1:
        eAudioSource = EAhaFront;
        break;
    case eIPOD_PORT_2:
        eAudioSource = EAhaRear;
        break;
    default:
        eAudioSource = ENone;
        break;
    }
    return eAudioSource;
}


EIPodPorts CSoundInterface::AudioSourceToPort(EAudioSource eAudioSource)
{
    EIPodPorts nCurrIPodPortNum;
    switch(eAudioSource)
    {
    case EAhaFront:
        nCurrIPodPortNum = eIPOD_PORT_1;
        break;
    case EAhaRear:
        nCurrIPodPortNum = eIPOD_PORT_2;
        break;
    default:
        nCurrIPodPortNum = eIPOD_PORT_NONE;
        break;
    }
    return nCurrIPodPortNum;
}

void CSoundInterface::closeSoundChannel()
{
    TAN_PRINTF("\n\n#########################\n");
    TAN_PRINTF("[TAN] closeSoundChannel, eAudioSource: %d\n", m_OpenedAudioSource);
    if(m_OpenedAudioSource != ENone)
    {
        TAN_PRINTF("[TAN] ReleaseAudioSource: OK\n");
        m_pAudioRouter->ReleaseAudioSource(m_OpenedAudioSource);
        m_OpenedAudioSource = ENone;
        m_nOpenedSampleRate = 0;
    }
    else
    {
        TAN_PRINTF("[TAN] ReleaseAudioSource: ALREADY RELEASED\n");
    }
    TAN_PRINTF("#########################\n\n");
}


//Copy from IPodController
//Coded by jonghwan.cho
//Edited by Tan to fit the Aha.
void CSoundInterface::FindAudioDevice(EIPodPorts nCurrIPodPortNum, QString &strSoundSource)
{
    QString     qnode;
    EIPodPorts	portNum;

    struct udev*            udev            = 0;
    struct udev_enumerate*  enumerate       = 0;
    struct udev_list_entry* devices         = 0;
    struct udev_list_entry* dev_list_entry  = 0;
    struct udev_device*     soundDev        = 0;
    struct udev_device*     dev             = 0;

    const char* manufacturer;
    const char* vendorID;
    const char* model;
    const char* syspath;

    //TAN_PRINTF("FindAudioDevice\n");

    strSoundSource.clear();

    /* Create the udev object */
    udev = udev_new();

    if( udev == NULL ) {
        TAN_PRINTF("Cannot create audio device\n");
    }

    /* Create a list of the devices in the 'usb' subsystem. */
    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "sound");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);
    /* For each item enumerated, print out its information.
    udev_list_entry_foreach is a macro which expands to
    a loop. The loop will be executed for each member in
    devices, setting dev_list_entry to a list entry
    which contains the device's path in /sys. */

    udev_list_entry_foreach(dev_list_entry, devices)
    {
        const char *path, *node;

        /* Get the filename of the /sys entry for the device
        and create a udev_device object (dev) representing it */
        path      = udev_list_entry_get_name(dev_list_entry);
        soundDev  = udev_device_new_from_syspath(udev, path);
        dev		  = udev_device_get_parent_with_subsystem_devtype(
                    soundDev,
                    "usb",
                    "usb_device");
        node = udev_device_get_devnode(dev);

        if( node != 0 )
        {
            qnode = node;
            //TAN_PRINTF("node : %s", node);

            manufacturer = udev_device_get_sysattr_value(dev, "manufacturer");
            vendorID = udev_device_get_sysattr_value(dev, "idVendor");
            QString serialNum = udev_device_get_sysattr_value(dev, "serial");
            QString card_num(udev_device_get_sysattr_value(soundDev, "number"));
            model = udev_device_get_property_value(dev, "ID_MODEL");
            syspath = udev_device_get_syspath(dev);

            if( card_num.isEmpty() == true )		continue;

            //TAN_PRINTF("manufacturer: %s\n", manufacturer);
            //TAN_PRINTF("vendorID: %s\n", vendorID);
            //TAN_PRINTF("serialNum: %s\n", serialNum.toUtf8().data());
            //TAN_PRINTF("card_num: %s\n", card_num.toUtf8().data());
            //TAN_PRINTF("model: %s\n", model);
            //TAN_PRINTF("syspath: %s\n", syspath);

            if( manufacturer && model )
            {
                //VendorID is "05ac" for all Apple devices
                if ( strncmp(vendorID,"05ac",4) == 0 &&
                     ( strncmp(model,"iPod",4) == 0
                       || strncmp(model,"iPad",4) == 0
                     ||strncmp (model,"iPhone",6) == 0 ))
                {
                    portNum = GetUSBPortNumber( syspath );

                    if( portNum == nCurrIPodPortNum )
                    {
                        QString gstPath("plughw:");
                        QString device_num(",0");
                        gstPath.append(card_num);
                        gstPath.append(device_num);

                        strSoundSource = gstPath;
                        //TAN_PRINTF("gstPath: %s\n", gstPath.toUtf8().data());
                    }
                }
            }
        }
    }
    /* Free the enumerator object */
    udev_enumerate_unref(enumerate);
    /* Free the udev object */
    udev_unref(udev);
}

//copy from ipod controller
EIPodPorts CSoundInterface::GetUSBPortNumber(QString sysPath)
{
    EIPodPorts portNum = eIPOD_PORT_NONE;

    // Read the 8th section in the absolute path of the iPod connection
    // which specifies, whether the iPod is connected to USB1 or USB2
    QString portString = sysPath.section( '/' ,8 ,8 );

    // Checking for the USB 1 or USB 2
    if(m_nVehicleType != 0)
    {
        //15MY, DH PE
        portNum = ( portString.compare( "1-1" ) ) ?
                ( ( portString.compare( "2-1" ) ) ? eIPOD_PORT_NONE : eIPOD_PORT_2 ) : eIPOD_PORT_1;

    }
    else
    {
        //DH
        portNum = ( portString.compare( "2-2" ) ) ?
                ( ( portString.compare( "2-1" ) ) ? eIPOD_PORT_NONE : eIPOD_PORT_2 ) : eIPOD_PORT_1;
    }
    //TAN_PRINTF("portNum: %d\n", portNum);
    //TAN_PRINTF("portString: %s\n", portString.toUtf8().data());
    return portNum;
}


