#ifndef DHAVN_APPFILEMANAGER_HASHLIST_H
#define DHAVN_APPFILEMANAGER_HASHLIST_H

#include <QHash>
#include <QList>
#include <QMutex>

template < typename ItemType >
class DefaultSearchEngine
{
public:
   static int SearchInsertIndex( const QList<ItemType*> & list, const ItemType & item )
   {
      int index = 0;
      int size = list.size();

      for ( index = 0; index < size; index++ )
      {
         if ( list[index]->compare( &item ) > 0 )
         {
            break;
         }
      }

      return index;
   }
};

template < typename ItemType >
class CustomSearchEngine
{
public:
   static int SearchInsertIndex( const QList<ItemType*> & list, const ItemType & item )
   {
      int index = 0;
      int size = list.size();

      for ( index = 0; index < size; index++ )
      {
         if ( list[index]->compare( &item ) < 0 )
         {
            break;
         }
      }

      return index;
   }
};

template < typename ItemType,
           typename KeyType,
           template < typename T > class SearchEngine = CustomSearchEngine >
class HashList
{
   typedef QHash<KeyType, ItemType*> HashItems;
   typedef QList<ItemType*> ListItems;
public:
   HashList(){};

   int count() const
   {
      return m_ItemsList.count();
   }

   int GetInsertIndex( const ItemType & item ) const//Changed by Alexey Edelev 2012.10.18 CR14766
   {
      return SearchEngine<ItemType>::SearchInsertIndex( m_ItemsList, item );
   }

   void insert( const ItemType & item, int index )
   {
      ItemType * pNewItem = new ItemType( item );

      m_ItemsList.insert( index, pNewItem );
      m_ItemsHash.insert( pNewItem->key(), pNewItem );
   }

   // { added by sungha.choi 2013.07.05
   void append( const ItemType & item )
   {
      ItemType * pNewItem = new ItemType( item );

      m_ItemsList.append( pNewItem );
      m_ItemsHash.insert( pNewItem->key(), pNewItem );
   }
   // } added by sungha.choi 2013.07.05

   ItemType * GetItemByIndex( int index ) const
   {
      if ( index < 0 || index >= m_ItemsList.count() )
      {
         return NULL;
      }

      return m_ItemsList.at(index);
   }

   ItemType * GetItemByKey( KeyType key ) const
   {
      typename HashItems::const_iterator iterator = m_ItemsHash.find( key );

      while ( iterator != m_ItemsHash.end() && iterator.key() == key )
      {
         return iterator.value();
      }

      return NULL;
   }

   bool UpdateItemByIndex( int index, const ItemType & item )
   {
      bool result = false;

      if ( index > 0 && index <= m_ItemsList.count() )
      {
         *(m_ItemsList.at( index )) = item;
         result = true;
      }

      return result;
   }

   bool UpdateItemByKey( const KeyType & key, const ItemType & item )
   {
      bool result = false;
      ItemType * pItem = NULL;

      pItem = GetItemByKey( key );
      if ( pItem )
      {
         *pItem = item;
         result = true;
      }

      return result;
   }

   void RemoveAt( int index )
   {
      KeyType key = m_ItemsList.at( index )->key();

      typename HashItems::iterator iterator = m_ItemsHash.find( key );

      while ( iterator != m_ItemsHash.end() && iterator.key() == key )
      {
         iterator = m_ItemsHash.erase( iterator );
      }

      delete m_ItemsList.at( index );
      m_ItemsList.removeAt( index );
   }

   void clear()
   {
      m_ItemsHash.clear();

      while ( !m_ItemsList.isEmpty() )
      {
         delete m_ItemsList.at( 0 );
         m_ItemsList.removeAt( 0 );
      }
   }

private:
   HashItems m_ItemsHash;
   ListItems m_ItemsList;

public:
   QMutex    m_Mutex;
};

#endif // DHAVN_APPFILEMANAGER_HASHLIST_H
