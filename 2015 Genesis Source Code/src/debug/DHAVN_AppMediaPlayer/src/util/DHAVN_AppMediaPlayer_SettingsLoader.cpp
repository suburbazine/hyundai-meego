#include "util/DHAVN_AppMediaPlayer_SettingsLoader.h"
#include <QDebug>
#include <DHAVN_AppSettings_Def.h>
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"

#define MAX_SIZE 1024

SettingsLoader* SettingsLoader::m_instance = NULL;

SettingsLoader::SettingsLoader() : m_pDBPath( SETTINGS_FILE_DB )
  , m_pDBSession( NULL )
{
   m_nDBState = SIPMOpen( &m_pDBSession, SI_PM_SQLITE, m_pDBPath, SETTINGS_HM_DB, 0 , 0);
}

SettingsLoader::~SettingsLoader()
{
   Close();
}


bool SettingsLoader::Close()
{
   int result = SI_PM_OK;

   if( NULL != m_pDBSession )
   {
      result = SIPMClose( m_pDBSession );
   }
   return result == SI_PM_OK ? true : false;
}

int SettingsLoader::LoadSetting( int key )
{
   int value = -1;
   int stateSize = 0;
   int result = SI_PM_OK;

   if( NULL != m_pDBSession )
   {
      result = SIPMGet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ key ].keyString, (unsigned char *)&value, &stateSize );
   }
   if ( result != SI_PM_OK )
   {
      return -1;
   }

   return value;
}

QString SettingsLoader::LoadSettingString( int key )
{
   wchar_t value [MAX_SIZE] = { 0 };
   int stateSize = 0;
   int result = SI_PM_OK;

   if( NULL != m_pDBSession )
   {
      result = SIPMGet( m_pDBSession, SETTINGS_DB_VARIABLES_KEYS[ key ].keyString, (unsigned char*)value, &stateSize );
   }
   if ( result != SI_PM_OK )
   {
      return QString("");
   }
   MP_LOG << "LoadClockSetting::LoadSettingString path = " << QString::fromStdWString(value)  << LOG_ENDL;
   return QString::fromStdWString(value);
}

