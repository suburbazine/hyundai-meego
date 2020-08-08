#ifndef SETTINGS_APP_ENGINE_H
#define SETTINGS_APP_ENGINE_H

// Qt Header
#include <QTimer>
#include <QtDBus>
#include <QDeclarativeView>
#include <QGraphicsObject>
#include <QTranslator>
#include <QDBusMessage>
#include <QDBusInterface>

// DH Header
#include <uistate_manager_def.h>

#include <DHAVN_DualDisplayHelper.h>
#include <DHAVN_SharedDataProvider.h>

#include <DHAVN_AppFramework_QML_Def.h>
#include <DHAVN_AppFramework_Event_Def.h>
#include <DHAVN_AppFramework_AppVextEngineQML_Def.h>


// Local Header
#include <DHAVN_AppSettings_Def.h>
#include <DHAVN_AppSettings_SaveSettings.h> // add for backup DB

// SW WDT
#include <signal.h>
#include <bits/siginfo.h>


#define MAX_STATE_SIZE 30

/** MICOM Manager QtDBus Service name */
#define MICOM_MANAGER_SERVICE_NAME          "com.lge.car.micom"
#define MICOM_MANAGER_SYSINFO_PATH          "/SysInfo"
#define MICOM_MANAGER_SYSINFO_INTERFACE     "com.lge.car.micom.sysinfo"

/** MICOM Manager QtDBus Signals  */
#define MICOM_MANAGER_SIGNAL_LCD_DAYNIGHT       "LgeDBus_EventDayNightStatus"
#define MICOM_MANAGER_SIGNAL_CAR_IF             "LgeDBus_EventCarIFStatus"

/** MICOM Managar QtDBus HMI Display Settings**/

#define MICOM_MANAGER_QTDBUS_NAME          "com.dhavn.mostmanager.qtdbus"
#define MICOM_MANAGER_HMI_PATH          "/hmi"

/** Aspect Ratio **/

#define ASPECT_RATIO_THUMBNAIL_WIDTH  365
#define ASPECT_RATIO_THUMBNAIL_HEIGHT 202
#define ASPECT_RATIO_THUMBNAIL_X  795
#define ASPECT_RATIO_THUMBNAIL_Y  464

extern bool JukeboxEnable;
extern bool factory; // add for its 243706
extern int glo_balance; // add for ISV 100925
extern int glo_fade; // add for ISV 100925
extern int glo_high; // add for ISV 100925
extern int glo_mid; // add for ISV 100925
extern int glo_low; // add for ISV 100925
extern int glo_front_bright; // add for ISV 100925
extern int glo_rear_bright; // add for ISV 100925


enum
{
    /** Jog events */
    SETTINGS_EVT_KEY_JOG_ARROW_UP = 0,
    SETTINGS_EVT_KEY_JOG_ARROW_DOWN,
    SETTINGS_EVT_KEY_JOG_ARROW_LEFT,
    SETTINGS_EVT_KEY_JOG_ARROW_RIGHT,
    SETTINGS_EVT_KEY_JOG_WHEEL_LEFT,
    SETTINGS_EVT_KEY_JOG_WHEEL_RIGHT,
    SETTINGS_EVT_KEY_JOG_CENTER,
};

enum SETTINGS_LCD_BRIGHTNESS
{
    SETTINGS_LCD_BRIGHTNESS_0 = 0x00,
    SETTINGS_LCD_BRIGHTNESS_1 = 0x01,
    SETTINGS_LCD_BRIGHTNESS_2 = 0x02,
    SETTINGS_LCD_BRIGHTNESS_3 = 0x03,
    SETTINGS_LCD_BRIGHTNESS_4 = 0x04,
    SETTINGS_LCD_BRIGHTNESS_5 = 0x05,
    SETTINGS_LCD_BRIGHTNESS_6 = 0x06,
    SETTINGS_LCD_BRIGHTNESS_7 = 0x07,
    SETTINGS_LCD_BRIGHTNESS_8 = 0x08,
    SETTINGS_LCD_BRIGHTNESS_9 = 0x09,
    SETTINGS_LCD_BRIGHTNESS_10 = 0x10
};

enum SETTINGS_DAYNIGHT_MODE
{
    SETTINGS_LCD_NIGHTMODE   = 0x00,
    SETTINGS_LCD_DAYMODE    = 0X01,
    SETTINGS_LCD_AUTO   = 0x02
};

enum SETTINGS_CLONE_STATUS
{
    VIDEO_CLONE_NONE  = 0,
    VIDEO_CLONE_BEGIN,
    VIDEO_CLONE_EXIT
};


class SettingsAppEngine: public AppVextEngineQML
{
    Q_OBJECT

public:

    SettingsAppEngine( ECountryVariant variant, QTranslator *translator, LANGUAGE_T language, QObject *pParent = NULL );

    ~SettingsAppEngine();

    /** @property inputText
     * Text entered by user
     */
    //Q_PROPERTY( QString inputtext
    //           READ inputtext
    //           NOTIFY inputTextChanged)
    /** @property captionStringList
     * List of found captions
     */
    Q_INVOKABLE void HandleBackKey( int screen );
    //Q_INVOKABLE void HandleRRCBackKey();

    //Q_INVOKABLE void HandleHomeKey( int screen );

    Q_INVOKABLE QString getFont(bool hdb);

    Q_INVOKABLE void ChangeLanguage(int language);

    Q_INVOKABLE void NotifyApplication(int key, int sValue, QString sValue2, int targetScreen = 0);
    Q_INVOKABLE void NotifyApplication(int key, int sValue, QString sValue2, int targetScreen, APP_ID_T targetAppID);

    //Q_INVOKABLE void LaunchFileManager();
    Q_INVOKABLE void LaunchListFM(int targetScreen);
    Q_INVOKABLE void LaunchUpgradeViaUSB();
    //Q_INVOKABLE void ShowVideo( int screen );
    //Q_INVOKABLE void HideVideo( int screen );

    // Q_INVOKABLE bool IsVideoPlayerOnFG( int screen );

    Q_INVOKABLE void FrontScreenDisplayOff();
    Q_INVOKABLE void RearScreenDisplayOff();
    Q_INVOKABLE void SetSystemTime(int year, int month, int day,
                                   int hours,int minutes, int ampm);

    Q_INVOKABLE void SetDateFormat(int dateFormat);
    Q_INVOKABLE void SetTimeFormat(uchar timeFormat);
    Q_INVOKABLE void SendDistanceUnit(int distanceUnit);
    Q_INVOKABLE void SendDateFormat(int dateFormat);

    Q_INVOKABLE void SendBeepSettingToIBox(int beep);
    Q_INVOKABLE void SendPhonePrioritySettingToIBox(int phonePriority);
    Q_INVOKABLE void SendTimeToIBox();
    Q_INVOKABLE void NotifyDisplayBrightnessChange(bool isFrontScreen, unsigned int value);
    Q_INVOKABLE void NotifyDistanceUnitChanged(int value);
    Q_INVOKABLE void NotifyDisplayModeChange(unsigned int value);

    Q_INVOKABLE void NotifyAppsFormatStarted();
    Q_INVOKABLE void NotifyAppsFormatCompleted();
    Q_INVOKABLE void showPopapInMainArea(int type, int screen);
    Q_INVOKABLE void PopupHandler(int type, int screen);
    //Q_INVOKABLE void focusStateHandler(bool isFocusPresent, int screen);

    //image settings
    //    Q_INVOKABLE void HandleBrightness(int value);
    //    Q_INVOKABLE void HandleSaturation(int value);
    //    Q_INVOKABLE void HandleColor(int value);
    //    Q_INVOKABLE void HandleDarkness(int value);

    //MOST api
    Q_INVOKABLE void NotifyFrontScreenBrightness(const uint value);
    Q_INVOKABLE void NotifyRearScreenBrightness(const uint value);
    Q_INVOKABLE void NotifyFrontRearScreenBrightness(const uint value);
    Q_INVOKABLE void NotifySetAspectRatioChange(unsigned int value);
    //Q_INVOKABLE void NotifySetBeepsetting(const uint beepSetting);
    Q_INVOKABLE void NotifySetVideoSettings( const int brightness, const int saturation, const int contrast, const int hue );
    Q_INVOKABLE void NotifySetAspectRatio(int value,int screen);
    //Q_INVOKABLE void NotifyFullScreen(int screen);

    Q_INVOKABLE bool NotifyVideoMode(int targetScreen);
    Q_INVOKABLE void NotifySetIlluminationMostManager(unsigned int value);
    Q_INVOKABLE void NotifyDisplayOnOffMostManager( unsigned int value );
    Q_INVOKABLE bool IsScreenOff(bool isFrontScreen);
    Q_INVOKABLE void LaunchSearchView(int activeTab);

    Q_INVOKABLE int maskValue( int value, int maskValue, bool isAnd );
    Q_INVOKABLE bool checkMaskValue( int value, int maskValue );

    Q_INVOKABLE bool isAgree10Variant(); //added for NA/CH Agreement Spec Modify(time 10, default on)

    Q_INVOKABLE void printLogMessage(QString logString); //added for QML Log Message

    // Is Display Video
    //Q_INVOKABLE bool isVideoShowing();

    // Time Picker Popup
    Q_INVOKABLE void setTimePickerPopupStatus( bool isShow );
    Q_INVOKABLE bool isGPSAvailable() {return m_bIsGPSAvailable;};

    Q_PROPERTY(int variantCountry READ variantCountry)
    Q_PROPERTY(bool jukeboxImageEnable READ jukeboxImageEnable)
    Q_PROPERTY(bool usbUpdatePresent READ usbUpdatePresent NOTIFY usbUpdatePresentChanged)

    // SWAP DC
    Q_PROPERTY(bool dcSwapped READ dcSwapped WRITE setDcSwapped NOTIFY dcSwapChanged)

    Q_PROPERTY(bool parkingStatus READ parkingStatus WRITE setParkingStatus NOTIFY parkingStatusChanged)

    Q_PROPERTY(bool isAccStatusOn READ isAccStatusOn WRITE setIsAccStatusOn NOTIFY isAccStatusOnChanged)

    Q_INVOKABLE void launchMapCareEngineeringMode( int screen );

    Q_INVOKABLE int getCureentTargetScreen() { return currentTargetScreen; }; //added for Rear Seat Control Spec Update

    void sendResponseFactoryReset();//added for DB File Crash Issue
public:
    int currentTargetScreen; //added for Rear Seat Control Spec Update
    ECountryVariant variantCountry() { return m_variantCounry; };
    int GetUishLanguage() { return m_currentLanguage; };
    bool usbUpdatePresent() {return m_usbUpdatePresent;};
    void SetLastChangedTime(SYSTEMTIME time);
    bool sendLanguageToIBox(LANGUAGE_T languageId, int targetScreen);
    void ResetAfterCloneCompleted();
    //QString inputtext(){return m_inputtext; }
    void Setinputtext( QString value);

    void keyboardSearchCodeSeleted();

    void NotifyVideoDefaultSetAck( uint deviceID, ushort senderHandle, SettingsDef::SETTINGS_VIDEO_DEFAULT_SET_ACK_T ackType );
    void showMotorErrorPopup();
    void showTimeZoneChangedToastPopup(QString sTimeZone);
    void setRTCInfoToMicom( SYSTEMTIME *time );
    void setGPSTimeSetting( bool isGpsTimeSetting ) {m_bIsGPSTimeSetting = isGpsTimeSetting;};
    bool getGPSTimeSetting() {return m_bIsGPSTimeSetting;};
    void requestGPSTimeToNavi();

    void NotifyFGReceivedToMobileTV();

    bool dcSwapped() { return m_bDCSwapped; }
    void setDcSwapped(bool value) { m_bDCSwapped = value; emit dcSwapChanged(); }

    bool parkingStatus() { return m_bParkingStatus; }
    void setParkingStatus(bool value) { m_bParkingStatus = value; emit parkingStatusChanged(); }

    bool isAccStatusOn() { return m_bAccStatusOn; }
    void setIsAccStatusOn(bool value) { m_bAccStatusOn = value; emit isAccStatusOnChanged(); }


    // Watch Dog
    static void signalHandelr(int signum);
    static void setSignalHandlerObject(SettingsAppEngine* m);
    static SettingsAppEngine* SigSettingsAppEngine;
    void signalConnection();
    void restart( int sig );

signals:
    void sigFactoryReset();
    void sigSetTimeZone(int timezone, bool isChangedByGPS);

    void dcSwapChanged ();
    void parkingStatusChanged ();

    void isAccStatusOnChanged ();

    void setStartState( QString mainState, QString nestedState, int targetScreen );
    void videoStatusChanged(int targetScreen, int enable);
    void showPopUpInfo( QString popupFileName, int targetScreen );

    void saveSettingsSignal( int value, int key, int targetScreen );
    void saveSettingsStringSignal( QString value, int key, int targetScreen );

    // general setting signals
    void sendRearScreenLockStatus(bool status);

    // system info settings signals  -->
    void sigJukeboxCapacityChanged(bool useTimer);
    void sigJukeboxImageInfo(int enable);
    void usbUpdatePresentChanged(bool isPresent);
    void sigNaviSdcardRemoved();
    void sigNaviSdcardMounted();
    // <--

    // display setting signals
    void sigCameraOnOff(int screen, int cameraOnOff);

    //void updatePhotoImg( QString path, int targetScreen );
    void keyEvent( int nEventKey, int nStatus, int targetScreen, int eventTarget );

    void saveVideoImageSettingsFromIBoxSignal( int brightness, int saturation, int contrast, int hue );

    void showListDVISearchSetting(int activeTab);
    void selectionDoneFromSearch();

    //void updateFrontScreenOnOffButton(bool isFrontScreenOn);

    //PopUp
    void showPopup( int type, int screen);
    void showToastPopup (int type, int screen);

    void showStartFocus( bool show, int targetScreen );
    //void hideFocus(int targetScreen);

    void resetScreen (int targetScreen );
    //void goBGState( int targetScreen );
    void mainFocusStateChanged( bool focusPresent, int targetScreen );


    //External Clock
    void accStatus(int value);
    void timePickerPopupStatus( bool isShow );

    //Rear State
    void SetLRearState(int value);
    void SetRRearState(int value);

    //for disp key
    void setStateLCD(int targetScreen);

    //for 'Set Aspect Ratio' when AppSetting goes FG
    void checkAspectRatioOnFG( int targetScreen );

    //void gpsAvailableChanged( bool isAvailable );

    //for Display Setting
    void updateDisplaySettingUI(int AppID);

    void isParkingModeChanged( bool isParking, int targetScreen );

    void chineKeypadChanged( int chineseKeypadType );

    void setTouchLock( bool bTouchLock );

    // added for modified ibox time set 02/05
    void SetTimeToIBox(uchar timeFormat, uint year, uint month,
                       uint day, uint hour, uint minute, uint second,
                       uint dateFormat);
    // added for modified ibox time set 02/05

public slots:
    virtual void NotifyFromUIStateManager( Event aEvent );
    virtual void NotifyFromUIStateManagerRear( Event aEvent );
    virtual void NotifyFromUIStateManagerCommon( Event aEvent);

    void NotifyFromUIStateManagerGeneric( Event &aEvent, AppEngineQMLConstants::EAPP_SCREEN_USAGE_T targetScreen );

    /**
     * request of save day night state
     */
    void SaveDayNightState(QDBusMessage message);
    void onTimeChangeTimeout();

    // for Rear Screen Lock
    void onMicomCarIFStatus (uchar mode, uchar status );

    void AskSysLastStatus();

    bool jukeboxImageEnable () { return m_bJukeboxEnable;}

    void notifyIsParkingMode();

Q_SIGNALS:
    void retranslateUi(int langId);

private:

    virtual APP_ID_T GetThisAppId() const { return APP_SETTINGS; }
    void SetLocalization( LANGUAGE_T languageId );
    void KeyEventHandler( SettingsDef::SETTINGS_EVT_KEY_T nEventKey,
                          Event &aEvent,
                          AppEngineQMLConstants::EAPP_SCREEN_USAGE_T targetScreen,
                          int EventTarget = 1);

    // for Rear Screen Lock
    void connectCarIFStatus();

    void getLcdStatusInfo();
    int DecimalToHexa(const uint value);

    void ResetNaviSettings();
    void ResetAllBTSettings();
    void ResetDVDSettings();

    void ResetXMAudioSettings();
    void ResetXMDataSettings();
    void ResetVRSettings(); //added for ISV 113930 VR Init Issue

    //! Set video setting from IBox
    void processVideoSettingFromIBox( Event aEvent, AppEngineQMLConstants::EAPP_SCREEN_USAGE_T targetScreen );
    void processAdjustScreenFromIBox( EVENT_SETTINGS_CHANGED_DATA_T *message );
    void processVideoDefaultSetFromIBox( EVENT_SETTINGS_CHANGED_DATA_T *message, AppEngineQMLConstants::EAPP_SCREEN_USAGE_T targetScreen );

    //void PlayerEnginrVideoBlendOn( int screen );
    //void PlayerEnginrVideoBlendOff( int screen );
    DualDisplayHelper::MEDIA_DISPLAY EffectiveDisplay();
    void receiveGPSTime( EVENT_NAVI_GPS_TIME_DATA_T *data );

    // Swap DC
    void UpdateViewMapping();


private:

    /*
    struct AppSettingsStates
    {
        char mainState[MAX_STATE_SIZE + 1];
        char nestedState[MAX_STATE_SIZE + 1];
    };
    */

    bool m_bFG_F;
    bool m_bFG_R;

    bool m_bJukeboxEnable;

    QTranslator *m_pTranslator;
    SettingsDef::SETTINGS_DISPLAY_T m_nDisplay;
    //static AppSettingsStates m_appStates[AppSettingsDef::eStart_Mode_MaxIndex];
    ECountryVariant m_variantCounry;
    LANGUAGE_T m_currentLanguage;

    //SETTINGS_ENTRY_POINT_T m_sCurrentSettingModeFront;
    //SETTINGS_ENTRY_POINT_T m_sCurrentSettingModeRear;

    CLONE_STATUS_T m_CloneStatus; // track system level clone status
    bool m_bFrontScreenShowing;
    bool m_bRearScreenShowing;
    bool m_bDCSwapped;             // track system level DCSwap status
    bool m_bParkingStatus;
    bool m_bViewContentSwapped; // flag whether m_bDCSwapped is applied onto myself

    bool m_bAccStatusOn;

    bool m_usbUpdatePresent;
    SETTINGS_DAYNIGHT_MODE m_nDayNightMode;
    uchar m_timeFormat;
    uint m_dateFormat;
    SYSTEMTIME m_lastChangedTime;
    SharedDataProvider *m_pShareDataProviderVideo;
    SharedDataProvider *m_pShareDataProviderDmb;
    QTimer *m_pIBoxTimer;
    QDBusInterface *m_pDBusinterface;
    QDBusReply<bool> m_DBusreply;
    int m_target;
    int m_FrontScreenBrightness;
    int m_Rear1ScreenBrightness;
    int m_RearScreenBrightness;
    //bool m_isVideoShowing;
    bool m_isFrontScreenOff;
    //QString m_inputtext;
    SETTINGS_CLONE_STATUS m_isVideoCloned; // @TODO : delete it
    DualDisplayHelper* m_pDDHelper;
    QHash<AppEngineQMLConstants::EAPP_SCREEN_USAGE_T, bool> m_transperentTarget;

    bool m_bIsGPSTimeSetting;
    bool m_bIsGPSAvailable;

    APP_ID_T m_AppID_forDisPlaySetting;

    // Last EntryPoint 저장.
    APP_ENTRY_POINT_T m_frontLastEntryPoint;
    APP_ENTRY_POINT_T m_rearLastEntryPoint;
    void setLastEntryPoint(APP_ENTRY_POINT_T lastEntryPoint ,AppEngineQMLConstants::EAPP_SCREEN_USAGE_T targetScreen);

    bool m_isCloneMode;
};

class LocalizeTrigger : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString empty READ empty NOTIFY retrigger);

public:
    QString empty() { return QString(); }

signals:
    void retrigger();
};

#endif // SETTINGS_APP_ENGINE_H
