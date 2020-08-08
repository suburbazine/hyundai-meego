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

#ifndef GRACENOTE_INTERFACE_H
#define GRACENOTE_INTERFACE_H

#include <QObject>
//#include <QMutex> //added by aettie 20130518 ISV 83208

#include <gn_textid_file_data.h>
#include <gn_playlist.h>
#include <gn_abs_types.h>
#include <gn_init_emms.h>
#include <gn_coverart.h>
#include <QStringList>
#include <QVector>
#include <DHAVN_TrackerMusicData.h>
#include <playlistresults.h>
#include <DHAVN_LogUtil.h>
#include <DHAVN_GracenotePlaylistMLDBCreator.h>
#include "DHAVN_NormalizedDataUtil.h" //added by Divya 2012.01.07 for gracenote normalized db
#include <signal.h>
#include <gn_lists_variations.h> //20131023 System info


// Constants
#define MLDB_Max_Size 20 * 1024 * 1024

//Forward declaration
class CTrackerQuery;
class CGracenoteOperationUtil;
class CGracenoteCDLookup;
class CGracenoteCoverArt;
class CGracenoteRequestQueue;
//class CPlaylistMLDBCreator; //added by aettie 20130518 ISV 83208
class GNThread;
class QImage;

enum EVolumeType
{
    e_JukeBox = 0,
    e_USB1,  //USB connected in front
    e_USB2,  //USB connected in rear
    e_Disc,
    e_IPOD1, //IPOD connected in front
    e_IPOD2, //IPOD connected in rear
    e_CA,
    e_MultiCA //added by aettie 2013.03.19 for gracenote issue
    //,e_USB1_DISCONN,  //USB connected in front
    //e_USB2_DISCONN,  //USB connected in rear
    //e_Disc_EJECT,
    //e_USB1_PRECONN,  //USB connected in front
    //e_USB2_PRECONN,  //USB connected in rear

};

/*
 *This structure stores the information of IPODCOntroller signals
 * about IPOD track data being added in the SQLite DB chunk by chunk.
 */
struct IPOD_Info
{
    QString iPODSerialNum;
    int startIndex;
    int endIndex;
    int syncState;
};

/*
 *This enum signifies the execution status of IPOD data. i.e. IPOD track data
 *being added into MLDB
 */
enum EipodExecutionStatus
{
    e_Unplugged = 0,
    e_Finished,
    e_InProgress,
    e_Error
};

extern int IPOD1ConnectStatus;
extern int IPOD2ConnectStatus;

extern int USB1ConnectStatus;
extern int USB2ConnectStatus;

//Number for MLDBs to be maintained in the system for USB
extern int USB_MLDB_count;
//Number for MLDBs to be maintained in the system for IPOD
extern int IPOD_MLDB_count;

/**
  * This class starts and shuts down the EMMS system, Performs TextID and
  * CD lookup, retrieves CoverArt, and populates the metadata into the MLDB
  * and calls APIs to update in Tracker Store.
  */
class CGracenoteInterface : public QObject
{
    Q_OBJECT
    USE_LOG_UTIL

public:
    CGracenoteInterface(QString appPath, QObject *parent = 0);
    ~CGracenoteInterface();

    static CGracenoteInterface *m_pInstance;
    bool Init();

    bool EMMSInit(QString countryVariant);
    bool EMMSShutDown();
    
    void IPODConnected(QString serialNum, int startIndex, int endIndex, int syncState, int volume);
    void UpdateMLDBWithIPODData(QStringList list, gn_collection_id_t nCID);
    EipodExecutionStatus AddIPODDataToGraceNoteMLDB(QString iPODSerialNum, int startIndex, int endIndex, int volume);
    void DisablePlaylistingForIPOD(QString iPODSerialNum, int volume);
    void IPODDisConnected(QString serialNum, int volume);

    void StartUSBPopulation(EVolumeType type);
    void USBConnected(QString serialNum, QString mountPath, int volume);
    void USBDisConnected(QString serialNum, int volume);

    void StartJukeboxPopulation();
    TrackerMusicData AddMusicFileEntryToGraceNoteMLDB(QString fileNamePath, gn_collection_id_t nCID);
    void DeleteMusicFileEntryFromGracenoteMLDB(QStringList filesList, int nvolume, QString serialNum = QString());

     void InsertCoverArt(TrackerMusicData &trackerMusicFiledata, QImage &qimage);

    void UpdateMLDBWithCDData(TrackerMusicData &musicData);
    void FetchMediaDataFromCDTOCLookup(QString tocInfo);
    void DeleteMediaDataForCD(QString tocInfo);
    void DisablePlaylistingForCD();
    void OnCDLookupComplete(TrackerMusicData musicData);
    void PerformCDLookupForTOC();
    
    void HighLightedVolume(int volume, QString serialNum, QString mountPath);

    void EnableMLTForVolume(int volume, QString serialNum, QString mountPath);

    void CloseCollectionIDs();


    //void AVOffHandling();
    //Retrieve cover arts for multiple songs
    void RetrieveMultipleCoverArts(QStringList albumList, QStringList artistList);

    //Retrieve cover art API for MusicPlayer for BT
    void RetrieveCoverArt(QStringList metaDataList);

    // Function to emit xsampa updation signal
    //{modified by esjang 2012.11.14 for XSampa DB update with Volume Info
    //void XSampaDataUpdatedInDB();
    //void XSampaDataUpdatedInDB(int volume); //deleted by aettie Xsampa DB is not used.
    //}modified by esjang 2012.11.14 for XSampa DB update with Volume Info

    //Clears the signal list queue when the IPOD is unplugged
    void ClearSignalList(int volume);

    //Updates the list index as and when the signal chunks are processed
    void UpdateIPODListIndex(int volume);

    gn_collection_id_t GetCollectionId(int nvolume, QString serialNum = QString());

    void Start();	
	
    void RetrieveMultipleCA();
	
    void RetrieveCA(); //added by aettie 2013.03.19 for gracenote issue
	
    EipodExecutionStatus PerformResyncForIPOD(QString serialNum, int volume);

    void ResyncAfterCopyOperation(int destinationVolume);


    //Setter Methods

    //Adds the IPODController signal information into a list
    void SetIPODinfo(QString serialNum, int startIndex, int endIndex, int syncState, int volume);

    void SetQueueInterface(CGracenoteRequestQueue* interface);


    //Getter Methods

    CPlaylistMLDBCreator* GetMLDBCreator();

    //Gets the stored IPODController signal information from list
    void GetIPODinfo(QString &serialNum,int &startIndex, int &endIndex, int &syncState, int volume);

    //Gets the total number of signals received in chunks for volume e_IPOD1 or e_IPOD2
    int GetIPODSignalCount(int volume);

    //Gets the current signal index to be processed for volume e_IPOD1 or e_IPOD2
    int GetIPODListIndex(int volume);

    //Copy XSampaDB to FileSystem
    //void BackupXsampaToFS(); //deleted by aettie Xsampa DB is not used.
	CNormalizedData* GetNormalizedDataObject(); //added by Divya 2012.01.07 for gracenote normalized db


    static void signalHandelr(int signum);

    static void setSignalHandlerObject(CGracenoteInterface *m);

    // Watch Dog
    void signalConnection();
    void restart();

//added by aettie.ji 20130904 for gracenote version
    void GetSDKVer();
    void GetDBVer();
    void GetDBRegionVer(); //20131023 System info

public Q_SLOTS:
    void onCoverArtRetrieved(bool status, QString imagePath);
Q_SIGNALS: // SIGNALS
    void OnMusicIDCDDataReceived(bool status);
    void CoverArtRetrieved(bool status, QString imagePath);
    //{modified by esjang 2012.11.14 for XSampa DB update with Volume Info
    //void XSampaDataUpdated();
    //void XSampaDataUpdated(int volume); //deleted by aettie Xsampa DB is not used.
    //}modified by esjang 2012.11.14 for XSampa DB update with Volume Info
    void CoverArtsRetrieved(QStringList imagePathList);

    void GracenoteIndexingStatus(bool start, int deviceType);
    void GracenoteKilled();
	//added by aettie.ji 20130904 for gracenote version
    void GNSDKVer(QString ver);
    void GNDBVer(int ver);
    void GNDBRegionVer(QString ver); //20131023 System info
    
private:
    void CreateTrackerMusicDataFromTextIDLookup(TrackerMusicData &trackerMusicFiledata, gn_pfile_data_t &fileData);

    void ClearTrackerMusicData(TrackerMusicData &trackerMusicFiledata);

    bool PopulateMLDB(QString sfileNamePath, gnplaylist_entrydata_t &entryData, gn_collection_id_t nCID); 
    bool PopulateMLDBWithID3Info(QString sfileNamePath, gn_pfile_data_t &fileData, gn_collection_id_t nCID); 

    TrackerMusicData PerformTextIDLookup(QString sfileNamePath, gn_pfile_data_t &fileData, gn_collection_id_t nCID, bool populateMLDBFlag = true); //modified by Divya 2012.01.07 for gracenote normalized db

    bool CheckIfEntryExistsInMLDB(QString sfilePath, gn_collection_id_t nCID, QString fileName = QString());

    bool CheckConfidence(gn_textid_match_source_t sourceType,
                         gn_textid_match_type_t matchType,
                         int index, gn_pfile_data_t &fileData,
                         gn_textid_presult_data_t &displayResult);

    void SetListWithCDData(TrackerMusicData &musicData, QStringList &cdDataList, int index);
	//deleted by aettie Xsampa DB is not used.
    //void ExtractXSampaData(TrackerMusicData &musicData, gn_textid_presult_data_t &displayResult, int nCID);

    void LogError(gn_error_t error);

	EipodExecutionStatus DeleteOrUpdateEntriesFromIPodMLDB(int volume, int collectionId,
                                                                       QVector<QStringList>& dataList,
                                                                       bool isDelete);
    
private:
    //EMMS Configuration

    //gn_emms_configuration_t m_emmsConfig;       // removed by aettie.ji 2012.10.30 for static analysis #253

    CTrackerQuery *m_pTrackerQuery;
    CGracenoteOperationUtil* m_pGNOperationUtil;
    CPlaylistMLDBCreator* m_pMLDBCreator;

    CGracenoteCDLookup* m_pGNCDLookup;
    CGracenoteCoverArt* m_pGNCoverArt;

    CGracenoteRequestQueue *m_pQueue;
    GNThread *m_pGNThread;

    int m_CoverRequestCount;
    QStringList m_CoverArtList;
    bool m_IsMultipleCoverRequest;

    //Store IPOD Info for e_IPOD1
    QList<IPOD_Info> m_FrontIPODInfo;
    //Total number of signals received chunk by chunk for front IPOD
    int m_FrontIPODSignalCount;
    //The index of the list of signals processed
    int m_FrontIPODSignalIndex;
	
    QVector<QStringList> m_FrontNewList;
    QVector<QStringList> m_FrontModifiedList;
    QVector<QStringList> m_FrontDeletedList;
		
    //Store IPOD Info for e_IPOD2
    QList<IPOD_Info> m_RearIPODInfo;

    //Total number of signals received chunk by chunk for rear IPOD
    int m_RearIPODSignalCount;
    //The index of the list of signals processed
    int m_RearIPODSignalIndex;

    QVector<QStringList> m_RearNewList;
    QVector<QStringList> m_RearModifiedList;
    QVector<QStringList> m_RearDeletedList;
    CNormalizedData *m_NormalizedData; //added by Divya 2012.01.07 for gracenote normalized db

};

#endif // GRACENOTE_INTERFACE_H
