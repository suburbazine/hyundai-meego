#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "util/video/DHAVN_AppMediaPlayer_VideoSettings.h"
#include <DHAVN_SharedDataProvider.h> // modified by ravikanth 27-12-12

#define VP_SETTINGS_FILE_DB "/app/data/AppMedia/VideoPlayerSettings.db"
#define VP_SETTINGS_MP_DB "video_player_app"
// { modified by ravikanth 27-12-12

AppMediaPlayer_VideoSettings* AppMediaPlayer_VideoSettings::m_pInstance = 0;
int AppMediaPlayer_VideoSettings::m_count = 0;

AppMediaPlayer_VideoSettings::AppMediaPlayer_VideoSettings( QObject* )
	:m_nDBState(0) // modified by eugene 2012.08.22 for static analysis defect #6953
{
    m_pDBSession = NULL;
}

int AppMediaPlayer_VideoSettings::LoadSetting( int key )
{
    MP_LOG << LOG_ENDL;

    if(!OpenScreenSettingsDb())
    {
        MP_LOG<<"OpenScreenSettingsDb failture" << LOG_ENDL;
        return SI_PM_UE;
    }

    int value = -100; //modified by shkim for ITS 189354
    int stateSize = 0;
    int result = SI_PM_OK;

    //get setting from Settings DB
    if( NULL != m_pDBSession )
    {
        result = SIPMGet( m_pDBSession, VIDEO_SETTINGS_DB_VARIABLES_KEYS[ key ].keyString, (unsigned char *)&value, &stateSize );
    }

    if (( result != SI_PM_OK) ||(value == -100) )
    {
        value = SetDefaultSetting( key );
    }
    CloseScreenSettingsDb();
    return value;
}


void AppMediaPlayer_VideoSettings::SaveSetting( int value, int key )
{
    Q_UNUSED(key);
    MP_LOG << LOG_ENDL;
    if(!OpenScreenSettingsDb())
    {
        MP_LOG<<"OpenScreenSettingsDb FAIL " << LOG_ENDL;
        return;
    }

    if( m_pDBSession != NULL)
    {
        //save setting to BD
        SIPMSet( m_pDBSession, VIDEO_SETTINGS_DB_VARIABLES_KEYS[ key ].keyString, (unsigned char *)&value, sizeof( int ) );
    }
    CloseScreenSettingsDb();
}

bool AppMediaPlayer_VideoSettings::OpenScreenSettingsDb()
{
    m_nDBState = SI_PM_OK;
    if( m_pDBSession == NULL )
    {
        m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, VP_SETTINGS_FILE_DB,VP_SETTINGS_MP_DB, 0 , 0);
    }
    return ( m_nDBState == SI_PM_OK );
}

void AppMediaPlayer_VideoSettings::CloseScreenSettingsDb()
{
    if( NULL != m_pDBSession )
    {
        SIPMClose( m_pDBSession );
    }
    m_pDBSession = NULL;
}

int AppMediaPlayer_VideoSettings::SetDefaultSetting( int key )
{
    int value = -1;
    MP_LOG << LOG_ENDL;

    for (int i=0; i<VideoSettingsDef::DB_KEY_MAX ;i++)
    {
        if (VIDEO_SETTINGS_VARIABLES_DEFAULT_INT[i][0] == key )
        {
            value = VIDEO_SETTINGS_VARIABLES_DEFAULT_INT[i][1];
            SaveSetting( value, key );
            break;
        }
    }
    return value;
}


AppMediaPlayer_VideoSettings*
AppMediaPlayer_VideoSettings::GetInstance()
{
    if ( m_pInstance == 0 )
    {
        m_pInstance = new AppMediaPlayer_VideoSettings();
    }
    ++m_count;
    return m_pInstance;
}

void
AppMediaPlayer_VideoSettings::FreeInstance()
{
    --m_count;
    if (m_count == 0)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

int AppMediaPlayer_VideoSettings::LoadSetting( int key, int type )
{
    // key refers to jukebox values DB_KEY_VIDEO_JUKEBOX_BRIGHTNESS etc.
    switch( type )
    {
    case APP_STATE_NONE: // added by eugene.seo 2013.04.23
        return LoadSetting(key + 5);
        break;
    case APP_STATE_AUX:
        return LoadSetting(key + 5);
        break;
    case APP_STATE_VCD:
        return LoadSetting(key + 4);
        break;
    case APP_STATE_USB1:
        return LoadSetting(key + 1);
        break;
    case APP_STATE_USB2:
        return LoadSetting(key + 2);
        break;
    case APP_STATE_JUKEBOX:
        return LoadSetting(key);
        break;
    case APP_STATE_DVD:
        return LoadSetting(key + 3);
        break;
    default:
        break;
    }
    return -1; // added by sungha.choi 2013.07.15 for static test, the default value of LoadSetting(int) is -1
}

void AppMediaPlayer_VideoSettings::SaveSetting( int value, int key , int type )
{
    // key refers to jukebox values DB_KEY_VIDEO_JUKEBOX_BRIGHTNESS etc.
    switch( type )
    {
    case APP_STATE_NONE: // added by eugene.seo 2013.04.23
        SaveSetting(value, key + 5);
        break;
    case APP_STATE_AUX:
        SaveSetting(value, key + 5);
        break;
    case APP_STATE_VCD:
        SaveSetting(value, key + 4);
        break;
    case APP_STATE_USB1:
        SaveSetting(value, key + 1);
        break;
    case APP_STATE_USB2:
        SaveSetting(value, key + 2);
        break;
    case APP_STATE_JUKEBOX:
        SaveSetting(value, key);
        break;
    case APP_STATE_DVD:
        SaveSetting(value, key + 3);
        break;
    default:
        break;
    }
}

// } modified by ravikanth 27-12-12
