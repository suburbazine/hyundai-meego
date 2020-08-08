#include "engine/DHAVN_AppMediaPlayer_EventFilter.h"

// { added by sungha.choi 2013.08.07 for ITS 0181470
EventFilter::EventFilter(QObject *parent) : QObject(parent)
{
    m_EventQueue.clear();
    connect(this, SIGNAL(requestDequeue()), this, SLOT(onDequeue()), Qt::QueuedConnection); // this signal/slot must connect by Queued Connection.
}

EventFilter::~EventFilter()
{
    m_EventQueue.clear();
}

void EventFilter::onEnqueue(EVENT_DISPLAY evt)
{
    m_EventQueue.enqueue(evt);
    if(m_EventQueue.count() == 1) {
        emit requestDequeue();
    }
}

void EventFilter::onDequeue()
{
    MP_LOG << "onDequeue() size = " << m_EventQueue.count() << LOG_ENDL;
    if(m_EventQueue.count() > 0)
    {
        EVENT_DISPLAY queuedEvent = m_EventQueue.dequeue();
        EVT_ID_T evtid = queuedEvent.m_event.GetEventId();
        int size = m_EventQueue.count();
#if 0   // for debug
        for (int i = 0; i < size; i ++)
        {
            MP_HIGH << "onDequeue() event id = " << (EVT_PUBLIC_ID_T)m_EventQueue[i].m_event.GetEventId() << LOG_ENDL;
        }
#endif
        switch(evtid)
        {
            case EVT_AV_MODE_CHANGE:    // if event loop has multiple EVT_AV_MODE_CHANGE, only last EVT_AV_MODE_CHANGE will be executed.
            {
                int i;
                for (i = 0; i < size; i ++)
                {
                    if(evtid == m_EventQueue[i].m_event.GetEventId())
                        break;
                }
                if(i == size)   // when duplicated EVT_AV_MODE_CHANGE is none.
                {
                    emit sendEventForExecute(queuedEvent);
                }
                else
                {
                    MP_LOG << "this EVT_AV_MODE_CHANGE was skipped" << LOG_ENDL;
                }
            }
            break;

            default :
            {
                emit sendEventForExecute(queuedEvent);
            }
            break;
        }
        if(size > 0) {
            emit requestDequeue();
        }
    }
}
// } added by sungha.choi 2013.08.07 for ITS 0181470
