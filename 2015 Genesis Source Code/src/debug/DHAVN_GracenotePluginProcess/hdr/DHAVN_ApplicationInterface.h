#ifndef DHAVN_APPLICATIONINTERFACE_H
#define DHAVN_APPLICATIONINTERFACE_H

#include <QObject>
#include <DHAVN_LogUtil.h>
#include <QStringList>

// Forward declaration
class CGracenoteRequestQueue;
class GNThread;

class CApplicationInterface : public QObject
{
    Q_OBJECT
    USE_LOG_UTIL

public:
     CApplicationInterface(QString variant, QObject* parent = 0);
    ~CApplicationInterface();
    bool Init();

public Q_SLOTS:
    //API for fetching metadata for CDDA
    void FetchMediaDataFromCDTOCLookup(QString tocInfo);    
    //API for deleting the CDDA data from Tracker Store when CD is ejected
    void DeleteMediaDataForCD(QString tocInfo);

    //Retrieve cover art API for MusicPlayer for BT
    void RetrieveCoverArt(QStringList metaDataList);
    //Retrieve cover arts for multiple songs
    void RetrieveMultipleCoverArts(QStringList albumList, QStringList artistList);

    //API for enabling playlisting only for a particular volume
    void EnableMLTForVolume(int volume, QString serialNum,
                            QString mountPath);
	//added by Puneet for fixing MLT not higlited for JB/USB1/USB2 issue 2013.06.12
    void jukeBoxMLT();
    //API for populating MLDB when USB is plugged
    void USBConnected(QString serialNum, QString mountPath, int volume);
    //API for closing CID when USB unplugged
    void USBDisConnected(QString serialNum, int volume);

    //API called when AppMusicPlayer gets the new IPOD connected signal
    void IPODConnected(QString serialNum, int startIndex, int endIndex, int syncState, int volume);
    //API called when AppMusicPlayer gets the IPOD disconnected signal
    void IPODDisConnected(QString serialNum, int volume);

    //API called when AppMusicPlayer has the highlighted volume tab
    void HighLightedVolume(int volume, QString serialNum, QString mountPath);

    //API called when ACC OFF
    void CloseCollectionIDs();
    
    //void AVOffSignal();

    //API called to perform resync after copy or move 
    void ResyncAfterCopyOperation(int targetVolume);


    //Slots internal to GPP
    //Called when the child thread is started.
    void onChildStarted();
    //Called when XSampa data is updated
    //{modified by esjang 2012.11.14 for XSampa DB update with Volume Info
    //void onXSampaDataUpdated();
    //void onXSampaDataUpdated(int volume); //deleted by aettie Xsampa DB is not used.
    //}modified by esjang 2012.11.14 for XSampa DB update with Volume Info
    //Called when GPP need to exit
    void onExitFromApp();

    void OnGracenoteIndexingStatus(bool status, int deviceType);
    void onGracenoteKilled();
	//{added by aettie.ji 20130904 for gracenote version
    void onGNDBVer(int ver);
    void onGNSDKVer(QString ver);
    void onGNDBRegionVer(QString ver); //20131023 System info
    void GetSDKVer();
    void GetDBVer();
    void GetDBRegionVer(); //20131023 System info
	//}added by aettie.ji 20130904 for gracenote version
    //void RequestEmitSignalForGracenoteLogo(); //deleted by aettie 20131010 unused code

    signals:
    void start(QString variant);

    void fetchCDData(QString tocInfo);
    void deleteCDData(QString tocInfo);

    //Retrieve cover art API for MusicPlayer for BT
    void retrieveCoverArt(QStringList metaDataList);
    void retrieveMultipleCoverArts(QStringList albumList, QStringList artistList);

    void enableMLT(int volume, QString serialNum,
                   QString mountPath);

    void usbConnected(QString serialNum, QString mountPath, int volume);
    void usbDisconnected(QString serialNum, int volume);

    void ipodConnected(QString serialNum, int startIndex, int endIndex,
                       int syncState, int volume);
     void ipodDisConnected(QString serialNum, int volume);

    void highLightedVolume(int volume, QString serialNum, QString mountPath);

    void closeCollections();  

    void resyncAfterCopyOperation(int targetVolume);
	//added by Puneet for fixing MLT not higlited for JB/USB1/USB2 issue 2013.06.12
    void jukeBoxMLTSignal();

    // Signals from GPP to AppMediaPlayer
    //Signal emitted when CD metadata is ready
    void OnMusicIDCDDataReceived(bool status);
    //Signal emitted when CoverArt is retrieved for BT
    void CoverArtRetrieved(bool status, QString imagePath);
    //Signal emitted when XSampa data is ready in SQlite DB
    //{modified by esjang 2012.11.14 for XSampa DB update with Volume Info
    //void XSampaDataUpdated();
    //void XSampaDataUpdated(int volume); //deleted by aettie Xsampa DB is not used.
    //}modified by esjang 2012.11.14 for XSampa DB update with Volume Info
    //Signal emitted when Cover Arts are retrieved
    void CoverArtsRetrieved(QStringList imagePathList);

    // For application to indicate, that indexing has started
    void GracenoteIndexingStatus(bool status, int deviceType);
    void GracenoteKilled();
    //void AVOff();
    // For application to indicate, that GPP is launched CR 13509
    void GracenoteLaunchComplete(bool status);
	//{added by aettie.ji 20130904 for gracenote version
    void GNDBVer(int ver);
    void GNSDKVer(QString ver);
    void GNDBRegionVer(QString ver); //20131023 System info
    
    void getSDKVer();
    void getDBVer();
    void getDBRegionVer(); //20131023 System info
	//}added by aettie.ji 20130904 for gracenote version
    //void requestEmitSignalForGracenoteLogo();  //deleted by aettie 20131010 unused code
private:
    //void CopyXSampaToTmpfs(); //deleted by aettie Xsampa DB is not used.
    //void BackupXsampaToFS(); //deleted by aettie Xsampa DB is not used.

private:
    GNThread* m_pThread;
    CGracenoteRequestQueue* m_pQueue;
    QString m_Variant;
};

#endif // DHAVN_APPLICATIONINTERFACE_H
