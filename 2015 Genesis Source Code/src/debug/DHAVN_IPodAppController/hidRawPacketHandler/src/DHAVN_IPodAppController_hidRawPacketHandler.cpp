/*
 ******************************************************************
 *        COPYRIGHT © Teleca AB                                   *
 *----------------------------------------------------------------*
 * MODULE     :  IPodAppController                                *
 *
 * PROGRAMMERS : Pawan Gupta                                      *
 * DATE       :  16 jan 2012                                      *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :

 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodAppController_hidRawPacketHandler.h>
#include <linux/hidraw.h>
#include <linux/version.h>
#include <libudev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <QDebug>
#include <errno.h>
#include <DHAVN_IPodAppController_global.h>
#include <DHAVN_IPodAppController_Private.h>
//#include <QFile>

/******************************************************************************
// Function                                            Parameters
// CHIDRawPacketHandle::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
CHIDRawPacketHandle::CHIDRawPacketHandle(QSocketNotifier::Type type) : m_Type(type)
{
    LOG_INIT_V2(gIPodAppControllerLogs);
    //LOG_TRACE << "CHIDRawPacketHandle::CHIDRawPacketHandle IN \n" << LOG_ENDL;
    m_notifier = NULL;
    m_Handle = NULL;
    m_SocketNumber = -1;
    setObjectName(gIAPRawPcktHandler);
    qRegisterMetaType<QByteArray>("QByteArray");
    connect(this , SIGNAL(emitToWrite(QByteArray)) , this ,
            SLOT(writeToSock(QByteArray)) , Qt::QueuedConnection);
    //LOG_TRACE << "CHIDRawPacketHandle::CHIDRawPacketHandle OUT \n" << LOG_ENDL;
}

/******************************************************************************
// Function                                            Parameters
// CHIDRawPacketHandle::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
CHIDRawPacketHandle::~CHIDRawPacketHandle()
{
    //LOG_TRACE << "CHIDRawPacketHandle::~CHIDRawPacketHandle() IN" << LOG_ENDL;

    // { modified by jungae 2012.01.07 for crash issue
    //delete m_notifier;
    if(m_notifier)
    {
        delete m_notifier;
        m_notifier = NULL;
    }
   // } modified by jungae  
    if(m_Handle){
        if(m_Handle->device_handle > 0)
            close(m_Handle->device_handle);
        delete m_Handle;
    }
    //LOG_TRACE << "CHIDRawPacketHandle::~CHIDRawPacketHandle() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CHIDRawPacketHandle::read                    int (OUT), (QByteArray) (IN)
//
// Explanation
// not implemented
// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CHIDRawPacketHandle::read(QByteArray& /*hidData*/)
{
    return 0;
}

int CHIDRawPacketHandle::write(QByteArray& hidData)
{
    emit emitToWrite(hidData);
    return hidData.length();
}

/******************************************************************************
// Function Name                                            Parameters
// CHIDRawPacketHandle::write                    int (OUT), (QByteArray) (IN)
//
// Explanation
// Writes the command to the and returns the bytes written
// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CHIDRawPacketHandle::writeToSock(QByteArray hidData)
{
    //LOG_TRACE << "CHIDRawPacketHandle::write IN\n " << LOG_ENDL;
    if(m_Handle == NULL){
        return;
    }
    int bytes_written = ::write(m_Handle->device_handle, hidData, hidData.length());
    //LOG_TRACE << "CHIDRawPacketHandle::write OUT with bytes written = " <<  bytes_written << LOG_ENDL;
    CIpodAppControllerGlobal::printArray(hidData);
   // return bytes_written;
}

/******************************************************************************
// Function                                            Parameters
// CHIDRawPacketHandle::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
EIPodAppErrors CHIDRawPacketHandle::hid_open(const QString& hidPath)
{
    EIPodAppErrors res=IPOD_APPERROR_NOTCONNECTED;
    //LOG_TRACE << "in CHIDRawPacketHandle::hid_open() IN\n" << LOG_ENDL;
    //LOG_TRACE << "path: "  << hidPath.toAscii().data() << LOG_ENDL;

    if(hidPath.length())
    {
        //LOG_TRACE << "path found from the detector: " << hidPath.toAscii().data() << LOG_ENDL;
        m_Handle = hid_open_path(hidPath.toAscii().data());
        if(m_Handle != NULL){
            if(m_Handle->device_handle > 0)
                res = IPOD_APPSUCCESS;
    }
    else
    {
        //LOG_TRACE << "no device found" << LOG_ENDL;
    }
    }
    //LOG_TRACE << "in CHIDRawPacketHandle::hid_open() out\n" << LOG_ENDL;
    return res;
}

/******************************************************************************
// Function                                            Parameters
// CHIDRawPacketHandle::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
hid_device* CHIDRawPacketHandle::new_hid_device()
{
    //LOG_TRACE << "in CHIDRawPacketHandle::new_hid_device() IN \n" << LOG_ENDL;
    hid_device *dev = new hid_device;
    dev->device_handle = -1;
    dev->blocking = 0;
    dev->uses_numbered_reports = 0;
    //LOG_TRACE << "in CHIDRawPacketHandle::new_hid_device() out \n" << LOG_ENDL;
    return dev;
}

/******************************************************************************
// Function                                            Parameters
// CHIDRawPacketHandle::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
hid_device * CHIDRawPacketHandle::hid_open_path(const char *path)
{
    //LOG_TRACE << "in CHIDRawPacketHandle::hid_open_path()  IN\n" << LOG_ENDL;
    hid_device *dev = NULL;

    if(path != NULL){
        dev = new_hid_device();
        if(dev){
    // OPEN HERE //
            dev->device_handle = open(path, O_RDWR | O_NONBLOCK);

    // If we have a good handle, return it.
            if (dev->device_handle > 0) {
                m_SocketNumber = dev->device_handle;
                InitializaSocketNotifier(dev->device_handle);
                /* Get the report descriptor */
                int res, desc_size = 0;
                struct hidraw_report_descriptor rpt_desc;

                memset(&rpt_desc, 0x0, sizeof(rpt_desc));

                /* Get Report Descriptor Size */
                res = ioctl(dev->device_handle, HIDIOCGRDESCSIZE, &desc_size);
                if (res < 0){
                    //LOG_TRACE << "CHIDRawPacketHandle::hid_open_path: HIDIOCGRDESCSIZE Failed\n" << LOG_ENDL;
                }
               /* Get Report Descriptor */
                rpt_desc.size = desc_size;
                res = ioctl(dev->device_handle, HIDIOCGRDESC, &rpt_desc);
                if (res < 0) {
                    //LOG_TRACE << "CHIDRawPacketHandle::hid_open_path: HIDIOCGRDESC Failed\n" << LOG_ENDL;
                } else {
                    /* Determine if this device uses numbered reports. */
                    dev->uses_numbered_reports =
                            uses_numbered_reports(rpt_desc.value,
                                                  rpt_desc.size);
                }
                //LOG_TRACE << "in CHIDRawPacketHandle::hid_open_path() opened the devices  \n" << LOG_ENDL;
                return dev;
            } else {
                //LOG_TRACE << "in CHIDRawPacketHandle::hid_open_path() Unable to open any devices \n" << LOG_ENDL;
                delete dev;
                return NULL;
            }
        }
    }
    return NULL;
}

/******************************************************************************
// Function Name                                            Parameters
// CHIDRawPacketHandle::uses_numbered_reports                    (int) (OUT), ((quint8*, quint32)) (IN)
//
// Explanation
// Check if the device uses the numberd report then created the
// Report ID X Size value pair for the future use
// History
// 06-jan-20121      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CHIDRawPacketHandle::uses_numbered_reports(quint8 *report_descriptor, quint32 size) {
    //LOG_TRACE << "in CHIDRawPacketHandle::uses_numbered_reports() in\n" << LOG_ENDL;
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
            //LOG_TRACE << "id: %02hhx" <<  report_descriptor[i+1] << LOG_ENDL;
            found =1;
        }

        if (key == 0x95/*Report ID*/) {
            /* This device has a Report ID, which means it uses
                           numbered reports. */
            //LOG_TRACE << "count: %02hhx" <<  report_descriptor[i+1] << LOG_ENDL;
            found =1;
        }

        qDebug("key: %02hhx\n", key);

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
            /*Commented out for coverity. default case will not come before we have 0x03 up there. it always between 0-3
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
    //LOG_TRACE << "in CHIDRawPacketHandle::uses_numbered_reports() out\n" << LOG_ENDL;
    /* Didn't find a Report ID key. Device doesn't use numbered reports. */
    return found;
}

/******************************************************************************
// Function                                            Parameters
// CHIDRawPacketHandle::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CHIDRawPacketHandle::InitializaSocketNotifier(int socketNumber)
{
    //LOG_TRACE << "in CHIDRawPacketHandle::InitializaSocketNotifier() in\n" << LOG_ENDL;
    int ret,flags = 1;
    ret = ioctl(socketNumber,FIONBIO,&flags);

/* Clean the previous notifier if exist */
    if(m_notifier){
        //LOG_TRACE << "CHIDRawPacketHandle::InitializaSocketNotifier: Prev Notifier is Still live ....Error" << LOG_ENDL;
        delete m_notifier;
        m_notifier = NULL;
    }

    m_notifier = new QSocketNotifier(socketNumber, m_Type, this);
    if(m_notifier == NULL){
        //LOG_TRACE << "CHIDRawPacketHandle::InitializaSocketNotifier: Failed to alloc m_Notifier\n" << LOG_ENDL;
        return ;
    }
    connect(m_notifier, SIGNAL(activated(int)), this, SLOT(Notified(int)));
    //LOG_TRACE << "in CHIDRawPacketHandle::InitializaSocketNotifier() out\n" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CHIDRawPacketHandle::Notified                    (void) (OUT), ((int) (IN)
//
// Explanation
// Will be called whne the device descriptor is ready to be read, emits a signal
// to notify that reading is done
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CHIDRawPacketHandle::Notified(int socket)
{
    //LOG_TRACE << "in CHIDRawPacketHandle::Notified() IN\n" << LOG_ENDL;
    if(m_SocketNumber == socket)
    {
        switch(m_Type)
        {
        case QSocketNotifier::Read:
            {
                char data[BYTESTOBEREAD];
                int bytes_read=0;

                do
                {
                    if(m_Handle != NULL)
                    {
                        bytes_read = ::read(m_Handle->device_handle, data, BYTESTOBEREAD);
                    }

                    if (bytes_read <= 0 && errno == EAGAIN)
                    {
                        //   bytes_read = 0;
                        break;
                    }

                    QByteArray tempArray( data, bytes_read);
                    CIpodAppControllerGlobal::printArray(tempArray);

                    //Tan: do some sleep to intentionally cause problem this is to test the thread only.
                    //usleep(5000);
                    emit hidPacketReceived(tempArray);

                }while(bytes_read > 0);

                break;
            }
        case QSocketNotifier::Write:
        case QSocketNotifier::Exception:

            break;
        }
    }else{
        //LOG_TRACE << "in CHIDRawPacketHandle::HID fd mismatch....Error\n" << LOG_ENDL;
    }
    //LOG_TRACE << "in CHIDRawPacketHandle::Notified() OUT\n" << LOG_ENDL;
}
