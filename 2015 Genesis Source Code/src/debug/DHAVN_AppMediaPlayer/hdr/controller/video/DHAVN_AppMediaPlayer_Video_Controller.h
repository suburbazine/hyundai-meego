/** @file DHAVN_AppMediaPlayer_Video_Controller.h
 *
 * This module is responsible for video playback backend. The interface and
 * implementaiton should not depend on the UI neither directly nor
 * indirectly. The class users can view the controller interface as an a
 * level of abstraction on top of LGE-provided controllers.
 * */
#if !defined( DHAVN_APPMEDIAPLAYER_VIDEO_CONTROLLER_H )
#define DHAVN_APPMEDIAPLAYER_VIDEO_CONTROLLER_H

#include <QObject>
#include "controller/DHAVN_AppMediaPlayer_ControllerBase.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h"
#include <DHAVN_AppSettings_Shared.h>
#include <DHAVN_TrackerAbstractor.h>
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h" // added by Sergey 17.10.2013 for ITS#195182

#define TA_REQUID(X) (X + m_mediaSource)
enum
{
    REQUID_VIDEO_ID  = 1000,
    REQUID_RESCAN_ID = 1100
};

// { moved by Sergey 25.10.2013 for new DivX
enum DIVX_STATE
{
    DIVX_STATE_REGISTRATION_CODE,
    DIVX_STATE_ALREADY_REGISTERED,
    DIVX_STATE_DEREGISTRATION_CODE,

    DIVX_STATE_MAX
};
// } moved by Sergey 25.10.2013 for new DivX

class AppMediaPlayer_Video_ControllerBase;
class AppMediaPlayer_Video_ControllerFS;
class AppMediaPlayer_Video_ControllerDVD;
class AppMediaPlayer_Video_ControllerVCD;
class AppMediaPlayer_EngineVideo;
class AppMediaPlayer_VideoSettings; // modified by ravikanth 06-03-13
class AppMediaPlayer_Video_CModel_FS; // added by kihyung 2013.07.19 for ITS 0179774

typedef AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T  PLAYBACK_STATUS_T;

/** @class AppMediaPlayer_Video_Controller
 * */
class AppMediaPlayer_Video_Controller : public AppMediaPlayer_Controller_Base
{
    Q_OBJECT

    friend class AppMediaPlayer_SettingsStorage;

public:

    AppMediaPlayer_Video_Controller( AppMediaPlayer_EngineMain &engineMain,
                                     AppMediaPlayer_EngineVideo *videoEngine );
    ~AppMediaPlayer_Video_Controller();

    void setSource( MEDIA_SOURCE_T source, QString path = QString("") );

    void stop();
    void stopAux(); // added by kihyung 2013.2.3
    void pause();
    void next();
    void prev();
    void fastForward();
    void fastForwardCritical();
    void rewind();
    void rewindCritical();
    void cancel_ff_rew();
    void play();
    void playAux(); // added by kihyung 2013.2.3
    void flickNext();
    void flickPrev();
    bool PrevNextOSD(bool vOSD);// added by yongkyun.lee 20130320 for : NO CR ff/rew OSD
    PLAYBACK_STATUS_T getPlaybackStatus();
    bool getLoadingStatus(); // added by lssanh 2013.07.02 ITS176637
	// { modified by ravikanth 27-12-12
    void setDarkness( DISPLAY_PLANES_T display, int value );
    void setColor( DISPLAY_PLANES_T display, int value );
    void setSaturation( DISPLAY_PLANES_T display, int value );
    void setBrightness( DISPLAY_PLANES_T display, int value );
	// } modified by ravikanth 27-12-12
    bool getCopyingStatus(void) { return m_isCopying; }; // added by wspark 2012.08.21 for DQA #47
    void setCopyingStatus(bool status) { m_isCopying = status; }; // added by wspark 2012.08.21 for DQA #47
    MEDIA_SOURCE_T getSource() { return m_mediaSource;}
    MEDIA_SOURCE_T getLastSource()
    {
        MP_LOG << "last source =" << m_LastMediaSource << LOG_ENDL;
        return m_LastMediaSource;
    }
    int PlayPosition();// added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD   
    bool IsPlaying();// modified by yongkyun.lee 2013-09-05 for : ITS 188359

    // { modified by Sergey for CR#15775
    void requestVideoInfo(); // added by kihyung 2013.2.14
    void clearDisplay(bool aOn); 
    void showVideo(DISPLAY_T aDisplay); // modified by Sergey for ITS#181872 30.07.2013
    // removed by Sergey 31.07.2013
    void moveWidget(DISPLAY_T aDisplay);
    void showSmallWidget(); //added by Sergey for CR#16002
    void changeWidgetDisplay(DISPLAY_T aDisplay); // added by Sergey 26.05.2013

    void SetVideoWidgetSize(int ax, int ay, int aw, int ah);
    void SetVideoSizeWrtDisplay(int ax, int ay, int aw, int ah);

    void SetAspectRatio ( ASPECT_RATIO_T aspectRatio );
    // { added by jeeeun 2013.01.04 for PAR implementation
    ASPECT_RATIO_T getAspectRatio(); 
    void SetActualRect(QRect aRect); 
    QRect getActualRect(){return m_actualRect;}
    void SetPAR(float aPar); 
    float getPAR(){return m_par;}
    void SetDAR(float aDar); 
    float getDAR(){return m_dar;}
    QRect getRectByPAR(float aPar, float aDar, int aWidth, int aHeight);
    void updateAspectRatioTable(ASPECT_RATIO_T aType, QRect aRect); 
    // } added by jeeeun 2013.01.04 for PAR implementation
    void updateAspectRatio( VideoWidgetProps *props );	

    AppMediaPlayer_Video_ControllerBase *getCurrentController(){ return m_pControllerCurrent; }
    AppMediaPlayer_Video_ControllerBase * getController( MEDIA_SOURCE_T mediaSource );
    CTrackerAbstractor *GetTrackerAbstractor();
        
    void releaseSource();

    // removed by Sergey 28.05.2013

    bool isSeekMode(); //added by junam 2012.10.18 for multiple seek command
    // } modified by Sergey for CR#15775
    // { modified by cychoi 2013.06.29 for ITS 175967 No tune wheel in PBC menu
    bool tuneWheel(bool direction);
    //void tuneWheel(bool direction); //Added by Radhakrushna 20120712 CR14111
    // } modified by cychoi 2013.06.29
    void cancelTune();//Added by Radhakrushna 20120712 CR14111
    bool selectTuned();//Added by Radhakrushna 20120712 CR14111
    bool checkCurrentFileSupported(const QString &filePath); // added by jeeeun 2013.01.14 for 68428
    void sendClearTpMessage() { emit clearTpMessage(); } // added by wspark 2013.04.17
    void SetSystemMute();//added by edo.lee 2013.06.23

    // { added by kihyung 2013.07.19 for ITS 0179774
    void handleUSBPluggedIn(QString path, bool isAutoPlay, USB_PORT_T usbPort, QString UUID);
    void handleUSBPluggedOut(MEDIA_SOURCE_T removedSource);
    // } added by kihyung 2013.07.19 for ITS 0179774

	// { added by Sergey 30.07.2013
    registrationState drmRegistrationStatus();
    void recieveRegistrationCode();
    void recieveDeregistrationCode();
    QString drmRegistrationCode() { return m_drmRegistrationCode; }
    QString drmDeregistrationCode() { return m_drmDeregistrationCode; }
    // } added by Sergey 30.07.2013
    
	void pauseByCallOnPowerOff();//added by edo.lee 2013.08.01 bt call on pwroff
	void playFromFileManager(); // added by Sergey 11.08.2013

    //{ added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
    void sendTrackClusterChanged(int index, int quantity) { emit trackClusterChanged(index, quantity); }
    void sendMediaInfoClusterChanged(QString title) { emit mediaInfoClusterChanged(title); }
    //} added by cychoi 2013.08.17
	
	void setDiscSource( MEDIA_SOURCE_T source);    // modified by yongkyun.lee 2013-09-05 for : ITS 188359
	bool isFSSource(MEDIA_SOURCE_T source); // added by Sergey 16.10.2013 for ITS#195185

    bool IsFFRewState(); // added by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
	void updateCaptionSizeInModels(AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_T nFontSize); // added by Sergey 17.10.2013 for ITS#195182
	void updateEnableSubtitleInModels(bool bOn); // added by Sergey 17.10.2013 for ITS#195182
	void setBackground(bool on); // added by Sergey 24.10.2013 for ITS#87134
	// { added by Sergey 25.10.2013 for new DivX
	DIVX_STATE divxScreenMode() { return m_divxScreenMode; }
	void setDivxScreenMode(DIVX_STATE mode);
	void loadDivxScreenMode(DIVX_STATE mode);
	// } added by Sergey 25.10.2013 for new DivX
	bool getClearDisplay() { return m_bClearDisplay; } // added by Sergey 24.12.2013 for ITS#217101
	
signals:
    void filenameChanged(QString);
    void foldernameChanged(QString name);
    void fullfoldernameChanged(QString);
    void playlistLengthChanged(int count);
    void playlistPositionChanged(int count);
    // { added by Sergey Vetugov for CR#13601
    void dvdScreenChanged(int);
    // } added by Sergey Vetugov for CR#13601
    // removed by Sergey for CR#15771
    void UpdateStateTrackOSD(int speed); // added by yungi 2012.11.14 for ISV 62667,ISV 62596
	// { modified by ravikanth 27-12-12
    void contrastChanged(int value);
    void hueChanged(int value);
    void saturationChanged(int value);
    void brightnessChanged(int value);
	// } modified by ravikanth 27-12-12

    void playAuxOnFG(); // added by radhakrushna 2013.03.11 for ISV 74997
    void onAuxStateChanged(int); // added by kihyung 2013.06.27
    void tuneTimerExpired(); // added ITS 188647
    void sourceReleased(); // added by Sergey 14.08.2013 for ITS#184081

public slots:
    Q_INVOKABLE void onReleaseResources( MEDIAPLAYER_RESOURCES_T resources );
    Q_INVOKABLE void onResourceGranted();

    //Slots from models
    void onFilenameChanged(QString filename);
    void onFoldernameChanged(QString foldername);
    void onFullFolderNameChanged(QString fullfolderename);
    void onChapterChanged(int chapter);
    void onTitleChanged(int title);
    void onPlaylistLengthChanged(int count);
    void onPlaylistPositionChanged(int count);
	int getOSDDevice(MEDIA_SOURCE_T media);
    // removed by Sergey for CR#15771

// { modified by Sergey for CR#15775
private slots:
    virtual void onError(int);
    virtual void onPositionChanged(int);
    virtual void onDurationChanged(int duration);
    virtual void onTrackChanged(int index);
    virtual void onStateChanged(int);
    virtual void onModeChanged(int);
    void connectCModels();
    void HandleAuxStateChanged(int nState);  // added by kihyung 2013.06.27

    void onDeviceAttached( MEDIA_SOURCE_T, bool );
    void onDeviceRemoved( MEDIA_SOURCE_T );
    // removed by Sergey 30.04.2013 for camera delay issue

	void onMuteTimerExpired();//added by edo.lee 2013.04.21  for Mute

    void HandleReqComplete(QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID);
    void HandleReqComplete(QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType, uint reqUID);
    void onOverplaneChanged(); // added by Sergey 15.10.2013 for ITS#194932

private:
	// removed by Sergey for ITS#181872 30.07.2013

    // { added by kihyung 2013.07.19 for ITS 0179774
    void ResetExtendDeviceInfo(); 
    QList<AppMediaPlayer_Video_CModel_FS*> GetExtendDeviceHistory(); 
    void CopyHistory(AppMediaPlayer_Video_CModel_FS *pDst, AppMediaPlayer_Video_CModel_FS *pSrc); 
    void CopyHistoryFromUBSExtended(AppMediaPlayer_Video_CModel_FS *pDst,  AppMediaPlayer_Video_CModel_FS *pSrc, QString mountPath);
    void CopyHistoryToUSBExtended(AppMediaPlayer_Video_CModel_FS *pDst, AppMediaPlayer_Video_CModel_FS *pSrc);    
    // } added by kihyung 2013.07.19 for ITS 0179774
	// removed by Sergey 16.10.2013 for ITS#195185


    MEDIA_SOURCE_T                        m_mediaSource;
    MEDIA_SOURCE_T                        m_pendingSource;
    MEDIA_SOURCE_T                        m_LastMediaSource;
    IMediaInterface                      *m_pIMedia;
    QWidget                              *m_pVideoWidget;

    bool                                  m_isResourcesOwned;
    AppMediaPlayer_Video_ControllerBase  *m_pControllerCurrent;
    // { added by Sergey 17.10.2013 for ITS#195182
    AppMediaPlayer_Video_ControllerFS    *m_pJBCtrl;
    AppMediaPlayer_Video_ControllerFS    *m_pUSB1Ctrl;
    AppMediaPlayer_Video_ControllerFS    *m_pUSB2Ctrl;
	// } added by Sergey 17.10.2013 for ITS#195182
	
    AppMediaPlayer_EngineVideo           *m_pEngineVideo;
    GPlayerEngine                        *m_pGPlayerEngine;
    // removed by Sergey for CR#16815
    ASPECT_RATIO_T                        m_currAspectRatio;
    QRect                                 m_smallWidgetRect; //added by Sergey for CR#16002
    // { added by jeeeun 2013.01.04 for PAR implementation
    QRect                                 m_actualRect;
    float                                 m_par;    
    float                                 m_dar;    
    // } added by jeeeun 2013.01.04 for PAR implementation
    bool                                  m_isCopying; // added by wspark 2012.08.21 for DQA #47
	bool                                  m_TuneInProgress; //Added by Radhakrushna 20120712 CR14111
    bool                                  m_isFgReceived; // modified by kihyung 2013.1.26

    CTrackerAbstractor                   *m_pTrackerAbs;

    QHash <MEDIA_SOURCE_T, AppMediaPlayer_Video_ControllerBase*>  m_tableOfControllers;
    QHash <ASPECT_RATIO_T,QRect>           m_tableAspectRatio;

    void doSetSource( MEDIA_SOURCE_T source );
    void setAUXSource(MEDIA_SOURCE_T source);
    AppMediaPlayer_VideoSettings* m_pSettingsStorage; // modified by ravikanth 06-03-13
    QTimer *m_pMuteTimer;//added by edo.lee 2013.04.21 for Mute
    bool m_isLongPress;

    QList<AppMediaPlayer_Video_CModel_FS*>  m_usbDevicesHistory; // added by kihyung 2013.07.19 for ITS 0179774

	// removed by Sergey for ITS#181872 30.07.2013
	QString         m_drmRegistrationCode;
    QString         m_drmDeregistrationCode; // added by Sergey 30.07.2013

    bool m_bClearDisplay; // added by Sergey 15.10.2013 for ITS#194932
    DIVX_STATE m_divxScreenMode; // added by Sergey 25.10.2013 for new DivX
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_CONTROLLER_H
