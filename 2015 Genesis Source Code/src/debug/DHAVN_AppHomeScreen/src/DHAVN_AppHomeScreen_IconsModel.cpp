#include <DHAVN_AppHomeScreen_IconsModel.h>

extern QString ViewID;

IconsModel::IconsModel( IconItem *pPrototype, QObject *pParent )
              :QAbstractListModel( pParent ),
               m_pPrototype( pPrototype ),
               m_bIsMainMenu( true ),
               m_nFocusIndex( 0 )
{
   setRoleNames( m_pPrototype->RoleNames() );
}

IconsModel::~IconsModel()
{
   disconnect();
   delete m_pPrototype;
   m_pPrototype = NULL;
   Clear();
}


int IconsModel::rowCount( const QModelIndex & ) const
{
   return m_pList.size();
}

QVariant IconsModel::data( const QModelIndex &index, int nRole ) const
{
   if( index.row() < 0 || index.row() >= m_pList.size() )
   {
      return QVariant();
   }

   return m_pList.at( index.row() )->Data( nRole );
}

void IconsModel::AppendItems( const QList<IconItem *> &items )
{
   beginInsertRows( QModelIndex(), rowCount(), rowCount() + items.size() - 1 );

   foreach( IconItem *pItem, items )
   {
      connect( pItem, SIGNAL( dataChanged() ), SLOT( handleItemChange() ) );
      m_pList.append( pItem );
   }

   endInsertRows();
}

void IconsModel::handleItemChange()
{
   IconItem* pItem = static_cast<IconItem*>( sender() );
   QModelIndex index = IndexFromItem( pItem );

   if( index.isValid() ) emit dataChanged( index, index );
}

IconItem* IconsModel::Find( const HSDefP::APP_ID_T &nAppId )
{
   foreach( IconItem* pItem, m_pList )
   {
      if( pItem->GetAppId() == nAppId ) return pItem;
   }

   return NULL;
}

QModelIndex IconsModel::IndexFromItem( const IconItem *pItem ) const
{
   Q_ASSERT( pItem );
   for( int nRow = 0; nRow < m_pList.size(); ++nRow)
   {
      if( m_pList.at( nRow ) == pItem ) return index( nRow );
   }

   return QModelIndex();
}

void IconsModel::Clear()
{
   //qDeleteAll( m_pList );
    removeItems( 0, m_pList.size() );
   m_pList.clear();
}

bool IconsModel::removeItems( int nRow, int nCount, const QModelIndex & )
{
   if( nRow < 0 || ( nRow + nCount ) > m_pList.size() ) return false;

   beginRemoveRows( QModelIndex(), nRow, nRow + nCount - 1);

   for( int i = 0; i < nCount; i++ )
   {
      delete m_pList.takeAt( nRow );
   }

   endRemoveRows();
   return true;
}

bool IconsModel::SetPosition( const int &nItemIndex, const int &nXPos, const int &nYPos )
{
   if( ( nItemIndex < 0 ) || ( nItemIndex > m_pList.size() - 1 ) ) return false;

   m_pList[ nItemIndex ]->SetPosition( nXPos, nYPos );

   return true;
}

void IconsModel::RemoveItem( const HSDefP::APP_ID_T &nAppId )
{
   foreach( IconItem* pItem, m_pList )
   {
      if( pItem->GetAppId() == nAppId )
      {
         removeRows( IndexFromItem( pItem ).row(), 1, IndexFromItem( pItem ) );
         return;
      }
   }
}

void IconsModel::RemoveItem( const HSDefP::VIEW_ID_T &nViewId )
{
   foreach( IconItem* pItem, m_pList )
   {
      if( pItem->GetViewId() == nViewId )
      {
         removeRows( IndexFromItem( pItem ).row(), 1, IndexFromItem( pItem ) );
         return;
      }
   }
}

void IconsModel::SetFocusIndex( int nIndex )
{
   m_nFocusIndex = nIndex;

   emit updateFocusIndex();
   emit UpdateFocusedText();
   emit UpdateFocusedViewId();
   emit UpdateFocusedAppId();
}

int IconsModel::GetIndexFromName(QString str)
{

    for (int i = 0; i < m_pList.count(); i++)
    {
        if ( str == m_pList.at(i)->GetText() )
            return i;
    }

    return -1;
}

int IconsModel::GetIndexFromAppID(HSDefP::APP_ID_T iconID)
{
    IconItem* pIcon = NULL;

    for (int i = 0; i < m_pList.size(); i++ )
    {
        pIcon = m_pList.at(i);

        if (pIcon->GetAppId() == iconID)
        {
            return i;
        }
    }

    return -1;
}

int IconsModel::GetIndexFromViewID(HSDefP::VIEW_ID_T iconID)
{
    IconItem* pIcon = NULL;

    for (int i = 0; i < m_pList.size(); i++ )
    {
        pIcon = m_pList.at(i);

        if (pIcon->GetViewId() == iconID)
        {
            return i;
        }
    }

    return -1;
}

void IconsModel::SetDataList( const QList<HSDefP::APP_DATA_T> &lData )
{
   m_lData = lData;

   if( m_nFocusIndex >= m_pList.size() - 1 ) SetFocusIndex( m_pList.size() - 1 );
}

void IconsModel::SetMainMenuFlag( bool isMainMenu )
{
   m_bIsMainMenu = isMainMenu;
}

bool IconsModel::IsMainMenu()
{
   return m_bIsMainMenu;
}

void IconsModel::SetFocusIconIndex( int iconIdx )
{
   int iFocusIdx;

   if ( iconIdx >= m_pList.size() )
   {
      iFocusIdx = 0;
   }
   else
   {
      iFocusIdx = iconIdx;
   }

   SetFocusIndex( iFocusIdx );
}

void IconsModel::SetFocusIconIndexByViewID( HSDefP::VIEW_ID_T iconID )
{
    IconItem* pIcon = NULL;

    for (int i = 0; i < m_pList.size(); i++ )
    {
        pIcon = m_pList.at(i);

        if (pIcon->GetViewId() == iconID)
        {
            SetFocusIndex(i);
        }
    }
}

void IconsModel::SetFocusIconIndexByAppID( HSDefP::APP_ID_T iconID )
{
    IconItem* pIcon = NULL;

    for (int i = 0; i < m_pList.size(); i++ )
    {
        pIcon = m_pList.at(i);

        if (pIcon->GetAppId() == iconID)
        {
            SetFocusIndex(i);
        }
    }
}




void IconsModel::UpdateDataField( HSDefP::APP_DATA_T nID, QVariant )
{
   if( m_lData.contains( nID ) &&
      ( nID > HSDefP::APP_DATA_AVAILABLE_MINIMUM ) &&
      ( nID < HSDefP::APP_DATA_AVAILABLE_MAXIMUM ) )
   {
      removeItems( 0, m_pList.size() );
      emit ReloadModel();
   }
}
