#ifndef DABCOMMDATAHANDLER_H
#define DABCOMMDATAHANDLER_H

#include <QThread>
#include <QString>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QFile>
#include <QTextStream>
#include <QTime>
#include "DABCommSPI.h"
#include "DABCommSPIPacketInfo.h"

//class DABCommManager;

#pragma pack(1)

typedef struct DAB_SPI_DATA
{
/*
    quint8 startOfSymbol;
    quint8 lengthHigh;
    quint8 lengthLow;
    quint8 index;
    quint8 data[2040];
    quint32 checksum;
*/
    quint8 startOfSymbol;
    quint8 continuityIndexFirstLastDevice;
    quint8 DataLengthHigh;
    quint8 DataLengthLow;
    quint8 data[2042];
    quint16 frameCRC;
} tDAB_SPI_DATA;

#pragma pack()

extern unsigned short   calculateCRC16(const unsigned char* pIData, unsigned int  IDataLen);

class DABCommDataHandler : public QThread
{
    Q_OBJECT

public:
    explicit DABCommDataHandler(DABCommSPI *comSpi, QObject *parent = 0);
    ~DABCommDataHandler();
    void stop();
    void setCommDataHandler(DABCommSPIPacketInfo* comPacketInfo);
    static const quint8 SPI_START_SYMBOL = 0xA5;
    static const quint32 SPI_PACKET_SIZE = 2048;
    static const quint32 SPI_DATA_LENGTH = 2042;
    static const quint32 ACC_COUNT = 9;

signals:
    void receivedCommand(unsigned char type, unsigned char command, unsigned char framepostion, QByteArray dataBuf, int bufLen, int totalLen);
protected:
        void run();
private:
        volatile bool stopped;
        QMutex mutex;
        bool checkDataPacket(const quint8 *buffer, const int bufLen);

//        void debugPrint(tDAB_SPI_DATA *dataPacket, quint32 checksum);
        void PrepareLogFile(QObject *pParent);
        void parsingDataPacket(quint8 *buffer, int bufLe, unsigned char frameposition);
        static void debugPrintOutContinuity(unsigned char oldConti, unsigned char newConti);
        static void debugPrintOutFramePosition(unsigned char pos);
        static void debugPrintOutDevice(unsigned char device);

        DABCommSPI *m_spiDev;

        DABCommSPIPacketInfo* pCommSPIPacketInfo;

        qint32 m_dabIntDev;

        // For debugging
        //QFile* m_pAppEngineLogFile;   //# 20130327 prevent
        //QFile *m_saveFile;            //# 20130327 prevent
        //QDataStream *m_out;           //# 20130327 prevent
        QTextStream m_tsLog;

        unsigned char continuity;
        unsigned char frameposition;
        unsigned char device;

public slots:

};

#endif // DABCOMMDATAHANDLER_H
