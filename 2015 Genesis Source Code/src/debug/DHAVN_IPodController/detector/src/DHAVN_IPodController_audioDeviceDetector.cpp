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
 *                 This is the main detector class HID
 *                 device connected or removed from the USB       *
 *                 will be notified here and controller will take *
 *                 action according to this.                      *
 *                 emits the signal on connection and removal of  *
 *                 the device                                     *                                           *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */

#include <DHAVN_IPodController_audioDeviceDetector.h>
#include <DHAVN_IPodController_CommonVars.h>


CAudioDeviceDetector::CAudioDeviceDetector(QObject* parent,  EIPodPorts portNum) : QObject(parent)
{
    switch(portNum)
    {
        case eIPOD_PORT_1:
            LOG_INIT_V2(giPodControllerLogName_1);
            break;
        case eIPOD_PORT_2:
            LOG_INIT_V2(giPodControllerLogName_2);
            break;
        default:
            LOG_INIT_V2(giPodControllerLogName);
    }
    //LOG_TRACE << "CAudioDeviceDetector::CAudioDeviceDetector() IN, portNum = " << portNum << LOG_ENDL;

    setObjectName(gAudioSourceDetector);

    m_Notifier    = NULL;
    m_Udev        = NULL;
    m_Mon         = NULL;
    m_currentPortNum = portNum;

    m_DetectionAvailable = true;    // added by sungha.choi 2013.04.02
    m_SavedDetection     = false;

    //LOG_TRACE << "   m_currentPortNum = "<<m_currentPortNum<<LOG_ENDL;
    registerForTheNotification();
    //LOG_TRACE << "CAudioDeviceDetector::CAudioDeviceDetector() OUT" << LOG_ENDL;
}

CAudioDeviceDetector::~CAudioDeviceDetector()
{
    if(m_Notifier)
    {
        delete m_Notifier;
        m_Notifier = NULL;
    }

    if(m_Mon)
        udev_monitor_unref (m_Mon);
    if(m_Udev)
        udev_unref (m_Udev);

}

/******************************************************************************
// Function Name                                            Parameters
// CHidDeviceDetector::registerForTheNotification()                   (int) (OUT), (void) (IN)
//
// Explanation
// Register the controller for the notification on any read in the usb descriptor on device
// connect
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CAudioDeviceDetector::registerForTheNotification()
{
    //LOG_TRACE << "CAudioDeviceDetector::registerForTheNotification() IN" << LOG_ENDL;

    int ret = 0;
    /* Create the udev object */
    m_Udev = udev_new();
    if (!m_Udev)
    {
        //LOG_TRACE << "   Can't create udev" << LOG_ENDL;
        return 0;
    }

    /* Set up a monitor to monitor hidraw devices */
    m_Mon = udev_monitor_new_from_netlink(m_Udev, "udev");
    if(!m_Mon)
    {
        //LOG_TRACE << "   Failed to get monitor" << LOG_ENDL;
        return -1;
    }

    ret = udev_monitor_filter_add_match_subsystem_devtype(m_Mon, "sound", NULL);
    if(ret != 0)
    {
        //LOG_TRACE << "   Failed to add match" << LOG_ENDL;
    }
    ret = udev_monitor_enable_receiving(m_Mon);
    if(ret != 0)
    {
        //LOG_TRACE << "   Failed to enable monitor" << LOG_ENDL;
        return ret;
    }
    else
    {
        int fd = 0;
        fd = udev_monitor_get_fd(m_Mon);
        m_Notifier = new QSocketNotifier(fd, QSocketNotifier::Read ,this);
        if(m_Notifier == NULL)
        {
            LOG_HIGH << "CAudioDeviceDetector::registerForTheNotification() Failed to alloc m_Notifier" << LOG_ENDL;
            return -1;
        }

        LOG_HIGH << "CAudioDeviceDetector, Notifier is activated, fd = " << fd << LOG_ENDL;
        connect( m_Notifier, SIGNAL(activated(int)), this, SLOT(deviceDetected()));
        //LOG_TRACE << "   success to enable monitoring the HID device" << LOG_ENDL;
    }
    //LOG_TRACE << "CAudioDeviceDetector::registerForTheNotification() OUT" << LOG_ENDL;
    return 0;

}

/******************************************************************************
// Function Name                                            Parameters
// CAudioDeviceDetector::deviceDetected()                   (void) (OUT), (void) (IN)
//
// Explanation
// Slot, which will be called when the notification of the device has come, it
// will emit signal on the basis of the action
// will set the device of the config setter
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CAudioDeviceDetector::deviceDetected()
{
    //LOG_TRACE << "CAudioDeviceDetector::deviceDetected() IN" << LOG_ENDL;

    if(m_DetectionAvailable == false)    // added by sungha.choi 2013.04.02
    {
        m_SavedDetection = true;
        return;
    }

    udev_device *Dev = udev_monitor_receive_device(m_Mon);
    if(Dev == NULL)
    {
        //LOG_TRACE << "   Failed to get the Monitor Handle" << LOG_ENDL;
        return ;
    }

    struct udev_device* dev = udev_device_get_parent_with_subsystem_devtype(
                Dev,
                "usb",
                "usb_device");

    if (dev)
    {
        QString vendor_id(udev_device_get_sysattr_value(dev,"idVendor"));
        QString product_id(udev_device_get_sysattr_value(dev, "idProduct"));
        QString card_num(udev_device_get_sysattr_value(Dev, "number"));

        //LOG_TRACE << "   Card number = " << card_num << LOG_ENDL;
        bool ok;
        /* if the vendor id matches then set the devices for the both coNFIG SETTER AND THE RAW PACKET HANDLER*/
        if(vendor_id.toInt(&ok, 16) == VENDOR_ID)
        {
            //LOG_TRACE << "   VENDOR_ID Match Found" << LOG_ENDL;

            EIPodPorts portNum = eIPOD_PORT_NONE;
            QString sysPath;
            sysPath.append(udev_device_get_syspath(dev));
            portNum = getAudioPortNum(sysPath);

	        QString node( udev_device_get_devnode( Dev ));
	        QString syspath( udev_device_get_syspath( Dev ));
	        QString serialNum( udev_device_get_sysattr_value( dev, "serial" ));
			QString model( udev_device_get_property_value( dev, "ID_MODEL" ));
            QString action( udev_device_get_action(dev) );

            //LOG_TRACE << "   sysPath = "<< sysPath << LOG_ENDL;

            if((portNum == m_currentPortNum)||( eIPOD_PORT_NONE == m_currentPortNum ))
            {
                //LOG_TRACE << "   IPod Port Match, portNum =" << m_currentPortNum << LOG_ENDL;

                QString path("plughw:");
                QString device_num(",0");
                path.append(card_num);
                path.append(device_num);

                LOG_HIGH << "emit audioDetected( path = " << path << ")"  << LOG_ENDL;
                emit audioDetected(path);

                if( node.contains( "Control", Qt::CaseInsensitive ) == true && (
                            model.contains( "iPod", Qt::CaseInsensitive ) == true ||
                            model.contains( "iPad", Qt::CaseInsensitive ) == true ||
                            model.contains( "iPhone", Qt::CaseInsensitive ) == true ))
                {

//                    LOG_HIGH << "CAudioDeviceDetector::deviceDetected, emit IPodAudioDeviceConnectedSignal(), node = "
//                             << node << ", serialNum = " << serialNum << ", portNum = " << portNum << " "  << LOG_ENDL;
                    emit onIPodAudioDeviceConnectedSignal( node, serialNum, portNum );
                }
            }
        }
    }
    else
    {
        //LOG_TRACE << "   No Device from receive_device(). An error occured." << LOG_ENDL;
    }
    udev_device_unref(Dev);
    //LOG_TRACE << "CAudioDeviceDetector::deviceDetected() OUT" << LOG_ENDL;
}


EIPodPorts CAudioDeviceDetector::getAudioPortNum(QString sysPath)
{
    EIPodPorts portNum = eIPOD_PORT_NONE;

    // Read the 8th section in the absolute path of the iPod connection
    // which specifies, whether the iPod is connected to USB1 or USB2
    QString portString = sysPath.section( '/' ,8 ,8 );
    //LOG_TRACE <<"CDeviceDetector::getPortNum(), sysPath = "<< sysPath <<LOG_ENDL;
    //LOG_TRACE <<"CAudioDeviceDetector::getAudioPortNum() portString = "<<portString<<LOG_ENDL;

    // Checking for the USB 1 or USB 2
    portNum = ( portString.compare( iPodUsbPort1 ) ) ?
        ( ( portString.compare( iPodUsbPort2 ) ) ? eIPOD_PORT_NONE : eIPOD_PORT_2 ) :
        eIPOD_PORT_1;
    //LOG_TRACE <<"CAudioDeviceDetector::getAudioPortNum(), port num = "<<portNum<<LOG_ENDL;
    return portNum;
}
