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
#ifndef DHAVN_IPODAPPCONTROLLER_ACK_H
#define DHAVN_IPODAPPCONTROLLER_ACK_H

#include <DHAVN_IPodAppController_iAPPacket.h>

class CACK : public CIAPPacket
{
public:
    CACK():m_AckCommand(0),m_AckStatus(-1), m_TransID(0) {}
    QByteArray marshall();
     void  unmarshall(const QByteArray&);
     int getAcknowledgedCommand();
     int getAckStatus() {return m_AckStatus;}
private:
        int m_AckCommand;
        int m_AckStatus;
        int m_TransID;
};
#endif // DHAVN_IPODAPPCONTROLLER_ACK_H
