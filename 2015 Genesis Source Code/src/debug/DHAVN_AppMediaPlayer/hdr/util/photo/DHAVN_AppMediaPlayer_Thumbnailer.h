#ifndef DHAVN_APPMEDIAPLAYER_THUMBNAILER_H
#define DHAVN_APPMEDIAPLAYER_THUMBNAILER_H

#include <QObject>
#include <QSize>
#include <QThread>
#include <QStringList>
#include <QHash>

//modified by honggi.shin, 2014.05.23, for changing thumbnail path
static const char* lThumbnailsDir = "/app/tracker/.thumbnails/photo_thumbnails/";
static const char* lThumbnailsDirJBox = "/app/tracker/.thumbnails/photo_thumbnails/jb/";
static const char* lThumbnailsDirUSB1 = "/app/tracker/.thumbnails/photo_thumbnails/usb1/";
static const char* lThumbnailsDirUSB2 = "/app/tracker/.thumbnails/photo_thumbnails/usb2/";

enum PHOTO_THUMBNAIL_MEDIA_LIST
{
   PHOTO_THUMBNAIL_MEDIA_JUKEBOX = 0,
   PHOTO_THUMBNAIL_MEDIA_USB_1,
   PHOTO_THUMBNAIL_MEDIA_USB_2,
   PHOTO_THUMBNAIL_MEDIA_MAX
};


class PhotoThumbnailProcessor;

class PhotoThumbnailer : public QObject
{
    Q_OBJECT
public:
    static PhotoThumbnailer* instance() {
        if(m_instance == 0)
        {
            m_instance = new PhotoThumbnailer();
        }
        return m_instance;
    }

    void createDirIfAbsent();
    void ClearThumbnailData(PHOTO_THUMBNAIL_MEDIA_LIST mediaSource);
    QString getThumbnail(const QString& image, PHOTO_THUMBNAIL_MEDIA_LIST mediaSource);
    void getThumbnail(const QString& image, const QString& thumbnailName, PHOTO_THUMBNAIL_MEDIA_LIST mediaSource);
    QString getThumbnailName(const QString& image);
    static void setReadyToPWOFF(bool value); //2014.05.23

public slots:
    void onThumbnailReady(const QString& image, const QString& thumbnailPath, const QString& thumbnailName);
    void onFileNotSupported(const QString& image, const QString& thumbnailName);

signals:
    void thumbnailReady(const QString& image, const QString& thumbnail);
    void processThumbnail(const QString& image, const QString& thumbnailName, int mediaSource);
    void fileNotSupported(const QString& image);

private:
    void formatThumbnailContent(PHOTO_THUMBNAIL_MEDIA_LIST mediaSource);

private:
    PhotoThumbnailer();
    ~PhotoThumbnailer();
    static PhotoThumbnailer* m_instance;
    PhotoThumbnailProcessor* m_processor;
    QThread m_thumbnailerThread;
    QStringList m_thumbnailsQueue;
    QHash<QString,QString> m_thumbnailsQueueHash;
    QHash<QString,PHOTO_THUMBNAIL_MEDIA_LIST> m_thumbnailsSourceHash;
    static bool m_readyToPWOFF; //2014.05.23
};

class PhotoThumbnailProcessor : public QObject
{
    Q_OBJECT
public:
    explicit PhotoThumbnailProcessor(QObject* parent = 0);
    ~PhotoThumbnailProcessor();

public slots:
    void processThumbnail(const QString& image, const QString& thumbnailFileName, int mediaSource);
    void fillThumbnailerCache();
    void createDirIfFormatted();

private:
    const char* relativeDir(int mediaSource);
    bool isFileSupported(const QString file_path);

signals:
    void thumbnailReady(const QString& image, const QString& thumbnailPath, const QString& thumbnailName);
    void fileNotSupported(const QString& image, const QString &thumbnailName);

private:
    QStringList m_thumbnailerCache;
};

#endif // DHAVN_APPMEDIAPLAYER_THUMBNAILER_H
