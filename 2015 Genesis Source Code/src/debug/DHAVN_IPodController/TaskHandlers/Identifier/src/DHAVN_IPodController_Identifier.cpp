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
#include <DHAVN_IPodController_Identifier.h>
#include <DHAVN_IPodController_ack.h>
#include <QDebug>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_IAPPacketReader.h>
#include <DHAVN_IPodController_IAPPacketWriter.h>
#include <QTimer>
#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
#include <DHAVN_IPodController_enterRemoteUiMode.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_Performance.h>
#include <DHAVN_IPodController_returnTransportMaxPayloadSize.h>
#include <DHAVN_IPodController_PerformanceLog.h>

using namespace GeneralLingo;

CIdentifierTaskHandler::CIdentifierTaskHandler(QObject *parent)
    :CTaskHandler(parent)
{
    m_iPodPrivateData   = parent->findChild<iPodControllerPrivateData*>(gControllerPrivateData);
    EIPodPorts portNum  = m_iPodPrivateData->m_currentPortNum ;

    m_IPodPandoraAppSessionID = gIPodPandoraAppSessionID + DOT + QString::number(portNum);
    m_IPodAhaAppSessionID     = gIPodAhaAppSessionID + DOT + QString::number(portNum);

    switch( portNum )
    {
        case eIPOD_PORT_1:
            LOG_INIT_V2(giPodControllerLogName_1);
            break;
        case eIPOD_PORT_2:
            LOG_INIT_V2(giPodControllerLogName_2);
            break;
        default:
            LOG_INIT_V2(giPodControllerLogName);
            m_IPodPandoraAppSessionID = gIPodPandoraAppSessionID;
            m_IPodAhaAppSessionID     = gIPodAhaAppSessionID;
            break;
    }

    //LOG_TRACE << "CIdentifierTaskHandler::CIdentifierTaskHandler() IN" << LOG_ENDL;
    //LOG_TRACE << "   m_IPodPandoraAppSessionID  = " << m_IPodPandoraAppSessionID << LOG_ENDL;
    //LOG_TRACE << "   m_IPodAhaAppSessionID      = " << m_IPodAhaAppSessionID << LOG_ENDL;

    setObjectName(gIdentifier);

    m_MaxWritePacketSize       = 0;
    m_EndIdpsState             = false;
    m_isDisconnected           = false;
    m_isIDPS                   = false;
    m_WaitingResponseOfCommand = -1;      // added for IDPS retry(CR 13200)
    m_CommandRetry             = 0;       // CR 13200
    m_ReadyToDelete            = true;    // added by sungha.choi 2013.04.12
    m_pLoop                    = NULL;
    m_pWaitTimer               = NULL;
    m_isRunningLoop            = false;   // added by sungha.choi 2013.04.13

    /* connect the task handler to iAP channel */
    qRegisterMetaType<QByteArray>("QByteArray");
    connect(this, SIGNAL(sendiAPPacket(QByteArray)),
            ((CIpodController*) parent)->m_Writer, SLOT(sendiAPPacket(QByteArray)),
            Qt::QueuedConnection);
    connect(((CIpodController*) parent)->m_Reader, SIGNAL(packetAvailable(int, int, QByteArray)),
            this, SLOT(packetAvailable(int,int,QByteArray)),
            Qt::QueuedConnection);
    //LOG_TRACE << "CIdentifierTaskHandler::CIdentifierTaskHandler() OUT" << LOG_ENDL;
}

CIdentifierTaskHandler::~CIdentifierTaskHandler()
{
    //LOG_TRACE << "CIdentifierTaskHandler::~CIdentifierTaskHandler() IN" << LOG_ENDL;
    if(m_pLoop)    // added by sungha.choi 2013.04.12
    {
        delete m_pLoop;
        m_pLoop = NULL;
    }

    if(m_pWaitTimer)
    {
        delete m_pWaitTimer;
        m_pWaitTimer = NULL;
    }

    //LOG_TRACE << "CIdentifierTaskHandler::~CIdentifierTaskHandler() OUT" << LOG_ENDL;
}

void CIdentifierTaskHandler::stopCurrentTask()
{
    m_isDisconnected = true;
    //emit mainLoopQuit();    // added by sungha.choi 2013.04.02

    if(isWaitingForResponse() == true)
    {
        m_ReadyToDelete = false;
        m_pLoop->quit();
    }
}

/* Stops the entair taskhandler. This will disconnect the taskhandler form iAP channel*
 * This can be called if the task handler is not required to operate.
 * This will improve the performance.
*/
void CIdentifierTaskHandler::stopTaskHandler()
{
    //LOG_TRACE << "CIdentifierTaskHandler::stopTaskHandler() IN" << LOG_ENDL;

    m_rtimer.stop();          // CR 13200, StartIDPS retry timer stop, 121228
    disconnect(&m_rtimer, SIGNAL(timeout()), this, SLOT(StartIDPS_timeout_slot()));

    disconnect(((CIpodController*) parent())->m_Reader,SIGNAL(packetAvailable(int, int, QByteArray)),this, SLOT(packetAvailable(int,int,QByteArray)));
    disconnect(this, SIGNAL(sendiAPPacket(CIAPPacket)) ,((CIpodController*) parent())->m_Writer, SLOT(SendPacket(CIAPPacket)) );
    //LOG_TRACE << "CIdentifierTaskHandler::stopTaskHandler() OUT" << LOG_ENDL;
}

bool CIdentifierTaskHandler::isReadyToDelete()  // added by sungha.choi 2013.04.12
{
    return m_ReadyToDelete;
}

bool CIdentifierTaskHandler::isWaitingForResponse()    // added by sungha.choi 2013.04.12
{
    return m_isRunningLoop;
}

/******************************************************************************
// Function Name                                            Parameters
// CIdentifierTaskHandler::identify                         (void) (OUT), (QBitArray, QList) (IN)
//
// Explanation
// Start the identification process
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIdentifierTaskHandler::identify(QBitArray /*supportedLingoes*/, QList<FIDTokenValue> /*fidTokenValues*/)
{
    //LOG_TRACE << "CIdentifierTaskHandler::identify() IN" << LOG_ENDL;
    //LOG_TRACE << "CIdentifierTaskHandler::identify() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIdentifierTaskHandler::StartIDPS                         (void) (OUT), (void) (IN)
//
// Explanation
// send the start IDPS process command
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIdentifierTaskHandler::StartIDPS()
{
    PT_START_TIME(PT_OBJ_IDENTIFICATION, 0);

    //LOG_TRACE << "CIdentifierTaskHandler::StartIDPS() IN" << LOG_ENDL;

    int milliseconds = m_iPodPrivateData->m_CurrentTime.elapsed();
    QString performanceTime = m_iPodPrivateData->getPerformanceTime( milliseconds );
    PERFORMANCELOG ( "IPod Device Detection time is %s " , performanceTime.toAscii().data()  );
    m_iPodPrivateData->m_CurrentTime.restart();


    /* added m_rtimer processing by SK 2012. 12. 28. for identify retry(CR 13200) */
    m_CommandRetry = 0;
    m_rtimer.setInterval(5000);     // iPod IDPS process retry timeout set to 1000ms
    m_rtimer.setSingleShot(true);
    connect(&m_rtimer, SIGNAL(timeout()), this, SLOT(StartIDPS_timeout_slot()));
    m_rtimer.start();

    CStartIDPS cmd(m_iPodPrivateData->getTransactionId(false), m_iPodPrivateData->m_isIDPS );
    cmd.marshall();
    emit sendiAPPacket(cmd.getCommandBytes());

    //LOG_TRACE << "CIdentifierTaskHandler::StartIDPS() OUT" << LOG_ENDL;
}

void CIdentifierTaskHandler::StartIDPS_timeout_slot()
{
    //LOG_TRACE << "CIdentifierTaskHandler::StartIDPS_timeout_slot() IN" << LOG_ENDL;
    if(m_CommandRetry < 5)    // 1sec * 120 = 120sec    // modified by sungha.choi 2013.03.29 after 5 second resetUcs
    {
        m_CommandRetry++;
        m_rtimer.stop();
        m_rtimer.setInterval(5000);     // iPod IDPS process retry timeout set tp 1000ms
        m_rtimer.setSingleShot(true);
        m_rtimer.start();
        CStartIDPS cmd(m_iPodPrivateData->getTransactionId(m_iPodPrivateData->m_isIDPS), m_iPodPrivateData->m_isIDPS );    // modified by sungha.choi 2013.03.07 for most recent StartIDPS is only valid -> 2013.03.15 iPod nano No trans ID
        cmd.marshall();
        emit sendiAPPacket(cmd.getCommandBytes());
    }
    // { added by sungha.choi 2013.03.29
    else
    {
        m_CommandRetry = 0;
        m_rtimer.stop();
        if(m_iPodPrivateData->m_currentPortNum == eIPOD_PORT_1 || m_iPodPrivateData->m_currentPortNum == eIPOD_PORT_2)
        { // modified by sungha.choi 2013.03.29
            m_iPodPrivateData->m_restartUcsFlag = true;
            m_iPodPrivateData->m_portresetFlag = true;
            LOG_HIGH << "StartIDPS_timeout_slot(), emit reconnection()"  << LOG_ENDL;
            emit reconnection();
        }
    }
    // } added by sungha.choi 2013.03.29
    //LOG_TRACE << "CIdentifierTaskHandler::StartIDPS_timeout_slot() OUT" << LOG_ENDL;
}

int CIdentifierTaskHandler::RequestTransPortMaxPayLoadSize()
{
    int res = -1;
    qint16 transId = m_iPodPrivateData->getTransactionId();
    int i = -1;
    QByteArray command;
    command[++i] = 0x55;
    command[++i] = 0x04;
    command[++i] = 0x00;
    command[++i] = 0x11;

    if(m_iPodPrivateData->m_isIDPS == true)
    {
        command[++i] = (transId & 0xff00) >> 8;
        command[++i] =  transId & 0x00ff;
    }

    command[1] = i - 1;
    command[i+1] = CIpodControllerGlobal::checkSum(command, i);
    emit sendiAPPacket(command);

    return res;
}

/******************************************************************************
// Function Name                                            Parameters
// CIdentifierTaskHandler::packetAvailable                   (void) (OUT), (int, int, QByteArray) (IN)
//
// Explanation
// Slot to handle the command which is read by the raw packet handler
// check for the last command status and on the basis of the status
// takes next action
// handle the whole flow of the IDPS process
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIdentifierTaskHandler::packetAvailable(int lingoID,int commandID,const QByteArray& buf )
{
#ifdef DBUGLOGS
    //LOG_TRACE << "CIdentifierTaskHandler::packetAvailable() Entry,Cmd="<<commandID<<"Lingo:"<<lingoID << LOG_ENDL;
#endif
    try
    {
        if(lingoID == 0x00) // for general Lingo
        {
            switch(commandID)
            {
#ifdef REQIDENTIFY // Added by jonghwan.cho@2013.06.19.
                case CIAPCmds::L0_00_RequestIdentify :
                {
                    //LOG_HIGH << "[WARNING] IDTH H/U Receviced General Lingo Command Request Identify ( 0x00 )!!!!!!!" << LOG_ENDL;
                    //LOG_HIGH << "          So, we will do usb port resetting" << LOG_ENDL;
                    CRequestIdentify recvCmd(m_iPodPrivateData->getTransactionId(false),m_iPodPrivateData->m_isIDPS);
                    recvCmd.unmarshall(buf);

                    if( m_iPodPrivateData->m_currentPortNum == eIPOD_PORT_1 || m_iPodPrivateData->m_currentPortNum == eIPOD_PORT_2 )
                    {
                        m_iPodPrivateData->m_restartUcsFlag = true;
                        m_iPodPrivateData->m_portresetFlag = true;
                        LOG_HIGH << "L0_00_RequestIdentify, emit reconnection()"  << LOG_ENDL;
                        emit reconnection();
                        return;
                    }
                }
                    break;
#endif
                case 0x02: //ACK = 0x02
                {
                    CACK cmd( m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);
                    int ackCmd = cmd.getAcknowledgedCommand();
                    //LOG_TRACE << "   ACK command ID = "  << ackCmd << LOG_ENDL;
                    //LOG_TRACE << "   ACK command status = " << cmd.getAckStatus() << LOG_ENDL;

                    switch(ackCmd)
                    {
                        case 0x38: //start IDPS
                        {
                            if(m_iPodPrivateData->m_isIDPS) // when using transaction ID
                            {
                                int transID = cmd.getTransactionID();
                                if( transID == 0 ) // Apple device not Support transaction ID
                                {
                                    ((CIpodController*)parent())->m_IsIDPS = false;
                                    m_iPodPrivateData->m_isIDPS = false;
                                }
                                else if(m_iPodPrivateData->getTransactionId(false) > transID) // if most recent send trans ID bigger than recvd trans ID of iPodAck
                                {
                                    //LOG_TRACE << "   Most recent send transaction ID = " << m_iPodPrivateData->getTransactionId(false) << ", Transacation ID of Received ack" << cmd.getTransactionID() << LOG_ENDL;
                                    // this ack should be ignored ; (MFi accessory Firmware spec. 2.3.2.2 in 96 page)
                                    break;    // modified by sungha.choi 130312 : iPod nano don't increase transaction ID
                                }
                            }
                            m_rtimer.stop();                // CR 13200 start idps timer stop, 121228

                            int ackStatus = cmd.getAckStatus();
                            if(ackStatus == 0x00) //successful IDPS command go ahead
                            {
                                m_isIDPS = true;
                                ((CIpodController*)parent())->m_IsIDPS = true;
                                m_iPodPrivateData->m_isIDPS = true;
                                //emit mainLoopQuit();
                                LOG_HIGH << "CIdentifierTaskHandler, emit identificationType(true)"  << LOG_ENDL;
                                emit identificationType(true);
                            }
                            else if(ackStatus == 0x04)  // bad parameter, will send IdentifyDeviceLingo
                            {
                                //LOG_TRACE << "   Error in ack of the start IDPS Command Exiting and emiting signal" << LOG_ENDL;
                                ((CIpodController*)parent())->m_IsIDPS = false;
                                m_iPodPrivateData->m_isIDPS = false;
                                //emit mainLoopQuit();
                                LOG_HIGH << "CIdentifierTaskHandler, emit identificationType(false)"  << LOG_ENDL;
                                emit identificationType(false);
                            }
                            else    //any other nonzero ; reference (MFi accessory Firmware spec. 98 page)
                            {
                                if(m_iPodPrivateData->m_currentPortNum == eIPOD_PORT_1 || m_iPodPrivateData->m_currentPortNum == eIPOD_PORT_2)
                                {    // modified by sungha.choi 2013.03.29
                                    m_iPodPrivateData->m_restartUcsFlag = true;
                                    m_iPodPrivateData->m_portresetFlag = true;
                                    LOG_HIGH << "CIdentifierTaskHandler, emit reconnection()"  << LOG_ENDL;
                                    emit reconnection();
                                    return;
                                }
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
                    CReturnTransportMaxPayloadSize cmd( m_iPodPrivateData->getTransactionId(false),m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);
                    quint32 size = cmd.getMaxPayloadSize();
                    m_MaxWritePacketSize = size;
                    m_iPodPrivateData->m_MaxPayload = size;
                    //LOG_TRACE << "   payloadsize is = " << size << LOG_ENDL;
                }
                    break;
            } //end of switch
        }
        else
        {
#ifdef DBUGLOGS
            //LOG_TRACE << "   Not my lingo...ignoring!" << LOG_ENDL;
#endif
            return;
        }
#ifdef DBUGLOGS
        //LOG_TRACE << "CIdentifierTaskHandler::packetAvailable() OUT" << LOG_ENDL;
#endif
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIdentifierTaskHandler::PacketAvailable Execption Caught *******"<< LOG_ENDL;
    }
    return;
}
