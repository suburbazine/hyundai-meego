#ifndef DHAVN_APPHOMESCREEN_ICONSMODEL_H
#define DHAVN_APPHOMESCREEN_ICONSMODEL_H

#include <QList>
#include <QVariant>
#include <QAbstractListModel>
#include <DHAVN_AppHomeScreen_IconItem.h>
#include <DHAVN_AppHomeScreen_Log_Def.h>

class IconsModel : public QAbstractListModel
{
   Q_OBJECT

public:
   IconsModel( IconItem* prototype, QObject* parent = 0 );
   ~IconsModel();
   int rowCount( const QModelIndex &parent = QModelIndex() ) const;
   QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
   void AppendItems( const QList<IconItem*> &items );
   IconItem* Find( const HSDefP::APP_ID_T &nAppId );
   bool removeItems( int row, int count, const QModelIndex &parent = QModelIndex() );
   void Clear();
   bool SetPosition( const int &nItemIndex, const int &nXPos, const int &nYPos );
   void RemoveItem( const HSDefP::APP_ID_T &nAppId );
   void RemoveItem( const HSDefP::VIEW_ID_T &nViewId );
   void SetDataList( const QList<HSDefP::APP_DATA_T> &lData );
   void SetMainMenuFlag( bool isMainMenu );
   bool IsMainMenu();
   void SetFocusIndex( int nIndex );
   void SetFocusIconIndex( int iconIdx );
   void SetFocusIconIndexByViewID( HSDefP::VIEW_ID_T iconID );
   void SetFocusIconIndexByAppID( HSDefP::APP_ID_T iconID );

   int GetIndexFromName(QString str);
   int GetIndexFromAppID(HSDefP::APP_ID_T iconID);
   int GetIndexFromViewID(HSDefP::VIEW_ID_T iconID);

   int GetFocusedAppId(){ return m_pList.at( m_nFocusIndex )->Data( IconItem::nAppId ).toInt(); }

   Q_PROPERTY( int nFocusIndex READ GetFocusIndex WRITE SetFocusIndex NOTIFY updateFocusIndex );
   Q_PROPERTY( QString sFocusedText READ GetFocusedText NOTIFY UpdateFocusedText );
   Q_PROPERTY( int nFocusedViewId READ GetFocusedViewId NOTIFY UpdateFocusedViewId );
   Q_PROPERTY( int nFocusedAppId READ GetFocusedAppId NOTIFY UpdateFocusedAppId );
   Q_PROPERTY( int nCountItems READ GetCountItems CONSTANT );

signals:
   void ReloadModel();

public slots:
   void UpdateDataField( HSDefP::APP_DATA_T nID, QVariant );

private slots:
   void handleItemChange();

private:
   /** Q_PROPERTY getters */
   int GetFocusIndex(){ return m_nFocusIndex; }
   QString GetFocusedText(){ return m_pList.at( m_nFocusIndex )->Data( IconItem::sText ).toString(); }
   int GetFocusedViewId(){ return m_pList.at( m_nFocusIndex )->Data( IconItem::nViewId ).toInt(); }

   int GetCountItems(){ return rowCount(); }

signals:
   /** Q_PROPERTY signals */
   void updateFocusIndex();
   void UpdateFocusedText();
   void UpdateFocusedViewId();
   void UpdateFocusedAppId();

private:
   QModelIndex IndexFromItem( const IconItem* item ) const;
   IconItem* m_pPrototype;
   QList<IconItem*> m_pList;
   QList<HSDefP::APP_DATA_T> m_lData;
   bool m_bIsMainMenu;

   /** Q_PROPERTY members */
   int m_nFocusIndex;
};

#endif // DHAVN_APPHOMESCREEN_ICONSMODEL_H
