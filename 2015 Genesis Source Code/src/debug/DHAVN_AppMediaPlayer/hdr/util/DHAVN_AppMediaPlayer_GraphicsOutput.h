#ifndef APP_MEDIA_PLAYER_GRAPHICSOUTPUT_H
#define APP_MEDIA_PLAYER_GRAPHICSOUTPUT_H

#include <QDeclarativeItem>
#include <QGraphicsProxyWidget>

/** @brief Proxy object to make graphics output possible.
 * This class is intended to be used by photo & video modes and provides
 * some sort of a bridge between QML UI and player engine backend.
 * */

class GraphicsOutput : public QDeclarativeItem
{
   Q_OBJECT
   Q_PROPERTY( QObject *proxy READ proxy );
public:
   GraphicsOutput( QDeclarativeItem *parent = NULL )
       : QDeclarativeItem(parent),
         m_pProxy(new QGraphicsProxyWidget(this)){};
   ~GraphicsOutput(){}

   QObject *proxy() { return dynamic_cast<QObject*>(m_pProxy); }
private:
   QGraphicsProxyWidget *m_pProxy;

};

#endif //APP_MEDIA_PLAYER_GRAPHICSOUTPUT_H
