#ifndef MESSAGESERIALPORT_H
#define MESSAGESERIALPORT_H

class MessageSerialPort
{
public:
    virtual int     Open() = 0;
    virtual int     Close() = 0;
    
    virtual int     SendMessage(const void *pData, int nLen) = 0;
    virtual int     RecvMessage(void *pData, int nMaxLen) = 0;
    virtual int     SendNRecvMessage(const void *pSendBuf, int nLen, void *pRecvBuf, int nMaxLen) = 0;
};

#endif // MESSAGESERIALPORT_H
