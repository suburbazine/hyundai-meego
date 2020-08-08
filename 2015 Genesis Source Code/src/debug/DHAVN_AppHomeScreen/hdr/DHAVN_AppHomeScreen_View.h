#ifndef DHAVN_APPHOMESCREEN_VIEW_H
#define DHAVN_APPHOMESCREEN_VIEW_H

#include <QObject>
#include <DHAVN_AppHomeScreen_Def_Private.h>
#include <DHAVN_AppHomeScreen_InitData.h>
#include <DHAVN_AppHomeScreen_IconsModel.h>
#include <DHAVN_AppHomeScreen_Log_Def.h>

class View : public QObject
{
   Q_OBJECT

public:
   View( int nDisplay, HSDefP::VIEW_ID_T nViewId, QString sViewName, InitData *pInitData, bool bTitleAvailable );
   ~View();

   IconsModel* Model(){ return m_pModel; }
   IconsModel* SubMenuModel(){ return m_pSubMenuIconModel; }
   HSDefP::VIEW_ID_T ViewId(){ return m_nViewId; }

   bool SetBGParameters();
   QString GetViewName(){ return m_sViewName; }

   void SetFocusIconIndex( int iconIdx );
   void SetFocusIconIndexByViewID( HSDefP::VIEW_ID_T iconID );
   void SetFocusIconIndexByAppID( HSDefP::APP_ID_T iconID );
   int  GetSubMenuFocusIconIndexByViewID( HSDefP::VIEW_ID_T iconID );
   int  GetSubMenuFocusIconIndexByAppID( HSDefP::APP_ID_T iconID );

   void UpdateSubMenuContext( HSDefP::VIEW_ID_T nView, QDeclarativeContext* pCtxt );


   /** Q_PROPERTY */
   Q_PROPERTY( QString sBG READ GetBg NOTIFY UpdateBg );
   Q_PROPERTY( QString sBGMask READ GetBgMask NOTIFY UpdateBgMask );
   Q_PROPERTY( int nBgMaskXPos READ GetBgMaskXPos NOTIFY UpdateBgMaskXPos );
   Q_PROPERTY( int nBgMaskYPos READ GetBgMaskYPos NOTIFY UpdateBGMaskYPos );
   Q_PROPERTY( bool bTitleAvailable READ GetTitleAvailable CONSTANT );
   Q_PROPERTY( bool bMediaAvailable READ GetMediaAvailable CONSTANT );
   Q_PROPERTY( int nFocusIndex READ GetFocusIndex WRITE SetFocusIndex NOTIFY UpdateFocusedIndex );
   Q_PROPERTY( int nViewId READ GetViewId CONSTANT );

private slots:
   void LoadModel();
   void LoadSubModel();

private:
   /** Q_PROPERTY getters */
   QString GetBg(){ return m_sBg; }
   QString GetBgMask(){ return m_sBGMask; }
   int GetBgMaskXPos(){ return m_nBgMaskXPos; }
   int GetBgMaskYPos(){ return m_nBgMaskYPos; }
   bool GetTitleAvailable(){ return m_bTitleAvailable; }
   bool GetMediaAvailable(){ return m_bMediaAvailable; }
   int GetFocusIndex(){ return m_nFocusIndex; }
   int GetViewId(){ return m_nViewId; }

   /** Q_PROPERTY setters */
   void SetFocusIndex( int nIndex );
   void SetFocusIndexByIconID( HSDefP::VIEW_ID_T iconID );
   void SetFocusIndexByIconID( HSDefP::APP_ID_T iconID );

signals:
   /** Q_PROPERTY signals */
   void UpdateFocusedIndex();
   void UpdateBg();
   void UpdateBgMask();
   void UpdateBgMaskXPos();
   void UpdateBGMaskYPos();
   void updateSubModel();

   /** Members */
private:
   InitData *m_pInitData;
   IconsModel *m_pModel;
   IconsModel *m_pSubMenuIconModel;
   HSDefP::VIEW_ID_T m_nViewId;
   HSDefP::VIEW_ID_T m_nSubViewId;
   QDeclarativeContext* m_pCtxt;
   QString m_sViewName;

   /** Q_PROPERTY members */
   QString m_sBg;
   QString m_sBGMask;
   int m_nBgMaskXPos;
   int m_nBgMaskYPos;
   bool m_bTitleAvailable;
   int m_nDisplay;
   bool m_bMediaAvailable;
   int m_nFocusIndex;
};

#endif // DHAVN_APPHOMESCREEN_VIEW_H
