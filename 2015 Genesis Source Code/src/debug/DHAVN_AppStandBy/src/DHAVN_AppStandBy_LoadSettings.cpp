#include "DHAVN_AppStandBy_LoadSettings.h"
#include <stdio.h>
#include <QDeclarativeComponent>
#include <DHAVN_AppSettings_Def.h>
#include <QDebug>

#define SLEEP_TIME 200000
#define MAX_SIZE 1024
#define MAX_RETRY 3
#define SETTINGS_FILE_DB "/app/data/app_settings/SettingsScreenSettings.db" //  "/home/meego/.config/SettingsScreenSettings.db"

LoadStandBySetting::LoadStandBySetting(QObject *) : m_pDBPath( SETTINGS_FILE_DB ), m_pDBSession( NULL )
{
    int retryCount = 0;
    bool bDBSessionEnabled = false;
    while(true)
    {
        m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, m_pDBPath, SETTINGS_HM_DB, 0 , 0);
        bDBSessionEnabled = m_pDBSession ? true : false;
        retryCount++;

        if( (( m_nDBState == 0 ) && bDBSessionEnabled ) || (retryCount > 5) )
        {
            if(( m_nDBState == 0 ) && bDBSessionEnabled )
            {
                HS_LOG("DB Open Session Success!! m_nDBState = " << m_nDBState << ", bDBSessionEnabled = " << bDBSessionEnabled);
            }
            else
            {
                HS_LOG("DB Open Session Failed!! m_nDBState = " << m_nDBState << ", bDBSessionEnabled = " << bDBSessionEnabled << ", retryCount = " << retryCount);
            }

            break;
        }
        else
        {
            CR_LOG("DB Open Session Failed!! m_nDBState = " << m_nDBState << ", bDBSessionEnabled = " << bDBSessionEnabled << ", retryCount = " << retryCount );

            Close();
            usleep(SLEEP_TIME);
        }
    }
}

LoadStandBySetting::~LoadStandBySetting()
{
   Close();
}

bool LoadStandBySetting::Close()
{
   int result = SI_PM_OK;

   if( NULL != m_pDBSession )
   {
      result = SIPMClose( m_pDBSession );
   }
   return result == SI_PM_OK ? true : false;
}

int LoadStandBySetting::LoadSetting( int key )
{
   int value = -1;
   int stateSize = 0;
   int result = SI_PM_OK;
   int cnt = 0;

   if( NULL != m_pDBSession )
   {
      while( cnt++ < MAX_RETRY )
      {
          result = SIPMGet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ key ].keyString, (unsigned char *)&value, &stateSize );
          if ( result == SI_PM_OK )
              break;
          usleep(100000);
      }
   }
   if ( result != SI_PM_OK )
      return -1;
   return value;
}

QString LoadStandBySetting::LoadSettingString( int key )
{
   wchar_t value [MAX_SIZE] = { 0 };
   int stateSize = 0;
   int result = SI_PM_OK;
   int cnt = 0;

   if( NULL != m_pDBSession )
   {
      while( cnt++ < MAX_RETRY )
      {
          result = SIPMGet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ key ].keyString, (unsigned char*)value, &stateSize );
          if ( result == SI_PM_OK )
              break;
          usleep(100000);
      }
   }
   if ( result != SI_PM_OK ) {
       HS_LOG( "LoadSettingString() - DB Read Fail!!!" );
      return "";
  }
   if(key == SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE )
       LW_LOG( "LoadSettings - path image : " << QString::fromStdWString(value) );
   return QString::fromStdWString(value);
}
