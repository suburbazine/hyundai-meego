#include "tab2driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#include "util.h"

Tab2Driver::Tab2Driver(QObject *parent) : QThread(parent),
m_nDeckFD(-1)
{
    m_nStatusCode   = -1;
    m_nWorking      = 0;

    m_tPacketList.reserve(10);
}

Tab2Driver::~Tab2Driver()
{
    MC_LOG << "~Tab2Driver()" << LOG_ENDL;
    Close();
}

int Tab2Driver::Open()
{
    #if USE_SYS_CLASS_TAB2_ASYNC
    MC_HIGH << "Using async" << LOG_ENDL;
    m_nDeckFD = open(TAB_DEVICE_PATH, O_RDWR);
    #else
    MC_HIGH << "Using sync" << LOG_ENDL;
    m_nDeckFD = open(TAB_DEVICE_PATH, O_RDWR|O_NONBLOCK);
    #endif
    if(m_nDeckFD < 0) {
        MC_HIGH << "Packet: failed to open " << TAB_DEVICE_PATH << LOG_ENDL;
        return -1;
    }
    
    start();

    return 0;
}

int Tab2Driver::Close()
{
    quit();
    
    if(m_nDeckFD >= 0)      
        close(m_nDeckFD);
    
    m_nDeckFD = -1;
    
    return 0;
}

int Tab2Driver::Write(const void *pData, int nLen, int bSync, int nStatusCode)
{
    Q_UNUSED(nStatusCode);

    int nWrite;
    usleep(35000); // added by cychoi 2014-12-12 ITS 254039 TAB2 communication interval (write: 35ms delay, read: 35ms delay)
    nWrite = write(m_nDeckFD, pData, nLen);
    if(nWrite <= 0) {
        MC_HIGH << "Tab2Driver::Write() failed. " << strerror(errno) << "(" << errno << ")" << LOG_ENDL;
        usleep(35000); // added by cychoi 2014-12-12 ITS 254039 TAB2 communication interval (write: 35ms delay, read: 35ms delay)
        // { added by cychoi 2013-06-05 Please retry one more if failed write.
        nWrite = write(m_nDeckFD, pData, nLen);
        if(nWrite <= 0) {
            MC_HIGH << "Tab2Driver::Write() failed twice. " << strerror(errno) << "(" << errno << ")" << LOG_ENDL;
        }
        else {
            // MC_LOG << "Tab2Driver::Write() twice : " << nWrite << LOG_ENDL;
        }
        // } added by cychoi 2013-06-05
    }
    else {
        // MC_LOG << "Tab2Driver::Write() : " << nWrite << LOG_ENDL;
    }

    if(bSync == true) {
        #if 0
        m_tAppMutex.lock();
        MC_LOG << "waiting Tab2Driver::Write(" << nStatusCode << ")" << LOG_ENDL;
        m_nStatusCode = nStatusCode;
        m_tAppCond.wait(&m_tAppMutex);
        MC_LOG << "end Tab2Driver::Write" << LOG_ENDL;
        m_tAppMutex.unlock();
        #endif
    }
    
    return nWrite;
}

int Tab2Driver::Read(void *pData, int nMaxLen, int bSync)
{
    int nReads = 0;
    
#if USE_SYS_CLASS_TAB2_ASYNC
    Q_UNUSED(nMaxLen);
    Q_UNUSED(bSync);
    // { modified by wonseok.heo for ITS 217637 2014.01.21
    m_tMutex.lock();
    int nCount = m_tPacketList.count();
    m_tMutex.unlock();
    //MC_LOG << "Tab2Driver::Read: m_tPacketList.count(): " << nCount << LOG_ENDL;
    if(nCount > 0) {
        m_tMutex.lock();
        Tab2RawPacket tPacket = m_tPacketList.takeFirst();
        m_tMutex.unlock();
    // } modified by wonseok.heo for ITS 217637 2014.01.21
        if(tPacket.m_nSize > 0) {
            memcpy(pData, (void*)tPacket.m_pData, tPacket.m_nSize);
            nReads = tPacket.m_nSize;
        }
        //MC_LOG << "Tab2Driver::Read " << nReads << " readed" << LOG_ENDL;
    }
#else
    nReads = read(m_nDeckFD, pData, nMaxLen);
    if(nReads >= 0) {
        //
    } else if(errno == EAGAIN) {
        // ok - no data
    } else {
        MC_HIGH << "Tab2Driver::Read(): Failed to read from deck: " << strerror(errno) << LOG_ENDL;
    }
#endif

    return nReads;
}

void Tab2Driver::StartWait()
{
    m_tMutex.lock();
    while(m_nWorking == 0) {
        m_tCond.wait(&m_tMutex);
    }
    m_tMutex.unlock();
}

void Tab2Driver::EndWait()
{
    m_tMutex.lock();
    m_nWorking = 0;
    m_tMutex.unlock();
}

void Tab2Driver::run()
{
    if(m_nDeckFD < 0) {
        MC_HIGH << "Tab2Driver::run(). m_nDeckFD is " << m_nDeckFD << LOG_ENDL;
        return;
    }

#if (USE_SYS_CLASS_TAB2_ASYNC == 0)
    fd_set          tReads, tTemps;
    int             nResult;
    struct timeval  tTimeout;

    FD_ZERO(&tReads);
    FD_SET(m_nDeckFD, &tReads);
#else
    Tab2RawPacket   tPacket;
#endif
    
    while(1)
    {
    #if USE_SYS_CLASS_TAB2_ASYNC
        if(m_nDeckFD < 0) break;
        
        //MC_HIGH << "Tab2Driver::run():wait read" << LOG_ENDL;
        tPacket.m_nSize = 0;
        usleep(35000); // added by cychoi 2014-12-12 ITS 254039 TAB2 communication interval (write: 35ms delay, read: 35ms delay)
        int nReads = read(m_nDeckFD, tPacket.m_pData, MAX_READ_BUFFER_SIZE);
        if(nReads > 0) {
            tPacket.m_nSize = nReads;
            // { modified by wonseok.heo for ITS 217637 2014.01.21
            m_tMutex.lock();
            m_tPacketList.append(tPacket);
            m_tMutex.unlock();
            // } modified by wonseok.heo for ITS 217637 2014.01.21
            //MC_HIGH << "Tab2Driver::run():end read " << nReads << LOG_ENDL;
            emit ReadReady(); 
        }
        else if(errno == EAGAIN) {
            // ok - no data
        } 
        else {
            MC_HIGH << "Tab2Driver::run(): Failed to read from deck: " << strerror(errno) << LOG_ENDL;
        }
    #else
        tTemps = tReads;
        
        tTimeout.tv_sec  = 0;
        tTimeout.tv_usec = 50000;

        nResult = select(m_nDeckFD + 1, &tTemps, 0, 0, &tTimeout);
        if(nResult == -1) // select error
        {
            MC_HIGH << "PacketReadThread::run() select failed: " << strerror(errno) << LOG_ENDL;
            break;
        }
        else if(nResult == 0) // timeout
        {
            //MC_LOG << "PacketReadThread::run() timeout" << LOG_ENDL;
            emit ReadReady();
        }
        else // 
        {
            if(FD_ISSET(m_nDeckFD, &tTemps)) {
                //MC_LOG << "PacketReadThread::run() ReadReady" << LOG_ENDL;
                emit ReadReady();
            }
        }
    #endif
    }
}

void Tab2Driver::HandlePolling()
{
    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_nWorking = 1;
        m_tCond.wakeAll();
    }
    m_tMutex.unlock();
}
