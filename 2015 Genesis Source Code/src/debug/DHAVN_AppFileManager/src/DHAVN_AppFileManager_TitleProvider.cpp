#include <DHAVN_AppFileManager_TitleProvider.h>
#include <QDebug>
#include <QFontMetrics>

TitleProvider::TitleProvider()
   : m_pTimer( new QTimer( this ) )
   , m_CurrentType( TITLE_TYPE_CURRENT_PATH )
   , m_CurrentPathType( CURRENT_PATH_FOLDER )
   , m_KeyPadTitle( "STR_MEDIA_MNG_ENTER_FOLDERNAME" )
   , m_CurrentPath( "" )
   , m_CurrentPlayedFile( "" )
   , m_TitleWidth(0) //added by aettie 2013.03.28 for Static defect 13127
{
   m_pTimer->setInterval( 5000 );
   m_pTimer->start();
   connect( m_pTimer, SIGNAL( timeout() ), this, SLOT( update() ) );
}

TitleProvider::~TitleProvider()
{
}

QString TitleProvider::getTitleText()
{
   QString result = "";

   switch ( m_CurrentType )
   {
      case TITLE_TYPE_CURRENT_PATH:
      {
         switch ( m_CurrentPathType )
         {
            case CURRENT_PATH_FOLDER:
            {
               result = m_CurrentPath;
            }
            break;

            case CURRENT_PATH_FILE:
            {
               if ( m_CurrentPlayedFile.isEmpty() )
               {
                  result = m_CurrentPath;
               }
               else
               {
                  result = m_CurrentPlayedFile;
               }
            }
            break;
         }
      }
      break;

      case TITLE_TYPE_KEYPAD:
      {
         result = m_KeyPadTitle;
      }
      break;
   }

   return result;
}

void TitleProvider::setTitleType( TITLE_TYPES type )
{
   m_CurrentType = type;
   emit titleTextChanged();

   switch ( m_CurrentType )
   {
      case TITLE_TYPE_CURRENT_PATH:
      {
         m_pTimer->start();
      }
      break;

      case TITLE_TYPE_KEYPAD:
      {
         m_pTimer->stop();
      }
      break;
   }
}

void TitleProvider::setCurrentPath( const QString & value )
{
   m_CurrentPath = value;
   emit titleTextChanged();
}

void TitleProvider::setCurrentPlayedFile( const QString & value )
{
   m_CurrentPlayedFile = value;
   emit titleTextChanged();
}

int TitleProvider::getTitleWidth()
{
   return m_TitleWidth;
}

void TitleProvider::setTitltWidth( int value )
{
   m_TitleWidth = value;
}

QFont TitleProvider::getFont()
{
   return m_Font;
}

void TitleProvider::setFont( const QFont & font )
{
   m_Font = font;
}

void TitleProvider::update()
{
   switch ( m_CurrentPathType )
   {
      case CURRENT_PATH_FOLDER:
      {     
         // } modified by yongkyun for ISV#70105
         //m_CurrentPathType = CURRENT_PATH_FILE;
         m_CurrentPathType = CURRENT_PATH_FOLDER;
         // } modified by yongkyun 
      }
      break;

      case CURRENT_PATH_FILE:
      {
         m_CurrentPathType = CURRENT_PATH_FOLDER;
      }
      break;
   }

   emit titleTextChanged();

   m_pTimer->start();
}
