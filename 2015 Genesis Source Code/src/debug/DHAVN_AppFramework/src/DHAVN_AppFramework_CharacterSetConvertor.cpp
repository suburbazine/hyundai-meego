#include "DHAVN_AppFramework_CharacterSetConvertor_Def.h"
#include <QTextStream>
#include <QTextCodec>
#include "DHAVN_AppFramework_AppEngineBase.h"

CharacterSetConvertor::CharacterSetConvertor()
: m_Region( CHARSET_REGION_NONE ), m_CodePage( CHARSET_CODEPAGE_NONE )
{
   SetRegion();
}

CharacterSetConvertor::CharacterSetConvertor( const QString &aInputString )
: m_InputString( aInputString ), m_Region( CHARSET_REGION_NONE ), m_CodePage( CHARSET_CODEPAGE_NONE )
{
   SetRegion();
}

const QString &
CharacterSetConvertor::Convert( const QString &aInputString, bool &aStatus )
{
   m_InputString = aInputString;

   return Convert( aStatus );
}

const QString &
CharacterSetConvertor::Convert( bool &aStatus )
{
   aStatus = false;

   if ( !m_InputString.isEmpty() )
   {
      if ( IsUTF8() )
      {
         m_CodePage = CHARSET_CODEPAGE_UTF8;

         return m_InputString;
      }

      // TODO: To chek original text encoding
      QTextCodec *pCodec = QTextCodec::codecForName( "EUC-KR" );

      if ( pCodec )
      {
         QTextStream in( m_InputString.toAscii() );

         in.setAutoDetectUnicode( false );
         in.setCodec( pCodec );

         m_OutputString = in.readAll();

         aStatus = true;
      }
   }

   return m_OutputString;
}

void
CharacterSetConvertor::SetRegion()
{
   switch ( AppEngineBase::GetLanguage() )
   {
   // Europe & Baltic region
   case LANGUAGE_EN:
   case LANGUAGE_EN_US:
   case LANGUAGE_ES:
   case LANGUAGE_FR:
   case LANGUAGE_IT:
   case LANGUAGE_EN_GB:
   case LANGUAGE_DE:
   case LANGUAGE_NL:
   case LANGUAGE_SV:
   case LANGUAGE_CS:
   case LANGUAGE_SK:
   case LANGUAGE_DA:
      m_Region = CHARSET_REGION_BL;
      break;
   // China
   case LANGUAGE_ZH_CN:
   case LANGUAGE_ZH_YUE:
      m_Region = CHARSET_REGION_CN;
      break;
   // Russia
   case LANGUAGE_RU:
      m_Region = CHARSET_REGION_RU;
      break;
   // Polish
   case LANGUAGE_PL:
      m_Region = CHARSET_REGION_PL;
      break;
   case LANGUAGE_TR:
      m_Region = CHARSET_REGION_TR;
      break;
   // Arabic
   case LANGUAGE_AR:
      m_Region = CHARSET_REGION_AR;
      break;
   // Korean
   case LANGUAGE_KO:
      m_Region = CHARSET_REGION_KR;
      break;
   default:
      break;
   }
}

bool
CharacterSetConvertor::IsUTF8()
{
   int str_bytes = m_InputString.length();
   bool isUtf8 = true;
   int index = 0;

   while ( index < str_bytes && isUtf8 )
   {
      char achar = m_InputString[ index ].toAscii();
      int supplemental_bytes = 0;

      if ( ( achar & 0x80 ) == 0 )
      {
         // 0xxxxxxx
         ++index;
      }
      else if ( ( achar & 0xF8 ) == 0xF0 )
      {
         // 11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
         ++index;
         supplemental_bytes = 3;
      }
      else if ( ( achar & 0xE0 ) == 0xE0 )
      {
         // 1110zzzz 10yyyyyy 10xxxxxx
         ++index;
         supplemental_bytes = 2;
      }
      else if ( ( achar & 0xE0 ) == 0xC0 )
      {
         // 10yyyyyy 10xxxxxx
         ++index;
         supplemental_bytes = 1;
      }
      else
      {
         isUtf8 = false;
      }

      while ( isUtf8 && supplemental_bytes-- )
      {
         if( index >= str_bytes )
         {
             isUtf8 = false;
         }
         else if ( ( m_InputString[ index++ ].toAscii() & 0xC0 ) != 0x80 )
         {
             // 10uuzzzz
             isUtf8 = false;
         }
      }
   }

   return isUtf8;
}
