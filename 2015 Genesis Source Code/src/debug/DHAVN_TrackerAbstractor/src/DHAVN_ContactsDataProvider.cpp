/*
******************************************************************
*        COPYRIGHT (C) Teleca AB      *
*----------------------------------------------------------------*
* MODULE     :  DHAVN_TrackerAbstractor                             *
*
* PROGRAMMER :  Chetan Nanda                                   *
* DATE       :  10 November 2011                                    *
* VERSION    :  1.0                                              *
*
*----------------------------------------------------------------*
*                                                                *
* MODULE SUMMARY : This file defines class which interact with    *
*                  Tracker database using spaql query to retrieve contact related     *
*                  information      *
*                                                                *
*----------------------------------------------------------------*
*
* MODIFICATION RECORDS :                                         *
*
* VERSION   DATE                     PROGRAMMER              COMMENT
******************************************************************
* 1.0       10 November 2011  Chetan Nanda           Draft version
******************************************************************
*/


#include "DHAVN_ContactsDataProvider.h"
#include <QUrl>
#include <QtSql>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QStringList>
#include <QSqlError>
#include<QObject>
#include <QTime>
#include <QDebug>

#define PhoneBookContacts_FILENAME   "/app/tracker/PhoneBookContactsNewAdd"

CContactsDataProvider::CContactsDataProvider()
{
    LOG_INIT_V2("Media_TrackerAbstractor")

}

CContactsDataProvider::~CContactsDataProvider()
{ 
    if(m_ContactDb.isValid())
        m_ContactDb.close();
}

void CContactsDataProvider::HandleError(int nError)
{
        LOG_TRACE << " the error is = " << nError <<  LOG_ENDL;
        LOG_TRACE << "In CContatDataProvider :: HandleError" << LOG_ENDL;
        emit Error(nError);
}

bool CContactsDataProvider::createConnection()
{
//    contactDb = QSqlDatabase::addDatabase("QSQLITE","PhoneDb");
//    contactDb.setDatabaseName("/app/tracker/PhoneBookContacts");


    bool retVal = false;
    QString str="PhoneDb";
    QString dbName= PhoneBookContacts_FILENAME;
    LOG_TRACE << "::createConnection: IN for DB:  " << dbName << LOG_ENDL;
    if(QSqlDatabase::contains(str))
    {
        m_ContactDb = QSqlDatabase::database(str);
        m_ContactDb.setDatabaseName(dbName);
        if(m_ContactDb.isOpen())
        {
            retVal = true;
        }
        else
        {
            if (!m_ContactDb.open())
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
        m_ContactDb = QSqlDatabase::addDatabase("QSQLITE",str);
        m_ContactDb.setDatabaseName(dbName);
        if (!m_ContactDb.open())
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

bool CContactsDataProvider::exec(QSqlQuery* query)
{
    bool result = query->exec();
    LOG_TRACE << Q_FUNC_INFO << " query: " << query->lastQuery() << LOG_ENDL;
    if (query->lastError().isValid()) {
        LOG_TRACE << Q_FUNC_INFO << " last error: " << query->lastError().text() << LOG_ENDL;
    }
    return result;
}

QVector<QStringList> CContactsDataProvider::GetContactData(QSqlQuery* query, const int noOfColumns)
{
    QVector<QStringList>dataList;
    while(query->next())
    {
        QStringList columnList;
        for (int i = 0; i < noOfColumns; i++)
        {
            QString value = query->value(i).toString();

            columnList.append(value.toUtf8());
        }

        dataList.append(columnList);
    }
    return dataList;
}

/* ---- PhoneBook Data Provider - SQLite ---- */
bool CContactsDataProvider::GetAllPhoneBookDataBySelectionFromSQLiteDB(const QString& bd_address, const BT_PBAP_DB_SORTING sortBy, const BT_PBAP_DB_TYPE dbType)
{
   bool ret = false;
   if(bd_address.isNull() || !createConnection())
   {
       HandleError(eNullStringParameter);
       return ret;
   }

   qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
   QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
   QString queryString;
	QString sortOrder;

	switch(sortBy)
	{
		case DB_SORT_BY_INDEX:
	        sortOrder = "idx";
			break;
		case DB_SORT_BY_FIRST_NAME:
	        sortOrder = "first_name";
			break;
		case DB_SORT_BY_LAST_NAME:
			sortOrder = "last_name";
			break;
		case DB_SORT_BY_FORMMATED_NAME:
			sortOrder = "formatted_name";
			break;
		case DB_SORT_BY_TIME_STAMP:
			sortOrder = "time_stamp";
			break;
		default:
			qDebug() << " sortBy value is invalid!!! ";
			sortOrder = "first_name";
			break;
	}

	QString db_type;
	int dataType_count = 0;
	db_type.setNum(dbType);

	switch(dbType)
	{
		case DB_PHONE_BOOK:
		case DB_FAVORITE_PHONE_BOOK:
			queryString = gGetAllSelectedPhoneContactsFromSQLiteDBQuery.arg(bd_address, db_type, sortOrder);
			dataType_count = PHONE_BOOK_DATA_TYPE_COUNT;
			break;
		case DB_RECENT_CALLS:
		case DB_RECEIVED_CALLS:
		case DB_MISSED_CALLS:
			queryString = gGetAllSelectedCallHistoryFromSQLiteDBQuery.arg(bd_address, db_type, sortOrder);
			dataType_count = CALL_HISTORY_DATA_TYPE_COUNT;
			break;
		case DB_CALL_HISTORY:
            /////IQS_15MY
            //queryString = gGetAllCallHistoryFromSQLiteDBQuery.arg(bd_address, sortOrder);
            queryString = gGetAllCallHistoryFromSQLiteDBQuery.arg(bd_address);
            dataType_count = CALL_HISTORY_DATA_TYPE_COUNT;
            /////IQS_15MY
			break;

        case DB_MID_PHONEBOOK:
        case DB_MID_FAVORITE:
            queryString = gMidGetAllSelectedPhoneContactsFromSQLiteDBQuery.arg(bd_address, db_type, sortOrder);
            dataType_count = 15;
            break;

		case DB_COMBINED_CALLS:
		case DB_ALL_TYPE:
		default:
			qDebug() << db_type << " NEED TO CHECK !! ";
			ret = false;
			return ret;
			break;
	}

    query->prepare(queryString);
    exec(query);

	QVector<QStringList>dataList;
	dataList = GetContactData(query, dataType_count);

	if(dataList.count() != 0)
		ret = true;
	else
		ret = false;

    delete query;

	switch(dbType)
	{
		case DB_PHONE_BOOK:
            emit DataFetched(dataList, eGetAllPhoneContactsQuery_Id);
			break;
		case DB_RECENT_CALLS:
            emit DataFetched(dataList, eGetAllRecentCallQuery_Id);
			break;
		case DB_RECEIVED_CALLS:
            emit DataFetched(dataList, eGetAllReceivedCallQuery_Id);
			break;
		case DB_MISSED_CALLS:
            emit DataFetched(dataList, eGetAllMissedCallQuery_Id);
			break;
		case DB_CALL_HISTORY:
            emit DataFetched(dataList, eGetAllCallHistoryQuery_Id);
			break;
		case DB_FAVORITE_PHONE_BOOK:
            emit DataFetched(dataList, eGetAllFavoriteContactsQuery_Id);
			break;
		//case DB_COMBINED_CALLS: // removed by eugene.seo 2013.03.27 for static test
		//case DB_ALL_TYPE:  // removed by eugene.seo 2013.03.27 for static test
		/* // removed by eugene.seo 2013.05.21 for static test
		default:
			qDebug() << db_type << " NEED TO CHECK !! ";
			break;
		*/
	}

	return ret;
}

bool CContactsDataProvider::GetAllPhoneBookDataBySelectionForForeignFromSQLiteDB(const QString& bd_address, const BT_PBAP_DB_TYPE dbType)
{
   bool ret = false;
   if(bd_address.isNull() || !createConnection())
   {
       HandleError(eNullStringParameter);
       return ret;
   }

   qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
   QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
   QString queryString;

    QString db_type;
    int dataType_count = 0;
    db_type.setNum(dbType);

    switch(dbType)
    {
        case DB_PHONE_BOOK:
        case DB_FAVORITE_PHONE_BOOK:
            queryString = gGetAllSelectedPhoneContactsForForeignFromSQLiteDBQuery.arg(bd_address, db_type);
            dataType_count = PHONE_CONTACTS_DATA_TYPE_COUNT;
            break;

        /* Middle Name 추가지원을 위해 DB Type을 임시 추가함(Fake DB Type)
         * DB_MID_FAVORITE 일때 DB_FAVORITE_PHONE_BOOK
         * DB_MID_PHONEBOOK 일때 DB_PHONE_BOOK
         */
        case DB_MID_PHONEBOOK:
            db_type.setNum(DB_PHONE_BOOK);
            queryString = gMidGetAllSelectedPhoneContactsForForeignFromSQLiteDBQuery.arg(bd_address, db_type);
            dataType_count = 17;
            break;

        case DB_MID_FAVORITE:
            db_type.setNum(DB_FAVORITE_PHONE_BOOK);
            queryString = gMidGetAllSelectedPhoneContactsForForeignFromSQLiteDBQuery.arg(bd_address, db_type);
            dataType_count = 17;
            break;

        default:
            qDebug() << db_type << " NEED TO CHECK !! ";
            ret = false;
            return ret;
            break;
    }

    query->prepare(queryString);
    exec(query);

    QVector<QStringList>dataList;
    dataList = GetContactData(query, dataType_count);

    if(dataList.count() != 0)
        ret = true;
    else
        ret = false;

    if(NULL == query) { delete query; }

    switch(dbType)
    {
        case DB_PHONE_BOOK:
            emit DataFetched(dataList, eGetAllPhoneContactsForForeignQuery_Id);
            break;

        case DB_FAVORITE_PHONE_BOOK:
            emit DataFetched(dataList, eGetAllFavoriteContactsForForeignQuery_Id);
            break;
/////SUPPORT_MIDDLE_NAME
        case DB_MID_PHONEBOOK:
            emit DataFetched(dataList, eMidGetAllPhoneContactsForForeignQuery_Id);
            break;

        case DB_MID_FAVORITE:
            emit DataFetched(dataList, eMidGetAllFavoriteContactsForForeignQuery_Id);
            break;
        default:
            // do something
            break;
    }

    return ret;
}


bool CContactsDataProvider::GetAllPhoneBookDataBySelectionForDomesticFromSQLiteDB(const QString& bd_address, const BT_PBAP_DB_TYPE dbType)
{
    bool ret = false;
    if(bd_address.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

	QString db_type;
	int dataType_count = 0;
	db_type.setNum(dbType);

	switch(dbType)
	{
		case DB_PHONE_BOOK:
            queryString = gGetAllSelectedPhoneContactsForDomesticFromSQLiteDBQuery.arg(bd_address, db_type);
            dataType_count = PHONE_CONTACTS_DATA_TYPE_COUNT;
            break;
		case DB_FAVORITE_PHONE_BOOK:
            queryString = gIdxGetAllSelectedPhoneContactsForDomesticFromSQLiteDBQuery.arg(bd_address, db_type);
			dataType_count = PHONE_CONTACTS_DATA_TYPE_COUNT;
			break;

        /*  Middle Name 추가지원을 위해 DB Type을 임시 추가함(Fake DB Type)
         * DB_MID_FAVORITE 일 때 DB_FAVORITE_PHONE_BOOK
         * DB_MID_PHONEBOOK 일 때 DB_PHONE_BOOK
         */
        case DB_MID_PHONEBOOK:
            db_type.setNum(DB_PHONE_BOOK);
            queryString = gMidGetAllSelectedPhoneContactsForDomesticFromSQLiteDBQuery.arg(bd_address, db_type);
            dataType_count = 17;
            break;

        case DB_MID_FAVORITE:
            db_type.setNum(DB_FAVORITE_PHONE_BOOK);
            queryString = gIdxMidGetAllSelectedPhoneContactsForDomesticFromSQLiteDBQuery.arg(bd_address, db_type);
            dataType_count = 17;
            break;

        /* 병음 지원
         * DB_PINYIN_PHONEBOOK 일때 DB_PHONE_BOOK
         */
        case DB_PINYIN_PHONEBOOK:
            db_type.setNum(DB_PHONE_BOOK);
            queryString = gPinyinGetAllSelectedPhoneContactsForDomesticFromSQLiteDBQuery.arg(bd_address, db_type);
            dataType_count = 18;
            break;

		default:
			qDebug() << db_type << " NEED TO CHECK !! ";
			ret = false;
			return ret;
			break;
	}

    query->prepare(queryString);
    exec(query);

    qDebug();

    QVector<QStringList> dataList = GetContactData(query, dataType_count);

	if(dataList.count() != 0)
		ret = true;
	else
		ret = false;


    if(NULL != query) { delete query; query = NULL; }

    switch(dbType)
    {
        case DB_PHONE_BOOK:
            emit DataFetched(dataList, eGetAllPhoneContactsForDomesticQuery_Id);
            break;
            
        case DB_FAVORITE_PHONE_BOOK:
            emit DataFetched(dataList, eGetAllFavoriteContactsForDomesticQuery_Id);
            break;

////SUPPORT_MIDDLE_NAME
        /* Middle Name 추가지원을 위해 DB Type을 임시 추가함
         * DB_MID_PHONEBOOK 일 때 eMidGetAllPhoneContactsForDomesticQuery_Id
         * DB_MID_FAVORITE 일 때 eMidGetAllFavoriteContactsForDomesticQuery_Id
         */
        case DB_MID_PHONEBOOK:
            emit DataFetched(dataList, eMidGetAllPhoneContactsForDomesticQuery_Id);
            break;

        case DB_MID_FAVORITE:
            emit DataFetched(dataList, eMidGetAllFavoriteContactsForDomesticQuery_Id);
            break;
/////SUPPORT_MIDDLE_NAME

        case DB_PINYIN_PHONEBOOK:
            emit DataFetched(dataList, ePinyinGetAllSelectedPhoneContactsForDomesticQuery_id);
            break;
    /* FEATURE_ORDER_BY_PINYIN */

        default:
            break;
    }

	return ret;
}

/* ---- PhoneBook Data Search - SQLite ---- */
bool CContactsDataProvider::GetContactDetailsByNameFromSQLiteDB(const QString& bd_address, const QString& contactName)
{
    bool ret = false;
    if(bd_address.isNull() || contactName.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

    queryString = gGetContactDetailsByNameFromSQLiteDBQuery.arg(bd_address, contactName);
    query->prepare(queryString);
    exec(query);

    QVector<QStringList>dataList;
    dataList = GetContactData(query, PHONE_BOOK_SEARCH_RESULT_DATA_TYPE);
     if(dataList.count() != 0)
         ret = true;
     else
         ret = false;

     delete query;

     emit DataFetched(dataList, eGetContactDetailsByNameQuery_Id);

    return ret;
}

bool CContactsDataProvider::GetAllContactsByKeywordForBDAddressFromSQLiteDB(const QString& bd_address, const QString& keyword)
{
    bool ret = false;

   if(bd_address.isNull() || keyword.isNull() || !createConnection())
   {
       HandleError(eNullStringParameter);
       return ret;
   }

   qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
   QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
   QString queryString;

   QString searchKeyword;
   searchKeyword.append("%");
   searchKeyword.append(keyword);
   searchKeyword.append("%");
   queryString = gGetAllContactsByKeywordForBDAdressFromSQLiteDBQuery.arg(bd_address, searchKeyword);
   query->prepare(queryString);
   exec(query);

   QVector<QStringList>dataList;
   dataList = GetContactData(query, PHONE_BOOK_SEARCH_RESULT_DATA_TYPE);
    if(dataList.count() != 0)
        ret = true;
    else
        ret = false;

    delete query;

    emit DataFetched(dataList, eGetAllContactsByKeywordForBDAdressQuery_Id);

    return ret;
}


bool CContactsDataProvider::GetContactByNumberFromSQLiteDB(const QUERY_TYPE_T type, const QString& bd_address, const QString& phoneNumber)
{
    bool ret = false;

    if(bd_address.isNull() || phoneNumber.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

    int count = 0;
    switch(type)
    {
        case DOMESTIC:
            queryString = gGetContactByNumberForDomesticFromSQLiteDBQuery.arg(bd_address, phoneNumber);
            count = PHONE_BOOK_SEARCH_RESULT_DATA_TYPE;
            break;

        case FOREIGN:
            queryString = gGetContactByNumberForForeignFromSQLiteDBQuery.arg(bd_address, phoneNumber);
            count = PHONE_BOOK_SEARCH_RESULT_DATA_TYPE;
            break;

        case DOMESTIC_MIDDLE:
            // TODO: queryString = gGetContactByNumberForDomesticFromSQLiteDBQuery.arg(bd_address, phoneNumber);
            queryString = gMidGetContactByNumberForDomesticFromSQLiteDBQuery.arg(bd_address, phoneNumber);
            count = 17;
            break;

        default:
            // TODO: error handling
            if(NULL != query) { delete query; query = NULL; }
            return ret;
    }
            
    query->prepare(queryString);
    exec(query);

    // Convert result set to QVector
    QVector<QStringList>dataList = GetContactData(query, count);
    ret = (dataList.count() != 0) ? true : false;

    if(NULL != query) { delete query; query = NULL; }

    switch(type)
    {
        case DOMESTIC:
            emit DataFetched(dataList, eGetContactByNumberForDomesticQuery_Id);
            break;

        case FOREIGN:
            emit DataFetched(dataList, eGetContactByNumberForForeignQuery_Id);
            break;

        case DOMESTIC_MIDDLE:
            // TODO: emit DataFetched(dataList, eGetContactByNumberForDomesticQuery_Id);
            emit DataFetched(dataList, eMidGetContactByNumberForDomesticQuery_Id);
            break;

        default:
            // TODO: error handling
            break;
    }
    
    return ret; 
}


#if 0   /* MERGED */
bool CContactsDataProvider::GetContactByNumberForForeignFromSQLiteDB(const QString& bd_address, const QString& phoneNumber)
{
    bool ret = false;

    if(bd_address.isNull() || phoneNumber.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

    queryString = gGetContactByNumberForForeignFromSQLiteDBQuery.arg(bd_address, phoneNumber);
    query->prepare(queryString);
    exec(query);

    // Convert result set to QVector
    QVector<QStringList>dataList = GetContactData(query, PHONE_BOOK_SEARCH_RESULT_DATA_TYPE);
    ret = (dataList.count() != 0) ? true : false;

    if(NULL != query) { delete query; query = NULL; }

    emit DataFetched(dataList, eGetContactByNumberForForeignQuery_Id);
    return ret;
}


bool CContactsDataProvider::GetContactByNumberForDomesticFromSQLiteDB(const QString& bd_address, const QString& phoneNumber)
{
    bool ret = false;

    if(bd_address.isNull() || phoneNumber.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

    queryString = gGetContactByNumberForDomesticFromSQLiteDBQuery.arg(bd_address, phoneNumber);
    query->prepare(queryString);
    exec(query);

    // Convert result set to QVector
    QVector<QStringList>dataList = GetContactData(query, PHONE_BOOK_SEARCH_RESULT_DATA_TYPE);
    ret = (dataList.count() != 0) ? true : false;

    if(NULL != query) { delete query; query = NULL; }


    emit DataFetched(dataList, eGetContactByNumberForDomesticQuery_Id);
    return ret;
}
#endif  /* MERGED */


bool CContactsDataProvider::GetContactByNumberFromSQLiteDB_KR(const QString& bd_address, const QString& phoneNumber, const int section)
{
    bool ret = false;

    if(bd_address.isNull() || phoneNumber.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;
	
	QString iSection;
	iSection.setNum(section);

    queryString = gGetContactByNumberFromSQLiteDBQuery_KR.arg(bd_address, phoneNumber, iSection);
    query->prepare(queryString);
    exec(query);

    QVector<QStringList>dataList;
    dataList = GetContactData(query, PHONE_BOOK_SEARCH_RESULT_DATA_TYPE);
     if(dataList.count() != 0)
         ret = true;
     else
         ret = false;

     delete query;

     emit DataFetched(dataList, eGetContactByNumberQuery_Id_KR);

    return ret;
}

bool CContactsDataProvider::GetContactByNumberFromSQLiteDB_CN(const QString& bd_address, const QString& phoneNumber, const int section)
{
    bool ret = false;

    if(bd_address.isNull() || phoneNumber.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

        QString iSection;
        iSection.setNum(section);

    queryString = gGetContactByNumberFromSQLiteDBQuery_CN.arg(bd_address, phoneNumber, iSection);
    query->prepare(queryString);
    exec(query);

    QVector<QStringList>dataList;
    dataList = GetContactData(query, PHONE_BOOK_SEARCH_RESULT_DATA_TYPE);
     if(dataList.count() != 0)
         ret = true;
     else
         ret = false;

     delete query;

     emit DataFetched(dataList, eGetContactByNumberQuery_Id_CN);

    return ret;
}

bool CContactsDataProvider::GetContactByNumberFromSQLiteDB_NA(const QString& bd_address, const QString& phoneNumber, const int section)
{
    bool ret = false;

    if(bd_address.isNull() || phoneNumber.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

        QString iSection;
        iSection.setNum(section);

    queryString = gGetContactByNumberFromSQLiteDBQuery_CN.arg(bd_address, phoneNumber, iSection);
    query->prepare(queryString);
    exec(query);

    QVector<QStringList>dataList;
    dataList = GetContactData(query, PHONE_BOOK_SEARCH_RESULT_DATA_TYPE);
     if(dataList.count() != 0)
         ret = true;
     else
         ret = false;

     delete query;

     emit DataFetched(dataList, eGetContactByNumberQuery_Id_NA);

    return ret;
}

bool CContactsDataProvider::GetContactByNumberCallHistoryNameFromSQLiteDB(const QUERY_TYPE_T type
                                                                        , const QString& bd_address, const QString& phoneNumber)
{
    bool ret = false;

    if(bd_address.isNull() || phoneNumber.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

    int count = 0;
    switch(type)
    {
        case DOMESTIC:
            queryString = gGetContactByNumberForDomesticCallHistoryNameFromSQLiteDBQuery.arg(bd_address, phoneNumber);
            count = CALL_HISTORY_SEARCH_RESULT_DATA_TYPE;
            break;

        case FOREIGN:
            queryString = gGetContactByNumberForForeignCallHistoryNameFromSQLiteDBQuery.arg(bd_address, phoneNumber);
            count = CALL_HISTORY_SEARCH_RESULT_DATA_TYPE;
            break;

        case DOMESTIC_MIDDLE:
            // TODO: queryString = gGetContactByNumberForForeignCallHistoryNameFromSQLiteDBQuery.arg(bd_address, phoneNumber);
            queryString = gMidGetContactByNumberForDomesticCallHistoryNameFromSQLiteDBQuery.arg(bd_address, phoneNumber);
            count = 17;
            break;

/*
        case FOREIGN_MIDDLE:
        // TODO: queryString = gGetContactByNumberForForeignCallHistoryNameFromSQLiteDBQuery.arg(bd_address, phoneNumber);
        queryString = gGetContactByNumberForForeignCallHistoryNameFromSQLiteDBQuery.arg(bd_address, phoneNumber);
        count = CALL_HISTORY_SEARCH_RESULT_DATA_TYPE + 1;
        break;
*/

        default:
            // TODO: error handling
            if(NULL != query) { delete query; query = NULL; }
            return ret;
    }

    qDebug() << queryString;
    query->exec(queryString);
    qDebug() << query->lastError().text();

    // Convert result set to QVector
    QVector<QStringList>dataList = GetContactData(query, count);
    ret = (dataList.count() != 0) ? true : false;

    if(NULL != query) { delete query; query = NULL; }

    switch(type)
    {
        case DOMESTIC:
            emit DataFetched(dataList, eGetContactByNumberForDomesticCallHistoryNameQuery_Id);
            break;
 
        case FOREIGN:
            emit DataFetched(dataList, eGetContactByNumberForForeignCallHistoryNameQuery_Id);
            break;
 
        case DOMESTIC_MIDDLE:
            // TODO: emit DataFetched(dataList, eGetContactByNumberForDomesticCallHistoryNameQuery_Id);
            emit DataFetched(dataList, eMidGetContactByNumberForDomesticCallHistoryNameQuery_Id);
            break;
        /*
            case FOREIGN_MIDDLE:
        emit DataFetched(dataList, eMidGetContactByNumberForForeignCallHistoryNameQuery_Id);
        break;
        */
 
        default:
            // TODO: error handling
            break;
    }
     
    return ret;
}

#if 0   /* MERGED */
bool CContactsDataProvider::GetContactByNumberForForeignCallHistoryNameFromSQLiteDB(QString bd_address, QString phoneNumber)
{
    bool ret = false;

    if(bd_address.isNull() || phoneNumber.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

    queryString = gGetContactByNumberForForeignCallHistoryNameFromSQLiteDBQuery.arg(bd_address, phoneNumber);
    qDebug() << queryString;
    query->exec(queryString);
    qDebug() << query->lastError().text();

    QVector<QStringList>dataList;
    dataList = GetContactData(query, CALL_HISTORY_SEARCH_RESULT_DATA_TYPE);
     if(dataList.count() != 0)
         ret = true;
     else
         ret = false;

     delete query;

     emit DataFetched(dataList, eGetContactByNumberForForeignCallHistoryNameQuery_Id);

    return ret;
}

bool CContactsDataProvider::GetContactByNumberForDomesticCallHistoryNameFromSQLiteDB(QString bd_address, QString phoneNumber)
{
    bool ret = false;

    if(bd_address.isNull() || phoneNumber.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

    queryString = gGetContactByNumberForDomesticCallHistoryNameFromSQLiteDBQuery.arg(bd_address, phoneNumber);
    qDebug() << queryString;
    query->exec(queryString);
    qDebug() << query->lastError().text();

    QVector<QStringList>dataList;
    dataList = GetContactData(query, CALL_HISTORY_SEARCH_RESULT_DATA_TYPE);
     if(dataList.count() != 0)
         ret = true;
     else
         ret = false;

     delete query;

     emit DataFetched(dataList, eGetContactByNumberForDomesticCallHistoryNameQuery_Id);

    return ret;
}
#endif  /* MERGED */


bool CContactsDataProvider::GetContactByNumberForCallHistoryNameFromSQLiteDB_KR(const QString& bd_address, const QString& phoneNumber, const int section)
{
    bool ret = false;

    if(bd_address.isNull() || phoneNumber.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

	QString iSection;
	iSection.setNum(section);

    queryString = gGetContactByNumberForCallHistoryNameFromSQLiteDBQuery_KR.arg(bd_address, phoneNumber, iSection);
    qDebug() << queryString;
    query->exec(queryString);
    qDebug() << query->lastError().text();

    QVector<QStringList>dataList;
    dataList = GetContactData(query, CALL_HISTORY_SEARCH_RESULT_DATA_TYPE);
     if(dataList.count() != 0)
         ret = true;
     else
         ret = false;

     delete query;

     emit DataFetched(dataList, eGetContactByNumberForCallHistoryNameQuery_Id_KR);

    return ret;
}

bool CContactsDataProvider::GetContactByNumberForCallHistoryNameFromSQLiteDB_CN(const QString& bd_address, const QString& phoneNumber, const int section)
{
    bool ret = false;

    if(bd_address.isNull() || phoneNumber.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

        QString iSection;
        iSection.setNum(section);

    queryString = gGetContactByNumberForCallHistoryNameFromSQLiteDBQuery_CN.arg(bd_address, phoneNumber, iSection);
    qDebug() << queryString;
    query->exec(queryString);
    qDebug() << query->lastError().text();

    QVector<QStringList>dataList;
    dataList = GetContactData(query, CALL_HISTORY_SEARCH_RESULT_DATA_TYPE);
     if(dataList.count() != 0)
         ret = true;
     else
         ret = false;

     delete query;

     emit DataFetched(dataList, eGetContactByNumberForCallHistoryNameQuery_Id_CN);

    return ret;
}

bool CContactsDataProvider::GetContactByNumberForCallHistoryNameFromSQLiteDB_NA(const QString& bd_address, const QString& phoneNumber, const int section)
{
    bool ret = false;

    if(bd_address.isNull() || phoneNumber.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

    QString iSection;
    iSection.setNum(section);

    queryString = gGetContactByNumberForCallHistoryNameFromSQLiteDBQuery_KR.arg(bd_address, phoneNumber, iSection);
    qDebug() << queryString;
    query->exec(queryString);
    qDebug() << query->lastError().text();

    QVector<QStringList>dataList;
    dataList = GetContactData(query, CALL_HISTORY_SEARCH_RESULT_DATA_TYPE);
     if(dataList.count() != 0)
         ret = true;
     else
         ret = false;

     delete query;

     emit DataFetched(dataList, eGetContactByNumberForCallHistoryNameQuery_Id_NA);

    return ret;
}

bool CContactsDataProvider::GetFavoriteContactByNameAndNumberFromSQLiteDB(const QString& bd_address, const QString& firstName, const QString& lastName, const QString& phoneNumber)
{
    bool ret = false;

	if(bd_address.isNull() || firstName.isNull() || lastName.isNull() || phoneNumber.isNull() || !createConnection())
	{
		HandleError(eNullStringParameter);
		return ret;
	}

   qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
   QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
   QString queryString;

    queryString = gGetFavoriteContactByNameAndNumberFromSQLiteDBQuery.arg(bd_address, firstName, lastName, phoneNumber);
    query->prepare(queryString);
    exec(query);

	QVector<QStringList>dataList;
	dataList = GetContactData(query, PHONE_BOOK_SEARCH_RESULT_DATA_TYPE);

	if(dataList.count() != 0)
		ret = true;
	else
		ret = false;

	delete query;

    emit DataFetched(dataList, eGetFavoriteContactByNameAndNumberQuery_Id);

   return ret;
}

bool CContactsDataProvider::MidGetFavoriteContactByNameAndNumberFromSQLiteDB(const QString& bd_address, const QString& firstName, const QString& lastName, const QString& middleName,const QString& phoneNumber)
{
    bool ret = false;

    if(bd_address.isNull() || firstName.isNull() || lastName.isNull() || phoneNumber.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

   qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
   QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
   QString queryString;

    queryString = gMidGetFavoriteContactByNameAndNumberFromSQLiteDBQuery.arg(bd_address, firstName, lastName, middleName, phoneNumber);

    query->prepare(queryString);
    exec(query);

    QVector<QStringList>dataList;
    dataList = GetContactData(query, PHONE_BOOK_SEARCH_RESULT_DATA_TYPE);

    if(dataList.count() != 0)
        ret = true;
    else
        ret = false;

    delete query;

    emit DataFetched(dataList, eMidGetFavoriteContactByNameAndNumberQuery_Id);

   return ret;
}


bool CContactsDataProvider::MidGetContactByCallHistoryInfoFromSQLiteDB(const QString& bd_address
                                                                    , const QString& firstName, const QString& lastName, const QString& middleName
                                                                    , const QString& phoneNumber)
{
    bool ret = false;

    if(bd_address.isNull() || firstName.isNull() || lastName.isNull() || phoneNumber.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

    queryString = gMidGetContactByCallHistoryInfoFromSQLiteDBQuery.arg(bd_address, firstName, lastName, middleName, phoneNumber);
    query->prepare(queryString);
    exec(query);

    QVector<QStringList>dataList = GetContactData(query, CALL_HISTORY_SEARCH_RESULT_DATA_TYPE);

    ret = (dataList.count() != 0) ? true : false;

    if(NULL != query) { delete query; query = NULL; }

    // TODO:
    emit DataFetched(dataList, eMidGetContactByCallHistoryInfoQuery_Id);
    return ret;
}


bool CContactsDataProvider::GetContactByCallHistoryInfoFromSQLiteDB(const QString& bd_address
                                                                    , const QString& firstName, const QString& lastName
                                                                    , const QString& phoneNumber)
{
    bool ret = false;

    if(bd_address.isNull() || firstName.isNull() || lastName.isNull() || phoneNumber.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

    queryString = gGetContactByCallHistoryInfoFromSQLiteDBQuery.arg(bd_address, firstName, lastName, phoneNumber);
    query->prepare(queryString);
    exec(query);

    QVector<QStringList>dataList = GetContactData(query, CALL_HISTORY_SEARCH_RESULT_DATA_TYPE);
    ret = (dataList.count() != 0) ? true : false;

    if(NULL != query) { delete query; query = NULL; }

    emit DataFetched(dataList, eGetContactByCallHistoryInfoQuery_Id);
    return ret;
}



bool CContactsDataProvider::GetContactByCallHistoryInfoFromSQLiteDB_KR_NA(const QString& bd_address
                                                                          , const QString& firstName, const QString& lastName
                                                                          , const QString& phoneNumber, const int section)
{
    bool ret = false;

    if(bd_address.isNull() || firstName.isNull() || lastName.isNull() || phoneNumber.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

   	QString iSection;
	iSection.setNum(section);

    queryString = gGetContactByCallHistoryInfoFromSQLiteDBQuery_KR_NA.arg(bd_address, firstName, lastName, phoneNumber, iSection);
    query->prepare(queryString);
    exec(query);

    QVector<QStringList>dataList = GetContactData(query, CALL_HISTORY_SEARCH_RESULT_DATA_TYPE);
    ret = (dataList.count() != 0) ? true : false;

    if(NULL != query) { delete query; query = NULL; }


    emit DataFetched(dataList, eGetContactByCallHistoryInfoQuery_Id_KR_NA);
    return ret;
}


bool CContactsDataProvider::GetAllPhoneBookDataCountBySelectionFromSQLiteDB(const QString& bd_address, const BT_PBAP_DB_TYPE dbType)
{
	LOG_TRACE << __FUNCTION__ << LOG_ENDL;

	bool ret = false;

	if(bd_address.isNull() || !createConnection())
	{
		LOG_TRACE << __FUNCTION__ << " handle error!! " << LOG_ENDL;
		HandleError(eNullStringParameter);
		return ret;
	}

	LOG_TRACE << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName() << LOG_ENDL;
	QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
	QString queryString;


	QString db_type;
	db_type.setNum(dbType);

	switch(dbType)
	{
		case DB_PHONE_BOOK:
		case DB_RECENT_CALLS:
		case DB_RECEIVED_CALLS:
		case DB_MISSED_CALLS:
		case DB_FAVORITE_PHONE_BOOK:
			queryString = gGetAllSelectedPhoneContactsCountFromSQLiteDBQuery.arg(bd_address, db_type);
			break;
		case DB_CALL_HISTORY:
			queryString = gGetAllCallHistoryCountFromSQLiteDBQuery.arg(bd_address);
			break;
		case DB_COMBINED_CALLS:
		case DB_ALL_TYPE:
		default:
			qDebug() << db_type << " NEED TO CHECK !! ";
			ret = false;
			return ret;
			break;
	}

    query->prepare(queryString);
    exec(query);

	QVector<QStringList>dataList;
	dataList = GetContactData(query, PHONE_BOOK_COUNT_DATA_TYPE);

	if(dataList.count() > 0)
	{
		ret = true;
	}
	else
	{
		QStringList list;
		QString str;
		str.setNum(0);
		list.append(str);
		dataList.append(list);
		LOG_TRACE << __FUNCTION__ <<" False " << LOG_ENDL;
		ret = false;
	}

	delete query;

	switch(dbType)
	{
		case DB_PHONE_BOOK:
            emit DataFetched(dataList, eGetAllPhoneContactCountQuery_Id);
			break;
		case DB_RECENT_CALLS:
            emit DataFetched(dataList, eGetAllRecentCallCountQuery_Id);
			break;
		case DB_RECEIVED_CALLS:
            emit DataFetched(dataList, eGetAllReceivedCallCountQuery_Id);
			break;
		case DB_MISSED_CALLS:
            emit DataFetched(dataList, eGetAllMissedCallCountQuery_Id);
			break;
		case DB_CALL_HISTORY:
            emit DataFetched(dataList, eGetAllCallHistoryCountQuery_Id);
			break;
		case DB_FAVORITE_PHONE_BOOK:
            emit DataFetched(dataList, eGetAllFavoriteContactCountQuery_Id);
			break;
		//case DB_COMBINED_CALLS: // removed by eugene.seo 2013.03.27 for static test
		//case DB_ALL_TYPE: // removed by eugene.seo 2013.03.27 for static test
		/* // removed by eugene.seo 2013.05.21 for static test
		default:
			qDebug() << db_type << " NEED TO CHECK !! ";
			break;
		*/
	}
	
	return ret;
}

bool CContactsDataProvider::GetPhoneBookHashValueByBDAddressFromSQLiteDB(const QString& bd_address)
{
    bool ret = false;

    if(bd_address.isNull() || !createConnection())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    qDebug() << "Name of database " << QSqlDatabase::database("PhoneDb").databaseName();
    QSqlQuery* query = new QSqlQuery(QSqlDatabase::database("PhoneDb"));
    QString queryString;

    queryString = gGetPhoneBookHashValueByBDAddressFromSQLiteDBQuery.arg(bd_address);
	
    query->prepare(queryString);
    exec(query);

    QVector<QStringList>dataList;
    dataList = GetContactData(query, PHONE_BOOK_CRYPTO_HASH_RESULT_DATA_TYPE);

     if(dataList.count() != 0)
         ret = true;
     else
         ret = false;

     delete query;

     emit DataFetched(dataList, eGetPhoneBookHashValueByBDAddressQuery_Id);

    return ret;
}
//End  of file
