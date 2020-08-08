#ifndef DHAVN_APPMEDIAPLAYER_EVENTSEMULATOR_H
#define DHAVN_APPMEDIAPLAYER_EVENTSEMULATOR_H

#include <QObject>

class QDeclarativeItem;

class EventsEmulator : public QObject
{
    Q_OBJECT
public:
    explicit EventsEmulator(QObject *parent = 0);
    Q_INVOKABLE void lockScrolling(QDeclarativeItem* object, bool lock);
    Q_INVOKABLE void sendWheel(int direction, int delta, QDeclarativeItem* object);
    virtual bool eventFilter(QObject *obj, QEvent *event);
private:
    bool m_eventFilterInstalled;
};

#endif // DHAVN_APPMEDIAPLAYER_EVENTSEMULATOR_H
