#include <DHAVN_AppPandoraBase.h>
#ifdef FRAMEWORK_ENABLED
#include <DHAVN_AppFramework_Event_Def.h>
#include <uistate_manager_def.h>
#include <DHAVN_AppSettings_Shared.h>
#include <DHAVN_AppHomeScreen_Def.h>
#endif
#include <QApplication>
#include <QGraphicsObject>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>
#include <qdeclarativeprivate.h>
#include <DHAVN_PndrStateHandler.h>
#include <QtDeclarative/QDeclarativeContext>
#include "DHAVN_AppPandora_Notifier.h"
#include <QDebug>
#include <QmlStatusBar/osdwriter.h> // added by esjang 2013.05.13 for Update Status bar

const quint16 c_AlbumArtDimension = 224;
const quint16 c_StationArtDimension = 74;
//const QString DEV_TYPE = "DeviceType" ;
//const QString DEV_NAME = "DeviceName" ;
/**
 *******************************************************************
 *
 * Constructor for CAppPandoraBase module. This will construct
 * other c++ modules in Pandora app qt layer.
 *
 * @param[in]   aQmlView    The view using which all screens will
 *                          be displayed
 * @param[in]   nLanguage   The default language to be used by
 *                          application
 * @param[in]   pParent     The parent screen for the application.
 *
 * @return     None
 *
 *******************************************************************
 */
#ifdef FRAMEWORK_ENABLED
CAppPandoraBase::CAppPandoraBase( QDeclarativeView &aQMLView, ECountryVariant variant, LANGUAGE_T nLanguage, int nVehicleType, QObject *pParent ) // add vehicle type for DH, 16MY, DH PE wonseok.heo 2014.12.29
: AppEngineQML( &aQMLView, "Pandora", pParent )
, m_pTranslator( NULL )
#ifdef FRAMEWORK_ENABLED
, m_nCountryVariant( variant )
, m_nLanguage(nLanguage)
#endif
, m_pSignals(NULL)
, m_pBTSignals(NULL) //added by cheolhwan 2014-02-27. Added "m_pBTSignals" to receive BT Signal.
, m_eHMIState(CAppPandoraBase::AppNotLaunched)
, m_bPandoraConnected(false)
, bAV_Mode_Changed(false)
, m_bIsBackGround(true)
, m_bOptionMenuVisible(false)
, m_bLoadingPopupVisible(false)
, bResumePlay(false)  // added by esjang 2013.05.05 for BT Call, Pandora will be paused
, nCallingState(false)  // added by esjang 2013.03.16 for BT Call flag, Disable Emmiting playing time
, m_bClusterUpdate(false)// added by esjang 2013.04.22 for cluster update, Disable sending cluster data
, isNetworkError(false)// added by esjang 2013.05.16 for certification issue, networkerror
, m_OSDFlag(false)//{ modified by yongkyun.lee 2014-04-06 for : ITS 233641
, m_bSystemPopupVisible(false)//added by esjang 2013.08.05 for ITS 182095
, m_bLocalPopupVisible(false)
#else
CAppPandoraBase::CAppPandoraBase(QDeclarativeView &aQMLView, LANGUAGE_T nLanguage, QObject *pParent):
    m_StationList(m_PndrInterface)
, m_Search(m_PndrInterface)
, m_Track(m_PndrInterface)
#endif
, mPandora(NULL)
, mPandoraF(NULL)
, mPandoraR(NULL)
, mBTPandora(NULL)
, m_pipeThread(NULL)
, m_tuneSelectTimer( new QTimer(this))
, m_bScrollingTicker(true)
, m_isFrontLCD(true)
, m_noSkipTimer( new QTimer(this)) // modified by esjang 2013.08.26 for ITS #186175
, m_PlayMuteTimer( new QTimer(this)) //{ modified by yongkyun.lee 2014-03-11 for : BT streaming Mute-play control
, m_isMuteState(false)
, m_isTemporalMute(TEMPORAL_UNDEFINED)//{ modified by yongkyun.lee 2014-06-18 for : ITS 240298
, m_eViewState(ENoView)
, m_isSeekNextLongKeyPressed(false) 	// added by jyjeon 2013.12.12 for ITS 214424
, isInsufficientNetwork(false) // added by jyjeon 2014.02.28 for ITS 227479
, m_NextTimeOSD(false)             // modified by yongkyun.lee 2014-11-03 for : //leeyk1 OSD TEST
, m_pShareDataProvider( new SharedDataProvider( PANDORA_SHARED_MEMORY_KEY ) )//added by jyjeon 2014-03-06 for VR
, isSeek(false) //add by wonseok.heo for OSD at BG 2015.02.04
{
    LOG_INIT_V2("Pandora");

    m_eCurrentDevice =  DeviceInfo::ENoDevice ;

    #ifndef FRAMEWORK_ENABLED
    Q_UNUSED(nLanguage)
    Q_UNUSED(pParent)
#endif


//{ added by esjang 2013.03.18 for Preventing Abnormal termination(SW WDT)
    signalConnection();
    setSignalHandlerObject(this);
    //} added by esjang 2013.03.18 for Preventing Abnormal termination(SW WDT)

    /** Reference to Track class instance */
    m_Track = new CAppPandoraTrack(this);
    m_Track->SetDeclarativeView(aQMLView);

    m_StationList = new CAppPandoraStationList();
    /** Reference to Search class instance */
    m_Search = new CAppPandoraSearch();

    m_pNotifier = new CExternalNotifier(this);

    connect(m_Track,SIGNAL(trackInfoReceived()),this,SLOT(NotifyAVWidget()));

    connect(m_Track,SIGNAL(trackUpdated(const quint32)),this,SLOT(OnTrackUpdated(const quint32)));

    connect(m_StationList,SIGNAL(handleError(const quint16 )),this,SIGNAL(handleError(const quint16 )));

    connect(m_Track,SIGNAL(handleError(const quint16 )),this,SIGNAL(handleError(const quint16 )));

    m_connect_type_name = ""; // added by esjang 2013.06.04 for ITS #166991

    m_tuneSelectTimer->setSingleShot( true );
    m_tuneSelectTimer->setInterval( 5000 ); //  5 sec
    connect( m_tuneSelectTimer, SIGNAL(timeout()), this, SLOT(onTuneSelectTimer()) );
    
    //{ modified by esjang 2013.08.26 for ITS #186175
    m_noSkipTimer->setSingleShot( true );
    m_noSkipTimer->setInterval( 5000 ); //  5 sec
    connect( m_noSkipTimer, SIGNAL(timeout()), this, SLOT(onNoSkipTimer()) );
    //} modified by esjang 2013.08.26 for ITS #186175

#ifdef PANDORA_REAR_ENABLED_FOR_CES
    //{ added by esjang 2013.12.31 for CES  
    m_bPandoraInFG[DISPLAY_FRONT] = false;
    m_bPandoraInFG[DISPLAY_REAR] = false;
    //} added by esjang 2013.12.31 for CES  
#endif
    
     //{ modified by yongkyun.lee 2014-03-11 for : BT streaming Mute-play control
     m_PlayMuteTimer->setSingleShot( true );
     m_PlayMuteTimer->setInterval( 500 ); //  0.5 sec
     connect( m_PlayMuteTimer, SIGNAL(timeout()), this, SLOT(onPlayMuteTimer()) );    
     //} modified by yongkyun.lee 2014-03-05 
     vehucleType = nVehicleType; //add vehicle type for DH, 16MY, DH PE wonseok.heo 2014.12.29
     
     connect(m_StationList,SIGNAL(toastLoadingPopup( )),this,SLOT(OnNextTimeOSD()));// modified by yongkyun.lee 2014-09-11 for : ITS 248198
     
}

void CAppPandoraBase::SetConnection(CPndrInterface* inPndrInterface)
{
    //modifed by jyjeon 2014-04-02 for prevent
    if(m_Track) m_Track->SetInterface(inPndrInterface);
    if(m_StationList) m_StationList->SetInterface(inPndrInterface);
    if(m_Search) m_Search->SetInterface(inPndrInterface);
    //modifed by jyjeon 2014-04-02 for prevent
}

/**
 *******************************************************************
 *
 * Destructor for the class. Expected to be called ONLY during
 * device shutdown
 *
 * @param   None
 *
 * @return  None
 *
 *******************************************************************
 */
CAppPandoraBase::~CAppPandoraBase()
{

    if ( m_pTranslator != NULL )
    {
       delete m_pTranslator;
    }
    delete m_pNotifier;
}

/**
*******************************************************************
* Clear the cache on back soft key .
*
* @param  None
*
* @return None
*
*******************************************************************
*/
void CAppPandoraBase::ClearCache()
{
    LOG_TRACE<<"CAppPandoraBase::ClearCache() <-"<<LOG_ENDL;
    m_bPandoraConnected = false;
    if(m_Track) //added by cheolhwan 2014-03-12. Prevent Issue.
        m_Track->ClearCache();
    if(m_StationList)
        m_StationList->ClearCache();
    if(m_Search)
        m_Search->ClearCache();
    bResumePlay = false; // added by esjang 2013.05.07 for Handling BT Call Status
    isInsufficientNetwork = false; //added by esjang 2013.10.31 for insufficient network error 
    // m_PndrInterface.DeInit();
    LOG_TRACE<<"CAppPandoraBase::ClearCache() ->"<<LOG_ENDL;
}

/**
*******************************************************************
* Set cluster update status flag.
*
* @param[in]   value    cluster update flag, 
*                       true: Send cluster date, 
*                       false: Do not send cluster data
*
* @return None
*
*******************************************************************
*/
void CAppPandoraBase::setClusterUpdate(bool value)
{
    m_bClusterUpdate = value;
    LOG_INFO << "CAppPandoraBase::setClusterUpdate m_bClusterUpdate = " << m_bClusterUpdate << LOG_ENDL;
} 



/**
 *******************************************************************
 * This function will be called when application needs to be exited
 * as a result of back request from track view. All cleanup will be
 * performed in this function.
 *
 * @param None
 *
 * @return None
 *
 *******************************************************************
 */
//void CAppPandoraBase::ExitApplication()
void CAppPandoraBase::ExitApplication(bool isJogDial) //modified by esjang 2013.06.21 for Touch Backkey
{

    LOG_INFO<<" CAppPandoraBase::ExitApplication() <-"<<LOG_ENDL;

    StopNotifyAVWidget();
    UpdatePandoraAvailableStatus(false); // user pressed back key .

    //{ modified by esjang 2013.06.21 for Touch BackKey
	///*
    EVT_ID_T eventID;
    if(isJogDial == true)
    {
        eventID = EVT_KEY_SOFT_BACK;

    }
    else
    {
        eventID = EVT_TOUCH_BACK_KEY;
    }

    Event event( eventID, GetThisAppId(), APP_NONE );
	//*/
    //} modified by esjang 2013.06.21 for Touch BackKey
    
	//Event event( EVT_KEY_SOFT_BACK, GetThisAppId(), APP_NONE );
	
    m_eHMIState = CAppPandoraBase::AppExitState;
    // Deinitialize the states
    ClearCache();
    if ( !NotifyUISH(event , DISPLAY_FRONT) )
    {
       LOG_CRITICAL<<"Event notification failed"<<LOG_ENDL;
    }
    LOG_INFO<<" CAppPandoraBase::ExitApplication() ->"<<LOG_ENDL;
}

/**
 *******************************************************************
 * Maintain the current state as BG
 *
 * @param   None
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraBase::HandleHomeKey()
{
#ifdef FRAMEWORK_ENABLED
    LOG_TRACE << "CAppPandoraBase::HandleHomeKey: Start" << LOG_ENDL;

    m_eHMIState = CAppPandoraBase::AppInBG;

    // Send event about pressed EVT_KEY_HOME to UI State Hnadler
    // It will take effect and this application will be switched to Active Screen
    Event event( EVT_KEY_SOFT_HOME, GetThisAppId(), APP_NONE );
    // Notify UISH with EVT_KEY_SOFT_HOME event
    if ( !NotifyUISH(event , DISPLAY_FRONT) )
    {
       LOG_CRITICAL<<"Event notification failed"<<LOG_ENDL;
    }

#endif
}

/**
 *******************************************************************
 * Maintain the current state as BG
 *
 * @param   None
 *
 * @return  None
 *
 *******************************************************************
 */
//void CAppPandoraBase::HandleBackKey()
#ifdef PANDORA_REAR_ENABLED_FOR_CES
void CAppPandoraBase::HandleBackKey(bool isJogDial, bool isRRC) //modified by esjang 2013.06.21 for Touch Backkey
#else
void CAppPandoraBase::HandleBackKey(bool isJogDial) //modified by esjang 2013.06.21 for Touch Backkey
#endif
{
#ifdef FRAMEWORK_ENABLED
#ifdef PANDORA_REAR_ENABLED_FOR_CES
    LOG_TRACE << "esjang 131231 CAppPandoraBase::HandleBackKey: Start: isJogDial: "<< isJogDial<< " isRRC: " << isRRC << LOG_ENDL;
#else
    //LOG_TRACE << "CAppPandoraBase::HandleBackKey: Start: isJogDial: "<< isJogDial << LOG_ENDL;
#endif	

    m_eHMIState = CAppPandoraBase::AppInBG;

    // Send event about pressed EVT_KEY_SOFT_BACK to UI State Hnadler
    // It will take effect and this application will be switched to Active Screen
    
	///*
    EVT_ID_T eventID;
    if(isJogDial == true)
    {
        eventID = EVT_KEY_SOFT_BACK;

    }
    else
    {
        eventID = EVT_TOUCH_BACK_KEY;
    }
    Event event( eventID, GetThisAppId(), APP_NONE );
	//*/
	//Event event( EVT_KEY_SOFT_BACK, GetThisAppId(), APP_NONE );
	
    // Notify UISH with EVT_KEY_SOFT_BACK event

#ifdef PANDORA_REAR_ENABLED_FOR_CES
    //{ modified by esjang 2013.12.31 for CES
    if(!isJogDial)
    {
        LOG_TRACE<<"esjang 131231, touch enabled, display front, Event notification start"<<LOG_ENDL;

        if ( !NotifyUISH(event,DISPLAY_FRONT ) ) // esjang 2013.12.30 for CES 
        {
           LOG_CRITICAL<<"esjang 131231, touch, display front, Event notification failed"<<LOG_ENDL;
        }            
    
    }
    else
    {
        DISPLAY_T display = isRRC? DISPLAY_REAR:DISPLAY_FRONT;
        LOG_TRACE<<"esjang 131231 getCurrentScreen: "<< getCurrentScreen()<< " display: "<< display<< " isRRC: "<< isRRC <<LOG_ENDL;
       
        LOG_TRACE<<"esjang 131231 , Event notification start"<<LOG_ENDL;

        if ( !NotifyUISH(event,display ) ) // esjang 2013.12.30 for CES 
        {
           LOG_CRITICAL<<"esjang 131231 , Event notification failed"<<LOG_ENDL;
        }            

        /*
       if(!bRRC )
       {
            LOG_TRACE<<"esjang 131231,  CCP , Event notification start"<<LOG_ENDL;

            if ( !NotifyUISH(event,DISPLAY_REAR ) ) // esjang 2013.12.30 for CES 
            {
               LOG_CRITICAL<<"esjang 131231, RRC, Event notification failed"<<LOG_ENDL;
            }            

       }
       */

    }
    /*
    if ( !NotifyUISH(event,DISPLAY_FRONT:DISPLAY_REAR ) ) // esjang 2013.12.30 for CES 
    {
       LOG_CRITICAL<<"esjang 131230 Event notification failed"<<LOG_ENDL;
    }
    */
    //} modified by esjang 2013.12.31 for CES
   LOG_CRITICAL<<"esjang 131230 Event notification success"<<LOG_ENDL;
#else
    if ( !NotifyUISH(event,DISPLAY_FRONT) )
    {
       LOG_CRITICAL<<"Event notification failed"<<LOG_ENDL;
    }
#endif
#endif
}

/**
 *******************************************************************
 * Called at start of application for setting the language and
 * signalling to UI about current language. Also called when
 * there is language change event occurs
 *
 * @param   None
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraBase::RetranslateUi()
{
    emit retranslateUi((int)m_nLanguage);
}

/**
 *******************************************************************
 * Return the StationList class Reference
 *
 * @param     None
 *
 * @return    Reference to CAppPandoraStationList
 *
 *******************************************************************
 */
CAppPandoraStationList& CAppPandoraBase::GetStationListRef()
{
    return  *m_StationList;
}

/**
 *******************************************************************
 * Return the Search class Reference
 *
 * @param     None
 *
 * @return    Reference to CAppPandoraSearch
 *
 *******************************************************************
 */
CAppPandoraSearch& CAppPandoraBase::GetSearchRef()
{
    return *m_Search;
}

/**
 *******************************************************************
 * Return the Track class Reference
 *
 * @param     None
 *
 * @return    Reference to CAppPandoraTrack
 *
 *******************************************************************
 */
CAppPandoraTrack& CAppPandoraBase::GetTrackRef()
{
    return *m_Track;
}

PandoraMenus& CAppPandoraBase::GetPandoraMenus()
{
    return m_PandoraMenus;
}
//{ added by esjang 2013.04.26 for Clear TP Message
CExternalNotifier& CAppPandoraBase::GetPandoraNotifier()
{
    return *m_pNotifier;
}
//} added by esjang 2013.04.26 for Clear TP Message

//{ modified for CR 14370 by Vijayakumar K
void CAppPandoraBase::SetPandoraStateActive()
{
    PANDORA_CONN_STATE_T state = PANDORA_ACTIVE ;
    QByteArray connStatus;
    connStatus.append(state);
    Event event( EVT_PANDORA_STATUS, GetThisAppId(), APP_UI_MANAGER , connStatus );

    //{ Modified by esjang 2012.12.22 for SwapDC
    //NotifyUISH( event,DISPLAY_FRONT );
    if ( !NotifyUISHCommon(event) )
    {
        LOG_CRITICAL << "CAppPandoraBase::SetPandoraStateActive: Event notification failed" << LOG_ENDL;
    }
    else
    {
        LOG_INFO<< "CAppPandoraBase::SetPandoraStateActive: Event notification sent" <<LOG_ENDL;
    }
    //} Modified by esjang 2012.12.22 for SwapDC

    // As a default device USBPort1 aquires the audio Path.
    //DeviceInfo::EConnectionType type = DeviceInfo::EUSBPort1;
    //NotifyAudioPathChanged(type);
}
//} modified for CR 14370 by Vijayakumar K

/**
*******************************************************************
* notify the AVWidget with song name .
*
* @param   None
*
* @return  None
*
*******************************************************************
*/
void CAppPandoraBase::NotifyAVWidget()
{

    TrackInfoOSD();//{ modified by yongkyun.lee 2014-04-06 for : ITS 233641
    if(!m_bIsBackGround)
        return;

    HOME_MEDIA_T stData;
    stData.nType = HomeScreenDef::MEDIA_TYPE_PANDORA;
    stData.nSource = HomeScreenDef::MEDIA_SOURCE_INVALID;

    wchar_t sTemp[MEDIA_STRING_LENGHT + 1];

   // QString songName = m_Track->GetCurrentTrackInfo()->getTitle();

    if(m_Track->GetCurrentTrackInfo())
    {
	    QString songName = m_Track->GetCurrentTrackInfo()->getTitle();
	    ( songName + '\0').toWCharArray(sTemp);
	    wcsncpy( stData.sMedia1, sTemp, songName.size() + 1 );
	    QByteArray aData((char*)&stData, sizeof(stData));//QByteArray::fromRawData( (char*)&stData, sizeof(stData) ); // modified by esjang 2013.02.18 for Broken IPC Payload
	    Event event( EVT_POST_EVENT, GetThisAppId(), APP_MAIN_MENU, HomeScreenDef::SET_PLAYING_MEDIA, aData );
	    NotifyUISH( event,DISPLAY_FRONT );
    }
}

/**
*******************************************************************
* stops notify the AVWidget
*
* @param   None
*
* @return  None
*
*******************************************************************
*/
void CAppPandoraBase::StopNotifyAVWidget()
{
    HOME_MEDIA_T stData;
    stData.nType = HomeScreenDef::MEDIA_TYPE_INVALID;
    //QByteArray aData = QByteArray::fromRawData( (char*)&stData, sizeof(stData) );
    QByteArray aData((char*)&stData, sizeof(stData));// modified by esjang 2013.02.18 for Broken IPC Payload

    Event event( EVT_POST_EVENT, GetThisAppId(), APP_MAIN_MENU, HomeScreenDef::SET_PLAYING_MEDIA, aData );
    NotifyUISH( event,DISPLAY_FRONT );
}

/**
 *******************************************************************
 * Launches the sound settings ui
 *
 * @param   None
 *
 * @return  none
 *
 *******************************************************************
 */
void CAppPandoraBase::LaunchSoundSetting()
{
    AppSettingsStartParameter param;
    param.startState = AppSettingsDef::eStart_Mode_Sound;
    //{ Modified by esjang 2013.03.19 for new entry point
    APP_EVENT_DATA_BASE_T pEventData;
    pEventData.sIsJogEnabled = false;

    Event softEvent(EVT_KEY_SOFT_SETTINGS, GetThisAppId(), APP_UI_MANAGER, eAPP_SETTINGS_SOUND,
                QByteArray((char *)&pEventData, sizeof(pEventData))); // modified by lssanh 2013.02.21 IPC payload
    emit NotifyUISH(softEvent, DISPLAY_FRONT);

    //} Modified by esjang 2013.03.19 for new entry point
}

/**
 *******************************************************************
 * Close system popup
 *
 * @param   None
 *
 * @return  none
 *
 *******************************************************************
 */
void CAppPandoraBase::CloseSystemPopup()
{
    Event event( EVT_REQUEST_SYSTEM_POPUP_CLOSE, GetThisAppId(), APP_UI_MANAGER);

    if ( !NotifyUISHCommon(event) )
    {
        LOG_CRITICAL << "CAppPandoraBase::CloseSystemPopup: Event notification failed" << LOG_ENDL;
    }
    else
    {
        LOG_INFO<< "CAppPandoraBase::CloseSystemPopup: Event notification sent" <<LOG_ENDL;
    }
}


/**
 *******************************************************************
 * Check the current status, request connect to RPM and Pandora
 *
 * @param[in] inSelectedIndex , selected index
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraBase::Connect()
{
    LOG_TRACE<<"CAppPandoraBase::Connect <-"<<LOG_ENDL;

    EPndrError error = E_INIT_FAILED ;

    //{ modified for CR 14366 by Vijayakumar K
    /* Initializing the pandora state in UISH for saying Pandora is now Active App*/
    PANDORA_CONN_STATE_T state = PANDORA_ACTIVE ;
    QByteArray connStatus;
    connStatus.append(state);
    Event event( EVT_PANDORA_STATUS, GetThisAppId(), APP_UI_MANAGER , connStatus );
    //{ Modified by esjang 2012.12.22 for SwapDC
    //NotifyUISH( event,DISPLAY_FRONT );
    if ( !NotifyUISHCommon(event) )
    {
        LOG_CRITICAL << "CAppPandoraBase::Connect: Event notification failed" << LOG_ENDL;
    }

    //} Modified by esjang 2012.12.22 for SwapDC
    //} modified for CR 14366 by Vijayakumar K

    //LOG_HIGH<<" Connect m_eCurrentDevice : "<<m_eCurrentDevice<<LOG_ENDL; //log Modified
    if( m_eCurrentDevice == DeviceInfo::EUSBPort1)
    {
        error = m_PndrInterface.Init();
    }
    else if (m_eCurrentDevice == DeviceInfo::EUSBPort2) {
        error = m_PndrInterfaceTwo.Init();
    }
    else if (m_eCurrentDevice == DeviceInfo::EBTConn) {
        error = m_PndrBTInterface.Init();
    }

    if(E_SUCCESS != error)
    {
	    LOG_CRITICAL<<"m_PndrInterface.Init() failed , so return" << error<<LOG_ENDL;
	    emit handleError(error);
	    // Init failed , don't proceed
	    return ;
    }
    mPandora->State()|=INIT;
    //LOG_HIGH<<"************Init Done****************** mPandora->State() = "<<mPandora->State()<<LOG_ENDL;  //log Modified

    EDeviceType deviceType = DEVICE_TYPE_NONE;
    switch(m_eCurrentDevice)
    {
        case DeviceInfo::EBTConn:
             deviceType = DEVICE_TYPE_SMARTPHONE ;
             SetConnection(&m_PndrBTInterface);
         break;
        case DeviceInfo::EUSBPort1:
             deviceType = DEVICE_TYPE_IPHONE_USB_FRONT ;
             SetConnection(&m_PndrInterface);
        break;
        case DeviceInfo::EUSBPort2:
            deviceType = DEVICE_TYPE_IPHONE_USB_REAR ;
             SetConnection(&m_PndrInterfaceTwo);
        break;
        case DeviceInfo::ENoDevice:
        default:
            LOG_CRITICAL<<"Its wrong device type" <<LOG_ENDL;
        break;
    }

    if(deviceType != DEVICE_TYPE_NONE)
    {
        if(m_isMuteState){
            m_isMuteState = false;
            NotifyAudioPathChanged(m_eCurrentDevice);
        }

        if(SUCCESS == (mPandora->State()&SUCCESS))
        {
            startPipeline(mPandora->port());
            UpdatePandoraAvailableStatus(true);
            emit connectionStatusChanged(1);
            //200 m sec , is to transit view , It helps for view switching .
            QTimer::singleShot(200, m_Track, SLOT(devChanged()));
        }
        else
        {
            EPndrError status = E_INVALID_STATE ;
            if( deviceType ==  DEVICE_TYPE_IPHONE_USB_FRONT){
            status=m_PndrInterface.Connect(deviceType,c_AlbumArtDimension,
                                                        IMG_TYPE_JPEG,c_StationArtDimension);
            }
            else if (deviceType ==  DEVICE_TYPE_IPHONE_USB_REAR) {
                status=m_PndrInterfaceTwo.Connect(deviceType,c_AlbumArtDimension,
                                                            IMG_TYPE_JPEG,c_StationArtDimension);
            }
            else if (deviceType ==  DEVICE_TYPE_SMARTPHONE) {
                status=m_PndrBTInterface.Connect(deviceType,c_AlbumArtDimension,
                                                            IMG_TYPE_JPEG,c_StationArtDimension);
            }

            if( E_SUCCESS != status  )
            {
                HandleIfError(status);
            }
            else
            {
                LOG_INFO<<"Connect successfull , Aquire pipeline "<<LOG_ENDL;
                if(mPandora){
                    if(deviceType ==  DEVICE_TYPE_IPHONE_USB_FRONT ||
                            deviceType ==  DEVICE_TYPE_IPHONE_USB_REAR)
                        startPipeline(mPandora->port());
                    bAV_Mode_Changed = false;
                }
            }
        }

    }
    else
    {
        LOG_CRITICAL<<"************handleError****************** "<<LOG_ENDL;
        emit handleError(E_DEVICE_NOT_FOUND);
    }

    m_OSDFlag = true;//{ modified by yongkyun.lee 2014-04-06 for : ITS 233641
 
    //{ modified by yongkyun.lee 2014-09-18 for :  ITS 248606
    if(IsRunningInBG())
        m_pNotifier->DisplayOSD(CExternalNotifier::Econnecting); 
    //} modified by yongkyun.lee 2014-09-18 
    
    LOG_TRACE<<"CAppPandoraBase::Connect ->" << IsRunningInBG() <<LOG_ENDL;
}

/**
*******************************************************************
* Notify to UISH to acquire audio path .
*
* @param  [in] inConnectedDev , device type .
*
* @return None
*
*******************************************************************
*/
void CAppPandoraBase::NotifyAudioPathChanged(DeviceInfo::EConnectionType inConnectedDev)
{
    LOG_TRACE<< "CAppPandoraBase::NotifyAudioPathChanged " << LOG_ENDL ;

    MODE_STATE_T avState = MODE_STATE_NONE;

    switch(inConnectedDev)
        {

            case DeviceInfo::EUSBPort1:
                avState =  MODE_STATE_IPOD1_PANDORA;
            break;
            case DeviceInfo::EUSBPort2:
                avState =  MODE_STATE_IPOD2_PANDORA;
            break;
            case DeviceInfo::EBTConn:
                 avState =  MODE_STATE_BT_PANDORA;
            break;
            case DeviceInfo::ENoDevice:
            default:
                LOG_CRITICAL<<"***Error,Its wrong device type"<<LOG_ENDL;
                break;
        }

    EVENT_AUDIO_PATH_REQUEST_DATA message = { { APP_UI_MANAGER, EVT_REQUEST_AUDIO_PATH, 0, 0 }, avState };

    message.header.dataSize = sizeof( message );

    //{ modified by esjang 2013.02.18 for Broken IPC Payload
    QByteArray temp((char*)&message, sizeof(message));
    Event event( EVT_MUTE_CANCEL /*EVT_REQUEST_AUDIO_PATH*/ , GetThisAppId(), //modified by esjang for chainging Mute I/F
                     APP_UI_MANAGER, temp );
    //} modified by esjang 2013.02.18 for Broken IPC Payload

    
    //{ Modified by esjang 2012.12.22 for SwapDC
    //NotifyUISH( event,DISPLAY_FRONT );
    if ( !NotifyUISHCommon(event ) )
    {
        LOG_CRITICAL << "CAppPandoraBase::NotifyAudioPathChanged: Event notification failed" << LOG_ENDL;
    }
    //} Modified by esjang 2012.12.22 for SwapDC


  }

/**
 *******************************************************************
 * Handle the events sent by UIStateHandler
 *
 * @param[in]   aEvent  event to be handled
 *
 * @return  None
 *
 *******************************************************************
 */
#ifdef FRAMEWORK_ENABLED
void CAppPandoraBase::NotifyFromUIStateManager( Event aEvent )
{
    LOG_TRACE << "CAppPandoraBase::NotifyFromUIStateManager Start" << LOG_ENDL;

#ifdef PANDORA_REAR_ENABLED_FOR_CES
    NotifyFromUIStateManagerGeneric(aEvent, DISPLAY_FRONT ); // modified by esjang 2013.12.31 for CES
#else
    NotifyFromUIStateManagerGeneric(aEvent);
#endif
    
    LOG_TRACE << "CAppPandoraBase::NotifyFromUIStateManager EXIT" << LOG_ENDL;    
}

#ifdef PANDORA_REAR_ENABLED_FOR_CES
void CAppPandoraBase::NotifyFromUIStateManagerRear( Event aEvent )
#else
void CAppPandoraBase::NotifyFromUIStateManagerRear( Event /*aEvent*/ )
#endif
{
    //LOG_TRACE << "CAppPandoraBase::NotifyFromUIStateManagerRear Start" << LOG_ENDL;

#ifdef PANDORA_REAR_ENABLED_FOR_CES
    NotifyFromUIStateManagerGeneric(aEvent, DISPLAY_REAR ); // modified by esjang 2013.12.31 for CES
#else
    //NotifyFromUIStateManagerGeneric(aEvent);
#endif

   // LOG_TRACE << "CAppPandoraBase::NotifyFromUIStateManagerRear EXIT" << LOG_ENDL;
}        

void CAppPandoraBase::NotifyFromUIStateManagerCommon( Event aEvent )
{
    LOG_TRACE << "CAppPandoraBase::NotifyFromUIStateManagerCommon Start :: aEvent.GetEventId()="<<aEvent.GetEventId()<<LOG_ENDL;

    //LOG_INFO<<"aEvent.GetEventId() :"<<aEvent.GetEventId()<<LOG_ENDL;

    switch ( aEvent.GetEventId() )
    {
#ifdef PANDORA_REAR_ENABLED_FOR_CES
    //{ added by esjang 2013.12.30 for CES
    case EVT_SWAP_DC_ENABLED:
    {
        LOG_HIGH << "esjang 131230 EVT_SWAP_DC_ENABLED" << LOG_ENDL;
        m_bSwapEnabled = true;
    }
    break;
    case EVT_SWAP_DC_DISABLED:
    {
        LOG_HIGH << "esjang 131230 EVT_SWAP_DC_DISABLED" << LOG_ENDL;
        
        m_bSwapEnabled = false;
    }
    break;
    //} added by esjang 2013.12.30 for CES
    case EVT_POWER_STATE_NORMAL_OFF:
    {
        //{ modified by yongkyun.lee 2014-12-18 for : ITS 254864
        if(IsSystemPopupVisible())
            CloseSystemPopup();
        if(m_bLocalPopupVisible)
            emit popupClose();
        //} modified by yongkyun.lee 2014-12-18           
        break;
    }
    
    case EVT_POWER_STATE_NORMAL_ON:
    {
        break;
    }
    
#else
    case EVT_POWER_STATE_NORMAL_OFF:
    {
        //{ modified by yongkyun.lee 2014-12-18 for : ITS 254864
        if(IsSystemPopupVisible())
            CloseSystemPopup();
        if(m_bLocalPopupVisible)
            emit popupClose();
        //} modified by yongkyun.lee 2014-12-18           
        break;
    }
    
    case EVT_POWER_STATE_NORMAL_ON:
    {
        break;
    }
#endif

    //When Receive Event Factory Reset Message From UISH
    case EVT_FACTORY_RESET_REQUEST:
    {
        LOG_INFO << "CAppPandoraBase::NotifyFromUIStateManagerCommon Request Factory Reset Message From UISH" << LOG_ENDL;
        Event event(EVT_FACTORY_RESET_RESPONSE, GetThisAppId(), APP_UI_MANAGER);

        // TODO: Delete all the saved files here

        
        if(! NotifyUISHCommon(event))
        {
            LOG_INFO << "Factory Reset Response Failed" << LOG_ENDL;
        }
        break;
    }

    //{ added by esjang for ITS #181703
    case EVT_NOTIFY_AV_MODE:
    {
        //LOG_LOW << "EVT_NOTIFY_AV_MODE" << LOG_ENDL;
        //{ modified by yongkyun.lee 2014-03-11 for : BT streaming Mute-play control
        if( !aEvent.Data().isEmpty() )
        {
            EVT_COMMON_BASE_EVENT_DATA_T* pData = (EVT_COMMON_BASE_EVENT_DATA_T*) aEvent.Data().data();
            m_modeStateHistory.prevMode = m_modeStateHistory.currMode;
            m_modeStateHistory.currMode = (MODE_STATE_T) pData->iEventData01;
            if( m_modeStateHistory.currMode == MODE_STATE_BT_PANDORA )//{ modified by yongkyun.lee 2014-03-21 for : 
                m_modeStateHistory.firstModeChang = true;
            LOG_HIGH << "EVT_NOTIFY_AV_MODE 1: prev->curr :["<< m_modeStateHistory.prevMode << "]->["<< m_modeStateHistory.currMode <<  "] , firstModeChang =" << m_modeStateHistory.firstModeChang << LOG_ENDL;
        }
        //} modified by yongkyun.lee 2014-03-11
        
        m_isTemporalMute = TEMPORAL_UNDEFINED;//{ modified by yongkyun.lee 2014-06-18 for : ITS 240298
       
        if(mPandora && CONNECT == (mPandora->State()&CONNECT) )
        {
            if( !aEvent.Data().isEmpty() )
            {
                EVT_COMMON_BASE_EVENT_DATA_T* pData = (EVT_COMMON_BASE_EVENT_DATA_T*) aEvent.Data().data();
               // LOG_HIGH << "EVT_NOTIFY_AV_MODE 1: iEventData01 =" << pData->iEventData01 << LOG_ENDL;  //log Modified
                switch(pData->iEventData01)
                {
                case MODE_STATE_BT_PANDORA:
                    m_Track->startPlayTimer(); //add by wonseok.heo 2015.01.20 for 256169 AV on off
                case MODE_STATE_IPOD1_PANDORA:
                case MODE_STATE_IPOD2_PANDORA:
                    LOG_HIGH<<" EVT_NOTIFY_AV_MODE from AV off to -> "<< pData->iEventData01<<LOG_ENDL;
                    //{ modified by yongkyun.lee 2014-04-06 for : ITS 233641
                    if(m_modeStateHistory.lastPandora != (MODE_STATE_T)pData->iEventData01 )
                        bAV_Mode_Changed =  true;
                    
                    m_modeStateHistory.lastPandora = (MODE_STATE_T) pData->iEventData01;
                    //} modified by yongkyun.lee 2014-04-06 
                    break;
                case MODE_STATE_AVOFF:
                    //{ modified by yongkyun.lee 2014-12-18 for : ITS 254864
                    if(IsSystemPopupVisible())
                        CloseSystemPopup();
                    if(m_bLocalPopupVisible)
                        emit popupClose();
                    //} modified by yongkyun.lee 2014-12-18                       
                default:
                    if( !aEvent.Data().isEmpty() )
                    {
                        EVENT_MODE_CHANGED_DATA_T* pData = (EVENT_MODE_CHANGED_DATA_T*) aEvent.Data().data();
                        handleAVModeChange(pData->sMode);
                    }
                    StopNotifyAVWidget();
                    break;
                }
                //add
            }
         //StopNotifyAVWidget();
        }
        else{
            if(mPandora && CURR == (mPandora->State()&CURR) ){
                EVT_COMMON_BASE_EVENT_DATA_T* pData = (EVT_COMMON_BASE_EVENT_DATA_T*) aEvent.Data().data();
                //LOG_HIGH << "EVT_NOTIFY_AV_MODE 2: iEventData01 =" << pData->iEventData01 << LOG_ENDL;  //log Modified

                switch(pData->iEventData01)
                {
                case MODE_STATE_BT_PANDORA:
                case MODE_STATE_IPOD1_PANDORA:
                case MODE_STATE_IPOD2_PANDORA:
                    break;
                default:
                    LOG_INFO << "EVT_NOTIFY_AV_MODE Stop the pipe!!" << LOG_ENDL;
                    stopPipe();
                    bAV_Mode_Changed =  true;
                break;
                }
            }
        }

        break;
    }
    //} added by esjang for ITS #181703
    case EVT_DRS_SHOW:
    {
        LOG_HIGH << "EVT_DRS_SHOW common "<<LOG_ENDL;
        m_bScrollingTicker = false;
        emit tickerChanged(m_bScrollingTicker);

        break;
    }
    case EVT_DRS_HIDE:
    {
        LOG_HIGH << "EVT_DRS_HIDE common "<<LOG_ENDL;
        m_bScrollingTicker = true;
        emit tickerChanged(m_bScrollingTicker);
        break;
    }

        //{ added by esjang 2013.04.22 for cluster update, Disable sending cluster data
    case EVT_UPDATE_CLUSTER:
    {
        //LOG_INFO << "esjang 0422 common EVT_UPDATE_CLUSTER" << LOG_ENDL;
        setClusterUpdate(true);
        m_pNotifier->ClearCluster();//added by esjang 2013.06.13 for update cluster timing issue
        // TODO: esjang 2013.06.12
        if(m_Track->GetCurrentTrackInfo() != NULL)
        {
            QString trackTitle = m_Track->GetCurrentTrackInfo()->getTitle();
            if( !trackTitle.isEmpty() )
            {
                m_pNotifier->UpdateTrackTitle(trackTitle);
            }
        }

        //emit sendTpMessage();
        break;
    }//} added by esjang 2013.04.22 for cluster update, Disable sending cluster data

    case EVT_LANGUAGE_CHANGED:
    {
        EVENT_LANGUAGE_CHANGED_DATA_T *pLanguageData = (EVENT_LANGUAGE_CHANGED_DATA_T *)aEvent.Data().data();

        if ( pLanguageData != NULL )
        {
            LOG_TRACE << "EVT_LANGUAGE_CHANGED : " << (int) pLanguageData->languageId  << LOG_ENDL;
            if ( !m_pTranslator )
            {
                m_pTranslator = new QTranslator( this );
            }

            switch ( pLanguageData->languageId )
            {
            //{ modified by esjang 2013.05.28 for language
            case LANGUAGE_KO:
                LOG_TRACE << "HomeMenuAppEngine::NotifyFromUIStateManager:Processing Event::EVT_LANGUAGE_CHANGED::KO" << LOG_ENDL;
                if(IsVehicleType() ==2 ){
                    m_pTranslator->load( "/app/share/AppPandora/localization/PandoraRadioPE_ko" );//added by wonseok.heo for integration DH PE
                }else{
                    m_pTranslator->load( "/app/share/AppPandora/localization/PandoraRadio_ko" );
                }

                break;
            case LANGUAGE_EN_US: //modified by esjang 2013.07.27 for Language
                LOG_TRACE << "HomeMenuAppEngine::NotifyFromUIStateManager:Processing Event::EVT_LANGUAGE_CHANGED::EN" << LOG_ENDL;
                if(IsVehicleType() ==2 ){
                    m_pTranslator->load( "/app/share/AppPandora/localization/PandoraRadioPE_en" );
                }else{
                    m_pTranslator->load( "/app/share/AppPandora/localization/PandoraRadio_en" );
                }

                break;
            case LANGUAGE_FR_NA: //modified by esjang 2013.07.27 for Language
                LOG_TRACE << "HomeMenuAppEngine::NotifyFromUIStateManager:Processing Event::EVT_LANGUAGE_CHANGED::FR_NA" << LOG_ENDL;
                if(IsVehicleType() ==2 ){
                    m_pTranslator->load( "/app/share/AppPandora/localization/PandoraRadioPE_fr-na" );
                }else{
                    m_pTranslator->load( "/app/share/AppPandora/localization/PandoraRadio_fr-na" );
                }

                break;
            case LANGUAGE_ES_NA: //modified by esjang 2013.07.27 for Language
                LOG_TRACE << "HomeMenuAppEngine::NotifyFromUIStateManager:Processing Event::EVT_LANGUAGE_CHANGED::ES_NA" << LOG_ENDL;
                if(IsVehicleType() ==2 ){
                    m_pTranslator->load( "/app/share/AppPandora/localization/PandoraRadioPE_es-na" );
                }else{
                    m_pTranslator->load( "/app/share/AppPandora/localization/PandoraRadio_es-na" );
                }

                break;
            default:
                LOG_TRACE << "HomeMenuAppEngine::NotifyFromUIStateManager:Processing Event::EVT_LANGUAGE_CHANGED::DEF" << LOG_ENDL;
                if(IsVehicleType() ==2 ){
                    m_pTranslator->load( "/app/share/AppPandora/localization/PandoraRadioPE_en" );
                }else{
                    m_pTranslator->load( "/app/share/AppPandora/localization/PandoraRadio_en" );
                }

                break;
                //} modified by esjang 2013.05.28 for language

            }

            qApp->installTranslator( m_pTranslator );
			// added by esjang 2013.12.18 for ITS # 216039
            m_connect_type_name = "";

            switch(m_eCurrentDevice)
            {
                case DeviceInfo::EBTConn:
                    LOG_CRITICAL<<"CAppPandoraBase::playInBG BT Pandora " << LOG_ENDL;
                    m_connect_type_name =  qApp->translate("main","STR_PANDORA_BLUETOOTH"); //added by jyjeon 2014.01.11 for ITS 218524
                break;
                case DeviceInfo::EUSBPort1:
                    if(GetVariantRearUSB())
                    {
                        m_connect_type_name =  qApp->translate("main","STR_PANDORA_FRONT"); // added by esjang 2013.07.08 for static test CID 22885, 22886
                    }
                break;
                case DeviceInfo::EUSBPort2:
                    if(GetVariantRearUSB())
                    {
                        m_connect_type_name =  qApp->translate("main","STR_PANDORA_REAR"); // added by esjang 2013.07.08 for static test CID 22885, 22886
                    }
                break;
            }


            emit retranslateUi( pLanguageData->languageId );
        }
    }
    break;
    case EVT_AV_MODE_NOTIFY_FROM_VR:
    {
        if(mPandora)
            LOG_HIGH<<"EVT_AV_MODE_NOTIFY_FROM_VR : mPandora->State() =="<< mPandora->State() <<LOG_ENDL;
        else
            LOG_HIGH<<"EVT_AV_MODE_NOTIFY_FROM_VR " <<LOG_ENDL;
        if( mPandora && INIT != (mPandora->State()&INIT) )
        {
            EVENT_MODE_CHANGED_DATA_T* pData = (EVENT_MODE_CHANGED_DATA_T*) aEvent.Data().data();
            LOG_HIGH<<"EVT_AV_MODE_NOTIFY_FROM_VR mode : "<<pData->sMode <<LOG_ENDL;
            switch(pData->sMode)
            {
            case MODE_STATE_BT_PANDORA: 
            case MODE_STATE_IPOD1_PANDORA:
            case MODE_STATE_IPOD2_PANDORA:
                playInBackground(pData->sMode);
                break;
            default:
                if(mPandora && CURR == (mPandora->State()&CURR) ){
                    stopPipe();
                }
                bResumePlay = false;
                break;
            }
        }
        //added by jyjeon 2014-02-28 for ITS 227359
        else{
            EVENT_MODE_CHANGED_DATA_T* pData = (EVENT_MODE_CHANGED_DATA_T*) aEvent.Data().data();
            switch(pData->sMode)
            {
            case MODE_STATE_BT_PANDORA:
            case MODE_STATE_IPOD1_PANDORA:
            case MODE_STATE_IPOD2_PANDORA:
                LOG_HIGH<<"EVT_AV_MODE_NOTIFY_FROM_VR : set bAV_Mode_Changed == true " <<LOG_ENDL;
                bAV_Mode_Changed = true;
                break;
            default:
                break;
            }
        }
        //added by jyjeon 2014-02-28 for ITS 227359
    }
    break;
    case EVT_AV_MODE_CHANGE:
    {
        EVENT_MODE_CHANGED_DATA_T* pData = (EVENT_MODE_CHANGED_DATA_T*) aEvent.Data().data();

        if(mPandora)
            LOG_HIGH<<"EVT_AV_MODE_CHANGE mode = " << pData->sMode << "  mPandora->State():" << mPandora->State() <<LOG_ENDL;
        else
            LOG_HIGH<<"EVT_AV_MODE_CHANGE mode = " << pData->sMode <<LOG_ENDL;

        if(mPandora && CONNECT == (mPandora->State()&CONNECT) )
        {
            if( !aEvent.Data().isEmpty())
            {
                //  EVENT_MODE_CHANGED_DATA_T* pData = (EVENT_MODE_CHANGED_DATA_T*) aEvent.Data().data();
               // LOG_HIGH << "EVT_AV_MODE_CHANGE , Pandora is connected , mode =" << pData->sMode << LOG_ENDL;
                //{ modified by yongkyun.lee 2014-04-06 for : ITS 233641
                if(IsRunningInBG() &&
                    ( pData->sMode == MODE_STATE_BT_PANDORA     ||
                      pData->sMode ==  MODE_STATE_IPOD1_PANDORA ||
                      pData->sMode ==  MODE_STATE_IPOD2_PANDORA) ){  //{ modified by yongkyun.lee 2014-06-19 for :
                    playInBackground(pData->sMode);
               } else{
                //} modified by yongkyun.lee 2014-04-06 
                handleAVModeChange(pData->sMode);
                    if(IsSystemPopupVisible()){ // { add by wonseok.heo for ITS 258721
                        CloseSystemPopup();
                    }
                    if(m_bLocalPopupVisible){
                        emit popupClose(); //
                    }
                }
            }
            StopNotifyAVWidget();
        }
        else{
            //LOG_HIGH<<"EVT_AV_MODE_CHANGE mode , But Pandora Not yet connected " <<LOG_ENDL;
            switch(pData->sMode)
            {
            case MODE_STATE_BT_PANDORA:
            case MODE_STATE_IPOD1_PANDORA:
            case MODE_STATE_IPOD2_PANDORA:
                if(IsRunningInBG())
                    playInBackground(pData->sMode);

                break;
            default:
               // LOG_HIGH << "EVT_AV_MODE_CHANGE: mode = No state" << LOG_ENDL;  //log Modified
                if(mPandora && CURR == (mPandora->State()&CURR) && !bAV_Mode_Changed ){
                    stopPipe();
                }
                bResumePlay = false;
                break;
            }
        }

#if 0
        // LOG_INFO<< "m_eHMIState : " << m_eHMIState << "m_bPandoraConnected" << m_bPandoraConnected << LOG_ENDL;
        if( m_eHMIState == CAppPandoraBase::AppInFG ||
                m_eHMIState == CAppPandoraBase::AppInBG )
        {
            if(m_bPandoraConnected)
            {
                if( !aEvent.Data().isEmpty() )
                {
                    EVENT_MODE_CHANGED_DATA_T* pData = (EVENT_MODE_CHANGED_DATA_T*)aEvent.Data().data();

                    LOG_INFO << "EVT_AV_MODE_CHANGE: mode =" << pData->sMode << LOG_ENDL;

                    handleAVModeChange(pData->sMode);
                }
                StopNotifyAVWidget();
            }
        }
#endif

    }
        break;
        
    case EVT_IPOD_MOUNTED:
    {
        //LOG_INFO<<"EVT_IPOD_MOUNTED"<<LOG_ENDL;
        APP_EVENT_IPOD_DATA_T *param = (APP_EVENT_IPOD_DATA_T*)(aEvent.Data().data());
        USB_PORT_T  port =  param->usbPort;
        LOG_HIGH<<"EVT_IPOD_MOUNTED port = " << port <<LOG_ENDL;
        switch(port)
        {
        case eUSB_PORT1:
            if(mPandoraF)delete mPandoraF;
            mPandoraF = new DeviceInfo(DeviceInfo::EUSBPort1,eUSB_PORT1);
            break;
        case eUSB_PORT2:
            if(mPandoraR)delete mPandoraR;
            mPandoraR  = new DeviceInfo(DeviceInfo::EUSBPort2,eUSB_PORT2);
            break;
        case eUSB_PORT_INVALID:
        default:
            break;
        }
    }
        break;
    case EVT_IPOD_REMOVED:
    {
        //LOG_INFO<<"EVT_IPOD_REMOVED"<<LOG_ENDL;

        APP_EVENT_IPOD_DATA_T *param = (APP_EVENT_IPOD_DATA_T*)(aEvent.Data().data());
        USB_PORT_T  port =  param->usbPort;
        LOG_HIGH<<"EVT_IPOD_REMOVED  port = " << port <<LOG_ENDL;
        switch (port)
        {
        case eUSB_PORT1:
            if(mPandoraF)
            {
                LOG_INFO<<"mPandoraF->State() :"<<mPandoraF->State()<<LOG_ENDL;
                if(CURR == (mPandoraF->State()&CURR) ||
                        m_eCurrentDevice == DeviceInfo::EUSBPort1)
                {
                    if(INIT == (mPandoraF->State()&INIT))
                    {
                        if(m_pSignals)
                        {
                            QObject::disconnect(m_pSignals,0,this,0);
                            m_pSignals = NULL;
                        }
                    }

                    Disconnect();
                    m_pNotifier->DeviceDisconnected();
                    m_eCurrentDevice = DeviceInfo::ENoDevice;
                    mPandora = NULL;
                    emit connectionStatusChanged(0); // added by esjang 2013.05.31 for ISV #82820

                }
                else
                {
                    if(INIT == (mPandoraF->State()&INIT))
                    {
                        m_PndrInterface.Disconnect();
                        m_PndrInterface.DeInit();
                    }
                }
                delete mPandoraF;
                mPandoraF =  NULL;
                if(m_eCurrentDevice == DeviceInfo::EUSBPort1){
                    mPandora = NULL;
                    m_eCurrentDevice = DeviceInfo::ENoDevice;
                }
            }

            break;
        case eUSB_PORT2:
            if(mPandoraR)
            {
                LOG_INFO<<"mPandoraR->State() :"<<mPandoraR->State()<<LOG_ENDL;
                if(CURR == (mPandoraR->State()&CURR) ||
                        m_eCurrentDevice == DeviceInfo::EUSBPort2 )
                {
                    if(INIT == (mPandoraR->State()&INIT))
                    {
                        if(m_pSignals)
                        {
                            QObject::disconnect(m_pSignals,0,this,0);
                            m_pSignals = NULL;
                        }
                    }

                    Disconnect();
                    m_pNotifier->DeviceDisconnected();
                    m_eCurrentDevice = DeviceInfo::ENoDevice;
                    mPandora = NULL;
                    // added by esjang 2013.05.31 for ISV #82820
                    emit connectionStatusChanged(0);

                }
                else
                {
                    if(INIT == ((mPandoraR->State())&INIT) ) // modified by esjang 2013.05.22 for static test # 20848
                    {
                        m_PndrInterfaceTwo.Disconnect();
                        m_PndrInterfaceTwo.DeInit();
                    }
                }
                delete mPandoraR;
                mPandoraR =  NULL;
                if(m_eCurrentDevice == DeviceInfo::EUSBPort2){
                    mPandora = NULL;
                    m_eCurrentDevice = DeviceInfo::ENoDevice;
                }
            }
            break;
        default:
            LOG_CRITICAL<<"ERROR USB port is Invalid"<<LOG_ENDL;
            break;
        }
    }
        break;
    case EVT_BT_CONNECTION_STATUS:
        {
            BLUETOOTH_CONNECT_STATUS *param = (BLUETOOTH_CONNECT_STATUS*)(aEvent.Data().data());
            LOG_HIGH<<"BT Connection Status = " << (int)param->sBTStatusEx<<LOG_ENDL;

            if(BT_STATUS_EX_CONNECTED == param->sBTStatusEx /*||
               BT_STATUS_EX_CONNECTING == param->sBTStatusEx*/ ) //removed by jyjeon 2014-02-13 for ITS 221808,222060. (from Radha)
            {
                if(!mBTPandora)
                    mBTPandora = new DeviceInfo(DeviceInfo::EBTConn);
            }
            else
            {
                if(mBTPandora)
                {
                    LOG_INFO<<"mBTPandora->State() :"<<mBTPandora->State()<<LOG_ENDL;
                    if(CURR == (mBTPandora->State()&CURR) ||
                            m_eCurrentDevice == DeviceInfo::EBTConn)
                    {
                        if(INIT == (mBTPandora->State()&INIT))
                        {
                            if(m_pSignals)
                            {
                                QObject::disconnect(m_pSignals,0,this,0);
                                m_pSignals = NULL;
                            }
                        }

                        Disconnect();
                        m_pNotifier->DeviceDisconnected();
                        m_eCurrentDevice = DeviceInfo::ENoDevice;
                        mPandora = NULL;
                        emit connectionStatusChanged(0); // added by esjang 2013.05.31 for ISV #82820

                    }
                    else
                    {
                        if(INIT == (mBTPandora->State()&INIT))
                        {
                            m_PndrBTInterface.Disconnect();
                            m_PndrBTInterface.DeInit();
                        }
                    }
                    delete mBTPandora;
                    mBTPandora =  NULL;
                    if(m_eCurrentDevice == DeviceInfo::EBTConn){
                        mPandora = NULL;
                        m_eCurrentDevice = DeviceInfo::ENoDevice;
                    }
                }
            }
        }
        break;

    case EVT_BT_MUSIC_ENABLED:
         LOG_INFO<<"EVT_BT_MUSIC_ENABLED"<<LOG_ENDL;
         break;
    case EVT_BT_MUSIC_DISABLED:
         LOG_INFO << "EVT_BT_MUSIC_DISABLED"<<LOG_ENDL;
         break;
        //{ added by esjang 2013.03.14 for BT Call, Pandora will be paused
    case EVT_CALL_START:
    {
        if(nCallingState)
        {
            //Its already set
            return;
        }
        LOG_INFO << "EVT_CALL_START "<<LOG_ENDL;
        nCallingState = true;
        //emit callingState(nCallingState); // removed by esjang 2013.10.27 for ITS # 198788, focus is changed before view changing.
        bResumePlay = false ;
        if(m_Track->TrackStatus()== 1)
        {
            bResumePlay = true ;
            m_Track->Pause();
        }
    }
        break;
    case EVT_CALL_END:
    {
        //{ modified by yongkyun.lee 2014-09-04 for : ITS 247991
        if(m_bLocalPopupVisible)
            emit popupClose();
        //} modified by yongkyun.lee 2014-09-04 
        LOG_HIGH << "EVT_CALL_END: Av_mode_was changed:"<<bAV_Mode_Changed<<", resume playback:"<<bResumePlay<<", nCallingState:"<<nCallingState<< LOG_ENDL;
        if(!nCallingState)
        {
            //its already set
            return;
        }
        nCallingState = false;
        emit callingState(nCallingState);
        if( bResumePlay )
        {
            m_Track->Play();
        }
        bResumePlay = false;
        if( !bAV_Mode_Changed && m_Track->GetCurrentTrackInfo() != NULL )
        {
            QString trackTitle = m_Track->GetCurrentTrackInfo()->getTitle();
            if( !trackTitle.isEmpty())
            {
                if(!IsClusterUpdate())
                {
                    m_bClusterUpdate = true;
                }
                m_pNotifier->UpdateTrackTitle(trackTitle);
            }
        }

        m_pNotifier->ClearCluster(); //add by wonseok.heo for Cluster op state 2015.02.04


    }
        break;

        //} added by esjang 2013.03.14 for BT Call, Pandora will be paused
    case EVT_KEY_HU_SEEK_NEXT:
    case EVT_KEY_SWRC_SEEK_NEXT:
#ifdef PANDORA_REAR_ENABLED_FOR_CES
    case EVT_KEY_RRC_SEEK_NEXT: //added by esjang 2013.12.30 for CES 
#endif
    case EVT_KEY_CCP_SEEK_NEXT: //added by cheolhwan 2014-03-20. for KH ITS 227985.
        if( !m_bLoadingPopupVisible ) // modified by esjang 2013.08.21 for BT Call
        {
            if ( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
				LOG_INFO << "isInsufficient network : " << isInsufficientNetwork << " Seek UP" << LOG_ENDL;

                isSeek = true; //add by wonseok.heo for OSD at BG 2015.02.04
                //{ modified by yongkyun.lee 2014-12-12 for : ITS 254504
                if(m_tuneSelectTimer->isActive() && (eventData->keyStatus == KEY_STATUS_PRESSED) )
                {
                    onTuneSelectTimer();
                    IfMute_SentAudioPass();
                }
                //} modified by yongkyun.lee 2014-12-12 
                if ( !IsCalling() && !(m_noSkipTimer->isActive()) && (!isInsufficientNetwork)) // modified by esjang 2013.11.08 for audit issue
                {
                    if(eventData->keyStatus == KEY_STATUS_RELEASED  )
                    {
                        // added by jyjeon 2013.12.12 for ITS 214424
                        if(m_isSeekNextLongKeyPressed)
                        {
                            m_isSeekNextLongKeyPressed = false;
                        }
                        else
                        // added by jyjeon 2013.12.12 for ITS 214424						
                        {
                            //{ modified by esjang 2013.03.20 for ITS # 159179
                            //LOG_INFO << "Seek up key released -> Display OSD : AllowSkip["<<m_Track->GetCurrentTrackInfo()->getAllowSkip()<<"]"<< LOG_ENDL;//esjang 2013.03.20 //removed by cheolhwan 2013-02-15. ITS 225462.
                            //{ modified by cheolhwan 2014-02-15. ITS 225462. added the condition to Prevent.
                            if(m_Track && m_Track->GetCurrentTrackInfo())
                            {
                                if(m_Track->GetCurrentTrackInfo()->getAllowSkip()) //added by cheolhwan 2014-02-09. ITS 0224238. Added the condition.
                                {
                                    //{ modified by yongkyun.lee 2014-11-03 for : //leeyk1 OSD TEST
                                    if(!(m_eViewState == ETrackView && !IsRunningInBG())&&!(isPopupVisibility() == true)) //added by wonseok.heo 2015.04.27 for ITS 261246
                                        m_pNotifier->DisplayOSD(CExternalNotifier::ESeekUp);
                                }
                            }
                            else
                            {
                                //{ modified by yongkyun.lee 2014-11-03 for : //leeyk1 OSD TEST
                                if(!(m_eViewState == ETrackView && !IsRunningInBG())&&!(isPopupVisibility() == true) && m_StationList->GetTotalStationsCount() > 0) //added by wonseok.heo 2015.04.27 for ITS 261246
                                m_pNotifier->DisplayOSD(CExternalNotifier::ESeekUp);
                                LOG_CRITICAL << "To prevent CRASH " << LOG_ENDL;
                                return; //added by cheolhwan. To prevent crashing during the continuous skipping in the commercial station end.
                            }
                            //} modified by cheolhwan 2014-02-15. ITS 225462. added the condition to Prevent.

                            emit skipKeyEvent(false);
                            //} modified by esjang 2013.03.20 for ITS # 159179
                            //emit skipKeyEvent(true);
                            emit handleSkipEvent(true);
                            m_pNotifier->SetSkiped(true); // added by esjang 2013.05.25 for OSD

                            //emit skipKeyEvent(false); //added by esjang 2013.08.01
                        }
                    }
                    else if (eventData->keyStatus == KEY_STATUS_PRESSED) {
                        //{ modified by yongkyun.lee 2014-07-17 for : ITS 243240
                        if(m_Track->GetCurrentTrackInfo() == NULL ) 
                        {
                            if(!(m_eViewState == ETrackView && !IsRunningInBG())&&!(isPopupVisibility() == true)) //added by wonseok.heo 2015.04.27 for ITS 261246
                            m_pNotifier->DisplayOSD(CExternalNotifier::EStationDoesNotExist);
                            //break;
                        }
                        m_isSeekNextLongKeyPressed = false; //added by cheolhwan 2014-02-28. After rebooting, it is not initialized.
                        emit skipKeyEvent(true);
                        m_OSDFlag = true;//{ modified by yongkyun.lee 2014-04-06 for : ITS 233641

                    }
                    // added by jyjeon 2013.12.12 for ITS 214424
                    else if (eventData->keyStatus == KEY_STATUS_PRESSED_LONG) {
                        m_isSeekNextLongKeyPressed = true;
                        emit skipKeyEvent(false);
                        //{ modified by wonseok.heo for ITS 238092 2014.05.22
                        //{ modified by yongkyun.lee 2014-04-06 for : ITS ITS 233689
                        //emit skipNextLongKeyPressed();
                        if(( m_eViewState == EStationListView && isPopupVisibility() == true )|| m_bIsBackGround || m_eViewState == ESearchView || m_eViewState == EExplainationView) { // modified by wonseok.heo for ITS 249431 20141002
                            m_pNotifier->DisplayOSD(CExternalNotifier::ECannotBeManipulated); //added by jyjeon 2013.12.18 for ITS 216143
                        }else{
                            emit skipNextLongKeyPressed();
                            //emit showPopupID(POPUPEnums::POPUP_PANDORA_CANNOT_BE_MANIPULATED);
                        }
                        //} modified by yongkyun.lee 2014-04-06 
                         //} modified by wonseok.heo for ITS 238092 2014.05.22

                    }
                    // added by jyjeon 2013.12.12 for ITS 214424
                }
                else if((!IsCalling()) && isInsufficientNetwork)
                {
                    if(eventData->keyStatus == KEY_STATUS_RELEASED  ) //added by jyjeon 2014-03-21 for ITS 230292
                    {
                        m_pNotifier->DisplayOSD(CExternalNotifier::ENetworkErrorOSD);
                    }
                }
                else if( IsCalling() ) // added by esjang 2013.08.21 for BT phone call
                {
                    if(eventData->keyStatus == KEY_STATUS_RELEASED  ) //added by jyjeon 2014-03-21 for ITS 230292
                    {
                        // modified by jyjeon 2014.01.06 for ITS 217984
                        if(IsRunningInBG())
                        {
                            m_pNotifier->DisplayOSD(CExternalNotifier::ECallingState);
                        }
                        else
                        {
                            //{ added by cheolhwan 2013.11.18. for ITS 209342.
                            //m_pNotifier->UpdateOSDOnCallingState();
                            emit callingPopup();
                            //} added by cheolhwan 2013.11.18. for ITS 209342.
                        }
                        // modified by jyjeon 2014.01.06 for ITS 217984
                    }
                }
            }
        }
        else
        {
            LOG_INFO << "Loading Popup is visible, Seek up key pressed is not working" << LOG_ENDL;
        } // } added by esjang 2013.01.09 for ITS # 150556
        break;
    case EVT_KEY_HU_SEEK_PREV:
    case EVT_KEY_SWRC_SEEK_PREV:
#ifdef PANDORA_REAR_ENABLED_FOR_CES
    case EVT_KEY_RRC_SEEK_PREV: // added by esjang 2013.12.30 for CES 
#endif
    case EVT_KEY_CCP_SEEK_PREV: //added by cheolhwan 2014-03-20. for KH ITS 227985.        
        LOG_INFO << "isInsufficient network : " << isInsufficientNetwork << " Seek Down" << LOG_ENDL;
        //modified by jyjeon 2014-03-21 for ITS 230292
        if ( !aEvent.Data().isEmpty() )
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
            if( !m_bLoadingPopupVisible && (!isInsufficientNetwork) ) // modified by esjang 2013.11.08 for audit issue
            {
                if ( eventData->keyStatus == KEY_STATUS_PRESSED && !IsCalling() ) // modified by esjang 2013.01.08 for ITS # 150499
                {
                    if(m_noSkipTimer->isActive())// added by esjang 2013.11.21 for ITS # 210313
                    {
                        m_noSkipTimer->stop();
                    }
                    //{ modified by yongkyun.lee 2014-07-17 for : ITS 243240
                    if(m_Track->GetCurrentTrackInfo() == NULL ) 
                    {
                        if(!(m_eViewState == ETrackView && !IsRunningInBG())&&!(isPopupVisibility() == true)) //added by wonseok.heo 2015.04.27 for ITS 261246
                        m_pNotifier->DisplayOSD(CExternalNotifier::EStationDoesNotExist);
                        break;
                    }
                    else
                    {
                        if(IsRunningInBG())// modified by yongkyun.lee 2014-10-10 for : ITS 250031
                        {
                            m_pNotifier->DisplayOSD(CExternalNotifier::ESeekDown); //added by esjang 2013.07.10 for ITS # 179225
                        }
                        else
                        {
                            emit handleRewindEvent();
                        }
                    }
                    //} modified by yongkyun.lee 2014-07-17 
                }
                else if(eventData->keyStatus == KEY_STATUS_PRESSED && IsCalling())// added by esjang 2013.08.21 for BT phone call
                {
                    // modified by jyjeon 2014.01.06 for ITS 217984
                    //m_pNotifier->DisplayOSD(CExternalNotifier::ECallingState);
                    if(IsRunningInBG())
                    {
                        m_pNotifier->DisplayOSD(CExternalNotifier::ECallingState);
                    }
                    else
                    {
                        emit callingPopup();
                    }
                    // modified by jyjeon 2014.01.06 for ITS 217984
                }
            }
            //added by esjang 2013.11.10 for audit issue
            else if((!IsCalling()) && isInsufficientNetwork)
            {
                if ( eventData->keyStatus == KEY_STATUS_PRESSED )
                {
                    m_pNotifier->DisplayOSD(CExternalNotifier::ENetworkErrorOSD);
                }
            }
            //added by esjang 2013.11.10 for audit issue
        }
        //modified by jyjeon 2014-03-21 for ITS 230292
        break;
    case EVT_KEY_HU_JOG_DIAL_ENCODER:
    {
        //If app is in BG , or if focus is in play control icon
        if( !IsCalling()&& (!isInsufficientNetwork)) // added by esjang 2013.08.21 for BT phone call
            TuneHandler( aEvent.GetEventId(),aEvent );
        //else //modified by esjang
        //added by esjang 2013.11.10 for audit issue
        else if((!IsCalling()) && isInsufficientNetwork)
        {
            m_pNotifier->DisplayOSD(CExternalNotifier::ENetworkErrorOSD);
        }
        //added by esjang 2013.11.10 for audit issue
        else if(IsCalling())
        {
            //{ modified by yongkyun.lee 2014-03-14 for :  ITS 229248
            if( m_eHMIState == CAppPandoraBase::AppInFG)
            {
                emit callingPopup();
            }
            else
            //} modified by yongkyun.lee 2014-03-14 
            { 
                m_pNotifier->DisplayOSD(CExternalNotifier::ECallingState);
            }
        }
    }
        break;
    case EVT_VR_COMMAND:
    {
        if ( !aEvent.Data().isEmpty() )
        {
            EVENT_EVT_VR_COMMAND_DATA_T *eventData = (EVENT_EVT_VR_COMMAND_DATA_T*)aEvent.Data().data();
            
            LOG_INFO<<"AV_MODE , EVT_VR_COMMAND ,  eventData->commandId : "<<eventData->commandId<< LOG_ENDL;

            //{ modified by yongkyun.lee 2014-07-17 for : ITS 243240
            if(m_Track->GetCurrentTrackInfo() == NULL && IsRunningInBG() ) 
            {        
                LOG_CRITICAL << "CAppPandoraBase::NotifyFromUIStateManagerCommon: GetCurrentTrackInfo is NULL .." << LOG_ENDL;
                m_pNotifier->DisplayOSD(CExternalNotifier::EStationDoesNotExist);
                break;
            }
            //} modified by yongkyun.lee 2014-07-17
            // added by wonseok.heo 2015.04.28 for ITS 262035
            if(IsSystemPopupVisible())
                CloseSystemPopup();
            if(m_bLocalPopupVisible)
                emit popupClose();
            //added by wonseok.heo 2015.04.28 for ITS 262035
            switch ( eventData->commandId )
            {
            case AV_MODE_CONTROL_PLAY:
            {
                int payLoad =  eventData->sVRPayload.data[0] ;
                LOG_INFO<<"AV_MODE_CONTROL_PLAY , payload : "<<payLoad<< LOG_ENDL;
                // payLoad 0 for pause , 1 for play.
                emit playCommandFromVR(payLoad);
            }
                break;
            case AV_MODE_CONTROL_NEXT_MEDIA:
            {
                int payLoad =  eventData->sVRPayload.data[0] ;
                LOG_INFO<<"Received AV_MODE_CONTROL_NEXT_MEDIA , payload : "<<payLoad<< LOG_ENDL;
                // payLoad 0 for Thumbs up , 1 for Thumbs down
                if(payLoad == 1 )
                {
                    payLoad = 2 ; // The value is consistent in app , Thumbs down
                }
                else if(payLoad == 0)
                {
                    payLoad = 1; // The value is consistent in app , Thumbs up
                }

                emit ratingCommandFromVR(payLoad);
            }
                break;
            case AV_MODE_CONTROL_RANDOM:
            {
                LOG_INFO<<"AV_MODE_CONTROL_RANDOM to play shuffle"<< LOG_ENDL;
                //{ modified by yongkyun.lee 2014-11-05 for : 
                if(m_StationList->GetTotalStationsCount() <=0 )
                    m_StationList->GetStationCount();
                //} modified by yongkyun.lee 2014-11-05 

                emit shuffleCommandFromVR();
            }
                break;
            case AV_MODE_PANDORA_RADIO_SKIP:
            {
                if( !m_bLoadingPopupVisible ) // { added by esjang 2013.01.09 for ITS # 150556            
                {

                    LOG_INFO<<"AV_MODE_PANDORA_RADIO_SKIP"<< LOG_ENDL;
                    emit handleSkipEvent(false);
                } // } added by esjang 2013.01.09 for ITS # 150556
            }
                break;
            default:
                break;
            }
        }
        break;
    }

    case EVT_KEY_HU_MENU:
    {
        //esjang - Static Test Defect 7567(Missing Break) 2012.11.01
        LOG_INFO<<"AppPandoraBase:EVT_KEY_HU_MENU "<<LOG_ENDL;

        if ( !aEvent.Data().isEmpty() )
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_PRESSED ) // modified by esjang 2013.01.08 for ITS # 150499
            {
                LOG_SIGNAL<<"AppPandoraBase:menuKeyPressed emitted "<<LOG_ENDL;

                emit menuKeyPressed(true);
            }
        }
        break;
        //esjang end - Static Test Defect 7567(Missing Break) 2012.11.01
    }
    case EVT_KEY_CCP_MENU:
#ifdef PANDORA_REAR_ENABLED_FOR_CES
    case EVT_KEY_RRC_MENU: // added by esjang 2013.12.30 for CES 
#endif
    {
        LOG_INFO<<"AppPandoraBase:EVT_KEY_CCP_MENU "<<LOG_ENDL;

        LOG_INFO << "isInsufficient network : " << isInsufficientNetwork << " ccp menu" << LOG_ENDL;
        if ( !aEvent.Data().isEmpty() /*&& (!isInsufficientNetwork)*/)// modified by esjang 2013.11.08 for audit issue  // removed by cheolhwan 2014-03-03. ITS 227922.
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_PRESSED ) // modified by esjang 2013.01.08 for ITS # 150499
            {
                LOG_SIGNAL<<"AppPandoraBase:menuKeyPressed emitted "<<LOG_ENDL;

                emit menuKeyPressed(true);
            }
        }
        break;
    }

    case EVT_KEY_HU_BACK:
    {
        //esjang - Static Test Defect 7566(Missing Break) 2012.11.01
        LOG_INFO<<"AppPandoraBase:EVT_KEY_HU_BACK "<<LOG_ENDL;

        if ( !aEvent.Data().isEmpty() )
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_PRESSED ) // modified by esjang 2013.01.08 for ITS # 150499
            {
                LOG_SIGNAL<<"AppPandoraBase:backKeyPressed emitted "<<LOG_ENDL;

                emit backKeyPressed(true); //modified by esjang 2013.06.21 for Touch Backkey
            }
        }
        break;
        //esjang end - Static Test Defect 7566(Missing Break) 2012.11.01
    }

    case EVT_KEY_CCP_BACK:
    {
        LOG_INFO<<"AppPandoraBase:EVT_KEY_CCP_BACK "<<LOG_ENDL;

        if ( !aEvent.Data().isEmpty() )
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_PRESSED )
            {
                LOG_SIGNAL<<"AppPandoraBase:backKeyPressed emitted "<<LOG_ENDL;
#ifdef PANDORA_REAR_ENABLED_FOR_CES
                emit isRRCKey(false);
#endif

                emit backKeyPressed(true); //modified by esjang 2013.06.21 for Touch Backkey
            }
        }
        break;
    }

#ifdef PANDORA_REAR_ENABLED_FOR_CES
    case EVT_KEY_RRC_BACK: //{ added by esjang 2013.12.30 for CES  
    {
        LOG_INFO<<"AppPandoraBase:EVT_KEY_RRC_BACK "<<LOG_ENDL;
        
        if ( !aEvent.Data().isEmpty() )
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_PRESSED )
            {
                LOG_SIGNAL<<"AppPandoraBase:backKeyPressed emitted "<<LOG_ENDL;
                emit isRRCKey(true);

                emit backKeyPressed(true); //modified by esjang 2013.06.21 for Touch Backkey
            }
        }
        break;
    }
#endif

    case EVT_KEY_SOFT_MENU:
    {
        LOG_INFO<<"EVT_KEY_SOFT_MENU "<<LOG_ENDL;
        emit menuKeyPressed(false);
        break;
    }
    case EVT_KEY_HU_TUNE_PRESSED:
    {
        LOG_INFO<<"EVT_KEY_HU_TUNE_PRESSED "<<LOG_ENDL;
        //        m_pNotifier->DisplayOSD(CExternalNotifier::ETuneKeyPress);

        //If app is in BG , or if focus is in play control icon
        if( !IsCalling() && (!isInsufficientNetwork) ) // modified by esjang 2013.08.21 for BT phone call
            TuneHandler( aEvent.GetEventId(), aEvent );
        //added by esjang 2013.11.10 for audit issue
        else if((!IsCalling()) && isInsufficientNetwork)
        {
            m_pNotifier->DisplayOSD(CExternalNotifier::ENetworkErrorOSD);
        }
        //added by esjang 2013.11.10 for audit issue
        else if(IsCalling()) //modified by esjang
        {
            //{ modified by yongkyun.lee 2014-03-14 for :  ITS 229248
            if ( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
            
                if ( eventData->keyStatus == KEY_STATUS_RELEASED )
                {
                    if( m_eHMIState == CAppPandoraBase::AppInFG)
                    {
                        emit callingPopup();
                    }
                    else
                    { 
                        m_pNotifier->DisplayOSD(CExternalNotifier::ECallingState);
                    }
                }
            }
            //} modified by yongkyun.lee 2014-03-14 
        }
        
        break;
    }

        //Pandora play for Last Mode: esjang
    case EVT_RESTORE_LAST_AV_MODE:
    {
        //Pandora is the last AV mode.
        bAV_Mode_Changed = false;
        LOG_INFO<<"EVT_RESTORE_LAST_AV_MODE has been received"<<LOG_ENDL;
        LOG_INFO<<"Play the Pandora"<<LOG_ENDL;
        m_Track->Play();
        NotifyAVWidget();
        m_pNotifier->DisplayOSD(CExternalNotifier::ETrackInfo);
    }
        break;
        //esjang

    case EVT_SETTINGS_CHANGED:
    {
        if (! aEvent.Data().isEmpty() )
        {

            EVENT_SETTINGS_CHANGED_DATA_T *eventData = (EVENT_SETTINGS_CHANGED_DATA_T*)aEvent.Data().data();
            if( eventData->sKey == AppSettingsDef::DB_KEY_SCROLL_TICKER )
            {
                m_bScrollingTicker = (eventData->sValue == 1);
                emit scrollingTickerChanged();
            }
        }

        break;
    }

    case EVT_REQEUST_MUTE:
    {
        LOG_CRITICAL<< "EVT_REQUEST_MUTE" <<LOG_ENDL;
        m_isMuteState = true;
        break;
    }
    //{ modified by yongkyun.lee 2014-04-07 for : NOCR
    case EVT_REQEUST_UNMUTE:
    {
        LOG_CRITICAL<< "EVT_REQEUST_UNMUTE" <<LOG_ENDL;
        m_isMuteState = false;
        break;
    }
    //} modified by yongkyun.lee 2014-04-07 
    default:
        break;
    }    

    LOG_TRACE << "CAppPandoraBase::NotifyFromUIStateManagerCommon EXIT" << LOG_ENDL;
}

#ifdef PANDORA_REAR_ENABLED_FOR_CES
void CAppPandoraBase::NotifyFromUIStateManagerGeneric( Event aEvent, DISPLAY_T display )
#else
void CAppPandoraBase::NotifyFromUIStateManagerGeneric( Event aEvent )
#endif
{
#ifdef PANDORA_REAR_ENABLED_FOR_CES
    LOG_TRACE << "CAppPandoraBase::NotifyFromUIStateManagerGeneric Start, display: "<< display << LOG_ENDL;
#else
    LOG_TRACE << "CAppPandoraBase::NotifyFromUIStateManagerGeneric Start ::aEvent.GetEventId()="<<aEvent.GetEventId()<<LOG_ENDL;
#endif
    //LOG_INFO<<"aEvent.GetEventId() :"<<aEvent.GetEventId()<<LOG_ENDL;
    //{ added by esjang 2013.01.28 for No CR, Updated UX
    APP_ENTRY_POINT_DATA_T *pEventData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();
    //} added by esjang 2013.01.28 for No CR, Updated UX

    switch ( aEvent.GetEventId() )
    {
        case EVT_FRONT_LCD_ON:
        {
            qDebug() << "F LCD ON";
            m_isFrontLCD = true;
            break;
        }

        case EVT_FRONT_LCD_OFF:
        {
            qDebug() << "F LCD OFF";
            m_isFrontLCD = false;
            break;
        }
        case EVT_REQUEST_BG:
        {
            LOG_HIGH<<"EVT_REQUEST_BG sAppEntryPoint -> "<<pEventData->sAppEntryPoint
                <<"[ Time = "<< (QTime::currentTime()).toString("hh.mm.ss.zzz")<<"]"<<LOG_ENDL;
            m_eHMIState = CAppPandoraBase::AppInBG;
#ifdef PANDORA_REAR_ENABLED_FOR_CES
            m_bPandoraInFG[display] = false; // added by esjang 2013.12.31 for CES  
#endif
            if(m_tuneSelectTimer->isActive())
                onTuneSelectTimer();
            //removed by esjang for ITS # 217174
            //emit background();
            m_bIsBackGround = true;
            m_pNotifier->ClearOSD(); // added by esjang 2013.03.21 for OSD Update
           // NotifyAVWidget();
            if(IsSystemPopupVisible()) // added by wonseok.heo for ITS 266691
                CloseSystemPopup();
            if(m_bLocalPopupVisible)
                emit popupClose();
        }
        break;

        case EVT_REQUEST_PRE_FG:
        {
         LOG_HIGH<<"EVT_REQUEST_PREFG sAppEntryPoint -> "<<pEventData->sAppEntryPoint
                 <<"[Time : "<< (QTime::currentTime()).toString("hh.mm.ss.zzz")<< "], bTemporalMode:" << pEventData->sHeader.bTemporalMode <<LOG_ENDL;

        }
        break;
        case EVT_REQUEST_FG:
        {
#ifdef PANDORA_REAR_ENABLED_FOR_CES
        LOG_HIGH<<"EVT_REQUEST_FG sAppEntryPoint -> "<<pEventData->sAppEntryPoint << " display: "<< display
                    <<"[ Time = "<< (QTime::currentTime()).toString("hh.mm.ss.zzz")<<"]"<<LOG_ENDL;
#else
            LOG_HIGH<<"EVT_REQUEST_FG sAppEntryPoint -> "<<pEventData->sAppEntryPoint
                    <<"[ Time = "<< (QTime::currentTime()).toString("hh.mm.ss.zzz")<<"], bTemporalMode:" << pEventData->sHeader.bTemporalMode <<LOG_ENDL;
#endif

            DeviceInfo::EConnectionType type = DeviceInfo::ENoDevice;
            m_connect_type_name = ""; 
#ifdef PANDORA_REAR_ENABLED_FOR_CES
        m_bPandoraInFG[display] = true; // added by esjang 2013.12.31 for CES  
#endif

            switch(pEventData->sAppEntryPoint)
            {
            case eAPP_PANDORA_MODE_IPOD1:
            {
                if(mPandoraF == NULL)
                {
                    mPandoraF = new DeviceInfo(DeviceInfo::EUSBPort1,eUSB_PORT1);
                }
                type = DeviceInfo::EUSBPort1;
                if(GetVariantRearUSB())
                {
                    m_connect_type_name =  qApp->translate("main","STR_PANDORA_FRONT"); // added by esjang 2013.07.08 for static test CID 22885, 22886
                }
                break;
            }
            case eAPP_PANDORA_MODE_IPOD2:
            {
                if(mPandoraR == NULL)
                {
                    mPandoraR = new DeviceInfo(DeviceInfo::EUSBPort2,eUSB_PORT2);
                }
                type = DeviceInfo::EUSBPort2;
                if(GetVariantRearUSB())
                {
                    m_connect_type_name =  qApp->translate("main","STR_PANDORA_REAR"); // added by esjang 2013.07.08 for static test CID 22885, 22886
                }
                break;
            }
            case eAPP_PANDORA_MODE_BT:
            {
                if(mBTPandora == NULL)
                {
                    mBTPandora = new DeviceInfo(DeviceInfo::EBTConn);
                }
                type = DeviceInfo::EBTConn;
                m_connect_type_name =  qApp->translate("main","STR_PANDORA_BLUETOOTH");//qApp->translate("main","BT "); //modified by jyjeon 2014.01.15 for ITS 219728
                break;
            }
            default:
                m_connect_type_name.clear(); //added by esjang for static test

                LOG_CRITICAL<<"Wrong entry point return**********"<<LOG_ENDL;
                return;
            }

            m_eHMIState = CAppPandoraBase::AppInFG;
            m_bIsBackGround = false;
            m_pNotifier->ClearOSD(); // added by esjang 2013.03.21 for OSD Update
            StopNotifyAVWidget();        

            // added by esjang 2013.12.24 for ITS # 217174
            if(pEventData->sHeader.bTemporalMode == FALSE)
                emit background();
            // added by esjang 2013.12.24 for ITS # 217174

            if(m_eCurrentDevice == DeviceInfo::ENoDevice)//No device mounted yet
            {
                m_eCurrentDevice = type;
                SetDevice();// device changed
                mPandora->State()|=CURR;
                //startPipeline(port);
                m_Track->RetriveState(mPandora);
                m_StationList->RetriveState(mPandora);
                emit deviceChanged();
            }
            else if(m_eCurrentDevice != type)
            {
                    stopPipe();
                if(m_pSignals)
                {
                    QObject::disconnect(m_pSignals,0,this,0);
                   // SetConnection(NULL);
                    //m_pSignals = NULL;
                }

                if(mPandora == NULL)
                {
                    LOG_HIGH<<"PandoraBase.cpp ::EVT_REQUEST_FG : mPandora == NULL " <<LOG_ENDL; //log Modified
                    break;
                }

                if(CONNECT != (mPandora->State()&CONNECT) )
                {
                    Disconnect();
                    mPandora->State()=0;
                }
                else
                {
                    if(mPandora) //added by jyjeon 2014-05-28 for static test
                    {
                        mPandora->State()^=CURR;
                        m_Track->SaveState(mPandora);
                        m_StationList->SaveState(mPandora);                
                    }

                    if(m_Track->TrackStatus() == 1){
                        m_Track->Pause();
                    }                
                    ClearCache();
                    SetConnection(NULL);
                }

                //IMP : Before set device , change the current device type.
                m_eCurrentDevice = type;
                SetDevice(); // device changed
                if(mPandora) //added by jyjeon 2014-05-28 for static test
                {
                    mPandora->State()|=CURR;
                    m_Track->RetriveState(mPandora);
                    m_StationList->RetriveState(mPandora);
                    emit deviceChanged();
                }
            }
            else
            {
                LOG_INFO << "EVT_REQUEST_FG pEventData->sHeader.bTemporalMode: " << pEventData->sHeader.bTemporalMode << " mPandora->State():" << mPandora->State() << "  bAV_Mode_Changed:" << bAV_Mode_Changed << LOG_ENDL;
                //if(pEventData->sHeader.bTemporalMode == FALSE && mPandora->State()&CONNECT)
                if(pEventData->sHeader.bTemporalMode == FALSE && mPandora->State()&CONNECT)
                {
                    emit avModeChanged();
                }
                // added by esjang for No CR error view reconnection
                if(pEventData->sHeader.bTemporalMode == FALSE && m_eViewState == CAppPandoraBase::EErrorView)
                {
                    emit deviceChanged();
                }
                //Pandora is connected,Av mode has changed->Then play pandora
                if(mPandora->State()&CONNECT && bAV_Mode_Changed == true)
                {
                    LOG_INFO<<"EVT_REQUEST_FG AV_Mode was Changed , So Play track "<<LOG_ENDL;
                    bAV_Mode_Changed = false;

                    //last mode was ipod , on Av switch start an assync timer
                    //to avoid mix sound and abnormal behaviour of play status.

                    if(m_isMuteState){
                        m_isMuteState = false;
                        NotifyAudioPathChanged(m_eCurrentDevice);
                    }
                    OnSyncWithIPod();
                }
                else
                {
                   if( mPandora && CONNECT != (mPandora->State()&CONNECT) ){
                       //ITS 0210311 fix
                       if(m_eViewState != CAppPandoraBase::EErrorView &&
                               m_eViewState != CAppPandoraBase::EDiscoonectedView &&
                               m_eViewState != CAppPandoraBase::ENoView)
                            startPipeline(mPandora->port());
                   }
                }
            }

            emit foreground();
            //{ added by esjang 2013.10.27 for ITS # 198788
            LOG_INFO << "EVT_REQUEST_FG Emit calling state on FG, nCallingState : " << nCallingState << LOG_ENDL;
            if(!isInsufficientNetwork)
            {
                LOG_INFO << "EVT_REQUEST_FG update calling state isInsufficientNetwork : " << isInsufficientNetwork << LOG_ENDL;
                emit callingState(nCallingState);
            } 
            //} added by esjang 2013.10.27 for ITS # 198788
            
            onNoSkipTimer(); // added by esjang 2013.08.26 for ITS #186175
            if(m_tuneSelectTimer->isActive())//{ modified by yongkyun.lee 2014-03-10 for : ISV 98550
                onTuneSelectTimer(); //added by jyjeon 2014-02-13 for ITS 225156
            
            if(m_Track->GetCurrentTrackInfo() != NULL)
            {
                QString trackTitle = m_Track->GetCurrentTrackInfo()->getTitle();
                if( !trackTitle.isEmpty() && m_bClusterUpdate && !nCallingState )
                {
                    m_pNotifier->UpdateTrackTitle(trackTitle);
                }
            }
           // LOG_HIGH<<"EVT_REQUEST_FG Finished"  //log Modified
            //        <<"[Time : "<< (QTime::currentTime()).toString("hh.mm.ss.zzz")<< "]"<<LOG_ENDL;
    }
    break;

    case EVT_DRS_SHOW:
    {
        LOG_INFO << "EVT_DRS_SHOW"<<LOG_ENDL;
        m_bScrollingTicker = false;
        emit tickerChanged(m_bScrollingTicker);

        break;
    }
    case EVT_DRS_HIDE:
    {
       LOG_INFO << "EVT_DRS_HIDE"<<LOG_ENDL;
       m_bScrollingTicker = true;
       emit tickerChanged(m_bScrollingTicker);

        break;
    }

    default:
        break;
    }
    LOG_TRACE << "CAppPandoraBase::NotifyFromUIStateManagerGeneric EXIT" << LOG_ENDL;

}
//} Modified by esjang 2012.12.22 for SwapDC
#endif

void CAppPandoraBase::SetDevice()
{
    if(m_eCurrentDevice == DeviceInfo::EUSBPort1)
    {
        m_pSignals = &(m_PndrInterface.GetSignalProvider());
        mPandora = mPandoraF ;
    }
    else if(m_eCurrentDevice == DeviceInfo::EUSBPort2){
        m_pSignals = &(m_PndrInterfaceTwo.GetSignalProvider());
        mPandora = mPandoraR ;
    }
    else if(m_eCurrentDevice == DeviceInfo::EBTConn){
        m_pSignals = &(m_PndrBTInterface.GetSignalProvider());
        m_pBTSignals = m_pSignals; //added by cheolhwan 2014-02-27. Added "m_pBTSignals" to receive BT Signal.ive BT signal.
        mPandora = mBTPandora ;
     }

    //{ modified by yongkyun.lee 2014-06-26 for : NOCR :m_pBTSignals connect 
    if(m_pBTSignals){
        QObject::disconnect(m_pBTSignals,0,this,0);
            connect(m_pBTSignals,SIGNAL(Result(const ERequestType, const EPndrError)),
            this,SLOT(OnBTRequestCompleted(const ERequestType, const EPndrError)) ,
                    Qt::QueuedConnection);
    }
    //} modified by yongkyun.lee 2014-06-26 

    if(m_pSignals){
        QObject::disconnect(m_pSignals,0,this,0);

        connect(m_pSignals,SIGNAL(Result(const ERequestType, const EPndrError)),
        this,SLOT(OnRequestCompleted(const ERequestType, const EPndrError)) ,
                Qt::QueuedConnection);
        connect(m_pSignals,SIGNAL(SessionTerminateAcked()),
             this, SLOT(onSessionTerminateAcked()), Qt::QueuedConnection);
        connect(m_pSignals,SIGNAL(AudioSamplingChanged(int)),
             this, SLOT(onAudioSampleRateChanged(int)), Qt::QueuedConnection);
    }
}

void CAppPandoraBase::OnTrackUpdated(const quint32 inTrackToken)
{
    LOG_INFO<<"CAppPandoraBase::OnTrackUpdated : inTrackToken="<<inTrackToken<<", mPandora->State()="<<mPandora->State()<<LOG_ENDL;
    mPandora->SetToken(inTrackToken);
    mPandora->State()|=SUCCESS;
	//added by esjang 2013.10.31 for ITS # 195120
    if(isInsufficientNetwork)
    {
        LOG_INFO << "insufficient networkerror false! CAppPandoraBase::OnTrackUpdated : " << LOG_ENDL;
        isInsufficientNetwork = FALSE;
    }
}

/**
 *******************************************************************
 * The result of request from Pandora MW
 * All failed requests related to network to be handled here only.
 *
 * @param[in] inRequestType The request for which the reply is
 *                          received
 * @param[in] inPndrError   The result of the request
 *
 * @return  None
 *
 *******************************************************************
 */

//{ modified by yongkyun.lee 2014-06-18 for :  ITS 240298

void
CAppPandoraBase::TemporalMute()
{
    EVENT_AUDIO_PATH_REQUEST_DATA requestData;
    requestData.avState = MODE_STATE_BT_PANDORA;    

    LOG_HIGH<<"TemporalMute , m_isTemporalMute=" << m_isTemporalMute<<LOG_ENDL;
    if(m_isTemporalMute != TEMPORAL_MUTE )
        return;

    Event tEvent(EVT_AV_TEMPORAL_MUTE, APP_PANDORA, APP_UI_MANAGER, QByteArray((char*)&requestData, sizeof(requestData)));
    //modifed by jyjeon 2014-04-02 for prevent
    if ( !NotifyUISHCommon(tEvent) )
    {
        LOG_CRITICAL << "CAppPandoraBase::TemporalMute: Event notification failed" << LOG_ENDL;
    }
    else
    {
        //m_isTemporalMute = TEMPORAL_MUTE;
        LOG_HIGH<<"TemporalMute Signal isAudio"<<LOG_ENDL;
    }
    //modifed by jyjeon 2014-04-02 for prevent
}

void
CAppPandoraBase::TemporalUnmute()
{
    EVENT_AUDIO_PATH_REQUEST_DATA requestData;
    requestData.avState = MODE_STATE_BT_PANDORA;    

    LOG_HIGH<<"TemporalUnmute , m_isTemporalMute=" << m_isTemporalMute<<LOG_ENDL;
    if(m_isTemporalMute != TEMPORAL_UNMUTE )
        return;

    Event tEvent(EVT_AV_TEMPORAL_UNMUTE, APP_PANDORA, APP_UI_MANAGER, QByteArray((char*)&requestData, sizeof(requestData)));
    //modifed by jyjeon 2014-04-02 for prevent
    if ( !NotifyUISHCommon(tEvent) )
    {
        LOG_CRITICAL << "CAppPandoraBase::TemporalUnmute: Event notification failed" << LOG_ENDL;
    }
    else
    {
        //m_isTemporalMute = TEMPORAL_UNMUTE;
        LOG_HIGH<<"TemporalUnmute Signal isAudio"<<LOG_ENDL;
    }
    //modifed by jyjeon 2014-04-02 for prevent
}


//} modified by yongkyun.lee 2014-06-18 

//{ modified by yongkyun.lee 2014-03-11 for : BT streaming Mute-play control
void
CAppPandoraBase::setUnMuteForBTStream()
{
	EVENT_AUDIO_PATH_REQUEST_DATA requestData;
    requestData.avState = MODE_STATE_BT_PANDORA;    

    Event tEvent(EVT_REQUEST_AUDIO_PATH, APP_PANDORA, APP_UI_MANAGER, QByteArray((char*)&requestData, sizeof(requestData)));
    //modifed by jyjeon 2014-04-02 for prevent
    if ( !NotifyUISHCommon(tEvent) )
    {
        LOG_CRITICAL << "CAppPandoraBase::setUnMuteForBTStream: Event notification failed" << LOG_ENDL;
    }
    else
    {
        LOG_HIGH<<"setUnMuteForBTStream Signal isAudio"<<LOG_ENDL;
    }
    //modifed by jyjeon 2014-04-02 for prevent
}
//} modified by yongkyun.lee 2014-03-11

void CAppPandoraBase::OnRequestCompleted(const ERequestType inRequestType,
                                         const EPndrError inPndrError)
{
    LOG_HIGH<<"CAppPandoraBase::OnRequestCompleted <- inRequestType["<<inRequestType<<"] inPndrError["<<inPndrError<<"]"<<LOG_ENDL;

    //If the error is No Stations or No Active Stations,
    //even in that case Connect is Success
    int status = ((E_SUCCESS == inPndrError)
                  || (E_NO_STATIONS == inPndrError)
                  || (E_NO_ACTIVE_STATIONS == inPndrError)
                  || (E_SESSION_ALREADY_STARTED == inPndrError) // modified by esjang 2013.05.07 for
                  || (E_TRACK_RATING_FAILED == inPndrError)
                  || (E_STATION_DOES_NOT_EXIST == inPndrError)
                  || (E_INSUFFICIENT_CONNECTIVITY == inPndrError) )? 1 :0 ; // added by esjang 2013.05.07 for certification # 8.3, 8.7 Read only

    if(m_Track)
    {
        m_Track->OnRequestCompleted(inRequestType , inPndrError);
    }
    if(status && mPandora){
        mPandora->State()|=CONNECT; // Set state
        if(E_NO_ACTIVE_STATIONS == inPndrError || E_NO_STATIONS == inPndrError){
            mPandora->State()|= SUCCESS;
        }
    }

    switch (inRequestType)
    {
        case REQ_TYPE_PAUSE: // connect successful
        case REQ_TYPE_PLAY: // connect successful
        {
            LOG_INFO<<"Successfully pandoraconnected"<<LOG_ENDL;
            UpdatePandoraAvailableStatus(true);
            // added by esjang 2013.12.17 for ITS # 215678
            if(inRequestType == REQ_TYPE_PLAY && isInsufficientNetwork)
                isInsufficientNetwork = false;

            //{ modified by yongkyun.lee 2014-06-18 for :  ITS 240298
            if(m_eCurrentDevice == DeviceInfo::EBTConn && m_isTemporalMute == TEMPORAL_MUTE)
            {
                m_isTemporalMute = TEMPORAL_UNMUTE;
                TemporalUnmute();
            }
            m_isTemporalMute = TEMPORAL_UNDEFINED;
            //} modified by yongkyun.lee 2014-06-18 

            //{ modified by yongkyun.lee 2014-03-11 for : BT streaming Mute-play control
            if(getModeStateHistoryFMode() && inRequestType == REQ_TYPE_PLAY)//{ modified by yongkyun.lee 2014-03-24 for : ISV 99097
            {
                m_PlayMuteTimer->start();
                //LOG_HIGH<<"CAppPandoraBase::OnRequestCompleted :: setModeStateHistoryFMode(false)"<<LOG_ENDL;     //log Modified
                setModeStateHistoryFMode(false);
            }
            //} modified by yongkyun.lee 2014-03-11 

            emit connectionStatusChanged(status);
        }
        break;
        case REQ_TYPE_DISCONNECT: // disconnect
        {
         LOG_INFO<<"REQ_TYPE_DISCONNECT"<<LOG_ENDL;

            UpdatePandoraAvailableStatus(false); // pandora dis connected .
            emit connectionStatusChanged(status);
        }
        break;
        case REQ_TYPE_CONNECT:
        {     
            //{ added by jyjeon 2014-02-20 for ITS 223475 -> modified by cheolhwan 2014-02-28.
            if(status != 1 && inPndrError == E_DEVICE_NOT_FOUND)
            {
                LOG_INFO << "E_DEVICE_NOT_FOUND: " << inPndrError << LOG_ENDL;
                if(mBTPandora)
                {
                    LOG_HIGH<<"mBTPandora->State() :"<<mBTPandora->State()<<" m_eCurrentDevice:"<<m_eCurrentDevice<<LOG_ENDL;
                    if(CURR == (mBTPandora->State()&CURR) ||
                            m_eCurrentDevice == DeviceInfo::EBTConn)
                    {
                        //Disconnect(); //remove by jyjeon 2014-04-03 for NAQC. Tune press error because stationlist.count is zero.
                        m_pNotifier->DeviceDisconnected();
                        mPandora->State() = 0;
                        //{ modified by yongkyun.lee 2014-06-18 for :  ITS 240298
                        m_isTemporalMute = TEMPORAL_MUTE;
                        TemporalMute();
                        //} modified by yongkyun.lee 2014-06-18 
                    }
                    //{ modified by yongkyun.lee 2014-06-26 for : NOCR :m_pBTSignals connect 
                    if(m_pBTSignals)
                        m_pBTSignals= NULL;
                    //} modified by yongkyun.lee 2014-06-26 
                }
            }
            //} added by jyjeon 2014-02-20 for ITS 223475 -> modified by cheolhwan 2014-02-28.
			
            //{ added by esjang 2013.06.27 for ISV #71393, ITS #167008
            if(status != 1 && (inPndrError == E_CHECK_PNDR_APP_ON_DEVICE) )
            {
                LOG_INFO << "E_CHECK_PNDR_APP_ON_DEVICE: " << inPndrError << LOG_ENDL;
                isNetworkError = true;
            }
            //} added by esjang 2013.06.27 for ISV #71393, ITS #167008

            emit connectionStatusChanged(status);

            if(E_NO_STATIONS == inPndrError)
            {
                //added by jyjeon 2014-03-26 for null check
                if(m_Track) m_Track->ClearCache();
                if(m_StationList) m_StationList->ClearCache();
                if(m_Search) m_Search->ClearCache();
                //added by jyjeon 2014-03-26 for null check
            }
        }
        break;
        default:
        {
            LOG_CRITICAL<<"Default:"<<inRequestType<<LOG_ENDL;
            break;
        }

    }
    if(inPndrError != E_SUCCESS)
    {		
        HandleIfError(inPndrError);
    }

    //LOG_SLOT<<"CAppPandoraBase::OnRequestCompleted ->"<<LOG_ENDL;

}

/**
 *******************************************************************
 * The result of request from Pandora MW for BT.
 * All failed requests related to network to be handled here only.
 *
 * @param[in] inRequestType The request for which the reply is
 *                          received
 * @param[in] inPndrError   The result of the request
 *
 * @return  None
 *
 *******************************************************************
 */
//{ added by cheolhwan 2014-02-27. Added "m_pBTSignals" to receive BT Signal.
void CAppPandoraBase::OnBTRequestCompleted(const ERequestType inRequestType,
                                         const EPndrError inPndrError)  //BCH_CHECK. "emit Result( , )" in DHAVN_PndrStateHandler.cpp. 
{
    LOG_SLOT<<"CAppPandoraBase::OnBTRequestCompleted <- inRequestType["<<inRequestType<<"] inPndrError["<<inPndrError<<"]"<<LOG_ENDL;

    //If the error is No Stations or No Active Stations,
    //even in that case Connect is Success
    int status = ((E_SUCCESS == inPndrError)
                  || (E_NO_STATIONS == inPndrError)
                  || (E_NO_ACTIVE_STATIONS == inPndrError)
                  || (E_SESSION_ALREADY_STARTED == inPndrError)
                  || (E_TRACK_RATING_FAILED == inPndrError)
                  || (E_STATION_DOES_NOT_EXIST == inPndrError)
                  || (E_INSUFFICIENT_CONNECTIVITY == inPndrError) )? 1 :0 ;

    switch (inRequestType)
    {
        case REQ_TYPE_CONNECT:
        {
            if(status != 1 && inPndrError == E_DEVICE_NOT_FOUND)
            {
                LOG_INFO << "E_DEVICE_NOT_FOUND: " << inPndrError << LOG_ENDL;
                if(mBTPandora)
                {
                    LOG_HIGH<<"mBTPandora->State() :"<<mBTPandora->State()<<" m_eCurrentDevice:"<<m_eCurrentDevice<<LOG_ENDL;
                    if(CONNECT == (mBTPandora->State()&CONNECT) /*||
                            m_eCurrentDevice == DeviceInfo::EBTConn*/)
                    {
                        m_PndrBTInterface.Disconnect();
                        m_PndrBTInterface.DeInit();
                        mBTPandora->State() = 0;
                    }
                    else
                    {
                        if(INIT == (mBTPandora->State()&INIT))
                        {
                            m_PndrBTInterface.Disconnect();
                            m_PndrBTInterface.DeInit();
                            mBTPandora->State() = 0;
                        }
                    }
                    //{ modified by yongkyun.lee 2014-06-18 for :  ITS 240298
                    m_isTemporalMute = TEMPORAL_MUTE;
                    TemporalMute(); //{ modified by yongkyun.lee 2014-06-18 for :    
                    //} modified by yongkyun.lee 2014-06-18 
                    //{ modified by yongkyun.lee 2014-06-26 for :  NOCR :m_pBTSignals connect 
                    if(m_pBTSignals)
                        m_pBTSignals= NULL;
                    //} modified by yongkyun.lee 2014-06-26 
                }
            }
        }
        break;
        default:
        {
            LOG_CRITICAL<<"Default:"<<inRequestType<<LOG_ENDL;
            break;
        }
    }
    //LOG_SLOT<<"CAppPandoraBase::OnBTRequestCompleted ->"<<LOG_ENDL;
}
//} added by cheolhwan 2014-02-27. Added "m_pBTSignals" to receive BT Signal.

/**
 *******************************************************************
 * The result of request from Pandora MW
 * All failed requests related to network to be handled here only.
 *
 * @param[in] inRequestType The request for which the reply is
 *                          received
 * @param[in] inPndrError   The result of the request
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraBase::onSessionTerminateAcked()
{
    LOG_SLOT<<"CAppPandoraBase::onSessionTerminateAcked <-"<<LOG_ENDL;
    Disconnect();
    LOG_SLOT<<"CAppPandoraBase::onSessionTerminateAcked ->"<<LOG_ENDL;
}

/**
 *******************************************************************
 * The result of request from RPM Client
 *
 * @param[in] inAM Audio mode for which request was raised
 * @param[in] inSink   Audio Sink which is provided for the request
 *
 * @return  None
 *
 *******************************************************************
 */
#ifdef FRAMEWORK_ENABLED
void CAppPandoraBase::OnRPMAcquireResponseReceived(TAudioMode /*tAM*/ ,TAudioSink /*tAS*/)
{

}
#endif

/**
 *******************************************************************
 * Check the current status, request disconnect to RPM and Pandora
 *
 * @param None
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraBase::Disconnect()
{
    LOG_INFO<<"CAppPandoraBase::Disconnect() device :"<<m_eCurrentDevice<<" <-"<<LOG_ENDL;
    ClearCache();
    if(m_eCurrentDevice == DeviceInfo::EUSBPort1)
    {
        m_PndrInterface.Disconnect();
        m_PndrInterface.DeInit();
    }
    else if(m_eCurrentDevice == DeviceInfo::EUSBPort2)
    {
        m_PndrInterfaceTwo.Disconnect();
        m_PndrInterfaceTwo.DeInit();
    }
 	else if(m_eCurrentDevice == DeviceInfo::EBTConn)
    {
        m_PndrBTInterface.Disconnect();
        m_PndrBTInterface.DeInit();
    }
    SetConnection(NULL);

    if(mPandora)
    {
        LOG_INFO<<"mPandora->State()="<<mPandora->State()<<" , is Av mode changed : "<< bAV_Mode_Changed<<LOG_ENDL;
        if(CURR == (mPandora->State() & CURR)){
            if(!bAV_Mode_Changed)
            {
                stopPipe();
            }
            mPandora->State() = CURR;
            bAV_Mode_Changed = false;
        }

       LOG_INFO<<"Disconnected , state : "<<mPandora->State()<<LOG_ENDL;
    }

    LOG_INFO<<"CAppPandoraBase::Disconnect() ->"<<LOG_ENDL;
}

//{ added by esjang 2013.05.15 for certification issue, sessionterminate.
/**
 *******************************************************************
 * Check the current status, request disconnect to RPM and Pandora
 *
 * @param None
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraBase::SessionTerminate()
{
    LOG_INFO<<"CAppPandoraBase::SessionTerminate()"<< LOG_ENDL;

    if(isNetworkError)
    {
        LOG_INFO<<"CAppPandoraBase::SessionTerminate() it is Network error "<< LOG_ENDL;
        Disconnect();
        isNetworkError = FALSE;
    }
    else
    {
        LOG_INFO<<"CAppPandoraBase::SessionTerminate() It is not an Network error "<< LOG_ENDL;

        if(m_eCurrentDevice == DeviceInfo::EUSBPort1)
        {
            //m_PndrInterface.Disconnect();
            m_PndrInterface.SessionTerminate();
        }
        else if(m_eCurrentDevice == DeviceInfo::EUSBPort2)
        {
            //m_PndrInterfaceTwo.Disconnect();
            m_PndrInterfaceTwo.SessionTerminate();
        }
		else{
			if(m_eCurrentDevice == DeviceInfo::EBTConn)
                m_PndrBTInterface.SessionTerminate();
        }
    }

    LOG_INFO<<"CAppPandoraBase::SessionTerminate() ->"<<LOG_ENDL;
}
//} added by esjang 2013.05.15 for certification issue, sessionterminate.

/**
 *******************************************************************
 * Handle the error , if any error exists notify to qml layer
 *
 * @param[in] inPndrError error code
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraBase::HandleIfError(const EPndrError inPndrError)
{

    if(E_SUCCESS != inPndrError)
    {
        LOG_CRITICAL<<"CAppPandoraBase::HandleIfError error = "<<inPndrError<<LOG_ENDL;
        switch(inPndrError)
        {

            case E_NETWORK_FAILED:
            {
                LOG_CRITICAL<<"Handle Network Failure.E_NETWORK_FAILED :" << IsRunningInBG()<<LOG_ENDL;
                UpdatePandoraAvailableStatus(false); // network error received .
                isNetworkError = TRUE; // added by esjang 2013.05.16 for certification issue, networkerror
                emit networkError();
            }
            break;
            case E_SESSION_ALREADY_STARTED:
            {
                LOG_CRITICAL<<"E_SESSION_ALREADY_STARTED "<<LOG_ENDL;
                break;
            }
            
            //{ modified by yongkyun.lee 2014-07-17 for : ITS 243240
            case E_NO_STATIONS        : // modified by yongkyun.lee 2014-11-03 for : //leeyk1 OSD TEST
            case E_NO_ACTIVE_STATIONS :
            {
                LOG_CRITICAL<<"CAppPandoraBase::HandleIfError E_NO_ACTIVE_STATIONS :"<<IsRunningInBG()<<LOG_ENDL;
                if(IsRunningInBG())
                    m_pNotifier->DisplayOSD(CExternalNotifier::EStationDoesNotExist);
            }
            //} modified by yongkyun.lee 2014-07-17 
            case E_INVALID_STATE:
            case E_UNKNOWN_ERROR:
            case E_NOTICE_ERROR_MAINTENANCE:
            case E_STATION_DOES_NOT_EXIST:
            default:
            {
                if(inPndrError == E_INIT_FAILED || inPndrError == E_DEVICE_NOT_FOUND ||
                inPndrError == E_BT_CONNECTION_FAILED || inPndrError == E_USB_CONNECTION_FAILED
                                            || inPndrError == E_CHECK_PNDR_APP_ON_DEVICE)
                {
                    if(inPndrError == E_CHECK_PNDR_APP_ON_DEVICE && m_bPandoraConnected)
                    {
                        isNetworkError = true;
                        LOG_CRITICAL<<" App Data session closed while pandora running"<<LOG_ENDL;
                    }
                    else
                    {
                        UpdatePandoraAvailableStatus(false); // error received .
                    }
                }
                if(inPndrError == E_SKIP_LIMIT_REACHED)
                {
                    if(IsRunningInBG())
                    {
                        m_pNotifier->DisplayOSD(CExternalNotifier::ESkipLimitReached);//{ modified by yongkyun.lee 2014-11-03 for : //leeyk1 OSD TEST
                        m_noSkipTimer->start(); // modified by esjang 2013.08.26 for ITS #186175
                }
                }
                //esjang  131026 for network error toast check                
                if(inPndrError == E_INSUFFICIENT_CONNECTIVITY)
                {
                    isInsufficientNetwork = true;
                    //m_pNotifier->DisplayOSD(CExternalNotifier::ENoNetwork); //added by jyjeon 2014-03-27 for NAQC Request
                    LOG_CRITICAL << "Insufficient network error TURE" << LOG_ENDL;
                }
                emit handleError(inPndrError);
                //emit networkErrorState(isInsufficientNetwork); //added by 2013.11.08 esjang for audit issue
            }
            break;
        }
    }
}

/**
*******************************************************************
* Register to receive mode key event .
*
* @param  None
*
* @return None
*
*******************************************************************
*/
void CAppPandoraBase::UpdatePandoraAvailableStatus(bool inStatus)
{

#ifdef FRAMEWORK_ENABLED
    LOG_INFO<<" CAppPandoraBase::UpdatePandoraAvailableStatus() <- inStatus:"<<inStatus <<LOG_ENDL;
    m_bPandoraConnected = false;
    PANDORA_CONN_STATE_T state = PANDORA_DISCONNECTED ;
    if(inStatus)
    {
        state = PANDORA_CONNECTED ;
        m_bPandoraConnected = true;
    }
    else
    {
        m_bOptionMenuVisible = false;

        //{ modified by yongkyun.lee 2014-12-11 for : ITS 254288
        // if(IsRunningInBG() && IsConnected() )
        // {      
        //     m_pNotifier->DisplayOSD(CExternalNotifier::EDisconnect);
        // }
        //} modified by yongkyun.lee 2014-12-11 
    }
    QByteArray status;
    status.append(state);
    Event event( EVT_PANDORA_STATUS, GetThisAppId(), APP_UI_MANAGER , status );
    if ( !NotifyUISH(event,DISPLAY_FRONT) )
    {
       LOG_CRITICAL<<"Event notification failed"<<LOG_ENDL;
    }
    LOG_INFO<<" CAppPandoraBase::UpdatePandoraAvailableStatus() -> inStatus:"<<inStatus<<LOG_ENDL;
#endif
}

/**
 *******************************************************************
 * Launch key pad setting
 *
 * @param   None
 *
 * @return  void
 *
 *******************************************************************
 */
void CAppPandoraBase::LaunchKeypadSettings()
{
#ifdef FRAMEWORK_ENABLED
    //{ modified by esjang 2013.05.06 for new setting entry point
    APP_EVENT_DATA_BASE_T pEventData;
    pEventData.sIsJogEnabled = false;

    Event softEvent(EVT_KEY_SOFT_SETTINGS, GetThisAppId(), APP_UI_MANAGER, eAPP_SETTINGS_GENERAL_KEYBOARD,
                    QByteArray((char *)&pEventData, sizeof(pEventData))); // modified by lssanh 2013.02.21 IPC payload
    emit NotifyUISH(softEvent, DISPLAY_FRONT );
    //} modified by esjang 2013.05.06 for new setting entry point

#endif
}

/**
*******************************************************************
* Handle AV_MODE has changed event .
*
* @param mode , current av mode
* @return None
*
*******************************************************************
*/
void CAppPandoraBase::handleAVModeChange(MODE_STATE_T mode)
{
    LOG_HIGH<<"HandleAVModeChange mode = "<<mode << " bAV_Mode_Changed = " << bAV_Mode_Changed
            <<" isInsufficientNetwork = " << isInsufficientNetwork <<" nCallingState = " << (int)nCallingState <<LOG_ENDL;
    //LOG_INFO << "esjang 131030 for OSD AV modechange!! " << LOG_ENDL;

    switch(mode)
    {
        case MODE_STATE_BT_PANDORA:
        case MODE_STATE_IPOD1_PANDORA:
        case MODE_STATE_IPOD2_PANDORA:
            LOG_INFO << "Need to handle on Av Off to Av on state"<<LOG_ENDL;
            
            //if(bAV_Mode_Changed){
            if(bAV_Mode_Changed && (!isInsufficientNetwork)){ //  modified by esjang for preventing timeout error AV OFF -> ON during 'insufficient network'
				//LOG_INFO << "esjang 131030 avmode change for pandora connection && !isInsufficient network" << LOG_ENDL;
                bAV_Mode_Changed = false;
                startPipeline(mPandora->port());
                if(!nCallingState) // added by esjang 2013.10.27 for ITS # 198788
                    m_Track->Play(); // added by esjang 2013.01.02 for ITS #150689
                //{ added by esjang 2014.01.07 for ITS # 217681 
                if(nCallingState)
                {
                    //LOG_INFO << "esjang 140107 for ITS # 217681, nCallingState is true, set bResumePlay true" << LOG_ENDL;
                    bResumePlay = true ;
                }
                //} added by esjang 2014.01.07 for ITS # 217681
            }
            //{ added by esjang 2013.01.15 for ITS # 150689, 150494, Display OSD for AV ON Event.
            NotifyAVWidget();
            //LOG_INFO << "esjang 131030 for OSD AV modechange for pandora!! " << LOG_ENDL;
            //LOG_INFO << "esjang 131030 isInsufficientNetwork : " << isInsufficientNetwork << LOG_ENDL; 
			//+ " mPandora &&CONNECT == (mPandora->State()&CONNECT) : " + mPandora && CONNECT == (mPandora->State()&CONNECT) << LOG_ENDL;
            if(mPandora && CONNECT == (mPandora->State()&CONNECT) && (!isInsufficientNetwork))
            {
                //LOG_INFO << "esjang 131030 for OSD track title!! " << LOG_ENDL;              
                //m_pNotifier->DisplayOSD(CExternalNotifier::ETrackInfo);//{ modified by yongkyun.lee 2014-04-06 for : ITS 233641
            }
            else
            {
                //Pandora disconnected or Pandora has network error
                //LOG_INFO << "esjang 131030 for OSD disconnected!! " << LOG_ENDL;
                if(isInsufficientNetwork)
                {
                    //LOG_INFO << "esjang 131030 for network osd !!" << LOG_ENDL;
                    m_pNotifier->DisplayOSD(CExternalNotifier::ENetworkErrorOSD);
                }
                else
                {
                    //LOG_INFO << "esjang 131030 for disconnect osd !!" <<LOG_ENDL;
                    m_pNotifier->DisplayOSD(CExternalNotifier::EDisconnect);
                }
            }
            //} added by esjang 2013.01.15 for ITS # 150689, 150494, Display OSD for AV ON Event.            
            
        break;
        case MODE_STATE_AVOFF:
            m_pNotifier->DisplayOSDAVOff();
            // No break required here
            //{ modified by yongkyun.lee 2014-12-18 for : ITS 254864
            if(IsSystemPopupVisible())
                CloseSystemPopup();
            if(m_bLocalPopupVisible)
                emit popupClose();
            //} modified by yongkyun.lee 2014-12-18               
        default:
            setClusterUpdate(false); // added by esjang 2013.04.22 for cluster update, Disable sending cluster data
            bResumePlay = false;
            if(!bAV_Mode_Changed)
            {
                bAV_Mode_Changed = true;
                if(!isInsufficientNetwork) //added by esjang 2013.11.10 for audit issue
                    m_Track->HandleAVModeChange();
                stopPipe();
            }
            emit avModeChanged(); // added by esjang 2013.01.28 for No CR, Updated UX
        break;
    }
}

//{ added by esjang 2013.03.18 for Preventing Abnormal termination(SW WDT)
void CAppPandoraBase::signalConnection()
{
    struct sigaction act;
    memset(&act,0x00,sizeof(act)); //added by jyjeon 2014-03-13

    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    //act.sa_flags |= SA_RESTART;
    act.sa_flags |= SA_RESETHAND;
    act.sa_handler = CAppPandoraBase::signalHandler;
    //sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGSEGV); // added by esjang 2013.03.26 for StaticTest # 20162 Uninitialized scalar variable

    for( int signumber = SIGHUP ; signumber<SIGUNUSED ; signumber++)
    {
        if( signumber == SIGKILL || signumber == SIGSTOP || signumber == SIGCHLD ) continue;
        if(sigaction(signumber, &act, (struct sigaction *)NULL)<0)
        {
            LOG_HIGH<<"CAppPandoraBase::signalConnection()"<<LOG_ENDL;
            //exit(1); //removed by jyjeon 2014-03-13
        }
    }
}

void CAppPandoraBase::setSignalHandlerObject(CAppPandoraBase* m)
{
    //LOG_TRACE<<"CAppPandoraBase::setSignalHandlerObject()"<<LOG_ENDL;
    qDebug()<< QString("CAppPandoraBase::restart()");

    SigM= m;
}

void CAppPandoraBase::signalHandler(int signum)
{
    //LOG_TRACE<<"CAppPandoraBase::signalHandelr()"<<LOG_ENDL;
    qDebug()<< QString("CAppPandoraBase::signalHandelr()");
    qDebug("Signum = %d", signum);
    SigM->restart(signum);
    raise(signum);
}

void CAppPandoraBase::restart(int sig)
{
    //LOG_TRACE<<"CAppPandoraBase::restart()"<<LOG_ENDL;
    qDebug()<< QString("CAppPandoraBase::restart()");

    Event event( EVT_REQUEST_ABNORMAL_TERMINATION, GetThisAppId(), APP_UI_MANAGER, (EVT_COMMAND_T)sig);
    if ( !NotifyUISH(event, DISPLAY_FRONT) )
    {
        qDebug("CAppPandoraBase::restart: Event notification failed");
    }
    //exit(0); //removed by esjang 201.09.10 for WDT raise() method
}
//} added by esjang 2013.03.18 for Preventing Abnormal termination(SW WDT)



bool CAppPandoraBase::FindDevice (USB_PORT_T port , QString& inSrc)
{
    LOG_INFO<< "CAppPandoraBase::FindDevice START" <<LOG_ENDL;
    QString     qnode;
    USB_PORT_T	portNum;
    bool		rtnValue = false;

    inSrc.clear();

    struct udev*            udev            = 0;
    struct udev_enumerate*  enumerate       = 0;
    struct udev_list_entry* devices         = 0;
    struct udev_list_entry* dev_list_entry  = 0;
    struct udev_device*     soundDev        = 0;
    struct udev_device*     dev             = 0;

    const char* manufacturer;
    const char* vendorID;
    const char* model;
    const char* syspath;

    LOG_TRACE<<"---> CDeviceDetector::FindDevices()"<<LOG_ENDL;

    /* Create the udev object */
    udev = udev_new();

    if( udev == NULL ) {
        LOG_TRACE << "Can't create udev ref.";
        return rtnValue;
    }

    /* Create a list of the devices in the 'usb' subsystem. */
    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "sound");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);
    /* For each item enumerated, print out its information.
    udev_list_entry_foreach is a macro which expands to
    a loop. The loop will be executed for each member in
    devices, setting dev_list_entry to a list entry
    which contains the device's path in /sys. */

    udev_list_entry_foreach(dev_list_entry, devices)
    {
        const char *path, *node;

        /* Get the filename of the /sys entry for the device
        and create a udev_device object (dev) representing it */
        path      = udev_list_entry_get_name(dev_list_entry);
        soundDev  = udev_device_new_from_syspath(udev, path);
        dev		  = udev_device_get_parent_with_subsystem_devtype(
                    soundDev,
                    "usb",
                    "usb_device");
        node = udev_device_get_devnode(dev);

        if( node != 0 )
        {
            qnode = node;
            LOG_TRACE<<"Device Node Path: "<<LOG_ENDL;

            manufacturer = udev_device_get_sysattr_value(dev, "manufacturer");
            vendorID = udev_device_get_sysattr_value(dev, "idVendor");
            LOG_TRACE<<QString("   idVendor : %1").arg( vendorID )<<LOG_ENDL;

            QString serialNum = udev_device_get_sysattr_value(dev, "serial");
            LOG_TRACE<<QString("   serialNum : %1").arg( serialNum)<<LOG_ENDL;

            QString card_num(udev_device_get_sysattr_value(soundDev, "number"));
            LOG_TRACE<<QString("   cardNum   : %1").arg( card_num )<<LOG_ENDL;
            if( card_num.isEmpty() == true )		continue;

            model = udev_device_get_property_value(dev, "ID_MODEL");
            syspath = udev_device_get_syspath(dev);
            LOG_TRACE<<QString("   syspath : %1").arg(syspath)<<LOG_ENDL ;
            LOG_TRACE<<QString("   driver : %1").arg( udev_device_get_driver(dev) )<<LOG_ENDL;

            if( manufacturer && model ) {
                //VendorID is "05ac" for all Apple devices
                if ( strncmp(vendorID,"05ac",4) == 0 && ( strncmp(model,"iPod",4) == 0 || strncmp(model,"iPad",4) == 0 ) ) {
                    LOG_TRACE<<QString("    %1 %2 detect").arg(model, manufacturer)<<LOG_ENDL;
                    portNum = getPortNum( syspath );

                    if( portNum == port) {//m_currentIPodPortNum ) {
                        //m_DeviceConnected = true;

                        QString gstPath("plughw:");
                        QString device_num(",0");
                        gstPath.append(card_num);
                        gstPath.append(device_num);
                        inSrc.append(gstPath);

                       // m_Track.SetiPodDevPath(gstPath);
                        //m_iPodAudioDevPath = gstPath;
                        //m_isAudioPathSet = true;
                        LOG_TRACE<<QString("GSTAudioRouter Path : %1").arg( gstPath )<<LOG_ENDL;
                        //LOG_TRACE<<QString(" GSTAudioRouter Path : %1").arg( m_iPodAudioDevPath )<<LOG_ENDL;

                        rtnValue = true;
                    }
                }
                //VendorID is "05ac" for all Apple devices
                else if ( strncmp (vendorID,"05ac",4) == 0 && strncmp (model,"iPhone",6) == 0 ) {
                    LOG_TRACE<<QString("    %1 %2 detect").arg(model, manufacturer)<<LOG_ENDL;

                    portNum = getPortNum( syspath );

                    if( portNum == port ) {//m_currentIPodPortNum ) {
                        //m_DeviceConnected = true;

                        QString gstPath("plughw:");
                        QString device_num(",0");
                        gstPath.append(card_num);
                        gstPath.append(device_num);

                        inSrc.append(gstPath);
                        //QString m_iPodAudioDevPath  = "";
                        //m_iPodAudioDevPath = gstPath;
                        //m_Track.SetiPodDevPath(gstPath);
                        //m_isAudioPathSet = true;
                        LOG_TRACE<<QString("GSTAudioRouter Path : %1").arg( gstPath )<<LOG_ENDL;
                        //LOG_TRACE<<QString(" GSTAudioRouter Path : %1").arg( m_iPodAudioDevPath )<<LOG_ENDL;

                        rtnValue = true;
                    }
                }
                else {
                    LOG_TRACE<<QString("   manufacturer: %1").arg( manufacturer )<<LOG_ENDL;
                    LOG_TRACE<<QString("   ID_MODEL: %1").arg( model )<<LOG_ENDL;
                }
            }
        }
    }

    /* Free the enumerator object */
    udev_enumerate_unref(enumerate);
    /* Free the udev object */
    udev_unref(udev);

    LOG_TRACE<<QString("<--- DHAVN_UDEVListener::FindDevices()")<<LOG_ENDL;
    LOG_INFO<< "CAppPandoraBase::FindDevice OUT " <<LOG_ENDL;

    return rtnValue;
}

USB_PORT_T CAppPandoraBase::getPortNum(QString sysPath)
{
    // { add vehicle type for DH, 16MY, DH PE wonseok.heo 2014.12.29
    QString UsbPort2_2( "2-2" );
    QString UsbPort1_1( "1-1" );

/*    QString UsbPort1;
    if( vehucleType == 0 ){ // value ( 0: DH, 1: DH15MY, 2:DH PE )
        UsbPort1="2-2";
    }else{
        UsbPort1="1-1"; // modified by wonseok.heo for usbport 2-2 => 1-1 2014.12.17
    }
    // }add vehicle type for DH, 16MY, DH PE wonseok.heo 2014.12.29*/
    QString UsbPort2( "2-1" );

    USB_PORT_T portNum = eUSB_PORT_INVALID;

    // Read the 8th section in the absolute path of the iPod connection
    // which specifies, whether the iPod is connected to USB1 or USB2
    QString portString = sysPath.section( '/' ,8 ,8 );
    LOG_TRACE <<"CAppPandoraBase::getPortNum: portString="<<portString<<LOG_ENDL;
    if( vehucleType == 0 ){ // value ( 0: DH, 1: DH15MY, 2:DH PE )
     // Checking for the USB 1 or USB 2
     portNum = ( portString.compare( UsbPort2_2 ) ) ?
         ( ( portString.compare( UsbPort2 ) ) ? eUSB_PORT_INVALID : eUSB_PORT2 ) :
         eUSB_PORT1;
     }else{
         portNum = ( portString.compare( UsbPort1_1 ) ) ?
             ( ( portString.compare( UsbPort2 ) ) ? eUSB_PORT_INVALID : eUSB_PORT2 ) :
             eUSB_PORT1;

     }
    // Checking for the USB 1 or USB 2
   /* portNum = ( portString.compare( UsbPort1 ) ) ?
        ( ( portString.compare( UsbPort2 ) ) ? eUSB_PORT_INVALID : eUSB_PORT2 ) :
        eUSB_PORT1;*/
    LOG_TRACE <<"CAppPandoraBase::getPortNum: port num= "<<portNum<<LOG_ENDL;
    return portNum;
}

void CAppPandoraBase::startPipeline(USB_PORT_T port)
{
    m_DeviceSrc.clear();
   // LOG_CRITICAL <<"CAppPandoraBase::startPipeline() START"<< LOG_ENDL;
    if(m_pipeThread == NULL){
        m_pipeThread = new CPipeLineThread();
        connect(m_pipeThread, SIGNAL(finished()), this, SLOT(Notified()));
        m_pipeThread->start();
    }
    if(port != eUSB_PORT_INVALID && FindDevice(port , m_DeviceSrc))
    {
        //LOG_INFO<<"found  inSrc : "<<m_DeviceSrc<<LOG_ENDL;
        m_pipeThread->SetSourceType(port);
        m_pipeThread->HandlePipeLine(CPipeLineThread::EStart , m_DeviceSrc,44100);
    }
    else
    {        
        LOG_CRITICAL <<" startPipeline source not found , Probably for BT"<< LOG_ENDL;

    }

    bAV_Mode_Changed = false; //Reaquired by pandora
    //LOG_CRITICAL <<" CAppPandoraBase::startPipeline() end"<< LOG_ENDL;

}

/**
*******************************************************************
* Pandora is paused, Stop Pipeline.
*
* @param  None.
*
* @return None
*
*******************************************************************
*/
void CAppPandoraBase::stopPipe()
{
    //LOG_CRITICAL <<"CAppPandoraBase::stopPipe() IN"<< LOG_ENDL;
    if( m_pipeThread == NULL )
    {
        LOG_CRITICAL <<"CAppPandoraBase::stopPipe() PipeThread is not created"<< LOG_ENDL;
        return;
    }
    if( m_pipeThread )
    {
        m_pipeThread->HandlePipeLine(CPipeLineThread::EStop , "",0);
    }

   // LOG_CRITICAL <<"CAppPandoraBase::stopPipe() OUT"<< LOG_ENDL;
}

/**
 *******************************************************************
 * This slot informs deletion of pipethread.
 * @param   None
 *
 * @return  void
 *
 *******************************************************************
 */
 void CAppPandoraBase::Notified()
{
     LOG_CRITICAL<<"********CAppPandoraBase::Notified()********"<<LOG_ENDL;
    if( m_pipeThread != NULL) {
        m_pipeThread->deleteLater();
        m_pipeThread = NULL;
    }
}

void CAppPandoraBase::OnSyncWithIPod()
{
    LOG_SLOT<<"CAppPandoraBase::OnSyncWithIPod(), is calling state : " <<IsCalling() <<LOG_ENDL;

    if(bAV_Mode_Changed == false && mPandora){
        startPipeline(mPandora->port());

        //LOG_LOW <<"App is on Foreground , is calling state : "<<IsCalling()<<LOG_ENDL;
        if(!IsCalling())
        {
            //{ modified by yongkyun.lee 2014-03-11 for : BT streaming Mute-play control
            if((m_modeStateHistory.currMode == MODE_STATE_BT_PANDORA) &&
               (m_modeStateHistory.prevMode == MODE_STATE_BT_AUDIO || m_modeStateHistory.prevMode == MODE_STATE_AHA_RADIO_BT ) &&
               (m_modeStateHistory.firstModeChang )  )
            {
                LOG_SLOT<<"OnSyncWithIPod : 2000"<<LOG_ENDL;
                QTimer::singleShot(2000, m_Track, SLOT(Play())); 
            }
            else
            {
                LOG_SLOT<<"OnSyncWithIPod : 200"<<LOG_ENDL;
            //} modified by yongkyun.lee 2014-03-11 
                QTimer::singleShot(200, m_Track, SLOT(Play()));
            }
        }
        else
            SetPlayState(1);
    }
}

/**
*******************************************************************
* Tune key handler .
*
* @param  [in] eventId , Event Id .
* @param  [in] aEvent , jog , event payload
*
* @return None
*
*******************************************************************
*/
void CAppPandoraBase::TuneHandler( int eventId, const Event &aEvent)
{
    LOG_SLOT<<"CAppPandoraBase::TuneHandler() " <<LOG_ENDL;

    //{ modified by yongkyun.lee 2014-12-19 for : ITS 254790 // ITS 228615
    //if(m_bLoadingPopupVisible)
    //    return;
    //} modified by yongkyun.lee 2014-12-19 
    //if(mPandora != NULL && SUCCESS == (mPandora->State()&SUCCESS))
    LOG_SLOT<<"isInsufficientnetwork: " << isInsufficientNetwork << ", IsSystemPopupVisible:" << IsSystemPopupVisible() << LOG_ENDL;
    //{ modified by yongkyun.lee 2014-03-07 for : ITS 228334
    if(mPandora != NULL && SUCCESS == (mPandora->State()&SUCCESS) && (!isInsufficientNetwork))
    //if(mPandora != NULL && SUCCESS == (mPandora->State()&SUCCESS) && (!isInsufficientNetwork)  // modified by esjang 2013.11.08 for audit issue
    //        && (!IsSystemPopupVisible()) )//added jyjeon 2014.01.14 for ITS 218708
    //} modified by yongkyun.lee 2014-03-07 
    {
        switch ( eventId )
        {
            case EVT_KEY_HU_JOG_DIAL_ENCODER:
            LOG_SLOT<<"Tune EVT_KEY_HU_JOG_DIAL_ENCODER"<<LOG_ENDL;
            //{ modified by yongkyun.lee 2014-11-27 for : ITS 253299
            if(m_OSDFlag)
                m_OSDFlag = false;
            //} modified by yongkyun.lee 2014-11-27 
            if(m_Track->GetCurrentTrackInfo() == NULL ) 
            {
                m_pNotifier->DisplayOSD(CExternalNotifier::EStationDoesNotExist);
            }            
             if( ( m_eViewState == EStationListView && isPopupVisibility() == true )|| m_eHMIState == CAppPandoraBase::AppInBG || m_eViewState == ESearchView || m_eViewState == EExplainationView) //modified by wonseok.heo for ITS 249431 20141002
            {
                m_StationList->GetStationName(aEvent.GetCommand() > 0);
                // Display OSD
                m_tuneSelectTimer->start();
            }
            else if(m_eHMIState == CAppPandoraBase::AppInFG ) // modified by esjang 2013.08.21 for BT phone call
            {
                m_tuneSelectTimer->start();
                emit tuneKeyDialed(aEvent.GetCommand() > 0);
            }
            break;
            case EVT_KEY_HU_TUNE_PRESSED:
             LOG_SLOT<<"Tune EVT_KEY_HU_TUNE_PRESSED -> m_eHMIState="<<m_eHMIState<<LOG_ENDL;

             isSeek = true; //add by wonseok.heo for OSD at BG 2015.02.04

             //{ modified by wonseok.heo for ITS 249431 added by wonseok.heo for ITS 237024 2014.05.15
//             if(m_eViewState == EStationListView && IsSystemPopupVisible() == true)
//                 return;
             //} modified by wonseok.heo for ITS 249431  added by wonseok.heo for ITS 237024 2014.05.15


             if( !aEvent.Data().isEmpty()) // modified by esjang 2013.08.21 for BT phone call
             {
                 KEY_EVENT_DATA_T *pData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
                 // added by jyjeon 2014.01.20 for ITS 218714
                 if( (KEY_STATUS_T)pData->keyStatus == KEY_STATUS_PRESSED )
                 {
                     //{ modified by yongkyun.lee 2014-06-13 for :
                     if(m_StationList->GetTotalStationsCount() <=0 )
                         m_StationList->GetStationCount();//{ modified by yongkyun.lee 2014-06-18 for : ITS 240645
                     //} modified by yongkyun.lee 2014-06-13

                     //} modified by wonseok.heo for ITS 250271
                     if(m_eHMIState != CAppPandoraBase::AppInBG )//{ modified by yongkyun.lee 2014-08-28 for :
                     {
                         if(m_eViewState == EStationListView && IsSystemPopupVisible() == true){
                             return;
                         }else{
                             emit tuneCenterPressed();
                         }
                     }//} modified by wonseok.heo for ITS 250271

                     LOG_SLOT<<" Tune EVT_KEY_HU_TUNE_PRESSED -> m_eHMIState="<<m_eHMIState<<LOG_ENDL;
                     m_OSDFlag = true;//{ modified by yongkyun.lee 2014-04-06 for : ITS 233641
                     //LOG_SLOT<<"Tune EVT_KEY_HU_TUNE_PRESSED -> m_eHMIState="<<m_eHMIState<<LOG_ENDL;
                 }
                 else 
                 // added by jyjeon 2014.01.20 for ITS 218714
                 if ( (KEY_STATUS_T)pData->keyStatus == KEY_STATUS_RELEASED )
                 {
                     if(( m_eViewState == EStationListView && isPopupVisibility() == true )|| m_eHMIState == CAppPandoraBase::AppInBG || m_eViewState == ESearchView || m_eViewState == EExplainationView) //modified by wonseok.heo for ITS 249431 20141002
                     {
                         if(m_tuneSelectTimer->isActive())
                         {
                             //select the station
                             m_StationList->TuneSelect();
                             m_tuneSelectTimer->stop();
                             emit tuneOff();
                             IfMute_SentAudioPass(); //added by wonseok.heo for ITS 238505 2014.05.27
                         }
                         else{
                             m_StationList->TuneOff();
                         }
                     }
                     else if(m_eHMIState == CAppPandoraBase::AppInFG)
                     {
                         //if(m_tuneSelectTimer->isActive())//{ modified by yongkyun.lee 2014-02-20 for : ITS 226030
                         {
                             //select the station
                             emit tuneCenterReleased(); //emit tuneCenterPressed(); //modified by jyjeon 2014.01.20 for ITS 218714
                         }
                     }
                 }
             }

            break;

        default:
            break;
        }
    }
}

/**
 *******************************************************************
 * This Method can be used for tune timer off and called from qml to make timer off
 * @param inStatus , True : Stop , False : Start
 *
 * @return  void
 *
 *******************************************************************
 */
void CAppPandoraBase::TuneSelectTimer(bool inStatus)
{
    if(inStatus)
        m_tuneSelectTimer->start();
    else
        m_tuneSelectTimer->stop();
}

/**
 *******************************************************************
 * This slot is binded to the m_tuneSelectTimer and is called after a
 * 5 seconds after the last touch of the tune wheel.
 *
 * @param[in] None
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraBase::onTuneSelectTimer()
{
    if(m_tuneSelectTimer->isActive())
    {
        m_tuneSelectTimer->stop();
    }
    emit tuneOff();
    
    if(!m_bIsBackGround) //added jyjeon 2014.01.10 for ITS 218779
    {
        m_StationList->TuneOff();
    }

}

/**
 *******************************************************************
 * This slot is binded to the m_noSkipTimer and is called after a
 * 5 seconds after the last press seek up key.
 *
 * @param[in] None
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraBase::onNoSkipTimer() // modified by esjang 2013.08.26 for ITS #186175
{
    if(m_noSkipTimer->isActive())
    {
        m_noSkipTimer->stop();
    }
}


//{ modified by yongkyun.lee 2014-03-11 for : BT streaming Mute-play control
void CAppPandoraBase::startPlayMuteTimer()
{
    LOG_HIGH<<"CAppPandoraBase::startPlayMuteTimer : getModeStateHistoryFMode()=" << getModeStateHistoryFMode() << LOG_ENDL;
    if(getModeStateHistoryFMode())
        m_PlayMuteTimer->start();
}

void CAppPandoraBase::onPlayMuteTimer()
{
    LOG_HIGH<<"CAppPandoraBase::onPlayMuteTimer" << LOG_ENDL;
    m_PlayMuteTimer->stop();
    setUnMuteForBTStream();
}
//} modified by yongkyun.lee 2014-03-05 


//{ added by cheolhwan.bae 2014-04-15. Add Debug Macro.
void CAppPandoraBase::printQMLLog(QString str)
{
    LOG_HIGH << "[QML] " << str.toUtf8().data() << LOG_ENDL;
}
//} added by cheolhwan.bae 2014-04-15. Add Debug Macro.


/**
 *******************************************************************
 * This methode will return the string length based on param
 * 5 seconds after the last touch of the tune wheel.
 *
 * @param[in] fullString , actual String
 * @param[in] fontFamily , Font
 * @param[in] pixelSize , Pixel size
 *
 * @return  int , Length of string
 *
 *******************************************************************
 */
int CAppPandoraBase::getStringWidth( const QString &fullString,
                                                      const QString &fontFamily,
                                                      int pixelSize )
{
    QFont mFont;
    mFont.setFamily(fontFamily);
    mFont.setPointSize(pixelSize);
    QFontMetrics fontMetrics(mFont);
    return fontMetrics.width(fullString, -1);
}

void CAppPandoraBase::playInBackground(MODE_STATE_T mode)
{
    if(mPandora)
        LOG_HIGH<<"CAppPandoraBase::playInBG  mode = "<<(int)mode << " curr dev = " << m_eCurrentDevice << "  mPandora->State() = " << mPandora->State() << LOG_ENDL;
    else
        LOG_HIGH<<"CAppPandoraBase::playInBG  mode = "<<(int)mode << " curr dev = " << m_eCurrentDevice << LOG_ENDL;

    DeviceInfo::EConnectionType type = DeviceInfo::ENoDevice;
    m_connect_type_name = "";
    switch(mode)
    {
        case MODE_STATE_BT_PANDORA:
            if(mBTPandora == NULL)
            {
                mBTPandora = new DeviceInfo(DeviceInfo::EBTConn );
            }
            type = DeviceInfo::EBTConn;
            m_connect_type_name =  qApp->translate("main","STR_PANDORA_BLUETOOTH"); //qApp->translate("main","DevName"); //modified by jyjeon 2014.01.15 for ITS 219728
        break;

        case MODE_STATE_IPOD1_PANDORA:
            if(mPandoraF == NULL)
            {
                mPandoraF = new DeviceInfo(DeviceInfo::EUSBPort1,eUSB_PORT1);
            }
            type = DeviceInfo::EUSBPort1;
            if(GetVariantRearUSB())
            {
                m_connect_type_name =  qApp->translate("main","STR_PANDORA_FRONT"); // added by esjang 2013.07.08 for static test CID 22885, 22886
            }
        break;

        case MODE_STATE_IPOD2_PANDORA:
            if(mPandoraR == NULL)
            {
                mPandoraR = new DeviceInfo(DeviceInfo::EUSBPort2,eUSB_PORT2);
            }
            type = DeviceInfo::EUSBPort2;
            if(GetVariantRearUSB())
            {
                m_connect_type_name =  qApp->translate("main","STR_PANDORA_REAR"); // added by esjang 2013.07.08 for static test CID 22885, 22886
            }
        break;

        default:
         LOG_CRITICAL<<"CAppPandoraBase::playInBG No device" << LOG_ENDL;
        break;
    }


    m_eHMIState = CAppPandoraBase::AppInBG;
    m_bIsBackGround = true;
    m_pNotifier->ClearOSD(); // added by esjang 2013.03.21 for OSD Update

    if(m_eCurrentDevice == DeviceInfo::ENoDevice)//No device mounted yet
    {
        m_eCurrentDevice = type;
        SetDevice();// device changed
        LOG_HIGH<<" playInBackground() 1. mPandora->State()="<< mPandora->State()<<LOG_ENDL;
        mPandora->State()|=CURR;
        //startPipeline(port);
        m_Track->RetriveState(mPandora);
        m_StationList->RetriveState(mPandora);
        emit deviceChanged();
    }
    else if(m_eCurrentDevice != type)
    {
        stopPipe();
        if(m_pSignals)
        {
            QObject::disconnect(m_pSignals,0,this,0);
           // SetConnection(NULL);
            //m_pSignals = NULL;
        }
        if(CONNECT != (mPandora->State()&CONNECT) )
        {
            LOG_HIGH<<" playInBackground() 2. mPandora->State() init="<<mPandora->State()<<LOG_ENDL;
            Disconnect();
            mPandora->State()=0;
        }
        else
        {
            if(mPandora) //added by jyjeon 2014-05-28 for static test
            {
                LOG_HIGH<<" playInBackground) 3. mPandora->State()="<< mPandora->State()<<LOG_ENDL;
                mPandora->State()^=CURR;

                m_Track->SaveState(mPandora);
                m_StationList->SaveState(mPandora);
            }

            if(m_Track->TrackStatus() == 1){
                m_Track->Pause();
            }
            ClearCache();
            SetConnection(NULL);
        }

        //IMP : Before set device , change the current device type.
        m_eCurrentDevice = type;
        SetDevice(); // device changed
        if(mPandora) //added by jyjeon 2014-05-28 for static test
        {
            mPandora->State()|=CURR;
            m_Track->RetriveState(mPandora);
            m_StationList->RetriveState(mPandora);
            emit deviceChanged();
        }
    }
    emit playInBG();
}

void CAppPandoraBase::IfMute_SentAudioPass()
{
    LOG_HIGH<<"IfMute_SentAudioPass -> m_isMuteState["<<m_isMuteState<<"]"<<LOG_ENDL;
    if(m_isMuteState)
    {
        m_isMuteState = false ;
        NotifyAudioPathChanged(m_eCurrentDevice);
    }
}

bool CAppPandoraBase::IsException()
{
    bool exception = false;
    if(( m_eViewState == EStationListView && isPopupVisibility() == true )|| m_eViewState == ESearchView || m_eViewState == EExplainationView ) //modified by wonseok.heo for ITS 249431 20141002
        exception = true;
    return exception;
}

void CAppPandoraBase::setCurrentView(int inCurrentView)
{
    LOG_HIGH<<"View Changed From  : "<<m_eViewState << " -> To : " << inCurrentView<< "[ Time = "<< (QTime::currentTime()).toString("hh.mm.ss.zzz")<<"]"<<LOG_ENDL;
    if(inCurrentView != m_eViewState)
    {
        TuneSelectTimer(false);
    }
    m_eViewState = (ActiveView) inCurrentView;
}

/**
 *******************************************************************
 * Audio Sample rate changed.
 * @param inSampleRate : New sample rate
 *
 * @return  void
 *
 *******************************************************************
 */
void CAppPandoraBase::onAudioSampleRateChanged(int inSampleRate)
{
    if( m_bPandoraConnected && !bAV_Mode_Changed)
    {
        if(m_pipeThread && mPandora){
            m_pipeThread->SetSourceType(mPandora->port());
            m_pipeThread->HandlePipeLine(CPipeLineThread::EReSet , m_DeviceSrc,inSampleRate);
        }
    }
}

/**
 *******************************************************************
 * SetShareDataProvider
 * @param bPlay : true[play] / false[pause]
 *
 * @return  void
 *
 *******************************************************************
 */
void CAppPandoraBase::SetShareDataProvider(bool bPlay)
{
    m_pShareDataProvider->SetStatusDateByIndex(DATA_TYPE_PLAY_STATUS, bPlay);
}

//{ modified by yongkyun.lee 2014-03-11 for : BT streaming Mute-play control
MODE_STATE_T CAppPandoraBase::getModeStateHistoryPrev()
{
    return m_modeStateHistory.prevMode ;
}

MODE_STATE_T CAppPandoraBase::getModeStateHistoryCurr()
{
    return m_modeStateHistory.currMode ;
}
bool CAppPandoraBase::getModeStateHistoryFMode()
{
    return m_modeStateHistory.firstModeChang ;
}
void CAppPandoraBase::setModeStateHistoryFMode(bool val)
{
    LOG_HIGH << "setModeStateHistoryFMode 1: val =" << val << LOG_ENDL;
    m_modeStateHistory.firstModeChang = val;
}
//} modified by yongkyun.lee 2014-03-11 


//{ modified by yongkyun.lee 2014-04-06 for : ITS 233641
void CAppPandoraBase::TrackInfoOSD()
{
    LOG_HIGH << "TrackInfoOSD: m_bPandoraConnected =" << m_bPandoraConnected << " , m_eCurrentDevice=" << m_eCurrentDevice << LOG_ENDL;

    //{ modified by yongkyun.lee 2014-11-03 for : //leeyk1 OSD TEST
    if( (m_eViewState == ETrackView && !IsRunningInBG()) || ( ( m_eViewState == EStationListView && isPopupVisibility() == true ))) // modified by wonseok.heo for ITS 262629
    {
        LOG_LOW << "TrackInfoOSD return...: m_eViewState =" << m_eViewState << " , IsRunningInBG()=" << IsRunningInBG() << LOG_ENDL;
        return;
    }

    if(IsRunningInBG() && !isSeek){
        LOG_HIGH << "TrackInfoOSD: m_bPandoraConnected =" << m_bPandoraConnected << " , m_eCurrentDevice=" << m_eCurrentDevice <<"isSeek"<<isSeek<< LOG_ENDL;

        return;
    }



    //} modified by yongkyun.lee 2014-11-03 
    //{ modified by yongkyun.lee 2014-09-11 for : ITS 248198
    if(m_NextTimeOSD)
    {
        m_NextTimeOSD= false;
        m_OSDFlag = true;
        LOG_LOW << "TrackInfoOSD: return : m_NextTimeOSD =" << m_NextTimeOSD   << LOG_ENDL;       
        return;
    }
    else
    //} modified by yongkyun.lee 2014-09-12 
    {
     if(!m_OSDFlag)
        return;
    }
     m_OSDFlag = false;
     if(m_bPandoraConnected)
         m_pNotifier->DisplayOSD(CExternalNotifier::ETrackInfo);
     else
         m_pNotifier->DisplayOSD(CExternalNotifier::ENoTrackInfo);
}
//} modified by yongkyun.lee 2014-04-06 

//{ modified by yongkyun.lee 2014-04-30 for : ITS 236250
void CAppPandoraBase::sendTouchCleanUpForApps()
{
    SendTouchCleanUpForApps ();
}
 //} modified by yongkyun.lee 2014-04-30 

//{ modified by yongkyun.lee 2014-07-10 for : popup OSD 
void CAppPandoraBase::SetSystemPopupVisibility(bool inVisible)
{
    m_bSystemPopupVisible =  inVisible;
    m_pNotifier->ClearAVOSD();    
}

void CAppPandoraBase::SetLocalPopupVisibility(bool inVisible)
{
    m_bLocalPopupVisible =  inVisible;
    m_pNotifier->ClearAVOSD();    
}
//} modified by yongkyun.lee 2014-07-10 


//{ modified by yongkyun.lee 2014-07-17 for : ITS 243240
void CAppPandoraBase::OSD(int osdID)
{
    m_pNotifier->DisplayOSD((CExternalNotifier::HandleOSDEvent)osdID);
}
//} modified by yongkyun.lee 2014-07-17 

//{ modified by yongkyun.lee 2014-09-11 for : 
void CAppPandoraBase::OnNextTimeOSD()
{
    LOG_LOW << "OnNextTimeOSD :: TrackInfoOSD: IsRunningInBG()"  << IsRunningInBG()<< LOG_ENDL;
    if(IsRunningInBG())//{ modified by yongkyun.lee 2014-11-03 for : //leeyk1 OSD TEST
        m_NextTimeOSD = true;
}

//} modified by yongkyun.lee 2014-09-11 

//End Of File
