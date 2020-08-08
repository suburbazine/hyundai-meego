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
#ifndef ACKDEVAUTHENTICATIONINFO_H
#define ACKDEVAUTHENTICATIONINFO_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace GeneralLingo
{

class CAckDevAuthenticationInfo: public CIAPPacket
{
//private:
//    USE_LOG_UTIL
public:
    CAckDevAuthenticationInfo(int transIdArgs, bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
    int getAuthenticationInfoStatus();
private:
    qint16 m_TransId;
    int m_AuthenticationStatus;
};

}
#endif // ACKDEVAUTHENTICATIONINFO_H
