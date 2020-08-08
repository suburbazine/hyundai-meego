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
#ifndef GETPBTRACKINFO_H
#define GETPBTRACKINFO_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{
    class CGetPBTrackInfo : public CIAPPacket
    {
        public:
        //{added by junam 2013.04.13 for AutoStart
        enum
        {
            CAPABILITIES = 1,
            TRACK_NAME = 1<<1,
            ARTIST_NAME = 1<<2,
            ALBUM_NAME = 1<<3,
            GENRE_NAME = 1<<4,
            COMPOSER_NAME = 1<<5,
            TOTAL_TRACK_DURATION = 1<<6,
            ITUNES_UNIQUE_TRACK_ID = 1<<7,

            RETURNCOMMANDID = 0x43
        };
        //}added by junam
           CGetPBTrackInfo(int startIndex,
                                   int count,
                                   int transId,bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           void setMask(qint8 mask){m_Mask=mask;}//added by junam 2013.04.13 for AutoStart
        private:
            QString m_Category;
            int m_StartIndex;
            int m_TrackCount;
            qint8 m_Mask;//added by junam 2013.04.13 for AutoStart
    };
} // end of namespace ExtendedLingo

#endif // GETDBTRACKINFO_H
