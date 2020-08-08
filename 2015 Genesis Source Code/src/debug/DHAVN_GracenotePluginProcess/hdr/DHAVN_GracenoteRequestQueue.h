/*
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * History
 * 01-Aug-2011      GADDAM SATEESH
 ******************************************************************
 */
#ifndef DHAVN_GRACENOTEREQUESTQUEUE_H
#define DHAVN_GRACENOTEREQUESTQUEUE_H

#include <QQueue>
#include <QThread>
#include <QEventLoop>
#include <QDebug>
#include <DHAVN_LogUtil.h>
#include "DHAVN_GracenoteInterface.h"

//Forward declaration
class CTrackerQuery;
class CGracenoteCDLookup;
class CGracenoteCoverArt;
class CPlaylistMLDBCreator;

//enum Priority {
//    JUKE = 0,
//    USB = 2,
//    IPOD = 7,
//    DISC = 8
//};

enum Priority {
    JUKE = 0,
    USB = 0,
    IPOD = 0,
    DISC = 1,
    CA = 2
};
/*struct USBInfo
    
    {
        QString serialNum;
        QString mountPath;
        int volume;
    };*/
/**
  * This class enqueue's and dequeue's all the request of Jukebox,USB,IPOD,CD and CA
     on priority base
  */
class CGracenoteRequestQueue : public QObject
{
    Q_OBJECT
    //USE_LOG_UTIL  //deleted by aettie CLOGUtil fault in GNTread


public:
    CGracenoteRequestQueue();
    ~CGracenoteRequestQueue();

    void Enqueue(EVolumeType type,Priority priority);
    int Count();
    void HVDequeue();
    bool PopulateMLDB(EVolumeType type);
    void RemoveItem(int type);
    void SetAllRequestsInterface(CTrackerQuery *aTQInterface, CGracenoteInterface* aGNInterface);
    bool CheckIfRequestExistsInQueue(EVolumeType type);
    void emitSignalForGracenoteLogo(EVolumeType type, bool status);  //added by aettie.ji 20130904 for gracenote logo
signals:
    void DeQueueStart();

    void OnMusicIDCDDataReceived(bool status);
    void CoverArtRetrieved(bool status, QString imagePath);
    void CoverArtsRetrieved(QStringList imagePathList);
    void ExitFromApp();

    void GracenoteIndexingStatus(bool status, int deviceType);
    void GracenoteKilled(); //{added by aettie for Gracenote killed handling
	//added by aettie.ji 20130904 for gracenote version
    void GNSDKVer(QString ver);
    void GNDBVer(int ver);
    void GNDBRegionVer(QString ver); //20131023 System info

	// New signal added for CR 13509
    void GracenoteLaunchComplete(bool status);
    
private Q_SLOTS:
    void KeepOnDequeue();

    void onStart(QString variant);

    void onFetchCDData(QString tocInfo);
    void onDeleteCDData(QString tocInfo);

    void onRetrieveCoverArt(QStringList metaDataList);

    void onEnableMLT(int volume, QString serialNum,
                   QString mountPath);

    void onUsbConnected(QString serialNum, QString mountPath, int volume);
    void onIPodConnected(QString serialNum, int startIndex, int endIndex,
                                        int syncState, int volume);

    void onHighLightedVolume(int volume, QString serialNum, QString mountPath);

    void onUsbDisconnected(QString serialNum, int volume);
    void onIPodDisConnected(QString serialNum, int volume);

    void onCloseCollections();

    void onRetrieveMultipleCoverArts(QStringList albumList, QStringList artistList);

    void onTimerFinished();

    void onResyncAfterCopyOperation(int targetVolume);
    void onjukeBoxMLTSignal(); 	//added by Puneet for fixing MLT not higlited for JB/USB1/USB2 issue 2013.06.12
	//added by aettie.ji 20130904 for gracenote version
    void onGetSDKVer();
    void onGetDBVer();
    void onGetDBRegionVer(); //20131023 System info
    //void onRequestEmitSignalForGracenoteLogo();  //deleted by aettie 20131010 unused code

private:    
    CGracenoteInterface *m_pGracenoteInterface;

    struct Event
    {
        Priority ReqPriority;
        EVolumeType ReqType;

        Event(EVolumeType type, Priority priority)
        {
            ReqPriority = priority;
            ReqType = type;
        }
    };

    QQueue <Event> m_RequestQueue;

};


#endif // DHAVN_GRACENOTEREQUESTQUEUE_H
