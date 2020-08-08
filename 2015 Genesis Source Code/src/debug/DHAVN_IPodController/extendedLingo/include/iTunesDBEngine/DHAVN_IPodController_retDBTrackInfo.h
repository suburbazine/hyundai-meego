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
#ifndef RETDBTRACKINFO_H
#define RETDBTRACKINFO_H

#include <DHAVN_IPodController_iAPPacket.h>

enum CapabityType
{
    isTrack = 0,
    isAudioBook,
    isPodcast,
    isITuneEpisode
};

namespace ExtendedLingo
{
    class CRetDBTrackInfo : public CIAPPacket
    {
        public:
           CRetDBTrackInfo(bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           QString getInfoData();
           int getInfoType();
           int getTrackIndex();
           CapabityType getCaps();
           int getAlbumTrackIndex(){return m_AlbumTrackIndex;}
           int getChaptorCount(){return m_ChaptorCount;}
           quint64 getUID() {return m_UID;}
		   int getTrackDuration();
        private:
           QString m_Data;
           int m_InfoType;
           int m_TrackIndex;
           qint32 m_Capability;
           qint16 m_ChaptorCount;
           qint16 m_AlbumTrackIndex;
           quint64 m_UID;
		   int m_TrackDuration;
    };

} // end of namespace ExtendedLingo

#endif // RETDBTRACKINFO_H
