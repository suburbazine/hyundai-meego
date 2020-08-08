#include <DHAVN_AppStandBy_AppEngine.h>
#include <DHAVN_AppFramework_Event_Def.h>
#include <uistate_manager_def.h>
#include <DHAVN_AppFramework_AppEngineBase.h>
#include <DHAVN_AppSettings_Def.h>
#include <QDebug>
#include <QSettings>
#include <persistman.h>

/* VehicleVariant */
#define VEHICLE_VARIANT 0       //0x00: DH,  0x01: KH, 0x02: VI
/* Clock */
#define MOST_SYNC_TIMEOUT 60*1000

#define SETTINGS_FILE_DB "/app/data/app_settings/SettingsScreenSettings.db" // "/home/meego/.config/SettingsScreenSettings.db"
#define PRESSED 1
#define RELEASED 2
#define CANCELED 3

#define STANDBY_LOGO 4
#define STANDBY_MAIN 5
#define CLOCK_MAIN 6

//#define APP_STANDBY_CONFIG_FILE "/home/meego/.config/AppStandBy.ini"
//#define SETTINGS_KEY_POWER "KEY_POWER"
#define TIME_ZONE_FILE      "/app/data/app_settings/localtime"
#define TIME_ZONE_FILE_KOR  "/usr/share/zoneinfo/ROK"

/** MICOM Manager QtDBus Service name */
#define MICOM_MANAGER_SERVICE_NAME          "com.lge.car.micom"
#define MICOM_MANAGER_POWERMANAGEMENT_PATH  "/PowerManagement"
#define MICOM_MANAGER_POWERMANAGEMENT_INTERFACE "com.lge.car.micom.powermanagement"
#define MICOM_MANAGER_SIGNAL_POWER_KEY_EVENT "LgeDBus_EventPowerKey"

/** MOST **/
#define MOST_MANAGER_SERVICE_NAME           "com.dhavn.mostmanager.qtdbus"
#define MOST_MANAGER_HMI_PATH                "/hmi"
#define MOST_MANAGER_HMI_INTERFACE           ""

static QSharedMemory shmUISHInfo;

StandByAppEngine::StandByAppEngine( int &argc, char **argv, QObject *pParent )
   : AppVextEngineQML( false, "AppStandBy", pParent ),
    m_nCountryVariant( eCVInvalid )
{
    LW_LOG(  ">>>>>> StandByAppEngine() " );
    m_nCountryVariant = AppEngineBase::GetCountryVariant();
    m_nLanguage = AppEngineBase::GetLanguage();
    m_nVehicle = VEHICLE_VARIANT;
    LW_LOG("StandByAppEngine() - cv: " << m_nCountryVariant << ", language: " << m_nLanguage );
    m_pLoadStandBySetting = NULL;
    m_nFocusIndex = -1;     // -1: none,        0: standby main,        1: standby language,        2: standby else,    3: clock
    m_isNormalOn = false;           // NORMAL_ON 이벤트 수신 시 true, NORMAL_OFF 이벤트 수신시 false
    m_bResetTimer = false;          // auto disappear timr를 start 해야하는지 여부, NORMAL_ON 이벤트 수신 시 해당 값이 true면 auto disappear timer start
    m_receivedPostEvent = false;         // POST_EVENT 이벤트 수신 시 true
    bIsMainScreen = true;                    // 동의함 Main 화면 여부
    bEmergency = false;                      // SOS 이벤트 수신 여부
    m_isSwapDC = false;
    m_isFG = false;
    m_nAutoDisappear = true;            // default on. 2014.07,18 내수 정업 3차 업데이트 사양 변경
    m_isCamera = false;          // EVT_CAMERA_ENABLE 시 true, temporalMode 시 false, 사용안함, 삭제 필요
    m_toastPopupVisible = false;     // 언어 변경 팝업 출력 여부
    m_bIsGPSTimeSetting = true;      // GPS On/Off
    m_bIsGPSAvailable = false;       // Navi로부터 전달받은 시간이 유효할 경우 true, 사용안함, 삭제 필요
    m_bSummerTime = false;
    m_nPhotoFrame = 0;               // 대기화면 설정값. 0. Off, 1. clock
//   getSettingsFromDB();   // main.cpp

    m_iEvv = 0;

   /* Clock */
    m_bFirstClockFG = true;      // 대기화면 entry point 최초 수신 여부
    m_timer = new QTimer(this);
//   m_mostTimer->start(MOST_SYNC_TIMEOUT);     // 1sec timer 사용
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeoutTimer()));
    m_bLogicOff = false;     // eAPP_CLOCK_LOGIC_OFF_BLACK_MODE 수신 시 true
    m_pTimeFormat = 0;
    m_bSendMostTime = false;         // GPS Off > On 변경 시 MOST에 시간 정보 1회 전송위해 사용

   // WDT //
   signalConnection();
   setSignalHandlerObject(this);

   /* Beep */
   msg = QDBusMessage::createMethodCall("com.lge.car.micom", "/", "com.lge.car.micom.audioif", "ProcessAudioControlMessage");
   QList<QVariant> args;
   args.append( (quint16) 0x0115 );
   args.append( (quint8) 0x0 );
   msg.setArguments( args );

   //For black screen of PWR OFF in boot time.
   m_bPowerOff = false;             // FG 이벤트 eAPP_CLOCK_ENABLE_SETTINGS 수신 시
   m_isPowerStateOn = true;     // 부팅 직후 arg로 전달받은 값, MICOM으로부터 ChangedPowerState 수신 시
   m_isAgreeShowOsd = false;        // EVT_AGREE_SHOW_OSD 수신 시
   engineToQml.insert("IsBlackMode", QVariant(false));      // 기본 qml은 로고 화면, IsBlackMode true 시 로고 위에 black 그림
   LW_LOG( "StandByAppEngine() - IsBlackMode:: false");
   ConnectPowerManagement();

   shmUISHInfo.setKey("UISHStatusInformation");
   shmUISHInfo.attach(QSharedMemory::ReadOnly);

   HS_LOG(  "<<<<<< StandByAppEngine() " );
}

StandByAppEngine::~StandByAppEngine()
{
    LW_LOG(  ">>>>>> ~StandByAppEngine() " );
    shmUISHInfo.detach();
    LW_LOG(  "<<<<<< ~StandByAppEngine() " );
}

bool StandByAppEngine::GetSharedMemoryInfo(/*__IN */ int offset, /*__OUT */ unsigned char * pBuf, /* __IN */ int size)
{
    if ( !shmUISHInfo.isAttached() )
        {
            CR_LOG("Lost Connection with the shared memroy -> error code :" << shmUISHInfo.error());
            return false;
        }

        LW_LOG("load shared memory from shmUISHInfo");

        shmUISHInfo.lock();
        memcpy( pBuf, ((unsigned char *)shmUISHInfo.data())+offset, size );
        shmUISHInfo.unlock();

        return true;
}

void StandByAppEngine::playAudioBeep()
{
    LW_LOG( ">>>>>> playAudioBeep()" );
    /* Beep */
    QDBusConnection::sessionBus().call(msg, QDBus::NoBlock);
    LW_LOG( "<<<<<< playAudioBeep()" );
}

int StandByAppEngine::getCountryVariant()
{
    return m_nCountryVariant;
}

bool StandByAppEngine::getLogicOff()
{
    return m_bLogicOff;
}

void StandByAppEngine::sendTimeAndDateToMOST()
{
    LW_LOG( ">>>>>> sendTimeAndDateToMOST()" );
    QDBusInterface iFaceiHmi( MOST_MANAGER_SERVICE_NAME , MOST_MANAGER_HMI_PATH , MOST_MANAGER_HMI_INTERFACE , QDBusConnection::sessionBus());

    QDate date = QDate::currentDate();
    if ( date.year() < 2012 ) return;            // 2012년도 이전 값일 경우 전송 안함

    QTime time = QTime::currentTime();
    QDBusReply<bool> reply;
    QByteArray timeArray;
    unsigned int timeFormat = (unsigned int)m_pTimeFormat;
    timeArray.append( ( char )timeFormat );
    timeArray.append( ( char )time.hour() );
    timeArray.append( ( char )time.minute() );
    timeArray.append( ( char )time.second() );
    unsigned int dst = 0;
    if ( m_bSummerTime ) dst = 1;
//            if ( dst ) m_nSummerTime = 1;
//            else m_nSummerTime = 0;
    reply = iFaceiHmi.call("SetTime", timeArray, m_nAutoTimeZone,  dst );
    if (!reply) {
        CR_LOG("sendTimeAndDateToMOST: SetTime to MOST failed");
    }
    else {
        LW_LOG("sendTimeAndDateToMOST: SetTime to MOST");
    }

    static QDate previousDate ;
    QDate curDate = QDate::currentDate();
    if(previousDate != curDate)
    {
        previousDate = curDate;
        QDBusVariant systemDate;
        SDate sDate;
        sDate.year = (ushort)curDate.year();
        sDate.month = (uchar)curDate.month();
        sDate.day = (uchar)curDate.day();
        sDate.datetype = (uint)m_pTimeFormat;
        QVariant var;
        QDBusArgument vArg;
        vArg.beginStructure();
        vArg << sDate.year << sDate.month << sDate.day << sDate.datetype;
        vArg.endStructure();
        var.setValue(vArg);
        systemDate.setVariant(var);
        reply = iFaceiHmi.call("SetDate",QVariant::fromValue(systemDate));
        if (!reply) {
            CR_LOG("sendTimeAndDateToMOST: SetDate to MOST failed");
            previousDate = previousDate.addDays(-1);        // setDate fail 시 다시 setDate 하기 위해
        }
        else {
            LW_LOG("sendTimeAndDateToMOST: SetDate to MOST");
        }
    }
    LW_LOG( "<<<<<< sendTimeAndDateToMOST()" );
}

void StandByAppEngine::setIsPowerStateOn(bool is)
{
    LW_LOG( ">>>>>> setIsPowerStateOn()" );
    m_isPowerStateOn = is;

    if (m_isPowerStateOn) {
        engineToQml.insert("IsBlackMode", QVariant(false));
        HS_LOG( "-pst:1  -- show logo - m_isPowerStateOn:: " << m_isPowerStateOn );
    }
    else {
        if (m_nPhotoFrame==0) {
            engineToQml.insert("IsBlackMode", QVariant(true));
            HS_LOG( "-pst:0 and no clock, show black");
        }
        else {
            engineToQml.insert("IsBlackMode", QVariant(false));
            HS_LOG( "-pst:0 and clock, show logo");
        }
    }
    LW_LOG( "<<<<<< setIsPowerStateOn()" );
}

void StandByAppEngine::ConnectPowerManagement()
{
    LW_LOG( ">>>>>> ConnectPowerManagement()" );
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.connect(MICOM_MANAGER_SERVICE_NAME,
                                MICOM_MANAGER_POWERMANAGEMENT_PATH,
                                MICOM_MANAGER_POWERMANAGEMENT_INTERFACE,
                                MICOM_MANAGER_SIGNAL_POWER_KEY_EVENT,
                                this,
                                SLOT(ChangedPowerState(QDBusMessage)));
    LW_LOG( "<<<<<< ConnectPowerManagement()" );
}

void StandByAppEngine::ChangedPowerState(QDBusMessage message)
{
    LW_LOG( ">>>>>> ChangedPowerState()" );
    QList<QVariant> list;

    // Checking for the message arguments
    list = message.arguments();

    if ( list.isEmpty() ) {
        CR_LOG( "No arguments in LgeDBus_EventPowerKey Message !!!!" );
        return;
    }

    int mode = list.at( 0 ).toInt();
    HS_LOG( "[Micom LgeDBus_EventPowerKey] Message has Arguments, mode:" << mode );

    switch(mode)
    {
        case 0: //DEF_NORMAL_POWER_OFF = 0x00,
        case 16: //DEF_LOGIC_POWER_OFF  = 0x10,
            m_isPowerStateOn = false;
            HS_LOG( "Power State On flag is changed as: " << m_isPowerStateOn );
            if (!m_isAgreeShowOsd) {
                if ( m_nPhotoFrame == 0 ) {
                    engineToQml.insert("IsBlackMode", QVariant(true));
                    HS_LOG( "ChangedPowerState(DEF_LOGIC_POWER_OFF) - IsBlackMode:: true");
                }
                else {
                    HS_LOG( "ChangedPowerState(DEF_LOGIC_POWER_OFF) - display Clock");
                    engineToQml.insert("IsBlackMode", QVariant(false));
                }
            }
        break;
        case 1: //DEF_NORMAL_POWER_ON  = 0x01,
        case 17://DEF_LOGIC_POWER_ON:
            m_isPowerStateOn = true;
            HS_LOG( "Power State On flag is changed as: " << m_isPowerStateOn );
            if (!m_isAgreeShowOsd) {
                engineToQml.insert("IsBlackMode", QVariant(false));
                HS_LOG( "ChangedPowerState(DEF_NORMAL_POWER_ON) - IsBlackMode:: false");
            }
        break;
    }

    LW_LOG( "<<<<<< ChangedPowerState()" );
}

bool StandByAppEngine::getPowerOff()
{
    return m_bPowerOff;
}

void
StandByAppEngine::HandleBackKey()
{
    LW_LOG( ">>>>>> HandleBackKey() - displayId : " << m_nDisplay );
   Event event( EVT_KEY_SOFT_BACK, GetThisAppId(), APP_NONE );

   if ( !NotifyUISH( event, m_nDisplay == AppEngineEnum::APP_DISPLAY_FRONT ? DISPLAY_FRONT : DISPLAY_REAR  ) )
       HS_LOG( "HandleBackKey - Event notification failed" );
   LW_LOG( "<<<<<< HandleBackKey()" );
}

void StandByAppEngine::signalConnection()
{
    LW_LOG( ">>>>>> signalConnection()");
    struct sigaction act = {};

    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_flags |= SA_RESETHAND;
    act.sa_handler = StandByAppEngine::signalHandler;

    for( int signumber = SIGHUP; signumber < SIGUNUSED; signumber++ )
    {
        if( signumber == SIGKILL || signumber == SIGSTOP ) continue;
        if( sigaction ( signumber, &act, (struct sigaction *)NULL ) < 0)
        {
//            exit(1);
        }
    }
    LW_LOG( "<<<<<< signalConnection()" );
}

void StandByAppEngine::setSignalHandlerObject(StandByAppEngine *m)
{
    SigM = m;
}

void StandByAppEngine::signalHandler(int signum)
{
    CR_LOG( "signalHandler :: signum = %l" << signum );
    if ( signum == SIGCHLD ) return;
    SigM->restart( signum );
    raise(signum);
}


void StandByAppEngine::restart(int signum)
{
    HS_LOG( "[WDT] restart signum : " << signum );
    Event event( EVT_REQUEST_ABNORMAL_TERMINATION, GetThisAppId(), APP_UI_MANAGER, (EVT_COMMAND_T) signum );
    NotifyUISH( event, DISPLAY_FRONT );
//    exit(0);
}

void StandByAppEngine::callFunctionAfterBoot()
{
    LW_LOG( ">>>>>> callFunctionAfterBoot()");
    setGPSTime(m_bIsGPSTimeSetting) ;
    emit setAutoDisappear( m_nAutoDisappear );
    sendTimeAndDateToMOST();
    updateTimeZoneToSystem(false);
    emit afterBoot();
    m_timer->setInterval(1000);
    m_timer->start();
    LW_LOG( "<<<<<< callFunctionAfterBoot()");
}

void StandByAppEngine::getSettingsFromDB()
{
    LW_LOG( ">>>>>> getSettingsFromDB()" );
    if ( m_pLoadStandBySetting == NULL )
    {
        LW_LOG( "m_pLoadStandBySetting.reset()" );
        m_pLoadStandBySetting = new LoadStandBySetting();      // 물어보기
    }
//    m_nAutoDisappear = m_pLoadStandBySetting->LoadSetting(SettingsDef::DB_KEY_APPROVAL);
    m_bIsGPSTimeSetting = m_pLoadStandBySetting->LoadSetting(SettingsDef::DB_KEY_GPS_TIME_SETTINGS);
    m_nAutoTimeZone = m_pLoadStandBySetting->LoadSetting(SettingsDef::DB_KEY_AUTO_TIMEZONE);
    m_bSummerTime = m_pLoadStandBySetting->LoadSetting(SettingsDef::DB_KEY_SUMMER_TIME);
    m_pTimeFormat = m_pLoadStandBySetting->LoadSetting(SettingsDef::DB_KEY_TIME_TYPE);
    HS_LOG( "getSettingsFromDB() - m_nAutoDisappear: " << m_nAutoDisappear << " , m_bIsGPSTimeSetting : " << m_bIsGPSTimeSetting
            << " , m_nAutoTimeZone: " << m_nAutoTimeZone << ", m_bSummerTime : " << m_bSummerTime );
    LW_LOG( "<<<<<< getSettingsFromDB()" );
}

void StandByAppEngine :: getSharedMemoryFromUISH ()
{
    LW_LOG(  ">>>>>> getSharedMemoryFromUISH() " );
    GetSharedMemoryInfo((int)SHM_OFFSET_SETTING_VALUE, (unsigned char*)&m_nPhotoFrame, (int)SHM_SETTING_VALUE_SIZE);
    HS_LOG(  "<<<<<< m_nPhotoFrame: " << m_nPhotoFrame);
}

int StandByAppEngine::getPhotoFrame()
{
    return m_nPhotoFrame;
}

void StandByAppEngine::receiveGPSTime( EVENT_NAVI_GPS_TIME_DATA_T *data )
{
    LW_LOG( ">>>>>> receiveGPSTime()" );

    SYSTEMTIME NaviTime = data->systemTime;

    int timezone_mins = data->timezone;

    HS_LOG( "receiveGPSTime timezone: " << timezone_mins << ", Month = [" << NaviTime.wMonth << "] Day = [" << NaviTime.wDay
           << "] Hour = [" << NaviTime.wHour << "] Minute = [" << NaviTime.wMinute << "] Second = [" << NaviTime.wSecond << "]" );

    if( ( NaviTime.wHour != 0xFF ) || ( NaviTime.wMinute != 0xFF ) || ( NaviTime.wSecond != 0xFF ) )
    {
        m_bIsGPSAvailable = true;
        setTimeZone(timezone_mins);
        setRTCInfoToMicom( &NaviTime );

        if ( !m_bSendMostTime ) {
            m_bSendMostTime = true;
            sendTimeAndDateToMOST();
        }
    }
    else
    {
        m_bIsGPSAvailable = false;
        HS_LOG( "receiveGPSTime() - GPS Time is unavailable" );
    }
    LW_LOG(  "<<<<<< receiveGPSTime() " );
}

void StandByAppEngine::setRTCInfoToMicom( SYSTEMTIME *time )
{
    LW_LOG( ">>>>>> setRTCInfoToMicom()" );

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
        MicomRTCInterface.setTimeout(1000);
        MicomRTCInterface.call("LgeDBus_SetHwRTCSet", barray);
    }
    LW_LOG( "<<<<<< setRTCInfoToMicom()" );
}

void StandByAppEngine::setAutoTimeZone(int timezone )
{
    LW_LOG( ">>>>>> setAutoTimeZone() - timezone : " << timezone );
    // Compare to DB
    if(timezone != m_nAutoTimeZone)
    {
        m_nAutoTimeZone = timezone;
        updateTimeZoneToSystem(true);
        sendTimeAndDateToMOST();
    }
    LW_LOG( "<<<<<< setAutoTimeZone()" );
}

void StandByAppEngine::setTimeZone(int timezone)
{
    // Compare to DB
    if(timezone != m_nAutoTimeZone)
    {
        m_nAutoTimeZone = timezone;
        sendTimeAndDateToMOST();
        updateTimeZoneToSystem( true );
    }

}


// 최초 부팅 시 false, 그 외 (GPS timezone 변경 / 설정>Timezone 변경)에는 tiemzone 변경 시에만 true로 전달
void StandByAppEngine::updateTimeZoneToSystem(bool isShowToastPopup)
{
    LW_LOG( "updateTimeZoneToSystem() - m_bSummerTime: " << m_bSummerTime << " , m_nAutoTimeZone : " << m_nAutoTimeZone );

    QFile timezone_file (TIME_ZONE_FILE);
    bool result = timezone_file.remove();

//    if (m_nCountryVariant == eCVKorea)
//        setSummerTime(false);

    if (m_bSummerTime)
    {
        switch (m_nAutoTimeZone)
        {
        case -600: // Hawaii
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+9", TIME_ZONE_FILE);
            break;

        case -540: // Alaska
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+8", TIME_ZONE_FILE);
            break;

        case -480: // Pacific
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+7", TIME_ZONE_FILE);
            break;

        case -420: // Mountain
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+6", TIME_ZONE_FILE);
            break;

        case -360: // Central
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+5", TIME_ZONE_FILE);
            break;

        case -300: // Eastern
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+4", TIME_ZONE_FILE);
            break;

        case -240: // Atlantic
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+3", TIME_ZONE_FILE);
            break;

        case -210: // Newfoundland
            result = QFile::copy("/usr/share/zoneinfo/Canada/Newfoundland_DST", TIME_ZONE_FILE);
            break;

        case -180:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+2", TIME_ZONE_FILE);
            break;

        case -120:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+1", TIME_ZONE_FILE);
            break;

        case -60:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT", TIME_ZONE_FILE);
            break;

        case 0:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-1", TIME_ZONE_FILE);
            break;

        case 60:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-2", TIME_ZONE_FILE);
            break;

        case 120:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-3", TIME_ZONE_FILE);
            break;

        case 180:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-4", TIME_ZONE_FILE);
            break;

        case 240:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-5", TIME_ZONE_FILE);
            break;

        case 300:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-6", TIME_ZONE_FILE);
            break;

        case 360:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-7", TIME_ZONE_FILE);
            break;

        case 420:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-8", TIME_ZONE_FILE);
            break;

        case 480:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-9", TIME_ZONE_FILE);
            break;

        case 540:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-10", TIME_ZONE_FILE);
            break;
        }
    }
    else
    {
        switch (m_nAutoTimeZone)
        {
        case -600: // Hawaii
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+10", TIME_ZONE_FILE);
            break;

        case -540: // Alaska
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+9", TIME_ZONE_FILE);
            break;

        case -480: // Pacific
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+8", TIME_ZONE_FILE);
            break;

        case -420: // Mountain
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+7", TIME_ZONE_FILE);
            break;

        case -360: // Central
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+6", TIME_ZONE_FILE);
            break;

        case -300: // Eastern
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+5", TIME_ZONE_FILE);
            break;

        case -240: // Atlantic
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+4", TIME_ZONE_FILE);
            break;

        case -210: // Newfoundland
            result = QFile::copy("/usr/share/zoneinfo/Canada/Newfoundland", TIME_ZONE_FILE);
            break;

        case -180:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+3", TIME_ZONE_FILE);
            break;

        case -120:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+2", TIME_ZONE_FILE);
            break;

        case -60:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT+1", TIME_ZONE_FILE);
            break;

        case 0:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT", TIME_ZONE_FILE);
            break;

        case 60:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-1", TIME_ZONE_FILE);
            break;

        case 120:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-2", TIME_ZONE_FILE);
            break;

        case 180:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-3", TIME_ZONE_FILE);
            break;

        case 240:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-4", TIME_ZONE_FILE);
            break;

        case 300:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-5", TIME_ZONE_FILE);
            break;

        case 360:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-6", TIME_ZONE_FILE);
            break;

        case 420:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-7", TIME_ZONE_FILE);
            break;

        case 480:
            result = QFile::copy("/usr/share/zoneinfo/Etc/GMT-8", TIME_ZONE_FILE);
            break;

        case 540:
            result = QFile::copy(TIME_ZONE_FILE_KOR, TIME_ZONE_FILE);
            break;
        }
    }

    showTimeZoneChangedToastPopup(isShowToastPopup);
}

void StandByAppEngine::showTimeZoneChangedToastPopup(bool isShowPopup)
{
    if(!isShowPopup || ( m_nCountryVariant != eCVNorthAmerica && m_nCountryVariant != eCVCanada ) )
        return;

    HS_LOG( "showTimeZoneChangedToastPopup() - m_nAutoTimeZone : " << m_nAutoTimeZone << " , isShowPopup : " << isShowPopup );

    QString sTimeZone;
    switch (m_nAutoTimeZone)
    {
    case -600: // Hawaii
        sTimeZone = QApplication::translate("main","STR_SETTING_CLOCK_TIMEZONE_HAWAII");
        break;
    case -540: // Alaska
        sTimeZone = QApplication::translate("main","STR_SETTING_CLOCK_TIMEZONE_ALASKA");
        break;

    case -480: // Pacific
        sTimeZone = QApplication::translate("main","STR_SETTING_CLOCK_TIMEZONE_PACIFIC");
        break;

    case -420: // Mountain
        sTimeZone = QApplication::translate("main","STR_SETTING_CLOCK_TIMEZONE_MOUNTAIN");
        break;

    case -360: // Central
        sTimeZone = QApplication::translate("main","STR_SETTING_CLOCK_TIMEZONE_CENTRAL");
        break;

    case -300: // Eastern
        sTimeZone = QApplication::translate("main","STR_SETTING_CLOCK_TIMEZONE_EASTERN");
        break;

    case -240: // Atlantic
        sTimeZone = QApplication::translate("main","STR_SETTING_CLOCK_TIMEZONE_ATLANTIC");
        break;

    case -210: // Newfoundland
        sTimeZone = QApplication::translate("main","STR_SETTING_CLOCK_TIMEZONE_NEWFOUNDLAND");
        break;
    }

    EVENT_SYSTEM_POPUP_T a;
    memset(&a, 0x0, sizeof(EVENT_SYSTEM_POPUP_T));

    QString popupMSG;
    popupMSG = QApplication::translate("main","STR_SETTING_CLOCK_TIME_ZONE_CHANGED");
    LW_LOG( "showTimeZoneChangedToastPopup() - before Popup's String:" << popupMSG );
    popupMSG = popupMSG.replace("##", sTimeZone);
    HS_LOG( "showTimeZoneChangedToastPopup() - after Popup's String:" << popupMSG );


//    a.Popupshow.type = POPUP_TOAST;
//    popupMSG.toWCharArray(a.Popupshow.popup.toast.msg);
    a.Popupshow.type = POPUP_TEXT;
    popupMSG.toWCharArray(a.Popupshow.popup.text.msg);

    QString popupBtn("OK");
    a.Popupshow.popup.text.btn_count = 1;
    a.Popupshow.popup.text.btn[0] = POPUP_BUTTON_TYPE_TEXT;
    popupBtn.toWCharArray(a.Popupshow.popup.text.btn_txt[0]);

    a.Popupshow.timeout = 0x0fffffff;
//    a.Popupshow.timeout = 5000;

    a.Display = DISPLAY_FRONT;
    a.Popuptype = ePOPUP_NAVI_CHAGNE_TIME_ZONE;
//    a.Popupshow.source = APP_SETTINGS;
    a.Popupshow.source = APP_STANDBY;

    Event event( EVT_REQUEST_SYSTEM_POPUP, APP_STANDBY, APP_UI_MANAGER, QByteArray( (char *)&a, sizeof( EVENT_SYSTEM_POPUP_T ) ) );
//    Event event( EVT_REQUEST_SYSTEM_POPUP, APP_STANDBY, APP_UI_MANAGER, QByteArray::fromRawData((char *)&a, sizeof( EVENT_SYSTEM_POPUP_T ) ) );

    if ( !NotifyUISHCommon(event) )
    {
        HS_LOG( "showTimeZoneChangedToastPopup() - failed NotifyUISHCommon" );
    }
}

void StandByAppEngine::timeoutTimer()
{
    LW_LOG( "timeoutTimer()" );
    sendTimeoutToUpdateClock();     // 1초 timeout, qml/statusbar
    if ( m_nCntTimeout % 2 == 0 ) sendTimeoutToExternalClock();     // 2초 timeout, external clock
    if ( m_nCntTimeout % 60 == 0 ) {    // 60초 timeout, iBox
        sendTimeAndDateToMOST();
        m_nCntTimeout = 0;
    }
    m_nCntTimeout++;
}

void StandByAppEngine::SetLocalization( LANGUAGE_T languageId )
{
    HS_LOG( "SetLocalization()  - languageId: " << languageId  );
    if (m_nLanguage == languageId ) return;
    m_nLanguage = languageId;
   QString lang;

   switch ( languageId )
   {
      case LANGUAGE_KO:
         lang = "/app/share/AppStandBy/localization/StandBy_ko";
         break;

       case LANGUAGE_ES:
         lang =  "/app/share/AppStandBy/localization/StandBy_es-eu";
         break;

       case LANGUAGE_ES_NA:
         lang =  "/app/share/AppStandBy/localization/StandBy_es-na";
         break;

      case LANGUAGE_FR:
         lang =  "/app/share/AppStandBy/localization/StandBy_fr-eu";
         break;

       case LANGUAGE_FR_NA:
         lang =  "/app/share/AppStandBy/localization/StandBy_fr-na";
          break;

      case LANGUAGE_AR:
         lang = "/app/share/AppStandBy/localization/StandBy_ar";
         break;

      case LANGUAGE_CS:
         lang = "/app/share/AppStandBy/localization/StandBy_cs";
         break;

      case LANGUAGE_DA:
         lang = "/app/share/AppStandBy/localization/StandBy_da";
         break;

      case LANGUAGE_EN_GB:
         lang = "/app/share/AppStandBy/localization/StandBy_en-gb";
         break;

      case LANGUAGE_DE:
         lang = "/app/share/AppStandBy/localization/StandBy_de";
         break;

      case LANGUAGE_IT:
         lang = "/app/share/AppStandBy/localization/StandBy_it";
         break;

      case LANGUAGE_NL:
         lang = "/app/share/AppStandBy/localization/StandBy_nl";
         break;

      case LANGUAGE_PL:
         lang = "/app/share/AppStandBy/localization/StandBy_pl";
         break;

      case LANGUAGE_PT:
         lang = "/app/share/AppStandBy/localization/StandBy_pt";
         break;

      case LANGUAGE_RU:
         lang = "/app/share/AppStandBy/localization/StandBy_ru";
         break;

      case LANGUAGE_SK:
         lang = "/app/share/AppStandBy/localization/StandBy_sk";
         break;

      case LANGUAGE_SV:
         lang = "/app/share/AppStandBy/localization/StandBy_sv";
         break;

      case LANGUAGE_TR:
         lang = "/app/share/AppStandBy/localization/StandBy_tr";
         break;

      case LANGUAGE_ZH_CN:
         lang = "/app/share/AppStandBy/localization/StandBy_zh-cmn";
         break;

       case LANGUAGE_EN_US:
       default:
         lang = "/app/share/AppStandBy/localization/StandBy_en";
   }

   if ( m_pTranslator.load( lang ) )
   {
       LW_LOG( "m_nLanguage: " << m_nLanguage );
      qApp->installTranslator( &m_pTranslator );
      emit retranslateUI();
   }
}

void StandByAppEngine::NotifyFromUIStateManagerCommon( Event aEvent )
{
    LW_LOG( "EventID : " << aEvent.GetEventId() );

    switch ( aEvent.GetEventId() )
    {
        case EVT_POST_EVENT:
        {
             LW_LOG( "EVT_POST_EVENT" );
             if ( aEvent.GetCommand() == EVT_MP_DH_SET_READY ) {
            HS_LOG( "NotifyFromUIStateManagerCommon EVT_MP_DH_SET_READY"  );
                if ( m_isFG ) {
                    HS_LOG( "NotifyFromUIStateManagerCommon EVT_MP_DH_SET_READY(APP_STANDBY_FG) "  );
                    EVT_ID_T aEventID  = EVT_KEY_AGREE_PRESSED;
                    Event event( aEventID, GetThisAppId(), APP_UI_MANAGER );
                    NotifyUISHCommon( event  );
                }
            }
           break;
        }

        case EVT_POWER_STATE_NORMAL_OFF:
        case EVT_POWER_STATE_LOGIC_OFF:
        {
           HS_LOG( " EVT_POWER_STATE_NORMAL_OFF / EVT_POWER_STATE_LOGIC_OFF" );
           m_isNormalOn = false;
           m_bResetTimer = false;
        }
        break;
        case EVT_POWER_STATE_NORMAL_ON:
        case EVT_POWER_STATE_LOGIC_ON:
        {
            HS_LOG( " EVT_POWER_STATE_NORMAL_ON / EVT_POWER_STATE_LOGIC_ON" );
            m_isNormalOn = true;
            if (m_bResetTimer ) {
                m_bResetTimer = false;
                HS_LOG( "EVT_POWER_STATE_NORMAL_ON - emit signalStartTimer"  );
               emit signalAutoDisappearTimer( true  , m_nDisplay);
           }
        }
        break;

        case EVT_AGREE_SHOW_OSD:
        {
            m_isAgreeShowOsd = true;
            engineToQml.insert("IsBlackMode", QVariant(false));
            HS_LOG( "EVT_AGREE_SHOW_OSD - IsBlackMode:: false");
            break;
        }

//    case EVT_MP_DH_SET_READY:
//    {
//        HS_LOG( " ----EVT_MP_DH_SET_READY--- (APP_STANDBY_FG) - m_isFG : " << m_isFG  );
//        if ( m_isFG ) {
//            HS_LOG( " ----EVT_MP_DH_SET_READY--- (APP_STANDBY_FG) "  );
//            EVT_ID_T aEventID  = EVT_KEY_AGREE_PRESSED;
//            Event event( aEventID, GetThisAppId(), APP_UI_MANAGER );
//            NotifyUISHCommon( event  );
//        }
//        break;
//    }
        case EVT_SWAP_DC_ENABLED:
        {
            LW_LOG( "---EVT_SWAP_DC_ENABLED----" );
            m_isSwapDC = true;
            break;
        }
        case EVT_SWAP_DC_DISABLED:
        {
            LW_LOG( "---EVT_SWAP_DC_DISABLED----" );
            m_isSwapDC = false;
            break;
        }
        case EVT_LANGUAGE_CHANGED:
        {
         HS_LOG( "EVT_LANGUAGE_CHANGED" );
           EVENT_LANGUAGE_CHANGED_DATA_T *pLanguageData = (EVENT_LANGUAGE_CHANGED_DATA_T *)aEvent.Data().data();
           SetLocalization(pLanguageData->languageId);
           break;
        }
        case EVT_SETTINGS_CHANGED:
        {
            EVENT_SETTINGS_CHANGED_DATA_T * message = (EVENT_SETTINGS_CHANGED_DATA_T *) aEvent.Data().data() ;
            switch ( message->sKey )
            {
                case SettingsDef::DB_KEY_AUTO_TIMEZONE:
                {
                    HS_LOG( "EVT_SETTINGS_CHANGED - DB_KEY_AUTO_TIMEZONE : " << message->sValue );
                    setAutoTimeZone( message->sValue );
                }
                break;
                case SettingsDef::DB_KEY_TIME_TYPE:         // 12h / 24h
                case SettingsDef::DB_KEY_CLOCK_TYPE:        // analog / digital
                case SettingsDef::DB_KEY_PHOTO_FRAME:   // not see / clock / photo
                case SettingsDef::DB_KEY_SUMMER_TIME:
                case SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE:
                {
                    HS_LOG( "EVT_SETTINGS_CHANGED ===> key :  " << message->sKey << ",   value : " << message->sValue );
                    settingsChanged( message ) ;
                    if ( message->sKey == SettingsDef::DB_KEY_SUMMER_TIME ) {
                        m_bSummerTime = message->sValue;
                        sendTimeAndDateToMOST();
                    }
                    else if (message->sKey == SettingsDef::DB_KEY_TIME_TYPE ) {
                        m_pTimeFormat = message->sValue;
                        sendTimeAndDateToMOST();
                    }
//                    setFile( true );
                }
                break;
//                case SettingsDef::DB_KEY_APPROVAL:
//                HS_LOG( "EVT_SETTINGS_CHANGED - DB_KEY_APPROVAL - message->sValue :   " << message->sValue );
//                m_nAutoDisappear = message->sValue ;
//                emit setAutoDisappear( m_nAutoDisappear );
//                    break;
                case SettingsDef::DB_KEY_GPS_TIME_SETTINGS:
                    HS_LOG( "EVT_SETTINGS_CHANGED - DB_KEY_GPS_TIME_SETTINGS - message->sValue :   " << message->sValue );   // GPS: 1 , User: 0
                    m_bIsGPSTimeSetting = message->sValue;
                    if ( m_bIsGPSTimeSetting ) {
                        HS_LOG( "EVT_SETTINGS_CHANGED - DB_KEY_GPS_TIME_SETTINGS - request GPSTimeToNavi " );
                        m_bSendMostTime = false;
                        Event event(EVT_POST_EVENT, GetThisAppId(), APP_NAVIGATION, EVT_NAVI_GPS_TIME_GET );
                        NotifyUISH( event );
                    }
                    setGPSTime(m_bIsGPSTimeSetting) ;
                    break;
            }
            break;
        }
        case EVT_KEY_CCP_BACK:
        {
            HS_LOG( "EVT_KEY_CCP_BACK" );
            if ( m_isSwapDC )
                return;
            HandleBackLanguage();
//            emit backKeyPressed( m_nDisplay );
//            HandleBackLanguage();
            break;
        }
        case EVT_KEY_RRC_BACK:
        {
            HS_LOG( "EVT_KEY_RRC_BACK" );
            if ( !m_isSwapDC )
                   return;
            HandleBackLanguage();
//            emit backKeyPressed( m_nDisplay );
//            HandleBackLanguage();
            break;
        }
        case EVT_CAMERA_ENABLE:
        {
            HS_LOG( "EVT_CAMERA_ENABLE" );
            m_isCamera = true;
            emit signalAutoDisappearTimer(false , m_nDisplay );
            break;
        }
        case EVT_KEY_CCP_JOG_DIAL_UP:
        {
            if ( m_isSwapDC )
                return;
            KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
            emit signalJog( message->keyStatus , AppEngineQMLConstants::JOG_UP ,  AppEngineEnum::APP_DISPLAY_FRONT );
//            if (message->keyStatus == KEY_STATUS_PRESSED) {
//                emit signalJog( PRESSED , AppEngineQMLConstants::JOG_UP ,  AppEngineEnum::APP_DISPLAY_FRONT );
//            } else if (message->keyStatus == KEY_STATUS_RELEASED) {
//                emit signalJog( RELEASED , AppEngineQMLConstants::JOG_UP ,  AppEngineEnum::APP_DISPLAY_FRONT );
//            }
            break;
        }

        case EVT_KEY_CCP_JOG_DIAL_DOWN:
        {
            if ( m_isSwapDC )
                return;
            KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
            emit signalJog( message->keyStatus , AppEngineQMLConstants::JOG_DOWN ,  AppEngineEnum::APP_DISPLAY_FRONT );
//            if (message->keyStatus == KEY_STATUS_PRESSED) {
//                emit signalJog( PRESSED , AppEngineQMLConstants::JOG_DOWN ,  AppEngineEnum::APP_DISPLAY_FRONT );
//            } else if (message->keyStatus == KEY_STATUS_RELEASED) {
//                emit signalJog( RELEASED , AppEngineQMLConstants::JOG_DOWN ,  AppEngineEnum::APP_DISPLAY_FRONT );
//            }
            break;
        }

        case EVT_KEY_CCP_JOG_DIAL_LEFT:
        {
            if ( m_isSwapDC )
                return;
            KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
            emit signalJog( message->keyStatus , AppEngineQMLConstants::JOG_LEFT  ,  AppEngineEnum::APP_DISPLAY_FRONT );
//            if (message->keyStatus == KEY_STATUS_PRESSED) {
//                emit signalJog( PRESSED , AppEngineQMLConstants::JOG_LEFT  ,  AppEngineEnum::APP_DISPLAY_FRONT );
//            } else if (message->keyStatus == KEY_STATUS_RELEASED) {
//                emit signalJog( RELEASED , AppEngineQMLConstants::JOG_LEFT  ,  AppEngineEnum::APP_DISPLAY_FRONT );
//            }
            break;
        }

        case EVT_KEY_CCP_JOG_DIAL_RIGHT:
        {
            if ( m_isSwapDC )
                return;
            KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
            emit signalJog( message->keyStatus , AppEngineQMLConstants::JOG_RIGHT   ,  AppEngineEnum::APP_DISPLAY_FRONT );
//            if (message->keyStatus == KEY_STATUS_PRESSED) {
//                emit signalJog( PRESSED , AppEngineQMLConstants::JOG_RIGHT   ,  AppEngineEnum::APP_DISPLAY_FRONT );
//            } else if (message->keyStatus == KEY_STATUS_RELEASED) {
//                emit signalJog( RELEASED , AppEngineQMLConstants::JOG_RIGHT ,  AppEngineEnum::APP_DISPLAY_FRONT );
//            }
            break;
        }

        case EVT_KEY_CCP_JOG_DIAL_CENTER:
        {
            if ( m_isSwapDC )
                return;
            KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
            emit signalJog( message->keyStatus , AppEngineQMLConstants::JOG_CENTER ,  AppEngineEnum::APP_DISPLAY_FRONT );
            break;
        }
        case EVT_KEY_CCP_JOG_DIAL_ENCODER:
        {
            if ( m_isSwapDC )
                return;
            KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
            if ( (int)aEvent.GetCommand() > 0) {
                emit signalJog ( message->keyStatus , AppEngineQMLConstants::JOG_WHEEL_RIGHT ,  AppEngineEnum::APP_DISPLAY_FRONT );
            }
            else {
                emit signalJog ( message->keyStatus , AppEngineQMLConstants::JOG_WHEEL_LEFT ,  AppEngineEnum::APP_DISPLAY_FRONT );
            }
            break;
        }
    case EVT_KEY_RRC_JOG_DIAL_UP:
    {
        if ( !m_isSwapDC )
            return;
        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
        emit signalJog( message->keyStatus , AppEngineQMLConstants::JOG_UP  ,  AppEngineEnum::APP_DISPLAY_REAR );
        break;
    }

    case EVT_KEY_RRC_JOG_DIAL_DOWN:
    {
        if ( !m_isSwapDC )
            return;
        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
        emit signalJog( message->keyStatus , AppEngineQMLConstants::JOG_DOWN  ,  AppEngineEnum::APP_DISPLAY_REAR );
        break;
    }

    case EVT_KEY_RRC_JOG_DIAL_LEFT:
    {
        if ( !m_isSwapDC )
            return;
        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
        emit signalJog( message->keyStatus , AppEngineQMLConstants::JOG_LEFT  ,  AppEngineEnum::APP_DISPLAY_REAR );
        break;
    }

    case EVT_KEY_RRC_JOG_DIAL_RIGHT:
    {
        if ( !m_isSwapDC )
            return;
        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
        emit signalJog( message->keyStatus , AppEngineQMLConstants::JOG_RIGHT ,  AppEngineEnum::APP_DISPLAY_REAR );
        break;
    }

    case EVT_KEY_RRC_JOG_DIAL_CENTER:
    {
        if ( !m_isSwapDC )
            return;
        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
        emit signalJog( message->keyStatus , AppEngineQMLConstants::JOG_CENTER ,  AppEngineEnum::APP_DISPLAY_REAR );
        break;
    }
    case EVT_KEY_RRC_JOG_DIAL_ENCODER:
    {
        if ( !m_isSwapDC )
            return;
        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
        if ( (int)aEvent.GetCommand() > 0) {
            emit signalJog ( message->keyStatus , AppEngineQMLConstants::JOG_WHEEL_RIGHT ,  AppEngineEnum::APP_DISPLAY_REAR );
        }
        else {
            emit signalJog ( message->keyStatus , AppEngineQMLConstants::JOG_WHEEL_LEFT ,  AppEngineEnum::APP_DISPLAY_REAR );
        }
        break;
    }
    default: break;
    }
}

void StandByAppEngine::NotifyFromUIStateManagerRear( Event aEvent )
{
    LW_LOG( "NotifyFromUIStateManagerRear - EventID : " << aEvent.GetEventId() );
    switch ( aEvent.GetEventId() )
    {
       case EVT_REQUEST_PRE_FG:
       {
            m_nDisplay = AppEngineEnum::APP_DISPLAY_REAR;
            QDateTime	dateTime = QDateTime::currentDateTime();
//            emit signalFg();
//            emit setLaunchApp( false );
            APP_ENTRY_POINT_DATA_T* pData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();
            HS_LOG( "[TIME] EVT_REQUEST_FG(REAR) [" << dateTime.toString("hh:mm:ss.zzz") << "]" << " , sAppEntryPoint: " <<  pData->sAppEntryPoint);
            switch ( pData->sAppEntryPoint )
            {
                case eAPP_STANDBY_REAR:
                {
                    HS_LOG( "(Rear) :: eAPP_STANDBY_REAR " );
                    m_bPowerOff = false;
                    m_bLogicOff = false;
                    m_nFocusIndex = 2;
                    emit qmlLoad( true , m_nDisplay );      // standby of clock
                    emit setFocusIndex( m_nFocusIndex , m_nDisplay );
                    emit setRearLogo( true , m_nDisplay );  // logo or main
                }
                break;
                case eAPP_HOME_MAIN:
                case eAPP_STANDBY_FRONT:
                {
                HS_LOG( "(Rear) :: eAPP_HOME_MAIN/eAPP_STANDBY_FRONT : " << pData->sAppEntryPoint );
                    m_bPowerOff = false;
                    m_bLogicOff = false;
                    m_isFG = true;
                    if ( !m_receivedPostEvent ) {
                        m_nFocusIndex = 2;
                        emit qmlLoad( true  , m_nDisplay );
                        emit setFocusIndex( m_nFocusIndex , m_nDisplay );
                        emit setRearLogo( true , m_nDisplay );
                        return;
                    }
                    emit qmlLoad( true  , m_nDisplay );
                    emit signalFg();
                    emit setLaunchApp( false );
                    if ( pData->sHeader.bTemporalMode ) {
                        m_isCamera = false;
                        if ( bIsMainScreen ) {
                            m_nFocusIndex = 0;
                            emit setFocusIndex( m_nFocusIndex , m_nDisplay );
                        }
                        else {
                            m_nFocusIndex = 1;
                            emit setFocusIndex( m_nFocusIndex , m_nDisplay );
                        }
                    }
                    else {
                        bIsMainScreen = true;
                        m_nFocusIndex = 0;
                        emit signalSetDefaultFocus();
                        emit setFocusIndex( m_nFocusIndex , m_nDisplay );
                        emit signalSetLanguage(bIsMainScreen);
                    }
                    emit setRearLogo( false , m_nDisplay );
                    if ( bEmergency ) emit signalEmergency( bEmergency );
                    if(bIsMainScreen)
                    {
                        if ( m_nAutoDisappear ) {
                            if (m_isNormalOn ) {
                                HS_LOG( "(REAR) EVT_REQUEST_FG - emit signalStartTimer " );
                               emit signalAutoDisappearTimer( true  , m_nDisplay);
                           }
                           else {
                               HS_LOG( "( Rear ) EVT_REQUEST_FG - emit signalStartTimer - FALSE!! NormalOff!!"  );
                               m_bResetTimer = true;
                           }
                       }
                    }
                    QString strOSD = "@EVT^_AGREE";
                    OSDWriter::SendOSD(strOSD);
                    break;
                }
            case eAPP_CLOCK_ENABLE_SETTINGS:      /* Clock */
            {
                HS_LOG( "(REAR) EVT_REQUEST_FG : eAPP_CLOCK_ENABLE_SETTINGS"  );
                m_nFocusIndex = 3;
                emit setFocusIndex( m_nFocusIndex , m_nDisplay );
                m_bPowerOff = true;
                m_bLogicOff = false;
                emit signalAutoDisappearTimer( false , m_nDisplay );
                if ( m_bFirstClockFG )
                {
                    LW_LOG( "(REAR) EVT_REQUEST_FG : eAPP_CLOCK_ENABLE_SETTINGS -  first FG"  );
                    setFG();
                    m_bFirstClockFG = false;
                }
                setFile( true );
                appFG(true);
                emit qmlLoad( false  , m_nDisplay );
                break;
            }
            case eAPP_CLOCK_DISABLE_SETTINGS:     /* Clock */
            {
                HS_LOG( "(REAR) EVT_REQUEST_FG : eAPP_CLOCK_DISABLE_SETTINGS"  );
                m_nFocusIndex = 3;
                m_bPowerOff = false;
                m_bLogicOff = false;
                emit qmlLoad( false , m_nDisplay );
                emit signalAutoDisappearTimer( false  , m_nDisplay);
                m_bFirstClockFG = true;
                setFile( false );
                break;
            }
            case eAPP_CLOCK_BLACK_MODE:
            {
                HS_LOG( "(REAR) EVT_REQUEST_FG : eAPP_CLOCK_BLACK_MODE"  );
                m_nFocusIndex = 3;
                m_bPowerOff = false;
                m_bLogicOff = false;
                emit qmlLoad( false , m_nDisplay );
//                firstFG = true;
                setFile( false );
                break;
            }
            case eAPP_CLOCK_LOGIC_OFF_BLACK_MODE :
            {
                HS_LOG( "(REAR) EVT_REQUEST_FG : eAPP_CLOCK_LOGIC_OFF_BLACK_MODE"  );
                m_nFocusIndex = 3;
                m_bPowerOff = false;
                m_bLogicOff = true;
                emit qmlLoad( false , m_nDisplay );
                setFile( false );
                break;
            }
            default: HS_LOG( "(REAR) EVT_REQUEST_FG - entryPoint Error[" << pData->sAppEntryPoint << "]" ); break;
            }

            QDeclarativeView *pView = NULL;
            pView = GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
            pView->repaint();
            break;
       }
        case EVT_POST_EVENT:
        {
             HS_LOG( "( Rear ) :: EVT_POST_EVENT" );
             if ( aEvent.GetCommand() == COMMAND_APPS_LOADING_COMPLETED )
             {
                 m_receivedPostEvent = true;
                if ( m_bPowerOff ) {
                    HS_LOG( "( Rear ) EVT_POST_EVENT - PowerOff.... return!!! ");
                    return;
                }
//                emit signalLoadingCompleted();
                if( bIsMainScreen && m_isFG )
                {
                    if ( m_nAutoDisappear ) {
                        if (m_isNormalOn ) {
                            HS_LOG( "( Rear ) :: EVT_POST_EVENT - emit signalStartTimer" );
                           emit signalAutoDisappearTimer( true  , m_nDisplay);
                       }
                       else {
                           HS_LOG( "( Rear ) :: EVT_POST_EVENT - emit signalStartTimer - FALSE!! NormalOff!!"  );
                           m_bResetTimer = true;
                       }
                   }
                }
             }
             else if ( aEvent.GetCommand() == EVT_MP_DH_SET_READY ) {
                 HS_LOG( "( Rear ) ::  ----EVT_MP_DH_SET_READY--- "  );
                 if ( m_isFG ) {
                     HS_LOG( "( Rear ) ::  ----EVT_MP_DH_SET_READY--- (APP_STANDBY_FG) "  );
                     EVT_ID_T aEventID  = EVT_KEY_AGREE_PRESSED;
                     Event event( aEventID, GetThisAppId(), APP_UI_MANAGER );
                     NotifyUISHCommon( event  );
                 }
             }
           break;
        }
       case EVT_REQUEST_BG:
       {
        HS_LOG( "(REAR) EVT_REQUEST_BG" );
        m_isFG = false;
          break;
       }
    default: break;
     }
}

void StandByAppEngine::NotifyFromUIStateManager( Event aEvent )
{
   LW_LOG( "EventID : " << aEvent.GetEventId() );
   switch ( aEvent.GetEventId() )
   {
       case EVT_EMERGENCY_ACTIVATED:        // 213 (213/2/22/0)
       {
           HS_LOG( "EVT_EMERGENCY_ACTIVATED");
           bEmergency = true;
           emit signalEmergency( bEmergency );
           if( !bIsMainScreen )
           {
               bIsMainScreen = true;
               m_nFocusIndex = 0;
               emit setFocusIndex( m_nFocusIndex , m_nDisplay );
               emit signalSetLanguage(bIsMainScreen);
               if ( m_nAutoDisappear ) {
                   if (m_isNormalOn ) {
                       HS_LOG( "EVT_EMERGENCY_ACTIVATED - emit signalStartTimer "  );
                      emit signalAutoDisappearTimer( true  , m_nDisplay);
                  }
                  else {
                      HS_LOG( "EVT_EMERGENCY_ACTIVATED - emit signalStartTimer - FALSE!! NormalOff!!"  );
                      m_bResetTimer = true;
                  }
              }
           }
           break;
       }
       case EVT_EMERGENCY_DEACTIVATED:      //214
       {
           HS_LOG( "EVT_EMERGENCY_DEACTIVATED");
           bEmergency = false;
           emit signalEmergency( bEmergency );
           emit signalLoadingCompleted();
           break;
       }
      case EVT_REQUEST_PRE_FG:
      {
           m_nDisplay = AppEngineEnum::APP_DISPLAY_FRONT;
           QDateTime	dateTime = QDateTime::currentDateTime();
           APP_ENTRY_POINT_DATA_T* pData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();
           HS_LOG( "[TIME] EVT_REQUEST_FG m_nLanguage: " << m_nLanguage << " [ " << dateTime.toString("hh:mm:ss.zzz") << "]"  << " , sAppEntryPoint: " << pData->sAppEntryPoint );
//           if (!m_isAgreeShowOsd) {
//                engineToQml.insert("IsBlackMode", QVariant(!m_isPowerStateOn));
//                HS_LOG( "Show black screen?" << !m_isPowerStateOn );
//           }

           switch ( pData->sAppEntryPoint )
           {
               case eAPP_STANDBY_REAR:
               {
                   HS_LOG( "EVT_REQUEST_FG :: eAPP_STANDBY_REAR "   );
                   m_bPowerOff = false;
                   m_bLogicOff = false;
                   m_nFocusIndex = 2;
                   emit qmlLoad( true  , m_nDisplay );
                   emit setFocusIndex( m_nFocusIndex , m_nDisplay );
                   emit setRearLogo( true , m_nDisplay );
               }
               break;
               case eAPP_HOME_MAIN:
               case eAPP_STANDBY_FRONT:
               {
                   HS_LOG( " EVT_REQUEST_FG :: pData->sAppEntryPoint: " << pData->sAppEntryPoint << " , bTemporalMode: " << pData->sHeader.bTemporalMode );
                   m_bPowerOff = false;
                   m_bLogicOff = false;
                   m_isFG = true;
                   if ( !m_receivedPostEvent ) {
                       HS_LOG( " EVT_REQUEST_FG :: setRearLogo" );
                       m_nFocusIndex = 2;
                       emit qmlLoad( true  , m_nDisplay );
                       emit setFocusIndex( m_nFocusIndex , m_nDisplay );
                       emit setRearLogo( true , m_nDisplay );
                       return;
                   }
                   emit qmlLoad( true  , m_nDisplay );
                   emit signalFg();
                   emit setLaunchApp( false );
                   if ( pData->sHeader.bTemporalMode ) {
                       m_isCamera = false;
                       if ( bIsMainScreen ) {
                           m_nFocusIndex = 0;
                           emit setFocusIndex( m_nFocusIndex , m_nDisplay );
                       }
                       else {
                           m_nFocusIndex = 1;
                           emit setFocusIndex( m_nFocusIndex , m_nDisplay );
                           emit signalFg();
                       }
                   }
                   else {
                       bIsMainScreen = true;
                       m_nFocusIndex = 0;
                       emit signalSetDefaultFocus();
                       emit setFocusIndex( m_nFocusIndex , m_nDisplay );
                       emit signalSetLanguage(bIsMainScreen);
                   }
                   emit setRearLogo( false , m_nDisplay );
                   if ( bEmergency ) emit signalEmergency(bEmergency);
                   if(bIsMainScreen)
                   {
                       if ( m_nAutoDisappear ) {
                           if (m_isNormalOn ) {
                               HS_LOG( "EVT_REQUEST_FG - emit signalStartTimer"  );
                              emit signalAutoDisappearTimer( true  , m_nDisplay);
                          }
                          else {
                              HS_LOG( "EVT_REQUEST_FG - emit signalStartTimer - FALSE!! NormalOff!!"  );
                              m_bResetTimer = true;
                          }
                      }
                   }
                   QString strOSD = "@EVT^_AGREE";
                   OSDWriter::SendOSD(strOSD);
                   break;
               }
                case eAPP_CLOCK_ENABLE_SETTINGS:      /* Clock */
               {
                   HS_LOG( "EVT_REQUEST_FG : eAPP_CLOCK_ENABLE_SETTINGS"  );
                   m_nFocusIndex = 3;
                   m_bPowerOff = true;
                   m_bLogicOff = false;
                   emit qmlLoad( false  , m_nDisplay );
                   emit signalAutoDisappearTimer( false , m_nDisplay );
                   if ( m_bFirstClockFG )
                   {
                       HS_LOG( "EVT_REQUEST_FG : eAPP_CLOCK_ENABLE_SETTINGS -  first FG"  );
                       setFG();
                       m_bFirstClockFG = false;
                   }
                   setFile( true );
                   appFG(true);
                   emit qmlLoad( false  , m_nDisplay );
                   break;
               }
               case eAPP_CLOCK_DISABLE_SETTINGS:     /* Clock */
               {
                   HS_LOG( "EVT_REQUEST_FG : eAPP_CLOCK_DISABLE_SETTINGS"  );
                   m_nFocusIndex = 3;
                   m_bPowerOff = false;
                   m_bLogicOff = false;
                   emit signalAutoDisappearTimer( false  , m_nDisplay);
                   emit qmlLoad( false  , m_nDisplay );
                   m_bFirstClockFG = true;
                   setFile( false );
                   break;
               }
               case eAPP_CLOCK_BLACK_MODE:
               {
                   HS_LOG( " EVT_REQUEST_FG : eAPP_CLOCK_BLACK_MODE"  );
                   m_nFocusIndex = 3;
                   m_bPowerOff = false;
                   m_bLogicOff = false;
                   emit qmlLoad( false , m_nDisplay );
//                   firstFG = true;
                   setFile( false );
                   break;
               }
               case eAPP_CLOCK_LOGIC_OFF_BLACK_MODE :
               {
                   HS_LOG( " EVT_REQUEST_FG : eAPP_CLOCK_LOGIC_OFF_BLACK_MODE"  );
                   m_nFocusIndex = 3;
                   m_bPowerOff = false;
                   m_bLogicOff = true;
                   emit qmlLoad( false , m_nDisplay );
                   setFile( false );
                   break;
               }
           default: HS_LOG( "EVT_REQUEST_FG - entryPoint Error[" << pData->sAppEntryPoint << "]" ); break;
           }

           QDeclarativeView *pView = NULL;
           pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
           pView->repaint();
       }
       break;
       case EVT_POST_EVENT:
       {
          if ( aEvent.GetCommand() == COMMAND_APPS_LOADING_COMPLETED )
          {
              HS_LOG( " EVT_POST_EVENT - COMMAND_APPS_LOADING_COMPLETED"  );
              m_receivedPostEvent = true;
              if ( m_bPowerOff ) {
                  HS_LOG( "EVT_POST_EVENT - PowerOff.... return!!! ");
                  return;
              }
              m_nFocusIndex = 0;
              emit qmlLoad( true , m_nDisplay );
              emit setFocusIndex( m_nFocusIndex , m_nDisplay );
              emit setRearLogo( false , m_nDisplay );
//             emit signalLoadingCompleted();
             if( bIsMainScreen && m_isFG )
             {
                 if ( m_nAutoDisappear) {
                     if (m_isNormalOn ) {
                         HS_LOG( "EVT_POST_EVENT - emit signalStartTimer");
                        emit signalAutoDisappearTimer( true  , m_nDisplay);
                    }
                    else {
                        HS_LOG( "EVT_POST_EVENT - emit signalStartTimer - FALSE!! NormalOff!!"  );
                        m_bResetTimer = true;
                    }
                }
             }
          }
          else if ( aEvent.GetCommand() == EVT_MP_DH_SET_READY ) {
              HS_LOG( " EVT_POST_EVENT - EVT_MP_DH_SET_READY"  );
              if ( m_isFG ) {
                  HS_LOG( " EVT_POST_EVENT - EVT_MP_DH_SET_READY (APP_STANDBY_FG)"  );
                  EVT_ID_T aEventID  = EVT_KEY_AGREE_PRESSED;
                  Event event( aEventID, GetThisAppId(), APP_UI_MANAGER );
                  NotifyUISHCommon( event  );
              }
          }
          else if ( aEvent.GetSource() == APP_NAVIGATION )
          {
              if ( aEvent.GetCommand() == EVT_NAVI_GPS_TIME_SET && m_bIsGPSTimeSetting == true )
              {
                  EVENT_NAVI_GPS_TIME_DATA_T *GPSTimeFromNaviApp = (EVENT_NAVI_GPS_TIME_DATA_T *)aEvent.Data().data();
                  if ( GPSTimeFromNaviApp )
                  {
                      receiveGPSTime( GPSTimeFromNaviApp );
                  }
              }
          }
          break;
       }
      case EVT_REQUEST_BG:
      {
       HS_LOG( "EVT_REQUEST_BG" );
       m_isFG = false;
         emit signalAutoDisappearTimer( false , m_nDisplay );
         break;
      }
   default: break;
   }
}

void
StandByAppEngine::ChangeLanguage(int language)
{
    HS_LOG( "languageID : " << language );

    EVENT_LANGUAGE_CHANGED_DATA_T message;
    message.header.appIdReciever = APP_NONE;
    message.header.reasonEvent = EVT_LANGUAGE_CHANGED;
    message.languageId = (LANGUAGE_T)language;

//    SetLocalization((LANGUAGE_T)language);

    QByteArray temp( (char *)&message, sizeof( message ) );
    Event event( EVT_LANGUAGE_CHANGED, GetThisAppId(), APP_ALL, temp );
    //Event event( EVT_LANGUAGE_CHANGED, GetThisAppId(), APP_ALL, QByteArray::fromRawData( (char *)&message, sizeof( message ) ) );

    if ( !NotifyUISH(event, DISPLAY_FRONT) )
    {
        HS_LOG("EVT_LANGUAGE_CHANGED notification failed");
    }
    
    // iBox에 language 전달
    QString iboxLangId;
    switch ( (LANGUAGE_T)language )
    {
    case LANGUAGE_EN:
    case LANGUAGE_EN_US:
        iboxLangId = "EN1";
        break;
    case LANGUAGE_KO:
        iboxLangId = "KO0";
        break;
    case LANGUAGE_ZH_YUE:
    case LANGUAGE_ZH_CN:
        iboxLangId = "ZH1";
        break;
    case LANGUAGE_PT:
        iboxLangId = "PT0";
        break;
    case LANGUAGE_EN_GB:
        iboxLangId = "EN2";
        break;
    case LANGUAGE_FR:
        iboxLangId = "FR0";
        break;
    case LANGUAGE_IT:
        iboxLangId = "IT0";
        break;
    case LANGUAGE_DE:
        iboxLangId = "DE0";
        break;
    case LANGUAGE_ES:
        iboxLangId = "ES0";
        break;
    case LANGUAGE_RU:
        iboxLangId = "RU0";
        break;
    case LANGUAGE_NL:
        iboxLangId = "NL0";
        break;
    case LANGUAGE_SV:
        iboxLangId = "SV0";
        break;
    case LANGUAGE_PL:
        iboxLangId = "PL0";
        break;
    case LANGUAGE_TR:
        iboxLangId = "TR0";
        break;
    case LANGUAGE_CS:
        iboxLangId = "CS0";
        break;
    case LANGUAGE_DA:
        iboxLangId = "DA0";
        break;
    case LANGUAGE_SK:
        iboxLangId = "SK0";
        break;
    case LANGUAGE_AR:
        iboxLangId = "AR0";
        break;
    case LANGUAGE_FR_NA:
        iboxLangId = "FR0";
        break;
    case LANGUAGE_ES_NA:
        iboxLangId = "ES0";
        break;
    default:
        return ;
    }
    QDBusInterface iFaceiHmi(MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, QDBusConnection::sessionBus());
    QDBusReply<bool> reply = iFaceiHmi.call("SetLanguage", iboxLangId );
    if (!reply) {
        HS_LOG("SetLanguage to MOST failed");
    }
    else {
        HS_LOG("SetLanguage to MOST");
    }
}

void StandByAppEngine::setTime( bool pressed)
{
    QDateTime	dateTime = QDateTime::currentDateTime();
    LW_LOG( "[TIME] button pressed: "<< pressed << " , " << dateTime.toString("hh:mm:ss.zzz") );
}

void StandByAppEngine::LaunchApplication( int appId, int nDisplay )
{
    LW_LOG( "appID: " << appId << " , nDisplay : " << nDisplay );
   EVT_ID_T aEventID = EVT_NONE;
   emit setLaunchApp( true );
   emit signalAutoDisappearTimer( false , m_nDisplay );

   switch ( appId )
   {
      case StandByAppConstants::BUTTON_AGREE:
         aEventID = EVT_KEY_AGREE_PRESSED;
         break;

      case StandByAppConstants::BUTTON_LANGUAGE:
      {
//         aEventID = EVT_KEY_SOFT_SETTINGS;

//         AppSettingsStartParameter param;
//         param.startState = AppSettingsDef::eStart_Mode_General_Language_only;
//         Event event( EVT_POST_EVENT, GetThisAppId(),
//                      APP_SETTINGS, QByteArray::fromRawData( (char*)&param, sizeof( param ) ) );
//         NotifyUISHCommon( event );
       HS_LOG( " LaunchApplication - BUTTON_LANGUAGE" );
       bIsMainScreen = false;
       m_nFocusIndex = 1;
       emit setFocusIndex( m_nFocusIndex , m_nDisplay );
       emit signalSetLanguage(bIsMainScreen);
         break;
      }

      case StandByAppConstants::BUTTON_AGREEMENT:
         break;

      case StandByAppConstants::BUTTON_USER_GUIDE:
   {
         aEventID = EVT_INFO_INTERACTIVE_GUIDE;
    }
       break;
      default: break;
   }

   if( EVT_NONE != aEventID )
   {
      Event event( aEventID, GetThisAppId(), APP_UI_MANAGER );
      Event eventCam( EVT_POST_EVENT, GetThisAppId(), APP_CAMERA, aEventID);

      if(aEventID == EVT_KEY_AGREE_PRESSED)
      {
          NotifyUISHCommon( event  );
          NotifyUISH (eventCam, DISPLAY_FRONT);
          HS_LOG( "NofityUISH EVT_KEY_AGREE_PRESSED" );
      }
//      else if(aEventID == EVT_INFO_INTERACTIVE_GUIDE)
//      {
//          event.SetCommand( 240 );
//          HS_LOG( "NofityUISH EVT_INFO_INTERACTIVE_GUIDE- eventID : " << aEventID  );
//          NotifyUISH( event, m_isSwapDC ? DISPLAY_REAR : DISPLAY_FRONT);
//          NotifyUISH (eventCam,   DISPLAY_FRONT);
//          LW_LOG( "EVT_INFO_INTERACTIVE_GUIDE"  );
//      }
   }
}

void StandByAppEngine::showToastPopup( bool visible )
{
//    HS_LOG( "showToastPopup() - visible : " << visible );
    m_toastPopupVisible = visible;
}

void StandByAppEngine::HandleBackLanguage()
{
    if ( m_nFocusIndex != 1 ) return;
    HS_LOG( "HandleBackLanguage()" );
    if ( m_toastPopupVisible ) {
        HS_LOG( "HandleBackLanguage() - Not available!! Toast Popup Visible!! " );
        return;
    }
//    if( !bIsMainScreen )
    {
        bIsMainScreen = true;
        emit setLaunchApp( false );
        LW_LOG( "HandleBackLanguage() - emit signalStartTimer - m_nAutoDisappear : " << m_nAutoDisappear );
        if ( m_nAutoDisappear ) {
            if ( m_isNormalOn ) {
                HS_LOG( "HandleBackLanguage() - emit signalStartTimer "  );
               emit signalAutoDisappearTimer( true  , m_nDisplay);
           }
            else {
                HS_LOG( "HandleBackLanguage() - emit signalStartTimer FALSE!! NormalOff!!"  );
                m_bResetTimer = true;
            }
       }
        m_nFocusIndex = 0;
        emit setFocusIndex( m_nFocusIndex , m_nDisplay );
        emit signalSetLanguage(bIsMainScreen);
    }
}

int StandByAppEngine::GetLanguageID( )
{
//    HS_LOG( "GetLanguageID - m_nLanguage : " << m_nLanguage );
    return (int)m_nLanguage;
}

//QString StandByAppEngine::GetStrConst( int id )
//{
//    return StandByAppConstants::getStr(id);
//}

bool StandByAppEngine::CheckNaviStatus(){
    bool naviStatus = false ;
    //Check for Navi support in EngApp
    naviStatus = (GetSystemVariant() & 0x1);
    return naviStatus;
}

int StandByAppEngine::CheckVehicleStatus(){
    return m_nVehicle;
}

int StandByAppEngine::getEvv()
{
    return m_iEvv;
}
