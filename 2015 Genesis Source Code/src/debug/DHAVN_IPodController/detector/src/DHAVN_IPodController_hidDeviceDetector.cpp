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

#include <DHAVN_IPodController_hidDeviceDetector.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_Def.h>

CHidDeviceDetector::CHidDeviceDetector(QObject* parent, EIPodPorts portNum) : QObject(parent)
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

    //LOG_TRACE << "CHidDeviceDetector::CHidDeviceDetector() IN" << LOG_ENDL;


    setObjectName(gHidDetector);
    m_Notifier  = NULL;
    m_Udev      = NULL;
    m_Mon       = NULL;

    m_ProductId = 0;
    m_VendorId  = 0;

    m_currentIPodPortNum = portNum;
    m_DetectionAvailable = true;    // added by sungha.choi 2013.04.02
    m_SavedDetection     = false;

    registerForTheNotification();
    //LOG_TRACE << "CHidDeviceDetector::CHidDeviceDetector() OUT" << LOG_ENDL;
}

CHidDeviceDetector::~CHidDeviceDetector()
{
    //LOG_TRACE << "CHidDeviceDetector::~CHidDeviceDetector() IN" << LOG_ENDL;
    if(m_Notifier)
    {
        delete m_Notifier;
        m_Notifier = NULL;
    }

    if(m_Mon)    udev_monitor_unref (m_Mon);
    if(m_Udev)   udev_unref (m_Udev);

    //LOG_TRACE << "CHidDeviceDetector::~CHidDeviceDetector() OUT" << LOG_ENDL;
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
int CHidDeviceDetector::registerForTheNotification()
{
    //LOG_TRACE << "CHidDeviceDetector::registerForTherNotification() IN" << LOG_ENDL;

    int ret = 0;
    /* Create the udev object */
    m_Udev = udev_new();
    if (!m_Udev)
    {
        LOG_HIGH << "CHidDeviceDetector::registerForTherNotification(), Can't create udev" << LOG_ENDL;
        return IPOD_ERROR_GENERIC;
    }

    /* Set up a monitor to monitor hidraw devices */
    m_Mon = udev_monitor_new_from_netlink(m_Udev, "udev");
    if(!m_Mon)
    {
        LOG_HIGH << "CHidDeviceDetector::registerForTherNotification(), Failed to get monitor" << LOG_ENDL;
        return IPOD_ERROR_GENERIC;
    }

    ret = udev_monitor_filter_add_match_subsystem_devtype(m_Mon, "hidraw", NULL);
    if(ret != 0)
        {
        LOG_HIGH << "CHidDeviceDetector::registerForTherNotification(), Failed to add match" << LOG_ENDL;
        return IPOD_ERROR_GENERIC;
    }

    ret = udev_monitor_enable_receiving(m_Mon);
    if(ret != 0)
        {
        LOG_HIGH << "CHidDeviceDetector::registerForTherNotification(), Failed to enable monitor" << LOG_ENDL;
        return IPOD_ERROR_GENERIC;
    }
    else
    {
        int fd = 0;
        fd = udev_monitor_get_fd(m_Mon);
        if(fd < 0){
            LOG_HIGH << "CHidDeviceDetector::registerForTheNotification() failed to get the fd for monitor" << LOG_ENDL;
            return IPOD_ERROR_GENERIC;
        }
        m_Notifier = new QSocketNotifier(fd, QSocketNotifier::Read ,this);
        if(m_Notifier == NULL){
            LOG_HIGH << "CHidDeviceDetector::registerForTheNotification() Failed to alloc m_Notifier" << LOG_ENDL;
            return IPOD_ERROR_GENERIC;
        }
        LOG_HIGH << "CHidDeviceDetector, notifier is activated, fd = " << fd << LOG_ENDL;
        connect(m_Notifier, SIGNAL(activated(int)), this, SLOT(HidDeviceDetected()));
        //LOG_TRACE << "   success to enable monitoring the HID device" << LOG_ENDL;
    }

    //LOG_TRACE << "CHidDeviceDetector::registerForTherNotification() OUT" << LOG_ENDL;
    return IPOD_SUCCESS;

}

/******************************************************************************
// Function Name                                            Parameters
// CHidDeviceDetector::deviceDetected()                   (void) (OUT), (void) (IN)
//
// Explanation
// Slot, which will be called when the notification of the device has come, it
// will emit signal on the basis of the action
// will set the device of the config setter
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CHidDeviceDetector::HidDeviceDetected()
{
    //LOG_TRACE << "CHidDeviceDetector::HidDeviceDetected() IN" << LOG_ENDL;

    if(m_DetectionAvailable == false) 
    {    // added by sungha.choi 2013.04.02
        m_SavedDetection = true;
        return;
    }

    struct udev_device *Dev = NULL;
    Dev = udev_monitor_receive_device(m_Mon);
    if(Dev == NULL)
    {
        LOG_HIGH << "CHidDeviceDetector::HidDeviceDetected() Failed to get the monitor Device" << LOG_ENDL;
        return;
    }

    struct udev_device* dev = udev_device_get_parent_with_subsystem_devtype(
                Dev,
                "usb",
                "usb_device");

    if (dev)
    {
        QString vendor_id(udev_device_get_sysattr_value(dev,"idVendor"));
        //QString product_id(udev_device_get_sysattr_value(dev, "idProduct"));
        bool ok;
        /* if the vendor id matches then set the devices for the both coNFIG SETTER AND THE RAW PACKET HANDLER*/
        if(vendor_id.toInt(&ok, 16) == VENDOR_ID)
        {
            //LOG_TRACE << "   VENDOR_ID Match Found.." << LOG_ENDL;
            EIPodPorts portNum = eIPOD_PORT_NONE;
            QString sysPath;
            sysPath.append(udev_device_get_syspath(dev));
            portNum = getHidPortNum(sysPath);

            if((portNum == m_currentIPodPortNum)||( eIPOD_PORT_NONE == m_currentIPodPortNum ))
            {
                LOG_HIGH << "CHidDeviceDetector::HidDeviceDetected() Port Match m_currentIPodPortNum = " << m_currentIPodPortNum << ", emit hidDetetcted()"  << LOG_ENDL;
                QString path(udev_device_get_devnode(Dev));
                emit hidDetetcted(path);
            }
        }
    }
    else
    {
        //LOG_TRACE << "   No Device from receive_device(). An error occured." << LOG_ENDL;
    }
    /* No Need to free dev as its not referenced. this will be automatically freed along with Dev
    if(dev){
         ("CHidDeviceDetector::deviceDetected: Freeing dev\n");
        udev_device_unref(dev);
    }
    */
    if(Dev)
    {
        //LOG_TRACE << "   No Device from receive_device(): Freeing Dev" << LOG_ENDL;
        udev_device_unref(Dev);
        Dev = NULL;
    }

    //LOG_TRACE << "CHidDeviceDetector::HidDeviceDetected() OUT" << LOG_ENDL;
    return;
}

/******************************************************************************
// Function Name                                            Parameters
// CHidDeviceDetector::startEnumerationAndSetDevicesForHIDMode()         (void) (OUT), (void) (IN)
//
// Explanation
// Single time use at the time of the controller startup, will enumerate the
// USB HID devices if already connected the desired device then set the devices to the
// raw packet handler.
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CHidDeviceDetector::startEnumerationAndSetDevicesForHIDMode()
{
    //LOG_TRACE << "CHidDeviceDetector::startEnumerationAndSetDevicesForHIDMode() IN"<<LOG_ENDL;
    struct udev *udev = NULL;
    struct udev_enumerate *enumerate = NULL;
    struct udev_list_entry *devices = NULL, *dev_list_entry = NULL;
    struct udev_device *dev = NULL;
    struct udev_device *dev_tmp = NULL;

    /* Create the udev object */
    udev = udev_new();
    if (!udev)
    {
        LOG_HIGH << "CHidDeviceDetector, Can't create udev"<<LOG_ENDL;
        return IPOD_ERROR_GENERIC;
    }

    /* Create a list of the devices in the 'hidraw' subsystem. */
    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "hidraw");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);

    udev_list_entry_foreach(dev_list_entry, devices)
    {
        const char *path;
        path = udev_list_entry_get_name(dev_list_entry);
         dev_tmp = udev_device_new_from_syspath(udev, path);

        QString devPath;
        devPath.append(udev_device_get_devnode(dev_tmp));
        devPath.append("\0");
        //LOG_TRACE<<"   Device Node Path = "<< devPath <<LOG_ENDL;

        dev = udev_device_get_parent_with_subsystem_devtype(
                dev_tmp,
                "usb",
                "usb_device");
      //  udev_device_unref(dev_tmp);
        if (!dev)
            {
            LOG_HIGH<<"CHidDeviceDetector, Unable to find parent usb device."<<LOG_ENDL;
            return IPOD_ERROR_GENERIC;
        }

        QString VendorId;
        QString ProductId;
        VendorId.append(udev_device_get_sysattr_value(dev,"idVendor"));
        ProductId.append(udev_device_get_sysattr_value(dev, "idProduct"));
        bool ok;
        m_VendorId = VendorId.toInt(&ok, 16);
        m_ProductId = ProductId.toInt(&ok, 16);

        /* if the vendor id matches then set the devices for the both coNFIG SETTER AND THE RAW PACKET HANDLER*/
        if( VENDOR_ID == m_VendorId)
        {
            EIPodPorts portNum = eIPOD_PORT_NONE;
            QString sysPath;
            sysPath.append(udev_device_get_syspath(dev));
            portNum = getHidPortNum(sysPath);

            if((portNum == m_currentIPodPortNum)||( eIPOD_PORT_NONE == m_currentIPodPortNum ))
            {
                LOG_HIGH << "CHidDeviceDetector,PortNum =" << m_currentIPodPortNum << ", emit hidDetetcted()"  << LOG_ENDL;
                emit hidDetetcted(devPath);
                udev_device_unref(dev);
                break;
            }
        }
        udev_device_unref(dev);
    }
    /* Free the enumerator object */
    udev_enumerate_unref(enumerate);
    udev_unref(udev);
    //LOG_TRACE<<"CDeviceDetector::startEnumerationAndSetDevicesForHIDMode() OUT"<<LOG_ENDL;
    return IPOD_SUCCESS;
}

EIPodPorts CHidDeviceDetector::getHidPortNum(QString sysPath)
{
    EIPodPorts portNum = eIPOD_PORT_NONE;

    // Read the 8th section in the absolute path of the iPod connection
    // which specifies, whether the iPod is connected to USB1 or USB2
    QString portString = sysPath.section( '/' ,8 ,8 );
    //LOG_TRACE <<"CDeviceDetector::getPortNum(), sysPath = "<< sysPath <<LOG_ENDL;
    //LOG_TRACE <<"CHidDeviceDetector::getHidPortNum() portString = "<<portString<<LOG_ENDL;

    // Checking for the USB 1 or USB 2
    portNum = ( portString.compare( iPodUsbPort1 ) ) ?
        ( ( portString.compare( iPodUsbPort2 ) ) ? eIPOD_PORT_NONE : eIPOD_PORT_2 ) :
        eIPOD_PORT_1;
    //LOG_TRACE <<"CHidDeviceDetector::getHidPortNum() port num = "<<portNum<<LOG_ENDL;
    return portNum;
}
