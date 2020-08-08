/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  21 Sept 2011                                     *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                   This is the class which will help control    *
 *                   to put the device to enter into the remote   *
 *                   UI mode                                      *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT                  *
 ******************************************************************
 * 21 Feb 2012  Neelima Kasam	    Code change for CR5862 (Added *
 *  				    IPodController support for    *
 *				    1.x.x firmware version)       *
 ******************************************************************
 */
#include <DHAVN_IPodController_Def.h>
#include <DHAVN_IPodController_remoteUiModeTaskHandler.h>
#include <DHAVN_IPodController_ack.h>
#include <DHAVN_IPodController_enterRemoteUiMode.h>
#include <qtimer.h>

#include <DHAVN_IPodController_ackDevAuthenticationStatus.h>
#include <DHAVN_IPodController_logoImageTransmitter.h>
#include <DHAVN_IPodController_eventloopTimer.h>
#include <DHAVN_IPodController_iPodDBFetchTaskHandler.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
#include <DHAVN_IPodController_authorisor.h>
#include <DHAVN_IPodController_Identifier.h>
#include <DHAVN_IPodController_returnSoftwareVersion.h>
#include <DHAVN_IPodController_retiPodOptionsForLingo.h>
#include <DHAVN_IPodController_Performance.h>
#include <DHAVN_IPodController_SetEventNotification.h>
#include <DHAVN_IPodController_GetSupportedEventNotification.h>
#include <DHAVN_IPodController_setUiMode.h>
#include <DHAVN_IPodController_returniPodName.h> // added by jungae 2012.08.16 to get the iPod device name
#include <DHAVN_IPodController_requestiPodName.h> // added by jungae 2012.08.17 to get the iPod device name
#include <DHAVN_IPodController_getIPODOptionsForLingo.h>
//#include <DHAVN_IPodController_ucsControl.h>    // added by sungha.choi 2013.03.05 to reset UCS > deleted at 2013.07.07


#define SOFTWARE_VERSION "1.0.2"

CRemoteUIModeTaskHandler::CRemoteUIModeTaskHandler(QObject *parent)
    : CTaskHandler(parent), m_remoteUIState(eExtStateIdle)
{
    m_iPodPrivateData = parent->findChild<iPodControllerPrivateData*>(gControllerPrivateData);
    switch( m_iPodPrivateData->m_currentPortNum )
    {
        case eIPOD_PORT_1:
            LOG_INIT_V2(giPodControllerLogName_1);
            break;
        case eIPOD_PORT_2:
            LOG_INIT_V2(giPodControllerLogName_2);
            break;
        default:
            LOG_INIT_V2(giPodControllerLogName);
            break;
    }

    //LOG_TRACE << "CRemoteUIModeTaskHandler::CRemoteUIModeTaskHandler() IN" << LOG_ENDL;
    setObjectName(gEnterUiModeTaskHandler);

    m_productID         = 0;
    m_LocalTransID      = 0;
    m_isUiModeRequested = false;

    /* Logo relate variables */
    m_logoWidth         = 0;
    m_logoHeight        = 0;
    m_logoFormat        = 0;
    m_logoIndex         = 0;
    m_countOf0A02       = 0;
    m_logoTransmitter   = NULL;
    m_softwareVersion   = "";
    m_ipodName          = "";

    /* connect the task handler to iAP channel */
    qRegisterMetaType<QByteArray>("QByteArray");
    connect(((CIpodController*) parent)->m_Reader, SIGNAL(packetAvailable(int, int, QByteArray)),
            this, SLOT(packetAvailable(int,int,QByteArray)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(sendiAPPacket(QByteArray)),
            ((CIpodController*) parent)->m_Writer, SLOT(sendiAPPacket(QByteArray)),
            Qt::QueuedConnection);

    m_logoTransmitter = new CLogoImageTrasmitter(m_iPodPrivateData, m_iPodPrivateData->m_isIDPS, m_logoWidth, m_logoHeight, m_logoFormat); // added by jungae, 2012.12.04 for ISV63632

    m_AlreadyRequested   = false;
    m_bSupportGetIPodOptionsForLingo = true;
    m_IdentifyRetryCount = 0;

    m_timer.setInterval( IDENTIFY_RETRY_TIMEOUT );     // iPod IDPS process retry timeout set
    m_timer.setSingleShot( true );
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(identify_timeout_slot()));
    m_timer.start();

    //LOG_TRACE << "CRemoteUIModeTaskHandler::CRemoteUIModeTaskHandler() OUT" << LOG_ENDL;
}

CRemoteUIModeTaskHandler::~CRemoteUIModeTaskHandler()
{
    //LOG_TRACE << "CRemoteUIModeTaskHandler::~CRemoteUIModeTaskHandler() IN" << LOG_ENDL;

    if(m_logoTransmitter != NULL)
    {
	    delete m_logoTransmitter;
	    m_logoTransmitter = NULL;
    }
    m_remoteUIState = eExtStateIdle;

    //LOG_TRACE << "CRemoteUIModeTaskHandler::~CRemoteUIModeTaskHandler() OUT" << LOG_ENDL;
}

/* Stops the current task */
void CRemoteUIModeTaskHandler::stopCurrentTask()
{
    //TODO
}


/* Stops the entair taskhandler. This will disconnect the taskhandler form iAP channel*
 * This can be called if the task handler is not required to operate.
 * This will improve the performance.
*/
void CRemoteUIModeTaskHandler::stopTaskHandler()
{
    disconnect(((CIpodController*) parent())->m_Reader, SIGNAL(packetAvailable(int, int, QByteArray)), this,                                    SLOT(packetAvailable(int,int,QByteArray)));
    disconnect(this,                                    SIGNAL(sendiAPPacket(QByteArray)) ,            ((CIpodController*) parent())->m_Writer, SLOT(sendiAPPacket(QByteArray)) );

    m_timer.stop();
    disconnect(&m_timer, SIGNAL(timeout()), this, SLOT(identify_timeout_slot()));
}

/* added by SK 2012. 12. 18. for identify retry(CR 31200) */
void CRemoteUIModeTaskHandler::identify_timeout_slot()
{
    //LOG_TRACE << "CRemoteUIModeTaskHandler::identify_timeout_slot() IN" << LOG_ENDL;

    if(m_IdentifyRetryCount >= 2)   //IDENTIFY_RETRY_MAX_COUNT) // modified by sungha.choi 2013.04.15 : 3 sec x 3 try.
    {
        m_timer.stop();
        if( m_iPodPrivateData->m_currentPortNum == eIPOD_PORT_1 || m_iPodPrivateData->m_currentPortNum == eIPOD_PORT_2 )
        { // modified by sungha.choi 2013.03.29
            m_iPodPrivateData->m_restartUcsFlag = true;
            m_iPodPrivateData->m_portresetFlag = true;
            LOG_HIGH << "CRemoteUIModeTaskHandler::identify_timeout_slot(), emit reconnection()"  << LOG_ENDL;
            emit reconnection();
        }
        return;
    }

    m_productID         = 0;
    m_isUiModeRequested = false;
    m_LocalTransID      = 0;

    /* Logo relate variables */
    m_logoWidth         = 0;
    m_logoHeight        = 0;
    m_logoFormat        = 0;
    m_logoIndex         = 0;
    m_countOf0A02       = 0;
    m_softwareVersion   = "";

    m_ipodName = "";

    m_AlreadyRequested  = false;

    m_bSupportGetIPodOptionsForLingo = true;

    m_remoteUIState     = eExtStateIdle;

    m_IdentifyRetryCount++;
    m_timer.stop();
    m_timer.setInterval(IDENTIFY_RETRY_TIMEOUT);     // iPod IDPS process retry timeout set
    m_timer.setSingleShot(true);
    m_timer.start();

    LOG_HIGH << "CRemoteUIModeTaskHandler::identify_timeout_slot(), emit identify_restart_signal()"  << LOG_ENDL;
    emit identify_restart_signal();

    //LOG_TRACE << "CRemoteUIModeTaskHandler::identify_timeout_slot() OUT" << LOG_ENDL;
}

void CRemoteUIModeTaskHandler::authorisationFinished(bool status)
{
    //LOG_TRACE << "CRemoteUIModeTaskHandler::authorisationFinished() IN, status = " << status << LOG_ENDL;

    if( !status )
    {
        return;
    }

    //LOG_HIGH << "authorisationFinished(), m_remoteUIState = " << m_remoteUIState << LOG_ENDL;
    switch(m_remoteUIState)
    {
        case eExtStateIdle:
            m_remoteUIState = eExtStateAuthorised;
            break;
        case eExtStateIdentified:
        {
            m_remoteUIState = eExtStateInitComplete;
            m_timer.stop();

            //Always try to GetIPodOptionsForLingo. if it gets non 0 ack,
            //then we can send request lingo protocol version.
            //LOG_TRACE << "   send CGetIPODOptionsForLingo" << LOG_ENDL;
            CGetIPODOptionsForLingo cmd(m_iPodPrivateData->getTransactionId(), ExtendedInterface,m_iPodPrivateData->m_isIDPS);
            cmd.marshall();
            emit sendiAPPacket(cmd.getCommandBytes());

            // For Non-IDPS devices "GetIPODOptionsForLingo" is not supported, if we send this command, it returns Bad parameter. Hence we should not send this command.
            /*
            if(m_iPodPrivateData->m_isIDPS)
            {
                //LOG_TRACE << "   send CGetIPODOptionsForLingo" << LOG_ENDL;
                CGetIPODOptionsForLingo cmd(m_iPodPrivateData->getTransactionId(), ExtendedInterface,m_iPodPrivateData->m_isIDPS);
                cmd.marshall();
                emit sendiAPPacket(cmd.getCommandBytes());
            }
            else
            {
                //LOG_TRACE << "   send CRequestLingoProtocolVersion" << LOG_ENDL;
                CRequestLingoProtocolVersion cmd(CIAPCmds::ExtenededLingoId,m_iPodPrivateData->getTransactionId(),m_iPodPrivateData->m_isIDPS);
                cmd.marshall();
                emit sendiAPPacket(cmd.getCommandBytes());
            }
            */
            break;
        }
        case eExtStateAuthorised:
        case eExtStateInitComplete:
        case eExtStateExtMode:
        default:
            break;
    }
    //LOG_TRACE << "CRemoteUIModeTaskHandler::authorisationFinished() OUT" << LOG_ENDL;
}

void CRemoteUIModeTaskHandler::IdentificationProcessFinished(bool status)
{
    //LOG_TRACE << "CRemoteUIModeTaskHandler::IdentificationProcessFinished() IN, status = " << status << LOG_ENDL;

    if(!status)
    {
        return;
    }

    //LOG_HIGH << "CRemoteUIModeTaskHandler::IdentificationProcessFinished(), m_remoteUIState = " << m_remoteUIState << LOG_ENDL;
    switch(m_remoteUIState)
    {
        case eExtStateIdle:
            m_remoteUIState = eExtStateIdentified;
            break;
        case eExtStateAuthorised:
        {
            m_remoteUIState = eExtStateInitComplete;

            m_timer.stop();

            //Always try to GetIPodOptionsForLingo. if it gets non 0 ack,
            //then we can send request lingo protocol version.

            //LOG_TRACE << "   send CGetIPODOptionsForLingo" << LOG_ENDL;
            CGetIPODOptionsForLingo cmd(m_iPodPrivateData->getTransactionId(), ExtendedInterface,m_iPodPrivateData->m_isIDPS);
            cmd.marshall();
            emit sendiAPPacket(cmd.getCommandBytes());



            // For Non-IDPS devices "GetIPODOptionsForLingo" is not supported, if we send this command, it returns Bad parameter. Hence we should not send this command.
            /*
            if(m_iPodPrivateData->m_isIDPS)
            {
                //LOG_TRACE << "   send CGetIPODOptionsForLingo" << LOG_ENDL;
                CGetIPODOptionsForLingo cmd(m_iPodPrivateData->getTransactionId(), ExtendedInterface,m_iPodPrivateData->m_isIDPS);
                cmd.marshall();
                emit sendiAPPacket(cmd.getCommandBytes());
            }
            else
            {
                //LOG_TRACE << "   send CRequestLingoProtocolVersion" << LOG_ENDL;
                CRequestLingoProtocolVersion cmd(CIAPCmds::ExtenededLingoId,m_iPodPrivateData->getTransactionId(),m_iPodPrivateData->m_isIDPS);
                cmd.marshall();
                emit sendiAPPacket(cmd.getCommandBytes());
            }
            */
            break;
        }
        case eExtStateIdentified:
        case eExtStateInitComplete:
        case eExtStateExtMode:
        default:
            break;
    }
    //LOG_TRACE << "CRemoteUIModeTaskHandler::IdentificationProcessFinished() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CRemoteUIModeTaskHandler::enterRemoteUiMode               (void) (OUT), (void) (IN)
//
// Explanation
// Send the command for the enterRemoteUiMode
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CRemoteUIModeTaskHandler::enterRemoteUiMode()
{
    PT_START_TIME(PT_OBJ_ENTER_REMOTEUI_MODE, 0);
    //LOG_TRACE << "CRemoteUIModeTaskHandler::enterRemoteUiMode() IN" << LOG_ENDL;

    if(m_AlreadyRequested)
        return;

    m_AlreadyRequested = true;
    //LOG_TRACE << "m_DisplayRemoteLingoVersion calling the command for it" << LOG_ENDL;

    //Tan, there are two cases:
    //a. GetIPodOptionsForLingo succeeded.
    //   Cannot send RequestLingoProtocolVersion. We need to assume that displayRemoteLingoVersion is 1.02
    //b. GetIPodOptionsForLingo failed.
    //   Send RequestLingoProtocolVersion to get the actual displayremotelingoversion.
    if(m_bSupportGetIPodOptionsForLingo == false)
    {
        CRequestLingoProtocolVersion cmd(CIAPCmds::DisplayRemote,m_iPodPrivateData->getTransactionId(),m_iPodPrivateData->m_isIDPS);
        cmd.marshall();
        emit sendiAPPacket(cmd.getCommandBytes());
    }
    else
    {
        m_iPodPrivateData->m_DisplayRemoteLingoVersion = 1.02;
    }


    CGetSupportedEventNotification notificationCmd(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
    notificationCmd.marshall();
    emit sendiAPPacket(notificationCmd.getCommandBytes());

    CRequestiPodName deviceNameCmd(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
    deviceNameCmd.marshall();
    emit sendiAPPacket(deviceNameCmd.getCommandBytes());

    //LOG_TRACE << "CRemoteUIModeTaskHandler::enterRemoteUiMode() OUT" << LOG_ENDL;
}

void CRemoteUIModeTaskHandler::SetIPODOptionsForExtendedLingo()
{
//    try
//    {
//        //double dVersion = (parent()->findChild<CITunesDBTaskHandler*>(giTunesTaskHandler))->lingoProtocolVersion(4);
//        //LOG_INFO << "CIpodController Extended Lingo Version is " << dVersion << LOG_ENDL;
//        if(dVersion >= 1.13)
//        {
//            ((CIpodController*)parent())->m_isLogoSupported = true;
//            //LOG_TRACE << "CRemoteUIModeTaskHandler::SetIPODOptionsForExtendedLingo Logo is supported" << LOG_ENDL;
//        }
//        else
//        {
//            ((CIpodController*)parent())->m_isLogoSupported = false;
//            //LOG_TRACE << "CRemoteUIModeTaskHandler::SetIPODOptionsForExtendedLingo Logo is not supported" << LOG_ENDL;
//        }
//    }
//    catch(EIPodErrors ex)
//    {
//        //LOG_TRACE << "CRemoteUIModeTaskHandler::SetIPODOptionsForExtendedLingo exception caught!!!" << LOG_ENDL;
//    }
}

/******************************************************************************
// Function Name                                            Parameters
// CRemoteUIModeTaskHandler::packetAvailable                   (void) (OUT), (int, int, QByteArray) (IN)
//
// Explanation
// Slot to handle the command which is read by the raw packet handler
// check for the last command status and on the basis of the status
// takes next action
// handle the whole flow of the process
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CRemoteUIModeTaskHandler::packetAvailable(int lingoID,int commandID,const QByteArray& buf )
{
#ifdef DBUGLOGS
    //LOG_TRACE << "CRemoteUIModeTaskHandler::packetAvailable Entry " << LOG_ENDL;
#endif
    try
    {
        if(lingoID == 0x00) // for general Lingo
        {
            switch(commandID)
            {
                case 0x02:
                {
                    CACK cmd( m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);
                    int ackCmd= cmd.getAcknowledgedCommand();

                    switch(ackCmd)
                    {
                        case 0x4B:
                        {
                            //LOG_TRACE << "    case 0x4B" << LOG_ENDL;
                            if(cmd.getAckStatus() != 0x00)
                            {
                                m_bSupportGetIPodOptionsForLingo = false;
                                //LOG_TRACE << "    AckStatus 0x04" << LOG_ENDL;
                                CRequestLingoProtocolVersion cmd(CIAPCmds::ExtenededLingoId,m_iPodPrivateData->getTransactionId(),m_iPodPrivateData->m_isIDPS);
                                cmd.marshall();
                                emit sendiAPPacket(cmd.getCommandBytes());
                            }
                            break;
                        }
                        case 0x05: //enteruimode
                        {
                            //LOG_TRACE << "    case 0x05, enteruimode ack has come" << LOG_ENDL;
                            if(cmd.getAckStatus() == 0x06)
                            {
                                //LOG_TRACE << "    AckStatus 0x06" << LOG_ENDL;
                                qint32 waitinMilliSec=0;
                                qint32 temp1 = 0; temp1 = temp1 |  buf[11];
                                qint32 temp2 = 0; temp2 = temp2 | (buf[10] << 8);
                                qint32 temp3 = 0; temp3 = temp3 | (buf[9] << 16);
                                qint32 temp4 = 0; temp4 = temp4 | (buf[8] << 24);

                                waitinMilliSec = temp1 | temp2 | temp3 | temp4;

                                QTimer *timer = new QTimer(this);
                                connect(timer, SIGNAL(timeout()), this, SLOT(EnterUiModeTimeOut()));
                                timer->start(waitinMilliSec);

                                //LOG_TRACE << "    wait in milisec = " << waitinMilliSec <<LOG_ENDL;
                            }
                            else if(cmd.getAckStatus() == 0x00)
                            {
                                //LOG_TRACE << "    AckStatus 0x00" << LOG_ENDL;
                                executeSuccessfulPostUiModeOperation();
                            }
                            else
                            {
                                //LOG_HIGH << "CRemoteUIModeTaskHandler::packetAvailable(), REMOTE UI PROCESS FINISHED WITH STATUS As FAILED STATUS = " << cmd.getAckStatus() << LOG_ENDL;
                                LOG_HIGH << "CRemoteUIModeTaskHandler::packetAvailable(), emit EnteredInUiMode(false)"  << LOG_ENDL;
                                emit EnteredInUiMode(false);
                            }
                        }
                            break;
                        case 0x37:
                        {
                            //LOG_TRACE << "   case 0x37" << LOG_ENDL;
                            if(cmd.getAckStatus() == 0x00)
                            {
                                //LOG_TRACE << "    AckStatus 0x00" << LOG_ENDL;
                                executeSuccessfulPostUiModeOperation();
                            }
                            else if(cmd.getAckStatus() == 0x04)
                            {
                                //LOG_TRACE << "    AckStatus 0x04" << LOG_ENDL;
                                CEnterRemoteUiMode cmd( m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
                                cmd.marshall();
                                emit sendiAPPacket(cmd.getCommandBytes());
                            }
                            else
                            {
                                //LOG_HIGH << "CRemoteUIModeTaskHandler::packetAvailable(), REMOTE UI PROCESS FINISHED WITH STATUS As FAILED STATUS = " << cmd.getAckStatus() << LOG_ENDL;
                                LOG_HIGH << "CRemoteUIModeTaskHandler::packetAvailable(), emit EnteredInUiMode(false)"  << LOG_ENDL;
                                emit EnteredInUiMode(false);
                            }
                        }
                            break;
                        case 0x49:
                        {
                            //LOG_TRACE << "   case 0x49" << LOG_ENDL;
                            if(!m_isUiModeRequested)
                            {
                                m_isUiModeRequested = true;

                                /* For CR 15396, SetInterface(1, 1) should be sent by iPod controller before SetUIMode */
                                ((CIpodController*)parent())->m_Writer->UsbAudioClassSetInterface(1,1);

                                //SetIPODOptionsForExtendedLingo(); // Find out from the device if logo is supported and set m_isLogoSupported on controller
                                CSetUiMode cmd(m_iPodPrivateData->getTransactionId(),  m_iPodPrivateData->m_isIDPS);
                                cmd.marshall();
                                emit sendiAPPacket(cmd.getCommandBytes());
                            }
                        }
                            break;
                        case 0x4F:
                        {
                            //LOG_TRACE << "   case 0x4F" << LOG_ENDL;
                            bool IsDBChangeNotificationSupported = false;
                            CSetEventNotification setNotificationCmd(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS, IsDBChangeNotificationSupported,false,false);
                            setNotificationCmd.marshall();
                            emit sendiAPPacket(setNotificationCmd.getCommandBytes());
                        }
                            break;
                        }
                    break;
                }
                case 0x51:
                {
                    bool IsDBChangeNotificationSupported = false;
                    bool IsNowPlayingApplicationBundleNameSupported = false;
                    bool IsNowPlayingApplicationDisplayNameStatus = false;

                    if(m_iPodPrivateData->m_isIDPS)
                    {
                        if(buf[12] & 0x02)
                        {
                            IsDBChangeNotificationSupported = true;
                        }

                        if(buf[12] & 0x04)
                        {
                            IsNowPlayingApplicationBundleNameSupported = true;
                        }
                        if(buf[11] & 0x08)
                        {
                            IsNowPlayingApplicationDisplayNameStatus = true;
                        }
                    }
                    else
                    {
                        if(buf[10] & 0x02)
                        {
                            IsDBChangeNotificationSupported = true;
                        }

                        if(buf[10] & 0x04)
                        {
                            IsNowPlayingApplicationBundleNameSupported = true;
                        }

                        if(buf[11] & 0x08)
                        {
                            IsNowPlayingApplicationDisplayNameStatus = true;
                        }
                    }

                    CSetEventNotification setNotificationCmd(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS, IsDBChangeNotificationSupported,IsNowPlayingApplicationBundleNameSupported,IsNowPlayingApplicationDisplayNameStatus );
                    setNotificationCmd.marshall();
                    emit sendiAPPacket(setNotificationCmd.getCommandBytes());
                }
                    break;
                case 0x4C: //RetiPodOptionsForLingo
                {
                    //LOG_TRACE << "   case 0x4C, RetiPodOptionsForLingo" << LOG_ENDL;
                    CRetiPodOptionsForLingo cmd(m_iPodPrivateData->getTransactionId(false),m_iPodPrivateData->m_isIDPS);
                    cmd.unmarshall(buf);
                    int dLingo = cmd.getLingoType();
                    //LOG_TRACE << "   RetiPodOptionsForLingo, dLingo = " << dLingo << LOG_ENDL;
                    switch(dLingo)
                    {
                        case 4:
                        {
                            //LOG_TRACE << "   Extended Interface 4C : 04 " << LOG_ENDL;
                            if(cmd.isLogoSupported())
                            {
                                ((CIpodController*)parent())->m_isLogoSupported = true;
                                //LOG_TRACE << "   Logo is supported"<<LOG_ENDL;
                            }
                            else
                            {
                                ((CIpodController*)parent())->m_isLogoSupported = false;
                                //LOG_TRACE << "   Logo is not supported"<<LOG_ENDL;
                            }
                            m_iPodPrivateData->m_isLogoSupported = ((CIpodController*)parent())->m_isLogoSupported;
                            m_iPodPrivateData->m_ExtendedLingoVersion = cmd.getExtendedLingoVersion();
                            if(eExtStateInitComplete == m_remoteUIState)
                            {
                                /*  Controller should send the EnterRemoteUiMode command after RetAccSampleRateCaps As per MFi spec *
                                 *  But for some devices, GetAccSampleRateCaps from ipod is sent after entering in to extended mode.*
                                 *  So wait for 5 sec and send EnterRemoteUiMode if we dont receive GetAccSampleRateCaps            *
                                 */
                                enterRemoteUiMode();
                            }
                            break;
                        }
                    }
                    break;
                }
                case 0x04:
                {
                    //LOG_TRACE << "   case 0x04, ReturnRemoteUIMode" << LOG_ENDL;
                    if( buf[4] != 0x00 )
                    {
                        //LOG_HIGH << "CRemoteUIModeTaskHandler::packetAvailable(), REMOTE UI PROCESS FINISHED WITH STATUS As SUCCESS" << LOG_ENDL;
                        LOG_HIGH << "CRemoteUIModeTaskHandler, emit EnteredInUiMode(true)"  << LOG_ENDL;
                        emit EnteredInUiMode(true);
                        if(eExtStateInitComplete == m_remoteUIState)
                        {
                            m_remoteUIState = eExtStateExtMode;
                        }
                    }
                    break;
                }
                case 0x0A:
                {
                    CReturnSoftwareVersion cmd( m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);
                    m_softwareVersion = QString::number(cmd.returnMajorSoftwareVersion()) + "." + QString::number(cmd.returnMinorSoftwareVersion())+ "." +QString::number(cmd.returnDeviceRevisionSoftwareVersion());
                    //LOG_TRACE << "   case 0x0A, CReturnSoftwareVersion, m_softwareVersion = " << m_softwareVersion << LOG_ENDL;
                    m_iPodPrivateData->setiPodSoftwareVerion(cmd.returnMajorSoftwareVersion(), cmd.returnMinorSoftwareVersion(), cmd.returnDeviceRevisionSoftwareVersion(), m_softwareVersion);
                    break;
                }
                // we send device name to BT Controller through AVP.
                // because DHAVN set cannot play together ( same IPod and BT Music ).
                case 0x08:
                {
                    CReturniPodName cmd( m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);
                    QString sIPodName = cmd.getiPodName();
                    //LOG_TRACE << "   case 0x08, CReturniPodName, m_IPodName = " << sIPodName << LOG_ENDL;
                    LOG_HIGH  << "emit GetDeviceName(m_iPodName = " << sIPodName.toUtf8().data() << ")" << LOG_ENDL;
                    if( sIPodName.isEmpty() == false )
                    {
                        m_ipodName = sIPodName;
                        emit GetDeviceName(m_ipodName);
                    }
                    else
                    {
                        emit GetDeviceName("");
                    }
                    break;
                }
                case CIAPCmds::L0_10_ReturnLingoProtocolVersion:
                {
                    CReturnLingoProtocolVersion cmd( m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);
                    if(cmd.returnReqLingoId() == 0x04)
                    {
                        QString sVersion = QString::number(cmd.returnMajorProtocolVersion()) + "." + QString::number(cmd.returnMinorProtocolVersion());
                        //LOG_TRACE << "   case L0_10_ReturnLingoProtocolVersion, sVersion = " << sVersion << LOG_ENDL;

                        if(!sVersion.isEmpty())
                            m_iPodPrivateData->m_ExtendedLingoVersion = sVersion.toDouble();

                        if(m_iPodPrivateData->m_ExtendedLingoVersion >= (double)1.01)
                        {
                            ((CIpodController*)parent())->m_isLogoSupported = true;
                            //LOG_TRACE << "   Logo is supported" << LOG_ENDL;
                        }
                        else
                        {
                            ((CIpodController*)parent())->m_isLogoSupported = false;
                            //LOG_TRACE << "   Logo is not supported" << LOG_ENDL;
                        }
                        m_iPodPrivateData->m_isLogoSupported = ((CIpodController*)parent())->m_isLogoSupported;
                        if(eExtStateInitComplete == m_remoteUIState)
                        {
                            /*
                             * Controller should send the EnterRemoteUiMode command after RetAccSampleRateCaps As per MFi spec *
                             * But for some devices, GetAccSampleRateCaps from ipod is sent after entering in to extended mode.*
                             * So wait for 5 sec and send EnterRemoteUiMode if we dont receive GetAccSampleRateCaps            *
                             */
                            enterRemoteUiMode();
                        }
                    }
                    else if(cmd.returnReqLingoId() == 0x03)
                    {
                        QString sVersion;
                        if(cmd.returnMinorProtocolVersion() > 9)
                            sVersion = QString::number(cmd.returnMajorProtocolVersion()) + "." + QString::number(cmd.returnMinorProtocolVersion());
                        else
                            sVersion = QString::number(cmd.returnMajorProtocolVersion()) + "." + "0" + QString::number(cmd.returnMinorProtocolVersion());
                        //LOG_TRACE << "   case L0_10_ReturnLingoProtocolVersion, sVersion = " << sVersion << LOG_ENDL;

                        if(!sVersion.isEmpty())
                            m_iPodPrivateData->m_DisplayRemoteLingoVersion = sVersion.toDouble();
                        //LOG_TRACE << "   m_iPodPrivateData->m_DisplayRemoteLingoVersion : " << m_iPodPrivateData->m_DisplayRemoteLingoVersion << LOG_ENDL;
                    }
                    break;
                }
                default :
                    break;
            } // switch()
        } // General Lingo
        else if(lingoID == 0x04) //Extended Lingo
        {
            switch(commandID)
            {
                case 0x01 :
                {
                    CACK cmd( m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);
                    int ackCmd= cmd.getAcknowledgedCommand();

                    switch(ackCmd)
                    {
                        case 0x32: //SetDisplayImage.
                        {
                            //LOG_TRACE << "   case 0x032, SetDisplayImage" << LOG_ENDL;
                            int status = cmd.getAckStatus();

                            if( status == 0x00 )
                            {
                                int ret = -1;
                                //LOG_TRACE << "    cmd.getAckStatus() = " << status << ", m_logoIndex = " << m_logoIndex << LOG_ENDL;
                                if( m_logoTransmitter != NULL )
                                {
                                    QByteArray logoCmd = m_logoTransmitter->TransmittLogo(++m_logoIndex,m_iPodPrivateData->getTransactionId(),m_logoFormat);
                                    if( logoCmd.size() >0 )
                                    {
                                        emit sendiAPPacket(logoCmd);
                                    }
                                    else
                                    {
                                        //LOG_INFO << "CRemoteUIModeTaskHandler:: Logo transmition stoped with ret="<< ret;
                                        stopTaskHandler();
                                    }
                                }
                            }
                            else
                            {
                                //LOG_TRACE << "    cmd.getAckStatus() = " << status << ", m_logoIndex = " << m_logoIndex << LOG_ENDL;
                                stopTaskHandler();
                            }
                            break;
                        }
                        case 0x39: //GetMonoDisplayImageLimits
                        {
                            //LOG_TRACE << "   case 0x039, GetMonoDisplayImageLimits" << LOG_ENDL;
                            int status = cmd.getAckStatus();

                            if(status == 0x04)
                            {
                                //LOG_TRACE << "    cmd.getAckStatus() = " << status << ", Send GetMonoDisplayImageLimits" << LOG_ENDL;
                                CGetMonoDisplayImageLimits cmd(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
                                cmd.marshall();
                                emit sendiAPPacket(cmd.getCommandBytes());
                            }
                            break;
                        }
                        default :
                            break;
                    } //switch(ackCmd)
                    break;
                }
                case 0x3A://RetColorDisplayImageLimits
                {
                    //LOG_TRACE << "   case 0x3A, RetColorDisplayImageLimits" << LOG_ENDL;

                    int  ret = -1;
                    CRetColorDisplayImageLimits cmd(m_iPodPrivateData->getTransactionId(false), m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);

                    m_logoFormat = cmd.getImageFormat();
                    m_logoHeight = cmd.getImageHeight();
                    m_logoWidth  = cmd.getImageWidth();
                    if(m_logoTransmitter != NULL)
                    {
                        QByteArray logoCmd = m_logoTransmitter->TransmittLogo(m_logoIndex,m_iPodPrivateData->getTransactionId(), m_logoFormat);
                        if( logoCmd.size() > 0 )
                        {
                            emit sendiAPPacket(logoCmd);
                        }
                        else
                        {
                            //LOG_TRACE << "   Logo transmition stopped with ret = " << ret << LOG_ENDL;;
                            stopTaskHandler();
                        }
                    }
                    break;
                }
                case 0x34://RetMonoDisplayImageLimits
                {

                    //LOG_TRACE << "   case 0x34, RetMonoDisplayImageLimits" << LOG_ENDL;

                    int ret = -1;
                    CReturnMonoDisplayImageLimits cmd(m_iPodPrivateData->getTransactionId(false), m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);

                    m_logoFormat = cmd.getImageFormat();
                    m_logoHeight = cmd.getImageHeight();
                    m_logoWidth  = cmd.getImageWidth();

                    //LOG_TRACE << "   m_logoFormat = " << m_logoFormat << ", m_logoHeight = " << m_logoHeight << ", m_logoWidth = " << m_logoWidth << LOG_ENDL;

                    if(m_logoTransmitter != NULL)
                    {
                        QByteArray logoCmd = m_logoTransmitter->TransmittLogo(m_logoIndex,m_iPodPrivateData->getTransactionId(), m_logoFormat);
                        if( logoCmd.size() > 0 )
                        {
                            emit sendiAPPacket(logoCmd);
                        }
                        else
                        {
                            //LOG_TRACE << "   Logo transmition stopped with ret = " << ret << LOG_ENDL;
                            stopTaskHandler();
                        }
                    }
                    break;
                }
                default :
                    break;
            } // switch()
        }
        else if(lingoID == 0x0A)//Digital Lingo
        {
            if((commandID == 0x02))
            {
                //LOG_TRACE << "   Digital Lingo 0x0A, command ID 0x02, Sending CRetAccSampleRateCaps m_countOf0A02 = " << m_countOf0A02 <<LOG_ENDL;
                if( ++m_countOf0A02 == 1 )
                {
                    m_LocalTransID = 0;
                    if(m_iPodPrivateData->m_isIDPS)
                    {
                        int nStartIndex = 4;
                        int nEndIndex   = 5;

                        for( int i = nStartIndex ; i <= nEndIndex ; i++ )
                        {
                            m_LocalTransID <<= 8;     m_LocalTransID |= (int)buf[i] & 0xFF;
                        }
                    }
                    CRetAccSampleRateCaps cmd(m_LocalTransID, m_iPodPrivateData->m_isIDPS);
                    cmd.marshall();
                    emit sendiAPPacket(cmd.getCommandBytes());
                }
            }
        }
        else
        {
#ifdef DBUGLOGS
            //LOG_TRACE << "CRemoteUIModeTaskHandler:Not my lingo...ignoring!" <<LOG_ENDL;
#endif
            return;
        }
    }   // end try
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CRemoteUIModeTaskHandler::PacketAvailable(), Execption Caught *******"<< LOG_ENDL;
    }
#ifdef DBUGLOGS
        //LOG_TRACE << "CRemoteUIModeTaskHandler::packetAvailable() OUT" <<LOG_ENDL;
#endif
    return;
}
void CRemoteUIModeTaskHandler::EnterUiModeTimeOut()
{
    //LOG_TRACE << "CRemoteUIModeTaskHandler::EnterUiModeTimeOut() IN" <<LOG_ENDL;
    //LOG_TRACE << "CRemoteUIModeTaskHandler::EnterUiModeTimeOut() OUT" <<LOG_ENDL;
}

void CRemoteUIModeTaskHandler::executeSuccessfulPostUiModeOperation()
{
    //LOG_TRACE << "CRemoteUIModeTaskHandler::executeSuccessfulPostUiModeOperation() IN" << LOG_ENDL;
    //LOG_TRACE << "############################# REMOTE UI PROCESS FINISHED WITH STATUS As Passed #################" << LOG_ENDL;

    //LOG_HIGH  << "executeSuccessfulPostUiModeOperation(), emit EnteredInUiMode(true)"  << LOG_ENDL;
    emit EnteredInUiMode(true);

    PT_END_TIME(PT_OBJ_ENTER_REMOTEUI_MODE, 0);

    if( eExtStateInitComplete == m_remoteUIState )
    {
        m_remoteUIState = eExtStateExtMode;
    }

    /* Get the display image range for the iPod.*/
    if(((CIpodController*)parent())->m_isLogoSupported == true)
    {
        CGetColorDisplayImageLimits cmd(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
        cmd.marshall();

        emit sendiAPPacket(cmd.getCommandBytes());
        //LOG_TRACE << "   Sent GetColorDisplayImageLimits" << LOG_ENDL;
    }
    else
    {
        //LOG_TRACE << "   done with its task, Stoping" << LOG_ENDL;
        stopTaskHandler();
    }
    //LOG_TRACE << "CRemoteUIModeTaskHandler::executeSuccessfulPostUiModeOperation() OUT" << LOG_ENDL;
}

void CRemoteUIModeTaskHandler::setRemoteUIProductID( int productID )
{
    //LOG_TRACE << "CRemoteUIModeTaskHandler::setRemoteUIProductID() IN, productID = " << productID <<LOG_ENDL;
    m_productID = productID;
    //LOG_TRACE << "CRemoteUIModeTaskHandler::setRemoteUIProductID() OUT" << LOG_ENDL;
}
