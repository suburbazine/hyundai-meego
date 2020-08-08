/*
 ******************************************************************
 *        COPYRIGHT © Teleca AB                                   *
 *----------------------------------------------------------------*
 * MODULE     :  IPodAppController                                *
 *
 * PROGRAMMERS : Pawan Gupta                                      *
 * DATE       :  16 jan 2012                                      *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :

 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef IAPPACKETWRITER_H
#define IAPPACKETWRITER_H

#include <QObject>
#include <DHAVN_IPodAppController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

static const int reports[][2] = {
{ 0x0d, 0x05 },
{ 0x0e, 0x09 },
{ 0x0f, 0x0d },
{ 0x10, 0x11 },
{ 0x11, 0x19 },
{ 0x12, 0x31 },
{ 0x13, 0x5f },
{ 0x14, 0xc1 },
{ 0x15, 0xff },
{0,0}
};



class CIAPPacketWriter : public QObject
{
    USE_LOG_UTIL
    Q_OBJECT
public:
    CIAPPacketWriter(QObject* parent);
    int SendPacket(CIAPPacket& packetData);
private:
    int DispachIAPPacket(CIAPPacket& packetData);
   int lookupReport(int size, int& size_out);


};

#endif //CIAPPACKETWRITER_H
