#ifndef DHAVN_COVERARTDATAPROVIDER_H
#define DHAVN_COVERARTDATAPROVIDER_H



/*
******************************************************************
*        COPYRIGHT (C) Teleca AB      *
*----------------------------------------------------------------*
* MODULE     :  DHAVN_CoverArtDataprovder                             *
*
* PROGRAMMER :  Mahesh Kumar                                   *
* DATE       :  03 October 2011                                    *
* VERSION    :  1.0                                              *
*
*----------------------------------------------------------------*
*                                                                *
* MODULE SUMMARY : This file defines class which interact with    *
*                  Tracker database using spaql query to retrieve media related    *
*                  information  (e.g Artist, Album, Genre etc)    *
*                                                                *
*----------------------------------------------------------------*
*
* MODIFICATION RECORDS :                                         *
*
* VERSION   DATE                     PROGRAMMER              COMMENT
******************************************************************
* 1.0       03 October 2011  Mahesh           Draft version
******************************************************************
*/

#include <QStringList>
#include <QVector>
#include "DHAVN_TrackerAbstractor_constants.h"
#include <QSize>
#include <thumbnailer.h>
#include <DHAVN_LogUtil.h>

#include <QtDBus/QDBusConnection>

//This Gstreamer code not in use
#if 0
#include <QGlib/Connect>
#include <QGlib/Error>
#include <QGst/Pipeline>
#include <QGst/ElementFactory>
#include <QGst/Bus>
#include <QGst/Message>
#include <QGst/Query>
#include <QGst/ClockTime>
#include <QGst/Event>
#include <QGst/Init>
#include <QGst/Pad>

#endif

using namespace Thumbnails;

/**
   * This class will handle the request from the CTrackerAbstractor class and
   * execute the CoverArt related sparql queries. This class exposes the signal that the CTrackerProvider
   * would be calling for any operation like receiving data
   */

class CCoverArtDataprovider : public QObject{
    Q_OBJECT

    USE_LOG_UTIL

public :
/**
           * Default Constructor
           */
CCoverArtDataprovider();
    /**
           * Default Destructor
           */
    ~CCoverArtDataprovider();
    bool RequestPhotoThumbnail(const QString &filePath);

    bool RequestPhotoThumbnail(const QStringList &filePath);

    bool GetMusicAlbumCoverArt(const QStringList &albumNameList , const QStringList &artistnameList);

    bool RequestVideoThumbnail(const QString &filePath);

    bool RequestVideoThumbnail(const QStringList &filePath);
    bool IsCoverArtExists(const QString &albumName, const QString &artistname);
    bool RequestCoverArtOnDemand(const QStringList &albumNameList ,
                                                        const QStringList &artistnameList,
                                                        const QStringList &filePaths);
    void CoverArtsFromIPOD(QStringList coverArts); // rollback

    QStringList InsertCoverArtInCache(const QStringList &albumNameList ,
                               const QStringList &artistnameList,
                               const QStringList &filePaths);
	
	void ResetIPODRequestComplete(); // added by eugene.seo 2013.04.06
    bool GetIPODRequestComplete(); // added by eugene.seo 2013.05.17
	void CanceliPodCoverArtRequest(); // added by eugene.seo 2013.05.17
    static  QString getIPodUID(QString filePath); //added by junam 2013.05.16 for coverart match

private:
    bool InitializeThumbNailer();

    bool CheckIfThumbnailPresent(QString filePath);
    QStringList RequestCoverArtFromCache(const QStringList &albumNameList ,
                                         const QStringList &artistnameList);
    void RequestCoverArtFromGracenote(const QStringList &albumNameList ,
                                             const QStringList &artistNameList);
    void RequestCoverArtFromTracker();
    void RequestCoverArtFromIPOD();
    void PrepareDataList();
	 //deleted by aettie 20131010 unused code
//added by aettie.ji 20130904 for gracenote logo
    //void RequestEmitSignalForGracenoteLogo();
    
    //Gstreamer code not in use
#if 0
    bool GetVideoThumbnails(QString VideoPath,QString &thumbnailPath);
    void GetMediaParams(const QString & uri);
    void type_found(QGst::ElementPtr &typefind);
    void OnNewPad (QGst::PadPtr parsepad );
#endif

signals:

    /**
       * Signal to notify CTrackerAbstractor class on query completion
       */
    void DataFetched(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId);

    void Error(int);

    void getCoverArt(QStringList albumList, QStringList urlList); //added by junam 2013.05.16 for coverart match

public slots:
    void Started();
    void Finished(int);
    void Thumbnail(QUrl,QUrl);
    void Error(QString,QUrl,QUrl);
    void Dequeued(QUrl);
    void onCoverArtsRetrived(QStringList coverArts);
private:

    void   HandleError(int nError);

private:
    Thumbnailer* m_ThumbNailer;
    QHash<QString, QString> m_ThumnailsMimes;
    QVector <QStringList> m_ThumbnailerInfo;
#if 0
    QString m_fileType;
    QString m_videoType;
#endif


    enum Ethumbnail_type{
        eImage = 1,
        eImageList,
        eVideo,
        eVideoList,
        eInvalid
    };
    int gthumbnail_flag; // 1- Image  2 - Image List 3 - Video 4 - Video List

    QList<int> m_IndexList;
    QStringList m_CoverArtList;
    QStringList m_FilePaths;
    QStringList m_AlbumList;
    QStringList m_ArtistList;
    QDBusConnection m_Connection;
    QStringList m_AlbumListForIPOD;
    //{changed by junam 2013.05.16 for coverart match
    //QStringList m_ArtistListForIPOD;
    QStringList m_UrlListForIPOD;
    //}changed by junam
    bool m_IfIPODRequest;
    bool m_IsIPODRequestComplete;
};

#endif // DHAVN_COVERARTDATAPROVIDER_H
