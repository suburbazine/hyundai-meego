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
 *                   This is the IAP packet writer class          *
 *                   it writes the packet coming from the         *
 *                   Task handlers handler and creates the        *
 *                   full HID packet and writes them to           *
 *                   raw packet handler                           *                                             *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodController_IAPPacketWriter.h>
#include <DHAVN_IPodController_PrivateData.h>

CIAPPacketWriter::CIAPPacketWriter(EIPodPorts nCurrentPortNum, int vendorId, int productId, QString hidDevPath)
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

    //LOG_INIT_V2(gControllerLogName);
    //LOG_TRACE << "CIAPPacketReader::CIAPPacketWriter() Entry" << LOG_ENDL;
    setObjectName(gWriter);

    m_maxReportSize = 0;
    m_isDisconnected = false;

    m_hidWriter = new CHIDRawPacketWriter(nCurrentPortNum);
    if(m_hidWriter==NULL){
        //LOG_TRACE << "CIAPPacketWriter::CIAPPacketWriter: failed to allocate for m_hidWriter"<< LOG_ENDL;
    }
    //LOG_TRACE << "CIAPPacketWriter::CIAPPacketWriter OUT"<< LOG_ENDL;
}

CIAPPacketWriter::~CIAPPacketWriter()
{
    // { modified by sungha.choi 2013.04.02
    if(m_hidWriter)
    {
        delete m_hidWriter;
        //m_hidWriter->deleteLater();
        m_hidWriter = NULL;
    }
    // } modified by sungha.choi 2013.04.02
}

void CIAPPacketWriter::setDevice(int vendorId, int productId, QString hidDevPath)
{
    int ret = 0;
    if(NULL == m_hidWriter){
        emit iAPWriterStatus(false);
        return;
    }
    ret = m_hidWriter->setHidDevice(vendorId, productId, hidDevPath);
    if(ret!=IPOD_SUCCESS){
        //LOG_TRACE<<"CIAPPacketWriter::setDevice: failed to set the device to hidWriter"<<LOG_ENDL;
        emit iAPWriterStatus(false);
        return;
    }
    //LOG_TRACE<<"CIAPPacketWriter::setDevice: Successfuly set the device to hidWriter"<<LOG_ENDL;
    emit iAPWriterStatus(true);
    return;
}

void CIAPPacketWriter::closeFileDescriptorSlot()
{
    //LOG_TRACE << "CIAPPacketWriter::closeFileDescriptorSlot" <<LOG_ENDL;
    if(m_hidWriter)
    {
        m_hidWriter->closeOpenedFileDescriptor();
    }
}

void CIAPPacketWriter::onDeviceDisconnect()
{
    m_isDisconnected = true;

    //LOG_TRACE<<"CIAPPacketWriter::onDeviceDisconnect In"<<LOG_ENDL;
    if(m_hidWriter){
        m_hidWriter->onDeviceDisconnected();
    }
    //LOG_TRACE<<"CIAPPacketWriter::onDeviceDisconnect OUT"<<LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIAPPacketWriter::SendPacket                (void) (OUT), (CIAPPacket) (IN)
//
// Explanation
// Send the packet to the raw packet handler to write it to the device
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
/* void CIAPPacketWriter::SendPacket(CIAPPacket packetData)
{
    //LOG_TRACE <<"CIAPPacketWriter::SendPacket IN"<< LOG_ENDL;
    int ret;
    //return (DispachIAPPacket(packetData));
    ret = DispachIAPPacket(packetData);
}
*/

void CIAPPacketWriter::sendiAPPacket(QByteArray packetBytes)
{
    if(m_isDisconnected) // added by sungha.choi 2013.04.12
        return;

    // //LOG_TRACE <<"CIAPPacketWriter::sendiAPPacket slot IN"<< LOG_ENDL;
    int ret = 0;
    ret = m_hidWriter->DispatchiAPToHid(packetBytes);
    // //LOG_TRACE <<"CIAPPacketWriter::sendiAPPacket slot OUT"<< LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIAPPacketWriter::DispachIAPPacket                (int) (OUT), (CIAPPacket) (IN)
//
// Explanation
// Send the packet to the raw packet handler to write it to the device
// before sending the packet as bytearray to raw packet hanlder it get the
// appropriate report id from the system and prepand it to the
// command array with the link control byte
// break down the packet into multiple packets if the size is too big to handle
// by the device
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CIAPPacketWriter::DispachIAPPacket(CIAPPacket& packetData)
{
#ifdef DBUGLOGS
    //LOG_TRACE << "CIAPPacketReader::DispachIAPPacket() Entry" << LOG_ENDL;
#endif
    int retVal = 0;
    //Add the report ID byte and the synch byte to the byte array
    //make it HID packet and split it if needed into multiple packets

    QList <int> reportIdList;
    QList <int> reportSizeList;

    int count =0;
    QByteArray tempArray;//(packetData.getCommandBytes());
    while(count < packetData.getCommandBytes().count())
    {
        //tempArray.resize(0);
        tempArray = packetData.getCommandBytes().mid(count);
        int size=0;
        int reportId = lookupReport(tempArray.count()+2, size);
        reportIdList.append(reportId);
        reportSizeList.append(size);
        count += size;
    }
    int elementCount=0;// = reportIdList.count();
    int position=0;
    while(elementCount < reportIdList.count())
    {
        int repsize = reportSizeList[elementCount];
        int repId = reportIdList[elementCount];
        QByteArray sendPacket;
        sendPacket.resize(2);
        sendPacket[0] = repId;
        if(reportIdList.count() ==1)
            sendPacket[1] = 0x00;
        else if(elementCount == (reportIdList.count()-1))
            sendPacket[1] = 0x01;
        else if(elementCount==0 && reportIdList.count() >1)
            sendPacket[1] = 0x02;
        else
            sendPacket[1] = 0x03;

        QByteArray tempArr = packetData.getCommandBytes().mid(position, repsize-2);
        sendPacket.append(tempArr);
        if(m_hidWriter)
            m_hidWriter->hidWrite(sendPacket);
        elementCount++;
        position +=repsize-2;
    }
#ifdef DBUGLOGS
    //LOG_TRACE << "CIAPPacketReader::DispachIAPPacket() Exit" << LOG_ENDL;
#endif
    return retVal;
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
int CIAPPacketWriter::lookupReport(int size, int& size_out)
{
#ifdef DBUGLOGS
    //LOG_TRACE << "CIAPPacketReader::lookupReport() Entry" << LOG_ENDL;
#endif
    int i;
    for (i=0; reports[i][0]!=0; i++) {
        if (reports[i][1] >= size || reports[i+1][1] == 0x00) {
            size_out = reports[i][1];
            return reports[i][0];
        }
    }
    size_out  = reports[i][1];
#ifdef DBUGLOGS
    //LOG_TRACE << "CIAPPacketReader::lookupReport() Exit" << LOG_ENDL;
#endif
    return reports[i][0];
}

void CIAPPacketWriter::setMaxReportSize()
{
    int i;
    int maxSize = 0;
    for (i=0; reports[i][0]!=0; i++) {
        if (reports[i][1] >= maxSize) {
            maxSize = reports[i][1];
        }
    }
    m_maxReportSize = maxSize;
}
int CIAPPacketWriter::getProductID()
{
    return m_productID;
}


void CIAPPacketWriter::onAuthorised(bool status)
{
    //LOG_TRACE << "at onAuthorised slot"<<LOG_ENDL;
    //LOG_TRACE<<"CIAPPacketWriter::onAuthorised In"<<LOG_ENDL;
    #ifdef FILEBASEDAPPHANDLER
    if(m_hidWriter){
        m_hidWriter->iPodReadyForApps(status);
    }
    #endif
    //LOG_TRACE<<"CIAPPacketWriter::onAuthorised OUT"<<LOG_ENDL;
}

/* For CR 15396, SetInterface(1, 1) should be sent by iPod controller before SetUIMode */
int CIAPPacketWriter::UsbAudioClassSetInterface(int interface_number, int alternate_setting)
{
	// Modified by jonghwan.cho@lge.com	2013.03.29.
	// for static test
    //m_hidWriter->UsbAudioClassSetInterface(interface_number, alternate_setting);
    return m_hidWriter->UsbAudioClassSetInterface(interface_number, alternate_setting);
}

// CR16035/15993, USB Device Vendor Request to set available current for iPod charging, after '/dev/hidrawX' file open
int CIAPPacketWriter::HIDVendorSpecificRequest(unsigned short request, unsigned short value, short index)
{
	// Modified by jonghwan.cho@lge.com	2013.03.29.
	// for static test
    //m_hidWriter->HIDVendorSpecificRequest(request, value, index);
    return m_hidWriter->HIDVendorSpecificRequest(request, value, index);
}
