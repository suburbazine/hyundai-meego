#ifndef DABCOMMRECEIVERTHREAD_H
#define DABCOMMRECEIVERTHREAD_H

#include <QThread>
#include <QMutex>
#include "DABCommUART.h"

extern unsigned short   calculateCRC16(const unsigned char* pIData, unsigned int  IDataLen);

class DABCommReceiverThread : public QThread
{
    Q_OBJECT
public:
    explicit DABCommReceiverThread(DABCommUART *comUart, QObject *parent = 0);
    ~DABCommReceiverThread();

    void stop();

signals:
    void receivedCommand(unsigned char type, unsigned char command, QByteArray dataBuf, int bufLen);

protected:
    void run();

private:
    QMutex mutex;
    volatile bool stopped;
    DABCommUART *DABModule;
    static const unsigned int ReadBufferSize = 20480; //1024; //Max Frame size is 2014Byte (C&S Tech IF Spec 0.5)
    static const unsigned int DataBufferSize = 40960;
    static const int SleepTime = 10000;

    enum ParsingState {
        ProcessStartByte = 1,
        ProcessContinuityIndexFirstLastDevice,
        ProcessDataLengthHigh,
        ProcessDataLengthLow,
        ProcessData,
        ProcessCheckCRCHigh,
        ProcessCheckCRCLow,
        ProcessFrameSuccess
    };
    enum ParsingPacketState {
        ProcessPacketType = ProcessFrameSuccess+1,
        ProcessPacketCommand,
        ProcessPacketLength,
        ProcessPacketData,
        ProcessPacketSuccess
    };

    quint16 dataLen;
    unsigned int dataBufOffset;
    ParsingState state;
    ParsingPacketState packetState;

    unsigned char continuity;
    unsigned char frameposition;
    unsigned char device;

    unsigned char byte, type;
    unsigned int command;
    unsigned int checksum;
    unsigned int packetLength;
    quint8 readBuffer[ReadBufferSize];
    quint8 dataBuffer[DataBufferSize];

    quint16 totalLength;
    quint8 commandValue;
    quint8* packetBuffer;
    int packetLastIndex;

    bool parsingCommand(unsigned char *readBuffer, int bufLen);
    bool parsingPacket(unsigned char *buffer, int bufLen);
    bool parsingPacketSeries(unsigned char *buffer, int bufLen);

    void setDevice(unsigned char conti){this->device = conti;}
    void setFrameposition(unsigned char framepos){this->frameposition = framepos;}
    void setTotalLength(quint16 totalLen){this->totalLength = totalLen;}
    void setCommandValue(quint8 value){this->commandValue = value;}
    void setPacketBuffer(quint8* pPacketBuffer){this->packetBuffer = pPacketBuffer;}
    void setPacketLastIndex(int index){this->packetLastIndex = index;}
    unsigned char getDevice(){return this->device;}
    unsigned char getFrameposition(){return this->frameposition;}
    quint16 getTotalLength(){return this->totalLength;}
    quint8 getCommandValue(){return this->commandValue;}
    quint8* getPacketBuffer(){return this->packetBuffer;}
    int getPacketLastIndex(){return this->packetLastIndex;}
    static void debugPrintOutContinuity(unsigned char oldConti, unsigned char newConti);
    static void debugPrintOutFramePosition(unsigned char pos);
    static void debugPrintOutDevice(unsigned char device);
};

#endif // DABCOMMRECEIVERTHREAD_H
