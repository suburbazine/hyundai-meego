#ifndef DHAVN_IPODCONTROLLER_EVENTLOOPTIMER_H
#define DHAVN_IPODCONTROLLER_EVENTLOOPTIMER_H

/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                                                                *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <QTimer>
#include <QObject>
#include <QDebug>
#include <QEventLoop>
#include <QVariantMap>
#include <DHAVN_LogUtil.h>

class CEventloopTimer : public QObject
{
	Q_OBJECT
private:
    USE_LOG_UTIL
public:
        // { Added by sungha.choi 2013.03.28
        CEventloopTimer();
        CEventloopTimer(QEventLoop *loop,int timeout = 5000);
        virtual ~CEventloopTimer();
        void clear();

        void start(QEventLoop *loop,int timeout = 5000);    // added by sungha.choi 2013.04.12
        void stop();
        // } Added by sungha.choi 2013.03.28
#if 0
        CEventloopTimer(QEventLoop *loop,int timeout = 5000){
                m_loop = loop;
              //  connect(this,SIGNAL(eventloopTimeout()),&loop,SLOT(quit()));
                QTimer::singleShot(timeout,this,SLOT(wakeUpAndEmit()));
	}
        CEventloopTimer(){}
        //redundant func!!!
        void clear(){
        }
#endif
public slots:
	void wakeUpAndEmit();
        void getListComplete(QVariantMap,int)        ;
signals:
        void eventloopTimeout();
private:
        QEventLoop *m_loop;
        QTimer *m_timer;        // Added by sungha.choi 2013.03.28

};

#endif //DHAVN_IPODCONTROLLER_EVENTLOOPTIMER_H
