/*
******************************************************************
*        COPYRIGHT © Teleca AB      *
*----------------------------------------------------------------*
* MODULE     :  DHAVN_TrackerAbstractor                             *
*
* PROGRAMMER :  Divya DSouza                                 *
* DATE       :  11 December 2012                                       *
* VERSION    :  1.0                                              *
*
*----------------------------------------------------------------*
*                                                                *
* MODULE SUMMARY : This file defines Apis which interact with    *
*                  Gracenote database using SQL query to retrieve media related  *
*                  information (e.g Artist, Album, Genre etc)    *
*                                                                *
*----------------------------------------------------------------*
*
* MODIFICATION RECORDS :                                         *
*
* VERSION   DATE                     PROGRAMMER              COMMENT
******************************************************************
* 1.0       11 December 2012  Divya DSouza              Draft version
******************************************************************
*/

#include "DHAVN_GracenoteDataProvider.h"
#include <QSparqlResult>
#include <QSparqlQuery>
#include <QSparqlConnection>
#include <QSparqlError>
#include <QUrl>
#include <QtSql>

CGracenoteDataProvider::CGracenoteDataProvider()
{
 //    LOG_INIT_V2("Media_TrackerAbstractor")
     m_CID = 0;
     m_Query_Offset = QUERY_OFFSET;
     m_Query_Limit  = QUERY_LIMIT;
	 
	 m_isCDInserted = false; // added by eugene.seo 2013.03.27 for static test
     m_IPOD_FRONT = false; // added by eugene.seo 2013.03.27 for static test
     m_IPOD_REAR = false; // added by eugene.seo 2013.03.27 for static test

    /* Dummy initilization of QStringlist*/
    for(int i=0;i<=EVOLUME_TYPE_SIZE;i++)
    {
        m_DeviceId << "Empty" ;
        m_DeviceURL << "Empty";
    }
	
    m_strMountPath.clear(); // added by eugene.seo 2013.05.13
}


CGracenoteDataProvider::~CGracenoteDataProvider()
{
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetCIDForVolume(ETrackerAbstarctor_VolumeType  volumeType)

// Explanation
// This method gets the CID of the connected device
// It connects signal/slot for the query completion

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
int CGracenoteDataProvider::GetCIDForVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    ////LOG_TRACE << "CGracenoteDataProvider::GetCIDForVolume Start" << volumeType << LOG_ENDL;

    bool query_status = false;
    int nstored_CID = 0;
	
	QSqlDatabase sqlitedb;
	QString path;
    path.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change
	
	QString connectionName;
	connectionName.append("CID");
	
    if(CreateDBConnection(sqlitedb, path, connectionName))
    {
		////LOG_TRACE << "CGracenoteDataProvider::GetCIDForVolume inside CreateDBConnection" << LOG_ENDL;
        QSqlQuery* query = new QSqlQuery(sqlitedb);

            switch(volumeType)
            {
            case eUSB:
            case eUSB_FRONT:
            case eUSB_REAR:
            case eIPod:
            case eIPOD_FRONT:
            case eIPOD_REAR:
                {
                    ////LOG_TRACE << "CGracenoteDataProvider::GetCIDForVolume Start inside USB/IPOD" << LOG_ENDL;
                    //Get the Collection ID created for this Device ID
                    QString DeviceID = getDeviceID(volumeType);
					////LOG_TRACE << "CGracenoteDataProvider::GetCIDForVolume DeviceID" << DeviceID << LOG_ENDL;

                    int connectStatus = 1;
                    query->prepare("SELECT CID FROM CollectionID_Data WHERE SerialNum = ?");
                    query->addBindValue(DeviceID);
                    query_status = query->exec();

                    break;
                }
            case eJukeBox:
                {
					////LOG_TRACE << "CGracenoteDataProvider::GetCIDForVolume Start inside eJukeBox" << LOG_ENDL;
                    query->prepare("SELECT CID FROM CollectionID_Data WHERE Volume = ?");
                    query->addBindValue(0); //Enum value of JukeBox in Gracenote is 0
                    query_status = query->exec();

                    break;
                }
            }

            if(query_status)
            {
                ////LOG_TRACE <<("CGracenoteDataProvider::GetCIDForVolume query passed") << LOG_ENDL;
                while (query->next())
                {
                    nstored_CID = query->value(0).toInt();

                    if(nstored_CID > 0)
                    {
                         ////LOG_TRACE <<("CGracenoteDataProvider::GetCIDForVolume CID to open = ");
                         ////LOG_TRACE << nstored_CID << LOG_ENDL;
                        break;
                    }
                }
            }
            delete query;
            sqlitedb.close();
        }
    

 return nstored_CID;
}

/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllSongsByComposerAlbum(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)

// Explanation
// This method gets the album name based on Genre and Composer
// It connects signal/slot for the query completion

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllSongsByComposerAlbum(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{
    ////LOG_TRACE << "CGracenoteDataProvider::GetAllSongsByComposerAlbum" <<LOG_ENDL;
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    ////LOG_TRACE << "CGracenoteDataProvider::GetAllSongsByComposerAlbum m_CID" << m_CID << LOG_ENDL;
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        ////LOG_TRACE << "CGracenoteDataProvider::GetAllSongsByComposerAlbum CreateDBConnection successful" << LOG_ENDL;

        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString queryString;
        queryString.append( QString("select DISTINCT  T_FilePath ,T_Title from %4 where T_Composer = \"%1\" AND T_Album = \"%2\" AND T_Title not like \"%3\" order by T_Title ").arg(searchList[0]).arg(searchList[1]).arg("").arg(CID_DB));

        ////LOG_TRACE << "CGracenoteDataProvider::GetAllSongsByComposerAlbum queryString" << queryString << LOG_ENDL;
        int noOfColumns = GETALLSONGSBYCOMPOSERALBUM;

        ret = senddata(noOfColumns,eGetAllSongsByComposerAlbumQuery_Id ,queryString, normalized_DB);
        normalized_DB.close();
    }
    else
    {
        ////LOG_TRACE << "CGracenoteDataProvider::GetAllSongsByComposerAlbum CreateDBConnection failed" << LOG_ENDL;
    }

    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllAlbumByGenreComposer(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)

// Explanation
// This method gets the album name based on Genre and Composer
// It connects signal/slot for the query completion

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool  CGracenoteDataProvider::GetAllAlbumByGenreComposer(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{
    ////LOG_TRACE << "CGracenoteDataProvider::GetAllAlbumByGenreComposer" <<LOG_ENDL;
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        ////LOG_TRACE << "CGracenoteDataProvider::GetAllAlbumByGenreComposer CreateDBConnection successful" << LOG_ENDL;

        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString queryString;
        queryString.append(QString("select DISTINCT  T_Album from %4 where T_Genre = \"%1\" AND T_Composer = \"%2\" AND T_Album not like \"%3\" order by T_Album ").arg(searchList[0]).arg(searchList[1]).arg("").arg(CID_DB));

        ////LOG_TRACE << "CGracenoteDataProvider::GetAllAlbumByGenreComposer queryString" << queryString << LOG_ENDL;
        int noOfColumns = GETALLALBUMBYGENERECOMPOSER;

        ret = senddata(noOfColumns,eGetAllAlbumByGenreComposerQuery_Id ,queryString, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllAlbumByComposer(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)

// Explanation
// This method gets the album name based on Genre and Composer
// It connects signal/slot for the query completion

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllAlbumByComposer(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{
    ////LOG_TRACE << "CGracenoteDataProvider::GetAllAlbumByComposer" <<LOG_ENDL;
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        ////LOG_TRACE << "CGracenoteDataProvider::GetAllAlbumByComposer CreateDBConnection successful" << LOG_ENDL;

        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString queryString;
        queryString.append(QString("select DISTINCT  T_Album from %3 where T_Composer = \"%1\" AND T_Album not like \"%2\" order by T_Album ").arg(searchList[0]).arg("").arg(CID_DB));

        int noOfColumns = GETALLALBUMBYCOMPOSER;

        ret = senddata(noOfColumns,eGetAllAlbumByComposerQuery_Id ,queryString, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllComposerByGenre(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)

// Explanation
// This method gets the album name based on Genre and Composer
// It connects signal/slot for the query completion

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllComposerByGenre(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{
    ////LOG_TRACE << "CGracenoteDataProvider::GetAllComposerByGenre" <<LOG_ENDL;
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        ////LOG_TRACE << "CGracenoteDataProvider::GetAllComposerByGenre CreateDBConnection successful" <<LOG_ENDL;

        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString queryString;
        queryString.append(QString("select DISTINCT T_Composer from %3 where T_Genre = \"%1\" AND T_Composer not like \"%2\" order by T_Composer ").arg(searchList[0]).arg("").arg(CID_DB));

        int noOfColumns = GETALLCOMPOSERBYGENER;
        ret = senddata(noOfColumns,eGetAllComposerByGenreQuery_Id ,queryString, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllSongsByGenreArtistAlbum(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)

// Explanation
// This method gets the album name based on Genre and Composer
// It connects signal/slot for the query completion

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllSongsByGenreArtistAlbum(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString queryString;
        queryString.append( QString("select T_Title ,T_FilePath from %5 where T_Genre = \"%1\" AND  T_Artist = \"%2\" AND  T_Album = \"%3\" AND T_Title not like \"%4\" order by T_Title").arg(searchList[0]).arg(searchList[1]).arg(searchList[2]).arg("").arg(CID_DB));

        int noOfColumns = GETALLSONGSBYGENREARTISTALBUMQUERY_DATATYPECOUNT;

        ret = senddata(noOfColumns,eGetAllSongsByGenreArtistAlbumQuery_Id ,queryString, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}




/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllSongsByGenreComposer(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)

// Explanation
// This method gets the album name based on Genre and Composer
// It connects signal/slot for the query completion

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllSongsByGenreComposerAlbum(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString queryString;
        queryString.append(QString("select T_Title ,T_FilePath from %5 where T_Genre = \"%1\" AND T_Composer = \"%2\" AND  T_Album = \"%3\" AND T_Title not like \"%4\" order by T_Title").arg(searchList[0]).arg(searchList[1]).arg(searchList[2]).arg("").arg(CID_DB));

        int noOfColumns = GETALLSONGSBYGENRECOMPOSERALBUMQUERY_DATATYPECOUNT;
        ret = senddata(noOfColumns,eGetAllSongsByGenreComposerAlbumQuery_Id ,queryString, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllSongsByArtistAlbum(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)

// Explanation
// This method gets the album name based on Genre and Composer
// It connects signal/slot for the query completion

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllSongsByArtistAlbum(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString queryString;
        queryString.append( QString("select DISTINCT T_FilePath ,T_Title from %4 where T_Artist = \"%1\" AND T_Album = \"%2\" AND T_Title not like \"%3\" order by T_Title").arg(searchList[0]).arg(searchList[1]).arg("").arg(CID_DB));

        int noOfColumns = GETALLSONGSBYARTISTALBUMQUERY_DATATYPECOUNT;

        ret = senddata(noOfColumns,eGetAllSongsByArtistAlbumQuery_Id ,queryString, normalized_DB);
        normalized_DB.close();
    }
    return ret;
  }


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetMusicAlbumsByGenreArtist(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)

// Explanation
// This method gets the album name based on Genre and Composer
// It connects signal/slot for the query completion

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetMusicAlbumsByGenreArtist(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString queryString;
        queryString.append(QString("select DISTINCT T_Album from %4 where T_Genre = \"%1\" AND T_Artist = \"%2\" AND T_Album not like \"%3\" order by T_Album").arg(searchList[0]).arg(searchList[1]).arg("").arg(CID_DB));

        int noOfColumns = GETALLALBUMBYGENREARTISTQUERY_DATATYPECOUNT;
        ret = senddata(noOfColumns,eGetAllAlbumByGenreArtistQuery_Id ,queryString, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetMusicAlbumsByArtist(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)

// Explanation
// This method gets the album name based on Genre and Composer
// It connects signal/slot for the query completion

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetMusicAlbumsByArtist(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString queryString;
        queryString.append(QString("select DISTINCT T_Album from %3 where T_Artist = \"%1\" AND T_Album not like \"%2\" order by T_Album ").arg(searchList[0]).arg("").arg(CID_DB));
        int noOfColumns = GETALLALBUMBYARTISTQUERY_DATATYPECOUNT;

        ret = senddata(noOfColumns,eGetAllAlbumByArtistQuery_Id ,queryString, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetMusicArtistsByGenre(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)

// Explanation
// This method gets the album name based on Genre and Composer
// It connects signal/slot for the query completion

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetMusicArtistsByGenre(QStringList searchList, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString queryString;
        queryString.append( QString("select DISTINCT T_Artist from %3 where T_Genre = \"%1\" AND T_Artist not like \"%2\" order by T_Artist").arg(searchList[0]).arg("").arg(CID_DB));

        int noOfColumns = GETALLARTISTBYGENREQUERY_DATATYPECOUNT;
        ret = senddata(noOfColumns,eGetAllArtistByGenreQuery_Id ,queryString, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllAlbumsWithCoverArtIndexFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType)

// Explanation
// This method gets the album name based on Genre and Composer
// It connects signal/slot for the query completion

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllAlbumsWithCoverArtIndexFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType)
{
    //DONE
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString queryString;
        queryString.append(QString("select DISTINCT T_Album from %2 where T_Album not like \"%1\" order by LTRIM(T_Album) COLLATE NOCASE").arg("").arg(m_CID));
        int noOfColumns = GETALLALBUMSWITHCOVERARTINDEXFROMVOLUMEQUERY_DATATYPECOUNT;

        m_Letter_GetAllAlbumsWithCoverArtIndexFromVolume = searchData;
        ret = senddata(noOfColumns,eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id ,queryString, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllMetaDataWithLimitFromVolume(ETrackerAbstarctor_OrderType orderType, ETrackerAbstarctor_VolumeType volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllMetaDataWithLimitFromVolume(ETrackerAbstarctor_OrderType orderType, ETrackerAbstarctor_VolumeType volumeType)
{
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

    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString queryString;
        queryString.append(QString("select T_Title, T_FilePath, T_Album, T_Artist, T_Genre, T_Composer FROM %4 where T_Album not like \"%1\" order by LTRIM(%2) COLLATE NOCASE LIMIT 1 OFFSET %3").arg("").arg(useOrderType).arg(m_Query_Offset).arg(CID_DB));

        int noOfColumns = GETALLMETADATAWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
        ret= senddata(noOfColumns,eGetAllMetaDataWithLimitFromVolumeQuery_Id ,queryString, normalized_DB);
         normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllSongsFromVolume(ETrackerAbstarctor_VolumeType volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllSongsFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    ////LOG_TRACE << "CGracenoteDataProvider::GetAllSongsFromVolume" <<LOG_ENDL;

    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        //LOG_TRACE << "CGracenoteDataProvider::GetAllSongsFromVolume CreateDBConnection successful" <<LOG_ENDL;
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString queryString;
        queryString.append(QString("select T_FilePath,T_Title from %2 where T_Title not like \"%1\" order by LTRIM(T_Title) COLLATE NOCASE ").arg("").arg(CID_DB));

        int noOfColumns = GETALLSONGSFROMVOLUMEQUERY_DATATYPECOUNT;

        ret=  senddata(noOfColumns,eGetAllSongsFromVolumeQuery_Id ,queryString, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllSongsWithTitleCountFromVolume(ETrackerAbstarctor_VolumeType volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllSongsWithTitleCountFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select count(T_Title) FROM %1").arg(CID_DB));

        int noOfColumns = GETALLSONGSWITHTITLECOUNTFROMVOLUMEQUERY_DATATYPECOUNT;
        ret= senddata(noOfColumns,eGetAllSongsWithTitleCountFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllSongsWithLimitFromVolume(ETrackerAbstarctor_VolumeType volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllSongsWithLimitFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select T_FilePath,T_Title from %4 where T_Title not like \"%1\" order by LTRIM(T_Title) COLLATE NOCASE LIMIT %2 OFFSET %3 ").arg("").arg(m_Query_Limit).arg(m_Query_Offset).arg(CID_DB));

        int noOfColumns = GETALLSONGSWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllSongsWithLimitFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllSongsWithLimitFromVolume(ETrackerAbstarctor_VolumeType volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllAlbumsWithLimitFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    //DONE
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select DISTINCT T_Album ,T_Artist from %4 where T_Album not like \"%1\" order by LTRIM(T_Album) COLLATE NOCASE LIMIT %2 OFFSET %3").arg("").arg(m_Query_Limit).arg(m_Query_Offset).arg(CID_DB));

        int noOfColumns = GETALLALBUMSWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllAlbumsWithLimitFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllMusicArtistsWithLimitFromVolume(ETrackerAbstarctor_VolumeType volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllMusicArtistsWithLimitFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select DISTINCT T_Artist from %4 where T_Artist not like \"%1\" order by LTRIM(T_Artist) COLLATE NOCASE LIMIT %2 OFFSET %3 ").arg("").arg(m_Query_Limit).arg(m_Query_Offset).arg(CID_DB));

        int noOfColumns = GETALLMUISCARTISTSWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllMusicArtistsWithLimitFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllMusicComposersWithLimitFromVolume(ETrackerAbstarctor_VolumeType volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllMusicComposersWithLimitFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select DISTINCT T_Composer from %4 where T_Composer not like \"%1\" order by LTRIM(T_Composer) COLLATE NOCASE LIMIT %2 OFFSET %3").arg("").arg(m_Query_Limit).arg(m_Query_Offset).arg(CID_DB));

        int noOfColumns = GETALLMUSICCOMPOSERSWITHLIMITFORMVOLUMEQUERY_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllMusicComposersWithLimitFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllMusicGenresWithLimitFromVolume(ETrackerAbstarctor_VolumeType volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllMusicGenresWithLimitFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    //LOG_TRACE <<"CGracenoteDataProvider::GetAllMusicGenresWithLimitFromVolume"<< LOG_ENDL;
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select DISTINCT T_Genre from %4 where T_Genre not like \"%1\" order by LTRIM(T_Genre) COLLATE NOCASE LIMIT %2 OFFSET %3 ").arg("").arg(m_Query_Limit).arg(m_Query_Offset).arg(CID_DB));

        int noOfColumns = GETALLMUSICGENRESWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllMusicGenresWithLimitFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllAlbumsWithCoverArtWithLimitFromVolume(ETrackerAbstarctor_VolumeType volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllAlbumsWithCoverArtWithLimitFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    //LOG_TRACE <<"CGracenoteDataProvider::GetAllAlbumsWithCoverArtWithLimitFromVolume"<< LOG_ENDL;
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select DISTINCT T_Album ,T_Artist from %4 where T_Album not like \"%1\" group by T_Album order by LTRIM(T_Album) COLLATE NOCASE LIMIT %2 OFFSET %3").arg("").arg(m_Query_Limit).arg(m_Query_Offset).arg(CID_DB));

        int noOfColumns = GETALLALBUMSWITHCOVERARTWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllAlbumsWithCoverArtFromVolume(ETrackerAbstarctor_VolumeType  volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllAlbumsWithCoverArtFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select DISTINCT T_Album ,T_Artist from %2 where T_Album not like \"%1\"  order by LTRIM(T_Album) COLLATE NOCASE ").arg("").arg(CID_DB));

        int noOfColumns = GETALLALBUMSWITHCOVERARTFROMVOLUMEQUERY_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllAlbumsWithCoverArtFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}

/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllAlbumsFromVolume(ETrackerAbstarctor_VolumeType  volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllAlbumsFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select DISTINCT T_Album ,T_Artist from %2 where T_Album not like \"%1\"  order by LTRIM(T_Album) COLLATE NOCASE").arg("").arg(CID_DB));

        int noOfColumns = GETALLALBUMSFROMVOLUMEQUERY_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllAlbumsFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllAlbumsCountFromVolume(ETrackerAbstarctor_VolumeType  volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllAlbumsCountFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select COUNT(DISTINCT T_Album) from %2 where T_Album not like \"%1\" ").arg("").arg(CID_DB));

        int noOfColumns = 1;
        ret=  senddata(noOfColumns,eGetAllAlbumsCountFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}

/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllMusicArtistsFromVolume(ETrackerAbstarctor_VolumeType  volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllMusicArtistsFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select DISTINCT T_Artist from %2 where T_Artist not like \"%1\"  order by LTRIM(T_Artist) COLLATE NOCASE").arg("").arg(CID_DB));

        int noOfColumns = GETALLMUISCARTISTSQUERY_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllMusicArtistsFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllMusicComposersFromVolume(ETrackerAbstarctor_VolumeType  volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllMusicComposersFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select DISTINCT T_Composer from %2 where T_Composer not like \"%1\" order by LTRIM(T_Composer) COLLATE NOCASE").arg("").arg(CID_DB));

        int noOfColumns = GETALLMUSICCOMPOSERSQUERY_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllMusicComposersFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}

/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllMusicGenresFromVolume(ETrackerAbstarctor_VolumeType  volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllMusicGenresFromVolume(ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select DISTINCT T_Genre from %2 where T_Genre not like \"%1\" order by LTRIM(T_Genre) COLLATE NOCASE").arg("").arg(CID_DB));

        int noOfColumns = GETALLMUSICGENRESQUERY_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllMusicGenresFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}

/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::senddata(int noOfColumns,ETrackerAbstarctor_Query_Ids queryId ,QString querystring)

// Explanation
// This method gets the CID of the connected device
// It connects signal/slot for the query completion

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::senddata(int noOfColumns,ETrackerAbstarctor_Query_Ids queryId ,QString querystring , QSqlDatabase &normalized_DB)
{
    //LOG_TRACE <<"CGracenoteDataProvider::senddata"<< queryId << LOG_ENDL;

    QVector<QStringList> queryDataList;
    bool  ret=false ;
    QSqlQuery* query = new QSqlQuery(querystring,normalized_DB);
    //LOG_TRACE <<"CGracenoteDataProvider::senddata querystring"<< querystring << LOG_ENDL;
    ret = query->exec();

    //LOG_TRACE <<"CGracenoteDataProvider::senddata query->exec()"<< query->lastError().text() << LOG_ENDL;

    //if(ret)
        //LOG_TRACE <<"CGracenoteDataProvider::senddata query passed" << LOG_ENDL;
    //else
        //LOG_TRACE <<"CGracenoteDataProvider::senddata query failed !!!" << LOG_ENDL;

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
        //LOG_TRACE <<"CGracenoteDataProvider::senddata default case" << LOG_ENDL;
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
                    //LOG_TRACE <<"CGracenoteDataProvider::senddata value" << value << LOG_ENDL;

                    // qDebug() << "  value"<<value;
                    columnList.append(QUrl::fromPercentEncoding(value.toUtf8()));
                }
                queryDataList.append(columnList);

            }
            //LOG_TRACE <<"CGracenoteDataProvider::senddata emit DataFeched" << LOG_ENDL;
            emit DataFeched(queryDataList, queryId);

        }
    }

    }

    if( (queryId == eGetAllSongsWithTitleIndexFromVolumeQuery_Id) ||
            (queryId == eGetAllMusicArtistsIndexFromVolumeQuery_Id)   ||
            (queryId == eGetAllMusicGenresIndexFromVolumeQuery_Id)    ||
            (queryId == eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id) )
    {
        //PrepareIndexDataList(query, queryDataList, queryId, letter, true);
    }

    delete query;
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllsongsByKeywordFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType)





// Explanation




//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllsongsByKeywordFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType)
{
    //LOG_TRACE << "CGracenoteDataProvider::GetAllsongsByKeywordFromVolume Start searchData" << searchData << LOG_ENDL;
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
	//LOG_TRACE << "CGracenoteDataProvider::GetAllsongsByKeywordFromVolume volumeType m_CID" << volumeType << m_CID << LOG_ENDL;
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
	    //LOG_TRACE << "CGracenoteDataProvider::GetAllsongsByKeywordFromVolume inside CreateDBConnection(normalized_DB)" << LOG_ENDL;
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select T_Title, T_Album ,T_Artist ,T_Genre, T_FilePath from %2 where T_Title  like  '%%1%' order by T_Title ").arg(searchData).arg(CID_DB));

        int noOfColumns = GETALLSONGSBYKEYWORDFROMVOLUME_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllsongsByKeywordFromVolumeQuery_Id ,Querystring,normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllAlbumsByKeywordFromVolume(ETrackerAbstarctor_VolumeType  volumeType, int query_limit, int query_offset)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllAlbumsByKeywordFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select T_Title, T_Album ,T_Artist ,T_Genre, T_FilePath from %1 where T_Album  like  '%%2%' order by T_Album").arg(CID_DB).arg(searchData));

        int noOfColumns = GETALLALBUMSYKEYWORDFROMVOLUME_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllAlbumsByKeywordFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllArtistsByKeywordFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllArtistsByKeywordFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select T_Title, T_Album ,T_Artist ,T_Genre, T_FilePath from %2 where T_Artist  like  '%%1%' order by T_Artist").arg(searchData).arg(CID_DB));

        int noOfColumns = GETALLARTISTBYKEYWORDFROMVOLUME_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllArtistsByKeywordFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::senddata(int noOfColumns,ETrackerAbstarctor_Query_Ids queryId ,QString querystring)

// Explanation
// This method gets the CID of the connected device
// It connects signal/slot for the query completion

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllGenresByKeywordFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select T_Title, T_Album ,T_Artist ,T_Genre, T_FilePath from %1 where T_Genre  like  '%%2%' order by T_Genre").arg(CID_DB).arg(searchData));

        int noOfColumns = GETALLGENREBYKEYWORDFROMVOLUME_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllGenresByKeywordFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllSongsByAlbumFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType)

// Explanation
//

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllSongsByAlbumFromVolume(QString searchData, ETrackerAbstarctor_VolumeType  volumeType)
{
	//LOG_TRACE << "CGracenoteDataProvider::GetAllSongsByAlbumFromVolume Start searchData " << searchData << LOG_ENDL;
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
	//LOG_TRACE << "CGracenoteDataProvider::GetAllSongsByAlbumFromVolume m_CID" << m_CID << LOG_ENDL;
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
		//LOG_TRACE << "CGracenoteDataProvider::GetAllSongsByAlbumFromVolume CreateDBConnection" << LOG_ENDL;
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select T_Title,T_FilePath from %2 where T_Album like \"%1\" order by T_Title ").arg(searchData).arg(CID_DB));
		//LOG_TRACE << "CGracenoteDataProvider::GetAllSongsByAlbumFromVolume Querystring" << Querystring << LOG_ENDL;

        int noOfColumns = GETALLSONGSBYCOMPOSERALBUM;
        ret=  senddata(noOfColumns,eGetAllSongsByAlbumFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}

/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllSongsByArtistFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType)

// Explanation
//

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllSongsByArtistFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select DISTINCT  T_Title , T_FilePath from %1 where T_Artist = \"%2\" AND T_Title not like \"%3\" order by T_Title").arg(CID_DB).arg(searchData).arg(""));

        int noOfColumns = GETALLSONGSBYARTISTFROMVOLUMEQUERY_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllSongsByArtistFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllMusicGenresIndexFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType)

// Explanation
//

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllMusicGenresIndexFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType)
{
    //DONE
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString Querystring;
        Querystring.append(QString("select DISTINCT T_Genre from MasterTable where T_Genre not like \"%1\" order by LTRIM(T_Genre) COLLATE NOCASE ").arg("").arg(m_CID));

        int noOfColumns = GETALLMUSICGENRESFROMVOLUMEQUERY_DATATYPECOUNT;
        m_Letter_GetAllMusicGenresIndexFromVolume = searchData;
        ret=  senddata(noOfColumns,eGetAllMusicGenresIndexFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllSongsWithoutAlbumFromVolume(ETrackerAbstarctor_VolumeType volumeType)

// Explanation
//

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllSongsWithoutAlbumFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select T_FilePath,T_Title FROM %2 where T_Album like \"%1\" order by LTRIM(T_Title) COLLATE NOCASE").arg("").arg(CID_DB));

        int noOfColumns = GETALLSONGSBYARTISTFROMVOLUMEQUERY_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllSongsWithoutAlbumFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllAlbumsWithUrlFromVolume(ETrackerAbstarctor_VolumeType volumeType)

// Explanation
//

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllAlbumsWithUrlFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    //LOG_TRACE << "CGracenoteDataProvider::GetAllAlbumsWithUrlFromVolume Start" << LOG_ENDL;
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
	//LOG_TRACE << "CGracenoteDataProvider::GetAllAlbumsWithUrlFromVolume m_CID" << m_CID << LOG_ENDL;
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
	    //LOG_TRACE << "CGracenoteDataProvider::GetAllAlbumsWithUrlFromVolume inside CreateDBConnection(normalized_DB)" << LOG_ENDL;
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select T_Album ,T_Artist,T_FilePath from %2 where T_Album not like \"%1\" order by LTRIM(T_Album) COLLATE NOCASE").arg("").arg(CID_DB));
   
        //LOG_TRACE << "CGracenoteDataProvider::GetAllAlbumsWithUrlFromVolume Querystring" << Querystring << LOG_ENDL;
        int noOfColumns = GETALLALBUMSWITHURLFROMVOLUMEQUERY_DATATYPECOUNT;
        ret=  senddata(noOfColumns,eGetAllAlbumsWithUrlFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllMusicArtistsCountFromVolume(ETrackerAbstarctor_VolumeType volumeType)

// Explanation
//

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllMusicArtistsCountFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select COUNT(DISTINCT T_Artist) from %1").arg(CID_DB));

        int noOfColumns = 1;
        ret=  senddata(noOfColumns,eGetAllMusicArtistsCountFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllMusicArtistsCountFromVolume(ETrackerAbstarctor_VolumeType volumeType)

// Explanation
//

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllMusicComposersCountFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select COUNT(DISTINCT T_Composer) from %1").arg(CID_DB));

        int noOfColumns = 1;
        ret=  senddata(noOfColumns,eGetAllMusicComposersCountFromVolumeQuery_Id ,Querystring, normalized_DB);

        normalized_DB.close();
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAllMusicArtistsCountFromVolume(ETrackerAbstarctor_VolumeType volumeType)

// Explanation
//

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllMusicGenresCountFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select COUNT(DISTINCT T_Genre) from %1").arg(CID_DB));

        int noOfColumns = 1;
        ret=  senddata(noOfColumns,eGetAllMusicGenresCountFromVolumeQuery_Id ,Querystring, normalized_DB);
        normalized_DB.close();
    }
    return ret;
}
/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::CreateDBConnection(QSqlDatabase &normalized_DB)

// Explanation
// This method creates connection to the Gracenote's normalized DB

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAllSongsWithTitleIndexFromVolume()
{
    bool ret = false; // added by eugene.seo 2013.03.27 for static test
  
  /*  
    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString Querystring;
        Querystring.append(QString("select T_Title from MasterTable where T_Title not like \"%1\" order by LTRIM(T_Title) COLLATE NOCASE ").arg("").arg(m_CID));

        int noOfColumns = GETALLSONGSWITHTITLEINDEXFROMVOLUMEQUERY_DATATYPECOUNT;
        m_Letter_GetAllSongsWithTitleIndexFromVolume = searchData;
        ret=  senddata(noOfColumns,eGetAllSongsWithTitleIndexFromVolumeQuery_Id ,Querystring);
        normalized_DB.close();
    }
    */
    return ret; // added by eugene.seo 2013.03.27 for static test
}





/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::CreateDBConnection(QSqlDatabase &normalized_DB)

// Explanation
// This method creates connection to the Gracenote's normalized DB

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::CreateDBConnection(QSqlDatabase &normalized_DB)
 {
     //LOG_TRACE << "CGracenoteDataProvider::CreateDBConnection Start" << LOG_ENDL;
     //Get the list of collection IDs for the volume for which the devices are connected
     bool ret = false;

     if(m_CID <= 0)
     {
         //LOG_TRACE << "CGracenoteDataProvider::CreateDBConnection m_CID" << m_CID << LOG_ENDL;
         return ret;
     }

     QString CID_DB;
     CID_DB.clear();
     CID_DB.append("CID_");
     CID_DB.append(QString("%1").arg(m_CID));

     //LOG_TRACE << "CGracenoteDataProvider::CreateDBConnection CID_DB" << CID_DB << LOG_ENDL;

    /* if(QSqlDatabase::contains(CID_DB))
     {
         QSqlDatabase::removeDatabase(CID_DB);
     }*/

    // normalized_DB = QSqlDatabase::addDatabase(CID_DB);

     QString fsdbName;
     fsdbName.append("/app/data/gracenote/"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

     fsdbName.append(CID_DB);

     QString tmpfsdbName;
     tmpfsdbName.append("/dev/shm/");
     tmpfsdbName.append(CID_DB);

     QString dbPath;

     if (QFile::exists(tmpfsdbName))
     {
         //LOG_TRACE << "CGracenoteDataProvider::CreateDBConnection CID_DB exists in tmpfs" << tmpfsdbName << LOG_ENDL;
         dbPath.append(tmpfsdbName);
     }
     else if (QFile::exists(fsdbName))
     {
         //LOG_TRACE << "CNormalizedData : CreateSQLiteDB CID_DB exists in FS" << fsdbName << LOG_ENDL;
         dbPath.append(fsdbName);
     }

     if(QSqlDatabase::contains(CID_DB))
     {
         //LOG_TRACE << "::createConnection: already have this connection name"<< LOG_ENDL;
         normalized_DB = QSqlDatabase::database(CID_DB);
         normalized_DB.setDatabaseName(dbPath);
         if(normalized_DB.isOpen())
         {
             //LOG_TRACE << "::createConnection: already opened successfully"<< LOG_ENDL;
             ret = true;
         }
         else
         {
             if (!normalized_DB.open())
             {
                 //LOG_TRACE << "::createConnection: not opened successfully"<< LOG_ENDL;
                 ret = false;
             }
             else
             {
                 //LOG_TRACE << "::createConnection: opened successfully"<< LOG_ENDL;
                 ret = true;
             }
         }
     }
     else
     {
         //LOG_TRACE << "::createConnection: not already have this connection name"<< LOG_ENDL;
         normalized_DB = QSqlDatabase::addDatabase("QSQLITE",CID_DB);
         normalized_DB.setDatabaseName(dbPath);
         if (!normalized_DB.open())
         {
             //LOG_TRACE << "::createConnection: not opened successfully"<< LOG_ENDL;
             ret = false;
         }
         else
         {
             //LOG_TRACE << "::createConnection: opened successfully"<< LOG_ENDL;
             ret = true;
         }
     }
     return ret;
 }



/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::CreateDBConnection(QSqlDatabase &normalized_DB)

// Explanation
// This method creates connection to the Gracenote's normalized DB

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::CreateDBConnection(QSqlDatabase &sqlitedb, QString path, QString connectionName)
 {
     bool ret = false;
     if(QSqlDatabase::contains("CID"))
     {
         //LOG_TRACE << "::createConnection: already have this connection name"<< LOG_ENDL;
         sqlitedb = QSqlDatabase::database(connectionName);
         sqlitedb.setDatabaseName(path);
         if(sqlitedb.isOpen())
         {
             //LOG_TRACE << "::createConnection: already opened successfully"<< LOG_ENDL;
             ret = true;
         }
         else
         {
             if (!sqlitedb.open())
             {
                 //LOG_TRACE << "::createConnection: not opened successfully"<< LOG_ENDL;
                 ret = false;
             }
             else
             {
                 //LOG_TRACE << "::createConnection: opened successfully"<< LOG_ENDL;
                 ret = true;
             }
         }
     }
     else
     {
         //LOG_TRACE << "::createConnection: not already have this connection name"<< LOG_ENDL;
         sqlitedb = QSqlDatabase::addDatabase("QSQLITE",connectionName);
         sqlitedb.setDatabaseName(path);
         if (!sqlitedb.open())
         {
             //LOG_TRACE << "::createConnection: not opened successfully"<< LOG_ENDL;
             ret = false;
         }
         else
         {
             //LOG_TRACE << "::createConnection: opened successfully"<< LOG_ENDL;
             ret = true;
         }
     }
     return ret;
 }


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::RequestData(QStringList searchList, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::RequestData(QStringList searchList, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret=false;
    //LOG_TRACE << "CGracenoteDataProvider::RequestData" <<LOG_ENDL;

    switch (queryId)
    {
    case eGetAllSongsByComposerAlbumQuery_Id:
        {
            //LOG_TRACE << "CGracenoteDataProvider::RequestData case eGetAllSongsByComposerAlbumQuery_Id" <<LOG_ENDL;
            ret = GetAllSongsByComposerAlbum(searchList, volumeType);
        }
        break;
    case eGetAllAlbumByGenreComposerQuery_Id:
        {
            //LOG_TRACE << "CGracenoteDataProvider::RequestData case eGetAllAlbumByGenreComposerQuery_Id" <<LOG_ENDL;
            ret = GetAllAlbumByGenreComposer(searchList, volumeType);
        }
        break;
    case eGetAllAlbumByComposerQuery_Id:
        {
            //LOG_TRACE << "CGracenoteDataProvider::RequestData case eGetAllAlbumByGenreComposerQuery_Id" <<LOG_ENDL;
            ret = GetAllAlbumByComposer(searchList, volumeType);
        }
        break;
    case eGetAllComposerByGenreQuery_Id:
        {
            //LOG_TRACE << "CGracenoteDataProvider::RequestData case eGetAllComposerByGenreQuery_Id" <<LOG_ENDL;
            ret = GetAllComposerByGenre(searchList, volumeType);
        }
        break;
    case eGetAllSongsByGenreArtistAlbumQuery_Id:
        {
             //LOG_TRACE << "CGracenoteDataProvider::RequestData case eGetAllSongsByGenreArtistAlbumQuery_Id" <<LOG_ENDL;
            ret = GetAllSongsByGenreArtistAlbum(searchList, volumeType);
        }
        break;
    case eGetAllSongsByGenreComposerAlbumQuery_Id:
        {
            //LOG_TRACE << "CGracenoteDataProvider::RequestData case eGetAllSongsByGenreComposerAlbumQuery_Id" <<LOG_ENDL;
            ret = GetAllSongsByGenreComposerAlbum(searchList, volumeType);
        }
        break;
    case eGetAllSongsByArtistAlbumQuery_Id:
        {
            //LOG_TRACE << "CGracenoteDataProvider::RequestData case eGetAllSongsByArtistAlbumQuery_Id" <<LOG_ENDL;
            ret = GetAllSongsByArtistAlbum(searchList, volumeType);
        }
        break;
    case eGetAllAlbumByGenreArtistQuery_Id:
        {
            //LOG_TRACE << "CGracenoteDataProvider::RequestData case eGetAllAlbumByGenreArtistQuery_Id" <<LOG_ENDL;
            ret = GetMusicAlbumsByGenreArtist(searchList, volumeType);
        }
        break;
    case eGetAllAlbumByArtistQuery_Id:
        {
            //LOG_TRACE << "CGracenoteDataProvider::RequestData case eGetAllAlbumByGenreArtistQuery_Id" <<LOG_ENDL;
            ret = GetMusicAlbumsByArtist(searchList, volumeType);
        }
        break;
    case eGetAllArtistByGenreQuery_Id:
        {
            //LOG_TRACE << "CGracenoteDataProvider::RequestData case eGetAllAlbumByGenreArtistQuery_Id" <<LOG_ENDL;
            ret = GetMusicArtistsByGenre(searchList, volumeType);
        }
        break;
    case eGetMultipleGenreSongsFromVolumeQuery_Id:
    case eGetMultipleAlbumSongsFromVolumeQuery_Id:
    case eGetMultipleArtistSongsFromVolumeQuery_Id:
        {
            //LOG_TRACE << "CGracenoteDataProvider::RequestData Not implemented to get normalized data ........WARNING!!!" <<LOG_ENDL;
        }
        break;

    default:
        break;
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::RequestData(ETrackerAbstarctor_OrderType orderType, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::RequestData(ETrackerAbstarctor_OrderType orderType, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType)
{
	    bool ret = false; // added by eugene.seo 2013.03.27 for static test

         //LOG_TRACE << "CGracenoteDataProvider::RequestData " <<LOG_ENDL;
        switch (queryId)
        {
        case eGetAllSongsWithMetaDataFromVolumeQuery_Id:
            {
                //LOG_TRACE << "CGracenoteDataProvider::RequestData case eGetAllSongsWithMetaDataFromVolumeQuery_Id not implemented for normalized data .......WARNING !!!" <<LOG_ENDL;
            }
            break;
        case eGetAllMetaDataWithLimitFromVolumeQuery_Id:
            {
                //LOG_TRACE << "CGracenoteDataProvider::RequestData case eGetAllMetaDataWithLimitFromVolumeQuery_Id" <<LOG_ENDL;
                ret = GetAllMetaDataWithLimitFromVolume(orderType, volumeType); // modified by eugene.seo 2013.03.27 for static test
            }
            break;
        default:
            break;
        }

		return ret; // added by eugene.seo 2013.03.27 for static test
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::RequestData( ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType)

// Explanation


//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::RequestData( ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret=false;

    switch (queryId)
    {
    case eGetAllSongsFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllSongsFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllSongsFromVolume(volumeType);
        }
        break;
    case eGetAllAlbumsFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllAlbumsFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllAlbumsFromVolume(volumeType);
        }
        break;
    case eGetAllMusicArtistsFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllMusicArtistsFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllMusicArtistsFromVolume(volumeType);
        }
        break;
    case eGetAllMusicComposersFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllMusicComposersFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllMusicComposersFromVolume(volumeType);
        }
        break;
    case eGetAllMusicGenresFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllMusicGenresFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllMusicGenresFromVolume(volumeType);
        }
        break;
    case eGetAllAlbumsWithCoverArtFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllAlbumsWithCoverArtFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllAlbumsWithCoverArtFromVolume(volumeType);
        }
        break;
    case eGetAllSongsWithoutAlbumFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllSongsWithoutAlbumFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllSongsWithoutAlbumFromVolume(volumeType);
        }
        break;
    case eIsThereAnySongWithoutAlbumFromVolumeQuery_Id:
        {
            //Needs testing
			//LOG_TRACE << "CGracenoteDataProvider::Requestdata eIsThereAnySongWithoutAlbumFromVolumeQuery_Id" << LOG_ENDL;
            ret = IsThereAnySongWithoutAlbumFromVolume(volumeType);
        }
        break;
    case eGetAllAlbumsWithUrlFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllAlbumsWithUrlFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllAlbumsWithUrlFromVolume(volumeType);
        }
        break;
    case eGetAllSongsWithTitleCountFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllSongsWithTitleCountFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllSongsWithTitleCountFromVolume(volumeType);
        }
        break;
    case eGetAllAlbumsCountFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllAlbumsCountFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllAlbumsCountFromVolume(volumeType);
        }
        break;
    case eGetAllMusicArtistsCountFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllMusicArtistsCountFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllMusicArtistsCountFromVolume(volumeType);
        }
        break;
    case eGetAllMusicComposersCountFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllMusicComposersCountFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllMusicComposersCountFromVolume(volumeType);
        }
        break;
    case eGetAllMusicGenresCountFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllMusicGenresCountFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllMusicGenresCountFromVolume(volumeType);
        }
        break;
    case eGetAllSongsWithLimitFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllSongsWithLimitFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllSongsWithLimitFromVolume(volumeType);
        }
        break;
    case eGetAllAlbumsWithLimitFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllAlbumsWithLimitFromVolumeQuery_Id" << LOG_ENDL;
        	ret = GetAllAlbumsWithLimitFromVolume(volumeType);
        }
        break;
    case eGetAllMusicArtistsWithLimitFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllMusicArtistsWithLimitFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllMusicArtistsWithLimitFromVolume(volumeType);
        }
        break;
    case eGetAllMusicComposersWithLimitFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllMusicComposersWithLimitFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllMusicComposersWithLimitFromVolume(volumeType);
        }
        break;
    case eGetAllMusicGenresWithLimitFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllMusicGenresWithLimitFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllMusicGenresWithLimitFromVolume(volumeType);
        }
        break;
    case eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id:
        {
		    //LOG_TRACE << "CGracenoteDataProvider::Requestdata eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllAlbumsWithCoverArtWithLimitFromVolume(volumeType);
        }
        break;
    }
	return ret; // added by eugene.seo 2013.03.27 for static test
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetSongIndexFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType)

// Explanation
//

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetSongIndexFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType)
{
    //LOG_TRACE << "CGracenoteDataProvider::GetSongIndexFromVolume start" << LOG_ENDL;
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
	//LOG_TRACE << "CGracenoteDataProvider::GetSongIndexFromVolume m_CID" << m_CID << LOG_ENDL;
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
		//LOG_TRACE << "CGracenoteDataProvider::GetSongIndexFromVolume inside CreateDBConnection(normalized_DB)" << LOG_ENDL;
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select DISTINCT T_FilePath from %2 where T_Title not like \"%1\" order by LTRIM(T_Title) COLLATE NOCASE ").arg("").arg(CID_DB));

        m_SearchData_ForIndex.clear();
        if(searchData.startsWith("file://")) //Check for IPOD Case
                searchData = searchData.mid(7);
        m_SearchData_ForIndex = searchData;
        //LOG_TRACE << "CGracenoteDataProvider::GetAlbumIndexFromVolume sendIndexData searchData" << searchData << LOG_ENDL;
        ret = sendIndexData(eGetSongIndexFromVolumeQuery_Id, Querystring,normalized_DB);
    }
    return ret;
}

/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetAlbumIndexFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType)

// Explanation
//

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetAlbumIndexFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType)
{
    //LOG_TRACE << "CGracenoteDataProvider::GetAlbumIndexFromVolume" << LOG_ENDL;
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
	//LOG_TRACE << "CGracenoteDataProvider::GetAlbumIndexFromVolume m_CID" << m_CID << LOG_ENDL;
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
		//LOG_TRACE << "CGracenoteDataProvider::GetAlbumIndexFromVolume inside CreateDBConnection(normalized_DB)" << LOG_ENDL;
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select DISTINCT T_Album from %2 where T_Album not like \"%1\" order by LTRIM(T_Album) COLLATE NOCASE ").arg("").arg(CID_DB));

        m_SearchData_ForIndex.clear();    
        m_SearchData_ForIndex = searchData;
        //LOG_TRACE << "CGracenoteDataProvider::GetAlbumIndexFromVolume sendIndexData" << LOG_ENDL;
        ret = sendIndexData(eGetAlbumIndexFromVolumeQuery_Id, Querystring,normalized_DB);
    }
    return ret;
}



/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::GetArtistIndexFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType)

// Explanation
//

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::GetArtistIndexFromVolume(QString searchData, ETrackerAbstarctor_VolumeType volumeType)
{
    //LOG_TRACE << "CGracenoteDataProvider::GetArtistIndexFromVolume searchData" << searchData << LOG_ENDL;
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
	//LOG_TRACE << "CGracenoteDataProvider::GetArtistIndexFromVolume m_CID" << m_CID << LOG_ENDL;
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
		//LOG_TRACE << "CGracenoteDataProvider::GetArtistIndexFromVolume inside CreateDBConnection(normalized_DB)" << LOG_ENDL;
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString Querystring;
        Querystring.append(QString("select DISTINCT T_Artist from %2 where T_Artist not like \"%1\" order by LTRIM(T_Artist) COLLATE NOCASE ").arg("").arg(CID_DB));

        m_SearchData_ForIndex.clear();
        m_SearchData_ForIndex = searchData;
        ret = sendIndexData(eGetArtistIndexFromVolumeQuery_Id, Querystring,normalized_DB);
    }
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::RequestData(QString searchData, ETrackerAbstarctor_VolumeType volumeType,
                                     ETrackerAbstarctor_Query_Ids queryId)

// Explanation
//

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool  CGracenoteDataProvider::RequestData(QString searchData, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType)
{
	//LOG_TRACE << "CGracenoteDataProvider::RequestData Start" << LOG_ENDL;
    bool ret = false;
    if(searchData.isEmpty())
    {
	//LOG_TRACE << "CGracenoteDataProvider::RequestData searchData.isEmpty()" << LOG_ENDL;
        HandleError(eEmptyString);
        return ret;
    }

    //Convert string to EUC-KR also to search
    QTextCodec *codec = QTextCodec::codecForName("EUC-KR");
    QByteArray euckrStr = codec->fromUnicode(searchData);

    switch (queryId)
    {
    case eGetAllSongsByArtistFromVolumeQuery_Id:
        {
			//LOG_TRACE << "CGracenoteDataProvider::RequestData eGetAllSongsByArtistFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllSongsByArtistFromVolume(searchData, volumeType);
        }
        break;
    case eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id:
        {
			//LOG_TRACE << "CGracenoteDataProvider::RequestData eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllAlbumsWithCoverArtIndexFromVolume(searchData, volumeType);
        }
        break;
    case eGetAllMusicArtistsIndexFromVolumeQuery_Id:
        {
            //ret = GetAllMusicArtistsIndexFromVolume(searchData, volumeType);
        }
        break;
    case eGetAllMusicGenresIndexFromVolumeQuery_Id:
        {
			//LOG_TRACE << "CGracenoteDataProvider::RequestData eGetAllMusicGenresIndexFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllMusicGenresIndexFromVolume(searchData, volumeType);
        }
        break;
    case eGetAllSongsWithTitleIndexFromVolumeQuery_Id:
        {
            //ret = GetAllSongsWithTitleIndexFromVolume(searchData, volumeType);
        }
        break;
     case eGetAllSongsByAlbumFromVolumeQuery_Id:
        {
			//LOG_TRACE << "CGracenoteDataProvider::RequestData eGetAllSongsByAlbumFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllSongsByAlbumFromVolume(searchData, volumeType);
        }
        break;
    case eGetAllGenresByKeywordFromVolumeQuery_Id:
        {
			//LOG_TRACE << "CGracenoteDataProvider::RequestData eGetAllGenresByKeywordFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllGenresByKeywordFromVolume(searchData, volumeType);
        }
        break;
    case eGetAllArtistsByKeywordFromVolumeQuery_Id:
        {
			//LOG_TRACE << "CGracenoteDataProvider::RequestData eGetAllArtistsByKeywordFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllArtistsByKeywordFromVolume(searchData, volumeType);
        }
        break;
     case eGetAllAlbumsByKeywordFromVolumeQuery_Id:
        {
			//LOG_TRACE << "CGracenoteDataProvider::RequestData eGetAllAlbumsByKeywordFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllAlbumsByKeywordFromVolume(searchData, volumeType);
        }
        break;
      case eGetAllsongsByKeywordFromVolumeQuery_Id:
        {
            //LOG_TRACE << "CGracenoteDataProvider::RequestData eGetAllsongsByKeywordFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetAllsongsByKeywordFromVolume(searchData, volumeType);
        }
        break;
      case eGetMusicPlaylistByKeywordQuery_Id:
    case eGetAllMusicByKeywordFromVolumeQuery_Id:
          {
              //Not implemented
          }
          break;
    case eGetSongIndexFromVolumeQuery_Id:
          {
		      //LOG_TRACE << "CGracenoteDataProvider::RequestData eGetSongIndexFromVolumeQuery_Id" << LOG_ENDL;
              ret = GetSongIndexFromVolume(searchData, volumeType);
          }
          break;
    case eGetAlbumIndexFromVolumeQuery_Id:
          {
		  	//LOG_TRACE << "CGracenoteDataProvider::RequestData eGetAlbumIndexFromVolumeQuery_Id searchData" << searchData << LOG_ENDL;
            ret = GetAlbumIndexFromVolume(searchData, volumeType);
          }
          break;
    case eGetArtistIndexFromVolumeQuery_Id:
          {
		  	//LOG_TRACE << "CGracenoteDataProvider::RequestData eGetArtistIndexFromVolumeQuery_Id" << LOG_ENDL;
            ret = GetArtistIndexFromVolume(searchData, volumeType);
          }
          break;
    default:
          break;
    }
	return ret; // added by eugene.seo 2013.03.27 for static test
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::sendIndexData(ETrackerAbstarctor_Query_Ids queryId, QString queryString,QSqlDatabase& normalizedDB )

// Explanation
//

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
bool CGracenoteDataProvider::sendIndexData(ETrackerAbstarctor_Query_Ids queryId, QString queryString,QSqlDatabase& normalizedDB )
{
    //LOG_TRACE << "CGracenoteDataProvider::sendIndexData start" << LOG_ENDL;
    bool  ret=false ;
    QSqlQuery* query = new QSqlQuery(queryString,normalizedDB);
    ret=query->exec();

    if (ret)
    {
        //LOG_TRACE << "CGracenoteDataProvider::sendIndexData query executed successfully" << LOG_ENDL;
        QStringList queryDataList;
        queryDataList.clear();

        while(query->next())
        {
            QString value = query->value(0).toString();
            if(!value.isEmpty())
            	//LOG_TRACE << "CGracenoteDataProvider::sendIndexData value" << value << LOG_ENDL;
            queryDataList.append(value.toUtf8());//QUrl::fromPercentEncoding(value.toUtf8())
        }

        //LOG_TRACE << "Search data for index query for IPOD : " << m_SearchData_ForIndex << LOG_ENDL;
        int index = queryDataList.indexOf(m_SearchData_ForIndex);

        //LOG_TRACE << "sendIndexData : " << index << LOG_ENDL;

        queryDataList.clear();

        QVector<QStringList> dataList;
        QString indexString;
        indexString.setNum(index);
        queryDataList.append(indexString);
        dataList.append(queryDataList);

        emit DataFeched(dataList, queryId);

    }
    delete query;
    return ret;
}


/******************************************************************************
// Function Name                                                                                  Parameters
// CGracenoteDataProvider::HandleError(int nError)

// Explanation
//

//
// History
// 11- December-2012      Divya DSouza                created for Gracenote notmalized data
*****************************************************************************/
void CGracenoteDataProvider::HandleError(int nError)
{
    //LOG_TRACE << " the error is = " << nError << LOG_ENDL;
    emit Error(nError);
}


/******************************************************************************

// Function Name                                                Parameters

// CGracenoteDataProvider::setDeviceURL(ETrackerAbstarctor_VolumeType volumeType,QString DeviceId,QString MountPath)
//

// Explanation
//This method set the URL of the mounted mounted Device based on the volumetype

//

// History
// 14-Dec-2012     Divya DSouza    set the URL of the mounted mounted Device based on the volumetype
*****************************************************************************/


bool CGracenoteDataProvider::setDeviceURL(ETrackerAbstarctor_VolumeType volumeType,QString DeviceId,QString MountPath)
{
    bool ret = false;
    //QSparqlQuery volumeQueryObject;
    //QSparqlResult* result;
    QString deviceUrl;

   // qDebug() << "CMediaDataProvider::setDeviceURL" << volumeType << DeviceId << MountPath;
    m_DeviceId.removeAt(volumeType);
    m_DeviceId.insert(volumeType,DeviceId);
	
	//LOG_TRACE << " CGracenoteDataProvider::setDeviceURL Device ID :" <<  m_DeviceId.at(volumeType) << " volumeType" << volumeType << LOG_ENDL;

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
            //LOG_TRACE << "Device Query " << result->query() << LOG_ENDL;
            while(result->next())
            {
            m_DeviceURL.insert(volumeType,result->value(0).toString());
            }
            //LOG_TRACE << "CMediaDataProvider::setDeviceURL m_DeviceURL : " << m_DeviceURL.at(volumeType) << "VolumeType :"<<volumeType << LOG_ENDL;
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


/*
 *******************************************************************
 * Function	: CGracenoteDataProvider::getDeviceID
 *
 * Description 	: Returns the device ID

 * Parameters 	: volumeType

 * Returns 	: QString device id
 *
 *******************************************************************
 */
QString CGracenoteDataProvider::getDeviceID(ETrackerAbstarctor_VolumeType volumeType)
 {
    QString DeviceID;

    if(m_DeviceId.length() <= 0)
    {
	     //LOG_TRACE << "CGracenoteDataProvider::getDeviceID m_DeviceId.length() <= 0"  << LOG_ENDL;
        return NULL;
    }

    DeviceID = m_DeviceId.at(volumeType);

    //LOG_TRACE << "CGracenoteDataProvider::getDeviceID" << DeviceID << LOG_ENDL;
    return DeviceID;
 }


/*
 *******************************************************************
 * Function	: CGracenoteDataProvider::IsThereAnySongWithoutAlbumFromVolume()
 *
 * Description 	: Returns the device ID

 * Parameters 	: volumeType

 * Returns 	: QString device id
 *
 *******************************************************************
 */
bool CGracenoteDataProvider::IsThereAnySongWithoutAlbumFromVolume(ETrackerAbstarctor_VolumeType volumeType)
{
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString CountSongsWithMetaInfo = "";
        QString CountAllSongs = "";
        QString CountSongsWithoutMetaInfo = "";
        QString CountTemp = "%1";
        QVector<QStringList> queryDataList;
        QStringList songsPresent;

        ret = GetAllSongsCountWithMetaInfoFromVolume(normalized_DB,CountSongsWithMetaInfo, CID_DB);
        if(!ret)
        {
            normalized_DB.close();
            return ret;
        }
        ret = GetAllSongsCountFromVolume(normalized_DB,CountAllSongs, CID_DB);
        if(!ret)
        {
            normalized_DB.close();
            return ret;
        }

        normalized_DB.close();
        CountSongsWithoutMetaInfo = CountTemp.arg(CountAllSongs.toInt() -  CountSongsWithMetaInfo.toInt());

        if(CountSongsWithoutMetaInfo.toInt() > 0)
            songsPresent.append("1");
        else
            songsPresent.append("0");

        queryDataList.append(songsPresent);
        emit DataFeched(queryDataList, eIsThereAnySongWithoutAlbumFromVolumeQuery_Id);
    }
    return ret;
}


/*
 *******************************************************************
 * Function	: CGracenoteDataProvider::GetAllSongsCountWithMetaInfoFromIPOD_sync(QSqlDatabase ipoddb, QString& Count)
 *
 * Description 	: Returns the device ID

 * Parameters 	: volumeType

 * Returns 	: QString device id
 *
 *******************************************************************
 */
bool CGracenoteDataProvider::GetAllSongsCountWithMetaInfoFromVolume(QSqlDatabase& normalized_DB, QString& Count, QString CID_DB)
{
    bool ret=false;

    QString querystring ;
    querystring.append("select count(*) FROM %2 where T_Album like \"%1\" ").arg("").arg(CID_DB);

    QSqlQuery* query = new QSqlQuery(querystring,normalized_DB);
    ret=query->exec();

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


/*
 *******************************************************************
 * Function	: CGracenoteDataProvider::GetAllSongsCountFromVolume(QSqlDatabase& normalized_DB, QString& Count)
 *
 * Description 	: Returns the device ID

 * Parameters 	: volumeType

 * Returns 	: QString device id
 *
 *******************************************************************
 */
bool CGracenoteDataProvider::GetAllSongsCountFromVolume(QSqlDatabase& normalized_DB, QString& Count, QString CID_DB)
{
    bool ret=false;
    QString querystring;
    querystring.append("select count (*) FROM %1").arg(CID_DB);

    QSqlQuery* query = new QSqlQuery(querystring,normalized_DB);
    ret=query->exec();

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


/*
 *******************************************************************
 * Function	: CGracenoteDataProvider::GetAllMusicByKeyword(QString keyword, QStringList volumeList)
 *
 * Description 	: Returns the device ID

 * Parameters 	: volumeType

 * Returns 	: QString device id
 *
 *******************************************************************
 */
bool CGracenoteDataProvider::GetAllMusicByKeyword(QString keyword, QStringList volumeList)
{
	bool ret=false; // added by eugene.seo 2013.03.27 for static test

    keyword = keyword.replace("%","\\%");
    keyword = keyword.replace("'","''");

    //LOG_TRACE << "CGracenoteDataProvider::GetAllMusicByKeyword" << LOG_ENDL;

    QVector<QStringList> queryDataList;
    queryDataList.clear();

    volumeList.append(gVolumePath_JukeBox);
    ETrackerAbstarctor_VolumeType volumeType;

    for( int i =0; i<volumeList.count(); i++)
    {
        QString volume = volumeList.at(i);
        if(volume.compare(gVolumePath_JukeBox) == 0)
        {
            volumeType = eJukeBox;
            //LOG_TRACE << "CGracenoteDataProvider::GetAllMusicByKeyword volumeType = eJukeBox" << LOG_ENDL;
            GetSearchData(keyword, volumeType, queryDataList); // moved by eugene.seo 2013.03.27 for static test
        }
        else if(volume.compare(GetUSBMountPath()) == 0) // modified by eugene.seo 2013.05.13
        {
             volumeType = eUSB;
             //LOG_TRACE << "CGracenoteDataProvider::GetAllMusicByKeyword volumeType = eUSB" << LOG_ENDL;
             GetSearchData(keyword, volumeType, queryDataList); // moved by eugene.seo 2013.03.27 for static test
        }        
    }

    if(m_IPOD_FRONT)
    {
        volumeType = eIPOD_FRONT;
        GetSearchData(keyword, volumeType, queryDataList);
    }

    if(m_IPOD_REAR)
    {
        volumeType = eIPOD_REAR;
        GetSearchData(keyword, volumeType, queryDataList);
    }

    if(m_isCDInserted)
    {
         int noOfColumns = 5;
         QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE","CDDA");
         database.setDatabaseName("/app/data/gracenote/CDDAData"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change
         if(database.open())
         {
             //LOG_TRACE << "Database for CD is open " << LOG_ENDL;
             QString queryStr;
             queryStr.append( QString("select Title, Album, Artist, Genre, Path From CDTable where Title like '%%1%'  OR Album like '%%1%' OR Artist like '%%1%' order by Title").arg(keyword));

             QSqlQuery* query = new QSqlQuery(queryStr,database);
			 ret = query->exec(); // modified by eugene.seo 2013.03.27 for static test

     		 // if query executed successfully then connecting signal/slot for result
		     if(ret)
             {
                 while(query->next())
                 {
                     QStringList columnList;

                     for (int i = 0; i < noOfColumns; i++)
                     {
                         QString value = query->value(i).toString();
                         //LOG_TRACE <<"CGracenoteDataProvider::senddata value" << value << LOG_ENDL;

                         columnList.append(QUrl::fromPercentEncoding(value.toUtf8()));
                     }
                     queryDataList.append(columnList);
                 }
             }
             delete query;
             database.close();
         }
     }

     CheckIfFileExists(queryDataList, 4);

     // after data feching emiting a singal to notify CTrackerAbstractor
     emit DataFeched(queryDataList, eGetAllMusicByKeywordQuery_Id);

	 return ret; // added by eugene.seo 2013.03.27 for static test
}

/*
 *******************************************************************
 * Function	: CGracenoteDataProvider::GetAllMusicByKeyword(QString keyword, QStringList volumeList)
 *
 * Description 	: Returns the device ID

 * Parameters 	: volumeType

 * Returns 	: QString device id
 *
 *******************************************************************
 */
void CGracenoteDataProvider :: GetSearchData( QString keyword, ETrackerAbstarctor_VolumeType volumeType, QVector<QStringList> &queryDataList)
{
    //LOG_TRACE << "CGracenoteDataProvider :: GetSearchData" << LOG_ENDL;
    int noOfColumns = 5;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString querystring;
        querystring.append(QString("select T_Title, T_Album, T_Artist, T_Genre, T_FilePath From %2 where T_Title like '%%1%'  OR T_Album like '%%1%' OR T_Artist like '%%1%'").arg(keyword).arg(CID_DB));

        QSqlQuery* query = new QSqlQuery(querystring,normalized_DB);
        if(query->exec())
        {
            //LOG_TRACE << "CGracenoteDataProvider :: GetSearchData query->exec() successful" << LOG_ENDL;
            while(query->next())
            {
                QStringList columnList;

                for (int i = 0; i < noOfColumns; i++)
                {
                    QString value = query->value(i).toString();
                    //LOG_TRACE <<"CGracenoteDataProvider::senddata value" << value << LOG_ENDL;
					
                    if(i==4 && !value.isEmpty())
                    {
                        if (!value.startsWith(QLatin1String("file://")))
                        {
                            value = QLatin1String("file://") + value;
                            //LOG_TRACE <<"CGracenoteDataProvider::senddata value" << value << LOG_ENDL;
                        }
                    }
                    columnList.append(value.toUtf8()); //QUrl::fromPercentEncoding(value.toUtf8())
                }
                queryDataList.append(columnList);
            }
        }
        normalized_DB.close();
    }

}


/*
 *******************************************************************
 * Function	: CGracenoteDataProvider::GetAllMusicByKeyword(QString keyword, QStringList volumeList)
 *
 * Description 	: Returns the device ID

 * Parameters 	: volumeType

 * Returns 	: QString device id
 *
 *******************************************************************
 */
void CGracenoteDataProvider::CheckIfFileExists(QVector<QStringList> &dataList, int column)
{
    //LOG_TRACE << "CGracenoteDataProvider::CheckIfFileExists" << LOG_ENDL;

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


/*
 *******************************************************************
 * Function	: CGracenoteDataProvider::GetAllMusicByKeyword(QString keyword, QStringList volumeList)
 *
 * Description 	: Returns the device ID

 * Parameters 	: volumeType

 * Returns 	: QString device id
 *
 *******************************************************************
 */
bool CGracenoteDataProvider::GetMusicMetaData(QString filePath)
{
    //LOG_TRACE <<"CGracenoteDataProvider::GetMusicMetaData" << filePath << LOG_ENDL;
    ETrackerAbstarctor_VolumeType volumeType;
    QVector<QStringList> queryDataList;
    queryDataList.clear();

    bool ret = false;

    if(filePath.startsWith(gVolumePath_IPod))
    {
        if(m_IPOD_FRONT)
        {
            volumeType = eIPOD_FRONT;
            ret = GetMusicMetaDataFromFilePath(filePath, volumeType, queryDataList);
        }

        if(m_IPOD_REAR && !ret)
        {
            volumeType = eIPOD_FRONT;
            ret = GetMusicMetaDataFromFilePath(filePath, volumeType, queryDataList);
        }
    }
    else if (filePath.startsWith(gVolumePath_JukeBox))
    {
	    //LOG_TRACE <<"CGracenoteDataProvider::GetMusicMetaData filePath.startsWith(gVolumePath_JukeBox)" << LOG_ENDL;
        volumeType = eJukeBox;
        filePath = filePath.mid(7);
        ret = GetMusicMetaDataFromFilePath(filePath, volumeType, queryDataList);
    }
    else if(filePath.startsWith(GetUSBMountPath())) // modified by eugene.seo 2013.05.13
    {
        volumeType = eUSB;
        filePath = filePath.mid(7);
        //May need to check fro front USB and back USB
        ret = GetMusicMetaDataFromFilePath(filePath, volumeType, queryDataList);
    }
    //LOG_TRACE <<" emit DataFeched" << LOG_ENDL;
    emit DataFeched(queryDataList, eGetMusicMetaDataQuery_Id);
	
	return ret; // added by eugene.seo 2013.03.27 for static test
}


bool CGracenoteDataProvider::GetMusicMetaDataFromFilePath(QString filePath, ETrackerAbstarctor_VolumeType volumeType, QVector<QStringList> &queryDataList)
{
	//LOG_TRACE <<" CGracenoteDataProvider::GetMusicMetaDataFromFilePath" << LOG_ENDL;
    int noOfColumns = 6;
    bool ret = false;

    m_CID = GetCIDForVolume(volumeType);
    if(m_CID <=0)
        return ret;

    QSqlDatabase normalized_DB;
    if(CreateDBConnection(normalized_DB))
    {
		//LOG_TRACE <<" CreateDBConnection(normalized_DB)" << LOG_ENDL;
        QString CID_DB;
        CID_DB.clear();
        CID_DB.append("CID_");
        CID_DB.append(QString("%1").arg(m_CID));

        QString queryStr;
        queryStr.append( QString("select T_Title, T_Artist, T_Album, T_Genre, T_FilePath, T_Composer From %2 where T_FilePath=\"%1\" ").arg(filePath).arg(CID_DB));

        QSqlQuery* query = new QSqlQuery(queryStr,normalized_DB);
        if(query->exec())
        {
            //LOG_TRACE << "CGracenoteDataProvider :: GetSearchData query->exec() successful" << LOG_ENDL;
            while(query->next())
            {
                QStringList columnList;

                for (int i = 0; i < noOfColumns; i++)
                {
                    QString value = query->value(i).toString();
					if(value.isEmpty())
						value.append("dummy");
					else
					    //LOG_TRACE <<"CGracenoteDataProvider::senddata value" << value << LOG_ENDL;      
                    if(i==4 && !value.isEmpty())
                    {
                        if (!value.startsWith(QLatin1String("file://")))
                        {
                            value = QLatin1String("file://") + value;
                            //LOG_TRACE <<"CGracenoteDataProvider::senddata value" << value << LOG_ENDL;
                        }
                    }
                    columnList.append(value.toUtf8());//QUrl::fromPercentEncoding(value.toUtf8())
                    //LOG_TRACE <<"columnList.appended" << LOG_ENDL;
                }
                queryDataList.append(columnList);
            }
        }
        normalized_DB.close();
    }

    if(queryDataList.count() > 0)
	{
		//LOG_TRACE <<"queryDataList.count() > 0" << LOG_ENDL;
        ret = true;
	}

    return ret;
}

// { added by eugene.seo 2013.05.13
QString CGracenoteDataProvider::GetUSBMountPath()
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
