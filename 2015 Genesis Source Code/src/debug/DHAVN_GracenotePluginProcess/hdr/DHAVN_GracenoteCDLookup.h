#ifndef DHAVN_GRACENOTECDLOOKUP_H
#define DHAVN_GRACENOTECDLOOKUP_H

#include <QObject>
#include <gn_abs_types.h>
#include <gn_album_track_types.h>
#include "DHAVN_TrackerMusicData.h"
#include <DHAVN_LogUtil.h>
//added by Puneet for fixing GPP crash issue 2013.06.12
#include <QSemaphore>
#include <QMutex>
#include <QMutexLocker>


//Forward declaration
class CGracenoteInterface;
class QSqlDatabase;

#define LOG_TAG_CDLOOKUP "CGracenoteCDLookup: "

class CGracenoteCDLookup : public QObject
{

    Q_OBJECT
    USE_LOG_UTIL

public:

    CGracenoteCDLookup(QObject* parent = 0);
    ~CGracenoteCDLookup();

    void PerformCDLookup();

    void SetGracenoteInterface(CGracenoteInterface* aInterface);
    void SetTOC(QString tocInfo);

private:
    void ClearTrackerMusicData(TrackerMusicData &trackerMusicFiledata);
    void CreateTrackerMusicDataFromCDLookup(TrackerMusicData &musicData,
                                            gn_palbum_t &albumData);

    void RetrieveCoverArtFromCDLookup(TrackerMusicData &musicData,
                                      gn_palbum_t &albumData);

    void LogError(gn_error_t error);

    bool CreateCDDADatabase();
    bool CreateCDDATables(QSqlDatabase database);
    bool CreateCDDATable(QSqlDatabase db);

    // New API is added to support CR 13474
    QString GetTrackComposer(gn_palbum_track_t &trackData);

public:
    bool InsertCDDAData(TrackerMusicData cddaData);
    void DeleteCDData(QString toc);
private:

    CGracenoteInterface* m_pGracenoteInterface;
    QString m_TocInfo;
	//added by Puneet for fixing GPP crash issue 2013.06.12
    bool createConnection(QString dbPath, QString connectionName, QSqlDatabase &dBHandle);
    void closeConnection(QSqlDatabase &dBHandle); //by aettie for Static Test Defect 201300711
    QSemaphore sem;
};

#endif // DHAVN_GRACENOTECDLOOKUP_H
