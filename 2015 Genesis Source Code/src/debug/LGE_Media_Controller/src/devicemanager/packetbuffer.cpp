#include "packetbuffer.h"
#include "util.h"

PacketBuffer *PacketBuffer::m_pInstance = 0;

PacketBuffer::~PacketBuffer()
{
    MC_LOG << "~PacketBuffer()" << LOG_ENDL;
}

PacketBuffer* PacketBuffer::GetInstance()
{
    if(m_pInstance == 0) {
        m_pInstance = new PacketBuffer();
    }

    return m_pInstance;    
}

void PacketBuffer::DeleteInstance()
{
    if(m_pInstance) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

int PacketBuffer::AppendPacket(const StatusData &tPacket)
{
    // { modified by wonseok.heo for ITS 217637 2014.01.21
    m_tMutex.lock();
    m_tPacketList.append(tPacket);
    m_tMutex.unlock();
    emit NewPacketArrived();
    
    m_tMutex.lock();
    int nCount = m_tPacketList.count();
    m_tMutex.unlock();
    return nCount;
    // } modified by wonseok.heo for ITS 217637 2014.01.21
}

int PacketBuffer::PrependPacket(const StatusData &tPacket)
{
    // { modified by wonseok.heo for ITS 217637 2014.01.21
    m_tMutex.lock();
    m_tPacketList.prepend(tPacket);
    m_tMutex.unlock();
    emit SyncPacketArrived();
    
    m_tMutex.lock();
    int nCount = m_tPacketList.count();
    m_tMutex.unlock();
    return nCount;
    // } modified by wonseok.heo for ITS 217637 2014.01.21
}

bool PacketBuffer::TakePacket(StatusData *pPacket)
{
    // { modified by wonseok.heo for ITS 217637 2014.01.21
    m_tMutex.lock();
    bool bEmpty = m_tPacketList.isEmpty();
    m_tMutex.unlock();
    if(bEmpty == true) {
        MC_HIGH << "m_tPacketList is empty..." << LOG_ENDL;
        return false;
    }
    
    m_tMutex.lock();
    StatusData tData = m_tPacketList.takeFirst();
    m_tMutex.unlock();
    // } modified by wonseok.heo for ITS 217637 2014.01.21
    memcpy(pPacket, &tData, sizeof(StatusData));
    
    return true;
}

bool PacketBuffer::IsEmpty()
{
    return m_tPacketList.isEmpty();
}

int PacketBuffer::GetCount()
{
    return m_tPacketList.count();    
}

bool PacketBuffer::RequestSyncStatus(int nStatusCode)
{
    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_nSyncStatusCode = nStatusCode;
        m_nWorking        = 1;
        m_tMutex.unlock();
        return true;
    }
    else {
        m_tMutex.unlock();
        return false;
    }
}

bool PacketBuffer::StartWait()
{
    m_tMutex.lock();
    while(m_nWorking == 1) {
        MC_LOG << "Start PacketBuffer::StartWait()" << LOG_ENDL;
        m_tCond.wait(&m_tMutex);
    }
    m_tMutex.unlock();
    
    MC_LOG << "End PacketBuffer::StartWait()" << LOG_ENDL;
    
    return true;
}

bool PacketBuffer::EndWait(int nStatusCode)
{
    MC_LOG << "PacketBuffer::EndWait: " << nStatusCode << ", " << m_nSyncStatusCode << LOG_ENDL;
    if(nStatusCode == m_nSyncStatusCode) {
        m_tMutex.lock();
        m_nSyncStatusCode = -1;
        m_nWorking        = 0;
        m_tCond.wakeAll();
        m_tMutex.unlock();
    }
    
    return true;
}


PacketBuffer::PacketBuffer(QObject *parent) : QObject(parent),
m_nSyncStatusCode(-1),
m_nWorking(0)
{
}
