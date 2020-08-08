#ifndef DHAVN_APPHOMESCREEN_ICONITEM_H
#define DHAVN_APPHOMESCREEN_ICONITEM_H

#include <QObject>
#include <QHash>
#include <QVariant>
#include <DHAVN_AppHomeScreen_Def_Private.h>
#include <DHAVN_AppHomeScreen_Log_Def.h>

class IconItem : public QObject
{
   Q_OBJECT

public:
   enum Roles
   {
     nViewId = Qt::UserRole + 1,
     nAppId,
     sText,
     sImage,
     nXPos,
     nYPos,
     bEnabled,
     bEnableInDriving
   };

public:
   IconItem();
   IconItem( const HSDefP::VIEW_ID_T &nViewId,
             const HSDefP::APP_ID_T &nAppId,
             const QString &sText,
             const QString &sImage,
             const bool bEnabled,
             const int iDisableCount,
             const bool bEnableInDriving,
             const QList<HSDefP::APP_DATA_T> &lDataList );

   ~IconItem();

   QVariant Data( int nRole ) const;
   QHash<int, QByteArray> RoleNames() const;
   HSDefP::APP_ID_T GetAppId(){ return m_nAppId; }
   HSDefP::VIEW_ID_T GetViewId(){ return m_nViewId; }

   void SetPosition( const int &nXPos, const int &nYPos );

   QString GetText() { return m_sText; }

public slots:
   void UpdateDataField( HSDefP::APP_DATA_T nID, QVariant data );

signals:
   void dataChanged();

private:
   HSDefP::VIEW_ID_T m_nViewId;
   HSDefP::APP_ID_T m_nAppId;
   QString m_sText;
   QString m_sImage;
   QList<HSDefP::APP_DATA_T> m_lDataList;
   int m_nXPos;
   int m_nYPos;
   bool m_bEnabled;
   int m_nDisableCount;
   bool m_bEnableInDriving;
};

#endif // DHAVN_APPHOMESCREEN_ICONITEM_H
