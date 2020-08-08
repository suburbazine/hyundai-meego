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
#ifndef GETDEVAUTHENTICATIONSIGNATURE_H
#define GETDEVAUTHENTICATIONSIGNATURE_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace GeneralLingo
{

class CGetDevAuthenticationSignature: public CIAPPacket
{
//private:
//    USE_LOG_UTIL
public:
    CGetDevAuthenticationSignature(int transIdArgs,  bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
    int getAuthenticationRetryCounter();
    QByteArray getOfferedChallenge();
private:
    qint16 m_TransId;
    int m_AuthenticationRetryCounter;
    QByteArray m_OfferedChallenge;
};

}

#endif // GETDEVAUTHENTICATIONSIGNATURE_H
