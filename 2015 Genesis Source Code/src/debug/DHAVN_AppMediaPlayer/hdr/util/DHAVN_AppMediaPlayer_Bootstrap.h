#ifndef DHAVN_APPMEDIAPLAYER_BOOTSTRAP_H
#define DHAVN_APPMEDIAPLAYER_BOOTSTRAP_H

#include <QObject>
#include <QHash>
#include <uistate_manager_def.h>
#include <DHAVN_AppFramework_AppVextEngineQML_Def.h>
#include "engine/DHAVN_AppMediaPlayer_EngineBase.h"

class AppMediaPlayer_EngineMain;
class QDeclarativeView;
class QDeclarativeComponent;
class QDeclarativeItem;
class QDeclarativeContext;

class AppMediaPlayer_Bootstrap : public QObject
{
   Q_OBJECT

public:
   AppMediaPlayer_Bootstrap( AppMediaPlayer_EngineMain *engine, QObject *parent = NULL );
   ~AppMediaPlayer_Bootstrap();

   // { modified by bluepark 2012.08.09
   // for booting time improvement
   // void loadComponents();
   void loadComponents(APP_MEDIA_MODE_T mode,DISPLAY_T display);
   // } modified by bluepark
   //QDeclarativeContext *componentContext( APP_MEDIA_MODE_T mode );
   //QDeclarativeItem    *componentItem( APP_MEDIA_MODE_T mode );
   QDeclarativeContext *componentContext( APP_MEDIA_MODE_T mode, DISPLAY_T display );
   QDeclarativeItem    *componentItem( APP_MEDIA_MODE_T mode, DISPLAY_T display );

   //void finishComponentCreation( APP_MEDIA_MODE_T mode );
   void finishComponentCreation( APP_MEDIA_MODE_T mode, DISPLAY_T display );
   
   void reparent( QObject *root );
signals:
   //void componentLoaded( int mode );
   void componentLoaded( int mode, int display );
   
private slots:
   void onComponentLoaded();
private:
   //void loadComponent( APP_MEDIA_MODE_T mode );
   void loadComponent( APP_MEDIA_MODE_T mode, DISPLAY_T display );

   
   AppMediaPlayer_EngineMain        *m_pEngine;
   //QDeclarativeView                 *m_pView;
   QDeclarativeView                 *m_pFrontView;
   QDeclarativeView                 *m_pRearView;
   //QDeclarativeView                 *m_pRearView; //01.26
   DISPLAY_T							m_LoadingDisplay;
   APP_MEDIA_MODE_T						m_LoadingMode;
   
   //QDeclarativeItem                 *m_pRoot;
   QDeclarativeItem                 *m_pFrontRoot;
   QDeclarativeItem                 *m_pRearRoot;
   //QDeclarativeContext              *m_pRootContext;
   QHash<APP_MEDIA_MODE_T, QString>  m_componentSources;
   QString 							m_RearVideoSource;

   struct COMPONENT_T {
      COMPONENT_T();
      ~COMPONENT_T();
      
      QDeclarativeComponent *m_pLoader;
      QDeclarativeItem      *m_pItem;
      QDeclarativeContext   *m_pContext;
   };

   //COMPONENT_T  m_components[APP_MEDIA_MODE_MAX];
   COMPONENT_T  m_components[APP_MEDIA_MODE_MAX][2];
};


#endif // DHAVN_APPMEDIAPLAYER_BOOTSTRAP_H
