#ifndef DHAVN_APPFRAMEWORK_APPSCREENVIEW_H
#define DHAVN_APPFRAMEWORK_APPSCREENVIEW_H

#include <QDeclarativeView>
#include <QDeclarativeContext>
#include "DHAVN_AppFramework_AppEngineBase.h"
#include "DHAVN_AppFramework_QML_Def.h"
#include "DHAVN_AppFramework_Event_Def.h"

#define DH_STATUS_BAR      0
#define DH_X_FRONT         0
#define DH_Y_FRONT         0 + DH_STATUS_BAR
#define DH_X_REAR          0
#define DH_Y_REAR          720 + DH_STATUS_BAR
#define DH_SCREEN_WIDTH    1280
#define DH_SCREEN_HEIGHT   720 - DH_STATUS_BAR

const QString POPUP_QML_PLUGIN_PATH = "/usr/lib/qt4/imports/QmlPopUpPlugin/qml/DHAVN_PopUp_PopUps.qml";

class AppScreenView : public QDeclarativeView
{
    Q_OBJECT
public:
	/** Constructor */
	AppScreenView(AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T nCurrentScreen, bool bEnableOpenGl, int nYOffset, QWidget *parent = 0, bool bEnableHandleTouch = false );
    /** Constructor */
    AppScreenView(const AppScreenView&);
    /** Destructor */
    ~AppScreenView();

    /**
      * Return current screen into qml
      */
    Q_INVOKABLE int getCurrentScreen();

    /**
      * Return qml avalibale on sceen or not
      */
    bool IsLoaded();

    /**
     * Load qml files in tree
     */
    void SetSource(QString sSource);

    /**
     * Move qml tree on nCurrentScreen place
     */
    void SetCurrentScreen( AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T nCurrentScreen );

    /**
     * Call  system popup on qml tree context
     */
    void ShowSystemPopup(Event &aEvent, APP_ID_T nAppID);
    void HideSystemPopup(bool bPressedButton);

    /**
     * Process all jog events on qml
     */
    void SendJogEvent( AppEngineQMLConstants::EJOG_EVENTS_T key, int keyStatus, bool bRRC = false );

    /**
     * Notify qml plugis about FG/BG events
     */
    void SendFG();

    void SendBG();

    void CancelKey();

    void SendLanguageChanged(int language);

    /**
      * Manual BEEP from QML
      */
    Q_INVOKABLE void ManualBeep();

    /**
      * Change set screen
      */
    void ChangeSetScreen( AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T nSetScreen );

    void SetOffset(int nOffset);

    /**
    * Returns country variant for ran application
    */
    Q_INVOKABLE int GetCountryVariantFromQML() { return (int)AppEngineBase::GetCountryVariant(); }

    Q_INVOKABLE int GetLanguageFromQML() { return (int)AppEngineBase::GetLanguage(); }

protected:
	virtual bool  viewportEvent ( QEvent * event );

Q_SIGNALS:
   /**
    * Jog events
    */
   void signalJogCenterClicked( bool bRRC );
   void signalJogCenterPressed( bool bRRC );
   void signalJogCenterReleased( bool bRRC );
   void signalJogCenterLongPressed( bool bRRC );
   void signalJogCenterCriticalPressed( bool bRRC );
   void signalJogNavigation( uint arrow, int status, bool bRRC, int nRepeat );

   void signalPopupJogCenterClicked( bool bRRC );
   void signalPopupJogCenterPressed( bool bRRC );
   void signalPopupJogCenterReleased( bool bRRC );
   void signalPopupJogCenterLongPressed( bool bRRC );
   void signalPopupJogCenterCriticalPressed( bool bRRC );
   void signalPopupJogNavigation( uint arrow, int status, bool bRRC );

   /**
    * FG/BG events to  qml plugins
    */
   void fgReceived();
   void bgReceived();

	void HandleTouchEvent(QEvent * event);

   /**
    * Popup key response
    */
   void HandlePopupResponse( int, QVariant );

   /**
     * Notification about BEEP for VextEngine
     */
   void signalManualBeep();

   //system popup
   void signalShowSystemPopup();
   void signalHideSystemPopup();

   void signalLanguageChanged(int lang);

private:

   void UpdateGeometry();
   bool m_bIsLoaded;
   bool m_bIsDisplayedPopup;
   uint m_nKeyPrev;
   int m_nKeyStatePrev;
   bool m_bRRCPrev;
   bool m_bEnableHandleTouch;
   QObject* m_pPopupObj;
   AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T m_nCurrentScreen;
   AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T m_nSetScreen;
   int m_nYOffset;
};

#endif // DHAVN_APPFRAMEWORK_APPSCREENVIEW_H
