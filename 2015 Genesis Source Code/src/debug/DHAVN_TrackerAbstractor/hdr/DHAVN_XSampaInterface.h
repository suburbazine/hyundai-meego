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
#ifndef DHAVN_XSAMPAINTERFACE_H
#define DHAVN_XSAMPAINTERFACE_H

#include <QObject>
#include <QSqlDatabase>
#include <QVector>
#include <QStringList>
#include "DHAVN_TrackerAbstractor_constants.h"

#include <DHAVN_LogUtil.h>

class CXSampaInterface : public QObject
{
    Q_OBJECT
    USE_LOG_UTIL

public:
    CXSampaInterface(QObject *parent = 0);
    ~CXSampaInterface();
    bool Init();

    bool CheckIfPhoneticDataExists(QString type, QString name);
    bool UpdatePhoneticData(QVector<QStringList> phoneticData, int CID);
    bool RetrievePhoneticData(QString type, QString name);
    bool RetrieveAllPhoneticData();
    bool RetrievePhoneticDataForType(QString type);
    bool GetID3NameFromXSampaSqlDB(QString id3);
    void UpdateCollectionIDInfo(int CID, QString type, QString name);

private:
    bool exec(QSqlQuery* query);

    bool CreateXSampaDatabase();
    bool CreateXSampaTable(QSqlDatabase db);
    QList<int> RetrieveCIDforConnectedDevices();
    bool createConnection(QString dbPath, QString connectionName, QSqlDatabase &dBHandle);

signals:

    void DataFetched(QVector<QStringList> dataList,
                               ETrackerAbstarctor_Query_Ids queryId);


private:
    bool m_IsXSampaDBCreated;
};

#endif // DHAVN_XSAMPAINTERFACE_H
