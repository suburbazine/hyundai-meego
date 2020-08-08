
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
 *  MODULE SUMMARY :
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

#include "DHAVN_NormalizedDataUtil.h"
#include "QString"
#include "QSqlError"



/*
 *******************************************************************
 * Function		: CNormalizedData::CNormalizedData
 *
 * Description 	: Constructor for CNormalizedData

 * Parameters 	: parent (IN) -

 * Returns 		: None
 *
 *******************************************************************
 */
CNormalizedData::CNormalizedData(QObject* parent)
{
    Q_UNUSED(parent);
     LOG_INIT_V2("Media_GracenotePluginProcess");
     m_semaphore = new QSemaphore(1); //added by Puneet for fixing GPP crash issue 2013.06.12
}

/*
 *******************************************************************
 * Function		: CNormalizedData::Init
 *
 * Description 	: Second phase construction

 * Parameters 	:

 * Returns 		: Bool
 *
 *******************************************************************
 */
bool CNormalizedData::Init()
{
    bool initStatus = false;
    return initStatus;
}

/*
 *******************************************************************
 * Function		: CNormalizedData::~CNormalizedData
 *
 * Description 	: Destructor for CNormalizedData

 * Parameters 	: parent (IN) -

 * Returns 		: None
 *
 *******************************************************************
 */
CNormalizedData::~CNormalizedData()
{
}

/*
 *******************************************************************
 * Function		: CNormalizedData::CreateSQLiteDB
 *
 * Description 	: Destructor for CNormalizedData

 * Parameters 	: parent (IN) -

 * Returns 		: None
 *
 *******************************************************************
 */
bool CNormalizedData::CreateSQLiteDB(int CID)
{
    //Create SQLite DB for the new CID
	//added by Puneet for fixing GPP crash issue 2013.06.12
    LOG_TRACE << "CNormalizedData: CreateSQLiteDB Sem trying to Acquire with available = " << m_semaphore->available() << LOG_ENDL;
    m_semaphore->acquire(1);
    QString str;
    str.append("CID_");
    str.append(QString("%1").arg(CID));
    bool result = false;
    LOG_TRACE << "CNormalizedData : CreateSQLiteDB" << str << LOG_ENDL;

    QString fsdbName;
    fsdbName.append("/app/data/gracenote/"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change
    fsdbName.append(str);

     LOG_TRACE << "CNormalizedData : CreateSQLiteDB" << fsdbName << LOG_ENDL;

    QString tmpfsdbName;
    tmpfsdbName.append("/dev/shm/");
    tmpfsdbName.append(str);

      LOG_TRACE << "CNormalizedData : CreateSQLiteDB" << tmpfsdbName << LOG_ENDL;

    if (QFile::exists(fsdbName))
    {
        if(QFile::exists(tmpfsdbName))
        {
            QFile::remove(tmpfsdbName);
        }
        QFile::copy(fsdbName, tmpfsdbName);
        sync();
    }
    else
    {
        QSqlDatabase sqlitedb = QSqlDatabase::addDatabase("QSQLITE",str);
        if(sqlitedb.isValid())
        {
            sqlitedb.setDatabaseName(tmpfsdbName);
            if(sqlitedb.open())
            {
                QSqlQuery* query = new QSqlQuery(sqlitedb);
                QString queryStr;
                queryStr.append(QString("CREATE TABLE %1").arg(str));
                queryStr.append( "(T_Title varchar(256), "
                                 "T_Album varchar(256), "
                                 "T_Artist varchar(256), "
                                 "T_Genre varchar(256), "
                                 "T_Composer varchar(256),"
                                 "T_FilePath varchar(256)) ");
                LOG_TRACE <<("CNormalizedData: CreateSQLiteDB query") << queryStr << LOG_ENDL;

                result = query->exec(queryStr);
                if(result)
                {
                    LOG_TRACE <<("CNormalizedData: CreateSQLiteDB success") << LOG_ENDL;
                }
                else
                {
                    LOG_TRACE <<("CNormalizedData: CreateSQLiteDB failed !!!") << LOG_ENDL;
                }
                delete query;
                sqlitedb.close();
				//Removed by Puneet for fixing GPP crash issue 2013.06.12
//                return result;  //added by aettie 2013.02.28 for static analysis 18439 
            }
        }
    }
//added and Modified by Puneet for fixing GPP crash issue 2013.06.12
    m_semaphore->release(1);
    LOG_TRACE << "CNormalizedData: CreateSQLiteDB Sem released now available = " << m_semaphore->available() << LOG_ENDL;
    return result;  //added by aettie 2013.02.28 for static analysis 18439
}

/*
 *******************************************************************
 * Function		: CNormalizedData::DeleteSQLiteDB
 *
 * Description 	: Destructor for CNormalizedData

 * Parameters 	: parent (IN) -

 * Returns 		: None
 *
 *******************************************************************
 */
bool CNormalizedData::DeleteSQLiteDB(int CID)
{
    QString str;
    str.append("CID_");
    str.append(QString("%1").arg(CID));

    QSqlDatabase sqlitedb;
    if(createConnection(str, sqlitedb))
    {
        QSqlQuery* query = new QSqlQuery(sqlitedb);
        QString queryStr;
        queryStr.append(QString("DROP DATABASE %1").arg(str));
        bool result = query->exec(queryStr);
        if(result)
        {
            LOG_TRACE <<("CNormalizedData: DeleteSQLiteDB success") << LOG_ENDL;
        }
        else
        {
            LOG_TRACE <<("CNormalizedData: DeleteSQLiteDB failed !!!") << LOG_ENDL;
        }
        delete query;
        sqlitedb.close();
        m_semaphore->release(1); //added by Puneet for fixing GPP crash issue 2013.06.12
        return result;      //added by aettie 2013.02.28 for static analysis 18440
    }
    return false;       //added by aettie 2013.02.28 for static analysis 18440
}

/*
 *******************************************************************
 * Function		: CNormalizedData::PopulateDB
 *
 * Description 	: Destructor for CNormalizedData

 * Parameters 	: parent (IN) -

 * Returns 		: None
 *
 *******************************************************************
 */
bool CNormalizedData::PopulateDB(const TrackerMusicData& musicData, int CID)
{
    QString str;
    str.append("CID_");
    str.append(QString("%1").arg(CID));

     LOG_TRACE <<("CNormalizedData: PopulateDB DBName ") << str << LOG_ENDL;

    QSqlQuery* query;
    QString queryStr;

    QSqlDatabase sqlitedb;
    if(createConnection(str, sqlitedb))
    {
        query = new QSqlQuery(sqlitedb);

        QString album_title;
        album_title.append(musicData.track_title);

        QString album;
        album.append(musicData.album);

        QString album_artist;
        album_artist.append(musicData.track_artist);

        QString genre;
        genre.append(musicData.genre);

        QString composer;
        composer.append(musicData.composer);

        QString path;
        path.append(musicData.path);

        if((album_title).contains("\""))
            {
                album_title = album_title.replace("\"","\"\"");
            }
        if((album).contains("\""))
            {
                album = album.replace("\"","\"\"");
            }
        if((album_artist).contains("\""))
            {
                album_artist.replace("\"","\"\"");
            }
        if((genre).contains("\""))
            {
                (genre).replace("\"","\"\"");
            }
        if((composer).contains("\""))
            {
                composer = composer.replace("\"","\"\"");
            }
        if((path).contains("\""))
            {
                path = path.replace("\"","\"\"");
            }

            queryStr.append(QString("Insert into %1 values (\"%2\", \"%3\", \"%4\", \"%5\", \"%6\", \"%7\")").arg(str, album_title, album, album_artist, genre, composer, path));

            LOG_TRACE << "CNormalizedData::PopulateDB: query" << queryStr << LOG_ENDL;

           /* if(index < (phoneticData.count() - 1))
            {
                queryStr.append(" UNION ALL");
            }*/

            bool result = query->exec(queryStr);
            if(result)
            {
                    LOG_TRACE << "CNormalizedData::PopulateDB passed" << LOG_ENDL;
                    result = true;
            }
            else
            {
                    LOG_TRACE << "CNormalizedData::PopulateDB failed" << LOG_ENDL;
                    result = false;
            }
            delete query;
            sqlitedb.close();
            m_semaphore->release(1); //added by Puneet for fixing GPP crash issue 2013.06.12
            return result; //added by aettie 2013.02.28 for static analysis 18438
        }
    return false; //added by aettie 2013.02.28 for static analysis 18438
}

/*
 *******************************************************************
 * Function		: CNormalizedData::UpdateDB
 *
 * Description 	: Destructor for CNormalizedData

 * Parameters 	: parent (IN) -

 * Returns 		: None
 *
 *******************************************************************
 */
bool CNormalizedData::UpdateRecord(int CID, QString value, QString filePath)
{
    LOG_INFO << "CNormalizedData::UpdateRecord start " << CID << LOG_ENDL;

    bool result = false;

    QString str;
    str.append("CID_");
    str.append(QString("%1").arg(CID));

   QSqlDatabase db;
    if(createConnection(str, db))
    {
        LOG_TRACE << "CNormalizedData::UpdateRecord db opened" << LOG_ENDL;
        QSqlQuery* query = new QSqlQuery(db);

        QString queryString;
        queryString.append(QString("Update %1 set T_FilePath=\"%2\" where T_Title=\"%3\"").arg(str, filePath, value));

        result = query->exec();
        if(result)
             LOG_TRACE << "CNormalizedData::UpdateRecord successful" << LOG_ENDL;

        delete query;
        db.close();
        m_semaphore->release(1); //added by Puneet for fixing GPP crash issue 2013.06.12
    }
    return result;
}

/*
 *******************************************************************
 * Function		: CNormalizedData::DeleteRecords
 *
 * Description 	: Destructor for CNormalizedData

 * Parameters 	: parent (IN) -

 * Returns 		: None
 *
 *******************************************************************
 */
bool CNormalizedData::DeleteRecords(int CID, QStringList filePaths)
{
    LOG_INFO << "CNormalizedData::DeleteRecords start " << CID << LOG_ENDL;

    bool result = false;

    QString str;
    str.append("CID_");
    str.append(QString("%1").arg(CID));

    QSqlDatabase db;
    if(createConnection(str, db))
    {
        LOG_TRACE << "CNormalizedData::DeleteRecords db opened" << LOG_ENDL;
        QSqlQuery* query = new QSqlQuery(db);

        QString queryString;
        queryString.append(QString("Delete from %1 where T_FilePath in (").arg(str));


        for(int index = 0; index < filePaths.count(); index++)
        {
           queryString.append(QString("\"%1\"").arg(filePaths.at(index)));

            if(index < (filePaths.count() - 1))
            {
                queryString.append(",");
            }
        }

        queryString.append((")"));

        LOG_TRACE << queryString << LOG_ENDL;

        bool queryresult = query->exec(queryString);
        if(queryresult)
        {
                LOG_TRACE << "CNormalizedData::DeleteRecords success" << LOG_ENDL;
        }
        else
        {
                LOG_TRACE << "CNormalizedData::DeleteRecords failed" << LOG_ENDL;
        }
        delete query;
        db.close();
        m_semaphore->release(1); //added by Puneet for fixing GPP crash issue 2013.06.12
    }

    LOG_TRACE << "CNormalizedData::DeleteRecords END" << LOG_ENDL;
    return result;
}

/*
 *******************************************************************
 * Function		: CNormalizedData::CheckIfEntryExists
 *
 * Description 	: Destructor for CNormalizedData

 * Parameters 	: parent (IN) -

 * Returns 		: None
 *
 *******************************************************************
 */
bool CNormalizedData::CheckIfEntryExists(int CID, QString filepath)
{
     LOG_TRACE << "CNormalizedData::CheckIfEntryExists  start"<< LOG_ENDL;

    QString str;
    str.append("CID_");
    str.append(QString("%1").arg(CID));

    //QSqlDatabase db = QSqlDatabase::database(str);
    bool result = false;

    QSqlDatabase db;
    if(createConnection(str, db))
    {
        LOG_TRACE << "CNormalizedData::CheckIfEntryExists db open " << LOG_ENDL;

        QSqlQuery* query = new QSqlQuery(db);

        QString str;
        str.append(QString("SELECT EXISTS(SELECT 1 FROM %1 WHERE filepath=\"%2\"))").arg(str, filepath));
        LOG_TRACE << str << LOG_ENDL;
        bool queryresult = query->exec(str);
        if(queryresult)
        {
                LOG_TRACE << "CNormalizedData::CheckIfEntryExists success" << LOG_ENDL;

                QString entry;
                while(query->next())
                {
                        entry.clear();
                        // get count of entries
                        entry.append(query->value(0).toString());
                }

                int count = entry.toInt();
                LOG_TRACE << "CNormalizedData::CheckIfEntryExists Count of entries" << count <<  LOG_ENDL;

                if(count != 0)
                {
                result = true;
                }
        }
        else
        {
                LOG_TRACE << "CNormalizedData::CheckIfEntryExists failed" << LOG_ENDL;
        }
        delete query;
        db.close();
        m_semaphore->release(1); //added by Puneet for fixing GPP crash issue 2013.06.12
    }

    LOG_TRACE << "CNormalizedData::CheckIfEntryExists END" << LOG_ENDL;
    return result;
}


/*
 *******************************************************************
 * Function		: CNormalizedData::CreateSQLiteDB
 *
 * Description 	: Destructor for CNormalizedData

 * Parameters 	: parent (IN) -

 * Returns 		: None
 *
 *******************************************************************
 */
void CNormalizedData::CopySQLiteDBtoFS(int CID)
{
    //Create SQLite DB for the new CID
    QString str;
    str.append("CID_");
    str.append(QString("%1").arg(CID));

    QString tmpfsdbName;
    tmpfsdbName.append("/dev/shm/");
    tmpfsdbName.append(str);

    QString fsdbName;
    fsdbName.append("/app/data/gracenote/"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change
    fsdbName.append(str);

    if (QFile::exists(tmpfsdbName))
    {
        if(QFile::exists(fsdbName))
        {
            //Remove existing file
            QFile::remove(fsdbName);
        }
        QFile::copy(tmpfsdbName, fsdbName);
        // Make sure that file copy is complete
        sync();
    }
}



/*
 *******************************************************************
 * Function	: createConnection
 *
 * Description 	: Creates Database connection
 *
 * Parameters 	: QString - Database path, QString - connectionName
 *                        QSqlDatabase - Database handle
 *
 * Returns 	: bool - true if connection to DB can be established
 *
 *******************************************************************
 */
bool CNormalizedData::createConnection(QString connectionName, QSqlDatabase &dBHandle)
{
   bool retVal = false;
   QString dbPath;
   dbPath.append("/dev/shm/");
   dbPath.append(connectionName);
   //added and modified by Puneet for fixing GPP crash issue 2013.06.12
   LOG_TRACE << "CNormalizedData::createConnection: Sem trying to Acquire with available = "<< m_semaphore->available() << LOG_ENDL;
   m_semaphore->acquire(1);
    if(QSqlDatabase::contains(connectionName))
    {
        LOG_TRACE << "CNormalizedData::createConnection: already have this connection name"<< LOG_ENDL;
        dBHandle = QSqlDatabase::database(connectionName);
        dBHandle.setDatabaseName(dbPath);
        if(dBHandle.isOpen())
        {
            LOG_TRACE << "CNormalizedData::createConnection: already opened successfully"<< LOG_ENDL;
            retVal = true;
        }
        else
        {
            if (!dBHandle.open())
            {
                LOG_TRACE << "CNormalizedData::createConnection: not opened successfully"<< LOG_ENDL;
                m_semaphore->release(1);
                retVal = false;
            }
            else
            {
                LOG_TRACE << "CNormalizedData::createConnection: opened successfully"<< LOG_ENDL;
                retVal = true;
            }
        }
    }
    else
    {
        LOG_TRACE << "CNormalizedData::createConnection: not already have this connection name"<< LOG_ENDL;
        dBHandle = QSqlDatabase::addDatabase("QSQLITE",connectionName);
        dBHandle.setDatabaseName(dbPath);
        if (!dBHandle.open())
        {
            LOG_TRACE << "CNormalizedData::createConnection: not opened successfully"<< LOG_ENDL;
            m_semaphore->release(1);
            retVal = false;
        }
        else
        {
            LOG_TRACE << "CNormalizedData::createConnection: opened successfully"<< LOG_ENDL;
            retVal = true;
        }
    }
    return retVal;
}
