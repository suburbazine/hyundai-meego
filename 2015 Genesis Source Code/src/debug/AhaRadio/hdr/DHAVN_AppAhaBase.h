/*
 ******************************************************************
 *        COPYRIGHT  2011, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :                                   *
 * PROGRAMMER : Vandana Ahuja                                     *
 * DATE       : 21/12/2011                                        *
 * VERSION    : 0.1                                               *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module is responsible for following activities
 *      * Check availability of device
 *      * Connect to Aha
 *      * Keep listening to device state and disconnect when
 *        required
 *      * Connect to RPM for Audio, if diconnected, disconnect
 *        from Aha also
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  21/12/2011  0.1 Vandana Ahuja   header file created
 *
 ******************************************************************
 */

#ifndef DHAVN_APPAHABASE_H
#define DHAVN_APPAHABASE_H

#include <QDeclarativeView>
#include <QTranslator>

//hsryu_0322_TRANSPORT_CAN
#include <DHAVN_QCANController_ApplnTxRx.h>
//#include <QDBusInterface>

#ifdef FRAMEWORK_ENABLED
#include <uistate_manager_def.h>
#include <DHAVN_AppFramework_AppEngineQML_Def.h>
#include <DHAVN_AppFramework_QML_Def.h>
#include <DHAVN_QRPMClient.h>
#include <DHAVN_LogUtil.h>
#endif

#include "ahainterface.h"
#include "DHAVN_AppAhaTrack.h"
#include "DHAVN_AppAha_Def.h"
#include "DHAVN_AppAhaMenus.h"
#include "DHAVN_AppAhaDeviceListInfo.h"
#include "ahaSoundInterface.h"

#ifndef FRAMEWORK_ENABLED
typedef quint16 LANGUAGE_T;
typedef quint16 APP_ID_T;
#define APP_AHA 1
#endif

// for SW WDT
#include <signal.h>
#include <bits/siginfo.h>
//


enum
{
AHA_EVT_KEY_JOG_CENTER = 0,
AHA_EVT_KEY_JOG_ARROW_UP,
AHA_EVT_KEY_JOG_ARROW_DOWN,
AHA_EVT_KEY_JOG_ARROW_LEFT,
AHA_EVT_KEY_JOG_ARROW_RIGHT,
AHA_EVT_KEY_JOG_TOP_RIGHT,
AHA_EVT_KEY_JOG_BOTTOM_RIGHT,
AHA_EVT_KEY_JOG_TOP_LEFT,
AHA_EVT_KEY_JOG_BOTTOM_LEFT,
AHA_EVT_KEY_JOG_WHEEL_LEFT,
AHA_EVT_KEY_JOG_WHEEL_RIGHT,
AHA_EVT_KEY_JOG_WHEEL_LONG_LEFT,
AHA_EVT_KEY_JOG_WHEEL_LONG_RIGHT,
AHA_EVT_KEY_JOG_NONE
};

enum
{
    AHA_CURRENT_VIEW_NONE = 0,
    AHA_CURRENT_VIEW_CONNECTINGVIEW,
    AHA_CURRENT_VIEW_TRACKVIEW
};

class  CAhaStateHandler;
class  CExternalNotifier;

#ifdef FRAMEWORK_ENABLED
class CAppAhaBase : public AppEngineQML
#else
class CAppAhaBase : public QObject
#endif
{
    Q_OBJECT   

public:

 enum  HMIState
    {
    AppNotLaunched = 0, // Not yet launched
    AppInFG ,           // Running ion fore ground
    AppInBG ,           // Running in background
    AppExitState        // Application exited
    };

    /**
     *******************************************************************
     *
     * Constructor for CAppAhaBase module. This will construct
     * other c++ modules in Aha app qt layer.
     *
     * @param[in]   aQmlView    The view using which all screens will
     *                          be displayed
     * @param[in]   nLanguage   The default language to be used by
     *                          application
     * @param[in]   pParent     The parent screen for the application.
     *******************************************************************
     */
    #ifdef FRAMEWORK_ENABLED
    CAppAhaBase(QDeclarativeView &aQMLView, ECountryVariant variant, LANGUAGE_T nLanguage, int nVehicleType, QObject *pParent = NULL);
	#else
    CAppAhaBase(QDeclarativeView &aQMLView, LANGUAGE_T nLanguage, QObject *pParent = NULL );
    #endif
	
    ~CAppAhaBase();

    /**
     *******************************************************************
     * This function will be called when application needs to be exited
     * as a result of back request from track view. All cleanup will be
     * performed in this function.
     *******************************************************************
     */

    Q_INVOKABLE bool isFrontLCD() { return m_isFrontLCD; }

    Q_INVOKABLE void ExitApplication();
    Q_INVOKABLE void printQMLDebugString(QString str);
    Q_INVOKABLE void NavigateToPOI();
    Q_INVOKABLE void CallPOI();
    Q_INVOKABLE void ConnectToTheOtherIpod(int nSelectedDevice);

    // Maintain the current state as BG
    Q_INVOKABLE void HandleHomeKey();

    // Maintain the current state as BG
    Q_INVOKABLE void HandleBackKey();

    // Check the current status, request connect to RPM and Aha
    Q_INVOKABLE void Connect( int inSelectedIndex);

    // Check the current status, request disconnect to RPM and Aha
    Q_INVOKABLE void Disconnect();

    //hsryu_0613_fail_load_station
    Q_INVOKABLE void UpdateAhaForDisconnect();

    //copy from Pandora when upgrading from AppPandora version 62 to 71
    //Q_INVOKABLE void SetAhaStateActive();

    // Launches the sound settings ui
    Q_INVOKABLE void LaunchSoundSetting();

    // Set the status of option menu
    Q_INVOKABLE void SetOptionMenuVisibilty(bool inStatus){m_bOptionMenuVisible =  inStatus;}

    Q_INVOKABLE int getCurrentScreen() {return AppEngineBase::GetDisplayType();}

    //hsryu_0404_restore_playmode
    //Q_INVOKABLE void setCallingPlayState(bool inStatus) { bCallingPlayState = inStatus; }

#ifdef FRAMEWORK_ENABLED
    // Launch key pad setting

    Q_INVOKABLE void LaunchKeypadSettings();
	  
    // Get the country varient
    Q_INVOKABLE int GetCountryVariant(){ return (int)m_nCountryVariant; }
#endif
    /**
     *******************************************************************
     * Called at start of application for setting the language and
     * signalling to UI about current language. Also called when
     * there is language change event occurs
     *******************************************************************
     */
    void RetranslateUi();

    // Return the AhaMenus class Reference
    AhaMenus& GetAhaMenus();

    // Clear the cache on back soft key .
    void ClearCache();

    Q_INVOKABLE bool IsAVModeChanged(){return m_bAV_Mode_Changed;}
    //hsryu_0404_restore_playmode
    Q_INVOKABLE bool IsRunningInBG(){return m_bIsBackGround;}
    bool IsConnected(){return m_bAhaConnected;}
    //hsryu_0322_TRANSPORT_CAN
    Q_INVOKABLE bool IsCallingSameDevice(); //hsryu_0423_block_play_btcall

    bool IsClusterUpdate(){return m_bClusterUpdate;}//hsryu_0603_cluster_update
    void setClusterUpdate(bool value); //hsryu_0603_cluster_update

/*wsuk.kim jog_long_press*/
    int isJogKeyLongPress() { return m_isJogKeyLongPressed; }
    void setIsJogKeyLongPress(int isJogKeyLongPress) { m_isJogKeyLongPressed = isJogKeyLongPress; }
    Q_INVOKABLE int getIsJogKeyLongPress(){return m_isJogKeyLongPressed;}
/*wsuk.kim jog_long_press*/

    Q_INVOKABLE quint16 GetAhaGPSStatus(); //hsryu_0312_navi_exception
    //hsryu_0514_check_latitude_longitude_for_aha
    Q_INVOKABLE bool CheckLocateForNavigation();

    Q_INVOKABLE bool GetVariantRearUSB(){ return ( AppEngineBase::GetSystemVariant() & SYSTEM_VARIANT_REAR_USB ); } //wsuk.kim 130807 ISV_86633 variant RearUSB

    Q_INVOKABLE void OSDTrackInfo();

    // for SW WDT -->
    static void signalHandler (int signum);
    void signalConnection();
    static void setSignalHandlerObject (CAppAhaBase* m);
    static CAppAhaBase* SigM;
    void restart(int signum);

    Q_INVOKABLE void updateSampleRate();
    Q_INVOKABLE void openSoundChannel();

    // <--
    Q_INVOKABLE int getStringWidth( const QString &fullString,const QString &fontFamily,int pixelSize );    //wsuk.kim 130813 ITS_0182986 one by one text scroll on trackview
    Q_INVOKABLE void setCurrentView(int nCurrentView);          // by Ryu 20131001
    Q_INVOKABLE int getCurrentView();          // by Ryu 20131001

    Q_INVOKABLE void setSelectionEvent(bool bSelectionEvent);          // Ryu 20140207 ITS_0223871
    Q_INVOKABLE bool getSelectionEvent(){return m_bSelectionEvent;}          // Ryu 20140207 ITS_0223871

    Q_INVOKABLE void NoNetworkPopupState(bool bShowPopup);   //wsuk.kim 131204 ITS_212572 state network error, display error info on OSD.
    Q_INVOKABLE void stopTimers();  // added by Ryu : stop timers at Trackview : ITS0224000
    QSharedMemory m_SharedMemory;

private:
    // Handle AV_MODE has changed event .
    void handleAVModeChange(MODE_STATE_T mode, bool nAhaIsConnected);

    // Register to receive mode key event .
    void UpdateAhaAvailableStatus(bool inStatus);

    //Tan: added when upgrading from 124.1 to 5.35.2

    // Notify to UISH to acquire audio path .
    //void NotifyAudioPathChanged(EDeviceType selectedDevice);
    void NotifyAudioPathChanged();

    //Display OSD
    void SetOSDFlag(bool bSetOSD);
    bool m_bSetOSD;
    //hsryu_0328_clear_OSD
    void ClearOSD();
    void requestOSDTimer(int interval); //hsryu_0411_change_track_info
    void DisplayOSD(OSDDisplayType osdtype, QString &strOSDString);
    void noBackGroundDisplayOSD(OSDDisplayType osdtype, QString &strOSDString);   //wsuk.kim 130827 ITS_0183823 dimming cue BTN & display OSD instead of inform popup during BT calling.
    Q_INVOKABLE void OSDInfoAVOff(); //hsryu_0228 ITS # 0158377

//wsuk.kim NOT_OSD    Q_INVOKABLE void OSDInfoCannotSkip();
//wsuk.kim NOT_OSD    Q_INVOKABLE void OSDInfoCannotSkipBack();
    void tuneHandler( int eventId, const Event &aEvent );   //wsuk.kim TUNE

public slots:
    void OSDInfoReadySlot(QString &strOSDString);
//wsuk.kim NOT_OSD
    void OSDInfoCannotSkip();
    void OSDInfoCannotSkipBack();
    void OSDInfoCannotREW15();
    void OSDInfoCannotFW30();
    void OSDInfoCannotPlayBTCall();  //hsryu_0423_block_play_btcall
    void OSDTrackTimeShift(int nTimeShift);
//wsuk.kim NOT_OSD

    void OnTimeout_OSDEvent(); //hsryu_0411_change_track_info
    void OSDStationNameTuneUpDown(int nTuneIndex, bool bTrackVew);  //wsuk.kim TUNE_OSD
    //hsryu_0423_block_play_btcall
    //bool getConnectCallState() {return m_bConnectCall;} //wsuk.kim deactivate_call
    void OSDInfoNoNetwork();    //wsuk.kim 131204 ITS_212572 state network error, display error info on OSD.
    void OSDInfoConnectingtoaha();

    void handleNotifyAudioPath();   //wsuk.kim UNMUTE
    QString getConnectTypeName();

    bool getDRSTextScrollState() {return m_bDRSTextScroll;}    //wsuk.kim text_scroll
    int getNetworkStatus() { return m_NetworkStatusCode; } //wsuk.kim no_network
    bool getContinueLastView() { return m_bContinueLastView; }

    void onDelayedPlayTimeOut(void);
signals:

    // Signal to UI for translating to new language set
    void retranslateUi(int languageId); // required for interaction with QML part

    void jumpToPresets();

    // Signal to UI about the device status as connected
    void connectionStatusChanged(const int isConnected);

    // Signal to QML for bringing the required UI in front on Foreground event
    void foreground();

    void connectToIpod(int selectedDevice);

    // Signal to QML for putting the UI in background
    void background();

    // Signal to QML about the Network Error.
    void networkError();

    // Signal to QML about the key event.

    void handleSkipEvent(bool isJogKey);
    void handleSkipBackEvent(bool isJogKey);
    void handleRewind15Event(bool isJogKey);
    void handleForward30Event(bool isJogKey);
    void signalSeekTrackKey( int arrow, int status );

    // Signal to QML about the key event.
    void handleRewindEvent();

    // Signal to QML about the jog hard key pressed.
    void jogHardkeyPressed(const int inJogKey);

    // Signal to QML about jog key event.
    void jogsendKeyTovc(const int inJogKey);

    // Signal to QML about menu key event.
    void menuKeyPressed(bool isJogMenuKey);

    //Signal to QML about back key event.
    void backKeyPressed();

    // Signal to QML about jog key event.
    void handleError(const quint16 inAhaError);

    // Signal to QML about VR Pause command.
    void playCommandFromVR(int inPayLoad);

    // Signal to QML about VR thumb up/down command.
    void ratingCommandFromVR(int inPayLoad);

    // Signal to QML about shuffle on/off command.
    void shuffleCommandFromVR();

    //wsuk.kim SEEK_TRACK
    void handleSeekTrackReleased();

//wskim TUNE
    void signalTuneReleased();
    void handleTuneDownEvent(uint arrow);
    void handleTuneUpEvent(uint arrow);
    void handleTuneCenterEvent();
    void handelTuneCenterPressedEvent();    //wsuk.kim 130806 ITS_0182685 depress when pressed with CCP/Tune Knob
//wskim TUNE

//wsuk.kim WHEEL_SEARCH
    void handleCCPDownEvent(uint arrow);
    void handleCCPUpEvent(uint arrow);
//wsuk.kim WHEEL_SEARCH

    //hsryu_0322_TRANSPORT_CAN
    void Can_data_send(const SQCanFrame &canframe);
    //hsryu_0423_block_play_btcall
    //void handleCallAllow(bool bConnectCall);  //wsuk.kim deactivate_call
    void handleCallStart(bool bCallingState);    //wsuk.kim 130827 ITS_0183823 dimming cue BTN & display OSD instead of inform popup during BT calling.
    void networkErrorResume();  //wsuk.kim no_network

    void handleTextScrollState(bool isTextScrolling);    //wsuk.kim text_scroll
    void handlePlayAnimationState(bool isPlayAnimation);

    void bufferingError();  //wsuk.kim 130904 ITS_0182092 repeat buffering

    void handleListViewPosition();
    void activeViewVisible(bool isVisible);  //wsuk.kim 131016 ITS_195123 Temporal Mode
    void handleHideErrorPopup();        // Ryu 20140227

    void handleShowPopup(int reason);
    void handleDisplayOff(bool isDisplayOff);  // ITS 217070
public slots:

    void ahaSampleRateChangedSlot(int nSampleRate);

    // Handle the events sent by UIStateHandler
#ifdef FRAMEWORK_ENABLED
    // borrowed this from pandora implementation
    //{ modified by esjang 2012.12.22 for SWAP DC
    //void NotifyFromUIStateManager( Event aEvent );
    virtual void NotifyFromUIStateManager( Event aEvent );
    virtual void NotifyFromUIStateManagerRear ( Event aEvent );
    virtual void NotifyFromUIStateManagerCommon ( Event aEvent );
    void NotifyFromUIStateManagerGeneric( Event aEvent );// added by esjang 2012.12.22 for SWAP DC
    //} modified by esjang 2012.12.22 for SWAP DC
#endif

    // notify the AVWidget with song name and current time.
    void NotifyAVWidget();

    void OnNoActiveStationSlot();

    // stop notify the AVWidget.
    void StopNotifyAVWidget();

    //  The result of request from Aha MW
    //  All failed requests related to network to be handled here only.
    void OnRequestCompleted(const ERequestType inRequestType, const EAhaError inAhaError);

    // The result of request from RPM Client
#ifdef FRAMEWORK_ENABLED
    void OnRPMAcquireResponseReceived(TAudioMode tAM ,TAudioSink tAS);
#endif
    //hsryu_0322_TRANSPORT_CAN
    void ontrackTitleReceived(QString title);
    //hsryu_0422_can_test
    void onClearTpMessage();
//wsuk.kim no_network
    void onNetworkResume();
    void onNetworkStatus(const int nNetworkStatus);
//wsuk.kim no_network

    void onUnmute();

private:
    //hsryu_0422_can_test
    void SendClusterData();
    void SendClusterData_OpStateOnly();
    void sendCanTpMessage(quint16 id, QString data);
    //hsryu_0422_can_test
    void sendClearTpMessage();

    // App ID for this application
    virtual APP_ID_T GetThisAppId() const { return APP_AHA_RADIO; }

    // Translator user
    QTranslator *m_pTranslator;

#ifdef FRAMEWORK_ENABLED
    /** Country Varient */
    ECountryVariant m_nCountryVariant;
    /** Current language */
    LANGUAGE_T m_nLanguage;
#endif

    // Handle the error , if any error exists notify to qml layer
    void HandleIfError(const EAhaError inAhaError);

    /** Aha MW interface */
    //suryanto.tan
    CAhaInterface m_AhaInterface;
    CSoundInterface m_SoundInterface;

    //hsryu_0322_TRANSPORT_CAN
    CQCANController_ApplnTxRx* m_pQCanController;
    QDBusInterface* m_pCanTPInterface; // added by esjang 2012.11.20 for CAN TP Message

    //hsryu_0514_check_latitude_longitude_for_aha
    bool isLocateInLatitude(double latitude);
    bool isLocateInLongitude(double longitude);

    /** Curent connected device index from the List */
    /** Signal handler */
    CAhaStateHandler* m_pSignals ;
    /** HMIState  handler */
    HMIState m_eHMIState;
    /** Aha Connected status */
    bool m_bAhaConnected;
    /** Logger instance */
    USE_LOG_UTIL
    /** Aha Menus */
    AhaMenus m_AhaMenus;
    /** Av mode has changed , play aha on FG event */
    bool  m_bAV_Mode_Changed;
    /** Aha is in Back ground*/
    bool m_bIsBackGround;
	/** Option menu visible status */
    bool m_bOptionMenuVisible;

    int m_isJogKeyLongPressed; /*wsuk.kim jog_long_press*/

    /** phone call is comming, end */
    bool m_bCallingState; //hsryu_0322_TRANSPORT_CAN
    bool isSoundOpen;//hsryu_0422_can_test
    bool bOSD_DISPLAY; //hsryu_0621_ITS_0174694

    //hsryu_0404_restore_playmode
    qint16 m_nPreviousPlayPause;
    //bool bCallingPlayState; //check status in call.
    bool bCheckInitOnce; //OnRequestCompleted come after EVT_REQUEST_BG

    bool m_bClusterUpdate; //hsryu_0603_cluster_update

    bool ReadDataFromNaviShareMemory(unsigned char &posFix);
    //hsryu_0312_navi_exception
    //AHA_GPS_STATUS_T GetAhaGPSStatus();

    bool m_b_MMC_removed;
    //hsryu_0423_block_play_btcall
    //bool m_bConnectCall;  //wsuk.kim deactivate_call
    bool m_bMuteState; //wsuk.kim UNMUTE
    QString m_connect_type_name;
    
    //front back usb
    EDeviceType m_CurrentAhaDevice;
    void ResumeOnSameIpod();

    bool m_bDRSTextScroll;  //wsuk.kim text_scroll
    int m_NetworkStatusCode;   //wsuk.kim no_network
    bool m_bDuplicate;
    bool m_bContinueLastView;
    bool m_isFrontLCD;
    int m_nCurrentView;
    bool m_bBeforeSoundSetting; //wsuk.kim 131106 temporal mode ON after launch sound setting, did not menu hide.
    bool m_bShowNoNetworkPopup; //wsuk.kim 131204 ITS_212572 state network error, display error info on OSD.
    bool m_bSelectionEvent;        // Ryu 20140207 ITS_0223871

    // av mode
    void DelayedPlay();
    void setUnMuteForBTAha();
    MODE_STATE_T m_PrevAvMode;
    MODE_STATE_T m_CurrentAvMode;
    bool m_bUnmuteSent;

    QTimer* m_DelayedPlayTimer;

    HEADUNIT_POWER_STATE_T m_HeadUnitPowerState;
};

class LocalizeTrigger : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString empty READ empty NOTIFY retrigger)

public:
    QString empty() { return QString(); }

signals:
    void retrigger();
};

#endif // DHAVN_APPAHABASE_H
