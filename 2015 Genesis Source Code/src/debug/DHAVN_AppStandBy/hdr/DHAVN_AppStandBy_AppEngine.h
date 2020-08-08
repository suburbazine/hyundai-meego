#ifndef STANDBY_APP_ENGINE_H
#define STANDBY_APP_ENGINE_H

#include <QDeclarativeView>
#include <QTranslator>
#include <QApplication>
#include <QGraphicsObject>
#include <QDeclarativePropertyMap>
#include <QtDBus/QtDBus>
#include <DHAVN_AppStandBy_Log_Def.h>

/* WDT */
#include <signal.h>
#include <bits/siginfo.h>

#include <DHAVN_AppFramework_AppVextEngineQML_Def.h>
#include <DHAVN_AppStandBy_Constants.h>

/* Clock */
#include <QTimer>
#include <QDateTime>
#include "DHAVN_AppStandBy_LoadSettings.h"
#include <uistate_manager_def.h>
#include <QmlStatusBar/osdwriter.h>

#define SHM_LANGUAGE_OPTION_SIZE					(sizeof(char)*4)
#define SHM_SYS_OPTION_SIZE							(sizeof(char)*9)
#define SHM_SYS_BOOT_TYPE_SIZE						(sizeof(char)*4)
#define SHM_LAST_STATUS_SIZE						(sizeof(char)*3)
#define SHM_AUDIO_LAST_MEMORY_SIZE					(sizeof(char)*8)
#define SHM_CURRENT_AV_MODE_SIZE					(sizeof(char)*4)
#define SHM_POPUP_STATUS_SIZE                       (sizeof(char)*4)
#define SHM_MEDIA_WRAPPER_OWNER_SIZE                (sizeof(char)*4)
#define SHM_CALL_STATUS_SIZE                (sizeof(char)*4)
#define SHM_SETTING_VALUE_SIZE                (sizeof(char)*4)

#define SHM_OFFSET_LANGUAGE_OPTION					(0)
#define SHM_OFFSET_SYS_OPTION						(SHM_LANGUAGE_OPTION_SIZE)
#define SHM_OFFSET_SYS_BOOT_TYPE					(SHM_OFFSET_SYS_OPTION+SHM_SYS_OPTION_SIZE)
#define SHM_OFFSET_LAST_STATUS						(SHM_OFFSET_SYS_BOOT_TYPE+SHM_SYS_BOOT_TYPE_SIZE)
#define SHM_OFFSET_AUDIO_LAST_MEMORY				(SHM_OFFSET_LAST_STATUS+SHM_LAST_STATUS_SIZE)
#define SHM_OFFSET_CURRENT_AV_MODE					(SHM_OFFSET_AUDIO_LAST_MEMORY+SHM_AUDIO_LAST_MEMORY_SIZE)
#define SHM_OFFSET_POPUP_STATUS                     (SHM_OFFSET_CURRENT_AV_MODE+SHM_CURRENT_AV_MODE_SIZE)
#define SHM_OFFSET_MEDIA_WRAPPER_OWNER              (SHM_OFFSET_POPUP_STATUS+SHM_POPUP_STATUS_SIZE)
#define SHM_OFFSET_CALL_STATUS              (SHM_OFFSET_MEDIA_WRAPPER_OWNER+SHM_MEDIA_WRAPPER_OWNER_SIZE)
#define SHM_OFFSET_SETTING_VALUE              (SHM_OFFSET_CALL_STATUS+SHM_CALL_STATUS_SIZE)

class AppEngineEnum : public QObject
{
   Q_OBJECT
   Q_ENUMS( APP_DISPLAY_T );

public:
    typedef enum
    {
       APP_DISPLAY_FRONT = 1,
       APP_DISPLAY_REAR,
       APP_DISPLAY_NONE
    }APP_DISPLAY_T;
};

struct SDate{
    ushort year;
    uchar month;
    uchar day;
    uint datetype;
};

class StandByAppEngine: public AppVextEngineQML
{
   Q_OBJECT

    Q_PROPERTY( QString empty READ empty NOTIFY retranslateUI )

public:
   StandByAppEngine( int &argc, char **argv, QObject *pParent = NULL );
   ~StandByAppEngine();

   void setEvv( int evv ) { m_iEvv = evv; };
   void callFunctionAfterBoot();
   bool GetSharedMemoryInfo(int offset, unsigned char * pBuf, int size);

    /* Beep */
    QDBusMessage msg;

   Q_INVOKABLE void LaunchApplication( int appId, int nDisplay );
   Q_INVOKABLE void setTime( bool pressed );
   Q_INVOKABLE int GetLanguageID();
//   Q_INVOKABLE QString GetStrConst( int id );
   Q_INVOKABLE bool CheckNaviStatus();
   Q_INVOKABLE int CheckVehicleStatus();
   QString empty() { return QString(); }
   QTranslator m_pTranslator;
   Q_INVOKABLE void HandleBackLanguage( );
   Q_INVOKABLE void ChangeLanguage(int language);
   Q_INVOKABLE void playAudioBeep();
   Q_INVOKABLE void showToastPopup(bool visible );
   Q_INVOKABLE int getEvv();

   /* Clock */
   LoadStandBySetting * m_pLoadStandBySetting;
   void getSettingsFromDB();
   void getSharedMemoryFromUISH();
   int getPhotoFrame();
   int getCountryVariant();
   bool getLogicOff();
   bool getPowerOff();
   Q_INVOKABLE void HandleBackKey();
   void GetTimeFormat();
   void showTimeZoneChangedToastPopup(bool);
   void setAutoTimeZone(int timezone );
   void setTimeZone(int timezone);
   void updateTimeZoneToSystem(bool isShowToastPopup);
   void sendTimeAndDateToMOST();

   // WDT //
   static void signalHandler( int signum );
   void signalConnection();
   static void setSignalHandlerObject( StandByAppEngine * m );
   static StandByAppEngine* SigM;
   void restart( int sigNum );

   //For black screen of PWR OFF in boot time.
   //this map sends variant info to QML
   QDeclarativePropertyMap engineToQml;
   void setIsPowerStateOn(bool is);
   void ConnectPowerManagement();

   /* Clock */
signals:
   void setFG();
   void setFile( bool );
   void appFG(bool val);
   void sendTimeoutToUpdateClock();
   void sendTimeoutToExternalClock();
   void setGPSTime(bool);
   void settingsChanged( EVENT_SETTINGS_CHANGED_DATA_T * message );

Q_SIGNALS:
   void retranslateUI();
   void signalLoadingCompleted();
   void signalAutoDisappearTimer( bool status , int screenId );
//   void signalFG();
   void signalLangChanged();

   void signalSetLanguage(bool mainScreen);
   void setFocusIndex( int index, int screenId );
   void signalEmergency( bool bEmergen);
   void setLaunchApp( bool launchApp );
   void signalSetDefaultFocus();
   void signalFg();

//   void qmlLoad( bool standby, int screenId );              // standby or clock
   void qmlLoad( bool state, int screenId );              // standby or clock
   void setRearLogo( bool setRear , int screenId );         // logo or main
   void clockFG( bool status );
   void afterBoot();
   void signalJog( int status , int arrow , int screenId);
   void setAutoDisappear( bool disappear );
   void backKeyPressed(int screenId);

public slots:
   virtual void NotifyFromUIStateManager( Event aEvent );
   virtual void NotifyFromUIStateManagerRear( Event aEvent );
   virtual void NotifyFromUIStateManagerCommon ( Event aEvent );
   void timeoutTimer();
//   void setSummerTime(bool value) { m_bSummerTime = value; emit summerTimeChanged(); }

   //For black screen of PWR OFF in boot time.
   void ChangedPowerState(QDBusMessage);

private:
   void NotifyFromUIStateManagerCommonGeneric ( Event aEvent );
   void SetLocalization( LANGUAGE_T languageId );
   void receiveGPSTime( EVENT_NAVI_GPS_TIME_DATA_T *data );
   void setRTCInfoToMicom( SYSTEMTIME *time );
   virtual APP_ID_T GetThisAppId() const { return APP_STANDBY; } // need change to APP_STAND_BY

   AppEngineEnum::APP_DISPLAY_T m_nDisplay;
   ECountryVariant m_nCountryVariant;
   LANGUAGE_T m_nLanguage;
   int m_nVehicle;
   int m_nCntTimeout;
   bool bIsMainScreen;
   bool bEmergency;
   bool m_isSwapDC;
   bool m_isFG;
   bool m_bPowerOff;
   bool m_nAutoDisappear;
   bool m_isCamera;
   bool m_toastPopupVisible;
   bool m_isNormalOn;
   bool m_bResetTimer;
   bool m_receivedPostEvent;
   bool m_bIsGPSTimeSetting;
   bool m_bIsGPSAvailable;
   int m_nFocusIndex;
   QDBusInterface *m_pDBusinterface;
   QDBusReply<bool> m_DBusreply;

   int m_iEvv;

   /* Clock */
   bool m_bFirstClockFG;
   bool m_bLogicOff;
   bool m_bSummerTime;
   int m_nAutoTimeZone;
   int m_pTimeFormat;
   int m_nPhotoFrame;
   bool m_bSendMostTime;
   QTimer *m_timer;

   //For black screen of PWR OFF in boot time.
   bool m_isPowerStateOn;
   bool m_isAgreeShowOsd;
};

#endif // STANDBY_APP_ENGINE_H
