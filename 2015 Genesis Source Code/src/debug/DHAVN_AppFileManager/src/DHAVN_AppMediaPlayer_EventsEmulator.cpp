#include "DHAVN_AppMediaPlayer_EventsEmulator.h"
#include <QApplication>
#include <QDeclarativeView>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QDeclarativeItem>
#include <DHAVN_AppFramework_QML_Def.h>

EventsEmulator::EventsEmulator(QObject *parent):
    QObject(parent)
  ,m_eventFilterInstalled(false)
{
}

void EventsEmulator::lockScrolling(QDeclarativeItem *object, bool lock)
{
    if(!object)
        return;
    if(lock) {
        object->installEventFilter(this);
    } else {
        object->removeEventFilter(this);
    }
}

bool EventsEmulator::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj) // modified by Dmitry 17.08.13
    if(event->type() == QEvent::GraphicsSceneWheel) {
        QGraphicsSceneWheelEvent *wheelEvent = (QGraphicsSceneWheelEvent*)event;
        qDebug() << "ruinaede wheelEvent -> screenPos = " << wheelEvent->screenPos();
        qDebug() << "ruinaede wheelEvent -> scenePos = " << wheelEvent->scenePos();
        return true;
    }

    return false;
}

// added by Dmitry 11.09.13 for ITS0183775
void EventsEmulator::sendMouseRelease(QDeclarativeItem *object)
{
   if(!qApp || !object || !object->scene()) {
       return;
   }

   QGraphicsSceneMouseEvent *mouseEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseRelease);
   qApp->postEvent(object->scene(), mouseEvent);
}
// added by Dmitry 11.09.13 for ITS0183775

void EventsEmulator::sendWheel(int direction, int delta, QDeclarativeItem* object)
{
    if(!qApp || !object || !object->scene()) {
        return;
    }

    QGraphicsSceneWheelEvent *wheelEvent = new QGraphicsSceneWheelEvent(QEvent::GraphicsSceneWheel);
    qDebug() << "ruinaede setScenePos x: " << object->x() << " y:" << object->y();
    qDebug() << "ruinaede setScenePos " << object->mapToScene(QPointF( object->x() + 10, object->y() + 10 ));
    wheelEvent->setScenePos( QPointF(object->x() + object->width()/2, object->y() + object->height()/2) );

    switch(direction) {
    case AppEngineQMLConstants::JOG_UP:
    {
        wheelEvent->setDelta(delta);
        wheelEvent->setOrientation(Qt::Vertical);
        break;
    }
    case AppEngineQMLConstants::JOG_DOWN:
    {
        wheelEvent->setDelta(-delta);
        wheelEvent->setOrientation(Qt::Vertical);
        break;
    }
    case AppEngineQMLConstants::JOG_LEFT:
    {
        wheelEvent->setDelta(-delta);
        wheelEvent->setOrientation(Qt::Horizontal);
        break;
    }
    case AppEngineQMLConstants::JOG_RIGHT:
    {
        wheelEvent->setDelta(delta);
        wheelEvent->setOrientation(Qt::Horizontal);
        break;
    }
    default:
        return;
    }

    qApp->postEvent(object->scene(), wheelEvent);
}
