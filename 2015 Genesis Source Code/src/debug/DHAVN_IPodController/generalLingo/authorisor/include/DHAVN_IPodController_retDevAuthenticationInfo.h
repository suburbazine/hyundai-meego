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
#ifndef RETDEVAUTHENTICATIONINFO_H
#define RETDEVAUTHENTICATIONINFO_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace GeneralLingo
{

class CRetDevAuthenticationInfo: public CIAPPacket
{
//private:
//    USE_LOG_UTIL
public:
    CRetDevAuthenticationInfo(int transIdArgs, QByteArray certificate, int cerLength, int count,  bool isTidEnabled, int maxPacketSize);
    QByteArray marshall();
    void  unmarshall(QByteArray);

    int getTotalSections();
    int getCurrentSection();
private:
    QByteArray getCerData();


private:
    qint16 m_TransId;
    QByteArray m_Certificate;
    int m_CerLength;
    int m_Count;
    int m_maxPacketSize;    
};

}
#endif // RETDEVAUTHENTICATIONINFO_H
