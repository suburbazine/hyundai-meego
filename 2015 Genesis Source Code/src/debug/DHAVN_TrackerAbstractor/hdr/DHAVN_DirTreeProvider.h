#ifndef DHAVN_DIRTREEPROVIDER_H
#define DHAVN_DIRTREEPROVIDER_H

#include <QStringList>
#include <QVector>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include "DHAVN_TrackerAbstractor_constants.h"
#include <DHAVN_LogUtil.h>
#include <QMetaType>

Q_DECLARE_METATYPE(QVector<QStringList>);

enum DT_CATEGORY
{
	eDT_AUDIO = 0x00,
	eDT_VIDEO = 0x01,
 	eDT_PHOTO = 0x02,
 	eDT_NUM   = 0x03
};

#define DT_FS_FIND_MODE     0
#define DT_FS_SCAN_MODE     1
#define DT_FS_READ_MODE     2


class CDTDirInfo
{
public:
    CDTDirInfo()
    {
        strName                     = QString();
        nParentDirIdx               = 0;
        nSubDirIdx                  = 0;
        nSubDirNum                  = 0;
        pFileIdx[eDT_AUDIO]         = 0;
        pFileIdx[eDT_VIDEO]         = 0;
        pFileIdx[eDT_PHOTO]         = 0;
        pFileNum[eDT_AUDIO]         = 0;
        pFileNum[eDT_VIDEO]         = 0;
        pFileNum[eDT_PHOTO]         = 0;
        pIsValid[eDT_AUDIO]         = 0;
        pIsValid[eDT_VIDEO]         = 0;
        pIsValid[eDT_PHOTO]         = 0;
        nIsValid                    = 1;
    }

    QString strName;
    int     nParentDirIdx;
    int     nSubDirIdx;
    int     nSubDirNum;
    int     pFileIdx[eDT_NUM];
    int     pFileNum[eDT_NUM];
    int     pIsValid[eDT_NUM];
    int     nIsValid;
};

class CDTFileInfo
{
public:
    CDTFileInfo() 
    {
        strName       = QString();
        nParentDirIdx = 0;
        nIsValid      = 1;
    }

    QString strName;
    int     nParentDirIdx;
    int     nIsValid;
};

class CDirTree
{
    USE_LOG_UTIL
public:
    CDirTree();
    ~CDirTree();

    void            Init();

    int             GetDirNum();
    int             GetAudioFileNum();
    int             GetVideoFileNum();
    int             GetPhotoFileNum();
    int             GetMediaFileNum();

    QString         GetFirstMediaFolder(DT_CATEGORY eCategory);

    bool            AddDir(CDTDirInfo &tDirInfo);
    bool            AddAudioFile(CDTFileInfo &tFileInfo);
    bool            AddVideoFile(CDTFileInfo &tFileInfo);
    bool            AddPhotoFile(CDTFileInfo &tFileInfo);

    CDTDirInfo&     GetDirInfo(int nIdx);
    CDTFileInfo&    GetAudioFileInfo(int nIdx);
    CDTFileInfo&    GetVideoFileInfo(int nIdx);
    CDTFileInfo&    GetPhotoFileInfo(int nIdx);

    void            PrintDirTree();
    bool            Read(const char *pFileName, int nTreeMode = DT_FS_READ_MODE);
    bool            Write(const char *pFileName);
    
    void            SetHashTable(QHash<QString, int> *pFileTable);   // added by sungha.choi 2013.07.31 for scandir performance
    bool            GetFSFolderInfo(QList<FSFolderInfo> &tFolderList, ETrackerAbstarctor_MediaType mediaType); // added by kihyung for ITS 0181967 
    
private:
    int                m_nPlayDirIdx;
    int                m_nPlayFileIdx;
    QList<CDTDirInfo>  m_tDirList;
    QList<CDTFileInfo> m_tAudioFileList;
    QList<CDTFileInfo> m_tVideoFileList;
    QList<CDTFileInfo> m_tPhotoFileList;

    CDTDirInfo         m_tNullDirInfo;
    CDTFileInfo        m_tNullFileInfo;

    QHash<QString, int> *m_pFileTable;   // added by sungha.choi 2013.07.31 for scandir performance
};

class CDirTreeThread : public QThread
{
	Q_OBJECT
    USE_LOG_UTIL
public :
    CDirTreeThread(ETrackerAbstarctor_VolumeType volumeType, QObject *parent = 0);
    ~CDirTreeThread();

    void            SetFileSystemScanMode(bool bScan);

    bool            StartScanFileSystem(QString strRootPath, bool bRescanMode = false);
    bool            StopScanFileSystem();
    bool            StartReadScanResult();
    bool            StopReadScanResult();

    int             GetAudioFileNum();
    int             GetVideoFileNum();
    int             GetPhotoFileNum();

    QString         GetFirstMediaFolder(DT_CATEGORY eCategory);

    int             GetAllAudioFileList(uint nReqID);
    int             GetAllVideoFileList();
    int             GetAllPhotoFileList();

    void            GetMediaEntryList(const QString &strSubDirPath, QList<QStringList> *entryList); // added by sungha.choi 2013.09.10
    void            GetAudioEntryList(const QString &strSubDirPath, QList<QStringList> *entryList); // added by sungha.choi 2013.09.10
    void            GetVideoEntryList(const QString &strSubDirPath, QList<QStringList> *entryList); // added by sungha.choi 2013.09.10
    void            GetPictureEntryList(const QString &strSubDirPath, QList<QStringList> *entryList); // added by sungha.choi 2013.09.10

    int             GetSubAudioFileList(const QString &strSubDirPath, uint nReqID);
    int             GetSubVideoFileList(const QString &strSubDirPath);
    int             GetSubPhotoFileList(const QString &strSubDirPath);

    bool            GetVideoFileListWithSubFolder(const QString &strSubDirPath);
    bool            GetPhotoFileListWithSubFolder(const QString &strSubDirPath);

    bool            GetVideoFileListByFolder(const QString &strSubDirPath);
    bool            GetPhotoFileListByFolder(const QString &strSubDirPath);

    // { added by kihyung 2013.07.23 for ITS 0178868
    bool            GetFirstImagePathByGivenFolder(const QString &strSubDirPath);
    bool            GetImageCountByGivenFolder(QString &strSubDirPath);
    bool            GetFirstImageAndCountByGivenFolder(QString &strSubDirPath);
    // } added by kihyung 2013.07.23 for ITS 0178868
    
    bool            FileExists(QString fullPath);   // added by sungha.choi 2013.07.31 for scandir performance
    bool            GetFSFolderInfo(QList<FSFolderInfo> &tFolderList, ETrackerAbstarctor_MediaType mediaType);  // added by kihyung for ITS 0181967 
        
    QString         GetMountPath();
    
protected:
	void            run();

signals:
    void            DataDirectFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, uint);
    void            DataDirectFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, ETrackerAbstarctor_VolumeType, uint);
    void            DataFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids);
    void            Error(int); 
    void            ScanCompleted(ETrackerAbstarctor_VolumeType, int, int);
    void            ReadCompleted(ETrackerAbstarctor_VolumeType, int);
    void            MediaFileFound(ETrackerAbstarctor_VolumeType volume, int bRescanMode, int audio, int video, int photo);
    
private:
    void            Init(); // added by kihyung 2013.07.22 for ITS 0180971
    
    void            NotifyMediaFileFound(CDirTree &tDirTree, bool bEnd = false);
    void            GetVideoFileListWithSubFolder_Impl(const QString &strSubDirPath, QVector<QStringList> &queryDataList);
    void            GetPhotoFileListWithSubFolder_Impl(const QString &strSubDirPath, QVector<QStringList> &queryDataList);   
    
    void            StartWait();
    void            EndWait();
    bool            StartScanDir();
    bool            StartReadDir();
    bool            ScanDir(CDirTree &tDirTree, QString strCurrPath, int nCurrDirIdx, int nCurrDirDepth);
    bool            ScanDirGLib(CDirTree &tDirTree, QString strCurrPath, int nCurrDirIdx, int nCurrDirDepth);
    bool            ScanDirBreadthFirst(CDirTree &tDirTree, int nRootDirIdx);        
    bool            ScanDirBreadthFirstGLib(CDirTree &tDirTree, int nRootDirIdx);
    bool            ScanDirBreadthFirstNameSortGLib(CDirTree &tDirTree, int nRootDirIdx);   // added by sungha.choi 2013.07.17
    bool            ScanDirBreadthFirstPosix(CDirTree &tDirTree, int nRootDirIdx);  // added by sungha.choi 2013.07.13
    bool            CheckMediaValidFlag(int &nAFileNum, int &nVFileNum, int &nPFileNum, CDirTree &tDirTree, int nRootDirIdx);

    bool            CheckInternalRead();
    void            ReadDirInSelfThread();   // added by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535

    QStringList     m_strAudioFmt;
    QStringList     m_strVideoFmt;
    QStringList     m_strAudioVideoFmt; // added by eugene.seo 2013.03.15
    QStringList     m_strPhotoFmt;
    QStringList     m_strIgnoreDir; // added by honggi.shin, 2014.03.10, for ignoring UNSUPPORTED directory.

    int             m_nTreeMode;
    int             m_bRescanMode;
    int             m_bDirTreeScanMode;
    int             m_bScanReadComplete;
    int             m_bAudioFound;
    int             m_bVideoFound;
    int             m_bPhotoFound;
    int             m_bAllMediaFound;
    QString         m_strRootPath;
    CDirTree        m_tDirTree;
    QHash<QString, int> m_tFileTable;   // added by sungha.choi 2013.07.31 for scandir performance
    
    int             m_bStop;
    int             m_nWorking;
    QWaitCondition  m_tCond;
    QMutex          m_tMutex;
    QMutex          m_tStopMutex;

    ETrackerAbstarctor_VolumeType m_volumeType;

    QString         m_strMountPath;
};

class CDirTreeProvider : public QObject
{
    Q_OBJECT
    USE_LOG_UTIL
public :
    static CDirTreeProvider*   GetInstance();
    static bool                IsDirTreeQuery(ETrackerAbstarctor_Query_Ids queryID);
    
                    ~CDirTreeProvider();
    void            SetSWVersion(QString strVersion);                    
    void            SetFileSystemScanMode(bool bScan);

    bool            StartScanFileSystem(ETrackerAbstarctor_VolumeType volumeType, bool bRescanMode = false);
    bool            StopScanFileSystem(ETrackerAbstarctor_VolumeType volumeType);
    bool            StartReadScanResult(ETrackerAbstarctor_VolumeType volumeType);
    bool            StopReadScanResult(ETrackerAbstarctor_VolumeType volumeType);

    int             GetAudioFileNum(ETrackerAbstarctor_VolumeType  volumeType);
    int             GetVideoFileNum(ETrackerAbstarctor_VolumeType  volumeType);
    int             GetPhotoFileNum(ETrackerAbstarctor_VolumeType  volumeType);

    QString         GetFirstAudioFolder(ETrackerAbstarctor_VolumeType volumeType);
    QString         GetFirstVideoFolder(ETrackerAbstarctor_VolumeType volumeType);
    QString         GetFirstPhotoFolder(ETrackerAbstarctor_VolumeType volumeType);

    bool            GetAllAudioFileList(ETrackerAbstarctor_VolumeType volumeType, uint reqUID = 0);
    bool            GetAllVideoFileList(ETrackerAbstarctor_VolumeType volumeType);
    bool            GetAllPhotoFileList(ETrackerAbstarctor_VolumeType volumeType);

    bool            GetSubAudioFileList(const QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType = eDefaultVolume, uint reqUID = 0);
    bool            GetSubVideoFileList(const QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType = eDefaultVolume);
    bool            GetSubPhotoFileList(const QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType = eDefaultVolume);

    void            GetMediaEntryList(const QString &strSubDirPath, QList<QStringList> *entryList, ETrackerAbstarctor_VolumeType volumeType); // added by sungha.choi 2013.09.10
    void            GetAudioEntryList(const QString &strSubDirPath, QList<QStringList> *entryList, ETrackerAbstarctor_VolumeType volumeType); // added by sungha.choi 2013.09.10
    void            GetVideoEntryList(const QString &strSubDirPath, QList<QStringList> *entryList, ETrackerAbstarctor_VolumeType volumeType); // added by sungha.choi 2013.09.10
    void            GetPictureEntryList(const QString &strSubDirPath, QList<QStringList> *entryList, ETrackerAbstarctor_VolumeType volumeType); // added by sungha.choi 2013.09.10

    bool            GetVideoFileListWithSubFolder(const QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType = eDefaultVolume);
    bool            GetPhotoFileListWithSubFolder(const QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType = eDefaultVolume);

    bool            GetVideoFileListByFolder(const QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType = eDefaultVolume);
    bool            GetPhotoFileListByFolder(const QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType = eDefaultVolume);

    // { added by kihyung 2013.07.23 for ITS 0178868
    bool            GetFirstImagePathByGivenFolder(const QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType = eDefaultVolume);
    bool            GetImageCountByGivenFolder(QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType = eDefaultVolume);
    bool            GetFirstImageAndCountByGivenFolder(QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType = eDefaultVolume);
    // } added by kihyung 2013.07.23 for ITS 0178868
    
    bool            FileExists(QString fullPath, ETrackerAbstarctor_VolumeType volumeType);   // added by sungha.choi 2013.07.31 for scandir performance
    bool            GetFSFolderInfo(QList<FSFolderInfo> &tFolderInfo, ETrackerAbstarctor_VolumeType volumeType, ETrackerAbstarctor_MediaType mediaType); // added by kihyung 2013.08.07 for ITS 0181967 

    void            RemoveScanResultFile(ETrackerAbstarctor_VolumeType volumeType); // added by kihyung 2013.10.02 for factory reset

public slots:    
    void            OnScanCompleted(ETrackerAbstarctor_VolumeType, int, int);
    void            OnReadCompleted(ETrackerAbstarctor_VolumeType, int);
    void            OnMediaFileFound(ETrackerAbstarctor_VolumeType volume, int bRescanMode, int audio, int video, int photo);
    
signals:
    void            DataDirectFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, uint);
    void            DataDirectFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, ETrackerAbstarctor_VolumeType, uint);
    void            DataFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids);
    void            Error(int);      
    void            ScanCompleted(ETrackerAbstarctor_VolumeType, int, int);
    void            ReadCompleted(ETrackerAbstarctor_VolumeType, int);
    void            MediaFileFound(ETrackerAbstarctor_VolumeType volume, int bRescanMode, int audio, int video, int photo);    

private:
    CDirTreeProvider(QObject *parent = 0);

    ETrackerAbstarctor_VolumeType   GetVolumeType(const QString &strSubDirPath);

    static CDirTreeProvider *m_pInstance;
    
    CDirTreeThread  *m_pJukebox;
    CDirTreeThread  *m_pUSB1;
    CDirTreeThread  *m_pUSB2;
};

#endif
