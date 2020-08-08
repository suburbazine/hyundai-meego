#include <DHAVN_AppHomeScreen_AppEngine.h>
//#include <athena.h>

extern int Command_Field;
extern QString ViewID;



#define SHM_LANGUAGE_OPTION_SIZE                    (sizeof(char)*4)
#define SHM_SYS_OPTION_SIZE                         (sizeof(char)*9)
#define SHM_SYS_BOOT_TYPE_SIZE                      (sizeof(char)*4)
#define SHM_LAST_STATUS_SIZE                        (sizeof(char)*3)
#define SHM_AUDIO_LAST_MEMORY_SIZE                  (sizeof(char)*8)
#define SHM_CURRENT_AV_MODE_SIZE                    (sizeof(char)*4)
#define SHM_POPUP_STATUS_SIZE                       (sizeof(char)*4)
#define SHM_MEDIA_WRAPPER_OWNER_SIZE                (sizeof(char)*4)

#define SHM_OFFSET_LANGUAGE_OPTION                  (0)
#define SHM_OFFSET_SYS_OPTION                       (SHM_LANGUAGE_OPTION_SIZE)
#define SHM_OFFSET_SYS_BOOT_TYPE                    (SHM_OFFSET_SYS_OPTION+SHM_SYS_OPTION_SIZE)
#define SHM_OFFSET_LAST_STATUS                      (SHM_OFFSET_SYS_BOOT_TYPE+SHM_SYS_BOOT_TYPE_SIZE)
#define SHM_OFFSET_AUDIO_LAST_MEMORY                (SHM_OFFSET_LAST_STATUS+SHM_LAST_STATUS_SIZE)
#define SHM_OFFSET_CURRENT_AV_MODE                  (SHM_OFFSET_AUDIO_LAST_MEMORY+SHM_AUDIO_LAST_MEMORY_SIZE)
#define SHM_OFFSET_POPUP_STATUS                     (SHM_OFFSET_CURRENT_AV_MODE+SHM_CURRENT_AV_MODE_SIZE)
#define SHM_OFFSET_MEDIA_WRAPPER_OWNER              (SHM_OFFSET_POPUP_STATUS+SHM_MEDIA_WRAPPER_OWNER_SIZE)
#define SHM_TOTAL_SIZE                              (SHM_OFFSET_MEDIA_WRAPPER_OWNER+SHM_POPUP_STATUS_SIZE)

#define HOME_MENU_MAIN  0
#define HOME_MENU_SUB   1
#define HOME_MENU_HELP  2

struct APP_MAP_T
{
    /** Internal HomeScreen application Id */
    HSDefP::APP_ID_T appId;
    /** Event Id for showing app */
    EVT_ID_T event;
    /** Application name */
    QString appName;
};

const APP_MAP_T APP_LIST[] =
{
    /** iBox applications */
    { HSDefP::APP_ID_BLUELINK_ASSISTANT,        EVT_KEY_SOFT_IBOX_APP_BLUELINK_ASSISTENT,        "BLUELINK ASSISTANT"     },
    { HSDefP::APP_ID_BLUELINK_SETTINGS,         EVT_KEY_SOFT_IBOX_APP_BLUELINK_SETTING,          "BLUELINK SETTINGS"      },

    { HSDefP::APP_ID_APP_STORE,                 EVT_KEY_SOFT_IBOX_APP_APPLICATIONS,              "APP_STORE"              },

    { HSDefP::APP_ID_DOWNLOAD_APP,              EVT_KEY_SOFT_IBOX_APP_STORE       ,              "DOWNLOAD APP"           },
    { HSDefP::APP_ID_MODEM_PHONE_MAIN,          EVT_KEY_SOFT_IBOX_APP_BLUELINK_PHONE,            "MODEM PHONE MAIN"       },
    { HSDefP::APP_ID_MODEM_DIAL,                EVT_KEY_SOFT_IBOX_APP_BLUELINK_PHONE,            "MODEM DIAL"             },
    { HSDefP::APP_ID_MODEM_PHONEBOOK,           EVT_KEY_SOFT_IBOX_APP_BLUELINK_PHONE,            "MODEM PHONEBOOK"        },
    { HSDefP::APP_ID_MODEM_CALL_HISTORY,        EVT_KEY_SOFT_IBOX_APP_BLUELINK_PHONE,            "MODEM CALL HISTORY"   },
    { HSDefP::APP_ID_WIFI_SETTINGS,             EVT_KEY_SOFT_IBOX_APP_WIFI_SETTINGS,             "WIFI_SETTINGS"          },

    { HSDefP::APP_ID_DRIVING_INFO,              EVT_KEY_SOFT_IBOX_APP_ECO_DRIVING_INFORMATION,   "DRIVING INFO"           },

    { HSDefP::APP_ID_VEHICLE_DIAGNOSTICS,       EVT_KEY_SOFT_IBOX_APP_VEHICLE_DIAGNOSTICS,       "VEHICLE DIAGNOSTICS"    },
    { HSDefP::APP_ID_SCHEDULED_DIAGNOSTICS,     EVT_KEY_SOFT_IBOX_APP_SCHEDULED_DIAGNOSTICS,     "SCHEDULED DIAGNOSTICS"  },
    { HSDefP::APP_ID_MAINTENANCE,               EVT_KEY_SOFT_IBOX_APP_MAINTENANCE_ALERT,         "MAINTENANCE"            },

    { HSDefP::APP_ID_SEND_TO_CAR,               EVT_KEY_SOFT_IBOX_APP_POI_INFORMATION,           "SEND TO CAR"            },
    { HSDefP::APP_ID_DAILY_ROUTE_GUIDANCE,      EVT_KEY_SOFT_IBOX_APP_CONSUMABLES_MANAGEMENT,    "DAILY ROUTE GUIDANCE"   },
    { HSDefP::APP_ID_BLUELINK_CENTER,           EVT_KEY_SOFT_IBOX_APP_BLUELINK_CENTER,           "BLUELINK CENTER"        },
    /**  Media */
    { HSDefP::APP_ID_AUX,                       EVT_KEY_SOFT_AVP,                                "AVP"                    },
    /**  Video */
    { HSDefP::APP_ID_VIDEO_JBOX,                EVT_KEY_SOFT_AVP,                                "AVP"                    },
    { HSDefP::APP_ID_VIDEO_USB1,                EVT_KEY_SOFT_AVP,                                "AVP"                    },
    { HSDefP::APP_ID_VIDEO_USB2,                EVT_KEY_SOFT_AVP,                                "AVP"                    },
    { HSDefP::APP_ID_VIDEO_DISC,                EVT_KEY_SOFT_AVP,                                "AVP"                    },
    /** Photo */
    { HSDefP::APP_ID_PHOTO_JBOX,                EVT_KEY_SOFT_AVP,                                "AVP"                    },
    { HSDefP::APP_ID_PHOTO_USB1,                EVT_KEY_SOFT_AVP,                                "AVP"                    },
    { HSDefP::APP_ID_PHOTO_USB2,                EVT_KEY_SOFT_AVP,                                "AVP"                    },
    /** Audio */
    { HSDefP::APP_ID_AUDIO_JBOX,                EVT_KEY_SOFT_AVP,                                "AVP"                    },
    { HSDefP::APP_ID_AUDIO_USB1,                EVT_KEY_SOFT_AVP,                                "AVP"                    },
    { HSDefP::APP_ID_AUDIO_USB2,                EVT_KEY_SOFT_AVP,                                "AVP"                    },
    { HSDefP::APP_ID_AUDIO_IPOD1,               EVT_KEY_SOFT_AVP,                                "AVP"                    },
    { HSDefP::APP_ID_AUDIO_IPOD2,               EVT_KEY_SOFT_AVP,                                "AVP"                    },
    { HSDefP::APP_ID_AUDIO_DISC,                EVT_KEY_SOFT_AVP,                                "AVP"                    },
    { HSDefP::APP_ID_AUDIO_BLUETOOTH,           EVT_KEY_SOFT_AVP,                                "AVP"                    },

    /** LGe applications */
    { HSDefP::APP_ID_NAVI,                      EVT_KEY_SOFT_NAVI,                              "NAVI"                   },
    { HSDefP::APP_ID_BT_PHONE,                  EVT_KEY_SOFT_BTPHONE,                           "BT_PHONE"               },
    { HSDefP::APP_ID_DMB,                       EVT_KEY_SOFT_MOBILETV,                          "DMB"                   },
    { HSDefP::APP_ID_RADIO,                     EVT_KEY_SOFT_RADIO,                             "RADIO"                  },
    { HSDefP::APP_ID_SIRIUS_XM_FROM_HOME,       EVT_KEY_SOFT_XM,                                "XM"                     },
    //{ HSDefP::APP_ID_ENGINEERING,               EVT_REQ_ENG_MODE_ENTER,                         "ENGINEERING"            },
    { HSDefP::APP_ID_XM_DATA,                   EVT_KEY_SOFT_XM_DATA,                           "XM DATA"                },
    { HSDefP::APP_ID_PANDORA1,                  EVT_KEY_SOFT_PANDORA,                           "PANDORA"                },
    { HSDefP::APP_ID_PANDORA2,                  EVT_KEY_SOFT_PANDORA,                           "PANDORA"                },
    { HSDefP::APP_ID_BT_PANDORA,                EVT_KEY_SOFT_PANDORA,                           "PANDORA"                },
    { HSDefP::APP_ID_AHA1,                      EVT_KEY_SOFT_AHA,                               "AHA_RADIO"                },
    { HSDefP::APP_ID_AHA2,                      EVT_KEY_SOFT_AHA,                               "AHA_RADIO"                },
    { HSDefP::APP_ID_BT_AHA,                    EVT_KEY_SOFT_AHA,                               "AHA_RADIO"                },
    { HSDefP::APP_ID_SETTINGS_BLUETOOTH,        EVT_KEY_SOFT_BT_SETTINGS,                       "BT SETTINGS"            },
    { HSDefP::APP_ID_SETTINGS_NAVI,             EVT_KEY_SOFT_NAVI_SETTINGS,                     "NAVI SETTINGS"          },

    { HSDefP::APP_ID_VIDEO_QUICK_GUIDE,         EVT_INFO_INTERACTIVE_GUIDE,                     "VIDEO QUICK GUIDE"      },
    { HSDefP::APP_ID_E_MANUAL,                  EVT_INFO_E_MANUAL,                              "E-MANUAL"               },



    { HSDefP::APP_ID_MAX,                      EVT_MAX,                                        ""                       }
};

AppEngine::AppEngine( InitData* pInitData, LocalizeTrigger *pLocTrigger, QObject *pParent )
    : AppVextEngineQML( false, "AppHomeMenu", pParent ),
      m_pInitData( pInitData ),
      m_pViewControllerFront( NULL ),
      m_pViewControllerRear( NULL ),
      m_bDCSwapped(false),
      m_bViewContentSwapped(false),
      m_pLocTrigger( pLocTrigger ),
      m_bAppLaunching_F(false),
      m_bAppLaunching_R(false),
      m_nCountryVariant(0),
      m_uishSharedMemory("UISHStatusInformation"),
      m_eLastEntryPoint_F(eAPP_NONE),
      m_eLastEntryPoint_R(eAPP_NONE),
      m_nMenuState_F(HOME_MENU_MAIN),
      m_nMenuState_R(HOME_MENU_MAIN)
{
    //athenaOpen();

    setCountryVariant((int)AppEngineBase::GetCountryVariant());

    setBeepCommand();

    m_pViewControllerFront = new ViewController( DISPLAY_FRONT ,GetScreenContext() , pInitData, pLocTrigger );
    m_pViewControllerRear = new ViewController( DISPLAY_REAR, GetScreenContext(AppEngineQMLConstants::APP_REAR_SCREEN) , pInitData, pLocTrigger );


    QDeclarativeContext *ctxt = GetScreenContext();
    ctxt->setContextProperty( "ViewControll", m_pViewControllerFront );

    QDeclarativeContext *ctxt2 = GetScreenContext(AppEngineQMLConstants::APP_REAR_SCREEN);
    ctxt2->setContextProperty( "ViewControll", m_pViewControllerRear );

    m_uishSharedMemory.attach(QSharedMemory::ReadOnly);



    // for SW WDT
    signalConnection();
    setSignalHandlerObject(this);
}


AppEngine::~AppEngine()
{
    if(m_pViewControllerFront != NULL )
    {
        delete m_pViewControllerFront;
        m_pViewControllerFront = NULL;
    }

    if(m_pViewControllerRear != NULL )
    {
        delete m_pViewControllerRear;
        m_pViewControllerRear = NULL;
    }

    //athenaClose();
}



void
AppEngine::NotifyFromUIStateManager( Event aEvent )
{
    NotifyFromUIStateManagerGeneric( aEvent, m_pViewControllerFront, AppEngineQMLConstants::SCREEN_FRONT );
}

void
AppEngine::NotifyFromUIStateManagerRear( Event aEvent )
{
    NotifyFromUIStateManagerGeneric( aEvent, m_pViewControllerRear, AppEngineQMLConstants::SCREEN_REAR );
}

void AppEngine::NotifyFromUIStateManagerCommon( Event aEvent )
{
    QTime time = QTime::currentTime();
    LW_LOG( "Event Id = " << aEvent.GetEventId() <<
           " Sender = " << aEvent.GetSource() <<
           " Receiver = " << aEvent.GetTarget() <<
           " Command " << aEvent.GetCommand() <<
           ",  time = " << time.hour() << ":" << time.minute() << ":" << time.second() );

    switch ( aEvent.GetEventId() )
    {

    case EVT_FACTORY_RESET_REQUEST:
    {
        HS_LOG ("EngineerAppEngine::Request Factory Reset Message From UISH");

        Event event(EVT_FACTORY_RESET_RESPONSE, GetThisAppId(), APP_UI_MANAGER);
        if(! NotifyUISHCommon(event))
        {
            HS_LOG("Factory Reset Response Failed");
        }
        break;
    }

    case EVT_LANGUAGE_CHANGED:
    {
        LW_LOG( "EVT_LANGUAGE_CHANGED" );

        if (aEvent.Data().size() == sizeof(EVENT_LANGUAGE_CHANGED_DATA_T))
        {
            EVENT_LANGUAGE_CHANGED_DATA_T *pLanguageData = NULL;
            pLanguageData = (EVENT_LANGUAGE_CHANGED_DATA_T *)aEvent.Data().data();
            HS_LOG( "Changed Language = " << AppEngineBase::GetLanguage() );

            if( pLanguageData )
            {
                HS_LOG( "VR Icon Set : " << m_pInitData->GetAppData()->GetSettingsVRIconInfo(pLanguageData->languageId) );

                m_pLocTrigger->SetLanguage( pLanguageData->languageId );

                m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_ENABLED_VR,
                                                     QVariant( m_pInitData->GetAppData()->GetSettingsVRIconInfo(pLanguageData->languageId) ) );

                m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_ENABLED_HELP,
                                                     QVariant( m_pInitData->GetAppData()->GetSettingsHELPIconInfo(pLanguageData->languageId) ) );

                if (m_pViewControllerFront->GetPopUpType() == HSDefP::POPUP_VR_NOT_SUPPORT)
                {
                    if (m_pInitData->GetAppData()->GetValue(HSDefP::APP_DATA_ENABLED_VR).toBool())
                    {
                        m_pViewControllerFront->SetPopUpType(HSDefP::POPUP_INVALID);
                        emit closepopup(1);
                    }
                }

                if (m_pViewControllerRear->GetPopUpType() == HSDefP::POPUP_VR_NOT_SUPPORT)
                {
                    if (m_pInitData->GetAppData()->GetValue(HSDefP::APP_DATA_ENABLED_VR).toBool())
                    {
                        m_pViewControllerRear->SetPopUpType(HSDefP::POPUP_INVALID);
                        emit closepopup(2);
                    }
                }

                if (m_pViewControllerFront->GetPopUpType() == HSDefP::POPUP_HELP_NOT_SUPPORT)
                {
                    if (m_pInitData->GetAppData()->GetValue(HSDefP::APP_DATA_ENABLED_HELP).toBool())
                    {
                        m_pViewControllerFront->SetPopUpType(HSDefP::POPUP_INVALID);
                        emit closepopup(1);
                    }
                }

                if (m_pViewControllerRear->GetPopUpType() == HSDefP::POPUP_HELP_NOT_SUPPORT)
                {
                    if (m_pInitData->GetAppData()->GetValue(HSDefP::APP_DATA_ENABLED_HELP).toBool())
                    {
                        m_pViewControllerRear->SetPopUpType(HSDefP::POPUP_INVALID);
                        emit closepopup(2);
                    }
                }

            }
        }
        else
        {
            CR_LOG("event data size error");
        }

        break;
    }


    case EVT_KEY_CCP_BACK:
    {
        HS_LOG( "EVT_KEY_CCP_BACK" );

        ViewController * pViewControllerLocal = GetViewController( AppEngineQMLConstants::SCREEN_FRONT);

        if( !aEvent.Data().isEmpty() )
        {
            emit signalBackKey();

            KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();

            if( message && ( KEY_STATUS_PRESSED == message->keyStatus ) )
            {
                if (pViewControllerLocal->GetPopUpType() != HSDefP::POPUP_INVALID )
                {
                    HS_LOG( "emit closepopup" );
                    pViewControllerLocal->SetPopUpType(HSDefP::POPUP_INVALID);
                    emit closepopup(1);
                }
                else
                {
                    BackKeyHandler( (int)AppEngineQMLConstants::SCREEN_FRONT );
                }
            }
        }

        break;
    }

    case EVT_KEY_RRC_BACK:
    {
        HS_LOG( "EVT_KEY_RRC_BACK" );

        ViewController * pViewControllerLocal = GetViewController( AppEngineQMLConstants::SCREEN_REAR);

        if( !aEvent.Data().isEmpty() )
        {
            emit signalBackKey();

            KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();

            if( message && ( KEY_STATUS_PRESSED == message->keyStatus ) )
            {
                if (pViewControllerLocal->GetPopUpType() != HSDefP::POPUP_INVALID )
                {
                    HS_LOG( "emit closepopup" );
                    pViewControllerLocal->SetPopUpType(HSDefP::POPUP_INVALID);
                    emit closepopup(2);
                }
                else
                {
                    BackKeyHandler( (int)AppEngineQMLConstants::SCREEN_REAR );
                }
            }
        }

        break;
    }

    case EVT_DRS_SHOW:
    case EVT_DRS_HIDE:
    {
        LW_LOG( "EVT_DRS_SHOW/EVT_DRS_HIDE" );

        bool isParkingMode = (aEvent.GetEventId() == EVT_DRS_SHOW ? false : true);
        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_ENABLED_PARKED, QVariant( isParkingMode ) );

        /*
        ViewController * pViewControllerLocal = GetViewController( AppEngineQMLConstants::SCREEN_FRONT);
        if( pViewControllerLocal->IsDrivingRestrictView() && !isParkingMode )
            pViewControllerLocal->ShowRootView();
            */
        break;
    }

    case EVT_CLONE_NOTIFICATION:
    {
        EVENT_CLONE_NOTIFICATION_DATA_T *pCloneData = (EVENT_CLONE_NOTIFICATION_DATA_T *)aEvent.Data().data();
        if (pCloneData != NULL)
        {
            if (pCloneData->sCloneStatus == CLONE_ON_FRONT || pCloneData->sCloneStatus == CLONE_ON_REAR) {
                m_pViewControllerFront->SetForeground( false );
                m_pViewControllerRear->SetForeground( false );
            }

        }
        break;
    }

    case EVT_SWAP_DC_ENABLED:
    {
        LW_LOG( "EVT_SWAP_DC_ENABLED " );
        m_bDCSwapped = true;
        UpdateViewControllerMapping();
        break;
    }

    case EVT_SWAP_DC_DISABLED:
    {
        LW_LOG( "EVT_SWAP_DC_DISABLED " );
        m_bDCSwapped = false;
        UpdateViewControllerMapping();
        break;
    }

    case EVT_MEDIA_ENABLED :
    {
        LW_LOG( "EVT_MEDIA_ENABLED " );
        EVT_MEDIA_ENABLE_DATA_T *pData = (EVT_MEDIA_ENABLE_DATA_T *)aEvent.Data().data();
        updateMediaMenuIcons(pData);
        m_pViewControllerFront->setMediaInfoUpdated(true);
        m_pViewControllerRear->setMediaInfoUpdated(true);
        break;
    }

        /*
    case EVT_MMC_MOUNTED:
    {
        HS_LOG( "EVT_MMC_MOUNTED");
        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_ENABLED_NAVI, true );
        break;
    }

    case EVT_MMC_REMOVED:
    {
        HS_LOG( "EVT_MMC_REMOVED");
        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_ENABLED_NAVI, false );
        break;
    }
    */

    }

}

void
AppEngine::NotifyFromUIStateManagerGeneric( Event aEvent, ViewController *pViewController, AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T nScreen )
{
    if( NULL == pViewController ) return;

    QTime time = QTime::currentTime();
    LW_LOG( "Event Id = " << aEvent.GetEventId() <<
           " Sender = " << aEvent.GetSource() <<
           " Receiver = " << aEvent.GetTarget() <<
           " Command " << aEvent.GetCommand() <<
           ",  time = " << time.hour() << ":" << time.minute() << ":" << time.second() );

    switch ( aEvent.GetEventId() )
    {

    case EVT_REQUEST_PRE_FG:
    {
        QTime time = QTime::currentTime();
        HS_LOG( "EVT_REQUEST_PRE_FG (" << time.hour() << ":" << time.minute() << ":" << time.second() <<")");

        appLaunching(false, (int)nScreen);

        if( !aEvent.Data().isEmpty() )
        {
            APP_ENTRY_POINT_DATA_T *pData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();

            if (pData)
            {
                /*
                if (nScreen == AppEngineQMLConstants::APP_FRONT_SCREEN)
                {
                    if (m_eLastEntryPoint_F != pData->sAppEntryPoint)
                    {
                        m_eLastEntryPoint_F = pData->sAppEntryPoint;
                    }
                    else
                    {
                        if (pData->sHeader.bTemporalMode)
                            return;
                    }
                }
                else if (nScreen == AppEngineQMLConstants::APP_FRONT_SCREEN)
                {
                    if (m_eLastEntryPoint_R != pData->sAppEntryPoint)
                    {
                        m_eLastEntryPoint_R = pData->sAppEntryPoint;
                    }
                    else
                    {
                        if (pData->sHeader.bTemporalMode)
                            return;
                    }
                }
                */

                if( pData && pData->sHeader.appIdReciever == APP_MAIN_MENU )
                {
                    if (pViewController->GetPopUpType() != HSDefP::POPUP_INVALID )
                    {
                        pViewController->SetPopUpType(HSDefP::POPUP_INVALID);
                        emit closepopup(nScreen);
                    }
                    ProcessEntryPoint( pData->sAppEntryPoint, pViewController );
                }
            }
        }

        emit receiveFGEvent((int)nScreen);

        pViewController->SetForeground( true );

        QDeclarativeView* pView =  GetDeclarativeView((AppEngineQMLConstants::EAPP_SCREEN_USAGE_T)nScreen);
        if (pView) pView->repaint();

        break;
    }

    case EVT_REQUEST_BG:
        HS_LOG( "EVT_REQUEST_BG ");
        appLaunching(false, (int)nScreen);

        emit receiveBGEvent((int)nScreen);

        pViewController->SetForeground( false );
        break;

    case EVT_SHOW_POPUP:
        HS_LOG( "EVT_SHOW_POPUP");
        appLaunching(false, (int)nScreen);
        break;

    case EVT_MMC_MOUNTED:
        HS_LOG( "EVT_MMC_MOUNTED");
        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_ENABLED_NAVI, true );
        break;

    case EVT_MMC_REMOVED:
#if 1
        Command_Field = aEvent.GetCommand();
        if( aEvent.GetCommand() == 0 )  // SD 카드 인증 실패
        {
            HS_LOG( "EVT_MMC_REMOVED ( Failed SD Card Certification )");
        }
        else if( aEvent.GetCommand() == 1 ) // SD 카드 실제로 제거
        {
            HS_LOG( "EVT_MMC_REMOVED");
            m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_ENABLED_NAVI, false );
        }
        else if( aEvent.GetCommand() == 2 ) // DH SD 가 아닐경우
        {
            HS_LOG( "EVT_MMC_REMOVED ( This is not a DH SD Card )");
            m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_ENABLED_NAVI, false );
        }
        break;
#else
        HS_LOG( "EVT_MMC_REMOVED");
        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_ENABLED_NAVI, false );
        break;
#endif

    case EVT_SHOW_LOCAL_POPUP:
    {
        appLaunching(false, (int)nScreen);

        int popupType = aEvent.GetCommand();
        HS_LOG( "EVT_SHOW_LOCAL_POPUP  type =" << popupType);
        switch (popupType)
        {
        case 0:  // POPUP_LOCAL_IGN
            pViewController->SetPopUpType(HSDefP::POPUP_LOCAL_IGN);
            break;
        case 1: // POPUP_LOCAL_BLUELINK
            pViewController->SetPopUpType(HSDefP::POPUP_LOCAL_BLUELINK);
            break;
        case 2: // POPUP_LOCAL_DRS
            pViewController->SetPopUpType(HSDefP::POPUP_LOCAL_DRS);
            break;
        }
    }

        break;

    case EVT_HIDE_LOCAL_POPUP:
        HS_LOG( "EVT_HIDE_LOCAL_POPUP");
        pViewController->SetPopUpType(HSDefP::POPUP_INVALID);
        emit closepopup(nScreen);
        break;

    default:
        break;
    }
}

void AppEngine::hideLocalPopup(int screen)
{
    HS_LOG( "--->  screen = " << screen );

    if (screen == 1)  // front
    {
        m_pViewControllerFront->SetPopUpType(HSDefP::POPUP_INVALID);
        emit closepopup(1);
    }
    else if (screen == 2)
    {
        m_pViewControllerRear->SetPopUpType(HSDefP::POPUP_INVALID);
        emit closepopup(2);
    }
}


// following methods will be invoked from QML, when applucation wishes to launch Music App
// application and jump to background.
void
AppEngine::LaunchApplication( int nAppId, int nViewId, int nScreen, int nScreenContent, QString sViewName )
{
    //PROCESS_EVENT_CALL

    if(nScreen == 1)
    {
        if (m_bAppLaunching_F)
            return;
    }

    else if (nScreen == 2)
    {
        if (m_bAppLaunching_R) return;
    }

    HS_LOG( "nAppId = " << nAppId << " nViewId = " << nViewId << " screen = " << nScreen << " screen content = " << nScreenContent );

    /*
    ViewController *pViewController = GetViewController( AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T( nScreen ) );

    if ( ( (nAppId == HSDefP::APP_ID_MODEM_DIAL) || (nAppId == HSDefP::APP_ID_BLUELINK_CENTER) )
            && ( nScreenContent == AppEngineQMLConstants::SCREEN_REAR ) )

    {
        pViewController->ShowPopUp( nAppId, nViewId );
        return;
    }
    */

    if ( HSDefP::APP_ID_T( nAppId ) == HSDefP::APP_ID_INVALID )
    {
        loadSubMenu(nViewId, nScreen, sViewName);
        return;
    }

    APP_ENTRY_POINT_T nState = ( APP_ENTRY_POINT_T ) EVT_COMMAND_INVALID;

    appLaunching(true, (int)nScreen);

    //m_appLaunchDataResetTimer.start();
    QTimer::singleShot(1500, this, SLOT(resetAppLaunchData()));

    switch( nAppId )
    {

    case HSDefP::APP_ID_RADIO :
        nState = eAPP_RADIO;
        break;

    case HSDefP::APP_ID_DMB :
        nState = eAPP_DMB;
        break;

    case HSDefP::APP_ID_SETTINGS_SOUND:
        LaunchHMISettings( 250, nScreen );  //eAPP_SETTINGS_SOUND_FROM_HOME
        return;

    case HSDefP::APP_ID_SETTINGS_SCREEN:
        LaunchHMISettings( eAPP_SETTINGS_SCREEN, nScreen );
        return;

    case HSDefP::APP_ID_SETTINGS_VR:
        LaunchHMISettings( eAPP_SETTINGS_VR, nScreen );
        return;

    case HSDefP::APP_ID_SETTINGS_CLOCK:
        LaunchHMISettings( eAPP_SETTINGS_GENERAL_CLOCK, nScreen );
        return;

    case HSDefP::APP_ID_SETTINGS_GENERAL_BASIC:
        LaunchHMISettings( eAPP_SETTINGS_GENERAL, nScreen );
        return;

    case HSDefP::APP_ID_SETTINGS_GENERAL_SYSTEM:
        LaunchHMISettings( eAPP_SETTINGS_SYSTEM, nScreen );
        return;

    case HSDefP::APP_ID_AUDIO_JBOX:
        nState = eAPP_AVP_AUDIO_MODE_JUKEBOX;
        break;

    case HSDefP::APP_ID_AUDIO_USB1:
        nState = eAPP_AVP_AUDIO_MODE_USB1;
        break;

    case HSDefP::APP_ID_AUDIO_USB2:
        nState = eAPP_AVP_AUDIO_MODE_USB2;
        break;

    case HSDefP::APP_ID_AUDIO_IPOD1:
        nState = eAPP_AVP_AUDIO_MODE_IPOD1;
        break;

    case HSDefP::APP_ID_AUDIO_IPOD2:
        nState = eAPP_AVP_AUDIO_MODE_IPOD2;
        break;

    case HSDefP::APP_ID_AUDIO_BLUETOOTH:
        nState = eAPP_AVP_MODE_BLUETOOTH;
        break;

    case HSDefP::APP_ID_AUDIO_DISC:
        nState = eAPP_AVP_AUDIO_MODE_DISC;
        break;

    case HSDefP::APP_ID_AUX:
        nState = eAPP_AVP_AUDIO_MODE_AUX;
        break;

    case HSDefP::APP_ID_VIDEO_JBOX:
        nState = eAPP_AVP_VIDEO_MODE_JUKEBOX;
        break;

    case HSDefP::APP_ID_VIDEO_USB1:
        nState = eAPP_AVP_VIDEO_MODE_USB1;
        break;

    case HSDefP::APP_ID_VIDEO_USB2:
        nState = eAPP_AVP_VIDEO_MODE_USB2;
        break;

    case HSDefP::APP_ID_VIDEO_DISC:
        nState = eAPP_AVP_VIDEO_MODE_DISC;
        break;

    case HSDefP::APP_ID_PHOTO_JBOX:
        nState = eAPP_AVP_PHOTO_MODE_JUKEBOX;
        break;

    case HSDefP::APP_ID_PHOTO_USB1:
        nState = eAPP_AVP_PHOTO_MODE_USB1;
        break;

    case HSDefP::APP_ID_PHOTO_USB2:
        nState = eAPP_AVP_PHOTO_MODE_USB2;
        break;

    case HSDefP::APP_ID_PANDORA1:
        nState = eAPP_PANDORA_MODE_IPOD1;
        break;

    case HSDefP::APP_ID_PANDORA2:
        nState = eAPP_PANDORA_MODE_IPOD2;
        break;

    case HSDefP::APP_ID_BT_PANDORA:
        nState = eAPP_PANDORA_MODE_BT;
        break;

    case HSDefP::APP_ID_BT_AHA:
        nState = eAPP_AHARADIO_MODE_BT;
        break;

    case HSDefP::APP_ID_AHA1:
        nState = eAPP_AHARADIO_MODE_IPOD1;
        break;

    case HSDefP::APP_ID_AHA2:
        nState = eAPP_AHARADIO_MODE_IPOD2;
        break;

    case HSDefP::APP_ID_SIRIUS_XM_FROM_HOME:
        nState = eAPP_XM;
        break;

    case HSDefP::APP_ID_MODEM_PHONE_MAIN:
    case HSDefP::APP_ID_MODEM_DIAL:
    case HSDefP::APP_ID_MODEM_PHONEBOOK:
    case HSDefP::APP_ID_MODEM_CALL_HISTORY:
        LaunchBluelinkPhone( nAppId, nScreen, eAPP_HOME_BLUELINK_PHONE );
        return;

    default:
        break;
    }

    QPair < EVT_ID_T, QString > appInfo = GetAppInfo( nAppId );

    HS_LOG( "Launching " << appInfo.second << " application." << " aEventID = " << appInfo.first << " nState = " << nState );

    if ( EVT_NONE != appInfo.first )
    {
        APP_EVENT_DATA_BASE_T pEventData;
        pEventData.sIsJogEnabled = GetViewController(AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T( nScreen ))->GetFocusEnabled();

        QByteArray payload( (char *)&pEventData, sizeof(pEventData) );

        Event event( appInfo.first, GetThisAppId(), APP_UI_MANAGER, nState, payload);

        NotifyUISH( event, RemapScreenToDisplay( AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T( nScreen ) ) );
    }
}

void AppEngine::LaunchHMISettings( int nEntryPoint, int nScreen )
{
    APP_EVENT_DATA_BASE_T pEventData;
    pEventData.sIsJogEnabled = GetViewController(AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T( nScreen ))->GetFocusEnabled();

    QByteArray payload( (char *)&pEventData, sizeof(pEventData) );

    Event softEvent( EVT_KEY_SOFT_SETTINGS, GetThisAppId(), APP_UI_MANAGER, nEntryPoint, payload);

    NotifyUISH( softEvent, RemapScreenToDisplay( AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T( nScreen ) ) );
}

void AppEngine::LaunchBluelinkPhone( int nAppId, int nScreen, APP_ENTRY_POINT_T nState )
{
    QPair < EVT_ID_T, QString > appInfo = GetAppInfo( nAppId );

    HS_LOG( "Launching " << appInfo.second << " application." << " aEventID = " << appInfo.first << " Entry point = " << nState );

    if ( EVT_NONE != appInfo.first )
    {
        APP_EVENT_DATA_BASE_T pEventData;
        pEventData.sIsJogEnabled = GetViewController(AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T( nScreen ))->GetFocusEnabled();

        QByteArray payload( (char *)&pEventData, sizeof(pEventData) );

        Event event( appInfo.first, GetThisAppId(), APP_UI_MANAGER, nState, payload);

        switch( nAppId )
        {
        case HSDefP::APP_ID_MODEM_PHONE_MAIN:
            event.SetCommand( eAPP_IBOX_BLUELINK_FAVORITE );
            break;

        case HSDefP::APP_ID_MODEM_DIAL:
            event.SetCommand( eAPP_IBOX_BLUELINK_DIAL );
            break;

        case HSDefP::APP_ID_MODEM_PHONEBOOK:
            event.SetCommand( eAPP_IBOX_BLUELINK_PHONEBOOK );
            break;
        case HSDefP::APP_ID_MODEM_CALL_HISTORY:
            event.SetCommand( eAPP_IBOX_BLUELINK_CALL_HISTORY );
            break;

        default:
            HS_LOG("It's unspecified BLUELINK phone menu")
                    break;
        }

        NotifyUISH( event, RemapScreenToDisplay( AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T( nScreen ) ) );
    }
}

QPair < EVT_ID_T, QString >
AppEngine::GetAppInfo( const int nAppId )
{
    QPair < EVT_ID_T, QString > result( EVT_NONE, "" );
    unsigned int index = 0;

    while ( APP_LIST[ index ].event != EVT_MAX )
    {
        if ( APP_LIST[ index ].appId == nAppId )
        {
            result.first = APP_LIST[ index ].event;
            result.second = APP_LIST[ index ].appName;

            return result;
        }

        ++index;
    }

    return result;
}

void
AppEngine::BackKeyHandler( int nScreen )
{
    HS_LOG("nScreen : " << nScreen );
    //PROCESS_EVENT_CALL

    int currentScreenState = 0;
    ViewController* pViewController = NULL;

    if (nScreen == 1)
    {
        currentScreenState = m_nMenuState_F;
        pViewController = m_pViewControllerFront;
    }
    else if (nScreen == 2)
    {
        currentScreenState = m_nMenuState_R;
        pViewController = m_pViewControllerRear;
    }

    if (currentScreenState == HOME_MENU_MAIN)
    {
        return;
    }

    else if (currentScreenState == HOME_MENU_HELP)
    {
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_SETTINGS, "STR_HOME_SETTINGS", nScreen, false, HSDefP::APP_ID_INVALID, HSDefP::VIEW_ID_INFO);
    }

    else if (currentScreenState == HOME_MENU_SUB)
    {
        QString subMenuTitle = pViewController->GetTitleText();

        View* pView = pViewController->getCurrentView();

        if ( subMenuTitle.compare(QString("STR_HOME_SETTINGS")) == 0)
            pView->SetFocusIconIndexByViewID( HSDefP::VIEW_ID_SETTINGS );

        else if (subMenuTitle.compare(QString("STR_HOME_MEDIA")) == 0)
            pView->SetFocusIconIndexByViewID( HSDefP::VIEW_ID_MEDIA );

        else if (subMenuTitle.compare(QString("STR_BLUELINK")) == 0)
            pView->SetFocusIconIndexByViewID( HSDefP::VIEW_ID_BLUELINK );

        setStateToMainMenu (nScreen, true);
    }
}

void AppEngine::loadSubMenu (int nViewId, int nScreen, QString sViewName)
{
    ViewID = sViewName;

    ViewController *pViewController = NULL;
    pViewController = GetViewController( AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T( nScreen ) );
    HS_LOG("nViewId = " << nViewId << ",  nScreen = " << nScreen << ", sViewName = "<< sViewName );

    if (pViewController)
    {
        if (nViewId == HSDefP::VIEW_ID_MEDIA)
        {
            int nModeValue = 0;
            int focusIndex = 0;

            if (getUISHSharedMemoryInfo(SHM_OFFSET_CURRENT_AV_MODE, (unsigned char *)&nModeValue, SHM_CURRENT_AV_MODE_SIZE) )
            {
                HS_LOG( "nModeValue = " << nModeValue );

                switch((MODE_STATE_T)nModeValue)
                {
                case MODE_STATE_DISC_AUDIO:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_AUDIO_DISC);
                    break;

                case MODE_STATE_DISC_VIDEO:
                case MODE_STATE_VCDC:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_VIDEO_DISC);
                    break;

                case MODE_STATE_USB1_AUDIO:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_AUDIO_USB1);
                    break;

                case MODE_STATE_USB1_VIDEO:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_VIDEO_USB1);
                    break;

                case MODE_STATE_USB2_AUDIO:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_AUDIO_USB2);
                    break;

                case MODE_STATE_USB2_VIDEO:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_VIDEO_USB2);
                    break;

                case MODE_STATE_IPOD1_AUDIO:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_AUDIO_IPOD1);
                    break;

                case MODE_STATE_IPOD1_PANDORA:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_PANDORA1);
                    break;

                case MODE_STATE_AHA_RADIO_IPOD1:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_AHA1);
                    break;

                case MODE_STATE_IPOD2_AUDIO:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_AUDIO_IPOD2);
                    break;

                case MODE_STATE_IPOD2_PANDORA:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_PANDORA2);
                    break;

                case MODE_STATE_AHA_RADIO_IPOD2:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_AHA2);
                    break;

                case MODE_STATE_JUKEBOX_AUDIO:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_AUDIO_JBOX);
                    break;

                case MODE_STATE_JUKEBOX_VIDEO:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_VIDEO_JBOX);
                    break;

                case MODE_STATE_AUX1_AUDIO:
                case MODE_STATE_AUX1_VIDEO:
                case MODE_STATE_AUX2_AUDIO:
                case MODE_STATE_AUX2_VIDEO:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_AUX);
                    break;

                case MODE_STATE_BT_AUDIO:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_AUDIO_BLUETOOTH);
                    break;
                case MODE_STATE_BT_PANDORA:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_BT_PANDORA);
                    break;
                case MODE_STATE_AHA_RADIO_BT:
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true, HSDefP::APP_ID_BT_AHA);
                    break;

                default:
                    HS_LOG( "load default focus media sub menu");
                    setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true);
                    break;
                }
            }
            else
            {
                HS_LOG( "getUISHSharedMemoryInfo faild, load default focus media sub menu");
                setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true);
            }
        }

        else if (nViewId == HSDefP::VIEW_ID_SETTINGS)
        {
            setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true);
        }

        else if (nViewId == HSDefP::VIEW_ID_BLUELINK)
        {
            setStateToSubMenu(pViewController, HSDefP::VIEW_ID_T( nViewId ), sViewName, nScreen, true);
        }

        else if (nViewId == HSDefP::VIEW_ID_INFO)
        {
            emit goHelpMenu (nScreen);

            if (nScreen == 1)
            {
                m_nMenuState_F = HOME_MENU_HELP;
                m_pViewControllerFront->UpdateTitle("");
            }

            else if (nScreen == 2)
            {
                m_nMenuState_R = HOME_MENU_HELP;
                m_pViewControllerRear->UpdateTitle("");
            }
        }
    }
}

void AppEngine::ProcessEntryPoint( APP_ENTRY_POINT_T nEntryPoint, ViewController *pViewController )
{
    View* pView = NULL;
    pViewController->ShowRootView();
    pView = pViewController->getCurrentView();
    HS_LOG( "nEntryPoint =" << nEntryPoint <<  ", nDisplay = " <<  pViewController->GetDisplay() );

    int nScreen = 0;

    if (pViewController==m_pViewControllerFront) nScreen = 1;
    else  nScreen = 2;

    int currentScreenMenuState = 0;
    if (nScreen == 1) currentScreenMenuState = m_nMenuState_F;
    else if (nScreen == 2 ) currentScreenMenuState = m_nMenuState_R;


    switch( nEntryPoint )
    {

    /** Last Mode */
    case eAPP_MODE_LAST_STATE:
    {
        if (nScreen == DISPLAY_FRONT)
        {
            if (m_nMenuState_F == HOME_MENU_MAIN)
                setStateToMainMenu(nScreen, (pViewController->GetMeOnTop()&&(currentScreenMenuState == HOME_MENU_SUB)));


        }
        else if (nScreen == DISPLAY_REAR)
        {
            if (m_nMenuState_R == HOME_MENU_MAIN)
                setStateToMainMenu(nScreen, (pViewController->GetMeOnTop()&&(currentScreenMenuState == HOME_MENU_SUB)));
        }

        break;
    }

    /** HOME */
    case eAPP_HOME_MAIN:
    {
        if (currentScreenMenuState == HOME_MENU_SUB)
        {
            QString subMenuTitle = pViewController->GetTitleText();

            pView = pViewController->getCurrentView();

            if ( subMenuTitle.compare(QString("STR_HOME_SETTINGS")) == 0)
                pView->SetFocusIconIndexByViewID( HSDefP::VIEW_ID_SETTINGS );

            else if (subMenuTitle.compare(QString("STR_HOME_MEDIA")) == 0)
                pView->SetFocusIconIndexByViewID( HSDefP::VIEW_ID_MEDIA );

            else if (subMenuTitle.compare(QString("STR_BLUELINK")) == 0)
                pView->SetFocusIconIndexByViewID( HSDefP::VIEW_ID_BLUELINK );

            setStateToMainMenu (nScreen, (pViewController->GetMeOnTop()&&(currentScreenMenuState == HOME_MENU_SUB)));
        }
        else
        {
            setStateToMainMenu(nScreen, (pViewController->GetMeOnTop()&&(currentScreenMenuState == HOME_MENU_SUB)));
        }

        break;
    }

    /** HOME -> Media */
    case eAPP_HOME_MEDIA:
    {
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA", nScreen, false);
        break;
    }

    /** HOME -> Bluelink */
    //! Audo Diagnostics, Entertainment, Navigation is removed and moved to Bluelink
    case eAPP_HOME_BLUELINK:
    {

        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_BLUELINK, "STR_BLUELINK", nScreen,
                          (pViewController->GetMeOnTop() && (currentScreenMenuState == HOME_MENU_MAIN)), HSDefP::APP_ID_MODEM_DIAL);
        break;
    }

        /** HOME -> Settings */
    case eAPP_HOME_SETTINGS:
    {
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_SETTINGS, "STR_HOME_SETTINGS", nScreen,
                          (pViewController->GetMeOnTop() && (currentScreenMenuState == HOME_MENU_MAIN)));

        //setStateToSubMenu(pViewController, HSDefP::VIEW_ID_SETTINGS, "STR_HOME_SETTINGS", nScreen, pViewController->GetMeOnTop());
        break;
    }

    // Root Menu icons
    case eAPP_HOME_ROOT_NAVI:
        setStateToMainMenu(nScreen, (pViewController->GetMeOnTop()&&(currentScreenMenuState == HOME_MENU_SUB)));
        pView->SetFocusIconIndexByAppID( HSDefP::APP_ID_NAVI );
        break;

    case eAPP_HOME_ROOT_RADIO:
        setStateToMainMenu(nScreen, (pViewController->GetMeOnTop()&&(currentScreenMenuState == HOME_MENU_SUB)));
        pView->SetFocusIconIndexByAppID( HSDefP::APP_ID_RADIO );
        break;

    case eAPP_HOME_ROOT_XM:
        setStateToMainMenu(nScreen, (pViewController->GetMeOnTop()&&(currentScreenMenuState == HOME_MENU_SUB)));
        pView->SetFocusIconIndexByAppID( HSDefP::APP_ID_SIRIUS_XM_FROM_HOME );
        break;

    case eAPP_HOME_ROOT_MEDIA:
        setStateToMainMenu(nScreen, (pViewController->GetMeOnTop()&&(currentScreenMenuState == HOME_MENU_SUB)));
        pView->SetFocusIconIndexByViewID( HSDefP::VIEW_ID_MEDIA );
        break;

    case eAPP_HOME_ROOT_DMB:
        setStateToMainMenu(nScreen, (pViewController->GetMeOnTop()&&(currentScreenMenuState == HOME_MENU_SUB)));
        pView->SetFocusIconIndexByAppID( HSDefP::APP_ID_DMB );
        break;

    case eAPP_HOME_ROOT_SETTINGS:
        setStateToMainMenu(nScreen, (pViewController->GetMeOnTop()&&(currentScreenMenuState == HOME_MENU_SUB)));
        pView->SetFocusIconIndexByViewID( HSDefP::VIEW_ID_SETTINGS );
        break;

    case eAPP_HOME_ROOT_PHONE:
        setStateToMainMenu(nScreen, (pViewController->GetMeOnTop()&&(currentScreenMenuState == HOME_MENU_SUB)));
        pView->SetFocusIconIndexByAppID( HSDefP::APP_ID_BT_PHONE );
        break;

    case eAPP_HOME_ROOT_XM_DATA:
        setStateToMainMenu(nScreen, (pViewController->GetMeOnTop()&&(currentScreenMenuState == HOME_MENU_SUB)));
        pView->SetFocusIconIndexByAppID( HSDefP::APP_ID_XM_DATA );
        break;

    case eAPP_HOME_ROOT_BLUELINK:
        setStateToMainMenu(nScreen, (pViewController->GetMeOnTop()&&(currentScreenMenuState == HOME_MENU_SUB)));
        pView->SetFocusIconIndexByViewID( HSDefP::VIEW_ID_BLUELINK );
        break;




    // Media Menu icons
    case eAPP_HOME_MEDIA_JB_AUDIO:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA", nScreen, false, HSDefP::APP_ID_AUDIO_JBOX);
        break;

    case eAPP_HOME_MEDIA_JB_VIDEO:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_VIDEO_JBOX);
        break;

    case eAPP_HOME_MEDIA_JB_PHOTO:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_PHOTO_JBOX);
        break;

    case eAPP_HOME_MEDIA_USB_AUDIO1:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_AUDIO_USB1);
        break;

    case eAPP_HOME_MEDIA_USB_VIDEO1:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_VIDEO_USB1);
        break;

    case eAPP_HOME_MEDIA_USB_PHOTO1:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_PHOTO_USB1);
        break;

    case eAPP_HOME_MEDIA_USB_AUDIO2:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_AUDIO_USB2);
        break;

    case eAPP_HOME_MEDIA_USB_VIDEO2:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_VIDEO_USB2);
        break;

    case eAPP_HOME_MEDIA_USB_PHOTO2:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_PHOTO_USB2);
        break;

    case eAPP_HOME_MEDIA_IPOD_AUDIO1:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_AUDIO_IPOD1);
        break;

    case eAPP_HOME_MEDIA_IPOD_AUDIO2:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_AUDIO_IPOD2);
        break;

    case eAPP_HOME_MEDIA_DISC:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_AUDIO_DISC);
        break;

    case eAPP_HOME_MEDIA_BT_AUDIO:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_AUDIO_BLUETOOTH);
        break;

    case eAPP_HOME_MEDIA_AUX:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_AUX);
        break;

    case eAPP_HOME_MEDIA_PANDORA1:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_PANDORA1);
        break;

    case eAPP_HOME_MEDIA_PANDORA2:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_PANDORA2);
        break;

    case eAPP_HOME_MEDIA_PANDORA_BT:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_BT_PANDORA);
        break;

    case eAPP_HOME_MEDIA_AHA_BT:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_BT_AHA);
        break;

    case eAPP_HOME_MEDIA_AHA1:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_AHA1);
        break;

    case eAPP_HOME_MEDIA_AHA2:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_MEDIA, "STR_HOME_MEDIA",nScreen, false, HSDefP::APP_ID_AHA2);
        break;


    // Setting Menu icons
    case eAPP_HOME_SETTING_SOUND:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_SETTINGS, "STR_HOME_SETTINGS", nScreen, false, HSDefP::APP_ID_SETTINGS_SOUND);
        break;

    case eAPP_HOME_SETTING_DISPLAY:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_SETTINGS, "STR_HOME_SETTINGS", nScreen, false, HSDefP::APP_ID_SETTINGS_SCREEN);
        break;

    case eAPP_HOME_SETTING_NAVIGATION:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_SETTINGS, "STR_HOME_SETTINGS", nScreen, false, HSDefP::APP_ID_SETTINGS_NAVI);
        break;

    case eAPP_HOME_SETTING_BT:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_SETTINGS, "STR_HOME_SETTINGS", nScreen, false, HSDefP::APP_ID_SETTINGS_BLUETOOTH);
        break;

    case eAPP_HOME_SETTING_VR:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_SETTINGS, "STR_HOME_SETTINGS", nScreen, false, HSDefP::APP_ID_SETTINGS_VR);
        break;

    case eAPP_HOME_SETTING_CLOCK:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_SETTINGS, "STR_HOME_SETTINGS", nScreen, false, HSDefP::APP_ID_SETTINGS_CLOCK);
        break;

    case eAPP_HOME_SETTING_GENERAL:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_SETTINGS, "STR_HOME_SETTINGS", nScreen, false, HSDefP::APP_ID_SETTINGS_GENERAL_BASIC);
        break;

    case eAPP_HOME_SETTING_SYSTEM:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_SETTINGS, "STR_HOME_SETTINGS", nScreen, false, HSDefP::APP_ID_SETTINGS_GENERAL_SYSTEM);
        break;

    case eAPP_HOME_SETTING_HELP:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_SETTINGS, "STR_HOME_SETTINGS", nScreen, false);
        break;

    case eAPP_HOME_SETTING_WIFI:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_SETTINGS, "STR_HOME_SETTINGS", nScreen, false, HSDefP::APP_ID_WIFI_SETTINGS);
        break;


    // Blue Link Menu icons
    case eAPP_HOME_BL_PHONE:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_BLUELINK, "STR_BLUELINK", nScreen,
                          (pViewController->GetMeOnTop() && (currentScreenMenuState == HOME_MENU_MAIN)), HSDefP::APP_ID_MODEM_DIAL);
        //setStateToSubMenu(pViewController, HSDefP::VIEW_ID_BLUELINK, "STR_BLUELINK", nScreen, false, HSDefP::APP_ID_MODEM_DIAL);
        break;

    case eAPP_HOME_BL_DRIVING:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_BLUELINK, "STR_BLUELINK", nScreen, false, HSDefP::APP_ID_DRIVING_INFO);
        break;

    case eAPP_HOME_BL_POI:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_BLUELINK, "STR_BLUELINK", nScreen, false, HSDefP::APP_ID_SEND_TO_CAR);
        break;

    case eAPP_HOME_BL_SETTINGS:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_BLUELINK, "STR_BLUELINK", nScreen, false, HSDefP::APP_ID_BLUELINK_SETTINGS);
        break;

    case eAPP_HOME_BL_DIAG:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_BLUELINK, "STR_BLUELINK", nScreen, false, HSDefP::APP_ID_VEHICLE_DIAGNOSTICS);
        break;

    case eAPP_HOME_BL_CENTER:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_BLUELINK, "STR_BLUELINK", nScreen, false, HSDefP::APP_ID_BLUELINK_CENTER);
        break;

    case 326:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_BLUELINK, "STR_BLUELINK", nScreen, false, HSDefP::APP_ID_APP_STORE);
        break;


    case eAPP_HOME_SETTING_HELP_GUIDE:
        emit goHelpMenu (nScreen);

        if (nScreen == 1) m_nMenuState_F = HOME_MENU_HELP;
        else if (nScreen == 2) m_nMenuState_R = HOME_MENU_HELP;
        break;

    case eAPP_HOME_SETTING_HELP_MANUAL:
        setStateToSubMenu(pViewController, HSDefP::VIEW_ID_SETTINGS, "STR_HOME_SETTINGS", nScreen, false, HSDefP::APP_ID_E_MANUAL);
        /*
        emit goHelpMenu (nScreen);

        if (nScreen == 1) m_nMenuState_F = HOME_MENU_HELP;
        else if (nScreen == 2) m_nMenuState_R = HOME_MENU_HELP;
        */
        break;

    }
}

ViewController* AppEngine::GetViewController( AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T nScreen )
{
    LW_LOG( "nScreen = " << nScreen );

    ViewController *pViewController = m_pViewControllerRear;

    if( AppEngineQMLConstants::SCREEN_FRONT == nScreen )
    {
        pViewController = m_pViewControllerFront;
    }

    return pViewController;
}

void AppEngine::UpdateViewControllerMapping()
{
#if 0 // swap view controller only

    if (m_bDCSwapped != m_bViewContentSwapped)
    {
        m_bViewContentSwapped = m_bDCSwapped;

        HS_LOG( "Do swap contents of ViewControllers for DCSwap status = "<<  m_bDCSwapped );
        ViewControllerHelper::SwapViewControllerContentForDCSwap(m_pViewControllerFront, m_pViewControllerRear);
    }

    QDeclarativeContext *ctxt = GetScreenContext();
    ctxt->setContextProperty( "ViewControll", m_pViewControllerFront );
    m_pViewControllerFront->UpdateViewContext();

    QDeclarativeContext *ctxt2 = GetScreenContext(AppEngineQMLConstants::APP_REAR_SCREEN);
    ctxt2->setContextProperty( "ViewControll", m_pViewControllerRear );
    m_pViewControllerRear->UpdateViewContext();

#else // swap ScreenViews of AppFramework and swap ViewControllers

    if (m_bDCSwapped != m_bViewContentSwapped)
    {
        m_bViewContentSwapped = m_bDCSwapped;

        int menuStateTemp = m_nMenuState_F;
        m_nMenuState_F = m_nMenuState_R;
        m_nMenuState_R = menuStateTemp;

        // exchange ViewController
        HS_LOG( "Exchange ViewController pointer  for DCSwap status = "<<  m_bDCSwapped );
        ViewController* pVC = m_pViewControllerFront;
        m_pViewControllerFront = m_pViewControllerRear;
        m_pViewControllerRear = pVC;

        // set new Display title for exchanged ViewController
        //int nDisplay = m_pViewControllerFront->GetDisplay();
        //m_pViewControllerFront->SetDisplay(m_pViewControllerRear->GetDisplay());
        //m_pViewControllerRear->SetDisplay(nDisplay);

        // keep logical 'Display ID' even after ViewControllers are exchanged

        // do change ScreenView, actual binding was done at initial time
        HS_LOG( "Do swap AppScreenView for new DCSwap status = "<<  m_bDCSwapped );
        SwapScreenContents();
    }

#endif
}

DISPLAY_T AppEngine::RemapScreenToDisplay( AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T nScreen )
{
    DISPLAY_T nDisplay = DISPLAY_AUTO;

    if( AppEngineQMLConstants::SCREEN_FRONT == nScreen )
    {
        nDisplay = DISPLAY_FRONT;
    }
    else if ( AppEngineQMLConstants::SCREEN_REAR == nScreen)
    {
        nDisplay = DISPLAY_REAR;
    }
    HS_LOG( "nScreen = " << nScreen << " nDisplay = " << nDisplay );

    return nDisplay;
}

/*
bool AppEngine::getIBoxModemFlag()
{
    return ( GetSystemVariant() & 0x2 );
}
*/

// for SW WDT -->

void AppEngine::signalConnection()
{
    struct sigaction act;
    memset(&act, 0x00, sizeof(struct sigaction));

    act.sa_flags = 0;
    act.sa_flags |= SA_RESETHAND;
    act.sa_handler = AppEngine::signalHandler;

    for ( int signumber = SIGHUP; signumber < SIGUNUSED; signumber++ )
    {
        if ( signumber == SIGKILL || signumber == SIGSTOP ) continue;

        if ( sigaction(signumber, &act, (struct sigaction*)NULL) < 0 )
        {
            exit(1);
        }
    }
}

/** 여기서 17번 signal이 들어오면 restart X */
void AppEngine::signalHandler (int signum)
{
    HS_LOG( "signum = " << signum );
    if(signum == SIGCHLD)
    {
        HS_LOG( "[SIGCHILD] Not Seding Restart Message to UISH ------->" );
    }
    else
    {
        SigM->restart(signum);
        raise(signum);
    }
}


void AppEngine::setSignalHandlerObject (AppEngine* m)
{
    SigM = m;
}

void AppEngine::restart(int signum)
{
    Event event ( EVT_REQUEST_ABNORMAL_TERMINATION, GetThisAppId(), APP_UI_MANAGER, (EVT_COMMAND_T)signum );
    NotifyUISH (event, DISPLAY_FRONT);
    //exit(0);
}
// <--

/*
void AppEngine::repaintView()
{
    QDeclarativeView* pDeclarativeView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
    pDeclarativeView->repaint();

    pDeclarativeView = GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
    pDeclarativeView->repaint();
}
*/

void AppEngine::resetAppLaunchData()
{
    appLaunching (false, 1);
    appLaunching (false, 2);
}

void AppEngine::updateMediaMenuIcons(EVT_MEDIA_ENABLE_DATA_T *pData)
{
    HS_LOG( "--->");

    if (!pData)
    {
        CR_LOG("pData is NULL --> return" ) ;
        return;
    }

    QString frontViewName(m_pViewControllerFront->GetTitleText());
    QString rearViewName(m_pViewControllerRear->GetTitleText());
    QString mediaViewName("STR_HOME_MEDIA");

    int focusedAppID_F = -1;
    int focusedAppID_R = -1;

    if (mediaViewName == frontViewName)
    {
        focusedAppID_F = m_pViewControllerFront->getCurrentView()->SubMenuModel()->GetFocusedAppId();
        LW_LOG( "focusedAppID_F = " << focusedAppID_F);
    }

    if (mediaViewName == rearViewName)
    {
        focusedAppID_R = m_pViewControllerRear->getCurrentView()->SubMenuModel()->GetFocusedAppId();
        LW_LOG( "focusedAppID_R = " << focusedAppID_R);
    }

/**
    struct MEDIA_CONTENT_MAP_T
    {
       HomeScreenDef::MEDIA_SOURCE_T nSource;
       HSDefP::APP_DATA_T nDevice;
       HSDefP::APP_DATA_T nVideo;
       HSDefP::APP_DATA_T nPhoto;
       HSDefP::APP_DATA_T nAudio;
       QString sDeviceName;
    };
    */
    // Jukebox
    m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_JBOX,
                                                      true,
                                                      pData->tMediaEnable.bJukeboxVideo,
                                                      pData->tMediaEnable.bJukeboxPhoto,
                                                      pData->tMediaEnable.bJukeboxAudio );


    // USB 1
    if (pData->tMediaEnable.bUSB1Connected)
    {
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_USB1,
                                                          pData->tMediaEnable.bUSB1Connected,
                                                          pData->tMediaEnable.bUSB1Video,
                                                          pData->tMediaEnable.bUSB1Photo,
                                                          pData->tMediaEnable.bUSB1Audio );
    }
    else
    {
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_USB1 );
    }

    // USB 2
    if (pData->tMediaEnable.bUSB2Connected)
    {
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_USB2,
                                                         pData->tMediaEnable.bUSB2Connected,
                                                         pData->tMediaEnable.bUSB2Video,
                                                         pData->tMediaEnable.bUSB2Photo,
                                                         pData->tMediaEnable.bUSB2Audio );
    }
    else
    {
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_USB2 );
    }

    // iPod 1
    if (pData->tMediaEnable.bIPod1Connected)
    {
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_IPOD1,
                                                         pData->tMediaEnable.bIPod1Connected,
                                                         false,
                                                         false,
                                                         pData->tMediaEnable.bIPod1Audio );

        if ( ! m_pInitData->GetAppData()->GetValue(HSDefP::APP_DATA_AVAILABLE_PANDORA1).toBool() )
            HS_LOG( "PANDORA1 = " << pData->tMediaEnable.bPandoraIPod1 ) ;
        if ( ! m_pInitData->GetAppData()->GetValue(HSDefP::APP_DATA_AVAILABLE_AHA1).toBool() )
            HS_LOG( "AHA1 = " << pData->tMediaEnable.bAhaRadioIPod1 ) ;

        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_AVAILABLE_PANDORA1, QVariant( pData->tMediaEnable.bPandoraIPod1 ) );
        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_AVAILABLE_AHA1, QVariant( pData->tMediaEnable.bAhaRadioIPod1 ) );
    }
    else
    {
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_IPOD1 );

        if ( m_pInitData->GetAppData()->GetValue(HSDefP::APP_DATA_AVAILABLE_PANDORA1).toBool() )
            HS_LOG( "PANDORA1 = " << pData->tMediaEnable.bPandoraIPod1 ) ;
        if ( m_pInitData->GetAppData()->GetValue(HSDefP::APP_DATA_AVAILABLE_AHA1).toBool() )
            HS_LOG( "AHA1 = " << pData->tMediaEnable.bAhaRadioIPod1 ) ;

        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_AVAILABLE_PANDORA1, QVariant(false));
        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_AVAILABLE_AHA1, QVariant(false));
    }

        // iPod 2
    if (pData->tMediaEnable.bIPod2Connected)
    {
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_IPOD2,
                                                         pData->tMediaEnable.bIPod2Connected,
                                                         false,
                                                         false,
                                                         pData->tMediaEnable.bIPod2Audio );

        if ( !m_pInitData->GetAppData()->GetValue( HSDefP::APP_DATA_AVAILABLE_PANDORA2 ).toBool() )
            HS_LOG( "PANDORA2 = " << pData->tMediaEnable.bPandoraIPod2 );
        if ( !m_pInitData->GetAppData()->GetValue( HSDefP::APP_DATA_AVAILABLE_AHA2 ).toBool() )
            HS_LOG( "AHA2 = " << pData->tMediaEnable.bAhaRadioIPod2 );

        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_AVAILABLE_PANDORA2, QVariant( pData->tMediaEnable.bPandoraIPod2 ) );
        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_AVAILABLE_AHA2, QVariant( pData->tMediaEnable.bAhaRadioIPod2 ) );
    }
    else
    {
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_IPOD2 );

        if ( m_pInitData->GetAppData()->GetValue( HSDefP::APP_DATA_AVAILABLE_PANDORA2 ).toBool() )
            HS_LOG( "PANDORA2 = " << pData->tMediaEnable.bPandoraIPod2 );
        if ( m_pInitData->GetAppData()->GetValue( HSDefP::APP_DATA_AVAILABLE_AHA2 ).toBool() )
            HS_LOG( "AHA2 = " << pData->tMediaEnable.bAhaRadioIPod2 );

        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_AVAILABLE_PANDORA2, QVariant(false) );
        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_AVAILABLE_AHA2, QVariant(false) );
    }


    // DISC
    if (pData->tMediaEnable.bDISCInserted)
    {
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_CD_AUDIO );
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_CD_DATA );
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_VCD );
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_DVD_AUDIO );
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_DVD_VIDEO );

        switch (pData->tMediaEnable.ucDiscType)
        {
        case eDISC_TYPE_AUDIO_DVD:
            m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_DVD_AUDIO,
                                                             pData->tMediaEnable.bDISCInserted,
                                                             false,
                                                             false,
                                                             true );
            break;
        case eDISC_TYPE_VIDEO_DVD:
            m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_DVD_VIDEO,
                                                             pData->tMediaEnable.bDISCInserted,
                                                             true,
                                                             false,
                                                             false );
            break;
        case eDISC_TYPE_AUDIO_CD:
            m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_CD_AUDIO,
                                                             pData->tMediaEnable.bDISCInserted,
                                                             false,
                                                             false,
                                                             true );
            break;
        case eDISC_TYPE_VIDEO_CD:
            m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_VCD,
                                                             pData->tMediaEnable.bDISCInserted,
                                                             true,
                                                             false,
                                                             false );
            break;
        case eDISC_TYPE_MP3_CD:
            m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_CD_DATA,
                                                             pData->tMediaEnable.bDISCInserted,
                                                             false,
                                                             false,
                                                             true );
            break;
        }
    }
    else
    {
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_CD_AUDIO );
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_CD_DATA );
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_VCD );
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_DVD_AUDIO );
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_DVD_VIDEO );
    }

    // AUX
    if (pData->tMediaEnable.bAUX1Connected)
    {
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_AUX,
                                                         pData->tMediaEnable.bAUX1Connected,
                                                         pData->tMediaEnable.bAux1Video,
                                                         false,
                                                         pData->tMediaEnable.bAux1Audio );
    }
    else
    {
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_AUX );
    }

    // BT
    if (pData->tMediaEnable.bBTConnected)
    {
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_BLUETOOTH,
                                                         pData->tMediaEnable.bBTConnected,
                                                         false,
                                                         false,
                                                         pData->tMediaEnable.bBTMusic );
#if 1
        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_AVAILABLE_BT_PANDORA, QVariant( pData->tMediaEnable.bPandoraBT ) );  // use for bt pandora
        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_AVAILABLE_BT_AHA, QVariant( pData->tMediaEnable.bAhaRadioBT ) );     // use for bt aha
#endif
    }
    else
    {
        m_pInitData->GetAppData()->SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_BLUETOOTH );
#if 1
        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_AVAILABLE_BT_PANDORA, QVariant( false ) );  // use for bt pandora
        m_pInitData->GetAppData()->SetValue( HSDefP::APP_DATA_AVAILABLE_BT_AHA, QVariant( false ) );      // use for bt aha
#endif
    }

    // if current menu is media menu and media menu icon count is zero,
    // go main menu and show popup "no media"



    if (mediaViewName == frontViewName)
    {
        if (m_pViewControllerFront->getCurrentView()->SubMenuModel()->rowCount() == 0)
        {
            CR_LOG("pData is NULL --> return") ;

            ProcessEntryPoint(eAPP_HOME_ROOT_MEDIA, m_pViewControllerFront);
            m_pViewControllerFront->SetPopUpType(HSDefP::POPUP_NO_MEDIA);
        }

        else if (focusedAppID_F != -1)
        {
            int focusIndex = m_pViewControllerFront->getCurrentView()->GetSubMenuFocusIconIndexByAppID((HSDefP::APP_ID_T)focusedAppID_F);

            emit setMediaMenuFocusIndex(1, focusIndex);

            HS_LOG("FRONT setMediaMenuFocusIndex = " << focusIndex);
        }
    }

    if (mediaViewName == rearViewName)
    {
        if (m_pViewControllerRear->getCurrentView()->SubMenuModel()->rowCount() == 0)
        {
            ProcessEntryPoint(eAPP_HOME_ROOT_MEDIA, m_pViewControllerRear);
            m_pViewControllerRear->SetPopUpType(HSDefP::POPUP_NO_MEDIA);
        }

        else if (focusedAppID_R != -1)
        {
            int focusIndex = m_pViewControllerRear->getCurrentView()->GetSubMenuFocusIconIndexByAppID((HSDefP::APP_ID_T)focusedAppID_R);

            emit setMediaMenuFocusIndex(2, focusIndex);

            HS_LOG("REAR setMediaMenuFocusIndex = " << focusIndex);
        }
    }

    if (m_pInitData->GetAppData()->GetValue(HSDefP::APP_DATA_ENABLED_MEDIA).toBool())
    {

        if (m_pViewControllerFront->GetPopUpType() == HSDefP::POPUP_NO_MEDIA)
        {
            HS_LOG("[FRONT] no medeia -> media connected, clear no media popup");
            m_pViewControllerFront->SetPopUpType(HSDefP::POPUP_INVALID);
            emit closepopup(1);
        }

        if (m_pViewControllerRear->GetPopUpType() == HSDefP::POPUP_NO_MEDIA)
        {
            HS_LOG("[REAR] no medeia -> media connected, clear no media popup");
            m_pViewControllerRear->SetPopUpType(HSDefP::POPUP_INVALID);
            emit closepopup(2);
        }
    }

    if (pData->tMediaEnable.bBTMusic)
    {

        if (m_pViewControllerFront->GetPopUpType() == HSDefP::POPUP_DISABLE_BT_MUSIC)
        {
            HS_LOG("[FRONT] BT audio disable -> enable, if BT audio disable popup is shown, clear popup");
            m_pViewControllerFront->SetPopUpType(HSDefP::POPUP_INVALID);
            emit closepopup(1);
        }

        if (m_pViewControllerRear->GetPopUpType() == HSDefP::POPUP_DISABLE_BT_MUSIC)
        {
            HS_LOG("[REAR] BT audio disable -> enable, if BT audio disable popup is shown, clear popup");
            m_pViewControllerRear->SetPopUpType(HSDefP::POPUP_INVALID);
            emit closepopup(2);
        }
    }

    LW_LOG("<---");
}

QString AppEngine::getFont(bool hdb)
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

void AppEngine::logForQML(QString log)
{
     Logger::Instance()->Log( log, Logger::PRIORITY_HI );
}

void AppEngine::appLaunching(bool bLaunching, int screen)
{
    LW_LOG( "bLaunching = " << bLaunching << ", screen = " << screen );

    if (screen == 1)
    {
        m_bAppLaunching_F = bLaunching;
    }
    else if  (screen == 2)
    {
        m_bAppLaunching_R = bLaunching;
    }

    emit sigAppLaunching (bLaunching, screen);
}

bool AppEngine::getUISHSharedMemoryInfo ( int offset,  unsigned char * pBuf,  int size)
{
    if ( !m_uishSharedMemory.isAttached() )
    {
        CR_LOG(":: Lost Connection with the shared memroy -> error code : " << m_uishSharedMemory.error() );
        return false;
    }

    m_uishSharedMemory.lock();
    memcpy( pBuf, ((unsigned char *)m_uishSharedMemory.data())+offset, size );
    m_uishSharedMemory.unlock();

    return true;
}

void AppEngine::setStateToMainMenu(int screen, bool animation)
{
    HS_LOG( "screen = " << screen << ", animation = " << animation );

    emit goMainMenu(screen, animation);

    if (screen == 1)
    {
        m_nMenuState_F = HOME_MENU_MAIN;
        m_pViewControllerFront->UpdateTitle("");

        SetFrontFullScreen(true);
    }

    else if (screen  == 2)
    {
        m_nMenuState_R = HOME_MENU_MAIN;
        m_pViewControllerRear->UpdateTitle("");

        SetRearFullScreen(true);
    }
}

void AppEngine::setStateToSubMenu (ViewController *pViewController,
                                   HSDefP::VIEW_ID_T subMenuId,
                                   QString subMenuTitle,
                                   int screen,
                                   bool animation,
                                   HSDefP::APP_ID_T focusAppID,
                                   HSDefP::VIEW_ID_T focusViewID )
{
    HS_LOG( "subMenuId = " << subMenuId << ", animation = " << animation << ", focusAppID = " << focusAppID << ", screen = " << screen );

    if (pViewController == NULL)
    {
        CR_LOG("pViewController is NULL");
        return;
    }

    int focusIndex = -1;

    pViewController->UpdateSubMenuContext (subMenuId);
    pViewController->UpdateTitle(subMenuTitle);

    if (focusViewID != HSDefP::VIEW_ID_INVALID)
    {
        View* pView = pViewController->getCurrentView();
        focusIndex = pView->GetSubMenuFocusIconIndexByViewID(focusViewID);
    }

    if (focusAppID != HSDefP::APP_ID_INVALID)
    {
        if (focusAppID == HSDefP::APP_ID_AUDIO_DISC)
        {
            View* pView = pViewController->getCurrentView();
            focusIndex = pView->GetSubMenuFocusIconIndexByAppID(focusAppID);

            if (focusIndex == -1)
            {
                View* pView = pViewController->getCurrentView();
                focusIndex = pView->GetSubMenuFocusIconIndexByAppID(HSDefP::APP_ID_VIDEO_DISC);
            }
        }

        else if (focusAppID == HSDefP::APP_ID_VIDEO_DISC)
        {
            View* pView = pViewController->getCurrentView();
            focusIndex = pView->GetSubMenuFocusIconIndexByAppID(focusAppID);

            if (focusIndex == -1)
            {
                View* pView = pViewController->getCurrentView();
                focusIndex = pView->GetSubMenuFocusIconIndexByAppID(HSDefP::APP_ID_AUDIO_DISC);
            }
        }

        else
        {
            View* pView = pViewController->getCurrentView();
            focusIndex = pView->GetSubMenuFocusIconIndexByAppID(focusAppID);
        }
    }

    emit goSubMenu(screen, animation, (int)subMenuId, focusIndex);

    if (screen == 1)
    {
        m_nMenuState_F = HOME_MENU_SUB;
        SetFrontFullScreen(false);
    }

    else if (screen  == 2)
    {
        m_nMenuState_R = HOME_MENU_SUB;

        SetRearFullScreen(false);
    }
}

void AppEngine::setBeepCommand()
{
    LW_LOG("---->")

    m_BeepMsg = QDBusMessage::createMethodCall("com.lge.car.micom", "/", "com.lge.car.micom.audioif", "ProcessAudioControlMessage");

    QList<QVariant> args;
    args.append( (quint16) 0x0115);
    args.append( (quint8) 0x0);

    m_BeepMsg.setArguments(args);
}

void AppEngine::playBeep()
{
    LW_LOG("---->")

    QDBusConnection::sessionBus().call(m_BeepMsg, QDBus::NoBlock);
}
