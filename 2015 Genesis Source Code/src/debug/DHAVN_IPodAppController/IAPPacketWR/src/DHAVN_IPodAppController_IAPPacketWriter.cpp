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
#include <DHAVN_IPodAppController_IAPPacketWriter.h>
#include <DHAVN_IPodAppController_hidRawPacketHandler.h>
#include <DHAVN_IPodAppController_global.h>
#include <DHAVN_IPodAppController_Private.h>
CIAPPacketWriter::CIAPPacketWriter(QObject* parent) : QObject(parent)
{
    LOG_INIT_V2(gIPodAppControllerLogs);
    //LOG_TRACE << "CIAPPacketWriter::CIAPPacketWriter IN" << LOG_ENDL;
    setObjectName(gIAPWriter);
    //LOG_TRACE << "CIAPPacketWriter::CIAPPacketWriter OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIAPPacketWriter::SendPacket                (void) (OUT), (CIAPPacket) (IN)
//
// Explanation
// Send the packet to the raw packet handler to write it to the device
// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CIAPPacketWriter::SendPacket(CIAPPacket& packetData)
{
    //LOG_TRACE << "CIAPPacketWriter::SendPacket IN " << LOG_ENDL;
    return (DispachIAPPacket(packetData));
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
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CIAPPacketWriter::DispachIAPPacket(CIAPPacket& packetData)
{
    //LOG_TRACE << "CIAPPacketWriter::DispachIAPPacket IN " << LOG_ENDL;
    int retVal = 0;
    //Add the report ID byte and the synch byte to the byte array
    //make it HID packet and split it if needed into multiple packets

    QList <int> reportIdList;
    QList <int> reportSizeList;

    int count =0;
    QByteArray tempArray;//(packetData.getCommandBytes());

    CIpodAppControllerGlobal::printArray(packetData.getCommandBytes());

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
        CHIDRawPacketHandle* handler = ((CIPodAppControllerPriv*)parent())->m_RawPacketHandler;
        //CHIDRawPacketHandle* handler = parent()->findChild<CHIDRawPacketHandle*>(gIAPRawPcktHandler);
        retVal=handler->write(sendPacket);
        elementCount++;
        position +=repsize-2;
    }
    //LOG_TRACE << "CIAPPacketWriter::DispachIAPPacket OUT " << LOG_ENDL;
    return retVal;
}

/******************************************************************************
// Function Name                                            Parameters
// CIAPPacketWriter::lookupReport                (int, int) (OUT), (int) (IN)
//
// Explanation
// returns the size and the report ID for the current command
// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CIAPPacketWriter::lookupReport(int size, int& size_out)
{
    //LOG_TRACE << "CIAPPacketWriter::lookupReport IN " << LOG_ENDL;
    int i;
    for (i=0; reports[i][0]!=0; i++) {
        if (reports[i][1] >= size || reports[i+1][1] == 0x00) {
            size_out = reports[i][1];
            return reports[i][0];
        }
    }
    size_out  = reports[i][1];
    //LOG_TRACE << "CIAPPacketWriter::lookupReport EXIT " << LOG_ENDL;
    return reports[i][0];

}
