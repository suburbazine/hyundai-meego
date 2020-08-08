#ifndef HOME_MENU_APP_ENGINE_H
#define HOME_MENU_APP_ENGINE_H

// for SW WDT
#include <signal.h>
#include <bits/siginfo.h>
//

#include <QtDBus/QDBusInterface>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDeclarativeView>
#include <QFileInfo>
#include <QApplication>
#include <QTimer>
#include <wchar.h>
#include <QSharedMemory>

#include <uistate_manager_def.h>
#include <DHAVN_AppSettings_Shared.h>
#include <DHAVN_AppHomeScreen_ViewController.h>
#include <DHAVN_AppHomeScreen_LocalizeTrigger.h>
#include <DHAVN_AppFramework_Event_Def.h>
#include <DHAVN_AppFramework_AppVextEngineQML_Def.h>
#include <DHAVN_AppMediaPlayer_Shared.h>
#include <DHAVN_AppHomeScreen_Log_Def.h>


class AppEngine: public AppVextEngineQML
{
   Q_OBJECT

public:

   AppEngine( InitData *pInitData,
              LocalizeTrigger *pLocTrigger,
              QObject *pParent = NULL );

   ~AppEngine();

   Q_PROPERTY ( int countryVariant READ countryVariant WRITE setCountryVariant NOTIFY countryVariantChanged )

   Q_INVOKABLE void LaunchApplication( int, int, int, int, QString );
   Q_INVOKABLE void BackKeyHandler(int);
   Q_INVOKABLE void hideLocalPopup(int screen);
   Q_INVOKABLE void setBeepCommand();
   Q_INVOKABLE void playBeep();

   Q_INVOKABLE QString getFont(bool hdb);

   Q_INVOKABLE void logForQML(QString log);

   // for SW WDT -->
   static void signalHandler (int signum);
   void signalConnection();
   static void setSignalHandlerObject (AppEngine* m);
   static AppEngine* SigM;

   void restart(int signum);
   // <--


public slots:
   void NotifyFromUIStateManager( Event aEvent );
   void NotifyFromUIStateManagerRear( Event aEvent );
   void NotifyFromUIStateManagerCommon( Event aEvent );
   void NotifyFromUIStateManagerGeneric( Event aEvent, ViewController *pViewController, AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T nScreen );

signals:
   //for QML
   void goMainMenu (int screen, bool animation);
   void goSubMenu (int screen, bool animation, int subMenuId, int focusIndex);
   void goHelpMenu (int screen);

   void setMediaMenuFocusIndex(int screen, int index);

   void sigAppLaunching(bool bLaunching, int screen);

   void receiveFGEvent(int screen);
   void receiveBGEvent(int screen);

   void signalBackKey();

   void closepopup(int screen);

   //for property
   void countryVariantChanged(int value);

private:
   APP_ID_T GetThisAppId() const { return APP_MAIN_MENU; }

   void LaunchHMISettings( int nEntryPoint, int nScreen );
   void LaunchBluelinkPhone( int nAppId, int nScreen, APP_ENTRY_POINT_T nState );

   void loadSubMenu (int nViewId, int nScreen, QString sViewName);

   void ProcessEntryPoint( APP_ENTRY_POINT_T nEntryPoint, ViewController *pViewController );
   void setStateToMainMenu(int screen, bool animation);
   void setStateToSubMenu(ViewController *pViewController,
                          HSDefP::VIEW_ID_T subMenuId,
                          QString subMenuTitle,
                          int screen,
                          bool animation,
                          HSDefP::APP_ID_T focusAppID = HSDefP::APP_ID_INVALID,
                          HSDefP::VIEW_ID_T focusViewID = HSDefP::VIEW_ID_INVALID );

   //for block user input during app launcing
   void appLaunching(bool bLaunching, int screen);

   QPair < EVT_ID_T, QString > GetAppInfo( const int nAppId );

   //for swapDC
   void UpdateViewControllerMapping();

   //bool getIBoxModemFlag();

   bool getUISHSharedMemoryInfo ( int offset,  unsigned char * pBuf,  int size);
   void updateMediaMenuIcons(EVT_MEDIA_ENABLE_DATA_T *pData);

   DISPLAY_T RemapScreenToDisplay( AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T nScreen );
   ViewController* GetViewController( AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T nScreen );


private slots:
   //void repaintView();
   void resetAppLaunchData();
   int countryVariant() { return m_nCountryVariant; }
   void setCountryVariant(int value) { m_nCountryVariant = value; emit countryVariantChanged(m_nCountryVariant); }

private:
   int m_nCountryVariant;

   bool m_bAppLaunching_F;
   bool m_bAppLaunching_R;

   int m_nMenuState_F;
   int m_nMenuState_R;

   APP_ENTRY_POINT_T m_eLastEntryPoint_F;
   APP_ENTRY_POINT_T m_eLastEntryPoint_R;

   bool m_bDCSwapped;
   bool m_bViewContentSwapped;

   //gui manage class
   InitData *m_pInitData;
   ViewController *m_pViewControllerFront;
   ViewController *m_pViewControllerRear;
   LocalizeTrigger *m_pLocTrigger;


   QSharedMemory m_uishSharedMemory;

   // beep
   QDBusMessage m_BeepMsg;


//public:
//   static int Command_Field;
};

#endif // HOME_MENU_APP_ENGINE_H
