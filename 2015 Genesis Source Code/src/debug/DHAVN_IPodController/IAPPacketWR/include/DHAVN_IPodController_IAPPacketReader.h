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
 *                   This is the IAP packet reader class          *
 *                   it reads the packet coming from the          *
 *                   raw packet handler and creates the           *
 *                   full IAP packet and emit the packet          *
 *                   received signal                              *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef IAPPACKETREADER_H
#define IAPPACKETREADER_H

#include <QObject>
#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_hidRawPacketReader.h>
#include <DHAVN_IPodController_Def.h>
#include <DHAVN_LogUtil.h>
#include <DHAVN_IPodController_PrivateData.h>

class CIAPPacketReader : public QObject
{
    Q_OBJECT
private:
    USE_LOG_UTIL
public:
    CIAPPacketReader(EIPodPorts nCurrentPortNum,int vendorId, int productId, QString hidDevPath);
    ~CIAPPacketReader();
    void setDevice(int vendorId, int productId, QString hidDevPath);

signals:
    void packetAvailable(int lingoId, int commandId, QByteArray iAPPacketData);
    void largePacketAvailable(int lingoId, int commandId, QByteArray iAPPacketData);

    /* Packet Filter Signal */
    void packetAvailableForPlayerTaskHandler(int lingoId, int commandId, QByteArray iAPPacketData);
    void packetAvailableForDBTaskHandler(int lingoId, int commandId, QByteArray iAPPacketData);
    void iAPReaderStatus(bool status);
    void requestQuit();    // added by sungha.choi 2013.04.18

public slots:
    void iAPPacketAvalable(QByteArray iAPPacket, bool isLargePacket);
    void onDeviceDisconnect();
    void closeFileDescriptorSlot();

public:
    EIPodPorts m_nCurrentPortNum;
private:
    CHIDRawPacketReader* m_hidReader;

    int m_vendorId;
    int m_productID;
    QString m_hidDevPath;
    bool m_isDisconnected;    // added by sungha.choi 2013.04.12

};

#endif //IAPPACKETREADER_H
