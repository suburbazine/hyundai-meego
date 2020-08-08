#ifndef DHAVN_APPFILEMANAGER_THUMBNAILER_H
#define DHAVN_APPFILEMANAGER_THUMBNAILER_H

#include <QObject>
#include <QSize>
#include <QThread>
#include <QStringList>
#include <QHash>
//#include "DHAVN_TrackerAbstractor.h"
#include "DHAVN_AppFileManager_Logger.h" //2014.05.23
#include <QTime> //2014.05.23

// modified by Dmitry 23.08.13
//static const char lThumbnailsDir[] = "/usr_data/thumbnails/"; // modified by Dmitry 17.08.13
// modified on 18-02-14

//modified by honggi.shin, 2014.05.23, change thumbnail path
static const char lThumbnailsDir[] = "/app/tracker/.thumbnails/pic_list_thumbnails/";
static const char lThumbnailsDirJB[] = "/app/tracker/.thumbnails/pic_list_thumbnails/jb/";
static const char lThumbnailsDirUSB1[] = "/app/tracker/.thumbnails/pic_list_thumbnails/usb1/";
static const char lThumbnailsDirUSB2[] = "/app/tracker/.thumbnails/pic_list_thumbnails/usb2/";

class ThumbnailProcessor;

class Thumbnailer : public QObject
{
    Q_OBJECT
public:
    static Thumbnailer* instance() {
        if(m_instance == 0)
        {
            m_instance = new Thumbnailer();
        }
        return m_instance;
    }
    void createDirIfAbsent(); // modified by ravikanth 05-04-13
    void generateThumbnail(const QString& image, int index, bool bFront);  // modified by sungha.choi 2013.09.05 for ITS 0188265
    QString getThumbnailPath(const QString& image);
    void clearQueue();
    static void setReadyToPWOFF(bool value); //2014.05.23

public slots:
    void onThumbnailReady(const QString& image, const QString& thumbnailPath, bool unsupported);
signals:
    void thumbnailReady(const QString& thumbnail, bool unsupported, int index, bool bFront); // modified by ravikanth 26-04-13  // modified by sungha.choi 2013.09.05
    void processThumbnail(const QString& image);

private:
    Thumbnailer();
    ~Thumbnailer();
    static Thumbnailer* m_instance;
    ThumbnailProcessor* m_processor;
    QThread m_thumbnailerThread;
    QStringList m_thumbnailsQueue;
    QHash<QString,int> m_thumbnailsQueueHash;
    QHash<QString,bool> m_thumbnailsFrontRearQueueHash;  // added by sungha.choi 2013.09.05 for ITS 0188265
    static bool m_readyToPWOFF; //2014.05.23
};

class ThumbnailProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ThumbnailProcessor(QObject* parent = 0);
    ~ThumbnailProcessor();

   QString getThumbnailPath(const QString& image);

public slots:
    void processThumbnail(const QString& image);
    void fillThumbnailerCache();
    void createDirIfFormatted(); // modified by ravikanth 05-04-13

private:
    const char* relativeDir(const QString& image); // modified on 18-02-14

signals:
    void thumbnailReady(const QString& image, const QString& thumbnailPath, bool unsupported);

private:
    QStringList m_thumbnailerCache;
};
// modified by Dmitry 23.08.13


#endif // DHAVN_APPFILEMANAGER_THUMBNAILER_H
