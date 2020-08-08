#include "DHAVN_AppFileManager_Thumbnailer.h"
#include <QFile>
#include <QCryptographicHash>
#include <QImage>
#include <QDir>
#include <QMutex>
#include <QDebug>
// { modified by ravikanth 25-04-13
#include <MediaInfo/MediaInfo.h>

#define PHOTO_MAX_SUPPORTED_SIZE 5 //mega bytes
#define PHOTO_MAX_SUPPORTED_WIDTH 4000
#define PHOTO_MAX_SUPPORTED_HEIGHT 3000
// } modified by ravikanth 25-04-13

#define THUMBNAILER_LOG MP_LOG << "[TH]" << metaObject()->className() << "::" << __func__
Thumbnailer* Thumbnailer::m_instance = 0;
QMutex writeImageDataMutex;

// modified by Dmitry 23.08.13
const QString defaultUnsupportedImage = "/app/share/images/photo/bg_photo_default.png"; // modified by ravikanth 25-04-13
bool Thumbnailer::m_readyToPWOFF(false);   //2014.05.23

Thumbnailer::Thumbnailer() : QObject()
  ,m_processor(new ThumbnailProcessor())
{
    THUMBNAILER_LOG << "Construct thumbnailer" << LOG_ENDL;
    m_processor->fillThumbnailerCache();
    m_processor->moveToThread(&m_thumbnailerThread);
    m_thumbnailerThread.setObjectName("m_thumbnailerThread");   // added by sungha.choi 2013.08.06
    m_thumbnailerThread.start(QThread::IdlePriority);
    connect(this, SIGNAL(processThumbnail(QString)), m_processor, SLOT(processThumbnail(QString)), Qt::QueuedConnection);
    connect(m_processor, SIGNAL(thumbnailReady(QString,QString,bool)), this, SLOT(onThumbnailReady(QString,QString,bool)), Qt::QueuedConnection); // modified by ravikanth 26-04-13
}

Thumbnailer::~Thumbnailer()
{

}

// 2014.05.23
void Thumbnailer::setReadyToPWOFF(bool value)
{
   m_readyToPWOFF = value;
}

// { modified by ravikanth 05-04-13
void Thumbnailer::createDirIfAbsent()
{
    m_processor->createDirIfFormatted();
}
// } modified by ravikanth 05-04-13

void Thumbnailer::generateThumbnail(const QString &image, int index, bool bFront)  // modified by sungha.choi 2013.09.05 for ITS 0188265
{
    m_thumbnailsQueue.append(image);
    m_thumbnailsQueueHash.insert(image, index);
    m_thumbnailsFrontRearQueueHash.insert(image, bFront);  // added by sungha.choi 2013.09.05 for ITS 0188265

    if(m_thumbnailsQueue.count() == 1) {
        if( !m_readyToPWOFF ) //added by honggi.shin, 2014.05.23, for finishing thumbnail operation.
        emit processThumbnail(image);
    }
}

void Thumbnailer::onThumbnailReady(const QString& image, const QString& thumbnailPath, bool unsupported)
{
    if (m_thumbnailsQueue.contains(image))
    {
       QString thumbnail = m_thumbnailsQueue.first();  // added by sungha.choi 2013.09.05 for ITS 0188265
       emit thumbnailReady(thumbnailPath, unsupported, m_thumbnailsQueueHash.value(thumbnail), m_thumbnailsFrontRearQueueHash.value(thumbnail));  // modified by sungha.choi 2013.09.05 for ITS 0188265

       m_thumbnailsQueue.removeAll(image);
       m_thumbnailsQueueHash.remove(image);
       m_thumbnailsFrontRearQueueHash.remove(image);
       while(!m_thumbnailsQueue.isEmpty()) {
           if(m_thumbnailsQueueHash.contains(m_thumbnailsQueue.first())) {
               if( !m_readyToPWOFF ) //added by honggi.shin, 2014.05.23, for finishing thumbnail operation.
               emit processThumbnail(m_thumbnailsQueue.first());
               break;
           } else {
               m_thumbnailsQueue.takeFirst();
           }
       }
    }
}

QString Thumbnailer::getThumbnailPath(const QString &image)
{
    return m_processor->getThumbnailPath(image);
}

void Thumbnailer::clearQueue()
{
   m_thumbnailsQueue.clear();
   m_thumbnailsQueueHash.clear();
   m_thumbnailsFrontRearQueueHash.clear();  // added by sungha.choi 2013.09.05 for ITS 0188265
}

ThumbnailProcessor::ThumbnailProcessor(QObject* parent) : QObject(parent)
{
}

ThumbnailProcessor::~ThumbnailProcessor()
{
}

// { modified by ravikanth 05-04-13
void ThumbnailProcessor::createDirIfFormatted()
{
    QFileInfo thumbnailerDirInfo(lThumbnailsDir);
    QDir thumbnailerDir(lThumbnailsDir);
    if(!thumbnailerDirInfo.exists() || !thumbnailerDirInfo.isDir()) {
        thumbnailerDir.mkdir(lThumbnailsDir);
		// modified on 18-02-14
        thumbnailerDir.mkdir(lThumbnailsDirJB);
        thumbnailerDir.mkdir(lThumbnailsDirUSB1);
        thumbnailerDir.mkdir(lThumbnailsDirUSB2);
    }
}
// } modified by ravikanth 05-04-13

void ThumbnailProcessor::fillThumbnailerCache()
{
    QFileInfo thumbnailerDirInfo(lThumbnailsDir);
    QDir thumbnailerDir(lThumbnailsDir);
    if(!thumbnailerDirInfo.exists() || !thumbnailerDirInfo.isDir()) {
        thumbnailerDir.mkdir(lThumbnailsDir);
		// modified on 18-02-14
        thumbnailerDir.mkdir(lThumbnailsDirJB);
        thumbnailerDir.mkdir(lThumbnailsDirUSB1);
        thumbnailerDir.mkdir(lThumbnailsDirUSB2);
        return;
    }

    m_thumbnailerCache = thumbnailerDir.entryList(QStringList(QString("*.png")));
    THUMBNAILER_LOG << m_thumbnailerCache << LOG_ENDL;
}

QString ThumbnailProcessor::getThumbnailPath(const QString &image)
{
    QString thumbnailName = QString(QCryptographicHash::hash(image.toUtf8(), QCryptographicHash::Md5).toHex())
            + QString(".png");
	// modified on 18-02-14
    //return (QString(lThumbnailsDir) + thumbnailName);
    return (QString(relativeDir(image)) + thumbnailName);
	
}

void ThumbnailProcessor::processThumbnail(const QString &image)
{
    QString validImageName(image);
    if(validImageName.startsWith("file://")) {
        validImageName = validImageName.remove(0, 7);
    }

    QString thumbnailPath = getThumbnailPath(image);

    QFile thumbnailFileName(thumbnailPath);
    if(m_thumbnailerCache.contains(thumbnailPath) && QFile::exists(thumbnailPath) && thumbnailFileName.size()>0)  {
        THUMBNAILER_LOG << "Thumbnail already in cache: " << thumbnailPath << LOG_ENDL;
        emit thumbnailReady(image, thumbnailPath, false); // modified by ravikanth 26-04-13
        return;
    }

    bool unsupported = false;
    // file size calculation
    double imageFileSize = 0;
    if(validImageName != NULL) // modified for ITS 0194889
    {
        if( QFile::exists(validImageName) != false )
        {
            QFile imageFile(validImageName);
            imageFileSize = (double)imageFile.size()/1048576; //  convert to MB
            THUMBNAILER_LOG << "File size: " << imageFileSize << LOG_ENDL;
        }
        else
        {
            THUMBNAILER_LOG << "Image Not found, content removed" << LOG_ENDL;
            emit thumbnailReady(image, thumbnailPath, false);
            return;
        }
    }
    // if file size > 5 MB or image file resolution greater than max supported resolution
    // i.e., PHOTO_MAX_SUPPORTED_WIDTH * PHOTO_MAX_SUPPORTED_HEIGHT, then ignore that image
    if( imageFileSize <= PHOTO_MAX_SUPPORTED_SIZE && imageFileSize > 0 )
    {
        // Used media library to read header for resolution
        MediaInfoLib::MediaInfo MI;
        MI.Open(validImageName.toStdWString());
        int imageWidth = QString::fromStdWString(MI.Get(MediaInfoLib::Stream_Image, 0, _T("Height"))).toInt();
        int imageHeight = QString::fromStdWString(MI.Get(MediaInfoLib::Stream_Image, 0, _T("Width"))).toInt();
        MI.Close();
        // THUMBNAILER_LOG() << "width" << imageWidth << "height" << imageHeight;
        // Image header is not safe always read from image if has invalid data
        if( ( ( imageWidth * imageHeight ) <= ( PHOTO_MAX_SUPPORTED_HEIGHT * PHOTO_MAX_SUPPORTED_WIDTH) )
                || imageWidth == 0 || imageHeight == 0 )
        {
            QImage img(validImageName);
            if( ( ( img.height() * img.width() ) <= ( PHOTO_MAX_SUPPORTED_HEIGHT * PHOTO_MAX_SUPPORTED_WIDTH ) )
                    && ( img.height() !=0 ) && ( img.width() != 0) ) // modified by ravikanth 08-05-13
            {
                img = img.scaled(QSize(212, 150), Qt::KeepAspectRatioByExpanding, Qt::FastTransformation);
                writeImageDataMutex.lock();
                if( img.save(thumbnailPath, "PNG", -1))  //pointed by honggi.shin, 2014.05.23, for finishing thumbnail operation.
                {
                    m_thumbnailerCache.append(thumbnailPath);
                } else {
                    THUMBNAILER_LOG << "Cannot save thumbnail image: " << thumbnailPath << LOG_ENDL;
                }
                writeImageDataMutex.unlock();
            }
            else
            {
                unsupported = true;
            }
        }
        else
        {
            unsupported = true;
        }
    }
    else
    {
        unsupported = true;
    }

    THUMBNAILER_LOG << "Thumbnail Generation complete..." << LOG_ENDL; //added by honggi.shin, 2014.05.23
    emit thumbnailReady(image, thumbnailPath, unsupported);
}
// modified by Dmitry 23.08.13

// modified on 18-02-14
const char* ThumbnailProcessor::relativeDir(const QString& image)
{
    QString filename = image;
    if(filename.startsWith("file://")) {
        filename = filename.remove(0, 7);
    }
    if(filename.startsWith("/usr_data/media/Picture/"))
    {
        return lThumbnailsDirJB;
    }
    else if(filename.startsWith("/rw_data/media/front_usb/"))
    {
        return lThumbnailsDirUSB1;
    }
    else if(filename.startsWith("/rw_data/media/rear_usb/"))
    {
        return lThumbnailsDirUSB2;
    }
    else
    {
        return "";
    }
}
