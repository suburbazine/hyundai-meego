#include "filemanager.h"
#include "mediaplayer_def.h"
#include "performance.h"
#include "iplayerengine.h"
#include "util.h"
#include <QFile>
#include <QDir>
#include <QTime>
#include <QTextCodec>
#include <stdio.h>
#include <mntent.h>
#include <sys/vfs.h>
#include <MediaInfo/MediaInfo.h>    // modified by sungha.choi 2013.09.10
#include "common.h"
#include <QCryptographicHash>

#define IFILE_SUCCESS                   (1)
#define IFILE_FAILED                    (0)

#define IFILE_OVERWRITE                 (2)
#define IFILE_SKIP                      (3)
#define IFILE_CANCEL                    (4)

#define COPY_IMPL_READ                  (256*1024) // 256KB

#define FM_USE_COPY_IMPL                1
#define FM_USE_SAME_FILE_TAG            0
#define FM_USE_QSYSTEMSTORAGEINFO       0

#define JUKEBOX_SYSTEM_DISC_SPACE       1737420800// 1737318400
#define JUKEBOX_DATA_PATH               "/usr_data"
#define JUKEBOX_MEDIA_PATH              "/usr_data/media"
#define JUKEBOX_MUSIC_PATH              "/usr_data/media/Music"
#define JUKEBOX_VIDEO_PATH              "/usr_data/media/Video"
#define JUKEBOX_PHOTO_PATH              "/usr_data/media/Picture"
#define JUKEBOX_LOST_FOUND_PATH         "/usr_data/lost+found"

// { modified by honggi.shin, 2014.05.23, change thumbnail path
#define JUKEBOX_PHOTO_THUMBNAIL_PATH         "/app/tracker/.thumbnails/photo_thumbnails" // modified by ravikanth 06-04-13
#define JUKEBOX_PHOTO_THUMBNAIL_USB1_PATH    "/app/tracker/.thumbnails/photo_thumbnails/usb1"
#define JUKEBOX_PHOTO_THUMBNAIL_USB2_PATH    "/app/tracker/.thumbnails/photo_thumbnails/usb2"
#define JUKEBOX_PHOTO_THUMBNAIL_JB_PATH      "/app/tracker/.thumbnails/photo_thumbnails/jb" // modified by ravikanth 20-07-13 for ITS 0180478

// file manager image thumbnail path. Modified on 18-02-14
#define FM_APP_PHOTO_THUMBNAIL_PATH          "/app/tracker/.thumbnails/pic_list_thumbnails"
#define FM_APP_PHOTO_THUMBNAIL_USB1_PATH     "/app/tracker/.thumbnails/pic_list_thumbnails/usb1"
#define FM_APP_PHOTO_THUMBNAIL_USB2_PATH     "/app/tracker/.thumbnails/pic_list_thumbnails/usb2"
#define FM_APP_PHOTO_THUMBNAIL_JB_PATH       "/app/tracker/.thumbnails/pic_list_thumbnails/jb"
// } modified by honggi.shin, END

////////////////////////////////////////////////////////////////////////
///// MoveFolderThread
FileManagerThread* FileManagerThread::m_pInstance = 0;

FileManagerThread::~FileManagerThread()
{
    MC_HIGH << LOG_ENDL;
    if(m_pTrackerAbstractor) {
        delete m_pTrackerAbstractor;
        m_pTrackerAbstractor = NULL;
    }
}

FileManagerThread* FileManagerThread::GetInstance()
{
    if(m_pInstance == 0) {
        m_pInstance = new FileManagerThread();
    }

    return m_pInstance;
}

void FileManagerThread::SetPlayerEngine(IPlayerEngine *pPE)
{
    m_pGPlayer = pPE;
}

int FileManagerThread::StartCreateFolder(const QString &strFolderName)
{
    MC_HIGH << LOG_ENDL;
    
    FMTask tTask;
    tTask.eCmd      = FC_CREATE_FOLDER;
    tTask.eCopyMode = FC_COPY_ALL;
    tTask.eVolume   = eJukeBox;
    tTask.strArg1   = strFolderName;

    m_tTaskMutex.lock();
    m_tTaskQueue.enqueue(tTask);
    m_tTaskMutex.unlock();
    
    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_nWorking = 1;
        m_tCond.wakeAll();
    }
    m_tMutex.unlock();
    
    return IFILE_SUCCESS;
}

int FileManagerThread::StartRenameFolder(const QString &strOldName, const QString &strNewName)
{
    MC_HIGH << LOG_ENDL;
    
    FMTask tTask;
    tTask.eCmd      = FC_RENAME_FOLDER;
    tTask.eCopyMode = FC_COPY_ALL;
    tTask.eVolume   = eJukeBox;
    tTask.strArg1   = strOldName;
    tTask.strArg2   = strNewName;

    m_tTaskMutex.lock();
    m_tTaskQueue.enqueue(tTask);
    m_tTaskMutex.unlock();
    
    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_nWorking = 1;
        m_tCond.wakeAll();
    }
    m_tMutex.unlock();
    
    return IFILE_SUCCESS;
}

int FileManagerThread::StartRenameFile(const QString &strOldName, const QString &strNewName)
{
    MC_HIGH << LOG_ENDL;
    
    FMTask tTask;
    tTask.eCmd      = FC_RENAME_FILE;
    tTask.eCopyMode = FC_COPY_ALL;
    tTask.eVolume   = eJukeBox;
    tTask.strArg1   = strOldName;
    tTask.strArg2   = strNewName;

    m_tTaskMutex.lock();
    m_tTaskQueue.enqueue(tTask);
    m_tTaskMutex.unlock();
    
    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_nWorking = 1;
        m_tCond.wakeAll();
    }
    m_tMutex.unlock();
    
    return IFILE_SUCCESS;
}

int FileManagerThread::StartDeleteFileFolder(const QString &strDeleteName, CopyMode nMode, ETAVolumeType volume)
{
    MC_HIGH << LOG_ENDL;
    
    FMTask tTask;
    tTask.eCmd      = FC_DELETE_FILE_FOLDER;
    tTask.eCopyMode = nMode;
    tTask.eVolume   = volume;
    tTask.strArg1   = strDeleteName;

    m_tTaskMutex.lock();
    m_tTaskQueue.enqueue(tTask);
    m_tTaskMutex.unlock();
    
    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_nWorking = 1;
        m_tCond.wakeAll();
    }
    m_tMutex.unlock();
    
    return IFILE_SUCCESS;
}

int FileManagerThread::StartMoveFolder(const QString &strFolderName, const QString &strDestPath, CopyMode nMode)
{
    MC_HIGH << LOG_ENDL;
    
    FMTask tTask;
    tTask.eCmd      = FC_MOVE_FOLDER;
    tTask.eCopyMode = nMode;
    tTask.eVolume   = eJukeBox;
    tTask.strArg1   = strFolderName;
    tTask.strArg2   = strDestPath;

    m_tTaskMutex.lock();
    m_tTaskQueue.enqueue(tTask);
    m_tTaskMutex.unlock();
    
    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_nWorking = 1;
        m_tCond.wakeAll();
    }
    m_tMutex.unlock();
    
    return IFILE_SUCCESS;
}

int FileManagerThread::StartCopyFolder(const QString &strFolderName, const QString &strDestPath, CopyMode nMode, ETAVolumeType volume)
{
    MC_HIGH << LOG_ENDL;
    
    FMTask tTask;
    tTask.eCmd      = FC_COPY_FOLDER;
    tTask.eCopyMode = nMode;
    tTask.eVolume   = volume;
    tTask.strArg1   = strFolderName;
    tTask.strArg2   = strDestPath;

    m_tTaskMutex.lock();
    m_tTaskQueue.enqueue(tTask);
    m_tTaskMutex.unlock();
    
    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_nWorking = 1;
        m_tCond.wakeAll();
    }
    m_tMutex.unlock();
    
    return IFILE_SUCCESS;
}

int FileManagerThread::StartDeleteList(const QStringList &strList, CopyMode nMode, ETAVolumeType volume)
{
    MC_HIGH << LOG_ENDL;
    
    FMTask tTask;
    tTask.eCmd      = FC_DELETE_LIST;
    tTask.eCopyMode = nMode;
    tTask.eVolume   = volume;
    tTask.strList   = strList;

    m_tTaskMutex.lock();
    m_tTaskQueue.enqueue(tTask);
    m_tTaskMutex.unlock();
    
    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_nWorking = 1;
        m_tCond.wakeAll();
    }
    m_tMutex.unlock();
    
    return IFILE_SUCCESS;
}
        
int FileManagerThread::StartMoveList(const QStringList &strList, const QString &strDestPath, CopyMode nMode)
{
    MC_HIGH << LOG_ENDL;
    
    FMTask tTask;
    tTask.eCmd      = FC_MOVE_LIST;
    tTask.eCopyMode = nMode;
    tTask.strArg2   = strDestPath;
    tTask.strList   = strList;

    m_tTaskMutex.lock();
    m_tTaskQueue.enqueue(tTask);
    m_tTaskMutex.unlock();
    
    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_nWorking = 1;
        m_tCond.wakeAll();
    }
    m_tMutex.unlock();
    
    return IFILE_SUCCESS;
}
    

int FileManagerThread::StartCopyList(const QStringList &strList, const QString &strDestPath, CopyMode nMode, ETAVolumeType volume)
{
    MC_HIGH << LOG_ENDL;
    
    FMTask tTask;
    tTask.eCmd      = FC_COPY_LIST;
    tTask.eCopyMode = nMode;
    tTask.eVolume   = volume;
    tTask.strArg2   = strDestPath;
    tTask.strList   = strList;

    m_tTaskMutex.lock();
    m_tTaskQueue.enqueue(tTask);
    m_tTaskMutex.unlock();
    
    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_nWorking = 1;
        m_tCond.wakeAll();
    }
    m_tMutex.unlock();

    return IFILE_SUCCESS;
}

int FileManagerThread::StartDiscFormat(const QString &strVolumeName)
{
    MC_HIGH << LOG_ENDL;
    
    FMTask tTask;
    tTask.eCmd      = FC_DISC_FORMAT;
    tTask.eCopyMode = FC_COPY_ALL;
    tTask.strArg1   = strVolumeName;

    m_tTaskMutex.lock();
    m_tTaskQueue.enqueue(tTask);
    m_tTaskMutex.unlock();
    
    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_nWorking = 1;
        m_tCond.wakeAll();
    }
    m_tMutex.unlock();

    return IFILE_SUCCESS;
}

int FileManagerThread::GetTotalFileNum()
{
    return ( m_nTotalFileNum - m_nTotalSupportFileNum ); // modified by ravikanth 16-05-13
}

qint64 FileManagerThread::GetTotalFileSpace()
{
    return m_nTotalFileSpace;
}

qint64 FileManagerThread::GetTotalDiscSpace(const QString &strVolumeName)
{
    FileManager *pFileMngr = FileManager::GetInstance();

    pFileMngr->UpdateStorage();

    ///////////////////////////////////////////////////////////////////
    // Find Drive and total disc space.
    int nDriveIdx = pFileMngr->FindDrive2(strVolumeName);
    if(nDriveIdx < 0) {
        MC_HIGH << "can not find "<< strVolumeName << LOG_ENDL;
        return 0;
    }

    qint64 nTotalSpace = pFileMngr->GetTotalSpace(nDriveIdx);   

    MC_HIGH << strVolumeName << "=" << QString::number(nTotalSpace) << LOG_ENDL;

    return nTotalSpace;
}

qint64 FileManagerThread::GetTotalUsedDiscSpace(const QString &strVolumeName)
{
    FileManager *pFileMngr = FileManager::GetInstance();

    pFileMngr->UpdateStorage();

    ///////////////////////////////////////////////////////////////////
    // Find Drive and available disc space.
    int nDriveIdx = pFileMngr->FindDrive2(strVolumeName);
    if(nDriveIdx < 0) {
        MC_HIGH << "can not find " << strVolumeName << LOG_ENDL;
        return 0;
    }

    qint64 nTotalSpace = pFileMngr->GetTotalSpace(nDriveIdx); 
    qint64 nAvailSpace = pFileMngr->GetAvailableSpace(nDriveIdx);  

    qint64 nUsed1 = GetFolderDiscSize("/usr_data/lost+found");
    qint64 nUsed2 = GetFolderDiscSize("/usr_data/.photo_thumbnails");    
    qint64 nUsed3 = GetFolderDiscSize("/usr_data/thumbnails");    

    // { modified by kihyung 2013.11.01 for ITS 0159285 
    // nAvailSpace = nAvailSpace - nUsed1 - nUsed2 - nUsed3;
    if(nTotalSpace - nAvailSpace < (qint64)(10737418240)) 
    {
        MC_HIGH << strVolumeName << "=" << QString::number(nAvailSpace + nUsed1 + nUsed2 + nUsed3) << "," << 
                     QString::number(nAvailSpace) << "," << 
                     QString::number(nUsed1) << "," << 
                     QString::number(nUsed2) << "," << 
                     QString::number(nUsed3) << LOG_ENDL;
        
        nAvailSpace = nAvailSpace + nUsed1 + nUsed2 + nUsed3;
    }
    
    MC_HIGH << strVolumeName << "TOTAL =" << QString::number(nTotalSpace) << 
                                   ", AVAIL =" << QString::number(nAvailSpace) <<
                                   ", USED =" << QString::number(nTotalSpace - nAvailSpace) << LOG_ENDL;


    //{ modified by jaehwan.lee 2014.07.18 to fix ITS 243242 (Jukebox size(used) display error)
    if (nTotalSpace - nAvailSpace < (10 * 1024 * 1024)) //10MB (if less than 10MB, count each file size)
    {
        CRITICAL() << "Used size < 10MB , real used size :" << GetAllFileSizeOfJukeBox();
        return GetAllFileSizeOfJukeBox();
    }
    //} modified by jaehwan.lee
    else
    {
        return nTotalSpace - nAvailSpace;
    }
    // } modified by kihyung 2013.11.01 for ITS 0159285
}

int FileManagerThread::CancelCopy()
{
    int bRet;

    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_tMutex.unlock();
        bRet = IFILE_FAILED;
    }
    else {
        MC_HIGH << LOG_ENDL;
        m_tMutex.unlock();
        
        m_tTaskMutex.lock();
        m_bPauseCopying = 0; 
        m_bCancelCopy   = 1;
        m_tTaskMutex.unlock();
        bRet = IFILE_SUCCESS;
    }

    EndOverWriteWait();

    return bRet;
}

int FileManagerThread::CancelDelete()
{
    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_tMutex.unlock();
        return IFILE_FAILED;
    }
    else {
        MC_HIGH << LOG_ENDL;
        m_tMutex.unlock();

        m_tTaskMutex.lock();
        m_bCancelDetete = 1;
        m_tTaskMutex.unlock();
        
        return IFILE_SUCCESS;
    }
}

int FileManagerThread::CancelFormat()
{
    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_tMutex.unlock();
        return IFILE_FAILED;
    }
    else {
        MC_HIGH << LOG_ENDL;
        m_tMutex.unlock();

        m_tTaskMutex.lock();
        m_bCancelFormat = 1;
        m_tTaskMutex.unlock();
        
        return IFILE_SUCCESS;
    }
}

int FileManagerThread::SetOverWriteCurrentExistFileMode(int event)
{
    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_tMutex.unlock();
        return IFILE_FAILED;
    }
    else {
        MC_HIGH << LOG_ENDL;
        m_nOverwriteType = event;
        m_tMutex.unlock();
        return IFILE_SUCCESS;
    }
}

int FileManagerThread::ContinueCopying()
{
    m_tTaskMutex.lock();
    m_bPauseCopying = 0; 
    m_tTaskMutex.unlock();
    
    EndOverWriteWait();
    
    return true;    
}

int FileManagerThread::PauseCopying()
{
    m_tMutex.lock();
    if(m_nWorking == 0) {
        m_tMutex.unlock();
        return IFILE_FAILED;
    }
    else {
        MC_HIGH << LOG_ENDL;
        m_tMutex.unlock();

        m_tTaskMutex.lock();
        m_bPauseCopying = 1;
        m_tTaskMutex.unlock();
        
        return IFILE_SUCCESS;
    }
}

int FileManagerThread::WaitCopying(int bErrSignal, QString strFileName)
{
//#if FM_USE_SAME_FILE_EVENT
    MC_HIGH << LOG_ENDL;
    
    m_tOWMutex.lock(); 
    m_nOWWorking = 1;
    m_tOWMutex.unlock();

    if(bErrSignal == true) {
        emit OnError(eMP_SAME_FILE_EXIST_INCOPY, strFileName);
    }
            
    StartOverWriteWait();
//#else
//    Q_UNUSED(bErrSignal);
//    Q_UNUSED(strFileName);
//#endif
    
    return true;
}

void FileManagerThread::SetCopySpeed(int nSpeed)
{
    //MC_HIGH << nSpeed << LOG_ENDL;
    m_tTaskMutex.lock();
    m_nCopySpeed = nSpeed;
    m_tTaskMutex.unlock();
}

void FileManagerThread::run()
{
    forever
    {
        StartWait();

        int nTaskCount = 0;
        m_tTaskMutex.lock();
        nTaskCount = m_tTaskQueue.count();
        if(nTaskCount > 0) {
            m_tCurrTask = m_tTaskQueue.dequeue();
            InitStart();
            MC_HIGH << "____________________________________________" << LOG_ENDL;
            MC_HIGH << "TOTAL TASK COUNT:" << nTaskCount << LOG_ENDL;
            MC_HIGH << "TASK COMMAND:" << m_tCurrTask.eCmd << LOG_ENDL;
            MC_HIGH << "TASK COPY MODE:" << m_tCurrTask.eCopyMode << LOG_ENDL;
            MC_HIGH << "TASK VOLUME:" << m_tCurrTask.eVolume << LOG_ENDL;
            MC_HIGH << "TASK COPY ARG1:" << m_tCurrTask.strArg1 << LOG_ENDL;
            MC_HIGH << "TASK COPY ARG2:" << m_tCurrTask.strArg2 << LOG_ENDL;            
        }
        m_tTaskMutex.unlock();

        if(nTaskCount <= 0) {
            EndWait();
            continue;
        }

        int             bSuccess = false;
        FileCommand     eCmd     = m_tCurrTask.eCmd;
        CopyMode        eMode    = m_tCurrTask.eCopyMode;
        ETAVolumeType   eVolume   = m_tCurrTask.eVolume;

        m_nEndTime       = 0;
        m_nTotalPrevTime = 0;
        m_nFilePrevTime  = 0;

        //MC_HIGH << "New update storage" << LOG_ENDL;
        FileManager::GetInstance()->UpdateStorage();
        
        switch(eCmd)
        {
        case FC_CREATE_FOLDER:
            bSuccess = CreateFolder(m_tCurrTask.strArg1);
            break;
        case FC_RENAME_FOLDER:
            bSuccess = RenameFolder(m_tCurrTask.strArg1, m_tCurrTask.strArg2);
            break;
        case FC_RENAME_FILE:
            bSuccess = RenameFile(m_tCurrTask.strArg1, m_tCurrTask.strArg2);
            break;
        case FC_DELETE_FILE_FOLDER:
            bSuccess = DeleteFileFolder();
            // FileManager::GetInstance()->UpdateStorage();
            break;   
        case FC_MOVE_FOLDER:
            bSuccess = MoveFileFolder();
            // FileManager::GetInstance()->UpdateStorage();
            break;
        case FC_COPY_FOLDER:
            bSuccess = CopyFileFolder();
            // FileManager::GetInstance()->UpdateStorage();
            break;
        case FC_DELETE_LIST:
            bSuccess = DeleteFileFolderList();
            // FileManager::GetInstance()->UpdateStorage();
            break;
        case FC_MOVE_LIST:
            bSuccess = MoveFileFolderList();
            // FileManager::GetInstance()->UpdateStorage();
            break;
        case FC_COPY_LIST:
            bSuccess = CopyFileFolderList();
            // FileManager::GetInstance()->UpdateStorage();
            break;
        case FC_DISC_FORMAT:
            bSuccess = FormatDisc(m_tCurrTask.strArg1);
            // FileManager::GetInstance()->UpdateStorage();
            break;
        default:
            break;
        }
        
        EndWait();

        ///////////////////////////////////////////////////////////
        // After EndWait(), Important!!!
        switch(eCmd)
        {
        case FC_CREATE_FOLDER:
            emit OnCreateFinished(bSuccess);
            break;
        case FC_RENAME_FOLDER:
            emit OnRenameFinished(bSuccess, true);
            break;
        case FC_RENAME_FILE:
            emit OnRenameFinished(bSuccess, false);
            break;
        case FC_DELETE_FILE_FOLDER:
        case FC_DELETE_LIST:
            emit OnDeleteFinished(bSuccess, eMode, eVolume);
            break;   
        case FC_MOVE_FOLDER:
        case FC_MOVE_LIST:
            emit OnMoveFinished(bSuccess, eMode);
            break;
        case FC_COPY_FOLDER:
        case FC_COPY_LIST:
            emit OnCopyFinished(bSuccess, m_nSkipCount - m_nTotalSupportFileNum, m_nFileCount - m_nTotalSupportFileNum, eMode, eVolume); // modified by ravikanth 16-05-13
            break;
        case FC_DISC_FORMAT:
            emit OnFormatFinished(bSuccess);
            break;
        default:
            break;
        }
        
        m_tMutex.lock();
        m_tTaskMutex.lock();    // { modified by sungha.choi 2013.09.02 for ITS 0187204
        if(m_tTaskQueue.count() > 0) m_nWorking = 1;
        m_tTaskMutex.unlock();  // { modified by sungha.choi 2013.09.02 for ITS 0187204
        m_tMutex.unlock();
    }
}

FileManagerThread::FileManagerThread(QObject *parent) : QThread(parent)
    // { modified by kihyung 2013.07.15 for STATIC 18475
    , m_bCancelCopy(0)
    , m_bCancelDetete(0)
    , m_bCancelFormat(0)
    , m_bPauseCopying(0)
    , m_nOverwriteType(CANCEL_COPY_OPERATION_MODE)
    , m_nTotalPrevTime(0)
    , m_nFilePrevTime(0)
    , m_nEndTime(0)
    , m_nFileCount(0)
    , m_nSkipCount(0)
    , m_nTotalFileNum(0)
    , m_nTotalFileSpace(0)
    , m_nCopyDiscSize(0)
    //, m_eCmd(FC_NONE)
    //, m_eCopyMode(FC_COPY_ALL)
    , m_nWorking(0)
    , m_nOWWorking(0)
    , m_pGPlayer(0)
    , m_nCopySpeed(FM_SLOW_COPY_SPEED) // modified by kihyung 2013.11.11 for ITS 0193747 
    , m_bSkipByUser(0)
    , m_nTotalSupportFileNum(0)
    , m_pTrackerAbstractor(NULL)
    // } modified by kihyung 2013.07.15 for STATIC 18475
{
    // { modified by eugene.seo 2013.04.17
    m_tMusicFmtList <<".m4a"<<".mp3"<<".oga"<<".ogg"<<".wav"<<".wma";
    m_tMovieFmtList <<".avi"<<".divx"<<".m2ts"<<".m4v"<<".mkv"<<".mp4"<<".mpg"<<".mpeg"<<".ps"<<".tp"<<".ts"<<".vod"<<".wmv"<<".smi";
    m_tImageFmtList <<".bmp"<<".gif"<<".jpg"<<".jpeg"<<".png"<<".tif"<<".tiff";
    m_tMusicMovieFmtList <<".asf";
    m_tMovieFmtSupportFileList <<".smi"; // modified by ravikanth 16-05-13
	// } modified by eugene.seo 2013.04.17
    m_pTrackerAbstractor = new CTrackerAbstractor();    // added by sungha.choi 2013.09.10
}

void FileManagerThread::InitStart()
{
    m_nFileCount        = 0;
    m_nSkipCount        = 0;
    m_nTotalFileNum     = 0;
    m_nTotalFileSpace   = 0;
    m_nCopyDiscSize     = 0;
    m_bCancelDetete     = 0;
    m_bCancelFormat     = 0;
    m_bCancelCopy       = 0;
    m_bPauseCopying     = 0;
    m_nOverwriteType    = OVERWRITE_ALL_EXIST_FILE_MODE; // modified by kihyung 2013.08.28
    m_bSkipByUser       = 0; //added by aettie.ji 2013.01.29 for ISV 71343
    m_nTotalSupportFileNum = 0; // modified by ravikanth 16-05-13
}

int FileManagerThread::CreateFolder(const QString &strFolderPath)
{
    QDir      tDir;
    QFileInfo tFileInfo(strFolderPath);

    if(tFileInfo.exists() == true && tFileInfo.isDir() == true) {
        MC_HIGH << strFolderPath << LOG_ENDL;
        emit OnError(eMP_SAME_FOLDER_EXIST_INCREATE, strFolderPath);
        return false;
    }
    
    if(tDir.mkpath(strFolderPath) == false) {
        MC_HIGH << "can not create "<< strFolderPath << LOG_ENDL;
        emit OnError(eMP_JUKEBOX_ERROR, strFolderPath);
        return false;
    }
    
    return true;
}

int FileManagerThread::RenameFolder(const QString &strOldName, const QString &strNewName)
{
    QDir      tDir;
    QFileInfo tFileInfo(strOldName);

    if(tFileInfo.exists() == false || tFileInfo.isDir() == false) {
        MC_HIGH << strOldName << "is not exist" << LOG_ENDL;
        emit OnError(eMP_JUKEBOX_ERROR, strOldName);
        return false;
    }

    if(tDir.rename(strOldName, strNewName) == false) {
        MC_HIGH << "rename fail " << strOldName << "->" << strNewName << LOG_ENDL;
        QFileInfo tFileInfoNew(strNewName);
        if(tFileInfoNew.exists() == true && tFileInfoNew.isDir() == true) {
            emit OnError(eMP_SAME_FOLDER_EXIST_INEDIT, strNewName);
        }else{
            emit OnError(eMP_JUKEBOX_ERROR, strOldName);
        }
        return false;
    }
    
    return true;
}

int FileManagerThread::RenameFile(const QString &strOldName, const QString &strNewName)
{
    QFileInfo tFileInfo(strOldName);

    if(tFileInfo.exists() == false || tFileInfo.isDir() == true) {
        MC_HIGH << strOldName << "is not exist" << LOG_ENDL;
        emit OnError(eMP_JUKEBOX_ERROR, strOldName);
        return false;
    }

    if(QFile::rename(strOldName, strNewName) == false) {
        MC_HIGH << "rename fail " << strOldName << "->" << strNewName << LOG_ENDL;
        QFileInfo tFileInfoNew(strNewName);
        if(tFileInfoNew.exists() == true && tFileInfoNew.isDir() == false) {
            emit OnError(eMP_SAME_FILE_EXIST_INEDIT, strNewName);
        }else{
            emit OnError(eMP_JUKEBOX_ERROR, strOldName);
        }
        return false;
    }
    
    return true;
}

int FileManagerThread::DeleteFileFolder()
{
    int bSuccess = false;
    
    m_tTime.restart();
    m_nTotalFileNum = CalcTotalFileNum(m_tCurrTask.strArg1, true);
    bSuccess = DeleteFileFolder_Impl(m_tCurrTask.strArg1);
    m_nEndTime = m_tTime.elapsed();
    
    //MC_HIGH << "DELETE FOLDER COMPLETE:" << m_nEndTime / 1000 << LOG_ENDL;
    
    return bSuccess;
}

int FileManagerThread::DeleteFileFolder_Impl(const QString &strDeleteName)
{
    QFileInfo tDeleteFileInfo(strDeleteName);
    if(!tDeleteFileInfo.exists()) // modified by ravikanth 13-07-13 for ITS 179903, 179925
    {
        // If file don't exit..skip the operation and delete remaining files.
        return true;
    }
    if(tDeleteFileInfo.isDir()) 
    {
        QDir          tDir(strDeleteName);
        QFileInfoList tFileInfoList = tDir.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);

        //MC_HIGH << "folder case" << LOG_ENDL;

        for(int i = 0; i < tFileInfoList.size(); i++) 
        {
            QFileInfo tFileInfo = tFileInfoList.at(i);
            qint64    nTempSize = tFileInfo.size();

            if(tFileInfo.isDir() == true) {
                if(DeleteFileFolder_Impl(tFileInfo.absoluteFilePath()) == false) {
                    return false;
                }
            }
            else // if(tFileInfo.isFile() == true) 
            {
                QString strTempName = tFileInfo.absoluteFilePath();
                if(QFile::remove(strTempName) == false) {
                    MC_HIGH << tFileInfo.absoluteFilePath() << "is not deleted" << LOG_ENDL;
                }
                else {
                    m_nFileCount++;
                    m_nCopyDiscSize += nTempSize;
                    emit OnPercentageAndSizeAndNameDeleted(m_nFileCount, m_nCopyDiscSize, strTempName);
                }
            }

            if(m_bCancelDetete == 1) {
                return false;
            }
        }

        if(tDir.rmdir(strDeleteName) == false) {
            MC_HIGH << strDeleteName << "folder is not deleted" << LOG_ENDL;
            return false;
        }else{
            return true;
        }
    }
    else if(tDeleteFileInfo.isFile())
    {
        qint64 nTempSize = tDeleteFileInfo.size();

        //MC_HIGH << "file case" << LOG_ENDL;
        
        if(QFile::remove(strDeleteName) == false) {
            MC_HIGH << "is not deleted" << strDeleteName << LOG_ENDL;
            return false;
        }else{
            DeleteImageThumbnail_Impl(strDeleteName); // Modified on 18-02-14
            m_nFileCount++;
            m_nCopyDiscSize += nTempSize;
            emit OnPercentageAndSizeAndNameDeleted(m_nFileCount, m_nCopyDiscSize, strDeleteName);
            return true;
        }
    }
    else {
        MC_HIGH << "Error" << LOG_ENDL;
        return false;
    }
}

// Added on 18-02-14
int FileManagerThread::DeleteImageThumbnail_Impl(const QString &strDeleteName)
{
    //MC_HIGH << strDeleteName << LOG_ENDL;
    CopyMode eMode = m_tCurrTask.eCopyMode;

    if((eMode == FC_COPY_VIDEO) || (eMode == FC_COPY_MUSIC))
        return false;

    QString filename = strDeleteName;
    if(filename.startsWith("///")) {
        filename = filename.prepend("file:");
    }
    QString thumbnailName = QString(QCryptographicHash::hash(filename.toUtf8(), QCryptographicHash::Md5).toHex());
    if(filename.startsWith("file:///usr_data/media/Picture/"))
    {
        bool photoDeleteSuccess = QFile::remove(QString(FM_APP_PHOTO_THUMBNAIL_JB_PATH) + QString("/") + thumbnailName + QString(".png"));
        bool fmDeleteSuccess = QFile::remove(QString(JUKEBOX_PHOTO_THUMBNAIL_JB_PATH) + QString("/") + thumbnailName + QString(".jpg"));
        if(!photoDeleteSuccess || !fmDeleteSuccess ) {
            MC_HIGH << "thumbnail is not deleted " << photoDeleteSuccess << " " << fmDeleteSuccess << " " <<thumbnailName << LOG_ENDL;
            return false;
        }
        else
            return true;
    }
    /* // remove comment below code if usb delete feature is added
    else if(filename.startsWith("file:///rw_data/media/front_usb/"))
    {
        if((QFile::remove(QString(FM_APP_PHOTO_THUMBNAIL_USB1_PATH) + QString("/") + thumbnailName + QString(".png")) == false)
                && (QFile::remove(QString(JUKEBOX_PHOTO_THUMBNAIL_USB1_PATH) + QString("/") + thumbnailName + QString(".jpg")) == false)) {
            MC_HIGH << "humbnail is not deleted" << thumbnailName << LOG_ENDL;
            return false;
        }
        else
            return true;
    }
    else if(filename.startsWith("file:///rw_data/media/rear_usb/"))
    {
        if((QFile::remove(QString(FM_APP_PHOTO_THUMBNAIL_USB2_PATH) + QString("/") + thumbnailName + QString(".png")) == false)
                && (QFile::remove(QString(JUKEBOX_PHOTO_THUMBNAIL_USB2_PATH) + QString("/") + thumbnailName + QString(".jpg")) == false)) {
            MC_HIGH << "humbnail is not deleted" << thumbnailName << LOG_ENDL;
            return false;
        }
        else
            return true;
    }
    */
    else
    {
        MC_HIGH << "Thumbnail is not deleted. File not valid" << thumbnailName << LOG_ENDL;
        return false;
    }
}

int FileManagerThread::MoveFileFolder()
{
    int bSuccess = false;
    
    QFileInfo tFileInfo(m_tCurrTask.strArg1);
    
    if(tFileInfo.isDir() == true && tFileInfo.isHidden() == false) 
    {
        m_nTotalFileNum = CalcTotalFileNum(m_tCurrTask.strArg1, false);

        if(CheckAvailDiscSize(m_tCurrTask.strArg2) == true) {
            m_tTime.restart();
            bSuccess = MoveFolder(m_tCurrTask.strArg1, m_tCurrTask.strArg2);
            m_nEndTime = m_tTime.elapsed();
        }
    }
    else if(tFileInfo.isFile() == true && tFileInfo.isHidden() == false) 
    {
        m_nTotalFileNum     = 1;
        m_nTotalFileSpace   = tFileInfo.size();
        if(CheckAvailDiscSize(m_tCurrTask.strArg2) == true) {
            m_tTime.restart();
            bSuccess = MoveFile(tFileInfo, m_tCurrTask.strArg2);
            m_nEndTime = m_tTime.elapsed();
        }
    }
    else {
        MC_LOG << "Error..." << LOG_ENDL;
    }

    //MC_HIGH << "MOVE FOLDER COMPLETE:" << m_nEndTime / 1000 << LOG_ENDL;

    return bSuccess;
}

int FileManagerThread::MoveFolder(const QString &strFolderName, const QString &strDestPath)
{
    ///////////////////////////////////////////////////////////////////
    // Check Source Dir
    QFileInfo tDirInfo(strFolderName);
    if(!tDirInfo.isDir()) {
        MC_HIGH << "is not folder" << strFolderName << LOG_ENDL;
        return false;
    }

    QDir          tSrcDir(strFolderName);
    QFileInfoList tFileInfoList  = tSrcDir.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);

    ///////////////////////////////////////////////////////////////////
    // Make destination folder
    QDir tDestDir(strDestPath);
    if(!tDestDir.exists()) {
        MC_HIGH << "is not exist" << strDestPath << LOG_ENDL;
        return false;
    }

    QString strNewFolderName = tSrcDir.dirName();
    if(tDestDir.mkdir(strNewFolderName) == false) {
        // { modified by kihyung 2012.08.30
        QFileInfo tTempInfo(strDestPath + QString("/") + strNewFolderName);
        if(tTempInfo.exists() == false) {
            MC_HIGH << "can not make " << strNewFolderName << LOG_ENDL;
            return false;
        }
        else {
            MC_HIGH << "exist " << strNewFolderName << LOG_ENDL;
        }
        // } modified by kihyung
    }
    tDestDir.cd(strNewFolderName);

    ///////////////////////////////////////////////////////////////////
    // Move src to dest
    int bError  = false;
    int bCancel = false;
    QString strDestFolderPath = tDestDir.absolutePath() + QString(QDir::separator());
    for(int i = 0; i < tFileInfoList.size(); i++) 
    {
        QFileInfo tFileInfo = tFileInfoList.at(i);
        
        if(tFileInfo.isDir() == true) 
        {
            if(MoveFolder(tFileInfo.absoluteFilePath(), tDestDir.absolutePath()) == false) {
                bError = true;
                if(m_bPauseCopying == 1) WaitCopying();
                if(m_bCancelCopy == 1) return false;
            }
        }
        else // if(tFileInfo.isFile() == true) 
        {
            QString strSrcFilePath = tFileInfo.absoluteFilePath();
            QString strDstFilePath = strDestFolderPath + tFileInfo.fileName();
            qint64  nTmpFileSize   = tFileInfo.size();

            int nResult = CopyFile_Impl(false, strSrcFilePath, strDstFilePath, nTmpFileSize);

            // FAIL
            if(nResult == IFILE_FAILED) {
                if(m_bPauseCopying == 1) WaitCopying();
                if(m_bCancelCopy == 1) {
                    bCancel = true;
                    break;
                }else{
                    bError = true;
                    MC_HIGH << "copy failure " << strSrcFilePath << "-> " << strDstFilePath << LOG_ENDL;    
                }
            }
            // SUCCESS --> REMOVE SOURCE FILE
            else if(nResult == IFILE_SUCCESS) {
                if(QFile::remove(strSrcFilePath) == false) {
                    MC_HIGH << "remove failure " << strSrcFilePath << "->" << strDstFilePath << LOG_ENDL;
                }
                else {
                    emit OnFileMoved(strSrcFilePath);
                    m_nFileCount++;
                    m_nCopyDiscSize += nTmpFileSize;
                }   
            }
            // SKIP OR OVERWRITE
            else {
                emit OnFileMoved(strSrcFilePath);
                m_nFileCount++;
                m_nCopyDiscSize += nTmpFileSize;
            }
        }
    }

    QString strSrcFolderName = tSrcDir.dirName(); 
    tSrcDir.cdUp();
    if(tSrcDir.rmdir(strSrcFolderName) == false) {
        // { removed by kihyung 2012.08.30
        // bError = true;
        // } removed by kihyung
    }

    if(bError == true || bCancel == true) {
        return false;
    }
    else {
        return true;
    }
}

int FileManagerThread::MoveFile(QFileInfo &tFileInfo, const QString &strDestPath)
{
    QDir tDestDir(strDestPath);
    if(!tDestDir.exists()) {
        MC_HIGH << "is not exist" << strDestPath << LOG_ENDL;
        return false;
    }

    int     nRet           = false;
    int     nResult        = IFILE_FAILED;
    QString strSrcFilePath = tFileInfo.absoluteFilePath();
    qint64  nTmpFileSize   = tFileInfo.size();
    QString strNewDstFilePath;
    
    if(strDestPath.compare(QString("/")) == 0)
        strNewDstFilePath = tDestDir.absolutePath() + tFileInfo.fileName();
    else
        strNewDstFilePath = tDestDir.absolutePath() + QString(QDir::separator()) + tFileInfo.fileName();

    nResult = CopyFile_Impl(false, strSrcFilePath, strNewDstFilePath, nTmpFileSize);

    // FAIL
    if(nResult == IFILE_FAILED) {
        MC_HIGH << "copy failure " << strSrcFilePath << strNewDstFilePath << LOG_ENDL;        
    }
    // SUCCESS --> REMOVE SOURCE FILE
    else if(nResult == IFILE_SUCCESS) {
        if(QFile::remove(strSrcFilePath) == false) {
            MC_HIGH << "remove failure " << strSrcFilePath << "," << strNewDstFilePath << LOG_ENDL;
        }
        else {
            emit OnFileMoved(strSrcFilePath);
            nRet = true;
            m_nFileCount++;
            m_nCopyDiscSize += nTmpFileSize;
        }
    }
    // SKIP OR OVERWRITE
    else {
        emit OnFileMoved(strSrcFilePath);
        nRet = true;
        m_nFileCount++;
        m_nCopyDiscSize += nTmpFileSize;        
    }
    
    return nRet;
}

int FileManagerThread::CopyFileFolder()
{
    int bSuccess = false;
    
    QFileInfo tFileInfo(m_tCurrTask.strArg1);
    int supportingFileCount = 0; // modified for ITS 0198324
    if(tFileInfo.isDir() == true && tFileInfo.isHidden() == false) 
    {
        m_nTotalFileNum = CalcTotalFileNumForCopy(m_tCurrTask.strArg1);

        if(CheckAvailDiscSize(m_tCurrTask.strArg2) == true) {
            m_tTime.restart();
            bSuccess = CopyFolder(m_tCurrTask.strArg1, m_tCurrTask.strArg2, supportingFileCount);
            m_nEndTime = m_tTime.elapsed();
        }
    }
    else if(tFileInfo.isFile() == true && tFileInfo.isHidden() == false) 
    {
        int bCopy = true;

        MediaType eMediaType = GetMediaType(tFileInfo.absoluteFilePath());
        if(eMediaType == FC_MEDIA_NONE) {
            bCopy = false;
        }
        else if(m_tCurrTask.eCopyMode != FC_COPY_ALL) {
            if(m_tCurrTask.eCopyMode == FC_COPY_IMAGE && eMediaType != FC_MEDIA_IMAGE)        bCopy = false;
            else if(m_tCurrTask.eCopyMode == FC_COPY_MUSIC && eMediaType != FC_MEDIA_AUDIO)   bCopy = false;
            else if(m_tCurrTask.eCopyMode == FC_COPY_VIDEO && eMediaType != FC_MEDIA_VIDEO)   bCopy = false;
        }
        
        if(bCopy == true) {
            m_nTotalFileNum     = 1;
            m_nTotalFileSpace   = tFileInfo.size();
            if(CheckAvailDiscSize(m_tCurrTask.strArg2) == true) {
                m_tTime.restart();
                bSuccess = CopyFile(tFileInfo, m_tCurrTask.strArg2, supportingFileCount);
                m_nEndTime = m_tTime.elapsed();
            }
        }
    }
    else {
        MC_HIGH << "Error..." << LOG_ENDL;
    }

    MC_HIGH << "COPY FOLDER COMPLETE: " << m_nEndTime / 1000 << LOG_ENDL;

    // { modified by kihyung 2013.1.24 for ISV 68422
    MC_HIGH << "m_nSkipCount:" << m_nSkipCount << ", m_nFileCount" << m_nFileCount << LOG_ENDL;
    if(m_nFileCount > 0 && m_nSkipCount == m_nFileCount) {  // modified by sungha.choi 2013.09.02 for ITS 0187204
		//{modified by aettie.ji 2013.01.29 for ISV 71343
        //emit OnError(eMP_UNSUPPORTED_FORMAT_COPY_ERROR, "");
        if(m_bCancelCopy==1||m_bSkipByUser==1){
            MC_HIGH << "copy canceled by user..." << m_bCancelCopy << m_bSkipByUser << LOG_ENDL;
            //emit OnError(eMP_USER_CANCEL_COPY_ERROR, "");
        }
        else{
            emit OnError(eMP_UNSUPPORTED_FORMAT_COPY_ERROR, "");
        }
		//}modified by aettie.ji 2013.01.29 for ISV 71343
        bSuccess = false;
    }
    // } modified by kihyung 2013.1.24
    
    return bSuccess;
}

int FileManagerThread::CopyFolder(const QString &strFolderName, const QString &strDestPath, int &supportingFileCount)
{
    ///////////////////////////////////////////////////////////////////
    // Check Source Dir
    QFileInfo tDirInfo(strFolderName);
    if(!tDirInfo.isDir()) {
        MC_HIGH << strFolderName << " is not folder" << LOG_ENDL;
        return false;
    }

    QDir          tSrcDir(strFolderName);
    QList<QStringList> entryList;   // { added by sungha.choi 2013.09.10

    if(m_tCurrTask.eCopyMode == FC_COPY_MUSIC) {
        m_pTrackerAbstractor->GetAudioEntryList(strFolderName, &entryList);
    }
    else if(m_tCurrTask.eCopyMode == FC_COPY_VIDEO) {
        m_pTrackerAbstractor->GetVideoEntryList(strFolderName, &entryList);
    }
    else if(m_tCurrTask.eCopyMode == FC_COPY_IMAGE) {
        m_pTrackerAbstractor->GetPictureEntryList(strFolderName, &entryList);
    }
    else {
        m_pTrackerAbstractor->GetMediaEntryList(strFolderName, &entryList);
    }

    ///////////////////////////////////////////////////////////////////
    // Make destination folder
    QDir tDestDir(strDestPath);
    if(!tDestDir.exists()) {
        MC_HIGH << strDestPath << "is not exist" << LOG_ENDL;
        return false;
    }

    QString strNewFolderName = tSrcDir.dirName();
    if(tDestDir.mkdir(strNewFolderName) == false) {
        QDir tNewFolder(strDestPath + strNewFolderName);
        if(!tDestDir.exists()) {
            MC_HIGH << "can not make" << strDestPath << strNewFolderName << LOG_ENDL;
            return false;
        }
        else {
            // MC_HIGH << "exists" << strDestPath << strNewFolderName << LOG_ENDL;
        }
    }
    tDestDir.cd(strNewFolderName);

    ///////////////////////////////////////////////////////////////////
    // Copy src to dest
    int     bError  = false;
    int     bCancel = false;
    QString strDestFolderPath = tDestDir.absolutePath() + QString(QDir::separator());    
    for(int i = 0; i < entryList.size(); i++)
    {
        QStringList tFileInfo = entryList.at(i);
        QString filePath = tFileInfo.at(0);
        bool bFolder = (tFileInfo.at(1) == "1")? true : false;
        bool fileWithSmiFile = false; // modified for ITS 0198324

        if(bFolder == true) {   // Folder
            if(CopyFolder(filePath, tDestDir.absolutePath(), supportingFileCount) == false) {
                bError = true;
                if(m_bPauseCopying == 1) WaitCopying();
                if(m_bCancelCopy == 1) return false;
            }
        }
        else    // File
        {
            if(m_tCurrTask.eCopyMode == FC_COPY_VIDEO) {
                QString smiFile = filePath;
                smiFile = smiFile.mid( 0 , smiFile.lastIndexOf(".")) + QString(".smi");

                QString strSMISrcFilePath = strFolderName + "/" + smiFile;
                QString strSMIDstFilePath = strDestFolderPath + smiFile;

                QFileInfo fileInfo(strSMISrcFilePath);
                if(fileInfo.exists())
                {
                    qint64  nTmpFileSize = fileInfo.size();

                    int nResult = CopyFile_Impl(true, strSMISrcFilePath, strSMIDstFilePath, nTmpFileSize, supportingFileCount);
                    if(nResult == IFILE_FAILED) {
                        if(m_bPauseCopying == 1) WaitCopying();
                        if(m_bCancelCopy == 1) {
                            bCancel = true;
                            break;
                        }else{
                            bError = true;
                            MC_HIGH << "copy failure " << strSMISrcFilePath << "->" << strSMIDstFilePath << LOG_ENDL;
                        }
                    }
                    else {
						// Smoke fix, copy icon fix
                        if(m_bCancelCopy == 1) {
                            bCancel = true;
                            break;
                        }
                        fileWithSmiFile = true;
                        m_nCopyDiscSize += nTmpFileSize;    // smi file do not need file counting
                    }
            }
            }
            
            QString strSrcFilePath = strFolderName + "/" + filePath;
            QString strDstFilePath = strDestFolderPath + filePath;

            // MC_HIGH << "strSrcFilePath = " << strSrcFilePath << "-> " << strDstFilePath << LOG_ENDL;

            QFileInfo fileInfo(strSrcFilePath);
            if(fileInfo.exists())
            {
                qint64  nTmpFileSize = fileInfo.size();

            int nResult = CopyFile_Impl(true, strSrcFilePath, strDstFilePath, nTmpFileSize, supportingFileCount);
            if(nResult == IFILE_FAILED) {
                if(m_bPauseCopying == 1) WaitCopying();
                if(m_bCancelCopy == 1) {
                    bCancel = true;
                    break;
                }else{
                    bError = true;
                    MC_HIGH << "copy failure " << strSrcFilePath << "->" << strDstFilePath << LOG_ENDL;
                }
            }
            else {
                if(nResult == IFILE_SKIP) {
					// Smoke fix, copy icon fix
                    if(m_bCancelCopy == 1) {
                        bCancel = true;
                        break;
                    }
                    m_nSkipCount++;
                }
                m_nFileCount++;
                if(fileWithSmiFile) // modified for ITS 0198324, smi file count fix
                {
                    m_nFileCount++;
                    m_nSkipCount++; // every smi file has to be skipped
                    supportingFileCount++;
                }
                m_nCopyDiscSize += nTmpFileSize;
            }
        }
            else
            {
                continue;
            }
        }
    }

    if(bError == true || bCancel == true) {
        return false;
    }
    else {
        return true;
    }
}

int FileManagerThread::CopyFile(QFileInfo &tFileInfo, const QString &strDestPath, int supportingFileCount)
{
    QDir tDestDir(strDestPath);
    if(!tDestDir.exists()) {
        MC_HIGH << strDestPath << "is not exist" << LOG_ENDL;
        return false;
    }

    int     nRet           = false;
    QString strSrcFilePath = tFileInfo.absoluteFilePath();
    qint64  nTmpFileSize   = tFileInfo.size();
    QString strNewDstFilePath;
    
    if(strDestPath.compare(QString("/")) == 0)
        strNewDstFilePath = tDestDir.absolutePath() + tFileInfo.fileName();
    else
        strNewDstFilePath = tDestDir.absolutePath() + QString(QDir::separator()) + tFileInfo.fileName();

    int nResult = CopyFile_Impl(true, strSrcFilePath, strNewDstFilePath, nTmpFileSize, supportingFileCount);
    if(nResult == IFILE_FAILED) {
        MC_HIGH << "copy failure" << strSrcFilePath << strNewDstFilePath << LOG_ENDL;    
    }
    else {
        QString strFormat = GetFormat(strSrcFilePath); // added by sungha.choi 2013.09.10
        bool bSubtitleFile = strFormat.endsWith("smi", Qt::CaseInsensitive);
        if(!bSubtitleFile) {    // if smi file, do not count.
        if(nResult == IFILE_SKIP) {
            m_nSkipCount++;
        }
        m_nFileCount++;
        }
        m_nCopyDiscSize += nTmpFileSize;
        nRet = true;
    }

    return nRet;
}

int FileManagerThread::DeleteFileFolderList()
{
    int bSuccess = false;
    int nCount   = m_tCurrTask.strList.size();
    int nIdx;

    MC_HIGH << "List:" << nCount << LOG_ENDL;

    //////////////////////////////////////////////////////////////////////
    // Deleted file size.
    for(nIdx = 0; nIdx < nCount; nIdx++) 
    {
        QString    strArg = m_tCurrTask.strList.at(nIdx);
        QFileInfo  tFileInfo(strArg);
        
        if(tFileInfo.isDir() == true)
        {
            m_nTotalFileNum += CalcTotalFileNum(strArg, true);
        }
        else if(tFileInfo.isFile() == true)
        {
            m_nTotalFileNum++;
            m_nTotalFileSpace += tFileInfo.size();
        }
        else 
        {
            MC_HIGH << "Error..." << LOG_ENDL;
        }
    }

    //////////////////////////////////////////////////////////////////////
    // Delete
    m_tTime.restart();
    for(nIdx = 0; nIdx < nCount; nIdx++) 
    {
        QString strArg = m_tCurrTask.strList.at(nIdx);
        bSuccess = DeleteFileFolder_Impl(strArg);
        if(bSuccess == false) break;
    }
    m_nEndTime = m_tTime.elapsed();

    return bSuccess;    
}

int FileManagerThread::MoveFileFolderList()
{
    int bSuccess = false;
    int nCount   = m_tCurrTask.strList.size();
    int nIdx;

    MC_HIGH << "List:" << nCount << LOG_ENDL;

    //////////////////////////////////////////////////////////////////////
    // Copied file size
    for(nIdx = 0; nIdx < nCount; nIdx++) 
    {
        QString strArg = m_tCurrTask.strList.at(nIdx);
        QFileInfo tFileInfo(strArg);
        if(tFileInfo.isDir() == true)
        {
            m_nTotalFileNum += CalcTotalFileNum(strArg, false);
        }
        else if(tFileInfo.isFile() == true)
        {
            int       bCopy = true;
            MediaType eMediaType = GetMediaType(tFileInfo.absoluteFilePath());
            
            if(eMediaType == FC_MEDIA_NONE) {
                bCopy = false;
            }
            else if(m_tCurrTask.eCopyMode != FC_COPY_ALL) {
                if(m_tCurrTask.eCopyMode == FC_COPY_IMAGE && eMediaType != FC_MEDIA_IMAGE)        bCopy = false;
                else if(m_tCurrTask.eCopyMode == FC_COPY_MUSIC && eMediaType != FC_MEDIA_AUDIO)   bCopy = false;
                else if(m_tCurrTask.eCopyMode == FC_COPY_VIDEO && eMediaType != FC_MEDIA_VIDEO)   bCopy = false;
            }

            if(bCopy == true) {
                m_nTotalFileNum++;
                m_nTotalFileSpace += tFileInfo.size();
            }
        }
        else 
        {
            MC_HIGH << "Error..." << LOG_ENDL;
        }
    }

    //////////////////////////////////////////////////////////////////////
    // Move
    if(CheckAvailDiscSize(m_tCurrTask.strArg2) == true)
    {
        m_tTime.restart();
        for(nIdx = 0; nIdx < nCount; nIdx++) 
        {
            QString strArg = m_tCurrTask.strList.at(nIdx);
            QFileInfo tFileInfo(strArg);
            if(tFileInfo.isDir() == true)
            {
                bSuccess = MoveFolder(strArg, m_tCurrTask.strArg2);
                if(bSuccess == false) break;
            }
            else if(tFileInfo.isFile() == true)
            {
                int       bCopy = true;
                MediaType eMediaType = GetMediaType(tFileInfo.absoluteFilePath());

                if(eMediaType == FC_MEDIA_NONE) {
                    bCopy = false;
                }
                else if(m_tCurrTask.eCopyMode != FC_COPY_ALL) {
                    if(m_tCurrTask.eCopyMode == FC_COPY_IMAGE && eMediaType != FC_MEDIA_IMAGE)        bCopy = false;
                    else if(m_tCurrTask.eCopyMode == FC_COPY_MUSIC && eMediaType != FC_MEDIA_AUDIO)   bCopy = false;
                    else if(m_tCurrTask.eCopyMode == FC_COPY_VIDEO && eMediaType != FC_MEDIA_VIDEO)   bCopy = false;
                }

                if(bCopy == true) {
                    bSuccess = MoveFile(tFileInfo, m_tCurrTask.strArg2);
                    if(bSuccess == false) break;
                }
            }
            else 
            {
                MC_HIGH << "Error..." << LOG_ENDL;
                // { added by kihyung 2013.10.30 for ITS 0198975
                bSuccess = false;
                break;
                // } added by kihyung 2013.10.30 for ITS 0198975
            }
        }
        m_nEndTime = m_tTime.elapsed();
    }

    return bSuccess;
}

int FileManagerThread::CopyFileFolderList()
{
    int bSuccess = false;
    int nCount   = m_tCurrTask.strList.size();
    int nIdx;

    MC_HIGH << "List:" << nCount << LOG_ENDL;

    //////////////////////////////////////////////////////////////////////
    // Copied file size
    for(nIdx = 0; nIdx < nCount; nIdx++) 
    {
        QString   strArg = m_tCurrTask.strList.at(nIdx);
        QFileInfo tFileInfo(strArg);
        if(tFileInfo.isDir() == true)
        {
            m_nTotalFileNum += CalcTotalFileNumForCopy(strArg);
        }
        else if(tFileInfo.isFile() == true)
        {
            int       bCopy = true;
            MediaType eMediaType = GetMediaType(tFileInfo.absoluteFilePath());
            
            if(eMediaType == FC_MEDIA_NONE) {
                bCopy = false;
            }
            else if(m_tCurrTask.eCopyMode != FC_COPY_ALL) {
                if(m_tCurrTask.eCopyMode == FC_COPY_IMAGE && eMediaType != FC_MEDIA_IMAGE)        bCopy = false;
                else if(m_tCurrTask.eCopyMode == FC_COPY_MUSIC && eMediaType != FC_MEDIA_AUDIO)   bCopy = false;
                else if(m_tCurrTask.eCopyMode == FC_COPY_VIDEO && eMediaType != FC_MEDIA_VIDEO)   bCopy = false;
            }

			QString strFormat = GetFormat(tFileInfo.absoluteFilePath()); // added by eugene.seo 2013.02.14 for removing smi from copy count
			bool bSubtitleFile = strFormat.endsWith("smi", Qt::CaseInsensitive); // added by eugene.seo 2013.02.14 for removing smi from copy count

            if(bCopy == true && !bSubtitleFile) // modified by eugene.seo 2013.02.14 for removing smi from copy count
            {
                // modified by ravikanth 11-08-13 for ITS 0183924
                if(m_tCurrTask.eCopyMode == FC_COPY_VIDEO)
                {
                    QString fileName =  m_tCurrTask.strList.at(nIdx);
                    fileName = fileName.mid( 0 , fileName.lastIndexOf(".")) + QString(".smi");
                    QFileInfo fileInfo(fileName);
                    if(fileInfo.exists())
                    {
                        m_tCurrTask.strList.append(fileName);
                        nCount++;
                        m_nTotalFileSpace += fileInfo.size();

                    }
                }
                m_nTotalFileNum++;
                m_nTotalFileSpace += tFileInfo.size();
            }
        }
        else 
        {
            MC_HIGH << "Error..." << LOG_ENDL;
        }
    }

    //////////////////////////////////////////////////////////////////////
    // Copy
    int supportingFileCount = 0; // modified for ITS 0198324
    if(CheckAvailDiscSize(m_tCurrTask.strArg2) == true)
    {
        m_tTime.restart();
        for(nIdx = 0; nIdx < nCount; nIdx++) 
        {
            QString strArg = m_tCurrTask.strList.at(nIdx);
            QFileInfo tFileInfo(strArg);
            if(tFileInfo.isDir() == true)
            {
                bSuccess = CopyFolder(strArg, m_tCurrTask.strArg2, supportingFileCount);
                if(bSuccess == false) break;
            }
            else if(tFileInfo.isFile() == true)
            {
                int       bCopy = true;
                MediaType eMediaType = GetMediaType(tFileInfo.absoluteFilePath());
                
                if(eMediaType == FC_MEDIA_NONE) {
                    bCopy = false;
                }
                else if(m_tCurrTask.eCopyMode != FC_COPY_ALL) {
                    if(m_tCurrTask.eCopyMode == FC_COPY_IMAGE && eMediaType != FC_MEDIA_IMAGE)        bCopy = false;
                    else if(m_tCurrTask.eCopyMode == FC_COPY_MUSIC && eMediaType != FC_MEDIA_AUDIO)   bCopy = false;
                    else if(m_tCurrTask.eCopyMode == FC_COPY_VIDEO && eMediaType != FC_MEDIA_VIDEO)   bCopy = false;
                }

                if(bCopy == true) {
                    bSuccess = CopyFile(tFileInfo, m_tCurrTask.strArg2, supportingFileCount);
                    if(bSuccess == false) break;
                }
            }
            else 
            {
                MC_HIGH << "Error..." << LOG_ENDL;
                // { added by kihyung 2013.10.30 for ITS 0198975 
                bSuccess = false;
                break;
                // } added by kihyung 2013.10.30 for ITS 0198975 
            }
        }
        m_nEndTime = m_tTime.elapsed();
    }

    //MC_HIGH << "COMPLETE: " << m_nEndTime / 1000 << LOG_ENDL;

    // { modified by kihyung 2013.1.24 for ISV 68422
    MC_HIGH << "m_nSkipCount:" << m_nSkipCount << "m_nFileCount:" << m_nFileCount << LOG_ENDL;
    if(m_nFileCount > 0 && m_nSkipCount == m_nFileCount) {  // { modified by sungha.choi 2013.09.02 for ITS 0187204
		//{modified by aettie.ji 2013.01.29 for ISV 71343
        //emit OnError(eMP_UNSUPPORTED_FORMAT_COPY_ERROR, "");
        if(m_bCancelCopy==1||m_bSkipByUser==1){
            MC_HIGH << "copy canceled by user..." << m_bCancelCopy << m_bSkipByUser << LOG_ENDL;
            //emit OnError(eMP_USER_CANCEL_COPY_ERROR, "");
        }
        else{
            emit OnError(eMP_UNSUPPORTED_FORMAT_COPY_ERROR, "");
        }
        if(m_nSkipCount == m_nTotalFileNum) // changes for ITS 0205007
            bSuccess = true;
        else
            bSuccess = false;
		//}modified by aettie.ji 2013.01.29 for ISV 71343
    }
    // } modified by kihyung 2013.1.24

    return bSuccess;
}

int FileManagerThread::FormatDisc(const QString &strVolumeName)
{
    FileManager *pFileMngr = FileManager::GetInstance();
    if(pFileMngr->FindDrive2(strVolumeName) < 0) {
        MC_HIGH << "can not find" << strVolumeName << LOG_ENDL;
        return false;
    }

    m_pTrackerAbstractor->RemoveJBScanResultFile();

    int           nRet = true;
    QDir          tDir(strVolumeName);
    QFileInfoList tFileInfoList = tDir.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);

    for(int i = 0; i < tFileInfoList.size(); i++) 
    {
        QFileInfo tFileInfo = tFileInfoList.at(i);
        
        if(tFileInfo.isDir() == true) {
            if(DeleteFolder(tFileInfo.absoluteFilePath()) == false) {
                nRet = false;
            }
        }
        else // if(tFileInfo.isFile() == true) 
        {
            if(QFile::remove(tFileInfo.absoluteFilePath()) == false) {
                MC_HIGH << "format error" << tFileInfo.absoluteFilePath() << LOG_ENDL;
                nRet = false;
            }
        }
    }

    system(QString("sync").toAscii().data());
    //MC_HIGH << "FORMAT DISC COMPLETE" << LOG_ENDL;

    return nRet;
}

int FileManagerThread::DeleteFolder(const QString &strFolderName)
{
    int           nRet = true;
    QDir          tDir(strFolderName);
    QFileInfoList tFileInfoList = tDir.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);

    for(int i = 0; i < tFileInfoList.size(); i++) 
    {
        QFileInfo tFileInfo = tFileInfoList.at(i);

        if(tFileInfo.isDir() == true) {
            if(DeleteFolder(tFileInfo.absoluteFilePath()) == false) {
                nRet = false;
            }
        }
        else // if(tFileInfo.isFile() == true) 
        {
            if(QFile::remove(tFileInfo.absoluteFilePath()) == false) {
                nRet = false;
            }
        }

        if(m_bCancelFormat == 1) {
            return false;
        }
    }

    if(strFolderName.compare(JUKEBOX_MEDIA_PATH) != 0 && 
       strFolderName.compare(JUKEBOX_MUSIC_PATH) != 0 && 
       strFolderName.compare(JUKEBOX_VIDEO_PATH) != 0 && 
       strFolderName.compare(JUKEBOX_PHOTO_PATH) != 0 &&
       strFolderName.compare(JUKEBOX_LOST_FOUND_PATH) != 0 &&
       strFolderName.compare(JUKEBOX_PHOTO_THUMBNAIL_PATH) != 0 &&
       strFolderName.compare(JUKEBOX_PHOTO_THUMBNAIL_USB1_PATH) != 0 &&
       strFolderName.compare(JUKEBOX_PHOTO_THUMBNAIL_USB2_PATH) != 0 &&
       strFolderName.compare(JUKEBOX_PHOTO_THUMBNAIL_JB_PATH) != 0 &&
       strFolderName.compare(FM_APP_PHOTO_THUMBNAIL_PATH) != 0 &&
       strFolderName.compare(FM_APP_PHOTO_THUMBNAIL_USB1_PATH) != 0 &&
       strFolderName.compare(FM_APP_PHOTO_THUMBNAIL_USB2_PATH) != 0 &&
       strFolderName.compare(FM_APP_PHOTO_THUMBNAIL_JB_PATH) != 0) // modified by ravikanth 20-07-13 for ITS 0180478
    {
        if(tDir.rmdir(strFolderName) == false) {
            MC_HIGH << "format error" << strFolderName << LOG_ENDL;
            nRet = false;
        }
    }
    else {
        MC_HIGH << strFolderName << "does not deleted" << LOG_ENDL;
    }
    
    return nRet;
}

int FileManagerThread::CopyFile_Impl(int bCopy, const QString &strSrcFile, const QString &strDstFile, qint64 nFileSize, int skipCount)
{

	if( strSrcFile == NULL || strDstFile == NULL ) // added by eugene.seo 2013.03.28
		return IFILE_FAILED;
	
#if FM_USE_COPY_IMPL
    int     bExistSameFile = false;
    int     bSkip   = false;
    int     bError  = false;
    int     bCancel = false;
    qint64  nTotalRead = 0;

    // { moved by eugene.seo 2013.4.26
	if(strSrcFile.isEmpty()) {
        MC_HIGH << "strSrcFile: Empty or null file name" << LOG_ENDL;
        return IFILE_FAILED;
    }

    if(strDstFile.isEmpty()) {
        MC_HIGH << "strDstFile: Empty or null file name" << LOG_ENDL;
        return IFILE_FAILED;
    }
    // } moved by eugene.seo 2013.4.26
    if(m_tCurrTask.eCopyMode != FC_COPY_IMAGE) {  // added by sungha.choi 2013.09.21, for audio, video,
        if(m_pGPlayer && m_pGPlayer->IsFileSupported(strSrcFile) == false) {
            MC_HIGH << "m_pGPlayer->IsFileSupported() return false" << LOG_ENDL;
            m_unsupportedSrcFile = strSrcFile; // added by eugene.seo 2013.04.20

            //{removed by Michael.Kim 2014.01.29 for ITS 223052
            m_nEndTime = m_tTime.elapsed(); // added by sungha.choi 2013.09.24 for ITS 0190595
            //if(m_nEndTime - m_nTotalPrevTime >= 1000) {
                //m_nTotalPrevTime = m_nEndTime;
                if(bCopy == true) {
                    emit OnPercentageAndSizeCopied(m_nFileCount - skipCount, m_nCopyDiscSize);
                }
                else {
                    emit OnPercentageAndSizeMoved(m_nFileCount, m_nCopyDiscSize);
                }
            //} //}removed by Michael.Kim 2014.01.29 for ITS 223052

            return IFILE_SKIP;
        }
    }
    else {  // added by sungha.choi 2013.09.21, for image
        if(m_pGPlayer->IsImageSupported(strSrcFile) == false) {
            MC_HIGH << "m_pGPlayer->IsImageSupported() return false" << LOG_ENDL;

            m_nEndTime = m_tTime.elapsed(); // added by sungha.choi 2013.09.24 for ITS 0190595
            if(m_nEndTime - m_nTotalPrevTime >= 1000) {
                m_nTotalPrevTime = m_nEndTime;
                if(bCopy == true) {
                    emit OnPercentageAndSizeCopied(m_nFileCount - skipCount, m_nCopyDiscSize);
                }
                else {
                    emit OnPercentageAndSizeMoved(m_nFileCount, m_nCopyDiscSize);
                }
            }

            return IFILE_SKIP;
        }
    }
    // { added by eugene.seo 2013.04.20
    if(GetFormat(strSrcFile).endsWith("smi", Qt::CaseInsensitive))
    {
        if(strSrcFile == m_unsupportedSrcFile.replace("avi","smi"))
        {
            m_unsupportedSrcFile == "";
            return IFILE_SKIP;
        }
    }
    // } added by eugene.seo 2013.04.20

    // Check if file exists
    QFileInfo tDstFileInfo(strDstFile);
    if(tDstFileInfo.exists() == true && tDstFileInfo.isFile() == true) {
        bExistSameFile = true;
    }

    #if FM_USE_SAME_FILE_EVENT
    if(bExistSameFile == true && m_nOverwriteType != OVERWRITE_ALL_EXIST_FILE_MODE) {
        if(m_bCancelCopy != 1) WaitCopying(true, strSrcFile); // modified by ravikanth 27.06.13 for ITS 0176503
        if(m_nOverwriteType == NOT_OVERWRITE_EXIST_FILE_MODE) { 
            MC_HIGH << "Skip by user" << LOG_ENDL;
            bSkip = true;
            m_bSkipByUser = 1; //added by aettie.ji 2013.01.29 for ISV 71343
        }
        else if(m_nOverwriteType == CANCEL_COPY_OPERATION_MODE) {
            MC_HIGH << "Stop by user cancel" << LOG_ENDL;
            m_bCancelCopy = 1;
            return IFILE_FAILED;
        }
    }
    #endif

    if(bSkip == false) 
    {
        QFile tSrcFile(strSrcFile);
        if(tSrcFile.open(QFile::ReadOnly) == false) {
            MC_HIGH << "tSrcFile.open == false" << strSrcFile << LOG_ENDL;
            tSrcFile.close();
            emit OnError(eMP_CORRUPT_FILE_COPY_ERROR, strSrcFile);
            return IFILE_FAILED;
        }

        QByteArray tDstFileByteArray = QCryptographicHash::hash(strDstFile.toUtf8(), QCryptographicHash::Md5);
        QString    tDstFileMD5       = tDstFileByteArray.toHex();
        QString    tDstFilePath      = QFileInfo(strDstFile).path() + QString(QDir::separator()) + tDstFileMD5;
        
        QFile      tDstFile(tDstFilePath);
        if(tDstFile.open(QFile::ReadWrite) == false) {
            MC_HIGH << "tDstFile.open == false" << strDstFile << LOG_ENDL;
            tSrcFile.close();
            tDstFile.close();
            emit OnError(eMP_CORRUPT_FILE_COPY_ERROR, strDstFile);
            return IFILE_FAILED;
        }

        char   *pBlock = new char[COPY_IMPL_READ];
        qint64  nRead = 0;

        while(!tSrcFile.atEnd()) {
            nRead = tSrcFile.read(pBlock, COPY_IMPL_READ);
            if(nRead <= 0) break;

            nTotalRead += nRead;
            if(nRead != tDstFile.write(pBlock, nRead)) {
                MC_HIGH << "Failure to write block" << LOG_ENDL;
                bError = true;
                break;
            }

            if(m_bPauseCopying == 1) WaitCopying();
            if(m_bCancelCopy == 1) {
                bCancel = true;
                break;
            }

            //{removed by Michael.Kim 2014.01.29 for ITS 223052
            m_nEndTime = m_tTime.elapsed();
            //if(m_nEndTime - m_nTotalPrevTime >= 1000) {
                //m_nTotalPrevTime = m_nEndTime;
                if(bCopy == true) {
                    emit OnPercentageAndSizeCopied(m_nFileCount - skipCount, m_nCopyDiscSize + nTotalRead);
                }
                else {
                    emit OnPercentageAndSizeMoved(m_nFileCount, m_nCopyDiscSize + nTotalRead);
                }
            //}  //}removed by Michael.Kim 2014.01.29 for ITS 223052
            if(m_nEndTime - m_nFilePrevTime >= 200) {
                m_nFilePrevTime = m_nEndTime;
                if(bCopy == true) {
                    double fRatio = (double)nTotalRead / (double)nFileSize;
                    emit OnFileCopiedPercentage(m_nFileCount, (int)(fRatio * 100.0));
                }
                else {
                    double fRatio = (double)nTotalRead / (double)nFileSize;
                    emit OnFileMovedPercentage(m_nFileCount, fRatio);
                }
            }

            if(m_nCopySpeed > 0) 
            {
                usleep(20000 * m_nCopySpeed); // 0.020 second(20000) * m_nCopySpeed(10 or 5)
            }
        }

        if(nTotalRead != tSrcFile.size()) {
            if(bCancel == false) bError = true;
        }

        // CLOSE SOURCE FILE
        tSrcFile.close();

        // { modified by kihyung 2013.09.09 for ISV 90701
        // OVERWRITE --> DELETE DEST FILE
        if(bCancel == false && bError == false && bExistSameFile == true) { 
            if(m_nOverwriteType == OVERWRITE_ONE_EXIST_FILE_MODE || m_nOverwriteType == OVERWRITE_ALL_EXIST_FILE_MODE) {
                // MC_HIGH << "remove dest file" << strDstFile << LOG_ENDL;
                if(QFile::remove(strDstFile) == false) { 
                    bError = true;
                    MC_HIGH << "remove fail to overwrite" << LOG_ENDL;
                    emit OnError(eMP_CORRUPT_FILE_COPY_ERROR, strDstFile);
                }
            }
        }

        // RENAME DEST FILE
        if(bCancel == false && bError == false) { 
            // MC_HIGH << "rename" << strDstFile << LOG_ENDL;
            if(tDstFile.rename(strDstFile) == false) {
                bError = true;
                MC_HIGH << "Cannot create %s for output" << strDstFile << LOG_ENDL;
            }
        }

        tDstFile.flush();
        tDstFile.close();

        delete [] pBlock;
        
        if(bError == true) {
            MC_HIGH << "bError:" << bError << ". Remove File" << LOG_ENDL;
            QFile::remove(tDstFilePath);
            emit OnError(eMP_CORRUPT_FILE_COPY_ERROR, strDstFile);
            return IFILE_FAILED;
        }
        else if(bCancel == true) {
            MC_HIGH << "bCancel:" << bCancel << ". Remove File" << LOG_ENDL;
            QFile::remove(tDstFilePath);
            return IFILE_FAILED;
        }
        // } modified by kihyung 2013.09.09 for ISV 90701
    }
    #if FM_USE_SAME_FILE_EVENT  //added by honggi.shin 2013.10.25, fix static test problem
    else 
    {
        QFileInfo tSrcFileInfo(strSrcFile);
        nTotalRead = tSrcFileInfo.size();

        m_nEndTime = m_tTime.elapsed();
        if(m_nEndTime - m_nTotalPrevTime >= 1000) {
            m_nTotalPrevTime = m_nEndTime;
            if(bCopy == true) {
                emit OnPercentageAndSizeCopied(m_nFileCount - skipCount, m_nCopyDiscSize + nTotalRead);
            }
            else {
                emit OnPercentageAndSizeMoved(m_nFileCount, m_nCopyDiscSize + nTotalRead);
            }
        }

        if(m_nEndTime - m_nFilePrevTime >= 200) {
            m_nFilePrevTime = m_nEndTime;
            if(bCopy == true) {
                emit OnFileCopiedPercentage(m_nFileCount, 100);
            }
            else {
                emit OnFileMovedPercentage(m_nFileCount, 100);
            }
        }
    }
    #endif  //added by honggi.shin 2013.10.25, fix static test problem

    MC_HIGH << m_nFileCount << m_nTotalFileNum << LOG_ENDL;
    if(m_nFileCount == m_nTotalFileNum - 1) {
        if(bCopy == true) {
            emit OnPercentageAndSizeCopied(m_nFileCount - skipCount, m_nCopyDiscSize + nTotalRead);
        }else{
            emit OnPercentageAndSizeMoved(m_nFileCount, m_nCopyDiscSize + nTotalRead);
        }
    }

    if(bExistSameFile == true) {
        if(bSkip == true) {
            return IFILE_SKIP;
        }else{
            return IFILE_OVERWRITE;
        }
    }
    else {
        return IFILE_SUCCESS;
    }
    
    return true;
#else
    return QFile::copy(strSrcFile, strDstFile);
#endif
}

int FileManagerThread::CalcTotalFileNum(const QString &strFolderPath, bool bHidden)
{
    int           nCount = 0;
    int           nRetVal;
    MediaType     eMediaType;
    qint64        nFileSize;
    QDir          tSrcDir(strFolderPath);
    QFileInfoList tFileInfoList;
	
    // modified by ravikanth 17-05-13
    if(bHidden)
        tFileInfoList = tSrcDir.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);
    else
        tFileInfoList = tSrcDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

    nCount  = tFileInfoList.size();
    nRetVal = 0;
    for(int i = 0; i < nCount; i++) 
    {
        QFileInfo tFileInfo = tFileInfoList.at(i);
        
        if(tFileInfo.isDir() == true) 
        {
            nRetVal = nRetVal + CalcTotalFileNum(tFileInfo.absoluteFilePath(), bHidden);
        }
        else // if(tFileInfo.isFile() == true) 
        {
            eMediaType = GetMediaType(tFileInfo.absoluteFilePath());
            if(eMediaType == FC_MEDIA_NONE) {
                continue;
            }
            else if(m_tCurrTask.eCopyMode != FC_COPY_ALL) {
                if(m_tCurrTask.eCopyMode == FC_COPY_IMAGE && eMediaType != FC_MEDIA_IMAGE) { continue; }
                if(m_tCurrTask.eCopyMode == FC_COPY_MUSIC && eMediaType != FC_MEDIA_AUDIO) { continue; }
                if(m_tCurrTask.eCopyMode == FC_COPY_VIDEO && eMediaType != FC_MEDIA_VIDEO) { continue; }
            }
            
			// { modified by ravikanth 16-05-13
            QString fileFormat = GetFormat(tFileInfo.absoluteFilePath());
            if(!fileFormat.isEmpty()) {
                if(m_tMovieFmtSupportFileList.indexOf(fileFormat) != -1)
                {
                    m_nTotalSupportFileNum++;
                }
            }
			// } modified by ravikanth 16-05-13
			
            nRetVal++;
            nFileSize = tFileInfo.size();
            m_nTotalFileSpace += nFileSize;
        }
    }
    
    return nRetVal;
}

int FileManagerThread::CalcTotalFileNumForCopy(const QString &strFolderPath)    // added by sungha.choi 2013.09.10
{
    int           nCount = 0;
    int           nRetVal;
    qint64        nFileSize;

    QList<QStringList> entryList;   // { added by sungha.choi 2013.09.10

    if(m_tCurrTask.eCopyMode == FC_COPY_MUSIC) {
        m_pTrackerAbstractor->GetAudioEntryList(strFolderPath, &entryList);
    }
    else if(m_tCurrTask.eCopyMode == FC_COPY_VIDEO) {
        m_pTrackerAbstractor->GetVideoEntryList(strFolderPath, &entryList);
    }
    else if(m_tCurrTask.eCopyMode == FC_COPY_IMAGE) {
        m_pTrackerAbstractor->GetPictureEntryList(strFolderPath, &entryList);
    }
    else {
        m_pTrackerAbstractor->GetMediaEntryList(strFolderPath, &entryList);
    }

    nCount  = entryList.size();
    nRetVal = 0;
    for(int i = 0; i < nCount; i++) 
    {
        QStringList tFileInfo = entryList.at(i);
        QString filePath = tFileInfo.at(0);
        bool bFolder = (tFileInfo.at(1) == "1")? true : false;

        if(bFolder == true)  // folder
        {
            nRetVal = nRetVal + CalcTotalFileNumForCopy(filePath);
        }
        else    // file
        {
            if(m_tCurrTask.eCopyMode == FC_COPY_VIDEO) {
                QString smiFile = filePath;
                smiFile = smiFile.mid( 0 , smiFile.lastIndexOf(".")) + QString(".smi");
                QFileInfo fileInfo(strFolderPath + "/" + smiFile);
                if(fileInfo.exists())
                {
                    m_nTotalSupportFileNum++;
                    nRetVal++;
                    nFileSize = fileInfo.size();
                    m_nTotalFileSpace += nFileSize;
                }
            }

            QFileInfo fileInfo(strFolderPath + "/" + filePath);
            if(fileInfo.exists())
            {
                nRetVal++;
                nFileSize = fileInfo.size();
                m_nTotalFileSpace += nFileSize;
            }
            else
            {
                continue;
            }
        }
    }
    
    return nRetVal;
}

int FileManagerThread::CheckAvailDiscSize(const QString &strDestPath)
{
    FileManager *pFileMngr = FileManager::GetInstance();

    pFileMngr->UpdateStorage();

    ///////////////////////////////////////////////////////////////////
    // Find Drive and available disc space.
    int nDriveIdx = pFileMngr->FindDrive(strDestPath);
    if(nDriveIdx < 0) {
        MC_HIGH << "can not file drive" << LOG_ENDL;
        emit OnError(eMP_JUKEBOX_ERROR);
        return false;
    }

    qint64 nAvailSpace = pFileMngr->GetAvailableSpace(nDriveIdx);

    if(m_nTotalFileSpace >= nAvailSpace) 
    {
        MC_HIGH << m_nTotalFileSpace << nAvailSpace << LOG_ENDL;
        emit OnError(eMP_LESS_SPACE_ERROR);
        return false;
    }
    
    return true;
}

FileManagerThread::MediaType FileManagerThread::GetMediaType(const QString &strFileName)
{
    int     nSize       = strFileName.size();
    int     nIndex      = strFileName.lastIndexOf(".");
    int     nDiff       = nSize - nIndex;
    QString strFormat   = strFileName.mid(nIndex, nDiff).toLower();
    bool ismetadata = strFileName.contains(".mediaartlocal");
    
    if(strFormat.isEmpty()) return FC_MEDIA_NONE;

    if(m_tImageFmtList.indexOf(strFormat) == -1 || ismetadata)
    {
        if(m_tMusicFmtList.indexOf(strFormat) == -1) 
        {
            if(m_tMovieFmtList.indexOf(strFormat) == -1) 
            {
                if(m_tMusicMovieFmtList.indexOf(strFormat) == -1) 
                {                                    
                    return FC_MEDIA_NONE;
                }
                else 
                {
                    int VideoStreamCount = -1;  
                    MediaInfoLib::MediaInfo  MI;
                    MI.Open(strFileName.toStdWString());
                    VideoStreamCount = QString::fromStdWString(MI.Get(MediaInfoLib::Stream_Video, 0, _T("StreamCount"))).toInt();
                    MI.Close();   // added by sungha.choi 2013.07.30
                    if(VideoStreamCount == 0) {
                        return FC_MEDIA_AUDIO;
                    }
                    else if(VideoStreamCount > 0) {
                        return FC_MEDIA_VIDEO;
                    }    
                    else {
                        return FC_MEDIA_NONE;
                    }
                }
            }
            else 
            {
                return FC_MEDIA_VIDEO;
            }
        }
        else
        {
            return FC_MEDIA_AUDIO;
        }
    }
    else 
    {
        return FC_MEDIA_IMAGE;
    }
}

// { added by eugene.seo 2013.02.14 for removing smi from copy count
QString FileManagerThread::FileManagerThread::GetFormat(const QString &strFileName)
{
    int     nSize       = strFileName.size();
    int     nIndex      = strFileName.lastIndexOf(".");
    int     nDiff       = nSize - nIndex;
    QString strFormat   = strFileName.mid(nIndex, nDiff).toLower();

	// MC_HIGH << "strFormat:" << strFormat << LOG_ENDL;

    return strFormat;
}
// } added by eugene.seo 2013.02.14 for removing smi from copy count

QString FileManagerThread::RemoveSameFilePath(const QString &strFilePath)
{
    if(strFilePath.size() == 0) {
        return strFilePath;
    }

    QString strSameFilePath = FindSameFile(strFilePath);
    int nSize = strSameFilePath.size();
    if(nSize <= 0) {
        return strFilePath;
    }
    else {
        MC_HIGH << strSameFilePath << "exists" << LOG_ENDL;
    }

    int nLeft   = strSameFilePath.lastIndexOf("(");
    int nRight  = strSameFilePath.lastIndexOf(")");
    int nNumber = 0;

    bool    bConvOK = false;
    QString strNumber;
    if(nLeft > 0 && nRight > 0 && nLeft < nRight) {
        nLeft += 1;
        strNumber = strSameFilePath.mid(nLeft, nRight - nLeft);
        nNumber = strNumber.toInt(&bConvOK);
    }

    nNumber++;
    
    QString strNewPath;
    if(bConvOK == false) 
    {
        strNumber = QString("(%1)").arg(nNumber);
        
        nLeft = strSameFilePath.lastIndexOf(".");
        if(nLeft < 0) nLeft = nSize;
        nRight = strSameFilePath.size() - nLeft;

        if(nRight > 0) {       
            strNewPath = strSameFilePath.left(nLeft) + strNumber + strSameFilePath.right(nRight);
        }
        else {
            strNewPath = strSameFilePath.left(nLeft) + strNumber;
        }
    }
    else 
    {
        strNumber = QString("%1").arg(nNumber);
        nRight    = nSize - nRight;
        strNewPath = strSameFilePath.left(nLeft) + strNumber + strSameFilePath.right(nRight);
    }

    return strNewPath;
}

QString FileManagerThread::FindSameFile(const QString &strFilePath)
{       
    QFileInfo tFileInfo(strFilePath);
    QString   strFileName = tFileInfo.fileName();
    QString   strBaseName;
    QString   strExtName;

    int nBaseSize = strFileName.lastIndexOf(".");
    if(nBaseSize < 0) {
        nBaseSize = strFileName.size();
    }
    else {
        strExtName = strFileName.right(strFileName.size() - nBaseSize - 1);            
    }
    strBaseName = strFileName.left(nBaseSize);

    QDir tDir(tFileInfo.absolutePath());
    QFileInfoList tFileInfoList = tDir.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);

    int nFindIndex = -1;
    int nMaxNumber = -1;
    for(int i = 0; i < tFileInfoList.size(); i++)
    {
        QFileInfo tFileInfo = tFileInfoList.at(i);

        if(tFileInfo.isDir() == true) continue;   

        QString strTempName = tFileInfo.fileName();

        int nExtIndex = strTempName.lastIndexOf(".");
        if(nExtIndex > 0 && strExtName.size() > 0) {
            if(strTempName.indexOf(strExtName, nExtIndex + 1) != nExtIndex + 1) {
                continue;
            }
        }
        
        if(strTempName.indexOf(strBaseName) != 0) continue;

        int     nLeft   = strTempName.lastIndexOf("(");
        int     nRight  = strTempName.lastIndexOf(")");
        int     nNumber = 0;
        bool    bConvOK = false;
        
        if(nLeft > 0 && nRight > 0 && nLeft < nRight) {
            nLeft += 1;
            nNumber = strTempName.mid(nLeft, nRight - nLeft).toInt(&bConvOK);
            if(bConvOK == false) {
                nNumber = 0;
            }
            if(nMaxNumber < nNumber) {
                nMaxNumber = nNumber;
                nFindIndex = i;
            }
        }
        else if(nFindIndex == -1) {
            int nTempBaseSize = strTempName.lastIndexOf(".");
            if(nTempBaseSize < 0) nTempBaseSize = strTempName.size();

            if(nBaseSize == nTempBaseSize) {
                nMaxNumber = 0;
                nFindIndex = i;
            }
        }
    }

    if(nFindIndex < 0 || nFindIndex >= tFileInfoList.size()) {
        return QString();
    }
    else {
        return tFileInfoList.at(nFindIndex).absoluteFilePath();
    }
}

qint64 FileManagerThread::GetFolderDiscSize(const QString &strFolderPath)
{
    int           nCount = 0;
    qint64        nFileSize;
    QDir          tSrcDir(strFolderPath);
    QFileInfoList tFileInfoList;
	
    tFileInfoList = tSrcDir.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);
    nCount        = tFileInfoList.size();
    nFileSize     = 0;
    
    for(int i = 0; i < nCount; i++) 
    {
        QFileInfo tFileInfo = tFileInfoList.at(i);
        
        if(tFileInfo.isDir() == true) 
        {
            nFileSize = nFileSize + GetFolderDiscSize(tFileInfo.absoluteFilePath());
        }
        else {
            nFileSize += tFileInfo.size();
        }
    }
    
    return nFileSize;
}

void FileManagerThread::StartWait()
{
    m_tMutex.lock();
    while(m_nWorking == 0) {
        //MC_HIGH << LOG_ENDL;
        m_tCond.wait(&m_tMutex);
    }
    m_tMutex.unlock();
}

void FileManagerThread::EndWait()
{
    m_tMutex.lock();
    //MC_HIGH << LOG_ENDL;
    m_nWorking = 0;
    m_tMutex.unlock();
}

void FileManagerThread::StartOverWriteWait()
{
    m_tOWMutex.lock();
    while(m_nOWWorking == 1) {
        //MC_HIGH << LOG_ENDL;
        m_tOWCond.wait(&m_tOWMutex);
    }
    m_tOWMutex.unlock();
}

void FileManagerThread::EndOverWriteWait()
{
    m_tOWMutex.lock();
    if(m_nOWWorking == 1) {
        //MC_HIGH << LOG_ENDL;
        m_nOWWorking = 0;
        m_tOWCond.wakeAll();
    }
    m_tOWMutex.unlock();
}
//{ added by jaehwan.lee 2014.07.18 to fix available disk space issue
/*
int FileManagerThread::GetAllFileCountOfJukeBox()
{
    return GetAllFileCount(JUKEBOX_DATA_PATH);
}

int FileManagerThread::GetAllFileCount(const QString &strPath)
{
    int nCount = 0;
    int nRetVal = 0;

    QDir dir(strPath);
    QFileInfoList tFileInfoList = dir.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);

    nCount = tFileInfoList.size();

    for(int i = 0; i < nCount; i++)
    {
        QFileInfo tFileInfo = tFileInfoList.at(i);

        if(tFileInfo.isDir() == true)
        {
            nRetVal = nRetVal + GetAllFileCount(tFileInfo.absoluteFilePath());
            if(tFileInfo.absoluteFilePath().compare(JUKEBOX_MEDIA_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(JUKEBOX_MUSIC_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(JUKEBOX_VIDEO_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(JUKEBOX_PHOTO_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(JUKEBOX_LOST_FOUND_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(JUKEBOX_PHOTO_THUMBNAIL_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(JUKEBOX_PHOTO_THUMBNAIL_USB1_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(JUKEBOX_PHOTO_THUMBNAIL_USB2_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(JUKEBOX_PHOTO_THUMBNAIL_JB_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(FM_APP_PHOTO_THUMBNAIL_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(FM_APP_PHOTO_THUMBNAIL_USB1_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(FM_APP_PHOTO_THUMBNAIL_USB2_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(FM_APP_PHOTO_THUMBNAIL_JB_PATH) != 0
            )
            {
                nRetVal++;
            }
        }
        else
        {
            nRetVal++;
        }
    }

    return nRetVal;
}
*/
int FileManagerThread::GetAllFileSizeOfJukeBox()
{
    return  GetAllFileSize(JUKEBOX_DATA_PATH);
}

int FileManagerThread::GetAllFileSize(const QString &strPath)
{
    int nCount = 0;
    int nRetVal = 0;

    QDir dir(strPath);
    QFileInfoList tFileInfoList = dir.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);

    nCount = tFileInfoList.size();

    for(int i = 0; i < nCount; i++)
    {
        QFileInfo tFileInfo = tFileInfoList.at(i);

        if(tFileInfo.isDir() == true)
        {
            nRetVal = nRetVal + GetAllFileSize(tFileInfo.absoluteFilePath());

            if(tFileInfo.absoluteFilePath().compare(JUKEBOX_MEDIA_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(JUKEBOX_MUSIC_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(JUKEBOX_VIDEO_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(JUKEBOX_PHOTO_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(JUKEBOX_LOST_FOUND_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(JUKEBOX_PHOTO_THUMBNAIL_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(JUKEBOX_PHOTO_THUMBNAIL_USB1_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(JUKEBOX_PHOTO_THUMBNAIL_USB2_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(JUKEBOX_PHOTO_THUMBNAIL_JB_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(FM_APP_PHOTO_THUMBNAIL_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(FM_APP_PHOTO_THUMBNAIL_USB1_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(FM_APP_PHOTO_THUMBNAIL_USB2_PATH) != 0 &&
                tFileInfo.absoluteFilePath().compare(FM_APP_PHOTO_THUMBNAIL_JB_PATH) != 0
            )
            {
                //if dir is not system path, count size of it. because dir has size and it shoud be deleted by format command.
                nRetVal = nRetVal + tFileInfo.size();
            }
        }
        else
        {
            nRetVal =  nRetVal + tFileInfo.size();
        }
    }
    return nRetVal;
}
//} added by jaehwan.lee

//4/////////////////////////////////////////////////////////////////////
//4//
FileManager* FileManager::m_pInstance = 0;

FileManager* FileManager::GetInstance()
{
    if(m_pInstance == 0) {
        m_pInstance = new FileManager();
    }

    return m_pInstance;
}

FileManager::~FileManager()
{
    MC_HIGH << LOG_ENDL;
    
    m_pFileMgrThread->exit();
}

void FileManager::SetPlayerEngine(IPlayerEngine *pPE)
{
    m_pFileMgrThread->SetPlayerEngine(pPE);
}

// ex) strFolderName: /home/meego/strFolderName
int FileManager::CreateFolder(const QString &strFolderName)
{
    return m_pFileMgrThread->StartCreateFolder(strFolderName);
}

// ex) strOldName: /home/meego/strOldName
//     strNewName: /home/meego/strNewName
int FileManager::RenameFolder(const QString &strOldName, const QString &strNewName)
{
    return m_pFileMgrThread->StartRenameFolder(strOldName, strNewName);
}

int FileManager::RenameFile(const QString &strOldName, const QString &strNewName)
{
    return m_pFileMgrThread->StartRenameFile(strOldName, strNewName);
}

// ex) strname: /home/meego/folder_name
//     strname: /home/meego/file_name
int FileManager::DeleteFileFolder(const QString &strName, int nTypeMode, ETAVolumeType volume)
{
    if(nTypeMode == VIDEO_FILE) {
        return m_pFileMgrThread->StartDeleteFileFolder(strName, FileManagerThread::FC_COPY_VIDEO, volume);
    }
    else if(nTypeMode == AUDIO_FILE) {
        return m_pFileMgrThread->StartDeleteFileFolder(strName, FileManagerThread::FC_COPY_MUSIC, volume);
    }
    else if(nTypeMode == IMAGE_FILE) {
        return m_pFileMgrThread->StartDeleteFileFolder(strName, FileManagerThread::FC_COPY_IMAGE, volume);
    }
    else {
        return m_pFileMgrThread->StartDeleteFileFolder(strName, FileManagerThread::FC_COPY_ALL, volume);
    }
}


// ex) strFolderName: /home/meego/src_folder
//     strDestPath:   /home/meego/dst_folder
//     --> result: /home/meego/dst_foler/src_folder
int FileManager::MoveFolder(const QString &strFolderName, const QString &strDestPath)
{
    return m_pFileMgrThread->StartMoveFolder(strFolderName, strDestPath);
}

// ex) strFolderName: /home/meego/src_folder
//     strDestPath:   /home/meego/dst_folder
//     --> result: /home/meego/dst_foler/src_folder
int FileManager::CopyFolder(const QString &strFolderName, const QString &strDestPath, int nTypeMode, ETAVolumeType volume)
{
    if(nTypeMode == VIDEO_FILE) {
        return m_pFileMgrThread->StartCopyFolder(strFolderName, strDestPath, FileManagerThread::FC_COPY_VIDEO, volume);
    }
    else if(nTypeMode == AUDIO_FILE) {
        return m_pFileMgrThread->StartCopyFolder(strFolderName, strDestPath, FileManagerThread::FC_COPY_MUSIC, volume);
    }
    else if(nTypeMode == IMAGE_FILE) {
        return m_pFileMgrThread->StartCopyFolder(strFolderName, strDestPath, FileManagerThread::FC_COPY_IMAGE, volume);
    }
    else {
        return m_pFileMgrThread->StartCopyFolder(strFolderName, strDestPath, FileManagerThread::FC_COPY_ALL, volume);
    }
}

int FileManager::DeleteFileList(const QStringList &strList, int nTypeMode, ETAVolumeType volume)
{
    if(nTypeMode == VIDEO_FILE) {
        return m_pFileMgrThread->StartDeleteList(strList, FileManagerThread::FC_COPY_VIDEO, volume);
    }
    else if(nTypeMode == AUDIO_FILE) {
        return m_pFileMgrThread->StartDeleteList(strList, FileManagerThread::FC_COPY_MUSIC, volume);
    }
    else if(nTypeMode == IMAGE_FILE) {
        return m_pFileMgrThread->StartDeleteList(strList, FileManagerThread::FC_COPY_IMAGE, volume);
    }
    else {
        return m_pFileMgrThread->StartDeleteList(strList, FileManagerThread::FC_COPY_ALL, volume);
    }
}

int FileManager::MoveFileList(const QStringList &strList, const QString &strDestPath, int nTypeMode)
{
    if(nTypeMode == VIDEO_FILE) {
        return m_pFileMgrThread->StartMoveList(strList, strDestPath, FileManagerThread::FC_COPY_VIDEO);
    }
    else if(nTypeMode == AUDIO_FILE) {
        return m_pFileMgrThread->StartMoveList(strList, strDestPath, FileManagerThread::FC_COPY_MUSIC);
    }
    else if(nTypeMode == IMAGE_FILE) {
        return m_pFileMgrThread->StartMoveList(strList, strDestPath, FileManagerThread::FC_COPY_IMAGE);
    }
    else {
        return m_pFileMgrThread->StartMoveList(strList, strDestPath);
    }
}

int FileManager::CopyFileList(const QStringList &strList, const QString &strDestPath, int nTypeMode, ETAVolumeType volume)
{
    m_nCurrentCopyMode = nTypeMode;
    m_nCurrentCopyVolume = volume;
    if(nTypeMode == VIDEO_FILE) {
        return m_pFileMgrThread->StartCopyList(strList, strDestPath, FileManagerThread::FC_COPY_VIDEO, volume);
    }
    else if(nTypeMode == AUDIO_FILE) {
        return m_pFileMgrThread->StartCopyList(strList, strDestPath, FileManagerThread::FC_COPY_MUSIC, volume);
    }
    else if(nTypeMode == IMAGE_FILE) {
        return m_pFileMgrThread->StartCopyList(strList, strDestPath, FileManagerThread::FC_COPY_IMAGE, volume);
    }
    else {
        return m_pFileMgrThread->StartCopyList(strList, strDestPath, FileManagerThread::FC_COPY_ALL, volume);
    }
}

qint64 FileManager::GetTotalFileSpace()
{
    return m_pFileMgrThread->GetTotalFileSpace();
}

int FileManager::GetTotalFileNum()
{
    return m_pFileMgrThread->GetTotalFileNum();
}

qint64 FileManager::GetTotalDiscSpace(const QString &strVolumeName)
{
    return m_pFileMgrThread->GetTotalDiscSpace(strVolumeName);
}

qint64 FileManager::GetTotalUsedDiscSpace(const QString &strVolumeName)
{
    return m_pFileMgrThread->GetTotalUsedDiscSpace(strVolumeName);
}

int FileManager::StartDiscFormat(const QString &strVolumeName)
{
    return m_pFileMgrThread->StartDiscFormat(strVolumeName);
}

int FileManager::CancelCopyOperation()
{
    return m_pFileMgrThread->CancelCopy();
}

int FileManager::CancelDeleteOperation()
{
    return m_pFileMgrThread->CancelDelete();
}

int FileManager::CancelFormatOperation()
{
    return m_pFileMgrThread->CancelFormat();
}

int FileManager::SetOverWriteCurrentExistFileMode(int event)
{
    return m_pFileMgrThread->SetOverWriteCurrentExistFileMode(event);
}

int FileManager::ContinueCopying()
{
    return m_pFileMgrThread->ContinueCopying();
}

int FileManager::PauseCopying()
{
    return m_pFileMgrThread->PauseCopying();
}

int FileManager::FindDrive(const QString &strDestPath)
{
    int nFindRoot = -1;
    
    if(strDestPath.count() <= 0) return 0;
    
    for(int i = 0; i < m_tVolumeList.count(); i++)
    {
        DriveEntry tEntry = m_tVolumeList.at(i);

        if(tEntry.m_strVolumeName.compare(QString("/")) == 0) {
            nFindRoot = i;
            continue;
        }
        else {
            QString strTemp = strDestPath.left(tEntry.m_strVolumeName.count());
            if(tEntry.m_strVolumeName.compare(strTemp) == 0)
                return i;
        }
    }

    MC_HIGH << strDestPath << nFindRoot << LOG_ENDL;
    
    return nFindRoot;
}

int FileManager::FindDrive2(const QString &strVolumeName)
{
    int nFindRoot = -1;
    
    if(strVolumeName.count() <= 0) return nFindRoot;

    for(int i = 0; i < m_tVolumeList.count(); i++)
    {
        DriveEntry tEntry = m_tVolumeList.at(i);
        if(tEntry.m_strVolumeName.compare(strVolumeName) == 0)
            return i;
    }
    
    return nFindRoot;
}

qint64 FileManager::GetAvailableSpace(int nIndex)
{
    if(nIndex >= 0 && nIndex < m_tVolumeList.count()) {
        DriveEntry tEntry = m_tVolumeList.at(nIndex);
        MC_HIGH << "tEntry.m_nAvailDiscSpace:" << nIndex << tEntry.m_nAvailDiscSpace << LOG_ENDL;
        return tEntry.m_nAvailDiscSpace;
    }
    
    return 0;
}

qint64 FileManager::GetTotalSpace(int nIndex)
{
    if(nIndex >= 0 && nIndex < m_tVolumeList.count()) {
        DriveEntry tEntry = m_tVolumeList.at(nIndex);
        MC_HIGH << "tEntry.m_nTotalDiscSpace" << nIndex << tEntry.m_nTotalDiscSpace << LOG_ENDL;
        return tEntry.m_nTotalDiscSpace;
    }
    
    return 0;
}

FileManager::FileManager(QObject *parent) : QObject(parent),
m_bUpdatedStorage(false),
m_pFileMgrThread(0),
m_pSTI(0),
m_nCurrentCopyMode(false),
m_nCurrentCopyVolume(eDefaultVolume)
{
    //MC_HIGH << "===========================================" << LOG_ENDL;
    //MC_HIGH << "Create File Manager" << LOG_ENDL;
    //MC_HIGH << "===========================================" << LOG_ENDL;
    
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    #if FM_USE_QSYSTEMSTORAGEINFO        
    m_pSTI = new QSystemStorageInfo(this);
    #endif

    UpdateStorage();
    
    m_pFileMgrThread = FileManagerThread::GetInstance();

    connect(m_pFileMgrThread, SIGNAL(OnPercentageAndSizeCopied(int, qint64)),                   this, SIGNAL(OnPercentageAndSizeCopied(int, qint64)));
    connect(m_pFileMgrThread, SIGNAL(OnPercentageAndSizeMoved(int, qint64)),                    this, SIGNAL(OnPercentageAndSizeMoved(int, qint64)));
    connect(m_pFileMgrThread, SIGNAL(OnPercentageAndSizeDeleted(int, qint64)),                  this, SIGNAL(OnPercentageAndSizeDeleted(int, qint64)));
    connect(m_pFileMgrThread, SIGNAL(OnPercentageAndSizeAndNameDeleted(int, qint64, QString)),  this, SIGNAL(OnPercentageAndSizeAndNameDeleted(int, qint64, QString)));
    connect(m_pFileMgrThread, SIGNAL(OnFileCopiedPercentage(int, int)),                         this, SIGNAL(OnFileCopiedPercentage(int, int)));
    connect(m_pFileMgrThread, SIGNAL(OnFileMovedPercentage(int, int)),                          this, SIGNAL(OnFileMovedPercentage(int, int)));
    connect(m_pFileMgrThread, SIGNAL(OnCreateFinished(int)),                                    this, SIGNAL(OnCreateFinished(int)));
    connect(m_pFileMgrThread, SIGNAL(OnRenameFinished(int,int)),                                this, SIGNAL(OnRenameFinished(int,int)));
    connect(m_pFileMgrThread, SIGNAL(OnDeleteFinished(int,int,int)),                            this, SIGNAL(OnDeleteFinished(int,int,int)));
    connect(m_pFileMgrThread, SIGNAL(OnMoveFinished(int,int)),                                  this, SIGNAL(OnMoveFinished(int,int)));
    connect(m_pFileMgrThread, SIGNAL(OnCopyFinished(int,int,int,int,int)),                      this, SLOT(slotOnCopyFinished(int,int,int,int,int))); // modified by ravikanth 22-04-13
    connect(m_pFileMgrThread, SIGNAL(OnFormatFinished(int)),                                    this, SIGNAL(OnFormatFinished(int)));
    connect(m_pFileMgrThread, SIGNAL(OnError(int,QString)),                                     this, SIGNAL(OnError(int, QString)));
    connect(m_pFileMgrThread, SIGNAL(OnFileMoved(QString)),                                     this, SIGNAL(OnFileMoved(QString)));
    
    m_pFileMgrThread->start();
}

void FileManager::GetMountEntries()
{
    m_tMountEntriesMap.clear();

    FILE *mntfp = setmntent( _PATH_MOUNTED, "r" );
    mntent *me = getmntent(mntfp);
    bool ok;
    while(me != NULL) {
        struct statfs fs;
        ok = false;
        if (strcmp(me->mnt_type, "cifs") != 0) { //smb has probs with statfs
            if (statfs(me->mnt_dir, &fs ) ==0 ) {
                QString num;
                // weed out a few types
                if ( fs.f_type != 0x01021994 //tmpfs
                     && fs.f_type != 0x9fa0 //procfs
                     && fs.f_type != 0x1cd1 //
                     && fs.f_type != 0x62656572
                     && (unsigned)fs.f_type != 0xabababab // ???
                     && fs.f_type != 0x52654973
                     && fs.f_type != 0x42494e4d
                     && fs.f_type != 0x64626720
                     && fs.f_type != 0x73636673 //securityfs
                     && fs.f_type != 0x65735543 //fusectl
                     && fs.f_type != 0x65735546 // fuse.gvfs-fuse-daemon

                     ) {
                    ok = true;
                }
            }
        } else {
            ok = true;
        }
        if (ok && !m_tMountEntriesMap.keys().contains(me->mnt_dir)
            && QString(me->mnt_fsname).left(1) == "/") {
            m_tMountEntriesMap[me->mnt_dir] = me->mnt_fsname;
        }

        me = getmntent(mntfp);
    }
    endmntent(mntfp);
}

QStringList FileManager::GetLogicalDrives()
{
    GetMountEntries();
    return m_tMountEntriesMap.keys();
}

qint64 FileManager::GetTotalSpaceOfVolume(const QString &strDriveVolume)
{
    if(strDriveVolume.left(2) == "//") {
        return 0;
    }
    
    struct statfs fs;
    
    if(statfs(strDriveVolume.toLatin1(), &fs) == 0 ) {
        double blockSize   = fs.f_bsize;
        double totalBlocks = fs.f_blocks;
        return (qint64)(totalBlocks * blockSize);
    }
    
    return 0;
}

qint64 FileManager::GetAvailableSpaceOfVolume(const QString &strDriveVolume)
{
    if(strDriveVolume.left(2) == "//") {
        return 0;
    }

    struct statfs fs;
    
    if(statfs(strDriveVolume.toLatin1(), &fs ) == 0 ) {
        double blockSize = (double)fs.f_bsize;
        double availBlocks = (double)fs.f_bavail;
        return (qint64)(availBlocks * blockSize);
    }
    
    return 0;
}

void FileManager::UpdateStorage()
{
    PT_START_TIME(PT_OBJ_NAME_UPDATE_STORAGE, 0);

    m_bUpdatedStorage = true;
    
    m_tVolumeList.clear();
#if FM_USE_QSYSTEMSTORAGEINFO
    QStringList tVolList = m_pSTI->logicalDrives();
    
    foreach(QString strTempVolName, tVolList)
    {
        DriveEntry tEntry;

        tEntry.m_strVolumeName   = strTempVolName;
        tEntry.m_nTotalDiscSpace = m_pSTI->totalDiskSpace(strTempVolName);
        tEntry.m_nAvailDiscSpace = m_pSTI->availableDiskSpace(strTempVolName);
        
        m_tVolumeList.append(tEntry);
    }
#else
    QStringList tVolList = GetLogicalDrives();
    
    foreach(QString strTempVolName, tVolList)
    {
        DriveEntry tEntry;

        tEntry.m_strVolumeName = strTempVolName;
        if(strTempVolName.contains(JUKEBOX_DATA_PATH)) {
            tEntry.m_nTotalDiscSpace = GetTotalSpaceOfVolume(strTempVolName) - JUKEBOX_SYSTEM_DISC_SPACE;
            tEntry.m_nAvailDiscSpace = GetAvailableSpaceOfVolume(strTempVolName);
            MC_HIGH << strTempVolName << "TOTAL:" << tEntry.m_nTotalDiscSpace << "AVAIL:" << tEntry.m_nAvailDiscSpace << LOG_ENDL;
        }
        else {
            tEntry.m_nTotalDiscSpace = GetTotalSpaceOfVolume(strTempVolName);
            tEntry.m_nAvailDiscSpace = GetAvailableSpaceOfVolume(strTempVolName);
            if(strTempVolName.contains("/media"))
                MC_HIGH << strTempVolName << "TOTAL:" << tEntry.m_nTotalDiscSpace << "AVAIL:" << tEntry.m_nAvailDiscSpace << LOG_ENDL;
        }
        
        m_tVolumeList.append(tEntry);
    }
#endif

    PT_END_TIME(PT_OBJ_NAME_UPDATE_STORAGE, "FileManager::UpdateStorage()");
}


void FileManager::SetCopySpeed(int nSpeed)
{
    m_pFileMgrThread->SetCopySpeed(nSpeed);
}

int FileManager::IsUpdatedStorage()

{
    return m_bUpdatedStorage;
}

void FileManager::SetUpdatedStorage(int bUpdate)
{
    m_bUpdatedStorage = bUpdate;
}

void FileManager::slotOnCopyFinished(int bSuccess, int nSkipCount, int nFilesCount, int nMode, int nVolume)
{
    int mode = FileManagerThread::FC_COPY_ALL;
    if(m_nCurrentCopyMode == VIDEO_FILE) {
        mode = FileManagerThread::FC_COPY_VIDEO;
    }
    else if(m_nCurrentCopyMode == AUDIO_FILE) {
        mode = FileManagerThread::FC_COPY_MUSIC;
    }
    else if(m_nCurrentCopyMode == IMAGE_FILE) {
        mode = FileManagerThread::FC_COPY_IMAGE;
    }
    else {
        // no Implementation
    }
    MC_HIGH << "eMediaType= " << nMode  << " nVolume = "
               <<   nVolume << " m_nCurrentCopyMode = " <<  mode
                  << " m_nCurrentCopyVolume = " << m_nCurrentCopyVolume;
    emit OnCopyFinished(bSuccess, nSkipCount, nFilesCount, nMode, nVolume, mode, m_nCurrentCopyVolume);
}
