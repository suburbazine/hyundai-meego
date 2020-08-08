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
 *                   This is the IAP packet reader class          *
 *                   it reads the packet coming from the          *
 *                   raw packet handler and creates the           *
 *                   full IAP packet and emit the packet          *
 *                   received signal                              *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodController_IAPPacketReader.h>
#include <DHAVN_IPodController_hidRawPacketReader.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <QElapsedTimer>


CIAPPacketReader::CIAPPacketReader(EIPodPorts nCurrentPortNum, int vendorId, int productId, QString hidDevPath)
    : m_vendorId(vendorId), m_productID(productId), m_hidDevPath(hidDevPath)
{
    m_nCurrentPortNum = nCurrentPortNum;

    switch(nCurrentPortNum){
    case eIPOD_PORT_1:
        LOG_INIT_V2(giPodControllerLogName_1);
        break;
    case eIPOD_PORT_2:
        LOG_INIT_V2(giPodControllerLogName_2);
        break;
    default:
         LOG_INIT_V2(giPodControllerLogName);
    }

    //LOG_TRACE << "CIAPPacketReader::CIAPPacketReader() Entry" << LOG_ENDL;
    setObjectName(gReader);

    m_hidReader = new CHIDRawPacketReader(nCurrentPortNum);
    m_isDisconnected = false;

    if(m_hidReader==NULL){
        //LOG_TRACE<<"CIAPPacketReader::CIAPPacketReader: failed to allocate for m_hidReader"<<LOG_ENDL;
    } else {
        connect(m_hidReader, SIGNAL(iAPPacketReceived(QByteArray, bool)), this, SLOT(iAPPacketAvalable(QByteArray, bool)));
        connect(this, SIGNAL(requestQuit()), m_hidReader, SLOT(requestQuitSlot()));  // added by sungha.choi 2013.04.18
    }
    //LOG_TRACE << "CIAPPacketReader::CIAPPacketReader() Exit" << LOG_ENDL;
}
CIAPPacketReader::~CIAPPacketReader()
{
    // { modified by sungha.choi 2013.04.02
    if(m_hidReader)
    {
        delete m_hidReader;
        //m_hidReader->deleteLater();
        m_hidReader = NULL;
    }
    // } modified by sungha.choi 2013.04.02
}

void CIAPPacketReader::setDevice(int vendorId, int productId, QString hidDevPath)
{
    int ret = 0;
    if(NULL == m_hidReader){
        emit iAPReaderStatus(false);
        return ;
    }
    ret = m_hidReader->setHidDevice(vendorId, productId, hidDevPath);
    if(ret != IPOD_SUCCESS){
        //LOG_TRACE << "CIAPPacketReader::setDevice: failed to set the device to hidReader"<< LOG_ENDL;
        emit iAPReaderStatus(false);
        return;
    }
    emit iAPReaderStatus(true);
    return;
}

void CIAPPacketReader::closeFileDescriptorSlot()
{
    //LOG_TRACE << "CIAPPacketReader::closeFileDescriptorSlot" << LOG_ENDL;
    if(m_hidReader)
    {
        m_hidReader->closeOpenedFileDescriptor();
    }
}

void CIAPPacketReader::onDeviceDisconnect()
{
    //LOG_TRACE <<"CIAPPacketReader::onDeviceDisconnect In"<< LOG_ENDL;
    m_isDisconnected = true;

    if(m_hidReader){
        disconnect(m_hidReader, SIGNAL(iAPPacketReceived(QByteArray, bool)), this, SLOT(iAPPacketAvalable(QByteArray, bool)));
        m_hidReader->onDeviceDisconnected();
        emit requestQuit();    // added by sungha.choi 2013.04.18
    }
    //LOG_TRACE <<"CIAPPacketReader::onDeviceDisconnect OUT"<< LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIAPPacketReader::hidReportAvalable                 (void) (OUT), (QByteArray) (IN)
//
// Explanation
// Slot to handle the signal emitted by the raw packet handler
// once the data is available
// it construct the packets and wait for the multiple packets if
// the packets is breakdown in multiple part, integret them and finally emit
// the signal for the full length packet
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIAPPacketReader::iAPPacketAvalable(QByteArray iAPPacket, bool isLargePacket)
{
    if(m_isDisconnected)    // added by sungha.choi 2013.04.12
        return;

    int i = 2; //Lingo Index in Small iAP Packet.
    // bool isLargePacket = false;
    qint16 command = 0x0000;

    if( 0x00 == iAPPacket[1]){ /* this is Large Packet*/
        i = 4;//Lingo Index for Large iAP Packet.
        isLargePacket = true;
    }

    if((int)iAPPacket[i] == 0x04) {

        command = iAPPacket[i+1];
        command = command <<8;
        command = command | iAPPacket[i+2];
    }
    else {
        command = iAPPacket[i+1];
    }

    if(0x04 == iAPPacket[i] ){
        switch(command){
        case 0x27:
            /* This is Position and play status Cmd, so send it only to player engine taskhandler*/
            emit packetAvailableForPlayerTaskHandler(iAPPacket[i], command, iAPPacket);
            return;
        case 0x41:
            /* This is DB sync Cmd, so send it only to iPodDB taskhandler*/
            emit packetAvailableForDBTaskHandler(iAPPacket[i], command, iAPPacket);
            return;
        default: break;

        }
    }

    if(isLargePacket){
        emit largePacketAvailable(iAPPacket[i], command, iAPPacket);
    } else{
        emit packetAvailable(iAPPacket[i], command, iAPPacket);
    }

    ////LOG_TRACE<<"CIAPPacketReader::iAPPacketAvalable OUT" \n")"<<LOG_ENDL;
}

