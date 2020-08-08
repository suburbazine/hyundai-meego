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
#ifndef IAPPACKETREADER_H
#define IAPPACKETREADER_H

#include <QObject>
//#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>
class CIAPPacketReader : public QObject
{
    USE_LOG_UTIL
    Q_OBJECT
public:
    CIAPPacketReader(QObject* parent);

signals:
    void packetAvailable(int lingoId, int commandId, QByteArray& iAPPacketData);
    void largePacketAvailable(int lingoId, int commandId, QByteArray& iAPPacketData);

public slots:
    void hidReportAvalable(QByteArray hidPacket);

private:
    QByteArray constructiAPPacket(QByteArray& hidPacket);

private:
    QByteArray m_ReadArray;
    bool m_Emit;
    bool m_isLargePacket;

};

#endif //IAPPACKETREADER_H
