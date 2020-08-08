/*
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * History
 * 01-Aug-2011      Divya Dsouza
 ******************************************************************
 */

#ifndef DHAVN_TRACKERQUERY_H
#define DHAVN_TRACKERQUERY_H

#include <QObject>
#include <QVector>
#include <QStringList>
#include <QTimer>
#include <DHAVN_TrackerMusicData.h>
#include <DHAVN_TrackerAbstractor_constants.h>
#include <DHAVN_LogUtil.h>
#include "DHAVN_GracenoteInterface.h"
// Constants
#define SQLCOUNTLIMIT 500
#define XSampaDB_Max_Size 10 * 1024 * 1024

// Forward declarations
class CTrackerExtractor;
class CTrackerAbstractor;
class CGracenoteFileHandling;

class CTrackerQuery : public QObject
{
    Q_OBJECT
    USE_LOG_UTIL

public:
    CTrackerQuery(QObject *parent = 0);
    ~CTrackerQuery();
    bool Init();

    void SetGracenoteInterface(CGracenoteInterface* aInterface);
    void GetFilePathsInJukebox();
    bool UpdateQuery(TrackerMusicData musicData);
    bool DeleteCDData(QString cdTOC); 
    
    void UpdatePhoneticData(QString CID, QVector<QStringList> phoneticData);
    void UpdateCIDInfoInXSampaDB(QString CID, QStringList phoneticData);
    bool CheckIfPhoneticDataExists(QString type, QString name);
    void GetFilePathsInUSB(QString serialNum, QString mountPath, int volume);
    void SetQueueInterface(CGracenoteRequestQueue *aInterface);

    void SetUSBInfo(QString serialNum, QString mountPath, int volume);

    void GetUSBInfo(QString &, QString &, int volume);
    void UpdateUSBIndex(int index, QString serialNum, int nVolume); //modified by aettie 2013.03.22 for ITS 148383
    int GetUSBIndex(QString serialNum);
    void SetMLDBSizeState(bool Value);
    bool IsMLDBSizeLimitReached();

    Q_SIGNALS: // SIGNALS

    void indexingStatus(bool start, int deviceType);


private:
    void GetFilePathsInUSB();
    void UpdateMLDBForJukeBox(QString filePath, gn_collection_id_t nCID);
    void UpdateMLDBForUSB(QString filePath, gn_collection_id_t nCID);
    QString ModifyFileName(QString originalFileName);
    void InsertFilesInformation(EVolumeType volumeType, QStringList existingList, QStringList &m_NewFileList, QString mountPath = QString());

    void UpdateFilesInJukebox();
    void UpdateFilesInUSB(QString serialNum, QString mountPath, int volume);
    bool CheckIfUSBExists(QString mountPath);

private:
    CTrackerAbstractor* m_pTrackerAbstractor;
    CGracenoteInterface* m_pGracenoteInterface;

    QTimer *m_Timer; 

    CGracenoteFileHandling* m_pFileHandling;
    CGracenoteRequestQueue *m_pQueue;
    QStringList m_JukeBoxNewFileList;
    QStringList m_USBNewFileList;
    int m_JukeIndex;
    // This value will be set to true If the population of the MLDB file is increased beyond the 20MB
    bool m_StopMLDBPopulation;
};

#endif // DHAVN_TRACKERQUERY_H
