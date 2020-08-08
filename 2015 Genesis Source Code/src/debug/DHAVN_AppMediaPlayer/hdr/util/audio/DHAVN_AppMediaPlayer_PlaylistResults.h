#ifndef _PLAYLISTRESULTS_H
#define _PLAYLISTRESULTS_H

#include <QList>
#include <QMetaType>
#include <QDBusArgument>

struct PlaylistResultsEntries
{
    QString     fileName;
    QString     filePath;
    QString     title;
    QString     artist;
    QString     album;
    QString     genre;
    int         length_seconds;
    int         bitrate;
    int         size_bytes;
    int         user_rating;
    int         release_year;
    int         lastplayed_date;
};

struct PlaylistResults
{
    int                             count;
    QList<PlaylistResultsEntries>   playlistData;
};

QDBusArgument &operator<<(QDBusArgument &argument, const PlaylistResults &playlist);
const QDBusArgument &operator>>(const QDBusArgument &argument, PlaylistResults &playlist);

QDBusArgument &operator<<(QDBusArgument &argument, const PlaylistResultsEntries &playlist);
const QDBusArgument &operator>>(const QDBusArgument &argument, PlaylistResultsEntries &playlist);


Q_DECLARE_METATYPE(PlaylistResultsEntries);
Q_DECLARE_METATYPE(PlaylistResults);

#endif
