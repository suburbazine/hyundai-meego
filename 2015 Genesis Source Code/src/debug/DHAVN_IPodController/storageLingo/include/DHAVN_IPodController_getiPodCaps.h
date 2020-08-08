/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Sowmini / Venkat                    *
 * DATE       :  02 January 2012                                   *
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
#ifndef IPODCAPS_H
#define IPODCAPS_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace StorageLingo
{

class CGetiPodCaps: public CIAPPacket
{
public:
    CGetiPodCaps(int transId, bool isTidEnabled);
    QByteArray marshall()        ;
    void  unmarshall(QByteArray)        ;
};

}

#endif
