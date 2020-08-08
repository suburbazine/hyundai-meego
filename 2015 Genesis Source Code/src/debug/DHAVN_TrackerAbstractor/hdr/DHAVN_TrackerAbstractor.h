/*
******************************************************************
*        COPYRIGHT (C) Teleca AB      *
*----------------------------------------------------------------*
* MODULE     :  DHAVN_TrackerAbstractor                             *
*
* PROGRAMMER :  Chetan Nanda                                   *
* DATE       :  12 September 2011                                    *
* VERSION    :  1.0                                              *
*
*----------------------------------------------------------------*
*                                                                *
* MODULE SUMMARY : This file defines class which interact with    *
*                  different contect providers (e.g. Media, Text Search, Contact)             *
*                  and used by application to retrieve information from respective    *
*                  content provider.           *
*                                                                *
*----------------------------------------------------------------*
*
* MODIFICATION RECORDS :                                         *
*
* VERSION   DATE                     PROGRAMMER              COMMENT
******************************************************************
* 1.0       12 September 2011  Chetan Nanda           Draft version
******************************************************************
*/

#ifndef DHAVN_TRACKERABSTRACTOR_H
#define DHAVN_TRACKERABSTRACTOR_H

#include "DHAVN_TrackerAbstractor_constants.h"
#include <QStringList>
#include <QVector>
#include <QQueue>
#include <QtSql/QSqlDatabase>

/**
   * forward declartion
   */
class CMediaDataProvider;
class CDataLinkedList;
class CCoverArtDataprovider;
class CContactsDataProvider;
class CTrackerDataUpdater;
class CIpodDataProvider;
class QSparqlConnection;
//class CDirListProvider;
class CDirTreeProvider;
class CXSampaInterface;
class QTimer;
class SRequestStateInfo;
class CGracenoteDataProvider; //added by Divya for gracenote normalized db 2013.01.07

/**
   * This class will handle the request from the application and forward it to
   *  respective data provider class. This class exposes the signal that the application
   *  would be calling for any operation like receiving data
   */
class  CTrackerAbstractor  : public QObject
{
	Q_OBJECT

    // USE_LOG_UTIL

public:

    /**
       * Default Constructor
       */
    CTrackerAbstractor();

    /**
       * Default Destructor
       */
    ~CTrackerAbstractor();

    bool RequestData(ETrackerAbstarctor_Query_Ids queryId, uint reqUID = 0);

    /**
       * Calls respective query APIs of Providers
       */
    bool RequestData(QString searchData, ETrackerAbstarctor_Query_Ids queryId, uint reqUID = 0);

    /**
       * Calls respective volume query APIs of Media Provider
       */
	bool RequestData(ETrackerAbstarctor_VolumeType volumeType, ETrackerAbstarctor_Query_Ids queryId, uint reqUID = 0);

	/**
        * Calls respective songs/abums/artist by genre/artist/ablum query APIs of Media Provider
        */
    bool RequestData(QStringList searchList, ETrackerAbstarctor_VolumeType volumeType, ETrackerAbstarctor_Query_Ids queryId, uint reqUID = 0);

	/**
        * Calls all songs with metada volume query APIs of Media Provider
        */
    bool RequestData(ETrackerAbstarctor_VolumeType volumeType, ETrackerAbstarctor_OrderType orderType, ETrackerAbstarctor_Query_Ids queryId, uint reqUID = 0);

    /* This API is for providing Songs list of Multiple Album & Artist Lists*/
    bool RequestData(QStringList searchData,QString FolderName, ETrackerAbstarctor_Query_Ids queryId, uint reqUID = 0);
    /* This API is for providing Thumbnails*/
    bool RequestData(QStringList searchData, ETrackerAbstarctor_Query_Ids queryId, uint reqUID = 0);
    /*This is API for CoverArt Provider As CoverArt For Album requires List of Album Name & its Artist Name */
    bool RequestData(QStringList Argumnet1, QStringList Argumnet2, ETrackerAbstarctor_Query_Ids queryId, uint reqUID = 0);
    /*This is API for track cover art and requires three parameters 1. for album name, 2. for artist name 3. for track name*/
	// bool RequestData(QStringList Argumnet1, QStringList Argumnet2, QStringList Argument3, ETrackerAbstarctor_Query_Ids queryId);
    bool RequestData(QString keyword, QStringList volumeList, bool cdInserted, ETrackerAbstarctor_Query_Ids queryId, uint reqUID = 0);
	bool RequestData(QStringList argument1, QStringList argument2, ETrackerAbstarctor_VolumeType volumeType, ETrackerAbstarctor_Query_Ids queryId, uint reqUID = 0);
	/**
       * Calls respective songs/abums/artist/genre search query APIs of Media Provider
       */
    bool RequestData(QString searchData, ETrackerAbstarctor_VolumeType volumeType, ETrackerAbstarctor_Query_Ids queryId, uint reqUID = 0);
    bool RequestData(QString searchData1, QString searchData2, ETrackerAbstarctor_Query_Ids queryId, uint reqUID = 0);					 
    bool RequestData(int startIndex, int endIndex, ETrackerAbstarctor_Query_Ids queryId, uint reqUID = 0);
    bool RequestData(QStringList albumList, QStringList artistList, QStringList filePaths, ETrackerAbstarctor_Query_Ids queryId, uint reqUID = 0);

	/* @@@ For PhoneBook DB - SQLite @@@ */
	bool RequestPBData(const QString& searchData, const ETrackerAbstarctor_Query_Ids queryId, const uint reqUID = 0);
	bool RequestPBData(const QString& searchData1, const QString& searchData2
						, const ETrackerAbstarctor_Query_Ids queryId, const uint reqUID = 0);
	bool RequestPBData(const QString& bd_address, const BT_PBAP_DB_SORTING sortBy
						, const ETrackerAbstarctor_Query_Ids queryId, const uint reqUID = 0);

	bool RequestPBData(const QString& bd_address
						, const QString& firstName, const QString& lastName
						, const QString& phoneNumber
						, const ETrackerAbstarctor_Query_Ids queryId, const uint reqUID = 0);
/////SUPPORT_MIDDLE_NAME
	bool RequestPBData(const QString& bd_address
						, const QString& firstName, const QString& lastName, const QString& middleName
						, const QString& phoneNumber
						, const ETrackerAbstarctor_Query_Ids queryId, const uint reqUID = 0);

/////SUPPORT_MIDDLE_NAME
    bool RequestPBData(const QString& bd_address, const QString& phoneNumber
    					, const int section, const ETrackerAbstarctor_Query_Ids queryId, const uint reqUID = 0);

	bool RequestPBData(const QString& bd_address
						, const QString& firstName, const QString& lastName
						, const QString& phoneNumber, const int section
						, const ETrackerAbstarctor_Query_Ids queryId, const uint reqUID = 0);

    bool UpdatePBData(void* data, ETrackerAbstarctor_Query_Ids const queryId);

    /*This API is to update the data to the Tracker Database*/
    bool UpdateData(void* data, ETrackerAbstarctor_Query_Ids queryId);
    bool UpdateData(QString updateCriteria, void* data, ETrackerAbstarctor_Query_Ids queryId);

    /* API for Multiple Device Support */

    bool setDeviceId(ETrackerAbstarctor_VolumeType volumeType,QString DeviceId,QString MountPath);

    //Tan 2014.04.17 when request cover art, it should pass the correct front/back serialnumber
    void setActiveIPod(ETrackerAbstarctor_VolumeType volumeType);

    QString getDeviceURL(ETrackerAbstarctor_VolumeType volumeType);

    
    bool createFirstIpodConnection(QString DeviceId);
    bool createSecondIpodConnection(QString DeviceId);
    /*This API is to set a offset variable to specified queries*/
    void setQueryOffset(int offset);
    /*This API is to set a limit variable to specified queries*/
    void setQueryLimit(int limit);


    void RemoveDeviceId(QString deviceId);
	
    //added by Divya for gracenote normalized db 2013.01.07
    /*Set m_GracenoteNormalizedData to true if the user selects Gracenote Normalized Data*/
    void SetGracenoteNormalizedData(bool normalizedDataflag);
    bool GetGracenoteNormalizedData();

    // { added by kihyung 2012.2.17
    void SetAVNSWVersion(QString strVersion);
    void SetFileSystemScanMode(bool bScan);
    bool StartScanFileSystem(ETrackerAbstarctor_VolumeType volumeType, bool bRescanMode = false);
    bool StopScanFileSystem(ETrackerAbstarctor_VolumeType volumeType);
    bool StartReadScanResult(ETrackerAbstarctor_VolumeType volumeType);
    bool StopReadScanResult(ETrackerAbstarctor_VolumeType volumeType);
    int  GetAudioFileNum(ETrackerAbstarctor_VolumeType  volumeType);
    int  GetVideoFileNum(ETrackerAbstarctor_VolumeType  volumeType);
    int  GetPhotoFileNum(ETrackerAbstarctor_VolumeType  volumeType);
    QString GetFirstAudioFolder(ETrackerAbstarctor_VolumeType volumeType);
    QString GetFirstVideoFolder(ETrackerAbstarctor_VolumeType volumeType);
    QString GetFirstPhotoFolder(ETrackerAbstarctor_VolumeType volumeType);    
    // } added by kihyung 2012.2.17
    //{added by junam 2013.05.23 for sorting
    static int GetCountryVariant() { return m_CountryVariant; }
    static void SetCountryVariant(int var) { m_CountryVariant = var; }
    //}added by junam
    //{added by junam 2013.08.21 for iPod sort
    static int GetSortingOrder() { return m_nSortingOrder; }
    static void SetSortingOrder(int var) { m_nSortingOrder = var; }
    //}added by junam
    void setMinerStatus(ETrackerAbstarctor_VolumeType volumeType, bool bFinished);  // added by sungha.choi 2013.07.23
    bool FileExists(QString fullPath, ETrackerAbstarctor_VolumeType volumeType = eDefaultVolume);   // added by sungha.choi 2013.07.31 for scandir performance
    bool GetFSFolderInfo(QList<FSFolderInfo> &tFolderInfo, ETrackerAbstarctor_VolumeType volumeType, ETrackerAbstarctor_MediaType mediaType); // added by kihyung 2013.08.07 for ITS 0181967
    void clearCategoryListCache(ETrackerAbstarctor_VolumeType  volumeType);  // added by sungha.choi 2013.08.21 for ITS 0184905
    static bool getIPodFileUID(QString filePath, quint64 &uid); //added by junam 2013.08.21 for iPod file compare
    void GetMediaEntryList(const QString &strSubDirPath, QList<QStringList> *entryList, ETrackerAbstarctor_VolumeType volumeType = eDefaultVolume);   // added by sungha.choi 2013.09.10
    void GetAudioEntryList(const QString &strSubDirPath, QList<QStringList> *entryList, ETrackerAbstarctor_VolumeType volumeType = eDefaultVolume);   // added by sungha.choi 2013.09.10
    void GetVideoEntryList(const QString &strSubDirPath, QList<QStringList> *entryList, ETrackerAbstarctor_VolumeType volumeType = eDefaultVolume);  // added by sungha.choi 2013.09.10
    void GetPictureEntryList(const QString &strSubDirPath, QList<QStringList> *entryList, ETrackerAbstarctor_VolumeType volumeType = eDefaultVolume);   // added by sungha.choi 2013.09.10

    void FactoryReset(); // added by kihyung 2013.10.02 for factory reset
    void RemoveJBScanResultFile(); // added by honggi.shin, 2014.03.12, for shutdown just after JB format
    static QString GetMediaArt(const QString &artist, const QString &album); //added by junam 2013.11.07 for coverart loading performance

private:

    /*This function is to set a offset variable to requested queries*/
    void setRequestOffset(int offset);
    /*This function is to set a limit variable to requested queries*/
	void setRequestLimit(int limit);
    /* This API is to handle the Query Requests */
    int  RequestHandler(ETrackerAbstarctor_Query_Ids queryId,ETrackerAbstarctor_VolumeType volumeType);
    void processQueuedRequests();
    void QueueRequest();
    void checkQueuedRequests();
    CDirTreeProvider* GetDirTreeProvider();
    bool getMinerStatus(ETrackerAbstarctor_VolumeType volumeType);  // added by sungha.choi 2013.07.23


    QStringList m_DevId;

    //Tan 2014.04.17 when request cover art, it should pass the correct front/back serialnumber
    ETrackerAbstarctor_VolumeType m_CurrentActiveIPodVolume;
    QString m_deviceid_ipod_front;
    QString m_deviceid_ipod_rear;

    QSqlDatabase firstIpodDB;
    QSqlDatabase secondIpodDB;
    /**
      * Creates the Music Album list
      */
    void CreateTrackerDataList(QVector<QStringList> dataList);

signals:

    /**
       * Signal to notify the application on query completion
       */
    void RequesteComplete(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID = 0);
    /**
       * Signal to notify the application on query completion ( added volume type )
       */
    void RequesteComplete(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId,
                                        ETrackerAbstarctor_VolumeType volumeType, uint reqUID = 0);
    /**
       * Signal to notify the application on query completion for dynamic fetching ( added offset )
       */
    void RequesteComplete(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId, int offset,
                                        uint reqUID = 0);

    void Error(int, uint reqUID = 0);
    void RequestState(int, uint reqUID = 0);

    void getCoverArt(QStringList albumList, QStringList artistList,QString deviceid);

    // { added by kihyung 2013.2.17
    void ScanCompleted(ETrackerAbstarctor_VolumeType volumeType, int bRescanMode, int bSuccess);
    void ReadCompleted(ETrackerAbstarctor_VolumeType volumeType, int bSuccess);    
    void MediaFileFound(ETrackerAbstarctor_VolumeType volumeType, int bRescanMode, int audio, int video, int photo);
    // } added by kihyung 2013.2.17

private:

    void HandleError(int nError);
    void RequestAlbumArts(QVector<QStringList> dataList);
    void PrepareDataListForAlbumWithCoverArt(QVector<QStringList> dataList);
    void PrepareDataListForAlbumWithCoverArtWithLimit(QVector<QStringList> dataList, int offset);

private slots:

    /**
       * Slot to get notification from providers on query completion
       */
    void DataReceived(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId);
    /**
       * Slot to get notification from providers on query completion for dynamic fetching
       */
    void DataLimitReceived(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId, int offset);
    void ErrorReceived(int error);
    void RequestTimeOut();
    void DirTreeDataReceived(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID);   // added by sungha.choi 2013.09.09 for ISV 90608
    void DirTreeDataReceived(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType, uint reqUID);   // added by sungha.choi 2013.09.09 for ISV 90608

public slots:

    void onCoverArtsFromIPOD(QStringList coverArts); // rollback
    void getCoverArtSlot(QStringList albumList, QStringList urlList);//change arument by junam 2013.05.16 for coverart match

private:

    /**
       * Media data provider object to request call media related APIs
       */
    CMediaDataProvider* m_MediaDataProvider;

    CCoverArtDataprovider* m_CoverArtDataprovider;

//    CDataLinkedList* m_MediaFolderList;

    /**
       * Tracker data list object to create music album list
       */
//    CDataLinkedList* m_MusicAlbumList;

    /**
       * contacts data provider object to request call contacts related APIs
       */
    CContactsDataProvider* m_ContactsDataProvider;

    /**
      * Tracker data updater object
      */
    CTrackerDataUpdater* m_DataUpdater;
	
	//IPod dataobject
	CIpodDataProvider* m_IpodDataProvider;

        /**
           * QSparqlConnection object to execute query
           */
        QSparqlConnection* m_pSparql_Connection;

 // album list to be used for requesting cover arts
    QStringList m_AlbumNameList;
    bool m_Album_CoverArtRequest;
    bool m_Album_CoverArtRequestWithLimit;
	int  m_Album_CoverArtRequestOffset;
	
	// CDirListProvider* m_DirListProvider; // removed by kihyung 2013.2.17
    CDirTreeProvider* m_DirTreeProvider;
	CXSampaInterface* m_XSampaInterface;
	ETrackerAbstarctor_VolumeType m_VolumeType;
    /**
      * A offset value for specifed queries
      */
    int m_Query_Offset;
    /**
      * A limit value for specifed queries
      */
    int m_Query_Limit;
     /**
       * Request State Info object
       */
    SRequestStateInfo *m_ReqStateInfo;
    QQueue<SRequestStateInfo *> m_waitReqList;
    SRequestStateInfo *m_CurrentReqStateInfo;

    QTimer  *m_QueueRequestTimer;

	//added by Divya for gracenote normalized db 2013.01.07
    bool m_GracenoteNormalizedData;
    CGracenoteDataProvider *m_GracenoteDataProvider;
	QString m_Removed_DeviceId; // added by eugene.seo 2013.04.29
    static int m_CountryVariant;//added by junam 2013.05.23 for sorting
    static int m_nSortingOrder;//added by junam 2013.08.21 for iPod sort

    bool m_jukeBoxMinerFinished, m_usb1MinerFinished, m_usb2MinerFinished;  // added by sungha.choi 2013.07.23
    bool m_isPBData;    // added by sungha.choi 2013.08.25 (PB do not use queue)
    int m_DirTreeQueryId;   // added by sungha.choi 2013.09.09 for ISV 90608
    int m_lang; 

public: 
    void SetLanguage(int lang);  
    //{ added by jaehwan 2014.01.08 for pinyin sorting
    static wchar_t GetFirstChar(QString title, bool bIPodSort);
    static wchar_t GetIndexChar(QString title, bool bIPodSort, bool bPinyinSort);
    static void LoadPinyinTable();
    //} added by jaehwan
};

#endif // DHAVN_TRACKERABSTRACTOR_H
