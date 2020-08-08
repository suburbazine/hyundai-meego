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
 *                   This is the class which helps the con-       *
 *                   troller to go into the HID mode and set the  *
 *                   configuratuion to desired one.               *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodController_usbconfigurationsetter.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_Def.h>
#include <DHAVN_IPodController_deviceDetector.h>

CUsbConfigurationSetter::CUsbConfigurationSetter(QObject* parent) : QObject(parent)
{
    switch( ((CDeviceDetector*)parent)->m_currentIPodPortNum ){
    case eIPOD_PORT_1:
        LOG_INIT_V2(giPodControllerLogName_1);
        break;
    case eIPOD_PORT_2:
        LOG_INIT_V2(giPodControllerLogName_2);
        break;
    default:
         LOG_INIT_V2(giPodControllerLogName);
    }
    setObjectName(gConfSetter);
    m_IsDeviceResetRequested = false;
    m_DevHandler = NULL;
}

/******************************************************************************
// Function Name                                            Parameters
// CUsbConfigurationSetter::setDevice                       (int) (OUT), (int) (IN)
//
// Explanation
// Set the devices.
// initalizes the libusb, open the device handle
// sets the configuation
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CUsbConfigurationSetter::setDevice(int vendorId, int productId)
{
    //LOG_TRACE << "CUsbConfigurationSetter::setDevice Entry " << LOG_ENDL;
    int result, configuration = 0 ;
    int cnt = 50;//Try to change the USB COnfiguration for 50 times.
    libusb_device *dev;
     libusb_context*  m_LibUsbContext;


    result = libusb_init(&m_LibUsbContext);

    if (result < 0) {
        //LOG_TRACE << "Unable to initialize libusb " << LOG_ENDL;
        return result;
    }

    m_DevHandler = open_iphone(&dev, vendorId, productId);
    if (m_DevHandler == NULL)
    {
        //LOG_TRACE << "CUsbConfigurationSetter::setDevice:open_iphone failed " << LOG_ENDL;
        libusb_exit(m_LibUsbContext);
        return IPOD_ERROR_GENERIC;
    }


    libusb_get_configuration(m_DevHandler, &configuration);

    if(configuration == CONFIGURATION_APPLE)
    {
        /* Added for CR 14001 S*/
        if(m_IsDeviceResetRequested)
        {
            m_IsDeviceResetRequested = false;
            while ((configuration != 1) && (cnt > 0)) {
                        result = set_config(dev, m_DevHandler, 1);
                        libusb_get_configuration(m_DevHandler, &configuration);
                        cnt--;
            }
            cnt = 50;
        /* Added for CR 14001 E*/
        }
        else
        {
            //LOG_TRACE << "device is already in HID mode " << LOG_ENDL;
            libusb_close(m_DevHandler);
            libusb_exit(m_LibUsbContext);
            return errDeviceIsAlreadyInHIDMode;
        }
    }
    while ((configuration != CONFIGURATION_APPLE) && (cnt > 0)) {
        result = set_config(dev, m_DevHandler, CONFIGURATION_APPLE);
        libusb_get_configuration(m_DevHandler, &configuration);
        cnt--;
    }


    libusb_close(m_DevHandler);
    libusb_exit(m_LibUsbContext);

  //  sleep(1);
    //LOG_TRACE << "CUsbConfigurationSetter::setDevice EXIT " << LOG_ENDL;
    if(configuration == CONFIGURATION_APPLE){
        return IPOD_SUCCESS;
    } else{
        return IPOD_ERROR_GENERIC;
    }
}

int CUsbConfigurationSetter::set_config(libusb_device* dev, libusb_device_handle *devh, int config)
{
      //LOG_TRACE << "CUsbConfigurationSetter::set_config Entry " << LOG_ENDL;
    int result = -1;
    int i = 0 ;
    struct libusb_config_descriptor *cdesc = NULL;


    result = libusb_get_active_config_descriptor(dev, &cdesc);
    if (result != 0) {
        //LOG_INFO << "Unable to get active config"<< result << LOG_ENDL;
        return result;
    }

    for (i = 0; i < cdesc->bNumInterfaces; i++)
    {
        result = libusb_detach_kernel_driver(devh, i);
        if (result != 0 && result != LIBUSB_ERROR_NOT_FOUND) {
            //LOG_INFO << "Unable to detach kernel driver:"<< result << LOG_ENDL;
            goto error;
        }
    }

    result =  libusb_set_configuration(devh, config);
    if ( result < 0 ) {
        //LOG_INFO << "unable to set configuration:"<< result << LOG_ENDL;
        goto error;
    }
    result = 0;
    //LOG_INFO << "set config = "<< config << "with result = "<< result << LOG_ENDL;

    error:
        libusb_free_config_descriptor (cdesc);
    //LOG_TRACE << "CUsbConfigurationSetter::set_config Exit " << LOG_ENDL;
    return result;
}


libusb_device_handle* CUsbConfigurationSetter::open_iphone(libusb_device **dev, int vendor_Id, int product_Id)
{
    //LOG_TRACE << "UsbConfigurationSetter::open_iphone Entry " << LOG_ENDL;
    ssize_t count;
    libusb_device **devs = NULL;
    libusb_device_handle *devh;
    QString hexvalue_Vendor;
    QString hexvalue_Product;
    int i;


    count = libusb_get_device_list(NULL, &devs);
    if (count < 0) {
         //LOG_TRACE << "UsbConfigurationSetter::open_iphone:Unable to list devices with count = "<< count << LOG_ENDL;
        goto error;
    }
    if((count == 0) || (devs == NULL) ){
         //LOG_TRACE << "UsbConfigurationSetter::open_iphone:No list Found = "<< count << LOG_ENDL;
         //LOG_TRACE << "UsbConfigurationSetter::open_iphone Exit " << LOG_ENDL;
         return NULL;
    }

    for (i=0; i<count; i++) {
        struct libusb_device_descriptor desc;
        if (libusb_get_device_descriptor(devs[i], &desc) < 0)
            return NULL;
        hexvalue_Vendor.setNum(desc.idVendor,16);
        hexvalue_Product.setNum(desc.idProduct,16);
        //LOG_TRACE << "open_iphone: idVendor= "<< hexvalue_Vendor << " and "
        //         << "idProduct = "<< hexvalue_Product << LOG_ENDL;
        if (desc.idVendor == vendor_Id && desc.idProduct == product_Id) {
            //LOG_TRACE << "Found iPhone" << LOG_ENDL;
            if (libusb_open (devs[i], &devh) == 0) {
                *dev = devs[i];
                libusb_free_device_list(devs, 1);
                return devh;
            }
        }
    }
    error:
        libusb_free_device_list(devs,1);
    //LOG_TRACE << "UsbConfigurationSetter::open_iphone Exit " << LOG_ENDL;
    return NULL;
}


int CUsbConfigurationSetter::getConfiguration(libusb_config_descriptor /*configDesc*/)
{
    //    int result = libusb_get_active_config_descriptor(dev, &configDesc);
    return -1;
    //    return result;
}

int CUsbConfigurationSetter::setConfiguration(libusb_config_descriptor /*devConfig*/)
{
    return -1;
}

/* Added for CR 14001 S*/
void CUsbConfigurationSetter::setDeviceResetValue(bool isDeviceResetRequested)
{
    //LOG_TRACE << "UsbConfigurationSetter::setDeviceResetValue Entry isDeviceResetRequested = " << isDeviceResetRequested << LOG_ENDL;
    m_IsDeviceResetRequested = isDeviceResetRequested;
    //LOG_TRACE << "UsbConfigurationSetter::setDeviceResetValue Exit " << LOG_ENDL;
}

bool CUsbConfigurationSetter::getDeviceResetValue()
{
    //LOG_TRACE << "UsbConfigurationSetter::getDeviceResetValue Entry m_IsDeviceResetRequested = " << m_IsDeviceResetRequested << LOG_ENDL;
    return m_IsDeviceResetRequested;
}
/* Added for CR 14001 E */
