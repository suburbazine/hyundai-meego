#ifndef USBCONTROLLER_H
#define USBCONTROLLER_H

#include <QObject>
#include <QMutex>
#include <QTimer>
#include "iusbexmediainterface.h"
#include "iviewerinterface.h"
#include "ifileinterface.h"
#include "mediaplayer_def.h"
#include <QtDBus/QDBusInterface>
#include <qmediaplayer.h>

#define USE_USB_SINGLE_INSTANCE         0

struct MP_TRACK_INDEX_T
{
    unsigned int    nID                  : 31;
    unsigned int    bError               : 1;
    unsigned short  nFileIdxInFolder;
    unsigned short  nFolderIdx;
};

struct MP_FOLDER_INDEX_T
{
    unsigned int    nFileSize;
    unsigned int    nStartIdx;
    unsigned int    nErrorSize;
};

struct MP_FOLDER_INFO_T
{
    MP_FOLDER_INDEX_T   *pFolderIdx;
    MP_FOLDER_INDEX_T    tTotalFolderIdx;
    unsigned int         nFolderCount;
    unsigned int         nCurrFolderSize;
    unsigned int         nMaxFolderSize;
};

// added by kihyung 2013.12.12 for ITS 214770
struct MP_MEDIA_INFO_T
{
    MediaInfo            tInfo;
    int                  nTrackIdx;
    int                  bTrackChanged;
    int                  bValid;
};

class  IPlayerEngine;
class  GPlayerEngine;
class  MPlayerEngine;
class  FileManager;

class USBController : public QObject, public IUSBExMediaInterface, public IViewerInterface, public IFileInterface
{
    Q_OBJECT
public:
    #if USE_USB_SINGLE_INSTANCE
    static USBController*       GetInstance();
    #else
    static USBController*       GetFirstInstance();
    static USBController*       GetSecondInstance();
    static USBController*       GetThirdInstance();
    #endif

    ~USBController();

    IMediaInterface*            GetMediaInstance();
    IViewerInterface*           GetViewerInstance();
    IFileInterface*             GetFileInstance();

    //////////////////////////////////////////////////////////////////////////////
    // FOR IMediaInterface
    int                         ChangeMediaMode(MEDIA_PLAYER_MODE eMode);
    QString                     GetCurrentFileName();
    int                         GetCurrentIndex();
    void                        SetCurrentIndex(int nIndex);
    int                         GetCurrentMediaState();
    int                         GetCurrentMediaDuration();
    int                         GetCurrentMediaPosition();
    void                        SetCurrentMediaPosition(int position);
    int                         GetDeviceState();

    void                        GetCurrentMediaInfo(MediaInfo *pMediaInfo);
    void                        GetDeviceInfo(DeviceInfo *pDeviceInfo);
    void                        GetPlaylist(PlayList *pPlayList);

    void                        SetPlayList(PlayList *pPlaylist);
    bool                        SeekableMedia();

    void                        SetBrightness(double fBrightness);
    void                        SetContrast(double fContrast);
    void                        SetTint(double fTint);
    void                        SetBrightness(MEDIA_DISPLAY displayPlane, double fBrightness);
    void                        SetContrast(MEDIA_DISPLAY displayPlane, double fContrast);
    void                        SetTint(MEDIA_DISPLAY displayPlane, double fTint);
    
    double                      GetBrightness();
    double                      GetContrast();
    double                      GetTint();
    void                        SetSystemState(int sysState);
    QWidget*                    GetVideoWidget();
    void                        EnableSubTitle(bool bEnable);
    void                        SetPlayerEngine(PLAYER_ENGINE_TYPE eType);
    void                        SetFontSize(const int fontSize);
    bool                        ClearDisplay(bool isBackground);
    void                        SetHue(double value);
    void                        SetHue(MEDIA_DISPLAY displayPlane, double value);
    double                      GetHue();
    void                        GetCurrentFileFolderIndex(int *pFileIdx, int *pTotalFiles);

    void                        AudioDragStart();
    void                        AudioDragEnd();    

    //////////////////////////////////////////////////////////////////////////////
    // FOR IUSBExMediaInterface
    void                        RequestAudioLanguages();
    void                        SetAudioLanguage(int nStreamId);
    int                         GetAudioLanguage(); // added by jeeeun 2013-01-07 for multi-audio
    bool                        IsFileSupported(const QString &strFileName);
    int                         IsFileSupportedAdvanced(const QString &strFileName);
    bool                        getIsFileSupportedCheck(); //added by jeeeun 2012-12-20 for file-check
    void                        setIsFileSupportedCheck(bool val); //added by jeeeun 2012-12-20 for file-check
    bool                        getIsSubtitleEnable(); // added by jeeeun 2012-01-24 for checking subtitle enable or disable
    void                        DrmConfirmPlay();
    void                        SetFSFolderInfo(QList<FSFolderInfo> &tFolderList); // added by kihyung for ITS 0181967 
    bool                        isDivXDRM(int *type, int *limitcnt, int *usecount);
    bool                        setSubtitleLanguage(int subId);
    int                         getCurrentSubtitleLanguage();
    void                        RequestSubtitles();
    //////////////////////////////////////////////////////////////////////////////
    // FOR IFileInterface
    int                         CreateFolder(const QString &createFolderName);
    int                         EditedFolder(const QString &strOldName, const QString &strNewName);
    int                         EditedFile(const QString &strOldName, const QString &strNewName);
    int                         DeleteFileFolder(const QString &absoluteFolderName, int typemode = -1, ETAVolumeType volume = eJukeBox);
    int                         MoveFolder(const QString &absoluteFolderName, const QString &destPath);
    int                         CopyFolder(const QString &absoluteFolderName, const QString &destPath);
    int                         CopyFolder(const QString &absoluteFolderName, const QString &destPath, int typemode, ETAVolumeType volume = eUSB);
    int                         DeleteFileList(const QStringList &strList, int typemode = -1, ETAVolumeType volume = eJukeBox);
    int                         MoveFileList(const QStringList &strList, const QString &destPath);
    int                         CopyFileList(const QStringList &strList, const QString &destPath);
    int                         CopyFileList(const QStringList &strList, const QString &destPath, int typemode, ETAVolumeType volume = eUSB);
    int                         TotalFileForCopy();
    int                         TotalFileForMove();
    int                         TotalFileForDeletion();
    qint64                      GetTotalCopySize();
    qint64                      GetTotalMoveSize();
    qint64                      GetTotalDeleteSize();
    qint64                      GetTotalDiscSpace(const QString &strVolumeName);
    qint64                      GetTotalUsedDiscSpace(const QString &strVolumeName);
    int                         StartDiscFormat(const QString &strVolumeName);
    int                         CancelCopyOperation();
    int                         CancelDeleteOperation();
    int                         CancelFormatOperation();    
    int                         SetOverWriteCurrentExistFileMode(int event);
    int                         ContinueCopying();
    int                         PauseCopying();
    int                         CancelMoveOperation();
    int                         ContinueMoving();
    int                         PauseMoving();

    // controller
    QMediaPlayer*               GetMediaPlayerInstance();
    void                        ConnectSignal();
    void                        DisconnectSignal();
    int                         IsBootWithUSB();
    int                         ContinueScan() { return CheckedScanMode();}
public slots:
    //////////////////////////////////////////////////////////////////////////////
    // FOR IMediaInterface
    void                        Play();
    void                        Pause();
    void                        FunctionStop();
    void                        Stop();
    void                        Next(int skiplevel);
    void                        Next(int skipLevel, int nAutoSkip); // added by kihyung 2013.11.13 for ITS 0208734 
    void                        Prev(int skiplevel, int nForced = 0);
    void                        Prev(int skiplevel, int nAutoSkip, int nForced); // added by kihyung 2013.11.13 for ITS 0208734    
    void                        Scan(int mode);
    void                        RandomPlay(int mode, int bPlay = true);
    void                        RepeatPlay(int mode, int bPlay = true);
    void                        FastForward(int rate);
    void                        Rewind(int rate);
    void                        NextFolder(PlayList *pPlayList);
    void                        PrevFolder(PlayList *pPlayList);

    //////////////////////////////////////////////////////////////////////////////
    // FOR IViewerInterface
    void                        StartSlideShow();
    void                        StopSlideShow();
    void                        Zoom(int level);
    void                        ZoomWithPinch(double level);
    void                        Rotate (int angle);
    void                        Scroll(int mode, int motion);
    void                        SaveImage(const QString &strOldName, const QString &strNewName, const QString & destPath);
    void                        SetSlideShowDuration(int);
    
    //////////////////////////////////////////////////////////////////////////////    
    // FOR Controller Handler
    void                        HandleUSBPositionChanged(int nPosition);
    void                        HandleUSBDurationChanged();
    void                        HandleUSBError(int nError);
    void                        HandleUSBStateChanged(int nState);
    void                        HandleTrackChange();
    void                        HandleUSBMediaStatusChanged(int nMediaStatus);
    void                        HandleUserReqTrack();
    void                        HandleVideoInfo(int type, int status, int width, int height, float par, float dar);
    void                        HandleCurrentMediaInfo(MediaInfo *pMediaInfo);
    void                        HandleRealSoundChanged();
    void                        HandlePlayTimer();  // added by sungha.choi 2013.07.01

    //////////////////////////////////////////////////////////////////////////////
    // FOR IFileInterface
    void                        HandleFileDeleted(int nIndex, qint64 nDeletedSize);
    void                        HandleFileDeleted2(int nIndex, qint64 nDeletedSize, QString strName);
    void                        HandleFileCopied(int nIndex, qint64 nCopiedSize);
    void                        HandleFileMoved(int nIndex, qint64 nMovedSize);
    void                        HandleFileCopiedPercentage(int nIndex, int nPercentage);
    void                        HandleFileMovedPercentage(int nIndex, int nPercentage);    
    void                        HandleCreateFileFinished(int bSuccess);
    void                        HandleRenameFinished(int bSuccess, int bFolder);
    void                        HandleDeleteFinished(int bSuccess, int nMode, int nVolume); // modified by kihyung 2013.08.28
    void                        HandleMoveFinished(int bSuccess, int nMode); // modified by kihyung 2013.08.28
    void                        HandleCopyFinished(int bSuccess, int nSkipCount, int nFilesCount, int nMode, int nVolume, int nCurrentReqMode, int nCurrentReqVolume);  // modified by kihyung 2013.08.28
    void                        HandleFormatFinished(int bSuccess);
    void                        HandleUsbAttached(QDBusObjectPath aDBUSObjectPath);
    void                        HandleUsbDetached(QDBusObjectPath aDBUSObjectPath);
    void                        HandleFMError(int nError, QString strFileName);

signals:
    //////////////////////////////////////////////////////////////////////////////
    // FOR IMediaInterface    
    void                        OnPositionChanged(int);
    void                        OnDurationChanged(int);
    void                        OnStateChanged(int);
    void                        OnError(int, QString filename = QString());
    void                        OnScanCompleted();
    void                        OnTrackChanged(int index); 
    void                        OnModeChanged(int index);
    void                        OnCheckedScanMode();
    void                        OnCurrentMediaInfo(MediaInfo *pMediaInfo);
    void                        OnCurrentMediaStatusChanged(int);
    void                        OnPositionImageInfo(int width, int height);
    void                        OnSubtitleChanged(QString strCaption);
    void                        HandleXSubtitleChanged(uchar*, int);
    void                        OnTotalAudioLangReceived(QStringList langCodeList, int *pStreamIdList, int nCurrStreamId);
    void                        OnTotalTextLangReceived(QStringList langCodeList, int *pStreamIdList, int nCurrStreamId);
//	void                                           OnDRMStatusUpdated(int nDRMStatus);
    void                        OnDRMStatusUpdated(int nDRMStatusMessage, int useLimit, int useCount); // modified by jeeeun 2012.11.14
    void                        OnPrivateSignal(int nType);
    void                        OnVideoInfo(int type, int status, int width, int height, float par, float dar, QString fileName);// modified for ITS 224766 2014.02.12
    void                        OnRealSoundChanged();
    void                        OnTrackBufferred(int bPrev, int nTotalIdx, int nIdxInFolder, int nFolderSize);
    void                        OnNormalPlay();
    void                        OnAutoTrackChanged(int index); // added by kihyung 2013.10.07
    
    //////////////////////////////////////////////////////////////////////////////
    // FOR IFileInterface
    void                        OnCreateFolder(int event); 
    void                        OnEditFolder(int event);
    void                        OnEditFile(int event);
    void                        OnTotalFileCopied(int event, int skipCount, int filesCount, int nMode = 0, int nVolume = eUSB,  int nCurrentReqMode = 0, int nCurrentReqVolume = eUSB); // modified by kihyung 2013.08.28  nMode is MEDIA_TYPE
    void                        OnTotalFileMoved(int event, int nMode = 0); // modified by kihyung 2013.08.28  nMode is MEDIA_TYPE
    void                        OnTotalFileDeleted(int event, int nMode = 0, int nVoluem = eJukeBox); // modified by kihyung 2013.08.28  nMode is MEDIA_TYPE
    void                        OnFormatFinished(int event);
    void                        OnPercentageAndSizeCopied(int percent, qint64 size, int copyCount); // modified by ravikanth 18-07-13 for copy count
    void                        OnPercentageAndSizeMoved(int percent, qint64 size);
    void                        OnFileCopiedPercentage(int nIndex, int nPercentage);
    void                        OnFileMovedPercentage(int nIndex, int nPercentage);
    void                        OnPercentageAndSizeDeleted(int percent, qint64 size);
    void                        OnPercentageAndSizeAndNameDeleted(int percent, qint64 nSize, QString strName); // modified by kihyung 2013.09.13 for ISV 91171
    void                        OnFileMoved(QString strName);

    //////////////////////////////////////////////////////////////////////////////
    // Signal about usb status
    void                        OnDeviceConnected();
    void                        OnDeviceDisConnected();
protected:
                                USBController(MEDIA_PLAYER_MODE eMode, QObject *parent = 0);
    FileManager*                GetFileManager();
    MPlayerEngine*              GetMPlayerEngine();
    int                         FindAttachedDevices();
    int                         CheckedScanMode();
    void                        SetMyMediaMode(MEDIA_PLAYER_MODE eMode);
    int                         GetMediaState(int EngineState);
    int                         IsLastTrack();
    int                         IsFirstTrack();
    int                         SetURI(int nTrackIdx, int bCalledPrevNext = false);
    int                         GetTrackIdx();
    int                         SetTrackIdx(int nTrackIdx);
    int                         GetNextTrackIdx();
    int                         GetPrevTrackIdx();
    int                         GetNextTrackIdx(int nSkiplevel, int nTrackIdx);
    int                         GetPrevTrackIdx(int nSkiplevel, int nTrackIdx);
    int                         GetRandomTrackIdx(int bNext);
    int                         GetFolderIdx(int nTrackIdx);
    int                         GetStartFileIdx(int nFolderIdx);
    int                         GetFolderSize();
    int                         GetFileSizeOfFolder(int nTrackIdx);
    int                         GetErrorSizeOfFolder(int nTrackIdx);
    void                        SetErrorSizeofFolder(int nTrackIdx, int nSize);
    int                         GetStartFileIdxOfFolder(int nTrackIdx);
    int                         GetFileIdxInFolder(int nTrackIdx);
    void                        SetShuffleList(int bAgain);
    void                        SetPlayableList(int bFolder = 1);
    bool                        RestoreFromShuffle(int nStartIdx, int nEndIdx);
    void                        Scan_Impl(int mode);
    void                        RandomPlay_Impl(int mode);
    void                        RepeatPlay_Impl(int mode);
    void                        PrintVersion();
    void                        EmitOnTrackChanged(int nTrackIdx, int bAuto = 0); // modified by kihyung 2013.10.07
    void                        EmitOnStateChanged(int nState);
    void                        EmitTrackBuffered(int bPrev, int nTotalIdx);
    void                        SetMute(int bMute);
    int                         GetMute();
    void                        ClearUserReqTrack();
    void                        CheckMediaInfoSignal();
    bool                        CheckUSBMounted();// added by jaehwan lee 2014.08.21 to fix ITS 245899, 245883, 245872

public:    
    IPlayerEngine           *m_pPlayer;

private:
    #if USE_USB_SINGLE_INSTANCE
    static USBController    *m_pInstance;
    #else
    static USBController    *m_pFirstInstance;
    static USBController    *m_pSecondInstance;
    static USBController    *m_pThirdInstance;
    #endif

    GPlayerEngine           *m_pGPlayer;
    MPlayerEngine           *m_pMPlayer;

    PLAYER_ENGINE_TYPE       m_eEngineMode;
    int                      m_bConnectFMSignal;

    QList<QString>           m_tPlayList;

    MP_TRACK_INDEX_T        *m_pShuffleList;
    int                      m_nShuffleStart;
    int                      m_nShuffleIdx;
    int                      m_nShuffleSize;
    int                      m_nPlayableSize;
    int                      m_nTotalErrSize;

    MP_FOLDER_INFO_T         m_tFolderInfo;

    QString                  m_playlistName;
    QString                  m_currentMediaFileName;
    int                      m_currentMediaState;
    int                      m_nEngineMediaState;
    double                   m_currentMediaPosition;
    int                      m_currentMediaIndex;
    int                      m_currentMediaDuration;
    int                      m_currentDeviceState;
    int                      m_slideShowDurationTime;
    int                      m_systemState;
    int                      m_brightness;
    int                      m_contrast;
    int                      m_tint;

    int                      m_nSetURITrackIndex;
    int                      m_nCurrTrackIndex;
    int                      m_bFirstPlay;
    int                      m_nDeviceMountAtBoot;
    
    int                      m_nRepeatMode;     // repeate play mode variable
    int                      m_nRandomMode;     // random play mode variable   
    int                      m_nScanMode;       // scan play mode variable
    int                      m_nEndScanIndex;
    bool                     m_bSldShowMode;    // slide show mode variable
    int                      m_nSldDuration;    // slide show duration time variable

    int                      m_bUserStop;
    int                      m_bUserPause;  // added by kihyung 2013.07.22 for ITS 0179477 
    int                      m_bPrevDirection;
    int                      m_nFFREWRate;
    int                      m_bREWMode;

    MEDIA_PLAYER_MODE        m_eMyMediaMode;
    int                      m_bConnectSignal;
    int                      m_bConnectGSignal;
    int                      m_bConnectMSignal;
    QDBusInterface          *m_pQDBus;
    QStringList              m_tUSBObjectPath;
    QStringList              m_tUSBMountPath;
    QStringList              m_tUSBSerialNumber;

    int                      m_bBootWithUSB;
    int                      m_nUserPlayPosition;
    int                      m_nPLCategory;
    int                      m_bControllerChanged;

    int                      m_nClearDisplay;
    int                      m_bEmitTrackChanged;

    int                      m_bPlayAfterFFREW;
    // int                      m_bisMLT; //added by jeeeun for MLT play in FF/REW 2012.12.05 // removed by kihyung 2013.07.18 for ITS 0180268

    int                      m_bMute;
    //int                    m_bFirstPlayAfterPlaylist; // removed by kihyung 2013.07.17

    QMutex                   m_tUserReqMutex;
    QList<int>               m_tUserReqTrackList;
    QTimer                   m_tUserReqTimer;
    QTimer                   m_tPlayTimer;   // added by sungha.choi 2013.07.01
    int                      m_nPrevForced;  // added by sungha.choi 2013.07.01


    // Video Info
    int                      m_nVideoType;
    int                      m_nVideoStatus;
    int                      m_nVideoWidth;
    int                      m_nVideoHeight;
    float                    m_nVideoPar;
    float                    m_nVideoDar;

    // for USB1, USB2
    int                      m_bUSB1Connect;
    int                      m_bUSB2Connect;

    int                      m_bSendMetaInfo;
    int                      m_bSkipPauseSignal;
    int                      m_bAutoTrackChanged;
    int                      m_bSystemRequestMute;
    int                      m_bBTCallStart;

    int                      m_bAudioDragStart;
    //remove by edo.lee 2013.10.04
    
    // int                   m_nRandomModeBeforeScan; // added by kihyung 2013.07.17 for ITS 0180420 // removed by kihyung 2013.09.17 for HMC new sepc.
    int                      divxDrmType;
    int                      divxDrmLimitCnt;
    int                      divxDrmUseCnt;
    int                      m_pDrmConfirmPlay;

    int                      m_bChangedDuration; // added by kihyung 2013.08.29 for ITS 0186278

    MP_MEDIA_INFO_T          m_tMediaInfo; // added by kihyung 2013.12.12 for ITS 214770                

	int						 m_bSameSong;
};  

#endif // USBCONTROLLER_H
