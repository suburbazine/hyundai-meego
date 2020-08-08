/*
 ******************************************************************
 *    COPYRIGHT ?Teleca AB      *
 *----------------------------------------------------------------*
 * MODULE     	:  CGracenoteFileHandling                             	  *
 *
 * PROGRAMMERS 	:  Vishal Jain
 * DATE       	:  7 March 2012                             	  *
 * VERSION    	:                                             	  *
 *
 *----------------------------------------------------------------*
 *                                                                *
 *  MODULE SUMMARY : CGracenoteFileHandling supports feature for
       creating a file database to maintain list of files being added
       and removed
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                     *
 *
 *     DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * History
 * 7-March-2011      Vishal Jain               Create new class and API's
 ******************************************************************
 */

#include "DHAVN_GracenoteFileHandling.h"
#include <DHAVN_GracenoteDBconnection.h>

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

/*
 *******************************************************************
 * Function	: CGracenoteFileHandling::CGracenoteFileHandling
 *
 * Description 	: Constructor for CGracenoteFileHandling

 * Parameters 	: parent (IN) -

 * Returns 	: None
 *
 *******************************************************************
 */
 	 //Added by Puneet for fixing GPP crash issue 2013.06.12
CGracenoteFileHandling::CGracenoteFileHandling(QObject *parent):sem(1)
{
    Q_UNUSED(parent);
    LOG_INIT_V2("Media_GracenotePluginProcess");
}

/*
 *******************************************************************
 * Function	: CGracenoteFileHandling::~CGracenoteFileHandling
 *
 * Description 	: Destructor for CGracenoteFileHandling

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
CGracenoteFileHandling::~CGracenoteFileHandling()
{
}

/*
 *******************************************************************
 * Function	: CGracenoteFileHandling::Init
 *
 * Description 	: Initialization function

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
bool CGracenoteFileHandling::Init()
{
    qDebug() << "Init called";

    bool result =  false;
    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/FilesDB"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("FilesDatabase");

    result = createConnection(fsDBPath, connectionName, db);
    if(result)
    {
        result = CreateTable(db);

        closeConnection(db); 		 //Added by Puneet for fixing GPP crash issue 2013.06.12
//      db.close(); 	 //Removed by Puneet for fixing GPP crash issue 2013.06.12
    }
    return result;
}

/*
 *******************************************************************
 * Function	: CGracenoteFileHandling::CreateTable
 *
 * Description 	: Create the table for the files database

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
bool CGracenoteFileHandling::CreateTable(QSqlDatabase db)
{
    bool result = false;

    QStringList tableNames = db.tables();
    if(tableNames.contains("Files"))
    {
        qDebug() << "Files table is already created";
        return true;
    }

    QSqlQuery* query = new QSqlQuery(db);

    QString queryString;
    queryString.append("CREATE TABLE Files (FilePath VARCHAR(1024))");

    result = query->exec(queryString);
    if(result)
    {
        LOG_TRACE <<("CGracenoteFileHandling : CreateTable query success") << LOG_ENDL;
    }
    else
    {
        LOG_TRACE <<("CGracenoteFileHandling : CreateTable query failed !!!") << LOG_ENDL;
    }
    delete query;

    return result;
}

/*
 *******************************************************************
 * Function	: CGracenoteFileHandling::InsertFilesInformation
 *
 * Description 	: Insert the files in the list

 * Parameters 	: filesList - file paths

 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CGracenoteFileHandling::InsertFilesInformation(QStringList filesList)
{
    bool result = false;

    QString queryString = QString("INSERT INTO Files (FilePath)");

    for(int index = 0; index < filesList.count(); index++)
    {
        QString subQuery;
        subQuery.append(QString(" SELECT \"%1\"").arg(filesList.at(index)));
        queryString.append(subQuery);

        if(index < (filesList.count() - 1))
        {
            queryString.append(" UNION ALL");
        }
    }

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/FilesDB"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("FilesDatabase");

    result = createConnection(fsDBPath, connectionName, db);
    if(result)
    {
            QSqlQuery* query = new QSqlQuery(db);

            result = query->exec(queryString);
            if(result)
            {
                LOG_TRACE <<("CGracenoteFileHandling : InsertFilesInformation query success") << LOG_ENDL;
                result = true;
            }
            else
            {
                LOG_TRACE <<("CGracenoteFileHandling : InsertFilesInformation query failed !!!") << LOG_ENDL;
                result = false;
            }

            delete query;
                        closeConnection(db); 	 //Added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close(); 	 //Removed by Puneet for fixing GPP crash issue 2013.06.12
        }
    return result;
}

	 //Added by Puneet for fixing GPP crash issue 2013.06.12
bool CGracenoteFileHandling::createConnection(QString dbPath, QString connectionName, QSqlDatabase &dBHandle)
{
    bool retVal = false;
    LOG_TRACE << "CGracenoteFileHandling::createConnection: Sem trying to Acquire with available = " << sem.available() <<  LOG_ENDL;
    sem.acquire(1);
    LOG_TRACE << "CGracenoteFileHandling::createConnection: Sem Acquired " << LOG_ENDL;

    if(QSqlDatabase::contains(connectionName))
    {
        LOG_TRACE << "CGracenoteFileHandling::createConnection: already have this connection name"<< LOG_ENDL;

        dBHandle = QSqlDatabase::database(connectionName);
        dBHandle.setDatabaseName(dbPath);
        if(dBHandle.isOpen())
        {
            LOG_TRACE << "CGracenoteFileHandling::createConnection: already opened successfully"<< LOG_ENDL;
            retVal = true;
        }
        else
        {
            if (!dBHandle.open())
            {
                sem.release(1);
//                m_mutex_db.unlockInline();
                LOG_TRACE << "CGracenoteFileHandling::createConnection: Sem Released on Error Avail = " << sem.available()<< LOG_ENDL;
                LOG_TRACE << "CGracenoteFileHandling::createConnection: not opened successfully"<< LOG_ENDL;
                retVal = false;
            }
            else
            {
                LOG_TRACE << "CGracenoteFileHandling::createConnection: opened successfully"<< LOG_ENDL;
                retVal = true;
            }
        }
    }
    else
    {
        LOG_TRACE << "CGracenoteFileHandling::createConnection: not already have this connection name"<< LOG_ENDL;
        dBHandle = QSqlDatabase::addDatabase("QSQLITE",connectionName);
        dBHandle.setDatabaseName(dbPath);
        if (!dBHandle.open())
        {
            sem.release(1);
            LOG_TRACE << "CGracenoteFileHandling::createConnection: Sem Released on Error, Avail = " << sem.available() << LOG_ENDL;
            LOG_TRACE << "CGracenoteFileHandling::createConnection: not opened successfully"<< LOG_ENDL;
            retVal = false;
        }
        else
        {

            LOG_TRACE << "CGracenoteFileHandling::createConnection: opened successfully"<< LOG_ENDL;
            retVal = true;
        }
    }
    return retVal;
}

	 //Added by Puneet for fixing GPP crash issue 2013.06.12
	 //by aettie for Static Test Defect 201300711
void CGracenoteFileHandling::closeConnection(QSqlDatabase &dBHandle)
{
    dBHandle.close();
    sem.release(1);
    LOG_TRACE << "CGracenoteFileHandling::closeConnection: Sem Released on request of close with avail = " << sem.available() << LOG_ENDL;
}

/*
 *******************************************************************
 * Function	: CGracenoteFileHandling::GetJukeBoxFiles
 *
 * Description 	: Get the list of files present in Jukebox

 * Parameters 	: filesList(OUT) - file paths

 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CGracenoteFileHandling::GetJukeBoxFiles(QStringList& filesList)
{
    return GetListOfFiles(eJukebox, filesList);
}

/*
 *******************************************************************
 * Function	: CGracenoteFileHandling::GetUSBFiles
 *
 * Description 	: Get the list of files present in USB

 * Parameters 	: filesList(OUT) - file paths

 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CGracenoteFileHandling::GetUSBFiles(QStringList& filesList, QString mountPath)
{
    return GetListOfFiles(eUsb, filesList, mountPath);
}

/*
 *******************************************************************
 * Function	: CGracenoteFileHandling::GetListOfFiles
 *
 * Description 	: Get the list of files present in specified volume type

 * Parameters 	: type(IN) - volume type
 *                        filesList(OUT) - file paths

 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CGracenoteFileHandling::GetListOfFiles(EFileType type, QStringList &filesList, QString mountPath)
{
    LOG_TRACE <<("CGracenoteFileHandling : GetListOfFiles Start") << LOG_ENDL;
    bool result = false;

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/FilesDB"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("FilesDatabase");

    result = createConnection(fsDBPath, connectionName, db);
    if(result)
    {
            QSqlQuery* query = new QSqlQuery(db);
            QString str;

            if(type == eJukebox)
            {
                str.append(QString("SELECT * FROM Files WHERE FilePath like '/usr_data/media/%'"));
                LOG_TRACE <<("CGracenoteFileHandling : GetListOfFiles SELECT * FROM Files WHERE FilePath like /usr_data/media") << LOG_ENDL;
            }
            else if(type == eUsb)
            {
                QString queryStr = "SELECT * FROM Files WHERE FilePath like '%1'";
                QString mountPathForQuery = mountPath;
                mountPathForQuery.append("/%");
                queryStr = queryStr.arg(mountPathForQuery);

                str.append(queryStr);
                //LOG_TRACE <<("CGracenoteFileHandling : GetListOfFiles SELECT * FROM Files WHERE FilePath like /media") << LOG_ENDL;
            }
            result = query->exec(str);
            if(result)
            {
                LOG_TRACE <<("CGracenoteFileHandling : GetListOfFiles query success") << LOG_ENDL;
            }
            else
            {
                LOG_TRACE <<("CGracenoteFileHandling : GetListOfFiles query failed !!!") << LOG_ENDL;
                filesList.clear();
                delete query;
                db.close();
                return false;
            }

            //Clear any old list
            filesList.clear();

            QString entry;
            while(query->next())
            {
                entry.clear();

                // file path
                entry.append(query->value(0).toString());
                filesList.append(entry);
            }

            LOG_TRACE <<("CGracenoteFileHandling : GetListOfFiles filesList count") << LOG_ENDL;
            LOG_TRACE << filesList.count() << LOG_ENDL;

            LOG_TRACE <<("CGracenoteFileHandling : GetListOfFiles End") << LOG_ENDL;

            delete query;
                        closeConnection(db); 	 //Added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close(); 	 //Removed by Puneet for fixing GPP crash issue 2013.06.12
        }
    return result;
}

/*
 *******************************************************************
 * Function	: CGracenoteFileHandling::DeleteJukeboxFiles
 *
 * Description 	: Delete files present in Jukebox from the database

 * Parameters 	: None

 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CGracenoteFileHandling::DeleteJukeboxFiles()
{
    bool result = false;

    QSqlDatabase db;
    QString fsDBPath; 
    fsDBPath.append("/app/data/gracenote/FilesDB"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("FilesDatabase");

    result = createConnection(fsDBPath, connectionName, db);
    if(result)
    {
            QSqlQuery* query = new QSqlQuery(db);
            QString str;
            str.append(QString("Delete FROM Files WHERE FilePath like '/usr_data/media/%'"));

            bool result = query->exec(str);
            qDebug() << query->lastError().text();
            if(result)
            {
                LOG_TRACE <<("CGracenoteFileHandling : DeleteJukeboxFiles query success") << LOG_ENDL;
            }
            else
            {
                LOG_TRACE <<("CGracenoteFileHandling : DeleteJukeboxFiles query failed !!!") << LOG_ENDL;
            }

            delete query;
                        closeConnection(db); 	 //Added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close(); 	 //Modified by Puneet for fixing GPP crash issue 2013.06.12
        }
    return result;
}

/*
 *******************************************************************
 * Function	: CGracenoteFileHandling::DeleteUSBFiles
 *
 * Description 	: Delete files present in USB from the database

 * Parameters 	: None

 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CGracenoteFileHandling::DeleteUSBFiles(QString mountPath)
{
    bool result = false;

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/FilesDB"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("FilesDatabase");

    result = createConnection(fsDBPath, connectionName, db);
    if(result)
    {
            QSqlQuery* query = new QSqlQuery(db);

            QString modifiedPath = mountPath;
            modifiedPath.append("/%");

            query->prepare("Delete FROM Files WHERE FilePath like ?");
            query->addBindValue(modifiedPath);

            result = query->exec();
            qDebug() << query->lastError().text();
            if(result)
            {
                LOG_TRACE <<("CGracenoteFileHandling : DeleteUSBFiles query success") << LOG_ENDL;
            }
            else
            {
                LOG_TRACE <<("CGracenoteFileHandling : DeleteUSBFiles query failed !!!") << LOG_ENDL;
            }

            delete query;
                                   closeConnection(db); 	 //Added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close(); 	 //Modified by Puneet for fixing GPP crash issue 2013.06.12
        }

    return result;
}

/*
 *******************************************************************
 * Function	: CGracenoteFileHandling::DeleteListOfFiles
 *
 * Description 	: Delete specifed files from the files database

 * Parameters 	: deletedFileList - list of files to be deleted

 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CGracenoteFileHandling::DeleteListOfFiles(QStringList deletedFileList)
{
    bool result = false;

    QString queryString = QString("DELETE FROM Files WHERE FilePath IN (");

    for(int index = 0; index < deletedFileList.count(); index++)
    {
        queryString.append(QString("\"%1\"").arg(deletedFileList.at(index)));

        if(index < (deletedFileList.count() - 1))
        {
            queryString.append(",");
        }

    }

    queryString.append((")"));

    qDebug() << "DeleteListOfFiles" << queryString;

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/FilesDB"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("FilesDatabase");

    result = createConnection(fsDBPath, connectionName, db);
    if(result)
    {
        QSqlQuery* query = new QSqlQuery(db);
         result = query->exec(queryString);

        if(result)
         {
            LOG_TRACE <<("CGracenoteFileHandling : DeleteListOfFiles query success") << LOG_ENDL;
            result = true;
        }
        else
        {
            LOG_TRACE <<("CGracenoteFileHandling : DeleteListOfFiles query failed !!!") << LOG_ENDL;
            result = false;
        }

        delete query;
                    closeConnection(db); 	 //Added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close(); 	 //Modified by Puneet for fixing GPP crash issue 2013.06.12
    }
    return result;
}

// End of file
