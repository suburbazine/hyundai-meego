#ifndef DHAVN_NORMALIZEDDATAUTIL_H
#define DHAVN_NORMALIZEDDATAUTIL_H

/*
 ******************************************************************
 *    COPYRIGHT ?Teleca AB      *
 *----------------------------------------------------------------*
 * MODULE     	:  CNormalizedData                             	  *
 *
 * PROGRAMMERS 	:  Divya Dsouza
 * DATE       	:  11 October 2012              *
 * VERSION    	:                                             	  *
 *
 *----------------------------------------------------------------*
 *                                                                *
 *  MODULE SUMMARY : CNormalizedData stores Gracenote metadata
 *  into a SQLite DB
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                     *
 *
 *     DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * History
 * 11-Oct-2012      Divya Dsouza
 ******************************************************************
 */

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDirIterator>
#include <QSemaphore> //added by Puneet for fixing GPP crash issue 2013.06.12
#include <DHAVN_LogUtil.h>
#include "DHAVN_TrackerMusicData.h"

class CNormalizedData : public QObject
{
    Q_OBJECT
    USE_LOG_UTIL

public:
    CNormalizedData(QObject *parent = 0);
    ~CNormalizedData();
    bool Init();

    bool CreateSQLiteDB(int CID);
    bool DeleteSQLiteDB(int CID);
    bool PopulateDB(const TrackerMusicData& musicData, int CID);
    bool UpdateRecord(int CID, QString value, QString filePath);
    bool DeleteRecords(int CID, QStringList filePaths);
    bool CheckIfEntryExists(int CID, QString filepath);
    void CopySQLiteDBtoFS(int CID);
    bool createConnection(QString connectionName, QSqlDatabase &dBHandle);
	//added by Puneet for fixing GPP crash issue 2013.06.12
private:
    QSemaphore *m_semaphore;
};

#endif // DHAVN_NORMALIZEDDATAUTIL_H
