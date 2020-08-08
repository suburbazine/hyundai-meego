/*
 ******************************************************************
 *        COPYRIGHT 2011, LG Electronics.
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
 *      * Connect to Pandora
 *      * Keep listening to device state and disconnect when
 *        required
 *      * Connect to RPM for Audio, if diconnected, disconnect
 *        from Pandora also
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  21/12/2011  0.1 Vandana Ahuja   header file created
 *
 ******************************************************************
 */


#ifndef DHAVN_APPPANDORABASE_H
#define DHAVN_APPPANDORABASE_H

#ifdef FRAMEWORK_ENABLED
#include <uistate_manager_def.h>
#include <DHAVN_AppFramework_AppEngineQML_Def.h>
#include <DHAVN_AppFramework_QML_Def.h>
#include <DHAVN_QRPMClient.h>
#endif

#include <DHAVN_AppPandoraTrack.h>
#include <DHAVN_AppPandoraSearch.h>
#include <DHAVN_AppPandoraStationList.h>
#include <QDeclarativeView>
#include <QTranslator>
#include <DHAVN_LogUtil.h>
#include <DHAVN_Pndr_Defs.h>
#include <DHAVN_PndrInterface.h>
#include <QList>
#include <QVariant>
#include "DHAVN_AppPandora_Def.h"
#include "DHAVN_AppPandoraMenus.h"
#include <QStringList>
#include <QObject>
#include "DHAVN_AppPandoraDeviceListInfo.h"
//{ added by esjang 2013.03.18 for Preventing Abnormal termination(SW WDT)
#include <signal.h>
#include <bits/siginfo.h>
//} added by esjang 2013.03.18 for Preventing Abnormal termination(SW WDT)

#include <libusb-1.0/libusb.h>
#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <QTimer>
#include "PipeLineThread.h"
#include <QmlStatusBar/osdwriter.h> // added by esjang 2013.05.13 for Update Status bar
#include <QDateTime> //added by esjang
#include <DHAVN_SharedDataProvider.h> //added by jyjeon 2014-03-06 for VR
#include <QtCore>
#ifdef PANDORA_REAR_ENABLED_FOR_CES
#include <QHash> // added by esjang 2013.12.31 for CES 
#endif

#ifndef FRAMEWORK_ENABLED
typedef quint16 LANGUAGE_T;
typedef quint16 APP_ID_T;
#define APP_PANDORA 1
#endif

//enum
//{
//PANDORA_EVT_KEY_JOG_CENTER = 0,
//PANDORA_EVT_KEY_JOG_ARROW_UP,
//PANDORA_EVT_KEY_JOG_ARROW_DOWN,
//PANDORA_EVT_KEY_JOG_ARROW_LEFT,
//PANDORA_EVT_KEY_JOG_ARROW_RIGHT,
//PANDORA_EVT_KEY_JOG_TOP_RIGHT,
//PANDORA_EVT_KEY_JOG_BOTTOM_RIGHT,
//PANDORA_EVT_KEY_JOG_TOP_LEFT,
//PANDORA_EVT_KEY_JOG_BOTTOM_LEFT,
//PANDORA_EVT_KEY_JOG_WHEEL_LEFT,
//PANDORA_EVT_KEY_JOG_WHEEL_RIGHT,
//PANDORA_EVT_KEY_JOG_WHEEL_LONG_LEFT,
//PANDORA_EVT_KEY_JOG_WHEEL_LONG_RIGHT,
//PANDORA_EVT_KEY_JOG_NONE
//};

class  CPndrStateHandler;
class  CExternalNotifier;

//{ modified by yongkyun.lee 2014-03-11 for :  BT streaming Mute-play control
typedef struct
{
    MODE_STATE_T prevMode;
    MODE_STATE_T currMode;
    MODE_STATE_T lastPandora;//{ modified by yongkyun.lee 2014-04-06 for : ITS 233641
    bool         firstModeChang;
}modeStateHistory;
//} modified by yongkyun.lee 2014-03-11 
//{ modified by yongkyun.lee 2014-02-18 for : 
class POPUPEnums : public QObject
{
    Q_OBJECT
    Q_ENUMS(PopupIDPnd);

public:
    
    typedef enum   
    {   
        POPUP_PANDORA_UNDEFINED = -1,
        POPUP_PANDORA_CALLING_STATE,
        POPUP_PANDORA_CANNOT_BE_MANIPULATED,
        POPUP_PANDORA_DRIVING_RESTRICTION,
        POPUP_PANDORA_INCOMPATIABLE_VERSION,
        POPUP_PANDORA_LOG_IN,
        POPUP_PANDORA_MAINTENANCE,
        POPUP_PANDORA_MAX_SKIP_LIMIT,
        POPUP_PANDORA_REWIND_NOT_AVAILABLE,
        POPUP_PANDORA_SEARCH_SELECT_FAILED,
        POPUP_PANDORA_STATION_DOES_NOT_EXIST,
        POPUP_PANDORA_STATION_LIST_IS_FULL,
        POPUP_RECEIVING_STATIONS,

        POPUP_ARGUMENT_PRESSED_INVALID,/*	Argument Passed are invalid.	*/
        POPUP_BOOKMARK_FAILED,          /*	Bookmark has failed.	*/
        POPUP_DELETION_STATION_FAILED, /*	Deletion of the station has failed. */
        POPUP_EMPTY_INPUT_CRITERIA,    /*	Empty OR doesn't follow the input criteria. */
        POPUP_EXTENDED_SEARCH_FAILED,  /*	Extended search has failed. */
        POPUP_TOKEN_STATION_NOT_VALID, /*	Given token for station is not valid.	*/
        POPUP_INITALIZATION_FAILED,    /*	Initialization failed.	*/
        POPUP_NO_PHONE_CONNECTED,      /*	No Phone is Connected.	*/
        POPUP_PANDORA_DISCONNECTED,    /*	Pandora disconnected.\nPlease check your mobile device	*/
        POPUP_REQUESTED_STATION_RANGE_NOT_VALID,/*	Requested Station list range is not valid.	*/
        POPUP_REQUIRED_MEMORY_NOT_ALLOCATED,/*	Required Memory not allocated.	*/
        POPUP_SESSION_ALREADY_EXIT,    /*	Session already exit	*/
        POPUP_TIMEOUT_ERROR,           /*	Timeout error.	*/
        POPUP_TRACK_EXPLAIN_REQUEST_FAILDE,/*	Track Explain Request has failed.	*/
        POPUP_TRACK_RATING_FAILED,     /*	Track Rating has failed.	*/
        POPUP_URL_NOT_SUPPORTED,       /*	URL not supported	*/
    }PopupIDPnd;
};
//} modified by yongkyun.lee 2014-02-18 

#ifdef FRAMEWORK_ENABLED
class CAppPandoraBase : public AppEngineQML
#else
class CAppPandoraBase : public QObject
#endif
{
    Q_OBJECT   
    Q_PROPERTY(bool scrollingTicker READ getScrollingTicker NOTIFY scrollingTickerChanged)

    Q_PROPERTY(int currView WRITE setCurrentView)

    Q_PROPERTY(int languageId READ getlanguageId)


public:

    Q_INVOKABLE bool isFrontLCD() { return m_isFrontLCD; }

    Q_INVOKABLE int getlanguageId(){ return (int)m_nLanguage;}

    //{ modified by yongkyun.lee 2014-06-18 for :  ITS 240298
    typedef enum   
    {	
        TEMPORAL_UNDEFINED = 0,
        TEMPORAL_MUTE,
        TEMPORAL_UNMUTE,
    }TempMuteState;
    //} modified by yongkyun.lee 2014-06-18 
    
    enum  HMIState
    {
        AppNotLaunched = 0, // Not yet launched
        AppInFG ,           // Running ion fore ground
        AppInBG ,           // Running in background
        AppExitState        // Application exited
    };

    enum ActiveView
    {
        ENoView = 0,
        EConnectingView = 1,
        ETrackView = 2 ,
        EStationListView = 3,
        ESearchView  = 4 ,
        EStationEntryView = 5 ,
        EExplainationView =6 ,
        ENoActiveStation = 7 ,
        EErrorView = 8 ,
        EDiscoonectedView = 9
    };
    /**
     *******************************************************************
     *
     * Constructor for CAppPandoraBase module. This will construct
     * other c++ modules in Pandora app qt layer.
     *
     * @param[in]   aQmlView    The view using which all screens will
     *                          be displayed
     * @param[in]   nLanguage   The default language to be used by
     *                          application
     * @param[in]   pParent     The parent screen for the application.
     *
     * @return     None
     *
     *******************************************************************
     */
    #ifdef FRAMEWORK_ENABLED
    CAppPandoraBase(QDeclarativeView &aQMLView, ECountryVariant variant, LANGUAGE_T nLanguage, int nVehicleType, QObject *pParent = NULL ); //add vehicle type for DH, 16MY, DH PE wonseok.heo 2014.12.29
	#else
    CAppPandoraBase(QDeclarativeView &aQMLView, LANGUAGE_T nLanguage, int nVehicleType, QObject *pParent = NULL );
    #endif
	
    /**
     *******************************************************************
     *
     * Destructor for the class. Expected to be called ONLY during
     * device shutdown
     *
     * @param   None
     *
     * @return  None
     *
     *******************************************************************
     */
    ~CAppPandoraBase();

    /**
     *******************************************************************
     * This function will be called when application needs to be exited
     * as a result of back request from track view. All cleanup will be
     * performed in this function.
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
    //Q_INVOKABLE void ExitApplication();
	Q_INVOKABLE void ExitApplication(bool isJogDial); //modified by esjang 2013.06.21 for Touch BackKey
	

    /**
     *******************************************************************
     * Maintain the current state as BG
     *
     * @param   None
     *
     * @return  None
     *
     *******************************************************************
     */
    Q_INVOKABLE void HandleHomeKey();

    /**
     *******************************************************************
     * Maintain the current state as BG
     *
     * @param   None
     *
     * @return  None
     *
     *******************************************************************
     */
    //Q_INVOKABLE void HandleBackKey();
#ifdef PANDORA_REAR_ENABLED_FOR_CES
	Q_INVOKABLE void HandleBackKey(bool isJogDial, bool isRRC); //modified by esjang 2013.06.21 for Touch BackKey
#else	
	Q_INVOKABLE void HandleBackKey(bool isJogDial); //modified by esjang 2013.06.21 for Touch BackKey
#endif

    /**
     *******************************************************************
     * Check the current status, request connect to RPM and Pandora
     *
     * @param[in] inSelectedIndex , selected index
     * @return  None
     *
     *******************************************************************
     */
    Q_INVOKABLE void Connect();

     /**
     *******************************************************************
     * Check the current status, request disconnect to RPM and Pandora
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
    Q_INVOKABLE void Disconnect();
     
    //{ added by esjang 2013.05.15 for certification issue, sessionterminate.

     /**
     *******************************************************************
     * Check the current status, request disconnect to RPM and Pandora
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
    Q_INVOKABLE void SessionTerminate();
    //} added by esjang 2013.05.15 for certification issue, sessionterminate.

     

    //{ modified for CR 14370 by Vijayakumar K
    /**
     *******************************************************************
     * Set the pandora state to active in UISH
     *
     * @param   None
     *
     * @return  None
     *
     *******************************************************************
     */
     Q_INVOKABLE void SetPandoraStateActive();
    //} modified for CR 14370 by Vijayakumar K

    /**
     *******************************************************************
     * Get the current devices connection status
     *
     * @param   None
     *
     * @return  QVariantList list of devices connection status
     *
     *******************************************************************
     */
    // Q_INVOKABLE QVariantList GetDeviceStatus();

    /**
     *******************************************************************
     * Launches the sound settings ui
     *
     * @param   None
     *
     * @return  none
     *
     *******************************************************************
     */
     Q_INVOKABLE void LaunchSoundSetting();

	/**
     *******************************************************************
     * Set the status of option menu 
     *
     * @param   inStatus , true if visible
     *
     * @return  none
     *
     *******************************************************************
     */
    Q_INVOKABLE void SetOptionMenuVisibilty(bool inStatus){m_bOptionMenuVisible =  inStatus;}

    Q_INVOKABLE int getCurrentScreen() {return AppEngineBase::GetDisplayType();}

	/**
     *******************************************************************
     * Set/Get the status of option menu 
     *
     * @param   inStatus , true if visible
     *
     * @return  none
     *
     *******************************************************************
     */
    Q_INVOKABLE void SetSystemPopupVisibility(bool inVisible);
    Q_INVOKABLE bool IsSystemPopupVisible() {return m_bSystemPopupVisible;}
    Q_INVOKABLE void CloseSystemPopup(); 

    //{ modified by yongkyun.lee 2014-07-10 for :  popup OSD 
    Q_INVOKABLE void SetLocalPopupVisibility(bool inVisible);
    Q_INVOKABLE bool IsLocalPopupVisibility( ){ return m_bLocalPopupVisible;}

    Q_INVOKABLE bool isPopupVisibility()       { return (m_bLocalPopupVisible||m_bSystemPopupVisible);  }
    //} modified by yongkyun.lee 2014-07-10 
    

    /**
     *******************************************************************
     * This methode will return the string length based on param
     * 5 seconds after the last touch of the tune wheel.
     *
     * @param[in] fullString , actual String
     * @param[in] fontFamily , Font
     * @param[in] pixelSize , Pixel size
     *
     * @return  int , Length of string
     *
     *******************************************************************
     */
    Q_INVOKABLE int getStringWidth( const QString &fullString,const QString &fontFamily,int pixelSize );

    Q_INVOKABLE void sendTouchCleanUpForApps(); //{ modified by yongkyun.lee 2014-04-30 for : ITS 236250

#ifdef FRAMEWORK_ENABLED
	/**
     *******************************************************************
     * Launch key pad setting
     *
     * @param   None
     *
     * @return  void
     *
     *******************************************************************
     */
      Q_INVOKABLE void LaunchKeypadSettings();
	  
	/**
     *******************************************************************
     * Get the country varient
     *
     * @param   None
     *
     * @return  int country varient
     *
     *******************************************************************
     */
     Q_INVOKABLE int GetCountryVariant(){ return (int)m_nCountryVariant; }

#endif
    /**
     *******************************************************************
     * Called at start of application for setting the language and
     * signalling to UI about current language. Also called when
     * there is language change event occurs
     *
     * @param   None
     *
     * @return  None
     *
     *******************************************************************
     */
    void RetranslateUi();

    /**
     *******************************************************************
     * Return the StationList class Reference
     *
     * @param     None
     *
     * @return    Reference to CAppPandoraStationList
     *
     *******************************************************************
     */
    CAppPandoraStationList& GetStationListRef();

    /**
     *******************************************************************
     * Return the Search class Reference
     *
     * @param     None
     *
     * @return    Reference to CAppPandoraSearch
     *
     *******************************************************************
     */
    CAppPandoraSearch& GetSearchRef();

    /**
     *******************************************************************
     * Return the Track class Reference
     *
     * @param     None
     *
     * @return    Reference to CAppPandoraTrack
     *
     *******************************************************************
     */
    CAppPandoraTrack& GetTrackRef();

    /**
     *******************************************************************
     * Return the PandoraMenus class Reference
     *
     * @param     None
     *
     * @return    Reference to PandoraMenus
     *
     *******************************************************************
     */
    PandoraMenus& GetPandoraMenus();

    //{ added by esjang 2013.04.26 for Clear TP Message
    /**
     *******************************************************************
     * Return the Pandora external notifier class Reference
     *
     * @param     None
     *
     * @return    Reference to Pandora external notifier
     *
     *******************************************************************
     */
    CExternalNotifier& GetPandoraNotifier();
    //} added by esjang 2013.04.26 for Clear TP Message

   /**
    *******************************************************************
    * Clear the cache on back soft key .
    *
    * @param  None
    *
    * @return None
    *
    *******************************************************************
    */
    void ClearCache();
	/**
     *******************************************************************
     * Get the RearUSB varient
     *
     * @param   None
     *
     * @return  bool isRearUSB
     *
     *******************************************************************
     */
    Q_INVOKABLE bool GetVariantRearUSB(){ return ( AppEngineBase::GetSystemVariant() & SYSTEM_VARIANT_REAR_USB ); } // added by esjang 2013.07.26 for ISV #86633


   //{ added by esjang 2013.01.09 for ITS # 150556
   /**
    *******************************************************************
    * Set loading popup status .
    *
    * @param  inStatus , true if visible
    *
    * @return None
    *
    *******************************************************************
    */
    Q_INVOKABLE void SetLoadingPopupVisibilty(bool inStatus){
    m_bLoadingPopupVisible =  inStatus;}

    //} added by esjang 2013.01.09 for ITS # 150556

    Q_INVOKABLE bool IsAVModeChanged(){return bAV_Mode_Changed;}
    Q_INVOKABLE bool IsRunningInBG(){return m_bIsBackGround;} //modified by cheolhwan 2014-03-22. ITS 230532. defined to Q_INVOKABLE .
    bool IsConnected(){return m_bPandoraConnected;}
    bool IsException();
    Q_INVOKABLE bool IsCalling(){return nCallingState;}// added by esjang 2013.03.16 for BT Call flag, Disable Emmiting playing time
    bool IsClusterUpdate(){return m_bClusterUpdate;}// added by esjang 2013.04.22 for Cluster update flag, Disable Emmiting cluster info.

    void setClusterUpdate(bool value);// added by esjang 2013.04.22 for Cluster update flag, Disable Emmiting cluster info.
    
//{ added by esjang 2013.03.18 for Preventing Abnormal termination(SW WDT)
   static void signalHandler (int signum);
   void signalConnection();
   static void setSignalHandlerObject (CAppPandoraBase* m);
   static CAppPandoraBase* SigM;

   void restart(int sig);
//} added by esjang 2013.03.18 for Preventing Abnormal termination(SW WDT)

   DeviceInfo* GetCurrentDevice();


   void startPipeline(USB_PORT_T port);

   void stopPipe();

   USB_PORT_T getPortNum(QString sysPath);

   bool FindDevice (USB_PORT_T port , QString& inSrc);

   Q_INVOKABLE bool getScrollingTicker() { return m_bScrollingTicker; } // added by esjang 2013.08.14 for ITS # 183734

   Q_INVOKABLE void IfMute_SentAudioPass();

   void setCurrentView(int inCurrentView);

   Q_INVOKABLE void SendTouchCleanUpForApps(void) { return AppEngineBase::SendTouchCleanUpForApps();} //added by jyjeon 2014-02-27 for  ITS 226165

   Q_INVOKABLE void SetShareDataProvider(bool bPlay); //added by jyjeon 2014-03-06 for VR
   //{ modified by yongkyun.lee 2014-03-11 for : BT streaming Mute-play control
   //{ modified by yongkyun.lee 2014-04-17 for : ITS 234955
   Q_INVOKABLE  MODE_STATE_T getModeStateHistoryPrev();
   Q_INVOKABLE  MODE_STATE_T getModeStateHistoryCurr();
   //} modified by yongkyun.lee 2014-04-17 
   bool getModeStateHistoryFMode();
   void setModeStateHistoryFMode(bool val);    
   void startPlayMuteTimer();	
   //} modified by yongkyun.lee 2014-03-11 
   Q_INVOKABLE void TrackInfoOSD();//{ modified by yongkyun.lee 2014-04-06 for : ITS 233641
   void setUnMuteForBTStream();//{ modified by yongkyun.lee 2014-04-06 for : ITS 233641
  
   void OSD(int osdID); //{ modified by yongkyun.lee 2014-07-17 for : ITS 243240
   void setOSDFlag(bool val){m_OSDFlag = val;}// modified by yongkyun.lee 2014-09-23 for : ITS 248810 
   ActiveView isActiveView() { return m_eViewState;}// modified by yongkyun.lee 2014-11-03 for : //leeyk1 OSD TEST

   int vehucleType; //add vehicle type for DH, 16MY, DH PE wonseok.heo 2014.12.29
   bool isSeek; //add by wonseok.heo for OSD at BG 2015.02.04

   Q_INVOKABLE void SetSeekStatus(bool index) {isSeek = index; } // add by wonseok.heo for OSD at BG 2015.02.04

   Q_INVOKABLE int IsVehicleType(){return vehucleType;} //ftw

private:

	/**
    *******************************************************************
    * Av mode change to Pandora to be plaed in BG .
    *
    * @param mode , current av mode
    * @return None
    *
    *******************************************************************
    */
    void playInBackground(MODE_STATE_T mode);
    /**
    *******************************************************************
    * Handle AV_MODE has changed event .
    *
    * @param mode , current av mode
    * @return None
    *
    *******************************************************************
    */
    void handleAVModeChange(MODE_STATE_T mode);

    /**
    *******************************************************************
    * Register to receive mode key event .
    *
    * @param  [in] inStatus , pandora connected .
    *
    * @return None
    *
    *******************************************************************
    */
    void UpdatePandoraAvailableStatus(bool inStatus);

    /**
    *******************************************************************
    * Notify to UISH to acquire audio path .
    *
    * @param  [in] inConnectedDev , device type .
    *
    * @return None
    *
    *******************************************************************
    */
    void NotifyAudioPathChanged(DeviceInfo::EConnectionType inConnectedDev);
    void SetDevice();

    /**
    *******************************************************************
    * Tune key handler .
    *
    * @param  [in] eventId , Event Id .
    * @param  [in] aEvent , jog , event payload
    *
    * @return None
    *
    *******************************************************************
    */
    void TuneHandler( int eventId, const Event &aEvent );

    //{ modified by yongkyun.lee 2014-06-18 for :  ITS 240298
    void TemporalMute();
    void TemporalUnmute();
    //} modified by yongkyun.lee 2014-06-18 

signals:

#ifndef FRAMEWORK_ENABLED
    void signalJogCenterClicked();

    void signalJogCenterPressed();

    void signalJogCenterReleased();

    void signalJogCenterLongPressed();

    void signalJogCenterCriticalPressed();

    void signalJogNavigation( uint arrow, int status );
#endif

    /**
     *******************************************************************
     * Signal to QML to connect pandora , play pandora in BG
     *
     * @param   None
     *
     * @return  None
     *
     *******************************************************************
     */
    void playInBG();

    void deviceChanged();
    /**
     *******************************************************************
     * Signal to UI for translating to new language set
     *
     * @param   languageId  The new language set
     *
     * @return  None
     *
     *******************************************************************
     */
    void retranslateUi(int languageId); // required for interaction with QML part

    /**
     *******************************************************************
     * Signal to UI about the device status as connected
     *
     * @param[in]   isConnected The current status of the device
     *
     * @return None
     *
     *******************************************************************
     */
    void connectionStatusChanged(const int isConnected);

    /**
     *******************************************************************
     * Signal to QML for bringing the required UI in front on Foreground
     * event
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
    void foreground();

    /**
     *******************************************************************
     * Signal to QML for putting the UI in background
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
    void background();

    /**
     *******************************************************************
     * Signal to QML about the Network Error.
     *
     * @param   None
     *
     * @return  None
     *
     *******************************************************************
     */
    void networkError();

    /**
     *******************************************************************
     * Signal to QML about the key event.
     *
     * @param[in] inKeyEvt The key to be handled
     *
     * @return  None
     *
     *******************************************************************
     */
    void handleSkipEvent(bool isJogKey);

    /**
     *******************************************************************
     * Signal to QML about the HU/SWRC track skip event.
     *
     * @param[in] inStatus The key to be handled
     *
     * @return  None
     *
     *******************************************************************
     */
    void skipKeyEvent(bool inStatus);

    /**
     *******************************************************************
     * Signal to QML about the key event.
     *
     * @param[in] inKeyEvt The key to be handled
     *
     * @return  None
     *
     *******************************************************************
     */
    void handleRewindEvent();

    /**
     *******************************************************************
     * Signal to QML about the jog hard key pressed.
     *
     * @param[in] inJogKey The jog key to be handled
     *
     * @return  None
     *
     *******************************************************************
     */
   // void jogHardkeyPressed(const int inJogKey);

    /**
     *******************************************************************
     * Signal to QML about jog key event.
     *
     * @param[in] inJogKey  The Jog key to be handled
     *
     * @return  None
     *
     *******************************************************************
     */
    //void jogsendKeyTovc(const int inJogKey);

     /**
     *******************************************************************
     * Signal to QML about menu key event.
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
    void menuKeyPressed(bool isJogMenuKey);

    /**
    *******************************************************************
    * Signal to QML about back key event.
    *
    * @param None
    *
    * @return  None
    *
    *******************************************************************
    */
    //void backKeyPressed();
    void backKeyPressed(bool isJogDial); //modified by esjang 2013.06.21 for Touch BackKey

#ifdef PANDORA_REAR_ENABLED_FOR_CES
    void isRRCKey(bool isRRCKey);
#endif

    /**
     *******************************************************************
     * Signal to QML .
     *
     * @param[in] inPndrError  , Request result
     *
     * @return  None
     *
     *******************************************************************
     */
    void handleError(const quint16 inPndrError);

    /**
     *******************************************************************
     * Signal to QML about VR Pause command.
     *
     * @param inPayLoad , 0 for pause , 1 for play .
     *
     * @return  None
     *
     *******************************************************************
     */
    void playCommandFromVR(int inPayLoad);

     /**
     *******************************************************************
     * Signal to QML about VR thumb up/down command.
     *
     * @param inPayLoad , 0 for thumb up , 1 for thumb down .
     *
     * @return  None
     *
     *******************************************************************
     */
    void ratingCommandFromVR(int inPayLoad);

    /**
     *******************************************************************
     * Signal to QML about shuffle on/off command.
     *
     * @param None .
     *
     * @return  None
     *
     *******************************************************************
     */
    void shuffleCommandFromVR();

     /**
     *******************************************************************
     * Signal to QML about Device list changed.
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
    void deviceListUpdated();

    //{ added by esjang 2012.11.15 for CAN TP Message
     /**
     *******************************************************************
     * Signal to QML Layer for informing about the current elapsed
     * time for currently playing track
     *
     * @param[in] inElapsedTime The elapsed time in Sec
     *
     * @return  None
     *
     *******************************************************************
     */
   // void trackElapsedTime(const unsigned int inElapsedTime);
    //} added by esjang 2012.11.15 for CAN TP Message

    // { added by esjang 2012.11.20 CAN TP Message
    /**
     *******************************************************************
     * Signal to AppPandora informing the current track title
     *
     * @param[in] title: The current track title
     *
     * @return  None
     *
     *******************************************************************
     */
    //void trackTitleReceived(QString title);
    // } added by esjang 2012.11.20 CAN TP Message


//{ added by esjang 2013.01.28 for No CR, Updated UX
    /**
     *******************************************************************
     * Signal to QML about the AV Mode changed.
     *
     * @param[in] None
     *
     * @return  None
     *
     *******************************************************************
     */
    void avModeChanged();
//} added by esjang 2013.01.28 for No CR, Updated UX

    /**
     *******************************************************************
     * Signal to QML on tune key dialed
     * @param isForward , True : Forward , False : Reverse
     *
     * @return  void
     *
     *******************************************************************
     */
    void tuneKeyDialed(bool isForward);

    /**
     *******************************************************************
     * Signal to QML on tune key center pressed
     * @param void
     *
     * @return  void
     *
     *******************************************************************
     */
    void tuneCenterPressed();

    /**
     *******************************************************************
     * Signal to QML on tune key center Released
     * @param void
     *
     * @return  void
     *
     *******************************************************************
     */
    void tuneCenterReleased(); //added by jyjeon 2014.01.20 for ITS 218714
	
    /**
     *******************************************************************
     * Signal to QML on tune timer off
     * @param void
     *
     * @return  void
     *
     *******************************************************************
     */
    void tuneOff();

    /**
     *******************************************************************
     * Signal to QML on DRS status changed
     * @param inScrollingTicker , True if status show , False for hide
     *
     * @return  void
     *
     *******************************************************************
     */
    void tickerChanged(bool inScrollingTicker);

    /**
     *******************************************************************
     * Notify to QML
     * @param void
     *
     * @return  void
     *
     *******************************************************************
     */
    void scrollingTickerChanged();

    /**
     *******************************************************************
     * Signal to QML on call state
     * @param 
     *
     * @return  
     *
     *******************************************************************
     */
    void callingState(bool incallingState);

    //{ added by esjang 2013.11.07 for audit issue network error popup 
    /**
     *******************************************************************
     * Signal to QML on network error state
     * @param 
     *
     * @return  
     *
     *******************************************************************
     */
    void networkErrorState(bool isInsufficientNetwork);
    //} added by esjang 2013.11.07 for audit issue network error popup 

    //{ added by cheolhwan 2013.11.18. for ITS 209342.
    /**
     *******************************************************************
     * Signal to QML for calling popup
     * @param 
     *
     * @return  
     *
     *******************************************************************
     */
    void callingPopup();
    //} added by cheolhwan 2013.11.18. for ITS 209342.
    
	// added by jyjeon 2013.12.12 for ITS 214424
    /**
     *******************************************************************
     * Signal to QML on skip long key pressed
     * @param void
     *
     * @return  void
     *
     *******************************************************************
     */
    void skipNextLongKeyPressed();
	// added by jyjeon 2013.12.12 for ITS 214424
	
    void showPopupID(int popupID);//POPUPEnums::PopupIDPnd popupID );//{ modified by yongkyun.lee 2014-04-06 for : ITS ITS 233689
    void popupClose();// modified by yongkyun.lee 2014-09-04 for : ITS 247991
	
public slots:

       void Notified();
    /**
     *******************************************************************
     * Handle the events sent by UIStateHandler
     *
     * @param[in]   aEvent  event to be handled
     *
     * @return  None
     *
     *******************************************************************
     */
#ifdef FRAMEWORK_ENABLED
    //{ modified by esjang 2012.12.22 for SWAP DC
    //void NotifyFromUIStateManager( Event aEvent );
    virtual void NotifyFromUIStateManager( Event aEvent );
    virtual void NotifyFromUIStateManagerRear ( Event aEvent );
    virtual void NotifyFromUIStateManagerCommon ( Event aEvent );
    void onSessionTerminateAcked();
    //} modified by esjang 2012.12.22 for SWAP DC
#endif

    /**
    *******************************************************************
    * notify the AVWidget with song name and current time.
    *
    * @param   None
    *
    * @return  None
    *
    *******************************************************************
    */
    void NotifyAVWidget();

    /**
    *******************************************************************
    * stop notify the AVWidget.
    *
    * @param   None
    *
    * @return  None
    *
    *******************************************************************
    */
    void StopNotifyAVWidget();
    /**
     *******************************************************************
     * The result of request from Pandora MW
     * All failed requests related to network to be handled here only.
     *
     * @param[in] inRequestType The request for which the reply is
     *                          received
     * @param[in] inPndrError   The result of the request
     *
     * @return  None
     *
     *******************************************************************
     */
    void OnRequestCompleted(const ERequestType inRequestType, const EPndrError inPndrError);

	/**
	 *******************************************************************
	 * The result of request from Pandora MW for BT.
	 * All failed requests related to network to be handled here only.
	 *
	 * @param[in] inRequestType The request for which the reply is
	 *							received
	 * @param[in] inPndrError	The result of the request
	 *
	 * @return	None
	 *
	 *******************************************************************
	 */
	//{ added by cheolhwan 2014-02-27. Added "m_pBTSignals" to receive BT Signal.
    void OnBTRequestCompleted(const ERequestType inRequestType, const EPndrError inPndrError);
    //} added by cheolhwan 2014-02-27. Added "m_pBTSignals" to receive BT Signal.

    void OnNextTimeOSD();// modified by yongkyun.lee 2014-09-11 for : ITS 248198
    void OnTrackUpdated(const quint32 inTrackToken);
    /**
     *******************************************************************
     * The result of request from RPM Client
     *
     * @param[in] inAM Audio mode for which request was raised
     * @param[in] inSink   Audio Sink which is provided for the request
     *
     * @return  None
     *
     *******************************************************************
     */
#ifdef FRAMEWORK_ENABLED
    void OnRPMAcquireResponseReceived(TAudioMode tAM ,TAudioSink tAS);
#endif


    void OnSyncWithIPod();

    /**
     *******************************************************************
     * This slot is binded to the m_tuneSelectTimer and is called after a
     * 5 seconds after the last touch of the tune wheel.
     *
     * @param[in] None
     *
     * @return  None
     *
     *******************************************************************
     */
     Q_INVOKABLE void onTuneSelectTimer();

    /**
     *******************************************************************
     * This slot is binded to the m_noSkipTimer and is called after a
     * 5 seconds after the last press seek up key.
     *
     * @param[in] None
     *
     * @return  None
     *
     *******************************************************************
     */
     Q_INVOKABLE void onNoSkipTimer(); // modified by esjang 2013.08.26 for ITS #186175
    
    /**
     *******************************************************************
     * 
     * @param 
     *
     * @return  void
     *
     *******************************************************************
     */
     Q_INVOKABLE void onPlayMuteTimer();//{ modified by yongkyun.lee 2014-03-11 for : BT streaming Mute-play control
	
    /**
     *******************************************************************
     * This Method can be used for tune timer off and called from qml to make timer off
     * @param inStatus , True : Stop , False : Start
     *
     * @return  void
     *
     *******************************************************************
     */
     Q_INVOKABLE void TuneSelectTimer(bool inStatus);

    /**
     *******************************************************************
     * ToDo : Provide info
     * @param void
     *
     * @return  void
     *
     *******************************************************************
     */
     Q_INVOKABLE QString getConnectTypeName() {return m_connect_type_name;} // modified by esjang 2013.06.04 for ITS #166991
     //added by esjang
     Q_INVOKABLE void SetPlayState(bool inState){ bResumePlay = inState; return;} // modified by jyjeon 2014.02.05 for prevent error
     //Q_INVOKABLE bool IsBTPandora(){ if(m_eCurrentDevice == DeviceInfo::EBTConn) return true;} //added by jyjeon 2014.01.11 for ITS 218524
	 Q_INVOKABLE bool IsBTPandora(){ bool ret = false; if(m_eCurrentDevice == DeviceInfo::EBTConn) ret = true; return ret;} //modified by cheolhwan 2014-02-14. coverity issue.

	 Q_INVOKABLE void printQMLLog(QString str);  //added by cheolhwan.bae 2014-04-15. Add Debug Macro.

    
     /**
      *******************************************************************
      * Audio Sample rate changed.
      * @param inSampleRate : New sample rate
      *
      * @return  void
      *
      *******************************************************************
      */
     void onAudioSampleRateChanged(int inSampleRate);
private:

    /** App ID for this application */
    virtual APP_ID_T GetThisAppId() const { return APP_PANDORA; }

#ifdef PANDORA_REAR_ENABLED_FOR_CES
    void NotifyFromUIStateManagerGeneric( Event aEvent, DISPLAY_T display );// added by esjang 2012.12.22 for SWAP DC
#else
    void NotifyFromUIStateManagerGeneric( Event aEvent );// added by esjang 2012.12.22 for SWAP DC
#endif

    /**
     *******************************************************************
     * Handle the error , if any error exists notify to qml layer
     *
     * @param[in] inPndrError error code
     *
     * @return  None
     *
     *******************************************************************
     */
    void HandleIfError(const EPndrError inPndrError);

    void SetConnection(CPndrInterface* inPndrInterface);

    modeStateHistory m_modeStateHistory;//{ modified by yongkyun.lee 2014-03-11 for :  BT streaming Mute-play control

    /** Translator user */
    QTranslator *m_pTranslator;

    #ifdef FRAMEWORK_ENABLED
    /** Country Varient */
    ECountryVariant m_nCountryVariant;
    /** Current language */
    LANGUAGE_T m_nLanguage;
    #endif

    /** Pandora MW interface */
    CPndrInterface  m_PndrInterface , m_PndrInterfaceTwo , m_PndrBTInterface ;
    /** Reference to Station List class instance */
    CAppPandoraStationList*   m_StationList;
    /** Reference to Search class instance */
    CAppPandoraSearch*        m_Search;
    /** Reference to Track class instance */
    CAppPandoraTrack*         m_Track;
    /** External notifier to update OSD */
    CExternalNotifier* m_pNotifier;
    /** list of Devices */
    //QVariantList m_ListofDevices;
    /** Curent connected device index from the List */
    //int nConnectedIndex;
    /** Signal handler */
    CPndrStateHandler* m_pSignals ;
    CPndrStateHandler* m_pBTSignals ;  //added by cheolhwan 2014-02-27. Added "m_pBTSignals" to receive BT signal.
    /** HMIState  handler */
    HMIState m_eHMIState;
    /** Pandora Connected status */
    bool m_bPandoraConnected;
    /** Logger instance */
    USE_LOG_UTIL
    /** Pandora Menus */
    PandoraMenus m_PandoraMenus;
 	/** Av mode has changed , play pandora on FG event */
    bool  bAV_Mode_Changed ;
    /** Pandora is in Back ground*/
    bool m_bIsBackGround;
	/** Option menu visible status */
    bool m_bOptionMenuVisible;
	/** Loading Popup visible status */
    bool m_bLoadingPopupVisible; // added by esjang 2013.01.09 for ITS # 150556

    /** Before phone call is comming, track plya/pause status  */
    bool bResumePlay; // added by esjang 2013.03.14 for BT Call is comming, save the previous track state.

    /** Before phone call is comming, track plya/pause status  */
    bool nCallingState; // added by esjang 2013.03.16 for BT Call flag, Disable Emmiting playing time

    /** Before cluster update is comming, do not send cluster data  */
    bool m_bClusterUpdate;  // added by esjang 2013.04.22 for Cluster udpate, Disable sending cluster data
    bool isNetworkError;    // added by esjang 2013.05.16 for certification issue, networkerror
    bool isInsufficientNetwork; // added by esjang 2013.10.26 for insufficient network issue. 
    bool m_bSystemPopupVisible; //added by esjang 2013.08.05 for ITS # 182095
    bool m_bLocalPopupVisible;  // modified by yongkyun.lee 2014-07-10 for :  popup OSD 
#ifdef PANDORA_REAR_ENABLED_FOR_CES
    bool m_bSwapEnabled;// added by esjang 2013.12.30 for CES 
    QHash<DISPLAY_T, bool> m_bPandoraInFG; // modified by Sergey 10.05.2013
#endif
    DeviceInfo::EConnectionType m_eCurrentDevice ;

    DeviceInfo *mPandora , *mPandoraF, *mPandoraR , *mBTPandora;

    CPipeLineThread* m_pipeThread ;

    QTimer m_SyncTimer;

    QString m_connect_type_name; // modified by esjang 2013.06.04 for ITS #166991

    /** Tune timer */
    QTimer *m_tuneSelectTimer;

    /** Marquee text scrolling*/
    bool m_bScrollingTicker;

    bool m_isFrontLCD;
    /** No Skip timer */
    QTimer *m_noSkipTimer; // modified by esjang 2013.08.26 for ITS #186175

    QTimer *m_PlayMuteTimer;// modified by yongkyun.lee 2014-03-11 for :  BT streaming Mute-play control

    /**  Mute state */
    bool m_isMuteState;

    /** Current View state*/
    ActiveView m_eViewState;

    /**Current device*/
    QString m_DeviceSrc;

    /** seek next long key **/
    bool m_isSeekNextLongKeyPressed; // added by jyjeon 2013.12.12 for ITS 214424

    /** SharedDataProvider **/
    SharedDataProvider *m_pShareDataProvider;//added by jyjeon 2014-03-06 for VR
    bool m_OSDFlag; //{ modified by yongkyun.lee 2014-04-06 for : ITS 233641
    bool m_NextTimeOSD; //{ modified by yongkyun.lee 2014-09-11 for : ITS 248198
    //bool m_isSoundSetting; //added by jyjeon 2014-06-05 for ITS 239408
    TempMuteState m_isTemporalMute;//{ modified by yongkyun.lee 2014-06-18 for :  ITS 240298
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

#endif // DHAVN_APPPANDORABASE_H
