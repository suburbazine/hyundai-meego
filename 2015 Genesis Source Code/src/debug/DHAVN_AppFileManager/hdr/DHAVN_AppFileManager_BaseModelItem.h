#ifndef DHAVN_APPFILEMANAGER_BASEMODELITEM_H
#define DHAVN_APPFILEMANAGER_BASEMODELITEM_H

#include <QVariant>
#include <QString>

class BaseModelItem
{
public:
   BaseModelItem();

   QBool isFolder() const;
   void setFolder(const QBool& value);

   QVariant getItemId() const;
   void setItemId(const QVariant& value);

   QString getItemName() const;
   void setItemName(const QString& value);

   QString getItemPath() const;
   void setItemPath(const QString& value);

   QBool getItemChecked() const;
   void setItemChecked(const QBool& value);

   QBool getItemPlay() const;
   void setItemPlay(const QBool& value);

// added by Dmitry 23.08.13
   QBool getItemSupported() const;
   void setItemSupported(const QBool& value);

   QString key() const;
   int compare( const BaseModelItem * pItem ) const;

private:
   QVariant itemId;
   QString itemName;
   QString itemPath;
   QBool itemChecked;
   QBool folder;
   QBool itemPlay;
   QBool itemSupported; // added by Dmitry 23.08.13
};

#endif // DHAVN_APPFILEMANAGER_BASEMODELITEM_H
