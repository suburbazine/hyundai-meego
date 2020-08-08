#ifndef DHAVN_RETURNINDEXEDPLAYINGTRACKINFO_H
#define DHAVN_RETURNINDEXEDPLAYINGTRACKINFO_H
#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{
    class CReturnIndexedPlayingTrackInfo : public CIAPPacket
    {

        public:


        enum sTypes
        {
            AUDIOBOOK,
            PODCAST,
            ITUNESU,
            MUSICTRACKS,
            PLAYLIST
        };
           CReturnIndexedPlayingTrackInfo( bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           int getTrackDuration();
           int getInfoType();
           QString getGenre();
           QString getComposer();
           int getTypeInfo();
           int getArtworkFormatId();
           int getArtworkFormatCount();
        private:
            int m_TrackDuration;
            int m_infoType;
	    QString m_Genre;
	    QString m_Composer;
            int m_artworkFormatId;
            int m_artworkcount;
            //QString m_TrackTypeInfo;

            sTypes m_TrackTypeInfo;

    };

} // end of namespace ExtendedLingo

#endif // DHAVN_RETURNINDEXEDPLAYINGTRACKINFO_H
