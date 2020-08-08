/**
 * DHAVN_AppFramework_AppEngineQML.h
 *
 * Declaration of the class AppEngineQML
 */

#ifndef APPENGINEQML_H
#define APPENGINEQML_H

#include "DHAVN_AppFramework_AppEngineBase.h"
#include "DHAVN_AppFramework_QML_Def.h"
#include <QVariant>

class AppEngineQMLPrivate;

class APP_FRAMEWORK_EXPORT AppEngineQML : public AppEngineBase
{
   Q_OBJECT

public:
//!!!!!!!!!!!!!!!!
   /** Only for test purpose */
   Q_INVOKABLE void testKeyClickEvents( int arrow, int status );
//!!!!!!!!!!!!!!!!

   /** Constructor */
   AppEngineQML( QDeclarativeView *pQMLView, QObject *pParent = NULL, APP_ENGINE_TYPE nType = APP_QML );

   /** Constructor */
   AppEngineQML( QDeclarativeView *pQMLView, QString aModuleName, QObject *pParent = NULL, APP_ENGINE_TYPE nType = APP_QML );

   /** Destructor */
   ~AppEngineQML();

   /**
    * Returns the current application view as declarative QML view object.
    */
   QDeclarativeView * GetCurrentView();

   /**
    * Applications need to oveerride this and provide their application ID which
    * must be defined in @ref APP_ID_T.
    */
   virtual APP_ID_T GetThisAppId() const = 0;

   Q_INVOKABLE void ManualBeep();

   void HandleAutoVisibility(uint nScreen);

Q_SIGNALS:
   void signalJogCenterClicked( bool bRRC );
   void signalJogCenterPressed( bool bRRC );
   void signalJogCenterReleased( bool bRRC );
   void signalJogCenterLongPressed( bool bRRC );
   void signalJogCenterCriticalPressed( bool bRRC );
   void signalJogNavigation( uint arrow, int status,  bool bRRC, int nRepeat );

   void signalPopupJogCenterClicked( bool bRRC );
   void signalPopupJogCenterPressed( bool bRRC );
   void signalPopupJogCenterReleased( bool bRRC );
   void signalPopupJogCenterLongPressed( bool bRRC );
   void signalPopupJogCenterCriticalPressed( bool bRRC );
   void signalPopupJogNavigation( uint arrow, int status, bool bRRC );

public slots:
   /** Popup response */
   void handlePopupResponse( int target, QVariant response );  

Q_SIGNALS:
   void fgReceived();
   void bgReceived();

   //system popup
   void signalShowSystemPopup();
   void signalHideSystemPopup();

   void signalLanguageChanged(int lang);
    
protected:
   /**
    * Used to handle events. This is used for internal handling and shall not
    * be available for applications.
    */
   virtual bool HandleEvent( Event &aEvent );

   virtual bool HandleEventRear( Event &aEvent );

   void sendJogEvent( AppEngineQMLConstants::EJOG_EVENTS_T key,
                      int keyStatus, bool bRRC = false );
   bool HandleEventEngine( Event &aEvent );

private:
   void hidePopup(bool bPressedButton);
   void CancelKey();

    void DoFrameBufferTransition(uint nscreen);

#ifdef REMOVE_APPTOPWIDGET
private slots:
   void SlotCallFgResponse();
#endif
private slots:
    void DoTimedTransition();

private:
   virtual void Reserved_1();
   virtual void Reserved_2();

private:
   //Private class object for AppEngineQML
   AppEngineQMLPrivate * d_ptr;
   Q_DECLARE_PRIVATE(AppEngineQML)
public:
   void handleDumyPopupResponse();
};

#endif // APPENGINEQML_H
