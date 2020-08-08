/*
******************************************************************
*        COPYRIGHT (C) Teleca AB      *
*----------------------------------------------------------------*
* MODULE     :  DHAVN_TrackerAbstractor                             *
*
* PROGRAMMER :  Elmy Elias                                 *
* DATE       :  18 November 2011                                        *
* VERSION    :  1.0                                              *
*
*----------------------------------------------------------------*
*                                                                *
* MODULE SUMMARY : This file defines Apis which interact with    *
*                  Tracker database using spaql query to retrieve media related  *
*                  information (e.g Artist, Album, Genre etc)    *
*                                                                *
*----------------------------------------------------------------*
*
* MODIFICATION RECORDS :                                         *
*
* VERSION   DATE                     PROGRAMMER              COMMENT
******************************************************************
* 1.0       18 November 2011  Elmy Elias              Draft version
******************************************************************
*/

#include "DHAVN_IpodDataProvider.h"
#include <QSparqlResult>
#include <QSparqlQuery>
#include <QSparqlConnection>
#include <QSparqlError>
#include <QUrl>
#include <QtSql>

CIpodDataProvider::CIpodDataProvider(QSparqlConnection* sparqlConnection)
{
    LOG_INIT_V2("Media_TrackerAbstractor"); //added by jurnam 2013.04.29 for remove warning
    m_pSparql_Connection = sparqlConnection;
     if (m_pSparql_Connection->isValid())
         LOG_TRACE << "DHAVN_TrackerAbstrator - CIpodDataProvider - Driver found" << LOG_ENDL;
     else
         LOG_TRACE << "DHAVN_TrackerAbstrator - CIpodDataProvider - Driver not found" << LOG_ENDL;

     m_pQueryResult = NULL;
     m_FirstTime  = false;

     /* Dummy initilization of QStringlist*/
     for(int i=0;i<=EVOLUME_TYPE_SIZE;i++)
     {
     m_DeviceId << "Empty" ;
     m_DeviceURL << "Empty";
     }
	 m_Query_Offset = QUERY_OFFSET;
	 m_Query_Limit  = QUERY_LIMIT;	 
}

CIpodDataProvider::~CIpodDataProvider()
{
    if(m_pQueryResult != NULL)
    {
        delete m_pQueryResult;
        m_pQueryResult = NULL;
    }
}

bool CIpodDataProvider::GetAllIPODAudiobookspodcastdata(ETrackerAbstarctor_Query_Ids queryId,QSqlDatabase IpodDB)
{

    QString Querrystring;
    int noOfColumns = 0;
    bool ret=false;
    QString Datatype;
   

    qDebug() << "in ipoddataprovider ::1";
    switch (queryId)
    {
    case eGetAllPodcastsByTitleQuery_Id:
    {
         qDebug() << "in GetAllPodcastsByTitle";
       

         Querrystring.clear();
         Datatype.clear();
         Datatype.append("Podcast");

         Querrystring.append(QString("select DISTINCT T_TopLevelName from MasterTable where T_TYPE = \"%1\" AND T_TopLevelName not like \"%2\" order by T_TopLevelName ").arg(Datatype).arg(""));

        noOfColumns = GETALLPODCASTSBYTITLEQUERY_DATATYPECOUNT;


    }
 break;

    case eGetAllAudiobooksByTitleQuery_Id:
    {

        Querrystring.clear();
        Datatype.clear();
        Datatype.append("AudioBook");
        Querrystring.append(QString("select DISTINCT T_TopLevelName from MasterTable where T_TYPE = \"%1\" AND T_TopLevelName not like \"%2\" order by T_TopLevelName ").arg(Datatype).arg(""));

        noOfColumns = GETALLAUDIOBOOKSBYTITLEQUERY_DATATYPECOUNT;

     }


   break;
    case eGetAlliTunesUByTitleQuery_Id:
    {

        Querrystring.clear();
        Datatype.clear();
        Datatype.append("iTunes U");
        Querrystring.append(QString("select DISTINCT T_TopLevelName from MasterTable where T_TYPE = \"%1\" AND T_TopLevelName not like \"%2\" order by T_TopLevelName ").arg(Datatype).arg(""));

        noOfColumns = GETALLITUNESUBYTITLEQUERY_DATATYPECOUNT;

       }


   break;
    default:
        break;

}
     ret = senddata(noOfColumns,queryId,Querrystring,IpodDB);
           return ret;
    }

bool CIpodDataProvider::GetSearchIPODdataByKeywordFromVolume(QString searchData,ETrackerAbstarctor_Query_Ids queryId,QSqlDatabase IpodDB)
{
    QString Querrystring;
    QString Datatype;
    int noOfColumns = 0;
    bool ret=false;

    switch (queryId)
    {
    case eGetAllAudiobookChapterNamesQuery_Id:
    {

         qDebug() << "in:"<< queryId;
        Querrystring.clear();
        Datatype.clear();
        Datatype.append("AudioBook");
        Querrystring.append(QString("select T_Title,T_FilePath from MasterTable where T_Album = \"%1\" AND T_TYPE = \"%2\" AND T_Title not like \"%3\" order by T_Title").arg(searchData,Datatype).arg(""));

        noOfColumns = GETALLAUDIOBOOKCHAPTERNAMESQUERY_DATATYPECOUNT  ;

    }
    break;
    case eGetAllPodcastsEpisodeNamesQuery_Id:
    {
        Querrystring.clear();
        Datatype.clear();
        Datatype.append("Podcast");
        Querrystring.append(QString("select T_Title,T_FilePath from MasterTable where T_Album = \"%1\" AND T_TYPE = \"%2\" AND T_Title not like \"%3\" order by T_Title").arg(searchData,Datatype).arg(""));

        noOfColumns = GETALLPODCASTSEPISODENAMESQUERY_DATATYPECOUNT;

    }

    break;
    case eGetAlliTunesUEpisodeNamesQuery_Id:
    {
        Querrystring.clear();
        Datatype.clear();
        Datatype.append("iTunes U");
        Querrystring.append(QString("select T_Title,T_FilePath from MasterTable where T_Album = \"%1\" AND T_TYPE = \"%2\" AND T_Title not like \"%3\" order by T_Title").arg(searchData,Datatype).arg(""));

        noOfColumns = GETALLITUNESUEPISODENAMESQUERY_DATATYPECOUNT;

    }

    break;
     default:
         break;

 }
    ret = senddata(noOfColumns,queryId,Querrystring,IpodDB);

    return ret;
}

void CIpodDataProvider::exec(QSparqlQuery* query)
{
    m_pQueryResult = m_pSparql_Connection->exec(*query);
    LOG_TRACE << Q_FUNC_INFO << " query: " << query->query() << LOG_ENDL;
}

bool CIpodDataProvider::exec(QSqlQuery* query)
{
    bool result = query->exec();
    LOG_TRACE << Q_FUNC_INFO << " query: " << query->lastQuery() << LOG_ENDL;
    if (query->lastError().isValid()) {
        LOG_TRACE << Q_FUNC_INFO << " last error: " << query->lastError().text() << LOG_ENDL;
    }
    return result;
}

bool CIpodDataProvider::senddata(int noOfColumns,ETrackerAbstarctor_Query_Ids queryId ,QString querrystring,QSqlDatabase ipoddb)
{



    qDebug() << "[TA] in send data"<<querrystring;
    QSqlQuery* query = new QSqlQuery(querrystring,ipoddb);
    // QSqlQuery query(querrystring,ipoddb);
    bool  ret=false ;

    ret=query->exec();

    qDebug() << "  [TA] exec successfully"<<ret;
    if (ret)
    {
        //queryId = MAP_QUERY_IDS.value(gGetAllSongsQuery_Key);
        QVector<QStringList> queryDataList;
        queryDataList.clear();

        while(query->next())
        {
            QStringList columnList;
            for (int i = 0; i < noOfColumns; i++)
            {
                QString value = query->value(i).toString();
                qDebug() << "  podcast value"<<value;
                //columnList.append(QUrl::fromPercentEncoding(value.toUtf8()));
                //{ modified by jaehwan 2014.2.10 for % deconfig issue
                if (value.startsWith(QLatin1String("file://")))
                {
                    value = QUrl::fromPercentEncoding(value.toUtf8());
                }
                columnList.append(value);
                //} modified by jaehwan
            }
            queryDataList.append(columnList);
         }
         emit DataFetched(queryDataList, queryId);
    }
    delete query;
    return ret;

}

/******************************************************************************

// Function Name                                            Parameters

// CIpodDataProvider::GetAllPodcastsByTitle()                bool (OUT)
//												ETrackerAbstarctor_VolumeType(IN)									

// Explanation

// This method handles the query for GetAllPodcastsByTitle

// It connects signal/slot for the query completion

*****************************************************************************/
bool CIpodDataProvider::GetAllPodcastsByTitle(enum ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret_val = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllPodcastsByTitle);

    if(!ret_val)
        return ret_val;

    //setting query string
    QString queryString;
    queryString = gGetAllPodcastsByTitleQuery.arg(gPodcastTypeName,m_VolumeName_GetAllPodcastsByTitle);
 
    LOG_TRACE<<"Inside GetAllPodcastsByTitle"<< LOG_ENDL;
    LOG_TRACE <<"volume type"<<volumeType << LOG_ENDL;
	
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

    // if query executed successfully then connecting signal/slot for result
    bool ret=false;
    if (!m_pQueryResult->hasError())
    {
                LOG_TRACE <<"GetAllPodcastsByTitle QueryResult dont have error" << LOG_ENDL;
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnFinished()));
    }
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }
    LOG_TRACE <<"GetAllPodcastsByTitle ret = "<< ret << LOG_ENDL;
    return ret;
}

/******************************************************************************

// Function Name                                            Parameters

// CIpodDataProvider::GetAllAudiobooksByTitle                 bool (OUT)

// ETrackerAbstarctor_VolumeType(IN)

// Explanation

// This method handles the query for GetAllAudiobooksByTitle

// It connects signal/slot for the query completion

//

*****************************************************************************/
bool CIpodDataProvider::GetAllAudiobooksByTitle(enum ETrackerAbstarctor_VolumeType  volumeType)
{

   bool ret_val = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllAudiobooksByTitle);
    if(!ret_val)
        return ret_val;
    //setting query string
    QString queryString;
    queryString = gGetAllAudiobooksByTitleQuery.arg(gAudioBookTypeName,m_VolumeName_GetAllAudiobooksByTitle);
    
        LOG_TRACE <<"Inside GetAllAudiobooksByTitle" << LOG_ENDL;
        LOG_TRACE <<"volume type"<<volumeType << LOG_ENDL;
	
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

    // if query executed successfully then connecting signal/slot for result
    bool ret=false;
    if (!m_pQueryResult->hasError())
    {
         ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnFinished()));
    }
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }
    
    return ret;
}

/******************************************************************************

// Function Name                                            Parameters

// CIpodDataProvider::GetAllPodcastsEpisodeNames

//															bool (OUT)
//															QString,
//												ETrackerAbstarctor_VolumeType	

// Explanation

// This method handles the query for GetAllPodcastsEpisodeNames.

// It connects signal/slot for the query completion

// History


*****************************************************************************/
  bool CIpodDataProvider::GetAllPodcastsEpisodeNames(QString podcastName,
							enum ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;
      if(podcastName.isNull())
      {
          HandleError(eNullStringParameter);
          return ret;
      }
    bool ret_val = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllPodcastsEpisodeNames);
    if(!ret_val)
        return ret_val;

      //setting query string
    QString queryString;
    queryString = gGetAllPodcastsEpisodeNamesQuery.arg(podcastName,gPodcastTypeName,m_VolumeName_GetAllPodcastsEpisodeNames);
    LOG_TRACE <<"Inside GetAllPodcastsEpisodeNames" << LOG_ENDL;
    LOG_TRACE <<"volume type"<<volumeType << LOG_ENDL;
	
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    m_PodCastName = podcastName;

    // executing query
    exec(&QueryObject);

    // if query executed successfully then connecting signal/slot for result

    if (!m_pQueryResult->hasError())
    {
         ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnFinished()));
    }
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }
    
    return ret;
   
  
  
}

/******************************************************************************

// Function Name                                            Parameters

// CIpodDataProvider::GetAllAudiobookChapterNames

//															bool (OUT)
//															QString,
//												ETrackerAbstarctor_VolumeType	

// Explanation

// This method handles the query for GetAllPodcastsEpisodeNames.

// It connects signal/slot for the query completion

// History

*****************************************************************************/
  bool CIpodDataProvider::GetAllAudiobookChapterNames(QString AudiobookName,
							enum ETrackerAbstarctor_VolumeType  volumeType)
{
      bool ret=false;
        if(AudiobookName.isNull())
        {
            HandleError(eNullStringParameter);
            return ret;
        }
       bool ret_val = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllAudiobookChapterNames);
       if(!ret_val)
           return ret_val;
      //setting query string
    QString queryString;
    queryString = gGetAllAudiobookChapterNamesQuery.arg(AudiobookName,gAudioBookTypeName,m_VolumeName_GetAllAudiobookChapterNames);

        LOG_TRACE <<"Inside GetAllAudiobookChapterNames" << LOG_ENDL;
        LOG_TRACE <<"volume type"<<volumeType << LOG_ENDL;

    m_AudioBookName = AudiobookName;
	
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

    // if query executed successfully then connecting signal/slot for result

    if (!m_pQueryResult->hasError())
    {
         ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnFinished()));
    }
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }
    
    return ret;
      
}

/******************************************************************************

// Function Name     OnFinished                            Parameters
//															None

// Explanation

// This is a slot called when finished signal emited by QSparqlResult on query completion

// It emits DataFetched signal for TrackerAbstractor

//

// History

*****************************************************************************/

void CIpodDataProvider::OnFinished()
{
        LOG_TRACE <<"OnFinished" << LOG_ENDL;
    QVector<QStringList> queryDataList;
//    QStringList dataList;
    int noOfColumns = 0;
    QString query = gBlankSpace;

    ETrackerAbstarctor_Query_Ids queryId = eDefault_Id;
    QSparqlResult* result = qobject_cast<QSparqlResult*>(sender());

       query = result->query();
	   
           LOG_TRACE <<"query == "<<query << LOG_ENDL;

    // comparing result query with orignal query to set query id and coloumn count
   
    if(!query.compare(gGetAllPodcastsByTitleQuery.arg(gPodcastTypeName,m_VolumeName_GetAllPodcastsByTitle)))
   {
       LOG_TRACE <<"Inside gGetAllPodcastsByTitleQuery" << gPodcastTypeName << LOG_ENDL;
//	   queryId = MAP_QUERY_IDS.value(gGetAllPodcastsByTitleQuery_Key);
       queryId = eGetAllPodcastsByTitleQuery_Id;
       noOfColumns = GETALLPODCASTSBYTITLEQUERY_DATATYPECOUNT;
           LOG_TRACE <<"queryID = "<<queryId<<" noOfColumns"<<noOfColumns << LOG_ENDL;
   }
   
   else if(!query.compare(gGetAllAudiobooksByTitleQuery.arg(gAudioBookTypeName,m_VolumeName_GetAllAudiobooksByTitle)))
   {
       LOG_TRACE <<"Inside gGetAllAudiobooksByTitleQuery" << gAudioBookTypeName << LOG_ENDL;
//    queryId = MAP_QUERY_IDS.value(gGetAllAudiobooksByTitleQuery_Key);
       queryId = eGetAllAudiobooksByTitleQuery_Id;
    noOfColumns = GETALLAUDIOBOOKSBYTITLEQUERY_DATATYPECOUNT;
   }
   
    else if(!query.compare(gGetAllPodcastsEpisodeNamesQuery.arg(m_PodCastName,gPodcastTypeName,m_VolumeName_GetAllPodcastsEpisodeNames)))
   {
        LOG_TRACE <<"Inside gGetAllPodcastsEpisodeNamesQuery" << m_PodCastName << gPodcastTypeName << LOG_ENDL;
//    queryId = MAP_QUERY_IDS.value(gGetAllPodcastsEpisodeNamesQuery_Key);
        queryId = eGetAllPodcastsEpisodeNamesQuery_Id;
    noOfColumns = GETALLPODCASTSEPISODENAMESQUERY_DATATYPECOUNT;   
   }
   
   else if(!query.compare(gGetAllAudiobookChapterNamesQuery.arg(m_AudioBookName,gAudioBookTypeName,m_VolumeName_GetAllAudiobookChapterNames)))
   {
        LOG_TRACE <<"Inside gGetAllAudiobookChapterNamesQuery" << m_AudioBookName << gAudioBookTypeName << LOG_ENDL;
//    queryId = MAP_QUERY_IDS.value(gGetAllAudiobookChapterNamesQuery_Key);
        queryId = eGetAllAudiobookChapterNamesQuery_Id;
    noOfColumns = GETALLAUDIOBOOKCHAPTERNAMESQUERY_DATATYPECOUNT;  
   }
    else
    {
        HandleError(eQueryMappingError);
        if(result != NULL)
        {
            delete result;
            result = NULL;
        }
        return;
    }
     
   LOG_TRACE  << "DEBUG CHECK" << m_PodCastName << m_AudioBookName << LOG_ENDL;

   m_FirstTime = true;
   if(result->hasError())
   {
        LOG_TRACE  <<"Error In Query :"<< query << LOG_ENDL;
       /* QStringList Error ;
        Error.append("Error In Query :");
        Error.append(query);
        queryDataList.append(Error);
        //queryDataList.append(query);*/
        HandleError(eErrorInQuery);
        QSparqlError queryError = result->lastError();
        LOG_TRACE<<"Query Error type = "<<queryError.type()
               << "Query Error Message = "<<queryError.message() << LOG_ENDL;
       if(result != NULL)
       {
           delete result;
           result = NULL;
       }
        return;
    }

	// retrieving data from QSpaqlResult
       while (result->next())
       {
           QStringList columnList;
           for (int i = 0; i < noOfColumns; i++)
           {
               QString value = result->value(i).toString();
               if (value.startsWith(QString::fromLatin1("file://"))) {
                   value = QLatin1String("file://") + QUrl::fromEncoded(value.toLatin1()).toLocalFile();
               }

               columnList.append(value);
           }
           queryDataList.append(columnList);
//           LOG_TRACE <<columnList << LOG_ENDL;
       }

	 /*  // removed by eugene.seo 2013.03.27 for static test
     if(result != NULL)
     {
         delete result;
         result = NULL;
     }
     */
    // after data feching emiting a singal to notify CTrackerAbstractor
    emit DataFetched(queryDataList, queryId);
}

void CIpodDataProvider::HandleError(int nError)
{
        LOG_TRACE << " the error is = " << nError << LOG_ENDL;
        emit Error(nError);
}

/******************************************************************************

// Function Name     GetVolumeQueryToAppend(ETrackerAbstarctor_VolumeType volumeType, QString& volumeQuery)
// Explanation

// This is to create the Filter for Query to append as per volume URL.

// History
 // 30-Apr-2012      mahesh        This is to create the Filter for Query to append as per volume URL.
*****************************************************************************/
bool CIpodDataProvider::GetVolumeQueryToAppend(ETrackerAbstarctor_VolumeType volumeType, QString& volumeQuery)
{
     QString filter = " FILTER(";
     QString url = "?url , ";
     bool ret = false;

        m_VolumeNameList.clear();
        //QString DeviceUrl = getDeviceURL(volumeType);
        QString DeviceUrl ;
        DeviceUrl.append(m_DeviceURL.at(volumeType));
        m_VolumeNameList.append(DeviceUrl);

     if(m_VolumeNameList.count() > 0)
        {
            for (int j = 0; j < m_VolumeNameList.count(); j++)
            {
                QString Regex = "REGEX (";

                Regex.append(url);
                Regex.append(gDoubleQuote);
                Regex.append("^");
                Regex.append(m_VolumeNameList[j]);
                Regex.append(gDoubleQuote);
                if(j == (m_VolumeNameList.size()-1) )
                    Regex.append(")");
                else
                    Regex.append(") || ");

                filter.append(Regex);
            }

            filter.append(")");
            ret = true;
        }
        else
        {
            QString Regex = "REGEX (";

            Regex.append(url);
            Regex.append(gDoubleQuote);
            Regex.append("^");
            Regex.append(gVolumePath_IPod);
            Regex.append(gDoubleQuote);
            Regex.append(")");

            filter.append(Regex);

            filter.append(")");
            //            return false;
             ret = true;
        }
      /* dead code // removed by eugene.seo 2013.03.27 for static test
	  if(!ret)
         return ret;
      */

    volumeQuery.clear();
    volumeQuery.append(filter);

    return ret;
}

/******************************************************************************

// Function Name                                                Parameters

// CMediaDataProvider::setDeviceURL(ETrackerAbstarctor_VolumeType volumeType,QString DeviceId,QString MountPath)
//

// Explanation

//This method Get the URL of the mounted  Device based on the volumetype

//

// History

// 27-Mar-2012     mahesh kumar    Set the URL of the mounted  Device based on the volumetype
*****************************************************************************/

bool CIpodDataProvider::setDeviceURL(ETrackerAbstarctor_VolumeType volumeType,QString DeviceId,QString MountPath)
{
    bool ret = false;
    //QSparqlQuery volumeQueryObject;
    //QSparqlResult* result;
    QString deviceUrl;

   // qDebug() << "CMediaDataProvider::setDeviceURL" << volumeType << DeviceId << MountPath;
    m_DeviceId.removeAt(volumeType);
    m_DeviceId.insert(volumeType,DeviceId);

    switch(volumeType)
    {

    case eIPod:
    case eIPOD_FRONT:
    case eIPOD_REAR:

        m_DeviceURL.removeAt(volumeType);
        deviceUrl.append(gVolumePath_IPod);
        deviceUrl.append(MountPath);
        m_DeviceURL.insert(volumeType,deviceUrl);
        ret = true;
        break;
default:
        HandleError(eErrorDeviceId);
        break;

    }
    return ret;

}


//End of file
