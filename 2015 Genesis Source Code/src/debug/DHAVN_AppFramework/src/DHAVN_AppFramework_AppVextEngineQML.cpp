#include "DHAVN_AppFramework_AppVextEngineQML_Def.h"
#include "DHAVN_AppFramework_AppMainWidget.h"
#include "DHAVN_AppFramework_Popup.h"
#include <QDeclarativeItem>
#include <QTimer>

//#define FG_TIMEOUT 300
#define FG_TIMEOUT 50

class AppVextEngineQMLPrivate
{
private:
	AppVextEngineQMLPrivate(AppVextEngineQML* pParent, bool bEnableOpenGl, bool bEnableHandleTouch)
	: m_pFGFrontResponseTimer(NULL), m_pFGRearResponseTimer(NULL),
      m_bFrontOnFront(false), m_bFrontOnRear(false),
      m_pMainWindow(NULL),
      m_bFrontWillBeClone(false), m_bFrontWillBeCloneOff(false),
      m_bRearWillBeClone(false), m_bRearWillBeCloneOff(false),
      m_bDoSwap(false), m_bFBTransitionOnResponse(false), m_frontRenderAppInBG(APP_NONE), m_rearRenderAppInBG(APP_NONE),
      m_frontRenderEntryInBG(eAPP_NONE), m_rearRenderEntryInBG(eAPP_NONE),
      m_bLateFBTransition(false), m_nDisplay4LateTransition(0),
      m_bPopupCloseIgnore(false),
      q_ptr(pParent)
    {
        m_pMainWindow = new AppMainWidget( bEnableOpenGl, bEnableHandleTouch );
	  m_bEnableHandleTouch = bEnableHandleTouch;
        m_pFGFrontResponseTimer = new QTimer;
        m_pFGRearResponseTimer = new QTimer;
    }

    AppVextEngineQMLPrivate(AppVextEngineQML* pParent, bool bEnableOpenGl)
    : m_pFGFrontResponseTimer(NULL), m_pFGRearResponseTimer(NULL),
      m_bFrontOnFront(false), m_bFrontOnRear(false),
      m_pMainWindow(NULL),
      m_bFrontWillBeClone(false), m_bFrontWillBeCloneOff(false),
      m_bRearWillBeClone(false), m_bRearWillBeCloneOff(false),
      m_bDoSwap(false), m_bFBTransitionOnResponse(false), m_frontRenderAppInBG(APP_NONE), m_rearRenderAppInBG(APP_NONE),
      m_frontRenderEntryInBG(eAPP_NONE), m_rearRenderEntryInBG(eAPP_NONE),
      m_bLateFBTransition(false), m_nDisplay4LateTransition(0),
      m_bPopupCloseIgnore(false),
      q_ptr(pParent)
    {
		m_pMainWindow = new AppMainWidget( bEnableOpenGl, false );
        m_pFGFrontResponseTimer = new QTimer;
        m_pFGRearResponseTimer = new QTimer;
        memset(&m_FrontPopup, 0, sizeof(POPUP_SHOW_T));
        m_FrontPopup.popup_type = _CURRENT_POPUP_NONE;
        memset(&m_RearPopup, 0, sizeof(POPUP_SHOW_T));
        m_RearPopup.popup_type = _CURRENT_POPUP_NONE;
    }

    ~AppVextEngineQMLPrivate() 
    {
        delete  m_pMainWindow;
    }

public:
    QObject* m_pFGFrontResponseTimer;
    QObject* m_pFGRearResponseTimer;
    bool m_bFrontOnFront;
    bool m_bFrontOnRear;
    QObject* m_pMainWindow;
    bool m_bFrontWillBeClone;
    bool m_bFrontWillBeCloneOff;
    bool m_bRearWillBeClone;
    bool m_bRearWillBeCloneOff;
    bool m_bDoSwap;
    bool m_bFBTransitionOnResponse;
    int m_frontRenderAppInBG;
    int m_rearRenderAppInBG;
    int m_frontRenderEntryInBG;
    int m_rearRenderEntryInBG;
    bool m_bLateFBTransition;
    int m_nDisplay4LateTransition;
    bool m_bPopupCloseIgnore;
	bool m_bEnableHandleTouch;

    POPUP_SHOW_T m_FrontPopup;
    POPUP_SHOW_T m_RearPopup;

    AppVextEngineQML * const q_ptr;
    Q_DECLARE_PUBLIC(AppVextEngineQML)
};

AppVextEngineQML::AppVextEngineQML( bool bEnableOpenGl, bool bEnableHandleTouch, QObject *pParent )
: AppEngineQML( NULL, pParent, APP_QML_VEXT )
{
		d_ptr = new AppVextEngineQMLPrivate(this, bEnableOpenGl, bEnableHandleTouch);
	  ((QTimer*)d_ptr->m_pFGFrontResponseTimer)->setInterval(FG_TIMEOUT);
	  connect((QTimer*)d_ptr->m_pFGFrontResponseTimer, SIGNAL(timeout()),this, SLOT(SlotCallFgFrontResponce()));
	
	  ((QTimer*)d_ptr->m_pFGRearResponseTimer)->setInterval(FG_TIMEOUT);
	  connect((QTimer*)d_ptr->m_pFGRearResponseTimer, SIGNAL(timeout()),this, SLOT(SlotCallFgRearResponce()));
	
	  GetLogger().Log( QString("AppVextEngineQML::AppVextEngineQML create "), Logger::PRIORITY_HI );
	}


AppVextEngineQML::AppVextEngineQML( bool bEnableOpenGl, QObject *pParent )
: AppEngineQML( NULL, pParent, APP_QML_VEXT )
{
  d_ptr = new AppVextEngineQMLPrivate(this, bEnableOpenGl);
  ((QTimer*)d_ptr->m_pFGFrontResponseTimer)->setInterval(FG_TIMEOUT);
  connect((QTimer*)d_ptr->m_pFGFrontResponseTimer, SIGNAL(timeout()),this, SLOT(SlotCallFgFrontResponce()));

  ((QTimer*)d_ptr->m_pFGRearResponseTimer)->setInterval(FG_TIMEOUT);
  connect((QTimer*)d_ptr->m_pFGRearResponseTimer, SIGNAL(timeout()),this, SLOT(SlotCallFgRearResponce()));

  GetLogger().Log( QString("AppVextEngineQML::AppVextEngineQML create "), Logger::PRIORITY_LOW );
}

AppVextEngineQML::AppVextEngineQML( bool bEnableOpenGl, QString aModuleName, bool bEnableHandleTouch, QObject *pParent )
: AppEngineQML( NULL, aModuleName, pParent, APP_QML_VEXT )
{
		d_ptr = new AppVextEngineQMLPrivate(this, bEnableOpenGl, bEnableHandleTouch);	
	   ((QTimer*)d_ptr->m_pFGFrontResponseTimer)->setInterval(FG_TIMEOUT);
	   connect((QTimer*)d_ptr->m_pFGFrontResponseTimer, SIGNAL(timeout()),this, SLOT(SlotCallFgFrontResponce()));
	
	   ((QTimer*)d_ptr->m_pFGRearResponseTimer)->setInterval(FG_TIMEOUT);
	   connect((QTimer*)d_ptr->m_pFGRearResponseTimer, SIGNAL(timeout()),this, SLOT(SlotCallFgRearResponce()));
	   GetLogger().Log( QString("AppVextEngineQML::AppVextEngineQML create "), Logger::PRIORITY_HI );
	}


AppVextEngineQML::AppVextEngineQML( bool bEnableOpenGl, QString aModuleName, QObject *pParent )

: AppEngineQML( NULL, aModuleName, pParent, APP_QML_VEXT )
{
   d_ptr = new AppVextEngineQMLPrivate(this, bEnableOpenGl);

   ((QTimer*)d_ptr->m_pFGFrontResponseTimer)->setInterval(FG_TIMEOUT);
   connect((QTimer*)d_ptr->m_pFGFrontResponseTimer, SIGNAL(timeout()),this, SLOT(SlotCallFgFrontResponce()));

   ((QTimer*)d_ptr->m_pFGRearResponseTimer)->setInterval(FG_TIMEOUT);
   connect((QTimer*)d_ptr->m_pFGRearResponseTimer, SIGNAL(timeout()),this, SLOT(SlotCallFgRearResponce()));
   GetLogger().Log( QString("AppVextEngineQML::AppVextEngineQML create "), Logger::PRIORITY_LOW );
}


AppVextEngineQML::~AppVextEngineQML()
{
   delete d_ptr;
}

void 
AppVextEngineQML::Reserved_1()
{
}

void 
AppVextEngineQML::Reserved_2()
{
}

bool
AppVextEngineQML::GetSceensUsage(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreen)
{
   if( NULL !=  d_ptr->m_pMainWindow )
   {
      return ((AppMainWidget*)d_ptr->m_pMainWindow)->GetSceensUsage(nScreen);
   }
   return false;
}

QDeclarativeContext *
AppVextEngineQML::GetScreenContext(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes )
{
   QDeclarativeContext* pRet = NULL;

   if( NULL !=  d_ptr->m_pMainWindow )
   {
      GetLogger().Log("AppVextEngineQML::GetScreenContext");
      pRet = ((AppMainWidget*)d_ptr->m_pMainWindow)->GetScreenContext(nScreenTypes);
   }
   return pRet;
}

/** visibility block */
void
AppVextEngineQML::show()
{
   if( NULL !=  d_ptr->m_pMainWindow )
   {
      ((AppMainWidget*)d_ptr->m_pMainWindow)->show();
   }
}

void
AppVextEngineQML::hide()
{
   if( NULL != d_ptr->m_pMainWindow &&
       ((AppMainWidget*)d_ptr->m_pMainWindow)->GetSceensUsage(AppEngineQMLConstants::APP_ALL_HIDE) == true )
   {
      ((AppMainWidget*)d_ptr->m_pMainWindow)->hide();
   }
}

void AppVextEngineQML::SetVisibleScreens(int nState, bool bVisible )
{
   if( NULL !=  d_ptr->m_pMainWindow )
   {
       ((AppMainWidget*)d_ptr->m_pMainWindow)->HandleRequestEvents( (AppEngineQMLConstants::EAPP_SCREEN_USAGE_T) nState, bVisible );
   }
}

/** init qml trees */
void AppVextEngineQML::SetSource( AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes, QString sFrontScreen )
{
   if( NULL !=  d_ptr->m_pMainWindow )
   {
      GetLogger().Log( "---> AppVextEngineQML::SetSource same" );

      int nOffset = 0;
      if( GetThisAppId() == APP_MOBILETV || GetThisAppId() == APP_STANDBY || GetThisAppId() == APP_IDLE || GetThisAppId() == APP_AVP || GetThisAppId() == APP_E_MANUAL  )
      {
         nOffset = -DH_STATUS_BAR;
      }
      ((AppMainWidget*)d_ptr->m_pMainWindow)->SetOffset(nOffset);
      ((AppMainWidget*)d_ptr->m_pMainWindow)->SetSource(nScreenTypes, sFrontScreen, "" );
      qmlRegisterType<AppEngineQMLConstants>("AppEngineQMLConstants", 1, 0, "UIListenerEnum");
      MakeConnections();
      GetLogger().Log( "<--- AppVextEngineQML::SetSource" );
   }
}

void AppVextEngineQML::SetSource( QString sFrontScreen, QString sRearScreen )
{
   if( NULL !=  d_ptr->m_pMainWindow )
   {
      GetLogger().Log( "---> AppVextEngineQML::SetSource dif" );

      int nOffset = 0;
      if( GetThisAppId() == APP_MOBILETV || GetThisAppId() == APP_STANDBY || GetThisAppId() == APP_IDLE || GetThisAppId() == APP_AVP || GetThisAppId() == APP_E_MANUAL  )
      {
         nOffset = -DH_STATUS_BAR;
      }
      ((AppMainWidget*)d_ptr->m_pMainWindow)->SetOffset(nOffset);
      ((AppMainWidget*)d_ptr->m_pMainWindow)->SetSource( AppEngineQMLConstants::APP_DOUBLE_SCREEN, sFrontScreen, sRearScreen );
      qmlRegisterType<AppEngineQMLConstants>("AppEngineQMLConstants", 1, 0, "UIListenerEnum");
      MakeConnections();
      GetLogger().Log( "<--- AppVextEngineQML::SetSource" );
   }
}

void AppVextEngineQML::MakeConnections()
{
   AppScreenView* pFrontView = ((AppMainWidget*)d_ptr->m_pMainWindow)->GetFrontView();
   AppScreenView* pRearView = ((AppMainWidget*)d_ptr->m_pMainWindow)->GetRearView();

   if( pFrontView != NULL )
   {
      disconnect(pFrontView, SIGNAL( HandlePopupResponse( int, QVariant )), this, SLOT(HandlePopupResponseFront( int, QVariant )) );
      connect(pFrontView, SIGNAL( HandlePopupResponse( int, QVariant )), this, SLOT(HandlePopupResponseFront( int, QVariant )) );
      connect(pFrontView, SIGNAL(signalManualBeep()), this, SLOT(ManualBeep()), Qt::UniqueConnection);
      connect(pFrontView, SIGNAL(HandleTouchEvent(QEvent*)), this, SLOT(HandleTouchViewFrontPort(QEvent*)), Qt::UniqueConnection);
   }

   if( pRearView != NULL )
   {
      disconnect(pRearView, SIGNAL( HandlePopupResponse( int, QVariant )), this, SLOT(HandlePopupResponseRear( int, QVariant )) );
      connect(pRearView, SIGNAL( HandlePopupResponse( int, QVariant )), this, SLOT(HandlePopupResponseRear( int, QVariant )) );
      connect(pRearView, SIGNAL(signalManualBeep()), this, SLOT(ManualBeep()), Qt::UniqueConnection);
      connect(pRearView, SIGNAL(HandleTouchEvent(QEvent*)), this, SLOT(HandleTouchViewRearPort(QEvent*)), Qt::UniqueConnection);
   }
}

/** Functions to set views properties */
void
AppVextEngineQML::SetAttribute(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes, Qt::WidgetAttribute attribute, bool on)
{
   if( NULL !=  d_ptr->m_pMainWindow )
   {
      GetLogger().Log( "AppVextEngineQML::SetAttribute" );
      ((AppMainWidget*)d_ptr->m_pMainWindow)->SetAttribute(nScreenTypes, attribute, on);
   }
}

void
AppVextEngineQML::SetStyleSheet(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes, const QString & styleSheet )
{
   if( NULL !=  d_ptr->m_pMainWindow )
   {
      GetLogger().Log( "AppVextEngineQML::SetAttribute" );
      ((AppMainWidget*)d_ptr->m_pMainWindow)->SetStyleSheet(nScreenTypes,styleSheet);
   }
}

QDeclarativeView*
AppVextEngineQML::GetDeclarativeView(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes )
{
   QDeclarativeView *screen = NULL;
   if(NULL !=  d_ptr->m_pMainWindow)
   {
     if( nScreenTypes == AppEngineQMLConstants::APP_REAR_SCREEN )
     {
        screen = (QDeclarativeView*)((AppMainWidget*)d_ptr->m_pMainWindow)->GetRearView();
     }
     else
     {
        screen = (QDeclarativeView*)((AppMainWidget*)d_ptr->m_pMainWindow)->GetFrontView();
     }
   }
   return screen;
}

/** event handlers*/
bool
AppVextEngineQML::HandleEvent( Event &aEvent )
{
   bool result = true;

   GetLogger().Log( "---> AppVextEngineQML::HandleEvent()" );
   EVT_ID_T nEventID = aEvent.GetEventId();
   KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
   AppScreenView* pView = ((AppMainWidget*)d_ptr->m_pMainWindow)->GetFrontView();
   if( NULL == pView )
   {
      return false;
   }

   GetLogger().Log( "---> AppVextEngineQML::HandleEvent() event " );

   switch ( nEventID )
   {
      // This event could be supported on QML applications only, since POPUP plugin based on QML
      case EVT_HIDE_POPUP:
      {
         memset(&d_ptr->m_FrontPopup, 0, sizeof(POPUP_SHOW_T));
         d_ptr->m_FrontPopup.popup_type = _CURRENT_POPUP_NONE;
         SetIsDisplayedPopup( DISPLAY_FRONT, false );
         pView->HideSystemPopup( true );
         break;
      }

      case EVT_SHOW_POPUP:
      {
        POPUP_SHOW_T* popupData = (POPUP_SHOW_T *)(aEvent.Data().data());
        if(popupData->ignoreautoclose == true)
           d_ptr->m_bPopupCloseIgnore = true;
        else
           d_ptr->m_bPopupCloseIgnore = false;
         SetIsDisplayedPopup( DISPLAY_FRONT, true );
         UpdateStatusbarOnPopupEvent(aEvent, GetFrontFullScreen());
         CancelHwKey(DISPLAY_FRONT);
         GetLogger().Log( QString("showPopUpInContext %1 %2 %3").arg(d_ptr->m_FrontPopup.popup_type).arg(popupData->popup_type).arg(d_ptr->m_FrontPopup.responsesource) , Logger::PRIORITY_LOW );
         if((d_ptr->m_FrontPopup.popup_type != popupData->popup_type) && (d_ptr->m_FrontPopup.popup_type != _CURRENT_POPUP_NONE))
         {
             handleDumyPopupResponseFront();
             memcpy(&d_ptr->m_FrontPopup, popupData, sizeof(POPUP_SHOW_T));
         }
         else
         {
             memcpy(&d_ptr->m_FrontPopup, popupData, sizeof(POPUP_SHOW_T));
         }
         pView->ShowSystemPopup(aEvent, GetThisAppId());
         break;
      }

      case EVT_REQUEST_FG:
      {
         GetLogger().Log( QString("AppVextEngineQML::HandleEvent:(EVT_REQUEST_FG) (fg=%1, visible=%2)").
            arg((int)m_AppHandlesResponseFg).arg((int)m_bAutoVisibility), Logger::PRIORITY_LOW);

         memset(&d_ptr->m_FrontPopup, 0, sizeof(POPUP_SHOW_T));
         d_ptr->m_FrontPopup.popup_type = _CURRENT_POPUP_NONE;
         int prevAppId = GetFrontApp();
         int prevEntryId = GetFrontEntry();
         int bLateRaise = true;
		 int bLower  = false;
		 
         SetFrontApp(GetThisAppId());
         SetFrontGuiApp(GetThisAppId());
         APP_ENTRY_POINT_DATA_T* entryData = (APP_ENTRY_POINT_DATA_T *)(aEvent.Data().data());
         d_ptr->m_bFrontWillBeClone = false;
         d_ptr->m_bFrontWillBeCloneOff = false;
         d_ptr->m_bDoSwap = false;
         if( GetCloneMode() == CLONE_OFF && entryData->sHeader.bReserved02 )
            d_ptr->m_bFrontWillBeClone = true;
         else if( GetCloneMode() != CLONE_OFF && entryData->sHeader.bReserved05 )
            d_ptr->m_bDoSwap = true;
         else if( GetCloneMode() != CLONE_OFF && !entryData->sHeader.bReserved02 )
            d_ptr->m_bFrontWillBeCloneOff = true;
         int entry = (int)(entryData->sAppEntryPoint);
         SetFrontEntry(entry);
         d_ptr->m_bFBTransitionOnResponse= false;

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

         if( IsDisplaySetting(entry) ) {
            if( prevAppId != APP_AVP && prevAppId != APP_MOBILETV ) {
                int renderApp = d_ptr->m_frontRenderAppInBG; 
                if( d_ptr->m_frontRenderAppInBG == APP_NONE ) {
                    GetLogger().Log("afwfb : get otherside rendering app", Logger::PRIORITY_LOW);
                    renderApp = d_ptr->m_rearRenderAppInBG;
                }
                GetLogger().Log(QString("afwfb : send render ON to [%1]").arg(renderApp), Logger::PRIORITY_LOW); 
                Event evt(EVT_POST_EVENT, APP_SETTINGS, (APP_ID_T)(renderApp), EVT_REQUEST_RENDER_ON);
                NotifyUISH(evt, DISPLAY_FRONT);
            }
            else {
                d_ptr->m_frontRenderAppInBG = prevAppId;
                SetFrontRenderAppInBG(prevAppId);
                SetFrontRenderEntryInBG(prevEntryId);
            }
         }
         else {
            d_ptr->m_frontRenderAppInBG = APP_NONE;
            SetFrontRenderAppInBG(APP_NONE);
            SetFrontRenderEntryInBG(eAPP_NONE);
         }

          GetLogger().Log( QString("afwfb fg :  s=%1 c=%2 e=%3 ec=%4 efc=%5 erc=%6 es=%7 ecoff=%8").
            arg(IsSwapDCEnabled()).arg(GetCloneMode()).arg(entry).
            arg(entryData->sHeader.bReserved02).arg(entryData->sHeader.bReserved03).
            arg(entryData->sHeader.bReserved04).arg(entryData->sHeader.bReserved05).
            arg(entryData->sHeader.bReserved06), Logger::PRIORITY_LOW);
         
         pView->SendFG();
         if( !IsSwapDCEnabled() ) {
            d_ptr->m_bFrontOnFront = true;
            SendTouchCleanUp();
         }	 

         if( m_bAutoVisibility == true )
         {
#ifdef RAISE_WINDOW
             // exception for asembled rendering between navigation & video
             if( prevAppId == APP_NAVIGATION && IsVideoApp(GetThisAppId()) )
                bLateRaise = false;
			 else
#endif
			 	bLower = true;
			 
             ((AppMainWidget*)d_ptr->m_pMainWindow)->HandleRequestEvents( AppEngineQMLConstants::APP_FRONT_SCREEN, true, false, !bLateRaise, bLower );
#ifdef RAISE_WINDOW
             if((prevAppId == APP_NAVIGATION && IsVideoApp(GetThisAppId())) ||
                  (prevAppId == GetThisAppId() && GetThisAppId() == APP_AVP) )
                DoFrameBufferTransition(DISPLAY_FRONT);
#endif
         }
#ifdef RAISE_WINDOW
         if( GetRealAppId(DISPLAY_FRONT) == APP_CAMERA && (GetRealAppId(DISPLAY_REAR) == APP_AVP || GetRealAppId(DISPLAY_REAR) == APP_MOBILETV) ) 
            DoFrameBufferTransition(DISPLAY_FRONT);
#endif
		 
         if(  NULL != d_ptr->m_pFGFrontResponseTimer && m_AppHandlesResponseFg == false)
         {
            ((QTimer*)d_ptr->m_pFGFrontResponseTimer)->stop();
            ((QTimer*)d_ptr->m_pFGFrontResponseTimer)->start();
         }
         break;
      }
      case EVT_REQUEST_BG:
      {
         GetLogger().Log( "---> AppVextEngineQML::HandleEvent(EVT_REQUEST_BG)", Logger::PRIORITY_LOW );
         pView->CancelKey();
         CancelHwKey(DISPLAY_FRONT, true);
         if( GetIsDisplayedPopup(DISPLAY_FRONT))
         {
             SetIsDisplayedPopup( DISPLAY_FRONT, false );
             pView->HideSystemPopup( false );
         }

         if( d_ptr->m_frontRenderAppInBG != APP_NONE && d_ptr->m_frontRenderAppInBG != GetFrontApp()) { 
            GetLogger().Log(QString("afwfb : send render OFF to [%1]").arg(d_ptr->m_frontRenderAppInBG), Logger::PRIORITY_LOW); 
            Event evt(EVT_POST_EVENT, APP_SETTINGS, (APP_ID_T)(d_ptr->m_frontRenderAppInBG), EVT_REQUEST_RENDER_OFF);
            NotifyUISH(evt, DISPLAY_FRONT);
         }

         pView->SendBG();
         //if( m_bAutoVisibility == true )
         {
            if( !IsSwapDCEnabled() )
                 d_ptr->m_bFrontOnFront = false;
            ((AppMainWidget*)d_ptr->m_pMainWindow)->HandleRequestEvents( AppEngineQMLConstants::APP_FRONT_SCREEN, false, false );
         }
         if( ((AppMainWidget*)d_ptr->m_pMainWindow)->GetSceensUsage(AppEngineQMLConstants::APP_ALL_HIDE) == true )
         {
            ((AppMainWidget*)d_ptr->m_pMainWindow)->hide();
         }
         if(  NULL != d_ptr->m_pFGFrontResponseTimer )
         {
           ((QTimer*)d_ptr->m_pFGFrontResponseTimer)->stop();
         }
         break;
      }
       case EVT_CLONE_NOTIFICATION:
       {
         QDateTime	dateTime = QDateTime::currentDateTime();
         int cloneMode = (int)(((EVENT_CLONE_NOTIFICATION_DATA_T *)(aEvent.Data().data()))->sCloneStatus);
         GetLogger().Log(QString("TouchCleanUp front [%1][%2][%3][%4]").arg(cloneMode).arg(IsSwapDCEnabled()).arg(d_ptr->m_bFrontOnFront).arg(d_ptr->m_bFrontOnRear), Logger::PRIORITY_LOW);
         if (GetThisAppId() == GetFrontApp() || GetThisAppId() == GetRearApp())
            GetLogger().Log(QString("afw clone notification [%1] at %2").arg(cloneMode).arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz")), Logger::PRIORITY_LOW);
//         if( (cloneMode == CLONE_ON_FRONT && d_ptr->m_bFrontOnRear) )
//             SendTouchCleanUp();
//         if( (cloneMode == CLONE_ON_REAR && d_ptr->m_bFrontOnFront) )
//             SendTouchCleanUp();
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
            if( d_ptr->m_bFrontWillBeCloneOff )
                d_ptr->m_bFrontWillBeCloneOff = false;
            if( d_ptr->m_bRearWillBeCloneOff )
                d_ptr->m_bRearWillBeCloneOff = false;
            if (GetThisAppId() == GetFrontApp() || GetThisAppId() == GetRearApp())
                GetLogger().Log(QString("afwfb CLONE_OFF at %1").arg(dateTime.toString("hh:mm:ss.zzz")), Logger::PRIORITY_LOW);
            if( ((AppMainWidget*)d_ptr->m_pMainWindow)->GetFrontView()->isVisible()  ) {
                ((AppMainWidget*)d_ptr->m_pMainWindow)->GetFrontView()->CancelKey();
                CancelHwKey(DISPLAY_FRONT);
            }
            if( ((AppMainWidget*)d_ptr->m_pMainWindow)->GetRearView()->isVisible()  ) {
                ((AppMainWidget*)d_ptr->m_pMainWindow)->GetRearView()->CancelKey();
                CancelHwKey(DISPLAY_REAR);
            }
         }
         else {
            if( d_ptr->m_bFrontWillBeClone ) {
               d_ptr->m_bFrontWillBeClone = false;
               GetLogger().Log(QString("afwfb CLONE_ON").arg(dateTime.toString("hh:mm:ss.zzz")), Logger::PRIORITY_LOW);
            }
            else if( cloneMode == CLONE_ON_FRONT && GetThisAppId() == GetFrontApp() )
                DoFrontFBTransition(FB_TR_DO_FRONT_CLONE);
            SendTouchCleanUp();
            if( d_ptr->m_bRearWillBeClone ) {
               d_ptr->m_bRearWillBeClone = false;
               GetLogger().Log(QString("afwfb CLONE_ON").arg(dateTime.toString("hh:mm:ss.zzz")), Logger::PRIORITY_LOW);
            }
            else if( cloneMode == CLONE_ON_REAR && GetThisAppId() == GetRearApp() )
                DoRearFBTransition(FB_TR_DO_REAR_CLONE);
            if( ((AppMainWidget*)d_ptr->m_pMainWindow)->GetFrontView()->isVisible()  ) {
                ((AppMainWidget*)d_ptr->m_pMainWindow)->GetFrontView()->CancelKey();
                CancelHwKey(DISPLAY_FRONT);
            }
            if( ((AppMainWidget*)d_ptr->m_pMainWindow)->GetRearView()->isVisible()  ) {
                ((AppMainWidget*)d_ptr->m_pMainWindow)->GetRearView()->CancelKey();
                CancelHwKey(DISPLAY_REAR);
            }
         }
         break;
       }

       case EVT_SWAP_DC_ENABLED:
       {
           //if( GetThisAppId() == GetFrontApp() )
           //     DoFrontFBTransition(FB_TR_DO_SWAP_DC);
           //else if( GetThisAppId() == GetRearApp() )            // OK?
           //     DoRearFBTransition(FB_TR_DO_SWAP_DC);           
           break;
       }
/*
       case EVT_DRS_CHANGE:
       {
           APP_ENTRY_POINT_DATA_T* entryData = (APP_ENTRY_POINT_DATA_T *)(aEvent.Data().data());
           int entry = (int)(entryData->sAppEntryPoint);
           SetFrontEntry(entry);
           GetLogger().Log( QString("afwfb drs change :  fe=%1").arg(entry), Logger::PRIORITY_LOW);
           DoFrameBufferTransition(DISPLAY_FRONT);
           break;
       }
*/
       case EVT_LANGUAGE_CHANGED:
       {
           EVENT_LANGUAGE_CHANGED_DATA_T *lang = (EVENT_LANGUAGE_CHANGED_DATA_T *)aEvent.Data().data();
           if( lang != NULL ) {
              GetLogger().Log( QString("--->AppVextEngineQML::HandleEventRear() - EVT_LANGUAGE_CHANGED (%1)").arg(lang->languageId) );
              m_Language = lang->languageId;
              AppScreenView* pFrontView = ((AppMainWidget*)d_ptr->m_pMainWindow)->GetFrontView();
              AppScreenView* pRearView = ((AppMainWidget*)d_ptr->m_pMainWindow)->GetRearView();
              if( pFrontView != NULL )
                  pFrontView->SendLanguageChanged((int)lang->languageId);
              if( pRearView != NULL )
                  pRearView->SendLanguageChanged((int)lang->languageId);
           }
           break;
       }


/*!--[ DHAVN_TELECA_ininragr_CR9046--20120602 DD : RRC Jog is not working in Forward Clone mode and CCP Jog is not working in Reverse Clone mode--*/

/** ----- Jog events ----- */
      case EVT_KEY_CCP_JOG_DIAL_UP:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_UP,
                       message->keyStatus, false );
         break;
      }

      case EVT_KEY_RRC_JOG_DIAL_UP:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_UP,
                       message->keyStatus, true );
         break;
      }

      case EVT_KEY_CCP_JOG_DIAL_DOWN:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_DOWN,
                       message->keyStatus, false );
         break;
      }

      case EVT_KEY_RRC_JOG_DIAL_DOWN:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_DOWN,
                       message->keyStatus, true );
         break;
      }

      case EVT_KEY_CCP_JOG_DIAL_LEFT:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_LEFT,
                       message->keyStatus, false );
         break;
      }

      case EVT_KEY_RRC_JOG_DIAL_LEFT:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_LEFT,
                       message->keyStatus, true );
         break;
      }

      case EVT_KEY_CCP_JOG_DIAL_RIGHT:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_RIGHT,
                       message->keyStatus, false );
         break;
      }

      case EVT_KEY_RRC_JOG_DIAL_RIGHT:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_RIGHT,
                       message->keyStatus, true );
         break;
      }

      case EVT_KEY_CCP_JOG_DIAL_CENTER:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_CENTER,
                       message->keyStatus, false );
         break;
      }

      case EVT_KEY_RRC_JOG_DIAL_CENTER:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_CENTER,
                       message->keyStatus, true );
         break;
      }

      case EVT_KEY_CCP_JOG_DIAL_TOP_RIGHT:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_TOP_RIGHT,
                      message->keyStatus, false );
         break;
      }

      case EVT_KEY_RRC_JOG_DIAL_TOP_RIGHT:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_TOP_RIGHT,
                      message->keyStatus, true );
         break;
      }

      case EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_BOTTOM_RIGHT,
                      message->keyStatus, false );
         break;
      }

      case EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_BOTTOM_RIGHT,
                      message->keyStatus, true );
         break;
      }

      case EVT_KEY_CCP_JOG_DIAL_TOP_LEFT:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_TOP_LEFT,
                     message->keyStatus, false );
         break;
      }

      case EVT_KEY_RRC_JOG_DIAL_TOP_LEFT:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_TOP_LEFT,
                     message->keyStatus, true );
         break;
      }

      case EVT_KEY_CCP_JOG_DIAL_DOWN_LEFT:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_BOTTOM_LEFT,
                       message->keyStatus, false );
         break;
      }
        
      case EVT_KEY_RRC_JOG_DIAL_DOWN_LEFT:
      {
         pView->SendJogEvent( AppEngineQMLConstants::JOG_BOTTOM_LEFT,
                       message->keyStatus, true );
         break;
      }

      case EVT_KEY_CCP_JOG_DIAL_ENCODER:
      {
         if( (int)aEvent.GetCommand() > 0 )
         {
            pView->SendJogEvent( AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                          abs((int)aEvent.GetCommand()), false );
         }
         else
         {
            pView->SendJogEvent( AppEngineQMLConstants::JOG_WHEEL_LEFT,
                          abs((int)aEvent.GetCommand()), false );
         }
         break;
      }

      case EVT_KEY_RRC_JOG_DIAL_ENCODER:
      {
         if( (int)aEvent.GetCommand() > 0 )
         {
            pView->SendJogEvent( AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                          abs((int)aEvent.GetCommand()), true );
         }
         else
         {
            pView->SendJogEvent( AppEngineQMLConstants::JOG_WHEEL_LEFT,
                          abs((int)aEvent.GetCommand()), true );
         }
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
/** ----- end Jog events ----- */
/*!-- ] DHAVN_TELECA_ininragr_CR9046--20120602 DD : RRC Jog is not working in Forward Clone mode and CCP Jog is not working in Reverse Clone mode --*/

      default:
         result = false;
         break;
   }

   GetLogger().Log( "<--- AppVextEngineQML::HandleEvent()" );
   return result;
}

bool
AppVextEngineQML::HandleEventRear( Event &aEvent )
{
    GetLogger().Log( "<--- AppVextEngineQML::HandleEventRear()" );
    bool result = true;

/*!-- [ DHAVN_TELECA_ininragr_CR11100--20120712 AFW : Coverity_R5_4_60 --*/

    if( d_ptr->m_pMainWindow == NULL )
    {
        return false;
    }
/*!-- ] DHAVN_TELECA_ininragr_CR11100--20120712 AFW : Coverity_R5_4_60 --*/

    EVT_ID_T nEventID = aEvent.GetEventId();
    KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
    AppScreenView* pView = ((AppMainWidget*)d_ptr->m_pMainWindow)->GetRearView();
    AppScreenView* pFrontView = ((AppMainWidget*)d_ptr->m_pMainWindow)->GetFrontView() ;
    if( pFrontView != NULL &&
        ( GetDisplayType() ==  DISPLAY_FRONT ||
          ( pFrontView->getCurrentScreen() == AppEngineQMLConstants::SCREEN_FRONT_EXTENDED &&
            ( nEventID == EVT_SHOW_POPUP || nEventID == EVT_REQUEST_FG || nEventID == EVT_REQUEST_BG )
          )
        )
      )
    {       
       GetLogger().Log( "<--- AppVextEngineQML::move to front" );
       pView = pFrontView;
    }
    if( NULL == pView )
    {
       return false;
    }

    GetLogger().Log( "---> AppVextEngineQML::HandleEventRear() event");

    switch ( nEventID )
    {
       // This event could be supported on QML applications only, since POPUP plugin based on QML
       case EVT_HIDE_POPUP:
       {
          memset(&d_ptr->m_RearPopup, 0, sizeof(POPUP_SHOW_T));
          d_ptr->m_RearPopup.popup_type = _CURRENT_POPUP_NONE;
          SetIsDisplayedPopup( DISPLAY_REAR, false );
          pView->HideSystemPopup( true );
          break;
       }

       case EVT_SHOW_POPUP:
       {
          POPUP_SHOW_T* popupData = (POPUP_SHOW_T *)(aEvent.Data().data());
          SetIsDisplayedPopup( DISPLAY_REAR, true );
          UpdateStatusbarOnPopupEvent(aEvent, GetRearFullScreen());
          CancelHwKey(DISPLAY_REAR);
          GetLogger().Log( QString("showPopUpInContext %1 %2 %3").arg(d_ptr->m_RearPopup.popup_type).arg(popupData->popup_type).arg(d_ptr->m_RearPopup.responsesource) , Logger::PRIORITY_LOW );
          if((d_ptr->m_RearPopup.popup_type != popupData->popup_type) && (d_ptr->m_RearPopup.popup_type != _CURRENT_POPUP_NONE))
          {
              handleDumyPopupResponseRear();
              memcpy(&d_ptr->m_RearPopup, popupData, sizeof(POPUP_SHOW_T));
          }
          else
          {
              memcpy(&d_ptr->m_RearPopup, popupData, sizeof(POPUP_SHOW_T));
          }
          pView->ShowSystemPopup(aEvent, GetThisAppId());
          break;
       }

       case EVT_REQUEST_FG:
       {
          GetLogger().Log( QString("AppVextEngineQML::HandleEventRear:(EVT_REQUEST_FG) (fg=%1, visible=%2)").
            arg((int)m_AppHandlesResponseFg).arg((int)m_bAutoVisibility), Logger::PRIORITY_LOW);
            memset(&d_ptr->m_RearPopup, 0, sizeof(POPUP_SHOW_T));
            d_ptr->m_RearPopup.popup_type = _CURRENT_POPUP_NONE;
          int prevAppId = GetRearApp();
          int prevEntryId = GetRearEntry();
          int bLateRaise = true;
		 int bLower  = false;

          SetRearApp(GetThisAppId());
          SetRearGuiApp(GetThisAppId());
          APP_ENTRY_POINT_DATA_T* entryData = (APP_ENTRY_POINT_DATA_T *)(aEvent.Data().data());
          d_ptr->m_bRearWillBeClone = false;
          d_ptr->m_bRearWillBeCloneOff = false;
          d_ptr->m_bDoSwap = false;
          if( GetCloneMode() == CLONE_OFF && entryData->sHeader.bReserved02 )
             d_ptr->m_bRearWillBeClone = true;
          else if( GetCloneMode() != CLONE_OFF && entryData->sHeader.bReserved05 )
            d_ptr->m_bDoSwap = true;
          else if( GetCloneMode() != CLONE_OFF && !entryData->sHeader.bReserved02 )
             d_ptr->m_bRearWillBeCloneOff = true;
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
		  

          if( IsDisplaySetting(entry) ) {
            if( prevAppId != APP_AVP && prevAppId != APP_MOBILETV ) {
                int renderApp = d_ptr->m_rearRenderAppInBG; 
                if( d_ptr->m_rearRenderAppInBG == APP_NONE ) {
                    GetLogger().Log("afwfb : get otherside rendering app", Logger::PRIORITY_LOW);
                    renderApp = d_ptr->m_frontRenderAppInBG;
                }
                GetLogger().Log(QString("afwfb : send render ON to [%1]").arg(renderApp), Logger::PRIORITY_LOW); 
                Event evt(EVT_POST_EVENT, APP_SETTINGS, (APP_ID_T)(renderApp), EVT_REQUEST_RENDER_ON);
                NotifyUISH(evt, DISPLAY_REAR);
             }
             else {
                d_ptr->m_rearRenderAppInBG = prevAppId;
                SetRearRenderAppInBG(prevAppId);
                SetRearRenderEntryInBG(prevEntryId);
             }
          }
          else {
             d_ptr->m_rearRenderAppInBG = APP_NONE;
             SetRearRenderAppInBG(APP_NONE);
             SetRearRenderEntryInBG(eAPP_NONE);
          }

          GetLogger().Log( QString("afwfb fg :  s=%1 c=%2 e=%3 ec=%4 efc=%5 erc=%6 es=%7 ecoff=%8").
            arg(IsSwapDCEnabled()).arg(GetCloneMode()).arg(entry).
            arg(entryData->sHeader.bReserved02).arg(entryData->sHeader.bReserved03).
            arg(entryData->sHeader.bReserved04).arg(entryData->sHeader.bReserved05).
            arg(entryData->sHeader.bReserved06), Logger::PRIORITY_LOW);
                     
          pView->SendFG();
          if( IsSwapDCEnabled() ) {
            SendTouchCleanUp();
            d_ptr->m_bFrontOnRear = true;
          }
          else if( GetCloneMode() == CLONE_ON_REAR )
             SendTouchCleanUp();

          if( m_bAutoVisibility == true )
          {
#ifdef RAISE_WINDOW
             // exception for asembled rendering between navigation & video
             if( prevAppId == APP_NAVIGATION && IsVideoApp(GetThisAppId()) )
                bLateRaise = false;
			else
#endif
				bLower = true;
             ((AppMainWidget*)d_ptr->m_pMainWindow)->HandleRequestEvents( AppEngineQMLConstants::APP_REAR_SCREEN, true, false, !bLateRaise, bLower );
#ifdef RAISE_WINDOW
             if((prevAppId == APP_NAVIGATION && IsVideoApp(GetThisAppId())) ||
                  (prevAppId == GetThisAppId() && GetThisAppId() == APP_AVP) )
                DoFrameBufferTransition(DISPLAY_REAR);
#endif
          }
#ifdef RAISE_WINDOW
          if( GetRealAppId(DISPLAY_FRONT) == APP_CAMERA && (GetRealAppId(DISPLAY_REAR) == APP_AVP || GetRealAppId(DISPLAY_REAR) == APP_MOBILETV) )
             DoFrameBufferTransition(DISPLAY_REAR);
#endif

          if(  NULL != d_ptr->m_pFGRearResponseTimer && m_AppHandlesResponseFg == false )
          {
             ((QTimer*)d_ptr->m_pFGRearResponseTimer)->stop();
             ((QTimer*)d_ptr->m_pFGRearResponseTimer)->start();
          }
          break;
       }
       case EVT_REQUEST_BG:
       {
         GetLogger().Log( "---> AppVextEngineQML::HandleEventRear(EVT_REQUEST_BG)", Logger::PRIORITY_LOW );
         pView->CancelKey();
         CancelHwKey(DISPLAY_REAR,true);
         if( GetIsDisplayedPopup(DISPLAY_REAR))
         {
             SetIsDisplayedPopup( DISPLAY_REAR, false );
             pView->HideSystemPopup( false );
         }

         if( d_ptr->m_rearRenderAppInBG != APP_NONE && d_ptr->m_rearRenderAppInBG != GetRearApp()) { 
            GetLogger().Log(QString("afwfb : send render OFF to [%1]").arg(d_ptr->m_rearRenderAppInBG), Logger::PRIORITY_LOW); 
            Event evt(EVT_POST_EVENT, APP_SETTINGS, (APP_ID_T)(d_ptr->m_rearRenderAppInBG), EVT_REQUEST_RENDER_OFF);
            NotifyUISH(evt, DISPLAY_REAR);
         }

          pView->SendBG();
          //if( m_bAutoVisibility == true )
          {
             if( IsSwapDCEnabled() )
                  d_ptr->m_bFrontOnRear = false;
             ((AppMainWidget*)d_ptr->m_pMainWindow)->HandleRequestEvents( AppEngineQMLConstants::APP_REAR_SCREEN, false, false );
          }
          if( ((AppMainWidget*)d_ptr->m_pMainWindow)->GetSceensUsage(AppEngineQMLConstants::APP_ALL_HIDE) == true )
          {
             ((AppMainWidget*)d_ptr->m_pMainWindow)->hide();
          }
          if(  NULL != d_ptr->m_pFGRearResponseTimer )
          {
             ((QTimer*)d_ptr->m_pFGRearResponseTimer)->stop();
          }
          break;
       }
/*
       case EVT_DRS_CHANGE:
       {
           APP_ENTRY_POINT_DATA_T* entryData = (APP_ENTRY_POINT_DATA_T *)(aEvent.Data().data());
           int entry = (int)(entryData->sAppEntryPoint);
           SetRearEntry(entry);
           GetLogger().Log( QString("afwfb drs change :  re=%1").arg(entry), Logger::PRIORITY_LOW);
           DoFrameBufferTransition(DISPLAY_REAR);
           break;
       }
*/
       case EVT_LANGUAGE_CHANGED:
       {
           EVENT_LANGUAGE_CHANGED_DATA_T *lang = (EVENT_LANGUAGE_CHANGED_DATA_T *)aEvent.Data().data();
           if( lang != NULL ) {
              GetLogger().Log( QString("--->AppVextEngineQML::HandleEventRear() - EVT_LANGUAGE_CHANGED (%1)").arg(lang->languageId) );
              m_Language = lang->languageId;
              AppScreenView* pFrontView = ((AppMainWidget*)d_ptr->m_pMainWindow)->GetFrontView();
              AppScreenView* pRearView = ((AppMainWidget*)d_ptr->m_pMainWindow)->GetRearView();
              if( pFrontView != NULL )
                  pFrontView->SendLanguageChanged((int)lang->languageId);
              if( pRearView != NULL )
                  pRearView->SendLanguageChanged((int)lang->languageId);
           }
           break;
       }

/*!--[ DHAVN_TELECA_ininragr_CR9046--20120602 DD : RRC Jog is not working in Forward Clone mode and CCP Jog is not working in Reverse Clone mode--*/
 /** ----- Jog events ----- */
       case EVT_KEY_RRC_JOG_DIAL_UP:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_UP,
                        message->keyStatus, true );
          break;
       }

       case EVT_KEY_CCP_JOG_DIAL_UP:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_UP,
                        message->keyStatus, false );
          break;
       }

       case EVT_KEY_RRC_JOG_DIAL_DOWN:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_DOWN,
                        message->keyStatus, true );
          break;
       }

       case EVT_KEY_CCP_JOG_DIAL_DOWN:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_DOWN,
                        message->keyStatus, false );
          break;
       }

       case EVT_KEY_RRC_JOG_DIAL_LEFT:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_LEFT,
                        message->keyStatus, true );
          break;
       }

       case EVT_KEY_CCP_JOG_DIAL_LEFT:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_LEFT,
                        message->keyStatus, false );
          break;
       }

       case EVT_KEY_RRC_JOG_DIAL_RIGHT:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_RIGHT,
                        message->keyStatus, true );
          break;
       }

       case EVT_KEY_CCP_JOG_DIAL_RIGHT:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_RIGHT,
                        message->keyStatus, false );
          break;
       }

       case EVT_KEY_RRC_JOG_DIAL_CENTER:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_CENTER,
                        message->keyStatus, true );
          break;
       }

       case EVT_KEY_CCP_JOG_DIAL_CENTER:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_CENTER,
                        message->keyStatus, false );
          break;
       }

       case EVT_KEY_CCP_JOG_DIAL_TOP_RIGHT:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_TOP_RIGHT,
                       message->keyStatus, false );
          break;
       }

       case EVT_KEY_RRC_JOG_DIAL_TOP_RIGHT:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_TOP_RIGHT,
                       message->keyStatus, true );
          break;
       }

       case EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_BOTTOM_RIGHT,
                       message->keyStatus, false );
          break;
       }

       case EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_BOTTOM_RIGHT,
                       message->keyStatus, true );
          break;
       }

       case EVT_KEY_CCP_JOG_DIAL_TOP_LEFT:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_TOP_LEFT,
                      message->keyStatus, false );
          break;
       }

       case EVT_KEY_RRC_JOG_DIAL_TOP_LEFT:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_TOP_LEFT,
                      message->keyStatus, true );
          break;
       }

       case EVT_KEY_CCP_JOG_DIAL_DOWN_LEFT:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_BOTTOM_LEFT,
                       message->keyStatus, false );
          break;
       }

       case EVT_KEY_RRC_JOG_DIAL_DOWN_LEFT:
       {
          pView->SendJogEvent( AppEngineQMLConstants::JOG_BOTTOM_LEFT,
                       message->keyStatus, true );
          break;
       }

       case EVT_KEY_RRC_JOG_DIAL_ENCODER:
       {
          if( (int)aEvent.GetCommand() > 0 )
          {
             pView->SendJogEvent( AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                           abs((int)aEvent.GetCommand()), true );
          }
          else
          {
             pView->SendJogEvent( AppEngineQMLConstants::JOG_WHEEL_LEFT,
                           abs((int)aEvent.GetCommand()), true );
          }
          break;
       }

       case EVT_KEY_CCP_JOG_DIAL_ENCODER:
       {
          if( (int)aEvent.GetCommand() > 0 )
          {
             pView->SendJogEvent( AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                           abs((int)aEvent.GetCommand()), false );
          }
          else
          {
             pView->SendJogEvent( AppEngineQMLConstants::JOG_WHEEL_LEFT,
                           abs((int)aEvent.GetCommand()), false );
          }
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
 /** ----- end Jog events ----- */
/*!-- ] DHAVN_TELECA_ininragr_CR9046--20120602 DD : RRC Jog is not working in Forward Clone mode and CCP Jog is not working in Reverse Clone mode --*/

       default:
          result = false;
          break;
    }

    GetLogger().Log( "<--- AppVextEngineQML::HandleEventRear()" );
    return result;
}

/**popup handlers */
void AppVextEngineQML::HandlePopupResponseFront( int target, QVariant response )
{
   AppScreenView* pView = ((AppMainWidget*)d_ptr->m_pMainWindow)->GetFrontView() ;
   if( NULL != pView && d_ptr->m_bPopupCloseIgnore == false)
   {
       pView->HideSystemPopup(true);
       SetIsDisplayedPopup(DISPLAY_FRONT, false);
   }

   POPUP_RESPONSE_T message;
   memset(&message, NULL, sizeof(POPUP_RESPONSE_T));
   message.type = ((POPUP_RESPONSE_T* )response.toByteArray().data())->type;
   message.btn = ((POPUP_RESPONSE_T* )response.toByteArray().data())->btn;
   message.index = ((POPUP_RESPONSE_T* )response.toByteArray().data())->index;
   message.value = ((POPUP_RESPONSE_T* )response.toByteArray().data())->value;
   message.popup_type = d_ptr->m_FrontPopup.popup_type;
   message.responsesource = (int)d_ptr->m_FrontPopup.responsesource;
   GetLogger().Log(QString( "AppEngineQML::handlePopupResponse(), target : [%1] [%2] [%3]" ).arg( target ).arg(message.popup_type).arg(message.responsesource));
   Event event( EVT_POPUP_RESPONSE, GetThisAppId(), APP_UI_MANAGER, QByteArray((char *)&message,sizeof(message)) );
   event.SetCommand(false);
   //convert to the new interface
   NotifyUISH( event, DISPLAY_FRONT );
}


void AppVextEngineQML::HandlePopupResponseRear( int target, QVariant response )
{
   AppScreenView* pView = ((AppMainWidget*)d_ptr->m_pMainWindow)->GetRearView();
   if( NULL != pView  && d_ptr->m_bPopupCloseIgnore == false)
   {
       pView->HideSystemPopup(true);
       SetIsDisplayedPopup(DISPLAY_REAR, false);
   }
   
   POPUP_RESPONSE_T message;
   memset(&message, NULL, sizeof(POPUP_RESPONSE_T));
   message.type = ((POPUP_RESPONSE_T* )response.toByteArray().data())->type;
   message.btn = ((POPUP_RESPONSE_T* )response.toByteArray().data())->btn;
   message.index = ((POPUP_RESPONSE_T* )response.toByteArray().data())->index;
   message.value = ((POPUP_RESPONSE_T* )response.toByteArray().data())->value;
   message.popup_type = d_ptr->m_RearPopup.popup_type;
   message.responsesource = (int)d_ptr->m_RearPopup.responsesource;
   GetLogger().Log(QString( "AppEngineQML::handlePopupResponse(), target : [%1] [%2] [%3]" ).arg( target ).arg(message.popup_type).arg(message.responsesource));
   Event event( EVT_POPUP_RESPONSE, GetThisAppId(), APP_UI_MANAGER, QByteArray((char *)&message,sizeof(message)) );
   event.SetCommand(false);
   //convert to the new interface
   NotifyUISH( event, DISPLAY_REAR );
}


void
AppVextEngineQML::SlotCallFgFrontResponce()
{
   if(  NULL != d_ptr->m_pFGFrontResponseTimer )
   {
      ((QTimer*)d_ptr->m_pFGFrontResponseTimer)->stop();
   }
   GetLogger().Log( "----> AppVextEngineQML::SlotCallFgFrontResponce()" );

   HandleAutoVisibility(DISPLAY_FRONT);
   if( NULL != d_ptr->m_pMainWindow && m_AppHandlesResponseFg == false )
   {
      if( ((AppMainWidget*)d_ptr->m_pMainWindow)->GetSceensUsage(AppEngineQMLConstants::APP_FRONT_SCREEN) )
      {
         SendFGResponse(DISPLAY_FRONT);
      }
   }
   GetLogger().Log( "<---- AppVextEngineQML::SlotCallFgFrontResponce()" );
}

void
AppVextEngineQML::SlotCallFgRearResponce()
{
   if(  NULL != d_ptr->m_pFGRearResponseTimer )
   {
      ((QTimer*)d_ptr->m_pFGRearResponseTimer)->stop();
   }
   GetLogger().Log( "----> AppVextEngineQML::SlotCallFgRearResponce()" );

   HandleAutoVisibility(DISPLAY_REAR);
   if( NULL != d_ptr->m_pMainWindow && m_AppHandlesResponseFg == false )
   {
      if( ((AppMainWidget*)d_ptr->m_pMainWindow)->GetSceensUsage(AppEngineQMLConstants::APP_REAR_SCREEN) )
      {
         SendFGResponse(DISPLAY_REAR);
      }
   }
   GetLogger().Log( "<---- AppVextEngineQML::SlotCallFgRearResponce()" );
}

void
AppVextEngineQML::ResizeMainWindow( int width, int height )
{
    GetLogger().Log( QString( "AppVextEngineQML::ResizeMainWindow: widht=[%1] height=[%2]" ).arg( width ).arg( height ) );

    if( NULL != d_ptr->m_pMainWindow )
    {
        //d_ptr->m_pMainWindow->resize( width, height );
    }
}

/**
 * Swap screen contents between front and rear
 */
void
AppVextEngineQML::SwapScreenContents(bool isRearFGIbox)
{
    GetLogger().Log( "AppVextEngineQML::SwapScreenContents: " );
    if( NULL != d_ptr->m_pMainWindow )
    {
        // disconnect previous connection
        {
            GetLogger().Log( "AppVextEngineQML::SwapScreenContents: disconnect current connection of HandlePopupResponse" );
            AppScreenView* pFrontView = ((AppMainWidget*)d_ptr->m_pMainWindow)->GetFrontView();
            AppScreenView* pRearView = ((AppMainWidget*)d_ptr->m_pMainWindow)->GetRearView();
            if ( pFrontView != NULL )
            {
				if( GetIsDisplayedPopup(DISPLAY_FRONT))
				{
					SetIsDisplayedPopup( DISPLAY_FRONT, false );
            		pFrontView->HideSystemPopup(false);
				}
                disconnect(pFrontView, SIGNAL( HandlePopupResponse(  int, QVariant )), this, SLOT(HandlePopupResponseFront( int, QVariant )) );
            }
            if ( pRearView != NULL )
            {
            	if( GetIsDisplayedPopup(DISPLAY_REAR))
				{
					SetIsDisplayedPopup( DISPLAY_REAR, false );
            		pRearView->HideSystemPopup(false);
				}
                disconnect(pRearView, SIGNAL( HandlePopupResponse( int, QVariant )), this, SLOT(HandlePopupResponseRear( int, QVariant )) );
            }
        } // ~disconnect previous connection

        // swap ScreenView
		((AppMainWidget*)d_ptr->m_pMainWindow)->SwapScreenView(isRearFGIbox);

        // make new connection for swapped ScreenView
        {
            GetLogger().Log( "AppVextEngineQML::SwapScreenContents: re-connect with swapped view " );
            AppScreenView* pNewFrontView = ((AppMainWidget*)d_ptr->m_pMainWindow)->GetFrontView();
            AppScreenView* pNewRearView = ((AppMainWidget*)d_ptr->m_pMainWindow)->GetRearView();
            if ( pNewFrontView != NULL )
            {
                connect(pNewFrontView, SIGNAL( HandlePopupResponse( int, QVariant )), this, SLOT(HandlePopupResponseFront( int, QVariant )) );
            }
            if ( pNewRearView != NULL )
            {
                connect(pNewRearView, SIGNAL( HandlePopupResponse( int, QVariant )), this, SLOT(HandlePopupResponseRear( int, QVariant )) );
            }
        } // ~make new connection for swapped ScreenView
    }
}


void AppVextEngineQML::HandleAutoVisibility(uint nScreen)
{
#ifdef RAISE_WINDOW
    if( m_bAutoVisibility ) {    
        if( nScreen == DISPLAY_FRONT ) 
        {
            if( d_ptr->m_pMainWindow != NULL  )
            {
                ((AppMainWidget*)d_ptr->m_pMainWindow)->HandleRequestEvents( AppEngineQMLConstants::APP_FRONT_SCREEN, true, false );
                GetLogger().Log( QString("AppVextEngineQML::HandleAutoVisibilty() front show") );   
            }
       }
       else if( nScreen == DISPLAY_REAR )
       {
            if( d_ptr->m_pMainWindow != NULL  )
            {
                ((AppMainWidget*)d_ptr->m_pMainWindow)->HandleRequestEvents( AppEngineQMLConstants::APP_REAR_SCREEN, true, false );
                GetLogger().Log( "AppVextEngineQML::HandleAutoVisibility() rear show" );
            }
       }
       else 
       {
            GetLogger().Log("AppVextEngineQML::HandleAutoVisibility() try to invalid screen!", Logger::PRIORITY_LOW);
       }
   }
#endif

   d_ptr->m_bFBTransitionOnResponse = true;
   d_ptr->m_nDisplay4LateTransition = nScreen;
   
   int time = 100;
    if( nScreen == DISPLAY_FRONT )
        QTimer::singleShot(time, this, SLOT(DoFrameBufferTransitionFront()));
    else
        QTimer::singleShot(time, this, SLOT(DoFrameBufferTransitionRear()));
}

void AppVextEngineQML::DoFrameBufferTransitionFront()
{
    if( d_ptr->m_bDoSwap && !d_ptr->m_bFBTransitionOnResponse )
        return;
    
    GetLogger().Log(QString("DoFrameBufferTransition() afwfb fcon=%1 fcoff=%2 rcon=%3 rcoff=%4 s=%5").
        arg(d_ptr->m_bFrontWillBeClone).arg(d_ptr->m_bFrontWillBeCloneOff).
        arg(d_ptr->m_bRearWillBeClone).arg(d_ptr->m_bRearWillBeCloneOff).
        arg(d_ptr->m_bDoSwap));

    if( d_ptr->m_bFrontWillBeClone )
        DoFrontFBTransition(FB_TR_CLONED);
    else if( d_ptr->m_bDoSwap ) {
        DoFrontFBTransition(FB_TR_DO_SWAP_DC);
        d_ptr->m_bDoSwap = false;
    }
    else if( d_ptr->m_bFrontWillBeCloneOff )
        DoFrontFBTransition(FB_TR_NORMAL);
    else {
    if( GetCloneMode() == CLONE_OFF )
        DoFrontFBTransition(FB_TR_NORMAL);
    else
        DoFrontFBTransition(FB_TR_CLONED);
    }
    d_ptr->m_bFBTransitionOnResponse = false;
}

void AppVextEngineQML::DoFrameBufferTransitionRear()
{
    if( d_ptr->m_bDoSwap && !d_ptr->m_bFBTransitionOnResponse )
        return;

    GetLogger().Log(QString("DoFrameBufferTransition() afwfb fcon=%1 fcoff=%2 rcon=%3 rcoff=%4 s=%5").
        arg(d_ptr->m_bFrontWillBeClone).arg(d_ptr->m_bFrontWillBeCloneOff).
        arg(d_ptr->m_bRearWillBeClone).arg(d_ptr->m_bRearWillBeCloneOff).
        arg(d_ptr->m_bDoSwap));

      if( d_ptr->m_bRearWillBeClone )
        DoRearFBTransition(FB_TR_CLONED);
      else if( d_ptr->m_bDoSwap ) {
        DoRearFBTransition(FB_TR_DO_SWAP_DC);
        d_ptr->m_bDoSwap = false;
      }
      else if( d_ptr->m_bRearWillBeCloneOff )
        DoRearFBTransition(FB_TR_NORMAL);
      else {
        if( GetCloneMode() == CLONE_OFF)
            DoRearFBTransition(FB_TR_NORMAL);
        else 
            DoRearFBTransition(FB_TR_CLONED);
      }
      d_ptr->m_bFBTransitionOnResponse = false;
}

void AppVextEngineQML::DoFrameBufferTransition(uint nScreen)
{
    if( nScreen == DISPLAY_FRONT ) {
        DoFrameBufferTransitionFront();
    }
    else {
        DoFrameBufferTransitionRear();
    }
}

/**popup handlers */
void AppVextEngineQML::HandleTouchViewFrontPort( QEvent *aEvent )
{
   emit SendTouchport(aEvent);
}

void AppVextEngineQML::HandleTouchViewRearPort( QEvent *aEvent )
{
   emit SendTouchport(aEvent);
}

void AppVextEngineQML::handleDumyPopupResponseFront()
{
   GetLogger().Log(QString( "AppVextEngineQML::handleDumyPopupResponseFront(), target " ));
    POPUP_RESPONSE_T message;
    memset(&message, NULL, sizeof(POPUP_RESPONSE_T));
    message.btn = POPUP_BUTTON_NONE;
    message.popup_type = d_ptr->m_FrontPopup.popup_type;
    message.responsesource = (int)d_ptr->m_FrontPopup.responsesource;
   Event event( EVT_POPUP_RESPONSE, GetThisAppId(), APP_UI_MANAGER, QByteArray((char *)&message,sizeof(message)) );
   event.SetCommand(true);
   NotifyUISH( event, DISPLAY_FRONT );
}

void AppVextEngineQML::handleDumyPopupResponseRear()
{
   GetLogger().Log(QString( "AppVextEngineQML::handleDumyPopupResponseRear(), target" ));
    POPUP_RESPONSE_T message;
    memset(&message, NULL, sizeof(POPUP_RESPONSE_T));
    message.btn = POPUP_BUTTON_NONE;
    message.popup_type = d_ptr->m_RearPopup.popup_type;
    message.responsesource = (int)d_ptr->m_RearPopup.responsesource;
   Event event( EVT_POPUP_RESPONSE, GetThisAppId(), APP_UI_MANAGER, QByteArray((char *)&message,sizeof(message)) );
   event.SetCommand(true);
   NotifyUISH( event, DISPLAY_REAR );
}
