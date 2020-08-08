/*
 ******************************************************************
 *    COPYRIGHT ?Teleca AB      *
 *----------------------------------------------------------------*
 * MODULE     	:  CPlaylistMLDBCreator                             	  *
 *
 * PROGRAMMERS 	:  Divya Dsouza / Vishal Jain
 * DATE       	:  29 September 2011                             	  *
 * VERSION    	:                                             	  *
 *
 *----------------------------------------------------------------*
 *                                                                *
 *  MODULE SUMMARY : CPlaylistMLDBCreator supports feature for
       registering a collection with MLDB and doing
       actual population to MLDB
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                     *
 *
 *     DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * History
 * 01-Sep-2011      Divya Dsouza


 ******************************************************************
 */

#include "DHAVN_GracenotePlaylistMLDBCreator.h"

#include <gn_playlist_types.h>
#include <gn_playlist.h>
#include <gnepal_string.h>
#include <gn_abs_errors.h>
#include <gn_error_codes.h>
#include <gn_error_values.h>
#include <gn_error_display.h>
#include <gnepal_memory.h>

#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QFileInfo>
#include <QDebug>
#include <QStringList>
#include <QDateTime>
#include <DHAVN_GracenoteInterface.h>
#include <QSqlError>
#include "DHAVN_GracenoteFileHandling.h"
#include <gn_fs.h>
#include <DHAVN_GracenoteDBconnection.h>
//#include <stdio.h> Commented to disable MLDB on RAM changes


//Number for MLDBs to be maintained in the system for USB
extern int USB_MLDB_count;
//Number for MLDBs to be maintained in the system for IPOD
extern int IPOD_MLDB_count;

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::CPlaylistMLDBCreator
 *
 * Description 	: Constructor for CPlaylistMLDBCreator

 * Parameters 	: parent (IN) -

 * Returns 	: None
 *
 *******************************************************************
 */
CPlaylistMLDBCreator :: CPlaylistMLDBCreator(QObject *parent)
{
    Q_UNUSED(parent);

    m_isDbValid = false;
    m_NormalizedData = NULL; //added by aettie 2013.02.28 for static analysis 18441
    //m_pGracenoteInterface = NULL;  //added by aettie 20130518 ISV 83208
}

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::~CPlaylistMLDBCreator
 *
 * Description 	: Destructor for CPlaylistMLDBCreator

 * Parameters 	: parent (IN) -

 * Returns 	: None
 *
 *******************************************************************
 */
CPlaylistMLDBCreator :: ~CPlaylistMLDBCreator()
{
    if(m_NormalizedData)
        delete m_NormalizedData; //added by Divya 2013.01.17 for gracenote normalized db
//added by aettie 20130518 ISV 83208		
    //if(m_pGracenoteInterface)
    //{
    //    delete m_pGracenoteInterface;
    //    m_pGracenoteInterface = NULL;
    //}
}

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::Init
 *
 * Description 	: Init for CPlaylistMLDBCreator

 * Parameters 	: parent (IN) -

 * Returns 	: None
 *
 *******************************************************************
 */
bool CPlaylistMLDBCreator::Init()
{
    LOG_INIT_V2("Media_GracenotePluginProcess");
	m_NormalizedData = new CNormalizedData(); //added by Divya 2013.01.17 for gracenote normalized db

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("CID");

    bool result = createConnection(fsDBPath, connectionName, db);

    if(result)
    {
        LOG_TRACE <<("CPlaylistMLDBCreator: Init: DB is open") << LOG_ENDL;
        QSqlQuery* query = new QSqlQuery(db);

        m_isDbValid = query->exec("CREATE TABLE CollectionID_Data"
                                          "(CID int primary key, "
                                          "Volume integer, "
                                          "SerialNum varchar(256), "
                                          "AccessDate varchar(256), "
                                          "Connect integer,"
                                          "MountPath varchar(256), "
                                          "PopulationState integer,"
                                          "Idx integer) " );

        if(m_isDbValid)
        {
            LOG_TRACE <<("PlaylistMLDBCreator: CID_Volume created") << LOG_ENDL;
        }
        else
        {
            LOG_TRACE <<("CPlaylistMLDBCreator: Init: CID_Volume create table query failed !!!") << LOG_ENDL;
        }
        delete query;
        closeConnection(db);  //added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close();  //Removed  by Puneet for fixing GPP crash issue 2013.06.12
    }

    //gnfs_enable_mlt(true);  //deleted by aettie 20131010 unused code
    return m_isDbValid;
}
/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::SetGracenoteInterface
 *
 * Description 	: Set the instance of CGracenoteInterface here

 * Parameters 	: aInterface (IN) - Pointer to CGracenoteInterface

 * Returns 	: None
 *
 *******************************************************************
 */
 //added by aettie 20130518 ISV 83208
//void CPlaylistMLDBCreator::SetGracenoteInterface(CGracenoteInterface* aInterface)
//{
    // Gracenote interface
//    m_pGracenoteInterface = aInterface;

//}
/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::RegisterCollectionIDForVolumeObject
 *
 * Description 	: Register a collection in MLDB for a specified volume

 * Parameters 	: nvolume (IN) - Volume information

 * Returns 	: Collection ID value
 *
 *******************************************************************
 */
unsigned long CPlaylistMLDBCreator :: RegisterCollectionIDForVolumeObject(int nvolume, QString serialNum)
{
    gn_error_t nError = 0;
    unsigned long nCID = GN_INVALID_COLLECTION_ID;
    bool query_status = false;

    nError = gnplaylist_create_collection_id(&nCID);
    if(nError != GN_SUCCESS)
    {
        return 0;
    }
    else
    {
        QSqlDatabase db;
        QString fsDBPath;
        fsDBPath.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

        QString connectionName;
        connectionName.append("CID");

        bool result = createConnection(fsDBPath, connectionName, db);
        if(result)
         {
                //m_pGracenoteInterface->LockGracenoteMutex(); //added by aettie 20130518 ISV 83208
             LOG_TRACE <<("CPlaylistMLDBCreator: RegisterCollectionIDForVolumeObject dbopen") << LOG_ENDL;
             QSqlQuery* query = new QSqlQuery(db);
             query->prepare("INSERT INTO CollectionID_Data(CID, Volume, SerialNum, AccessDate, Connect, MountPath, PopulationState, Idx) "
                                "VALUES (?, ?, ?, ?, ?, ?, ?, 0)");

              QVariant column_val;
              column_val.setValue(nCID);
              query->addBindValue(column_val);

              query->addBindValue(nvolume);

              QString SNum;
              SNum.clear();

              if(nvolume== e_JukeBox)
              {
                  SNum.append("");
                  query->addBindValue(SNum);
              }
              else
              {
                  column_val = serialNum;
                  query->addBindValue(column_val);
              }

              QString mountPath;
              mountPath.clear();

              QDateTime date = QDateTime::currentDateTime();
              QString dateString = date.toString();
              query->addBindValue(dateString);

              int connectStatus = 1;
              query->addBindValue(connectStatus);

              mountPath.append("");
              query->addBindValue(mountPath);

              int state = 0;
              query->addBindValue(state);

              query_status = query->exec();
              if(!query_status)
                  LOG_TRACE <<("CPlaylistMLDBCreator: RegisterCollectionIDForVolumeObject query execution failed !!!") << LOG_ENDL;

              delete query;
              closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12 
//            db.close();   //Removed by Puneet for fixing GPP crash issue 2013.06.12
              //m_pGracenoteInterface->UnlockGracenoteMutex(); //added by aettie 20130518 ISV 83208
         }
    }//END if(nError != GN_SUCCESS)

       return nCID;
}

/*
 *******************************************************************
 * Function		: CPlaylistMLDBCreator::PopulateCollectionID
 *
 * Description 	: Populate a collection in MLDB with media information

 * Parameters 	: CID (IN) - Collection ID
       entryData(IN) - Entry data structure to be populated
       in MLDB
       filePath(IN) - path of the media file

 * Returns 		: gn_error_t error, if any
 *
 *******************************************************************
 */
gn_error_t  CPlaylistMLDBCreator :: PopulateCollectionID(gn_collection_id_t CID,
                                                         gnplaylist_entrydata_t entryData,
                                                         QString filePath)
{
    //Open Collection ID
    gn_error_t nError = GN_FAILURE;
    gn_bool_t isOpened;

    nError = gnplaylist_is_collection_id_open(CID, &isOpened);

    LOG_TRACE <<("CPlaylistMLDBCreator : PopulateCollectionID collection_id") << LOG_ENDL;
    LOG_TRACE << CID << LOG_ENDL;

    if(nError == GN_SUCCESS && isOpened == false)
    {
        nError =  OpenCollectionID(CID);
        LOG_TRACE <<("CPlaylistMLDBCreator Open collection id error") << LOG_ENDL;
        LogError(nError);
    }
    else if(nError != GN_SUCCESS)
    {
        return nError;
    }

    gn_uchar_t* gfileName = NULL;
    gn_uchar_t* gfilePath = NULL;

    if(filePath.contains("file:///ipod"))
    {
        gn_uchar_t temp[200];
        gnplaylist_getentryfieldval(entryData, gnpl_crit_field_file_name, temp, sizeof(temp)/sizeof(gn_uchar_t));

        gfileName = gnepal_strdup(temp);

        gfilePath = gnepal_strdup( (gn_uchar_t *)  (filePath.toUtf8().constData()) );

    }
    else
    {
        QFileInfo fileParser(filePath);

        gfileName = gnepal_strdup( (gn_uchar_t *)  (fileParser.fileName().toUtf8().constData()) );
        gfilePath = gnepal_strdup( (gn_uchar_t *)  (fileParser.filePath().toUtf8().constData()) );

    }

    LOG_TRACE <<("CPlaylistMLDBCreator : PopulateCollectionID") << LOG_ENDL;
	//Removed Puneet for fixing GPP crash issue 2013.06.12
//    LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(gfileName))) << LOG_ENDL;
//    LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(gfilePath))) << LOG_ENDL;

    if(gfileName != GN_NULL)
    {
        //check if the entry already exists
        gnplaylist_entrydata_t	metadata = GN_NULL;
        nError =  gnplaylist_getentry (CID, gfileName, gfilePath, NULL, &metadata);

        LOG_TRACE <<("CPlaylistMLDBCreator: gnplaylist_getentry");
        LogError(nError);

        if( nError == DBERR_NotFound )
        {
            nError = gnplaylist_addentry(CID, gfileName, gfilePath, entryData);
            LOG_TRACE <<("CPlaylistMLDBCreator: gnplaylist_addentry error")<< LOG_ENDL;
            LogError(nError);
        }

        // Free the data structure
        gnplaylist_freeentry(&metadata);
        if(gfileName != NULL) gnepal_mem_free(gfileName); gfileName = NULL;
        if(gfilePath != NULL) gnepal_mem_free(gfilePath); gfilePath = NULL;
    }
    return nError;
}

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::DeleteMusicFileEntryInCollectionID
 *
 * Description 	: Delete a media entry from MLDB

 * Parameters 	: CID (IN) - Collection ID
       filePath(IN) - path of the media file

 * Returns 	: gn_error_t error, if any
 *
 *******************************************************************
 */
int CPlaylistMLDBCreator :: DeleteMusicFileEntryInCollectionID(int CID, QString filePath)
{
    //Open Collection ID
    gn_error_t nError = GN_FAILURE;
    nError =  gnplaylist_open_collection_id(CID);

    QString qfileName = filePath.section('/', -1, -1);
    gn_uchar_t* gfileName = gnepal_strdup( (gn_uchar_t *)  (qfileName.toUtf8().constData()) );
    gn_uchar_t* gfilePath = gnepal_strdup( (gn_uchar_t *)  (filePath.toUtf8().constData()) );

    LOG_TRACE <<("CPlaylistMLDBCreator: DeleteMusicFileEntryInCollectionID");
    LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(gfileName)));
    LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(gfilePath)));

    if(gfileName != GN_NULL)
    {
        gnplaylist_entrydata_t	metadata = GN_NULL;
        //check if the entry already exists
        nError =  gnplaylist_getentry (CID, gfileName, gfilePath, NULL, &metadata );
        LOG_TRACE <<("CPlaylistMLDBCreator: gnplaylist_getentry error") << LOG_ENDL;
        LogError(nError);

        if( nError == GN_SUCCESS  && metadata != GN_NULL )
        {
            nError = gnplaylist_deleteentry(CID, gfileName, gfilePath);
            LOG_TRACE <<("CPlaylistMLDBCreator: gnplaylist_deleteentry error") << LOG_ENDL;
            LogError(nError);
        }
        // Free the data structure
        gnplaylist_freeentry(&metadata);
    }

    if(gfileName != NULL) gnepal_mem_free(gfileName); gfileName = NULL;
    if(gfilePath != NULL) gnepal_mem_free(gfilePath); gfilePath = NULL;
    return nError;
}

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::CloseCollectionID
 *
 * Description 	: Close a collection id in MLDB

 * Parameters 	: CID (IN) - Collection ID

 * Returns 	: gn_error_t error, if any
 *
 *******************************************************************
 */
gn_error_t CPlaylistMLDBCreator :: CloseCollectionID(int CID)
{

    LOG_TRACE << "Close CollectionID : : " << CID << LOG_ENDL;
    gn_error_t err = gnplaylist_close_collection_id(CID);
    BackupMLDBOnFS(CID);
    return err;
}

/*
 *******************************************************************
 * Function		: CPlaylistMLDBCreator::OpenCollectionID
 *
 * Description 	: Open a collection id in MLDB

 * Parameters 	: CID (IN) - Collection ID

 * Returns 		: gn_error_t error, if any
 *
 *******************************************************************
 */
gn_error_t CPlaylistMLDBCreator :: OpenCollectionID(int CID)
{
    return gnplaylist_open_collection_id(CID);
}

/*
 *******************************************************************
 * Function		: CPlaylistMLDBCreator::RetrieveCollectionIDforVolume
 *
 * Description 	: Retrieve a collection ID associated with the volume

 * Parameters 	: nvolume (IN) - Volume information

 * Returns 		: Collection id retrieved
 *
 *******************************************************************
 */
unsigned long CPlaylistMLDBCreator :: RetrieveCollectionIDforVolume(int nvolume, QString serialNum)
{
    LOG_TRACE <<("CPlaylistMLDBCreator :RetrieveCollectionIDforVolume start");

    gn_error_t nError = 0;
    bool query_status = true;
    int nstored_CID = 0;

    //get list of collection IDs
    gn_collection_id_list_t *list;
    nError = gnplaylist_get_collection_id_list(&list);

    if(list->id_count == 0 || nError != GN_SUCCESS)
    {
        //Free the collection id list
        gnplaylist_free_collection_id_list(&list);
        LOG_TRACE <<("CPlaylistMLDBCreator :RetrieveCollectionIDforVolume list->id_count == 0");
        return 0;
    }
    else if(list->id_count > 0)
    {
        QSqlDatabase db;
        QString fsDBPath;
        fsDBPath.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

        QString connectionName;
        connectionName.append("CID");

        bool result = createConnection(fsDBPath, connectionName, db);
        if(result)
         {
            //m_pGracenoteInterface->LockGracenoteMutex(); //added by aettie 20130518 ISV 83208
            QSqlQuery* query = new QSqlQuery(db);

            if(nvolume == e_JukeBox || nvolume == e_Disc)
            {
                query->prepare("SELECT CID FROM CollectionID_Data WHERE Volume = ?");
                query->addBindValue(nvolume);
            }
            else if (nvolume == e_USB1 || nvolume == e_USB2)  //modified by aettie 2013.03.19 for Gracenote issue
//            else
            {
			//{modified by aettie 2013.03.22 for ITS 148383
                query->prepare("SELECT CID FROM CollectionID_Data WHERE SerialNum = ? AND Volume=?");
                //query->prepare("SELECT CID FROM CollectionID_Data WHERE SerialNum = ?");
                query->addBindValue(serialNum);
                query->addBindValue(nvolume);
			//}modified by aettie 2013.03.22 for ITS 148383
            }
			//added by aettie 20130518 ISV 83208
	      //else
           // {
           //     m_pGracenoteInterface->UnlockGracenoteMutex();
                //return 0; 
	    //  }
			else return 0; //added by aettie 2013.03.19 for Gracenote issue
            query_status = query->exec();
            if(query_status)
            {
                LOG_TRACE << "CPlaylistMLDBCreator :RetrieveCollectionIDforVolume select query passed Volume = " << nvolume << LOG_ENDL ;

                while (query->next())
                {
                    nstored_CID = query->value(0).toInt();

                    //Get all the MLDBs & the corresponding dates for a particular volume
                    QSqlQuery* updateQuery = new QSqlQuery(db);

                    //Update access date for the retrieved CID
					//modified by aettie.ji 2013.02.22 for ISV 71131 etc.
                    //updateQuery->prepare("UPDATE CollectionID_Data SET AccessDate = ?, Volume = ? WHERE CID = ?");
                    updateQuery->prepare("UPDATE CollectionID_Data SET AccessDate = ? WHERE CID = ?");

                    QDateTime date = QDateTime::currentDateTime();
                    QString dateString = date.toString();

                    updateQuery->addBindValue(dateString);
                    //updateQuery->addBindValue(nvolume); //deleted by aettie.ji 2013.02.22 for ISV 71131 etc.
                    updateQuery->addBindValue(nstored_CID);

                    query_status = updateQuery->exec();

                    if(query_status)
                    {
                        LOG_TRACE <<("CPlaylistMLDBCreator :Access Date updated in DB");
                    }
                    else
                        LOG_TRACE <<("CPlaylistMLDBCreator :Access Date update in DB failed !!!");

                    delete updateQuery;
                    break;
                } //END while (query.next())
            }
            else
            {
                LOG_TRACE <<("CPlaylistMLDBCreator :RetrieveCollectionIDforVolume select query failed !!!");
            }

            delete query;
            closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close();
            //m_pGracenoteInterface->UnlockGracenoteMutex(); //added by aettie 20130518 ISV 83208
        }

        //Free the collection id list
        gnplaylist_free_collection_id_list(&list);

        LOG_TRACE <<("GracenotePluginProcess :CPlaylistMLDBCreator: RetrieveCollectionIDforVolume nstored_CID");
        LOG_TRACE <<(nstored_CID);
        return nstored_CID;
    }//END if(list->id_count == 0)
    return nstored_CID;
}

/*
 *******************************************************************
 * Function		: CPlaylistMLDBCreator::EnableForPlaylisting
 *
 * Description 	: Enable CID for Playlisting.

 * Parameters 	: nvolume - volume like usb, jukebox

 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CPlaylistMLDBCreator::EnableForPlaylisting(int nvolume, QString serialNum, QString mountPath)
{
    LOG_TRACE <<("CPlaylistMLDBCreator :EnableForPlaylisting START") << LOG_ENDL;
    LOG_TRACE << "CPlaylistMLDBCreator :EnableForPlaylisting" << nvolume << LOG_ENDL;
    LOG_TRACE << "CPlaylistMLDBCreator :EnableForPlaylisting" << serialNum << LOG_ENDL;
    Q_UNUSED(mountPath);

    //Get the list of collection IDs for the volume for which the devices are connected
    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("CID");

    bool result = createConnection(fsDBPath, connectionName, db);
    if(result)
     {
        //m_pGracenoteInterface->LockGracenoteMutex(); //added by aettie 20130518 ISV 83208
        QSqlQuery* query =  new QSqlQuery(db);
        bool query_status = false;

        switch (nvolume)
        {
        case e_JukeBox :
        case e_Disc :
            {
                query->prepare("SELECT CID FROM CollectionID_Data WHERE Volume = ?");
                query->addBindValue(nvolume);
                query_status = query->exec();
            }
            break;

        case e_USB1 :
        case e_USB2 :
//        case e_IPOD1 : //deleted by aettie 2013.03.19 for Gracenote issue
//        case e_IPOD2 :
            {
                int connectStatus = 1;
                query->prepare("SELECT CID FROM CollectionID_Data WHERE SerialNum = ? AND Connect = ?");
                query->addBindValue(serialNum);
                query->addBindValue(connectStatus);
                query_status = query->exec();
            }
            break;

        default :
                break;
    }

    if(query_status)
        {
        LOG_TRACE <<("CPlaylistMLDBCreator :EnableForPlaylisting query passed") << LOG_ENDL;
        while (query->next())
        {
            int nstored_CID = query->value(0).toInt();
            LOG_TRACE <<("CPlaylistMLDBCreator :EnableForPlaylisting CID to open =");
            LOG_TRACE << nstored_CID << LOG_ENDL;

            // Check if collection id is already open
            // If its open, indicates that MLDB population for specific volume is ongoing
            // if its closed, we can enable for MLT
            //gn_bool_t isOpen = false;
            //gnplaylist_is_collection_id_open(nstored_CID, &isOpen);

            //Enable MLT
            //if(!isOpen)
            //{
            LOG_TRACE << "Enabling MLT for Volume" << LOG_ENDL;
            //gnfs_enable_mlt(true);  //deleted by aettie 20131010 unused code
            OpenCollectionID(nstored_CID);
            gnplaylist_enable_for_playlisting(nstored_CID);
            //}
            //else
            //{
            //    gnplaylist_enable_for_playlisting(nstored_CID);
            //    LOG_TRACE << "MLT population is on going" << LOG_ENDL;
            //}
        }
    }
    else
        LOG_TRACE <<("CPlaylistMLDBCreator :EnableForPlaylisting query failed !!!") << LOG_ENDL;

    delete query;
                closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close();
    //m_pGracenoteInterface->UnlockGracenoteMutex(); //added by aettie 20130518 ISV 83208
}
    return true;
}


/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::DeleteCollectionID
 *
 * Description 	: Close the collection and delete from CID_volume

 * Parameters 	: nvolume - volume like usb, jukebox, disc

 * Returns 	: NULL
 *
 *******************************************************************
 */
void CPlaylistMLDBCreator::DeleteCollectionID(int cid)
{
    //Get reference to database
    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("CID");

    bool result = createConnection(fsDBPath, connectionName, db);
    if(result)
     {
        //m_pGracenoteInterface->LockGracenoteMutex(); //added by aettie 20130518 ISV 83208
        
        QSqlQuery* query = new QSqlQuery(db);
        query->prepare("DELETE FROM CollectionID_Data "
                           "where CID=?");
        query->addBindValue(cid);
        bool result = query->exec();

        if(result)
        {
            // Ignore the error code, as there is no action to be taken
            gnplaylist_delete_collection_id(cid);
            LOG_TRACE << "CPlaylistMLDBCreator ::  DeleteCollectionID collectionid deleted query passed" << LOG_ENDL;
        }
        else
        {
            LOG_TRACE << "CPlaylistMLDBCreator ::  DeleteCollectionID collectionid deleted query failed !!!" << LOG_ENDL;
        }

        delete query;
                    closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12 
//            db.close();
        //m_pGracenoteInterface->UnlockGracenoteMutex(); //added by aettie 20130518 ISV 83208
    }
}

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::LogError(gn_error_t error)
 *
 * Description 	: Log error
 *
 * Parameters 	: error - Error code
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CPlaylistMLDBCreator::LogError(gn_error_t error)
{
    const gn_uchar_t* errorStr = gnerr_get_code_desc( error);

    QString txt;
    txt.append (QString("Error: %1").arg(reinterpret_cast<const char*>(errorStr)));

    LOG_TRACE << txt << LOG_ENDL;
}

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::DisableAllCollectionsForPlaylisting
 *
 * Description 	: Disable all the collections from playlisting
 *
 * Parameters 	: None
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CPlaylistMLDBCreator::DisableAllCollectionsForPlaylisting()
{
    LOG_TRACE << "CPlaylistMLDBCreator : DisableAllCollectionsForPlaylisting Start" << LOG_ENDL;

    gn_collection_id_list_t* enabledList;
    gn_error_t error = gnplaylist_get_enabled_for_playlisting_list(&enabledList);

    if(error == GN_SUCCESS)
    {
        gn_collection_id_t* enabledIds = enabledList->ids;
        gn_uint32_t count = enabledList->id_count;

        LOG_TRACE << "Count of open collections:" << count << LOG_ENDL;

        // Disable all the enabled collections from playlisting
        for(gn_uint32_t index = 0; index < count; index++)
        {
            gn_collection_id_t id = *enabledIds;

            gnplaylist_disable_for_playlisting(id);
            enabledIds++;
        }
    }

    gnplaylist_free_collection_id_list(&enabledList);

    LOG_TRACE << "CPlaylistMLDBCreator : DisableAllCollectionsForPlaylisting End" << LOG_ENDL;
}

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::CloseCollections
 *
 * Description 	: Close all the open collections
 *
 * Parameters 	: None
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CPlaylistMLDBCreator::CloseCollections()
{
    LOG_TRACE << "CPlaylistMLDBCreator : CloseCollections Start" << LOG_ENDL;

    gn_collection_id_list_t* openList;

    gn_error_t error = gnplaylist_get_opened_collection_id_list(&openList);

    if(error == GN_SUCCESS)
    {
        gn_collection_id_t* openIds = openList->ids;
        gn_uint32_t count = openList->id_count;

        LOG_TRACE << "Count of open collections:" << count << LOG_ENDL;

        // Close all the open collections
        for(gn_uint32_t index = 0; index < count; index++)
        {
            gn_collection_id_t id = *openIds;

            CloseCollectionID(id);
            openIds++;
        }
    }

    gnplaylist_free_collection_id_list(&openList);
    LOG_TRACE << "CPlaylistMLDBCreator : CloseCollections End" << LOG_ENDL;
}

/*
 *******************************************************************

 * Function	: CPlaylistMLDBCreator::DeleteLeastRecentlyUsedMLDB
 *
 * Description 	: Deletes Least Recently used MLDB

 *
 * Parameters 	: nVolume
 *

 * Returns 	: None
 *
 *******************************************************************

 */
void CPlaylistMLDBCreator::DeleteLeastRecentlyUsedMLDB(QList<int> CIDList, QList<QDateTime> accessDates, int nVolume)
{
    LOG_TRACE << "CPlaylistMLDBCreator : DeleteLeastRecentlyUsedMLDB Start" << LOG_ENDL;

    if((nVolume== e_IPOD1 || nVolume== e_IPOD2) && (CIDList.count() >= IPOD_MLDB_count))
    {
        QDateTime least = accessDates[0];
        int pos =0;

        for(int i=1; i<accessDates.count(); i++)
        {
            if(least > accessDates[i])
            {
                least = accessDates[i];
                pos = i;
            }
        }

        int LRU_CID = CIDList[pos];
        LOG_TRACE << "CPlaylistMLDBCreator : LRU_CID" << LOG_ENDL;
        LOG_TRACE << LRU_CID << LOG_ENDL;
        DeleteCollectionID(LRU_CID);
    }
    else if((nVolume== e_USB1 || nVolume== e_USB2) && (CIDList.count() >= USB_MLDB_count))
    {
        QDateTime least = accessDates[0];
        int pos =0;

        for(int i=1; i<accessDates.count(); i++)
        {
            if(least > accessDates[i])
            {
                least = accessDates[i];
                pos = i;
            }
        }

        int LRU_CID = CIDList[pos];
        LOG_TRACE << "CPlaylistMLDBCreator : LRU_CID" << LOG_ENDL;
        LOG_TRACE << LRU_CID << LOG_ENDL;

        QSqlDatabase db;
        QString fsDBPath;
        fsDBPath.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

        QString connectionName;
        connectionName.append("CID");
        QString mountPath; //added by Puneet for fixing GPP crash issue 2013.06.12
        bool result = createConnection(fsDBPath, connectionName, db);
        if(result)
         {
           // m_pGracenoteInterface->LockGracenoteMutex(); //added by aettie 20130518 ISV 83208
            QSqlQuery* query = new QSqlQuery(db);

            //Get mount path for the CID
            query->prepare("SELECT  MountPath from CollectionID_Data "
                                   "where CID = ?");
            query->addBindValue(LRU_CID);
            bool result = query->exec();

            if(!result)
            {
                LOG_TRACE << "CPlaylistMLDBCreator : mountPath query failed !!!" << LOG_ENDL;
            }
            else
            {
                LOG_TRACE << "CPlaylistMLDBCreator : mountPath query passed" << LOG_ENDL;

                // QString mountPath; //Removed by Puneet for fixing GPP crash issue 2013.06.12
                while (query->next())
                {
                    mountPath = query->value(0).toString();
                    mountPath.prepend("'");
                    mountPath.append("%");
                    mountPath.append("'");
                    break;
                }

                LOG_TRACE << "CPlaylistMLDBCreator : mountPath value = ";
                LOG_TRACE << mountPath << LOG_ENDL;

            }
            delete query;
            closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12
            //db.close();
            CGracenoteFileHandling* pFileHandling = new CGracenoteFileHandling;
            result = pFileHandling->DeleteUSBFiles(mountPath);
            delete pFileHandling;
            //  m_pGracenoteInterface->UnlockGracenoteMutex(); //added by aettie 20130518 ISV 83208
        }
        DeleteCollectionID(LRU_CID);
        m_NormalizedData->DeleteSQLiteDB(LRU_CID); //added by Divya 2013.01.17 for gracenote normalized db
    }
    LOG_TRACE << "CPlaylistMLDBCreator : DeleteLeastRecentlyUsedMLDB End" << LOG_ENDL;
}

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::GetConnectStatus
 *
 * Description 	: Retrieves the Connection status from the SQLite DB
 *                Based on volume and serial number
 *
 * Parameters 	: nVolume - Volume of the device connected
 *                serialNum - Serial number of the device connected
 * Returns 	: None
 *
 *******************************************************************
 */
int CPlaylistMLDBCreator::GetConnectStatus(QString serialNum)
{
    LOG_TRACE <<("CPlaylistMLDBCreator: GetConnectStatus Start") << LOG_ENDL;
    int connectStatus = 0;

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("CID");

    if(createConnection(fsDBPath, connectionName, db))
     {
      //  m_pGracenoteInterface->LockGracenoteMutex(); //added by aettie 20130518 ISV 83208
        QSqlQuery* query = new QSqlQuery(db);
        query->prepare("SELECT  Connect from CollectionID_Data "
                           "where SerialNum = ?");
        query->addBindValue(serialNum);
        bool result = query->exec();
        if(result)
        {
            LOG_TRACE <<("CPlaylistMLDBCreator: GetConnectStatus query passed") << LOG_ENDL;
            while (query->next())
            {
                connectStatus =  query->value(0).toInt();
                LOG_TRACE <<("CPlaylistMLDBCreator: GetConnectStatus val") << connectStatus << LOG_ENDL;
            }//End while (query.next())
        }//End  if(result)
        else
        {
            LOG_TRACE <<("CPlaylistMLDBCreator: GetConnectStatus query failed !!!") << LOG_ENDL;
        }

        delete query;
        closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close();
       // m_pGracenoteInterface->UnlockGracenoteMutex(); //added by aettie 20130518 ISV 83208
    }

    return connectStatus;
}

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::UpdateConnectStatus
 *
 * Description 	: Updates the Connection status to the SQLite DB
 *                Based on volume and serial number
 *
 * Parameters 	: nVolume - Volume of the device connected
 *                serialNum - Serial number of the device connected
 *                ConnectStatus - 1 if connected, 0 if disconnected
 * Returns 	: None
 *
 *******************************************************************
 */
//{deleted by aettie 2013.03.22 for ITS 148383
//void CPlaylistMLDBCreator::UpdateConnectStatus(int ConnectStatus, QString serialNum)
//{
//    LOG_TRACE <<("CPlaylistMLDBCreator: UpdateConnectStatus Start") << LOG_ENDL;
//
//    QSqlDatabase db;
//    QString fsDBPath;
//    fsDBPath.append("/gracenote/CID_volume");

//    QString connectionName;
//    connectionName.append("CID");

//    if(createConnection(fsDBPath, connectionName, db))
//    {
//        QSqlQuery* query = new QSqlQuery(db);

//        if(serialNum.isNull() || serialNum.isEmpty())
//        {
//			//modified by aettie.ji 2013.02.22 for ISV 71131 etc.
//            //query->prepare("UPDATE CollectionID_Data SET Connect = ?");
//            query->prepare("UPDATE CollectionID_Data SET Connect = ? where Volume != '0'"); //if volume is jukebox, connection is always 1
//            query->addBindValue(ConnectStatus);

//        }
//        else
//        {
//            query->prepare("UPDATE CollectionID_Data SET Connect = ? where SerialNum = ?");
//            query->addBindValue(ConnectStatus);
//            query->addBindValue(serialNum);

//        }

//        bool result = query->exec();
//        if(!result)
//            LOG_TRACE <<("CPlaylistMLDBCreator: UpdateConnectStatus query failed !!!") << LOG_ENDL;

//        delete query;
//        db.close();
//    }
//    LOG_TRACE <<("CPlaylistMLDBCreator: UpdateConnectStatus End") << LOG_ENDL;
//}
//}deleted by aettie 2013.03.22 for ITS 148383

//{added by aettie 2013.03.22 for ITS 148383
 void CPlaylistMLDBCreator::UpdateConnectStatus(int ConnectStatus, int nVolume, QString serialNum)
{
    LOG_TRACE <<("CPlaylistMLDBCreator: UpdateConnectStatus Start") << LOG_ENDL;

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("CID");

    if(createConnection(fsDBPath, connectionName, db))
    {
      //  m_pGracenoteInterface->LockGracenoteMutex(); //added by aettie 20130518 ISV 83208
        QSqlQuery* query = new QSqlQuery(db);

        if(nVolume == -1||serialNum.isNull() || serialNum.isEmpty())
        {
            query->prepare("UPDATE CollectionID_Data SET Connect = ? where Volume != '0'"); //if volume is jukebox, connection is always 1
            query->addBindValue(ConnectStatus);
        }
        else
        {
            query->prepare("UPDATE CollectionID_Data SET Connect = ? where SerialNum = ? AND Volume = ?");
            query->addBindValue(ConnectStatus);
            query->addBindValue(serialNum);
            query->addBindValue(nVolume);
        }

        bool result = query->exec();
        if(!result)
            LOG_TRACE <<("CPlaylistMLDBCreator: UpdateConnectStatus query failed !!!") << LOG_ENDL;

        delete query;
        closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close();
      //  m_pGracenoteInterface->UnlockGracenoteMutex(); //added by aettie 20130518 ISV 83208
    }
    LOG_TRACE <<("CPlaylistMLDBCreator: UpdateConnectStatus End") << LOG_ENDL;
}
//}added by aettie 2013.03.22 for ITS 148383

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::MLDBMaintenance
 *
 * Description 	: Maintains the MLDBs based on the configuration set by
 *                the customer
 *
 * Parameters 	: nVolume - Volume of the device connected
 * Returns 	: None
 *
 *******************************************************************
 */
void CPlaylistMLDBCreator::MLDBMaintenance(int nVolume )
{
    LOG_TRACE << "CPlaylistMLDBCreator : MLDBMaintenance Start" << LOG_ENDL;

    if(nVolume == e_JukeBox || nVolume == e_Disc)
    {
        return;
    }

    QList<int> CIDList;
    QList<QDateTime> accessDates;

    //Get the total number of MLDBs created for a volume
    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("CID");

    if(createConnection(fsDBPath, connectionName, db))
     {
      //  m_pGracenoteInterface->LockGracenoteMutex(); //added by aettie 20130518 ISV 83208
        QSqlQuery* query = new QSqlQuery(db);
        query->prepare("SELECT  CID, AccessDate from CollectionID_Data "
                           "where Volume=? OR Volume=?");

        if(nVolume == e_USB1 || nVolume == e_USB2)
        {
            query->addBindValue(e_USB1);
            query->addBindValue(e_USB2);
        }
        else if(nVolume == e_IPOD1 || nVolume == e_IPOD2)
        {
            query->addBindValue(e_IPOD1);
            query->addBindValue(e_IPOD2);
        }

        bool result = query->exec();
        if(result)
        {
            LOG_TRACE <<("CPlaylistMLDBCreator: MLDBMaintenance query passed") << LOG_ENDL;
            while (query->next())
            {
                CIDList.append(query->value(0).toInt());
                accessDates.append(QDateTime::fromString(query->value(1).toString(), "ddd MMM dd hh:mm:ss yyyy"));
            }
        }
        else
        {
            LOG_TRACE <<("CPlaylistMLDBCreator: MLDBMaintenance query failed !!!") << LOG_ENDL;
        }

        LOG_TRACE <<("CPlaylistMLDBCreator: CID list count") << LOG_ENDL;
        LOG_TRACE <<(CIDList.count()) << LOG_ENDL;

        //Free memory
        delete query;
        closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close();
       // m_pGracenoteInterface->UnlockGracenoteMutex(); //added by aettie 20130518 ISV 83208
    }

    //If the total exceeds the Maximum to be created Delete the LRU
    if(nVolume == e_IPOD1 || nVolume == e_IPOD2)
    {
        if(CIDList.count() >= IPOD_MLDB_count)
        {
            DeleteLeastRecentlyUsedMLDB(CIDList, accessDates, nVolume);
        }
    }
    else if(nVolume == e_USB1 || nVolume == e_USB2)
    {
        if(CIDList.count() >= USB_MLDB_count)
        {
            DeleteLeastRecentlyUsedMLDB(CIDList, accessDates, nVolume);
        }
    }
    LOG_TRACE << "CPlaylistMLDBCreator : MLDBMaintenance END" << LOG_ENDL;
}

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::UpdateMountPathInDB
 *
 * Description 	: Updates mountPath in the SQLite DB
 *
 * Parameters 	: mountPath - mountPath of the device connected
 *                nCID - Collection ID created for the volume
 * Returns 	: None
 *
 *******************************************************************
 */
void CPlaylistMLDBCreator::UpdateMountPathInDB(QString mountPath , int nCID)
{
    LOG_TRACE << "CPlaylistMLDBCreator : UpdateMountPathInDB mountPath =";
    LOG_TRACE << mountPath << LOG_ENDL;

    LOG_TRACE << "CPlaylistMLDBCreator : UpdateMountPathInDB nCID =";
    LOG_TRACE << nCID << LOG_ENDL;

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("CID");

    if(createConnection(fsDBPath, connectionName, db))
     {
       // m_pGracenoteInterface->LockGracenoteMutex(); //added by aettie 20130518 ISV 83208
        QSqlQuery* query = new QSqlQuery(db);
        query->prepare("UPDATE CollectionID_Data SET MountPath = ?, Connect = ? WHERE CID = ?");

        int connectStatus = 1;
        query->addBindValue(mountPath);
        query->addBindValue(connectStatus);
        query->addBindValue(nCID);

        bool result = query->exec();
        if(!result)
            LOG_TRACE << "CPlaylistMLDBCreator : UpdateMountPathInDB query execution failed !!!" << LOG_ENDL;

        delete query;
        closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close();
      //  m_pGracenoteInterface->UnlockGracenoteMutex(); //added by aettie 20130518 ISV 83208
    }
}

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::SetMLDBLocation
 *
 * Description 	: Set the MLDB location path
 *
 * Parameters 	: mldbLocation - string containing MLDB location
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CPlaylistMLDBCreator::SetMLDBLocation(QString mldbLocation)
{
    qDebug() << "MLDB location" << mldbLocation;
    m_MLDBLocation = mldbLocation;
}

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::CopyMLDBToRAM
 *
 * Description 	: Copy MLDB to RAM
 *
 * Parameters 	: collectionId - collection id
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CPlaylistMLDBCreator::CopyMLDBToRAM(int collectionId)
{
    LOG_TRACE << "CPlaylistMLDBCreator::CopyMLDBToRAM : COPy from FS to TMPS"  << collectionId << LOG_ENDL;
    return;
	//MLDB on RAM changes commented as no major performance improvement seen
	/*
    QString fsPath = m_MLDBLocation;
    fsPath.append("mldb%1.pdb");
    fsPath = fsPath.arg(collectionId);

    QString tmpfsPath = "/dev/shm/mldb%1.pdb";
    tmpfsPath = tmpfsPath.arg(collectionId);

    QFile fsmldb(fsPath);
    QFile tmpfsmldb(tmpfsPath);

    // Copy the mldb from the FS to TMPFS
    if(fsmldb.exists())
    {
        if(fsmldb.size() == 0)
        {
            LOG_TRACE << "CPlaylistMLDBCreator::CopyMLDBToRAM : fsmldb size is 0"  << LOG_ENDL;
	}
        else if(fsmldb.copy(tmpfsPath))
        {
            sync();
            LOG_TRACE << "CPlaylistMLDBCreator::CopyMLDBToRAM : Sync Called"  << LOG_ENDL;
            //if(fsync(tmpfsmldb.handle()) == 0)
            //   {
            //    LOG_TRACE << "CPlaylistMLDBCreator::CopyMLDBToRAM: fsync successful" << LOG_ENDL;
		//}
	    //else
         //       {
         //       LOG_TRACE << "CPlaylistMLDBCreator::CopyMLDBToRAM: fsync failed" << LOG_ENDL;
         //       }
	}
        fsmldb.close();
        tmpfsmldb.close();
        // MLT will not work, because mldb population is happening on tmpfs
        //gnfs_enable_mlt(false);
    }*/
}

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::BackupMLDBOnFS
 *
 * Description 	: Copy the MLDB from RAM to FS
 *
 * Parameters 	: collectionId - collection id
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CPlaylistMLDBCreator::BackupMLDBOnFS(int collectionId)
{
    LOG_TRACE << "CPlaylistMLDBCreator::BackupMLDBOnFS: Start" << LOG_ENDL;
	return;
   /* QString fsPath = m_MLDBLocation;
    fsPath.append("mldb%1.pdb");
    fsPath = fsPath.arg(collectionId);

    QString tmpfsPath = "/dev/shm/mldb%1.pdb";
    tmpfsPath = tmpfsPath.arg(collectionId);

    QFile tmpfsmldb(tmpfsPath);
    QFile fsmldb(fsPath);

     LOG_TRACE << "CPlaylistMLDBCreator::BackupMLDBOnFS: start copy operation" << LOG_ENDL;
    // Copy MLDB from TMPFS to FS, and delete tmpfs mldb
        if(tmpfsmldb.exists() && (tmpfsmldb.size() != 0))
        {
            LOG_TRACE << "CPlaylistMLDBCreator::BackupMLDBOnFS: TMPFS MLDB exists" << LOG_ENDL;
            // if any mldb is present in FS, delete it
            if(fsmldb.exists())
            {
                LOG_TRACE << "CPlaylistMLDBCreator::BackupMLDBOnFS: Remove FS MLDB" << LOG_ENDL;
                fsmldb.remove();
            }
            if(tmpfsmldb.copy(fsPath))
            {
             LOG_TRACE << "CPlaylistMLDBCreator::BackupMLDBOnFS: Sync Called" << LOG_ENDL;
             sync();
             tmpfsmldb.remove();
            }
            //if(fsync(fsmldb.handle()) == 0)
		//{
		//LOG_TRACE << "CPlaylistMLDBCreator::BackupMLDBOnFS: fsync successful" << LOG_ENDL;
		//}
	    //else
         //       {
          //      LOG_TRACE << "CPlaylistMLDBCreator::BackupMLDBOnFS: fsync failed" << LOG_ENDL;
          //      }

            LOG_TRACE << "CPlaylistMLDBCreator::BackupMLDBOnFS: Backup on FS" << LOG_ENDL;
        }
    fsmldb.close();
    tmpfsmldb.close();
    LOG_TRACE << "CPlaylistMLDBCreator::BackupMLDBOnFS: End" << LOG_ENDL;*/
    }


/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::UpdateMLDBState
 *
 * Description 	: Updates the MLDB population state to the
 *                       CID_Volume based on serial number.
 *
 * Parameters 	: state - MLDB population state
 *                        serialNum - Serial number of the device connected
 * Returns 	: None
 *
 *******************************************************************
 */
void CPlaylistMLDBCreator::UpdateMLDBState(int state, QString serialNum)
{
    LOG_TRACE <<("CPlaylistMLDBCreator: UpdateMLDBState Start") << state << " : Num : " << serialNum << LOG_ENDL;

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("CID");

    if(createConnection(fsDBPath, connectionName, db))
    {
       // m_pGracenoteInterface->LockGracenoteMutex(); //added by aettie 20130518 ISV 83208
        QSqlQuery* query = new QSqlQuery(db);
        query->prepare("UPDATE CollectionID_Data SET PopulationState = ? where SerialNum = ?");
        query->addBindValue(state);

        if(serialNum.length() == 0)
            query->addBindValue("");
        else
            query->addBindValue(serialNum);

        bool result = query->exec();
        if(!result)
            LOG_TRACE <<("CPlaylistMLDBCreator: UpdateMLDBState query failed !!!") << LOG_ENDL;

        delete query;
        closeConnection(db);	//added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close();
       // m_pGracenoteInterface->UnlockGracenoteMutex(); //added by aettie 20130518 ISV 83208
    }

    LOG_TRACE <<("CPlaylistMLDBCreator: UpdateMLDBState End") << LOG_ENDL;
}

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::GetMLDBState
 *
 * Description 	: Returns MLDB population state for a serial num
 *
 * Parameters 	: serialNum - Serial number of the device
 * Returns 	: int - Population state
 *
 *******************************************************************
 */
int CPlaylistMLDBCreator::GetMLDBState(QString serialNum, QString mountPath)
{
    LOG_TRACE <<("CPlaylistMLDBCreator: GetMLDBState Start") << LOG_ENDL;
    int state = -1;

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("CID");

    if(createConnection(fsDBPath, connectionName, db))
     {
       // m_pGracenoteInterface->LockGracenoteMutex(); //added by aettie 20130518 ISV 83208
        QSqlQuery* query = new QSqlQuery(db);
        query->prepare("SELECT  PopulationState from CollectionID_Data "
                           "where SerialNum = ? And MountPath = ?");

        // "" should be used as bind value, as most of the time select and update
        // doesnt work, if we just pass empty string as bind value
        if(serialNum.length() == 0)
        {
            query->addBindValue("");
        }
        else
        {
            query->addBindValue(serialNum);
        }

        if(mountPath.length() == 0)
        {
            query->addBindValue("");
        }
        else
        {
            query->addBindValue(mountPath);
        }

        bool result = query->exec();
        if(result)
        {
            LOG_TRACE <<("CPlaylistMLDBCreator: GetMLDBState query passed") << LOG_ENDL;
            while (query->next())
            {
                state =  query->value(0).toInt();
                LOG_TRACE <<("CPlaylistMLDBCreator: GetMLDBState value : ") << state << LOG_ENDL;
            }
        }
        else
        {
            LOG_TRACE <<("CPlaylistMLDBCreator: GetMLDBState query failed !!!") << LOG_ENDL;
        }

        delete query;
        closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close();
       // m_pGracenoteInterface->UnlockGracenoteMutex(); //added by aettie 20130518 ISV 83208
    }

    // modified by aettie.ji 2012.10.30 for static analysis #7102
    LOG_TRACE <<("CPlaylistMLDBCreator: GetMLDBState End") << LOG_ENDL;

    return state;
    //LOG_TRACE <<("CPlaylistMLDBCreator: GetMLDBState End") << LOG_ENDL;
    // modified by aettie.ji 2012.10.30 for static analysis #7102
}

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::DeleteIPODEntryFromMLDB
 *
 * Description 	: Delete an entry from IPOD MLDB
 *
 * Parameters 	: collectionId - Collection Id
 *                fileName - file name to be deleted
 *                filePath - file path to be deleted
 * Returns 	: None
 *
 *******************************************************************
 */
void CPlaylistMLDBCreator::DeleteIPODEntryFromMLDB(int collectionId, QString fileName, QString filePath)
{
    //Open Collection ID
    gn_error_t nError = GN_FAILURE;
    nError =  gnplaylist_open_collection_id(collectionId);

    gn_uchar_t* gfileName = gnepal_strdup( (gn_uchar_t *)  (fileName.toUtf8().constData()) );
    gn_uchar_t* gfilePath = gnepal_strdup( (gn_uchar_t *)  (filePath.toUtf8().constData()) );

    LOG_TRACE <<("CPlaylistMLDBCreator: DeleteIPODEntryFromMLDB");
    LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(gfileName)));
    LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(gfilePath)));


    if(gfileName != GN_NULL)
    {
        nError = gnplaylist_deleteentry(collectionId, gfileName, gfilePath);
        LOG_TRACE <<("CPlaylistMLDBCreator: gnplaylist_deleteentry error") << LOG_ENDL;
        LogError(nError);
    }

    if(gfileName != NULL) gnepal_mem_free(gfileName); gfileName = NULL;
    if(gfilePath != NULL) gnepal_mem_free(gfilePath); gfilePath = NULL;

	//{added by Divya 2013.01.17 for gracenote normalized db
    //Delete the entries from the SQLite DB
    QStringList path;
    path.append(filePath);
    m_NormalizedData->DeleteRecords(collectionId, path);
	//}added by Divya 2013.01.17 for gracenote normalized db
}

/*
 *******************************************************************
 * Function	: CPlaylistMLDBCreator::UpdateIPODEntryFromMLDB
 *
 * Description 	: Update an entry in IPOD MLDB
 *
 * Parameters 	: collectionId - Collection Id
 *                fileName - file name to be deleted
 *                filePath - file path to be deleted
 * Returns 	: None
 *
 *******************************************************************
 */
void CPlaylistMLDBCreator::UpdateIPODEntryFromMLDB(int collectionId, QString fileName, QString filePath)
{

    gn_error_t nError = GN_FAILURE;
    nError =  gnplaylist_open_collection_id(collectionId);

    gn_uchar_t* gfileName = gnepal_strdup( (gn_uchar_t *)  (fileName.toUtf8().constData()) );
    gn_uchar_t* gfilePath = gnepal_strdup( (gn_uchar_t *)  (filePath.toUtf8().constData()) );

    LOG_TRACE <<("CPlaylistMLDBCreator: UpdateIPODEntryFromMLDB");
    LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(gfileName)));
    LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(gfilePath)));

    if(gfileName != GN_NULL)
    {
        //check if the entry already exists
        gnplaylist_entrydata_t	metadata = GN_NULL;
        nError =  gnplaylist_getentry (collectionId, gfileName, NULL, NULL, &metadata);

        LOG_TRACE <<("CPlaylistMLDBCreator: gnplaylist_getentry");
        LogError(nError);

        if(nError == GN_SUCCESS  && metadata != GN_NULL)
        {
            //Set the new path for the entry
            gnplaylist_setentryfieldval(metadata, gnpl_crit_field_path_name,gfilePath);
            gn_error_t err = gnplaylist_updateentry(gfileName, NULL, metadata);

            //Update entry in the SQLite DB
            m_NormalizedData->UpdateRecord(collectionId, fileName, filePath); //added by Divya 2013.01.17 for gracenote normalized db

            LOG_TRACE << "UpdateIPODEntryFromMLDB Error" << LOG_ENDL;
            LogError(err);
        }

        // Free the data structure
        gnplaylist_freeentry(&metadata);
        if(gfileName != NULL) gnepal_mem_free(gfileName); gfileName = NULL;
        if(gfilePath != NULL) gnepal_mem_free(gfilePath); gfilePath = NULL;
    }

}


//End of file
