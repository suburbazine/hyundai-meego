#include "util/audio/DHAVN_AppMediaPlayer_PlaylistResults.h"

QDBusArgument &operator<<(QDBusArgument &argument, const PlaylistResultsEntries &playlistEntries)
{
    argument.beginStructure();
    argument << playlistEntries.fileName <<
                playlistEntries.filePath <<
                playlistEntries.title <<
                playlistEntries.artist <<
                playlistEntries.album <<
                playlistEntries.genre <<
                playlistEntries.length_seconds <<
                playlistEntries.bitrate <<
                playlistEntries.size_bytes <<
                playlistEntries.user_rating <<
                playlistEntries.release_year <<
                playlistEntries.lastplayed_date;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, PlaylistResultsEntries &playlistEntries)
{
    argument.beginStructure();
    argument >> playlistEntries.fileName >>
                playlistEntries.filePath >>
                playlistEntries.title >>
                playlistEntries.artist >>
                playlistEntries.album >>
                playlistEntries.genre >>
                playlistEntries.length_seconds >>
                playlistEntries.bitrate >>
                playlistEntries.size_bytes >>
                playlistEntries.user_rating >>
                playlistEntries.release_year >>
                playlistEntries.lastplayed_date;
    argument.endStructure();

    return argument;
}



QDBusArgument &operator<<(QDBusArgument &argument, const PlaylistResults &playlist)
{
    argument.beginStructure();
    argument << playlist.count << playlist.playlistData;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, PlaylistResults &playlist)
{
    argument.beginStructure();
    argument >> playlist.count >> playlist.playlistData;
    argument.endStructure();

    return argument;

}


