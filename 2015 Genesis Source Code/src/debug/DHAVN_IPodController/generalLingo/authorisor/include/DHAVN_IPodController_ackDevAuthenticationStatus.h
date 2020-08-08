/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  BaseMediaPlay                                    *
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
#ifndef ACKDEVAUTHENTICATIONSTATUS_H
#define ACKDEVAUTHENTICATIONSTATUS_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace GeneralLingo
{

class CAckDevAuthenticationStatus: public CIAPPacket
{
//private:
//    USE_LOG_UTIL
public:
    CAckDevAuthenticationStatus(int transIdArgs, bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
    int getAuthenticationStatus();
private:
    qint16 m_TransId;
    int m_AuthenticationStatus;
};

}
#endif // ACKDEVAUTHENTICATIONSTATUS_H
