#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>


// Qt Header
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QDeclarativeComponent>

#include <QProcess>

// Local Header
#include "DHAVN_AppSetting_Log.h"
#include "DHAVN_AppSettings_Def.h"
#include "DHAVN_AppSettings_AppEngine.h"
#include "DHAVN_AppSettings_SaveSettings.h"
#include "DHAVN_AppSettings_CMOSTManager.h"

// DH Header
#include <DHAVN_AppFramework_Def.h>

#define SLEEP_TIME 200000

#define MAX_SIZE 1024

#define TIME_ZONE_FILE      "/app/data/app_settings/localtime"
#define TIME_ZONE_FILE_KOR  "/usr/share/zoneinfo/ROK"


#define SHM_LANGUAGE_OPTION_SIZE                  (sizeof(char)*4)
#define SHM_SYS_OPTION_SIZE                       (sizeof(char)*9)
#define SHM_SYS_BOOT_TYPE_SIZE                    (sizeof(char)*4)
#define SHM_LAST_STATUS_SIZE                      (sizeof(char)*3)
#define SHM_AUDIO_LAST_MEMORY_SIZE                (sizeof(char)*8)
#define SHM_CURRENT_AV_MODE_SIZE                  (sizeof(char)*4)
#define SHM_POPUP_STATUS_SIZE                     (sizeof(char)*4)
#define SHM_MEDIA_WRAPPER_OWNER_SIZE              (sizeof(char)*4)
#define SHM_CALL_STATUS_SIZE                      (sizeof(char)*4)
#define SHM_SETTING_VALUE_SIZE                    (sizeof(char)*4)

#define SHM_OFFSET_LANGUAGE_OPTION                (0)
#define SHM_OFFSET_SYS_OPTION                     (SHM_LANGUAGE_OPTION_SIZE)
#define SHM_OFFSET_SYS_BOOT_TYPE                  (SHM_OFFSET_SYS_OPTION + SHM_SYS_OPTION_SIZE)
#define SHM_OFFSET_LAST_STATUS                    (SHM_OFFSET_SYS_BOOT_TYPE + SHM_SYS_BOOT_TYPE_SIZE)
#define SHM_OFFSET_AUDIO_LAST_MEMORY              (SHM_OFFSET_LAST_STATUS + SHM_LAST_STATUS_SIZE)
#define SHM_OFFSET_CURRENT_AV_MODE                (SHM_OFFSET_AUDIO_LAST_MEMORY + SHM_AUDIO_LAST_MEMORY_SIZE)
#define SHM_OFFSET_POPUP_STATUS                   (SHM_OFFSET_CURRENT_AV_MODE + SHM_CURRENT_AV_MODE_SIZE)
#define SHM_OFFSET_MEDIA_WRAPPER_OWNER            (SHM_OFFSET_POPUP_STATUS + SHM_POPUP_STATUS_SIZE)
#define SHM_OFFSET_CALL_STATUS                    (SHM_OFFSET_MEDIA_WRAPPER_OWNER + SHM_MEDIA_WRAPPER_OWNER_SIZE)
#define SHM_OFFSET_SETTING_VALUE                  (SHM_OFFSET_CALL_STATUS + SHM_CALL_STATUS_SIZE)
#define SHM_TOTAL_SIZE                            (SHM_OFFSET_SETTING_VALUE + SHM_SETTING_VALUE_SIZE)


// Shared Memory Info for QwertyKeypad
#define SHM_SYS_LANGUAGE_TYPE_SIZE                (sizeof(char)*4)
#define SHM_SYS_CURRENT_REGION_SIZE               (sizeof(char)*4)
#define SHM_DEFAULT_KEYPAD_TYPE_SIZE              (sizeof(char)*4)
#define SHM_KR_KEYPAD_TYPE_SIZE                   (sizeof(char)*4)
#define SHM_EN_KEYPAD_TYPE_SIZE                   (sizeof(char)*4)
#define SHM_EU_KEYPAD_TYPE_SIZE                   (sizeof(char)*4)
#define SHM_CYRILLIC_KEYPAD_TYPE_SIZE             (sizeof(char)*4)
#define SHM_ARABIC_KEYPAD_TYPE_SIZE               (sizeof(char)*4)


#define SHM_OFFSET_SYS_LANGUAGE_TYPE              (0)
#define SHM_OFFSET_SYS_CURRENT_REGION             (SHM_SYS_LANGUAGE_TYPE_SIZE)
#define SHM_OFFSET_DEFAULT_KEYPAD_TYPE            (SHM_OFFSET_SYS_CURRENT_REGION + SHM_SYS_CURRENT_REGION_SIZE)
#define SHM_OFFSET_KR_KEYPAD_TYPE                 (SHM_OFFSET_DEFAULT_KEYPAD_TYPE + SHM_DEFAULT_KEYPAD_TYPE_SIZE)
#define SHM_OFFSET_EN_KEYPAD_TYPE                 (SHM_OFFSET_KR_KEYPAD_TYPE + SHM_KR_KEYPAD_TYPE_SIZE)
#define SHM_OFFSET_EU_KEYPAD_TYPE                 (SHM_OFFSET_EN_KEYPAD_TYPE + SHM_EN_KEYPAD_TYPE_SIZE)
#define SHM_OFFSET_CYRILLIC_KEYPAD_TYPE           (SHM_OFFSET_EU_KEYPAD_TYPE + SHM_EU_KEYPAD_TYPE_SIZE)
#define SHM_OFFSET_ARABIC_KEYPAD_TYPE             (SHM_OFFSET_CYRILLIC_KEYPAD_TYPE + SHM_CYRILLIC_KEYPAD_TYPE_SIZE)
#define SHM_KEYPAD_INFO_TOTAL_SIZE                (SHM_OFFSET_ARABIC_KEYPAD_TYPE + SHM_ARABIC_KEYPAD_TYPE_SIZE)



const int DB_KEYS[9] = {
    SettingsDef::DB_KEY_PHOTO_FRAME,
    SettingsDef::DB_KEY_CLOCK_TYPE,
    SettingsDef::DB_KEY_LANGUAGE_TYPE,
    SettingsDef::DB_KEY_TEMPERATURE_TYPE,
    SettingsDef::DB_KEY_LOCKREARMONITOR_FUNCTION,
    SettingsDef::DB_KEY_EXPOSURE,
    SettingsDef::DB_KEY_ASPECT_RADIO,
    SettingsDef::DB_KEY_KEYPAD,
    SettingsDef::DB_KEY_DISTANCE_UNIT
};

SettingsStorage::SettingsStorage(SettingsAppEngine* engine, QObject *parent)
    : m_Engine(engine), m_pDBPath( SETTINGS_FILE_DB ), m_pDBSession( NULL ),
      m_pProcessingVDSTimer( NULL ),
      m_iSendDispControlToIBox(SettingsDef::SETTINGS_DMASK_ALL),
      m_bFrontScreenOnOff(true),
      m_bLeftRearScreen(false),
      m_bRightRearScreen(false),
      conn(QDBusConnection::sessionBus()) ,
      m_responseCountryVariant(QDBusConnection::sessionBus()),
      m_responseLCD(QDBusConnection::sessionBus()),
      m_sharedVideoPlayer(QString("VideoPlayer")),
      m_sharedMobileTv(QString("MobileTv")),
      m_nBalance(10), m_nFader(10), m_nHighTone(10), m_nMidTone(10), m_nLowTone(10),
      m_bVipSound(false), m_bAVC(false), m_bBeepSound(false), m_bSurround(false), m_bNaviSDCard(true),
      m_pResetScreenThread(NULL), m_pResetAllThread(NULL), m_pSharedMemory(NULL), m_pSharedMemoryForKeypad(NULL)
{
    isResetAll = false; //added for ITS 248640

    retryCount = 0; //added for DB write Fail Issue
    bool bDBSessionEnabled = false;

    while(true)
    {
        //LOG2(QString("[QT] show current Time: Previous DB Open :: [%1]").arg(getCurrentTime()), Logger::PRIORITY_HI);
        m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, m_pDBPath, SETTINGS_HM_DB, 0 , 0);
        //LOG2(QString("[QT] show current Time: END DB Open :: [%1]").arg(getCurrentTime()), Logger::PRIORITY_HI);

        bDBSessionEnabled = m_pDBSession ? true : false;
        retryCount++;

        if( (( m_nDBState == 0 ) && bDBSessionEnabled ) || (retryCount > 5) )
        {
            if(( m_nDBState == 0 ) && bDBSessionEnabled )
            {
                LOG2 ( QString("DB Open OK! State = %1, SEnabled = %2").arg(m_nDBState).arg(bDBSessionEnabled), Logger::PRIORITY_HI );
            }
            else
            {
                LOG2 ( QString("DB Open Failed! State = %1, Enabled = %2, retryCount = %3").arg(m_nDBState).arg(bDBSessionEnabled).arg(retryCount), Logger::PRIORITY_CRITICAL );
            }

            break;
        }
        else
        {
            LOG2 ( QString("DB Open Failed! State = %1, Enabled = %2, retryCount = %3").arg(m_nDBState).arg(bDBSessionEnabled).arg(retryCount), Logger::PRIORITY_CRITICAL );


            Close();

            usleep(SLEEP_TIME);
        }
    }

    // add for backup DB -->
    if(retryCount > 5)
    {
        retryCount = 0;
        LOG2(QString("DB File Crashed! Rep DB !"), Logger::PRIORITY_HI);

        Close();
        usleep(SLEEP_TIME);
        system("rm -f /app/data/app_settings/SettingsScreenSettings.db");
        usleep(SLEEP_TIME);
        system("cp /app/data/app_settings/SettingsScreenSettings_backup.db /app/data/app_settings/SettingsScreenSettings.db");
        usleep(SLEEP_TIME);
        system("sync");
        usleep(SLEEP_TIME);

        while(true)
        {
            m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, m_pDBPath, SETTINGS_HM_DB, 0 , 0);

            bDBSessionEnabled = m_pDBSession ? true : false;
            retryCount++;

            if( (( m_nDBState == 0 ) && bDBSessionEnabled ) || (retryCount > 5) )
            {
                if(( m_nDBState == 0 ) && bDBSessionEnabled )
                {
                    LOG2 ( QString("DB Open OK!! State = %1, Enabled = %2").arg(m_nDBState).arg(bDBSessionEnabled), Logger::PRIORITY_HI );
                }
                else
                {
                    LOG2 ( QString("DB Open Failed!! State = %1, Enabled = %2, retryCount = %3").arg(m_nDBState).arg(bDBSessionEnabled).arg(retryCount), Logger::PRIORITY_CRITICAL );
                }

                break;
            }
            else
            {
                LOG2 ( QString("DB Open Failed!! State = %1, Enabled = %2, retryCount = %3").arg(m_nDBState).arg(bDBSessionEnabled).arg(retryCount), Logger::PRIORITY_CRITICAL );
                Close();
                usleep(SLEEP_TIME);
            }
        }
    }
    // add for backup DB <--




    m_dateTime = new QDateTime;

    /*CR 13388 when mode change main send the abnormal treble values to micom so due to that this siganl handle
     Is removed */

    connect ( m_Engine, SIGNAL(saveSettingsSignal(int,int,int)),
              this, SLOT(SaveSettingsFromEngine(int,int,int)));

    connect ( m_Engine, SIGNAL(saveSettingsStringSignal(QString, int, int)),
              this, SLOT(SaveStringSettingsFromEngine(QString, int, int)));

    connect ( m_Engine, SIGNAL(saveVideoImageSettingsFromIBoxSignal(int, int, int, int)),
              this, SLOT(SaveVideoImageSettingsFromIBox(int, int, int, int)));

    connect( m_Engine, SIGNAL(sigNaviSdcardMounted()), this, SLOT(mountedNaviSDCard()));
    connect( m_Engine, SIGNAL(sigNaviSdcardRemoved()), this, SLOT(removedNaviSDCard()));

    connect (m_Engine, SIGNAL(sigFactoryReset()), this, SLOT(factoryReset()));

    //connect (m_Engine, SIGNAL(sigJukeboxCapacityChanged()), this, SLOT(SetDefaultWatingImage()));

    connect ( m_Engine, SIGNAL(updateDisplaySettingUI(int)), this, SLOT(updateDisplaySettingUI(int)));

    connect ( m_Engine, SIGNAL (sigSetTimeZone(int, bool)), this, SLOT(setTimeZone(int, bool)) );

    connect ( m_Engine, SIGNAL (chineKeypadChanged(int)), this, SLOT(chineKeypadChanged(int)) );

    /** Init Display State */
    initDisplayStateSettings();

    //Rear State Storage
    connect(m_Engine,SIGNAL(SetLRearState(int)),this,SLOT(onSetLRearState(int)));
    connect(m_Engine,SIGNAL(SetRRearState(int)),this,SLOT(onSetRRearState(int)));

    // added for modified ibox time set 02/05
    connect(m_Engine, SIGNAL(SetTimeToIBox(uchar,uint,uint,uint,uint,uint,uint,uint))
            , this, SLOT(onSetTimeToIBox(uchar,uint,uint,uint,uint,uint,uint,uint)));
    // added for modified ibox time set 02/05

    initRearMonitor();
    initGpsVariant();
    initIBoxVariant();

    connectDBUSInterface();
    initPropertyValue();

    // Time Zone Set (by m_nAutoTimeZone(property))
    //updateTimeZoneToSystem(false);
    InitialTimeformatAndDateformat();

    ConnectToMostBoardSetting();

    m_Engine->setGPSTimeSetting( LoadSetting( SettingsDef::DB_KEY_GPS_TIME_SETTINGS ) );

    if(!requestSoundSettigValue())
    {
        //qDebug()<< "Reading SoundSetupvalue from DBus-Interface is failed";
        LOG2(QString("Read SSvalue Fail!"), Logger::PRIORITY_HI);
    }

    requestDisplaySettigValue();

    //Read from Shared Memory
    getSharedMemoryFromUISH();

    setSHMForKeypad();
    //SetDefaultWatingImage();

    initIBoxSettings();

    setRearRRCVariant(AppEngineBase::GetSystemVariant() & 0x00010000);
    LOG2 ( QString("setRearRRCVariant: %1").arg(m_bRearRRCVariant), Logger::PRIORITY_HI );
}

//added for DB write Fail Issue
QString SettingsStorage::getCurrentTime()
{
    return QString("%1:%2:%3:%4").arg(QTime::currentTime().hour()).arg(QTime::currentTime().minute())
                                      .arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}
//added for DB write Fail Issue
void SettingsStorage::checkDBFile()
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, m_pDBPath, SETTINGS_HM_DB, 0 , 0);

    int value = -101; // seems it means empty data, however data has range [-10..10]
    int stateSize = 0;
    int result = SI_PM_OK;

    int errorCount = 0;

    //get setting from Settings DB
    if( NULL != m_pDBSession )
    {
        for (int i = 0; i < 9; i++)
        {
            result = SIPMGet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[DB_KEYS[i]].keyString, (unsigned char *)&value, &stateSize );

            if( ( result != SI_PM_OK ) || ( value == -101 ) )
                errorCount++;

            if (errorCount>4)
            {

                Close();
                QFile db_file( SETTINGS_FILE_DB );
                bool result = db_file.remove();
                LOG2 ( QString("DB file crashed. DELETE result = %1").arg(result), Logger::PRIORITY_HI );

                //QByteArray retval = CommandExecute("rm -rf /app/data/app_settings/SettingsScreenSettings.db");
                //LOG2( QString("DB DELETE result = %1").arg(retval.data()), Logger::PRIORITY_HI);
                //retval = CommandExecute("sync");
                //LOG2( QString("sync Command result = %1").arg(retval.data()), Logger::PRIORITY_HI);

                break;
            }
        }
    }
}

void SettingsStorage::initRearMonitor()
{
    uint uiSystemVariant = m_Engine->GetSystemVariant();

    if ( uiSystemVariant & SYSTEM_VARIANT_REAR_MONITOR )
    {
        setRearMonitor(true);
    }
    else
    {
        setRearMonitor(false);
    }

    LOG2 ( QString("initRMonitor: %1").arg(m_bRearMonitor), Logger::PRIORITY_HI );
}

void SettingsStorage::initGpsVariant()
{
    uint uiSystemVariant = m_Engine->GetSystemVariant();

    if ( uiSystemVariant & SYSTEM_VARIANT_NAVIGATION )
    {
        setIsNaviVariant(true);
    }
    else
    {
        setIsNaviVariant(false);
    }

    LOG2 ( QString("initGpsVariant: %1").arg(m_bIsNaviVariant), Logger::PRIORITY_HI );
}

void SettingsStorage::initIBoxVariant()
{
    uint uiSystemVariant = m_Engine->GetSystemVariant();

    if ( uiSystemVariant & SYSTEM_VARIANT_IBOX_MODEM )
    {
        setIsIBoxVariant(true);
    }
    else
    {
        setIsIBoxVariant(false);
    }

    LOG2 ( QString("initIBoxVariant: %1").arg(m_bIBoxVariant), Logger::PRIORITY_HI );
}

void SettingsStorage::initPropertyValue()
{
    LOG2 ( QString("initPropVal"), Logger::PRIORITY_HI );
    bool retResult; //modified for Smoke Test Issue DB Inavalid Value
    uint countErrValue = 0; //modified for Smoke Test Issue DB Inavalid Value
    /** Screen */
    setBrightness(0);
    setSaturation(0);
    setHue(0);
    setContrast(0);
    //modified for Smoke Test Issue DB Inavalid Value
    retResult = loadFromProperty(SettingsDef::DB_KEY_CURRENT_REGION);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_EXPOSURE);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_ASPECT_RADIO);
    if(retResult == false)
        countErrValue++;

    retResult = loadFromProperty(SettingsDef::DB_KEY_AUTO_TIMEZONE);
    if(retResult == false)
        countErrValue++;

    /** Sound */
    retResult = loadFromProperty(SettingsDef::DB_KEY_SOUND_VOLUME_RATIO);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_SOUND_BALANCE);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_SOUND_FADER);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_SOUND_LOWTONE);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_SOUND_MIDTONE);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_SOUND_HIGHTONE);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_SOUND_SURROUND);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_VIP_SOUND);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_SOUND_SPEED);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_SOUND_BEEP);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_SOUND_VEQ);
    if(retResult == false)
        countErrValue++;

    /** Voice */
    retResult = loadFromProperty(SettingsDef::DB_KEY_VOICE_VOICECOMMAND);
    if(retResult == false)
        countErrValue++;

    /** General */
    retResult = loadFromProperty(SettingsDef::DB_KEY_LANGUAGE_TYPE);
    if(retResult == false)
        countErrValue++;
    //loadFromProperty(SettingsDef::DB_KEY_FIRST_CAPITAL);
    retResult = loadFromProperty(SettingsDef::DB_KEY_DATEFORMAT_TYPE);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_VR_PHONE_PRIORITY);
    if(retResult == false)
        countErrValue++;
    //loadFromProperty(SettingsDef::DB_KEY_SCROLL_TICKER);
    retResult = loadFromProperty(SettingsDef::DB_KEY_APPROVAL);
    if(retResult == false)
        countErrValue++;

    //Photo
    //loadFromProperty(SettingsDef::DB_KEY_PHOTO_FRAME);
    retResult = loadFromProperty(SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE);
    if(retResult == false)
        countErrValue++;

    // Clock
    retResult = loadFromProperty(SettingsDef::DB_KEY_GPS_TIME_SETTINGS);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_AUTO_TIMEZONE);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_AUTO_TIMEZONE_SET);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_SUMMER_TIME);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_SUMMER_TIME_SET);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_CLOCK_TYPE);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_TIME_TYPE);
    if(retResult == false)
        countErrValue++;
    //loadFromProperty(SettingsDef::DB_KEY_CALENDAR_TYPE);

    //Keypad
    retResult = loadFromProperty(SettingsDef::DB_KEY_HYUNDAY_KEYPAD);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_ENGLISH_KEYPAD);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_KOREAN_KEYPAD);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_CHINA_KEYPAD);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_EUROPE_KEYPAD);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_ARABIC_KEYPAD);
    if(retResult == false)
        countErrValue++;
    retResult = loadFromProperty(SettingsDef::DB_KEY_RUSSIAN_KEYPAD);
    if(retResult == false)
        countErrValue++;

    retResult = loadFromProperty(SettingsDef::DB_KEY_DISTANCE_UNIT);
    if(retResult == false)
        countErrValue++;

    //Siri
    //loadFromProperty(SettingsDef::DB_KEY_VOICE_APPLE_SIRI);

    if(countErrValue > 3)
    {
        LOG2(QString("Load DB File Data Invalid value!: [%1]").arg(countErrValue), Logger::PRIORITY_HI);
        rewriteDB();
    }
    else
    {
        LOG2(QString("DB Data Err Count: [%1]").arg(countErrValue), Logger::PRIORITY_HI);
    }
    //modified for Smoke Test Issue DB Inavalid Value
}

bool SettingsStorage::loadFromProperty(int key)
{
    int value = -1;
    bool retResult = false; //modified for Smoke Test Issue DB Inavalid Value

    if(key != SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE)
    {
        value = LoadSetting(key);
        //modified for Smoke Test Issue DB Inavalid Value
        if(value > 1000 || value < -1000)
        {
            retResult = false;
        }
        else
            retResult = true;
        //modified for Smoke Test Issue DB Inavalid Value
    }

    switch(key)
    {
    case SettingsDef::DB_KEY_SUMMER_TIME:
        setSummerTime((bool)value);
        break;
    case SettingsDef::DB_KEY_GPS_TIME_SETTINGS:
        setGPSTime((bool)value);
        break;
        //        case SettingsDef::DB_KEY_DISPLAY_CLOCK_AT_AUDIO_END:
        //            break;
        //        case SettingsDef::DB_KEY_DISPLAY_CLOCK:
        //            break;
    case SettingsDef::DB_KEY_CLOCK_TYPE:
        //qDebug()<<"loadFromProperty(SettingsDef::DB_KEY_CLOCK_TYPE):"<<value;
        setClockType(value);
        break;
    case SettingsDef::DB_KEY_TIME_TYPE:
        setTimeType(value);
        break;
    case SettingsDef::DB_KEY_CALENDAR_TYPE:
        setCalendarType(value);
        break;
    case SettingsDef::DB_KEY_LANGUAGE_TYPE:
    {
        int language = m_Engine->GetUishLanguage();
        //qDebug()<<"loadFromProperty(DB_KEY_LANGUAGE_TYPE):"<<m_Engine->GetUishLanguage();
        if (language != value)
        {
            if(language == SettingsDef::SETTINGS_LANGUAGE_AR)
                m_bArabicLanguage = true;
            else
                m_bArabicLanguage = false;

            SaveSetting(language, SettingsDef::DB_KEY_LANGUAGE_TYPE);
            setLanguageType(language);
        }
        else
        {
            if(language == SettingsDef::SETTINGS_LANGUAGE_AR)
                m_bArabicLanguage = true;
            else
                m_bArabicLanguage = false;

            setLanguageType(value);
        }
        break;

    }
    case SettingsDef::DB_KEY_APPROVAL:
        setApproval((bool)value);
        break;

    case SettingsDef::DB_KEY_EXPOSURE:
        setExposure(value);
        break;
    case SettingsDef::DB_KEY_ASPECT_RADIO:
        setAspectRatio(value);
        break;


        /** DB & MMUS */
    case SettingsDef::DB_KEY_VIP_SOUND:
        setVipSound(value);
        break;
    case SettingsDef::DB_KEY_SOUND_BALANCE:
        setBalance(value);
        break;
    case SettingsDef::DB_KEY_SOUND_FADER:
        setFader(value);
        break;
    case SettingsDef::DB_KEY_SOUND_LOWTONE:
        setLowTone(value);
        break;
    case SettingsDef::DB_KEY_SOUND_MIDTONE:
        setMidTone(value);
        break;
    case SettingsDef::DB_KEY_SOUND_HIGHTONE:
        setHighTone(value);
        break;
    case SettingsDef::DB_KEY_SOUND_VOLUME_RATIO:
    {
        //added for DH volume Ratio spec modification
        if(value == 1)
        {
            setVolumeRatio(0); // Navi Priority
            m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_VOLUME_RATIO, 0, "");
            SaveSetting( 0, SettingsDef::DB_KEY_SOUND_VOLUME_RATIO, true );
        }
        else
        {
            setVolumeRatio(value);
        }

        break;
    }

    case SettingsDef::DB_KEY_SOUND_SURROUND:
        setSurround(value);
        break;
    case SettingsDef::DB_KEY_SOUND_SPEED:
        setAVC(value);
        break;
    case SettingsDef::DB_KEY_SOUND_BEEP:
        setBeepSound(value);
        break;
    case SettingsDef::DB_KEY_SOUND_VEQ:
        setVariableEQ(value);
        break;

    case SettingsDef::DB_KEY_VOICE_VOICECOMMAND:
        setVoiceCommand(value);
        break;

    case SettingsDef::DB_KEY_DISTANCE_UNIT:
        //qDebug()<<"loadFromProperty(SettingsDef::DB_KEY_DISTANCE_UNIT):"<<value;
        setDistanceUnit(value);
        break;

    case SettingsDef::DB_KEY_DATEFORMAT_TYPE:
        setDateFormatType(value);
        break;

    case SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS:
        setFrontScreenBrightness(value);
        break;
    case SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS:
        setRearScreenBrightness(value);
        break;

    case SettingsDef::DB_KEY_ENGLISH_KEYPAD:
        setEnglishKeypad(value);
        break;
    case SettingsDef::DB_KEY_KOREAN_KEYPAD:
        setKoreanKeypad(value);
        break;
    case SettingsDef::DB_KEY_ARABIC_KEYPAD:
        setArabicKeypad(value);
        break;
    case SettingsDef::DB_KEY_CHINA_KEYPAD:
        setChinaKeypad(value);
        break;
    case SettingsDef::DB_KEY_EUROPE_KEYPAD:
        setEuropeKeypad(value);
        break;
    case SettingsDef::DB_KEY_RUSSIAN_KEYPAD:
        setRussianKeypad(value);
        break;
    case SettingsDef::DB_KEY_CURRENT_REGION:
    {
        /*
        int uishValue = (int) m_Engine->GetCountryVariant();

        if (value != uishValue)
        {
            qDebug()<<"Called CountryVariant :: (dbValue!=uishValue)=>true";
            SaveSetting(uishValue, SettingsDef::DB_KEY_CURRENT_REGION);
            setCurrentRegion(uishValue);
        }
        else
        {
            setCurrentRegion(value);
        }
        */
        setCurrentRegion(AppEngineBase::GetCountryVariant());
        break;
    }
    case SettingsDef::DB_KEY_AUTO_TIMEZONE:
    {
        //modified for ITS 248479 Auto Time Zone Issue EU Variant
        if (((value >= -600) && (value <=540)) && (value != -101))
        {
        }
        else
        {
            value = GetDefaultTimeZone();
        }
        //modified for ITS 248479 Auto Time Zone Issue EU Variant
        setAutoTimeZone(value);

        break;
    }
        break;
    case SettingsDef::DB_KEY_AUTO_TIMEZONE_SET:
        setAutoTimeZoneSet(value);
        break;
    case SettingsDef::DB_KEY_SUMMER_TIME_SET:
        setSummerTimeSet((bool)value);
        break;
    case SettingsDef::DB_KEY_VR_PHONE_PRIORITY:
        setVRPhonePriority(value);
        break;
    case SettingsDef::DB_KEY_REAR_ON:
        break;
    case SettingsDef::DB_KEY_HYUNDAY_KEYPAD:
        setHyunDaiKeypad(value);
        break;

    case SettingsDef::DB_KEY_VOICE_APPLE_SIRI:
        setAppleSiri((bool) value);
        break;
    default :
        //qDebug()<< "default :: "<< key;
        break;
    }

    //LOG2 ( QString("key(%1), value(%2)").arg(key).arg(value), Logger::PRIORITY_HI );
    return retResult; //modified for Smoke Test Issue DB Inavalid Value
}

void SettingsStorage::UpdateDateTime(QDateTime *dateTime )
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    m_dateTime = dateTime;
}

SettingsStorage::~SettingsStorage()
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    Close();
    if(m_dateTime)
        delete m_dateTime;

    if(m_pResetScreenThread)
        delete m_pResetScreenThread;

    if(m_pResetAllThread)
        delete m_pResetAllThread;

    if(m_pSharedMemory)
    {
        if(m_pSharedMemory->isAttached())
            m_pSharedMemory->detach();

        delete m_pSharedMemory;
    }

    if(m_pSharedMemoryForKeypad)
    {
        if(m_pSharedMemoryForKeypad->isAttached())
            m_pSharedMemoryForKeypad->detach();

        delete m_pSharedMemoryForKeypad;
    }
}

bool SettingsStorage::Close()
{
    LOG2 ( QString("DB Close"), Logger::PRIORITY_HI );

    int result = SI_PM_OK;

    if( NULL != m_pDBSession )
    {
        result = SIPMClose( m_pDBSession );
        m_pDBSession = NULL;//added for Fact Reset-> DB Write segfault Issue
        m_nDBState = -1;//added for Fact Reset-> DB Write segfault Issue
    }
    return result == SI_PM_OK ? true : false;
}

int SettingsStorage::GetUishLanguage()
{
    return m_Engine->GetUishLanguage();
}

int SettingsStorage::variantCountry()
{
    return m_Engine->variantCountry();
}

void SettingsStorage::SaveSetting( int value, int key, bool setMMUS )
{
    LOG2 ( QString("Save: key:[%1], value[%2]").arg(key).arg(value), Logger::PRIORITY_HI );

    /** SETTINGS->SOUND:
     * change value using DBus-Interface */
    switch ( key )
    {
    case SettingsDef::DB_KEY_SOUND_BALANCE:
    {
        //Fixed value to correspond to MICOM range 0 to 20 instead -10 +10, so correction = 10
        if (setMMUS)
        {
            int newValue = value;
            NotifyBalanceChanged(newValue);
        }
        break;
    }
    case SettingsDef::DB_KEY_SOUND_FADER:
    {
        //Fixed value to correspond to MICOM range 0 to 20 instead -10 +10, so correction = 10
        if (setMMUS)
        {
            int newValue = value;
            NotifyFaderChanged(newValue);
        }
        break;
    }
    case SettingsDef::DB_KEY_SOUND_LOWTONE:
    {
        //Fixed value to correspond to MICOM range 0 to 20 instead -10 +10, so correction = 10
        if (setMMUS)
        {
            int newValue = value + 10;
            NotifyBassChanged(newValue);
        }
        break;
    }
    case SettingsDef::DB_KEY_SOUND_MIDTONE:
    {
        //Fixed value to correspond to MICOM range 0 to 20 instead -10 +10, so correction = 10
        if (setMMUS)
        {
            int newValue = value + 10;
            NotifyMidChanged(newValue);
        }
        break;
    }
    case SettingsDef::DB_KEY_SOUND_HIGHTONE:
    {
        //Fixed value to correspond to MICOM range 0 to 20 instead -10 +10, so correction = 10
        if (setMMUS)
        {
            int newValue = value + 10;
            NotifyTrebleChanged(newValue);
        }
        break;
    }

    case SettingsDef::DB_KEY_VIP_SOUND:
    {
        if (setMMUS)
        {
            NotifyVipSoundChanged(value);
        }
        break;
    }

    case SettingsDef::DB_KEY_SOUND_VOLUME_RATIO:
    {
        if (setMMUS)
        {
            NotifyVolumeRatioChanged(value);
        }
        break;
    }

    case SettingsDef::DB_KEY_SOUND_SURROUND:
    {
        if (setMMUS)
        {
            NotifySurroundChanged(value);
        }
        break;
    }

    case SettingsDef::DB_KEY_SOUND_SPEED:
    {
        if (setMMUS)
        {
            NotifyAVCChanged(value);
        }
        break;
    }
    case SettingsDef::DB_KEY_SOUND_BEEP:
    {
        if (setMMUS)
        {
            NotifyBeepChanged(value);
        }
        break;
    }
    case SettingsDef::DB_KEY_SOUND_VEQ:
    {
        if (setMMUS)
        {
            NotifyVariableEQChanged(value);
        }
        break;
    }

    case SettingsDef::DB_KEY_LOCKREARMONITOR_FUNCTION:
    {
        setRearLockScreen(value);
        break;
    }

    case SettingsDef::DB_KEY_VIDEO_BRIGHTNESS:
    {
        m_brightness = value;
        //m_Engine->NotifySetVideoSettings( m_brightness, m_saturation, m_contrast, m_hue );
        break;
    }

    case SettingsDef::DB_KEY_VIDEO_SATURATION:
    {
        m_saturation = value;
        //m_Engine->NotifySetVideoSettings( m_brightness, m_saturation, m_contrast, m_hue );
        break;
    }

    case SettingsDef::DB_KEY_VIDEO_CONTRAST:
    {
        m_contrast = value;
        //m_Engine->NotifySetVideoSettings( m_brightness, m_saturation, m_contrast, m_hue );
        break;
    }

    case SettingsDef::DB_KEY_VIDEO_HUE:
    {
        m_hue = value;
        //m_Engine->NotifySetVideoSettings( m_brightness, m_saturation, m_contrast, m_hue );
        break;
    }
    case SettingsDef::DB_KEY_LANGUAGE_TYPE:
    {
        SetSharedMemoryInfoForKeypad( SHM_OFFSET_SYS_LANGUAGE_TYPE, (unsigned char *)&value, SHM_SYS_LANGUAGE_TYPE_SIZE );
        // 현재 Arabic Language이고, 다른 Language로 초기화할 경우, UI변경을 위해 Property 변경
        if(isArabicLanguage())
        {
            if(value != SettingsDef::SETTINGS_LANGUAGE_AR)
                setIsArabicLanguage(false);
        }
        else    // 현재 Arabic Language가 아닌경우, Arabic Language로 변경할 경우, UI변경을 위해 Property 변경
        {
            if(value == SettingsDef::SETTINGS_LANGUAGE_AR)
                setIsArabicLanguage(true);
        }

        if(m_nLanguageType != value)
        {
            setLanguageType(value);
        }

        break;
    }


    case SettingsDef::DB_KEY_PHOTO_FRAME:
    {
        // MMUS Save
        NotifyPhotoFrameChanged(value);

        // Save SharedMemory
        SetSharedMemoryInfo( SHM_OFFSET_SETTING_VALUE, (unsigned char *)&value, SHM_SETTING_VALUE_SIZE );

        break;
    }

    case SettingsDef::DB_KEY_HYUNDAY_KEYPAD:
    {
        SetSharedMemoryInfoForKeypad( SHM_OFFSET_DEFAULT_KEYPAD_TYPE, (unsigned char *)&value, SHM_DEFAULT_KEYPAD_TYPE_SIZE );
        break;
    }
    case SettingsDef::DB_KEY_ENGLISH_KEYPAD:
    {
        SetSharedMemoryInfoForKeypad( SHM_OFFSET_EN_KEYPAD_TYPE, (unsigned char *)&value, SHM_EN_KEYPAD_TYPE_SIZE );
        break;
    }
    case SettingsDef::DB_KEY_KOREAN_KEYPAD:
    {
        SetSharedMemoryInfoForKeypad( SHM_OFFSET_KR_KEYPAD_TYPE, (unsigned char *)&value, SHM_KR_KEYPAD_TYPE_SIZE );
        break;
    }
    case SettingsDef::DB_KEY_ARABIC_KEYPAD:
    {
        SetSharedMemoryInfoForKeypad( SHM_OFFSET_ARABIC_KEYPAD_TYPE, (unsigned char *)&value, SHM_ARABIC_KEYPAD_TYPE_SIZE );
        break;
    }
    case SettingsDef::DB_KEY_EUROPE_KEYPAD:
    {
        SetSharedMemoryInfoForKeypad( SHM_OFFSET_EU_KEYPAD_TYPE, (unsigned char *)&value, SHM_EU_KEYPAD_TYPE_SIZE );
        break;
    }
    case SettingsDef::DB_KEY_RUSSIAN_KEYPAD:
    {
        SetSharedMemoryInfoForKeypad( SHM_OFFSET_CYRILLIC_KEYPAD_TYPE, (unsigned char *)&value, SHM_CYRILLIC_KEYPAD_TYPE_SIZE );
        break;
    }

        // fix its 243298 -->
        case SettingsDef::DB_KEY_AUTO_TIMEZONE:
        {
            if (((value >= -600) && (value <=540)) && (value != -101))
            {
            }
            else
            {
                value = GetDefaultTimeZone();
            }
        }
        // fix its 243298 <--

    default:
        break;
    }

    // read DB check DB state -->
    if(!factory) // add for its 243706
    {
        int stateSize = 0;
        int result1 = -1;
        int value1 = -101;
        result1 = SIPMGet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ key ].keyString, (unsigned char *)&value1, &stateSize );
        if( ( result1 != SI_PM_OK ) )
        {
            LOG2 ( QString("DB Read fail! Before write: %1 %2 ").arg(key).arg(value1), Logger::PRIORITY_CRITICAL );
            Close();
            usleep(SLEEP_TIME);
            system("rm -f /app/data/app_settings/SettingsScreenSettings.db");
            usleep(SLEEP_TIME);
            m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, m_pDBPath, SETTINGS_HM_DB, 0 , 0);
            rewriteDB();
        }
    }
    // read DB check DB state <--



    // add for backup DB -->
    if( (m_nDBState == 0) && (m_pDBSession != NULL) )
    {
        // save setting to DB
        //LOG2 ( QString("DB write start !!"), Logger::PRIORITY_CRITICAL );
        int result = -1;
        result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ key ].keyString, (unsigned char *)&value, sizeof( int ) );

        if( result != 0 )
        {
            LOG2 ( QString("DB write fail !!"), Logger::PRIORITY_CRITICAL );
            Close();
            usleep(SLEEP_TIME);
            system("rm -f /app/data/app_settings/SettingsScreenSettings.db");
            usleep(SLEEP_TIME);
            m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, m_pDBPath, SETTINGS_HM_DB, 0 , 0);
            rewriteDB();
        }
        //else
        //{
            //LOG2 ( QString("DB write Success !!"), Logger::PRIORITY_CRITICAL );
            //LOG2 ( QString("value : %1, key : %2").arg(value).arg(key), Logger::PRIORITY_CRITICAL );
        //}
    }
    // add for backup DB <--
    else
    {
        LOG2 ( QString("DB Write Failed! Session Null!!"), Logger::PRIORITY_CRITICAL );

        Close();
        m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, m_pDBPath, SETTINGS_HM_DB, 0 , 0);

        bool bDBSessionEnabled = m_pDBSession ? true : false;
        if( (m_nDBState !=0) || !bDBSessionEnabled )
        {
            LOG2 ( QString("DB Open Failed! State = %1, Enabled = %2").arg(m_nDBState).arg(bDBSessionEnabled), Logger::PRIORITY_CRITICAL );
        }
    }
}

// add for backup DB -->
void SettingsStorage::rewriteDB()
{
    //modified for Smoke Test Issue DB Inavalid Value
    LOG2 ( QString("rewriteDB"), Logger::PRIORITY_CRITICAL );
    int result = -1;
    int value = -101;

    value = m_nPhotoFrame;
    if(value > 1000 || value < -1000)
    {
        value = GetDefaultSetting(SettingsDef::DB_KEY_PHOTO_FRAME);
        setPhotoFrame(value);
    }
    //LOG2(QString("rewriteDB : key[0] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 0 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_bSummerTime;
    if(value > 10 || value < -10)
    {
        value = GetDefaultSummerTime();
        setSummerTime((bool)value);
    }
    //LOG2(QString("rewriteDB : key[1] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 2 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_bGPSTime;
    if(value > 10 || value < -10)
    {
        if(isNaviVariant())
            value = GetDefaultSetting( SettingsDef::DB_KEY_GPS_TIME_SETTINGS );
        else
            value = 0;

        setGPSTime((bool)value);
    }
    //LOG2(QString("rewriteDB : key[3] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 3 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nClockType;
    if(value > 10 || value < -10)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_CLOCK_TYPE );
        setClockType(value);
    }
    //LOG2(QString("rewriteDB : key[6] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 6 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nTimeType;
    if(value > 10 || value < -10)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_TIME_TYPE );
        setTimeType(value);
    }
    //LOG2(QString("rewriteDB : key[7] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 7 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nLanguageType;   
    if(value > 100 || value < -101)
    {
        value = GetDefaultLanguage();

        setLanguageType(value);
        m_Engine->ChangeLanguage(value);

    }
    //LOG2(QString("rewriteDB : key[9] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 9 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_bApproval;
    if(value > 10 || value < -10)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_APPROVAL );
        setApproval(value);
    }
    //LOG2(QString("rewriteDB : key[11] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 11 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_bRearLockScreen;
    if(value > 10 || value < -10)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_LOCKREARMONITOR_FUNCTION );
        setRearLockScreen(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_LOCKREARMONITOR_FUNCTION, value, "");
    }
    //LOG2(QString("rewriteDB : key[13] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 13 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nBrightness;
    if(value > 1000 || value < -1000)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_VIDEO_BRIGHTNESS );
    }
    //LOG2(QString("rewriteDB : key[14] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 14 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nSaturation;
    if(value > 1000 || value < -1000)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_VIDEO_SATURATION );
    }
    //LOG2(QString("rewriteDB : key[15] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 15 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nHue;
    if(value > 1000 || value < -1000)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_VIDEO_HUE );
    }
    //LOG2(QString("rewriteDB : key[16] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 16 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nContrast;
    if(value > 1000 || value < -1000)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_VIDEO_CONTRAST );
    }
    //LOG2(QString("rewriteDB : key[17] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 17 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nExposure;
    if(value > 1000 || value < -1000)
    {
         value = GetDefaultSetting( SettingsDef::DB_KEY_EXPOSURE );
    }
    //LOG2(QString("rewriteDB : key[19] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 19 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nAspectRatio;
    if(value > 10 || value < -10)
    {
         value = GetDefaultSetting( SettingsDef::DB_KEY_ASPECT_RADIO );
    }
    //LOG2(QString("rewriteDB : key[20] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 20 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nBalance;
    if(value > 1000 || value < -1000)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_BALANCE );
        setBalance(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_BALANCE, value, "");
    }
    //LOG2(QString("rewriteDB : key[24] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 24 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nFader;
    if(value > 1000 || value < -1000)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_FADER );
        setFader(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_FADER, value, "");
    }
    //LOG2(QString("rewriteDB : key[25] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 25 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nLowTone;
    if(value > 1000 || value < -1000)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_LOWTONE );
        setLowTone(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_LOWTONE, value, "");
    }
    //LOG2(QString("rewriteDB : key[26] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 26 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nMidTone;
    if(value > 1000 || value < -1000)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_MIDTONE );
        setMidTone(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_MIDTONE, value, "");
    }
    //LOG2(QString("rewriteDB : key[27] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 27 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nHighTone;;
    if(value > 1000 || value < -1000)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_HIGHTONE );
        setHighTone(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_HIGHTONE, value, "");
    }
    //LOG2(QString("rewriteDB : key[28] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 28 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nVolumeRatio;
    if(value > 10 || value < -10)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_VOLUME_RATIO );
        setVolumeRatio(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_VOLUME_RATIO, value, "");
    }
    //LOG2(QString("rewriteDB : key[29] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 29 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_bSurround;
    if(value > 10 || value < -10)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_SURROUND );
        setSurround(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_SURROUND, value, "");
    }
    //LOG2(QString("rewriteDB : key[32] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 32 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_bAVC;
    if(value > 10 || value < -10)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_SPEED );
        setAVC(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_SPEED, value, "");
    }
    //LOG2(QString("rewriteDB : key[34] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 34 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_bBeepSound;
    if(value > 10 || value < -10)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_BEEP );
        setBeepSound(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_BEEP, value, "");
        m_Engine->SendBeepSettingToIBox((uint)value);
    }
    //LOG2(QString("rewriteDB : key[35] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 35 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nVariableEQ;
    if(value > 10 || value < -10)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_VEQ );
        setVariableEQ(value);
    }
    //LOG2(QString("rewriteDB : key[36] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 36 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nVoiceCommand;
    //added for DQMS VR DB Init value Issue
    if( m_nVoiceCommand > 2 || m_nVoiceCommand < 0 )
    {
        value = 0;
        setVoiceCommand(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_VOICE_VOICECOMMAND, value, "");
    }
    //added for DQMS VR DB Init value Issue
    //LOG2(QString("rewriteDB : key[37] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 37 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nDistanceUnit;
    if(value > 10 || value < -10)
    {
        if(m_nCurrentRegion == eCVCanada)
        {

            value = 0;
            //LOG2 ( QString("[eCVCanada] Distance Unit Default value : km(0) : %1").arg(value), Logger::PRIORITY_HI );
            setDistanceUnit(value);

            m_Engine->NotifyDistanceUnitChanged(0); //added for NAVI Canada Variant Default Setting(KM)
            m_Engine->NotifyApplication( SettingsDef::DB_KEY_DISTANCE_UNIT, value, "");

        }
        else
        {

            value = GetDefaultSetting(SettingsDef::DB_KEY_DISTANCE_UNIT);
            //LOG2 ( QString("[NA] Distance Unit Default value : mi(1) : %1").arg(value), Logger::PRIORITY_HI );
            setDistanceUnit(value);

            m_Engine->NotifyDistanceUnitChanged(1); //added for NAVI Canada Variant Default Setting(KM)
            m_Engine->NotifyApplication( SettingsDef::DB_KEY_DISTANCE_UNIT, value, "");

        }
    }
    //LOG2(QString("rewriteDB : key[40] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 40 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nDateFormatType;
    if(value > 10 || value < -10)
    {
        value = GetDefaultDateTimeFormat();
        setDateFormatType(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_DATEFORMAT_TYPE, value, "");
    }
    //LOG2(QString("rewriteDB : key[43] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 43 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nFrontScreenBrightness;
    if(value > 100 || value < -100)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS );
    }
    //LOG2(QString("rewriteDB : key[46] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 46 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nRearScreenBrightness;
    if(value > 100 || value < -100)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS );
    }
    //LOG2(QString("rewriteDB : key[47] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 47 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nEnglishKeypad;
    if(value > 1000 || value < -1000)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_ENGLISH_KEYPAD );
        setEnglishKeypad(value);
    }
    //LOG2(QString("rewriteDB : key[49] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 49 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nKoreanKeypad;
    if(value > 1000 || value < -1000)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_KOREAN_KEYPAD );
        setKoreanKeypad(value);
    }
    //LOG2(QString("rewriteDB : key[50] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 50 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nArabicKeypad;
    if(value > 1000 || value < -1000)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_ARABIC_KEYPAD );
        setArabicKeypad(value);
    }
    //LOG2(QString("rewriteDB : key[51] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 51 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nChinaKeypad;
    if(value > 1000 || value < -1000)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_CHINA_KEYPAD );
        setChinaKeypad(value);
    }
    //LOG2(QString("rewriteDB : key[52] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 52 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nEuropeKeypad;
    if(value > 1000 || value < -1000)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_EUROPE_KEYPAD );
        setEuropeKeypad(value);
    }
    //LOG2(QString("rewriteDB : key[53] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 53 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nRussianKeypad;
    if(value > 1000 || value < -1000)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_RUSSIAN_KEYPAD );
        setEuropeKeypad(value);
    }
    //LOG2(QString("rewriteDB : key[54] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 54 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nCurrentRegion;
    //LOG2(QString("rewriteDB : key[55] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 55 ].keyString, (unsigned char *)&value, sizeof( int ) );

    //modified for ITS 248479 Auto Time Zone Issue EU Variant
    if (((m_nAutoTimeZone >= -600) && (m_nAutoTimeZone <=540)) && (m_nAutoTimeZone != -101))
    {
        value = m_nAutoTimeZone;
    }
    else
    {
        value = GetDefaultTimeZone();
    }
    //LOG2(QString("rewriteDB : key[61] value[%1]").arg(value), Logger::PRIORITY_HI);
    //modified for ITS 248479 Auto Time Zone Issue EU Variant
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 61 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_bAutoTimeZoneSet;
    if(value > 1000 || value < -1000)
    {
        value = 0;
    }
    //LOG2(QString("rewriteDB : key[62] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 62 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_bSummerTimeSet;
    if(value > 1000 || value < -1000)
    {
        value = 0;
    }
    //LOG2(QString("rewriteDB : key[63] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 63 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nVRPhonePriority;
    if(value > 2 || value < 0)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_VR_PHONE_PRIORITY );
        setVRPhonePriority(value);

        m_Engine->NotifyApplication( SettingsDef::DB_KEY_VR_PHONE_PRIORITY, value, "");
    }
    //LOG2(QString("rewriteDB : key[65] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 65 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = GetDefaultSetting( SettingsDef::DB_KEY_REAR_ON );
    //LOG2(QString("rewriteDB : key[66] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 66 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nHyunDaiKeypad;
    int currentRegion = m_nCurrentRegion;
    if(value > 10 || value < -10)
    {
        switch ( currentRegion )
        {
        case eCVInvalid:
        case eCVGeneral:
        case eCVKorea:
            value = SettingsDef::SETTINGS_HYUNDAI_KOREAN;
            break;
        case eCVEuropa:
        case eCVRussia:
            value = SettingsDef::SETTINGS_HYUNDAI_EUROPE;
            break;
        case eCVNorthAmerica:
        case eCVCanada:
            value = SettingsDef::SETTINGS_HYUNDAI_ENGLISH_LATIN;
            break;
        case eCVChina:
            value = SettingsDef::SETTINGS_HYUNDAI_CHINA;
            break;
        case eCVMiddleEast:
            value = SettingsDef::SETTINGS_HYUNDAI_ARABIC;
            break;
        default:
            value = SettingsDef::SETTINGS_HYUNDAI_KOREAN;
            break;
        }

        setHyunDaiKeypad(value);
    }
    //LOG2(QString("rewriteDB : key[67] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 67 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = GetDefaultSetting( SettingsDef::DB_KEY_LREAR );
    //LOG2(QString("rewriteDB : key[69] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 69 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = GetDefaultSetting( SettingsDef::DB_KEY_RREAR );
    //LOG2(QString("rewriteDB : key[70] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 70 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_nAppleSiri;
    if(value > 10 || value < -10)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_VOICE_APPLE_SIRI );
    }
    //LOG2(QString("rewriteDB : key[71] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 71 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = m_bVipSound;
    if(value > 10 || value < -10)
    {
        if(m_nCurrentRegion != eCVNorthAmerica || m_nCurrentRegion != eCVEuropa)
        {
            //SURROUND
            value = GetDefaultSetting( SettingsDef::DB_KEY_VIP_SOUND );
            setVipSound(value);
            m_Engine->NotifyApplication( SettingsDef::DB_KEY_VIP_SOUND, value, "");

        }
    }
    //LOG2(QString("rewriteDB : key[72] value[%1]").arg(value), Logger::PRIORITY_HI);
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 72 ].keyString, (unsigned char *)&value, sizeof( int ) );

    value = GetDefaultSetting( SettingsDef::DB_KEY_RESET_ALL_COMPLETE );
    result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ 73 ].keyString, (unsigned char *)&value, sizeof( int ) );

    if(result != 0) // DB write fail
    {
        LOG2 ( QString("DB reWrite Fail!"), Logger::PRIORITY_CRITICAL );
    }
    else
    {
        LOG2 ( QString("DB reWrite OK!"), Logger::PRIORITY_CRITICAL );
    }

}
// add for backup DB <--

void SettingsStorage::SaveSettingString( QString value, int key, int target_screen )
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    if( (m_nDBState == 0) && (m_pDBSession != NULL) )
    {
        SIPMRemove(m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ key ].keyString);
        int result = -1;
        result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ key ].keyString, (unsigned char *)value.toStdWString().c_str(), sizeof( wchar_t) * ( 1 + value.size() ));

        if( result != 0 )
            LOG2 ( QString("DB Write Failed!! key(%1), result(%2)").arg(key).arg(result), Logger::PRIORITY_CRITICAL );

        if(key == SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE) setPhotoPath(value);
    }
    else
    {
        LOG2 ( QString("DB Write Failed!! Session NULL!!"), Logger::PRIORITY_CRITICAL );

        Close();
        m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, m_pDBPath, SETTINGS_HM_DB, 0 , 0);

        bool bDBSessionEnabled = m_pDBSession ? true : false;
        if( (m_nDBState != 0) || !bDBSessionEnabled )
        {
            LOG2 ( QString("DB Open Failed! State = %1, Enabled = %2").arg(m_nDBState).arg(bDBSessionEnabled), Logger::PRIORITY_CRITICAL );
        }
    }
}

int SettingsStorage::LoadSetting( int key )
{


    int value = -101; // seems it means empty data, however data has range [-10..10]
    int stateSize = 0;
    int result = SI_PM_OK;

    //get setting from Settings DB
    if( (m_nDBState == 0) && (m_pDBSession != NULL) )
    {
        result = SIPMGet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ key ].keyString, (unsigned char *)&value, &stateSize );

        LOG2 ( QString("Load: key:[%1], value:[%2]").arg(key).arg(value), Logger::PRIORITY_HI );

        if( result != SI_PM_OK )
        {
            if(key != 62 && key != 63)
                LOG2 ( QString("DB_R Fail!! key(%1), result(%2)").arg(key).arg(result), Logger::PRIORITY_CRITICAL );
        }
        //if(key == SettingsDef::DB_KEY_CLOCK_TYPE)
        //    qDebug() << "Hanuk, DB_KEY_CLOCK_TYPE = " << value ;
    }
    else
    {
        LOG2 ( QString("DB_R Fail! Se_Null:key(%1)").arg(key), Logger::PRIORITY_CRITICAL );

        Close();
        m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, m_pDBPath, SETTINGS_HM_DB, 0 , 0);

        bool bDBSessionEnabled = m_pDBSession ? true : false;
        if( (m_nDBState != 0) || !bDBSessionEnabled )
        {
            LOG2 ( QString("DB_O Fail! State = %1, Enabled = %2").arg(m_nDBState).arg(bDBSessionEnabled), Logger::PRIORITY_CRITICAL );
        }
    }

    return value;
}


void SettingsStorage::SetSystemTimeByGPStime(int GPSYear,int GPSMonth,int GPSDay,int GPSHour,int GPSMin,int GPSSec, QString ampm = "")
{
    //LOG2 ( QString("--->"), Logger::PRIORITY_HI );

    SYSTEMTIME time;

    time.wYear = (unsigned short)GPSYear;
    time.wMonth = (unsigned short) GPSMonth;
    time.wDay = (unsigned short) GPSDay;
    time.wHour = (unsigned short) GPSHour;
    time.wMinute =(unsigned short) GPSMin;
    time.wSecond = (unsigned short) GPSSec;

    //qDebug() << "SetSystemTimeByGPStime, year = " << time.wYear ;
    //qDebug() << "SetSystemTimeByGPStime, wMonth = " << time.wMonth;
    //qDebug() << "SetSystemTimeByGPStime, wDay = " << time.wDay;
    //qDebug() << "SetSystemTimeByGPStime, wHour = " << time.wHour;
    //qDebug() << "SetSystemTimeByGPStime, wMinute = " << time.wMinute;
    //qDebug() << "SetSystemTimeByGPStime, wSecond = "<< time.wSecond;

    m_Engine->SetLastChangedTime(time);

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );

}

QString SettingsStorage::LoadSettingString( int key )
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    wchar_t value [MAX_SIZE] = { 0 };
    int stateSize = 0;
    int result = SI_PM_OK;

    if( (m_nDBState == 0) && (NULL != m_pDBSession ) )
    {
        result = SIPMGet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ key ].keyString, (unsigned char*)value, &stateSize );

        if( result != SI_PM_OK )
            LOG2 ( QString("DB_R Fail! key(%1), result(%2)").arg(key).arg(result), Logger::PRIORITY_CRITICAL );
    }
    else
    {
        LOG2 ( QString("DB_R Fail! S_Null: key(%1)").arg(key), Logger::PRIORITY_CRITICAL );

        Close();
        m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, m_pDBPath, SETTINGS_HM_DB, 0 , 0);

        bool bDBSessionEnabled = m_pDBSession ? true : false;
        if( (m_nDBState != 0) || !bDBSessionEnabled )
        {
            LOG2 ( QString("DB_OS Fail! State = %1, Enabled = %2").arg(m_nDBState).arg(bDBSessionEnabled), Logger::PRIORITY_CRITICAL );
        }
    }

    if ( result != SI_PM_OK )
    {
        return QString("");
    }
    //qDebug() << "SettingsStorage::LoadSettingString = " << QString::fromStdWString(value) ;
    return QString::fromStdWString(value);
}

int SettingsStorage::SetDefaultSetting( int key )
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    int value = -1;

    //qDebug() << "SettingsStorage::SetDefaultSetting key=" << key;

    for (int i=0; i<SettingsDef::DB_KEY_MAX ;i++)
    {
        if (SETTINGS_VARIABLES_DEFAULT_INT[i][0] == key )
        {
            //modify for Canada variant Distance Unit default value km
            if(key == SettingsDef::DB_KEY_DISTANCE_UNIT)
            {
                if(m_nCurrentRegion == eCVCanada)
                {
                   value = 0;
                   SaveSetting( value, key );
                   break;
                }
                else
                {
                    value = SETTINGS_VARIABLES_DEFAULT_INT[i][1];
                    SaveSetting( value, key );
                    break;
                }
            } //modify for NA/CA/CH Agreement Spec Modify(10 seconds, default on) 01/14
            else if(key == SettingsDef::DB_KEY_APPROVAL)
            {
                if(m_nCurrentRegion == eCVNorthAmerica || m_nCurrentRegion == eCVCanada || m_nCurrentRegion == eCVChina )
                {
                    value = 1;
                    SaveSetting( value, key );
                    break;
                }
                else
                {
                    value = SETTINGS_VARIABLES_DEFAULT_INT[i][1];
                    SaveSetting( value, key );
                    break;
                }
            } //modify for NA/CA/CH Agreement Spec Modify(10 seconds, default on) 01/14
            else
            {
                value = SETTINGS_VARIABLES_DEFAULT_INT[i][1];
                SaveSetting( value, key );
                break;
            }
            //modify for Canada variant Distance Unit default value km

            //value = SETTINGS_VARIABLES_DEFAULT_INT[i][1];
            //SaveSetting( value, key );
            //break;
        }
    }
    return value;
}

int SettingsStorage::GetDefaultSetting( int key )
{
    //LOG2 ( QString(" key value: %1").arg(key), Logger::PRIORITY_HI );

    int value = -1;

    //qDebug() << "SettingsStorage::GetDefaultSetting key=" << key;
    for (int i=0; i<SettingsDef::DB_KEY_MAX ;i++)
    {
        if (SETTINGS_VARIABLES_DEFAULT_INT[i][0] == key )
        {
            value = SETTINGS_VARIABLES_DEFAULT_INT[i][1];
            break;
        }
    }

    if ( key == SettingsDef::DB_KEY_REAR_ON )
    {
        return GetDefaultSettingOfDispOnOff( value );
    }
    return value;
}

void SettingsStorage::ResetSoundSettings(bool isRequestFromQml, bool factoryReset)
{
    LOG2 ( QString("ResetSoundSettings"), Logger::PRIORITY_HI );

    if(NotifyAllResetChanged())
    {
        int value;

        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_FADER );
        setFader(value);
        if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_FADER, value, "");
        SaveSetting( value, SettingsDef::DB_KEY_SOUND_FADER, false);

        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_BALANCE );
        setBalance(value);
        if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_BALANCE, value, "");
        SaveSetting( value, SettingsDef::DB_KEY_SOUND_BALANCE, false );

        //TONES
        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_LOWTONE );
        setLowTone(value);
        if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_LOWTONE, value, "");
        SaveSetting( value, SettingsDef::DB_KEY_SOUND_LOWTONE, false );

        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_MIDTONE );
        setMidTone(value);
        if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_MIDTONE, value, "");
        SaveSetting( value, SettingsDef::DB_KEY_SOUND_MIDTONE, false );

        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_HIGHTONE );
        setHighTone(value);
        if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_HIGHTONE, value, "");
        SaveSetting( value, SettingsDef::DB_KEY_SOUND_HIGHTONE, false );

        //VOLUME RATIO
        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_VOLUME_RATIO );
        setVolumeRatio(value);
        if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_VOLUME_RATIO, value, "");
        SaveSetting( value, SettingsDef::DB_KEY_SOUND_VOLUME_RATIO, false );

        //SURROUND
        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_SURROUND );
        setSurround(value);
        if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_SURROUND, value, "");
        SaveSetting( value, SettingsDef::DB_KEY_SOUND_SURROUND, false );

        if(m_nCurrentRegion != eCVNorthAmerica || m_nCurrentRegion != eCVEuropa)
        {
            //SURROUND
            value = GetDefaultSetting( SettingsDef::DB_KEY_VIP_SOUND );
            setVipSound(value);
            if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_VIP_SOUND, value, "");
            SaveSetting( value, SettingsDef::DB_KEY_VIP_SOUND, false );
        }

        //BEEP
        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_BEEP );
        setBeepSound(value);
        if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_BEEP, value, "");
        m_Engine->SendBeepSettingToIBox((uint)value);
        SaveSetting( value, SettingsDef::DB_KEY_SOUND_BEEP, false );

        //SPEED
        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_SPEED );
        setAVC(value);
        if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_SPEED, value, "");
        SaveSetting( value, SettingsDef::DB_KEY_SOUND_SPEED, false );

        //VEQ
        value = GetDefaultSetting( SettingsDef::DB_KEY_SOUND_VEQ );
        setVariableEQ(value);
        //m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_VEQ, value, "");
        SaveSetting( value, SettingsDef::DB_KEY_SOUND_VEQ, false );

        if(isRequestFromQml)
            QTimer::singleShot(1000, this, SLOT(notifyResetSoundCompleted()));
    }

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

//modified for ITS 248479 Auto Time Zone Issue EU Variant
void SettingsStorage::ResetGeneralSettings(bool isRequestFromQml, bool factoryReset, bool countryVariantChanged)
{
    LOG2 ( QString("ResetGeneralSettings"), Logger::PRIORITY_HI );

    //qDebug()<<"Called SettingsStorage::ResetGeneralSettings()";
    int value = 0;

    //GPS Reception
    //modify for GPS checkBox 13/12/27
    //LOG2 ( QString("naviSDCard(): %1 , isNaviVariant() : %2 ").arg(naviSDCard()).arg(isNaviVariant()), Logger::PRIORITY_HI );
    if(/*naviSDCard() &&*/ isNaviVariant())
        value = GetDefaultSetting( SettingsDef::DB_KEY_GPS_TIME_SETTINGS );
    else
        value = 0;
    //modify for GPS checkBox 13/12/27
    setGPSTime((bool)value);
    SaveSetting( value, SettingsDef::DB_KEY_GPS_TIME_SETTINGS );

    // 2013/12/27
    //delete for GPS Time CAN Date Issue
    //if(m_bNaviSDCard && m_bIsNaviVariant)
    //    SettingsModeChange((bool)value);
    //else
    //    SettingsModeChange(false);
    //delete for GPS Time CAN Date Issue
    //if (!factoryReset) SettingsModeChange((bool)value);
    // 2013/12/27
    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_GPS_TIME_SETTINGS, value, "");


    //SummerTime
    value = GetDefaultSummerTime();
    setSummerTime((bool)value);
    SaveSetting( value, SettingsDef::DB_KEY_SUMMER_TIME );
    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_SUMMER_TIME, value, "" );

    //ClockType
    value = GetDefaultSetting( SettingsDef::DB_KEY_CLOCK_TYPE );
    setClockType(value);
    SaveSetting( value, SettingsDef::DB_KEY_CLOCK_TYPE );
    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_CLOCK_TYPE, value, "");

    //TimeFormat
    value = GetDefaultSetting( SettingsDef::DB_KEY_TIME_TYPE );
    setTimeType(value);
    SaveSetting( value, SettingsDef::DB_KEY_TIME_TYPE );
    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_TIME_TYPE, value, "");

    //ViewCalendar
    //value = GetDefaultSetting( SettingsDef::DB_KEY_CALENDAR_TYPE );
    //setCalendarType(value);
    //SaveSetting( value, SettingsDef::DB_KEY_CALENDAR_TYPE );
    //m_Engine->NotifyApplication( SettingsDef::DB_KEY_CALENDAR_TYPE, value, "" );

    //TimeZone set
    //value = GetDefaultSetting( SettingsDef::DB_KEY_AUTO_TIMEZONE_SET );
    //setAutoTimeZoneSet(value);
    //SaveSetting( value, SettingsDef::DB_KEY_AUTO_TIMEZONE_SET );
    //m_Engine->NotifyApplication( SettingsDef::DB_KEY_AUTO_TIMEZONE_SET, value, "" );

    //AutoTimeZone
    //value = GetDefaultTimeZone();
    //setAutoTimeZone(value);
    //SaveSetting( value, SettingsDef::DB_KEY_AUTO_TIMEZONE );
    //m_Engine->NotifyApplication( SettingsDef::DB_KEY_AUTO_TIMEZONE, value, "");

    if (factoryReset || countryVariantChanged) //added for ITS 248479 Auto Time Zone Issue EU Variant
    {
        value = GetDefaultTimeZone();
        setAutoTimeZone(value);
        SaveSetting( value, SettingsDef::DB_KEY_AUTO_TIMEZONE );
        updateTimeZoneToSystem(false);
    }

    //Waiting Display (None/Clock/Image)
    value = GetDefaultSetting( SettingsDef::DB_KEY_PHOTO_FRAME );
    setPhotoFrame(value);
    SaveSetting( value, SettingsDef::DB_KEY_PHOTO_FRAME );
    // MMUS Save
    NotifyPhotoFrameChanged(value);
    // Save SharedMemory
    SetSharedMemoryInfo( SHM_OFFSET_SETTING_VALUE, (unsigned char *)&value, SHM_SETTING_VALUE_SIZE );
    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_PHOTO_FRAME, value, "Off");
    //SetDefaultWatingImage(true);

    //Language
    int current_lang = LoadSetting( SettingsDef::DB_KEY_LANGUAGE_TYPE );
    value = GetDefaultLanguage();
    if (current_lang != value)
    {
        setLanguageType(value);

        // 현재 Arabic Language가 아니고, Arabic Language로 초기화할 경우, UI변경을 위해 Property 변경
        if(!isArabicLanguage())
        {
            //LOG2 ( QString("isArabicLanguage():").arg(isArabicLanguage()), Logger::PRIORITY_HI );
            if(value == SettingsDef::SETTINGS_LANGUAGE_AR)
            {
                //LOG2 ( QString("value==SettingsDef::SETTINGS_LANGUAGE_AR"), Logger::PRIORITY_HI );
                setIsArabicLanguage(true);
            }
        }

        SaveSetting( value, SettingsDef::DB_KEY_LANGUAGE_TYPE );
        m_Engine->ChangeLanguage(value);
    }

    //Keypad
    GetDefaultKeyPad();

    UpdateKeyboardData();


    //Distance Unit
    //value = GetDefaultDistanceUnit();
    //modify for Canada variant Distance Unit default value km
    if(m_nCurrentRegion == eCVCanada)
    {

        value = 0;
        //LOG2 ( QString("[eCVCanada] Distance Unit Default value : km(0) : %1").arg(value), Logger::PRIORITY_HI );
        setDistanceUnit(value);
        SaveSetting( value, SettingsDef::DB_KEY_DISTANCE_UNIT );
        m_Engine->NotifyDistanceUnitChanged(0); //added for NAVI Canada Variant Default Setting(KM)
        if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_DISTANCE_UNIT, value, "");

    }
    else
    {

        value = GetDefaultSetting(SettingsDef::DB_KEY_DISTANCE_UNIT);
        //LOG2 ( QString("[NA] Distance Unit Default value : mi(1) : %1").arg(value), Logger::PRIORITY_HI );
        setDistanceUnit(value);
        SaveSetting( value, SettingsDef::DB_KEY_DISTANCE_UNIT );
        m_Engine->NotifyDistanceUnitChanged(1); //added for NAVI Canada Variant Default Setting(KM)
        if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_DISTANCE_UNIT, value, "");

    }
    //modify for Canada variant Distance Unit default value km

    //    value = GetDefaultSetting(SettingsDef::DB_KEY_DISTANCE_UNIT);
    //    setDistanceUnit(value);
    //    SaveSetting( value, SettingsDef::DB_KEY_DISTANCE_UNIT );
    //    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_DISTANCE_UNIT, value, "");

    //Date Format
    value = GetDefaultDateTimeFormat();
    setDateFormatType(value);
    SaveSetting( value, SettingsDef::DB_KEY_DATEFORMAT_TYPE );
    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_DATEFORMAT_TYPE, value, "");

    //Approval

	//added for NA/CH Agreement Spec Modify(time 10, default on)
    if(m_nCurrentRegion == eCVNorthAmerica || m_nCurrentRegion == eCVCanada || m_nCurrentRegion == eCVChina)
    {
        value = 1;
        setApproval(value);
        SaveSetting( value, SettingsDef::DB_KEY_APPROVAL );
        if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_APPROVAL, value, "");
    }
    else
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_APPROVAL );
        setApproval(value);
        SaveSetting( value, SettingsDef::DB_KEY_APPROVAL );
        if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_APPROVAL, value, "");
    }
	//added for NA/CH Agreement Spec Modify(time 10, default on)

    //    value = GetDefaultSetting( SettingsDef::DB_KEY_APPROVAL );
    //    setApproval(value);
    //    SaveSetting( value, SettingsDef::DB_KEY_APPROVAL );
    //    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_APPROVAL, value, "");

    //A

    if(m_bRearRRCVariant)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_LOCKREARMONITOR_FUNCTION );
        setRearLockScreen(value);
        SaveSetting( value, SettingsDef::DB_KEY_LOCKREARMONITOR_FUNCTION );
        if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_LOCKREARMONITOR_FUNCTION, value, "");
    }

    value = GetDefaultSetting( SettingsDef::DB_KEY_VR_PHONE_PRIORITY );
    setVRPhonePriority(value);
    SaveSetting( value, SettingsDef::DB_KEY_VR_PHONE_PRIORITY );
    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_VR_PHONE_PRIORITY, value, "");

    value = GetDefaultSetting( SettingsDef::DB_KEY_VOICE_VOICECOMMAND );
    setVoiceCommand(value);
    SaveSetting( value, SettingsDef::DB_KEY_VOICE_VOICECOMMAND );
    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_VOICE_VOICECOMMAND, value, "");

    //added for DQMS VR DB Init value Issue
    value = GetDefaultSetting( SettingsDef::DB_KEY_AUTO_TIMEZONE_SET );
    SaveSetting( value, SettingsDef::DB_KEY_AUTO_TIMEZONE_SET );

    value = GetDefaultSetting( SettingsDef::DB_KEY_SUMMER_TIME_SET );
    SaveSetting( value, SettingsDef::DB_KEY_SUMMER_TIME_SET );

    int countryVariantFromUISH = AppEngineBase::GetCountryVariant();
	
    value = countryVariantFromUISH;
    SaveSetting(value, SettingsDef::DB_KEY_CURRENT_REGION);

    //added for DQMS VR DB Init value Issue

    /*
    value = GetDefaultSetting( SettingsDef::DB_KEY_VOICE_APPLE_SIRI );
    setAppleSiri((bool)value);
    SaveSetting( value, SettingsDef::DB_KEY_VOICE_APPLE_SIRI );
    m_Engine->NotifyApplication( SettingsDef::DB_KEY_VOICE_APPLE_SIRI, value, "");
    */

    /*
    //Left Rear
    value = GetDefaultSetting(SettingsDef::DB_KEY_LREAR);
    setLeftRearScreen(value);
    m_Engine->NotifyApplication(SettingsDef::DB_KEY_REAR_ON ,value,"");

    //Right Rear
    value = GetDefaultSetting(SettingsDef::DB_KEY_RREAR);
    setRightRearScreen((value==2)?false:true);
    m_Engine->NotifyApplication(SettingsDef::DB_KEY_REAR_ON ,value,"");
    */

    updateTimeZoneToSystem(false); //added for ITS 230259 Summer Time On/Off Time Sync issue

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );

    if(isRequestFromQml)
        QTimer::singleShot(1000, this, SLOT(notifyResetGeneralCompleted()));
}

void SettingsStorage::InitialTimeformatAndDateformat()
{
    //LOG2 ( QString("InitialTimeformatAndDateformat"), Logger::PRIORITY_HI );

    int dbValue = LoadSetting(SettingsDef::DB_KEY_DATEFORMAT_TYPE);
    //qDebug("SettingsStorage::InitialTimeformatAndDateformat(), DATEFORMAT = %d,", dbValue);
    m_Engine->SetDateFormat(dbValue);

    dbValue = LoadSetting(SettingsDef::DB_KEY_TIME_TYPE);
    //qDebug("SettingsStorage::InitialTimeformatAndDateformat(), TIME_TYPE = %d,", dbValue);
    m_Engine->SetTimeFormat( (char) 0xFF & dbValue );
}

void SettingsStorage::InitialDistanceUnit()
{
    //LOG2 ( QString("InitialDistanceUnit"), Logger::PRIORITY_HI );

    int dbValue = LoadSetting(SettingsDef::DB_KEY_DISTANCE_UNIT);
    //qDebug("SettingsStorage::InitialDistanceUnit(), DISTANCE_UNIT = %d,", dbValue);
    m_Engine->SendDistanceUnit(dbValue);
}

void SettingsStorage::ResetScreenSettings(bool isRequestFromQml, bool factoryReset)
{
    LOG2 ( QString("ResetScreenSettings"), Logger::PRIORITY_HI );

    int value = 0;

    //Aspect Ratio
    value = GetDefaultSetting( SettingsDef::DB_KEY_ASPECT_RADIO );
    setAspectRatio(value);
    SaveSetting( value, SettingsDef::DB_KEY_ASPECT_RADIO );
    m_Engine->NotifySetAspectRatio(value, 1);
    //m_Engine->NotifyApplication( SettingsDef::DB_KEY_ASPECT_RADIO, value, "", screen );

    //video image
    value = GetDefaultSetting( SettingsDef::DB_KEY_VIDEO_BRIGHTNESS );
    setBrightness(value);
    SaveSetting( value, SettingsDef::DB_KEY_VIDEO_BRIGHTNESS );
    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_VIDEO_BRIGHTNESS, value, "");
    //m_Engine->HandleBrightness(value);

    value = GetDefaultSetting( SettingsDef::DB_KEY_VIDEO_SATURATION );
    setSaturation(value);
    SaveSetting( value, SettingsDef::DB_KEY_VIDEO_SATURATION );
    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_VIDEO_SATURATION, value, "");
    //m_Engine->HandleSaturation(value);

    value = GetDefaultSetting( SettingsDef::DB_KEY_VIDEO_HUE );
    setHue(value);
    SaveSetting( value, SettingsDef::DB_KEY_VIDEO_HUE );
    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_VIDEO_HUE, value, "");
    //m_Engine->HandleColor(value);

    value = GetDefaultSetting( SettingsDef::DB_KEY_VIDEO_CONTRAST );
    setContrast(value);
    SaveSetting( value, SettingsDef::DB_KEY_VIDEO_CONTRAST );
    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_VIDEO_CONTRAST, value, "");
    //m_Engine->HandleDarkness(value);

    // front rear screen brightness
    value = GetDefaultSetting( SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS );
    setFrontScreenBrightness(value);
    SaveSetting( value, SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS );

    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS,value, "");
    m_Engine->NotifyDisplayBrightnessChange(1, 10);

    value = GetDefaultSetting( SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS );
    setRearScreenBrightness(value);
    SaveSetting( value, SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS );

    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS,value, "");
    m_Engine->NotifyDisplayBrightnessChange(0, 10);

    //Dimming
    value = GetDefaultSetting( SettingsDef::DB_KEY_EXPOSURE );
    setExposure(value);
    SaveSetting( value, SettingsDef::DB_KEY_EXPOSURE );
    m_Engine->NotifyDisplayModeChange(value);
    m_Engine->NotifySetIlluminationMostManager(value);
    if (!factoryReset) m_Engine->NotifyApplication( SettingsDef::DB_KEY_EXPOSURE,value, "");

    //view clock
    value = GetDefaultSetting( SettingsDef::DB_KEY_DISPLAY_CLOCK );
    SaveSetting( value, SettingsDef::DB_KEY_DISPLAY_CLOCK );

    //! Send MOST message
    //SendVideoDefaultSetMost( SettingsDef::SETTINGS_VDS_S_ALL, bIsRearDispChanged );

    if(isRequestFromQml)
        QTimer::singleShot(1000, this, SLOT(notifyResetScreenCompleted()));

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

void SettingsStorage::notifyResetSoundCompleted()
{
    emit resetSoundCompleted();
}

void SettingsStorage::notifyResetScreenCompleted()
{
    emit resetScreenCompleted();
}
//added for DB File Crash Issue
void SettingsStorage::notifyFactoryResetCompleted()
{
    LOG2 ( QString("notifyFactoryResetCompleted"), Logger::PRIORITY_HI );
    m_Engine->sendResponseFactoryReset();
}

void SettingsStorage::notifyResetGeneralCompleted()
{
    emit resetGeneralCompleted();
}

void SettingsStorage::SaveSettingsFromEngine (int value, int key, int screen)
{
    LOG2 ( QString("SaveSettingsFromEngine: [%1][%2]").arg(key).arg(value), Logger::PRIORITY_HI );

    //added for ITS 248640
    if(isResetAll == true)
    {
        if(key != SettingsDef::DB_KEY_LANGUAGE_TYPE)
            SaveSetting(value,key);
        else
          LOG2(QString("Save Exception. L_Type"), Logger::PRIORITY_HI );
    }
    else if(isResetAll == false)
    {
         SaveSetting(value,key);
    }
    //added for ITS 248640
    switch ( key )
    {
    case SettingsDef::DB_KEY_DISTANCE_UNIT:
    {
        setDistanceUnit(value);
        emit changeDistanceUnit();
    }
        break;


    default: break;
    }
}

/*
void SettingsStorage::changeToGPSMode(bool isGpsAvilable)
{

}
*/

void SettingsStorage::SaveStringSettingsFromEngine(QString value, int key, int target_screen)
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    //qDebug()<<"!!!!  SettingsStorage::SaveStringSettingsFromEngine value="<<value;
    SaveSettingString(value,key, target_screen);
    setPhotoPath(value);
}

void SettingsStorage::SaveVideoImageSettingsFromIBox( int brightness, int saturation, int contrast, int hue )
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );
    int result = -1;
    m_brightness = brightness;
    m_saturation = saturation;
    m_contrast = contrast;
    m_hue = hue;

    if( (m_nDBState == 0) && (m_pDBSession != NULL) )
    {
        //save setting to DB
        setBrightness(m_brightness);
        result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ SettingsDef::DB_KEY_VIDEO_BRIGHTNESS ].keyString,
                (unsigned char *)&brightness, sizeof( int ) );

        if( result != 0 )
            LOG2 ( QString("DB Write Failed!! key(%1), result(%2)").arg(SettingsDef::DB_KEY_VIDEO_BRIGHTNESS).arg(result), Logger::PRIORITY_CRITICAL );

        setSaturation(m_saturation);
        result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ SettingsDef::DB_KEY_VIDEO_SATURATION ].keyString,
                (unsigned char *)&saturation, sizeof( int ) );

        if( result != 0 )
            LOG2 ( QString("DB Write Failed!! key(%1), result(%2)").arg(SettingsDef::DB_KEY_VIDEO_SATURATION).arg(result), Logger::PRIORITY_CRITICAL );

        setContrast(m_contrast);
        result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ SettingsDef::DB_KEY_VIDEO_CONTRAST ].keyString,
                (unsigned char *)&contrast, sizeof( int ) );

        if( result != 0 )
            LOG2 ( QString("DB Write Failed!! key(%1), result(%2)").arg(SettingsDef::DB_KEY_VIDEO_CONTRAST).arg(result), Logger::PRIORITY_CRITICAL );

        setHue(m_hue);
        result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ SettingsDef::DB_KEY_VIDEO_HUE ].keyString,
                (unsigned char *)&hue, sizeof( int ) );

        if( result != 0 )
            LOG2 ( QString("DB Write Failed!! key(%1), result(%2)").arg(SettingsDef::DB_KEY_VIDEO_HUE).arg(result), Logger::PRIORITY_CRITICAL );
    }
    else
    {
        LOG2 ( QString("DB_W Fail! S_Null!"), Logger::PRIORITY_CRITICAL );

        Close();
        m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, m_pDBPath, SETTINGS_HM_DB, 0 , 0);

        bool bDBSessionEnabled = m_pDBSession ? true : false;
        if( (m_nDBState != 0) || !bDBSessionEnabled )
        {
            LOG2 ( QString("DB_OS Fail! State = %1, Enabled = %2").arg(m_nDBState).arg(bDBSessionEnabled), Logger::PRIORITY_CRITICAL );
        }
    }
}

int SettingsStorage::GetDefaultLanguage( )
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    ECountryVariant currentRegion = (ECountryVariant)m_nCurrentRegion;
    int language = -1;

    //qDebug() << "SettingsStorage::GetDefaultLanguage currentRegion = " << currentRegion;
    //modified for Smoke Test Issue DB Inavalid Value
    if(currentRegion <= eCVInvalid || currentRegion >= eCVInvalidMax)
    {
        LOG2(QString("Invailid Region: %1").arg(currentRegion), Logger::PRIORITY_HI);
        int countryVariantFromUISH = AppEngineBase::GetCountryVariant();

        setCurrentRegion(countryVariantFromUISH);

        currentRegion = (ECountryVariant)m_nCurrentRegion;
    }
	//modified for Smoke Test Issue DB Inavalid Value

    switch ( currentRegion )
    {
    case eCVKorea:
        language = SettingsDef::SETTINGS_LANGUAGE_KO;
        break;

    case eCVNorthAmerica:
    case eCVGeneral:
    case eCVInvalid:
    case eCVCanada:
        language = SettingsDef::SETTINGS_LANGUAGE_EN_US;
        break;

    case eCVEuropa:
        language = SettingsDef::SETTINGS_LANGUAGE_EN_UK;
        break;

    case eCVChina:
        language = SettingsDef::SETTINGS_LANGUAGE_ZH_MA;
        break;

    case eCVMiddleEast:
        language = SettingsDef::SETTINGS_LANGUAGE_AR;
        break;

    case eCVRussia:
        language = SettingsDef::SETTINGS_LANGUAGE_RU;
        break;

    default: break;
    }

    return language;
}

int SettingsStorage::GetDefaultSummerTime( )
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    ECountryVariant currentRegion = (ECountryVariant)m_nCurrentRegion;
    int summerTime = 0;

    if(currentRegion <= eCVInvalid || currentRegion >= eCVInvalidMax)
        LOG2 ( QString("INCORRECT currentRegion"), Logger::PRIORITY_HI );

    switch ( currentRegion )
    {
    case eCVKorea:
    case eCVGeneral:
    case eCVChina:
    case eCVMiddleEast:
        summerTime = 0;
        break;

    case eCVNorthAmerica:
    case eCVCanada:
    case eCVEuropa:
    case eCVRussia:
        summerTime = 1;
        break;

    default: break;
    }

    return summerTime;
}

int SettingsStorage::GetDefaultDateTimeFormat( )
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    int currentRegion = m_nCurrentRegion;
    int dateTimeFormat = SettingsDef::SETTINGS_DATEFORMAT_YYYYMMDD_DASH;
    //qDebug() << "SettingsStorage::GetDefaultDateTimeFormat currentRegion = " << currentRegion;

    if(currentRegion <= (int)eCVInvalid || currentRegion >= (int)eCVInvalidMax)
        LOG2(QString("INCORRECT Region: %1").arg(currentRegion), Logger::PRIORITY_HI);
        //qDebug() << "SettingsStorage::GetDefaultDateTimeFormat INCORRECT currentRegion";

    switch ( currentRegion )
    {
    case eCVKorea:
    case eCVGeneral:
    case eCVChina:
    case eCVMiddleEast:
        dateTimeFormat = SettingsDef::SETTINGS_DATEFORMAT_YYYYMMDD_DASH;
        break;

    case eCVNorthAmerica:
    case eCVCanada:
        dateTimeFormat = SettingsDef::SETTINGS_DATEFORMAT_MMDDYYYY_DASH;
        break;

    case eCVEuropa:
    case eCVRussia:
        dateTimeFormat = SettingsDef::SETTINGS_DATEFORMAT_DDMMYYYY_DASH;
        break;

    default: break;
    }

    return dateTimeFormat;
}

/*
int SettingsStorage::GetDefaultDistanceUnit()
{
    LOG2 ( QString(""), Logger::PRIORITY_HI );

    int currentRegion = LoadSetting(SettingsDef::DB_KEY_CURRENT_REGION);
    int distanceValue = LoadSetting( SettingsDef::DB_KEY_DISTANCE_UNIT );

    qDebug() << "SettingsStorage::GetDefaultDistanceUnit currentRegion = " << currentRegion;

    if(currentRegion <= (int)eCVInvalid || currentRegion >= (int)eCVInvalidMax)
        qDebug() << "SettingsStorage::GetDefaultDistanceUnit INCORRECT currentRegion";

    switch ( currentRegion )
    {
    case eCVMiddleEast:
        distanceValue = SettingsDef::SETTINGS_DISTANCE_KM;
        break;

    case eCVGeneral:
    case eCVNorthAmerica:
    case eCVCanada:
    case eCVEuropa:
        distanceValue = SettingsDef::SETTINGS_DISTANCE_MILE;
        break;

    default: break;
    }

    return distanceValue;
}
*/

void SettingsStorage::GetDefaultKeyPad( )
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    int currentRegion = m_nCurrentRegion;
    int keypadType = -1;

    //qDebug() << "SettingsStorage::GetDefaultKeyPad, currentRegion = " << currentRegion;

    if(currentRegion <= (int)eCVInvalid || currentRegion >= (int)eCVInvalidMax)
        LOG2(QString("INCORRECT Region: %1").arg(currentRegion), Logger::PRIORITY_HI);
        //qDebug() << "SettingsStorage::GetDefaultKeyPad INCORRECT currentRegion";

    keypadType = GetDefaultSetting( SettingsDef::DB_KEY_ENGLISH_KEYPAD );
    setEnglishKeypad(keypadType);
    SaveSetting( keypadType, SettingsDef::DB_KEY_ENGLISH_KEYPAD );
    m_Engine->NotifyApplication( SettingsDef::DB_KEY_ENGLISH_KEYPAD, keypadType, "" );

    keypadType = GetDefaultSetting( SettingsDef::DB_KEY_KOREAN_KEYPAD );
    setKoreanKeypad(keypadType);
    SaveSetting( keypadType, SettingsDef::DB_KEY_KOREAN_KEYPAD );
    m_Engine->NotifyApplication( SettingsDef::DB_KEY_KOREAN_KEYPAD, keypadType, "" );

    keypadType = GetDefaultSetting( SettingsDef::DB_KEY_ARABIC_KEYPAD );
    setArabicKeypad(keypadType);
    SaveSetting( keypadType, SettingsDef::DB_KEY_ARABIC_KEYPAD );
    m_Engine->NotifyApplication( SettingsDef::DB_KEY_ARABIC_KEYPAD, keypadType, "");

    keypadType = GetDefaultSetting( SettingsDef::DB_KEY_CHINA_KEYPAD );
    setChinaKeypad(keypadType);
    SaveSetting( keypadType, SettingsDef::DB_KEY_CHINA_KEYPAD );
    m_Engine->NotifyApplication( SettingsDef::DB_KEY_CHINA_KEYPAD, keypadType, "");

    keypadType = GetDefaultSetting( SettingsDef::DB_KEY_EUROPE_KEYPAD );
    setEuropeKeypad(keypadType);
    SaveSetting( keypadType, SettingsDef::DB_KEY_EUROPE_KEYPAD );
    m_Engine->NotifyApplication( SettingsDef::DB_KEY_EUROPE_KEYPAD, keypadType, "" );

    keypadType = GetDefaultSetting( SettingsDef::DB_KEY_RUSSIAN_KEYPAD );
    setEuropeKeypad(keypadType);
    SaveSetting( keypadType, SettingsDef::DB_KEY_RUSSIAN_KEYPAD );
    m_Engine->NotifyApplication( SettingsDef::DB_KEY_RUSSIAN_KEYPAD, keypadType, "" );

    switch ( currentRegion )
    {
    case eCVInvalid:
    case eCVGeneral:
    case eCVKorea:
        keypadType = SettingsDef::SETTINGS_HYUNDAI_KOREAN;
        break;
    case eCVEuropa:
    case eCVRussia:
        keypadType = SettingsDef::SETTINGS_HYUNDAI_EUROPE;
        break;
    case eCVNorthAmerica:
    case eCVCanada:
        keypadType = SettingsDef::SETTINGS_HYUNDAI_ENGLISH_LATIN;
        break;
    case eCVChina:
        keypadType = SettingsDef::SETTINGS_HYUNDAI_CHINA;
        break;
    case eCVMiddleEast:
        keypadType = SettingsDef::SETTINGS_HYUNDAI_ARABIC;
        break;
    default:
        keypadType = SettingsDef::SETTINGS_HYUNDAI_KOREAN;
        break;
    }

    setHyunDaiKeypad(keypadType);
    SaveSetting( keypadType, SettingsDef::DB_KEY_HYUNDAY_KEYPAD );
    m_Engine->NotifyApplication( SettingsDef::DB_KEY_HYUNDAY_KEYPAD, keypadType, "" );
}

int SettingsStorage::GetDefaultTimeZone()
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    int retVal = 0;

    switch ( m_nCurrentRegion )
    {
    case eCVKorea:
        retVal = 540;
        break;
    case eCVNorthAmerica:
    case eCVCanada:
        retVal = -300;   //디트로이트
        break;
    case eCVChina:
        retVal = 480;
        break;
    case eCVGeneral:
    case eCVRussia:
    case eCVEuropa:
        retVal = 60;   //프랑크푸르트
        break;
    case eCVMiddleEast:
        retVal = 240;   //두바이
        break;
    default:
        retVal = 0;
        break;
    }

    return retVal;
}

int SettingsStorage::getDefaultHyundaiKeyboard()
{
    LOG2 ( QString("getDefaultHyundaiKeyboard"), Logger::PRIORITY_HI );

    int currentRegion = m_nCurrentRegion;
    int keypadType = -1;

    switch ( currentRegion )
    {
    case eCVInvalid:
    case eCVGeneral:
    case eCVKorea:
        keypadType = SettingsDef::SETTINGS_HYUNDAI_KOREAN;
        break;
    case eCVEuropa:
    case eCVRussia:
        keypadType = SettingsDef::SETTINGS_HYUNDAI_EUROPE;
        break;
    case eCVNorthAmerica:
    case eCVCanada:
        keypadType = SettingsDef::SETTINGS_HYUNDAI_ENGLISH_LATIN;
        break;
    case eCVChina:
        keypadType = SettingsDef::SETTINGS_HYUNDAI_CHINA;
        break;
    case eCVMiddleEast:
        keypadType = SettingsDef::SETTINGS_HYUNDAI_ARABIC;
        break;
    default:
        keypadType = SettingsDef::SETTINGS_HYUNDAI_KOREAN;
        break;
    }

    return keypadType;
}

void SettingsStorage::UpdateKeyboardData()
{
    //LOG2 ( QString("UpdateKeyboardData"), Logger::PRIORITY_HI );

    uint tempArray[5];
    tempArray[0] = LoadSetting(SettingsDef::DB_KEY_KOREAN_KEYPAD);
    tempArray[1] = LoadSetting(SettingsDef::DB_KEY_ENGLISH_KEYPAD);
    tempArray[2] = LoadSetting(SettingsDef::DB_KEY_EUROPE_KEYPAD);
    tempArray[3] = LoadSetting(SettingsDef::DB_KEY_CHINA_KEYPAD);
    int currentRegion = m_nCurrentRegion;

    //values from MOST LLD
    switch ( currentRegion )
    {
    case eCVInvalid:
    case eCVGeneral:
    case eCVKorea:
    case eCVNorthAmerica:
    case eCVCanada:
        tempArray[4] = 0x03;
        break;
    case eCVEuropa:
    case eCVRussia:
        tempArray[4] = 0x05;
        break;
    case eCVChina:
        tempArray[4] = 0x0B;
        break;
    case eCVMiddleEast:
        tempArray[4] = 0x13;
        break;
    default:tempArray[4] = 0x03;
        break;
    }
    //qDebug()<<"SettingsAppEngine::sendKeyboardToMostManager: " <<
    //          " key0 = "<<tempArray[0]<<
    //          " key1 = "<<tempArray[1]<<
    //          " key2 = "<<tempArray[2]<<
    //          " key3 = "<<tempArray[3]<<
    //          " key4 = "<<tempArray[4];

    CMOSTManager *instance = CMOSTManager::getInstance();

    if (instance != NULL)
    {
        instance->sendKeyboardToIBox(tempArray[0],tempArray[1],tempArray[2],tempArray[3],tempArray[4]);

    }
    //else
    //{
        //qDebug() << "SettingsAppEngine::sendKeyboardToMostManager can't create instance of CMOSTManager";
    //}
}

void SettingsStorage::ConnectToMostBoardSetting()
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    //qDebug("trying connect to MOST");
    CMOSTManager *instance = CMOSTManager::getInstance();
    instance->ConnectToMostBoardSetting(this);
}

void SettingsStorage::KeyBoardSettingChangedSlot(QDBusVariant keyboard)
{
    LOG2 ( QString("KeyBoardSettingChangedSlot"), Logger::PRIORITY_HI );

    struct QList<uint> keyBoardData;
    keyboard.variant().value<QDBusArgument>() >> keyBoardData;
    //qDebug()  << "Keyboard korean value :: " << keyBoardData.at(0)
    //          << "Keyboard English US value :: " << keyBoardData.at(1)
    //          << "Keyboard English EN value :: " << keyBoardData.at(2)
    //          << "Keyboard Chinese    value :: " << keyBoardData.at(3);

    /*
    LOG2(QString("Keyboard Settings value:[%1][%2][%3][%4][%5]")
         .arg(keyBoardData.at(0))
         .arg(keyBoardData.at(1))
         .arg(keyBoardData.at(2))
         .arg(keyBoardData.at(3))
         .arg(keyBoardData.at(4)), Logger::PRIORITY_HI );

    int defaultKeyboardVal = 0;
    //added for iBox -> HU Default Keyboard Settings Issue
    if(keyBoardData.at(0) == 0x01)
    {
        defaultKeyboardVal = SettingsDef::SETTINGS_HYUNDAI_KOREAN;
    }
    else if(keyBoardData.at(0) == 0x02)
    {
        defaultKeyboardVal = SettingsDef::SETTINGS_HYUNDAI_ENGLISH_LATIN;
    }
    else if(keyBoardData.at(0) == 0x04)
    {
        defaultKeyboardVal = SettingsDef::SETTINGS_HYUNDAI_EUROPE;
    }
    else if(keyBoardData.at(0) == 0x08)
    {
        defaultKeyboardVal = SettingsDef::SETTINGS_HYUNDAI_CHINA;
    }
    else if(keyBoardData.at(0) == 0x10)
    {
        defaultKeyboardVal = SettingsDef::SETTINGS_HYUNDAI_ARABIC;
    }
    else
    {
        defaultKeyboardVal = SettingsDef::SETTINGS_HYUNDAI_KOREAN;
    }

    SaveSetting( keyBoardData.at(1), SettingsDef::DB_KEY_KOREAN_KEYPAD );
    SaveSetting( keyBoardData.at(2), SettingsDef::DB_KEY_ENGLISH_KEYPAD );
    SaveSetting( keyBoardData.at(3), SettingsDef::DB_KEY_EUROPE_KEYPAD );;
    SaveSetting( keyBoardData.at(4), SettingsDef::DB_KEY_CHINA_KEYPAD );
    SaveSetting( defaultKeyboardVal, SettingsDef::DB_KEY_HYUNDAY_KEYPAD );

    setKoreanKeypad(keyBoardData.at(1));
    setEnglishKeypad(keyBoardData.at(2));
    setChinaKeypad(keyBoardData.at(3));
    setEuropeKeypad(keyBoardData.at(4));
    setHyunDaiKeypad(defaultKeyboardVal);

    emit changeKeypadSettings();

    //added for ITS 252116 Keyboard Settings Issue
    m_Engine->NotifyApplication(SettingsDef::DB_KEY_KOREAN_KEYPAD,keyBoardData.at(1), "" );
    m_Engine->NotifyApplication(SettingsDef::DB_KEY_ENGLISH_KEYPAD,keyBoardData.at(2),"" );
    m_Engine->NotifyApplication(SettingsDef::DB_KEY_EUROPE_KEYPAD,keyBoardData.at(3), "" );
    m_Engine->NotifyApplication(SettingsDef::DB_KEY_CHINA_KEYPAD,keyBoardData.at(4), "" );
    m_Engine->NotifyApplication( SettingsDef::DB_KEY_HYUNDAY_KEYPAD, defaultKeyboardVal, "" );
    //added for ITS 252116 Keyboard Settings Issue
    */
    

    SaveSetting( keyBoardData.at(0), SettingsDef::DB_KEY_KOREAN_KEYPAD );
    SaveSetting( keyBoardData.at(1), SettingsDef::DB_KEY_ENGLISH_KEYPAD );
    SaveSetting( keyBoardData.at(2), SettingsDef::DB_KEY_EUROPE_KEYPAD );;
    SaveSetting( keyBoardData.at(3), SettingsDef::DB_KEY_CHINA_KEYPAD );

    setKoreanKeypad(keyBoardData.at(0));
    setEnglishKeypad(keyBoardData.at(1));
    setChinaKeypad(keyBoardData.at(2));
    setEuropeKeypad(keyBoardData.at(3));

    emit changeKeypadSettings();
    
	//added for ITS 252116 Keyboard Settings Issue
    m_Engine->NotifyApplication(SettingsDef::DB_KEY_KOREAN_KEYPAD,keyBoardData.at(0), "" );
    m_Engine->NotifyApplication(SettingsDef::DB_KEY_ENGLISH_KEYPAD,keyBoardData.at(1),"" );
    m_Engine->NotifyApplication(SettingsDef::DB_KEY_EUROPE_KEYPAD,keyBoardData.at(2), "" );
    m_Engine->NotifyApplication(SettingsDef::DB_KEY_CHINA_KEYPAD,keyBoardData.at(3), "" );
	//added for ITS 252116 Keyboard Settings Issue


}



void SettingsStorage::onTimeoutProcessingVDS()
{
    LOG2 ( QString("onTimeoutProcessingVDS"), Logger::PRIORITY_HI );

    m_Engine->NotifyVideoDefaultSetAck( m_uiVDS_DeviceID, m_usVDS_SenderHandle, SettingsDef::SETTINGS_VDS_PROCESSING );
}

void SettingsStorage::SendVideoDefaultSetMost( int type, bool isSetDispOnOff )
{
    LOG2 ( QString("SendVideoDefaultSetMost"), Logger::PRIORITY_HI );

    int value, value2, value3, value4;

    switch( type )
    {
    case SettingsDef::SETTINGS_VDS_S_ALL:
    {
        //Aspect Ratio
        value = GetDefaultSetting( SettingsDef::DB_KEY_ASPECT_RADIO );
        m_Engine->NotifySetAspectRatioChange( 2 - value );

        //video image
        value = GetDefaultSetting( SettingsDef::DB_KEY_VIDEO_BRIGHTNESS );
        value2 = GetDefaultSetting( SettingsDef::DB_KEY_VIDEO_SATURATION );
        value3 = GetDefaultSetting( SettingsDef::DB_KEY_VIDEO_HUE );
        value4 = GetDefaultSetting( SettingsDef::DB_KEY_VIDEO_CONTRAST );
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_VIDEO_BRIGHTNESS, value, "", DISPLAY_FRONT );
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_VIDEO_SATURATION, value2, "", DISPLAY_FRONT );
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_VIDEO_HUE,        value3, "", DISPLAY_FRONT );
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_VIDEO_CONTRAST,   value4, "", DISPLAY_FRONT );
        //m_Engine->NotifySetVideoSettings( value, value2, value3, value4 );

        // front rear screen brightness
        value = GetDefaultSetting( SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS );
        value2 = GetDefaultSetting( SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS );
        m_Engine->NotifyFrontScreenBrightness( value );
        m_Engine->NotifyRearScreenBrightness( value2 );

        //Dimming
        value = GetDefaultSetting( SettingsDef::DB_KEY_EXPOSURE );
        m_Engine->NotifySetIlluminationMostManager( value );

        //! Rear on
        //! If it doesn't turn disp on/off due to disp state is same, send MOST message
        if ( isSetDispOnOff == false )
        {
            value = GetDefaultSetting( SettingsDef::DB_KEY_REAR_ON );
            m_Engine->NotifyDisplayOnOffMostManager( value );
        }

        break;
    }

    case SettingsDef::SETTINGS_VDS_S_LCD_BRIGHTNESS:
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS );
        value2 = GetDefaultSetting( SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS );
        m_Engine->NotifyFrontScreenBrightness( value );
        m_Engine->NotifyRearScreenBrightness( value2 );
        break;
    }

    case SettingsDef::SETTINGS_VDS_S_ILLUMINATION:
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_EXPOSURE );
        m_Engine->NotifySetIlluminationMostManager( value );
        break;
    }

    case SettingsDef::SETTINGS_VDS_S_VIDEO_RATIO:
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_ASPECT_RADIO );
        m_Engine->NotifySetAspectRatioChange( 2 - value );
        break;
    }

    case SettingsDef::SETTINGS_VDS_S_DISPLAY_ONOFF:
    {
        //! If it doesn't turn disp on/off due to disp state is same, send MOST message
        if ( isSetDispOnOff == false )
        {
            value = GetDefaultSetting( SettingsDef::DB_KEY_REAR_ON );
            m_Engine->NotifyDisplayOnOffMostManager( value );
        }
        break;
    }

    default:
        break;
    }
}

void SettingsStorage::initIBoxSettings()
{
    //LOG2 ( QString("initIBoxSettings"), Logger::PRIORITY_HI );
    //delete for not initialize langauge message to ibox 02/26
    //m_Engine->sendLanguageToIBox( (LANGUAGE_T)LoadSetting( SettingsDef::DB_KEY_LANGUAGE_TYPE ), AppEngineQMLConstants::APP_FRONT_SCREEN);



    //SetLCDBrightness
    m_Engine->NotifyFrontScreenBrightness( LoadSetting( SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS ) );
    m_Engine->NotifyRearScreenBrightness( LoadSetting( SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS ) );

    //SetIllumination
    m_Engine->NotifyDisplayModeChange( LoadSetting( SettingsDef::DB_KEY_EXPOSURE ) );

    //SetVideoRatio
    m_Engine->NotifySetAspectRatioChange( 2 - LoadSetting( SettingsDef::DB_KEY_ASPECT_RADIO ) );

    //SetKeyBoard
    UpdateKeyboardData();

    //SetDistanceUnit
    InitialDistanceUnit();

    //setBeepsetting
    //m_Engine->NotifySetBeepsetting( LoadSetting( SettingsDef::DB_KEY_SOUND_BEEP ) );
}

void SettingsStorage::initDisplayStateSettings()
{
    //LOG2 ( QString("initDisplayStateSettings"), Logger::PRIORITY_HI );

    SetDefaultSetting( SettingsDef::DB_KEY_REAR_ON );
    SetDefaultSetting( SettingsDef::DB_KEY_LREAR );
    SetDefaultSetting( SettingsDef::DB_KEY_RREAR );
}

void SettingsStorage::TimeFormatechanged(int value)
{
    LOG2 ( QString("TimeFormatechanged: %1").arg(value), Logger::PRIORITY_HI );

    emit timeFormateDataChanged(value);
}
//added for GPS Time CAN Date Issue
void SettingsStorage::GpsCheckStateChange(bool isGpsOn)
{
    LOG2 ( QString("GpsCheckStateChange"), Logger::PRIORITY_HI );

    m_Engine->setGPSTimeSetting( isGpsOn );

    emit SettingsModeChanged(isGpsOn);
}
//added for GPS Time CAN Date Issue
void SettingsStorage::SettingsModeChange(bool iSettingsMode)
{
    LOG2 ( QString("SettingsModeChange"), Logger::PRIORITY_HI );

    m_Engine->setGPSTimeSetting( iSettingsMode );

    //! If it's GPS Time
    if ( iSettingsMode == true )
    {
        m_Engine->requestGPSTimeToNavi();
        //        if ( m_Engine->isGPSAvailable() == false )
        //        {
        //            emit displayGPSUnavailablePopup();
        //        }
    }
    emit SettingsModeChanged(iSettingsMode);
}

void SettingsStorage::startProcessingVDSTimer()
{
    LOG2 ( QString("startProcessingVDSTimer"), Logger::PRIORITY_HI );

    if ( m_pProcessingVDSTimer != NULL )
    {
        m_pProcessingVDSTimer->start();
    }
    else
    {
        m_pProcessingVDSTimer = new QTimer;
        m_pProcessingVDSTimer->setInterval( 100 );
        connect(m_pProcessingVDSTimer, SIGNAL(timeout()),this, SLOT(onTimeoutProcessingVDS()));
        m_pProcessingVDSTimer->start();
    }
}

void SettingsStorage::sendResultVDS( uint sTarget, bool isSetDispOnOff )
{
    LOG2 ( QString("sendResultVDS"), Logger::PRIORITY_HI );

    if ( m_pProcessingVDSTimer )
    {
        m_pProcessingVDSTimer->stop();
    }

    m_Engine->NotifyVideoDefaultSetAck( m_uiVDS_DeviceID, m_usVDS_SenderHandle, SettingsDef::SETTINGS_VDS_RESULT );

    SendVideoDefaultSetMost( sTarget, isSetDispOnOff );
}

void SettingsStorage::setSendDispControlToIBoxFlag( SettingsDef::SETTINGS_DISPLAY_MASK_T displayType,
                                                    bool flag )
{
    LOG2 ( QString("setSendDispControlToIBoxFlag"), Logger::PRIORITY_HI );

    if ( flag == true )
    {
        m_iSendDispControlToIBox |= displayType;
    }
    else
    {
        m_iSendDispControlToIBox &= ~displayType;
    }

    //qDebug() << "SettingsStorage::setSendDispControlToIBoxFlag: Disp - " << displayType <<
    //            " Flag - " << flag << " Total Disp - " << m_iSendDispControlToIBox;
}

bool SettingsStorage::getSendDispControlToIBoxFlag( SettingsDef::SETTINGS_DISPLAY_MASK_T displayType )
{
    //LOG2 ( QString("getSendDispControlToIBoxFlag"), Logger::PRIORITY_HI );

    bool bResult = m_iSendDispControlToIBox & displayType;

    //qDebug() << "SettingsStorage::getSendDispControlToIBoxFlag: Disp- " << displayType <<
    //            " Cur Disp Flag- " << m_iSendDispControlToIBox << " Result- " << bResult;
    return bResult;
}

int SettingsStorage::GetDefaultSettingOfDispOnOff( int defaultDispOnOff )
{
    LOG2 ( QString("GetDefaultSettingOfDispOnOff"), Logger::PRIORITY_HI );

    int iLValue, iRValue, iLMaskValue, iRMaskValue, iReturnVal;

    iLMaskValue = SettingsDef::SETTINGS_DMASK_LREAR;
    iRMaskValue = SettingsDef::SETTINGS_DMASK_RREAR;
    iReturnVal = defaultDispOnOff;

    iLValue = GetDefaultSetting(SettingsDef::DB_KEY_LREAR);
    //! If Left display is on
    if ( iLValue == 1 )
    {
        iReturnVal |= iLMaskValue;
    }
    else
    {
        iReturnVal &= ~iLMaskValue;
    }

    iRValue = GetDefaultSetting(SettingsDef::DB_KEY_RREAR);
    //! If Right display is on
    if ( iRValue == 3 )
    {
        iReturnVal |= iRMaskValue;
    }
    else
    {
        iReturnVal &= ~iRMaskValue;
    }

    return iReturnVal;
}

bool SettingsStorage::isDiffWithCurDisplayState( SettingsDef::SETTINGS_DISPLAY_MASK_T displayType, int value )
{
    LOG2 ( QString("isDiffWithCurDisplayState"), Logger::PRIORITY_HI );

    bool bResult = true;

    int iLoadValue = LoadSetting( SettingsDef::DB_KEY_REAR_ON );
    if ( ( value & displayType ) == ( iLoadValue & displayType ) )
    {
        bResult = false;
    }

    //qDebug() << "SettingsStorage::isDiffWithCurDisplayState: Disp- " << displayType <<
    //            " Cur Val- " << iLoadValue << " Will Val- " << value << " isDiff- " << bResult;

    return bResult;
}

void SettingsStorage::onSetLRearState(int value)
{
    LOG2 ( QString("onSetLRearState: value:%1").arg(value), Logger::PRIORITY_HI );

    //qDebug()<<"The Lrear saved+ ="<<value;

    int iRearOn = 1;

    //if (leftRearScreen())   iRearOn += 1;
    if (rightRearScreen())   iRearOn += 4;

    int iMaskValue = SettingsDef::SETTINGS_DMASK_LREAR;

    //! If LRear is on
    if ( value == 1 )
    {
        iRearOn |= iMaskValue;
        setLeftRearScreen(true);
    }
    else
    {
        iRearOn &= ~iMaskValue;
        setLeftRearScreen(false);
    }

    if ( getSendDispControlToIBoxFlag( SettingsDef::SETTINGS_DMASK_LREAR ) == true )
    {
        m_Engine->NotifyDisplayOnOffMostManager( iRearOn );
    }
    else
    {
        //qDebug() << "IBox sent Rear1 on/off message. Do not sent MOST message.";
        setSendDispControlToIBoxFlag( SettingsDef::SETTINGS_DMASK_LREAR, true );
    }
}

void SettingsStorage::onSetRRearState(int value)
{
    LOG2 ( QString("onSetRRearState: value:%1").arg(value), Logger::PRIORITY_HI );

    //qDebug()<<"The Rrear saved+ ="<<value;

    int iRearOn = 1;

    //R1 on
    if (leftRearScreen())   iRearOn += 2;

    //R2 on
    //if (rightRearScreen())   iRearOn += 2;

    int iMaskValue = SettingsDef::SETTINGS_DMASK_RREAR;

    //! If RRear is on
    if ( value == 3 )
    {
        iRearOn |= iMaskValue;
        setRightRearScreen(true);
    }
    else
    {
        iRearOn &= ~iMaskValue;
        setRightRearScreen(false);
    }

    if ( getSendDispControlToIBoxFlag( SettingsDef::SETTINGS_DMASK_RREAR ) == true )
    {
        m_Engine->NotifyDisplayOnOffMostManager( iRearOn );
    }
    else
    {
        //qDebug() << "IBox sent Rear2 on/off message. Do not sent MOST message.";
        setSendDispControlToIBoxFlag( SettingsDef::SETTINGS_DMASK_RREAR, true );
    }
}

void SettingsStorage :: setSHMForKeypad ()
{
    //LOG2 ( QString("setSHMForKeypad"), Logger::PRIORITY_HI );

    if(m_pSharedMemoryForKeypad == NULL)
    {
        m_pSharedMemoryForKeypad = new QSharedMemory("QwertyKeypadInformation");
    }

    if(m_pSharedMemoryForKeypad)
    {
        if(!m_pSharedMemoryForKeypad->create(SHM_KEYPAD_INFO_TOTAL_SIZE, QSharedMemory::ReadWrite)) {
            //LOG2 ( QString("m_pSharedMemoryForKeypad->create() failed!!"), Logger::PRIORITY_CRITICAL );
            return;
        }

        SetSharedMemoryInfoForKeypad(SHM_OFFSET_SYS_CURRENT_REGION, (unsigned char *)&m_nCurrentRegion, SHM_SYS_CURRENT_REGION_SIZE);
        SetSharedMemoryInfoForKeypad(SHM_OFFSET_SYS_LANGUAGE_TYPE, (unsigned char *)&m_nLanguageType, SHM_SYS_LANGUAGE_TYPE_SIZE);
        SetSharedMemoryInfoForKeypad(SHM_OFFSET_EN_KEYPAD_TYPE, (unsigned char *)&m_nEnglishKeypad, SHM_EN_KEYPAD_TYPE_SIZE);
        SetSharedMemoryInfoForKeypad(SHM_OFFSET_KR_KEYPAD_TYPE, (unsigned char *)&m_nKoreanKeypad, SHM_KR_KEYPAD_TYPE_SIZE);
        SetSharedMemoryInfoForKeypad(SHM_OFFSET_ARABIC_KEYPAD_TYPE, (unsigned char *)&m_nArabicKeypad, SHM_ARABIC_KEYPAD_TYPE_SIZE);
        SetSharedMemoryInfoForKeypad(SHM_OFFSET_EU_KEYPAD_TYPE, (unsigned char *)&m_nEuropeKeypad, SHM_EU_KEYPAD_TYPE_SIZE);
        SetSharedMemoryInfoForKeypad(SHM_OFFSET_CYRILLIC_KEYPAD_TYPE, (unsigned char *)&m_nRussianKeypad, SHM_CYRILLIC_KEYPAD_TYPE_SIZE);
        SetSharedMemoryInfoForKeypad(SHM_OFFSET_DEFAULT_KEYPAD_TYPE, (unsigned char *)&m_nHyunDaiKeypad, SHM_DEFAULT_KEYPAD_TYPE_SIZE);
    }
}

void SettingsStorage :: getSharedMemoryFromUISH ()
{
    //LOG2 ( QString("getSharedMemoryFromUISH"), Logger::PRIORITY_HI );

#define _SETTING_DATA_SIZE 20
    //===========================================================
    // Create Shared memory

    if(m_pSharedMemory == NULL)
        m_pSharedMemory = new QSharedMemory("UISHStatusInformation");

    if (!m_pSharedMemory->attach(QSharedMemory::ReadWrite)) {
        LOG2 ( QString("getSharedMemoryFromUISH()attach() failed!!"), Logger::PRIORITY_CRITICAL );
        return;
    }

    // Lock shared memory - TODO :: Needs time out(Timer)
    bool bValid = m_pSharedMemory->lock();

    int size  = m_pSharedMemory->size();

    // check error
    if(!bValid)
    {
        // debug message
        //qDebug("SettingsAppEngine: m_pSharedMemory->lock failed");
        // set with default value

        m_pSharedMemory->unlock();
        return;
    }
    if (m_pSharedMemory->size() < _SETTING_DATA_SIZE)
    {
        // Unlock shared memory
        m_pSharedMemory->unlock();

        // debug message
        //qDebug("SettingsAppEngine: m_pSharedMemory->size() is null");
        LOG2 ( QString("getSharedMemoryFromUISH() size Null"), Logger::PRIORITY_CRITICAL );
        // set with default value
        return;
    }

    // Additional check
    if (m_pSharedMemory->size() > _SETTING_DATA_SIZE)
    {
        //qDebug("SettingsAppEngine: m_pSharedMemory->size() is greater than spec(20)");
        LOG2 ( QString("getSharedMemoryFromUISH() size Over"), Logger::PRIORITY_CRITICAL );
    }

    //===========================================================
    // Read Shared memory : Shared Memory to temp storage (data)
    //qDebug()<< "S_MFUISH size: " << m_pSharedMemory->size(); // Needs Check SharedMemory Size (20byte)
    //LOG2(QString("S_M UISH size: %1").arg(m_pSharedMemory->size()), Logger::PRIORITY_HI);

#define _SETTING_TEMP_DATA_SIZE (_SETTING_DATA_SIZE + 20)

    uint8_t a;
    qint8 data[_SETTING_TEMP_DATA_SIZE] = {0, };
    if (size > _SETTING_TEMP_DATA_SIZE) size = _SETTING_TEMP_DATA_SIZE;
    memcpy(data, m_pSharedMemory->constData(), size);

    // Unlock shared memory
    m_pSharedMemory->unlock();

    //===========================================================
    // Ready to read real data from temporal memroy : data[]
    qint8* ptr = data; // pointer to current data

    //===========================================================
    // log for verification
#if 0
    QString str; // hex Test
    for(int i=0; i<size; i++)
    {
        a = *(uint8_t*)ptr;
        qDebug()<<"SettingsStorage::getSharedMemoryFromUISH()::value of index=>" << i << " : " << a;
        str = QString("%1").arg(a, 0, 16);

        qDebug()<<"SettingsStorage::getSharedMemoryFromUISH()::value of index<HEX>=> " << i << " : " << str << "\n";
        ptr += 1;
    }
#endif

    // temporal values
    uint16_t temp16;
    uint32_t temp32;
    //===========================================================
    // 1. Language (4byte)
    // {spec note}

    //.language = *((uint32_t*)ptr);
    ptr += sizeof(uint32_t);
    //===========================================================
    // 2. SYS Option Set
    //.vehicle variant = *((uint8_t*)ptr);
    ptr += sizeof(uint8_t);

    //.Country variant = *((uint8_t*)ptr);
    ptr += sizeof(uint8_t);

    temp32 = *((uint32_t*)ptr); // big endian
    ptr += sizeof(uint32_t);
    temp32 = ((temp32 << 24) & 0xFF000000)
            | ((temp32 <<  8) & 0x00FF0000)
            | ((temp32 >>  8) & 0x0000FF00)
            | ((temp32 >> 24) & 0x000000FF);
    //.System variant = temp32;

    /** Need to add property about kind of amp */

    //.DVD Region = *((uint8_t*)ptr);
    ptr += sizeof(uint8_t);

    temp16 = *((uint16_t*)ptr); // big endian
    ptr += sizeof(uint16_t);
    temp16 = ((temp16 << 8) & 0xFF00)
            | ((temp16 >> 8) & 0x00FF);
    //.DVD Menu Language = temp16;

    //===========================================================
    // 3. SYS Boot Type
    //.Booting Type = *((uint8_t*)ptr);
    ptr += sizeof(uint8_t);
    //.Power Status = *((uint8_t*)ptr);
    ptr += sizeof(uint8_t);
    //.Buyer = *((uint8_t*)ptr);
    ptr += sizeof(uint8_t);
    //.Car Type = *((uint8_t*)ptr);
    ptr += sizeof(uint8_t);

    //===========================================================
    // 3. SYS Last Statuse
#define _SETTING_CAR_IF_BITMASK_ACC            0x0001 // bit 0
#define _SETTING_CAR_IF_BITMASK_ALT            0x0002 // bit 1
#define _SETTING_CAR_IF_BITMASK_ILL            0x0004 // bit 2
#define _SETTING_CAR_IF_BITMASK_PARKING        0x0008 // bit 3
#define _SETTING_CAR_IF_BITMASK_REAR_GEAR      0x0010 // bit 4
#define _SETTING_CAR_IF_BITMASK_AUX            0x0020 // bit 5

#define _SETTING_CAR_IF_BITMASK_AUTO_LIGHT     0x0100 // bit 8
#define _SETTING_CAR_IF_BITMASK_IGN            0x0200 // bit 9
#define _SETTING_CAR_IF_BITMASK_REAR_MONITOR_L 0x0400 // bit 10
#define _SETTING_CAR_IF_BITMASK_RSE_LOCK       0x0800 // bit 11
#define _SETTING_CAR_IF_BITMASK_REAR_MONITOR_R 0x1000 // bit 12

    uint16_t car_if_value = *((uint16_t*)ptr);
    ptr += sizeof(uint16_t);

    //.Rear Monitor L = car_if_value & _SETTING_CAR_IF_BITMASK_REAR_MONITOR_L;
    int leftRearScreen  = car_if_value & _SETTING_CAR_IF_BITMASK_REAR_MONITOR_L;
    setLeftRearScreen((leftRearScreen==1) ? true:false);

    //qDebug()<<"SettingsStorage::getSharedMemoryFromUISH()::leftRearScreen=>"<<leftRearScreen;

    //.RSE           = car_if_value & _SETTING_CAR_IF_BITMASK_RSE_LOCK;
    int rearLock = car_if_value & _SETTING_CAR_IF_BITMASK_RSE_LOCK;
    setRearLockScreen((rearLock==1) ? true: false);

    //.Rear Monitor R = car_if_value & _SETTING_CAR_IF_BITMASK_REAR_MONITOR_R;
    int rightRearScreen  = car_if_value & _SETTING_CAR_IF_BITMASK_REAR_MONITOR_L;
    setRightRearScreen((rightRearScreen==1)? true:false);

    /*
    //2. Sys Option (9byte)
    newData.at(4);  // Vehicle Variant - 0x00:DH, 0x01:KH, 0x02:VI, 0x03~0xff:reserved
    newData.at(5);  // Country Variant - 0x00:Korea, 0x01:N.A, 0x02:China, 0x03:General, 0x04:M.E, 0x05:Europe, 0x06:Canada
    newData.at(6);  // System Variant - Refer to DH VIT Variant Coding, Spec_A0110412.pdf 3~4 Page
    newData.at(7);  // System Variant - Refer to DH VIT Variant Coding, Spec_A0110412.pdf 3~4 Page
    newData.at(8);  // System Variant - Refer to DH VIT Variant Coding, Spec_A0110412.pdf 3~4 Page
    newData.at(9);  // System Variant - Refer to DH VIT Variant Coding, Spec_A0110412.pdf 3~4 Page
    newData.at(10); // DVD Region - 0x01:N.A, 0x02:General/M.E/Europe, 0x03:Korean, 0x06:China
    newData.at(11); // DVD Menu Language - CV 0x00 -> ko:0x6B6F, CV 0x01 -> en:0x656E, CV 0x02 -> zh:0x7A68,
    newData.at(12); // DVD Menu Language - CV 0x03 -> en:0x656E, CV 0x04 -> en:0x656E, CV 0x05 -> en:0x656E

    //Sys Boot Type (4byte)
    newData.at(13); // Booting Type - 0x00:Normal(Acc Off->On), 0x01:Power-Init (Micom-Reset), 0x02:Surge, 0x03:Keyless, 0x04:CD-Insert, 0x05:Logic-On
    newData.at(14); // Power State - 0x00:Normal Off, 0x01:Normal On, 0x10:Logic-Off, 0x11:Logic-On
    newData.at(15); // Buyer - 0x00:HMC, 0x01:KMC
    newData.at(16); // Car Type - 0x00:DH, 0x01:KH, 0x02:VI

    //Sys Last Status (3byte)
    newData.at(17); // Last GUI - 0x00:Other Mode(HOME), 0x01:Navi(Map)Mode, 0x02:iBox Mode, 0x03:AV Mode
    newData.at(18); // Car I/F - 0:ACC(0-Off, 1-On), 1:ALT(), 2:ILL(), 3:Parking(), 4:Rear-Gear(),5:Aux,6:reserved, 7:reserved
    newData.at(19); // Car I/F - 8:Auto-Light, 9:IGN, 10:Rear-Monitor-L, 11:RearLock, 12:Rear-Monitor-R, 13:reserved, 14:reserved, 15:reserved

    //char *newData = (char*) m_pSharedMemory->constData();

    //language:4byte, SysOption:9byte, SysBootType:4byte, SysLastStatus:3byte)
    //offset point (0, 4, 13, 17)
    */

    int nPhotoFrame = -1;
    if(GetSharedMemoryInfo( SHM_OFFSET_SETTING_VALUE, (unsigned char *)&nPhotoFrame, SHM_SETTING_VALUE_SIZE ))
    {
        //LOG2 ( QString("GetSharedMemoryInfo() is true. nPhotoFrame = %1").arg(nPhotoFrame), Logger::PRIORITY_HI );

        if(nPhotoFrame == 2)
        {
            int iResetValue = 0;
            setPhotoFrame(iResetValue);

            m_Engine->NotifyApplication( SettingsDef::DB_KEY_PHOTO_FRAME, iResetValue, "");

            // Save MMUS
            NotifyPhotoFrameChanged(iResetValue);

            // Save SharedMemory
            SetSharedMemoryInfo( SHM_OFFSET_SETTING_VALUE, (unsigned char *)&iResetValue, SHM_SETTING_VALUE_SIZE );

        }
        else
        {
            setPhotoFrame(nPhotoFrame);
        }
    }

    QDBusMessage message = QDBusMessage::createMethodCall("com.lge.car.micom", "/SysInfo","com.lge.car.micom.sysinfo", "LgeDBus_AskSettingValueMsg");
    QDBusReply<QByteArray> reply = QDBusConnection::sessionBus().call(message,QDBus::Block,1000);
    QByteArray replyArgs = reply.value();

    //LOG2 ( QString("Get photoFrame from MMUS : replyArgs.length() = %1").arg(replyArgs.length()), Logger::PRIORITY_HI );
    //LOG2 ( QString("Get photoFrame from MMUS : replyArgs.at(0) = %1").arg((int)replyArgs.at(0)), Logger::PRIORITY_HI );
    LOG2 ( QString("sharedMem photoFrame :[%1][%2]").arg((int)replyArgs.at(0)).arg((int)replyArgs.at(1)), Logger::PRIORITY_HI );
}

void SettingsStorage :: connectDBUSInterface()
{
    //LOG2 ( QString("connectDBUSInterface"), Logger::PRIORITY_HI );

    if (!conn.isConnected())
    {
        //qDebug()<<"SettingsStorage :: connectDBUSInterface() connect fail!!";
        return;
    }

    soundSetupControl = QDBusMessage::createMethodCall( "com.lge.car.micom", "/","com.lge.car.micom.audioif", "ProcessAudioControlMessage" );

    if(conn.connect("com.lge.car.eng.service", "/", "com.lge.car.eng", "variantInfo", this, SLOT(responseCountryVariantInfo(int))))
    {
        //qDebug()<<"SettingsStorage::connectDBUSInterface() => m_responseCountryVariant connect success!!";
        LOG2 ( QString("responseCountryVariantInfo OK!"), Logger::PRIORITY_LOW );
    }
    else{
        //qDebug()<<"SettingsStorage::connectDBUSInterface() => m_responseCountryVariant connect failed!!";
        LOG2 ( QString("responseCountryVariantInfo Fail!"), Logger::PRIORITY_LOW );
    }


    if(conn.connect("com.lge.car.micom", "/Lcd", "com.lge.car.micom.lcd", "LgeDBus_EventLCDSetupInfo", this, SLOT(LgeDBus_EventLCDSetupInfo(QByteArray))))
    {
        //qDebug()<<"SettingsStorage::connectDBUSInterface() => LgeDBus_EventLCDSetupInfo connect success!!";
        LOG2 ( QString("LgeDBus_EventLCDSetupInfo OK!"), Logger::PRIORITY_LOW );
    }
    else
    {
        //qDebug()<<"SettingsStorage::connectDBUSInterface() => LgeDBus_EventLCDSetupInfo connect failed!!";
        LOG2 ( QString("LgeDBus_EventLCDSetupInfo Fail!"), Logger::PRIORITY_LOW );
    }

    if(conn.connect("com.lge.car.micom", "/", "com.lge.car.micom.audioif", "EventMessage", this, SLOT(EventMessage(QDBusMessage))))
    {
        //qDebug()<<"SettingsStorage::connectDBUSInterface() => EventMessage connect success!!";
        LOG2 ( QString("EventMessage OK!"), Logger::PRIORITY_LOW );
    }
    else
    {
        //qDebug()<<"SettingsStorage::connectDBUSInterface() => EventMessage connect failed!!";
        LOG2 ( QString("EventMessage Fail!"), Logger::PRIORITY_LOW );
    }
}

int SettingsStorage :: requestSoundSettigValue()
{
    //LOG2 ( QString("requestSoundSettigValue"), Logger::PRIORITY_HI );

    bool isSentMsg = false;
    QList<QVariant> args;
    args.append( c_CntlAudioSetupInfoGetCommand );
    soundSetupControl.setArguments(args);

    isSentMsg = conn.callWithCallback(soundSetupControl, this, SLOT(DBusMicomResponseHandler(QDBusMessage)),SLOT(ErrorResponseHandler()),-1 );

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );

    return isSentMsg;
}

void SettingsStorage :: requestDisplaySettigValue()
{
    //LOG2 ( QString("requestDisplaySettigValue"), Logger::PRIORITY_HI );

    QDBusInterface interface( "com.lge.car.micom",
                              "/Lcd",
                              "com.lge.car.micom.lcd");

    QDBusReply<QByteArray> reply = interface.call("LgeDBus_AskLcdSetInfoValue");

    if (reply.isValid())
    {
        QByteArray tmp(reply.value());
        int mode = tmp.at(0);
        int status = tmp.at(1);
        int front_step = tmp.at(2);
        int rear_step = tmp.at(3);

        LOG2 ( QString("rDSval: Mode(%1), Status(%2), F_Step(%3), R_Step(%4)").arg(mode).arg(status).arg(front_step).arg(rear_step), Logger::PRIORITY_HI );

        switch (mode)
        {
        case 0: // night
            SaveSetting(2, SettingsDef::DB_KEY_EXPOSURE);
            setExposure(2);
            break;
        case 1: // day
            SaveSetting(1, SettingsDef::DB_KEY_EXPOSURE);
            setExposure(1);
            break;
        case 2: // auto
            SaveSetting(0, SettingsDef::DB_KEY_EXPOSURE);
            setExposure(0);
            break;
        }

        SaveSetting(front_step, SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS);
        setFrontScreenBrightness(front_step);

        SaveSetting(rear_step, SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS);
        setRearScreenBrightness(rear_step);
    }
    else
    {
        LOG2 ( QString("# reply.invalid!!"), Logger::PRIORITY_HI );
    }

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

void SettingsStorage :: DBusMicomResponseHandler(QDBusMessage reply)
{
    //LOG2 ( QString("--->"), Logger::PRIORITY_HI );

    //qDebug()<<"SettingsStorage:: In Dbus micom response handler";
    QList<QVariant> list;

    list = reply.arguments();
    //qDebug()<<"SettingsStorage::Response received from Micom";

    if(list.isEmpty())
    {
        //qCritical()<<"SettingsStorage::No Arguments in the Message";
        LOG2 ( QString("DBusMRH:No Arg!"), Logger::PRIORITY_HI );
    }
    quint16 responseCommand = list.first().toInt();

    switch(responseCommand)
    {
    case c_ResAudioSetupInfoGetCommand:
    {
        getAudioSetupResponse(list);
        break;
    }
    default:
    {
        break;
    }

    }

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

void SettingsStorage::getAudioSetupResponse(QList<QVariant> list)
{
    //LOG2 ( QString("--->  Response Received From Micom Manager for Get Audio Setup Request and the same is sending to app"), Logger::PRIORITY_HI );

    int value;
    quint8 nBass = list.at(1).toUInt();
    value = nBass - 10;

    //    if(nBass>=0 && nBass<=20)
    //        setLowTone(value);
    if(nBass<=20) //modify for static test 01/08
        setLowTone(value);
    else
    {
        LOG2 ( QString("getASR incorrect value(LowTone : %1)!").arg(nBass), Logger::PRIORITY_HI );
        loadFromProperty(SettingsDef::DB_KEY_SOUND_LOWTONE);
        NotifyBassChanged(m_nLowTone);
    }

    quint8 nMid = list.at(2).toUInt();
    value = nMid - 10;

    //    if(nMid>=0 && nMid<=20)
    //        setMidTone(value);
    if(nMid<=20) //modify for static test 01/08
        setMidTone(value);
    else
    {
        LOG2 ( QString("getASR incorrect value(MidTone : %1)!").arg(nMid), Logger::PRIORITY_HI );
        loadFromProperty(SettingsDef::DB_KEY_SOUND_MIDTONE);
        NotifyMidChanged(m_nMidTone);
    }

    quint8 nTreble = list.at(3).toUInt();
    value = nTreble - 10;

    //    if(nTreble>=0 && nTreble<=20)
    //        setHighTone(value);
    if(nTreble<=20) //modify for static test 01/08
        setHighTone(value);
    else
    {
        LOG2 ( QString("getASR incorrect value(HighTone : %1)!").arg(nTreble), Logger::PRIORITY_HI );
        loadFromProperty(SettingsDef::DB_KEY_SOUND_HIGHTONE);
        NotifyTrebleChanged(m_nHighTone);
    }

    quint8 nBalance = list.at(4).toUInt();
    value = nBalance;
    //modify for static test defect 01/13
    //    if(nBalance>=0 && nBalance<=20)
    //        setBalance(value);
    if(nBalance<=20)
        setBalance(value);
    else
    {
        LOG2 ( QString("getASR incorrect value(Balance : %1)!").arg(nBalance), Logger::PRIORITY_HI );
        loadFromProperty(SettingsDef::DB_KEY_SOUND_BALANCE);
        NotifyBalanceChanged(m_nBalance);
    }

    quint8 nFader = list.at(5).toUInt();
    value = nFader;

    //modify for static test defect 01/13
    //    if(nFader>=0 && nFader<=20)
    //        setFader(value);
    if(nFader<=20)
        setFader(value);
    else
    {
        LOG2 ( QString("Received incorrect value(Fade : %1)!").arg(nFader), Logger::PRIORITY_HI );
        loadFromProperty(SettingsDef::DB_KEY_SOUND_FADER);
        NotifyBalanceChanged(m_nFader);
    }

    quint8 nVariableEQ = list.at(6).toUInt();
    setVariableEQ(nVariableEQ);

    quint8 nAVCSDVC = list.at(7).toUInt();
    setAVC((bool)nAVCSDVC);

    quint8 nSurround = list.at(8).toUInt();
    setSurround((bool)nSurround);

    quint8 nBeep = list.at(9).toUInt();
    setBeepSound((bool)nBeep);
    m_Engine->SendBeepSettingToIBox((uint)nBeep);

    quint8 nVolRatio = list.at(10).toUInt();
    //added for DH volume Ratio spec modification
    if(nVolRatio == 1)
    {
        setVolumeRatio(0);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_VOLUME_RATIO, 0, "");
        SaveSetting( 0, SettingsDef::DB_KEY_SOUND_VOLUME_RATIO, true );
    }
    else
    {
        setVolumeRatio(nVolRatio);
    }

    quint8 nVipSound = list.at(11).toUInt();
    setVipSound((bool)nVipSound);

    quint8 nPowerBass = list.at(12).toUInt();
    quint8 nWelcoming = list.at(13).toUInt();

    LOG2 ( QString("Bass [%1], Mid [%2], Treble [%3], Balance [%4], Fader [%5]")
           .arg(nBass).arg(nMid).arg(nTreble).arg(nBalance).arg(nFader), Logger::PRIORITY_HI );

    LOG2 ( QString("SDVC [%1], Surround [%2], Beep [%3], VolRatio [%4]")
           .arg(nAVCSDVC).arg(nSurround).arg(nBeep).arg(nVolRatio), Logger::PRIORITY_HI );

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

/*
bool SettingsStorage :: NotifyValueChanged()
{
    QDBusMessage reply;
    audiosetupptr = (CntlAudioSetupArg *)Data;
    audiosetuptype = audiosetupptr->type;
    audiosetupvalue = audiosetupptr->value;

    list.append(QVariant(c_CntlAudioSetupValueSetCommand));
    list.append(QVariant(audiosetuptype));
    list.append(QVariant(audiosetupvalue));

    qDebug()<<"QRPMClient::Setting AudioSetupValue to micom Setup Type="<<audiosetuptype<<"Setup value="<<audiosetupvalue;

    msg.setArguments(list);

    reply = QDBusConnection::sessionBus().call( msg, QDBus::NoBlock, -1);
    return false;
}
*/

bool SettingsStorage :: NotifyPhotoFrameChanged(int value)
{
    //LOG2 ( QString("--->"), Logger::PRIORITY_HI );

    bool isNotifySuccess = false;

    uchar args1 = (uchar)value;
    uchar args2 = (uchar)0xFF;

    QByteArray args;
    args.append(args1);
    args.append(args2);

    QDBusMessage photoFrameSetupControl = QDBusMessage::createMethodCall("com.lge.car.micom", "/SysInfo","com.lge.car.micom.sysinfo", "LgeDBus_SettingValueStatusMsg");
    photoFrameSetupControl << qVariantFromValue(args);
    isNotifySuccess = conn.send(photoFrameSetupControl);

    if(!isNotifySuccess)
    {
        LOG2 ( QString("PhotoFrame's' send failed!!"), Logger::PRIORITY_CRITICAL );
    }

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );

    return isNotifySuccess;
}

bool SettingsStorage :: NotifyBassChanged(int value)
{
    //LOG2 ( QString("--->"), Logger::PRIORITY_HI );

    bool isNotifySuccess = false;
    QList<QVariant> args;
    args.append( c_CntlAudioSetupValueSetCommand );
    args.append( SettingsDef::SETTINGS_SOUND_BASS);
    args.append( (quint8) value);
    soundSetupControl.setArguments(args);

    isNotifySuccess = conn.send(soundSetupControl);
    LOG2 ( QString("Bass value = %1").arg(value), Logger::PRIORITY_HI );

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
    return isNotifySuccess;
}

bool SettingsStorage :: NotifyMidChanged(int value)
{
    //LOG2 ( QString("--->"), Logger::PRIORITY_HI );

    bool isNotifySuccess = false;
    QList<QVariant> args;
    args.append( c_CntlAudioSetupValueSetCommand );
    args.append( SettingsDef::SETTINGS_SOUND_MIDDLE );
    args.append( (quint8) value);
    soundSetupControl.setArguments(args);

    isNotifySuccess = conn.send(soundSetupControl);
    LOG2 ( QString("Mid value = %1").arg(value), Logger::PRIORITY_HI );

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
    return isNotifySuccess;
}

bool SettingsStorage :: NotifyTrebleChanged(int value)
{
    //LOG2 ( QString("--->"), Logger::PRIORITY_HI );

    bool isNotifySuccess = false;
    QList<QVariant> args;
    args.append( c_CntlAudioSetupValueSetCommand );
    args.append( SettingsDef::SETTINGS_SOUND_TREBLE );
    args.append( (quint8) value);
    soundSetupControl.setArguments(args);

    isNotifySuccess = conn.send(soundSetupControl);
    LOG2 ( QString("Treble value = %1").arg(value), Logger::PRIORITY_HI );

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
    return isNotifySuccess;
}

bool SettingsStorage ::
NotifyBalanceChanged(int value)
{
    //LOG2 ( QString("--->"), Logger::PRIORITY_HI );

    bool isNotifySuccess = false;
    QList<QVariant> args;
    args.append( c_CntlAudioSetupValueSetCommand );
    args.append( SettingsDef::SETTINGS_SOUND_BALANCE );
    args.append( (quint8) value);
    soundSetupControl.setArguments(args);

    isNotifySuccess = conn.send(soundSetupControl);
    LOG2 ( QString("Balance value = %1").arg(value), Logger::PRIORITY_HI );

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
    return isNotifySuccess;
}

bool SettingsStorage :: NotifyFaderChanged(int value)
{
    //LOG2 ( QString("--->"), Logger::PRIORITY_HI );

    bool isNotifySuccess = false;
    QList<QVariant> args;
    args.append( c_CntlAudioSetupValueSetCommand );
    args.append( SettingsDef::SETTINGS_SOUND_FADER );
    args.append( (quint8) value);
    soundSetupControl.setArguments(args);

    isNotifySuccess = conn.send(soundSetupControl);
    LOG2 ( QString("Fader value = %1").arg(value), Logger::PRIORITY_HI );

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
    return isNotifySuccess;
}

bool SettingsStorage :: NotifyVariableEQChanged(int value)
{
    //LOG2 ( QString("--->"), Logger::PRIORITY_HI );

    bool isNotifySuccess = false;
    QList<QVariant> args;
    args.append( c_CntlAudioSetupValueSetCommand );
    args.append( SettingsDef::SETTINGS_SOUND_VARIABLE_EQ );
    args.append( (quint8) value);
    soundSetupControl.setArguments(args);

    isNotifySuccess = conn.send(soundSetupControl);
    LOG2 ( QString("VEQ value = %1").arg(value), Logger::PRIORITY_HI );

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
    return isNotifySuccess;
}

bool SettingsStorage :: NotifyAVCChanged(int value)
{
    //LOG2 ( QString("--->"), Logger::PRIORITY_HI );

    bool isNotifySuccess = false;
    QList<QVariant> args;
    args.append( c_CntlAudioSetupValueSetCommand );
    args.append( SettingsDef::SETTINGS_SOUND_SDVC );
    args.append( (qint8) value );
    soundSetupControl.setArguments(args);

    isNotifySuccess = conn.send(soundSetupControl);

    LOG2 ( QString("AVC value = %1").arg(value), Logger::PRIORITY_HI );

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );

    return isNotifySuccess;
}

bool SettingsStorage :: NotifySurroundChanged(int value)
{
    //LOG2 ( QString("--->"), Logger::PRIORITY_HI );

    bool isNotifySuccess = false;
    QList<QVariant> args;

    args.append( c_CntlAudioSetupValueSetCommand );
    args.append( SettingsDef::SETTINGS_SOUND_SURROUND );
    args.append( (quint8) value);
    soundSetupControl.setArguments(args);

    isNotifySuccess = conn.send(soundSetupControl);

    LOG2 ( QString("Surround value = %1").arg(value), Logger::PRIORITY_HI );

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );

    return isNotifySuccess;
}

bool SettingsStorage :: NotifyBeepChanged(int value)
{
    //LOG2 ( QString("--->"), Logger::PRIORITY_HI );

    bool isNotifySuccess = false;
    QList<QVariant> args;
    args.append( c_CntlAudioSetupValueSetCommand );
    args.append( SettingsDef::SETTINGS_SOUND_BEEP );
    args.append( (quint8) value);
    soundSetupControl.setArguments(args);

    isNotifySuccess = conn.send(soundSetupControl);

    LOG2 ( QString("Beep value = %1").arg(value), Logger::PRIORITY_HI );

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
    return isNotifySuccess;
}

bool SettingsStorage :: NotifyVipSoundChanged(int value)
{
    //LOG2 ( QString("--->"), Logger::PRIORITY_HI );

    bool isNotifySuccess = false;
    QList<QVariant> args;
    args.append( c_CntlAudioSetupValueSetCommand );
    args.append( SettingsDef::SETTINGS_SOUND_VIPSOUND );
    args.append( (quint8) value);
    soundSetupControl.setArguments(args);

    isNotifySuccess = conn.send(soundSetupControl);

    LOG2 ( QString("VipS value = %1").arg(value), Logger::PRIORITY_HI );

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
    return isNotifySuccess;
}

bool SettingsStorage :: NotifyVolumeRatioChanged(int value)
{
    LOG2 ( QString("NotiVRCh"), Logger::PRIORITY_HI );

    bool isNotifySuccess = false;
    QList<QVariant> args;
    args.append( c_CntlAudioSetupValueSetCommand );
    args.append( SettingsDef::SETTINGS_SOUND_VOLUMERATIO );
    args.append( (quint8) value);
    soundSetupControl.setArguments(args);

    isNotifySuccess = conn.send(soundSetupControl);

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
    return isNotifySuccess;
}

bool SettingsStorage :: NotifySoundToneResetChanged()
{
    LOG2 ( QString("NotiSTResetCh"), Logger::PRIORITY_HI );

    bool isNotifySuccess = false;
    QList<QVariant> args;
    args.append( c_CntlAudioSetupValueSetCommand );
    args.append( SettingsDef::SETTINGS_SOUND_BASS_MID_TREBLE_CENTER );
    soundSetupControl.setArguments(args);

    isNotifySuccess = conn.send(soundSetupControl);

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );

    return isNotifySuccess;
}

bool SettingsStorage :: NotifyBalanceFaderResetChanged()
{
    LOG2 ( QString("NotiBFResetCh"), Logger::PRIORITY_HI );

    bool isNotifySuccess = false;
    QList<QVariant> args;
    args.append( c_CntlAudioSetupValueSetCommand );
    args.append( SettingsDef::SETTINGS_SOUND_BALANCE_FADER_CENTER );
    soundSetupControl.setArguments(args);

    isNotifySuccess = conn.send(soundSetupControl);

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );

    return isNotifySuccess;
}

bool SettingsStorage :: NotifyAllResetChanged()
{
    LOG2 ( QString("NotiAllResetCh"), Logger::PRIORITY_HI );

    bool isNotifySuccess = false;
    QList<QVariant> args;
    args.append( c_CntlAudioSetupValueSetCommand );
    args.append( SettingsDef::SETTINGS_SOUND_ALL_INIT );
    soundSetupControl.setArguments(args);

    isNotifySuccess = conn.send(soundSetupControl);

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );

    return isNotifySuccess;
}

void SettingsStorage :: callAudioBeepCommand()
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    QList<QVariant> args;
    args.append( c_CntlAudioBeepCommand );
    args.append( (quint8) 0x0);
    soundSetupControl.setArguments(args);
    conn.call(soundSetupControl, QDBus::NoBlock);
}

/*
void SettingsStorage::SetDefaultWatingImage(bool bForce)
{

    LOG2 ( QString(""), Logger::PRIORITY_HI );

    QString defaultWatingImage1 = "";
    QString defaultWatingImage2 = "";

    switch(m_nCurrentRegion)
    {
    case eCVKorea:
    case eCVCanada:
    case eCVNorthAmerica:
    case eCVChina:
    {
        defaultWatingImage1 = "/app/share/images/AppSettings/settings/Logo_bluelink_On.png";
        defaultWatingImage2 = "/app/share/images/AppSettings/settings/Logo_bluelink_On1.png";
        break;
    }
    case eCVGeneral:
    case eCVMiddleEast:
    case eCVEuropa:
    case eCVRussia:
    default:
    {
        defaultWatingImage1 = "/app/share/images/AppSettings/settings/Logo_bluelink_Off.png";
        defaultWatingImage2 = "/app/share/images/AppSettings/settings/Logo_bluelink_Off1.png";
        break;
    }
    }

    QString waitingImage = LoadSettingString( SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE );

    LOG2 ( waitingImage, Logger::PRIORITY_HI );

    QFile file(waitingImage);

    if (bForce)
    {
        LOG2 ( QString("[WatingImage] Set Force Default Imge"), Logger::PRIORITY_HI );

        QPixmap image1 (defaultWatingImage1);
        QPixmap image2 (defaultWatingImage2);

        if (image1.isNull() && image2.isNull())
        {
            LOG2 ( QString("Default image files are Null"), Logger::PRIORITY_CRITICAL );

            SaveSettingString("", SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE, 0);
            m_Engine->NotifyApplication(SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE, -1, "");
            setPhotoPath("");
        }

        else if (image1.isNull())
        {
            LOG2 ( QString("Default image file 1 is Null"), Logger::PRIORITY_CRITICAL );

            SaveSettingString(defaultWatingImage2, SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE, 0);
            m_Engine->NotifyApplication(SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE, -1, defaultWatingImage2);
            setPhotoPath(defaultWatingImage2);
        }

        else if (image2.isNull())
        {
            LOG2 ( QString("Default image file 2 is Null"), Logger::PRIORITY_CRITICAL );

            SaveSettingString(defaultWatingImage1, SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE, 0);
            m_Engine->NotifyApplication(SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE, -1, defaultWatingImage1);
            setPhotoPath(defaultWatingImage1);
        }

        else
        {
            SaveSettingString(defaultWatingImage1, SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE, 0);
            m_Engine->NotifyApplication(SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE, -1, defaultWatingImage1);
            setPhotoPath(defaultWatingImage1);
        }
    }

    else
    {
        QPixmap image (waitingImage);

        if (image.isNull())
        {
            LOG2 ( QString("Saved photo frame image file is Null"), Logger::PRIORITY_CRITICAL );

            SetDefaultWatingImage(true);
        }

        else
        {
            SaveSettingString(waitingImage, SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE, 0);
            m_Engine->NotifyApplication(SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE, -1, waitingImage);
            setPhotoPath(waitingImage);
        }
    }
}
*/

void SettingsStorage::updateDisplaySettingUI(int AppID)
{
    LOG2 ( QString("updateDisUI"), Logger::PRIORITY_HI );

    switch (AppID)
    {
    case APP_AVP:
    case APP_VIDEO:
    {
        //qDebug() << "[UpdateUI From VIDEO SDP] " << m_sharedVideoPlayer.GetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_BRIGHTNESS)
        //         << " " << m_sharedVideoPlayer.GetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_SATURATION)
        //         << " "<< m_sharedVideoPlayer.GetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_HUE)
        //         << " " << m_sharedVideoPlayer.GetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_CONTRAST);

        setBrightness( m_sharedVideoPlayer.GetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_BRIGHTNESS) );
        setSaturation( m_sharedVideoPlayer.GetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_SATURATION) );
        setHue( m_sharedVideoPlayer.GetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_HUE) );
        setContrast( m_sharedVideoPlayer.GetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_CONTRAST) );
    }
        break;

    case APP_MOBILETV:
    {
        //qDebug() << "[UpdateUI From DMB SDP] " << m_sharedMobileTv.GetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_BRIGHTNESS)
        //         << " " << m_sharedMobileTv.GetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_SATURATION)
        //         << " "<< m_sharedMobileTv.GetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_HUE)
        //         << " " << m_sharedMobileTv.GetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_CONTRAST);

        setBrightness( m_sharedMobileTv.GetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_BRIGHTNESS) );
        setSaturation( m_sharedMobileTv.GetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_SATURATION) );
        setHue( m_sharedMobileTv.GetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_HUE) );
        setContrast( m_sharedMobileTv.GetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_CONTRAST) );
    }
        break;
    }

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

/*
void SettingsStorage::updateFrontScreenOnOffButton(bool isFrontScreenOn)
{
    LOG2 ( QString(""), Logger::PRIORITY_HI );

    setFrontScreenOnOff(isFrontScreenOn);
}
*/

void SettingsStorage::LgeDBus_EventLCDSetupInfo (QByteArray in0)
{
    //LOG2 ( QString("EvtLcdSetInfo"), Logger::PRIORITY_HI );

    //qDebug() << "SettingsStorage :: LgeDBus_EventLCDSetupInfo size =" << in0.size() ;

    int mode = in0.at(0);
    int status = in0.at(1);
    int front_step = in0.at(2);
    int rear_step = in0.at(3);

    qDebug() << "SettingsStorage :: LgeDBus_EventLCDSetupInfo" << mode << status << front_step << rear_step;

    switch (mode)
    {
    case 0: // night
        //SaveSetting(2, SettingsDef::DB_KEY_EXPOSURE);
        setExposure(2);
        break;
    case 1: // day
        //SaveSetting(1, SettingsDef::DB_KEY_EXPOSURE);
        setExposure(1);
        break;
    case 2: // auto
        //SaveSetting(0, SettingsDef::DB_KEY_EXPOSURE);
        setExposure(0);
        break;
    }

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );

    /*
    SaveSetting(front_step, SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS);
    setFrontScreenBrightness(front_step);

    SaveSetting(rear_step, SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS);
    setRearScreenBrightness(rear_step);
    */
}

void SettingsStorage::EventMessage (QDBusMessage in0)
{
    //LOG2 ( QString("--->"), Logger::PRIORITY_HI );

    QList<QVariant> list;

    list = in0.arguments();


    if(list.isEmpty())
    {
        //qDebug() << "SettingsStorage :: EventMessage data is empty =";
        return;
    }

    //qDebug() << "SettingsStorage :: EventMessage data size =" << list.size();

    quint16 responseCommand = list.first().toInt();

    switch(responseCommand)
    {
    case c_VALAnCEGetCommand:
    {
        //qDebug() << "SettingsStorage :: EventMessage VolRatio =" << list.at(10).toUInt();
        quint8 nVolRatio = list.at(10).toUInt();

        // If current is korean, Use received-value.
        //added for DH volume Ratio spec modification
        if(nVolRatio == 1) // Sound Priority
        {
            setVolumeRatio(0); // Navi Priority
            m_Engine->NotifyApplication( SettingsDef::DB_KEY_SOUND_VOLUME_RATIO, 0, "");
            SaveSetting( 0, SettingsDef::DB_KEY_SOUND_VOLUME_RATIO, true );
        }
        else
        {
            setVolumeRatio(nVolRatio);
        }

        break;
    }
    default:
    {
        break;
    }
    }

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}



void SettingsStorage :: responseCountryVariantInfo(int countryInfo)
{
    LOG2 ( QString("resCVInfo"), Logger::PRIORITY_HI );

    //qDebug() << "SettingsStorage :: responseCountryVariantInfo()=>countryInfo:"<<countryInfo;
    int dbCountry = LoadSetting(SettingsDef::DB_KEY_CURRENT_REGION);

    if(countryInfo != dbCountry)
    {
        setCurrentRegion(countryInfo);

        //start reset
        SaveSetting(1, SettingsDef::DB_KEY_RESET_ALL_COMPLETE);

        CMOSTManager::getInstance()->sendResetSettingToIBox(0);
        ResetSoundSettings(false);
        ResetScreenSettings(false);
        ResetGeneralSettings(false, false, true); //modified for ITS 248479 Auto Time Zone Issue EU Variant

        SaveSetting(countryInfo, SettingsDef::DB_KEY_CURRENT_REGION);

        //complete reset
        SaveSetting(0, SettingsDef::DB_KEY_RESET_ALL_COMPLETE);
    }

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}


void SettingsStorage::factoryReset()
{
    LOG2 ( QString("factoryReset"), Logger::PRIORITY_HI );
    //added for DB File Crash Issue
    Close();
    usleep(SLEEP_TIME);
    //LOG2(QString("Delete DB File!!"), Logger::PRIORITY_HI);

    //deleteDBFile();

    QFile db_file( SETTINGS_FILE_DB );
    bool result = db_file.remove();
    LOG2 ( QString(" DB DELETE result = %1").arg(result), Logger::PRIORITY_HI );

    usleep(SLEEP_TIME);
    m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, m_pDBPath, SETTINGS_HM_DB, 0 , 0);
    //added for DB File Crash Issue

    ResetSoundSettings(false, true);
    ResetScreenSettings(false, true);
    ResetGeneralSettings(false, true);
    
    //added for DQMS VR DB Init value Issue
    initDisplayStateSettings();
    SaveSetting(0, SettingsDef::DB_KEY_RESET_ALL_COMPLETE);
    //added for DQMS VR DB Init value Issue

    LOG2 ( QString("<---"), Logger::PRIORITY_HI );


    //added for DB File Crash Issue

    //Close(); // delete for its 243706
    usleep(SLEEP_TIME);
    //CommandExecute("sync");
    QTimer::singleShot(1000, this,SLOT(notifyFactoryResetCompleted())); // fix for its 243706

}

void SettingsStorage::mountedNaviSDCard()
{
    //LOG2 ( QString("mountedNaviSDCard"), Logger::PRIORITY_HI );

    setNaviSDCard(true);

    //SaveSetting( 0, SettingsDef::DB_KEY_GPS_TIME_SETTINGS );

    loadFromProperty(SettingsDef::DB_KEY_GPS_TIME_SETTINGS);
    //SettingsModeChange(m_bGPSTime);//delete for GPS Time CAN Date Issue
}

void SettingsStorage::removedNaviSDCard()
{
    //LOG2 ( QString("removedNaviSDCard"), Logger::PRIORITY_HI );

    setNaviSDCard(false);

    //SaveSetting( 0, SettingsDef::DB_KEY_GPS_TIME_SETTINGS );

    //setGPSTime(false);
    //SettingsModeChange(false);//delete for GPS Time CAN Date Issue
}

void SettingsStorage::setTimeZone(int timezone, bool isShowToastPopup )
{
    LOG2 ( QString("setTimeZone"), Logger::PRIORITY_HI );

    bool bUsedPopup = isShowToastPopup;

    // Compare to DB
    if(timezone == m_nAutoTimeZone)
    {
        if(bUsedPopup)
            bUsedPopup = false;
    }
    else
    {
        setAutoTimeZone(timezone);
        SaveSetting(timezone, SettingsDef::DB_KEY_AUTO_TIMEZONE);
    }

    //updateTimeZoneToSystem(bUsedPopup);

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

void SettingsStorage::updateTimeZoneToSystem(bool isShowToastPopup)
{
    LOG2 ( QString("m_bSummerTime = %1, m_nAutoTimeZone = %2").arg(m_bSummerTime).arg(m_nAutoTimeZone) , Logger::PRIORITY_HI );

    // add for its 243298 -->
    if (((m_nAutoTimeZone >= -600) && (m_nAutoTimeZone <=540)) && (m_nAutoTimeZone != -101))
    {
    }
    else
    {
        m_nAutoTimeZone = GetDefaultTimeZone();
    }
    // add for its 243298 <--

    QFile timezone_file (TIME_ZONE_FILE);
    bool result = timezone_file.remove();

    if (m_nCurrentRegion == eCVKorea)
        setSummerTime(false);

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

    //LOG2 ( QString("<---QTime::currentTime():%1").arg(QDateTime::currentDateTime().toString()).arg(m_nAutoTimeZone), Logger::PRIORITY_HI );
}

void SettingsStorage::showTimeZoneChangedToastPopup(bool isShowPopup)
{
    LOG2 ( QString("--->isShowPopup(%1), m_nAutoTimeZone(%2)").arg(isShowPopup).arg(m_nAutoTimeZone), Logger::PRIORITY_HI );

    if(!isShowPopup)
        return;

    switch (m_nAutoTimeZone)
    {
    case -600: // Hawaii
        m_Engine->showTimeZoneChangedToastPopup(QApplication::translate("main","STR_SETTING_CLOCK_TIMEZONE_HAWAII"));
        break;
    case -540: // Alaska
        m_Engine->showTimeZoneChangedToastPopup(QApplication::translate("main","STR_SETTING_CLOCK_TIMEZONE_ALASKA"));
        break;

    case -480: // Pacific
        m_Engine->showTimeZoneChangedToastPopup(QApplication::translate("main","STR_SETTING_CLOCK_TIMEZONE_PACIFIC"));
        break;

    case -420: // Mountain
        m_Engine->showTimeZoneChangedToastPopup(QApplication::translate("main","STR_SETTING_CLOCK_TIMEZONE_MOUNTAIN"));
        break;

    case -360: // Central
        m_Engine->showTimeZoneChangedToastPopup(QApplication::translate("main","STR_SETTING_CLOCK_TIMEZONE_CENTRAL"));
        break;

    case -300: // Eastern
        m_Engine->showTimeZoneChangedToastPopup(QApplication::translate("main","STR_SETTING_CLOCK_TIMEZONE_EASTERN"));
        break;

    case -240: // Atlantic
        m_Engine->showTimeZoneChangedToastPopup(QApplication::translate("main","STR_SETTING_CLOCK_TIMEZONE_ATLANTIC"));
        break;

    case -210: // Newfoundland
        m_Engine->showTimeZoneChangedToastPopup(QApplication::translate("main","STR_SETTING_CLOCK_TIMEZONE_NEWFOUNDLAND"));
        break;
    }
    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

void SettingsStorage::StartResetScreenSetting()
{
    LOG2 ( QString("SRScreenSet"), Logger::PRIORITY_HI );

    if(m_pResetScreenThread != NULL)
    {
        if( m_pResetScreenThread->isRunning())
            return;
    }

    if(m_pResetScreenThread == NULL)
    {
        m_pResetScreenThread = new ResetSettingThread(this, SettingsDef::SETTINGS_RESET_TYPE_SCREEN);
        connect ( m_pResetScreenThread, SIGNAL(propertyResetCompleted(int, int)), this, SLOT(updateProperty(int, int)));
        connect ( m_pResetScreenThread, SIGNAL(finished()), this, SLOT(initScreenThread()));
    }

    // Screen Thread Start
    if(m_pResetScreenThread != NULL)
    {
        if( !(m_pResetScreenThread->isRunning()) )
        {
            m_pResetScreenThread->start(QThread::NormalPriority);
        }
    }

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

void SettingsStorage::initScreenThread()
{
    LOG2 ( QString("initScreenTh"), Logger::PRIORITY_HI );

    QTimer::singleShot(1000, this, SLOT(notifyResetScreenCompleted()));

    if(m_pResetScreenThread != NULL)
    {
        if( m_pResetScreenThread->isFinished() )
        {
            disconnect ( m_pResetScreenThread, SIGNAL(propertyResetCompleted(int, int)), this, SLOT(updateProperty(int, int)));
            disconnect ( m_pResetScreenThread, SIGNAL(finished()), this, SLOT(initScreenThread()));
            delete m_pResetScreenThread;
            m_pResetScreenThread = NULL;
        }//modified for ResetAll Ifinit loading Issue
        else
        {

            disconnect ( m_pResetScreenThread, SIGNAL(propertyResetCompleted(int, int)), this, SLOT(updateProperty(int, int)));
            disconnect ( m_pResetScreenThread, SIGNAL(finished()), this, SLOT(initScreenThread()));
            delete m_pResetScreenThread;
            m_pResetScreenThread = NULL;
            LOG2(QString("m_pResetScreenThread->isFinished(): false."), Logger::PRIORITY_HI);

        }//modified for ResetAll Ifinit loading Issue
    }

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

void SettingsStorage::StartResetAllSetting()
{
    isResetAll = true; //added for ITS 248640
    LOG2 ( QString("SResetAllSet"), Logger::PRIORITY_HI );


    if(m_pResetScreenThread != NULL)
    {
        //modified for ResetAll Ifinit loading Issue
        //QTimer::singleShot(200, this, SLOT(StartResetAllSetting()));
        LOG2 ( QString("Delete Screen Thread."), Logger::PRIORITY_HI );

        disconnect ( m_pResetScreenThread, SIGNAL(propertyResetCompleted(int, int)), this, SLOT(updateProperty(int, int)));
        disconnect ( m_pResetScreenThread, SIGNAL(finished()), this, SLOT(initScreenThread()));
        delete m_pResetScreenThread;
        m_pResetScreenThread = NULL;

        //return;
        //modified for ResetAll Ifinit loading Issue
    }

    if(m_pResetAllThread != NULL)
    {
        if( m_pResetAllThread->isRunning() )
            return;
    }
	//added for NAVI Canada Variant Default Setting(KM)
    if(m_nCurrentRegion == eCVCanada)
    {
        m_Engine->NotifyDistanceUnitChanged(0);

    }
    else
    {
        m_Engine->NotifyDistanceUnitChanged(1);
    }
	//added for NAVI Canada Variant Default Setting(KM)

    // Init Sound
    //ResetSoundSettings(false); //delete for Reset Popup Holding issue
    //SettingsModeChange(true); //added for GPS Time CAN Date Issue
    //GetDefaultKeyPad(); //added for ITS 223959 not init keypad default type

    if(m_pResetAllThread == NULL)
    {
        m_pResetAllThread = new ResetSettingThread(this, SettingsDef::SETTINGS_RESET_TYPE_GENERAL);
        connect ( m_pResetAllThread, SIGNAL(propertyResetCompleted(int, int)), this, SLOT(updateProperty(int, int)));
        connect ( m_pResetAllThread, SIGNAL(finished()), this, SLOT(initGeneralThread()));
    }

    if( m_pResetAllThread != NULL )
    {
        if( !(m_pResetAllThread->isRunning()) )
            m_pResetAllThread->start(QThread::NormalPriority);
    }

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
    //LOG2 ( QString("kjw :: resetGeneralSettings:: 5"), Logger::PRIORITY_HI );
}

void SettingsStorage::initGeneralThread()
{
    //LOG2 ( QString("kjw :: resetGeneralSettings:: 6"), Logger::PRIORITY_HI );
    LOG2 ( QString("initGeneralTh"), Logger::PRIORITY_HI );

    QTimer::singleShot(3000, this, SLOT(notifyResetGeneralCompleted()));

    if(m_pResetAllThread != NULL)
    {
        if( m_pResetAllThread->isFinished() )
        {
            disconnect ( m_pResetAllThread, SIGNAL(propertyResetCompleted(int, int)), this, SLOT(updateProperty(int, int)));
            disconnect ( m_pResetAllThread, SIGNAL(finished()), this, SLOT(initGeneralThread()));
            delete m_pResetAllThread;
            m_pResetAllThread = NULL;
        }
    }

    isResetAll = false; //added for ITS 248640
    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

void SettingsStorage::updateProperty(int value, int key)
{
    LOG2 ( QString("updateProperty"), Logger::PRIORITY_HI );

    switch(key)
    {
    //SCREEN
    case SettingsDef::DB_KEY_ASPECT_RADIO:
        setAspectRatio(value);
        m_Engine->NotifySetAspectRatio(value, 1);
        break;
    case SettingsDef::DB_KEY_VIDEO_BRIGHTNESS:
        setBrightness(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_VIDEO_BRIGHTNESS, value, "");
        break;
    case SettingsDef::DB_KEY_VIDEO_SATURATION:
        setSaturation(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_VIDEO_SATURATION, value, "");
        break;
    case SettingsDef::DB_KEY_VIDEO_HUE:
        setHue(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_VIDEO_HUE, value, "");
        break;
    case SettingsDef::DB_KEY_VIDEO_CONTRAST:
        setContrast(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_VIDEO_CONTRAST, value, "");
        break;
    case SettingsDef::DB_KEY_EXPOSURE:
        setExposure(value);
        m_Engine->NotifyDisplayModeChange(value);
        m_Engine->NotifySetIlluminationMostManager(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_EXPOSURE,value, "");
        break;

        // CLOCK
    case SettingsDef::DB_KEY_GPS_TIME_SETTINGS:
        setGPSTime((bool)value);
        //modify for GPS checkBox 13/12/27
        //delete for GPS Time CAN Date Issue
        //if(m_bNaviSDCard && m_bIsNaviVariant)
        //    SettingsModeChange((bool)value);
        //else
        //    SettingsModeChange(false);
        //delete for GPS Time CAN Date Issue
        //modify for GPS checkBox 13/12/27
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_GPS_TIME_SETTINGS, value, "");
        break;
    case SettingsDef::DB_KEY_SUMMER_TIME:
        setSummerTime((bool)value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_SUMMER_TIME, value, "" );
        updateTimeZoneToSystem(false);
        break;
    case SettingsDef::DB_KEY_SUMMER_TIME_SET:
        setSummerTimeSet((bool)value);
        break;
    case SettingsDef::DB_KEY_CLOCK_TYPE:
        setClockType(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_CLOCK_TYPE, value, "");
        break;
    case SettingsDef::DB_KEY_TIME_TYPE:
        setTimeType(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_TIME_TYPE, value, "");
        break;
    case SettingsDef::DB_KEY_CALENDAR_TYPE:
        setCalendarType(value); // 이거때매 죽음
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_CALENDAR_TYPE, value, "" );
        break;
    case SettingsDef::DB_KEY_DATEFORMAT_TYPE:
        setDateFormatType(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_DATEFORMAT_TYPE, value, "");
        break;

        // GENERAL
    case SettingsDef::DB_KEY_LANGUAGE_TYPE:
        setLanguageType(value);
        m_Engine->ChangeLanguage(value);
        m_Engine->sendLanguageToIBox( (LANGUAGE_T)value, AppEngineQMLConstants::APP_FRONT_SCREEN);
        break;

    case SettingsDef::DB_KEY_LOCKREARMONITOR_FUNCTION:
        setRearLockScreen(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_LOCKREARMONITOR_FUNCTION, value, "");
        break;

    case SettingsDef::DB_KEY_APPROVAL:
        setApproval(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_APPROVAL, value, "");
        break;

    case SettingsDef::DB_KEY_PHOTO_FRAME:
        // MMUS Save
        NotifyPhotoFrameChanged(value);

        // Save SharedMemory
        SetSharedMemoryInfo( SHM_OFFSET_SETTING_VALUE, (unsigned char *)&value, SHM_SETTING_VALUE_SIZE );

        setPhotoFrame(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_PHOTO_FRAME, value, "Off");
        //SetDefaultWatingImage(true);
        break;

    case SettingsDef::DB_KEY_VOICE_VOICECOMMAND:
        setVoiceCommand(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_VOICE_VOICECOMMAND, value, "");
        break;

    case SettingsDef::DB_KEY_DISTANCE_UNIT:
        setDistanceUnit(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_DISTANCE_UNIT, value, "");
        CMOSTManager::getInstance()->sendDistanceUnitToIBox(value);
        break;

    case SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS:
        setFrontScreenBrightness(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS,value, "");
        m_Engine->NotifyDisplayBrightnessChange(1, 10);
        break;

    case SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS:
        setRearScreenBrightness(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS,value, "");
        m_Engine->NotifyDisplayBrightnessChange(0, 10);
        break;

    case SettingsDef::DB_KEY_ENGLISH_KEYPAD:
        setEnglishKeypad(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_ENGLISH_KEYPAD, value, "" );
        UpdateKeyboardData();
        break;

    case SettingsDef::DB_KEY_KOREAN_KEYPAD:
        setKoreanKeypad(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_KOREAN_KEYPAD, value, "" );
        UpdateKeyboardData();
        break;

    case SettingsDef::DB_KEY_ARABIC_KEYPAD:
        setArabicKeypad(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_ARABIC_KEYPAD, value, "");
        //UpdateKeyboardData();  Not Used Arabic Keyboard(I-box)
        break;

    case SettingsDef::DB_KEY_CHINA_KEYPAD:
        setChinaKeypad(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_CHINA_KEYPAD, value, "");
        UpdateKeyboardData();
        break;

    case SettingsDef::DB_KEY_EUROPE_KEYPAD:
        setEuropeKeypad(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_EUROPE_KEYPAD, value, "" );
        UpdateKeyboardData();
        break;

    case SettingsDef::DB_KEY_RUSSIAN_KEYPAD:
        setRussianKeypad(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_RUSSIAN_KEYPAD, value, "" );
        UpdateKeyboardData();
        break;

    case SettingsDef::DB_KEY_HYUNDAY_KEYPAD:
        setHyunDaiKeypad(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_HYUNDAY_KEYPAD, value, "" );
        UpdateKeyboardData();
        break;

    case SettingsDef::DB_KEY_CURRENT_REGION:
        setCurrentRegion(value);
        break;

    case SettingsDef::DB_KEY_AUTO_TIMEZONE:
        setAutoTimeZone(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_AUTO_TIMEZONE, value, "");
        break;

    case SettingsDef::DB_KEY_AUTO_TIMEZONE_SET:
        setAutoTimeZoneSet(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_AUTO_TIMEZONE_SET, value, "" );
        break;

    case SettingsDef::DB_KEY_VR_PHONE_PRIORITY:
        setVRPhonePriority(value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_VR_PHONE_PRIORITY, value, "");
        m_Engine->SendPhonePrioritySettingToIBox(value+1);
        break;

    case SettingsDef::DB_KEY_VOICE_APPLE_SIRI:
        setAppleSiri((bool) value);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_VOICE_APPLE_SIRI, value, "");
        break;

        /*
    case SettingsDef::DB_KEY_REAR_ON:
        break;

    case SettingsDef::DB_KEY_LREAR:
        setLeftRearScreen(value);
        m_Engine->NotifyApplication(SettingsDef::DB_KEY_REAR_ON ,value,"");
        break;

    case SettingsDef::DB_KEY_RREAR:
        setRightRearScreen((value==2)?false:true);
        m_Engine->NotifyApplication(SettingsDef::DB_KEY_REAR_ON ,value,"");
        break;
        */
    }

    LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

bool SettingsStorage::SetSharedMemoryInfo(int offset, unsigned char * pBuf, int size)
{
    LOG2 ( QString("SSMInfo"), Logger::PRIORITY_HI );
    if(!m_pSharedMemory)
        return false;

    if ( !m_pSharedMemory->isAttached() )
    {
        LOG2 ( QString("Lost shared memroy -> error code : %1" ).arg( m_pSharedMemory->error()), Logger::PRIORITY_CRITICAL );
        //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
        return false;
    }

    m_pSharedMemory->lock();
    memcpy( ((unsigned char *)m_pSharedMemory->data())+offset, pBuf, size );
    m_pSharedMemory->unlock();

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
    return true;
}

bool SettingsStorage::SetSharedMemoryInfoForKeypad(int offset, unsigned char * pBuf, int size)
{
    //LOG2 ( QString("SSMInfoKeypad"), Logger::PRIORITY_HI );
    if(!m_pSharedMemoryForKeypad)
        return false;

    if ( !m_pSharedMemoryForKeypad->isAttached() )
    {
        LOG2 ( QString("Lost shared memroy -> error code : %1" ).arg( m_pSharedMemoryForKeypad->error()), Logger::PRIORITY_CRITICAL );
        //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
        return false;
    }

    m_pSharedMemoryForKeypad->lock();
    memcpy( ((unsigned char *)m_pSharedMemoryForKeypad->data())+offset, pBuf, size );
    m_pSharedMemoryForKeypad->unlock();

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
    return true;
}

bool SettingsStorage::GetSharedMemoryInfo(int offset, unsigned char * pBuf, int size)
{
    //LOG2 ( QString("--->"), Logger::PRIORITY_HI );
    if(!m_pSharedMemory)
        return false;

    if ( !m_pSharedMemory->isAttached() )
    {
        LOG2 ( QString("Lost shared memroy: err code : %1" ).arg( m_pSharedMemory->error()), Logger::PRIORITY_CRITICAL );
        //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
        return false;
    }

    m_pSharedMemory->lock();
    memcpy( pBuf, ((unsigned char *)m_pSharedMemory->data())+offset, size );
    m_pSharedMemory->unlock();

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
    return true;
}

void SettingsStorage::showToastPopup(int toastPopupType)
{
    switch(toastPopupType)
    {
        case SettingsDef::SETTINGS_TOAST_LANGUAGE_CHANGING:
        {
            emit showLanguageChangingToastPopup();
        }
            break;
        //added for ITS 217706 daylight savings time not sync
        case SettingsDef::SETTINGS_TOAST_DAYLIGHT_SAVINGS_TIME_CHANGING:
        {
            emit showDaylightTimeChangingToastPopup();
        }
            break;
        //added for ITS 217706 daylight savings time not sync
    }

}
//added for DB write Fail Issue
void SettingsStorage::InitializeDB()
{
    //LOG2(QString("-------->") , Logger::PRIORITY_HI);

    m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, m_pDBPath, SETTINGS_HM_DB, 0 , 0);
    int countryVariantFromUISH = AppEngineBase::GetCountryVariant();


    LOG2 ( QString("InitializeDB!"), Logger::PRIORITY_CRITICAL );

    setCurrentRegion(countryVariantFromUISH);

    //start reset
    SaveSetting(1, SettingsDef::DB_KEY_RESET_ALL_COMPLETE);

    ResetSoundSettings(false);
    ResetScreenSettings(false);
    ResetGeneralSettings(false);

    SaveSetting(countryVariantFromUISH, SettingsDef::DB_KEY_CURRENT_REGION);

    //complete reset
    SaveSetting(0, SettingsDef::DB_KEY_RESET_ALL_COMPLETE);

    initPropertyValue();
    //LOG2(QString("<--------") , Logger::PRIORITY_HI);

}
//added for DB write Fail Issue
//execute command cmd
QByteArray SettingsStorage::CommandExecute(const QByteArray cmd)
{
    //LOG2(QString("-------->") , Logger::PRIORITY_HI);
    QProcess syncCmd;
    QByteArray ret;

    syncCmd.startDetached(QString("%1").arg(cmd.data()));
    if(!syncCmd.waitForFinished(-1))
        return ret;

    ret = syncCmd.readAll();


    return ret;
}

//execute command cmd
QByteArray SettingsStorage::syncCommandExecuteReturn(const QByteArray cmd)
{
    QProcess syncCmd;
    QByteArray ret;

    syncCmd.start(QString("%1").arg(cmd.data()));

    if(!syncCmd.waitForFinished(-1))
        return ret;

    ret = syncCmd.readAll();


    return ret;
}
//added for DB write Fail Issue
void SettingsStorage::deleteDBFile()
{
    //LOG2(QString("-------->") , Logger::PRIORITY_HI);
    //QByteArray retval = CommandExecute("rm -rf /app/data/app_settings/SettingsScreenSettings.db");
    //LOG2( QString("DB DELETE result = %1").arg(retval.data()), Logger::PRIORITY_HI);
    //retval = CommandExecute("sync");
    //LOG2( QString("sync Command result = %1").arg(retval.data()), Logger::PRIORITY_HI);

    QFile db_file( SETTINGS_FILE_DB );
    bool result = db_file.remove();
    LOG2 ( QString("DELETE result = %1").arg(result), Logger::PRIORITY_HI );
    //usleep(SLEEP_TIME);
    //QTimer::singleShot(2000, this, SLOT(InitializeDB()));
}

void SettingsStorage::checkDBAndDelete()
{
    //LOG2(QString("-------->") , Logger::PRIORITY_HI);
    if(retryCount > 5)
    {
        retryCount = 0;
        LOG2(QString("DB File Crashed !"), Logger::PRIORITY_HI);

        Close();
        usleep(SLEEP_TIME);
        deleteDBFile();
        usleep(SLEEP_TIME);
        InitializeDB();
    }
}
//added for DB write Fail Issue
void SettingsStorage::checkResetComplete()
{
    int countryVariantFromUISH = AppEngineBase::GetCountryVariant();
    int countryVariantFromDB = LoadSetting(SettingsDef::DB_KEY_CURRENT_REGION);

    if (countryVariantFromUISH != countryVariantFromDB)
    {

        LOG2 ( QString("CVariant not match: UISH = %1, DB = %2 ").arg(countryVariantFromUISH).arg(countryVariantFromDB), Logger::PRIORITY_CRITICAL );
        //LOG2 ( QString("RESET DB!!!!!!!!!!!1"), Logger::PRIORITY_CRITICAL );

        setCurrentRegion(countryVariantFromUISH);

        //start reset
        SaveSetting(1, SettingsDef::DB_KEY_RESET_ALL_COMPLETE);

        ResetSoundSettings(false);
        ResetScreenSettings(false);
        ResetGeneralSettings(false);

        SaveSetting(countryVariantFromUISH, SettingsDef::DB_KEY_CURRENT_REGION);

        //complete reset
        SaveSetting(0, SettingsDef::DB_KEY_RESET_ALL_COMPLETE);

        initPropertyValue();

    }

    if (LoadSetting(SettingsDef::DB_KEY_RESET_ALL_COMPLETE))
    {
        //LOG2 ( QString("Do Not Completed RESET DB!!!!!!!!!!!1"), Logger::PRIORITY_CRITICAL );
        LOG2 ( QString("RETRY RESET DB!"), Logger::PRIORITY_CRITICAL );

        setCurrentRegion(countryVariantFromUISH);

        //start reset
        SaveSetting(1, SettingsDef::DB_KEY_RESET_ALL_COMPLETE);

        ResetSoundSettings(false);
        ResetScreenSettings(false);
        ResetGeneralSettings(false);

        SaveSetting(countryVariantFromUISH, SettingsDef::DB_KEY_CURRENT_REGION);

        //complete reset
        SaveSetting(0, SettingsDef::DB_KEY_RESET_ALL_COMPLETE);

        initPropertyValue();

    }
}

void SettingsStorage::chineKeypadChanged(int chineseKeypadType)
{
    LOG2 ( QString("--->chineseKeypadType:%1").arg(chineseKeypadType), Logger::PRIORITY_HI );
    if(chineseKeypadType == m_nChinaKeypad)
        return;

    switch(chineseKeypadType)
    {
    case SettingsDef::SETTINGS_CHINA_PINYIN:
    case SettingsDef::SETTINGS_CHINA_VOCAL_SOUND:
    case SettingsDef::SETTINGS_CHINA_HAND_WRITING:
        setChinaKeypad(chineseKeypadType);
        m_Engine->NotifyApplication( SettingsDef::DB_KEY_CHINA_KEYPAD, chineseKeypadType, "");
        UpdateKeyboardData();
        break;
    default:
        return;
    }

    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

// added for modified ibox time set 02/05
void SettingsStorage::onSetTimeToIBox(uchar timeFormat,
                                      uint year, uint month,
                                      uint day, uint hour,
                                      uint minute, uint second,
                                      uint dateFormat)
{

    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    //GetLogger().Log("SettingsAppEngine::onTimeChangeTimeout() cnage system time");

    CMOSTManager *instance = CMOSTManager::getInstance();

    uint m_SummerTime;
    int m_autoTime;
    LOG2( QString("SummerTime value : [%1]").arg(summerTime()), Logger::PRIORITY_HI);
    LOG2( QString("AutoTimeZone value : [%1]").arg(autoTimeZone()), Logger::PRIORITY_HI);
    m_autoTime = autoTimeZone();

    if(summerTime())
    {
        m_SummerTime = 1;
    }
    else
    {
        m_SummerTime = 0 ;
    }

    if (instance != NULL)
    {
        instance->NewsendDateTimeToIBox(timeFormat,
                                     year,
                                     month,
                                     day,
                                     hour,
                                     minute,
                                     second,
                                     dateFormat,
                                     m_SummerTime,
                                     m_autoTime   );
    }
    else
    {
        LOG2( QString("SetSystemTime can't instance"), Logger::PRIORITY_HI);
        //GetLogger().Log("SettingsAppEngine::SetSystemTime can't create instance of CMOSTManager");
    }
}

// added for modified ibox time set 02/05
