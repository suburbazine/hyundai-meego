#include <QDeclarativeView>
#include <QDeclarativeComponent>
#include <QDeclarativeItem>
#include <QDeclarativeContext>

#include <DHAVN_AppFramework_QML_Def.h>

#include "engine/DHAVN_AppMediaPlayer_EngineMain.h"
#include "util/DHAVN_AppMediaPlayer_Bootstrap.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"

AppMediaPlayer_Bootstrap::COMPONENT_T::COMPONENT_T()
   :m_pLoader(NULL),
    m_pItem(NULL),
    m_pContext(NULL)
{
}

AppMediaPlayer_Bootstrap::COMPONENT_T::~COMPONENT_T()
{
   delete m_pItem;
   delete m_pContext;
   delete m_pLoader;
}
//{  modified by edo.lee 2013.02.04
AppMediaPlayer_Bootstrap::AppMediaPlayer_Bootstrap( AppMediaPlayer_EngineMain *engine, QObject *parent )
   : QObject ( parent ),
     m_pEngine( engine ),
     //m_pRoot(NULL)
     m_pFrontRoot(NULL),
     m_pRearRoot(NULL)
{
   m_pFrontView = m_pEngine->GetDeclarativeView( AppEngineQMLConstants::APP_FRONT_SCREEN );
   m_pRearView = m_pEngine->GetDeclarativeView( AppEngineQMLConstants::APP_REAR_SCREEN );//01.26
  // m_pRootContext = m_pView->rootContext();

  m_LoadingDisplay = DISPLAY_FRONT;
  m_LoadingMode = APP_MEDIA_MODE_AUDIO;

   m_componentSources[APP_MEDIA_MODE_AUDIO] = "/app/share/AppMediaPlayer/qml/audio/DHAVN_MusicPlayer.qml";
   m_componentSources[APP_MEDIA_MODE_VIDEO] = "/app/share/AppMediaPlayer/qml/video/DHAVN_VP_main.qml";
   m_componentSources[APP_MEDIA_MODE_PHOTO_FRONT] = "/app/share/AppMediaPlayer/qml/photo/DHAVN_AppPhoto_main.qml";
   m_componentSources[APP_MEDIA_MODE_PHOTO_REAR] = "/app/share/AppMediaPlayer/qml/photo/DHAVN_AppPhoto_main.qml";
   m_RearVideoSource = "/app/share/AppMediaPlayer/qml/video/DHAVN_VP_rear_main.qml";
}

AppMediaPlayer_Bootstrap::~AppMediaPlayer_Bootstrap()
{
}

// { modified by bluepark 2012.08.09
// for booting time improvement
/*
void AppMediaPlayer_Bootstrap::loadComponents()
{
   loadComponent( APP_MEDIA_MODE_AUDIO );
   loadComponent( APP_MEDIA_MODE_VIDEO );
   loadComponent( APP_MEDIA_MODE_PHOTO_FRONT );
   loadComponent( APP_MEDIA_MODE_PHOTO_REAR );
}
*/
void AppMediaPlayer_Bootstrap::loadComponents(APP_MEDIA_MODE_T mode, DISPLAY_T display)
{
    MP_MEDIUM << "mode:" << mode << "display:" << display << LOG_ENDL;
    
    loadComponent( mode, display );
}
// } modified by bluepark

QDeclarativeContext *AppMediaPlayer_Bootstrap::componentContext( APP_MEDIA_MODE_T mode , DISPLAY_T display)
{
    MP_MEDIUM << "mode:" << mode << "display:" << display << LOG_ENDL;
    
    if ( mode == APP_MEDIA_MODE_MAX )
        return NULL;

   //return m_components[mode].m_pContext;
   if ( display == DISPLAY_REAR ) 
        return m_components[mode][1].m_pContext;
    else 
        return m_components[mode][0].m_pContext;
}

QDeclarativeItem *AppMediaPlayer_Bootstrap::componentItem( APP_MEDIA_MODE_T mode, DISPLAY_T display )
{
    MP_MEDIUM << "mode:" << mode << "display:" << display << LOG_ENDL;
    
    if ( mode == APP_MEDIA_MODE_MAX )
        return NULL;

   //return m_components[mode].m_pItem;
   if ( display == DISPLAY_REAR ) 
        return m_components[mode][1].m_pItem;
    else
        return m_components[mode][0].m_pItem;
}

void AppMediaPlayer_Bootstrap::finishComponentCreation( APP_MEDIA_MODE_T mode , DISPLAY_T display)
{
    MP_MEDIUM << "mode:" << mode << "display:" << display << LOG_ENDL;
    
    if ( mode == APP_MEDIA_MODE_MAX )
        return;
 
    int nIdx = 0;
    if( display == DISPLAY_REAR )
        nIdx = 1;  

   if ( m_components[mode][nIdx].m_pItem != NULL || m_components[mode][nIdx].m_pContext == NULL )
   {
      MP_MEDIUM << "Could not finish component creation: it is either created already or not loaded yet." << LOG_ENDL;
      return;
   }

  	m_components[mode][nIdx].m_pItem = (QDeclarativeItem*)m_components[mode][nIdx].m_pLoader->create( m_components[mode][nIdx].m_pContext );
    if ( m_components[mode][nIdx].m_pLoader->isError() )
    {
        foreach( QDeclarativeError err, m_components[mode][nIdx].m_pLoader->errors() ) 
        {
            MP_MEDIUM << err.toString() << LOG_ENDL;
        }
        return;
    }
    
    if (  m_components[mode][nIdx].m_pItem != NULL )
    {
        MP_MEDIUM << "Reparent" << LOG_ENDL;
		if(nIdx == 0 && m_pFrontRoot != NULL )
        	m_components[mode][nIdx].m_pItem->setParentItem( m_pFrontRoot );
		else if(nIdx == 1 && m_pRearRoot != NULL )
			m_components[mode][nIdx].m_pItem->setParentItem( m_pRearRoot );
    } // else we will be notified once the view becomes ready  

}


void AppMediaPlayer_Bootstrap::reparent( QObject *root )
{
   MP_MEDIUM << (void *)root << LOG_ENDL;
   MP_MEDIUM << (void*)qobject_cast<QDeclarativeItem *>(root) << LOG_ENDL;

	if(m_pFrontRoot == NULL)
	   m_pFrontRoot = (QDeclarativeItem *)root;
	else
		m_pRearRoot = (QDeclarativeItem *)root;
   // check if we have created items, not targeted to the root element:

   for( unsigned int i=0; i < APP_MEDIA_MODE_MAX; i++ )
    {
        if ( m_components[i][0].m_pItem != NULL )
        {
            m_components[i][0].m_pItem->setParentItem( (QDeclarativeItem *)root );
        }

        if ( m_components[i][1].m_pItem != NULL )
        {
            m_components[i][1].m_pItem->setParentItem( (QDeclarativeItem *)root );
        }
    }
}


void AppMediaPlayer_Bootstrap::loadComponent( APP_MEDIA_MODE_T mode , DISPLAY_T display )
{
    MP_LOG << "mode:" << mode << "display:" << display << LOG_ENDL;
    
    if ( mode == APP_MEDIA_MODE_MAX )
        return;

   // { modified by bluepark 2012.08.09
   // for booting time improvement
   /*
   m_components[mode].m_pLoader = new QDeclarativeComponent( m_pView->engine(),
                                                         m_componentSources[mode]);
   */
 
   m_LoadingDisplay = display;
   m_LoadingMode = mode;
   
   int nIdx = 0;
    if( display == DISPLAY_REAR )
        nIdx = 1;
		
        if( display == DISPLAY_REAR ) {
            if( m_components[mode][1].m_pLoader == NULL ) // modified by kihyung 2013.10.07
			{
				if(mode == APP_MEDIA_MODE_VIDEO) 
					m_components[mode][1].m_pLoader = new QDeclarativeComponent( m_pRearView->engine(), m_RearVideoSource);
				else
                	m_components[mode][1].m_pLoader = new QDeclarativeComponent( m_pRearView->engine(), m_componentSources[mode]);

				m_components[mode][1].m_pContext = NULL;
                MP_MEDIUM << "rear :" << mode << DISPLAY_REAR << " created" << LOG_ENDL;
            }
        }
        else if( display == DISPLAY_FRONT ){
            if( m_components[mode][0].m_pLoader == NULL ) // modified by kihyung 2013.10.07
		    {
                m_components[mode][0].m_pLoader = new QDeclarativeComponent( m_pFrontView->engine(), m_componentSources[mode]);
				m_components[mode][0].m_pContext = NULL;

                MP_MEDIUM << "front :" << mode << DISPLAY_FRONT << " created" << LOG_ENDL;
            }
        }
    //}

    if ( m_components[mode][nIdx].m_pLoader->isLoading() )
    {
        connect( m_components[mode][nIdx].m_pLoader, SIGNAL(statusChanged(QDeclarativeComponent::Status)), this, SLOT(onComponentLoaded()));
    }
    else
    {
        onComponentLoaded();
    }
}


void AppMediaPlayer_Bootstrap::onComponentLoaded()
{
    MP_LOG << LOG_ENDL;
    
    //for ( unsigned int i = 0; i < APP_MEDIA_MODE_MAX; i++ )
    {
        // front      
        
        if (m_LoadingDisplay == DISPLAY_FRONT)
                        //&& m_components[m_LoadingMode][0].m_pLoader != NULL && m_components[m_LoadingMode][0].m_pContext == NULL ) //removed by junam 2013.10.10 for ITS_KOR_194466
        {
            if(m_components[m_LoadingMode][0].m_pLoader != NULL && m_components[m_LoadingMode][0].m_pContext == NULL ) //added by junam 2013.10.10 for ITS_KOR_194466
            {
                if ( m_components[m_LoadingMode][0].m_pLoader->isError() )
                {
                    MP_LOG << "Loading component " << m_LoadingMode << "failed" << LOG_ENDL;
                    foreach( QDeclarativeError error, m_components[m_LoadingMode][0].m_pLoader->errors())
                        MP_LOG << error.toString() << LOG_ENDL;
                }
                else if ( m_components[m_LoadingMode][0].m_pLoader->isReady() )
                {
                    m_components[m_LoadingMode][0].m_pContext = new QDeclarativeContext( m_pFrontView->engine() );
                    MP_MEDIUM << "front:" << m_LoadingMode << DISPLAY_FRONT << " created" << LOG_ENDL;
                    //emit componentLoaded( m_LoadingMode, DISPLAY_FRONT ); //removed by junam 2013.10.10 for ITS_KOR_194466
                }
            }
            //{added by junam 2013.10.10 for ITS_KOR_194466
            if( m_components[m_LoadingMode][0].m_pContext && m_components[m_LoadingMode][0].m_pLoader && m_components[m_LoadingMode][0].m_pLoader->isReady())
            {
                MP_MEDIUM<<"emit componentLoaded - DISPLAY_FRONT " << LOG_ENDL;
                emit componentLoaded( m_LoadingMode, DISPLAY_FRONT );
            }
            //}added by junam
        }
        // rear
        else if (m_LoadingDisplay == DISPLAY_REAR)
                        //&& m_components[m_LoadingMode][1].m_pLoader != NULL && m_components[m_LoadingMode][1].m_pContext == NULL )//removed by junam 2013.10.10 for ITS_KOR_194466
        {
            if( m_components[m_LoadingMode][1].m_pLoader != NULL && m_components[m_LoadingMode][1].m_pContext == NULL ) //added by junam 2013.10.10 for ITS_KOR_194466
            {
                if ( m_components[m_LoadingMode][1].m_pLoader->isError() )
                {
                    MP_LOG << "Loading component " << m_LoadingMode << "failed" << LOG_ENDL;
                    foreach(QDeclarativeError error, m_components[m_LoadingMode][1].m_pLoader->errors())
                        MP_LOG << error.toString() << LOG_ENDL;
                }
                else if ( m_components[m_LoadingMode][1].m_pLoader->isReady() )
                {
                    m_components[m_LoadingMode][1].m_pContext = new QDeclarativeContext( m_pRearView->engine() );

                    MP_MEDIUM << "rear:" << m_LoadingMode << DISPLAY_REAR << " created" << LOG_ENDL;
                    //emit componentLoaded( m_LoadingMode, DISPLAY_REAR ); //removed by junam 2013.10.10 for ITS_KOR_194466
                }
            }
            //{added by junam 2013.10.10 for ITS_KOR_194466
            if(m_components[m_LoadingMode][1].m_pContext && m_components[m_LoadingMode][1].m_pLoader && m_components[m_LoadingMode][1].m_pLoader->isReady())
            {
                MP_MEDIUM<<"emit componentLoaded -  DISPLAY_REAR " << LOG_ENDL;
                emit componentLoaded( m_LoadingMode, DISPLAY_REAR );
            }
            //}added by junam
        }
    }
}
// }  modified by edo.lee 
