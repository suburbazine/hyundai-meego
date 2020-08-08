#include <QDeclarativeView>
#include <QDeclarativeItem>
#include <QDeclarativeContext>
#include <QDeclarativeComponent>

#include <QFile>

#include "engine/DHAVN_AppMediaPlayer_EngineMain.h"
#include "util/DHAVN_AppMediaPlayer_ViewLoader.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"

AppMediaPlayer_ViewLoader::AppMediaPlayer_ViewLoader( QDeclarativeView *view,
                                                      QObject *parent )

    : QObject(parent)
{
    m_pEngine = view->engine();
    m_pRootObject = NULL;
    m_pVideoRoot = NULL;
    m_pRootContext = NULL;

    m_pView = view;
}

AppMediaPlayer_ViewLoader::~AppMediaPlayer_ViewLoader()
{
    for(int i = 0; i < m_views.values().count(); i++)
    {
        VIEW_T* tmp = m_views.values().at(i);
        if(tmp)
        {
            delete tmp;
        }
    }
    m_views.clear();
}

void AppMediaPlayer_ViewLoader::loadView( const QString &viewName )
{
   MP_LOG << "viewName" << viewName << LOG_ENDL;

    if ( m_views.contains(viewName) )
    {
        VIEW_T *view = m_views.value(viewName);
        emit viewLoaded( viewName, view->pItem, view->pContext );
    }
    else
    {
        // we are loading it now?
        for( int i=0; i < m_loadQueue.size(); i++ )
        {
            if ( viewName == m_loadQueue.at(i)->name )
            {
                MP_LOG << "View" << viewName << "is loading." << LOG_ENDL;
                return;
            }
        }

        // looks like we don't, in this case start loading now:
        addToLoadQueue( viewName );
    }
}

void AppMediaPlayer_ViewLoader::addToLoadQueue( const QString &viewName )
{
	MP_LOG << LOG_ENDL;
    VIEW_T *view = new VIEW_T();

    view->name = viewName;
    view->pContext = NULL;
    view->pItem = NULL;
    view->pLoader = new QDeclarativeComponent( m_pEngine,
                                               viewLocation(viewName),
                                               this);

    m_loadQueue.append( view );

    if ( view->pLoader->isLoading() )
    {
        connect( view->pLoader,
                 SIGNAL(statusChanged(QDeclarativeComponent::Status)),
                 this,
                 SLOT(loadComplete()));
    }
    else
    {
        loadComplete();
    }
}


void AppMediaPlayer_ViewLoader::loadComplete()
{	
	MP_LOG << LOG_ENDL;
    QList<VIEW_T*>::Iterator iter;
    QList<QList<VIEW_T*>::Iterator> removeList;
    QList<VIEW_T*> notifyList;
    //static LocalizeTrigger locTrigger; // removed by richard.dale 2012.11.03 for CR 14076
    
    for( iter = m_loadQueue.begin(); iter != m_loadQueue.end(); iter++ )
    {
        VIEW_T *view = *iter;
        MP_LOG<<view->pLoader->isReady()<<" m_pRootContext"<< m_pRootContext << LOG_ENDL;
        if ( view->pLoader->isError() )
        {
            MP_LOG << "Loading screen " << view->name << "Failed." << LOG_ENDL;
            foreach (QDeclarativeError error, view->pLoader->errors())
                MP_LOG << error.toString() << LOG_ENDL;
            
            removeList << iter;
            delete view;            
        }

		// { modified by eugene 2012.08.22 for static analysis defect #6962 
        //if ( view->pLoader->isReady() && m_pRootContext != NULL )
        else if ( view->pLoader->isReady() && m_pRootContext != NULL )
 		// } modified by eugene 2012.08.22 for static analysis defect #6962 
        {
            // move the view to the set of the available views:
            removeList << iter;
            m_views.insert( view->name, view );

            view->pContext = new QDeclarativeContext( m_pRootContext, this );
            view->pItem = qobject_cast<QDeclarativeItem*>(view->pLoader->create( view->pContext ));

            // Reparent the item:
            reparentItem( view->pItem );

            // Notifying clients should be defered till we end manipulation
            // with the m_loadQueue. The signals are synchronous and it is
            // possible that loadComplete() slot will be invoked during the
            // handling of the 'emit'.
            notifyList << view;
        }
    }

    foreach( iter, removeList )
    {
        iter = m_loadQueue.erase(iter); // modified by ravikanth 07-05-13, assigned return iter value to iter
    }
	//  added by edo.lee 2013.02.04
	if(notifyList.count() == 0)
	{
		MP_LOG<<"notifyList is Empty" << LOG_ENDL;
		return;
	}
	//  added by edo.lee
    for( iter = notifyList.begin(); iter != notifyList.end(); iter++ )
    {
        VIEW_T *view = *iter;
        emit viewLoaded( view->name, view->pItem, view->pContext );
    }
}


void AppMediaPlayer_ViewLoader::reparentItem( QDeclarativeItem *pItem )
{
	MP_LOG << LOG_ENDL;
    if ( m_pVideoRoot != NULL && pItem != NULL) // modified  by edo.lee2013.02.04
    {
        pItem->setParentItem( m_pVideoRoot );
        pItem->setProperty("visible", false);
    }
    else
    {
        // add the item into the reparent queue, to do this as soon we're
        // ready with the main QML.
        m_reparentQueue << pItem;
    }
}
//added by edo.lee 2013.02.07
QDeclarativeItem* AppMediaPlayer_ViewLoader::getRootItem()
{
	return m_pVideoRoot;
}


QUrl AppMediaPlayer_ViewLoader::viewLocation( const QString &viewName )
{
    static QString dataRoot = "/app/share/AppMediaPlayer/qml/video/";

    QString viewFileName = dataRoot + viewName + ".qml";

    // A trivial sanity check: check if file exists:
    if ( !QFile::exists( viewFileName ) )
    {
        MP_LOG << "View" << viewName << "cannot be loaded." << LOG_ENDL;
        MP_LOG << "Make sure that " << viewFileName << "exists." << LOG_ENDL;
    }

    return QUrl::fromLocalFile( viewFileName );
}

void AppMediaPlayer_ViewLoader::setContext( QDeclarativeContext *context )
{
   MP_LOG << (void*)context << LOG_ENDL;

   if ( context != NULL )
   {
      m_pRootContext = context;

      // if some views was requested before the context is ready will need to continue
      // view creation:
      loadComplete();
   }
}

void AppMediaPlayer_ViewLoader::setRootItem( QObject *root )
{
   MP_LOG << (void *)root << LOG_ENDL;

    m_pVideoRoot = (QDeclarativeItem *)root;

    // Now, we have a video root, so we can reparent pending items:
    foreach(QDeclarativeItem *item, m_reparentQueue)
    {
        reparentItem( item );
    }

    m_reparentQueue.clear();
}
