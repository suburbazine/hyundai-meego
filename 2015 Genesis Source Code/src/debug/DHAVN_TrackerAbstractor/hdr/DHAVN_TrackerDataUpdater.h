/*
******************************************************************
*        COPYRIGHT (C) Teleca AB      *
*----------------------------------------------------------------*
* MODULE     :  DHAVN_TrackerAbstractor                             *
*
* PROGRAMMER :  Pawan Gupta                                   *
* DATE       :  15 November 2011                                    *
* VERSION    :  1.0                                              *
*
*----------------------------------------------------------------*
*                                                                *
* MODULE SUMMARY : This file defines class which interact with    *
*                  Tracker database using spaql query to update    *
*                  information                  *
*                                                                *
*----------------------------------------------------------------*
*
* MODIFICATION RECORDS :                                         *
*
* VERSION   DATE                     PROGRAMMER              COMMENT
******************************************************************
* 1.0       15 Nov 2011  Pawan Gupta           Draft version
******************************************************************
*/
#ifndef DHAVN_TRACKERDATAUPDATER_H
#define DHAVN_TRACKERDATAUPDATER_H

#include <QStringList>
#include <QVector>
#include "DHAVN_TrackerAbstractor_constants.h"
#include <QtSparql/QSparqlResult>
#include <QtSparql/QSparqlQuery>
#include <QtSparql/QSparqlConnection>
#include <QtSparql/QSparqlError>
#include <mediaartinfo.h>
#include <qt4/Qt/qpixmap.h>
#include <playlistresults.h>
#include <DHAVN_TrackerMusicData.h>
#include <QSqlDatabase>
#include <DHAVN_LogUtil.h>

/**
   * forward declaration
   */
class QSparqlConnection;
class QSparqlResult;

/**
   * This class will handle the request from the CTrackerAbstractor class and
   * execute the sparql queries for the updation of the data into tracker.
   */
class CTrackerDataUpdater : public QObject{
    Q_OBJECT

    USE_LOG_UTIL

public :
        CTrackerDataUpdater(QSparqlConnection* sparqlConnection);
      ~CTrackerDataUpdater();


      QSqlDatabase contactDb;
/////
      bool UpdateSqlContactData(QList<BT_PBAP_DB_CONTACT>& contactDataList);
/////
      bool CreateSqlPhoneBookTable();
      bool CreateSqlConnectionForContacts();

      bool RemovePhoneBookDataBySelectionFromSQLiteDB(QString bd_address, BT_PBAP_DB_TYPE dbType);
      bool RemoveContactDataByNameFromSQLiteDB(QString bd_address, QString name, BT_PBAP_DB_MEMORY memoryType);
      bool RemoveContactDataByIndexFromSQLiteDB(QString bd_address, qint16 index, BT_PBAP_DB_MEMORY memoryType);

/////
      bool InsertContactDataIntoSQLiteDB(QList<BT_PBAP_DB_CONTACT>& contactDataList);
/////


      /* Ipod track info Updater & remove API's */
      bool UpdateIPodMusicTrackData(SAppleInfoList trackList);
      bool RemoveIPodMusicTrackData(SIPodTrackRemoveStruct trackStruct);
      bool UpdateCoverArtData(SCoverartMusicData musicdata);

      /*Ipod Type(podcast/Audiobook/playlist) updater & remove API's */
      bool UpdateIPodTypeData(SAppleHierarchyInfoList infoList, ETrackerAbstarctor_Query_Ids id);

      bool RemoveAllIPodInfo(QString serialNumber);
      bool RemoveIPodPodcastData(SIPodTypeRemoveInfo PodcastInfo);
      bool RemoveIPodAudioBookData(SIPodTypeRemoveInfo AudioBookInfo);
      bool RemoveIPodPlaylistData(SIPodTypeRemoveInfo PlayListInfo);

      /* Update Gracenote Playlist data into Tracker Store */
      bool UpdateTrackerWithPlaylistInfo(QString playlistCriteria, PlaylistResults playlistData);

      /*API's for updating Gracenote data in Tracker Store*/
      bool UpdateTrackerWithGracenoteData(TrackerMusicData musicData);
      bool DeleteCDData(QString cdTOC);


private:
    /* Common Execute query   API's */
    bool ExecuteQuery(QString query , QSparqlQuery::StatementType type );
    QString AlbumInsertQuery(QString albumName);
    QString ArtistInsertQuery(QString ArtistName);
    bool PrepareVolumeList(QStringList& volumeList);
    bool InsertVolume(QString SerialNumber);
    void CreateURL(QString& retURL, QString serialNumber, QString type, quint64 UID, int index, int topIndex=0);

    /* Ipod private API calls */
    bool UpdateIPodAudioBookData(SAppleHierarchyInfoList infoList);
    bool UpdateIPodPodcastData(SAppleHierarchyInfoList infoList);
    bool UpdateIPodMusicPlaylistData(SAppleHierarchyInfoList infoList);

    /*API's for updating Gracenote Data to Tracker Store*/
    bool BasicMusicInfoUpdateQuery(QString MusicFileUrn ,TrackerMusicData musicData);
    QString GracenoteDataAlbumInsertQuery(QString albumName);
    bool  CDMusicInfoInsertQuery(TrackerMusicData musicData);
    QString MusicAlbumDiscUpdateQuery(QString albumName, QString discTOC);
    QString CDAlbumUpdateQuery(QString albumName, QString artist, QString trackCount);

private:
/**
   * QSparqlConnection object to execute query
   */
QSparqlConnection* m_pSparql_Connection;

bool m_IsContactDBCreated;
    };

#endif // DHAVN_TRACKERDATAUPDATER_H
