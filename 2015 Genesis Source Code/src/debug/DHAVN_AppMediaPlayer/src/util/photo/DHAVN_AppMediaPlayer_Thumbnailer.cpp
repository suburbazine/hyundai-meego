#include "util/photo/DHAVN_AppMediaPlayer_Thumbnailer.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include <QFile>
#include <QCryptographicHash>
#include <QImage>
#include <QDir>
#include <QMutex>
#include <QDebug>
#include <QTime>

#define PHOTO_MAX_SUPPORTED_SIZE 5 //mega bytes
#define PHOTO_MAX_DISPLAY_HEIGHT 720
#define PHOTO_MAX_DISPLAY_WIDTH 1280
#define PHOTO_MAX_SUPPORTED_WIDTH 4000
#define PHOTO_MAX_SUPPORTED_HEIGHT 3000

#define THUMBNAILER_MP_LOG MP_LOG << "[TH_PHOTO]" << metaObject()->className() << "::" << __func__
PhotoThumbnailer* PhotoThumbnailer::m_instance = 0;
QMutex writeImageDataMutex;

bool PhotoThumbnailer::m_readyToPWOFF(false);   //2014.05.23

PhotoThumbnailer::PhotoThumbnailer() : QObject()
  ,m_processor(new PhotoThumbnailProcessor())
{
    THUMBNAILER_MP_LOG << "Construct thumbnailer" << LOG_ENDL;
    m_processor->fillThumbnailerCache();
    m_processor->moveToThread(&m_thumbnailerThread);
    m_thumbnailerThread.setObjectName("PhotoThumbnailProcessor");   // added by sungha.choi 2013.07.19
    m_thumbnailerThread.start(QThread::IdlePriority);
    connect(this, SIGNAL(processThumbnail(QString,QString,int)),
            m_processor, SLOT(processThumbnail(QString,QString,int)), Qt::QueuedConnection);
    connect(m_processor, SIGNAL(thumbnailReady(QString,QString,QString)), this, SLOT(onThumbnailReady(QString,QString,QString)), Qt::QueuedConnection);
    connect(m_processor,SIGNAL(fileNotSupported(QString,QString)), this, SLOT(onFileNotSupported(QString,QString)), Qt::QueuedConnection);
    m_thumbnailsQueue.clear();
}

PhotoThumbnailer::~PhotoThumbnailer()
{

}

// 2014.05.23
void PhotoThumbnailer::setReadyToPWOFF(bool value)
{
   m_readyToPWOFF = value;
}

void PhotoThumbnailer::createDirIfAbsent()
{
    m_processor->createDirIfFormatted();
}

void PhotoThumbnailer::ClearThumbnailData(PHOTO_THUMBNAIL_MEDIA_LIST mediaSource)
{
    THUMBNAILER_MP_LOG << "ClearThumbnailData" + mediaSource << LOG_ENDL;
    if(!m_readyToPWOFF) //added by honggi.shin, 2014.05.23, for stoping IO operation
    formatThumbnailContent(mediaSource);
}

void PhotoThumbnailer::formatThumbnailContent(PHOTO_THUMBNAIL_MEDIA_LIST mediaSource)
{
    if(!m_thumbnailsQueue.isEmpty())
    for ( int it = 0; it < m_thumbnailsQueue.count(); it++ ) // modified by Dmitry 08.05.13
    {
        //THUMBNAILER_MP_LOG << "queue index" << it << LOG_ENDL;
        QString tmp = m_thumbnailsQueue.at(it);
        if(m_thumbnailsSourceHash.value(tmp) == mediaSource)
        {
            //THUMBNAILER_MP_LOG << "removed from queue" << it << LOG_ENDL;
            m_thumbnailsQueue.removeAt(it); // modified by Dmitry 17.08.13
            m_thumbnailsQueueHash.remove(tmp);
            m_thumbnailsSourceHash.remove(tmp);
        }
     }

    const char* dirPath = (mediaSource == PHOTO_THUMBNAIL_MEDIA_JUKEBOX) ? lThumbnailsDirJBox :
                                    ( (mediaSource == PHOTO_THUMBNAIL_MEDIA_USB_1) ? lThumbnailsDirUSB1 : lThumbnailsDirUSB2);
    QDir thumbnailerDir(dirPath);
    if(thumbnailerDir.exists())
    {
        QStringList list = thumbnailerDir.entryList();
        for(int i = 0; i < list.count() ; i++)
        {
            if(!m_readyToPWOFF) //added by honggi.shin, 2014.05.23, for stoping IO operation
            thumbnailerDir.remove(list.at(i));
        }
    }
}

QString PhotoThumbnailer::getThumbnail(const QString& image, PHOTO_THUMBNAIL_MEDIA_LIST mediaSource)
{
    QString thumbnailName = getThumbnailName(image);

    getThumbnail(image, thumbnailName, mediaSource);
    thumbnailName.prepend(lThumbnailsDir);
    return thumbnailName;
}

QString PhotoThumbnailer::getThumbnailName(const QString &image)
{
    QString thumbnailName = QString(QCryptographicHash::hash(image.toUtf8(), QCryptographicHash::Md5).toHex())
            + QString(".jpg");
    //THUMBNAILER_MP_LOG << "image: " << image << " thumbnailName: " << thumbnailName << LOG_ENDL;
    return thumbnailName;
}

void PhotoThumbnailer::getThumbnail(const QString &image, const QString &thumbnailName, PHOTO_THUMBNAIL_MEDIA_LIST mediaSource)
{
    //THUMBNAILER_MP_LOG << "image: " << image << " thumbnailName: " << thumbnailName << LOG_ENDL;
    if(thumbnailName.isEmpty() || image.isEmpty()) {
        THUMBNAILER_MP_LOG << "thumbnailName.isEmpty() || image.isEmpty()" << LOG_ENDL;
        return;
    }

    m_thumbnailsQueue.prepend(thumbnailName);
    m_thumbnailsQueueHash.insert(thumbnailName, image);
    m_thumbnailsSourceHash.insert(thumbnailName, mediaSource);
    //THUMBNAILER_MP_LOG << "m_thumbnailsQueue.count()" << m_thumbnailsQueue.count() << LOG_ENDL;
    if(m_thumbnailsQueue.count() == 1) {
        THUMBNAILER_MP_LOG << "processThumbnail(" << image << ", " << thumbnailName << ") " << LOG_ENDL;
        if( !m_readyToPWOFF ) //added by honggi.shin, 2014.05.23, for finishing thumbnail operation.
        emit processThumbnail(image, thumbnailName, (int)mediaSource);
    }
}

void PhotoThumbnailer::onThumbnailReady(const QString& image, const QString& thumbnailPath, const QString& thumbnailName)
{
    // THUMBNAILER_MP_LOG << "m_thumbnailsQueue.count()" << m_thumbnailsQueue.count() << LOG_ENDL;
    m_thumbnailsQueue.removeAll(thumbnailName);
    m_thumbnailsQueueHash.remove(thumbnailName);
    m_thumbnailsSourceHash.remove(thumbnailName);
    while(!m_thumbnailsQueue.isEmpty()) {
        if(m_thumbnailsQueueHash.contains(m_thumbnailsQueue.first())) {
            // THUMBNAILER_MP_LOG << "processThumbnail(" << m_thumbnailsQueueHash.value(m_thumbnailsQueue.first()) << ", " << m_thumbnailsQueue.first() << ") " << LOG_ENDL;
            if( !m_readyToPWOFF ) //added by honggi.shin, 2014.05.23, for finishing thumbnail operation.
            emit processThumbnail(m_thumbnailsQueueHash.value(m_thumbnailsQueue.first()), m_thumbnailsQueue.first(),
                                    (int)m_thumbnailsSourceHash.value(m_thumbnailsQueue.first()));
            break;
        } else {
            m_thumbnailsQueue.takeFirst();
        }
    }
    emit thumbnailReady(image, thumbnailPath);
}

void PhotoThumbnailer::onFileNotSupported(const QString &image, const QString& thumbnailName)
{
    THUMBNAILER_MP_LOG << "m_thumbnailsQueue.count()" << m_thumbnailsQueue.count() << LOG_ENDL;
    m_thumbnailsQueue.removeAll(thumbnailName);
    m_thumbnailsQueueHash.remove(thumbnailName);
    m_thumbnailsSourceHash.remove(thumbnailName);
    emit fileNotSupported(image);
}

PhotoThumbnailProcessor::PhotoThumbnailProcessor(QObject* parent) : QObject(parent)
{
}

PhotoThumbnailProcessor::~PhotoThumbnailProcessor()
{
}

void PhotoThumbnailProcessor::fillThumbnailerCache()
{
    QFileInfo thumbnailerDirInfo(lThumbnailsDir);
    QDir thumbnailerDir(lThumbnailsDir);
    if(!thumbnailerDirInfo.exists() || !thumbnailerDirInfo.isDir()) {
        thumbnailerDir.mkdir(lThumbnailsDir);
        thumbnailerDir.mkdir(lThumbnailsDirJBox);
        thumbnailerDir.mkdir(lThumbnailsDirUSB1);
        thumbnailerDir.mkdir(lThumbnailsDirUSB2);
        return;
    }

    m_thumbnailerCache = thumbnailerDir.entryList(QStringList(QString("*.jpg")));
    THUMBNAILER_MP_LOG << m_thumbnailerCache << LOG_ENDL;
}

void PhotoThumbnailProcessor::createDirIfFormatted()
{
    QFileInfo thumbnailerDirInfo(lThumbnailsDir);
    QDir thumbnailerDir(lThumbnailsDir);
    if(!thumbnailerDirInfo.exists() || !thumbnailerDirInfo.isDir()) {
        thumbnailerDir.mkdir(lThumbnailsDir);
        thumbnailerDir.mkdir(lThumbnailsDirJBox);
        thumbnailerDir.mkdir(lThumbnailsDirUSB1);
        thumbnailerDir.mkdir(lThumbnailsDirUSB2);
    }
}

void PhotoThumbnailProcessor::processThumbnail(const QString &image, const QString &thumbnailName, int mediaSource)
{
    QString validImageName(image);
    if(validImageName.startsWith("file://")) {
        validImageName = validImageName.remove(0, 7);
    }
//    if(!isFileSupported(validImageName))
//    {
//        emit fileNotSupported(validImageName, thumbnailName);
//        return;
//    }
    // THUMBNAILER_MP_LOG << "Image path: " << validImageName << " thumbnailName: " << thumbnailName << LOG_ENDL;
    QString thumbnailPath = QString(relativeDir(mediaSource));

    thumbnailPath += thumbnailName;

    // THUMBNAILER_MP_LOG << "Thumbnail path: " << thumbnailPath << " for image: " << validImageName << LOG_ENDL;

    //if(m_thumbnailerCache.contains(thumbnailName) && QFile::exists(thumbnailPath) {
    QFile thumbnailFileName(thumbnailPath);
    if( (m_thumbnailerCache.contains(thumbnailName) && QFile::exists(thumbnailPath) && thumbnailFileName.size()>0)
            || (validImageName.endsWith(".png")) ) // modified by ravikanth 28-06-13 for ITS 0177119 
    {
        THUMBNAILER_MP_LOG << "Thumbnail already in cache: " << thumbnailPath << LOG_ENDL;
        emit thumbnailReady(image, thumbnailPath, thumbnailName);
        return;
    }

    QImage img(validImageName);
    // THUMBNAILER_MP_LOG << "thumbnail scale time" << QTime::currentTime().toString("hh:mm:ss.zzz") << LOG_ENDL;
    if(!(img.width() < PHOTO_MAX_DISPLAY_WIDTH && img.height() < PHOTO_MAX_DISPLAY_HEIGHT))
    img = img.scaled(QSize(PHOTO_MAX_DISPLAY_WIDTH, PHOTO_MAX_DISPLAY_HEIGHT), Qt::KeepAspectRatio, Qt::FastTransformation);
    // THUMBNAILER_MP_LOG << "thumbnail Scale completed" << QTime::currentTime().toString("hh:mm:ss.zzz") << LOG_ENDL;
    if(img.width() == 0 || img.height() == 0 || img.isNull())
    {
        THUMBNAILER_MP_LOG << "Cannot scale thumbnail image" << LOG_ENDL;
        return;
    }
    writeImageDataMutex.lock();
    if(img.save(thumbnailPath, "JPG", -1))  //pointed by honggi.shin, 2014.05.23, for finishing thumbnail operation.
    {
        m_thumbnailerCache.append(thumbnailName);
    } else {
        THUMBNAILER_MP_LOG << "Cannot save thumbnail image: " << thumbnailPath << LOG_ENDL;
    }
    THUMBNAILER_MP_LOG << "thumbnail Save complete" << QTime::currentTime().toString("hh:mm:ss.zzz") << LOG_ENDL;
    writeImageDataMutex.unlock();
    // THUMBNAILER_MP_LOG << "Thumbnail prepeared: " << thumbnailPath << LOG_ENDL;
    emit thumbnailReady(image, thumbnailPath, thumbnailName);
}

const char* PhotoThumbnailProcessor::relativeDir(int mediaSource)
{
    switch((PHOTO_THUMBNAIL_MEDIA_LIST)mediaSource)
    {
    case PHOTO_THUMBNAIL_MEDIA_JUKEBOX:
        return lThumbnailsDirJBox;
        break;
    case PHOTO_THUMBNAIL_MEDIA_USB_1:
        return lThumbnailsDirUSB1;
        break;
    case PHOTO_THUMBNAIL_MEDIA_USB_2:
        return lThumbnailsDirUSB2;
        break;
    default:
        break;
    }
    return "";
}

bool PhotoThumbnailProcessor::isFileSupported(const QString file_path)
{
    THUMBNAILER_MP_LOG << "isFileSupported" << file_path << LOG_ENDL;
    QString filename = (file_path == "") ? NULL : file_path;
    double imageFileSize = 0;
    if(filename != NULL && QFile::exists(filename) != false)
    {
        QFile imageFile(filename);
        imageFileSize = (double)imageFile.size()/1048576;
        THUMBNAILER_MP_LOG << "File size: " << imageFileSize << LOG_ENDL;
    }

    if( imageFileSize > 0 && imageFileSize <= PHOTO_MAX_SUPPORTED_SIZE)
    {
        QImage image(filename);
        double imageWidth = image.width();
        double imageHeight = image.height();
        THUMBNAILER_MP_LOG << "Width: " << imageWidth << ", Height: " << imageHeight << LOG_ENDL;
        if( imageWidth == 0 || imageHeight == 0 ||
                imageWidth * imageHeight > PHOTO_MAX_SUPPORTED_WIDTH * PHOTO_MAX_SUPPORTED_HEIGHT )
            return false;
        else
            return true;
    }
    return false; // unsupported value greater than 5
}
