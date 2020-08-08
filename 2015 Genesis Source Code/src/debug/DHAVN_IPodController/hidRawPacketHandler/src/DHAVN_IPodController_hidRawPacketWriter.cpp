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
 *                  This is the main class which interact with    *
 *                  the device directly, opens the device, wait   *
 *                  for the notification from the IPOD for the    *
 *                  read, writes to the device etc.               *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodController_hidRawPacketWriter.h>
#include <linux/hidraw.h>
#include <linux/version.h>
#include <libudev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <QDebug>
#include <errno.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_deviceDetector.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
#include <DHAVN_IPodController_IAPPacketWriter.h>
#include <DHAVN_IPodController_CommonVars.h>
#ifdef FILEBASEDAPPHANDLER
#include <QDir>
#include <QFile>
#include <DHAVN_IPodController_PrivateData.h>
#endif

CHIDRawPacketWriter::CHIDRawPacketWriter(EIPodPorts nCurrrentPortNum) : QObject(NULL),m_IPodHidDevicePath(""),m_IPodPandoraAppSessionID(""), m_IPodAhaAppSessionID("")
{
    EIPodPorts portNum = nCurrrentPortNum;
    m_IPodPandoraAppSessionID = gIPodPandoraAppSessionID + DOT + QString::number(portNum);
    m_IPodAhaAppSessionID = gIPodAhaAppSessionID + DOT + QString::number(portNum);

    switch(portNum)
    {
        case eIPOD_PORT_1:
            LOG_INIT_V2(giPodControllerLogName_1);
            m_IPodHidDevicePath = gIPodHidDevicePathPort1;
            break;
        case eIPOD_PORT_2:
            LOG_INIT_V2(giPodControllerLogName_2);
            m_IPodHidDevicePath = gIPodHidDevicePathPort2;
            break;
        default:
            m_IPodHidDevicePath = gIPodHidDevicePath;
            LOG_INIT_V2(giPodControllerLogName);
            m_IPodPandoraAppSessionID = gIPodPandoraAppSessionID ;
            m_IPodAhaAppSessionID = gIPodAhaAppSessionID ;
        break;
    }


    //LOG_TRACE << "CHIDRawPacketHandle::CHIDRawPacketHandle() m_IPodPandoraAppSessionID" << m_IPodPandoraAppSessionID <<  LOG_ENDL;
    //LOG_TRACE << "CHIDRawPacketHandle::CHIDRawPacketHandle() m_IPodAhaAppSessionID" << m_IPodAhaAppSessionID <<  LOG_ENDL;
    //LOG_TRACE << "CHIDRawPacketHandle::CHIDRawPacketHandle() port number " << portNum << LOG_ENDL;
    //LOG_TRACE << "CHIDRawPacketHandle::CHIDRawPacketHandle() Entry" << LOG_ENDL;
    m_DevPath.resize(0);
    m_Handle = NULL;
    m_SocketNumber = -1;
    //    setObjectName(gRawPcktHandler);
    m_maxReportSize = 0;
    m_isDisconnected = false;    // added by sungha.choi 2013.04.15

#ifdef FILEBASEDAPPHANDLER
    if(!QDir(gIPodTempPath).exists())
    {
        QDir tempdir;
        tempdir.mkpath(gIPodTempPath);
    }
    else
    {
	if(!m_IPodHidDevicePath.isEmpty())
		QFile::remove(m_IPodHidDevicePath);
        QFile::remove(m_IPodPandoraAppSessionID);
        QFile::remove(m_IPodAhaAppSessionID);
    }
#endif
    //LOG_TRACE << "CHIDRawPacketWriter::CHIDRawPacketWriter() Exit" << LOG_ENDL;
}

CHIDRawPacketWriter::~CHIDRawPacketWriter()
{

    if(m_Handle){
        //LOG_HIGH << "CHIDRawPacketWriter m_Handle->device_handle = " << m_Handle->device_handle << LOG_ENDL;
        //if(m_Handle->device_handle > 0)
        //    close(m_Handle->device_handle);
        delete m_Handle;
		// Added by jonghwan.cho@lge.com	2013.03.27.
		m_Handle = NULL;
    }
    // { added by sungha.choi 2013.04.02
#ifdef FILEBASEDAPPHANDLER
    //LOG_HIGH <<"CHIDRawPacketWriter::deviceDisconnected() m_IPodHidDevicePath=" << m_IPodHidDevicePath <<  LOG_ENDL;
    if(!m_IPodHidDevicePath.isEmpty())
        QFile::remove(m_IPodHidDevicePath);

    QFile::remove(m_IPodPandoraAppSessionID);
    QFile::remove(m_IPodAhaAppSessionID);
#endif

    LOG_HIGH << "~CHIDRawPacketWriter(), close m_SocketNumber = " << m_SocketNumber << LOG_ENDL;
    if(m_SocketNumber >= 0){
        close(m_SocketNumber);
        m_SocketNumber = -1;
    }

    // } added by sungha.choi 2013.04.02
}

/****************************************************************************************/
/*                                  Public Methods                                     */
/****************************************************************************************/



/******************************************************************************
// Function Name                                            Parameters
// CHIDRawPacketWriter::write                    int (OUT), (QByteArray) (IN)
//
// Explanation
// Writes the command to the and returns the bytes written
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CHIDRawPacketWriter::hidWrite(QByteArray hidData)
{
  //  //LOG_TRACE << "CHIDRawPacketWriter::write Entry" << LOG_ENDL;
    if(m_isDisconnected)    // added by sungha.choi 2013.04.15
    {
        LOG_HIGH << "CHIDRawPacketWriter::write: m_isDisconnected is true" << LOG_ENDL;
        return 0;
    }

    if(m_Handle == NULL || m_SocketNumber < 0)
    {
        LOG_HIGH << "CHIDRawPacketWriter::write: Handle is NULL!!!!" << LOG_ENDL;
        return 0;
    }
    //printOutArray(hidData);
    int bytes_written = ::write(m_Handle->device_handle, hidData, hidData.length());

    //qDebug()<<"iPod Sent >>:"<<hidData.toHex();
  //  //LOG_INFO << "CHIDRawPacketWriter::write OUT with bytes written ="<<bytes_written<<LOG_ENDL;
    return bytes_written;
}

int CHIDRawPacketWriter::setHidDevice(int vendorId, int productId, QString hidDevPath)
{
    //LOG_TRACE <<"CHIDRawPacketWriter::setHidDevice() Entry"<<LOG_ENDL;
    setPath(hidDevPath);
/* Clean the previous handler if any */
    if(m_Handle){
        LOG_HIGH << "CHIDRawPacketWriter m_Handle->device_handle = " << m_Handle->device_handle << LOG_ENDL;
        if(m_Handle->device_handle > 0)
            close(m_Handle->device_handle);
        delete m_Handle;
        m_Handle = NULL;
    }
    m_Handle = hid_open(vendorId, productId, NULL);
    if(m_Handle != NULL){
        if(m_Handle->device_handle > 0){
            setMaxReportSize();
            //LOG_TRACE<<"in CHIDRawPacketWriter::setHidDevice() out IPOD_SUCCESS\n" << LOG_ENDL;
            return IPOD_SUCCESS;

        }
    }
    //LOG_TRACE << "CHIDRawPacketWriter::setHidDevice() out IPOD_ERROR_GENERIC"<< LOG_ENDL;
    return IPOD_ERROR_GENERIC;
}

int CHIDRawPacketWriter::DispatchiAPToHid(QByteArray iAPPacketBytes)
{
   // //LOG_TRACE << "CHIDRawPacketWriter::DispatchiAPToHid:IN"<< LOG_ENDL;
    int len =0;
    int packCnt = 0;
    int i = 1;
    QByteArray hidPacket;
    int packetSize= 0;
    int ret = -1;

    if(m_maxReportSize <= 0)    // added by sungha.choi 2013.04.15
    {
        return 0;
    }

    len = iAPPacketBytes.length();
    if(len <= m_maxReportSize){
        hidPacket.clear();
        hidPacket[0] = lookupReport(len,packetSize);
        hidPacket[1] = 0x00; // Only one packet.
        hidPacket.append(iAPPacketBytes.mid(0,len));
        ret = hidWrite(hidPacket);
       // //LOG_TRACE << "CHIDRawPacketWriter::DispatchiAPToHid:OUT"<< LOG_ENDL;
        return ret;
    }
    packCnt = len/m_maxReportSize;
    if(len%m_maxReportSize)
        packCnt++;
   // //LOG_TRACE << "CHIDRawPacketWriter::DispatchiAPToHid: Num HID packets="<< packCnt<< LOG_ENDL;
    hidPacket.clear();
    hidPacket[0] = lookupReport(m_maxReportSize,packetSize);
    hidPacket[1] = 0x02; // First chunk of large packet.
    hidPacket.append(iAPPacketBytes.mid(0,m_maxReportSize));
    ret = hidWrite(hidPacket);
    if(m_isDisconnected)    // added by sungha.choi 2013.04.15
        return 0;

    for(i=1; i<packCnt-1; i++){
        hidPacket.clear();
        hidPacket[0] = lookupReport(m_maxReportSize,packetSize);
        hidPacket[1] = 0x03; // Middle chunks of large packet.
        hidPacket.append(iAPPacketBytes.mid((i*m_maxReportSize),m_maxReportSize));
        ret = hidWrite(hidPacket);
        if(m_isDisconnected)    // added by sungha.choi 2013.04.15
            return 0;
    }
    hidPacket.clear();
    hidPacket[0] = lookupReport(len%m_maxReportSize,packetSize);
    hidPacket[1] = 0x01; // last chunk of large packet.
    hidPacket.append(iAPPacketBytes.mid(m_maxReportSize*(packCnt-1)));
    ret = hidWrite(hidPacket);

   // //LOG_TRACE << "CHIDRawPacketWriter::DispatchiAPToHid:IN"<< LOG_ENDL;
    return len;

}


/****************************************************************************************/
/*                                  Private Methods                                     */
/****************************************************************************************/


hid_device * CHIDRawPacketWriter::hid_open(unsigned short /*vendor_id*/, unsigned short /*product_id*/, char */*serial_number*/)
{
    //LOG_TRACE << "CHIDRawPacketWriter::hid_open() Entry" << LOG_ENDL;
    hid_device *handle = NULL;
    if(m_DevPath.length())
    {
        //LOG_INFO<< "path found from the detector: " << m_DevPath.toAscii().data() << LOG_ENDL;
        handle = hid_open_path(m_DevPath.toAscii().data());
    }
    //LOG_TRACE << "CHIDRawPacketWriter::hid_open() Exit" << LOG_ENDL;
    return handle;
}



hid_device * CHIDRawPacketWriter::hid_open_path(const char *path)
{
    //LOG_TRACE << "CHIDRawPacketWriter::hid_open_path() Entry" << LOG_ENDL;
    hid_device *dev = NULL;

    if(path == NULL){
        LOG_HIGH<<"CHIDRawPacketWriter::hid_open_path: hid file path is NULL"<< LOG_ENDL;
        goto hid_open_path_fail;
    }
    dev = new_hid_device();
    if(dev == NULL){
        LOG_HIGH<<"CHIDRawPacketWriter::hid_open_path: memaloc failed!!"<< LOG_ENDL;
        goto hid_open_path_fail;
    }
    // OPEN HERE //
    dev->device_handle = open(path, O_RDWR);

    // If we have a good handle, return it.
    if (dev->device_handle >= 0) {
        m_SocketNumber = dev->device_handle;

        /* Get the report descriptor */
        int res, desc_size = 0;
        struct hidraw_report_descriptor rpt_desc;

        memset(&rpt_desc, 0x0, sizeof(rpt_desc));

        /* Get Report Descriptor Size */
        res = ioctl(dev->device_handle, HIDIOCGRDESCSIZE, &desc_size);
        if (res < 0){
            //LOG_TRACE << "CHIDRawPacketWriter::hid_open_path: HIDIOCGRDESCSIZE Failed"<< LOG_ENDL;
            goto hid_open_path_fail;
        }
        /* Get Report Descriptor */
        rpt_desc.size = desc_size;
        res = ioctl(dev->device_handle, HIDIOCGRDESC, &rpt_desc);
        if (res < 0) {
            //LOG_TRACE << "CHIDRawPacketWriter::hid_open_path: HIDIOCGRDESC Failed"<< LOG_ENDL;
            goto hid_open_path_fail;
        } else {
            /* Determine if this device uses numbered reports. */
            dev->uses_numbered_reports =
                    uses_numbered_reports(rpt_desc.value,
                                          rpt_desc.size);
        }
        LOG_HIGH << "CHIDRawPacketWriter, m_SocketNumber = " << m_SocketNumber << ", path = "<< path << LOG_ENDL;
        return dev;
    }
    else {
        //LOG_TRACE << "CHIDRawPacketWriter::hid_open_path() Unable to open any devices"<< LOG_ENDL;
    }

hid_open_path_fail:
    if(dev!= NULL)
        delete dev;
    return NULL;
}

hid_device* CHIDRawPacketWriter::new_hid_device()
{
    //LOG_TRACE << "in CHIDRawPacketWriter::new_hid_device() IN \n"<< LOG_ENDL;
    hid_device *dev = new hid_device;
    dev->device_handle = -1;
    dev->blocking = 1;
    dev->uses_numbered_reports = 0;
    //LOG_TRACE << "in CHIDRawPacketWriter::new_hid_device() out \n"<< LOG_ENDL;
    return dev;
}

/******************************************************************************
// Function Name                                            Parameters
// CHIDRawPacketWriter::uses_numbered_reports                    (int) (OUT), ((quint8*, quint32)) (IN)
//
// Explanation
// Check if the device uses the numberd report then created the
// Report ID X Size value pair for the future use
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CHIDRawPacketWriter::uses_numbered_reports(quint8 *report_descriptor, quint32 size) {
    //LOG_TRACE << "in CHIDRawPacketWriter::uses_numbered_reports() in"<< LOG_ENDL;
    int i = 0;
    int size_code = 0;
    int data_len = 0, key_size = 0;
    int found = 0;

    while (i < (int)size) {
        int key = report_descriptor[i];

        /* Check for the Report ID key */
        if (key == 0x85/*Report ID*/) {
            /* This device has a Report ID, which means it uses
                           numbered reports. */
            found =1;
        }

        if (key == 0x95/*Report ID*/) {
            /* This device has a Report ID, which means it uses
                           numbered reports. */
            found =1;
        }

        if ((key & 0xf0) == 0xf0) {
            /* This is a Long Item. The next byte contains the
                           length of the data section (value) for this key.
                           See the HID specification, version 1.11, section
                           6.2.2.3, titled "Long Items." */
            if (i+1 < (int)size)
                data_len = report_descriptor[i+1];
            else
                data_len = 0; /* malformed report */
            key_size = 3;
        }
        else {
            /* This is a Short Item. The bottom two bits of the
                           key contain the size code for the data section
                           (value) for this key.  Refer to the HID
                           specification, version 1.11, section 6.2.2.2,
                           titled "Short Items." */
            size_code = key & 0x3;
            switch (size_code) {
            case 0:
            case 1:
            case 2:
                data_len = size_code;
                break;
            case 3:
                data_len = 4;
                break;
            /* Tan, coverity, bitwise && operation with 0x3 causes value to 3 at most
            default:
                // Can't ever happen since size_code is & 0x3
                data_len = 0;
                break;
            */
            };
            key_size = 1;
        }

        /* Skip over this key and it's associated data */
        i += data_len + key_size;
    }
    //LOG_TRACE << "in CHIDRawPacketWriter::uses_numbered_reports() Exit" << LOG_ENDL;
    /* Didn't find a Report ID key. Device doesn't use numbered reports. */
    return found;
}



/******************************************************************************
// Function Name                                            Parameters
// CHIDRawPacketWriter::setPath                    (void) (OUT), (QString) (IN)
//
// Explanation
// Set the path of the device desc to m_DevPath
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CHIDRawPacketWriter::setPath(QString path)
{
    //LOG_TRACE << "CHIDRawPacketWriter::setPath() IN"<< LOG_ENDL;
    m_DevPath.resize(0);
    m_DevPath.append(path);
    iPodReadyForApps(true);
    //LOG_TRACE <<"setting path to CHIDRawPacketWriter =" <<m_DevPath<< LOG_ENDL;
    //LOG_TRACE <<"CHIDRawPacketWriter::setPath() OUT"<< LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIAPPacketWriter::lookupReport                (int, int) (OUT), (int) (IN)
//
// Explanation
// returns the size and the report ID for the current command
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CHIDRawPacketWriter::lookupReport(int size, int& size_out)
{
//    //LOG_TRACE <<"CIAPPacketWriter::lookupReport IN"<< LOG_ENDL;
    int i;
    for (i=0; hidReports[i][0]!=0; i++) {
        if (hidReports[i][1] >= size) {
            size_out = hidReports[i][1];
            return hidReports[i][0];
        }
    }
    size_out  = hidReports[i][1];
//    //LOG_TRACE <<"CIAPPacketWriter::lookupReport OUT"<< LOG_ENDL;
    return hidReports[i][0];
}

void CHIDRawPacketWriter::setMaxReportSize()
{
    int i;
    int maxSize = 0;
    for (i=0; hidReports[i][0]!=0; i++) {
        if (hidReports[i][1] >= maxSize) {
            maxSize = hidReports[i][1];
        }
    }
    m_maxReportSize = maxSize;
}

void CHIDRawPacketWriter::printOutArray(QByteArray array)
{

    //LOG_TRACE << "Sending Packet to iPod: " << LOG_ENDL;
    int count = array.count()/8;
    count++;
    /*
    for(int i=0;i<count;i++)
    {
        //LOG_INFO<<"[" << i << "] = ["<< (array.mid(i*8, 8)).toHex().data()
        //        << "]" << LOG_ENDL;
    }
    */
}

/****************************************************************************************/
/*                                  Slots                                               */
/****************************************************************************************/


/******************************************************************************
// Function Name                                            Parameters
// CHIDRawPacketWriter::deviceDisconnected                    (void) (OUT), (void) (IN)
//
// Explanation
// Slot to get the notification on device disconnected, disable the
// notification of read desc
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/

void CHIDRawPacketWriter::closeOpenedFileDescriptor()
{
    if(m_SocketNumber >= 0)
    {
        close(m_SocketNumber);
        m_SocketNumber = -1;
        if(m_Handle)
            m_Handle->device_handle = -1;
    }
}

void CHIDRawPacketWriter::onDeviceDisconnected()
{
    //LOG_TRACE <<"SCHIDRawPacketWriter::deviceDisconnected()" << LOG_ENDL;
    m_isDisconnected = true;    // added by sungha.choi 2013.04.15

    // { deleted by sungha.choi 2013.04.02 this object must not be deleted here
#if 0   // this routine will be called in destructor
    if(m_Handle){
        if(m_Handle->device_handle > 0)
            close(m_Handle->device_handle);
        delete m_Handle;
        m_Handle = NULL;
    }
#ifdef FILEBASEDAPPHANDLER
    LOG_HIGH <<"CHIDRawPacketWriter::deviceDisconnected() m_IPodHidDevicePath=" << m_IPodHidDevicePath <<  LOG_ENDL;
    if(!m_IPodHidDevicePath.isEmpty())
        QFile::remove(m_IPodHidDevicePath);
        QFile::remove(m_IPodPandoraAppSessionID);
//#ifdef DHAVN_COMPILE_FOR_AHA // added by jungae, 2012.12.01 for AhaRadio application
        QFile::remove(m_IPodAhaAppSessionID);
//#endif
#endif
    m_SocketNumber = -1;
#endif
    // } deleted by sungha.choi 2013.04.02 this object must not be deleted here
    //LOG_TRACE <<"CHIDRawPacketWriter::deviceDisconnected() OUT"<< LOG_ENDL;
}

#ifdef FILEBASEDAPPHANDLER
void CHIDRawPacketWriter::iPodReadyForApps(bool)
{
    //LOG_TRACE <<"CHIDRawPacketWriter::iPodReadyForApps() In"<< LOG_ENDL;
    if(!m_IPodHidDevicePath.isEmpty())
    {
	    QFile devFile(m_IPodHidDevicePath);
	    bool ret = devFile.open(QIODevice::WriteOnly);
	    //LOG_INFO << "CHIDRawPacketWriter::open file is ret = " << ret << LOG_ENDL;
	    if(ret)
	    {
		    QTextStream stream(&devFile);
		    stream << m_DevPath <<endl<<flush;
		    devFile.close();
	    }
    }
    //LOG_TRACE <<"CHIDRawPacketWriter::iPodReadyForApps() OUT"<< LOG_ENDL;
}
#endif


/***************************************************************************************************/
/* For CR 15396/15262, SetInterface(1, 1) should be sent by iPod controller before SetUIMode       */
/* IOCTL (HIDIOCSINTERFACE) for SetInterface is supported by usb audio class driver(BSP), CR 15262 */
/***************************************************************************************************/
#ifndef HIDIOCSINTERFACE
#define HIDIOCSINTERFACE(len)  _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x0F, len)
#endif
int CHIDRawPacketWriter::UsbAudioClassSetInterface(int interface_number, int alternate_setting)
{
    //LOG_TRACE << "CHIDRawPacketWriter::UsbAudioClassSetInterface Entry " << LOG_ENDL;

    int res = 0;
    int buf[2];

    if (m_SocketNumber < 0) {
        LOG_HIGH << "ioctl HIDIOCSINTERFACE handle error : " << res << LOG_ENDL;
        return -1;
    }

    memset(buf, 0x0, sizeof(buf));

    /* Set Feature */
    buf[0] = interface_number;
    buf[1] = alternate_setting;

    /* m_SocketNumber is Device handle set in hid_open_path() */
    res = ioctl(m_SocketNumber, HIDIOCSINTERFACE(2), buf);
    if (res < 0) {
        LOG_HIGH << "ioctl HIDIOCSINTERFACE error : " << res << LOG_ENDL;
        //LOG_TRACE << "ioctl HIDIOCSINTERFACE Fail returned : " << res  << LOG_ENDL;
    }
    else {
        //LOG_TRACE << "ioctl HIDIOCSINTERFACE Success returned: " << res  << LOG_ENDL;
    }

    return 0;
}


/*********************************************************************************************/
/*********************************************************************************************/
// CR16035/15993, USB Device Vendor Request to set available current for iPod charging, after '/dev/hidrawX' file open
#ifndef HIDIOCSVENDORREQ
#define HIDIOCSVENDORREQ(len)  _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x0E, len)
#endif
int CHIDRawPacketWriter::HIDVendorSpecificRequest(unsigned short request, unsigned short value, short index)
{
    struct hidraw_vendorreq {
        unsigned short	request;
        unsigned short	value;
        short           index;
        unsigned short	dummy;
    } vendorreq;

    int res = 0;
    unsigned short *if_buf;


    if (m_SocketNumber < 0) {
        LOG_HIGH << "ioctl HIDIOCSVENDORREQ handle error : " << res << LOG_ENDL;
        return -1;
    }

    vendorreq.request	= request;
    vendorreq.value     = value;
    vendorreq.index     = index;

    if_buf = (unsigned short *) &vendorreq;

    res = ioctl(m_SocketNumber, HIDIOCSVENDORREQ(3), if_buf);
    if (res < 0) {
      //LOG_TRACE << "ioctl HIDIOCSVENDORREQ Fail returned : %d\n" << res << LOG_ENDL;
    }
    else {
      //LOG_TRACE << "ioctl HIDIOCSVENDORREQ Success returned : %d\n" << res << LOG_ENDL;
    }

    return 0;
}
