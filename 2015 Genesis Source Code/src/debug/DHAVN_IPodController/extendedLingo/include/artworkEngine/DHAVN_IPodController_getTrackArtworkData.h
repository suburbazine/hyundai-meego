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
#ifndef GETTRACKARTWORKDATA_H
#define GETTRACKARTWORKDATA_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

namespace ExtendedLingo
{
    class CGetTrackArtworkData : public CIAPPacket
    {
#ifdef DBUGLOGS
    private:
        USE_LOG_UTIL
#endif
    public:
       CGetTrackArtworkData(int trackIndex,
                                        int formatId,
                                        int timeOffset,
                                        int transId,bool isTidEnabled);
       QByteArray marshall();
        void  unmarshall(QByteArray);

    private:
        int m_TrackIndex;
        int m_FormatId;
        int m_TimeOffset;
    };
}
#endif // GETTRACKARTWORKDATA_H
