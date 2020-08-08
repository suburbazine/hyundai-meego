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
#include <DHAVN_IPodController_hidRawPacketReader.h>
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
#include <DHAVN_IPodController_Def.h>
#include <DHAVN_IPodController_IAPPacketReader.h>

CHIDRawPacketReader::CHIDRawPacketReader(EIPodPorts nCurrentPortNum) : QObject(NULL)
{
    EIPodPorts portNum = nCurrentPortNum;

    switch(portNum){
    case eIPOD_PORT_1:
        LOG_INIT_V2(giPodControllerLogName_1);
        break;
    case eIPOD_PORT_2:
        LOG_INIT_V2(giPodControllerLogName_2);
        break;
    default:
         LOG_INIT_V2(giPodControllerLogName);
    }
    //LOG_INIT_V2(gControllerLogName);
    //LOG_TRACE<<"CHIDRawPacketReader::CHIDRawPacketReader IN"<<LOG_ENDL;
    m_DevPath.resize(0);
    m_notifier = NULL;
    m_SocketNumber = -1;
    m_isDisconnected = false;
//    setObjectName(gRawPcktHandler);

    //LOG_TRACE<<"CHIDRawPacketReader::CHIDRawPacketReader OUT"<<LOG_ENDL;
}

CHIDRawPacketReader::~CHIDRawPacketReader()
{
    if(m_notifier)
    {
        delete m_notifier;
        m_notifier = NULL;
    }

    LOG_HIGH << "~CHIDRawPacketReader(), close m_SocketNumber = " << m_SocketNumber << LOG_ENDL;
    if( m_SocketNumber >= 0 ){
        close(m_SocketNumber);
        m_SocketNumber = -1;    // added by sungha.choi 2013.04.02
    }
}

/****************************************************************************************/
/*                                  Public Methods                                     */
/****************************************************************************************/


int CHIDRawPacketReader::setHidDevice(int vendorId, int productId, QString hidDevPath)
{
    //LOG_TRACE<<"in CHIDRawPacketReader::setDevice() in"<<LOG_ENDL;

    setPath(hidDevPath);
    /* Clean the previous handler if any */
    return hid_open(vendorId, productId, NULL);

}


/****************************************************************************************/
/*                                  Private Methods                                     */
/****************************************************************************************/


int CHIDRawPacketReader::hid_open(unsigned short /*vendor_id*/, unsigned short /*product_id*/, char */*serial_number*/)
{
    //LOG_TRACE<<"in CHIDRawPacketReader::hid_open() IN"<<LOG_ENDL;
    if(m_DevPath.length())
    {
        //LOG_TRACE<<"CHIDRawPacketReader::hid_open:Trying to open hidDevice:"<<m_DevPath <<LOG_ENDL;
        return hid_open_path(m_DevPath.toAscii().data());
    }
    //LOG_TRACE<<"in CHIDRawPacketReader::hid_open() out"<<LOG_ENDL;
    return IPOD_ERROR_GENERIC;
}



int CHIDRawPacketReader::hid_open_path(const char *path)
{
    //LOG_TRACE<<"in CHIDRawPacketReader::hid_open_path()  IN"<<LOG_ENDL;

    if(path == NULL){
        LOG_HIGH<<"CHIDRawPacketReader::hid_open_path: hid file path is NULL"<<LOG_ENDL;
        return IPOD_ERROR_GENERIC;
    }

    // OPEN HERE //
    m_SocketNumber = open(path, O_RDWR|O_NONBLOCK);
    // If we have a good handle, return it.
    if (m_SocketNumber >= 0) {
        InitializaSocketNotifier( m_SocketNumber );
        LOG_HIGH<<"CHIDRawPacketReader, m_SocketNumber = " << m_SocketNumber << ", path = "<< path << LOG_ENDL;
        return IPOD_SUCCESS;
    }
    else
    {
        // For CR 14814, sometimes open() fail, so added '/dev/hidrawX/' file permission checking
        int i = 0;
        while(i < 600)	// CR16774, 200ms(1ms * 200) -> 3000ms(5ms * 600)
        {
            if(QFile::exists(path))
            {
                QFile::Permissions file_permission = QFile::permissions(path);
                //LOG_TRACE<<"CHIDRawPacketReader::hid_open_path() file permission - " << file_permission << LOG_ENDL;
                if((file_permission & (QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser | QFile::WriteUser
                                      | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther))
                        == (QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser | QFile::WriteUser
                            | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther))
                {
                    m_SocketNumber = open(path, O_RDWR | O_NONBLOCK);
                    // If we have a good handle, return it.
                    if (m_SocketNumber >= 0) {
//                        m_SocketNumber = fd;
                        InitializaSocketNotifier(m_SocketNumber);
                        LOG_HIGH<<"CHIDRawPacketReader::hid_open_path() opened the devices success, m_SocketNumber = " << m_SocketNumber << ", path = "<< path <<", " << i << " times" <<LOG_ENDL;
                        return IPOD_SUCCESS;
                    }

                    LOG_HIGH<<"CHIDRawPacketReader::hid_open_path() open failure !! m_SocketNumber - "<< m_SocketNumber <<LOG_ENDL;
                    break;  // real open fail
                }

                i++;
                if(i >= 600)
                    LOG_HIGH<<"CHIDRawPacketReader::hid_open_path() open failure - " << i << " time, permission - " << file_permission << LOG_ENDL;

                usleep(5000);  // CR16774, 200ms(1ms * 200) -> 3000ms(5ms * 600)
            }
            else // for removed case
            {
                //LOG_TRACE<<"CHIDRawPacketReader::hid_open_path() removed case !!"<<LOG_ENDL;
                break;
            }
        }
    }

    return IPOD_ERROR_GENERIC;
}


void CHIDRawPacketReader::InitializaSocketNotifier(int socketNumber)
{
    //LOG_TRACE<<"in CHIDRawPacketReader::InitializaSocketNotifier() IN"<<LOG_ENDL;
    // Enabling bellow is resulting in fake notification from socketnotifier.
    // int ret,flags = 1;
    // ret = ioctl(socketNumber,FIONBIO,&flags);

    /* Clean the previous notifier if exist */
    if(m_notifier){
        LOG_HIGH<<"CHIDRawPacketReader::InitializaSocketNotifier: Prev Notifier is Still live ....Error"<<LOG_ENDL;
        delete m_notifier;
        m_notifier = NULL;
    }

    m_notifier = new QSocketNotifier(socketNumber, QSocketNotifier::Read, this);
    if(m_notifier == NULL){
        LOG_HIGH<<"CHIDRawPacketReader::InitializaSocketNotifier: Failed to alloc m_Notifier"<<LOG_ENDL;
        return ;
    }
    connect(m_notifier, SIGNAL(activated(int)), this, SLOT(Notified(int)));
    //LOG_HIGH << "CHIDRawPacketReader, notifier is activated, fd = " << socketNumber << LOG_ENDL;
    //LOG_TRACE<<"in CHIDRawPacketReader::InitializaSocketNotifier() OUT"<<LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CHIDRawPacketReader::Notified                    (void) (OUT), ((int) (IN)
//
// Explanation
// Will be called whne the device descriptor is ready to be read, emits a signal
// to notify that reading is done
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CHIDRawPacketReader::Notified(int socket)
{
    ////LOG_TRACE<<"CHIDRawPacketReader::Notified() IN"<<LOG_ENDL;
    if(m_isDisconnected || m_SocketNumber < 0)    // added by sungha.choi 2013.04.15
    {
        return;
    }

    if(m_SocketNumber == socket)
    {
        char data[BYTESTOBEREAD];
        int bytes_read=0;

#if 1
        while((bytes_read = ::read(m_SocketNumber, data, BYTESTOBEREAD))>0)
        {
            QByteArray tempArray( data, bytes_read);
            constructiAPPacket(tempArray);
        }
        if(!QFile::exists(m_DevPath)){
            m_isDisconnected = true;
        }
#else
        if(m_SocketNumber > 0){
            bytes_read = ::read(m_SocketNumber, data, BYTESTOBEREAD);
        }

        if (bytes_read > 0){
            QByteArray tempArray( data, bytes_read);
            //printOutArray(tempArray);
            constructiAPPacket(tempArray);
        }else{
            //LOG_HIGH<<"CHIDRawPacketReader::Notified: Read Error ret=%d",bytes_read"<<LOG_ENDL;
            if(!QFile::exists(m_DevPath)){
                m_isDisconnected = true;
            }
        }
#endif
    }else{
        LOG_HIGH<<"CHIDRawPacketReader::HID fd mismatch....Error"<<LOG_ENDL;
    }
  //  //LOG_TRACE<<"CHIDRawPacketReader::Notified() OUT"<<LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CHIDRawPacketReader::setPath                    (void) (OUT), (QString) (IN)
//
// Explanation
// Set the path of the device desc to m_DevPath
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CHIDRawPacketReader::setPath(QString path)
{
    //LOG_TRACE<<"CHIDRawPacketReader::setPath() IN"<<LOG_ENDL;
    m_DevPath.resize(0);
    m_DevPath.append(path);
    //LOG_TRACE<<"setting path to CHIDRawPacketReader =" <<m_DevPath <<LOG_ENDL;
    //LOG_TRACE<<"CHIDRawPacketReader::setPath() OUT"<<LOG_ENDL;
}

void CHIDRawPacketReader::printOutArray(QByteArray array)
{
    //LOG_TRACE << "Received Packet from iPod: " << LOG_ENDL;
    int count = array.count()/8;
    count++;
    /*
    for(int i=0;i<count;i++)
    {
        //LOG_INFO<<"[" << i << "] = ["<< (array.mid(i*8, 8)).toHex().data()
                << "]" << LOG_ENDL;
    }
    */
}

/****************************************************************************************/
/*                                  Slots                                               */
/****************************************************************************************/


/******************************************************************************
// Function Name                                            Parameters
// CHIDRawPacketReader::deviceDisconnected                    (void) (OUT), (void) (IN)
//
// Explanation
// Slot to get the notification on device disconnected, disable the
// notification of read desc
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/

void CHIDRawPacketReader::closeOpenedFileDescriptor()
{
    if(m_SocketNumber >= 0)
    {
        close(m_SocketNumber);
        m_SocketNumber = -1;
    }
}

void CHIDRawPacketReader::onDeviceDisconnected()
{
    //LOG_TRACE<<"CHIDRawPacketReader::deviceDisconnected() IN"<<LOG_ENDL;
    m_isDisconnected = true;
#if 0  // { deleted by sungha.choi 2013.04.15 m_notifier is not thread safety
    if(m_notifier){
        if(m_notifier->isEnabled())
            m_notifier->setEnabled(false);
    	//connect(m_notifier, SIGNAL(activated(int)), this, SLOT(Notified(int)));
        //disconnect(m_notifier,0,0,0);
        //LOG_TRACE<<"CHIDRawPacketReader::deviceDisconnected: Deleting m_notifier"<<LOG_ENDL;
        disconnect(m_notifier, SIGNAL(activated(int)), this, SLOT(Notified(int)));
        // { deleted by sungha.choi 2013.04.02 this object must not be deleted here
        //delete m_notifier;
        //m_notifier = NULL;
        // } deleted by sungha.choi 2013.04.02 this object must not be deleted here
    }
#endif
#if 0  // { deleted by sungha.choi 2013.04.02 move to destoryer
    if(m_SocketNumber>0){
        close(m_SocketNumber);
    }
    m_SocketNumber = -1;
#endif
    // } deleted by sungha.choi 2013.04.02
    //LOG_TRACE<<"CHIDRawPacketReader::deviceDisconnected() OUT"<<LOG_ENDL;
}


/******************************************************************************
// Function Name                                            Parameters
// CHIDRawPacketReader::constructiAPPacket                (QByteArray) (OUT), (QByteArray) (IN)
//
// Explanation
// Construct the packet by removing the first two byes and return it to caller
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CHIDRawPacketReader::constructiAPPacket(QByteArray hidPacket)
{
    ////LOG_TRACE<<"CIAPPacketReader::constructiAPPacket IN"<<LOG_ENDL;
    switch(hidPacket[1])
    {
    case 0:
        emit iAPPacketReceived(hidPacket.mid(2), false);
        break;

    case 1://Last HID report in Large iAP Packet
        m_ReadArray.append(hidPacket.mid(2));
        emit iAPPacketReceived(m_ReadArray, true);
        break;

    case 2://First HID report in Large iAP Packet
        m_ReadArray.resize(0);
        m_ReadArray.append(hidPacket.mid(2));
        break;

    case 3://Middle HID report in Large iAP Packet
        m_ReadArray.append(hidPacket.mid(2));
        break;

    }
    ////LOG_TRACE<<"CIAPPacketReader::constructiAPPacket OUT \n");
}

void CHIDRawPacketReader::requestQuitSlot() // added by sungha.choi 2013.04.18
{
    if(m_isDisconnected)    // added by sungha.choi 2013.04.15
    {
        if(m_notifier)
        {
            if(m_notifier->isEnabled())
            {
                m_notifier->setEnabled(false);
            }
            disconnect(m_notifier, SIGNAL(activated(int)), this, SLOT(Notified(int)));
        }
    }
}



