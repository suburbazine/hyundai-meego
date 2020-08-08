/*
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * History
 * 01-Aug-2011      Divya Dsouza
 ******************************************************************
 */

#ifndef DHAVN_TRACKERMUSICDATA_H
#define DHAVN_TRACKERMUSICDATA_H

#include <QString>
#include <QMetaType>
#include <QtDBus/QDBusArgument>

/**
  * Structure containing the Track information.
  */
struct TrackerMusicTrackData
{
    QString track_title;
    QString track_artist;
    QString track_year;
    QString tract_artistType;
    QString track_artistOrigin;
    QString track_artistEra;
    QString track_genre;
    QString track_path;
    QString track_composer;
};

/**
  * Structure containing the media file information.
  */
struct TrackerMusicData
{
public:
    QString track_title;
    QString album;
    QString track_artist;
    QString genre;
    QString id3album;
    QString filename;
    QString path;
    QString composer;
    QString id3artist;
    QString imagePath;
    QString track_number;

    QString album_title;
    QString album_artist;
    QString album_genre;
    QString album_year;

    QString CD_TOC;
    int track_count;
    QList<TrackerMusicTrackData> trackData;
};

Q_DECLARE_METATYPE(TrackerMusicTrackData);
Q_DECLARE_METATYPE(TrackerMusicData);

QDBusArgument &operator<<(QDBusArgument &argument, const TrackerMusicTrackData &trackMusicData);
const QDBusArgument &operator>>(const QDBusArgument &argument, TrackerMusicTrackData &trackMusicData);

QDBusArgument &operator<<(QDBusArgument &argument, const TrackerMusicData &trackData);
const QDBusArgument &operator>>(const QDBusArgument &argument, TrackerMusicData &trackData);




#endif // TRACKERMUSICDATA_H
