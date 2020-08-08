/*
 ******************************************************************
 *        COPYRIGHT © Teleca AB      *
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
#ifndef GETCURRENTPLAYINGTRACKINDEX_H
#define GETCURRENTPLAYINGTRACKINDEX_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{

class CGetCurrentPlayingTrackIndex : public CIAPPacket
{
public:
    CGetCurrentPlayingTrackIndex(int transId, bool isTidEnabled);
    QByteArray marshall()        ;
     void  unmarshall(QByteArray)        ;
};

}
#endif // GETCURRENTPLAYINGTRACKINDEX_H
