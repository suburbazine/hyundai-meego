#include <QDebug>
#include <QApplication>
// #include <gconf/gconf-client.h>

#include <DHAVN_AppHomeScreen_Def_Private.h>
#include <DHAVN_AppHomeScreen_LocalizeTrigger.h>

LocalizeTrigger::LocalizeTrigger( LANGUAGE_T nLanguage )
               : m_pTranslator( new QTranslator )
                 //m_nLanguage( nLanguage )
{
    SetLanguage(nLanguage);
   //InitTranslator();
}

void LocalizeTrigger::SetLanguage( LANGUAGE_T nLanguage )
{
   m_nLanguage = nLanguage;
   InitTranslator();

   if (m_nLanguage == LANGUAGE_AR)
   {
       setArab(true);
   }
   else
   {
       setArab(false);
   }

   emit retrigger();
}

QString LocalizeTrigger::Translate( QString sStrId )
{
   return m_pTranslator->translate( "main", sStrId.toUtf8().data() );
}

void LocalizeTrigger::InitTranslator()
{
   m_pTranslator->load( GetTransRes() );
   ( (QApplication *) qApp)->installTranslator( m_pTranslator );
}

QString LocalizeTrigger::GetTransRes()
{
   switch( m_nLanguage )
   {
      case LANGUAGE_ZH_CMN: return TransRes_ZH_CMN;
      case LANGUAGE_ZH_YUE: return TransRes_ZH_YUE;
      case LANGUAGE_EN_GB: return TransRes_EN_GB;

      case LANGUAGE_EN_US:
      case LANGUAGE_EN: return TransRes_EN;

      case LANGUAGE_KO: return TransRes_KO;
      case LANGUAGE_PT: return TransRes_PT;
      case LANGUAGE_FR: return TransRes_FR;
      case LANGUAGE_IT: return TransRes_IT;
      case LANGUAGE_DE: return TransRes_DE;
      case LANGUAGE_ES: return TransRes_ES;
      case LANGUAGE_RU: return TransRes_RU;
      case LANGUAGE_NL: return TransRes_NL;
      case LANGUAGE_SV: return TransRes_SV;
      case LANGUAGE_PL: return TransRes_PL;
      case LANGUAGE_TR: return TransRes_TR;
      case LANGUAGE_CS: return TransRes_CS;
      case LANGUAGE_DA: return TransRes_DA;
      case LANGUAGE_SK: return TransRes_SK;
      case LANGUAGE_AR: return TransRes_AR;
       case LANGUAGE_FR_NA: return TransRes_FR_NA;
       case LANGUAGE_ES_NA: return TransRes_ES_NA;
      default: return TransRes_EN;
   }
}
