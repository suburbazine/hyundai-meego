// { modified by Sergey for CR16250
#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_CONTROLLERFS_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_CONTROLLERFS_H

#include <QObject>
#include "controller/DHAVN_AppMediaPlayer_Controllers.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerBase.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"
#include <DHAVN_FileOperationsHelper.h>
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h"//added by edo.lee 2013.02.19

class AppMediaPlayer_Video_CModel_Base;
class AppMediaPlayer_Video_CModel_FS;

class AppMediaPlayer_Video_ControllerFS : public QObject, public AppMediaPlayer_Video_ControllerBase
{
    Q_OBJECT
public:
    enum VIDEOINFO_TYPE_T
    {
        VIDEOINFO_TYPE_NORMAL,
        VIDEOINFO_TYPE_DIVX,
        VIDEOINFO_TYPE_AVI,
        VIDEOINFO_TYPE_MAX
    };

    enum FSERROR_TYPE_T
    {
        FSERROR_TYPE_UNSUPPORTED,
        FSERROR_TYPE_UNSUPPORTED_ALL,
        FSERROR_TYPE_DRM_STATUS,
        FSERROR_TYPE_DRM_EXPIRED,
        FSERROR_TYPE_DRM_NOT_AUTH,
        FSERROR_TYPE_AUDIO_FORMAT_UNSUPPORTED,
        FSERROR_TYPE_RESOLUTION_UNSUPPORTED, // added by Sergey 10.11.2013 new DivX popups
        FSERROR_TYPE_MAX
    };

    virtual void dropControllers();
    virtual void attachControllers();
    virtual AppMediaPlayer_Video_CModel_Base *getModel();
    AppMediaPlayer_Video_CModel_FS* getFSModel() { return m_pModel; }

    virtual void setAudioPath();
    virtual bool tuneWheel(bool bTuneForward);  // modified by kihyung 2012.07.17. add return value.
    virtual bool selectTuned(); // modified by junggil 2012.07.05 for CR11261 : Not able to search video files using CCP/RRC jogdial
    virtual void cancelTune();
    virtual void rewind();
    virtual void rewindCritical();
    virtual void fastForward();
    virtual void fastForwardCritical();
    virtual void next();
    virtual void prev(bool bForce = 0); // modified by Sergey 28.05.2013
    virtual void flickPrev();
    virtual void flickNext();
    virtual bool subTitle(); // added by junggil 2012.08.22 for NEW UX 5.1.8.1 Caption
    virtual bool showSubTitle(bool bShow); // modified by kihyung 2013.07.10 for ITS 0173538 
    virtual void ShowCurrentModelToOSD();
	virtual void cancel_ff_rew(); // added by edo.lee 2013.10.12 pop noise

    void activateSource();
    void connectUSBSignals();
    void connectJBSignals();
    void onDeviceAttached();
    void onDeviceRemoved();

    /** @brief Plays the file passed.
     * This method is designed to be used by the list & FM features.
     * */
    void playFile( QString filepath, bool bFromFM, int position = 0 ); //modified by Sergey for CR#16015
    
    /** @brief Copy the current file with USB to Jukebox.
      * The file is copied to the root directory.
      * */
    void copyFileToJukebox();

    /** @brief Cancel copying the file with USB to Jukebox.
      * */
    void cancelCopyOperation();
    void overwriteCopyOperation(); // added by wspark 2012.08.21 for DQA #47
    
    AppMediaPlayer_Video_ControllerFS(AppMediaPlayer_Controllers *controllers,
                                      AppMediaPlayer_EngineVideo *engine,
                                      QString mediaRoot,
                                      MEDIA_SOURCE_T mediaSource,
                                      QObject *parent = NULL);
    ~AppMediaPlayer_Video_ControllerFS();

    void enableSubTitle( bool bEnabledSubtitle );
    void setFontSize( AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_T nFontSize );
    QString getRootPath();
    void saveFileInfoForRestore(const QString & newFile);
    void clearRestoreData();
    void checkRestoreData(int value);
    void setCopyFileName(QString fileName) { m_copyFileName = fileName; }; // added by wspark 2012.08.21 for DQA #47
    QString getCopyFileName() { return m_copyFileName; }; // added by wspark 2012.08.21 for DQA #47
	
	// removed by Sergey 23.05.2013
	void saveLastPlayedInfo();
	void restoreLastPlay();
    int getFilesCount() { return m_filesCount; };
    int getFilesSize() { return m_totalFilesSize; };
    void setCheckForPlayableFile(bool value); // added by Sergey for ScreenTransition
    bool checkIsFileSupported(const QString &filePath); // added by jeeeun 2013.01.14 for 68428
    void setTitleEnabled(); // added by wspark 2013.01.25 for ISV 66482

    void requestAllPlaylist(int nReqID); // modifyed by kihyung 2013.06.18
    void requestCurrFolderList(QString strFolderName);
    
    void UpdateAllPlaylistAfterScan(QStringList &fileList, bool bRescan); // modifyed by kihyung 2013.08.16 for ITS 0184724
    void UpdateCurrFolderList(QStringList &fileList);
    void confirmDRMPlay(); // added by Sergey for DRM 29.07.2013
    void playFromFileManager(); // added by Sergey 11.08.2013
    void setAudioLanguage(int item); // added by Sergey 07.09.2013 for ITS#185543
    void setSubtitleLanguage(int item); // added by Sergey 09.10.2013 for ITS#187939
    void startSkipTimer() { m_pSkipTimer->start(); } // added by Sergey 08.10.2013 for ISV#92307 
    void startLongBtnTimer() {m_pLongBtnTimer->start();}// added by edo.lee 2013.10.12 pop noise
     // { added by Sergey 19.10.2013 for ITS#196896
    QString fileNameFromPath(QString filePath);
    QString folderNameFromPath(QString filePath);
    int fileIndexFromPath(QString filePath);
     // } added by Sergey 19.10.2013 for ITS#196896
    QTimer* longBtnTimer() { return m_pLongBtnTimer; } // added by Sergey 26.10.2013 for ITS#197713

signals:

    /** @brief This signal emmited when when a file is successfully copied from Jukebox to USB.
      * */
    void operationCopyComplete();
    void operationCopyError( int type );
    void fsPlaybackError(AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_T error); //CR_8464
    void durationChanged( int duration );
    void cancelFfRew(); // added by yongkyun 2013.01.19 for ITS 150703
    // removed by Sergey 23.09.2013 for ITS#188498
    void showPopup(POPUP_TXT_BTN_TYPE_T type);
    // removed by sjhyun 2013.11.05 for ITS 206357

public slots:
    void skip(); // added by Sergey 23.05.2013

private slots:
    void onDurationChanged( int duration );
    void onTrackChanged( int trackNo );
    void onProgress( int index, int total, int copyPercentage );
    virtual void onError(int error);
    void onSubtitleChanged( QString strCaption );
    void onAudioLanguagesReceived(QStringList langCodeList, int *pStreamIdList, int nCurrStreamId );
    void onTotalTextLangReceived(QStringList langCodeList, int *pStreamIdList, int nCurrStreamId ); // added by Sergey 20.08.2013 for ITS#184640 
    void onVideoInfo(int type, int status, int width, int height, float par, float dar, QString fileName); // modified for ITS 224766 2014.02.12
	// removed by Sergey 07.09.2013 for ITS#185543
	// modified by ravikanth 07-07-13 for ITS 0178184
    void replaceFileRequest(QString file, int totalFilesForCopy); // added by wspark 2012.08.21 for DQA #47
    void onDRMInfo(int, int, int);
    void onCapacityError(int filesCount, qint64 filesSize); // added by wspark 2012.11.20 for CR15380
    void onFgBgStateChanged(bool value);
    void onSourceAvailableChanged(MEDIA_SOURCE_T, MEDIA_TYPE_T, bool);
    void setSourceAvailable(bool);
    // removed by Sergey 18.05.2013
    void onHandleXSubtitleChanged(uchar* buf, int bufSize); //XSubtitle added by jeeeun 2013.04.04
	// removed by Sergey for ITS#181872 30.07.2013
    void onFileSystemChanged(); // added by kihyung 2013.2.17
    void onRealSoundChanged();//added by edo.lee 2013.04.21 for Mute
    void onNormalPlay(); // added by Sergey 28.05.2013
    void OnTrackBufferred(int bPrev, int nTotalIdx, int nIdxInFolder, int nFolderSize); //added by Puneet 2013.07.03 ITS# 0177010
    void onAudioSinkError();
    void onAutoTrackChanged(int index);
    void onLongBtnTimerExpired();// added by edo.lee 2013.10.12 pop noise
    void stopFFRewTimer(); // added by Sergey13.10.13

private:
    bool isFilePlayable(QString path);
    void clearPlaylist();
    void updatePlaylist(QStringList &fileList);
    USBController* GetUSBController(); // added by eugeny.novikov 2012.12.10 for CR 16564 fastboot
    // { added by Sergey for CR#15771
    void onPlaylistChanged(bool bFromFM); 
    void setPlaylist(PlayList aPlaylist);
    void addToPlaylist(QString aFile);
    // } added by Sergey for CR#15771
    void updateFSFolderList(); // added by kihyung 2013.08.08 for ITS 0181967 
    void onMuteTimerExpired();//added by edo.lee 2013.04.21 for Mute
    void detachDrmSignals();
    QString                         m_mediaRoot; // root folder of device
    IUSBExMediaInterface           *m_pIMediaEx;  // added by kihyung 2013.08.08 for ITS 0181967 
    AppMediaPlayer_Video_CModel_FS *m_pModel;
    AppMediaPlayer_Controllers     *m_pControllers;
    AppMediaPlayer_EngineVideo     *m_pEngineVideo;
    JukeboxController              *m_pJukeboxController;
    CTrackerAbstractor             *m_pTrackerAbstractor;
    FileOperationsHelper           *m_pFileOperationHelper;
    MEDIA_SOURCE_T                  m_mediaSource;
    PlayList                        m_playlist;
    QStringList                     m_nameFilter;
    int                             m_tempTuneIndex;
    bool                            m_isForwardDirection;
    QString                         m_copyFileName; // added by wspark 2012.08.21 for DQA #47
    bool                            m_isSubTitle;
    QString                         m_isSubTitleString;	
    // QStringList                  m_curDirContent; // removed by kihyung 2013.10.08 for ITS 0194414
    QString                         m_tuneFile;
    int                             m_filesCount;
    int                             m_totalFilesSize;
    int                             m_attachedJukebox; // added by jeeeun 2013.01.05 for device dettached checking
    int                             m_attachedUSB; // added by jeeeun 2013.01.05 for device dettached checking
    AppMediaPlayer_Video_ScreenManager * m_screenManager;
    uchar* m_pBitmapBufAddr; // modified by ravikanth 04-04-13
    bool m_currentSkip;//added by edo.lee 2013.06.23
    QTimer                         *m_pAudioSinkTimer;
	QTimer                         *m_pLongBtnTimer;// added by edo.lee 2013.10.12 pop noise
	QString                         m_searchTitle;
	QTimer						   *m_pSkipTimer; // added by Sergey 08.10.2013 for ISV#92307
	bool                           m_bFF;// added by edo.lee 2013.10.12 pop noise
	bool                           m_bIsAutoTrackChanged;
	bool                           m_isSourceActivation; // added by Sergey 08.01.2014 for ITS#211412
// removed by Dmitry 13.10.13
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_CONTROLLERFS_H
// } modified by Sergey for CR16250
