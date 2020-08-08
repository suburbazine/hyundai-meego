/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
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
#include <DHAVN_IPodController_eventloopTimer.h>
#include <DHAVN_IPodController_Def.h>
#include <DHAVN_IPodController_CommonVars.h>

// { Added by sungha.choi 2013.03.28
CEventloopTimer::CEventloopTimer(){
    LOG_INIT_V2(gControllerLogName);
    ////LOG_TRACE << "CEventloopTimer Constructor" << LOG_ENDL;
    m_loop = NULL;
    m_timer = NULL;
}

CEventloopTimer::CEventloopTimer(QEventLoop *loop,int timeout){
    LOG_INIT_V2(gControllerLogName);
#ifdef DBUGLOGS
    //LOG_TRACE << "CEventloopTimer Constructor" << LOG_ENDL;
        // Added by jonghwan.cho@lge.com 2013.05.21. for static test 20862
        m_loop = NULL;
#endif
        m_loop = loop;
      //  connect(this,SIGNAL(eventloopTimeout()),&loop,SLOT(quit()));
        m_timer = new QTimer(this);
        m_timer->setInterval(timeout);
        m_timer->setSingleShot(true);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(wakeUpAndEmit()));
        m_timer->start();
}

CEventloopTimer::~CEventloopTimer()
{
    m_loop = NULL;  // m_loop is only pointer refernce
    if(m_timer)
    {
        disconnect(m_timer, SIGNAL(timeout()), this, SLOT(wakeUpAndEmit()));
        if(m_timer->isActive() == true)
        {
            m_timer->stop();
        }
        delete m_timer;
        m_timer = NULL;
    }
}

void CEventloopTimer::wakeUpAndEmit()
{
    if(m_loop == NULL)
        return;

    //LOG_TRACE << "Woke up after timeout!!!" << LOG_ENDL;
        if(m_loop){
            //LOG_TRACE << "IPOD_ERROR_TIMEOUT" << LOG_ENDL;
            m_loop->exit(IPOD_ERROR_TIMEOUT);
        }
       // emit eventloopTimeout();
}

void CEventloopTimer::clear()
{
    if(m_timer && m_timer->isActive() == true)
    {
        m_timer->stop();
    }
}

void CEventloopTimer::start(QEventLoop *loop,int timeout)    // added by sungha.choi 2013.04.12
{
    m_loop = loop;
  //  connect(this,SIGNAL(eventloopTimeout()),&loop,SLOT(quit()));
    m_timer = new QTimer(this);
    m_timer->setInterval(timeout);    // seconds
    m_timer->setSingleShot(true);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(wakeUpAndEmit()));
    m_timer->start();
}

void CEventloopTimer::stop()
{
    m_loop = NULL;  // m_loop is only pointer refernce
    if(m_timer)
    {
        disconnect(m_timer, SIGNAL(timeout()), this, SLOT(wakeUpAndEmit()));
        if(m_timer->isActive() == true)
        {
            m_timer->stop();
        }
        delete m_timer;
        m_timer = NULL;
    }
}

void CEventloopTimer::getListComplete(QVariantMap contents,int length)
{
        //LOG_INFO << "Number of items in the list is " << length << LOG_ENDL;
        QMap<QString,QVariant>::iterator i = contents.begin();
                       while(i != contents.end())
                       {
                     //      DBGLOG("key=%d, valsue=%d\n",i.key(),i.value());
                                                 i++;
                       }
        //emit eventloopTimeout();
    if(m_loop){
        m_loop->exit(IPOD_ERROR_TIMEOUT);
    }
}
// } Added by sungha.choi 2013.03.28

#if 0  // deleted by sungha.choi 2013.03.28
void CEventloopTimer::wakeUpAndEmit()
{
    LOG_INIT_V2(gControllerLogName);
    //LOG_TRACE << "Woke up after timeout!!!" << LOG_ENDL;
        if(m_loop){
                ////LOG_TRACE << "IPOD_ERROR_TIMEOUT" << LOG_ENDL;
            m_loop->exit(IPOD_ERROR_TIMEOUT);
        }
       // emit eventloopTimeout();
}

void CEventloopTimer::getListComplete(QVariantMap contents,int length)
{
        //LOG_INFO << "Number of items in the list is " << length << LOG_ENDL;
        QMap<QString,QVariant>::iterator i = contents.begin();
                       while(i != contents.end())
                       {
                     //      DBGLOG("key=%d, valsue=%d\n",i.key(),i.value());
                                                 i++;
                       }
        //emit eventloopTimeout();
    if(m_loop){
        m_loop->exit(IPOD_ERROR_TIMEOUT);
    }
}
#endif
