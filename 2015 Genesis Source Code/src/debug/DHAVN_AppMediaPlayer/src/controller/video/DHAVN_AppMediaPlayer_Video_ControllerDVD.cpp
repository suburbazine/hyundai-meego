#include "controller/DHAVN_AppMediaPlayer_Controllers.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerDVD.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h" // added by lssanh 2012.08.24 for audio path
#include "util/osd/OSD_Manager.h"

AppMediaPlayer_Video_ControllerDVD::
AppMediaPlayer_Video_ControllerDVD( AppMediaPlayer_Controllers *controllers,
                                   AppMediaPlayer_EngineVideo *engine, // added by lssanh 2012.08.24 for audio path
                                   QObject *parent )
    : QObject(parent),
      m_pControllers(controllers),
      // { Added by Kihyung 2012.06.29
      m_pModel(NULL),
      m_pEngineVideo(engine), // added by lssanh 2012.08.24 for audio path
      m_pDeckController(NULL),
      m_pIVideo(NULL),
      // } Added by Kihyung
      m_nPendingChapter(-1),
      m_nPendingTitle(-1),
      m_onRestoreDiscState(false),
      // { added by cychoi 2013.06.24 for new DVD Settings UX
      m_nUserSubTitleLangCode(-1),
      m_nUserAudioLangCode(-1),
      m_nUserMenuLangCode(-1)
      // } added by cychoi 2013.06.24
      //m_bSelectByTitleMenu(false) // commented by cychoi 2014.06.27 for removed too many logs... // added by kihyung 2012.07.13
{
    m_pIMedia = NULL;
    m_pModel = new AppMediaPlayer_Video_CModel_DVD(this);

    m_pDeckController = controllers->GetDeckController();

    m_pModel->setPlayerScreenState(PLAYER_PLAY_DISPLAY);
    m_pModel->setPlayerState(PLAYER_STOP);
    memset(&m_storedDVDInfo,0,sizeof(m_storedDVDInfo));

    m_saveChapter = -1; // added by sungha.choi 2013.07.15 for static test
    m_bDVDMuteState = false; // modified by cychoi 2013.09.01 for ITS 183910 & 183912 Sound path if DVD or VCD //modified by yongkyun.lee 2013-07-17 for : ITS 175107
    m_isStart = false;
    m_bPlayTimeOSDAllowed = false; // added by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
    m_bDelayedMuteForSeek = false; // added by cychoi 20103.12.05 for ITS 212889 should not mute on forbidden command
    m_bDelayedLoadingMode = false; // added by cychoi 2014.01.29 for ITS 223305 delayed loading screen
    // removed by cychoi 2014.03.18 for ITS 229882 No option menu on Title/Disc Menu Screen // added by yongkyun.lee 2013-11-05 for ITS 206356
}


AppMediaPlayer_Video_ControllerDVD::~AppMediaPlayer_Video_ControllerDVD()
{
    delete m_pModel;
}


AppMediaPlayer_Video_CModel_Base *AppMediaPlayer_Video_ControllerDVD::getModel()
{
    return qobject_cast<AppMediaPlayer_Video_CModel_Base*>(m_pModel);
}


void AppMediaPlayer_Video_ControllerDVD::dropControllers()
{
    MP_LOG << LOG_ENDL;

    if ( m_pDeckController == NULL )
    {
        MP_LOG << "Inconsistent state." << LOG_ENDL;
        return;
    }

    m_pModel->setPlayerScreenState(PLAYER_PLAY_DISPLAY); // added by cychoi 2013.10.02 for ITS 192167

    disconnect( m_pDeckController, SIGNAL(OnTrackChanged(int)),
               this, SLOT(onTrackChanged(int)));

    disconnect( m_pDeckController, SIGNAL(OnScreenStateChanged(int)),
               this, SLOT(onScreenStateChanged(int)));

    disconnect( m_pDeckController, SIGNAL(OnStateChanged(int)),
               this, SLOT(onStateChanged(int)));

    disconnect(m_pDeckController, SIGNAL(OnSearchCompleted(int,int,int)),
               this, SLOT(onSearchCompleted(int,int,int)));

    disconnect( m_pDeckController, SIGNAL(OnAudioLangChanged(int)),
               this, SLOT(onActiveAudioLangChanged(int)));

    disconnect( m_pDeckController, SIGNAL(OnSubtitleLangChanged(int)),
               this, SLOT(onActiveCaptionChanged(int)));

    disconnect( m_pDeckController, SIGNAL(OnAngleChanged(int)),
               this, SLOT(onActiveAngleChanged(int)));

    disconnect( m_pDeckController, SIGNAL(OnDeckPriorityLangChanged()),
               this, SLOT(OnDeckLangChanged())); // modified by ravikanth 27-11-12

    // { added by kihyung 2012.07.13 for CR 9867
    disconnect( m_pDeckController, SIGNAL(OnError(int)),
             this, SLOT(onHandleError(int)));
    // } added by kihyung

    // { added by cychoi 20103.06.11
    disconnect( m_pDeckController, SIGNAL(OnRealSoundChanged()),
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

    // removed by Sergey for CR#15775
}

// { added by cychoi 2013.08.07 for ITS 183155 Repeat mode init error on Disc Eject
void AppMediaPlayer_Video_ControllerDVD::onDeviceRemoved()
{
    if(m_pModel) 
    {
        m_pModel->setRepeatMode(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_OFF);
        m_pModel->setRandomMode(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_OFF);
    }
}
// } added by cychoi 2013.08.07

void AppMediaPlayer_Video_ControllerDVD::attachControllers()
{
    MP_LOG << LOG_ENDL;

    if( m_pDeckController == NULL )
    {
        MP_LOG << "Inconsistent state." << LOG_ENDL;
        return;
    }

    m_pIMedia = m_pDeckController->GetMediaInstance();
    m_pIVideo = m_pDeckController->GetVideoInstance();
    m_pEngineVideo->setVideoInterface(m_pIVideo); // added by cychoi 2014.06.27 for removed too many logs...

    m_pIMedia->ChangeMediaMode( MP_MODE_DISC );

    connect( m_pDeckController, SIGNAL(OnTrackChanged(int)),
            this, SLOT(onTrackChanged(int)));
    connect( m_pDeckController, SIGNAL(OnSearchCompleted(int,int,int)),
            this, SLOT(onSearchCompleted(int,int,int)));
    connect( m_pDeckController, SIGNAL(OnAudioLangChanged(int)),
            this, SLOT(onActiveAudioLangChanged(int)));
    connect( m_pDeckController, SIGNAL(OnSubtitleLangChanged(int)),
            this, SLOT(onActiveCaptionChanged(int)));
    connect( m_pDeckController, SIGNAL(OnAngleChanged(int)),
            this, SLOT(onActiveAngleChanged(int)));
    connect( m_pDeckController, SIGNAL(OnDeckPriorityLangChanged()),
               this, SLOT(OnDeckLangChanged())); // modified by ravikanth 27-11-12
    connect( m_pDeckController, SIGNAL(OnScreenStateChanged(int)),
            this, SLOT(onScreenStateChanged(int)));
    connect( m_pDeckController, SIGNAL(OnStateChanged(int)),
            this, SLOT(onStateChanged(int)));
    // { added by kihyung 2012.07.13 for CR 9867
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
    //        this, SLOT(onAutoTrackChanged(int))); // added by cychoi 2014.04.06 for ITS 233500

    // { added by cychoi 2013.09.24 for ITS 191333
    // { modified by cychoi 2013.10.01 for ITS 192167
    m_pModel->setPlayerScreenState(m_pIVideo->GetCurrentScreenState());
    //pModel->setPlayerScreenState(PLAYER_PLAY_DISPLAY);
    // } modified by cychoi 2013.10.01
    m_pModel->setChapter(0);
    m_pModel->setTitle(0);
    // } added by cychoi 2013.09.24

    /*To start playing every time when aquire source*/
    //{ modified by yongkyun.lee 2013-07-17 for : ITS 175107
    //setAudioPath();
    m_bDVDMuteState = true; // modified by cychoi 2013.09.01 for ITS 183910 & 183912 Sound path if DVD or VCD 
	m_isStart = true;
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
        // removed by Sergey 18.05.2013
    }
    // } modified by kihyung

    // removed by Sergey for CR#15775
}


void AppMediaPlayer_Video_ControllerDVD::onTrackChanged(int arg)
{
    MP_LOG << LOG_ENDL;
    Q_UNUSED(arg);
    //added by edo.lee 2013.06.11
    // { deleted by lssanh 2013.06.26 bt call 
    //if(m_pEngineVideo->getEngineMain().getisBTCall() == true) 
    //{
    //   pause();
    //   m_pModel->setPosition(0);//added by edo.lee2013.06.19
    //}
    // } deleted by lssanh 2013.06.26 bt call 
    //added by edo.lee 2013.06.11
    getDVDInfo(arg); // modified by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
}


void AppMediaPlayer_Video_ControllerDVD::onSearchCompleted(int nTitleNo,
                                                           int nChapterNo,
                                                           int nChapterSize)
{
    MP_LOG << "nTitleNo " << nTitleNo << "nChapterNo " << nChapterNo << "nChapterSize " << nChapterSize << LOG_ENDL;

    //m_pEngineVideo->getEngineMain().setUnMuteState(false); // added by cychoi 2013.07.05 for SelectTuned Mute

    if(nChapterSize <= 0 || nChapterNo <= 0 || nTitleNo <= 0)
    {
        MP_LOG << "Invalid values. Exit." << LOG_ENDL;
        // { added by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
        m_pEngineVideo->getEngineMain().setUnMuteState(false); // added by cychoi 2013.07.05 for SelectTuned Mute
        m_bPlayTimeOSDAllowed = false; // added by cychoi 2013.10.20 for ITS 197025 Should not update OSD on Tune Wheel
        // } added by cychoi 2013.10.20
        return;
    }
    // { added by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
    else
    {
        if(m_pModel->chapter() == nChapterNo && m_pModel->title() == nTitleNo)
        {
            m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false); // added by yungi 2013.08.10 for ITS 183835 No OSD on track change automatically
            m_bPlayTimeOSDAllowed = false;
            m_pEngineVideo->getEngineMain().setUnMuteState(false); // added by cychoi 2013.07.05 for SelectTuned Mute
        }
    }
    // } added by cychoi 2013.10.20

    m_pModel->setChapterCount(nChapterSize);
    m_pModel->setChapter(nChapterNo);
    m_pModel->setTitle(nTitleNo);

    // { added by kihyung 2012.07.29 for CR 11057
    // If DVD is pause state, and then play.
    if(m_storedDVDInfo.playStatus == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PAUSED && m_onRestoreDiscState == false) {
        m_pIMedia->Play();
    }

    // Restore mode
    if(m_onRestoreDiscState)
    {
        // If screen is play screen, and then change position.
        if(m_storedDVDInfo.displayState == 0) {
            MP_LOG << "Restore Disc setPosition " << m_pModel->resumePosition() << LOG_ENDL;
            setPosition(m_pModel->resumePosition());
        }
        m_onRestoreDiscState = false;
        m_pModel->setResumePosition(0);

        // If screen is play screen and DVD is pause state, and then pause.
        if(m_storedDVDInfo.playStatus == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PAUSED && m_storedDVDInfo.displayState == 0) {
            m_pIMedia->Pause();
        }
    }
    // } added by kihyung
}


void AppMediaPlayer_Video_ControllerDVD::restoreDiscState()
{
    MP_LOG << LOG_ENDL;

    // { modified by kihyung 2012.07.29 for CR 11057
    /*
    selectTitleChapter( m_storedDVDInfo.groupOrTitleNum,
                       m_storedDVDInfo.trackOrChapterNo );

    if ( m_storedDVDInfo.playStatus == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PAUSED )
    {
        pause();
    }
    m_onRestoreDiscState = true;
    */
    // the order is important.
    
	// { deleted by wspark 2013.03.09 for ISV 65056
	/*
    if(m_pModel->playerState() != PLAYER_PAUSE) {
        m_pModel->setLoadingMode(true); // added by Sergey for Loading popup
    }
    m_onRestoreDiscState = true;
    m_pIVideo->SearchTitleChapter(m_storedDVDInfo.groupOrTitleNum, m_storedDVDInfo.trackOrChapterNo);
	*/
	// } deleted by wspark
    // } modified by kihyung
}


void AppMediaPlayer_Video_ControllerDVD::storeDiscState()
{
    MP_LOG << LOG_ENDL;
    m_pModel->setResumePosition(m_pModel->position());
    memset(&m_storedDVDInfo,0,sizeof( m_storedDVDInfo));
    m_pIVideo->GetDVDStatus(& m_storedDVDInfo);
    m_storedDVDInfo.playStatus = m_pModel->playbackStatus();
    // { commentted by kihyung 2012.07.29 for CR 11057
    /*
    if(m_storedDVDInfo.playStatus == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PAUSED)
    {
        play();
    }
    */
    // } commentted by kihyung
}


void AppMediaPlayer_Video_ControllerDVD::titleMenu()
{
    // { modified  by Kihyung 2012.07.13 for CR 9867
    if(m_pIVideo) {
        // { modified by cychoi 2013.08.30 for ITS 187026 DVD Title/Disc Menu Loading Screen
        if(m_pModel->playerState() == PLAYER_PAUSE)
        {
            m_pEngineVideo->getEngineMain().setUnMuteState(false);//added by edo.lee 2013.05.19
            play();
        }
        m_pIVideo->Titlemenu(1);
        // { modified by cychoi 2013.11.23 for ITS 210743 Loading Screen only when screen is changed
        int nPlayerScreenState = m_pModel->playerScreenState();
        if(nPlayerScreenState == PLAYER_PLAY_DISPLAY ||
           nPlayerScreenState == PLAYER_DEFAULTSCREEN_DISPLAY)
        {
            // { modified by cychoi 2014.01.29 for ITS 223305 delayed loading screen
            m_bDelayedLoadingMode = true; 
            //getModel()->setLoadingMode(true);
            //m_pEngineVideo->GetVideoController()->sendTrackClusterChanged(0xffff, 0); // added by cychoi 2013.11.28 for ITS 211634
            // } modified by cychoi 2014.01.29
        }
        // } modified by cychoi 2013.11.23
        // } modified by cychoi 2013.08.30
    }
    // } modified by kihyung
}


void AppMediaPlayer_Video_ControllerDVD::topMenu()
{
    // { modified  by Kihyung 2012.07.13 for CR 9867
    if(m_pIVideo) {
        // { modified by cychoi 2013.08.30 for ITS 187026 DVD Title/Disc Menu Loading Screen
        if(m_pModel->playerState() == PLAYER_PAUSE)
        {
            m_pEngineVideo->getEngineMain().setUnMuteState(false);//added by edo.lee 2013.05.19
            play();
       	}
        m_pIVideo->TopMenu(1);
        // { modified by cychoi 2013.11.23 for ITS 210743 Loading Screen only when screen is changed
        int nPlayerScreenState = m_pModel->playerScreenState();
        if(nPlayerScreenState == PLAYER_PLAY_DISPLAY ||
           nPlayerScreenState == PLAYER_DEFAULTSCREEN_DISPLAY)
        {
            // { modified by cychoi 2014.01.29 for ITS 223305 delayed loading screen
            m_bDelayedLoadingMode = true;
            //getModel()->setLoadingMode(true);
            //m_pEngineVideo->GetVideoController()->sendTrackClusterChanged(0xffff, 0); // added by cychoi 2013.11.28 for ITS 211634
            // } modified by cychoi 2014.01.29
        }
        // } modified by cychoi 2013.11.23
        // } modified by cychoi 2013.08.29
    }
    // } modified by kihyung
}


void AppMediaPlayer_Video_ControllerDVD::selectTitleChapter(int title, int chapter)
{
    MP_LOG << "Going to switch to chapter" << chapter << "title" << title << LOG_ENDL;
    if(!( m_pModel->title() == title && ( chapter == 0 || chapter == m_pModel->chapter())))
    {
        m_pIVideo->SearchTitleChapter( title, chapter );
    }
}


void AppMediaPlayer_Video_ControllerDVD::selectChapter(int chapter)
{
    selectTitleChapter(m_pModel->title(),chapter);
}


// { modified by cychoi 2013.07.07 for ISV 85460 DVD OSD mismatch
void AppMediaPlayer_Video_ControllerDVD::getDVDInfo(int trackIndex)
//void AppMediaPlayer_Video_ControllerDVD::getDVDInfo()
// } modified by cychoi 2013.07.07
{
    MP_MEDIUM << LOG_ENDL;

    if(m_pIVideo)
    {
        DVDStatusResponse DVDInfo;
        memset(&DVDInfo,0,sizeof( DVDInfo));
        m_pIVideo->GetDVDStatus(&DVDInfo);

        //{ modified by yungi 2013.08.10 for ITS 183835 No OSD on track change automatically
        //m_pEngineVideo->DisplayOSD(true);//added by yongkyun.lee 20130402 for : ISV roll back
        if(trackIndex >= 0)
        {
            // { modified by cychoi 2013.07.07 for ISV 85460 DVD OSD mismatch
            // This code is temporary code.
            // TODO: We should move this function to onStateChanged or onTrackChanged as like MP3 and CDDA.
            DVDInfo.trackOrChapterNo = trackIndex + 1;
            // } modified by cychoi 2013.07.07

            // { added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
            m_pEngineVideo->GetVideoController()->sendMediaInfoClusterChanged(QString::number(DVDInfo.trackOrChapterNo));
            m_pEngineVideo->GetVideoController()->sendTrackClusterChanged(DVDInfo.trackOrChapterNo, 0);
            // } added by cychoi 2013.08.17

            // { modified by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
            if(m_nPendingChapter == -1 || m_nPendingChapter == m_saveChapter)
            {
                // { modified by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
                int nPlayerScreenState = m_pModel->playerScreenState();
                if(m_bPlayTimeOSDAllowed == true ||
                   nPlayerScreenState == PLAYER_PLAY_DISPLAY ||
                   nPlayerScreenState == PLAYER_DEFAULTSCREEN_DISPLAY)
                {
                    m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false);	
                    if(m_isStart)
                    {
                        //{ modified by yongkyun.lee 2013-07-26 for : somke test DVD OSD           
                        m_pEngineVideo->setOSDMetaData(DISC_DVD_VIDEO,
                                getOSDTitle(QString("%1").arg(DVDInfo.groupOrTitleNum), QString("%1").arg(DVDInfo.trackOrChapterNo)), // modified by cychoi 2014.02.21 for ITS 226393
                                RANDOMOFF,REPEATOFF,0, false); // modified by cychoi 2013.11.12 for OSD Title Translation
                        //} modified by yongkyun.lee 2013-07-26 
                        m_isStart = false;			
                    }else{
                        m_pEngineVideo->updateOSDMetaData(DISC_DVD_VIDEO,
                                getOSDTitle(QString("%1").arg(DVDInfo.groupOrTitleNum), QString("%1").arg(DVDInfo.trackOrChapterNo))); // modified by cychoi 2014.02.21 for ITS 226393 // modified by cychoi 2013.11.12 for OSD Title Translation
                    }
                    m_bPlayTimeOSDAllowed = false;
                }
                // } modified by cychoi 20103.10.16
            }
            else //if(m_nPendingChapter != -1)
            {
                m_saveChapter = DVDInfo.trackOrChapterNo;
            }
            // } modified by cychoi 20103.10.20
        }

        MP_LOG << "Chapter =" << DVDInfo.trackOrChapterNo <<
                 "Title =" << DVDInfo.groupOrTitleNum <<
                 "ChapterCount =" << DVDInfo.totalChaptorOrTrack <<
                 "TitleCount =" << DVDInfo.totalGroupOrTitle <<
                 "Angle =" << DVDInfo.angleNo <<
                 // { modified by ravikanth CR 15994 12-11-22
                 "AngleCount =" << DVDInfo.totalAngleNo<<
                 "audiocode =" << DVDInfo.audioCode << DVDInfo.audioAttribute << DVDInfo.audioAttribute <<
                 "caption =" << DVDInfo.subTitleCode << LOG_ENDL;
                 // } modified by ravikanth CR 15994 12-11-22
        //} modified by yungi 2013.08.10

        m_pModel->setChapter(DVDInfo.trackOrChapterNo);
        m_pModel->setTitle(DVDInfo.groupOrTitleNum);
        m_pModel->setChapterCount(DVDInfo.totalChaptorOrTrack);
        m_pModel->setTitleCount(DVDInfo.totalGroupOrTitle);
        // For 'angleNo' first element is '1'. We expect '0'.
        // { modified by ravikanth CR 15994 12-11-22
        //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
        m_pModel->setActiveCaption(DVDInfo.subTitleNum, DVDInfo.subTitleCode);
        m_pModel->setActiveAudioLang(DVDInfo.audioSetting, DVDInfo.audioCode, DVDInfo.audioAttribute, DVDInfo.channel, DVDInfo.LFEExist); // added by cychoi 2013.07.20 for ITS 181110 Audio attribute
        //} modified by cychoi 2013.10.06
        m_pModel->setActiveAngle(DVDInfo.angleNo);
        // } modified by ravikanth CR 15994 12-11-22
    }
}
// added by minho 20120828
// { for able list menu on DVD
int AppMediaPlayer_Video_ControllerDVD::getChapterCount()
{
   return m_pModel->chapterCount();
}
// } added by minho

void AppMediaPlayer_Video_ControllerDVD::cursorKey(int key)
{
    if(m_pIVideo)
        m_pIVideo->CursorKey(key);
}


void AppMediaPlayer_Video_ControllerDVD::select()
{
    if(m_pIVideo)
    {
        // { added by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
        // { modified by cychoi 20103.12.05 for ITS 212889 should not mute on forbidden command
        if(m_pModel->playerScreenState() == PLAYER_PLAY_DISPLAY)
        {
            m_bDelayedMuteForSeek = true;
        }
        else
        {
            m_bDelayedMuteForSeek = false;
            m_pEngineVideo->getEngineMain().setMuteForSeek(false); // added by cychoi 20103.08.12 for ITS 183910 Sound mute on Enter Key
        }
        // } modified by cychoi 20103.12.05
        m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(true);
        m_bPlayTimeOSDAllowed = true;
        // } added by cychoi 20103.10.16
        m_pIVideo->EnterKey();
    }
}

// { modified by ravikanth 27-11-12
void AppMediaPlayer_Video_ControllerDVD::requestDefaultLanguagesSet()
{
    ///MP_LOG << "m_playerScreenState=" << m_pModel->playerScreenState() << LOG_ENDL;
    DeckEngineerMode  *pDeckEngine = m_pEngineVideo->getEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();
    EDeckInit1Setting  tSetting;
    pDeckEngine->GetInitSetting1(tSetting);
    MP_LOG << "m_playerScreenState=" << m_pModel->playerScreenState() << " m_nPrioSubTitleCode=" << tSetting.m_nPrioSubTitleCode <<
              " m_nPrioAudioCode=" << tSetting.m_nPrioAudioCode << " m_nPrioMenuLangCode=" << tSetting.m_nPrioMenuLangCode << LOG_ENDL;
    //MP_LOG << tSetting.m_nPrioSubTitleCode << LOG_ENDL;
    //MP_LOG << tSetting.m_nPrioAudioCode << LOG_ENDL;
    //MP_LOG << tSetting.m_nPrioMenuLangCode << LOG_ENDL;
    // { added by cychoi 2013.06.24 for new DVD Settings UX
    if(m_nUserSubTitleLangCode != -1)
    {
        tSetting.m_nPrioSubTitleCode = m_nUserSubTitleLangCode;
    }
    else
    {
        m_nUserSubTitleLangCode = tSetting.m_nPrioSubTitleCode;
    }

    if(m_nUserAudioLangCode != -1)
    {
        tSetting.m_nPrioAudioCode = m_nUserAudioLangCode;
    }
    else
    {
        m_nUserAudioLangCode = tSetting.m_nPrioAudioCode;
    }

    if(m_nUserMenuLangCode != -1)
    {
        tSetting.m_nPrioMenuLangCode = m_nUserMenuLangCode;
    }
    else
    {
        m_nUserMenuLangCode = tSetting.m_nPrioMenuLangCode;
    }
    // } added by cychoi 2013.06.24
    SendVideoControlData( tSetting.m_nPrioSubTitleCode, tSetting.m_nPrioAudioCode,
                          tSetting.m_nPrioMenuLangCode);
}
// } modified by ravikanth 27-11-12

void AppMediaPlayer_Video_ControllerDVD::onActiveAudioLangChanged(int nAudioCode)
{
    // removed by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute

    if(!m_pIVideo)
    {
        MP_LOG << "m_pIVideo is NULL" << LOG_ENDL;
        return;
    }

    //MP_LOG << "nAudioCode =" << nAudioCode << LOG_ENDL;

    /* Seems we never get this signal. Just in case handle it.
     * But we expect audio code not index. So get it.
     */
    // Let's get active audio lang code.
    DVDStatusResponse DVDInfo;
    memset(&DVDInfo,0,sizeof( DVDInfo));
    m_pIVideo->GetDVDStatus(&DVDInfo);

    //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
    MP_LOG << "nAudioCode =" << nAudioCode << " active audioLang no =" << DVDInfo.audioSetting << LOG_ENDL;

    m_pModel->setActiveAudioLang(DVDInfo.audioSetting, DVDInfo.audioCode, DVDInfo.audioAttribute, DVDInfo.channel, DVDInfo.LFEExist); // added by cychoi 2013.07.20 for ITS 181110 Audio attribute// modified by ravikanth CR 15994 12-11-22
    //} modified by cychoi 2013.10.06
}


void AppMediaPlayer_Video_ControllerDVD::onActiveCaptionChanged(int nSubtitleCode)
{
    //MP_LOG << "subtitle code =" << nSubtitleCode << LOG_ENDL;

    //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
    // Let's get active caption lang no.
    DVDStatusResponse DVDInfo;
    memset(&DVDInfo,0,sizeof( DVDInfo));
    m_pIVideo->GetDVDStatus(&DVDInfo);

    MP_LOG << "subtitle code =" << nSubtitleCode << " active subtitle no =" << DVDInfo.subTitleNum << LOG_ENDL;

    m_pModel->setActiveCaption(DVDInfo.subTitleNum, nSubtitleCode);
    //} modified by cychoi 2013.10.06
}


void AppMediaPlayer_Video_ControllerDVD::onActiveAngleChanged(int nAngle)
{
    MP_LOG << "nAngle =" << nAngle << LOG_ENDL;

    m_pModel->setActiveAngle(nAngle); // modified by ravikanth CR 15994 12-11-22
}


void AppMediaPlayer_Video_ControllerDVD::onScreenStateChanged( int nScreenState )
{
    MP_LOG << "nScreenState =" << nScreenState << LOG_ENDL;
	
	// commented by ravikanth 31-03-13 ISV 77717
    // m_pModel->setLoadingMode(false); // added by kihyung 2013.1.28 for ISV 71502
    if(nScreenState == 0 ||
       m_pEngineVideo->isDVDTrackPlaying()) // added by cychoi 2013.09.04 for ITS 186733
    {
	    m_isStart = true;
    }

    // { added by cychoi 2014.03.18 for ITS 229882
    if(m_nPendingTitle != -1 && m_nPendingChapter != -1)
    {
        if(nScreenState == PLAYER_TOPMENU_DISPLAY  ||
           nScreenState == PLAYER_ROOTMENU_DISPLAY ||
           nScreenState == PLAYER_TITLEMENU_DISPLAY)
        {
            cancelTune();
        }
    }
    // } added by cychoi 2014.03.18

    m_pModel->setPlayerScreenState(nScreenState);
    if( m_pModel->playerScreenState() != PLAYER_PLAY_DISPLAY )
    {
        cleanDVDSettingsInfo();
    }

    //{ modified by yongkyun.lee 2013-11-05 for ITS 206356
    if( nScreenState == PLAYER_TOPMENU_DISPLAY  ||
        nScreenState == PLAYER_ROOTMENU_DISPLAY ||
        nScreenState == PLAYER_TITLEMENU_DISPLAY)
    {
        // removed by cychoi 2014.03.18 for ITS 229882 No option menu on Title/Disc Menu Screen //{ added by yongkyun.lee 2013-11-05 for ITS 206356
        emit screenStateChanged(nScreenState); // moved by cychoi 2014.01.29 for ITS 223305 delayed loading screen
        // { added by cychoi 2014.01.29 for ITS 223305 delayed loading screen
        if(m_bDelayedLoadingMode == true)
        {
            getModel()->setLoadingMode(true);
            m_bDelayedLoadingMode = false;
            m_pEngineVideo->GetVideoController()->sendTrackClusterChanged(0xffff, 0); // added by cychoi 2013.11.28 for ITS 211634
        }
        else
        {
            // { added by cychoi 2013.11.28 for ITS 211634
            if(!m_pEngineVideo->isDVDTrackPlaying())
            {
                m_pEngineVideo->GetVideoController()->sendTrackClusterChanged(0xffff, 0);
            }
            // } added by cychoi 2013.11.28
        }
        // } added by cychoi 2014.01.29
    }
    else
    {
        // removed by cychoi 2014.03.18 for ITS 229882 No option menu on Title/Disc Menu Screen //{ added by yongkyun.lee 2013-11-05 for ITS 206356
        emit screenStateChanged(nScreenState); // moved by cychoi 2014.01.29 for ITS 223305 delayed loading screen
    }
    //} modified by yongkyun.lee
}


void AppMediaPlayer_Video_ControllerDVD::onStateChanged(int nState)
{
    // { modified by kihyung 2013.1.28
    MP_MEDIUM << "nState =" << nState << " m_playerState =" << m_pModel->playerState()  << LOG_ENDL;

    //{ modified by cychoi 2013.09.01 for ITS 183910 & 183912 Sound path if DVD or VCD
    //{ modified by yongkyun.lee 2013-07-16 for : ITS 175107
    if( nState == PLAYER_PLAY )
    {
        if(m_bDVDMuteState == true)
        {
            m_bDVDMuteState = false;
            m_pEngineVideo->modeTabClicked( true);
            setAudioPath();
        }

        // { added by cychoi 2013.10.14 for ITS 195547 Display Title/Disc menu OSD on AV Off
        if( m_pModel->playerScreenState() != PLAYER_PLAY_DISPLAY )
        {
            if(m_pEngineVideo->getScreenDVDModeChanged() && !m_pEngineVideo->isDVDTrackPlaying()) // added by cychoi 2013.09.04 for ITS 186733
            {
                OSDManager::instance()->displayDVDMenuMode();
            }
            m_pEngineVideo->setScreenDVDModeChanged(false);// modified by yongkyun.lee 2013-07-22 for : NO CR DVD title OSD
        }
        // } added by cychoi 2013.10.14

        m_bDelayedMuteForSeek = false; // added by cychoi 20103.12.05 for ITS 212889 should not mute on forbidden command
        m_bDelayedLoadingMode = false; // added by cychoi 2014.01.29 for ITS 223305 delayed loading screen
    }
    //} modified by yongkyun.lee 2013-07-16 
    //} modified by cychoi 2013.08.27
    else if(nState == PLAYER_STOP)
    {
        m_bDVDMuteState = true; // modified by cychoi 2013.09.01 for ITS 183910 & 183912 Sound path if DVD or VCD // modified by yongkyun.lee 2013-07-16 for : ITS 175107
        cleanDVDSettingsInfo();
    }
    // { modified by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
    else if( nState == PLAYER_FASTFORWARD ||
             nState == PLAYER_REWIND)
    {
        int nPlayerScreenState = m_pModel->playerScreenState();
        if(nPlayerScreenState != PLAYER_PLAY_DISPLAY &&
           nPlayerScreenState != PLAYER_DEFAULTSCREEN_DISPLAY)
        {
            m_bDelayedMuteForSeek = false; // added by cychoi 20103.12.05 for ITS 212889 should not mute on forbidden command
            m_pEngineVideo->getEngineMain().setMuteForSeek(false); // added by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute
            m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false); // added by cychoi 2013.09.04 for OSD on FF
        }
    }
    // } modified by cychoi 20103.10.16
    // { added by cychoi 20103.12.05 for ITS 212889 should not mute on forbidden command
    else if( nState == PLAYER_SEARCH )
    {
        // { modified by cychoi 2014.04.14 for HW Event DV2-1 DVD sound truncated
        if(m_bDVDMuteState == true)
        {
            m_bDVDMuteState = false;
            m_pEngineVideo->modeTabClicked( true);
            setAudioPath();
        }
        // } modified by cychoi 2014.04.14

        if(m_bDelayedMuteForSeek == true)
        {
            m_pEngineVideo->getEngineMain().setMuteForSeek(false);
            m_bDelayedMuteForSeek = false;
        }
    }
    // } added by cychoi 20103.12.05

    // removed by Sergey 18.05.2013
    
    m_pModel->setPlayerState(nState);
    // } modified by kihyung 2013.1.28    

    // { added by cychoi 2013.11.10 for ITS 207831 Control Cue Icon on Tune Mode
    if( nState == PLAYER_PLAY || nState == PLAYER_SEARCH )
    {
        if(m_nPendingTitle != -1 && m_nPendingChapter != -1)
        {
            m_pEngineVideo->setTuneModeOnStateChanged(m_nPendingChapter == m_saveChapter);
        }
    }
    // } added by cychoi 2013.11.10
}

void AppMediaPlayer_Video_ControllerDVD::onHandleError(int nError)
{
    MP_LOG << "nError =" << nError << LOG_ENDL;
    
    if(nError == PLAYER_COMMANDNOTSUPPORTED)
    {
        // { modified by sjhyun 2013.11.05 for ITS 206357
        //emit showInhibitionIcon(true);
        m_pEngineVideo->inhibitionIcon(true);
        // } modified by sjhyun
        m_bDelayedMuteForSeek = false; // added by cychoi 20103.12.05 for ITS 212889 should not mute on forbidden command
    }

    // { added by cychoi 2013.10.03 for ITS 193259 Infinite loading screeen on Error
    if(m_pModel->loadingMode() == true)
    {
        m_pModel->setLoadingMode(false);
    }
    m_bDelayedLoadingMode = false; // added by cychoi 2014.01.29 for ITS 223305 delayed loading screen
    // } added by cychoi 2013.10.03
}

// { added by cychoi 20103.06.11
void AppMediaPlayer_Video_ControllerDVD::onRealSoundChanged()
{
    MP_LOG << LOG_ENDL;
    // removed by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
    // { modified by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
    int nPlayerScreenState = m_pModel->playerScreenState();
    if(m_bPlayTimeOSDAllowed == true ||
       nPlayerScreenState == PLAYER_PLAY_DISPLAY ||
       nPlayerScreenState == PLAYER_DEFAULTSCREEN_DISPLAY)
    {
        m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false); // added by yungi 2013.08.10 for ITS 183835 No OSD on track change automatically
        m_bPlayTimeOSDAllowed = false;
    }
    // } modified by cychoi 20103.10.16
    m_pEngineVideo->getEngineMain().setUnMuteState(false);
}
// } added by cychoi 20103.06.11

// { modified by cychoi 2013.06.18 for ISV 85460 DVD OSD mismatch
void AppMediaPlayer_Video_ControllerDVD::OnTrackBufferred(int bPrev, int nTotalIdx, int nIdxInFolder, int nFolderSize)
{
    MP_LOG << LOG_ENDL;
    Q_UNUSED(bPrev); Q_UNUSED(nIdxInFolder); Q_UNUSED(nFolderSize);
    getDVDInfo(nTotalIdx);
}
// } modified by cychoi 2013.06.18

void AppMediaPlayer_Video_ControllerDVD::onUserTrackChanged(int trackindex)
{
    MP_LOG << LOG_ENDL;
    if(m_pEngineVideo->GetVideoController()->getSource() != MEDIA_SOURCE_DVDVIDEO
        ||  m_pEngineVideo->getEngineMain().GetLastPlayerMode() != APP_MEDIA_MODE_VIDEO)
    {
        MP_LOG<<"mode is not matched" << LOG_ENDL;
        return;
    }

    //{ added by yungi 2013.08.10 for  ITS 183571 Chapter No display
    //{ modified by yongkyun.lee 2013-07-26 for : somke test DVD OSD
    int trackOrChapterNo = trackindex + 1;
    int groupOrTitleNo = m_pModel->title(); // modified by cychoi 2014.02.24 for ITS 226393

    // { modified by cychoi 2014.02.24 for ITS 226393
    if(groupOrTitleNo <= 0)
    {
        DVDStatusResponse DVDInfo;
        memset(&DVDInfo,0,sizeof( DVDInfo));
        m_pIVideo->GetDVDStatus(&DVDInfo);
        groupOrTitleNo = DVDInfo.groupOrTitleNum;
    }
    // } modified by cychoi 2014.02.24

    m_pEngineVideo->setOSDMetaData(DISC_DVD_VIDEO,
                    getOSDTitle(QString("%1").arg(groupOrTitleNo), QString("%1").arg(trackOrChapterNo)), // modified by cychoi 2014.02.21 for ITS 226393
                    RANDOMOFF,REPEATOFF,0, false); // modified by cychoi 2013.11.12 for OSD Title Translation
    //} modified by yongkyun.lee 2013-07-26 
    //} added by yungi 2013.08.10

    // { added by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
    if(m_pModel->chapter() == trackOrChapterNo)
    {
        // Track donw: over 3 sec in current TRACK ¡æ current TRACK -> Send OnUserTrackChanged signal
        m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false);
    }
    // } added by cychoi 2013.10.20

    m_pEngineVideo->getEngineMain().sendVRCancelSignal(); // added by cychoi 2014.03.12 for ITS 224783 VR cancel on onUserTrackChanged
}

// { added by cychoi 2014.04.06 for ITS 233500
//void AppMediaPlayer_Video_ControllerDVD::onAutoTrackChanged(int index)
//{
//    Q_UNUSED(index)
//}
// } added by cychoi 2014.04.06

QString AppMediaPlayer_Video_ControllerDVD::getOSDTitle(QString title, QString chapter) // modified by cychoi 2014.02.21 for ITS 226393
{
    return (QApplication::translate("main", "STR_MEDIA_TITLE") + " " + title + "   " +
            QApplication::translate("main", "STR_MEDIA_CHAPTER") + " " + chapter); // modified by cychoi 2014.02.21 for ITS 226393
}

void AppMediaPlayer_Video_ControllerDVD::ShowCurrentModelToOSD()
{
    MP_LOG << LOG_ENDL;
    // { commented by cychoi 2013.10.11 for ITS 195154 No OSD if operation is forbidden
    //{ modified by yongkyun.lee 2013-07-26 for : somke test DVD OSD
    //m_pEngineVideo->setOSDMetaData(DISC_DVD_VIDEO,
    //    getOSDTitle(QString("%1").arg(m_pModel->title()), QString("%1").arg(m_pModel->chapter())), // modified by cychoi 2014.02.21 for ITS 226393
    //    RANDOMOFF,REPEATOFF,0, false); // modified by cychoi 2013.11.12 for OSD Title Translation
    //} modified by yongkyun.lee 2013-07-26 
    // } commented by cychoi 2013.10.11
}

// { added by cychoi 2013.07.22 for for DVD Seek Up/Down mute
void AppMediaPlayer_Video_ControllerDVD::prev(bool bForce)
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
        
        // { modified by cychoi 20103.12.05 for ITS 212889 should not mute on forbidden command
        m_bDelayedMuteForSeek = true;
        //m_pEngineVideo->getEngineMain().setMuteForSeek(false);
        // } modified by cychoi 20103.12.05
        m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(true);
        m_bPlayTimeOSDAllowed = true; // added by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
        m_pIMedia->Prev(1, bForce);

        // { commented by cychoi 2013.10.11 for ITS 195154 No OSD if operation is forbidden
        //if(bForce == false)
        //   ShowCurrentModelToOSD();
        // } commented by cychoi 2013.10.11
    }
}

void AppMediaPlayer_Video_ControllerDVD::next()
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
        
        // { modified by cychoi 20103.12.05 for ITS 212889 should not mute on forbidden command
        m_bDelayedMuteForSeek = true;
        //m_pEngineVideo->getEngineMain().setMuteForSeek(false);
        // } modified by cychoi 20103.12.05
        m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(true);
        m_bPlayTimeOSDAllowed = true; // added by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
        m_pIMedia->Next(1);
    }
}
// } added by cychoi 2013.07.22

// { modified by kihyung 2013.1.10 for ISV 67946
void AppMediaPlayer_Video_ControllerDVD::setAudioLanguage(int code)
{
    MP_LOG << "code =" << code << LOG_ENDL;

    // { modified by cychoi 2013.06.24 for new DVD Settings UX
    if(m_nUserAudioLangCode != code)
    {
        m_nUserAudioLangCode = code;
    }
    //DeckEngineerMode  *pDeckEngine = m_pEngineVideo->getEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();
    //EDeckInit1Setting  tSetting;
    //pDeckEngine->GetInitSetting1(tSetting);
    //tSetting.m_nPrioAudioCode = code;
    //pDeckEngine->SetInitSetting1(tSetting);
    // } modified by cychoi 2013.06.24
}

void AppMediaPlayer_Video_ControllerDVD::setSubtitleLanguage(int code)
{
    MP_LOG << "code =" << code << LOG_ENDL;
    // { modified by cychoi 2013.06.24 for new DVD Settings UX
    if(m_nUserSubTitleLangCode != code)
    {
        m_nUserSubTitleLangCode = code;
    }
    //DeckEngineerMode  *pDeckEngine = m_pEngineVideo->getEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();
    //EDeckInit1Setting  tSetting;
    //pDeckEngine->GetInitSetting1(tSetting);
    //tSetting.m_nPrioSubTitleCode = code;
    //pDeckEngine->SetInitSetting1(tSetting);    
    // } modified by cychoi 2013.06.19
}

void AppMediaPlayer_Video_ControllerDVD::setMenuLanguage(int code)
{
    MP_LOG << "code =" << code << LOG_ENDL;

    // { modified by cychoi 2013.06.24 for new DVD Settings UX
    if(m_nUserMenuLangCode != code)
    {
        m_nUserMenuLangCode = code;
    }
    //DeckEngineerMode  *pDeckEngine = m_pEngineVideo->getEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();
    //EDeckInit1Setting  tSetting;
    //pDeckEngine->GetInitSetting1(tSetting);
    //tSetting.m_nPrioMenuLangCode = code;
    //pDeckEngine->SetInitSetting1(tSetting);
    // } modified by cychoi 2013.06.24
}

// { added by cychoi 2013.06.24 for new DVD Settings UX
void AppMediaPlayer_Video_ControllerDVD::setDVDSettingsLanguages()
{
    MP_LOG << "m_nSubTitleLangCode =" << m_nUserSubTitleLangCode << " m_nAudioLangCode =" << m_nUserAudioLangCode <<
              " m_nMenuLangCode =" << m_nUserMenuLangCode << LOG_ENDL;
    //MP_LOG << "m_nSubTitleLangCode =" << m_nUserSubTitleLangCode << LOG_ENDL;
    //MP_LOG << "m_nAudioLangCode =" << m_nUserAudioLangCode << LOG_ENDL;
    //MP_LOG << "m_nMenuLangCode =" << m_nUserMenuLangCode << LOG_ENDL;
    DeckEngineerMode  *pDeckEngine = m_pEngineVideo->getEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();
    EDeckInit1Setting  tSetting;
    pDeckEngine->GetInitSetting1(tSetting);
    if(tSetting.m_nPrioSubTitleCode != m_nUserSubTitleLangCode ||
       tSetting.m_nPrioAudioCode != m_nUserAudioLangCode ||
       tSetting.m_nPrioMenuLangCode != m_nUserMenuLangCode)
    {
        if(m_nUserSubTitleLangCode != -1)
        {
           tSetting.m_nPrioSubTitleCode = m_nUserSubTitleLangCode;
           m_nUserSubTitleLangCode = -1; // added by cychoi 2013.08.23 for ITS 181382 EVT_INITIALIZATION_REQUEST
        }

        if(m_nUserAudioLangCode != -1)
        {
            tSetting.m_nPrioAudioCode = m_nUserAudioLangCode;
            m_nUserAudioLangCode = -1; // added by cychoi 2013.08.23 for ITS 181382 EVT_INITIALIZATION_REQUEST
        }

        if(m_nUserMenuLangCode != -1)
        {
            tSetting.m_nPrioMenuLangCode = m_nUserMenuLangCode;
            m_nUserMenuLangCode = -1; // added by cychoi 2013.08.23 for ITS 181382 EVT_INITIALIZATION_REQUEST
        }

        pDeckEngine->SetInitSetting1(tSetting);
        getModel()->setLoadingMode(true); // added by cychoi 2013.08.30 for ITS 187026 DVD Title/Disc Menu Loading Screen & ITS 187122 DVD Black after DVD Setting
    }
}
// } added by cychoi 2013.06.24

void AppMediaPlayer_Video_ControllerDVD::setAngle(int code)
{
    MP_LOG << "code =" << code << LOG_ENDL;
    DeckEngineerMode  *pDeckEngine = m_pEngineVideo->getEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();
    EDeckInit1Setting  tSetting;
    pDeckEngine->GetInitSetting1(tSetting);
    tSetting.m_nAngleDisplay = code;
    pDeckEngine->SetInitSetting1(tSetting);
}
// } modified by kihyung 2013.1.10 for ISV 67946

// { added by lssanh 2013.04.19 ITS158176
int AppMediaPlayer_Video_ControllerDVD::getAudioLanguage()
{
    //MP_LOG << LOG_ENDL;

    // { added by cychoi 2013.06.24 for new DVD Settings UX
    if(m_nUserAudioLangCode != -1)
    {
        MP_LOG << LOG_ENDL;
        return m_nUserAudioLangCode;
    }
    // } added by cychoi 2013.06.24
    DeckEngineerMode  *pDeckEngine = m_pEngineVideo->getEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();
    EDeckInit1Setting  tSetting;
    pDeckEngine->GetInitSetting1(tSetting);
	
    MP_LOG << "Audio Language = " << tSetting.m_nPrioAudioCode << LOG_ENDL;
    m_nUserAudioLangCode = tSetting.m_nPrioAudioCode; // added by cychoi 2013.06.24 for new DVD Settings UX
	return tSetting.m_nPrioAudioCode;
}

int AppMediaPlayer_Video_ControllerDVD::getSubtitleLanguage()
{
    //MP_LOG << LOG_ENDL;

    // { added by cychoi 2013.06.24 for new DVD Settings UX
    if(m_nUserSubTitleLangCode != -1)
    {
        MP_LOG << LOG_ENDL;
        return m_nUserSubTitleLangCode;
    }
    // } added by cychoi 2013.06.24
    DeckEngineerMode  *pDeckEngine = m_pEngineVideo->getEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();
    EDeckInit1Setting  tSetting;
    pDeckEngine->GetInitSetting1(tSetting);

    MP_LOG << "Subtitle Language = " << tSetting.m_nPrioSubTitleCode << LOG_ENDL;
    m_nUserSubTitleLangCode = tSetting.m_nPrioSubTitleCode; // added by cychoi 2013.06.24 for new DVD Settings UX
	return tSetting.m_nPrioSubTitleCode;	
}

int AppMediaPlayer_Video_ControllerDVD::getMenuLanguage()
{
    //MP_LOG << LOG_ENDL;

    // { added by cychoi 2013.06.24 for new DVD Settings UX
    if(m_nUserMenuLangCode != -1)
    {
        MP_LOG << LOG_ENDL;
        return m_nUserMenuLangCode;
    }
    // } added by cychoi 2013.06.24
    DeckEngineerMode  *pDeckEngine = m_pEngineVideo->getEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();
    EDeckInit1Setting  tSetting;
    pDeckEngine->GetInitSetting1(tSetting);

    MP_LOG << "Menu Language = " << tSetting.m_nPrioMenuLangCode << LOG_ENDL;
    m_nUserMenuLangCode = tSetting.m_nPrioMenuLangCode; // added by cychoi 2013.06.24 for new DVD Settings UX
	return tSetting.m_nPrioMenuLangCode;	
}

int AppMediaPlayer_Video_ControllerDVD::getAngle()
{
    //MP_LOG << LOG_ENDL;
    DeckEngineerMode  *pDeckEngine = m_pEngineVideo->getEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();
    EDeckInit1Setting  tSetting;
    pDeckEngine->GetInitSetting1(tSetting);

    MP_LOG << "Angle = " << tSetting.m_nAngleDisplay << LOG_ENDL;
	return tSetting.m_nAngleDisplay;	
}	
// } added by lssanh 2013.04.19 ITS158176

void AppMediaPlayer_Video_ControllerDVD::cleanDVDSettingsInfo()
{
    //MP_LOG << LOG_ENDL;
// { modified by ravikanth - CR 13268
//    m_pModel->setAudioLangData(NULL, NULL, NULL, 0);
//    m_pModel->setCaptionData(NULL, 0);
//    m_pModel->setAngleData(0,-1);
// } modified by ravikanth
}

// { commented by cychoi 2014.06.27 for removed too many logs... // { added by kihyung 2012.07.13 for CR 9867
//void AppMediaPlayer_Video_ControllerDVD::setSelectByTitleMenu(bool bShow)
//{
//    MP_LOG << "m_bSelectByTitleMenu = " << bShow << LOG_ENDL;
//    m_bSelectByTitleMenu = bShow;
//}

//bool AppMediaPlayer_Video_ControllerDVD::getSelectByTitleMenu()
//{
//    MP_LOG << "m_bSelectByTitleMenu = " << m_bSelectByTitleMenu << LOG_ENDL;
//    return m_bSelectByTitleMenu;
//}
// } added by kihyung
// } commented by cychoi 2014.06.27

// { added by kihyung 2012.07.29 for CR 11057
void AppMediaPlayer_Video_ControllerDVD::requestScreenTitleChapter()
{
    onScreenStateChanged(m_pModel->playerScreenState());
}
// } added by kihyung
    
void AppMediaPlayer_Video_ControllerDVD::setAudioPath()
{
    MP_LOG << LOG_ENDL;

    // m_pControllers->SetAudioPath( eDeckVideoSPDIF ); // removed by kihyung 2012.11.03 for booting disable
    m_pEngineVideo->NofityAudioPathChanged( MODE_STATE_DISC_VIDEO); // added by lssanh 2012.08.24 for audio path
}


void AppMediaPlayer_Video_ControllerDVD::fastForward()
{
    MP_LOG << LOG_ENDL;

    if( m_pIMedia )
    {
        // { modified by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
        int nPlayerScreenState = m_pModel->playerScreenState();
        if(nPlayerScreenState == PLAYER_PLAY_DISPLAY ||
           nPlayerScreenState == PLAYER_DEFAULTSCREEN_DISPLAY)
        {
            // { modified by cychoi 20103.12.05 for ITS 212889 should not mute on forbidden command
            m_bDelayedMuteForSeek = true;
            //m_pEngineVideo->getEngineMain().setMuteForSeek(false); // added by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute
            // } modified by cychoi 20103.12.05
            m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false); // added by cychoi 2013.09.04 for OSD on FF
        }
        m_bPlayTimeOSDAllowed = false; // added by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
        // } modified by cychoi 20103.10.16
        m_pIMedia->FastForward(FAST_SPEED_X4);
    }
}


void AppMediaPlayer_Video_ControllerDVD::fastForwardCritical()
{
    MP_LOG << LOG_ENDL;

    if( m_pIMedia )
    {
        m_pIMedia->FastForward(FAST_SPEED_X16);
    }
}


void AppMediaPlayer_Video_ControllerDVD::rewind()
{
    MP_LOG << LOG_ENDL;

    if( m_pIMedia )
    {
        // { modified by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
        int nPlayerScreenState = m_pModel->playerScreenState();
        if(nPlayerScreenState == PLAYER_PLAY_DISPLAY ||
           nPlayerScreenState == PLAYER_DEFAULTSCREEN_DISPLAY)
        {
            // { modified by cychoi 20103.12.05 for ITS 212889 should not mute on forbidden command
            m_bDelayedMuteForSeek = true;
            //m_pEngineVideo->getEngineMain().setMuteForSeek(false); // added by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute
            // } modified by cychoi 20103.12.05
            m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false); // added by cychoi 2013.09.04 for OSD on REW
        }
        m_bPlayTimeOSDAllowed = false; // added by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
        // } modified by cychoi 20103.10.16
        m_pIMedia->Rewind(FAST_SPEED_X4);
    }
}


void AppMediaPlayer_Video_ControllerDVD::rewindCritical()
{
    MP_LOG << LOG_ENDL;

    if( m_pIMedia )
    {
        m_pIMedia->Rewind(FAST_SPEED_X16);
    }
}


void AppMediaPlayer_Video_ControllerDVD::flickNext()
{
    // if(m_pEngineVideo->getEngineMain().getisBTCall() == true) return; // commented by Sergey 17.05.2013 for ISV#83170

    // { added by kihyung 2013.07.22 for ITS 0181137
    if(m_pModel->loadingMode() == true) {
        MP_LOG << "m_pModel->loadingMode()... return..." << LOG_ENDL;
        return; 
    }
    // } added by kihyung 2013.07.22 for ITS 0181137


    // { modified by cychoi 2013.07.25 for ITS 181742 Track change on first flick
    if(m_pIMedia)
    {
        if(m_pModel->titleCount() == 0 || m_pModel->chapterCount() == 0)
        {
            // Try to get actual value of titele/chapter numbers from DVD manager
            DVDStatusResponse DVDInfo;
            memset(&DVDInfo,0,sizeof( DVDInfo));
            m_pIVideo->GetDVDStatus(&DVDInfo);
            m_pModel->setChapterCount(DVDInfo.totalChaptorOrTrack);
            m_pModel->setTitleCount(DVDInfo.totalGroupOrTitle);
        }

        if(m_pModel->titleCount() > 1 || m_pModel->chapterCount() > 1)
        {
            MP_LOG << LOG_ENDL;
            // { modified by cychoi 2013.09.10 for Sound mute on flick prev/next
            next();
            //m_pIMedia->Next(1);
            // } modified by cychoi 2013.09.10
        }
    }
    // } modified by cychoi 2013.07.25
}


void AppMediaPlayer_Video_ControllerDVD::flickPrev()
{
    // if(m_pEngineVideo->getEngineMain().getisBTCall() == true) return; // commented by Sergey 17.05.2013 for ISV#83170

    // { added by kihyung 2013.07.22 for ITS 0181137
    if(m_pModel->loadingMode() == true) {
        MP_LOG << "m_pModel->loadingMode()... return..." << LOG_ENDL;
        return; 
    }
    // } added by kihyung 2013.07.22 for ITS 0181137

    // { modified by cychoi 2013.07.25 for ITS 181742 Track change on first flick
    if(m_pIMedia)
    {
        if(m_pModel->titleCount() == 0 || m_pModel->chapterCount() == 0)
        {
            // Try to get actual value of titele/chapter numbers from DVD manager
            DVDStatusResponse DVDInfo;
            memset(&DVDInfo,0,sizeof( DVDInfo));
            m_pIVideo->GetDVDStatus(&DVDInfo);
            m_pModel->setChapterCount(DVDInfo.totalChaptorOrTrack);
            m_pModel->setTitleCount(DVDInfo.totalGroupOrTitle);
        }

        if(m_pModel->titleCount() > 1 || m_pModel->chapterCount() > 1)
        {
            MP_LOG << LOG_ENDL;
            // { modified by cychoi 2013.09.10 for Sound mute on flick prev/next
            prev(1);
            //m_pIMedia->Prev(1,1); // modified by Sergey 28.05.2013
            // } modified by cychoi 2013.09.10
        }
    }
    // } modified by cychoi 2013.07.25
}


bool AppMediaPlayer_Video_ControllerDVD::tuneWheel(bool bTuneForward)
{
    // { added by kihyung 2013.07.22 for ITS 0181137
    if(m_pModel->loadingMode() == true) {
        MP_LOG << "m_pModel->loadingMode()... return..." << LOG_ENDL;
        return false; 
    }
    // } added by kihyung 2013.07.22 for ITS 0181137
        
    // { added by kihyung 2012.07.17
    // if dvd title menu, tune must not be work.
    if(m_pModel->playerScreenState() != PLAYER_PLAY_DISPLAY) {
        MP_LOG << "m_pModel->playerScreenState() != PLAYER_PLAY_DISPLAY" << LOG_ENDL;
        return false;
    }
    // } added by kihyung
    
    MP_MEDIUM << "tune forward =" << bTuneForward << LOG_ENDL;
    emit cancelFfRew(); // added by yongkyun 2013.01.19 for ITS 150703

    // removed by Sergey 23.09.2013 for ITS#188498

    if ( m_nPendingChapter == -1 || m_nPendingTitle == -1 )
    {
        getDVDInfo();
        m_nPendingChapter = m_pModel->chapter();
        m_saveChapter = m_nPendingChapter; // added   by yongkyun.lee  2012.10.17  for Current file is white color      
        m_nPendingTitle = m_pModel->title();
    }

    if ( bTuneForward )
    {
        if ( m_nPendingChapter == m_pModel->chapterCount() )
        {
            m_nPendingChapter = 1;
        }
        else
        {
            m_nPendingChapter++;
        }
    }
    else
    {
        if ( m_nPendingChapter == 1 )
        {
            m_nPendingChapter = m_pModel->chapterCount();
        }
        else
        {
            m_nPendingChapter--;
        }
    }
	// { added by eugene.seo 2013.03.07
	if(m_nPendingChapter > 0 && m_nPendingTitle > 0)  
	{
        // removed by cychoi 2013.07.25 for unused code
        if( (m_nPendingChapter == m_saveChapter)  && (m_pModel->chapterCount() !=1)  ) ////added by shkim for ITS 180960
        {
            //m_pEngineVideo->releaseTuneMode();     // added by yongkyun.lee 20130703 for : ITS 177707
            m_pEngineVideo->showCurrentOSD();
            m_pEngineVideo->SetTuneSameFile(true);//added by edo.lee 2013.03.22
            //{ added by yongkyun.lee 20130701 for : DISC OSD 
            return true;
            //return false; 
            //} added by yongkyun.lee 20130701 
        }
        m_pEngineVideo->SetTuneSameFile(false);
        // } modified by wspark
        //MP_LOG<<" searchTitle "<< m_nPendingChapter << LOG_ENDL;
        //added by shkim for ITS 180960
        if((m_nPendingChapter == m_saveChapter) && (m_pModel->chapterCount() == 1)){
            MP_LOG << "pendingChapter == saveChapter == chapterCount == 1" << LOG_ENDL;
            m_pEngineVideo->SetTuneSameFile(true);
            m_pEngineVideo->showCurrentOSD();
        }
        else{
            MP_LOG<<" searchTitle "<< m_nPendingChapter << LOG_ENDL;
            m_searchTitle = QString("%1").arg(m_nPendingChapter);
            //{ modified by yongkyun.lee 2013-07-26 for : somke test DVD OSD            
            m_pEngineVideo->setOSDMetaData(DISC_DVD_VIDEO,
                getOSDTitle(QString("%1").arg(m_nPendingTitle), m_searchTitle), RANDOMOFF,REPEATOFF, 0, true); // modified by cychoi 2014.02.21 for ITS 226393 // modified by cychoi 2013.11.12 for OSD Title Translation
            //} modified by yongkyun.lee 2013-07-26 

            m_pModel->setTitle(m_nPendingTitle);
            m_pModel->setChapter(m_nPendingChapter);
        }
        //added by shkim for ITS 180960
	}
	// } added by eugene.seo 2013.03.07

    return true; // added by kihyung 2012.07.17
}

bool AppMediaPlayer_Video_ControllerDVD::selectTuned() // modified by junggil 2012.07.05 for CR11261 : Not able to search video files using CCP/RRC jogdial
{
    // { added by kihyung 
    // if dvd title menu, tune must not be work.
    if(m_pModel->playerScreenState() != PLAYER_PLAY_DISPLAY) {
        MP_LOG << "m_pModel->playerScreenState() != PLAYER_PLAY_DISPLAY" << LOG_ENDL;
        return false;
    }
    // } added by kihyung

    //MP_LOG << "pending title =" << m_nPendingTitle << LOG_ENDL;
    //MP_LOG << "pending chapter =" << m_nPendingChapter << LOG_ENDL;

    // modified  by yongkyun.lee@lge.com  2012.08.21  for : New UX :DQA #92 
    //if(m_nPendingTitle == 0 || m_nPendingChapter == 0)
    if(m_nPendingTitle == -1 || m_nPendingChapter == -1)
    // modified  by yongkyun.lee@lge.com
    {
        MP_LOG << "Ttle/chapter is 0" << LOG_ENDL;
        return false; // added by junggil 2012.07.05 for CR11261 : Not able to search video files using CCP/RRC jogdial
    }

    MP_LOG << "pending title =" << m_nPendingTitle << " pending chapter =" << m_nPendingChapter << LOG_ENDL;

    // { modified by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
    //{ modified by yongkyun.lee 2013-07-26 for : somke test DVD OSD
    if(m_nPendingChapter == m_saveChapter)
    {
        m_pEngineVideo->setOSDMetaData(DISC_DVD_VIDEO,
                getOSDTitle(QString("%1").arg(m_nPendingTitle), m_searchTitle), RANDOMOFF,REPEATOFF, 0, false); // modified by cychoi 2014.02.21 for ITS 226393 // modified by cychoi 2013.11.12 for OSD Title Translation
    }
    //} modified by yongkyun.lee 2013-07-26 
    // } modified by cychoi 2013.10.20
    m_searchTitle = "";
    // { added by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
    // { modified by cychoi 20103.12.05 for ITS 212889 should not mute on forbidden command
    m_bDelayedMuteForSeek = true;
    //m_pEngineVideo->getEngineMain().setMuteForSeek(false);
    // } modified by cychoi 20103.12.05
    m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(true);
    m_bPlayTimeOSDAllowed = true; // added by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
    // } added by cychoi 2013.10.20
    m_pIVideo->SearchTitleChapter(m_nPendingTitle, m_nPendingChapter);

    m_nPendingTitle = -1;
    m_nPendingChapter = -1;
    m_saveChapter = -1; // added by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
    return true; // added by junggil 2012.07.05 for CR11261 : Not able to search video files using CCP/RRC jogdial
}


void AppMediaPlayer_Video_ControllerDVD::cancelTune()
{
    // { added by kihyung 
    // if dvd title menu, tune must not be work.
    if(m_pModel->playerScreenState() != PLAYER_PLAY_DISPLAY) {
        MP_LOG << "m_pModel->playerScreenState() != PLAYER_PLAY_DISPLAY" << LOG_ENDL;
        return;
    }
    // } added by kihyung
    //{ added by yongkyun.lee 20130403 for : NO CR
    if(  m_nPendingChapter < 0 )
        return;
    //} added by yongkyun.lee 20130403 

    // Clear tempory variable
    m_nPendingTitle = -1;
    m_nPendingChapter = -1;
    m_saveChapter = -1; // added by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel

    //Try to restore actual value of titele/chapter numbers in the model
    if ( m_pIVideo )
    {
        DVDStatusResponse DVDInfo;
        memset(&DVDInfo,0,sizeof( DVDInfo));
        m_pIVideo->GetDVDStatus(&DVDInfo);

        m_pModel->setChapter( DVDInfo.trackOrChapterNo );
        m_pModel->setTitle( DVDInfo.groupOrTitleNum );
    }
}

// { modified by ravikanth 27-11-12
// { modified by ravikanth CR 15994 12-11-22
void AppMediaPlayer_Video_ControllerDVD::SendVideoControlData(
        int captionLangCode, int audioLangCode, int menuLangCode)
{
    MP_LOG << LOG_ENDL;
    VCInfo param;

    param.activeCaptionLangCode = captionLangCode;
    param.activeAudioLangCode = audioLangCode;
    param.activeMenuLangCode = menuLangCode;

    m_pModel->setActiveLangData(captionLangCode, audioLangCode, menuLangCode); // modified by ravikanth 13-03-13

    // removed by cychoi 2013.06.19 for ITS 174575 unused code
}
// } modified by ravikanth 27-11-12

void AppMediaPlayer_Video_ControllerDVD::switchCaption()
{
    //removed by yungi 2013.10.24 for ITS 189997 // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
    m_pIVideo->SwitchSubTitle();
}

void AppMediaPlayer_Video_ControllerDVD::switchVoice()
{
    //{ added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
    DVDStatusResponse DVDInfo;
    memset(&DVDInfo,0,sizeof( DVDInfo));
    m_pIVideo->GetDVDStatus(&DVDInfo);
    //} added by cychoi 2013.08.27

    // { modified by yungi 2013.10.24 ITS 189997
    if(DVDInfo.totalAudioSetting > 1)
    {
        m_pEngineVideo->getEngineMain().setMuteForSeek(false); // added by cychoi 20103.07.22 for ISV 87885
    }
    m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(true);
    m_bPlayTimeOSDAllowed = true; // added by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
    m_pIVideo->SwitchAudio();
    // } modified by yungi
}

void AppMediaPlayer_Video_ControllerDVD::switchAngle()
{
    ////removed by yungi 2013.10.24 for ITS 189997 // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
    m_pIVideo->SwitchAngle();
}
// } modified by ravikanth CR 15994 12-11-22

// { added by cychoi 2013.09.08 for ITS 181382 EVT_INITIALIZATION_REQUEST
void AppMediaPlayer_Video_ControllerDVD::InitDeckLang()
{
    m_nUserSubTitleLangCode = -1;
    m_nUserAudioLangCode = -1;
    m_nUserMenuLangCode = -1;
}
// } added by cychoi 2013.09.08

// { modified by ravikanth 27-11-12
void AppMediaPlayer_Video_ControllerDVD::OnDeckLangChanged()
{
    // { added by cychoi 2013.08.23 for ITS 181382 EVT_INITIALIZATION_REQUEST
    m_nUserSubTitleLangCode = -1;
    m_nUserAudioLangCode = -1;
    m_nUserMenuLangCode = -1;
    // } added by cychoi 2013.08.23
    requestDefaultLanguagesSet();
}
// } modified by ravikanth 27-11-12

// { added by cychoi 2013.09.24 for ITS 191333
bool AppMediaPlayer_Video_ControllerDVD::isScreenDVDMenuTitleState()
{
    int nState = PLAYER_PLAY_DISPLAY;

    //setSelectByTitleMenu(false); // modified by cychoi 2014.06.27 for removed too many logs...
    nState = m_pModel->playerScreenState();

    return (nState == PLAYER_TOPMENU_DISPLAY ||
            nState == PLAYER_ROOTMENU_DISPLAY ||
            nState == PLAYER_TITLEMENU_DISPLAY);
}
// } added by cychoi 2013.09.24

// { added by cychoi 2013.10.27 for ITS 197433 & 197384
void AppMediaPlayer_Video_ControllerDVD::retranslateUI()
{
    // { modified by cychoi 2013.12.05 for ITS 212884
    if(m_pEngineVideo->GetVideoController()->getSource() != MEDIA_SOURCE_DVDVIDEO ||
       m_pEngineVideo->getEngineMain().GetLastPlayerMode() != APP_MEDIA_MODE_VIDEO)
    {
        MP_LOG<<"mode is not matched" << LOG_ENDL;
    }
    else
    {
        int nState = m_pModel->playerScreenState();
        if(nState == PLAYER_PLAY_DISPLAY ||
           m_pEngineVideo->isDVDTrackPlaying())
        {
            m_pEngineVideo->updateOSDMetaData(DISC_DVD_VIDEO,
                getOSDTitle(QString("%1").arg(m_pModel->title()), QString("%1").arg(m_pModel->chapter()))); // modified by cychoi 2014.02.21 for ITS 226393 // modified by cychoi 2013.11.12 for OSD Title Translation
        }
    }

    m_pModel->retranslateUI();
    // } modified by cychoi 2013.12.05
}
// } added by cychoi 2013.10.27

