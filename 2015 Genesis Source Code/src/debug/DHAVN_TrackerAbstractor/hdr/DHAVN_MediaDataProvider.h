/*
******************************************************************
*        COPYRIGHT (C) Teleca AB      *
*----------------------------------------------------------------*
* MODULE     :  DHAVN_TrackerAbstractor                             *
*
* PROGRAMMER :  Chetan Nanda                                   *
* DATE       :  12 September 2011                                    *
* VERSION    :  1.0                                              *
*
*----------------------------------------------------------------*
*                                                                *
* MODULE SUMMARY : This file defines class which interact with    *
*                  Tracker database using spaql query to retrieve media related    *
*                  information  (e.g Artist, Album, Genre etc)    *
*                                                                *
*----------------------------------------------------------------*
*
* MODIFICATION RECORDS :                                         *
*
* VERSION   DATE                     PROGRAMMER              COMMENT
******************************************************************
* 1.0       12 September 2011  Chetan Nanda           Draft version
******************************************************************
*/

#ifndef DHAVN_MEDIADATAPROVIDER_H
#define DHAVN_MEDIADATAPROVIDER_H

#include <QStringList>
#include <QVector>
#include "DHAVN_TrackerAbstractor_constants.h"

#include <DHAVN_LogUtil.h>

#ifndef MULTIPLE_DEVICE_SUPPORT
#define MULTIPLE_DEVICE_SUPPORT
#endif

#define PINYIN_TABLE_SIZE 20992 //added by jaehwan to apply pinyin sort for chiness 2014.01.08

/**
   * forward declaration
   */
class QSparqlConnection;
class QSparqlResult;
class QSparqlQuery;
class QSqlDatabase;
class QSqlQuery;


/**
   * This class will handle the request from the CTrackerAbstractor class and
   * execute the media related sparql queries. This class exposes the signal that the CTrackerProvider
   * would be calling for any operation like receiving data
   */
class CMediaDataProvider : public QObject{
    Q_OBJECT

    //USE_LOG_UTIL

public :

    /**
       * Default Constructor
       */
    CMediaDataProvider(QSparqlConnection* sparqlConnection);

    /**
       * Default Destructor
       */
    ~CMediaDataProvider();

    /**
       * Executes the query for retrieving songs from folder
       */
    bool GetSongsByFolder(QString folderName);

    /**
       * Executes the query for retrieving artists from folder
       */
    bool GetArtistByFolder(QString folderName);

    /**
       * Executes the query for retrieving albums from folder
       */
    bool GetAlbumsByFolder(QString folderName);

    /**
       * Executes the query for retrieving sub folder list from root folder
       */
    bool GetFolderList(QString folderName);

    /**
       * Executes the query for retrieving songs from folder
       */
//    bool GetSongsByNextFolder(QString folderName, ETrackerAbstarctor_Query_Ids queryId);

    /**
       * Executes the query for retrieving songs from folder
       */
//    bool GetSongsByPrevFolder(QString folderName, ETrackerAbstarctor_Query_Ids queryId);

    /**
       * Executes the query for retrieving songs from album
       */
//    bool GetSongsByNextAlbum(QString albumName, ETrackerAbstarctor_Query_Ids queryId);

    /**
       * Executes the query for retrieving songs from album
       */
//    bool GetSongsByPrevAlbum(QString albumName, ETrackerAbstarctor_Query_Ids queryId);

    /**
       * Executes the query for retrieving all songs
       */
    bool GetAllSongs();

    /**
       * Executes the query for retrieving all albums
       */
    bool GetAllAlbums();

    /**
       * Executes the query for retrieving all music artists
       */
    bool GetAllMusicArtists();

    /**
       * Executes the query for retrieving all music composers
       */
    bool GetAllMusicComposers();

    /**
       * Executes the query for retrieving all music genres
       */
    bool GetAllMusicGenres();

    /**
       * Executes the query for retrieving metadata (album,artist etc) from a song
       */
    bool GetMusicMetaData(QString filePath);

    /**
       * Executes the query for retrieving metadata (album,artist etc) from a list of URL's
       */
    bool GetMusicMetaDataInfoFromUrlList(QStringList filePathUrl);

    /**
       * Executes the query for retrieving songs info using given album name
       */
    bool GetAllSongsByAlbum(QString albumName);

    /**
       * Executes the query for retrieving songs info using given artist name
       */
    bool GetAllSongsByArtist(QString artistName);

    /**
       * Executes the query for retrieving songs info using given genre name
       */
    bool GetAllSongsByGenre(QString genreName);

    /**
       * Executes the query for retrieving songs info using given composer name
       */
    bool GetAllSongsByComposer(QString composerName);

    /**
       * Executes the query for retrieving images from folder
       */
    bool GetImagesByFolder(QString folderName);

    /**
       * Executes the query for retrieving videos from folder
       */
    bool GetVideosByFolder(QString folderName);

    /**
       * Executes the query for retrieving music info using given keyword
       */
    bool GetAllMusicByKeyword(QString keyword, QStringList volumeList, bool cdInsterted);
    /**
       * Executes the query for retrieving music info using given keyword
       */
   bool GetAllImagesByKeyword(QString keyword);
  /**
    * Executes the query for retrieving music info using given keyword
    */
     bool GetAllVideosByKeyword(QString keyword);


    /**
       * Executes the query for retrieving songs list fro all list of artists
       */
    bool GetMultipleArtistSongs(QStringList ArtistnamesList,QString folderName);
    /**
       * Executes the query for retrieving songs list fro all list of albums
       */
    bool GetMultipleAlbumSongs(QStringList AlbumnamesList,QString folderName);
	
    /**
       * Executes the query for retrieving songs count from folder
       */
    bool GetSongsCountByFolder(QString folderName);

    /**
       * Executes the query for retrieving images count from folder
       */
    bool GetImagesCountByFolder(QString folderName);

    /**
       * Executes the query for retrieving videos count from folder
       */
    bool GetVideosCountByFolder(QString folderName);

    /**
       * Executes the query for retrieving all songs count
       */
    bool GetAllSongsCount();

    /**
       * Executes the query for retrieving all songs from given volume
       */
    bool GetAllSongsFromVolume(enum ETrackerAbstarctor_VolumeType  volumeType);



    bool IpodRequestData(ETrackerAbstarctor_Query_Ids queryId,QSqlDatabase ipoddb, ETrackerAbstarctor_OrderType = eAlbum);
    bool IpodRequestsearchData(QStringList searchList,ETrackerAbstarctor_Query_Ids queryId,QSqlDatabase ipoddb);
    bool GetSearchdataByKeywordFromVolume(QString searchData,ETrackerAbstarctor_Query_Ids queryId,QSqlDatabase ipoddb);






    /**
       * Executes the query for retrieving all albums from given volume
       */
    bool GetAllAlbumsFromVolume(enum ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving all artists from given volume
       */
    bool GetAllMusicArtistsFromVolume(enum ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving all composers from given volume
       */
    bool GetAllMusicComposersFromVolume(enum ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving all genres from given volume
       */
    bool GetAllMusicGenresFromVolume(enum ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving all images from given volume
       */
    bool GetAllImagesFromVolume(enum ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving all videos from given volume
       */
    bool GetAllVideosFromVolume(enum ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving all songs with url, title and Ids
       */
    bool GetAllSongsIDs();
	
    /**
       * Executes the query for retrieving all songs with metada from given volume
       */
    bool GetAllSongsWithMetaDataFromVolume(ETrackerAbstarctor_VolumeType  volumeType,
                                                                     ETrackerAbstarctor_OrderType orderType);

    /**
       * Executes the query for retrieving all songs by genre artist album
       */
    bool GetAllSongsByGenreArtistAlbum(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving all songs by genre composer album
       */
    bool GetAllSongsByGenreComposerAlbum(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving all songs by Artist album
       */
    bool GetAllSongsByArtistAlbum(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving all albums by genre
       */
    bool GetMusicAlbumsByGenreArtist(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving all album by artist
       */
    bool GetMusicAlbumsByArtist(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving all artist by genre
       */
    bool GetMusicArtistsByGenre(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType);


    /**
       * Executes the query for retrieving all songs By Keyword for list view
       */
    bool GetAllSongsByKeywordFromVolume(QString searchdata, ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving all artist By Keyword for list view
       */
    bool GetAllArtistsByKeywordFromVolume(QString searchdata, ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving all songs By Album for list view
       */
    bool GetAllSongsByAlbumFromVolume(QString searchdata, ETrackerAbstarctor_VolumeType volumeType);

    /**
       * Executes the query for retrieving all albums By Keyword for list view
       */
    bool GetAllAlbumsByKeywordFromVolume(QString searchdata, ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving all Genre By Keyword for list view
       */
    bool GetAllGenresByKeywordFromVolume(QString searchdata, ETrackerAbstarctor_VolumeType  volumeType);

       /** Gets the Playlist Entries (file paths) present in the Playlist in Tracker
        * which matches the playlist title.
        */

        bool GetAllSongsbyPlaylist(QString playlistCriteria,enum ETrackerAbstarctor_VolumeType  volumeType);

            /**
           * Executes the query for retrieving all listofPlaylists
           */
        bool GetlistofPlaylists(enum ETrackerAbstarctor_VolumeType  volumeType);

        /**
           * Executes the query for retrieving all MostPlayedMusicSongs
           */
        bool GetMostPlayedMusicSongs(enum ETrackerAbstarctor_VolumeType  volumeType);

        /**
           * Executes the query for retrieving MostRecentlyPlayedSongs
           */
        bool GetMostRecentlyPlayedSongs(enum ETrackerAbstarctor_VolumeType  volumeType);

        /**
           * Executes the query for retrieving Music playlist based on Keyword.
           */
        bool GetMusicPlaylistByKeyword(enum ETrackerAbstarctor_VolumeType  volumeType,QString Keyword);

    /**
       * Executes the query for retrieving all videos count from a specified volume
       */
    bool GetAllVideosCountFromVolume(ETrackerAbstarctor_VolumeType volumeType);

    /**
       * Executes the query for retrieving all images count from a specified volume
       */
    bool GetAllImagesCountFromVolume(ETrackerAbstarctor_VolumeType volumeType);
	
	  /**
       * Executes the query for retrieving songs from CD
       */
    bool GetMusicInfoFromCD(QString cdTOC);
    /**
       * Executes the query for retrieving info about a song from CD
       */
    bool GetMusicMetaDataInfoFromCD(QString cdTOC, QString trackNum);

    /**
           * Executes the query for retrieving all songs by Composer album
           */
    bool  GetAllSongsByComposerAlbum(QStringList searchList,ETrackerAbstarctor_VolumeType                                    volumeType);

    /**
           * Executes the query for retrieving all albums by GenreComposer
           */

    bool GetMusicAlbumsByGenreComposer(QStringList searchList,ETrackerAbstarctor_VolumeType
                                       volumeType);

    /**
           * Executes the query for retrieving all albums by Composer
           */
    bool GetMusicAblumsByComposer(QStringList searchList, ETrackerAbstarctor_VolumeType                                        volumeType) ;


    /**
           * Executes the query for retrieving all composers by genre
           */
    bool GetMusicComposersByGenre(QStringList searchList,ETrackerAbstarctor_VolumeType                                         volumeType);
	/*
      * Execute the query to extract the data for the specific index from IPOD
      */
    bool GetMusicTracksBetweenIndexFromIpod(int startIndex, int endIndex);

    bool GetImagesWithSubFolder(QString folderName);

    bool GetVideosWithSubFolder(QString folderName);

    bool GetAllAlbumsWithCoverArtFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    /**
      * Executes the query for retrieving all songs without meta information from a specified volume
      */
    bool GetAllSongsWithoutMetaInfoFromVolume(ETrackerAbstarctor_VolumeType volumeType);

    /**
      * Executes the query for verify whether without metadata songs are present or not
      */
    bool IsThereAnySongWithoutAlbumFromVolume(ETrackerAbstarctor_VolumeType volumeType);

    /**
       * Executes the query for retrieving music info using given keyword from a specified volume
       */
    bool GetAllMusicByKeywordFromVolume(QString keyword, ETrackerAbstarctor_VolumeType volumeType);

    /**
      * Remove All media Data from volume */
    bool RemoveAllMediaData(ETrackerAbstarctor_VolumeType volumeType);
    /* API's to support Multiple Device support */

   // bool setDeviceId(ETrackerAbstarctor_VolumeType volumeType,QString DeviceId);

    bool setDeviceURL(ETrackerAbstarctor_VolumeType volumeType,QString DeviceId,QString MountPath);

    QString getDeviceURL(ETrackerAbstarctor_VolumeType volumeType);
    bool GetAllAlbumsWithUrlFromVolume(ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving song info using given artist from a specified volume
       */
    bool GetAllSongsByArtistFromVolume(QString artistName, ETrackerAbstarctor_VolumeType  volumeType);
	/**
       * Executes the query for retrieving song info using given genre from a specified volume
       */
    bool GetAllSongsByGenreFromVolume(QString generName, ETrackerAbstarctor_VolumeType  volumeType); //added by eugene.seo 2013.02.13
    /**
       * Executes the query to get count of songs from a specified volume
       */
	bool GetAllSongsCountFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    /**
       * Executes the query for retrieving count of all songs with title from a specified volume
       */
	bool GetAllSongsWithTitleCountFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    /**
       * Executes the query for retrieving count of all albums from a specified volume
       */
	bool GetAllAlbumsCountFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    /**
       * Executes the query for retrieving count of all artitsts from a specified volume
       */
	bool GetAllMusicArtistsCountFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    /**
       * Executes the query for retrieving count of all composers from a specified volume
       */
	bool GetAllMusicComposersCountFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    /**
       * Executes the query for retrieving count of all genres from a specified volume
       */
	bool GetAllMusicGenresCountFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    /**
       * Executes the query for retrieving count of all albums with corverart from a specified volume
       */
	bool GetAllAlbumsWithCoverArtCountFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    /**
       * Executes the query for retrieving the songs of limited number from a specified volume
       */
    bool GetAllSongsWithLimitFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    /**
       * Executes the query for retrieving the albums of limited number from a specified volume
       */
    bool GetAllAlbumsWithLimitFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    /**
       * Executes the query for retrieving the artists of limited number from a specified volume
       */
    bool GetAllMusicArtistsWithLimitFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    /**
       * Executes the query for retrieving the composers of limited number from a specified volume
       */
    bool GetAllMusicComposersWithLimitFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    /**
       * Executes the query for retrieving the genres of limited number from a specified volume
       */
    bool GetAllMusicGenresWithLimitFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    /**
       * Executes the query for retrieving the albums with coverart of limited number from a specified volume
       */
    bool GetAllAlbumsWithCoverArtWithLimitFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    /**
       * Executes the query for retrieving a index from song list using given letter from a specified volume
       */
    bool GetAllSongsWithTitleIndexFromVolume(QString letter, ETrackerAbstarctor_VolumeType  volumeType);
    /**
       * Executes the query for retrieving a index from artists list using given letter from a specified volume
       */
    bool GetAllMusicArtistsIndexFromVolume(QString letter, ETrackerAbstarctor_VolumeType  volumeType);
    /**
       * Executes the query for retrieving a index from genres list using given letter from a specified volume
       */
    bool GetAllMusicGenresIndexFromVolume(QString letter, ETrackerAbstarctor_VolumeType  volumeType);
    /**
       * Executes the query for retrieving a index from albums with coverart list using given letter from a specified volume
       */
    bool GetAllAlbumsWithCoverArtIndexFromVolume(QString letter, ETrackerAbstarctor_VolumeType  volumeType);
     
	    /**
       * Executes the query for retrieving song info using given artist from given alubms for specified volume
       */
    bool GetMultipleAlbumSongsByArtistFromVolume(QStringList albumNames, QString artistName,
                                                                             ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving song info using given genres for specified volume
       */
    bool GetMultipleGenreSongsFromVolume(QStringList genreList, ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving song info using given genre from given artist name for specified volume
       */
    bool GetMultipleArtistSongsByGenreFromVolume(QStringList artistNameList, QString genre,
                                                                             ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving song info using given genre and artist from given albume for specified volume
       */
    bool GetMultipleAlbumSongsByGenreArtistFromVolume(QStringList albumNameList,
                                                                                      QString genre, QString artist,
                                                                                       ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving song info using given folder paths.
       */
    bool GetMultipleFolderSongs(QStringList folderPathList);


    /**
       * Executes the query for retrieving songs list fro all list of artists
       */
    bool GetMultipleArtistSongsFromVolume(QStringList AlbumnamesList,
                                                                ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * Executes the query for retrieving songs list fro all list of albums
       */
    bool GetMultipleAlbumSongsFromVolume(QStringList AlbumnamesList,
                                                                ETrackerAbstarctor_VolumeType  volumeType);

    /**
       * retrieves the music folder/file list used by music player folder tab list
       */
    bool GetFolderTabList(QString folderPath);


    void SetIPodDBConnectionList(QList<QSqlDatabase> ipodDBConnList);

    bool GetAllMusicByKeywordForVR(QString keyword);

    bool GetIndexBySongFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType);
    bool GetIndexByAlbumFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType);
    bool GetIndexByArtistFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType);

    bool GetIndexWithCountBySongFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType, bool bEnableCacahe = false);  // { added by sungha.choi 2013.07.23
    bool GetIndexWithCountByAlbumFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType, bool bEnableCacahe = false);
    bool GetIndexWithCountByArtistFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType, bool bEnableCacahe = false);
    bool GetAllSongsWithLimitCacheFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllAlbumsWithCoverArtWithLimitCacheFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllMusicArtistsWithLimitCacheFromVolume(ETrackerAbstarctor_VolumeType  volumeType);    // } added by sungha.choi 2013.07.23
    void DeleteLimitCache(ETrackerAbstarctor_VolumeType  volumeType);   // added by sungha.choi 2013.07.30

    bool GetIndexDataFromVolumeForIPOD(QString searchData, ETrackerAbstarctor_Query_Ids queryId, QSqlDatabase ipodDb);
    bool GetAllMetadataWithLimitFromVolume(ETrackerAbstarctor_VolumeType  volumeType,
                                           ETrackerAbstarctor_OrderType orderType);
										   
    bool GetMediaFileListByFolder(QString folderPath, ETrackerAbstarctor_Query_Ids queryId);
    bool GetMediaFileListWithSubFolder(QString folderPath, ETrackerAbstarctor_Query_Ids queryId);
    bool GetAllMusicByConnectedVolumes(QStringList volumeList);

    bool RequestTrackerMetadataExtraction(QString& filePath);
	bool GetPhotoMetaDataInfoFromUrl(QString& url); // added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
    QString getDeviceID(ETrackerAbstarctor_VolumeType volumeType); // Added by Neelima 2013.01.03 , for CR13965

    bool GetMusicSimpleMetaData(QString filePath); //added by junam 2013.03.27 for ISV72425
    QString GetUSBMountPath(); // added by eugene.seo 2013.05.13
    bool GetMusicNormalMetaData(QString filePath);   // added by sungha.choi 2013.09.06 for ITS 0188361

private:
    void exec(QSparqlQuery* query);
    bool exec(QSqlQuery* query);

/**
      * Executes the query for retrieving the all songs count from the IPOD
      */
     bool GetAllSongsCountFromIPOD_sync(QSqlDatabase ipoddb, QString& Count);
    /**
      * Executes the query for retrieving the all songs count from the IPOD
      */
     bool  GetAllSongsCountWithMetaInfoFromIPOD_sync(QSqlDatabase ipoddb, QString& Count);
     /**
       * Executes the query for retrieving the all songs Without Album from the IPOD
       */

  bool IsThereAnySongWithoutAlbumFromIPOD(QSqlDatabase ipoddb);

    /**
       * Prepare a index number with a given letter from a specified volume
       */
        void PrepareIndexDataList(void* result, QVector<QStringList> queryDataList,
                                  ETrackerAbstarctor_Query_Ids queryId, QString letter, bool isIPod);

     bool senddata(int noOfColumns,ETrackerAbstarctor_Query_Ids queryId ,QString querrystring,QSqlDatabase ipoddb);
     bool senddata(int noOfColumns,QString querrystring,QSqlDatabase ipoddb, QVector<QStringList> &queryDataList);
    QString GetMusicUrlAppendedFromUrlList(QStringList urlList);
    QString GetMusicUrlAppended(QString url); // added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
    bool  limitsenddata(int noOfColumns,ETrackerAbstarctor_Query_Ids queryId ,QString querrystring,QSqlDatabase ipoddb);
    bool sendIndexData(ETrackerAbstarctor_Query_Ids queryId, QString querrystring,QSqlDatabase ipoddb );
    bool sendIndexDataForBadFile(ETrackerAbstarctor_Query_Ids queryId, QString querrystring,QSqlDatabase ipoddb ); //added by junam 2013.06.10 for UID no play icon

    /**
       * Handle special character in the music keyword seach
       */
    void CheckForSpecialCharacters(QString &keyword);

    /**
      * Executes the query for retrieving count of all songs from a specified volume
      */
    bool GetAllSongsCountFromVolume_sync(ETrackerAbstarctor_VolumeType volumeType, QString& Count);
    /**
      * Executes the query for retrieving count of all songs with meta information from a specified volume
      */
    bool GetSongsCountWithMetaInfoFromVolume_sync(ETrackerAbstarctor_VolumeType volumeType, QString& Count);

    /**
       * Executes the query for retrieving songs of given album
       */
    bool GetSongsByAlbum(QString albumName);
    
	/**
	  * Modify the filename paths got from the Tracker
	  */
//    QString ModifyFileName(QString originalFileName);

	/**
	* Modify data list for images, remove sub folder entries for images
	*/
    void ModifyDataForImages(QVector<QStringList>& dataList);

	/**
	* Modify data list for images, remove sub folder entries for videos
	*/
    void ModifyDataForVideos(QVector<QStringList>& dataList);

    /**
      * prepares the volume list.
      */
    bool PrepareVolumeList();

    /**
      * Update the volume query in the Argument (volumeQuery)
      */
    bool GetVolumeQueryToAppend(ETrackerAbstarctor_VolumeType volumeType, QString& volumeQuery);
    bool GetExtendOrderingQueryToAppend(int lang, QString colName, QString& orderQuery); 
    //added by jaehwan 2013.11.27 to fix ITS 210480
    bool GetExtendOrderingQueryToAppend2(int lang, QString colName1, QString colName2, QString& orderQuery);

    void CheckIfFileExists(QVector<QStringList> &dataList, int column);

    void SendCDData(QString queryString,  int noOfColumns, ETrackerAbstarctor_Query_Ids queryId);


    QString getOrderByMethod(QString item);  //added by junam 2013.05.23 for sorting

private slots:

    /**
       * Slot to get notification on query completion
       */
    void OnFinished();
    /**
       * Slot to get notification on query completion for dynamic fetching feature
       */
    void OnLimitFinished();
    void OnLimitCacheFinished();    // added by sungha.choi 2013.07.23

    void OnIndexFinished();
    void OnIndexWithCountFinished();    // added by sungha.choi 2013.07.23

signals:

    /**
       * Signal to notify CTrackerAbstractor class on query completion
       */
    void DataFetched(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId);
    /**
       * Signal to notify CTrackerAbstractor class on query completion for dynamic fetching feature
       */
    void DataLimitFetched(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId, int offset);

     void Error(int);

public :
     /**
       * A offset value for specifed queries
       */
       int m_Query_Offset;
       /**
       * A limit value for specifed queries
       */
       int m_Query_Limit;

private:

	void   HandleError(int nError);

private:

    QStringList m_DeviceId;
    QStringList m_DeviceURL;

    /**
       * QSparqlConnection object to execute query
       */
    QSparqlConnection* m_pSparql_Connection;

    /**
       * QSparqlResult object to receive data on query completion
       */
    QSparqlResult* m_pQueryResult;

    /**
       * folder name used for song by folder query
       */
    QString m_FolderName_SongByFolder;

    /**
       * folder name used for album by folder query
       */
    QString m_FolderName_AlbumByFolder;

    /**
       * folder name used for artist by folder query
       */
    QString m_FolderName_ArtistByFolder;

    /**
       * folder name used for folder list query
       */
    QString m_FolderName_FolderList;

    /**
       * folder name used for album list by folder query
       */
    QString m_FolderName_AlbumListByFolder;

    /**
       * album name used for songs by album query
       */
    QString m_AlbumName;

    /**
       * query id used for next and prev album queries
       */
    ETrackerAbstarctor_Query_Ids m_QueryId;

    /**
       * Boolean to identify result loop execution
       */
    bool m_FirstTime;

    /**
       * file path used for music metadata query
       */
    QString m_FilePath_MusicMetaData;

    /**
       * file path used for music metadata query
       */
    QString m_FilePathInfo_MusicMetaData;

    /**
       * album name used for all songs by album name query
       */
    QString m_AlbumName_AllSongsByAlbum;

    /**
       * artist name used for all songs by artist name query
       */
    QString m_ArtistName_AllSongsByArtist;

    /**
       * genre name used for all songs by genre name query
       */
    QString m_GenreName_AllSongsByGenre;

    /**
       * composer name used for all songs by composer name query
       */
    QString m_ComposerName_AllSongsByComposer;

    /**
       * folder name used for images by folder query
       */
    QString m_FolderName_ImagesByFolder;

    /**
       * folder name used for videos by folder query
       */
    QString m_FolderName_VideosByFolder;

    /**
       * keyword used for all music by keyword query
       */
    QString m_Keyword_AllMusicByKeyword;

    /**
       * keyword1 used for all music by keyword query
       */
    QString m_Keyword1_AllMusicByKeyword;

    /**
       * keyword used for all music for List of Artist  query
       */
    QString m_ArtistName_GetMultipleArtistSongs;

    /**
       * keyword used for all music for list of albums query
       */
    QString m_AlbumName_GetMultipleAlbumSongs;

    /**
       * keyword used for all Images by keyword query
       */
    QString m_Keyword_AllImagesByKeyword;

    /**
       * keyword used for all Images by keyword query
       */
    QString m_Keyword1_AllImagesByKeyword;

    /**
       * keyword used for all Videos by keyword query
       */
    QString m_Keyword_AllVidoesByKeyword;

    /**
       * keyword used for all Videos by keyword query
       */
    QString m_Keyword1_AllVidoesByKeyword;

    /**
       * keyword used for all Documents by keyword query
       */
    QString m_Keyword_AllDocumentsByKeyword;
    /**
       * keyword used for all Apps by keyword query
       */
    QString m_Keyword_AllAppsByKeyword;

    /**
       * keyword used for all Contacts by keyword query
       */
    QString m_Keyword_AllContactsByKeyword;
	
    /**
       * folder name used for song by folder query
       */
    QString m_FolderName_SongsCountByFolder;

    /**
       * folder name used for images by folder query
       */
    QString m_FolderName_ImagesCountByFolder;

    /**
       * folder name used for videos by folder query
       */
    QString m_FolderName_VideosCountByFolder;

    /**
       * keyword used for get all songs from volume query
       */
    QString m_VolumeName_GetAllSongsFromVolume;

    /**
       * keyword used for get all songs from volume query
       */
    QString m_VolumeName_GetAllSongsByGenreComposerAlbum;

    /**
       * keyword used for get all songs by genre artist album from volume query
       */
    QString m_VolumeName_GetAllSongsByGenreArtistAlbum;

    /**
       * keyword used for get all songs with metadata from volume query
       */
    QString m_VolumeName_GetAllSongsWithMetaDataFromVolume;

    /**
       * volume name used for volume queries
       */
    QStringList m_VolumeNameList;

    /**
       * volume type used for volume queries
       */
    ETrackerAbstarctor_VolumeType m_VolumeType;

    /**
       * keyword used for get all albums from volume query
       */
    QString m_VolumeName_GetAllAlbumsFromVolume;

    /**
       * keyword used for get all artists from volume query
       */
    QString m_VolumeName_GetAllMusicArtistsFromVolume;

    /**
       * keyword used for get all composers from volume query
       */
    QString m_VolumeName_GetAllMusicComposersFromVolume;

    /**
       * keyword used for get all genres from volume query
       */
    QString m_VolumeName_GetAllMusicGenresFromVolume;

    /**
       * keyword used for get all images from volume query
       */
    QString m_VolumeName_GetAllImagesFromVolume;

    /**
       * keyword used for get all videos from volume query
       */
    QString m_VolumeName_GetAllVideosFromVolume;

    /**
       * order type used for all songs with metadata volume query
       */
    ETrackerAbstarctor_OrderType  m_OrderType;

    /**
       * order used for get all songs with metadata from volume query
       */
    QString m_OrderBy_GetAllSongsFromVolume;

    /**
       * Genre used for get all songs by genre artist album query
       */
    QString m_Genre_GetAllSongsByGenreArtistAlbum;

    /**
       * Artist name used for get all songs by genre artist album query
       */
    QString m_Artist_GetAllSongsByGenreArtistAlbum;

    /**
       * Album name used for get all songs by genre artist album query
       */
    QString m_Album_GetAllSongsByGenreArtistAlbum;

    /**
       * Genre used for get all songs by genre composer album query
       */
    QString m_Genre_GetAllSongsByGenreComposerAlbum;

    /**
       * Composer name used for get all songs by genre composer album query
       */
    QString m_Composer_GetAllSongsByGenreComposerAlbum;

    /**
       * Album name used for get all songs by genre composer album query
       */
    QString m_Album_GetAllSongsByGenreComposerAlbum;

    /**
       * Album name used for GetAllSongsByArtistAlbum
       */
    QString m_Album_GetAllSongsByArtistAlbum;

    /**
       * Artist name used for GetAllSongsByArtistAlbum
       */
    QString m_Artist_GetAllSongsByArtistAlbum;

    /**
       * VolumeName name used for GetAllSongsByArtistAlbum
       */
    QString m_VolumeName_GetAllSongsByArtistAlbum;

    /**
       * Genre name used for GetAllAlbumByGenreArtist
       */
    QString m_Genre_GetAllAlbumByGenreArtist;

    /**
       * Artist name used for GetAllAlbumByGenreArtist
       */
    QString m_Artist_GetAllAlbumByGenreArtist;

    /**
       * VolumeName name used for GetAllAlbumByGenreArtist
       */
    QString m_VolumeName_GetAllAlbumByGenreArtist;

    /**
       * Artist name used for GetAllAlbumByArtist
       */
    QString m_Artist_GetAllAlbumByArtist;

    /**
       * VolumeName used for GetAllAlbumByArtist
       */
    QString m_VolumeName_GetAllAlbumByArtist;

    /**
       * Genre used for GetAllArtistByGenre
       */
    QString m_Genre_GetAllArtistByGenre;

    /**
       * VolumeName name used for GetAllArtistByGenre
       */
    QString m_VolumeName_GetAllArtistByGenre;

   /**
       * SongKeyword name used for GetAllsongsByKeywordFromVolume
       */
    QString m_SongKeyword_GetAllsongsByKeywordFromVolume;
    QString m_SongKeyword_EucKR_GetAllsongsByKeywordFromVolume;

    /**
       * ArtistKeyword name used for GetAllArtistsByKeywordFromVolume
       */
    QString m_ArtistKeyword_GetAllArtistsByKeywordFromVolume;
    QString  m_ArtistKeyword_EucKR_GetAllArtistsByKeywordFromVolume;


    /**
       * ArtistKeyword name used for GetAllArtistsByKeywordFromVolume
       */
    QString m_AlbumKeyword_GetAllSongsByAlbumFromVolume;

    /**
       * volume name used for GetAllArtistsByKeywordFromVolume
       */
    QString m_VolumeName_GetAllSongsByAlbumFromVolume;

    /**
       * AlbumKeyword name used for GetAllAlbumsByKeywordFromVolume
       */
    QString m_AlbumKeyword_GetAllAlbumsByKeywordFromVolume;
    QString m_AlbumKeyword_EucKR_GetAllAlbumsByKeywordFromVolume;

    /**
       * GenreKeyword name used for GetAllGenresByKeywordFromVolume
       */
    QString m_GenreKeyword_GetAllGenresByKeywordFromVolume;
    QString m_GenreKeyword_EucKR_GetAllGenresByKeywordFromVolume;

    QString m_VolumeName_forKeyword;
    
     /**
       * Artist name used for GetAllAlbumByArtist
       */

    QString m_playlist_Getallsongsbyplaylist;
    /**
       * VolumeName name used for GetAllSongsbyPlaylist
       */

    QString m_VolumeName_GetAllSongsbyPlaylist;

     /**
       * VolumeName name used for GetlistofPlaylists
       */

    QString m_VolumeName_GetlistofPlaylists;

    /**
       * VolumeName name used for GetMostPlayedMusicSongs
       */

    QString m_VolumeName_GetMostPlayedMusicSongs;

    /**
     * VolumeName name used for GetMostRecentlyPlayedSongs
     */

      QString m_VolumeName_GetMostRecentlyPlayedSongs;

       /**
      * Keyword name used for MusicPlaylist
      */

   QString m_Keyword_MusicPlaylistByKeyword;

     /**
      * Volume name used for GetMusicPlaylistByKeyword
      */

   QString  m_VolumeName_GetMusicPlaylistByKeyword;
   
       /**
       * Volume string used for GetAllVideosCountFromVolume
       */
    QString m_VolumeStr_GetAllVideosCountFromVolume;
    /**
       *  Volume string used for GetAllImagesCountFromVolume
       */
    QString m_VolumeStr_GetAllImagesCountFromVolume;
	
    /**
       *  Volume string used for GetAllSongsCountFromVolume
       */
    QString m_VolumeName_GetAllSongsCountFromVolume;
    /**
       *  Volume string used for GetSongsCountWithMetaInfoFromVolume
       */
    QString m_VolumeName_GetSongsCountWithMetaInfoFromVolume;
    /**
       *  Volume string used for GetAllSongsWithoutMetaInfoFromVolume
       */
    QString m_VolumeName_GetAllSongsWithoutMetaInfoFromVolume;
	
    QString m_cd_toc;

       QString   m_Album_GetAllSongsByComposerAlbum;
       QString   m_Composer_GetAllSongsByComposerAlbum;
       QString   m_VolumeName_GetAllSongsByComposerAlbum;


     QString   m_Genre_GetMusicAlbumsByGenreComposer;
     QString   m_Composer_GetMusicAlbumsByGenreComposer;
     QString   m_VolumeName_GetMusicAlbumsByGenreComposer;



    QString   m_Genre_GetAllComposersByGenre;
    QString   m_VolumeName_GetAllComposersByGenre;

    QString  m_VolumeName_GetAllAlbumByComposer ;
    QString  m_Composer_GetAllAlbumByComposer ;
   QString m_TOC_GetMusicMetaDataFromCD;
   QString m_TrackNum_GetMusicMetaDataFromCD;

    /**
       * TypeName used for storing Playlists
       * iPod is "Playlist", Gracenote is "MoreLikeThis"
       */

    QString m_playlist_TypeName;
    /**
      *StartIndex of the ipod music Data search
      */
        int m_StartIndex;

        /**
          *EndIndex of the ipod music Data search
          */
        int m_EndIndex;

        QString m_FolderName_ImagesWithSubFolder;
        QString m_FolderName_VideosWithSubFolder;
    QString m_VolumeName_GetAllAlbumsWithCoverArt;

    /**
       * keyword used for all music by keyword query from volume
       */
    QString m_Keyword_GetAllMusicByKeywordFromVolume;

    /**
       * keyword1 used for all music by keyword query from volume
       */
    QString m_Keyword1_GetAllMusicByKeywordFromVolume;

    /**
       * VolumeName name used for GetAllMusicByKeywordFromVolume
       */
    QString m_VolumeName_GetAllMusicByKeywordFromVolume;

    QString m_VolumeName_RemoveMediaData;

    QString m_VolumeName_GetAllAlbumsWithUrl;

    /**
       *  Volume string used for GetAllSongsByArtistFromVolume
       */
    QString m_VolumeName_GetAllSongsByArtistFromVolume;

    /**
       * artist name used for all songs by artist name for a specified volume query
       */
    QString m_ArtistName_GetAllSongsByArtistFromVolume;

     // { added by eugene.seo 2013.02.13
    /**
     *  Volume string used for GetAllSongsByGenreFromVolume
       */
    QString m_VolumeName_GetAllSongsByGenreFromVolume;

    /**
       * genre name used for all songs by genre name for a specified volume query
       */
    QString m_GenreName_GetAllSongsByGenreFromVolume;
    // } added by eugene.seo 2013.02.13

    /**	
       * VolumeName name used for GetAllSongsWithTitleCountFromVolume
       */
    QString m_VolumeName_GetAllSongsWithTitleCountFromVolume;
    /**
       * VolumeName name used for GetAllAlbumsCountFromVolume
       */	
    QString m_VolumeName_GetAllAlbumsCountFromVolume;
    /**
       * VolumeName name used for GetAllMusicArtistsCountFromVolume
       */		
    QString m_VolumeName_GetAllMusicArtistsCountFromVolume;
    /**
       * VolumeName name used for GetAllMusicComposersCountFromVolume
       */	
    QString m_VolumeName_GetAllMusicComposersCountFromVolume;
    /**
       * VolumeName name used for GetAllMusicGenresCountFromVolume
       */		
    QString m_VolumeName_GetAllMusicGenresCountFromVolume;
    /**
       * VolumeName name used for GetAllAlbumsWithCoverArtCountFromVolume
       */	
	QString m_VolumeName_GetAllAlbumsWithCoverArtCountFromVolume;
    /**
       * VolumeName name used for GetAllSongsWithLimitFromVolume
       */	
    QString m_VolumeName_GetAllSongsWithLimitFromVolume;
    /**
       * VolumeName name used for GetAllAlbumsWithLimitFromVolume
       */
    QString m_VolumeName_GetAllAlbumsWithLimitFromVolume;
    /**
       * VolumeName name used for GetAllMusicArtistsWithLimitFromVolume
       */
    QString m_VolumeName_GetAllMusicArtistsWithLimitFromVolume;
    /**
       * VolumeName name used for GetAllMusicComposersWithLimitFromVolume
       */
    QString m_VolumeName_GetAllMusicComposersWithLimitFromVolume;
    /**
       * VolumeName name used for GetAllMusicGenresWithLimitFromVolume
       */
    QString m_VolumeName_GetAllMusicGenresWithLimitFromVolume;
    /**
       * VolumeName name used for GetAllAlbumsWithCoverArtWithLimitFromVolume
       */
    QString m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume;
    /**
       * VolumeName name used for GetAllSongsWithTitleIndexFromVolume
       */
    QString m_VolumeName_GetAllSongsWithTitleIndexFromVolume;
    /**
       * VolumeName name used for GetAllMusicArtistsIndexFromVolume
       */
    QString m_VolumeName_GetAllMusicArtistsIndexFromVolume;
    /**
       * VolumeName name used for GetAllMusicGenresIndexFromVolume
       */
    QString m_VolumeName_GetAllMusicGenresIndexFromVolume;
    /**
       * VolumeName name used for GetAllAlbumsWithCoverArtIndexFromVolume
       */
    QString m_VolumeName_GetAllAlbumsWithCoverArtIndexFromVolume;
    /**
       * VolumeName name used for GetAllSongsWithTitleIndexFromVolume
       */
    QString m_Letter_GetAllSongsWithTitleIndexFromVolume;
    /**
       * VolumeName name used for GetAllMusicArtistsIndexFromVolume
       */
    QString m_Letter_GetAllMusicArtistsIndexFromVolume;
    /**
       * VolumeName name used for GetAllMusicGenresIndexFromVolume
       */
    QString m_Letter_GetAllMusicGenresIndexFromVolume;
    /**
       * VolumeName name used for GetAllAlbumsWithCoverArtIndexFromVolume
       */
    QString m_Letter_GetAllAlbumsWithCoverArtIndexFromVolume;
	
	    /**
       * argument used for GetMultipleAlbumSongsByArtistFromVolume
       */
    QString m_Argument_GetMultipleAlbumSongsByArtistFromVolume;

    /**
       * argument used for GetMultipleGenreSongsFromVolume
       */
    QString m_Argument_GetMultipleGenreSongsFromVolume;

    /**
       * argument used for GetMultipleArtistSongsByGenreFromVolume
       */
    QString m_Argument_GetMultipleArtistSongsByGenreFromVolume;

    /**
       * argument used for GetMultipleAlbumSongsByGenreArtistFromVolume
       */
    QString m_Argument_GetMultipleAlbumSongsByGenreArtistFromVolume;

    /**
       * argument used for GetMultipleFolderSongs
       */
    QString m_Argument_GetMultipleFolderSongs;

    /**
       * argument used for GetMultipleAlbumSongsFromVolume
       */
    QString m_Argument_GetMultipleAlbumSongsFromVolume;

    /**
       * argument used for GetMultipleArtistSongsFromVolume
       */
    QString m_Argument_GetMultipleArtistSongsFromVolume;

    QList<QSqlDatabase> m_Ipod_DBConnList;

    /**
       * usb and cd path list for GetAllMusicByKeyword
       */
    QString m_PathList_AllMusicByKeyword;

    /**
       * keyword used for all music by keyword  for VRquery
       */
    QString m_Keyword_AllMusicByKeywordForVR;

    /**
       * keyword1 used for all music by keyword for VR query
       */
    QString m_Keyword1_AllMusicByKeywordForVR;

    /**
       * keyword used for music search for Ipod query
       */
    QString m_Keyword_AllMusicByKeywordForIPod;

    QString m_VolumeName_GetAllSongsFromVolumeForIndex;
    QString m_VolumeName_GetAllAlbumsFromVolumeForIndex;
    QString m_VolumeName_GetAllArtistsFromVolumeForIndex;
    QString m_SearchData_ForIndex;

    QString m_VolumeName_GetAllMetaDataWithLimitFromVolume;
    QString m_OrderBy_GetMetaDataFromVolume;

    QString m_ExtendOrdering;

    bool m_isCDInserted;

    /**
       * usb path list for GetAllMusicByConnectedVolumes
       */
    QString m_PathList_GetAllMusicByConnectedVolumes;
	QString m_strMountPath; // added by eugene.seo 2013.05.13

    QVector<QStringList> songIndexCountSavedList, artistIndexCountSavedList, albumIndexCountSavedList;   // { added by sungha.choi 2013.07.23
    QString savedSongSearchData, savedArtistSearchData, savedAlbumSearchData;
    ETrackerAbstarctor_VolumeType savedSongVolumeType, savedArtistVolumeType, savedAlbumVolumeType;
    QVector<QStringList> songDataSavedList, artistDataSavedList, albumDataSavedList;
    bool m_cacheSuccess;    // } added by sungha.choi 2013.07.23
    int m_lang;
    int GetLanguage() {return m_lang; }
public: 
    void SetLanguage(int lang); 
    //{ added by jaehwan 2014.01.08 for pinyin sorting
    static wchar_t pinyinTable[PINYIN_TABLE_SIZE];
    static void LoadPinyinTable();
    void reorderListForChina(QVector<QStringList> &fileList, int indexColumn);
    //} added by jaehwan

};

#endif // DHAVN_MEDIADATAPROVIDER_H
