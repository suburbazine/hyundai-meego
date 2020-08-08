/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                                                                *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <QString>

#include <DHAVN_IPodController_iOSAppTaskHandler.h>
#include <DHAVN_IPodController_ack.h>
#include <DHAVN_IPodController_eventloopTimer.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
#include <DHAVN_IPodController_iPodDBFetchTaskHandler.h>
#include <DHAVN_IPodController_AccAck.h>
#include <DHAVN_IPodController_IPodAck.h>
#include <DHAVN_IPodController_PerformanceLog.h>
#include <DHAVN_IPodController_retNowPlayingAppBundleName.h>
#include <DHAVN_IPodController_Identifier.h>
#include <DHAVN_IPodController_iPodNotification.h>
//#include <DHAVN_IPodController_UIDplayEngineTaskHandler.h>
#include <DHAVN_IPodController_iPodDBIndexFetchTaskHandler.h>
#include <DHAVN_IPodController_playEngineTaskHandler.h>

#include "DHAVN_EventLoop.h"
#include "DHAVN_EventLoopTimerExitCode.h"

using namespace GeneralLingo;

#define PNDR_FRAME_START  ((uint) 0x7E)
#define PNDR_FRAME_END    ((uint) 0x7C)

/******************************************************************************
// Function Name                                            Parameters
// CiOSAppTaskHandler::CiOSAppTaskHandler
//
// Explanation
//Constrcutor
// History
// 06-Dec-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
CiOSAppTaskHandler::CiOSAppTaskHandler(QObject* parent, iPodLogger *iLog)
    :CTaskHandler(parent)
{
    m_nRunningLoopFromSignal = 0;
    m_ReadyToDeleteFromSignal = true;

    m_iPodPrivateData = parent->findChild<iPodControllerPrivateData*>(gControllerPrivateData);
    switch(m_iPodPrivateData->m_currentPortNum)
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
    //LOG_TRACE << "CiOSAppTaskHandler::CiOSAppTaskHandler() IN" << LOG_ENDL;

    m_isDisconnected = false;
    m_CommandStatus  = false;
    m_ReadyToDelete  = true;
    m_isRunningLoop  = false;    // added by sungha.choi 2013.04.13

    m_pLoop          = NULL;    // added by sungha.choi 2013.04.12
    m_pWaitTimer     = NULL;

    piPodLog         = iLog;

    m_ApplicationBundleName1 = QString();


    qRegisterMetaType<QByteArray>("QByteArray");
    connect(((CIpodController*) parent)->m_Reader, SIGNAL(packetAvailable(int, int, QByteArray)),
            this, SLOT(packetAvailable(int,int,QByteArray)),
            Qt::QueuedConnection);
    connect(((CIpodController*) parent)->m_Reader, SIGNAL(packetAvailableForPlayerTaskHandler(int,int,QByteArray)),
            this, SLOT(packetAvailable(int,int,QByteArray)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(sendiAPPacket(QByteArray)),
            ((CIpodController*) parent)->m_Writer, SLOT(sendiAPPacket(QByteArray)),
            Qt::QueuedConnection);

    m_iPodDBIndexFetchTaskHandler = new CIPodDBIndexFetchTaskHandler(this,m_iPodPrivateData,
                                                                     ((CIpodController*)parent)->m_Writer,
                                                                     ((CIpodController*)parent)->m_Reader);
    setObjectName(gPandoraTaskHandler);

    m_bBasicControlStatusToBeSend = true;
    m_bCurrentBasicControlState = true;
    m_strBasicControlerAppName.clear();
    m_delayBasicControlEnableTimer = new QTimer(this);
    if(m_delayBasicControlEnableTimer)
    {
        m_delayBasicControlEnableTimer->setSingleShot(true);
        connect(m_delayBasicControlEnableTimer, SIGNAL(timeout()), this, SLOT(delayBasicControlEnableSlot()));
    }



    //LOG_TRACE << "CiOSAppTaskHandler::CiOSAppTaskHandler() OUT" << LOG_ENDL;
}

CiOSAppTaskHandler::~CiOSAppTaskHandler()
{
    //LOG_TRACE << "CiOSAppTaskHandler::~CiOSAppTaskHandler() IN" << LOG_ENDL;

    if(m_delayBasicControlEnableTimer)
    {
        m_delayBasicControlEnableTimer->stop();
        delete m_delayBasicControlEnableTimer;
        m_delayBasicControlEnableTimer = NULL;
    }

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

    if(m_iPodDBIndexFetchTaskHandler)
    {
        delete m_iPodDBIndexFetchTaskHandler;
        m_iPodDBIndexFetchTaskHandler = NULL;
    }
    //LOG_TRACE << "CiOSAppTaskHandler::~CiOSAppTaskHandler() OUT" << LOG_ENDL;
}

/* Stops the current task */
void CiOSAppTaskHandler::stopTaskHandler()
{
    //LOG_TRACE << "CiOSAppTaskHandler::stopTaskHandler() IN" << LOG_ENDL;

    if(m_iPodDBIndexFetchTaskHandler)
    {
        m_iPodDBIndexFetchTaskHandler->stopTaskHandler();
    }

    //LOG_TRACE << "CiOSAppTaskHandler::stopTaskHandler() OUT" << LOG_ENDL;
}


void CiOSAppTaskHandler::stopCurrentTask()
{
    //LOG_TRACE << "CiOSAppTaskHandler::stopCurrentTask() IN" << LOG_ENDL;

    m_isDisconnected = true;

    if(m_iPodDBIndexFetchTaskHandler)
    {
        m_iPodDBIndexFetchTaskHandler->stopCurrentTask();   // added by sungha.choi 2013.04.02
    }

    //emit mainLoopQuit();   // modified by sungha.choi 2013.04.12
    piPodLog->WriteLog("Here 1-1 m_pLoop->quit() before CiOSAppTaskHandler");
    if(isWaitingForResponse() == true)
    {
        m_ReadyToDelete = false;
        m_pLoop->quit();
        piPodLog->WriteLog("Here 1-2 m_pLoop->quit() after CiOSAppTaskHandler");
    }

    if(isWaitingForResponseFromSignal() > 0)  // added by sungha.choi 2013.04.26
    {
        m_ReadyToDeleteFromSignal = false;
        //emit getDBTrackInfoReturned();  // If signals will be added, emit the signal here.
        emit mainLoopQuit(ExitCode_AllExit);  // If signals will be added, emit the signal here.
    }

    //LOG_TRACE << "CiOSAppTaskHandler::stopCurrentTask() OUT" << LOG_ENDL;
}

bool CiOSAppTaskHandler::isReadyToDelete()  // added by sungha.choi 2013.04.12
{
    //LOG_TRACE << "CiOSAppTaskHandler::isReadyToDelete() IN" << LOG_ENDL;
    //LOG_TRACE << "   m_ReadyToDelete = " << m_ReadyToDelete << ", m_ReadyToDeleteFromSignal = " << m_ReadyToDeleteFromSignal << LOG_ENDL;

    //LOG_TRACE << "CiOSAppTaskHandler::isReadyToDelete() OUT" << LOG_ENDL;
    if( m_iPodDBIndexFetchTaskHandler->isReadyToDelete() == true )
        return (m_ReadyToDelete && m_ReadyToDeleteFromSignal);
    else
        return false;
}

bool CiOSAppTaskHandler::isWaitingForResponse()    // added by sungha.choi 2013.04.12
{
    return m_isRunningLoop;
}

int CiOSAppTaskHandler::isWaitingForResponseFromSignal()    // added by sungha.choi 2013.04.26
{
    return m_nRunningLoopFromSignal;
}

int CiOSAppTaskHandler::waitForResponceFromExitCode(int timeOut, int nExitCode)
{
    ////LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode() IN" << LOG_ENDL;
    int ret = 0;

    if(m_isDisconnected)
    {
        LOG_HIGH << "IOSAPP, iPod Already disconnect" << LOG_ENDL;
        throw  IPOD_ERROR_DISCONNECT;
    }

    CIpodEventLoop localLoop(nExitCode);
    connect(this,SIGNAL(mainLoopQuit(int)),&localLoop,SLOT(mainLoopQuitSlot(int)));

    CEventloopTimerExitCode localWaitTimer;
    localWaitTimer.start(&localLoop,timeOut);

    m_nRunningLoopFromSignal ++;   // added by sungha.choi 2013.04.26
    ret = localLoop.exec();
    m_nRunningLoopFromSignal --;   // added by sungha.choi 2013.04.26

    if(m_nRunningLoopFromSignal == 0 && m_ReadyToDeleteFromSignal == false)  // added by sungha.choi 2013.04.26
    {
        m_ReadyToDeleteFromSignal = true;
    }

    localWaitTimer.stop();

    if((ret == IPOD_ERROR_DISCONNECT) || m_isDisconnected)
    {
        //LOG_TRACE << "CiOSAppTaskHandler::waitForResponceFromExitCode(), Loop exit with Error on disconnect" << LOG_ENDL;
        //You are right, thanks... This should not be unlock here.
        throw  IPOD_ERROR_DISCONNECT;
    }
    //LOG_TRACE << "CiOSAppTaskHandler::waitForResponceFromExitCode() OUT" << LOG_ENDL;
    return ret;//Return IPOD_SUCCESS or IPOD_ERROR_TIMEOUT
}


void CiOSAppTaskHandler::SetIscurrentPlayingMPFlag(QString bundleName)
{
    LOG_HIGH <<"SetIscurrentPlayingMPFlag bundleName = '" << bundleName<<"'"<<LOG_ENDL;

    if( bundleName.contains(IPOD_MOBILEAPP_APP_ID, Qt::CaseInsensitive) ||
        bundleName.contains(IPOD_MUSICAPP_APP_ID, Qt::CaseInsensitive))
    {
        //getplayengine instance and stop the play ipodfiles timer.
        CPlayEngineTaskHandler* PlayEngineHandler = parent()->findChild<CPlayEngineTaskHandler*>(gPlayEngineTaskHandler);
        if(PlayEngineHandler)
        {
            PlayEngineHandler->stopPlayIpodFilesTimer();
        }
    }

    if( bundleName.contains(IPOD_APP_ID, Qt::CaseInsensitive))
    {
        LOG_HIGH <<"Set m_IsCurrentPlayingMP to true"<<LOG_ENDL;
        //LOG_TRACE<<"setting m_IsRestartOfMPInProgress false, m_IsCurrentPlayingMP true 1"<<LOG_ENDL;
        m_iPodPrivateData->m_IsRestartOfMPInProgress = false;
        m_iPodPrivateData->m_IsCurrentPlayingMP = true;
        delayBasicControlEnable(true, "");
    }
    else
    {
        LOG_HIGH <<"Set m_IsCurrentPlayingMP to false"<<LOG_ENDL;
        m_iPodPrivateData->m_IsCurrentPlayingMP = false;
        delayBasicControlEnable(false, "");
    }
}

/*
void CiOSAppTaskHandler::SetIscurrentPlayingMPFlag()
{
    //LOG_TRACE << "CiOSAppTaskHandler::SetIscurrentPlayingMPFlag() IN" <<LOG_ENDL;

    //LOG_TRACE << "IOSAPP:   m_ApplicationBundleName1   = " << m_ApplicationBundleName1 << LOG_ENDL;
    //LOG_TRACE << "IOSAPP:   m_ApplicationSeedId        = " << m_ApplicationSeedId << LOG_ENDL;
    //LOG_TRACE << "IOSAPP:   m_ApplicationDisplayName   = " << m_ApplicationDisplayName << LOG_ENDL;
    //LOG_TRACE << "IOSAPP:   m_LocalLanguageDisplayName = " << m_LocalLanguageDisplayName << LOG_ENDL;

    if( m_ApplicationBundleName1.contains(IPAD_MUSICAPP_APP_ID, Qt::CaseInsensitive) ||
		m_ApplicationBundleName1.contains(IPOD_APPLICATION_BUNDLESEED_ID, Qt::CaseInsensitive) )
    {
        //LOG_TRACE<<"setting m_IsRestartOfMPInProgress false, m_IsCurrentPlayingMP true 1"<<LOG_ENDL;
        m_iPodPrivateData->m_IsRestartOfMPInProgress = false;
        m_iPodPrivateData->m_IsCurrentPlayingMP = true;
    }
    else if(m_ApplicationSeedId.contains(IPOD_APPLICATION_BUNDLESEED_ID, Qt::CaseInsensitive))
    {
        if(m_LocalLanguageDisplayName.length() > 0 && m_ApplicationDisplayName.contains(m_LocalLanguageDisplayName, Qt::CaseInsensitive))
        {
            //LOG_TRACE<<"setting m_IsRestartOfMPInProgress false, m_IsCurrentPlayingMP true 2"<<LOG_ENDL;
            m_iPodPrivateData->m_IsRestartOfMPInProgress = false;
            m_ApplicationBundleName1 = IPAD_MUSICAPP_APP_ID;
            m_iPodPrivateData->m_IsCurrentPlayingMP = true;
        }
        else if((m_iPodPrivateData->m_ExtendedLingoVersion <= 1.13) && !m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
        {
            //LOG_TRACE<<"setting m_IsRestartOfMPInProgress false, m_IsCurrentPlayingMP true 3"<<LOG_ENDL;
            m_iPodPrivateData->m_IsRestartOfMPInProgress = false;
            m_iPodPrivateData->m_IsCurrentPlayingMP = true;
            m_ApplicationBundleName1 = IPAD_MUSICAPP_APP_ID;
        }
        else if(m_ApplicationDisplayName.length() <= 0)
        {
            //LOG_TRACE<<"setting m_IsCurrentPlayingMP false 1"<<LOG_ENDL;
            //LOG_TRACE<<"m_IsCurrentPlayingMP restartin progress flag is" <<
            //           m_iPodPrivateData->m_IsRestartOfMPInProgress<<LOG_ENDL;
            m_iPodPrivateData->m_IsCurrentPlayingMP = false;
        }
    }
    else
    {
        //LOG_TRACE<<"setting m_IsCurrentPlayingMP false 2"<<LOG_ENDL;
//        m_ApplicationBundleName1 = m_ApplicationSeedId;
        m_iPodPrivateData->m_IsCurrentPlayingMP = false;
    }
    //LOG_TRACE << "CiOSAppTaskHandler::SetIscurrentPlayingMPFlag() OUT" <<LOG_ENDL;
}
*/

void CiOSAppTaskHandler::resetCurrentBasicControlStateVariable()
{
    m_bCurrentBasicControlState = true;
}

void CiOSAppTaskHandler::delayBasicControlEnable(bool enable, QString AppName)
{
    m_bBasicControlStatusToBeSend = enable;
    m_strBasicControlerAppName = AppName;
    m_delayBasicControlEnableTimer->start(500);
}

void CiOSAppTaskHandler::delayBasicControlEnableSlot()
{
    //LOG_TRACE<<"delayBasicControlEnableSlot m_bBasicControlStatusToBeSend " << m_bBasicControlStatusToBeSend<<LOG_ENDL;
    //LOG_TRACE<<"delayBasicControlEnableSlot m_strBasicControlerAppName "<< m_strBasicControlerAppName <<LOG_ENDL;

    if(m_bCurrentBasicControlState == m_bBasicControlStatusToBeSend)
    {
        LOG_HIGH <<"delayBasicControlEnableSlot Same value, returned" <<LOG_ENDL;
        return;
    }

    emit OnBasicControlEnableStatusChange(m_bBasicControlStatusToBeSend, m_strBasicControlerAppName);
    m_bCurrentBasicControlState = m_bBasicControlStatusToBeSend;
    if(m_bBasicControlStatusToBeSend == false)
    {
        //LOG_TRACE<<"OnTrackChanged from delayBasicControlEnableSlot "<<LOG_ENDL;
        emit OnTrackChanged(-1);
    }

    if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
    {
        CPlayEngineTaskHandler* PlayEngineHandler = parent()->findChild<CPlayEngineTaskHandler*>(gPlayEngineTaskHandler);
        if(PlayEngineHandler)
        {
            PlayEngineHandler->delayMetadataIndexQuery();
        }
    }
}

/******************************************************************************
// Function Name                                            Parameters
// CiOSAppTaskHandler::packetAvailable                  void (OUT), (int,int,QByteArray) (IN)
//
// Explanation
// This method is a callback that is called when a command packet is available from the apple device.
// The task handler checks the lingoId and commandId input params and does the needful with the
// command bytes in the QByteArray
// History
// 06-Dec-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CiOSAppTaskHandler::packetAvailable(int lingoID,int commandID,const QByteArray& buf )
{
#ifdef DBUGLOGS
    //LOG_TRACE << "CiOSAppTaskHandler::packetAvailable() IN, lingoID = " << lingoID << ", commandID = " << commandID << LOG_ENDL;
#endif
    try
    {
        if (lingoID == 0x00)
        {
            switch(commandID)
            {
                case 0x02: //ACK
                {
                    //LOG_TRACE << "IOSAPP:   Case 0x02_ACK" << LOG_ENDL;
                    CACK cmd( m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);
                    int ackCmd= cmd.getAcknowledgedCommand();
                    switch(ackCmd)
                    {
                        case 0x65:
                        {
                            //LOG_TRACE << "IOSAPP:   error for case 0x02 for 0x65" << LOG_ENDL;
                            m_ApplicationBundleName1 = IPOD_APPLICATION_BUNDLESEED_ID;
                            emit mainLoopQuit(ExitCode_GetNowPlayingApplicationBundleName);
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                case 0x66: //ret now playing application bundle name
                {
                    //LOG_TRACE << "IOSAPP:   Case 0x66" << LOG_ENDL;
                    CRetNowPlayingAppBundleName cmd(m_iPodPrivateData->m_isIDPS);
                    cmd.unmarshall(buf);
                    m_ApplicationBundleName1 = cmd.getNowPlayingAppBundleName();

                    //LOG_TRACE << "IOSAPP:   case 0x66, m_ApplicationBundleName1 =  " << m_ApplicationBundleName1 << LOG_ENDL;

                    emit mainLoopQuit(ExitCode_GetNowPlayingApplicationBundleName);
                }
                    break;
                case 0x4A:
                {
                    //LOG_TRACE << "IOSAPP:   Case 0x4A" << LOG_ENDL;
                    CiPodNotification cmd(m_iPodPrivateData->m_isIDPS);
                    cmd.unmarshall(buf);
                    // { Added by pawan 2013.05.15.

                    //replace this with getnotificationtype
                    //if(cmd.isDBChanged())

                    switch(cmd.getNotificationType())
                    {
                        case 0x0A:
                        {
                            //m_ApplicationBundleName1.clear();
                            //Tan. sometimes application display name come first
							//before the bundle name come, so we should not clear
							//the displayname when receiving bundle name
                            //m_ApplicationDisplayName.clear();
                            m_ApplicationBundleName1.clear();
                            m_ApplicationBundleName1 = cmd.getBundleName();
                            if(m_ApplicationBundleName1.isEmpty())
                            {
                                if(isNowPlayingApplicationIsMPE())
                                {
                                    //m_ApplicationSeedId = IPOD_APPLICATION_BUNDLESEED_ID;
                                }
                                else if(!m_iPodPrivateData->m_IsConvertedFromUIDToIndex && m_iPodPrivateData->m_IsBTCalling == false)
                                {
                                    RestartPlay();
                                }
                                else if(!m_iPodPrivateData->m_IsConvertedFromUIDToIndex && m_iPodPrivateData->m_IsBTCalling == true)
                                {
                                    LOG_HIGH << "IOSAPP: Don't RestartPlay 11" << LOG_ENDL;
                                }
                                else
                                {
                                    //This routine forcely update for empty to "com.apple" ??
                                    //m_ApplicationSeedId = IPOD_APPLICATION_BUNDLESEED_ID;
                                }
                            }

                            //LOG_TRACE << "IOSAPP:   case 0x4A for 0x0A, m_ApplicationSeedId = " << m_ApplicationSeedId << LOG_ENDL;
                        }
                            break;
                        case 0x13:
                        {

                        }
                            break;
                        default:
                        {
                            //LOG_TRACE << "IOSAPP:   This is for something else, so return." << LOG_ENDL;
                            return;
                        }
                    }

                    SetIscurrentPlayingMPFlag(m_ApplicationBundleName1);

#ifdef SIRI_TIMEOUT_TEST
                    if( m_iPodPrivateData->m_IsCurrentPlayingMP )
                    {
                        emit LaunchedDefaultMusicApp( true,  m_ApplicationBundleName1 );
                    }
                    else
                    {
                        emit LaunchedDefaultMusicApp( false, m_ApplicationBundleName1 );
                    }
#endif

                    //LOG_TRACE << "IOSAPP:   case 0x4A, nowPlayingApplicationBundleName = " << m_ApplicationBundleName1 << LOG_ENDL;

                    if(m_iPodPrivateData->m_IsTabOff)
                    {
                        //LOG_INFO << "IOSAPP:   m_IsTabOff = true, so returning" << LOG_ENDL;
                        return;
                    }
                    else if(m_iPodPrivateData->m_IsCurrentPlayingMP)
                    {
                        return;
                    }
                    else if(!m_iPodPrivateData->m_IsCurrentPlayingMP)
                    {
                        bool isIoSVersionForResumeIPod = ((m_iPodPrivateData->m_MajorVersion > 5)  ||
                                                         ((m_iPodPrivateData->m_MajorVersion == 5) &&
                                                          (m_iPodPrivateData->m_MinorVersion >= 1)));
                        if(isIoSVersionForResumeIPod)
                        {
                            //LOG_INFO  << "IOSAPP:   TAB ON and some one is playing with other media app in Device with version > 1.13 so lets play it" << LOG_ENDL;
                            //emit OnBasicControlEnableStatusChange(false, m_AppNameToBeDisplayed);
                            //delayBasicControlEnable(false, m_AppNameToBeDisplayed);
                        }
                        else
                        {
                            if(m_iPodPrivateData->m_IsBTCalling == false)
                            {
                                LOG_HIGH << "IOSAPP: below 1.13 play other media, RestartPlay" << LOG_ENDL;
                                RestartPlay();
                            }
                            else
                            {
                                LOG_HIGH << "IOSAPP: Don't RestartPlay 22" << LOG_ENDL;
                            }
                        }
                    }
                    //LOG_TRACE << "IOSAPP:   case 0x4A, nowPlayingApplicationBundleName =  " << m_ApplicationBundleName1 << LOG_ENDL;
                }
                    break;
                default :
                    break;
            }// end of switch
        }
        else
        {
#ifdef DBUGLOGS
            //LOG_TRACE<<"CiOSAppTaskHandler: Not my lingo...ignoring!"<<LOG_ENDL;
#endif
        }
    }   // end try
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CiOSAppTaskHandler::PacketAvailable(), Execption Caught *******"<< LOG_ENDL;
    }
    return;
}

bool CiOSAppTaskHandler::isNowPlayingApplicationIsMPE()
{
    //LOG_TRACE<< "IOSAPP:CiOSAppTaskHandler::isNowPlayingApplicationIsMPE() IN" << LOG_ENDL;
    QString string = QString();
    qint16 transId = m_iPodPrivateData->getTransactionId();
    int i=-1;
    QByteArray command;
    command[++i] = 0x55;
    command[++i] = 0x04;
    command[++i] = 0x00;
    command[++i] = 0x65;

    if(m_iPodPrivateData->m_isIDPS == true)
    {
        command[++i] = (transId & 0xff00) >> 8;
        command[++i] =  transId & 0x00ff;
    }

    command[1] = i - 1;
    command[i+1] = CIpodControllerGlobal::checkSum(command, i);
    emit sendiAPPacket(command);
    piPodLog->WriteLog("getNowPlayingApplicationBundleNameE waitForResponceFromExitCode before");
    waitForResponceFromExitCode(7000, ExitCode_GetNowPlayingApplicationBundleName);
    string =  m_ApplicationBundleName1;

    SetIscurrentPlayingMPFlag(m_ApplicationBundleName1);

    //LOG_TRACE<< "IOSAPP:CiOSAppTaskHandler::isNowPlayingApplicationIsMPE() OUT, string = "
    //         << string.toAscii().data() << LOG_ENDL;
    return m_iPodPrivateData->m_IsCurrentPlayingMP;
}

bool CiOSAppTaskHandler::isBasicControlEnabled()
{
    return m_bCurrentBasicControlState;
}

bool CiOSAppTaskHandler::RestartPlay()
{
    //LOG_TRACE << "IOSAPP:CiOSAppTaskHandler::RestartPlay() IN" << LOG_ENDL;
    bool isIoSVersionForResumeIPod = ((m_iPodPrivateData->m_MajorVersion > 5)  ||
                                     ((m_iPodPrivateData->m_MajorVersion == 5) &&
                                      (m_iPodPrivateData->m_MinorVersion >= 1)));

    m_iPodPrivateData->m_IsRestartOfMPInProgress = true;
    bool result = false;

    bool isEmptyBundleName = (m_ApplicationBundleName1 == "") || (m_ApplicationBundleName1 == " ") ;

    CPlayEngineTaskHandler* PlayEngineHandler = parent()->findChild<CPlayEngineTaskHandler*>(gPlayEngineTaskHandler);
    if( PlayEngineHandler )
    {
        bool isAfter3rdPartyInteraction = false;
        if(isAfter3rdPartyInteraction)
        {
            LOG_HIGH << "IOSAPP:RestartPlay(), call playControlE(\"ResumeiPod\")"  << LOG_ENDL;
            result = PlayEngineHandler->playControlE("ResumeiPod");
        }
        else if(isEmptyBundleName)
        {
            LOG_HIGH << "IOSAPP:RestartPlay(), call PlayiPodFiles()" <<LOG_ENDL;
            result = PlayEngineHandler->PlayiPodFiles();
        }
        else
        {
            //check if bundle name is empty.
            LOG_HIGH << "IOSAPP:RestartPlay(), call playControlE(\"Play\")"  << LOG_ENDL;
            result = PlayEngineHandler->playControlE("Play");
        }
    }
    LOG_TRACE << "IOSAPP:CiOSAppTaskHandler::RestartPlay() OUT" << LOG_ENDL;
    return result;
}


/*
bool CiOSAppTaskHandler::RestartPlay()
{
    LOG_TRACE << "IOSAPP:CiOSAppTaskHandler::RestartPlay() IN" << LOG_ENDL;
    bool isIoSVersionForResumeIPod = ((m_iPodPrivateData->m_MajorVersion > 5)  ||
                                     ((m_iPodPrivateData->m_MajorVersion == 5) &&
                                      (m_iPodPrivateData->m_MinorVersion >= 1)));



    m_iPodPrivateData->m_IsRestartOfMPInProgress = true;
    bool result = false;

    CPlayEngineTaskHandler* PlayEngineHandler = parent()->findChild<CPlayEngineTaskHandler*>(gPlayEngineTaskHandler);
    if( PlayEngineHandler )
    {
        bool isNowPlayingComApple = isNowPlayingApplicationIsMPE();
        //if(isNowPlayingComApple || (m_ApplicationBundleName1.length() == 0)) // if playing podcast etc.
        bool isEmptyBundlewithUILock = ( (m_bCurrentBasicControlState) && (m_ApplicationBundleName1.length() == 0));
        bool isPlayControlPlay = (isNowPlayingComApple || isEmptyBundlewithUILock);
        if(isPlayControlPlay)
        {
            LOG_HIGH << "IOSAPP:RestartPlay(), call playControlE(\"Play\") m_ApplicationBundleName1:"  << m_ApplicationBundleName1<<LOG_ENDL;
            result = PlayEngineHandler->playControlE("Play");
        }
        else
        {
            if(isIoSVersionForResumeIPod) // } modified 2012.12.20
            {
                LOG_HIGH << "IOSAPP:RestartPlay(), call playControlE(\"ResumeiPod\")"  << LOG_ENDL;
                result = PlayEngineHandler->playControlE("ResumeiPod");
            }
            else
            {
                int currentPosition = PlayEngineHandler->getCurrentMediaPosition();
                LOG_HIGH << "IOSAPP:RestartPlay(), call playCommandCall(PlayEngineHandler->m_CurrentIndex = " << PlayEngineHandler->m_CurrentIndex << ")"  << LOG_ENDL;
                m_iPodDBIndexFetchTaskHandler->resetPreviousSelectionsE();
                m_iPodDBIndexFetchTaskHandler->getCategorizedListCountE("Track");
                if(PlayEngineHandler->m_CurrentIndex < 0)
                    PlayEngineHandler->m_CurrentIndex = 0;
                result =  m_iPodDBIndexFetchTaskHandler->playCommandCall(PlayEngineHandler->m_CurrentIndex);
                //LOG_TRACE << "IOSAPP:   playCommandCall(PlayEngineHandler->m_CurrentIndex), result =  " << result << LOG_ENDL;
                PlayEngineHandler->setPosition(currentPosition);

                if(result && m_iPodPrivateData->m_IsCurrentPlayingMP)
                {
                    MediaInfo currentMediaInfo;
                    PlayEngineHandler->emitCurrentMediaInfoE(&currentMediaInfo);
                }
            }
        }
    }
    return result;
}
*/
