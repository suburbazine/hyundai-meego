#include <DHAVN_AppSettings_AppEngine.h>
#include <DHAVN_AppSettings_Shared.h>
#include <DHAVN_AppMediaPlayer_Shared.h>
#include <persistman.h>
//#include <athena.h>

// Qt Header
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QVariant>
#include <QTime>
#include <QDBusConnection>
#include <QDebug>
#include <QApplication>
#include <QDBusReply>

// DH Header
#include <DHAVN_AppClock_Shared.h>
#include <DHAVN_AppFileManager_Shared.h>
#include <DHAVN_AppFramework_Logger_Def.h>
#include <uistate_manager_def.h>

// Local Header
#include "DHAVN_AppSettings_CMOSTManager.h"
#include "DHAVN_AppSetting_Log.h"



//From MOST LLD
#define UNIT_HEAD_UNIT          0x01
#define UNIT_REAR_SCREEN        0x02
#define VIDEO_SHOW              1
#define VIDEO_HIDE              0
#define INVALID_TIMEDATE_FORMAT     0XFF

SettingsAppEngine::SettingsAppEngine( ECountryVariant variant, QTranslator *translator, LANGUAGE_T language, QObject *pParent)
    : AppVextEngineQML( false, "AppSettings", pParent ) // opengl is disabled to make possible video stream to visible
    , m_pTranslator(translator)
    , m_nDisplay( SettingsDef::SETTINGS_DISPLAY_FRONT )
    , m_variantCounry(variant)
    , m_currentLanguage(language)
    , m_CloneStatus(CLONE_OFF)
    , m_bFrontScreenShowing(false)
    , m_bRearScreenShowing(false)
    , m_bDCSwapped(false)
    , m_bViewContentSwapped(false)

    , m_pShareDataProviderVideo( new SharedDataProvider( VIDEOPLAYER_SHARED_MEMORY_KEY ) )
    , m_pShareDataProviderDmb( new SharedDataProvider( MOBILETV_SHARED_MEMORY_KEY ) )
    , m_usbUpdatePresent(false)
    , m_pDBusinterface(NULL)
    , m_target(0)
    , m_FrontScreenBrightness(0)
    , m_Rear1ScreenBrightness(0)
    , m_RearScreenBrightness(0)
    , m_isVideoCloned(VIDEO_CLONE_NONE) // @TODO : delete it. not used.
    , m_pDDHelper(NULL)
    , m_isFrontScreenOff(false)
    , m_bIsGPSTimeSetting(false)
    , m_bIsGPSAvailable(false)
    , m_AppID_forDisPlaySetting(APP_NONE)
    , m_bJukeboxEnable(false)
    , m_bFG_F(false)
    , m_bFG_R(false)
    , m_frontLastEntryPoint(eAPP_NONE)
    , m_rearLastEntryPoint(eAPP_NONE)
    , m_isCloneMode(false)
    , m_bAccStatusOn(false)
{
//    athenaOpen();

    this->m_pIBoxTimer = new QTimer(this);
    this->m_pIBoxTimer->setSingleShot(true);
    this->m_pIBoxTimer->setInterval(200);
    connect(this->m_pIBoxTimer, SIGNAL(timeout()), SLOT(onTimeChangeTimeout()));

    this->m_timeFormat = INVALID_TIMEDATE_FORMAT;
    this->m_dateFormat = INVALID_TIMEDATE_FORMAT;
    memset(&this->m_lastChangedTime, 0, sizeof(SYSTEMTIME));

    getLcdStatusInfo();
    connectCarIFStatus();

    if(m_pDBusinterface==NULL)
    {
        m_pDBusinterface = new QDBusInterface(MICOM_MANAGER_QTDBUS_NAME,MICOM_MANAGER_HMI_PATH,
                                              "",
                                              QDBusConnection::sessionBus());
    }

    // Watch Dog
    signalConnection();
    setSignalHandlerObject(this);

    AskSysLastStatus();
}

SettingsAppEngine::~SettingsAppEngine()
{
    if(m_pDBusinterface)
    {
        delete m_pDBusinterface;
        m_pDBusinterface=NULL;
    }

    //athenaClose();
}
//added for DB File Crash Issue
void SettingsAppEngine::sendResponseFactoryReset()
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );
    Event event(EVT_FACTORY_RESET_RESPONSE, GetThisAppId(), APP_UI_MANAGER);
    if(! NotifyUISHCommon(event))
    {
        LOG2( QString("Factory Reset Failed."), Logger::PRIORITY_HI);
    }
    else
    {
        LOG2( QString("Factory Reset OK."), Logger::PRIORITY_HI);
    }
}

void
SettingsAppEngine::NotifyFromUIStateManager( Event aEvent )
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    NotifyFromUIStateManagerGeneric( aEvent, AppEngineQMLConstants::APP_FRONT_SCREEN );
}

void
SettingsAppEngine::NotifyFromUIStateManagerRear( Event aEvent )
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    NotifyFromUIStateManagerGeneric( aEvent, AppEngineQMLConstants::APP_REAR_SCREEN );
}

void
SettingsAppEngine::NotifyFromUIStateManagerGeneric( Event &aEvent,
                                                    AppEngineQMLConstants::EAPP_SCREEN_USAGE_T targetScreen )
{

    //LOG2( QString("[Generic]evt_ID:[%1], cmd:[%2]").arg(aEvent.GetEventId()).arg(aEvent.GetCommand()), Logger::PRIORITY_HI );
    switch ( aEvent.GetEventId() )
    {
    case EVT_POST_EVENT:
    {
        //LOG2 ( QString("EVT_POST_EVENT display = %1, command = %2").arg(targetScreen).arg(aEvent.GetCommand()), Logger::PRIORITY_HI );

        if(aEvent.GetCommand() == EVT_SETTINGS_CHANGED)
        {
            LOG2 ( QString("EVT_SETTINGS_CHANGED"), Logger::PRIORITY_HI );
            EVENT_SETTINGS_CHANGED_DATA_T *ChineseKeypadFromApp = (EVENT_SETTINGS_CHANGED_DATA_T *)aEvent.Data().data();

            if(ChineseKeypadFromApp->sKey == SettingsDef::DB_KEY_CHINA_KEYPAD)
            {
                //LOG2 ( QString("SettingsDef::DB_KEY_CHINA_KEYPAD. sValue:%1").arg(ChineseKeypadFromApp->sValue), Logger::PRIORITY_HI );
                emit chineKeypadChanged( ChineseKeypadFromApp->sValue );
            }
            else
                LOG2 ( QString("SettingsDef::DB_KEY_CHINA_KEYPAD. failed!!").arg(ChineseKeypadFromApp->sValue), Logger::PRIORITY_HI );
        }

        if ( aEvent.GetSource() == APP_FMGR || (aEvent.GetSource() == APP_AVP))
        {
            if ( aEvent.GetCommand() == EVT_JUKEBOX_CAPACITY_CHANGE )
            {
                LOG2 ( QString("EVT_JUKEBOX_CAPACITY_CHANGE"), Logger::PRIORITY_HI );
                emit sigJukeboxCapacityChanged(true);
            }

            /*
            else if ( aEvent.GetCommand() == -1 )
            {
                QString file( QString::fromUtf8(aEvent.Data().data()) );
                LOG2 ( QString("EVT_POST_EVENT waiting image path = %1").arg(file), Logger::PRIORITY_HI );
                file = QUrl(file).path();

                //DB Save
                emit saveSettingsStringSignal( file ,SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE, (int)targetScreen );
                NotifyApplication(SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE, -1, file, (int)targetScreen);
            }
            */
        }

        else if ( aEvent.GetSource() == APP_NAVIGATION )
        {
            if ( aEvent.GetCommand() == EVT_NAVI_GPS_TIME_SET && m_bIsGPSTimeSetting == true )
            {
                LOG2 ( QString("EVT_NAVI_GPS_TIME_SET"), Logger::PRIORITY_HI );
                EVENT_NAVI_GPS_TIME_DATA_T *GPSTimeFromNaviApp = (EVENT_NAVI_GPS_TIME_DATA_T *)aEvent.Data().data();
                if ( GPSTimeFromNaviApp )
                {
                    receiveGPSTime( GPSTimeFromNaviApp );
                }
            }
            else if ( aEvent.GetCommand() == EVT_NAVI_GPS_AVAILABLE_STATUS )
            {

                EVEVT_NAVI_GPS_AVAILABLE_DATA_T *GPSAvailableData = (EVEVT_NAVI_GPS_AVAILABLE_DATA_T *)aEvent.Data().data();
                if ( GPSAvailableData )
                {
                    //GetLogger().Log( QString( "EVT_NAVI_GPS_AVAILABLE_STATUS - %1" ).arg( GPSAvailableData->isGPSAvailable ) );
                    LOG2 ( QString("EVT_NAVI_GPS_AVAILABLE_STATUS - %1").arg( GPSAvailableData->isGPSAvailable ), Logger::PRIORITY_HI );
                    m_bIsGPSAvailable = GPSAvailableData->isGPSAvailable;
                    if (m_bIsGPSAvailable)
                    {
                        if (getGPSTimeSetting() == true)
                        {
                            requestGPSTimeToNavi();
                        }
                    }
                    //emit gpsAvailableChanged( m_bIsGPSAvailable );
                }
            }
        }
    }
        break;

    case EVT_DISPLAY_REAR_SCREEN_STATE:
    {
        LOG2 ( QString("EVT_DISPLAY_REAR_SCREEN_STATE display = %1").arg(targetScreen), Logger::PRIORITY_HI );

        EVT_DISPLAY_REAR_SCREEN_STATE_DATA_T *prearscreenstate = (EVT_DISPLAY_REAR_SCREEN_STATE_DATA_T *)aEvent.Data().data();

        if(prearscreenstate->sRearState == 0 || prearscreenstate->sRearState == 1)
        {
            emit SetLRearState(prearscreenstate->sRearState);
        }
        else
        {
            emit SetRRearState(prearscreenstate->sRearState);
        }
    }
        break;

    case EVT_REQUEST_PRE_FG:
    {
        LOG2 ( QString("EVT_REQUEST_PRE_FG display = %1").arg(targetScreen), Logger::PRIORITY_HI );

        currentTargetScreen = (int)targetScreen; //added for Rear Seat Control Spec Update

        if (targetScreen == AppEngineQMLConstants::APP_FRONT_SCREEN) m_bFG_F = true;
        else if (targetScreen == AppEngineQMLConstants::APP_REAR_SCREEN) m_bFG_R = true;

        if (!QDBusConnection::sessionBus().isConnected())
        {          
            LOG2( QString("D-Bus Sesstion Fail!"), Logger::PRIORITY_HI);
        }

        if(m_pDBusinterface==NULL)
        {
            m_pDBusinterface = new QDBusInterface(MICOM_MANAGER_QTDBUS_NAME,MICOM_MANAGER_HMI_PATH,
                                                  "",  QDBusConnection::sessionBus());
        }

        //EVENT_EVT_REQUEST_FG_SETTINGS_T *pSettingMode = (EVENT_EVT_REQUEST_FG_SETTINGS_T *)aEvent.Data().data();
        APP_ENTRY_POINT_DATA_T* pEntryPointData = NULL;
        pEntryPointData = (APP_ENTRY_POINT_DATA_T*)aEvent.Data().data();

        if ( pEntryPointData != NULL)
        {
            LOG2 ( QString("bTemporalMode = %1").arg(pEntryPointData->sHeader.bTemporalMode), Logger::PRIORITY_HI );

            // Last EntryPoint와 New EntryPoint 비교 (전석 및 후석) (==)

            if ( (targetScreen == AppEngineQMLConstants::APP_FRONT_SCREEN && m_frontLastEntryPoint == pEntryPointData->sAppEntryPoint) ||
                 (targetScreen == AppEngineQMLConstants::APP_REAR_SCREEN && m_rearLastEntryPoint == pEntryPointData->sAppEntryPoint) )
            {
                if (pEntryPointData->sHeader.bTemporalMode)
                {                    
                    LOG2( QString("L_Entry equal N_Entry. bTemp:true PRE_FG break!"), Logger::PRIORITY_HI);
                    // bTemporalMode일 경우 화면 업데이트 하지 않고 이벤트 종료.
                    break;
                }
            }
            else // Last EntryPoint와 New EntryPoint 비교 (전석 및 후석) (!=)
            {

                LOG2( QString("L_Entry != N_Entry. Update L_Entry!"), Logger::PRIORITY_HI);
                // Last EntryPoint에 New EntryPoint 및 현재화면(전,후석)을 저장
                setLastEntryPoint(pEntryPointData->sAppEntryPoint, targetScreen);
            }

            if (pEntryPointData->sAppEntryPoint != eAPP_SETTINGS_MAIN)
                emit resetScreen (targetScreen );

            SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_NoSystemBackground , false);

            //LOG2( QString("pEntryPointData->sAppEntryPoint =  %1").arg(pEntryPointData->sAppEntryPoint), Logger::PRIORITY_HI);
            switch (pEntryPointData->sAppEntryPoint)
            {
            case eAPP_SETTINGS_SOUND:
                LOG2 ( QString("eAPP_SETTINGS_SOUND display = %1").arg(targetScreen), Logger::PRIORITY_HI );
                emit setStartState( QString("sound"), QString(""), (int)targetScreen );
                emit videoStatusChanged((int)targetScreen, false);
                break;

            case eAPP_SETTINGS_SOUND_FROM_HOME: // eAPP_SETTINGS_SOUND_FROM_HOME
                LOG2 ( QString("eAPP_SETTINGS_SOUND_FROM_HOME display = %1").arg(targetScreen), Logger::PRIORITY_HI );
                emit setStartState( QString("sound"), QString(""), (int)targetScreen );
                emit videoStatusChanged((int)targetScreen, false);
                break;

            case eAPP_SETTINGS_SCREEN:
                LOG2 ( QString("eAPP_SETTINGS_SCREEN display = %1").arg(targetScreen), Logger::PRIORITY_HI );
                emit setStartState( QString("screen"), QString(""), (int)targetScreen );
                emit videoStatusChanged((int)targetScreen, false);
                break;

            case eAPP_SETTINGS_SCREEN_DISP: // eAPP_SETTINGS_SCREEN_DISP
                LOG2 ( QString("eAPP_SETTINGS_SCREEN_DISP display = %1").arg(targetScreen), Logger::PRIORITY_HI );
                emit setStartState( QString("screen"), QString(""), (int)targetScreen );
                emit videoStatusChanged((int)targetScreen, false);
                emit setStateLCD((int)targetScreen); // go LCD menu
                break;

            case eAPP_SETTINGS_VR:
                LOG2 ( QString("eAPP_SETTINGS_SCREEN_DISP display = %1").arg(targetScreen), Logger::PRIORITY_HI );
                emit setStartState( QString("voice"), QString(""), (int)targetScreen );
                emit videoStatusChanged((int)targetScreen, false);
                break;
            case eAPP_SETTINGS_GENERAL:
                LOG2 ( QString("eAPP_SETTINGS_GENERAL display = %1").arg(targetScreen), Logger::PRIORITY_HI );
                emit setStartState( QString("general"), QString(""), (int)targetScreen );
                emit videoStatusChanged((int)targetScreen, false);
                break;

            case eAPP_SETTINGS_SCREEN_AVP_DIVX: //235
            {
                LOG2 ( QString("eAPP_SETTINGS_SCREEN_AVP_DIVX display = %1").arg(targetScreen), Logger::PRIORITY_HI );

                SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_NoSystemBackground , true);

                m_AppID_forDisPlaySetting = APP_AVP;
                emit updateDisplaySettingUI((int)APP_AVP);
                emit setStartState( QString("screen"), QString(""), (int)targetScreen );
                emit videoStatusChanged((int)targetScreen, true);
            }
                break;

            case eAPP_SETTINGS_SCREEN_AVP_DVD:  //236
            {
                LOG2 ( QString("eAPP_SETTINGS_SCREEN_AVP_DVD display = %1").arg(targetScreen), Logger::PRIORITY_HI );

                SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_NoSystemBackground , true);

                m_AppID_forDisPlaySetting = APP_AVP;
                emit updateDisplaySettingUI((int)APP_AVP);
                emit setStartState( QString("screen"), QString(""), (int)targetScreen );
                emit videoStatusChanged((int)targetScreen, true);
            }
                break;

            case eAPP_SETTINGS_SCREEN_AVP_AUX : //237
            {
                LOG2 ( QString("eAPP_SETTINGS_SCREEN_AVP_AUX display = %1").arg(targetScreen), Logger::PRIORITY_HI );

                SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_NoSystemBackground , true);

                m_AppID_forDisPlaySetting = APP_AVP;
                emit updateDisplaySettingUI((int)APP_AVP);
                emit setStartState( QString("screen"), QString(""), (int)targetScreen );
                emit videoStatusChanged((int)targetScreen, true);
            }
                break;

            case eAPP_SETTINGS_SCREEN_MOBILETV:  //238
            {
                LOG2 ( QString("eAPP_SETTINGS_SCREEN_MOBILETV display = %1").arg(targetScreen), Logger::PRIORITY_HI );

                SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_NoSystemBackground , true);

                m_AppID_forDisPlaySetting = APP_MOBILETV;
                emit updateDisplaySettingUI((int)APP_MOBILETV);
                emit setStartState( QString("screen"), QString(""), (int)targetScreen );
                emit videoStatusChanged((int)targetScreen, true);
            }
                break;

            case eAPP_SETTINGS_GENERAL_KEYBOARD:
                LOG2 ( QString("eAPP_SETTINGS_GENERAL_KEYBOARD display = %1").arg(targetScreen), Logger::PRIORITY_HI );
                emit setStartState( QString("keypad"), QString("keypad"), (int)targetScreen );
                emit videoStatusChanged((int)targetScreen, false);
                break;

            case eAPP_SETTINGS_SYSTEM:
                LOG2 ( QString("eAPP_SETTINGS_SYSTEM display = %1").arg(targetScreen), Logger::PRIORITY_HI );
                emit setStartState( QString("system"), QString(""), (int)targetScreen );
                emit videoStatusChanged((int)targetScreen, false);
                break;

            case eAPP_SETTINGS_GENERAL_CLOCK:
                LOG2 ( QString("eAPP_SETTINGS_GENERAL_CLOCK display = %1").arg(targetScreen), Logger::PRIORITY_HI );
                emit setStartState( QString("clock"), QString(""), (int)targetScreen );
                emit videoStatusChanged((int)targetScreen, false);
                break;
            }

            emit showStartFocus( pEntryPointData->sHeader.sIsJogEnabled, (int)targetScreen );
            emit checkAspectRatioOnFG( (int)targetScreen );

            // check if it's new-show event during clone is active ---
            if (targetScreen == AppEngineQMLConstants::APP_FRONT_SCREEN)
            {
                if (m_CloneStatus == CLONE_ON_FRONT && !m_bFrontScreenShowing )
                {
                    // set rear-screen-entry-point with front-screen-entry-point : front entry > rear entry
                }
                m_bFrontScreenShowing = true; // update it here after checking is done above
            }
            else if (targetScreen == AppEngineQMLConstants::APP_REAR_SCREEN)
            {
                if (m_CloneStatus == CLONE_ON_REAR && !m_bRearScreenShowing )
                {
                    // set front-screen-entry-point with rear-screen-entry-point : rear entry > front entry
                }
                m_bRearScreenShowing = false; // update it here after checking is done above
            }
        }

        QDeclarativeView* pView =  GetDeclarativeView(targetScreen);
        if (pView) pView->repaint();

        //LOG2 ( QString("athenaSendObject()").arg(targetScreen), Logger::PRIORITY_HI );
        //athenaSendObject();

        break;
    }

    case EVT_REQUEST_BG:
    {
        LOG2 ( QString("EVT_REQUEST_BG display = %1").arg(targetScreen), Logger::PRIORITY_HI );

        if (targetScreen == AppEngineQMLConstants::APP_FRONT_SCREEN) m_bFG_F = false;
        else if (targetScreen == AppEngineQMLConstants::APP_REAR_SCREEN) m_bFG_R = false;

        // keep show status ---
        if ( targetScreen == AppEngineQMLConstants::APP_FRONT_SCREEN )
        {
            m_bFrontScreenShowing = false;
        }
        else
        {
            m_bRearScreenShowing = false;
        }
        // ~~~ keep show status


        //setTimePickerPopupStatus( false );
        break;
    }

    case EVT_REQUEST_FG:
    {
        LOG2 ( QString("EVT_REQUEST_FG"), Logger::PRIORITY_HI );

        switch(targetScreen)
        {
        case AppEngineQMLConstants::APP_FRONT_SCREEN:
        {
            if( m_frontLastEntryPoint == eAPP_SETTINGS_SCREEN_MOBILETV )
            {
                NotifyFGReceivedToMobileTV();
            }
            break;
        }

        case AppEngineQMLConstants::APP_REAR_SCREEN:
        {
            if( m_rearLastEntryPoint == eAPP_SETTINGS_SCREEN_MOBILETV )
            {
                NotifyFGReceivedToMobileTV();
            }
            break;
        }
        }

        break;
    }

    case EVT_MMC_MOUNTED:
        LOG2 ( QString("EVT_MMC_MOUNTED"), Logger::PRIORITY_HI );
        emit sigNaviSdcardMounted();
        break;

    case EVT_MMC_REMOVED:
        LOG2 ( QString("EVT_MMC_REMOVED"), Logger::PRIORITY_HI );
        emit sigNaviSdcardRemoved();
        break;

    case EVT_JUKEBOX_ENABLED :
    {
        //LOG2 ( QString("EVT_JUKEBOX_ENABLED"), Logger::PRIORITY_HI );

        if ( sizeof(EVT_COMMON_BASE_EVENT_DATA_T) == aEvent.Data().size() )
        {
            EVT_COMMON_BASE_EVENT_DATA_T* pData = (EVT_COMMON_BASE_EVENT_DATA_T*)aEvent.Data().data();
            LOG2 ( QString("EVT_JUKEBOX_ENABLED jukebox image = %1").arg(pData->iEventData01), Logger::PRIORITY_HI );
            //LOG2 ( QString("EVT_JUKEBOX_ENABLED iEventData02 = %1").arg(pData->iEventData02), Logger::PRIORITY_HI );
            //LOG2 ( QString("EVT_JUKEBOX_ENABLED iEventData03 = %1").arg(pData->iEventData03), Logger::PRIORITY_HI );
            //LOG2 ( QString("EVT_JUKEBOX_ENABLED iEventData04 = %1").arg(pData->iEventData04), Logger::PRIORITY_HI );
            //LOG2 ( QString("EVT_JUKEBOX_ENABLED enter"), Logger::PRIORITY_HI );

            m_bJukeboxEnable = pData->iEventData01;
            JukeboxEnable = m_bJukeboxEnable;
            emit sigJukeboxImageInfo(pData->iEventData01);
        }
        else
        {
            LOG2 ( QString("event data error"), Logger::PRIORITY_HI );
        }
    }
        break;
    case EVT_CAMERA_ENABLE :
        LOG2 ( QString("EVT_CAMERA_ENABLE"), Logger::PRIORITY_HI );
        emit sigCameraOnOff((int)aEvent.GetCommand(), 1);
        break;

    case EVT_CAMERA_DISABLE :
        LOG2 ( QString("EVT_CAMERA_DISABLE"), Logger::PRIORITY_HI );
        emit sigCameraOnOff((int)aEvent.GetCommand(), 0);
        break;


        /*
    case EVT_POPUP_RESPONSE:
        LOG2 ( QString("EVT_POPUP_RESPONSE"), Logger::PRIORITY_HI );
        break;
        */
    default:
        break;
    }
}

void
SettingsAppEngine::NotifyFromUIStateManagerCommon( Event aEvent)
{

    //LOG2( QString("[Common]evt_ID:[%1], cmd:[%2]").arg(aEvent.GetEventId()).arg(aEvent.GetCommand()), Logger::PRIORITY_HI );
    switch(aEvent.GetEventId())
    {

    case EVT_FACTORY_RESET_REQUEST:
    {
        LOG2 (QString("Factory_Reset From UISH"), Logger::PRIORITY_HI);
        factory = true; // add for its 243706

        emit sigFactoryReset();

        //        Event event(EVT_FACTORY_RESET_RESPONSE, GetThisAppId(), APP_UI_MANAGER);
        //        if(! NotifyUISHCommon(event))
        //        {
        //            LOG2( QString("Factory Reset Response Failed"), Logger::PRIORITY_HI);
        //        }
        break;
    }

    case EVT_CLONE_NOTIFICATION:
    {
        EVENT_CLONE_NOTIFICATION_DATA_T *pCloneData = (EVENT_CLONE_NOTIFICATION_DATA_T *)aEvent.Data().data();
        if (pCloneData != NULL)
        {
            if (pCloneData->sCloneStatus == CLONE_ON_FRONT || pCloneData->sCloneStatus == CLONE_ON_REAR) {
                m_isCloneMode = true;
            }
            else if (pCloneData->sCloneStatus == CLONE_OFF) {
                m_isCloneMode = false;
                emit setTouchLock(false);
            }

            notifyIsParkingMode();

            LOG2 (QString("EVT_CLONE_NOTIFICATION CloneMode(%1)").arg(m_isCloneMode), Logger::PRIORITY_HI);
        }
        break;
    }

    case EVT_USB_MOUNTED:
    {
        LOG2 ( QString("EVT_USB_MOUNTED"), Logger::PRIORITY_HI );

        APP_EVENT_USB_DATA_T eventData;
        if (sizeof(APP_EVENT_USB_DATA_T) == aEvent.Data().size())
        {
            memcpy(&eventData, aEvent.Data().data(), sizeof(APP_EVENT_USB_DATA_T));
            if (( eventData.contentmask & USB_DATA_CONTENT_UPDATE) || (eventData.contentmask & USB_DATA_CONTENT_UPDATEAPP ))
            {
                m_usbUpdatePresent = true;
                emit usbUpdatePresentChanged(m_usbUpdatePresent);
            }
        }
        break;
    }

    case EVT_USB_REMOVED:
    {
        LOG2 ( QString("EVT_USB_REMOVED"), Logger::PRIORITY_HI );

        m_usbUpdatePresent = false;
        emit usbUpdatePresentChanged(m_usbUpdatePresent);
        break;
    }

    case EVT_LANGUAGE_CHANGED:
    {
        LOG2 ( QString("EVT_LANGUAGE_CHANGED"), Logger::PRIORITY_HI );

        EVENT_LANGUAGE_CHANGED_DATA_T *pLanguageData = (EVENT_LANGUAGE_CHANGED_DATA_T *)aEvent.Data().data();

        if ( pLanguageData != NULL )
        {
            emit saveSettingsSignal((int) pLanguageData->languageId, SettingsDef::DB_KEY_LANGUAGE_TYPE, NULL);
            SetLocalization(pLanguageData->languageId);

            sendLanguageToIBox( pLanguageData->languageId, AppEngineQMLConstants::APP_FRONT_SCREEN );
        }
    }
        break;
    case EVT_KEY_HU_BACK:
    case EVT_KEY_CCP_BACK:
    {
        LOG2 ( QString("EVT_KEY_CCP_BACK"), Logger::PRIORITY_HI );

        if (m_bFG_F && !m_bFG_R)
            KeyEventHandler( SettingsDef::SETTINGS_EVT_KEY_BACK, aEvent, AppEngineQMLConstants::APP_FRONT_SCREEN, DISPLAY_FRONT );
        else if (!m_bFG_F && m_bFG_R)
            KeyEventHandler( SettingsDef::SETTINGS_EVT_KEY_BACK, aEvent, AppEngineQMLConstants::APP_REAR_SCREEN, DISPLAY_FRONT );
        else if (m_bFG_F && m_bFG_R)
            KeyEventHandler( SettingsDef::SETTINGS_EVT_KEY_BACK, aEvent, AppEngineQMLConstants::APP_FRONT_SCREEN, DISPLAY_FRONT );

        break;
    }
    case EVT_KEY_RRC_BACK:
    {
        LOG2 ( QString("EVT_KEY_RRC_BACK"), Logger::PRIORITY_HI );

        if (m_bFG_F && !m_bFG_R)
            KeyEventHandler( SettingsDef::SETTINGS_EVT_KEY_BACK, aEvent, AppEngineQMLConstants::APP_FRONT_SCREEN, DISPLAY_REAR );
        else if (!m_bFG_F && m_bFG_R)
            KeyEventHandler( SettingsDef::SETTINGS_EVT_KEY_BACK, aEvent, AppEngineQMLConstants::APP_REAR_SCREEN, DISPLAY_REAR );
        else if (m_bFG_F && m_bFG_R)
            KeyEventHandler( SettingsDef::SETTINGS_EVT_KEY_BACK, aEvent, AppEngineQMLConstants::APP_REAR_SCREEN, DISPLAY_REAR );

        break;
    }

    case EVT_KEY_RRC_JOG_DIAL_UP:
    case EVT_KEY_RRC_JOG_DIAL_DOWN:
    case EVT_KEY_RRC_JOG_DIAL_LEFT:
    case EVT_KEY_RRC_JOG_DIAL_RIGHT:
    case EVT_KEY_RRC_JOG_DIAL_CENTER:
    {
        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
        if (message->keyStatus == KEY_STATUS_PRESSED ) {
            if ( m_isCloneMode ) {
                LOG2( QString("RRC jog pressed:SetTouchLock(true)"), Logger::PRIORITY_HI );
                emit setTouchLock(true);
            }
        } else if (message->keyStatus == KEY_STATUS_RELEASED || message->keyStatus == KEY_STATUS_CANCEL ) {
            LOG2( QString("RRC jog released:SetTouchLock(false)"), Logger::PRIORITY_HI );
            emit setTouchLock(false);
        }

        break;
    }

    case EVT_NAVI_SETTTING_UNIT:
    {
        LOG2 ( QString("EVT_NAVI_SETTTING_UNIT"), Logger::PRIORITY_HI );

        EVENT_NAVI_SETTING_UNIT_DATA_T eventData;
        if (sizeof(EVENT_NAVI_SETTING_UNIT_DATA_T) == aEvent.Data().size())
        {
            memcpy(&eventData, aEvent.Data().data(), sizeof(EVENT_NAVI_SETTING_UNIT_DATA_T));
            emit saveSettingsSignal( eventData.Unit, SettingsDef::DB_KEY_DISTANCE_UNIT, NULL );
            //emit saveSettingsSignal( eventData.Unit, SettingsDef::DB_KEY_DISTANCE_UNIT, (int)targetScreen );

            this->SendDistanceUnit(eventData.Unit);
        }
        else
        {
            LOG2 ( QString("event data error"), Logger::PRIORITY_HI );
        }
        break;
    }

    case EVT_JUKEBOX_ENABLED :
    {
        //LOG2 ( QString("EVT_JUKEBOX_ENABLED"), Logger::PRIORITY_HI );

        if ( sizeof(EVT_COMMON_BASE_EVENT_DATA_T) == aEvent.Data().size() )
        {
            EVT_COMMON_BASE_EVENT_DATA_T* pData = (EVT_COMMON_BASE_EVENT_DATA_T*)aEvent.Data().data();
            LOG2 ( QString("EVT_JUKEBOX_ENABLED jukebox image = %1").arg(pData->iEventData01), Logger::PRIORITY_HI );
            m_bJukeboxEnable = pData->iEventData01;
            JukeboxEnable = m_bJukeboxEnable;
            emit sigJukeboxImageInfo(pData->iEventData01);
        }
        else
        {
            LOG2 ( QString("event data error"), Logger::PRIORITY_HI );
        }
    }
        break;

    case EVT_CAMERA_ENABLE :
        LOG2 ( QString("EVT_CAMERA_ENABLE"), Logger::PRIORITY_HI );
        emit sigCameraOnOff((int)aEvent.GetCommand(), 1);
        break;

    case EVT_CAMERA_DISABLE :
        LOG2 ( QString("EVT_CAMERA_DISABLE"), Logger::PRIORITY_HI );
        emit sigCameraOnOff((int)aEvent.GetCommand(), 0);
        break;



    case EVT_SWAP_DC_ENABLED:
        LOG2 ( QString("EVT_SWAP_DC_ENABLED"), Logger::PRIORITY_HI );
        setDcSwapped(true);
        notifyIsParkingMode();
        break;

    case EVT_SWAP_DC_DISABLED:
        LOG2 ( QString("EVT_SWAP_DC_DISABLED"), Logger::PRIORITY_HI );
        setDcSwapped(false);
        notifyIsParkingMode();
        break;

    case EVT_DRS_SHOW:
        LOG2 ( QString("EVT_DRS_SHOW"), Logger::PRIORITY_HI );
        setParkingStatus(false);
        notifyIsParkingMode();
        break;

    case EVT_DRS_HIDE:
        LOG2 ( QString("EVT_DRS_HIDE"), Logger::PRIORITY_HI );
        setParkingStatus(true);
        notifyIsParkingMode();
        break;

        // add for backup DB -->
    case EVT_REQUEST_SAVE_UNSAVED_DATA:
        LOG2 ( QString("EVT_REQUEST_SAVE_UNSAVED_DATA"), Logger::PRIORITY_HI );
        emit saveSettingsSignal( glo_balance, SettingsDef::DB_KEY_SOUND_BALANCE, 0 ); // add for ISV 100925
        emit saveSettingsSignal( glo_fade, SettingsDef::DB_KEY_SOUND_FADER, 0 ); // add for ISV 100925
        emit saveSettingsSignal( glo_high, SettingsDef::DB_KEY_SOUND_HIGHTONE, 0 ); // add for ISV 100925
        emit saveSettingsSignal( glo_mid, SettingsDef::DB_KEY_SOUND_MIDTONE, 0 ); // add for ISV 100925
        emit saveSettingsSignal( glo_low, SettingsDef::DB_KEY_SOUND_LOWTONE, 0 ); // add for ISV 100925
        emit saveSettingsSignal( glo_front_bright, SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS, 1 ); // add for ISV 100925
        emit saveSettingsSignal( glo_rear_bright, SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS, 0 ); // add for ISV 100925
        system("rm -f /app/data/app_settings/SettingsScreenSettings_backup.db");
        usleep(50000); // microsecond
        system("cp /app/data/app_settings/SettingsScreenSettings.db /app/data/app_settings/SettingsScreenSettings_backup.db");
        usleep(50000);
        system("sync");
        usleep(50000);
        //LOG2 ( QString("CP Backup DB file !!!"), Logger::PRIORITY_HI );
        break;
        // add for backup DB <--
    }
}

void
SettingsAppEngine::HandleBackKey( int screen )
{
    LOG2 ( QString("screen = %1").arg(screen), Logger::PRIORITY_HI );

    DISPLAY_T target;

    if (screen == 0)  // Back Button By Touch
    {
        target = DISPLAY_FRONT;

        Event event( EVT_TOUCH_BACK_KEY, GetThisAppId(), APP_UI_MANAGER );

        if ( !NotifyUISH(event, target) )
        {

            LOG2(QString("HandleBackKey:notiFailed!"), Logger::PRIORITY_HI);
        }
    }
    else // Back Button By CCP or RRC
    {
        if ( screen == AppEngineQMLConstants::APP_FRONT_SCREEN )
        {
            target = DISPLAY_FRONT;
        }
        else
        {
            target = DISPLAY_REAR;
        }

        Event event( EVT_KEY_SOFT_BACK, GetThisAppId(), APP_UI_MANAGER );

        if ( !NotifyUISH(event, target) )
        {
            LOG2(QString("HandleBackKey:notiFailed!"), Logger::PRIORITY_HI);
        }
    }

}

// following methods will be invoked from QML, when applucation wishes to change language
void
SettingsAppEngine::ChangeLanguage(int language)
{
    LOG2 ( QString("language = %1").arg(language), Logger::PRIORITY_HI );

    EVENT_LANGUAGE_CHANGED_DATA_T message;
    message.header.appIdReciever = APP_NONE;
    message.header.reasonEvent = EVT_LANGUAGE_CHANGED;
    message.languageId = (LANGUAGE_T)language;

    SetLocalization(message.languageId);

    QByteArray payload( (char *)&message, sizeof(EVENT_LANGUAGE_CHANGED_DATA_T) );

    Event event( EVT_LANGUAGE_CHANGED, GetThisAppId(), APP_ALL, payload );

    if ( !NotifyUISHCommon(event) )
    {
        LOG2 ( QString("failed NotifyUISHCommon").arg(language), Logger::PRIORITY_HI );
    }
}

void SettingsAppEngine::ResetAfterCloneCompleted()
{
    LOG2 ( QString("RACC"), Logger::PRIORITY_HI );

    m_isVideoCloned = VIDEO_CLONE_NONE;
}

void SettingsAppEngine::NotifyVideoDefaultSetAck( uint deviceID, ushort senderHandle, SettingsDef::SETTINGS_VIDEO_DEFAULT_SET_ACK_T ackType )
{
    //LOG2 ( QString("NVDS"), Logger::PRIORITY_HI );
    LOG2(QString("NVDS:D_ID:[%1],Handle:[%2],Ack:[%3]").arg(deviceID).arg(senderHandle).arg(ackType), Logger::PRIORITY_LOW);

    //GetLogger().Log( QString( "SettingsAppEngine::NotifyVideoDefaultSetAck: Value - Device ID: %1, Handle: %2, Ack T: %3" )
    //                 .arg(deviceID).arg(senderHandle).arg(ackType) );

    if(m_pDBusinterface->isValid())
    {
        //GetLogger().Log( "m_pDBusinterface is valid" );
        uint uiAckState = ackType;
        ushort usSenderHandle = senderHandle;
        uint uiDeviceID = 0x501;

        m_DBusreply = m_pDBusinterface->call("SetVideoDefaultResultIBOX", uiAckState, deviceID ,usSenderHandle);
        //m_DBusreply = m_pDBusinterface->call("SetVideoDefaultResultIBOX", uiAckState, uiDeviceID ,usSenderHandle);
    }
}

void SettingsAppEngine::setRTCInfoToMicom( SYSTEMTIME *time )
{
    LOG2 ( QString("RTCToMicom"), Logger::PRIORITY_HI );

    const char data[7] = { 0,0,0,0,0,0,0 };
    QByteArray barray = QByteArray( data, 7 );


    barray[0] = 0x00;
    barray[1] = time->wYear - 1900;
    barray[2] = time->wMonth;
    barray[3] = time->wDay;
    barray[4] = time->wHour;
    barray[5] = time->wMinute;
    barray[6] = time->wSecond;

    if (  QDBusConnection::sessionBus().isConnected() )
    {
        QDBusInterface MicomRTCInterface( "com.lge.car.micom","/System", "com.lge.car.micom.system" );

        MicomRTCInterface.call("LgeDBus_SetHwRTCSet", barray);

        //GetLogger().Log(QString(" SetRTCTime micom method done --->"));
    }

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

void SettingsAppEngine::requestGPSTimeToNavi()
{
    LOG2 ( QString("GPSTimeToNavi."), Logger::PRIORITY_HI );

    Event event(EVT_POST_EVENT, GetThisAppId(), APP_NAVIGATION, EVT_NAVI_GPS_TIME_GET );
    NotifyUISH( event );
}

void SettingsAppEngine::NotifyFGReceivedToMobileTV()
{
    LOG2 ( QString("NFGToMobileTv"), Logger::PRIORITY_HI );

    Event event(EVT_POST_EVENT, GetThisAppId(), APP_MOBILETV, EVT_SETTING_FG_RECEIVED );
    NotifyUISH( event );

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}


void SettingsAppEngine::NotifyApplication(int skey, int sValue, QString sValue2, int targetScreen)
{
    LOG2 ( QString("[NotiApp]Key: %1, Value: %2, Str Value: %3, Screen: %4").arg(skey).
           arg(sValue).arg(sValue2).arg(targetScreen), Logger::PRIORITY_HI );

    // Notify Display Setting Data to Request App  ---->>
    if (skey == SettingsDef::DB_KEY_VIDEO_BRIGHTNESS ||
            skey == SettingsDef::DB_KEY_VIDEO_SATURATION ||
            skey == SettingsDef::DB_KEY_VIDEO_HUE ||
            skey == SettingsDef::DB_KEY_VIDEO_CONTRAST )
    {
        NotifyApplication(skey, sValue, sValue2, targetScreen, APP_AVP);
        NotifyApplication(skey, sValue, sValue2, targetScreen, APP_MOBILETV);
        //NotifyApplication(skey, sValue, sValue2, targetScreen, APP_INTERACTIVE_GUIDE);
        return;
    }

    //if(skey == SettingsDef::DB_KEY_SUMMER_TIME)
    //    qDebug()<<"NotifyApplication::SettingsDef::DB_KEY_SUMMER_TIME::sValue:"<<sValue<<"sValue2:"<<sValue2;
    // <<------ Notify Display Setting Data to Request App

    //for Rear Screen Lock
    if (skey == SettingsDef::DB_KEY_LOCKREARMONITOR_FUNCTION)
    {
        if (sValue) emit sendRearScreenLockStatus(true);
        else        emit sendRearScreenLockStatus(false);

        return;
    }

    // add for ISV 100925
    if (skey == SettingsDef::DB_KEY_SOUND_FADER)
    {
        glo_fade = sValue;
    }
    if (skey == SettingsDef::DB_KEY_SOUND_BALANCE)
    {
        glo_balance = sValue;
    }
    if (skey == SettingsDef::DB_KEY_SOUND_HIGHTONE)
    {
        glo_high = sValue;
    }
    if (skey == SettingsDef::DB_KEY_SOUND_MIDTONE)
    {
        glo_mid = sValue;
    }
    if (skey == SettingsDef::DB_KEY_SOUND_LOWTONE)
    {
        glo_low = sValue;
    }
    if (skey == SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS)
    {
        glo_front_bright = sValue;
    }
    if (skey == SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS)
    {
        glo_rear_bright = sValue;
    } // add for ISV 100925


    EVENT_SETTINGS_CHANGED_DATA_T message;
    message.sHeader.appIdReciever = APP_NONE;
    message.sHeader.reasonEvent = EVT_SETTINGS_CHANGED;

    message.sKey = skey;
    message.sValue = sValue;
    if( m_isVideoCloned == VIDEO_CLONE_NONE )
    {
        message.sSourceDisplay = OVERLAY_PLANE;
    }
    else if( m_isVideoCloned == VIDEO_CLONE_BEGIN )
    {
        message.sSourceDisplay = OVERLAY_SPRITEC;
    }
    else
    {
        message.sSourceDisplay = SPRITEC_PLANE;
    }

    strncpy( message.sValue2, sValue2.toUtf8().data(), PATH_MAX_LENGTH );  //fix coverity

    QByteArray payload( (char *)&message, sizeof(EVENT_SETTINGS_CHANGED_DATA_T) );

    Event event( EVT_SETTINGS_CHANGED, GetThisAppId(), APP_ALL, payload );

    if ( !NotifyUISHCommon(event) )
    {
        LOG2(QString("ChangeSet:Evt Fail!"), Logger::PRIORITY_HI);
        //qDebug("SettingsAppEngine::ChangeSettings: Event notification failed");
    }
    else
    {
        LOG2(QString("ChangeSet:Evt sent!"), Logger::PRIORITY_HI);
        //qDebug("SettingsAppEngine::ChangeSettings: Event notification sent");
    }
}

// App_AVP & App_DMB
void SettingsAppEngine::NotifyApplication(int skey, int sValue, QString sValue2, int targetScreen, APP_ID_T targetAppID)
{
    //GetLogger().Log( "SettingsAppEngine::ChangeSettings: wishes to change settings" );
    //GetLogger().Log( QString("Key: %1, Value: %2, Str Value: %3, Screen: %4, targetAppID: %5").arg(skey).
    //                 arg(sValue).arg(sValue2).arg(targetScreen).arg(targetAppID));
    LOG2(QString("[NotiApp]AVP&DMB:Key:%1, Val:%2, Str:%3").arg(skey).arg(sValue).arg(sValue2), Logger::PRIORITY_HI);
    LOG2(QString("[NotiApp]AVP&DMB:Screen:%1, targetAppID:%2").arg(targetScreen).arg(targetAppID), Logger::PRIORITY_HI);

    /*
    if (skey == SettingsDef::DB_KEY_VIDEO_BRIGHTNESS)
    {
        qDebug() << "NotifyApplication::APP_ALL(), Key :: SettingsDef::DB_KEY_VIDEO_BRIGHTNESS, Value :: "<< sValue;
    }
    else if(skey == SettingsDef::DB_KEY_VIDEO_CONTRAST)
    {
        qDebug() << "NotifyApplication::APP_ALL(), Key :: SettingsDef::DB_KEY_VIDEO_CONTRAST, Value :: "<< sValue;
    }
    else if(skey == SettingsDef::DB_KEY_VIDEO_HUE)
    {
        qDebug() << "NotifyApplication::APP_ALL(), Key :: SettingsDef::DB_KEY_VIDEO_HUE, Value :: "<< sValue;
    }
    else if(skey == SettingsDef::DB_KEY_VIDEO_SATURATION)
    {
        qDebug() << "NotifyApplication::APP_ALL(), Key :: SettingsDef::DB_KEY_VIDEO_SATURATION, Value :: "<< sValue;
    }
    */

    EVENT_SETTINGS_CHANGED_DATA_T message;
    message.sHeader.appIdReciever = targetAppID;
    message.sHeader.reasonEvent = EVT_SETTINGS_CHANGED;

    message.sKey = skey;
    message.sValue = sValue;

    QByteArray payload( (char *)&message, sizeof(EVENT_SETTINGS_CHANGED_DATA_T) );

    Event event( EVT_SETTINGS_CHANGED, GetThisAppId(), targetAppID, payload );
    
    if ( !NotifyUISHCommon(event) )
    {
        //qDebug("SettingsAppEngine::ChangeSettings: Event notification failed");
        LOG2(QString("[NotiApp]EVT Fail!"), Logger::PRIORITY_HI);
    }
    else
    {
        //qDebug("SettingsAppEngine::ChangeSettings: Event notification sent");
        LOG2(QString("[NotiApp]EVT sent!"), Logger::PRIORITY_LOW);
    }
}

void
SettingsAppEngine::LaunchListFM(int targetScreen)
{
    LOG2 ( QString("L_ListFM targetScreen = %1").arg(targetScreen), Logger::PRIORITY_HI );

    AppFileManagerStartParameter param;
    strncpy(param.currentPlayFileName, "", FILE_PATH_SIZE - 1);  //fix coverity
    strncpy(param.folderPath, "", FILE_PATH_SIZE - 1);           //fix coverity

    param.startState = eStartJukeListPicture;

    QByteArray payload( (char *)&param, sizeof(AppFileManagerStartParameter) );

    Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_FMGR, EVT_FM_LAUNCH_STATE, payload );
    NotifyUISH( postEvent, (DISPLAY_T)targetScreen );

    Event event( EVT_KEY_SOFT_FMGR, GetThisAppId(), APP_UI_MANAGER );
    NotifyUISH( event, (DISPLAY_T)targetScreen );

    //LOG2 ( QString("<-----"), Logger::PRIORITY_HI );

}

void SettingsAppEngine::LaunchUpgradeViaUSB()
{
    LOG2 ( QString("L_UpUSB"), Logger::PRIORITY_HI );


    Event event( EVT_REQUEST_UPGRADE, GetThisAppId(), APP_UI_MANAGER );

    if ( !NotifyUISHCommon(event) )
    {
        //qDebug("LaunchUpgradeViaUSB: Event notification failed");
        LOG2 ( QString("L_UpUSB:EVT Fail!"), Logger::PRIORITY_HI );
    }
}

void
SettingsAppEngine::SetLocalization( LANGUAGE_T languageId )
{
    LOG2 ( QString("SetLocal:languageId = %1").arg(languageId), Logger::PRIORITY_HI );

    if (m_currentLanguage == languageId) // verify on current laguage
        return;

    m_currentLanguage = languageId;

    if ( m_pTranslator == NULL )
    {
        m_pTranslator = new QTranslator( this );
    }

    switch ( languageId )
    {
    case LANGUAGE_KO:
        GetLogger().Log( "SettingsAppEngine::SetLocalization::KO" );
        m_pTranslator->load( "/app/share/AppSettings/localization/AppSettings_ko" );
        break;

    case LANGUAGE_ZH_YUE:
    case LANGUAGE_ZH_CMN:
        GetLogger().Log("SettingsAppEngine::SetLocalization::ZH_CMN");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_zh-cmn");
        break;

    case LANGUAGE_PT:
        GetLogger().Log("SettingsAppEngine::SetLocalization::PT");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_pt");
        break;

    case LANGUAGE_EN_GB:
        GetLogger().Log("SettingsAppEngine::SetLocalization::EN_GB");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_en-gb");
        break;

    case LANGUAGE_FR:
        GetLogger().Log("SettingsAppEngine::SetLocalization::FR EU");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_fr-eu");
        break;

    case LANGUAGE_IT:
        GetLogger().Log("SettingsAppEngine::SetLocalization::IT");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_it");
        break;

    case LANGUAGE_DE:
        GetLogger().Log("SettingsAppEngine::SetLocalization::DE");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_ge");
        break;

    case LANGUAGE_ES:
        GetLogger().Log("SettingsAppEngine::SetLocalization::ES EU");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_es-eu");
        break;

    case LANGUAGE_RU:
        GetLogger().Log("SettingsAppEngine::SetLocalization::RU");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_ru");
        break;

    case LANGUAGE_NL:
        GetLogger().Log("SettingsAppEngine::SetLocalization::NL");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_nl");
        break;

    case LANGUAGE_SV:
        GetLogger().Log("SettingsAppEngine::SetLocalization::SV");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_sv");
        break;

    case LANGUAGE_PL:
        GetLogger().Log("SettingsAppEngine::SetLocalization::PL");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_pl");
        break;

    case LANGUAGE_TR:
        GetLogger().Log("SettingsAppEngine::SetLocalization::TR");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_tr");
        break;

    case LANGUAGE_CS:
        GetLogger().Log("SettingsAppEngine::SetLocalization::CS");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_cs");
        break;

    case LANGUAGE_DA:
        GetLogger().Log("SettingsAppEngine::SetLocalization::DA");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_da");
        break;

    case LANGUAGE_SK:
        GetLogger().Log("SettingsAppEngine::SetLocalization::SK");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_sk");
        break;

    case LANGUAGE_AR:
        GetLogger().Log("SettingsAppEngine::SetLocalization::AR");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_ar");
        break;

    case LANGUAGE_FR_NA:
        GetLogger().Log("SettingsAppEngine::SetLocalization::FR NA");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_fr-na");
        break;

    case LANGUAGE_ES_NA:
        GetLogger().Log("SettingsAppEngine::SetLocalization::ES NA");
        m_pTranslator->load("/app/share/AppSettings/localization/AppSettings_es-na");
        break;

    case LANGUAGE_EN:
    case LANGUAGE_EN_US:
    default:
        GetLogger().Log( "SettingsAppEngine::SetLocalization::DEF" );
        m_pTranslator->load( "/app/share/AppSettings/localization/AppSettings_en" );
        break;
    }

    qApp->installTranslator( m_pTranslator );
    emit retranslateUi( languageId );
}

void
SettingsAppEngine::KeyEventHandler( SettingsDef::SETTINGS_EVT_KEY_T nEventKey,
                                    Event &aEvent,
                                    AppEngineQMLConstants::EAPP_SCREEN_USAGE_T targetScreen,
                                    int EventTarget)
{
    LOG2 ( QString("KeyEventHandler"), Logger::PRIORITY_HI );

    KEY_EVENT_DATA_T *message = NULL;

    if ( !aEvent.Data().isEmpty() )
    {
        message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
    }

    if ( message )
    {
        emit keyEvent( (int)nEventKey, (int)message->keyStatus, (int)targetScreen, EventTarget );
    }
}


void SettingsAppEngine::FrontScreenDisplayOff()
{
    LOG2 ( QString("FrontScreenDisplayOff"), Logger::PRIORITY_HI );

    AppClockStartParameter param;
    param.startState = eStart_Screen_Saver_Mode;

    QByteArray payload( (char *)&param, sizeof(AppClockStartParameter) );

    Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_IDLE, payload );
    NotifyUISH( postEvent );


    DISPLAY_T screen = DISPLAY_FRONT;
    if(m_bDCSwapped)
    {
        screen = DISPLAY_REAR;
    }

    Event event( EVT_KEY_SOFT_CLOCK, GetThisAppId(),APP_UI_MANAGER,(EVT_COMMAND_T)screen,0);

    if ( !NotifyUISH(event,  screen))
    {
        //qDebug("LaunchFrontScreenClock: Event notification failed");
        LOG2 ( QString("FrontScreenDisplayOff:EVT Fail!"), Logger::PRIORITY_HI );
    }
    else
    {
        m_isFrontScreenOff = true;
    }
}

void SettingsAppEngine::RearScreenDisplayOff()
{
    LOG2 ( QString("RearScreenDisplayOff"), Logger::PRIORITY_HI );

    AppClockStartParameter param;
    param.startState = eStart_Screen_Saver_Mode;

    QByteArray payload( (char *)&param, sizeof(AppClockStartParameter) );

    Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_IDLE, payload );

    NotifyUISH( postEvent );

    DISPLAY_T screen = DISPLAY_REAR;
    if(m_bDCSwapped)
    {
        screen = DISPLAY_FRONT;
    }

    Event event( EVT_KEY_SOFT_CLOCK, GetThisAppId(),APP_UI_MANAGER,(EVT_COMMAND_T)screen,0);

    if ( !NotifyUISH(event,  DISPLAY_REAR))
    {
        //qDebug("LaunchFrontScreenClock: Event notification failed");
        LOG2 ( QString("RearScreenDisplayOff:EVT Fail!"), Logger::PRIORITY_HI );
    }
}

void SettingsAppEngine::NotifyFrontScreenBrightness(const uint value)
{
    LOG2 ( QString("N_F_Bright: %1").arg(value), Logger::PRIORITY_HI );

    if(m_pDBusinterface->isValid())
    {
        m_target=DecimalToHexa(1);
        m_FrontScreenBrightness=value;

        m_DBusreply = m_pDBusinterface->call("SetLCDBrightness", m_target, m_FrontScreenBrightness, m_Rear1ScreenBrightness, m_RearScreenBrightness);
    }
}

void SettingsAppEngine::NotifyRearScreenBrightness(const uint value)
{
    LOG2 ( QString("N_R_Bright: %1").arg(value), Logger::PRIORITY_HI );

    if(m_pDBusinterface->isValid())
    {
        m_target=DecimalToHexa(2);
        m_Rear1ScreenBrightness=value;
        m_DBusreply = m_pDBusinterface->call("SetLCDBrightness",m_target,m_FrontScreenBrightness,m_Rear1ScreenBrightness,m_RearScreenBrightness);
    }
}

void SettingsAppEngine::NotifyFrontRearScreenBrightness(const uint value)
{
    LOG2 ( QString("N_FR_Bright"), Logger::PRIORITY_HI );

    if(m_pDBusinterface->isValid())
    {
        m_target=DecimalToHexa(0);
        m_FrontScreenBrightness=value;
        m_Rear1ScreenBrightness=value;
        m_RearScreenBrightness=value;

        m_DBusreply = m_pDBusinterface->call("SetLCDBrightness",m_target,m_FrontScreenBrightness,m_Rear1ScreenBrightness,m_RearScreenBrightness);
    }
}

//convert the uint to hexa to pass to most manager
int SettingsAppEngine::DecimalToHexa(const uint value)
{


    bool ok;
    QString numInDec;
    QString numInDec1=numInDec.setNum(value,10);
    int hexValue = numInDec1.toInt(&ok,16);

    //LOG2 ( QString("DtoHexa:%1").arg(hexValue), Logger::PRIORITY_HI );
    return hexValue;
}

void SettingsAppEngine::SendTimeToIBox()
{
    LOG2 ( QString("SendTimeToIBox"), Logger::PRIORITY_HI );

    this->m_pIBoxTimer->stop();
    this->m_pIBoxTimer->start();
}

void SettingsAppEngine::SetTimeFormat(uchar timeFormat)
{
    LOG2 ( QString("SetTimeFormat: %1").arg(timeFormat), Logger::PRIORITY_HI );

    this->m_timeFormat = timeFormat;
}

void SettingsAppEngine::SendDistanceUnit(int distanceUnit)
{
    //LOG2 ( QString("SDU"), Logger::PRIORITY_HI );

    CMOSTManager *instance = CMOSTManager::getInstance();

    if (instance != NULL)
    {
        instance->sendDistanceUnitToIBox((uint) distanceUnit);
    }
    else
    {
        //qDebug("SettingsAppEngine::SetDistanceUnit can't create instance of CMOSTManager");
        LOG2 ( QString("SDU:Fail!"), Logger::PRIORITY_HI );
    }
}

void SettingsAppEngine::SendBeepSettingToIBox(int beep)
{
    //LOG2 ( QString("SBTI"), Logger::PRIORITY_HI );

    CMOSTManager *instance = CMOSTManager::getInstance();

    if (instance != NULL)
    {
        instance->sendBeepSettingToIBox((uint) beep);
    }
    else
    {
        //qDebug("SettingsAppEngine::SendBeepSetting can't create instance of CMOSTManager");
        LOG2 ( QString("SBTI:Fail!"), Logger::PRIORITY_HI );
    }
}

void SettingsAppEngine::SendPhonePrioritySettingToIBox(int phonePriority)
{
    LOG2 ( QString("SPPSTI"), Logger::PRIORITY_HI );

    CMOSTManager *instance = CMOSTManager::getInstance();

    if (instance != NULL)
    {
        instance->sendPhonePrioritySettingToIBox((uint) phonePriority);
    }
    else
    {
        //qDebug("SettingsAppEngine::SendPhonePrioritySettingToIBox can't create instance of CMOSTManager");
        LOG2 ( QString("SPPSTI:Fail"), Logger::PRIORITY_HI );
    }
}

void SettingsAppEngine::SetDateFormat(int dateFormat)
{
    LOG2 ( QString("SetDateF: %1").arg(dateFormat), Logger::PRIORITY_HI );

    this->m_dateFormat = (uint) dateFormat;

}

void SettingsAppEngine::SendDateFormat(int dateFormat)
{
    LOG2 ( QString("SendDateF"), Logger::PRIORITY_HI );

    this->m_dateFormat = (uint) dateFormat;

    QTime time = QTime::currentTime();
    QDate curDate = QDate::currentDate();

    this->m_lastChangedTime.wYear = (unsigned short) curDate.year();
    this->m_lastChangedTime.wMonth = (unsigned short) curDate.month();
    this->m_lastChangedTime.wDay = (unsigned short) curDate.day();
    this->m_lastChangedTime.wHour = (unsigned short) time.hour();
    this->m_lastChangedTime.wMinute = (unsigned short) time.minute();
    this->m_lastChangedTime.wSecond = (unsigned short) time.second();

    onTimeChangeTimeout();
}

void SettingsAppEngine::SetSystemTime(int year, int month, int day,
                                      int hours,int minutes, int ampm)
{
    LOG2 ( QString("SetSystemTime"), Logger::PRIORITY_HI );

    //merge of 7320 fix User time is not updating properly to the MICOM .
    //RCA:Appsettings will send localetime to MICOM but micom take it as UTC time
    //so if the locale time is PDT ,EST,PST etc.. we are getting the time diffrence .
    //Issue is fixed by coverting the locale time to UTC time and sending to micom.

    SYSTEMTIME time;

    //qDebug()  << "*year = " << year;
    //qDebug () << "wMonth = " << month;
    //qDebug () << "wDay = " << day;
    //qDebug () << "wHour = " << hours;
    //qDebug () << "wMinute = " << minutes;
    //qDebug () << "ampm = " << ampm;
    LOG2(QString("Y:%1, Mon:%2, D:%3, H:%4, M:%5, ap:%6").arg(year).arg(month)
         .arg(day).arg(hours).arg(minutes).arg(ampm), Logger::PRIORITY_HI);

    int seconds = 0;

    //sync current second
    //QTime cu_time = QTime::currentTime();
    //int seconds = cu_time.second();

    if(ampm == SettingsDef::SETTINGS_TIME_TYPE_PM)
    {
        if (hours != 12)
        {
            hours+=12;
        }
    }

    if((hours==12)&&(ampm == SettingsDef::SETTINGS_TIME_TYPE_AM))
    {
        hours=0;
    }

    QTime time_set_by_user(hours,minutes,seconds);
    QDate date_set_by_user(year,month,day);
    QDateTime dt = QDateTime(date_set_by_user, time_set_by_user);
    QDateTime utc(dt);
    QDateTime convertedUtcTime=utc.toUTC();

    int MicomYear=convertedUtcTime.date().year();
    int MicomMonth=convertedUtcTime.date().month();
    int MicomDay=convertedUtcTime.date().day();
    int MicomHours=convertedUtcTime.time().hour();
    int MicomMinutes=convertedUtcTime.time().minute();
    int MicomSec=convertedUtcTime.time().second();

    //merge of 7320 fix User time is not updating properly to the MICOM .

    time.wYear = (unsigned short)MicomYear;
    time.wMonth = (unsigned short) MicomMonth;
    time.wDay = (unsigned short) MicomDay;
    time.wHour = (unsigned short) MicomHours;
    time.wMinute =(unsigned short) MicomMinutes;
    time.wSecond = (unsigned short) MicomSec;

    //qDebug()  << "**year = " << time.wYear ;
    //qDebug () << "wMonth = " << time.wMonth;
    //qDebug () << "wDay = " << time.wDay;
    //qDebug () << "wHour = " << time.wHour;
    //qDebug () << "wMinute = " << time.wMinute;
    //qDebug () << "wSecond = "<< time.wSecond;

    this->m_lastChangedTime.wYear = (unsigned short) dt.date().year();
    this->m_lastChangedTime.wMonth = (unsigned short) dt.date().month();
    this->m_lastChangedTime.wDay = (unsigned short) dt.date().day();
    this->m_lastChangedTime.wHour = (unsigned short) dt.time().hour();
    this->m_lastChangedTime.wMinute = (unsigned short) dt.time().minute();
    this->m_lastChangedTime.wSecond = (unsigned short) dt.time().second();

    setRTCInfoToMicom( &time );

    this->m_pIBoxTimer->stop();
    this->m_pIBoxTimer->start();

    emit saveSettingsSignal(0, SettingsDef::DB_KEY_GPS_TIME_SETTINGS, 0);
    NotifyApplication(SettingsDef::DB_KEY_GPS_TIME_SETTINGS, 0, "", 1);

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

bool SettingsAppEngine::sendLanguageToIBox(LANGUAGE_T languageId, int targetScreen)
{
    LOG2 ( QString("s_Lan_toIBox"), Logger::PRIORITY_HI );

    bool result = false;
    CMOSTManager *instance = CMOSTManager::getInstance();

    if (instance != NULL)
    {
        uint unit = (targetScreen == AppEngineQMLConstants::APP_FRONT_SCREEN) ? UNIT_HEAD_UNIT :
                                                                                UNIT_REAR_SCREEN;

        result = instance->sendLanguageToIBox(languageId, unit);
    }
    else
    {
        //GetLogger().Log("SettingsAppEngine::sendLanguageToIBox can't create instance of CMOSTManager");
        LOG2 ( QString("s_Lan_toIBox:can't instance!"), Logger::PRIORITY_HI );
    }

    return result;
}

void SettingsAppEngine::getLcdStatusInfo()
{
    //LOG2 ( QString("getLcdStatusInfo"), Logger::PRIORITY_HI );

    QDBusConnection connection = QDBusConnection::sessionBus();

    if (!connection.isConnected())
    {
        //GetLogger().Log("SettingsAppEngine::getLcdStatusInfo Cannot connect to the D-BUS session bus.");
        LOG2 ( QString("LcdStatus:can't D-Bus"), Logger::PRIORITY_HI );
        return;
    }


    if(connection.connect(MICOM_MANAGER_SERVICE_NAME, MICOM_MANAGER_SYSINFO_PATH, MICOM_MANAGER_SYSINFO_INTERFACE,
                          MICOM_MANAGER_SIGNAL_LCD_DAYNIGHT, this, SLOT(SaveDayNightState(QDBusMessage))))
    {
        //GetLogger().Log("SettingsAppEngine::getLcdStatusInfo Successfully connected for the day night status request signal from Micom");
        LOG2 ( QString("LcdStatus:con D-Bus"), Logger::PRIORITY_LOW );
    }
    else
    {
        //GetLogger().Log("SettingsAppEngine::getLcdStatusInfo Connecting for the day night status request signal failed");
        LOG2 ( QString("LcdStatus:con D-Bus fail!"), Logger::PRIORITY_HI );
        return;
    }
}

void SettingsAppEngine::SaveDayNightState(QDBusMessage message)
{
    //LOG2 ( QString("SaveDayNightState"), Logger::PRIORITY_HI );

    QList<QVariant> list;

    //GetLogger().Log("Signal request signal received from Micom.. Inform all apps to save their state");

    // Checking for the message arguments
    list = message.arguments();

    if(list.isEmpty())
    {
        //GetLogger().Log("No arguments in Message");
        LOG2 ( QString("SDNS:argu Fail!"), Logger::PRIORITY_HI );
    }
    //else {
    //    GetLogger().Log("Message has Arguments");
    //    LOG2 ( QString("SDNS:argu OK"), Logger::PRIORITY_HI );
    //}

    int mode = list.at( 0 ).toInt();

    if(SETTINGS_LCD_DAYMODE == mode)
    {
        m_nDayNightMode = SETTINGS_LCD_DAYMODE;
    }
    else
    {
        m_nDayNightMode = SETTINGS_LCD_NIGHTMODE;
    }
}


void SettingsAppEngine::NotifyDisplayBrightnessChange(bool isFrontScreen, unsigned int value)
{
    LOG2 ( QString("NotiDisBrightCh:isFront[%1], val[%2]").arg(isFrontScreen).arg(value), Logger::PRIORITY_HI );

    QDBusConnection conn = QDBusConnection::sessionBus();
    QDBusMessage lcdPWMControl;

    if (!conn.isConnected())
    {
        return;
    }

    lcdPWMControl = QDBusMessage::createMethodCall( "com.lge.car.micom", "/Lcd","com.lge.car.micom.lcd", "LgeDBus_SetLcdPWMControl" );
    uchar args1 = (uchar)0xFF;
    uchar args2 = (uchar)m_nDayNightMode;
    uchar args3 = (uchar)0xFF;
    uchar args4 = (uchar)0xFF;

    if(isFrontScreen)
    {
        // Front Screen Brightness
        args3 = (uchar)(SETTINGS_LCD_BRIGHTNESS_0 + value);
    }
    else
    {
        // Rear Screen Brightness
        args4 = (uchar)(SETTINGS_LCD_BRIGHTNESS_0 + value);
    }

    lcdPWMControl <<  qVariantFromValue(uchar(args1));
    lcdPWMControl <<  qVariantFromValue(uchar(args2));
    lcdPWMControl <<  qVariantFromValue(uchar(args3));
    lcdPWMControl <<  qVariantFromValue(uchar(args4));
    conn.send(lcdPWMControl);
}

// added(modify) for modified ibox time set 02/05
void SettingsAppEngine::onTimeChangeTimeout()
{
    LOG2 ( QString("onTCTime"), Logger::PRIORITY_HI );

    GetLogger().Log("SettingsAppEngine::onTimeChangeTimeout() cnage system time");

    emit SetTimeToIBox(this->m_timeFormat,
                       this->m_lastChangedTime.wYear,
                       this->m_lastChangedTime.wMonth,
                       this->m_lastChangedTime.wDay,
                       this->m_lastChangedTime.wHour,
                       this->m_lastChangedTime.wMinute,
                       this->m_lastChangedTime.wSecond,
                       this->m_dateFormat);
#if 0
    CMOSTManager *instance = CMOSTManager::getInstance();

    if (instance != NULL)
    {
        instance->sendDateTimeToIBox(this->m_timeFormat,
                                     this->m_lastChangedTime.wYear,
                                     this->m_lastChangedTime.wMonth,
                                     this->m_lastChangedTime.wDay,
                                     this->m_lastChangedTime.wHour,
                                     this->m_lastChangedTime.wMinute,
                                     this->m_lastChangedTime.wSecond,
                                     this->m_dateFormat);
    }
    else
    {
        GetLogger().Log("SettingsAppEngine::SetSystemTime can't create instance of CMOSTManager");
    }
#endif

}
// added(modify) for modified ibox time set 02/05
void SettingsAppEngine::NotifyDistanceUnitChanged(int value)
{
    LOG2 ( QString("NotiDisUnitCh: %1").arg(value), Logger::PRIORITY_HI );

    EVENT_NAVI_SETTING_UNIT_DATA_T message;
    message.Unit = value;

    QByteArray payload( (char *)&message, sizeof(EVENT_NAVI_SETTING_UNIT_DATA_T) );

    Event event( EVT_NAVI_SETTTING_UNIT, GetThisAppId(), APP_NAVIGATION, payload );

    if ( !NotifyUISHCommon( event ) )
    {
        //qDebug( "NotifyDistanceUnitChanged: Event notification failed" );
        LOG2(QString("NotiDisUnitCh:fail!"), Logger::PRIORITY_HI);
    }
}

void SettingsAppEngine::NotifyDisplayModeChange(unsigned int value)
{
    LOG2 ( QString("NotiDisModeCh: %1").arg(value), Logger::PRIORITY_HI );

    QDBusConnection conn = QDBusConnection::sessionBus();
    QDBusMessage lcdPWMControl;

    if (!conn.isConnected())
    {
        return;
    }

    lcdPWMControl = QDBusMessage::createMethodCall( "com.lge.car.micom",
                                                    "/Lcd",
                                                    "com.lge.car.micom.lcd",
                                                    "LgeDBus_SetLcdPWMControl" );

    uchar args1 = (uchar)0x00;
    uchar args2 = (uchar)0xFF;
    uchar args3 = (uchar)0xFF;
    uchar args4 = (uchar)0xFF;
    switch(value)
    {
    //for auto mode
    case 0: args1 = (uchar)0x02;
        break;
        // for day mode
    case 1: args1 = (uchar)0x01;
        break;
        //for night mode
    case 2: args1 = (uchar)0x00;
        break;
    }
    lcdPWMControl <<  qVariantFromValue(uchar(args1));
    lcdPWMControl <<  qVariantFromValue(uchar(args2));
    lcdPWMControl <<  qVariantFromValue(uchar(args3));
    lcdPWMControl <<  qVariantFromValue(uchar(args4));
    conn.send(lcdPWMControl);
}

void SettingsAppEngine::NotifySetIlluminationMostManager(unsigned int value)
{
    LOG2 ( QString("NotiSetIlluMost: %1").arg(value), Logger::PRIORITY_HI );

    if(m_pDBusinterface->isValid())
    {
        int m_Setillumination=DecimalToHexa(value);
        m_DBusreply = m_pDBusinterface->call("SetIllumination",m_Setillumination);
    }
}

void SettingsAppEngine::NotifyDisplayOnOffMostManager( unsigned int value )
{
    LOG2 ( QString("NotiDisOnOffMost: %1").arg(value), Logger::PRIORITY_HI );
    if(m_pDBusinterface->isValid())
    {
        //GetLogger().Log( QString("SettingsAppEngine::NotifyDisplayOnOffMostManager - %1  BLOCK ").arg(value) );
        m_DBusreply = m_pDBusinterface->call("SetDisplayControl",value);
    }
}

void SettingsAppEngine::NotifySetAspectRatioChange(unsigned int value)
{
    LOG2 ( QString("NotiSetAspect: %1").arg(value), Logger::PRIORITY_HI );

    //GetLogger().Log( QString("SettingsAppEngine::NotifySetAspectRatioChange - %1").arg(value) );

    if(m_pDBusinterface->isValid())
    {
        //GetLogger().Log( "m_pDBusinterface is valid" );
        //GetLogger().Log( QString("SettingsAppEngine::NotifySetAspectRatioChange - %1").arg(value) );
        int m_setAspectratio=DecimalToHexa(value);
        m_DBusreply = m_pDBusinterface->call("SetVideoRatio",m_setAspectratio);
    }
}


void SettingsAppEngine::NotifySetVideoSettings( const int brightness, const int saturation, const int contrast, const int hue )
{
    LOG2 ( QString("NotiSetVideoSet: %1 %2 %3 %4").arg(brightness).arg(saturation).arg(contrast).arg(hue), Logger::PRIORITY_HI );

    if(m_pDBusinterface->isValid())
    {
        m_DBusreply = m_pDBusinterface->call("SetScreen",brightness, saturation, contrast, hue);
    }
}

bool SettingsAppEngine::NotifyVideoMode(int targetScreen)
{
    LOG2 ( QString("NotiVideoMode: %1").arg(targetScreen), Logger::PRIORITY_HI );

    if(m_transperentTarget.value ( AppEngineQMLConstants::EAPP_SCREEN_USAGE_T(targetScreen), false ))
    {
        int mvideostatus = m_pShareDataProviderVideo->GetStatusDateByIndex(DATA_TYPE_PLAY_STATUS);
        int mvideotabactive = m_pShareDataProviderVideo->GetStatusDateByIndex(DATA_TYPE_ACTIVE_MEDIA_SOURCE);
        int mDmbStatus = m_pShareDataProviderDmb->GetStatusDateByIndex(DATA_TYPE_PLAY_STATUS);
        //qDebug()<<"The video status="<<mvideostatus;
        //qDebug()<<"The video tab active="<< mvideotabactive;
        //qDebug()<<"The dmb status="<< mDmbStatus;
        return ( (((mvideostatus == 1)  && ((APP_STATE_T)mvideotabactive != APP_STATE_NONE))
                  || ((APP_STATE_T)mvideotabactive == APP_STATE_AUX))
                 || (mDmbStatus == true) ) ? true : false;
    }
    else
    {
        return false;
    }
}

bool SettingsAppEngine::IsScreenOff(bool isFrontScreen)
{
    LOG2 ( QString("IsScreenOff: %1").arg(isFrontScreen), Logger::PRIORITY_HI );

    if(isFrontScreen)
    {
        return m_isFrontScreenOff;
    }
    //The method don't use to Rear display, return false - fix of coverity issue
    return false;
}

int SettingsAppEngine::maskValue( int value, int maskValue, bool isAnd )
{
    LOG2 ( QString("maskValue: %1 %2 %3").arg(value).arg(maskValue).arg(isAnd), Logger::PRIORITY_HI );

    if ( isAnd == true )
    {
        return ( value & maskValue );
    }
    else
    {
        return ( value | maskValue );
    }
}

bool SettingsAppEngine::checkMaskValue( int value, int maskValue )
{
    LOG2 ( QString("checkMaskValue: %1 %2").arg(value).arg(maskValue), Logger::PRIORITY_HI );

    if ( value & maskValue )
    {
        return true;
    }

    return false;
}

//added for NA/CH Agreement Spec Modify(time 10, default on)
bool SettingsAppEngine::isAgree10Variant()
{
    //LOG2(QString("[QT] Current Country Code : [ %1 ]").arg(m_variantCounry), Logger::PRIORITY_HI);
    if(m_variantCounry == (ECountryVariant)eCVNorthAmerica || m_variantCounry == (ECountryVariant)eCVCanada
            || m_variantCounry == (ECountryVariant)eCVChina)
    {
        //LOG2(QString("[QT] Current Country Code is NA/CA/CH ---"), Logger::PRIORITY_HI);
        return true;
    }
    else
    {
        //LOG2(QString("[QT] Current Country Code is exception country(NA/CA/CH) ---"), Logger::PRIORITY_HI);
        return false;
    }
}
//added for NA/CH Agreement Spec Modify(time 10, default on)

//added for QML Log Message
void SettingsAppEngine::printLogMessage(QString logString)
{
    LOG2(QString("[QML]: %1").arg(logString), Logger::PRIORITY_HI);
}

void SettingsAppEngine::NotifySetAspectRatio(int value ,int screen)
{
    LOG2 ( QString("N_SetAspect:value = %1").arg(value), Logger::PRIORITY_HI );

    VideoWidgetProps message;
    message.aspectRatio=(ASPECT_RATIO_T)value;
    message.displayHeight= 720;
    message.displayWidth= 1280;
    message.displayX= 0;
    message.displayY= 0;

    if(screen == AppEngineQMLConstants::APP_FRONT_SCREEN)
        message.isFrontDisplay = true;
    else
        message.isFrontDisplay =false;

    QByteArray payload( (char *)&message, sizeof(VideoWidgetProps) );

    Event postEventToAVP( EVT_POST_EVENT, GetThisAppId(), APP_AVP, EVT_ASPECTRATIO_CHANGE, payload );
    Event postEventToDMB( EVT_POST_EVENT, GetThisAppId(), APP_MOBILETV, EVT_ASPECTRATIO_CHANGE, payload );
    //Event postEventToQuickGuide( EVT_POST_EVENT, GetThisAppId(), APP_INTERACTIVE_GUIDE, EVT_ASPECTRATIO_CHANGE, payload );

    NotifyUISH( postEventToAVP );
    NotifyUISH( postEventToDMB );
    //NotifyUISH( postEventToQuickGuide );
}

void SettingsAppEngine::NotifyAppsFormatStarted()
{
    LOG2 ( QString("N_FormatStart"), Logger::PRIORITY_HI );

    Event event( EVT_FORMAT_JUKEBOX_START, GetThisAppId(), APP_ALL );

    if ( !NotifyUISHCommon(event) )
    {
        LOG2 ( QString("failed NotifyUISHCommon"), Logger::PRIORITY_HI );
    }
}

void SettingsAppEngine::NotifyAppsFormatCompleted()
{
    LOG2 ( QString("N_FComplete"), Logger::PRIORITY_HI );

    Event event( EVT_FORMAT_JUKEBOX_END, GetThisAppId(), APP_ALL );

    if ( !NotifyUISHCommon(event) )
    {
        LOG2 ( QString("failed NotifyUISHCommon"), Logger::PRIORITY_HI );
    }
}

void SettingsAppEngine::SetLastChangedTime(SYSTEMTIME time)
{
    m_lastChangedTime = time;
}

void SettingsAppEngine::showPopapInMainArea(int type, int screen)
{
    emit showPopup(type, screen);
}

void SettingsAppEngine::ResetNaviSettings()
{
    LOG2 ( QString("R_NaviSet"), Logger::PRIORITY_HI );

    Event eventNavi( EVT_INITIALIZATION_REQUEST, GetThisAppId(), APP_NAVIGATION );
    Event eventGps( EVT_INITIALIZATION_REQUEST, GetThisAppId(), APP_GPS );

    if ( !NotifyUISHCommon( eventNavi ) )
    {
        LOG2 ( QString("failed NotifyUISHCommon"), Logger::PRIORITY_HI );
    }

    if ( !NotifyUISHCommon( eventGps ) )
    {
        LOG2 ( QString("failed NotifyUISHCommon"), Logger::PRIORITY_HI );
    }
}

void SettingsAppEngine::ResetAllBTSettings()
{
    LOG2 ( QString("R_ABTSet"), Logger::PRIORITY_HI );

    Event event( EVT_INITIALIZATION_REQUEST, GetThisAppId(), APP_BTPHONE );
    if ( !NotifyUISHCommon( event ) )
    {
        LOG2 ( QString("failed NotifyUISHCommon"), Logger::PRIORITY_HI );
    }
}

void SettingsAppEngine::ResetDVDSettings()
{
    LOG2 ( QString("R_DVDSet"), Logger::PRIORITY_HI );

    Event event( EVT_INITIALIZATION_REQUEST, GetThisAppId(), APP_AVP );
    if ( !NotifyUISHCommon( event ) )
    {
        LOG2 ( QString("failed NotifyUISHCommon"), Logger::PRIORITY_HI );
    }
}

void SettingsAppEngine::ResetXMAudioSettings()
{
    LOG2 ( QString("R_XMAudioSet"), Logger::PRIORITY_HI );

    Event event( EVT_INITIALIZATION_REQUEST, GetThisAppId(), APP_XM_AUDIO );
    if ( !NotifyUISHCommon( event ) )
    {
        LOG2 ( QString("failed NotifyUISHCommon"), Logger::PRIORITY_HI );
    }
}

void SettingsAppEngine::ResetXMDataSettings()
{
    LOG2 ( QString("R_XMDataSet"), Logger::PRIORITY_HI );

    Event event( EVT_INITIALIZATION_REQUEST, GetThisAppId(), APP_XM_DATA );
    if ( !NotifyUISHCommon( event ) )
    {
        LOG2 ( QString("failed NotifyUISHCommon"), Logger::PRIORITY_HI );
    }
}

//added for ISV 113930 VR Init Issue
void SettingsAppEngine::ResetVRSettings()
{
    LOG2 ( QString("VR Init"), Logger::PRIORITY_HI );

    Event event( EVT_INITIALIZATION_REQUEST, GetThisAppId(), APP_VR );
    if ( !NotifyUISHCommon( event ) )
    {
        LOG2 ( QString("failed NotifyUISHCommon"), Logger::PRIORITY_HI );
    }
}

void SettingsAppEngine::PopupHandler(int type, int screen)
{
    LOG2 ( QString("P_Handler: %1 %2").arg(type).arg(screen), Logger::PRIORITY_HI );

    switch(type)
    {
    case SettingsDef::SETTINGS_RESET_GENERAL_SETTINGS:
        ResetXMAudioSettings();
        ResetXMDataSettings();
        ResetNaviSettings();
        ResetAllBTSettings();
        ResetDVDSettings();
        ResetVRSettings(); //added for ISV 113930 VR Init Issue
        CMOSTManager::getInstance()->sendResetSettingToIBox(0);
        emit showToastPopup(type, screen);
        break;

    case SettingsDef::SETTINGS_RESET_SOUND_SETTINGS:
        emit showToastPopup(type, screen);
        break;

    case SettingsDef::SETTINGS_RESET_SCREEN_SETTINGS:
        emit showToastPopup(type, screen);
        break;
    }
}

void SettingsAppEngine::LaunchSearchView(int activeTab)
{
    emit showListDVISearchSetting(activeTab);
}

void SettingsAppEngine::keyboardSearchCodeSeleted()
{
    emit selectionDoneFromSearch();
}

DualDisplayHelper::MEDIA_DISPLAY SettingsAppEngine::EffectiveDisplay()
{
    if( m_isVideoCloned == VIDEO_CLONE_NONE )
    {
        return DualDisplayHelper::eMP_DISPLAY_B;
    }
    else if( m_isVideoCloned == VIDEO_CLONE_BEGIN )
    {
        return DualDisplayHelper::eMP_DISPLAY_A_AND_B;
    }
    else
    {
        return DualDisplayHelper::eMP_DISPLAY_A;
    }
}

void SettingsAppEngine::receiveGPSTime( EVENT_NAVI_GPS_TIME_DATA_T *data )
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    SYSTEMTIME NaviTime = data->systemTime;

    int timezone_mins = data->timezone;

    GetLogger().Log( QString("(Before NaviTime timezone_mins = [%1] Month = [%2] Day = [%3] Hour = [%4] Minute = [%5] Second = [%6])")
                     .arg(timezone_mins)
                     .arg(NaviTime.wMonth)
                     .arg(NaviTime.wDay)
                     .arg(NaviTime.wHour)
                     .arg(NaviTime.wMinute)
                     .arg(NaviTime.wSecond)
                     , Logger::PRIORITY_HI);

    if( ( NaviTime.wHour != 0xFF ) || ( NaviTime.wMinute != 0xFF ) || ( NaviTime.wSecond != 0xFF ) )
    {
        m_bIsGPSAvailable = true;
        //emit gpsAvailableChanged( m_bIsGPSAvailable );

        emit sigSetTimeZone(timezone_mins, true);
        //setRTCInfoToMicom( &NaviTime );
        //this->m_pIBoxTimer->stop();
        //this->m_pIBoxTimer->start();
    }
    else
    {
        m_bIsGPSAvailable = false;
        //emit gpsAvailableChanged( m_bIsGPSAvailable );
        //GetLogger().Log( QString("GPS Time is unavailable") );
    }
}


void SettingsAppEngine::setTimePickerPopupStatus( bool isShow )
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );
    emit timePickerPopupStatus( isShow );
}



// for Rear Screen Lock
void SettingsAppEngine::connectCarIFStatus()
{
    //LOG2 ( QString("conCarIFStatus"), Logger::PRIORITY_HI );

    QDBusConnection connection = QDBusConnection::sessionBus();

    if (!connection.isConnected())
    {
        //GetLogger().Log("SettingsAppEngine::connectCarIFStatus Cannot connect to the D-BUS session bus.");
        return;
    }


    if(connection.connect(MICOM_MANAGER_SERVICE_NAME,
                          MICOM_MANAGER_SYSINFO_PATH,
                          MICOM_MANAGER_SYSINFO_INTERFACE,
                          MICOM_MANAGER_SIGNAL_CAR_IF,
                          this, SLOT( onMicomCarIFStatus (uchar, uchar)) ))
    {
        ;
    }
    else
    {
        //GetLogger().Log("SettingsAppEngine::connectCarIFStatus Connecting for the day night status request signal failed");
        LOG2(QString("conCarIFStatus::con Fail!"), Logger::PRIORITY_HI);
        return;
    }
}

// for Rear Screen Lock
void SettingsAppEngine::onMicomCarIFStatus (uchar mode, uchar status )
{
    switch ( mode )
    {
    case 0x00:
    {
        if (status == 0) // ACC off
        {
            LOG2 ( QString("## Current ACC Status(Off)."), Logger::PRIORITY_HI );
            setIsAccStatusOn(false);
        }
        else if (status == 1) // ACC on
        {
            LOG2 ( QString("## Current ACC Status(On)."), Logger::PRIORITY_HI );
            setIsAccStatusOn(true);
        }

        break;
    }

    case 0x0B:
    {
        LOG2 ( QString("## Rear Lock Status(%1)").arg(status), Logger::PRIORITY_HI );
        emit saveSettingsSignal( status, SettingsDef::DB_KEY_LOCKREARMONITOR_FUNCTION, 0);
        break;
    }
    }
}

void SettingsAppEngine::AskSysLastStatus()
{
    unsigned char accState = 0;
    QDBusInterface interface( "com.lge.car.micom", "/SysInfo", "com.lge.car.micom.sysinfo");
    QDBusReply<QByteArray> reply = interface.call("LgeDBus_AskSysLastStatus");

    if (reply.isValid())
    {
        QByteArray tmp(reply.value());
        for (int i = 0; i < tmp.count(); ++i) {
            if (i==2) { //2.2.02 Ask Sys Last Status, 2nd Byte, 1st bit -> Acc On/Off
                accState = ((unsigned char)tmp.at(i) & 1);
                if (accState==1) {
                    //ACC on
                    LOG2 ( QString("## Current ACC Status(On)."), Logger::PRIORITY_HI );
                    setIsAccStatusOn(true);
                }
                else {
                    //Acc off
                    LOG2 ( QString("## Current ACC Status(Off)."), Logger::PRIORITY_HI );
                    setIsAccStatusOn(false);
                }
            }
        }
    }
    else
        LOG2 ( QString("## LgeDBus_AskSysLastStatus - reply is not valid."), Logger::PRIORITY_HI );
}



// Swap DC
void SettingsAppEngine::UpdateViewMapping()
{
    LOG2 ( QString("UpdateViewMapping"), Logger::PRIORITY_HI );

    if (m_bDCSwapped != m_bViewContentSwapped)
    {
        //qDebug() << "[CLONE-SWAP] " "Do swap ScreenView for new DCSwap status=" <<  m_bDCSwapped
        //         << " clone=" <<  m_CloneStatus
        //         << " frontShow=" << m_bFrontScreenShowing
        //         << " rearShow=" << m_bRearScreenShowing;

        /**
           - check special cases : if it was showing in the clone mode, swap means 'clone off' also.

           It change flag only (swapped status) for the combination of [swap dc] and [clone off].

           ; 'changing flag' means that changing view's role each oterh between rear and front.
           ; you do not need to swap real contents of screens in this case.
           ;
           ; situation)
           ;   - front clone (AVP + Settings:Display Setting)
           ;   - user want to exit current mode (to Home)
           ;   - UISH do swap + clone off to keep AVP in rear screen without flicking.
           ;   - UISH intended or is expecting that
           ;        rear screen (of SettingsApp) was set as front screen was
           ;        when clone info was provided if it was showing.
           ; So)
           ;   Rear view is expected to show what front view was showing before swapping
           ;
         **/
        // if it was front-clone and front screen was showing
        if (m_CloneStatus == CLONE_ON_FRONT && m_bFrontScreenShowing)
        {
            // swap + role change = nothing to do > just set that swap status is changed.
            m_bViewContentSwapped = m_bDCSwapped;
            // Showing status is not changed :  m_bFrontScreenShowing
        }
        // if it was rear-clone and rear screen was showing
        else if (m_CloneStatus == CLONE_ON_REAR && m_bRearScreenShowing)
        {
            // swap + role change = nothing to do > just set that swap status is changed.
            m_bViewContentSwapped = m_bDCSwapped;
            // Showing status is not changed :  m_bFrontScreenShowing
        }
        // otherwise, it's normal case.
        else
        {
            if (m_bFrontScreenShowing || m_bRearScreenShowing)
            {
                //qDebug() << " : warning : it's strange ! Settings got swap event in unknown status";
                //qDebug() << " : warning : do role change anyway. when UISH send proper clone info, this should be removed";
                m_bViewContentSwapped = m_bDCSwapped;
            }
            else
            {
                m_bViewContentSwapped = m_bDCSwapped;
                SwapScreenContents();
            }
        }

        // Swap means clone off also.
        // If you do 'Swap role' above, you must do following set here
        // even EVT_CLONE_NOTIFICATION shall follow it.
        m_CloneStatus = CLONE_OFF;
    }
}

// Watch Dog
void SettingsAppEngine::signalConnection()
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    struct sigaction act;
    memset(&act, 0x00, sizeof(struct sigaction));

    act.sa_flags = 0;
    act.sa_flags |= SA_RESETHAND;
    act.sa_handler = SettingsAppEngine::signalHandelr;

    for( int signumber = SIGHUP ; signumber<SIGUNUSED ; signumber++)
    {
        if( signumber == SIGKILL || signumber == SIGSTOP) continue;
        if(sigaction(signumber, &act, (struct sigaction *)NULL)<0)
        {
            //GetLogger().Log( QString( "Sigaction failnum = %1" ).arg(signumber));
            LOG2(QString("sig failnum: %1").arg(signumber), Logger::PRIORITY_HI);
            exit(1);
        }
    }
}

void SettingsAppEngine::setSignalHandlerObject(SettingsAppEngine* m)
{
    SigSettingsAppEngine = m;
}

void SettingsAppEngine::signalHandelr(int signum)
{
    //qDebug()<<"Signum = "<< signum;
    //modified for ITS 0231401 Jukebox Format Settings Restart Issue
    if(signum == SIGCHLD)
    {
        LOG2(QString("[SIGCHILD] exception case!"), Logger::PRIORITY_HI);
    }
    else
    {
        SigSettingsAppEngine->restart( signum );
        raise(signum);
    }
    //modified for ITS 0231401 Jukebox Format Settings Restart Issue
}

void SettingsAppEngine::restart( int sig )
{
    LOG2 ( QString("restart"), Logger::PRIORITY_HI );

    Event event( EVT_REQUEST_ABNORMAL_TERMINATION, GetThisAppId(), APP_UI_MANAGER, (EVT_COMMAND_T) sig );
    if ( !NotifyUISH(event, DISPLAY_FRONT) )
    {
        //qDebug("SettingsAppEngine::restart: Event notification failed");
        LOG2(QString("restart:Noti Fail!"), Logger::PRIORITY_HI);
    }
    //exit(0);
}

void SettingsAppEngine::showMotorErrorPopup()
{
    LOG2 ( QString("sMotoErrPopUP"), Logger::PRIORITY_HI );

    EVENT_SYSTEM_POPUP_T a;
    memset(&a, 0x0, sizeof(EVENT_SYSTEM_POPUP_T));

    QString popupMSG;
    popupMSG = QApplication::translate("main","STR_SETTING_GENERAL_NOT_WORKING");

    QString popupBtn("OK");

    a.Popupshow.type = POPUP_TEXT;
    popupMSG.toWCharArray(a.Popupshow.popup.text.msg);

    a.Popupshow.popup.text.btn_count = 1;

    a.Popupshow.popup.text.btn[0] = POPUP_BUTTON_TYPE_TEXT;
    popupBtn.toWCharArray(a.Popupshow.popup.text.btn_txt[0]);

    a.Popupshow.timeout = 0x0fffffff;

    a.Display = DISPLAY_FRONT;
    a.Popuptype = MOTO_ERORR_POPUP;
    a.Popupshow.source = APP_SETTINGS;

    Event event( EVT_REQUEST_SYSTEM_POPUP, APP_SETTINGS, APP_UI_MANAGER, QByteArray( (char *)&a, sizeof( EVENT_SYSTEM_POPUP_T ) ) );

    if ( !NotifyUISHCommon(event) )
    {
        LOG2 ( QString("failed NotifyUISHCommon"), Logger::PRIORITY_HI );
    }

    //qDebug() <<" <<<<<------- [MOTOR_STATUS] showMotorErrorPopup() ";
}

QString SettingsAppEngine::getFont(bool hdb)
{
    if (hdb)
    {
        //        if (GetCountryVariant() == eCVChina)
        //        {
        //            return QString("CHINESS_HDB");
        //        }
        //        else
        //        {
        return QString("DH_HDB");
        //}
    }
    else
    {
        //        if (GetCountryVariant() == eCVChina)
        //        {
        //            return QString("CHINESS_HDR");
        //        }
        //        else
        //        {
        return QString("DH_HDR");
        //        }
    }
}

void
SettingsAppEngine::setLastEntryPoint(APP_ENTRY_POINT_T lastEntryPoint ,AppEngineQMLConstants::EAPP_SCREEN_USAGE_T targetScreen)
{
    LOG2 ( QString("setLastEntryPoint: %1 %2").arg(m_frontLastEntryPoint).arg(m_rearLastEntryPoint), Logger::PRIORITY_HI );

    switch(targetScreen)
    {
    case AppEngineQMLConstants::APP_FRONT_SCREEN:
    {
        m_frontLastEntryPoint = lastEntryPoint;
        break;
    }

    case AppEngineQMLConstants::APP_REAR_SCREEN:
    {
        m_rearLastEntryPoint = lastEntryPoint;
        break;
    }
    }
    //LOG2 ( QString("m_frontLastEntryPoint = %1, m_rearLastEntryPoint = %2").arg(m_frontLastEntryPoint).arg(m_rearLastEntryPoint), Logger::PRIORITY_HI );

    //LOG2 ( QString("<----"), Logger::PRIORITY_HI );
}

void
SettingsAppEngine :: notifyIsParkingMode()
{
    if(m_bParkingStatus)
    {
        emit isParkingModeChanged(true, AppEngineQMLConstants::APP_FRONT_SCREEN );
        emit isParkingModeChanged(true, AppEngineQMLConstants::APP_REAR_SCREEN );

        return;
    }

    if (m_isCloneMode)
    {
        emit isParkingModeChanged(false, AppEngineQMLConstants::APP_FRONT_SCREEN ); // Front Screen
        emit isParkingModeChanged(false, AppEngineQMLConstants::APP_REAR_SCREEN );  // Rear Screen
    }
    else
    {
        if(m_bDCSwapped)
        {
            emit isParkingModeChanged(true, AppEngineQMLConstants::APP_FRONT_SCREEN ); // Front Screen
            emit isParkingModeChanged(false, AppEngineQMLConstants::APP_REAR_SCREEN );  // Rear Screen
        }
        else
        {
            emit isParkingModeChanged(false, AppEngineQMLConstants::APP_FRONT_SCREEN ); // Front Screen
            emit isParkingModeChanged(true, AppEngineQMLConstants::APP_REAR_SCREEN );  // Rear Screen
        }
    }
}

void SettingsAppEngine::showTimeZoneChangedToastPopup(QString sTimeZone)
{
    LOG2 ( QString("sTimeZoneChPopUp"), Logger::PRIORITY_HI );

    EVENT_SYSTEM_POPUP_T a;
    memset(&a, 0x0, sizeof(EVENT_SYSTEM_POPUP_T));

    QString popupMSG;
    popupMSG = QApplication::translate("main","STR_SETTING_CLOCK_TIME_ZONE_CHANGED");
    //LOG2 ( QString("before Popup's String:").arg(popupMSG), Logger::PRIORITY_HI );
    popupMSG = popupMSG.replace("##", sTimeZone);
    //LOG2 ( QString("after Popup's String:").arg(popupMSG), Logger::PRIORITY_HI );

    //QString popupBtn("OK");

    a.Popupshow.type = POPUP_TOAST;
    popupMSG.toWCharArray(a.Popupshow.popup.toast.msg);


    //a.Popupshow.popup.text.btn_count = 1;
    //a.Popupshow.popup.text.btn[0] = POPUP_BUTTON_TYPE_TEXT;
    //popupBtn.toWCharArray(a.Popupshow.popup.text.btn_txt[0]);

    //a.Popupshow.timeout = 0x0fffffff;
    a.Popupshow.timeout = 3000;

    a.Display = DISPLAY_FRONT;
    a.Popuptype = ePOPUP_NAVI_CHAGNE_TIME_ZONE;
    a.Popupshow.source = APP_SETTINGS;

    Event event( EVT_REQUEST_SYSTEM_POPUP, APP_SETTINGS, APP_UI_MANAGER, QByteArray( (char *)&a, sizeof( EVENT_SYSTEM_POPUP_T ) ) );

    if ( !NotifyUISHCommon(event) )
    {
        LOG2 ( QString("failed NotifyUISHCommon"), Logger::PRIORITY_HI );
    }

    //qDebug() <<" <<<<showTimeZoneChangedToastPopup() ";
}

void
SettingsAppEngine::launchMapCareEngineeringMode( int screen )
{
    LOG2 ( QString("launchMapCareEngMode screen = %1").arg(screen), Logger::PRIORITY_HI );

    DISPLAY_T target = DISPLAY_FRONT;

    Event event( EVT_SOFT_DYNAMICS_MAP_CARE, GetThisAppId(), APP_UI_MANAGER );

    if ( !NotifyUISH(event, target) )
    {
        //qDebug("SettingsAppEngine::HandleBackKey: Event notification failed");
        LOG2(QString("launchMapCareEngMode: Noti Fail!"), Logger::PRIORITY_HI);
    }
}
