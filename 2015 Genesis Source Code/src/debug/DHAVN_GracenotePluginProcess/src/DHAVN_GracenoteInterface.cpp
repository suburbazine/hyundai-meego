
/*
 ******************************************************************
 *    COPYRIGHT ?Teleca AB      *
 *----------------------------------------------------------------*
 * MODULE     	:  CGracenoteInterface                            	  *
 *
 * PROGRAMMERS 	:  Divya Dsouza / Vishal Jain
 * DATE       	:  29 September 2011                             	  *
 * VERSION    	:                                             	  *
 *
 *----------------------------------------------------------------*
 *                                                                *
 *  MODULE SUMMARY : CGracenoteInterface supports functionalities
        for inserting metadata in MLDB. Also MoreLikeThis
        playlist creation, cover art retrieval and MusicID CD lookup.
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                     *
 *
 *     DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * History
 * 01-Sep-2011      Divya Dsouza


 ******************************************************************
 */

#include <QDBusMetaType>
#include <QFile>

#include <QtSql>
#include <QSqlDatabase>
#include <QString>

//#include <QMutex>  //added by aettie 20130518 ISV 83208

#include <gnepal_string.h>
#include <gn_error_codes.h>
#include <gn_error_values.h>
#include <gn_abs_errors.h>
#include <gnepal_fs.h>
#include <gn_error_display.h>
#include <gnepal_memory.h>
#include <gn_local_lookup.h>
#include <gn_album_track_accessors.h>
#include <gn_textid_cache.h>
#include <QFileInfo>
#include <gn_apm.h>
#include <QDebug>
#include <cdbuswrapper.h>
#include "DHAVN_GracenoteInterface.h"
#include "DHAVN_GracenoteOperationUtil.h"
#include "DHAVN_GracenotePlaylistMLDBCreator.h"
#include "DHAVN_TrackerQuery.h"
#include "DHAVN_GracenoteCDLookup.h"
#include "DHAVN_GracenoteCoverArt.h"
#include "DHAVN_GracenoteRequestQueue.h"
#include "DHAVN_GracenotePluginProcessThread.h"
#include "DHAVN_Performance.h"

#include <MediaArtInfo>
#include <qt4/Qt/qpixmap.h>
#include<QImage>


// Constans for EMMS
const QString EMMSClientId = "4994048"; /** Client ID provided by Gracenote */
const QString EMMSClientIdTag = "32A28718CF550B8372B082E0EABA389F"; /** Client ID tag provided by Gracenote */
const QString EMMSLdacKey = "LDAC0001001055CCAA7BA03B7FE61CC4AD8385CC5DED"; /** LDAC Key provided by Gracenote */

//When IPOD Disconnect signal is emitted, this value is set to 0.
int IPOD1ConnectStatus = -1;
int IPOD2ConnectStatus = -1;

//When USB Disconnect signal is emitted, this value is set to 0.
int USB1ConnectStatus = -1;
int USB2ConnectStatus = -1;

//Number for MLDBs to be maintained in the system for USB
//Customer set value overrides this
int USB_MLDB_count = 5;

//Number for MLDBs to be maintained in the system for IPOD
//Customer set value overrides this
int IPOD_MLDB_count = 5;

//When AppMusicPlayer has highlighted volume info, this value is changed.
int HighlightedVolume = 0;

CGracenoteInterface* CGracenoteInterface::m_pInstance = NULL; 	//added by aettie for Gracenote killed handling

/*
 *******************************************************************
 * Function		: CGracenoteInterface::CGracenoteInterface
 *
 * Description 	: Constructor for CGracenoteInterface

 * Parameters 	: appPath (IN) - Application directory path

 * Returns 		: None
 *
 *******************************************************************
 */
CGracenoteInterface :: CGracenoteInterface(QString appPath, QObject *parent)
{

    LOG_TRACE <<("CGracenoteInterface:CGracenoteInterface() start") << LOG_ENDL;

    Q_UNUSED(parent);
    Q_UNUSED(appPath);

    // Register meta type for Tracker music data
    qRegisterMetaType<PlaylistResults>("PlaylistResults");
    qDBusRegisterMetaType<PlaylistResults>();

    qRegisterMetaType<PlaylistResultsEntries>("PlaylistResultsEntries");
    qDBusRegisterMetaType<PlaylistResultsEntries>();

    qRegisterMetaType<TrackerMusicData>("TrackerMusicData");
    qDBusRegisterMetaType<TrackerMusicData>();

    qRegisterMetaType<TrackerMusicTrackData>("TrackerMusicTrackData");
    qDBusRegisterMetaType<TrackerMusicTrackData>();

    m_pTrackerQuery = NULL;
    m_pGNOperationUtil = NULL;
    m_pMLDBCreator = NULL;
    m_pGNCoverArt = NULL;
    m_pGNCDLookup = NULL;
    m_pQueue = NULL;
    m_pGNThread = NULL;

    m_NormalizedData = NULL; //added by aettie 2013.02.28 for static analysis 13292

    m_CoverRequestCount = 0;
    m_CoverArtList.clear();
    m_IsMultipleCoverRequest = false; //added by aettie 2013.03.27 for static analysis 13292
    
    m_FrontIPODInfo.clear();
    m_RearIPODInfo.clear();

    m_RearIPODSignalCount = 0;
    m_FrontIPODSignalCount = 0;

    m_FrontIPODSignalIndex = 0;
    m_RearIPODSignalIndex = 0;

//added by aettie for Gracenote killed handling
    setSignalHandlerObject(this);
    signalConnection();

    LOG_TRACE <<("CGracenoteInterface:CGracenoteInterface() end") << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::Init
 *
 * Description 	: Initialize member variables

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
bool CGracenoteInterface :: Init()
{
    bool initStatus = false;

    LOG_INIT_V2("Media_GracenotePluginProcess");

    m_NormalizedData = new CNormalizedData(); //added by Divya 2012.01.07 for gracenote normalized db
    LOG_TRACE <<("CGracenoteInterface: Init () Start") << LOG_ENDL;

    m_pTrackerQuery = new CTrackerQuery();
    if(!m_pTrackerQuery)
        return initStatus;

    m_pGNOperationUtil = new CGracenoteOperationUtil();
    if(!m_pGNOperationUtil)
        return initStatus;

    m_pMLDBCreator = new CPlaylistMLDBCreator;
	//modified by aettie 20130518 ISV 83208
    //if(m_pMLDBCreator->Init())
    //    m_pMLDBCreator->SetGracenoteInterface(this);
    //else
    //    return initStatus;
    
	if(!m_pMLDBCreator)
        return initStatus;
    else
        m_pMLDBCreator->Init();
		
    m_pGNCoverArt = new CGracenoteCoverArt;
    if(!m_pGNCoverArt)
        return initStatus;
    else
        connect(m_pGNCoverArt, SIGNAL(coverArtRetrieved(bool,QString)), this,
                SLOT(onCoverArtRetrieved(bool,QString)));

    // Create object to perform CD lookup
    m_pGNCDLookup = new CGracenoteCDLookup;
    if(!m_pGNCDLookup)
        return initStatus;
    else
        m_pGNCDLookup->SetGracenoteInterface(this);

    m_IsMultipleCoverRequest = false;
    m_CoverRequestCount = 0;

    if(m_pTrackerQuery->Init())
    {
        m_pTrackerQuery->SetGracenoteInterface(this);

        connect(m_pTrackerQuery, SIGNAL(indexingStatus(bool,int)),
                this, SIGNAL(GracenoteIndexingStatus(bool,int)), Qt::DirectConnection);
    }
    else
    {
        return initStatus;
    }

    LOG_TRACE <<("CGracenoteInterface: Init () end") << LOG_ENDL;
    return true;
}


/*
 *******************************************************************
 * Function		: CGracenoteInterface::~CGracenoteInterface
 *
 * Description 	: Destructor for CGracenoteInterface

 * Parameters 	: None

 * Returns 		: None
 *
 *******************************************************************
 */
CGracenoteInterface :: ~CGracenoteInterface()
{
    EMMSShutDown();

    if(m_pTrackerQuery) {
        delete m_pTrackerQuery;
        m_pTrackerQuery = NULL;
    }

    if(m_pGNOperationUtil) {
        delete m_pGNOperationUtil;
        m_pGNOperationUtil = NULL;
    }

    if(m_pMLDBCreator) {
        delete m_pMLDBCreator;
        m_pMLDBCreator = NULL;
    }

    if(m_pGNCoverArt)
    {
        delete m_pGNCoverArt;
        m_pGNCoverArt = NULL;
    }

    if(m_pGNCDLookup)
    {
        delete m_pGNCDLookup;
        m_pGNCDLookup = NULL;
    }
//{added by Divya 2012.01.07 for gracenote normalized db
    if(m_NormalizedData)
    {
        delete m_NormalizedData;
        m_NormalizedData = NULL;
    }

    //UnlockGracenoteMutex();
    //UnlockGracenoteMutexCDDA();
//}//added by Divya 2012.01.07 for gracenote normalized db
}

/*
 *******************************************************************
 * Function		: CGracenoteInterface::AVOffHandling
 *
 * Description 	: Destructor for CGracenoteInterface

 * Parameters 	: None

 * Returns 		: None
 *
 *******************************************************************
 *
 void CGracenoteInterface :: AVOffHandling()
{
    LOG_TRACE <<(" CGracenoteInterface:AVOffHandling() start") << LOG_ENDL;

    UnlockGracenoteMutex();
    UnlockGracenoteMutexCDDA();

}*/


/*
 *******************************************************************
 * Function		: CGracenoteInterface::EMMSInit
 *
 * Description 	: Initialize EMMS

 * Parameters 	: None

 * Returns 		: None
 *
 *******************************************************************
 */
bool CGracenoteInterface :: EMMSInit(QString countryVariant)
{
    LOG_TRACE <<(" CGracenoteInterface:EMMSInit() start") << LOG_ENDL;

    //Initialize EMMS
    gn_bool_t EMMSInitStatus = false;
    gn_error_t nError = GN_FAILURE;
    gn_emms_configuration_t emmsConfig;

    //Verify the permissions of the database path, coninue only if the path has permissions of rwxr_xr_x
    // Get the correct database path
    QString databasePathStr = m_pGNOperationUtil->GetDatabasePathForRegion(countryVariant);
    LOG_TRACE <<(" CGracenoteInterface:EMMSInit() GetDatabasePathForRegion called") << LOG_ENDL;
    
    QString mldbPathStr = "/app/data/gracenote/"; //modified by aettie.ji 2013.05.01 for Gracenote DB location change
//    QFileInfo DbPermission(databasePathStr);
    QFileInfo DbPermission(mldbPathStr);//modified by aettie.ji 2013.05.13 for Gracenote DB location change
    if(!((DbPermission.permission(QFile::ReadUser)) &&  (DbPermission.permission(QFile::WriteUser)) && (DbPermission.permission(QFile::ExeUser))
        &&
        (DbPermission.permission(QFile::ReadGroup)) && (DbPermission.permission(QFile::ExeGroup))
        &&
        (DbPermission.permission(QFile::ReadOther)) && (DbPermission.permission(QFile::ExeOther))
        ))
    {
        LOG_TRACE <<("CGracenoteInterface: databasePathStr permissions error : /gracenote/ELDB/xxx (KOR or NA) copied from root") << LOG_ENDL;
        return EMMSInitStatus;
    }

    nError = gnconf_initialize_configuration(&emmsConfig);
    LOG_TRACE <<(" CGracenoteInterface:EMMSInit() gnconf_initialize_configuration called") << LOG_ENDL;
    LogError(nError);

    if(nError != GN_SUCCESS)
    {
        LOG_CRITICAL <<("CGracenoteInterface:gnconf_initialize_configuration() error") << LOG_ENDL;
        LogError(nError);

        return EMMSInitStatus;
    }

    gn_uchar_t* clientId = NULL;
    gn_uchar_t* clientIdTag = NULL;
    gn_uchar_t* ldacKey = NULL;
    gn_uchar_t* enabledFeature = NULL;
    gn_uchar_t* databasePath = NULL;
    gn_uchar_t* mldbPath = NULL; //modified by aettie.ji 2013.05.01 for Gracenote DB location change
    gn_uchar_t* textIdBufSize = NULL;
    gn_uchar_t* textIdAlbumCacheSize = NULL;
    gn_uchar_t* textIdGenreCacheSize = NULL;
    gn_uchar_t* textIdLeadPerformerCacheSize = NULL;

    clientId = gnepal_strdup((gn_uchar_t *) (EMMSClientId.toUtf8().constData()));
    clientIdTag = gnepal_strdup((gn_uchar_t *) (EMMSClientIdTag.toUtf8().constData()));
    ldacKey = gnepal_strdup((gn_uchar_t *) (EMMSLdacKey.toUtf8().constData()));
    enabledFeature = gnepal_strdup((gn_uchar_t *) ("TRUE"));

    // Set the MLDB location
//modified by aettie.ji 2013.05.01 for Gracenote DB location change	
//    m_pMLDBCreator->SetMLDBLocation(databasePathStr);
    m_pMLDBCreator->SetMLDBLocation(mldbPathStr);

    databasePath = gnepal_strdup((gn_uchar_t *) (databasePathStr.toUtf8().constData()));
    mldbPath = gnepal_strdup((gn_uchar_t *) (mldbPathStr.toUtf8().constData())); //added by aettie.ji 2013.05.01 for Gracenote DB location change

    emmsConfig.base_database_path = databasePath;
    emmsConfig.set_base_database_path = TRUE;

    

    //Get the Cache configurations set by the customer from config file.
    QStringList textidCacheVal = m_pGNOperationUtil->GetTextIDCacheInfoFomConfig();
    if(textidCacheVal[4] == "1")
    {
	/* Customers should customize these values as per their targets*/
        textIdBufSize = gnepal_strdup((gn_uchar_t *) (textidCacheVal[0].toUtf8().constData()));
        textIdAlbumCacheSize = gnepal_strdup((gn_uchar_t *) (textidCacheVal[1].toUtf8().constData()));
        textIdGenreCacheSize = gnepal_strdup((gn_uchar_t *) (textidCacheVal[3].toUtf8().constData()));
        textIdLeadPerformerCacheSize = gnepal_strdup((gn_uchar_t *) (textidCacheVal[2].toUtf8().constData()));


        emmsConfig.textid_db_lookup_buffer_size = textIdBufSize;
        emmsConfig.set_textid_db_lookup_buffer_size = GN_TRUE;
        LOG_TRACE <<("CGracenoteInterface: textid_db_lookup_buffer_size done") << LOG_ENDL;
        LOG_INFO <<(reinterpret_cast<const char*>(textIdBufSize ))<< LOG_ENDL;


        //textIdBufSize is a low level database configuration parameter and is available in all eMMS versions
        emmsConfig.textid_album_cache_max = textIdAlbumCacheSize;
        emmsConfig.set_textid_album_cache_max = GN_TRUE;
        LOG_TRACE <<("CGracenoteInterface: textid_album_cache_max done") << LOG_ENDL;
        LOG_INFO <<(reinterpret_cast<const char*>(textIdAlbumCacheSize ))<< LOG_ENDL;


        emmsConfig.textid_lead_performer_cache_max = textIdLeadPerformerCacheSize;
        emmsConfig.set_textid_lead_performer_cache_max = GN_TRUE;
        LOG_TRACE <<("CGracenoteInterface: textid_lead_performer_cache_max done") << LOG_ENDL;
        LOG_INFO <<(reinterpret_cast<const char*>(textIdLeadPerformerCacheSize ))<< LOG_ENDL;


        emmsConfig.textid_genre_cache_max = textIdGenreCacheSize;
        emmsConfig.set_textid_genre_cache_max = GN_TRUE;
        LOG_TRACE <<("CGracenoteInterface: textid_genre_cache_max done") << LOG_ENDL;
        LOG_INFO <<(reinterpret_cast<const char*>(textIdGenreCacheSize ))<< LOG_ENDL;
    }

    // Set the path where mldb's should be created
	//modified by aettie.ji 2013.05.01 for Gracenote DB location change
    //emmsConfig.playlist_data_file_path = databasePath;
    //emmsConfig.set_playlist_data_file_path = TRUE;
	
    emmsConfig.playlist_data_file_path = mldbPath;
    emmsConfig.set_playlist_data_file_path = TRUE;

    emmsConfig.client_id = clientId;
    emmsConfig.set_client_id = TRUE;

    emmsConfig.client_id_tag = clientIdTag;
    emmsConfig.set_client_id_tag = TRUE;

    emmsConfig.set_LDAC_key = TRUE;
    emmsConfig.LDAC_key = ldacKey;

    emmsConfig.set_enable_playlist = TRUE;
    emmsConfig.enable_playlist = enabledFeature;

    emmsConfig.set_enable_textid = TRUE;
    emmsConfig.enable_textid = enabledFeature;

    emmsConfig.enable_textidalb = enabledFeature;
    emmsConfig.set_enable_textidalb = TRUE;

    emmsConfig.set_enable_mediavocs = TRUE;
    emmsConfig.enable_mediavocs = enabledFeature;

    emmsConfig.enable_cvrt_local = enabledFeature;
    emmsConfig.set_enable_cvrt_local = TRUE;

    emmsConfig.enable_cd_local = enabledFeature;
    emmsConfig.set_enable_cd_local = TRUE;

    emmsConfig.minimize_file_handles = enabledFeature;
    emmsConfig.set_minimize_file_handles = TRUE;

    // Added to enable classical audio CD support
    emmsConfig.enable_classical = enabledFeature;
    emmsConfig.set_enable_classical = TRUE;

    LOG_TRACE <<("CGracenoteInterface: calling gninit_initialize_emms") << LOG_ENDL;
    nError = gninit_initialize_emms(&emmsConfig);
    LOG_TRACE <<("CGracenoteInterface: called gninit_initialize_emms") << LOG_ENDL;

    if(nError == GN_SUCCESS)
    {
        EMMSInitStatus = true;

        LOG_TRACE <<("CGracenoteInterface: gninit_initialize_emms No error") << LOG_ENDL;
        LogError(nError);

    }
    else
    {
        LOG_TRACE <<("CGracenoteInterface:gninit_initialize_emms() error") << LOG_ENDL;
        LogError(nError);
    }
    //Free the allocations
    if(clientId != NULL) gnepal_mem_free(clientId); clientId = NULL;
    if(clientIdTag != NULL) gnepal_mem_free(clientIdTag); clientIdTag = NULL;
    if(ldacKey != NULL) gnepal_mem_free(ldacKey); ldacKey = NULL;
    if(enabledFeature != NULL) gnepal_mem_free(enabledFeature); enabledFeature = NULL;
    if(databasePath != NULL) gnepal_mem_free(databasePath); databasePath = NULL;
    if(mldbPath != NULL) gnepal_mem_free(mldbPath); mldbPath = NULL;
    if(textIdBufSize != NULL) gnepal_mem_free(textIdBufSize); textIdBufSize = NULL;
    if(textIdAlbumCacheSize != NULL) gnepal_mem_free(textIdAlbumCacheSize); textIdAlbumCacheSize = NULL;
    if(textIdGenreCacheSize != NULL) gnepal_mem_free(textIdGenreCacheSize); textIdGenreCacheSize = NULL;
    if(textIdLeadPerformerCacheSize != NULL) gnepal_mem_free(textIdLeadPerformerCacheSize); textIdLeadPerformerCacheSize = NULL;

    //Get the MLDB configurations set by the customer from config file.
    if(!m_pGNOperationUtil->GetMLDBConfiguration())
    {
        LOG_TRACE <<("CGracenoteInterface::Reading configuration file failed") << LOG_ENDL;

        //Default number of MLDBs created for USB and IPOD
        USB_MLDB_count = 5;
        IPOD_MLDB_count = 5;
    }

    return EMMSInitStatus;
}


/*
 *******************************************************************
 * Function		: CGracenoteInterface::EMMSShutDown
 *
 * Description 	: Uninstall and ShutDown EMMS

 * Parameters 	: None

 * Returns 		: None
 *
 *******************************************************************
 */
bool CGracenoteInterface :: EMMSShutDown()
{
    //Uninstall EMMS
    LOG_TRACE <<("CGracenoteInterface: EMMSShutDown Started") << LOG_ENDL;

    gn_error_t nError = GN_FAILURE;
    bool EMMSShutDownStatus = false;


    nError = gninit_shutdown_emms();
    LOG_TRACE <<("GracenotePluginProcess : CGracenoteInterface :: EMMS shutdown Error:") << LOG_ENDL;
    LogError(nError);
    if(nError == GN_SUCCESS)
    {
        EMMSShutDownStatus = true;
    }

    LOG_TRACE <<("CGracenoteInterface: EMMSShutDown End") << LOG_ENDL;
    return EMMSShutDownStatus;
}


/*
 *******************************************************************
 * Function		: CGracenoteInterface::Start
 *
 * Description 	: Start the MLDB population by fetching media file
 *				  paths from Tracker-Store


 * Parameters 	: None

 * Returns 		: None
 *
 *******************************************************************
 */
void CGracenoteInterface::Start()
{
    LOG_TRACE <<("Time before calling GetFilePathsInJukebox()") << LOG_ENDL;
    HighlightedVolume = e_JukeBox;

    LOG_INFO << "CGracenoteInterface::Start - HighlightedVolume" << HighlightedVolume << LOG_ENDL;
    LOG_INFO << "CGracenoteInterface::Start - Enqueueing JUKEBOX" << LOG_ENDL;
    m_pQueue->Enqueue(e_JukeBox, JUKE);
    LOG_INFO << "CGracenoteInterface::Start - After Enqueueing JUKEBOX GNqueue.count()" << m_pQueue->Count() << LOG_ENDL;

    LOG_TRACE <<("Time after calling GetFilePathsInJukebox()") << LOG_ENDL;
}
//{added by aettie for Gracenote killed handling

void CGracenoteInterface::signalHandelr(int signum)
{
    qDebug("Sigaction Signum = %d", signum);
    m_pInstance->restart();
}

void CGracenoteInterface::setSignalHandlerObject(CGracenoteInterface *m)
{
    m_pInstance = m;
}

// Watch Dog
void CGracenoteInterface::signalConnection()
{
    struct sigaction act;
    act.sa_flags    = 0;
    act.sa_flags   |= SA_RESTART;
    act.sa_handler  = CGracenoteInterface::signalHandelr;
    sigemptyset(&act.sa_mask);  //added by aettie 2013.04.03 for static defect 20375


    for( int signumber = SIGHUP ; signumber<SIGUNUSED ; signumber++)
    {
        qDebug("Sigaction succes num = %d ",signumber);

        if( signumber == SIGKILL || signumber == SIGSTOP) continue;
        if(sigaction(signumber, &act, (struct sigaction *)NULL)<0)
        {
            //GetLogger().Log(QString("Sigaction failnum = %1").arg(signumber));
                qDebug("Sigaction failnum = %d ",signumber);

            exit(1);
        }
    }
}

void CGracenoteInterface::restart()
{
        qDebug("Sigaction GracenoteKilled");

    emit GracenoteKilled();
    exit(0);
}
//}added by aettie for Gracenote killed handling

/*
 *******************************************************************
 * Function		: CGracenoteInterface::AddMusicFileEntryToGraceNoteMLDB
 *
 * Description 	: Add a media file information to MLDB

 * Parameters 	: sfileNamePath (IN) - path of the media file
 *				  nvolume (IN) - volume information

 * Returns 		: TrackerMusicData - Structure to populate the
       media information in tracker store
 *
 *******************************************************************
 */
TrackerMusicData CGracenoteInterface :: AddMusicFileEntryToGraceNoteMLDB(QString sfileNamePath, gn_collection_id_t nCID)
{
    TrackerMusicData trackerMusicFiledata;
    ClearTrackerMusicData(trackerMusicFiledata);

    // If file name is empty, return empty music data
    if(sfileNamePath.length() == 0)
        return trackerMusicFiledata;

    gn_error_t nError = GN_FAILURE;

    PT_START_TIME(PT_OBJ_CHECK_IF_EXISTS_IN_MLDB, 0);
    //Check if entry is there in MLDB already
    bool entryExists = CheckIfEntryExistsInMLDB(sfileNamePath, nCID);
    PT_END_TIME(PT_OBJ_CHECK_IF_EXISTS_IN_MLDB, 0);
//{modified by Divya 2012.01.07 for gracenote normalized db
    bool DBEntryExists = m_NormalizedData->CheckIfEntryExists(nCID,sfileNamePath);
    bool populateMLDB=true;

    // If entry is already there in MLDB, no need to update tracker
    // return empty data structure
    if(entryExists && DBEntryExists)
    {
        //Check if CoverArt for USB data is existing. If not need to update from Gracenote

        LOG_TRACE <<("CGracenoteInterface :: Entry exists") << LOG_ENDL;
        return trackerMusicFiledata;
    }
    else if(entryExists)
    {
        populateMLDB = false;
    }

//}modified by Divya 2012.01.07 for gracenote normalized db
    LOG_TRACE <<(": CGracenoteInterface :: Entry does not exist") << LOG_ENDL;

    PT_START_TIME(PT_OBJ_ETC, 0);
    gn_pfile_data_t fileData;
    nError = gn_textid_file_data_init(&fileData);
    LOG_TRACE <<("CGracenoteInterface :: gn_textid_file_data_init Error:") << LOG_ENDL;
    LogError(nError);
    PT_END_TIME(PT_OBJ_ETC, 0);

    if(nError == GN_SUCCESS)
    {
        //Parse filename and file path, errors can be ignored
        PT_START_TIME(PT_OBJ_PARSE_FILE_PATH, 0);
        m_pGNOperationUtil->ParseFilePath(sfileNamePath, fileData);
        PT_END_TIME(PT_OBJ_PARSE_FILE_PATH, 0);

        PT_START_TIME(PT_OBJ_EXTRACT_ID3_INFO, 0);
        //Extract ID3 Information
        nError = m_pGNOperationUtil->ExtractID3Information(sfileNamePath, fileData);
        LOG_TRACE <<("CGracenoteInterface :: ExtractID3Information Error:") << LOG_ENDL;
        LogError(nError);
        PT_END_TIME(PT_OBJ_EXTRACT_ID3_INFO, 0);

        if(nError == GN_SUCCESS)
        {
            trackerMusicFiledata = PerformTextIDLookup(sfileNamePath , fileData, nCID, populateMLDB); //modified by Divya 2013.01.17 for gracenote normalized db
            PT_START_TIME(PT_OBJ_ETC, 0);
            // Clean and end the lookup
    	    gn_textid_cleanup_lookup();
            gn_textid_cache_free_all();
            gn_textid_lookup_end();
            PT_END_TIME(PT_OBJ_ETC, 0);
        }

        //Free the file data
        gn_textid_file_data_smart_free( &fileData );
    }//END (nError == GN_SUCCESS)

    return trackerMusicFiledata;
}

/*
 *******************************************************************
 * Function		: CGracenoteInterface::PerformTextIDLookup
 *
 * Description 	: TextID lookup and add a media file information to MLDB

 * Parameters 	: sfileNamePath (IN) - path of the media file
 *				  nvolume (IN) - volume information

 * Returns 		: TrackerMusicData - Structure to populate the
                                  media information in tracker store
 *
 *******************************************************************
 */
 // modified by Divya 2012.01.07 for gracenote normalized db
TrackerMusicData CGracenoteInterface :: PerformTextIDLookup(QString sfileNamePath,
                                                            gn_pfile_data_t &fileData,
                                                            gn_collection_id_t nCID, bool populateMLDBFlag)
{
    TrackerMusicData trackerMusicFiledata;
    ClearTrackerMusicData(trackerMusicFiledata);

    //Verify the size of the mldb file and set the flag to stop further population
    QString mldbpath;
    mldbpath.append(QString("/dev/shm/mldb%1.pdb").arg(nCID));
    QFile mldbFileSize(mldbpath);
    if(mldbFileSize.size() > MLDB_Max_Size)
    {
        LOG_TRACE <<("PerformTextIDLookup:Maximum size of mldb file is reached - Stop populating DB") << LOG_ENDL;
        //Set the flag to true to stop further population of DB
        m_pTrackerQuery->SetMLDBSizeState(true);
        return trackerMusicFiledata;
    }
    PT_START_TIME(PT_OBJ_ETC, 0);
    gn_error_t nError = gn_textid_lookup_begin();
    PT_END_TIME(PT_OBJ_ETC, 0);

    LOG_TRACE <<("GracenoteInterface :: gn_textid_lookup_begin Error:") << LOG_ENDL;
    LOG_TRACE <<(nError) << LOG_ENDL;

    if(nError != GN_SUCCESS)
    {
        //Return empty tracker data
        return trackerMusicFiledata;
    }

    //Set ID3 values for artist and album
    // Year and contributor fields are used in the structure for temporary purpose
    const gn_uchar_t*  id3artist = NULL;
    nError = gn_textid_file_data_get_track_artist(fileData, &id3artist);
    if(nError == GN_SUCCESS)
    {
        trackerMusicFiledata.id3artist =
                QString::fromUtf8(reinterpret_cast<const char*>(id3artist));
    }
    const gn_uchar_t*  id3album = NULL;
    nError = gn_textid_file_data_get_disc_title(fileData, &id3album);
    if(nError == GN_SUCCESS)
    {
        trackerMusicFiledata.id3album =
                QString::fromUtf8(reinterpret_cast<const char*>(id3album));
    }

    PT_START_TIME(PT_OBJ_TEXTID_LOOKUP, 0);
    gn_textid_lu_flags_t lu_flag = GN_TEXTID_LU_FLAG_DFLT;
    gn_textid_match_source_t matchsource = GN_TEXTID_MATCH_SOURCE_ALBUM | GN_TEXTID_MATCH_SOURCE_CONTRIB | GN_TEXTID_MATCH_SOURCE_COMPOSER;

    gn_uint32_t nmatchcount = 0;
    gn_textid_match_type_t matchtype = GN_TEXTID_MATCH_TYPE_NOT_AVAILABLE;

    nError = gn_textid_local_lookup(fileData, lu_flag, &matchsource, &matchtype, &nmatchcount);

    LOG_TRACE <<("CGracenoteInterface :: gn_textid_local_lookup Error:") << LOG_ENDL;
    LogError(nError);

    PT_END_TIME(PT_OBJ_TEXTID_LOOKUP, 0);

    if(nError == GN_SUCCESS)
    {
        if(nmatchcount <= 0 && fileData != GN_NULL)
        {
            //Populate MLDB with only ID3 Info

            PT_START_TIME(PT_OBJ_MLDB_POPULATE, 0);
			//{ modified by Divya 2012.01.07 for gracenote normalized db
            bool dbPopulated = true;
            if(populateMLDBFlag)
                dbPopulated = PopulateMLDBWithID3Info(sfileNamePath, fileData, nCID);
			//}modified by Divya 2012.01.07 for gracenote normalized db
            PT_END_TIME(PT_OBJ_MLDB_POPULATE, 0);
            LOG_TRACE <<("CGracenoteInterface :: MLDB populated with ID3") << LOG_ENDL;
            if(dbPopulated)
            {
                //Create the structure containing media file metadata for storing in tracker store
                PT_START_TIME(PT_OBJ_CREATE_MUSIC_DATA, 0);
                CreateTrackerMusicDataFromTextIDLookup(trackerMusicFiledata, fileData);
                PT_END_TIME(PT_OBJ_CREATE_MUSIC_DATA, 0);
                //No CoverArt can be retrieved as ELDB lookup failed

                //Populate SQLite DB
                m_NormalizedData->PopulateDB(trackerMusicFiledata, nCID); // added by Divya 2012.01.07 for gracenote normalized db
            }//(dbPopulated)
        }
        else
        {
            for(gn_uint32_t i = 1; i <= nmatchcount; i++)
            {
                //Get the result
                PT_START_TIME(PT_OBJ_ETC, 0);
                gn_textid_presult_data_t displayResult;
                nError = gn_textid_get_result( i, &displayResult );
                PT_END_TIME(PT_OBJ_ETC, 0);
                if(nError != GN_SUCCESS)
                {
                    return trackerMusicFiledata;
                }

                if(displayResult != GN_NULL)
                {
                    PT_START_TIME(PT_OBJ_ETC, 0);
                    bool result = CheckConfidence(matchsource, matchtype, i, fileData, displayResult);
                    PT_END_TIME(PT_OBJ_ETC, 0);
                    // If not confident about the lookup, look for next match
                    if(!result)
                    {
                        LOG_TRACE <<("Result for CheckConfidence returned false") << LOG_ENDL;
                        // Free the old structure
                        PT_START_TIME(PT_OBJ_TEXTID_LOOKUP, 0);
                        gn_textid_free_result(&displayResult);
                        nError = gn_textid_local_lookup(fileData, GN_TEXTID_LU_FLAG_NO_ALBUM,
                                                        &matchsource, &matchtype, &nmatchcount);
                        PT_END_TIME(PT_OBJ_TEXTID_LOOKUP, 0);
                        if(nError == GN_SUCCESS)
                        {
                            if(nmatchcount <= 0 )
                                return trackerMusicFiledata;
                            // Pick the first entry, which is the best match
                            PT_START_TIME(PT_OBJ_ETC, 0);
                            nError = gn_textid_get_result(1, &displayResult );
                            PT_END_TIME(PT_OBJ_ETC, 0);
                            if(nError != GN_SUCCESS)
                            {
                                return trackerMusicFiledata;
                            }
                        }
                    }
                    PT_START_TIME(PT_OBJ_ETC, 0);
                    //Convert the result for MLDB entry
                    gnplaylist_entrydata_t entryData;
                    entryData = GN_NULL;

                    gn_bool_t isOpened;
                    nError = gnplaylist_is_collection_id_open(nCID, &isOpened);

                    if(nError == GN_SUCCESS && isOpened == false)
                    {
                        //Open Collection ID
                        nError = gnplaylist_open_collection_id(nCID);
                    }

                    nError = gn_textid_result_to_entrydata(nCID, i, fileData, &entryData);

                    LOG_TRACE <<("gn_textid_result_entrydata") << LOG_ENDL;
                    LogError(nError);

                    PT_END_TIME(PT_OBJ_ETC, 0);

                    if(nError != GN_SUCCESS || entryData == GN_NULL)
                    {
                        //TODO : check if the MLDB can be populated with ID3 in this case
                        return trackerMusicFiledata;
                    }
                    else
                    {

                        PT_START_TIME(PT_OBJ_MLDB_POPULATE, 0);
                        LOG_TRACE << ("Before PopulateMLDB") << LOG_ENDL;
						//{modified by Divya 2012.01.07 for gracenote normalized db
                        //Populate MLDB
                        bool dbPopulated = true;
                        if(populateMLDBFlag)
                            dbPopulated = PopulateMLDB(sfileNamePath, entryData, nCID);
						//}modified by Divya 2012.01.07 for gracenote normalized db
                        PT_END_TIME(PT_OBJ_MLDB_POPULATE, 0);

                        if(dbPopulated)
                        {

                            PT_START_TIME(PT_OBJ_CREATE_MUSIC_DATA, 0);
                            //Create the structure containing media file metadata for storing in tracker store
                            CreateTrackerMusicDataFromTextIDLookup(trackerMusicFiledata, fileData);
                            PT_END_TIME(PT_OBJ_CREATE_MUSIC_DATA, 0);

                            //Populate SQLite DB
                            m_NormalizedData->PopulateDB(trackerMusicFiledata, nCID); //modified by Divya 2012.01.07 for gracenote normalized db
							//deleted by aettie Xsampa DB is not used.
                           // LOG_TRACE << ("Before calling ExtractXSampaData") << LOG_ENDL;

                            // Retrieve xsampa data for the current album, artist and genre
                            //PT_START_TIME(PT_OBJ_EXTRACT_XSAMPA, 0);
                            //Verify the size of the Xsampa DB file
                            //QString mldbpath;
                            //mldbpath.append("/dev/shm/XSampaDB");
                            //QFile mldbFileSize(mldbpath);
                            //Extract/update only if the max size of XSampaDB is NOT reached
                            //if(mldbFileSize.size() < XSampaDB_Max_Size)
                            //{
                            //    ExtractXSampaData(trackerMusicFiledata, displayResult, nCID);
                            //}
                            //PT_END_TIME(PT_OBJ_EXTRACT_XSAMPA, 0);
							//deleted by aettie Xsampa DB is not used.
                        }//END if(nError == GN_SUCCESS)

                        //Free memory
                        gnplaylist_freeentry( &entryData );

                    } //END else (nError != GN_SUCCESS || entryData == GN_NULL)
                    // Cache alternate phrases

                    // Free the look up result
                    gn_textid_free_result(&displayResult);

                    break;
                } //END if(displayResult != GN_NULL)
            }//END for
        }//END else
    }//END (nError == GN_SUCCESS)

    return trackerMusicFiledata;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::PopulateMLDB
 *
 * Description 	: Populate MLDB with the media file information

 * Parameters 	: sfileNamePath (IN) - path of the media file
 *				  nvolume (IN) - volume information

 * Returns 	: bool - True/False - indicating whether the entry
                            is added successfully or not
 *
 *******************************************************************
 */
bool CGracenoteInterface :: PopulateMLDB(QString sfileNamePath,
                                         gnplaylist_entrydata_t &entryData,
                                         gn_collection_id_t nCID)
{
    bool MLDBStatus = false;
    if(entryData == NULL)
    {
        return MLDBStatus;
    }

    gn_error_t nError = GN_FAILURE;

    if(m_pMLDBCreator)
    {
        //Populate the MLDB
        nError = m_pMLDBCreator->PopulateCollectionID(nCID, entryData, sfileNamePath);
        if(nError == GN_SUCCESS)
        {
            MLDBStatus = true;
        }
    }
    return MLDBStatus;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::PopulateMLDBWithID3Info
 *
 * Description 	: Populate MLDB with only ID3 information retrieved
                  from the media file

 * Parameters 	: sfileNamePath (IN) - path of the media file
 *				  nvolume (IN) - volume information

 * Returns 	: bool - True/False - indicating whether the entry
                  is added successfully or not
 *
 *******************************************************************
 */
bool CGracenoteInterface :: PopulateMLDBWithID3Info(QString sfileNamePath,
                                                    gn_pfile_data_t &fileData,
                                                    gn_collection_id_t nCID)
{
    gn_error_t nError = GN_FAILURE;
    bool MLDBStatus = false;

    QString qfileName = sfileNamePath.section('/', -1, -1);
    gn_uchar_t* gfileName = gnepal_strdup( (gn_uchar_t *)  (qfileName.toUtf8().constData()) );
    if(gfileName == GN_NULL)
        return MLDBStatus;

    gn_uchar_t* gfilePath = gnepal_strdup( (gn_uchar_t *)  (sfileNamePath.toUtf8().constData()) );

    //Open Collection ID
    nError = GN_FAILURE;
    nError =  gnplaylist_open_collection_id(nCID);

    if(nError != GN_SUCCESS)
        return MLDBStatus;

    //Initialize the entry data
    gnplaylist_entrydata_t entryData;
    entryData = GN_NULL;
    nError = gnplaylist_newentry (nCID, gfileName, gfilePath, &entryData);

    if(fileData != GN_NULL && nError == GN_SUCCESS)
    {
        //Set file name
        const gn_uchar_t*  filename = NULL;
        nError = gn_textid_file_data_get_filename(fileData, &filename);
        if(nError == GN_SUCCESS)
        {
            gnplaylist_setentryfieldval (entryData, gnpl_crit_field_file_name,
                                         const_cast<unsigned char*>(filename));
            LOG_INFO <<(reinterpret_cast<const char*>(filename)) << LOG_ENDL;
        }

        //Set file path
        const gn_uchar_t*  path = NULL;
        nError = gn_textid_file_data_get_path(fileData, &path);
        if(nError == GN_SUCCESS)
        {
            gnplaylist_setentryfieldval (entryData, gnpl_crit_field_path_name,
                                         const_cast<unsigned char*>(path));
            LOG_INFO <<(reinterpret_cast<const char*>(path)) << LOG_ENDL;

        }

        //Set track artist
        const gn_uchar_t*  track_artist = NULL;
        nError = gn_textid_file_data_get_track_artist(fileData, &track_artist);
        if(nError == GN_SUCCESS)
        {
            gnplaylist_setentryfieldval (entryData, gnpl_crit_field_track_artist_name,
                                         const_cast<unsigned char*>(track_artist));
            LOG_INFO <<(reinterpret_cast<const char*>(track_artist)) << LOG_ENDL;
        }

        //Set composer
        const gn_uchar_t*  composer = NULL;
        nError = gn_textid_file_data_get_composer(fileData, &composer);
        if(nError == GN_SUCCESS)
        {
            gnplaylist_setentryfieldval (entryData, gnpl_crit_field_track_composer,
                                         const_cast<unsigned char*>(composer));
            LOG_INFO <<(reinterpret_cast<const char*>(composer)) << LOG_ENDL;
        }

        //Set genre
        const gn_uchar_t*  genre = NULL;
        nError = gn_textid_file_data_get_genre(fileData, &genre);

        if(nError == GN_SUCCESS)
        {
            gnplaylist_setentryfieldval (entryData, gnpl_crit_field_album_genre,
                                         const_cast<unsigned char*>(genre));
            LOG_INFO <<(reinterpret_cast<const char*>(genre)) << LOG_ENDL;
        }

        //Set track title
        const gn_uchar_t*  track_title = NULL;
        nError = gn_textid_file_data_get_track_title(fileData, &track_title);

        if(nError == GN_SUCCESS)
        {
            gnplaylist_setentryfieldval (entryData, gnpl_crit_field_track_name,
                                         const_cast<unsigned char*>(track_title));
            LOG_INFO <<(reinterpret_cast<const char*>(track_title)) << LOG_ENDL;
        }

        //Set track number
        const gn_uchar_t*  track_num = NULL;
        nError = gn_textid_file_data_get_track_num(fileData, &track_num);
        if(nError == GN_SUCCESS)
        {
            gnplaylist_setentryfieldval (entryData, gnpl_crit_field_track_num,
                                         const_cast<unsigned char*>(track_num));
            LOG_INFO <<(reinterpret_cast<const char*>(track_num)) << LOG_ENDL;
        }

        //Set tag id
        const gn_uchar_t*  tag_id = NULL;
        nError = gn_textid_file_data_get_tagid(fileData, &tag_id);
        if(nError == GN_SUCCESS)
        {
            gnplaylist_setentryfieldval (entryData, gnpl_crit_field_tagid,
                                         const_cast<unsigned char*>(tag_id));
            LOG_INFO <<(reinterpret_cast<const char*>(tag_id)) << LOG_ENDL;
        }

        //Set original release year
        const gn_uchar_t*  org_release_yr = NULL;
        nError = gn_textid_file_data_get_original_release_year(fileData, &org_release_yr);
        if(nError == GN_SUCCESS)
        {
            gnplaylist_setentryfieldval(entryData, gnpl_crit_field_album_releaseyear,
                                        const_cast<unsigned char*>(org_release_yr));
            LOG_INFO <<(reinterpret_cast<const char*>(org_release_yr)) << LOG_ENDL;
        }

        //Set length
        const gn_uchar_t*  length = NULL;
        nError = gn_textid_file_data_get_length(fileData, &length);
        if(nError == GN_SUCCESS)
        {
            gnplaylist_setentryfieldval (entryData, gnpl_crit_field_file_length,
                                         const_cast<unsigned char*>(length));
            LOG_INFO <<(reinterpret_cast<const char*>(length)) << LOG_ENDL;
        }

        //Populate the MLDB
        nError = m_pMLDBCreator->PopulateCollectionID(nCID, entryData, sfileNamePath);
        if(nError == GN_SUCCESS)
        {
            MLDBStatus = true;
        }

    }//END if(fileData != GN_NULL && nError == GN_SUCCESS)

    //Free allocations
    if(gfileName != NULL) gnepal_mem_free(gfileName); gfileName = NULL;
    if(gfilePath != NULL) gnepal_mem_free(gfilePath); gfilePath = NULL;

    if(entryData != NULL) gnplaylist_freeentry(&entryData);
    return MLDBStatus;
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::DeleteMusicFileEntryFromGracenoteMLDB
 *
 * Description 	: Retrieve cover art from the latest text id lookup

 * Parameters 	: sfileNamePath (IN) - path of the media file
 *				  nvolume (IN) - volume information

 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface :: DeleteMusicFileEntryFromGracenoteMLDB(QStringList filesList,
                                                                  int nvolume, QString serialNum)
{
    gn_collection_id_t nCID =GN_INVALID_COLLECTION_ID;

    if(m_pMLDBCreator)
    {
        nCID = m_pMLDBCreator->RetrieveCollectionIDforVolume(nvolume, serialNum);
        if(nCID <= 0)
        {
            return;
        }
        else
        {
            //Delete the media file entry from CID
            for ( int index = 0; index <  filesList.count(); index++)
            {
                m_pMLDBCreator->DeleteMusicFileEntryInCollectionID(nCID, filesList.at(index));
            }

            //gnplaylist_close_collection_id(nCID);
        }
    }
	//{modified by Divya 2012.01.07 for gracenote normalized db
    //Delete entry from SQLite DB
    m_NormalizedData->DeleteRecords(nCID, filesList);
    LOG_INFO << "DeleteMusicFileEntryFromGracenoteMLDB end" << LOG_ENDL;
	//}modified by Divya 2012.01.07 for gracenote normalized db
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::CreateTrackerMusicDataFromTextIDLookup
 *
 * Description 	: Create the TrackerMusicData structure for populating
       in tracker-store

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface :: CreateTrackerMusicDataFromTextIDLookup(TrackerMusicData &trackerMusicFiledata,
                                                                   gn_pfile_data_t &fileData)
{
    gn_error_t nError = GN_FAILURE;

    if(fileData != GN_NULL)
    {
        const gn_uchar_t*  filename = NULL;
        nError = gn_textid_file_data_get_filename(fileData, &filename);
        if(nError == GN_SUCCESS)
        {
            trackerMusicFiledata.filename = QString::fromUtf8(reinterpret_cast<const char*>(filename));
            LOG_TRACE <<("CGracenoteInterface : trackerMusicFiledata.filename") << LOG_ENDL;
            LOG_INFO <<(trackerMusicFiledata.filename) << LOG_ENDL;
        }

        const gn_uchar_t*  path = NULL;
        nError = gn_textid_file_data_get_path(fileData, &path);
        if(nError == GN_SUCCESS)
        {
            trackerMusicFiledata.path = QString::fromUtf8(reinterpret_cast<const char*>(path));
            LOG_TRACE <<("CGracenoteInterface : trackerMusicFiledata.path") << LOG_ENDL;
            LOG_INFO <<(trackerMusicFiledata.path) << LOG_ENDL;
        }

        const gn_uchar_t*  track_artist = NULL;
        nError = gn_textid_file_data_get_track_artist(fileData, &track_artist);
        if(nError == GN_SUCCESS)
        {
            trackerMusicFiledata.track_artist = QString::fromUtf8(reinterpret_cast<const char*>(track_artist));
            LOG_TRACE <<("CGracenoteInterface : trackerMusicFiledata.track_artist") << LOG_ENDL;
            LOG_INFO <<(trackerMusicFiledata.track_artist) << LOG_ENDL;
        }

        const gn_uchar_t*  disc_title = NULL;
        nError = gn_textid_file_data_get_disc_title(fileData, &disc_title);
        if(nError == GN_SUCCESS)
        {
            trackerMusicFiledata.album = QString::fromUtf8(reinterpret_cast<const char*>(disc_title));
            LOG_TRACE <<("CGracenoteInterface : trackerMusicFiledata.album") << LOG_ENDL;
            LOG_INFO <<(trackerMusicFiledata.album) << LOG_ENDL;
        }

        const gn_uchar_t*  genre = NULL;
        nError = gn_textid_file_data_get_genre(fileData, &genre);
        if(nError == GN_SUCCESS)
        {
            trackerMusicFiledata.genre = QString::fromUtf8(reinterpret_cast<const char*>(genre));
            LOG_TRACE <<("CGracenoteInterface : trackerMusicFiledata.genre") << LOG_ENDL;
            LOG_INFO <<(trackerMusicFiledata.genre) << LOG_ENDL;
        }

        const gn_uchar_t*  track_title = NULL;
        nError = gn_textid_file_data_get_track_title(fileData, &track_title);
        if(nError == GN_SUCCESS)
        {
            trackerMusicFiledata.track_title = QString::fromUtf8(reinterpret_cast<const char*>(track_title));
            LOG_TRACE <<("CGracenoteInterface : trackerMusicFiledata.track_title") << LOG_ENDL;
            LOG_INFO <<(trackerMusicFiledata.track_title) << LOG_ENDL;
        }

        const gn_uchar_t*  composer = NULL;
        nError = gn_textid_file_data_get_composer(fileData, &composer);
        if(nError == GN_SUCCESS)
        {
            trackerMusicFiledata.composer = QString::fromUtf8(reinterpret_cast<const char*>(composer));
            LOG_TRACE <<("CGracenoteInterface : trackerMusicFiledata.composer") << LOG_ENDL;
            LOG_INFO <<(trackerMusicFiledata.composer) << LOG_ENDL;
        }

    }//END if(fileData != GN_NULL)

}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::ClearTrackerMusicData
 *
 * Description 	: Clear the data inside the TrackerMusicData structure

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::ClearTrackerMusicData(TrackerMusicData &trackerMusicFiledata)
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
 * Function	: CGracenoteInterface::FetchMediaDataFromCDTOCLookup
 *
 * Description 	: Fetch metadata information from the TOC(table of
       contents) provided from a CD

 * Parameters 	: tocInfo(IN) - TOC information

 * Returns 	: TrackerMusicData - TrackerMusicData structure populated
                    with information retrieved from CD lookup
 *
 *******************************************************************
 */
void CGracenoteInterface::FetchMediaDataFromCDTOCLookup(QString tocInfo)
{
    LOG_TRACE <<("CGracenoteInterface::FetchMediaDataFromCDTOCLookup Start");

    HighlightedVolume = e_Disc;
    LOG_INFO << "CGracenoteInterface::FetchMediaDataFromCDTOCLookup - HighlightedVolume" << HighlightedVolume << LOG_ENDL;
    if(m_pGNCDLookup)
    {

        // Delete any old data present for the same CD TOC
        //m_pGNCDLookup->DeleteCDData(tocInfo); //deleted by aettie for ISV_KR 79915 Audiocontroller already tried deleting Same CData 

        m_pGNCDLookup->SetTOC(tocInfo);
        m_pQueue->Enqueue(e_Disc, DISC);
    }

    LOG_TRACE <<("CGracenoteInterface::FetchMediaDataFromCDTOCLookup End");
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::GetCollectionId
 *
 * Description 	: Retrieve the collection id for specified volume

 * Parameters 	: nvolume - volume like usb, jukebox

 * Returns 	: gn_collection_id_t - collection id
 *
 *******************************************************************
 */
gn_collection_id_t CGracenoteInterface::GetCollectionId(int nvolume, QString serialNum)
{
    LOG_TRACE <<("CGracenoteInterface :: GetCollectionId: Start") << LOG_ENDL;

    //get the CID for the volume
    gn_collection_id_t nCID =GN_INVALID_COLLECTION_ID;

    if(m_pMLDBCreator)
    {
	//{modified by aettie 2013.03.22 for ITS 148383
        LOG_TRACE <<("CGracenoteInterface :: RetrieveCollectionIDforVolume nvolume : ") <<nvolume<<"serialNum : " <<serialNum<< LOG_ENDL;

        nCID = m_pMLDBCreator->RetrieveCollectionIDforVolume(nvolume, serialNum);
        LOG_TRACE <<("CGracenoteInterface :: RetrieveCollectionIDforVolume nCID : ") << nCID << LOG_ENDL;
        if(nCID <=0)
        {
            //Check if the total number of MLDBs created exceeds the maximum list
            m_pMLDBCreator->MLDBMaintenance(nvolume);

            //Create and store Collection ID
            nCID = m_pMLDBCreator->RegisterCollectionIDForVolumeObject(nvolume, serialNum);
            LOG_TRACE <<("CGracenoteInterface :: RegisterCollectionIDForVolumeObject :nCID : ") << nCID <<LOG_ENDL;
	//{modified by aettie 2013.03.22 for ITS 148383

            //Create SQLite DB for storing Gracenote metadata
            m_NormalizedData->CreateSQLiteDB(nCID); //added by Divya 2012.01.07 for gracenote normalized db

            LOG_TRACE <<("CGracenoteInterface :: GetCollectionId:") << LOG_ENDL;
            LOG_INFO <<(nCID) << LOG_ENDL;
        }
//{ modified by aettie.ji 2012.10.30 for static analysis #7101
        m_pMLDBCreator->CopyMLDBToRAM(nCID);
        m_pMLDBCreator->OpenCollectionID(nCID);
    }

    //m_pMLDBCreator->CopyMLDBToRAM(nCID);
    //m_pMLDBCreator->OpenCollectionID(nCID);
//} modified by aettie.ji 2012.10.30 for static analysis #7101

    //gnplaylist_enable_for_playlisting(nCID);
    LOG_TRACE <<("CGracenoteInterface :: GetCollectionId: End") << LOG_ENDL;
    return nCID;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::CheckIfEntryExistsInMLDB
 *
 * Description 	: Check if an entry is already present in the MLDB

 * Parameters 	: sfilePath - file path of the media
                          nvolume - volume information

 * Returns 	: bool - true if exists, else false
 *
 *******************************************************************
 */
bool CGracenoteInterface::CheckIfEntryExistsInMLDB(QString sfilePath, gn_collection_id_t nCID, QString fileName)
{
    LOG_TRACE <<(" CGracenoteInterface :: CheckIfEntryExistsInMLDB:") << LOG_ENDL;

    bool entryExists = false;

    if(nCID <= 0)
    {
        LOG_TRACE <<("CGracenoteInterface :: CheckIfEntryExistsInMLDB: Collection id < 0") << LOG_ENDL;
    }
    else
    {
        QFileInfo fileParser(sfilePath);

        gn_uchar_t* gfileName = NULL;

        // File name will come for IPOD,
        if(fileName.length() > 0)
        {
            gn_uchar_t* temp = gnepal_strdup( (gn_uchar_t *)  (fileName.toUtf8().constData()) );

            QString result = m_pGNOperationUtil->encodeString(temp);
            gfileName = gnepal_strdup((gn_uchar_t *)(result.toUtf8().constData()));

            if(temp != NULL) gnepal_mem_free(temp); temp = NULL;
        }
        else
        {
            gfileName = gnepal_strdup( (gn_uchar_t *)  (fileParser.fileName().toUtf8().constData()) );
        }
        gn_uchar_t* gfilePath = gnepal_strdup( (gn_uchar_t *)  (fileParser.filePath().toUtf8().constData()) );

        //check if the entry already exists
        gnplaylist_entrydata_t metadata = GN_NULL;
        gn_error_t nError =  gnplaylist_getentry (nCID, gfileName, gfilePath, NULL, &metadata);

        if( nError == DBERR_NotFound )
            entryExists = false;
        else if(nError == GN_SUCCESS && metadata != GN_NULL)
            entryExists = true;

        //Free memory
        if(gfileName != NULL) gnepal_mem_free(gfileName); gfileName = NULL;
        if(gfilePath != NULL) gnepal_mem_free(gfilePath); gfilePath = NULL;
        if(metadata != GN_NULL)
            gnplaylist_freeentry(&metadata);
    }

    LOG_TRACE <<("CGracenoteInterface :: CheckIfEntryExistsInMLDB End") << LOG_ENDL;
    return entryExists;
}


/*
 *******************************************************************
 * Function		: CGracenoteInterface::DeleteMediaDataForCD
 *
 * Description 	: Delete the CD data for a specific TOC from tracker store

 * Parameters 	: tocInfo - CD TOC information

 * Returns 		: None
 *
 *******************************************************************
 */
void CGracenoteInterface::DeleteMediaDataForCD(QString tocInfo)
{
    LOG_TRACE <<("CGracenoteInterface: DeleteMediaDataForCD called") << LOG_ENDL;
    m_pGNCDLookup->DeleteCDData(tocInfo);

    DisablePlaylistingForCD();

    LOG_TRACE <<("CGracenoteInterface: DeleteMediaDataForCD End") << LOG_ENDL;
}

/*
 *******************************************************************
 * Function		: CGracenoteInterface::InsertCoverArt
 *
 * Description 	: Inserts Gracenote ELDB coverart into Tracker Store
 *
 * Parameters 	: trackerMusicFiledata(TrackerMusicData) - Structure containing
 *                       music metadata from Gracenote ELDB
 *
 * Returns 		: None
 *
 *******************************************************************
 */
void CGracenoteInterface::InsertCoverArt(TrackerMusicData &trackerMusicFiledata, QImage &qimage)
{
    LOG_TRACE << "CGracenoteInterface::InsertCoverArt" << trackerMusicFiledata.imagePath << LOG_ENDL;

    QSize size = qimage.size();

    QString log;
    log.setNum(size.height());
    LOG_TRACE <<(log);
    log.setNum(size.width());
    LOG_TRACE <<(log);

    LOG_TRACE << "CGracenoteInterface::InsertCoverArt size" << size.height() << LOG_ENDL;
    LOG_TRACE << "CGracenoteInterface::InsertCoverArt size" << size.width() << LOG_ENDL;

    MediaArt::Info albumArt;

    if(trackerMusicFiledata.CD_TOC.length() != 0)
    {

        albumArt = MediaArt::Album(trackerMusicFiledata.album_artist, trackerMusicFiledata.album_title);
    }
    else
    {
        albumArt = MediaArt::Album(trackerMusicFiledata.track_artist, trackerMusicFiledata.album);
    }
    QString fileName = albumArt.potentialPath().toString();

    fileName = fileName.remove("file://");

    if(QFile::exists(fileName) == 0)
    {
        m_pGNCoverArt->SaveImageToCache(qimage, fileName);
        LOG_TRACE << " fileName created and set in Tracker: "<< fileName<< LOG_ENDL;
    }
    else
    {
        LOG_TRACE <<("Media art already present") << LOG_ENDL;
    }
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::AddIPODDataToGraceNoteMLDB
 *
 * Description 	: Updates MLDB with IPOD data which is read from IPod
                          SQL Master DB at tempfs
 *
 * Parameters 	: startIndex(int) - start index
                          endIndex(int) - end index
 *
 * Returns 	: EipodExecutionStatus - Showing the status of execution for IPOD
 *
 *******************************************************************
 */
EipodExecutionStatus CGracenoteInterface::AddIPODDataToGraceNoteMLDB(QString iPODSerialNum, int startIndex, int endIndex, int volume)
{
    LOG_TRACE <<("AddIPODDataToGraceNoteMLDB START") << LOG_ENDL;
    LOG_TRACE <<("iPODSerialNum") << LOG_ENDL;
    LOG_INFO <<(iPODSerialNum) << LOG_ENDL;
    int i = 0;
    int ConnectStatus = 0;
    EipodExecutionStatus returnStatus = e_Finished;

    PT_RESET_TIME;

    PT_START_TIME(PT_OBJ_READ_DATA_IPOD, 0);

    QVector<QStringList> dataList;
    LOG_TRACE <<("GetIpodDataFromTempfs1") << LOG_ENDL;

    dataList = m_pGNOperationUtil->ReadDataFromIPODSqlDB(iPODSerialNum, startIndex, endIndex);
    if(dataList.count() == 0)
    {
        returnStatus = e_Finished;
        return returnStatus;
    }

    PT_END_TIME(PT_OBJ_READ_DATA_IPOD, 0);

    gn_collection_id_t nCID = GetCollectionId(volume, iPODSerialNum);

    GetNormalizedDataObject()->CreateSQLiteDB(nCID); //added by Divya 2012.01.07 for gracenote normalized db

    LOG_TRACE << ("AddIPODDataToGraceNoteMLDB_Read") << LOG_ENDL;

    LOG_TRACE <<("Count of IPOD data") << LOG_ENDL;
    LOG_INFO <<(dataList.count()) << LOG_ENDL;
    QStringList ipodList;

    PT_SET_FILECOUNT(dataList.count());

    PT_START_TIME(PT_OBJ_UPDATE_MLDB_IPOD, 0);


    //LOG_INFO << "datalist.count and IPODindex" << dataList.count() << m_IPODIndex << LOG_ENDL ;
    for(i = 0; i < dataList.count(); i++)
    {
        ipodList = dataList.at(i);

        //Verify the maximum size of mldb on RAM and stop further population of DB
        if(m_pTrackerQuery->IsMLDBSizeLimitReached())
        {
            //Reset the flag to false
            m_pTrackerQuery->SetMLDBSizeState(false);
            LOG_TRACE <<("AddIPODDataToGraceNoteMLDB : Maximum size of mldb file is reached - Stop populating DB") << LOG_ENDL;
            break;
        }
        if((IPOD1ConnectStatus == 0 && volume == e_IPOD1) || (IPOD2ConnectStatus == 0 && volume == e_IPOD2))
        {
            ConnectStatus = m_pMLDBCreator->GetConnectStatus(iPODSerialNum);
            if(ConnectStatus == 0)
            {
                /* reset the index and clear the list */
                dataList.clear();
                returnStatus = e_Unplugged;

                //Move SQlite DB to filesystem
                m_NormalizedData->CopySQLiteDBtoFS(nCID); //added by Divya 2012.01.07 for gracenote normalized db
                break;
            }
        }
        else if(HighlightedVolume != volume)
        {
            LOG_INFO << "STOP updating and enqueue IPOD back" << LOG_ENDL;
            /* preemption occured enqueue back to update MLDB for rest of the files */
            if(volume == e_IPOD1)
            {
                m_pQueue->Enqueue(e_IPOD1, IPOD);
            }
            else if(volume == e_IPOD2)
            {
                m_pQueue->Enqueue(e_IPOD2, IPOD);
            }
            returnStatus = e_InProgress;

            // Close the currently being populated MLDB
            m_pMLDBCreator->CloseCollectionID(nCID);
            break;
        }

        UpdateMLDBWithIPODData(ipodList, nCID);


        LOG_TRACE <<("UpdateMLDBWithIPODData called") << LOG_ENDL;
    }//end of for
    /* If no preemption reset the index and clear the list */
    if(i == dataList.count())
    {
        dataList.clear();
    }

    // Close the collections, in case population is complete
    if(m_FrontIPODSignalCount == m_FrontIPODSignalIndex)
    {
        //Update the MLDB population state
        m_pMLDBCreator->UpdateMLDBState(1, iPODSerialNum);
        m_pMLDBCreator->CloseCollectionID(nCID);

        //Copy XSampaDB to FS and emit signal to VR
        //BackupXsampaToFS(); //deleted by aettie Xsampa DB is not used.
        //{modified by esjang 2012.11.14 for XSampa DB update with Volume Info
        //XSampaDataUpdatedInDB();
        //XSampaDataUpdatedInDB(volume); //deleted by aettie Xsampa DB is not used.
        //}modified by esjang 2012.11.14 for XSampa DB update with Volume Info
    }
    else if( m_RearIPODSignalCount == m_RearIPODSignalIndex)
    {
        m_pMLDBCreator->UpdateMLDBState(1, iPODSerialNum);
        m_pMLDBCreator->CloseCollectionID(nCID);

        //Copy XSampaDB to FS and emit signal to VR
       // BackupXsampaToFS(); //deleted by aettie Xsampa DB is not used.
        //{ modified by esjang 2012.11.14 for XSampa DB update with Volume Info
        //XSampaDataUpdatedInDB();
       // XSampaDataUpdatedInDB(volume); //deleted by aettie Xsampa DB is not used.
        //} modified by esjang 2012.11.14 for XSampa DB update with Volume Info

    }

    PT_END_TIME(PT_OBJ_UPDATE_MLDB_IPOD, 0);

    PT_PRINT_TIME(PT_LOG_FILE_NAME);
    LOG_TRACE <<("AddIPODDataToGraceNoteMLDB End") << LOG_ENDL;

    //Move SQlite DB to filesystem
    m_NormalizedData->CopySQLiteDBtoFS(nCID); //added by Divya 2012.01.07 for gracenote normalized db

    return returnStatus;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::UpdateMLDBWithIPODData
 *
 * Description 	: Updates MLDB with IPOD data
 *
 * Parameters 	: iPodMetadata(QStringList) - List containing iPod metadata
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::UpdateMLDBWithIPODData(QStringList iPodMetadata, gn_collection_id_t nCID)
{
    TrackerMusicData trackerMusicFiledata;
    ClearTrackerMusicData(trackerMusicFiledata);

    gn_pfile_data_t fileData;
    fileData = GN_NULL;

    gn_error_t nError = gn_textid_file_data_init(&fileData);
    if(nError != GN_SUCCESS)
        return;

    QString filePath = iPodMetadata.at(5);
    QString fileName =  iPodMetadata.at(0);

    //check if entry is already present in MLDB
    bool entryExists = CheckIfEntryExistsInMLDB(filePath.toUtf8().constData(), nCID, fileName);

    // If entry is already there in MLDB, no need to update tracker
    // return empty data structure
    if(entryExists)
    {
        LOG_TRACE <<("CGracenoteInterface :: Entry exists") << LOG_ENDL;
        return;
    }

    if(m_pGNOperationUtil->SetIPODID3Info(iPodMetadata, fileData))
    {
        LOG_TRACE <<("CGracenoteInterface :: Entry does not exist") << LOG_ENDL;

        trackerMusicFiledata = PerformTextIDLookup(filePath.toUtf8().constData() , fileData, nCID);

        // Clean and end the lookup
    	gn_textid_cleanup_lookup();
    	gn_textid_cache_free_all();
    	gn_textid_lookup_end();

    }//END (nError == GN_SUCCESS)

    //Free the file data
    gn_textid_file_data_smart_free( &fileData );

    LOG_TRACE << " CGracenoteInterface::UpdateMLDBWithIPODData Exit" << LOG_ENDL;

}


/*
 *******************************************************************
 * Function		: CGracenoteInterface::DisablePlaylistingForIPOD
 *
 * Description 	: Disables playlisting for IPOD
 *
 * Parameters 	: iPODSerialNum(QString) - IPOD serial num
 *
 * Returns 		: None
 *
 *******************************************************************
 */
void CGracenoteInterface::DisablePlaylistingForIPOD(QString iPODSerialNum, int volume)
{
    LOG_TRACE <<("CGracenoteInterface::DisablePlaylistingForIPOD  called") << LOG_ENDL;
    gn_collection_id_t nCID =GN_INVALID_COLLECTION_ID;

    //get the CID created for IPOD
    nCID = m_pMLDBCreator->RetrieveCollectionIDforVolume(volume, iPODSerialNum);

    //Disable for playlisting
    if(nCID > 0) {
	//Close CID
	gn_error_t nError = m_pMLDBCreator->CloseCollectionID(nCID);
        if(nError == GN_SUCCESS) {
            LOG_TRACE <<("Collection ID for IPOD closed") << LOG_ENDL;
        }
    }
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::CheckConfidence
 *
 * Description 	: Checks the match confidence of the TextID lookup
 *
 * Parameters 	: sourceType(gn_textid_match_source_t) - Source Type
 *		  matchType(gn_textid_match_type_t) - match Type
 *		  index(int) - index
 *		  fileData(gn_pfile_data_t) - fileData after TexID lookup
 *		  displayResult(gn_textid_presult_data_t) - results after TexID lookup
 * Returns 	: Bool
 *
 *******************************************************************
 */
bool CGracenoteInterface::CheckConfidence(gn_textid_match_source_t sourceType,
                                          gn_textid_match_type_t matchType,
                                          int index,
                                          gn_pfile_data_t &fileData,
                                          gn_textid_presult_data_t &displayResult)
{
    LOG_TRACE <<("CGracenoteInterface::CheckConfidence Start") << LOG_ENDL;

    Q_UNUSED(matchType);
    Q_UNUSED(index);

    gn_error_t nError = GN_FAILURE;

    bool result =  true;

    gn_uint32_t confidence = 0;
    if(sourceType == GN_TEXTID_MATCH_SOURCE_ALBUM)
    {
        nError =  gn_textid_get_album_artist_match_confidence(fileData, displayResult, &confidence);
        if(nError == GN_SUCCESS )
        {
            if(confidence < 66)
            {
                LOG_TRACE <<("CGracenoteInterface::Album artist match confidence is ") << LOG_ENDL;
                LOG_INFO <<(confidence) << LOG_ENDL;
                result = false;
            }
        }
    }

    /*if(matchType == GN_TEXTID_MATCH_TYPE_SINGLE_FUZZY || matchType == GN_TEXTID_MATCH_TYPE_MULTI_FUZZY)
    {

        gn_uint32_t correlation;

        nError =  gn_textid_get_correlation_at_index(index, &correlation);

        // if value is less than 68, could be false positive
        if(nError == GN_SUCCESS )
        {
            if(correlation < 68)
            {
                LOG_TRACE <<("CGracenoteInterface::Correlation at index");
                LOG_TRACE <<(correlation);
                result = false;
            }
        }
    }*/

    LOG_TRACE <<("CGracenoteInterface::CheckConfidence End") << LOG_ENDL;
    return result;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::UpdateMLDBWithCDData
 *
 * Description 	: Updates MLDB with IPOD data
 *
 * Parameters 	: musicData(TrackerMusicData) - Structure containing
 *                        data about the songs present in a CD
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::UpdateMLDBWithCDData(TrackerMusicData &musicData)
{

    gn_pfile_data_t fileData;
    fileData = GN_NULL;

    //Extract the required data from TrackerMusicData object
    QStringList cdDataList;

    //Create CID
    gn_collection_id_t nCID = GetCollectionId(e_Disc);

    for(int index = 0 ; index < musicData.trackData.count(); index ++)
    {
        gn_error_t nError = gn_textid_file_data_init(&fileData);

        if(nError != GN_SUCCESS)
            return;


        SetListWithCDData(musicData, cdDataList, index);

        if(m_pGNOperationUtil->SetID3InfoForCD(cdDataList, fileData))
        {
            // Check if entry exists not required, as entry should always be updated

            LOG_TRACE <<("UpdateMLDBWithCDData PerformTextIDLookup called")<< LOG_ENDL;

            // Return not to be used, as the data is already populated in tracker
            PerformTextIDLookup(cdDataList.at(4).toUtf8().constData() , fileData, nCID);

            // Clean and end the lookup
            gn_textid_cleanup_lookup();
            gn_textid_cache_free_all();
            gn_textid_lookup_end();
        }
        else
        {
            LOG_TRACE <<("UpdateMLDBWithCDData Error") << LOG_ENDL;
        }

        //Free the file data
        gn_textid_file_data_smart_free( &fileData );
    }

    //Close Collection ID
    m_pMLDBCreator->CloseCollectionID(nCID);
    gn_apm_cache_backup();

}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::DisablePlaylistingForCD
 *
 * Description 	: Disables playlisting for IPOD
 *
 * Parameters 	: None
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::DisablePlaylistingForCD()
{
    LOG_TRACE <<("CGracenoteInterface::DisablePlaylistingForCD start") << LOG_ENDL;

    gn_collection_id_t nCID =GN_INVALID_COLLECTION_ID;

    //get the CID created for IPOD
    nCID = m_pMLDBCreator->RetrieveCollectionIDforVolume(e_Disc);

    //Disable for playlisting
    if(nCID > 0) {
        //Close CID
        m_pMLDBCreator->DeleteCollectionID(nCID);
    }

    LOG_TRACE <<("CGracenoteInterface::DisablePlaylistingForCD end") << LOG_ENDL;

}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::SetListWithCDData
 *
 * Description 	: Creates data to be populated in TS
 *
 * Parameters 	: musicData - TrackerMusicData structure
                          cdDataList - list to be popuated with metadata
                          index - index of entry to be taken from TrackerMusicData
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::SetListWithCDData(TrackerMusicData &musicData,
                                            QStringList &cdDataList, int index)
{
    cdDataList.clear();
    cdDataList.append(musicData.album_title);

    TrackerMusicTrackData trackData = musicData.trackData.at(index);

    // In case track artist is not present, use album artist
    if(trackData.track_artist.length() > 0)
    {
        cdDataList.append(trackData.track_artist);
    }
    else
    {
        cdDataList.append(musicData.album_artist);
    }

    cdDataList.append(trackData.track_title);
    cdDataList.append(trackData.track_genre);
    cdDataList.append(trackData.track_path);
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::OnCDLookupComplete
 *
 * Description 	: Slot will be hit once the CD Lookup is complete
 *
 * Parameters 	: musicData - TrackerMusicData structure
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::OnCDLookupComplete(TrackerMusicData musicData)
{
    LOG_TRACE <<("CGracenoteInterface::OnCDLookupComplete Start") << LOG_ENDL;

    bool musicIDCDStatus = false;

    if(musicData.trackData.count() == 0)
    {
        emit OnMusicIDCDDataReceived(musicIDCDStatus);
        LOG_TRACE <<("CGracenoteInterface::OnMusicIDCDDataReceived emitted with false") << LOG_ENDL;
    }
    else
    {
        // Update the data in tracker store
        //musicIDCDStatus = m_pTrackerQuery->UpdateQuery(musicData);

        musicIDCDStatus = m_pGNCDLookup->InsertCDDAData(musicData);
        LOG_TRACE <<("CGracenoteInterface::OnMusicIDCDDataReceived emitted with musicIDCDStatus") << LOG_ENDL;

        emit OnMusicIDCDDataReceived(musicIDCDStatus);
        LOG_TRACE <<("CGracenoteInterface::emitSignalForGracenoteLogo type = e_Disc")  << LOG_ENDL;

        m_pQueue->emitSignalForGracenoteLogo(e_Disc, 1); //added by aettie.ji 20130904 for gracenote logo

        // Populate the data in MLDB post doing lookup
        UpdateMLDBWithCDData(musicData);
    }

    LOG_TRACE <<("CGracenoteInterface::OnCDLookupComplete End") << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::RetrieveCoverArt
 *
 * Description 	: API called over DBus for retrieving cover art
                          for a specific metadata for MusicPlayer
 *
 * Parameters 	: metaDataList - meta data list
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::RetrieveCoverArt(QStringList metaDataList)
{
//{modified by aettie 2013.03.19 for Gracenote issue
   // LOG_TRACE <<("CGracenoteInterface:: RetrieveCoverArt Enter") << LOG_ENDL;

//    if(m_pGNCoverArt)
 //   {
   //     m_IsMultipleCoverRequest = false;
    //    LOG_TRACE <<("CGracenoteInterface::RetrieveCoverArt call");
      //  m_pGNCoverArt->RetrieveCoverArtForMP(metaDataList);
   // }

   // LOG_TRACE <<("CGracenoteInterface::RetrieveCoverArt Exit")  << LOG_ENDL;    
    LOG_TRACE <<("CGracenoteInterface::RetrieveCoverArt Enter") << LOG_ENDL;

    // List of album and artist will have same number of entries. Even if artist is not present,
    // the list will contain null string. We will set just the album name to metadata and cover
    // art will be retrieved accordingly. This API is called from TrackerAbstractor. The count
    // difference handling is not required.

    HighlightedVolume = e_CA;
    LOG_INFO << "CGracenoteInterface::RetrieveCoverArt - HighlightedVolume" << HighlightedVolume << LOG_ENDL;
    if(m_pGNCoverArt)
    {
        LOG_TRACE <<("CGracenoteInterface:RetrieveCoverArt Enqueue call");
        m_pGNCoverArt->SetMetadataForCA(metaDataList);
        m_pQueue->Enqueue(e_CA, CA);
    }
//}modified by aettie 2013.03.19 for Gracenote issue
    LOG_TRACE <<("CGracenoteInterface::RetrieveCoverArt Exit")  << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::onCoverArtRetrieved
 *
 * Description 	: Slot hit once cover art is retrieved
 *
 * Parameters 	: status - true is cover art retrieved else false
                          imagePath - variable containing image path
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::onCoverArtRetrieved(bool status, QString imagePath)
{
    LOG_TRACE <<("::RetrievedCoverArt Slot Enter")  << LOG_ENDL;
    //modified by aettie 20130610 for Signal to TA when CoverArt not found
        if(!m_IsMultipleCoverRequest)
        {
    	//{modified by aettie.ji 2013.02.22 for ISV 71131 etc.
        	QString imagePathRetrieved;

    	      if(status)
            {
                LOG_TRACE <<("Cover art retrieved from Gracenote")  << LOG_ENDL;
                imagePathRetrieved = imagePath;
        		LOG_TRACE <<("emitSignalForGracenoteLogo type = e_CA")  << LOG_ENDL;
                //m_pQueue->emitSignalForGracenoteLogo(e_CA, 1);  //deleted by aettie 20131010 unused code
            }
            else
            {
                LOG_TRACE <<("Cover art not available from Gracenote")  << LOG_ENDL;

                imagePathRetrieved = "No CoverArt Found";
            }
            
    	        emit CoverArtRetrieved(status, imagePathRetrieved);
        //}modified by aettie.ji 2013.02.22 for ISV 71131 etc.
        }
        else
        {
            if(status)
            {
                LOG_TRACE <<("Cover art retrieved from Gracenote")  << LOG_ENDL;
                m_CoverArtList.append(imagePath);
            }
            else
            {
                LOG_TRACE <<("Cover art not available from Gracenote")  << LOG_ENDL;
                m_CoverArtList.append("No CoverArt Found");
            }

            if(m_CoverArtList.count() == m_CoverRequestCount)
            {
                LOG_TRACE <<("Requested Cover Count == Retrived Cover Count:: emit CoverArtsRetrieved(m_CoverArtList)")  << LOG_ENDL;
                 //deleted by aettie 20131010 unused code
				//{added by aettie.ji 20130904 for gracenote logo
			//	LOG_TRACE <<("emitSignalForGracenoteLogo type = e_MultiCA")  << LOG_ENDL;
                //if(m_CoverArtList.count()==1 && m_CoverArtList.at(0)=="No CoverArt Found")
                //{
                  //  m_pQueue->emitSignalForGracenoteLogo(e_MultiCA, 0);
                //}
                //else
                //{
                  //  m_pQueue->emitSignalForGracenoteLogo(e_MultiCA, 1);
                //}
				//}added by aettie.ji 20130904 for gracenote logo
                emit CoverArtsRetrieved(m_CoverArtList);
            }
            else
            {
                QStringList imagePathNotFound;
                imagePathNotFound.append("No CoverArt Found");
                LOG_TRACE <<("Requested Cover Count != Retrived Cover Count:: emit CoverArtsRetrieved(NULL)")  << LOG_ENDL;
                emit CoverArtsRetrieved(imagePathNotFound);

            }
                
        }
	//modified by aettie 20130610 for Signal to TA when CoverArt not found

    LOG_TRACE <<("CGracenoteInterface::RetrievedCoverArt Slot Exit")  << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::LogError
 *
 * Description 	: Log error
 *
 * Parameters 	: error - Error code
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::LogError(gn_error_t error)
{
    const gn_uchar_t* errorStr = gnerr_get_code_desc( error);

    QString txt;
    txt.append (QString("Error: %1").arg(reinterpret_cast<const char*>(errorStr)));

    LOG_TRACE << txt << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::EnableMLTForVolume
 *
 * Description 	: Enable MoreLikeThis only for a specified volume
 *                       like either Jukebox, USB or IPod
 *
 * Parameters 	: volume - 0 - Jukebox, 1 - USB, 2 - IPOD
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::EnableMLTForVolume(int volume,QString serialNum, QString mountPath)
{
    LOG_TRACE << "EnableMLTForVolume Enter" << LOG_ENDL;

    // Disable all other collections from playlisting
    m_pMLDBCreator->DisableAllCollectionsForPlaylisting();

    // Enable specified for playlisting
    m_pMLDBCreator->EnableForPlaylisting(volume, serialNum, mountPath);

    LOG_TRACE << "EnableMLTForVolume End" << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::ExtractXSampaData
 *
 * Description 	: Extract XSampa data for album, artist and genre
 *
 * Parameters 	: musicData - TrackerMusicData structure
 *                        displayResult - gracenote structure
 *
 * Returns 	: None
 *
 *******************************************************************
 *
 //deleted by aettie Xsampa DB is not used.
void CGracenoteInterface::ExtractXSampaData(TrackerMusicData &musicData, gn_textid_presult_data_t &displayResult, int nCID)
{
    LOG_TRACE << "CGracenoteInterface: ExtractXSampaData, Enter" << LOG_ENDL;

    QVector<QStringList> completePhoneticData;
    QVector<QStringList> phoneticData;
    completePhoneticData.clear();

    QString type;
    bool exists = false;

    // For Album
    type.append("Album");

    exists = m_pTrackerQuery->CheckIfPhoneticDataExists(type, musicData.album);

    if(!exists)
    {
        LOG_TRACE << "xsampa for album not present, so fetch" << LOG_ENDL;
        m_pGNOperationUtil->GetXSampaDataForAlbum(musicData.album,musicData.id3album, displayResult);
        m_pGNOperationUtil->GetXSampaList(phoneticData);
    }
     else
    {
       LOG_TRACE << "xsampa for album is present" << LOG_ENDL;
       QStringList data;
       data.clear();

       data.append(type);
       data.append(musicData.album);
       QString str = QString::number(nCID);
       m_pTrackerQuery->UpdateCIDInfoInXSampaDB(str, data);
    }

    // Populate in complete list
    for (int index = 0; index <  phoneticData.count(); index++)
    {
        completePhoneticData.append(phoneticData.at(index));
    }

    //For artist
    type.clear();
    phoneticData.clear();
    type.append("Artist");

    exists = m_pTrackerQuery->CheckIfPhoneticDataExists(type, musicData.track_artist);

    if(!exists)
    {
        LOG_TRACE << "xsampa for artist not present, so fetch" << LOG_ENDL;
        m_pGNOperationUtil->GetXSampaDataForArtist(musicData.track_artist, musicData.id3artist, displayResult);
        m_pGNOperationUtil->GetXSampaList(phoneticData);
    }
     else
    {
       LOG_TRACE << "xsampa for artist is present" << LOG_ENDL;
       QStringList data;
       data.clear();

       data.append(type);
       data.append(musicData.track_artist);

       QString str = QString::number(nCID);
       m_pTrackerQuery->UpdateCIDInfoInXSampaDB(str, data);
    }

    // Populate in complete list
    for (int index = 0; index <  phoneticData.count(); index++)
    {
        completePhoneticData.append(phoneticData.at(index));
    }

    // Update whole data in the XSampa Database
    if(completePhoneticData.count() > 0)
	{
        QString str = QString::number(nCID);
        m_pTrackerQuery->UpdatePhoneticData(str, completePhoneticData);
	}

    //Clear the list
    completePhoneticData.clear();

    LOG_TRACE << "ExtractXSampaData, Exit" << LOG_ENDL;
}*/


/*
 *******************************************************************
 * Function	: CGracenoteInterface::XSampaDataUpdatedInDB
 *
 * Description 	: Notify Application about XSampa data population
 *
 * Parameters 	: None
 *
 * Returns 	: None
 *
 *******************************************************************
 *
 //deleted by aettie Xsampa DB is not used.
//{modified by esjang 2012.11.14 for XSampa DB update with Volume Info
//void CGracenoteInterface::XSampaDataUpdatedInDB()
void CGracenoteInterface::XSampaDataUpdatedInDB(int volume)
//}modified by esjang 2012.11.14 for XSampa DB update with Volume Info
{
    LOG_TRACE << "CGracenoteInterface : XSampaDataUpdatedInDB" << LOG_ENDL;
    //{modified by esjang 2012.11.14 for XSampa DB update with Volume Info
    //emit XSampaDataUpdated();
    emit XSampaDataUpdated(volume);
    //}modified by esjang 2012.11.14 for XSampa DB update with Volume Info

}*/


/*
 *******************************************************************
 * Function	: CGracenoteInterface::CloseCollectionIDs
 *
 * Description 	: Close all the open collection id's
 *
 * Parameters 	: None
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::CloseCollectionIDs()
{
    LOG_TRACE << "CGracenoteInterface : CloseCollectionIDs Start" << LOG_ENDL;

    m_pMLDBCreator->CloseCollections();

    //Reset connect Status to 0 for all devices
    int connectStatus = 0;
    m_pMLDBCreator->UpdateConnectStatus(connectStatus);

    /*
	//Below change is for deleting the DB file from tmpfs
    //Get the list of CID's and move the SQLite DB files to FS
    //get list of collection IDs
    gn_collection_id_list_t *list;
    gnplaylist_get_collection_id_list(&list);

    if(list->id_count > 0)
    {
       for(int index=0; index<list->id_count; index++)
        {
          //m_NormalizedData->CopySQLiteDBtoFS(list->ids->at(index));
       }
    }

    //Free the collection id list
    gnplaylist_free_collection_id_list(&list);*/

    LOG_TRACE << "CGracenoteInterface : CloseCollectionIDs End" << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::USBConnected
 *
 * Description 	: This function is called when the user plugs a USB
 *
 * Parameters 	: QString serialNum - UUID in case of USB of the connected USB
 *                        QString mountPath - Mount Path of the connected USB
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::USBConnected(QString serialNum, QString mountPath, int volume)
{
    LOG_CRITICAL << "CGracenoteInterface::USB - USB Connected HighlightedVolume " << HighlightedVolume << " And Volume = " << volume  << LOG_ENDL;
        // Update the serialNum and mountPath in DB based on volume for later use
    gn_collection_id_t nCID = GetCollectionId(volume, serialNum);
	//{modified by aettie 2013.03.22 for ITS 148383
    LOG_INFO << "CGracenoteInterface::USB - USB Connected nCID : " << nCID <<" serialNum: "<< serialNum <<LOG_ENDL;
    m_pMLDBCreator->UpdateMountPathInDB(mountPath , nCID);

    if(volume == e_USB1)
    {
        LOG_INFO << "CGracenoteInterface::e_USB1 - Enqueueing USB" << LOG_ENDL;
        USB1ConnectStatus = 1;
        //Update the connect status of USB as 1 in DB based on serial num
        m_pMLDBCreator->UpdateConnectStatus(USB1ConnectStatus, volume, serialNum);
        qDebug() << "enqueue e_USB1";
        HighlightedVolume = e_USB1;
        m_pQueue->Enqueue(e_USB1, USB);
    }
    else if(volume == e_USB2)
    {
        LOG_INFO << "CGracenoteInterface::e_USB2 - Enqueueing USB" << LOG_ENDL;
        USB2ConnectStatus = 1;
        //Update the connect status of USB as 1 in DB based on serial num
        m_pMLDBCreator->UpdateConnectStatus(USB2ConnectStatus, volume, serialNum);
        qDebug() << "enqueue e_USB2";
        HighlightedVolume = e_USB2;
        m_pQueue->Enqueue(e_USB2, USB);
    }
	//}modified by aettie 2013.03.22 for ITS 148383
    LOG_INFO << "CGracenoteInterface::USB - After Enqueueing USB GNqueue.count()" << m_pQueue->Count() << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::IPODConnected
 *
 * Description 	: This function is called when the user plugs a IPOD
 *
 * Parameters 	: QString serialNum - Serial Num of the connected IPOD
 *                        int StartIndex - StartIndex of the IPOD metadata populated in the SQLite DB
 *                        int EndIndex - End Index of the IPOD metadata populated in SQLite DB
 *                        int volume - Indicating IPOD1 or IPOD2
 *                        int syncState - Indicating Sync=0/Resync=2/NoSync=1/SyncComplete=3
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::IPODConnected(QString serialNum, int startIndex, int endIndex, int syncState, int volume)
{
    LOG_INFO << "CGracenoteInterface::IPODConnected - Enter" << serialNum << " " << startIndex << " ~ " << endIndex << " : " << syncState << LOG_ENDL;


    //When IPOD Disconnect signal is emitted, this value is set to 0.
    if(volume == e_IPOD1)
    {
        IPOD1ConnectStatus = 1;
        LOG_INFO << "CGracenoteInterface::IPODConnected IPOD1ConnectStatus = 1" << LOG_ENDL;
    }
    else if(volume == e_IPOD2)
    {
        IPOD2ConnectStatus = 1;
        LOG_INFO << "CGracenoteInterface::IPODConnected IPOD2ConnectStatus = 1" << LOG_ENDL;
    }

    HighlightedVolume = volume;

    //SyncState = 0, means complete Sync
    if(syncState == 0)
    {

        emit GracenoteIndexingStatus(0, volume);
        LOG_TRACE << "Sync state for iPod" << syncState << ":" << startIndex << ":" << endIndex << LOG_ENDL;

        SetIPODinfo(serialNum, startIndex, endIndex, syncState, volume);

        LOG_INFO << "CGracenoteInterface::IPODConnected - IPOD Connected HighlightedVolume" << HighlightedVolume << LOG_ENDL;
        LOG_INFO << "CGracenoteInterface::IPODConnected - Enqueueing IPOD" << LOG_ENDL;
        if(volume == e_IPOD1)
            {
                m_pQueue->Enqueue(e_IPOD1, IPOD);
            }
        else if(volume == e_IPOD2)
            {
                m_pQueue->Enqueue(e_IPOD2, IPOD);
            }
    }
    else if ((syncState == 2) || (syncState == 3))// Resync, Sync Complete
    {
        LOG_TRACE << "Sync State for iPod" <<  syncState << LOG_ENDL;

        if(syncState == 2)
        {
            // In case of resync, emit start status here
            // End status will be emitted in PerformResyncForIPOD function
            emit GracenoteIndexingStatus(0, volume);
        }

        //Handle sync complete state only for UID based devices to get the deleted track list
        //Resync state is emitted only for Index based devices
        if(( (syncState == 3) &&  (!m_pGNOperationUtil->CheckIPODType(serialNum)) ) || (syncState == 2))
        {
            //Changing syncState to Resync
            syncState = 2;

            // Set just the serial number value in the IPOD Info
            SetIPODinfo(serialNum, 0, 0, syncState, volume);

            if(volume == e_IPOD1)
            {
                m_pMLDBCreator->UpdateConnectStatus(IPOD1ConnectStatus, volume, serialNum); 	//modified by aettie 2013.03.22 for ITS 148383
                m_pQueue->Enqueue(e_IPOD1, IPOD);
            }
            else if(volume == e_IPOD2)
            {
                m_pMLDBCreator->UpdateConnectStatus(IPOD2ConnectStatus, volume, serialNum);		//modified by aettie 2013.03.22 for ITS 148383
                m_pQueue->Enqueue(e_IPOD2, IPOD);

            }
        }

        if(syncState == 3)
        {
            // As sync state is coming as 3, Indexing will be complete for the iPod device
            // Resync case indexing state will not be handled for indexing
            emit GracenoteIndexingStatus(1, volume);

            //{modified by esjang 2012.11.14 for XSampa DB update with Volume Info
            //emit XSampaDataUpdated(volume); //deleted by aettie Xsampa DB is not used.
            //}modified by esjang 2012.11.14 for XSampa DB update with Volume Info
        }
    }
    else  //No Sync
    {
        //Check the population state
        int populationState = m_pMLDBCreator->GetMLDBState(serialNum);

        //If population state = 0 then perform sync and then resync
        //This is to avoid any tracks which are not populated in MLDB due to IPOD being unplugged
        if(populationState !=1)
        {
            syncState = 0;
            SetIPODinfo(serialNum, startIndex, endIndex, syncState, volume);

            LOG_INFO << "CGracenoteInterface::IPODConnected - IPOD Connected HighlightedVolume" << HighlightedVolume << LOG_ENDL;
            LOG_INFO << "CGracenoteInterface::IPODConnected - Enqueueing IPOD" << LOG_ENDL;
            if(volume == e_IPOD1)
                {
                    m_pMLDBCreator->UpdateConnectStatus(IPOD1ConnectStatus, volume, serialNum); 	//modified by aettie 2013.03.22 for ITS 148383
                    m_pQueue->Enqueue(e_IPOD1, IPOD);
                }
            else if(volume == e_IPOD2)
                {
                    m_pMLDBCreator->UpdateConnectStatus(IPOD2ConnectStatus, volume, serialNum);		//modified by aettie 2013.03.22 for ITS 148383
                    m_pQueue->Enqueue(e_IPOD2, IPOD);
                }
        }
        else
        {
            if(volume == e_IPOD1)
            {
               m_pMLDBCreator->UpdateConnectStatus(IPOD1ConnectStatus, volume, serialNum);		//modified by aettie 2013.03.22 for ITS 148383
            }
            else if(volume == e_IPOD2)
            {
                m_pMLDBCreator->UpdateConnectStatus(IPOD2ConnectStatus, volume, serialNum);		//modified by aettie 2013.03.22 for ITS 148383
            }
            //{modified by esjang 2012.11.14 for XSampa DB update with Volume Info
            //emit XSampaDataUpdated(volume); //deleted by aettie Xsampa DB is not used.
            //}modified by esjang 2012.11.14 for XSampa DB update with Volume Info
        }
    }
    LOG_INFO << "CGracenoteInterface::IPODConnected - After Enqueueing IPOD GNqueue.count()" << m_pQueue->Count() << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::HighLightedVolume
 *
 * Description 	: This function is called when the user clicks on the USB/JukeBox/IPOD tab
 *
 * Parameters 	: None
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::HighLightedVolume(int volume, QString serialNum, QString mountPath)
{
    Q_UNUSED(serialNum);
    Q_UNUSED(mountPath);
    HighlightedVolume = volume;
    LOG_TRACE << " CGracenoteInterface : After Setting HighLightedVolume from HMI = " << volume << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::USBDisConnected
 *
 * Description 	: This function is called when the USB is unplugged
 *
 * Parameters 	: QSting - serialNum of the USB unplugged
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::USBDisConnected(QString serialNum, int volume)
{
    LOG_TRACE << " CGracenoteInterface : USBDisConnected CID " << LOG_ENDL;
    int index = 0;

    //update the disconnect status to break the ongoing e_USBx population
    switch(volume)
    {
    case e_USB1:
        USB1ConnectStatus = 0;

        //Update the connect status of USB as 0 in DB based on serial num
//{modified by aettie 2013.03.22 for ITS 148383
        m_pMLDBCreator->UpdateConnectStatus(USB1ConnectStatus, volume, serialNum); 
        m_pTrackerQuery->UpdateUSBIndex(index, serialNum, volume);
        break;

    case e_USB2 :
        USB2ConnectStatus = 0;

        //Update the connect status of USB as 0 in DB based on serial num
        m_pMLDBCreator->UpdateConnectStatus(USB2ConnectStatus, volume, serialNum);
        m_pTrackerQuery->UpdateUSBIndex(index, serialNum, volume);
        break;
//}modified by aettie 2013.03.22 for ITS 148383
    default :
        break;
    }

    //Get the collection id for current volume
    gn_collection_id_t collectionId =  m_pMLDBCreator->RetrieveCollectionIDforVolume(volume, serialNum);

    LOG_TRACE << "CGracenoteInterface : USBDisConnected CID " << LOG_ENDL;
    LOG_TRACE << collectionId << LOG_ENDL;

    //Close Collection ID
    m_pMLDBCreator->CloseCollectionID(collectionId);
    m_pQueue->RemoveItem(volume);
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::IPODDisConnected
 *
 * Description 	: This function is called when the IPOD is unplugged
 *
 * Parameters 	: QString - serialNum of the IPOD unplugged
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::IPODDisConnected(QString serialNum, int volume)
{
    //GetCollectionID
    gn_collection_id_t collectionId =  m_pMLDBCreator->RetrieveCollectionIDforVolume(volume, serialNum);
    LOG_TRACE << "CGracenoteInterface : IPODDisConnected CID " << LOG_ENDL;
    LOG_TRACE << collectionId << LOG_ENDL;

    switch(volume)
    {
    case e_IPOD1 :
        IPOD1ConnectStatus = 0;
        m_pMLDBCreator->UpdateConnectStatus(IPOD1ConnectStatus, volume, serialNum); //modified by aettie 2013.03.22 for ITS 148383

        m_pQueue->RemoveItem(e_IPOD1);

        if(m_FrontIPODInfo.count() != 0)
            m_FrontIPODInfo.clear();

        m_FrontIPODSignalCount = 0;
        m_FrontIPODSignalIndex = 0;

        m_FrontDeletedList.clear();
        m_FrontModifiedList.clear();
        m_FrontNewList.clear();

        break;

    case e_IPOD2 :
        IPOD2ConnectStatus = 0;
        m_pMLDBCreator->UpdateConnectStatus(IPOD2ConnectStatus, volume, serialNum); //modified by aettie 2013.03.22 for ITS 148383

        m_pQueue->RemoveItem(e_IPOD2);
        if(m_RearIPODInfo.count() != 0)
            m_RearIPODInfo.clear();

        m_RearIPODSignalCount = 0;
        m_RearIPODSignalIndex = 0;

        m_RearDeletedList.clear();
        m_RearModifiedList.clear();
        m_RearNewList.clear();

        break;

    default :
        break;
    }

    //Close Collection ID
    m_pMLDBCreator->CloseCollectionID(collectionId);
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::GetMLDBCreator
 *
 * Description 	: Returns MLDBCreator object pointer
 *
 * Parameters  : None
 *
 * Returns 	: Pointer to CPlaylistMLDBCreator
 *
 *******************************************************************
 */
CPlaylistMLDBCreator* CGracenoteInterface :: GetMLDBCreator()
{
    return m_pMLDBCreator;
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::RetrieveMultipleCoverArts
 *
 * Description 	: API called over DBus for retrieving cover art
                          for various album\artists
 *
 * Parameters 	: albumList - album list
 *                artistList - artist list
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::RetrieveMultipleCoverArts(QStringList albumList, QStringList artistList)
{
    LOG_TRACE <<("CGracenoteInterface::RetrieveMultipleCoverArts Enter") << LOG_ENDL;

    // List of album and artist will have same number of entries. Even if artist is not present,
    // the list will contain null string. We will set just the album name to metadata and cover
    // art will be retrieved accordingly. This API is called from TrackerAbstractor. The count
    // difference handling is not required.
	//HighlightedVolume = e_CA;
    HighlightedVolume = e_MultiCA; //modified by aettie 2013.03.19 for Gracenote issue
    LOG_INFO << "CGracenoteInterface::RetrieveMultipleCoverArts - HighlightedVolume" << HighlightedVolume << LOG_ENDL;
    if(m_pGNCoverArt)
    {
        LOG_TRACE <<("CGracenoteInterface::RetrieveMultipleCoverArts Enqueue call");
        m_pGNCoverArt->SetAlbumArtistForCA(albumList, artistList);
		//m_pQueue->Enqueue(e_CA, CA);
        m_pQueue->Enqueue(e_MultiCA, CA);//modified by aettie 2013.03.19 for Gracenote issue
    }

    LOG_TRACE <<("CGracenoteInterface::RetrieveMultipleCoverArts Exit")  << LOG_ENDL;

}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::StoreIPODinfo
 *
 * Description 	: To retain the values of serial num and mnt path of IPOD

 * Parameters 	: serialNum(IN) - serial num of connected IPOD
      startIndex(IN) endIndex(IN) - NOT used
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::SetIPODinfo(QString serialNum,int startIndex, int endIndex, int syncState,  int volume)
{
    IPOD_Info ipodData;
    ipodData.iPODSerialNum = serialNum;
    ipodData.startIndex = startIndex;
    ipodData.endIndex = endIndex;
    ipodData.syncState = syncState;

    switch(volume)
    {
    case e_IPOD1 :
        m_FrontIPODInfo.append(ipodData);
        m_FrontIPODSignalCount++;
        break;

    case e_IPOD2 :
        m_RearIPODInfo.append(ipodData);
        m_RearIPODSignalCount++;
        break;

    default :
        break;
    }
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::GetIPODinfo
 *
 * Description 	: To gets the values of serial num and mnt path of IPOD

 * Parameters 	: serialNum(IN) - serial num of connected IPOD
      startIndex(IN) endIndex(IN) - NOT used
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::GetIPODinfo(QString &serialNum,int &startIndex, int &endIndex, int &syncState, int volume)
{
    switch(volume)
    {
    case e_IPOD1 :
        serialNum = m_FrontIPODInfo.at(m_FrontIPODSignalIndex).iPODSerialNum;
        startIndex = m_FrontIPODInfo.at(m_FrontIPODSignalIndex).startIndex;
        endIndex = m_FrontIPODInfo.at(m_FrontIPODSignalIndex).endIndex;
        syncState = m_FrontIPODInfo.at(m_FrontIPODSignalIndex).syncState;
        break;

    case e_IPOD2 :
        serialNum = m_RearIPODInfo.at(m_RearIPODSignalIndex).iPODSerialNum;
        startIndex = m_RearIPODInfo.at(m_RearIPODSignalIndex).startIndex;
        endIndex = m_RearIPODInfo.at(m_RearIPODSignalIndex).endIndex;
        syncState = m_RearIPODInfo.at(m_RearIPODSignalIndex).syncState;
        break;

    default :
        break;
    }
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::GetIPODSignalCount
 *
 * Description 	: Returns the total number of chunks of signals received from IPODController
 *
 * Returns 	: int : Total number of signals received in chunks
 *
 *******************************************************************
 */
int CGracenoteInterface::GetIPODSignalCount(int volume)
{
    int signalCount = 0;
    switch(volume)
    {
    case e_IPOD1 :
        signalCount =  m_FrontIPODSignalCount;
        break;

    case e_IPOD2 :
        signalCount = m_RearIPODSignalCount;
        break;

    default :
        break;
    }

    return signalCount;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::ClearSignalList
 *
 * Description 	: Clears the signal list when the device is unplugged
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::ClearSignalList(int volume)
{
    switch(volume)
    {
    case e_IPOD1 :
        m_FrontIPODInfo.clear();
        m_FrontIPODSignalCount = 0;
        m_FrontIPODSignalIndex = 0;
        break;

    case e_IPOD2 :
        m_RearIPODInfo.clear();
        m_RearIPODSignalCount = 0;
        m_RearIPODSignalIndex = 0;
        break;

    default :
        break;
    }
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::UpdateIPODListIndex
 *
 * Description 	: Updates the indexes processed
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::UpdateIPODListIndex(int volume)
{
    switch(volume)
    {
    case e_IPOD1 :
        m_FrontIPODSignalIndex++;
        break;

    case e_IPOD2 :
        m_RearIPODSignalIndex++;
        break;

    default :
        break;
    }
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::GetIPODListIndex
 *
 * Description 	: Returns the index to be processed
 *
 * Returns 	: None
 *
 *******************************************************************
 */

int CGracenoteInterface::GetIPODListIndex(int volume)
{
    int listIndex = 0;
    switch(volume)
    {
    case e_IPOD1 :
        listIndex = m_FrontIPODSignalIndex;
        break;

    case e_IPOD2 :
        listIndex = m_RearIPODSignalIndex;
        break;

    default :
        break;
    }
    return listIndex;
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::StartJukeboxPopulation
 *
 * Description 	: Starts the jukebox population
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::StartJukeboxPopulation()
{
    qDebug() << "********JUkebox Start********";

    m_pTrackerQuery->GetFilePathsInJukebox();

    qDebug() << "********JB End********";
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::StartUSBPopulation
 *
 * Description 	: Starts the USB population
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::StartUSBPopulation(EVolumeType type)
{
    qDebug() << "CGracenoteInterface: Start USB population";
    //Added by Puneet for fixing GPP crash issue 2013.06.12
    LOG_CRITICAL << "CGracenoteInterface::StartUSBPopulation: For Volume = " << type << LOG_ENDL;
    switch(type)
    {
    case e_USB1:
    {
        qDebug() << "********USB Start********";

        LOG_INFO << "CGracenoteInterface::StartUSBPopulation: Started USB1 "<< LOG_ENDL;
        QString serialNum = "";
        QString mountPath = "";
        //Gets the values of serial num and mnt path based on voulme and its connect status
        m_pTrackerQuery->GetUSBInfo(serialNum, mountPath, e_USB1);
        HighlightedVolume = e_USB1;

        LOG_INFO << "CGracenoteInterface::StartUSBPopulation e_USB1---" << serialNum << mountPath << LOG_ENDL;
        m_pTrackerQuery->GetFilePathsInUSB(serialNum, mountPath, e_USB1);
        LOG_INFO << "CGracenoteInterface::StartUSBPopulation : Finished USB1" << LOG_ENDL;

        qDebug() << "********USB End********";
        break;
    }
    case e_USB2:
    {
        LOG_INFO << "CGracenoteInterface::StartUSBPopulation : Started USB2 " << LOG_ENDL;
        QString serialNum = "";
        QString mountPath = "";
        m_pTrackerQuery->GetUSBInfo(serialNum, mountPath, e_USB2);
        HighlightedVolume = e_USB2;

        LOG_INFO << "CGracenoteInterface::StartUSBPopulation e_USB2---" << serialNum << mountPath << LOG_ENDL;
        m_pTrackerQuery->GetFilePathsInUSB(serialNum, mountPath, e_USB2);
        LOG_INFO << "CGracenoteInterface::StartUSBPopulation: Finished USB2"  << LOG_ENDL;
        break;
    }
    default:
        break;
    }

    qDebug() << "CGracenoteInterface: Start USB Exit";
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::SetQueueInterface
 *
 * Description 	: Set the instance of queue
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::SetQueueInterface(CGracenoteRequestQueue* interface)
{
    LOG_TRACE << "CGracenoteInterface::SetQueueInterface Enter" << LOG_ENDL;
    m_pQueue = interface;

    m_pTrackerQuery->SetQueueInterface(m_pQueue);

    LOG_TRACE << "CGracenoteInterface::SetQueueInterface Exit" << LOG_ENDL;
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::PerformCDLookupForTOC
 *
 * Description 	:Call the respective API to perform CD lookup
 *
 * Parameters  : toc - TOC string
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::PerformCDLookupForTOC()
{
    if(m_pGNCDLookup)
    {
        m_pGNCDLookup->PerformCDLookup();
    }
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::RetrieveMultipleCA
 *
 * Description 	:Call the respective API to Retrieve Multiple CA's
 *
 * Parameters  : None
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::RetrieveMultipleCA()
{
    if(m_pGNCoverArt)
    {
        m_IsMultipleCoverRequest = true;
        m_CoverRequestCount = m_pGNCoverArt->GetAlbumListCount();
        m_CoverArtList.clear();

        LOG_TRACE <<("CGracenoteInterface::RetrieveMultipleCoverArts Start") << LOG_ENDL;
        m_pGNCoverArt->RetrieveMultipleCoverArts();
        LOG_TRACE <<("CGracenoteInterface::RetrieveMultipleCoverArts End") << LOG_ENDL;
    }
}
/*
 *******************************************************************
 * Function	: CGracenoteInterface::RetrieveCA
 *
 * Description 	:Call the respective API to Retrieve single CA
 *
 * Parameters  : None
 *
 * Returns 	: None
 *
 *******************************************************************
 */
 //added by aettie 2013.03.19 for Gracenote issue
void CGracenoteInterface::RetrieveCA()
{
    if(m_pGNCoverArt)
    {
        m_IsMultipleCoverRequest = false;

        LOG_TRACE <<("CGracenoteInterface::RetrieveCA Start") << LOG_ENDL;
        m_pGNCoverArt->RetrieveCoverArtForMP(m_pGNCoverArt->GetMetadataForCA());
        LOG_TRACE <<("CGracenoteInterface::RetrieveCA End") << LOG_ENDL;
    }
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::PerformResyncForIPOD
 *
 * Description 	: Perform Resync operation for IPOD, add the modified
 *                       updated tracks and remove deleted tracker
 *
 * Parameters  : serialNum - Serial number
 *                       volume - IPOD front of rear
 *
 * Returns 	: EipodExecutionStatus - Current execution status
 *
 *******************************************************************
 */
EipodExecutionStatus CGracenoteInterface::PerformResyncForIPOD(QString serialNum, int volume)
{
    LOG_TRACE <<"CGracenoteInterface: PerformResyncForIPOD START" << LOG_ENDL;
    LOG_TRACE <<"IPOD Serial number : "<< serialNum << LOG_ENDL;
    LOG_TRACE << "Volume == :: "<< volume;

    int i = 0;
    int ConnectStatus = 0;
    EipodExecutionStatus returnStatus = e_Finished;

    GNVList newDataList;

    if(volume == e_IPOD1)
    {
        m_FrontNewList.clear();
        m_FrontModifiedList.clear();
        m_FrontDeletedList.clear();

        m_pGNOperationUtil->GetResyncData(serialNum,
                                          m_FrontNewList,
                                          m_FrontModifiedList,
                                          m_FrontDeletedList);

        newDataList = m_FrontNewList;
        m_FrontNewList.clear();
    }
    else if(volume == e_IPOD2)
    {
        m_RearNewList.clear();
        m_RearModifiedList.clear();
        m_RearDeletedList.clear();

        m_pGNOperationUtil->GetResyncData(serialNum,
                                          m_RearNewList,
                                          m_RearModifiedList,
                                          m_RearDeletedList);

        newDataList = m_RearNewList;
        m_RearNewList.clear();
    }

    // Check if IPOD is still connected
    // If IPOD is disconnected, while calculating Resync information, return
   /* if((IPOD1ConnectStatus == 0 && volume == e_IPOD1) ||
            (IPOD2ConnectStatus == 0 && volume == e_IPOD2))
    {
        LOG_TRACE << "PerformResyncForIPOD: IPOD is disconnected" << LOG_ENDL;
        return e_Unplugged;
    }*/

    gn_collection_id_t nCID = GetCollectionId(volume, serialNum);
    GetNormalizedDataObject()->CreateSQLiteDB(nCID); //modified by Divya 2012.01.07 for gracenote normalized db

    if(volume == e_IPOD1)
    {
        m_pMLDBCreator->UpdateConnectStatus(IPOD1ConnectStatus, volume, serialNum); //modified by aettie 2013.03.22 for ITS 148383

        //Delete Entries from MLDB
        if(m_FrontDeletedList.count() > 0)
            returnStatus = DeleteOrUpdateEntriesFromIPodMLDB(volume, nCID, m_FrontDeletedList, true);

        if(returnStatus != e_Finished)
            return returnStatus;

        //Update Entries in MLDB
        if(m_FrontModifiedList.count() > 0)
            returnStatus = DeleteOrUpdateEntriesFromIPodMLDB(volume, nCID, m_FrontModifiedList, false);

        if(returnStatus != e_Finished)
            return returnStatus;
    }
    else if(volume == e_IPOD2)
    {
        m_pMLDBCreator->UpdateConnectStatus(IPOD2ConnectStatus, volume, serialNum); //modified by aettie 2013.03.22 for ITS 148383

        //Delete Entries from MLDB
        if(m_RearDeletedList.count() > 0)
            returnStatus = DeleteOrUpdateEntriesFromIPodMLDB(volume, nCID, m_RearDeletedList, true);

        if(returnStatus != e_Finished)
            return returnStatus;

        //Update Entries in MLDB
        if(m_RearModifiedList.count() > 0)
            returnStatus = DeleteOrUpdateEntriesFromIPodMLDB(volume, nCID, m_RearModifiedList, false);

        if(returnStatus != e_Finished)
            return returnStatus;
    }

    LOG_TRACE <<("Count of IPOD data") << LOG_ENDL;
    LOG_INFO <<(newDataList.count()) << LOG_ENDL;

    QStringList ipodList;

    for(i = 0; i < newDataList.count(); i++)
    {
        ipodList = newDataList.at(i);

        if((IPOD1ConnectStatus == 0 && volume == e_IPOD1)
                || (IPOD2ConnectStatus == 0 && volume == e_IPOD2))
        {
            ConnectStatus = m_pMLDBCreator->GetConnectStatus(serialNum);
            if(ConnectStatus == 0)
            {
                newDataList.clear();
                returnStatus = e_Unplugged;
                break;
            }
        }
        else if(HighlightedVolume != volume)
        {
            LOG_INFO << "STOP Resync and enqueue IPOD back" << LOG_ENDL;
            /* preemption occured enqueue back to Resync MLDB for rest of the files */
            if(volume == e_IPOD1)
            {
                m_pQueue->Enqueue(e_IPOD1, IPOD);
            }
            else if(volume == e_IPOD2)
            {
                m_pQueue->Enqueue(e_IPOD2, IPOD);
            }
            returnStatus = e_InProgress;
            break;
        }

        UpdateMLDBWithIPODData(ipodList, nCID);
    }

    // If no preemption occured, clear the list
    if(i == newDataList.count())
    {
        newDataList.clear();
        //Update the MLDB PopulationState
        m_pMLDBCreator->UpdateMLDBState(1, serialNum);
    }
    m_pMLDBCreator->CloseCollectionID(nCID);

    //Remove the OLD Table from the IPOD DB
    m_pGNOperationUtil->DeleteOldTableFromIPODDB(serialNum);

    // In case of resync, Indexing finish signal also should be emitted

    emit GracenoteIndexingStatus(1, volume);

    //{added by esjang 2012.11.14 for XSampa DB update with Volume Info
    //there is no update process for resynced xsampa!
    //XSampaDataUpdatedInDB(volume); //deleted by aettie Xsampa DB is not used.
    //}added by esjang 2012.11.14 for XSampa DB update with Volume Info


    LOG_TRACE << "CGracenoteInterface : PerformResyncForIPOD End" << LOG_ENDL;

    return returnStatus;
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::DeleteOrUpdateEntriesFromIPodMLDB
 *
 * Description 	: Call the API's to delete\update entry in MLDB
 *
 * Parameters  : volume - Indicating iPod Front or Rear
 *               collectionId - Collection id
 *               dataList - list containing information about entries to be deleted
 *               isDelete - true: delete, else update
 *
 * Returns 	: EipodExecutionStatus - Current execution status
 *
 *******************************************************************
 */
EipodExecutionStatus CGracenoteInterface::DeleteOrUpdateEntriesFromIPodMLDB(int volume, int collectionId,
                                                                            QVector<QStringList>& dataList,
                                                                            bool isDelete)
{

    LOG_TRACE << "CGracenoteInterface : DeleteOrUpdateEntriesFromIPodMLDB Enter " << LOG_ENDL;

    QString fileName, filePath;
    QString utf8FileName;

    EipodExecutionStatus returnStatus = e_Finished;

    while (dataList.count() > 0)
    {

        // If the IPOD is disconnected, exit from the operation
        if((IPOD1ConnectStatus == 0 && volume == e_IPOD1)
                || (IPOD2ConnectStatus == 0 && volume == e_IPOD2))
        {

            LOG_TRACE << "CDeleteOrUpdateEntriesFromIPodMLDB IPOD Disconnected " << LOG_ENDL;
            returnStatus = e_Unplugged;
            break;
        }
        else if(HighlightedVolume != volume)
        {
            LOG_INFO << "Stop Resync and enqueue IPOD back" << LOG_ENDL;

            // Highlighted volume is changed, Enqueue the Resync operation
            if(volume == e_IPOD1)
            {
                m_pQueue->Enqueue(e_IPOD1, IPOD);
            }
            else if(volume == e_IPOD2)
            {
                m_pQueue->Enqueue(e_IPOD2, IPOD);
            }
            returnStatus = e_InProgress;
            break;
        }

        //Take data at first position
        fileName = dataList.at(0).at(0);
        filePath = dataList.at(0).at(1);

        gn_uchar_t* temp = gnepal_strdup( (gn_uchar_t *)  (fileName.toUtf8().constData()) );

        utf8FileName = m_pGNOperationUtil->encodeString(temp);

        if(temp != NULL) gnepal_mem_free(temp); temp = NULL;

        if(isDelete)
        {
            m_pMLDBCreator->DeleteIPODEntryFromMLDB(collectionId, utf8FileName, filePath);
        }
        else
        {
            m_pMLDBCreator->UpdateIPODEntryFromMLDB(collectionId, utf8FileName, filePath);
        }

        //Remove the entry from list, which is deleted\updated from MLDB
        dataList.remove(0);
    }

    LOG_TRACE << "CGracenoteInterface : DeleteOrUpdateEntriesFromIPodMLDB Exit " << LOG_ENDL;

    return returnStatus;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::ResyncAfterCopyOperation
 *
 * Description 	: Call the API's for updating entry in MLDB after copy/move
 * operation
 *
 * Parameters  : destinationVolume - The volume to which the files have been
 *                      moved to or copied.
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteInterface::ResyncAfterCopyOperation(int destinationVolume)
{
   LOG_TRACE << "CGracenoteInterface : ResyncAfterCopyOperation " << LOG_ENDL;


    if(destinationVolume == e_JukeBox)
    {
        m_pQueue->Enqueue(e_JukeBox, JUKE);
        LOG_TRACE << "CGracenoteInterface : ResyncAfterCopyOperation  m_pQueue->Enqueue(e_JukeBox, JUKE) called" << LOG_ENDL;
    }
    else if(destinationVolume == e_USB1)
    {
        m_pQueue->Enqueue(e_USB1, USB);
    }
    else if(destinationVolume == e_USB2)
    {
        m_pQueue->Enqueue(e_USB2, USB);
    }
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::BackupXsampaToFS
 *
 * Description 	: Take backup of XSampa database to File system
 *
 *******************************************************************
 *
 //deleted by aettie Xsampa DB is not used.
void CGracenoteInterface::BackupXsampaToFS()
{
    LOG_TRACE << " CApplicationInterface::BackupXsampaToFS() Exit" << LOG_ENDL;

    QString xsampaFSName = "/app/data/gracenote/XSampaDB"; //modified by aettie.ji 2013.05.01 for Gracenote DB location change
    QString xsampaTmpfsName = "/dev/shm/XSampaDB";

    if(QFile::exists(xsampaTmpfsName))
    {
        if(QFile::exists(xsampaFSName))
        {
            LOG_TRACE << "CGracenoteInterface :: Removing FS file " << LOG_ENDL;
            QFile::remove(xsampaFSName);
        }

        LOG_TRACE << "CGracenoteInterface :: copying TMPFS file to FS " << LOG_ENDL;
        QFile::copy(xsampaTmpfsName, xsampaFSName);
    }

    // Make sure that file copy is complete
    sync();

    LOG_TRACE << " CGracenoteInterface::BackupXsampaToFS() Exit" << LOG_ENDL;
}*/
//{added by Divya 2012.01.07 for gracenote normalized db
/*
 *******************************************************************
 * Function	: CGracenoteInterface::GetNormalizedDataObject
 *
 * Description 	: Returns the pointer to CNormalizedData object
 * Returns 	: CNormalizedData* Returns the pointer to CNormalizedData object
 *
 *******************************************************************
 */
CNormalizedData* CGracenoteInterface::GetNormalizedDataObject()
{
    return m_NormalizedData;
}
//{added by aettie.ji 20130904 for gracenote version
/*
 *******************************************************************
 * Function	: CGracenoteInterface::GetSDKVer
 *
 * Description 	: 
 * Returns 	: 
 *
 *******************************************************************
 */
void CGracenoteInterface::GetSDKVer()
{
    const gn_uchar_t* emmsVer = gn_get_emms_version();
    QString emmsVerStr = QString::fromUtf8(reinterpret_cast<const char*>(emmsVer)); 
    LOG_TRACE << "aettiever GetSDKVer emmsVerStr : "<<emmsVerStr << LOG_ENDL;
    emit GNSDKVer(emmsVerStr);

}
/*
 *******************************************************************
 * Function	: CGracenoteInterface::GetDBVer
 *
 * Description 	: 
 * Returns 	: 
 *
 *******************************************************************
 */
void CGracenoteInterface::GetDBVer()
{
    gn_sys_install_profile_t* installed_profile = NULL;    
    gn_error_t nError = GN_FAILURE; 
    gn_db_rev_t dbRev = 0;
    nError = gn_sys_get_install_profile (&installed_profile);
    dbRev = installed_profile->base_db_revision;
    LOG_TRACE << "aettiever GetDBVer dbRev : "<<dbRev << LOG_ENDL;
    emit GNDBVer(dbRev);
}
//}added by aettie.ji 20130904 for gracenote version


/*******************************************************************
 * Function	: CGracenoteInterface::GetDBRegionVer
 *
 * Description 	: 
 * Returns 	: 
 *
 *******************************************************************
 */
 //20131023 System info
void CGracenoteInterface::GetDBRegionVer()
{
    gn_sys_install_profile_t* installed_profile = NULL;    
    gn_error_t nError = GN_FAILURE; 
    gn_db_rev_t dbRev = 0;
    
    nError = gn_sys_get_install_profile (&installed_profile);
    dbRev = installed_profile->base_db_revision;
    
    gn_uchar_t* hierarchy_id = NULL;    

    nError = gn_list_get_genre_hierarchy_id (&hierarchy_id);
    
    LOG_TRACE << "aettiever GetDBRegionVer dbRev : "<<dbRev << LOG_ENDL;

    QString dbRevName = QString::fromUtf8(reinterpret_cast<const char*>(hierarchy_id));
    
    if( "gls" == dbRevName ) dbRevName = "GLOBAL";
    else if( "uss" == dbRevName ) dbRevName = "NA";
    else if( "eus" == dbRevName ) dbRevName = "EU";
    else if( "chs" == dbRevName ) dbRevName = "CN";
    else if( "kos" == dbRevName ) dbRevName = "KOR";
    else dbRevName = "undefined";
    
    dbRevName.append(QString("[ %1 ]").arg(dbRev));

    LOG_TRACE << "aettiever GetDBRegionVer dbRevName : "<<dbRevName << LOG_ENDL;
    
    emit GNDBRegionVer(dbRevName);
}
//End of file
