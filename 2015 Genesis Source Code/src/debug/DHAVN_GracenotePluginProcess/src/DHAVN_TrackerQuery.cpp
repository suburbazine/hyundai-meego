
/*
 ******************************************************************
 *    COPYRIGHT ?Teleca AB      *
 *----------------------------------------------------------------*
 * MODULE     	:  CTrackerQuery                             	  *
 *
 * PROGRAMMERS 	:  Divya Dsouza / Vishal Jain
 * DATE       	:  29 September 2011                             	  *
 * VERSION    	:                                             	  *
 *
 *----------------------------------------------------------------*
 *                                                                *
 *  MODULE SUMMARY : CTrackerQuery supports the features of retrieving
       data from Tracker-Store using TrackerAbstractor
       and inserting data into Tracker-Store using
       TrackerExtractor
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

#include <QStringList>
#include <QFile>
#include <QDebug>
#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDirIterator>
#include <QUrl>
#include "DHAVN_TrackerQuery.h"

#include <DHAVN_TrackerAbstractor.h>
#include <DHAVN_GracenoteInterface.h>
#include "DHAVN_GracenotePlaylistMLDBCreator.h"
#include "DHAVN_GracenoteFileHandling.h"
#include "DHAVN_GracenoteRequestQueue.h"
#include "DHAVN_Performance.h"
#include <DHAVN_GracenoteDBconnection.h>

extern int HighlightedVolume;
extern int IPOD1ConnectStatus;
extern int IPOD2ConnectStatus;

extern int USB1ConnectStatus;
extern int USB2ConnectStatus;

/*
 *******************************************************************
 * Function		: CTrackerQuery::CTrackerQuery
 *
 * Description 	: Constructor for CTrackerQuery

 * Parameters 	: parent (IN) -

 * Returns 		: None
 *
 *******************************************************************
 */
CTrackerQuery::CTrackerQuery(QObject* parent)
{
    Q_UNUSED(parent);

    m_pTrackerAbstractor = NULL;
    m_pGracenoteInterface = NULL;
    m_pFileHandling = NULL;
    m_JukeIndex = 0;
    m_Timer = NULL;
    m_pQueue = NULL;
    m_JukeBoxNewFileList.clear();
    m_USBNewFileList.clear();
    m_JukeIndex = 0;
    m_StopMLDBPopulation = false;
}

/*
 *******************************************************************
 * Function		: CTrackerQuery::Init
 *
 * Description 	: Second phase construction

 * Parameters 	:

 * Returns 		: Bool
 *
 *******************************************************************
 */
bool CTrackerQuery::Init()
{
    bool initStatus = false;
    LOG_INIT_V2("Media_GracenotePluginProcess");

    m_pTrackerAbstractor = new CTrackerAbstractor;
    if(!m_pTrackerAbstractor)
        return initStatus;

    m_pFileHandling = new CGracenoteFileHandling;
    if(!m_pFileHandling)
        return initStatus;
    //{ modified by aettie.ji 2012.10.30 for static analysis #7100
    else
    {
        initStatus = m_pFileHandling->Init();    
        if(!initStatus)
        {
            qDebug() << "m_pFileHandling initialize error";
            return initStatus;
        }
    }
    qDebug() << "m_pFileHandling initialized properly";

    /*
    else
    {
        initStatus = m_pFileHandling->Init();
            if(!initStatus)
            return initStatus;    }

    if(initStatus)
    {
        qDebug() << "m_pFileHandling initialized properly";
    }
    else
    {
        qDebug() << "m_pFileHandling initialize error";
    }*/

    //} modified by aettie.ji 2012.10.30 for static analysis #7100

    initStatus = true;
    return initStatus;
}

/*
 *******************************************************************
 * Function		: CTrackerQuery::~CTrackerQuery
 *
 * Description 	: Destructor for CTrackerQuery

 * Parameters 	: parent (IN) -

 * Returns 		: None
 *
 *******************************************************************
 */
CTrackerQuery::~CTrackerQuery()
{
    if(m_pTrackerAbstractor) {
        delete m_pTrackerAbstractor;
        m_pTrackerAbstractor = NULL;
    }

    if(m_pFileHandling)
    {
        delete m_pFileHandling;
        m_pFileHandling = NULL;
    }
}

/*
 *******************************************************************
 * Function		: CTrackerQuery::SetGracenoteInterface
 *
 * Description 	: Set the instance of CGracenoteInterface here

 * Parameters 	: aInterface (IN) - Pointer to CGracenoteInterface

 * Returns 		: None
 *
 *******************************************************************
 */
void CTrackerQuery::SetGracenoteInterface(CGracenoteInterface* aInterface)
{
    // Gracenote interface
    m_pGracenoteInterface = aInterface;
}

/*
 *******************************************************************
 * Function	: CTrackerQuery::GetFilePathsInJukebox
 *
 * Description 	: Issue the request to get the file paths stored in
       Tracker store

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
void CTrackerQuery::GetFilePathsInJukebox()
{
    LOG_TRACE << "CTrackerQuery : GetFilePathsInJukebox Start" << LOG_ENDL;
    int index = 0;

    PT_RESET_TIME;

    //Indexing for Jukebox started
    //emit indexingStatus(0, e_JukeBox); //deleted by aettie 2013.03.19 for Gracenote issue

    // Check if Jukebox population was complete
    int state = m_pGracenoteInterface->GetMLDBCreator()->GetMLDBState();
    LOG_TRACE << "CTrackerQuery : GetFilePathsInJukebox state" << state << LOG_ENDL;

    if(state == 0)
    {
        // Delete the jukebox files from the Files database
        // If there are some entries populated, they will not be added again
        // as CheckIfEntryExists will not allow it
        m_pFileHandling->DeleteJukeboxFiles();
    }

    PT_START_TIME(PT_OBJ_UPDATE_FILES_IN_JUKEBOX, 0);
    UpdateFilesInJukebox();
    PT_END_TIME(PT_OBJ_UPDATE_FILES_IN_JUKEBOX, 0);

    gn_collection_id_t nCID = m_pGracenoteInterface->GetCollectionId(e_JukeBox);

    //Copy SQLite DB for population into tmpfs
    m_pGracenoteInterface->GetNormalizedDataObject()->CreateSQLiteDB(nCID); //added by Divya 2013.01.17 for gracenote normalized db
    PT_SET_FILECOUNT(m_JukeBoxNewFileList.count());
    PT_START_TIME(PT_OBJ_UPDATE_MLDB_JB, 0);

   for(index = m_JukeIndex; index < m_JukeBoxNewFileList.count(); index++)
    {
        //Verify the maximum size of mldb on RAM and stop further population of DB
        if(IsMLDBSizeLimitReached())
        {
            //Reset the flag to false
            SetMLDBSizeState(false);
            LOG_TRACE <<("GetFilePathsInJukebox : Maximum size of mldb file is reached - Stop populating DB") << LOG_ENDL;
            break;
        }
        // update the gracenote MLDB with all the files
        UpdateMLDBForJukeBox(m_JukeBoxNewFileList.at(index), nCID);

        //If highlighted volume info is changed, break from loop
        if(HighlightedVolume != e_JukeBox )
        {
            LOG_INFO << "STOP updating and Enqueue JukeBox Back" << LOG_ENDL;
            /* preemption occured enqueue back to update MLDB for rest of the files */
            m_pQueue->Enqueue(e_JukeBox, JUKE);

            //Move SQlite DB from tmpfs to FS
            m_pGracenoteInterface->GetNormalizedDataObject()->CopySQLiteDBtoFS(nCID); //added by Divya 2013.01.17 for gracenote normalized db
            break;
        }
        m_JukeIndex++;
    }

    /* If no preemption reset the index and clear the list */
    if(index == m_JukeBoxNewFileList.count())
    {
        m_JukeIndex = 0;
        m_JukeBoxNewFileList.clear();

        int state = 1;
        m_pGracenoteInterface->GetMLDBCreator()->UpdateMLDBState(state);
        LOG_TRACE << "CTrackerQuery : GetFilePathsInJukebox state updated" << state << LOG_ENDL;

        //Indexing for Jukebox finished
        //emit indexingStatus(1, e_JukeBox); //deleted by aettie 2013.03.19 for Gracenote issue

        //LOG_TRACE << "CTrackerQuery : XSampaDataUpdatedInDB VIS" << LOG_ENDL; //deleted by aettie Xsampa DB is not used.
        //m_pGracenoteInterface->BackupXsampaToFS(); //deleted by aettie Xsampa DB is not used.
        //{modified by esjang 2012.11.14 for XSampa DB update with Volume Info
        //m_pGracenoteInterface->XSampaDataUpdatedInDB();
        //m_pGracenoteInterface->XSampaDataUpdatedInDB(e_JukeBox); //deleted by aettie Xsampa DB is not used.
        //}modified by esjang 2012.11.14 for XSampa DB update with Volume Info
        //Move SQlite DB to filesystem
        m_pGracenoteInterface->GetNormalizedDataObject()->CopySQLiteDBtoFS(nCID); //added by Divya 2013.01.17 for gracenote normalized db
    }

    PT_END_TIME(PT_OBJ_UPDATE_MLDB_JB, 0);

    //CLose Collection
    m_pGracenoteInterface->GetMLDBCreator()->CloseCollectionID(nCID);

    LOG_TRACE << "CTrackerQuery : GetFilePathsInJukebox End" << LOG_ENDL;
    PT_PRINT_TIME(PT_LOG_FILE_NAME);
}

/*
 *******************************************************************
 * Function		: CTrackerQuery::GetFilePathsInUSB
 *
 * Description 	: Issue the request to get the file paths stored in
       USB

 * Parameters 	: None

 * Returns 		: None
 *
 *******************************************************************
 */
void CTrackerQuery::GetFilePathsInUSB(QString serialNum, QString mountPath, int volume)
{
    LOG_TRACE << "CTrackerQuery : GetFilePathsInUSB start" << LOG_ENDL;
    int index = 0;    
    int USBIndex = 0;
    int ConnectStatus = 0;

    int state = m_pGracenoteInterface->GetMLDBCreator()->GetMLDBState(serialNum,mountPath);
    LOG_TRACE << "CTrackerQuery : GetFilePathsInUSB state" << state << LOG_ENDL;

    // O indicates start
    //emit indexingStatus(0, volume); //deleted by aettie 2013.03.19 for Gracenote issue

    if(state == 0)
    {
        LOG_TRACE << "CTrackerQuery : MLDB state is 0" << serialNum << mountPath << LOG_ENDL;
        // Delete the USB files from the Files database
        // If there are some entries populated in MLDB, they will not be added again
        // as CheckIfEntryExists will not allow it
        m_pFileHandling->DeleteUSBFiles(mountPath);
    }

    gn_collection_id_t nCID = m_pGracenoteInterface->GetCollectionId(volume, serialNum);

    m_pGracenoteInterface->GetNormalizedDataObject()->CreateSQLiteDB(nCID); //added by Divya 2013.01.17 for gracenote normalized db

    //Get USBindex from DB based on serialNum
    USBIndex = GetUSBIndex(serialNum);
    PT_RESET_TIME;
    
    PT_START_TIME(PT_OBJ_UPDATE_FILES_IN_USB, 0);
    UpdateFilesInUSB(serialNum, mountPath, volume);
    PT_END_TIME(PT_OBJ_UPDATE_FILES_IN_USB, "UpdateFilesInUSB");

    PT_SET_FILECOUNT(m_USBNewFileList.count());

    PT_START_TIME(PT_OBJ_UPDATE_MLDB_USB, 0);
    for(index = USBIndex; index < m_USBNewFileList.count(); index++)
    {
        //Verify the maximum size of mldb on RAM and stop further population of DB
        if(IsMLDBSizeLimitReached())
        {
            //Reset the flag to false
            SetMLDBSizeState(false);
            LOG_TRACE <<("GetFilePathsInUSB : Maximum size of mldb file is reached - Stop populating DB") << LOG_ENDL;
            break;
        }
        //Verify the connectstatus of USB in DB
        if( (USB1ConnectStatus == 0 && volume == e_USB1) || (USB2ConnectStatus == 0 && volume == e_USB2) )
        {
          ConnectStatus = m_pGracenoteInterface->GetMLDBCreator()->GetConnectStatus(serialNum);
            if(ConnectStatus == 0)
            {
                // Delete the entries from Files database, which are not updated
                // in MLDB but are already inserted
                QStringList notUpdatedList = m_USBNewFileList.mid(index);

                m_pFileHandling->DeleteListOfFiles(notUpdatedList);
                // Get out of the loop
                /* reset the index and clear the list */
                m_USBNewFileList.clear();

                //Copy SQLite DB back to file system
                m_pGracenoteInterface->GetNormalizedDataObject()->CopySQLiteDBtoFS(nCID); //added by Divya 2013.01.17 for gracenote normalized db
                break;
            }
        }
        else if(HighlightedVolume != volume)
        {
            LOG_INFO << "STOP updating and enqueue USB Back" << LOG_ENDL;
            /* preemption occured enqueue back to update MLDB for rest of the files */
            if(volume == e_USB1)
            {
                m_pQueue->Enqueue(e_USB1, USB);
            }
            else if(volume == e_USB2)
            {
                m_pQueue->Enqueue(e_USB2, USB);
            }

            QStringList notUpdatedList = m_USBNewFileList.mid(index);
            m_pFileHandling->DeleteListOfFiles(notUpdatedList);

            m_USBNewFileList.clear();
            //update the USBindex(number of files) in DB based on serialNum
            UpdateUSBIndex(index, serialNum, volume); //modified by aettie 2013.03.22 for ITS 148383

            //Move SQlite DB from tmpfs to FS
            m_pGracenoteInterface->GetNormalizedDataObject()->CopySQLiteDBtoFS(nCID); //added by Divya 2013.01.17 for gracenote normalized db

            break;
        }

        // update the gracenote MLDB with all the files
        UpdateMLDBForUSB(m_USBNewFileList.at(index), nCID);        

    }
    /* If no preemption reset the index and clear the list */
    if(index == m_USBNewFileList.count())
    {
        LOG_TRACE << "Population complete" << serialNum << " : " << mountPath << LOG_ENDL;

        m_USBNewFileList.clear();

        //update the USBindex in DB based on serialNum
        UpdateUSBIndex(index, serialNum, volume); //modified by aettie 2013.03.22 for ITS 148383
        // USB population is complete, so update the MLDB state
        m_pGracenoteInterface->GetMLDBCreator()->UpdateMLDBState(1,serialNum);
		//update state with serial num, so same serial different volume can cause problem.
        LOG_TRACE << "CTrackerQuery : GetFilePathsInUSB population state updated to 1" << LOG_ENDL;

        // 1 indicates finished for the specified volume
        //emit indexingStatus(1, volume); //deleted by aettie 2013.03.19 for Gracenote issue

       // LOG_TRACE << "CTrackerQuery : XSampaDataUpdatedInDB USB" << LOG_ENDL; //deleted by aettie Xsampa DB is not used.
       // m_pGracenoteInterface->BackupXsampaToFS(); //deleted by aettie Xsampa DB is not used.
        //{modified by esjang 2012.11.14 for XSampa DB update with Volume Info
        //m_pGracenoteInterface->XSampaDataUpdatedInDB();
        //m_pGracenoteInterface->XSampaDataUpdatedInDB(volume); //deleted by aettie Xsampa DB is not used.
        //}modified by esjang 2012.11.14 for XSampa DB update with Volume Info
		
		//Move SQlite DB from tmpfs to FS
        m_pGracenoteInterface->GetNormalizedDataObject()->CopySQLiteDBtoFS(nCID); //added by Divya 2013.01.17 for gracenote normalized db
    }
    m_pGracenoteInterface->GetMLDBCreator()->CloseCollectionID(nCID);

    PT_END_TIME(PT_OBJ_UPDATE_MLDB_USB, "UpdateMLDBForUSB");

    LOG_TRACE << "CTrackerQuery : GetFilePathsInUSB End" << LOG_ENDL;
    PT_PRINT_TIME(PT_LOG_FILE_NAME);
}

/*
 *******************************************************************
 * Function	: CTrackerQuery::UpdateQuery
 *
 * Description 	: Update the data in Tracker-Store

 * Parameters 	: musicData (IN) - Structure containing media info

 * Returns 	: None
 *
 *******************************************************************
 */
bool CTrackerQuery::UpdateQuery(TrackerMusicData musicData)
{
    if(musicData.CD_TOC.length() != 0)
    {
        LOG_TRACE <<("CTrackerQuery : Update Query called for CD case");
        void *data;
        data = &musicData;
        return m_pTrackerAbstractor->UpdateData(data, eInsertGracenoteMetadataQuery_Id);
    }
    return true;
}

/*
 *******************************************************************
 * Function	: CTrackerQuery::UpdateMLDBForJukeBox
 *
 * Description 	: Update the data for the file paths retrieved from
       Tracker-Store in Gracenote's MLDB

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
void CTrackerQuery::UpdateMLDBForJukeBox(QString filePath, gn_collection_id_t nCID)
{   
    QRegExp jukeBoxExp("^/usr_data/media");

    if(QFile::exists(filePath))
    {
        if(filePath.contains(jukeBoxExp))
        {
            TrackerMusicData musicData;
            musicData = m_pGracenoteInterface->AddMusicFileEntryToGraceNoteMLDB(filePath,nCID);
         
        }
    }
}

/*
 *******************************************************************
 * Function	: CTrackerQuery::UpdateMLDBForUSB
 *
 * Description 	: Updates MLDB for USB

 * Parameters 	: filePath 
 *                      nCID - Collection ID of MLDB
 * Returns 	: None

 *******************************************************************
 */
void CTrackerQuery::UpdateMLDBForUSB(QString filePath, gn_collection_id_t nCID)
{
    if(QFile::exists(filePath))
    {
        TrackerMusicData musicData;
        musicData = m_pGracenoteInterface->AddMusicFileEntryToGraceNoteMLDB(filePath,nCID);
    }
}

/*
 *******************************************************************
 * Function	: CTrackerQuery::ModifyFileName
 *
 * Description 	: Modify the file name for Tracker

 * Parameters 	: originalFileName(IN) - file name

 * Returns 	: None
 *
 *******************************************************************
 */
QString CTrackerQuery::ModifyFileName(QString originalFileName)
{

    QString fileName;
    fileName.clear();

    if(originalFileName.length() == 0)
        return fileName;

    //Remove "file://" string from the path sent by tracker. Gracenote does not
    //recognize path in such format
    fileName = originalFileName.mid(7, originalFileName.length());

    // Modify all the paramters like [,] etc changed by tracker to actual character
    return QUrl::fromPercentEncoding(fileName.toUtf8());
}

/*
 *******************************************************************
 * Function	: CTrackerQuery::DeleteCDData
 *
 * Description 	: Call DBus API to delete data from tracker store related
                          to a CD TOC

 * Parameters 	: cdTOC(IN) - CD Table of contents

 * Returns 	: bool - true if inserted, else false
 *
 *******************************************************************
 */
bool CTrackerQuery::DeleteCDData(QString cdTOC)
{
    LOG_TRACE <<("CTrackerQuery: DeleteCDData called") << LOG_ENDL;
    void *data;
    data = &cdTOC;
    return m_pTrackerAbstractor->UpdateData(data, eDeleteGracenoteCDMetadataQuery_Id);
}

/*
 *******************************************************************
 * Function	: CTrackerQuery::UpdatePhoneticData
 *
 * Description 	: Update phonetic data in XSampa DB

 * Parameters 	: phoneticData - list containing data
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CTrackerQuery::UpdatePhoneticData(QString CID, QVector<QStringList> phoneticData)
{
    LOG_TRACE <<("CTrackerQuery: UpdatePhoneticData called") << phoneticData.count() << LOG_ENDL;
    void *data;
    data = &phoneticData;

    m_pTrackerAbstractor->UpdateData(CID, data, eInsertXSampeData_Id);
}

/*
 *******************************************************************
 * Function	: CTrackerQuery::UpdateCIDInfoInXSampaDB
 *
 * Description 	: Update CID info in XSampa DB

 * Parameters 	:QStringList containting type and name
 *		 QString containing CID
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CTrackerQuery::UpdateCIDInfoInXSampaDB(QString CID, QStringList phoneticData)
{
   LOG_TRACE <<("CTrackerQuery: UpdateCIDInfoInXSampaDB called") << LOG_ENDL;
   void *data;
    data = &phoneticData;

    m_pTrackerAbstractor->UpdateData(CID, data, eUpdateCIDInfoInXSampaDB);
}

/*
 *******************************************************************
 * Function	: CTrackerQuery::CheckIfPhoneticDataExists
 *
 * Description 	: Check if phonetic data is already present in database

 * Parameters 	: type - type of data (album, artist)
                          name - name of artist or album
 *
 * Returns 	: None
 *
 *******************************************************************
 */
bool CTrackerQuery::CheckIfPhoneticDataExists(QString type, QString name)
{
    return m_pTrackerAbstractor->RequestData(type, name, eCheckExistenceOfXSampaData_Id);
}

/*
 *******************************************************************
 * Function	: CTrackerQuery::InsertFilesInformation
 *
 * Description 	: Insert the information about files in database

 * Parameters 	: None
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CTrackerQuery::InsertFilesInformation(EVolumeType volumeType, QStringList existingList, QStringList &m_NewFileList, QString mountPath)
{

    QStringList filesList;

    QString path;

    if(volumeType == e_JukeBox)
    {
        path.append("/usr_data/media");
    }
    else if(volumeType == e_USB1 || volumeType ==e_USB2 )
    {
        path.append(mountPath);
    }

    QStringList filters;
    filters.append("*.mp3");
    QDirIterator dirIterator(path, filters, QDir::NoSymLinks | QDir::Files, QDirIterator::Subdirectories);

    while(dirIterator.hasNext())
    {
        dirIterator.next();

        if(dirIterator.fileInfo().suffix() == "mp3")
        {
            // update the gracenote MLDB with all the files
            filesList.append(dirIterator.filePath());
        }
    }

    // Compare the difference in the existing list and new file list
    // and extract the list of new files

    for(int index = 0 ; index < filesList.count(); index++)
    {
        QString filePath = filesList.at(index);

        // If entry is not present in the existing list, this is a newly added file
        if(!existingList.contains(filePath))
        {
            m_NewFileList.append(filePath);            
        }
    }

    // We can insert only 500 entries at a time for file paths in sqllite db
    if(m_NewFileList.count() > SQLCOUNTLIMIT)
    {
        QStringList insertList;

        for(int index = 0; index < m_NewFileList.count(); index++)
        {
            insertList.append(m_NewFileList.at(index));

            if(insertList.count() == SQLCOUNTLIMIT)
            {
                m_pFileHandling->InsertFilesInformation(insertList);
                insertList.clear();
            }
        }

        // If there are 1200 files, 1000 files will be inserted in the loop above
        // rest of the files will be inserted here
        if(insertList.count() > 0)
        {
            m_pFileHandling->InsertFilesInformation(insertList);
            insertList.clear();
        }
    }
    else if(m_NewFileList.count() > 0)
    {
        m_pFileHandling->InsertFilesInformation(m_NewFileList);
    }

}

/*
 *******************************************************************
 * Function	: CTrackerQuery::UpdateFilesInJukebox
 *
 * Description 	: Update the file paths present in Jukebox

 * Parameters 	: None
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CTrackerQuery::UpdateFilesInJukebox()
{
    QStringList filesList;
    QStringList deletedFilesList;
    QString filePath;
    bool result = m_pFileHandling->GetJukeBoxFiles(filesList);

    if(result)
    {
        for (int index = 0 ; index < filesList.count() ; index++)
        {
            filePath = filesList.at(index);

            // If the file is not present, append this in deleted file list
            if(!QFile::exists(filePath))
            {
                deletedFilesList.append(filePath);
            }
        }

        qDebug() << "deletedFilesList and count is " << deletedFilesList.count();

        // Delete the entries from MLDB
        if(deletedFilesList.count() > 0)
        {
            m_pGracenoteInterface->DeleteMusicFileEntryFromGracenoteMLDB(deletedFilesList, e_JukeBox);

            //Delete the files from the files database
            m_pFileHandling->DeleteListOfFiles(deletedFilesList);
        }

        // Insert the information again of the files
        InsertFilesInformation(e_JukeBox, filesList, m_JukeBoxNewFileList);
    }
    else
    {
        qDebug() << "Did not get Jukebox files correctly";
    }
}


/*
 *******************************************************************
 * Function	: CTrackerQuery::UpdateFilesInUSB
 *
 * Description 	: Update the file paths present in USB

 * Parameters 	: None
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CTrackerQuery::UpdateFilesInUSB(QString serialNum, QString mountPath, int volume)
{
    QStringList filesList;
    QStringList deletedFilesList;
    QString filePath;

    if(CheckIfUSBExists(mountPath))
    {
        LOG_TRACE << "CTrackerQuery : USB is connected" << LOG_ENDL;
    }
    else
    {
        LOG_TRACE << "CTrackerQuery : USB is not connected" << LOG_ENDL; 
        return;
    }

    bool result = m_pFileHandling->GetUSBFiles(filesList, mountPath);

    if(result)
    {

        for (int index = 0 ; index < filesList.count() ; index++)
        {
            filePath = filesList.at(index);

            // If the file is not present, append this in deleted file list
            if(!QFile::exists(filePath))
            {
                deletedFilesList.append(filePath);
            }
        }
        qDebug() << "deletedFilesList.count();" << deletedFilesList.count();

        // Delete the entries from MLDB
        if(deletedFilesList.count() > 0)
        {
            m_pGracenoteInterface->DeleteMusicFileEntryFromGracenoteMLDB(deletedFilesList, volume, serialNum);

            //Delete the files from the files database
            m_pFileHandling->DeleteListOfFiles(deletedFilesList);
        }

        // Insert the information again of the files
        if(volume == e_USB1)
        {
            InsertFilesInformation(e_USB1, filesList, m_USBNewFileList, mountPath);
        }
        else if(volume == e_USB2)
        {
            InsertFilesInformation(e_USB2, filesList, m_USBNewFileList, mountPath);
        }
    }
    else
    {
        qDebug() << "Did not get USB files correctly";
    }
}

/*
 *******************************************************************
 * Function	: CTrackerQuery::CheckIfUSBExists
 *
 * Description 	: Check if USB is actually present in /media path

 * Parameters 	: None
 *
 * Returns 	: None
 *
 *******************************************************************
 */
bool CTrackerQuery::CheckIfUSBExists(QString mountPath)
{

    QStringList filters;

    QDirIterator dirIterator(mountPath, filters, QDir::NoSymLinks| QDir::Files,  QDirIterator::Subdirectories);

    // If hasNext() returns false, it indicates there are no sub folders present in /media
    // which indicates no USB is mounted in /media

    return dirIterator.hasNext();
}


/*
 *******************************************************************
 * Function		: CTrackerQuery::SetQueueInterface
 *
 * Description 	: Set the instance of queue here

 * Parameters 	: aInterface (IN) - Pointer to CGracenoteRequestQueue

 * Returns 	: None
 *
 *******************************************************************
 */
void CTrackerQuery::SetQueueInterface(CGracenoteRequestQueue *aInterface)
{
    m_pQueue = aInterface;
}

/*
 *******************************************************************
 * Function	: CTrackerQuery::SetUSBInfo
 *
 * Description 	: stores the values of serial num and mnt path in
 *                        DB based on volume
 *
 * Parameters 	: serialNum(IN) - serial num of connected USB
      mountPath(IN) - mount path of the device
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CTrackerQuery::SetUSBInfo(QString serialNum, QString mountPath, int volume)
{
    gn_collection_id_t nCID = m_pGracenoteInterface->GetCollectionId(volume, serialNum);
    m_pGracenoteInterface->GetMLDBCreator()->UpdateMountPathInDB(mountPath , nCID);
}


/*
 *******************************************************************
 * Function	: CTrackerQuery::GetUSBInfo
 *
 * Description 	: Gets the values of serial num and mnt path
 *                        based on voulme and its connect status
 *
 * Parameters 	: serialNum(IN) - serial num of connected USB
      mountPath(IN) - mount path of the device
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CTrackerQuery::GetUSBInfo(QString &serialNum, QString &mountPath, int volume)
{
    LOG_TRACE <<("CTrackerQuery::GetUSBInfo start");

    int connectstatus = 1;
    QStringList USBinfo;

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("CID");

    if(createConnection(fsDBPath, connectionName, db))
    {
       // m_pGracenoteInterface->LockGracenoteMutex(); //added by aettie 20130518 ISV 83208
        QSqlQuery* query = new QSqlQuery(db);
        USBinfo.clear();
        query->prepare("SELECT  serialNum, mountPath from CollectionID_Data "
                           "where volume=? AND Connect= ?");
        query->addBindValue(volume);
        query->addBindValue(connectstatus);

        bool result = query->exec();

        if(result)
        {
            while(query->next())
            {
                //qDebug() << "SerialNum" << query->value(0).toString();
                //qDebug() << "Mountpath" << query->value(1).toString();
                USBinfo.append(query->value(0).toString());
                USBinfo.append(query->value(1).toString());
            }
            if(!USBinfo.isEmpty())
            {
                serialNum = USBinfo.at(0);
                mountPath = USBinfo.at(1);
            }
        }
        else
        {
            LOG_TRACE << "CTrackerQuery::GetUSBInfo query failed !!!" << LOG_ENDL;
        }

        delete query;
                    closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close(); //added by Puneet for fixing GPP crash issue 2013.06.12
       // m_pGracenoteInterface->UnlockGracenoteMutex(); //added by aettie 20130518 ISV 83208
    }

    LOG_TRACE << "CTrackerQuery::GetUSBInfo End" << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CTrackerQuery::SetMLDBSizeState
 *
 * Description 	: Sets the flag when mldb maximum size is reached
 *                        to stop further mldb population
 *
 * Parameters 	: value - state to be set
 * Returns 	: None
 *
 *******************************************************************
 */
void CTrackerQuery::SetMLDBSizeState(bool value)
{
    m_StopMLDBPopulation = value;
}


/*
 *******************************************************************
 * Function	: CTrackerQuery::IsMLDBSizeLimitReached
 *
 * Description 	: returns the state of m_StopMLDBPopulation
 *
 * Parameters 	: None
 * Returns 	: state of m_StopMLDBPopulation
 *
 *******************************************************************
 */
bool CTrackerQuery::IsMLDBSizeLimitReached()
{
    return m_StopMLDBPopulation;
}

/*
 *******************************************************************
 * Function	: CTrackerQuery::UpdateUSBIndex
 *
 * Description 	: Updates the USBIndex status to the SQLite DB
 *                Based on serial number.
 *
 * Parameters 	: index - holds the number of files updated to DB
 *                serialNum - Serial number of the device connected
 * Returns 	: None
 *
 *******************************************************************
 */
//{deleted by aettie 2013.03.22 for ITS 148383
//void CTrackerQuery::UpdateUSBIndex(int index, QString serialNum)
//{
//    LOG_TRACE <<("CTrackerQuery: UpdateUSBIndex Start") << LOG_ENDL;
//    QSqlDatabase db;
//    QString fsDBPath;
//    fsDBPath.append("/gracenote/CID_volume");

//    QString connectionName;
//    connectionName.append("CID");

//    if(createConnection(fsDBPath, connectionName, db))
//    {
//        QSqlQuery* query = new QSqlQuery(db);
//        query->prepare("UPDATE CollectionID_Data SET Idx = ? where SerialNum = ?");
//        query->addBindValue(index);
//        query->addBindValue(serialNum);
//        bool result = query->exec();

//        if(!result)
//            LOG_TRACE <<("CTrackerQuery: UpdateUSBIndex query failed !!!") << LOG_ENDL;

//        delete query;
//        db.close();
//    }
//    LOG_TRACE <<("CTrackerQuery: UpdateUSBIndex End") << LOG_ENDL;
//}
//}deleted by aettie 2013.03.22 for ITS 148383

//{added by aettie 2013.03.22 for ITS 148383
void CTrackerQuery::UpdateUSBIndex(int index, QString serialNum, int nVolume)
{
    LOG_TRACE <<("CTrackerQuery: UpdateUSBIndex Start") << LOG_ENDL;

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("CID");

    if(createConnection(fsDBPath, connectionName, db))
    {
        //  m_pGracenoteInterface->LockGracenoteMutex(); //added by aettie 20130518 ISV 83208
        QSqlQuery* query = new QSqlQuery(db);
        query->prepare("UPDATE CollectionID_Data SET Idx = ? where SerialNum = ? AND Volume = ?");
        query->addBindValue(index);
        query->addBindValue(serialNum);
        query->addBindValue(nVolume);
        bool result = query->exec();

        if(!result)
            LOG_TRACE <<("CTrackerQuery: UpdateUSBIndex query failed !!!") << LOG_ENDL;

        delete query;
        closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12
        //            db.close();
        //  m_pGracenoteInterface->UnlockGracenoteMutex(); //added by aettie 20130518 ISV 83208
    }
    LOG_TRACE <<("CTrackerQuery: UpdateUSBIndex End") << LOG_ENDL;
}
//}added by aettie 2013.03.22 for ITS 148383

/*
 *******************************************************************
 * Function	: CTrackerQuery::GetConnectedUSBIndex
 *
 * Description 	: Returns USBIndex specific to given serial num
 *
 * Parameters 	: serialNum - Serial number of the device connected
 * Returns 	: USBIndex
 *
 *******************************************************************
 */
int CTrackerQuery::GetUSBIndex(QString serialNum)
{
    LOG_TRACE <<("CTrackerQuery: GetUSBIndex Start") << LOG_ENDL;
    int index = -1;

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("CID");

    if(createConnection(fsDBPath, connectionName, db))
    {
      //  m_pGracenoteInterface->LockGracenoteMutex(); //added by aettie 20130518 ISV 83208
        QSqlQuery* query = new QSqlQuery(db);
        query->prepare("SELECT  Idx from CollectionID_Data "
                           "where SerialNum = ?");
        query->addBindValue(serialNum);
        bool result = query->exec();
        if(result)
        {
            LOG_TRACE <<("CTrackerQuery: GetUSBIndex query passed") << LOG_ENDL;
            while (query->next())
            {
                index =  query->value(0).toInt();
                LOG_TRACE <<("CTrackerQuery: GetUSBIndex val") << index << LOG_ENDL;
            }
        }
        else
        {
            LOG_TRACE <<("CTrackerQuery: GetUSBIndex query failed !!!") << LOG_ENDL;
        }

        delete query;
        closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close();
      //  m_pGracenoteInterface->UnlockGracenoteMutex(); //added by aettie 20130518 ISV 83208
    }

    //{ modified by aettie.ji 2012.10.30 for static analysis # 6969
    LOG_TRACE <<("CTrackerQuery: GetUSBIndex End") << LOG_ENDL;
    return index;

    //LOG_TRACE <<("CTrackerQuery: GetUSBIndex End") << LOG_ENDL;
    //} modified by aettie.ji 2012.10.30 for static analysis # 6969
}

// End of file
