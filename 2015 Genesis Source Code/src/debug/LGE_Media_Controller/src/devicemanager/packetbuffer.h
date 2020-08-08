#ifndef PACKETBUFFER_H
#define PACKETBUFFER_H

#include <QObject>
#include <QWaitCondition>
#include <QMutex>
#include "packet.h"

class PacketBuffer : public QObject
{
    Q_OBJECT
public:
    ~PacketBuffer();

    static PacketBuffer* GetInstance();
    static void          DeleteInstance();

    int     AppendPacket(const StatusData &tPacket);
    int     PrependPacket(const StatusData &tPacket);
    bool    TakePacket(StatusData *pPacket);
    bool    IsEmpty();
    int     GetCount();

    bool    RequestSyncStatus(int nStatusCode);
    bool    StartWait();
    bool    EndWait(int nStatusCode);

signals:
    void    NewPacketArrived();
    void    SyncPacketArrived();

private:
    PacketBuffer(QObject *parent = 0);

    static PacketBuffer     *m_pInstance;

    QList<StatusData>        m_tPacketList;
    int                      m_nSyncStatusCode;
    int                      m_nWorking;
    QWaitCondition           m_tCond;
    QMutex                   m_tMutex;
};

#endif // PACKETBUFFER_H
