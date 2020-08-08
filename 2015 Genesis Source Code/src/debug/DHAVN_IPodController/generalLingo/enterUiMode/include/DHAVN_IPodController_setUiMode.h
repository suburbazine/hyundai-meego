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
#ifndef DHAVN_IPODCONTROLLER_SETUIMODE_H
#define DHAVN_IPODCONTROLLER_SETUIMODE_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace GeneralLingo
{

class CSetUiMode: public CIAPPacket
{
public:
    CSetUiMode(int transIdArgs,  bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
};

}

#endif // DHAVN_IPODCONTROLLER_SETUIMODE_H
