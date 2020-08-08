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
#ifndef GETDEVAUTHENTICATIONINFO_H
#define GETDEVAUTHENTICATIONINFO_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace GeneralLingo
{

class CGetDevAuthenticationInfo: public CIAPPacket
{
//private:
//    USE_LOG_UTIL
public:
    CGetDevAuthenticationInfo(int transIdArgs, bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
private:
    qint16 m_TransId;
};

}
#endif // GETDEVAUTHENTICATIONINFO_H
