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
#include <DHAVN_TrackerDataUpdater.h>
#include <QUrl>
#include <qfile.h>
#include <qdir.h>
#include <QtSql>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QStringList>
#include <QSqlError>
#include "DHAVN_CoverArtDataProvider.h"

#define PhoneBookContacts_FILENAME   "/app/tracker/PhoneBookContactsNewAdd"

/*
 *******************************************************************
 * Function		: CTrackerDataUpdater::CTrackerDataUpdater
 *
 * Description 	: Constructor

 * Parameters 	:

 * Returns 		:
 *
 *******************************************************************
 */
CTrackerDataUpdater::CTrackerDataUpdater(QSparqlConnection* sparqlConnection)
{
    LOG_INIT_V2("Media_TrackerAbstractor")
    m_pSparql_Connection = sparqlConnection;
    m_IsContactDBCreated = false;
}

/*
 *******************************************************************
 * Function		: CTrackerDataUpdater::~CTrackerDataUpdater
 *
 * Description 	: Destructor

 * Parameters 	:

 * Returns 		:
 *
 *******************************************************************
 */
CTrackerDataUpdater::~CTrackerDataUpdater()
{
}

/*
 *******************************************************************
 * Function		: CTrackerDataUpdater::UpdateIPodMusicTrackData
 *
 * Description 	: This API is to insert/update ipod tracks data into tracker store

 * Parameters 	:

 * Returns 		: None
 *
 *******************************************************************/
bool CTrackerDataUpdater::UpdateIPodMusicTrackData(SAppleInfoList trackList)
{
    bool ret = false;
    LOG_TRACE << "in UpdateIPodMusicTrackData" << LOG_ENDL;

    if(!InsertVolume(trackList.m_ipodSerialNumber))
        return ret;

    QList<SAppleTrackInfo> IpodTrackInfo = trackList.m_AppleInfoList;

    for ( int index = 0; index < IpodTrackInfo.count(); index++) {

        QString artistname = IpodTrackInfo.at(index).m_Artist;
        artistname.replace(" ", "%20");
        artistname.replace("'", "\\'");
        QString composername = IpodTrackInfo.at(index).m_Composer;
        composername.replace(" ", "%20");
        composername.replace("'", "\\'");
        QString albumname = IpodTrackInfo.at(index).m_Album;
        albumname.replace(" ", "%20");
        albumname.replace("'", "\\'");

        QString title;
        title.append(IpodTrackInfo[index].m_Title);
//        title.replace(" ", "%20");
        title.replace("'", "\\'");

        QString genre;
        genre.append(IpodTrackInfo[index].m_Genre);
//        genre.replace(" ", "%20");
        genre.replace("'", "\\'");

        ArtistInsertQuery(IpodTrackInfo.at(index).m_Artist);
        ArtistInsertQuery(IpodTrackInfo.at(index).m_Composer);
        AlbumInsertQuery(IpodTrackInfo.at(index).m_Album);

        QString url;
        CreateURL(url, trackList.m_ipodSerialNumber, IpodTrackInfo.at((index)).m_Type, IpodTrackInfo.at(index).m_UID, IpodTrackInfo.at(index).m_trackIndex);

        QString ipodmusicDataQuery;
        ipodmusicDataQuery= gIpodMusicTrackInsertQuery.arg(title)
                            .arg(artistname)
                            .arg(composername)
                            .arg(albumname)
                            .arg(genre)
                            .arg(IpodTrackInfo.at(index).m_UID)
                            .arg(IpodTrackInfo.at(index).m_trackIndex)
                            .arg(url).arg(gIPODAllSongType);

        ret = ExecuteQuery(ipodmusicDataQuery, QSparqlQuery::InsertStatement);
        if(!ret)
            LOG_TRACE << "failed to insert Ipod track" << LOG_ENDL;
    }
    return ret;
}

/*
 *******************************************************************
 * Function		: CTrackerDataUpdater::UpdateIPodMusicPlaylistData
 *
 * Description 	: This API is to insert/update ipod playlist data into tracker store

 * Parameters 	:

 * Returns 		: None
 *
 *******************************************************************/
bool CTrackerDataUpdater::UpdateIPodMusicPlaylistData(SAppleHierarchyInfoList infoList)
{

    bool ret = false;
    int count=0;
    int index=0;

    LOG_TRACE <<"in UpdateIPodMusicPlaylistData" << LOG_ENDL;

    if(!InsertVolume(infoList.m_ipodSerialNumber))
        return ret;

     QList<SAppleHierarchyInfo> IpodPlaylistInfo = infoList.m_AppleHierarchyInfoList;

     LOG_TRACE << "IpodPlaylistInfo.count()" << IpodPlaylistInfo.count() << LOG_ENDL;

    for (index = 0; index < IpodPlaylistInfo.count(); index++)
        {

        LOG_TRACE << "Inside for IpodPlaylistInfo.count() " <<IpodPlaylistInfo.count() << LOG_ENDL;
        LOG_TRACE << "Index for " << index << LOG_ENDL;
           QString IpodPlaylistQuery = gIpodPlaylistInsertQuery.arg(IpodPlaylistInfo.at((index)).m_TopLevelName)
                                                               .arg(IpodPlaylistInfo.at((index)).m_TrackCount)
                                                               .arg(IpodPlaylistInfo.at((index)).m_Type)
                                                               .arg(IpodPlaylistInfo.at((index)).m_TopLevelIndex);
          //  do
           while(count < (IpodPlaylistInfo.at((index)).m_TrackCount))
             {

               QString url;
               CreateURL(url,infoList.m_ipodSerialNumber, IpodPlaylistInfo.at((index)).m_Type, IpodPlaylistInfo.at(index).m_UID, IpodPlaylistInfo.at(index).m_trackIndex);


               QString ipodmusicDataQuery;

               ipodmusicDataQuery = gIpodMusicDataQuery.arg(url).arg(count).arg(IpodPlaylistInfo.at(index).m_Title);

               IpodPlaylistQuery.append(ipodmusicDataQuery);
                  LOG_TRACE << "IpodPlaylistInfo.at((index)).m_TrackCount" <<IpodPlaylistInfo.at((index)).m_TrackCount << LOG_ENDL;

                   LOG_TRACE << "Index while before " << index << LOG_ENDL;
                   LOG_TRACE << "Count before " << count << LOG_ENDL;

               // semi colon should not be inserted in case there is only one entry in playlist
               // or the last item is being inserted
               if(IpodPlaylistInfo.at((index)).m_TrackCount != 1 && count != ( (IpodPlaylistInfo.at((index)).m_TrackCount) - 1)) {
                   LOG_TRACE << "Inside if IpodPlaylistInfo.at((index)).m_TrackCount" <<IpodPlaylistInfo.at((index)).m_TrackCount << LOG_ENDL;
                   LOG_TRACE << "Inside IF Count " << count << LOG_ENDL;
                   IpodPlaylistQuery.append(";");
                   index++;
               }

               count ++;


               LOG_TRACE << "Index while after " << index << LOG_ENDL;
               LOG_TRACE << "Count after " << count << LOG_ENDL;
            }
            // } while(count < (IpodPlaylistInfo.at((index)).m_TrackCount-1));

                count = 0;
                IpodPlaylistQuery.append("}");
                LOG_TRACE << IpodPlaylistQuery << LOG_ENDL;

             ret = ExecuteQuery(IpodPlaylistQuery,QSparqlQuery::InsertStatement);

             if(!ret)
              LOG_TRACE << "failed to insert Ipod playlist data" << LOG_ENDL;

      }

    return ret;

}

bool CTrackerDataUpdater::UpdateIPodTypeData(SAppleHierarchyInfoList infoList, ETrackerAbstarctor_Query_Ids id)
{
    bool ret = false;
    switch(id)
    {
    case eInsertIPodPodcastToDBaseQuery_Id:
        return UpdateIPodPodcastData(infoList);
        break;
    case eInsertIPodAudioBookToDBaseQuery_Id:
        return UpdateIPodAudioBookData(infoList);
        break;
    case eInsertIPodMusicPlaylistToDBaseQuery_Id:
        return UpdateIPodMusicPlaylistData(infoList);
        break;
    default:
        break;
    }
    return ret;
}

bool CTrackerDataUpdater::UpdateIPodAudioBookData(SAppleHierarchyInfoList infoList)
{
    bool ret = false;
    LOG_TRACE << "in UpdateIPodAudioBookData" << LOG_ENDL;

    if(!InsertVolume(infoList.m_ipodSerialNumber))
        return ret;

    QList<SAppleHierarchyInfo> IpodAudioBookInfo = infoList.m_AppleHierarchyInfoList;

    for ( int index = 0; index < IpodAudioBookInfo.count(); index++) {

        ArtistInsertQuery(IpodAudioBookInfo.at(index).m_Artist);
        ArtistInsertQuery(IpodAudioBookInfo.at(index).m_Composer);
        AlbumInsertQuery(IpodAudioBookInfo.at(index).m_TopLevelName);

        QString artistname = IpodAudioBookInfo.at(index).m_Artist;
        QString albumname =IpodAudioBookInfo.at(index).m_TopLevelName;
          artistname.replace(" ", "%20");
            albumname.replace(" ", "%20");
            artistname.replace("'", "\\'");
            albumname.replace("'", "\\'");

            QString title;
            title.append(IpodAudioBookInfo[index].m_Title);
//            title.replace(" ", "%20");
            title.replace("'", "\\'");


            QString genre;
            genre.append(IpodAudioBookInfo[index].m_Genre);
//            genre.replace(" ", "%20");
            genre.replace("'", "\\'");


        QString url;
        CreateURL(url, infoList.m_ipodSerialNumber, IpodAudioBookInfo.at((index)).m_Type, IpodAudioBookInfo.at(index).m_UID, IpodAudioBookInfo.at(index).m_trackIndex, IpodAudioBookInfo.at(index).m_TopLevelIndex);

        LOG_TRACE << "title:"<< IpodAudioBookInfo.at(index).m_Title << LOG_ENDL;
        LOG_TRACE << "artist:"<< artistname << LOG_ENDL;
        LOG_TRACE << "album:"<< albumname << LOG_ENDL;
        LOG_TRACE << "genre:"<< IpodAudioBookInfo.at(index).m_Genre << LOG_ENDL;
        LOG_TRACE << "UID:"<< IpodAudioBookInfo.at(index).m_UID << LOG_ENDL;
        LOG_TRACE << "Index:"<< IpodAudioBookInfo.at(index).m_trackIndex << LOG_ENDL;
        LOG_TRACE << "url:"<< url << LOG_ENDL;
        LOG_TRACE << "Top Level Type:"<< IpodAudioBookInfo.at(index).m_Type << LOG_ENDL;
        LOG_TRACE << "Top Level Index:"<< IpodAudioBookInfo.at(index).m_TopLevelIndex << LOG_ENDL;

        QString ipodmusicDataQuery;
        ipodmusicDataQuery= gIpodPodcastAudioBookInsertQuery.arg(title)
                            .arg(artistname).arg(albumname)
                            .arg(genre).arg(IpodAudioBookInfo.at(index).m_UID)
                            .arg(IpodAudioBookInfo.at(index).m_trackIndex).arg(url)
                            .arg(IpodAudioBookInfo.at(index).m_Type).arg(IpodAudioBookInfo.at(index).m_TopLevelIndex);

        ret = ExecuteQuery(ipodmusicDataQuery, QSparqlQuery::InsertStatement);
        if(!ret)
            LOG_TRACE << "failed to insert Audiobook" << LOG_ENDL;
    }
    return ret;
}

bool CTrackerDataUpdater::UpdateIPodPodcastData(SAppleHierarchyInfoList infoList)
{
    bool ret = false;
    LOG_TRACE << "in UpdateIPodPodcastData" << LOG_ENDL;

    if(!InsertVolume(infoList.m_ipodSerialNumber))
        return ret;

    QList<SAppleHierarchyInfo> IpodPodcastInfo = infoList.m_AppleHierarchyInfoList;

    for ( int index = 0; index < IpodPodcastInfo.count(); index++) {

        ArtistInsertQuery(IpodPodcastInfo.at(index).m_Artist);
        ArtistInsertQuery(IpodPodcastInfo.at(index).m_Composer);
        AlbumInsertQuery(IpodPodcastInfo.at(index).m_TopLevelName);

        QString artistname = IpodPodcastInfo.at(index).m_Artist;
        QString albumname =IpodPodcastInfo.at(index).m_TopLevelName;
        artistname.replace(" ", "%20");
        albumname.replace(" ", "%20");
        artistname.replace("'", "\\'");
        albumname.replace("'", "\\'");

        QString title;
        title.append(IpodPodcastInfo[index].m_Title);
//        title.replace(" ", "%20");
        title.replace("'", "\\'");

        QString genre;
        genre.append(IpodPodcastInfo[index].m_Genre);
//        genre.replace(" ", "%20");
        genre.replace("'", "\\'");


        QString url;
        CreateURL(url, infoList.m_ipodSerialNumber, IpodPodcastInfo.at((index)).m_Type, IpodPodcastInfo.at(index).m_UID, IpodPodcastInfo.at(index).m_trackIndex, IpodPodcastInfo.at(index).m_TopLevelIndex);


        LOG_TRACE << "title:"<< IpodPodcastInfo.at(index).m_Title << LOG_ENDL;
        LOG_TRACE << "artist:"<< artistname << LOG_ENDL;
        LOG_TRACE << "album:"<< albumname << LOG_ENDL;
        LOG_TRACE << "genre:"<< IpodPodcastInfo.at(index).m_Genre << LOG_ENDL;
        LOG_TRACE << "UID:"<< IpodPodcastInfo.at(index).m_UID << LOG_ENDL;
        LOG_TRACE << "Index:"<< IpodPodcastInfo.at(index).m_trackIndex << LOG_ENDL;
        LOG_TRACE << "url:"<< url << LOG_ENDL;
        LOG_TRACE << "Top Level Type:"<< IpodPodcastInfo.at(index).m_Type << LOG_ENDL;
        LOG_TRACE << "Top Level Index:"<< IpodPodcastInfo.at(index).m_TopLevelIndex << LOG_ENDL;

        QString ipodmusicDataQuery;

        ipodmusicDataQuery= gIpodPodcastAudioBookInsertQuery.arg(title)
                            .arg(artistname).arg(albumname)
                            .arg(genre).arg(IpodPodcastInfo.at(index).m_UID)
                            .arg(IpodPodcastInfo.at(index).m_trackIndex).arg(url)
                            .arg(IpodPodcastInfo.at(index).m_Type).arg(IpodPodcastInfo.at(index).m_TopLevelIndex);

        ret = ExecuteQuery(ipodmusicDataQuery, QSparqlQuery::InsertStatement);
        if(!ret)
            LOG_TRACE << "failed to insert Ipod podcast" << LOG_ENDL;
    }
    return ret;
}

bool CTrackerDataUpdater::RemoveIPodMusicTrackData(SIPodTrackRemoveStruct trackStruct)
{
    bool ret=false;
    QString url;
    CreateURL(url, trackStruct.SerialNumber, gIPODAllSongType, trackStruct.TrackUID, trackStruct.TrackIndex);
    QString queryString = gIpodRemoveTrackFromDBQuery.arg(url);
    ret = ExecuteQuery(queryString , QSparqlQuery::InsertStatement);
    if(!ret)
    {
        LOG_TRACE << "failed to remove Ipod track" << LOG_ENDL;
    }
    return ret;
}

bool CTrackerDataUpdater::RemoveIPodPodcastData(SIPodTypeRemoveInfo podcastInfo)
{
    bool ret = false;
QString queryString = gIpodRemovePodcastAudioBookFromDBQuery.arg(gIPODPodcastType).arg(podcastInfo.IPodTypeName).arg(podcastInfo.SerialNumber);
ret = ExecuteQuery(queryString , QSparqlQuery::InsertStatement);
if(!ret)
{
    LOG_TRACE << "failed to remove Ipod podcast" << LOG_ENDL;
}
return ret;
}

bool CTrackerDataUpdater::RemoveIPodAudioBookData(SIPodTypeRemoveInfo AudioBookInfo)
{
    bool ret = false;
    QString queryString = gIpodRemovePodcastAudioBookFromDBQuery.arg(gIPODAudioBookType).arg(AudioBookInfo.IPodTypeName).arg(AudioBookInfo.SerialNumber);
    ret = ExecuteQuery(queryString , QSparqlQuery::InsertStatement);
    if(!ret)
    {
        LOG_TRACE << "failed to remove Ipod audiobook" << LOG_ENDL;
    }
    return ret;
}
bool CTrackerDataUpdater::RemoveIPodPlaylistData(SIPodTypeRemoveInfo PlayListInfo)
{
    bool ret = false;
    QString queryString = gIpodRemovePodcastAudioBookFromDBQuery.arg(gIPODPlayListType).arg(PlayListInfo.IPodTypeName).arg(PlayListInfo.SerialNumber);
    ret = ExecuteQuery(queryString , QSparqlQuery::InsertStatement);
    if(!ret)
    {
        LOG_TRACE << "failed to remove Ipod audiobook" << LOG_ENDL;
    }
    return ret;
}

bool CTrackerDataUpdater::RemoveAllIPodInfo(QString serialNumber)
{
bool ret = false;
QString volumePath;
volumePath.append(gVolumePath_IPod);
volumePath.append(gForwardSlace);
volumePath.append(serialNumber);
QString queryString = gIpodRemoveAllInfoFromDBQuery.arg(volumePath);
ret = ExecuteQuery(queryString , QSparqlQuery::InsertStatement);
if(!ret)
{
    LOG_TRACE << "failed to remove all Ipod info" << LOG_ENDL;
}
return ret;
}

bool CTrackerDataUpdater::InsertVolume(QString SerialNumber)
{
    bool ret=true;
    QString IpodFileURL;
    IpodFileURL.append(gVolumePath_IPod);
    IpodFileURL.append(gForwardSlace);
    IpodFileURL.append(SerialNumber);

    QStringList volumeList;
    PrepareVolumeList(volumeList);

    if(!volumeList.contains(IpodFileURL))
    {
        QString IpodvolumeQuery = gIpodVolumeInsertQuery.arg(SerialNumber)
                                  .arg(IpodFileURL);
        ret = ExecuteQuery(IpodvolumeQuery , QSparqlQuery::InsertStatement);
        if(!ret)
        {
            LOG_TRACE << "failed to insert Ipod as volume" << LOG_ENDL;
            return ret;
        }
    }
    return ret;
}

void CTrackerDataUpdater::CreateURL(QString& retURL, QString serialNumber, QString type, quint64 UID, int index, int topIndex)
{
    retURL.clear();
    retURL.append(gVolumePath_IPod);
    retURL.append(gForwardSlace);
    retURL.append(serialNumber);
    retURL.append(gForwardSlace);
    retURL.append(type);
    retURL.append(gForwardSlace);
    QString strIndex;
    strIndex.setNum(index);
    QString strUid;
    strUid.setNum(UID);
    retURL.append(strIndex);
    retURL.append(gUnderscoreChar);
    retURL.append(strUid);
    QString strTopIndex;
    strTopIndex.setNum(topIndex);
    if(type.compare(gIPODAllSongType))
    {
        retURL.append(gUnderscoreChar);
        retURL.append(strTopIndex);
    }
}

/*
 *******************************************************************
 * Function		: CTrackerDataUpdater::ArtistInsertQuery
 *
 * Description 	: This function inserts a record in the Tracker Store with
 *                        the Artist information.
 *
 * Parameters 	: ArtistName(IN) - Artist name to be updated in the record
 *                        in Tracker Store.
 *
 * Returns 		: QString (OUT) - artistUrn of the record in the Tracker Store
 *                        is returned.
 *
 *******************************************************************
 */
QString  CTrackerDataUpdater::ArtistInsertQuery(QString ArtistName)
{
    QString artistUrn ;
    artistUrn.append(ArtistName);
    artistUrn.replace(" ", "%20");

    //Modified artist name to update special characters
    QString artistName;
    artistName.append(ArtistName);
    artistName = artistName.replace("'","\\'");

    QString artistQuery;
    artistQuery =gIpodArtistNameUpdateQuery.arg(artistUrn,artistName);
    bool status = ExecuteQuery(artistQuery , QSparqlQuery::InsertStatement);

    if(status != true)
        return NULL;

    return artistUrn;
}


/*
 *******************************************************************
 * Function		: CTrackerDataUpdater::AlbumInsertQuery
 *
 * Description 	: This function inserts a record in the Tracker Store with
 *                        the Album information.

 * Parameters 	: albumName(IN) - Album name to be updated in the record
 *                        in Tracker Store.

 * Returns 		: QString(OUT) - albumUrn of the record in the Tracker Store
 *                        is returned.
 *
 *******************************************************************
 */
QString CTrackerDataUpdater::AlbumInsertQuery(QString albumName)
{
    QString albumUrn;
    albumUrn.append(albumName);
    albumUrn.replace(" ","%20");

    //Modified album name to update special characters
    QString modifiedAlbumName;
    modifiedAlbumName.append(albumName);
    modifiedAlbumName = modifiedAlbumName.replace("'","\\'");

    QString albumQuery;
    albumQuery =gIpodAlbumUpdateQuery.arg(albumUrn,modifiedAlbumName);
    bool status = ExecuteQuery(albumQuery , QSparqlQuery::InsertStatement);

    if(status != true)
        return NULL;

    return albumUrn;
}

/*
 *******************************************************************
 * Function		: CTrackerDataUpdater::ExecuteQuery
 *
 * Description 	: Thsi function execute a QSparql query for Tracker Store.

 * Parameters 	: query(IN) - QSparql query
 *                        type(IN) - QSparql query type

 * Returns 		: Bool(OUT) - Bool value indicating the status of the query.
 *
 *******************************************************************
 */
bool CTrackerDataUpdater::ExecuteQuery(QString query , QSparqlQuery::StatementType type )
{
    QSparqlQuery QueryObject;
    QueryObject.setType(type);
    QueryObject.setQuery(query);

    QSparqlResult* QueryResult = m_pSparql_Connection->exec(QueryObject);

    if (!QueryResult->hasError()) {
        LOG_TRACE << "Tracker Interface : TrackerQuery :: executeQuery() Executing query ok" << LOG_ENDL;
        QueryResult->waitForFinished();
        bool status = QueryResult->isFinished();
        return status;
        //connect(result, SIGNAL(finished()), this, SLOT(onFinished()));
    }
    else {
        LOG_TRACE << "Tracker Interface : TrackerQuery :: executeQuery() Executing query failed" << LOG_ENDL;
        return false;
    }

    return true;
}

bool CTrackerDataUpdater::PrepareVolumeList(QStringList& volumeList)
{
    bool ret = false;
    volumeList.clear();
    QSparqlQuery volumeQueryObject;
    volumeQueryObject.setQuery(gGetMountedVolumeListQuery);

    QSparqlResult* result = m_pSparql_Connection->syncExec(volumeQueryObject);

    //    QStringList volumeList;
    if (!result->hasError())
    {
        ret = true;
        while(result->next())
        {
            volumeList.append(result->value(0).toString());
        }
    }

    return ret;
}

/*
 *******************************************************************
 * Function		: CTrackerDataUpdater::UpdateCoverArtData
 *
 * Description 	: This API is to insert coverArt data into tracker store

 * Parameters 	: SCoverartMusicData

 * Returns 		: true or false
 *
 *******************************************************************/

bool CTrackerDataUpdater::UpdateCoverArtData(SCoverartMusicData musicData)
{

    LOG_TRACE << "CTrackerDataUpdater::UpdateCoverArtData" << musicData.imagePath << LOG_ENDL;

    QImage qImage(musicData.imagePath);
	QPixmap pixmap(QPixmap::fromImage(qImage));
    QSize size = pixmap.size();

    LOG_TRACE << "CTrackerDataUpdater::UpdateCoverArtData height" << size.height() << LOG_ENDL;
    LOG_TRACE << "CTrackerDataUpdater::UpdateCoverArtData width" << size.width() << LOG_ENDL;

    if(size.height() > 256 || size.width() > 256) // added by eugene.seo 2013.05.09
        pixmap = pixmap.scaled(QSize(256, 256), Qt::KeepAspectRatio, Qt::FastTransformation);  // added by eugene.seo 2013.05.09
    //{added by junam 2013.05.16 for coverart match
    //MediaArt::Info albumArt = MediaArt::Album(musicData.track_artist, musicData.album);
    QString iPodKey = CCoverArtDataprovider::getIPodUID(musicData.track_artist);
    MediaArt::Info albumArt = MediaArt::Album( iPodKey.isEmpty() ? musicData.track_artist : iPodKey, musicData.album);
    //}added by junam
    albumArt.setMediaArtImage(pixmap, true);

    return true;

}

/*
 *******************************************************************
 * Function		: CTrackerDataUpdater::UpdateTrackerWithPlaylistInfo
 *
 * Description 	: This API is to insert/update Gracenote playlist data into tracker store

 * Parameters 	:

 * Returns 		: None
 *
 *******************************************************************/
bool CTrackerDataUpdater::UpdateTrackerWithPlaylistInfo(QString playlistCriteria, PlaylistResults playlistRes)
{

    QString modifiedCriteria;
    modifiedCriteria = QUrl::toPercentEncoding(playlistCriteria,":/(),");

    LOG_TRACE << "modifiedCriteria" << modifiedCriteria  << LOG_ENDL;
    LOG_TRACE << "playlistRes.count" << playlistRes.count << LOG_ENDL;


    QString gracenotePlaylistQuery = gIpodPlaylistInsertQuery.arg(playlistCriteria)
                                                           .arg(playlistRes.count)
                                                           .arg(gGracenotePlaylistTypeName)
                                                           .arg(0);

   LOG_TRACE << "gracenotePlaylistQuery " << gracenotePlaylistQuery  << LOG_ENDL;


    // Append one element in the playlist
    QString completefilePath;
    QString mediaEntryInPlaylist ;

    for ( int index = 0; index < playlistRes.count; index++) {
        completefilePath.clear();
        mediaEntryInPlaylist.clear();

        completefilePath = playlistRes.playlistData.at(index).filePath;

        //Construct path as needed by tracker-store
        QString filePath;
        filePath.append("file://");
        filePath.append(completefilePath);
        filePath = QUrl::toPercentEncoding(filePath,":/(),");

        //qDebug() << "filePath" << filePath;

        mediaEntryInPlaylist = gIpodMusicDataQuery.arg(filePath).arg(index).arg(playlistCriteria);

        gracenotePlaylistQuery.append(mediaEntryInPlaylist);

        LOG_TRACE << "gracenotePlaylistQuery" << gracenotePlaylistQuery << LOG_ENDL;

        // semi colon should not be inserted in case there is only one entry in playlist
        // or the last item is being inserted
        if(playlistRes.count != 1 && index != ( playlistRes.count - 1)) {
            gracenotePlaylistQuery.append(";");
        }

    }

    gracenotePlaylistQuery.append("}");
    LOG_TRACE << gracenotePlaylistQuery << LOG_ENDL;

    bool status = ExecuteQuery(gracenotePlaylistQuery, QSparqlQuery::InsertStatement);

    return status;
}

/*
 *******************************************************************
 * Function		: CTrackerDataUpdater::UpdateQuery
 *
 * Description 	: This function updates the Tracker Store records with the
 *                        Gracenote metadata.
 *
 * Parameters 	: musicData(IN) - Structure containing the Gracenote metadata
 *                        for media files and tracks to be updated in the Tracker Store.

 * Returns 		: bool(OUT) - Bool value indicating the status of the query.
 *
 *******************************************************************
 */
bool CTrackerDataUpdater::UpdateTrackerWithGracenoteData(TrackerMusicData musicData)
{

    LOG_TRACE << "UpdateQuery" << LOG_ENDL;
    //For media files in Jukebox and USB
    if(musicData.CD_TOC == NULL)
    {

        LOG_TRACE << "CTrackerExtractor::UpdateQuery CD_TOC is NULL" << LOG_ENDL;

        //InsertCoverArt(musicData);

        QSparqlQuery QueryObject;
        QueryObject.setType(QSparqlQuery::SelectStatement);

        //Construct path as needed by tracker-store
        QString filePath;
        filePath.append("file://");
        filePath.append(musicData.path);

        filePath = filePath.replace("%","%25");
        filePath = filePath.replace(" ","%20");
        filePath = filePath.replace("[","%5B");
        filePath = filePath.replace("]","%5D");
        filePath = filePath.replace(";","%3B");
        filePath = filePath.replace("#","%23");
        filePath = filePath.replace("`","%60");

        filePath = filePath.replace("'","\\'");

        QString MusicFileQuery = gFileUrnQuery.arg(filePath);
        QueryObject.setQuery(MusicFileQuery);

        QSparqlResult* QueryResult = m_pSparql_Connection->syncExec(QueryObject);

        if (!QueryResult->hasError()) {
            LOG_TRACE << "CTrackerExtractor::UpdateQuery() Executing query ok" << LOG_ENDL;
        }
        else {
            LOG_TRACE << "CTrackerExtractor::UpdateQuery() Executing query failed" << LOG_ENDL;
            return false;
        }

        QString MusicFileUrn;
        while (QueryResult->next())
            MusicFileUrn =QueryResult->value(0).toString();

        LOG_TRACE << "CTrackerExtractor::UpdateQuery() MusicFileUrn: " << MusicFileUrn << LOG_ENDL;

        bool status = BasicMusicInfoUpdateQuery(MusicFileUrn ,musicData);

        if(status != true)
            return false;

        /*bool check = GetMusicInfo(filePath);
       if(check !=true)
            return false;*/

        return true;
    }
    //For tracks in album CD
    else
    {
        LOG_TRACE << "CTrackerExtractor::UpdateQuery CD_TOC is not NULL" << LOG_ENDL;
        LOG_TRACE << musicData.CD_TOC << LOG_ENDL;
        //InsertCoverArt(musicData);
        bool CDDataupdateStatus = CDMusicInfoInsertQuery(musicData);
        return CDDataupdateStatus;
    }

}

    /*
     *******************************************************************
     * Function		: CTrackerDataUpdater::BasicMusicInfoUpdateQuery
     *
     * Description 	: This function updates the Tracker Store with the Basic information
     *                        of the media file
     *                        .

     * Parameters 	: MusicFileUrn(IN) - Unique urn for the media file record
     *                        in the Tracker Store
     *                        musicData(IN) - Structure containing the Gracenote metadata
     *                        for media files and tracks to be updated in the Tracker Store.

     * Returns 		: Bool(OUT) - Bool value indicating the status of the query.
     *
     *******************************************************************
     */
    bool  CTrackerDataUpdater::BasicMusicInfoUpdateQuery(QString MusicFileUrn ,TrackerMusicData musicData)
    {

        if(MusicFileUrn.length() == 0) {
            LOG_TRACE << "URN EMPTY" << LOG_ENDL;
            return false;
        }

        QString performer = ArtistInsertQuery(musicData.track_artist);
        QString composer = ArtistInsertQuery(musicData.composer);
        QString album = GracenoteDataAlbumInsertQuery(musicData.album);

        QString title;
        title.append(musicData.track_title);
        title = title.replace("'","\\'");

        QString genre;
        genre.append(musicData.genre);
        genre =  genre.replace("'", "\\'");

        QString BasicInfoQuery = gBasicMusicUpdateQuery.arg(MusicFileUrn,title,performer,
                                                            composer,album,genre);

        bool status = ExecuteQuery(BasicInfoQuery , QSparqlQuery::InsertStatement);

        if(status != true)
            return false;

        return true;
    }



    /*
     *******************************************************************
     * Function		: CTrackerDataUpdater::GracenoteDataAlbumInsertQuery
     *
     * Description 	: This function inserts a record in the Tracker Store with
     *                        the Album information.

     * Parameters 	: albumName(IN) - Album name to be updated in the record
     *                        in Tracker Store.

     * Returns 		: QString(OUT) - albumUrn of the record in the Tracker Store
     *                        is returned.
     *
     *******************************************************************
     */
    QString CTrackerDataUpdater::GracenoteDataAlbumInsertQuery(QString albumName)
    {
        QString albumUrn;
        albumUrn.append(albumName);
        albumUrn.replace(" ","%");

        //Modified album name to update special characters
        QString modifiedAlbumName;
        modifiedAlbumName.append(albumName);
        modifiedAlbumName = modifiedAlbumName.replace("'","\\'");

        QString albumQuery;
        albumQuery =gAlbumUpdateQuery.arg(albumUrn,modifiedAlbumName);
        bool status = ExecuteQuery(albumQuery , QSparqlQuery::InsertStatement);

        if(status != true)
            return NULL;

        return albumUrn;
    }


    /*
     *******************************************************************
     * Function		: CTrackerDataUpdater::CDMusicInfoInsertQuery
     *
     * Description 	: This function inserts the Tracker Store with the track information
     *                        in the Album CD

     * Parameters 	: musicData(IN) - Structure containing the Gracenote metadata
     *                        for media files and tracks to be updated in the Tracker Store.

     * Returns 		: Bool(OUT) - Bool value indicating the status of the query.
     *
     *******************************************************************
     */
    bool  CTrackerDataUpdater::CDMusicInfoInsertQuery(TrackerMusicData musicData)
    {
        LOG_TRACE << musicData.album << LOG_ENDL;
        LOG_TRACE << musicData.track_artist << LOG_ENDL;
        QString trackCount;
        trackCount.setNum(musicData.track_count);

        QString album = CDAlbumUpdateQuery(musicData.album_title, musicData.album_artist, trackCount);

        LOG_TRACE << "show album name:"<< musicData.album << LOG_ENDL;

        QString musicAlbumDisc = MusicAlbumDiscUpdateQuery(musicData.album_title, musicData.CD_TOC);



        QString track_count;
        track_count.append(musicData.track_count);

        QString musicPieceAddr;
        QString track_title;
        QString track_genre;
        QString track_performer;
        QString track_num;
        QString checkStr;
        QString track_path;

        QString track_duration('0');

        for(int i=0; i<musicData.track_count; i++)
        {
            //retrieve the address for each track
            musicPieceAddr.clear();
            musicPieceAddr = musicData.CD_TOC.section(" ", i, i);

            track_title.clear();
            track_title.append(musicData.trackData.at(i).track_title);
            track_title = track_title.replace("'","\\'");

            track_genre.clear();

            track_path.clear();
            track_path.append(musicData.trackData.at(i).track_path);
            track_path = track_path.replace("'","\\'");

            LOG_TRACE << "CDMusicInfoInsertQuery track_path " << track_path << LOG_ENDL;

                checkStr = musicData.trackData.at(i).track_genre;
                if(checkStr.length() == 0)
                {
                   checkStr = musicData.album_genre;
                }

            track_genre.append(checkStr);
            track_genre =  track_genre.replace("'", "\\'");

                checkStr = musicData.trackData.at(i).track_artist;
            if(checkStr.length() == 0)
                {
                    checkStr = musicData.album_artist;
                }
            track_performer.clear();
            track_performer.append(ArtistInsertQuery(checkStr));

            track_num.clear();
            track_num.setNum(i);

            QString CDAlbumInfoUpdateQuery = gCDAlbumInfoUpdateQuery.arg(musicPieceAddr, track_title, track_performer, album,
                                                                         track_genre, track_num, musicAlbumDisc, track_duration,track_path);

            bool status = ExecuteQuery(CDAlbumInfoUpdateQuery , QSparqlQuery::InsertStatement);

            if(status != true)
                return false;

        }
        return true;
    }


    /*
     *******************************************************************
     * Function		: CTrackerDataUpdater::CDAlbumUpdateQuery
     *
     * Description 	: This function inserts a record in the Tracker Store with
     *                        the Album information.

     * Parameters 	: albumName(IN) - Album name to be updated in the record
     *                        in Tracker Store.

     * Returns 		: QString(OUT) - albumUrn of the record in the Tracker Store
     *                        is returned.
     *
     *******************************************************************
     */
    QString CTrackerDataUpdater::CDAlbumUpdateQuery(QString albumName, QString artist, QString trackCount)
    {
        LOG_TRACE << "CTrackerExtractor::CDAlbumUpdateQuery() CDAlbumUpdateQuery ................" << LOG_ENDL;
        QString albumUrn;
        albumUrn.append(albumName);
        albumUrn.replace(" ","%");

        //Modified album name to update special characters
        QString modifiedAlbumName;
        modifiedAlbumName.append(albumName);
        modifiedAlbumName = modifiedAlbumName.replace("'","\\'");

        QString performer = ArtistInsertQuery(artist);
        LOG_TRACE << "artistUrn after ArtistInsertQuery" << performer << LOG_ENDL;

        QString albumQuery;
        albumQuery =gCDAlbumUpdateQuery.arg(albumUrn,modifiedAlbumName, performer, trackCount);
        bool status = ExecuteQuery(albumQuery , QSparqlQuery::InsertStatement);

        LOG_TRACE << "albumQuery" << albumQuery  << "urn"<<albumUrn << LOG_ENDL;

        if(status != true)
            return NULL;

        return albumUrn;
    }


    /*
     *******************************************************************
     * Function		: CTrackerDataUpdater::MusicAlbumDiscUpdateQuery
     *
     * Description 	: This function updates the Tracker Store with the Disc Album
     *                        information.

     * Parameters 	: albumName(IN) - Album Name for the Album CD
     *                        discNum(IN) - The number of the disc

     * Returns 		: QString(OUT) - albumUrn : Unique urn for the music album disc
     *
     *******************************************************************
     */
    QString CTrackerDataUpdater::MusicAlbumDiscUpdateQuery(QString albumName, QString discTOC)
    {

        QString albumUrn;
        albumUrn.append(albumName);
        albumUrn.replace(" ","%");

        QString albumQuery;
        albumQuery =gMusicAlbumDiscUpdateQuery.arg(albumUrn, discTOC);
        bool status = ExecuteQuery(albumQuery , QSparqlQuery::InsertStatement);

        if(status != true)
            return NULL;

        return albumUrn;
    }

    /*
     *******************************************************************
     * Function		: CTrackerDataUpdater::DeleteCDData
     *
     * Description 	: Delete the tracks associated with a CD TOC from
                              tracker store

     * Parameters 	: cdTOC - CD table of contents

     * Returns 		: bool - status
     *
     *******************************************************************/
    bool CTrackerDataUpdater::DeleteCDData(QString cdTOC)
    {

        LOG_TRACE << "CTrackerExtractor: DeleteCDData called" << LOG_ENDL;

        if(cdTOC.length() == 0) {
            LOG_TRACE << "CD TOC EMPTY" << LOG_ENDL;
            return false;
        }

        QSparqlQuery queryObject;
        queryObject.setType(QSparqlQuery::SelectStatement);

        QString musicUrnQuery = gCDFileUrnQuery.arg(cdTOC);
        queryObject.setQuery(musicUrnQuery);

        QSparqlResult* queryResult = m_pSparql_Connection->syncExec(queryObject);

        if (!queryResult ->hasError()) {
            LOG_TRACE << "CTrackerExtractor::DeleteCDData() Executing query ok" << LOG_ENDL;
        }
        else {
            LOG_TRACE << "CTrackerExtractor::DeleteCDData() Executing query failed" << LOG_ENDL;
            return false;
        }

        // Retrieve all the Urn's related to TOC
        QStringList musicUrns;
        while (queryResult->next())
            musicUrns.append(queryResult->value(0).toString());

        //Delete each music piece one by one
        QString urn;
        for(int loop = 0 ; loop < musicUrns.count() ; loop++)
        {
            urn =  musicUrns.at(loop);

            QString deleteQuery = gDeleteCDFilesQuery.arg(urn);

            bool status = ExecuteQuery(deleteQuery , QSparqlQuery::DeleteStatement);

            if(status != true)
                return false;
        }

        return true;
    }

bool CTrackerDataUpdater::CreateSqlPhoneBookTable()
{
    QSqlQuery query(QSqlDatabase::database("PhoneDb"));
    query.exec("CREATE TABLE PhoneTable (idx integer,db_type int,memory_type int,bd_address varchar(13) \
                , first_name varchar(80), crypto_hash varchar(80), last_name varchar(80) \
                , formatted_name varchar(240), num_type1 int, number1 varchar(25), num_type2 int \
                , number2 varchar(25), num_type3 int, number3 varchar(25), num_type4 int, number4 varchar(25) \
                , num_type5 int, number5 varchar(25), time_stamp varchar(20), middle_name varchar(80) \
                , pinyin varchar(600), use_pinyin integer, pinyin_order varchar(300) \
                , PRIMARY KEY (idx,db_type,memory_type,bd_address))");
    qDebug() << "Error in query" << query.lastError().text();
    /* FEATURE_ORDER_BY_PINYIN */
    return true;
}
bool CTrackerDataUpdater::CreateSqlConnectionForContacts()
{
    bool result = false;

    contactDb = QSqlDatabase::addDatabase("QSQLITE","PhoneDb");

    /*
    * 기존에 존재하는 middleName 필드가 없는 DB파일 사용을 막고
    * 새로운 DB파일 생성을 위한 코드 추가 (2013.09.22)
    */
    contactDb.setDatabaseName(PhoneBookContacts_FILENAME);
    //contactDb.setDatabaseName("/app/tracker/PhoneBookContacts");

    if (!contactDb.open())
    {
        //        LOG_TRACE << " CreateSqlConnectionForContacts error opening database";
        qDebug() << " IN CreateSqlConnectionForContacts error opening database";
        return result;
    }
    QStringList tableNames = contactDb.tables();
    if(tableNames.contains("PhoneTable"))
    {
        qDebug() << "PhoneBookContacts Table is already created";
        //tableNames.removeAll("PhoneTable");
        //system("rm -rf PhoneBookContacts");
        //qDebug() << "Removed PhoneBookContacts DB and  PhoneTable, now recreate again...";
        //result = CreateSqlPhoneBookTable();
    }
    else
    {
        qDebug() << "PhoneBookContacts Table is being created";
        result = CreateSqlPhoneBookTable();
    }
    //  LOG_TRACE << "Database opened successfully";
    contactDb.close();
    return result;
}

/*
     *******************************************************************
     * Function		: CTrackerDataUpdater:: InsertContactDataIntoSQLiteDB
     *
     * Description 	: This API is to remove contact data from tracker store

     * Parameters 	:

     * Returns 		: None
     *
     *******************************************************************/
bool CTrackerDataUpdater::InsertContactDataIntoSQLiteDB(QList<BT_PBAP_DB_CONTACT>& contactDataList)
{
    bool ret;

    QSqlQuery query(QSqlDatabase::database("PhoneDb"));

    QString queryString;
    queryString = gInsertContactIntoDBFromSQLiteDBQuery;

    for(int i=0; i< contactDataList.count(); i++)
    {
        BT_PBAP_DB_CONTACT PhoneContactsInfo = contactDataList[i];
        QString index, db_type, memory_type;
        index.setNum(PhoneContactsInfo.index);
        db_type.setNum(PhoneContactsInfo.type);
        memory_type.setNum(PhoneContactsInfo.memory_Type);

		PhoneContactsInfo.first_name.replace(QByteArray("\""), QByteArray("\"\""));
		PhoneContactsInfo.last_name.replace(QByteArray("\""), QByteArray("\"\""));
		PhoneContactsInfo.formatted_name.replace(QByteArray("\""), QByteArray("\"\""));
        PhoneContactsInfo.middle_name.replace(QByteArray("\""), QByteArray("\"\""));

        QString subQuery;
        subQuery.append(QString(" SELECT \"%1\", \"%2\", \"%3\", \"%4\", ").arg(index).arg(db_type).arg(memory_type).arg(PhoneContactsInfo.bd_address));

        subQuery.append(QString("\""));
        subQuery.append(PhoneContactsInfo.first_name);
        subQuery.append(QString("\","));

        subQuery.append(QString("\""));
        subQuery.append(PhoneContactsInfo.crypto_hash);
        subQuery.append(QString("\","));

        subQuery.append(QString("\""));
        subQuery.append(PhoneContactsInfo.last_name);
        subQuery.append(QString("\","));

        subQuery.append(QString("\""));
        subQuery.append(PhoneContactsInfo.formatted_name);
        subQuery.append(QString("\","));

        subQuery.append(QString(" \"%1\", \"%2\", \"%3\", \"%4\", \"%5\", \"%6\", \"%7\", \"%8\", \"%9\", \"%10\", \"%11\",")
                        .arg(PhoneContactsInfo.num_type1)
                        .arg(PhoneContactsInfo.number1)
                        .arg(PhoneContactsInfo.num_type2)
                        .arg(PhoneContactsInfo.number2)
                        .arg(PhoneContactsInfo.num_type3)
                        .arg(PhoneContactsInfo.number3)
                        .arg(PhoneContactsInfo.num_type4)
                        .arg(PhoneContactsInfo.number4)
                        .arg(PhoneContactsInfo.num_type5)
                        .arg(PhoneContactsInfo.number5)
                        .arg(PhoneContactsInfo.time_stamp));

/////SUPPORT_MIDDLE_NAME
        subQuery.append(QString("\""));
        subQuery.append(PhoneContactsInfo.middle_name);
        subQuery.append(QString("\","));
/////SUPPORT_MIDDLE_NAME

        subQuery.append(QString("\""));
        subQuery.append(PhoneContactsInfo.pinyin);
        subQuery.append(QString("\","));

        QString use_pinyin;
        use_pinyin.setNum(PhoneContactsInfo.use_pinyin);
        subQuery.append(QString(" \"%1\", ").arg(use_pinyin));

        subQuery.append(QString("\""));
        subQuery.append(PhoneContactsInfo.pinyin_order);
        subQuery.append(QString("\" "));
/////TEST
        //qDebug() << subQuery << PhoneContactsInfo.use_pinyin;
/////TEST
    /* FEATURE_ORDER_BY_PINYIN */


        queryString.append(subQuery);

        if(i < (contactDataList.count()-1))
        {
            queryString.append(" UNION ALL");

        }

    }
    ret = query.exec(queryString);
    qDebug() << query.lastError().text();

    return ret;
}


bool CTrackerDataUpdater::UpdateSqlContactData(QList<BT_PBAP_DB_CONTACT>& contactDataList)
{
    if(!m_IsContactDBCreated)
    {
        CreateSqlConnectionForContacts();
        m_IsContactDBCreated = true;
    }
    
    bool ret;
    //LOG_TRACE << "UpdateSqlContactData" << LOG_ENDL;
    QList<BT_PBAP_DB_CONTACT> contactInfoList = contactDataList;

    int i =0;
    if(contactInfoList.count() > 500)
    {
        QList<BT_PBAP_DB_CONTACT> contactDataCopy;
        for (i = 0; i<contactInfoList.count(); i++)
        {
            contactDataCopy.append(contactInfoList[i]);
            if(contactDataCopy.count() == 500)
            {
                ret = InsertContactDataIntoSQLiteDB(contactDataCopy);
                contactDataCopy.clear();
            }
        }

        if(contactDataCopy.count() > 0)
        {
            ret = InsertContactDataIntoSQLiteDB(contactDataCopy);
        }
    }
    else
    {
        ret = InsertContactDataIntoSQLiteDB(contactInfoList);
    }
    return ret;
}

bool CTrackerDataUpdater::RemovePhoneBookDataBySelectionFromSQLiteDB(QString bd_address, BT_PBAP_DB_TYPE dbType)
{
    bool ret = true;

    if(!m_IsContactDBCreated)
    {
        CreateSqlConnectionForContacts();
        m_IsContactDBCreated = true;
    }

    QString removeQuery;

    if(bd_address.isEmpty())
    {
        removeQuery = gRemoveAllPhoneBookFromSQLiteDBQuery;
    }
    else
    {
		switch(dbType)
		{
			case DB_PHONE_BOOK:
			case DB_RECENT_CALLS:
			case DB_RECEIVED_CALLS:
			case DB_MISSED_CALLS:
			case DB_FAVORITE_PHONE_BOOK:
			{
				QString db_type;
				db_type.setNum(dbType);

				removeQuery = gRemoveSelectedDataOfBDAddressFromSQLiteDBQuery.arg(bd_address,db_type);
			}
				break;
			case DB_COMBINED_CALLS:
				break;
			case DB_CALL_HISTORY:
				removeQuery = gRemoveAllCallHistoryOfBDAddressFromSQLiteDBQuery.arg(bd_address);
				break;
			case DB_ALL_TYPE:
				removeQuery = gRemoveAllDataByBDAddressFromSQLiteDBQuery.arg(bd_address);
				break;
			default:
				break;
		}        
    }
	QSqlQuery query(QSqlDatabase::database("PhoneDb"));

	LOG_TRACE << "query is: " << removeQuery << LOG_ENDL;
	ret = query.exec(removeQuery);

	return ret;
}

bool CTrackerDataUpdater::RemoveContactDataByNameFromSQLiteDB(QString bd_address,
                                                              QString first_name,BT_PBAP_DB_MEMORY memoryType)
{
    bool ret = false;

    if(!m_IsContactDBCreated)
    {
        CreateSqlConnectionForContacts();
        m_IsContactDBCreated = true;
    }

    if(first_name.isEmpty() || bd_address.isEmpty())
        return ret;

    QSqlQuery query(QSqlDatabase::database("PhoneDb"));

    QString memory_Type;
    memory_Type.setNum(memoryType);

    QString removeQuery;
    removeQuery = gRemoveContactDataByNameFromSQLiteDBQuery.arg(bd_address,
                                                                first_name, memory_Type);
    LOG_TRACE << "query is: " << removeQuery << LOG_ENDL;
    ret = query.exec(removeQuery);

    return ret;
}

bool CTrackerDataUpdater::RemoveContactDataByIndexFromSQLiteDB(QString bd_address,
                                                               qint16 index, BT_PBAP_DB_MEMORY memory_Type)
{
    bool ret = false;

    if(!m_IsContactDBCreated)
    {
        CreateSqlConnectionForContacts();
        m_IsContactDBCreated = true;
    }

    if(bd_address.isEmpty())
        return ret;

    QSqlQuery query(QSqlDatabase::database("PhoneDb"));

    QString indexs;
    indexs.setNum(index);

    QString memoryType;
    memoryType.setNum(memory_Type);

    QString removeQuery;
    removeQuery = gRemoveContactDataByIndexFromSQLiteDBQuery.arg(bd_address,
                                                                 indexs, memoryType);
    LOG_TRACE << "query is: " << removeQuery << LOG_ENDL;
    ret = query.exec(removeQuery);

    return ret;
}

