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
 * 08 Aug 2011  Neelima Khedad           Created Instance of class CDLNAShadow Control
 * 09 Aug 2011  Sajeer Shamsudeen        Created Instance of class CDVDFBlock Control
 * 09 Aug 2011  venkatesh Aeturi         Created Instance of class CAuxInFBlock Control
 * 10 Aug 2011  venkatesh Aeturi         Created Instance of class CDMBtunerFBlock Control
 * 22 Aug 2011  Neelima Khedad           Created Instance of class CHMIFBlock_ShadowControl
 * 29 Aug 2011  Neelima Khedad           Created Instance of class CIBOXShadowControl
 * 06 Sep 2011  venkatesh Aeturi         Created Instance of class CDMBTunerFblockControl,
 *                                       CHMIFBlock_ShadowControl,CIBOXShadowControl,
 *                                       CSpeechRecognitionFblockControl,CSatRadioFblockControl,
 *                                       CBluetoothFblockControl, CSWDLFBlockShadowControl,
 *                                       CVehicleFblockControl,CVirtualDiscFblockControl,
 *                                       CTelephoneShadowControl
 * 06 Sep 2011  Shiva Kumar              Created Instance of class CSWDLFBlockShadowControl
 * 12 Sep 2011  Amit koparde             Created Instance of class CDABTunerFblockControl
 * 12 Sep 2011  Venkatesh Aeturi         Created Instance of class CNavigationFblockControl.
 * 13 Sep 2011  Amit Koparde             Created Instance of class CDiagnosisFblock_ShadowControl.
 * 30 Oct 2012  Shiva Kumar              Changes for CR15173
 ******************************************************************
 */

#ifndef DHAVN_MOSTMANAGER_MOSTMANAGER_H
#define DHAVN_MOSTMANAGER_MOSTMANAGER_H

#include <QThread>
#include <QProcess>
#include <QTimer>

#include <DHAVN_LogUtil.h>
#include <DHAVN_AppFramework_IPC.h>

#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_ACSApplication.h"
#include "DHAVN_MOSTManager_DLNAShadowControl.h"
#include "DHAVN_MOSTManager_AuxInFblockControl.h"
#include "DHAVN_MOSTManager_HMIFBlock_ShadowControl.h"
#include "DHAVN_MOSTManager_IBOXShadowControl.h"
#include "DHAVN_MOSTManager_SpeechRecognitionFblockControl.h"
#include "DHAVN_MOSTManager_BluetoothFblockControl.h"
#include "DHAVN_MOSTManager_TelephoneShadowControl.h"
#include "DHAVN_MOSTManager_SoftwareDownloadFBlockShadowControl.h"
#include "DHAVN_MOSTManager_NavigationFBlockControl.h"
#include "DHAVN_MOSTManager_DiagnosisFBlock_ShadowControl.h"
#include "DHAVN_MOSTManager_DMBTunerFBlockControl.h"
#include "DHAVN_MOSTManager_SatRadioFBlockControl.h"
#include "DHAVN_MOSTManager_LogFBlockControl.h"


using namespace k2l::acs::samples;
using namespace k2l::platform::system;

#define LOG_TAG_MOSTMANAGER     "MOSTManager"
#define LOG_TAG_ADAPTORTHREAD   "Adaptor Thread"
#define LOG_TAG_MOSTDRIVER      "MOSTDriver"

typedef struct FBLOCK_INIT_T
{
    bool SR_FBLOCK        :1;
    bool DLNA_SHADOW      :1;
    bool AUXIN_FBLOCK     :1;
    bool BT_FBLOCK        :1;
    bool TELEPHONE_SHADOW :1;
    bool DIAG_FBLOCK      :1;
    bool SWDL_FBLOCK      :1;
    bool HMI_FBLOCK       :1;
    bool NAVI_FBLOCK      :1;
    bool IBOX_SHADOW      :1;
    bool DMB_FBLOCK       :1;
    bool SATRADIO_FBLOCK  :1;
    bool LOG_FBLOCK       :1;
} FBLOCK_INIT;

extern bool g_BridgeConnected;
extern QMutex g_BridgeMutex;
extern bool g_AcsDaemonConnected;
extern QMutex g_AcsDaemonMutex;

void SetAcsBridgeState(bool state);
bool GetAcsBridgeState(void);
void SetAcsDaemonState(bool state);
bool GetAcsDaemonState(void);

class CMOSTDriver;
class CSystemReset;
/**
 * This class is derived form CIACSApplicationEvents to receive the events from
 * marshalled application queue
 */

class CMOSTManager : public QThread, public CIACSApplicationEvents
{
    Q_OBJECT
    USE_LOG_UTIL

    /** Pointer Object of marshalled application */
    CMarshaledACSApplication *m_Application;    

    CManualResetEvent m_MessagesEvent;

    CMOSTDriver *m_MostDriver;

    FBLOCK_INIT m_FBlockInitialized;

    bool m_Running;
    QMutex m_RunningMutex;
    bool retVal;
    void run();

public:
    CDLNAShadowControl *m_CDLNAShadowControl;
    CAuxInFblockControl *m_CAuxInFBlockControl;
    CHMIFBlockShadowControl *m_CHMIFBlockShadowControl;
    CIBOXShadowControl *m_CIBOXShadowControl;
    CSpeechRecognitionFblockControl *m_CSpeechRecognitionFblockControl;
    CBluetoothFblockControl *m_CBluetoothFblockControl;
    CSWDLFBlockShadowControl *m_CSWDLFBlockControl;
    CTelephoneShadowControl *m_CTelephoneShadowControl;
    CNavigationFblockControl *m_CNavigationFblockControl;
    CDiagnosisFBlockShadowControl *m_CDiagnosisFBlockShadowControl;
    CDMBTunerFblockControl *m_CDMBTunerFblockControl;
    CSatRadioFblockControl *m_CSatRadioFblockControl;    
    CLogFblockControl *m_CLogFblockControl;

public:
    CMOSTManager();
    ~CMOSTManager();

    void Launch();
    bool Configure();
    //void Execute();
    void SetRunningState(bool state);
    bool GetRunningState();

    virtual void MessagesAvailable();
    virtual void NoMessagesAvailable();

    CDLNAShadowControl* GetDLNAShadowControlInstance();
    CAuxInFblockControl* GetAuxInControlInstance();
    CHMIFBlockShadowControl* GetHMIFBlockShadowControl();
    CIBOXShadowControl* GetIBOXShadowControlInstance();
    CSpeechRecognitionFblockControl* GetSpeechRecognitionControlInstance();
    CBluetoothFblockControl* GetBluetoothControlInstance();
    CNavigationFblockControl* GetNavigationControlInstance();
    CTelephoneShadowControl* GetTelephoneShadowControlInstance();
    CSWDLFBlockShadowControl* GetSWDLFBlockControlInstance();
    CDiagnosisFBlockShadowControl* GetDiagnosisFBlockShadowControlInstance();
    CDMBTunerFblockControl* GetDMBTunerFblockControlInstance();
    CSatRadioFblockControl* GetSatRadioFblockControlInstance();


private slots:
    void onDriverLoaded();
    void onDriverCrashed();
	void onMHPCrashed ();	// 2014.11.22 shy, VI 개선평가 , UVO menu 진입 불가 
};


/**
 * This class is a thread which registers all adaptor API and signals
 * on Dbus and waits for events from QtDbus
 */
class CAdaptorThread : public QObject
{
    Q_OBJECT
    USE_LOG_UTIL

    CMOSTManager *m_MostManagerContext;

    AppFrameworkIPC *m_AppFrameworkIPC;

public:
    CAdaptorThread(CMOSTManager *m_context);
    ~CAdaptorThread();

    void Execute();

    // For restart whenever abnormal termination
    void ConnectToUISH();
    void Restart(int signum);
    void SignalConnection();
    static void SignalHandler(int signum);
    static void SetSignalHandlerObject(CAdaptorThread *object);
    static CAdaptorThread *g_SigReceiverObject;
};

class CMOSTDriver : public QObject
{
    Q_OBJECT
    USE_LOG_UTIL

    QProcess *m_Daemon;

    QFileSystemWatcher *m_FSWatcher;
    QTimer *m_DaemonWather;

    bool m_Initialized;
    bool m_WaitFor10s;
    int m_ResetCount;

    int m_NetOn;
    int m_ConfigOk;
    int m_BridgeConnected;

    QString m_DaemonProcPath;

#ifdef _AGING_TEST_
    CSystemReset* m_SystemReset;
#endif

private:
    void CheckProcessRunning();

public:
    CMOSTDriver();
    ~CMOSTDriver();

    void LaunchDriver(int mode);
    void StartMonitoring();
    void StopMonitoring();
    void ReInitialize();

signals:
    void DriverLoaded();
    void DriverCrashed();
	void MHPCrashed();	// 2014.11.22 shy, VI 개선평가 , UVO menu 진입 불가 
    
private slots:
    void onFileChanged(const QString& path);
    void onTimerTimeout();
    void onDaemonWatherTimeout();
};

#ifdef _AGING_TEST_
class CSystemReset : public QObject
{
    Q_OBJECT
    USE_LOG_UTIL

public:
    CSystemReset();
    ~CSystemReset();

    void StartTimer(QObject *receiver);

public slots:
    void onResetTimeout();
};
#endif

#endif // DHAVN_MOSTMANAGER_MOSTMANAGER_H
