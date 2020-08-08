#include "controller/DHAVN_AppMediaPlayer_Controllers.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerVCD.h"
#include "QObject"
// { added by kihyung 2012.07.12
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h" 
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h"
// } added by kihyung



AppMediaPlayer_Video_ControllerVCD::AppMediaPlayer_Video_ControllerVCD( AppMediaPlayer_Controllers *controllers,
    AppMediaPlayer_EngineVideo *engine, // added by kihyung 2012.07.12
    QObject *parent )
    : QObject(parent),
      // { added by kihyung 2012.07.12
      m_pDeckController(NULL),
      m_pIVideo(NULL),
      m_pEngineVideo(engine),
      // } added by kihyung
      m_pControllers(controllers),
      m_tempTuneIndex(-1),
      m_lastPlayingIndex(-1), // added by Sergey for CR#13466
      m_onRestoreDiscState(false)
      // m_bDeckPBCMode(false), // commented by Sergey for CR#14692
      // removed by cychoi 2013.06.04 for ISV 78671
{
    m_pIMedia = NULL;
	m_searchTitle = "";
	m_isStart = false;
    m_pModel = new AppMediaPlayer_Video_CModel_VCD(this);
    m_pDeckController = controllers->GetDeckController();
    m_bVCDMuteState = false; // modified by cychoi 2013.09.01 for ITS 183910 & 183912 Sound path if DVD or VCD // modified by yongkyun.lee 2013-07-16 for : ITS 175107
    // { modified by wspark 2014.02.19 for ITS 225827
    m_DirectMenuState[VIDEO_DISPLAY_FRONT] = false; //added by yungi 2013.09.07 for ITS 187311
    m_DirectMenuState[VIDEO_DISPLAY_REAR] = false;
    // } modified by wspark

    memset(&m_storedVCDInfo,0,sizeof(m_storedVCDInfo));
}

AppMediaPlayer_Video_ControllerVCD::~AppMediaPlayer_Video_ControllerVCD()
{
}


void AppMediaPlayer_Video_ControllerVCD::dropControllers()
{
    if( m_pDeckController == NULL )
    {
        MP_LOG << "Inconsistent state." << LOG_ENDL;
        return;
    }

    // { modified by wspark 2014.02.19 for ITS 225827
    setDirectAccessDisplayed(VIDEO_DISPLAY_FRONT, false); // added by cychoi 2014.01.09 for ITS 218948
    setDirectAccessDisplayed(VIDEO_DISPLAY_REAR, false);
    // } modified by wspark

    disconnect( m_pDeckController, SIGNAL(OnTrackChanged(int)),
                this, SLOT(onTrackChanged(int)));
    disconnect( m_pDeckController, SIGNAL(OnTOCReceived(DvdcTOC*)),
                this, SLOT(onTOCReceived(DvdcTOC*)));
    disconnect( m_pDeckController, SIGNAL(OnScreenStateChanged(int)),
                this, SLOT(onScreenStateChanged(int)));
    disconnect( m_pDeckController, SIGNAL(OnStateChanged(int)),
             this, SLOT(onStateChanged(int)));
    disconnect( m_pDeckController, SIGNAL(OnPositionChanged(int)),
             this, SLOT(onPositionChanged(int))); // added by Sergey Vetugov for CR#13620

    // { added by kihyung 2012.11.29
    disconnect( m_pDeckController, SIGNAL(OnError(int)),
             this, SLOT(onHandleError(int)));
    // } added by kihyung                

    // { added by cychoi 20103.06.11
    disconnect(m_pDeckController, SIGNAL(OnRealSoundChanged()),
             this, SLOT(onRealSoundChanged()));
    // } added by cychoi 20103.06.11

    // { modified by cychoi 2013.06.18 for ISV 85460 DVD OSD mismatch
    disconnect(m_pDeckController, SIGNAL(OnTrackBufferred(int, int, int, int)),
               this, SLOT(OnTrackBufferred(int, int, int, int)));
    // } modified by cychoi 2013.06.18
    disconnect(m_pDeckController, SIGNAL(OnUserTrackChanged(int)),
               this, SLOT(onUserTrackChanged(int)));
    //disconnect(m_pDeckController, SIGNAL(OnAutoTrackChanged(int)),
    //        this, SLOT(onAutoTrackChanged(int))); // added by cychoi 2014.04.06 for ITS 233500

    m_pIMedia = NULL;
}

// { added by cychoi 2013.08.07 for ITS 183155 Repeat mode init error on Disc Eject
void AppMediaPlayer_Video_ControllerVCD::onDeviceRemoved()
{
    if(m_pModel) 
    {
        m_pModel->setRepeatMode(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_OFF);
        m_pModel->setRandomMode(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_OFF);
        // { modified by wspark 2014.02.19 for ITS 225827
        setDirectAccessDisplayed(VIDEO_DISPLAY_FRONT, false); //added by yungi 2013.09.12 for SmokeTest DirectAccess During DRS On
        setDirectAccessDisplayed(VIDEO_DISPLAY_REAR, false);
        // } modified by wspark
    }
}
// { added by cychoi 2013.08.07

void AppMediaPlayer_Video_ControllerVCD::attachControllers()
{
    if ( m_pDeckController == NULL )
    {
        MP_LOG << "[MP] AppMediaPlayer_Video_ControllerVCD::attachControllers"
                 << "Inconsistent state." << LOG_ENDL;
        return;
    }

    m_pIMedia = m_pDeckController->GetMediaInstance();
    m_pIVideo = m_pDeckController->GetVideoInstance();
    if(m_pIVideo == NULL)
    {
        MP_LOG << " m_pIVideo == NULL" << LOG_ENDL;
        return;
    }
    if(m_pIMedia == NULL)
    {
        MP_LOG << "m_pIMedia == NULL" << LOG_ENDL;
        return;
    }
    //m_pIMedia->SetPlayerEngine( GPLAYER_ENGINE );
    m_pIMedia->ChangeMediaMode( MP_MODE_DISC );

    connect( m_pDeckController, SIGNAL(OnTrackChanged(int)),
             this, SLOT(onTrackChanged(int)));
    connect( m_pDeckController, SIGNAL(OnTOCReceived(DvdcTOC*)),
             this, SLOT(onTOCReceived(DvdcTOC*)));
    connect( m_pDeckController, SIGNAL(OnScreenStateChanged(int)),
             this, SLOT(onScreenStateChanged(int)));
    connect( m_pDeckController, SIGNAL(OnStateChanged(int)),
             this, SLOT(onStateChanged(int)));
    connect( m_pDeckController, SIGNAL(OnPositionChanged(int)),
             this, SLOT(onPositionChanged(int))); // added by Sergey Vetugov for CR#13620

    // { added by kihyung 2012.11.29
    connect( m_pDeckController, SIGNAL(OnError(int)),
             this, SLOT(onHandleError(int)));
    // } added by kihyung             

    // { added by cychoi 20103.06.11
    connect( m_pDeckController, SIGNAL(OnRealSoundChanged()),
             this, SLOT(onRealSoundChanged()));
    // } added by cychoi 20103.06.11

    // { modified by cychoi 2013.06.18 for ISV 85460 DVD OSD mismatch
    connect(m_pDeckController, SIGNAL(OnTrackBufferred(int, int, int, int)),
            this, SLOT(OnTrackBufferred(int, int, int, int)));
    // } modified by cychoi 2013.06.18
	connect(m_pDeckController, SIGNAL(OnUserTrackChanged(int)),
				this, SLOT(onUserTrackChanged(int)));
    //connect(m_pDeckController, SIGNAL(OnAutoTrackChanged(int)),
    //            this, SLOT(onAutoTrackChanged(int))); // added by cychoi 2014.04.06 for ITS 233500

    m_pIVideo->RequestTOC();

    if( m_pModel->dirty() )
    {
    	// { added by Sergey Vetugov. CR#12599
        MP_LOG << m_pModel->currentTrack() << LOG_ENDL;

        if(m_pModel->currentTrack() > 0)
            m_pIMedia->SetCurrentIndex(m_pModel->currentTrack() - 1);
        // { added by Sergey Vetugov. CR#12599
    }
    // { added by cychoi 2013.09.24 for ITS 191333
    else
    {
        m_pModel->setCurrentTrack(0);
    }
    // } added by cychoi 2013.09.24

    if( m_pModel->repeatMode() == AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_FILE) 
        m_pIMedia->RepeatPlay( REPEATALL );
    
    //{ modified by yongkyun.lee 2013-07-16 for : ITS 175107
    //setAudioPath();    
    m_bVCDMuteState = true; // modified by cychoi 2013.09.01 for ITS 183910 & 183912 Sound path if DVD or VCD
    //} modified by yongkyun.lee 2013-07-17 

    // { modified by kihyung 2013.1.26
    if(m_pEngineVideo->getIsFgReceived() == false) {
        // { modified by cychoi 2013.08.30 for ITS 187026 BT call on PowerOff mode
        if(m_pEngineVideo->getIsFgReceived() == false && !m_pEngineVideo->getEngineMain().getPowerOffBTCall() ) {
            m_pEngineVideo->getEngineMain().setUnMuteForSeek(false);
            play();
        }
        // } modified by cychoi 2013.08.30
    }
    else{
        onStateChanged(m_pIMedia->GetCurrentMediaState());
        //onScreenStateChanged(m_pIVideo->GetCurrentScreenState()); // added by kihyung 2013.2.6 for ISV 70642
        // removed by Sergey 18.05.2013
    }
    // } modified by kihyung
}

AppMediaPlayer_Video_CModel_Base *AppMediaPlayer_Video_ControllerVCD::getModel()
{
    return qobject_cast<AppMediaPlayer_Video_CModel_Base*>(m_pModel);
}


void AppMediaPlayer_Video_ControllerVCD::switchTrack( int trackNo )
{
    if ( trackNo > m_pModel->trackCount() || trackNo < 0)
    {
        MP_LOG << "Invalid track number. Ignoring." << LOG_ENDL;
        return;
    }
    if(m_pModel->currentTrack() != trackNo)
    {
        m_pModel->setIsTrackLoading(true); // added by Sergey Vetugov for CR#13620
        m_pIMedia->SetCurrentIndex( trackNo - 1 );		
        // { added by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
        m_pEngineVideo->getEngineMain().setMuteForSeek(false);
        m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(true);
        //m_pEngineVideo->getEngineMain().setUnMuteState(false);//added by edo.lee 2013.05.19
        // } added by cychoi 2013.10.20
        play();
    }
}

void AppMediaPlayer_Video_ControllerVCD::setPBC( bool enabled )
{
    // { modified by kihyung 2012.08.21 for 3PDMS 0137765 
    if(m_pIVideo) {
        MP_LOG << "enabled =" << enabled << LOG_ENDL;
        m_pIVideo->SetPBC( enabled );
        m_pModel->setIsPbcMode(enabled); // added by Sergey for CR#14692
        // removed by cychoi 2013.06.04 for ISV 78671
    }
    // } modified by kihyung    
}

// { added by wspark 2013.03.17 for ISV 76329
void AppMediaPlayer_Video_ControllerVCD::setPbcMenuDisplayed( bool enabled )
{
    if(m_pIVideo) {
        m_pModel->setIsPbcMenuDisplayed( enabled );
    }
}
// } added by wspark

// { added by cychoi 2013.12.19 for ITS 215825 Default focus
bool AppMediaPlayer_Video_ControllerVCD::isPbcMenuShown()
{
    bool isPbcMenuShown = false;

    if(m_pIVideo) {
        isPbcMenuShown = m_pModel->isPbcMenuShown();
    }

    return isPbcMenuShown;
}
// } added by cychoi 2013.12.19

void AppMediaPlayer_Video_ControllerVCD::onTrackChanged(int trackNo)
{
    MP_LOG << "trackNo =" << trackNo << LOG_ENDL;
    getVCDInfo(trackNo); // modified by cychoi 2013.08.17 for HMC request - Cluster display = OSD display

    // { added by Sergey Vetugov. CR#12599
    // m_pEngineVideo->SaveSettings(); // removed by kihyung 2013.06.18
	// { added by Sergey Vetugov. CR#12599

    // { added by kihyung 2012.07.12
    if(m_pModel->vcdVersion() == VCD_VERSION_2_0
       && m_pModel->isPbcMode() == true) //modified by Sergey for CR#14692
    {
        m_pEngineVideo->getScreenManager(VIDEO_DISPLAY_FRONT)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                                                                  AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                                                                  EVENTS_PBC_MENU,
                                                                  QVariant(true));
		m_pEngineVideo->getScreenManager(VIDEO_DISPLAY_REAR)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                                                                  AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                                                                  EVENTS_PBC_MENU,
                                                                  QVariant(true));
    }
    else {
        m_pEngineVideo->getScreenManager(VIDEO_DISPLAY_FRONT)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                                                                  AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                                                                  EVENTS_PBC_MENU,
                                                                  QVariant(false));
		m_pEngineVideo->getScreenManager(VIDEO_DISPLAY_REAR)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                                                                  AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                                                                  EVENTS_PBC_MENU,
                                                                  QVariant(false));
    }
    // } added by kihyung
}

void AppMediaPlayer_Video_ControllerVCD::restoreDiscState()
{
   MP_LOG << LOG_ENDL;
   // { modified by wspark 2013.03.09 for ISV 65056
   /*
   m_pModel->setLoadingMode(true); // added by Sergey for Loading popup
   switchTrack( m_storedVCDInfo.trackNo );

   if (m_storedVCDInfo.playStatus == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PAUSED)
   {
      pause();
   }
   m_onRestoreDiscState = true;
   */
   // } modified by wspark
}


void AppMediaPlayer_Video_ControllerVCD::storeDiscState()
{
    MP_LOG << LOG_ENDL;
    m_pModel->setResumePosition(m_pModel->position());
    m_pIVideo->GetVCDStatus(&m_storedVCDInfo);
    m_storedVCDInfo.playStatus = m_pModel->playbackStatus();
    m_storedVCDInfo.playbackTime = m_pModel->position();
}


// { modified by cychoi 2013.07.07 for ISV 85460 DVD OSD mismatch
void AppMediaPlayer_Video_ControllerVCD::getVCDInfo(int trackIndex)
//void AppMediaPlayer_Video_ControllerVCD::getVCDInfo()
// } modified by cychoi 2013.07.07
{
    MP_MEDIUM << LOG_ENDL;

    if(m_pIVideo)
    {    
        VCDStatusResponse status;
        memset( &status, 0, sizeof(VCDStatusResponse) );

        m_pIVideo->GetVCDStatus( &status );
        //{ modified by yungi 2013.08.10 for ITS 183835 No OSD on track change automatically
        if(trackIndex >= 0)
        {
            // { modified by cychoi 2013.07.07 for ISV 85460 DVD OSD mismatch
            // This code is temporary code.
            // TODO: We should move this function to onStateChanged or onTrackChanged as like MP3 and CDDA.
            status.trackNo = trackIndex + status.startTrackNo;
            // } modified by cychoi 2013.07.07

            // { added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
            m_pEngineVideo->GetVideoController()->sendMediaInfoClusterChanged(QString::number(status.trackNo));
            m_pEngineVideo->GetVideoController()->sendTrackClusterChanged(status.trackNo, 0);
            // } added by cychoi 2013.08.17

            // { modified by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
            if(m_tempTuneIndex == -1 || m_tempTuneIndex == m_lastPlayingIndex)
            {
                // { modified by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
                if(m_pModel->isPbcMenuDisplayed() == false)
                {
                    m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false);	
                    if(m_isStart)
                    {
                        m_pEngineVideo->setOSDMetaData(DISC_VCD_VIDEO, getOSDTitle(QString("%1").arg(status.trackNo)),
                                m_pModel->randomMode(),m_pModel->repeatMode(),0, false); // modified by cychoi 2013.11.12 for OSD Title Translation
                        m_isStart = false;
                    }else{
                        m_pEngineVideo->updateOSDMetaData(DISC_VCD_VIDEO, getOSDTitle(QString("%1").arg(status.trackNo))); // modified by cychoi 2013.11.12 for OSD Title Translation
                    }
                }
                // } modified by cychoi 20103.10.16
            }
            else //if(m_tempTuneIndex != -1)
            {
                m_lastPlayingIndex = status.trackNo;
            }
            // } modified by cychoi 20103.10.20
        }
        //} modified by yungi 2013.08.10 for ITS 183835
        // { added by cychoi 20103.10.17 for SmokeTest Invalid Track No
        else if(status.trackNo == 0xFF)
        {
            // If track no is invalid, don't display track no.
            status.trackNo = 0;
        }
        // } added by cychoi 20103.10.17

        m_pModel->setCurrentTrack( status.trackNo );
        m_pModel->setVcdVersion( status.version );
    }
}

void AppMediaPlayer_Video_ControllerVCD::onTOCReceived( DvdcTOC *pTOC )
{
    m_pModel->setTrackCount( pTOC->last_track - 1 );
    //m_pModel->setTotalPlayMins( pTOC->total_play_min );
}

void AppMediaPlayer_Video_ControllerVCD::onScreenStateChanged(int nScreenState)
{
    MP_LOG << " nScreenState " << nScreenState << LOG_ENDL;

    // removed by cychoi 2013.06.04 for ISV 78671

    // { modified by kihyung 2013.1.9 for ISV 70642
    //if(AppMediaPlayer_Video_Screens::SCREEN_OPTIONS == m_pEngineVideo->getScreenManager()->topScreenName()) {
    //    m_pEngineVideo->getScreenManager()->popScreen();
    //}
    if(nScreenState == 0)
		m_isStart = true;
    // { commented by cychoi 2013.07.23 for ITS 181238
    //m_pEngineVideo->getScreenManager(VIDEO_DISPLAY_FRONT)->closeOptionPopup();
	//m_pEngineVideo->getScreenManager(VIDEO_DISPLAY_REAR)->closeOptionPopup();
    // } commented by cychoi 2013.07.23

    bool bPBCMenu = false;
    if(nScreenState == PLAYER_TOPMENU_DISPLAY  ||
       nScreenState == PLAYER_ROOTMENU_DISPLAY ||
       nScreenState == PLAYER_TITLEMENU_DISPLAY)
    {
        bPBCMenu = true;
        // { added by cychoi 2013.11.28 for ITS 211634
        if(m_pModel->isPbcMenuDisplayed() == false)
        {
            m_pEngineVideo->GetVideoController()->sendTrackClusterChanged(0xffff, 0);
        }
        // } added by cychoi 2013.11.28
    }
    // { added by cychoi 2013.10.06 for ITS 206869
    else
    {
        if(m_pModel->loadingMode() == false)
        {
            m_pModel->setLoadingMode(true);
        }
    }
    // } added by cychoi 2013.10.06
    m_pModel->setIsPbcMenuDisplayed( bPBCMenu );
    //setPBC(bPBCMenu); // commented by cychoi 2013.07.27 for setPBC is already called when pressed PBC On in Menu
    // } modified by kihyung 2013.1.9    
}

void AppMediaPlayer_Video_ControllerVCD::onStateChanged( int nState )
{
    MP_MEDIUM << " nState " << nState << LOG_ENDL;
    
    if( nState == PLAYER_PLAY )
    {
        //{ modified by cychoi 2013.09.01 for ITS 183910 & 183912 Sound path if DVD or VCD
        //{ modified by yongkyun.lee 2013-07-16 for : ITS 175107
        if(m_bVCDMuteState == true)
        {
            m_bVCDMuteState = false;
            m_pEngineVideo->modeTabClicked( true);
            setAudioPath();
        }    
        //} modified by yongkyun.lee 2013-07-16 
        //} modified by cychoi 2013.08.27
        
        if(m_onRestoreDiscState)
        {
            MP_MEDIUM << " setPosition " << m_pModel->resumePosition() << LOG_ENDL;
            setPosition(m_pModel->resumePosition());
            m_onRestoreDiscState = false;
            m_pModel->setResumePosition(0);
        }       

        // removed by cychoi 2013.06.04 for ISV 78671
    }
    //{ modified by yongkyun.lee 2013-07-17 for : ITS 175107
    else if( nState == PLAYER_STOP )
    {
        m_bVCDMuteState = true; // modified by cychoi 2013.09.01 for ITS 183910 & 183912 Sound path if DVD or VCD
    }
    //} modified by yongkyun.lee 2013-07-17 
    // { modified by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
    else if( nState == PLAYER_FASTFORWARD ||
             nState == PLAYER_REWIND)
    {
        if(m_pModel->isPbcMenuDisplayed() == true)
        {
            m_pEngineVideo->getEngineMain().setMuteForSeek(false); // added by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute
            m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false); // added by cychoi 2013.09.04 for OSD on FF
        }
    }
    // } modified by cychoi 20103.10.16
    // { modified by cychoi 2014.04.14 for HW Event DV2-1 DVD sound truncated
    else if( nState == PLAYER_SEARCH )
    {
        if(m_bVCDMuteState == true)
        {
            m_bVCDMuteState = false;
            m_pEngineVideo->modeTabClicked( true);
            setAudioPath();
        }
    }
    // } modified by cychoi 2014.04.14

    // removed by Sergey 26.07.2013 for ITS#181096


    // { added by cychoi 2013.11.10 for ITS 207831 Control Cue Icon on Tune Mode
    if( nState == PLAYER_PLAY || nState == PLAYER_SEARCH )
    {
        if(m_tempTuneIndex != -1)
        {
            m_pEngineVideo->setTuneModeOnStateChanged(m_tempTuneIndex == m_lastPlayingIndex);
        }
    }
    // } added by cychoi 2013.11.10
}

void AppMediaPlayer_Video_ControllerVCD::pbcTenKey (int key )
{
    MP_LOG << "key == " << key << LOG_ENDL;

    if( m_pIVideo )
    {
        m_pIVideo->TenKey( key + 1, 0);
    }
}

void AppMediaPlayer_Video_ControllerVCD::pbcEnterKey ()
{
    MP_LOG << LOG_ENDL;

    if( m_pIVideo )
    {
        m_pIVideo->EnterKey();
    }
}

void AppMediaPlayer_Video_ControllerVCD::pbcReturnKey ()
{
    MP_LOG << LOG_ENDL;

    if( m_pIVideo )
    {
        m_pIVideo->ReturnKey();
    }
}

// { added by cychoi 2013.07.06 for ITS 163782
void AppMediaPlayer_Video_ControllerVCD::pbcClearKey()
{
    MP_LOG << LOG_ENDL;

    if( m_pIVideo )
    {
        m_pIVideo->TenKey( 0, 0);
    }
}
// } added by cychoi 2013.07.06

// { added by Sergey for CR#14692
/*void AppMediaPlayer_Video_ControllerVCD::setDeckPBC(bool enable)
{
    m_bDeckPBCMode = enable;
}
bool AppMediaPlayer_Video_ControllerVCD::isDeckPBC()
{
    return m_bDeckPBCMode;
}*/
// } added by Sergey for CR#14692.


// { added by kihyung 2012.08.27 for Video(LGE) #12
void AppMediaPlayer_Video_ControllerVCD::setPlaybackControl(bool bShow)
{
    MP_LOG << bShow << LOG_ENDL;
    emit showPlaybackControls(bShow);
}
// } added by kihyung

void AppMediaPlayer_Video_ControllerVCD::setAudioPath()
{
    MP_LOG << LOG_ENDL;

    // m_pControllers->SetAudioPath( eDeckVideoSPDIF ); // removed by kihyung 2012.11.03 for booting disable
    m_pEngineVideo->NofityAudioPathChanged( MODE_STATE_DISC_VIDEO); // changed by minho 20120828 for able active tab on VCD
}

void AppMediaPlayer_Video_ControllerVCD::fastForward()
{
    MP_LOG << LOG_ENDL;

    if( m_pIMedia )
    {
        // { modified by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
        if(m_pModel->isPbcMenuDisplayed() == false)
        {
            m_pEngineVideo->getEngineMain().setMuteForSeek(false); // added by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute
            m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false); // added by cychoi 2013.09.04 for OSD on FF
        }
        // } modified by cychoi 20103.10.16
        m_pIMedia->FastForward(FAST_SPEED_X4);
    }
}

void AppMediaPlayer_Video_ControllerVCD::fastForwardCritical()
{
    MP_LOG << LOG_ENDL;

    if( m_pIMedia )
    {
        m_pIMedia->FastForward(FAST_SPEED_X16);
    }
}

void AppMediaPlayer_Video_ControllerVCD::rewind()
{
    MP_LOG << LOG_ENDL;

    if( m_pIMedia )
    {
        // { modified by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
        if(m_pModel->isPbcMenuDisplayed() == false)
        {
            m_pEngineVideo->getEngineMain().setMuteForSeek(false); // added by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute
            m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false); // added by cychoi 2013.09.04 for OSD on REW
        }
        // } modified by cychoi 20103.10.16
        m_pIMedia->Rewind(FAST_SPEED_X4);
    }
}

void AppMediaPlayer_Video_ControllerVCD::rewindCritical()
{
    MP_LOG << LOG_ENDL;

    if( m_pIMedia )
    {
        m_pIMedia->Rewind(FAST_SPEED_X16);
    }
}

void AppMediaPlayer_Video_ControllerVCD::flickNext()
{
    MP_LOG << LOG_ENDL;

//    if(m_pEngineVideo->getEngineMain().getisBTCall() == true) return; // commented by Sergey 17.05.2013 for ISV#83170

    if( m_pIMedia && m_pModel->trackCount() > 1 )
    {
        // { modified by cychoi 2013.09.10 for Sound mute on flick prev/next
        next();
        //m_pIMedia->Next(1);
        // } modified by cychoi 2013.09.10
    }
}

void AppMediaPlayer_Video_ControllerVCD::flickPrev()
{
    MP_LOG << LOG_ENDL;

//    if(m_pEngineVideo->getEngineMain().getisBTCall() == true) return; // commented by Sergey 17.05.2013 for ISV#83170

	// { modified by eugene 2012.08.22 for static analysis defect #7090
    //if( m_pIMedia || m_pModel->trackCount() > 1 )
	if( m_pIMedia && m_pModel->trackCount() > 1 )
	// } modified by eugene 2012.08.22 for static analysis defect #7090
    {
        // { modified by cychoi 2013.09.10 for Sound mute on flick prev/next
        prev(1);
        //m_pIMedia->Prev(1,1); // modified by Sergey 28.05.2013
        // } modified by cychoi 2013.09.10
    }
}



bool AppMediaPlayer_Video_ControllerVCD::tuneWheel(bool bTuneForward)
{
    // { added by kihyung 2013.07.22 for ITS 0181137
    if(m_pModel->loadingMode() == true) {
        MP_LOG << "m_pModel->loadingMode()... return..." << LOG_ENDL;
        return false; 
    }
    // } added by kihyung 2013.07.22 for ITS 0181137

    // { added by kihyung 2012.07.17 for CR 8042
    if(m_pModel->isPbcMode() == true) // { added by Sergey for CR#14692
    {
        MP_LOG << "not support on PBC mode" << LOG_ENDL;
        return false;
    }
    // } added by kihyung

    MP_LOG << "tune forward =" << bTuneForward << LOG_ENDL;

    if(m_pIMedia == NULL || m_pModel->trackCount() == 0)
    {
        MP_LOG << "m_pIMedia is NULL or playlist is empty" << LOG_ENDL;
        return false;
    }
    emit cancelFfRew(); // added by yongkyun 2013.01.19 for ITS 150703

    // removed by Sergey 23.09.2013 for ITS#188498

    if ( m_tempTuneIndex == -1 )
    {
        // { modified by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
        VCDStatusResponse status;
        memset( &status, 0, sizeof(VCDStatusResponse) );

        m_pIVideo->GetVCDStatus( &status );
        m_lastPlayingIndex = status.trackNo;
        //m_lastPlayingIndex = m_pIMedia->GetCurrentIndex() + 1; // modified by eunhye.yoo 2012.08.14 for CR 10985 
        // } modified by cychoi 2013.10.20
        m_tempTuneIndex = m_lastPlayingIndex;
    }

    if ( bTuneForward )
    {
        // If end of list switch to first.
        if ( m_tempTuneIndex >= m_pModel->trackCount() )
        {
            m_tempTuneIndex = 1;
        }
        else
        {
            m_tempTuneIndex++;
        }
    }
    else
    {
        // If first file in list switch to last.
        if ( m_tempTuneIndex <= 1)
        {
            m_tempTuneIndex = m_pModel->trackCount();
        }
        else
        {
            m_tempTuneIndex--;
        }
    }
	
    // { added by eugene.seo 2013.03.07
    if(m_tempTuneIndex > 0)
	{ 
        // { added  by  yongkyun.lee 2012.10.17  for Current file is white color 
        //added by sh.kim for ITS 180960
        if( m_tempTuneIndex == m_lastPlayingIndex )
        {
            if(m_pModel->trackCount() == 1)
            {
                MP_LOG << ":: m_pModel->trackCount() == 1 " << LOG_ENDL;
                m_pEngineVideo->SetTuneSameFile(false);
                m_pEngineVideo->showCurrentOSD();
                m_pEngineVideo->SetTuneSameFile(true);
                return true;
            }
            else
            {
                MP_LOG << ":: m_pModel->trackCount() > 1 " << LOG_ENDL;
                // { modified by lssanh 2013.03.09 ISV72498 tune center
                //m_pEngineVideo->SetTuneSameFile(true);
                //m_pEngineVideo->releaseTuneMode();    // added by yongkyun.lee 20130703 for : ITS 177707
                m_pEngineVideo->showCurrentOSD();
                m_pEngineVideo->SetTuneSameFile(true);//added by edo.lee 2013.03.22
                //{ added by yongkyun.lee 20130701 for : DISC OSD
                return true;
                //return false; // added by lssanh 2013.03.27 NoCR tune search -1
                //} added by yongkyun.lee 20130701
                // } modified by lssanh 2013.03.09 ISV72498 tune center
            }
        } //added by sh.kim for ITS 180960
        else{
            MP_LOG << ":: m_tempTuneIndex == m_lastPlayingIndex" <<
                      "m_tempTuneIndex : " << m_tempTuneIndex <<
                      "m_pModel->trackCount() : " << m_pModel->trackCount() << LOG_ENDL;

            m_searchTitle = QString("%1").arg(m_tempTuneIndex);
            m_pEngineVideo->SetTuneSameFile(false); 
            m_pEngineVideo->setOSDMetaData(
                    DISC_VCD_VIDEO,getOSDTitle(m_searchTitle), m_pModel->randomMode(), m_pModel->repeatMode(), 0, true); // modified by cychoi 2013.11.12 for OSD Title Translation
        }
	    // } added  by  yongkyun.lee 
	    // Set temporary track.
	    // { modified by kihyung 2012.07.17 for CR 8042
	    // m_pModel->setCurrentTrack(m_tempTuneIndex);
	    m_pModel->setTuneWheelTrack(m_tempTuneIndex);
	    // } modified by kihyung
    }
	// } added by eugene.seo 2013.03.07
	
    return true; // added by kihyung 2012.07.17
}

bool AppMediaPlayer_Video_ControllerVCD::selectTuned() // modified by junggil 2012.07.05 for CR11261 : Not able to search video files using CCP/RRC jogdial
{
    // { added by kihyung 2012.07.17 for CR 8042
    if(m_pModel->isPbcMode() == true) // { added by Sergey for CR#14692
    {
        MP_LOG << "not support on PBC mode" << LOG_ENDL;
        return false;
    }
    // } added by kihyung

    //MP_LOG << "temp index =" << m_tempTuneIndex << LOG_ENDL;

    if(m_pIMedia == NULL || (m_pModel->trackCount() == 0) || (m_tempTuneIndex == -1))
    {
        MP_LOG << "m_pIMedia is NULL or playlist is empty or incorrect index" << LOG_ENDL;
        return false; // added by junggil 2012.07.05 for CR11261 : Not able to search video files using CCP/RRC jogdial
    }

    MP_LOG << "temp index =" << m_tempTuneIndex << LOG_ENDL;

    // { modified by kihyung 2012.08.03 for CR 12313
    if(m_pModel->trackCount() < m_tempTuneIndex) // in the scope of playlist?
    {
        m_tempTuneIndex = -1;
        m_lastPlayingIndex = -1; // added by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
        return false;
    }
    // { modified by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
    if(m_tempTuneIndex == m_lastPlayingIndex)
    {
        m_pEngineVideo->setOSDMetaData(
        	   			DISC_VCD_VIDEO,getOSDTitle(m_searchTitle), m_pModel->randomMode(), m_pModel->repeatMode(), 0, false); // modified by cychoi 2013.11.12 for OSD Title Translation
    }
    // } modified by cychoi 2013.10.20
    m_searchTitle = "";
    switchTrack(m_tempTuneIndex);
    m_tempTuneIndex = -1;
    m_lastPlayingIndex = -1; // added by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
    return true; // added by junggil 2012.07.05 for CR11261 : Not able to search video files using CCP/RRC jogdial
    // } modified by kihyung
}

void AppMediaPlayer_Video_ControllerVCD::cancelTune()
{
    // { added by kihyung 2012.07.17 for CR 8042
    if(m_pModel->isPbcMode() == true) // { added by Sergey for CR#14692
    {
        MP_LOG << "not support on PBC mode" << LOG_ENDL;
        return;
    }
    // } added by kihyung

    // { modified by Sergey for CR#13466
    // Set playing track.
    if(m_tempTuneIndex != -1
       && (m_lastPlayingIndex > 0) && (m_lastPlayingIndex <= m_pModel->currentTrack()))
    {
        m_pModel->setCurrentTrack(m_lastPlayingIndex);
    }

    m_tempTuneIndex = -1;
    m_lastPlayingIndex = -1; // added by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
    // } modified by Sergey for CR#13466
}

// { added by Sergey Vetugov for CR#13620
void AppMediaPlayer_Video_ControllerVCD::onPositionChanged(int position)
{
    // removed by Sergey 18.05.2013

    /* Unfortunately the only case to know that track really started playing is
     * to catch onPositionChanged signal. Catching onTrackChanged or OnStateChanged
     * can't be used as they come too early while Deck is still not ready
     * to get new requests.
     */
    if(position >= 1000)
    {
        m_pModel->setIsTrackLoading(false);
        // { commented by cychoi 2013.08.27 for ITS 184304 Loading screen close
        //if(m_pModel->loadingMode()) // added by Sergey 18.05.2013
        //    m_pModel->setLoadingMode(false); // added by Sergey for Loading popup
        // } commented by cychoi 2013.08.27

        // removed by cychoi 2013.06.04 for ISV 78671
    }
}
// } added by Sergey Vetugov for CR#13620.


// { added by kihyung 2012.11.29
void AppMediaPlayer_Video_ControllerVCD::onHandleError(int nError)
{
    MP_LOG << "nError =" << nError << LOG_ENDL;
    
    if(nError == PLAYER_COMMANDNOTSUPPORTED)
    {
        // { modified by sjhyun 2013.11.05 for ITS 206357
        //emit showInhibitionIcon(true);
        m_pEngineVideo->inhibitionIcon(true);
        // } modified by sjhyun
    }

    // { added by cychoi 2013.10.03 for ITS 193259 Infinite loading screeen on Error
    if(m_pModel->loadingMode() == true)
    {
        m_pModel->setLoadingMode(false);
    }
    // } added by cychoi 2013.10.03
}
// } added by kihyung 2012.11.29

// { added by cychoi 20103.06.11
void AppMediaPlayer_Video_ControllerVCD::onRealSoundChanged()
{
    MP_LOG << LOG_ENDL;
    // removed by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
	m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false); // added by yungi 2013.08.10 for ITS 183835 No OSD on track change automatically
    m_pEngineVideo->getEngineMain().setUnMuteState(false);
}
// } added by cychoi 20103.06.11

// { modified by cychoi 2013.06.18 for ISV 85460 DVD OSD mismatch
void AppMediaPlayer_Video_ControllerVCD::OnTrackBufferred(int bPrev, int nTotalIdx, int nIdxInFolder, int nFolderSize)
{
    MP_LOG << LOG_ENDL;
    Q_UNUSED(bPrev); Q_UNUSED(nIdxInFolder); Q_UNUSED(nFolderSize);
    getVCDInfo(nTotalIdx);
}
// } modified by cychoi 2013.06.18

void AppMediaPlayer_Video_ControllerVCD::onUserTrackChanged(int trackindex)
{
    MP_LOG << LOG_ENDL;
    if(m_pEngineVideo->GetVideoController()->getSource() != MEDIA_SOURCE_VCD
        ||  m_pEngineVideo->getEngineMain().GetLastPlayerMode() != APP_MEDIA_MODE_VIDEO)
    {
        MP_LOG<<"mode is not matched" << LOG_ENDL;
        return;
    }

    //{ added by yungi 2013.08.10 for  ITS 183571 Track No display
    VCDStatusResponse status;
    memset( &status, 0, sizeof(VCDStatusResponse) );

    m_pIVideo->GetVCDStatus( &status );
    int trackNo = trackindex + status.startTrackNo;

    m_pEngineVideo->setOSDMetaData(DISC_VCD_VIDEO, getOSDTitle(QString("%1").arg(trackNo)),
                    m_pModel->randomMode(),m_pModel->repeatMode(),0, false); // modified by cychoi 2013.11.12 for OSD Title Translation
    //} added by yungi 2013.08.10

    // { added by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
    if(m_pModel->currentTrack() == trackNo)
    {
        // Track donw: over 3 sec in current TRACK ¡æ current TRACK -> Send OnUserTrackChanged signal
        m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false);
    }
    // } added by cychoi 2013.10.20

    m_pEngineVideo->getEngineMain().sendVRCancelSignal(); // added by cychoi 2014.03.12 for ITS 224783 VR cancel on onUserTrackChanged
}

// { added by cychoi 2014.04.06 for ITS 233500
//void AppMediaPlayer_Video_ControllerVCD::onAutoTrackChanged(int index)
//{
//    Q_UNUSED(index)
//}
// } added by cychoi 2014.04.06

void AppMediaPlayer_Video_ControllerVCD::ShowCurrentModelToOSD()
{
    MP_LOG << LOG_ENDL;
    // { commented by cychoi 2013.10.11 for ITS 195154 No OSD if operation is forbidden
    //m_pEngineVideo->setOSDMetaData(DISC_VIDEO, getOSDTitle(QString("%1").arg(m_pModel->currentTrack())),
    //                    m_pModel->randomMode(),m_pModel->repeatMode(),0, false);
    // } commented by cychoi 2013.10.11
}

QString AppMediaPlayer_Video_ControllerVCD::getOSDTitle(QString title)
{
    return  QApplication::translate("main", "STR_MEDIA_TRACK") + " "+title;
}

// { added by ravikanth - 12-09-19
void AppMediaPlayer_Video_ControllerVCD::toggleRepeat()
{
    MP_LOG<<" value = "<< getModel()->repeatMode() << LOG_ENDL;
    switch(getModel()->repeatMode())
    {
    case AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_OFF:
        repeatPlayFile();
        break;
    case AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_FILE:
        repeatPlayAll();
        break;
    default:
        //ignore
        break;
    }
}
// } added by ravikanth - 12-09-19

// { added by eunhye 2013.02.07 for ISV70720
void AppMediaPlayer_Video_ControllerVCD::prev(bool bForce ) //modified by edo.lee 2013.06.11
{
    MP_LOG << LOG_ENDL;
    if(m_pIMedia)
    {
        // { added by kihyung 2013.07.22 for ITS 0181137
        if(m_pModel->loadingMode() == true) {
            MP_LOG << "m_pModel->loadingMode()... return..." << LOG_ENDL;
            return; 
        }
        // } added by kihyung 2013.07.22 for ITS 0181137
        
        // { modified by cychoi 2013.07.07 for blocking prev fucntion if PBC on
        if(m_pModel->isPbcMode() == true &&
           m_pIMedia->GetCurrentMediaState() == PLAYER_SEARCH)
        {
            return;
        }
		
        // { modified by cychoi 2013.08.30 for ITS 184879
        if(m_pModel->isPbcMenuDisplayed() == true)
        {
            m_pIMedia->Prev(1);
        }
        else
        {
            if(m_pModel->isPbcMode() == false) // added by cychoi 2013.10.24 for ITS 198417 No mute on Seek Down in PBC on
            {
                m_pEngineVideo->getEngineMain().setMuteForSeek(false); // added by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute
                m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(true);
            }
            if(m_pEngineVideo->isVideoListView()!= VIDEO_DISPLAY_MAX)//modified by edo.lee 2013.05.29
            {
                m_pIMedia->Prev(1,1);
            }
            // { modified by cychoi 2013.08.20 for ITS 178895 No OSD display on PBC menu
            else
            {
                m_pIMedia->Prev(1, bForce);
                // { commented by cychoi 2013.10.11 for ITS 195154 No OSD if operation is forbidden
                //if(bForce == false)
                //    ShowCurrentModelToOSD();
                // } commented by cychoi 2013.10.11
            }
            // } modified by cychoi 2013.08.20
        }
        // } modified by cychoi 2013.08.30
        // } modified by cychoi 2013.07.07        
    }
}
// } added by eunhye 2013.02.07

// { added by cychoi 2013.07.07 for blocking next fucntion if PBC on 
void AppMediaPlayer_Video_ControllerVCD::next()
{
    MP_LOG << LOG_ENDL;
    if(m_pIMedia)
    {
        // { added by kihyung 2013.07.22 for ITS 0181137
        if(m_pModel->loadingMode() == true) {
            MP_LOG << "m_pModel->loadingMode()... return..." << LOG_ENDL;
            return; 
        }
        // } added by kihyung 2013.07.22 for ITS 0181137
        
        if(m_pModel->isPbcMode() == true &&
           m_pIMedia->GetCurrentMediaState() == PLAYER_SEARCH)
        {
            return;
        }
        // { modified by cychoi 2013.08.30 for ITS 184879
        if(m_pModel->isPbcMenuDisplayed() != true)
        {
            m_pEngineVideo->getEngineMain().setMuteForSeek(false); // added by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute
            m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(true);
        }
        // } modified by cychoi 2013.08.30
        m_pIMedia->Next(1);
    }
}
// { added by cychoi 2013.07.07

// { added by shkim 2013.08.27 for ITS 186402
bool AppMediaPlayer_Video_ControllerVCD::isMediaSearchState()
{
    MP_LOG << LOG_ENDL;
    if(m_pIMedia)
    {
        return (m_pIMedia->GetCurrentMediaState() == PLAYER_SEARCH);
    }

    return false;
}
// } added by shkim 2013.08.27

// { added by cychoi 2013.10.27 for ITS 197433
void AppMediaPlayer_Video_ControllerVCD::retranslateUI()
{
    // { modified by cychoi 2013.12.05 for ITS 212884
    if(m_pEngineVideo->GetVideoController()->getSource() != MEDIA_SOURCE_VCD ||
       m_pEngineVideo->getEngineMain().GetLastPlayerMode() != APP_MEDIA_MODE_VIDEO)
    {
        MP_LOG<<"mode is not matched" << LOG_ENDL;
        return;
    }
    // } modified by cychoi 2013.12.05

    if(m_pModel->isPbcMenuDisplayed() != true)
    {
        m_pEngineVideo->updateOSDMetaData(DISC_VCD_VIDEO, getOSDTitle(QString("%1").arg(m_pModel->currentTrack()))); // modified by cychoi 2013.11.12 for OSD Title Translation
    }
}
// } added by cychoi 2013.10.27

