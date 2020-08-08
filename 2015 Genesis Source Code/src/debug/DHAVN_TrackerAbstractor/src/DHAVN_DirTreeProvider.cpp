#include <gio/gio.h>
#include "DHAVN_DirTreeProvider.h"
#include <QDirIterator>
#include <QDebug>
#include <QMutex>
#include <QUrl>
#include <QByteArray>
#include <MediaInfo/MediaInfo.h>
#include <QQueue>
#include <dirent.h>    // added by sungha.choi 2013.07.13
#include "DHAVN_LockedFile.h"   // added by sungha.choi 2013.08.18 for AVP crash (file read/write lock)

#define DT_DIR_DEPTH_LIMIT	20
#define DT_DIR_MEDIA_LIMIT  6000

QString gTA_VERSION                     = "2013.05.08.02";
const char *gScanResult_path			= "/app/data/TrackerAbs/";
const char *gScanResult_JukeBox_path	= "/app/data/TrackerAbs/jukebox_fs.txt";
const char *gScanResult_USB1_path		= "/app/data/TrackerAbs/usb1_fs.txt";
const char *gScanResult_USB2_path		= "/app/data/TrackerAbs/usb2_fs.txt";

// added by kihyung 2013.07.09. It is same tracker open source
#define FILE_ATTRIBUTES	  \
	G_FILE_ATTRIBUTE_STANDARD_NAME "," \
	G_FILE_ATTRIBUTE_STANDARD_TYPE

CDirTreeProvider* CDirTreeProvider::m_pInstance = 0;

CDirTree::CDirTree()
{
    LOG_INIT_V2("Media_TrackerAbstractor");

    m_tNullDirInfo.nIsValid = 0;
    m_tNullFileInfo.nIsValid = 0;
    m_pFileTable = NULL;   // added by sungha.choi 2013.07.31 for scandir performance
    Init();
}

CDirTree::~CDirTree()
{
}

void CDirTree::Init()
{
    qDebug("CDirTree::Init()");
    m_nPlayDirIdx  = -1;
    m_nPlayFileIdx = -1;
    m_tDirList.clear();
    m_tAudioFileList.clear();
    m_tVideoFileList.clear();
    m_tPhotoFileList.clear();
}

int CDirTree::GetDirNum()
{
    return m_tDirList.count();
}

int CDirTree::GetAudioFileNum()
{
    return m_tAudioFileList.count();
}

int CDirTree::GetVideoFileNum()
{
    return m_tVideoFileList.count();
}

int CDirTree::GetPhotoFileNum()
{
    return m_tPhotoFileList.count();
}

int CDirTree::GetMediaFileNum()
{
    return GetAudioFileNum() + GetVideoFileNum() + GetPhotoFileNum(); 
}

QString CDirTree::GetFirstMediaFolder(DT_CATEGORY eCategory)
{
    if(eCategory != eDT_NUM) {
        for(int i = 0; i < m_tDirList.size(); i++)
        {
            CDTDirInfo &tDirInfo = m_tDirList[i];
            if(tDirInfo.pFileNum[eCategory] != 0) {
                return tDirInfo.strName;
            }
        }
    }

    return QString();
}

bool CDirTree::AddDir(CDTDirInfo &tDirInfo)
{
    m_tDirList.append(tDirInfo);
    return true;
}

bool CDirTree::AddAudioFile(CDTFileInfo &tFileInfo)
{
    m_tAudioFileList.append(tFileInfo);
    return true;
}

bool CDirTree::AddVideoFile(CDTFileInfo &tFileInfo)
{
    m_tVideoFileList.append(tFileInfo);
    return true;
}

bool CDirTree::AddPhotoFile(CDTFileInfo &tFileInfo)
{
    m_tPhotoFileList.append(tFileInfo);
    return true;
}

CDTDirInfo& CDirTree::GetDirInfo(int nIdx)
{
    if(!m_tDirList.isEmpty() && nIdx >= 0 && nIdx < m_tDirList.size())
        return m_tDirList[nIdx];
    else
        return m_tNullDirInfo;
}

CDTFileInfo& CDirTree::GetAudioFileInfo(int nIdx)
{
    if(!m_tAudioFileList.isEmpty() && nIdx >= 0 && nIdx < m_tAudioFileList.size())
        return m_tAudioFileList[nIdx];
    else
        return m_tNullFileInfo;
}

CDTFileInfo& CDirTree::GetVideoFileInfo(int nIdx)
{
    if(!m_tVideoFileList.isEmpty() && nIdx >= 0 && nIdx < m_tVideoFileList.size())
        return m_tVideoFileList[nIdx];
    else
        return m_tNullFileInfo;    
}
        
CDTFileInfo& CDirTree::GetPhotoFileInfo(int nIdx)
{
    if(!m_tPhotoFileList.isEmpty() && nIdx >= 0 && nIdx < m_tPhotoFileList.size())
        return m_tPhotoFileList[nIdx];
    else
        return m_tNullFileInfo;    
}

void CDirTree::PrintDirTree()
{
    qDebug("CDirTree::PrintDirTree() --------------------------------------");
    qDebug("CDirTree::PrintDirTree(): DIR LIST");
    for(int i = 0; i < m_tDirList.size(); i++)
    {
        CDTDirInfo &tDirInfo = m_tDirList[i];
        qDebug("CDirTree::PrintDirTree(): [%d/%d] P(%d), D(%d, %d), A(%d, %d), V(%d, %d), P(%d, %d), N:%s", 
               i, m_tDirList.size(),
               tDirInfo.nParentDirIdx, 
               tDirInfo.nSubDirIdx, tDirInfo.nSubDirNum,
               tDirInfo.pFileIdx[eDT_AUDIO], tDirInfo.pFileNum[eDT_AUDIO], 
               tDirInfo.pFileIdx[eDT_VIDEO], tDirInfo.pFileNum[eDT_VIDEO], 
               tDirInfo.pFileIdx[eDT_PHOTO], tDirInfo.pFileNum[eDT_PHOTO], 
               qPrintable(tDirInfo.strName));
    }

    qDebug("CDirTree::PrintDirTree(): AUDIO LIST");
    for(int i = 0; i < m_tAudioFileList.size(); i++)
    {
        CDTFileInfo &tFileInfo = m_tAudioFileList[i];
        qDebug("CDirTree::PrintDirTree(): P(%d), N:%s", tFileInfo.nParentDirIdx, qPrintable(tFileInfo.strName));
    }
    
    qDebug("CDirTree::PrintDirTree(): VIDEO LIST");
    for(int i = 0; i < m_tVideoFileList.size(); i++)
    {
        CDTFileInfo &tFileInfo = m_tVideoFileList[i];
        qDebug("CDirTree::PrintDirTree(): P(%d), N:%s", tFileInfo.nParentDirIdx, qPrintable(tFileInfo.strName));
    }
    
    qDebug("CDirTree::PrintDirTree(): PHOTO LIST");
    for(int i = 0; i < m_tPhotoFileList.size(); i++)
    {
        CDTFileInfo &tFileInfo = m_tPhotoFileList[i];
        qDebug("CDirTree::PrintDirTree(): P(%d), N:%s", tFileInfo.nParentDirIdx, qPrintable(tFileInfo.strName));
    }
    qDebug("CDirTree::PrintDirTree() --------------------------------------");    
}

bool CDirTree::Read(const char *pFileName, int nTreeMode)
{
    int             i;
    int             nCount;
    int             totalMediaCount = 0;    // added by sungha.choi 2013.07.31
    QtLockedFile    inFile(pFileName);  // modified by sungha.choi 2013.08.18 for AVP crash (file read/write lock)
    if(inFile.open(QIODevice::ReadOnly) == false) {
        { QString a; LOG_CRITICAL << a.sprintf("CDirTree::Read %s file not open", pFileName) << LOG_ENDL; }
        return false;
    }

    inFile.lock(QtLockedFile::ReadLock, true);    // added by sungha.choi 2013.08.18 for AVP crash (file read/write lock)

    Init();

    QString     strVersion;
    QTextStream inFileStream(&inFile);
    
    inFileStream.skipWhiteSpace();

    strVersion = inFileStream.readLine();   // added by sungha.choi 2013.08.25 for AFM crash ITS 0182950
    //inFileStream >> strVersion;   // deleted by sungha.choi 2013.08.25 for AFM crash ITS 0182950
    if(strVersion.compare(gTA_VERSION) != 0) {
        { QString a; LOG_CRITICAL << a.sprintf("CDirTree::Read() version is different %s != %s", qPrintable(strVersion), qPrintable(gTA_VERSION)) << LOG_ENDL; }
        inFile.unlock();    // added by sungha.choi 2013.08.18 for AVP crash (file read/write lock)
        inFile.close();
        return false;
    }

    nCount = inFileStream.readLine().toInt();   // added by sungha.choi 2013.08.25 for AFM crash ITS 0182950
    //inFileStream >> nCount;   // deleted by sungha.choi 2013.08.25 for AFM crash ITS 0182950
    qDebug("CDirTree::Read() OK D: Count: %d", nCount);
    for(i = 0; i < nCount; i++)
    {
        QString lineStr = inFileStream.readLine();   // added by sungha.choi 2013.08.25 for AFM crash ITS 0182950
        QTextStream inLineStream(&lineStr);   // added by sungha.choi 2013.08.25 for AFM crash ITS 0182950

        CDTDirInfo tDirInfo;
        
        inLineStream >> tDirInfo.nParentDirIdx   // modified by sungha.choi 2013.08.25 for AFM crash ITS 0182950
                     >> tDirInfo.nSubDirIdx >> tDirInfo.nSubDirNum
                     >> tDirInfo.pFileIdx[eDT_AUDIO] >> tDirInfo.pFileNum[eDT_AUDIO]
                     >> tDirInfo.pFileIdx[eDT_VIDEO] >> tDirInfo.pFileNum[eDT_VIDEO] 
                     >> tDirInfo.pFileIdx[eDT_PHOTO] >> tDirInfo.pFileNum[eDT_PHOTO] 
                     >> tDirInfo.pIsValid[eDT_AUDIO] >> tDirInfo.pIsValid[eDT_VIDEO] >> tDirInfo.pIsValid[eDT_PHOTO] 
                     >> tDirInfo.strName;
        
        tDirInfo.strName.replace("*", " ");
        AddDir(tDirInfo);

        if(nTreeMode == DT_FS_SCAN_MODE && inFileStream.atEnd() == true) break;
    }
    
    if(nTreeMode == DT_FS_SCAN_MODE && i < nCount) {
        inFile.unlock();    // added by sungha.choi 2013.08.18 for AVP crash (file read/write lock)
        inFile.close();
        return false;
    }
    
    nCount = inFileStream.readLine().toInt();   // added by sungha.choi 2013.08.25 for AFM crash ITS 0182950
    //inFileStream >> nCount;   // deleted by sungha.choi 2013.08.25 for AFM crash ITS 0182950
    qDebug("CDirTree::Read() OK A: Count: %d", nCount);
    for(i = 0; i < nCount; i++)
    {
        QString lineStr = inFileStream.readLine();   // added by sungha.choi 2013.08.25 for AFM crash ITS 0182950
        QTextStream inLineStream(&lineStr);   // added by sungha.choi 2013.08.25 for AFM crash ITS 0182950

        CDTFileInfo tFileInfo;
        inLineStream >> tFileInfo.nParentDirIdx >> tFileInfo.strName;   // modified by sungha.choi 2013.08.25 for AFM crash ITS 0182950
        tFileInfo.strName.replace("*", " ");
        AddAudioFile(tFileInfo);

        if(m_pFileTable) {   // { added by sungha.choi 2013.07.31 for scandir performance
            QString fullName = m_tDirList[tFileInfo.nParentDirIdx].strName + '/' + tFileInfo.strName;   // added by sungha.choi 2013.07.31
            m_pFileTable->insert(fullName, totalMediaCount++);
        }   // } added by sungha.choi 2013.07.31 for scandir performance

        if(nTreeMode == DT_FS_SCAN_MODE && inFileStream.atEnd() == true) break;
    }
    
    if(nTreeMode == DT_FS_SCAN_MODE && i < nCount) {
        inFile.unlock();    // added by sungha.choi 2013.08.18 for AVP crash (file read/write lock)
        inFile.close();
        return false;
    }

    nCount = inFileStream.readLine().toInt();   // added by sungha.choi 2013.08.25 for AFM crash ITS 0182950
    //inFileStream >> nCount;   // deleted by sungha.choi 2013.08.25 for AFM crash ITS 0182950
    qDebug("CDirTree::Read() OK V: Count: %d", nCount);
    for(i = 0; i < nCount; i++)
    {
        QString lineStr = inFileStream.readLine();   // added by sungha.choi 2013.08.25 for AFM crash ITS 0182950
        QTextStream inLineStream(&lineStr);   // added by sungha.choi 2013.08.25 for AFM crash ITS 0182950

        CDTFileInfo tFileInfo;
        inLineStream >> tFileInfo.nParentDirIdx >> tFileInfo.strName;   // modified by sungha.choi 2013.08.25 for AFM crash ITS 0182950
        tFileInfo.strName.replace("*", " ");
        AddVideoFile(tFileInfo);

        if(m_pFileTable) {   // { added by sungha.choi 2013.07.31 for scandir performance
            QString fullName = m_tDirList[tFileInfo.nParentDirIdx].strName + '/' + tFileInfo.strName;   // added by sungha.choi 2013.07.31
            m_pFileTable->insert(fullName, totalMediaCount++);
        }   // } added by sungha.choi 2013.07.31 for scandir performance

        if(nTreeMode == DT_FS_SCAN_MODE && inFileStream.atEnd() == true) break;
    }
    
    if(nTreeMode == DT_FS_SCAN_MODE && i < nCount) {
        inFile.unlock();    // added by sungha.choi 2013.08.18 for AVP crash (file read/write lock)
        inFile.close();
        return false;
    }

    nCount = inFileStream.readLine().toInt();   // added by sungha.choi 2013.08.25 for AFM crash ITS 0182950
    //inFileStream >> nCount;   // deleted by sungha.choi 2013.08.25 for AFM crash ITS 0182950
    qDebug("CDirTree::Read() OK P: Count: %d", nCount);
    for(i = 0; i < nCount; i++)
    {
        QString lineStr = inFileStream.readLine();   // added by sungha.choi 2013.08.25 for AFM crash ITS 0182950
        QTextStream inLineStream(&lineStr);   // added by sungha.choi 2013.08.25 for AFM crash ITS 0182950

        CDTFileInfo tFileInfo;
        inLineStream >> tFileInfo.nParentDirIdx >> tFileInfo.strName;   // modified by sungha.choi 2013.08.25 for AFM crash ITS 0182950
        tFileInfo.strName.replace("*", " ");
        AddPhotoFile(tFileInfo);

        if(m_pFileTable) {   // { added by sungha.choi 2013.07.31 for scandir performance
            QString fullName = m_tDirList[tFileInfo.nParentDirIdx].strName + '/' + tFileInfo.strName;   // added by sungha.choi 2013.07.31
            m_pFileTable->insert(fullName, totalMediaCount++);   // added by sungha.choi 2013.07.31
        }   // } added by sungha.choi 2013.07.31 for scandir performance

        if(nTreeMode == DT_FS_SCAN_MODE && inFileStream.atEnd() == true) break;
    }
    if(nTreeMode == DT_FS_SCAN_MODE && i < nCount-1) {  // modified by honggi.shin, 2014.07.09, photo reading operation finished by endline.
        inFile.unlock();    // added by sungha.choi 2013.08.18 for AVP crash (file read/write lock)
        inFile.close();
        return false;
    }
    
    inFile.unlock();    // added by sungha.choi 2013.08.18 for AVP crash (file read/write lock)
    inFile.close();
    return true;
}

bool CDirTree::Write(const char *pFileName)
{
    QFileInfo tCacheDirInfo(gScanResult_path);
    QDir tCacheDir(gScanResult_path);
    if(!tCacheDirInfo.exists() || !tCacheDirInfo.isDir()) {
        tCacheDir.mkdir(gScanResult_path);
    }

#if 0   // deleted by sungha.choi 2013.08.18 no need
    QFileInfo tFileInfo(pFileName);
    if(tFileInfo.exists()) {
        QFile::remove(pFileName);
    }
#endif
    QtLockedFile  outFile(pFileName);   // modified by sungha.choi 2013.08.18 for AVP crash (file read/write lock)
    //if(outFile.open(QIODevice::WriteOnly | QIODevice::Truncate) == false) {
    if(outFile.open(QIODevice::WriteOnly) == false) { // modified by sungha.choi 2013.08.18 for AVP crash (file read/write lock)
        qDebug("CDirTree::Write %s file not open", pFileName);
        return false;
    }

    outFile.lock(QtLockedFile::WriteLock, true);    // added by sungha.choi 2013.08.18 for AVP crash (file read/write lock)

    QTextStream outFileStream(&outFile);

    outFileStream << gTA_VERSION << endl;

    outFileStream << m_tDirList.size() << endl;
    for(int i = 0; i < m_tDirList.size(); i++)
    {
        CDTDirInfo &tDirInfo = m_tDirList[i];

        QString strName  = tDirInfo.strName;
        strName.replace(" ", "*");
        
        outFileStream << tDirInfo.nParentDirIdx << " "
                      << tDirInfo.nSubDirIdx << " " 
                      << tDirInfo.nSubDirNum << " "
                      << tDirInfo.pFileIdx[eDT_AUDIO] << " " 
                      << tDirInfo.pFileNum[eDT_AUDIO] << " "
                      << tDirInfo.pFileIdx[eDT_VIDEO] << " "
                      << tDirInfo.pFileNum[eDT_VIDEO] << " " 
                      << tDirInfo.pFileIdx[eDT_PHOTO] << " " 
                      << tDirInfo.pFileNum[eDT_PHOTO] << " " 
                      << tDirInfo.pIsValid[eDT_AUDIO] << " " 
                      << tDirInfo.pIsValid[eDT_VIDEO] << " " 
                      << tDirInfo.pIsValid[eDT_PHOTO] << " " 
                      << strName << endl;
    }

    outFileStream << m_tAudioFileList.size() << endl;
    for(int i = 0; i < m_tAudioFileList.size(); i++)
    {
        CDTFileInfo &tFileInfo = m_tAudioFileList[i];
        
        QString strName  = tFileInfo.strName;
        strName.replace(" ", "*");
        
        outFileStream << tFileInfo.nParentDirIdx << " " << strName << endl;
    }

    outFileStream << m_tVideoFileList.size() << endl;
    for(int i = 0; i < m_tVideoFileList.size(); i++)
    {
        CDTFileInfo &tFileInfo = m_tVideoFileList[i];

        QString strName  = tFileInfo.strName;
        strName.replace(" ", "*");
        
        outFileStream << tFileInfo.nParentDirIdx << " " << strName << endl;
    }

    outFileStream << m_tPhotoFileList.size() << endl;
    for(int i = 0; i < m_tPhotoFileList.size(); i++)
    {
        CDTFileInfo &tFileInfo = m_tPhotoFileList[i];

        QString strName  = tFileInfo.strName;
        strName.replace(" ", "*");
        
        outFileStream << tFileInfo.nParentDirIdx << " " << strName << endl;
    }
    outFile.flush();
    outFile.unlock();    // added by sungha.choi 2013.08.18 for AVP crash (file read/write lock)
    outFile.close();

    return true;
}

void CDirTree::SetHashTable(QHash<QString, int> *pFileTable)   // { added by sungha.choi 2013.07.31 for scandir performance
{
    m_pFileTable = pFileTable;
}   // } added by sungha.choi 2013.07.31 for scandir performance

// { added by kihyung for ITS 0181967 
bool CDirTree::GetFSFolderInfo(QList<FSFolderInfo> &tFolderList, ETrackerAbstarctor_MediaType mediaType)
{
    if(mediaType == eFSAudio)
    {
        for(int i = 0; i < m_tDirList.count(); i++)
        {
            CDTDirInfo &tDirInfo = m_tDirList[i];
            if(tDirInfo.pFileNum[eDT_AUDIO] <= 0) continue;
            
            FSFolderInfo tFolderInfo = {tDirInfo.pFileNum[eDT_AUDIO], tDirInfo.pFileIdx[eDT_AUDIO], 0};
            tFolderList.append(tFolderInfo);
        }
        return true;
    }
    else if(mediaType == eFSVideo)
    {
        for(int i = 0; i < m_tDirList.count(); i++)
        {
            CDTDirInfo  &tDirInfo = m_tDirList[i];
            if(tDirInfo.pFileNum[eDT_VIDEO] <= 0) continue;
            
            FSFolderInfo tFolderInfo = {tDirInfo.pFileNum[eDT_VIDEO], tDirInfo.pFileIdx[eDT_VIDEO], 0};
            tFolderList.append(tFolderInfo);
        }
        return true;
    }
    else if(mediaType == eFSPhoto)
    {
        for(int i = 0; i < m_tDirList.count(); i++)
        {
            CDTDirInfo  &tDirInfo = m_tDirList[i];
            if(tDirInfo.pFileNum[eDT_PHOTO] <= 0) continue;
            
            FSFolderInfo tFolderInfo = {tDirInfo.pFileNum[eDT_PHOTO], tDirInfo.pFileIdx[eDT_PHOTO], 0};
            tFolderList.append(tFolderInfo);
        }
        return true;
    }
    else
    {
        return false;
    }
}
// } added by kihyung for ITS 0181967
 
CDirTreeThread::CDirTreeThread(ETrackerAbstarctor_VolumeType volumeType, QObject *parent) : QThread(parent),
m_nWorking(0)    
{
    LOG_INIT_V2("Media_TrackerAbstractor");

    qRegisterMetaType<ETrackerAbstarctor_Query_Ids>("ETrackerAbstarctor_Query_Ids");
    qRegisterMetaType<ETrackerAbstarctor_VolumeType>("ETrackerAbstarctor_VolumeType");
    qRegisterMetaType<QVector<QStringList> >();
    
    m_bStop             = false;
	m_nTreeMode         = DT_FS_FIND_MODE; // added by eugene.seo 2013.03.27 for static test
	m_bRescanMode       = false; // added by eugene.seo 2013.03.27 for static test
    m_volumeType        = volumeType;
    m_bDirTreeScanMode  = true;
    m_bScanReadComplete = false;
    m_bAudioFound       = false;
    m_bVideoFound       = false;
    m_bPhotoFound       = false;
    m_bAllMediaFound    = false;

    // { modified by eugene.seo 2013.04.17
    m_strAudioFmt <<".m4a"<<".mp3"<<".oga"<<".ogg"<<".wav"<<".wma"
		          <<".M4A"<<".MP3"<<".OGA"<<".OGG"<<".WAV"<<".WMA"; 
    m_strVideoFmt <<".avi"<<".divx"<<".m2ts"<<".m4v"<<".mkv"<<".mp4"<<".mpg"<<".mpeg"<<".ps"<<".tp"<<".ts"<<".vod"<<".wmv"
                  <<".AVI"<<".DIVX"<<".M2TS"<<".M4V"<<".MKV"<<".MP4"<<".MPG"<<".MPEG"<<".PS"<<".TP"<<".TS"<<".VOD"<<".WMV";   
    m_strPhotoFmt <<".bmp"<<".gif"<<".jpg"<<".jpeg"<<".png"<<".tif"<<".tiff"
		          <<".BMP"<<".GIF"<<".JPG"<<".JPEG"<<".PNG"<<".TIF"<<".TIFF";
    m_strAudioVideoFmt <<".asf"<<".ASF";
    // } modified by eugene.seo 2013.04.17

    // { added by honggi.shin, 2014.03.10, for ignoring UNSUPPORTED directory.
    m_strIgnoreDir <<"po"<<"CVS"<<"core-dumps"<<"lost+found"<<"/media/sdcard"<<"$RECYCLE.BIN";
    // } added by honggi.shin, 2014.03.10, END

    m_strMountPath.clear();
}

CDirTreeThread::~CDirTreeThread()
{
}

void CDirTreeThread::SetFileSystemScanMode(bool bScan)
{
    m_bDirTreeScanMode = bScan;    
}

bool CDirTreeThread::StartScanFileSystem(QString strRootPath, bool bRescanMode)
{
    m_tMutex.lock();
    if(m_nWorking == 0) 
    {
        m_nWorking    = 1;
        m_bStop       = false;
        m_nTreeMode   = DT_FS_SCAN_MODE;
        m_bRescanMode = bRescanMode;
        m_strRootPath = strRootPath;
        m_bAudioFound = false;
        m_bVideoFound = false;
        m_bPhotoFound = false;  
        m_bAllMediaFound = false;
        qDebug("CDirTreeThread::StartScanFileSystem(%s)", qPrintable(strRootPath));
        m_tCond.wakeAll();
    }
    m_tMutex.unlock();
    return true;
}

bool CDirTreeThread::StopScanFileSystem()
{
    m_tMutex.lock();
    qDebug("CDirTreeThread::StopScanFileSystem() m_nWorking == %d", m_nWorking);
    if(m_nWorking == 1) 
    {
        m_tStopMutex.lock();
        m_bStop = true;
        m_tStopMutex.unlock();
    }
    else if(m_nWorking == 0)
    {
        Init();  // modified by kihyung 2013.07.22 for ITS 0180971
    }
    m_tMutex.unlock();
    return true;    
}

bool CDirTreeThread::StartReadScanResult()
{
    m_tMutex.lock();
    if(m_nWorking == 0) 
    {
        m_nWorking    = 1;
        m_bStop       = false;
        m_nTreeMode   = DT_FS_READ_MODE;
        m_bRescanMode = false;
        qDebug("CDirTreeThread::StartReadScanResult()");
        //m_tCond.wakeAll();   // deleted by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
        ReadDirInSelfThread();   // added by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    }
    m_tMutex.unlock();    
    return true;
}

bool CDirTreeThread::StopReadScanResult()
{
    m_tMutex.lock();
    qDebug("CDirTreeThread::StartReadScanResult() m_nWorking == %d", m_nWorking);
    if(m_nWorking == 1) 
    {
        m_tStopMutex.lock();
        m_bStop = true;
        m_tStopMutex.unlock();
    }
    else if(m_nWorking == 0)
    {
        Init();  // modified by kihyung 2013.07.22 for ITS 0180971
    }
    m_tMutex.unlock();
    return true;
}

int CDirTreeThread::GetAudioFileNum()
{
    int nRet = 0;
    m_tMutex.lock();
    if(m_nWorking == 0) 
    {
        nRet = m_tDirTree.GetAudioFileNum();
    }
    m_tMutex.unlock();
    return nRet;
}

int CDirTreeThread::GetVideoFileNum()
{
    int nRet = 0;
    m_tMutex.lock();
    if(m_nWorking == 0) 
    {
        nRet = m_tDirTree.GetVideoFileNum();    
    }
    m_tMutex.unlock();
    return nRet;
}

int CDirTreeThread::GetPhotoFileNum()
{
    int nRet = 0;
    m_tMutex.lock();
    if(m_nWorking == 0) 
    {
        nRet = m_tDirTree.GetPhotoFileNum();        
    }
    m_tMutex.unlock();
    return nRet;
}

QString CDirTreeThread::GetFirstMediaFolder(DT_CATEGORY eCategory)
{
    QString strRet("");
    m_tMutex.lock();
    if(m_nWorking == 0) 
    {
        strRet = m_tDirTree.GetFirstMediaFolder(eCategory);   
    }
    m_tMutex.unlock();
    return strRet;
}

int CDirTreeThread::GetAllAudioFileList(uint nReqID)
{
    QVector<QStringList> queryDataList;

    queryDataList.clear();

    m_tMutex.lock();
    if(m_nWorking == 0) 
    {
        qDebug("CDirTreeThread::GetAllAudioFileList(). Start");
        
        QStringList slist;

        for(int i = 0; i < m_tDirTree.GetAudioFileNum(); i++)
        {
            CDTFileInfo &tFileInfo = m_tDirTree.GetAudioFileInfo(i);
            if(tFileInfo.nIsValid == 0) {
                qDebug("CDirTreeThread::GetAllAudioFileList(). tFileInfo(%d) is invalid", i);
                break;
            }

            CDTDirInfo &tDirInfo = m_tDirTree.GetDirInfo(tFileInfo.nParentDirIdx);
            if(tDirInfo.nIsValid == 0) {
                qDebug("CDirTreeThread::GetAllAudioFileList(). tDirInfo(%d) is invalid", tFileInfo.nParentDirIdx);
                break;
            }

            slist.append(tDirInfo.strName + "/" + tFileInfo.strName);
        }

        if(slist.size() > 0)
            queryDataList.append(slist);

        qDebug("CDirTreeThread::GetAllAudioFileList(). End(%d->%d)", slist.size(), queryDataList.count());
    }
    m_tMutex.unlock();

    emit DataDirectFetched(queryDataList, eGetMusicLimitFileListDir_Id, m_volumeType, nReqID);

    return true;
}

int CDirTreeThread::GetAllVideoFileList()
{
    QVector<QStringList> queryDataList;

    queryDataList.clear();

    m_tMutex.lock();
    if(m_nWorking == 0) 
    {
        QStringList slist;

        for(int i = 0; i < m_tDirTree.GetVideoFileNum(); i++)
        {
            CDTFileInfo &tFileInfo = m_tDirTree.GetVideoFileInfo(i);
            if(tFileInfo.nIsValid == 0) {
                qDebug("CDirTreeThread::GetAllVideoFileList(). tFileInfo(%d) is invalid", i);
                break;
            }

            CDTDirInfo &tDirInfo = m_tDirTree.GetDirInfo(tFileInfo.nParentDirIdx);
            if(tDirInfo.nIsValid == 0) {
                qDebug("CDirTreeThread::GetAllVideoFileList(). tDirInfo(%d) is invalid", tFileInfo.nParentDirIdx);
                break;
            }

            slist.append(tDirInfo.strName + "/" + tFileInfo.strName);
        }

        if(slist.size() > 0)
        queryDataList.append(slist);
    }
    m_tMutex.unlock();
    
    emit DataFetched(queryDataList, eGetVideoLimitFileListDir_Id);

    return true;    
}

int CDirTreeThread::GetAllPhotoFileList()
{
    QVector<QStringList> queryDataList;

    queryDataList.clear();

    m_tMutex.lock();
    if(m_nWorking == 0) 
    {
        QStringList slist;

        for(int i = 0; i < m_tDirTree.GetPhotoFileNum(); i++)
        {
            CDTFileInfo &tFileInfo = m_tDirTree.GetPhotoFileInfo(i);
            if(tFileInfo.nIsValid == 0) {
                qDebug("CDirTreeThread::GetAllPhotoFileList(). tFileInfo(%d) is invalid", i);
                break;
            }

            CDTDirInfo &tDirInfo = m_tDirTree.GetDirInfo(tFileInfo.nParentDirIdx);
            if(tDirInfo.nIsValid == 0) {
                qDebug("CDirTreeThread::GetAllPhotoFileList(). tDirInfo(%d) is invalid", tFileInfo.nParentDirIdx);
                break;
            }

            slist.append(tDirInfo.strName + "/" + tFileInfo.strName);
        }

        if(slist.size() > 0)
        queryDataList.append(slist);
    }
    m_tMutex.unlock();
    
    emit DataFetched(queryDataList, eGetPhotoLimitFileListDir_Id);

    return true;
}

void CDirTreeThread::GetMediaEntryList(const QString &strSubDirPath, QList<QStringList> *entryList) // added by sungha.choi 2013.09.10
{
    m_tMutex.lock();
    if(m_nWorking == 0)
    {
        qDebug("CDirTreeThread::GetMediaEntryList(%s)", qPrintable(strSubDirPath));

        QString strModifiedPath;
        if(strSubDirPath.startsWith("file://"))
            strModifiedPath = strSubDirPath.mid(7);
        else
            strModifiedPath = strSubDirPath;

        CDTDirInfo tSubDirInfo;
        for(int i = 0; i < m_tDirTree.GetDirNum(); i++)
        {
            CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
            if(tTmpDirInfo.strName.compare(strModifiedPath) == 0) {
                tSubDirInfo = tTmpDirInfo;
                break;
            }
        }

        QStringList media;

        if(tSubDirInfo.nIsValid == 1)
        {
            // Dir list
            if(tSubDirInfo.nSubDirIdx >= 0)
            {
                for(int i = tSubDirInfo.nSubDirIdx; i < (tSubDirInfo.nSubDirIdx + tSubDirInfo.nSubDirNum); i++)
                {
                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
                    if(tTmpDirInfo.pIsValid[eDT_AUDIO] == 1 || tTmpDirInfo.pIsValid[eDT_VIDEO] == 1 || tTmpDirInfo.pIsValid[eDT_PHOTO] == 1)
                    {
                        media.clear();
                        media.append(tTmpDirInfo.strName);
                        media.append("1");
                        entryList->append(media);
                    }
                }
            }

            // File list
            if(tSubDirInfo.pFileIdx[eDT_AUDIO] >= 0)
            {
                for(int i = tSubDirInfo.pFileIdx[eDT_AUDIO]; i < (tSubDirInfo.pFileIdx[eDT_AUDIO] + tSubDirInfo.pFileNum[eDT_AUDIO]); i++)
                {
                    CDTFileInfo &tFileInfo = m_tDirTree.GetAudioFileInfo(i);
                    if(tFileInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetSubAudioFileList(). tFileInfo(%d) is invalid", i);
                        break;
                    }

                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(tFileInfo.nParentDirIdx);
                    if(tTmpDirInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetSubAudioFileList(). tTmpDirInfo(%d) is invalid", tFileInfo.nParentDirIdx);
                        continue;
                    }
                    media.clear();
                    media.append(tFileInfo.strName);
                    media.append("0");
                    entryList->append(media);
                }
            }
            if(tSubDirInfo.pFileIdx[eDT_VIDEO] >= 0)
            {
                for(int i = tSubDirInfo.pFileIdx[eDT_VIDEO]; i < (tSubDirInfo.pFileIdx[eDT_VIDEO] + tSubDirInfo.pFileNum[eDT_VIDEO]); i++)
                {
                    CDTFileInfo &tFileInfo = m_tDirTree.GetVideoFileInfo(i);
                    if(tFileInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetVideoEntryList(). tFileInfo(%d) is invalid", i);
                        break;
                    }

                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(tFileInfo.nParentDirIdx);
                    if(tTmpDirInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetVideoEntryList(). tTmpDirInfo(%d) is invalid", tFileInfo.nParentDirIdx);
                        continue;
                    }
                    media.clear();
                    media.append(tFileInfo.strName);
                    media.append("0");
                    entryList->append(media);
                }
            }
            if(tSubDirInfo.pFileIdx[eDT_PHOTO] >= 0)
            {
                for(int i = tSubDirInfo.pFileIdx[eDT_PHOTO]; i < (tSubDirInfo.pFileIdx[eDT_PHOTO] + tSubDirInfo.pFileNum[eDT_PHOTO]); i++)
                {
                    CDTFileInfo &tFileInfo = m_tDirTree.GetPhotoFileInfo(i);
                    if(tFileInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetPictureEntryList(). tFileInfo(%d) is invalid", i);
                        break;
                    }

                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(tFileInfo.nParentDirIdx);
                    if(tTmpDirInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetPictureEntryList(). tTmpDirInfo(%d) is invalid", tFileInfo.nParentDirIdx);
                        continue;
                    }
                    media.clear();
                    media.append(tFileInfo.strName);
                    media.append("0");
                    entryList->append(media);
                }
            }
        }
    }
    m_tMutex.unlock();
}

void CDirTreeThread::GetAudioEntryList(const QString &strSubDirPath, QList<QStringList> *entryList) // added by sungha.choi 2013.09.10
{
    m_tMutex.lock();
    if(m_nWorking == 0)
    {
        qDebug("CDirTreeThread::GetAudioEntryList(%s)", qPrintable(strSubDirPath));

        QString strModifiedPath;
        if(strSubDirPath.startsWith("file://"))
            strModifiedPath = strSubDirPath.mid(7);
        else
            strModifiedPath = strSubDirPath;

        CDTDirInfo tSubDirInfo;
        for(int i = 0; i < m_tDirTree.GetDirNum(); i++)
        {
            CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
            if(tTmpDirInfo.strName.compare(strModifiedPath) == 0) {
                tSubDirInfo = tTmpDirInfo;
                break;
            }
        }

        QStringList audio;

        if(tSubDirInfo.nIsValid == 1)
        {
            // Dir list
            if(tSubDirInfo.nSubDirIdx >= 0)
            {
                for(int i = tSubDirInfo.nSubDirIdx; i < (tSubDirInfo.nSubDirIdx + tSubDirInfo.nSubDirNum); i++)
                {
                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
                    if(tTmpDirInfo.pIsValid[eDT_AUDIO] == 1)
                    {
                        audio.clear();
                        audio.append(tTmpDirInfo.strName);
                        audio.append("1");
                        entryList->append(audio);
                    }
                }
            }

            // File list
            if(tSubDirInfo.pFileIdx[eDT_AUDIO] >= 0)
            {
                for(int i = tSubDirInfo.pFileIdx[eDT_AUDIO]; i < (tSubDirInfo.pFileIdx[eDT_AUDIO] + tSubDirInfo.pFileNum[eDT_AUDIO]); i++)
                {
                    CDTFileInfo &tFileInfo = m_tDirTree.GetAudioFileInfo(i);
                    if(tFileInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetSubAudioFileList(). tFileInfo(%d) is invalid", i);
                        break;
                    }

                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(tFileInfo.nParentDirIdx);
                    if(tTmpDirInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetSubAudioFileList(). tTmpDirInfo(%d) is invalid", tFileInfo.nParentDirIdx);
                        continue;
                    }
                    audio.clear();
                    audio.append(tFileInfo.strName);
                    audio.append("0");
                    entryList->append(audio);
                }
            }
        }
    }
    m_tMutex.unlock();
}

void CDirTreeThread::GetVideoEntryList(const QString &strSubDirPath, QList<QStringList> *entryList) // added by sungha.choi 2013.09.10
{
    m_tMutex.lock();
    if(m_nWorking == 0)
    {
        qDebug("CDirTreeThread::GetVideoEntryList(%s)", qPrintable(strSubDirPath));

        QString strModifiedPath;
        if(strSubDirPath.startsWith("file://"))
            strModifiedPath = strSubDirPath.mid(7);
        else
            strModifiedPath = strSubDirPath;

        CDTDirInfo tSubDirInfo;
        for(int i = 0; i < m_tDirTree.GetDirNum(); i++)
        {
            CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
            if(tTmpDirInfo.strName.compare(strModifiedPath) == 0) {
                tSubDirInfo = tTmpDirInfo;
                break;
            }
        }

        QStringList video;

        if(tSubDirInfo.nIsValid == 1)
        {
            // Dir list
            if(tSubDirInfo.nSubDirIdx >= 0)
            {
                for(int i = tSubDirInfo.nSubDirIdx; i < (tSubDirInfo.nSubDirIdx + tSubDirInfo.nSubDirNum); i++)
                {
                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
                    if(tTmpDirInfo.pIsValid[eDT_VIDEO] == 1)
                    {
                        video.clear();
                        video.append(tTmpDirInfo.strName);
                        video.append("1");
                        entryList->append(video);
                    }
                }
            }

            // File list
            if(tSubDirInfo.pFileIdx[eDT_VIDEO] >= 0)
            {
                for(int i = tSubDirInfo.pFileIdx[eDT_VIDEO]; i < (tSubDirInfo.pFileIdx[eDT_VIDEO] + tSubDirInfo.pFileNum[eDT_VIDEO]); i++)
                {
                    CDTFileInfo &tFileInfo = m_tDirTree.GetVideoFileInfo(i);
                    if(tFileInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetVideoEntryList(). tFileInfo(%d) is invalid", i);
                        break;
                    }

                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(tFileInfo.nParentDirIdx);
                    if(tTmpDirInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetVideoEntryList(). tTmpDirInfo(%d) is invalid", tFileInfo.nParentDirIdx);
                        continue;
                    }

                    video.clear();
                    video.append(tFileInfo.strName);
                    video.append("0");
                    entryList->append(video);
                }
            }
        }
    }
    m_tMutex.unlock();
}

void CDirTreeThread::GetPictureEntryList(const QString &strSubDirPath, QList<QStringList> *entryList) // added by sungha.choi 2013.09.10
{
    m_tMutex.lock();
    if(m_nWorking == 0)
    {
        qDebug("CDirTreeThread::GetPictureEntryList(%s)", qPrintable(strSubDirPath));

        QString strModifiedPath;
        if(strSubDirPath.startsWith("file://"))
            strModifiedPath = strSubDirPath.mid(7);
        else
            strModifiedPath = strSubDirPath;

        CDTDirInfo tSubDirInfo;
        for(int i = 0; i < m_tDirTree.GetDirNum(); i++)
        {
            CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
            if(tTmpDirInfo.strName.compare(strModifiedPath) == 0) {
                tSubDirInfo = tTmpDirInfo;
                break;
            }
        }

        QStringList picture;

        if(tSubDirInfo.nIsValid == 1)
        {
            // Dir list
            if(tSubDirInfo.nSubDirIdx >= 0)
            {
                for(int i = tSubDirInfo.nSubDirIdx; i < (tSubDirInfo.nSubDirIdx + tSubDirInfo.nSubDirNum); i++)
                {
                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
                    if(tTmpDirInfo.pIsValid[eDT_PHOTO] == 1)
                    {
                        picture.clear();
                        picture.append(tTmpDirInfo.strName);
                        picture.append("1");
                        entryList->append(picture);
                    }
                }
            }

            // File list
            if(tSubDirInfo.pFileIdx[eDT_PHOTO] >= 0)
            {
                for(int i = tSubDirInfo.pFileIdx[eDT_PHOTO]; i < (tSubDirInfo.pFileIdx[eDT_PHOTO] + tSubDirInfo.pFileNum[eDT_PHOTO]); i++)
                {
                    CDTFileInfo &tFileInfo = m_tDirTree.GetPhotoFileInfo(i);
                    if(tFileInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetPictureEntryList(). tFileInfo(%d) is invalid", i);
                        break;
                    }

                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(tFileInfo.nParentDirIdx);
                    if(tTmpDirInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetPictureEntryList(). tTmpDirInfo(%d) is invalid", tFileInfo.nParentDirIdx);
                        continue;
                    }
                    picture.clear();
                    picture.append(tFileInfo.strName);
                    picture.append("0");
                    entryList->append(picture);
                }
            }
        }
    }
    m_tMutex.unlock();
}

int CDirTreeThread::GetSubAudioFileList(const QString &strSubDirPath, uint nReqID)
{
    QVector<QStringList> queryDataList;

    queryDataList.clear();

    m_tMutex.lock();
    if(m_nWorking == 0) 
    {
        qDebug("CDirTreeThread::GetSubAudioFileList(%s)", qPrintable(strSubDirPath));
        
        QString strModifiedPath;
        if(strSubDirPath.startsWith("file://"))
            strModifiedPath = strSubDirPath.mid(7);
        else
            strModifiedPath = strSubDirPath;

        // QStringList itemNameList;
        QStringList itemPathList;
        QStringList folderFlag;
        QStringList emptyFolderFlag;
        
        CDTDirInfo tSubDirInfo;
        for(int i = 0; i < m_tDirTree.GetDirNum(); i++)
        {
            CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
            if(tTmpDirInfo.strName.compare(strModifiedPath) == 0) {
                tSubDirInfo = tTmpDirInfo;
                break;
            }
        }

        if(tSubDirInfo.nIsValid == 1) 
        {
            // Dir list
            if(tSubDirInfo.nSubDirIdx >= 0) 
            {
                for(int i = tSubDirInfo.nSubDirIdx; i < (tSubDirInfo.nSubDirIdx + tSubDirInfo.nSubDirNum); i++) 
                {
                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);

                    // int nLastIdx = tTmpDirInfo.strName.lastIndexOf("/");
                    // itemNameList    << nLastIdx >= 0 ? tTmpDirInfo.strName.right(tTmpDirInfo.strName.size() - nLastIdx) : QString();
                    itemPathList    << tTmpDirInfo.strName;
                    folderFlag      << "true";
                    emptyFolderFlag << (tTmpDirInfo.pIsValid[eDT_AUDIO] == 1 ? "false" : "true");
                }
            }
            
            // File list
            if(tSubDirInfo.pFileIdx[eDT_AUDIO] >= 0) 
            {
                for(int i = tSubDirInfo.pFileIdx[eDT_AUDIO]; i < (tSubDirInfo.pFileIdx[eDT_AUDIO] + tSubDirInfo.pFileNum[eDT_AUDIO]); i++) 
                {
                    CDTFileInfo &tFileInfo = m_tDirTree.GetAudioFileInfo(i);
                    if(tFileInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetSubAudioFileList(). tFileInfo(%d) is invalid", i);
                        break;
                    }

                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(tFileInfo.nParentDirIdx);
                    if(tTmpDirInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetSubAudioFileList(). tTmpDirInfo(%d) is invalid", tFileInfo.nParentDirIdx);
                        continue;
                    }


                    // itemNameList    << tFileInfo.strName;
                    itemPathList    << tTmpDirInfo.strName + "/" + tFileInfo.strName;
                    folderFlag      << "false";
                    emptyFolderFlag << "false";
                }            
            }

            QStringList resultData;
            for(int i=0; i<itemPathList.count(); i++)
            {
                resultData.clear();
                resultData.append(itemPathList[i]);
                resultData.append(folderFlag[i]);
                resultData.append(emptyFolderFlag[i]);

                queryDataList.append(resultData);
            }
        }
    }
    m_tMutex.unlock();

    emit DataDirectFetched(queryDataList, eGetFolderTabList_Id, nReqID);

    return true;
}

int CDirTreeThread::GetSubVideoFileList(const QString &strSubDirPath)
{
    QVector<QStringList> queryDataList;

    queryDataList.clear();

    m_tMutex.lock();
    if(m_nWorking == 0) 
    {
        qDebug("CDirTreeThread::GetSubVideoFileList(%s)", qPrintable(strSubDirPath));
        
        QString strModifiedPath;
        if(strSubDirPath.startsWith("file://"))
            strModifiedPath = strSubDirPath.mid(7);
        else
            strModifiedPath = strSubDirPath;

        // QStringList itemNameList;
        QStringList itemPathList;
        QStringList folderFlag;
        QStringList emptyFolderFlag;
        
        CDTDirInfo tSubDirInfo;
        for(int i = 0; i < m_tDirTree.GetDirNum(); i++)
        {
            CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
            if(tTmpDirInfo.strName.compare(strModifiedPath) == 0) {
                tSubDirInfo = tTmpDirInfo;
                break;
            }
        }

        if(tSubDirInfo.nIsValid == 1) 
        {
            // Dir list
            if(tSubDirInfo.nSubDirIdx >= 0) 
            {
                for(int i = tSubDirInfo.nSubDirIdx; i < (tSubDirInfo.nSubDirIdx + tSubDirInfo.nSubDirNum); i++) 
                {
                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);

                    // int nLastIdx = tTmpDirInfo.strName.lastIndexOf("/");
                    // itemNameList    << nLastIdx >= 0 ? tTmpDirInfo.strName.right(tTmpDirInfo.strName.size() - nLastIdx) : QString();
                    itemPathList    << tTmpDirInfo.strName;
                    folderFlag      << "true";
                    emptyFolderFlag << (tTmpDirInfo.pIsValid[eDT_VIDEO] == 1 ? "false" : "true");
                }
            }
            
            // File list
            if(tSubDirInfo.pFileIdx[eDT_VIDEO] >= 0) 
            {
                for(int i = tSubDirInfo.pFileIdx[eDT_VIDEO]; i < (tSubDirInfo.pFileIdx[eDT_VIDEO] + tSubDirInfo.pFileNum[eDT_VIDEO]); i++) 
                {
                    CDTFileInfo &tFileInfo = m_tDirTree.GetVideoFileInfo(i);
                    if(tFileInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetSubVideoFileList(). tFileInfo(%d) is invalid", i);
                        break;
                    }

                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(tFileInfo.nParentDirIdx);
                    if(tTmpDirInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetSubVideoFileList(). tTmpDirInfo(%d) is invalid", tFileInfo.nParentDirIdx);
                        continue;
                    }


                    // itemNameList    << tFileInfo.strName;
                    itemPathList    << tTmpDirInfo.strName + "/" + tFileInfo.strName;
                    folderFlag      << "false";
                    emptyFolderFlag << "false";
                }            
            }

            QStringList resultData;
            for(int i=0; i<itemPathList.count(); i++)
            {
                resultData.clear();
                resultData.append(itemPathList[i]);
                resultData.append(folderFlag[i]);
                resultData.append(emptyFolderFlag[i]);

                queryDataList.append(resultData);
            }
        }
    }
    m_tMutex.unlock();

    emit DataFetched(queryDataList, eGetVideoFolderTabList_Id);

    return true;
}

int CDirTreeThread::GetSubPhotoFileList(const QString &strSubDirPath)
{
    QVector<QStringList> queryDataList;

    queryDataList.clear();

    m_tMutex.lock();
    if(m_nWorking == 0) 
    {
        QString strModifiedPath;
        if(strSubDirPath.startsWith("file://"))
            strModifiedPath = strSubDirPath.mid(7);
        else
            strModifiedPath = strSubDirPath;

        qDebug("CDirTreeThread::GetSubPhotoFileList(%s)", qPrintable(strModifiedPath));

        // QStringList itemNameList;
        QStringList itemPathList;
        QStringList folderFlag;
        QStringList emptyFolderFlag;
        
        CDTDirInfo tSubDirInfo;
        for(int i = 0; i < m_tDirTree.GetDirNum(); i++)
        {
            CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
            if(tTmpDirInfo.strName.compare(strModifiedPath) == 0) {
                tSubDirInfo = tTmpDirInfo;
                break;
            }
        }

        if(tSubDirInfo.nIsValid == 1) 
        {
            qDebug("CDirTreeThread::GetSubPhotoFileList() D:%d-%d, F:%d-%d", tSubDirInfo.nSubDirIdx, tSubDirInfo.nSubDirNum, tSubDirInfo.pFileIdx[eDT_PHOTO], tSubDirInfo.pFileNum[eDT_PHOTO]);
            
            // Dir list
            if(tSubDirInfo.nSubDirIdx >= 0) 
            {
                for(int i = tSubDirInfo.nSubDirIdx; i < (tSubDirInfo.nSubDirIdx + tSubDirInfo.nSubDirNum); i++) 
                {
                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);

                    // int nLastIdx = tTmpDirInfo.strName.lastIndexOf("/");
                    // itemNameList    << nLastIdx >= 0 ? tTmpDirInfo.strName.right(tTmpDirInfo.strName.size() - nLastIdx) : QString();
                    itemPathList    << tTmpDirInfo.strName;
                    folderFlag      << "true";
                    emptyFolderFlag << (tTmpDirInfo.pIsValid[eDT_PHOTO] == 1 ? "false" : "true");
                }
            }
            
            // File list
            if(tSubDirInfo.pFileIdx[eDT_PHOTO] >= 0) 
            {
                for(int i = tSubDirInfo.pFileIdx[eDT_PHOTO]; i < (tSubDirInfo.pFileIdx[eDT_PHOTO] + tSubDirInfo.pFileNum[eDT_PHOTO]); i++) 
                {
                    CDTFileInfo &tFileInfo = m_tDirTree.GetPhotoFileInfo(i);
                    if(tFileInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetSubPhotoFileList(). tFileInfo(%d) is invalid", i);
                        break;
                    }

                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(tFileInfo.nParentDirIdx);
                    if(tTmpDirInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetSubPhotoFileList(). tTmpDirInfo(%d) is invalid", tFileInfo.nParentDirIdx);
                        continue;
                    }


                    // itemNameList    << tFileInfo.strName;
                    itemPathList    << tTmpDirInfo.strName + "/" + tFileInfo.strName;
                    folderFlag      << "false";
                    emptyFolderFlag << "false";
                }            
            }

            QStringList resultData;
            for(int i=0; i<itemPathList.count(); i++)
            {
                resultData.clear();
                resultData.append(itemPathList[i]);
                resultData.append(folderFlag[i]);
                resultData.append(emptyFolderFlag[i]);

                queryDataList.append(resultData);
            }
        }
        else
        {
            qDebug("CDirTreeThread::GetSubPhotoFileList() tSubDirInfo.nIsValid == 0");
        }
    }
    m_tMutex.unlock();

    emit DataFetched(queryDataList, eGetPhotoFolderTabList_Id);

    return true;
}

bool CDirTreeThread::GetVideoFileListWithSubFolder(const QString &strSubDirPath)
{
    QVector<QStringList> queryDataList;
    queryDataList.clear();
    
    m_tMutex.lock();
    if(m_nWorking == 0) 
    {
        GetVideoFileListWithSubFolder_Impl(strSubDirPath, queryDataList);
    }
    m_tMutex.unlock();

    qDebug("CDirTreeThread::GetVideoFileListWithSubFolder(%s) %d", qPrintable(strSubDirPath), queryDataList.size());
    
    emit DataFetched(queryDataList, eGetVideosBySubFolderQuery_Id);

    return true;   
}

bool CDirTreeThread::GetPhotoFileListWithSubFolder(const QString &strSubDirPath)
{
    QVector<QStringList> queryDataList;
    queryDataList.clear();

    m_tMutex.lock();
    if(m_nWorking == 0) 
    {
        GetPhotoFileListWithSubFolder_Impl(strSubDirPath, queryDataList);
    }
    m_tMutex.unlock();

    qDebug("CDirTreeThread::GetPhotoFileListWithSubFolder(%s) %d", qPrintable(strSubDirPath), queryDataList.size());
    
    emit DataFetched(queryDataList, eGetImagesBySubFolderQuery_Id);

    return true;
}

// { modified by kihyung.lee 2013.07.16 for ITS 0178868
bool CDirTreeThread::GetVideoFileListByFolder(const QString &strSubDirPath)
{
    if(m_bDirTreeScanMode == false && CheckInternalRead() == true) {
        qDebug("CDirTreeThread::GetVideoFileListByFolder()--------NOT_READ--->START_READ");        
        //return false;   // deleted by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    }
    
    QVector<QStringList> queryDataList;

    queryDataList.clear();

    m_tMutex.lock();
    if(m_nWorking == 0) 
    {
        // Find folder
        CDTDirInfo tSubDirInfo;
        for(int i = 0; i < m_tDirTree.GetDirNum(); i++)
        {
            CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
            if(tTmpDirInfo.strName.compare(strSubDirPath) == 0) {
                tSubDirInfo = tTmpDirInfo;
                break;
            }
        }

        // Find files
        if(tSubDirInfo.nIsValid == 1)
        {
            QStringList itemNameList;
            QStringList itemPathList;
            QStringList folderFlag;

            // Dir list
            if(tSubDirInfo.nSubDirIdx >= 0) 
            {
                for(int i = tSubDirInfo.nSubDirIdx; i < (tSubDirInfo.nSubDirIdx + tSubDirInfo.nSubDirNum); i++) 
                {
                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
                    if(tTmpDirInfo.pIsValid[eDT_VIDEO] == 1) {
                        int nLastIdx = tTmpDirInfo.strName.lastIndexOf("/");
                        itemNameList    << (nLastIdx >= 0 ? tTmpDirInfo.strName.right(tTmpDirInfo.strName.size() - (nLastIdx + 1)) : QString());
                        itemPathList    << tTmpDirInfo.strName;
                        folderFlag      << "true";
                    }
                }
            }

            // File list
            if(tSubDirInfo.pFileIdx[eDT_VIDEO] >= 0) 
            {
                for(int i = tSubDirInfo.pFileIdx[eDT_VIDEO]; i < (tSubDirInfo.pFileIdx[eDT_VIDEO] + tSubDirInfo.pFileNum[eDT_VIDEO]); i++) 
                {
                    CDTFileInfo &tFileInfo = m_tDirTree.GetVideoFileInfo(i);
                    if(tFileInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetVideoFileListByFolder(). tFileInfo(%d) is invalid", i);
                        break;
                    }

                    itemNameList    << tFileInfo.strName;
                    itemPathList    << "file://" + tSubDirInfo.strName + "/" + tFileInfo.strName;
                    folderFlag      << "false";
                }
            }

            if(itemPathList.count() == itemNameList.count())
            {
                queryDataList.append(itemNameList);
                queryDataList.append(itemPathList);
                queryDataList.append(folderFlag);
            }
            else
            {
                { QString a; LOG_CRITICAL << a.sprintf("itemPathList.count(%d) != itemNameList.count(%d)", itemPathList.count(), itemNameList.count()) << LOG_ENDL; }
            }
        }
    }
    m_tMutex.unlock();
    
    qDebug("CDirTreeThread::GetVideoFileListByFolder(%s) %d", qPrintable(strSubDirPath), queryDataList.size());
    emit DataFetched(queryDataList, eGetVideosByFolderQuery_Id);

    return true;   
}
// } modified by kihyung.lee 2013.07.16 for ITS 0178868

// { modified by kihyung.lee 2013.07.16 for ITS 0178868
bool CDirTreeThread::GetPhotoFileListByFolder(const QString &strSubDirPath)
{
    if(m_bDirTreeScanMode == false && CheckInternalRead() == true) {
        qDebug("CDirTreeThread::GetPhotoFileListByFolder()--------NOT_READ--->START_READ");
        //return false;   // deleted by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    }
    
    QVector<QStringList> queryDataList;

    queryDataList.clear();
    
    if(m_nWorking == 0) 
    {
        // Find folder
        CDTDirInfo tSubDirInfo;
        for(int i = 0; i < m_tDirTree.GetDirNum(); i++)
        {
            CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
            if(tTmpDirInfo.strName.compare(strSubDirPath) == 0) {
                tSubDirInfo = tTmpDirInfo;
                break;
            }
        }

        // Find files
        if(tSubDirInfo.nIsValid == 1)
        {
            QStringList itemNameList;
            QStringList itemPathList;
            QStringList folderFlag;

            // Dir list
            if(tSubDirInfo.nSubDirIdx >= 0) 
            {
                for(int i = tSubDirInfo.nSubDirIdx; i < (tSubDirInfo.nSubDirIdx + tSubDirInfo.nSubDirNum); i++) 
                {
                    CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
                    if(tTmpDirInfo.pIsValid[eDT_PHOTO] == 1) {
                        int nLastIdx = tTmpDirInfo.strName.lastIndexOf("/");
                        itemNameList    << (nLastIdx >= 0 ? tTmpDirInfo.strName.right(tTmpDirInfo.strName.size() - (nLastIdx + 1)) : QString());
                        itemPathList    << tTmpDirInfo.strName;
                        folderFlag      << "true";
                    }
                }
            }

            // File list
            if(tSubDirInfo.pFileIdx[eDT_PHOTO] >= 0) 
            {
                for(int i = tSubDirInfo.pFileIdx[eDT_PHOTO]; i < (tSubDirInfo.pFileIdx[eDT_PHOTO] + tSubDirInfo.pFileNum[eDT_PHOTO]); i++) 
                {
                    CDTFileInfo &tFileInfo = m_tDirTree.GetPhotoFileInfo(i);
                    if(tFileInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetPhotoFileListByFolder(). tFileInfo(%d) is invalid", i);
                        break;
                    }

                    itemNameList << tFileInfo.strName;
                    itemPathList << "file://" + tSubDirInfo.strName + "/" + tFileInfo.strName;
                    folderFlag   << "false";
                }
            }

            if(itemPathList.count() == itemNameList.count())
            {
                queryDataList.append(itemNameList);
                queryDataList.append(itemPathList);
                queryDataList.append(folderFlag);
            }
            else
            {
                { QString a; LOG_CRITICAL << a.sprintf("itemPathList.count(%d) != itemNameList.count(%d)", itemPathList.count(), itemNameList.count()) << LOG_ENDL; }
            }
        }
    }

    qDebug("CDirTreeThread::GetPhotoFileListByFolder(%s) %d", qPrintable(strSubDirPath), queryDataList.size());
    emit DataFetched(queryDataList, eGetImagesByFolderQuery_Id);

    return true;
}
// } modified by kihyung.lee 2013.07.16 for ITS 0178868


// { added by kihyung 2013.07.23 for ITS 0178868
bool CDirTreeThread::GetFirstImagePathByGivenFolder(const QString &strSubDirPath)
{
    if(m_bDirTreeScanMode == false && CheckInternalRead() == true) {
        qDebug("CDirTreeThread::GetFirstImagePathByGivenFolder()--------NOT_READ--->START_READ");
        //return false;   // deleted by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    }
    
    QVector<QStringList> queryDataList;

    queryDataList.clear();
    
    if(m_nWorking == 0) 
    {
        // Find folder
        CDTDirInfo tSubDirInfo;
        for(int i = 0; i < m_tDirTree.GetDirNum(); i++)
        {
            CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
            if(tTmpDirInfo.strName.compare(strSubDirPath) == 0) {
                tSubDirInfo = tTmpDirInfo;
                break;
            }
        }

        // Find files
        if(tSubDirInfo.nIsValid == 1)
        {
            QStringList itemPathList;

            // File list
            if(tSubDirInfo.pFileIdx[eDT_PHOTO] >= 0) 
            {
                for(int i = tSubDirInfo.pFileIdx[eDT_PHOTO]; i < (tSubDirInfo.pFileIdx[eDT_PHOTO] + tSubDirInfo.pFileNum[eDT_PHOTO]); i++) 
                {
                    CDTFileInfo &tFileInfo = m_tDirTree.GetPhotoFileInfo(i);
                    if(tFileInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetFirstImagePathByGivenFolder(). tFileInfo(%d) is invalid", i);
                        break;
                    }

                    itemPathList << "file://" + tSubDirInfo.strName + "/" + tFileInfo.strName;
                    break;
                }
            }

            if(itemPathList.count() > 0)
            {
                queryDataList.append(itemPathList);
            }
        }
    }

    qDebug("CDirTreeThread::GetFirstImagePathByGivenFolder(%s) %d", qPrintable(strSubDirPath), queryDataList.size());
    emit DataFetched(queryDataList, eGetFirstImagePathByGivenFolder);

    return true;
}

bool CDirTreeThread::GetImageCountByGivenFolder(QString &strSubDirPath)
{
    if(m_bDirTreeScanMode == false && CheckInternalRead() == true) {
        qDebug("CDirTreeThread::GetImageCountByGivenFolder()--------NOT_READ--->START_READ");
        //return false;   // deleted by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    }
    
    QVector<QStringList> queryDataList;

    queryDataList.clear();
    
    if(m_nWorking == 0) 
    {
        // Find folder
        CDTDirInfo tSubDirInfo;
        for(int i = 0; i < m_tDirTree.GetDirNum(); i++)
        {
            CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
            if(tTmpDirInfo.strName.compare(strSubDirPath) == 0) {
                tSubDirInfo = tTmpDirInfo;
                break;
            }
        }

        // Find file count
        if(tSubDirInfo.nIsValid == 1)
        {
            QStringList itemList;

            if(tSubDirInfo.pFileNum[eDT_PHOTO] >= 0) 
            {
                itemList << QString::number(tSubDirInfo.pFileNum[eDT_PHOTO]);
            }

            if(itemList.count() > 0)
            {
                queryDataList.append(itemList);
            }
        }
    }

    qDebug("CDirTreeThread::GetImageCountByGivenFolder(%s) %d", qPrintable(strSubDirPath), queryDataList.size());
    emit DataFetched(queryDataList, eGetImageCountByGivenFolder);

    return true;
}

bool CDirTreeThread::GetFirstImageAndCountByGivenFolder(QString &strSubDirPath)
{
    if(m_bDirTreeScanMode == false && CheckInternalRead() == true) {
        qDebug("CDirTreeThread::GetFirstImageAndCountByGivenFolder()--------NOT_READ--->START_READ");
        //return false;   // deleted by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    }
    
    QVector<QStringList> queryDataList;

    queryDataList.clear();
    
    if(m_nWorking == 0) 
    {
        // Find folder
        CDTDirInfo tSubDirInfo;
        for(int i = 0; i < m_tDirTree.GetDirNum(); i++)
        {
            CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
            if(tTmpDirInfo.strName.compare(strSubDirPath) == 0) {
                tSubDirInfo = tTmpDirInfo;
                break;
            }
        }

        // Find files
        if(tSubDirInfo.nIsValid == 1)
        {
            QStringList itemList;

            // File list
            if(tSubDirInfo.pFileIdx[eDT_PHOTO] >= 0) 
            {
                for(int i = tSubDirInfo.pFileIdx[eDT_PHOTO]; i < (tSubDirInfo.pFileIdx[eDT_PHOTO] + tSubDirInfo.pFileNum[eDT_PHOTO]); i++) 
                {
                    CDTFileInfo &tFileInfo = m_tDirTree.GetPhotoFileInfo(i);
                    if(tFileInfo.nIsValid == 0) {
                        qDebug("CDirTreeThread::GetFirstImageAndCountByGivenFolder(). tFileInfo(%d) is invalid", i);
                        break;
                    }

                    itemList << "file://" + tSubDirInfo.strName + "/" + tFileInfo.strName;
                    itemList << QString::number(tSubDirInfo.pFileNum[eDT_PHOTO]);
                    break;
                }
            }

            if(itemList.count() > 0)
            {
                queryDataList.append(itemList);
            }
        }
    }

    qDebug("CDirTreeThread::GetFirstImageAndCountByGivenFolder(%s) %d", qPrintable(strSubDirPath), queryDataList.size());
    emit DataFetched(queryDataList, eGetFirstImageAndCountByGivenFolder);

    return true;
}
// } added by kihyung 2013.07.23 for ITS 0178868 

QString CDirTreeThread::GetMountPath()
{
    if(m_strMountPath.isEmpty() == true)
    {
        if(m_volumeType == eJukeBox) {
            m_strMountPath = "/usr_data/media";
        }
        else {
            QFileInfo tFileInfo("/rw_data");
            if(tFileInfo.exists()) {
                if(m_volumeType == eUSB_FRONT) 
                    m_strMountPath = "/rw_data/media/front_usb";
                else
                    m_strMountPath = "/rw_data/media/rear_usb";
            }
            else {
                if(m_volumeType == eUSB_FRONT) 
                    m_strMountPath = "/media/front_usb";
                else
                    m_strMountPath = "/media/rear_usb";
            }
        }
    }

    return m_strMountPath;
}

void CDirTreeThread::ReadDirInSelfThread()   // added by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
{
    bool bRet = true;

    qDebug("CDirTreeThread::run() mode: %d", m_nTreeMode);

    m_bScanReadComplete = false;
    bRet = StartReadDir();

    if(bRet)
        m_bScanReadComplete = true;

    m_nWorking = 0;

    m_tStopMutex.lock();
    if(m_bStop == false)
    {
        emit ReadCompleted(m_volumeType, true);
    }
    else {
        qDebug("CDirTreeThread::run() m_bStop is true");
        Init();  // modified by kihyung 2013.07.22 for ITS 0180971
    }
    m_tStopMutex.unlock();
}

void CDirTreeThread::run()
{
    bool bRet;  // added by sungha.choi 2013.08.25
    forever
    {
        StartWait();

        qDebug("CDirTreeThread::run() mode: %d", m_nTreeMode);

        m_bScanReadComplete = false;
        bRet = true;    // added by sungha.choi 2013.08.25

        if(m_nTreeMode == DT_FS_SCAN_MODE) {
            QFileInfo tFileInfo(GetMountPath());
            if(m_volumeType == eJukeBox && tFileInfo.exists() && m_bRescanMode == false) {
                if(StartReadDir() == false) {
                    LOG_CRITICAL << "CDirTreeThread::run() StartReadDir() return false ---> StartScanDir()" << LOG_ENDL;
                    StartScanDir();
                }
                else {
                    NotifyMediaFileFound(m_tDirTree, true);
                }
            }
            else {
                StartScanDir();
            }
        }
        else {
            bRet = StartReadDir();    // added by sungha.choi 2013.08.25
        }

        if(bRet)    // added by sungha.choi 2013.08.25
            m_bScanReadComplete = true;        

        EndWait();

        m_tStopMutex.lock();
        if(m_bStop == false) 
        {
        	// { modified by kihyung 2013.07.05 for ISV 84474
            if(m_nTreeMode == DT_FS_READ_MODE)
            {
                emit ReadCompleted(m_volumeType, true);
            }
            else 
            {
                QFileInfo tFileInfo(m_strRootPath);
                if(tFileInfo.exists() == true) {
                    qDebug("CDirTreeThread::run() m_strRootPath(%s) exist", qPrintable(m_strRootPath));
                    emit ScanCompleted(m_volumeType, m_bRescanMode, true);
                }
                else {
                    qDebug("CDirTreeThread::run() m_strRootPath(%s) not exist", qPrintable(m_strRootPath));
                    Init();  // modified by kihyung 2013.07.22 for ITS 0180971
                    emit ScanCompleted(m_volumeType, m_bRescanMode, false);
                }
            }
            // } modified by kihyung 2013.07.05 for ISV 84474
        }
        else {
            qDebug("CDirTreeThread::run() m_bStop is true");
            Init();  // modified by kihyung 2013.07.22 for ITS 0180971
        }
        m_tStopMutex.unlock();
    }
}

 // { added by kihyung 2013.07.22 for ITS 0180971
void CDirTreeThread::Init()
{
    m_tDirTree.Init();
    
    if(m_volumeType == eJukeBox) {
        QFile::remove(gScanResult_JukeBox_path);
    }
    else if(m_volumeType == eUSB_FRONT) {
        QFile::remove(gScanResult_USB1_path);
    }
    else if(m_volumeType == eUSB_REAR) {
        QFile::remove(gScanResult_USB2_path);
    }
}
// } added by kihyung 2013.07.22 for ITS 0180971

void CDirTreeThread::NotifyMediaFileFound(CDirTree &tDirTree, bool bEnd)
{
    if(m_bDirTreeScanMode == false) return;

    int bAudioFound = tDirTree.GetAudioFileNum() > 0 ? true : false;
    int bVideoFound = tDirTree.GetVideoFileNum() > 0 ? true : false;
    int bPhotoFound = tDirTree.GetPhotoFileNum() > 0 ? true : false;
    
    // { modified by kihyung 2013.07.04 for ISV 84474
    /*
    if(m_bAudioFound != bAudioFound && m_bRescanMode == false) {
        m_bAudioFound = bAudioFound;
        m_bVideoFound = bVideoFound;
        m_bPhotoFound = bPhotoFound;
        qDebug("CDirTreeThread::NotifyMediaFileFound()1 %d,%d,%d,%d,%d", m_volumeType, m_bRescanMode, m_bAudioFound, m_bVideoFound, m_bPhotoFound);
        emit MediaFileFound(m_volumeType, m_bRescanMode, m_bAudioFound, m_bVideoFound, m_bPhotoFound);
    }
    else if(bEnd == true && (m_bAudioFound != bAudioFound || m_bVideoFound != bVideoFound || m_bPhotoFound != bPhotoFound)) {
        m_bAudioFound = bAudioFound;
        m_bVideoFound = bVideoFound;
        m_bPhotoFound = bPhotoFound;
        qDebug("CDirTreeThread::NotifyMediaFileFound()2 %d,%d,%d,%d,%d", m_volumeType, m_bRescanMode, m_bAudioFound, m_bVideoFound, m_bPhotoFound);
        emit MediaFileFound(m_volumeType, m_bRescanMode, m_bAudioFound, m_bVideoFound, m_bPhotoFound);
    }
    else if(bEnd == true && (bAudioFound == false && bVideoFound == false && bPhotoFound == false)) {
        m_bAudioFound = bAudioFound;
        m_bVideoFound = bVideoFound;
        m_bPhotoFound = bPhotoFound;
        qDebug("CDirTreeThread::NotifyMediaFileFound()3 %d,%d,%d,%d,%d", m_volumeType, m_bRescanMode, m_bAudioFound, m_bVideoFound, m_bPhotoFound);
        emit MediaFileFound(m_volumeType, m_bRescanMode, m_bAudioFound, m_bVideoFound, m_bPhotoFound);
    }
    */
    if(m_bAllMediaFound == false && bAudioFound == true && bVideoFound == true && bPhotoFound == true)
    {
        m_bAllMediaFound = true;
        qDebug("CDirTreeThread::NotifyMediaFileFound()1 %d,%d,%d,%d,%d", m_volumeType, m_bRescanMode, bAudioFound, bVideoFound, bPhotoFound);
        emit MediaFileFound(m_volumeType, m_bRescanMode, bAudioFound, bVideoFound, bPhotoFound);
    }
    else if(bEnd == true && m_bAllMediaFound == false)
    {
        m_bAllMediaFound = true;
        qDebug("CDirTreeThread::NotifyMediaFileFound()2 %d,%d,%d,%d,%d", m_volumeType, m_bRescanMode, bAudioFound, bVideoFound, bPhotoFound);
        emit MediaFileFound(m_volumeType, m_bRescanMode, bAudioFound, bVideoFound, bPhotoFound);
    }
    // } modified by kihyung 2013.07.04 for ISV 84474
}
    
void CDirTreeThread::GetVideoFileListWithSubFolder_Impl(const QString &strSubDirPath, QVector<QStringList> &queryDataList)
{
    // Find folder
    CDTDirInfo tSubDirInfo;
    for(int i = 0; i < m_tDirTree.GetDirNum(); i++)
    {
        CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
        if(tTmpDirInfo.strName.compare(strSubDirPath) == 0) {
            tSubDirInfo = tTmpDirInfo;
            break;
        }
    }

    if(tSubDirInfo.nIsValid == 0) return;

    // Find files
    if(tSubDirInfo.pFileNum[eDT_VIDEO] > 0)
    {
        for(int i = tSubDirInfo.pFileIdx[eDT_VIDEO]; i < (tSubDirInfo.pFileIdx[eDT_VIDEO] + tSubDirInfo.pFileNum[eDT_VIDEO]); i++) 
        {
            CDTFileInfo &tFileInfo = m_tDirTree.GetVideoFileInfo(i);
            if(tFileInfo.nIsValid == 0) {
                qDebug("CDirTreeThread::GetVideoFileListWithSubFolder_Impl(). tFileInfo(%d) is invalid", i);
                break;
            }

            int nFirstIdx = tFileInfo.strName.indexOf(".");

            QStringList slist;
            slist.clear();
            slist.append(nFirstIdx >= 0 ? tFileInfo.strName.left(nFirstIdx + 1) : tFileInfo.strName);
            slist.append(tSubDirInfo.strName + "/" + tFileInfo.strName);
            slist.append("");
            queryDataList.append(slist);
        }
    }

    // Find subfolder
    if(tSubDirInfo.nSubDirNum > 0)
    {
        for(int i = tSubDirInfo.nSubDirIdx; i < (tSubDirInfo.nSubDirIdx + tSubDirInfo.nSubDirNum); i++) 
        {
            CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
            if(tTmpDirInfo.pIsValid[eDT_VIDEO] == true) {
                GetVideoFileListWithSubFolder_Impl(tTmpDirInfo.strName, queryDataList);
            }
        }
    }
}

void CDirTreeThread::GetPhotoFileListWithSubFolder_Impl(const QString &strSubDirPath, QVector<QStringList> &queryDataList)
{
    // Find folder
    CDTDirInfo tSubDirInfo;
    for(int i = 0; i < m_tDirTree.GetDirNum(); i++)
    {
        CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
        if(tTmpDirInfo.strName.compare(strSubDirPath) == 0) {
            tSubDirInfo = tTmpDirInfo;
            break;
        }
    }

    if(tSubDirInfo.nIsValid == 0) return;

    // Find files
    if(tSubDirInfo.pFileNum[eDT_PHOTO] > 0)
    {
        for(int i = tSubDirInfo.pFileIdx[eDT_PHOTO]; i < (tSubDirInfo.pFileIdx[eDT_PHOTO] + tSubDirInfo.pFileNum[eDT_PHOTO]); i++) 
        {
            CDTFileInfo &tFileInfo = m_tDirTree.GetPhotoFileInfo(i);
            if(tFileInfo.nIsValid == 0) {
                qDebug("CDirTreeThread::GetPhotoFileListWithSubFolder_Impl(). tFileInfo(%d) is invalid", i);
                break;
            }

            int nFirstIdx = tFileInfo.strName.indexOf(".");

            QStringList slist;
            slist.clear();
            slist.append(nFirstIdx >= 0 ? tFileInfo.strName.left(nFirstIdx + 1) : tFileInfo.strName);
            slist.append(tSubDirInfo.strName + "/" + tFileInfo.strName);
            slist.append("");
            queryDataList.append(slist);
        }
    }

    // Find subfolder
    if(tSubDirInfo.nSubDirNum > 0)
    {
        for(int i = tSubDirInfo.nSubDirIdx; i < (tSubDirInfo.nSubDirIdx + tSubDirInfo.nSubDirNum); i++) 
        {
            CDTDirInfo &tTmpDirInfo = m_tDirTree.GetDirInfo(i);
            if(tTmpDirInfo.pIsValid[eDT_PHOTO] == true) {
                GetPhotoFileListWithSubFolder_Impl(tTmpDirInfo.strName, queryDataList);
            }
        }
    }
}


void CDirTreeThread::StartWait()
{
    m_tMutex.lock();
    while(m_nWorking == 0) {
        m_tCond.wait(&m_tMutex);
    }
    m_tMutex.unlock();
}

void CDirTreeThread::EndWait()
{
    m_tMutex.lock();
    qDebug("CDirTreeThread::============== EndWait() ==============");
    m_nWorking = 0;
    m_tMutex.unlock();
}

bool CDirTreeThread::StartScanDir()
{
    { QString a; LOG_TRACE << a.sprintf("CDirTreeThread::StartScanDir() Start") << LOG_ENDL; }

    int nRet = 0;

    m_tDirTree.Init();

    CDTDirInfo tNewDirInfo;
    tNewDirInfo.strName       = m_strRootPath;
    tNewDirInfo.nParentDirIdx = -1;
    m_tDirTree.AddDir(tNewDirInfo);

    m_tFileTable.clear();   // added by sungha.choi 2013.07.31 for scandir performance

    ScanDirBreadthFirstPosix(m_tDirTree, 0);   // added by sungha.choi 2013.07.31 for scandir performance
    //ScanDirBreadthFirstNameSortGLib(m_tDirTree, 0);    // added by sungha.choi 2013.07.17
    
    NotifyMediaFileFound(m_tDirTree, true);

    if(m_volumeType == eJukeBox) {   
        nRet = m_tDirTree.Write(gScanResult_JukeBox_path);
    }
    else if(m_volumeType == eUSB_FRONT) {   
        nRet = m_tDirTree.Write(gScanResult_USB1_path);
    }
    else if(m_volumeType == eUSB_REAR) {   
        nRet = m_tDirTree.Write(gScanResult_USB2_path);
    }
    
    { QString a; LOG_TRACE    << a.sprintf("CDirTreeThread::StartScanDir() End") << LOG_ENDL; }
    { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::StartScanDir() | %s | Total Folder: %d / Media: %d / A:  %d / V: %d / P: %d",
                                           m_volumeType == eJukeBox ? "Jukebox" : m_volumeType == eUSB_FRONT ? "USB_F" : m_volumeType == eUSB_REAR ? "USB_R" : "??",
                                           m_tDirTree.GetDirNum(), m_tDirTree.GetMediaFileNum(),
                                           m_tDirTree.GetAudioFileNum(), m_tDirTree.GetVideoFileNum(),
                                           m_tDirTree.GetPhotoFileNum() ) << LOG_ENDL; }
    //{ QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::StartScanDir() Total Folder: %d", m_tDirTree.GetDirNum()) << LOG_ENDL; }
    //{ QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::StartScanDir() Total Media : %d", m_tDirTree.GetMediaFileNum()) << LOG_ENDL; }
    //{ QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::StartScanDir() Total Audio : %d", m_tDirTree.GetAudioFileNum()) << LOG_ENDL; }
    //{ QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::StartScanDir() Total Video : %d", m_tDirTree.GetVideoFileNum()) << LOG_ENDL; }
    //{ QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::StartScanDir() Total Photo : %d", m_tDirTree.GetPhotoFileNum()) << LOG_ENDL; }

    // m_tDirTree.PrintDirTree();
    
    return nRet;
}

bool CDirTreeThread::StartReadDir()
{
    qDebug("CDirTreeThread::StartReadDir() Start");

    bool bRet = false;  // modified

    m_tFileTable.clear();   // added by sungha.choi 2013.07.31 for scandir performance
    m_tDirTree.SetHashTable(&m_tFileTable);   // added by sungha.choi 2013.07.31 for scandir performance

    if(m_volumeType == eJukeBox) {   
        bRet = m_tDirTree.Read(gScanResult_JukeBox_path, m_nTreeMode);
    }
    else if(m_volumeType == eUSB_FRONT) {   
        bRet = m_tDirTree.Read(gScanResult_USB1_path);
        //QFile::remove(gScanResult_USB1_path); // removed by kihyung 2013.07.09 for debuging
    }
    else if(m_volumeType == eUSB_REAR) {   
        bRet = m_tDirTree.Read(gScanResult_USB2_path);
        //QFile::remove(gScanResult_USB2_path); // removed by kihyung 2013.07.09 for debuging
    }

    qDebug("CDirTreeThread::StartReadDir() End");

    // m_tDirTree.PrintDirTree();

    return bRet;
}

bool CDirTreeThread::ScanDir(CDirTree &tDirTree, QString strCurrPath, int nCurrDirIdx, int nCurrDirDepth)
{
    m_tStopMutex.lock();
    if(m_bStop) {
        qDebug("CDirTreeThread::ScanDir() stopped by user");
        m_tStopMutex.unlock();
        return false;
    }
    m_tStopMutex.unlock();

    QDir          tSrcDir(strCurrPath);
    QFileInfoList tFileInfoList = tSrcDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

    if(tFileInfoList.size() <= 0) {
        return true;
    }

    int nSubDirIdx             = tDirTree.GetDirNum();
    int nSubDirNum             = 0;
    int nSubFileIdx[eDT_NUM]   = {0,0,0};
    int nSubFileNum[eDT_NUM]   = {0,0,0};

    for(int i = 0; i < tFileInfoList.size(); i++)
    {
        m_tStopMutex.lock();
        if(m_bStop) {
            qDebug("CDirTreeThread::ScanDir() stopped by user");
            m_tStopMutex.unlock();
            return false;
        }
        m_tStopMutex.unlock();
        
        QFileInfo &tFileInfo = tFileInfoList[i];
        
        if(tFileInfo.isHidden()) continue;
        
        if(tFileInfo.isDir() == true) 
        {
            CDTDirInfo tNewDirInfo;

            tNewDirInfo.strName       = tFileInfo.absoluteFilePath();
            tNewDirInfo.nParentDirIdx = nCurrDirIdx;
            nSubDirNum++;
            tDirTree.AddDir(tNewDirInfo);
        }
        else
        {
            QString     strFormat;
            CDTFileInfo tNewFileInfo;

            tNewFileInfo.strName = tFileInfo.fileName();
            tNewFileInfo.nParentDirIdx = nCurrDirIdx;
            
            int nIndex = tNewFileInfo.strName.lastIndexOf(".");
            if(nIndex < 0) continue;
            
            strFormat = tNewFileInfo.strName.mid(nIndex, tNewFileInfo.strName.size() - nIndex).toLower();

            if(m_strAudioFmt.indexOf(strFormat) != -1 ) {
                tDirTree.AddAudioFile(tNewFileInfo);
                nSubFileNum[eDT_AUDIO]++;
                if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
            }
            else if(m_strVideoFmt.indexOf(strFormat) != -1 ) {
                tDirTree.AddVideoFile(tNewFileInfo);
                nSubFileNum[eDT_VIDEO]++;                
                if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
            }
            else if(m_strAudioVideoFmt.indexOf(strFormat) != -1 ) {
                MediaInfoLib::MediaInfo MI;                 
                MI.Open(tFileInfo.filePath().toStdWString());                   
                
                int nVideoStreamCount = QString::fromStdWString(MI.Get(MediaInfoLib::Stream_Video, 0, _T("StreamCount"))).toInt();
                if(nVideoStreamCount == 0)
                {
                    tDirTree.AddAudioFile(tNewFileInfo);
                    nSubFileNum[eDT_AUDIO]++;
                    if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                }
                else if(nVideoStreamCount > 0)
                {
                    tDirTree.AddVideoFile(tNewFileInfo);
                    nSubFileNum[eDT_VIDEO]++;                
                    if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                }
            }
            else if(m_strPhotoFmt.indexOf(strFormat) != -1) {
                tDirTree.AddPhotoFile(tNewFileInfo);
                nSubFileNum[eDT_PHOTO]++;
                if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
            }
            else {
                // qDebug("CDirTreeThread::ScanDir() etc");
                continue;
            }
            if(m_volumeType != eJukeBox && tDirTree.GetMediaFileNum() >= DT_DIR_MEDIA_LIMIT) {  // modified by sungha.choi 2013.07.22
                qDebug("CDirTreeThread::ScanDir(). Total Media File is %d. Stop!!!", DT_DIR_MEDIA_LIMIT);
                break;
            }
        }

        usleep(1); // added by kihyung 2013.3.16
    }

    CDTDirInfo &tCurrDirInfo = tDirTree.GetDirInfo(nCurrDirIdx);
    if(tCurrDirInfo.nIsValid == 0) {
        qDebug("CDirTreeThread::ScanDir(): tCurrDirInfo is invalid");
        return false;
    }

    tCurrDirInfo.nSubDirIdx = nSubDirIdx;
    tCurrDirInfo.nSubDirNum = nSubDirNum;
    tCurrDirInfo.pFileIdx[eDT_AUDIO] = nSubFileNum[eDT_AUDIO] == 0 ? -1 : tDirTree.GetAudioFileNum() - nSubFileNum[eDT_AUDIO];
    tCurrDirInfo.pFileIdx[eDT_VIDEO] = nSubFileNum[eDT_VIDEO] == 0 ? -1 : tDirTree.GetVideoFileNum() - nSubFileNum[eDT_VIDEO];
    tCurrDirInfo.pFileIdx[eDT_PHOTO] = nSubFileNum[eDT_PHOTO] == 0 ? -1 : tDirTree.GetPhotoFileNum() - nSubFileNum[eDT_PHOTO];
    tCurrDirInfo.pFileNum[eDT_AUDIO] = nSubFileNum[eDT_AUDIO];
    tCurrDirInfo.pFileNum[eDT_VIDEO] = nSubFileNum[eDT_VIDEO];
    tCurrDirInfo.pFileNum[eDT_PHOTO] = nSubFileNum[eDT_PHOTO];

    for(int i = 0; i < nSubDirNum && nCurrDirDepth < DT_DIR_DEPTH_LIMIT; i++)
    {
        if(m_volumeType != eJukeBox && tDirTree.GetMediaFileNum() >= DT_DIR_MEDIA_LIMIT) break;  // modified by sungha.choi 2013.07.22
        
        nSubFileIdx[eDT_AUDIO] = tDirTree.GetAudioFileNum();
        nSubFileIdx[eDT_VIDEO] = tDirTree.GetVideoFileNum();
        nSubFileIdx[eDT_PHOTO] = tDirTree.GetPhotoFileNum();

        CDTDirInfo &tSubDirInfo = tDirTree.GetDirInfo(tCurrDirInfo.nSubDirIdx + i);
        if(tSubDirInfo.nIsValid == 0) {
            qDebug("CDirTreeThread::ScanDir(): tSubDirInfo is invalid");
            continue;
        }
        
        if(ScanDir(tDirTree, tSubDirInfo.strName, tCurrDirInfo.nSubDirIdx + i, nCurrDirDepth + 1) == false) {
            return false;
        }

        if(nSubFileIdx[eDT_AUDIO] != tDirTree.GetAudioFileNum())
            tSubDirInfo.pIsValid[eDT_AUDIO] = 1;
        if(nSubFileIdx[eDT_VIDEO] != tDirTree.GetVideoFileNum())
            tSubDirInfo.pIsValid[eDT_VIDEO] = 1;
        if(nSubFileIdx[eDT_PHOTO] != tDirTree.GetPhotoFileNum())
            tSubDirInfo.pIsValid[eDT_PHOTO] = 1;
    }
    
    return true;
}

bool CDirTreeThread::ScanDirGLib(CDirTree &tDirTree, QString strCurrPath, int nCurrDirIdx, int nCurrDirDepth)
{
    m_tStopMutex.lock();
    if(m_bStop) {
        { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDir() stopped by user") << LOG_ENDL; }
        m_tStopMutex.unlock();
        return false;
    }
    m_tStopMutex.unlock();

    GFile           *container = NULL;
    GFileEnumerator *enumerator = NULL;
    GFileInfo       *fileInfo = NULL;
    GError          *error = NULL;
    const gchar     *uriFile;

    QString         strContaner("file://" + strCurrPath);

    container = g_file_new_for_uri(strContaner.toUtf8().data());
    if(container == NULL) {
        { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDir() Unable to create uri: %s", qPrintable(strCurrPath)) << LOG_ENDL; }
        return false;
    }

    // modified by kihyung 2013.07.09 
    enumerator = g_file_enumerate_children(container, FILE_ATTRIBUTES, G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, &error);
    if(error != NULL) {
        { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDir() Unable to enumerate directory: %s", error->message) << LOG_ENDL; }
        g_object_unref(container);
        g_error_free (error);
        return false;
    }

    int nSubDirIdx             = tDirTree.GetDirNum();
    int nSubDirNum             = 0;
    int nSubFileIdx[eDT_NUM]   = {0,0,0};
    int nSubFileNum[eDT_NUM]   = {0,0,0};

    int bExistFlag = 0;
    while((fileInfo = g_file_enumerator_next_file (enumerator, NULL, NULL)) != NULL)
    {
        bExistFlag = 1;
        
        GFileType fileType = g_file_info_get_file_type(fileInfo);

        if(fileType == G_FILE_TYPE_DIRECTORY) 
        {
            CDTDirInfo tNewDirInfo;

            uriFile = g_file_info_get_attribute_byte_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_NAME);
            if(uriFile == NULL) 
            {
                uriFile = g_file_info_get_display_name(fileInfo);
                if(uriFile == NULL) {
                    { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDir() dir uriFile is null") << LOG_ENDL; }
                    g_object_unref(fileInfo);
                    continue;
                }
            }
            if(uriFile[0] == '.') { // added by kihyung 2013.07.09 
                { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstGLib() %s is skipped", uriFile) << LOG_ENDL; }
                g_object_unref(fileInfo);
                continue;
            }
            
            tNewDirInfo.strName       = strCurrPath + "/" + QString::fromUtf8(uriFile);
            tNewDirInfo.nParentDirIdx = nCurrDirIdx;
            nSubDirNum++;
            tDirTree.AddDir(tNewDirInfo);
        }
        else if(fileType == G_FILE_TYPE_REGULAR)
        {
            QString     strFormat;
            CDTFileInfo tNewFileInfo;

            uriFile = g_file_info_get_attribute_byte_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_NAME);
            if(uriFile == NULL) 
            {
                uriFile = g_file_info_get_display_name(fileInfo);
                if(uriFile == NULL) {
                    { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDir() file uriFile is null") << LOG_ENDL; }
                    g_object_unref(fileInfo);
                    continue;
                }
            }
            
            tNewFileInfo.strName       = QString::fromUtf8(uriFile);
            tNewFileInfo.nParentDirIdx = nCurrDirIdx;
            
            int nIndex = tNewFileInfo.strName.lastIndexOf(".");
            if(nIndex < 0) {
                g_object_unref(fileInfo);
                continue;
            }
            
            strFormat = tNewFileInfo.strName.mid(nIndex, tNewFileInfo.strName.size() - nIndex).toLower();

            if(m_strAudioFmt.indexOf(strFormat) != -1 ) 
            {
                tDirTree.AddAudioFile(tNewFileInfo);
                nSubFileNum[eDT_AUDIO]++;
                if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
            }
            else if(m_strVideoFmt.indexOf(strFormat) != -1 ) 
            { 
                tDirTree.AddVideoFile(tNewFileInfo);
                nSubFileNum[eDT_VIDEO]++;                
                if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
            }
            else if(m_strAudioVideoFmt.indexOf(strFormat) != -1 ) 
            {   
                MediaInfoLib::MediaInfo  MI;                 
                MI.Open(tNewFileInfo.strName.toStdWString());                               
                
                int VideoStreamCount = QString::fromStdWString(MI.Get(MediaInfoLib::Stream_Video, 0, _T("StreamCount"))).toInt();
                if( VideoStreamCount == 0 )
                {
                    tDirTree.AddAudioFile(tNewFileInfo);
                    nSubFileNum[eDT_AUDIO]++;
                    if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                }
                else if( VideoStreamCount > 0)
                {
                    tDirTree.AddVideoFile(tNewFileInfo);
                    nSubFileNum[eDT_VIDEO]++;                
                    if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                }
            }
            else if(m_strPhotoFmt.indexOf(strFormat) != -1) 
            {
                tDirTree.AddPhotoFile(tNewFileInfo);
                nSubFileNum[eDT_PHOTO]++;
                if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
            }
            else {
                //qDebug("CDirTreeThread::ScanDir() etc");
                continue;
            }
            if(m_volumeType != eJukeBox && tDirTree.GetMediaFileNum() >= DT_DIR_MEDIA_LIMIT) {  // modified by sungha.choi 2013.07.22
                { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDir(). Total Media File is %d. Stop!!!", DT_DIR_MEDIA_LIMIT) << LOG_ENDL; }
                g_object_unref (fileInfo);
                break;
            }
        }else{
            //{ QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDir() fileType: %d", fileType) << LOG_ENDL; }
        }

        g_object_unref(fileInfo);
    }

    g_file_enumerator_close(enumerator, NULL, NULL);
    g_object_unref(enumerator);
    g_object_unref(container);

    if(bExistFlag == 0) return true;

    CDTDirInfo &tCurrDirInfo = tDirTree.GetDirInfo(nCurrDirIdx);
    if(tCurrDirInfo.nIsValid == 0) {
        { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDir(): tCurrDirInfo is invalid") << LOG_ENDL; }
        return false;
    }

    tCurrDirInfo.nSubDirIdx = nSubDirIdx;
    tCurrDirInfo.nSubDirNum = nSubDirNum;
    tCurrDirInfo.pFileIdx[eDT_AUDIO] = nSubFileNum[eDT_AUDIO] == 0 ? -1 : tDirTree.GetAudioFileNum() - nSubFileNum[eDT_AUDIO];
    tCurrDirInfo.pFileIdx[eDT_VIDEO] = nSubFileNum[eDT_VIDEO] == 0 ? -1 : tDirTree.GetVideoFileNum() - nSubFileNum[eDT_VIDEO];
    tCurrDirInfo.pFileIdx[eDT_PHOTO] = nSubFileNum[eDT_PHOTO] == 0 ? -1 : tDirTree.GetPhotoFileNum() - nSubFileNum[eDT_PHOTO];
    tCurrDirInfo.pFileNum[eDT_AUDIO] = nSubFileNum[eDT_AUDIO];
    tCurrDirInfo.pFileNum[eDT_VIDEO] = nSubFileNum[eDT_VIDEO];
    tCurrDirInfo.pFileNum[eDT_PHOTO] = nSubFileNum[eDT_PHOTO];

    for(int i = 0; i < nSubDirNum && nCurrDirDepth < DT_DIR_DEPTH_LIMIT; i++)
    {
        if(m_volumeType != eJukeBox && tDirTree.GetMediaFileNum() >= DT_DIR_MEDIA_LIMIT) break;  // modified by sungha.choi 2013.07.22
        
        nSubFileIdx[eDT_AUDIO] = tDirTree.GetAudioFileNum();
        nSubFileIdx[eDT_VIDEO] = tDirTree.GetVideoFileNum();
        nSubFileIdx[eDT_PHOTO] = tDirTree.GetPhotoFileNum();

        CDTDirInfo &tSubDirInfo = tDirTree.GetDirInfo(tCurrDirInfo.nSubDirIdx + i);
        if(tSubDirInfo.nIsValid == 0) {
            { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDir(): tSubDirInfo is invalid") << LOG_ENDL; }
            continue;
        }
        
        if(ScanDirGLib(tDirTree, tSubDirInfo.strName, tCurrDirInfo.nSubDirIdx + i, nCurrDirDepth + 1) == false) {
            return false;
        }

        if(nSubFileIdx[eDT_AUDIO] != tDirTree.GetAudioFileNum())
            tSubDirInfo.pIsValid[eDT_AUDIO] = 1;
        if(nSubFileIdx[eDT_VIDEO] != tDirTree.GetVideoFileNum())
            tSubDirInfo.pIsValid[eDT_VIDEO] = 1;
        if(nSubFileIdx[eDT_PHOTO] != tDirTree.GetPhotoFileNum())
            tSubDirInfo.pIsValid[eDT_PHOTO] = 1;
    }
    
    return true;
}

bool CDirTreeThread::FileExists(QString fullPath)   // { added by sungha.choi 2013.07.31 for scandir performance
{
    return m_tFileTable.contains(fullPath);
}   // } added by sungha.choi 2013.07.31 for scandir performance

// { added by kihyung for ITS 0181967 
bool CDirTreeThread::GetFSFolderInfo(QList<FSFolderInfo> &tFolderList, ETrackerAbstarctor_MediaType mediaType)
{
    bool bRet = false;
    
    m_tMutex.lock();
    if(m_nWorking == 0) 
    {
        m_tDirTree.GetFSFolderInfo(tFolderList, mediaType);
        bRet = true;
    }
    m_tMutex.unlock();

    return bRet;
}
// } added by kihyung for ITS 0181967

// { added by sungha.choi 2013.07.13
static int FileNameCompare(const void *file1, const void *file2)
{
    struct dirent *a, *b;

    a = *(struct dirent **)(file1);
    b = *(struct dirent **)(file2);

    return strcmp(a->d_name, b->d_name);
}

bool CDirTreeThread::ScanDirBreadthFirstPosix(CDirTree &tDirTree, int nRootDirIdx)
{
    QQueue<int> m_tQueue;
    m_tQueue.enqueue(nRootDirIdx);

    while(m_tQueue.isEmpty() == false)
    {
        m_tStopMutex.lock();
        if(m_bStop) {
            { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstPosix() stopped by user") << LOG_ENDL; }
            m_tStopMutex.unlock();
            break;
        }
        m_tStopMutex.unlock();

        int nTraversIdx = m_tQueue.dequeue();

        CDTDirInfo &tTraverseDirInfo = tDirTree.GetDirInfo(nTraversIdx);

        struct dirent **items;
        const char *uriFile = NULL;
        unsigned char fileType;

        int nitems = scandir(tTraverseDirInfo.strName.toUtf8().data(), &items, NULL, NULL);
        if(nitems <= 0) {
            continue;
        }
        qsort(items, nitems, sizeof(items[0]), FileNameCompare);
        //{ QString a; LOG_CRITICAL << a.sprintf("[SHCHOI] nTraversIdx = %d nitems = %d", nTraversIdx , nitems) << LOG_ENDL; }

        int nSubDirIdx             = tDirTree.GetDirNum();
        int nSubDirNum             = 0;
        int nSubFileNum[eDT_NUM]   = {0,0,0};
        int i;    // added by sungha.choi 2013.07.29

        for(i = 0; i < nitems; i++)    // modified by sungha.choi 2013.07.29
        {
            m_tStopMutex.lock();
            if(m_bStop) {
                { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstPosix() stopped by user") << LOG_ENDL; }
                m_tStopMutex.unlock();
                return false;
            }
            m_tStopMutex.unlock();

            uriFile = items[i]->d_name;
            fileType = items[i]->d_type;

            if(uriFile == NULL || uriFile[0] == '.' || m_strIgnoreDir.indexOf(uriFile) != -1 )  //modified by honggi.shin, 2014.03.10, for ignoring UNSUPPORTED directory.
            {
                //{ QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstPosix() uriFile is null") << LOG_ENDL; }
                //{ QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstPosix() %s is skipped", uriFile) << LOG_ENDL; }
                if(items[i])
                {
                    free(items[i]);
                    items[i] = NULL;
                }
                continue;
            }

            if( (fileType & DT_DIR) == DT_DIR ) // quick check whether dir is
            {
                QString fullName = tTraverseDirInfo.strName + "/" + QString::fromUtf8(uriFile);
                struct stat fileStat;
                memset(&fileStat, 0x00, sizeof(fileStat));
                if(lstat(fullName.toUtf8().data(), &fileStat) < 0)
                {
                    { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstPosix() : lstat return fail") << LOG_ENDL; }
                    if(items[i])
                    {
                        free(items[i]);
                        items[i] = NULL;
                    }
                    continue;    // modified by sungha.choi 2013.07.29, for crashed folder, sync with GLIB
                }
                if( (fileStat.st_mode & S_IFDIR) == S_IFDIR )    // confirm directory
                {
                    CDTDirInfo tNewDirInfo;

                    tNewDirInfo.strName       = fullName;
                    tNewDirInfo.nParentDirIdx = nTraversIdx;
                    nSubDirNum++;
                    tDirTree.AddDir(tNewDirInfo);

                    m_tQueue.enqueue(tDirTree.GetDirNum() - 1);
                }
                else    // broken folder
                {
                    //{ QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstPosix() : d_type dir, but attr is not dir") << LOG_ENDL; }
                    if(items[i])
                    {
                        free(items[i]);
                        items[i] = NULL;
                    }
                    continue;
                }
            }
            else if( (fileType & DT_REG) == DT_REG ) // quick check whether file is
            {
                QString     strFormat;
                CDTFileInfo tNewFileInfo;

                tNewFileInfo.strName       = QString::fromUtf8(uriFile);
                tNewFileInfo.nParentDirIdx = nTraversIdx;

                QString     fullName = tTraverseDirInfo.strName + "/" + tNewFileInfo.strName;   // added by sungha.choi 2013.07.31 for scandir performance

                int nIndex = tNewFileInfo.strName.lastIndexOf(".");
                if(nIndex < 0) {
                    if(items[i])
                    {
                        free(items[i]);
                        items[i] = NULL;
                    }
                    continue;
                }
                strFormat = tNewFileInfo.strName.mid(nIndex, tNewFileInfo.strName.size() - nIndex).toLower();

                if(m_strAudioFmt.indexOf(strFormat) != -1 )
                {
                    tDirTree.AddAudioFile(tNewFileInfo);
                    nSubFileNum[eDT_AUDIO]++;
                    if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);

                    m_tFileTable.insert(fullName, tDirTree.GetMediaFileNum());   // added by sungha.choi 2013.07.31 for scandir performance
                }
                else if(m_strVideoFmt.indexOf(strFormat) != -1 )
                {
                    tDirTree.AddVideoFile(tNewFileInfo);
                    nSubFileNum[eDT_VIDEO]++;
                    if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);

                    m_tFileTable.insert(fullName, tDirTree.GetMediaFileNum());   // added by sungha.choi 2013.07.31 for scandir performance
                }
                else if(m_strAudioVideoFmt.indexOf(strFormat) != -1 )
                {
                    //QString fullName = tTraverseDirInfo.strName + "/" + QString::fromUtf8(uriFile);    // deleted by sungha.choi 2013.07.31 for scandir performance
                    MediaInfoLib::MediaInfo  MI;
                    MI.Open(fullName.toStdWString());

                    int VideoStreamCount = QString::fromStdWString(MI.Get(MediaInfoLib::Stream_Video, 0, _T("StreamCount"))).toInt();
                    if( VideoStreamCount == 0 )
                    {
                        tDirTree.AddAudioFile(tNewFileInfo);
                        nSubFileNum[eDT_AUDIO]++;
                        if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                    }
                    else if( VideoStreamCount > 0)
                    {
                        tDirTree.AddVideoFile(tNewFileInfo);
                        nSubFileNum[eDT_VIDEO]++;
                        if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                    }
                    MI.Close();   // added by sungha.choi 2013.07.30

                    m_tFileTable.insert(fullName, tDirTree.GetMediaFileNum());   // added by sungha.choi 2013.07.31 for scandir performance
                }
                else if(m_strPhotoFmt.indexOf(strFormat) != -1)
                {
                    tDirTree.AddPhotoFile(tNewFileInfo);
                    nSubFileNum[eDT_PHOTO]++;
                    if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);

                    m_tFileTable.insert(fullName, tDirTree.GetMediaFileNum());   // added by sungha.choi 2013.07.31 for scandir performance
                }
                else {
                    if(items[i])
                    {
                        free(items[i]);
                        items[i] = NULL;
                    }
                    continue;
                }

                if(m_volumeType != eJukeBox && tDirTree.GetMediaFileNum() >= DT_DIR_MEDIA_LIMIT) {  // modified by sungha.choi 2013.07.22
                    m_tQueue.clear();
                    { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstPosix(). Total Media File is %d. Stop!!!", DT_DIR_MEDIA_LIMIT) << LOG_ENDL; }
                    break;
                }
            }
            else
            {
                if(items[i])
                {
                    free(items[i]);
                    items[i] = NULL;
                }
                continue;
            }
            if(items[i])
            {
                free(items[i]);
                items[i] = NULL;
            }
            usleep(1);
        }

        for( ; i < nitems ; i ++){    // { added by sungha.choi 2013.07.29
            if(items[i])
            {
                free(items[i]);
                items[i] = NULL;
            }
        }
        if(items)
        {
            free(items);    // } added by sungha.choi 2013.07.29
            items = NULL;
        }

        tTraverseDirInfo.nSubDirIdx = nSubDirIdx;
        tTraverseDirInfo.nSubDirNum = nSubDirNum;
        tTraverseDirInfo.pFileIdx[eDT_AUDIO] = nSubFileNum[eDT_AUDIO] == 0 ? -1 : tDirTree.GetAudioFileNum() - nSubFileNum[eDT_AUDIO];
        tTraverseDirInfo.pFileIdx[eDT_VIDEO] = nSubFileNum[eDT_VIDEO] == 0 ? -1 : tDirTree.GetVideoFileNum() - nSubFileNum[eDT_VIDEO];
        tTraverseDirInfo.pFileIdx[eDT_PHOTO] = nSubFileNum[eDT_PHOTO] == 0 ? -1 : tDirTree.GetPhotoFileNum() - nSubFileNum[eDT_PHOTO];
        tTraverseDirInfo.pFileNum[eDT_AUDIO] = nSubFileNum[eDT_AUDIO];
        tTraverseDirInfo.pFileNum[eDT_VIDEO] = nSubFileNum[eDT_VIDEO];
        tTraverseDirInfo.pFileNum[eDT_PHOTO] = nSubFileNum[eDT_PHOTO];
    }

    int nAFileNum = 0;
    int nVFileNum = 0;
    int nPFileNum = 0;
    { QString a; LOG_TRACE << a.sprintf("CDirTreeThread::ScanDirBreadthFirstPosix() start CheckMediaValidFlag") << LOG_ENDL; }
    CheckMediaValidFlag(nAFileNum, nVFileNum, nPFileNum, tDirTree, nRootDirIdx);
    { QString a; LOG_TRACE << a.sprintf("CDirTreeThread::ScanDirBreadthFirstPosix() end CheckMediaValidFlag") << LOG_ENDL; }

    return true;
}
// } added by sungha.choi 2013.07.13

bool CDirTreeThread::ScanDirBreadthFirst(CDirTree &tDirTree, int nRootDirIdx)
{
    QQueue<int> m_tQueue;

    m_tQueue.enqueue(nRootDirIdx);

    while(m_tQueue.isEmpty() == false)
    {
        m_tStopMutex.lock();
        if(m_bStop) {
            { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirst() stopped by user") << LOG_ENDL; }
            m_tStopMutex.unlock();
            break;
        }
        m_tStopMutex.unlock();
            
        int nTraversIdx = m_tQueue.dequeue();

        CDTDirInfo &tTraverseDirInfo = tDirTree.GetDirInfo(nTraversIdx);

        QDir          tSrcDir(tTraverseDirInfo.strName);
        QFileInfoList tFileInfoList = tSrcDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

        if(tFileInfoList.size() <= 0) {
            continue;
        }

        int nSubDirIdx             = tDirTree.GetDirNum();
        int nSubDirNum             = 0;
        int nSubFileNum[eDT_NUM]   = {0,0,0};

        for(int i = 0; i < tFileInfoList.size(); i++)
        {
            m_tStopMutex.lock();
            if(m_bStop) {
                { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirst() stopped by user") << LOG_ENDL; }
                m_tStopMutex.unlock();
                return false;
            }
            m_tStopMutex.unlock();
            
            QFileInfo &tFileInfo = tFileInfoList[i];
            
            if(tFileInfo.isHidden()) continue;
            
            if(tFileInfo.isDir() == true) 
            {
                CDTDirInfo tNewDirInfo;

                tNewDirInfo.strName       = tFileInfo.absoluteFilePath();
                tNewDirInfo.nParentDirIdx = nTraversIdx;
                nSubDirNum++;
                tDirTree.AddDir(tNewDirInfo);

                // qDebug("DirTree: D %s", qPrintable(tNewDirInfo.strName));
                
                m_tQueue.enqueue(tDirTree.GetDirNum() - 1);
            }
            else
            {
                QString     strFormat;
                CDTFileInfo tNewFileInfo;

                tNewFileInfo.strName       = tFileInfo.fileName();
                tNewFileInfo.nParentDirIdx = nTraversIdx;
                
                int nIndex = tNewFileInfo.strName.lastIndexOf(".");
                if(nIndex < 0) continue;
                
                strFormat = tNewFileInfo.strName.mid(nIndex, tNewFileInfo.strName.size() - nIndex).toLower();

                // qDebug("DirTree: F %s", qPrintable(tNewFileInfo.strName));

                if(m_strAudioFmt.indexOf(strFormat) != -1 ) {
                    tDirTree.AddAudioFile(tNewFileInfo);
                    nSubFileNum[eDT_AUDIO]++;
                    if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                }
                else if(m_strVideoFmt.indexOf(strFormat) != -1 ) {
                    tDirTree.AddVideoFile(tNewFileInfo);
                    nSubFileNum[eDT_VIDEO]++;
                    if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                }
                else if(m_strAudioVideoFmt.indexOf(strFormat) != -1 ) {
                    MediaInfoLib::MediaInfo MI;                 
                    MI.Open(tFileInfo.filePath().toStdWString());                   
                    
                    int nVideoStreamCount = QString::fromStdWString(MI.Get(MediaInfoLib::Stream_Video, 0, _T("StreamCount"))).toInt();
                    if(nVideoStreamCount == 0)
                    {
                        tDirTree.AddAudioFile(tNewFileInfo);
                        nSubFileNum[eDT_AUDIO]++;
                        if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                    }
                    else if(nVideoStreamCount > 0)
                    {
                        tDirTree.AddVideoFile(tNewFileInfo);
                        nSubFileNum[eDT_VIDEO]++;                
                        if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                    }                    
                }
                else if(m_strPhotoFmt.indexOf(strFormat) != -1) {
                    tDirTree.AddPhotoFile(tNewFileInfo);
                    nSubFileNum[eDT_PHOTO]++;
                    if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                }
                else {
                    // qDebug("CDirTreeThread::ScanDir() etc");
                    continue;
                }

                if(m_volumeType != eJukeBox && tDirTree.GetMediaFileNum() >= DT_DIR_MEDIA_LIMIT) {  // modified by sungha.choi 2013.07.22
                    m_tQueue.clear();
                    { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirst(). Total Media File is %d. Stop!!!", DT_DIR_MEDIA_LIMIT) << LOG_ENDL; }
                    break;
                }
            }

            usleep(1); // added by kihyung 2013.3.16
        }

        tTraverseDirInfo.nSubDirIdx = nSubDirIdx;
        tTraverseDirInfo.nSubDirNum = nSubDirNum;
        tTraverseDirInfo.pFileIdx[eDT_AUDIO] = nSubFileNum[eDT_AUDIO] == 0 ? -1 : tDirTree.GetAudioFileNum() - nSubFileNum[eDT_AUDIO];
        tTraverseDirInfo.pFileIdx[eDT_VIDEO] = nSubFileNum[eDT_VIDEO] == 0 ? -1 : tDirTree.GetVideoFileNum() - nSubFileNum[eDT_VIDEO];
        tTraverseDirInfo.pFileIdx[eDT_PHOTO] = nSubFileNum[eDT_PHOTO] == 0 ? -1 : tDirTree.GetPhotoFileNum() - nSubFileNum[eDT_PHOTO];
        tTraverseDirInfo.pFileNum[eDT_AUDIO] = nSubFileNum[eDT_AUDIO];
        tTraverseDirInfo.pFileNum[eDT_VIDEO] = nSubFileNum[eDT_VIDEO];
        tTraverseDirInfo.pFileNum[eDT_PHOTO] = nSubFileNum[eDT_PHOTO];
    }

    int nAFileNum = 0;
    int nVFileNum = 0;
    int nPFileNum = 0;
    { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirst() start CheckMediaValidFlag") << LOG_ENDL; }
    CheckMediaValidFlag(nAFileNum, nVFileNum, nPFileNum, tDirTree, nRootDirIdx);
    { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirst() end CheckMediaValidFlag") << LOG_ENDL; }
    
    return true;
}

// { added by sungha.choi 2013.07.17
class FileNode
{
    public:
        QString filename;
        GFileType filetype;
};

static gint filename_compare(gconstpointer a, gconstpointer b)
{
    return QString::compare(((FileNode *)a)->filename, ((FileNode *)b)->filename, Qt::CaseInsensitive);
}

bool CDirTreeThread::ScanDirBreadthFirstNameSortGLib(CDirTree &tDirTree, int nRootDirIdx)
{
    int max_filenum = 1000;
    QQueue<int> m_tQueue;
    GList *filelist = NULL;
    FileNode **filedata = NULL;

    filedata = new FileNode*[max_filenum];
    for(int i = 0 ; i < max_filenum ; i ++){
        filedata[i] = new FileNode;
    }

    m_tQueue.enqueue(nRootDirIdx);

    while(m_tQueue.isEmpty() == false)
    {
        m_tStopMutex.lock();
        if(m_bStop) {
            { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstNameSortGLib() stopped by user") << LOG_ENDL; }
            m_tStopMutex.unlock();
            break;
        }
        m_tStopMutex.unlock();
            
        int nTraversIdx = m_tQueue.dequeue();

        CDTDirInfo &tTraverseDirInfo = tDirTree.GetDirInfo(nTraversIdx);

        GFile           *container  = NULL;
        GFileEnumerator *enumerator = NULL;
        GFileInfo       *fileInfo   = NULL;
        GError          *error      = NULL;
        const gchar     *uriFile    = NULL;

        QString         strContaner("file://" + tTraverseDirInfo.strName);

        container = g_file_new_for_uri(strContaner.toUtf8().data());
        if(container == NULL) {
            { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstNameSortGLib() Unable to create uri: %s", qPrintable(tTraverseDirInfo.strName)) << LOG_ENDL; }
            continue;
        }

        // modified by kihyung 2013.07.09 
        enumerator = g_file_enumerate_children(container, FILE_ATTRIBUTES, G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, &error);
        if(error != NULL) {
            { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstNameSortGLib() Unable to enumerate directory: %s", error->message) << LOG_ENDL; }
            g_object_unref(container);
            g_error_free (error);
            continue;
        }

        // get the number of files in directory
        struct dirent **items;
        int nitems = scandir(tTraverseDirInfo.strName.toUtf8().data(), &items, NULL, NULL);

        // adjust filedata size
        if(nitems > max_filenum){
            for(int i = 0 ; i < max_filenum ; i ++){
                delete filedata[i];
            }
            delete[] filedata;
            filedata = NULL;

            if( (filedata = new FileNode*[nitems]) == NULL){
                { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstNameSortGLib() Unable to alloc filedata") << LOG_ENDL; }
                g_object_unref(enumerator);
                g_object_unref(container);
                break;
            }

            for(int i = 0 ; i < nitems ; i ++){
                filedata[i] = new FileNode;
            }
            max_filenum = nitems;
        }

        int nCount = 0;
        while((fileInfo = g_file_enumerator_next_file (enumerator, NULL, NULL)) != NULL)
        {
            m_tStopMutex.lock();
            if(m_bStop) {
                { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstNameSortGLib() stopped by user") << LOG_ENDL; }
                m_tStopMutex.unlock();
                g_object_unref(fileInfo);
                break;
            }
            m_tStopMutex.unlock();

            uriFile = g_file_info_get_display_name(fileInfo);
            if(uriFile == NULL) {
                uriFile = g_file_info_get_attribute_byte_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_NAME);
                if(uriFile == NULL) {
                    g_object_unref(fileInfo);
                    continue;
                }
            }
            if(uriFile[0] == '.') {
                g_object_unref(fileInfo);
                continue;
            }

            filedata[nCount]->filename = QString::fromUtf8(uriFile);
            filedata[nCount]->filetype = g_file_info_get_file_type(fileInfo);
            filelist = g_list_prepend(filelist, filedata[nCount]);
            g_object_unref(fileInfo);
            nCount ++;
            if(nCount >= max_filenum){
                break;
            }
            usleep(1);
        }

        m_tStopMutex.lock();
        if(!m_bStop){
            filelist = g_list_sort(filelist, filename_compare);
        }
        m_tStopMutex.unlock();

        int nSubDirIdx             = tDirTree.GetDirNum();
        int nSubDirNum             = 0;
        int nSubFileNum[eDT_NUM]   = {0,0,0};

        int bExistFlag = 0;

        for(GList *list = filelist; list; list = list->next)
        {
            m_tStopMutex.lock();
            if(m_bStop) {
                { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstNameSortGLib() stopped by user") << LOG_ENDL; }
                m_tStopMutex.unlock();
                break;
            }
            m_tStopMutex.unlock();

            bExistFlag = 1;
            FileNode *filenode = (FileNode *)list->data;

            if(filenode->filetype == G_FILE_TYPE_DIRECTORY)
            {
                CDTDirInfo tNewDirInfo;
                tNewDirInfo.strName       = tTraverseDirInfo.strName + "/" + filenode->filename;
                tNewDirInfo.nParentDirIdx = nTraversIdx;
                nSubDirNum++;
                tDirTree.AddDir(tNewDirInfo);
                m_tQueue.enqueue(tDirTree.GetDirNum() - 1);
            }
            else if(filenode->filetype == G_FILE_TYPE_REGULAR)
            {
                QString     strFormat;
                CDTFileInfo tNewFileInfo;

                tNewFileInfo.strName       = filenode->filename;;
                tNewFileInfo.nParentDirIdx = nTraversIdx;
                
                int nIndex = tNewFileInfo.strName.lastIndexOf(".");
                if(nIndex < 0) {
                    continue;
                }
                
                strFormat = tNewFileInfo.strName.mid(nIndex, tNewFileInfo.strName.size() - nIndex).toLower();

                if(m_strAudioFmt.indexOf(strFormat) != -1 ) 
                {
                    tDirTree.AddAudioFile(tNewFileInfo);
                    nSubFileNum[eDT_AUDIO]++;
                    if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                }
                else if(m_strVideoFmt.indexOf(strFormat) != -1 ) 
                { 
                    tDirTree.AddVideoFile(tNewFileInfo);
                    nSubFileNum[eDT_VIDEO]++;
                    if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                }
                else if(m_strAudioVideoFmt.indexOf(strFormat) != -1 ) 
                {   
                    MediaInfoLib::MediaInfo  MI;              
                       
                    // { modified by eugene.seo 2013.07.25
                    // MI.Open(tNewFileInfo.strName.toUtf8().data()); 
                    QString strPath = tTraverseDirInfo.strName + "/" + tNewFileInfo.strName;
                    MI.Open(strPath.toStdWString());       
                    // } modified by eugene.seo 2013.07.25					              
                    
                    int VideoStreamCount = QString::fromStdWString(MI.Get(MediaInfoLib::Stream_Video, 0, _T("StreamCount"))).toInt();
                    if( VideoStreamCount == 0 )
                    {
                        tDirTree.AddAudioFile(tNewFileInfo);
                        nSubFileNum[eDT_AUDIO]++;
                        if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                    }
                    else if( VideoStreamCount > 0)
                    {
                        tDirTree.AddVideoFile(tNewFileInfo);
                        nSubFileNum[eDT_VIDEO]++;   
                        if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                    }
                }
                else if(m_strPhotoFmt.indexOf(strFormat) != -1) 
                {
                    tDirTree.AddPhotoFile(tNewFileInfo);
                    nSubFileNum[eDT_PHOTO]++;
                    if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                }
                else {
                    continue;
                }

                if(m_volumeType != eJukeBox && tDirTree.GetMediaFileNum() >= DT_DIR_MEDIA_LIMIT) {  // modified by sungha.choi 2013.07.22
                    m_tQueue.clear();
                    { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstNameSortGLib(). Total Media File is %d. Stop!!!", DT_DIR_MEDIA_LIMIT) << LOG_ENDL; }
                    break;
                }
                else {
                    continue;
                }
            }
            usleep(1);
        }

        g_file_enumerator_close(enumerator, NULL, NULL);
        g_object_unref(enumerator);
        g_object_unref(container);

        if(filelist != NULL){
            g_list_free(filelist);
            filelist = NULL;
        }

        if(bExistFlag == 0) continue;

        tTraverseDirInfo.nSubDirIdx = nSubDirIdx;
        tTraverseDirInfo.nSubDirNum = nSubDirNum;
        tTraverseDirInfo.pFileIdx[eDT_AUDIO] = nSubFileNum[eDT_AUDIO] == 0 ? -1 : tDirTree.GetAudioFileNum() - nSubFileNum[eDT_AUDIO];
        tTraverseDirInfo.pFileIdx[eDT_VIDEO] = nSubFileNum[eDT_VIDEO] == 0 ? -1 : tDirTree.GetVideoFileNum() - nSubFileNum[eDT_VIDEO];
        tTraverseDirInfo.pFileIdx[eDT_PHOTO] = nSubFileNum[eDT_PHOTO] == 0 ? -1 : tDirTree.GetPhotoFileNum() - nSubFileNum[eDT_PHOTO];
        tTraverseDirInfo.pFileNum[eDT_AUDIO] = nSubFileNum[eDT_AUDIO];
        tTraverseDirInfo.pFileNum[eDT_VIDEO] = nSubFileNum[eDT_VIDEO];
        tTraverseDirInfo.pFileNum[eDT_PHOTO] = nSubFileNum[eDT_PHOTO];
    }

    int nAFileNum = 0;
    int nVFileNum = 0;
    int nPFileNum = 0;

    { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstNameSortGLib() start CheckMediaValidFlag") << LOG_ENDL; }
    CheckMediaValidFlag(nAFileNum, nVFileNum, nPFileNum, tDirTree, nRootDirIdx);
    { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstNameSortGLib() end CheckMediaValidFlag") << LOG_ENDL; }

    if(filelist != NULL){
        g_list_free(filelist);
        filelist = NULL;
    }

    if(filedata != NULL)
    {
        for(int i = 0 ; i < max_filenum ; i ++){
            delete filedata[i];
        }
        delete[] filedata;
    }
    return true;
}
// } added by sungha.choi 2013.07.17


bool CDirTreeThread::ScanDirBreadthFirstGLib(CDirTree &tDirTree, int nRootDirIdx)
{
    QQueue<int> m_tQueue;

    m_tQueue.enqueue(nRootDirIdx);

    while(m_tQueue.isEmpty() == false)
    {
        m_tStopMutex.lock();
        if(m_bStop) {
            { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstGLib() stopped by user") << LOG_ENDL; }
            m_tStopMutex.unlock();
            break;
        }
        m_tStopMutex.unlock();
            
        int nTraversIdx = m_tQueue.dequeue();

        CDTDirInfo &tTraverseDirInfo = tDirTree.GetDirInfo(nTraversIdx);

        GFile           *container  = NULL;
        GFileEnumerator *enumerator = NULL;
        GFileInfo       *fileInfo   = NULL;
        GError          *error      = NULL;
        const gchar     *uriFile    = NULL;

        QString         strContaner("file://" + tTraverseDirInfo.strName);

        container = g_file_new_for_uri(strContaner.toUtf8().data());
        if(container == NULL) {
            { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstGLib() Unable to create uri: %s", qPrintable(tTraverseDirInfo.strName)) << LOG_ENDL; }
            continue;
        }

        // modified by kihyung 2013.07.09 
        enumerator = g_file_enumerate_children(container, FILE_ATTRIBUTES, G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, &error);
        if(error != NULL) {
            { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstGLib() Unable to enumerate directory: %s", error->message) << LOG_ENDL; }
            g_object_unref(container);
            g_error_free (error);
            continue;
        }

        int nSubDirIdx             = tDirTree.GetDirNum();
        int nSubDirNum             = 0;
        int nSubFileNum[eDT_NUM]   = {0,0,0};

        int bExistFlag = 0;
        while((fileInfo = g_file_enumerator_next_file (enumerator, NULL, NULL)) != NULL)
        {
            m_tStopMutex.lock();
            if(m_bStop) {
                { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstGLib() stopped by user") << LOG_ENDL; }
                m_tStopMutex.unlock();
                g_object_unref(fileInfo);
                break;    // modified by sungha.choi 2013.07.17
            }
            m_tStopMutex.unlock();
            
            bExistFlag = 1;
            
            GFileType fileType = g_file_info_get_file_type(fileInfo);

            if(fileType == G_FILE_TYPE_DIRECTORY) 
            {
                CDTDirInfo tNewDirInfo;

                // { modified by kihyung 2013.07.09 
                uriFile = g_file_info_get_attribute_byte_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_NAME);
                if(uriFile == NULL) 
                {
                    //{ QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstGLib() G_FILE_ATTRIBUTE_STANDARD_NAME is null") << LOG_ENDL; }
                    uriFile = g_file_info_get_display_name(fileInfo);
                    if(uriFile == NULL) {
                        //{ QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstGLib() G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME is null") << LOG_ENDL; }
                        g_object_unref(fileInfo);
                        continue;
                    }
                }
                if(uriFile[0] == '.') {
                    //{ QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstGLib() %s is skipped", uriFile) << LOG_ENDL; }
                    g_object_unref(fileInfo);
                    continue;
                }
                // } modified by kihyung 2013.07.09 
                
                tNewDirInfo.strName       = tTraverseDirInfo.strName + "/" + QString::fromUtf8(uriFile);
                tNewDirInfo.nParentDirIdx = nTraversIdx;
                nSubDirNum++;
                tDirTree.AddDir(tNewDirInfo);

                // gDebug("DirTree: D %s", qPrintable(tNewDirInfo.strName));

                m_tQueue.enqueue(tDirTree.GetDirNum() - 1);
            }
            else if(fileType == G_FILE_TYPE_REGULAR)
            {
                QString     strFormat;
                CDTFileInfo tNewFileInfo;

                // { modified by kihyung 2013.07.09 
                uriFile = g_file_info_get_attribute_byte_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_NAME);
                if(uriFile == NULL) 
                {
                    uriFile = g_file_info_get_display_name(fileInfo);
                    if(uriFile == NULL) {
                        g_object_unref(fileInfo);
                        continue;
                    }
                }
                // } modified by kihyung 2013.07.09 
                
                tNewFileInfo.strName       = QString::fromUtf8(uriFile);
                tNewFileInfo.nParentDirIdx = nTraversIdx;
                
                int nIndex = tNewFileInfo.strName.lastIndexOf(".");
                if(nIndex < 0) {
                    g_object_unref(fileInfo);
                    continue;
                }
                
                strFormat = tNewFileInfo.strName.mid(nIndex, tNewFileInfo.strName.size() - nIndex).toLower();

                if(m_strAudioFmt.indexOf(strFormat) != -1 ) 
                {
                    tDirTree.AddAudioFile(tNewFileInfo);
                    nSubFileNum[eDT_AUDIO]++;
                    if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                }
                else if(m_strVideoFmt.indexOf(strFormat) != -1 ) 
                { 
                    tDirTree.AddVideoFile(tNewFileInfo);
                    nSubFileNum[eDT_VIDEO]++;
                    if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                }
                else if(m_strAudioVideoFmt.indexOf(strFormat) != -1 ) 
                {   
                    MediaInfoLib::MediaInfo  MI;                 
                    MI.Open(tNewFileInfo.strName.toStdWString());                               
                    
                    int VideoStreamCount = QString::fromStdWString(MI.Get(MediaInfoLib::Stream_Video, 0, _T("StreamCount"))).toInt();
                    if( VideoStreamCount == 0 )
                    {
                        tDirTree.AddAudioFile(tNewFileInfo);
                        nSubFileNum[eDT_AUDIO]++;
                        if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                    }
                    else if( VideoStreamCount > 0)
                    {
                        tDirTree.AddVideoFile(tNewFileInfo);
                        nSubFileNum[eDT_VIDEO]++;   
                        if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                    }
                }
                else if(m_strPhotoFmt.indexOf(strFormat) != -1) 
                {
                    tDirTree.AddPhotoFile(tNewFileInfo);
                    nSubFileNum[eDT_PHOTO]++;
                    if(m_bAllMediaFound == false) NotifyMediaFileFound(tDirTree);
                }
                else {
                    g_object_unref(fileInfo);
                    continue;
                }

                if(m_volumeType != eJukeBox && tDirTree.GetMediaFileNum() >= DT_DIR_MEDIA_LIMIT) {  // modified by sungha.choi 2013.07.22
                    m_tQueue.clear();
                    { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstGLib(). Total Media File is %d. Stop!!!", DT_DIR_MEDIA_LIMIT) << LOG_ENDL; }
                    g_object_unref (fileInfo);
                    break;
                }
                else {
                    g_object_unref(fileInfo);
                    continue;
                }
            }

            g_object_unref(fileInfo);
            usleep(1);
        }

        g_file_enumerator_close(enumerator, NULL, NULL);
        g_object_unref(enumerator);
        g_object_unref(container);

        if(bExistFlag == 0) continue;

        tTraverseDirInfo.nSubDirIdx = nSubDirIdx;
        tTraverseDirInfo.nSubDirNum = nSubDirNum;
        tTraverseDirInfo.pFileIdx[eDT_AUDIO] = nSubFileNum[eDT_AUDIO] == 0 ? -1 : tDirTree.GetAudioFileNum() - nSubFileNum[eDT_AUDIO];
        tTraverseDirInfo.pFileIdx[eDT_VIDEO] = nSubFileNum[eDT_VIDEO] == 0 ? -1 : tDirTree.GetVideoFileNum() - nSubFileNum[eDT_VIDEO];
        tTraverseDirInfo.pFileIdx[eDT_PHOTO] = nSubFileNum[eDT_PHOTO] == 0 ? -1 : tDirTree.GetPhotoFileNum() - nSubFileNum[eDT_PHOTO];
        tTraverseDirInfo.pFileNum[eDT_AUDIO] = nSubFileNum[eDT_AUDIO];
        tTraverseDirInfo.pFileNum[eDT_VIDEO] = nSubFileNum[eDT_VIDEO];
        tTraverseDirInfo.pFileNum[eDT_PHOTO] = nSubFileNum[eDT_PHOTO];
    }

    int nAFileNum = 0;
    int nVFileNum = 0;
    int nPFileNum = 0;

    { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstGLib() start CheckMediaValidFlag") << LOG_ENDL; }
    CheckMediaValidFlag(nAFileNum, nVFileNum, nPFileNum, tDirTree, nRootDirIdx);
    { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::ScanDirBreadthFirstGLib() end CheckMediaValidFlag") << LOG_ENDL; }
    
    return true;
}

bool CDirTreeThread::CheckMediaValidFlag(int &nAFileNum, int &nVFileNum, int &nPFileNum, CDirTree &tDirTree, int nRootDirIdx)
{
    int         nSubFileIdx[eDT_NUM]  = {0,0,0};
    CDTDirInfo &tCurrDirInfo          = tDirTree.GetDirInfo(nRootDirIdx);

    if(tCurrDirInfo.nIsValid == 0) {
        { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::CheckMediaValidFlag(): tCurrDirInfo is invalid") << LOG_ENDL; }
        return false;
    }

    nAFileNum += tCurrDirInfo.pFileNum[eDT_AUDIO];
    nVFileNum += tCurrDirInfo.pFileNum[eDT_VIDEO];
    nPFileNum += tCurrDirInfo.pFileNum[eDT_PHOTO];

    for(int i = 0; i < tCurrDirInfo.nSubDirNum; i++)
    {
        CDTDirInfo &tSubDirInfo = tDirTree.GetDirInfo(tCurrDirInfo.nSubDirIdx + i);
        if(tSubDirInfo.nIsValid == 0) {
            { QString a; LOG_CRITICAL << a.sprintf("CDirTreeThread::CheckMediaValidFlag(): tSubDirInfo is invalid") << LOG_ENDL; }
            break;
        }

        nSubFileIdx[eDT_AUDIO] = nAFileNum;
        nSubFileIdx[eDT_VIDEO] = nVFileNum;
        nSubFileIdx[eDT_PHOTO] = nPFileNum;

        if(CheckMediaValidFlag(nAFileNum, nVFileNum, nPFileNum, tDirTree, tCurrDirInfo.nSubDirIdx + i) == false) {
            return false;
        }

        if(nSubFileIdx[eDT_AUDIO] != nAFileNum)
            tSubDirInfo.pIsValid[eDT_AUDIO] = 1;
        if(nSubFileIdx[eDT_VIDEO] != nVFileNum)
            tSubDirInfo.pIsValid[eDT_VIDEO] = 1;
        if(nSubFileIdx[eDT_PHOTO] != nPFileNum)
            tSubDirInfo.pIsValid[eDT_PHOTO] = 1;
    }

    return true;
}

bool CDirTreeThread::CheckInternalRead()
{
    bool bRet = false;
    m_tMutex.lock();
    if(m_bScanReadComplete == false)   // modified by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    {
        bRet          = true;
        m_nWorking    = 1;
        m_bStop       = false;
        m_nTreeMode   = DT_FS_READ_MODE;
        m_bRescanMode = false;
        qDebug("[KKA]CDirTreeThread::CheckInternalRead() ---> StartReadScanResult() ");
        //m_tCond.wakeAll();   // deleted by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
        ReadDirInSelfThread();   // added by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    }
    m_tMutex.unlock();    
    return bRet;
}

CDirTreeProvider* CDirTreeProvider::GetInstance()
{

    if(m_pInstance == 0) {
        m_pInstance = new CDirTreeProvider();
    }

    return m_pInstance;
}

bool CDirTreeProvider::IsDirTreeQuery(ETrackerAbstarctor_Query_Ids queryID)
{
    switch(queryID)
    {
    //case eGetVideoLimitFileListDir_Id:
    case eGetMusicLimitFileListDir_Id:
    //case eGetPhotoLimitFileListDir_Id:
    //case eGetImagesByFolderQuery_Id:
    //case eGetVideosByFolderQuery_Id:
    //case eGetImagesBySubFolderQuery_Id:
    //case eGetVideosBySubFolderQuery_Id:
    case eGetFolderTabList_Id:
    //case eGetVideoFolderTabList_Id:
    //case eGetPhotoFolderTabList_Id:
    //case eGetFirstImagePathByGivenFolder:
    //case eGetImageCountByGivenFolder:
    //case eGetFirstImageAndCountByGivenFolder:
        return true;
    default:
        return false;
    }
}

CDirTreeProvider::~CDirTreeProvider()
{
    delete m_pJukebox;
    delete m_pUSB1;
    delete m_pUSB2;
}

void CDirTreeProvider::SetSWVersion(QString strVersion)
{
    if(strVersion.isEmpty() == false) {
        gTA_VERSION = strVersion;
    }
}

void CDirTreeProvider::SetFileSystemScanMode(bool bScan)
{
    m_pJukebox->SetFileSystemScanMode(bScan);
    m_pUSB1->SetFileSystemScanMode(bScan);
    m_pUSB2->SetFileSystemScanMode(bScan);
}

bool CDirTreeProvider::StartScanFileSystem(ETrackerAbstarctor_VolumeType volumeType, bool bRescanMode)
{
    qDebug("CDirTreeProvider::StartScanFileSystem(%d)", volumeType);
    
    if(volumeType == eJukeBox) {
        return m_pJukebox->StartScanFileSystem(m_pJukebox->GetMountPath(), bRescanMode);
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->StartScanFileSystem(m_pUSB1->GetMountPath(), bRescanMode);
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->StartScanFileSystem(m_pUSB2->GetMountPath(), bRescanMode);
    }
    else {
        qDebug("CDirTreeProvider::StartScanFileSystem() invalid volumeType(%d)!!!", volumeType);
        return false;
    }
}

bool CDirTreeProvider::StopScanFileSystem(ETrackerAbstarctor_VolumeType volumeType)
{
    qDebug("CDirTreeProvider::StopScanFileSystem(%d)", volumeType);
    
    if(volumeType == eJukeBox) {
        return m_pJukebox->StopScanFileSystem();
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->StopScanFileSystem();
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->StopScanFileSystem();
    }
    else {
        qDebug("CDirTreeProvider::StopScanFileSystem() invalid volumeType(%d)!!!", volumeType);
        return false;
    }
}

bool CDirTreeProvider::StartReadScanResult(ETrackerAbstarctor_VolumeType volumeType)
{
    qDebug("CDirTreeProvider::ReadScanResult(%d)", volumeType);

    if(volumeType == eJukeBox) {
        return m_pJukebox->StartReadScanResult();
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->StartReadScanResult();
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->StartReadScanResult();
    }
    else {
        qDebug("CDirTreeProvider::StartReadScanResult() invalid volumeType(%d)!!!", volumeType);
        return false;
    }
}

bool CDirTreeProvider::StopReadScanResult(ETrackerAbstarctor_VolumeType volumeType)
{
    qDebug("CDirTreeProvider::ReadScanResult(%d)", volumeType);
    
    if(volumeType == eJukeBox) {
        return m_pJukebox->StopReadScanResult();
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->StopReadScanResult();
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->StopReadScanResult();
    }
    else {
        qDebug("CDirTreeProvider::StopReadScanResult() invalid volumeType(%d)!!!", volumeType);
        return false;
    }
}

int CDirTreeProvider::GetAudioFileNum(ETrackerAbstarctor_VolumeType  volumeType)
{
    qDebug("CDirTreeProvider::GetAudioFileNum(%d)", volumeType);
    
    if(volumeType == eJukeBox) {
        return m_pJukebox->GetAudioFileNum();
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetAudioFileNum();
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetAudioFileNum();
    }
    else {
        qDebug("CDirTreeProvider::GetAudioFileNum() invalid volumeType(%d)!!!", volumeType);
        return 0;
    } 
}

int CDirTreeProvider::GetVideoFileNum(ETrackerAbstarctor_VolumeType  volumeType)
{
    qDebug("CDirTreeProvider::GetVideoFileNum(%d)", volumeType);
    
    if(volumeType == eJukeBox) {
        return m_pJukebox->GetVideoFileNum();
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetVideoFileNum();
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetVideoFileNum();
    }
    else {
        qDebug("CDirTreeProvider::GetVideoFileNum() invalid volumeType(%d)!!!", volumeType);
        return 0;
    } 
}

int CDirTreeProvider::GetPhotoFileNum(ETrackerAbstarctor_VolumeType  volumeType)
{
    qDebug("CDirTreeProvider::GetVideoFileNum(%d)", volumeType);
    
    if(volumeType == eJukeBox) {
        return m_pJukebox->GetPhotoFileNum();
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetPhotoFileNum();
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetPhotoFileNum();
    }
    else {
        qDebug("CDirTreeProvider::GetVideoFileNum() invalid volumeType(%d)!!!", volumeType);
        return 0;
    } 
}

QString CDirTreeProvider::GetFirstAudioFolder(ETrackerAbstarctor_VolumeType volumeType)
{
    qDebug("CDirTreeProvider::GetFirstAudioFolder(%d)", volumeType);
    
    if(volumeType == eJukeBox) {
        return m_pJukebox->GetFirstMediaFolder(eDT_AUDIO);
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetFirstMediaFolder(eDT_AUDIO);
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetFirstMediaFolder(eDT_AUDIO);
    }
    else {
        qDebug("CDirTreeProvider::GetFirstAudioFolder() invalid volumeType(%d)!!!", volumeType);
        return QString();
    } 
}

QString CDirTreeProvider::GetFirstVideoFolder(ETrackerAbstarctor_VolumeType volumeType)
{
    qDebug("CDirTreeProvider::GetFirstVideoFolder(%d)", volumeType);
    
    if(volumeType == eJukeBox) {
        return m_pJukebox->GetFirstMediaFolder(eDT_VIDEO);
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetFirstMediaFolder(eDT_VIDEO);
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetFirstMediaFolder(eDT_VIDEO);
    }
    else {
        qDebug("CDirTreeProvider::GetFirstVideoFolder() invalid volumeType(%d)!!!", volumeType);
        return QString();
    } 
}

QString CDirTreeProvider::GetFirstPhotoFolder(ETrackerAbstarctor_VolumeType volumeType)
{
    qDebug("CDirTreeProvider::GetFirstPhotoFolder(%d)", volumeType);
    
    if(volumeType == eJukeBox) {
        return m_pJukebox->GetFirstMediaFolder(eDT_PHOTO);
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetFirstMediaFolder(eDT_PHOTO);
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetFirstMediaFolder(eDT_PHOTO);
    }
    else {
        qDebug("CDirTreeProvider::GetFirstPhotoFolder() invalid volumeType(%d)!!!", volumeType);
        return QString();
    } 
}

bool CDirTreeProvider::GetAllAudioFileList(ETrackerAbstarctor_VolumeType  volumeType, uint reqUID)
{
    qDebug("CDirTreeProvider::GetAllAudioFileList(%d)", volumeType);
    
    if(volumeType == eJukeBox) {
        return m_pJukebox->GetAllAudioFileList(reqUID);
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetAllAudioFileList(reqUID);
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetAllAudioFileList(reqUID);
    }
    else {
        qDebug("CDirTreeProvider::GetAllAudioFileList() invalid volumeType(%d)!!!", volumeType);
        return false;
    }
}

bool CDirTreeProvider::GetAllVideoFileList(ETrackerAbstarctor_VolumeType  volumeType)
{
    qDebug("CDirTreeProvider::GetAllVideoFileList(%d)", volumeType);
    
    if(volumeType == eJukeBox) {
        return m_pJukebox->GetAllVideoFileList();
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetAllVideoFileList();
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetAllVideoFileList();
    }
    else {
        qDebug("CDirTreeProvider::GetAllVideoFileList() invalid volumeType(%d)!!!", volumeType);
        return false;
    }
}

bool CDirTreeProvider::GetAllPhotoFileList(ETrackerAbstarctor_VolumeType  volumeType)
{
    qDebug("CDirTreeProvider::GetAllPhotoFileList(%d)", volumeType);
    
    if(volumeType == eJukeBox) {
        return m_pJukebox->GetAllPhotoFileList();
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetAllPhotoFileList();
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetAllPhotoFileList();
    }
    else {
        qDebug("CDirTreeProvider::GetAllPhotoFileList() invalid volumeType(%d)!!!", volumeType);
        return false;
    }
}

void CDirTreeProvider::GetMediaEntryList(const QString &strSubDirPath, QList<QStringList> *entryList, ETrackerAbstarctor_VolumeType volumeType) // added by sungha.choi 2013.09.10
{
    if(volumeType == eDefaultVolume) {
        if(strSubDirPath.indexOf(m_pJukebox->GetMountPath()) == 0) {
            volumeType = eJukeBox;
        }
        else if(strSubDirPath.indexOf(m_pUSB1->GetMountPath()) == 0) {
            volumeType = eUSB_FRONT;
        }
        else if(strSubDirPath.indexOf(m_pUSB2->GetMountPath()) == 0) {
            volumeType = eUSB_REAR;
        }
        else {
            qDebug("CDirTreeProvider::GetAudioEntryList() invalid strSubDirPath(%s)!!!", qPrintable(strSubDirPath));
            return;
        }
    }

    qDebug("CDirTreeProvider::GetAudioEntryList(%d)", volumeType);

    if(volumeType == eJukeBox) {
        if(m_pJukebox) {
            m_pJukebox->GetMediaEntryList(strSubDirPath, entryList);
        }
    }
    else if(volumeType == eUSB_FRONT) {
        if(m_pUSB1) {
            m_pUSB1->GetMediaEntryList(strSubDirPath, entryList);
        }
    }
    else if(volumeType == eUSB_REAR) {
        if(m_pUSB2) {
            m_pUSB2->GetMediaEntryList(strSubDirPath, entryList);
        }
    }
    qDebug("CDirTreeProvider::GetAudioEntryList() invalid input");
}

void CDirTreeProvider::GetAudioEntryList(const QString &strSubDirPath, QList<QStringList> *entryList, ETrackerAbstarctor_VolumeType volumeType) // added by sungha.choi 2013.09.10
{
    if(volumeType == eDefaultVolume) {
        if(strSubDirPath.indexOf(m_pJukebox->GetMountPath()) == 0) {
            volumeType = eJukeBox;
        }
        else if(strSubDirPath.indexOf(m_pUSB1->GetMountPath()) == 0) {
            volumeType = eUSB_FRONT;
        }
        else if(strSubDirPath.indexOf(m_pUSB2->GetMountPath()) == 0) {
            volumeType = eUSB_REAR;
        }
        else {
            qDebug("CDirTreeProvider::GetAudioEntryList() invalid strSubDirPath(%s)!!!", qPrintable(strSubDirPath));
            return;
        }
    }

    qDebug("CDirTreeProvider::GetAudioEntryList(%d)", volumeType);

    if(volumeType == eJukeBox) {
        if(m_pJukebox) {
            m_pJukebox->GetAudioEntryList(strSubDirPath, entryList);
        }
    }
    else if(volumeType == eUSB_FRONT) {
        if(m_pUSB1) {
            m_pUSB1->GetAudioEntryList(strSubDirPath, entryList);
        }
    }
    else if(volumeType == eUSB_REAR) {
        if(m_pUSB2) {
            m_pUSB2->GetAudioEntryList(strSubDirPath, entryList);
        }
    }
    qDebug("CDirTreeProvider::GetAudioEntryList() invalid input");
}

void CDirTreeProvider::GetVideoEntryList(const QString &strSubDirPath, QList<QStringList> *entryList, ETrackerAbstarctor_VolumeType volumeType) // added by sungha.choi 2013.09.10
{
    if(volumeType == eDefaultVolume) {
        if(strSubDirPath.indexOf(m_pJukebox->GetMountPath()) == 0) {
            volumeType = eJukeBox;
        }
        else if(strSubDirPath.indexOf(m_pUSB1->GetMountPath()) == 0) {
            volumeType = eUSB_FRONT;
        }
        else if(strSubDirPath.indexOf(m_pUSB2->GetMountPath()) == 0) {
            volumeType = eUSB_REAR;
        }
        else {
            qDebug("CDirTreeProvider::GetVideoEntryList() invalid strSubDirPath(%s)!!!", qPrintable(strSubDirPath));
            return;
        }
    }

    qDebug("CDirTreeProvider::GetVideoEntryList(%d)", volumeType);

    if(volumeType == eJukeBox) {
        if(m_pJukebox) {
            m_pJukebox->GetVideoEntryList(strSubDirPath, entryList);
        }
    }
    else if(volumeType == eUSB_FRONT) {
        if(m_pUSB1) {
            m_pUSB1->GetVideoEntryList(strSubDirPath, entryList);
        }
    }
    else if(volumeType == eUSB_REAR) {
        if(m_pUSB2) {
            m_pUSB2->GetVideoEntryList(strSubDirPath, entryList);
        }
    }
    qDebug("CDirTreeProvider::GetVideoEntryList() invalid input");
}

void CDirTreeProvider::GetPictureEntryList(const QString &strSubDirPath, QList<QStringList> *entryList, ETrackerAbstarctor_VolumeType volumeType)  // added by sungha.choi 2013.09.10
{
    if(volumeType == eDefaultVolume) {
        if(strSubDirPath.indexOf(m_pJukebox->GetMountPath()) == 0) {
            volumeType = eJukeBox;
        }
        else if(strSubDirPath.indexOf(m_pUSB1->GetMountPath()) == 0) {
            volumeType = eUSB_FRONT;
        }
        else if(strSubDirPath.indexOf(m_pUSB2->GetMountPath()) == 0) {
            volumeType = eUSB_REAR;
        }
        else {
            qDebug("CDirTreeProvider::GetPictureEntryList() invalid strSubDirPath(%s)!!!", qPrintable(strSubDirPath));
            return;
        }
    }

    qDebug("CDirTreeProvider::GetPictureEntryList(%d)", volumeType);

    if(volumeType == eJukeBox) {
        if(m_pJukebox) {
            m_pJukebox->GetPictureEntryList(strSubDirPath, entryList);
        }
    }
    else if(volumeType == eUSB_FRONT) {
        if(m_pUSB1) {
            m_pUSB1->GetPictureEntryList(strSubDirPath, entryList);
        }
    }
    else if(volumeType == eUSB_REAR) {
        if(m_pUSB2) {
            m_pUSB2->GetPictureEntryList(strSubDirPath, entryList);
        }
    }
    qDebug("CDirTreeProvider::GetPictureEntryList() invalid input");
}

bool CDirTreeProvider::GetSubAudioFileList(const QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType, uint reqUID)
{
    if(volumeType == eDefaultVolume) {
        if(strSubDirPath.indexOf(m_pJukebox->GetMountPath()) == 0) {
            volumeType = eJukeBox;
        }
        else if(strSubDirPath.indexOf(m_pUSB1->GetMountPath()) == 0) {
            volumeType = eUSB_FRONT;
        }
        else if(strSubDirPath.indexOf(m_pUSB2->GetMountPath()) == 0) {
            volumeType = eUSB_REAR;
        }
        else {
            qDebug("CDirTreeProvider::GetSubAudioFileList() invalid strSubDirPath(%s)!!!", qPrintable(strSubDirPath));
            return false;
        }
    }

    qDebug("CDirTreeProvider::GetSubAudioFileList(%d)", volumeType);

    int nRet = 0;
    
    if(volumeType == eJukeBox) {
        if(m_pJukebox) {
            nRet = m_pJukebox->GetSubAudioFileList(strSubDirPath, reqUID);
        }
    }
    else if(volumeType == eUSB_FRONT) {
        if(m_pUSB1) {
            nRet = m_pUSB1->GetSubAudioFileList(strSubDirPath, reqUID);
        }
    }
    else if(volumeType == eUSB_REAR) {
        if(m_pUSB2) {
            nRet = m_pUSB2->GetSubAudioFileList(strSubDirPath, reqUID);
        }
    }
    else {
        qDebug("CDirTreeProvider::GetSubAudioFileList() invalid volumeType(%d)!!!", volumeType);
    }

    return nRet;
}

bool CDirTreeProvider::GetSubVideoFileList(const QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType)
{
    if(volumeType == eDefaultVolume) {
        if(strSubDirPath.indexOf(m_pJukebox->GetMountPath()) == 0) {
            volumeType = eJukeBox;
        }
        else if(strSubDirPath.indexOf(m_pUSB1->GetMountPath()) == 0) {
            volumeType = eUSB_FRONT;
        }
        else if(strSubDirPath.indexOf(m_pUSB2->GetMountPath()) == 0) {
            volumeType = eUSB_REAR;
        }
        else {
            qDebug("CDirTreeProvider::GetSubVideoFileList() invalid strSubDirPath(%s)!!!", qPrintable(strSubDirPath));
            return false;
        }
    }

    qDebug("CDirTreeProvider::GetSubVideoFileList(%d)", volumeType);
    
    if(volumeType == eJukeBox) {
        return m_pJukebox->GetSubVideoFileList(strSubDirPath);
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetSubVideoFileList(strSubDirPath);
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetSubVideoFileList(strSubDirPath);
    }
    else {
        qDebug("CDirTreeProvider::GetSubVideoFileList() invalid volumeType(%d)!!!", volumeType);
        return false;
    }
}

bool CDirTreeProvider::GetSubPhotoFileList(const QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType)
{
    if(volumeType == eDefaultVolume) {
        if(strSubDirPath.indexOf(m_pJukebox->GetMountPath()) == 0) {
            volumeType = eJukeBox;
        }
        else if(strSubDirPath.indexOf(m_pUSB1->GetMountPath()) == 0) {
            volumeType = eUSB_FRONT;
        }
        else if(strSubDirPath.indexOf(m_pUSB2->GetMountPath()) == 0) {
            volumeType = eUSB_REAR;
        }
        else {
            qDebug("CDirTreeProvider::GetSubPhotoFileList() invalid strSubDirPath(%s)!!!", qPrintable(strSubDirPath));
            return false;
        }
    }

    qDebug("CDirTreeProvider::GetSubPhotoFileList(%d)", volumeType);
    
    if(volumeType == eJukeBox) {
        return m_pJukebox->GetSubPhotoFileList(strSubDirPath);
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetSubPhotoFileList(strSubDirPath);
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetSubPhotoFileList(strSubDirPath);
    }
    else {
        qDebug("CDirTreeProvider::GetSubPhotoFileList() invalid volumeType(%d)!!!", volumeType);
        return false;
    }
}

bool CDirTreeProvider::GetVideoFileListWithSubFolder(const QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType)
{
    QString strModifiedPath;
    if(strSubDirPath.startsWith("file://"))
        strModifiedPath = strSubDirPath.mid(7);
    else
        strModifiedPath = strSubDirPath;
        
    if(volumeType == eDefaultVolume) {
        if(strModifiedPath.indexOf(m_pJukebox->GetMountPath()) == 0) {
            volumeType = eJukeBox;
        }
        else if(strModifiedPath.indexOf(m_pUSB1->GetMountPath()) == 0) {
            volumeType = eUSB_FRONT;
        }
        else if(strModifiedPath.indexOf(m_pUSB2->GetMountPath()) == 0) {
            volumeType = eUSB_REAR;
        }
        else {
            qDebug("CDirTreeProvider::GetVideoFileListWithSubFolder() invalid strSubDirPath(%s)!!!", qPrintable(strModifiedPath));
            return false;
        }
    }

    qDebug("CDirTreeProvider::GetVideoFileListWithSubFolder(%d)", volumeType);
    
    if(volumeType == eJukeBox) {
        return m_pJukebox->GetVideoFileListWithSubFolder(strModifiedPath);
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetVideoFileListWithSubFolder(strModifiedPath);
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetVideoFileListWithSubFolder(strModifiedPath);
    }
    else {
        qDebug("CDirTreeProvider::GetVideoFileListWithSubFolder() invalid volumeType(%d)!!!", volumeType);
        return false;
    }
}

bool CDirTreeProvider::GetPhotoFileListWithSubFolder(const QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType)
{
    QString strModifiedPath;
    if(strSubDirPath.startsWith("file://"))
        strModifiedPath = strSubDirPath.mid(7);
    else
        strModifiedPath = strSubDirPath;
        
    if(volumeType == eDefaultVolume) {
        if(strModifiedPath.indexOf(m_pJukebox->GetMountPath()) == 0) {
            volumeType = eJukeBox;
        }
        else if(strModifiedPath.indexOf(m_pUSB1->GetMountPath()) == 0) {
            volumeType = eUSB_FRONT;
        }
        else if(strModifiedPath.indexOf(m_pUSB2->GetMountPath()) == 0) {
            volumeType = eUSB_REAR;
        }
        else {
            qDebug("CDirTreeProvider::GetPhotoFileListWithSubFolder() invalid strSubDirPath(%s)!!!", qPrintable(strModifiedPath));
            return false;
        }
    }

    qDebug("CDirTreeProvider::GetPhotoFileListWithSubFolder(%d)", volumeType);
    
    if(volumeType == eJukeBox) {
        return m_pJukebox->GetPhotoFileListWithSubFolder(strModifiedPath);
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetPhotoFileListWithSubFolder(strModifiedPath);
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetPhotoFileListWithSubFolder(strModifiedPath);
    }
    else {
        qDebug("CDirTreeProvider::GetPhotoFileListWithSubFolder() invalid volumeType(%d)!!!", volumeType);
        return false;
    }
}

bool CDirTreeProvider::GetVideoFileListByFolder(const QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType)
{
    QString strModifiedPath;
    if(strSubDirPath.startsWith("file://"))
        strModifiedPath = strSubDirPath.mid(7);
    else
        strModifiedPath = strSubDirPath;
        
    if(volumeType == eDefaultVolume) {
        if(strModifiedPath.indexOf(m_pJukebox->GetMountPath()) == 0) {
            volumeType = eJukeBox;
        }
        else if(strModifiedPath.indexOf(m_pUSB1->GetMountPath()) == 0) {
            volumeType = eUSB_FRONT;
        }
        else if(strModifiedPath.indexOf(m_pUSB2->GetMountPath()) == 0) {
            volumeType = eUSB_REAR;
        }
        else {
            qDebug("CDirTreeProvider::GetVideoFileListByFolder() invalid strSubDirPath(%s)!!!", qPrintable(strModifiedPath));
            return false;
        }
    }

    qDebug("CDirTreeProvider::GetVideoFileListByFolder(%d)", volumeType);

    if(volumeType == eJukeBox) {
        return m_pJukebox->GetVideoFileListByFolder(strModifiedPath);   // modified by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetVideoFileListByFolder(strModifiedPath);   // modified by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetVideoFileListByFolder(strModifiedPath);   // modified by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    }
    else {
        qDebug("CDirTreeProvider::GetVideoFileListByFolder() invalid volumeType(%d)!!!", volumeType);
        return false;
    }
}

bool CDirTreeProvider::GetPhotoFileListByFolder(const QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType)
{
    QString strModifiedPath;
    if(strSubDirPath.startsWith("file://"))
        strModifiedPath = strSubDirPath.mid(7);
    else
        strModifiedPath = strSubDirPath;
        
    if(volumeType == eDefaultVolume) {
        if(strModifiedPath.indexOf(m_pJukebox->GetMountPath()) == 0) {
            volumeType = eJukeBox;
        }
        else if(strModifiedPath.indexOf(m_pUSB1->GetMountPath()) == 0) {
            volumeType = eUSB_FRONT;
        }
        else if(strModifiedPath.indexOf(m_pUSB2->GetMountPath()) == 0) {
            volumeType = eUSB_REAR;
        }
        else {
            qDebug("CDirTreeProvider::GetPhotoFileListByFolder() invalid strSubDirPath(%s)!!!", qPrintable(strModifiedPath));
            return false;
        }
    }

    qDebug("CDirTreeProvider::GetPhotoFileListByFolder(%d)", volumeType);

    if(volumeType == eJukeBox) {
        return m_pJukebox->GetPhotoFileListByFolder(strModifiedPath);   // modified by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetPhotoFileListByFolder(strModifiedPath);   // modified by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetPhotoFileListByFolder(strModifiedPath);   // modified by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    }
    else {
        qDebug("CDirTreeProvider::GetPhotoFileListByFolder() invalid volumeType(%d)!!!", volumeType);
        return false;
    }
}

// { added by kihyung 2013.07.23 for ITS 0178868
bool CDirTreeProvider::GetFirstImagePathByGivenFolder(const QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType)
{
    QString strModifiedPath;
    if(strSubDirPath.startsWith("file://"))
        strModifiedPath = strSubDirPath.mid(7);
    else
        strModifiedPath = strSubDirPath;
        
    if(volumeType == eDefaultVolume) {
        volumeType = GetVolumeType(strModifiedPath);
    }

    qDebug("CDirTreeProvider::GetFirstImagePathByGivenFolder(%d)", volumeType);

    if(volumeType == eJukeBox) {
        return m_pJukebox->GetFirstImagePathByGivenFolder(strModifiedPath);
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetFirstImagePathByGivenFolder(strModifiedPath);
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetFirstImagePathByGivenFolder(strModifiedPath);
    }
    else {
        qDebug("CDirTreeProvider::GetFirstImagePathByGivenFolder() invalid volumeType(%d)!!!", volumeType);
        return false;
    }
}

bool CDirTreeProvider::GetImageCountByGivenFolder(QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType)
{
    QString strModifiedPath;
    if(strSubDirPath.startsWith("file://"))
        strModifiedPath = strSubDirPath.mid(7);
    else
        strModifiedPath = strSubDirPath;
        
    if(volumeType == eDefaultVolume) {
        volumeType = GetVolumeType(strModifiedPath);
    }

    qDebug("CDirTreeProvider::GetImageCountByGivenFolder(%d)", volumeType);

    if(volumeType == eJukeBox) {
        return m_pJukebox->GetImageCountByGivenFolder(strModifiedPath);
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetImageCountByGivenFolder(strModifiedPath);
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetImageCountByGivenFolder(strModifiedPath);
    }
    else {
        qDebug("CDirTreeProvider::GetImageCountByGivenFolder() invalid volumeType(%d)!!!", volumeType);
        return false;
    }
}

bool CDirTreeProvider::GetFirstImageAndCountByGivenFolder(QString &strSubDirPath, ETrackerAbstarctor_VolumeType volumeType)
{
    QString strModifiedPath;
    if(strSubDirPath.startsWith("file://"))
        strModifiedPath = strSubDirPath.mid(7);
    else
        strModifiedPath = strSubDirPath;
        
    if(volumeType == eDefaultVolume) {
        volumeType = GetVolumeType(strModifiedPath);
    }

    qDebug("CDirTreeProvider::GetFirstImageAndCountByGivenFolder(%d)", volumeType);

    if(volumeType == eJukeBox) {
        return m_pJukebox->GetFirstImageAndCountByGivenFolder(strModifiedPath);
    }
    else if(volumeType == eUSB_FRONT) {
        return m_pUSB1->GetFirstImageAndCountByGivenFolder(strModifiedPath);
    }
    else if(volumeType == eUSB_REAR) {
        return m_pUSB2->GetFirstImageAndCountByGivenFolder(strModifiedPath);
    }
    else {
        qDebug("CDirTreeProvider::GetFirstImageAndCountByGivenFolder() invalid volumeType(%d)!!!", volumeType);
        return false;
    }
}
// } added by kihyung 2013.07.23 for ITS 0178868

void CDirTreeProvider::OnScanCompleted(ETrackerAbstarctor_VolumeType volumeType, int bRescanMode, int bSuccess)
{
    qDebug("CDirTreeProvider::OnScanCompleted(%d, %d, %d)", volumeType, bRescanMode, bSuccess);
    emit ScanCompleted(volumeType, bRescanMode, bSuccess);
}

void CDirTreeProvider::OnReadCompleted(ETrackerAbstarctor_VolumeType volumeType, int bSuccess)
{
    qDebug("CDirTreeProvider::OnReadCompleted(%d, %d)", volumeType, bSuccess);    
    emit ReadCompleted(volumeType, bSuccess);
}

void CDirTreeProvider::OnMediaFileFound(ETrackerAbstarctor_VolumeType volume, int bRescanMode, int audio, int video, int photo)
{
    qDebug("CDirTreeProvider::OnMediaFileFound(%d, %d, %d, %d, %d)", volume, bRescanMode, audio, video, photo);
    emit MediaFileFound(volume, bRescanMode, audio, video, photo);
}

CDirTreeProvider::CDirTreeProvider(QObject *parent) : QObject(parent)
{
    LOG_INIT_V2("Media_TrackerAbstractor");

    m_pJukebox = new CDirTreeThread(eJukeBox);
    m_pUSB1    = new CDirTreeThread(eUSB_FRONT);
    m_pUSB2    = new CDirTreeThread(eUSB_REAR);

    connect(m_pJukebox, SIGNAL(DataDirectFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,uint)),this,SIGNAL(DataDirectFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,uint)));
    connect(m_pJukebox, SIGNAL(DataDirectFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,ETrackerAbstarctor_VolumeType,uint)),this,SIGNAL(DataDirectFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,ETrackerAbstarctor_VolumeType,uint)));
    connect(m_pJukebox, SIGNAL(DataFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)), this, SIGNAL(DataFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)));
    connect(m_pJukebox, SIGNAL(Error(int)), this, SIGNAL(Error(int)));
    connect(m_pJukebox, SIGNAL(ScanCompleted(ETrackerAbstarctor_VolumeType, int, int)), this, SLOT(OnScanCompleted(ETrackerAbstarctor_VolumeType, int, int)));
    connect(m_pJukebox, SIGNAL(ReadCompleted(ETrackerAbstarctor_VolumeType, int)), this, SLOT(OnReadCompleted(ETrackerAbstarctor_VolumeType, int)), Qt::QueuedConnection);   // modified by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    connect(m_pJukebox, SIGNAL(MediaFileFound(ETrackerAbstarctor_VolumeType,int,int,int,int)), this, SLOT(OnMediaFileFound(ETrackerAbstarctor_VolumeType,int,int,int,int)), Qt::DirectConnection);
    m_pJukebox->start();

    connect(m_pUSB1, SIGNAL(DataDirectFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,uint)),this,SIGNAL(DataDirectFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,uint)));
    connect(m_pUSB1, SIGNAL(DataDirectFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,ETrackerAbstarctor_VolumeType,uint)),this,SIGNAL(DataDirectFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,ETrackerAbstarctor_VolumeType,uint)));
    connect(m_pUSB1, SIGNAL(DataFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)), this, SIGNAL(DataFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)));
    connect(m_pUSB1, SIGNAL(Error(int)), this, SIGNAL(Error(int)));
    connect(m_pUSB1, SIGNAL(ScanCompleted(ETrackerAbstarctor_VolumeType, int, int)), this, SLOT(OnScanCompleted(ETrackerAbstarctor_VolumeType, int, int)));
    connect(m_pUSB1, SIGNAL(ReadCompleted(ETrackerAbstarctor_VolumeType, int)), this, SLOT(OnReadCompleted(ETrackerAbstarctor_VolumeType, int)), Qt::QueuedConnection);   // modified by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    connect(m_pUSB1, SIGNAL(MediaFileFound(ETrackerAbstarctor_VolumeType,int,int,int,int)), this, SLOT(OnMediaFileFound(ETrackerAbstarctor_VolumeType,int,int,int,int)), Qt::DirectConnection);
    m_pUSB1->start();
    
    connect(m_pUSB2, SIGNAL(DataDirectFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,uint)),this,SIGNAL(DataDirectFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,uint)));
    connect(m_pUSB2, SIGNAL(DataDirectFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,ETrackerAbstarctor_VolumeType,uint)),this,SIGNAL(DataDirectFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,ETrackerAbstarctor_VolumeType,uint)));
    connect(m_pUSB2, SIGNAL(DataFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)), this, SIGNAL(DataFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)));
    connect(m_pUSB2, SIGNAL(Error(int)), this, SIGNAL(Error(int)));
    connect(m_pUSB2, SIGNAL(ScanCompleted(ETrackerAbstarctor_VolumeType, int, int)), this, SLOT(OnScanCompleted(ETrackerAbstarctor_VolumeType, int, int)));
    connect(m_pUSB2, SIGNAL(ReadCompleted(ETrackerAbstarctor_VolumeType, int)), this, SLOT(OnReadCompleted(ETrackerAbstarctor_VolumeType, int)), Qt::QueuedConnection);   // modified by sungha.choi 2013.09.09 for ITS 0188266, ISV 89535
    connect(m_pUSB2, SIGNAL(MediaFileFound(ETrackerAbstarctor_VolumeType,int,int,int,int)), this, SLOT(OnMediaFileFound(ETrackerAbstarctor_VolumeType,int,int,int,int)), Qt::DirectConnection);    
    m_pUSB2->start();
}

ETrackerAbstarctor_VolumeType CDirTreeProvider::GetVolumeType(const QString &strSubDirPath)
{
    ETrackerAbstarctor_VolumeType volumeType = eDefaultVolume;
    
    if(strSubDirPath.indexOf(m_pJukebox->GetMountPath()) == 0) {
        volumeType = eJukeBox;
    }
    else if(strSubDirPath.indexOf(m_pUSB1->GetMountPath()) == 0) {
        volumeType = eUSB_FRONT;
    }
    else if(strSubDirPath.indexOf(m_pUSB2->GetMountPath()) == 0) {
        volumeType = eUSB_REAR;
    }
    else {
        qDebug("CDirTreeProvider::eDefaultVolume() invalid strSubDirPath(%s)!!!", qPrintable(strSubDirPath));
    }

    return volumeType;
}

bool CDirTreeProvider::FileExists(QString fullPath, ETrackerAbstarctor_VolumeType volumeType)   // { added by sungha.choi 2013.07.30 for scandir performance
{
    bool ret = false;
    if(volumeType == eDefaultVolume) {
        volumeType = GetVolumeType(fullPath);
    }
    switch(volumeType)
    {
        case eJukeBox:
            ret = m_pJukebox->FileExists(fullPath);
            break;
        case eUSB_FRONT:
            ret = m_pUSB1->FileExists(fullPath);
            break;
        case eUSB_REAR:
            ret = m_pUSB2->FileExists(fullPath);
            break;
        default:
            break;
    }
    return ret;
}   // } added by sungha.choi 2013.07.30 for scandir performance

// { added by kihyung for ITS 0181967 
bool CDirTreeProvider::GetFSFolderInfo(QList<FSFolderInfo> &tFolderList, ETrackerAbstarctor_VolumeType volumeType, ETrackerAbstarctor_MediaType mediaType)
{
    switch(volumeType)
    {
        case eJukeBox:
            return m_pJukebox->GetFSFolderInfo(tFolderList, mediaType);
        case eUSB_FRONT:
            return m_pUSB1->GetFSFolderInfo(tFolderList, mediaType);
        case eUSB_REAR:
            return m_pUSB2->GetFSFolderInfo(tFolderList, mediaType);
        default:
            return false;
    }
}
// } added by kihyung for ITS 0181967

// { added by kihyung 2013.10.02 for factory reset
void CDirTreeProvider::RemoveScanResultFile(ETrackerAbstarctor_VolumeType volumeType)
{
    QString strFileName;
    switch(volumeType)
    {
        case eJukeBox:
            strFileName = gScanResult_JukeBox_path;
            break;
        case eUSB_FRONT:
            strFileName = gScanResult_USB1_path;
            break;
        case eUSB_REAR:
            strFileName = gScanResult_USB2_path;
            break;
        default:
            return;
    }

    if(QFile::remove(strFileName) == true) {
        LOG_CRITICAL << strFileName << "is deleted" << LOG_ENDL;
    }
}
// } added by kihyung 2013.10.02 for factory reset
