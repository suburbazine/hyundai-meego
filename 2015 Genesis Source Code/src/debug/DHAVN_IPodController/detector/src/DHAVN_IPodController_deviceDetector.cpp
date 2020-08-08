/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                 This is the main detector class any
 *                 device connected or removed from the USB       *
 *                 will be notified here and controller will take *
 *                 action according to this.                      *
 *                 emits the signal on connection and removal of  *
 *                 the device                                     *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodController_deviceDetector.h>
#include <DHAVN_IPodController_CommonVars.h>

#include <DHAVN_IPodController_Def.h>
#include <DHAVN_IPodController_Performance.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>

CDeviceDetector::CDeviceDetector(QObject* parent, EIPodPorts portNum) : QObject(parent)
{
    switch(portNum)
    {
        case eIPOD_PORT_1:
            LOG_INIT_V2(giPodControllerLogName_1);
            piPodLog = new iPodLogger(NULL, 1); // added by sungha.choi 2013.04.12
            break;
        case eIPOD_PORT_2:
            LOG_INIT_V2(giPodControllerLogName_2);
            piPodLog = new iPodLogger(NULL, 2); // added by sungha.choi 2013.04.12
            break;
        default:
            LOG_INIT_V2(giPodControllerLogName);
            piPodLog = new iPodLogger(NULL, 0); // added by sungha.choi 2013.04.12
    }
    //LOG_TRACE << "CDeviceDetector::CDeviceDetector() IN" << LOG_ENDL;
    setObjectName(gDetector);

    m_detectorState = eiPodDeviceDisconnected;
    m_VendorId      = 0;
    m_ProductId     = 0;
    m_Udev          = NULL;
    m_Notifier      = NULL;
    m_Mon           = NULL;
    m_DisconnectCompletion = true;    // added by sungha.choi 2013.04.02
    m_DetectionAvailable   = true;
    m_SavedDetection       = false;

    m_iPodAudioDevPath     = "";
    m_DeviceConnected      = false; // Added by jonghwan.cho@lge.com 2013.05.06. for bootup iPod Connecting

    connect(this, SIGNAL(destroyTaskHandler()), parent, SLOT(destroyTaskHandlerSlot()), Qt::QueuedConnection);  // modified by sungha.choi 2013.04.12

    m_isAudioPathSet       = false;
    m_currentIPodPortNum   = portNum;
    m_IsDeviceReset        = false;
    m_NodePath             = QString();

    m_UsbConfSetter = new CUsbConfigurationSetter(this);
    if( m_UsbConfSetter == NULL )
    {
        LOG_HIGH << "CDeviceDetector::CDeviceDetector(), Failed to alloc m_UsbConfSetter"<<LOG_ENDL;
        return;
    }

    m_HidDevDetector = new CHidDeviceDetector(this, m_currentIPodPortNum);
    if( m_HidDevDetector == NULL )
    {
        LOG_HIGH << "CDeviceDetector::CDeviceDetector(), Failed to alloc m_HidDevDetector" << LOG_ENDL;
        return;
    }

    m_AudioDevDetector = new CAudioDeviceDetector(this, m_currentIPodPortNum);
    if( m_AudioDevDetector == NULL )
    {
        LOG_HIGH << "CDeviceDetector::CDeviceDetector()  Failed to alloc m_AudioDevDetector"<<LOG_ENDL;
        return;
    }
    connect(m_HidDevDetector,   SIGNAL(hidDetetcted(QString)),  this, SLOT(hidDeviceDetetcedSlot(QString)));
    connect(m_AudioDevDetector, SIGNAL(audioDetected(QString)), this, SLOT(audioDeviceDetetcedSlot(QString)) );
    connect(m_AudioDevDetector, SIGNAL(onIPodAudioDeviceConnectedSignal(QString, QString, EIPodPorts)), this, SLOT(onIPodAudioDeviceConnectedSLOT(QString, QString, EIPodPorts )) );

    if( FindDevice() == true )
    {
        LOG_HIGH << "CDeviceDetector::CDeviceDetector(), FindDevice() = true, m_iPodAudioDevPath =" << m_iPodAudioDevPath << LOG_ENDL;
	}
    else
    {
        //LOG_TRACE << "   FindDevice() = false,  Now is not connecting Apple Device" << LOG_ENDL;
    }

    //LOG_TRACE << "CDeviceDetector::CDeviceDetector() OUT" << LOG_ENDL;
}

CDeviceDetector::~CDeviceDetector()
{
    //LOG_TRACE << "CDeviceDetector::~CDeviceDetector() IN" << LOG_ENDL;
    if(m_Notifier)       delete m_Notifier;
    if(m_Mon)            udev_monitor_unref (m_Mon);
    if(m_Udev)           udev_unref (m_Udev);
    if(m_HidDevDetector) delete m_HidDevDetector;
    //LOG_TRACE << "CDeviceDetector::~CDeviceDetector() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CDeviceDetector::registerForTheNotification()                   (int) (OUT), (void) (IN)
//
// Explanation
// Register the controller for the notification on any read in the usb descriptor on device
// connect
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CDeviceDetector::registerForTheNotification()
{
    //LOG_TRACE << "CDeviceDetector::registerForTheNotification() IN" << LOG_ENDL;

    if(m_Udev)
        return 0;

    /* Check if the device is already connected*/
    startEnumerationAndSetDevices();

    int ret;
    /* Create the udev object */
    m_Udev = udev_new();
    if (!m_Udev)
    {
        LOG_HIGH << "CDeviceDetector::registerForTheNotification(), Can't create udev" << LOG_ENDL;
        return 0;
    }

    /* Set up a monitor to monitor hidraw devices */
    m_Mon = udev_monitor_new_from_netlink(m_Udev, "udev");
    if(!m_Mon)
    {
        LOG_HIGH << "CDeviceDetector::registerForTheNotification(), Failed to get monitor" << LOG_ENDL;
        return -1;
    }

    ret = udev_monitor_filter_add_match_subsystem_devtype(m_Mon, "usb", "usb_device");
    if(ret != 0)
    {
        LOG_HIGH << "CDeviceDetector::registerForTheNotification(), Failed to add match" << LOG_ENDL;
    }
    ret = udev_monitor_enable_receiving(m_Mon);
    if(ret != 0)
    {
        LOG_HIGH << "CDeviceDetector::registerForTheNotification(), Failed to enable monitor" << LOG_ENDL;
        return ret;
    }
    else
    {
        int fd = 0;
        fd = udev_monitor_get_fd(m_Mon);
        m_Notifier = new QSocketNotifier(fd, QSocketNotifier::Read ,this);
        if(m_Notifier == NULL)
        {
            LOG_HIGH << "CDeviceDetector::registerForTheNotification: Failed to alloc m_Notifier" << LOG_ENDL;
            return -1;
        }
        connect(m_Notifier, SIGNAL(activated(int)), this, SLOT(iPodDeviceDetectedSlot()));
        LOG_HIGH << "CDeviceDetector, Notifier is activated, fd = " << fd << LOG_ENDL;
        //LOG_TRACE << "   success to enable monitor" << LOG_ENDL;
    }

    //LOG_TRACE << "CDeviceDetector::registerForTheNotification() OUT" << LOG_ENDL;
    return 0;

}

bool CDeviceDetector::isUnsupportedDevice(int nProductId)
{
    //if it is one of the unsupported device return true.
    /*
        0x1201, //  3G iPod
        0x1202, //  iPod 2G
        0x1203, //  iPod 4.Gen Grayscale 40G
        0x1204, //  iPod [Photo]
        0x1205, //  iPod Mini 1.Gen/2.Gen
        0x1206, //  iPod '06'
        0x1207, //  iPod '07'
        0x1208, //  iPod '08'
        0x1209, //  iPod Video

        0x1300, //  iPod Shuffle
        0x1301, //  iPod Shuffle 2.Gen
        0x1302, //  iPod Shuffle 3.Gen
        0x1303, //  iPod Shuffle 4.Gen
    */
    bool isAncientiPod = (nProductId >= 0x1201 && nProductId <= 0x1209);
    bool isiPodShuffle = (nProductId >= 0x1300 && nProductId <= 0x1303);
    if(isAncientiPod || isiPodShuffle)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/******************************************************************************
// Function Name                                            Parameters
// CDeviceDetector::deviceDetected()                   (void) (OUT), (void) (IN)
//
// Explanation
// Slot, which will be called when the notification of the device has come, it
// will emit signal on the basis of the action
// will set the device of the config setter
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CDeviceDetector::iPodDeviceDetectedSlot()
{
    //LOG_TRACE << "CDeviceDetector::iPodDeviceDetectedSlot() IN" << LOG_ENDL;

    if(m_DetectionAvailable == false) 
    {    // added by sungha.choi 2013.04.02
        m_SavedDetection = true;
        LOG_HIGH << "CDeviceDetector::iPodDeviceDetectedSlot() m_DetectionAvailable = false" << LOG_ENDL;
        return;
    }

    PT_START_TIME(PT_OBJ_DEVICE_DETECT, 0);
    
    struct udev_device *Dev = NULL;
    bool isConnected = false;

    Dev = udev_monitor_receive_device(m_Mon);
    if(Dev == NULL)
    {
        //LOG_TRACE << "   Failed to get the Monitor Handle" << LOG_ENDL;
        return ;
    }

    EIPodPorts PortNo = eIPOD_PORT_NONE;
    QString SysPath;
    SysPath.append(udev_device_get_syspath(Dev));
    PortNo = getPortNum(SysPath);

    if((PortNo != m_currentIPodPortNum) &&
         (eIPOD_PORT_NONE != m_currentIPodPortNum) )
    {
        //LOG_TRACE << "   Not my port <=========" << LOG_ENDL;
        return;
    }

    const char remove[] = "remove";
    const char add[] = "add";
    const char* action = udev_device_get_action(Dev);

    if(!strcmp(action, remove))
    {
        //LOG_TRACE << "   removal has happeded" << LOG_ENDL;
        if( (eiPodDeviceDisconnected != m_detectorState) && !isAppleDeviceConnected())
        {
            LOG_HIGH << "   Apple device is disconnected, emit iPodDeviceDisconnected() "  << LOG_ENDL;
            //piPodLog->WriteLog("CDeviceDetector::iPodDeviceDetectedSlot : eiPodDeviceDisconnected");
            m_detectorState = eiPodDeviceDisconnected;
            onDeviceDisconnect();
            emit iPodDeviceDisconnected();
        }
        else
        {
            LOG_HIGH << "   Apple device is NOT disconnected" << LOG_ENDL;
        }
    }

    if(!strcmp(action, add))
    {
        if(eiPodDeviceDisconnected == m_detectorState)
        {
            LOG_HIGH << "   addition has happeded" << LOG_ENDL;
            //piPodLog->WriteLog("CDeviceDetector::iPodDeviceDetectedSlot : addition");
            isConnected = true;
        }
        else
        {
            LOG_HIGH << "   A device is connected, but detector is already in connected state!!!!!!" <<LOG_ENDL;
        }
    }

    if (Dev && isConnected)
    {
        //LOG_TRACE <<"   Got Device" << LOG_ENDL;
        //LOG_INFO << "   Node      =" << udev_device_get_devnode(Dev)   << LOG_ENDL;
        //LOG_INFO << "   SysPath   =" << udev_device_get_syspath(Dev)   << LOG_ENDL;
        //LOG_INFO << "   Subsystem =" << udev_device_get_subsystem(Dev) << LOG_ENDL;
        //LOG_INFO << "   Devtype   =" << udev_device_get_devtype(Dev)   << LOG_ENDL;
        //LOG_INFO << "   Action    =" << udev_device_get_action(Dev)    << LOG_ENDL; /* Devie add, remove , modified etc*/

        QString VendorId;
        QString ProductId;

        VendorId.append(udev_device_get_sysattr_value(Dev,"idVendor"));
        ProductId.append(udev_device_get_sysattr_value(Dev, "idProduct"));

        bool ok;
        m_VendorId = VendorId.toInt(&ok, 16);
        m_ProductId = ProductId.toInt(&ok, 16);

        /* if the vendor id matches then set the devices for the both coNFIG SETTER AND THE RAW PACKET HANDLER*/
        if(m_VendorId == VENDOR_ID && isUnsupportedDevice(m_ProductId)==false)
        {
            //LOG_TRACE << "   VENDOR_ID Match Found... m_VendorId = " << m_VendorId << LOG_ENDL;
            ////LOG_TRACE << "iPodPerformanceMeasurement:CDeviceDetector::iPodDeviceDetectedSlot: iPod Connected-Start"<<LOG_ENDL;

            EIPodPorts portNum = eIPOD_PORT_NONE;
            QString    sysPath;
            sysPath.append(udev_device_get_syspath(Dev));
            portNum = getPortNum( sysPath );

            if((portNum == m_currentIPodPortNum)||( eIPOD_PORT_NONE == m_currentIPodPortNum ))
            {
                m_NodePath.clear();
                m_NodePath.append(udev_device_get_devnode(Dev));

                //LOG_TRACE << "   port match, PortNum = " << m_currentIPodPortNum << LOG_ENDL;
                piPodLog->WriteLog("CDeviceDetector::iPodDeviceDetectedSlot : eiPodDeviceConnected");
                m_detectorState = eiPodDeviceConnected;
                //LOG_TRACE << "   emit iPodDeviceDetetcted( m_VendorId = " << m_VendorId << ", m_ProductId = " << m_ProductId << ")"  << LOG_ENDL;
                emit iPodDeviceDetetcted(m_VendorId, m_ProductId);

                int error = IPOD_ERROR_GENERIC;
                if(m_UsbConfSetter)
                {
                    error = m_UsbConfSetter->setDevice(m_VendorId, m_ProductId);
                }
                if(error)
                {
                    if(error == errDeviceIsAlreadyInHIDMode)
                    {
                        /* if ipod is connected with already config=2, then there is change of getting hid detect event first
                           and ipod connect event next. in this case we will not process the hid detect event. So enumerate the
                           hid always if config is already 2 hence we will get hid detect event after ipod detect. */
                        if(m_HidDevDetector->startEnumerationAndSetDevicesForHIDMode() != IPOD_SUCCESS)
                        {
                            QString path = "";
                            emit iPodHidDetected( false, path );
                        }
                    }
                    else  //As configuration change is failed, HID swithch will not happen.
                    {
                        //LOG_TRACE <<"   Failed to change the configuration!!"<< LOG_ENDL;
                        QString path = "";
                        emit iPodHidDetected( false, path );
                    }
                    udev_device_unref(Dev);
                    //LOG_TRACE << "CDeviceDetector::iPodDeviceDetectedSlot() OUT " << LOG_ENDL;
                    return;
                }// END -if(error)
            }//if((portNum == m_currentIPodPortNum)||( eIPOD_PORT_NONE == m_currentIPodPortNum ))
            else 
            {
                //LOG_TRACE << "   port num mismatch, m_currentIPodPortNum = "<<m_currentIPodPortNum<<", portNum ="<<portNum<< LOG_ENDL;
            }
        }
    }
    else
    {
        //LOG_TRACE << "   No Device from receive_device(). An error occured." << LOG_ENDL;
    }

    if( Dev )
    {
        udev_device_unref(Dev);
        Dev = NULL;
    }

    //LOG_TRACE << "CDeviceDetector::iPodDeviceDetectedSlot() OUT" << LOG_ENDL;
}


/******************************************************************************
// Function Name                                            Parameters
// CDeviceDetector::hidDeviceDetetced()                   (void) (OUT), (void) (IN)
//
// Explanation
// Slot, which will be called when the notification of the HID device has come, it
// will emit signal that detection process is finished after setting the
// device of the raw packet handler
// will set the device of the raw packet handler
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CDeviceDetector::hidDeviceDetetcedSlot(QString path)
{
    //LOG_TRACE << "CDeviceDetector::hidDeviceDetetcedSot() IN, path = " << path << LOG_ENDL;
    //LOG_TRACE << "   m_detectorState = " << m_detectorState << LOG_ENDL;

    switch(m_detectorState)
    {
        case eiPodDeviceDisconnected:
        case eiPodHidDeviceConnected:
            break;
        case eiPodDeviceConnected:
            m_detectorState = eiPodHidDeviceConnected;
            //LOG_HIGH << "CDeviceDetector::hidDeviceDetetced(), emit iPodHidDetected(true, path = " << path << ")"  << LOG_ENDL;
            emit iPodHidDetected(true, path);
            break;
        default:
            m_detectorState = eiPodDeviceDisconnected;
            break;
    }
    //LOG_TRACE << "CDeviceDetector::hidDeviceDetetced() OUT" << LOG_ENDL;
}

void CDeviceDetector::onIPodAudioDeviceConnectedSLOT( QString node, QString serialNum, EIPodPorts portNum )
{
    //LOG_TRACE << "CDeviceDetector::onIPodAudioDeviceConnectedSLOT() IN" << LOG_ENDL;
    //LOG_HIGH  << "CDeviceDetector::onIPodAudioDeviceConnectedSLOT(), emit onIPodDeviceConnectedSignal( node, serialNum, portNum )" << LOG_ENDL;
    LOG_HIGH  << "emit onIPodDeviceConnectedSignal node = " << node << ", serial = " << serialNum << ", port = " << portNum << LOG_ENDL;
    emit onIPodDeviceConnectedSignal( node, serialNum, portNum );
    //LOG_TRACE << "CDeviceDetector::onIPodAudioDeviceConnectedSLOT() OUT" << LOG_ENDL;
}

void CDeviceDetector::audioDeviceDetetcedSlot(QString path )
{
	//LOG_TRACE << "CDeviceDetector::audioDeviceDetetcedSlot() IN path = " << path << LOG_ENDL;

    if(m_isAudioPathSet)
    {
        return; //Already set, return;
    }

    if(path.length() != 10)
    {
        return; // Invalid path, path name should be 10 charactors like plughw:1,0 .
    }

    m_iPodAudioDevPath = path;
    m_isAudioPathSet = true;

	//LOG_TRACE << "CDeviceDetector::audioDeviceDetetcedSlot() OUT" << LOG_ENDL;
    //emit iPodAudioDeviceDetected(path);
}

/******************************************************************************
// Function Name                                            Parameters
// CDeviceDetector::startEnumerationAndSetDevices()         (void) (OUT), (void) (IN)
//
// Explanation
// Single time use at the time of the controller startup, will enumerate the
// USB devices if already connected the desired device then set the devices to the
// raw packet handler and config setter.
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CDeviceDetector::startEnumerationAndSetDevices()
{
    //LOG_TRACE << "CDeviceDetector::startEnumerationAndSetDevices() IN" << LOG_ENDL;

    struct udev *udev = NULL;
    struct udev_enumerate *enumerate = NULL;
    struct udev_list_entry *devices = NULL, *dev_list_entry = NULL;
    struct udev_device *dev = NULL;

    /* Create the udev object */
    udev = udev_new();
    if (!udev)
    {
         return;
    }

    /* Create a list of the devices in the 'hidraw' subsystem. */
    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "usb");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);

    udev_list_entry_foreach(dev_list_entry, devices)
    {
        const char *path;

        /* Get the filename of the /sys entry for the device
                       and create a udev_device object (dev) representing it */
        path = udev_list_entry_get_name(dev_list_entry);
        dev = udev_device_new_from_syspath(udev, path);

        if (!dev)
        {
            LOG_HIGH << "CDeviceDetector::startEnumerationAndSetDevices() Unable to find parent usb device." << LOG_ENDL;
            udev_enumerate_unref(enumerate);
            udev_unref(udev);
            return;
        }

        QString VendorId;
        QString ProductId;
        VendorId.append(udev_device_get_sysattr_value(dev,"idVendor"));
        ProductId.append(udev_device_get_sysattr_value(dev, "idProduct"));
        bool ok;
        m_VendorId = VendorId.toInt(&ok, 16);
        m_ProductId = ProductId.toInt(&ok, 16);
        /* if the vendor id matches then set the devices for the both coNFIG SETTER AND THE RAW PACKET HANDLER*/
        if(m_VendorId == VENDOR_ID && isUnsupportedDevice(m_ProductId)==false)
        {
            //LOG_TRACE << "   VENDOR_ID Match Found..."<< LOG_ENDL;

            EIPodPorts portNum = eIPOD_PORT_NONE;
            QString sysPath;
            sysPath.append(udev_device_get_syspath(dev));
            portNum = getPortNum(sysPath);

            if((portNum == m_currentIPodPortNum)||( eIPOD_PORT_NONE == m_currentIPodPortNum ))
            {
                //LOG_TRACE << "   PortMatch, m_currentIPodPortNum = "<<m_currentIPodPortNum<<LOG_ENDL;

                // { added by sungha.choi 2013.04.02 for issue port1&2 attached booting, remove port2, port1 also removed.
                m_NodePath.clear();
                m_NodePath.append(udev_device_get_devnode(dev));
                //LOG_TRACE << "   current node path = " << m_NodePath << LOG_ENDL;
                // } added by sungha.choi 2013.04.02

                LOG_HIGH << "CDeviceDetector::startEnumerationAndSetDevices(), emit iPodDeviceDetetcted(), m_VendorId = " << m_VendorId << ", m_ProductId = " << m_ProductId << LOG_ENDL;
                emit iPodDeviceDetetcted(m_VendorId,m_ProductId);
                m_detectorState = eiPodDeviceConnected;
                int error = IPOD_ERROR_GENERIC;

                if(m_UsbConfSetter)
                {
                    // as error is not used and having a constant value,below if(error == errDeviceIsAlreadyInHIDMode) never get called.
                    // so depending the device status, the remaining functionality will be performed. Coverity fix-Deadcode.
                    error = m_UsbConfSetter->setDevice(m_VendorId, m_ProductId);
                }

                if(error)
                {
                    if(error == errDeviceIsAlreadyInHIDMode)
                    {
                        if(m_HidDevDetector->startEnumerationAndSetDevicesForHIDMode() != IPOD_SUCCESS)
                        {
                            QString qPath = "";
                            emit iPodHidDetected(false,qPath);
                        }
                    }
                    else//As configuration change is failed, HID swithch will not happen.
                    {
                        LOG_HIGH<<"   Failed to change the configuration!!"<<LOG_ENDL;
                        QString qPath = "";
                        emit iPodHidDetected(false,qPath);
                    }
                    //LOG_TRACE << "   same config is set to device so called the setdevice of the raw paket handler" << LOG_ENDL;
                    udev_device_unref(dev);
                    break;
                }
                else
                {
                    udev_device_unref(dev);
                    break;
                }
            }
        }
        udev_device_unref(dev);
    }
    /* Free the enumerator object */
    udev_enumerate_unref(enumerate);

    udev_unref(udev);
    //LOG_TRACE << "CDeviceDetector::startEnumerationAndSetDevices() OUT"<< LOG_ENDL;
}

bool CDeviceDetector::isAppleDeviceConnected()
{
    //LOG_TRACE << "CDeviceDetector::isAppleDeviceConnected() IN"<< LOG_ENDL;

    bool connected = false;
    struct udev *udev = NULL;
    struct udev_enumerate *enumerate  = NULL;
    struct udev_list_entry *devices = NULL, *dev_list_entry = NULL;
    struct udev_device *dev = NULL;

    /* Create the udev object */
    udev = udev_new();
    if (!udev)
    {
        //LOG_TRACE << "   Can't create udev" << LOG_ENDL;
        return false;
    }

    /* Create a list of the devices in the 'hidraw' subsystem. */
    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "usb");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);

    udev_list_entry_foreach(dev_list_entry, devices)
    {
        const char *path;

        /* Get the filename of the /sys entry for the device
                       and create a udev_device object (dev) representing it */
        path = udev_list_entry_get_name(dev_list_entry);
        dev = udev_device_new_from_syspath(udev, path);

        if (!dev)
        {
            //LOG_TRACE << "   Unable to find parent usb device" << LOG_ENDL;
            return false;
        }

        QString VendorId;
        QString ProductId;
        VendorId.append(udev_device_get_sysattr_value(dev,"idVendor"));
        ProductId.append(udev_device_get_sysattr_value(dev, "idProduct"));
        bool ok;
        m_VendorId = VendorId.toInt(&ok, 16);
        m_ProductId = ProductId.toInt(&ok, 16);

        /* if the vendor id matches then set the devices for the both coNFIG SETTER AND THE RAW PACKET HANDLER*/
        if(m_VendorId == VENDOR_ID)
        {
            //LOG_TRACE << "   VENDOR_ID Match Found.." << LOG_ENDL;

            EIPodPorts portNum = eIPOD_PORT_NONE;
            QString sysPath;
            sysPath.append(udev_device_get_syspath(dev));
            portNum = getPortNum(sysPath);

            if((portNum == m_currentIPodPortNum)||( eIPOD_PORT_NONE == m_currentIPodPortNum ))
            {
                //LOG_TRACE << "   PortMatch, m_currentIPodPortNum = "<<m_currentIPodPortNum<<LOG_ENDL;

                QString curNodePath;
                curNodePath.append(udev_device_get_devnode(dev));
                LOG_HIGH << "CDeviceDetector::isAppleDeviceConnected(), nodepath = " << m_NodePath << " current Nodepath = " << curNodePath << LOG_ENDL;

                if(curNodePath.compare(m_NodePath) == 0)
                {
                    connected = true;
                    udev_device_unref(dev);
                    break;
                }
                else
                {
                    //LOG_TRACE << "   current Nodepath is differ from previous nodepath." << LOG_ENDL;
                }
            }
        }
        udev_device_unref(dev);
    }
    /* Free the enumerator object */
    udev_enumerate_unref(enumerate);

    udev_unref(udev);
    //LOG_TRACE << "CDeviceDetector::isAppleDeviceConnected() OUT" << LOG_ENDL;
    return connected;
}

void CDeviceDetector::onDeviceDisconnect()
{
    //LOG_TRACE << "CDeviceDetector::onDeviceDisconnect() IN" << LOG_ENDL;

    m_DisconnectCompletion = false;    // added by sungha.choi 2013.04.02
    m_DetectionAvailable   = false;
    m_SavedDetection       = false;

    m_HidDevDetector->m_DetectionAvailable   = false;
    m_AudioDevDetector->m_DetectionAvailable = false;

    m_HidDevDetector->m_SavedDetection   = false;
    m_AudioDevDetector->m_SavedDetection = false;

    m_iPodAudioDevPath = "";
    m_isAudioPathSet   = false;
    m_detectorState    = eiPodDeviceDisconnected;

	m_VendorId  = 0;
	m_ProductId = 0;

    //LOG_TRACE << "CDeviceDetector::onDeviceDisconnect() OUT" << LOG_ENDL;
}

void CDeviceDetector::requestDestroyTask()
{
    emit destroyTaskHandler();  // modified by sungha.choi 2013.04.12
}

void CDeviceDetector::iPodDeviceDisconnectionComplete()
{
    //LOG_TRACE << "CDeviceDetector::iPodDeviceDisconnectionComplete() IN" << LOG_ENDL;
    m_DisconnectCompletion = true;

    m_DetectionAvailable = true;
    m_HidDevDetector->m_DetectionAvailable = true;
    m_AudioDevDetector->m_DetectionAvailable = true;

    if(m_SavedDetection == true)
    {
        iPodDeviceDetectedSlot();
    }

    if(m_HidDevDetector->m_SavedDetection == true)
    {
        m_HidDevDetector->HidDeviceDetected();
    }

    if(m_AudioDevDetector->m_SavedDetection == true)
    {
        m_AudioDevDetector->deviceDetected();
    }
    //LOG_TRACE << "CDeviceDetector::iPodDeviceDisconnectionComplete() OUT" << LOG_ENDL;
}

QString CDeviceDetector::getiPodAudioDevPath()
{
    return m_iPodAudioDevPath;
}

EIPodPorts CDeviceDetector::getPortNum(QString sysPath)
{
    EIPodPorts portNum = eIPOD_PORT_NONE;

    // Read the 8th section in the absolute path of the iPod connection
    // which specifies, whether the iPod is connected to USB1 or USB2
    QString portString = sysPath.section( '/' ,8 ,8 );
    //LOG_TRACE <<"CDeviceDetector::getPortNum(), sysPath = "<< sysPath <<LOG_ENDL;
    //LOG_TRACE <<"CDeviceDetector::getPortNum(), portString = "<<portString<<LOG_ENDL;

    // Checking for the USB 1 or USB 2
    portNum = ( portString.compare( iPodUsbPort1 ) ) ?
        ( ( portString.compare( iPodUsbPort2 ) ) ? eIPOD_PORT_NONE : eIPOD_PORT_2 ) :
        eIPOD_PORT_1;
    //LOG_TRACE <<"CDeviceDetector::getPortNum(), port num = "<<portNum<<LOG_ENDL;
    return portNum;
}

/* Added for CR 14001 S*/
void CDeviceDetector::resetIPod()
{
    //LOG_TRACE <<"CDeviceDetector::resetIPod() IN" <<LOG_ENDL;
    //LOG_TRACE <<"   m_NodePath = "<< m_NodePath <<LOG_ENDL;
    QByteArray ba = m_NodePath.toLatin1();
    const char *c_str2 = ba.data();
    //LOG_TRACE <<"   c_str2     = " << c_str2 <<LOG_ENDL;

    if(c_str2)
    {
        int fd = open(c_str2, O_WRONLY);
        //LOG_TRACE <<"   fd         = "<< fd <<LOG_ENDL;

        if(fd >= 0)
        {
            int rc = ioctl(fd, USBDEVFS_RESET, 0);
            if(rc >= 0)
            {
                //LOG_TRACE << "   Reset successful emit iPodDeviceDisconnected()"  << LOG_ENDL;
                m_UsbConfSetter->setDeviceResetValue(true);
                setDeviceResetValue(true);
                emit iPodDeviceDisconnected();
		        QTimer::singleShot(800, this, SLOT(startEnumerationAndSetDevices()));                
            }
            else
            {
               //LOG_TRACE << "   Reset unsuccessful" << LOG_ENDL;
            }
        }
        else
        {
            //LOG_TRACE <<"   open() failed, m_NodePath =  "<< m_NodePath  << LOG_ENDL;
        }

        if( fd >= 0 )
        {
			close( fd );
			fd = -1;
		}
    }
    //LOG_TRACE << "CDeviceDetector::resetIPod() OUT" << LOG_ENDL;
}

void CDeviceDetector::setDeviceResetValue(bool isDeviceReset)
{
    //LOG_TRACE << "CDeviceDetector::setDeviceResetValue() isDeviceReset = " << isDeviceReset << LOG_ENDL;
    m_IsDeviceReset = isDeviceReset;
}

bool CDeviceDetector::getDeviceResetValue()
{
    ////LOG_TRACE << "CDeviceDetector::getDeviceResetValue() return m_IsDeviceReset = "<< m_IsDeviceReset <<LOG_ENDL;
    return m_IsDeviceReset;
}

EDetectorState CDeviceDetector::GetDetectorState()
{
    return m_detectorState;
}

/* Added for CR 14001 E*/

//--------------------------------------------------------------------------------
// find all devices after startup
// copied by jonghwan.cho@lge.com	2013.03.18.
// from UISH DBusManager module
//--------------------------------------------------------------------------------
bool
CDeviceDetector::FindDevice()
{
    //LOG_TRACE << "CDeviceDetector::FindDevices() IN" << LOG_ENDL;

    QString     qnode;
	EIPodPorts	portNum;
	bool		rtnValue = false;

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

	/* Create the udev object */
	udev = udev_new();

    if( udev == NULL )
    {
        LOG_HIGH << "CDeviceDetector::FindDevices() Can't create udev ref. "  << LOG_ENDL;
		return rtnValue;
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
            //LOG_TRACE << "   Device Node Path : "<<LOG_ENDL;

			manufacturer = udev_device_get_sysattr_value(dev, "manufacturer");
			vendorID = udev_device_get_sysattr_value(dev, "idVendor");
            //LOG_TRACE << QString("   idVendor  : %1").arg( vendorID )<<LOG_ENDL;

			QString serialNum = udev_device_get_sysattr_value(dev, "serial");
            //LOG_TRACE << QString("   serialNum : %1").arg( serialNum)<<LOG_ENDL;

            QString card_num(udev_device_get_sysattr_value(soundDev, "number"));
            //LOG_TRACE << QString("   cardNum   : %1").arg( card_num )<<LOG_ENDL;
			if( card_num.isEmpty() == true )		continue;

			model = udev_device_get_property_value(dev, "ID_MODEL");
			syspath = udev_device_get_syspath(dev);
            //LOG_TRACE << QString("   syspath   : %1").arg(syspath)<<LOG_ENDL ;
            //LOG_TRACE << QString("   driver    : %1").arg( udev_device_get_driver(dev) )<<LOG_ENDL;

			if( manufacturer && model ) {
				//VendorID is "05ac" for all Apple devices
				if ( strncmp(vendorID,"05ac",4) == 0 && ( strncmp(model,"iPod",4) == 0 || strncmp(model,"iPad",4) == 0 ) ) {
                    //LOG_TRACE << QString("   %1 %2 detect").arg(model, manufacturer)<<LOG_ENDL;
					portNum = getPortNum( syspath );

					if( portNum == m_currentIPodPortNum ) {
						m_DeviceConnected = true;

                		QString gstPath("plughw:");
                		QString device_num(",0");
                		gstPath.append(card_num);
                		gstPath.append(device_num);

						m_iPodAudioDevPath = gstPath; 
						m_isAudioPathSet = true;
                        //LOG_TRACE<<QString("   GSTAudioRouter Path : %1").arg( gstPath )<<LOG_ENDL;
                        //LOG_TRACE<<QString("   GSTAudioRouter Path : %1").arg( m_iPodAudioDevPath )<<LOG_ENDL;
					
						rtnValue = true;
					}
				}
				//VendorID is "05ac" for all Apple devices
				else if ( strncmp (vendorID,"05ac",4) == 0 && strncmp (model,"iPhone",6) == 0 ) {
                    //LOG_TRACE<<QString("   %1 %2 detect").arg(model, manufacturer)<<LOG_ENDL;

					portNum = getPortNum( syspath );

					if( portNum == m_currentIPodPortNum ) {
						m_DeviceConnected = true;

                		QString gstPath("plughw:");
                		QString device_num(",0");
                		gstPath.append(card_num);
                		gstPath.append(device_num);
						
						m_iPodAudioDevPath = gstPath; 
						m_isAudioPathSet = true;
                        //LOG_TRACE<<QString("   GSTAudioRouter Path : %1").arg( gstPath )<<LOG_ENDL;
                        //LOG_TRACE<<QString("   GSTAudioRouter Path : %1").arg( m_iPodAudioDevPath )<<LOG_ENDL;

						rtnValue = true;
					}
				}
				else {
                    //LOG_TRACE<<QString("    manufacturer: %1").arg( manufacturer )<<LOG_ENDL;
                    //LOG_TRACE<<QString("    ID_MODEL    : %1").arg( model )<<LOG_ENDL;
				}
			}
		}
	}

#ifdef CONNECT_BOOTUP
    LOG_HIGH << "CDeviceDetector::FindDevices(), m_DeviceConnected = " << m_DeviceConnected << ", m_iPodAudioDevPath = " << m_iPodAudioDevPath << LOG_ENDL;
    connect(this, SIGNAL(IPodConnectStateForBootupSignal( bool )), parent(), SLOT(IPodConnectStateForBootupSlot( bool )));
    //LOG_HIGH << "CDeviceDetector::FindDevices(), emit IPodConnectStateForBootupSignal()"  << LOG_ENDL;
    emit IPodConnectStateForBootupSignal( m_DeviceConnected );
    disconnect(this, SIGNAL(IPodConnectStateForBootupSignal( bool )), parent(), SLOT(IPodConnectStateForBootupSlot( bool )));
#endif

	/* Free the enumerator object */
	udev_enumerate_unref(enumerate);
	/* Free the udev object */
	udev_unref(udev);

    //LOG_TRACE << "CDeviceDetector::FindDevices() OUT" << LOG_ENDL;
	return rtnValue;
}
