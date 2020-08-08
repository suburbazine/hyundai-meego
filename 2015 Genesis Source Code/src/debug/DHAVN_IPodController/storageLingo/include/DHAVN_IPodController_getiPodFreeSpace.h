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
#ifndef getiPodFreeSpace_H
#define getiPodFreeSpace_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace StorageLingo
{

class CGetiPodFreeSpace: public CIAPPacket
{
public:
    CGetiPodFreeSpace(int transId, bool isTidEnabled);
    QByteArray marshall()        ;
    void  unmarshall(QByteArray)        ;
};
}

#endif
