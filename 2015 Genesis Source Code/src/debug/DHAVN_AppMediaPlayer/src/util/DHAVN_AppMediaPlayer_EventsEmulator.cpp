#include "util/DHAVN_AppMediaPlayer_EventsEmulator.h"
#include <QApplication>
#include <QDeclarativeView>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QDeclarativeItem>
#include <DHAVN_AppFramework_QML_Def.h>
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"

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
    if(event->type() == QEvent::GraphicsSceneWheel) {
        QGraphicsSceneWheelEvent *wheelEvent = (QGraphicsSceneWheelEvent*)event;
        MP_LOG << "ruinaede wheelEvent -> screenPos = " << wheelEvent->screenPos() << LOG_ENDL;
        MP_LOG << "ruinaede wheelEvent -> scenePos = " << wheelEvent->scenePos() << LOG_ENDL;
        return true;
    }

    return false;
}

void EventsEmulator::sendWheel(int direction, int delta, QDeclarativeItem* object)
{
    if(!qApp || !object || !object->scene()) {
        return;
    }

    QGraphicsSceneWheelEvent *wheelEvent = new QGraphicsSceneWheelEvent(QEvent::GraphicsSceneWheel);
    MP_LOG << "ruinaede setScenePos x: " << object->x() << " y:" << object->y() << LOG_ENDL;
    MP_LOG << "ruinaede setScenePos " << object->mapToScene(QPointF( object->x() + 10, object->y() + 10 )) << LOG_ENDL;
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
