
/*
******************************************************************
*        COPYRIGHT (C) Teleca AB      *
*----------------------------------------------------------------*
* MODULE     :  DHAVN_CCoverArtDataProvider                            *
*
* PROGRAMMER :  Mahesh kumar                                       *
* DATE       :  03 October 2011                                    *
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
* 1.0       03 OCtober 2011  mahesh Kumar           Draft version
******************************************************************
*/

#include "DHAVN_CoverArtDataProvider.h"
#include <QSparqlResult>
#include <QSparqlQuery>
#include <QSparqlConnection>
#include <QSparqlError>
//#include <thumbnailer/MediaArtRequester>
#include <MediaArtInfo>
#include<Thumbnailer>
#include <QFile>
#include <QDir>
#include <QtCore/QCryptographicHash>
#include <QtDBus/QDBusInterface>
#include <QDebug>
#include <QtDBus/QDBusMessage>
#include <QTime>

//Gstreamer code not in use
#if 0
#include <gst/gst.h>


#ifndef QT_NO_KEYWORDS
#undef signals
#endif

#include <gtk/gtk.h>

#ifndef QT_NO_KEYWORDS
#ifdef QOBJECTDEFS_H
#define signals protected
#endif
#endif


#define THUMBNAIL_IMAGE_CAPS "video/x-raw-rgb,width=160,pixel-aspect-ratio=1/1,bpp=(int)24,depth=(int)24,endianness=(int)4321,red_mask=(int)0xff0000, green_mask=(int)0x00ff00, blue_mask=(int)0x0000ff"

#endif

CCoverArtDataprovider::CCoverArtDataprovider() : m_ThumbNailer(NULL),
    m_Connection(QDBusConnection::connectToBus(QDBusConnection::SessionBus, "org.lge.GracenoteCDLookupDBus"))
{
    LOG_INIT_V2("Media_TrackerAbstractor");

    /*m_QueryResult = NULL;*/
    gthumbnail_flag = eInvalid;
    m_IfIPODRequest = false;
    m_IsIPODRequestComplete = false;

    if(!m_Connection.isConnected()) {
        LOG_CRITICAL << Q_FUNC_INFO << "Interface to Gracenote CD interface is not available " << m_Connection.lastError().message() << LOG_ENDL;
    }
}

CCoverArtDataprovider::~CCoverArtDataprovider()
{
    /* if(m_QueryResult != NULL)
        delete m_QueryResult; */
    if(m_ThumbNailer != NULL)
    {
        delete m_ThumbNailer;
        m_ThumbNailer = NULL;
    }
}

bool CCoverArtDataprovider::IsCoverArtExists(const QString &albumName, const QString &artistname)
{
    //{changed by junam 2013.05.16 for coverart match
    //MediaArt::Info AlbumArt = MediaArt::Album(artistname, albumName);
    QString iPodKey = getIPodUID(artistname);
    MediaArt::Info AlbumArt = MediaArt::Album(iPodKey.isEmpty() ? artistname : iPodKey,  albumName);
    //}changed by junam

    // QString CoverUrl = AlbumArt.potentialPath().toString();
    if(!AlbumArt.exists())
        return false;

    return true;
}


bool CCoverArtDataprovider::GetMusicAlbumCoverArt(const QStringList &albumName ,const QStringList &artistname){

    //QList<MediaArt::Album> AlbumArtList;
    QVector <QStringList> queryDataList;

    ETrackerAbstarctor_Query_Ids queryId = eGetMusicAlbumCoverArt_Id;


    for(int i=0;i < albumName.count();i++)
    {
        // Crash Fix for No Artist case
        if(artistname.count() <= i)
        {
            QStringList emptyDataList;
            QString str;
            str.append(albumName[i]);
            str.append(" - Error - No artist found");
            emptyDataList.append(str);
            queryDataList.append(emptyDataList);
        }
        else
        {
            if( albumName[i].isEmpty() || artistname[i].isEmpty())
            {
                QStringList emptyDataList;
                QString str;
                str.append("No CoverArt Image Found - album/artist name empty");
                emptyDataList.append(str);
                queryDataList.append(emptyDataList);
                LOG_TRACE << Q_FUNC_INFO << " [TA]:Empty Album Name :" << albumName[i] << "  OR  Artist Name :"<< artistname[i] << LOG_ENDL;
            }
            else
            {
                //{changed by junam 2013.05.16 for coverart match
                //MediaArt::Album mediaArt = MediaArt::Album(artistname[i], albumName[i]);
                QString iPodKey = getIPodUID(artistname[i]);
                MediaArt::Album mediaArt = MediaArt::Album( iPodKey.isEmpty() ? artistname[i] : iPodKey, albumName[i]);
                //}changed by junam
                bool status = mediaArt.exists();

                if(!status){
                    LOG_TRACE << "No CoverArt Image Found" << LOG_ENDL;
                    QString ErrorMsg = "No CoverArt Image Found";
                    QStringList ErrorMsgList;
                    ErrorMsgList.append(ErrorMsg);
                    queryDataList.append(ErrorMsgList);

                }
                else{
                    QString CoverArtUrl = mediaArt.potentialPath().toString();
                    QStringList CoverArtUrlList;
                    CoverArtUrlList.append(CoverArtUrl);
                    queryDataList.append(CoverArtUrlList);
                }

            }
        }
    }

    emit DataFetched(queryDataList, queryId);

    return true;

}


//bool CCoverArtDataprovider::GetMusicTrackCoverArt(const QStringList &albumNameList , const QStringList &artistnameList, const QStringList &tracks)
//{
//    QVector <QStringList> queryDataList;
//    QStringList trackList;
//    QStringList trackCoverArtURLList;
//    ETrackerAbstarctor_Query_Ids queryId = eGetMusicTrackCoverArt_Id;
//    for(int i=0; i<tracks.count(); i++)
//    {
//        trackList.append(tracks[i]);
//        if(albumNameList.count() < i || artistnameList.count() < i)
//        {
//            trackCoverArtURLList.append(tracks[i]);
//            trackCoverArtURLList.append(" - No Artist/Album found");
//        }
//        else
//        {
//            MediaArt::Info trackArtInfo = MediaArt::Track(artistnameList[i], albumNameList[i], tracks[i]);
//            if(trackArtInfo.exists())
//            {
//                qDebug() << "url is : " <<trackArtInfo.potentialPath().toString();
//                trackCoverArtURLList.append(trackArtInfo.potentialPath().toString());
//            }
//            else
//            {
//                qDebug() << "url is : " <<trackArtInfo.potentialPath().toString();
//                trackCoverArtURLList.append("CoverArt Not found");
//                qDebug() << " - CoverArt Not found";
//            }
//        }
//    }
//    for(int j=0; j<tracks.count(); j++)
//    {
//        qDebug() << "track - " <<trackList[j] <<"url -" <<trackCoverArtURLList[j];
//    }
//    queryDataList.append(trackList);
//    queryDataList.append(trackCoverArtURLList);
//    emit DataFetched(queryDataList, queryId);
//    return true;
//}

bool CCoverArtDataprovider::RequestVideoThumbnail(const QString &filePath){

    bool ret = false;
    if(filePath.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }

    QString VideoFileUrl;
    VideoFileUrl = filePath;
    m_ThumbnailerInfo.clear();

    //  gthumbnail_flag = eVideo; // to handle it in slot //commented as video thumbnail logic is changed.
    QString VideoTitle;
    QStringList ThumbInfo;
    QString FilePathFromSlash; // Modified on 2013.07.09 for ITS # 0178912
    QFileInfo FileInfo(filePath);

    // Modified on 2013.07.09 for ITS # 0178912
    VideoTitle.append(FileInfo.baseName());
    VideoTitle.append("_");

    if(FileInfo.filePath().startsWith("file://")){
        FilePathFromSlash = FileInfo.filePath().mid(7);
    }else{
        FilePathFromSlash = FileInfo.filePath();
    }

        QFileInfo Info(FilePathFromSlash);
        VideoTitle.append(QString::number(Info.size()));


    ThumbInfo.append(filePath);

    QByteArray VideoTitleByteArray = QCryptographicHash::hash(VideoTitle.toUtf8(), QCryptographicHash::Md5);
    QString VideoTitleMD5 = VideoTitleByteArray.toHex();

    QString fileName = QLatin1String("/app/tracker/.thumbnails/normal/")
            + VideoTitleMD5
            + QLatin1String(".jpeg");

    LOG_CRITICAL << "TA::RequestVideoThumbnail 1 videoTitle " << VideoTitle << LOG_ENDL;


    if (QFile::exists(fileName))
    {
        fileName.prepend("file://");
        ThumbInfo.append(fileName);
        LOG_TRACE << "Thumbnail path is : " << fileName << LOG_ENDL;


    }
    else
    {
        ThumbInfo.append("No Thumbnail Data");
        HandleError(eErrorNoThumbnailData);
        m_ThumbnailerInfo.append(ThumbInfo);
    }

    ret = true;
    emit DataFetched(m_ThumbnailerInfo, eGetVideoFileThumbNail_Id);
    return ret;
}


bool CCoverArtDataprovider::RequestVideoThumbnail(const QStringList &filePath)
{
    bool ret = false;

    /*
    QList<QUrl> uris;
    QStringList mimes;
   if(!m_ThumbNailer)
    {
        ret = InitializeThumbNailer();
    }
    else
        ret = true;
        */

    ret = true;
    m_ThumbnailerInfo.clear();
    if(ret)
    {
        for(int i=0; i<filePath.count(); i++)
        {
            QString VideoTitle;
            QStringList ThumbInfo;
            QString FilePathFromSlash; // Modified on 2013.07.09 for ITS # 0178912
            QFileInfo FileInfo(filePath.at(i));

            // Modified on 2013.07.09 for ITS # 0178912


            if(FileInfo.filePath().startsWith("file://")){
                FilePathFromSlash = FileInfo.filePath().mid(7);
            }else{
                FilePathFromSlash = FileInfo.filePath();
            }

            QFileInfo Info(FilePathFromSlash);

            VideoTitle.append(FilePathFromSlash);
            VideoTitle.append("_");
            VideoTitle.append(QString::number(Info.size()));

            ThumbInfo.append(filePath.at(i));


            QByteArray VideoTitleByteArray = QCryptographicHash::hash(VideoTitle.toUtf8(), QCryptographicHash::Md5);
            QString VideoTitleMD5 = VideoTitleByteArray.toHex();

            QString fileName = QLatin1String("/app/tracker/.thumbnails/normal/")
                    + VideoTitleMD5
                    + QLatin1String(".jpeg");

            //commented below log by honggi.shin, 2014.02.27, for removing heavy log
//            LOG_CRITICAL << "Puneet TA::RequestVideoThumbnail 2 filepath = " << VideoTitle
//                         << " thumb Path = " << fileName << LOG_ENDL;
            QFile videothumbPath(fileName);
            if (videothumbPath.exists())
            {
                if(videothumbPath.size() > 0)
                {
                    fileName.prepend("file://");
                    ThumbInfo.append(fileName);
                    m_ThumbnailerInfo.append(ThumbInfo);
                    ret = true;
                }else
                {
                    LOG_CRITICAL << "Puneet TA::RequestVideoThumbnail: ThumbFilePath is 0" << LOG_ENDL;
                    ThumbInfo.append("No Thumbnail Data");
                    //HandleError(eErrorNoThumbnailData);
                    m_ThumbnailerInfo.append(ThumbInfo);
                    ret = true;
                }
            }
            else
            {
                // removed by Dmitry 26.07.13
// { modified by eugene.seo 2013.03.27 for static test
#if 0
                status = GetVideoThumbnails(VideoFileUrl,thumbnailpath);

				if(!status)
				{
					ThumbInfo.append("No Thumbnail Data");
					HandleError(eErrorNoThumbnailData);
					m_ThumbnailerInfo.append(ThumbInfo);
				}
				else
				{
					ThumbInfo.append(thumbnailpath);
					m_ThumbnailerInfo.append(ThumbInfo);
					ret = true;
				}

#else
                // modified by Dmitry 26.07.13
                ThumbInfo.append("No Thumbnail Data");
                //HandleError(eErrorNoThumbnailData);
                m_ThumbnailerInfo.append(ThumbInfo); 
                ret = true;
                // modified by Dmitry 26.07.13
#endif
// } modified by eugene.seo 2013.03.27 for static test
            }
            //LOG_TRACE << "ThumbInfo [0]" << ThumbInfo.at(0) << LOG_ENDL;
            //LOG_TRACE << "ThumbInfo [1]" << ThumbInfo.at(1) << LOG_ENDL;

        }
        //LOG_TRACE << "Tracker Result Data Count : " << m_ThumbnailerInfo.count() << LOG_ENDL;
        //LOG_TRACE << "Tracker Result Data  :" << m_ThumbnailerInfo << LOG_ENDL;
        //LOG_TRACE << "Query ID : " << eGetVideoFileListThumbNail_Id << LOG_ENDL;
        emit DataFetched(m_ThumbnailerInfo, eGetVideoFileListThumbNail_Id);
        //        emit DataFetched(m_ThumbnailerInfo, eGetVideoFileThumbNail_Id);
        //
#if 0
        QString tempMime;
        QString tempURI(filePath[i]);
        if(tempURI.startsWith("/"))
            tempURI.prepend(QString("file://"));
        foreach(QString mime, m_ThumnailsMimes.keys()) {
            if(filePath[i].endsWith(mime, Qt::CaseInsensitive)) {
                LOG_TRACE << "mime type req is: " << m_ThumnailsMimes[mime] << LOG_ENDL;
                tempMime.append(m_ThumnailsMimes[mime]);
            }
        }
        uris << tempURI;
        mimes << tempMime;
    }
    m_ThumbNailer->request(uris, mimes);
#endif

}

return ret;
}

bool CCoverArtDataprovider::RequestPhotoThumbnail(const QString &filePath)
{
    bool ret = false;
    if(filePath.isNull())
    {
        HandleError(eNullStringParameter);
        return ret;
    }
    m_ThumbnailerInfo.clear();
    gthumbnail_flag =eImage;
    LOG_TRACE <<"in CCoverArtDataprovider::RequestPhotoThumbnail for string" << LOG_ENDL;
    if(!m_ThumbNailer)
    {
        ret = InitializeThumbNailer();
    }
    else
        ret = true;

    if(ret)
    {
        QString tempMime;
        QString tempURI(filePath);
        if(tempURI.startsWith("/"))
            tempURI.prepend(QString("file://"));
        foreach(QString mime, m_ThumnailsMimes.keys()) {
            if(filePath.endsWith(mime, Qt::CaseInsensitive)) {
                LOG_TRACE << "mime type req is: " << m_ThumnailsMimes[mime] << LOG_ENDL;
                tempMime.append(m_ThumnailsMimes[mime]);
            }
        }
        QList<QUrl> uris;
        uris << tempURI;
        QStringList mimes;
        mimes << tempMime;
        m_ThumbNailer->request(uris, mimes);
    }

    return ret;
}

bool CCoverArtDataprovider::RequestPhotoThumbnail(const QStringList &filePath)
{
    bool ret = false;

    gthumbnail_flag =eImageList;
    LOG_TRACE << "in CCoverArtDataprovider::RequestPhotoThumbnail for string list" << LOG_ENDL;
    QList<QUrl> uris;
    QStringList mimes;
    m_ThumbnailerInfo.clear();
    if(!m_ThumbNailer)
    {
        ret = InitializeThumbNailer();
    }
    else
        ret = true;

    if(ret)
    {
        for(int i=0; i<filePath.count(); i++)
        {
            QString tempMime;
            QString tempURI(filePath[i]);
            if(tempURI.startsWith("/"))
                tempURI.prepend(QString("file://"));
            foreach(QString mime, m_ThumnailsMimes.keys()) {
                if(filePath[i].endsWith(mime, Qt::CaseInsensitive)) {
                    LOG_TRACE << "mime type req is: " << m_ThumnailsMimes[mime] << LOG_ENDL;
                    tempMime.append(m_ThumnailsMimes[mime]);
                }
            }

            if(CheckIfThumbnailPresent(tempURI))
            {
                LOG_TRACE << "Cover art present" << LOG_ENDL;
            }
            else
            {
                LOG_TRACE << "Cover art not present" << LOG_ENDL;
                uris << tempURI;
                mimes << tempMime;
            }
        }
        // If there are no paths, for which cover art should be created, emit the signal

        if(uris.count() == 0)
        {
            emit DataFetched(m_ThumbnailerInfo, eGetImageThumbNail_Id);
        }
        else
        {
            // Issue the request for filepaths, for which cover art is not present
            m_ThumbNailer->request(uris, mimes);
        }
    }

    return ret;
}

void CCoverArtDataprovider::Started()
{
}

void CCoverArtDataprovider::Finished(int)
{

    switch(gthumbnail_flag)
    {
    case eImage:
    {
        emit DataFetched(m_ThumbnailerInfo, eGetImageThumbNail_Id);
        break;
    }
    case eImageList:
    {
        emit DataFetched(m_ThumbnailerInfo, eGetImageThumbNail_Id);
        break;
    }
    case eVideo:
    {
        emit DataFetched(m_ThumbnailerInfo, eGetVideoFileThumbNail_Id);
        break;
    }
    case eVideoList:
    {
        emit DataFetched(m_ThumbnailerInfo, eGetVideoFileListThumbNail_Id);
        break;
    }
    default:
        break;
    }
    gthumbnail_flag = eInvalid;

    m_ThumbnailerInfo.clear();
}

void CCoverArtDataprovider::Thumbnail(QUrl filePath,QUrl thumnailPath)
{
    //    if(!m_ThumbnailerInfo.count())
    //        m_ThumbnailerInfo.resize(2);
    //    m_ThumbnailerInfo[0].append(filePath.toString());
    //    if(thumnailPath.toString().length()>0)
    //        m_ThumbnailerInfo[1].append(thumnailPath.toString());
    //    else
    //        m_ThumbnailerInfo[1].append("No Thumbnail Data");

    QStringList thumbInfo;
    thumbInfo.append(filePath.toString());
    if(thumnailPath.toString().length()>0)
    {
        /**
          * Fixed CR#2251
          *changes to replace the .jpeg path with .png
          *and check for the file path is done due to the
          *bug in libthumbnailer library which always returns the
          *jpeg file even if the .png exists(not jpeg).
          */
        QString clippedPath(thumnailPath.toString());
        clippedPath.replace("file://", "");

        QString pngPath(clippedPath);
        pngPath.replace(".jpeg", ".png");

        if(QFile::exists(clippedPath))
        {
            thumbInfo.append(thumnailPath.toString());
        }
        else if(QFile::exists(pngPath))
        {
            pngPath.prepend("file://");
            thumbInfo.append(pngPath);
        }
        else
        {
            thumbInfo.append("No Thumbnail Data");
            HandleError(eErrorNoThumbnailData);
        }
    }
    else
    {
        thumbInfo.append("No Thumbnail Data");
        HandleError(eErrorNoThumbnailData);
    }

    m_ThumbnailerInfo.append(thumbInfo);

}

void CCoverArtDataprovider::Error(QString error, QUrl filePath,QUrl thumnailPath)
{
    LOG_TRACE << "CCoverArtDataprovider::Error() IN" << LOG_ENDL;
    LOG_TRACE <<"error is: " << error << LOG_ENDL;
    //    if(!m_ThumbnailerInfo.count())
    //        m_ThumbnailerInfo.resize(2);
    //    m_ThumbnailerInfo[0].append(filePath.toString());
    //    m_ThumbnailerInfo[1].append(thumnailPath.toString());

    QStringList thumbInfo;
    thumbInfo.append(filePath.toString());
    if(thumnailPath.toString().length()>0)
        thumbInfo.append(thumnailPath.toString());
    else
        thumbInfo.append("No Thumbnail Data");

    m_ThumbnailerInfo.append(thumbInfo);

}

void CCoverArtDataprovider::Dequeued(QUrl filePath)
{
    LOG_TRACE << "CCoverArtDataprovider::Dequeued() IN" << LOG_ENDL;
    //    if(!m_ThumbnailerInfo.count())
    //        m_ThumbnailerInfo.resize(2);
    //    m_ThumbnailerInfo[0].append(filePath.toString());
    //    m_ThumbnailerInfo[1].append("No Thumbnail Data");

    QStringList thumbInfo;
    thumbInfo.append(filePath.toString());
    thumbInfo.append("No Thumbnail Data");

    m_ThumbnailerInfo.append(thumbInfo);

}

bool CCoverArtDataprovider::InitializeThumbNailer()
{
    //TODO: dynamic support of the mime type has to implemented
    // m_ThumnailsMimes = Thumbnailer::getSupported ();

    bool ret = false;

    if(m_ThumnailsMimes.count() <= 0) {
        m_ThumnailsMimes.insert(".jpeg", "image/jpeg");         //OK
        m_ThumnailsMimes.insert(".jpg",  "image/jpeg");          //OK
        m_ThumnailsMimes.insert(".gif",  "image/gif");             //OK
        m_ThumnailsMimes.insert(".png",  "image/png");         //OK
        m_ThumnailsMimes.insert(".bmp",  "image/bmp");      //OK
        m_ThumnailsMimes.insert(".tif",  "image/tiff");             //OK
        m_ThumnailsMimes.insert(".tiff", "image/tiff");             //X

        m_ThumnailsMimes.insert(".avi",  "video/x-msvideo");
        m_ThumnailsMimes.insert(".AVI",  "video/x-msvideo");
        m_ThumnailsMimes.insert(".mpg",  "video/mpeg");
        m_ThumnailsMimes.insert(".dat",  "video/mpeg");
        m_ThumnailsMimes.insert(".mpeg", "video/mpeg");
        m_ThumnailsMimes.insert(".mp4",  "video/mp4");
        m_ThumnailsMimes.insert(".WMV",  "video/x-ms-wmv");
        m_ThumnailsMimes.insert(".asf",  "video/x-ms-asf");
        m_ThumnailsMimes.insert(".m4v",  "video/x-m4v");
        m_ThumnailsMimes.insert(".mkv",  "video/x-matroska");
        m_ThumnailsMimes.insert(".divx",  "video/x-divx");
        /* This For Future Reference */
        // m_ThumnailsMimes.insert(".ogv",  "video/ogg");
        // m_ThumnailsMimes.insert(".mpga", "video/mpeg");
        // m_ThumnailsMimes.insert(".mpe",  "video/mpeg");
        // m_ThumnailsMimes.insert(".mpa",  "video/mpeg");
        //  m_ThumnailsMimes.insert(".mp2",  "video/mpeg");
        //  m_ThumnailsMimes.insert(".3gp",  "video/3gpp");
        //  m_ThumnailsMimes.insert(".flv",  "video/x-flv");
        //  m_ThumnailsMimes.insert(".mov",  "video/quicktime");
        //   m_ThumnailsMimes.insert(".ogg",  "video/ogg");
        //    m_ThumnailsMimes.insert(".vob",  "video/mpeg");
    }

    m_ThumbNailer  =  new Thumbnailer();

    if(m_ThumbNailer != NULL)
    {
        ret = true;
        // connect all signals from Thumbnailer
        connect(m_ThumbNailer, SIGNAL(started()), this, SLOT(Started()));
        connect(m_ThumbNailer, SIGNAL(finished(int)), this, SLOT(Finished(int)));
        connect(m_ThumbNailer, SIGNAL(thumbnail(QUrl,QUrl,QPixmap,QString)), this, SLOT(Thumbnail(QUrl,QUrl)));
        connect(m_ThumbNailer, SIGNAL(error(QString,QUrl,QUrl)), this, SLOT(Error(QString,QUrl, QUrl)));
        connect(m_ThumbNailer, SIGNAL(dequeued(QUrl)), this, SLOT(Dequeued(QUrl)));
    }

    return ret;
}

void CCoverArtDataprovider::HandleError(int nError)
{
    LOG_CRITICAL << " the error is = " << nError << LOG_ENDL;
    emit Error(nError);
}


//This code is not used

#if 0

bool CCoverArtDataprovider::GetVideoThumbnails(QString VideoPath,QString &thumbnailPath)
{
    GstElement *pipeline, *sink;
    gint width, height;
    GstBuffer *buffer;
    gchar *descr=NULL; //pointer initialisation
    GError *error = NULL;
    GdkPixbuf *pixbuf;
    gint64 duration, position;
    GstFormat format;
    GstStateChangeReturn ret;
    gboolean res;

    bool status  = false;

    gst_init (NULL,NULL);
    QGst::init(0,0);

    if(VideoPath.isEmpty())
    {
        return status;
    }



    /*Creating Uri Compatible with decodebin*/

    //QString VideoPath = VideoList.at(i);
    QString VideoTitleURL = VideoPath;
    //VideoPath = VideoPath.replace(" ","%20");
    VideoPath = VideoPath.replace(" ","\ ");
    VideoPath = VideoPath.remove(0,7);
    GetMediaParams(VideoPath);

    QByteArray VPUrl = VideoPath.toLocal8Bit();
    char *videoCharPathUrl = VPUrl.data();

    LOG_TRACE<<"VideoPath = "<<VideoPath << LOG_ENDL;
    //qDebug()<<"VPUrl = "<<VPUrl;
    //qDebug()<<"videoCharPathUrl = "<<videoCharPathUrl;
    QFileInfo FileInfo(VideoTitleURL);
    QString VideoTitle = FileInfo.baseName();
    QString VideoThumbnailPath = "/app/tracker/.thumbnails/normal/";

    QByteArray VideoTitleByteArray = QCryptographicHash::hash(VideoTitle.toUtf8(), QCryptographicHash::Md5);
    QString VideoTitleMD5 = VideoTitleByteArray.toHex();

    VideoThumbnailPath.append(VideoTitleMD5);
    VideoThumbnailPath.append(".jpeg");
    QByteArray VTMD5 = VideoThumbnailPath.toLocal8Bit();
    char *videoThumbnailDestPath = VTMD5.data();

    LOG_TRACE<<"VideoThumbnailPath = "<<VideoThumbnailPath << LOG_ENDL;
    LOG_TRACE<<"VideoTitle = "<<VideoTitle << LOG_ENDL;
    LOG_TRACE<<"VideoTitleMD5 = "<<VideoTitleMD5 << LOG_ENDL;
    //qDebug()<<"VTMD5 = "<<VTMD5;
    LOG_TRACE <<"videoThumbnailDestPath = "<<videoThumbnailDestPath << LOG_ENDL;

    /* create a new pipeline */
    /*
          descr = g_strdup_printf ("uridecodebin uri=%s ! ffmpegcolorspace ! videoscale ! "
                               " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);
      */
    if(m_fileType.compare("MEDIA_DIVX_AVI")==0)
    {
        if(m_videoType.compare("VIDEO_H264")==0)
        {
            LOG_TRACE<<"DIVX-AVI-H264" << LOG_ENDL;
            descr = g_strdup_printf ("filesrc location=%s ! avidemux ! ffdec_h264 ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);
        }
        else if(m_videoType.compare("VIDEO_H263")==0)
        {
            LOG_TRACE <<"DIVX-AVI-H263" << LOG_ENDL;
            descr = g_strdup_printf ("filesrc location=%s ! avidemux ! ffdec_h263 ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);

        }
        else if((m_videoType.compare("VIDEO_MPEG4")==0) ||
                (m_videoType.compare("VIDEO_DIVX")==0) ||
                (m_videoType.compare("VIDEO_XVID")==0))
        {
            LOG_TRACE <<"DIVX-AVI-MPEG4 or DIVX or XVID" << LOG_ENDL;
            descr = g_strdup_printf ("filesrc location=%s ! avidemux ! ffdec_mpeg4 ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);
        }
        else if((m_videoType.compare("VIDEO_MPGE1")==0) ||
                (m_videoType.compare("VIDEO_MPGE2")==0))
        {
            LOG_TRACE <<"DIVX-AVI MPEG1 or MPEG2" << LOG_ENDL;

            //descr = g_strdup_printf ("filesrc location=%s ! avidemux ! ffdec_mpeg2video ! ffmpegcolorspace ! videoscale ! "
            //                         " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);

            descr = g_strdup_printf ("uridecodebin uri=file://%s ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);
        }
        else
            LOG_TRACE<<"Add Support for"<<m_fileType << "with" <<m_videoType << LOG_ENDL;
    }
    else if (m_fileType.compare("MEDIA_MP4")==0)
    {
        if(m_videoType.compare("VIDEO_H264")==0)
        {
            LOG_TRACE << "MP4-H264" << LOG_ENDL;
            descr = g_strdup_printf ("filesrc location=%s ! qtdemux ! ffdec_h264 ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);
        }
        else if(m_videoType.compare("VIDEO_H263")==0)
        {
            LOG_TRACE<<"MP4-H263" << LOG_ENDL;
            descr = g_strdup_printf ("filesrc location=%s ! qtdemux ! ffdec_h263 ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);

        }
        else if((m_videoType.compare("VIDEO_MPEG4")==0) ||
                (m_videoType.compare("VIDEO_DIVX")==0) ||
                (m_videoType.compare("VIDEO_XVID")==0))
        {
            LOG_TRACE<<"MP4-MPEG4 or DIVX or XVID" << LOG_ENDL;
            descr = g_strdup_printf ("filesrc location=%s ! qtdemux ! ffdec_mpeg4 ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);

        }
        else if((m_videoType.compare("VIDEO_MPGE1")==0) ||
                (m_videoType.compare("VIDEO_MPGE2")==0))
        {
            LOG_TRACE <<"MP4-MPEG1 or MPEG2" << LOG_ENDL;

            //descr = g_strdup_printf ("filesrc location=%s ! qtdemux ! ffdec_mpeg2video ! ffmpegcolorspace ! videoscale ! "
            //                         " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);

            descr = g_strdup_printf ("uridecodebin uri=file://%s ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);
        }
        else
            LOG_TRACE <<"Add Support for"<<m_fileType << "with" <<m_videoType << LOG_ENDL;
    }
    else if (m_fileType.compare("MEDIA_WMV_ASF")==0)
    {
        if((m_videoType.compare("VIDEO_VC1")==0) ||
                (m_videoType.compare("VIDEO_WMV")==0))
        {
            LOG_TRACE <<"DIVX-AVI-VC1 or WMV" << LOG_ENDL;
            descr = g_strdup_printf ("filesrc location=%s ! asfdemux ! ffdec_wmv3 ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);
        }
        else
            LOG_TRACE <<"Add Support for"<<m_fileType << "with" <<m_videoType << LOG_ENDL;
    }
    else if(m_fileType.compare("MEDIA_MPEG_MPG")==0)
    {
        if((m_videoType.compare("VIDEO_MPEG4")==0) ||
                (m_videoType.compare("VIDEO_DIVX")==0) ||
                (m_videoType.compare("VIDEO_XVID")==0))
        {
            LOG_TRACE <<"MEDIA_MPEG_MPG MPEG4 or DIVX or XVID" << LOG_ENDL;
            descr = g_strdup_printf ("filesrc location=%s ! flupsdemux ! ffdec_mpeg4 ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);
        }
        else if((m_videoType.compare("VIDEO_MPGE1")==0) ||
                (m_videoType.compare("VIDEO_MPGE2")==0))
        {
            LOG_TRACE <<"MEDIA_MPEG_MPG MPEG1 or MPEG2" << LOG_ENDL;

            //descr = g_strdup_printf ("filesrc location=%s ! flupsdemux ! ffdec_mpeg2video ! ffmpegcolorspace ! videoscale ! "
            //                         " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);

            descr = g_strdup_printf ("uridecodebin uri=file://%s ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);

        }
        else
            LOG_TRACE <<"Add Support for"<<m_fileType << "with" <<m_videoType << LOG_ENDL;
    }
    else if(m_fileType.compare("MEDIA_MKV")==0)
    {
        if(m_videoType.compare("VIDEO_H264")==0)
        {
            LOG_TRACE <<"MKV H264" << LOG_ENDL;
            descr = g_strdup_printf ("filesrc location=%s ! matroskademux ! ffdec_h264 ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);
        }
        else if(m_videoType.compare("VIDEO_H263")==0)
        {
            LOG_TRACE <<"MKV-H263" << LOG_ENDL;
            descr = g_strdup_printf ("filesrc location=%s ! matroskademux ! ffdec_h263 ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);
        }
        else if((m_videoType.compare("VIDEO_MPEG4")==0) ||
                (m_videoType.compare("VIDEO_DIVX")==0) ||
                (m_videoType.compare("VIDEO_XVID")==0))
        {
            LOG_TRACE <<"MKV-MPEG4 or DIVX or XVID" << LOG_ENDL;
            descr = g_strdup_printf ("filesrc location=%s ! matroskademux ! ffdec_mpeg4 ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);

        }
        else if((m_videoType.compare("VIDEO_MPGE1")==0) ||
                (m_videoType.compare("VIDEO_MPGE2")==0))
        {
            LOG_TRACE <<"MKV-MPEG1 or MPEG2" << LOG_ENDL;
            //descr = g_strdup_printf ("filesrc location=%s ! matroskademux ! ffdec_mpeg2video ! ffmpegcolorspace ! videoscale ! "
            //                         " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);

            descr = g_strdup_printf ("uridecodebin uri=file://%s ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);

        }
        else if((m_videoType.compare("VIDEO_VC1")==0) ||
                (m_videoType.compare("VIDEO_WMV")==0))
        {
            LOG_TRACE <<"MKV-VC1 or WMV" << LOG_ENDL;
            descr = g_strdup_printf ("filesrc location=%s ! matroskademux ! ffdec_wmv3 ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);

        }
        else
            LOG_TRACE<<"Add Support for"<<m_fileType << "with" <<m_videoType << LOG_ENDL;
    }
    else if(m_fileType.compare("MEDIA_OGV")==0)
    {
        if(m_videoType.compare("VIDEO_THEORA")==0)
        {
            LOG_TRACE <<"OGV THEORA" << LOG_ENDL;

            //descr = g_strdup_printf ("filesrc location=%s ! oggdemux ! theoradec ! ffmpegcolorspace ! videoscale ! "
            //                       " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);

            descr = g_strdup_printf ("uridecodebin uri=file://%s ! ffmpegcolorspace ! videoscale ! "
                                     " appsink name=sink caps=\"" THUMBNAIL_IMAGE_CAPS "\"", videoCharPathUrl);
        }
        else
            LOG_TRACE <<"Add Support for"<<m_fileType << "with" <<m_videoType << LOG_ENDL;
    }
    else
        LOG_TRACE <<"Problem in filetype and videotype comparison" << LOG_ENDL;

    LOG_TRACE <<"Constructed Pipeline===="<<descr << LOG_ENDL;
    sleep(1);
    if(descr!=NULL)
    {
        pipeline = gst_parse_launch (descr, &error);
    }

    if(!pipeline){
        goto done;
    }

    if (error != NULL)
    {
        LOG_TRACE <<"could not construct pipeline: %s"<<error->message << LOG_ENDL;
        g_error_free (error);
        gst_element_set_state (pipeline, GST_STATE_NULL);
        gst_object_unref (pipeline);
        status = false;
        goto done;
    }

    /* get sink */
    sink = gst_bin_get_by_name (GST_BIN (pipeline), "sink");

    /* set to PAUSED to make the first frame arrive in the sink */
    ret = gst_element_set_state (pipeline, GST_STATE_PAUSED);

    switch (ret)
    {
    case GST_STATE_CHANGE_FAILURE:
        LOG_TRACE <<"failed to play the file" << LOG_ENDL;
        gst_element_set_state (pipeline, GST_STATE_NULL);
        gst_object_unref (pipeline);
        status = false;
        goto done;

    default:
        break;
    }

    /* This can block for up to 5 seconds. If your machine is really overloaded,
       * it might time out before the pipeline prerolled and we generate an error. A
       * better way is to run a mainloop and catch errors there. */

    ret = gst_element_get_state (pipeline, NULL, NULL, 5 * GST_SECOND);

    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        LOG_TRACE <<"failed to play the file" << LOG_ENDL;
        gst_element_set_state (pipeline, GST_STATE_NULL);
        gst_object_unref (pipeline);
        status = false;
        goto done;
    }

    /* get the duration */
    format = GST_FORMAT_TIME;
    gst_element_query_duration (pipeline, &format, &duration);

    if (duration != -1)
    {
        /* we have a duration, seek to 5% */
        position = duration * 5 / 100;
    }
    else
    {
        /* no duration, seek to 1 second, this could EOS */
        position = 1 * GST_SECOND;
        LOG_TRACE <<"no duration, seek to 1 second, this could EOS" << LOG_ENDL;
    }

    /* seek to the a position in the file. Most files have a black first frame so
       * by seeking to somewhere else we have a bigger chance of getting something
       * more interesting. An optimisation would be to detect black images and then
       * seek a little more */

    gst_element_seek_simple (pipeline, GST_FORMAT_TIME,
                             (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT) , position);

    /* get the preroll buffer from appsink, this block untils appsink really
       * prerolls */

    g_signal_emit_by_name (sink, "pull-preroll", &buffer, NULL);

    /* if we have a buffer now, convert it to a pixbuf. It's possible that we
       * don't have a buffer because we went EOS right away or had an error. */

    if (buffer)
    {
        GstCaps *caps;
        GstStructure *s;

        /* get the snapshot buffer format now. We set the caps on the appsink so
             * that it can only be an rgb buffer. The only thing we have not specified
             * on the caps is the height, which is dependant on the pixel-aspect-ratio
             * of the source material */
        caps = GST_BUFFER_CAPS (buffer);

        if (!caps)
        {
            LOG_TRACE<<"could not get snapshot format" << LOG_ENDL;
            gst_element_set_state (pipeline, GST_STATE_NULL);
            gst_object_unref (pipeline);
            status = false;
            goto done;
        }
        s = gst_caps_get_structure (caps, 0);

        /* we need to get the final caps on the buffer to get the size */

        res = gst_structure_get_int (s, "width", &width);
        res |= gst_structure_get_int (s, "height", &height);
        if (!res)
        {
            LOG_TRACE <<"could not get snapshot dimension" << LOG_ENDL;
            gst_element_set_state (pipeline, GST_STATE_NULL);
            gst_object_unref (pipeline);
            status = false;
            goto done;
        }

        /* create pixmap from buffer and save, gstreamer video buffers have a stride
             * that is rounded up to the nearest multiple of 4 */
        pixbuf = gdk_pixbuf_new_from_data (GST_BUFFER_DATA (buffer),
                                           GDK_COLORSPACE_RGB, FALSE, 8, width, height,
                                           GST_ROUND_UP_4 (width * 3), NULL, NULL);

        /* save the pixbuf */

        gdk_pixbuf_save (pixbuf, videoThumbnailDestPath, "jpeg", &error, NULL);

        thumbnailPath.append("file://");
        thumbnailPath.append(videoThumbnailDestPath);
        status = true;
    }
    else
    {
        LOG_TRACE <<"could not make snapshot" << LOG_ENDL;
        status = false;
    }

    /* cleanup and exit */
done:
    if(pipeline!=NULL)
    {
        gst_element_set_state (pipeline, GST_STATE_NULL);
        gst_object_unref (pipeline);
        pipeline = NULL;
    }
    if(sink!=NULL)
    {
        sink=NULL;
    }
    if(buffer!=NULL){
        buffer = NULL;
    }
    if(descr!=NULL){
        descr = NULL;
    }
    if(error!=NULL){
        error = NULL;
    }
    if(pixbuf!=NULL){
        pixbuf = NULL;
    }

    return status;
}



/*
 *******************************************************************
 * Function		: GPlayerEngine::GetMediaParams
 *
 * Description 	:
 * Parameters 	: const QString(IN)- Uri  as input

 * Returns 		: None
 *
 *******************************************************************
 */

void CCoverArtDataprovider::GetMediaParams(const QString & uri)
{
    QGst::init(0,0);
    LOG_TRACE <<"inside GetMediaParams uri = "<< uri << LOG_ENDL;
    //Creates the pipeline
    QGst::PipelinePtr m_pipeline = QGst::Pipeline::create();
    if(!m_pipeline) LOG_TRACE<<"pipeline creation failed" << LOG_ENDL;

    QGst::ElementPtr m_parse;

    //Construct the source Video for Linux element
    QGst::ElementPtr m_src = QGst::ElementFactory::make("filesrc");

    if(!m_src) LOG_TRACE << "SRC Element Creation Failed" << LOG_ENDL;
    m_src->setProperty("location", (QString)uri);

    QGst::ElementPtr m_typefind = QGst::ElementFactory::make("typefind");
    if(!m_typefind) LOG_TRACE << "type Element Creation Failed" << LOG_ENDL;

    m_pipeline->add(m_src);
    m_pipeline->add(m_typefind);

    m_src->link(m_typefind);

    QGlib::connect(m_typefind, "have-type", this, &CCoverArtDataprovider::type_found,QGlib::PassSender);

    //Start the pipeline
    m_pipeline->setState(QGst::StatePaused);
    m_pipeline->setState(QGst::StateReady);
    sleep(1);

    if((m_fileType.compare("MEDIA_DIVX_AVI"))==0 ||
            (m_fileType.compare("MEDIA_MP4"))==0 ||
            (m_fileType.compare("MEDIA_WMV_ASF"))==0 ||
            (m_fileType.compare("MEDIA_MPEG_MPG"))==0 ||
            (m_fileType.compare("MEDIA_MKV"))==0 ||
            (m_fileType.compare("MEDIA_OGV"))==0)
    {
        if (m_fileType.compare("MEDIA_DIVX_AVI")==0)
            m_parse = QGst::ElementFactory::make("avidemux");
        if (m_fileType.compare("MEDIA_MP4")==0)
            m_parse = QGst::ElementFactory::make("qtdemux");
        if (m_fileType.compare("MEDIA_WMV_ASF")==0)
            m_parse = QGst::ElementFactory::make("asfdemux");
        if(m_fileType.compare("MEDIA_MPEG_MPG")==0)
            m_parse = QGst::ElementFactory::make("flupsdemux");
        if(m_fileType.compare("MEDIA_MKV")==0)
            m_parse = QGst::ElementFactory::make("matroskademux");
        if(m_fileType.compare("MEDIA_OGV")==0)
            m_parse = QGst::ElementFactory::make("oggdemux");

        if(!m_parse)
            LOG_TRACE <<"Parse element Null" << LOG_ENDL;

        m_pipeline->add(m_parse);
        m_typefind->link(m_parse);
        QGlib::connect(m_parse, "pad-added", this, &CCoverArtDataprovider::OnNewPad);


        m_pipeline.dynamicCast<QGst::Element>();
        m_pipeline->setState(QGst::StatePaused);
        sleep(1);
    }

    if (m_pipeline) LOG_TRACE << "Pipeline Created Successfully" << LOG_ENDL;
    else LOG_TRACE  << "Pipeline Creation failed" << LOG_ENDL;

    /* Clearing elements and setting pipeline to NULL */
    if(m_src) m_src.clear();
    if(m_typefind) m_typefind.clear();
    if(m_parse) m_parse.clear();

    if(m_pipeline)
    {
        m_pipeline->setState(QGst::StateNull);
        m_pipeline.clear();
    }
    LOG_TRACE <<"going outside GetMediaParams" << LOG_ENDL;
}

/*
 *******************************************************************
 * Function		: GPlayerEngine::type_found
 *
 * Description 	:

 * Parameters 	: QGst::ElementPtr &(IN)
 *
 * Returns 		: None
 *
 *******************************************************************
 */

void CCoverArtDataprovider::type_found( QGst::ElementPtr &typefind)
{
    QGst::init(0,0);
    LOG_TRACE <<"Inside type_found" << LOG_ENDL;
    QGst::StructurePtr str;
    QGst::PadPtr typePtr;
    QString StructName;

    typePtr = typefind->getStaticPad("src");

    // Get structure
    str = typePtr->caps()->internalStructure(0);

    StructName = str->name();

    if(StructName.compare("video/quicktime")==0)
    {
        m_fileType = "MEDIA_MP4";
    }
    else if(StructName.compare("video/mpeg")==0)
    {
        m_fileType = "MEDIA_MPEG_MPG";
    }
    else if(StructName.compare("video/x-ms-asf")==0)
    {
        m_fileType = "MEDIA_WMV_ASF";
    }
    else if(StructName.compare("application/ogg")==0)
    {
        m_fileType = "MEDIA_OGV";
    }
    else if(StructName.compare("video/x-msvideo")==0)
    {
        m_fileType = "MEDIA_DIVX_AVI";
    }
    else if(StructName.compare("video/matroska")==0)
    {
        m_fileType = "MEDIA_MKV";
    }
    else
        LOG_CRITICAL << "Unsupported filetype" << LOG_ENDL;

    LOG_TRACE<<"going outside type_found and filetype = "<<m_fileType << LOG_ENDL;

}

/*
 *******************************************************************
 * Function		: GPlayerEngine::OnNewPad
 *
 * Description 	: Funtion to get the mime,width and height information from
                                  Video

 * Parameters 	: QGst::ElementPtr &(IN)
 *
 * Returns 		: None
 *
 *******************************************************************
 */
void CCoverArtDataprovider::OnNewPad ( QGst::PadPtr parsepad )
{
    QGst::init(0,0);
    LOG_TRACE <<"Inside OnNewPad" << LOG_ENDL;
    QGst::StructurePtr m_str=parsepad->caps()->internalStructure(0);

    QString mimetype=m_str->name();
    QString format=m_str->value("format").toString();

    int offset=mimetype.lastIndexOf("/");
    QString Type=mimetype.mid(0,offset);

    /*
        if(Type == "audio" )
        {
          return;
        }
    */
    if(Type.compare("video")==0)
    {
        LOG_TRACE <<"OnNewPad ... Calculating the vidoe type"<<mimetype<<Type << LOG_ENDL;

        if(mimetype.compare("video/x-h264")== 0 )
            m_videoType="VIDEO_H264";
        else if(mimetype.compare("video/x-h263")== 0)
            m_videoType="VIDEO_H263";
        else if((mimetype.compare("video/mpeg")== 0 ) && (4==m_str->value("mpegversion").toInt()))
            m_videoType="VIDEO_MPEG4";
        else if(mimetype.compare("video/x-xvid")== 0 )
            m_videoType="VIDEO_XVID";
        else if((mimetype.compare("video/mpeg")== 0 ) && (1==m_str->value("mpegversion").toInt()))
            m_videoType="VIDEO_MPGE1";
        else if((mimetype.compare("video/mpeg")== 0 ) && (2==m_str->value("mpegversion").toInt()))
            m_videoType="VIDEO_MPGE2";
        else if((mimetype.compare("video/x-wmv")== 0) && (format.compare("WVC1")==0))
            m_videoType="VIDEO_VC1";
        else if((mimetype.compare("video/x-wmv")== 0) && (format.compare("WMV3")==0))
            m_videoType="VIDEO_WMV";
        else if(mimetype.compare("video/x-divx")== 0)
            m_videoType="VIDEO_DIVX";
        else if(mimetype.compare("video/x-theora")== 0)
            m_videoType="VIDEO_THEORA";
        else
            m_videoType="VIDEO_NONE";
    }
    //qDebug()<<"going outside OnNewPad m_videoType=="<<m_videoType;
}

#endif

bool CCoverArtDataprovider::CheckIfThumbnailPresent(QString filePath)
{
    bool result = false;
    QString flavor = "normal";
    QUrl fileUrl = filePath;
    QUrl thumbnailUrl = m_ThumbNailer->getThumbnailPath(fileUrl, flavor);

    QString thumbnailPath = thumbnailUrl.toString();

    thumbnailPath.replace(".jpeg",".png");
    thumbnailPath.replace("file://","");

    if(QFile::exists(thumbnailPath))
    {
        QStringList thumbList;
        thumbList.append(filePath);
        thumbnailPath.prepend("file://");
        thumbList.append(thumbnailPath);

        m_ThumbnailerInfo.append(thumbList);
        result = true;
    }
    return result;

}

/*
 *******************************************************************
 * Function	: CCoverArtDataprovider::RequestCoverArtOnDemand
 *
 * Description 	: Cover arts will be retrieved on demand based on album
 *                artist and file paths provided

 * Parameters 	: albumNameList (IN) - album names list
 *                artistNameList (IN) - artist name list
 *                filePaths (IN) - file path list
 * Returns 	: None
 *
 *******************************************************************
 */
bool CCoverArtDataprovider::RequestCoverArtOnDemand(const QStringList &albumNameList ,
                                                    const QStringList &artistNameList,
                                                    const QStringList &filePaths)
{
    QString filePath;
    filePath.clear();
    // Check if the request is for USB\Jukebox or IPOD
    if(filePaths.count() > 0)
    {
        filePath = filePaths.at(0);

        if(filePath.contains("file:///ipod"))
            m_IfIPODRequest = true;
        else
            m_IfIPODRequest = false;
    }

    // If the request is for iPod, and previous request to iPod Controller is not complete
    // Return empty list
    if(m_IfIPODRequest && m_IsIPODRequestComplete)
    {
        LOG_TRACE << "IPOD Controller is busy handling cover art request" << LOG_ENDL;

        QVector<QStringList> dataList;
        //{added by junam 2013.05.07 for not showing coverart
        foreach(QString albumName, albumNameList)
        {
            QStringList entry;
            entry.append(albumName);
            entry.append("No CoverArt Found");
            dataList.append(entry);
        }
        //}added by junam
        emit DataFetched(dataList, eRetrieveCoverArtOnDemand_Id);

        return false;
    }


    m_CoverArtList.clear();

    // Populate file paths in class variable, which will be used in case cover arts are
    //not retrieved from Gracenote and we have to call Tracker DBus API
    m_FilePaths = filePaths;

    //{ modified by jaehwan.lee 2014.01.24 to fix ITS 0221698
    m_AlbumList.clear();
    m_ArtistList.clear();

    foreach ( QString albumName, albumNameList)
    {
        if (albumName.trimmed().isEmpty()) albumName = "Unknown Albums";
        m_AlbumList.append(albumName);
    }
    foreach ( QString artistName, artistNameList)
    {
        if (artistName.trimmed().isEmpty()) artistName = "Unknown Artists";
        m_ArtistList.append(artistName);
    }

    //{added by junam 2013.05.16 for coverart match
    if(filePath.startsWith("file:///ipod"))
        m_CoverArtList = RequestCoverArtFromCache(m_AlbumList, filePaths);
    else //}added by junam
        m_CoverArtList = RequestCoverArtFromCache(m_AlbumList, m_ArtistList);
    //} modified by jaehwan.lee
    // Fetch remaining cover arts from Gracenote

    QStringList albumNamesForGN, artistNamesForGN;

    if(m_IndexList.count() > 0)
    {
        if(!filePath.contains("file:///ipod"))
        {
            RequestCoverArtFromTracker();

        }
		else
			RequestCoverArtFromIPOD(); // adde by eugene.seo 2013.02.19
    }
    else
    {
        // Prepare the list with cover arts and emit signal
        PrepareDataList();
    }

    return true;
}

/*
 *******************************************************************
 * Function	: CCoverArtDataprovider::RequestCoverArtFromCache
 *
 * Description 	: Take the cover arts from cache

 * Parameters 	: albumNameList (IN) - album names list
 *                artistNameList (IN) - artist name list

 * Returns 	: QStringList - cover art paths
 *
 *******************************************************************
 */
QStringList CCoverArtDataprovider::RequestCoverArtFromCache(const QStringList &albumNameList ,
                                                            const QStringList &artistNameList)
{

    QStringList  coverArtList;
    QString coverPath;
    QString albumName, artistName;

    m_IndexList.clear();

    if(albumNameList.count() != artistNameList.count())
    {
        // If the count is not same, return empty list
        coverArtList.clear();
        return coverArtList;
    }

    for (int index = 0 ; index < albumNameList.count() ; index++)
    {
        albumName.clear(); artistName.clear();

        albumName = albumNameList.at(index);
        artistName = artistNameList.at(index);

        if(albumName.isEmpty() || artistName.isEmpty())
        {
            coverArtList.append("No CoverArt Found");
        }
        else
        {
            //{changed by junam 2013.05.16 for coverart match
            //MediaArt::Info artInfo = MediaArt::Album(artistName, albumName);
            QString iPodKey = getIPodUID(artistName);
            MediaArt::Info artInfo = MediaArt::Album( iPodKey.isEmpty() ? artistName : iPodKey, albumName);
            //}changed by junam
//{added by aettie.ji 20130904 for gracenote logo
            if(artInfo.exists())
            {
                coverPath = artInfo.potentialPath().toString();
                coverArtList.append(coverPath);
 //deleted by aettie 20131010 unused code
               /* if(!m_IfIPODRequest)
                {
                    QString gnstr = artistName;
                    gnstr.insert(0, QString("gn_"));

                    MediaArt::Info gn_albumArt;
                    gn_albumArt = MediaArt::Album(gnstr, albumName);

                    if(gn_albumArt.exists())
                    {
                        LOG_TRACE << "cached file from grcenote exist "<< LOG_ENDL;
                        RequestEmitSignalForGracenoteLogo();
                    }
                }*/
            }
//}added by aettie.ji 20130904 for gracenote logo
            else
            {
                coverArtList.append("No CoverArt Found");
                m_IndexList.append(index);
            }
        }
    }

    return coverArtList;

}
//{added by aettie.ji 20130904 for gracenote logo
/*
 *******************************************************************
 * Function	: CCoverArtDataprovider::SendEmitSignalForGracenoteLogo
 *
 * Description 	: 

 * Parameters 	: 

 * Returns 	: 
 *
 *******************************************************************
 */
  //deleted by aettie 20131010 unused code
/*void CCoverArtDataprovider::RequestEmitSignalForGracenoteLogo()
{
        QDBusInterface interface ("org.lge.GracenoteCDLookupDBus", "/");
        LOG_TRACE << "RequestEmitSignalForGracenoteLogo called" << LOG_ENDL;

        QDBusMessage msg = interface.call("RequestEmitSignalForGracenoteLogo");

        if(msg.errorMessage().length() > 0)
        {
            LOG_TRACE << "Error msg came from Gracenote: " << msg.errorMessage() << LOG_ENDL;
        }
    
}*/
//}added by aettie.ji 20130904 for gracenote logo

/*
 *******************************************************************
 * Function	: CCoverArtDataprovider::RequestCoverArtFromGracenote
 *
 * Description 	: Request cover arts from GracenotePluginProcess

 * Parameters 	: albumNameList (IN) - album names list
 *                artistNameList (IN) - artist name list

 * Returns 	: QStringList - cover art paths
 *
 *******************************************************************
 */
void CCoverArtDataprovider::RequestCoverArtFromGracenote(const QStringList &albumNameList ,
                                                         const QStringList &artistNameList)
{
    // Connect signal with Gracenote DBus interface

    bool result = m_Connection.connect("org.lge.GracenoteCDLookupDBus", "/",
                                       "org.lge.GracenoteCDLookupDBus", "CoverArtsRetrieved", this,
                                       SLOT(onCoverArtsRetrived(QStringList)));

    LOG_HIGH << "m_Connection.connect result = "<< result <<LOG_ENDL;
    if(albumNameList.count() > 0)
    {
        QDBusInterface interface ("org.lge.GracenoteCDLookupDBus", "/");
        QVariant arg1 = albumNameList;
        QVariant arg2 = artistNameList;

        QDBusMessage msg = interface.call("RetrieveMultipleCoverArts", arg1, arg2);

        if(msg.errorMessage().length() > 0)
        {
            LOG_TRACE << "Error msg came from Gracenote: " << msg.errorMessage() << LOG_ENDL;

            if(m_IfIPODRequest)
                RequestCoverArtFromIPOD();
            else
            {
                PrepareDataList();				
            }
        }
    }
}


/*
 *******************************************************************
 * Function	: CCoverArtDataprovider::onCoverArtsRetrived
 *
 * Description 	: This slot will be hit, once cover arts are returned
 *                from Gracenote process

 * Parameters 	: coverArts - cover arts list

 * Returns 	: None
 *
 *******************************************************************
 */
void CCoverArtDataprovider::onCoverArtsRetrived(QStringList coverArts)
{
    // Disconnect signal with Gracenote DBus interface

    m_Connection.disconnect("org.lge.GracenoteCDLookupDBus", "/",
                            "org.lge.GracenoteCDLookupDBus", "CoverArtsRetrieved", this,
                            SLOT(onCoverArtsRetrived(QStringList)));

    if(coverArts.count() > 0)
    {
        for (int index = 0; index < coverArts.count(); index++)
        {
            if(index < m_IndexList.count() && index < coverArts.count())
            {

                if(m_IndexList.at(index) < m_CoverArtList.count())
                {
                    m_CoverArtList.replace(m_IndexList.at(index), coverArts.at(index));
                }
            }
        }
    }

    m_IndexList.clear();

    // Check what all entries does not contain, actual file path and error string
    for(int index = 0 ; index < m_CoverArtList.count() ; index++)
    {
        QString coverArt =  m_CoverArtList.at(index);

        if(coverArt.contains("file:///") == 0)
        {
            m_IndexList.append(index);
        }
    }

    if(m_IndexList.count() > 0)
    {
        if(m_IfIPODRequest)
            RequestCoverArtFromIPOD();
        else
            PrepareDataList();
    }
    else
    {
        PrepareDataList();
    }
}

/*
 *******************************************************************
 * Function	: CCoverArtDataprovider::RequestCoverArtFromTracker
 *
 * Description 	: Request cover art from Tracker open source

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */

void CCoverArtDataprovider::RequestCoverArtFromTracker()
{
    int listIndex = 0;
    QString filePath, album, artist, coverPath;
    for (int index = 0 ; index < m_IndexList.count() ; index ++ )
    {
        listIndex   = m_IndexList.at(index);
        QString temp = m_FilePaths.at(listIndex);
        filePath    = QString::fromLatin1(QUrl::fromLocalFile(temp.replace(QLatin1String("file://"), QLatin1String(""))).toEncoded());
        album       = m_AlbumList.at(listIndex);
        artist      = m_ArtistList.at(listIndex);

        // { added by eugene.seo 2013.05.22 
        if(album == "Unknown Albums")
            album = filePath;

        if(artist == "Unknown Artists")
            artist = filePath;
        // } added by eugene.seo 2013.05.22
		
        QDBusInterface interface ("org.freedesktop.Tracker1.Extract",
                                  "/org/freedesktop/Tracker1/Extract",
                                  "org.freedesktop.Tracker1.Extract");

        QVariant arg1 = filePath;

        LOG_TRACE << "File path requesting from tracker" << filePath << LOG_ENDL;

		if(filePath.contains("file:///")) // added by eugene.seo 2013.06.03
		{		
	        QVariant arg2 = "audio/x-alb";
	        QVariant arg3 = "";

	        QString format = "hh:mm:ss.zzz";

	        QDBusMessage reply = interface.call("GetMetadata", arg1, arg2, arg3);

	        QList<QVariant> args = reply.arguments();

	        if(args.count() > 0)
	        {
	            MediaArt::Info artInfo = MediaArt::Album(artist, album);

	            if(artInfo.exists())
	            {
	                coverPath = artInfo.potentialPath().toString();
	                m_CoverArtList.replace(listIndex,  coverPath);
	            }
	        }
		}
    }

    m_IndexList.clear();
    for(int index = 0 ; index < m_CoverArtList.count() ; index++)
    {
        QString coverArt =  m_CoverArtList.at(index);

        if(coverArt.contains("file:///") == 0)
        { 
            m_IndexList.append(index);
        }
    }

    if(m_IndexList.count() > 0)
    {
        QStringList albumNamesForGN, artistNamesForGN;
        artistNamesForGN.clear();
        albumNamesForGN.clear();

        for(int index =0 ; index < m_IndexList.count(); index++)
        {
            // { modified by sangmin.seol 2014.01.22 ITS 0221698 for empty album & artists
            if(m_AlbumList.at(m_IndexList.at(index)).trimmed().isEmpty())
                albumNamesForGN.append("Unknown Albums");
            else
                albumNamesForGN.append(m_AlbumList.at(m_IndexList.at(index)));

            if(m_AlbumList.at(m_IndexList.at(index)).trimmed().isEmpty())
                artistNamesForGN.append("Unknown Artists");
            else
                artistNamesForGN.append(m_ArtistList.at(m_IndexList.at(index)));
            // { modified by sangmin.seol 2014.01.22 ITS 0221698 for empty album & artists
        }
        RequestCoverArtFromGracenote(albumNamesForGN, artistNamesForGN);		
    }
    else
    {
        PrepareDataList();
    }
}

// { added by eugene.seo 2013.04.06
void CCoverArtDataprovider::ResetIPODRequestComplete()
{
	m_IsIPODRequestComplete = false;
}
// } added by eugene.seo 2013.04.06

// { added by eugene.seo 2013.05.17
bool CCoverArtDataprovider::GetIPODRequestComplete()
{
	return m_IsIPODRequestComplete;
}
// } added by eugene.seo 2013.05.17

// { added by eugene.seo 2013.05.17
void CCoverArtDataprovider::CanceliPodCoverArtRequest()
{
	m_IndexList.clear();
    m_AlbumListForIPOD.clear();
	m_UrlListForIPOD.clear();
	m_AlbumList.clear();
    m_CoverArtList.clear();

	m_IsIPODRequestComplete = false;	
	QVector<QStringList> dataList;
	emit DataFetched(dataList, eRetrieveCoverArtOnDemand_Id);
}
// } added by eugene.seo 2013.05.17



/*
 *******************************************************************
 * Function	: CCoverArtDataprovider::RequestCoverArtFromIPOD
 *
 * Description 	: Request cover art from IPOD controller

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
void CCoverArtDataprovider::RequestCoverArtFromIPOD()
{

    m_AlbumListForIPOD.clear();
    //{changed by junam 2013.05.16 for coverart match
    //m_ArtistListForIPOD.clear();
    m_UrlListForIPOD.clear(); 
    //}changed by junam

    for(int index =0 ; index < m_IndexList.count(); index++)
    {
        m_AlbumListForIPOD.append(m_AlbumList.at(m_IndexList.at(index)));
        //{changed by junam 2013.05.16 for coverart match
        //m_ArtistListForIPOD.append(m_ArtistList.at(m_IndexList.at(index)));
        m_UrlListForIPOD.append(m_FilePaths.at(m_IndexList.at(index)));  
        //}changed by junam
    }

    // Request issue to IPOd Controller, make flag true so that further on demand requests dont come
    m_IsIPODRequestComplete = true;

    LOG_TRACE << "Emitting signal for IPOD " << LOG_ENDL;

    //{changed by junam 2013.05.16 for coverart match
    //emit getCoverArt(m_AlbumListForIPOD, m_ArtistListForIPOD);
    emit getCoverArt(m_AlbumListForIPOD, m_UrlListForIPOD);
    //}changed by junam
}

/*
 *******************************************************************
 * Function	: CCoverArtDataprovider::CoverArtsFromIPOD
 *
 * Description 	: Slot will be hit once cover arts from IPOD controller
 *                are retrieved

 * Parameters 	: coverArts - list of cover arts

 * Returns 	: None
 *
 *******************************************************************
 */
void CCoverArtDataprovider::CoverArtsFromIPOD(QStringList coverArts) // rollback
{
    QStringList cacheCoverArts;

	m_IsIPODRequestComplete = false; // added by eugene.seo 2013.04.06

    if( (m_AlbumListForIPOD.count() == m_UrlListForIPOD.count()) &&  //change name by junam 2013.05.16 for coverart match
        (m_AlbumListForIPOD.count() != coverArts.count()) )
    {
    	 // { added by eugene.seo 2013.04.06
    	 for(int i=coverArts.count(); i< m_AlbumListForIPOD.count(); i++)
			   coverArts.append("");
		 // } added by eugene.seo 2013.04.06
   
         LOG_TRACE << "No cover arts available from iPod " << LOG_ENDL;
         //return;   // removed by eugene.seo 2013.04.06
    }

    cacheCoverArts = InsertCoverArtInCache(m_AlbumListForIPOD, m_UrlListForIPOD, coverArts); //change name by junam 2013.05.16 for coverart match
    if(cacheCoverArts.count() > 0 && m_CoverArtList.count() > 0)
    {
        for (int index = 0; index < cacheCoverArts.count(); index++)
        {
            if(index < m_IndexList.count() && index < cacheCoverArts.count())
            {

                LOG_TRACE << "CoverArtsFromIPOD: Index" <<  m_IndexList.at(index) << LOG_ENDL;
                LOG_TRACE << "CoverArtsFromIPOD: Cover art" <<  cacheCoverArts.at(index) << LOG_ENDL;

                m_CoverArtList.replace(m_IndexList.at(index), cacheCoverArts.at(index));
            }
        }
    }
    else
    {
        // If list is empty, no signal should be emitted for HMI
        LOG_TRACE <<"CCoverArtDataprovider::CoverArtsFromIPOD Cover art list is empty" << LOG_ENDL;

        //{added by junam 2013.05.07 for not showing coverart
        QVector<QStringList> dataList;// added by eugene.seo 2013.05.01
        foreach(QString albumName, m_AlbumListForIPOD)
        {
            QStringList entry;
            entry.append(albumName);
            entry.append("No CoverArt Found");
            dataList.append(entry);
        }
        //}added by junam
        m_IndexList.clear();
        m_AlbumListForIPOD.clear();
        //{changed by junam 2013.05.16 for coverart match
        m_UrlListForIPOD.clear();
        //m_ArtistListForIPOD.clear();
        //}changed by junam

        // Make the flag false, so that further requests for cover art can come
        m_IsIPODRequestComplete = false;
        //	QVector<QStringList> dataList; //moved by junam 2013.05.07 for not showing coverart
		emit DataFetched(dataList, eRetrieveCoverArtOnDemand_Id); // added by eugene.seo 2013.05.01
        return;
    }

    m_IndexList.clear();
    m_AlbumListForIPOD.clear();
    //{changed by junam 2013.05.16 for coverart match
    m_UrlListForIPOD.clear();
    //m_ArtistListForIPOD.clear();
    //}changed by junam

    // Make the flag false, so that further requests for cover art can come
    //m_IsIPODRequestComplete = false; // removed by eugene.seo 2013.05.17
    PrepareDataList();
}

/*
 *******************************************************************
 * Function	: CCoverArtDataprovider::InsertCoverArtInCache
 *
 * Description 	: Save the cover arts  retrieved from IPOD Controller
 *                in cache

 * Parameters 	: albumNameList (IN) - album names list
 *                artistNameList (IN) - artist name list
 *                filePaths (IN) - file path list

 * Returns 	: QStringList - cover art paths which are saved
 *
 *******************************************************************
 */
QStringList CCoverArtDataprovider::InsertCoverArtInCache(const QStringList &albumNameList ,
                                                         const QStringList &artistnameList,
                                                         const QStringList &filePaths)
{
    QStringList coverArts;
    QString imagePath;

    for(int index = 0; index < albumNameList.count(); index++)
    {
    	// { added by eugene.seo 2013.05.18
    	if(index < filePaths.count())
	        imagePath = filePaths.at(index);
		else 
			imagePath = "";
		// } added by eugene.seo 2013.05.18

        if(imagePath.contains("/tmp/"))
        {
            QPixmap pixmap(imagePath);

            QSize size = pixmap.size();

            LOG_TRACE << "pixmap: height" << size.height() << LOG_ENDL;
            LOG_TRACE << "pixmap width" << size.width() << LOG_ENDL;

            if(size.height() > 0 && size.width() > 0)
            {
            	if(size.height() > 256 || size.width() > 256) // added by eugene.seo 2013.05.09
					pixmap = pixmap.scaled(QSize(256, 256), Qt::KeepAspectRatio, Qt::FastTransformation);  // added by eugene.seo 2013.05.09

                LOG_TRACE << "CCoverArtDataprovider::Size and height is > 0" << LOG_ENDL;
//{changed by junam 2013.05.16 for coverart match
                //MediaArt::Info albumArt = MediaArt::Album(artistnameList.at(index),
                //                                          albumNameList.at(index));
                QString iPodKey = getIPodUID(artistnameList.at(index));
                MediaArt::Info albumArt = MediaArt::Album( iPodKey.isEmpty() ? artistnameList.at(index) : iPodKey,
                                                          albumNameList.at(index));
//}changed by junam
                albumArt.setMediaArtImage(pixmap, true);

                coverArts.append(albumArt.potentialPath().toString());

                qDebug() << "[TA] COVERART INSERT INTO CACHE" << "ALBUM NAME:" << albumNameList.at(index) << "ARTIST NAME" << artistnameList.at(index) << "IPOD PATH" << imagePath;
                qDebug() << "[TA] COVERART INSERT INTO CACHE CACHE PATH " << albumArt.potentialPath().toString();
            }
            else
            {
                LOG_TRACE << "CCoverArtDataprovider::Size and height is <= 0" << LOG_ENDL;
                qDebug() << "[TA] COVERART INSERT INTO CACHE NO COVERART FOUND CASE" << "ALBUM NAME:" << albumNameList.at(index) << "ARTIST NAME" << artistnameList.at(index);
                coverArts.append("No CoverArt Found");
            }

        }
        else
        {
            coverArts.append("No CoverArt Found");
        }
    }
    LOG_TRACE << "CCoverArtDataprovider::InsertCoverArtInCache End" << LOG_ENDL;

    return coverArts;
}


/*
 *******************************************************************
 * Function	: CCoverArtDataprovider::PrepareDataList
 *
 * Description 	: Prepare the final list, to be given to application

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
void CCoverArtDataprovider::PrepareDataList()
{
    if(m_CoverArtList.count() <= 0)
    {
        LOG_TRACE << "CCoverArtDataprovider : PrepareDataList Cover Art list count is 0" <<  LOG_ENDL;
        return;
    }

    // Populate in a QVector list and emit signal
    QVector<QStringList> dataList;
    QStringList entry;
    for(int index = 0; index < m_CoverArtList.count(); index++)
    {
        entry.clear();
        entry.append(m_AlbumList.at(index));
        entry.append(m_CoverArtList.at(index));
        dataList.append(entry);
    }

    emit DataFetched(dataList, eRetrieveCoverArtOnDemand_Id);

    m_AlbumList.clear();
    m_CoverArtList.clear();
}
//{changed by junam 2013.05.16 for coverart match
QString CCoverArtDataprovider::getIPodUID(QString filePath)
{
    if(filePath.isNull() || filePath.isEmpty() || filePath.size() < 6)
        return QString();

    if(filePath.startsWith("file://"))
        filePath.remove("file://");

    if(!filePath.startsWith("/ipod/"))
        return QString();

    QStringList result = filePath.split("/");

    if(result.size() < 5)
        return QString();

    QString retKey;
    retKey.append(result.at(2));
    retKey.append("_");
    retKey.append(result.at(4));

    return retKey;
}
//}added by junam
//End of file
