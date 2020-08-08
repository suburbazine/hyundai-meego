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
#include <DHAVN_IPodAppController_IAPPacketReader.h>
#include <DHAVN_IPodAppController_hidRawPacketHandler.h>
#include <DHAVN_IPodAppController_global.h>
//#include <DHAVN_IPodController_CommonVars.h>
//#include <DHAVN_IPodController_Debug.h>
#include <DHAVN_IPodAppController_Private.h>
#include <QMetaType>

CIAPPacketReader::CIAPPacketReader(QObject* parent) : QObject(parent), m_Emit(false), m_isLargePacket(false)
{
    LOG_INIT_V2(gIPodAppControllerLogs);
    //LOG_TRACE << "CIAPPacketReader::CIAPPacketReader IN " << LOG_ENDL;
    setObjectName(gIAPReader);
    qRegisterMetaType<QByteArray>("QByteArray");

    //connect(parent->findChild<CHIDRawPacketHandle*>(gIAPRawPcktHandler), SIGNAL(hidPacketReceived(QByteArray&)),
    //        this, SLOT(hidReportAvalable(QByteArray&)) , Qt::QueuedConnection);
    connect(((CIPodAppControllerPriv*)parent)->m_RawPacketHandler, SIGNAL(hidPacketReceived(QByteArray)),
            this, SLOT(hidReportAvalable(QByteArray)) , Qt::QueuedConnection);


    //LOG_TRACE << "CIAPPacketReader::CIAPPacketReader OUT " << LOG_ENDL;
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
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIAPPacketReader::hidReportAvalable(QByteArray hidPacket)
{
    //LOG_TRACE << "CIAPPacketReader::hidReportAvalable IN" << LOG_ENDL;
    QByteArray tempArray = constructiAPPacket(hidPacket);
    if(m_Emit)
    {
        int i = 2; //Lingo Index in Small iAP Packet.
        qint16 command = 0x0000;

        if(0x00 == (int)tempArray[1]){/* this is Large Packet*/
            i = 4;//Lingo Index for Large iAP Packet.
        }
        if((int)tempArray[i] == 0x04)
        {

            command = command | tempArray[i+1];
            command = command <<8;
            command = command | tempArray[i+2];
            //emit packetAvailable(tempArray[i], command, tempArray);
        }
        else
        {
            command = tempArray[i+1];
            //emit packetAvailable(tempArray[i], tempArray[i+1], tempArray);
        }
        if(m_isLargePacket){
            emit largePacketAvailable(tempArray[i], command, tempArray);
        } else{
            emit packetAvailable(tempArray[i], command, tempArray);
        }
    }
    m_Emit=false;
    m_isLargePacket = false;
    //LOG_TRACE << "CIAPPacketReader::hidReportAvalable OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIAPPacketReader::constructiAPPacket                (QByteArray) (OUT), (QByteArray) (IN)
//
// Explanation
// Construct the packet by removing the first two byes and return it to caller
// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CIAPPacketReader::constructiAPPacket(QByteArray& hidPacket)
{
    //LOG_TRACE << "CIAPPacketReader::constructiAPPacket IN" << LOG_ENDL;
    switch(hidPacket[1])
    {
    case 0:
        m_Emit = true;
        return hidPacket.mid(2);

    case 1://Last HID report in Large iAP Packet
        m_Emit = true;
        m_isLargePacket = true;
        m_ReadArray.append(hidPacket.mid(2));
        return m_ReadArray;

    case 2://First HID report in Large iAP Packet
        m_ReadArray.resize(0);
        m_ReadArray.append(hidPacket.mid(2));
        return m_ReadArray;

    case 3://Middle HID report in Large iAP Packet
        m_ReadArray.append(hidPacket.mid(2));
        return m_ReadArray;

    }
    //LOG_TRACE << "CIAPPacketReader::constructiAPPacket OUT" << LOG_ENDL;
    return QByteArray(0);
}
