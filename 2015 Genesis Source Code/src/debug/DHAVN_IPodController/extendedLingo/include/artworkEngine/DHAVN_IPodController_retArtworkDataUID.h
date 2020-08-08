
/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  BaseMediaPlay                                    *
 *
 * PROGRAMMERS : Pawan Gupta                                    *
 * DATE       :  18 May 2012                                   *
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
#ifndef DHAVN_IPODCONTROLLER_RETARTWORKDATAUID_H
#define DHAVN_IPODCONTROLLER_RETARTWORKDATAUID_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

namespace ExtendedLingo
{
    class CRetArtworkDataUID : public CIAPPacket
    {
#ifdef DBUGLOGS
    private:
        USE_LOG_UTIL
#endif
        public:
           CRetArtworkDataUID(bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           QByteArray getArtwork();

            int getRowSize();
            int getArtworkWidth();
            int getArtworkHeight();
            int getPacketIndex();
            int getTotalPacketIndex();
            int getArtworkFormat();

        private:
            QByteArray m_Artwork;
            int m_ArtworkFormat;
            int m_ArtworkRowSize;
            int m_PacketIndex;
            int m_TotalPacketIndex;
            int m_ArtworkWidth;
            int m_ArtworkHeight;
            int m_RowSize;
            int m_TopLeftX;
            int m_TopLeftY;
            int m_BottomRightX;
            int m_BottomRightY;
    };
} // end of namespace ExtendedLingo

#endif // DHAVN_IPODCONTROLLER_RETARTWORKDATAUID_H
