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
#ifndef RETTRACKARTWORKDATA_H
#define RETTRACKARTWORKDATA_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{
    class CRetTrackArtworkData : public CIAPPacket
    {
        public:
           CRetTrackArtworkData(bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           QByteArray getArtwork();
            int getRowSize();
            int getArtworkWidth();
            int getArtworkHeight();
            int getPacketIndex();
            int  getArtworkFormat();
//            int getTopLeftX();
//            int getTopLeftY();
//            int getBottomRightX();
//            int getBottomRightY();
        private:
            QByteArray m_Artwork;
            int m_ArtworkFormat;
            int m_ArtworkRowSize;
            int m_PacketIndex;
            int m_ArtworkWidth;
            int m_ArtworkHeight;
            int m_RowSize;
            int m_TopLeftX;
            int m_TopLeftY;
            int m_BottomRightX;
            int m_BottomRightY;
    };
} // end of namespace ExtendedLingo

#endif // RETTRACKARTWORKDATA_H
