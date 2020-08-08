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
#ifndef RETPBTRACKINFO_H
#define RETPBTRACKINFO_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_retDBTrackInfo.h>

namespace ExtendedLingo
{
    class CRetPBTrackInfo : public CIAPPacket
    {
        public:
        //{added by junam 2013.04.13 for AutoStart
        enum
        {
            CAPABILITIES = 0,
            TRACK_NAME = 1,
            ARTIST_NAME = 2,
            ALBUM_NAME = 3,
            GENRE_NAME = 4,
            COMPOSER_NAME = 5,
            TOTAL_TRACK_DURATION = 6,
            ITUNES_UNIQUE_TRACK_ID = 7
        };
        //}added by junam
           CRetPBTrackInfo(bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           QString getInfoData();
           int getInfoType();
           int getTrackIndex();
           bool hasChapter();
           CapabityType getCaps();
           int getAlbumTrackIndex(){return m_AlbumTrackIndex;}
           int getChaptorCount(){return m_ChaptorCount;}
	   int getTrackDuration();
           quint64 getUID();
        private:
           QString m_Data;
           int m_InfoType;
           int m_TrackIndex;
           qint32 m_Capability;
           qint16 m_ChaptorCount;
           qint16 m_AlbumTrackIndex;
	   int m_TrackDuration;
           quint64 m_UID;
    };

} // end of namespace ExtendedLingo

#endif // RETDBTRACKINFO_H
