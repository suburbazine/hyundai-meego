/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                                                                *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef RETFIDTOKENVALUE_H
#define RETFIDTOKENVALUE_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace GeneralLingo
{

class CRetFIDTokenValue: public CIAPPacket
{
    USE_LOG_UTIL
public:
    CRetFIDTokenValue(int transIdArgs,  bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
private:
    void printTheStatusOfAck(QByteArray returnCommand, int startOfNextTokenAck, int LenOfToken);
    int m_TransId;
public:
    bool m_AccCapsStatus;
};

}

#endif //RETFIDTOKENVALUE_H
