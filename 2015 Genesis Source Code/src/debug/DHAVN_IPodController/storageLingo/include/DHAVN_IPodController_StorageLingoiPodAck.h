/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS :  Sowmini / Venkat                    *
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
#ifndef IPODACKSTORAGE_H
#define IPODACKSTORAGE_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace StorageLingo
{
class CIPodStorageLingoAck : public CIAPPacket
{
public:
    CIPodStorageLingoAck( bool isTidEnabled) {  m_isTidEnabled = isTidEnabled;
                                                m_AckCommand = -1;
                                                m_AckStatus =-1;
                                                m_TransID = 0;} //uninitialized variables - coverity fix
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
}

#endif // CIPODACKSTORAGE_H
