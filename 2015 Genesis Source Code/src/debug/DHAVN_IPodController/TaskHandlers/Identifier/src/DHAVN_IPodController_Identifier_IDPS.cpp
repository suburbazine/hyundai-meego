
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
#include <DHAVN_IPodController_Identifier_IDPS.h>
#include <DHAVN_IPodController_retiPodOptionsForLingo.h>
#include <DHAVN_IPodController_retFIDTokenValue.h>
#include <DHAVN_IPodController_idpsStatus.h>
#include <DHAVN_IPodController_endIDPS.h>
#include <DHAVN_IPodController_ack.h>
#include <QDebug>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_IAPPacketReader.h>
#include <DHAVN_IPodController_IAPPacketWriter.h>
#include <QTimer>
#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
#include <DHAVN_IPodController_enterRemoteUiMode.h>
#include <DHAVN_IPodController_eventloopTimer.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_DevAck.h>
#include <DHAVN_IPodController_OpenDataSession.h>
#include <DHAVN_IPodController_CloseDataSession.h>
#include <DHAVN_IPodController_requestSoftwareVersion.h>
#include <DHAVN_IPodController_Performance.h>
#include <DHAVN_IPodController_PerformanceLog.h>

#include <DHAVN_AppFramework_AppEngineBase.h>

using namespace GeneralLingo;

bool readSessionIDFromFile(QString filePath, int &nSessionID)
{
    QFile file(filePath);
    QString sessionIdstr;
    if(!file.open(QIODevice::ReadOnly))
    {
        nSessionID = -1;
        return false;
    }
    QTextStream in(&file);
    in >> sessionIdstr;
    file.close();
    nSessionID = sessionIdstr.toInt();
    return true;
}

CIdentifierIDPSTaskHandler::CIdentifierIDPSTaskHandler(QObject *parent)
    :CIdentifierTaskHandler(parent)
{
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

    //LOG_TRACE << "CIdentifierIDPSTaskHandler::CIdentifierIDPSTaskHandler() IN" << LOG_ENDL;
    //LOG_TRACE << "CIdentifierIDPSTaskHandler::CIdentifierIDPSTaskHandler() OUT" << LOG_ENDL;

    /* connect the task handler to iAP channel */
    //connect(this, SIGNAL(sendiAPPacket(QByteArray)) ,((CIpodController*) parent)->m_Writer, SLOT(sendiAPPacket(QByteArray)));
    //connect(((CIpodController*) parent)->m_Reader,SIGNAL(packetAvailable(int, int, QByteArray)),this, SLOT(packetAvailable(int,int,QByteArray)));
}

CIdentifierIDPSTaskHandler::~CIdentifierIDPSTaskHandler()
{
}


/******************************************************************************
// Function Name                                            Parameters
// CIdentifierIDPSTaskHandler::identify_restart_slot        (void) (OUT), (QBitArray, QList) (IN)
//
// Explanation
// Start the identification process
// History
// 18-Dec-2012      SK Paek                    Added initial implementation
*****************************************************************************/
// Added for identify retry(CR 31200)
void CIdentifierIDPSTaskHandler::identify_restart_slot()
{
    //LOG_TRACE << "CIdentifierIDPSTaskHandler::identify_restart_slot() IN" << LOG_ENDL;

    if(m_isDisconnected)    // if already disconnected
    {
        LOG_HIGH << "identify_restart_slot() Disconnected Exit" << LOG_ENDL;
        return;
    }

    m_MaxWritePacketSize = 0;
    m_EndIdpsState       = false;
    m_CommandRetry       = 0;

    // Resetting to initial value
    m_SupportedLingoes = m_SupportedLingoes_org;
    m_FidTokenValues   = m_FidTokenValues_org;

    LOG_HIGH << "idenfity_restart_slot(), resend_"  << LOG_ENDL;
    CStartIDPS cmd(m_iPodPrivateData->getTransactionId(true), m_iPodPrivateData->m_isIDPS );
    cmd.marshall();
    emit sendiAPPacket(cmd.getCommandBytes());

    // Set waiting status of CIAPCmds::L0_13_IdentifyDeviceLingoes Ack
    m_WaitingResponseOfCommand = CIAPCmds::L0_38_StartIDPS;     // added for IDPS retry(CR 13200)

    // No need waitForResponceFromExitCode() because identify retry timer is working.
    //LOG_TRACE << "CIdentifierIDPSTaskHandler::identify_restart_slot() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIdentifierIDPSTaskHandler::identify                         (void) (OUT), (QBitArray, QList) (IN)
//
// Explanation
// Start the identification process
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIdentifierIDPSTaskHandler::identify(QBitArray supportedLingoes, QList<FIDTokenValue> fidTokenValues)
{
    //LOG_TRACE << "CIdentifierIDPSTaskHandler::identify() IN" << LOG_ENDL;
    m_SupportedLingoes = supportedLingoes;
    m_FidTokenValues   = fidTokenValues;

    m_SupportedLingoes_org = supportedLingoes;      // saving initial value for identify retry(CR 13200)
    m_FidTokenValues_org   = fidTokenValues;        //

    ((CIpodController*)parent())->m_IsIDPS = true;
    m_iPodPrivateData->m_isIDPS = true;
    // StartIDPS is already done so start with the next command
    GetIPODOptionsForGeneralLingo();

    //LOG_TRACE << "CIdentifierIDPSTaskHandler::identify() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIdentifierIDPSTaskHandler::GetIPODOptionsForGeneralLingo    (void) (OUT), (void) (IN)
//
// Explanation
// send the GetIPODOptionsForGeneralLingo command
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIdentifierIDPSTaskHandler::GetIPODOptionsForGeneralLingo()
{
    //LOG_TRACE << "CIdentifierIDPSTaskHandler::GetIPODOptionsForGeneralLingo() IN" << LOG_ENDL;
    CGetIPODOptionsForLingo cmd(m_iPodPrivateData->getTransactionId(), GeneralLing, m_iPodPrivateData->m_isIDPS );
    cmd.marshall();
    emit sendiAPPacket(cmd.getCommandBytes());
    //LOG_TRACE << "CIdentifierIDPSTaskHandler::GetIPODOptionsForGeneralLingo() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIdentifierIDPSTaskHandler::GetIPODOptionsForStorageLingo    (void) (OUT), (void) (IN)
//
// Explanation
// send the GetIPODOptionsForStorageLingo command
// History
// 09-Jan-2012      Sowmini Philip                        Added initial implementation
*****************************************************************************/
void CIdentifierIDPSTaskHandler::GetIPODOptionsForStorageLingo()
{
    //LOG_TRACE << "CIdentifierIDPSTaskHandler::GetIPODOptionsForStorageLingo() IN" << LOG_ENDL;
    CGetIPODOptionsForLingo cmd(m_iPodPrivateData->getTransactionId(), Storage, m_iPodPrivateData->m_isIDPS );
    cmd.marshall();
    emit sendiAPPacket(cmd.getCommandBytes());
    //LOG_TRACE << "CIdentifierIDPSTaskHandler::GetIPODOptionsForStorageLingo() OUT" << LOG_ENDL;
}
/******************************************************************************
// Function Name                                            Parameters
// CIdentifierIDPSTaskHandler::SetFIDTokenValues               (void) (OUT), (void) (IN)
//
// Explanation
// send the SetFIDTokenValues command
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIdentifierIDPSTaskHandler::SetFIDTokenValues(bool isInterleaved, bool bUsVariant)
{
    bool bUsVariantIOSSupport = m_iPodPrivateData->m_IsiOSAppCommunicationSupported && bUsVariant;
    //LOG_TRACE << "CIdentifierIDPSTaskHandler::SetFIDTokenValues() IN, isInterleaved = " << isInterleaved << LOG_ENDL;
    if(bUsVariantIOSSupport)
    {
        //LOG_TRACE << "   adding the bundleseed token and EAProtocol token as device support iOS app Cmmunication" << LOG_ENDL;
        m_FidTokenValues.append(FIDTokenValue(gEAProtocolToken));
        m_FidTokenValues.append(FIDTokenValue(gBundleSeedIDPrefToken));
    }
    CSetFIDTokenValue cmd(m_iPodPrivateData->getTransactionId(), m_FidTokenValues, m_SupportedLingoes,
                          m_SupportedDeviceOptions, m_iPodPrivateData->m_isIDPS, bUsVariantIOSSupport, isInterleaved );
    cmd.marshall();
    emit sendiAPPacket(cmd.getCommandBytes());
    //LOG_TRACE << "CIdentifierIDPSTaskHandler::SetFIDTokenValues() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIdentifierIDPSTaskHandler::EndIDPS                         (void) (OUT), (void) (IN)
//
// Explanation
// send the EndIDPS command
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIdentifierIDPSTaskHandler::EndIDPS()
{
    //LOG_TRACE << "CIdentifierIDPSTaskHandler::EndIDPS() IN" << LOG_ENDL;
    CEndIDPS cmd(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
    cmd.marshall();
    emit sendiAPPacket(cmd.getCommandBytes());
    //LOG_TRACE << "CIdentifierIDPSTaskHandler::EndIDPS() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIdentifierIDPSTaskHandler::packetAvailable                   (void) (OUT), (int, int, QByteArray) (IN)
//
// Explanation
// Slot to handle the command which is read by the raw packet handler
// check for the last command status and on the basis of the status
// takes next action
// handle the whole flow of the IDPS process
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIdentifierIDPSTaskHandler::packetAvailable(int lingoID,int commandID,const QByteArray& buf )
{
#ifdef DBUGLOGS
    //LOG_TRACE << "CIdentifierIDPSTaskHandler::packetAvailable() Entry,Cmd="<<commandID<<"Lingo:"<<lingoID << LOG_ENDL;
#endif
    try
    {    // added by sungha.choi 2013.04.16
        // if identify process is retrying(CR 13200)
        if(m_WaitingResponseOfCommand == CIAPCmds::L0_38_StartIDPS)
        {
            //LOG_TRACE << "   m_WaitingResponseOfCommand, L0_38_StartIDPS" << LOG_ENDL;
            // waiting Ack for CIAPCmds::L0_38_StartIDPS
            if(lingoID == 0x00) // for general Lingo
            {
                if(commandID == CIAPCmds::L0_02_iPodAck)
                {
                    //LOG_TRACE << "   L0_02_iPodAck" << LOG_ENDL;
                    CACK tmp_cmd( m_iPodPrivateData->m_isIDPS );
                    tmp_cmd.unmarshall(buf);
                    int tmp_ackCmd= tmp_cmd.getAcknowledgedCommand();
                    if(tmp_ackCmd == CIAPCmds::L0_38_StartIDPS)
                    {
                        int ackStatus = tmp_cmd.getAckStatus();     // sungha.choi added 2013.03.14
                        //LOG_TRACE << "   L0_38_StartIDPS, ackStatus = " << ackStatus << LOG_ENDL;
                        if( ackStatus == 0x00 ) // added for identify retry(CR 13200)
                        {
                            m_WaitingResponseOfCommand = -1;      // Clear waiting status of CIAPCmds::L0_38_StartIDPS Ack
                            // StartIDPS is done so start with the next command
                            GetIPODOptionsForGeneralLingo();
                        }
                        else if(ackStatus == 0x04)  // sungha.choi modified 2013.03.14 ; bad parameter, will send IdentifyDeviceLingo
                        {
                            // Even if Ack with unexpected parameter, no need to retry here because identify retry timer is working.
                        }
                        else    // { sungha.choi added 2013.03.14 ; any other nonzero ; reference (MFi accessory Firmware spec. 98 page)
                        {
                            // No need to reconnection, because identify retry timer is working.
//                            // { added by sungha.choi 2013.03.05 to reset UCS
//                            if(m_iPodPrivateData->m_currentPortNum == eIPOD_PORT_1 || m_iPodPrivateData->m_currentPortNum == eIPOD_PORT_2)
//                            {    // modified by sungha.choi 2013.03.29
//                                LOG_HIGH << "CIdentifierIDPSTaskHandler::packetAvailable(), emit reconnection()"  << LOG_ENDL;
//                                emit reconnection();
//                                return;
//                            }
                            // } added by sungha.choi 2013.03.05 to reset UCS
                        }       // } sungha.choi added 2013.03.14 ; any other nonzero
                    }
                }
            }

            //LOG_TRACE << "   lingoID = " << lingoID << ", command ID = " << commandID << " is ignored until Ack for CIAPCmds::L0_38_StartIDPS" << LOG_ENDL;
            return; // ignore all commands/response to prohibit unexpected working until Ack for CIAPCmds::L0_38_StartIDPS
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
                    //LOG_TRACE << "   /////////////its an Acknoledge command return/////////// " << LOG_ENDL;
                    //LOG_INFO << "      ACK command ID = "  << ackCmd << LOG_ENDL;
                    //LOG_INFO << "      ACK command status = " << cmd.getAckStatus() << LOG_ENDL;
                    switch(ackCmd)
                    {
                        case 0x39: //SetFIDTokenValues
                        {
                            if(cmd.getAckStatus()) //unsuccessful SetFIDTokenValues command
                            {
                                EndIDPS();
                                //LOG_TRACE << "   unsuccessful SetFIDTokenValues command Exiting" << LOG_ENDL;
                            }
                        }
                            break;
                        case 0x3B: //endIDPS
                        {
                            ////LOG_TRACE << "emitting signalfrom endIDPS with error" << LOG_ENDL;
                            //  emit identified(true);
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                case 0x4C: //RetiPodOptionsForLingo
                {
                    //LOG_TRACE << "   ack of RetiPodOptionsForLingo" << LOG_ENDL;
                    CRetiPodOptionsForLingo cmd(m_iPodPrivateData->getTransactionId(false), m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);
                    int dLingo = cmd.getLingoType();
                    //LOG_INFO << "   RetiPodOptionsForLingo for " << dLingo << LOG_ENDL;
                    switch(dLingo)
                    {
                        case 0:
                        {
                            m_iPodPrivateData->m_IsiOSAppCommunicationSupported = cmd.isiOSAppCommunicationSupported();
                            GetIPODOptionsForStorageLingo();
                            break;
                        }
                            //0x0C = 12
                        case 12:
                        {
                            m_SupportedLingoes.setBit(0x0C);
                            ECountryVariant countryVariant = AppEngineBase::GetCountryVariant();
                            bool isUSorCanada = (countryVariant == eCVNorthAmerica) ||(countryVariant == eCVCanada);
                            SetFIDTokenValues(true, isUSorCanada);
                            break;
                        }
                        case 4:
                        {
                            //LOG_TRACE << "   RetiPodOptionsForLingo::Extended Interface 04 " << LOG_ENDL;
                            if(cmd.isLogoSupported())
                            {
                                //LOG_TRACE << "   RetiPodOptionsForLingo::Extended Interface 04 m_isLogoSupported " << LOG_ENDL;
                                ((CIpodController*)parent())->m_isLogoSupported = true;
                            }
                            break;
                        }
                        default :
                            break;
                    }
                    //GetNextLingo(cmd.getLingoType());
                    break;
                }
                case 0x3A: //RetFIDTokenValueACKs
                {
                    //LOG_TRACE << "   ack of RetFIDTokenValueACKs" << LOG_ENDL;
                    CRetFIDTokenValue cmd(m_iPodPrivateData->getTransactionId(false),  m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);
                    if(cmd.m_AccCapsStatus)
                    {
                        //LOG_TRACE << "   acccaps: success" << LOG_ENDL;
                        RequestTransPortMaxPayLoadSize();
                        EndIDPS();
                    }
                    else
                    {
                        //LOG_TRACE << "   acccaps: retrying" << LOG_ENDL;
                        ECountryVariant countryVariant = AppEngineBase::GetCountryVariant();
                        bool isUSorCanada = (countryVariant == eCVNorthAmerica) ||(countryVariant == eCVCanada);
                        SetFIDTokenValues(false, isUSorCanada);
                    }
                }
                    break;
                case 0x3C: //IDPSStatus
                {
                    //LOG_TRACE << "   Reset and got GetAccessoryInfo" << LOG_ENDL;
                    CIdpsStatus cmd(m_iPodPrivateData->getTransactionId(false),  m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);
                    if( cmd.getIdpsStatus() == 0 )
                    {
                        CRequestSoftwareVersion deviceVersionCmd(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
                        deviceVersionCmd.marshall();
                        emit sendiAPPacket(deviceVersionCmd.getCommandBytes());
                        //LOG_TRACE << "///////////////////////// IDPS process is finished SUCCESS /////////////////////" << LOG_ENDL;
                        LOG_HIGH << "CIdentifierIDPSTaskHandler, emit identified(true)"  << LOG_ENDL;
                        emit identified(true);
                        m_EndIdpsState = true;
                    }
                    else
                    {
                        //LOG_TRACE << "///////////////////////// IDPS process is finished FAILED  /////////////////////" << LOG_ENDL;
                        LOG_HIGH << "CIdentifierIDPSTaskHandler, emit identified(false)"  << LOG_ENDL;
                        emit identified(false);
                    }
                }
                    break;
                case 0x12:
                {
                    CIdentifierTaskHandler::packetAvailable(lingoID,commandID,buf);
                }
                    break;
                case 0x3f:
                {
                    //LOG_TRACE << "   at packet available for 0x3f"<<LOG_ENDL;
                    COpenDataSessionProtocol cmd1(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
                    cmd1.unmarshall(buf);

                    CDevAck cmd(cmd1.getTransactionId(), commandID,  m_iPodPrivateData->m_isIDPS);
                    cmd.marshall();
                    emit sendiAPPacket(cmd.getCommandBytes());
                    //LOG_TRACE << "   command is sent for devack" << LOG_ENDL ;

                    int protocolindex = 0;
                    qint16 sessionId  = 0;
                    cmd1.getProtocolIndexAndSessionId(sessionId, protocolindex);

                    if(gPandoraProtocolIndex == protocolindex)
                    {
                        //LOG_TRACE <<  "   its pandora thing" <<LOG_ENDL;
                        //get session id
                        qint16 sessionId=0;
                        for(int i = 6 ; i <= 7 ; i++)
                        {
                            sessionId <<= 8;
                            sessionId |= (int)buf[i] & 0xFF;
                        }
#ifdef DBUSBASEDAPPHANDLER
                        //Append to the list
                        AppSessionId id;
                        //LOG_TRACE <<  "   DBUSBASEDAPPHANDLER sessionId = " << sessionId <<LOG_ENDL;
                        id.m_AppId.append(gIPodPandoraAppID);
                        id.sessionId = sessionId;

                        if(!m_AppSessionIdList.contains(id))
                        {
                            m_AppSessionIdList.append(id);
                            //LOG_TRACE << "   DBUSBASEDAPPHANDLER m_AppId = " << id.m_AppId << ", session Id = " << id.sessionId << LOG_ENDL;
                            if(!id.m_AppId.compare(gIPodPandoraAppID) )
                            {
                                //LOG_TRACE <<  "   DBUSBASEDAPPHANDLER entered if loop " << LOG_ENDL;
                                m_iPodPrivateData->setAppSessionId( id.m_AppId, id.sessionId );
                            }
                        }

                        //LOG_TRACE << "   emit AppSessionOpend(gIpodPandoraAppID, sessionId = " << sessionId << ")"  << LOG_ENDL;
                        //emit
                        emit AppSessionOpened(gIPodPandoraAppID, sessionId);
#endif
#ifdef FILEBASEDAPPHANDLER
                        QString sessionIdstr;
                        sessionIdstr.setNum(sessionId);
                        //LOG_TRACE << "   m_IPodPandoraAppSessionID " << m_IPodPandoraAppSessionID << LOG_ENDL;
                        QFile devFile(m_IPodPandoraAppSessionID);
                        bool ret = devFile.open(QIODevice::WriteOnly);
                        if(ret)
                        {
                            //LOG_TRACE << "   m_IPodPandoraAppSessionID open file is ret = "<< ret << LOG_ENDL;
                            QTextStream stream(&devFile);
                            stream << sessionIdstr <<endl<<flush;
                            devFile.close();
                        }
#endif
                    }
                    else if(gAhaProtocolIndex == protocolindex)
                    {
                        //get session id
                        qint16 sessionId=0;
                        for(int i = 6;i<= 7;i++)
                        {
                            sessionId <<= 8;
                            sessionId |= (int)buf[i] & 0xFF;
                        }

#ifdef DBUSBASEDAPPHANDLER
                        //Append to the list
                        AppSessionId id;
                        id.m_AppId.append(gIPodAhaAppID);
                        id.m_AppId = sessionId;
                        if(!m_AppSessionIdList.contains(id))
                            m_AppSessionIdList.append(id);

                        //emit
                        //LOG_TRACE << "   emit AppSessionOpend(gIpodAhaAppID, sessionId = " << sessionId << ")"  << LOG_ENDL;
                        emit AppSessionOpened(gIPodAhaAppID, sessionId);
#endif

#ifdef FILEBASEDAPPHANDLER
                        QString sessionIdstr;
                        sessionIdstr.setNum(sessionId);
                        //LOG_TRACE << "   m_IPodAhaAppSessionID = " << m_IPodAhaAppSessionID << LOG_ENDL;
                        QFile devFile(m_IPodAhaAppSessionID);
                        bool ret = devFile.open(QIODevice::WriteOnly);
                        if(ret)
                        {
                            //LOG_TRACE << "   m_IPodAhaAppSessionID open file is ret = "<< ret << LOG_ENDL;
                            QTextStream stream(&devFile);
                            stream << sessionIdstr <<endl<<flush;
                            devFile.close();
                        }
#endif
                    }
                    else
                    {
                        //LOG_TRACE << "   its not pandora or aha thing" << LOG_ENDL;
                    }
                }
                    break;
                case 0x40:
                {
                    qint16 sessionId=0;
                    CCloseDataSessionProtocol cmd2(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
                    cmd2.unmarshall(buf);
                    cmd2.getSessionId(sessionId);
#ifdef DBUSBASEDAPPHANDLER
                    //remove session id from list
                    for( int i=0; i<m_AppSessionIdList.count() ; i++ )
                    {
                        if(m_AppSessionIdList[i].sessionId == sessionId)
                        {
                            m_AppSessionIdList.removeAt(i);
                            break;
                        }
                    }
#endif
#ifdef FILEBASEDAPPHANDLER
                    //QFile::remove(m_IPodPandoraAppSessionID);
                    //QFile::remove(m_IPodAhaAppSessionID);
                    //TODO: has to be handled for the multi application based handling
                    // { modified by Tan 2013.01.05
                    //1. Open the file
                    //2. Get the station id
                    //3. Close the file
                    //3. Compare the session id from file and from 0x40
                    //4. If match, remove the file

                    //QFile::remove(m_IPodPandoraAppSessionID);
                    int nPandoraSessionID;
                    if(readSessionIDFromFile(m_IPodPandoraAppSessionID, nPandoraSessionID))
                    {
                        if(nPandoraSessionID == sessionId)
                        {
                            QFile::remove(m_IPodPandoraAppSessionID);
                        }
                    }
                    // } modified by Tan
                    //#ifdef DHAVN_COMPILE_FOR_AHA // added by jungae, 2012.12.01 for AhaRadio application
                    // { modified by Tan 2013.01.05
                    int nAhaSessionID;
                    if(readSessionIDFromFile(m_IPodAhaAppSessionID, nAhaSessionID))
                    {
                        if(nAhaSessionID == sessionId)
                        {
                            QFile::remove(m_IPodAhaAppSessionID);
                        }
                    }
                    // } modified by Tan
                    //#endif
#endif

                    CDevAck cmd4(m_iPodPrivateData->getTransactionId(), commandID, m_iPodPrivateData->m_isIDPS);
                    cmd4.marshall();
                    emit sendiAPPacket(cmd4.getCommandBytes());
                    //LOG_TRACE << "   command is sent for devack " << LOG_ENDL;

#ifdef DBUSBASEDAPPHANDLER
                    //LOG_TRACE << "   emit AppSessionClosed(sessionId = " << sessionId << ")"  << LOG_ENDL;
                    emit AppSessionClosed(sessionId);
#endif
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
        //LOG_TRACE << "CIdentifierIDPSTaskHandler::packetAvailable() Exit " << LOG_ENDL;
#endif
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIdentifierIDPSTaskHandler::packetAvailable(), Execption Caught *******"<< LOG_ENDL;
    }
    return;
}
