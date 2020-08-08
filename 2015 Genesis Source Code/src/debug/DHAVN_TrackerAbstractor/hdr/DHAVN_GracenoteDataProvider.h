/*
******************************************************************
*        COPYRIGHT © Teleca AB      *
*----------------------------------------------------------------*
* MODULE     :  DHAVN_TrackerAbstractor                             *
*
* PROGRAMMER :  Elmy Elias                                  *
* DATE       :  18 November 2011                                    *
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
* 1.0       18 November 2011     Elmy Elias           Draft version
******************************************************************
*/

#ifndef DHAVN_GRACENOTEDATAPROVIDER_H
#define DHAVN_GRACENOTEDATAPROVIDER_H

#include <QStringList>
#include <QVector>
//#include "DHAVN_TrackerAbstractorLogger.h"
#include "DHAVN_TrackerAbstractor_constants.h"



/**
   * forward declaration
   */
class QSparqlConnection;
class QSparqlResult;
class QSqlDatabase;

/*enum EVolumeType
{
    e_JukeBox = 0,
    e_USB1,  //USB connected in front
    e_USB2,  //USB connected in rear
    e_Disc,
    e_IPOD1, //IPOD connected in front
    e_IPOD2, //IPOD connected in rear
    e_CA
};*/


/**
   * This class will handle the request from the CTrackerAbstractor class and
   * execute the SQL queries to fetch Gracenote metadata. This class exposes the signal that the CTrackerProvider
   * would be calling for any operation like receiving data
   */
class CGracenoteDataProvider : public QObject{
    Q_OBJECT

 //   USE_LOG_UTIL

public :

    /**
       * Default Constructor
       */
    CGracenoteDataProvider();

    /**
       * Default Destructor
       */
    ~CGracenoteDataProvider();

    bool GetAllSongsByComposerAlbum(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType); //Passed
    bool GetAllAlbumByGenreComposer(QStringList searchList, ETrackerAbstarctor_VolumeType volumeType); //Passed
    bool GetAllAlbumByComposer(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllComposerByGenre(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllSongsByGenreArtistAlbum(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllSongsByGenreComposerAlbum(QStringList searchList, ETrackerAbstarctor_VolumeType volumeType);
    bool GetAllSongsByArtistAlbum(QStringList searchList, ETrackerAbstarctor_VolumeType volumeType);
    bool GetMusicAlbumsByGenreArtist(QStringList searchList, ETrackerAbstarctor_VolumeType volumeType);
    bool GetMusicAlbumsByArtist(QStringList searchList, ETrackerAbstarctor_VolumeType volumeType);
    bool GetMusicArtistsByGenre(QStringList searchList, ETrackerAbstarctor_VolumeType volumeType);


    bool GetAllAlbumsWithCoverArtIndexFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType);
    bool GetAllSongsByArtistFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType);
    bool GetAllMusicGenresIndexFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType);


    bool GetAllSongsWithoutAlbumFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    bool GetAllsongsByKeywordFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllAlbumsByKeywordFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllArtistsByKeywordFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllGenresByKeywordFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllSongsByAlbumFromVolume(QString searchData,ETrackerAbstarctor_VolumeType  volumeType);

    bool GetSongIndexFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType);
    bool GetAlbumIndexFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType);
    bool GetArtistIndexFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType);

    bool GetAllMetaDataWithLimitFromVolume(ETrackerAbstarctor_OrderType orderType, ETrackerAbstarctor_VolumeType volumeType);
    bool GetAllMusicGenresFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllMusicComposersFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllMusicArtistsFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllAlbumsCountFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllAlbumsFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllAlbumsWithCoverArtFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllAlbumsWithCoverArtWithLimitFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    bool GetAllMusicGenresWithLimitFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    bool GetAllMusicComposersWithLimitFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    bool GetAllMusicArtistsWithLimitFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllAlbumsWithLimitFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllSongsWithLimitFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllSongsWithTitleCountFromVolume(ETrackerAbstarctor_VolumeType  volumeType);
    bool GetAllSongsFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    bool GetAllMusicArtistsCountFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    bool GetAllAlbumsWithUrlFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    bool GetAllMusicComposersCountFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    bool GetAllMusicGenresCountFromVolume(ETrackerAbstarctor_VolumeType volumeType);
    bool GetMusicMetaData(QString filePath);

    bool RequestData(QStringList searchList, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType);
    bool RequestData(ETrackerAbstarctor_OrderType orderType, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType);
    bool RequestData( ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType);
    bool RequestData(QString searchData, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType);
    bool GetAllMusicByKeyword(QString keyword, QStringList volumeList);

    bool GetAllSongsWithTitleIndexFromVolume();
    bool IsThereAnySongWithoutAlbumFromVolume(ETrackerAbstarctor_VolumeType volumeType);


    QString getDeviceID(ETrackerAbstarctor_VolumeType volumeType);
    bool setDeviceURL(ETrackerAbstarctor_VolumeType volumeType,QString DeviceId,QString MountPath);
    QString GetUSBMountPath(); // added by eugene.seo 2013.05.13

signals:
    /**
       * Signal to notify CTrackerAbstractor class on query completion
       */
    void DataFeched(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId);

    void Error(int);

private:
        void   HandleError(int nError);

         bool senddata(int noOfColumns,ETrackerAbstarctor_Query_Ids queryId ,QString querystring, QSqlDatabase &normalizedDB);
         bool sendIndexData(ETrackerAbstarctor_Query_Ids queryId, QString queryString,QSqlDatabase& normalizedDB);

         int GetCIDForVolume(ETrackerAbstarctor_VolumeType volumeType);
         bool CreateDBConnection(QSqlDatabase &normalized_DB);
		 bool CreateDBConnection(QSqlDatabase &sqlitedb, QString path, QString connectionName);

         bool GetAllSongsCountWithMetaInfoFromVolume(QSqlDatabase& normalized_DB, QString& Count, QString CID_DB);
         bool GetAllSongsCountFromVolume(QSqlDatabase& normalized_DB, QString& Count, QString CID_DB);

         void CheckIfFileExists(QVector<QStringList> &dataList, int column);
         void GetSearchData( QString keyword, ETrackerAbstarctor_VolumeType volumeType, QVector<QStringList> &queryDataList);
         bool GetMusicMetaDataFromFilePath(QString filePath, ETrackerAbstarctor_VolumeType volumeType, QVector<QStringList> &queryDataList);

public:
         int m_CID;
         int m_Query_Offset;
         int m_Query_Limit;
         QString m_Letter_GetAllAlbumsWithCoverArtIndexFromVolume;
         QString m_Letter_GetAllMusicArtistsIndexFromVolume;
         QString m_Letter_GetAllSongsWithTitleIndexFromVolume;
         QString m_SearchData_ForIndex;
         QString m_Letter_GetAllMusicGenresIndexFromVolume;
         bool m_isCDInserted;
         bool m_IPOD_FRONT;
         bool m_IPOD_REAR;
		 
         QStringList m_DeviceId;
         QStringList m_DeviceURL;		 
         QString m_strMountPath; // added by eugene.seo 2013.05.13
};

#endif // DHAVN_GRACENOTEDATAPROVIDER_H
