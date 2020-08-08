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
#ifndef GETPLAYSTATUS_H
#define GETPLAYSTATUS_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{

class CGetPlayStatus: public CIAPPacket
{
public:
    CGetPlayStatus(int transId, bool isTidEnabled);
    QByteArray marshall()        ;
    void  unmarshall(QByteArray)        ;
};

}

#endif //GETPLAYSTATUS_H
