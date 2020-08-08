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
#ifndef RESETDBSELECTION_H
#define RESETDBSELECTION_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace ExtendedLingo
{

class CResetDBSelection: public CIAPPacket
{
#ifdef DBUGLOGS
private:
    USE_LOG_UTIL
#endif
public:
    CResetDBSelection(int transId,bool isTidEnabled);
    QByteArray marshall()        ;
    void  unmarshall(QByteArray)        ;
};

}

#endif
