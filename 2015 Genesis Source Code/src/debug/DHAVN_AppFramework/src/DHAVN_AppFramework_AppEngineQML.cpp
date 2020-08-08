#include "DHAVN_AppFramework_AppEngineQML_Def.h"
#include "DHAVN_AppFramework_AppScreenView.h"
#include <QGraphicsItem>
#include <QDeclarativeItem>
#include <QKeyEvent>
#include <QList>
#include <QGraphicsScene>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include "DHAVN_AppFramework_Popup.h"
#include "math.h"

#ifdef REMOVE_APPTOPWIDGET 
#include <QTimer>
//#define FG_QML_TIMEOUT 250
#define FG_QML_TIMEOUT 50
#define FG_QML_EXTEND_TIMEOUT 800
#endif

#define LONG_WELL_SIZE 10

#define REMOVE_CLICK

/** AppEngineQMLPrivate class definition */
class AppEngineQMLPrivate
{

private:

    /** Constructor */
    AppEngineQMLPrivate( AppEngineQML *pParent )
    :m_pPopupObj(NULL),
#ifdef REMOVE_APPTOPWIDGET 
     m_pFGResponseTimer(NULL), m_nDisplay(DISPLAY_NONE),
#endif
     m_nKeyPrev(AppEngineQMLConstants::JOG_NONE),
     m_nKeyStatePrev(AppEngineQMLConstants::KEY_STATUS_NONE),
     m_bRRCPrev(false),
     m_bFrontOnFront(false), m_bFrontOnRear(false), 
     m_bWillBeClone(false), m_bWillBeCloneOff(false),
     m_bDoSwap(false), m_bFBTransitionOnResponse(false),
     m_bLateFBTransition(false), m_nDisplay4LateTransition(0),
     m_bPopupCloseIgnore(false),
     q_ptr(pParent)
    {
#ifdef REMOVE_APPTOPWIDGET
        m_pFGResponseTimer = new QTimer;
#endif
        memset(&m_Popup, 0, sizeof(POPUP_SHOW_T));
        m_Popup.popup_type = _CURRENT_POPUP_NONE;
    }

       
    /** Destructor */
    virtual ~AppEngineQMLPrivate() {}

public:
   QObject* m_pPopupObj;
#ifdef REMOVE_APPTOPWIDGET
   QTimer* m_pFGResponseTimer;
   DISPLAY_T m_nDisplay;
#endif
   int m_nKeyPrev;
   int m_nKeyStatePrev;
   bool m_bRRCPrev;
   bool m_bFrontOnFront;
   bool m_bFrontOnRear;
   bool m_bWillBeClone;
   bool m_bWillBeCloneOff;
   bool m_bDoSwap;
   bool m_bFBTransitionOnResponse;
   bool m_bLateFBTransition;
   int m_nDisplay4LateTransition;
   bool m_bPopupCloseIgnore;

   POPUP_SHOW_T m_Popup;
   //Q-Pointer to the AppEngineQML class
   AppEngineQML * const q_ptr;
   Q_DECLARE_PUBLIC(AppEngineQML)
};

AppEngineQML::AppEngineQML( QDeclarativeView *pQMLView, QObject *pParent, APP_ENGINE_TYPE nType )
: AppEngineBase( pQMLView, pParent, nType )
{
   d_ptr = new AppEngineQMLPrivate(this);

   qmlRegisterType<AppEngineQMLConstants>("AppEngineQMLConstants", 1, 0, "UIListenerEnum");
   SetIsDisplayedPopup(DISPLAY_FRONT_REAR, false );
#ifdef REMOVE_APPTOPWIDGET
   ((QTimer*)d_ptr->m_pFGResponseTimer)->setInterval(FG_QML_TIMEOUT);
   connect((QTimer*)d_ptr->m_pFGResponseTimer, SIGNAL(timeout()),this, SLOT(SlotCallFgResponse()));
#endif
}

AppEngineQML::AppEngineQML( QDeclarativeView *pQMLView, QString aModuleName, QObject *pParent, APP_ENGINE_TYPE nType )
: AppEngineBase( pQMLView, aModuleName, pParent, nType )
{
   d_ptr = new AppEngineQMLPrivate(this);

   qmlRegisterType<AppEngineQMLConstants>("AppEngineQMLConstants", 1, 0, "UIListenerEnum");
   SetIsDisplayedPopup(DISPLAY_FRONT_REAR, false );
#ifdef REMOVE_APPTOPWIDGET
   ((QTimer*)d_ptr->m_pFGResponseTimer)->setInterval(FG_QML_TIMEOUT);
   connect((QTimer*)d_ptr->m_pFGResponseTimer, SIGNAL(timeout()),this, SLOT(SlotCallFgResponse()));
#endif
}

AppEngineQML::~AppEngineQML()
{
    delete d_ptr;
}

QDeclarativeView *
AppEngineQML::GetCurrentView()
{
   return dynamic_cast< QDeclarativeView * >( GetAppView() );
}

void AppEngineQML::testKeyClickEvents( int arrow, int status )
{
	int nRepeat = 1;
   if ( arrow == AppEngineQMLConstants::JOG_CENTER )
   {
      switch ( status )
      {
         case AppEngineQMLConstants::KEY_STATUS_CLICKED:
            emit signalJogCenterClicked(false);
            break;
         case AppEngineQMLConstants::KEY_STATUS_PRESSED:
            emit signalJogCenterPressed(false);
            break;
         case AppEngineQMLConstants::KEY_STATUS_RELEASED:
            emit signalJogCenterReleased(false);
            break;
         case AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED:
            emit signalJogCenterLongPressed(false);
            break;
         case AppEngineQMLConstants::KEY_STATUS_CRITICAL_PRESSED:
            emit signalJogCenterCriticalPressed(false);
            break;
      }
   }

   emit signalJogNavigation( arrow, status, false, nRepeat );
}

void
AppEngineQML::sendJogEvent( AppEngineQMLConstants::EJOG_EVENTS_T key, int keyStatus, bool bRRC )
{
	int nRepeat = keyStatus;
   GetLogger().Log( QString("AppEngineQML::sendJogEvent key=%1 keyStatus=%2 bRRC=%2").arg((int)key).arg(keyStatus).arg(bRRC) );
   if ( key == AppEngineQMLConstants::JOG_WHEEL_LEFT ||
        key == AppEngineQMLConstants::JOG_WHEEL_RIGHT )
   {
      /*
      for( int index = 1; index <= keyStatus - 1; index++ )
      {
         if( GetIsDisplayedPopup(DISPLAY_FRONT_REAR) )
            emit signalPopupJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_CLICKED, bRRC );
         else
            emit signalJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_CLICKED, bRRC, nRepeat );
      }
      */
      keyStatus = AppEngineQMLConstants::KEY_STATUS_CLICKED;
   }
   else if ( key == AppEngineQMLConstants::JOG_CENTER )
   {
      switch ( keyStatus )
      {
         case KEY_STATUS_PRESSED:
            keyStatus = AppEngineQMLConstants::KEY_STATUS_PRESSED;
            if( GetIsDisplayedPopup(DISPLAY_FRONT_REAR) )
               emit signalPopupJogCenterPressed(bRRC);
            else
               emit signalJogCenterPressed(bRRC);
            break;

         case KEY_STATUS_RELEASED:
            keyStatus = AppEngineQMLConstants::KEY_STATUS_RELEASED;

            if( d_ptr->m_nKeyPrev != key ) {
                GetLogger().Log( "key cancel : release without press ", Logger::PRIORITY_LOW);
                return;
            }

            if ( d_ptr->m_nKeyPrev == key &&
                 d_ptr->m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_PRESSED )
            {
               if( GetIsDisplayedPopup(DISPLAY_FRONT_REAR) )
               {
                  emit signalPopupJogCenterClicked(bRRC);
                  emit signalPopupJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_CLICKED, bRRC );
               }
               else
               {
                  emit signalJogCenterClicked(bRRC);
                  emit signalJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_CLICKED, bRRC, nRepeat );
               }
            }

            if( GetIsDisplayedPopup(DISPLAY_FRONT_REAR) )
               emit signalPopupJogCenterReleased(bRRC);
            else
               emit signalJogCenterReleased(bRRC);
            break;

         case KEY_STATUS_PRESSED_LONG:
            keyStatus = AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED;
            if( GetIsDisplayedPopup(DISPLAY_FRONT_REAR) )
               emit signalPopupJogCenterLongPressed(bRRC);
            else
               emit signalJogCenterLongPressed(bRRC);
            break;

         case KEY_STATUS_PRESSED_CRITICAL:
            keyStatus = AppEngineQMLConstants::KEY_STATUS_CRITICAL_PRESSED;
            if( GetIsDisplayedPopup(DISPLAY_FRONT_REAR) )
               emit signalPopupJogCenterCriticalPressed(bRRC);
            else
               emit signalJogCenterCriticalPressed(bRRC);
            break;
         case KEY_STATUS_CANCEL:
            CancelKey();
            break;

         default:
            key = AppEngineQMLConstants::JOG_NONE;
            keyStatus = AppEngineQMLConstants::KEY_STATUS_NONE;
      }
   }
   else
   {
      if ( d_ptr->m_nKeyPrev == key &&
           keyStatus == KEY_STATUS_RELEASED &&
           d_ptr->m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_PRESSED )
      {
         if( GetIsDisplayedPopup(DISPLAY_FRONT_REAR) )
            emit signalPopupJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_CLICKED, bRRC );
         else
            emit signalJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_CLICKED, bRRC, nRepeat );
      }
      keyStatus += AppEngineQMLConstants::KEY_STATUS_PRESSED;
   }

   d_ptr->m_nKeyPrev = key;
   d_ptr->m_nKeyStatePrev = keyStatus;
   d_ptr->m_bRRCPrev = bRRC;

#ifdef REMOVE_CLICK
   if( (key == AppEngineQMLConstants::JOG_WHEEL_LEFT || key == AppEngineQMLConstants::JOG_WHEEL_RIGHT) &&
        keyStatus == AppEngineQMLConstants::KEY_STATUS_CLICKED ) {
       if( GetIsDisplayedPopup(DISPLAY_FRONT_REAR) )
          emit signalPopupJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_PRESSED, bRRC );
       else
          emit signalJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_PRESSED, bRRC, nRepeat );
   }
#endif

   if( GetIsDisplayedPopup(DISPLAY_FRONT_REAR) )
      emit signalPopupJogNavigation( key, keyStatus, bRRC );
   else
      emit signalJogNavigation( key, keyStatus, bRRC, nRepeat );

#ifdef REMOVE_CLICK
   if( (key == AppEngineQMLConstants::JOG_WHEEL_LEFT || key == AppEngineQMLConstants::JOG_WHEEL_RIGHT) &&
        keyStatus == AppEngineQMLConstants::KEY_STATUS_CLICKED ) {
       if( GetIsDisplayedPopup(DISPLAY_FRONT_REAR) )
          emit signalPopupJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_RELEASED, bRRC );
       else
          emit signalJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_RELEASED, bRRC, nRepeat );
   }
#endif
}

void
AppEngineQML::hidePopup(bool bPressedButton)
{
   if ( d_ptr->m_pPopupObj && GetIsDisplayedPopup(DISPLAY_FRONT_REAR) )
   {
      SetIsDisplayedPopup( DISPLAY_FRONT_REAR, false );
      emit signalHideSystemPopup();
      QMetaObject::invokeMethod( d_ptr->m_pPopupObj, "hidePopUpInContext", Q_ARG(QVariant, (int)bPressedButton) );
   }
}

bool
AppEngineQML::HandleEventEngine( Event &aEvent )
{
   bool result = true;
   EVT_ID_T nEventID = aEvent.GetEventId();
   KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
   GetLogger().Log( "---> AppEngineQML::HandleEventEngine()" );

   switch ( nEventID )
   {
      // This event could be supported on QML applications only, since POPUP plugin based on QML
      case EVT_HIDE_POPUP:
      {
         memset(&d_ptr->m_Popup, 0, sizeof(POPUP_SHOW_T));
         d_ptr->m_Popup.popup_type = _CURRENT_POPUP_NONE;
         hidePopup(true);
         break;
      }

      case EVT_SHOW_POPUP:
      {
         POPUP_SHOW_T* popupData = (POPUP_SHOW_T *)(aEvent.Data().data());
         if(popupData->ignoreautoclose == true)
             d_ptr->m_bPopupCloseIgnore = true;
         else
             d_ptr->m_bPopupCloseIgnore = false;

         if ( d_ptr->m_pPopupObj == NULL && GetCurrentView() != NULL )
         {
            QObject *pRootObject = dynamic_cast< QObject * >( GetCurrentView()->rootObject() );
            QDeclarativeItem *rootItem = qobject_cast<QDeclarativeItem *>( pRootObject );
            QDeclarativeComponent component( GetCurrentView()->engine(), QUrl::fromLocalFile( POPUP_QML_PLUGIN_PATH ) );

            d_ptr->m_pPopupObj = component.create();

            QDeclarativeItem *item = qobject_cast<QDeclarativeItem *>( d_ptr->m_pPopupObj );
            item->setParentItem( rootItem );

            connect( d_ptr->m_pPopupObj, SIGNAL( popup_Response( int, QVariant ) ),this, SLOT( handlePopupResponse( int, QVariant ) ) );
         }

         if ( d_ptr->m_pPopupObj )
         {
            CancelKey();
            CancelHwKey(DISPLAY_COMMON);
            SetIsDisplayedPopup( DISPLAY_FRONT_REAR, true );
            emit signalShowSystemPopup();

            if((d_ptr->m_Popup.popup_type != popupData->popup_type) && (d_ptr->m_Popup.popup_type != _CURRENT_POPUP_NONE))
            {
                handleDumyPopupResponse();
                memcpy( &d_ptr->m_Popup,popupData,sizeof(POPUP_SHOW_T));
            }
            else
            {

                memcpy( &d_ptr->m_Popup,popupData,sizeof(POPUP_SHOW_T));
            }
            QMetaObject::invokeMethod( d_ptr->m_pPopupObj, "showPopUpInContext", Q_ARG(QVariant, aEvent.Data()),  Q_ARG(QVariant, (int)GetThisAppId()) );


         }

         break;
      }

      case EVT_REQUEST_FG:
      {
         GetLogger().Log( "---> AppEngineQML::HandleEventEngine() - EVT_REQUEST_FG" );
         memset(&d_ptr->m_Popup, 0, sizeof(POPUP_SHOW_T));
         d_ptr->m_Popup.popup_type = _CURRENT_POPUP_NONE;
         emit fgReceived();
         break;
      }
      case EVT_REQUEST_BG:
      {
         GetLogger().Log( "---> AppEngineQML::HandleEventEngine() - EVT_REQUEST_BG" );
         hidePopup(false);
         emit bgReceived();
         break;
      }

      case EVT_LANGUAGE_CHANGED:
      {
         EVENT_LANGUAGE_CHANGED_DATA_T *lang = (EVENT_LANGUAGE_CHANGED_DATA_T *)aEvent.Data().data();
         if( lang != NULL ) {
            GetLogger().Log( QString("--->AppEngineQML::HandleEventEngine() - EVT_LANGUAGE_CHANGED (%1)").arg(lang->languageId) );
            m_Language = lang->languageId;
            emit signalLanguageChanged((int)lang->languageId);
         }
         break;
      }

/** ----- Jog events ----- */
      case EVT_KEY_CCP_JOG_DIAL_UP:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_UP,
                       message->keyStatus, false );
         break;
      }

      case EVT_KEY_RRC_JOG_DIAL_UP:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_UP,
                       message->keyStatus, true );
         break;
      }

      case EVT_KEY_CCP_JOG_DIAL_DOWN:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_DOWN,
                       message->keyStatus, false );
         break;
      }

      case EVT_KEY_RRC_JOG_DIAL_DOWN:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_DOWN,
                       message->keyStatus, true );
         break;
      }

      case EVT_KEY_CCP_JOG_DIAL_LEFT:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_LEFT,
                       message->keyStatus, false );
         break;
      }
        
      case EVT_KEY_RRC_JOG_DIAL_LEFT:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_LEFT,
                       message->keyStatus, true );
         break;
      }

      case EVT_KEY_CCP_JOG_DIAL_RIGHT:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_RIGHT,
                       message->keyStatus, false );
         break;
      }

      case EVT_KEY_RRC_JOG_DIAL_RIGHT:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_RIGHT,
                       message->keyStatus, true );
         break;
      }

      case EVT_KEY_CCP_JOG_DIAL_CENTER:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_CENTER,
                       message->keyStatus, false );
         break;
      }

      case EVT_KEY_RRC_JOG_DIAL_CENTER:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_CENTER,
                       message->keyStatus, true );
         break;
      }

      case EVT_KEY_CCP_JOG_DIAL_TOP_RIGHT:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_TOP_RIGHT,
                      message->keyStatus, false );
         break;
      }
      
      case EVT_KEY_RRC_JOG_DIAL_TOP_RIGHT:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_TOP_RIGHT,
                      message->keyStatus, true );
         break;
      }

      case EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_BOTTOM_RIGHT,
                      message->keyStatus, false );
         break;
      }

      case EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_BOTTOM_RIGHT,
                      message->keyStatus, true );
         break;
      }
      
      case EVT_KEY_CCP_JOG_DIAL_TOP_LEFT:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_TOP_LEFT,
                     message->keyStatus, false );
         break;
      }

      case EVT_KEY_RRC_JOG_DIAL_TOP_LEFT:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_TOP_LEFT,
                     message->keyStatus, true );
         break;
      }
      
      case EVT_KEY_CCP_JOG_DIAL_DOWN_LEFT:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_BOTTOM_LEFT,
                       message->keyStatus, false );
         break;
      }
      
      case EVT_KEY_RRC_JOG_DIAL_DOWN_LEFT:
      {
         sendJogEvent( AppEngineQMLConstants::JOG_BOTTOM_LEFT,
                       message->keyStatus, true );
         break;
      }

      case EVT_KEY_CCP_JOG_DIAL_ENCODER:
      {
           if( (int)aEvent.GetCommand() > 0 )
           {
              sendJogEvent( AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                            abs((int)aEvent.GetCommand()), false );
           }
           else
           {
              sendJogEvent( AppEngineQMLConstants::JOG_WHEEL_LEFT,
                            abs((int)aEvent.GetCommand()), false );
           }
           break;
      }

      case EVT_KEY_RRC_JOG_DIAL_ENCODER:
      {
         if( (int)aEvent.GetCommand() > 0 )
         {
            sendJogEvent( AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                          abs((int)aEvent.GetCommand()), true );
         }
         else
         {
            sendJogEvent( AppEngineQMLConstants::JOG_WHEEL_LEFT,
                          abs((int)aEvent.GetCommand()), true );
         }
         break;
      }
/** ----- end Jog events ----- */

      default:
         result = false;
         break;
   }

   GetLogger().Log( "<--- AppEngineQML::HandleEventEngine()" );
   return result;
}


void
AppEngineQML::CancelKey()
{
	int nRepeat = 1;
    GetLogger().Log("AppEngineQML::CancelKey()");
    if( d_ptr->m_nKeyPrev == AppEngineQMLConstants::JOG_CENTER &&
        (d_ptr->m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_PRESSED ||
         d_ptr->m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED ||
         d_ptr->m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_CRITICAL_PRESSED) ) {
        if( GetIsDisplayedPopup(DISPLAY_FRONT_REAR) )
           emit signalPopupJogNavigation(d_ptr->m_nKeyPrev, AppEngineQMLConstants::KEY_STATUS_CANCELED, d_ptr->m_bRRCPrev);
        else
           emit signalJogNavigation(d_ptr->m_nKeyPrev, AppEngineQMLConstants::KEY_STATUS_CANCELED, d_ptr->m_bRRCPrev, nRepeat );
        GetLogger().Log( QString("[KEY_STATE] Cancel key pressed [%1]").arg(d_ptr->m_nKeyPrev) );
        d_ptr->m_nKeyPrev = AppEngineQMLConstants::JOG_NONE;
        d_ptr->m_nKeyStatePrev = AppEngineQMLConstants::KEY_STATUS_NONE;
    }
    else if( (d_ptr->m_nKeyPrev >= AppEngineQMLConstants::JOG_UP &&
              d_ptr->m_nKeyPrev <= AppEngineQMLConstants::JOG_BOTTOM_LEFT) &&
             (d_ptr->m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_PRESSED ||
              d_ptr->m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED ||
              d_ptr->m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_CRITICAL_PRESSED) ) {
        if( GetIsDisplayedPopup(DISPLAY_FRONT_REAR) )
           emit signalPopupJogNavigation(d_ptr->m_nKeyPrev, AppEngineQMLConstants::KEY_STATUS_CANCELED, d_ptr->m_bRRCPrev);
        else
           emit signalJogNavigation(d_ptr->m_nKeyPrev, AppEngineQMLConstants::KEY_STATUS_CANCELED, d_ptr->m_bRRCPrev, nRepeat );
        GetLogger().Log( QString("[KEY_STATE] Cancel key pressed [%1]").arg(d_ptr->m_nKeyPrev) );
        d_ptr->m_nKeyPrev = AppEngineQMLConstants::JOG_NONE;
        d_ptr->m_nKeyStatePrev = AppEngineQMLConstants::KEY_STATUS_NONE;
    }
}



bool
AppEngineQML::HandleEvent( Event &aEvent )
{ 
   GetLogger().Log( "---> AppEngineQML::HandleEvent()" );
   switch ( aEvent.GetEventId() )
   {
      case EVT_REQUEST_FG:
      {
         GetLogger().Log( QString("AppEngineQML::HandleEvent:(EVT_REQUEST_FG) (fg=%1, visible=%2)").
            arg((int)m_AppHandlesResponseFg).arg((int)m_bAutoVisibility), Logger::PRIORITY_LOW);

         int prevAppId = GetFrontApp();
         GetLogger().Log( QString("afwfb : Previous AppId = %1").arg(prevAppId));

         SetFrontApp(GetThisAppId());
         SetFrontGuiApp(GetThisAppId());
         APP_ENTRY_POINT_DATA_T* entryData = (APP_ENTRY_POINT_DATA_T *)(aEvent.Data().data());
         d_ptr->m_bWillBeClone = false;
         d_ptr->m_bWillBeCloneOff = false;
         d_ptr->m_bDoSwap = false;
         if( GetCloneMode() == CLONE_OFF && entryData->sHeader.bReserved02 )
            d_ptr->m_bWillBeClone = true;
         else if( GetCloneMode() != CLONE_OFF && entryData->sHeader.bReserved05 )
            d_ptr->m_bDoSwap = true;
         else if( GetCloneMode() != CLONE_OFF && !entryData->sHeader.bReserved02 )
            d_ptr->m_bWillBeCloneOff = true;
         int entry = (int)(entryData->sAppEntryPoint);
         SetFrontEntry(entry);
         d_ptr->m_bFBTransitionOnResponse = false;

		if ( entryData->sHeader.bReserved03 )
		{
			SetRearGuiApp(GetFrontApp());
			SetRearEntry(GetFrontEntry());
		}
		if ( entryData->sHeader.bReserved04 )
		{
			SetFrontGuiApp(GetRearApp());
			SetFrontEntry(GetRearEntry());
		}

          GetLogger().Log( QString("afwfb fg :  s=%1 c=%2 e=%3 ec=%4 efc=%5 erc=%6 es=%7 ecoff=%8").
            arg(IsSwapDCEnabled()).arg(GetCloneMode()).arg(entry).
            arg(entryData->sHeader.bReserved02).arg(entryData->sHeader.bReserved03).
            arg(entryData->sHeader.bReserved04).arg(entryData->sHeader.bReserved05).
            arg(entryData->sHeader.bReserved06), Logger::PRIORITY_LOW);
         
         if( !IsSwapDCEnabled() ) {
            d_ptr->m_bFrontOnFront = true;
            SendTouchCleanUp();
         }

#ifdef REMOVE_AUTOVISIBILITY
		 if ( true )
#else
         if( true == m_bAutoVisibility )
#endif
         {
            int nSkipOffset = 0;

            if( GetThisAppId() == APP_MOBILETV || GetThisAppId() == APP_IDLE || GetThisAppId() == APP_AVP || GetThisAppId() == APP_NAVIGATION || GetThisAppId() == APP_CAMERA || GetThisAppId() == APP_E_MANUAL )
            {
               nSkipOffset = DH_STATUS_BAR;
            }
            if( GetAppView() != NULL )
            GetAppView()->setGeometry(DH_X_FRONT, DH_Y_FRONT - nSkipOffset, DH_SCREEN_WIDTH, DH_SCREEN_HEIGHT + nSkipOffset);
            m_DisplayType = DISPLAY_FRONT;
            
            if ( GetAppView() != NULL )
            {
                GetLogger().Log( "AppEngineBase::HandleEvent():Processing Event::show" );
				GetAppView()->repaint();
                GetAppView()->show();
#ifndef WINDOW_MANAGER_RAISE
#ifdef RAISE_WINDOW
#else
				GetAppView()->lower();
#endif
#endif
#ifdef RAISE_WINDOW
                if( IsDRS() && d_ptr->m_bWillBeClone ) {
                    GetAppView()->raise();
                    if( NULL != d_ptr->m_pFGResponseTimer )
                        ((QTimer*)d_ptr->m_pFGResponseTimer)->setInterval(FG_QML_EXTEND_TIMEOUT);
                }
                else {
                    if( NULL != d_ptr->m_pFGResponseTimer )
                        ((QTimer*)d_ptr->m_pFGResponseTimer)->setInterval(FG_QML_TIMEOUT); 
                }
#else
				if( NULL != d_ptr->m_pFGResponseTimer )
					((QTimer*)d_ptr->m_pFGResponseTimer)->setInterval(FG_QML_TIMEOUT); 
#endif
            }		
         }
#ifdef RAISE_WINDOW
         // exception for asembled rendering between navigation & video
         if( GetThisAppId() == APP_NAVIGATION && prevAppId != APP_CAMERA && IsVideoApp(prevAppId) ) {
            GetAppView()->raise();
            DoFrameBufferTransition(DISPLAY_FRONT);
         }
#endif

#ifdef REMOVE_APPTOPWIDGET			
         d_ptr->m_nDisplay = DISPLAY_FRONT;
         if ( NULL != d_ptr->m_pFGResponseTimer && m_AppHandlesResponseFg == false )
         {            
            ((QTimer*)d_ptr->m_pFGResponseTimer)->stop();
            ((QTimer*)d_ptr->m_pFGResponseTimer)->start();
         }
#endif
         break;
      }

      case EVT_REQUEST_BG:
      {
           GetLogger().Log( "AppEngineQML::HandleEvent(EVT_REQUEST_BG)", Logger::PRIORITY_LOW );
           CancelKey();
           CancelHwKey(DISPLAY_FRONT, true);
           if ( GetAppView() != NULL )
           {
               GetLogger().Log( "AppEngineBase::HandleEvent:Processing Event::hide" );
               if( !IsSwapDCEnabled() ) {
                   d_ptr->m_bFrontOnFront = false;
               }
               GetAppView()->hide();
           }
#ifdef REMOVE_APPTOPWIDGET
           if( NULL != d_ptr->m_pFGResponseTimer )
           {
                ((QTimer*)d_ptr->m_pFGResponseTimer)->stop();
           }
#endif
         break;
      }

        case EVT_CLONE_NOTIFICATION:
        {
            QDateTime	dateTime = QDateTime::currentDateTime();
            int cloneMode = (int)(((EVENT_CLONE_NOTIFICATION_DATA_T *)(aEvent.Data().data()))->sCloneStatus);
            GetLogger().Log(QString("TouchCleanUp front [%1][%2][%3][%4]").arg(cloneMode).arg(IsSwapDCEnabled()).arg(d_ptr->m_bFrontOnFront).arg(d_ptr->m_bFrontOnRear), Logger::PRIORITY_LOW);
            if (GetThisAppId() == GetFrontApp() || GetThisAppId() == GetRearApp())
                GetLogger().Log(QString("afwfb : clone notification [%1] at %2").arg(cloneMode).arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz")), Logger::PRIORITY_LOW);
//            if( (cloneMode == CLONE_ON_FRONT && d_ptr->m_bFrontOnRear) )
//                SendTouchCleanUp();
//            if( (cloneMode == CLONE_ON_REAR && d_ptr->m_bFrontOnFront) )
//                SendTouchCleanUp();
            if(cloneMode == (int)CLONE_ON_FRONT && GetFrontApp() == GetThisAppId())
            {
                SetRearGuiApp(GetFrontApp());
                SetRearEntry(GetFrontEntry());
            }
            else if(cloneMode == (int)CLONE_ON_REAR && GetRearApp() == GetThisAppId())
            {
                SetFrontGuiApp(GetRearApp());
                SetFrontEntry(GetRearEntry());
            }

            if( cloneMode == CLONE_OFF ) {
                SendTouchCleanUp();
                if( d_ptr->m_bWillBeCloneOff )
                    d_ptr->m_bWillBeCloneOff = false;
                if (GetThisAppId() == GetFrontApp() || GetThisAppId() == GetRearApp())
                    GetLogger().Log(QString("afwfb CLONE_OFF at %1").arg(dateTime.toString("hh:mm:ss.zzz")), Logger::PRIORITY_LOW);
                if( GetAppView()->isVisible() ) {
                    CancelKey();
                    CancelHwKey(m_DisplayType);
                }
            }
            else {
                SendTouchCleanUp();
                if( d_ptr->m_bWillBeClone ) {
                    d_ptr->m_bWillBeClone = false;
                    GetLogger().Log(QString("afwfb CLONE_ON at %1)").arg(dateTime.toString("hh:mm:ss.zzz")), Logger::PRIORITY_LOW);
                }
                else {
                    if( cloneMode == CLONE_ON_FRONT && GetThisAppId() == GetFrontApp() )
                        DoFrontFBTransition(FB_TR_DO_FRONT_CLONE);
                    if( cloneMode == CLONE_ON_REAR && GetThisAppId() == GetRearApp() )
                        DoRearFBTransition(FB_TR_DO_REAR_CLONE);
                }
                if( GetAppView()->isVisible() ) {
                    CancelKey();
                    CancelHwKey(m_DisplayType);
                }
            }
            break;
        }

        case EVT_SWAP_DC_ENABLED:
        {
            //if( GetThisAppId() == GetFrontApp() )
            //    DoFrontFBTransition(FB_TR_DO_SWAP_DC);
            //else if( GetThisAppId() == GetRearApp() )        // OK?
            //    DoRearFBTransition(FB_TR_DO_SWAP_DC);
            break;
        }

        case EVT_SHOW_POPUP:
        {
            UpdateStatusbarOnPopupEvent(aEvent, GetFrontFullScreen());
            break;
        }
		case EVT_POWER_STATE_NORMAL_OFF:
	    {
	   		SendTouchCleanUp();
			break;
	    }
		case EVT_POWER_STATE_LOGIC_OFF:
	    {
	   		SendTouchCleanUp();
			break;
	    }
        
      default:
         break;
   }

   GetLogger().Log( "<--- AppEngineQML::HandleEvent()" );
   return HandleEventEngine( aEvent );
}

bool
AppEngineQML::HandleEventRear( Event &aEvent )
{
   GetLogger().Log( "---> AppEngineQML::HandleEventRear()" );
   switch ( aEvent.GetEventId() )
   {
      case EVT_REQUEST_FG:
      {
//         if( GetThisAppId() == APP_STATUS_BAR )
//         {
//             break;
//         } 
         GetLogger().Log( QString("AppEngineQML::HandleEventRear(EVT_REQUEST_FG) (fg=%1, visible=%2)").
            arg((int)m_AppHandlesResponseFg).arg((int)m_bAutoVisibility), Logger::PRIORITY_LOW);

         int prevAppId = GetRearApp();
         GetLogger().Log( QString("afwfb : Previous AppId = %1").arg(prevAppId));

         SetRearApp(GetThisAppId());
         SetRearGuiApp(GetThisAppId());
         APP_ENTRY_POINT_DATA_T* entryData = (APP_ENTRY_POINT_DATA_T *)(aEvent.Data().data());
         d_ptr->m_bWillBeClone = false;
         d_ptr->m_bWillBeCloneOff = false;
         d_ptr->m_bDoSwap = false;
         if( GetCloneMode() == CLONE_OFF && entryData->sHeader.bReserved02 )
            d_ptr->m_bWillBeClone = true;
         else if( GetCloneMode() != CLONE_OFF && entryData->sHeader.bReserved05 )
            d_ptr->m_bDoSwap = true;
         else if( GetCloneMode() != CLONE_OFF && !entryData->sHeader.bReserved02 )
            d_ptr->m_bWillBeCloneOff = true;
         int entry = (int)(entryData->sAppEntryPoint);
         SetRearEntry(entry);
         d_ptr->m_bFBTransitionOnResponse = false;

		if ( entryData->sHeader.bReserved03 )
		{
			SetRearGuiApp(GetFrontApp());
			SetRearEntry(GetFrontEntry());
		}
		if ( entryData->sHeader.bReserved04 )
		{
			SetFrontGuiApp(GetRearApp());
			SetFrontEntry(GetRearEntry());
		}
         
          GetLogger().Log( QString("afwfb fg :  s=%1 c=%2 e=%3 ec=%4 efc=%5 erc=%6 es=%7 ecoff=%8").
            arg(IsSwapDCEnabled()).arg(GetCloneMode()).arg(entry).
            arg(entryData->sHeader.bReserved02).arg(entryData->sHeader.bReserved03).
            arg(entryData->sHeader.bReserved04).arg(entryData->sHeader.bReserved05).
            arg(entryData->sHeader.bReserved06), Logger::PRIORITY_LOW);
         
         if( IsSwapDCEnabled() ) {
            d_ptr->m_bFrontOnRear = true;
            SendTouchCleanUp();
         }
         else if( GetCloneMode() == CLONE_ON_REAR )
            SendTouchCleanUp();

         if( true == m_bAutoVisibility )
         {
            int nSkipOffset = 0;

            if( GetThisAppId() == APP_MOBILETV || GetThisAppId() == APP_IDLE || GetThisAppId() == APP_AVP || GetThisAppId() == APP_NAVIGATION || GetThisAppId() == APP_CAMERA || GetThisAppId() == APP_E_MANUAL )
            {
               nSkipOffset = DH_STATUS_BAR;
            }
            if( GetAppView() != NULL )
            GetAppView()->setGeometry(DH_X_REAR, DH_Y_REAR - nSkipOffset , DH_SCREEN_WIDTH, DH_SCREEN_HEIGHT + nSkipOffset);
            m_DisplayType = DISPLAY_REAR;

            if ( GetAppView() != NULL )
            {
                GetLogger().Log( "AppEngineBase::HandleEventRear():Processing Event::show" );
				GetAppView()->repaint();
                GetAppView()->show();
#ifndef WINDOW_MANAGER_RAISE
#ifdef RAISE_WINDOW
#else
				GetAppView()->lower();
#endif				
#endif
#ifdef RAISE_WINDOW
                if( IsDRS() && d_ptr->m_bWillBeClone ) {
                    GetAppView()->raise();
                    if( NULL != d_ptr->m_pFGResponseTimer )
                        ((QTimer*)d_ptr->m_pFGResponseTimer)->setInterval(FG_QML_EXTEND_TIMEOUT);
                }
                else {
                    if( NULL != d_ptr->m_pFGResponseTimer )
                        ((QTimer*)d_ptr->m_pFGResponseTimer)->setInterval(FG_QML_TIMEOUT); 
                }
#else
				if( NULL != d_ptr->m_pFGResponseTimer )
					((QTimer*)d_ptr->m_pFGResponseTimer)->setInterval(FG_QML_TIMEOUT); 
#endif
            }
         }

#ifdef RAISE_WINDOW
        // exception for asembled rendering between navigation & video
        if( GetThisAppId() == APP_NAVIGATION && prevAppId != APP_CAMERA && IsVideoApp(prevAppId) ) {
            GetAppView()->raise();
            DoFrameBufferTransition(DISPLAY_REAR);
         }
#endif

#ifdef REMOVE_APPTOPWIDGET
        d_ptr->m_nDisplay = DISPLAY_REAR;
         if ( NULL != d_ptr->m_pFGResponseTimer && m_AppHandlesResponseFg == false )
         {
             ((QTimer*)d_ptr->m_pFGResponseTimer)->stop();
             ((QTimer*)d_ptr->m_pFGResponseTimer)->start();
         }
#endif
         break;
      }

      case EVT_REQUEST_BG:
      {
           GetLogger().Log( "AppEngineQML::HandleEventRear(EVT_REQUEST_BG)", Logger::PRIORITY_LOW );
           CancelKey();
           CancelHwKey(DISPLAY_REAR);
           if ( GetAppView() != NULL  )
           {
               GetLogger().Log( "AppEngineBase::HandleEventRear:Processing Event::hide" );
               if( IsSwapDCEnabled() ) {
                   d_ptr->m_bFrontOnRear = false;
               }
               GetAppView()->hide();
           }
#ifdef REMOVE_APPTOPWIDGET
           if( NULL != d_ptr->m_pFGResponseTimer )
           {
                ((QTimer*)d_ptr->m_pFGResponseTimer)->stop();
           }
#endif
         break;
      }
      
      case EVT_SHOW_POPUP:
      {
          UpdateStatusbarOnPopupEvent(aEvent, GetRearFullScreen());
          break;
      }
	  case EVT_POWER_STATE_NORMAL_OFF:
	  {
	   		SendTouchCleanUp();
			break;
	  }
	  case EVT_POWER_STATE_LOGIC_OFF:
	  {
	   		SendTouchCleanUp();
			break;
	  }
        
      default:
         break;
   }

   GetLogger().Log( "<--- AppEngineQML::HandleEventRear()" );
   return HandleEventEngine( aEvent );
}

void AppEngineQML::handlePopupResponse( int target, QVariant response )
{
    if(d_ptr->m_bPopupCloseIgnore == false)
        hidePopup( true );
    POPUP_RESPONSE_T message;
    memset(&message, NULL, sizeof(POPUP_RESPONSE_T));
    message.type = ((POPUP_RESPONSE_T* )response.toByteArray().data())->type;
    message.btn = ((POPUP_RESPONSE_T* )response.toByteArray().data())->btn;
    message.index = ((POPUP_RESPONSE_T* )response.toByteArray().data())->index;
    message.value = ((POPUP_RESPONSE_T* )response.toByteArray().data())->value;
    message.popup_type = d_ptr->m_Popup.popup_type;
    message.responsesource = d_ptr->m_Popup.responsesource;
    GetLogger().Log(QString( "AppEngineQML::handlePopupResponse(), target : [%1] [%2] [%3]" ).arg( target ).arg(message.popup_type).arg(message.responsesource));
    Event event( EVT_POPUP_RESPONSE, GetThisAppId(), APP_UI_MANAGER, QByteArray((char *)&message,sizeof(message)) );
    event.SetCommand(false);
   if(d_ptr->m_nDisplay == DISPLAY_REAR )
       NotifyUISH( event, DISPLAY_REAR );
   else
       NotifyUISH( event, DISPLAY_FRONT );
}

void AppEngineQML::handleDumyPopupResponse()
{
   GetLogger().Log(QString( "AppEngineQML::handleDumyPopupResponse() " ));
    POPUP_RESPONSE_T message;
    memset(&message, NULL, sizeof(POPUP_RESPONSE_T));
    message.btn = POPUP_BUTTON_NONE;
    message.popup_type = d_ptr->m_Popup.popup_type;
    message.responsesource = d_ptr->m_Popup.responsesource;

   Event event( EVT_POPUP_RESPONSE, GetThisAppId(), APP_UI_MANAGER, true, QByteArray((char *)&message,sizeof(message)) );
   event.SetCommand(true);
   if(d_ptr->m_nDisplay == DISPLAY_REAR )
       NotifyUISH( event, DISPLAY_REAR );
   else
       NotifyUISH( event, DISPLAY_FRONT );
}


#ifdef REMOVE_APPTOPWIDGET
void
AppEngineQML::SlotCallFgResponse()
{
    GetLogger().Log( "----> AppEngineQML::SlotCallFgResponse()" );
    if( NULL != d_ptr->m_pFGResponseTimer )
    {
        ((QTimer*)d_ptr->m_pFGResponseTimer)->stop();
    }
    HandleAutoVisibility(d_ptr->m_nDisplay);  
    GetLogger().Log( "----> AppEngineQML::SlotCallFgResponse() send" );
    SendFGResponse(d_ptr->m_nDisplay);
}
#endif

void 
AppEngineQML::Reserved_1()
{
}

void 
AppEngineQML::Reserved_2()
{
}

void
AppEngineQML::ManualBeep()
{
    PlayBeep();
}

void AppEngineQML::HandleAutoVisibility(uint nScreen)
{
#ifdef RAISE_WINDOW
    if ( m_bAutoVisibility && GetAppView() != NULL )
    {
        GetLogger().Log( QString("AppEngineQML::HandleAutoVisibility(%1) show").arg(nScreen)  );\
		GetAppView()->repaint();
        GetAppView()->show();
#ifndef WINDOW_MANAGER_RAISE
#ifdef RAISE_WINDOW
#else
		GetAppView()->lower();
#endif		
#endif
    }    
#endif

    d_ptr->m_bFBTransitionOnResponse = true;
    d_ptr->m_nDisplay4LateTransition = nScreen;
    int time = 100;
	if( GetThisAppId() == APP_CAMERA && nScreen == DISPLAY_REAR && GetFrontApp() == APP_IBOX)
		time = 100;

    if( GetThisAppId() == APP_VR || GetThisAppId() == APP_IDLE ) // exception in case of APP_VR, can't find slot DoTimedTransition()... WHY?
        DoTimedTransition();
    else {
		QTimer::singleShot(time, this, SLOT(DoTimedTransition()));
   }        
}

void AppEngineQML::DoTimedTransition() 
{
    DoFrameBufferTransition(d_ptr->m_nDisplay4LateTransition);    
}

void AppEngineQML::DoFrameBufferTransition(uint nscreen)
{
    if( d_ptr->m_bDoSwap && !d_ptr->m_bFBTransitionOnResponse )
        return;
    
    GetLogger().Log(QString("DoFrameBufferTransition() afwfb con=%1 coff=%2 s=%3").
        arg(d_ptr->m_bWillBeClone).arg(d_ptr->m_bWillBeCloneOff).arg(d_ptr->m_bDoSwap));

    if( nscreen == DISPLAY_FRONT ) {
        if( d_ptr->m_bWillBeClone )
            DoFrontFBTransition(FB_TR_CLONED);
        else if( d_ptr->m_bDoSwap ) {
            DoFrontFBTransition(FB_TR_DO_SWAP_DC);
            d_ptr->m_bDoSwap = false;
        }
        else if( d_ptr->m_bWillBeCloneOff )
            DoFrontFBTransition(FB_TR_NORMAL);
        else {
            if( GetCloneMode() == CLONE_OFF )
                DoFrontFBTransition(FB_TR_NORMAL);
            else 
                DoFrontFBTransition(FB_TR_CLONED);
        }
    }
    else {
        if( d_ptr->m_bWillBeClone )
            DoRearFBTransition(FB_TR_CLONED);
        else if( d_ptr->m_bDoSwap ) {
            DoRearFBTransition(FB_TR_DO_SWAP_DC);
            d_ptr->m_bDoSwap = false;
        }
        else if( d_ptr->m_bWillBeCloneOff )
            DoRearFBTransition(FB_TR_NORMAL);
        else {
            if( GetCloneMode() == CLONE_OFF )
                DoRearFBTransition(FB_TR_NORMAL);
            else
                DoRearFBTransition(FB_TR_CLONED);
        }
    }    
    d_ptr->m_bFBTransitionOnResponse = false;
}

