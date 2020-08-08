/*
 ******************************************************************
 *    COPYRIGHT ?Teleca AB      *
 *----------------------------------------------------------------*
 * MODULE     	:  CGracenoteCDLookup                             	  *
 *
 * PROGRAMMERS 	:  Vishal Jain
 * DATE       	:  15 March 2012                             	  *
 * VERSION    	:                                             	  *
 *
 *----------------------------------------------------------------*
 *                                                                *
 *  MODULE SUMMARY : CGracenoteCDLookup supports feature for
       performing look up related to CD TOC
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                     *
 *
 *     DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * History
 * 15-March-2011      Vishal Jain               Create new class and API's
 ******************************************************************
 */
#include "DHAVN_GracenoteCDLookup.h"
#include "DHAVN_GracenoteInterface.h"
#include <DHAVN_GracenoteDBconnection.h>

//Gracenote header files
#include <gn_lookup_defs.h>
#include <gn_coverart.h>
#include <gn_album_track_accessors.h>
#include <gnepal_string.h>
#include <gn_local_lookup.h>
#include <gnepal_memory.h>
#include <gn_abs_errors.h>
#include <gn_error_display.h>

//Other header file
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


/*
 *******************************************************************
 * Function	: CGracenoteCDLookup::CGracenoteCDLookup
 *
 * Description 	: Constructor for CGracenoteCDLookup

 * Parameters 	: parent - pointer to parent object

 * Returns 	: None
 *
 *******************************************************************
 */
 //Modified by Puneet for fixing GPP crash issue 2013.06.12
CGracenoteCDLookup::CGracenoteCDLookup(QObject *parent):sem(1)
{
    LOG_INIT_V2("Media_GracenotePluginProcess");
    Q_UNUSED(parent);

    m_pGracenoteInterface = NULL;
}

/*
 *******************************************************************
 * Function	: CGracenoteCDLookup::~CGracenoteCDLookup
 *
 * Description 	: Destructor for CGracenoteCDLookup

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
CGracenoteCDLookup::~CGracenoteCDLookup()
{
    // No implementation required
}

/*
 *******************************************************************
 * Function	: CGracenoteCDLookup::SetGracenoteInterface
 *
 * Description 	: Set the instance of CGracenoteInterface here

 * Parameters 	: aInterface (IN) - Pointer to CGracenoteInterface

 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteCDLookup::SetGracenoteInterface(CGracenoteInterface* aInterface)
{
    // Gracenote interface
    m_pGracenoteInterface = aInterface;

    // Initialize CDDA database
    CreateCDDADatabase();
}

/*
 *******************************************************************
 * Function	: CGracenoteCDLookup::PerformCDLookup
 *
 * Description 	: Perform the CD Lookup on the TOC received

 * Parameters 	: tocInfo (IN) - TOC string

 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteCDLookup::PerformCDLookup()
{
    LOG_TRACE << LOG_TAG_CDLOOKUP << " PerformCDLookup Start " << LOG_ENDL;
    LOG_INFO << LOG_TAG_CDLOOKUP << m_TocInfo << LOG_ENDL;

    TrackerMusicData trackerMusicFiledata;
    
    //Clear the Media Metadata from the structure
    ClearTrackerMusicData(trackerMusicFiledata);
		
    gn_error_t		nError = GNERR_NoError;
    gn_lu_options_t	lu_options = TLOPT_Result_Album;
    gn_lu_match_t	match_type = TLMATCH_None;
    gn_size_t		match_count = 0;
    
    //Perform lookup for CD TOC
    lu_options |= TLOPT_TOC_LOOKUP;

    //Convert TOC from QString to gn_uchar_t
    gn_uchar_t* gtocInfo = gnepal_strdup( (gn_uchar_t *)  (m_TocInfo.toUtf8().constData()) );
    if(gtocInfo == GN_NULL)
    {
        m_pGracenoteInterface->OnCDLookupComplete(trackerMusicFiledata);
        return;
    }

    /* Perform the lookup in the primary DB. */
    nError = gnllu_local_lookup(ALBUMLOCAL, gtocInfo, NULL, lu_options, &match_type);
    LOG_TRACE << LOG_TAG_CDLOOKUP <<("PerformCDLookup :: gnllu_local_lookup error:");
    LogError(nError);
    if (nError != GN_SUCCESS) {
        //Free string allocations
        if(gtocInfo != NULL) gnepal_mem_free(gtocInfo); gtocInfo = NULL;

        m_pGracenoteInterface->OnCDLookupComplete(trackerMusicFiledata);
        return;
    }

    nError = gnllu_get_match_count(ALBUMLOCAL, &match_count);
    if ((nError != GN_SUCCESS) || (match_count <= 0)) {
        //Free string allocations
        if(gtocInfo != NULL) gnepal_mem_free(gtocInfo); gtocInfo = NULL;

        m_pGracenoteInterface->OnCDLookupComplete(trackerMusicFiledata);
        return;
    }
	
    gn_palbum_t albumData;
    albumData = GN_NULL;

    if (match_count == 1) {
        gn_size_t index=0;
        nError = gnllu_convert_album_data( ALBUMLOCAL, index, &albumData, GN_FALSE );

        LOG_TRACE << LOG_TAG_CDLOOKUP <<("PerformCDLookup :: gnllu_convert_album_data error:");
        LogError(nError);

        if (nError == GN_SUCCESS && albumData!=NULL) {

            CreateTrackerMusicDataFromCDLookup(trackerMusicFiledata, albumData);
            trackerMusicFiledata.CD_TOC = m_TocInfo;
            RetrieveCoverArtFromCDLookup(trackerMusicFiledata, albumData);
        }
    }
    else if ((match_type == TLMATCH_MultiExact) || (match_type == TLMATCH_Fuzzy)) {
        for (gn_size_t i = 0; (i < match_count); i++) {


            nError = gnllu_convert_album_data(ALBUMLOCAL,i,&albumData,GN_TRUE);
            LOG_TRACE << LOG_TAG_CDLOOKUP <<("PerformCDLookup :: gnllu_convert_album_data error:");
            LogError(nError);

            if(match_type == TLMATCH_MultiExact)
            {
                gn_uint32_t certification;
                nError = gn_album_data_get_album_certification_level(albumData, &certification);

                if(nError == GN_SUCCESS && certification == 0)
                {
                    gn_album_data_release_album(&albumData);
                    continue;
                }
            }
            if (nError == GN_SUCCESS && albumData != NULL) {
                LOG_TRACE << LOG_TAG_CDLOOKUP <<("PerformCDLookup :: CreateTrackerMusicDataFromCDLookup match type:");
                LOG_INFO << LOG_TAG_CDLOOKUP << match_type << LOG_ENDL;
                CreateTrackerMusicDataFromCDLookup(trackerMusicFiledata, albumData);
                trackerMusicFiledata.CD_TOC = m_TocInfo;
                RetrieveCoverArtFromCDLookup(trackerMusicFiledata, albumData);
                break;
            }//END  if (nError == GN_SUCCESS && m_albumData != NULL)
        }//END for()
    }//END else if

    //Free album Data
    gn_album_data_release_album(&albumData);

    //Free string allocations
    if(gtocInfo != NULL) gnepal_mem_free(gtocInfo); gtocInfo = NULL;

    m_pGracenoteInterface->OnCDLookupComplete(trackerMusicFiledata);

    LOG_TRACE << LOG_TAG_CDLOOKUP << ("PerformCDLookup End");
}

/*
 *******************************************************************
 * Function	: CGracenoteCDLookup::ClearTrackerMusicData
 *
 * Description 	: Clear the data inside the TrackerMusicData structure

 * Parameters 	: trackerMusicFiledata - data to be cleared

 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteCDLookup::ClearTrackerMusicData(TrackerMusicData &trackerMusicFiledata)
{
    trackerMusicFiledata.track_title.clear();
    trackerMusicFiledata.album.clear();
    trackerMusicFiledata.track_artist.clear();
    trackerMusicFiledata.genre.clear();
    trackerMusicFiledata.id3album.clear();
    trackerMusicFiledata.filename.clear();
    trackerMusicFiledata.path.clear();
    trackerMusicFiledata.composer.clear();
    trackerMusicFiledata.id3artist.clear();


    trackerMusicFiledata.track_number.clear();

    trackerMusicFiledata.album_title.clear();
    trackerMusicFiledata.album_artist.clear();
    trackerMusicFiledata.album_genre.clear();
    trackerMusicFiledata.album_year.clear();

    trackerMusicFiledata.CD_TOC.clear();
    trackerMusicFiledata.track_count = 0;

    trackerMusicFiledata.trackData.clear();
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::CreateTrackerMusicDataFromCDLookup
 *
 * Description 	: Create TrackerMusicData structure from the
                                  recent CD look up

 * Parameters 	: trackerMusicFiledata - TrackerMusicData structure
                          albumData - album data object

 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteCDLookup :: CreateTrackerMusicDataFromCDLookup(TrackerMusicData &trackerMusicFiledata,
                                                              gn_palbum_t &albumData)
{
    gn_error_t nError;

    if(albumData != GN_NULL)
    {
        const gn_uchar_t*  tmp_str = NULL;
        nError = gn_album_data_get_album_title(albumData, &tmp_str );
        if(nError == GN_SUCCESS)
        {
            trackerMusicFiledata.album_title = QString::fromUtf8(reinterpret_cast<const char*>(tmp_str));
            LOG_TRACE << LOG_TAG_CDLOOKUP <<("CreateTrackerMusicDataFromCDLookup :: album_title:") << LOG_ENDL;
            LOG_INFO << LOG_TAG_CDLOOKUP <<(trackerMusicFiledata.album_title) << LOG_ENDL ;
        }

        nError = gn_album_data_get_album_artist( albumData, &tmp_str );
        if(nError == GN_SUCCESS)
        {
            trackerMusicFiledata.album_artist = QString::fromUtf8(reinterpret_cast<const char*>(tmp_str));
            LOG_TRACE << LOG_TAG_CDLOOKUP <<("CreateTrackerMusicDataFromCDLookup :: album_artist:") << LOG_ENDL;
            LOG_INFO << LOG_TAG_CDLOOKUP << (trackerMusicFiledata.album_artist) << LOG_ENDL ;
        }

        nError = gn_album_data_get_album_genre( albumData, (gn_uchar_t **)&tmp_str );
        if(nError == GN_SUCCESS)
        {
            trackerMusicFiledata.album_genre = QString::fromUtf8(reinterpret_cast<const char*>(tmp_str));
            LOG_TRACE << LOG_TAG_CDLOOKUP <<("CreateTrackerMusicDataFromCDLookup :: album_genre:") << LOG_ENDL;
            LOG_INFO << LOG_TAG_CDLOOKUP <<(trackerMusicFiledata.album_genre) << LOG_ENDL;
        }

        //Track Metadata Access
        nError = gn_album_data_get_album_track_count( albumData, &tmp_str );
        QString track_countStr = QString::fromUtf8(reinterpret_cast<const char*>(tmp_str));
        int track_count = track_countStr.toInt();
        if(nError == GN_SUCCESS)
        {
            trackerMusicFiledata.track_count = track_count;
            LOG_TRACE << LOG_TAG_CDLOOKUP <<("CreateTrackerMusicDataFromCDLookup :: track_count:") << LOG_ENDL ;
            LOG_INFO << LOG_TAG_CDLOOKUP <<(trackerMusicFiledata.track_count) << LOG_ENDL ;
        }

        // Check whether a CD has classical support
        gn_bool_t isClassical = false;
        gn_album_data_is_album_classical(albumData, &isClassical);
        gn_palbum_track_t track;
        TrackerMusicTrackData track_data;

        QString cdDummyPath;
        QString trackIndex;

        if(nError == GN_SUCCESS)
        {
            LOG_TRACE << LOG_TAG_CDLOOKUP <<("CreateTrackerMusicDataFromCDLookup :: Tracks in TOC");
            LOG_INFO << LOG_TAG_CDLOOKUP <<(track_count);

            for (int track_index = 1; (GN_SUCCESS == nError) && (track_index <= track_count); track_index++)
            {

                cdDummyPath = "file:///cd/";
                nError = gn_album_data_get_album_track_by_ord (albumData, track_index, &track);
                LOG_TRACE << LOG_TAG_CDLOOKUP <<("CreateTrackerMusicDataFromCDLookup :: gn_album_data_get_album_track_by_ord error:") << LOG_ENDL;
                LogError(nError);


                if(nError == GN_SUCCESS && track != NULL) {

                    if(isClassical)
                	{
                    	//Retrieve Track composer
                    	track_data.track_composer = GetTrackComposer(track);
                	}

                    nError = gn_album_data_get_track_title (track, &tmp_str);
                    if(nError == GN_SUCCESS) {
                        track_data.track_title = QString::fromUtf8(reinterpret_cast<const char*>(tmp_str));
                        LOG_TRACE << LOG_TAG_CDLOOKUP <<("CreateTrackerMusicDataFromCDLookupe :: track_data.track_title:") << LOG_ENDL ;
                        LOG_INFO << LOG_TAG_CDLOOKUP <<(track_data.track_title) << LOG_ENDL;

                    }

                    nError = gn_album_data_get_track_year (track, &tmp_str);
                    if(nError == GN_SUCCESS)
                    {
                        track_data.track_year = QString::fromUtf8(reinterpret_cast<const char*>(tmp_str));
                        LOG_TRACE << LOG_TAG_CDLOOKUP <<("CreateTrackerMusicDataFromCDLookup :: track_data.track_year:") << LOG_ENDL;
                        LOG_INFO << LOG_TAG_CDLOOKUP <<(track_data.track_year) << LOG_ENDL;
                    }

                    nError = gn_album_data_get_track_artist (track, &tmp_str);
                    if(nError == GN_SUCCESS)
                    {
                        track_data.track_artist = QString::fromUtf8(reinterpret_cast<const char*>(tmp_str));
                        LOG_TRACE << LOG_TAG_CDLOOKUP <<("CreateTrackerMusicDataFromCDLookup :: track_data.track_artist:") << LOG_ENDL;
                        LOG_INFO << LOG_TAG_CDLOOKUP <<(track_data.track_artist) << LOG_ENDL ;
                    }

                    gn_uchar_t*  tmp_str_val = NULL;

                    nError = gn_album_data_get_track_artist_type (track, &tmp_str_val);
                    if(nError == GN_SUCCESS)
                    {
                        track_data.tract_artistType = QString::fromUtf8(reinterpret_cast<const char*>(tmp_str_val));
                        LOG_TRACE << LOG_TAG_CDLOOKUP <<("CreateTrackerMusicDataFromCDLookup :: track_data.tract_artistType:") << LOG_ENDL;
                        LOG_INFO << LOG_TAG_CDLOOKUP <<(track_data.tract_artistType)<< LOG_ENDL;
                    }

                    nError = gn_album_data_get_track_artist_origin (track, &tmp_str_val);
                    if(nError == GN_SUCCESS)
                    {
                        track_data.track_artistOrigin = QString::fromUtf8(reinterpret_cast<const char*>(tmp_str_val));
                        LOG_TRACE << LOG_TAG_CDLOOKUP <<("CreateTrackerMusicDataFromCDLookup :: track_data.track_artistOrigin:") << LOG_ENDL;
                        LOG_INFO << LOG_TAG_CDLOOKUP <<(track_data.track_artistOrigin)<< LOG_ENDL;
                    }

                    nError = gn_album_data_get_track_artist_era (track, &tmp_str_val);
                    if(nError == GN_SUCCESS)
                    {
                        track_data.track_artistEra = QString::fromUtf8(reinterpret_cast<const char*>(tmp_str_val));
                        LOG_TRACE << LOG_TAG_CDLOOKUP <<("CreateTrackerMusicDataFromCDLookup :: track_data.track_artistEra:")<< LOG_ENDL;
                        LOG_INFO << LOG_TAG_CDLOOKUP <<(track_data.track_artistEra)<< LOG_ENDL;
                    }

                    nError = gn_album_data_get_track_genre (track, &tmp_str_val);
                    if(nError == GN_SUCCESS)
                    {
                        track_data.track_genre = QString::fromUtf8(reinterpret_cast<const char*>(tmp_str_val));
                        LOG_TRACE << LOG_TAG_CDLOOKUP <<("CreateTrackerMusicDataFromCDLookup :: track_data.track_genre:")<< LOG_ENDL;
                        LOG_INFO << LOG_TAG_CDLOOKUP <<(track_data.track_genre)<< LOG_ENDL;
                    }//END  if(nError == GN_SUCCESS)

                    // Append dummy file path
                    trackIndex.setNum(track_index);
                    cdDummyPath.append(trackIndex);

                    track_data.track_path = cdDummyPath;

                    trackerMusicFiledata.trackData.append(track_data);

                }
            }//END for

        }//END  if(nError == GN_SUCCESS)
    }        
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::RetrieveCoverArtFromCDLookup
 *
 * Description 	: Retrieve cover art from the recent CD lookup

 * Parameters 	: musicData - TrackerMusicData structure
                          albumData - album data object

 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteCDLookup :: RetrieveCoverArtFromCDLookup(TrackerMusicData &musicData,
                                                        gn_palbum_t &albumData)
{
    LOG_TRACE << LOG_TAG_CDLOOKUP << "RetrieveCoverArtFromCDLookup Start" << LOG_ENDL;

    if (albumData == GN_NULL)
        return;

    gn_coverart_t coverart = NULL;
    gn_error_t nError = gn_coverart_prepare_from_album_data(albumData, &coverart );
    LOG_TRACE << LOG_TAG_CDLOOKUP <<("RetrieveCoverArtFromCDLookup :: gn_coverart_prepare_from_album_data error:") << LOG_ENDL;
    LogError(nError);

    QImage image;

    if( nError == GN_SUCCESS )
    {
        nError = GN_FAILURE;
        gn_bool_t coverart_available=GN_FALSE;
        gn_uchar_t* imageBuf = NULL;
        gn_uint32_t image_size;

        //Retrieve album art
        nError = gn_coverart_retrieve_local_album_art(coverart, &coverart_available);

        //if album art not available, try to retrieve genre art
        if( !coverart_available || coverart == NULL)
        {
            //Retrieve Genre Art
            nError = gn_coverart_retrieve_local_genre_art(coverart, &coverart_available);
        }

        if(nError == GN_SUCCESS && coverart_available && coverart != NULL)
        {

            //Get image size
            nError = gn_coverart_get_image_size(coverart, &image_size);

            if(nError != GN_SUCCESS || image_size == 0)
            {
                return;
            }

            //Get image buffer
            nError = gn_coverart_get_image(coverart, &imageBuf);
            if( nError != GN_SUCCESS || imageBuf == NULL)
            {
                return;
            }

            image = QImage::fromData(reinterpret_cast<const uchar*>(imageBuf), image_size);

        }//END if(nError == GN_SUCCESS && coverart_available && coverart != NULL)
    }//END if( nError == GN_SUCCESS )

    // Free the cover art structure
    gn_coverart_smart_free(&coverart);

    m_pGracenoteInterface->InsertCoverArt(musicData, image);

    LOG_TRACE << LOG_TAG_CDLOOKUP << "RetrieveCoverArtFromCDLookup End" << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteCDLookup::LogError
 *
 * Description 	: Log error
 *
 * Parameters 	: error - Error code
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteCDLookup::LogError(gn_error_t error)
{
    const gn_uchar_t* errorStr = gnerr_get_code_desc( error);

    QString txt;
    txt.append (QString("Error: %1").arg(reinterpret_cast<const char*>(errorStr)));

    LOG_TRACE << txt << LOG_ENDL;
}

/*
 *******************************************************************
 * Function	: CGracenoteCDLookup::SetTOC
 *
 * Description 	: Set the TOC string
 *
 * Parameters 	: tocInfo - toc string
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteCDLookup::SetTOC(QString tocInfo)
{
    m_TocInfo = tocInfo;
}

/*
 *******************************************************************
 * Function	: CGracenoteCDLookup::CTrackerDataUpdater
 *
 * Description 	: Create SqlLite database for CDDA

 * Parameters 	: None

 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CGracenoteCDLookup::CreateCDDADatabase()
{
    LOG_TRACE << "CGracenoteCDLookup: CreateCDDADatabase Start" << LOG_ENDL;

    QSqlDatabase database;
    QString fsDBPath; 
    fsDBPath.append("/app/data/gracenote/CDDAData"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("CDDA");

    bool result = createConnection(fsDBPath, connectionName, database);
    if(result)
    {
        CreateCDDATables(database);
        closeConnection(database);  //Modified by Puneet for fixing GPP crash issue 2013.06.12
//        database.close();  //Modified by Puneet for fixing GPP crash issue 2013.06.12
        return true;
    }
    else
        return false;
}

 //Added by Puneet for fixing GPP crash issue 2013.06.12
bool CGracenoteCDLookup::createConnection(QString dbPath, QString connectionName, QSqlDatabase &dBHandle)
{

    bool retVal = false;
    LOG_TRACE << "CGracenoteCDLookup ::createConnection: Sem trying to Acquire with available = " << sem.available() <<  LOG_ENDL;
    sem.acquire(1);
    LOG_TRACE << "CGracenoteCDLookup ::createConnection: Sem Acquired " << LOG_ENDL;

    if(QSqlDatabase::contains(connectionName))
    {
        LOG_TRACE << "CGracenoteCDLookup ::createConnection: already have this connection name"<< LOG_ENDL;

        dBHandle = QSqlDatabase::database(connectionName);
        dBHandle.setDatabaseName(dbPath);
        if(dBHandle.isOpen())
        {
            LOG_TRACE << "CGracenoteCDLookup ::createConnection: already opened successfully"<< LOG_ENDL;
            retVal = true;
        }
        else
        {
            if (!dBHandle.open())
            {
                sem.release(1);
//                m_mutex_db.unlockInline();
                LOG_TRACE << "CGracenoteCDLookup ::createConnection: Sem Released on Error Avail = " << sem.available()<< LOG_ENDL;
                LOG_TRACE << "CGracenoteCDLookup ::createConnection: not opened successfully"<< LOG_ENDL;
                retVal = false;
            }
            else
            {
                LOG_TRACE << "CGracenoteCDLookup ::createConnection: opened successfully"<< LOG_ENDL;
                retVal = true;
            }
        }
    }
    else
    {
        LOG_TRACE << "CGracenoteCDLookup ::createConnection: not already have this connection name"<< LOG_ENDL;
        dBHandle = QSqlDatabase::addDatabase("QSQLITE",connectionName);
        dBHandle.setDatabaseName(dbPath);
        if (!dBHandle.open())
        {
            sem.release(1);
            LOG_TRACE << "CGracenoteCDLookup ::createConnection: Sem Released on Error, Avail = " << sem.available() << LOG_ENDL;
            LOG_TRACE << "CGracenoteCDLookup ::createConnection: not opened successfully"<< LOG_ENDL;
            retVal = false;
        }
        else
        {

            LOG_TRACE << "CGracenoteCDLookup ::createConnection: opened successfully"<< LOG_ENDL;
            retVal = true;
        }
    }
    return retVal;
}

 //Added by Puneet for fixing GPP crash issue 2013.06.12
 //by aettie for Static Test Defect 201300711
void CGracenoteCDLookup::closeConnection(QSqlDatabase &dBHandle) 
{
    dBHandle.close();
    sem.release(1);
    LOG_TRACE << "CGracenoteCDLookup ::closeConnection: Sem Released on request of close with avail = " << sem.available() << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteCDLookup::CreateCDDATables
 *
 * Description 	: Call API to Create Table in CDDA database

 * Parameters 	: None

 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CGracenoteCDLookup::CreateCDDATables(QSqlDatabase database)
{

    LOG_TRACE << "CTrackerDataUpdater: CreateCDDATables Start" << LOG_ENDL;

    bool result = false;
    QStringList tableNames = database.tables();

    if(tableNames.contains("CDTable"))
    {
        LOG_TRACE << "CGracenoteCDLookup: CDDA Table is already created" << LOG_ENDL;
        result = true;
    }
    else
    {
        LOG_TRACE << "CGracenoteCDLookup: CDDA Table is being created" << LOG_ENDL;
        result = CreateCDDATable(database);
    }

    LOG_TRACE << "CGracenoteCDLookup: CreateCDDATables End" << LOG_ENDL;
    return result;

}
/*
 *******************************************************************
 * Function	: CGracenoteCDLookup::CreateCDDATable
 *
 * Description 	: Create Table in CreateCDDATable database

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
bool CGracenoteCDLookup::CreateCDDATable(QSqlDatabase db)
{

    LOG_TRACE << "CGracenoteCDLookup: CreateCDDATable Start" << LOG_ENDL;

    bool result = false;

    QSqlQuery* query = new QSqlQuery(db);
    QString queryString;

    queryString.append("CREATE TABLE CDTable (Title VARCHAR(256), Album VARCHAR(256), Artist VARCHAR(256), Genre VARCHAR(60), AlbumArtist VARCHAR(256), TrackNum INTEGER, TrackCount INTEGER, TOC VARCHAR(1024), Path VARCHAR(60), Composer VARCHAR(256) )");

    result = query->exec(queryString);

    LOG_TRACE << "Result is : " << result << query->lastError().text() << LOG_ENDL;

    if(result)
    {
        LOG_TRACE << "CGracenoteCDLookup: CDDA Query success" << LOG_ENDL;
    }
    else
    {
        LOG_TRACE << "CGracenoteCDLookup: CDDA Query failed" << LOG_ENDL;
    }

    LOG_TRACE << "CGracenoteCDLookup: CreateCDDATable End" << LOG_ENDL;
    delete query;
    return result;

}

/*
 *******************************************************************
 * Function	: CGracenoteCDLookup::InsertCDDAData
 *
 * Description 	: Insert CDDA data in the CDDA database
 *
 * Parameters 	: cddaData - List of all CD data
 *
 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CGracenoteCDLookup::InsertCDDAData(TrackerMusicData cddaData)
{

    LOG_TRACE << "CGracenoteCDLookup: InsertCDDAData called" << LOG_ENDL;

    bool result = false;
    QString title, album, artist, genre, albumArtist, trackNum, trackCount, cdToc, path, composer;

    QString queryString = QString("INSERT INTO CDTable (Title, Album, Artist, Genre, AlbumArtist, TrackNum, TrackCount, TOC, Path, Composer)");


    qDebug() << "Count: " << cddaData.track_count;

    for( int index = 0 ; index < cddaData.track_count; index++)
    {
        QString subQuery;

        // Modify strings to take care of double quote "
        title            = cddaData.trackData.at(index).track_title;
        album         = cddaData.album_title;

        if(cddaData.trackData.at(index).track_artist.length() == 0)
        {
            artist = cddaData.album_artist;
        }
        else
        {
            artist = cddaData.trackData.at(index).track_artist;
        }

        genre  = cddaData.album_genre;
        albumArtist = cddaData.album_artist;
        trackNum.setNum(index);
        trackCount.setNum(cddaData.track_count);
        cdToc = cddaData.CD_TOC;
        path = cddaData.trackData.at(index).track_path;
        composer = cddaData.trackData.at(index).track_composer;

        if(title.contains("\""))
        {
            title = title.replace("\"","\"\"");
        }
        if(album.contains("\""))
        {
            album = album.replace("\"","\"\"");
        }
        if(artist.contains("\""))
        {
            artist =  artist.replace("\"","\"\"");
        }
        if(genre.contains("\""))
        {
            genre = genre.replace("\"","\"\"");
        }
        if(albumArtist.contains("\""))
        {
            albumArtist = albumArtist.replace("\"","\"\"");
        }
        if(composer.contains("\""))
        {
            composer = composer.replace("\"","\"\"");
        }

        subQuery.append(QString(" SELECT \"%1\", \"%2\", \"%3\", \"%4\", \"%5\", \"%6\", \"%7\", \"%8\", \"%9\", \"%10\" ").
                        arg(title).arg(album).arg(artist).arg(genre).arg(albumArtist).arg(trackNum).arg(trackCount).arg(cdToc).arg(path).arg(composer));

        queryString.append(subQuery);

        if(index < (cddaData.track_count - 1))
        {
            queryString.append(" UNION ALL");
        }

    }

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/CDDAData"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("CDDA");

    result = createConnection(fsDBPath, connectionName, db);
    if(result)
    {
            //m_pGracenoteInterface->LockGracenoteMutexCDDA();

            QSqlQuery* query = new QSqlQuery(db);

            result = query->exec(queryString);
            if(result)
            {
                qDebug() << "InsertCDDAData pass";
                LOG_TRACE << "CGracenoteCDLookup: InsertCDDAData passed" << LOG_ENDL;
                result = true;
            }
            else
            {
                qDebug() << "InsertCDDAData fail : " <<  query->lastError().text() ;
                LOG_TRACE << "CGracenoteCDLookup: InsertCDDAData failed with error: " << query->lastError().text() << LOG_ENDL;
                result = false;
            }
            delete query;
            closeConnection(db);
//            db.close();
            //m_pGracenoteInterface->UnlockGracenoteMutexCDDA();
        }

    return result;
}

/*
 *******************************************************************
 * Function	: CGracenoteCDLookup::DeleteCDData
 *
 * Description 	: Delete CDDA data from the CDDA database
 *
 * Parameters 	: toc - Delete data matching the TOC
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteCDLookup::DeleteCDData(QString toc)
{
    LOG_TRACE << "CGracenoteCDLookup: DeleteCDData called" << LOG_ENDL;

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/CDDAData"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("CDDA");

    bool result = createConnection(fsDBPath, connectionName, db);
    if(result)
    {
            //m_pGracenoteInterface->LockGracenoteMutexCDDA();
            QSqlQuery* query = new QSqlQuery(db);
            QString str;

            str.append(QString("Delete From CDTable where TOC = \"%1\" ").arg(toc));

            result = query->exec(str);

            if(result)
            {
                LOG_TRACE << "CGracenoteCDLookup: DeleteCDData success" << LOG_ENDL;
            }
            else
            {
                qDebug() << query->lastError().text();
                LOG_TRACE << "CGracenoteCDLookup: DeleteCDData failed" << LOG_ENDL;
            }
            delete query;
            closeConnection(db);
//            db.close();
            //m_pGracenoteInterface->UnlockGracenoteMutexCDDA();

        }
}

/*
 *******************************************************************
 * Function	: CGracenoteCDLookup::GetTrackComposer
 *
 * Description 	: Retrieve the composer information for classical CD
 *
 * Parameters 	: trackData - track data
 *
 * Returns 	: QString - composer string
 *
 *******************************************************************
 */
QString CGracenoteCDLookup::GetTrackComposer(gn_palbum_track_t &trackData)
{
    LOG_TRACE << "CGracenoteCDLookup: GetTrackComposer Start" << LOG_ENDL;

    QString trackComposer = "";

    gn_uint32_t workCount;
    gn_error_t error = gn_album_data_get_track_work_count(trackData, &workCount);

    if(error == GN_SUCCESS)
    {
        LOG_TRACE << "CGracenoteCDLookup: Work count : " << workCount << LOG_ENDL;
        // If work count is > 0, it indicates work is available
        // We can retrieve the composer name
        if(workCount > 0)
        {
            gn_pwork_t work;
            error = gn_album_data_get_track_work(trackData, 0, &work);

            if(error == GN_SUCCESS)
            {
                const gn_uchar_t* composer;
                error = gn_work_get_composer(work, &composer);

                if(error == GN_SUCCESS)
                {
                    trackComposer = QString::fromUtf8(reinterpret_cast<const char*>(composer));
                }
            }
        }
    }

    LOG_TRACE << "CGracenoteCDLookup: GetTrackComposer End : " << trackComposer << LOG_ENDL;

    return trackComposer;
}

//End of file
