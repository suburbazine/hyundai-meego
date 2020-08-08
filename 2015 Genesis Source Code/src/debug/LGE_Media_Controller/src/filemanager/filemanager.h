#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QSystemStorageInfo>
#include <QFileInfo>
#include <QTime>
#include <QStringList>
#include <QMap>
#include <QQueue>
#include <DHAVN_TrackerAbstractor.h>    // added by sungha.choi 2013.09.10


#include "ifileinterface.h"

using namespace QtMobility;

class IPlayerEngine;

#define FM_SLOW_COPY_SPEED      10   // modified by kihyung 2013.10.08 for ITS 0193747 
#define FM_NORMAL_COPY_SPEED    5
#define FM_USE_SAME_FILE_EVENT  0

//4/////////////////////////////////////////////////////////////////////
//4//
class FileManagerThread : public QThread
{
    Q_OBJECT
public:
    enum FileCommand {
        FC_CREATE_FOLDER,
        FC_RENAME_FOLDER,
        FC_RENAME_FILE,
        FC_DELETE_FILE_FOLDER,
        FC_MOVE_FOLDER,
        FC_COPY_FOLDER,
        FC_DELETE_LIST,
        FC_MOVE_LIST,
        FC_COPY_LIST,
        FC_DISC_FORMAT,
        FC_NONE
    };

    enum CopyMode {
        FC_COPY_MUSIC,
        FC_COPY_VIDEO,
        FC_COPY_IMAGE,
        FC_COPY_ALL
    };

    enum MediaType {
        FC_MEDIA_VIDEO,
        FC_MEDIA_AUDIO,
        FC_MEDIA_IMAGE,
        FC_MEDIA_NONE
    };

    class FMTask
    {
    public:
        FMTask()
        {
            eCmd      = FC_NONE;
            eCopyMode = FC_COPY_ALL;
            eVolume   = eUSB;//TAIHYUN.AHN 2013.10.06
        }

        FileCommand     eCmd;
        CopyMode        eCopyMode;
        ETAVolumeType   eVolume;
        QString         strArg1;
        QString         strArg2;
        QStringList     strList;
    };
                               ~FileManagerThread();
    static FileManagerThread*   GetInstance();

    void                        SetPlayerEngine(IPlayerEngine *pPE);

    int                         StartCreateFolder(const QString &strFolderName);
    int                         StartRenameFolder(const QString &strOldName, const QString &strNewName);
    int                         StartRenameFile(const QString &strOldName, const QString &strNewName);
    int                         StartDeleteFileFolder(const QString &strDeleteName, CopyMode nMode = FC_COPY_ALL, ETAVolumeType volume = eJukeBox);
    int                         StartMoveFolder(const QString &strFolderName, const QString &strDestPath, CopyMode nMode = FC_COPY_ALL);
    int                         StartCopyFolder(const QString &strFolderName, const QString &strDestPath, CopyMode nMode = FC_COPY_ALL, ETAVolumeType volume = eUSB);
    int                         StartDeleteList(const QStringList &strList, CopyMode nMode = FC_COPY_ALL, ETAVolumeType volume = eJukeBox);
    int                         StartMoveList(const QStringList &strList, const QString &strDestPath, CopyMode nMode = FC_COPY_ALL);
    int                         StartCopyList(const QStringList &strList, const QString &strDestPath, CopyMode nMode = FC_COPY_ALL, ETAVolumeType volume = eUSB);
    int                         StartDiscFormat(const QString &strVolumeName);
    
    int                         GetTotalFileNum();
    qint64                      GetTotalFileSpace();
    qint64                      GetTotalDiscSpace(const QString &strVolumeName);
    qint64                      GetTotalUsedDiscSpace(const QString &strVolumeName);

    int                         CancelCopy();
    int                         CancelDelete();
    int                         CancelFormat();
    int                         SetOverWriteCurrentExistFileMode(int event);
    int                         ContinueCopying();
    int                         PauseCopying();
    int                         WaitCopying(int bErrSignal = false, QString strFileName = QString());
    
    void                        SetCopySpeed(int nSpeed); // nSpeed: 0, 1, 2 ~ 
protected:
    void                        run();

signals:
    void                        OnPercentageAndSizeCopied(int nIndex, qint64 nSize);
    void                        OnPercentageAndSizeMoved(int nIndex, qint64 nSize);
    void                        OnFileCopiedPercentage(int nIndex, int nPercentage);
    void                        OnFileMovedPercentage(int nIndex, int nPercentage);
    void                        OnPercentageAndSizeDeleted(int nIndex, qint64 nSize);
    void                        OnPercentageAndSizeAndNameDeleted(int nIndex, qint64 nSize, QString strName);
    void                        OnCreateFinished(int bSuccess);
    void                        OnRenameFinished(int bSuccess, int bFolder);
    void                        OnDeleteFinished(int bSuccess, int nMode, int nVolume);
    void                        OnMoveFinished(int bSuccess, int nMode);
    void                        OnCopyFinished(int bSuccess, int nSkipCount, int nFilesCount, int nMode, int nVolume); // modified by ravikanth 22-04-13
    void                        OnFormatFinished(int bSuccess);
    void                        OnError(int nError, QString strFileName = QString());
    void                        OnFileMoved(QString strFileName);
    
private:
                                FileManagerThread(QObject *parent = 0);                            
    void                        InitStart();
    int                         CreateFolder(const QString &strFolderPath);
    int                         RenameFolder(const QString &strOldName, const QString &strNewName);
    int                         RenameFile(const QString &strOldName, const QString &strNewName);
    int                         DeleteFileFolder();
    int                         DeleteFileFolder_Impl(const QString &strDeleteName);
    int                         DeleteImageThumbnail_Impl(const QString &strDeleteName); // modified on 18-02-14
    int                         MoveFileFolder();
    int                         MoveFolder(const QString &strFolderName, const QString &strDestPath);
    int                         MoveFile(QFileInfo &tFileInfo, const QString &strDestPath);
    int                         CopyFileFolder();
    int                         CopyFolder(const QString &strFolderName, const QString &strDestPath, int &supportingFileCount);
    int                         CopyFile(QFileInfo &tFileInfo, const QString &strDestPath, int supportingFileCount = 0);
    int                         DeleteFileFolderList();
    int                         MoveFileFolderList();
    int                         CopyFileFolderList();
    
    int                         FormatDisc(const QString &strVolumeName);
    int                         DeleteFolder(const QString &strDeleteName);

    int                         CopyFile_Impl(int bCopy, const QString &strSrcFile, const QString &strDstFile, qint64 nFileSize, int skipCount = 0);

    int                         CalcTotalFileNum(const QString &strFolderPath, bool bHidden);
    int                         CalcTotalFileNumForCopy(const QString &strFolderPath); // added by sungha.choi 2013.09.10

    int                         CheckAvailDiscSize(const QString &strDestPath);
    MediaType                   GetMediaType(const QString &strFileName);
	QString                     GetFormat(const QString &strFileName); // added by eugene.seo 2013.02.14 for removing smi from copy count
    QString                     RemoveSameFilePath(const QString &strFilePath);
    QString                     FindSameFile(const QString &strFilePath);

    qint64                      GetFolderDiscSize(const QString &strFolderPath);
    
    void                        StartWait();
    void                        EndWait();

    void                        StartOverWriteWait();
    void                        EndOverWriteWait();
    
    //{ added by jaehwan.lee 2014.07.18 to fix available disk space issue
    int GetAllFileSizeOfJukeBox();
    int GetAllFileSize(const QString &strPath);
    //int GetAllFileCountOfJukeBox();
    //int GetAllFileCount(const QString &strPath);
    //} added by jaewhan.lee

    static FileManagerThread   *m_pInstance;

    QTime                       m_tTime;
    int                         m_bCancelCopy;
    int                         m_bCancelDetete;
    int                         m_bCancelFormat;
    int                         m_bPauseCopying;
    int                         m_nOverwriteType;
    int                         m_nTotalPrevTime;
    int                         m_nFilePrevTime;
    int                         m_nEndTime;
    //QString                     m_strArg1;
    //QString                     m_strArg2;
    //QStringList                 m_strList;
    int                         m_nFileCount;
    int                         m_nSkipCount;
    int                         m_nTotalFileNum;
    qint64                      m_nTotalFileSpace;
    qint64                      m_nCopyDiscSize;
    //FileCommand                 m_eCmd;
    //CopyMode                    m_eCopyMode;

    int                         m_nWorking;
    QWaitCondition              m_tCond;
    QMutex                      m_tMutex;

    QWaitCondition              m_tOWCond;
    QMutex                      m_tOWMutex;
    int                         m_nOWWorking;

    QStringList                 m_tImageFmtList;
    QStringList                 m_tMusicFmtList;
    QStringList                 m_tMovieFmtList;
    QStringList                 m_tMusicMovieFmtList; // added by eugene.seo 2013.04.11

    IPlayerEngine              *m_pGPlayer;

    int                         m_nCopySpeed;
    int                         m_bSkipByUser; //added by aettie.ji 2013.01.29 for ISV 71343
    QString                     m_unsupportedSrcFile; // added by eugene.seo 2013.04.20
    QStringList                 m_tMovieFmtSupportFileList; // modified by ravikanth 16-05-13
    int                         m_nTotalSupportFileNum; // modified by ravikanth 16-05-13

    QMutex                      m_tTaskMutex;
    QQueue<FMTask>              m_tTaskQueue;
    FMTask                      m_tCurrTask;

    CTrackerAbstractor         *m_pTrackerAbstractor;  // added by sungha.choi 2013.0
};

//4/////////////////////////////////////////////////////////////////////
//4//
class DriveEntry
{
public:
    DriveEntry() {
        m_eType             = QSystemStorageInfo::NoDrive;
        m_strVolumeName     = QString();
        m_nTotalDiscSpace   = 0;
        m_nAvailDiscSpace   = 0;
    }
    
    QSystemStorageInfo::DriveType   m_eType;
    QString                         m_strVolumeName;
    qint64                          m_nTotalDiscSpace;
    qint64                          m_nAvailDiscSpace;
};

class FileManager : public QObject
{
    Q_OBJECT
public:
    static FileManager* GetInstance();
    
                       ~FileManager();

    void                SetPlayerEngine(IPlayerEngine *pPE);                       
                        
    int                 CreateFolder(const QString &strFolderName);
    int                 RenameFolder(const QString &strOldName, const QString &strNewName);
    int                 RenameFile(const QString &strOldName, const QString &strNewName);
    int                 DeleteFileFolder(const QString &strName, int nTypeMode, ETAVolumeType volume);
    int                 MoveFolder(const QString &strFolderName, const QString &strDestPath);
    int                 CopyFolder(const QString &strFolderName, const QString &strDestPath, int nTypeMode, ETAVolumeType volume);

    int                 DeleteFileList(const QStringList &strList, int nTypeMode, ETAVolumeType volume);
    int                 MoveFileList(const QStringList &strList, const QString &strDestPath, int nTypeMode);
    int                 CopyFileList(const QStringList &strList, const QString &strDestPath, int nTypeMode, ETAVolumeType volume);
                        
                        
    qint64              GetTotalFileSpace();
    int                 GetTotalFileNum();

    qint64              GetTotalDiscSpace(const QString &strVolumeName);
    qint64              GetTotalUsedDiscSpace(const QString &strVolumeName);
    int                 StartDiscFormat(const QString &strVolumeName);

    int                 CancelCopyOperation();
    int                 CancelDeleteOperation();
    int                 CancelFormatOperation();
    int                 SetOverWriteCurrentExistFileMode(int event);
    int                 ContinueCopying();
    int                 PauseCopying();

    int                 FindDrive(const QString &strPath);
    int                 FindDrive2(const QString &strVolumeName);
    qint64              GetAvailableSpace(int nIndex);
    qint64              GetTotalSpace(int nIndex);
    
    void                UpdateStorage();
    int                 IsUpdatedStorage();
    void                SetUpdatedStorage(int bUpdate);
    
    void                SetCopySpeed(int nSpeed); // nSpeed: 0, 1, 2 ~ 
    
signals:
    void                OnPercentageAndSizeCopied(int nIndex, qint64 nSize);
    void                OnPercentageAndSizeMoved(int nIndex, qint64 nSize);
    void                OnFileCopiedPercentage(int nIndex, int nPercentage);
    void                OnFileMovedPercentage(int nIndex, int nPercentage);
    void                OnPercentageAndSizeDeleted(int nIndex, qint64 nSize);
    void                OnPercentageAndSizeAndNameDeleted(int nIndex, qint64 nSize, QString strName);
    void                OnCreateFinished(int bSuccess);
    void                OnRenameFinished(int bSuccess, int nMode);
    void                OnDeleteFinished(int bSuccess, int nMode, int nVolume);
    void                OnMoveFinished(int bSuccess, int nMode);
    void                OnCopyFinished(int bSuccess, int nSkipCount, int nFilesCount, int nMode, int nVolume, int nCurrentReqMode, int nCurrentReqVolume);
    void                OnFormatFinished(int bSuccess);
    void                OnError(int nError, QString strFileName = QString());
    void                OnFileMoved(QString strFileName);

public slots:
    void slotOnCopyFinished(int bSuccess, int nSkipCount, int nFilesCount, int nMode, int nVolume); // modified by ravikanth 22-04-13

private:
                        FileManager(QObject *parent = 0);
    void                GetMountEntries();
    QStringList         GetLogicalDrives();
    qint64              GetTotalSpaceOfVolume(const QString &strDriveVolume);
    qint64              GetAvailableSpaceOfVolume(const QString &strDriveVolume);

    static FileManager* m_pInstance;

    int                     m_bUpdatedStorage;
    FileManagerThread      *m_pFileMgrThread;
    QSystemStorageInfo     *m_pSTI;
    QList<DriveEntry>       m_tVolumeList;
    QMap<QString, QString>  m_tMountEntriesMap;
    int m_nCurrentCopyMode;
    ETAVolumeType m_nCurrentCopyVolume;
};

#endif // FILEMANAGER_H
