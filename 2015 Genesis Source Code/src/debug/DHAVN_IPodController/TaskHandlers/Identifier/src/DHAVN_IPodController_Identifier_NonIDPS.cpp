/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  26 Sept 2012                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                   This is the Identification TaskHandler       *
 *                   it Starts and handle the whole iden-         *
 *                   tification process                           *                                              *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodController_Identifier_NonIDPS.h>
#include <DHAVN_IPodController_ack.h>
#include <QDebug>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_IAPPacketReader.h>
#include <DHAVN_IPodController_IAPPacketWriter.h>
#include <QTimer>
#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
#include <DHAVN_IPodController_retAccessoryInfo.h>
#include <DHAVN_IPodController_getAccessoryInfo.h>
#include <DHAVN_IPodController_enterRemoteUiMode.h>
#include <DHAVN_IPodController_eventloopTimer.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_Performance.h>
#include <DHAVN_IPodController_PerformanceLog.h>

using namespace GeneralLingo;

CIdentifierNonIDPSTaskHandler::CIdentifierNonIDPSTaskHandler(QObject *parent)
    :CIdentifierTaskHandler(parent)
{
    m_EmitIdentify = false;
    m_iPodPrivateData = parent->findChild<iPodControllerPrivateData*>(gControllerPrivateData);
    EIPodPorts portNum = m_iPodPrivateData->m_currentPortNum ;
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

    //LOG_TRACE << "CIdentifierNonIDPSTaskHandler::CIdentifierNonIDPSTaskHandler() IN" << LOG_ENDL;
    setObjectName(gIdentifier);

    /* connect the task handler to iAP channel */
//    connect(this, SIGNAL(sendiAPPacket(QByteArray)) ,((CIpodController*) parent)->m_Writer, SLOT(sendiAPPacket(QByteArray)));
//    connect(((CIpodController*) parent)->m_Reader,SIGNAL(packetAvailable(int, int, QByteArray)),this, SLOT(packetAvailable(int,int,QByteArray)));
    //LOG_TRACE << "CIdentifierNonIDPSTaskHandler::CIdentifierNonIDPSTaskHandler() OUT" << LOG_ENDL;

}

CIdentifierNonIDPSTaskHandler::~CIdentifierNonIDPSTaskHandler()
{
}

/******************************************************************************
// Function Name                                            Parameters
// CIdentifierNonIDPSTaskHandler::identify_restart_slot     (void) (OUT), (QBitArray, QList) (IN)
//
// Explanation
// Start the identification process
// History
// 18-Dec-2012      SK Paek                    Added initial implementation
*****************************************************************************/
// Added for identify retry(CR 31200)
void CIdentifierNonIDPSTaskHandler::identify_restart_slot()
{
    //LOG_TRACE << "CIdentifierNonIDPSTaskHandler::identify_restart_slot() IN" << LOG_ENDL;

    if(m_isDisconnected)    // if already disconnected
    {
        LOG_HIGH << "CIdentifierNonIDPSTaskHandler::identify_restart_slot() Disconnected Exit " << LOG_ENDL;
        return;
    }

    m_MaxWritePacketSize = 0;
    m_EndIdpsState       = false;
    m_CommandRetry       = 0;

    //LOG_TRACE << "   call identifyDeviceLingoReset()" << LOG_ENDL;
    identifyDeviceLingoReset ();

    // Set waiting status of CIAPCmds::L0_13_IdentifyDeviceLingoes Ack
    m_WaitingResponseOfCommand = CIAPCmds::L0_13_IdentifyDeviceLingoes; // added for IDPS retry(CR 13200)

    //LOG_TRACE << "CIdentifierNonIDPSTaskHandler::identify_restart_slot() OUT" << LOG_ENDL;
}


/******************************************************************************
// Function Name                                            Parameters
// CIdentifierNonIDPSTaskHandler::identify                         (void) (OUT), (QBitArray, QList) (IN)
//
// Explanation
// Start the identification process
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIdentifierNonIDPSTaskHandler::identify(QBitArray /*supportedLingoes*/, QList<FIDTokenValue> /*fidTokenValues*/)
{
    //LOG_TRACE << "CIdentifierNonIDPSTaskHandler::identify() IN" << LOG_ENDL;
    ((CIpodController*)parent())->m_IsIDPS = false;
    m_iPodPrivateData->m_isIDPS = false;
    //LOG_TRACE << "   call identifyDeviceLingoReset()" << LOG_ENDL;
    identifyDeviceLingoReset ();
    //LOG_TRACE << "CIdentifierNonIDPSTaskHandler::identify() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIdentifierNonIDPSTaskHandler::identifyDeviceLingoReset                         (void) (OUT), (void) (IN)
//
// Explanation
// send the identifyDeviceLingoReset command
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIdentifierNonIDPSTaskHandler::identifyDeviceLingoReset()
{
    //LOG_TRACE << "CIdentifierNonIDPSTaskHandler::identifyDeviceLingoReset() IN" << LOG_ENDL;
    //no trans id	int m_TransId = 8;
    QByteArray cmd;
    cmd.resize(17);
    cmd[0] = 0x55;
    cmd[1] = 0x0E;
    cmd[2] = 0x00;
    cmd[3] = 0x13;
    //device lingos spoken 4 bytes
    cmd[4] = 0x00;
    cmd[5] = 0x00;
    cmd[6] = 0x00;
    cmd[7] = 0x01;
    //device options  4 bytes
    cmd[8] = 0x00;
    cmd[9] = 0x00;
    cmd[10] = 0x00;
    cmd[11] = 0x00;

    //device id  4 bytes
    cmd[12] = 0x00;
    cmd[13] = 0x00;
    cmd[14] = 0x00;
    cmd[15] = 0x00;

    cmd[16] = CIpodControllerGlobal::checkSum(cmd, 15);
    CIAPPacket packet(cmd);

    //(parent()->findChild<CIAPPacketWriter*>(gWriter))->SendPacket(packet);
    emit sendiAPPacket(packet.getCommandBytes());
    //LOG_TRACE << "CIdentifierNonIDPSTaskHandler::identifyDeviceLingoReset() OUT" << LOG_ENDL;
}

void CIdentifierNonIDPSTaskHandler::identifyDeviceLingo()
{
    //LOG_TRACE << "CIdentifierNonIDPSTaskHandler::identifyDeviceLingo() IN" << LOG_ENDL;
    //no trans id	int m_TransId = 8;
    QByteArray cmd;
    cmd.resize(17);
    cmd[0] = 0x55;
    cmd[1] = 0x0E;
    cmd[2] = 0x00;
    cmd[3] = 0x13;
    //device lingos spoken 4 bytes
    cmd[4] = 0x00;
    cmd[5] = 0x00;
    cmd[6] = 0x04;
    //cmd[7] = 0x11;
    cmd[7] = 0x19;   // Modified by jonghwan.cho@lge.com 2013.04.30. for setposition
    //device options  4 bytes
    cmd[8] = 0x00;
    cmd[9] = 0x00;
    cmd[10] = 0x00;
    cmd[11] = 0x02;

    //device id  4 bytes
    cmd[12] = 0x00;
    cmd[13] = 0x00;
    cmd[14] = 0x02;
    cmd[15] = 0x00;

    cmd[16] = CIpodControllerGlobal::checkSum(cmd, 15);
    CIAPPacket packet(cmd);

    emit sendiAPPacket(packet.getCommandBytes());
    //LOG_TRACE << "CIdentifierNonIDPSTaskHandler::identifyDeviceLingo() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIdentifierNonIDPSTaskHandler::packetAvailable                   (void) (OUT), (int, int, QByteArray) (IN)
//
// Explanation
// Slot to handle the command which is read by the raw packet handler
// check for the last command status and on the basis of the status
// takes next action
// handle the whole flow of the IDPS process
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIdentifierNonIDPSTaskHandler::packetAvailable(int lingoID,int commandID,const QByteArray& buf )
{
#ifdef DBUGLOGS
    //LOG_TRACE << "CIdentifierNonIDPSTaskHandler::packetAvailable() IN, cmd = " << commandID << ", LingoID = "<< lingoID << LOG_ENDL;
#endif
    try
    {
        // if identify process is retrying(CR 13200)
        if(m_WaitingResponseOfCommand == CIAPCmds::L0_13_IdentifyDeviceLingoes)
        {
            // waiting Ack for CIAPCmds::L0_13_IdentifyDeviceLingoes
            if(lingoID == 0x00) // for general Lingo
            {
                if(commandID == CIAPCmds::L0_02_iPodAck)
                {
                    CACK tmp_cmd( m_iPodPrivateData->m_isIDPS );
                    tmp_cmd.unmarshall(buf);
                    int tmp_ackCmd= tmp_cmd.getAcknowledgedCommand();
                    if(tmp_ackCmd == CIAPCmds::L0_13_IdentifyDeviceLingoes)
                    {
                        m_WaitingResponseOfCommand = -1;      // Clear waiting status of CIAPCmds::L0_13_IdentifyDeviceLingoes Ack
                        return;
                    }
                }
            }

            //LOG_INFO << "   lingoID = " << lingoID << ", command ID = " << commandID << " is ignored until Ack for CIAPCmds::L0_13_IdentifyDeviceLingoes" << LOG_ENDL;
            return; // ignore all commands/response to prohibit unexpected working until Ack for CIAPCmds::L0_13_IdentifyDeviceLingoes
        }


        if(lingoID == 0x00) // for general Lingo
        {
            switch(commandID)
            {
                case 0x02: //ACK = 0x02
                {
                    CACK cmd( m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);
                    int ackCmd= cmd.getAcknowledgedCommand();
                    //LOG_TRACE << "/////////////its an Acknoledge command return/////////// " << LOG_ENDL;
                    //LOG_INFO << "   ACK command ID = "  << ackCmd << LOG_ENDL;
                    //LOG_INFO << "   ACK command status = " << cmd.getAckStatus() << LOG_ENDL;
                    switch(ackCmd)
                    {
                        case 0x0028:
                        {
                            SAccessoryInfo info;
                            if(!cmd.getAckStatus())
                            {
                            }//unsuccessful
                        }
                            break;
                        default :
                            break;
                    }
                }
                    break;
                case 0x27: //GetAccInfo
                {
                    CGetAccessoryInfo cmd(m_iPodPrivateData->getTransactionId(false));
                    cmd.unmarshall(buf);
                    //LOG_INFO << "   RetAccessoryInfo to be sent for type " << cmd.getInfoType() << LOG_ENDL;
                    switch(cmd.getInfoType())
                    {
                        case 0:
                        {
                            SAccessoryInfo info;
                            //LOG_TRACE << "   0 for RetAccessoryInfo" << LOG_ENDL;
                            CRetAccessoryInfo infoCmd1(1,CRetAccessoryInfo::CAPABILITIES,info);
                            infoCmd1.marshall();
                            emit sendiAPPacket(infoCmd1.getCommandBytes());
                            break;
                        }
                        case 1:
                        {
                            SAccessoryInfo info;
                            //LOG_TRACE << "   1 for RetAccessoryInfo" << LOG_ENDL;
                            info.m_StringToSend = "DH AVN 2.0"; // acc name
                            CRetAccessoryInfo infoCmd2(1,CRetAccessoryInfo::ACCESSORYNAME,info);
                            infoCmd2.marshall();
                            emit sendiAPPacket(infoCmd2.getCommandBytes());
                            break;
                        }
                        case 4:
                        {
                            SAccessoryInfo info;
                            //LOG_TRACE << "   4 for RetAccessoryInfo" << LOG_ENDL;
                            CRetAccessoryInfo infoCmd4(1,CRetAccessoryInfo::ACCESSORYFIRMWAREVERSION,info);
                            infoCmd4.marshall();
                            emit sendiAPPacket(infoCmd4.getCommandBytes());
                            break;
                        }
                        case 5:
                        {
                            SAccessoryInfo info;
                            //LOG_TRACE << "   5 for RetAccessoryInfo" << LOG_ENDL;
                            CRetAccessoryInfo infoCmd4(1,CRetAccessoryInfo::ACCESSORYHARDWAREVERSION,info);
                            infoCmd4.marshall();
                            emit sendiAPPacket(infoCmd4.getCommandBytes());
                            break;
                        }
                        case 6:
                        {
                            SAccessoryInfo info;
                            //LOG_TRACE << "   6 for RetAccessoryInfo" << LOG_ENDL;
                            info.m_StringToSend = "LGE"; // manufac
                            CRetAccessoryInfo infoCmd3(1,CRetAccessoryInfo::ACCESSORYMANUFACTURER,info);
                            infoCmd3.marshall();
                            emit sendiAPPacket(infoCmd3.getCommandBytes());
                            break;
                        }
                        case 7:
                        {
                            SAccessoryInfo info;
                            //LOG_TRACE << "   7 for RetAccessoryInfo" << LOG_ENDL;
                            info.m_StringToSend = "0.1"; // model no
                            CRetAccessoryInfo infoCmd4(1,CRetAccessoryInfo::ACCESSORYMODELNUMBER,info);
                            infoCmd4.marshall();
                            emit sendiAPPacket(infoCmd4.getCommandBytes());
                            if(!m_EmitIdentify)
                            {
                                int productID = (((CIpodController*) parent())->m_Writer)->getProductID();
                                //LOG_TRACE << "   productID = " << productID << LOG_ENDL;
                                //CR14645: remove the delay as the Kernel has fixed the issue
                                //if(productID != NANO_2G_PRODUCT_ID)
                                //{
                                //    QTimer::singleShot(10000,this,SLOT(identifyDeviceLingo ()));
                                //}
                                //else
                                //{
                                identifyDeviceLingo();
                                //}
                                m_EmitIdentify = true;
                            }
                            else
                            {
                                LOG_HIGH << "CIdentifierNonIDPSTaskHandler, emit identified(true)"  << LOG_ENDL;
                                emit identified(true);
                                m_EmitIdentify=false;
                            }
                            break;
                        }
                        default :
                            break;
                    }
                    break;
                }
                case 0x12:
                {
                    CIdentifierTaskHandler::packetAvailable(lingoID,commandID,buf);
                }
                    break;
            } //end of switch
        }
        else
        {
#ifdef DBUGLOGS
            //LOG_TRACE << "Not my lingo...ignoring!" << LOG_ENDL;
#endif
            return;
        }
#ifdef DBUGLOGS
        //LOG_TRACE << "CIdentifierNonIDPSTaskHandler::packetAvailable() OUT" << LOG_ENDL;
#endif
    }
    catch(EIPodErrors ex)
    {
        //LOG_HIGH << "CIdentifierNonIDPSTaskHandler::PacketAvailable(), Execption Caught *******"<< LOG_ENDL;
    }
    return;
}

