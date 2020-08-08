/*
 ******************************************************************
 * COPYRIGHT � <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Neelima Khedad,Venkatesh Aeturi                  *
 * DATE       :  28 July 2011                                     *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 * MODULE SUMMARY : MOST Manager is a middle ware component which *
 *                  talks to the applications and controller over *
 *                  QtDbus.The data sent over QtDbus is further   *
 *                  communicated to other MOST devices through ACS*
 *                  daemon.This module is also responsible for    *
 *                  commands from ACS daemon.                     *
 *                  This file contains the class definition for   *
 *                  CMOST manager.This is the class which connects*
 *                  to ACS daemon using marshalled application    *
 *                  instance.All the FBlockControl class will be  *
 *                  instanctiated here                            *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * 28 Jul 2011  Neelima Khedad           Draft version
 * 08 Aug 2011  Neelima Khedad           Initializing DLNA Shadow block
 * 09 Aug 2011  Sajeer Shamsudeen        Initializing DVD Function block
 * 07 Aug 2011  venkatesh Aeturi         Initializing AuxIn Function block
 * 09 Aug 2011  venkatesh Aeturi         Initializing DMB Function block
 * 22 Aug 2011  Neelima Khedad           Initializing HMI Fblock and Shadow
 * 29 Aug 2011  Neelima Khedad           Initializing IBOX Shadow block
 * 06 Sep 2011  Shiva Kumar              Initializing SWDL Function block
 * 12 Sep 2011  Amit Koparde             Initializing DAB Function block
 * 12 Sep 2011  Venkatesh Aeturi         Initializing Navigation Function block.
 * 13 Sep 2011  Amit Koparde             Initializing Diagnosis Function block.
 * 08 Feb 2011  Neelima Khedad           Using the latest Version of LOG_INIT function Logger module
 * 30 Oct 2012  Shiva Kumar              Changes for CR15173
 ******************************************************************
 */

#include <linux/unistd.h>   // for printing PID, TID

#include <signal.h>         // for signal handler
#include <bits/siginfo.h>
#include <stdlib.h>
#include <stdio.h>          // for snprintf(...)
#include <syslog.h>         // for syslog(...)

#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_MOSTManager.h"
#include "DHAVN_MOSTManager_Adaptor.h"
#include "DHAVN_MOSTManager_ServiceName.h"
#include <QtDBus/QtDBus>
#include <QTimer>

#include <DHAVN_AppFramework_Def.h>
#include <DHAVN_AppFramework_Logger_Def.h>

// To check connection between mostmanager and acsdaemon, acs and amf.
bool g_BridgeConnected = false;
QMutex g_BridgeMutex;
bool g_AcsDaemonConnected = false;
QMutex g_AcsDaemonMutex;
bool g_ACSDisconnByMHP = false;

void SetAcsBridgeState(bool state)
{
    g_BridgeMutex.lock();
    g_BridgeConnected = state;
    g_BridgeMutex.unlock();
}

bool GetAcsBridgeState(void)
{
    bool state;

    g_BridgeMutex.lock();
    state = g_BridgeConnected;
    g_BridgeMutex.unlock();

    return state;
}

void SetAcsDaemonState(bool state)
{
    g_AcsDaemonMutex.lock();
    g_AcsDaemonConnected = state;
    g_AcsDaemonMutex.unlock();
}

bool GetAcsDaemonState(void)
{
    bool state;

    g_AcsDaemonMutex.lock();
    state = g_AcsDaemonConnected;
    g_AcsDaemonMutex.unlock();

    return state;
}

/*
 *******************************************************************
 * Function:CMOSTManager
 *
 * Description :Is a constructor of CMOSTManager class which
 *              initializes all the ACS Application events and 
 *              message events.
 *
 * Parameters : 
 *
 * Returns : none
 *
 *******************************************************************
 */

CMOSTManager::CMOSTManager()
    : CIACSApplicationEvents(), m_MessagesEvent(true), retVal(false), m_Running(false)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)

    m_Application = new CMarshaledACSApplication(this);
    m_MostDriver = new CMOSTDriver();

    m_CDLNAShadowControl = new CDLNAShadowControl(m_Application);
    m_CAuxInFBlockControl = new CAuxInFblockControl(m_Application);
    m_CHMIFBlockShadowControl = new CHMIFBlockShadowControl(m_Application);
    m_CIBOXShadowControl = new CIBOXShadowControl(m_Application);
    m_CSpeechRecognitionFblockControl = new CSpeechRecognitionFblockControl(m_Application);
    m_CBluetoothFblockControl = new CBluetoothFblockControl(m_Application);
    m_CTelephoneShadowControl = new CTelephoneShadowControl(m_Application);
    m_CSWDLFBlockControl = new CSWDLFBlockShadowControl(m_Application);
    m_CNavigationFblockControl = new CNavigationFblockControl(m_Application);
    m_CDiagnosisFBlockShadowControl = new CDiagnosisFBlockShadowControl(m_Application);
    m_CDMBTunerFblockControl = new CDMBTunerFblockControl(m_Application);
    m_CSatRadioFblockControl = new CSatRadioFblockControl(m_Application);
#ifdef _LOG_FB_ENABLE_
    m_CLogFblockControl = new CLogFblockControl(m_Application);
#endif

    memset(&m_FBlockInitialized, 0, sizeof(m_FBlockInitialized));

    connect(m_MostDriver, SIGNAL(DriverLoaded()), this, SLOT(onDriverLoaded()));
    connect(m_MostDriver, SIGNAL(DriverCrashed()), this, SLOT(onDriverCrashed()));

	connect(m_MostDriver, SIGNAL(MHPCrashed()), this, SLOT(onMHPCrashed()));	// 2014.11.22 shy, VI 개선 평가 , uvo menu 진입 불가 
}

/*
 *******************************************************************
 * Function:~CMOSTManager
 *
 * Description :Is a destructor of CMOSTManager class
 *
 * Parameters :
 *
 * Returns :
 *
 *******************************************************************
 */

CMOSTManager::~CMOSTManager()
{
    delete m_Application;    
    delete m_MostDriver;

    delete m_CDLNAShadowControl;
    delete m_CAuxInFBlockControl;
    delete m_CHMIFBlockShadowControl;
    delete m_CIBOXShadowControl;
    delete m_CBluetoothFblockControl;
    delete m_CSpeechRecognitionFblockControl;
    delete m_CTelephoneShadowControl;
    delete m_CSWDLFBlockControl;
    delete m_CNavigationFblockControl;
    delete m_CDiagnosisFBlockShadowControl;
    delete m_CDMBTunerFblockControl;
    delete m_CSatRadioFblockControl;
#ifdef _LOG_FB_ENABLE_
    delete m_CLogFblockControl;
#endif
}

/*
 *******************************************************************
 * Function:MessagesAvailable
 *
 * Description :Function called by ACSApplicationEvent on ACS events.
 *              This function sets the MessageEvent to notify the 
 *              ACS Marshaled Application Queue.
 * Parameters : none
 *
 * Returns : void
 *
 *******************************************************************
 */

void CMOSTManager::MessagesAvailable()
{
    m_MessagesEvent.Set();
    return;
}

/*
 *******************************************************************
 * Function:NoMessagesAvailable
 *
 * Description :Function called by ACSApplicationEvent to reset the 
 *              ACS Marshaled Application Queue.
 *
 * Parameters : none
 *   
 *
 * Returns : void
 *
 *******************************************************************
 */

void CMOSTManager::NoMessagesAvailable()
{
    m_MessagesEvent.Reset();
    return;
}


void CMOSTManager::Launch()
{
    m_MostDriver->LaunchDriver(0);
}

/*
 *******************************************************************
 * Function:Configure
 *
 * Description :Function called by main to initialize all FBlock to 
 *              MOST elements.
 *
 * Parameters : none
 *    
 * Returns : true or false as a bool
 *
 *******************************************************************
 */

bool CMOSTManager::Configure()
{
    bool bRet = false;
    uint initailized = 0;

    LOG_SIGNAL << LOG_TAG_MOSTMANAGER << " Main, Start Most Manager Configure " << LOG_ENDL;

#if 0		// 2014.11.6, hyunyong.sung, need to init after register object
    m_FBlockInitialized.DLNA_SHADOW = m_CDLNAShadowControl->Initialize();
    m_FBlockInitialized.AUXIN_FBLOCK = m_CAuxInFBlockControl->Initialize();
    m_FBlockInitialized.HMI_FBLOCK = m_CHMIFBlockShadowControl->Initialize();
    m_FBlockInitialized.IBOX_SHADOW = m_CIBOXShadowControl->Initialize();
    m_FBlockInitialized.SR_FBLOCK = m_CSpeechRecognitionFblockControl->Initialize();
    m_FBlockInitialized.BT_FBLOCK = m_CBluetoothFblockControl->Initialize();
    m_FBlockInitialized.TELEPHONE_SHADOW = m_CTelephoneShadowControl->Initialize();
    m_FBlockInitialized.SWDL_FBLOCK = m_CSWDLFBlockControl->Initialize();
    m_FBlockInitialized.NAVI_FBLOCK = m_CNavigationFblockControl->Initialize();
    m_FBlockInitialized.DIAG_FBLOCK = m_CDiagnosisFBlockShadowControl->Initialize();
    m_FBlockInitialized.DMB_FBLOCK = m_CDMBTunerFblockControl->Initialize();
    m_FBlockInitialized.SATRADIO_FBLOCK = m_CSatRadioFblockControl->Initialize();
#endif

#ifdef _LOG_FB_ENABLE_
    m_FBlockInitialized.LOG_FBLOCK = m_CLogFblockControl->Initialize();
#endif

	if(!(m_FBlockInitialized.LOG_FBLOCK))
	{
	        LOG_CRITICAL << LOG_TAG_MOSTMANAGER << " Failed to initialize control objects = " << initailized << LOG_ENDL;
		bRet = false;
	}
	else
	{
		bRet = true;
	}
	
#if 0		// 2014.11.6, hyunyong.sung, need to init after register object
    memcpy(&initailized, &m_FBlockInitialized, sizeof(m_FBlockInitialized));

#ifdef _LOG_FB_ENABLE_
    if((initailized & 0x1FFF) != 0x1FFF)
#else
    if((initailized & 0x1FFF) != 0x0FFF)
#endif
    {
        LOG_CRITICAL << LOG_TAG_MOSTMANAGER << " Failed to initialize control objects = " << initailized << LOG_ENDL;
        bRet = false;
    }
    else
    {
        bRet = true;
    }
#endif	

    return bRet;
}

void CMOSTManager::SetRunningState(bool state)
{
    m_RunningMutex.lock();
    m_Running = state;
    m_RunningMutex.unlock();
}

bool CMOSTManager::GetRunningState()
{
    bool state;

    m_RunningMutex.lock();
    state = m_Running;
    m_RunningMutex.unlock();

    return state;
}

void CAdaptorThread::ConnectToUISH()
{    
    if( !m_AppFrameworkIPC->RegisterService(APP_MOSTMANAGER) )    
    {
        LOG_CRITICAL << LOG_TAG_ADAPTORTHREAD << " Failed to register to dbus" << LOG_ENDL;
    }

    m_AppFrameworkIPC->ListenService(APP_UI_MANAGER);
    m_AppFrameworkIPC->RegisterApp(APP_UI_MANAGER, DISPLAY_FRONT, APP_MOSTMANAGER);

}

void CAdaptorThread::SignalConnection()
{
    struct sigaction act;

    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_flags |= SA_RESETHAND;
    act.sa_handler = CAdaptorThread::SignalHandler;

    for(int signumber = SIGHUP; signumber < SIGUNUSED; signumber++)
    {
        if( signumber == SIGKILL || signumber == SIGSTOP || signumber == SIGCHLD)
        {
            continue;
        }

        if( sigaction(signumber, &act, (struct sigaction *)NULL) == -1 )
        {
            LOG_CRITICAL << LOG_TAG_ADAPTORTHREAD << " Signal configuration failed : " << signumber << LOG_ENDL;
            continue;
        }
    }
}

CAdaptorThread * CAdaptorThread::g_SigReceiverObject = NULL;

void CAdaptorThread::SetSignalHandlerObject(CAdaptorThread *object)
{
    g_SigReceiverObject = object;
}

void CAdaptorThread::SignalHandler(int signum)
{
    if(g_SigReceiverObject != NULL)
    {
        g_SigReceiverObject->Restart(signum);
    }
    else
    {
        char log[100];
        openlog(NULL, LOG_PID, LOG_LOCAL0);
        snprintf(log, 100, " DHAVN_MOSTManager:CRITICAL  :: SignalHandler Can NOT send EVT_REQUEST_ABNORMAL_TERMINATION event to UISH, signum = %d", signum);
        syslog(LOG_CRIT|LOG_LOCAL0, log);
        closelog();
    }

    raise(signum);
}

void CAdaptorThread::Restart(int signum)
{
    // remove 2014/02/04
    //LOG_CRITICAL << "SignalHandler" << " MOSTManager Will be restarted, signum = " << signum << LOG_ENDL;

    Event event(EVT_REQUEST_ABNORMAL_TERMINATION, APP_MOSTMANAGER, APP_UI_MANAGER, (EVT_COMMAND_T)signum);

    g_SigReceiverObject->m_AppFrameworkIPC->DisconnectService(APP_UI_MANAGER);
    g_SigReceiverObject->m_AppFrameworkIPC->UnregisterService(APP_MOSTMANAGER);
    g_SigReceiverObject->m_AppFrameworkIPC->SendEvent(APP_UI_MANAGER, event, DISPLAY_FRONT);
}

void CMOSTManager::onDriverLoaded()
{
    m_MostDriver->StartMonitoring();

    /** Connect to ACS daemon to receive events */
    start(QThread::HighestPriority);
}

void CMOSTManager::onDriverCrashed()
{
    LOG_SIGNAL << LOG_TAG_MOSTMANAGER << " call onDriverCrashed( ) " << LOG_ENDL;

    SetRunningState(false);

    m_MostDriver->StopMonitoring();
    SetAcsDaemonState(false);

    m_MostDriver->ReInitialize();
    m_MostDriver->LaunchDriver(1);
}

// 2014.11.22 shy, VI 개선평가 , UVO menu 진입 불가 
void CMOSTManager::onMHPCrashed()
{
    LOG_SIGNAL << LOG_TAG_MOSTMANAGER << " call onMHPCrashed( ) " << LOG_ENDL;

    SetRunningState(false);

    m_MostDriver->StopMonitoring();
    SetAcsDaemonState(false);

    m_MostDriver->ReInitialize();
    m_MostDriver->LaunchDriver(3);
}



/*
 *******************************************************************
 * Function:Execute
 *
 * Description :Function called by main to start ACS Application 
 *              and wait for the message events from ACS Marshaled 
 *              Application Queue.
 *
 * Parameters : none
 *    
 * Returns : void
 *
 *******************************************************************
 */

void CMOSTManager::run()
{
    uchar eventChannels[] = { EVENT_CHANNEL_MOST_HIGH, EVENT_CHANNEL_MLB_PINGPONG, EVENT_CHANEL_LOG_MESSAGE };

    LOG_HIGH << LOG_TAG_MOSTMANAGER << " Trying to connect to ACS Daemon," << " PID : " << getpid() << ", TID : " << syscall(__NR_gettid) << LOG_ENDL;

    /**
     * TODO:: The parameters for Start application will be changed later
     * once actual ACS daemon will be used
     */    

    if( m_Application->StartApplication(THREAD_PRIORITY, SERVER_ADDRESS, SERVER_PORT, BUS_ADDRESS) )
    {
        LOG_HIGH << LOG_TAG_MOSTMANAGER << " Connected to ACS daemon " << LOG_ENDL;

        SetRunningState(true);
        SetAcsDaemonState(true);

        m_Application->Events()->SetListener(m_CLogFblockControl);

        if( !m_Application->Events()->JoinChannels(sizeof(eventChannels), eventChannels) )
        {
            LOG_CRITICAL << LOG_TAG_MOSTMANAGER << " Cannot joined ACS event channel " << LOG_ENDL;
        }
		
        while( GetRunningState() )
        {            
            if (m_MessagesEvent.Wait(MOST_MSG_TIMEOUT))
            {
                /** Message arrived */
                while( m_Application->Dispatch() )
                {
                    LOG_TRACE << LOG_TAG_MOSTMANAGER << " Dispatch the event received for processing " << LOG_ENDL;
                }
            }
        }

//        LOG_CRITICAL << LOG_TAG_MOSTMANAGER << " Disconnected from ACS daemon " << LOG_ENDL;
        m_Application->StopApplication();
        SetAcsDaemonState(false);
    }
    else
    {
        LOG_CRITICAL << LOG_TAG_MOSTMANAGER << " Disconnected from ACS daemon on run( ) " << LOG_ENDL;
        SetAcsDaemonState(false);
    }

    //exec();   // Do NOT need to wait exit calling outside of this thread

    return;

}
/*
 *******************************************************************
 * Function:GetAuxInControlInstance
 *
 * Description :Function called by AuxIn Adaptor to get the
 *              instance of CAuxInFBlockControl.
 *
 * Parameters : none
 *
 * Returns :returns instance of CAuxInFBlockControl
 *
 *******************************************************************
 */

CAuxInFblockControl* CMOSTManager::GetAuxInControlInstance()
{
    return m_CAuxInFBlockControl;
}

/*
 *******************************************************************
 * Function:GetSWDLFBlockControlInstance
 *
 * Description :Function called by SWDL Adaptor to get the
 *              instance of CSWDLFBlockControl.
 *
 * Parameters : none
 *
 * Returns :returns instance of CSWDLFBlockControl
 *
 *******************************************************************
 */

CSWDLFBlockShadowControl* CMOSTManager::GetSWDLFBlockControlInstance()
{
    return m_CSWDLFBlockControl;
}

/*
 *******************************************************************
 * Function:GetDLNAShadowControlInstance
 *
 * Description :Function called by DLNA Adaptor to get the
 *              instance of CDLNAShadowControl.
 *
 * Parameters :
 *
 * Returns :returns instance of CAmFmTunerControl
 *
 *******************************************************************
 */

CDLNAShadowControl* CMOSTManager::GetDLNAShadowControlInstance()
{
    return m_CDLNAShadowControl;
}

/*
 *******************************************************************
 * Function:GetHMIFBlock_ShadowControl
 *
 * Description :Function called by HMI Adaptor to get the
 *              instance of CHMIFBlock_ShadowControl.
 *
 * Parameters :
 *
 * Returns :returns instance of CHMIFBlock_ShadowControl
 *
 *******************************************************************
 */

CHMIFBlockShadowControl* CMOSTManager::GetHMIFBlockShadowControl()
{
    return m_CHMIFBlockShadowControl;
}

/*
 *******************************************************************
 * Function:GetIBOXShadowControl
 *
 * Description :Function called by HMI Adaptor to get the
 *              instance of CHMIFBlock_ShadowControl.
 *
 * Parameters :
 *
 * Returns :returns instance of CHMIFBlock_ShadowControl
 *
 *******************************************************************
 */

CIBOXShadowControl* CMOSTManager::GetIBOXShadowControlInstance()
{
    return m_CIBOXShadowControl;
}

/*
 *******************************************************************
 * Function:GetSpeechRecognitionControlInstance
 *
 * Description :Function called by speechrecognition Adaptor to get the
 *              instance of CSpeechRecognitionFblockControl.
 *
 * Parameters :
 *
 * Returns :returns instance of CSpeechRecognitionFblockControl
 *
 *******************************************************************
 */
CSpeechRecognitionFblockControl* CMOSTManager::GetSpeechRecognitionControlInstance()
{
    return m_CSpeechRecognitionFblockControl;
}
/*
 *******************************************************************
 * Function:GetBluetoothControlInstance
 *
 *
 *
 * Description :Function called by bluetooth Adaptor to get the
 *              instance of CBluetoothFblockControl.
 *
 *
 * Parameters :
 *
 * Returns :returns instance of CBluetoothFblockControl
 *
 *******************************************************************
 */
CBluetoothFblockControl* CMOSTManager::GetBluetoothControlInstance()
{
    return m_CBluetoothFblockControl;
}

/*
 *******************************************************************
 * Function:GetTelephoneShadowControl
 *
 * Description :Function called by HMI Adaptor to get the
 *              instance of CHMIFBlock_ShadowControl.
 *
 * Parameters :
 *
 * Returns :returns instance of CTelephoneShadowControl
 *
 *******************************************************************
 */

CTelephoneShadowControl* CMOSTManager::GetTelephoneShadowControlInstance()
{
    return m_CTelephoneShadowControl;
}

/*
 *******************************************************************
 * Function:GetNavigationControlInstance
 *
 * Description :Function called by Navigation Adaptor to get the
 *              instance of CNavigationFblockControl.
 *
 * Parameters :
 *
 * Returns :returns instance of CNavigationFblockControl
 *
 *******************************************************************
 */

CNavigationFblockControl* CMOSTManager::GetNavigationControlInstance()
{
    return m_CNavigationFblockControl;
}

/*
 *******************************************************************
 * Function:GetDiagnosisFBlock_ShadowControl
 *
 * Description :Function called by Diagnosis Adaptor to get the
 *              instance of CDiagnosisFBlock_ShadowControl.
 *
 * Parameters : none
 *
 * Returns :returns instance of CDiagnosisFBlock_ShadowControl
 *
 *******************************************************************
 */

CDiagnosisFBlockShadowControl* CMOSTManager::GetDiagnosisFBlockShadowControlInstance()
{
    return m_CDiagnosisFBlockShadowControl;
}

CDMBTunerFblockControl* CMOSTManager::GetDMBTunerFblockControlInstance()
{
    return m_CDMBTunerFblockControl;
}

CSatRadioFblockControl* CMOSTManager::GetSatRadioFblockControlInstance()
{
    return m_CSatRadioFblockControl;
}

/*
 *******************************************************************
 * Function:CAdaptorThread
 *
 * Description :Is a constructor of CAdaptorThread.
 *
 * Parameters :
 *    context     rw   Instance of CMOSTManager.
 *
 * Returns : none
 *
 *******************************************************************
 */

CAdaptorThread::CAdaptorThread(CMOSTManager *context)
{
    LOG_INIT(LOG_FILE, LOG_PREFIX_MOST)

    m_MostManagerContext = context;

    m_AppFrameworkIPC = new AppFrameworkIPC(this);

    SignalConnection();
    SetSignalHandlerObject(this);
}

/*
 *******************************************************************
 * Function:~CAdaptorThread
 *
 * Description :Is a Destructor
 *
 * Parameters : none	
 *
 * Returns :none
 *
 *******************************************************************
 */

CAdaptorThread::~CAdaptorThread()
{
    delete m_AppFrameworkIPC;
}
/*
 *******************************************************************
 * Function:Run
 *
 * Description :Is Adaptor Thread function which conects MOST Manager 
 *              to QTDBus. Registers the service and registers 
 *              AmFmTuner Object.
 *
 * Parameters : none
 *   
 * Returns : void
 *
 *******************************************************************
 */

void CAdaptorThread::Execute()
{
//    LOG_SIGNAL << LOG_TAG_ADAPTORTHREAD << " Creating Adaptor thread " << " PID : " << getpid() << ", TID : " << syscall(__NR_gettid) << LOG_ENDL;
//    LOG_CRITICAL << LOG_TAG_ADAPTORTHREAD << " Connect to D-BUS" << LOG_ENDL;
    if (!QDBusConnection::sessionBus().isConnected())
    {
        fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                "To start it, run:\n"
                "\teval `dbus-launch --auto-syntax`\n");
        //DebugLog(LOG_TAG_ADAPTORTHREAD,"Cannot connect to D-BUS session bus");
        LOG_CRITICAL << LOG_TAG_ADAPTORTHREAD << " Cannot connect to D-BUS session bus" << LOG_ENDL;
        return;
    }
    LOG_CRITICAL << LOG_TAG_ADAPTORTHREAD << " Connect to D-BUS" << LOG_ENDL;

    LOG_TRACE << LOG_TAG_ADAPTORTHREAD << " Register Service :: com.dhavn.mostmanager.qtdbus " << LOG_ENDL;
    QDBusConnection::sessionBus().registerService(SERVICE_NAME);

    LOG_TRACE << LOG_TAG_ADAPTORTHREAD << " Register DLNA Object " << LOG_ENDL;
    CDLNAAdaptor *dlnaAdaptorObject = new CDLNAAdaptor(m_MostManagerContext);
    QDBusConnection::sessionBus().registerObject(DLNA_OBJECT, dlnaAdaptorObject, QDBusConnection::ExportAllContents);

    LOG_TRACE << LOG_TAG_ADAPTORTHREAD << " Register Auxin Object " << LOG_ENDL;
    CAuxInAdaptor *auxinAdaptorObject = new CAuxInAdaptor(m_MostManagerContext);
    QDBusConnection::sessionBus().registerObject(AUXIN_OBJECT, auxinAdaptorObject, QDBusConnection::ExportAllContents);

    LOG_TRACE << LOG_TAG_ADAPTORTHREAD << " Register HMI Object " << LOG_ENDL;
    CHMIAdaptor *hmiAdaptorObject = new CHMIAdaptor(m_MostManagerContext);
    QDBusConnection::sessionBus().registerObject(HMI_OBJECT, hmiAdaptorObject, QDBusConnection::ExportAllContents);

    LOG_TRACE << LOG_TAG_ADAPTORTHREAD << " Register IBOX Object " << LOG_ENDL;
    CIBoxAdaptor *iboxAdaptorObject = new CIBoxAdaptor(m_MostManagerContext);
    QDBusConnection::sessionBus().registerObject(IBOX_OBJECT, iboxAdaptorObject, QDBusConnection::ExportAllContents);

    LOG_TRACE << LOG_TAG_ADAPTORTHREAD << " Register Speech Recognition Object " << LOG_ENDL;
    CSpeechAdaptor *speechRecognitionAdaptorObject = new CSpeechAdaptor(m_MostManagerContext);
    QDBusConnection::sessionBus().registerObject(SPEECH_OBJECT, speechRecognitionAdaptorObject, QDBusConnection::ExportAllContents);

    LOG_TRACE << LOG_TAG_ADAPTORTHREAD << " Register Bluetooth Object " << LOG_ENDL;
    CBlueToothAdaptor *bluetoothAdaptorObject = new CBlueToothAdaptor(m_MostManagerContext);
    QDBusConnection::sessionBus().registerObject(BLUETOOTH_OBJECT, bluetoothAdaptorObject, QDBusConnection::ExportAllContents);

    LOG_TRACE << LOG_TAG_ADAPTORTHREAD << " Register Telephone Object " << LOG_ENDL;
    CTelephoneAdaptor *telephoneAdaptorObject = new CTelephoneAdaptor(m_MostManagerContext);
    QDBusConnection::sessionBus().registerObject(TELEPHONE_OBJECT, telephoneAdaptorObject, QDBusConnection::ExportAllContents);

    LOG_TRACE << LOG_TAG_ADAPTORTHREAD << " Register SWDL Object " << LOG_ENDL;
    CSWDLAdaptor *swdlAdaptorObject = new CSWDLAdaptor(m_MostManagerContext);
    QDBusConnection::sessionBus().registerObject(SWDL_OBJECT, swdlAdaptorObject, QDBusConnection::ExportAllContents);

    LOG_TRACE << LOG_TAG_ADAPTORTHREAD << " Register Navigation Object " << LOG_ENDL;
    CNavigationAdaptor *navigationAdaptorObject = new CNavigationAdaptor(m_MostManagerContext);
    QDBusConnection::sessionBus().registerObject(NAVIGATION_OBJECT, navigationAdaptorObject, QDBusConnection::ExportAllContents);

    LOG_TRACE << LOG_TAG_ADAPTORTHREAD << " Register Diagnosis Object " << LOG_ENDL;
    CDiagnosisAdaptor *diagnosisAdaptorObject = new CDiagnosisAdaptor(m_MostManagerContext);
    QDBusConnection::sessionBus().registerObject(DIAGNOSIS_OBJECT, diagnosisAdaptorObject, QDBusConnection::ExportAllContents);

    LOG_TRACE << LOG_TAG_ADAPTORTHREAD << " Register DMBTnuer Object " << LOG_ENDL;
    CDMBTunerAdaptor *dmbTunerAdaptorObject = new CDMBTunerAdaptor(m_MostManagerContext);
    QDBusConnection::sessionBus().registerObject(DMBTUNER_OBJECT, dmbTunerAdaptorObject, QDBusConnection::ExportAllContents);

    LOG_TRACE << LOG_TAG_ADAPTORTHREAD << " Register SatRadio Object " << LOG_ENDL;
    CSatRadioAdaptor *satRadioAdaptorObject = new CSatRadioAdaptor(m_MostManagerContext);
    QDBusConnection::sessionBus().registerObject(SATRADIO_OBJECT, satRadioAdaptorObject, QDBusConnection::ExportAllContents);

    LOG_TRACE << LOG_TAG_ADAPTORTHREAD << " Register Adaptor Object " << LOG_ENDL;
    CAppStateAdaptor *appStateAdaptorObject = new CAppStateAdaptor(m_MostManagerContext);
    QDBusConnection::sessionBus().registerObject(APPSTATE_OBJECT, appStateAdaptorObject, QDBusConnection::ExportAllContents);

    appStateAdaptorObject->RegisterAll();

    return;
}

CMOSTDriver::CMOSTDriver()
    : m_Initialized(false),
      m_WaitFor10s(false),
      m_ResetCount(0),
      m_NetOn(-1),
      m_ConfigOk(-1),
      m_BridgeConnected(-1)
{
    LOG_INIT_V2(LOG_PREFIX_MOST);

    m_Daemon = new QProcess(this);

    m_FSWatcher = new QFileSystemWatcher(this);
    connect(m_FSWatcher, SIGNAL(fileChanged(const QString&)), this, SLOT(onFileChanged(const QString&)));

    m_DaemonWather = new QTimer(this);
    connect(m_DaemonWather, SIGNAL(timeout()), this, SLOT(onDaemonWatherTimeout()));
}

CMOSTDriver::~CMOSTDriver()
{
    delete m_Daemon;
    delete m_FSWatcher;
    delete m_DaemonWather;
}

void CMOSTDriver::ReInitialize()
{
    m_Initialized = false;
    m_WaitFor10s = false;
    m_ResetCount = 0;
    m_NetOn = -1;
    m_ConfigOk = -1;
    m_BridgeConnected = -1;
}

// mode 0 : Launch k2lacsdaemon at 1st
// mode 1 : Launch k2lacsdaemon at 2nd, 3rd ... so on after the daemon killed
// mode 2 : Check info file after timeout and then decide to reset the daemon
void CMOSTDriver::LaunchDriver(int mode)
{
    if(mode == 0)
    {
        LOG_HIGH << LOG_TAG_MOSTDRIVER << " Launch MOST Driver" << LOG_ENDL;

        m_Daemon->start("/app/bin/K2L-init start");
        QTimer::singleShot(DAEMON_WAIT_TIME, this, SLOT(onTimerTimeout()));

        return;
    }
    else if(mode == 1)
    {
        LOG_HIGH << LOG_TAG_MOSTDRIVER << " Launch MOST Driver again_mode 1" << LOG_ENDL;

        m_Daemon->start("/app/bin/K2L-init start");
        QTimer::singleShot(DAEMON_WAIT_TIME, this, SLOT(onTimerTimeout()));

        return;
    }
	else if(mode == 3)		// 2014.11.22 shy, VI 개선평가 , UVO menu 진입 불가 
	{
		LOG_HIGH << LOG_TAG_MOSTDRIVER << " Launch MOST Driver again_mode 3" << LOG_ENDL;

		m_Daemon->start("/app/bin/K2L-init kill");
		QTimer::singleShot(DAEMON_WAIT_TIME, this, SLOT(onTimerTimeout()));
		return;
	}
    else
    {
		LOG_SIGNAL << LOG_TAG_MOSTDRIVER << " Launch MOST Driver again_mode 2" << LOG_ENDL;    
        /* No Statement */
        // In case of mode 2, nothing to do
    }

    QFile *file = new QFile(DRIVER_INFO_PATH);

    // Exception handling
    if(file == NULL)
    {
//        QTimer::singleShot(500, this, SLOT(onTimerTimeout()));
        return;
    }

    if (!file->open(QIODevice::ReadOnly))
    {
        LOG_CRITICAL << LOG_TAG_MOSTDRIVER << " Can NOT open k2lacsdaemon.info file. Wait for 5 secs." << LOG_ENDL;
        QTimer::singleShot(DAEMON_WAIT_TIME*5, this, SLOT(onTimerTimeout()));

        delete file;
        return;
    }

    QTextStream in(file);
    QString line;

    do
    {
        line = in.readLine();

        if(line.contains("Connected"))
        {
            if(line.contains("TRUE"))
            {
                LOG_HIGH << LOG_TAG_MOSTDRIVER << " ACS-AMF was connected" << LOG_ENDL;
                m_Initialized = true;
            }
            else
            {
                // ACS-AMF was not connected rarely, but why?
                // service thread of k2lacsdaemon process must update timer within 3 seconds.
                // If not, ACS-AMF bridge will be disconnected.
                if(m_WaitFor10s == false)
                {
                    // Sometimes, the bridge is disconnected during 7~8 seconds from experiments
                    // because the other processes take CPU.
                    // Need to wait for about 10 seconds, to retry connection 2~3 times.
                    LOG_CRITICAL << LOG_TAG_MOSTDRIVER << " ACS-AMF was disconnected. Wait For 10 secs." << LOG_ENDL;
                    QTimer::singleShot(10000, this, SLOT(onTimerTimeout()));
                    m_WaitFor10s = true;

                    file->close();
                    delete file;
                    return;
                }
                else if( (m_WaitFor10s == true) && (m_ResetCount < DAEMON_RESET_COUNT) )
                {
                    LOG_CRITICAL << LOG_TAG_MOSTDRIVER << " ACS-AMF was disconnected. Reset MOST driver (" << m_ResetCount+1 << ")" << LOG_ENDL;
                    //m_Daemon->start("/app/bin/K2L-init restart");
                    m_Daemon->start("/app/bin/K2L-init start");
                    QTimer::singleShot(DAEMON_WAIT_TIME, this, SLOT(onTimerTimeout()));
                    m_WaitFor10s = false;
                    m_ResetCount++;

                    file->close();
                    delete file;
                    return;
                }                
            }
            break;
        }
    } while(!line.isNull());

    if(m_Initialized)
    {
        LOG_HIGH << LOG_TAG_MOSTDRIVER << " MOST driver is launched successfully" << LOG_ENDL;
        SetAcsBridgeState(true);

        emit DriverLoaded();

#ifdef _AGING_TEST_
        m_SystemReset = new CSystemReset();

        if(m_SystemReset != NULL)
        {
            m_SystemReset->StartTimer(m_SystemReset);
        }
#endif
    }
    else
    {
        if(m_ResetCount == DAEMON_RESET_COUNT)
        {
            LOG_CRITICAL << LOG_TAG_MOSTDRIVER << " Failed to start MOST driver. Kill k2lacsdaemon process" << LOG_ENDL;
            m_Daemon->start("/app/bin/K2L-init kill");
            SetAcsBridgeState(false);
        }
        else
        {
            // In case of error : File opend but no information in file
            QTimer::singleShot(500, this, SLOT(onTimerTimeout()));
        }
    }

    file->close();
    delete file;
}

void CMOSTDriver::onTimerTimeout()
{
	LOG_SIGNAL << LOG_TAG_MOSTDRIVER << " Call onTimerTimeout( ) " << LOG_ENDL;
	if(g_ACSDisconnByMHP == true)
	{
		LaunchDriver(1);
		g_ACSDisconnByMHP = false;
	}
	else
	{
		LaunchDriver(2);
	}
}

void CMOSTDriver::onDaemonWatherTimeout()
{
    CheckProcessRunning();
}

void CMOSTDriver::StartMonitoring()
{
    LOG_SIGNAL << LOG_TAG_MOSTDRIVER << " Start Monitoring MOST Driver " << LOG_ENDL;

    m_FSWatcher->addPath(DRIVER_INFO_PATH);

    m_DaemonProcPath.clear();

    QFile *file = new QFile(DRIVER_PID_PATH);

    // Exception handling
    if(file == NULL)
    {
        LOG_CRITICAL << LOG_TAG_MOSTDRIVER << " Can NOT monitor k2lacsdaemon" << LOG_ENDL;
        return;
    }

    if (!file->open(QIODevice::ReadOnly))
    {
        LOG_CRITICAL << LOG_TAG_MOSTDRIVER << " Can NOT open k2lacsdaemon.pid file" << LOG_ENDL;
    }
    else
    {
        QTextStream in(file);
        QString pid;

        pid = in.readLine();

        LOG_SIGNAL << LOG_TAG_MOSTDRIVER << " k2lacsdaemon PID " << pid << LOG_ENDL;

        pid.append("/cmdline");
        pid.insert(0, "/proc/");

        m_DaemonProcPath.append(pid);

        file->close();

        m_DaemonWather->start(500);
    }

    delete file;
}

void CMOSTDriver::StopMonitoring()
{
    LOG_SIGNAL << LOG_TAG_MOSTDRIVER << " Stop Monitoring MOST Driver " << LOG_ENDL;

    m_FSWatcher->removePath(DRIVER_INFO_PATH);

    m_DaemonWather->stop();
}

void CMOSTDriver::CheckProcessRunning()
{
	LOG_LOW << LOG_TAG_MOSTDRIVER << " call checkProcessRunning( ) " << LOG_ENDL;
	
	if(g_ACSDisconnByMHP == true)		// 2014.11.22 shy, VI 개선평가 , UVO menu 진입 불가 
	{
		emit MHPCrashed();
		return;
	}

	if( QFile::exists(m_DaemonProcPath) == false )
	{
		emit DriverCrashed();
		return;
	}    
}

void CMOSTDriver::onFileChanged(const QString& path)
{
    if( path.compare(DRIVER_INFO_PATH) == 0 )
    {
        QFile *file = new QFile(DRIVER_INFO_PATH);

        // Exception handling
        if(file == NULL)
        {
            return;
        }

        if (!file->open(QIODevice::ReadOnly))
        {
            LOG_CRITICAL << LOG_TAG_MOSTDRIVER << " Can NOT open k2lacsdaemon.info file. " << LOG_ENDL;
        }
        else
        {
            QTextStream in(file);
            QString line;

            do
            {
                line = in.readLine();

                if(line.contains("NetOn"))
                {
                    if(line.contains("TRUE"))
                    {
                        if( (m_NetOn == -1) || (m_NetOn == 0) )
                        {
                            LOG_HIGH << LOG_TAG_MOSTDRIVER << " Light is On." << LOG_ENDL;
                            m_NetOn = 1;
                        }
                    }
                    else
                    {
                        if( (m_NetOn == -1) || (m_NetOn == 1) )
                        {
                            LOG_CRITICAL << LOG_TAG_MOSTDRIVER << " Light is Off." << LOG_ENDL;
                            m_NetOn = 0;
                        }
                    }
                }
                else if(line.contains("ConfigOk"))
                {
                    if(line.contains("TRUE"))
                    {
                        if( (m_ConfigOk == -1) || (m_ConfigOk == 0) )
                        {
                            LOG_HIGH << LOG_TAG_MOSTDRIVER << " Network State is OK." << LOG_ENDL;
                            m_ConfigOk = 1;
                        }
                    }
                    else
                    {
                        if( (m_ConfigOk == -1) || (m_ConfigOk == 1) )
                        {
                            LOG_CRITICAL << LOG_TAG_MOSTDRIVER << " Network State is NotOK." << LOG_ENDL;
                            m_ConfigOk = 0;
                        }
                    }
                }
                else if(line.contains("Connected"))  // State of ACS-AMF Bridge
                {
                    if(line.contains("TRUE"))
                    {
                        SetAcsBridgeState(true);

                        if( (m_BridgeConnected == -1) || (m_BridgeConnected == 0) )
                        {
                            LOG_HIGH << LOG_TAG_MOSTDRIVER << " ACS-AMF is connected. " << LOG_ENDL;
                            m_BridgeConnected = 1;
                        }
                    }
                    else
                    {
                        SetAcsBridgeState(false);

                        if( (m_BridgeConnected == -1) || (m_BridgeConnected == 1) )
                        {
                            LOG_HIGH << LOG_TAG_MOSTDRIVER << " ACS-AMF is disconnected. " << LOG_ENDL;
                            m_BridgeConnected = 0;
                        }
                    }
                }
                else
                {
                    /* No Statement */
                }
            } while(!line.isNull());

            file->close();
        }

        delete file;
    }
}

#ifdef _AGING_TEST_
CSystemReset::CSystemReset()
{
    LOG_INIT_V2(LOG_PREFIX_MOST)
}

CSystemReset::~CSystemReset()
{
}

void CSystemReset::StartTimer(QObject *receiver)
{
    LOG_HIGH << " Start Reset Timer" << LOG_ENDL;
    QTimer::singleShot(5*60*1000, receiver, SLOT(onResetTimeout()));
}

void CSystemReset::onResetTimeout()
{
    LOG_HIGH << " Reset Timer is timeout" << LOG_ENDL;

    QDBusConnection conn = QDBusConnection::sessionBus();
    QDBusMessage msg;

    msg = QDBusMessage::createMethodCall("com.lge.car.micom", "/System", "com.lge.car.micom.system", "LgeDBus_ResetHWPeripheral");
    QByteArray array;
    array.append((char)0);
    array.append((char)0);

    msg << qVariantFromValue(array);
    conn.send(msg);
}
#endif
