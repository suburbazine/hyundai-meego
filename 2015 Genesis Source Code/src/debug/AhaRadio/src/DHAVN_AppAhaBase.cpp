#include "DHAVN_AppAhaBase.h"

#ifdef FRAMEWORK_ENABLED
#include <DHAVN_AppFramework_Event_Def.h>
#include <uistate_manager_def.h>
#include <DHAVN_AppSettings_Shared.h>
#include <DHAVN_AppHomeScreen_Def.h>
#endif

#include <QGraphicsObject>
//#include <QDeclarativeEngine>
//#include <QDeclarativeComponent>
#include <qdeclarativeprivate.h>
#include <QtDeclarative/QDeclarativeContext>
#include <osdwriter.h> //hsryu_0508_QmlStatusBar
#include <stdio.h>
#include <QFile>

#include "DHAVN_AppAhaStationList.h"
#include "DHAVN_AppAha_QmlProperty.h"

const QString DEV_TYPE = "DeviceType";
const QString DEV_NAME = "DeviceName";
/**
 *******************************************************************
 *
 * Constructor for CAppAhaBase module. This will construct
 * other c++ modules in Aha app qt layer.
 *
 * @param[in]   aQmlView    The view using which all screens will be displayed
 * @param[in]   nLanguage   The default language to be used by application
 * @param[in]   pParent     The parent screen for the application.
 *
 * @return     None
 *
 *******************************************************************
 */
#ifdef FRAMEWORK_ENABLED
CAppAhaBase::CAppAhaBase( QDeclarativeView &aQMLView, ECountryVariant variant, LANGUAGE_T nLanguage, int nVehicleType, QObject *pParent )
    : AppEngineQML( &aQMLView, "AppAhaRadio", pParent )
    , m_pTranslator( NULL )
    #ifdef FRAMEWORK_ENABLED
    , m_nCountryVariant( variant )
    , m_nLanguage(nLanguage)
    #endif
    , m_CurrentAhaDevice(DEVICE_TYPE_NONE)
    , m_eHMIState(CAppAhaBase::AppNotLaunched)
    , m_bAhaConnected(false)
    , m_bAV_Mode_Changed(false)
    , m_bIsBackGround(true)
    , m_bOptionMenuVisible(false)
    , m_bCallingState(false) //hsryu_0322_TRANSPORT_CAN
    , m_nPreviousPlayPause(AHA_PLAYBACK_PLAY) //hsryu_0404_restore_playmode
    //, bCallingPlayState(false) //hsryu_0404_restore_playmode
    , bCheckInitOnce(false) //hsryu_0404_restore_playmode
    , isSoundOpen(false)//hsryu_0422_can_test
    , m_bClusterUpdate(false) //hsryu_0603_cluster_update
    , bOSD_DISPLAY(true) //hsryu_0621_ITS_0174694
    , m_isFrontLCD(true)
    , m_nCurrentView(AHA_CURRENT_VIEW_NONE)     // by Ryu 20131001
    , m_bBeforeSoundSetting(false)  //wsuk.kim 131106 temporal mode ON after launch sound setting, did not menu hide.
    , m_bSelectionEvent(false) // Ryu 20140207  List view , Back key --> Track view,  playback state should not changed
    #else
CAppAhaBase::CAppAhaBase(QDeclarativeView &aQMLView, LANGUAGE_T nLanguage, QObject *pParent):
  #endif

{
    new CAppAhaStationList(m_AhaInterface);
    CAppAhaTrack *m_Track = new CAppAhaTrack(m_AhaInterface);

    LOG_INIT_V2("Aha");

    m_SoundInterface.setVehicleType(nVehicleType);

//hsryu_0322_TRANSPORT_CAN
    m_pQCanController = new CQCANController_ApplnTxRx(this);

    //hsryu_0422_can_test
    if (m_pQCanController)
    {
        m_pQCanController->Init();
    }

    if (QDBusConnection::sessionBus().interface()->isServiceRegistered("com.lge.car.micom"))
    {
        m_pCanTPInterface = new QDBusInterface("com.lge.car.micom", "/Dtc", "com.lge.car.micom.dtc", QDBusConnection::sessionBus());
    }
    else
    {
        m_pCanTPInterface = NULL;
    }
//hsryu_0322_TRANSPORT_CAN

    m_b_MMC_removed = false;

    m_pSignals = &(m_AhaInterface.GetSignalProvider());

    connect(m_pSignals,SIGNAL(Result(const ERequestType, const EAhaError)),
            this, SLOT(OnRequestCompleted(const ERequestType, const EAhaError)) , Qt::QueuedConnection);
    connect(m_pSignals,SIGNAL(ahaSampleRateChanged(int)), this , SLOT(ahaSampleRateChangedSlot(int)));
    connect(m_pSignals, SIGNAL(NoActiveStation()), this, SLOT(OnNoActiveStationSlot()));
    connect(m_pSignals, SIGNAL(OSDInfoReady(QString&)), this, SLOT(OSDInfoReadySlot(QString&)));
    connect(m_Track, SIGNAL(trackInfoReceived()), this, SLOT(NotifyAVWidget()));
    //hsryu_0322_TRANSPORT_CAN
    connect(m_Track, SIGNAL(trackTitleReceived(QString)), this, SLOT(ontrackTitleReceived(QString)));
    //hsryu_0422_can_test
    connect(m_Track, SIGNAL(clearTpMessage()), this, SLOT(onClearTpMessage()));
//wsuk.kim no_network
    connect(m_pSignals, SIGNAL(NetworkResume()), this, SLOT(onNetworkResume()));
    connect(m_pSignals, SIGNAL(NetworkStatus(const int)), this, SLOT(onNetworkStatus(const int)));
    connect(m_pSignals, SIGNAL(unmute()), this, SLOT(onUnmute()) );
//wsuk.kim no_network
#ifndef FRAMEWORK_ENABLED
    Q_UNUSED(nLanguage)
    Q_UNUSED(pParent)
#endif
    m_isJogKeyLongPressed = 0; /*wsuk.kim jog_long_press*/
    m_bSetOSD = false;
    //hsryu_0423_block_play_btcall
    //m_bConnectCall = false; //wsuk.kim deactivate_call
    m_bMuteState = false;   //wsuk.kim UNMUTE
    m_connect_type_name = "";
    m_bDRSTextScroll = false;   //wsuk.kim text_scroll
    m_NetworkStatusCode = AHA_NOTIFY_DISCONNECTED;   //wsuk.kim no_network
    m_bShowNoNetworkPopup = false;  //wsuk.kim 131204 ITS_212572 state network error, display error info on OSD.
    // for SW WDT
    signalConnection();
    setSignalHandlerObject(this);

    m_bDuplicate = false;
    m_bContinueLastView = false;

    m_PrevAvMode = MODE_STATE_NONE;
    m_CurrentAvMode = MODE_STATE_NONE;
    m_bUnmuteSent = false;
    m_HeadUnitPowerState = HEADUNIT_POWER_ON;

    m_DelayedPlayTimer = new QTimer(this);
    if(m_DelayedPlayTimer)
    {
        m_DelayedPlayTimer->setSingleShot(true);
        connect(m_DelayedPlayTimer, SIGNAL(timeout()), this, SLOT(onDelayedPlayTimeOut()));
    }
    /*
    LOG_HIGH <<"GetSystemVariant = %d \n", AppEngineBase::GetSystemVariant() <<LOG_ENDL;

    if(AppEngineBase::GetSystemVariant() & 0x00008000 )
    {
        LOG_HIGH <<"Rear USB is TRUE \n"<<LOG_ENDL;
    }
    else
    {
        LOG_HIGH <<"Rear USB is FALSE \n"<<LOG_ENDL;
    }
*/

    // not use auto response
    //m_AppHandlesResponseFg = true;
}

// for SW WDT -->

void CAppAhaBase::signalConnection()
{
    struct sigaction act;
    memset(&act, 0x00, sizeof(struct sigaction));

    act.sa_flags = 0;
    act.sa_flags |= SA_RESETHAND;
    act.sa_handler = CAppAhaBase::signalHandler;

    for ( int signumber = SIGHUP; signumber < SIGUNUSED; signumber++ )
    {
        if ( signumber == SIGKILL || signumber == SIGSTOP || signumber == SIGCHLD ) continue;

        if ( sigaction(signumber, &act, (struct sigaction*)NULL) < 0 )
        {
            LOG_HIGH<<"[CAppAhaBase::signalConnection] error "<<LOG_ENDL;
            //exit(1);
        }
    }
}

void CAppAhaBase::signalHandler (int signum)
{
    SigM->restart(signum);
    raise(signum);
}

void CAppAhaBase::setSignalHandlerObject (CAppAhaBase* m)
{
    SigM = m;
}

void CAppAhaBase::restart(int signum)
{
    Event event ( EVT_REQUEST_ABNORMAL_TERMINATION, GetThisAppId(), APP_UI_MANAGER, (EVT_COMMAND_T) signum);
    NotifyUISH (event, DISPLAY_FRONT);
    //exit(0);
}

void CAppAhaBase::printQMLDebugString(QString str)
{
    //LOG_HIGH <<"[QML] %s", str.toUtf8().data());
    LOG_HIGH << "[QML] " << str.toUtf8().data() << LOG_ENDL;
}

void CAppAhaBase::CallPOI()
{
    LOG_HIGH<< "CAppAhaBase::CallPOI" <<LOG_ENDL;

    QString strRawNumber;
    QString strPhoneNumber;
    QByteArray byteArray;
    QByteArray byteArrayPhoneNumber;
    int nPhoneByteLength;

    EVENT_EVT_NAVI_COMMAND_DATA_T* navicommanddata;

    bool bCanCall;

    bCanCall = m_AhaInterface.queryPhoneNumber(strRawNumber);
    strPhoneNumber.clear();
    for(int cnt = 0 ; cnt < strRawNumber.count(); cnt++)
    {
        if(strRawNumber.at(cnt).isDigit() || strRawNumber.at(cnt) == QChar('+') ||
                strRawNumber.at(cnt) == QChar('*') || strRawNumber.at(cnt) == QChar('#'))
        {
            strPhoneNumber.append(strRawNumber.at(cnt));
        }
    }

    if(bCanCall)
    {
        m_AhaInterface.logCall();
        m_AhaInterface.pause();
        //strPhoneNumber.clear();
        //strPhoneNumber = QString("114");
        byteArrayPhoneNumber = strPhoneNumber.toAscii(); //convert to bytearray
        nPhoneByteLength = byteArrayPhoneNumber.length() + 1;
        int totalsize = sizeof(EVENT_EVT_NAVI_COMMAND_DATA_T) + nPhoneByteLength;

        navicommanddata = (EVENT_EVT_NAVI_COMMAND_DATA_T*) malloc(totalsize);


        navicommanddata->header.appIdReciever = APP_BTPHONE;
        navicommanddata->header.reasonEvent = EVT_REQUEST_FG;
        navicommanddata->header.dataSize = totalsize;
        navicommanddata->header.sIsJogEnabled = false;
        navicommanddata->commandId = NAVI_BT_OUTGOING_CALL_INPUTNUM;
        navicommanddata->sNAVIPayload.iLength = nPhoneByteLength;
        //allocate memory
        memcpy(navicommanddata->sNAVIPayload.data, (char*) byteArrayPhoneNumber.data(), nPhoneByteLength);

        byteArray.setRawData((const char*)navicommanddata, totalsize);

        Event event( EVT_NAVI_COMMAND,
                     GetThisAppId(), APP_UI_MANAGER,
                     NAVI_BT_OUTGOING_CALL_INPUTNUM,
                     byteArray);
        //AppEngineBase::NotifyUISH( event );
        if ( !NotifyUISHCommon(event) )
        {
            LOG_CRITICAL << "CAppAhaBase::CallPOI: Event notification failed" << LOG_ENDL;
        }
        else
        {
            LOG_HIGH<< "CAppAhaBase::CallPOI: Event notification sent" <<LOG_ENDL;
        }

        //free memory
        free(navicommanddata);
    }
}

//hsryu_0312_navi_exception
void CAppAhaBase::NavigateToPOI()
{
    LOG_HIGH<< "CAppAhaBase::NavigateToPOI" <<LOG_ENDL;

//    AHA_GPS_STATUS_T currentAhaGPSStatus;
    double dbLon, dbLat;
    QString strName, strStateName, strStreet, strCity, strZipCode, strPhoneNumber;


    bool nCanNavigate;

//    currentAhaGPSStatus = GetAhaGPSStatus();
//    switch(currentAhaGPSStatus)
//    {
//    case AHA_GPS_NO_NAVI:
//        TAN_PRINTF("AHA_GPS_NO_NAVI\n");
//        break;
//    case AHA_GPS_WEAK:
//        TAN_PRINTF("AHA_GPS_WEAK\n");
//        break;
//    case AHA_GPS_OK:
//        TAN_PRINTF("AHA_GPS_OK\n");
//        break;
//    default:
//        TAN_PRINTF("currentNaviGPSStatus: %d\n", currentAhaGPSStatus);
//        break;
//    }


    nCanNavigate = m_AhaInterface.queryAhaLBSValue(strName,
                                                   strStateName,
                                                   strStreet,
                                                   strCity,
                                                   strZipCode,
                                                   strPhoneNumber,
                                                   dbLon, dbLat);
    //ok let us send a signal via dbus to notify the navigation app.
    if(nCanNavigate)
    {

        m_AhaInterface.logNavigate();
        m_AhaInterface.pause();
        m_AhaInterface.reqSendContentInfoToNavigation(strName,
                                                      strStateName,
                                                      strStreet,
                                                      strCity,
                                                      strZipCode,
                                                      strPhoneNumber,
                                                      dbLon, dbLat);
        Event event( EVT_XM_REQUEST_NAVI, GetThisAppId(), APP_UI_MANAGER);
        //event.SetCommand((NAVI_ENTRY_POINT_T)NAVI_ENTRY_POINT_MOVIE_TIMES);
        event.SetCommand((NAVI_ENTRY_POINT_T)NAVI_ENTRY_POINT_LBS);     // by Ryu 20130712
        //AppEngineBase::NotifyUISH( event );
        //NotifyUISH( event,DISPLAY_FRONT );
        if ( !NotifyUISHCommon(event) )
        {
            LOG_CRITICAL << "CAppAhaBase::NavigateToPOI: Event notification failed" << LOG_ENDL;
        }
        else
        {
            LOG_HIGH<< "CAppAhaBase::NavigateToPOI: Event notification sent" <<LOG_ENDL;
        }
    }
}

//hsryu_0514_check_latitude_longitude_for_aha
bool CAppAhaBase::CheckLocateForNavigation()
{
    double dbLon, dbLat;
    QString strName, strStateName, strStreet, strCity, strZipCode, strPhoneNumber;

    m_AhaInterface.queryAhaLBSValue(strName,
                                       strStateName,
                                       strStreet,
                                       strCity,
                                       strZipCode,
                                       strPhoneNumber,
                                       dbLon, dbLat);
    bool rtnVal = false;
    if(isLocateInLatitude(dbLat) && isLocateInLongitude(dbLon))
    {
        rtnVal = true;
    }

    return rtnVal;
}


bool CAppAhaBase::isLocateInLatitude(double latitude)
{
    if(latitude < 91.0000 && latitude > 6.0000)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CAppAhaBase::isLocateInLongitude(double longitude)
{
    if(longitude < -47.0000 && longitude > -172.0000)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//hsryu_0514_check_latitude_longitude_for_aha

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
CAppAhaBase::~CAppAhaBase()
{
//hsryu_0322_TRANSPORT_CAN
    if ( m_pQCanController != NULL )
    {
        delete m_pQCanController;
    }
    if ( m_pCanTPInterface != NULL )
    {
        delete m_pCanTPInterface;
    }
//hsryu_0322_TRANSPORT_CAN
    if ( m_pTranslator != NULL )
    {
        delete m_pTranslator;
    }
    if( m_DelayedPlayTimer != NULL )
    {
        delete m_DelayedPlayTimer;
    }
}

//hsryu_0322_TRANSPORT_CAN
/**
 *******************************************************************
 * ontrackTitleReceived
 *
 *******************************************************************
 */
void CAppAhaBase::ontrackTitleReceived(QString title)
{
    sendClearTpMessage();

    //hsryu_0422_can_test
    SendClusterData();
    sendCanTpMessage(0x48F , title);
}

//hsryu_0422_can_test
/**
 *******************************************************************
 * onClearTpMessage
 *
 *******************************************************************
 */
void CAppAhaBase::onClearTpMessage()
{
    SendClusterData();
    sendCanTpMessage(0x48F , "");
}

/**
 *******************************************************************
 * SendClusterData
 *
 *******************************************************************
 */
//hsryu_0422_can_test
void CAppAhaBase::SendClusterData()
{
    //LOG_HIGH << "CExternalNotifier::SendClusterData() <--" << LOG_ENDL;

    //hsryu_0603_cluster_update
    if(IsClusterUpdate() && !IsCallingSameDevice())
    {
        quint64 data = 0;

        // HU NAVI ONOFF (first 1bit)
        // HU_OpState (7 bits)
        data = 0x10; //aha HU_OpState

        // HU_Tuner_Area (3bits)
        data <<= 3;
        data += 0x7;

        // HU_Preset_Number (5bits)
        data <<= 5;
        data += 0x1f;

        // HU_Track_Number (16bits)
        data <<= 16;
        data += 0xffff;

        // HU_Play_time_Sec (6bits)
        data <<= 6;
        data += 0x3f;

        // HU_Play_time_Min (7bits)
        data <<= 7;
        data += 0x7f;

        // HU_Play_time_Hour (6bits)
        data <<= 6;
        data += 0x3f;

        // HU_Frequency (9bits)
        data <<= 9;
        data += 0x1ff;

        // HU_Disc_select_No (4bits)
        data <<= 4;
        data += 0xf;

        SQCanFrame frame = {0x114, 0x08, {0,0,0,0,0,0,0,0}};
        data = qToBigEndian(data);
        memcpy(&frame.uqcandata, &data, sizeof(quint64));
        emit Can_data_send(frame);
    }
}

/**
 *******************************************************************
 * SendClusterData_OpStateOnly
 *
 *******************************************************************
 */
void CAppAhaBase::SendClusterData_OpStateOnly()
{
    //hsryu_0422_can_test
    SendClusterData();
}

/**
 *******************************************************************
 * sendCanTpMessage
 *
 *******************************************************************
 */
void CAppAhaBase::sendCanTpMessage(quint16 id, QString data)
{
    //TAN_PRINTF("##sendCanTpMessage##: %s %d %d \n\n", data.toAscii().data(),IsClusterUpdate(), m_bCallingState);

    //hsryu_0603_cluster_update
    if(IsClusterUpdate() && !IsCallingSameDevice())
    {
        QDBusReply<bool> reply;

        QByteArray buffer;
        buffer.resize(100);

        buffer.clear();
        QByteArray arrayStationName = data.toLocal8Bit();
        quint8 nLength2 = data.size();

        buffer[0] = (id >> 8);
        buffer[1] = (id & 0xff);

        if(id == 0x48F) // 0x48F : IPOD, Pandora, Aha
        {
            if(data == "")
            {
                buffer[2] = 3; // opcode + clear TP
                buffer[4] = 0x0; // clear TP
                buffer[5] = 0x0;
            }
            else
            {
                buffer[2] = nLength2 * 2 + 1; // for opcode
            }

            if(id == 0x48F)
            {
                buffer[3] = 0x3; // opcode : 0x3
            }
        }
        else
        {
            if(data == "")
            {
                //hsryu_0603_cluster_update
//                buffer[2] = 1;      // length
//                buffer[3] = 0x0;    // clear TP

                buffer[2] = 2;      // length
                buffer[3] = 0x0;    // clear TP
                buffer[4] = 0x0;    // clear TP
            }
            else
            {
                buffer[2] = nLength2 * 2;
            }
        }

        if(data != "")
        {
            QByteArray tpbody((const char*) (data.utf16()), data.size() * 2);
            buffer.append(tpbody);
        }

        //{added by juname 2012.10.24 for crash protection
        if(m_pCanTPInterface == NULL)
        {
            if(QDBusConnection::sessionBus().interface()->isServiceRegistered("com.lge.car.micom"))
            {
                m_pCanTPInterface = new QDBusInterface("com.lge.car.micom", "/Dtc", "com.lge.car.micom.dtc", QDBusConnection::sessionBus());
            }
            else
            {
                LOG_HIGH << "CExternalNotifier::sendCanTpMessage() Service com.lge.car.micom is not registered" << LOG_ENDL;
            }
        }

        if(m_pCanTPInterface && m_pCanTPInterface->isValid()) //} added by junam
        {
            reply = m_pCanTPInterface->call("LgeDBus_CANTPSendMsg", buffer);
        }

        LOG_HIGH << "CExternalNotifier::sendCanTpMessage() -->" << LOG_ENDL;// Modified by esjang 2013.02.28 for Static Test #17531
        //LOG_HIGH <<"##sendCanTpMessage##: %d \n", (bool)reply);
        //return reply;
    }
}
//hsryu_0322_TRANSPORT_CAN

//hsryu_0422_can_test
void CAppAhaBase::sendClearTpMessage()
{
    SendClusterData();
    sendCanTpMessage(0x48F, "");
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

void CAppAhaBase::ClearCache()
{
    LOG_TRACE<<"CAppAhaBase::ClearCache() <-"<<LOG_ENDL;
    if( DEVICE_TYPE_SMARTPHONE != m_CurrentAhaDevice )
    {
        m_SoundInterface.closeSoundChannel();
    }

    //hsryu_0613_fail_load_station
    CAppAhaStationList::getInstance()->initialize();
    CAppAhaQmlProperty::getInstance()->initialize();
    CAppAhaTrack::getInstance()->initialize();

    m_CurrentAhaDevice = DEVICE_TYPE_NONE;
    m_NetworkStatusCode = AHA_NOTIFY_DISCONNECTED;   //wsuk.kim no_network
    m_AhaInterface.DeInit();

    //hsryu_0422_can_test
    isSoundOpen = false;

    //hsryu_0404_restore_playmode
    CAppAhaTrack::getInstance()->ClearCache();

    //LOG_TRACE<<"CAppAhaBase::ClearCache() ->"<<LOG_ENDL;
}

//hsryu_0603_cluster_update
void CAppAhaBase::setClusterUpdate(bool value)
{
    m_bClusterUpdate = value;
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
void CAppAhaBase::ExitApplication()
{
#ifdef FRAMEWORK_ENABLED
    LOG_HIGH<<" CAppAhaBase::ExitApplication() <-"<<LOG_ENDL;

    StopNotifyAVWidget();
    UpdateAhaAvailableStatus(false); // user pressed back key .

    Event event( EVT_KEY_SOFT_BACK, GetThisAppId(), APP_NONE );
    m_eHMIState = CAppAhaBase::AppExitState;
    // Deinitialize the states
    ClearCache();

    if ( !NotifyUISH(event, DISPLAY_FRONT) )
    {
        LOG_CRITICAL<<"Event notification failed"<<LOG_ENDL;
    }


    LOG_HIGH<<" CAppAhaBase::ExitApplication() ->"<<LOG_ENDL;
#else
    ClearCache();
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
void CAppAhaBase::HandleHomeKey()
{
#ifdef FRAMEWORK_ENABLED
    LOG_TRACE << "CAppAhaBase::HandleHomeKey: Start" << LOG_ENDL;
    m_eHMIState = CAppAhaBase::AppInBG;

    // Send event about pressed EVT_KEY_HOME to UI State Hnadler
    // It will take effect and this application will be switched to Active Screen
    Event event( EVT_KEY_SOFT_HOME, GetThisAppId(), APP_NONE );
    // Notify UISH with EVT_KEY_SOFT_HOME event
    if ( !NotifyUISH(event, DISPLAY_FRONT) )
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
void CAppAhaBase::HandleBackKey()
{
#ifdef FRAMEWORK_ENABLED
    LOG_TRACE << "[[ CAppAhaBase::HandleBackKey ]]: Start" << LOG_ENDL;

    m_eHMIState = CAppAhaBase::AppInBG;

    // Send event about pressed EVT_KEY_SOFT_BACK to UI State Hnadler
    // It will take effect and this application will be switched to Active Screen
    Event event( EVT_KEY_SOFT_BACK, GetThisAppId(), APP_NONE );
    // Notify UISH with EVT_KEY_SOFT_BACK event
    if ( !NotifyUISH(event, DISPLAY_FRONT) )
    {
        LOG_CRITICAL<<"Event notification failed"<<LOG_ENDL;
    }
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
void CAppAhaBase::RetranslateUi()
{
    emit retranslateUi((int)m_nLanguage);
}


AhaMenus& CAppAhaBase::GetAhaMenus()
{
    return m_AhaMenus;
}

/*
//{ modified for CR 14370 by Vijayakumar K
void CAppAhaBase::SetAhaStateActive()
{
    AHARADIO_CONN_STATE_T state = AHARADIO_ACTIVE ;
    QByteArray connStatus;
    connStatus.append(state);
    Event event( EVT_AHA_STATUS, GetThisAppId(), APP_UI_MANAGER , connStatus );
    //{ Modified by esjang 2012.12.22 for SwapDC
    //NotifyUISH( event,DISPLAY_FRONT );
    if ( !NotifyUISHCommon(event) )
    {
        LOG_CRITICAL << "CAppAhaBase::SetAhaStateActive: Event notification failed" << LOG_ENDL;
    }
    else
    {
        LOG_HIGH<< "CAppAhaBase::SetAhaStateActive: Event notification sent" <<LOG_ENDL;
    }
    //} Modified by esjang 2012.12.22 for SwapDC


    // As a default device USBPort1 aquires the audio Path.
    NotifyAudioPathChanged();
}
//} modified for CR 14370 by Vijayakumar K
*/

/**
 *******************************************************************
 * Get the current devices connection status
 *
 * @param   None
 *
 * @return  QStringList list of devices connection status *
 *******************************************************************
 */

bool CAppAhaBase::ReadDataFromNaviShareMemory(unsigned char &posFix)
{
    NMEAGPS nmea_data;
    //QSharedMemory m_SharedMemory;
    m_SharedMemory.setKey(NAVI_GPSNMEA_SHARED_MEMORY);
    m_SharedMemory.attach();
    m_SharedMemory.create(sizeof(NMEAGPS));
    posFix = 0;
    bool bValid = m_SharedMemory.lock();
    if(bValid)
    {
        char* from = (char*) m_SharedMemory.data();
        memcpy(&nmea_data, from, sizeof(NMEAGPS));
        posFix = nmea_data.GPS_GGA.PosFix;
    }
    else
    {
        LOG_HIGH <<"invalid\n"<<LOG_ENDL;
        TAN_PRINTF("invalid\n");
    }

    //always unlock the memory
    m_SharedMemory.unlock();


    return bValid;
}


//hsryu_0312_navi_exception
quint16 CAppAhaBase::GetAhaGPSStatus()
{
    AHA_GPS_STATUS_T gpsAvailableStatus = AHA_GPS_NONE;
    unsigned char posFix;
    //there are three possible cases as stated in enum;
    if(!QFile::exists( "/navi/bin/AppNavi" ) || m_b_MMC_removed)
    {
        gpsAvailableStatus = AHA_GPS_NO_NAVI;
    }
    else
    {
        ReadDataFromNaviShareMemory(posFix);
        if(posFix == 0)
        {
            gpsAvailableStatus = AHA_GPS_WEAK;
        }
        else
        {
            gpsAvailableStatus = AHA_GPS_OK;
        }
    }
    return gpsAvailableStatus;
}



void CAppAhaBase::ahaSampleRateChangedSlot(int nSampleRate)
{
    LOG_HIGH<< "CAppAhaBase::ahaSampleRateChangedSlot nSampleRate = " << nSampleRate <<LOG_ENDL;
    //LOG_HIGH <<"AppAhaBase::ahaSampleRateChangedSlot nSampleRate = %d \n" << nSampleRate <<LOG_ENDL;

    if(m_CurrentAhaDevice == DEVICE_TYPE_IPHONE_USB_FRONT)
    {
        m_SoundInterface.changeSampleRate(eIPOD_PORT_1, nSampleRate);
    }
    else if(m_CurrentAhaDevice == DEVICE_TYPE_IPHONE_USB_REAR )
    {
        m_SoundInterface.changeSampleRate(eIPOD_PORT_2, nSampleRate);
    }
}

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

void CAppAhaBase::NotifyAVWidget()
{
    if(!m_bIsBackGround)
        return;

    HOME_MEDIA_T stData;
    stData.nType = HomeScreenDef::MEDIA_TYPE_AHA;
    stData.nSource = HomeScreenDef::MEDIA_SOURCE_INVALID;

    wchar_t sTemp[MEDIA_STRING_LENGHT + 1];

    //hsryu_0228 ITS # 0158377
    //QString songName = CAppAhaTrack::getInstance()->GetCurrentTrackTitle();
    QString songName = CAppAhaTrack::getInstance()->GetCurrentArtist();
    ( songName + '\0').toWCharArray(sTemp);
    wcsncpy( stData.sMedia1, sTemp, songName.size() + 1 );

    //QByteArray aData = QByteArray::fromRawData( (char*)&stData, sizeof(stData) );
    QByteArray aData((char*)&stData, sizeof(stData));
    Event event( EVT_POST_EVENT, GetThisAppId(), APP_MAIN_MENU, HomeScreenDef::SET_PLAYING_MEDIA, aData );
    NotifyUISH( event, DISPLAY_FRONT );
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
void CAppAhaBase::StopNotifyAVWidget()
{
    HOME_MEDIA_T stData;
    stData.nType = HomeScreenDef::MEDIA_TYPE_INVALID;
    //QByteArray aData = QByteArray::fromRawData( (char*)&stData, sizeof(stData) );
    QByteArray aData((char*)&stData, sizeof(stData) );
    Event event( EVT_POST_EVENT, GetThisAppId(), APP_MAIN_MENU, HomeScreenDef::SET_PLAYING_MEDIA, aData );
    NotifyUISH( event, DISPLAY_FRONT );
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
void CAppAhaBase::LaunchSoundSetting()
{
    /*
    AppSettingsStartParameter param;
    param.startState = AppSettingsDef::eStart_Mode_Sound;
    Event postEvent( EVT_POST_EVENT, GetThisAppId(),
                     APP_SETTINGS, QByteArray::fromRawData( (char*)&param, sizeof( param ) ) );
    Event softEvent( EVT_KEY_SOFT_SETTINGS, GetThisAppId(), APP_UI_MANAGER );

    NotifyUISH( postEvent , DISPLAY_FRONT);
    NotifyUISH( softEvent , DISPLAY_FRONT);
    */

    APP_EVENT_DATA_BASE_T pEventData;
    pEventData.sIsJogEnabled = true;// or false
    QByteArray aData((char*)&pEventData, sizeof(pEventData));

    Event softEvent( EVT_KEY_SOFT_SETTINGS, GetThisAppId(), APP_UI_MANAGER,
                     eAPP_SETTINGS_SOUND, aData);

    NotifyUISH( softEvent, DISPLAY_FRONT );
    m_bBeforeSoundSetting = true;   //wsuk.kim 131106 temporal mode ON after launch sound setting, did not menu hide.
}


/**
 *******************************************************************
 * Check the current status, request connect to RPM and Aha
 *
 * @param[in] inSelectedIndex , selected index
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppAhaBase::Connect(int inSelectedIndex)
{
    LOG_HIGH<<"CAppAhaBase::Connect sel: "<< inSelectedIndex << " current: "<<m_CurrentAhaDevice<<LOG_ENDL;

    EAhaError error = E_SUCCESS;

    //{ modified for CR 14366 by Vijayakumar K
    /* Initializing the aha state in UISH for saying aha is now Active App*/
    AHARADIO_CONN_STATE_T state = AHARADIO_ACTIVE ;
    QByteArray connStatus;
    connStatus.append(state);
    Event event( EVT_AHA_STATUS, GetThisAppId(), APP_UI_MANAGER , connStatus );
    //NotifyUISH(event,DISPLAY_FRONT);
    //} modified for CR 14366 by Vijayakumar K
    if ( !NotifyUISHCommon(event) )
    {
        LOG_CRITICAL << "CAppAhaBase::Connect: Event notification failed ---" << LOG_ENDL;
    }
    else
    {
        LOG_HIGH<< "CAppAhaBase::Connect: Event notification sent ---" <<LOG_ENDL;
    }

    m_AhaInterface.Init();

    if(E_SUCCESS != error)
    {
        LOG_CRITICAL<<"m_AhaInterface.Init() failed error:" << error<<LOG_ENDL;
        emit handleError(error);
        // Init failed , don't proceed
        return;
    }
    LOG_HIGH<<"connect to dev : "<< inSelectedIndex <<LOG_ENDL;

    m_CurrentAhaDevice = (EDeviceType) inSelectedIndex;

    NotifyAudioPathChanged();
    updateSampleRate();       // by Ryu 20130828  ISV 89254
    m_bAV_Mode_Changed = false;

    //m_Track.SetAudioPathRPM(type);
    if(m_CurrentAhaDevice != DEVICE_TYPE_NONE || m_CurrentAhaDevice != -1)
    {
        CAppAhaStationList::getInstance()->initialize();
        CAppAhaQmlProperty::getInstance()->initialize();
        CAppAhaTrack::getInstance()->initialize();

        EAhaError status = m_AhaInterface.Connect(m_CurrentAhaDevice);

        if( E_SUCCESS != status  )
        {
            HandleIfError(status);
        }
    }
    LOG_HIGH<<"CAppAhaBase::Connect ->"<<LOG_ENDL;

    //ITS_0228611
    m_NetworkStatusCode = AHA_NOTIFY_CONNECTING;
}


//hsryu_0228 ITS # 0158377
void CAppAhaBase::OSDInfoAVOff()
{
    QString str;
    str.clear();
    DisplayOSD(OSD_AV_OFF, str);
}

void CAppAhaBase::OSDTrackInfo()
{
    QString str="";
    str = CAppAhaTrack::getInstance()->GetCurrentArtist();

    DisplayOSD(OSD_TRACK_INFO, str);
}
//hsryu_0228 ITS # 0158377

//wsuk.kim TUNE
void CAppAhaBase::tuneHandler( int eventId, const Event &aEvent )
{
    switch(eventId)
    {
        case EVT_KEY_HU_JOG_DIAL_ENCODER:
            if( (int)aEvent.GetCommand() > 0 )
            {
                SetOSDFlag(true);   //wsuk.kim OSD_TUNE
                emit handleTuneDownEvent(AppEngineQMLConstants::JOG_WHEEL_RIGHT);
            }
            else if( (int)aEvent.GetCommand() < 0 )
            {
                SetOSDFlag(true);   //wsuk.kim OSD_TUNE
                emit handleTuneUpEvent(AppEngineQMLConstants::JOG_WHEEL_LEFT);
            }
            else    //wsuk.kim 131025 ITS_198006 exception OSD false flag.
            {
                LOG_HIGH<<"CAppAhaBase::tuneHandler SetOSDFlag(false)"<<LOG_ENDL;
                SetOSDFlag(false);
            }
            break;

        case EVT_KEY_CCP_JOG_DIAL_ENCODER:  //wsuk.kim WHEEL_SEARCH
        LOG_HIGH<<"[ CAppAhaBase::tuneHandler ] 111111111"<<LOG_ENDL;
            if( (int)aEvent.GetCommand() > 0 )
            {
                LOG_HIGH<<"[ CAppAhaBase::tuneHandler ] right"<<LOG_ENDL;
                emit handleCCPDownEvent(AppEngineQMLConstants::JOG_WHEEL_RIGHT);
            }
            else
            {
                LOG_HIGH<<"[ CAppAhaBase::tuneHandler ] left"<<LOG_ENDL;
                emit handleCCPUpEvent(AppEngineQMLConstants::JOG_WHEEL_LEFT);
            }
            break;

//        case EVT_KEY_RRC_JOG_DIAL_ENCODER:
//            if ( m_AudioController.tuneWheel( aEvent.GetCommand() >= 0) )
//            {
//                if(!m_tuneRequestTimer.isActive())
//                {
//                    m_AudioController.onTuneRequestTimer();
//                }
//                m_tuneSelectTimer->start();
//            }
//            break;

//        case EVT_KEY_CCP_JOG_DIAL_CENTER:
//        case EVT_KEY_RRC_JOG_DIAL_CENTER:
//            if(isForeground() && m_stateUI != "listView" && m_AudioController.GetListViewModel()->getPopType() < 0)
//                    emit setDisplayOSD(true);
        case EVT_KEY_HU_TUNE_PRESSED:
            if( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

                if ( eventData->keyStatus == KEY_STATUS_RELEASED )
                {
                    SetOSDFlag(true);   //wsuk.kim OSD_TUNE
                    emit handleTuneCenterEvent();
                }
                else    //wsuk.kim 130806 ITS_0182685 depress when pressed with CCP/Tune Knob
                {
                    emit handelTuneCenterPressedEvent();
                }
            }
//            else
//            {
//                if (!selectTune())
//                {
//                    emit setDisplayOSD(true);
//                }
//            }
        break;

        default:
            break;
    }
}
//wsuk.kim TUNE

void CAppAhaBase::OSDInfoReadySlot(QString &strOSDString)
{
    DisplayOSD(OSD_TRACK_INFO,strOSDString);
}

void CAppAhaBase::OSDInfoCannotSkip()
{
    QString str;
    str.clear();
    DisplayOSD(OSD_CANNOT_SKIP, str);
}

void CAppAhaBase::OSDInfoCannotSkipBack()
{
    QString str;
    str.clear();
    DisplayOSD(OSD_CANNOT_SKIP_BACK, str);
}

void CAppAhaBase::OSDInfoCannotREW15()
{
    QString str;
    str.clear();
    DisplayOSD(OSD_CANNOT_REW15, str);
}

void CAppAhaBase::OSDInfoCannotFW30()
{
    QString str;
    str.clear();
    DisplayOSD(OSD_CANNOT_FW30, str);
}
//hsryu_0423_block_play_btcall
void CAppAhaBase::OSDInfoCannotPlayBTCall()
{
    QString str;
    str.clear();

    if(m_bIsBackGround)
    {
        DisplayOSD(OSD_CANNOT_PLAY_BT_CALL, str);
    }
    else    //wsuk.kim 130827 ITS_0183823 dimming cue BTN & display OSD instead of inform popup during BT calling.
    {
        noBackGroundDisplayOSD(OSD_CANNOT_PLAY_BT_CALL, str); //modify by ys ITS-0221518
    }

}

void CAppAhaBase::OSDTrackTimeShift(int nTimeShift)
{
    QString str="";
    str = CAppAhaTrack::getInstance()->GetCurrentArtist();

    // 1 = REW15,   2 = FF30
    if (nTimeShift == 1)
        DisplayOSD(OSD_TRACK_REW15, str);
    else
        DisplayOSD(OSD_TRACK_FW30, str);
}

//wsuk.kim OSD_TUNE
void CAppAhaBase::OSDStationNameTuneUpDown(int nTuneIndex, bool bTrackVew)
{
    bool bCurrentStation = false;   //0404
    QString str="";

    bCurrentStation = CAppAhaStationList::getInstance()->checkSameStationId(nTuneIndex);    //0404
    str = CAppAhaStationList::getInstance()->getStationNameTune(nTuneIndex, bTrackVew);

    if(bCurrentStation)
        DisplayOSD(OSD_TUNE_CURRENT, str);
    else
        DisplayOSD(OSD_TUNE_UPDOWN, str);
}
//wsuk.kim OSD_TUNE

void CAppAhaBase::OSDInfoNoNetwork()    //wsuk.kim 131204 ITS_212572 state network error, display error info on OSD.
{
    QString str;
    str.clear();
    DisplayOSD(OSD_NO_NETWORK, str);
}

void CAppAhaBase::OSDInfoConnectingtoaha()      // added by Ryu 20141027
{
    QString str;
    str.clear();
    DisplayOSD(OSD_CONNECTING_TO_AHA, str);
}

void CAppAhaBase::SetOSDFlag(bool bSetOSD)
{
    m_bSetOSD = bSetOSD;
}

//hsryu_0621_ITS_0174694
void CAppAhaBase::ClearOSD()
{
    OSDWriter::SendOSD("@OSD:FR:CLEAR_AV");
}


//hsryu_0411_change_track_info
void CAppAhaBase::requestOSDTimer(int interval)
{
    QTimer::singleShot(interval, this, SLOT(OnTimeout_OSDEvent()));
}

void CAppAhaBase::OnTimeout_OSDEvent()
{
    SetOSDFlag(true);
    OSDTrackInfo();
}
//hsryu_0411_change_track_info

void CAppAhaBase::DisplayOSD(OSDDisplayType osdtype, QString &strOSDString)
{
    if(!m_bSetOSD)
    {
        return;
    }
    m_bSetOSD = false;

    //ITS_0226666
    if( osdtype == OSD_TRACK_REW15 || osdtype == OSD_TRACK_FW30 )
    {

    }
    //hsryu_0621_ITS_0174694
    else if(!m_bIsBackGround || /*!m_bAhaConnected ||*/ !bOSD_DISPLAY) //if is not in background, do not print anything
    {
        return;
    }

    QString str="";

    switch(osdtype)
    {
    //hsryu_0410 ITS # 0162659
    case OSD_AV_OFF:
//hsryu_0412_clear_OSD
/*
        str = "@OSD:FR^COMMON^WARN^";
        str += qApp->translate("main","STR_AHA_AV_OFF");
        this->GetCurrentView()->setWindowTitle(str);
        str.clear();*/
        break;
    case OSD_TRACK_INFO:
        str = "@OSD:FR^MEDIA^INFO^AHA^";
        str += "aha^";
        str += "MUSIC^";
        str += strOSDString;
        //hsryu_0508_QmlStatusBar
        OSDWriter::SendOSD(str);
        //this->GetCurrentView()->setWindowTitle(str);
        str.clear();
        break;
    case OSD_CANNOT_SKIP:
        str = "@OSD:FR^MEDIA_INFO^WARN^AHA^";
        str += "aha^";
        str += qApp->translate("main", "STR_NO_SUPPORT_SKIP");
        //hsryu_0508_QmlStatusBar
        OSDWriter::SendOSD(str);
        //this->GetCurrentView()->setWindowTitle(str);
        str.clear();
        break;
    case OSD_CANNOT_SKIP_BACK:
        str = "@OSD:FR^MEDIA_INFO^WARN^AHA^";
        str += "aha^";
        str += qApp->translate("main", "STR_NO_SUPPORT_SKIP_BACK");
        //hsryu_0508_QmlStatusBar
        OSDWriter::SendOSD(str);
        //this->GetCurrentView()->setWindowTitle(str);
        str.clear();
        break;
    case OSD_CANNOT_REW15:
        str = "@OSD:FR^MEDIA_INFO^WARN^AHA^";
        str += "aha^";
        str += qApp->translate("main", "STR_AHA_NO_SUPPORT_REW15_OSD");
        //hsryu_0508_QmlStatusBar
        OSDWriter::SendOSD(str);
        //this->GetCurrentView()->setWindowTitle(str);
        str.clear();
        break;
    case OSD_CANNOT_FW30:
        str = "@OSD:FR^MEDIA_INFO^WARN^AHA^";
        str += "aha^";
        str += qApp->translate("main", "STR_AHA_NO_SUPPORT_FW30_OSD");
        //hsryu_0508_QmlStatusBar
        OSDWriter::SendOSD(str);
        //this->GetCurrentView()->setWindowTitle(str);
        str.clear();
        break;
    case OSD_TRACK_REW15:
        str = "@OSD:FR^MEDIA^INFO^AHA^";
        str += "aha^";
        str += "MUSIC^";
        str += strOSDString + QString("^");
        str += "(REW)";
        //hsryu_0508_QmlStatusBar
        OSDWriter::SendOSD(str);
        //this->GetCurrentView()->setWindowTitle(str);
        str.clear();
        break;
    case OSD_TRACK_FW30:
        str = "@OSD:FR^MEDIA^INFO^AHA^";
        str += "aha^";
        str += "MUSIC^";
        str += strOSDString + QString("^");
        str += "(FF)";
        //hsryu_0508_QmlStatusBar
        OSDWriter::SendOSD(str);
        //this->GetCurrentView()->setWindowTitle(str);
        str.clear();
        break;
    //hsryu_0423_block_play_btcall
    case OSD_CANNOT_PLAY_BT_CALL:
        str = "@OSD:FR^MEDIA_INFO^WARN^AHA^";
        str += "aha^";
        str += qApp->translate("main", "STR_AHA_PLEASE_TRY_AGAIN_AFTER_ENDING_CALL");
        //hsryu_0508_QmlStatusBar
        OSDWriter::SendOSD(str);
        //this->GetCurrentView()->setWindowTitle(str);
        str.clear();
        break;
//wsuk.kim OSD_TUNE
    case OSD_TUNE_CURRENT:
//        str = "@OSD:FR^MEDIA_INFO^AHA_STATION^AHA^Aha^";
        str = "@OSD:FR^AHA^INFO^aha^";
        str += strOSDString;
        //hsryu_0508_QmlStatusBar
        OSDWriter::SendOSD(str);
        //this->GetCurrentView()->setWindowTitle(str);
        str.clear();
        break;
    case OSD_TUNE_UPDOWN:
//        str = "@OSD:FR^DMB^INFO_DIAL^DMB^";
        str = "@OSD:FR^AHA^INFO_DIAL^aha^";
        str += strOSDString;
        //hsryu_0508_QmlStatusBar
        OSDWriter::SendOSD(str);
        //this->GetCurrentView()->setWindowTitle(str);
        str.clear();
        break;
//wsuk.kim OSD_TUNE

    case OSD_NO_NETWORK:    //wsuk.kim 131204 ITS_212572 state network error, display error info on OSD.
        str = "@OSD:FR^MEDIA_INFO^WARN^AHA^";
        str += "aha^";
        str += qApp->translate("main", "STR_AHA_UNABLE_TO_CONNECT_TO_AHA");
        OSDWriter::SendOSD(str);
        str.clear();
        break;
    case OSD_CONNECTING_TO_AHA:
        str = "@OSD:FR^MEDIA_INFO^LOADING^AHA^";
        str += "aha^";
        str += qApp->translate("main", "STR_AHA_CONNECTING_VIEW_TEXT1");
        OSDWriter::SendOSD(str);
        str.clear();
        break;

    default:
        break;
    }
    //hsryu_0411_change_track_info
    //QCoreApplication::flush();
}

//wsuk.kim 130827 ITS_0183823 dimming cue BTN & display OSD instead of inform popup during BT calling.
void CAppAhaBase::noBackGroundDisplayOSD(OSDDisplayType osdtype, QString &strOSDString)
{
    if(!m_bAhaConnected || !bOSD_DISPLAY)
    {
        return;
    }

    QString str="";

    switch(osdtype)
    {
    case OSD_CANNOT_PLAY_BT_CALL:
/* //modify by ys ITS-0221518
        str = "@OSD:FR^MEDIA_INFO^WARN^AHA^";
        str += "aha^";
        str += qApp->translate("main", "STR_AHA_PLEASE_TRY_AGAIN_AFTER_ENDING_CALL");

        OSDWriter::SendOSD(str);
        str.clear();
*/
        LOG_HIGH<< "[JYS]CAppAhaBase::noBackGroundDisplayOSD" << LOG_ENDL;
        emit handleShowPopup(16); //const_AHA_AHA_FEATURE_NOT_AVAILABLE_DURING_CALL

        break;

    default:
        break;
    }
}
//wsuk.kim 130827 ITS_0183823 dimming cue BTN & display OSD instead of inform popup during BT calling.

void CAppAhaBase::updateSampleRate()
{
//    LOG_HIGH <<"CAppAhaBase::updateSampleRate \n"<<LOG_ENDL;
    LOG_HIGH<< "CAppAhaBase::updateSampleRate" << LOG_ENDL;

    switch(m_CurrentAhaDevice)
    {
    case DEVICE_TYPE_IPHONE_USB_FRONT:
        m_SoundInterface.updateSampleRate(eIPOD_PORT_1);
        break;
    case DEVICE_TYPE_IPHONE_USB_REAR:
        m_SoundInterface.updateSampleRate(eIPOD_PORT_2);
        break;
    case DEVICE_TYPE_SMARTPHONE:
        break;
    case DEVICE_TYPE_NONE:
    default:
        LOG_CRITICAL<<"Its wrong device type \n";
        break;
    }
}

void CAppAhaBase::openSoundChannel()
{
    //LOG_HIGH <<"CAppAhaBase::openSoundChannel \n"<<LOG_ENDL;
    LOG_HIGH<< "CAppAhaBase::openSoundChannel" << LOG_ENDL;

    if(IsAVModeChanged() == true)
        return;

  //  MODE_STATE_T avState = MODE_STATE_NONE;

    switch(m_CurrentAhaDevice)
    {
    case DEVICE_TYPE_IPHONE_USB_FRONT:
        m_SoundInterface.openSoundChannel(eIPOD_PORT_1);
        isSoundOpen = true; //hsryu_0422_can_test
        break;
    case DEVICE_TYPE_IPHONE_USB_REAR:
        m_SoundInterface.openSoundChannel(eIPOD_PORT_2);
        isSoundOpen = true; //hsryu_0422_can_test
        break;
    case DEVICE_TYPE_SMARTPHONE:
        break;
    case DEVICE_TYPE_NONE:
    default:
        LOG_CRITICAL<<"Its wrong device type \n";
        break;
    }
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
//void CAppAhaBase::NotifyAudioPathChanged(EDeviceType selectedDevice)
void CAppAhaBase::NotifyAudioPathChanged()
{
//    LOG_HIGH <<"CAppAhaBase::NotifyAudioPathChanged\n"<<LOG_ENDL;
    LOG_HIGH<< "CAppAhaBase::NotifyAudioPathChanged" << LOG_ENDL;

    MODE_STATE_T avState = MODE_STATE_NONE;

    switch(m_CurrentAhaDevice)
    {
    case DEVICE_TYPE_IPHONE_USB_FRONT:
        avState =  MODE_STATE_AHA_RADIO_IPOD1;
        break;
    case DEVICE_TYPE_IPHONE_USB_REAR:
        avState =  MODE_STATE_AHA_RADIO_IPOD2;
        break;
    case DEVICE_TYPE_SMARTPHONE:
        avState =  MODE_STATE_AHA_RADIO_BT;
        break;
    case DEVICE_TYPE_NONE:
    default:
        LOG_CRITICAL<<"Its wrong device type \n";
        break;
    }

    EVENT_AUDIO_PATH_REQUEST_DATA message = { { APP_UI_MANAGER, EVT_REQUEST_AUDIO_PATH, 0, 0 }, avState };

    message.header.dataSize = sizeof( message );

    QByteArray aData( (char*)&message, sizeof( message ) ) ;

    Event event( EVT_MUTE_CANCEL/*EVT_REQUEST_AUDIO_PATH*/, GetThisAppId(), APP_UI_MANAGER, aData );    //wsuk.kim 131202 ITS_212328 to change unmute interface.

    //{ Modified by esjang 2012.12.22 for SwapDC
    //NotifyUISH( event,DISPLAY_FRONT );
    if ( !NotifyUISHCommon(event ) )
    {
        LOG_HIGH <<"NOTIFY : EVT_REQUEST_AUDIO_PATH  to UISH : Event notification failed \n"<<LOG_ENDL;
        LOG_CRITICAL << "CAppAhaBase::NotifyAudioPathChanged: Event notification failed" << LOG_ENDL;
    }
    else
    {
        LOG_HIGH<< "CAppAhaBase::NotifyAudioPathChanged: Event notification sent" <<LOG_ENDL;
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

void CAppAhaBase::NotifyFromUIStateManagerCommon( Event aEvent )
{
//    LOG_HIGH <<"300suk NotifyFromUI [%d] \n", aEvent.GetEventId());
    LOG_HIGH<<"CAppAhaBase::NotifyFromUIStateManagerCommon event id: "<< aEvent.GetEventId() <<LOG_ENDL;

    switch ( aEvent.GetEventId() )
    {
    //Tan 2014.03.25
    case EVT_POWER_STATE_NORMAL_ON:
    case EVT_POWER_STATE_LOGIC_ON:
        m_HeadUnitPowerState = HEADUNIT_POWER_ON;
        break;
    case EVT_POWER_STATE_NORMAL_OFF:
    case EVT_POWER_STATE_LOGIC_OFF:
        m_HeadUnitPowerState = HEADUNIT_POWER_OFF;
        break;
    //hsryu_0603_cluster_update
    case EVT_UPDATE_CLUSTER:
    {
        LOG_HIGH<<"EVT_UPDATE_CLUSTER"<<LOG_ENDL;
        setClusterUpdate(true);
        sendClearTpMessage();
        SendClusterData();
        sendCanTpMessage(0x48F , CAppAhaTrack::getInstance()->GetCurrentArtist());
    }
    break;    
    case EVT_LANGUAGE_CHANGED:
    {
        LOG_HIGH<<"EVT_LANGUAGE_CHANGED"<<LOG_ENDL;
        EVENT_LANGUAGE_CHANGED_DATA_T *pLanguageData = (EVENT_LANGUAGE_CHANGED_DATA_T *)aEvent.Data().data();

        if ( pLanguageData != NULL )
        {
            if ( !m_pTranslator )
            {
                m_pTranslator = new QTranslator( this );
            }

            switch ( pLanguageData->languageId )
            {
            case LANGUAGE_KO:
                LOG_TRACE << "HomeMenuAppEngine::NotifyFromUIStateManager:Processing Event::EVT_LANGUAGE_CHANGED::KO" << LOG_ENDL;
                m_pTranslator->load( "/app/share/AppAhaRadio/localization/AhaRadio_ko" );
                break;
            case LANGUAGE_EN_US:    // modified by Ryu for language 20130823
                LOG_TRACE << "HomeMenuAppEngine::NotifyFromUIStateManager:Processing Event::EVT_LANGUAGE_CHANGED::EN" << LOG_ENDL;
                m_pTranslator->load( "/app/share/AppAhaRadio/localization/AhaRadio_en" );
                break;
            case LANGUAGE_FR_NA:    // modified by Ryu for language 20130823
                LOG_TRACE << "HomeMenuAppEngine::NotifyFromUIStateManager:Processing Event::EVT_LANGUAGE_CHANGED::FR_NA" << LOG_ENDL;
                m_pTranslator->load( "/app/share/AppAhaRadio/localization/AhaRadio_fr-na" );
                break;
            case LANGUAGE_ES_NA:    // modified by Ryu for language 20130823
                LOG_TRACE << "HomeMenuAppEngine::NotifyFromUIStateManager:Processing Event::EVT_LANGUAGE_CHANGED::ES_NA" << LOG_ENDL;
                m_pTranslator->load( "/app/share/AppAhaRadio/localization/AhaRadio_es-na" );
                break;
            default:
                LOG_TRACE << "HomeMenuAppEngine::NotifyFromUIStateManager:Processing Event::EVT_LANGUAGE_CHANGED::DEF" << LOG_ENDL;
                m_pTranslator->load( "/app/share/AppAhaRadio/localization/AhaRadio_en" );
                break;
            }

            qApp->installTranslator( m_pTranslator );

            emit retranslateUi( pLanguageData->languageId );
        }
    }
    break;

    case EVT_AV_MODE_CHANGE:
    {
        LOG_HIGH <<"NOTIFY : EVT_AV_MODE_CHANGE \n"<<LOG_ENDL;
        LOG_HIGH<<"EVT_AV_MODE_CHANGE"<<LOG_ENDL;
        //LOG_HIGH<< "m_eHMIState : " << m_eHMIState << "m_bAhaConnected" << m_bAhaConnected << LOG_ENDL;

        // update the current state
        if( m_eHMIState == CAppAhaBase::AppInFG || m_eHMIState == CAppAhaBase::AppInBG )
        {
            if( !aEvent.Data().isEmpty() )
            {
                EVENT_MODE_CHANGED_DATA_T* pData = (EVENT_MODE_CHANGED_DATA_T*)aEvent.Data().data();

                handleAVModeChange(pData->sMode, m_bAhaConnected);
            }
        }
    }
    break;

    case EVT_IPOD_REMOVED:
    {
        LOG_HIGH<<"EVT_IPOD_REMOVED"<<LOG_ENDL;
        if ( !aEvent.Data().isEmpty() )
        {
            APP_EVENT_IPOD_DATA_T *param = (APP_EVENT_IPOD_DATA_T*)(aEvent.Data().data());
            USB_PORT_T  port =  param->usbPort;
            LOG_INFO<<"IPod unmounted to port : " << port <<LOG_ENDL;
            LOG_INFO<<"IPod unmount path :  " << param->mountPath <<LOG_ENDL;
            EDeviceType removedDeviceType;
            switch (port)
            {
            case eUSB_PORT1:
                removedDeviceType = DEVICE_TYPE_IPHONE_USB_FRONT;
                m_SoundInterface.resetSampleRate(eIPOD_PORT_1);
                break;
            case eUSB_PORT2:
                removedDeviceType = DEVICE_TYPE_IPHONE_USB_REAR;
                m_SoundInterface.resetSampleRate(eIPOD_PORT_2);
                break;
            case eUSB_PORT_INVALID:
            default:
                removedDeviceType = DEVICE_TYPE_NONE;
                break;
            }

            if( m_CurrentAhaDevice == removedDeviceType)
            {
                CAppAhaStationList::getInstance()->modelClear();

                //Aha is not running in background
                emit connectionStatusChanged(2); //hsryu_0516_remove_device
                UpdateAhaAvailableStatus(false);
                ClearCache();
                m_CurrentAhaDevice = DEVICE_TYPE_NONE;//No aha connected
            }
            if(m_AhaInterface.m_AhaStateHandler.m_AhaUSBFrameTxRx)
                m_AhaInterface.m_AhaStateHandler.m_AhaUSBFrameTxRx->DestroyIpodAppControllerInstance(removedDeviceType);
            break;
        }
    }
    break;

    case EVT_KEY_HU_SEEK_NEXT:
        LOG_HIGH<<"EVT_KEY_HU_SEEK_NEXT"<<LOG_ENDL;
        if ( !aEvent.Data().isEmpty() )
        {
            LOG_HIGH << "Seek up key pressed -> Display OSD" << LOG_ENDL;

            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_RELEASED && m_bDuplicate == false)
            {
                emit signalSeekTrackKey( AppEngineQMLConstants::JOG_RIGHT, AppEngineQMLConstants::KEY_STATUS_RELEASED );
                if(isJogKeyLongPress()) //wsuk.kim SWRC_HU_long
                {
                    emit handleSeekTrackReleased();
                    setIsJogKeyLongPress(0);
                }
                else
                {
                    SetOSDFlag(true);
                    emit handleSkipEvent(false);
                }
            }
            else if(eventData->keyStatus == KEY_STATUS_PRESSED )    //wsuk.kim SWRC_HU_long
            {
                //ITS_0222228, 0222230
//                if(isJogKeyLongPress())
//                {
//                    emit signalSeekTrackKey( AppEngineQMLConstants::JOG_RIGHT, AppEngineQMLConstants::KEY_STATUS_RELEASED );
//                    emit signalSeekTrackKey( AppEngineQMLConstants::JOG_LEFT, AppEngineQMLConstants::KEY_STATUS_RELEASED );
//                    emit handleSeekTrackReleased();
//                    setIsJogKeyLongPress(0);
//                    m_bDuplicate = true;
//                }
//                else
                {
                    setIsJogKeyLongPress(0);
                    emit signalSeekTrackKey( AppEngineQMLConstants::JOG_RIGHT, AppEngineQMLConstants::KEY_STATUS_PRESSED );
                    m_bDuplicate = false;
                 }
            }
            else if(eventData->keyStatus == KEY_STATUS_PRESSED_LONG )   //wsuk.kim SWRC_HU_long
            {
                SetOSDFlag(true);
                emit handleForward30Event(false);
                setIsJogKeyLongPress(1);
            }
        }
        break;

    case EVT_KEY_SWRC_SEEK_NEXT:
        LOG_HIGH<<"EVT_KEY_SWRC_SEEK_NEXT"<<LOG_ENDL;
        if ( !aEvent.Data().isEmpty() )
        {
            LOG_HIGH << "Seek up key pressed -> Display OSD" << LOG_ENDL;

            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_RELEASED && m_bDuplicate == false)
            {
                emit signalSeekTrackKey( AppEngineQMLConstants::JOG_RIGHT, AppEngineQMLConstants::KEY_STATUS_RELEASED );
                if(isJogKeyLongPress()) //wsuk.kim SWRC_HU_long
                {
                    emit handleSeekTrackReleased();
                    setIsJogKeyLongPress(0);
                }
                else
                {
                    SetOSDFlag(true);
                    emit handleSkipEvent(false);
                }
            }
            else if(eventData->keyStatus == KEY_STATUS_PRESSED )    //wsuk.kim SWRC_HU_long
            {
                //ITS_0222228, 0222230
//                if(isJogKeyLongPress())
//                {
//                    emit signalSeekTrackKey( AppEngineQMLConstants::JOG_RIGHT, AppEngineQMLConstants::KEY_STATUS_RELEASED );
//                    emit signalSeekTrackKey( AppEngineQMLConstants::JOG_LEFT, AppEngineQMLConstants::KEY_STATUS_RELEASED );
//                    emit handleSeekTrackReleased();
//                    setIsJogKeyLongPress(0);
//                    m_bDuplicate = true;
//                }
//                else
                {
                    setIsJogKeyLongPress(0);
                    emit signalSeekTrackKey( AppEngineQMLConstants::JOG_RIGHT, AppEngineQMLConstants::KEY_STATUS_PRESSED );
                    m_bDuplicate = false;
                 }
            }
            else if(eventData->keyStatus == KEY_STATUS_PRESSED_LONG )   //wsuk.kim SWRC_HU_long
            {
                SetOSDFlag(true);
                emit handleForward30Event(false);
                setIsJogKeyLongPress(1);
            }
        }
        break;

    case EVT_KEY_HU_SEEK_PREV:
        LOG_HIGH<<"EVT_KEY_HU_SEEK_PREV"<<LOG_ENDL;
        if ( !aEvent.Data().isEmpty() )
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_RELEASED && m_bDuplicate == false)
            {
                emit signalSeekTrackKey( AppEngineQMLConstants::JOG_LEFT, AppEngineQMLConstants::KEY_STATUS_RELEASED );
                if(isJogKeyLongPress()) //wsuk.kim SWRC_HU_long
                {
                    emit handleSeekTrackReleased();
                    setIsJogKeyLongPress(0);
                }
                else
                {
                    SetOSDFlag(true);
                    emit handleSkipBackEvent(false);
                }
            }
            else if(eventData->keyStatus == KEY_STATUS_PRESSED )    //wsuk.kim SWRC_HU_long
            {
                //ITS_0222228, 0222230
//                if(isJogKeyLongPress())
//                {
//                    emit signalSeekTrackKey( AppEngineQMLConstants::JOG_RIGHT, AppEngineQMLConstants::KEY_STATUS_RELEASED );
//                    emit signalSeekTrackKey( AppEngineQMLConstants::JOG_LEFT, AppEngineQMLConstants::KEY_STATUS_RELEASED );
//                    emit handleSeekTrackReleased();
//                    setIsJogKeyLongPress(0);
//                    m_bDuplicate = true;
//                }
//                else
                {
                    setIsJogKeyLongPress(0);
                    emit signalSeekTrackKey( AppEngineQMLConstants::JOG_LEFT, AppEngineQMLConstants::KEY_STATUS_PRESSED );
                    m_bDuplicate = false;
                }
            }
            else if(eventData->keyStatus == KEY_STATUS_PRESSED_LONG )   //wsuk.kim SWRC_HU_long
            {
                SetOSDFlag(true);
                emit handleRewind15Event(false);
                setIsJogKeyLongPress(1);
            }
        }
        break;

    case EVT_KEY_SWRC_SEEK_PREV:
        LOG_HIGH<<"EVT_KEY_SWRC_SEEK_PREV"<<LOG_ENDL;
        if ( !aEvent.Data().isEmpty() )
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_RELEASED && m_bDuplicate == false)
            {
                emit signalSeekTrackKey( AppEngineQMLConstants::JOG_LEFT, AppEngineQMLConstants::KEY_STATUS_RELEASED );
                if(isJogKeyLongPress()) //wsuk.kim SWRC_HU_long
                {
                    emit handleSeekTrackReleased();
                    setIsJogKeyLongPress(0);
                }
                else
                {
                    SetOSDFlag(true);
                    emit handleSkipBackEvent(false);
                }
            }
            else if(eventData->keyStatus == KEY_STATUS_PRESSED )    //wsuk.kim SWRC_HU_long
            {
                //ITS_0222228, 0222230
//                if(isJogKeyLongPress())
//                 {
//                    emit signalSeekTrackKey( AppEngineQMLConstants::JOG_RIGHT, AppEngineQMLConstants::KEY_STATUS_RELEASED );
//                    emit signalSeekTrackKey( AppEngineQMLConstants::JOG_LEFT, AppEngineQMLConstants::KEY_STATUS_RELEASED );
//                    emit handleSeekTrackReleased();
//                    setIsJogKeyLongPress(0);
//                    m_bDuplicate = true;
//                 }
//                else
                {
                    setIsJogKeyLongPress(0);
                    emit signalSeekTrackKey( AppEngineQMLConstants::JOG_LEFT, AppEngineQMLConstants::KEY_STATUS_PRESSED );
                    m_bDuplicate = false;
                }
            }
            else if(eventData->keyStatus == KEY_STATUS_PRESSED_LONG )   //wsuk.kim SWRC_HU_long
            {
                SetOSDFlag(true);
                emit handleRewind15Event(false);
                setIsJogKeyLongPress(1);
            }
        }
        break;

    case EVT_KEY_CCP_JOG_DIAL_UP:
        LOG_HIGH<<"EVT_KEY_CCP_JOG_DIAL_UP"<<LOG_ENDL;

        if ( !aEvent.Data().isEmpty() )
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_RELEASED )
            {
                emit jogHardkeyPressed(AHA_EVT_KEY_JOG_ARROW_UP);
            }
//wsuk.kim 130815 NO_USED
//            else
//            {
//                emit jogsendKeyTovc(AHA_EVT_KEY_JOG_ARROW_UP);
//            }
        }
        break;

    case EVT_KEY_CCP_JOG_DIAL_DOWN:
        LOG_HIGH<<"EVT_KEY_CCP_JOG_DIAL_DOWN"<<LOG_ENDL;

        if ( !aEvent.Data().isEmpty() )
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_RELEASED )
            {
                emit jogHardkeyPressed(AHA_EVT_KEY_JOG_ARROW_DOWN);
            }
//wsuk.kim 130815 NO_USED
//            else
//            {
//                emit jogsendKeyTovc(AHA_EVT_KEY_JOG_ARROW_DOWN);
//            }
        }
        break;

    case EVT_KEY_CCP_JOG_DIAL_LEFT:
        LOG_HIGH<<"EVT_KEY_CCP_JOG_DIAL_LEFT"<<LOG_ENDL;
        if ( !aEvent.Data().isEmpty() )
        {
            LOG_HIGH<<"EVT_KEY_CCP_JOG_DIAL_LEFT 11111111111111"<<LOG_ENDL;
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_RELEASED )
            {
                LOG_HIGH<<"EVT_KEY_CCP_JOG_DIAL_LEFT 22222222222222"<<LOG_ENDL;

                /*wsuk.kim jog_long_press*/
                if(isJogKeyLongPress())
                {
                    setIsJogKeyLongPress(0);
                }
                else
                {
                    LOG_HIGH<<"EVT_KEY_CCP_JOG_DIAL_LEFT 33333333333333"<<LOG_ENDL;
                    emit jogHardkeyPressed(AHA_EVT_KEY_JOG_ARROW_LEFT);
                }
            }
            /*wsuk.kim jog_long_press*/
            else if(eventData->keyStatus == KEY_STATUS_PRESSED_LONG)
            {
                LOG_HIGH<<"EVT_KEY_CCP_JOG_DIAL_LEFT 44444444444444"<<LOG_ENDL;
//wsuk.kim 131114 OSB build error, signalJogNavigation parameter no matching.       emit signalJogNavigation( AppEngineQMLConstants::JOG_LEFT, AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED, false);
                setIsJogKeyLongPress(1);
            }
            /*wsuk.kim jog_long_press*/
//wsuk.kim 130815 NO_USED
//            else
//            {
//                emit jogsendKeyTovc(AHA_EVT_KEY_JOG_ARROW_LEFT);
//            }
        }
        break;

    case EVT_KEY_CCP_JOG_DIAL_RIGHT:
        LOG_HIGH<<"EVT_KEY_CCP_JOG_DIAL_RIGHT"<<LOG_ENDL;
        if ( !aEvent.Data().isEmpty() )
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_RELEASED )
            {
                LOG_HIGH<<"EVT_KEY_CCP_JOG_DIAL_RIGHT 111111"<<LOG_ENDL;
                /*wsuk.kim jog_long_press*/
                if(isJogKeyLongPress())
                {
                    setIsJogKeyLongPress(0);
                }
                else
                {
                    LOG_HIGH<<"EVT_KEY_CCP_JOG_DIAL_RIGHT 222222222222"<<LOG_ENDL;
                    emit jogHardkeyPressed( AHA_EVT_KEY_JOG_ARROW_RIGHT);
                }
            }
            /*wsuk.kim jog_long_press*/
            else if(eventData->keyStatus == KEY_STATUS_PRESSED_LONG)
            {
                LOG_HIGH<<"EVT_KEY_CCP_JOG_DIAL_RIGHT 333333333333"<<LOG_ENDL;
//wsuk.kim 131114 OSB build error, signalJogNavigation parameter no matching.       emit signalJogNavigation( AppEngineQMLConstants::JOG_RIGHT, AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED, false);
                setIsJogKeyLongPress(1);
            }
            /*wsuk.kim jog_long_press*/
//wsuk.kim 130815 NO_USED
//            else
//            {
//                emit jogsendKeyTovc(AHA_EVT_KEY_JOG_ARROW_RIGHT);
//            }
        }
        break;

//    case EVT_KEY_CCP_JOG_DIAL_TOP_RIGHT:

//        if ( !aEvent.Data().isEmpty() )
//        {
//            TAN_PRINTF("NOTIFY : EVT_KEY_CCP_JOG_DIAL_TOP_RIGHT \n");
//            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

//            if ( eventData->keyStatus == KEY_STATUS_RELEASED )
//            {
//                emit jogHardkeyPressed(AHA_EVT_KEY_JOG_WHEEL_RIGHT);
//            }
//            else
//            {
//                emit jogsendKeyTovc(AHA_EVT_KEY_JOG_WHEEL_RIGHT);
//            }
//        }
//        break;

    case EVT_KEY_CCP_JOG_DIAL_CENTER:
        LOG_HIGH<<"EVT_KEY_CCP_JOG_DIAL_CENTER"<<LOG_ENDL;

        if ( !aEvent.Data().isEmpty() )
        {
            LOG_HIGH<<"EVT_KEY_CCP_JOG_DIAL_CENTER 1111111111111"<<LOG_ENDL;
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_RELEASED )
            {
                LOG_HIGH<<"EVT_KEY_CCP_JOG_DIAL_CENTER 2222222222222222"<<LOG_ENDL;
                emit jogHardkeyPressed(AHA_EVT_KEY_JOG_CENTER);
            }
//wsuk.kim 130815 NO_USED
//            else
//            {
//                emit jogsendKeyTovc(AHA_EVT_KEY_JOG_CENTER);
//            }
        }
        break;

//wsuk.kim TUNE
    case EVT_KEY_HU_JOG_DIAL_ENCODER:
        LOG_HIGH<<"EVT_KEY_HU_JOG_DIAL_ENCODER"<<LOG_ENDL;
        tuneHandler( aEvent.GetEventId(), aEvent );
        break;
    case EVT_KEY_HU_TUNE_PRESSED:
        LOG_HIGH<<"EVT_KEY_HU_TUNE_PRESSED"<<LOG_ENDL;
        tuneHandler( aEvent.GetEventId(), aEvent );
        break;
    case EVT_KEY_CCP_JOG_DIAL_ENCODER:  //wsuk.kim WHEEL_SEARCH
        LOG_HIGH<<"EVT_KEY_CCP_JOG_DIAL_ENCODER"<<LOG_ENDL;
//    case EVT_KEY_RRC_JOG_DIAL_CENTER:
//    case EVT_KEY_RRC_JOG_DIAL_ENCODER:
      tuneHandler( aEvent.GetEventId(), aEvent );
      break;
//wsuk.kim TUNE

    case EVT_VR_COMMAND:
    {
        LOG_HIGH<<"EVT_VR_COMMAND"<<LOG_ENDL;
        if ( !aEvent.Data().isEmpty() )
        {
            EVENT_EVT_VR_COMMAND_DATA_T *eventData = (EVENT_EVT_VR_COMMAND_DATA_T*)aEvent.Data().data();
            switch ( eventData->commandId )
            {
            case AV_MODE_CONTROL_PLAY:
            {
                int payLoad =  eventData->sVRPayload.data[0] ;
                LOG_HIGH<<"AV_MODE_CONTROL_PLAY , payload : "<<payLoad<< LOG_ENDL;
                // payLoad 0 for pause , 1 for play.
                emit playCommandFromVR(payLoad);
            }
                break;
            case AV_MODE_CONTROL_NEXT_MEDIA:
            {
                int payLoad =  eventData->sVRPayload.data[0] ;
                LOG_HIGH<<"Received AV_MODE_CONTROL_NEXT_MEDIA , payload : "<<payLoad<< LOG_ENDL;
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
                LOG_HIGH<<"AV_MODE_CONTROL_RANDOM to play shuffle"<< LOG_ENDL;
                emit shuffleCommandFromVR();
            }
                break;
            /*case AV_MODE_AHA_RADIO_SKIP:
            {
                LOG_HIGH<<"AV_MODE_AHA_RADIO_SKIP"<< LOG_ENDL;
                emit handleSkipEvent(false);
            }
                break;
            */
            default:
                break;
            }
        }
        break;
    }

    case EVT_KEY_HU_MENU:
        LOG_HIGH<<"EVT_KEY_HU_MENU "<<LOG_ENDL;
        if ( !aEvent.Data().isEmpty() )
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_RELEASED )
            {
                LOG_SIGNAL<<"AppAhaBase:menuKeyPressed emitted "<<LOG_ENDL;
                emit menuKeyPressed(true);
            }
        }
        break;
    case EVT_KEY_CCP_MENU:
    {
        LOG_HIGH<<"EVT_KEY_CCP_MENU "<<LOG_ENDL;

        if ( !aEvent.Data().isEmpty() )
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_PRESSED ) // modified ITS 222249
            {
                LOG_SIGNAL<<"AppAhaBase:menuKeyPressed emitted "<<LOG_ENDL;
                emit menuKeyPressed(true);
            }
        }
        break;
    }

    case EVT_KEY_HU_BACK:
        LOG_HIGH<<"+++++ EVT_KEY_HU_BACK ++"<<LOG_ENDL;
        if ( !aEvent.Data().isEmpty() )
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_PRESSED )
            {
                LOG_SIGNAL<<"AppAhaBase:backKeyPressed emitted "<<LOG_ENDL;

                emit backKeyPressed();
            }
        }
        break;
    case EVT_KEY_CCP_BACK:
    {
        LOG_HIGH<<"+++++ EVT_KEY_CCP_BACK "<<LOG_ENDL;

        if ( !aEvent.Data().isEmpty() )
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_PRESSED )
            {
                LOG_SIGNAL<<"AppAhaBase:backKeyPressed emitted "<<LOG_ENDL;

                emit backKeyPressed();
            }
        }
        break;
    }
    case EVT_KEY_SOFT_MENU:
    {
        LOG_HIGH<<"EVT_KEY_SOFT_MENU "<<LOG_ENDL;
        emit menuKeyPressed(false);
        break;
    }
//wsuk.kim NOT_USED    case EVT_KEY_HU_TUNE_PRESSED:
//    {
//        LOG_INFO<<"EVT_KEY_HU_TUNE_PRESSED "<<LOG_ENDL;
//        break;
//    }

    //Aha play for Last Mode: esjang
    case EVT_RESTORE_LAST_AV_MODE:
    {
        LOG_HIGH<<"EVT_RESTORE_LAST_AV_MODE "<<LOG_ENDL;
        //Aha is the last AV mode.
        m_bAV_Mode_Changed = false;
        openSoundChannel();
        CAppAhaTrack::getInstance()->Play();
        NotifyAVWidget();
        //hsryu_0411_change_track_info
        SetOSDFlag(true);
        OSDTrackInfo();
        break;
    }
        //esjang
    case EVT_CALL_START:
        LOG_HIGH<<"EVT_CALL_START "<<LOG_ENDL;
//hsryu_0404_restore_playmode

        if(m_bCallingState == true)
            break;

        m_bCallingState = true; //hsryu_0322_TRANSPORT_CAN
        emit handleCallStart(m_bCallingState);    //wsuk.kim 130827 ITS_0183823 dimming cue BTN & display OSD instead of inform popup during BT calling.
        if(CAppAhaTrack::getInstance()->TrackStatus() == 1)
        {
            m_nPreviousPlayPause = AHA_PLAYBACK_PLAY;
            CAppAhaTrack::getInstance()->Pause();
        }
        else if(CAppAhaTrack::getInstance()->TrackStatus() == 0)
        {
            m_nPreviousPlayPause = AHA_PLAYBACK_PAUSE;
        }
        else
        {
            LOG_HIGH << "EVT_CALL_START: Check nPlayPause state is invalid."<< LOG_ENDL;
        }
        //bCallingPlayState = false;
        bCheckInitOnce = false;
//wsuk.kim 131016 ITS_195123 Temporal Mode        m_bContinueLastView = true; //wsuk.kim 130925 ITS_191096 remain last view menu during call.
//hsryu_0404_restore_playmode
//hsryu_0423_block_play_btcall
//        m_bConnectCall = true;
//        emit handleCallAllow(m_bConnectCall);
//wsuk.kim deactivate_call
        break;
    case EVT_CALL_END:
        LOG_HIGH<<"EVT_CALL_END "<<LOG_ENDL;
        m_bCallingState = false; //hsryu_0322_TRANSPORT_CAN
        emit handleCallStart(m_bCallingState);    //wsuk.kim 130827 ITS_0183823 dimming cue BTN & display OSD instead of inform popup during BT calling.
//hsryu_0423_block_play_btcall
//        m_bConnectCall = false;
//        emit handleCallAllow(m_bConnectCall);

        //hsryu_0422_can_test
        //TAN_PRINTF("[EVT_CALL_END2] %d\n", isSoundOpen);

        //hsryu_0603_cluster_update
        //if(isSoundOpen)
        {
            sendClearTpMessage();
            SendClusterData();
            sendCanTpMessage(0x48F , CAppAhaTrack::getInstance()->GetCurrentArtist());
        }
        LOG_HIGH<<"m_bAV_Mode_Changed "<< m_bAV_Mode_Changed <<LOG_ENDL;
        LOG_HIGH<<"m_nPreviousPlayPause "<< m_nPreviousPlayPause <<LOG_ENDL;
        //hsryu_0612_initialize_flag_status_bt_call
        if(!m_bAV_Mode_Changed)  //wsuk.kim 130910 FG is not Aha, audio play aha when BT call ended.
        {
            openSoundChannel();
            if(m_nPreviousPlayPause == AHA_PLAYBACK_PLAY)
            {
                CAppAhaTrack::getInstance()->Play();
            }
        }
        //bCallingPlayState = false;
        bCheckInitOnce = false;
//wsuk.kim 131016 ITS_195123 Temporal Mode        m_bContinueLastView = false; //wsuk.kim 130925 ITS_191096 remain last view menu during call.
        //hsryu_0404_restore_playmode
        break;

//wsuk.kim UNMUTE
    case EVT_REQEUST_MUTE:
        LOG_HIGH<<"EVT_REQEUST_MUTE "<<LOG_ENDL;
        m_bMuteState = true;
        break;

    case EVT_REQEUST_UNMUTE:
        LOG_HIGH<<"EVT_REQEUST_UNMUTE "<<LOG_ENDL;
        m_bMuteState = false;
        break;
//wsuk.kim UNMUTE
    //hsryu_0502_factory_reset
    case EVT_FACTORY_RESET_REQUEST:
    {
        LOG_HIGH<<"EVT_FACTORY_RESET_REQUEST "<<LOG_ENDL;
        Event event(EVT_FACTORY_RESET_RESPONSE, GetThisAppId(), APP_UI_MANAGER);

        //reset routine
        m_AhaInterface.m_AhaStateHandler.ReadyForClearCache();
        CAppAhaStationList::getInstance()->modelClear();

        emit connectionStatusChanged(0);
        UpdateAhaAvailableStatus(false);
        ClearCache();
        m_CurrentAhaDevice = DEVICE_TYPE_NONE;//No aha connected
        //m_AhaInterface.m_AhaStateHandler.m_AhaUSBFrameTxRx->DestroyIpodAppControllerInstance(removedDeviceType);
        m_AhaInterface.m_AhaStateHandler.ClearCacheData();

        if(! NotifyUISHCommon(event))
        {
            qDebug() << "Factory Reset Response Failed";
        }
        break;
    }

//wsuk.kim text_scroll
    case EVT_CAMERA_ENABLE:
        LOG_HIGH<<"EVT_CAMERA_ENABLE "<<LOG_ENDL;
//wsuk.kim 131016 ITS_195123 Temporal Mode
//        m_bContinueLastView = true;
//        emit handlePlayAnimationState(false);
//        m_bDRSTextScroll = false;
//        emit handleTextScrollState(false);
        break;
    case EVT_DRS_SHOW: //case EVT_KEY_CCP_JOG_DIAL_TOP_RIGHT: //wsuk.kim DRS textscroll test
        LOG_HIGH<<"EVT_DRS_SHOW "<<LOG_ENDL;
        m_bDRSTextScroll = false;
        emit handleTextScrollState(false);
        break;
    case EVT_CAMERA_DISABLE:
        LOG_HIGH<<"EVT_CAMERA_DISABLE "<<LOG_ENDL;
//wsuk.kim 131016 ITS_195123 Temporal Mode
//        m_bContinueLastView = false;
//        emit handlePlayAnimationState(true);
//        m_bDRSTextScroll = true;
//        emit handleTextScrollState(true);
        emit handleListViewPosition();
        break;
    case EVT_DRS_HIDE:  //case EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT:    //wsuk.kim DRS textscroll test
        LOG_HIGH<<"EVT_DRS_HIDE "<<LOG_ENDL;
        m_bDRSTextScroll = true;
        emit handleTextScrollState(true);
        break;
//wsuk.kim text_scroll

    case EVT_NOTIFY_AV_MODE:
        // update the current state
        LOG_HIGH<< "[EVT_NOTIFY_AV_MODE] m_eHMIState : " << m_eHMIState << "m_bAhaConnected" << m_bAhaConnected << LOG_ENDL;
        if( !aEvent.Data().isEmpty() )
        {
            EVT_COMMON_BASE_EVENT_DATA_T* pData = (EVT_COMMON_BASE_EVENT_DATA_T*)aEvent.Data().data();

            m_PrevAvMode = m_CurrentAvMode;
            m_CurrentAvMode = (MODE_STATE_T) (pData->iEventData01);

            if(m_CurrentAvMode == MODE_STATE_AHA_RADIO_BT)
            {
                m_bUnmuteSent = false;
            }

            if( m_eHMIState == CAppAhaBase::AppInFG || m_eHMIState == CAppAhaBase::AppInBG )
            {
                switch(pData->iEventData01)
                {
                case MODE_STATE_AHA_RADIO_IPOD1:
                case MODE_STATE_AHA_RADIO_IPOD2:
                case MODE_STATE_AHA_RADIO_BT:
                    bOSD_DISPLAY = true;  // ITS 222264
                    break;
                case MODE_STATE_AVOFF:
                default:
                    LOG_HIGH <<"m_SoundInterface.closeSoundChannel() data:\n"<< pData->iEventData01 <<LOG_ENDL;
                    LOG_HIGH<<"closeSoundChannel"<<LOG_ENDL;
                    CAppAhaTrack::getInstance()->Pause();  // pause
                    m_SoundInterface.closeSoundChannel();
                    break;
                }
            }
        }
         break;
    case EVT_CLONE_NOTIFICATION:
        LOG_HIGH<<"EVT_CLONE_NOTIFICATION"<<LOG_ENDL;
        break;
    case EVT_AV_MODE_NOTIFY_FROM_VR:  // ITS 222254
        LOG_HIGH<<"EVT_AV_MODE_NOTIFY_FROM_VR"<<LOG_ENDL;
        openSoundChannel();
        CAppAhaTrack::getInstance()->Play();
        break;
    case EVT_DISPLAY_OFF: // ITS 217070
        LOG_LOW<<"EVT_DISPLAY_OFF"<<LOG_ENDL;
        handleDisplayOff(true);
        break;
    case EVT_DISPLAY_ON: // ITS 217070
        LOG_LOW<<"EVT_DISPLAY_ON"<<LOG_ENDL;
        handleDisplayOff(false);
        break;
    default:
        LOG_HIGH<<"NOTIFY : default"<<LOG_ENDL;
        // LOG_HIGH <<"NOTIFY : default \n");
        break;
    }
}
#endif

//hsryu_0423_block_play_btcall
bool CAppAhaBase::IsCallingSameDevice()
{
    return m_bCallingState;
}

void CAppAhaBase::NotifyFromUIStateManager( Event aEvent )
{
    //LOG_HIGH << "CAppAhaBase::NotifyFromUIStateManager Start" << LOG_ENDL;
    //LOG_HIGH<<"aEvent.GetEventId() :"<<aEvent.GetEventId()<<LOG_ENDL;
    NotifyFromUIStateManagerGeneric(aEvent);
}

void CAppAhaBase::NotifyFromUIStateManagerRear( Event aEvent )
{
    //LOG_HIGH << "CAppAhaBase::NotifyFromUIStateManagerRear Start" << LOG_ENDL;
    //LOG_HIGH<<"aEvent.GetEventId() :"<<aEvent.GetEventId()<<LOG_ENDL;
    NotifyFromUIStateManagerGeneric(aEvent);
}

void CAppAhaBase::  NotifyFromUIStateManagerGeneric( Event aEvent )
{
    APP_ENTRY_POINT_DATA_T *entryData = (APP_ENTRY_POINT_DATA_T*) aEvent.Data().data();

    //LOG_HIGH << "CAppAhaBase::NotifyFromUIStateManagerGeneric Start" << LOG_ENDL;
    LOG_HIGH<<"[CAppAhaBase::NotifyFromUIStateManagerGeneric] Event ID :"<<aEvent.GetEventId()<<LOG_ENDL;

    switch ( aEvent.GetEventId() )
    {
    //hsryu_0422_can_test
    case EVT_REQUEST_BG:
        //LOG_HIGH<<"EVT_REQUEST_BG"<<LOG_ENDL;
        LOG_HIGH <<"hsryu EVT_REQUEST_BG  m_bCallingState => " << m_bCallingState<<LOG_ENDL;
        //m_nPreviousPlayPause = CAppAhaTrack::getInstance()->TrackStatus();.
        m_eHMIState = CAppAhaBase::AppInBG;
//wsuk.kim 131016 ITS_195123 Temporal Mode        emit background();
        emit activeViewVisible(false);  //wsuk.kim 131016 ITS_195123 Temporal Mode
        emit handleHideErrorPopup();    // Ryu 20140227
        m_bIsBackGround = true;

        //hsryu_0404_restore_playmode
        if(m_bCallingState == true && bCheckInitOnce == false)
        {
            //LOG_HIGH <<"hsryu EVT_REQUEST_BG bCallingPlayState: " << bCallingPlayState<<LOG_ENDL;
            //bCallingPlayState = false;
            bCheckInitOnce = true;
        }

        if(isJogKeyLongPress())
        {
            emit handleSeekTrackReleased();
            setIsJogKeyLongPress(0);
        }

        //hsryu_0412_clear_OSD
        ClearOSD();
        // NotifyAVWidget();
        break;

    case EVT_REQUEST_PRE_FG:    //wsuk.kim 131016 ITS_195123 Temporal Mode
        LOG_HIGH<<"EVT_REQUEST_PRE_FG"<<LOG_ENDL;
        emit activeViewVisible(true);  //wsuk.kim 131016 ITS_195123 Temporal Mode
        if(entryData->sHeader.bTemporalMode == TRUE)
        {
            m_bContinueLastView = true;
        }
        else if(entryData->sHeader.bTemporalMode == FALSE)
        {
            m_bContinueLastView = false;
        }

//wsuk.kim 131106 temporal mode ON after launch sound setting, did not menu hide.
        if(m_bBeforeSoundSetting == true)
        {
            m_bContinueLastView = false;
        }
        m_bBeforeSoundSetting = false;
//wsuk.kim 131106 temporal mode ON after launch sound setting, did not menu hide.

        emit background();
        break;

    case EVT_REQUEST_FG:
        //LOG_HIGH <<"hsryu EVT_REQUEST_FG " << bCallingPlayState <<LOG_ENDL;
        LOG_HIGH <<"EVT_REQUEST_FG"<<LOG_ENDL;
        m_eHMIState = CAppAhaBase::AppInFG;
        //hsryu_0621_ITS_0174694
        ClearOSD();
        m_bIsBackGround = false;
        StopNotifyAVWidget();
        bCheckInitOnce = true; //hsryu_0404_restore_playmode
        m_connect_type_name = "";  //wsuk.kim 130807 ISV_86633 variant RearUSB

        //Tan, check the entry point
        EDeviceType selectedDevice;

        if(entryData->sAppEntryPoint == eAPP_AHARADIO_MODE_IPOD1)
        {
            selectedDevice =  DEVICE_TYPE_IPHONE_USB_FRONT;
//wsuk.kim 130827 update indicator text when to change language.
            if(GetVariantRearUSB()) //wsuk.kim 130807 ISV_86633 variant RearUSB
            {
                m_connect_type_name =  qApp->translate("main","STR_AHA_FRONT");
            }
//wsuk.kim 130827 update indicator text when to change language.
        }
        else if(entryData->sAppEntryPoint == eAPP_AHARADIO_MODE_IPOD2)
        {
            selectedDevice =  DEVICE_TYPE_IPHONE_USB_REAR;
//wsuk.kim 130827 update indicator text when to change language.
            if(GetVariantRearUSB()) //wsuk.kim 130807 ISV_86633 variant RearUSB
            {
                m_connect_type_name =  qApp->translate("main","STR_AHA_REAR");
            }
//wsuk.kim 130827 update indicator text when to change language.
        }
        else if(entryData->sAppEntryPoint == eAPP_AHARADIO_MODE_BT)
        {
            selectedDevice = DEVICE_TYPE_SMARTPHONE;
            m_connect_type_name =  qApp->translate("main","STR_AHA_BT_CONNECT");
        }
        else
        {
            selectedDevice =  DEVICE_TYPE_NONE;
            m_connect_type_name.clear();    //wsuk.kim 130827 update indicator text when to change language.
        }
        LOG_HIGH <<"selectedDevice: " << selectedDevice <<LOG_ENDL;
//        LOG_HIGH <<"m_CurrentAhaDevice: %d\n", m_CurrentAhaDevice);
//        LOG_HIGH <<"EVT_REQUEST_FG cluster[%d],  IsCalling[%d]\n",IsClusterUpdate(), IsCallingSameDevice());

        if(selectedDevice != m_CurrentAhaDevice && !getContinueLastView())  //wsuk.kim 131219 ITS_215989 Exception temporal mode, automatic reconnect on error view.
        {
            //let us reset all old data because we are switching to connect to new device.
            ConnectToTheOtherIpod((int)selectedDevice);
			//hsryu_0603_cluster_update
            if(IsClusterUpdate()&& !IsCallingSameDevice())
            {
                sendClearTpMessage();
            }
        }
        else
        {
            ResumeOnSameIpod();
        }

        emit RetranslateUi(); // ITS 228391

        break;
    case EVT_MMC_MOUNTED:
        LOG_HIGH<<"EVT_MMC_MOUNTED"<<LOG_ENDL;
        m_b_MMC_removed = false;
        break;
    case EVT_MMC_REMOVED:
        LOG_HIGH<<"EVT_MMC_REMOVED"<<LOG_ENDL;
        m_b_MMC_removed = true;
        break;
    case EVT_NOTIFY_AV_MODE:
        LOG_HIGH<<"EVT_NOTIFY_AV_MODE"<<LOG_ENDL;
        break;
    case EVT_CLONE_NOTIFICATION:
        LOG_HIGH<<"EVT_CLONE_NOTIFICATION"<<LOG_ENDL;
        //LOG_HIGH <<"NOTIFY : EVT_CLONE_NOTIFICATION \n");
        break;
    case EVT_SHOW_POPUP:
        LOG_HIGH<<"EVT_SHOW_POPUP"<<LOG_ENDL;
        if(isJogKeyLongPress())
        {
            emit handleSeekTrackReleased();
            setIsJogKeyLongPress(0);
        }
        break;
    default:
        LOG_HIGH<<"NOTIFY : default"<<LOG_ENDL;
        break;
    }
    LOG_HIGH << "CAppAhaBase::NotifyFromUIStateManagerGeneric EXIT" << LOG_ENDL;
}

void CAppAhaBase::ConnectToTheOtherIpod(int nSelectedDevice)
{
    LOG_HIGH << "CAppAhaBase::ConnectToTheOtherIpod to:" << nSelectedDevice << " current:"<<m_CurrentAhaDevice<<LOG_ENDL;

    bOSD_DISPLAY = true;  //hsryu_0621_ITS_0174694

    SetOSDFlag(true);
    OSDInfoConnectingtoaha();       // added by Ryu 20140127

    EDeviceType selectedDevice = (EDeviceType) nSelectedDevice;
    CAppAhaStationList::getInstance()->modelClear();


//    if(m_CurrentAhaDevice != DEVICE_TYPE_NONE && m_CurrentAhaDevice != -1)
    {
        LOG_HIGH <<"m_pSignals->getDeviceType() != DEVICE_TYPE_NONE"<<LOG_ENDL;

        //if(DEVICE_TYPE_IPHONE_USB_FRONT == selectedDevice || DEVICE_TYPE_IPHONE_USB_REAR == selectedDevice)
        if(DEVICE_TYPE_SMARTPHONE == nSelectedDevice &&
           (DEVICE_TYPE_IPHONE_USB_FRONT == m_CurrentAhaDevice || DEVICE_TYPE_IPHONE_USB_REAR == m_CurrentAhaDevice ) )
        {
            CAppAhaTrack::getInstance()->Pause();
            LOG_HIGH <<"CAppAhaBase::ConnectToTheOtherIpod 44444444444444444444"<<LOG_ENDL;

            Disconnect();
        }
        else if( DEVICE_TYPE_SMARTPHONE == m_CurrentAhaDevice &&
                (DEVICE_TYPE_IPHONE_USB_FRONT == nSelectedDevice || DEVICE_TYPE_IPHONE_USB_REAR == nSelectedDevice ) )
        {
            LOG_HIGH <<"CAppAhaBase::ConnectToTheOtherIpod 555555555555555555555"<<LOG_ENDL;
            Disconnect();
        }
    }

    m_pSignals->setDeviceType(selectedDevice);
    m_CurrentAhaDevice = selectedDevice;

    LOG_HIGH <<" m_CurrentAhaDevice : "<< m_CurrentAhaDevice <<LOG_ENDL;
    emit connectToIpod((int) selectedDevice);
}

void CAppAhaBase::ResumeOnSameIpod()
{
    LOG_HIGH << "CAppAhaBase::ResumeOnSameIpod" << LOG_ENDL;
    //TAN_PRINTF("CAppAhaBase::ResumeOnSameIpod \n");
    //LOG_HIGH <<"CAppAhaBase::ResumeOnSameIpod \n"<<LOG_ENDL;

    LOG_HIGH << "ResumeOnSameIpod m_bAhaConnected" << m_bAhaConnected <<LOG_ENDL;
    LOG_HIGH << "ResumeOnSameIpod m_bAV_Mode_Changed" << m_bAV_Mode_Changed <<LOG_ENDL;
    LOG_HIGH << "ResumeOnSameIpod m_CurrentAhaDevice" << m_CurrentAhaDevice <<LOG_ENDL;
    LOG_HIGH << "ResumeOnSameIpod getCurrentView()" << getCurrentView() <<LOG_ENDL;
    LOG_HIGH << "ResumeOnSameIpod m_NetworkStatusCode" << m_NetworkStatusCode <<LOG_ENDL;
    LOG_HIGH << "ResumeOnSameIpod m_bShowNoNetworkPopup" << m_bShowNoNetworkPopup <<LOG_ENDL;

    TAN_PRINTF("CAppAhaBase::ResumeOnSameIpod \n");
    // Aha is connected , Av mode has changed, not connecting view  -> Then play aha
    if(m_bAhaConnected == true
            && m_bAV_Mode_Changed == true
            && m_CurrentAhaDevice != DEVICE_TYPE_NONE
            && getCurrentView() != AHA_CURRENT_VIEW_CONNECTINGVIEW)
    {
        if(m_NetworkStatusCode != AHA_NOTIFY_RESUME_NORMAL && m_bShowNoNetworkPopup == false)/*AHA_NOTIFY_RESUME_NORMAL*/    //wsuk.kim 131204 ITS_212572 state network error, display error info on OSD.
        {
            //state network error view.
        }
        else
        {
            m_nPreviousPlayPause = AHA_PLAYBACK_PLAY;
            bOSD_DISPLAY = true;  //hsryu_0621_ITS_0174694
            NotifyAudioPathChanged();

            if(IsCallingSameDevice())   //wsuk.kim 130722 ITS_0180879 aha pause at siri mode after incoming call
            {
                CAppAhaTrack::getInstance()->Pause();
            }
            else //if(m_nPreviousPlayPause == AHA_PLAYBACK_PLAY)
            {
                m_bAV_Mode_Changed = false;
                openSoundChannel();
                //ITS 0230529: POWER ON --> MEDIA KEY --> Unmute request is ignored causing no sound issue.
                if((m_CurrentAvMode == MODE_STATE_AHA_RADIO_BT) &&
                        ((m_PrevAvMode == MODE_STATE_BT_AUDIO)||
                         (m_HeadUnitPowerState == HEADUNIT_POWER_OFF)) // prev mode was BT mode
                         ||
                         ((m_PrevAvMode == MODE_STATE_IPOD1_AUDIO || m_PrevAvMode == MODE_STATE_IPOD2_AUDIO) &&
                         (m_CurrentAvMode == MODE_STATE_AHA_RADIO_IPOD1 || m_CurrentAvMode == MODE_STATE_AHA_RADIO_IPOD2)) )                {
                    LOG_HIGH << "#### Delayed Play" <<LOG_ENDL;
                    DelayedPlay();
                }
                else
                {
                    CAppAhaTrack::getInstance()->Play();
                }
            }

//hsryu_0603_cluster_update
//        sendClearTpMessage();
//        SendClusterData();
//        sendCanTpMessage(0x48F , CAppAhaTrack::getInstance()->GetCurrentArtist());
        }
    }
    else
    {
        LOG_HIGH<<"ResumeOnSameIpod but do not openSoundChannel 2"<<LOG_ENDL;
    }

    m_bAV_Mode_Changed = false;
    emit foreground();
}

void CAppAhaBase::OnNoActiveStationSlot()
{
    LOG_HIGH <<"CAppAhaBase::OnNoActiveStationSlot jumpToPresets \n"<<LOG_ENDL;
    emit jumpToPresets();
}

/**
 *******************************************************************
 * The result of request from Aha MW
 * All failed requests related to network to be handled here only.
 *
 * @param[in] inRequestType The request for which the reply is
 *                          received
 * @param[in] inAhaError   The result of the request
 *
 * @return  None
 *
 *******************************************************************
 */

void CAppAhaBase::OnRequestCompleted(const ERequestType inRequestType,
                                     const EAhaError inAhaError)
{
    //LOG_SLOT<<"CAppAhaBase::OnRequestCompleted <-req:"<<inRequestType<<LOG_ENDL;
    LOG_HIGH <<"CAppAhaBase::OnRequestCompleted inRequestType:"<<inRequestType<<LOG_ENDL;

    //If the error is No Stations or No Active Stations,
    //even in that case Connect is Success
    int status = ((E_SUCCESS == inAhaError)
                  || (E_NO_STATIONS == inAhaError)
                  || (E_NO_ACTIVE_STATIONS == inAhaError)
                  || (E_SESSION_ALREADY_STARTED == inAhaError)
                  || (E_NETWORK_FAILED == inAhaError)   //wsuk.kim no_network
                  )? 1 :0 ;

    if(inAhaError == E_REQUEST_TIMEOUT)
    {
        LOG_HIGH <<"300suk ---------------E_REQUEST_TIMEOUT-----------\n"<<LOG_ENDL;
        //to do excpt error
    }

    LOG_HIGH<<"error = " << inAhaError << ", status = "<< status <<LOG_ENDL;

    switch (inRequestType)
    {
    case REQ_TYPE_PAUSE: // connect successful
    {
        //LOG_HIGH << "REQ_TYPE_PAUSE" << LOG_ENDL;
        //LOG_HIGH <<"REQ_TYPE_PAUSE \n");
        UpdateAhaAvailableStatus(true);
        emit connectionStatusChanged(status);
    }
        break;
    case REQ_TYPE_PLAY: // connect successful
    {
        //LOG_HIGH << "REQ_TYPE_PLAY" << LOG_ENDL;
        //LOG_HIGH <<"REQ_TYPE_PLAY \n");
        //LOG_HIGH<<"Successfully aha connected"<<LOG_ENDL;
        UpdateAhaAvailableStatus(true);
//        if(m_CurrentAvMode == MODE_STATE_AHA_RADIO_BT)
//        {
            setUnMuteForBTAha();
//        }

        emit connectionStatusChanged(status);
    }
        break;
    case REQ_TYPE_DISCONNECT: // disconnect
    {
        //LOG_HIGH << "REQ_TYPE_DISCONNECT" << LOG_ENDL;
        //LOG_HIGH <<"REQ_TYPE_DISCONNECT \n");
        //LOG_HIGH<<"REQ_TYPE_DISCONNECT"<<LOG_ENDL;
        UpdateAhaAvailableStatus(false); // aha disconnected .
        emit connectionStatusChanged(status);
    }
        break;
    case REQ_TYPE_CONNECT:
    {
        //LOG_HIGH << "REQ_TYPE_CONNECT" << LOG_ENDL;
        //LOG_HIGH <<"REQ_TYPE_CONNECT \n");
        if(status != 1)
        {
//wsuk.kim 131219 ITS_215989 Exception temporal mode, automatic reconnect on error view.    m_CurrentAhaDevice = DEVICE_TYPE_NONE;
            emit connectionStatusChanged(status);
            //hsryu_0603_cluster_update
            //SendClusterData_OpStateOnly();
        }
        else //hsryu_0621_ITS_0174694
        {
        	// ITS_0223424
            m_NetworkStatusCode = AHA_NOTIFY_RESUME_NORMAL;
            UpdateAhaAvailableStatus(true);
        }
//wsuk.kim no_network
        if(inAhaError == E_NETWORK_FAILED)
        {
            emit handleError(inAhaError);
        }
//wsuk.kim no_network
    }
        break;
    default:
    {
        LOG_CRITICAL<<"Default:"<<inRequestType<<LOG_ENDL;
        LOG_HIGH <<"Default \n"<<LOG_ENDL;
        break;
    }

    }
    if(inAhaError != E_SUCCESS)
    {
        HandleIfError(inAhaError);
    }


//wsuk.kim 130722 ITS_0180879 aha pause at siri mode after incoming call
//    //hsryu_0612_bt_call
//    if(m_bCallingState == true)
//    {
//        m_AhaInterface.pause();
//    }
//wsuk.kim 130722 ITS_0180879 aha pause at siri mode after incoming call
    //LOG_SLOT<<"CAppAhaBase::OnRequestCompleted ->"<<LOG_ENDL;

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
void CAppAhaBase::OnRPMAcquireResponseReceived(TAudioMode /*tAM*/ ,TAudioSink /*tAS*/)
{

}
#endif

/**
 *******************************************************************
 * Check the current status, request disconnect to RPM and Aha
 *
 * @param None
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppAhaBase::Disconnect()
{
    LOG_HIGH<<"CAppAhaBase::Disconnect() <-"<<LOG_ENDL;

    // Deinitialize the states
    ClearCache();

    LOG_HIGH<<"CAppAhaBase::Disconnect() ->"<<LOG_ENDL;
}

//hsryu_0613_fail_load_station
void CAppAhaBase::UpdateAhaForDisconnect()
{
    UpdateAhaAvailableStatus(false);
//    m_AhaInterface.m_AhaStateHandler.m_AhaActiveFrameTxRx->DestroyIpodAppControllerInstance(m_CurrentAhaDevice);
//    m_CurrentAhaDevice = DEVICE_TYPE_NONE;
}
/**
 *******************************************************************
 * Handle the error , if any error exists notify to qml layer
 *
 * @param[in] inAhaError error code
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppAhaBase::HandleIfError(const EAhaError inAhaError)
{
    if(E_SUCCESS != inAhaError)
    {
        LOG_CRITICAL<<"HandleIfError :: EAhaError :"<<inAhaError<<LOG_ENDL;
        switch(inAhaError)
        {
        case E_INVALID_STATE:
            //Programetical error
            LOG_CRITICAL<<"E_INVALID_STATE "<<LOG_ENDL;
            emit handleError(inAhaError);
            break;
        case E_NETWORK_FAILED:
            LOG_CRITICAL<<"Handle Network Failure.E_NETWORK_FAILED "<<LOG_ENDL;
            //ITS 0224240 no sound after resume playing ipod. Do not updateAhaAvailableStatus to false.
            //UpdateAhaAvailableStatus(false); // network error received .
            emit networkError();
            break;
        case E_SESSION_ALREADY_STARTED:
            LOG_CRITICAL<<"CAppAhaBase::HandleIfError"<<"E_SESSION_ALREADY_STARTED "<<LOG_ENDL;
            //If there is some thing need to be handled in case of SESSION Already started
            break;
        case E_REPEAT_BUFFERING:    //wsuk.kim 130904 ITS_0182092 repeat buffering
            emit bufferingError();
            break;
        default:
            if(inAhaError == E_INIT_FAILED || inAhaError == E_DEVICE_NOT_FOUND ||
                    inAhaError == E_BT_CONNECTION_FAILED || inAhaError == E_USB_CONNECTION_FAILED
                    || inAhaError == E_CHECK_AHA_APP_ON_DEVICE)
            {
                UpdateAhaAvailableStatus(false); // error received .
            }

            emit handleError(inAhaError);
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
void CAppAhaBase::UpdateAhaAvailableStatus(bool inStatus)
{
#ifdef FRAMEWORK_ENABLED

    m_bAhaConnected = false;
    AHARADIO_CONN_STATE_T state = AHARADIO_DISCONNECTED;
    if(inStatus)
    {
        state = AHARADIO_CONNECTED ; //not defined in UISH , once defined removed.
        m_bAhaConnected = true;
    }
    else
    {
        m_bOptionMenuVisible = false;
    }

    QByteArray status;
    status.append(state);
    Event event( EVT_AHA_STATUS, GetThisAppId(), APP_UI_MANAGER , status );
    if ( !NotifyUISH(event, DISPLAY_FRONT) )
    {
        LOG_CRITICAL<<"Event notification failed"<<LOG_ENDL;
    }

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
void CAppAhaBase::LaunchKeypadSettings()
{
#ifdef FRAMEWORK_ENABLED

    AppSettingsStartParameter startState;
    startState.startState = AppSettingsDef::eStart_Mode_General_Keyboard;
    Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_SETTINGS,
                     QByteArray((char *) &startState, sizeof(AppSettingsStartParameter)));

    NotifyUISH( postEvent , DISPLAY_FRONT );

    Event event( EVT_KEY_SOFT_SETTINGS, GetThisAppId(), APP_UI_MANAGER );
    emit NotifyUISH( event , DISPLAY_FRONT );

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
void CAppAhaBase::handleAVModeChange(MODE_STATE_T mode, bool nAhaIsConnected)
{
    LOG_HIGH <<"CAppAhaBase::handleAVModeChange mode: "<< mode <<LOG_ENDL;
    switch(mode)
    {
    case MODE_STATE_AHA_RADIO_IPOD1:
        LOG_HIGH <<"CAppAhaBase::handleAVModeChange - MODE_STATE_AHA_RADIO_IPOD1 \n"<<LOG_ENDL;
        m_bAV_Mode_Changed = false;
        //hsryu_0228 ITS # 0158377
        if(m_CurrentAhaDevice == DEVICE_TYPE_IPHONE_USB_FRONT && nAhaIsConnected)
        {
            if(m_bCallingState == false && m_NetworkStatusCode == AHA_NOTIFY_RESUME_NORMAL )   //wsuk.kim 131204 ITS_212572 state network error, display error info on OSD.
            {
                TAN_PRINTF("[handleAVModeChange] Play front \n");
                //m_SoundInterface.openSoundChannel(eIPOD_PORT_1);
                openSoundChannel();
                CAppAhaTrack::getInstance()->Play();
                NotifyAVWidget();
                requestOSDTimer(200); //hsryu_0411_change_track_info
                NotifyAudioPathChanged();
                sleep(1);       // no sound is fixed when USB audio <-> Aha changed rapidly
            }
            else    //wsuk.kim 131204 ITS_212572 state network error, display error info on OSD.
            {
                if(m_bShowNoNetworkPopup)
                {
                    openSoundChannel();
                    CAppAhaTrack::getInstance()->Play();
                    NotifyAVWidget();
                    NotifyAudioPathChanged();
                }
                bOSD_DISPLAY = true;
                SetOSDFlag(true);

                //ITS_0228611
                if(m_NetworkStatusCode == AHA_NOTIFY_CONNECTING)
                {
                    LOG_HIGH <<"CAppAhaBase::handleAVModeChange - MODE_STATE_AHA_RADIO_IPOD1 connecting...\n"<<LOG_ENDL;
                    OSDInfoConnectingtoaha();
                }
                else if(m_NetworkStatusCode == AHA_NOTIFY_SERVER_LOST || m_NetworkStatusCode == AHA_NOTIFY_SERVER_DOWN)
                {
                    LOG_HIGH <<"CAppAhaBase::handleAVModeChange - MODE_STATE_AHA_RADIO_IPOD1 net:"<<m_NetworkStatusCode<<LOG_ENDL;
                    OSDInfoNoNetwork();
                }
                else if(m_bCallingState == true)
                {
                    LOG_HIGH <<"CAppAhaBase::handleAVModeChange - MODE_STATE_AHA_RADIO_IPOD1 calling..."<<LOG_ENDL;
                    OSDTrackInfo();
                };
            }
        }
        else
        {
            ConnectToTheOtherIpod((int)DEVICE_TYPE_IPHONE_USB_FRONT);
        }
        bOSD_DISPLAY = true;  //hsryu_0621_ITS_0174694
        break;
    case MODE_STATE_AHA_RADIO_IPOD2:
        LOG_HIGH <<"CAppAhaBase::handleAVModeChange - MODE_STATE_AHA_RADIO_IPOD2 \n"<<LOG_ENDL;
        m_bAV_Mode_Changed = false;
        //hsryu_0228 ITS # 0158377
        if(m_CurrentAhaDevice == DEVICE_TYPE_IPHONE_USB_REAR && nAhaIsConnected)
        {
            if(m_bCallingState == false && m_NetworkStatusCode == AHA_NOTIFY_RESUME_NORMAL)   //wsuk.kim 131204 ITS_212572 state network error, display error info on OSD.
            {
                TAN_PRINTF("[handleAVModeChange] Play rear \n");
                //m_SoundInterface.openSoundChannel(eIPOD_PORT_2);
                openSoundChannel();
                CAppAhaTrack::getInstance()->Play();
                NotifyAVWidget();
                requestOSDTimer(200); //hsryu_0411_change_track_info
                NotifyAudioPathChanged();
                sleep(1);       // no sound is fixed when USB audio <-> Aha changed rapidly
            }
            else    //wsuk.kim 131204 ITS_212572 state network error, display error info on OSD.
            {
                if(m_bShowNoNetworkPopup)
                {
                    openSoundChannel();
                    CAppAhaTrack::getInstance()->Play();
                    NotifyAVWidget();
                    NotifyAudioPathChanged();
                }
                bOSD_DISPLAY = true;
                SetOSDFlag(true);

                //ITS_0228611
                if(m_NetworkStatusCode == AHA_NOTIFY_CONNECTING)
                {
                    LOG_HIGH <<"CAppAhaBase::handleAVModeChange - MODE_STATE_AHA_RADIO_IPOD2 connecting...\n"<<LOG_ENDL;
                    OSDInfoConnectingtoaha();
                }
                else if(m_NetworkStatusCode == AHA_NOTIFY_SERVER_LOST || m_NetworkStatusCode == AHA_NOTIFY_SERVER_DOWN)
                {
                    LOG_HIGH <<"CAppAhaBase::handleAVModeChange - MODE_STATE_AHA_RADIO_IPOD2 net:"<<m_NetworkStatusCode<<LOG_ENDL;
                    OSDInfoNoNetwork();
                }
                else if(m_bCallingState == true)
                {
                    LOG_HIGH <<"CAppAhaBase::handleAVModeChange - MODE_STATE_AHA_RADIO_IPOD2 calling..."<<LOG_ENDL;
                    OSDTrackInfo();
                }
            }
        }
        else
        {
            ConnectToTheOtherIpod((int)DEVICE_TYPE_IPHONE_USB_REAR);
        }
        bOSD_DISPLAY = true;  //hsryu_0621_ITS_0174694
        break;
    case MODE_STATE_AVOFF:
        // No break required here
        //hsryu_0603_cluster_update
        setClusterUpdate(false);
        m_bAV_Mode_Changed = true;
        //hsryu_0228 ITS # 0158377
        if(nAhaIsConnected)
        {
            //hsryu_0621_ITS_0174694
            //SetOSDFlag(true);
            //OSDInfoAVOff();            

            CAppAhaTrack::getInstance()->HandleAVModeChange();
            m_SoundInterface.closeSoundChannel();
            //hsryu_0422_can_testMODE_STATE_AHA_RADIO_BT
            isSoundOpen = false;
            StopNotifyAVWidget();
        }
        break;
    case MODE_STATE_AHA_RADIO_BT:    // modified ITS 226744, ITS 228112
        LOG_HIGH <<"CAppAhaBase::handleAVModeChange - MODE_STATE_AHA_RADIO_BT \n"<<LOG_ENDL;
        m_bAV_Mode_Changed = false;
        if(m_CurrentAhaDevice == DEVICE_TYPE_SMARTPHONE && nAhaIsConnected)
        {
            if(m_bCallingState == false && m_NetworkStatusCode == AHA_NOTIFY_RESUME_NORMAL)
            {
                TAN_PRINTF("[handleAVModeChange] Play front \n");
                openSoundChannel();
                CAppAhaTrack::getInstance()->Play();
                NotifyAVWidget();
                requestOSDTimer(200); //hsryu_0411_change_track_info
                NotifyAudioPathChanged();
                sleep(1);       // no sound is fixed when USB audio <-> Aha changed rapidly
            }
            else
            {
                if(m_bShowNoNetworkPopup)
                {
                    //openSoundChannel();
                    //CAppAhaTrack::getInstance()->Play(); //TODO: play -> stop..
                    NotifyAVWidget();
                    NotifyAudioPathChanged();
                }
                bOSD_DISPLAY = true;
                SetOSDFlag(true);

                //ITS_0228611
                if(m_NetworkStatusCode == AHA_NOTIFY_CONNECTING)
                {
                    LOG_HIGH <<"CAppAhaBase::handleAVModeChange - MODE_STATE_AHA_RADIO_BT connecting...\n"<<LOG_ENDL;
                    OSDInfoConnectingtoaha();
                }
                else if(m_NetworkStatusCode == AHA_NOTIFY_SERVER_LOST || m_NetworkStatusCode == AHA_NOTIFY_SERVER_DOWN)
                {
                    LOG_HIGH <<"CAppAhaBase::handleAVModeChange - MODE_STATE_AHA_RADIO_BT net:"<<m_NetworkStatusCode<<LOG_ENDL;
                    OSDInfoNoNetwork();
                }
                else if(m_bCallingState == true)
                {
                    LOG_HIGH <<"CAppAhaBase::handleAVModeChange - MODE_STATE_AHA_RADIO_BT calling..."<<LOG_ENDL;
                    OSDTrackInfo();
                }
            }
        }
        else
        {
            ConnectToTheOtherIpod((int)DEVICE_TYPE_SMARTPHONE);
        }
        break;
    default:
        LOG_HIGH <<"CAppAhaBase::handleAVModeChange - default \n"<<LOG_ENDL;
        //hsryu_0603_cluster_update
        setClusterUpdate(false);
        m_bAV_Mode_Changed = true;
        if(nAhaIsConnected)
        {
            //hsryu_0621_ITS_0174694
            LOG_HIGH <<"av mode change clear osd\n"<<LOG_ENDL;
            //ClearOSD(); // removed ITS 228608
            bOSD_DISPLAY = false; //hsryu_0621_ITS_0174694
            //CAppAhaTrack::getInstance()->HandleAVModeChange();
            //m_SoundInterface.closeSoundChannel();

            //hsryu_0422_can_test
            //isSoundOpen = false;
            StopNotifyAVWidget();
        }
        break;
    }
}

//wsuk.kim UNMUTE
void CAppAhaBase::handleNotifyAudioPath()
{
    if(m_bMuteState)
    {
        NotifyAudioPathChanged();
        openSoundChannel();
        LOG_HIGH <<"CAppAhaBase::handleNotifyAudioPath - openSoundChannel \n"<<LOG_ENDL;
    }
}
//wsuk.kim UNMUTE

//wsuk.kim no_network
void CAppAhaBase::onNetworkResume()
{
    emit networkErrorResume();
}

void CAppAhaBase::onNetworkStatus(const int nNetworkStatus)
{
    m_NetworkStatusCode = nNetworkStatus;
}
//wsuk.kim no_network

void CAppAhaBase::onUnmute()
{
    LOG_HIGH<<"CAppAhaBase::onUnmute() tried..."<<LOG_ENDL;
    setUnMuteForBTAha();
}

//wsuk.kim 130813 ITS_0182986 one by one text scroll on trackview
int CAppAhaBase::getStringWidth( const QString &fullString, const QString &fontFamily, int pixelSize )
{
    QFont mFont;
    mFont.setFamily(fontFamily);
    mFont.setPointSize(pixelSize);
    QFontMetrics fontMetrics(mFont);
    return fontMetrics.width(fullString, -1);
}
//wsuk.kim 130813 ITS_0182986 one by one text scroll on trackview

void CAppAhaBase::setCurrentView(int nCurrentView)
{
    m_nCurrentView = nCurrentView;
}

int CAppAhaBase::getCurrentView()
{
    return m_nCurrentView;
}

void CAppAhaBase::setSelectionEvent(bool bSelectionEvent)
{
    m_bSelectionEvent = bSelectionEvent;
}

void CAppAhaBase::NoNetworkPopupState(bool bShowPopup) //wsuk.kim 131204 ITS_212572 state network error, display error info on OSD.
{
    m_bShowNoNetworkPopup = bShowPopup;
}

// added by Ryu : stop timers at Trackview : ITS0224000
void CAppAhaBase::stopTimers()
{
    m_AhaInterface.m_AhaStateHandler.stopTimers();
}

void CAppAhaBase::DelayedPlay()
{
    if(m_DelayedPlayTimer != NULL)
        m_DelayedPlayTimer->start(2000);
}

void CAppAhaBase::onDelayedPlayTimeOut()
{
    if((MODE_STATE_AHA_RADIO_BT == m_CurrentAvMode) && (false == IsCallingSameDevice())
        ||
        ((m_PrevAvMode == MODE_STATE_IPOD1_AUDIO || m_PrevAvMode == MODE_STATE_IPOD2_AUDIO) &&
        (m_CurrentAvMode == MODE_STATE_AHA_RADIO_IPOD1 || m_CurrentAvMode == MODE_STATE_AHA_RADIO_IPOD2)) )
    {
        CAppAhaTrack::getInstance()->Play();
    }
}

void CAppAhaBase::setUnMuteForBTAha()
{
    EVENT_AUDIO_PATH_REQUEST_DATA requestData;
    if(m_bUnmuteSent == false && MODE_STATE_AHA_RADIO_BT == m_CurrentAvMode)
    {
        m_bUnmuteSent = true;

        requestData.avState = MODE_STATE_AHA_RADIO_BT;

        LOG_HIGH <<"### Unmute success \n"<<LOG_ENDL;

        Event event(EVT_REQUEST_AUDIO_PATH, APP_AHA_RADIO, APP_UI_MANAGER, QByteArray((char*)&requestData, sizeof(requestData)));

        if ( !NotifyUISHCommon(event) )
        {
            LOG_HIGH <<"CAppAhaBase::setUnMuteForBTAha: Event notification failed"<<LOG_ENDL;
        }
        else
        {
            LOG_HIGH<< "CAppAhaBase::setUnMuteForBTAha: Event notification sent" <<LOG_ENDL;
        }
    }
}

QString CAppAhaBase::getConnectTypeName()
{
    QString returnName;

    returnName.clear();

    switch(m_CurrentAhaDevice)
    {
    case DEVICE_TYPE_NONE:
        // do not clear m_connect_type_name
        break;
    case DEVICE_TYPE_SMARTPHONE:
        m_connect_type_name.clear();
        m_connect_type_name =  qApp->translate("main","STR_AHA_BT_CONNECT");
        break;
    case DEVICE_TYPE_IPHONE_USB_FRONT:
        m_connect_type_name.clear();
        m_connect_type_name =  qApp->translate("main","STR_AHA_FRONT");
        break;
    case DEVICE_TYPE_IPHONE_USB_REAR:
        m_connect_type_name.clear();
        m_connect_type_name =  qApp->translate("main","STR_AHA_REAR");
        break;
    }

    if(m_CurrentAhaDevice != DEVICE_TYPE_NONE)
    {
        if(GetVariantRearUSB() || m_CurrentAhaDevice == DEVICE_TYPE_SMARTPHONE)
        {
            returnName.append(" (");
            returnName.append(m_connect_type_name);     // (Front) / (Rear) / (Bluetooth)
            returnName.append(")");
        }
    }
    else
    {
        // ITS 228602  handling DEVICE_TYPE_NONE case for errorView
        if(m_connect_type_name.contains(qApp->translate("main","STR_AHA_BT_CONNECT")) || GetVariantRearUSB())
        {
            returnName.append(" (");
            returnName.append(m_connect_type_name);     // (Front) / (Rear) / (Bluetooth)
            returnName.append(")");
        }
    }

    return returnName;
}

//End Of File
