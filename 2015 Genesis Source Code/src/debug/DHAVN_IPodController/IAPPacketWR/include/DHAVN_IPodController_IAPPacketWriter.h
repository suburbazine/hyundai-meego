/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                   This is the IAP packet writer class          *
 *                   it writes the packet coming from the         *
 *                   Task handlers handler and creates the        *
 *                   full HID packet and writes them to           *
 *                   raw packet handler                           *                                             *
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
#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_hidRawPacketWriter.h>
#include <DHAVN_IPodController_CommonVars.h>
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
{ 0x00, 0x00 }
};





class CIAPPacketWriter : public QObject
{
    Q_OBJECT
private:
    USE_LOG_UTIL
public:
    CIAPPacketWriter(EIPodPorts nCurrentPortNum, int vendorId, int productId, QString hidDevPath);
    ~CIAPPacketWriter();
    void setDevice(int vendorId, int productId, QString hidDevPath);
    int getProductID();
    int UsbAudioClassSetInterface(int interface_number, int alternate_setting);
    int HIDVendorSpecificRequest(unsigned short request, unsigned short value, short index);

signals:
    void iAPWriterStatus(bool status);

public slots:
    void onDeviceDisconnect();

    void sendiAPPacket(QByteArray packetBytes);
    void onAuthorised(bool status);

    void closeFileDescriptorSlot();

private:
    int DispachIAPPacket(CIAPPacket& packetData);
    int lookupReport(int size, int& size_out);
    void setMaxReportSize();
public:
    EIPodPorts m_nCurrentPortNum;
private:
   CHIDRawPacketWriter* m_hidWriter;
   int m_vendorId;
   int m_productID;
   QString m_hidDevPath;
   int m_maxReportSize;
   bool m_isDisconnected;    // added by sungha.choi 2013.04.12
};

#endif //CIAPPACKETWRITER_H
