#include<DHAVN_AppSettings_SaveSettings_ResetThread.h>
#include "DHAVN_AppSettings_SaveSettings.h"
#include "DHAVN_AppSetting_Log.h"

#define SLEEP_TIME 20000

ResetSettingThread::ResetSettingThread(SettingsStorage* storage, SettingsDef::SETTINGS_RESET_TYPE_T resetType, QObject *parent)
    : m_Storage(storage), m_nResetType(resetType), QThread(parent)
{
    m_nCurrentRegion = m_Storage->variantCountry();
    m_bNaviSDCard = m_Storage->naviSDCard();
    m_bNaviSystemVariant = m_Storage->isNaviVariant();
    m_bRearRRCVariant = m_Storage->rearRRCVariant();

    m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, SETTINGS_FILE_DB, SETTINGS_HM_DB, 0 , 0);
    bool bDBSessionEnabled = m_pDBSession ? true : false;

    if( (m_nDBState != 0) || !bDBSessionEnabled)
    {
        LOG2 ( QString("DB_O Failed!! : State = %1, Enabled = %2").arg(m_nDBState).arg(bDBSessionEnabled), Logger::PRIORITY_CRITICAL );

        close();
        m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, SETTINGS_FILE_DB, SETTINGS_HM_DB, 0 , 0);
    }
}

ResetSettingThread::~ResetSettingThread()
{
    close();
}

bool ResetSettingThread::close()
{
    int result = SI_PM_OK;

    if( NULL != m_pDBSession )
    {
        result = SIPMClose( m_pDBSession );
        m_pDBSession = NULL;//added for Fact Reset-> DB Write segfault Issue
        m_nDBState = -1;//added for Fact Reset-> DB Write segfault Issue
    }
    return result == SI_PM_OK ? true : false;
}

void ResetSettingThread::run()
{


    switch(m_nResetType)
    {
    case SettingsDef::SETTINGS_RESET_TYPE_SCREEN:
    {
        resetScreenSettings();
    }
        break;
    case SettingsDef::SETTINGS_RESET_TYPE_GENERAL:
    {

        m_Storage->GpsCheckStateChange(true); //added for Reset Popup Holding issue
        m_Storage->GetDefaultKeyPad();//added for GPS Time CAN Date Issue
        m_Storage->ResetSoundSettings(false);//added for ITS 223959 not init keypad default type
        usleep(10000); // delete for test

        resetScreenSettings();

        usleep(SLEEP_TIME);

        resetGeneralSettings();
    }
        break;
    }
}

void ResetSettingThread::resetScreenSettings()
{
    int value = 0;

    value = GetDefaultSetting( SettingsDef::DB_KEY_ASPECT_RADIO );
    SaveSetting( value, SettingsDef::DB_KEY_ASPECT_RADIO );

    value = GetDefaultSetting( SettingsDef::DB_KEY_VIDEO_BRIGHTNESS );
    SaveSetting( value, SettingsDef::DB_KEY_VIDEO_BRIGHTNESS );

    value = GetDefaultSetting( SettingsDef::DB_KEY_VIDEO_SATURATION );
    SaveSetting( value, SettingsDef::DB_KEY_VIDEO_SATURATION );

    value = GetDefaultSetting( SettingsDef::DB_KEY_VIDEO_HUE );
    SaveSetting( value, SettingsDef::DB_KEY_VIDEO_HUE );

    value = GetDefaultSetting( SettingsDef::DB_KEY_VIDEO_CONTRAST );
    SaveSetting( value, SettingsDef::DB_KEY_VIDEO_CONTRAST );

    value = GetDefaultSetting( SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS );
    SaveSetting( value, SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS );

    value = GetDefaultSetting( SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS );
    SaveSetting( value, SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS );

    value = GetDefaultSetting( SettingsDef::DB_KEY_EXPOSURE );
    SaveSetting( value, SettingsDef::DB_KEY_EXPOSURE );

    value = GetDefaultSetting( SettingsDef::DB_KEY_DISPLAY_CLOCK );
    SaveSetting( value, SettingsDef::DB_KEY_DISPLAY_CLOCK );
}

void ResetSettingThread::resetGeneralSettings()
{
    int value = 0;
    SaveSetting(m_nCurrentRegion, SettingsDef::DB_KEY_CURRENT_REGION);
    //modify for GPS checkBox 13/12/27
    if(m_bNaviSystemVariant) 
        value = GetDefaultSetting( SettingsDef::DB_KEY_GPS_TIME_SETTINGS );
    else
        value = 0;
    //modify for GPS checkBox 13/12/27
    SaveSetting( value, SettingsDef::DB_KEY_GPS_TIME_SETTINGS );

    value = GetDefaultSummerTime();
    SaveSetting( value, SettingsDef::DB_KEY_SUMMER_TIME );

    value = GetDefaultSetting( SettingsDef::DB_KEY_CLOCK_TYPE );
    SaveSetting( value, SettingsDef::DB_KEY_CLOCK_TYPE );

    value = GetDefaultSetting( SettingsDef::DB_KEY_TIME_TYPE );
    SaveSetting( value, SettingsDef::DB_KEY_TIME_TYPE );

    // delete for 243957 -->
    //value = GetDefaultSetting( SettingsDef::DB_KEY_CALENDAR_TYPE );
    //SaveSetting( value, SettingsDef::DB_KEY_CALENDAR_TYPE ); // 이거때매 죽음
    // delete for 243957 <--

    value = GetDefaultSetting( SettingsDef::DB_KEY_AUTO_TIMEZONE_SET );
    SaveSetting( value, SettingsDef::DB_KEY_AUTO_TIMEZONE_SET );

    //value = GetDefaultSetting( SettingsDef::DB_KEY_AUTO_TIMEZONE );
    //SaveSetting( value, SettingsDef::DB_KEY_AUTO_TIMEZONE );

    value = GetDefaultSetting( SettingsDef::DB_KEY_PHOTO_FRAME );
    SaveSetting( value, SettingsDef::DB_KEY_PHOTO_FRAME );

    value = GetDefaultLanguage();
    SaveSetting( value, SettingsDef::DB_KEY_LANGUAGE_TYPE ); // 이거때매 끊김현상 발생

    value = GetDefaultSetting(SettingsDef::DB_KEY_DISTANCE_UNIT);
    //modify for Canada variant Distance Unit default value km
    if(m_nCurrentRegion == eCVCanada)
    {
        value = 0;
        //LOG2 ( QString("[eCVCanada]resetGeneralSettings:: Distance Unit Default value : km(0) : %1").arg(value), Logger::PRIORITY_HI );
        SaveSetting( value, SettingsDef::DB_KEY_DISTANCE_UNIT );
    }
    else
    {
        //LOG2 ( QString("[eCVCanada]resetGeneralSettings:: Distance Unit Default value : km(0) : %1").arg(value), Logger::PRIORITY_HI );
        SaveSetting( value, SettingsDef::DB_KEY_DISTANCE_UNIT );
    }
    //modify for Canada variant Distance Unit default value km

    value = GetDefaultDateTimeFormat();
    SaveSetting( value, SettingsDef::DB_KEY_DATEFORMAT_TYPE );

    //modify for NA/CA/CH Agreement Spec Modify(10 seconds, default on) 01/14
    if(m_nCurrentRegion == eCVNorthAmerica || m_nCurrentRegion == eCVCanada || m_nCurrentRegion == eCVChina)
    {
        value = 1;
        SaveSetting( value, SettingsDef::DB_KEY_APPROVAL );
    }
    else
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_APPROVAL );
        SaveSetting( value, SettingsDef::DB_KEY_APPROVAL );
    }
    //modify for NA/CA/CH Agreement Spec Modify(10 seconds, default on) 01/14

    //    value = GetDefaultSetting( SettingsDef::DB_KEY_APPROVAL );
    //    SaveSetting( value, SettingsDef::DB_KEY_APPROVAL );

    if(m_bRearRRCVariant)
    {
        value = GetDefaultSetting( SettingsDef::DB_KEY_LOCKREARMONITOR_FUNCTION );
        SaveSetting( value, SettingsDef::DB_KEY_LOCKREARMONITOR_FUNCTION );
    }

    value = GetDefaultSetting( SettingsDef::DB_KEY_VR_PHONE_PRIORITY );
    SaveSetting( value, SettingsDef::DB_KEY_VR_PHONE_PRIORITY );

    value = GetDefaultSetting( SettingsDef::DB_KEY_VOICE_VOICECOMMAND );
    SaveSetting( value, SettingsDef::DB_KEY_VOICE_VOICECOMMAND );


    //value = GetDefaultSetting( SettingsDef::DB_KEY_VOICE_APPLE_SIRI );
    //SaveSetting( value, SettingsDef::DB_KEY_VOICE_APPLE_SIRI );


    GetDefaultKeyPad();


    //value = GetDefaultSetting( SettingsDef::DB_KEY_REAR_ON );
    //emit propertyResetCompleted(value, SettingsDef::DB_KEY_REAR_ON);

    //value = GetDefaultSetting(SettingsDef::DB_KEY_LREAR);
    //emit propertyResetCompleted(value, SettingsDef::DB_KEY_LREAR);

    //value = GetDefaultSetting(SettingsDef::DB_KEY_RREAR);
    //emit propertyResetCompleted(value, SettingsDef::DB_KEY_RREAR);


}

int ResetSettingThread::GetDefaultSetting( int key )
{
    int value = -1;

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

    //qDebug() << "[CPP][ResetSettingThread]GetDefaultSetting():key=" << key << ", value=" << value;

    return value;
}

int ResetSettingThread::GetDefaultSettingOfDispOnOff( int defaultDispOnOff )
{
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

void ResetSettingThread::SaveSetting( int value, int key )
{
    //LOG2 ( QString("R_Thread::SaveSetting: key:[%1], value[%2]").arg(key).arg(value), Logger::PRIORITY_HI );
    int result = SI_PM_OK;
    if( (m_nDBState == 0) && (m_pDBSession != NULL) )
    {
        // save setting to DB
        result = SIPMSet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ key ].keyString, (unsigned char *)&value, sizeof( int ) );

        if( result != SI_PM_OK )
            LOG2 ( QString("DB Write Failed!! key(%1), result(%2)").arg(key).arg(result), Logger::PRIORITY_CRITICAL );


        emit propertyResetCompleted(value, key);
    }
    else
    {
        LOG2 ( QString("DB Write Failed! DB_S NULL! key(%1)").arg(key), Logger::PRIORITY_CRITICAL );

        close();
        m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, SETTINGS_FILE_DB, SETTINGS_HM_DB, 0 , 0);
    }
}

void ResetSettingThread::GetDefaultKeyPad( )
{
    int keypadType = -1;

    if(m_nCurrentRegion <= (int)eCVInvalid || m_nCurrentRegion >= (int)eCVInvalidMax)
        LOG2(QString("INCORRECT Region."), Logger::PRIORITY_HI);
        //qDebug() << "ResetSettingThread::GetDefaultKeyPad INCORRECT currentRegion";

    keypadType = GetDefaultSetting( SettingsDef::DB_KEY_ENGLISH_KEYPAD );
    SaveSetting( keypadType, SettingsDef::DB_KEY_ENGLISH_KEYPAD );

    keypadType = GetDefaultSetting( SettingsDef::DB_KEY_KOREAN_KEYPAD );
    SaveSetting( keypadType, SettingsDef::DB_KEY_KOREAN_KEYPAD );

    keypadType = GetDefaultSetting( SettingsDef::DB_KEY_ARABIC_KEYPAD );
    SaveSetting( keypadType, SettingsDef::DB_KEY_ARABIC_KEYPAD );

    keypadType = GetDefaultSetting( SettingsDef::DB_KEY_CHINA_KEYPAD );
    SaveSetting( keypadType, SettingsDef::DB_KEY_CHINA_KEYPAD );

    keypadType = GetDefaultSetting( SettingsDef::DB_KEY_EUROPE_KEYPAD );
    SaveSetting( keypadType, SettingsDef::DB_KEY_EUROPE_KEYPAD );

    keypadType = GetDefaultSetting( SettingsDef::DB_KEY_RUSSIAN_KEYPAD );
    SaveSetting( keypadType, SettingsDef::DB_KEY_RUSSIAN_KEYPAD );

    switch ( m_nCurrentRegion )
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

    SaveSetting( keypadType, SettingsDef::DB_KEY_HYUNDAY_KEYPAD );
}

int ResetSettingThread::LoadSetting( int key )
{
    int value = -101; // seems it means empty data, however data has range [-10..10]
    int stateSize = 0;
    int result = SI_PM_OK;

    //get setting from Settings DB
    if( (m_nDBState == 0) && (m_pDBSession != NULL) )
    {
        result = SIPMGet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ key ].keyString, (unsigned char *)&value, &stateSize );

        if( result != SI_PM_OK )
            LOG2 ( QString("DB Read Failed!! key(%1), result(%2)").arg(key).arg(result), Logger::PRIORITY_CRITICAL );

        //if(key == SettingsDef::DB_KEY_CLOCK_TYPE)
        //    qDebug() << "Hanuk, DB_KEY_CLOCK_TYPE = " << value ;
    }
    else
    {
        LOG2 ( QString("DB R Failed! DB_S NULL! key(%1)").arg(key), Logger::PRIORITY_CRITICAL );

        close();
        m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, SETTINGS_FILE_DB, SETTINGS_HM_DB, 0 , 0);

        bool bDBSessionEnabled = m_pDBSession ? true : false;
        if( (m_nDBState != 0) || !bDBSessionEnabled )
        {
            LOG2 ( QString("DB O Failed!! State = %1, enabled = %2").arg(m_nDBState).arg(bDBSessionEnabled), Logger::PRIORITY_CRITICAL );
        }
    }

    if( ( result != SI_PM_OK ) || ( value == -101 ) )
    {
        switch(key)
        {
        case SettingsDef::DB_KEY_DATEFORMAT_TYPE:
            value = GetDefaultDateTimeFormat();
            SaveSetting( value, key );
            break;
            //        case SettingsDef::DB_KEY_DISTANCE_UNIT:
            //            value = GetDefaultDistanceUnit();
            //            SaveSetting( value, key );
            //            break;
        case SettingsDef::DB_KEY_HYUNDAY_KEYPAD:
            value = getDefaultHyundaiKeyboard();
            SaveSetting( value, key );
            break;
        case SettingsDef::DB_KEY_SUMMER_TIME:
            value = GetDefaultSummerTime();
            SaveSetting( value, key );
            break;
        case SettingsDef::DB_KEY_RESET_ALL_COMPLETE:
            LOG2 ( QString("Load Failed!! DB_KEY_RESET_ALL_COMPLETE"), Logger::PRIORITY_CRITICAL );
            break;
        default:
            value = SetDefaultSetting( key );

            //if(key == SettingsDef::DB_KEY_CLOCK_TYPE)
            //    qDebug() << "Hanuk, exception. DB_KEY_CLOCK_TYPE = " << value ;

            break;
        }
    }

    return value;
}

int ResetSettingThread::GetDefaultDateTimeFormat( )
{
    int dateTimeFormat = SettingsDef::SETTINGS_DATEFORMAT_YYYYMMDD_DASH;
    //qDebug() << "ResetSettingThread::GetDefaultDateTimeFormat currentRegion = " << m_nCurrentRegion;

    if(m_nCurrentRegion <= (int)eCVInvalid || m_nCurrentRegion >= (int)eCVInvalidMax)
        LOG2(QString("INCORRECT Region"), Logger::PRIORITY_HI);
        //qDebug() << "ResetSettingThread::GetDefaultDateTimeFormat INCORRECT currentRegion";

    switch ( m_nCurrentRegion )
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

int ResetSettingThread::getDefaultHyundaiKeyboard()
{
    int keypadType = -1;

    switch ( m_nCurrentRegion )
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

int ResetSettingThread::GetDefaultSummerTime( )
{
    int summerTime = 0;

    if(m_nCurrentRegion <= eCVInvalid || m_nCurrentRegion >= eCVInvalidMax)
        LOG2(QString("INCORRECT Region"), Logger::PRIORITY_HI);
        //qDebug()<<"ResetSettingThread::GetDefaultSummerTime(): INCORRECT currentRegion";

    switch ( m_nCurrentRegion )
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

int ResetSettingThread::GetDefaultLanguage()
{
    int language = -1;

    //qDebug() << "ResetSettingThread::GetDefaultLanguage currentRegion = " << m_nCurrentRegion;

    if(m_nCurrentRegion <= eCVInvalid || m_nCurrentRegion >= eCVInvalidMax)
        LOG2(QString("INCORRECT Region"), Logger::PRIORITY_HI);
        //qDebug() << "ResetSettingThread::GetDefaultLanguage INCORRECT currentRegion";

    switch ( m_nCurrentRegion )
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

int ResetSettingThread::SetDefaultSetting( int key )
{
    int value = -1;

    //qDebug() << "ResetSettingThread::SetDefaultSetting key=" << key;

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
            }//modify for NA/CA/CH Agreement Spec Modify(10 seconds, default on) 01/14
            else if(key == SettingsDef::DB_KEY_APPROVAL)
            {
                if(m_nCurrentRegion == eCVNorthAmerica || m_nCurrentRegion == eCVCanada
                        || m_nCurrentRegion == eCVChina )
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
            }//modify for NA/CA/CH Agreement Spec Modify(10 seconds, default on) 01/14
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
