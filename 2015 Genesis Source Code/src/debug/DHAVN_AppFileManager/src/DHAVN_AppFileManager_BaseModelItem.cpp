#include "hdr/DHAVN_AppFileManager_BaseModelItem.h"

#include <QDebug>

// modified by Dmitry 23.08.13
BaseModelItem::BaseModelItem()
    : itemChecked(false),
      folder(false),
      itemPlay(false),
      itemSupported(true)
{
}

QBool BaseModelItem::isFolder() const
{
    return this->folder;
}

void BaseModelItem::setFolder(const QBool& value)
{
    this->folder = value;
}

QVariant BaseModelItem::getItemId() const
{
    return this->itemId;
}

void BaseModelItem::setItemId(const QVariant& value)
{
    this->itemId = value;
}

QString BaseModelItem::getItemName() const
{
    return this->itemName;
}

void BaseModelItem::setItemName(const QString& value)
{
    this->itemName = value;
}

QString BaseModelItem::getItemPath() const
{
    return this->itemPath;
}

void BaseModelItem::setItemPath(const QString& value)
{
    this->itemPath = value;
}

QBool BaseModelItem::getItemChecked() const
{
    return this->itemChecked;
}

void BaseModelItem::setItemChecked(const QBool& value)
{
    this->itemChecked = value;
}

QBool BaseModelItem::getItemPlay() const
{
    return this->itemPlay;
}

void BaseModelItem::setItemPlay(const QBool& value)
{
    this->itemPlay = value;
}

// added by Dmitry 23.08.13
QBool BaseModelItem::getItemSupported() const
{
    return this->itemSupported;
}

void BaseModelItem::setItemSupported(const QBool& value)
{
    this->itemSupported = value;
}

QString BaseModelItem::key() const
{
   return itemName;
}

int BaseModelItem::compare( const BaseModelItem * pItem ) const
{
   if ( folder )
   {
      if ( pItem->folder )
      {
         // modified by sungha.choi 2013.07.05 : File manager should use the order from TA because of efficiency. (do not change the order from TA)
         return 1;
         //return -itemName.compare( pItem->itemName, Qt::CaseInsensitive );    // deleted by sungha.choi
      }
      else
      {
         return 1;
      }
   }
   else
   {
      if ( pItem->folder )
      {
         return -1;
      }
      else
      {
         return -itemName.compare( pItem->itemName, Qt::CaseInsensitive );
      }
   }
}
