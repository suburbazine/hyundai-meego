/*
******************************************************************
*        COPYRIGHT (C) Teleca AB      *
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

#ifndef DHAVN_IPODDATAPROVIDER_H
#define DHAVN_IPODDATAPROVIDER_H

#include <QStringList>
#include <QVector>
#include "DHAVN_TrackerAbstractor_constants.h"

#include <DHAVN_LogUtil.h>

/**
   * forward declaration
   */
class QSparqlConnection;
class QSparqlQuery;
class QSparqlResult;
class QSqlDatabase;
class QSqlQuery;


/**
   * This class will handle the request from the CTrackerAbstractor class and
   * execute the ipod related sparql queries. This class exposes the signal that the CTrackerProvider
   * would be calling for any operation like receiving data
   */
class CIpodDataProvider : public QObject{
    Q_OBJECT

    USE_LOG_UTIL

public :

    /**
       * Default Constructor
       */
    CIpodDataProvider(QSparqlConnection* sparqlConnection);

    /**
       * Default Destructor
       */
    ~CIpodDataProvider();

    /**
       * Executes the query for retrieving ipodcast name list
       */
	   
	bool   GetAllPodcastsByTitle(enum ETrackerAbstarctor_VolumeType  volumeType);
   
    /**
       * Executes the query for retrieving audiobook name list
       */
    
	bool   GetAllAudiobooksByTitle(enum ETrackerAbstarctor_VolumeType  volumeType);
   
    /**
       * Executes the query for retrieving  chapter titile , url 
       */
   
    bool GetAllPodcastsEpisodeNames(QString podcastName,enum ETrackerAbstarctor_VolumeType  volumeType);
    
	 
    /**
       * Executes the query for retrieving  audiobook title , url 
       */ 
	 
	bool GetAllAudiobookChapterNames(QString AudiobookName,enum ETrackerAbstarctor_VolumeType  volumeType);
	
       bool GetVolumeQueryToAppend(ETrackerAbstarctor_VolumeType volumeType, QString& volumeQuery);
       bool setDeviceURL(ETrackerAbstarctor_VolumeType volumeType,QString DeviceId,QString MountPath);


 bool GetAllIPODAudiobookspodcastdata(ETrackerAbstarctor_Query_Ids queryId,QSqlDatabase IpodDB);

        bool GetSearchIPODdataByKeywordFromVolume(QString searchData,ETrackerAbstarctor_Query_Ids queryId,QSqlDatabase IpodDB);

public :
     /**
       * A offset value for specifed queries
       */
       int m_Query_Offset;
     /**
       * A limit value for specifed queries
       */
       int m_Query_Limit;
	
private slots:

    /**
       * Slot to get notification on query completion
       */
    void OnFinished();

signals:

    /**
       * Signal to notify CTrackerAbstractor class on query completion
       */
    void DataFetched(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId);

     void Error(int);
	 
private:
     void exec(QSparqlQuery* query);
     bool exec(QSqlQuery* query);
     bool senddata(int noOfColumns,ETrackerAbstarctor_Query_Ids queryId ,QString querrystring,QSqlDatabase ipoddb);
     void   HandleError(int nError);

private:

    /**
       * QSparqlConnection object to execute query
       */
    QSparqlConnection* m_pSparql_Connection;

    /**
       * QSparqlResult object to receive data on query completion
       */
    QSparqlResult* m_pQueryResult;
    QString m_VolumeName_forKeyword;
    QString m_AudioBookName;
    QString m_PodCastName;
	   /**
       * Boolean to identify result loop execution
       */
    bool m_FirstTime;
	
	/**
       * file path used for music metadata query
       */
    QString m_FilePath_MusicMetaData;

    QStringList m_DeviceId;
    QStringList m_DeviceURL;
    QStringList m_VolumeNameList;
    QString m_VolumeType;
    QString m_VolumeName_GetAllPodcastsByTitle;
    QString m_VolumeName_GetAllAudiobooksByTitle;
    QString m_VolumeName_GetAllPodcastsEpisodeNames;
    QString m_VolumeName_GetAllAudiobookChapterNames;
  
};

#endif // DHAVN_IPODDATAPROVIDER_H
