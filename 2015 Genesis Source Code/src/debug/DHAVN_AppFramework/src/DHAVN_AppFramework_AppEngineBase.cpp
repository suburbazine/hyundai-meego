
// Inlcudes
#include "DHAVN_AppFramework_AppEngineBase.h"
#include "DHAVN_AppFramework_Event_Def.h"
#ifndef REMOVE_APPTOPWIDGET
#include "DHAVN_AppFramework_AppTopWidget.h"
#endif
#include "DHAVN_AppFramework_AppScreenView.h"

#include <QGraphicsItem>
#include <QtServiceFramework/QRemoteServiceRegister>
#include <QtServiceFramework/QServiceManager>
#include <QKeyEvent>
#include <string>
#include <QTimer>
#include <QGraphicsSceneEvent>
#include <QSharedMemory>
#include "DHAVN_QRPMClient_Internal.h"
#include "DHAVN_AppFramework_IPC.h"
#include "DHAVN_AppFramework_AppEngineQML_Def.h"
#include "DHAVN_AppFramework_AppVextEngineQML_Def.h"
#include "DHAVN_DualDisplayHelper.h"
#include "DHAVN_AppFramework_Popup.h"

// Defines
#define UI_STATE_DAEMON_NAME "UIStateMgrDaemon"
#define BG_TIMEOUT            500

#define GP_CHECK_INTERVAL 200
//#define GP_MAX_WAIT_TIME 1000
#define GP_MAX_WAIT_TIME 1600

QTM_USE_NAMESPACE

struct GstPipelineUsage {
    bool use[3];
    int app[3];
};

struct AppUsage {
    int m_frontEntry;
    int m_rearEntry;
    int m_frontGuiApp;
    int m_rearGuiApp;
    int m_frontApp;
    int m_rearApp;
    int m_frontRenderAppInBG;
    int m_rearRenderAppInBG;
    int m_frontRenderEntryInBG;
    int m_rearRenderEntryInBG;
};

struct DisplaySetting {
    unsigned int brightness[AppEngineBase::ADS_MAX];
    unsigned int constrast[AppEngineBase::ADS_MAX];
    unsigned int tint[AppEngineBase::ADS_MAX];
    unsigned int hue[AppEngineBase::ADS_MAX];
};

struct DisplayPlane {
    unsigned int displayPlane[APP_TOTAL];
};

/** AppEngineBasePrivate class definition */
class AppEngineBasePrivate
{

private:

    /** Constructor */
    AppEngineBasePrivate( AppEngineBase *pParent, QWidget *pQAppWidget, Logger* logger, AppEngineBase::APP_ENGINE_TYPE nType)
      :
#ifndef REMOVE_APPTOPWIDGET
      m_pMainWindow(NULL),
#endif
      m_pIPC(NULL),m_pQAppWidget(pQAppWidget),
      m_Logger(*logger),m_nEngineType(nType),m_bIsSwapDCEnabled(false),
      m_bTouchPressed(false),m_bTouchLock(false),m_bTouchEnable(false),m_bJogIgnored(false), m_bHUSeekLock(false), m_bRRCSeekLock(false),
      m_bSWRCSeekLock(false),m_bHUSeekPress(false),m_bCCPSeekPress(false),m_bRRCSeekPress(false),
      m_hwKeyKind(0), m_bHwKeyPressed(false), m_nJogKeyPressed(JOG_NONE_PRESSED), m_bCameraEnabled(false),
      m_cloneMode(0), m_DRS(0), m_micomManager(NULL), m_pDDHelper(NULL),
      m_shmFb("AppEngineAppUsage"), m_shmDs("AppEngineDisplaySetting"), m_shmDp("AppEngineDisplayPlane"),
      m_pGPFrontTimer(NULL),m_pGPRearTimer(NULL),m_pGPCameraTimer(NULL),
      m_nGPFrontElapsed(0),m_nGPRearElapsed(0),m_nGPCameraElapsed(0),
      m_nGstOccupiedApp(0), m_shm("GstreamerPipelinesStatus"),
      m_bFrontFullScreen(false), m_bRearFullScreen(false),
      q_ptr(pParent)
    {
        m_Logger.Log( "AppEngineBasePrivate::AppEngineBasePrivate pre-created" );
        if( !m_shm.create(sizeof(GstPipelineUsage)) )
        {
            m_Logger.Log( "AppEngineBasePrivate::AppEngineBasePrivate unable to create shared memory, try to attach." );
            if(!m_shm.attach())
            {
                m_Logger.Log("AppEngineBasePrivate::AppEngineBasePrivate unable to attach to shared memory");
            }
        }
        m_pDDHelper = new DualDisplayHelper();
        if( !m_shmFb.create(sizeof(AppUsage)) )
        {
            m_Logger.Log( "AppEngineBasePrivate::AppEngineBasePrivate unable to create shared memory FB, try to attach." );
            if(!m_shmFb.attach())
            {
                m_Logger.Log("AppEngineBasePrivate::AppEngineBasePrivate unable to attach to shared memory FB");
            }
        }
        if( !m_shmDs.create(sizeof(DisplaySetting)) )
        {
            m_Logger.Log( "AppEngineBasePrivate::AppEngineBasePrivate unable to create shared memory DS, try to attach." );
            if(!m_shmDs.attach())
            {
                m_Logger.Log("AppEngineBasePrivate::AppEngineBasePrivate unable to attach to shared memory DS");
            }
        }
        if( !m_shmDp.create(sizeof(DisplayPlane)) )
        {
            m_Logger.Log( "AppEngineBasePrivate::AppEngineBasePrivate unable to create shared memory DP, try to attach." );
            if(!m_shmDp.attach())
            {
                m_Logger.Log("AppEngineBasePrivate::AppEngineBasePrivate unable to attach to shared memory DP");
            }
        }

        m_Logger.Log( "AppEngineBasePrivate::AppEngineBasePrivate created" );
    }

    /** Destructor */
    virtual ~AppEngineBasePrivate()
    {
      Logger::Destroy();
      if(m_pGPFrontTimer)
        delete m_pGPFrontTimer;
      if(m_pGPRearTimer)
        delete m_pGPRearTimer;
      if(m_pGPCameraTimer)
        delete m_pGPCameraTimer;
      if(m_shm.isAttached())
        m_shm.detach();
      if(m_shmFb.isAttached())
        m_shmFb.detach();
      if( m_shmDs.isAttached())
        m_shmDs.detach();  
      if( m_shmDp.isAttached())
        m_shmDp.detach();  
      //m_Logger.Log( "AppEngineBasePrivate::AppEngineBasePrivate destroyed" );
    }

public:

#ifndef REMOVE_APPTOPWIDGET
  QWidget *m_pMainWindow;
#endif
  AppFrameworkIPC *m_pIPC;
  QWidget *m_pQAppWidget;
  Logger &m_Logger;
  AppEngineBase::APP_ENGINE_TYPE m_nEngineType;
  //StatusAdapter *m_pStatusAdapter;
  bool m_bIsSwapDCEnabled;
  bool m_bTouchPressed;
  bool m_bJogIgnored;
  int m_hwKeyKind;
  bool m_bHwKeyPressed;
  int m_nJogKeyPressed;
  bool m_bTouchLock;
  bool m_bTouchEnable;
  bool m_bHUSeekLock;
  bool m_bRRCSeekLock;
  bool m_bSWRCSeekLock;
  bool m_bHUSeekPress;
  bool m_bCCPSeekPress;
  bool m_bRRCSeekPress;
  bool m_bCameraEnabled;
  int m_cloneMode;
  uint m_DRS;
  QDBusInterface* m_micomManager;

  // FB Utilities
  DualDisplayHelper *m_pDDHelper;
  QSharedMemory m_shmFb;
  QSharedMemory m_shmDs;
  QSharedMemory m_shmDp;

  // Gst Pipelines Utilities
  QTimer *m_pGPFrontTimer;
  QTimer *m_pGPRearTimer;
  QTimer *m_pGPCameraTimer;
  int m_nGPFrontElapsed;
  int m_nGPRearElapsed;
  int m_nGPCameraElapsed;
  int m_nGstOccupiedApp;
  QSharedMemory m_shm;

  // Check FullScreen for Popup
  bool m_bFrontFullScreen;
  bool m_bRearFullScreen;
  
   //Q-Pointer to the AppEngineBase class
  AppEngineBase * const q_ptr;
  Q_DECLARE_PUBLIC(AppEngineBase)
};

// ----------------------------------------------------------------------------
// static argv and argc for later purposes
// ----------------------------------------------------------------------------

char** AppEngineBase::m_pArgv = NULL;
int    AppEngineBase::m_Argc  = 0;
LANGUAGE_T AppEngineBase::m_Language = LANGUAGE_UNKNOWN;
DISPLAY_T  AppEngineBase::m_DisplayType = DISPLAY_NONE;
ECountryVariant AppEngineBase::m_CountryVariant = eCVInvalid;
EVehicleVariants AppEngineBase::m_VehicleVariant = eVVDH;
uint AppEngineBase::m_SystemVariant = 0;

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------

AppEngineBase::AppEngineBase( QWidget *pQAppWidget, QString aModuleName, QObject *pParent, APP_ENGINE_TYPE nType )
:  QObject( pParent ),
   m_bAutoVisibility( true ),
   m_AppHandlesResponseFg( false ),
   m_AppHandlesBlendingFg(false),
//m_pStatusAdapter( NULL ),
   m_bIsDisplayedPopupOnFront( false ),
   m_bIsDisplayedPopupOnRear( false )
{
   d_ptr = new AppEngineBasePrivate(this, pQAppWidget, Logger::Instance(aModuleName), nType);

   QApplication::instance()->installEventFilter(this);
   d_ptr->m_Logger.SetLogType( Logger::LOG_FILE, aModuleName );

//   qInstallMsgHandler( (QtMsgHandler)&Logger::HandleSystemLogs );

   d_ptr->m_Logger.Log( QString("AppEngineBase::AppEngineBase create type ") + QString::number(nType), Logger::PRIORITY_LOW  );

   d_ptr->m_pIPC = new AppFrameworkIPC(this);

   //m_pStatusAdapter = new StatusAdapter( d_ptr->m_Logger, this );
   //connect( m_pStatusAdapter, SIGNAL( notifyUISH( STATUS_CONTROL_EVENT ) ),
   //        this,  SLOT( StatusControlAdapterListener( STATUS_CONTROL_EVENT ) ) );
   //connect( m_pStatusAdapter, SIGNAL( notifyUISH( STATUS_CONTROL_EVENT, DISPLAY_T ) ),
   //        this,  SLOT( StatusControlAdapterListener( STATUS_CONTROL_EVENT, DISPLAY_T ) ) );
}


// ----------------------------------------------------------------------------
// constructor
// ----------------------------------------------------------------------------

AppEngineBase::AppEngineBase( QWidget *pQAppWidget, QObject *pParent,  APP_ENGINE_TYPE nType )
:  QObject( pParent ),
  m_bAutoVisibility( true ),
  m_AppHandlesResponseFg( false ),
  m_AppHandlesBlendingFg(false),
  //m_pStatusAdapter( NULL ),
  m_bIsDisplayedPopupOnFront( false ),
  m_bIsDisplayedPopupOnRear( false )
{
   d_ptr = new AppEngineBasePrivate( this, pQAppWidget, Logger::Instance("APP_NONE"), nType);
   QApplication::instance()->installEventFilter(this);
   d_ptr->m_Logger.SetLogType( Logger::LOG_CONSOLE );

//   qInstallMsgHandler( (QtMsgHandler)&Logger::HandleSystemLogs );

   d_ptr->m_Logger.Log( QString("AppEngineBase::AppEngineBase create type ") + QString::number(nType), Logger::PRIORITY_LOW  );

   d_ptr->m_pIPC = new AppFrameworkIPC(this);
   //m_pStatusAdapter = new StatusAdapter( d_ptr->m_Logger, this );
//   connect( m_pStatusAdapter, SIGNAL( notifyUISH( STATUS_CONTROL_EVENT ) ),
//           this,  SLOT( StatusControlAdapterListener( STATUS_CONTROL_EVENT ) ) );
//   connect( m_pStatusAdapter, SIGNAL( notifyUISH( STATUS_CONTROL_EVENT, DISPLAY_T ) ),
//           this,  SLOT( StatusControlAdapterListener( STATUS_CONTROL_EVENT, DISPLAY_T ) ) );
}

LocalSocketIpcClient* g_pLocalSocket = 0;

// ----------------------------------------------------------------------------
// destructor
// ----------------------------------------------------------------------------
AppEngineBase::~AppEngineBase()
{
    QApplication::instance()->removeEventFilter(this);
    DestroyAppArgs();

    delete g_pLocalSocket;
    //delete m_pStatusAdapter;
    delete d_ptr;
}

//QObject * AppEngineBase::GetUIStateManager() const { return d_ptr->m_pIPC; }
QWidget * AppEngineBase::GetAppView() const { return d_ptr->m_pQAppWidget; }
Logger & AppEngineBase::GetLogger() { return d_ptr->m_Logger; }
Q_INVOKABLE bool AppEngineBase::IsSwapDCEnabled() { return d_ptr->m_bIsSwapDCEnabled; }

Q_INVOKABLE int AppEngineBase::getJogKeyPressed()
{
	return d_ptr->m_nJogKeyPressed;
}

Q_INVOKABLE void AppEngineBase::setJogKeyPressed(int key)
{
	d_ptr->m_nJogKeyPressed |= key; 
	emit changedJogKeyPressed();
}

Q_INVOKABLE int AppEngineBase::getJogKeyReleased()
{
	return d_ptr->m_nJogKeyPressed;
}

Q_INVOKABLE void AppEngineBase::setJogKeyReleased(int key)
{
	d_ptr->m_nJogKeyPressed &= !key; 
	emit changedJogKeyReleased();
}

Q_INVOKABLE bool AppEngineBase::getTouchPressed() 
{ 
	return d_ptr->m_bTouchPressed; 
}

Q_INVOKABLE void AppEngineBase::setTouchPressed(bool bTouchPressed) 
{ 
	d_ptr->m_bTouchPressed = bTouchPressed; 
	emit changedTouchPressed();
}


// ----------------------------------------------------------------------------
// startup connection from App to UISH
// ----------------------------------------------------------------------------

void
AppEngineBase::ConnectToUIStateManager()
{
    int nSkipOffset = 0;

    d_ptr->m_Logger.Log( "---> AppEngineBase::ConnectToUIStateManager" );
    d_ptr->m_Logger.Log( QString("AppEngineBase::AppEngineBase create type ") + QString::number(d_ptr->m_nEngineType)  );


    if( NULL != d_ptr->m_pQAppWidget && GetThisAppId() == APP_NAVIGATION)
    {
      d_ptr->m_pQAppWidget->setAttribute(Qt::WA_PaintOnScreen);
    }


#ifndef REMOVE_APPTOPWIDGET
    if( d_ptr->m_nEngineType != APP_QML_VEXT && GetThisAppId() != APP_STATUS_BAR )
    {
       d_ptr->m_pMainWindow = new AppTopWidget(d_ptr->m_pQAppWidget);

       connect( d_ptr->m_pMainWindow, SIGNAL(SignalDrawFinish(uint)), this, SLOT(SendFGResponse(uint)) );
    }
#endif
    d_ptr->m_pIPC->RegisterService(GetThisAppId());
    d_ptr->m_pIPC->ListenService(APP_UI_MANAGER);

   /** Register apps **/
   switch( m_DisplayType )
   {
      case DISPLAY_REAR:
      {
            // Register app to rear state machine
            d_ptr->m_pIPC->RegisterApp(APP_UI_MANAGER, DISPLAY_REAR, GetThisAppId());
            if( GetThisAppId() == APP_MOBILETV || GetThisAppId() == APP_IDLE || GetThisAppId() == APP_AVP || GetThisAppId() == APP_NAVIGATION || GetThisAppId() == APP_CAMERA || GetThisAppId() == APP_E_MANUAL )
            {
                nSkipOffset = DH_STATUS_BAR;
            }
            if( GetAppView() != NULL )
                GetAppView()->setGeometry(DH_X_REAR, DH_Y_REAR - nSkipOffset , DH_SCREEN_WIDTH, DH_SCREEN_HEIGHT + nSkipOffset);
      }
      break;

      case DISPLAY_FRONT_REAR:
      {
           // Register app to rear state machine
            d_ptr->m_Logger.Log( QString( "AppEngineBase::ConnectToUIStateManager register app on front display" ) );
            d_ptr->m_pIPC->RegisterApp(APP_UI_MANAGER, DISPLAY_REAR, GetThisAppId());
            d_ptr->m_pIPC->RegisterApp(APP_UI_MANAGER, DISPLAY_FRONT, GetThisAppId());
            if( GetThisAppId() == APP_MOBILETV || GetThisAppId() == APP_IDLE || GetThisAppId() == APP_AVP || GetThisAppId() == APP_NAVIGATION || GetThisAppId() == APP_CAMERA || GetThisAppId() == APP_E_MANUAL )
            {
                nSkipOffset = DH_STATUS_BAR;
            }
            if( GetAppView() != NULL )
                GetAppView()->setGeometry(DH_X_FRONT, DH_Y_FRONT - nSkipOffset, DH_SCREEN_WIDTH, DH_SCREEN_HEIGHT + nSkipOffset);
       }
       break;

       case DISPLAY_FRONT:
       default:
       {
            // Register app to front state machine
            d_ptr->m_pIPC->RegisterApp(APP_UI_MANAGER, DISPLAY_FRONT, GetThisAppId());
            if( GetThisAppId() == APP_MOBILETV || GetThisAppId() == APP_IDLE || GetThisAppId() == APP_AVP || GetThisAppId() == APP_NAVIGATION || GetThisAppId() == APP_CAMERA || GetThisAppId() == APP_E_MANUAL )
            {
                nSkipOffset = DH_STATUS_BAR;
            }
            if( GetAppView() != NULL )
                GetAppView()->setGeometry(DH_X_FRONT, DH_Y_FRONT - nSkipOffset, DH_SCREEN_WIDTH, DH_SCREEN_HEIGHT + nSkipOffset);
            }
            break;
   }
   //d_ptr->m_Logger.Log( QString("Connect to front successfully Done ") + QString::number(bResult) );
   d_ptr->m_Logger.Log( "<--- AppEngineBase::ConnectToUIStateManager" );
}


// ----------------------------------------------------------------------------
// notify UISH from App
// ----------------------------------------------------------------------------
bool
AppEngineBase::NotifyUISHCommon( Event &aEvent )
{
   if( NotifyUISH( aEvent, DISPLAY_COMMON ) )
	return true;
   else return false;
}

bool
AppEngineBase::NotifyUISH( Event &aEvent, DISPLAY_T aDisplayType )
{
   GetLogger().Log( QString( "AppEngineBase::NotifyUISH(event=[%1], to=[%2] display=(%3,%4)" ).
                   arg( aEvent.GetEventId() ).arg( aEvent.GetTarget() ).
                   arg( (int)m_DisplayType ).arg((int)aDisplayType), Logger::PRIORITY_LOW );

    bool bNeedAutoVisibilityLater = false;
    if ( (m_AppHandlesResponseFg) && (aEvent.GetEventId() == EVT_RESPONSE_FG) )
    {
#ifndef REMOVE_AUTOVISIBILITY
        if ( APP_VR != GetThisAppId() )
        {
#endif
            if ( (aDisplayType == DISPLAY_FRONT && !IsSwapDCEnabled()) ||
                 (aDisplayType == DISPLAY_REAR && IsSwapDCEnabled()) ||
                 (aDisplayType == DISPLAY_REAR && GetCloneMode() == CLONE_ON_REAR) )
                SendTouchCleanUp();
            HandleAutoVisibility(aDisplayType);
#ifndef REMOVE_AUTOVISIBILITY
        }
        else if(( EVT_RESPONSE_FG == aEvent.GetEventId()) && ( eErrorNotReady != ( ErrorCodes ) aEvent.GetCommand() ) )
        {
            GetLogger().Log( "AppEngineBase::NotifyUISH: Do Show VR app", Logger::PRIORITY_LOW );
            if ( (aDisplayType == DISPLAY_FRONT && !IsSwapDCEnabled()) ||
                 (aDisplayType == DISPLAY_REAR && IsSwapDCEnabled()) ||
                 (aDisplayType == DISPLAY_REAR && GetCloneMode() == CLONE_ON_REAR) )
                SendTouchCleanUp();
            bNeedAutoVisibilityLater = true;
        }
        else
        {
            // GetAppView()->hide();
            GetLogger().Log( "AppEngineBase::NotifyUISH: Do not show VR app", Logger::PRIORITY_LOW );
        }
#endif
    }
//    if( aDisplayType == DISPLAY_AUTO )
//    {
//      aDisplayType = m_DisplayType;
//    }

    if(aEvent.GetEventId() == EVT_RESPONSE_FG)
    {
         if(m_AppHandlesBlendingFg == true)
        {
            aEvent.SetCommand(APP_BLENDING_ON);
        }else if (m_AppHandlesBlendingFg == false){
            aEvent.SetCommand(APP_BLENDING_OFF);
        }
    }

	if ( aEvent.GetEventId() == EVT_REQUEST_ABNORMAL_TERMINATION )
	{
		GetLogger().Log( "AppEngineBase::NotifyUISH: Request abnormal termination", Logger::PRIORITY_LOW );
		d_ptr->m_pIPC->DisconnectService(APP_UI_MANAGER);
		d_ptr->m_pIPC->UnregisterService(GetThisAppId());
	}

    int targetAppId = APP_UI_MANAGER;
    if(aEvent.GetEventId() == EVT_POST_EVENT)
        targetAppId = aEvent.GetTarget();
    bool bRet = false;
    switch ( aDisplayType )
    {
        case DISPLAY_FRONT:
            d_ptr->m_pIPC->SendEvent(targetAppId, aEvent, DISPLAY_FRONT);
            bRet = true;
            break;
        case DISPLAY_REAR:
            d_ptr->m_pIPC->SendEvent(targetAppId, aEvent, DISPLAY_REAR);
            bRet = true;
            break;
//        case DISPLAY_FRONT_REAR:
//            return QMetaObject::invokeMethod( d_ptr->m_pUIStateManager, UISH_EVENT_HANDLER_REAR, Q_ARG( Event, aEvent ) ) &&
//                   QMetaObject::invokeMethod( d_ptr->m_pUIStateManager, UISH_EVENT_HANDLER_FRONT, Q_ARG( Event, aEvent ) );
	case DISPLAY_COMMON:
            d_ptr->m_pIPC->SendEvent(targetAppId, aEvent, DISPLAY_COMMON);
            bRet = true;
            break;
        default:
            GetLogger().Log( "AppEngineBase::NotifyUISH: Error: Unknown UISH handler!", Logger::PRIORITY_LOW );
            bRet = false;
            break;
    }
    if( bNeedAutoVisibilityLater ) {
        usleep(20000);
        HandleAutoVisibility(aDisplayType);
    }
    return bRet;
}

// ----------------------------------------------------------------------------
// base notifier from UISH
// ----------------------------------------------------------------------------

 void
AppEngineBase::NotifyFromUIStateManagerBase( Event aEvent, uint aScreenId )
{
    GetLogger().Log( QString("AppEngineBase::NotifyFromUIStateManagerBase(evt=[%1] from=[%2] to=[%3] screen=[%4] engine=[%5] clone=[%6] swap=[%7])").
        arg(aEvent.GetEventId()).
        arg(aEvent.GetSource()).arg(aEvent.GetTarget()).
        arg(aScreenId).arg((int)d_ptr->m_nEngineType).
        arg(GetCloneMode()).arg(IsSwapDCEnabled()),
        Logger::PRIORITY_LOW);
    /*
    GetLogger().Log( QString("AppEngineBase::VEXT-1 NotifyFromUIStateManagerBase EVENT [%1]").arg(aEvent.GetEventId() ) );
    GetLogger().Log( QString("AppEngineBase::VEXT-2 NotifyFromUIStateManagerBase [%1][%2]").arg(GetThisAppId()).arg(aEvent.GetTarget()));
    GetLogger().Log( QString("AppEngineBase::VEXT-3 NotifyFromUIStateManagerBase SCREEN  [%1]").arg(aScreenId));
    */

    if ( aEvent.GetTarget() != GetThisAppId() && aEvent.GetTarget() != APP_ALL )
    {
        return;
    }

    GetLogger().Log( "AppEngineBase::NotifyFromUIStateManagerBase 1" );


    if(aEvent.GetEventId() == EVT_TOUCH_ENABLE)
        d_ptr->m_bTouchEnable = true;

    if(aEvent.GetEventId() == EVT_KEY_HU_SEEK_NEXT || aEvent.GetEventId() == EVT_KEY_HU_SEEK_PREV)
    {
        if(!IsSeekValid(aEvent , EVT_KEY_HU_SEEK_NEXT, EVT_KEY_HU_SEEK_PREV))
        {
            GetLogger().Log( QString("AppEngineBase::HU SEEK LOCKING, RRC SEEK RETURN"), Logger::PRIORITY_LOW );
            return;
        }
    }
    if(aEvent.GetEventId() == EVT_KEY_RRC_SEEK_NEXT || aEvent.GetEventId() == EVT_KEY_RRC_SEEK_PREV)
    {
        if(!IsSeekValid(aEvent , EVT_KEY_RRC_SEEK_NEXT, EVT_KEY_RRC_SEEK_PREV))
        {
            GetLogger().Log( QString("AppEngineBase::RRC SEEK LOCKING, RRC SEEK RETURN"), Logger::PRIORITY_LOW );
            return;
        }
    }
    if(aEvent.GetEventId() == EVT_KEY_CCP_SEEK_NEXT || aEvent.GetEventId() == EVT_KEY_CCP_SEEK_PREV)
    {
        if(!IsSeekValid(aEvent , EVT_KEY_CCP_SEEK_NEXT, EVT_KEY_CCP_SEEK_PREV))
        {
            GetLogger().Log( QString("AppEngineBase::CCP SEEK LOCKING, RRC SEEK RETURN"), Logger::PRIORITY_LOW );
            return;
        }
    }
    if(aEvent.GetEventId() == EVT_KEY_SWRC_SEEK_NEXT || aEvent.GetEventId() == EVT_KEY_SWRC_SEEK_PREV)
    {
        if(!IsSeekValid(aEvent , EVT_KEY_SWRC_SEEK_NEXT, EVT_KEY_SWRC_SEEK_PREV))
        {
            GetLogger().Log( QString("AppEngineBase::SWRC SEEK LOCKING, SWRC SEEK RETURN"), Logger::PRIORITY_LOW );
            return;
        }

    }


    if( EVT_AV_MODE_CHANGE == aEvent.GetEventId())
    {
        EVENT_AUDIO_PATH_REQUEST_DATA* pAudioPathEvent;
        pAudioPathEvent = (EVENT_AUDIO_PATH_REQUEST_DATA *)aEvent.Data().data();

        if(pAudioPathEvent->avState == MODE_STATE_NONE)
        {
            SetHUSeekLock(false);
            SetRRCSeekLock(false);
            SetSWRCSeekLock(false);
        }

    }

    if ( EVT_REQUEST_FG == aEvent.GetEventId() )
    {
          SetTouchLock(false);
          if ( aEvent.Data().isEmpty() )
          {
              GetLogger().Log( "AppEngineBase::NotifyFromUIStateManagerBase NO FG DATA" );
          }

          Event event( EVT_REQUEST_PRE_FG,
                       aEvent.GetSource(),
                       aEvent.GetTarget(),
                       QByteArray::fromRawData( aEvent.Data().data(), aEvent.Data().size() ) );

#ifdef REMOVE_AUTOVISIBILITY
		if( true )
#else
		if( m_bAutoVisibility )
#endif
		{
			if(aScreenId == (uint)DISPLAY_FRONT)
			{
				HandleNotifyFromUIStateManager( (DISPLAY_T)aScreenId, event );
			}
			else if( aScreenId == (uint)DISPLAY_REAR )
			{
				HandleNotifyFromUIStateManagerRear( (DISPLAY_T)aScreenId, event );
			}
		}
	}


    // Lets handle common both QT/QML based applications
    switch ( aEvent.GetEventId() )
    {
//        case EVT_STATUS_BAR_UPDATE:
//        {
//            GetLogger().Log( "AppEngineBase::NotifyFromUIStateManagerBase:Processing Event::EVT_STATUS_BAR_UPDATE" );
//            int nParam = 0;
//            QByteArray arrayParam;

//            if (!aEvent.Data().isEmpty())
//            {
//               nParam = ((int*)( aEvent.Data().data()))[0];
//               arrayParam = aEvent.Data().mid( sizeof( nParam ));
//            }
            //m_pStatusAdapter->ParseSignalFromAppFramework( aEvent.GetCommand(), nParam, arrayParam );
//            break;
//        }

        case EVT_SWAP_DC_ENABLED:
        {
            GetLogger().Log( "AppEngineBase::NotifyFromUIStateManagerBase: Swap DC is enabled " );
            d_ptr->m_bIsSwapDCEnabled = true;
            break;
        }

        case EVT_SWAP_DC_DISABLED:
        {
            GetLogger().Log( "AppEngineBase::NotifyFromUIStateManagerBase: Swap DC is disabled " );
            d_ptr->m_bIsSwapDCEnabled = false;
            break;
        }

		case EVT_SHOW_POPUP:
		{
			if(aScreenId == DISPLAY_FRONT && d_ptr->m_bIsSwapDCEnabled == false)
				SendTouchCleanUp();
			if(aScreenId == DISPLAY_REAR && d_ptr->m_bIsSwapDCEnabled == true)
				SendTouchCleanUp();
			break;
    	}
		
        default:
        {
            break;
        }
    }

    CheckDRS(aEvent, aScreenId);
    if( IgnoreKeyWhenTouched(aEvent, (DISPLAY_T)aScreenId) )
        return;

    // Lets handle common behavior for specific QT/QML based applications
    GetLogger().Log( "AppEngineBase::NotifyFromUIStateManagerBase:Processing Event::send to inheriters" );
    if( IsJogEvent( aEvent.GetEventId() ) )
    {
	   	if( GetIsDisplayedPopup((DISPLAY_T)aScreenId) )
	    {
	        if( (DISPLAY_T)aScreenId == DISPLAY_FRONT)
	            HandleEvent( aEvent );
	        else if ((DISPLAY_T)aScreenId == DISPLAY_REAR)
	            HandleEventRear( aEvent );
	        else 
				HandleEvent( aEvent );
	    	return;
	    }
	    else
	    {
			if( (DISPLAY_T)aScreenId == DISPLAY_FRONT )
	            HandleEvent( aEvent );
	        else 
				HandleEventRear( aEvent );

			HandleNotifyFromUIStateManagerCommon( (DISPLAY_T)aScreenId, aEvent );
        }
    }
    else if( d_ptr->m_nEngineType == APP_QML_VEXT )
    {
        if( aScreenId == (uint)DISPLAY_REAR ||
            aScreenId == (uint)DISPLAY_FRONT_REAR ||
            ( aScreenId == (uint)DISPLAY_AUTO && ( m_DisplayType == DISPLAY_REAR || m_DisplayType == DISPLAY_FRONT_REAR) ) )
        {
           if( IsIgnorableKeyWhenSystemPopup(DISPLAY_REAR, aEvent.GetEventId()) )
           {
               GetLogger().Log( QString("AppEngineBase::ignoring MENU key because system popup is displayed 1" ), Logger::PRIORITY_LOW);
               return;
           }
           GetLogger().Log( QString("AppEngineBase::Calling HandleEventRear" ));
        /*
	           if( GetThisAppId() == APP_AVP )
	           {
	              HandleNotifyFromUIStateManagerRear( (DISPLAY_T)aScreenId, aEvent );
	              HandleEventRear( aEvent );
	           }
	           else
           */
           {
              HandleEventRear( aEvent );
              HandleNotifyFromUIStateManagerRear( (DISPLAY_T)aScreenId, aEvent );
           }
        }
        if( aScreenId == (uint)DISPLAY_FRONT ||
            aScreenId == (uint)DISPLAY_FRONT_REAR ||
            ( aScreenId == (uint)DISPLAY_AUTO && ( m_DisplayType == DISPLAY_FRONT || m_DisplayType == DISPLAY_FRONT_REAR) ) )
        {
            if( IsIgnorableKeyWhenSystemPopup(DISPLAY_FRONT, aEvent.GetEventId()) )
            {
                GetLogger().Log( QString("AppEngineBase::ignoring MENU key because system popup is displayed 2" ), Logger::PRIORITY_LOW);
                return;
            }
           GetLogger().Log( QString("AppEngineBase::Calling HandleEvent" ));
           /*
           if( GetThisAppId() == APP_AVP )
           {
              HandleNotifyFromUIStateManager( (DISPLAY_T)aScreenId, aEvent );
              HandleEvent( aEvent );
           }
           else
           */
           {
              HandleEvent( aEvent );
              HandleNotifyFromUIStateManager( (DISPLAY_T)aScreenId, aEvent );
           }
        }
		if( aScreenId == (uint)DISPLAY_COMMON )
		{
			DISPLAY_T display = DISPLAY_FRONT;
			EVT_ID_T nEventID = aEvent.GetEventId();

			display = aEvent.GetEventId() == EVT_KEY_RRC_MENU ? DISPLAY_REAR : DISPLAY_FRONT;
			if ( d_ptr->m_cloneMode == CLONE_ON_FRONT )
				display = DISPLAY_FRONT;
			else if (d_ptr->m_cloneMode == CLONE_ON_REAR )
				display = DISPLAY_REAR;
			
            if( IsIgnorableKeyWhenSystemPopup(display, aEvent.GetEventId()))
            {
                GetLogger().Log( QString("AppEngineBase::ignoring MENU key because system popup is displayed 3" ), Logger::PRIORITY_LOW);
                return;
            }
            HandleEvent( aEvent );
            HandleNotifyFromUIStateManagerCommon( (DISPLAY_T)aScreenId, aEvent );
		}
    }
    else
    {
       if( IsIgnorableKeyWhenSystemPopup(DISPLAY_FRONT_REAR, aEvent.GetEventId()) )
       {
            GetLogger().Log( QString("AppEngineBase::ignoring MENU key because system popup is displayed" ), Logger::PRIORITY_LOW);
            return;
       }
       GetLogger().Log( "AppEngineBase::NotifyFromUIStateManagerBase:Processing Event::non Vext qml" );
       if( aScreenId == (uint)DISPLAY_REAR ||
           ( aScreenId == (uint)DISPLAY_AUTO && m_DisplayType == DISPLAY_REAR ) )
       {
          GetLogger().Log( QString("AppEngineBase::Calling HandleEventRear" ));
          HandleEventRear( aEvent );
          HandleNotifyFromUIStateManagerRear( (DISPLAY_T)aScreenId, aEvent );
       }
       if( aScreenId == (uint)DISPLAY_FRONT ||
           ( aScreenId == (uint)DISPLAY_AUTO && m_DisplayType == DISPLAY_FRONT ) )
       {
          GetLogger().Log( QString("AppEngineBase::Calling HandleEvent" ));
          HandleEvent( aEvent );
          HandleNotifyFromUIStateManager( (DISPLAY_T)aScreenId, aEvent );
       }
       if( aScreenId == (uint)DISPLAY_COMMON )
       {
           HandleEvent( aEvent );
           HandleNotifyFromUIStateManagerCommon( (DISPLAY_T)aScreenId, aEvent );		
       }
#ifndef REMOVE_APPTOPWIDGET	   
       if ( aEvent.GetEventId() == EVT_REQUEST_FG && m_AppHandlesResponseFg == false && (d_ptr->m_pMainWindow) != NULL )
       {
          ((AppTopWidget*)(d_ptr->m_pMainWindow))->startEventHandling((DISPLAY_T)aScreenId);
       }
#endif
    }

    if ( aEvent.GetEventId() == EVT_REQUEST_APP_EXIT )
    {
        GetLogger().Log( "AppEngineBase::NotifyFromUIStateManagerBase:Processing Event::EVT_REQUEST_APP_EXIT" );
        if( d_ptr->m_shm.isAttached() ) {
            GetLogger().Log("(afw) detatch shared memory before application quits");
            d_ptr->m_shm.detach();
        }
        if( d_ptr->m_shmFb.isAttached() ) {
            GetLogger().Log("(afw) detatch shared memory before application quits FB");
            d_ptr->m_shmFb.detach();
        }
        if( d_ptr->m_shmDs.isAttached() ) {
            GetLogger().Log("(afw) detatch shared memory before application quits DS");
            d_ptr->m_shmDs.detach();
        }
        if( d_ptr->m_shmDp.isAttached() ) {
            GetLogger().Log("(afw) detatch shared memory before application quits DP");
            d_ptr->m_shmDp.detach();
        }
        // killing the application
        qApp->quit();
    }
}

void
AppEngineBase::HandleNotifyFromUIStateManager( DISPLAY_T nScreenID, Event aEvent )
{
   GetLogger().Log( QString("AppEngineBase::HandleNotifyFromUIStateManager(%1,evt) Calling NotifyFromUIStateManager").arg(nScreenID) );

   NotifyFromUIStateManager( aEvent );
}

void
AppEngineBase::HandleNotifyFromUIStateManagerRear( DISPLAY_T nScreenID, Event aEvent )
{
   GetLogger().Log( QString("AppEngineBase::HandleNotifyFromUIStateManagerRear(%1, evt) Calling NotifyFromUIStateManagerRear").arg(nScreenID) );

   NotifyFromUIStateManagerRear( aEvent );
}

void
AppEngineBase::HandleNotifyFromUIStateManagerCommon( DISPLAY_T nScreenID, Event aEvent )
{
   GetLogger().Log( QString("AppEngineBase::HandleNotifyFromUIStateManagerCommon(%1, evt) Calling NotifyFromUIStateManagerCommon").arg(nScreenID) );

   NotifyFromUIStateManagerCommon( aEvent );
}


bool
AppEngineBase::IsJogEvent( EVT_ID_T nEventID )
{
   if( (nEventID >= EVT_KEY_CCP_JOG_DIAL_TOP_LEFT && nEventID <= EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT)
      || nEventID == EVT_KEY_CCP_JOG_DIAL_ENCODER || nEventID == EVT_KEY_RRC_JOG_DIAL_ENCODER )
   {
      return true;
   }
   else
   {
      return false;
   }
}

bool
AppEngineBase::IsCCPEvent( EVT_ID_T nEventID )
{
   if( (nEventID >= EVT_KEY_CCP_JOG_DIAL_TOP_LEFT && nEventID <= EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT)
      || nEventID == EVT_KEY_CCP_JOG_DIAL_ENCODER )
      return true;
   else return false; 
}

bool
AppEngineBase::IsRRCEvent( EVT_ID_T nEventID )
{
   if( (nEventID >= EVT_KEY_RRC_JOG_DIAL_TOP_LEFT && nEventID <= EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT)
      || nEventID == EVT_KEY_RRC_JOG_DIAL_ENCODER )
      return true;
   else return false; 
}

void
AppEngineBase::SendFGResponse( uint nScreen )
{
   Logger::Instance()->Log( QString("AppEngineBase::SendFGResponse(from=[%1], display(%2,%3))").arg(GetThisAppId()).arg(m_DisplayType).arg(nScreen));

   Event event( EVT_RESPONSE_FG, GetThisAppId(), APP_UI_MANAGER );
   if ( ( nScreen == DISPLAY_AUTO && m_DisplayType == DISPLAY_REAR ) ||
          nScreen == DISPLAY_REAR )
   {
      Logger::Instance()->Log( "AppEngineBase::SendFGResponse:Processing Event::EVT_REQUEST_FG rear" );
      // Acknowledge to rear state machine
      d_ptr->m_pIPC->SendEvent(APP_UI_MANAGER, event, DISPLAY_REAR);
   }
   else if( ( nScreen == DISPLAY_AUTO && m_DisplayType == DISPLAY_FRONT ) ||
              nScreen == DISPLAY_FRONT )
   {
        Logger::Instance()->Log( "AppEngineBase::SendFGResponse:Processing Event::EVT_REQUEST_FG front" );
        // Acknowledge to front state machine
        d_ptr->m_pIPC->SendEvent(APP_UI_MANAGER, event, DISPLAY_FRONT);
   }
}

// ----------------------------------------------------------------------------
// parse the arguments and get display out
// ----------------------------------------------------------------------------

void
AppEngineBase::ParseAppArguments( int &argc, char** argv )
{
    Logger::Instance()->Log( "---> AppEngineBase::ParseAppArguments" );
    Logger::Instance()->Log( QString( "# of arguments: [%1]" ).arg( argc ) );

    m_DisplayType = DISPLAY_FRONT;

    for ( int index = 0; index < argc; ++index )
    {
        Logger::Instance()->Log(QString(argv[index]));
        if ( std::string( argv[index] ) == std::string( "-display" ) )
        {
            if ( ( index+1 ) < argc )
            {
                if ( std::string( argv[index+1] ) == std::string( ":0.1" ) )
                {
                    Logger::Instance()->Log( "display set to DISPLAY_REAR" );
                    m_DisplayType = DISPLAY_REAR;
                }
            }
        }
        else if ( std::string( argv[index] ) == std::string( "-vext_display" ) )
        {
            if ( ( index+1 ) < argc )
            {
               if ( std::string( argv[index+1] ) == std::string( ":0.1" ) )
               {
                   m_DisplayType = DISPLAY_REAR;
               }
               else if ( std::string( argv[index+1] ) == std::string( ":0.2" ) )
               {
                   m_DisplayType = DISPLAY_FRONT_REAR;
               }
               Logger::Instance()->Log( QString("display set to ") + QString::number(m_DisplayType) );
            }
        }
        else if ( strstr( argv[index], "-l:" ) != NULL ) // Language
        {
           QStringList langParams = QString( argv[ index ] ).split( ":" );

           if ( langParams.count() == 2 )
           {
              m_Language = (LANGUAGE_T)langParams.at( 1 ).toInt();
           }
        }
        else if ( strstr( argv[index], "-cv:" ) != NULL ) // Country Variant
        {
           QStringList cvParams = QString( argv[ index ] ).split( ":" );

           if ( cvParams.count() == 2 )
           {
              m_CountryVariant = (ECountryVariant)cvParams.at( 1 ).toInt();
           }
        }
        else if ( strstr( argv[index], "-vv:" ) != NULL ) // Vehicle Variant
        {
           QStringList vvParams = QString( argv[ index ] ).split( ":" );

           if ( vvParams.count() == 2 )
           {
              m_VehicleVariant = (EVehicleVariants)vvParams.at( 1 ).toInt();
           }
        }
        else if ( strstr( argv[index], "-sv:" ) != NULL ) // System Variant
        {
           QStringList cvParams = QString( argv[ index ] ).split( ":" );

           if ( cvParams.count() == 2 )
           {
              m_SystemVariant = cvParams.at( 1 ).toUInt();
           }
        }
    }

    Logger::Instance()->Log( "<--- AppEngineBase::ParseAppArguments" );
}

// ----------------------------------------------------------------------------
// Set program arguments to AFW, store for later purposes
// ----------------------------------------------------------------------------

void
AppEngineBase::SetAppArgs( int aArgc, char **pArgv )
{
   if ( !pArgv ) return;

      DestroyAppArgs();

   ParseAppArguments( aArgc, pArgv );

   m_Argc  = aArgc;
   m_pArgv = (char **) malloc( (m_Argc + 1) * sizeof( char * ) );

   for ( int index = 0; index < m_Argc; index++ )
   {
      m_pArgv[ index ] = (char *)malloc( ( strlen( pArgv[ index ] ) + 1 ) * sizeof( char ) );
      if(m_pArgv[ index ] != NULL)
      {
         strncpy( m_pArgv[ index ], pArgv[ index ], strlen( pArgv[ index ] ) + 1 );
      }
   }

   m_pArgv[ m_Argc ] = NULL;
}

// ----------------------------------------------------------------------------
// !!!DEPRECATED Old connector to UISH
// ----------------------------------------------------------------------------

void
AppEngineBase::ConnectToUISH( int& argc, char** argv )
{
    Logger::Instance()->Log("AppEngineBase::ConnectToUISH");
    if ( !m_pArgv )
    {
    ParseAppArguments( argc, argv );
    }
    ConnectToUIStateManager();
}

// ----------------------------------------------------------------------------
// destroy the public and static program args
// ----------------------------------------------------------------------------

void
AppEngineBase::DestroyAppArgs()
{
   if ( m_Argc > 0 && m_pArgv )
   {
      int index = 0;

      for ( index = 0; index <  m_Argc; index++ )
      {
         free( m_pArgv[ index ] );
      }

      free( m_pArgv );
      m_pArgv = NULL;
      m_Argc = 0;
   }
}

// ----------------------------------------------------------------------------
// provide read parameter from app start to AFW
// ----------------------------------------------------------------------------
void
AppEngineBase::ConnectToUISH()
{
   ConnectToUIStateManager();
}

void
AppEngineBase::Reserved_1()
{
}

void
AppEngineBase::Reserved_2()
{
}

void
AppEngineBase::NotifyFromUIStateManagerRear( Event aEvent )
{
    Q_UNUSED(aEvent);
    d_ptr->m_Logger.Log( "AppEngineBase::NotifyFromUIStateManagerRear");
}

void
AppEngineBase::SetAutoScreenVisibility( bool state )
{
    d_ptr->m_Logger.Log( QString("setAutoScreenVisibility: [%1]" ).arg( state ) );
    m_bAutoVisibility = state;
}

void
AppEngineBase::SetTouchLock( bool state )
{
    if(d_ptr->m_bTouchLock == state)
        return;
    if(GetRealAppId(DISPLAY_FRONT) == GetThisAppId() || (GetCloneMode() == true && GetRealAppId(DISPLAY_REAR) == GetThisAppId()))
    {
        d_ptr->m_Logger.Log( QString("SetTouchLock: [%1]" ).arg( state ), Logger::PRIORITY_LOW );
        d_ptr->m_bTouchLock = state;
    }
}

void
AppEngineBase::SetHUSeekLock( bool state )
{
    d_ptr->m_Logger.Log( QString("SetHUSeekLock: [%1]" ).arg( state ), Logger::PRIORITY_LOW );
    d_ptr->m_bHUSeekLock = state;
}

void
AppEngineBase::SetRRCSeekLock( bool state )
{
    d_ptr->m_Logger.Log( QString("SetRRCSeekLock: [%1]" ).arg( state ), Logger::PRIORITY_LOW );
    d_ptr->m_bRRCSeekLock = state;
}

void
AppEngineBase::SetSWRCSeekLock( bool state )
{
    d_ptr->m_Logger.Log( QString("SetSWRCSeekLock: [%1]" ).arg( state ), Logger::PRIORITY_LOW );
    d_ptr->m_bSWRCSeekLock = state;
}

bool
AppEngineBase::IsSeekValid(Event aEvent, EVT_ID_T typeA, EVT_ID_T typeB)
{
    if(aEvent.GetEventId() == typeA || aEvent.GetEventId() == typeB)
   {
       if(d_ptr->m_bHUSeekLock == true)
       {
           // GetLogger().Log( QString("AppEngineBase::HU SEEK LOCKING, HU SEEK RETURN"), Logger::PRIORITY_LOW );
            return false;
       }
       else
       {
           KEY_EVENT_DATA_T *Keymsg;
           Keymsg = (KEY_EVENT_DATA_T *)aEvent.Data().data();
           if(Keymsg->keyStatus == KEY_STATUS_PRESSED)
               d_ptr->m_bHUSeekPress = true;
           else if(Keymsg->keyStatus == KEY_STATUS_CANCEL)
               d_ptr->m_bHUSeekPress = false;
           else if(Keymsg->keyStatus != KEY_STATUS_PRESSED && d_ptr->m_bHUSeekPress == false)
           {
               if(Keymsg->keyStatus == KEY_STATUS_RELEASED)
                    d_ptr->m_bHUSeekPress = false;
               return false;
           }
       }
    }
    return true;
}

//void
//AppEngineBase::StatusControlAdapterListener( STATUS_CONTROL_EVENT sEvent, DISPLAY_T nDisplay )
//{
//   GetLogger().Log(QString(" AppEngineBase::StatusControlAdapterListener() [%1] ").arg( sEvent.EVTid ) );
//   QByteArray array;
//   array.append( ( ( char* )( &sEvent.nParam ) ), sizeof(sEvent.nParam) );
//   array.append( sEvent.arrayParam );
//   Event event( EVT_STATUS_BAR_UPDATE, GetThisAppId(), APP_STATUS_BAR, sEvent.EVTid, array);
//   NotifyUISH( event, nDisplay );
//}

//void
//AppEngineBase::StatusControlAdapterListener( STATUS_CONTROL_EVENT sEvent )
//{
//   StatusControlAdapterListener( sEvent, DISPLAY_AUTO );
//}

void AppEngineBase::SetGeometry( int x, int y, int width, int height )
{
   d_ptr->m_pQAppWidget->setGeometry( x, y, width, height );
}

bool
AppEngineBase::eventFilter( QObject *pFromObj, QEvent *pEvent )
{
   if ( pEvent->type() == QEvent::MouseButtonPress) {
       if(d_ptr->m_bTouchEnable == false && GetThisAppId() == APP_STANDBY)
       {
           SendTouchCleanUp();
           return false;
       }
       if(d_ptr->m_bTouchLock == true && GetRealAppId(DISPLAY_FRONT) == GetThisAppId())
       {
               SendTouchCleanUp();
               return false;
       }
       if( !d_ptr->m_bHwKeyPressed)
       {
        GetLogger().Log("key canceling - started because touch is pressed");
        d_ptr->m_bTouchPressed = true;
       }
   }
   else if ( pEvent->type() == QEvent::MouseButtonRelease ) {
       if(d_ptr->m_bTouchLock == true)
       {
               SendTouchCleanUp();
               return false;
       }
       GetLogger().Log("key canceling - stopped because touch is released");
       d_ptr->m_bTouchPressed = false;
   }
   else if ( pEvent->type() == ( QEvent::User - 15 ) )
   {
      APP_ID_T curAppId = GetThisAppId();

      // Some LGE applications already handling BEEP and not requested support with it, so
      // we need to disable them from system BEEP design to avoid of double beeping
      if ( curAppId != APP_RADIO && curAppId != APP_NAVIGATION && curAppId != APP_TV && curAppId != APP_MOBILETV &&
           /*curAppId != APP_BTPHONE && */curAppId != APP_VR && curAppId != APP_IDLE &&
           curAppId != APP_DAB && curAppId != APP_XM_AUDIO && curAppId != APP_XM_DATA )
      {
		APP_ID_T frontGuiAppId ;
		if ( IsSwapDCEnabled() )
			frontGuiAppId = (APP_ID_T)GetRearGuiApp();
		else
			frontGuiAppId = (APP_ID_T)GetFrontGuiApp();
		GetLogger().Log( QString("AppEngineBase::eventFilter: Recieved BEEP event, forwarding to UISH for playing before App[%1] ThisAppID[%2]").arg(frontGuiAppId).arg(curAppId) );
		if ( frontGuiAppId == GetThisAppId() )
     	{
         GetLogger().Log( "AppEngineBase::eventFilter: Recieved BEEP event, forwarding to UISH for playing..." );
         pEvent->accept();
         PlayBeep();
      	}
         return true;
      }
   }

   return QObject::eventFilter( pFromObj, pEvent );
}

void AppEngineBase::StatusBarPostEventSend( int nCMD, int nDispType )
{
   GetLogger().Log( QString( " AppEngineBase::StatusBarPostEventSend() nCMD = [%1] nDispType = [%2]" ).arg( nCMD ).arg( nDispType ) );
   Event event( EVT_POST_EVENT, GetThisAppId(), APP_STATUS_BAR, nCMD );
   NotifyUISH( event, ( DISPLAY_T )nDispType );
}

void AppEngineBase::SendTouchCleanUpForApps()
{
	APP_ID_T frontGuiAppId;
	frontGuiAppId =(APP_ID_T)GetRealAppId(DISPLAY_FRONT);

	if ( frontGuiAppId != GetThisAppId() )
        return;

    GetLogger().Log(QString("AppEngineBase::SendTouchCleanUpFor %1 App").arg(GetThisAppId()), Logger::PRIORITY_LOW);

    if( d_ptr->m_micomManager == NULL || !d_ptr->m_micomManager->isValid() ) {
        d_ptr->m_micomManager = new QDBusInterface("com.lge.car.micom", "/Dtc", "com.lge.car.micom.dtc", QDBusConnection::sessionBus(), this);
        if(d_ptr->m_micomManager == NULL || !d_ptr->m_micomManager->isValid()) {
            Logger::Instance()->Log(QString("invalid interface : %1").arg(QDBusConnection::sessionBus().lastError().message()));
            return;
        }
    }
    if( d_ptr->m_micomManager->isValid()) {
        d_ptr->m_micomManager->asyncCall(QLatin1String("LgeDBus_TouchCleanup"));
    }
}

//void AppEngineBase::StatusBarClearOSD( int nDispType )
//{
//   GetLogger().Log( QString( " AppEngineBase::StatusBarClearOSD() nDispType = [%1]" ).arg( nDispType ) );
//
//   STATUS_CONTROL_EVENT event;
//   event.EVTid =(int) EVT_ID_CLEAR_OSD_INFO;
//   event.nParam = 0;
//
//   StatusControlAdapterListener( event, (DISPLAY_T)nDispType );
//}


bool AppEngineBase::GetIsDisplayedPopup(DISPLAY_T nDispType)
{
    if( nDispType == DISPLAY_FRONT_REAR )
        return (m_bIsDisplayedPopupOnFront & m_bIsDisplayedPopupOnRear);
    else if( nDispType == DISPLAY_FRONT )
        return m_bIsDisplayedPopupOnFront;
    else if( nDispType == DISPLAY_REAR )
        return m_bIsDisplayedPopupOnRear;

    return false;
}

void AppEngineBase::SetIsDisplayedPopup(DISPLAY_T nDispType, bool bMode)
{
    if( nDispType == DISPLAY_FRONT_REAR )
    {
        m_bIsDisplayedPopupOnFront = bMode;
        m_bIsDisplayedPopupOnRear  = bMode;
    }
    else if( nDispType == DISPLAY_FRONT )
        m_bIsDisplayedPopupOnFront = bMode;
    else if( nDispType == DISPLAY_REAR )
        m_bIsDisplayedPopupOnRear  = bMode;
}

void AppEngineBase::PlayBeep(uint beepType)
{
    GetLogger().Log( "AppEngineBase::PlayBeep" );
    QDBusMessage msg;
    QList<QVariant> list;
    TAudioBeepType tBeepType = (TAudioBeepType)beepType;

    msg = QDBusMessage::createMethodCall( "com.lge.car.micom", "/","com.lge.car.micom.audioif", "ProcessAudioControlMessage" );
    GetLogger().Log( QString( "AppEngineBase::PlayBeep - Beep Type = %1" ).arg(beepType) );

    list.append(QVariant(c_CntlAudioBeepCommand));
    list.append(QVariant(tBeepType));
    msg.setArguments(list);

    QDBusConnection::sessionBus().call( msg, QDBus::NoBlock, -1 );

    return;
}

void AppEngineBase::NotifyWhenGstPipelinesAvailable(int nGpType)
{
  GetLogger().Log( QString( "AppEngineBase::NotifyWhenGstPipelinesAvailable()"), Logger::PRIORITY_LOW );
  
  if (GetGstPipelinesStatus(nGpType) && d_ptr->m_nGstOccupiedApp != (int)GetThisAppId())
  {
    if(nGpType == GP_DISPLAY_FRONT)
    {
      if(d_ptr->m_pGPFrontTimer == NULL) {
        d_ptr->m_pGPFrontTimer = new QTimer;
        connect(d_ptr->m_pGPFrontTimer, SIGNAL(timeout()), this, SLOT(CheckFrontGstPipelines()));
      }
      d_ptr->m_nGPFrontElapsed = 0;
      d_ptr->m_pGPFrontTimer->setInterval(GP_CHECK_INTERVAL);
      d_ptr->m_pGPFrontTimer->start();
    }
    else if(nGpType == GP_DISPLAY_REAR)
    {
      if(d_ptr->m_pGPRearTimer == NULL) {
        d_ptr->m_pGPRearTimer = new QTimer;
        connect(d_ptr->m_pGPRearTimer, SIGNAL(timeout()), this, SLOT(CheckRearGstPipelines()));
      }
      d_ptr->m_nGPRearElapsed = 0;
      d_ptr->m_pGPRearTimer->setInterval(GP_CHECK_INTERVAL);
      d_ptr->m_pGPRearTimer->start();
    }
    else if(nGpType == GP_DISPLAY_CAMERA)
    {
      if(d_ptr->m_pGPCameraTimer == NULL) {
        d_ptr->m_pGPCameraTimer = new QTimer;
        connect(d_ptr->m_pGPCameraTimer, SIGNAL(timeout()), this, SLOT(CheckCameraGstPipelines()));
      }
      d_ptr->m_nGPCameraElapsed = 0;
      d_ptr->m_pGPCameraTimer->setInterval(GP_CHECK_INTERVAL);
      d_ptr->m_pGPCameraTimer->start();
    }
  }
  else
  {
    SetGstPipelinesStatusInternal(nGpType, true);
    HandleGstPipelinesAvailable(nGpType);
  }
}

void AppEngineBase::CheckFrontGstPipelines()
{
  GetLogger().Log( QString( "AppEngineBase::CheckFrontGstPipelines()") );
  if( d_ptr->m_pGPFrontTimer != NULL)
  {
    if (GetGstPipelinesStatus(GP_DISPLAY_FRONT))
    {
#ifdef AFW_FORCE_RENDER_OFF        
      if( d_ptr->m_nGPFrontElapsed == 0 && GetFrontApp() == GetThisAppId()) {
        int ownerApp = GetGstPipelinesOwner(GP_DISPLAY_FRONT);
        GetLogger().Log(QString("afwfb : send render OFF to [%1]").arg(ownerApp), Logger::PRIORITY_LOW); 
        Event evt(EVT_POST_EVENT, GetThisAppId(), (APP_ID_T)ownerApp, EVT_REQUEST_RENDER_OFF);
        NotifyUISH(evt, DISPLAY_FRONT);
      }    
#endif
      d_ptr->m_nGPFrontElapsed += GP_CHECK_INTERVAL;
      if( d_ptr->m_nGPFrontElapsed >= GP_MAX_WAIT_TIME )
      {
        d_ptr->m_pGPFrontTimer->stop();
        SetGstPipelinesStatusInternal(GP_DISPLAY_FRONT, true);
        HandleGstPipelinesAvailable(GP_DISPLAY_FRONT);
      }
    }
    else
    {
      d_ptr->m_pGPFrontTimer->stop();
      SetGstPipelinesStatusInternal(GP_DISPLAY_FRONT, true);
      HandleGstPipelinesAvailable(GP_DISPLAY_FRONT);
    }
  }
}

void AppEngineBase::CheckRearGstPipelines()
{
  GetLogger().Log( QString( "AppEngineBase::CheckRearGstPipelines()") );
  if( d_ptr->m_pGPRearTimer != NULL)
  {
    if (GetGstPipelinesStatus(GP_DISPLAY_REAR))
    {
#ifdef AFW_FORCE_RENDER_OFF        
      if( d_ptr->m_nGPRearElapsed == 0 && GetRearApp() == GetThisAppId()) {
        int ownerApp = GetGstPipelinesOwner(GP_DISPLAY_REAR);
        GetLogger().Log(QString("afwfb : send render OFF to [%1]").arg(ownerApp)); 
        Event evt(EVT_POST_EVENT, GetThisAppId(), (APP_ID_T)ownerApp, EVT_REQUEST_RENDER_OFF);
        NotifyUISH(evt, DISPLAY_REAR);
      }    
#endif
      d_ptr->m_nGPRearElapsed += GP_CHECK_INTERVAL;
      if( d_ptr->m_nGPRearElapsed >= GP_MAX_WAIT_TIME )
      {
        d_ptr->m_pGPRearTimer->stop();
        SetGstPipelinesStatusInternal(GP_DISPLAY_REAR, true);
        HandleGstPipelinesAvailable(GP_DISPLAY_REAR);
      }
    }
    else
    {
      d_ptr->m_pGPRearTimer->stop();
      SetGstPipelinesStatusInternal(GP_DISPLAY_REAR, true);
      HandleGstPipelinesAvailable(GP_DISPLAY_REAR);
    }
  }
}
void AppEngineBase::CheckCameraGstPipelines()
{
  GetLogger().Log( QString( "AppEngineBase::CheckCameraGstPipelines()") );
  if( d_ptr->m_pGPCameraTimer != NULL)
  {
    if (GetGstPipelinesStatus(GP_DISPLAY_CAMERA))
    {
      d_ptr->m_nGPCameraElapsed += GP_CHECK_INTERVAL;
      if( d_ptr->m_nGPCameraElapsed >= GP_MAX_WAIT_TIME )
      {
        d_ptr->m_pGPCameraTimer->stop();
        SetGstPipelinesStatusInternal(GP_DISPLAY_CAMERA, true);
        HandleGstPipelinesAvailable(GP_DISPLAY_CAMERA);
      }
    }
    else
    {
      d_ptr->m_pGPCameraTimer->stop();
      SetGstPipelinesStatusInternal(GP_DISPLAY_CAMERA, true);
      HandleGstPipelinesAvailable(GP_DISPLAY_CAMERA);
    }
  }
}

void AppEngineBase::HandleGstPipelinesAvailable(int nGpType)
{
  // intentional empty
    nGpType = nGpType;
}

bool AppEngineBase::GetGstPipelinesStatus(int nGpType)
{
  GetLogger().Log( QString( "AppEngineBase::GetGstPipelinesStatus()") );
  bool result = false;

  d_ptr->m_shm.attach();
  if ( !d_ptr->m_shm.isAttached() )
  {
	GetLogger().Log("AppEngineBase::GetGstPipelinesStatus - fail to attach shared memory", Logger::PRIORITY_LOW);
	return result;
  }

  d_ptr->m_shm.lock();
  GstPipelineUsage* usage = (GstPipelineUsage*)(d_ptr->m_shm.constData());
  if(nGpType == GP_DISPLAY_FRONT) {
    result = usage->use[0];
    d_ptr->m_nGstOccupiedApp = usage->app[0];
  }
  else if(nGpType == GP_DISPLAY_REAR) {
    result = usage->use[1];
    d_ptr->m_nGstOccupiedApp = usage->app[1];
  }
  else if(nGpType == GP_DISPLAY_CAMERA) {
    result = usage->use[2];
    d_ptr->m_nGstOccupiedApp = usage->app[2];
  }
  d_ptr->m_shm.unlock();
  GetLogger().Log( QString( "AppEngineBase::GetGstPipelinesStatus() nGpType = [%1] bUse = [%2] occupied = [%3]").arg(nGpType).arg((int)result).arg(d_ptr->m_nGstOccupiedApp));
  return result;
}

int AppEngineBase::GetGstPipelinesOwner(int nGpType)
{
  int result = 0;

  d_ptr->m_shm.attach();
  if ( !d_ptr->m_shm.isAttached() )
  {
	GetLogger().Log("AppEngineBase::GetGstPipelinesOwner - fail to attach shared memory", Logger::PRIORITY_LOW);
	return result;
  }

  d_ptr->m_shm.lock();
  GstPipelineUsage* usage = (GstPipelineUsage*)(d_ptr->m_shm.constData());
  if(nGpType == GP_DISPLAY_FRONT && usage->use[0])
    result = usage->app[0];
  else if(nGpType == GP_DISPLAY_REAR && usage->use[1])
    result = usage->app[1];
  else if(nGpType == GP_DISPLAY_CAMERA && usage->use[2])
    result = usage->app[2];
  d_ptr->m_shm.unlock();
  GetLogger().Log( QString( "AppEngineBase::GetGstPipelinesOwner(%1) return = [%2]").arg(nGpType).arg(result));
  return result;    
}
void AppEngineBase::SetGstPipelinesStatusInternal(int nGpType, bool bUse)
{
    GetLogger().Log( QString( "AppEngineBase::SetGstPipelinesStatusInternal()") );

	d_ptr->m_shm.attach();
	if ( !d_ptr->m_shm.isAttached() )
	{
		GetLogger().Log("AppEngineBase::SetGstPipelinesStatusInternal - fail to attach shared memory", Logger::PRIORITY_LOW);
		return;
	}

    d_ptr->m_shm.lock();
    GstPipelineUsage* usage = (GstPipelineUsage*)(d_ptr->m_shm.constData());
    if(nGpType == GP_DISPLAY_FRONT)
    {
      if(d_ptr->m_pGPFrontTimer && d_ptr->m_pGPFrontTimer->isActive())
        d_ptr->m_pGPFrontTimer->stop();
      usage->use[0] = bUse;
      if( bUse ) {
          usage->app[0] = (int)GetThisAppId();
          d_ptr->m_nGstOccupiedApp = (int)GetThisAppId();
      }
    }
    else if(nGpType == GP_DISPLAY_REAR)
    {
      if(d_ptr->m_pGPRearTimer && d_ptr->m_pGPRearTimer->isActive())
        d_ptr->m_pGPRearTimer->stop();
      usage->use[1] = bUse;
      if( bUse ) {
          usage->app[1] = (int)GetThisAppId();
          d_ptr->m_nGstOccupiedApp = (int)GetThisAppId();
      }
    }
    else if(nGpType == GP_DISPLAY_CAMERA)
    {
      if(d_ptr->m_pGPCameraTimer && d_ptr->m_pGPCameraTimer->isActive())
        d_ptr->m_pGPCameraTimer->stop();
      usage->use[2] = bUse;
      if( bUse ) {
          usage->app[2] = (int)GetThisAppId();
          d_ptr->m_nGstOccupiedApp = (int)GetThisAppId();
      }
    }
    d_ptr->m_shm.unlock();
    GetLogger().Log( QString( " AppEngineBase::SetGstPipelinesStatusInternal() nGpType = [%1] bUse = [%2] occupied = [%3]" ).arg( nGpType ).arg( (int)bUse ).arg( d_ptr->m_nGstOccupiedApp ), Logger::PRIORITY_LOW );
}

void AppEngineBase::SetGstPipelinesStatus(int nGpType, bool bUse)
{
  if(bUse) {
      GetLogger().Log( QString( "AppEngineBase::SetGstPipelinesStatus(x, true) prohibited.") );
      return;
  }

  d_ptr->m_shm.attach(); 
  if ( !d_ptr->m_shm.isAttached() )
  {
	GetLogger().Log("AppEngineBase::SetGstPipelinesStatus - fail to attach shared memory", Logger::PRIORITY_LOW);
	return;
  }
  
  if(nGpType == GP_DISPLAY_FRONT && d_ptr->m_pGPFrontTimer != NULL && d_ptr->m_pGPFrontTimer->isActive())
    d_ptr->m_pGPFrontTimer->stop();     
  if(nGpType == GP_DISPLAY_REAR && d_ptr->m_pGPRearTimer != NULL && d_ptr->m_pGPRearTimer->isActive())
    d_ptr->m_pGPRearTimer->stop();     
  if(nGpType == GP_DISPLAY_CAMERA && d_ptr->m_pGPCameraTimer != NULL && d_ptr->m_pGPCameraTimer->isActive())
    d_ptr->m_pGPCameraTimer->stop();     

  GetLogger().Log( QString( "AppEngineBase::SetGstPipelinesStatus()") );
  d_ptr->m_shm.lock();
  GstPipelineUsage* usage = (GstPipelineUsage*)(d_ptr->m_shm.constData());
  if(nGpType == GP_DISPLAY_FRONT)
  {
    if( usage->app[0] == (int)GetThisAppId() )
    {
        usage->use[0] = bUse;
        usage->app[0] = 0;
        d_ptr->m_nGstOccupiedApp = 0;
    }
    else
    {
        d_ptr->m_nGstOccupiedApp = usage->app[0];
        GetLogger().Log( QString( " AppEngineBase::ignored. oppcupied by (%1)").arg(d_ptr->m_nGstOccupiedApp) );
    }
  }
  else if(nGpType == GP_DISPLAY_REAR)
  {
      if( usage->app[1] == (int)GetThisAppId() )
      {
          usage->use[1] = bUse;
          usage->app[1] = 0;
          d_ptr->m_nGstOccupiedApp = 0;
      }
      else
      {
          d_ptr->m_nGstOccupiedApp = usage->app[1];
          GetLogger().Log( QString( " AppEngineBase::ignored. oppcupied by (%1)").arg(d_ptr->m_nGstOccupiedApp) );
      }
  }
  else if(nGpType == GP_DISPLAY_CAMERA)
  {
      if( usage->app[2] == (int)GetThisAppId() )
      {
          usage->use[2] = bUse;
          usage->app[2] = 0;
          d_ptr->m_nGstOccupiedApp = 0;
      }
      else
      {
          d_ptr->m_nGstOccupiedApp = usage->app[2];
          GetLogger().Log( QString( " AppEngineBase::ignored. oppcupied by (%1)").arg(d_ptr->m_nGstOccupiedApp) );
      }
  }
  d_ptr->m_shm.unlock();
  if( d_ptr->m_nGstOccupiedApp == 0 )
    GetLogger().Log( QString( " AppEngineBase::SetGstPipelinesStatus() nGpType = [%1] bUse = [%2] occupied = [%3]" ).arg( nGpType ).arg( (int)bUse ).arg( 0 ), Logger::PRIORITY_LOW );
}

bool
AppEngineBase::IsIgnorableKeyEvent(int eventId)
{
    //GetLogger().Log( QString( "key canceling : ignorable key = %1").arg(eventId), Logger::PRIORITY_LOW );

    if( d_ptr->m_cloneMode != CLONE_OFF ) {
        if((EVT_KEY_CCP_JOG_DIAL_TOP_LEFT <= eventId && eventId <= EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT) ||
            (EVT_KEY_RRC_JOG_DIAL_TOP_LEFT <= eventId && eventId <= EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT) ||
            eventId == EVT_KEY_CCP_JOG_DIAL_ENCODER || eventId == EVT_KEY_RRC_JOG_DIAL_ENCODER )
            return true;
    }
    else {
        if( d_ptr->m_bIsSwapDCEnabled ) {
            if( (EVT_KEY_RRC_JOG_DIAL_TOP_LEFT <= eventId && eventId <= EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT) ||
                 eventId == EVT_KEY_RRC_JOG_DIAL_ENCODER )
                return true;
        }
        else {
            if( (EVT_KEY_CCP_JOG_DIAL_TOP_LEFT <= eventId && eventId <= EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT) ||
                 eventId == EVT_KEY_CCP_JOG_DIAL_ENCODER )
                return true;
        }
    }
    
    return false;
}

bool
AppEngineBase::IsIgnorableKeyWhenSystemPopup( DISPLAY_T aScreen, EVT_ID_T nEventID )
{
    if( GetIsDisplayedPopup(aScreen) )
    {
        switch(nEventID)
        {
        case EVT_KEY_HU_MENU:
        case EVT_KEY_CCP_MENU:
        case EVT_KEY_RRC_MENU:
            return true;

        default:
            break;
        }
    }
    return false;
}

void
AppEngineBase::CheckHWKeyStatus(Event &aEvent)
{
    EVT_ID_T eId = aEvent.GetEventId();

    if( eId == EVT_CAMERA_ENABLE )
        d_ptr->m_bCameraEnabled = true;
    else if( eId == EVT_CAMERA_DISABLE )
        d_ptr->m_bCameraEnabled = false;
    else if( (EVT_KEY_HU_BACK <= eId && eId <= EVT_KEY_SWRC_SEEK_PREV) ||
         (EVT_KEY_HU_EJECT <= eId && eId <= EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT)
            || (EVT_KEY_CCP_SEEK_NEXT <= eId && eId <= EVT_KEY_CCP_SEEK_PREV))
    {
        // exception : if front is photo and rear is seekable app, do not cancel seek key
        if( ((eId == EVT_KEY_HU_SEEK_NEXT || eId == EVT_KEY_HU_SEEK_PREV) && !IsSwapDCEnabled()) ||
             ((eId == EVT_KEY_RRC_SEEK_NEXT || eId == EVT_KEY_RRC_SEEK_PREV) && IsSwapDCEnabled()) ||
                (eId == EVT_KEY_CCP_SEEK_NEXT || eId == EVT_KEY_CCP_SEEK_PREV))  {
            int entry =  IsSwapDCEnabled() ? GetRearEntry() : GetFrontEntry();
            //GetLogger().Log(QString("key cancel : front entry = %1, (%2,%3)").arg(entry).arg(GetFrontEntry()).arg(GetRearEntry()));
            if( IsPhotoEntry(entry) ) {
                GetLogger().Log(QString("key cancel : do not check key for photo"), Logger::PRIORITY_LOW);
                SetHwKeyPressed(false, aEvent, 0);
                return;
            }
        }
                
        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
        if( message->keyStatus == KEY_STATUS_PRESSED ||
             message->keyStatus == KEY_STATUS_PRESSED_LONG ||
             message->keyStatus == KEY_STATUS_PRESSED_CRITICAL ) {
             SetHwKeyPressed(true, aEvent, eId);
        }
        else {
            SetHwKeyPressed(false, aEvent, 0);
        }
        GetLogger().Log(QString("key canceling (key=%1, keyStatus=%2, hwflag=%3)").arg(eId).arg((int)message->keyStatus).arg(d_ptr->m_bHwKeyPressed));
    }
}

void
AppEngineBase::SetHwKeyPressed(bool Value, Event &aEvent, EVT_ID_T eid)
{
    if(isUsedCancelKey(aEvent))
    {
        d_ptr->m_bHwKeyPressed = Value;
        d_ptr->m_hwKeyKind = eid;
    }
}

bool
AppEngineBase::isUsedCancelKey(Event &aEvent)
{
    switch(aEvent.GetEventId())
    {
        case EVT_KEY_HU_SEEK_NEXT:
        case EVT_KEY_HU_SEEK_PREV:
        case EVT_KEY_RRC_SEEK_NEXT:
        case EVT_KEY_RRC_SEEK_PREV:
        case EVT_KEY_CCP_SEEK_NEXT:
        case EVT_KEY_CCP_SEEK_PREV:
        case EVT_KEY_SWRC_SEEK_NEXT:
        case EVT_KEY_SWRC_SEEK_PREV:
        case EVT_KEY_RRC_CH_FW:
        case EVT_KEY_RRC_CH_BK:
        case EVT_KEY_CCP_JOG_DIAL_TOP_LEFT:
        case EVT_KEY_CCP_JOG_DIAL_LEFT:
        case EVT_KEY_CCP_JOG_DIAL_DOWN_LEFT:
        case EVT_KEY_CCP_JOG_DIAL_UP:
        case EVT_KEY_CCP_JOG_DIAL_CENTER:
        case EVT_KEY_CCP_JOG_DIAL_DOWN:
        case EVT_KEY_CCP_JOG_DIAL_TOP_RIGHT:
        case EVT_KEY_CCP_JOG_DIAL_RIGHT:
        case EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT:
        case EVT_KEY_RRC_JOG_DIAL_TOP_LEFT:
        case EVT_KEY_RRC_JOG_DIAL_LEFT:
        case EVT_KEY_RRC_JOG_DIAL_DOWN_LEFT:
        case EVT_KEY_RRC_JOG_DIAL_UP:
        case EVT_KEY_RRC_JOG_DIAL_CENTER:
        case EVT_KEY_RRC_JOG_DIAL_DOWN:
        case EVT_KEY_RRC_JOG_DIAL_TOP_RIGHT:
        case EVT_KEY_RRC_JOG_DIAL_RIGHT:
        case EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT:
            return true;
            break;
        default:
            return false;
            break;
    }
    return false;
}

bool
AppEngineBase::IsCameraEnabled()
{
    return d_ptr->m_bCameraEnabled;
}

bool
AppEngineBase::IsRRCKey(int key)
{
    if( (EVT_KEY_RRC_BACK <= key && key <= EVT_KEY_RRC_JOG_DIAL) ||
          (EVT_KEY_RRC_JOG_DIAL_TOP_LEFT <= key && key <= EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT) ||
          (key == EVT_KEY_RRC_JOG_DIAL_ENCODER) )
          return true;
    return false;
}

void
AppEngineBase::CancelHwKey(DISPLAY_T aScreen, bool WithOutSeek)
{
    if(WithOutSeek == true)
    {
         if( d_ptr->m_hwKeyKind == EVT_KEY_HU_SEEK_NEXT || d_ptr->m_hwKeyKind == EVT_KEY_HU_SEEK_PREV
                 || d_ptr->m_hwKeyKind == EVT_KEY_RRC_SEEK_NEXT || d_ptr->m_hwKeyKind == EVT_KEY_RRC_SEEK_PREV
                 || d_ptr->m_hwKeyKind == EVT_KEY_SWRC_SEEK_NEXT || d_ptr->m_hwKeyKind == EVT_KEY_SWRC_SEEK_PREV
                 || d_ptr->m_hwKeyKind == EVT_KEY_CCP_SEEK_NEXT || d_ptr->m_hwKeyKind == EVT_KEY_CCP_SEEK_PREV)
             return;
    }
    //GetLogger().Log(QString("key canceling (key=%1, %2)").arg(d_ptr->m_hwKeyKind).arg(d_ptr->m_bHwKeyPressed));
    if( d_ptr->m_bHwKeyPressed && d_ptr->m_hwKeyKind != 0 ) {
        GetLogger().Log(QString("key canceling (key=%1) on %2").arg(d_ptr->m_hwKeyKind).arg(aScreen), Logger::PRIORITY_LOW);
        KEY_EVENT_DATA_T message;
        message.header.appIdReciever = APP_ALL;
        message.header.dataSize = sizeof( message );
        message.keyStatus = KEY_STATUS_CANCEL;
        Event event( d_ptr->m_hwKeyKind, GetThisAppId(), GetThisAppId(), QByteArray( (char *)&message, sizeof( message ) ));
        HandleNotifyFromUIStateManagerCommon(DISPLAY_COMMON, event);
    }
}

bool
AppEngineBase::IgnoreKeyWhenTouched(Event &aEvent, DISPLAY_T Display)
{
    bool bWillCloneMode = false;
    if( aEvent.GetEventId() == EVT_CLONE_NOTIFICATION ) {
        d_ptr->m_cloneMode = (int)(((EVENT_CLONE_NOTIFICATION_DATA_T *)(aEvent.Data().data()))->sCloneStatus);
        GetLogger().Log(QString("EVT_CLONE_NOTIFICATION [%1]").arg(d_ptr->m_cloneMode));
    }

    if( aEvent.GetEventId() == EVT_REQUEST_FG ) {
        APP_ENTRY_POINT_DATA_T* entryData = (APP_ENTRY_POINT_DATA_T *)(aEvent.Data().data());

        if( GetCloneMode() == CLONE_OFF && entryData->sHeader.bReserved02 )
           bWillCloneMode = true;
        else if( GetCloneMode() != CLONE_OFF && !entryData->sHeader.bReserved02 )
           bWillCloneMode = true;

        APP_ID_T OtherScreenApp = APP_NONE;
        APP_ID_T ThisScreenApp = APP_NONE;
        APP_ENTRY_POINT_T OtherEntry = eAPP_NONE;
        if(Display == DISPLAY_FRONT)
            OtherScreenApp = (APP_ID_T)GetRearGuiApp();
        else if(Display == DISPLAY_REAR)
            OtherScreenApp = (APP_ID_T)GetFrontGuiApp();

        if(Display == DISPLAY_FRONT)
            OtherEntry = (APP_ENTRY_POINT_T)GetRearEntry();
        else if(Display == DISPLAY_REAR)
            OtherEntry = (APP_ENTRY_POINT_T)GetFrontEntry();

        if(Display == DISPLAY_FRONT)
            ThisScreenApp = (APP_ID_T)GetFrontApp();
        else if(Display == DISPLAY_REAR)
            ThisScreenApp = (APP_ID_T)GetRearApp();

        if(d_ptr->m_cloneMode == 0 )
        {
            if(bWillCloneMode == false)
            {
                if(OtherScreenApp != GetThisAppId())
                {
                    d_ptr->m_bJogIgnored = true;
                    d_ptr->m_bHwKeyPressed = false;
                }
                else if(GetThisAppId() == APP_MAIN_MENU)
                {
                    d_ptr->m_bJogIgnored = false;
                    d_ptr->m_bHwKeyPressed = false;
                }
            }
        }
        else
        {
            if(bWillCloneMode == 0)
            {
                d_ptr->m_bJogIgnored = true;
                d_ptr->m_bHwKeyPressed = false;
            }
            else
            {
                if(OtherScreenApp != GetThisAppId())
                {
                    d_ptr->m_bJogIgnored = true;
                    d_ptr->m_bHwKeyPressed = false;
                }
                else if(GetThisAppId() == APP_MAIN_MENU)
                {
                    d_ptr->m_bJogIgnored = false;
                    d_ptr->m_bHwKeyPressed = false;
                }
            }
        }

//        if(!(IsPhotoEntry(entryData->sAppEntryPoint) && OtherScreenApp == APP_MEDIA_PLAYER) )
//        {
//            d_ptr->m_bJogIgnored = true;
//            if(ThisScreenApp != GetThisAppId())
//                d_ptr->m_bHwKeyPressed = false;
//        }
    }

    // key canceling while touch pressed
    //GetLogger().Log(QString("key canceling : touch pressed [%1][%2][%3]").arg(d_ptr->m_bTouchPressed).arg(QApplication::mouseButtons()).arg(aEvent.GetEventId()));

    if( d_ptr->m_bTouchPressed && QApplication::mouseButtons() == Qt::LeftButton ) {
        int eId = aEvent.GetEventId();
        if( IsIgnorableKeyEvent(eId) ) {
            d_ptr->m_bJogIgnored = true;
            KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
            GetLogger().Log(QString("key canceling (key=%1, keyStatus=%2)").arg(eId).arg((int)message->keyStatus), Logger::PRIORITY_LOW);
            return true;
        }
    }
    else {
        if( d_ptr->m_bJogIgnored ) {
            int eId = aEvent.GetEventId();
            if( IsIgnorableKeyEvent(eId) && eId < EVT_KEY_HU_JOG_DIAL_ENCODER ) {
                KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
                if( message->keyStatus == KEY_STATUS_RELEASED ||
                    message->keyStatus == KEY_STATUS_PRESSED_LONG ) {
                    if(GetThisAppId() != APP_MOBILETV)
                    {
                        GetLogger().Log(QString("key canceling long & release (key=%1, keyStatus=%2)").arg(eId).arg((int)message->keyStatus), Logger::PRIORITY_LOW);
                        return true;
                    }
                }
                else {
                    d_ptr->m_bJogIgnored = false;
                }
            }
        }
    }

    CheckHWKeyStatus(aEvent);

    if( EVT_REQUEST_BG == aEvent.GetEventId() ) {
        SetTouchLock(false);
        d_ptr->m_bTouchPressed = false;
        d_ptr->m_bJogIgnored = false;
        //d_ptr->m_bHwKeyPressed = false;
    }
    return false;
}

int
AppEngineBase::GetCloneMode()
{
    return d_ptr->m_cloneMode;
}

void
AppEngineBase::SendTouchCleanUp()
{
    GetLogger().Log("AppEngineBase::SendTouchCleanUp()", Logger::PRIORITY_LOW);
    if( d_ptr->m_micomManager == NULL || !d_ptr->m_micomManager->isValid() ) {
        d_ptr->m_micomManager = new QDBusInterface("com.lge.car.micom", "/Dtc", "com.lge.car.micom.dtc", QDBusConnection::sessionBus(), this);
        if(d_ptr->m_micomManager == NULL || !d_ptr->m_micomManager->isValid()) {
            Logger::Instance()->Log(QString("invalid interface : %1").arg(QDBusConnection::sessionBus().lastError().message()));
            return;
        }
    }
    if( d_ptr->m_micomManager->isValid()) {
        d_ptr->m_micomManager->asyncCall(QLatin1String("LgeDBus_TouchCleanup"));
    }
}

void AppEngineBase::HandleAutoVisibility(uint nScreen)
{
    if( d_ptr->m_nEngineType  == APP_QML ) {
        AppEngineQML* pEngine = dynamic_cast<AppEngineQML*> (this);
        if ( pEngine != NULL )
            pEngine->HandleAutoVisibility(nScreen); 
    }
    else if( d_ptr->m_nEngineType == APP_QML_VEXT ) {
        AppVextEngineQML* pEngine = dynamic_cast<AppVextEngineQML*> (this);
        if ( pEngine != NULL )
            pEngine->HandleAutoVisibility(nScreen);
    }  
}

void AppEngineBase::SetFrontApp(int app)
{
	d_ptr->m_shmFb.attach();

	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::SetFrontApp - fail to attach shared memory", Logger::PRIORITY_LOW);
		return;
	}
	
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    usage->m_frontApp = app;
    d_ptr->m_shmFb.unlock();
}
void AppEngineBase::SetRearApp(int app)
{
	d_ptr->m_shmFb.attach();

	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::SetRearApp - fail to attach shared memory", Logger::PRIORITY_LOW);
		return;
	}
	
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    usage->m_rearApp = app;
    d_ptr->m_shmFb.unlock();
}

void AppEngineBase::SetFrontGuiApp(int app)
{
	d_ptr->m_shmFb.attach();
	
	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::SetFrontGuiApp - fail to attach shared memory", Logger::PRIORITY_LOW);
		return;
	}
	
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    usage->m_frontGuiApp = app;
    d_ptr->m_shmFb.unlock();
}
void AppEngineBase::SetRearGuiApp(int app)
{
	d_ptr->m_shmFb.attach();

	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::SetRearGuiApp - fail to attach shared memory", Logger::PRIORITY_LOW);
		return;
	}
	
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    usage->m_rearGuiApp = app;
    d_ptr->m_shmFb.unlock();
}

void AppEngineBase::SetFrontRenderAppInBG(int app)
{
	d_ptr->m_shmFb.attach();

	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::SetFrontRenderAppInBG - fail to attach shared memory", Logger::PRIORITY_LOW);
		return;
	}
	
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    usage->m_frontRenderAppInBG = app;
    d_ptr->m_shmFb.unlock();
}
void AppEngineBase::SetRearRenderAppInBG(int app)
{
	d_ptr->m_shmFb.attach();

	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::SetRearRenderAppInBG - fail to attach shared memory", Logger::PRIORITY_LOW);
		return;
	}
	
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    usage->m_rearRenderAppInBG = app;
    d_ptr->m_shmFb.unlock();
}

void AppEngineBase::SetFrontRenderEntryInBG(int entry)
{
	d_ptr->m_shmFb.attach();

	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::SetFrontRenderEntryInBG - fail to attach shared memory", Logger::PRIORITY_LOW);
		return;
	}
	
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    usage->m_frontRenderEntryInBG = entry;
    d_ptr->m_shmFb.unlock();
}
void AppEngineBase::SetRearRenderEntryInBG(int entry)
{
	d_ptr->m_shmFb.attach();

	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::SetRearRenderEntryInBG - fail to attach shared memory", Logger::PRIORITY_LOW);
		return;
	}
		
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    usage->m_rearRenderEntryInBG = entry;
    d_ptr->m_shmFb.unlock();
}

void AppEngineBase::SetFrontEntry(int entry)
{
	d_ptr->m_shmFb.attach();

	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::SetFrontEntry - fail to attach shared memory", Logger::PRIORITY_LOW);
		return;
	}
	
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    usage->m_frontEntry = entry;
    d_ptr->m_shmFb.unlock();    
}

void AppEngineBase::SetRearEntry(int entry)
{
	d_ptr->m_shmFb.attach();

	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::SetRearEntry - fail to attach shared memory", Logger::PRIORITY_LOW);
		return;
	}
	
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    usage->m_rearEntry= entry;
    d_ptr->m_shmFb.unlock();    
}

int AppEngineBase::GetFrontApp()
{
    int app = APP_NONE;

	d_ptr->m_shmFb.attach();

	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::GetFrontApp - fail to attach shared memory", Logger::PRIORITY_LOW);
		return app;
	}
		
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    app =  usage->m_frontApp;
    d_ptr->m_shmFb.unlock();
    return app;
}


int AppEngineBase::GetFrontGuiApp()
{
    int app = APP_NONE;
	
	d_ptr->m_shmFb.attach();
	
	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::GetFrontGuiApp - fail to attach shared memory", Logger::PRIORITY_LOW);
		return app;
	}

    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    app =  usage->m_frontGuiApp;
    d_ptr->m_shmFb.unlock();
    return app;
}

int AppEngineBase::GetRearGuiApp()
{
    int app = APP_NONE;

	d_ptr->m_shmFb.attach();
	
	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::GetRearGuiApp - fail to attach shared memory", Logger::PRIORITY_LOW);
		return app;
	}

    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    app =  usage->m_rearGuiApp;
    d_ptr->m_shmFb.unlock();        
    return app;
}

int AppEngineBase::GetRearApp()
{
    int app = APP_NONE;

	d_ptr->m_shmFb.attach();
	
	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::GetRearApp - fail to attach shared memory", Logger::PRIORITY_LOW);
		return app;
	}
	
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    app =  usage->m_rearApp;
    d_ptr->m_shmFb.unlock();        
    return app;
}

int AppEngineBase::GetFrontRenderAppInBG()
{
    int app = APP_NONE;

	d_ptr->m_shmFb.attach();
	
	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::GetFrontRenderAppInBG - fail to attach shared memory", Logger::PRIORITY_LOW);
		return app;
	}
	
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    app =  usage->m_frontRenderAppInBG;
    d_ptr->m_shmFb.unlock();
    return app;
}

int AppEngineBase::GetRearRenderAppInBG()
{
    int app = APP_NONE;
	
	d_ptr->m_shmFb.attach();
	
	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::GetRearRenderAppInBG - fail to attach shared memory", Logger::PRIORITY_LOW);
		return app;
	}
	
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    app =  usage->m_rearRenderAppInBG;
    d_ptr->m_shmFb.unlock();        
    return app;
}

int AppEngineBase::GetFrontRenderEntryInBG()
{
    int entry = eAPP_NONE;

	d_ptr->m_shmFb.attach();
	
	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::GetFrontRenderEntryInBG - fail to attach shared memory", Logger::PRIORITY_LOW);
		return entry;
	}
	
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    entry =  usage->m_frontRenderEntryInBG;
    d_ptr->m_shmFb.unlock();
    return entry;
}

int AppEngineBase::GetRearRenderEntryInBG()
{
    int entry = eAPP_NONE;

	d_ptr->m_shmFb.attach();
	
	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::GetRearRenderEntryInBG - fail to attach shared memory", Logger::PRIORITY_LOW);
		return entry;
	}
		
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    entry =  usage->m_rearRenderEntryInBG;
    d_ptr->m_shmFb.unlock();        
    return entry;
}

int AppEngineBase::GetFrontEntry()
{
    int entry = APP_NONE;

	d_ptr->m_shmFb.attach();
	
	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::GetFrontEntry - fail to attach shared memory", Logger::PRIORITY_LOW);
		return entry;
	}
	
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    entry =  usage->m_frontEntry;
    d_ptr->m_shmFb.unlock();        
    return entry;
}

int AppEngineBase::GetRearEntry()
{
    int entry = APP_NONE;
	
	d_ptr->m_shmFb.attach();
	
	if(!d_ptr->m_shmFb.isAttached())
	{
		GetLogger().Log("AppEngineBase::GetRearEntry - fail to attach shared memory", Logger::PRIORITY_LOW);
		return entry;
	}
	
    d_ptr->m_shmFb.lock();
    AppUsage* usage = (AppUsage*)(d_ptr->m_shmFb.constData());
    entry =  usage->m_rearEntry;
    d_ptr->m_shmFb.unlock();        
    return entry;
}
    
bool 
AppEngineBase::IsOverlayUse( int uiGuiEntryPoint )
{
    switch( uiGuiEntryPoint )
    {	
        case eAPP_CAMERA_DIRECT:
        case eAPP_CAMERA_PGS:
        case eAPP_CAMERA_AVM:
        case eAPP_DMB:
        case eAPP_DMB_MODE_DMB1:
        case eAPP_MODE_DYNAMICS_DMB:
        case eAPP_AVP_VIDEO:
        case eAPP_AVP_VIDEO_MODE_AUX:
        //case eGUI_ENTRY_AVP_VIDEO_AUX2                    TODO
        case eAPP_AVP_VIDEO_MODE_DISC:
        case eAPP_AVP_VIDEO_MODE_JUKEBOX:
        case eAPP_AVP_VIDEO_MODE_USB1:
        case eAPP_AVP_VIDEO_MODE_USB2:
        case eAPP_VIDEOGUIDE_LIST:
        case eAPP_VIDEOGUIDE_GUIDE_PLAY:
        case eAPP_VIDEOGUIDE_GUIDE_PLAY_DRS:
        case eAPP_VIDEOGUIDE_BTHELP_CONNECTION:
        case eAPP_VIDEOGUIDE_BTHELP_REGISTRATION:
        case eAPP_VIDEOGUIDE_VRHELP_USAGE:
        case eAPP_EMANUAL_BTHELP_CONNECTION:
        case eAPP_EMANUAL_VRHELP_USAGE:
        case eAPP_SETTINGS_SCREEN_AVP_AUX:
        case eAPP_SETTINGS_SCREEN_AVP_DIVX:
        case eAPP_SETTINGS_SCREEN_AVP_DVD:
        case eAPP_SETTINGS_SCREEN_MOBILETV:
        case eAPP_SETTINGS_SCREEN_QUICK_GUIDE:
        case eAPP_SETTINGS_SCREEN_VIDEO:
            return true;
            break;
        case eAPP_AVP_AUDIO_MODE_DISC:
            //if ( GetUIStateManager()->GetUtils().GetDiscType() == eDISC_TYPE_AUDIO_DVD )
            return true;				
            break;
        default:
            break;
    }
    return false;
}

bool 
AppEngineBase::IsVideoPlayerEntry(int entry)
{
    switch(entry)
    {
        case eAPP_AVP_VIDEO:
        case eAPP_AVP_VIDEO_MODE_AUX:
        //case eGUI_ENTRY_AVP_VIDEO_AUX2:           TODO
        case eAPP_AVP_VIDEO_MODE_DISC:
        case eAPP_AVP_VIDEO_MODE_JUKEBOX:
        case eAPP_AVP_VIDEO_MODE_USB1:
        case eAPP_AVP_VIDEO_MODE_USB2:
        case eAPP_AVP_VIDEO_MODE_AUX_DRS:
        case eAPP_AVP_VIDEO_MODE_DISC_DRS:
        case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS:
        case eAPP_AVP_VIDEO_MODE_USB1_DRS:
        case eAPP_AVP_VIDEO_MODE_USB2_DRS:
        case eAPP_DMB_MODE_DMB1:
        case eAPP_DMB_MODE_DMB2:
        case eAPP_DMB_MODE_DMB1_DRS:
        case eAPP_DMB_MODE_DMB2_DRS:
        case eAPP_SETTINGS_SCREEN_AVP_AUX:
        case eAPP_SETTINGS_SCREEN_AVP_DIVX:
        case eAPP_SETTINGS_SCREEN_AVP_DVD:
        case eAPP_SETTINGS_SCREEN_MOBILETV:
        case eAPP_SETTINGS_SCREEN_VIDEO:            
        case eAPP_SETTINGS_SCREEN_AVP_DIVX_DRS:
        case eAPP_SETTINGS_SCREEN_AVP_DVD_DRS:
        case eAPP_SETTINGS_SCREEN_AVP_AUX_DRS:
        case eAPP_SETTINGS_SCREEN_MOBILETV_DRS:
            return true;
            break;
        default:
            break;
    }
    return false;
}

bool AppEngineBase::IsPhotoEntry(int entry)
{
    bool ret;
    switch(entry) {
        case eAPP_AVP_PHOTO_MODE_JUKEBOX:
        case eAPP_AVP_PHOTO_MODE_USB1:
        case eAPP_AVP_PHOTO_MODE_USB2:
        case eAPP_AVP_PHOTO_MODE_JUKEBOX_DRS:
        case eAPP_AVP_PHOTO_MODE_USB1_DRS:
        case eAPP_APP_PHOTO_MODE_USB2_DRS:
            ret = true;
            break;

        default:
            ret = false;
            break;
    }
    return ret;
}

int
AppEngineBase::GetRealAppId(DISPLAY_T display)
{
    /*
    if( d_ptr->m_cloneMode == CLONE_ON_FRONT )
        return GetFrontApp();
    else if( d_ptr->m_cloneMode == CLONE_ON_REAR )
        return GetRearApp();
    else {
    */
        if( display == DISPLAY_FRONT ) {
            if( d_ptr->m_bIsSwapDCEnabled )
                return GetRearApp();
            else
                return GetFrontApp();
        }
        else {
            if( d_ptr->m_bIsSwapDCEnabled )
                return GetFrontApp();
            else
                return GetRearApp();
        }
    //}
    return  APP_NONE;
}

int
AppEngineBase::GetRealEntry(DISPLAY_T display)
{
    /*
    if( d_ptr->m_cloneMode == CLONE_ON_FRONT )
        return GetFrontEntry();
    else if( d_ptr->m_cloneMode == CLONE_ON_REAR )
        return GetRearEntry();
    else {
    */
        if( display == DISPLAY_FRONT ) {
            if( d_ptr->m_bIsSwapDCEnabled )
                return GetRearEntry();
            else
                return GetFrontEntry();
        }
        else {
            if( d_ptr->m_bIsSwapDCEnabled )
                return GetFrontEntry();
            else
                return GetRearEntry();
        }
    //}
    return  eAPP_NONE;
}

bool 
AppEngineBase::IsVideoApp(int app)
{
    bool bVideo;
    switch(app) {
        case APP_CAMERA:
        case APP_MOBILETV:
        case APP_AVP:
        case APP_INTERACTIVE_GUIDE:
		case APP_IBOX:
            bVideo = true;
            break;
        default:
            bVideo = false;
            break;
    }
    return bVideo;
}


bool
AppEngineBase::IsDisplaySetting(int entry)
{
    bool result;
    switch(entry) {
        case eAPP_SETTINGS_SCREEN_AVP_AUX:
        case eAPP_SETTINGS_SCREEN_AVP_DIVX:
        case eAPP_SETTINGS_SCREEN_AVP_DVD:
        case eAPP_SETTINGS_SCREEN_MOBILETV:
        case eAPP_SETTINGS_SCREEN_QUICK_GUIDE:
        case eAPP_SETTINGS_SCREEN_VIDEO:
            result = true;
            break;
        default:
            result = false;
            break;
    }
    return result;
}

AppEngineBase::APP_DISPLAY_SETTING_T 
AppEngineBase::TranslateToDisplayAppID(int appId, int entryId)
{
    APP_DISPLAY_SETTING_T daId = ADS_MAX;
    switch(appId) {
        case APP_CAMERA:
            daId = ADS_CAMERA;
            break;
        case APP_MOBILETV:
            daId = ADS_MOBILETV;
            break;
        case APP_AVP: {
            switch(entryId) {
                case eAPP_AVP_VIDEO_MODE_USB1:
	        case eAPP_AVP_VIDEO_MODE_USB2:
                case eAPP_AVP_VIDEO_MODE_USB1_DRS:
                case eAPP_AVP_VIDEO_MODE_USB2_DRS:
                    daId = ADS_AVP_USB;
                    break;
                case eAPP_AVP_VIDEO_MODE_DISC:
                case  eAPP_AVP_VIDEO_MODE_DISC_DRS:
                    daId = ADS_AVP_DISC;
                    break;
	        case eAPP_AVP_VIDEO_MODE_AUX:
                case eAPP_AVP_VIDEO_MODE_AUX_DRS:
                    daId = ADS_AVP_AUX;
                    break;
                case eAPP_AVP_VIDEO:
                case eAPP_AVP_VIDEO_MODE_JUKEBOX:
                case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS:
                    daId = ADS_AVP_JUKEBOX;
                    break;
                default:
                    GetLogger().Log(QString("afwfb : unsupported avp entry [%1] for display setting ").arg(entryId));
                    break;
            }
            break;
        case APP_INTERACTIVE_GUIDE:
            daId = ADS_QUICKGUIDE;
            break;
        }
        default: 
            GetLogger().Log(QString("afwfb : unsupported app [%1] for display setting ").arg(appId));
            break;
    }
    return daId;
}

void AppEngineBase::SetAppDisplaySetting( APP_DISPLAY_SETTING_T type, unsigned int brightness, unsigned int contrast, unsigned int tint, unsigned int hue )
{
    if( type < ADS_MAX ) {
		d_ptr->m_shmDs.attach();
		if ( !d_ptr->m_shmDs.isAttached() )
		{	
			GetLogger().Log(QString("afwfb : SetAppDisplaySetting - fail to attach shared memory "), Logger::PRIORITY_LOW);
			return;
    	}
			
        d_ptr->m_shmDs.lock();
        DisplaySetting* setting = (DisplaySetting*)(d_ptr->m_shmDs.constData());
        setting->brightness[type] = brightness;
        setting->constrast[type] = contrast;
        setting->hue[type] = hue;
        setting->tint[type] = tint;
        d_ptr->m_shmDs.unlock();
        GetLogger().Log( QString("afwfb : SetAppDisplaySetting[%1] = (b=%2, c=%3, t=%4, h=%5)").
            arg((int)type).arg(brightness).arg(contrast).arg(tint).arg(hue), Logger::PRIORITY_LOW);
    }
}

bool AppEngineBase::GetAppDisplaySetting( APP_DISPLAY_SETTING_T type, unsigned int *brightness, unsigned int *contrast, unsigned int *tint, unsigned int *hue )
{
    if( type < ADS_MAX ) {
		d_ptr->m_shmDs.attach();
		if ( !d_ptr->m_shmDs.isAttached() )
		{	
			GetLogger().Log(QString("afwfb : GetAppDisplaySetting - fail to attach shared memory "), Logger::PRIORITY_LOW);
			return false;
    	}
		
        d_ptr->m_shmDs.lock();
        DisplaySetting* setting = (DisplaySetting*)(d_ptr->m_shmDs.constData());
        *brightness = setting->brightness[type];
        *contrast = setting->constrast[type];
        *hue = setting->hue[type];
        *tint = setting->tint[type];
        d_ptr->m_shmDs.unlock();
        GetLogger().Log( QString("afwfb : GetAppDisplaySetting[%1] = (b=%2, c=%3, t=%4, h=%5)").
            arg((int)type).arg(*brightness).arg(*contrast).arg(*tint).arg(*hue), Logger::PRIORITY_LOW);
        return true;
    }
    return false;
}

void AppEngineBase::SetDisplayPlane(APP_ID_T appId, unsigned int displayPlane)
{
	d_ptr->m_shmDp.attach();
	if ( !d_ptr->m_shmDp.isAttached() )
	{	
		GetLogger().Log(QString("afwfb : SetDisplayPlane - fail to attach shared memory "), Logger::PRIORITY_LOW);
		return;
	}
    d_ptr->m_shmDp.lock();
    DisplayPlane* dp = (DisplayPlane*)(d_ptr->m_shmDp.constData());
    dp->displayPlane[appId] = displayPlane;
    d_ptr->m_shmDp.unlock();
    GetLogger().Log(QString("afwfb : SetDisplayPlane(%1=%2)").arg(appId).arg(displayPlane), Logger::PRIORITY_LOW);
}

unsigned int AppEngineBase::GetDisplayPlane(APP_ID_T appId)
{
    unsigned int disp = 0;

	d_ptr->m_shmDp.attach();
	if ( !d_ptr->m_shmDp.isAttached() )
	{	
		GetLogger().Log(QString("afwfb : GetDisplayPlane - fail to attach shared memory "), Logger::PRIORITY_LOW);
		return disp;
	}
	
    d_ptr->m_shmDp.lock();
    DisplayPlane* dp = (DisplayPlane*)(d_ptr->m_shmDp.constData());
    disp =  dp->displayPlane[appId];
    d_ptr->m_shmDp.unlock();       
    GetLogger().Log(QString("afwfb : GetDisplayPlane(%1) = %2").arg(appId).arg(disp), Logger::PRIORITY_LOW);
    return disp;    
}

void
AppEngineBase::CheckDRS(Event &evt, uint nScreen)
{
    int evtId = evt.GetEventId();
    if( evtId == EVT_DRS_SHOW ) {
        //GetLogger().Log(QString("afwfb : drs ON screen=%1").arg(nScreen), Logger::PRIORITY_LOW);
        d_ptr->m_DRS = nScreen;
    }
    else if( evtId == EVT_DRS_HIDE ) {
        //GetLogger().Log(QString("afwfb : drs OFF screen=%1").arg(nScreen), Logger::PRIORITY_LOW);
        d_ptr->m_DRS = 0;
    }
}

bool
AppEngineBase::IsDRS()
{
    return d_ptr->m_DRS != 0;
}

void AppEngineBase::BlendOverlayFb(int aSet, int aDisplayTarget, bool needSwap)
{
#if 1
	Q_UNUSED(aSet);
	Q_UNUSED(aDisplayTarget);
	Q_UNUSED(needSwap);
	return;
#else

    GetLogger().Log(QString("afwfb : blend [%1] on [%2]")
        .arg(aSet == 0 ? "off" : (aSet == 2 ? "all" : "on"))
        .arg(aDisplayTarget ==DualDisplayHelper::eDisplay_Primary ? "primary" : "secondary"), Logger::PRIORITY_HI);

    if( aSet == 0 )
        aSet = 1;
    else if( aSet == 1 )
        aSet = 0;
    if( needSwap && !d_ptr->m_bIsSwapDCEnabled ) {
        GetLogger().Log("afwfb : blend on the unwanted swap state!!!", Logger::PRIORITY_LOW); 
        //aDisplayTarget = (aDisplayTarget == DualDisplayHelper::eDisplay_Primary ? DualDisplayHelper::eDisplay_Secondary : DualDisplayHelper::eDisplay_Primary);
    }
	if ( GetThisAppId() != APP_CAMERA )
    d_ptr->m_pDDHelper->BlendOverlayFb(aSet, (DualDisplayHelper::EDisplayTarget)aDisplayTarget);
#endif
#if 0
    if( aSet == 0 )
    {
        DISPLAY_T display = ( aDisplayTarget == DualDisplayHelper::eDisplay_Primary ) ? DISPLAY_FRONT : DISPLAY_REAR;
        Event evt1( EVT_FB_OVERLAY_BLEND_ON, GetThisAppId(), APP_AVP );
        NotifyUISH(evt1, display);
        Event evt2( EVT_FB_OVERLAY_BLEND_ON, GetThisAppId(), APP_MOBILETV );
        NotifyUISH(evt2, display);
    }
    else if( aSet == 1 )
    {
        DISPLAY_T display = ( aDisplayTarget == DualDisplayHelper::eDisplay_Primary ) ? DISPLAY_FRONT : DISPLAY_REAR;
        Event evt1( EVT_FB_OVERLAY_BLEND_OFF, GetThisAppId(), APP_AVP );
        NotifyUISH(evt1, display);
        Event evt2( EVT_FB_OVERLAY_BLEND_OFF, GetThisAppId(), APP_MOBILETV );
        NotifyUISH(evt2, display);
    }
    else if( aSet == 2 )
    {
        Event evt1( EVT_FB_OVERLAY_BLEND_ON, GetThisAppId(), APP_AVP );
        NotifyUISH(evt1, DISPLAY_COMMON);
        Event evt2( EVT_FB_OVERLAY_BLEND_ON, GetThisAppId(), APP_MOBILETV );
        NotifyUISH(evt2, DISPLAY_COMMON);
    }
#endif
}

void AppEngineBase::BindOverlayFb(int aSet, int aDisplayTarget)
{
#if 1
	Q_UNUSED(aSet);
	Q_UNUSED(aDisplayTarget);
	return;
#else
    GetLogger().Log(QString("afwfb : bind [%1] on [%2]")
        .arg(aSet == 0 ? "overlay" : "spritec")
        .arg(aDisplayTarget ==DualDisplayHelper::eDisplay_Primary ? "primary" : "secondary"), Logger::PRIORITY_LOW);
    d_ptr->m_pDDHelper->BindOverlayFb(aSet, (DualDisplayHelper::EDisplayTarget)aDisplayTarget);
#endif 
}

void AppEngineBase::ChangeOverlay(uint nScreen, int displayPlane)
{
#if 1
	Q_UNUSED(nScreen);
	Q_UNUSED(displayPlane);
	return;
#else
    int appId = (nScreen == DISPLAY_FRONT ? GetFrontApp() : GetRearApp());
    int entryId = (nScreen == DISPLAY_FRONT ? GetFrontEntry() : GetRearEntry());
    if( IsDisplaySetting(entryId) ) {
        appId = (nScreen == DISPLAY_FRONT ? GetFrontRenderAppInBG() : GetRearRenderAppInBG());
        if( appId == APP_AVP )
            entryId = (nScreen == DISPLAY_FRONT ? GetFrontRenderEntryInBG() : GetRearRenderEntryInBG());
    }
    GetLogger().Log(QString("afwfb : change plane [%1=%2] (e=%3)")
        .arg(appId)
        .arg(displayPlane == OVERLAY_PLANE ? "overlay" : (displayPlane == SPRITEC_PLANE ? "spritec" : "dual"))
        .arg(entryId), 
        Logger::PRIORITY_LOW);
    unsigned int brightness = 0, contrast = 0, tint = 0, hue = 0;
    APP_DISPLAY_SETTING_T displayApp = TranslateToDisplayAppID(appId, entryId);
    if( GetAppDisplaySetting(displayApp, &brightness, &contrast, &tint, &hue) ) {
        DualDisplayHelper::MEDIA_DISPLAY disp;
        switch(displayPlane) {
            case OVERLAY_PLANE:
                disp = DualDisplayHelper::eMP_DISPLAY_B;
                break;
            case SPRITEC_PLANE:
                disp = DualDisplayHelper::eMP_DISPLAY_A;
                break;
            case OVERLAY_SPRITEC:
                disp = DualDisplayHelper::eMP_DISPLAY_A_AND_B;
                break;
            default:
                disp = DualDisplayHelper::eMP_DISPLAY_B;
                break;
        }
        SetDisplayPlane((APP_ID_T)appId, disp);
    }
#endif
}

void
AppEngineBase::DoFrontFBTransition(FB_TR_REASON reason)
{  
    QDateTime	dateTime = QDateTime::currentDateTime();
    GetLogger().Log( 
        QString("AppEngineBase::DoFrontFBTransition() afwfb [r=%1][fa=%2][ra=%3][fe=%4][re=%5] at %6").
            arg((int)reason).
            arg(GetFrontApp()).arg(GetRearApp()).
            arg(GetFrontEntry()).arg(GetRearEntry()).
            arg(dateTime.toString("hh:mm:ss.zzz")));
    
    bool bIsFrontOverlay = IsOverlayUse(GetFrontEntry());
    bool bIsRearOverlay = IsOverlayUse(GetRearEntry());

    GetLogger().Log( 
        QString("AppEngineBase::DoFrontFBTransition() afwfb [fo=%1][ro=%2][rfa=%3][rra=%4][rfe=%5][rre=%6]").
            arg(bIsFrontOverlay).arg(bIsRearOverlay).
            arg(GetRealAppId(DISPLAY_FRONT)).arg(GetRealAppId(DISPLAY_REAR)).
            arg(GetRealEntry(DISPLAY_FRONT)).arg(GetRealEntry(DISPLAY_REAR)));
    
    switch(reason) {
        case FB_TR_DO_CLONE:
            break;

        case FB_TR_DO_FRONT_CLONE: {
            if( bIsFrontOverlay ) {
                BlendOverlayFb(2, DualDisplayHelper::eDisplay_Primary);
                ChangeOverlay(DISPLAY_FRONT, OVERLAY_SPRITEC);
            }
            else {
                BlendOverlayFb(0, DualDisplayHelper::eDisplay_Primary);
                BlendOverlayFb(0, DualDisplayHelper::eDisplay_Secondary);
            }
            break;
        }
        
        case FB_TR_DO_REAR_CLONE: {
            break;
        }

        case FB_TR_DO_SWAP_DC: {
            //if( d_ptr->m_cloneMode == CLONE_ON_REAR ) {
                GetLogger().Log("AppEngineBase::DoFrontFBTransition() afwfb IsReverseCloned=true");
                if( bIsFrontOverlay ) {
                    if( bIsRearOverlay ) {
						if( GetRealAppId(DISPLAY_FRONT) == APP_CAMERA && IsVideoPlayerEntry(GetRealEntry(DISPLAY_REAR)) ) {
                            BlendOverlayFb(2, DualDisplayHelper::eDisplay_Primary, true);
							ChangeOverlay(DISPLAY_FRONT, SPRITEC_PLANE);
                            ChangeOverlay(DISPLAY_REAR, OVERLAY_PLANE);
                            BindOverlayFb(0, DualDisplayHelper::eDisplay_Primary);
                        }
                        else {
                            BlendOverlayFb(2, DualDisplayHelper::eDisplay_Primary, true);
                            ChangeOverlay(DISPLAY_REAR, SPRITEC_PLANE);
                        }
                        BlendOverlayFb(2, DualDisplayHelper::eDisplay_Primary, true);
                        ChangeOverlay(DISPLAY_FRONT, SPRITEC_PLANE);
                    }
                    else {
                        BlendOverlayFb(1, DualDisplayHelper::eDisplay_Primary, true);
						ChangeOverlay(DISPLAY_FRONT, OVERLAY_PLANE);
                        BindOverlayFb(0, DualDisplayHelper::eDisplay_Primary);
                        BlendOverlayFb(0, DualDisplayHelper::eDisplay_Secondary, true);
                    }
                }
                else {
                    if( bIsRearOverlay ) {
                        BlendOverlayFb(1, DualDisplayHelper::eDisplay_Secondary, true);
						ChangeOverlay(DISPLAY_REAR, OVERLAY_PLANE);
                        BindOverlayFb(0, DualDisplayHelper::eDisplay_Secondary);
                        BlendOverlayFb(0, DualDisplayHelper::eDisplay_Primary, true);
                    }
                    else {
                        BlendOverlayFb(0, DualDisplayHelper::eDisplay_Primary, true);
                        BlendOverlayFb(0, DualDisplayHelper::eDisplay_Secondary, true);
                    }
                }
            //}
            break;
        }
        
        case FB_TR_CLONED: {
            if( bIsFrontOverlay ) {
                BlendOverlayFb(2, DualDisplayHelper::eDisplay_Primary);
                ChangeOverlay(DISPLAY_FRONT, OVERLAY_SPRITEC);
            }
            else {
                BlendOverlayFb(0, DualDisplayHelper::eDisplay_Primary);
                BlendOverlayFb(0, DualDisplayHelper::eDisplay_Secondary);
            }
            break;
        }
        
        case FB_TR_NORMAL: {
            if( bIsFrontOverlay ) {
                if( bIsRearOverlay ) {
                    if( GetRealAppId(DISPLAY_FRONT) == APP_CAMERA && IsVideoPlayerEntry(GetRealEntry(DISPLAY_REAR)) ) {       
                        if( d_ptr->m_bIsSwapDCEnabled ) {
                            ChangeOverlay(DISPLAY_REAR, SPRITEC_PLANE);
                            ChangeOverlay(DISPLAY_FRONT, OVERLAY_PLANE);
                            BindOverlayFb(0, DualDisplayHelper::eDisplay_Primary);
                            BlendOverlayFb(1, DualDisplayHelper::eDisplay_Primary);
                        }
                        else {
                            ChangeOverlay(DISPLAY_FRONT, SPRITEC_PLANE);
                            ChangeOverlay(DISPLAY_REAR, OVERLAY_PLANE);
                            BindOverlayFb(0, DualDisplayHelper::eDisplay_Secondary);
                            BlendOverlayFb(1, DualDisplayHelper::eDisplay_Primary);
                        }
                    }
                    else {
                        ChangeOverlay(DISPLAY_FRONT, SPRITEC_PLANE);
                        ChangeOverlay(DISPLAY_REAR, OVERLAY_PLANE);
                        BindOverlayFb(0, DualDisplayHelper::eDisplay_Secondary);
                        BlendOverlayFb(1, DualDisplayHelper::eDisplay_Primary);
                    }
                }
                else {
                    BlendOverlayFb(1, DualDisplayHelper::eDisplay_Primary);
					ChangeOverlay(DISPLAY_FRONT, OVERLAY_PLANE);
                    BindOverlayFb(0, DualDisplayHelper::eDisplay_Primary);
                    BlendOverlayFb(0, DualDisplayHelper::eDisplay_Secondary);
                }
            }
            else {
#if 0
                if( GetThisAppId() == APP_STANDBY ) {
                    BlendOverlayFb(1, DualDisplayHelper::eDisplay_Primary);
                    ChangeOverlay(DISPLAY_FRONT, OVERLAY_PLANE);
                }
                else 
#endif
				if( bIsRearOverlay ) {
                    BlendOverlayFb(1, DualDisplayHelper::eDisplay_Secondary);
                    BlendOverlayFb(0, DualDisplayHelper::eDisplay_Primary);
                    ChangeOverlay(DISPLAY_REAR, OVERLAY_PLANE); // early set display setting before bind, because of flickering
                    BindOverlayFb(0, DualDisplayHelper::eDisplay_Secondary);
                    //ChangeOverlay(DISPLAY_REAR, OVERLAY_PLANE);
                }
                else {
                    BlendOverlayFb(0, DualDisplayHelper::eDisplay_Primary);
                }
            }
            break;
        }

        default:
            break;
    }
    GetLogger().Log( 
        QString("AppEngineBase::DoFrontFBTransition() afwfb end at %1").
            arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz")));
}

void
AppEngineBase::DoRearFBTransition(FB_TR_REASON reason)
{
    QDateTime	dateTime = QDateTime::currentDateTime();
    GetLogger().Log( 
        QString("AppEngineBase::DoRearFBTransition() afwfb [r=%1][fa=%2][ra=%3][fe=%4][re=%5] at %6").
            arg((int)reason).
            arg(GetFrontApp()).arg(GetRearApp()).
            arg(GetFrontEntry()).arg(GetRearEntry()).
            arg(dateTime.toString("hh:mm:ss.zzz")));
    
    bool bIsFrontOverlay = IsOverlayUse(GetFrontEntry());
    bool bIsRearOverlay = IsOverlayUse(GetRearEntry());

    GetLogger().Log( 
        QString("AppEngineBase::DoRearFBTransition() afwfb [fo=%1][ro=%2][rfa=%3][rra=%4][rfe=%5][rre=%6]").
            arg(bIsFrontOverlay).arg(bIsRearOverlay).
            arg(GetRealAppId(DISPLAY_FRONT)).arg(GetRealAppId(DISPLAY_REAR)).
            arg(GetRealEntry(DISPLAY_FRONT)).arg(GetRealEntry(DISPLAY_REAR)));
    
    switch(reason) {
        case FB_TR_DO_CLONE:
            break;

        case FB_TR_DO_FRONT_CLONE: {
            break;
        }
        
        case FB_TR_DO_REAR_CLONE: {
            if( bIsRearOverlay ) {
                BlendOverlayFb(2, DualDisplayHelper::eDisplay_Primary);
                ChangeOverlay(DISPLAY_REAR, OVERLAY_SPRITEC);
            }
            else {
                BlendOverlayFb(0, DualDisplayHelper::eDisplay_Primary);
                BlendOverlayFb(0, DualDisplayHelper::eDisplay_Secondary);
            }
            break;
        }

        case FB_TR_DO_SWAP_DC: {
            //if( d_ptr->m_cloneMode == CLONE_ON_FRONT ) {
                GetLogger().Log("AppEngineBase::DoRearFBTransition() afwfb IsFrontCloned=true");
                if( bIsFrontOverlay ) {
                    if( bIsRearOverlay ) {
                        if( GetRealAppId(DISPLAY_FRONT) == APP_CAMERA && IsVideoPlayerEntry(GetRealEntry(DISPLAY_REAR)) ) {
                            BlendOverlayFb(2, DualDisplayHelper::eDisplay_Primary, true);
                            ChangeOverlay(DISPLAY_REAR, SPRITEC_PLANE);
                            ChangeOverlay(DISPLAY_FRONT, OVERLAY_PLANE);
                            BindOverlayFb(0, DualDisplayHelper::eDisplay_Primary);
                        }
                        else {
                            BlendOverlayFb(2, DualDisplayHelper::eDisplay_Primary, true);
                            ChangeOverlay(DISPLAY_REAR, SPRITEC_PLANE);
                        }
                    }
                    else {
                        if( GetRearApp() == APP_IDLE && IsVideoPlayerEntry(GetFrontEntry()) ) {
                            BlendOverlayFb(1, DualDisplayHelper::eDisplay_Secondary, true);
							ChangeOverlay(DISPLAY_FRONT, OVERLAY_PLANE);
                            BindOverlayFb(0, DualDisplayHelper::eDisplay_Secondary);
                            BlendOverlayFb(0, DualDisplayHelper::eDisplay_Primary, true);
                            GetLogger().Log("afwfb : reverse blend & bind because the transition is done before swap");
                        }
                        else {
                            BlendOverlayFb(1, DualDisplayHelper::eDisplay_Primary, true);
						ChangeOverlay(DISPLAY_FRONT, OVERLAY_PLANE);
                            BindOverlayFb(0, DualDisplayHelper::eDisplay_Primary);
                            BlendOverlayFb(0, DualDisplayHelper::eDisplay_Secondary, true);
                        }
                    }
                }
                else {
                    if( bIsRearOverlay ) {
                        BlendOverlayFb(1, DualDisplayHelper::eDisplay_Secondary, true);
						ChangeOverlay(DISPLAY_REAR, OVERLAY_PLANE); 
                        BindOverlayFb(0, DualDisplayHelper::eDisplay_Secondary);
                        BlendOverlayFb(0, DualDisplayHelper::eDisplay_Primary, true);
                    }
                    else {
                        BlendOverlayFb(0, DualDisplayHelper::eDisplay_Primary, true);
                        BlendOverlayFb(0, DualDisplayHelper::eDisplay_Secondary, true);
                    }
                }
            //}
            break;
        }
        
        case FB_TR_CLONED: {
            if( bIsRearOverlay ) {
                BlendOverlayFb(2, DualDisplayHelper::eDisplay_Primary);
                ChangeOverlay(DISPLAY_REAR, OVERLAY_SPRITEC);
            }
            else {
                BlendOverlayFb(0, DualDisplayHelper::eDisplay_Primary);
                BlendOverlayFb(0, DualDisplayHelper::eDisplay_Secondary);
            }
            break;
        }
        
        case FB_TR_NORMAL: {
            if( bIsRearOverlay ) {
                if( bIsFrontOverlay ) {
                    if( GetRealAppId(DISPLAY_FRONT) == APP_CAMERA && IsVideoPlayerEntry(GetRealEntry(DISPLAY_REAR)) ) {       
                        if( d_ptr->m_bIsSwapDCEnabled ) {
                            ChangeOverlay(DISPLAY_REAR, SPRITEC_PLANE);
                            ChangeOverlay(DISPLAY_FRONT, OVERLAY_PLANE);
                            BindOverlayFb(0, DualDisplayHelper::eDisplay_Primary);
                            BlendOverlayFb(1, DualDisplayHelper::eDisplay_Secondary);
                        }
                        else {
                            ChangeOverlay(DISPLAY_FRONT, SPRITEC_PLANE);
                            ChangeOverlay(DISPLAY_REAR, OVERLAY_PLANE);
                            BindOverlayFb(0, DualDisplayHelper::eDisplay_Secondary);
                            BlendOverlayFb(1, DualDisplayHelper::eDisplay_Secondary);
                        }
                    }
                    else {
                        ChangeOverlay(DISPLAY_REAR, SPRITEC_PLANE);
                        ChangeOverlay(DISPLAY_FRONT, OVERLAY_PLANE);
                        BindOverlayFb(0, DualDisplayHelper::eDisplay_Primary);
                        BlendOverlayFb(1, DualDisplayHelper::eDisplay_Secondary);
                    }
                }
                else {
                    BlendOverlayFb(1, DualDisplayHelper::eDisplay_Secondary);
					ChangeOverlay(DISPLAY_REAR, OVERLAY_PLANE);
                    BindOverlayFb(0, DualDisplayHelper::eDisplay_Secondary);
                    BlendOverlayFb(0, DualDisplayHelper::eDisplay_Primary);
                }
            }
            else {
                if( bIsFrontOverlay ) {
                    BlendOverlayFb(1, DualDisplayHelper::eDisplay_Primary);
                    BlendOverlayFb(0, DualDisplayHelper::eDisplay_Secondary);
                    ChangeOverlay(DISPLAY_FRONT, OVERLAY_PLANE);  // early set display setting before bind, because of flickering
                    BindOverlayFb(0, DualDisplayHelper::eDisplay_Primary);
                    //ChangeOverlay(DISPLAY_FRONT, OVERLAY_PLANE);
                }
                else {
                    BlendOverlayFb(0, DualDisplayHelper::eDisplay_Secondary);
                }
            }
            break;
        }

        default:
            break;
    }
    GetLogger().Log( 
        QString("AppEngineBase::DoRearFBTransition() afwfb end at %1").
            arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz")));
}

void 
AppEngineBase::SetFrontFullScreen(bool bFullScreen)
{
    GetLogger().Log(QString("SetFrontFullScreen(%1").arg(bFullScreen));
    d_ptr->m_bFrontFullScreen = bFullScreen;
}

bool 
AppEngineBase::GetFrontFullScreen()
{
    return d_ptr->m_bFrontFullScreen;
}

void 
AppEngineBase::SetRearFullScreen(bool bFullScreen)
{
    GetLogger().Log(QString("SetRearFullScreen(%1").arg(bFullScreen));
    d_ptr->m_bRearFullScreen = bFullScreen;
}

bool 
AppEngineBase::GetRearFullScreen()
{
    return d_ptr->m_bRearFullScreen;
}

void 
AppEngineBase::UpdateStatusbarOnPopupEvent(Event &aEvent, bool bFullScreen)
{
    POPUP_SHOW_T* popupData = (POPUP_SHOW_T *)(aEvent.Data().data());
    switch(popupData->type) {
    case POPUP_MESSAGE_BOX:
        popupData->popup.msg_box.statusbar = !bFullScreen;
        break;
    case POPUP_TEXT:
        popupData->popup.text.statusbar = !bFullScreen;
        break;
    case POPUP_CUSTOM_TEXT:
        popupData->popup.custom_text.statusbar = !bFullScreen;
        break;
    case POPUP_WARNING:
        popupData->popup.text.statusbar = !bFullScreen;
        break;
    case POPUP_LIST:
        popupData->popup.list.statusbar = !bFullScreen;
        break;
    case POPUP_TOAST:
        popupData->popup.toast.statusbar = !bFullScreen;
        break;
    case POPUP_XM_ALERT:
        popupData->popup.xm_alert.statusbar = !bFullScreen;
        break;
    default:
        break;
    }

#if 0
    // temporary for debugging
    int statusbar = -1;
    popupData = (POPUP_SHOW_T *)(aEvent.Data().data());
    switch(popupData->type) {
    case POPUP_MESSAGE_BOX:
        statusbar = popupData->popup.msg_box.statusbar;
        break;
    case POPUP_TEXT:
        statusbar = popupData->popup.text.statusbar;
        break;
    case POPUP_CUSTOM_TEXT:
        statusbar = popupData->popup.custom_text.statusbar;
        break;
    case POPUP_WARNING:
        statusbar = popupData->popup.text.statusbar;
        break;
    case POPUP_LIST:
        statusbar = popupData->popup.list.statusbar;
        break;
    case POPUP_TOAST:
        statusbar = popupData->popup.toast.statusbar;
        break;
    case POPUP_XM_ALERT:
        statusbar = popupData->popup.xm_alert.statusbar;
        break;
    default:
        break;
    }
    GetLogger().Log(QString("statusbar = %1").arg(statusbar), Logger::PRIORITY_LOW);
#endif    
}

int
AppEngineBase::GetVersion( QString &strArgVersionInfo )
{
    GetLogger().Log( QString("--> %1").arg(__PRETTY_FUNCTION__) );
    
    int nIndex = 0;
    QString strVersionInfo;
    QStringList strlistVersionInfo;
    
    QFile fdSWVersion( "/etc/version.txt" );
    if ( !fdSWVersion.open( QIODevice::ReadOnly ) )
    {
        strArgVersionInfo.clear();
        strArgVersionInfo.append( "Can't open verison file" );
        return 0;
    }
    
    QTextStream rdStream( &fdSWVersion );
    
    for ( nIndex = 0 ; nIndex < eCVTotalCount ; nIndex++ )
    {
        strVersionInfo.clear(); 
        strVersionInfo = rdStream.readLine(); 
        
        // 동일 Variant enum 값을 조회 
        if ( stVariantString[nIndex].eCV == GetCountryVariant() )
        {
            strlistVersionInfo = strVersionInfo.split(".");

            if ( stVariantString[nIndex].strCV.compare( strlistVersionInfo[1] ) != 0 &&     // SOP 이전버전에서는 국가구분코드가 2자, SOP 버전은 3자를 사용.. 
                 stVariantString[nIndex].strSOPCV.compare( strlistVersionInfo[1] ) != 0 )
            {
                strVersionInfo.clear();
                strVersionInfo.append( "Out of Variant!!" );
            }
            
            break;
        }
        else
            continue;
    }
    
    fdSWVersion.close();
    
    GetLogger().Log( QString("<-- %1").arg(__PRETTY_FUNCTION__) );
    
    strArgVersionInfo.clear();
    if ( nIndex < eCVTotalCount )
    {
        strArgVersionInfo.append( strVersionInfo );
        return 1;
    }
    else
    {
        strArgVersionInfo.append( "Out of Variant!!" );
        return 0; 
    }
}
