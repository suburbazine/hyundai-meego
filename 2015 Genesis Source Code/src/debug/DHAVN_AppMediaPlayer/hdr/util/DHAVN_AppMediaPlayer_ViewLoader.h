#ifndef DHAVN_APPMEDIAPLAYER_VIEWLOADER_H
#define DHAVN_APPMEDIAPLAYER_VIEWLOADER_H

#ifndef APPMEDIAPLAYER_VIEWLOADER_H
#define APPMEDIAPLAYER_VIEWLOADER_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QUrl>

class QDeclarativeItem;
class QDeclarativeContext;
class QDeclarativeView;
class QDeclarativeComponent;
class QDeclarativeEngine;
class QGraphicsObject;

/** @class AppMediaPlayer_ViewLoader
 * This is helper class to simplify the loading of the views.
 *
 * By design, each view is identified by it's name. The view QML source
 * is built on the fly, by prepending the path to the view folder and
 * appending the .qml extension.
 *
 * In the current implementation, each view is assigned it's own context
 * which can be used by the controllers to set a model, for example.
 * */
class AppMediaPlayer_ViewLoader : public QObject
{
    Q_OBJECT
public:
    AppMediaPlayer_ViewLoader( QDeclarativeView *view, QObject *parent = NULL );
    ~AppMediaPlayer_ViewLoader();

    /** @brief Loads the view by it's name.
     * While view loading is asynchronous operation, the caller should
     * treat this method as a request for the view. Once the view is loaded
     * the viewLoaded() signal will be emitted.
     *
     * The AppMediaPlayer_ViewLoader class owns the view instances, so
     *   1. Don't try to remove it.
     *   2. Do not complain about loading delays: it will cache the instances.
     * */
    void loadView( const QString &viewName );

    void setContext( QDeclarativeContext *context );
    void setRootItem( QObject *root );
	QDeclarativeItem* getRootItem(); //added by edo.lee 2013.02.07
signals:
    /** @brief Signal emitted once the view is loaded.
     * @param viewName The view name, so caller could distinguish the view
     * it requested. Yes, it is possible to load several views a time.
     * @param pView The view itself. It should be ready for use, but:
     *   1. It will be invisible (visible property is set to false)
     *   2. It will be reparented to the root item of the video qml.
     *
     * @note It is not garanteed that this signal will be emitted ever. If
     * QML file for the requested view was failed to load, the signall will
     * not been emitted. However, it has logs within, so this should be
     * easy to debug.
     * */
    void viewLoaded( const QString &viewName,
                     QDeclarativeItem *pView,
                     QDeclarativeContext *pContext );

private slots:
    void loadComplete();

private:
    void addToLoadQueue( const QString &viewName );
    void reparentItem( QDeclarativeItem *pItem );
    QUrl viewLocation( const QString &viewName );

    struct VIEW_T
    {
        QString name;
        QDeclarativeContext *pContext;
        QDeclarativeComponent *pLoader;
        QDeclarativeItem *pItem;
    };

    QList<VIEW_T*>           m_loadQueue;
    QList<QDeclarativeItem*> m_reparentQueue;
    QHash<QString,VIEW_T*>   m_views;
    QDeclarativeEngine      *m_pEngine;
    QGraphicsObject         *m_pRootObject;
    QDeclarativeItem        *m_pVideoRoot;
    QDeclarativeView        *m_pView;
    QDeclarativeContext     *m_pRootContext;
};


#endif // APPMEDIAPLAYER_VIEWLOADER_H


#endif // DHAVN_APPMEDIAPLAYER_VIEWLOADER_H
