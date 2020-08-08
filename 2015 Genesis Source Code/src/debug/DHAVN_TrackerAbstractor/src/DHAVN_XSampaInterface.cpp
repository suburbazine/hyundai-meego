/*
******************************************************************
*        COPYRIGHT (C) Teleca AB      *
*----------------------------------------------------------------*
* MODULE     :  DHAVN_TrackerAbstractor                          *
*                                                                *
* PROGRAMMER :  Vishal Jain                                      *
* DATE       :  1 March 2012                                     *
* VERSION    :  1.0                                              *
*
*----------------------------------------------------------------*
*                                                                *
* MODULE SUMMARY : This file defines APIs which interact with    *
*                  GracenoteProcess for inserting XSampa data    *
*                  and with VR module for retrieving XSampa data *
*----------------------------------------------------------------*
*
* MODIFICATION RECORDS :                                         *
*
* VERSION   DATE                     PROGRAMMER              COMMENT
******************************************************************
* 1.0       1 March 2012            Vishal Jain           Created
******************************************************************
*/

#include "DHAVN_XSampaInterface.h"

#include <QtSql>
#include <QVector>
#include <QStringList>

/*
 *******************************************************************
 * Function	: CXSampaInterface::CXSampaInterface
 *
 * Description 	: Constructor for CXSampaInterface

 * Parameters 	: parent (IN) - pointer to parent object

 * Returns 	: None
 *
 *******************************************************************
 */
CXSampaInterface::CXSampaInterface(QObject *parent)
{
    Q_UNUSED(parent);
    LOG_INIT_V2("Media_TrackerAbstractor");
    m_IsXSampaDBCreated = false;
}


/*
 *******************************************************************
 * Function	: CXSampaInterface::~CXSampaInterface
 *
 * Description 	: Destructor for CXSampaInterface

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
CXSampaInterface::~CXSampaInterface()
{
}


/*
 *******************************************************************
 * Function	: CXSampaInterface::Init
 *
 * Description 	: Initialization function

 * Parameters 	: None

 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CXSampaInterface::Init()
{
    LOG_TRACE << "CXSampaInterface: Init Start" << LOG_ENDL;

    bool result =  false;
    result = CreateXSampaDatabase();

    return result;
}


/*
 *******************************************************************
 * Function	: CXSampaInterface::CreateXSampaDatabase
 *
 * Description 	: Create SqlLite database for XSampa

 * Parameters 	: None

 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CXSampaInterface::CreateXSampaDatabase()
{
    LOG_TRACE << "CXSampaInterface: CreateXSampaDatabase Start" << LOG_ENDL;

    QSqlDatabase database;
    QString tmpfsDBPath;
    tmpfsDBPath.append("/dev/shm/XSampaDB");

    QString connectionName;
    connectionName.append("XSampa");

    bool result = createConnection(tmpfsDBPath, connectionName, database);
    if(result)
    {
        QStringList tableNames = database.tables();
        if(tableNames.contains("XSampaTable"))
        {
            LOG_TRACE << "CXSampaInterface: XSampaTable Table is already created" << LOG_ENDL;
        }
        else
        {
            LOG_TRACE << "CXSampaInterface: XSampaTable Table is being created" << LOG_ENDL;
            CreateXSampaTable(database);
        }

        database.close();
        return true;
    }
    else
        return false;
}


/*
 *******************************************************************
 * Function	: CXSampaInterface::CreateXSampaTable
 *
 * Description 	: Create Table in XSampa database

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
bool CXSampaInterface::CreateXSampaTable(QSqlDatabase db)
{
    bool result = false;

    QSqlQuery* query = new QSqlQuery(db);
    QString queryString;

    queryString.append("CREATE TABLE XSampaTable (Type VARCHAR(10), ID3Name VARCHAR(256), Name VARCHAR(256), NameAltPhrases VARCHAR(256), Lang VARCHAR(20), XSampa VARCHAR(256), CID_1 int, CID_2 int, CID_3 int, CID_4 int, CID_5 int, CID_6 int, CID_7 int, CID_8 int, CID_9 int, CID_10 int, CID_11 int, CID_12 int)");

    query->prepare(queryString);
    result = exec(query);

    delete query;
    return result;
}

bool CXSampaInterface::exec(QSqlQuery* query)
{
    bool result = query->exec();
    LOG_TRACE << Q_FUNC_INFO << " query: " << query->lastQuery() << LOG_ENDL;
    if (query->lastError().isValid()) {
        LOG_TRACE << Q_FUNC_INFO << " last error: " << query->lastError().text() << LOG_ENDL;
    }
    return result;
}

/*
 *******************************************************************
 * Function	: CXSampaInterface::CheckIfPhoneticDataExists
 *
 * Description 	: Check if Phonetic (XSampa) data for a specific type
 *                and name like album, artist or genre is present in
 *                database
 * Parameters 	: type - album, artist or genre
 *                name - specific name like Konvicted, Akon, Hip-Hop etc
 *
 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CXSampaInterface::CheckIfPhoneticDataExists(QString type, QString name)
{
    if(!m_IsXSampaDBCreated)
    {
        Init();
        m_IsXSampaDBCreated = true;
    }

    bool result = false;

    QSqlDatabase db;
    QString tmpfsDBPath;
    tmpfsDBPath.append("/dev/shm/XSampaDB");

    QString connectionName;
    connectionName.append("XSampa");

    if(createConnection(tmpfsDBPath, connectionName, db))
    {
        QSqlQuery* query = new QSqlQuery(db);

        QString str;
        str.append(QString("SELECT EXISTS(SELECT 1 FROM XSampaTable WHERE Type=\"%1\" AND Name=\"%2\" LIMIT 1)").arg(type, name));
        query->prepare(str);
        bool queryresult = exec(query);
        if(queryresult)
        {		
            QString entry;
            while(query->next())
            {
                entry.clear();
                // get count of entries
                entry.append(query->value(0).toString());
            }

            int count = entry.toInt();

            if(count != 0)
            {
                result = true;
            }
        }

        delete query;
        db.close();
    }

    return result;
}

/*
 *******************************************************************
 * Function	: CXSampaInterface::UpdateCollectionIDInfo
 *
 * Description 	: Update CID information into the XSampa DB.
 * Parameters 	: CID - Collection ID
 *                type - album, artist or genre
 *                name - specific name like Konvicted, Akon, Hip-Hop etc
 *
 *******************************************************************
 */
void CXSampaInterface::UpdateCollectionIDInfo(int CID, QString type, QString name)
{
    if(!m_IsXSampaDBCreated)
    {
        Init();
        m_IsXSampaDBCreated = true;
    }

    bool updateResult =  true;

    QSqlDatabase db;
    QString tmpfsDBPath;
    tmpfsDBPath.append("/dev/shm/XSampaDB");

    QString connectionName;
    connectionName.append("XSampa");

    bool result = createConnection(tmpfsDBPath, connectionName, db);
    if(result)
    {
        QSqlQuery* query = new QSqlQuery(db);
        //Update CID information into the XSampa table for the data which is already existing in the DB
        QString CID_Column;
        CID_Column.clear();
        CID_Column.append("CID_");
        CID_Column.append(QString("%1").arg(CID));

        QString str;
        str.clear();
        str.append(QString("UPDATE  XSampaTable set \"%3\"=1 WHERE Type=\"%1\" AND Name=\"%2\"").arg(type, name, CID_Column));

        query->prepare(str);
        updateResult = exec(query);

        db.close();
        delete query;
    }
}


/*
 *******************************************************************
 * Function	: CXSampaInterface::UpdatePhoneticData
 *
 * Description 	: Insert phonetic data in the XSampa database
 *
 * Parameters 	: phoneticData - List of all phonetic data
 *
 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CXSampaInterface::UpdatePhoneticData(QVector<QStringList> phoneticData, int CID)
{
    if(!m_IsXSampaDBCreated)
    {
        Init();
        m_IsXSampaDBCreated = true;
    }

    //Update CID information into the XSampa table
    QString CID_Column;
    CID_Column.append("CID_");
    CID_Column.append(QString("%1").arg(CID));

    bool result = false;
    QString type, id3Name, name, nameAltPhrases, lang, xsampa;

    QList<QString> CIDInfo;
    for(int i=0; i<12; i++)
    {
        CIDInfo.append("0");
    }

    CIDInfo.replace(CID-1, "1");    

    QString queryString = QString("INSERT INTO XSampaTable (Type, ID3Name, Name, NameAltPhrases, Lang, XSampa, CID_1, CID_2, CID_3, CID_4, CID_5, CID_6, CID_7, CID_8, CID_9, CID_10, CID_11, CID_12)");

    for(int index = 0; index < phoneticData.count(); index++)
    {
        QString subQuery;
        QStringList data = phoneticData.at(index);

        if(data.count() != 6)
        {
            LOG_TRACE << "CXSampaInterface: UpdatePhoneticData: Insufficient data to populate" << LOG_ENDL;
            continue;
        }
        // Modify strings to take care of double quote "
        type            = data.at(0);
        id3Name         = data.at(1);
        name            = data.at(2);
        nameAltPhrases  = data.at(3);
        lang            = data.at(4);
        xsampa          = data.at(5);

        if(id3Name.contains("\""))
        {
            id3Name = id3Name.replace("\"","\"\"");
        }
        if(name.contains("\""))
        {
            name = name.replace("\"","\"\"");
        }
        if(nameAltPhrases.contains("\""))
        {
            nameAltPhrases.replace("\"","\"\"");
        }
        if(xsampa.contains("\""))
        {
            xsampa.replace("\"","\"\"");
        }

        subQuery.append(QString(" SELECT \"%1\", \"%2\", \"%3\", \"%4\", \"%5\", \"%6\", \"%7\", \"%8\", \"%9\",").arg(type, id3Name, name, nameAltPhrases, lang, xsampa, CIDInfo.at(0), CIDInfo.at(1), CIDInfo.at(2)));

        subQuery.append(QString("\"%1\", \"%2\", \"%3\", \"%4\", \"%5\", \"%6\", \"%7\", \"%8\", \"%9\"").arg(CIDInfo.at(3), CIDInfo.at(4), CIDInfo.at(5), CIDInfo.at(6), CIDInfo.at(7), CIDInfo.at(8), CIDInfo.at(9), CIDInfo.at(10), CIDInfo.at(11)));

        LOG_TRACE << "CXSampaInterface: UpdatePhoneticData: subquery" << subQuery << LOG_ENDL;

        queryString.append(subQuery);
        
        if(index < (phoneticData.count() - 1))
        {
            queryString.append(" UNION ALL");
        }
    }

    if(phoneticData.count() > 0)
    {
        QSqlDatabase db;

        QString tmpfsDBPath;
        tmpfsDBPath.append("/dev/shm/XSampaDB");

        QString connectionName;
        connectionName.append("XSampa");

        if(createConnection(tmpfsDBPath, connectionName, db))
        {
            QSqlQuery* query = new QSqlQuery(db);
            query->prepare(queryString);
            if(exec(query))
            {
                result = true;
            }
            else
            {
                result = false;
            }

            delete query;
            db.close();
        }
    }

    return result;
}

/*
 *******************************************************************
 * Function	: CXSampaInterface::RetrieveCIDforConnectedDevices
 *
 * Description 	: Retrieve CID for all the connected device from CID_Volume DB
 *
 * Returns 	: QList - List of CID's for the connected devices
 *
 *******************************************************************
 */
QList<int> CXSampaInterface::RetrieveCIDforConnectedDevices()
{
    QList<int> CIDInfo;
    CIDInfo.clear();
    for(int i=0; i<12; i++)
    {
        CIDInfo.append(0);
    }

    QSqlDatabase db_cidVolume;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/CID_volume"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change

    QString connectionName;
    connectionName.append("CID");

    bool result = createConnection(fsDBPath, connectionName, db_cidVolume);
    if(result)
    {
        LOG_TRACE << "CXSampaInterface: RetrieveCIDforConnectedDevices db opened"  << LOG_ENDL;

        QSqlQuery* query_cidVolume = new QSqlQuery(db_cidVolume);
        query_cidVolume->prepare("SELECT CID FROM CollectionID_Data WHERE Connect = 1");
        bool query_status = exec(query_cidVolume);
        if(query_status)
        {
             while (query_cidVolume->next())
            {
                int CID = query_cidVolume->value(0).toInt();
                CIDInfo.replace(CID-1, 1);
                LOG_TRACE << "CXSampaInterface: RetrieveCIDforConnectedDevices connected CID"  << CID << LOG_ENDL;
            } //END while (query_cidVolume->next())
        }//END  if(query_status)

        delete query_cidVolume;
        db_cidVolume.close();
    }
    return CIDInfo;
}

/*
 *******************************************************************
 * Function	: CXSampaInterface::RetrievePhoneticData
 *
 * Description 	: Retrieve phonetic data for a specified type and name
 *
 * Parameters 	: type - type
 *                name - name of artist, album or genre
 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CXSampaInterface::RetrievePhoneticData(QString type, QString name)
{
    if(!m_IsXSampaDBCreated)
    {
        Init();
        m_IsXSampaDBCreated = true;
    }

    QVector<QStringList> data;
    bool returnStatus = false;
    QList<int> CIDInfo;
    CIDInfo = RetrieveCIDforConnectedDevices();

    QString whereCondition;
    whereCondition.clear();

    int count = 0;

    for(int i=0; i<CIDInfo.count(); i++)
    {
        if(CIDInfo.at(i) == 1)
        {
            if(count >=1)
                 whereCondition.append(" OR ");

            int CID = i+1;
            whereCondition.append("CID_");
            whereCondition.append(QString("%1").arg(CID));
            whereCondition.append("=1");
            count ++;
        }
    }

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/XSampaDB"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change
    
    QString connectionName;
    connectionName.append("XSampa");

    bool result = createConnection(fsDBPath, connectionName, db);
    if(result)
    {
        QSqlQuery* query = new QSqlQuery(db);

        QString str;
        str.append(QString("SELECT Lang, XSampa FROM XSampaTable WHERE Type=\"%1\" AND ID3Name=\"%2\" AND ").arg(type, name));
        str.append(whereCondition);

        query->prepare(str);
        bool result = exec(query);
        if(result)
        {
            QStringList entry;
            while(query->next())
            {
                entry.clear();

                //Language
                entry.append(query->value(0).toString());
                // XSampa
                entry.append(query->value(1).toString());
                data.append(entry);
            }
            emit DataFetched(data, eRetrieveXSampaData_Id);
            returnStatus = true;
        }
        delete query;
        db.close();
    }
    return returnStatus;
}

/*
 *******************************************************************
 * Function	: CXSampaInterface::RetrieveAllPhoneticData
 *
 * Description 	: Retrieve all phonetic data present in database
 *
 * Parameters 	: None
 *
 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CXSampaInterface::RetrieveAllPhoneticData()
{
    if(!m_IsXSampaDBCreated)
    {
        Init();
        m_IsXSampaDBCreated = true;
    }

    QVector<QStringList> data;
    bool returnStatus = false;
    QList<int> CIDInfo;
    CIDInfo = RetrieveCIDforConnectedDevices();

    QString whereCondition;
    whereCondition.clear();

    int count = 0;

    for(int i=0; i<CIDInfo.count(); i++)
    {
        if(CIDInfo.at(i) == 1)
        {
            if(count >=1)
                 whereCondition.append(" OR ");

            int CID = i+1;
            whereCondition.append("CID_");
            whereCondition.append(QString("%1").arg(CID));
            whereCondition.append("=1");
            count ++;
        }

    }

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/XSampaDB"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change
    
    QString connectionName;
    connectionName.append("XSampa");

    if(createConnection(fsDBPath, connectionName, db))
    {
        QSqlQuery* query = new QSqlQuery(db);

        QString str;
        str.append(QString("SELECT Type, ID3Name, Name, NameAltPhrases, Lang, XSampa FROM XSampaTable WHERE "));
        str.append(whereCondition);

        query->prepare(str);
        if(exec(query))
        {

            data.clear();
            QStringList entry;

            while(query->next())
            {
                entry.clear();
                // type
                entry.append(query->value(0).toString());
                // LOG_TRACE << "CXSampaInterface: RetrieveAllPhoneticData type"  << query->value(0).toString() << LOG_ENDL;
                // id3 name
                entry.append(query->value(1).toString());
                // LOG_TRACE << "CXSampaInterface: RetrieveAllPhoneticData id3 name"  << query->value(1).toString() << LOG_ENDL;
                // name
                entry.append(query->value(2).toString());
                // LOG_TRACE << "CXSampaInterface: RetrieveAllPhoneticData name"  << query->value(2).toString() << LOG_ENDL;
                // alternate phrases
                entry.append(query->value(3).toString());
                // LOG_TRACE << "CXSampaInterface: RetrieveAllPhoneticData alternate phrases"  << query->value(3).toString() << LOG_ENDL;
                // language
                entry.append(query->value(4).toString());
                // LOG_TRACE << "CXSampaInterface: RetrieveAllPhoneticData language"  << query->value(4).toString() << LOG_ENDL;
                // xsampa
                entry.append(query->value(5).toString());
                // LOG_TRACE << "CXSampaInterface: RetrieveAllPhoneticData xsampa"  << query->value(5).toString() << LOG_ENDL;

                data.append(entry);
            }
            returnStatus = true;
            delete query;
            emit DataFetched(data, eRetrieveAllXSampaData_Id);
        }
        db.close();
    }
    return returnStatus;
}

/*
 *******************************************************************
 * Function	: CXSampaInterface::RetrievePhoneticDataForType
 *
 * Description 	: Retrieve phonetic data for a specific type
 *
 * Parameters 	: type - Album, Artist or Genre
 *
 * Returns 	: bool - true or false
 *
 *******************************************************************
 */
bool CXSampaInterface::RetrievePhoneticDataForType(QString type)
{
    if(!m_IsXSampaDBCreated)
    {
        Init();
        m_IsXSampaDBCreated = true;
    }

    QVector<QStringList> data;
    bool returnStatus = false;
    QList<int> CIDInfo;
    CIDInfo = RetrieveCIDforConnectedDevices();

    QString whereCondition;
    whereCondition.clear();

    int count = 0;

    for(int i=0; i<CIDInfo.count(); i++)
    {
        if(CIDInfo.at(i) == 1)
        {
            if(count >=1)
                 whereCondition.append(" OR ");

            int CID = i+1;
            whereCondition.append("CID_");
            whereCondition.append(QString("%1").arg(CID));
            whereCondition.append("=1");
            count ++;
        }
    }

    QSqlDatabase db;
    QString fsDBPath;
    fsDBPath.append("/app/data/gracenote/XSampaDB"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change    

    QString connectionName;
    connectionName.append("XSampa");

    if(createConnection(fsDBPath, connectionName, db))
    {
        QSqlQuery* query = new QSqlQuery(db);

        QString str;
        str.append(QString("SELECT * FROM XSampaTable where Type=\"%1\" AND ").arg(type));
        str.append(whereCondition);

        query->prepare(str);
        if(exec(query))
        {
            returnStatus = true;
            data.clear();
            QStringList entry;

            while(query->next())
            {
                entry.clear();

                // type
                entry.append(query->value(0).toString());
                // LOG_TRACE << "CXSampaInterface: RetrievePhoneticDataForType type"  << query->value(0).toString() << LOG_ENDL;
                // id3 name
                entry.append(query->value(1).toString());
                // LOG_TRACE << "CXSampaInterface: RetrievePhoneticDataForType id3 name"  << query->value(1).toString() << LOG_ENDL;
                // name
                entry.append(query->value(2).toString());
                // LOG_TRACE << "CXSampaInterface: RetrievePhoneticDataForType name"  << query->value(2).toString() << LOG_ENDL;
                // alternate phrases
                entry.append(query->value(3).toString());
                // LOG_TRACE << "CXSampaInterface: RetrievePhoneticDataForType alternate phrases"  << query->value(3).toString() << LOG_ENDL;
                // language
                entry.append(query->value(4).toString());
                // LOG_TRACE << "CXSampaInterface: RetrievePhoneticDataForType language"  << query->value(4).toString() << LOG_ENDL;
                // xsampa
                entry.append(query->value(5).toString());
                // LOG_TRACE << "CXSampaInterface: RetrievePhoneticDataForType xsampa"  << query->value(5).toString() << LOG_ENDL;

                data.append(entry);
            }
            emit DataFetched(data, eRetrieveXSampaDataForType_Id);
        }
        delete query;
        db.close();
    }
    return returnStatus;
}

/*
 *******************************************************************
 * Function	: CXSampaInterface::GetID3NameFromXSampaSqlDB
 *
 * Description 	: Retrieve ID3Name Name for MLT Request
 *
 * Parameters 	: type - ID3Name
 *
 * Returns 	: QDtring - ID3Name Name
 *
 *******************************************************************
 */
bool CXSampaInterface::GetID3NameFromXSampaSqlDB(QString id3)
{
    if(!m_IsXSampaDBCreated)
    {
        Init();
        m_IsXSampaDBCreated = true;
    }

    QString XSampaDBPath("/app/data/gracenote/XSampaDB"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change    
    
    QVector<QStringList> data;
    data.clear();
    QStringList entry;

    QString connectionName;
    connectionName.append("XSampa");

    QFile dbpath(XSampaDBPath);
    if(dbpath.exists())
    {
        QSqlDatabase db;
        bool result = createConnection(XSampaDBPath, connectionName, db);
        if(result)
        {
            LOG_TRACE << "CXSampaInterface::GetID3NameFromXSampaSqlDB db opened" << LOG_ENDL;

            QSqlQuery* query = new QSqlQuery(db);
            query->prepare("SELECT Name FROM XSampaTable WHERE ID3Name = ?");
            query->addBindValue(id3);

            bool status = exec(query);
            if(status)
            {
                while (query->next())
                {
                    entry.clear();
                    entry.append(query->value(0).toString());
                    data.append(entry);
                    // qDebug() << "[TA] GetID3NameFromXSampaSqlDB - ID3-Name" << data.at(0).at(0);
                    break;
                }//end of while
            }//end of if(status)
            else
            {
                return false;
            }
            delete query;
            db.close();
        }//end of db.open()
    }//end of dbpath.exists
    else
    {
        return false;
    }
    emit DataFetched(data, eGetXSampaId3NameQuery_Id);

    return true;
}

/*
 *******************************************************************
 * Function	: CXSampaInterface::createConnection
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
bool CXSampaInterface::createConnection(QString dbPath, QString connectionName, QSqlDatabase &dBHandle)
{
    bool retVal = false;

    if(QSqlDatabase::contains(connectionName))
    {
        dBHandle = QSqlDatabase::database(connectionName);
        dBHandle.setDatabaseName(dbPath);
        if(dBHandle.isOpen())
        {
            retVal = true;
        }
        else
        {
            if (!dBHandle.open())
            {
                LOG_TRACE << "::createConnection: not opened successfully"<< LOG_ENDL;
                retVal = false;
            }
            else
            {
                retVal = true;
            }
        }
    }
    else
    {
        LOG_TRACE << "::createConnection: not already have this connection name"<< LOG_ENDL;
        dBHandle = QSqlDatabase::addDatabase("QSQLITE",connectionName);
        dBHandle.setDatabaseName(dbPath);
        if (!dBHandle.open())
        {
            LOG_TRACE << "::createConnection: not opened successfully"<< LOG_ENDL;
            retVal = false;
        }
        else
        {
            retVal = true;
        }
    }
    return retVal;
}

// End of file
