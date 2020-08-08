#include "DHAVN_AppUserManual_Engine.h"
#include <QDeclarativeView>
#include <QTranslator>
#include <QApplication>
#include <qdeclarative.h>
#include <QObject>
#include <DHAVN_AppSettings_Shared.h>
#include "DHAVN_AppUserManual_Def.h"
#include <DHAVN_AppSettings_Def.h>

/* VehicleVariant */
#define VEHICLE_VARIANT 0       //0x00: DH,  0x01: KH, 0x02: VI

#define FRONT_SCREEN 1
#define REAR_SCREEN 2

#define PDF_LIST            3
#define PDF_SCREEN      4
#define SEARCHVIEW      5

OPTION_MENU_T MAIN_MENU[] =
{
    {MANUALMENU::OPTION_MANUAL_MENU,    "STR_MANUAL_MENU", false, false, false, true, true, NULL, 0, ""},
    {MANUALMENU::OPTION_MANUAL_SEARCH,  "STR_MANUAL_SEARCH", false, false, false, true, true, NULL, 0, ""}
};

OPTION_MENU_T SCREEN_MENU[] =
{
    {MANUALMENU::OPTION_MANUAL_MENU,    "STR_MANUAL_MENU", false, false, false, true, true, NULL, 0, ""},
    {MANUALMENU::OPTION_MANUAL_SEARCH,  "STR_MANUAL_SEARCH", false, false, false, true, true, NULL, 0, ""},
    {MANUALMENU::OPTION_MANUAL_GOTO,    "STR_MANUAL_GOTO", false, false, false, true, true, NULL, 0, ""}
};

OPTION_MENU_T EXIT_SEARCH[] =
{
    {MANUALMENU::OPTION_MANUAL_MENU,    "STR_MANUAL_MENU", false, false, false, true, true, NULL, 0, ""},
    {MANUALMENU::OPTION_MANUAL_EXIT_SEARCH, "STR_MANUAL_SEARCH_EXIT", false, false, false, true, true, NULL, 0, "" }
};

OPTION_MENU_T EXIT_ZOOM[] =
{
    {MANUALMENU::OPTION_MANUAL_MENU,    "STR_MANUAL_MENU", false, false, false, true, true, NULL, 0, ""},
    {MANUALMENU::OPTION_MANUAL_EXIT_ZOOM, "STR_MANUAL_ZOOM_EXIT", false, false, false, true, true, NULL, 0, "" }
};

const QString KCountryVariantId = QString("-cv:");

AppUserManual_Engine :: AppUserManual_Engine( int &argc, char **argv, LANGUAGE_T aLanguage, QTranslator& aTranslator, int evv ,  QObject* aParent )
    :AppVextEngineQML( false, "AppUserManual", aParent ),
      m_pTranslator( aTranslator ),
      m_OptionMenuModel( NULL ),
      m_iEvv(evv)
{
//    SetAppHandlesResponseFg(true); // false : RESPONSE_FG is auto.
    m_nCountryVariant = AppEngineBase::GetCountryVariant();
    m_nLanguage = AppEngineBase::GetLanguage();
    m_nVehicle = VEHICLE_VARIANT;
    LW_LOG("m_nCountryVariant: " << m_nCountryVariant );

    m_nDRS = false;
    m_bEmergency = false;
    m_isCloneMode = false;
    m_isSwapDC = false;
    m_strState = "";
    m_strTempState = "";
    m_strOldState = "";
    m_strRearState = "";
    m_strRearTempState = "";
    m_strRearOldState = "";
    m_nPage = 0;
    m_nTotalPage = 0;
    m_nEntryPoint = 0;
    m_nOldEntryPoint = 0;
    m_nRearEntryPoint = 0;
    m_nRearOldEntryPoint = 0;
    m_nChineseKeypadType = 0;       // 기본값은 0 (병음) – 병음 : 0, 성음 : 1, 필기인식 : 2
    m_bSendChangeEntry = false;
    m_bTouchLock = false;

    signalConnection();
    setSignalHandlerObject( this );

    /* Beep */
    msg = QDBusMessage::createMethodCall("com.lge.car.micom", "/", "com.lge.car.micom.audioif", "ProcessAudioControlMessage");
    QList<QVariant> args;
    args.append( (quint16) 0x0115 );
    args.append( (quint8) 0x0 );
    msg.setArguments( args );

//    athenaOpen();
}

AppUserManual_Engine :: ~AppUserManual_Engine()
{
    setOptionsMenuModel( NULL );
    if( m_OptionMenuModel != NULL )
    {
        delete m_OptionMenuModel;
        m_OptionMenuModel = NULL;
    }
//    athenaClose();
}

void AppUserManual_Engine::savePage( int curPage )
{
    HS_LOG( "savePage() page:" << curPage );
    m_nPage = curPage;
}

QString AppUserManual_Engine::getState()
{
    HS_LOG( "getState() m_strState:" << m_strState );
    return m_strState;
}

QString AppUserManual_Engine::getTempState()
{
    LW_LOG( "getTempState() m_strTempState:" << m_strTempState );
    return m_strTempState;
}

int AppUserManual_Engine::getPage()
{
    HS_LOG( "getPage() page:" << m_nPage );
    return m_nPage;
}

int AppUserManual_Engine::getTotalPage()
{
    LW_LOG( "getTotalPage() m_nTotalPage:" << m_nTotalPage );
    return m_nTotalPage;
}

void AppUserManual_Engine::saveState(QString state ,  int curPage , int totalPages , int screen )
{
    LW_LOG( "saveState() before state: " << m_strState  << " , " << state );
    if ( screen == FRONT_SCREEN ) {
        m_strState.clear();
        m_strState.append(state);
        if ( m_strRearState != "" ) {
            m_strRearState.clear();
            m_strRearState.append(state);
        }
    }
    else {
        m_strRearState.clear();
        m_strRearState.append(state);
        if ( m_strState != "" ) {
            m_strState.clear();
            m_strState.append(state);
        }
    }
//    m_nPage = curPage;
    m_nTotalPage = totalPages;
    if ( state == "pdfScreenView") setViewport( m_nDisplayId , true );
    else setViewport( m_nDisplayId , false );
    HS_LOG( "saveState() after state: " << state << " , page : " << m_nPage );
}

void AppUserManual_Engine::saveTempState(QString state )
{
    LW_LOG( "saveTempState() before tempState: " << m_strTempState );
//    if ( m_strTempState != "" ) m_strTempState = state;
//    if ( m_strRearTempState != "" ) m_strRearTempState = state;
    m_strTempState = state;
    m_strRearTempState = state;
    LW_LOG( "saveTempState() after tempState: " << m_strTempState );
}

void AppUserManual_Engine::playAudioBeep()
{
    LW_LOG( "AppUserManual_Engine :: playAudioBeep()" );
    /* Beep */
    QDBusConnection::sessionBus().call(msg, QDBus::NoBlock);
}

void
AppUserManual_Engine::sendAthenaObject()
{
//    athenaSendObject();
}

int AppUserManual_Engine::getCountyVariant()
{
    return m_nCountryVariant;
}

int AppUserManual_Engine::getLanguage()
{
    return m_nLanguage;
}

void
AppUserManual_Engine::SetLocalization( LANGUAGE_T languageId )
{
    m_nLanguage = languageId;
   QString lang;

   switch ( languageId )
   {
      case LANGUAGE_KO:
       lang = "/app/share/AppUserManual/localization/UserManual_ko";
         break;

      case LANGUAGE_ES:
         lang = "/app/share/AppUserManual/localization/UserManual_es-eu";
         break;

       case LANGUAGE_ES_NA:
          lang = "/app/share/AppUserManual/localization/UserManual_es-na";
          break;

      case LANGUAGE_FR:
         lang = "/app/share/AppUserManual/localization/UserManual_fr-eu";
         break;

       case LANGUAGE_FR_NA:
          lang = "/app/share/AppUserManual/localization/UserManual_fr-na";
          break;

      case LANGUAGE_AR:
         lang = "/app/share/AppUserManual/localization/UserManual_ar";
         break;

      case LANGUAGE_CS:
         lang = "/app/share/AppUserManual/localization/UserManual_cs";
         break;

      case LANGUAGE_DA:
         lang = "/app/share/AppUserManual/localization/UserManual_da";
         break;

      case LANGUAGE_DE:
         lang = "/app/share/AppUserManual/localization/UserManual_de";
         break;

      case LANGUAGE_IT:
         lang = "/app/share/AppUserManual/localization/UserManual_it";
         break;

      case LANGUAGE_NL:
         lang = "/app/share/AppUserManual/localization/UserManual_nl";
         break;

      case LANGUAGE_PL:
         lang = "/app/share/AppUserManual/localization/UserManual_pl";
         break;

      case LANGUAGE_PT:
         lang = "/app/share/AppUserManual/localization/UserManual_pt";
         break;

      case LANGUAGE_RU:
         lang = "/app/share/AppUserManual/localization/UserManual_ru";
         break;

      case LANGUAGE_SK:
         lang = "/app/share/AppUserManual/localization/UserManual_sk";
         break;

      case LANGUAGE_SV:
         lang = "/app/share/AppUserManual/localization/UserManual_sv";
         break;

      case LANGUAGE_TR:
         lang = "/app/share/AppUserManual/localization/UserManual_tr";
         break;

      case LANGUAGE_ZH_CN:
        case LANGUAGE_ZH_YUE:
         lang = "/app/share/AppUserManual/localization/UserManual_zh-cmn";
         break;

       case LANGUAGE_EN_GB:
          lang = "/app/share/AppUserManual/localization/UserManual_en-gb";
          break;

       case LANGUAGE_EN_US:
        default:
          lang = "/app/share/AppUserManual/localization/UserManual_en";
          break;
   }

   if ( m_pTranslator.load( lang ) )
   {
      HS_LOG(  "EVT_LANGUAGE_CHANGED -->" << lang );
      qApp->installTranslator( &m_pTranslator );
      RetranslateUI();
   }
}
void AppUserManual_Engine :: NotifyFromUIStateManagerCommon ( Event aEvent )
{
    LW_LOG( "aEvent : " << aEvent.GetEventId() );
    switch( aEvent.GetEventId() )
    {
        case EVT_CLONE_NOTIFICATION:
        {
            EVENT_CLONE_NOTIFICATION_DATA_T *cloneData = (EVENT_CLONE_NOTIFICATION_DATA_T *)aEvent.Data().data();
            if (cloneData != NULL)
            {
                if (cloneData->sCloneStatus == CLONE_ON_FRONT) {
					/*	TODO: delete this lines(unused pView)
                    QDeclarativeView *pView = NULL;
                    pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
					*/
                    m_nDisplayId = DISPLAY_FRONT;
                    HS_LOG( "EVT_CLONE_NOTIFICATION - CLONE_ON_FRONT" );
                    m_isCloneMode = true;
                }
                else if (cloneData->sCloneStatus == CLONE_ON_REAR) {
					/*	TODO: delete this lines(unused pView)
                    QDeclarativeView *pView = NULL;
                    pView = GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
					*/
                    m_nDisplayId = DISPLAY_REAR;
                    HS_LOG( "EVT_CLONE_NOTIFICATION - CLONE_ON_REAR" );
                    m_isCloneMode = true;
                }
                else if (cloneData->sCloneStatus == CLONE_OFF) {
                    HS_LOG( "EVT_CLONE_NOTIFICATION - CLONE_OFF" );
                    m_isCloneMode = false;
                    SetTouchLock(false);
                    m_bTouchLock = false;
                    emit setTouchLock(m_bTouchLock);
                }
            }
            break;
        }
        case EVT_CAMERA_ENABLE:
        {
//            m_isCamera = true;
            break;
        }
        case EVT_CAMERA_DISABLE:
        {
//            m_isCamera = false;
            break;
        }
        case EVT_SWAP_DC_ENABLED:
        {
            HS_LOG( " ----EVT_SWAP_DC_ENABLED----" );
            m_isSwapDC = true;
            break;
        }
        case EVT_SWAP_DC_DISABLED:
        {
            HS_LOG( " ----EVT_SWAP_DC_DISABLED----" );
            m_isSwapDC = false;
            break;
        }
        case EVT_DRS_SHOW:  // 600
        {
                HS_LOG( " ---EVT_DRS_SHOW" );
//                emit setDrs(true);
        }
        break;
        case EVT_DRS_HIDE:  // 601
        {
                HS_LOG( " ---EVT_DRS_HIDE" );
//                emit setDrs(false);
                if ( m_nDRS ) hideDRS( m_nDisplayId );
//                if ( !m_nDRS ) return;
//                m_nDRS = false;
//                QDeclarativeView *pView = NULL;
//
//                if (m_nDisplayId == DISPLAY_FRONT)
//                    pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
//                else
//                    pView = GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
//
//                if (pView)
//                {
//                    usleep(700000);
//                    HS_LOG( " EVT_DRS_HIDE - unloadDRSPopUp" );
//                    QMetaObject::invokeMethod((QObject*)pView->rootObject(), "unloadDRSPopUp", Qt::QueuedConnection);
//                }
        }
        break;
        case EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT:
        case EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT:
        {
            KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
                if ( message->keyStatus == 1 )
            {
                LW_LOG( "DOWN_RIGHT" );
                emit jog8Dir(MANUALMENU::JOG_BOTTOM_RIGHT);
            }
        }
            break;
        case EVT_KEY_CCP_JOG_DIAL_DOWN_LEFT:
        case EVT_KEY_RRC_JOG_DIAL_DOWN_LEFT:
        {
            KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
                if ( message->keyStatus == 1 )
            {
                LW_LOG( "DOWN_LEFT" );
                emit jog8Dir(MANUALMENU::JOG_BOTTOM_LEFT);
            }
        }
            break;
        case EVT_KEY_CCP_JOG_DIAL_TOP_LEFT:
        case EVT_KEY_RRC_JOG_DIAL_TOP_LEFT:
        {
            KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
                if ( message->keyStatus == 1 )
            {
                LW_LOG( "TOP_LEFT" );
                emit jog8Dir(MANUALMENU::JOG_TOP_LEFT);
            }
        }
            break;
        case EVT_KEY_CCP_JOG_DIAL_TOP_RIGHT:
        case EVT_KEY_RRC_JOG_DIAL_TOP_RIGHT:
        {
            KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
                if ( message->keyStatus == 1 )
            {
                LW_LOG( "TOP_RIGHT" );
                emit jog8Dir(MANUALMENU::JOG_TOP_RIGHT);
            }
        }
            break;
        case EVT_LANGUAGE_CHANGED:
        {
            LW_LOG( "EVT_LANGUAGE_CHANGED" );
              EVENT_LANGUAGE_CHANGED_DATA_T *pLanguageData = (EVENT_LANGUAGE_CHANGED_DATA_T *)aEvent.Data().data();
              SetLocalization(pLanguageData->languageId);
              break;
        }
        case EVT_KEY_HU_BACK:
        case EVT_KEY_CCP_BACK:
        {
            HS_LOG( "EVT_KEY_CCP_BACK" );
            if( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T* eventData = ( KEY_EVENT_DATA_T* ) aEvent.Data().data();
                if( eventData )
                {
                    if( eventData->keyStatus == KEY_STATUS_PRESSED )
                    {
//                        emit showFullScreen();    // fix 5.21
                        if ( !m_isCloneMode ) {
                            emit backkeyPressed( (int)AppEngineQMLConstants::APP_FRONT_SCREEN , true );
                        }
                        else
                            emit backkeyPressed( m_nDisplayId , true );
                        emit pressedBackHK(); // hyeonil.shin [ITS DH_CHN 229944]
                    }
                }
            }
        } break;
        case EVT_KEY_RRC_BACK:
        {
            HS_LOG( "EVT_KEY_RRC_BACK" );
            if( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T* eventData = ( KEY_EVENT_DATA_T* ) aEvent.Data().data();
                if( eventData )
                {
                    if( eventData->keyStatus == KEY_STATUS_PRESSED )
                    {
                        if ( !m_isCloneMode ) {
                                emit backkeyPressed( (int)AppEngineQMLConstants::APP_REAR_SCREEN , false );
                        }
                        else
                            emit backkeyPressed(  m_nDisplayId , false );
                        emit pressedBackHK(); // hyeonil.shin [ITS DH_CHN 229944]
                    }
                }
            }
        } break;

        case EVT_KEY_RRC_JOG_DIAL_UP:
        case EVT_KEY_RRC_JOG_DIAL_DOWN:
        case EVT_KEY_RRC_JOG_DIAL_LEFT:
        case EVT_KEY_RRC_JOG_DIAL_RIGHT:
        case EVT_KEY_RRC_JOG_DIAL_CENTER:
        {
            KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
            if (message->keyStatus == KEY_STATUS_PRESSED ) {
                if ( m_isCloneMode ) {
                    LW_LOG( "##clone## RRC jog pressed - SetTouchLock(true)" );
                    SetTouchLock(true);
                    m_bTouchLock = true;
                    emit setTouchLock(m_bTouchLock);
                }
            } else if (message->keyStatus == KEY_STATUS_RELEASED || message->keyStatus == KEY_STATUS_CANCEL ) {
                LW_LOG( "##clone## RRC jog released - SetTouchLock(false)" );
                SetTouchLock(false);
                m_bTouchLock = false;
                emit setTouchLock(m_bTouchLock);
            }
        }
            break;
        case EVT_KEY_SOFT_MENU:
        {
            HS_LOG( "EVT_KEY_SOFT_MENU" );
            emit menukeyPressed( m_nDisplayId );
        }
            break;

        case EVT_KEY_CCP_MENU:
        {
            HS_LOG( "EVT_KEY_CCP_MENU - " << m_isSwapDC );
            if( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T* eventData = ( KEY_EVENT_DATA_T* ) aEvent.Data().data();

                if( eventData )
                {
                    if( eventData->keyStatus == KEY_STATUS_PRESSED ) {
                        if ( !m_isCloneMode )
                            emit menukeyPressed( (int)AppEngineQMLConstants::APP_FRONT_SCREEN );
                        else
                            emit menukeyPressed( m_nDisplayId );
                    }
                }
            }
            break;
        }
        case EVT_KEY_HU_MENU:
        case EVT_KEY_RRC_MENU:
        {
            HS_LOG( "EVT_KEY_RRC_MENU/EVT_KEY_HU_MENU - " << m_isSwapDC );
            if( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T* eventData = ( KEY_EVENT_DATA_T* ) aEvent.Data().data();

                if( eventData )
                {
                    if( eventData->keyStatus == KEY_STATUS_PRESSED ) {
                        if ( !m_isCloneMode )
                            emit menukeyPressed( (int)AppEngineQMLConstants::APP_REAR_SCREEN );
                        else
                            emit menukeyPressed( m_nDisplayId );
                    }
                }
            }
        }
            break;

        case EVT_KEY_HU_SEEK_PREV:
        case EVT_KEY_SWRC_SEEK_PREV:
        {
            HS_LOG( "EVT_KEY_HU_SEEK_PREV/EVT_KEY_SWRC_SEEK_PREV" );
            if( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T* eventData = ( KEY_EVENT_DATA_T* ) aEvent.Data().data();

                if( eventData )
                {
                    if( eventData->keyStatus == KEY_STATUS_PRESSED )
                        emit prevkeyPressed( m_nDisplayId );
                }
            }
        }
            break;

        case EVT_KEY_RRC_SEEK_PREV:
        {
            HS_LOG( "EVT_KEY_RRC_SEEK_PREV" );
            if( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T* eventData = ( KEY_EVENT_DATA_T* ) aEvent.Data().data();

                if( eventData )
                {
                    if( eventData->keyStatus == KEY_STATUS_PRESSED )
                        emit prevkeyPressed( m_nDisplayId );
                }
            }
        }
            break;
        case EVT_KEY_HU_SEEK_NEXT:
        case EVT_KEY_SWRC_SEEK_NEXT:
        {
            HS_LOG( "EVT_KEY_HU_SEEK_NEXT/EVT_KEY_SWRC_SEEK_NEXT" );
            if( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T* eventData = ( KEY_EVENT_DATA_T* ) aEvent.Data().data();

                if( eventData )
                {
                    if( eventData->keyStatus == KEY_STATUS_PRESSED )
                        emit nextkeyPressed( m_nDisplayId );
                }
            }
        }
            break;
        case EVT_KEY_RRC_SEEK_NEXT:
        {
            HS_LOG( "EVT_KEY_RRC_SEEK_NEXT" );
            if( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T* eventData = ( KEY_EVENT_DATA_T* ) aEvent.Data().data();

                if( eventData )
                {
                    if( eventData->keyStatus == KEY_STATUS_PRESSED )
                        emit nextkeyPressed( m_nDisplayId );
                }
            }
        }
            break;
        case EVT_SETTINGS_CHANGED:
        {
            EVENT_SETTINGS_CHANGED_DATA_T * message = (EVENT_SETTINGS_CHANGED_DATA_T *) aEvent.Data().data() ;
            switch ( message->sKey )
            {
                case SettingsDef::DB_KEY_ENGLISH_KEYPAD:
                case SettingsDef::DB_KEY_KOREAN_KEYPAD:
                case SettingsDef::DB_KEY_ARABIC_KEYPAD:
                case SettingsDef::DB_KEY_CHINA_KEYPAD:
                case SettingsDef::DB_KEY_EUROPE_KEYPAD:
                case SettingsDef::DB_KEY_RUSSIAN_KEYPAD:
                case SettingsDef::DB_KEY_HYUNDAY_KEYPAD:
//                if ( m_strState == "" && m_strRearState == "" ) {
//                    HS_LOG( "EVT_SETTINGS_CHANGED - not FG.. return..." );
//                    return;
//                }
                HS_LOG( "EVT_SETTINGS_CHANGED - message->sKey : " << message->sKey );
                emit updateKeypad(m_nDisplayId);
                    break;
            }
            break;
        }
    default: break;
    }
}

void AppUserManual_Engine :: NotifyFromUIStateManager( Event aEvent )
{
    LW_LOG( "aEvent: " << aEvent.GetEventId()  );
    switch( aEvent.GetEventId() )
    {
        case EVT_KEY_HU_TA:
        {
            HS_LOG( "EVT_KEY_HU_TA " );
            emit showFullScreen();
        }
        break;
        case EVT_EMERGENCY_ACTIVATED:        // 213 (213/2/31/0)
        {
                HS_LOG( "EVT_EMERGENCY_ACTIVATED" );
                m_bEmergency = true;
                QDeclarativeView *pView = NULL;
                pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
                QMetaObject::invokeMethod((QObject*)pView->rootObject(), "setSos", Qt::QueuedConnection);
                pView = GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
                QMetaObject::invokeMethod((QObject*)pView->rootObject(), "setSos", Qt::QueuedConnection);
        }
        break;
        case EVT_EMERGENCY_DEACTIVATED:        // 214 (213/2/31/0)
        {
                HS_LOG( "EVT_EMERGENCY_DEACTIVATED" );
        }
        break;
        case EVT_REQUEST_PRE_FG:
        {
            m_nDisplayId = DISPLAY_FRONT;
            APP_ENTRY_POINT_DATA_T* pData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();
            m_nEntryPoint = pData->sAppEntryPoint;
            QDateTime	dateTime = QDateTime::currentDateTime();
            HS_LOG( "EVT_REQUEST_FG - [ " << dateTime.toString("hh:mm:ss.zzz")  << " ] - EntryPoint : " << m_nEntryPoint << " ,  bTemporalMode: " << pData->sHeader.bTemporalMode  );

            if ( m_nCountryVariant == 0 && m_nLanguage != 2 ) {     // 내수, 한글이 아닌 경우 FG 수신 시 팝업 호출
                emit showDisablePopup(true);
            }
            switch ( pData->sAppEntryPoint )
            {
                case eAPP_EMANUAL_DRS:      // 355
                {
                    HS_LOG( "EVT_REQUEST_FG ::  ===> eAPP_EMANUAL_DRS!!! " );

                    // ITS 267183
                    if ( m_bSendChangeEntry && m_nOldEntryPoint == eAPP_EMANUAL_VRHELP_USAGE_DRS ) {
                        HS_LOG( "EVT_REQUEST_FG ::  ===> changeEntryPoint: " <<  m_nOldEntryPoint << " => " << eAPP_EMANUAL_MAIN );
                        m_bSendChangeEntry = false;
                        m_nOldEntryPoint  = 350;//eAPP_EMANUAL_MAIN;
                        return;
                    }

                    if ( pData->sHeader.bTemporalMode ) {
                        if ( m_strState == "" ) {
                            if ( m_strRearState != "" ) {
                                m_strState = m_strRearState;
                            }
                            else m_strState = m_strOldState;
                        }
                    }
                    else if ( m_strState == "" && m_strRearState != "" ) {
                        m_strState = m_strRearState;
                    }
                    else {
                        emit setDefaultState( m_nDisplayId );
                    }
                    showDRS( m_nDisplayId );
                    if ( m_bSendChangeEntry ) {
                        HS_LOG( "EVT_REQUEST_FG ::  ===> changeEntryPoint: " <<  m_nOldEntryPoint << " => " << eAPP_EMANUAL_MAIN );
                        m_bSendChangeEntry = false;
                        m_nOldEntryPoint  = 350;//eAPP_EMANUAL_MAIN;
                    }
//                    if ( m_strState != "" ) emit setDrsState( FRONT_SCREEN , m_strState , m_strTempState );
                }
                break;
                case eAPP_EMANUAL_MAIN: // 350
                {
                    HS_LOG( "EVT_REQUEST_FG ::  ===> eAPP_EMANUAL_MAIN!!! " );
                    if ( m_bSendChangeEntry ) {
                        HS_LOG( "EVT_REQUEST_FG ::  ===> changeEntryPoint: " <<  m_nOldEntryPoint << " => " << m_nEntryPoint );
                        m_bSendChangeEntry = false;
                        m_nOldEntryPoint  = m_nEntryPoint;
                        emit hideDRSBtVrManual(FRONT_SCREEN);        // BT 도움말 > 카메라 > 카메라 해제 시 주행규제 화면 해제되지 않는 이슈 수정을 위해 필요
                        return;
                    }
                    LW_LOG( "EVT_REQUEST_FG :: ====> eAPP_EMANUAL_MAIN - state : " << m_strState << " , rearState : " << m_strRearState );
                    hideDRS( m_nDisplayId );
                    if ( m_bEmergency ) {
                        HS_LOG( "EVT_REQUEST_FG ::  ===> SOS!! bTemporalMode!!! " );
                        m_bEmergency = false;
                        emit foregroundEvent( FRONT_SCREEN );
                    }
                    else if ( pData->sHeader.bTemporalMode ) {
                        HS_LOG( "EVT_REQUEST_FG ::  ===> bTemporalMode!!! " );
//                        if ( m_strState == "" ) {
                            if ( m_strOldState != "" ) {
                                m_strState = m_strOldState;
                                HS_LOG( "EVT_REQUEST_FG ::  ===> bTemporalMode!!! m_strState : " << m_strState );
                                emit temporalMode( FRONT_SCREEN , m_strState, m_strTempState);
                            }
                            else emit foregroundEvent( FRONT_SCREEN );
                    }
                    else {
                        if ( m_nEntryPoint == m_nOldEntryPoint ) {
                            HS_LOG( "EVT_REQUEST_FG :: ===> same entrypoint.. return..." );
                            return;
                        }
                        if ( m_strState == "" ) {
                            if ( m_strRearState == "" ) {
                                HS_LOG( "EVT_REQUEST_FG ::  ===> foregroundEvent!!! " );
                                m_strState = "pdfListView";
                                emit foregroundEvent( FRONT_SCREEN );
                            }
                            else {
                                HS_LOG( "EVT_REQUEST_FG ::  ===> go rearState!!! m_strRearState : " << m_strRearState );
                                m_strState = m_strRearState;
                                m_strTempState = m_strRearTempState;
                                emit temporalSettingMode( FRONT_SCREEN , m_strState, m_strTempState);
                            }
                        }
                        else {
                            HS_LOG( "EVT_REQUEST_FG ::  ===> go temporalSettingMode!!! m_strState : " << m_strState );
                            emit temporalSettingMode( FRONT_SCREEN , m_strState, m_strTempState);
                        }
                    }
                }
                break;
            case eAPP_EMANUAL_BTHELP_CONNECTION:        // 351
                {
                    HS_LOG( "EVT_REQUEST_FG ::  ===> eAPP_EMANUAL_BTHELP_CONNECTION!!! " );
                    hideDRS( m_nDisplayId );
                    m_strState = "pdfListView";
                    emit btManual( FRONT_SCREEN );
                    ChangeEntry();
                }
                break;
            case eAPP_EMANUAL_VRHELP_USAGE:
                {
                    HS_LOG( "EVT_REQUEST_FG ::  ===> eAPP_EMANUAL_VRHELP_USAGE!!! " );
                    hideDRS( m_nDisplayId );
                    m_strState = "pdfListView";
                    emit vrManual( FRONT_SCREEN );
                    ChangeEntry();
                }
                break;
            case eAPP_EMANUAL_BTHELP_CONNECTION_DRS:
                {
                    HS_LOG( "EVT_REQUEST_FG ::  ===> eAPP_EMANUAL_BTHELP_CONNECTION_DRS!!! " );
//                    showDRS( m_nDisplayId );      // qml onBtDRSManual에서 showDRSPopup 함수 호출
                    emit btDRSManual( FRONT_SCREEN );
//                    if ( m_strState == "" ) {
//                        if ( m_strOldState == "" ) m_strState = "pdfListView";
//                        else m_strState = m_strOldState;
//                    }
                }
                break;
            case eAPP_EMANUAL_VRHELP_USAGE_DRS:
                {
                    HS_LOG( "EVT_REQUEST_FG ::  ===> eAPP_EMANUAL_VRHELP_USAGE_DRS!!! " );
//                    showDRS( m_nDisplayId );      // qml onVrDRSManual showDRSPopup 함수 호출
                    emit vrDRSManual( FRONT_SCREEN );
                }
                break;
            default: break;
            }
            m_nOldEntryPoint  = m_nEntryPoint;
            QDeclarativeView *pView = NULL;
            pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
            pView->repaint();

            m_strOldState = "";
        }
            break;

        case EVT_REQUEST_BG:
        {
            LW_LOG( "EVT_REQUEST_BG" );
            emit backgroundEvent( FRONT_SCREEN );
            if ( m_strRearState == "" ) m_strRearOldState = m_strState;
//            else m_strRearState = m_strState;
            HS_LOG( "EVT_REQUEST_BG  - m_strRearOldState : " << m_strRearOldState << " , m_strState : " << m_strState );
            m_strOldState = m_strState;
            m_strState = "";
            m_nOldEntryPoint = -1;
        }
            break;

        default:
            LW_LOG( "Unhandled Event." << aEvent.GetEventId() );
            break;
    }

//    NotifyFromUIStateManagerGeneric( aEvent, FRONT_SCREEN );
}

void AppUserManual_Engine :: NotifyFromUIStateManagerRear( Event aEvent )
{
    LW_LOG( "aEvent.GetEventId() : " << aEvent.GetEventId()  );
    switch( aEvent.GetEventId() )
    {
        case EVT_EMERGENCY_ACTIVATED:        // 213 (213/2/31/0)
        {
                HS_LOG( "EVT_EMERGENCY_ACTIVATED" );
                m_bEmergency = true;
                QDeclarativeView *pView = NULL;
                pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
                QMetaObject::invokeMethod((QObject*)pView->rootObject(), "setSos", Qt::QueuedConnection);
                pView = GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
                QMetaObject::invokeMethod((QObject*)pView->rootObject(), "setSos", Qt::QueuedConnection);
        }
        break;
        case EVT_EMERGENCY_DEACTIVATED:        // 214 (213/2/31/0)
        {
                HS_LOG( "EVT_EMERGENCY_DEACTIVATED" );
        }
        break;
        case EVT_REQUEST_PRE_FG:
        {
            m_nDisplayId = DISPLAY_REAR;
            APP_ENTRY_POINT_DATA_T* pData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();
            m_nRearEntryPoint =  pData->sAppEntryPoint;
            HS_LOG( "EVT_REQUEST_FG  (Rear) - EntryPoint : " << m_nRearEntryPoint << " , bTemporalMode: " << pData->sHeader.bTemporalMode );
            if ( m_nCountryVariant == 0 && m_nLanguage != 2 ) {     // 내수, 한글이 아닌 경우 FG 수신 시 팝업 호출
                emit showDisablePopup(true);
            }
            switch ( pData->sAppEntryPoint )
            {
                case eAPP_EMANUAL_DRS:
                {
                    HS_LOG( "EVT_REQUEST_FG (Rear) ::  ===>eAPP_EMANUAL_DRS !!! "  );

                    // ITS 267183
                    if ( m_bSendChangeEntry && m_nRearOldEntryPoint == eAPP_EMANUAL_VRHELP_USAGE_DRS ) {
                        HS_LOG( "EVT_REQUEST_FG ::  ===> changeEntryPoint: " <<  m_nRearOldEntryPoint << " => " << eAPP_EMANUAL_MAIN );
                        m_bSendChangeEntry = false;
                        m_nRearOldEntryPoint  = 350;//eAPP_EMANUAL_MAIN;
                        return;
                    }

                    if ( pData->sHeader.bTemporalMode ) {
                        if ( m_strRearState == "" ) {
                            if ( m_strState != "" ) {
                                m_strRearState = m_strState;
                            }
                            else m_strRearState = m_strRearOldState;
                        }
                    }
                    else if ( m_strRearState == "" && m_strState != "" ) {
                        m_strRearState = m_strState;
                    }
                    else {
                        emit setDefaultState( m_nDisplayId );
                    }
                    showDRS( m_nDisplayId );
                    if ( m_bSendChangeEntry ) {
                        HS_LOG( "EVT_REQUEST_FG(Rear) ::  ===> changeEntryPoint: " <<  m_nRearOldEntryPoint << " => " << eAPP_EMANUAL_MAIN );
                        m_bSendChangeEntry = false;
                        m_nRearOldEntryPoint  = 350;//eAPP_EMANUAL_MAIN;
                    }
//                    if ( m_strRearState != "" ) emit setDrsState( REAR_SCREEN , m_strRearState , m_strRearTempState );
                }
                break;
                case eAPP_EMANUAL_MAIN:
                {
                    HS_LOG( "EVT_REQUEST_FG (Rear) ::  ===> eAPP_EMANUAL_MAIN !!! " );
                    if ( m_bSendChangeEntry ) {
                        HS_LOG( "EVT_REQUEST_FG (Rear) ::  ===> changeEntryPoint: " <<  m_nRearOldEntryPoint << " => " << m_nRearEntryPoint );
                        m_bSendChangeEntry = false;
                        m_nRearOldEntryPoint  = m_nRearEntryPoint;
                        emit hideDRSBtVrManual(REAR_SCREEN);        // BT 도움말 > 카메라 > 카메라 해제 시 주행규제 화면 해제되지 않는 이슈 수정을 위해 필요
                        return;
                    }
                    hideDRS( m_nDisplayId );
                    HS_LOG( "EVT_REQUEST_FG (Rear) :: ====> eAPP_EMANUAL_MAIN - state : " << m_strState << " , rearState : " << m_strRearState );
                    if ( m_bEmergency ) {
                        HS_LOG( "EVT_REQUEST_FG(Rear) ::  ===> SOS!! bTemporalMode!!! " );
                        m_bEmergency = false;
                        emit foregroundEvent( REAR_SCREEN );
                    }
                    else if ( pData->sHeader.bTemporalMode ) {
                        HS_LOG( "EVT_REQUEST_FG (Rear) ::  ===> bTemporalMode !!! " );
//                        if ( m_strRearState == "" ) {
                            if ( m_strRearOldState != "" ) {
                                m_strRearState = m_strRearOldState;
                                HS_LOG( "EVT_REQUEST_FG (Rear) ::  ===> bTemporalMode!!! m_strRearState : " << m_strRearState );
                                emit temporalMode( REAR_SCREEN , m_strRearState, m_strRearTempState);
                            }
                            else emit foregroundEvent( REAR_SCREEN );
                    }
                    else {
                            if ( m_nRearEntryPoint == m_nRearOldEntryPoint ) {
                                HS_LOG( "EVT_REQUEST_FG :: ===> same entrypoint.. return..." );
                                return;
                            }
                            if ( m_strRearState == "" ) {
                                if ( m_strState == "" ) {
                                    HS_LOG( "EVT_REQUEST_FG (Rear) ::  ===> foregroundEvent !!! " );
                                    m_strRearState = "pdfListView";
                                    emit foregroundEvent( REAR_SCREEN );
                                }
                                else {
                                    HS_LOG( "EVT_REQUEST_FG (Rear)  ::  ===> go frontState!!! m_strRearState : " << m_strRearState );
                                    m_strRearState = m_strState;
                                    m_strRearTempState = m_strTempState;
                                    emit temporalSettingMode( REAR_SCREEN , m_strRearState, m_strRearTempState);
                                }
                            }
                            else {
                                HS_LOG( "EVT_REQUEST_FG (Rear) ::  ===> go temporalSettingMode !!! " );
                                emit temporalSettingMode( REAR_SCREEN , m_strRearState, m_strRearTempState);
                            }
                        }
                }
                break;
            case eAPP_EMANUAL_BTHELP_CONNECTION:
                {
                    HS_LOG( "EVT_REQUEST_FG (Rear) ::  ===> eAPP_EMANUAL_BTHELP_CONNECTION!!! " );
                    hideDRS( m_nDisplayId );
                    m_strRearState = "pdfListView";
                    emit btManual( REAR_SCREEN );
                    ChangeEntry();
                }
                break;
            case eAPP_EMANUAL_VRHELP_USAGE:
                {
                    HS_LOG( "EVT_REQUEST_FG (Rear) ::  ===> eAPP_EMANUAL_VRHELP_USAGE!!! " );
                    hideDRS( m_nDisplayId );
                    m_strRearState = "pdfListView";
                    emit vrManual( REAR_SCREEN );
                    ChangeEntry();
                }
                break;
            case eAPP_EMANUAL_BTHELP_CONNECTION_DRS:
                {
                    HS_LOG( "EVT_REQUEST_FG (Rear) ::  ===> eAPP_EMANUAL_BTHELP_CONNECTION_DRS!!! " );
//                    showDRS( m_nDisplayId );          // qml onBtDRSManual에서 showDRSPopup 함수 호출
                    emit btDRSManual( REAR_SCREEN );
//                    if ( m_strRearState == "" ) {
//                        if ( m_strRearOldState == "" ) m_strRearState = "pdfListView";
//                        else m_strRearState = m_strRearOldState;
//                    }
                }
                break;
            case eAPP_EMANUAL_VRHELP_USAGE_DRS:
                {
                    HS_LOG( "EVT_REQUEST_FG (Rear) ::  ===> eAPP_EMANUAL_VRHELP_USAGE_DRS!!! " );
//                    showDRS( m_nDisplayId );      // qml onVrDRSManual에서 showDRSPopup 함수 호출
                    emit vrDRSManual( REAR_SCREEN );
//                    emit setVrStatePage();
//                    if ( m_strRearState == "" ) {
//                        if ( m_strRearOldState == "" ) m_strRearState = "pdfListView";
//                        else m_strRearState = m_strRearOldState;
//                    }
                }
                break;
            default: break;
            }
            m_nRearOldEntryPoint  = m_nRearEntryPoint;
            QDeclarativeView *pView = NULL;
            pView = GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
            pView->repaint();

            m_strRearOldState = "";
        }
            break;
        case EVT_REQUEST_BG:
        {
            LW_LOG( "EVT_REQUEST_BG (Rear) " );
            emit backgroundEvent( REAR_SCREEN );
            if ( m_strState == "" ) m_strOldState = m_strRearState;
//            else m_strRearState = m_strState;
            HS_LOG( "EVT_REQUEST_BG (Rear) - m_strOldState : " << m_strOldState << " , m_strRearState : " << m_strRearState );
            m_strRearOldState = m_strRearState;
            m_strRearState = "";
            m_nRearOldEntryPoint = -1;
        }
            break;
        default:
            LW_LOG( "Unhandled Event." << aEvent.GetEventId() );
            break;
    }

}

void AppUserManual_Engine :: HandleSoftKeyHome()
{
    HS_LOG( "HandleSoftKeyHome" );
    Event event( EVT_KEY_SOFT_HOME, GetThisAppId(), APP_UI_MANAGER );
    NotifyUISH( event, m_nDisplayId );
}

void AppUserManual_Engine::ChangeEntry()
{
    HS_LOG( "ChangeEntry() " );
    m_bSendChangeEntry = true;
    Event event( EVT_INFO_E_MANUAL , GetThisAppId(), APP_UI_MANAGER ,  310);//(EVT_COMMAND_T) eGUI_ENTRY_EMANUAL  );
    NotifyUISH ( event, m_nDisplayId );
}

void AppUserManual_Engine :: HandleBackKey( int screenId , bool touch , bool front )
{
    HS_LOG( "HandleBackKey() - screenId : " << screenId <<  " , touch : " << touch << " , front : " << front );
    Event event( EVT_KEY_SOFT_BACK, GetThisAppId(), APP_UI_MANAGER );
    if ( touch && m_isSwapDC) {
        LW_LOG( "AppUserManual_Engine ::  HandleBackKey() - touch && m_isSwapDC , DISPLAY_REAR" );
        NotifyUISH ( event, DISPLAY_REAR );
    }
    else if ( touch ) {
        LW_LOG( "AppUserManual_Engine ::  HandleBackKey() - touch  , DISPLAY_FRONT" );
        NotifyUISH ( event, DISPLAY_FRONT );
    }
    else if ( front ) {
        LW_LOG( "AppUserManual_Engine ::  HandleBackKey() - !bRRC  , DISPLAY_FRONT" );
        NotifyUISH ( event, DISPLAY_FRONT );
    }
    else {
        LW_LOG( "AppUserManual_Engine ::  HandleBackKey() - bRRC  , DISPLAY_REAR" );
        NotifyUISH ( event, DISPLAY_REAR );
    }
}

void AppUserManual_Engine :: onSetOptionMenu(int id_State)
{
    LW_LOG( "AppUserManual_Engine :: onSetOptionMenu"<< "\t" << id_State );

    OPTION_MENU_T* optionsMenu = NULL;
    int optionsSize = 0;

    switch( id_State )
    {
    case MANUALMENU :: MANUAL_MAIN:
        HS_LOG( "MANUAL_MAIN" );
        optionsMenu = MAIN_MENU;
        optionsSize = sizeof(MAIN_MENU)/sizeof( OPTION_MENU_T );
        break;

    case MANUALMENU :: MANUAL_PDF_SCREEN:
        HS_LOG( "MANUAL_PDF_SCREEN" );
        optionsMenu = SCREEN_MENU;
        optionsSize = sizeof(SCREEN_MENU)/sizeof( OPTION_MENU_T );
        break;
    case MANUALMENU :: MANUAL_EXIT_SEARCH:
        HS_LOG( "MANUAL_EXIT_SEARCH" );
        optionsMenu = EXIT_SEARCH;
        optionsSize = sizeof(EXIT_SEARCH)/sizeof( OPTION_MENU_T );
        break;
    case MANUALMENU :: MANUAL_EXIT_ZOOM:
        HS_LOG( "MANUAL_EXIT_ZOOM" );
        optionsMenu = EXIT_ZOOM;
        optionsSize = sizeof( EXIT_ZOOM )/sizeof( OPTION_MENU_T );
        break;
    default:
        HS_LOG( "onSetOptionMenu :: unHandled" );
        break;
    }

    if( m_OptionMenuModel )
        m_OptionMenuModel->updateModel( optionsMenu, optionsSize );
    else
        setOptionsMenuModel(new OptionMenuModel( optionsMenu, optionsSize ));
}

void AppUserManual_Engine :: setOptionsMenuModel( OptionMenuModel* optionMenuModel )
{
    LW_LOG( "setOptionsMenuModel" );
    if( m_OptionMenuModel !=  optionMenuModel)
    {
        m_OptionMenuModel = optionMenuModel;
        emit changeOptionMenuModel();
    }
}

void AppUserManual_Engine :: RetranslateUI()
{
    HS_LOG( "RetranslateUi" );
    emit retranslateUi( ( int ) m_nLanguage );
}

void AppUserManual_Engine :: setFullScreen( bool fullScreen, int screenId )
{
    HS_LOG( "setFullScreen - " << screenId );

	/* unused variable
    DISPLAY_T displayId;

    if( screenId == FRONT_SCREEN )
        displayId = DISPLAY_FRONT;
    else if( screenId == REAR_SCREEN )
        displayId = DISPLAY_REAR;
	*/

    if ( fullScreen )
    {
        if( screenId == FRONT_SCREEN )
            GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN)->setWindowTitle("@OP^SHIDE");
        else if( screenId == REAR_SCREEN )
            GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN)->setWindowTitle("@OP^SHIDE");
    }
    else
    {
        if( screenId == FRONT_SCREEN )
            GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN)->setWindowTitle("@OP^SHOW");
        else if( screenId == REAR_SCREEN )
            GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN)->setWindowTitle("@OP^SHOW");
    }
//    EVT_PUBLIC_ID_T aEventId = ( fullScreen ) ? EVT_HIDE_STATUS_BAR : EVT_SHOW_STATUS_BAR;
//    Event aEvent( aEventId, GetThisAppId(), APP_STATUS_BAR);

//    NotifyUISH( aEvent, displayId );
}

void AppUserManual_Engine :: foregroundEventReceived( int screenId )
{
    LW_LOG( "foregroundEventReceived()" );

    Event aEvent( EVT_RESPONSE_FG, GetThisAppId(), APP_UI_MANAGER );

    if( screenId == FRONT_SCREEN )
        NotifyUISH( aEvent , DISPLAY_FRONT );
    else if( screenId == REAR_SCREEN )
        NotifyUISH( aEvent , DISPLAY_REAR );
}

void AppUserManual_Engine :: launchKeypadSettings( int screenId )
{
    HS_LOG( "launchKeypadSettings()" );

    AppSettingsStartParameter startState;
    startState.startState = AppSettingsDef::eStart_Mode_General_Keyboard;
    Event softEvent( EVT_KEY_SOFT_SETTINGS, GetThisAppId(), APP_UI_MANAGER, eAPP_SETTINGS_GENERAL_KEYBOARD,
                    QByteArray((char *) &startState, sizeof(AppSettingsStartParameter)));
    if( screenId == FRONT_SCREEN )
        emit NotifyUISH( softEvent , DISPLAY_FRONT );
    else if( screenId == REAR_SCREEN )
        emit NotifyUISH( softEvent , DISPLAY_REAR );
}


void AppUserManual_Engine :: signalConnection()
{
    struct sigaction act = {};

    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_flags |= SA_RESETHAND;
    act.sa_handler = AppUserManual_Engine::signalHandler;

    for( int signumber = SIGHUP; signumber < SIGUNUSED; ++signumber )
    {
        if( signumber == SIGKILL || signumber == SIGSTOP )
            continue;
        if( sigaction( signumber, &act, ( struct sigaction* ) NULL ) < 0 )
        {
            HS_LOG( "The signal received is:" << "\t" << signumber );
//            exit( 1 );
        }
    }
}

void AppUserManual_Engine :: setSignalHandlerObject( AppUserManual_Engine *m_appUserManualEngine )
{
    SigM = m_appUserManualEngine;
}

void AppUserManual_Engine :: signalHandler( int sigNum )
{
    CR_LOG( "signalHandler :: signum = %l" << sigNum );
    if ( sigNum == SIGCHLD ) return;
    SigM->restart( sigNum );
    raise(sigNum);
}

void AppUserManual_Engine :: restart( int sigNum )
{
    HS_LOG( "[WDT] restart signum : " << sigNum );
    Event event( EVT_REQUEST_ABNORMAL_TERMINATION, GetThisAppId(), APP_UI_MANAGER, (EVT_COMMAND_T) sigNum );
    NotifyUISH( event, DISPLAY_FRONT );
//    exit( 0 );
}

void AppUserManual_Engine::setViewport( int screenId , bool full )
{
    HS_LOG( "setViewport() - full: " << full );

    QDeclarativeView *pView = NULL;

    if ( screenId == FRONT_SCREEN )
        pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
    else
        pView = GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
    if ( full )
        pView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    else
        pView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

void AppUserManual_Engine::showDRS(DISPLAY_T display)
{
    LW_LOG( "showDRS()" );

    QDeclarativeView *pView = NULL;

    if (display == DISPLAY_FRONT)
    {
        HS_LOG( "FRONT" );
        pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
//        m_nDisplayId = DISPLAY_REAR;
    }
    else
    {
        HS_LOG( "REAR" );
        pView = GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
//        m_nDisplayId = DISPLAY_FRONT;
    }

    if (pView)
    {
        m_nDRS = true;
        QMetaObject::invokeMethod((QObject*)pView->rootObject(), "showDRSPopup", Qt::QueuedConnection);
    }
}

void AppUserManual_Engine::hideDRS(DISPLAY_T display)
{
    if ( !m_nDRS ) return;
    HS_LOG( "hideDRS() display: " << display );
    m_nDRS = false;
}

bool AppUserManual_Engine::getDRSStatus()
{
    LW_LOG( "m_nDRS: " << m_nDRS );
    return m_nDRS;
}

int AppUserManual_Engine::getChineseKeypadType()
{
    HS_LOG( "m_nChineseKeypadType: " << m_nChineseKeypadType );
    return m_nChineseKeypadType;
}

void AppUserManual_Engine::setChineseKeypadType(int chineseKeypadType )
{
    HS_LOG( "chineseKeypadType: " << chineseKeypadType );
    m_nChineseKeypadType = chineseKeypadType;
}

void AppUserManual_Engine::sendChineseKeypad( int chineseKeypadType )
{
    EVENT_SETTINGS_CHANGED_DATA_T message;
    message.sHeader.appIdReciever = APP_NONE;
    message.sHeader.reasonEvent = EVT_SETTINGS_CHANGED;

    message.sKey = SettingsDef::DB_KEY_CHINA_KEYPAD;               // enum DB_SETTINGS_KEY_T (52)
    message.sValue = chineseKeypadType;                               // 위 함수 인자 값

    QByteArray payload( (char *)&message, sizeof(EVENT_SETTINGS_CHANGED_DATA_T) );

    Event event( EVT_POST_EVENT, GetThisAppId(), APP_SETTINGS, EVT_SETTINGS_CHANGED, payload );   // Event Command - EVT_SETTINGS_CHANGED

    if ( !NotifyUISH(event, m_nDisplayId) ) HS_LOG( "sendChineseKeypad() - Event notification failed" );

}

int AppUserManual_Engine::CheckVehicleStatus(){
    return m_nVehicle;
}

int AppUserManual_Engine::getHWType()
{
    return m_iEvv;
}

void AppUserManual_Engine::logForQML( QString msg )
{
//    HS_LOG("[QML] " << msg );
}
