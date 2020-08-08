
#include "DHAVN_TrackerMusicData.h"

QDBusArgument &operator<<(QDBusArgument &argument, const TrackerMusicTrackData &trackMusicData)
{
    argument.beginStructure();
    argument << trackMusicData.track_artist << trackMusicData.track_artistEra << trackMusicData.track_artistOrigin ;
    argument << trackMusicData.track_genre << trackMusicData.track_title << trackMusicData.track_year ;
    argument << trackMusicData.tract_artistType;

    argument.endStructure();

    return argument;

}

const QDBusArgument &operator>>(const QDBusArgument &argument, TrackerMusicTrackData &trackMusicData)
{

    argument.beginStructure();

    argument >> trackMusicData.track_artist >> trackMusicData.track_artistEra >> trackMusicData.track_artistOrigin ;
    argument >> trackMusicData.track_genre >> trackMusicData.track_title >> trackMusicData.track_year ;
    argument >> trackMusicData.tract_artistType;

    argument.endStructure();

    return argument;
}

QDBusArgument &operator<<(QDBusArgument &argument, const TrackerMusicData &trackData)
{

    argument.beginStructure();

    argument << trackData.track_title << trackData.album << trackData.track_artist;
    argument << trackData.genre << trackData.id3album << trackData.filename;
    argument << trackData.path << trackData.composer<< trackData.id3artist;
    argument << trackData.imagePath << trackData.track_number << trackData.album_title;
    argument << trackData.album_artist << trackData.album_genre << trackData.album_year;
    argument << trackData.CD_TOC << trackData.track_count <<trackData.trackData;

    argument.endStructure();

    return argument;

}

const QDBusArgument &operator>>(const QDBusArgument &argument, TrackerMusicData &trackData)
{

    argument.beginStructure();

    argument >> trackData.track_title >> trackData.album >> trackData.track_artist;
    argument >> trackData.genre >> trackData.id3album >> trackData.filename;
    argument >> trackData.path >> trackData.composer>> trackData.id3artist;
    argument >> trackData.imagePath >> trackData.track_number >> trackData.album_title;
    argument >> trackData.album_artist >> trackData.album_genre >> trackData.album_year;
    argument >> trackData.CD_TOC >> trackData.track_count >> trackData.trackData;

    argument.endStructure();

    return argument;

}

