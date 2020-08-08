#ifndef DHAVN_APPMEDIAPLAYER_EVENTFILTER_H
#define DHAVN_APPMEDIAPLAYER_EVENTFILTER_H

#include "util/DHAVN_AppMediaPlayer_Settings.h"
#include <QQueue>   // added by sungha.choi 2013.08.07 for ITS 0181470


class EVENT_DISPLAY   // { added by sungha.choi 2013.08.07 for ITS 0181470
{
    public:
        EVENT_DISPLAY(Event evt, DISPLAY_T disp)
        : m_event(evt.GetEventId(), evt.GetSource(), evt.GetTarget(), evt.GetCommand(), evt.Data()),
          m_display(disp) { };
        ~EVENT_DISPLAY() { };
        Event m_event;
        DISPLAY_T m_display;
};

class EventFilter : public QObject
{
    Q_OBJECT
    public:
        EventFilter(QObject *parent = 0);
        ~EventFilter();

    signals:
        void sendEventForExecute(EVENT_DISPLAY evt);
        void requestDequeue();

    public slots:
        void onEnqueue(EVENT_DISPLAY evt);
        void onDequeue();

    // removed by sungha.choi

    private :
        QQueue<EVENT_DISPLAY> m_EventQueue;
};   // } added by sungha.choi 2013.08.07 for ITS 0181470

#endif // DHAVN_APPMEDIAPLAYER_EVENTFILTER_H
