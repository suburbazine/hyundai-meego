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

#ifndef DHAVN_CONTACTSDATAPROVIDER_H
#define DHAVN_CONTACTSDATAPROVIDER_H

#include <QStringList>
#include <QVector>
#include <DHAVN_LogUtil.h>
#include "DHAVN_TrackerAbstractor_constants.h"
#include <QSqlDatabase>
class QSqlQuery;

/**
   * This class will handle the request from the CTrackerAbstractor class and
   *  execute the contact related sqlite query. This class exposes the signal that the CTrackerProvider
   *  would be calling for any operation like receiving data
   */
class CContactsDataProvider : public QObject{
    Q_OBJECT

    USE_LOG_UTIL

public:
	/* CONSTANTS */
	typedef enum
{
          INVALID           = -1
        , DOMESTIC          = 0
        , FOREIGN
        , DOMESTIC_MIDDLE
        , FOREIGN_MIDDLE
	} QUERY_TYPE_T;

public:
	CContactsDataProvider();
	~CContactsDataProvider();

    bool GetAllPhoneBookDataBySelectionFromSQLiteDB(const QString& bd_address, const BT_PBAP_DB_SORTING sortBy,const BT_PBAP_DB_TYPE dbType);
	bool GetAllPhoneBookDataBySelectionForForeignFromSQLiteDB(const QString& bd_address, const BT_PBAP_DB_TYPE dbType);
	bool GetAllPhoneBookDataBySelectionForDomesticFromSQLiteDB(const QString& bd_address, const BT_PBAP_DB_TYPE dbType);

    bool GetContactDetailsByNameFromSQLiteDB(const QString& bd_address, const QString& contactName);
    bool GetAllContactsByKeywordForBDAddressFromSQLiteDB(const QString& bd_address, const QString& keyword);

	bool GetContactByNumberFromSQLiteDB(const QUERY_TYPE_T type, const QString& bd_address, const QString& phoneNumber);

    bool GetContactByNumberFromSQLiteDB_KR(const QString& bd_address, const QString& phoneNumber, const int section);
    bool GetContactByNumberFromSQLiteDB_CN(const QString& bd_address, const QString& phoneNumber, const int section);
    bool GetContactByNumberFromSQLiteDB_NA(const QString& bd_address, const QString& phoneNumber, const int section);
    bool GetFavoriteContactByNameAndNumberFromSQLiteDB(const QString& bd_address, const QString& firstName, const QString& lastName, const QString& phoneNumber);
    bool MidGetFavoriteContactByNameAndNumberFromSQLiteDB(const QString& bd_address
                                                          , const QString& firstName, const QString& lastName, const QString& middleName
                                                          , const QString& phoneNumber);


    /* eGetContactByCallHistoryInfoQuery_Id
     * eGetContactByCallHistoryInfoQuery_Id_KR_NA
     * eMidGetContactByCallHistoryInfoQuery_Id
     */
    bool GetContactByCallHistoryInfoFromSQLiteDB(const QString& bd_address
                                                    , const QString& firstName, const QString& lastName
                                                    , const QString& phoneNumber);

    bool MidGetContactByCallHistoryInfoFromSQLiteDB(const QString& bd_address
                                                    , const QString& firstName, const QString& lastName, const QString& middleName
                                                    , const QString& phoneNumber);

    bool GetContactByCallHistoryInfoFromSQLiteDB_KR_NA(const QString& bd_address
                                                       , const QString& firstName, const QString& lastName
                                                       , const QString& phoneNumber, const int section);


    /*
     */
	bool GetContactByNumberCallHistoryNameFromSQLiteDB(const QUERY_TYPE_T type
                                                        ,  const QString& bd_address, const QString& phoneNumber);

    bool GetContactByNumberForCallHistoryNameFromSQLiteDB_KR(const QString& bd_address, const QString& phoneNumber, const int section);
    bool GetContactByNumberForCallHistoryNameFromSQLiteDB_NA(const QString& bd_address, const QString& phoneNumber, const int section);
    bool GetContactByNumberForCallHistoryNameFromSQLiteDB_CN(const QString& bd_address, const QString& phoneNumber, const int section);

    bool GetAllPhoneBookDataCountBySelectionFromSQLiteDB(const QString& bd_address, const BT_PBAP_DB_TYPE dbType);
    bool GetPhoneBookHashValueByBDAddressFromSQLiteDB(const QString& bd_address);
	
private:
    QVector<QStringList> GetContactData(QSqlQuery* query, const int noOfColumns);
    bool exec(QSqlQuery* query);

signals:
    /**
      * Signal to notify CTrackerAbstractor class on query completion
      */
    void DataFetched(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId);
	void Error(int);
	
private:
	void   HandleError(int nError);
	bool createConnection();

private:
	QSqlDatabase m_ContactDb;
};
#endif // DHAVN_CONTACTSDATAPROVIDER_H
