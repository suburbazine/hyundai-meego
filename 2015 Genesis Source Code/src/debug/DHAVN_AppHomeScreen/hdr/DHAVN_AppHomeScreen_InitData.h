#ifndef DHANV_APPHOMESCREEN_INITDATA_H
#define DHANV_APPHOMESCREEN_INITDATA_H
#include <QObject>
#include <uistate_manager_def.h>
#include <DHAVN_AppHomeScreen_Def_Private.h>
#include <DHAVN_AppFramework_AppVextEngineQML_Def.h>
#include <DHAVN_AppHomeScreen_IconsModel.h>
#include <DHAVN_AppHomeScreen_AppData.h>
#include <DHAVN_AppHomeScreen_Log_Def.h>

class InitData : public QObject
{
   Q_OBJECT

public:
   InitData( AppData *pAppData );
   ~InitData(){};

   AppData* GetAppData(){ return m_pAppData; }

   Q_INVOKABLE QString GetImage( int nID );
   Q_INVOKABLE QString GetColor( int nColor );

   int GetDisableCount ( const QList<HSDefP::APP_DATA_T> &lData );
   bool GetEnable ( const QList<HSDefP::APP_DATA_T> &lData );
   bool GetEnableInDrivingMode ( const QList<HSDefP::APP_DATA_T> &lData );

   void FillViewModel( int nDisplay, IconsModel *pModel, HSDefP::VIEW_ID_T nViewId, int nTopMarginDelta );
   void UpdateMargins( IconsModel *pModel, int nTopMarginDelta );
   QString GetBG( int nCountItems, bool isMainMenu = false );
   QString GetBGMask( int nCountItems, bool isMainMenu = false );
   QPoint GetBGMaskPosition( int nCountItems, bool isMainMenu = false );

private:
   void FillAppList( IconsModel *pModel, int nDisplay, HSDefP::VIEW_ID_T nViewId );
   int getViewLayoutIdx( bool isMainMenu, int nCountItems );
   //void setSettingViewLayout( QList<IconItem*> *iconList, int curSettingIdx );

   AppData *m_pAppData;

   int m_nCountryVariant;
};
#endif // DHANV_APPHOMESCREEN_INITDATA_H
