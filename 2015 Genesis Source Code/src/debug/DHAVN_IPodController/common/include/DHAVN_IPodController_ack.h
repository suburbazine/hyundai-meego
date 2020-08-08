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
 *                    General and extended lingo command class to *
 *                    unmarsal the Ack command                    *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef ACK_H
#define ACK_H

#include <DHAVN_IPodController_iAPPacket.h>

class CACK : public CIAPPacket
{
public:
    CACK(bool isTidEnabled) :m_AckCommand(0),m_AckStatus(-1), m_TransID(0)
    {
        m_isTidEnabled = isTidEnabled;
    }
    QByteArray marshall()        ;
     void  unmarshall(QByteArray)        ;
     int getAcknowledgedCommand();
     int getAckStatus();
     int getTransactionID();
private:
        int m_AckCommand;
        int m_AckStatus;
        int m_TransID;
};

#endif // CACK_H
