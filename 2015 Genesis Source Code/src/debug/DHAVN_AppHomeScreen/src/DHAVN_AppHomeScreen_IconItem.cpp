#include <DHAVN_AppHomeScreen_IconItem.h>
#include <DHAVN_AppHomeScreen_AppData.h>

IconItem::IconItem( const HSDefP::VIEW_ID_T &nViewId, const HSDefP::APP_ID_T &nAppId,
                    const QString &sText, const QString &sImage, const bool bEnabled, const int iDisableCount, const bool bEnableInDriving,
                    const QList< HSDefP::APP_DATA_T> &lDataList )
         : m_nViewId( nViewId ),
           m_nAppId( nAppId ),
           m_sText( sText ),
           m_sImage( sImage ),
           m_lDataList( lDataList ),
           m_nXPos( 0 ),
           m_nYPos( 0 ),
           m_bEnabled( bEnabled ),
           m_nDisableCount ( iDisableCount ),
           m_bEnableInDriving( bEnableInDriving )
{
}

IconItem::IconItem( )
         : m_nViewId( HSDefP::VIEW_ID_MAIN ),
           m_nAppId( HSDefP::APP_ID_INVALID ),
           m_sText( "" ),
           m_sImage( "" ),
           m_lDataList( QList<HSDefP::APP_DATA_T>() ),
           m_nXPos( 0 ),
           m_nYPos( 0 ),
           m_bEnabled( false ),
           m_nDisableCount ( 0 ),
           m_bEnableInDriving( false )
{
}

IconItem::~IconItem()
{
   disconnect();
}

QHash<int, QByteArray> IconItem::RoleNames() const
{
   QHash<int, QByteArray> names;
   names[ nViewId ] = "nViewId";
   names[ nAppId ] = "nAppId";
   names[ sText ] = "sText";
   names[ sImage ] = "sImage";
   names[ nXPos ] = "nXPos";
   names[ nYPos ] = "nYPos";
   names[ bEnabled ] = "bEnabled";
   names[ bEnableInDriving ] = "bEnableInDriving";

   return names;
}

QVariant IconItem::Data( int nRole ) const
{
   switch( nRole )
   {
      case nViewId: return m_nViewId;
      case nAppId: return m_nAppId;
      case sText: return m_sText;
      case sImage: return m_sImage;
      case nXPos: return m_nXPos;
      case nYPos: return m_nYPos;
      case bEnabled: return m_bEnabled;
      case bEnableInDriving: return m_bEnableInDriving;
      default: return QVariant();
   }
}

void IconItem::SetPosition( const int &nXPos, const int &nYPos )
{
   m_nXPos = nXPos;
   m_nYPos = nYPos;

   emit dataChanged();
}

void IconItem::UpdateDataField( HSDefP::APP_DATA_T nID, QVariant data )
{
   if( !m_lDataList.contains( nID ) ) return;


   if ( nID > HSDefP::APP_DATA_ENABLED_MINIMUM && nID < HSDefP::APP_DATA_ENABLED_MAXIMUM )
   {
       /*
       if (data.toBool())
       {
           if (m_nDisableCount == 1 )
           {
               m_nDisableCount = 0;
               m_bEnabled = true;
           }
           else if ( m_nDisableCount > 1 )
           {
               m_nDisableCount -= 1;
               m_bEnabled = false;
           }
           else if ( m_nDisableCount <= 0 )
           {
               m_nDisableCount = 0;
               m_bEnabled = true;
           }
       }

       else
       {
           m_nDisableCount += 1;
           m_bEnabled = false;
       }
       */
       m_bEnabled = data.toBool();
   }


   if( nID == HSDefP::APP_DATA_ENABLED_PARKED )
   {
      m_bEnableInDriving = data.toBool();
   }

   if( ( nID > HSDefP::APP_DATA_STRING_MINIMUM ) &&
       ( nID < HSDefP::APP_DATA_STRING_MAXIMUM ) )
   {
      m_sText = data.toString();
   }

   emit dataChanged();
}
