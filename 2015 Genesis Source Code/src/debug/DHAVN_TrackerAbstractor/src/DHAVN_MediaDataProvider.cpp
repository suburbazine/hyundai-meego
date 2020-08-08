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
* 1.0       12 September 2011  Chetan Nanda           Draft version
******************************************************************
*/

#include "DHAVN_MediaDataProvider.h"
#include "DHAVN_AppFramework_Def.h"
#include <QSparqlResult>
#include <QSparqlQuery>
#include <QSparqlConnection>
#include <QSparqlError>
#include <QUrl>
#include <QTextCodec>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QtSql>
#include <QDirIterator>
#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>
#include <QDebug>

#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusMessage>

#include "DHAVN_TrackerAbstractor.h"

static CLogUtil m_Logger; //added by jaehwan to use logging api in static functions 20140108.

//initialize static variables.
wchar_t CMediaDataProvider::pinyinTable[PINYIN_TABLE_SIZE] = {QChar::Null, };

CMediaDataProvider::CMediaDataProvider(QSparqlConnection* sparqlConnection)
{
    LOG_INIT_V2("Media_TrackerAbstractor")

    m_pSparql_Connection = sparqlConnection;
    if (m_pSparql_Connection->isValid())
    {
        LOG_TRACE << "DHAVN_TrackerAbstrator - CMediaDataProvider - Driver found" << LOG_ENDL;
    }
    else
    {
        LOG_TRACE << "DHAVN_TrackerAbstrator - CMediaDataProvider - Driver not found" << LOG_ENDL;
    }

    m_pQueryResult = NULL;
    m_EndIndex = 0;
    m_FirstTime = false;
    m_OrderType = eAlbum;
    m_QueryId = eDefault_Id;
    m_StartIndex = 0;
    m_VolumeType = eJukeBox;
    m_isCDInserted = false;    
    /* Dummy initilization of QStringlist*/
    for(int i=0;i<=EVOLUME_TYPE_SIZE;i++)
    {
    m_DeviceId << "Empty" ;
    m_DeviceURL << "Empty";
    }
	m_Query_Offset = QUERY_OFFSET;
	m_Query_Limit  = QUERY_LIMIT;
	
    m_strMountPath.clear(); // added by eugene.seo 2013.05.13

    songIndexCountSavedList.clear();   // { added by sungha.choi 2013.07.23
    artistIndexCountSavedList.clear();
    albumIndexCountSavedList.clear();
    savedSongSearchData.clear();
    savedArtistSearchData.clear();
    savedAlbumSearchData.clear();
    savedSongVolumeType = eDefaultVolume;
    savedArtistVolumeType = eDefaultVolume;
    savedAlbumVolumeType = eDefaultVolume;
    songDataSavedList.clear();
    artistDataSavedList.clear();
    albumDataSavedList.clear();
    m_cacheSuccess = false;    // } added by sungha.choi 2013.07.23
}

CMediaDataProvider::~CMediaDataProvider()
{
    if(m_pQueryResult != NULL)
    {
        //delete m_pQueryResult;
        m_pQueryResult = NULL;
    }
}

/******************************************************************************

// Function Name                                            Parameters

// CMediaDataProvider::GetSongsByFolder        bool (OUT), QString (IN) - folder to search

//

// Explanation

// This method handles the query for searching songs under respective folder

// It connects signal/slot for the query completion

//

// History

// 12-Sept-2011      Chetan Nanda                created for seaching songs inside particular folder
//
*****************************************************************************/
bool CMediaDataProvider::GetSongsByFolder(QString folderName)
{
    bool ret=false;
    if(folderName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }
    // replacing space with %20 escape character
    folderName = folderName.replace(gBlankSpace, gSpaceEscapeChar);
    m_FolderName_SongByFolder = folderName;
    QString queryString;

    // setting query string
    if (folderName.isEmpty())
    {
        queryString = gSongsByEmptyFolderQuery;
    }
    else
    {
        queryString = gSongsByFolderQuery.arg(folderName);
    }

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

// Function Name                                            Parameters

// CMediaDataProvider::GetArtistByFolder        bool (OUT), QString (IN) - folder to search

//

// Explanation

// This method handles the query for searching song artists under respective folder

// It connects signal/slot for the query completion

//

// History

// 12-Sept-2011      Chetan Nanda                created for seaching song artists inside particular folder
*****************************************************************************/
bool CMediaDataProvider::GetArtistByFolder(QString folderName)
{
    bool ret=false;
    if(folderName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }
    // replacing space with %20 escape character
    folderName = folderName.replace(gBlankSpace, gSpaceEscapeChar);
    m_FolderName_ArtistByFolder = folderName;
    QString queryString;

    // setting query string
    if (folderName.isEmpty())
    {
        queryString = gArtistByEmptyFolderQuery;
    }
    else
    {
        queryString = gArtistByFolderQuery.arg(folderName);
    }

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

// Function Name                                            Parameters

// CMediaDataProvider::GetAlbumsByFolder      bool (OUT), QString (IN) - folder to search

//

// Explanation

// This method handles the query for searching song albums under respective folder

// It connects signal/slot for the query completion

//

// History

// 12-Sept-2011      Chetan Nanda                created for seaching song albums inside particular folder
*****************************************************************************/
bool CMediaDataProvider::GetAlbumsByFolder(QString folderName)
{
    bool ret=false;
    if(folderName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }
    // replacing space with %20 escape character
    folderName = folderName.replace(gBlankSpace, gSpaceEscapeChar);
    m_FolderName_AlbumByFolder = folderName;
    QString queryString;

    // setting query string
    if (folderName.isEmpty())
    {
        queryString = gAlbumByEmptyFolderQuery;
    }
    else
    {
        queryString = gAlbumByFolderQuery.arg(folderName);
    }

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

// Function Name                                            Parameters

// CMediaDataProvider::GetAllSongs                 bool (OUT)

//

// Explanation

// This method handles the query for searching all songs

// It connects signal/slot for the query completion

//

// History

// 12-Sept-2011      Chetan Nanda                created for seaching all songs
*****************************************************************************/
bool CMediaDataProvider::GetAllSongs()
{
    //setting query string
    QString queryString;
    queryString = gGetAllSongsQuery;

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

// CMediaDataProvider::GetAllSongsIDs                 bool (OUT)

//

// Explanation

// This method handles the query for searching all songs and also returns IDs

// It connects signal/slot for the query completion

//

// History

// 28-Oct-2011      Divya                created for seaching all songs
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsIDs()
{
    //setting query string
    QString queryString;
    queryString = gGetAllSongsIDQuery;

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

// CMediaDataProvider::GetFoldersList                 bool (OUT)

//

// Explanation

// This method handles the query for searching all folders in current path.

// It connects signal/slot for the query completion

//

// History

// 12-Sept-2011      Chetan Nanda                created for seaching all songs
*****************************************************************************/
bool CMediaDataProvider::GetFolderList(QString folderName)
{
    bool ret=false;
    if(folderName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    // replacing space with %20 escape character
    folderName = folderName.replace(gBlankSpace, gSpaceEscapeChar);
    m_FolderName_FolderList = folderName;
    QString queryString;

    // setting query string
    if (folderName.isEmpty())
    {
        queryString = gFolderListEmptyQuery;
    }
    else
    {
        queryString = gFolderListQuery.arg(folderName);
    }

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

// Function Name                                            Parameters

// CMediaDataProvider::GetSongsByAlbum        bool (OUT), QString (IN) - album to search

//

// Explanation

// This method handles the query for searching songs under respective album

// It connects signal/slot for the query completion

//

// History

// 20-Sept-2011      Chetan Nanda                created for seaching songs inside particular album
*****************************************************************************/
bool CMediaDataProvider::GetSongsByAlbum(QString albumName)
{
    bool ret = false;
    if(albumName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    m_AlbumName = albumName;
    QString queryString;

    // setting query string
    queryString = gSongsByAlbumQuery.arg(albumName);

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

// Function Name                                            Parameters

// CMediaDataProvider::GetAllAlbums                 bool (OUT)

//

// Explanation

// This method handles the query for searching all albums

// It connects signal/slot for the query completion

//

// History

// 21-Sept-2011      Chetan Nanda                created for seaching all albums
*****************************************************************************/
bool CMediaDataProvider::GetAllAlbums()
{
    // setting query
    QString queryString;
    queryString = gGetAllAlbumsQuery;

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

    // if query executed successfully then connecting signal/slot for result
    bool ret = false;
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

// CMediaDataProvider::GetAllMusicArtists        bool (OUT)

//

// Explanation

// This method handles the query for searching all music artists

// It connects signal/slot for the query completion

//

// History

// 22-Sept-2011      Chetan Nanda                created for seaching all music artists
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicArtists()
{
    // setting query string
    QString queryString;
    queryString = gGetAllMusicArtistsQuery;

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

    // if query executed successfully then connecting signal/slot for result
    bool ret = false;
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

// Function Name                                                    Parameters

// CMediaDataProvider::GetAllMusicComposers         bool (OUT)

//

// Explanation

// This method handles the query for searching all music composers

// It connects signal/slot for the query completion

//

// History

// 22-Sept-2011      Chetan Nanda                created for seaching all music composers
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicComposers()
{
    // setting query string
    QString queryString;
    queryString = gGetAllMusicComposersQuery;

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

    // if query executed successfully then connecting signal/slot for result
    bool ret = false;
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

// Function Name                                                    Parameters

// CMediaDataProvider::GetAllMusicGenres               bool (OUT)

//

// Explanation

// This method handles the query for searching all music genres

// It connects signal/slot for the query completion

//

// History

// 23-Sept-2011      Chetan Nanda                created for seaching all music genres
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicGenres()
{
    // setttng query string
    QString queryString;
    queryString = gGetAllMusicGenresQuery;

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

    // if query executed successfully then connecting signal/slot for result
    bool ret = false;
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

// Function Name                                                 Parameters

// CMediaDataProvider::GetMusicUrlAppendedFromUrlList           bool (OUT), QString (IN) - file path to retrieve metadata

//

// Explanation

// This method forms a complete URL for retrieving music metadata information (artist, album,genre etc)

// It connects signal/slot for the query completion

//

// History

// 8th-March-2012      Viquar Hashmi
*****************************************************************************/
QString CMediaDataProvider::GetMusicUrlAppendedFromUrlList(QStringList urlList)
{
   QString filter;
   QString url;

   url.append( "?url , ");
   filter.append(" FILTER(");

   for(int i = 0 ; i < urlList.count(); i++)
   {
       urlList[i] = QUrl::toPercentEncoding(urlList[i],":/()~!@$&_-',.=+%");
//       qDebug() << "URL LIST"<< urlList[i];
        QString Regex = "REGEX (";
        Regex.append(url);
        Regex.append(gDoubleQuote);
        Regex.append(urlList[i]);
        Regex.append(gDoubleQuote);
        if(i == (urlList.size()-1))
            Regex.append(")");
        else
            Regex.append(") || ");

        filter.append(Regex);
   }
   filter.append(")");
//    qDebug() << "FILTER "<< filter;
   return filter;
}

// { added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
QString CMediaDataProvider::GetMusicUrlAppended(QString path)
{   
   QString filter;
   QString url;

   url.append( "?url , ");
   filter.append(" FILTER(");

   if(!path.isNull())
   {
        path = QUrl::toPercentEncoding(path,":/()~!@$&_-',.=+%");
        QString Regex = "REGEX (";
        Regex.append(url);
        Regex.append(gDoubleQuote);
        Regex.append(path);
        Regex.append(gDoubleQuote);
        Regex.append(")");
        filter.append(Regex);
   }
   filter.append(")");
   return filter;
}
// } added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup


/******************************************************************************

// Function Name                                                 Parameters

// CMediaDataProvider::GetMusicMetaDataInfoFromUrlList           bool (OUT), QString (IN) - file path to retrieve metadata

//

// Explanation

// This method handles the query for retrieving music metadata information (artist, album,genre etc)

// It connects signal/slot for the query completion

//

// History

// 8th-March-2012      Viquar Hashmi                created for retrieving music metadata information
*****************************************************************************/
bool CMediaDataProvider::GetMusicMetaDataInfoFromUrlList(QStringList filePathUrl)
{
    bool ret = false;
    if(filePathUrl.contains(QString()))
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    m_FilePathInfo_MusicMetaData = GetMusicUrlAppendedFromUrlList(filePathUrl);

    QString queryString;
    // setting query string
    queryString = gGetMusicMetaDataInfoFromURLListQuery.arg(m_FilePathInfo_MusicMetaData);

//    qDebug() << "Query" << queryString ;
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

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

// Function Name                                                    Parameters

// CMediaDataProvider::GetMusicMetaData               bool (OUT), QString (IN) - file path to retrieve metadata

//

// Explanation

// This method handles the query for retrieving music metadata information (artist, album,genre etc)

// It connects signal/slot for the query completion

//

// History

// 23-Sept-2011      Chetan Nanda                created for retrieving music metadata information
*****************************************************************************/
bool CMediaDataProvider::GetMusicMetaData(QString filePath)
{
    bool ret = false;
    if(filePath.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    if(filePath.startsWith(gVolumePath_IPod))
    {
        if(m_Ipod_DBConnList.count() > 0)
        {
            QVector<QStringList> queryDataList;

            QString queryStr;
            queryStr.append( QString("select T_Title, T_Artist, T_Album, T_Genre, T_FilePath, T_Composer From MasterTable where T_FilePath=\"%1\" ").arg(filePath));


            // TODO: As iPod changes are not available, currenlty hard coding the string to 6
            // Later it should be modified
             if(m_Ipod_DBConnList[0].isOpen())
                  ret = senddata(6, queryStr, m_Ipod_DBConnList[0], queryDataList);
             if(m_Ipod_DBConnList.count() > 1)
             {
                 if(m_Ipod_DBConnList[1].isOpen())
                    ret = senddata(6, queryStr, m_Ipod_DBConnList[1], queryDataList);
             }

             emit DataFetched(queryDataList, eGetMusicMetaDataQuery_Id);
         }
    }
    else
    {
        //    filePath = filePath.replace(gBlankSpace, gSpaceEscapeChar);
        //  filePath = filePath.replace(gSingeQuote, gSingeQuoteEscapeChar);
        filePath = QUrl::toPercentEncoding(filePath,":/()~!@$&_-',.=+");

        m_FilePath_MusicMetaData = filePath;
        QString queryString;

        // setting query string
        queryString = gGetMusicMetaDataQuery.arg(filePath);

        QSparqlQuery QueryObject;
        QueryObject.setQuery(queryString);

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
    }

    return ret;
}

bool CMediaDataProvider::GetMusicNormalMetaData(QString filePath)    // added by sungha.choi 2013.09.06 for ITS 0188361
{
    bool ret = false;
    if(filePath.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    filePath = QUrl::toPercentEncoding(filePath,":/()~!@$&_-',.=+");

    m_FilePath_MusicMetaData = filePath;
    QString queryString;

    // setting query string
    queryString = gGetMusicNormalMetaDataQuery.arg(filePath);

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

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

//{added by junam 2013.03.27 for ISV72425
bool CMediaDataProvider::GetMusicSimpleMetaData(QString filePath)
{
    bool ret = false;
    if(filePath.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    if(filePath.startsWith(gVolumePath_IPod))
    {
        if(m_Ipod_DBConnList.count() > 0)
        {
            QVector<QStringList> queryDataList;

            QString queryStr;
            queryStr.append( QString("select T_Title, T_Artist, T_Album, T_FilePath From MasterTable where T_FilePath=\"%1\" ").arg(filePath));


            // TODO: As iPod changes are not available, currenlty hard coding the string to 6
            // Later it should be modified
             if(m_Ipod_DBConnList[0].isOpen())
                  ret = senddata(GETMUSICSIMPLEMETADATAQUERY_DATATYPECOUNT, queryStr, m_Ipod_DBConnList[0], queryDataList);
             if(m_Ipod_DBConnList.count() > 1)
             {
                 if(m_Ipod_DBConnList[1].isOpen())
                    ret = senddata(GETMUSICSIMPLEMETADATAQUERY_DATATYPECOUNT, queryStr, m_Ipod_DBConnList[1], queryDataList);
             }

             emit DataFetched(queryDataList, eGetMusicSimpleMetaDataQuery_Id);
         }
    }
    else
    {
        //    filePath = filePath.replace(gBlankSpace, gSpaceEscapeChar);
        //  filePath = filePath.replace(gSingeQuote, gSingeQuoteEscapeChar);
        filePath = QUrl::toPercentEncoding(filePath,":/()~!@$&_-',.=+");

        m_FilePath_MusicMetaData = filePath;
        QString queryString;

        // setting query string
        queryString = gGetMusicSimpleMetaDataQuery.arg(filePath);

        QSparqlQuery QueryObject;
        QueryObject.setQuery(queryString);

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
    }

    return ret;
}
//}added by junam 2013.03.27


/******************************************************************************

// Function Name                                            Parameters

// CMediaDataProvider::GetAllSongsByAlbum        bool (OUT), QString (IN) - album to search

//

// Explanation

// This method handles the query for searching songs under respective album

// It connects signal/slot for the query completion

//

// History

// 26-Sept-2011      Chetan Nanda                created for seaching songs inside particular album
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsByAlbum(QString albumName)
{
    bool ret=false;
    if(albumName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    m_AlbumName_AllSongsByAlbum = albumName;
    QString queryString;

    // setting query string
    queryString = gSongsByAlbumQuery.arg(albumName);

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

// Function Name                                            Parameters

// CMediaDataProvider::GetAllSongsByArtist        bool (OUT), QString (IN) - artist to search

//

// Explanation

// This method handles the query for searching songs of given artist

// It connects signal/slot for the query completion

//

// History

// 26-Sept-2011      Chetan Nanda                created for seaching songs of given artist
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsByArtist(QString artistName)
{
    bool ret=false;
    if(artistName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }
    m_ArtistName_AllSongsByArtist = artistName;
    QString queryString;

    // setting query string
    queryString = gGetAllSongsByArtistQuery.arg(artistName);

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

// Function Name                                            Parameters

// CMediaDataProvider::GetAllSongsByGenre        bool (OUT), QString (IN) - genre to search

//

// Explanation

// This method handles the query for searching songs from given genre

// It connects signal/slot for the query completion

//

// History

// 27-Sept-2011      Chetan Nanda                created for seaching songs from given genre
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsByGenre(QString genreName)
{
    bool ret=false;
    if(genreName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }
    m_GenreName_AllSongsByGenre = genreName;
    QString queryString;

    // setting query string
    queryString = gGetAllSongsByGenreQuery.arg(genreName);

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

// Function Name                                                    Parameters

// CMediaDataProvider::GetAllSongsByComposer      bool (OUT), QString (IN) - composer to search

//

// Explanation

// This method handles the query for searching songs of given composer

// It connects signal/slot for the query completion

//

// History

// 28-Sept-2011      Chetan Nanda                created for seaching songs of given composer
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsByComposer(QString composerName)
{
    bool ret=false;
    if(composerName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }
    m_ComposerName_AllSongsByComposer = composerName;
    QString queryString;

    // setting query string
    queryString = gGetAllSongsByComposerQuery.arg(composerName);

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

// Function Name                                            Parameters

// CMediaDataProvider::GetImagesByFolder        bool (OUT), QString (IN) - folder to search

//

// Explanation

// This method handles the query for searching images under respective folder

// It connects signal/slot for the query completion

//

// History

// 28-Sept-2011      Chetan Nanda                created for seaching images inside particular folder
*****************************************************************************/
bool CMediaDataProvider::GetImagesByFolder(QString folderName)
{
    bool ret=false;
    if(folderName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    folderName = QUrl::toPercentEncoding(folderName, ":/()~!@$&_-',.=+");

    // + sign should be modified later, else percent encoding API will change
    // the [] to %5B and %5D which should not happen
    folderName = folderName.replace(gPlusSign, gPlusSignCorrected);
    folderName = folderName.replace(gLBracketSign, gLBracketSignCorrected);
    folderName = folderName.replace(gRBracketSign, gRBracketSignCorrected);
    folderName = folderName.replace(gDollarSign, gDollarSignCorrected);
    m_FolderName_ImagesByFolder = folderName;

    QString queryString;

    // setting query string
    queryString = gGetImagesByFolderQuery.arg(folderName);

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

// Function Name                                            Parameters

// CMediaDataProvider::GetVideosByFolder        bool (OUT), QString (IN) - folder to search

//

// Explanation

// This method handles the query for searching videos under respective folder

// It connects signal/slot for the query completion

//

// History

// 28-Sept-2011      Chetan Nanda                created for seaching videos inside particular folder
*****************************************************************************/
bool CMediaDataProvider::GetVideosByFolder(QString folderName)
{
    bool ret=false;
    if(folderName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    LOG_TRACE << "In GetVideosByFolder  FilePath :" << folderName << LOG_ENDL;

    folderName = QUrl::toPercentEncoding(folderName, ":/()~!@$&_-',.=+");

    // + sign should be modified later, else percent encoding API will change
    // the [] to %5B and %5D which should not happen
    folderName = folderName.replace(gPlusSign, gPlusSignCorrected);
    folderName = folderName.replace(gLBracketSign, gLBracketSignCorrected);
    folderName = folderName.replace(gRBracketSign, gRBracketSignCorrected);
    folderName = folderName.replace(gDollarSign, gDollarSignCorrected);
    m_FolderName_VideosByFolder = folderName;

    QString queryString;

    // setting query string
    queryString = gGetVideosByFolderQuery.arg(folderName);

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

// Function Name                                                Parameters

// CMediaDataProvider::GetAllMusicByKeyword      bool (OUT), QString (IN) - keyword to search

//

// Explanation

// This method handles the query for retrieving music info using given keyword

// It connects signal/slot for the query completion

//

// History

// 29-Sept-2011      Chetan Nanda                created for retrieving music info using given keyword
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicByKeyword(QString keyword, QStringList volumeList, bool cdInsterted)
{
    bool ret=false;
    if(keyword.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }
    m_PathList_AllMusicByKeyword.clear();

    volumeList.append(gVolumePath_JukeBox);

    if(cdInsterted)
    {
        LOG_TRACE << "CD Inserted case is True" << LOG_ENDL;
        m_isCDInserted = true;
        //volumeList.append(gVolumePath_DISC);
    }

    if(volumeList.count() > 0)
    {
        m_PathList_AllMusicByKeyword = GetMusicUrlAppendedFromUrlList(volumeList);
        m_PathList_AllMusicByKeyword.replace("FILTER", "");
    }
    m_Keyword_AllMusicByKeywordForIPod = keyword;
    //    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    //    QString uc =codec->toUnicode(keyword.toAscii());
    //    QString keywordEUCKR;
    //    QTextStream streamFileOut(&uc);
    //    streamFileOut.setAutoDetectUnicode(false);
    //    streamFileOut.setCodec("EUC-KR");
    //    keywordEUCKR = streamFileOut.readAll();
    //    CLoggenerator::Loggenerator("Euc-Kr converted string");
    //    CLoggenerator::Loggenerator (keywordEUCKR);

//    QByteArray data = keyword.toUtf8();
    QTextCodec *codec2 = QTextCodec::codecForName("EUC-KR");
    QString utfStr = codec2->fromUnicode(keyword);

    LOG_TRACE << "first method::" << keyword << LOG_ENDL;    
    LOG_TRACE << "second method::" << utfStr << LOG_ENDL;

    CheckForSpecialCharacters(keyword);
    CheckForSpecialCharacters(utfStr);

    m_Keyword_AllMusicByKeyword = keyword;
    m_Keyword1_AllMusicByKeyword = utfStr;
    QString queryString;

    // setting query string
    queryString = gGetAllMusicByKeywordQuery.arg(keyword, utfStr, m_PathList_AllMusicByKeyword);

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

// Function Name                                            Parameters

// CMediaDataProvider::GetMultipleAlbumSongs        bool (OUT), QStringList (IN) - albumList to search

//

// Explanation

// This method handles the query for searching songs under respective album list

// It connects signal/slot for the query completion

//

// History

// 05-Oct-2011      Mahesh kumar             created for seaching songs inside particular album list
*****************************************************************************/
bool CMediaDataProvider::GetMultipleAlbumSongs(QStringList AlbumnamesList , QString folderName)
{
    folderName = folderName.replace(gBlankSpace, gSpaceEscapeChar);
    QString filter = "FILTER(";
    QString album = "?album=";

    for (int j = 0; j < AlbumnamesList.size(); j++) {
        QString Regex = " (";

        Regex.append(album);
        Regex.append(gDoubleQuote);
        Regex.append(AlbumnamesList[j]);
        Regex.append(gDoubleQuote);
        //Regex.append(" , 'i' ");
        if(j == (AlbumnamesList.size()-1) )
            Regex.append(")");
        else
            Regex.append(") || ");
        filter.append(Regex);
    }

    filter.append(")");
    QString FolderFilter = " . FILTER(REGEX(?url , ";
    filter.append(FolderFilter);
    filter.append(gDoubleQuote);
    filter.append(folderName);
    filter.append(gDoubleQuote);
    filter.append(" , 'i'))");

    QSparqlQuery QueryObject(gGetMultipleAlbumSongsQuery.arg(filter));
    QueryObject.bindValue("FILTER",filter);
    m_AlbumName_GetMultipleAlbumSongs = filter;

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

// CMediaDataProvider::GetMultipleArtistSongs        bool (OUT), QStringList (IN) - artistList to search

//

// Explanation

// This method handles the query for searching songs of given artist List

// It connects signal/slot for the query completion

//

// History

// 05-oct-2011      Mahesh              created for seaching songs of given artist List
*****************************************************************************/
bool CMediaDataProvider::GetMultipleArtistSongs(QStringList AlbumnamesList,QString folderName)
{
    //m_ArtistName_AllSongsByArtist = artistName;
    //QString queryString;

    // setting query string
    // queryString = gGetAllSongsByArtistQuery.arg(artistName);
    folderName = folderName.replace(gBlankSpace, gSpaceEscapeChar);

    QString filter = " FILTER(";
    QString performer = "?performer=";

    for (int j = 0; j < AlbumnamesList.size(); j++) {
        QString Regex = " (";

        Regex.append(performer);
        Regex.append(gDoubleQuote);
        Regex.append(AlbumnamesList[j]);
        Regex.append(gDoubleQuote);
        //Regex.append(", 'i'");
        if(j == (AlbumnamesList.size()-1) )
            Regex.append(")");
        else
            Regex.append(") || ");

        filter.append(Regex);
    }

    filter.append(")");
    QString FolderFilter = " . FILTER(REGEX(?url , ";
    filter.append(FolderFilter);
    filter.append(gDoubleQuote);
    filter.append(folderName);
    filter.append(gDoubleQuote);
    filter.append(",'i'))");
    m_ArtistName_GetMultipleArtistSongs = filter;

    QSparqlQuery QueryObject(gGetMultipleArtistSongsQuery.arg(filter));
    QueryObject.bindValue("FILTER",filter);

    // QueryObject.setQuery(queryString);

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

// Function Name                                                Parameters

// CMediaDataProvider::GetAllImagesByKeyword      bool (OUT), QString (IN) - keyword to search

//

// Explanation

// This method handles the query for retrieving images info using given keyword

// It connects signal/slot for the query completion

//

// History

// 11-oct-2011      mahesh               created for retrieving images info using given keyword
*****************************************************************************/
bool CMediaDataProvider::GetAllImagesByKeyword(QString keyword)
{
    bool ret=false;
    if(keyword.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    //QByteArray data = keyword.toUtf8();
    QTextCodec *codec2 = QTextCodec::codecForName("EUC-KR");
    QString utfStr = codec2->fromUnicode(keyword);
    LOG_TRACE << "first method::" << keyword << LOG_ENDL;
    LOG_TRACE << "second method::" << utfStr << LOG_ENDL;

    m_Keyword_AllImagesByKeyword = keyword;
    m_Keyword1_AllImagesByKeyword = utfStr;
    QString queryString;

    // setting query string

    queryString = gGetAllImagesByKeywordQuery.arg(keyword, utfStr);

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

// Function Name                                                Parameters

// CMediaDataProvider::GetAllVideosByKeyword      bool (OUT), QString (IN) - keyword to search

//

// Explanation

// This method handles the query for retrieving Videos info using given keyword

// It connects signal/slot for the query completion

//

// History

// 11-oct-2011      mahesh              created for retrieving videos info using given keyword
*****************************************************************************/
bool CMediaDataProvider::GetAllVideosByKeyword(QString keyword)
{
    bool ret=false;
    if(keyword.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

//    QByteArray data = keyword.toUtf8();
    QTextCodec *codec2 = QTextCodec::codecForName("EUC-KR");
    QString utfStr = codec2->fromUnicode(keyword);
    LOG_TRACE << "first method::" << keyword << LOG_ENDL;
    LOG_TRACE << "second method::" << utfStr << LOG_ENDL;

    m_Keyword_AllVidoesByKeyword = keyword;
    m_Keyword1_AllVidoesByKeyword = utfStr;

    QString queryString;

    // setting query string
    queryString = gGetAllVideosByKeywrodQuery.arg(keyword, utfStr);

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

// Function Name                                            Parameters

// CMediaDataProvider::GetSongsCountByFolder        bool (OUT), QString (IN) - folder to search

//

// Explanation

// This method handles the query for retrieving songs count under respective folder

// It connects signal/slot for the query completion

//

// History

// 18-Oct-2011      Chetan Nanda                created for retrieving songs count inside particular folder
//
*****************************************************************************/
bool CMediaDataProvider::GetSongsCountByFolder(QString folderName)
{
    bool ret=false;
    if(folderName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }
    // replacing space with %20 escape character
    folderName = folderName.replace(gBlankSpace, gSpaceEscapeChar);
    m_FolderName_SongsCountByFolder = folderName;
    QString queryString;

    // setting query string
    queryString = gSongsCountByFolderQuery.arg(folderName);

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

// Function Name                                            Parameters

// CMediaDataProvider::GetImagesCountByFolder        bool (OUT), QString (IN) - folder to search

//

// Explanation

// This method handles the query for retrieving images count under respective folder

// It connects signal/slot for the query completion

//

// History

// 18-Oct-2011      Chetan Nanda                created for retrieving images count inside particular folder
*****************************************************************************/
bool CMediaDataProvider::GetImagesCountByFolder(QString folderName)
{
    bool ret=false;
    if(folderName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }
    folderName = folderName.replace(gBlankSpace, gSpaceEscapeChar);
    m_FolderName_ImagesCountByFolder = folderName;
    QString queryString;

    // setting query string
    queryString = gGetImagesCountByFolderQuery.arg(folderName);

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

// Function Name                                            Parameters

// CMediaDataProvider::GetVideosCountByFolder        bool (OUT), QString (IN) - folder to search

//

// Explanation

// This method handles the query for retrieving videos count under respective folder

// It connects signal/slot for the query completion

//

// History

// 18-Oct-2011      Chetan Nanda                created for retriving videos count inside particular folder
*****************************************************************************/
bool CMediaDataProvider::GetVideosCountByFolder(QString folderName)
{
    bool ret=false;
    if(folderName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }
    folderName = folderName.replace(gBlankSpace, gSpaceEscapeChar);
    m_FolderName_VideosCountByFolder = folderName;
    QString queryString;

    // setting query string
    queryString = gGetVideosCountByFolderQuery.arg(folderName);

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

// Function Name                                            Parameters

// CMediaDataProvider::GetAllSongsCount                 bool (OUT)

//

// Explanation

// This method handles the query for retrieving all songs count

// It connects signal/slot for the query completion

//

// History

// 18-Oct-2011      Chetan Nanda                created for retrieving all songs count
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsCount()
{
    //setting query string
    QString queryString;
    queryString = gGetAllSongsCountQuery;

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



bool CMediaDataProvider::IpodRequestData(ETrackerAbstarctor_Query_Ids queryId,QSqlDatabase ipoddb, ETrackerAbstarctor_OrderType orderType)
{

       qDebug() << "[TA]:"<<ipoddb;
QString queryString;
int noOfColumns;
//ETrackerAbstarctor_Query_Ids queryId_ = eDefault_Id;//need to check.
bool ret=false;

if(queryId==eGetAllSongsFromVolumeQuery_Id)
{

    qDebug() << "[TA] in GetAllSongs"<<ipoddb;

    //{change by junam 2013.04.29 for song sorting order
    //queryString.append(QString("select T_FilePath,T_Title from MasterTable where T_Title not like \"%1\" order by LTRIM(T_Title) COLLATE NOCASE ").arg(""));
    queryString.append(QString("select T_FilePath,T_Title from MasterTable"));
	//}changed by junam
//    queryId = MAP_QUERY_IDS.value(gGetAllSongsFromVolumeQuery_Key);
    queryId = eGetAllSongsFromVolumeQuery_Id;
    noOfColumns = GETALLSONGSFROMVOLUMEQUERY_DATATYPECOUNT;

    ret=  senddata(noOfColumns,queryId ,queryString,ipoddb);

}

else if(queryId==eGetAllSongsWithTitleCountFromVolumeQuery_Id)
{

    qDebug() << "[TA]::"<< eGetAllSongsWithTitleCountFromVolumeQuery_Id;

    queryString.append("select count(T_Title) FROM MasterTable");
    noOfColumns = GETALLSONGSWITHTITLECOUNTFROMVOLUMEQUERY_DATATYPECOUNT;
    ret= senddata(noOfColumns,queryId ,queryString,ipoddb);

}

else if(queryId==eGetAllSongsWithLimitFromVolumeQuery_Id)
{

    qDebug() << "[TA]::"<< eGetAllSongsWithLimitFromVolumeQuery_Id;
//{change by junam 2013.04.29 for song sorting order
    //queryString.append(QString("select T_FilePath,T_Title from MasterTable where T_Title not like \"%1\" order by LTRIM(T_Title) COLLATE NOCASE LIMIT %2 OFFSET %3 ").arg("").arg(m_Query_Limit).arg(m_Query_Offset));
    queryString.append(QString("select T_FilePath,T_Title from MasterTable LIMIT %1 OFFSET %2 ").arg(m_Query_Limit).arg(m_Query_Offset));
	//}changed by junam
    noOfColumns = GETALLSONGSWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
    ret=  limitsenddata(noOfColumns,queryId ,queryString,ipoddb);

}
else if(queryId==eGetAllAlbumsWithLimitFromVolumeQuery_Id)
{

    qDebug() << "[TA]::"<< eGetAllAlbumsWithLimitFromVolumeQuery_Id;
    //{change by junam 2013.05.23 for sorting
    //queryString.append(QString("select DISTINCT T_Album ,T_Artist from MasterTable where T_Album not like \"%1\" order by LTRIM(T_Album) COLLATE NOCASE LIMIT %2 OFFSET %3").arg("").arg(m_Query_Limit).arg(m_Query_Offset));
    queryString.append(QString("select DISTINCT T_Album ,T_Artist from MasterTable where T_Album not like \"%1\"").arg(""));
    queryString.append(getOrderByMethod("T_Album"));
    queryString.append(QString(" LIMIT %1 OFFSET %2").arg(m_Query_Limit).arg(m_Query_Offset));
    //}added by junam
    noOfColumns = GETALLALBUMSWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
    ret=  limitsenddata(noOfColumns,queryId ,queryString,ipoddb);

}
else if(queryId==eGetAllMusicArtistsWithLimitFromVolumeQuery_Id)
{

    qDebug() << "[TA]::"<< eGetAllMusicArtistsWithLimitFromVolumeQuery_Id;
    //{change by junam 2013.05.23 for sorting
    //queryString.append(QString("select DISTINCT T_Artist from MasterTable where T_Artist not like \"%1\" order by LTRIM(T_Artist) COLLATE NOCASE LIMIT %2 OFFSET %3 ").arg("").arg(m_Query_Limit).arg(m_Query_Offset));
    queryString.append(QString("select DISTINCT T_Artist from MasterTable where T_Artist not like \"%1\"  ").arg(""));
    queryString.append(getOrderByMethod("T_Artist"));
    queryString.append(QString("  LIMIT %1 OFFSET %2 ").arg(m_Query_Limit).arg(m_Query_Offset));
    //}changed by junam
    noOfColumns = GETALLMUISCARTISTSWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
    ret=  limitsenddata(noOfColumns,queryId ,queryString,ipoddb);

}
else if(queryId==eGetAllMusicComposersWithLimitFromVolumeQuery_Id)
{

    qDebug() << "[TA]::"<< m_Query_Offset<<eGetAllMusicComposersWithLimitFromVolumeQuery_Id;
    //{change by junam 2013.05.23 for sorting
    //queryString.append(QString("select DISTINCT T_Composer from MasterTable where T_Composer not like \"%1\" order by LTRIM(T_Composer) COLLATE NOCASE LIMIT %2 OFFSET %3").arg("").arg(m_Query_Limit).arg(m_Query_Offset));
    queryString.append(QString("select DISTINCT T_Composer from MasterTable where T_Composer not like \"%1\" ").arg(""));
    queryString.append(getOrderByMethod("T_Composer"));
    queryString.append(QString(" LIMIT %1 OFFSET %2").arg(m_Query_Limit).arg(m_Query_Offset));
    //}changed by junam
    noOfColumns = GETALLMUSICCOMPOSERSWITHLIMITFORMVOLUMEQUERY_DATATYPECOUNT;
    ret=  limitsenddata(noOfColumns,queryId ,queryString,ipoddb);

}
else if(queryId==eGetAllMusicGenresWithLimitFromVolumeQuery_Id)
{

    qDebug() << "[TA]::"<< eGetAllMusicGenresWithLimitFromVolumeQuery_Id;
    //{change by junam 2013.05.23 for sorting
    //queryString.append(QString("select DISTINCT T_Genre from MasterTable where T_Genre not like \"%1\" order by LTRIM(T_Genre) COLLATE NOCASE LIMIT %2 OFFSET %3 ").arg("").arg(m_Query_Limit).arg(m_Query_Offset));
    queryString.append(QString("select DISTINCT T_Genre from MasterTable where T_Genre not like \"%1\" ").arg(""));
    queryString.append(getOrderByMethod("T_Genre"));
    queryString.append(QString(" LIMIT %1 OFFSET %2 ").arg(m_Query_Limit).arg(m_Query_Offset));
    //}changed by junam
    noOfColumns = GETALLMUSICGENRESWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
    ret=  limitsenddata(noOfColumns,queryId ,queryString,ipoddb);

}
else if(queryId==eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id)
{

    qDebug() << "[TA]::"<< m_Query_Offset;

    //{change by junam 2013.05.23 for sorting
    //queryString.append(QString("select DISTINCT T_Album ,T_Artist, T_FilePath from MasterTable  where T_Album not like \"%1\" group by T_Album order by LTRIM(T_Album) COLLATE NOCASE LIMIT %2 OFFSET %3").arg("").arg(m_Query_Limit).arg(m_Query_Offset));
    queryString.append(QString("select DISTINCT T_Album ,T_Artist, T_FilePath from MasterTable  where T_Album not like \"%1\" group by T_Album ").arg(""));
    queryString.append(getOrderByMethod("T_Album"));
    queryString.append(QString("LIMIT %1 OFFSET %2").arg(m_Query_Limit).arg(m_Query_Offset));
    //}changed by junam

    noOfColumns = GETALLALBUMSWITHCOVERARTWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
    ret=  limitsenddata(noOfColumns,queryId ,queryString,ipoddb);

}



else if(queryId==eGetAllAlbumsWithCoverArtFromVolumeQuery_Id)
{

    qDebug() << "[TA] in COVER ART FROM VOLUME";
    //{change by junam 2013.05.23 for sorting
    //queryString.append(QString("select DISTINCT T_Album ,T_Artist from MasterTable where T_Album not like \"%1\"  order by LTRIM(T_Album) COLLATE NOCASE ").arg(""));
    queryString.append(QString("select DISTINCT T_Album ,T_Artist from MasterTable where T_Album not like \"%1\"  ").arg(""));
    queryString.append(getOrderByMethod("T_Album"));
    //}changed by junam
//    queryId = MAP_QUERY_IDS.value(gGetAllAlbumsWithCoverArtFromVolumeQuery_Key);
    queryId = eGetAllAlbumsWithCoverArtFromVolumeQuery_Id;
    noOfColumns = GETALLALBUMSWITHCOVERARTFROMVOLUMEQUERY_DATATYPECOUNT;

    ret=   senddata(noOfColumns,queryId ,queryString,ipoddb);

}

else if(queryId==eGetAllAlbumsFromVolumeQuery_Id)
{
    //{change by junam 2013.05.23 for sorting
    //queryString.append(QString("select DISTINCT T_Album ,T_Artist from MasterTable where T_Album not like \"%1\"  order by LTRIM(T_Album) COLLATE NOCASE").arg(""));
    queryString.append(QString("select DISTINCT T_Album ,T_Artist from MasterTable where T_Album not like \"%1\" ").arg(""));
    queryString.append(getOrderByMethod("T_Album"));
    //}changed by junam
//    queryId = MAP_QUERY_IDS.value(gGetAllAlbumsFromVolumeQuery_Key);
    queryId = eGetAllAlbumsFromVolumeQuery_Id;
    noOfColumns = GETALLALBUMSFROMVOLUMEQUERY_DATATYPECOUNT;

    ret=   senddata(noOfColumns,queryId ,queryString,ipoddb);

}

else if(queryId==eGetAllAlbumsCountFromVolumeQuery_Id)
{

    queryString.append(QString("select COUNT(DISTINCT T_Album) from MasterTable where T_Album not like \"%1\" ").arg(""));
    queryId = eGetAllAlbumsCountFromVolumeQuery_Id;
    noOfColumns = 1;

    ret=   senddata(noOfColumns,queryId ,queryString,ipoddb);

}

else if(queryId== eGetAllMusicArtistsFromVolumeQuery_Id)
{
    //{change by junam 2013.05.23 for sorting
    //queryString.append(QString("select DISTINCT T_Artist from MasterTable where T_Artist not like \"%1\"  order by LTRIM(T_Artist) COLLATE NOCASE").arg(""));
    queryString.append(QString("select DISTINCT T_Artist from MasterTable where T_Artist not like \"%1\" ").arg(""));
    queryString.append(getOrderByMethod("T_Artist"));
    //}changed by junam

//    queryId = MAP_QUERY_IDS.value(gGetAllMusicArtistsFromVolumeQuery_Key);
    queryId = eGetAllMusicArtistsFromVolumeQuery_Id;
    noOfColumns = GETALLMUISCARTISTSQUERY_DATATYPECOUNT;

    ret=  senddata(noOfColumns,queryId ,queryString,ipoddb);

}

else if(queryId== eGetAllMusicArtistsCountFromVolumeQuery_Id)
{

    queryString.append("select COUNT(DISTINCT T_Artist) from MasterTable");
    queryId = eGetAllMusicArtistsCountFromVolumeQuery_Id;
    noOfColumns = 1;

    ret=  senddata(noOfColumns,queryId ,queryString,ipoddb);

}


else if(queryId== eGetAllMusicComposersFromVolumeQuery_Id)
{
    //{change by junam 2013.05.23 for sorting
    // queryString.append(QString("select DISTINCT T_Composer from MasterTable where T_Composer not like \"%1\" order by LTRIM(T_Composer) COLLATE NOCASE").arg(""));
    queryString.append(QString("select DISTINCT T_Composer from MasterTable where T_Composer not like \"%1\" ").arg(""));
    queryString.append(getOrderByMethod("T_Composer"));
    //}changed by junam
//    queryId = MAP_QUERY_IDS.value(gGetAllMusicComposersFromVolumeQuery_Key);
    queryId = eGetAllMusicComposersFromVolumeQuery_Id;
    noOfColumns = GETALLMUSICCOMPOSERSQUERY_DATATYPECOUNT;

    ret= senddata(noOfColumns,queryId ,queryString,ipoddb);

}

else if(queryId== eGetAllMusicGenresFromVolumeQuery_Id)
{
    //{change by junam 2013.05.23 for sorting
    //queryString.append(QString("select DISTINCT T_Genre from MasterTable where T_Genre not like \"%1\" order by LTRIM(T_Genre) COLLATE NOCASE").arg(""));
    queryString.append(QString("select DISTINCT T_Genre from MasterTable where T_Genre not like \"%1\" ").arg(""));
    queryString.append(getOrderByMethod("T_Genre"));
    //}changed by junam
//    queryId = MAP_QUERY_IDS.value(gGetAllMusicGenresFromVolumeQuery_Key);
    queryId = eGetAllMusicGenresFromVolumeQuery_Id;
    noOfColumns = GETALLMUSICGENRESQUERY_DATATYPECOUNT;

    ret= senddata(noOfColumns,queryId ,queryString,ipoddb);

}
else if(queryId== eGetlistofPlaylistsQuery_Id)
{

    queryString.append(QString("select T_PlayListNames from PlayListProperty where T_PlayListNames not like \"%1\" order by T_PlayListNames ").arg(""));
//    queryId = MAP_QUERY_IDS.value(gGetlistofPlaylistsQuery_Key);
    queryId = eGetlistofPlaylistsQuery_Id;
    noOfColumns = 1;

    ret= senddata(noOfColumns,queryId ,queryString,ipoddb);

}
else if(queryId== eGetAllAlbumsWithUrlFromVolumeQuery_Id)
{
    //{change by junam 2013.05.23 for sorting
    //queryString.append(QString("select T_Album ,T_Artist,T_FilePath from MasterTable where T_Album not like \"%1\" order by LTRIM(T_Album) COLLATE NOCASE").arg(""));
    queryString.append(QString("select DISTINCT T_Album ,T_Artist,T_FilePath from MasterTable where T_Album not like \"%1\" group by T_Album").arg(""));
    queryString.append(getOrderByMethod("T_Album"));
    //}changed by junam
    noOfColumns = GETALLALBUMSWITHURLFROMVOLUMEQUERY_DATATYPECOUNT;

    ret= senddata(noOfColumns,queryId ,queryString,ipoddb);

}
else if(queryId == eIsThereAnySongWithoutAlbumFromVolumeQuery_Id)
{
    qDebug() << "[TA] in IsThereAnySong";
    ret = IsThereAnySongWithoutAlbumFromIPOD(ipoddb);
}
else if(queryId == eGetAllSongsWithoutAlbumFromVolumeQuery_Id)
{
     qDebug() << "[TA] in GetAllSongsWithoutAlbumFromVolume";
     //{change by junam 2013.05.23 for sorting
     //queryString.append(QString("select T_FilePath,T_Title FROM MasterTable where T_Album like \"%1\" order by LTRIM(T_Title) COLLATE NOCASE").arg(""));
     queryString.append(QString("select T_FilePath,T_Title FROM MasterTable where T_Album like \"%1\"").arg(""));
     queryString.append(getOrderByMethod("T_Title"));
     //}changed by junam
     noOfColumns = GETALLSONGSWITHOUTMETAINFOFROMVOLUMEQUERY_DATATYPECOUNT;

     ret= senddata(noOfColumns,queryId ,queryString,ipoddb);
}
else  if(queryId== eGetAllImagesFromVolumeQuery_Id)
{
    return  GetAllImagesFromVolume(eIPod);
}
else  if(queryId== eGetAllVideosFromVolumeQuery_Id)
{
    ret= GetAllVideosFromVolume(eIPod);
}
else  if(queryId== eGetMostPlayedMusicSongsQuery_Id)
{
    return  GetMostPlayedMusicSongs(eIPod);

}
else if(queryId== eGetMostRecentlyPlayedSongsQuery_Id)
{
    return  GetMostRecentlyPlayedSongs(eIPod);

}
else if(queryId==  eGetAllVideosCountFromVolumeQuery_Id)
{
    return GetAllVideosCountFromVolume(eIPod);
}
else if (queryId== eGetAllImagesCountFromVolumeQuery_Id)
{
    return GetAllImagesCountFromVolume(eIPod);
}
else if(queryId == eGetAllMetaDataWithLimitFromVolumeQuery_Id)
{
    qDebug() << "[TA] in eGetAllMetaDataWithLimitFromVolumeQuery_Id";
    QString useOrderType;

    switch(orderType)
    {
    case eAlbum:
        useOrderType = "T_Album";
        break;
    case eArtist:
        useOrderType = "T_Artist";
        break;
    case eGenre:
        useOrderType = "T_Genre";
        break;
    case eTitle:
        useOrderType = "T_Title";
        break;
    case eUrl:
        useOrderType = "T_FilePath";
        break;
    }

   //{change by junam 2013.05.23 for sorting
    //queryString.append(QString("select T_Title, T_FilePath, T_Album, T_Artist, T_Genre, T_Composer FROM MasterTable where T_Album not like \"%1\" order by LTRIM(%2) COLLATE NOCASE LIMIT 1 OFFSET %3").arg("").arg(useOrderType).arg(m_Query_Offset));
    queryString.append(QString("select T_Title, T_FilePath, T_Album, T_Artist, T_Genre, T_Composer FROM MasterTable where T_Album not like \"%1\" ").arg(""));
    queryString.append(getOrderByMethod(useOrderType));
    queryString.append(QString(" LIMIT 1 OFFSET %1").arg(m_Query_Offset));
    //}changed by junam
    noOfColumns = GETALLMETADATAWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;

    ret= senddata(noOfColumns,queryId ,queryString,ipoddb);
}


return ret;



}
bool CMediaDataProvider::IsThereAnySongWithoutAlbumFromIPOD(QSqlDatabase ipoddb)
{
    //LOG_TRACE << "Inside IsThereAnySongWithoutAlbumFromVolume" << LOG_ENDL;
    qDebug() << "[TA] in IsThereAnySong:1";

    bool ret=false;
    QString CountSongsWithMetaInfo = "";
    QString CountAllSongs = "";
    QString CountSongsWithoutMetaInfo = "";
    QString CountTemp = "%1";
    QVector<QStringList> queryDataList;
    QStringList songsPresent;

    ret = GetAllSongsCountWithMetaInfoFromIPOD_sync(ipoddb,CountSongsWithMetaInfo);
    if(!ret)
        return ret;
    ret = GetAllSongsCountFromIPOD_sync(ipoddb,CountAllSongs);
    if(!ret)
        return ret;

    CountSongsWithoutMetaInfo = CountTemp.arg(CountAllSongs.toInt() -  CountSongsWithMetaInfo.toInt());

    if(CountSongsWithoutMetaInfo.toInt() > 0)
        songsPresent.append("1");
    else
        songsPresent.append("0");

    queryDataList.append(songsPresent);
    emit DataFetched(queryDataList, eIsThereAnySongWithoutAlbumFromVolumeQuery_Id);

    return ret;
}

void CMediaDataProvider::exec(QSparqlQuery* query)
{
    m_pQueryResult = m_pSparql_Connection->exec(*query);
    LOG_HIGH << Q_FUNC_INFO << "SPARQL query: " << query->query() << LOG_ENDL;
}

bool CMediaDataProvider::exec(QSqlQuery* query)
{
    bool result = query->exec();
    LOG_HIGH << Q_FUNC_INFO << "SQL query: " << query->lastQuery() << LOG_ENDL;
    if (query->lastError().isValid()) {
        LOG_CRITICAL << Q_FUNC_INFO << " last error: " << query->lastError().text() << LOG_ENDL;
    }
    return result;
}

bool CMediaDataProvider::GetAllSongsCountFromIPOD_sync(QSqlDatabase ipoddb, QString& Count)
{
     qDebug() << "[TA] in IsThereAnySong:2";
    bool ret=false;
    QString queryString = "select count (*) FROM MasterTable";

   QSqlQuery* query = new QSqlQuery(queryString,ipoddb);
   ret=exec(query);
   qDebug() << "  [TA] exec successfully"<<ret;

    // if query executed successfully then connecting signal/slot for result
  if(ret)
   {
        while(query->next())
        {
            Count.append(query->value(0).toString());
        }
    }
        else
        {
            HandleError(eErrorInQueryExecution);
            return ret;
        }
 delete query;
    return ret;
}
bool CMediaDataProvider::GetAllSongsCountWithMetaInfoFromIPOD_sync(QSqlDatabase ipoddb, QString& Count)
{
     qDebug() << "[TA] in IsThereAnySong:3";
    bool ret=false;

   QString queryString ;
    queryString.append("select count(*) FROM MasterTable where T_Album like \"%1\" ").arg("");

   QSqlQuery* query = new QSqlQuery(queryString,ipoddb);
   ret=exec(query);
   qDebug() << "  [TA] exec successfully"<<ret;

    // if query executed successfully then connecting signal/slot for result
  if(ret)
   {
        while(query->next())
        {
            Count.append(query->value(0).toString());
        }
    }
        else
        {
            HandleError(eErrorInQueryExecution);
            return ret;
        }
 delete query;

    return ret;
}




bool CMediaDataProvider::IpodRequestsearchData(QStringList searchList,ETrackerAbstarctor_Query_Ids queryId,QSqlDatabase ipoddb)
{

    QString queryString;
    int noOfColumns = 0;
    //ETrackerAbstarctor_Query_Ids queryId = eDefault_Id;//need to check.

    bool ret=false;

    LOG_TRACE << "[TA] Getallsongs" << LOG_ENDL;
    switch (queryId)
    {
   case eGetAllSongsByComposerAlbumQuery_Id:
    {
              queryString.clear();
              queryString.append( QString("select DISTINCT  T_FilePath ,T_Title from MasterTable where T_Composer = \"%1\" AND T_Album = \"%2\" AND T_Title not like \"%3\" order by T_Title ").arg(searchList[0]).arg(searchList[1]).arg(""));

              noOfColumns = GETALLSONGSBYCOMPOSERALBUM;

    }
   break;
   case eGetAllAlbumByGenreComposerQuery_Id:
    {
              queryString.clear();
              queryString.append( QString("select DISTINCT  T_Album from MasterTable where T_Genre = \"%1\" AND T_Composer = \"%2\" AND T_Album not like \"%3\" order by T_Album ").arg(searchList[0]).arg(searchList[1]).arg(""));

              noOfColumns = GETALLALBUMBYGENERECOMPOSER;

    }

    break;
    case eGetAllAlbumByComposerQuery_Id:
    {
              queryString.clear();
//{add arist file by junam 2013.10.28 for 2nd depth coverart
              queryString.append(QString("select DISTINCT  T_Album ,T_Artist,T_FilePath from MasterTable where T_Composer = \"%1\" AND T_Album not like \"%2\" group by T_Album  order by T_Album ").arg(searchList[0]).arg(""));

              noOfColumns = GETALLALBUMBYCOMPOSER;

    }
   break;
   case eGetAllComposerByGenreQuery_Id:
    {
              queryString.clear();
              queryString.append(QString("select DISTINCT T_Composer from MasterTable where T_Genre = \"%1\" AND T_Composer not like \"%2\" order by T_Composer ").arg(searchList[0]).arg(""));

              noOfColumns = GETALLCOMPOSERBYGENER;

    }
    break;
    case eGetAllSongsByGenreArtistAlbumQuery_Id:
    {
              queryString.clear();
              queryString.append(QString("select T_Title ,T_FilePath from MasterTable where T_Genre = \"%1\" AND  T_Artist = \"%2\" AND  T_Album = \"%3\" AND T_Title not like \"%4\" order by T_Title").arg(searchList[0]).arg(searchList[1]).arg(searchList[2]).arg(""));
//              queryId = MAP_QUERY_IDS.value(gGetAllSongsByGenreArtistAlbumQuery_Key);
              queryId = eGetAllSongsByGenreArtistAlbumQuery_Id;
              noOfColumns = GETALLSONGSBYGENREARTISTALBUMQUERY_DATATYPECOUNT;


    }
    break;
    case eGetAllSongsByGenreComposerAlbumQuery_Id:
     {

              queryString.clear();
              queryString.append(QString("select T_Title ,T_FilePath from MasterTable where T_Genre = \"%1\" AND T_Composer = \"%2\" AND  T_Album = \"%3\" AND T_Title not like \"%4\" order by T_Title").arg(searchList[0]).arg(searchList[1]).arg(searchList[2]).arg(""));
//              queryId = MAP_QUERY_IDS.value(gGetAllSongsByGenreComposerAlbumQuery_Key);
              queryId = eGetAllSongsByGenreComposerAlbumQuery_Id;
              noOfColumns = GETALLSONGSBYGENRECOMPOSERALBUMQUERY_DATATYPECOUNT;

     }
    break;
    case eGetAllSongsByArtistAlbumQuery_Id:
    {

              queryString.clear();

              queryString.append( QString("select DISTINCT T_FilePath ,T_Title from MasterTable where T_Artist = \"%1\" AND T_Album = \"%2\" AND T_Title not like \"%3\" order by T_Title").arg(searchList[0]).arg(searchList[1]).arg(""));

              noOfColumns = GETALLSONGSBYARTISTALBUMQUERY_DATATYPECOUNT;


    }
   break;
   case eGetAllAlbumByGenreArtistQuery_Id:
    {
              queryString.clear();

              queryString.append(QString("select DISTINCT T_Album from MasterTable where T_Genre = \"%1\" AND T_Artist = \"%2\" AND T_Album not like \"%3\" order by T_Album").arg(searchList[0]).arg(searchList[1]).arg(""));

              noOfColumns = GETALLALBUMBYGENREARTISTQUERY_DATATYPECOUNT;

    }
    break;
    case eGetAllAlbumByArtistQuery_Id:
    {
              queryString.clear();
//add file by junam 2013.10.28 for 2nd depth coverart
              queryString.append(QString("select DISTINCT T_Album, T_FilePath from MasterTable where T_Artist = \"%1\" AND T_Album not like \"%2\" group by T_Album order by T_Album ").arg(searchList[0]).arg(""));

              noOfColumns = GETALLALBUMBYARTISTQUERY_DATATYPECOUNT;

    }
   break;
   case eGetAllArtistByGenreQuery_Id:
    {

              queryString.clear();

              queryString.append( QString("select DISTINCT T_Artist from MasterTable where T_Genre = \"%1\" AND T_Artist not like \"%2\" order by T_Artist").arg(searchList[0]).arg(""));

              noOfColumns = GETALLARTISTBYGENREQUERY_DATATYPECOUNT;

    }

   break;

    default:
        break;

}

    ret=  senddata(noOfColumns,queryId ,queryString,ipoddb);

    return ret;
}


bool CMediaDataProvider:: GetSearchdataByKeywordFromVolume(QString searchData,ETrackerAbstarctor_Query_Ids queryId,QSqlDatabase ipoddb)
{

    QString queryString;
    int noOfColumns = 0;
    bool ret=false;

    if(searchData.isEmpty())
    {
        HandleError(eEmptyString);
        return ret;
    }

    //Convert string to EUC-KR also to search
    QTextCodec *codec = QTextCodec::codecForName("EUC-KR");
    QByteArray euckrStr = codec->fromUnicode(searchData);
    switch (queryId)
    {

    case eGetAllsongsByKeywordFromVolumeQuery_Id:
    {
        queryString.clear();
        queryString.append( QString("select T_Title, T_Album ,T_Artist ,T_Genre, T_FilePath from MasterTable where T_Title  like  '%%1%' order by T_Title ").arg(searchData));

        noOfColumns = GETALLSONGSBYKEYWORDFROMVOLUME_DATATYPECOUNT;

    }
            break;
        case eGetAllAlbumsByKeywordFromVolumeQuery_Id:
    {
              queryString.clear();
              queryString.append( QString("select T_Title, T_Album ,T_Artist ,T_Genre, T_FilePath from MasterTable where T_Album  like  '%%1%' order by T_Album").arg(searchData));

              noOfColumns = GETALLALBUMSYKEYWORDFROMVOLUME_DATATYPECOUNT;

    }
 break;
        case eGetAllArtistsByKeywordFromVolumeQuery_Id:
    {
              queryString.clear();
              queryString.append( QString("select T_Title, T_Album ,T_Artist ,T_Genre, T_FilePath from MasterTable where T_Artist  like  '%%1%' order by T_Artist").arg(searchData));

              noOfColumns = GETALLARTISTBYKEYWORDFROMVOLUME_DATATYPECOUNT;

    }
 break;
        case eGetAllGenresByKeywordFromVolumeQuery_Id:
    {
              queryString.clear();
              queryString.append( QString("select T_Title, T_Album ,T_Artist ,T_Genre, T_FilePath from MasterTable where T_Genre  like  '%%1%' order by T_Genre").arg(searchData));
              //queryId = eGetAllSongsByComposerAlbumQuery_Id;
              noOfColumns = GETALLGENREBYKEYWORDFROMVOLUME_DATATYPECOUNT;

    }
  break;
        case eGetAllSongsByAlbumFromVolumeQuery_Id:
    {
              queryString.clear();
              searchData.replace(QByteArray("\""), QByteArray("\"\"")); // added by eugene.seo 2013.05.20
              //{changed by junam 2013.06.21 for album selection
              //queryString.append( QString("select T_Title,T_FilePath from MasterTable where T_Album like \"%1\" order by T_Title ").arg(searchData));
              queryString.append( QString("select T_Title,T_FilePath from MasterTable where T_Album = \"%1\" order by T_Title ").arg(searchData));
              //}changed by junam

              //queryId = eGetAllSongsByComposerAlbumQuery_Id;
              noOfColumns = GETALLSONGSBYCOMPOSERALBUM;
    }
             break;

        case eGetAllSongsbyPlaylistQuery_Id:
       {

        queryString.clear();

        qDebug() << "[TA] in playlist"<<searchData;

        queryString.append(QString("select T_Title, T_FilePath from \"%1\" order by T_Title").arg(searchData));;

//        queryId = MAP_QUERY_IDS.value(gGetAllSongsbyPlaylistQuery_Key);
        queryId = eGetAllSongsbyPlaylistQuery_Id;
        noOfColumns=GETPLAYLISTDATA;

       }
     break;


     ///////
    case eGetAllSongsWithTitleIndexFromVolumeQuery_Id:
    {
        m_Letter_GetAllSongsWithTitleIndexFromVolume = searchData;

       queryString.clear();
       //{change by junam 2013.05.23 for sorting
       //queryString.append(QString("select T_Title from MasterTable where T_Title not like \"%1\" order by LTRIM(T_Title) COLLATE NOCASE ").arg(""));
       queryString.append(QString("select T_Title from MasterTable where T_Title not like \"%1\"  ").arg(""));
       //queryString.append(getOrderByMethod("T_Title")); //removed by junam 2013.06.06 for iPod quick search
       //}changed by junam

       noOfColumns=GETALLSONGSWITHTITLEINDEXFROMVOLUMEQUERY_DATATYPECOUNT;

        qDebug() << "[TA] string "<<queryString;

    }
    break;
    case eGetAllMusicArtistsIndexFromVolumeQuery_Id:
    {

       m_Letter_GetAllMusicArtistsIndexFromVolume = searchData;
       queryString.clear();
       //{change by junam 2013.05.23 for sorting
       //queryString.append(QString("select DISTINCT T_Artist from MasterTable where T_Artist not like \"%1\" order by LTRIM(T_Artist) COLLATE NOCASE").arg(""));
       queryString.append(QString("select DISTINCT T_Artist from MasterTable where T_Artist not like \"%1\" ").arg(""));
       queryString.append(getOrderByMethod("T_Artist"));
       //}changed by junam
       noOfColumns=GETALLMUISCARTISTSFROMVOLUMEQUERY_DATATYPECOUNT;

    }
    break;
    case eGetAllMusicGenresIndexFromVolumeQuery_Id:
    {
        m_Letter_GetAllMusicGenresIndexFromVolume = searchData;

       queryString.clear();
       //{change by junam 2013.05.23 for sorting
       //queryString.append(QString("select DISTINCT T_Genre from MasterTable where T_Genre not like \"%1\" order by LTRIM(T_Genre) COLLATE NOCASE ").arg(""));
       queryString.append(QString("select DISTINCT T_Genre from MasterTable where T_Genre not like \"%1\" ").arg(""));
       queryString.append(getOrderByMethod("T_Genre"));
       //}changed by junam
       noOfColumns=GETALLMUSICGENRESFROMVOLUMEQUERY_DATATYPECOUNT;

    }
    break;
    case eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id:
    {

        m_Letter_GetAllAlbumsWithCoverArtIndexFromVolume = searchData;
       queryString.clear();
       //{change by junam 2013.05.23 for sorting
       //queryString.append(QString("select DISTINCT T_Album from MasterTable where T_Album not like \"%1\" order by LTRIM(T_Album) COLLATE NOCASE").arg(""));
       queryString.append(QString("select DISTINCT T_Album from MasterTable where T_Album not like \"%1\" ").arg(""));
       queryString.append(getOrderByMethod("T_Album"));
       //}changed by junam
       noOfColumns=GETALLALBUMSWITHCOVERARTINDEXFROMVOLUMEQUERY_DATATYPECOUNT;

    }
    break;
    case eGetAllSongsByArtistFromVolumeQuery_Id:
        {
            queryString.clear();

            queryString.append( QString("select DISTINCT  T_Title , T_FilePath from MasterTable where T_Artist = \"%1\" AND T_Title not like \"%2\" order by T_Title").arg(searchData).arg(""));

            noOfColumns = GETALLSONGSBYARTISTFROMVOLUMEQUERY_DATATYPECOUNT;
        }
        break;
	//{added by junam 2012.03.19 for ISV 76018
    case eGetAllSongsByGenreFromVolumeQuery_Id:
    {
        queryString.clear();

        queryString.append( QString("select DISTINCT  T_Title , T_FilePath from MasterTable where T_Genre = \"%1\" AND T_Title not like \"%2\" group by T_Title").arg(searchData).arg(""));
        queryString.append(getOrderByMethod("T_Title")); //added by jaehwan 2013.11.20 for ipod sorting order for genre
        noOfColumns = GETALLSONGSBYGENREFROMVOLUMEQUERY_DATATYPECOUNT;
    }
	//}added by junam
        default:
            break;

}
    ret=  senddata(noOfColumns,queryId ,queryString,ipoddb);

   return  ret;
}

bool CMediaDataProvider::senddata(int noOfColumns,ETrackerAbstarctor_Query_Ids queryId ,QString queryString,QSqlDatabase ipoddb)
{

    qDebug() << "[TA] in send data"<<queryId <<queryString;

    QVector<QStringList> queryDataList;
    bool  ret=false ;
    QSqlQuery* query = new QSqlQuery(queryString,ipoddb);
    ret=exec(query);
    qDebug() << "  [TA] exec successfully"<<ret;

    QString letter;
    letter.clear();

    switch(queryId)
    {
    case eGetAllSongsWithTitleIndexFromVolumeQuery_Id:
    {
        letter = m_Letter_GetAllSongsWithTitleIndexFromVolume;
    }
        break;
    case eGetAllMusicArtistsIndexFromVolumeQuery_Id:
    {
        letter = m_Letter_GetAllMusicArtistsIndexFromVolume;
    }
        break;
    case eGetAllMusicGenresIndexFromVolumeQuery_Id:
    {
        letter = m_Letter_GetAllMusicGenresIndexFromVolume;
    }
        break;
    case eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id:
    {
        letter = m_Letter_GetAllAlbumsWithCoverArtIndexFromVolume;
    }
        break;
    default:
    {

        //Execute code for the rest of queries
        if (ret)
        {
            queryDataList.clear();

            while(query->next())
            {
                QStringList columnList;


                for (int i = 0; i < noOfColumns; i++)
                {
                    QString value = query->value(i).toString();
                    //{ modified by jaehwan 2014.1.9 for % deconfig issue
                    if (value.startsWith(QLatin1String("file://")))
                    {
                        value = QUrl::fromPercentEncoding(value.toUtf8());
                    }
                    columnList.append(value);
                    //} modified by jaehwan
                }
                queryDataList.append(columnList);

            }
            qDebug()<< "SendData queryDataList: " << queryDataList;
            emit DataFetched(queryDataList, queryId);

        }
    }

    }

    if( (queryId == eGetAllSongsWithTitleIndexFromVolumeQuery_Id) ||
            (queryId == eGetAllMusicArtistsIndexFromVolumeQuery_Id)   ||
            (queryId == eGetAllMusicGenresIndexFromVolumeQuery_Id)    ||
            (queryId == eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id) )
    {
        PrepareIndexDataList(query, queryDataList, queryId, letter, true);
    }

    delete query;
    return ret;
}

bool CMediaDataProvider::limitsenddata(int noOfColumns,ETrackerAbstarctor_Query_Ids queryId ,QString queryString,QSqlDatabase ipoddb)
{

    qDebug() << "[TA] in send data"<<queryId <<queryString;

  bool  ret=false ;
 QSqlQuery* query = new QSqlQuery(queryString,ipoddb);
 ret=exec(query);
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
                         //{ modified by jaehwan 2014.1.9 for % deconfig issue
                         if (value.startsWith(QLatin1String("file://")))
                         {
                             value = QUrl::fromPercentEncoding(value.toUtf8());
                         }
                         columnList.append(value);
                         //} modified by jaehwan.
                       }
                       queryDataList.append(columnList);

                     }

                emit DataLimitFetched(queryDataList, queryId, m_Query_Offset);


                }
               delete query;
             return ret;


          }


/******************************************************************************

// Function Name                                            Parameters

// CMediaDataProvider::GetAllSongsFromVolume        bool (OUT), enum ETrackerAbstarctor_VolumeType  (IN) - volume to search

//

// Explanation

// This method handles the query for searching all songs from given volume

// It connects signal/slot for the query completion

//

// History

// 20-oct-2011      Chetan Nanda              created for seaching all songs from given volume
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsFromVolume(enum ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsFromVolume);
    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllSongsFromVolume.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }

    if(!ret)
        return ret;

    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("title"), m_ExtendOrdering) )
            return false;

        //m_ExtendOrdering = "(REGEX('*o*', ?title)) ";
        //m_ExtendOrdering = " ((?title >= 'A') && (?title <= 'z')) ";
        //m_ExtendOrdering = " !(REGEX(?title, 'Th*')) "; //

        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllSongsFromVolumeExtOrderQuery.arg( m_VolumeName_GetAllSongsFromVolume, m_ExtendOrdering);
    }
    else 
    {
        queryString = gGetAllSongsFromVolumeQuery.arg( m_VolumeName_GetAllSongsFromVolume);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsFromVolume);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	
    //}   

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

// CMediaDataProvider::GetAllAlbumsFromVolume        bool (OUT), enum ETrackerAbstarctor_VolumeType (IN) - volume to search

//

// Explanation

// This method handles the query for searching all albums from given volume

// It connects signal/slot for the query completion

//

// History

// 20-oct-2011      Chetan Nanda              created for seaching all albums from given volume
*****************************************************************************/
bool CMediaDataProvider::GetAllAlbumsFromVolume(enum ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeNameList.clear();

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllAlbumsFromVolume);
    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllAlbumsFromVolume.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }

    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllAlbumsFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsFromVolume);

    //    // executing query
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

// CMediaDataProvider::GetAllMusicArtistsFromVolume        bool (OUT), enum ETrackerAbstarctor_VolumeType (IN) - volume to search

//

// Explanation

// This method handles the query for searching all artists from given volume

// It connects signal/slot for the query completion

//

// History

// 21-oct-2011      Chetan Nanda              created for seaching all artists from given volume
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicArtistsFromVolume(enum ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeNameList.clear();

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllMusicArtistsFromVolume);
    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllMusicArtistsFromVolume.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }

    if(!ret)
        return ret;

    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02

    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("performer"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllMusicArtistsFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllMusicArtistsFromVolume).arg(m_ExtendOrdering);
    }
    else 
    {
        queryString = gGetAllMusicArtistsFromVolumeQuery.arg(m_VolumeName_GetAllMusicArtistsFromVolume);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllMusicArtistsFromVolume);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	

    //}

/*
    QSparqlQuery QueryObject(gGetAllMusicArtistsFromVolumeQuery.arg(m_VolumeName_GetAllMusicArtistsFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllMusicArtistsFromVolume);

    //    // executing query
    exec(&QueryObject);
*/
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

// CMediaDataProvider::GetAllMusicComposersFromVolume        bool (OUT), enum ETrackerAbstarctor_VolumeType (IN) - volume to search

//

// Explanation

// This method handles the query for searching all composers from given volume

// It connects signal/slot for the query completion

//

// History

// 21-oct-2011      Chetan Nanda              created for seaching all compers from given volume
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicComposersFromVolume(enum ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeNameList.clear();

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllMusicComposersFromVolume);

    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllMusicComposersFromVolumeQuery.arg(m_VolumeName_GetAllMusicComposersFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllMusicComposersFromVolume);

    //    // executing query
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

// CMediaDataProvider::GetAllMusicGenresFromVolume        bool (OUT), enum ETrackerAbstarctor_VolumeType (IN) - volume to search

//

// Explanation

// This method handles the query for searching all genres from given volume

// It connects signal/slot for the query completion

//

// History

// 21-oct-2011      Chetan Nanda              created for seaching all genres from given volume
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicGenresFromVolume(enum ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeNameList.clear();

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllMusicGenresFromVolume);
    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllMusicGenresFromVolume.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }

    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllMusicGenresFromVolumeQuery.arg(m_VolumeName_GetAllMusicGenresFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllMusicGenresFromVolume);

    //    // executing query
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

// CMediaDataProvider::GetAllImagesFromVolume        bool (OUT), enum ETrackerAbstarctor_VolumeType (IN) - volume to search

//

// Explanation

// This method handles the query for searching all images from given volume

// It connects signal/slot for the query completion

//

// History

// 21-oct-2011      Chetan Nanda              created for seaching all images from given volume
*****************************************************************************/
bool CMediaDataProvider::GetAllImagesFromVolume(enum ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeNameList.clear();

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllImagesFromVolume);

    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllImagesFromVolumeQuery.arg(m_VolumeName_GetAllImagesFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllImagesFromVolume);


    //    // executing query
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

// CMediaDataProvider::GetAllVideosFromVolume        bool (OUT), enum ETrackerAbstarctor_VolumeType (IN) - volume to search

//

// Explanation

// This method handles the query for searching all videos from given volume

// It connects signal/slot for the query completion

//

// History

// 21-oct-2011      Chetan Nanda              created for seaching all videos from given volume
*****************************************************************************/
bool CMediaDataProvider::GetAllVideosFromVolume(enum ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeNameList.clear();

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllVideosFromVolume);

    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllVideosFromVolumeQuery.arg(m_VolumeName_GetAllVideosFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllVideosFromVolume);

    //    // executing query
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

// CMediaDataProvider::GetAllSongsWithMetaDataFromVolume        bool (OUT), ETrackerAbstarctor_VolumeType  (IN) - volume to search

//                                                                                                  ETrackerAbstractor_OrderType (IN)

// Explanation

// This method handles the query for searching all songs with all metadata from given volume

// It connects signal/slot for the query completion

//

// History

// 29-oct-2011      Chetan Nanda              created for seaching all songs with all metadata from given volume
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsWithMetaDataFromVolume(ETrackerAbstarctor_VolumeType  volumeType,
                                                           ETrackerAbstarctor_OrderType orderType)
{
    bool ret=false;
    m_VolumeType = volumeType;
    m_OrderType = orderType;

    switch (orderType)
    {
    case eAlbum:
    {
        ret = true;
        m_OrderBy_GetAllSongsFromVolume = "?album";
    }
    break;
    case eArtist:
    {
        ret = true;
        m_OrderBy_GetAllSongsFromVolume = "?artist";
    }
    break;
    case eGenre:
    {
        ret = true;
        m_OrderBy_GetAllSongsFromVolume = "?genre";
    }
    break;
    case eTitle:
    {
        ret = true;
        m_OrderBy_GetAllSongsFromVolume = "?title";
    }
    break;
    case eUrl:
    {
        ret = true;
        m_OrderBy_GetAllSongsFromVolume = "?url";
    }
    break;
    default:
        break;
    }

    if(!ret)
        return ret;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsWithMetaDataFromVolume);

    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllSongsWithMetaDataFromVolumeQuery
                             .arg(m_VolumeName_GetAllSongsWithMetaDataFromVolume)
                             .arg(m_OrderBy_GetAllSongsFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsWithMetaDataFromVolume);

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

// Function Name                                                                Parameters

// CMediaDataProvider::GetAllSongsByGenreArtistAlbum        bool (OUT), QStringList (IN) - Genre, Artist, Album to search
                                                                                          ETrackerAbstarctor_VolumeType  (IN) - volume to search

//

// Explanation

// This method handles the query for searching all songs by genre artist album

// It connects signal/slot for the query completion

//

// History

// 04-Nov-2011      Chetan Nanda              created for seaching all songs by genre artist album
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsByGenreArtistAlbum(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeType = volumeType;

    if(searchList.contains(QString()))
        return ret;

    m_Genre_GetAllSongsByGenreArtistAlbum = searchList[0];
    m_Artist_GetAllSongsByGenreArtistAlbum = searchList[1];
    m_Album_GetAllSongsByGenreArtistAlbum = searchList[2];

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsByGenreArtistAlbum);

    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllSongsByGenreArtistAlbum.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }

    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllSongsByGenreArtistAlbumQuery.arg(searchList[0]).arg(searchList[1])
                             .arg(searchList[2]).arg(m_VolumeName_GetAllSongsByGenreArtistAlbum));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsByGenreArtistAlbum);

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

// Function Name                                                                    Parameters

// CMediaDataProvider::GetAllSongsByGenreComposerAlbum    bool (OUT), QStringList (IN) - Genre, composer, Album to search
                                                                                             ETrackerAbstarctor_VolumeType  (IN) - volume to search

//

// Explanation

// This method handles the query for searching all songs by genre artist album

// It connects signal/slot for the query completion

//

// History

// 04-Nov-2011      Chetan Nanda              created for seaching all songs by genre artist album
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsByGenreComposerAlbum(QStringList searchList,
                                                         ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeType = volumeType;

    // return if genre, compoer or album string is empty
    if(searchList.contains(QString()))
        return ret;

    m_Genre_GetAllSongsByGenreComposerAlbum = searchList[0];
    m_Composer_GetAllSongsByGenreComposerAlbum = searchList[1];
    m_Album_GetAllSongsByGenreComposerAlbum = searchList[2];

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsByGenreComposerAlbum);

    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllSongsByGenreComposerAlbumQuery.arg(searchList[0]).arg(searchList[1])
                             .arg(searchList[2]).arg(m_VolumeName_GetAllSongsByGenreComposerAlbum));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsByGenreComposerAlbum);

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

// Function Name                                                                    Parameters

// CMediaDataProvider::GetAllSongsByComposerAlbum    bool (OUT), QStringList (IN) -  composer, Album to search
                                                                                             ETrackerAbstarctor_VolumeType  (IN) - volume to search
//

// Explanation

// This method handles the query for searching all songs by Composer album

// It connects signal/slot for the query completion

//

// History

// 07-Dec-2011      venkatesh              created for seaching all songs by Composer album
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsByComposerAlbum(QStringList searchList,
                                                    ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    if(searchList.contains(QString()))
        return ret;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsByComposerAlbum);
    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllSongsByComposerAlbum.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }

    if(!ret)
        return ret;

    m_Album_GetAllSongsByComposerAlbum.clear();
    m_Album_GetAllSongsByComposerAlbum.append(searchList[1]);
    m_Composer_GetAllSongsByComposerAlbum.clear();
    m_Composer_GetAllSongsByComposerAlbum.append(searchList[0]);

    // setting query string
    QString queryString = gGetAllSongsByComposerAlbumQuery.arg(searchList[0], searchList[1], m_VolumeName_GetAllSongsByComposerAlbum);

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

    if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnFinished()));
    }

    return ret;
}

/******************************************************************************

// Function Name                                                                    Parameters

// CMediaDataProvider::GetMusicAlbumsByGenreComposer    bool (OUT), QStringList (IN) -  composer, Album to search
                                                                                             ETrackerAbstarctor_VolumeType  (IN) - volume to search
//

// Explanation

// This method handles the query for searching all songs by Composer album

// It connects signal/slot for the query completion

//

// History

// 07-Dec-2011      venkatesh              created for seaching all songs by Composer album
*****************************************************************************/
bool CMediaDataProvider::GetMusicAlbumsByGenreComposer(QStringList searchList,
                                                       ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    if(searchList.contains(QString()))
        return ret;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetMusicAlbumsByGenreComposer);
    if(volumeType==eIPod)
    {
        m_VolumeName_GetMusicAlbumsByGenreComposer.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }

    if(!ret)
        return ret;

    m_Genre_GetMusicAlbumsByGenreComposer.clear();
    m_Genre_GetMusicAlbumsByGenreComposer.append(searchList[0]);
    m_Composer_GetMusicAlbumsByGenreComposer.clear();
    m_Composer_GetMusicAlbumsByGenreComposer.append(searchList[1]);

    // setting query string
    QString queryString = gGetAllAlbumByGenreComposerQuery.arg(searchList[0], searchList[1], m_VolumeName_GetMusicAlbumsByGenreComposer);

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

    if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnFinished()));
    }

    return ret;
}
/******************************************************************************

// Function Name                                                                    Parameters

// CMediaDataProvider::GetMusicComposersByGenre    bool (OUT), QStringList (IN) -  composer to search
                                                                                             ETrackerAbstarctor_VolumeType  (IN) - volume to search
//

// Explanation

// This method handles the query for searching all songs by genre album

// It connects signal/slot for the query completion

//

// History

// 07-Dec-2011      venkatesh              created for seaching all songs by Composer album
*****************************************************************************/
bool CMediaDataProvider::GetMusicComposersByGenre(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    if(searchList.contains(QString()))
        return ret;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllComposersByGenre);
    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllComposersByGenre.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }

    if(!ret)
        return ret;

    m_Genre_GetAllComposersByGenre.clear();
    m_Genre_GetAllComposersByGenre.append(searchList[0]);

    // setting query string
    QString queryString = gGetAllComposerByGenreQuery.arg(searchList[0], m_VolumeName_GetAllComposersByGenre);

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

    if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnFinished()));
    }
    return ret;
}

/******************************************************************************

// Function Name                                                                    Parameters

// CMediaDataProvider::GetMusicAblumsByComposer    bool (OUT), QStringList (IN) -  composer to search
                                                                                             ETrackerAbstarctor_VolumeType  (IN) - volume to search
//

// Explanation

// This method handles the query for searching all songs by genre album

// It connects signal/slot for the query completion

//

// History

// 07-Dec-2011      venkatesh              created for seaching all songs by Composer album
*****************************************************************************/
bool CMediaDataProvider::GetMusicAblumsByComposer(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{

    bool ret = false;

    if(searchList.contains(QString()))
        return ret;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllAlbumByComposer);
    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllAlbumByComposer.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }

    if(!ret)
        return ret;

    m_Composer_GetAllAlbumByComposer.clear();
    m_Composer_GetAllAlbumByComposer.append(searchList[0]);

    // setting query string
    QString queryString = gGetAllAlbumByComposerQuery.arg(searchList[0], m_VolumeName_GetAllAlbumByComposer);

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

    if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnFinished()));
    }
    return ret;
}


bool CMediaDataProvider::GetAllSongsByArtistAlbum(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    if(searchList.contains(QString()))
        return ret;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsByArtistAlbum);
    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllSongsByArtistAlbum.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }

    if(!ret)
        return ret;

    //added by jaehwan.lee 2013.12.18 to fix aritst tab double quote issue.
    searchList[0].replace(gDoubleQuote, "\\\"");
    searchList[1].replace(gDoubleQuote, "\\\"");

    m_Album_GetAllSongsByArtistAlbum.clear();
    m_Album_GetAllSongsByArtistAlbum.append(searchList[1]);
    m_Artist_GetAllSongsByArtistAlbum.clear();
    m_Artist_GetAllSongsByArtistAlbum.append(searchList[0]);

    // setting query string
    QString queryString = gGetAllSongsByArtistAlbumQuery.arg(searchList[0], searchList[1], m_VolumeName_GetAllSongsByArtistAlbum);

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

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

bool CMediaDataProvider::GetMusicAlbumsByGenreArtist(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    if(searchList.contains(QString()))
        return ret;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllAlbumByGenreArtist);

    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllAlbumByGenreArtist.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }


    if(!ret)
        return ret;

    m_Genre_GetAllAlbumByGenreArtist.clear();
    m_Genre_GetAllAlbumByGenreArtist.append(searchList[0]);

    m_Artist_GetAllAlbumByGenreArtist.clear();
    m_Artist_GetAllAlbumByGenreArtist.append(searchList[1]);

    // setting query string
    QString queryString = gGetAllAlbumByGenreArtistQuery.arg(searchList[0], searchList[1], m_VolumeName_GetAllAlbumByGenreArtist);

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

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

bool CMediaDataProvider::GetMusicAlbumsByArtist(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    if(searchList.contains(QString()))
        return ret;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllAlbumByArtist);
    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllAlbumByArtist.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }


    if(!ret)
        return ret;

    //added by jaehwan.lee 2013.12.18 to fix aritst tab double quote issue.
    searchList[0].replace(gDoubleQuote, "\\\"");

    m_Artist_GetAllAlbumByArtist.clear();
    m_Artist_GetAllAlbumByArtist.append(searchList[0]);

    // setting query string
    QString queryString = gGetAllAlbumByArtistQuery.arg(searchList[0], m_VolumeName_GetAllAlbumByArtist);

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

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

bool CMediaDataProvider::GetMusicArtistsByGenre(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    if(searchList.contains(QString()))
        return ret;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllArtistByGenre);
    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllArtistByGenre.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }

    if(!ret)
        return ret;

    m_Genre_GetAllArtistByGenre.clear();
    m_Genre_GetAllArtistByGenre.append(searchList[0]);

    // setting query string
    QString queryString = gGetAllArtistByGenreQuery.arg(searchList[0], m_VolumeName_GetAllArtistByGenre);

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

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


bool CMediaDataProvider::GetAllSongsByKeywordFromVolume(QString searchdata, ETrackerAbstarctor_VolumeType volumeType){
    bool ret = false;

    QString queryString;

    //Convert string to EUC-KR also to search
    QTextCodec *codec = QTextCodec::codecForName("EUC-KR");
    QByteArray euckrStr = codec->fromUnicode(searchdata);

    m_SongKeyword_GetAllsongsByKeywordFromVolume = searchdata;
    m_SongKeyword_EucKR_GetAllsongsByKeywordFromVolume = euckrStr;

    LOG_TRACE << "EUC-KR GetAllSongsByKeywordFromVolume converted string" << LOG_ENDL;
    //CLoggenerator::Loggenerator(euckrStr) ;
    LOG_TRACE << m_SongKeyword_EucKR_GetAllsongsByKeywordFromVolume << LOG_ENDL;

    switch (volumeType)
    {
    case eUSB:
        // setting query string
        queryString = gGetAllsongsByKeywordFromVolumeQuery.arg(searchdata,euckrStr, GetUSBMountPath()); // modified by eugene.seo 2013.05.13
        m_VolumeName_forKeyword.clear();
        m_VolumeName_forKeyword.append(GetUSBMountPath()); // modified by eugene.seo 2013.05.13
        break;
    case eJukeBox:
        // setting query string
        queryString = gGetAllsongsByKeywordFromVolumeQuery.arg(searchdata,euckrStr,gVolumePath_JukeBox);
        m_VolumeName_forKeyword.clear();
        m_VolumeName_forKeyword.append(gVolumePath_JukeBox);
        break;
    case eIPod:
        queryString = gGetAllsongsByKeywordFromVolumeQuery.arg(searchdata,euckrStr, gVolumePath_IPod);
        m_VolumeName_forKeyword.clear();
        m_VolumeName_forKeyword.append(gVolumePath_IPod);
        break;
    case eDisc:
        queryString = gGetAllsongsByKeywordFromVolumeQuery.arg(searchdata,euckrStr, gVolumePath_DISC);
        m_VolumeName_forKeyword.clear();
        m_VolumeName_forKeyword.append(gVolumePath_DISC);
        break;
    default:
        break;

    }
    if(m_VolumeName_forKeyword.isEmpty())
    {
        HandleError(eEmptyString);
        return ret;
    }

    LOG_TRACE << "query is ->" << queryString << LOG_ENDL;

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

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

bool CMediaDataProvider::GetAllAlbumsByKeywordFromVolume(QString searchdata, ETrackerAbstarctor_VolumeType volumeType){

    bool ret = false;

    QString queryString;

    //Convert string to EUC-KR also to search
    QTextCodec *codec = QTextCodec::codecForName("EUC-KR");
    QByteArray euckrStr = codec->fromUnicode(searchdata);

    m_AlbumKeyword_GetAllAlbumsByKeywordFromVolume = searchdata;
    m_AlbumKeyword_EucKR_GetAllAlbumsByKeywordFromVolume = euckrStr;

    LOG_TRACE << "EUC-KR GetAllAlbumsByKeywordFromVolume converted string" << LOG_ENDL;
    //CLoggenerator::Loggenerator(euckrStr) ;
    LOG_TRACE << m_AlbumKeyword_EucKR_GetAllAlbumsByKeywordFromVolume << LOG_ENDL;

    switch (volumeType)
    {
    case eUSB:
        // setting query string
        queryString = gGetAllAlbumsByKeywordFromVolumeQuery.arg(searchdata, euckrStr, GetUSBMountPath()); // modified by eugene.seo 2013.05.13
        m_VolumeName_forKeyword.clear();
        m_VolumeName_forKeyword.append(GetUSBMountPath()); // modified by eugene.seo 2013.05.13
        break;
    case eJukeBox:
        // setting query string
        queryString = gGetAllAlbumsByKeywordFromVolumeQuery.arg(searchdata, euckrStr, gVolumePath_JukeBox);
        m_VolumeName_forKeyword.clear();
        m_VolumeName_forKeyword.append(gVolumePath_JukeBox);
        break;
    case eIPod:
        queryString = gGetAllAlbumsByKeywordFromVolumeQuery.arg(searchdata, euckrStr, gVolumePath_IPod);
        m_VolumeName_forKeyword.clear();
        m_VolumeName_forKeyword.append(gVolumePath_IPod);
        break;
    case eDisc:
        queryString = gGetAllAlbumsByKeywordFromVolumeQuery.arg(searchdata, euckrStr, gVolumePath_DISC);
        m_VolumeName_forKeyword.clear();
        m_VolumeName_forKeyword.append(gVolumePath_DISC);
        break;
    default:
        break;

    }
    if(m_VolumeName_forKeyword.isEmpty())
    {
        HandleError(eEmptyString);
        return ret;
    }

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

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

bool CMediaDataProvider::GetAllArtistsByKeywordFromVolume(QString searchdata, ETrackerAbstarctor_VolumeType volumeType){
    bool ret = false;

    QString queryString;


    //Convert string to EUC-KR also to search
    QTextCodec *codec = QTextCodec::codecForName("EUC-KR");
    QByteArray euckrStr = codec->fromUnicode(searchdata);

    m_ArtistKeyword_GetAllArtistsByKeywordFromVolume = searchdata;
    m_ArtistKeyword_EucKR_GetAllArtistsByKeywordFromVolume = euckrStr;

    LOG_TRACE << "EUC-KR GetAllArtistsByKeywordFromVolume converted string" << LOG_ENDL;
    //CLoggenerator::Loggenerator(euckrStr) ;
    LOG_TRACE << m_ArtistKeyword_EucKR_GetAllArtistsByKeywordFromVolume << LOG_ENDL;

    switch (volumeType)
    {
    case eUSB:
        // setting query string
        queryString = gGetAllArtistsByKeywordFromVolumeQuery.arg(searchdata,euckrStr, GetUSBMountPath()); // modified by eugene.seo 2013.05.13
        m_VolumeName_forKeyword.clear();
        m_VolumeName_forKeyword.append(GetUSBMountPath()); // modified by eugene.seo 2013.05.13
        break;
    case eJukeBox:
        // setting query string
        queryString = gGetAllArtistsByKeywordFromVolumeQuery.arg(searchdata,euckrStr, gVolumePath_JukeBox);
        m_VolumeName_forKeyword.clear();
        m_VolumeName_forKeyword.append(gVolumePath_JukeBox);
        break;
    case eIPod:
        queryString = gGetAllArtistsByKeywordFromVolumeQuery.arg(searchdata,euckrStr, gVolumePath_IPod);
        m_VolumeName_forKeyword.clear();
        m_VolumeName_forKeyword.append(gVolumePath_IPod);
        break;
    case eDisc:
        queryString = gGetAllArtistsByKeywordFromVolumeQuery.arg(searchdata,euckrStr, gVolumePath_DISC);
        m_VolumeName_forKeyword.clear();
        m_VolumeName_forKeyword.append(gVolumePath_DISC);
        break;
    default:
        break;

    }
    if(m_VolumeName_forKeyword.isEmpty())
    {
        HandleError(eEmptyString);
        return ret;
    }

    LOG_TRACE << "query is ->" <<queryString << LOG_ENDL;

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

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

bool CMediaDataProvider::GetAllGenresByKeywordFromVolume(QString searchdata, ETrackerAbstarctor_VolumeType volumeType){
    bool ret = false;

    QString queryString;

    m_GenreKeyword_GetAllGenresByKeywordFromVolume = searchdata;
    //Convert string to EUC-KR also to search
    QTextCodec *codec = QTextCodec::codecForName("EUC-KR");
    QByteArray euckrStr = codec->fromUnicode(searchdata);

    m_GenreKeyword_GetAllGenresByKeywordFromVolume = searchdata;
    m_GenreKeyword_EucKR_GetAllGenresByKeywordFromVolume = euckrStr;

    LOG_TRACE << "EUC-KR GetAllGenresByKeywordFromVolume converted string" << LOG_ENDL;
    //CLoggenerator::Loggenerator(euckrStr) ;
    LOG_TRACE << m_GenreKeyword_EucKR_GetAllGenresByKeywordFromVolume <<  LOG_ENDL;

    switch (volumeType)
    {
    case eUSB:
        // setting query string
        queryString = gGetAllGenresByKeywordFromVolumeQuery.arg(searchdata,euckrStr, GetUSBMountPath()); // modified by eugene.seo 2013.05.13
        m_VolumeName_forKeyword.clear();
        m_VolumeName_forKeyword.append(GetUSBMountPath()); // modified by eugene.seo 2013.05.13
        break;
    case eJukeBox:
        // setting query string
        queryString = gGetAllGenresByKeywordFromVolumeQuery.arg(searchdata,euckrStr, gVolumePath_JukeBox);
        m_VolumeName_forKeyword.clear();
        m_VolumeName_forKeyword.append(gVolumePath_JukeBox);
        break;
    case eIPod:
        queryString = gGetAllGenresByKeywordFromVolumeQuery.arg(searchdata,euckrStr, gVolumePath_IPod);
        m_VolumeName_forKeyword.clear();
        m_VolumeName_forKeyword.append(gVolumePath_IPod);
        break;
    case eDisc:
        queryString = gGetAllGenresByKeywordFromVolumeQuery.arg(searchdata,euckrStr, gVolumePath_DISC);
        m_VolumeName_forKeyword.clear();
        m_VolumeName_forKeyword.append(gVolumePath_DISC);
        break;

    default:
        break;

    }
    if(m_VolumeName_forKeyword.isEmpty())
    {
        HandleError(eEmptyString);
        return ret;
    }

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);

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


bool CMediaDataProvider::GetAllSongsByAlbumFromVolume(QString searchdata, ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret = false;

    searchdata.replace(gDoubleQuote, QByteArray("\\\""));   // added by sangmin.seol 2013.10.16 ITS_0189462 replace " to \" for sparql query

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsByAlbumFromVolume);

    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllSongsByAlbumFromVolume.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }

    if(!ret)
        return ret;

    QString queryString;

    /* switch (volumeType)
    {

    case eUSB:
        // setting query string
        break;
    case eJukeBox:
        break;
    case eIPod:
              queryString = gGetAllSongsByAlbumFromVolumeQuery.arg(searchdata, gIPODAllSongType);
              m_AlbumKeyword_GetAllSongsByAlbumFromVolume.clear();
              m_AlbumKeyword_GetAllSongsByAlbumFromVolume.append(searchdata);

              break;
    default:
           break;

    }*/

    m_AlbumKeyword_GetAllSongsByAlbumFromVolume.clear();
    m_AlbumKeyword_GetAllSongsByAlbumFromVolume.append(searchdata);

    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("title"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllSongsByAlbumFromVolumeExtOrderQuery.arg(searchdata, m_VolumeName_GetAllSongsByAlbumFromVolume, m_ExtendOrdering);
    }
    else 
    {
        queryString = gGetAllSongsByAlbumFromVolumeQuery.arg(searchdata, m_VolumeName_GetAllSongsByAlbumFromVolume);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	
    //}

 if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnFinished()));
    }
    return ret;
}

bool CMediaDataProvider::PrepareVolumeList()
{
    bool ret = false;
    m_VolumeNameList.clear();
    QSparqlQuery volumeQueryObject;
    volumeQueryObject.setQuery(gGetMountedVolumeListQuery);

    QSparqlResult* result = m_pSparql_Connection->syncExec(volumeQueryObject);

    //    QStringList volumeList;
    if (!result->hasError())
    {
        ret = true;
        while(result->next())
        {
            m_VolumeNameList.append(result->value(0).toString());
        }
    }
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }

    return ret;
}

bool CMediaDataProvider::GetExtendOrderingQueryToAppend(int lang, QString colName, QString& orderQuery)
{
    orderQuery.clear(); 
    if (lang == LANGUAGE_KO) 
    {
        // modified by jaehwan.lee to fix korean ordering 2013.10.15
        orderQuery.append(gOrderByKor.arg(colName).arg(gKorBegin1).arg(gKorEnd1)); //modified by honggi.shin 2013.12.05
    }
    else if (lang == LANGUAGE_AR) 
    {
        orderQuery.append(gOrderByArb.arg(colName).arg(gArbBegin1).arg(gArbEnd1).arg(gArbSpecial1).arg(gArbBegin2).arg(gArbEnd2)
            .arg(gArbSpecial2).arg(gArbSpecial3)); //modified by honggi.shin 2013.12.05
    }
    else if (lang == LANGUAGE_ZH_CMN)
    {
        orderQuery.append(gOrderByZh.arg(colName).arg(gZhBegin1).arg(gZhEnd1)); //modified by honggi.shin 2013.12.05
    }
    else
        return false; 
    
    return true; 
}

//{ added by jaehwan 2013.11.27 to fix ITS 210480
bool CMediaDataProvider::GetExtendOrderingQueryToAppend2(int lang, QString colName1, QString colName2, QString& orderQuery)
{
    orderQuery.clear();
    if (lang == LANGUAGE_KO)
    {
        // modified by jaehwan.lee to fix korean ordering 2013.10.15
        orderQuery.append(gOrderByKor.arg(colName1).arg(gKorBegin1).arg(gKorEnd1)); //modified by jaehwan 2013.12.05
        orderQuery.append(QString(" ?%1").arg(colName1));
        orderQuery.append(QString(" !BOUND(?trackno) ?trackno ")); // modified by jaehwan.lee 2014.01.13 for track number sorting in the album depth_1
        orderQuery.append(gOrderByKor.arg(colName2).arg(gKorBegin1).arg(gKorEnd1)); //modified by jaehwan 2013.12.05
        orderQuery.append(QString(" ?%1").arg(colName2));
    }
    else if (lang == LANGUAGE_AR)
    {
        orderQuery.append(gOrderByArb.arg(colName1).arg(gArbBegin1).arg(gArbEnd1).arg(gArbSpecial1).arg(gArbBegin2).arg(gArbEnd2)
            .arg(gArbSpecial2).arg(gArbSpecial3)); //modified by jaehwan 2013.12.05
        orderQuery.append(QString(" ?%1").arg(colName1));
        orderQuery.append(QString(" !BOUND(?trackno) ?trackno ")); // modified by jaehwan.lee 2014.01.13 for track number sorting in the album depth_1
        orderQuery.append(gOrderByArb.arg(colName2).arg(gArbBegin1).arg(gArbEnd1).arg(gArbSpecial1).arg(gArbBegin2).arg(gArbEnd2)
            .arg(gArbSpecial2).arg(gArbSpecial3)); //modified by jaehwan 2013.12.05
        orderQuery.append(QString(" ?%1").arg(colName2));

    }
    else if (lang == LANGUAGE_ZH_CMN)
    {
        // modified by jaehwan.lee to fix korean ordering 2013.10.15
        orderQuery.append(gOrderByZh.arg(colName1).arg(gZhBegin1).arg(gZhEnd1)); //modified by jaehwan 2013.12.05
        orderQuery.append(QString(" ?%1").arg(colName1));
        orderQuery.append(QString(" !BOUND(?trackno) ?trackno ")); // modified by jaehwan.lee 2014.01.13 for track number sorting in the album depth_1
        orderQuery.append(gOrderByZh.arg(colName2).arg(gZhBegin1).arg(gZhEnd1)); //modified by jaehwan 2013.12.05
        orderQuery.append(QString(" ?%1").arg(colName2));
    }
    else
        return false;

    return true;
}
//}

bool CMediaDataProvider::GetVolumeQueryToAppend(ETrackerAbstarctor_VolumeType volumeType, QString& volumeQuery)
{
    bool ret = false;
    m_VolumeType = volumeType;

    QString filter = " FILTER(";
    QString url = "?url , ";

    switch (volumeType)
    {
    case eUSB:
    case eUSB_FRONT:
    case eUSB_REAR:
    {
        ret= true;
        //PrepareVolumeList();

#ifdef MULTIPLE_DEVICE_SUPPORT

        m_VolumeNameList.clear();
        QString DeviceUrl ;
       // DeviceUrl = getDeviceURL(volumeType);
          DeviceUrl = m_DeviceURL.at(volumeType);
        //m_VolumeNameList.append(DeviceUrl);

        DeviceUrl = QUrl::toPercentEncoding(DeviceUrl, ":/()~!@$&_-',.=+");
        if(DeviceUrl.contains("+"))
            DeviceUrl.replace(gPlusSign, gPlusSignCorrected);
        if(DeviceUrl.contains("("))
            DeviceUrl.replace(gLBracketSign, gLBracketSignCorrected);
        if(DeviceUrl.contains(")"))
            DeviceUrl.replace(gRBracketSign, gRBracketSignCorrected);
        if(DeviceUrl.contains("$"))
            DeviceUrl.replace(gDollarSign, gDollarSignCorrected);
        m_VolumeNameList.append(DeviceUrl);


#else

        m_VolumeNameList.clear();

        // Get the current mounted volume for USB
        //QDir directory("/media");
        QDir directory(GetUSBMountPath().remove("file://")); // modified by eugene.seo 2013.05.13

        QFileInfoList fileInfoList = directory.entryInfoList();

        for(int i = 0 ; i < fileInfoList.count(); i++)
        {

            QFileInfo fileInfo = fileInfoList.at(i) ;
            QString fileName = fileInfo.fileName();

            if(fileName.contains("sdcard") == 0 && fileName != "." && fileName != "..")
            {
                //QString volumeName = "file:///media/";
                QString volumeName = GetUSBMountPath(); // modified by eugene.seo 2013.05.13
                volumeName.append("/"); // modified by eugene.seo 2013.05.13
                volumeName.append(fileName);

                volumeName = QUrl::toPercentEncoding(volumeName, ":/()~!@$&_-',.=+");

                if(volumeName.contains("+"))
                    volumeName.replace(gPlusSign, gPlusSignCorrected);

                if(volumeName.contains("("))
                    volumeName.replace(gLBracketSign, gLBracketSignCorrected);

                if(volumeName.contains(")"))
                    volumeName.replace(gRBracketSign, gRBracketSignCorrected);

                if(volumeName.contains("$"))
                    volumeName.replace(gDollarSign, gDollarSignCorrected);

                m_VolumeNameList.append(volumeName);
            }
        }
#endif
        QStringList volumeList;
        for(int x=0; x<m_VolumeNameList.count(); x++)
        {
            if(m_VolumeNameList[x].contains(GetUSBMountPath())) // modified by eugene.seo 2013.05.13
                volumeList.append(m_VolumeNameList[x]);
        }
        m_VolumeNameList.clear();
        m_VolumeNameList = volumeList;
        if(m_VolumeNameList.count() > 0)
        {
            for (int j = 0; j < m_VolumeNameList.count(); j++)
            {
                QString Regex = "REGEX (";

                Regex.append(url);
                Regex.append(gDoubleQuote);
                Regex.append("^");
                Regex.append(m_VolumeNameList[j]);
                Regex.append("/"); // modified by jaehwan.lee 2014.02.04 to fix ITS 220429
                Regex.append(gDoubleQuote);

                if(j == (m_VolumeNameList.size()-1) )
                    Regex.append(")");
                else
                    Regex.append(") || ");

                filter.append(Regex);
            }

            filter.append(")");
        }
        else
        {
            QString Regex = "REGEX (";

            Regex.append(url);
            Regex.append(gDoubleQuote);
            Regex.append("^");
            Regex.append(GetUSBMountPath()); // modified by eugene.seo 2013.05.13
            Regex.append("/"); // modified by jaehwan.lee 2014.02.04 to fix ITS 220429
            Regex.append(gDoubleQuote);
            Regex.append(")");

            filter.append(Regex);

            filter.append(")");
            //            return false;
        }
    }
    break;
    case eJukeBox:
    {
        ret= true;
        QString Regex = "REGEX (";

        Regex.append(url);
        Regex.append(gDoubleQuote);
        Regex.append("^");
#ifdef MULTIPLE_DEVICE_SUPPORT
        Regex.append(m_DeviceURL.at(volumeType));
#else
        Regex.append(gVolumePath_JukeBox);
#endif
        Regex.append(gDoubleQuote);
        Regex.append(")");

        filter.append(Regex);

        filter.append(")");
    }
    break;

    case eIPod:
    case eIPOD_FRONT:
    case eIPOD_REAR:
    {

        ret = true;
// Commenting the multiple iPod support from tracker side temporarily till the
// full support is available from the iPodController/HMI.

#ifdef MULTIPLE_DEVICE_SUPPORT
        m_VolumeNameList.clear();
        //QString DeviceUrl = getDeviceURL(volumeType);
        QString DeviceUrl ;
        DeviceUrl.append(m_DeviceURL.at(volumeType));
        m_VolumeNameList.append(DeviceUrl);

#else

        PrepareVolumeList();
        QStringList volumeList;
        for(int x=0; x<m_VolumeNameList.count(); x++)
        {
            if(m_VolumeNameList[x].contains("file:///ipod"))
                volumeList.append(m_VolumeNameList[x]);
        }
        m_VolumeNameList.clear();
        m_VolumeNameList = volumeList;
#endif
        if(m_VolumeNameList.count() > 0)
        {
            /*  QStringList volumeList;
                for(int x=0; x<m_VolumeNameList.count(); x++)
                {
                    if(m_VolumeNameList[x].contains("file:///ipod"))
                        volumeList.append(m_VolumeNameList[x]);
                }
                m_VolumeNameList.clear();
                m_VolumeNameList = volumeList;*/
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
        }
//#endif
    }

    break;

    default:
        break;
    }

    if(!ret)
        return ret;

    volumeQuery.clear();
    volumeQuery.append(filter);

    return ret;
}

/******************************************************************************

// Function Name                                            Parameters

// CFullTextSearch::OnFinished                        void (OUT)

//

// Explanation

// This is a slot called when finished signal emited by QSparqlResult on query completion

// It emits DataFetched signal for TrackerAbstractor

//

// History

// 12-Sept-2011      Chetan Nanda                created hanlding music seach query completion signal
*****************************************************************************/

void CMediaDataProvider::OnFinished()
{
    QVector<QStringList> queryDataList;
    //    QStringList dataList;
    int noOfColumns = 0;
    QString query = gBlankSpace;
	QString letter;

    ETrackerAbstarctor_Query_Ids queryId = eDefault_Id;
    QSparqlResult* result = qobject_cast<QSparqlResult*>(sender());
    if(result == NULL)  return; // added by sungha.choi 2013.07.24

    query = result->query();

    // comparing result query with orignal query to set query id and coloumn count
    if(!query.compare(gMusicAlbumListQuery.arg(m_FolderName_AlbumListByFolder)))
    {
        m_FolderName_AlbumListByFolder.clear();
//        queryId = MAP_QUERY_IDS.value(gMusicAlbumListQuery_Key);
        queryId = eMusicAlbumListQuery_Id;
        noOfColumns = MUSICALBUMLISTQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gSongsByAlbumQuery.arg(m_AlbumName)) &&
            m_QueryId == eSongsByNextAlbumQuery_Id )
    {
        m_AlbumName.clear();
        queryId = m_QueryId;
        noOfColumns = SONGSBYALBUMQUERY_DATATYPECOUNT;
        m_QueryId = eDefault_Id;
    }
    else if(!query.compare(gSongsByAlbumQuery.arg(m_AlbumName)) &&
            m_QueryId == eSongsByPrevAlbumQuery_Id )
    {
        m_AlbumName.clear();
        queryId = m_QueryId;
        noOfColumns = SONGSBYALBUMQUERY_DATATYPECOUNT;
        m_QueryId = eDefault_Id;
    }
    /*else if(!query.compare(gSongsByFolderQuery.arg(m_FolderName_SongByFolder)) &&
      m_QueryId == eGetSongsByNextFolderQuery_Id )
   {
       queryId = m_QueryId;
       noOfColumns = SONGBYFOLDERQUERY_DATATYPECOUNT;
       m_QueryId = eDefault_Id;
   }
   else if(!query.compare(gSongsByFolderQuery.arg(m_FolderName_SongByFolder)) &&
            m_QueryId == eGetSongsByPrevFolderQuery_Id )
   {
       queryId = m_QueryId;
       noOfColumns = SONGBYFOLDERQUERY_DATATYPECOUNT;
       m_QueryId = eDefault_Id;
   }*/
    /* !query.compare(gSongsByEmptyFolderQuery) ||*/
    else if(!query.compare(gSongsByFolderQuery.arg(m_FolderName_SongByFolder)) )
    {
        m_FolderName_SongByFolder.clear();
//        queryId = MAP_QUERY_IDS.value(gSongsByFoderQuery_Key);
        queryId = eSongsByFolderQuery_Id;
        noOfColumns = SONGBYFOLDERQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gArtistByEmptyFolderQuery) ||
            !query.compare(gArtistByFolderQuery.arg(m_FolderName_ArtistByFolder)))
    {
        m_FolderName_ArtistByFolder.clear();
//        queryId = MAP_QUERY_IDS.value(gArtistByFolderQuery_Key);
        queryId = eArtistByFolderQuery_Id;
        noOfColumns = ARTISTBYFOLDERQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gAlbumByEmptyFolderQuery) ||
            !query.compare(gAlbumByFolderQuery.arg(m_FolderName_AlbumByFolder)))
    {
        m_FolderName_AlbumByFolder.clear();
//        queryId = MAP_QUERY_IDS.value(gAlbumByFolderQuery_Key);
        queryId = eAlbumByFolderQuery_Id;
        noOfColumns = ALBUMBYFOLDERQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllSongsQuery))
    {
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsQuery_Key);
        queryId  = eGetAllSongsQuery_Id;
        noOfColumns = GETALLSONGSQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllSongsIDQuery))
    {
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsIDQuery_Key);
        queryId = eGetAllSongsIDQuery_id;
        noOfColumns = GETALLSONGSIDQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gFolderListEmptyQuery) ||
            !query.compare(gFolderListQuery.arg(m_FolderName_FolderList)))
    {
        m_FolderName_FolderList.clear();
//        queryId = MAP_QUERY_IDS.value(gFolderListQuery_Key);
        queryId = eFolderListQuery_Id;
        noOfColumns = FOLDERLISTQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllAlbumsQuery))
    {
//        queryId = MAP_QUERY_IDS.value(gGetAllAlbumsQuery_Key);
        queryId  = eGetAllAlbumsQuery_Id;
        noOfColumns = GETALLALBUMSQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllMusicArtistsQuery))
    {
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicArtistsQuery_Key);
        queryId = eGetAllMusicArtistsQuery_Id;
        noOfColumns = GETALLMUISCARTISTSQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllMusicComposersQuery))
    {
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicComposersQuery_Key);
        queryId  = eGetAllMusicComposersQuery_Id;
        noOfColumns = GETALLMUSICCOMPOSERSQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllMusicGenresQuery))
    {
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicGenresQuery_Key);
        queryId =  eGetAllMusicGenresQuery_Id;
        noOfColumns = GETALLMUSICGENRESQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetMusicMetaDataQuery.arg(m_FilePath_MusicMetaData)))
    {
        m_FilePath_MusicMetaData.clear();
//        queryId = MAP_QUERY_IDS.value(gGetMusicMetaDataQuery_Key);
        queryId =  eGetMusicMetaDataQuery_Id;
        noOfColumns = GETMUSICMETADATAQUERY_DATATYPECOUNT;
    }//{added by junam 2013.03.27 for ISV72425
    else if(!query.compare(gGetMusicSimpleMetaDataQuery.arg(m_FilePath_MusicMetaData)))
    {
        m_FilePath_MusicMetaData.clear();
        queryId =  eGetMusicSimpleMetaDataQuery_Id;
        noOfColumns = GETMUSICSIMPLEMETADATAQUERY_DATATYPECOUNT;
    }//}added by junam 2013.03.27 
    else if(!query.compare(gGetMusicNormalMetaDataQuery.arg(m_FilePath_MusicMetaData)))
    {
        m_FilePath_MusicMetaData.clear();
        queryId =  eGetMusicNormalMetaDataQuery_Id;
        noOfColumns = GETMUSICNORMALMETADATAQUERY_DATATYPECOUNT;
    }   //  added by sungha.choi 2013.09.06
    else if(!query.compare(gSongsByAlbumQuery.arg(m_AlbumName_AllSongsByAlbum)))
    {
        m_AlbumName_AllSongsByAlbum.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsByAlbumQuery_Key);
        queryId = eGetAllSongsByAlbumQuery_Id;
        noOfColumns = SONGSBYALBUMQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllSongsByArtistQuery.arg(m_ArtistName_AllSongsByArtist)))
    {
        m_ArtistName_AllSongsByArtist.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsByArtistQuery_Key);
        queryId = eGetAllSongsByArtistQuery_Id;
        noOfColumns = GETALLSONGSBYARTISTQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllSongsByGenreQuery.arg(m_GenreName_AllSongsByGenre)))
    {
        m_GenreName_AllSongsByGenre.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsByGenreQuery_Key);
        queryId = eGetAllSongsByGenreQuery_Id;
        noOfColumns = GETALLSONGSBYGENREQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllSongsByComposerQuery.arg(m_ComposerName_AllSongsByComposer)))
    {
        m_ComposerName_AllSongsByComposer.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsByComposerQuery_Key);
        queryId = eGetAllSongsByComposerQuery_Id;
        noOfColumns = GETALLSONGSBYCOMPOSERQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetImagesByFolderQuery.arg(m_FolderName_ImagesByFolder)))
    {
//        queryId = MAP_QUERY_IDS.value(gGetImagesByFolderQuery_Key);
        queryId = eGetImagesByFolderQuery_Id;
        noOfColumns = GETIMAGESBYFOLDERQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetVideosByFolderQuery.arg(m_FolderName_VideosByFolder)))
    {
//        queryId = MAP_QUERY_IDS.value(gGetVideosByFolderQuery_Key);
        queryId = eGetVideosByFolderQuery_Id;
        noOfColumns = GETVIDEOSBYFOLDERQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllMusicByKeywordQuery.arg(m_Keyword_AllMusicByKeyword,
                                                          m_Keyword1_AllMusicByKeyword, m_PathList_AllMusicByKeyword)))
    {
        m_Keyword_AllMusicByKeywordForIPod = m_Keyword_AllMusicByKeywordForIPod.replace("%","\\%");
        m_Keyword_AllMusicByKeywordForIPod = m_Keyword_AllMusicByKeywordForIPod.replace("'","''");
        QString queryStr;
        queryStr.append( QString("select T_Title, T_Album, T_Artist, T_Genre, T_FilePath From MasterTable where T_Title like '%%1%'  OR T_Album like '%%1%' OR T_Artist like '%%1%'").arg(m_Keyword_AllMusicByKeywordForIPod));

        m_Keyword_AllMusicByKeyword.clear();
        m_Keyword1_AllMusicByKeyword.clear();
        LOG_TRACE << "got event for music by keyword" << LOG_ENDL;
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicByKeywordQuery_Key);
        queryId = eGetAllMusicByKeywordQuery_Id;
        noOfColumns = GETALLMUSICBYKEYWORDQUERY_DATATYPECOUNT;

        //Added by Divya for the AVP crash reported for CR13965
        if(m_Ipod_DBConnList.count() > 0) 
        {
            if(m_Ipod_DBConnList[0].isOpen())
                 senddata(noOfColumns, queryStr, m_Ipod_DBConnList[0], queryDataList);
            if(m_Ipod_DBConnList.count() > 1) 
            {
                if(m_Ipod_DBConnList[1].isOpen())
                    senddata(noOfColumns, queryStr, m_Ipod_DBConnList[1], queryDataList);
            }
        }
        // Get Response from CD
        if(m_isCDInserted)
        {
            m_isCDInserted = false;

            QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE","CDDA");
            database.setDatabaseName("/app/data/gracenote/CDDAData"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change
             if(database.open())
             {
                 LOG_TRACE << "Database for CD is open " << LOG_ENDL;
                 QString queryStr;
                 queryStr.append( QString("select Title, Album, Artist, Genre, Path From CDTable where Title like '%%1%'  OR Album like '%%1%' OR Artist like '%%1%' order by Title").arg(m_Keyword_AllMusicByKeywordForIPod));

                 senddata (noOfColumns, queryStr, database, queryDataList);

                 database.close();
             }
             else
             {
                 LOG_TRACE << "CD Search results not available" << LOG_ENDL;
             }
        }
        m_Keyword_AllMusicByKeywordForIPod.clear();
    }
    else if(!query.compare(gGetMultipleAlbumSongsQuery.arg(m_AlbumName_GetMultipleAlbumSongs)))
    {
        m_AlbumName_GetMultipleAlbumSongs.clear();
//        queryId = MAP_QUERY_IDS.value(gGetMultipleAlbumSongsQuery_Key);
        queryId = eGetMultipleAlbumSongsQuery_Id;
        noOfColumns = GETMULTIPLEALBUMSONGSQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetMultipleArtistSongsQuery.arg(m_ArtistName_GetMultipleArtistSongs)))
    {
        m_ArtistName_GetMultipleArtistSongs.clear();
//        queryId = MAP_QUERY_IDS.value(gGetMultipleArtistSongsQuery_Key);
        queryId = eGetMultipleArtistSongsQuery_Id;
        noOfColumns = GETMULTIPLEARTISTSONGSQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllImagesByKeywordQuery.arg(m_Keyword_AllImagesByKeyword,
                                                           m_Keyword1_AllImagesByKeyword)))
    {
        m_Keyword_AllImagesByKeyword.clear();
        m_Keyword1_AllImagesByKeyword.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllImagesByKeywordQuery_key);
        queryId = eGetAllImagesByKeywordQuery_Id;
        noOfColumns = IMAGES_BYKEYWORD_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllVideosByKeywrodQuery.arg(m_Keyword_AllVidoesByKeyword,
                                                           m_Keyword1_AllVidoesByKeyword)))
    {
        m_Keyword_AllVidoesByKeyword.clear();
        m_Keyword1_AllVidoesByKeyword.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllVideosByKeywrodQuery_key);
        queryId = eGetAllVideosByKeywrodQuery_Id;
        noOfColumns = VIDEOS_BYKEYWORD_DATATYPECOUNT;
    }
    else if(!query.compare(gSongsCountByFolderQuery.arg(m_FolderName_SongsCountByFolder)))
    {
        m_FolderName_SongsCountByFolder.clear();
//        queryId = MAP_QUERY_IDS.value(gSongsCountByFolderQuery_Key);
        queryId = eSongsCountByFolderQuery_Id;
        noOfColumns = SONGSCOUNTBYFOLDERQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetImagesCountByFolderQuery.arg(m_FolderName_ImagesCountByFolder)))
    {
        m_FolderName_ImagesCountByFolder.clear();
//        queryId = MAP_QUERY_IDS.value(gGetImagesCountByFolderQuery_Key);
        queryId = eGetImagesCountByFolderQuery_Id;
        noOfColumns = GETIMAGESCOUNTBYFOLDERQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetVideosCountByFolderQuery.arg(m_FolderName_VideosCountByFolder)))
    {
        m_FolderName_VideosCountByFolder.clear();
//        queryId = MAP_QUERY_IDS.value(gGetVideosCountByFolderQuery_Key);
        queryId = eGetVideosCountByFolderQuery_Id;
        noOfColumns = GETVIDEOSCOUNTBYFOLDERQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllSongsCountQuery))
    {
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsCountQuery_Key);
        queryId = eGetAllSongsCountQuery_Id;
        noOfColumns = GETALLSONGSCOUNTQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllSongsFromVolumeQuery.arg(m_VolumeName_GetAllSongsFromVolume))	
        || !query.compare(gGetAllSongsFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllSongsFromVolume).arg(m_ExtendOrdering))	
        )
    {
        m_VolumeName_GetAllSongsFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsFromVolumeQuery_Key);
        queryId = eGetAllSongsFromVolumeQuery_Id;
        noOfColumns = GETALLSONGSFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllAlbumsFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsFromVolume)))
    {
        m_VolumeName_GetAllAlbumsFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllAlbumsFromVolumeQuery_Key);
        queryId = eGetAllAlbumsFromVolumeQuery_Id;
        noOfColumns = GETALLALBUMSFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(
        !query.compare(gGetAllMusicArtistsFromVolumeQuery.arg(m_VolumeName_GetAllMusicArtistsFromVolume))
      ||  !query.compare(gGetAllMusicArtistsFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllMusicArtistsFromVolume).arg(m_ExtendOrdering))
       )
    {
        m_VolumeName_GetAllMusicArtistsFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicArtistsFromVolumeQuery_Key);
        queryId = eGetAllMusicArtistsFromVolumeQuery_Id;
        noOfColumns = GETALLMUISCARTISTSFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllMusicComposersFromVolumeQuery.arg(m_VolumeName_GetAllMusicComposersFromVolume)))
    {
        m_VolumeName_GetAllMusicComposersFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicComposersFromVolumeQuery_Key);
        queryId = eGetAllMusicComposersFromVolumeQuery_Id;
        noOfColumns = GETALLMUSICCOMPOSERSFORMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllMusicGenresFromVolumeQuery.arg(m_VolumeName_GetAllMusicGenresFromVolume)))
    {
        m_VolumeName_GetAllMusicGenresFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicGenresFromVolumeQuery_Key);
        queryId = eGetAllMusicGenresFromVolumeQuery_Id;
        noOfColumns = GETALLMUSICGENRESFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllImagesFromVolumeQuery.arg(m_VolumeName_GetAllImagesFromVolume)))
    {
        m_VolumeName_GetAllImagesFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllImagesFromVolumeQuery_Key);
        queryId = eGetAllImagesFromVolumeQuery_Id;
        noOfColumns = GETALLIMAGESFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllVideosFromVolumeQuery.arg(m_VolumeName_GetAllVideosFromVolume)))
    {
        m_VolumeName_GetAllVideosFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllVideosFromVolumeQuery_Key);
        queryId = eGetAllVideosFromVolumeQuery_Id;
        noOfColumns = GETALLVIDEOSFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllSongsWithMetaDataFromVolumeQuery.arg(m_VolumeName_GetAllSongsWithMetaDataFromVolume).arg(m_OrderBy_GetAllSongsFromVolume)))
    {
        m_VolumeName_GetAllSongsWithMetaDataFromVolume.clear();
        m_OrderBy_GetAllSongsFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsWithMetaDataFromVolumeQuery_Key);
        queryId = eGetAllSongsWithMetaDataFromVolumeQuery_Id;
        noOfColumns = GETALLSONGSWITHMETADATAFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllSongsByGenreArtistAlbumQuery.arg(m_Genre_GetAllSongsByGenreArtistAlbum).
                           arg(m_Artist_GetAllSongsByGenreArtistAlbum).arg(m_Album_GetAllSongsByGenreArtistAlbum).
                           arg(m_VolumeName_GetAllSongsByGenreArtistAlbum)))
    {
        m_Genre_GetAllSongsByGenreArtistAlbum.clear();
        m_Album_GetAllSongsByGenreArtistAlbum.clear();
        m_Album_GetAllSongsByGenreArtistAlbum.clear();
        m_VolumeName_GetAllSongsByGenreArtistAlbum.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsByGenreArtistAlbumQuery_Key);
        queryId = eGetAllSongsByGenreArtistAlbumQuery_Id;
        noOfColumns = GETALLSONGSBYGENREARTISTALBUMQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllSongsByGenreComposerAlbumQuery.
                           arg(m_Genre_GetAllSongsByGenreComposerAlbum).
                           arg(m_Composer_GetAllSongsByGenreComposerAlbum).
                           arg(m_Album_GetAllSongsByGenreComposerAlbum).
                           arg(m_VolumeName_GetAllSongsByGenreComposerAlbum)))
    {
        m_Genre_GetAllSongsByGenreComposerAlbum.clear();
        m_Composer_GetAllSongsByGenreComposerAlbum.clear();
        m_Album_GetAllSongsByGenreComposerAlbum.clear();
        m_VolumeName_GetAllSongsByGenreComposerAlbum.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsByGenreComposerAlbumQuery_Key);
        queryId = eGetAllSongsByGenreComposerAlbumQuery_Id;
        noOfColumns = GETALLSONGSBYGENREARTISTALBUMQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllSongsByArtistAlbumQuery.
                           arg(m_Artist_GetAllSongsByArtistAlbum).
                           arg(m_Album_GetAllSongsByArtistAlbum).arg(m_VolumeName_GetAllSongsByArtistAlbum)))
    {
        m_Artist_GetAllSongsByArtistAlbum.clear();
        m_Album_GetAllSongsByArtistAlbum.clear();
        m_VolumeName_GetAllSongsByArtistAlbum.clear();
        LOG_TRACE<< "in case gGetAllSongsByArtistAlbumQuery" << LOG_ENDL;
        queryId = eGetAllSongsByArtistAlbumQuery_Id;
        noOfColumns = GETALLSONGSBYARTISTALBUMQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllAlbumByGenreArtistQuery.
                           arg(m_Genre_GetAllAlbumByGenreArtist).
                           arg(m_Artist_GetAllAlbumByGenreArtist).arg(m_VolumeName_GetAllAlbumByGenreArtist)))
    {
        m_Genre_GetAllAlbumByGenreArtist.clear();
        m_Artist_GetAllAlbumByGenreArtist.clear();
        m_VolumeName_GetAllAlbumByGenreArtist.clear();
        LOG_TRACE << "in case gGetAllAlbumByGenreArtistQuery" << LOG_ENDL;
        queryId = eGetAllAlbumByGenreArtistQuery_Id;
        noOfColumns = GETALLALBUMBYGENREARTISTQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllAlbumByArtistQuery.
                           arg(m_Artist_GetAllAlbumByArtist).arg(m_VolumeName_GetAllAlbumByArtist)))
    {
        m_Artist_GetAllAlbumByArtist.clear();
        m_VolumeName_GetAllAlbumByArtist.clear();
        LOG_TRACE << "in case gGetAllAlbumByArtistQuery" << LOG_ENDL;
        queryId = eGetAllAlbumByArtistQuery_Id;
        noOfColumns = GETALLALBUMBYARTISTQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllArtistByGenreQuery.
                           arg(m_Genre_GetAllArtistByGenre).arg(m_VolumeName_GetAllArtistByGenre)))
    {
        m_Genre_GetAllArtistByGenre.clear();
        m_VolumeName_GetAllArtistByGenre.clear();
        LOG_TRACE << " in case gGetAllArtistByGenreQuery" << LOG_ENDL;
        queryId = eGetAllArtistByGenreQuery_Id;
        noOfColumns = GETALLARTISTBYGENREQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllSongsByComposerAlbumQuery.arg(m_Composer_GetAllSongsByComposerAlbum).
                           arg(m_Album_GetAllSongsByComposerAlbum).
                           arg(m_VolumeName_GetAllSongsByComposerAlbum)))
    {
        m_Composer_GetAllSongsByComposerAlbum.clear();
        m_Album_GetAllSongsByComposerAlbum.clear();
        m_VolumeName_GetAllSongsByComposerAlbum.clear();
        LOG_TRACE << " in case gGetAllSongsByComposerAlbumQuery" << LOG_ENDL;
        queryId = eGetAllSongsByComposerAlbumQuery_Id;
        noOfColumns = GETALLSONGSBYCOMPOSERALBUM;
    }

    else if(!query.compare(gGetAllAlbumByGenreComposerQuery.
                           arg(m_Genre_GetMusicAlbumsByGenreComposer).
                           arg(m_Composer_GetMusicAlbumsByGenreComposer).arg(m_VolumeName_GetMusicAlbumsByGenreComposer)))
    {
        LOG_TRACE << " in case gGetAllAlbumByGenreComposerQuery" << LOG_ENDL;
        m_Genre_GetMusicAlbumsByGenreComposer.clear();
        m_Composer_GetMusicAlbumsByGenreComposer.clear();
        m_VolumeName_GetMusicAlbumsByGenreComposer.clear();
        queryId = eGetAllAlbumByGenreComposerQuery_Id;
        noOfColumns = GETALLALBUMBYGENERECOMPOSER;
    }


    else if(!query.compare(gGetAllAlbumByComposerQuery.
                           arg(m_Composer_GetAllAlbumByComposer).arg(m_VolumeName_GetAllAlbumByComposer)))
    {
        LOG_TRACE << "in case gGetAllAlbumByComposerQuery" << LOG_ENDL;
        m_Composer_GetAllAlbumByComposer.clear();
        m_VolumeName_GetAllAlbumByComposer.clear();
        queryId = eGetAllAlbumByComposerQuery_Id;
        noOfColumns = GETALLALBUMBYCOMPOSER ;
    }


    else if(!query.compare(gGetAllComposerByGenreQuery.
                           arg(m_Genre_GetAllComposersByGenre).arg(m_VolumeName_GetAllComposersByGenre)))
    {
        LOG_TRACE << " in case gGetAllComposerByGenreQuery" << LOG_ENDL;
        m_Genre_GetAllComposersByGenre.clear();
        m_VolumeName_GetAllComposersByGenre.clear();
        queryId = eGetAllComposerByGenreQuery_Id;
        noOfColumns = GETALLCOMPOSERBYGENER;
    }
    else if(!query.compare(gGetAllsongsByKeywordFromVolumeQuery.
                           arg(m_SongKeyword_GetAllsongsByKeywordFromVolume).arg(m_SongKeyword_EucKR_GetAllsongsByKeywordFromVolume).arg(m_VolumeName_forKeyword)))
    {
        LOG_TRACE << " in case gGetAllsongsByKeywordFromVolumeQuery" << LOG_ENDL;
        m_SongKeyword_GetAllsongsByKeywordFromVolume.clear();
        m_SongKeyword_EucKR_GetAllsongsByKeywordFromVolume.clear();
        m_VolumeName_forKeyword.clear();
        queryId = eGetAllsongsByKeywordFromVolumeQuery_Id;
        noOfColumns = GETALLSONGSBYKEYWORDFROMVOLUME_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllGenresByKeywordFromVolumeQuery.
                           arg(m_GenreKeyword_GetAllGenresByKeywordFromVolume).arg(m_GenreKeyword_EucKR_GetAllGenresByKeywordFromVolume).arg(m_VolumeName_forKeyword)))
    {
        LOG_TRACE << " in case gGetAllGenresByKeywordFromVolumeQuery" << LOG_ENDL;
        m_GenreKeyword_GetAllGenresByKeywordFromVolume.clear();
        m_GenreKeyword_EucKR_GetAllGenresByKeywordFromVolume.clear();
        m_VolumeName_forKeyword.clear();
        queryId = eGetAllGenresByKeywordFromVolumeQuery_Id;
        noOfColumns = GETALLGENREBYKEYWORDFROMVOLUME_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllArtistsByKeywordFromVolumeQuery.
                           arg(m_ArtistKeyword_GetAllArtistsByKeywordFromVolume).arg( m_ArtistKeyword_EucKR_GetAllArtistsByKeywordFromVolume).arg(m_VolumeName_forKeyword)))
    {
        LOG_TRACE << " in case gGetAllArtistsByKeywordFromVolumeQuery" << LOG_ENDL;
        m_ArtistKeyword_GetAllArtistsByKeywordFromVolume.clear();
        m_ArtistKeyword_EucKR_GetAllArtistsByKeywordFromVolume.clear();
        m_VolumeName_forKeyword.clear();
        queryId = eGetAllArtistsByKeywordFromVolumeQuery_Id;
        noOfColumns = GETALLARTISTBYKEYWORDFROMVOLUME_DATATYPECOUNT;
    }

    else if(
        !query.compare(gGetAllSongsByAlbumFromVolumeQuery.arg(m_AlbumKeyword_GetAllSongsByAlbumFromVolume,
                               m_VolumeName_GetAllSongsByAlbumFromVolume))
        || !query.compare(gGetAllSongsByAlbumFromVolumeExtOrderQuery.arg(m_AlbumKeyword_GetAllSongsByAlbumFromVolume,
                               m_VolumeName_GetAllSongsByAlbumFromVolume, m_ExtendOrdering))
      )
    {
        LOG_TRACE << " in case geGetAllSongsByAlbumFromVolumeQuery_Id" << LOG_ENDL;
        m_AlbumKeyword_GetAllSongsByAlbumFromVolume.clear();
        queryId = eGetAllSongsByAlbumFromVolumeQuery_Id;
        noOfColumns = GETALLSONGSBYALBUMFROMVOLUME;
    }


    else if(!query.compare(gGetAllAlbumsByKeywordFromVolumeQuery.
                           arg(m_AlbumKeyword_GetAllAlbumsByKeywordFromVolume).arg(m_AlbumKeyword_EucKR_GetAllAlbumsByKeywordFromVolume).arg(m_VolumeName_forKeyword)))
    {
        LOG_TRACE << " in case gGetAllAlbumsByKeywordFromVolumeQuery" << LOG_ENDL;
        m_AlbumKeyword_GetAllAlbumsByKeywordFromVolume.clear();
        m_AlbumKeyword_EucKR_GetAllAlbumsByKeywordFromVolume.clear();
        m_VolumeName_forKeyword.clear();
        queryId = eGetAllAlbumsByKeywordFromVolumeQuery_Id;
        noOfColumns = GETALLALBUMSYKEYWORDFROMVOLUME_DATATYPECOUNT;
    }


    else if(!query.compare(gGetAllSongsbyPlaylistQuery.arg(m_playlist_Getallsongsbyplaylist).arg(m_playlist_TypeName)))
    {
        LOG_TRACE << " in case gGetAllSongsbyPlaylistQuery" << LOG_ENDL;
        m_playlist_Getallsongsbyplaylist.clear();
        m_playlist_TypeName.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsbyPlaylistQuery_Key);
        queryId = eGetAllSongsbyPlaylistQuery_Id;
        noOfColumns = GETPLAYLISTDATA;
    }

    else if(!query.compare(gGetlistofPlaylistsQuery))
    {
        LOG_TRACE << " in case gGetlistofPlaylistsQuery" << LOG_ENDL;
//        queryId = MAP_QUERY_IDS.value(gGetlistofPlaylistsQuery_Key);
        queryId = eGetlistofPlaylistsQuery_Id;
        noOfColumns = GETLISTOFPLAYLIST;
    }
    else if(!query.compare(gGetMostPlayedMusicSongsQuery.arg(m_VolumeName_GetMostPlayedMusicSongs)
                           ))
    {
        m_VolumeName_GetMostPlayedMusicSongs.clear();
        LOG_TRACE << " in case gGetMostPlayedMusicSongsQuery" << LOG_ENDL;
//        queryId = MAP_QUERY_IDS.value(gGetMostPlayedMusicSongsQueryy_Key);
        queryId = eGetMostPlayedMusicSongsQuery_Id;
        noOfColumns = GETMOSTPLAYEDMUSICSONGS;
    }
    else if(!query.compare(gGetMostRecentlyPlayedSongsQuery.arg(m_VolumeName_GetMostRecentlyPlayedSongs)
                           ))
    {
        m_VolumeName_GetMostRecentlyPlayedSongs.clear();
        LOG_TRACE << " in case gGetMostRecentlyPlayedSongsQuery" << LOG_ENDL;
//        queryId = MAP_QUERY_IDS.value(gGetMostRecentlyPlayedSongsQuery_Key);
        queryId = eGetMostRecentlyPlayedSongsQuery_Id;
        noOfColumns = GETMOSTRECENTPLAYEDSONGS;
    }
    else if(!query.compare(gGetMusicPlaylistByKeywordQuery.arg(m_Keyword_MusicPlaylistByKeyword).arg(m_playlist_TypeName)
                           ))
    {
        m_Keyword_MusicPlaylistByKeyword.clear();
        m_playlist_TypeName.clear();
        LOG_TRACE << " in case GetMusicPlaylistByKeywordQuery" << LOG_ENDL;
//        queryId = MAP_QUERY_IDS.value(gGetMusicPlaylistByKeywordQuery_Key);
        queryId = eGetMusicPlaylistByKeywordQuery_Id;
        noOfColumns = GETMUSICPLAYLISTBYKEYWORD;
    }
    else if(!query.compare(gGetAllVideosCountFromVolumeQuery.arg(m_VolumeStr_GetAllVideosCountFromVolume)))
    {
        m_VolumeStr_GetAllVideosCountFromVolume.clear();
        queryId = eGetAllVideosCountFromVolumeQuery_Id;
        noOfColumns = GETALLVIDEOSCOUNTFROMVOLUME_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllImagesCountFromVolumeQuery.arg(m_VolumeStr_GetAllImagesCountFromVolume)))
    {
        m_VolumeStr_GetAllImagesCountFromVolume.clear();
        queryId = eGetAllImagesCountFromVolumeQuery_Id;
        noOfColumns = GETALLIMAGESCOUNTFROMVOLUME_DATATYPECOUNT;
    }
    else if(!query.compare(gGetMusicFromCDQuery.arg(m_cd_toc)))
    {
        m_cd_toc.clear();
//        queryId = MAP_QUERY_IDS.value(gGetMusicFromCDQuery_Key);
        queryId = eGetMusicFromCDQuery_Id;
        noOfColumns = GETALLMUSICFROMCDQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetMusicDetailFromIpodOnIndexBasisQuery.arg(m_StartIndex).arg(m_EndIndex)))
    {
//        queryId = MAP_QUERY_IDS.value(gGetMusicDetailFromIpodOnIndexBasisQuery_Key);
        queryId = eGetMusicDetailFromIpodOnIndexBasis_Id;
        noOfColumns = GETMUSICDETAILFROMIPODONINDEXBASIS_DATATYPECOUNT;
    }
    else if(!query.compare(gGetImagesBySubFolderQuery.arg(m_FolderName_ImagesWithSubFolder)))
    {
        m_FolderName_ImagesWithSubFolder.clear();
//        queryId = MAP_QUERY_IDS.value(gGetImagesBySubFolderQuery_Key);
        queryId = eGetImagesBySubFolderQuery_Id;
        noOfColumns = GETIMAGESBYSUBFOLDERQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetVideosBySubFolderQuery.arg(m_FolderName_VideosWithSubFolder)))
    {
        m_FolderName_VideosWithSubFolder.clear();
//        queryId = MAP_QUERY_IDS.value(gGetVideosBySubFolderQuery_Key);
        queryId = eGetVideosBySubFolderQuery_Id;
        noOfColumns = GETVIDEOSBYSUBFOLDERQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllAlbumsWithCoverArtFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArt)))
    {
        m_VolumeName_GetAllAlbumsWithCoverArt.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllAlbumsWithCoverArtFromVolumeQuery_Key);
        queryId = eGetAllAlbumsWithCoverArtFromVolumeQuery_Id;
        noOfColumns = GETALLALBUMSWITHCOVERARTFROMVOLUMEQUERY_DATATYPECOUNT;
        LOG_TRACE << "[MDP] Query Mapping for gGetAllAlbumsWithCoverArtFromVolumeQuery" << LOG_ENDL;
    }
    else if(!query.compare(gGetAllSongsWithoutMetaInfoFromVolumeQuery.arg(m_VolumeName_GetAllSongsWithoutMetaInfoFromVolume).arg(m_Query_Offset).arg(m_Query_Limit)))
    {
        m_VolumeName_GetAllSongsWithoutMetaInfoFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsWithoutMetaInfoFromVolumeQuery_key);
        queryId = eGetAllSongsWithoutAlbumFromVolumeQuery_Id;
        noOfColumns = GETALLSONGSWITHOUTMETAINFOFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllMusicByKeywordFromVolumeQuery.arg(m_Keyword_GetAllMusicByKeywordFromVolume,
                                                          m_Keyword1_GetAllMusicByKeywordFromVolume, m_VolumeName_GetAllMusicByKeywordFromVolume)))
    {
        m_Keyword_GetAllMusicByKeywordFromVolume.clear();
        m_Keyword1_GetAllMusicByKeywordFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicByKeywordFromVolumeQuery_Key);
        queryId = eGetAllMusicByKeywordFromVolumeQuery_Id;
        noOfColumns = GETALLMUSICBYKEYWORDFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetMusicMetaDataInfoFromURLListQuery.arg(m_FilePathInfo_MusicMetaData)))
    {
        m_FilePathInfo_MusicMetaData.clear();
        queryId = eGetMusicMetaDataInfoFromUrlListQuery_Id;
        noOfColumns = GETMUSICMETADATAINFOFROMURLLISTQUERY_DATATYPECOUNT;
    }
	// { added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
	else if(!query.compare(gGetPhotoMetaDataInfoFromURLQuery.arg(m_FilePathInfo_MusicMetaData)))
    {
        m_FilePathInfo_MusicMetaData.clear();
        queryId = eGetPhotoMetaDataInfoFromUrlQuery_Id;
        noOfColumns = GETPHOTOMETADATAINFOFROMURLQUERY_DATATYPECOUNT;
    }
	// } added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
    else if(
        !query.compare(gGetAllAlbumsWithUrlFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithUrl))
        || !query.compare(gGetAllAlbumsWithUrlFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllAlbumsWithUrl).arg(m_ExtendOrdering))
        )
    {
        m_VolumeName_GetAllAlbumsWithUrl.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllAlbumsWithUrlFromVolumeQuery_Key);
        queryId = eGetAllAlbumsWithUrlFromVolumeQuery_Id;
        noOfColumns = GETALLALBUMSWITHURLFROMVOLUMEQUERY_DATATYPECOUNT;
        LOG_TRACE << "[MDP] Query Mapping for gGetAllAlbumsWithUrlFromVolumeQuery" << LOG_ENDL;
    }
    else if( //modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllSongsByArtistFromVolumeQuery.arg(m_ArtistName_GetAllSongsByArtistFromVolume, 
                               m_VolumeName_GetAllSongsByArtistFromVolume))
        || !query.compare(gGetAllSongsByArtistFromVolumeExtOrderQuery.arg(m_ArtistName_GetAllSongsByArtistFromVolume, 
                               m_VolumeName_GetAllSongsByArtistFromVolume, m_ExtendOrdering))
        )
    {
        m_ArtistName_GetAllSongsByArtistFromVolume.clear();
		m_VolumeName_GetAllSongsByArtistFromVolume.clear();
        queryId = eGetAllSongsByArtistFromVolumeQuery_Id;
        noOfColumns = GETALLSONGSBYARTISTFROMVOLUMEQUERY_DATATYPECOUNT;
    }
	// { added eugene.seo 2013.02.13
    else if(
        !query.compare(gGetAllSongsByGenreFromVolumeQuery.arg(m_GenreName_GetAllSongsByGenreFromVolume,
                                              m_VolumeName_GetAllSongsByGenreFromVolume))
        || !query.compare(gGetAllSongsByGenreFromVolumeExtOrderQuery.arg(m_GenreName_GetAllSongsByGenreFromVolume,
                                              m_VolumeName_GetAllSongsByGenreFromVolume, m_ExtendOrdering))
        )
    {
        m_GenreName_GetAllSongsByGenreFromVolume.clear();
		m_VolumeName_GetAllSongsByGenreFromVolume.clear();
        queryId = eGetAllSongsByGenreFromVolumeQuery_Id;
        noOfColumns = GETALLSONGSBYGENREFROMVOLUMEQUERY_DATATYPECOUNT;
    }
	// } added eugene.seo 2013.02.13
    else if(!query.compare(gGetAllSongsCountFromVolumeQuery.arg(m_VolumeName_GetAllSongsCountFromVolume)))
    {
        m_VolumeName_GetAllSongsCountFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsCountFromVolumeQuery_key);
        queryId = eGetAllSongsCountFromVolumeQuery_Id;
        noOfColumns = GETALLSONGSCOUNTFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllSongsWithTitleCountFromVolumeQuery.arg(m_VolumeName_GetAllSongsWithTitleCountFromVolume)))
    {
        m_VolumeName_GetAllSongsWithTitleCountFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsWithTitleCountFromVolumeQuery_Key);
        queryId = eGetAllSongsWithTitleCountFromVolumeQuery_Id;
        noOfColumns = GETALLSONGSWITHTITLECOUNTFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllAlbumsCountFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsCountFromVolume)))
    {
        m_VolumeName_GetAllAlbumsCountFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllAlbumsCountFromVolumeQuery_Key);
        queryId = eGetAllAlbumsCountFromVolumeQuery_Id;
        noOfColumns = GETALLALBUMSCOUNTFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllMusicArtistsCountFromVolumeQuery.arg(m_VolumeName_GetAllMusicArtistsCountFromVolume)))
    {
        m_VolumeName_GetAllMusicArtistsCountFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicArtistsCountFromVolumeQuery_Key);
        queryId = eGetAllMusicArtistsCountFromVolumeQuery_Id;
        noOfColumns = GETALLMUSICARTISTSCOUNTFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllMusicComposersCountFromVolumeQuery.arg(m_VolumeName_GetAllMusicComposersCountFromVolume)))
    {
        m_VolumeName_GetAllMusicComposersCountFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicComposersCountFromVolumeQuery_Key);
        queryId = eGetAllMusicComposersCountFromVolumeQuery_Id;
        noOfColumns = GETALLMUSICCOMPOSERSCOUNTFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllMusicGenresCountFromVolumeQuery.arg(m_VolumeName_GetAllMusicGenresCountFromVolume)))
    {
        m_VolumeName_GetAllMusicGenresCountFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicGenresCountFromVolumeQuery_Key);
        queryId = eGetAllMusicGenresCountFromVolumeQuery_Id;
        noOfColumns = GETALLMUSICGENRESCOUNTFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllAlbumsWithCoverArtCountFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArtCountFromVolume)))
    {
        m_VolumeName_GetAllAlbumsWithCoverArtCountFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllAlbumsWithCoverArtCountFromVolumeQuery_Key);
        queryId = eGetAllAlbumsWithCoverArtCountFromVolumeQuery_Id;
        noOfColumns = GETALLALBUMSWITHCOVERARTCOUNTFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if( //modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllSongsWithTitleIndexFromVolumeQuery.arg(m_VolumeName_GetAllSongsWithTitleIndexFromVolume))
       || !query.compare(gGetAllSongsWithTitleIndexFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllSongsWithTitleIndexFromVolume)
            .arg(m_ExtendOrdering))
        )
    {
    	letter = m_Letter_GetAllSongsWithTitleIndexFromVolume;
		m_Letter_GetAllSongsWithTitleIndexFromVolume.clear();
        m_VolumeName_GetAllSongsWithTitleIndexFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsWithTitleIndexFromVolumeQuery_Key);
        queryId = eGetAllSongsWithTitleIndexFromVolumeQuery_Id;
        noOfColumns = GETALLSONGSWITHTITLEINDEXFROMVOLUMEQUERY_DATATYPECOUNT;
        LOG_TRACE << "[MDP] Query Mapping for gGetAllSongsWithTitleIndexFromVolumeQuery" << LOG_ENDL;
    }
    else if( //modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllMusicArtistsFromVolumeQuery.arg(m_VolumeName_GetAllMusicArtistsIndexFromVolume))
        || !query.compare(gGetAllMusicArtistsFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllMusicArtistsIndexFromVolume)
            .arg(m_ExtendOrdering))
        )
    {
    	letter = m_Letter_GetAllMusicArtistsIndexFromVolume;
		m_Letter_GetAllMusicArtistsIndexFromVolume.clear();    
        m_VolumeName_GetAllMusicArtistsIndexFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicArtistsIndexFromVolumeQuery_Key);
        queryId = eGetAllMusicArtistsIndexFromVolumeQuery_Id;
        noOfColumns = GETALLMUISCARTISTSFROMVOLUMEQUERY_DATATYPECOUNT;
        LOG_TRACE << "[MDP] Query Mapping for gGetAllMusicArtistsIndexFromVolumeQuery" << LOG_ENDL;
    }	
    else if(!query.compare(gGetAllMusicGenresFromVolumeQuery.arg(m_VolumeName_GetAllMusicGenresIndexFromVolume)))
    {
    	letter = m_Letter_GetAllMusicGenresIndexFromVolume;
		m_Letter_GetAllMusicGenresIndexFromVolume.clear();    
        m_VolumeName_GetAllMusicGenresIndexFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicGenresIndexFromVolumeQuery_Key);
        queryId = eGetAllMusicGenresIndexFromVolumeQuery_Id;
        noOfColumns = GETALLMUSICGENRESFROMVOLUMEQUERY_DATATYPECOUNT;
        LOG_TRACE << "[MDP] Query Mapping for gGetAllMusicGenresIndexFromVolumeQuery" << LOG_ENDL;
    }		
    else if( //modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllAlbumsWithCoverArtIndexFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArtIndexFromVolume))
        || !query.compare(gGetAllAlbumsWithCoverArtIndexFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArtIndexFromVolume)
            .arg(m_ExtendOrdering))
        )
    {
    	letter = m_Letter_GetAllAlbumsWithCoverArtIndexFromVolume;
		m_Letter_GetAllAlbumsWithCoverArtIndexFromVolume.clear();       
        m_VolumeName_GetAllAlbumsWithCoverArtIndexFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Key);
        queryId = eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id;
        noOfColumns = GETALLALBUMSWITHCOVERARTINDEXFROMVOLUMEQUERY_DATATYPECOUNT;
        LOG_TRACE << "[MDP] Query Mapping for gGetAllAlbumsWithCoverArtIndexFromVolumeQuery" << LOG_ENDL;
    }
	else if(!query.compare(gGetMultipleAlbumSongsByArtistFromVolumeQuery.
                                        arg(m_Argument_GetMultipleAlbumSongsByArtistFromVolume)))
    {
        m_Argument_GetMultipleAlbumSongsByArtistFromVolume.clear();
        queryId = eGetMultipleAlbumSongsByArtistFromVolumeQuery_Id;
        noOfColumns = GETMULTIPLEALBUMSONGSBYARTISTFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetMultipleGenreSongsFromVolumeQuery.
                                        arg(m_Argument_GetMultipleGenreSongsFromVolume)))
    {
        m_Argument_GetMultipleGenreSongsFromVolume.clear();
        queryId = eGetMultipleGenreSongsFromVolumeQuery_Id;
        noOfColumns = GETMULTIPLEGENRESSONGSFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetMultipleArtistSongsByGenreFromVolumeQuery.
                                        arg(m_Argument_GetMultipleArtistSongsByGenreFromVolume)))
    {
        m_Argument_GetMultipleArtistSongsByGenreFromVolume.clear();
        queryId = eGetMultipleArtistSongsByGenreFromVolumeQuery_Id;
        noOfColumns = GETMULTIPLEARTISTSONGSBYGENREFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetMultipleAlbumSongsByGenreArtistFromVolumeQuery.
                                        arg(m_Argument_GetMultipleAlbumSongsByGenreArtistFromVolume)))
    {
        m_Argument_GetMultipleAlbumSongsByGenreArtistFromVolume.clear();
        queryId = eGetMultipleAlbumSongsByGenreArtistFromVolumeQuery_Id;
        noOfColumns = GETMULTIPLEALBUMSONGSBYGENREARTISTFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetMultipleFolderSongsQuery.arg(m_Argument_GetMultipleFolderSongs)))
    {
        m_Argument_GetMultipleFolderSongs.clear();
        queryId = eGetMultipleFolderSongsQuery_Id;
        noOfColumns = GETMULTIPLEFOLDERSONGSQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetMultipleAlbumSongsFromVolumeQuery.
                                        arg(m_Argument_GetMultipleAlbumSongsFromVolume)))
    {
        m_Argument_GetMultipleAlbumSongsFromVolume.clear();
        queryId = eGetMultipleAlbumSongsFromVolumeQuery_Id;
        noOfColumns = GETMULTIPLEALBUMSONGSFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetMultipleArtistSongsFromVolumeQuery.
                                        arg(m_Argument_GetMultipleArtistSongsFromVolume)))
    {
        m_Argument_GetMultipleArtistSongsFromVolume.clear();
        queryId = eGetMultipleArtistSongsFromVolumeQuery_Id;
        noOfColumns = GETMULTIPLEARTISTSONGSFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllMusicByKeywordForVRQuery.
                           arg(m_Keyword_AllMusicByKeywordForVR, m_Keyword1_AllMusicByKeywordForVR)))
    {
        m_Keyword_AllMusicByKeywordForVR.clear();
        m_Keyword1_AllMusicByKeywordForVR.clear();
        queryId = eGetAllMusicByKeywordQuery_Id;
        noOfColumns = GETALLMUSICBYKEYWORDFORVRQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllMetaDataWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllMetaDataWithLimitFromVolume,
                                                    m_OrderBy_GetMetaDataFromVolume).arg(m_Query_Offset )))
    {
        m_VolumeName_GetAllMetaDataWithLimitFromVolume.clear();
        m_OrderBy_GetMetaDataFromVolume.clear();
        queryId = eGetAllMetaDataWithLimitFromVolumeQuery_Id;
        noOfColumns = GETALLMETADATAWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;

    }
    else if(!query.compare(gGetAllMusicByConnectedVolumesQuery.arg(m_PathList_GetAllMusicByConnectedVolumes)))
    {

        QString queryStr;
        queryStr.append("select T_Title, T_Album, T_Artist, T_Genre, T_FilePath From MasterTable");
        m_PathList_GetAllMusicByConnectedVolumes.clear();

        LOG_TRACE << "got event for music by connected volumes" << LOG_ENDL;
        queryId = eGetAllMusicByConnectedVolumesQuery_Id;
        noOfColumns = GETALLMUSICBYCONNECTEDVOLUMESQUERY_DATATYPECOUNT;

        if(m_Ipod_DBConnList.count() > 0)
        {
            if(m_Ipod_DBConnList[0].isOpen())
                 senddata(noOfColumns, queryStr, m_Ipod_DBConnList[0], queryDataList);
            if(m_Ipod_DBConnList[1].isOpen())
                senddata(noOfColumns, queryStr, m_Ipod_DBConnList[1], queryDataList);
        }
    }
    else
    {
        LOG_HIGH << "Query MAPPING Error"<< LOG_ENDL;
        HandleError(eQueryMappingError);
        if(result != NULL)
        {
            delete result;
            result = NULL;
        }
        return;
    }

    LOG_HIGH << "queryId  = "<< queryId << LOG_ENDL;

    m_FirstTime = true;
    if(result->hasError())
    {
        LOG_CRITICAL <<"Error In Query :"<< query << LOG_ENDL;
        HandleError(eErrorInQuery);
        QSparqlError queryError = result->lastError();
        LOG_CRITICAL <<"Query Error type = "<<queryError.type()
               << "Query Error Message = "<<queryError.message() << LOG_ENDL;
        if(result != NULL)
        {
            delete result;
            result = NULL;
        }
        return;
    }

    if( (queryId == eGetAllSongsWithTitleIndexFromVolumeQuery_Id) ||
		(queryId == eGetAllMusicArtistsIndexFromVolumeQuery_Id)   ||
		(queryId == eGetAllMusicGenresIndexFromVolumeQuery_Id)    ||
		(queryId == eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id) )
    {
        PrepareIndexDataList(result, queryDataList, queryId, letter, false);

        if(result != NULL)
        {
            delete result;
            result = NULL;
        }
        return;
    }
//{ added by jaehwan to fix ITS 210480
    if ( queryId == eGetAllAlbumsWithUrlFromVolumeQuery_Id )
    {
        QString prevAlbum = "";
        while (result->next())
        {
            QStringList columnList;
            QString album = result->value(0).toString();;
            if (album == prevAlbum) //drop duplicated album.
            {
                continue;
            }
            else
            {
                prevAlbum = album;
            }
            for (int i = 0; i < noOfColumns; i++)
            {
                QString value = result->value(i).toString();

                if (value.startsWith(QLatin1String("file://")))
                {
                    value = QLatin1String("file://") + QUrl::fromEncoded(value.toLatin1()).toLocalFile();
                }
                columnList.append(value);
            }
            queryDataList.append(columnList);
        }
    }
//} added by jaehwan
    else // modified by jaehwan to fix ITS 210480
    {
        // retrieving data from QSparqlResult
        while (result->next())
        {
            QStringList columnList;
            for (int i = 0; i < noOfColumns; i++)
            {
                QString value = result->value(i).toString();
                if (value.startsWith(QLatin1String("file://"))) {
                    value = QLatin1String("file://") + QUrl::fromEncoded(value.toLatin1()).toLocalFile();
                }

                columnList.append(value);
            }
            queryDataList.append(columnList);
        }
    } // modified by jaehwan to fix ITS 210480
    if (GetLanguage() == LANGUAGE_ZH_CMN || GetLanguage() == LANGUAGE_ZH_CN)
    {
        int indexColumn = 0;

        switch (queryId)
        {
        case eGetAllSongsFromVolumeQuery_Id:
             indexColumn = 1;
        //case eGetAllAlbumsWithUrlFromVolumeQuery_Id:
        //case eGetAllSongsByAlbumFromVolumeQuery_Id:
        case eGetAllMusicArtistsFromVolumeQuery_Id:
        case eGetAllAlbumByArtistQuery_Id:
        //case eGetAllSongsByArtistAlbumQuery_Id:
        case eGetAllMusicGenresFromVolumeQuery_Id:
        case eGetAllSongsByGenreFromVolumeQuery_Id:
            LOG_CRITICAL << "reorderListForChina : queryId = "<<queryId<< LOG_ENDL;
            reorderListForChina(queryDataList, indexColumn);
            break;
        default:
            break;
        }
    }
    //{ added by jaehwan.lee 2014.2.12 to release memory of sparql query result
    if (result)
    {
        //LOG_CRITICAL << "DELETE SPARQL Query Result !!!" << LOG_ENDL;
        delete result;
        result = NULL;
    }
    //} added by jaehwan.lee

    /* need to Check the Count As in Some cases Count is Nill and trying to Append Data */
    /*  if(queryId == eGetMusicMetaDataQuery_Id  && queryDataList.count())
    {
        queryDataList[0].append(m_FilePath_MusicMetaData);
        m_FilePath_MusicMetaData.clear();
    }*/

    if(queryId == eGetImagesByFolderQuery_Id )
    {
        ModifyDataForImages(queryDataList);
        m_FolderName_ImagesByFolder.clear();
    }
    else if(queryId == eGetVideosByFolderQuery_Id)
    {
        ModifyDataForVideos(queryDataList);
        m_FolderName_VideosByFolder.clear();
    }
    else if(queryId == eGetAllMusicByKeywordQuery_Id || queryId == eGetAllMusicByConnectedVolumesQuery_Id)
    {
        CheckIfFileExists(queryDataList, 4);
    }

    // after data fetching emiting a signal to notify CTrackerAbstractor
    emit DataFetched(queryDataList, queryId);
}
/******************************************************************************

// Function Name                                            Parameters

// CFullTextSearch::OnLimitFinished                        void (OUT)

//

// Explanation

// This is a slot called when finished signal emited by QSparqlResult on query completion

// It emits DataLimitFetched signal for TrackerAbstractor

//

// History

// 8-Mar-2012     Byungjo Hyun                created hanlding music seach query completion signal
*****************************************************************************/
void CMediaDataProvider::OnLimitFinished()
{
    QVector<QStringList> queryDataList;
    int noOfColumns = 0;
    QString query = gBlankSpace;
	
    ETrackerAbstarctor_Query_Ids queryId = eDefault_Id;
    QSparqlResult* result = qobject_cast<QSparqlResult*>(sender());
    if(result == NULL)  return; // added by sungha.choi 2013.07.24

    query = result->query();

    
    if( //modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllSongsWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllSongsWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit))
        || !query.compare(gGetAllSongsWithLimitFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllSongsWithLimitFromVolume)
                 .arg(m_ExtendOrdering).arg(m_Query_Offset).arg(m_Query_Limit))
        )
    {
        m_VolumeName_GetAllSongsWithLimitFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsWithLimitFromVolumeQuery_Key);
        queryId = eGetAllSongsWithLimitFromVolumeQuery_Id;
        noOfColumns = GETALLSONGSWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if( //modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllAlbumsWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit))
        ||  !query.compare(gGetAllAlbumsWithLimitFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllAlbumsWithLimitFromVolume)
                .arg(m_ExtendOrdering).arg(m_Query_Offset).arg(m_Query_Limit))
        )
    {
        m_VolumeName_GetAllAlbumsWithLimitFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllAlbumsWithLimitFromVolumeQuery_Key);
        queryId = eGetAllAlbumsWithLimitFromVolumeQuery_Id;
        noOfColumns = GETALLALBUMSWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if( //modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllMusicArtistsWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllMusicArtistsWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit))
        ||  !query.compare(gGetAllMusicArtistsWithLimitFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllMusicArtistsWithLimitFromVolume)
                .arg(m_ExtendOrdering).arg(m_Query_Offset).arg(m_Query_Limit))
        )
    {
        m_VolumeName_GetAllMusicArtistsWithLimitFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicArtistsWithLimitFromVolumeQuery_Key);
        queryId = eGetAllMusicArtistsWithLimitFromVolumeQuery_Id;
        noOfColumns = GETALLMUISCARTISTSWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllMusicComposersWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllMusicComposersWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit)))
    {
        m_VolumeName_GetAllMusicComposersWithLimitFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicComposersWithLimitFromVolumeQuery_Key);
        queryId = eGetAllMusicComposersWithLimitFromVolumeQuery_Id;
        noOfColumns = GETALLMUSICCOMPOSERSWITHLIMITFORMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllMusicGenresWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllMusicGenresWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit)))
    {
        m_VolumeName_GetAllMusicGenresWithLimitFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicGenresWithLimitFromVolumeQuery_Key);
        queryId = eGetAllMusicGenresWithLimitFromVolumeQuery_Id;
        noOfColumns = GETALLMUSICGENRESWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if( //modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit))
        || !query.compare(gGetAllAlbumsWithCoverArtWithLimitFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume)
            .arg(m_ExtendOrdering).arg(m_Query_Offset).arg(m_Query_Limit))
        )
    {
        m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Key);
        queryId = eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id;
        noOfColumns = GETALLALBUMSWITHCOVERARTWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else
    {
        LOG_CRITICAL << "Mapping ERROR Query" << query << LOG_ENDL;
        LOG_CRITICAL << "Mapping ERROR Query  Number" << queryId << LOG_ENDL;

        HandleError(eQueryMappingError);
        if(result != NULL)
        {
            delete result;
            result = NULL;
        }
        return;
    }

    LOG_HIGH << "queryId = "<< queryId << LOG_ENDL;

    m_FirstTime = true;
    if(result->hasError())
    {
        LOG_CRITICAL <<"Error In Query :"<< query << LOG_ENDL;
        HandleError(eErrorInQuery);
        QSparqlError queryError = result->lastError();
        LOG_CRITICAL <<"Query Error type = "<<queryError.type()
               << "Query Error Message = "<<queryError.message() << LOG_ENDL;
        if(result != NULL)
        {
            delete result;
            result = NULL;
        }
        return;
    }

    // retrieving data from QSparqlResult
    while (result->next())
    {
        QStringList columnList;
        for (int i = 0; i < noOfColumns; i++)
        {
            QString value = result->value(i).toString();
            if (value.startsWith(QLatin1String("file://"))) {
                value = QLatin1String("file://") + QUrl::fromEncoded(value.toLatin1()).toLocalFile();
            }

            columnList.append(value);
        }
        queryDataList.append(columnList);
    }
    //{added by jaehwan.lee 2014.2.12 to release memory of sparql query result
    if (result)
    {
        delete result;
        result = NULL;
    }
    //}added by jaehwan.lee

    // after data fetching emiting a signal to notify CTrackerAbstractor
    emit DataLimitFetched(queryDataList, queryId, m_Query_Offset);
}
// { added by sungha.choi 2013.07.23
void CMediaDataProvider::OnLimitCacheFinished()
{
    QVector<QStringList> *queryDataList;    // modified by sungha.choi 2013.07.23
    QVector<QStringList> dataList;    // added by sungha.choi 2013.07.23

    int noOfColumns = 0;
    QString query = gBlankSpace;

    ETrackerAbstarctor_Query_Ids queryId = eDefault_Id;
    QSparqlResult* result = qobject_cast<QSparqlResult*>(sender());
    if(result == NULL)  return; // added by sungha.choi 2013.07.24

    query = result->query();

    queryDataList = &dataList;  // added by sungha.choi
    //modify by jawhean.lee for audio list sorting issue. 13.10. 02 
    if(!query.compare(gGetAllSongsWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllSongsWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit))
       || !query.compare(gGetAllSongsWithLimitFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllSongsWithLimitFromVolume)
            .arg(m_ExtendOrdering).arg(m_Query_Offset).arg(m_Query_Limit))
        )
    {
        m_VolumeName_GetAllSongsWithLimitFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllSongsWithLimitFromVolumeQuery_Key);
        queryId = eGetAllSongsWithLimitFromVolumeQuery_Id;
        noOfColumns = GETALLSONGSWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
        songDataSavedList.clear();
        queryDataList = &songDataSavedList;
    }
    else if( //modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllAlbumsWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit))
       ||  !query.compare(gGetAllAlbumsWithLimitFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllAlbumsWithLimitFromVolume)
            .arg(m_ExtendOrdering).arg(m_Query_Offset).arg(m_Query_Limit))
        )
    {
        m_VolumeName_GetAllAlbumsWithLimitFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllAlbumsWithLimitFromVolumeQuery_Key);
        queryId = eGetAllAlbumsWithLimitFromVolumeQuery_Id;
        noOfColumns = GETALLALBUMSWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if( //modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllMusicArtistsWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllMusicArtistsWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit))
        ||  !query.compare(gGetAllMusicArtistsWithLimitFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllMusicArtistsWithLimitFromVolume)
            .arg(m_ExtendOrdering).arg(m_Query_Offset).arg(m_Query_Limit))
        )
    {
        m_VolumeName_GetAllMusicArtistsWithLimitFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicArtistsWithLimitFromVolumeQuery_Key);
        queryId = eGetAllMusicArtistsWithLimitFromVolumeQuery_Id;
        noOfColumns = GETALLMUISCARTISTSWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
        artistDataSavedList.clear();  // added by sungha.choi 2013.07.28
        queryDataList = &artistDataSavedList;  // added by sungha.choi 2013.07.28
    }
    else if(!query.compare(gGetAllMusicComposersWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllMusicComposersWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit)))
    {
        m_VolumeName_GetAllMusicComposersWithLimitFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicComposersWithLimitFromVolumeQuery_Key);
        queryId = eGetAllMusicComposersWithLimitFromVolumeQuery_Id;
        noOfColumns = GETALLMUSICCOMPOSERSWITHLIMITFORMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if(!query.compare(gGetAllMusicGenresWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllMusicGenresWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit)))
    {
        m_VolumeName_GetAllMusicGenresWithLimitFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllMusicGenresWithLimitFromVolumeQuery_Key);
        queryId = eGetAllMusicGenresWithLimitFromVolumeQuery_Id;
        noOfColumns = GETALLMUSICGENRESWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
    }
    else if( //modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit))
        || !query.compare(gGetAllAlbumsWithCoverArtWithLimitFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume)
            .arg(m_ExtendOrdering).arg(m_Query_Offset).arg(m_Query_Limit))
        )
    {
        m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume.clear();
//        queryId = MAP_QUERY_IDS.value(gGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Key);
        queryId = eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id;
        noOfColumns = GETALLALBUMSWITHCOVERARTWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
        albumDataSavedList.clear();  // added by sungha.choi 2013.07.28
        queryDataList = &albumDataSavedList;  // added by sungha.choi 2013.07.28
    }
    else
    {
        LOG_CRITICAL << "Mapping ERROR Query" << query << LOG_ENDL;
        LOG_CRITICAL << "Mapping ERROR Query  Number" << queryId << LOG_ENDL;

        HandleError(eQueryMappingError);
        if(result != NULL)
        {
            delete result;
            result = NULL;
        }
        return;
    }

    LOG_HIGH << "queryId  = "<< queryId << LOG_ENDL;
    m_FirstTime = true;
    if(result->hasError())
    {
        LOG_CRITICAL <<"Error In Query :"<< query << LOG_ENDL;
        HandleError(eErrorInQuery);
        QSparqlError queryError = result->lastError();
        LOG_CRITICAL <<"Query Error type = "<<queryError.type()
               << "Query Error Message = "<<queryError.message() << LOG_ENDL;
        if(result != NULL)
        {
            delete result;
            result = NULL;
        }
        return;
    }

    // retrieving data from QSparqlResult
    while (result->next())
    {
        QStringList columnList;
        for (int i = 0; i < noOfColumns; i++)
        {
            QString value = result->value(i).toString();
            if (value.startsWith(QLatin1String("file://"))) {
                value = QLatin1String("file://") + QUrl::fromEncoded(value.toLatin1()).toLocalFile();
            }

            columnList.append(value);
        }
        queryDataList->append(columnList);
    }

    //{added by jaehwan.lee 2014.2.12 to release memory of sparql query result
    if (result)
    {
        delete result;
        result = NULL;
    }
    //}added by jaehwan.lee

    // after data fetching emiting a signal to notify CTrackerAbstractor
    emit DataLimitFetched(*queryDataList, queryId, m_Query_Offset);
}
// } added by sungha.choi 2013.07.23

/******************************************************************************

// Function Name                                                Parameters

// CMediaDataProvider::ModifyDataForImages      dataList (IN) - data list containing information
                                                                        about filename, url and title
// Explanation

// This function will remove any subfolder files information from the datalist
//

// History

// 22-Oct-2011      Vishal Jain                created for modifying data list
*****************************************************************************/
void CMediaDataProvider::ModifyDataForImages(QVector<QStringList>& dataList)
{

    QString folderName = m_FolderName_ImagesByFolder;

    // Replace + from folder name which would have been replaced by [+]
    folderName.replace(gPlusSignCorrected, gPlusSign);
    folderName.replace(gLBracketSignCorrected, gLBracketSign);
    folderName.replace(gRBracketSignCorrected, gRBracketSign);
    folderName.replace(gDollarSignCorrected, gDollarSign);
    folderName = QUrl::fromPercentEncoding(folderName.toUtf8());

    folderName.append("/");

    QVector<QStringList> modifiedDataList;

    // Contains two values of file and url
    QString file;
    QString url;

    QStringList singleList;

    for(int index = 0; index < dataList.count();index++)
    {
        file.clear();
        url.clear();

        file = dataList[index][0];
        url = dataList[index][1];

        QString value(url);
        value= value.remove(folderName);

        LOG_TRACE << value << LOG_ENDL;

        // If there is any backslash in the value, it means some sub folder value
        // is present in the url, fromPercentEncodingso dont add that
        if(value.contains("/") == 0)
        {
            singleList.append(file);
            singleList.append(url);
        }
        if(singleList.count() > 0)
        {
            modifiedDataList.append(singleList);
        }

        singleList.clear();
    }
    dataList.clear();
    dataList = modifiedDataList;
}

/******************************************************************************

// Function Name                                                Parameters

// CMediaDataProvider::ModifyDataForVideos      dataList (IN) - data list containing information
                                                                        about filename, url and title
//

// Explanation

// This function will remove any subfolder files information from the datalist
//
// History

// 22-Oct-2011      Vishal Jain                created for modifying data list
*****************************************************************************/
void CMediaDataProvider::ModifyDataForVideos(QVector<QStringList>& dataList)
{
    QVector<QStringList> modifiedDataList;

    QString folderName = m_FolderName_VideosByFolder;

    // Replace + from folder name which would have been replaced by [+]
    folderName.replace(gPlusSignCorrected, gPlusSign);
    folderName.replace(gLBracketSignCorrected, gLBracketSign);
    folderName.replace(gRBracketSignCorrected, gRBracketSign);
    folderName.replace(gDollarSignCorrected, gDollarSign);

    folderName = QUrl::fromPercentEncoding(folderName.toUtf8());

    folderName.append("/");

    //Contains file, url and title
    QString file;
    QString url;
    QString title;

    QStringList singleList;

    for(int index = 0; index < dataList.count();index++)
    {
        file.clear();
        url.clear();
        title.clear();

        file = dataList[index][0];
        url = dataList[index][1];
        title = dataList[index][2];

        QString value(url);

        LOG_TRACE << value << LOG_ENDL;
        value= value.remove(folderName);
        LOG_TRACE << "after removig folder name:: " << value << LOG_ENDL;

        // If there is any backslash in the value, it means some sub folder value
        // is present in the url, so dont add that
        if(value.contains("/") == 0)
        {
            singleList.append(file);
            singleList.append(url);
            singleList.append(title);
        }
        if(singleList.count() > 0)
        {
            modifiedDataList.append(singleList);
        }
        singleList.clear();
    }
    dataList.clear();
    dataList = modifiedDataList;
}




/*
 *******************************************************************
 * Function		: CMediaDataProvider::ModifyFileName
 *
 * Description 	: Modify the file name for Tracker

 * Parameters 	: originalFileName(IN) - file name

 * Returns 		: None
 *
 *******************************************************************
 */
//QString CMediaDataProvider::ModifyFileName(QString originalFileName)
//{

//    QString fileName;
//    fileName.clear();

//    if(originalFileName.length() == 0)
//        return fileName;

//    //Remove "file://" string from the path sent by tracker. Gracenote does not
//    //recognize path in such format
//    fileName = originalFileName.mid(7, originalFileName.length());

//    // Modify all the paramters like [,] etc changed by tracker to actual character

//    fileName.clear();
//    fileName.append(QUrl::fromPercentEncoding(fileName.toUtf8()));

//    return fileName;
//}

/******************************************************************************

// Function Name                                                Parameters

// CMediaDataProvider::GetAllSongsbyPlaylist                   Bool (OUT), playlistCriteria (IN) - keyword to search

//

// Explanation

// This method handles the query for retrieving music info from the Playlist

// It connects signal/slot for the query completion

//

// History

// 12-Oct-2011      Divya DSouza                created for retrieving playlist data using playlistCriteria
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsbyPlaylist(QString playlistCriteria,enum ETrackerAbstarctor_VolumeType  volumeType)
{

    bool ret=false;

    m_VolumeType = volumeType;
    QString queryString;
    m_playlist_Getallsongsbyplaylist.clear();
    m_playlist_Getallsongsbyplaylist.append(playlistCriteria);

    m_playlist_TypeName.clear();
    


#if(0)
    GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsbyPlaylist);

    QSparqlQuery QueryObject(gGetAllSongsbyPlaylistQuery.arg(m_playlist_Getallsongsbyplaylist));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsbyPlaylist);
#endif

    switch(m_VolumeType)
    {

    case eUSB:
    case eJukeBox:
        ret=true;
        m_playlist_TypeName.append(gGracenotePlaylistTypeName);
        queryString=gGetAllSongsbyPlaylistQuery.arg(m_playlist_Getallsongsbyplaylist).arg(m_playlist_TypeName);
        break;

    case eIPod:
        ret=true;
        m_playlist_TypeName.append(gIpodPlaylistTypeName);
        queryString=gGetAllSongsbyPlaylistQuery.arg(m_playlist_Getallsongsbyplaylist).arg(m_playlist_TypeName);
        break;

    default:
        break;


    }

    if(ret==true)
    {
        QSparqlQuery QueryObject;
        QueryObject.setQuery(queryString);

        // executing query
        exec(&QueryObject);
    }
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

// Function Name                                                Parameters

// CMediaDataProvider::GetMostPlayedMusicSongs      GNPlaylistResults (OUT), playlistCriteria (IN) - keyword to search

//

// Explanation

// This method handles the query for retrieving music info from the Playlist

// It connects signal/slot for the query completion

//

// History

// 12-Oct-2011      Divya DSouza                created for retrieving playlist data using playlistCriteria
*****************************************************************************/

bool CMediaDataProvider::GetlistofPlaylists(enum ETrackerAbstarctor_VolumeType  volumeType)
{

    bool ret=false;

    m_VolumeType = volumeType;
    QString queryString;

    //GetVolumeQueryToAppend(volumeType, m_VolumeName_GetlistofPlaylists);
    //QueryObject.bindValue("FILTER",m_VolumeName_GetlistofPlaylists);

    switch(m_VolumeType)
    {

    case eUSB:
        break;

    case eJukeBox:
        break;

    case eIPod:
        ret=true;
        queryString=gGetlistofPlaylistsQuery;

        break;


    default:
        break;



    }

    if(ret==true)
    {
        QSparqlQuery QueryObject;
        QueryObject.setQuery(queryString);
        // executing query
        exec(&QueryObject);
    }
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

// Function Name                                                Parameters

// CMediaDataProvider::GetMusicPlaylistByKeyword      ETrackerAbstarctor_VolumeType (IN), Keyword (IN) - keyword to search

//

// Explanation

// This method handles the query for retrieving music info from the Playlist

// It connects signal/slot for the query completion

//

// History

// 12-Oct-2011      Divya DSouza                created for retrieving playlist data using playlistCriteria
*****************************************************************************/

bool CMediaDataProvider::GetMusicPlaylistByKeyword(enum ETrackerAbstarctor_VolumeType  volumeType,QString Keyword)
{
    bool ret=false;


    QString queryString;

    m_VolumeType = volumeType;
    m_Keyword_MusicPlaylistByKeyword = Keyword;

    //GetVolumeQueryToAppend(volumeType, m_VolumeName_GetMusicPlaylistByKeyword);
    //QueryObject.bindValue("FILTER",m_VolumeName_GetMusicPlaylistByKeyword);

    switch(m_VolumeType)
    {

    case eUSB:
    case eJukeBox:
        ret=true;
        m_playlist_TypeName.append(gGracenotePlaylistTypeName);
        queryString = gGetMusicPlaylistByKeywordQuery.arg(Keyword).arg(m_playlist_TypeName);
        break;

    case eIPod:
    case eIPOD_FRONT:
    case eIPOD_REAR:
        ret=true;
        m_playlist_TypeName.append(gIpodPlaylistTypeName);
        queryString = gGetMusicPlaylistByKeywordQuery.arg(Keyword).arg(m_playlist_TypeName);
        break;


    default:
        break;


    }
    
    if(ret==true)
    {
        QSparqlQuery QueryObject;
        QueryObject.setQuery(queryString);
        // executing query
        exec(&QueryObject);
    }
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

// Function Name                                                Parameters

// CMediaDataProvider::GetMostPlayedMusicSongs      GNPlaylistResults (OUT), playlistCriteria (IN) - keyword to search

//

// Explanation

// This method handles the query for retrieving music info from the Playlist

// It connects signal/slot for the query completion

//

// History

// 12-Oct-2011      Divya DSouza                created for retrieving playlist data using playlistCriteria
*****************************************************************************/
bool CMediaDataProvider::GetMostPlayedMusicSongs(enum ETrackerAbstarctor_VolumeType  volumeType)
{

    bool ret=false;

    m_VolumeType = volumeType;

   ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetMostPlayedMusicSongs);

   if(!ret)
       return ret;

    QSparqlQuery QueryObject(gGetMostPlayedMusicSongsQuery.arg(m_VolumeName_GetMostPlayedMusicSongs));

    QString  res=gGetMostPlayedMusicSongsQuery.arg(m_VolumeName_GetMostPlayedMusicSongs);


    LOG_TRACE << "query is ->" <<res << LOG_ENDL;

    QueryObject.bindValue("FILTER",m_VolumeName_GetMostPlayedMusicSongs);

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

// Function Name                                                Parameters

// CMediaDataProvider::GetMostPlayedMusicSongs      GNPlaylistResults (OUT), playlistCriteria (IN) - keyword to search

//

// Explanation

// This method handles the query for retrieving music info from the Playlist

// It connects signal/slot for the query completion

//

// History

// 12-Oct-2011      Divya DSouza                created for retrieving playlist data using playlistCriteria
*****************************************************************************/

bool CMediaDataProvider::GetMostRecentlyPlayedSongs(enum ETrackerAbstarctor_VolumeType  volumeType)
{

    bool ret=false;

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetMostRecentlyPlayedSongs);
    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetMostRecentlyPlayedSongsQuery.arg(m_VolumeName_GetMostRecentlyPlayedSongs));
    QueryObject.bindValue("FILTER",m_VolumeName_GetMostRecentlyPlayedSongs);


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

// Function Name                                                          Parameters

// CMediaDataProvider::GetAllVideosCountFromVolume      volumeType (IN) - volume like USB, Jukebox

// Explanation

// This function will get the count of all videos present inside a specified volume
//
// History

// 19-Nov-2011      Vishal Jain                created for getting all videos count
*****************************************************************************/
bool CMediaDataProvider::GetAllVideosCountFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret = false;

    QString queryString;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeStr_GetAllVideosCountFromVolume);
    if(!ret)
        return ret;

    queryString = gGetAllVideosCountFromVolumeQuery.arg(m_VolumeStr_GetAllVideosCountFromVolume);

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

// Function Name                                                          Parameters

// CMediaDataProvider::GetAllImagesCountFromVolume      volumeType (IN) - volume like USB, Jukebox

// Explanation

// This function will get the count of all images present inside a specified volume
//
// History

// 19-Nov-2011      Vishal Jain                created for getting all images count
*****************************************************************************/
bool CMediaDataProvider::GetAllImagesCountFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret = false;

    QString queryString;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeStr_GetAllImagesCountFromVolume);
    if(!ret)
        return ret;

    queryString = gGetAllImagesCountFromVolumeQuery.arg(m_VolumeStr_GetAllImagesCountFromVolume);

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

/*
 *******************************************************************
 * Function		: CMediaDataProvider::GetMusicInfoFromCD
 *
 * Description 	: Get information about CD songs

 * Parameters 	: cdTOC(IN) - CD TOC

 * Returns 		: None
 *
 *******************************************************************
 */
bool CMediaDataProvider::GetMusicInfoFromCD(QString cdTOC)
{
    LOG_TRACE << "GetMusicInfoFromCD Enter" << cdTOC << LOG_ENDL;

    QString musicInfoFromCD;
    musicInfoFromCD = QString("select Title, Album, Artist, Genre, TrackNum, TrackCount, AlbumArtist, Composer From CDTable where TOC = \"%1\" ").arg(cdTOC);

    SendCDData(musicInfoFromCD, 8, eGetMusicFromCDQuery_Id);

    return true;
}
//{ added by jaehwan 2014.01.08 for pinyin sorting
void CMediaDataProvider::LoadPinyinTable()
{
    LOG_TRACE << "----------------------------------------------------------"<<LOG_ENDL;
    LOG_TRACE <<"Enter" << LOG_ENDL;
    LOG_TRACE << "----------------------------------------------------------"<<LOG_ENDL;

    bool  ret=false ;

    QString queryString = "select id, substr(pinyin, 1, 1) from dictionary";
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", "PINYIN");
    database.setDatabaseName("/app/share/TrackerAbstractor/media_pinyin.db");

    if(database.open())
    {
        QSqlQuery* query = new QSqlQuery(queryString,database);
        ret = query->exec();
        LOG_TRACE << " query: " << query->lastQuery()<< LOG_ENDL;

        if (query->lastError().isValid()) {
            LOG_TRACE <<"LoadPinyinTable() last error: " << query->lastError().text() << LOG_ENDL;
        }

        if (ret)
        {
            while(query->next())
            {
                int id = query->value(0).toInt();
                id = id - 0x4E00; // 0x4E00 ~ 0x9FFF

                if (id < 0 || id >= PINYIN_TABLE_SIZE)
                {
                    LOG_TRACE <<"invalide id : %d" << id <<LOG_ENDL;
                    continue;
                }
                if(query->value(1).isNull())
                {
                    pinyinTable[id] = wchar_t(NULL);
                }
                else
                {
                    QString pinyin = query->value(1).toString();
                    //LOG_TRACE <<  "id = " << id << " value = " << pinyin.at(0) << LOG_ENDL;
                    pinyinTable[id] = pinyin.at(0).unicode();
                }
            }
        }
        else
        {
            LOG_TRACE <<"PINYIN Query Error : "<< query->lastError().text() << LOG_ENDL;
        }
        delete query;
    }
    database.close();
    database.removeDatabase("PINYIN");
    LOG_TRACE << "----------------------------------------------------------"<<LOG_ENDL;
    LOG_TRACE << "Exit" << LOG_ENDL;
    LOG_TRACE << "----------------------------------------------------------"<<LOG_ENDL;
}

/*
 *******************************************************************
 * Function		: CMediaDataProvider::GetMusicInfoFromCD
 *
 * Description 	: Get information about CD songs

 * Parameters 	: cdTOC(IN) - CD TOC
                          trackNum(IN) - Tracker number
 * Returns 		: None
 *
 *******************************************************************
 */
bool CMediaDataProvider::GetMusicMetaDataInfoFromCD(QString cdTOC, QString trackNum)
{
    QString musicMetaDataInfoFromCD;
    musicMetaDataInfoFromCD = QString("select Title, Album, Artist, Genre, Composer From CDTable where TOC = \"%1\" and TrackNum = \"%2\" ").arg(cdTOC, trackNum);

    qDebug() << musicMetaDataInfoFromCD;
    SendCDData(musicMetaDataInfoFromCD, 5, eGetMusicMetaDataFromCDQuery_Id);

    return true;
}

/*
 *******************************************************************
 * Function		: CMediaDataProvider::GetMusicTracksBetweenIndexFromIpod
 *
 * Description 	: Get information about MusicTracks

 * Parameters 	: int(IN) - startIndex, int(IN) - endIndex

 * Returns 		: bool
 *
 *******************************************************************
 */
bool CMediaDataProvider::GetMusicTracksBetweenIndexFromIpod(int startIndex, int endIndex)
{
    m_StartIndex = startIndex;
    m_EndIndex = endIndex;
    QString queryString;
    queryString = gGetMusicDetailFromIpodOnIndexBasisQuery.arg(startIndex).arg(endIndex);

    LOG_TRACE << "IPOD music track query for between index is : " << queryString << LOG_ENDL;

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

    return ret;
}

void CMediaDataProvider::HandleError(int nError)
{
    LOG_CRITICAL << " the error is = " << nError << LOG_ENDL;
    emit Error(nError);
}

/*
 *******************************************************************
 * Function	: CMediaDataProvider::GetImagesWithSubFolder
 *
 * Description 	: Get image information in sub folders also

 * Parameters 	: QString - folder name

 * Returns 	: bool
 *
 *******************************************************************
 */
bool CMediaDataProvider::GetImagesWithSubFolder(QString folderName)
{

    bool ret=false;
    if(folderName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    folderName = QUrl::toPercentEncoding(folderName, ":/()~!@$&_-',.=+");

    // + sign should be modified later, else percent encoding API will change
    // the [] to %5B and %5D which should not happen
    folderName = folderName.replace(gPlusSign, gPlusSignCorrected);
    folderName = folderName.replace(gLBracketSign, gLBracketSignCorrected);
    folderName = folderName.replace(gRBracketSign, gRBracketSignCorrected);
    folderName = folderName.replace(gDollarSign, gDollarSignCorrected);

    m_FolderName_ImagesWithSubFolder = folderName;

    QString queryString;

    // setting query string
    queryString = gGetImagesBySubFolderQuery.arg(folderName);

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

/*
 *******************************************************************
 * Function	: CMediaDataProvider::GetVideosWithSubFolder
 *
 * Description 	: Get videos information in sub folders also

 * Parameters 	: QString - folder name

 * Returns 	: bool
 *
 *******************************************************************
 */
bool CMediaDataProvider::GetVideosWithSubFolder(QString folderName)
{

    bool ret=false;
    if(folderName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    LOG_TRACE <<"GetVideosWithSubFolder" << LOG_ENDL;

    LOG_TRACE << "In GetVideosWithSubFolder  FilePath :" << folderName << LOG_ENDL;

    folderName = QUrl::toPercentEncoding(folderName, ":/()~!@$&_-',.=+");

    // + sign should be modified later, else percent encoding API will change
    // the [] to %5B and %5D which should not happen
    folderName = folderName.replace(gPlusSign, gPlusSignCorrected);
    folderName = folderName.replace(gLBracketSign, gLBracketSignCorrected);
    folderName = folderName.replace(gRBracketSign, gRBracketSignCorrected);
    folderName = folderName.replace(gDollarSign, gDollarSignCorrected);

    m_FolderName_VideosWithSubFolder = folderName;

    LOG_TRACE << "In m_FolderName_VideosWithSubFolder :" << m_FolderName_VideosWithSubFolder << LOG_ENDL;

    QString queryString;

    // setting query string
    queryString = gGetVideosBySubFolderQuery.arg(folderName);

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

// Function Name                                            Parameters

// CMediaDataProvider::GetAllAlbumsWithCoverArtFromVolume        bool (OUT), enum ETrackerAbstarctor_VolumeType (IN) - volume to search

//

// Explanation

// This method handles the query for searching all albums from given volume

// It connects signal/slot for the query completion

//

// History

// 6-Jan-2012      Vishal Jain          created for seaching all albums from given volume
*****************************************************************************/
bool CMediaDataProvider::GetAllAlbumsWithCoverArtFromVolume(enum ETrackerAbstarctor_VolumeType  volumeType)
{
     bool ret=false;

     m_VolumeNameList.clear();

     m_VolumeType = volumeType;


    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllAlbumsWithCoverArt.append(" FILTER(REGEX(?url, 'AllSongs'))");
	ret = true;
    }
    else
    {
        ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllAlbumsWithCoverArt);
    }

    if(!ret)
        return ret;

    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("album"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllAlbumsWithCoverArtFromVolumeExtOrderQuery.arg( m_VolumeName_GetAllAlbumsWithCoverArt, m_ExtendOrdering);
    }
    else 
    {
        queryString = gGetAllAlbumsWithCoverArtFromVolumeQuery.arg( m_VolumeName_GetAllAlbumsWithCoverArt);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsWithCoverArt);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	
    //}   
/*
    QSparqlQuery QueryObject(gGetAllAlbumsWithCoverArtFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArt));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsWithCoverArt);

    // executing query
    exec(&QueryObject);
*/
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

// CMediaDataProvider::GetAllSongsCountFromVolume_sync        bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get count of all songs from volume

// It connects signal/slot for the query completion

//

// History

// 10-Jan-2012     mahesh         Created to get count of all songs from volume
*****************************************************************************/

bool CMediaDataProvider::GetAllSongsCountFromVolume_sync(ETrackerAbstarctor_VolumeType volumeType, QString& Count)
{
    bool ret=false;

    m_VolumeType = volumeType;
    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsCountFromVolume);
    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllSongsCountFromVolume.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }

    if(!ret)
        return ret;
    QSparqlQuery QueryObject(gGetAllSongsCountFromVolumeQuery.arg(m_VolumeName_GetAllSongsCountFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsFromVolume);

    // executing query
    QSparqlResult* result = m_pSparql_Connection->syncExec(QueryObject);

    // if query executed successfully then connecting signal/slot for result
    if (!result->hasError())
    {
        ret = true;
        while(result->next())
        {
            Count.append(result->value(0).toString());
        }
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

// CMediaDataProvider::GetSongsCountWithMetaInfoFromVolume_sync        bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get count of all songs with MetaInfo from volume

// It connects signal/slot for the query completion

//

// History

// 10-Jan-2012      mahesh          Created to get count of all Songs with MetaInfo from volume
*****************************************************************************/
bool CMediaDataProvider::GetSongsCountWithMetaInfoFromVolume_sync(ETrackerAbstarctor_VolumeType volumeType, QString& Count)
{
    bool ret=false;

    m_VolumeType = volumeType;
    
    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetSongsCountWithMetaInfoFromVolume);
    if(volumeType==eIPod)
    {
        m_VolumeName_GetSongsCountWithMetaInfoFromVolume.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }

    if(!ret)
        return ret;
    QSparqlQuery QueryObject(gGetSongsCountWithMetaInfoFromVolumeQuery.arg(m_VolumeName_GetSongsCountWithMetaInfoFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsFromVolume);

    // executing query
    QSparqlResult* result = m_pSparql_Connection->syncExec(QueryObject);

    // if query executed successfully then connecting signal/slot for result
    if (!result->hasError())
    {
        ret = true;
        while(result->next())
        {
            Count.append(result->value(0).toString());
        }
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

// CMediaDataProvider::GetAllSongsWithoutMetaInfoFromVolume        bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to Get All Songs without MetaInfo From Volume

// It connects signal/slot for the query completion

//

// History

// 24-Jan-2012      Byungjo Hyun      Created to get all songs without MetaInfo from volume
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsWithoutMetaInfoFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    LOG_TRACE << "Inside GetAllSongsWithoutMetaInfoFromVolume" << LOG_ENDL;
    bool ret=false;
    QString CountSongsWithMetaInfo = "";
    QString CountAllSongs = "";
    QString CountSongsWithoutMetaInfo = "";
    QString CountTemp = "%1";

//    qDebug() << "DHAVN_TrackerAbstrator - GetAllSongsWithoutMetaInfoFromVolume()+";

    m_VolumeType = volumeType;

    ret = GetAllSongsCountFromVolume_sync(volumeType,CountAllSongs);
    if(!ret)
        return ret;

    ret = GetSongsCountWithMetaInfoFromVolume_sync(volumeType,CountSongsWithMetaInfo);
    if(!ret)
        return ret;
    LOG_TRACE << "Inside GetAllSongsWithoutMetaInfoFromVolume :: count1" << CountAllSongs << LOG_ENDL;
    LOG_TRACE << "Inside GetAllSongsWithoutMetaInfoFromVolume :: count1" << CountSongsWithMetaInfo << LOG_ENDL;
    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsWithoutMetaInfoFromVolume);
    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllSongsWithoutMetaInfoFromVolume.append(" . FILTER(REGEX(?url, 'AllSongs'))");
    }

    if(!ret)
        return ret;

    CountSongsWithoutMetaInfo = CountTemp.arg(CountAllSongs.toInt() -  CountSongsWithMetaInfo.toInt());

    QSparqlQuery QueryObject(gGetAllSongsWithoutMetaInfoFromVolumeQuery.arg(m_VolumeName_GetAllSongsWithoutMetaInfoFromVolume).arg("0").arg(CountSongsWithoutMetaInfo));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsFromVolume);

    m_Query_Offset = 0;
    m_Query_Limit = CountSongsWithoutMetaInfo.toInt();
	
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

 //   qDebug() << "DHAVN_TrackerAbstrator - GetAllSongsWithoutMetaInfoFromVolume()-";

    return ret;
}

/**
  * Executes the query for verify whether without metadata songs are present or not
  */
/******************************************************************************

// Function Name                                            Parameters

// CMediaDataProvider::IsThereAnySongWithoutAlbumFromVolume        bool (OUT), enum ETrackerAbstarctor_VolumeType (IN) - volume to search

//

// Explanation

// This method handles the query to verify whether without metadata songs are present or not From Volume

// It connects signal/slot for the query completion

//

// History

// 25-Feb-2012      Chetan Nanda          verify whether without metadata songs are present or not
*****************************************************************************/
bool CMediaDataProvider::IsThereAnySongWithoutAlbumFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    LOG_TRACE << "Inside IsThereAnySongWithoutAlbumFromVolume" << LOG_ENDL;
    bool ret=false;
    QString CountSongsWithMetaInfo = "";
    QString CountAllSongs = "";
    QString CountSongsWithoutMetaInfo = "";
    QString CountTemp = "%1";
    QVector<QStringList> queryDataList;
    QStringList songsPresent;

    m_VolumeType = volumeType;

    ret = GetAllSongsCountFromVolume_sync(volumeType,CountAllSongs);
    if(!ret)
        return ret;

    ret = GetSongsCountWithMetaInfoFromVolume_sync(volumeType,CountSongsWithMetaInfo);
    if(!ret)
        return ret;


    CountSongsWithoutMetaInfo = CountTemp.arg(CountAllSongs.toInt() -  CountSongsWithMetaInfo.toInt());

    if(CountSongsWithoutMetaInfo.toInt() > 0)
        songsPresent.append("1");
    else
        songsPresent.append("0");

    queryDataList.append(songsPresent);
    emit DataFetched(queryDataList, eIsThereAnySongWithoutAlbumFromVolumeQuery_Id);

    return ret;
}

/******************************************************************************

// Function Name                                                Parameters

// CMediaDataProvider::GetAllMusicByKeywordFromVolume      bool (OUT), QString (IN) - keyword to search
                                                                                            ETrackerAbstarctor_VolumeType (IN) volume type
//

// Explanation

// This method handles the query for retrieving music info using given keyword from a specified volume

// It connects signal/slot for the query completion

//

// History

// 27-Feb-2012      Chetan Nanda                created for retrieving music info using given keyword from a specified volume
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicByKeywordFromVolume(QString keyword, ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret=false;
    if(keyword.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    QTextCodec *codec2 = QTextCodec::codecForName("EUC-KR");
    QString utfStr = codec2->fromUnicode(keyword);

    LOG_TRACE << "GetAllMusicByKeywordFromVolume::first method::" << keyword << LOG_ENDL;
    LOG_TRACE << "GetAllMusicByKeywordFromVolume::second method::" << utfStr << LOG_ENDL;

    m_Keyword_GetAllMusicByKeywordFromVolume = keyword;
    m_Keyword1_GetAllMusicByKeywordFromVolume = utfStr;
    QString queryString;

    switch (volumeType)
    {
    case eUSB:
        m_VolumeName_GetAllMusicByKeywordFromVolume.clear();
        m_VolumeName_GetAllMusicByKeywordFromVolume.append(GetUSBMountPath()); // modified by eugene.seo 2013.05.13
        break;
    case eJukeBox:
        m_VolumeName_GetAllMusicByKeywordFromVolume.clear();
        m_VolumeName_GetAllMusicByKeywordFromVolume.append(gVolumePath_JukeBox);
        break;
    case eIPod:
        m_VolumeName_GetAllMusicByKeywordFromVolume.clear();
        m_VolumeName_GetAllMusicByKeywordFromVolume.append(gVolumePath_IPod);
        break;
    case eDisc:
        m_VolumeName_GetAllMusicByKeywordFromVolume.clear();
        m_VolumeName_GetAllMusicByKeywordFromVolume.append(gVolumePath_DISC);
        break;
    default:
        break;

    }
    if(m_VolumeName_GetAllMusicByKeywordFromVolume.isEmpty())
    {
        HandleError(eEmptyString);
        return ret;
    }

    // setting query string
    queryString = gGetAllMusicByKeywordFromVolumeQuery.arg(keyword, utfStr,
                                                                                m_VolumeName_GetAllMusicByKeywordFromVolume);

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

// Function Name                                                Parameters

// CMediaDataProvider::RemoveAllMediaData(ETrackerAbstarctor_VolumeType volumeType)
//

// Explanation

//This method removes all media data from specifed voulme in store

//

// History

// 27-Mar-2012     mahesh kumar                removes all media data from specifed voulme in store
*****************************************************************************/
bool CMediaDataProvider::RemoveAllMediaData(ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret=false;

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_RemoveMediaData);

    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gRemoveAllMediaDataFromVolumeQuery.arg(m_VolumeName_RemoveMediaData));
    QueryObject.bindValue("FILTER",m_VolumeName_RemoveMediaData);
    QueryObject.setType(QSparqlQuery::DeleteStatement);

    //    // executing query
    exec(&QueryObject);

    // if query executed successfully then connecting signal/slot for result

    if (!m_pQueryResult->hasError())
    {
       LOG_TRACE << "Query executed :" << m_pQueryResult->query() << LOG_ENDL;
        m_pQueryResult->waitForFinished();
        ret= m_pQueryResult->isFinished();
        LOG_TRACE << "Remove All media Data Successfully" << LOG_ENDL;
        ret=true;
    }
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }

    return ret;
}


#if 0
bool CMediaDataProvider::setDeviceId(ETrackerAbstarctor_VolumeType volumeType,QString DeviceId)
{
        m_DeviceId.insert(volumeType,DeviceId);
//        LOG_TRACE  << "Size of M_Device_ID:" << m_DeviceId.size() << LOG_ENDL;
        LOG_TRACE << "Device ID :" << DeviceId << "volumeType" << volumeType << LOG_ENDL;
        //qDebug() << "Device ID :" << DeviceId << "volumeType" << volumeType;
        LOG_TRACE << "CMediaDataProvider::setDeviceId DeviceID:" << m_DeviceId.at(volumeType) << "volumeType:" << volumeType << LOG_ENDL;
         return(setDeviceURL(volumeType,m_DeviceId.at(volumeType)));

}
#endif

/******************************************************************************

// Function Name                                                Parameters

// CMediaDataProvider::setDeviceURL(ETrackerAbstarctor_VolumeType volumeType,QString DeviceId,QString MountPath)
//

// Explanation

//This method set the URL of the mounted mounted Device based on the volumetype

//

// History

// 27-Mar-2012     mahesh kumar    set the URL of the mounted mounted Device based on the volumetype
*****************************************************************************/

bool CMediaDataProvider::setDeviceURL(ETrackerAbstarctor_VolumeType volumeType,QString DeviceId,QString MountPath)
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
    case eUSB:
    case eUSB_FRONT:
    case eUSB_REAR:
        {
        m_DeviceURL.removeAt(volumeType);
        deviceUrl.append(FilePath_Prefix);
        deviceUrl.append(MountPath);
          //qDebug() << "CMediaDataProvider::setDeviceURL USB" << volumeType << DeviceId << deviceUrl;
        m_DeviceURL.insert(volumeType,deviceUrl);
        ret = true;
          }
        break;
        /*volumeQueryObject.setQuery(gGetDeviceUrlQuery.arg(DeviceId));
        result = m_pSparql_Connection->syncExec(volumeQueryObject);

        if (!result->hasError())
        {
            ret = true;
            LOG_TRACE << "Device Query " << result->query() << LOG_ENDL;
            while(result->next())
            {
            m_DeviceURL.insert(volumeType,result->value(0).toString());
            }
            LOG_TRACE << "CMediaDataProvider::setDeviceURL m_DeviceURL : " << m_DeviceURL.at(volumeType) << "VolumeType :"<<volumeType << LOG_ENDL;
        }
        else
        {
            HandleError(eErrorInQueryExecution);
            // incase of Query Error set default Mount path
            m_DeviceURL.insert(volumeType,gVolumeName_USB);
            ret =false;
        }*/


    case eJukeBox:
        m_DeviceURL.removeAt(volumeType);
        deviceUrl.append(FilePath_Prefix);
        deviceUrl.append(MountPath);
        m_DeviceURL.insert(volumeType,deviceUrl);
        //qDebug() << "CMediaDataProvider::setDeviceURL JukeBox" << volumeType << DeviceId << deviceUrl;
        ret = true;
        break;

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


/******************************************************************************

// Function Name                                                Parameters

// CMediaDataProvider::getDeviceURL(ETrackerAbstarctor_VolumeType volumeType,QString DeviceId,QString MountPath)
//

// Explanation

//This method Get the URL of the mounted  Device based on the volumetype

//

// History

// 27-Mar-2012     mahesh kumar    Get the URL of the mounted  Device based on the volumetype
*****************************************************************************/

QString CMediaDataProvider::getDeviceURL(ETrackerAbstarctor_VolumeType volumeType)
{
    QString DeviceUrl;

    if(m_DeviceURL.length() <= 0 || m_DeviceURL.contains(QString()))
    {
        HandleError(eNullStringParameter);
        return NULL;
    }

    DeviceUrl = QUrl::toPercentEncoding(m_DeviceURL.at(volumeType), ":/()~!@$&_-',.=+");

    if(DeviceUrl.contains("+"))
        DeviceUrl.replace(gPlusSign, gPlusSignCorrected);

    if(DeviceUrl.contains("("))
        DeviceUrl.replace(gLBracketSign, gLBracketSignCorrected);

    if(DeviceUrl.contains(")"))
        DeviceUrl.replace(gRBracketSign, gRBracketSignCorrected);

    if(DeviceUrl.contains("$"))
        DeviceUrl.replace(gDollarSign, gDollarSignCorrected);

     LOG_TRACE << "CMediaDataProvider::getDeviceURL m_DeviceURL:" << DeviceUrl << LOG_ENDL;
    return DeviceUrl;

}


/******************************************************************************

// Function Name                                            Parameters

// CMediaDataProvider::GetAllAlbumsWithUrlFromVolume        bool (OUT), enum ETrackerAbstarctor_VolumeType (IN) - volume to search

//

// Explanation

// This method handles the query for searching all albums from given volume

// It connects signal/slot for the query completion

//

// History

// 4-Apr-2012      Vishal Jain          created for seaching all albums from given volume with url
*****************************************************************************/
bool CMediaDataProvider::GetAllAlbumsWithUrlFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
     bool ret=false;

     m_VolumeNameList.clear();

     m_VolumeType = volumeType;

     LOG_TRACE << "GetAllAlbumsWithUrlFromVolume called " << LOG_ENDL;


    if(volumeType==eIPod)
    {
        m_VolumeName_GetAllAlbumsWithUrl.append(" FILTER(REGEX(?url, 'AllSongs'))");
        ret = true;
    }
    else
    {
        ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllAlbumsWithUrl);
    }

    if(!ret)
        return ret;

    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        //modified by jaehwan for ITS 210480
        if (!GetExtendOrderingQueryToAppend2(m_lang, QString("album"), QString("title"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllAlbumsWithUrlFromVolumeExtOrderQuery.arg( m_VolumeName_GetAllAlbumsWithUrl, m_ExtendOrdering);
    }
    else 
    {
        queryString = gGetAllAlbumsWithUrlFromVolumeQuery.arg( m_VolumeName_GetAllAlbumsWithUrl);
    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsWithUrl);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	
    //}   
    
/*
    QSparqlQuery QueryObject(gGetAllAlbumsWithUrlFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithUrl));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsWithUrl);

    // executing query
    exec(&QueryObject);
*/
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

    LOG_TRACE << "GetAllAlbumsWithUrlFromVolume END " << LOG_ENDL;

    return ret;
}


/******************************************************************************

// Function Name                                            Parameters

// CMediaDataProvider::GetAllSongsByArtistFromVolume        bool (OUT), QString (IN) - artist to search

//                                                                                        ETrackerAbstarctor_VolumeType (IN) - volume type

// Explanation

// This method handles the query for searching songs of given artist from a particular volume.

// It connects signal/slot for the query completion

//

// History

// 27-Aprl-2012      Chetan Nanda                created for seaching songs of given artist from volume.
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsByArtistFromVolume(QString artistName,
                                                                                ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    if(artistName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    artistName.replace(gDoubleQuote, QByteArray("\\\""));	// added by sangmin.seol 2013.10.16 ITS_0189462 replace " to \" for sparql query

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsByArtistFromVolume);

    if(!ret)
        return ret;

    m_ArtistName_GetAllSongsByArtistFromVolume = artistName;

   // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("title"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllSongsByArtistFromVolumeExtOrderQuery.arg(artistName, m_VolumeName_GetAllSongsByArtistFromVolume, m_ExtendOrdering);
    }
    else 
    {
        queryString = gGetAllSongsByArtistFromVolumeQuery.arg(artistName, m_VolumeName_GetAllSongsByArtistFromVolume);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	
    //}

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

// { added eugene.seo 2013.02.13
bool CMediaDataProvider::GetAllSongsByGenreFromVolume(QString genreName, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    genreName.replace(gDoubleQuote, QByteArray("\\\""));	// added by sangmin.seol 2013.10.16 ITS_0189462 replace " to \" for sparql query

    if(genreName.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsByGenreFromVolume);

    if(!ret)
        return ret;

    m_GenreName_GetAllSongsByGenreFromVolume = genreName;


    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("title"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllSongsByGenreFromVolumeExtOrderQuery.arg(genreName, m_VolumeName_GetAllSongsByGenreFromVolume, m_ExtendOrdering);
    }
    else 
    {
        queryString = gGetAllSongsByGenreFromVolumeQuery.arg(genreName, m_VolumeName_GetAllSongsByGenreFromVolume);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	
    //}
/*
    QString queryString;

    // setting query string
    queryString = gGetAllSongsByGenreFromVolumeQuery.arg(genreName, m_VolumeName_GetAllSongsByGenreFromVolume);

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

    // executing query
    exec(&QueryObject);
*/
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
// } added eugene.seo 2013.02.13

/******************************************************************************

// Function Name                                    		Parameters

// CMediaDataProvider::GetAllSongsCountFromVolume	            bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get count of songs from volume

// It connects signal/slot for the query completion

//

// History

// 2-April-2012     Byungjo Hyun   Created to get count of songs from volume
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsCountFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret=false;

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsCountFromVolume);

    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllSongsCountFromVolumeQuery.arg(m_VolumeName_GetAllSongsCountFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsFromVolume);

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

// Function Name                                    		Parameters

// CMediaDataProvider::GetAllSongsWithTitleCountFromVolume	bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get count of all songs with title from volume

// It connects signal/slot for the query completion

//

// History

// 2-Mar-2012     Byungjo Hyun   Created to get count of all songs with title from volume
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsWithTitleCountFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret=false;

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsWithTitleCountFromVolume);
    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllSongsWithTitleCountFromVolumeQuery.arg(m_VolumeName_GetAllSongsWithTitleCountFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsCountFromVolume);

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

// Function Name                                    Parameters

// CMediaDataProvider::GetAllAlbumsCountFromVolume	bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get count of all albums from volume

// It connects signal/slot for the query completion

//

// History

// 2-Mar-2012     Byungjo Hyun   Created to get count of all albums from volume
*****************************************************************************/
bool CMediaDataProvider::GetAllAlbumsCountFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllAlbumsCountFromVolume);
    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllAlbumsCountFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsCountFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsCountFromVolume);

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

// Function Name                                   			Parameters

// CMediaDataProvider::GetAllMusicArtistsCountFromVolume	bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get count of all artists from volume

// It connects signal/slot for the query completion

//

// History

// 2-Mar-2012     Byungjo Hyun   Created to get count of all artists from volume
*****************************************************************************/

bool CMediaDataProvider::GetAllMusicArtistsCountFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllMusicArtistsCountFromVolume);
    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllMusicArtistsCountFromVolumeQuery.arg(m_VolumeName_GetAllMusicArtistsCountFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllMusicArtistsCountFromVolume);

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

// Function Name                                   			Parameters

// CMediaDataProvider::GetAllMusicArtistsCountFromVolume	bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get count of all composers from volume

// It connects signal/slot for the query completion

//

// History

// 2-Mar-2012     Byungjo Hyun   Created to get count of all composers from volume
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicComposersCountFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllMusicComposersCountFromVolume);
    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllMusicComposersCountFromVolumeQuery.arg(m_VolumeName_GetAllMusicComposersCountFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllMusicComposersCountFromVolume);

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

// Function Name                                   			Parameters

// CMediaDataProvider::GetAllMusicArtistsCountFromVolume	bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get count of all genres from volume

// It connects signal/slot for the query completion

//

// History

// 2-Mar-2012     Byungjo Hyun   Created to get count of all genres from volume
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicGenresCountFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllMusicGenresCountFromVolume);
    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllMusicGenresCountFromVolumeQuery.arg(m_VolumeName_GetAllMusicGenresCountFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllMusicGenresCountFromVolume);

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

// Function Name                                   			Parameters

// CMediaDataProvider::GetAllAlbumsWithCoverArtCountFromVolumeQuery	bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get count of all albums with corverart from volume

// It connects signal/slot for the query completion

//

// History

// 8-Mar-2012     Byungjo Hyun   Created to get count of all albums with corverart from volume
*****************************************************************************/
bool CMediaDataProvider::GetAllAlbumsWithCoverArtCountFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
	bool ret=false;

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllAlbumsWithCoverArtCountFromVolume);
    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllAlbumsWithCoverArtCountFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArtCountFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsWithCoverArtCountFromVolume);

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

// Function Name                                   			Parameters

// CMediaDataProvider::GetAllSongsWithLimitFromVolume	bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get songs of limited number from a specified volume

// It connects signal/slot for the query completion

//

// History

// 8-Mar-2012     Byungjo Hyun   Created to get songs of limited number from a specified volume
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsWithLimitFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;
    LOG_TRACE << "GetAllSongsWithLimitFromVolume! "<<LOG_ENDL; 

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsWithLimitFromVolume);
    if(!ret)
        return ret;

    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02

    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("title"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllSongsWithLimitFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllSongsWithLimitFromVolume)
            .arg(m_ExtendOrdering).arg(m_Query_Offset).arg(m_Query_Limit);
    }
    else 
    {
        queryString = gGetAllSongsWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllSongsWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsWithLimitFromVolume);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO <<  "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	

    //}

    // if query executed successfully then connecting signal/slot for result

    if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnLimitFinished()));
    }
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }

    return ret;
}

// { added by sungha.choi 2013.07.23
bool CMediaDataProvider::GetAllSongsWithLimitCacheFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    LOG_TRACE << "GetAllSongsWithLimitFromVolume! "<<LOG_ENDL; 

    if(m_cacheSuccess) {    // modified by sungha.choi 2013.07.30
        LOG_TRACE << "Cache Success : songDataList" << LOG_ENDL;
        m_cacheSuccess = false;
        emit DataLimitFetched(songDataSavedList, eGetAllSongsWithLimitFromVolumeQuery_Id, m_Query_Offset);
        return true;
    }
    else
    {
        bool ret=false;

        m_VolumeType = volumeType;

        ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsWithLimitFromVolume);
        if(!ret)
            return ret;

        
        // {modify by jawhean.lee for audio list sorting issue. 13.10. 02

        QString queryString;
        if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
        {
            if (!GetExtendOrderingQueryToAppend(m_lang, QString("title"), m_ExtendOrdering) )
                return false; 
            LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL;     

            queryString = gGetAllSongsWithLimitFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllSongsWithLimitFromVolume).arg(m_ExtendOrdering).arg(m_Query_Offset).arg(m_Query_Limit);
        }
        else 
        {
            queryString = gGetAllSongsWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllSongsWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit);

        }
        
        QSparqlQuery QueryObject;
        QueryObject.setQuery(queryString);
        QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsWithLimitFromVolume);

        LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
        LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
        LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
        exec(&QueryObject);	

        //}

        
        // if query executed successfully then connecting signal/slot for result

        if (!m_pQueryResult->hasError())
        {
            ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnLimitCacheFinished()));
        }
        else
        {
            HandleError(eErrorInQueryExecution);
            return ret;
        }

        return ret;
    }
}
// } added by sungha.choi 2013.07.23

/******************************************************************************

// Function Name                                   			Parameters

// CMediaDataProvider::GetAllAlbumsWithLimitFromVolume	bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get albums of limited number from a specified volume

// It connects signal/slot for the query completion

//

// History

// 8-Mar-2012     Byungjo Hyun   Created to get albums of limited number from a specified volume
*****************************************************************************/
bool CMediaDataProvider::GetAllAlbumsWithLimitFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeNameList.clear();

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllAlbumsWithLimitFromVolume);
    if(!ret)
        return ret;


    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02

    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("album"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllAlbumsWithLimitFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllAlbumsWithLimitFromVolume)
            .arg(m_ExtendOrdering).arg(m_Query_Offset).arg(m_Query_Limit);
    }
    else 
    {
        queryString = gGetAllAlbumsWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsWithLimitFromVolume);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	

    //}
/*
    QSparqlQuery QueryObject(gGetAllAlbumsWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsWithLimitFromVolume);

    //    // executing query
    exec(&QueryObject);
*/
    // if query executed successfully then connecting signal/slot for result
    if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnLimitFinished()));
	}
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }

    return ret;
}
/******************************************************************************

// Function Name                                   			Parameters

// CMediaDataProvider::GetAllMusicArtistsWithLimitFromVolume	bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get artists of limited number from a specified volume

// It connects signal/slot for the query completion

//

// History

// 8-Mar-2012     Byungjo Hyun   Created to get artists of limited number from a specified volume
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicArtistsWithLimitFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeNameList.clear();

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllMusicArtistsWithLimitFromVolume);
    if(!ret)
        return ret;

    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02

    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("performer"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllMusicArtistsWithLimitFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllMusicArtistsWithLimitFromVolume)
            .arg(m_ExtendOrdering).arg(m_Query_Offset).arg(m_Query_Limit);
    }
    else 
    {
        queryString = gGetAllMusicArtistsWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllMusicArtistsWithLimitFromVolume)
            .arg(m_Query_Offset).arg(m_Query_Limit);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllMusicArtistsWithLimitFromVolume);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	

    //}

    /*
    QSparqlQuery QueryObject(gGetAllMusicArtistsWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllMusicArtistsWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllMusicArtistsWithLimitFromVolume);

    //    // executing query
    exec(&QueryObject);
    */
    // if query executed successfully then connecting signal/slot for result
    if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnLimitFinished()));
	}
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }

    return ret;
}

// { added by sungha.choi 2013.07.23
bool CMediaDataProvider::GetAllMusicArtistsWithLimitCacheFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    if(m_cacheSuccess) {    // modified by sungha.choi 2013.07.30
        LOG_TRACE << "Cache Success : artistDataList" << LOG_ENDL;
        m_cacheSuccess = false;
        emit DataLimitFetched(artistDataSavedList, eGetAllMusicArtistsWithLimitFromVolumeQuery_Id, m_Query_Offset);
        return true;
    }
    else {
        bool ret=false;

        m_VolumeNameList.clear();

        m_VolumeType = volumeType;

        ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllMusicArtistsWithLimitFromVolume);
        if(!ret)
            return ret;

        // {modify by jawhean.lee for audio list sorting issue. 13.10. 02

        QString queryString;
        if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
        {
            if (!GetExtendOrderingQueryToAppend(m_lang, QString("performer"), m_ExtendOrdering) )
                return false; 
            
            LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
            queryString = gGetAllMusicArtistsWithLimitFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllMusicArtistsWithLimitFromVolume)
                .arg(m_ExtendOrdering).arg(m_Query_Offset).arg(m_Query_Limit);
        }
        else 
        {
            queryString = gGetAllMusicArtistsWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllMusicArtistsWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit);

        }
        QSparqlQuery QueryObject;
        QueryObject.setQuery(queryString);
        QueryObject.bindValue("FILTER",m_VolumeName_GetAllMusicArtistsWithLimitFromVolume);

        LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
        LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
        LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

        exec(&QueryObject);	

        //}
/*      
        QSparqlQuery QueryObject(gGetAllMusicArtistsWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllMusicArtistsWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit));
        QueryObject.bindValue("FILTER",m_VolumeName_GetAllMusicArtistsWithLimitFromVolume);

        //    // executing query
        exec(&QueryObject);
*/
        // if query executed successfully then connecting signal/slot for result
        if (!m_pQueryResult->hasError())
        {
            ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnLimitCacheFinished()));
            }
        else
        {
            HandleError(eErrorInQueryExecution);
            return ret;
        }

        return ret;
    }
}
// } added by sungha.choi 2013.07.23

/******************************************************************************

// Function Name                                   			Parameters

// CMediaDataProvider::GetAllMusicComposersWithLimitFromVolume	bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get composers of limited number from a specified volume

// It connects signal/slot for the query completion

//

// History

// 8-Mar-2012     Byungjo Hyun   Created to get composers of limited number from a specified volume
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicComposersWithLimitFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeNameList.clear();

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllMusicComposersWithLimitFromVolume);

    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllMusicComposersWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllMusicComposersWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllMusicComposersWithLimitFromVolume);

    //    // executing query
    exec(&QueryObject);

    // if query executed successfully then connecting signal/slot for result
    if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnLimitFinished()));
	}
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }

    return ret;
}
/******************************************************************************

// Function Name                                   			Parameters

// CMediaDataProvider::GetAllMusicGenresWithLimitFromVolume	bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get genres of limited number from a specified volume

// It connects signal/slot for the query completion

//

// History

// 8-Mar-2012     Byungjo Hyun   Created to get genres of limited number from a specified volume
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicGenresWithLimitFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeNameList.clear();

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllMusicGenresWithLimitFromVolume);
    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllMusicGenresWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllMusicGenresWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllMusicGenresWithLimitFromVolume);

    //    // executing query
    exec(&QueryObject);

    // if query executed successfully then connecting signal/slot for result
    if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnLimitFinished()));
    }
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }

    return ret;
}
/******************************************************************************

// Function Name                                   			Parameters

// CMediaDataProvider::GetAllAlbumsWithCoverArtWithLimitFromVolume	bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get albums with coverart of limited number from a specified volume

// It connects signal/slot for the query completion

//

// History

// 8-Mar-2012     Byungjo Hyun   Created to get albums with coverart of limited number from a specified volume
*****************************************************************************/
bool CMediaDataProvider::GetAllAlbumsWithCoverArtWithLimitFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
     bool ret=false;

     m_VolumeNameList.clear();

     m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume);
    if(!ret)
        return ret;
    
    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("album"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllAlbumsWithCoverArtWithLimitFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume)
            .arg(m_ExtendOrdering).arg(m_Query_Offset).arg(m_Query_Limit);
    }
    else 
    {
        queryString = gGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume)
            .arg(m_Query_Offset).arg(m_Query_Limit);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	
    //} 
/*
    QSparqlQuery QueryObject(gGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume);
    exec(&QueryObject);
*/
    // if query executed successfully then connecting signal/slot for result
    if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnLimitFinished()));
    }
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }

    return ret;
}

// { added by sungha.choi 2013.07.23
bool CMediaDataProvider::GetAllAlbumsWithCoverArtWithLimitCacheFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    if(m_cacheSuccess) {    // modified by sungha.choi 2013.07.30
        LOG_TRACE << "Cache Success : albumDataList" << LOG_ENDL;
        m_cacheSuccess = false;
        emit DataLimitFetched(albumDataSavedList, eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id, m_Query_Offset);
        return true;
    }
    else {
        bool ret=false;

        m_VolumeNameList.clear();

        m_VolumeType = volumeType;

        ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume);
        if(!ret)
            return ret;

        // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
        QString queryString;
        if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
        {
            if (!GetExtendOrderingQueryToAppend(m_lang, QString("album"), m_ExtendOrdering) )
                return false; 
            
            LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
            queryString = gGetAllAlbumsWithCoverArtWithLimitFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume)
                .arg(m_ExtendOrdering).arg(m_Query_Offset).arg(m_Query_Limit);
        }
        else 
        {
            queryString = gGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit);

        }
        QSparqlQuery QueryObject;
        QueryObject.setQuery(queryString);
        QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume);

        LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
        LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
        LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

        exec(&QueryObject);	
        //} 
/*
        QSparqlQuery QueryObject(gGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume).arg(m_Query_Offset).arg(m_Query_Limit));
        QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsWithCoverArtWithLimitFromVolume);

        // executing query
        exec(&QueryObject);
*/
        // if query executed successfully then connecting signal/slot for result
        if (!m_pQueryResult->hasError())
        {
            ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnLimitCacheFinished()));
        }
        else
        {
            HandleError(eErrorInQueryExecution);
            return ret;
        }

        return ret;
    }
}
// } added by sungha.choi 2013.07.23

/******************************************************************************

// Function Name                                   			Parameters

// CMediaDataProvider::GetAllSongsWithTitleIndexFromVolume	bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get a index from song list using given letter from a specified volume

// It connects signal/slot for the query completion

//

// History

// 8-Mar-2012     Byungjo Hyun   Created to get a index from song list using given letter from a specified volume
*****************************************************************************/
bool CMediaDataProvider::GetAllSongsWithTitleIndexFromVolume(QString letter, ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret=false;

    if(letter.isEmpty())
    {
        HandleError(eEmptyString);
        return ret;
    }
    m_VolumeType = volumeType;
	m_Letter_GetAllSongsWithTitleIndexFromVolume = letter;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsWithTitleIndexFromVolume);
    if(!ret)
        return ret;

    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("title"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllSongsWithTitleIndexFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllSongsWithTitleIndexFromVolume)
            .arg(m_ExtendOrdering);
    }
    else 
    {
        queryString = gGetAllSongsWithTitleIndexFromVolumeQuery.arg(m_VolumeName_GetAllSongsWithTitleIndexFromVolume);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsWithTitleIndexFromVolume);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	
    //} 
/*
    QSparqlQuery QueryObject(gGetAllSongsWithTitleIndexFromVolumeQuery.arg(m_VolumeName_GetAllSongsWithTitleIndexFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsWithTitleIndexFromVolume);


    //executing query
    exec(&QueryObject);
*/
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

// Function Name                                   			Parameters

// CMediaDataProvider::GetAllMusicArtistsIndexFromVolume	bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get a index from artists list using given letter from a specified volume

// It connects signal/slot for the query completion

//

// History

// 8-Mar-2012     Byungjo Hyun   Created to get a index from artists list using given letter from a specified volume
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicArtistsIndexFromVolume(QString letter, ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret=false;
	
    if(letter.isEmpty())
    {
        HandleError(eEmptyString);
        return ret;
    }
    m_VolumeType = volumeType;
	m_Letter_GetAllMusicArtistsIndexFromVolume = letter;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllMusicArtistsIndexFromVolume);
    if(!ret)
        return ret;


    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("performer"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllMusicArtistsFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllMusicArtistsIndexFromVolume)
            .arg(m_ExtendOrdering);
    }
    else 
    {
        queryString = gGetAllMusicArtistsFromVolumeQuery.arg(m_VolumeName_GetAllMusicArtistsIndexFromVolume);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllMusicArtistsIndexFromVolume);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	
    //} 

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

// Function Name                                   			Parameters

// CMediaDataProvider::GetAllMusicGenresIndexFromVolume	bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get a index from genres list using given letter from a specified volume

// It connects signal/slot for the query completion

//

// History

// 8-Mar-2012     Byungjo Hyun   Created to get a index from genres list using given letter from a specified volume
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicGenresIndexFromVolume(QString letter, ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret=false;
	
    if(letter.isEmpty())
    {
        HandleError(eEmptyString);
        return ret;
    }
    m_VolumeType = volumeType;
	m_Letter_GetAllMusicGenresIndexFromVolume = letter;
	
    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllMusicGenresIndexFromVolume);
    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllMusicGenresFromVolumeQuery.arg(m_VolumeName_GetAllMusicGenresIndexFromVolume));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllMusicGenresIndexFromVolume);


    //executing query
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

// Function Name                                   			Parameters

// CMediaDataProvider::GetAllAlbumsWithCoverArtIndexFromVolume	bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume type

//

// Explanation

// This method handles the query to get a index from albums with coverart list using given letter from a specified volume

// It connects signal/slot for the query completion

//

// History

// 8-Mar-2012     Byungjo Hyun   Created to get a index from albums with coverart list using given letter from a specified volume
*****************************************************************************/
bool CMediaDataProvider::GetAllAlbumsWithCoverArtIndexFromVolume(QString letter, ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret=false;

    if(letter.isEmpty())
    {
        HandleError(eEmptyString);
        return ret;
    }    
	m_VolumeType = volumeType;
	m_Letter_GetAllAlbumsWithCoverArtIndexFromVolume = letter;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllAlbumsWithCoverArtIndexFromVolume);
    if(!ret)
        return ret;

    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("album"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllAlbumsWithCoverArtIndexFromVolumeExtOrderQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArtIndexFromVolume)
            .arg(m_ExtendOrdering);
    }
    else 
    {
        queryString = gGetAllAlbumsWithCoverArtIndexFromVolumeQuery.arg(m_VolumeName_GetAllAlbumsWithCoverArtIndexFromVolume);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsWithCoverArtIndexFromVolume);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	
    //} 

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

// Function Name                                   			Parameters

// CMediaDataProvider::PrepareIndexDataList	void (OUT), QSparqlResult (IN) - volume type

//

// Explanation

// This method handles the query to prepare a index from the data list using given letter 

// It connects signal/slot for the query completion

//

// History

// 8-Mar-2012     Byungjo Hyun   Created to prepare a index from the data list using given letter 
*****************************************************************************/
void CMediaDataProvider::PrepareIndexDataList(void* result, QVector<QStringList> queryDataList,
                                              ETrackerAbstarctor_Query_Ids queryId,
                                              QString letter, bool isIPod)
{


    LOG_CRITICAL << "@@@=======================================================@@@" << LOG_ENDL;

    int 	nCurrentOffset 	= 0;
    int 	nLetterOffset 	= -1;
    QChar 	chLetter 		= letter.at(0);
    ushort  usLetterUnicode = chLetter.unicode();
    int		nLetterIndex	= -1;
    int		nLetterIndexTbl[NUM_UNICODE_TBL][2];

    QStringList columnList;						
    QString strOffset;
    QString strTemp = "%1";

    int resultSize;

    if(isIPod)
    {
        resultSize = ( (QSqlQuery*)result)->size();
    }
    else
    {
        resultSize = ( (QSparqlResult*)result)->size();
    }

    for(int i=0;i<NUM_UNICODE_TBL;++i)
    {
        nLetterIndexTbl[i][0] = -1;
        nLetterIndexTbl[i][1] = -1;

        if(Letter_Tbl[i] == usLetterUnicode)
            nLetterIndex = i;
    }

    if( (nLetterIndex == -1) || (resultSize == 0) )
    {
        columnList.append("0");
        queryDataList.append(columnList);
        emit DataFetched(queryDataList, queryId);
        return;
    }

    // If size is 0, we are returning, so here the results will have some "next" results
    bool continueProcessing = false;

    // Point to the first result
    if(isIPod)
    {
        continueProcessing = ( (QSqlQuery*)result)->next();
    }
    else
    {
        continueProcessing = ( (QSparqlResult*)result)->next();
    }

    do
    {

        QString strData;
        // Get the result string
        if(isIPod)
        {
            strData = ( (QSqlQuery*)result)->value(0).toString();
        }
        else
        {
            strData = ( (QSparqlResult*)result)->value(0).toString();
        }
        //{changed by junam 2013.07.25 for ITS_EU_180117
        //QChar 	chData 		= strData.at(0);
        QChar 	chData 		= strData.at(0).toUpper();
        //}changed by junam
        ushort 	usDataUnicode 	= chData.unicode();

        if( (usDataUnicode >= Letter_Unicode_Map_Tbl[nLetterIndex][0]) &&
                (usDataUnicode <= Letter_Unicode_Map_Tbl[nLetterIndex][1]) )
        {
            //int test = usDataUnicode - Letter_Unicode_Map_Tbl[nLetterIndex][0];
            nLetterOffset = nCurrentOffset;
            break;
        }
        else
        {
            // make index table for each letter
            for(int i=0;i<NUM_UNICODE_TBL;++i)
            {
                if( (usDataUnicode >= Letter_Unicode_Map_Tbl[i][0]) &&
                        (usDataUnicode <= Letter_Unicode_Map_Tbl[i][1]) )
                {
                    //int test = usDataUnicode - Letter_Unicode_Map_Tbl[i][0];
                    if(nLetterIndexTbl[i][0] == -1){
                        nLetterIndexTbl[i][0] = nCurrentOffset;	// set first index for letter
                        nLetterIndexTbl[i][1] = nCurrentOffset; // set last index for letter
                    }
                    else{
                        nLetterIndexTbl[i][1] = nCurrentOffset; // last index for letter
                    }
                }
            }
        }

        nCurrentOffset++;

        // Check if there are any more results
        if(isIPod)
        {
            continueProcessing = ( (QSqlQuery*)result)->next();
        }
        else
        {
            continueProcessing = ( (QSparqlResult*)result)->next();
        }

    } while (continueProcessing);


    bool firstCheck = false;
    bool secondCheck = false;
    QString firstLetter;
    ushort firstUn;
    int firstIndex;
    QString secondLetter;
    int secondIndex;
    ushort secondUn;
    if(nLetterOffset == -1)
    {
        // Finding closest charactor from unicode table
        for(int i=(NUM_UNICODE_TBL-1);i>=0;--i)
        {
            if( (i < (NUM_UNICODE_TBL-1)) && (i > (NUM_UNICODE_TBL-(NUM_KOREAN_TBL+2))) )
            {
                // korean letter
                if( (usLetterUnicode >= Letter_Tbl[i]) && (nLetterIndexTbl[i][0] != -1) /*&& !firstCheck*/){
                    if(!firstCheck)
                    {
                        firstLetter.clear();
                        firstUn = Letter_Tbl[i];
                        firstLetter.setNum( Letter_Tbl[i] );
                        firstIndex = nLetterIndexTbl[i][0];
                        firstCheck = true;
                    }
                    else
                    {
                        if( Letter_Tbl[i] >= firstUn )
                        {
                            firstLetter.clear();
                            firstUn = Letter_Tbl[i];
                            firstLetter.setNum( Letter_Tbl[i] );
                            firstIndex = nLetterIndexTbl[i][0];
                        }
                    }

                    //					nLetterOffset = nLetterIndexTbl[i][1];
                    //					break;
                }
                else if( (usLetterUnicode < Letter_Tbl[i]) && (nLetterIndexTbl[i][0] != -1) /*&& !secondCheck*/)
                {
                    if(!secondCheck)
                    {
                        secondLetter.clear();
                        secondUn = Letter_Tbl[i];
                        secondLetter.setNum( Letter_Tbl[i] );
                        secondIndex = nLetterIndexTbl[i][0];
                        secondCheck = true;
                    }
                    else
                    {
                        if( Letter_Tbl[i] <= secondUn )
                        {
                            secondLetter.clear();
                            secondUn = Letter_Tbl[i];
                            secondLetter.setNum( Letter_Tbl[i] );
                            secondIndex = nLetterIndexTbl[i][0];
                        }
                    }
                }
            }
            else
            {	// others letter
                if( (usLetterUnicode >= Letter_Unicode_Map_Tbl[i][0]) && (nLetterIndexTbl[i][0] != -1) /*&& !firstCheck*/ )
                {
                    if(!firstCheck)
                    {
                        firstLetter.clear();
                        firstUn = Letter_Unicode_Map_Tbl[i][0];
                        firstLetter.setNum( Letter_Unicode_Map_Tbl[i][0] );
                        firstIndex = nLetterIndexTbl[i][0];
                        firstCheck = true;
                    }
                    else
                    {
                        if( Letter_Unicode_Map_Tbl[i][0] >= firstUn )
                        {
                            firstLetter.clear();
                            firstUn = Letter_Unicode_Map_Tbl[i][0];
                            firstLetter.setNum( Letter_Unicode_Map_Tbl[i][0] );
                            firstIndex = nLetterIndexTbl[i][0];
                        }
                    }

                    //					nLetterOffset = nLetterIndexTbl[i][1];
                    //					break;
                }
                else if( (usLetterUnicode < Letter_Unicode_Map_Tbl[i][0]) && (nLetterIndexTbl[i][0] != -1) /*&& !secondCheck*/){
                    if(!secondCheck)
                    {
                        secondLetter.clear();
                        secondUn = Letter_Unicode_Map_Tbl[i][0];
                        secondLetter.setNum( Letter_Unicode_Map_Tbl[i][0] );
                        secondIndex = nLetterIndexTbl[i][0];
                        secondCheck = true;
                    }
                    else
                    {
                        if( Letter_Tbl[i] <= secondUn )
                        {
                            secondLetter.clear();
                            secondUn = Letter_Unicode_Map_Tbl[i][0];
                            secondLetter.setNum( Letter_Unicode_Map_Tbl[i][0]  );
                            secondIndex = nLetterIndexTbl[i][0];
                        }
                    }
                }
            }
        }
    }

    if(firstCheck && secondCheck)
    {
        if( (firstLetter.toInt() - letter.toInt()) <= (letter.toInt() - secondLetter.toInt()) )
            nLetterOffset = firstIndex;
        else
            nLetterOffset = secondIndex;
    }
    else if(firstCheck && !secondCheck)
        nLetterOffset = firstIndex;
    else if(!firstCheck && secondCheck)
        nLetterOffset = secondIndex;

    if(nLetterOffset == -1)
        nLetterOffset = 0;

    // Added for ITS 190700
    //LOG_TRACE << "firstIndex : " << firstIndex <<  " secondIndex : " << secondIndex << " nLetterOffset : " << nLetterOffset << " letter :" << letter << " nCurrentOffset : " << nCurrentOffset << LOG_ENDL;
    if ( isIPod && letter == "#" && nLetterOffset != nCurrentOffset)
    {
        nLetterOffset = nCurrentOffset - 1;
    }

    strOffset = strTemp.arg(nLetterOffset);
    columnList.append(strOffset);
    queryDataList.append(columnList);

    emit DataFetched(queryDataList, queryId);
}

/******************************************************************************

// Function Name                                                                               Parameters

// CMediaDataProvider::GetMultipleAlbumSongsByArtistFromVolume     bool (OUT), QStringList (IN) - album names
                                                                                                        QString (IN) - artist to search
//                                                                                        ETrackerAbstarctor_VolumeType (IN) - volume type

// Explanation

// This method handles the query for searching songs of given artist from given albums for specified volume.

// It connects signal/slot for the query completion

//

// History

// 11-May-2012      Chetan Nanda                created for seaching songs of given artist from given ablumes.
*****************************************************************************/
bool CMediaDataProvider::GetMultipleAlbumSongsByArtistFromVolume(QStringList albumNameList, QString artistName,
                                                                                                        ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret;
    QString volumeName;
    QString filter = "FILTER( (";
    QString album = "?album=";

    for (int j = 0; j < albumNameList.size(); j++)
    {
        QString Regex = " (";

        Regex.append(album);
        Regex.append(gDoubleQuote);
        Regex.append(albumNameList[j]);
        Regex.append(gDoubleQuote);
        if(j == (albumNameList.size()-1) )
            Regex.append(")");
        else
            Regex.append(") || ");
        filter.append(Regex);
    }

    filter.append(" ) && (?artist=");
    filter.append(gDoubleQuote);
    filter.append(artistName);
    filter.append(gDoubleQuote);
    filter.append(") ) .");

    ret = GetVolumeQueryToAppend(volumeType, volumeName);

    if(!ret)
        return ret;

    filter.append(volumeName);

    QSparqlQuery QueryObject(gGetMultipleAlbumSongsByArtistFromVolumeQuery.arg(filter));
    QueryObject.bindValue("FILTER",filter);
    m_Argument_GetMultipleAlbumSongsByArtistFromVolume = filter;

    // executing query
    exec(&QueryObject);

    // if query executed successfully then connecting signal/slot for result
//    bool ret=false;
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

// Function Name                                                                               Parameters

// CMediaDataProvider::GetMultipleGenreSongsFromVolume     bool (OUT), QStringList (IN) - genre List
//                                                                                          ETrackerAbstarctor_VolumeType (IN) - volume type

// Explanation

// This method handles the query for searching songs of given genre for specified volume.

// It connects signal/slot for the query completion

//

// History

// 11-May-2012      Chetan Nanda                created for seaching songs of given genre for specified volume.
*****************************************************************************/
bool CMediaDataProvider::GetMultipleGenreSongsFromVolume(QStringList genreList,
                                                                                            ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret;
    QString volumeName;
    QString filter = "FILTER( ";
    QString album = "?genre=";

    for (int j = 0; j < genreList.size(); j++)
    {
        QString Regex = " (";

        Regex.append(album);
        Regex.append(gDoubleQuote);
        genreList[j].replace(gDoubleQuote, QByteArray("\\\""));		// added by sangmin.seol 2013.10.16 ITS_0189462 replace " to \" for sparql query
        Regex.append(genreList[j]);
        Regex.append(gDoubleQuote);
        if(j == (genreList.size()-1) )
            Regex.append(")");
        else
            Regex.append(") || ");
        filter.append(Regex);
    }

    filter.append(" ) .");

    ret = GetVolumeQueryToAppend(volumeType, volumeName);

    if(!ret)
        return ret;

    filter.append(volumeName);

    QSparqlQuery QueryObject(gGetMultipleGenreSongsFromVolumeQuery.arg(filter));
    QueryObject.bindValue("FILTER",filter);
    m_Argument_GetMultipleGenreSongsFromVolume = filter;

    // executing query
    exec(&QueryObject);

    // if query executed successfully then connecting signal/slot for result
//    bool ret=false;
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

// Function Name                                                                               Parameters

// CMediaDataProvider::GetMultipleArtistSongsByGenreFromVolume     bool (OUT), QStringList (IN) - artist names
                                                                                                        QString (IN) - genre to search
//                                                                                        ETrackerAbstarctor_VolumeType (IN) - volume type

// Explanation

// This method handles the query for searching songs of given genre from given artistname for specified volume.

// It connects signal/slot for the query completion

//

// History

// 11-May-2012      Chetan Nanda                created for seaching songs of given genre from given artist names.
*****************************************************************************/
bool CMediaDataProvider::GetMultipleArtistSongsByGenreFromVolume(QStringList artistNameList, QString genre,
                                                                                                        ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret;
    QString volumeName;
    QString filter = "FILTER( (";
    QString album = "?artist=";

    for (int j = 0; j < artistNameList.size(); j++)
    {
        QString Regex = " (";

        Regex.append(album);
        Regex.append(gDoubleQuote);
        Regex.append(artistNameList[j]);
        Regex.append(gDoubleQuote);
        if(j == (artistNameList.size()-1) )
            Regex.append(")");
        else
            Regex.append(") || ");
        filter.append(Regex);
    }

    filter.append(" ) && (?genre=");
    filter.append(gDoubleQuote);
    filter.append(genre);
    filter.append(gDoubleQuote);
    filter.append(") ) .");

    ret = GetVolumeQueryToAppend(volumeType, volumeName);

    if(!ret)
        return ret;

    filter.append(volumeName);

    QSparqlQuery QueryObject(gGetMultipleArtistSongsByGenreFromVolumeQuery.arg(filter));
    QueryObject.bindValue("FILTER",filter);
    m_Argument_GetMultipleArtistSongsByGenreFromVolume = filter;

    // executing query
    exec(&QueryObject);

    // if query executed successfully then connecting signal/slot for result
//    bool ret=false;
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

// Function Name                                                                                        Parameters

// CMediaDataProvider::GetMultipleAlbumSongsByGenreArtistFromVolume     bool (OUT), QStringList (IN) - album names
                                                                                                        QString (IN) - genre to search
                                                                                                        QString (IN) - artist to search
//                                                                                        ETrackerAbstarctor_VolumeType (IN) - volume type

// Explanation

// This method handles the query for searching songs of given genre and artist from given albums for specified volume.

// It connects signal/slot for the query completion

//

// History

// 11-May-2012      Chetan Nanda                created for seaching songs of given genre and artist from given albums.
*****************************************************************************/
bool CMediaDataProvider::GetMultipleAlbumSongsByGenreArtistFromVolume(QStringList albumNameList,
                                                                                                                QString genre, QString artist,
                                                                                                    ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret;
    QString volumeName;
    QString filter = "FILTER( (";
    QString album = "?album=";

    for (int j = 0; j < albumNameList.size(); j++)
    {
        QString Regex = " (";

        Regex.append(album);
        Regex.append(gDoubleQuote);
        Regex.append(albumNameList[j]);
        Regex.append(gDoubleQuote);
        if(j == (albumNameList.size()-1) )
            Regex.append(")");
        else
            Regex.append(") || ");
        filter.append(Regex);
    }

    filter.append(" ) && (?genre=");
    filter.append(gDoubleQuote);
    filter.append(genre);
    filter.append(gDoubleQuote);
    filter.append(" ) && (?artist=");
    filter.append(gDoubleQuote);
    filter.append(artist);
    filter.append(gDoubleQuote);
    filter.append(") ) .");

    ret = GetVolumeQueryToAppend(volumeType, volumeName);

    if(!ret)
        return ret;

    filter.append(volumeName);

    QSparqlQuery QueryObject(gGetMultipleAlbumSongsByGenreArtistFromVolumeQuery.arg(filter));
    QueryObject.bindValue("FILTER",filter);
    m_Argument_GetMultipleAlbumSongsByGenreArtistFromVolume = filter;

    // executing query
    exec(&QueryObject);

    // if query executed successfully then connecting signal/slot for result
//    bool ret=false;
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

// Function Name                                                                                        Parameters

// CMediaDataProvider::GetMultipleFolderSongsQuery     bool (OUT), QStringList (IN) - folder path list

// Explanation

// This method handles the query for searching songs from given folder paths.

// It connects signal/slot for the query completion

//

// History

// 11-May-2012      Chetan Nanda                created for seaching songs from given folder paths.
*****************************************************************************/
bool CMediaDataProvider::GetMultipleFolderSongs(QStringList folderPathList)
{
    bool ret = false;

    m_Argument_GetMultipleFolderSongs = GetMusicUrlAppendedFromUrlList(folderPathList);

    QSparqlQuery QueryObject(gGetMultipleFolderSongsQuery.arg(m_Argument_GetMultipleFolderSongs));
    QueryObject.bindValue("FILTER",m_Argument_GetMultipleFolderSongs);

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

// Function Name                                                                    Parameters

// CMediaDataProvider::GetMultipleAlbumSongsFromVolume     bool (OUT), QStringList (IN) - albumList to search

//                                                                                          ETrackerAbstarctor_VolumeType - volume type

// Explanation

// This method handles the query for searching songs under respective album list

// It connects signal/slot for the query completion

//

// History

// 14-May-2011      Chetan Nanda             created for seaching songs inside particular album list
*****************************************************************************/
bool CMediaDataProvider::GetMultipleAlbumSongsFromVolume(QStringList AlbumnamesList,
                                                                    ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;
    QString filter = "FILTER(";
    QString album = "?album=";

    for (int j = 0; j < AlbumnamesList.size(); j++) {
        QString Regex = " (";

        Regex.append(album);
        Regex.append(gDoubleQuote);
        AlbumnamesList[j].replace(gDoubleQuote, QByteArray("\\\""));	// added by sangmin.seol 2013.10.16 ITS_0189462 replace " to \" for sparql query
        Regex.append(AlbumnamesList[j]);
        Regex.append(gDoubleQuote);
        if(j == (AlbumnamesList.size()-1) )
            Regex.append(")");
        else
            Regex.append(") || ");
        filter.append(Regex);
    }

    filter.append(") .");
    QString volumeName;
    ret = GetVolumeQueryToAppend(volumeType, volumeName);

    if(!ret)
        return ret;

    filter.append(volumeName);

    QSparqlQuery QueryObject(gGetMultipleAlbumSongsFromVolumeQuery.arg(filter));
    QueryObject.bindValue("FILTER",filter);
    m_Argument_GetMultipleAlbumSongsFromVolume = filter;

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

// Function Name                                                                    Parameters

// CMediaDataProvider::GetMultipleArtistSongsFromVolume        bool (OUT), QStringList (IN) - artistList to search

//                                                                                          ETrackerAbstarctor_VolumeType - volume type

// Explanation

// This method handles the query for searching songs of given artist List

// It connects signal/slot for the query completion

//

// History

// 14-May-2011      Chetan Nanda              created for seaching songs of given artist List
*****************************************************************************/
bool CMediaDataProvider::GetMultipleArtistSongsFromVolume(QStringList AlbumnamesList,
                                                                        ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;
    QString filter = " FILTER(";
    QString performer = "?performer=";

    for (int j = 0; j < AlbumnamesList.size(); j++) {
        QString Regex = " (";

        Regex.append(performer);
        Regex.append(gDoubleQuote);
        AlbumnamesList[j].replace(gDoubleQuote, QByteArray("\\\""));	// added by sangmin.seol 2013.10.16 ITS_0189462 replace " to \" for sparql query
        Regex.append(AlbumnamesList[j]);
        Regex.append(gDoubleQuote);
        if(j == (AlbumnamesList.size()-1) )
            Regex.append(")");
        else
            Regex.append(") || ");

        filter.append(Regex);
    }

    filter.append(") .");
    QString volumeName;
    ret = GetVolumeQueryToAppend(volumeType, volumeName);

    if(!ret)
        return ret;

    filter.append(volumeName);

    QSparqlQuery QueryObject(gGetMultipleArtistSongsFromVolumeQuery.arg(filter));
    QueryObject.bindValue("FILTER",filter);
    m_Argument_GetMultipleArtistSongsFromVolume = filter;

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

// Function Name                                                                    Parameters

// CMediaDataProvider::GetMultipleArtistSongsFromVolume        bool (OUT), QString (IN) - folder path

// Explanation

// This method retrieves the music folder/file list used by folder tab in music list view

//

// History

// 29-May-2011      Chetan Nanda              created for retriving the music folder/file list
*****************************************************************************/
bool CMediaDataProvider::GetFolderTabList(QString folderPath)
{
    bool ret = false;
    const QStringList SUPPORTED_MIME_TYPE (QStringList()<<"*.m4a"<<"*.mp3"<<"*.oga"<<"*.ogg"<<"*.wav"<<"*.wma"); // modified by eugene.eseo 2013.04.17
    const QDir::Filters FILE_FILTERS = QDir::Files | QDir::System | QDir::Hidden | QDir::NoDot | QDir::NoDotDot;
    QString curItemPath;
    QStringList itemNameList;
    QStringList itemPathList;
    QStringList folderFlag;
    QStringList emptyFolderFlag;

    QStringList folderList;
    folderList = QDir(folderPath).entryList(QStringList("*"), QDir::Dirs | QDir::NoDot | QDir::NoDotDot );
    itemNameList = folderList;

    foreach ( QString folderName, folderList )
    {
        itemPathList << folderPath + "/" + folderName;
        folderFlag << "true";
        QStringList fileListCheck;
        fileListCheck = QDir(folderPath + "/" + folderName).entryList(SUPPORTED_MIME_TYPE, FILE_FILTERS );
        if(fileListCheck.count() > 0)
		{
            emptyFolderFlag << "false";
		}
        else
		{
            QString itemPath;
            itemPath = folderPath + "/" + folderName;
            QDirIterator dirIterator(itemPath, SUPPORTED_MIME_TYPE, FILE_FILTERS, QDirIterator::Subdirectories);
            bool flag = false;
    	    while(dirIterator.hasNext())
			{
	             flag = true;
                 emptyFolderFlag << "false";
                 break;
			}
            
			if(flag == false)
            	emptyFolderFlag << "true";
        }
    }
    QStringList fileList;
    fileList = QDir(folderPath).entryList(SUPPORTED_MIME_TYPE, FILE_FILTERS );
    foreach ( QString fileName, fileList )
    {
        curItemPath = folderPath + "/" + fileName;
        itemNameList << fileName;
        itemPathList << curItemPath;
        folderFlag << "false";
        emptyFolderFlag << "false";
    }

    if(itemPathList.count() > 0)
    {
        QVector<QStringList> dataList;
        QStringList resultData;
        for(int i=0; i<itemPathList.count(); i++)
        {
            resultData.clear();
            resultData.append(itemPathList[i]);
            resultData.append(folderFlag[i]);
            resultData.append(emptyFolderFlag[i]);

            dataList.append(resultData);
        }

        emit DataFetched(dataList, eGetFolderTabList_Id);
        ret = true;
    }
    else
        ret = false;

    return ret;
}

bool CMediaDataProvider::senddata(int noOfColumns,QString querystring,QSqlDatabase ipoddb, QVector<QStringList> &queryDataList)
{
    qDebug() << "[TA] in send data for IPod Search"<<querystring;

    bool  ret=false ;
    QSqlQuery* query = new QSqlQuery(querystring,ipoddb);
     ret=exec(query);
     qDebug() << "  [TA] send data for IPod Search exec successfully"<<ret;
      if (ret)
      {
         //QVector<QStringList> queryDataList;
         //queryDataList.clear();
             while(query->next())
             {
                QStringList columnList;
                for (int i = 0; i < noOfColumns; i++)
                {
                    QString value = query->value(i).toString();
                    //{ modified by jaehwan 2014.1.9 for % deconfig issue
                    if (value.startsWith(QLatin1String("file://")))
                    {
                        value = QUrl::fromPercentEncoding(value.toUtf8());
                    }
                    columnList.append(value);
                    //} modified by jaehwan
                }
                queryDataList.append(columnList);
             }
      }
    delete query;
    qDebug() << "  [TA] send data for IPod Search : result Data count"<<queryDataList.count();
    return ret;
}

void CMediaDataProvider::SetIPodDBConnectionList(QList<QSqlDatabase> ipodDBConnList)
{
    m_Ipod_DBConnList.clear();
    m_Ipod_DBConnList.append(ipodDBConnList);
}

void CMediaDataProvider::CheckIfFileExists(QVector<QStringList> &dataList, int column)
{
    QVector<QStringList> temp;
    QString filePath;
    for(int i=0; i<dataList.count(); i++)
    {
        filePath.clear();
        filePath.append(dataList[i][column]);
        if(!filePath.startsWith("file:///ipod") && !filePath.startsWith("file:///usr_data") && !filePath.startsWith("/usr_data") && !filePath.startsWith("file:///cd"))
        {
            if(filePath.startsWith("file://"))
                filePath = filePath.mid(7);
            QFile f1(filePath);
            if(!f1.exists())
            {
                temp.append(dataList[i]);
            }
        }
    }

    for(int j=0; j<temp.count(); j++)
    {
        for(int k=0;k<dataList.count(); k++)
        {
            if(dataList[k][0] == temp[j][0])
            {
                dataList.remove(k);
                break;
            }
        }
    }
}

/******************************************************************************

// Function Name                                                Parameters

// CMediaDataProvider::GetAllMusicByKeywordForVR      bool (OUT), QString (IN) - keyword to search

//

// Explanation

// This method handles the query for retrieving music info using given keyword

// It connects signal/slot for the query completion

//

// History

// 22-June-2012      Chetan Nanda                created for retrieving music info using given keyword for VR app
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicByKeywordForVR(QString keyword)
{
    bool ret=false;
    if(keyword.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    QTextCodec *codec2 = QTextCodec::codecForName("EUC-KR");
    QString utfStr = codec2->fromUnicode(keyword);

    LOG_TRACE << "first method For VR::" << keyword << LOG_ENDL;
    LOG_TRACE << "second method For VR::" << utfStr << LOG_ENDL;

    CheckForSpecialCharacters(keyword);
    CheckForSpecialCharacters(utfStr);

    m_Keyword_AllMusicByKeywordForVR = keyword;
    m_Keyword1_AllMusicByKeywordForVR = utfStr;
    QString queryString;

    // setting query string
    queryString = gGetAllMusicByKeywordForVRQuery.arg(keyword, utfStr);

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

// Function Name                                                       Parameters

// CMediaDataProvider::CheckForSpecialCharacters      bool (OUT), QString (IN) - keyword to search

//

// Explanation

// This method handles the speical charcters used while searching

// It connects signal/slot for the query completion

//

// History

// 05-July-2012      Chetan Nanda                created for handling special characters used for search
*****************************************************************************/
void CMediaDataProvider::CheckForSpecialCharacters(QString &keyword)
{
    QList<QChar> specialCharList;
    specialCharList << '`' << '~' << '!' << '@' << '#' << '$' << '%' << '^';
    specialCharList << '&' << '*' << '(' << ')' << '-' << '_' << '+' << '=' << '[' << ']';
    specialCharList << '{' << '}' << '|' << ':' << ';'  << '\'';
//    specialCharList.append('\'');
    specialCharList << ',' << '.' << '<' << '>' << '?' << '/' ;

    QChar charaa('\'');

   QChar backslash = '\\';

   QString backSlashEscape = "\\\\\\\\";
   keyword = keyword.replace(backslash, backSlashEscape);

   QString doubleQuote = "\"";
   QString doubleQuoteEscape = "\\\\\\\"" ;
   keyword = keyword.replace(doubleQuote, doubleQuoteEscape);
    foreach (QChar ch,  specialCharList)
    {
        QString specialCharEscape = "\\\\";
        specialCharEscape.append(ch);
        keyword = keyword.replace(ch, specialCharEscape);
    }
}

/******************************************************************************

// Function Name                                                       Parameters

// CMediaDataProvider::GetIndexBySongFromVolume      bool (OUT), QString (IN) - keyword to search,
                                                          volumeType(IN) - volume to be searched

//

// Explanation

// This method will return the index for specified song

// It connects signal/slot for the query completion

//

// History

// 25-July-2012     Vishal Jain               created for returning index for specific song
*****************************************************************************/

bool CMediaDataProvider::GetIndexBySongFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType)
{

    bool ret=false;

    m_VolumeNameList.clear();

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsFromVolumeForIndex);
    if(!ret)
        return ret;

    m_SearchData_ForIndex.clear();
	//Added for CR14033
    if (!searchData.startsWith(QLatin1String("file://"))) {
            searchData = QLatin1String("file://") + searchData;
            LOG_TRACE << "searchData does not have file://" << searchData << LOG_ENDL;
        }
    m_SearchData_ForIndex = searchData;

    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("title"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllSongsFromVolumeExtOrderQueryForIndex.arg(m_VolumeName_GetAllSongsFromVolumeForIndex)
            .arg(m_ExtendOrdering);
    }
    else 
    {
        queryString = gGetAllSongsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllSongsFromVolumeForIndex);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsFromVolumeForIndex);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	
    //} 
/*
    QSparqlQuery QueryObject(gGetAllSongsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllSongsFromVolumeForIndex));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsFromVolumeForIndex);

    // executing query
    exec(&QueryObject);
*/
    // if query executed successfully then connecting signal/slot for result
    if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnIndexFinished()));
    }
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }

    return ret;

}

// { added by sungha.choi 2013.07.23
bool CMediaDataProvider::GetIndexWithCountBySongFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType, bool bEnableCacahe)
{
    if(bEnableCacahe) {    // modified by sungha.choi 2013.07.30
        if(savedSongVolumeType == volumeType && QString::compare(searchData, savedSongSearchData) == 0) {
            LOG_TRACE << "Cache Success : Song Index, Total Count" << LOG_ENDL;
            m_cacheSuccess = true;
            emit DataFetched(songIndexCountSavedList, eGetSongIndexWithTitleCountFromVolumeQuery_Id);
            return true;
        }
        else {
            savedSongSearchData.clear();
            savedSongSearchData.append(searchData);
            savedSongVolumeType = volumeType;
        }
    } else {
        savedSongSearchData.clear();    // added by sungha.choi 2013.07.30
        savedSongVolumeType = eDefaultVolume;    // added by sungha.choi 2013.07.30
    }
    m_cacheSuccess = false;
    bool ret=false;

    m_VolumeNameList.clear();

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllSongsFromVolumeForIndex);
    if(!ret)
        return ret;

    m_SearchData_ForIndex.clear();
	//Added for CR14033
    if (!searchData.startsWith(QLatin1String("file://"))) {
            searchData = QLatin1String("file://") + searchData;
            LOG_TRACE << "searchData does not have file://" << searchData << LOG_ENDL;
        }
    m_SearchData_ForIndex = searchData;


    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    QString queryString;
     if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("title"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllSongsFromVolumeExtOrderQueryForIndex.arg(m_VolumeName_GetAllSongsFromVolumeForIndex)
            .arg(m_ExtendOrdering);
    }
    else 
    {
        queryString = gGetAllSongsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllSongsFromVolumeForIndex);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsFromVolumeForIndex);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	
    //} 

/*
    QSparqlQuery QueryObject(gGetAllSongsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllSongsFromVolumeForIndex));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllSongsFromVolumeForIndex);

    // executing query
    exec(&QueryObject);
*/
    // if query executed successfully then connecting signal/slot for result
    if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnIndexWithCountFinished()));
    }
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }

    return ret;

}
// } added by sungha.choi 2013.07.23

/******************************************************************************

// Function Name                                                       Parameters

// CMediaDataProvider::GetIndexByAlbumFromVolume      bool (OUT), QString (IN) - keyword to search,
                                                           volumeType(IN) - Volume type

//

// Explanation

// This method is used to return index of a specific album

// It connects signal/slot for the query completion

//

// History

// 25-July-2012      Vishal Jain       created for returning index of a given album
*****************************************************************************/

bool CMediaDataProvider::GetIndexByAlbumFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeNameList.clear();

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllAlbumsFromVolumeForIndex);
    if(!ret)
        return ret;

    m_SearchData_ForIndex.clear();
    m_SearchData_ForIndex = searchData;

    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("album"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllAlbumsFromVolumeExtOrderQueryForIndex.arg(m_VolumeName_GetAllAlbumsFromVolumeForIndex)
            .arg(m_ExtendOrdering);
    }
    else 
    {
        queryString = gGetAllAlbumsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllAlbumsFromVolumeForIndex);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsFromVolumeForIndex);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	
    //} 
/*
    QSparqlQuery QueryObject(gGetAllAlbumsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllAlbumsFromVolumeForIndex));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsFromVolumeForIndex);

    //    // executing query
    exec(&QueryObject);
*/
    // if query executed successfully then connecting signal/slot for result
    if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnIndexFinished()));
    }
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }

    return ret;

}

// { added by sungha.choi 2013.07.23
bool CMediaDataProvider::GetIndexWithCountByAlbumFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType, bool bEnableCacahe)
{
    if(bEnableCacahe) {    // modified by sungha.choi 2013.07.30
        if(savedAlbumVolumeType == volumeType && QString::compare(searchData, savedAlbumSearchData) == 0) {
            LOG_TRACE << "Cache Success : Album Index, Total Count" << LOG_ENDL;
            m_cacheSuccess = true;
            emit DataFetched(albumIndexCountSavedList, eGetAlbumIndexWithAlbumCountFromVolumeQuery_Id);
            return true;
        }
        else {
            savedAlbumSearchData.clear();
            savedAlbumSearchData.append(searchData);
            savedAlbumVolumeType = volumeType;
        }
    } else {
        savedAlbumSearchData.clear();    // added by sungha.choi 2013.07.30
        savedAlbumVolumeType = eDefaultVolume;    // added by sungha.choi 2013.07.30
    }

    m_cacheSuccess = false;
    bool ret=false;

    m_VolumeNameList.clear();

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllAlbumsFromVolumeForIndex);
    if(!ret)
        return ret;

    m_SearchData_ForIndex.clear();
    m_SearchData_ForIndex = searchData;

    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("album"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllAlbumsFromVolumeExtOrderQueryForIndex.arg(m_VolumeName_GetAllAlbumsFromVolumeForIndex)
            .arg(m_ExtendOrdering);
    }
    else 
    {
        queryString = gGetAllAlbumsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllAlbumsFromVolumeForIndex);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsFromVolumeForIndex);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	
    //} 

/*
    QSparqlQuery QueryObject(gGetAllAlbumsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllAlbumsFromVolumeForIndex));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllAlbumsFromVolumeForIndex);

    //    // executing query
    exec(&QueryObject);
*/
    // if query executed successfully then connecting signal/slot for result
    if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnIndexWithCountFinished()));
    }
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }

    return ret;

}
// } added by sungha.choi 2013.07.23

/******************************************************************************

// Function Name                                                       Parameters

// CMediaDataProvider::GetIndexByArtistFromVolume      bool (OUT), QString (IN) - keyword to search,
                                                           volumeType(IN) - Volume type
//

// Explanation

// This method will return index for specified artist name

// It connects signal/slot for the query completion

//

// History

// 25-July-2012      Vishal Jain              created for returning index of a artist
*****************************************************************************/

bool CMediaDataProvider::GetIndexByArtistFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret=false;

    m_VolumeNameList.clear();

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllArtistsFromVolumeForIndex);
    if(!ret)
        return ret;

    m_SearchData_ForIndex.clear();

    m_SearchData_ForIndex = searchData;

    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("artist"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllArtistsFromVolumeExtOrderQueryForIndex.arg(m_VolumeName_GetAllArtistsFromVolumeForIndex)
            .arg(m_ExtendOrdering);
    }
    else 
    {
        queryString = gGetAllArtistsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllArtistsFromVolumeForIndex);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllArtistsFromVolumeForIndex);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	
    //} 
/*
    QSparqlQuery QueryObject(gGetAllArtistsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllArtistsFromVolumeForIndex));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllArtistsFromVolumeForIndex);

    //    // executing query
    exec(&QueryObject);
*/
    // if query executed successfully then connecting signal/slot for result
    if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnIndexFinished()));
    }
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }

    return ret;

}

// { added by sungha.choi 2013.07.23
bool CMediaDataProvider::GetIndexWithCountByArtistFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType, bool bEnableCacahe)
{
    if(bEnableCacahe) {    // modified by sungha.choi 2013.07.30
        if(savedArtistVolumeType == volumeType && QString::compare(searchData, savedArtistSearchData) == 0) {
            LOG_TRACE << "Cache Success : Artist Index, Total Count" << LOG_ENDL;
            m_cacheSuccess = true;
            emit DataFetched(artistIndexCountSavedList, eGetArtistIndexWithArtistCountFromVolumeQuery_Id);
            return true;
        }
        else {
            savedArtistSearchData.clear();
            savedArtistSearchData.append(searchData);
            savedArtistVolumeType = volumeType;
        }
    } else {
        savedArtistSearchData.clear();    // added by sungha.choi 2013.07.30
        savedArtistVolumeType = eDefaultVolume;    // added by sungha.choi 2013.07.30
    }

    m_cacheSuccess = false;
    bool ret=false;

    m_VolumeNameList.clear();

    m_VolumeType = volumeType;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllArtistsFromVolumeForIndex);
    if(!ret)
        return ret;

    m_SearchData_ForIndex.clear();

    m_SearchData_ForIndex = searchData;
   
    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    // {modify by jawhean.lee for audio list sorting issue. 13.10. 02
    QString queryString;
    if (m_lang == LANGUAGE_KO ||  m_lang == LANGUAGE_AR || m_lang == LANGUAGE_ZH_CMN ) //modified by jaehwan.lee 2013.12.27
    {
        if (!GetExtendOrderingQueryToAppend(m_lang, QString("artist"), m_ExtendOrdering) )
            return false; 
        
        LOG_TRACE << "m_ExtendOrdering = "<<m_ExtendOrdering<<LOG_ENDL; 
        queryString = gGetAllArtistsFromVolumeExtOrderQueryForIndex.arg(m_VolumeName_GetAllArtistsFromVolumeForIndex)
            .arg(m_ExtendOrdering);
    }
    else 
    {
        queryString = gGetAllArtistsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllArtistsFromVolumeForIndex);

    }
    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllArtistsFromVolumeForIndex);

    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 
    LOG_TRACE << Q_FUNC_INFO << "query:  " <<  QueryObject.query() <<LOG_ENDL; 
    LOG_TRACE << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << LOG_ENDL; 

    exec(&QueryObject);	
    //} 
/*
    QSparqlQuery QueryObject(gGetAllArtistsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllArtistsFromVolumeForIndex));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllArtistsFromVolumeForIndex);

    // executing query
    exec(&QueryObject);
*/
    // if query executed successfully then connecting signal/slot for result
    if (!m_pQueryResult->hasError())
    {
        ret=connect(m_pQueryResult, SIGNAL(finished()), this, SLOT(OnIndexWithCountFinished()));
    }
    else
    {
        HandleError(eErrorInQueryExecution);
        return ret;
    }

    return ret;
}
// } added by sungha.choi 2013.07.23

/******************************************************************************

// Function Name                                                       Parameters

// CMediaDataProvider::OnIndexFinished                                  None

//

// Explanation

// This slot will handle the index based queries

//

// History

// 25-July-2012      Vishal Jain    Slot will be hit for index based queries
*****************************************************************************/

void CMediaDataProvider::OnIndexFinished()
{

    QStringList queryDataList;
    QString query = gBlankSpace;

    ETrackerAbstarctor_Query_Ids queryId = eDefault_Id;
    QSparqlResult* result = qobject_cast<QSparqlResult*>(sender());
    if(result == NULL)  return; // added by sungha.choi 2013.07.24

    query = result->query();

    if( // modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllSongsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllSongsFromVolumeForIndex))
        || !query.compare(gGetAllSongsFromVolumeExtOrderQueryForIndex.arg(m_VolumeName_GetAllSongsFromVolumeForIndex)
            .arg(m_ExtendOrdering))
        )
    {
        m_VolumeName_GetAllSongsFromVolumeForIndex.clear();
        queryId = eGetSongIndexFromVolumeQuery_Id;
    }
    else if( // modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllAlbumsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllAlbumsFromVolumeForIndex))
        || !query.compare(gGetAllAlbumsFromVolumeExtOrderQueryForIndex.arg(m_VolumeName_GetAllAlbumsFromVolumeForIndex)
            .arg(m_ExtendOrdering))
       )
    {
        m_VolumeName_GetAllAlbumsFromVolumeForIndex.clear();
        queryId = eGetAlbumIndexFromVolumeQuery_Id;
    }
    else if( // modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllArtistsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllArtistsFromVolumeForIndex))
        || !query.compare(gGetAllArtistsFromVolumeExtOrderQueryForIndex.arg(m_VolumeName_GetAllArtistsFromVolumeForIndex)
            .arg(m_ExtendOrdering))
       )
    {
        m_VolumeName_GetAllArtistsFromVolumeForIndex.clear();
        queryId = eGetArtistIndexFromVolumeQuery_Id;
    }
    else
    {
        LOG_TRACE << "Mapping ERROR Query" << query << LOG_ENDL;
        LOG_TRACE << "Mapping ERROR Query  Number" << queryId << LOG_ENDL;

        HandleError(eQueryMappingError);
        if(result != NULL)
        {
            delete result;
            result = NULL;
        }
        return;
    }

    if(result->hasError())
    {
        LOG_TRACE <<"Error In Query :"<< query << LOG_ENDL;
        HandleError(eErrorInQuery);
        QSparqlError queryError = result->lastError();
        LOG_TRACE <<"Query Error type = "<<queryError.type()
                << "Query Error Message = "<<queryError.message() << LOG_ENDL;
        if(result != NULL)
        {
            delete result;
            result = NULL;
        }
        return;
    }

    // retrieving data from QSparqlResult
    while (result->next())
    {
        QString value = result->value(0).toString();
        if (value.startsWith(QLatin1String("file://"))) {
            value = QLatin1String("file://") + QUrl::fromEncoded(value.toLatin1()).toLocalFile();
        }

        queryDataList.append(value);
    }

    /* // removed by eugene.seo 2013.03.27 for static test
    if(result != NULL)
    {
        delete result;
        result = NULL;
    }
    */
    int index = queryDataList.indexOf(m_SearchData_ForIndex);

    // Clear the old list
    queryDataList.clear();

    // Prepare the qvector list, as expected by HMI
    QVector<QStringList> dataList;
    QString indexString;
    indexString.setNum(index);
    queryDataList.append(indexString);
    dataList.append(queryDataList);
    // after data fetching emiting a signal to notify CTrackerAbstractor

    emit DataFetched(dataList, queryId);

}

// { added by sungha.choi 2013.07.23
void CMediaDataProvider::OnIndexWithCountFinished()
{
    QStringList queryDataList;
    QString query = gBlankSpace;

    ETrackerAbstarctor_Query_Ids queryId = eDefault_Id;
    QSparqlResult* result = qobject_cast<QSparqlResult*>(sender());
    if(result == NULL)  return; // added by sungha.choi 2013.07.24
    QVector<QStringList> *dataList;


    query = result->query();

    if( // modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllSongsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllSongsFromVolumeForIndex))
        || !query.compare(gGetAllSongsFromVolumeExtOrderQueryForIndex.arg(m_VolumeName_GetAllSongsFromVolumeForIndex)
            .arg(m_ExtendOrdering))
        )
    {
        m_VolumeName_GetAllSongsFromVolumeForIndex.clear();
        queryId = eGetSongIndexWithTitleCountFromVolumeQuery_Id;
        songIndexCountSavedList.clear();
        dataList = &songIndexCountSavedList;
    }
    else if( // modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllAlbumsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllAlbumsFromVolumeForIndex))
        || !query.compare(gGetAllAlbumsFromVolumeExtOrderQueryForIndex.arg(m_VolumeName_GetAllAlbumsFromVolumeForIndex)
            .arg(m_ExtendOrdering))        
        )
    {
        m_VolumeName_GetAllAlbumsFromVolumeForIndex.clear();
        queryId = eGetAlbumIndexWithAlbumCountFromVolumeQuery_Id;
        albumIndexCountSavedList.clear();
        dataList = &albumIndexCountSavedList;
    }
    else if( // modify by jawhean.lee for audio list sorting issue. 13.10. 02
        !query.compare(gGetAllArtistsFromVolumeQueryForIndex.arg(m_VolumeName_GetAllArtistsFromVolumeForIndex))
        || !query.compare(gGetAllArtistsFromVolumeExtOrderQueryForIndex.arg(m_VolumeName_GetAllArtistsFromVolumeForIndex)
            .arg(m_ExtendOrdering))        
        )
    {
        m_VolumeName_GetAllArtistsFromVolumeForIndex.clear();
        queryId = eGetArtistIndexWithArtistCountFromVolumeQuery_Id;
        artistIndexCountSavedList.clear();
        dataList = &artistIndexCountSavedList;
    }
    else
    {
        LOG_TRACE << "Mapping ERROR Query" << query << LOG_ENDL;
        LOG_TRACE << "Mapping ERROR Query  Number" << queryId << LOG_ENDL;

        HandleError(eQueryMappingError);
        if(result != NULL)
        {
            delete result;
            result = NULL;
        }
        return;
    }

    if(result->hasError())
    {
        LOG_TRACE <<"Error In Query :"<< query << LOG_ENDL;
        HandleError(eErrorInQuery);
        QSparqlError queryError = result->lastError();
        LOG_TRACE <<"Query Error type = "<<queryError.type()
                << "Query Error Message = "<<queryError.message() << LOG_ENDL;
        if(result != NULL)
        {
            delete result;
            result = NULL;
        }
        return;
    }

    // retrieving data from QSparqlResult
    int index = 0;  // added by sungha.choi 2013.07.22
    int total_count = result->size();

    while (result->next())
    {
        QString value = result->value(0).toString();
        if (value.startsWith(QLatin1String("file://"))) {
            value = QLatin1String("file://") + QUrl::fromEncoded(value.toLatin1()).toLocalFile();
        }
        // { added by sungha.choi
        if(m_SearchData_ForIndex.compare(value) == 0) break;
        index ++;
        // } added by sungha.choi
        //queryDataList.append(value);  // deleted by sungha.choi
    }

    //int index = queryDataList.indexOf(m_SearchData_ForIndex);

    // Clear the old list
    //queryDataList.clear();

    // Prepare the qvector list, as expected by HMI
    //dataList->clear();

    QString indexString;
    indexString.setNum(total_count);
    queryDataList.append(indexString);

    indexString.clear();
    indexString.setNum(index);
    queryDataList.append(indexString);

    dataList->append(queryDataList);
    // after data fetching emiting a signal to notify CTrackerAbstractor

    emit DataFetched(*dataList, queryId);

}
// } added by sungha.choi 2013.07.23

/******************************************************************************

// Function Name                                                       Parameters

// CMediaDataProvider::GetIndexDataFromVolumeForIPOD      bool (OUT), queryId (IN) - query id,
                                                          ipodDb - IPOD Sql instance
//

// Explanation

// This method prepare the queries for index based for iPod

// It connects signal/slot for the query completion

//

// History

// 25-July-2012      Vishal Jain                created for preparing queries for iPod
*****************************************************************************/

bool CMediaDataProvider::GetIndexDataFromVolumeForIPOD(QString searchData, ETrackerAbstarctor_Query_Ids queryId, QSqlDatabase ipodDb)
{

    QString queryString;

    bool ret=false;

    if(searchData.isEmpty())
    {
        HandleError(eEmptyString);
        return ret;
    }

    switch (queryId)
    {

    case eGetSongIndexFromVolumeQuery_Id:
    {
        queryString.clear();
        //{changed by junma 2013.04.30 for song iPod sort
        //queryString.append( QString("select DISTINCT T_FilePath from MasterTable where T_Title not like \"%1\" order by LTRIM(T_Title) COLLATE NOCASE ").arg(""));
        queryString.append( QString("select DISTINCT T_FilePath from MasterTable"));
        //}changed by junam

        m_SearchData_ForIndex.clear();
        m_SearchData_ForIndex = searchData;
        //{added by junam 2013.06.10 for UID no play icon
        if(searchData.mid(searchData.lastIndexOf("/")+1, 2).toInt() == -1)
        {
            return sendIndexDataForBadFile(queryId, queryString, ipodDb);
        }
        //}added by junam
    }
        break;
    case eGetAlbumIndexFromVolumeQuery_Id:
    {
        queryString.clear();
        //{change by junam 2013.05.23 for sorting
        //queryString.append( QString("select DISTINCT T_Album from MasterTable where T_Album not like \"%1\" order by LTRIM(T_Album) COLLATE NOCASE ").arg(""));
        queryString.append( QString("select DISTINCT T_Album from MasterTable where T_Album not like \"%1\" ").arg(""));
        queryString.append(getOrderByMethod("T_Album"));
        //}changed by junam
        m_SearchData_ForIndex.clear();
        m_SearchData_ForIndex = searchData;
    }
        break;
    case eGetArtistIndexFromVolumeQuery_Id:
    {
        queryString.clear();
        //{change by junam 2013.05.23 for sorting
        //queryString.append( QString("select DISTINCT T_Artist from MasterTable where T_Artist not like \"%1\" order by LTRIM(T_Artist) COLLATE NOCASE ").arg(""));
        queryString.append( QString("select DISTINCT T_Artist from MasterTable where T_Artist not like \"%1\"  ").arg(""));
        queryString.append(getOrderByMethod("T_Artist"));
        //}changed by junam
        m_SearchData_ForIndex.clear();
        m_SearchData_ForIndex = searchData;
    }
        break;

    default:
        break;
    }

    ret = sendIndexData(queryId, queryString, ipodDb);
    return ret;
}

/******************************************************************************

// Function Name                                                       Parameters

// CMediaDataProvider::sendIndexData      bool (OUT), QString (IN) - keyword to search

//

// Explanation

// This method executes SQL queries for index based queries for iPod

// It connects signal/slot for the query completion

//

// History

// 25-July-2012      Vishal Jain       created for handling index based queries for iPod
*****************************************************************************/

bool CMediaDataProvider::sendIndexData(ETrackerAbstarctor_Query_Ids queryId, QString queryString,QSqlDatabase ipodDb )
{
    qDebug() << "[TA] in sendIndexData"<< queryId << queryString;

    bool  ret=false ;
    QSqlQuery* query = new QSqlQuery(queryString,ipodDb);
    ret=exec(query);

    if (ret)
    {

        QStringList queryDataList;
        queryDataList.clear();

        while(query->next())
        {
            QString value = query->value(0).toString();
            //queryDataList.append(QUrl::fromPercentEncoding(value.toUtf8()));
            //{ modified by jaehwan 2014.2.10 for % deconfig issue
            if (value.startsWith(QLatin1String("file://")))
            {
                value = QUrl::fromPercentEncoding(value.toUtf8());
            }
            queryDataList.append(value);
            //} modified by jaehwan
        }

        int index = queryDataList.indexOf(m_SearchData_ForIndex);

        queryDataList.clear();

        QVector<QStringList> dataList;
        QString indexString;
        indexString.setNum(index);
        queryDataList.append(indexString);
        dataList.append(queryDataList);        

        emit DataFetched(dataList, queryId);

    }
    delete query;
    return ret;

}

//{changed by junam 2013.08.21 for iPod file compare
bool CMediaDataProvider::sendIndexDataForBadFile(ETrackerAbstarctor_Query_Ids queryId, QString querrystring,QSqlDatabase ipoddb )
{
    qDebug() << "[TA] in sendIndexDataForBadFile"<< queryId << querrystring;

    bool  ret=false ;
    QSqlQuery query(querrystring,ipoddb);
    ret=query.exec();
    QString indexString = "-1";

    if (ret)
    {
        int index = -1;
        quint64 uid;
        if(CTrackerAbstractor::getIPodFileUID(m_SearchData_ForIndex, uid))
        {
            quint64 retUID;
            while(query.next())
            {
                index++; //added by junam 2013.08.26 for playicon
                if(CTrackerAbstractor::getIPodFileUID(query.value(0).toString(), retUID) && retUID == uid)
                {
                    indexString.setNum(index); //added by junam 2013.09.06 for ISV_KOR_90719
                    break;
                }
            }
        }
        //indexString.setNum(index); //removed by junam 2013.09.06 for ISV_KOR_90719
    }

    QVector<QStringList> dataList;
    QStringList queryDataList;
    queryDataList.append(indexString);
    dataList.append(queryDataList);
    emit DataFetched(dataList, queryId);
    return ret;
}
//}changed by junam

/******************************************************************************

// Function Name                                            Parameters

// CMediaDataProvider::GetAllMetadataWithLimitFromVolume        bool (OUT), ETrackerAbstarctor_VolumeType  (IN) - volume to search
//                                                              ETrackerAbstractor_OrderType (IN)

// Explanation

// This method handles the query for getting metadata for a specific index

// It connects signal/slot for the query completion

//

// History

// 25-07-2012      Vishal Jain              created for getting metadata for one song
*****************************************************************************/
bool CMediaDataProvider::GetAllMetadataWithLimitFromVolume(ETrackerAbstarctor_VolumeType  volumeType,
                                                           ETrackerAbstarctor_OrderType orderType)
{
    bool ret=false;
    m_VolumeType = volumeType;
    m_OrderType = orderType;

    switch (orderType)
    {
    case eAlbum:
    {
        ret = true;
        m_OrderBy_GetMetaDataFromVolume = "?album";
    }
    break;
    case eArtist:
    {
        ret = true;
        m_OrderBy_GetMetaDataFromVolume = "?artist";
    }
    break;
    case eGenre:
    {
        ret = true;
        m_OrderBy_GetMetaDataFromVolume = "?genre";
    }
    break;
    case eTitle:
    {
        ret = true;
        m_OrderBy_GetMetaDataFromVolume = "?title";
    }
    break;
    case eUrl:
    {
        ret = true;
        m_OrderBy_GetMetaDataFromVolume = "?url";
    }
    break;
    default:
        break;
    }

    if(!ret)
        return ret;

    ret = GetVolumeQueryToAppend(volumeType, m_VolumeName_GetAllMetaDataWithLimitFromVolume);

    if(!ret)
        return ret;

    QSparqlQuery QueryObject(gGetAllMetaDataWithLimitFromVolumeQuery
                             .arg(m_VolumeName_GetAllMetaDataWithLimitFromVolume)
                             .arg(m_OrderBy_GetMetaDataFromVolume)
                             .arg(m_Query_Offset));
    QueryObject.bindValue("FILTER",m_VolumeName_GetAllMetaDataWithLimitFromVolume);

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

// CMediaDataProvider::SendCDData        queryString  (IN) - query, noOfColumns(IN) - number of columns
//                                                              ETrackerAbstractor_OrderType (IN)

// Explanation

// This method handles the response for sending CD data

// It connects signal/slot for the query completion

//

// History

// 08-08-2012      Vishal Jain              created for returning CD data
*****************************************************************************/

void CMediaDataProvider::SendCDData(QString queryString, int noOfColumns, ETrackerAbstarctor_Query_Ids queryId)
{
    LOG_TRACE << "SendCDData Enter " << LOG_ENDL;
    LOG_TRACE << queryString << LOG_ENDL;

    bool  ret=false ;

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", "CDDA");
    database.setDatabaseName("/app/data/gracenote/CDDAData"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change
    
    if(database.open())
    {

        QSqlQuery* query = new QSqlQuery(queryString,database);
        ret=exec(query);

        if (ret)
        {

            QVector<QStringList> queryDataList;
            queryDataList.clear();

            while(query->next())
            {
                QStringList columnList;

                for (int i = 0; i < noOfColumns; i++)
                {
                    QString value = query->value(i).toString();
                    columnList.append(QUrl::fromPercentEncoding(value.toUtf8()));
                }
                //LOG_TRACE << "CD Entry : " << columnList << LOG_ENDL;
                qDebug() << "CD Entry : " << columnList;
                queryDataList.append(columnList);

            }
            emit DataFetched(queryDataList, queryId);

        }
        else
        {
            LOG_TRACE << "CD Query Error : " << query->lastError().text() << LOG_ENDL;
        }
        delete query;
    }
    database.close();
    database.removeDatabase("CDDA");

    LOG_TRACE << "SendCDData Exit" << LOG_ENDL;
}

/******************************************************************************

// Function Name                                      Parameters

// CMediaDataProvider::GetMediaFileListByFolder       bool (OUT), QString (IN) - folder path,
                                                      ETrackerAbstarctor_Query_Ids (IN) - query Id

// Explanation

// This method retrieves the media file list

//

// History

// 19-July-2012      Chetan Nanda              created for retriving the media file list
*****************************************************************************/
bool CMediaDataProvider::GetMediaFileListByFolder(QString folderPath, ETrackerAbstarctor_Query_Ids queryId)
{
    // LOG_TRACE << Q_FUNC_INFO << folderPath << LOG_ENDL;
    bool ret = false;
    const QStringList SUPPORTED_VIDEO_MIME_TYPE (QStringList()<<"*.asf"<<"*.avi"<<"*.divx"<<"*.m2ts"<<"*.m4v"<<"*.mkv"<<"*.mp4"<<"*.mpg"<<"*.mpeg"<<"*.ps"<<"*.tp"<<"*.ts"<<"*.vod"<<"*.wmv"); // modified by eugene.seo 2013.04.17
    const QStringList SUPPORTED_IMG_MIME_TYPE (QStringList()<<"*.bmp"<<"*.gif"<<"*.jpg"<<"*.jpeg"<<"*.png"<<"*.tif"<<"*.tiff"); // modified by eugene.seo 2013.04.17

    const QDir::Filters FILE_FILTERS = QDir::Files | QDir::System | QDir::Hidden | QDir::NoDot | QDir::NoDotDot;
    QString curItemPath;
    QStringList itemNameList;
    QStringList itemPathList;
    QStringList folderFlag;
    QString modifiedFolderPath;

    if(folderPath.startsWith("file://"))
        modifiedFolderPath = folderPath.mid(7);
    else
        modifiedFolderPath = folderPath;

 //  LOG_TRACE << "CMediaDataProvider::GetMediaFileListByFolder modified path:: " << modifiedFolderPath << LOG_ENDL;
//    QStringList folderList;
//    folderList = QDir(folderPath).entryList(QStringList("*"), QDir::Dirs | QDir::NoDot | QDir::NoDotDot );
//    itemNameList = folderList;

//    foreach ( QString folderName, folderList )
//    {
//        itemPathList << folderPath + "/" + folderName;
//        folderFlag << "true";
//    }
    QStringList fileList;
    if( queryId == eGetVideosByFolderQuery_Id )
        fileList = QDir(modifiedFolderPath).entryList(SUPPORTED_VIDEO_MIME_TYPE, FILE_FILTERS );
    else if( queryId == eGetImagesByFolderQuery_Id )
          fileList = QDir(modifiedFolderPath).entryList(SUPPORTED_IMG_MIME_TYPE, FILE_FILTERS );

    foreach ( QString fileName, fileList )
    {
        curItemPath = folderPath + "/" + fileName;
        itemNameList << fileName;
        itemPathList << curItemPath;
        folderFlag << "false";

        //LOG_TRACE << "FileName: " << fileName << LOG_ENDL;
        //LOG_TRACE << "curItemPath: " << curItemPath << LOG_ENDL;

    }

    if(itemPathList.count() == itemNameList.count())
    {
        QVector<QStringList> dataList;
        QStringList resultData;
        for(int i=0; i<itemPathList.count(); i++)
        {
            resultData.clear();
            resultData.append(itemNameList[i]);
            resultData.append(itemPathList[i]);
            // LOG_TRACE << Q_FUNC_INFO << "Got Data:: " << itemNameList[i] << itemPathList[i] << LOG_ENDL;
            if( queryId == eGetVideosByFolderQuery_Id )
                resultData.append("");

            dataList.append(resultData);
        }
      //  LOG_TRACE << "CMediaDataProvider::GetMediaFileListByFolder Got Data:: " << dataList.count() << LOG_ENDL;
      //  LOG_TRACE << "CMediaDataProvider::GetMediaFileListByFolder queryId:: " << queryId << LOG_ENDL;
      //  LOG_TRACE << "CMediaDataProvider::GetMediaFileListByFolder Exit::" << LOG_ENDL;
        emit DataFetched(dataList, queryId);
        ret = true;
    }
    else
        ret = false;
    
    return ret;
}

/******************************************************************************

// Function Name                                        Parameters

// CMediaDataProvider::GetMediaFileListWithSubFolder    bool (OUT), QString (IN) - folder path,
                                                        ETrackerAbstarctor_Query_Ids (IN) - query Id

// Explanation

// This method retrieves the media file list with sub folders

//

// History

// 09-Aug-2012      Vishal Jain              created for retriving the media file list with sub folders
*****************************************************************************/
bool CMediaDataProvider::GetMediaFileListWithSubFolder(QString folderPath, ETrackerAbstarctor_Query_Ids queryId)
{
    LOG_TRACE << "CMediaDataProvider::GetMediaFileListWithSubFolder Enter::" << folderPath <<  LOG_ENDL;
    bool ret = false;
    const QStringList SUPPORTED_VIDEO_MIME_TYPE (QStringList()<<"*.asf"<<"*.avi"<<"*.divx"<<"*.m2ts"<<"*.m4v"<<"*.mkv"<<"*.mp4"<<"*.mpg"<<"*.mpeg"<<"*.ps"<<"*.tp"<<"*.ts"<<"*.vod"<<"*.wmv"); // modified by eugene.seo 2013.04.17
    const QStringList SUPPORTED_IMG_MIME_TYPE (QStringList()<<"*.bmp"<<"*.gif"<<"*.jpg"<<"*.jpeg"<<"*.png"<<"*.tif"<<"*.tiff"); // modified by eugene.seo 2013.04.17
	
    QStringList filterList;
    QString modifiedFolderPath;
    if(folderPath.startsWith("file://"))
        modifiedFolderPath = folderPath.mid(7);
    else
        modifiedFolderPath = folderPath;

  // LOG_TRACE << "CMediaDataProvider::GetMediaFileListWithSubFolder modified path:: " << modifiedFolderPath << LOG_ENDL;

    if( queryId == eGetVideosBySubFolderQuery_Id )
        filterList = SUPPORTED_VIDEO_MIME_TYPE;
    else if( queryId == eGetImagesBySubFolderQuery_Id )
        filterList = SUPPORTED_IMG_MIME_TYPE;

   //LOG_TRACE << "CMediaDataProvider::GetMediaFileListWithSubFolder Create object:: " << LOG_ENDL;

    QDirIterator dirIterator(modifiedFolderPath, filterList, QDir::NoSymLinks | QDir::Files, QDirIterator::Subdirectories);

   // LOG_TRACE << "CMediaDataProvider::GetMediaFileListWithSubFolder Create object:: " << LOG_ENDL;

    QVector<QStringList> dataList;
    QStringList resultData;
    while(dirIterator.hasNext())
	{
	   dirIterator.next();
           resultData.clear();
           resultData.append(dirIterator.fileInfo().baseName());
           resultData.append(dirIterator.fileInfo().filePath());
	   if( queryId == eGetVideosBySubFolderQuery_Id )
	       resultData.append("");
	   dataList.append(resultData);
	}
    LOG_TRACE << "CMediaDataProvider::GetMediaFileListWithSubFolder Got Data:: " << dataList.count() << LOG_ENDL;
   // LOG_TRACE << "CMediaDataProvider::GetMediaFileListWithSubFolder queryId:: " << queryId << LOG_ENDL;
   // LOG_TRACE << "CMediaDataProvider::GetMediaFileListWithSubFolder Exit::" << LOG_ENDL;

    if(dataList.count() > 0)
    {	
        ret = true;
    }
    else
        ret = false;

    emit DataFetched(dataList, queryId);
    return ret;
    
}

/******************************************************************************

// Function Name                                                Parameters

// CMediaDataProvider::GetAllMusicByConnectedVolumes      bool (OUT), QString (IN) - USB volume list

//

// Explanation

// This method handles the query for retrieving music from all connected volume except CD

// It connects signal/slot for the query completion

//

// History

// 22-Aug-2012      Chetan Nanda                created for retrieving music from all connected volume except CD
*****************************************************************************/
bool CMediaDataProvider::GetAllMusicByConnectedVolumes(QStringList volumeList)
{
    bool ret=false;

    m_PathList_GetAllMusicByConnectedVolumes.clear();

    volumeList.append(gVolumePath_JukeBox);

    if(volumeList.count() > 0)
    {
        m_PathList_GetAllMusicByConnectedVolumes = GetMusicUrlAppendedFromUrlList(volumeList);
        //m_PathList_GetAllMusicByConnectedVolumes.replace("FILTER", "");
    }

    QString queryString;

    // setting query string
    queryString = gGetAllMusicByConnectedVolumesQuery.arg(m_PathList_GetAllMusicByConnectedVolumes);

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

/*
 *******************************************************************
 * Function	: CMediaDataProvider::RequestTrackerMetadataExtraction
 *
 * Description 	: Request cover art from Tracker open source

 * Parameters 	: Human readable file URI of a music file

 * Returns 	: None
 *
 *******************************************************************
 */

bool CMediaDataProvider::RequestTrackerMetadataExtraction(QString& uri)
{
    QString filePath = uri;
    filePath.replace(QLatin1String("file://"), QLatin1String(""));
    QFileInfo fileInfo(filePath);
    QString encodedUri = QString::fromLatin1(QUrl::fromLocalFile(filePath).toEncoded());

    QDBusInterface interface(QLatin1String("org.freedesktop.Tracker1.Extract"),
                              QLatin1String("/org/freedesktop/Tracker1/Extract"),
                              QLatin1String("org.freedesktop.Tracker1.Extract"));

    QVariant arg1(encodedUri);

    QVariant arg2(QLatin1String("audio/mpeg"));
    QString suffix(fileInfo.completeSuffix().toUpper());
    if (    suffix == QLatin1String("MP3")
            || suffix == QLatin1String("MPEG")
            || suffix == QLatin1String("MPG") )
    {
        arg2 = QLatin1String("audio/mpeg");
    } else if (suffix == QLatin1String("OGG") || suffix == QLatin1String("OGA")) {
        arg2 = QLatin1String("audio/ogg");
    } else if (suffix == QLatin1String("WMA")) {
        arg2 = QLatin1String("audio/x-ms-wm");
    } else if (suffix == QLatin1String("M4A")) {
        arg2 = QLatin1String("audio/mp4");
    }
	else if (suffix == QLatin1String("WAV")) {
        arg2 = QLatin1String("audio/wav");
    }

    QVariant arg3(QLatin1String(""));

    QDBusMessage message = interface.call("GetMetadata", arg1, arg2, arg3);
    QDBusReply<QString> reply = message;

    QList<QVariant> args = message.arguments();
    Q_FOREACH (QVariant arg, args) {
        // LOG_TRACE << "arg: " << arg.toString() << LOG_ENDL;
    }

    QVector<QStringList> queryDataList;
    if (!reply.isValid() || args.count() < 3) {
        emit DataFetched(queryDataList, eTrackerExtractMetadataQuery_Id);
        return reply.isValid();
    }

    /*
      Tracker returns the results of a GetMetadata() call in turtle. The first
      part in args[0] contains INSERT statements for URIs for the Artist and Album.
      The second part of the turtle in args[2] gives details of the particular
      track. For example:

      args[0]:

        INSERT {
        GRAPH <> {
        <urn:artist:%EC%84%9C%EC%9D%B8%EC%98%81> a nmm:Artist ;
                nmm:artistName "?�인?? .
        }
        }
        INSERT {
        GRAPH <> {
        <urn:album:Brand%20New%20ELLY> a nmm:MusicAlbum ;
                nmm:albumTitle "Brand New ELLY" ;
                nmm:albumArtist <urn:artist:%EC%84%9C%EC%9D%B8%EC%98%81> .
        }
        }

      args[1]: <empty>

      args[2]:

        a nmm:MusicPiece , nfo:Audio ;
                nie:title "Oh My Gosh" ;
                nmm:performer <urn:artist:%EC%84%9C%EC%9D%B8%EC%98%81> ;
                nmm:musicAlbum <urn:album:Brand%20New%20ELLY> ;
                nfo:averageBitrate 320000 .
    */

    QString result0 = args[0].toString();
    QString result2 = args[2].toString();
    QStringList resultRow;
    int pos = 0;

    QRegExp matchTitle(QLatin1String("nie:title \"([^\\n]*)\" ;\\n"));
    pos = matchTitle.indexIn(result2, 0);
    if (pos != -1) {
        // LOG_TRACE << "Title: " << matchTitle.cap(1) <<  LOG_ENDL;
        resultRow.append(matchTitle.cap(1));
    } else {
        resultRow.append(QLatin1String(""));
    }

    QRegExp matchArtistName(QLatin1String("nmm:artistName \"([^\\n]*)\" \\.\\n"));
    pos = matchArtistName.indexIn(result0, 0);
    if (pos != -1) {
        // LOG_TRACE << "Artist name: " << matchArtistName.cap(1) <<  LOG_ENDL;
        resultRow.append(matchArtistName.cap(1));
    } else {
        resultRow.append(QLatin1String(""));
    }

    QRegExp matchAlbumTitle(QLatin1String("nmm:albumTitle \"([^\\n]*)\" ;\\n"));
    pos = matchAlbumTitle.indexIn(result0, 0);
    if (pos != -1) {
        // LOG_TRACE << "Album title: " << matchAlbumTitle.cap(1) << LOG_ENDL;
        resultRow.append(matchAlbumTitle.cap(1));
    } else {
        resultRow.append(QLatin1String(""));
    }

    QRegExp matchGenre(QLatin1String("nfo:genre \"([^\\n]*)\" ;\\n"));
    pos = matchGenre.indexIn(result2, 0);
    if (pos != -1) {
        // LOG_TRACE << "Genre: " << matchGenre.cap(1) <<  LOG_ENDL;
        resultRow.append(matchGenre.cap(1));
    } else {
        resultRow.append(QLatin1String(""));
    }

    resultRow.append(filePath);
    resultRow.append(QLatin1String(""));
    queryDataList.append(resultRow);
    emit DataFetched(queryDataList, eTrackerExtractMetadataQuery_Id);

    // LOG_TRACE << "Error message " << reply.error().message() <<  LOG_ENDL;
    // LOG_TRACE << "RequestTrackerMetadataExtraction Exit" <<  LOG_ENDL;

    return reply.isValid();
}


// { added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
bool CMediaDataProvider::GetPhotoMetaDataInfoFromUrl(QString& url)
{
	bool ret = false;
    if(url.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }
	
    m_FilePathInfo_MusicMetaData = GetMusicUrlAppended(url);

    QString queryString;
    queryString = gGetPhotoMetaDataInfoFromURLQuery.arg(m_FilePathInfo_MusicMetaData);

    QSparqlQuery QueryObject;
    QueryObject.setQuery(queryString);

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
// } added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
// { Added by Neelima, 2013.01.03 , for CR13965
/*
 *******************************************************************
 * Function	: CMediaDataProvider::getDeviceID
 *
 * Description 	: Returns the device ID

 * Parameters 	: volumeType

 * Returns 	: QString device id
 *
 *******************************************************************
 */
QString CMediaDataProvider::getDeviceID(ETrackerAbstarctor_VolumeType volumeType)
 {
    QString DeviceID;

    if(m_DeviceId.length() <= 0)
    {
        return NULL;
    }

    DeviceID = m_DeviceId.at(volumeType);

     LOG_TRACE << "CMediaDataProvider::getDeviceID m_DeviceId:" << DeviceID << LOG_ENDL;
    return DeviceID;
 }
// } Added by Divya

// { added by eugene.seo 2013.05.13
QString CMediaDataProvider::GetUSBMountPath()
{
    if(m_strMountPath.isEmpty() == true)
    {
        QFileInfo tFileInfo("/rw_data");
        if(tFileInfo.exists())
            m_strMountPath = "file:///rw_data/media";
        else
			m_strMountPath = "file:///media";
    }
    return m_strMountPath;
}
// } added by eugene.seo 2013.05.13

//{changed by junam 2013.08.21 for iPod sort
QString CMediaDataProvider::getOrderByMethod(QString item)
{
//{ modified by jaehwan to sort arabian for ipod
    LOG_CRITICAL << "GetLanguage() == " << GetLanguage() << "CTrackerAbstractor::GetSortingOrder() = " << CTrackerAbstractor::GetSortingOrder()<< LOG_ENDL;
    if (GetLanguage() == LANGUAGE_KO)
    {
        switch( CTrackerAbstractor::GetSortingOrder())
        {
        case 0:
            return QString(" order by (CASE "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN 'A' AND 'Z' THEN 1 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN 'a' AND 'z' THEN 1 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN CAST(x'EAB080' AS CHAR) AND CAST(x'ED9EAF' AS CHAR) THEN 2 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN CAST(x'E384B1' AS CHAR) AND CAST(x'E3858E' AS CHAR) THEN 2 " //modified by jaehwan for  hangul chosung range 2013.11.20
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '!' AND '/' THEN 3 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN ':' AND '@' THEN 3 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '[' AND '`' THEN 3 "   //modified by jaehwan 2014.02.28 to fix invalid sorting for lowcase alphabet
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '{' AND '~' THEN 3 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '0' AND '9' THEN 4 "
                           "ELSE 6 END), LTRIM(%1) COLLATE NOCASE ").arg(item);
        case 1:
            return QString(" order by (CASE "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN CAST(x'EAB080' AS CHAR) AND CAST(x'ED9EAF' AS CHAR) THEN 1 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN CAST(x'E384B1' AS CHAR) AND CAST(x'E3858E' AS CHAR) THEN 1 " //modified by jaehwan for  hangul chosung range 2013.11.20
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN 'A' AND 'Z' THEN 2 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN 'a' AND 'z' THEN 2 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '!' AND '/' THEN 3 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN ':' AND '@' THEN 3 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '[' AND '`' THEN 3 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '{' AND '~' THEN 3 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '0' AND '9' THEN 4 "
                           "ELSE 6 END), LTRIM(%1) COLLATE NOCASE ").arg(item);
        default:
            return QString(" order by LTRIM(%1) COLLATE NOCASE ").arg(item);
        }
    }
    else if (GetLanguage() == LANGUAGE_AR)
    {
        switch( CTrackerAbstractor::GetSortingOrder())
        {
        case 0:
            return QString(" order by (CASE "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN 'A' AND 'Z' THEN 1 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN 'a' AND 'z' THEN 1 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN CAST(x'D880' AS CHAR) AND CAST(x'DBBF' AS CHAR) THEN 2 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN CAST(x'DD90' AS CHAR) AND CAST(x'DDAD' AS CHAR) THEN 2 " //modified by jaehwan for  hangul chosung range 2013.11.20
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '!' AND '/' THEN 3 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN ':' AND '@' THEN 3 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '[' AND '`' THEN 3 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '{' AND '~' THEN 3 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '0' AND '9' THEN 4 "
                           "ELSE 6 END), LTRIM(%1) COLLATE NOCASE ").arg(item);
        case 1:
            return QString(" order by (CASE "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN CAST(x'D880' AS CHAR) AND CAST(x'DBBF' AS CHAR) THEN 1 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN CAST(x'DD90' AS CHAR) AND CAST(x'DDAD' AS CHAR) THEN 1 " //modified by jaehwan for  hangul chosung range 2013.11.20
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN 'A' AND 'Z' THEN 2 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN 'a' AND 'z' THEN 2 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '!' AND '/' THEN 3 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN ':' AND '@' THEN 3 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '[' AND '`' THEN 3 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '{' AND '~' THEN 3 "
                           "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '0' AND '9' THEN 4 "
                           "ELSE 6 END), LTRIM(%1) COLLATE NOCASE ").arg(item);
        default:
            return QString(" order by LTRIM(%1) COLLATE NOCASE ").arg(item);
        }
    }
    else
    //modified by honggi.shin, 2013.12.17, change orderby priority to show numbers later
    //return QString(" order by LTRIM(%1) COLLATE NOCASE ").arg(item);
    {
        return QString(" order by (CASE "
                       "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '!' AND '/' THEN 3 "
                       "WHEN substr(LTRIM(%1), 1, 1) BETWEEN ':' AND '@' THEN 3 "
                       "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '[' AND '`' THEN 3 "
                       "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '{' AND '~' THEN 3 "
                       "WHEN substr(LTRIM(%1), 1, 1) BETWEEN '0' AND '9' THEN 4 "
                       "ELSE 1 END), LTRIM(%1) COLLATE NOCASE ").arg(item);
    }
    //modified by honggi.shin, 2013.12.17 END

//} modified by jaehwan
}
//}changed by junam


void CMediaDataProvider::DeleteLimitCache(ETrackerAbstarctor_VolumeType  volumeType)   // { added by sungha.choi 2013.07.30
{
    LOG_TRACE << "DeleteLimitCache volumeType = " << volumeType << LOG_ENDL;
    if(savedSongVolumeType == volumeType) {
        savedSongSearchData.clear();
        savedSongVolumeType = eDefaultVolume;
    }
    if(savedArtistVolumeType == volumeType) {
        savedArtistSearchData.clear();
        savedArtistVolumeType = eDefaultVolume;
    }
    if(savedAlbumVolumeType == volumeType) {
        savedAlbumSearchData.clear();
        savedAlbumVolumeType = eDefaultVolume;
    }
}   // } added by sungha.choi 2013.07.30
// added by jawhean.lee for audio list sorting issue. 13.10. 02
void CMediaDataProvider::SetLanguage(int lang)
{
    LOG_TRACE << "CMediaDataProvider::SetLanguage lang="<<lang << " m_lang = "<<m_lang << LOG_ENDL; 
    char addr[32]; 
    sprintf(addr, "%p", this); 

    LOG_TRACE << "this  = "<< QString(addr) << LOG_ENDL; 

    if (m_lang != lang)  {
        savedSongSearchData.clear(); 
        savedArtistSearchData.clear(); 
        savedAlbumSearchData.clear(); 
    }
    
    m_lang = lang; 
}

//{ added by jaehwan.lee 2014.01.02 for pinyin sorting
void CMediaDataProvider::reorderListForChina(QVector<QStringList> &fileList, int indexColumn)
{
    QVector<QStringList> rowsByAlphabet[26];
    QVector<QStringList> numAndSymbols;
    QVector<QStringList> etcLists;

    for(int i=0; i < 26 ; i++)
        rowsByAlphabet[i].clear();

    numAndSymbols.clear();
    etcLists.clear();

    foreach (QStringList list, fileList)
    {
        QChar indexChar = CTrackerAbstractor::GetIndexChar(list[indexColumn], false, GetLanguage() == LANGUAGE_ZH_CMN);
        wchar_t wIndexChar = indexChar.unicode();
        if( wIndexChar == '#' || wIndexChar == ' ') //added by jaehwan.lee 2014.1.28 to fix "unknown aritsts" sorting issue in china
            numAndSymbols.append(list);
        else if (wIndexChar >= 'A' && indexChar <='Z')
            rowsByAlphabet[char(wIndexChar) - 'A'].append(list);
        else
            etcLists.append(list);
    }
    fileList.clear();
    fileList += numAndSymbols;

    for(int i=0; i < 26 ; i++)
        fileList += rowsByAlphabet[i];

    fileList += etcLists;
}
//}added by jaehwan.lee


//End of file
