#if !defined( APP_MEDIA_PLAYER_CONTROLLER_BASE_H )
#define APP_MEDIA_PLAYER_CONTROLLER_BASE_H

#include "controller/DHAVN_AppMediaPlayer_Controllers.h"
#include "util/DHAVN_AppMediaPlayer_MediaManager.h"
#define GET_MODE_ENGINE(T, MODE) ((T *)GetEngineMain().GetEngine(MODE))


enum OSD_EVENT_COMMON
{
    OSD_EVENT_NO_PLAYABLE_FILE,
    OSD_EVENT_VIDEOFILE_IS_CORRUPTED,
    OSD_EVENT_AUX_EXTERNAL_DEVICE_CONNECTED,
    OSD_EVENT_UNSUPPORTED_ACTION,
    OSD_EVENT_IPOD_SENDINGTAGS,
    OSD_EVENT_IPOD_MEMORYFULL,
    OSD_EVENT_IPOD_SUCCESS,
    OSD_EVENT_IPOD_FAILED,
    OSD_EVENT_IPOD_READING,
    OSD_EVENT_IPOD_DISCONNECTED,
    OSD_EVENT_AUDIO_TRACK_INFO,
    OSD_EVENT_VIDEO_AV_OFF,
    OSD_EVENT_AUDIO_AV_OFF,
    OSD_EVENT_COPY_COMPLETED,
    OSD_EVENT_MOVE_COMPLETED,
    // { added by kihyung 2012.07.09 for CR 10343
    OSD_EVENT_IPOD_UNSUPPORTED,
    OSD_EVENT_IPOD_CONNECTION_ERROR,
    // } added by kihyung
    OSD_EVENT_ADISC_EJECTED, //Add by changjin 2012.07.26 for CR11681
    OSD_EVENT_NO_INSERTED_DISC, // added by kihyung 2012.07.27 for ITS 0136589 
    OSD_EVENT_DELETE_COMPLETED, // added by wspark 2012.08.30 for CR13375
    OSD_EVENT_JUKEBOX_READING, // added by lssanh 2012.10.03 for sanity AJ009
    OSD_EVENT_COMMON_MAX, //Add by changjin 2012.07.26 for CR11681
    OSD_EVENT_AUX_VIDEO_INFO, // added by yungi 2012.12.14 for CR16059
    OSD_EVENT_AUX_AUDIO_INFO, // added by yungi 2012.12.14 for CR16059
    OSD_EVENT_AUX_DISCONNECTED, // added by lssanh 2013.03.03 Sanity AF006
    OSD_EVENT_AUX_CONNECTED, //added by edo.lee 2013.03.07
    OSD_EVENT_BT_UNSUPPORTED,	//added by hyochang.ryu 20130704
    OSD_EVENT_IPOD_UNSUPPORTED_ACTION, //added by junam 2013.08.27
    OSD_EVENT_IPOD_UNSUPPORTED_ACTION_FG, //added for ITS 219160
    OSD_EVENT_IPOD_EMPTY_TITLE
};

class AppMediaPlayer_EngineMain;

class AppMediaPlayer_Controller_Base : public QObject
{
    Q_OBJECT
public:

    AppMediaPlayer_Controller_Base( AppMediaPlayer_EngineMain &engineMain );

    virtual ~AppMediaPlayer_Controller_Base();

    AppMediaPlayer_Controllers & GetControllers() { return m_Controllers; }
    AppMediaPlayer_EngineMain & GetEngineMain() { return m_EngineMain; }

signals:
Q_SIGNALS:

    void positionChanged(int position);         // seconds
    void durationChanged(int duration);         // seconds
    // { modified by wspark 2013.04.09
    //void positionCleared(); // added by wspark 2013.03.30 for ISV 77359
    void positionCleared(bool value);
    // } modified by wspark
    void clearTpMessage(); // added by wspark 2013.04.09 for cluster IOT #37
    void playListChanged(); // added by wspark 2013.04.21 for cluster IOT
    void trackChanged(int index, int quantity); // modified by cychoi 2013.08.17 for HMC request - Cluster display = OSD display // track no and total number // zero-based index and total number
    void trackClusterChanged(int index, int quantity, bool bResetPosition = true); // added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display // track no and total number
    void mediaInfoChanged( int     device, // added by kihyung 2013.08.08 for ITS 0183028 
                           QString title,
                           QString album,
                           QString artist,
                           QString genre,
                           QString cover,
                           QString composer, // add by junggil 2012.06.29 for CR08958 : Composer info is not displayed on activating more info in Basic play back screen
                           QString folder, // added by junggil 2012.08.02 for CR12564
                           QString filename);
    void coverFlowMediaInfoChanged(int     device, // added by kihyung 2013.08.21 for ITS 0183028 
                                   QString title,
                                   QString album,
                                   QString artist,
                                   QString cover);
	//added by edo.lee 2013.05.11
	void mediaInfoTempChanged( QString title,
                           QString album,
                           QString artist,
                           QString genre,
                           QString cover,
                           QString composer, 
                           QString folder, 
                           QString filename);

    void currPlayingItemPosChanged(QString album, QString artist, QString file); // added by kihyung 2013.09.13 for ISV 89552
    
    // { added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
    void mediaInfoClusterChanged(QString title);
    // } added by cychoi 2013.08.17
	
	void btMusicInfoChanged( QString title, QString album, QString artist, QString count ); //added by eunkoo 2012.07.26 for CR11898
    void deviceChanged(int deviceId);           // MEDIA_SOURCE_T
    void stateChanged(int state);               // PLAYER_STATE
    void updateOSD(unsigned int nEventType);
    // { removed by junam 2012.11.28 for CR14466
    //void cancelUpdateOSD(bool cancelMode); //added by changjin 2012.10.22 for CR 14445
    // } removed by junam
    void contentSize(MEDIA_SOURCE_T source, MEDIA_TYPE_T type, int count);
    // { modified by yongkyun.lee@lge.com 2012.07.14
    //void playingModeChanged( int nRandom, int nRepeat, int nScan );
    void playingModeChanged( int nRandom, int nRepeat, int nScan , bool nNoOSD );
    // } modified by yongkyun.lee@lge.com
    void playingModeChanged(MEDIA_MODE mode);

    //{added by junam 2013.03.26 for ISV72425
    //void tuneSearch( QString m_coverart, QString m_artist, QString m_song, QString m_album, QString m_folder, QString m_genre, QString m_composer, QString m_countInfo , bool m_samefile );
    void tuneSearch( QString m_coverart, QString m_artist, QString m_song, QString m_album, QString m_folder, QString m_countInfo , bool m_samefile );
    //}added by junam

    void moreLikeThisCluster();
    void updateMP3CDFileCount(); // added by kihyung 2013.4.9 ISV 77100
    // { modified by wspark 2013.04.30 for cluster IOT
    //void sendTpMessage(QString); // added by wspark 2013.04.09 for cluster IOT #34
    void sendTpMessage(QString, int);
    // } modified by wspark

public slots:
    // Generic virtual slots used for almost all controllers.
    /// @{ predefined_connections
    virtual void onPositionChanged(int){}
    virtual void onStateChanged(int){}
    virtual void onTrackChanged(int){}
    virtual void onMediaInfoChanged(MediaInfo*){}
    virtual void onDurationChanged(int){}
    virtual void onModeChanged(int){}
    virtual void onError(int){}
    /// @}

protected:
    /** @brief Common method to connect to the low-level controllers signals.
     * While the connection/reconnections are duplicated among video & audio
     * controllers, let's move it here.
     * Please note, that this method declared as virtual. Feel free to
     * override it to add additional initialization specific to the
     * controller, but don't forget to call this one to establish the
     * connectsion from the @ref predefined_connections group.
     * @param aSource The media source to acquire.
     * */
    virtual void AcquireMediaSource( MEDIA_SOURCE_T aSource );

    /** @brief Common method to disconnect from the low-level controllers.
     * Reverts the actions of AcquireMediaSource(). See it's documentation
     * for more details.
     * @param aSource The media source to release.
     * */
    virtual void ReleaseMediaSource( MEDIA_SOURCE_T aSource );

private:
    AppMediaPlayer_EngineMain  &m_EngineMain;
    AppMediaPlayer_Controllers &m_Controllers;
    // QRPMClient              *m_pRPMClient; // removed by kihyung 2012.11.03 for booting disable
    DeckController             *m_pDeckController;
    CIpodController            *m_pIPodController;
};

#endif
