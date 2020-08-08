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
* MODULE SUMMARY : This file defines Apis which interact with    *
*                  Tracker database using spaql query to retrieve media related  *
*                  information (e.g Artist, Album, Genre etc)    *
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

#include "DHAVN_DirListProvider.h"
#include <QDirIterator>
#include <QDebug>
#include <QMutex>
#include <QUrl>


// it can be modified as per the need , here right now i have put down 
// only value of 10.

#define MAX_LIST_COUNTER	10
#define MUSICDIRLIST	1
#define VIDEODIRLIST	2
#define	PHOTODIRLIST	3
#define MUSICLIMITDIRLIST	4
#define	VIDEOLIMITDIRLIST	5
#define PHOTOLIMITDIRLIST	6
#define MUSICFOLDERTABLIST  7

static QString modeToString(int mode)
{
    switch (mode) {
    case MUSICDIRLIST:
        return "MUSICDIRLIST";
    case VIDEODIRLIST:
        return "VIDEODIRLIST";
    case PHOTODIRLIST:
        return "PHOTODIRLIST";
    case MUSICLIMITDIRLIST:
        return "MUSICLIMITDIRLIST";
    case VIDEOLIMITDIRLIST:
        return "VIDEOLIMITDIRLIST";
    case PHOTOLIMITDIRLIST:
        return "PHOTOLIMITDIRLIST";
    case MUSICFOLDERTABLIST:
        return "MUSICFOLDERTABLIST";
    default:
        return "";
    }
}

const QString gVolumePath_JukeBox_Path = "/usr_data/media";
const QString gVolumePath_USB_Path = "/media";



CDirListProvider::CDirListProvider()
{
    LOG_INIT_V2("Media_TrackerAbstractor");

    qRegisterMetaType<ETrackerAbstarctor_Query_Ids>("ETrackerAbstarctor_Query_Ids");
    qRegisterMetaType<QVector<QStringList> >();

    /* Dummy initilization of QStringlist*/
    for(int i=0;i<=EVOLUME_TYPE_SIZE;i++)
    {
        m_DeviceId << "Empty" ;
        m_DeviceURL << "Empty";
    }

    for(int i=0;i<FOLDER_KEY_LIST_SIZE;i++)
    {
        m_ppFolderList[i] = &m_pFolderList[i];
    }

	m_mode = 0; // added by eugene.seo 2013.03.27 for static test
}

CDirListProvider::~CDirListProvider()
{

}

// { added by kihyung 2013.2.8
bool CDirListProvider::GetFolderTabList(QString folderPath)
{
    LOG_TRACE << Q_FUNC_INFO << " path: " << folderPath;
    terminate();
    wait();
    if(folderPath.isNull())
	return false;
	SetPath(folderPath);
	SetCreateListMode(MUSICFOLDERTABLIST);
    start();
	qDebug() << " after calling start";
	return true;
}
// } added by kihyung 2013.2.8

bool CDirListProvider::GetMusicListByDirList(QString path)
{
    LOG_TRACE << Q_FUNC_INFO << " path: " << path;
    terminate();
    wait();
    if(path.isNull())
	return false;
	SetPath(path);
	SetCreateListMode(MUSICDIRLIST);
    start();
	qDebug() << " after calling start";
	return true;
}

bool CDirListProvider::GetMusicListByDirList(ETrackerAbstarctor_VolumeType  volumeType)
{
	switch(volumeType)
	{
		case eUSB:
                case eUSB_FRONT:
                case eUSB_REAR:
            qDebug() << "file path " << m_DeviceURL.at(volumeType) << "volumeType" << volumeType;
                        return GetMusicListByDirList(m_DeviceURL.at(volumeType));
		break;
		case eJukeBox:
                        return GetMusicListByDirList(m_DeviceURL.at(volumeType));
		break;
		default:
			return false;
		break;
	}
}

bool CDirListProvider::GetVideoListByDirList(QString path)
{
    LOG_TRACE << Q_FUNC_INFO << " path: " << path;
    terminate();
    wait();
    if(path.isNull())
	return false;
	SetPath(path);
	SetCreateListMode(VIDEODIRLIST);
    start();
	return true;
}

bool CDirListProvider::GetVideoListByDirList(ETrackerAbstarctor_VolumeType  volumeType)
{
	switch(volumeType)
	{
		case eUSB:
                case eUSB_FRONT:
                case eUSB_REAR:
                        return GetVideoListByDirList(m_DeviceURL.at(volumeType));
		break;
		case eJukeBox:
                        return GetVideoListByDirList(m_DeviceURL.at(volumeType));
		break;
		default:
			return false;
		break;
	}
}
	
bool CDirListProvider::GetPhotoListByDirList(QString path)
{
    LOG_TRACE << Q_FUNC_INFO << " path: " << path;
    terminate();
    wait();
    if(path.isNull())
	return false;
	SetPath(path);
	SetCreateListMode(PHOTODIRLIST);
    start();
	return true;
}

bool CDirListProvider::GetPhotoListByDirList(ETrackerAbstarctor_VolumeType  volumeType)
{
	switch(volumeType)
	{
		case eUSB:
                case eUSB_FRONT:
                case eUSB_REAR:
                        return GetPhotoListByDirList(m_DeviceURL.at(volumeType));
		break;
		case eJukeBox:
                        return GetPhotoListByDirList(m_DeviceURL.at(volumeType));
		break;
		default:
			return false;
		break;
	}
}

bool CDirListProvider::GetLimitedMusicListByDirList(QString path)
{
    LOG_TRACE << Q_FUNC_INFO << " path: " << path;
    terminate();
    wait();
    if(path.isNull())
	return false;
	SetPath(path);
	SetCreateListMode(MUSICLIMITDIRLIST);
    start();
	return true;
}

bool CDirListProvider::GetLimitedMusicListByDirList(ETrackerAbstarctor_VolumeType  volumeType)
{
	switch(volumeType)
	{
		case eUSB:
                case eUSB_FRONT:
                case eUSB_REAR:
                        return GetLimitedMusicListByDirList(m_DeviceURL.at(volumeType));
		break;
		case eJukeBox:
                        return GetLimitedMusicListByDirList(m_DeviceURL.at(volumeType));
		break;
		default:
			return false;
		break;
	}
}

bool CDirListProvider::GetLimitedVideoListByDirList(QString path)
{
    LOG_TRACE << Q_FUNC_INFO << " path: " << path;
    terminate();
    wait();
    if(path.isNull())
	return false;
	SetPath(path);
	SetCreateListMode(VIDEOLIMITDIRLIST);
    start();
	return true;
}

bool CDirListProvider::GetLimitedVideoListByDirList(ETrackerAbstarctor_VolumeType  volumeType)
{
	switch(volumeType)
	{
		case eUSB:
                case eUSB_FRONT:
                case eUSB_REAR:
                        return GetLimitedVideoListByDirList(m_DeviceURL.at(volumeType));
		break;
		case eJukeBox:
                        return GetLimitedVideoListByDirList(m_DeviceURL.at(volumeType));
		break;
		default:
			return false;
		break;
	}
}

bool CDirListProvider::GetLimitedPhotoListByDirList(QString path)
{
    LOG_TRACE << Q_FUNC_INFO << " path: " << path;
    terminate();
    wait();
    if(path.isNull())
	return false;
	SetPath(path);
	SetCreateListMode(PHOTOLIMITDIRLIST);
    start();
	return true;
}

bool CDirListProvider::GetLimitedPhotoListByDirList(ETrackerAbstarctor_VolumeType  volumeType)
{
	switch(volumeType)
	{
		case eUSB:
                case eUSB_FRONT:
                case eUSB_REAR:
                        return GetLimitedPhotoListByDirList(m_DeviceURL.at(volumeType));
		break;
		case eJukeBox:
                        return GetLimitedPhotoListByDirList(m_DeviceURL.at(volumeType));
		break;
		default:
			return false;
		break;
	}
}

void CDirListProvider::run()
{
    LOG_TRACE << Q_FUNC_INFO;
    if(m_mode == MUSICFOLDERTABLIST) {
        CreateFolderList(m_path);
    }
    else {
        CreateFileList(m_path, m_mode);
    }
}

void CDirListProvider::CreateFileList(QString path, int mode)
{
	QMutex mutex;
	mutex.lock();
	
        qDebug()<<"inside the Find attached devices " << path;
    QDir dir(path);
	if(!dir.exists())
	{
		emit Error(eErrorDirPathNotExists);
		return;
	}
    qDebug()<<"inside the Find attached devices111";
    QStringList listformatmedia;
	listformatmedia.clear();
	switch(mode)
	{
		case MUSICDIRLIST:
		case MUSICLIMITDIRLIST:	
			listformatmedia <<"*.asf"<<"*.m4a"<<"*.mp3"<<"*.oga"<<"*.ogg"<<"*.wav"<<"*.wma"
				            <<"*.ASF"<<"*.M4A"<<"*.MP3"<<"*.OGA"<<"*.OGG"<<"*.WAV"<<"*.WMA"; // modified by eugene.seo 2013.04.17
		break;
		case VIDEODIRLIST:
		case VIDEOLIMITDIRLIST:
			listformatmedia <<"*.asf"<<"*.avi"<<"*.divx"<<"*.m2ts"<<"*.m4v"<<"*.mkv"<<"*.mp4"<<"*.mpg"<<"*.mpeg"<<"*.ps"<<"*.tp"<<"*.ts"<<"*.vod"<<"*.wmv" 
                            <<"*.ASF"<<"*.AVI"<<"*.DIVX"<<"*.M2TS"<<"*.M4V"<<"*.MKV"<<"*.MP4"<<"*.MPG"<<"*.MPEG"<<"*.PS"<<"*.TP"<<"*.TS"<<"*.VOD"<<"*.WMV"; // modified by eugene.seo 2013.04.17
		break;
		case PHOTODIRLIST:
		case PHOTOLIMITDIRLIST:
			listformatmedia <<"*.bmp"<<"*.gif"<<"*.jpg"<<"*.jpeg"<<"*.png"<<"*.tif"<<"*.tiff"
				            <<"*.BMP"<<"*.GIF"<<"*.JPG"<<"*.JPEG"<<"*.PNG"<<"*.TIF"<<"*.TIFF"; // modified by eugene.seo 2013.04.17
		break;
	}
	                     

    dir.setNameFilters(listformatmedia);



   //Append playlist with all files in
    QString curFileToAdd;

    QDirIterator qdi(dir.absolutePath(), listformatmedia,  QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories );
    QStringList slist;
	slist.clear();
    if(mode == MUSICDIRLIST || mode == VIDEODIRLIST || mode == PHOTODIRLIST)
    {
	    while (qdi.hasNext())
	    {
		  curFileToAdd = qdi.next();
		  slist.append(curFileToAdd);

	    }
    }
	else
	{
		while (qdi.hasNext())
	    {
			curFileToAdd = qdi.next();
			slist.append(curFileToAdd);
			if(slist.size() >= MAX_LIST_COUNTER)
			break;
	    }
	}

   m_veclist queryDataList;
   queryDataList.clear();
   if(slist.size() > 0)
   queryDataList.append(slist);

	switch(mode)
	{
		case MUSICDIRLIST:
            //qDebug() << "result" << queryDataList;
			emit DataFetched(queryDataList, eGetMusicFileListDir_Id);
		break;
		case VIDEODIRLIST:
			emit DataFetched(queryDataList, eGetVideoFileListDir_Id);
		break;
		case PHOTODIRLIST:
			emit DataFetched(queryDataList, eGetPhotoFileListDir_Id);
		break;
		case MUSICLIMITDIRLIST:
			emit DataFetched(queryDataList, eGetMusicLimitFileListDir_Id);
		break;
		case VIDEOLIMITDIRLIST:
			emit DataFetched(queryDataList, eGetVideoLimitFileListDir_Id);
		break;
		case PHOTOLIMITDIRLIST:
			emit DataFetched(queryDataList, eGetPhotoLimitFileListDir_Id);
		break;
	}
	mutex.unlock();
}

void CDirListProvider::CreateFolderList(QString folderPath)
{
    QMutex mutex;
	mutex.lock();

    qDebug()<<"CDirListProvider::CreateFolderList()" << folderPath;

    int bUSBPath = false;
    if(folderPath.indexOf(gVolumePath_USB_Path) == 0) {
        bUSBPath = true;
    }

    int nPrevKeyResIdx = m_FolderKeyList.indexOf(folderPath);
    
    if(nPrevKeyResIdx >= 0 && bUSBPath == true) 
    {
        emit DataFetched(*m_ppFolderList[nPrevKeyResIdx], eGetFolderTabList_Id);
    }
    else 
    {
        if(m_FolderKeyList.size() >= FOLDER_KEY_LIST_SIZE && bUSBPath == true) {
            m_FolderKeyList.removeFirst();
            QVector<QStringList> *pTemp = m_ppFolderList[0];
            for(int i = 1; i < FOLDER_KEY_LIST_SIZE; i++)
                m_ppFolderList[i-1] = m_ppFolderList[i];
            m_ppFolderList[FOLDER_KEY_LIST_SIZE-1] = pTemp;
        }
        
        const QStringList SUPPORTED_MIME_TYPE (QStringList()<<"*.m4a"<<"*.mp3"<<"*.oga"<<"*.ogg"<<"*.wav"<<"*.wma"); // modified by eugene.seo 2013.04.17
        const QDir::Filters FILE_FILTERS = QDir::Files | QDir::System | QDir::Hidden | QDir::NoDot | QDir::NoDotDot;
        QString curItemPath;
        QStringList itemNameList;
        QStringList itemPathList;
        QStringList folderFlag;
        QStringList emptyFolderFlag;

        QStringList folderList;
        folderList = QDir(folderPath).entryList(QStringList("*"), QDir::Dirs | QDir::NoDot | QDir::NoDotDot );
        itemNameList = folderList;

        foreach ( QString folderName, folderList )
        {
            itemPathList << folderPath + "/" + folderName;
            folderFlag << "true";
            QStringList fileListCheck;
            fileListCheck = QDir(folderPath + "/" + folderName).entryList(SUPPORTED_MIME_TYPE, FILE_FILTERS );
            if(fileListCheck.count() > 0)
    		{
                emptyFolderFlag << "false";
    		}
            else
    		{
                QString itemPath;
                itemPath = folderPath + "/" + folderName;
                QDirIterator dirIterator(itemPath, SUPPORTED_MIME_TYPE, FILE_FILTERS, QDirIterator::Subdirectories);
                bool flag = false;
        	    while(dirIterator.hasNext())
    			{
    	             flag = true;
                     emptyFolderFlag << "false";
                     break;
    			}
                
    			if(flag == false)
                	emptyFolderFlag << "true";
            }
        }
        QStringList fileList;
        fileList = QDir(folderPath).entryList(SUPPORTED_MIME_TYPE, FILE_FILTERS );
        foreach ( QString fileName, fileList )
        {
            curItemPath = folderPath + "/" + fileName;
            itemNameList << fileName;
            itemPathList << curItemPath;
            folderFlag << "false";
            emptyFolderFlag << "false";
        }

        QVector<QStringList> dataList;
        QStringList resultData;
        for(int i=0; i<itemPathList.count(); i++)
        {
            resultData.clear();
            resultData.append(itemPathList[i]);
            resultData.append(folderFlag[i]);
            resultData.append(emptyFolderFlag[i]);

            dataList.append(resultData);
        }

        if(itemPathList.count() > 0 && bUSBPath == true) {
            nPrevKeyResIdx = m_FolderKeyList.size();
            if(nPrevKeyResIdx < FOLDER_KEY_LIST_SIZE) 
            {
                m_FolderKeyList.append(folderPath);
                *m_ppFolderList[nPrevKeyResIdx] = dataList;
            }
        }

        emit DataFetched(dataList, eGetFolderTabList_Id);
    }
    mutex.unlock();
}

void CDirListProvider::SetPath(QString str)
{
	m_path = str;
}

void CDirListProvider::SetCreateListMode(int mode)
{
	m_mode = mode;
}

int CDirListProvider::GetCreateListMode()
{
	return m_mode;
}
	

/******************************************************************************

// Function Name                                                Parameters

// CMediaDataProvider::setDeviceURL(ETrackerAbstarctor_VolumeType volumeType,QString DeviceId,QString MountPath)
//

// Explanation

//This method set the URL of the mounted mounted Device based on the volumetype

//

// History

// 27-Mar-2012     mahesh kumar    set the URL of the mounted mounted Device based on the volumetype
*****************************************************************************/

bool CDirListProvider::setDeviceURL(ETrackerAbstarctor_VolumeType volumeType,QString DeviceId,QString MountPath)
{
    bool ret = false;
    //QSparqlQuery volumeQueryObject;
    //QSparqlResult* result;
    QString deviceUrl;

   qDebug() << "CMediaDataProvider::setDeviceURL" << volumeType << DeviceId << MountPath;
    m_DeviceId.removeAt(volumeType);
    m_DeviceId.insert(volumeType,DeviceId);

    // { added by kihyung 2013.2.8
    for(int i = 0; i < FOLDER_KEY_LIST_SIZE;i++) m_pFolderList[i].clear();
    m_FolderKeyList.clear();
    // } added by kihyung 2013.2.8
    
    switch(volumeType)
    {
    case eUSB:
    case eUSB_FRONT:
    case eUSB_REAR:
        {
        m_DeviceURL.removeAt(volumeType);
        //deviceUrl.append(FilePath_Prefix);
        deviceUrl.append(MountPath);
          //qDebug() << "CMediaDataProvider::setDeviceURL USB" << volumeType << DeviceId << deviceUrl;

        deviceUrl = QUrl::toPercentEncoding(deviceUrl, ":/()~!@$&_-',.=+");
        if(deviceUrl.contains("+"))
            deviceUrl.replace(gPlusSign, gPlusSignCorrected);
        if(deviceUrl.contains("("))
            deviceUrl.replace(gLBracketSign, gLBracketSignCorrected);
        if(deviceUrl.contains(")"))
            deviceUrl.replace(gRBracketSign, gRBracketSignCorrected);
        if(deviceUrl.contains("$"))
            deviceUrl.replace(gDollarSign, gDollarSignCorrected);

        m_DeviceURL.insert(volumeType,deviceUrl);

        ret = true;
          }
        break;

    case eJukeBox:
        m_DeviceURL.removeAt(volumeType);
        //deviceUrl.append(FilePath_Prefix);
        deviceUrl.append(MountPath);
        m_DeviceURL.insert(volumeType,deviceUrl);
        //qDebug() << "CMediaDataProvider::setDeviceURL JukeBox" << volumeType << DeviceId << deviceUrl;
        ret = true;
        break;


default:
        emit Error(eErrorDeviceId);
        ret = false;
        break;

    }
    return ret;

}



