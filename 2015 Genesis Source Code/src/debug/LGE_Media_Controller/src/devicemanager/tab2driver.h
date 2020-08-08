#ifndef TAB2DRIVER_H
#define TAB2DRIVER_H

#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include "common.h"

class Tab2RawPacket
{
public:
    Tab2RawPacket() {
        m_nSize = 0;
        m_pData[0] = 0; // added by cychoi 2013-07-15 for Prevent fix
    }
    
    Tab2RawPacket(const Tab2RawPacket &rst) {
        m_nSize = rst.m_nSize;
        memcpy(m_pData, rst.m_pData, sizeof(unsigned char)*MAX_READ_BUFFER_SIZE);
    }
    
    int           m_nSize;
    unsigned char m_pData[MAX_READ_BUFFER_SIZE];
};

class Tab2Driver : public QThread
{
    Q_OBJECT
public:
    Tab2Driver(QObject *parent = 0);
    ~Tab2Driver();

    int         Open();
    int         Close();
    int         Write(const void *pData, int nLen, int bSync, int nStatusCode);
    int         Read(void *pData, int nMaxLen, int bSync);

    void        StartWait();
    void        EndWait();

protected:
    void        run();

signals:
    void        ReadReady();

public slots:
    void        HandlePolling();

private:
    int                  m_nDeckFD;

    int                  m_nStatusCode;
    int                  m_nWorking;
    QWaitCondition       m_tCond;
    QMutex               m_tMutex;

    QWaitCondition       m_tAppCond;
    QMutex               m_tAppMutex;

    QList<Tab2RawPacket> m_tPacketList;
};

#endif // TAB2DRIVER_H
