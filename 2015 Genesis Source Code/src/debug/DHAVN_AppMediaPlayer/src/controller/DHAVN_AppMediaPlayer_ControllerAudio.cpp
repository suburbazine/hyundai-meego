#include "controller/DHAVN_AppMediaPlayer_ControllerAudio.h"
#include "DHAVN_AppMediaPlayer_Shared.h"
#include <DHAVN_AppFramework_Event_Def.h>
#include <uistate_manager_def.h>
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "util/audio/DHAVN_AppMediaPlayer_Utils.h"
#include "engine/DHAVN_AppMediaPlayer_EngineMain.h"
#include "engine/DHAVN_AppMediaPlayer_EngineAudio.h" // added by kihyung 2012.07.09 for CR 10343
#include <DHAVN_AudioSourceType.h>
#include <DHAVN_GSTAudioRouterInterface.h>
#include "util/osd/OSD_Manager.h"
#include <QProcess> //added by aettie 20130605 for Gracenote restart
#include <dirent.h>   // added by sungha.choi 2013.08.12 for ITS 182674_3rd
#include <gplayerengine.h> //added by junam 2013.09.13 for ITS_KOR_189335


#define ERROR_POPUP_DISABLE   0
#define ERROR_POPUP_READY     1
#define ERROR_POPUP_ENABLE    2

//{added by junam 2013.03.10 for ISV73458
#define TUNE_TIME_METAINFO_REQUEST   10
#define TUNE_TIME_METAINFO_RESPONSE  2000
#define TUNE_TIME_COVERART_REQUEST   2000
//}added by junam

#define AUDIO_NORMAL_ERROR_INTERVAL  2000
#define AUDIO_SINK_ERROR_INTERVAL    1000

// { added by wspark 2012.09.17 for CR13605
QString ID3_Tag_Genre[ID3_TAG_GENRE_MAX] =
{
    "Blues",    "Classic Rock",     "Country",      "Dance",
    "Disco",    "Funk",             "Grunge",       "Hip-Hop",
    "Jazz",     "Metal",            "New Age",      "Oldies",
    "Other",    "Pop",              "R&B",          "Rap",
    "Reggae",   "Rock",             "Techno",       "Industrial",
    "Alternative",  "Ska",          "Death Metal",  "Pranks",
    "Soundtrack",   "Euro-Techno",  "Ambient",      "Trip-Hop",
    "Vocal",        "Jazz+Funk",    "Fusion",       "Trance",
    "Classical",    "Instrumental", "Acid",         "House",
    "Game",         "Sound Clip",   "Gospel",       "Noise",
    "AlternRock",   "Bass",         "Soul",         "Punk",
    "Space",        "Meditative",   "Instrumental Pop",     "Instrumental Rock",
    "Ethnic",       "Gothic",       "Darkwave",     "Techno-Industrial",
    "Electronic",   "Pop-Folk",     "Eurodance",    "Dream",
    "Southern Rock",    "Comedy",   "Cult",         "Gangsta",
    "Top 40",       "Christian Rap",    "Pop/Funk", "Jungle",
    "Native American",  "Cabaret",  "New Wave",     "Psychadelic",
    "Rave",     "Showtunes",        "Trailer",      "Lo-Fi",
    "Tribal",       "Acid Punk",    "Acid Jazz",    "Polka",
    "Retro",        "Musical",      "Rock & Roll",  "Hard Rock",
    "Folk",         "Folk-Rock",    "National Folk",    "Swing",
    "Fast Fusion",  "Bebob",        "Latin",        "Revival",
    "Celtic",       "Bluegrass",    "Avantgarde",   "Gothic Rock",
    "Progressive Rock", "Psychedelic Rock", "Symphonic Rock",   "Slow Rock",
    "Big Band",     "Chorus",       "Easy Listening",   "Acoustic",
    "Humour",       "Speech",       "Chanson",      "Opera",
    "Chamber Music",    "Sonata",   "Symphony",     "Booty Bass",
    "Primus",       "Porn Groove",  "Satire",       "Slow Jam",
    "Club",         "Tango",        "Samba",        "Folklore",
    "Ballad",       "Power Ballad", "Rhythmic Soul",    "Freestyle",
    "Duet",         "Punk Rock"     "Drum Solo",    "A capella",
    "Euro-House",   "Dance Hall"

};
// } added by wspark

// { modified by kihyung 2013.2.12 for MEDIA HK Crash
/*Internal only*/
enum
{
    REQUID_DEFAULT                  = 0,
    REQUID_DEVICE_ID                = 100,
    REQUID_ROTATE_TUNE              = 200,
    REQUID_REQUST_COVER_FLOW_VIEW   = 300,
    REQUID_REQUST_COVER_LIST_VIEW   = 400,
    REQUID_ALBUM_CHANGE             = 500,
    REQUID_NEXT_ALBUM               = 600,
    REQUID_COVERFLOW_CLICK          = 700,  //added by junam 2013.11.16 for coverflow click
    REQUID_ALL_FILELIST             = 800,
    REQUID_ALL_FILELIST_HKMC        = 850,
    REQUID_IPOD_ERR_ALL_SONGS       = 900,
    REQUID_IPOD_ERR_SONG_IDX        = 950,
    REQUID_ALL_FILELIST_RESCAN      = 1000,    // added by kihyung 2013.07.09 for ITS 0177427
    REQUID_ALL_FILELIST_RESCAN_COPY = 1100,		// added by sangmin.seol 2013.10.22 JUKEBOX PlayerView Counter Update after copy from USB    
    REQUID_NEXT_TRACK				= 100000, //added by edo.lee 2013.05.09
    REQUID_PREV_TRACK				= 200000, //added by edo.lee 2013.05.09
    REQUID_MAXIMUM
};
// } modified by kihyung 2013.2.12 for MEDIA HK Crash

#define GET_TA_REQUID(X) (X + m_device) // added by kihyung 2013.2.12 for MEDIA HK Crash
#define GET_TA_REQUID2(X,Y) (X + Y)

extern SharedDataProvider *m_pSharedAudioData; // added by kihyung 2013.2.20

// { modified by eugeny.novikov 2012.12.10 for CR 16564 fastboot
AppMediaPlayer_Controller_Audio::
AppMediaPlayer_Controller_Audio( AppMediaPlayer_EngineMain &engineMain )
  : AppMediaPlayer_Controller_Base( engineMain ),
    m_bSameDName(false), // added by jungae 2012.10.09, for CR14252, iPod reset when device name is same with BT
    m_pIMedia( NULL ),
    m_pIMediaEx(NULL),  // added by kihyung 2013.08.08 for ITS 0181967 
    m_pJukeboxController(NULL),
    m_pUsbController(NULL),
    m_pDeckController(NULL),
    m_pIpod1Controller(NULL),
    m_pIpod2Controller(NULL),
    m_pIAudio(NULL),
    m_pIVideo(NULL),
    m_pVideoWidget(NULL), // added by kihyung 2012.08.30 for DVD-Audio
    m_isRunFromFileManager(false), //Added by Alexey Edelev. Fix bug 13058. 2012.09.13    
    m_iPod1ContentSizeReady(false), //Added by Alexey Edelev. 2012.09.08 CR13600, CR13610
    m_iPod2ContentSizeReady(false),  //Added by Alexey Edelev. 2012.09.08 CR13600, CR13610
    m_isForwardDirection(true), // added by wspark 2012.09.10 for CR12707   
    //{ added by yongkyun.lee 2012.12.19 for CR 14219 ,POPUP/list close
    //m_bCamInFG(false),  
    m_isAccOn( true ),  
    m_isFrontLCD(true),  
    //} added by yongkyun.lee 
    m_btSending(false), //added by edo.lee 2012.12.28 ITS 150290
    m_iTrackInfoReqCount(0),  //added by hyochang.ryu 20130611
    m_tuneMetaInfoTimer(this),
    //m_tuneCoverArtTimer(this), //removed by junam 2013.10.17 for deprecated code
    m_pGPlayerEngine(new GPlayerEngine()), //added by junam 2013.09.13 for ITS_KOR_189335
    m_skipTrack(false),// modified by Dmitry 13.10.13
    m_bFirstDiscOSD(false), // added by oseong.kwon 2014.02.04 for OSD update on TrackChange
    m_bFF(false) // added by Dmitry 13.10.13
{
    MP_LOG << LOG_ENDL;

    // <--
    m_pTrackerAbstractor = new CTrackerAbstractor();
    // added by jaehwan.lee 2013.10.03
    m_pTrackerAbstractor->SetLanguage(engineMain.GetLanguage());
    m_pTrackerExtractor = new CTrackerExtractorClient();
    m_pTreeModel = new CATreeModel( this );
    m_pPathViewModel = new PathViewModel(m_imageMap); //changed by junam 2013.10.28 for 2nd depth coverart
    //m_pSearchEngine = new SearchEngine(this);  //ys-20131010 delete seach engine
    // --> 20 ms

    m_BTSetMediaMode = false;	//added by LYK 20130828
    m_device = MP::UNDEFINED;
    m_deviceValid = MP::UNDEFINED; //{ modified by yongkyun.lee 2013-08-30 for : BT sound mute
    m_isSourceChanged = true; // added by kihyung 2013.2.12 for MEDIA HK Crash
    m_PlayerState = PLAYER_STOP;
    m_repeatPlayMode = REPEATALL;
    m_randomPlayMode = RANDOMOFF;
    m_scanMode = SCANOFF;
    m_isListView = false; // modified by eugene.seo 2012.10.23 for Function_USB_0190
    m_CDDAGraceNote = GRACENOTE_INDEXING_READY;// added by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
    m_isPlayFromMLT = false;
    m_isBTStreaming = false;// modified by yongkyun.lee 2013-10-14 for : ITS 195248
    m_albumName = "";
    m_albumThumbnail = "";
    m_searchData.bIsSearch = false;
    m_searchData.matchField = -1;
    m_searchData.trackUrl = "";
    m_isLongPressed = 0;
    // m_isPrev = false; // removed by kihyung 2013.08.28
    m_NoOSD = false; // modified by yongkyun.lee@lge.com 2012.07.14  CR 11922
    m_nPendingMedia = -1;
    m_nSavedMedia = -1; // added by suilyou 20131030 ITS 0204905
    m_tuneState.m_nTuneFF = false;
    m_tuneState.m_nTuneBack = false;
    m_tuneState.m_nTuneStart = false;
    m_tuneState.m_nTuneRequest = false;
    // m_lastSong = ""; // removed by kihyung 2013.08.28
    // m_rndFlag = NONE; // removed by kihyung 2013.08.28
    m_bIsApple1Ready = false;
    m_bIsApple2Ready = false;
    //{added by junam 2013.05.02
    m_bIsDatabase1Ready = false;
    m_bIsDatabase2Ready = false;
    //}added by junam
    m_bIsAppleConnectError = false; // added by kihyung 2012.07.09 for CR 10343
    m_minerState.minerInProgress = 0;
    //m_minerState.isMinerCleanRequired = true; //removed by junam 2013.11.06 for coverflow update
    m_minerState.minerInProgressPercentageStep = 1;//30; modified by edo.lee 2013.05.14
	m_minerState.isMinerFinished = false; // modified by eugene.seo 2013.05.14
	m_minerState.isUSB1MinerFinished= false; // added by eugene.seo 2013.04.15
	m_minerState.isUSB2MinerFinished= false; // added by eugene.seo 2013.04.15
    m_bIsBTPlaying = false;
    m_bPlayerActive = false;
    m_isResourcesOwned = false;
    m_pendingSource = MP::UNDEFINED;
    m_discType = MEDIA_SOURCE_MAX;
    m_bRestoring = false;
    m_enableErrorPopupState =  ERROR_POPUP_DISABLE;
    m_bRequestCoverOnBasicView = false; // added by kihyung 2012.07.09 for CR 11165

    m_InOutTestCnt = 0; //added by wonseok.heo 2013.07.04 disc in out test
    m_InOutStepCnt = 0; //added by wonseok.heo 2013.07.04 disc in out test

    m_bJukeboxDelayedPlay = true;
    m_bUSB1DelayedPlay = true;
    m_bUSB2DelayedPlay = true;
    m_iPodChangingPosition = 0;//added by edo.lee 2013.04.25
    m_bBTAvrcpAbove13 = false;// added by yongkyun.lee 20130427 for : BT sync
    m_isChangeToSame = false;//added by edo.lee 2013.05.18
    m_isForeground = false;//added by edo.lee 2013.06.02
    m_bNewUSB1Connected = false; //added by edo.lee 2013.06.10
    m_bNewUSB2Connected = false; //added by edo.lee 2013.06.10
	m_tuneTitle = "";
	m_isCreatNewList = false;
	m_isChangedDevice = false;
    m_iPodSortingOrder = -1;//added by junam 2013.08.21 for iPod sorting
    // removed by cychoi 2013.12.29 for ITS 217565
    m_isShowPopup = false; //added by edo.lee 2013.09.02 ITS0184880

    m_bUpdateFileFolderIndex = false;

    m_bDiscPostponeRestore = false; // added by ruindmby 2012.08.09 for CR 12782
    m_iPod1DeviceName = "Unknown"; // add by jungae 2012.09.03 for CR 13162
    m_iPod2DeviceName = "Unknown"; // added by jungae 2012.12.26, for send the connection information to AppBtPhone
    
    ResetAllHistory();
    m_DelayCoverArt = ""/*DEFALUT_COVER*/; //modified by Michael.Kim 2013.04.09 for New UX // added by lssanh 2013.01.24 ISV70714
    // m_currentRepeatmode = REPEATALL;  // added by eunhye 2013.03.30 for ISV 78066 // removed by kihyung 2013.05.27 for ITS 148293

    m_nIPOD1ErrIndex = -1;
    m_nIPOD2ErrIndex = -1;
	nAcount = 0; // added by eugene.seo 2013.04.15
    //m_skipTrackCount = 0;//added by edo.lee 2013.05.18    // deleted by sungha.choi 2013.07.01

    m_pListViewModel = new CListModel(*this, m_imageMap); //changed by junam 2013.10.28 for 2nd depth coverart
    if (m_pListViewModel)
    {
        connect(m_pListViewModel, SIGNAL(contentSize(MEDIA_SOURCE_T,MEDIA_TYPE_T,int)),
                            this, SIGNAL(contentSize(MEDIA_SOURCE_T,MEDIA_TYPE_T,int)));                
    }

    // { added by junam 2012.10.25 for CR14451
    if (m_pPathViewModel)
    {
        connect(m_pPathViewModel, SIGNAL(requestCover()), this, SLOT(onRequestCoverArtForFlowView()));
        connect(m_pPathViewModel, SIGNAL(setHighlightCover(int,bool)), 
				this, SIGNAL(setHighlightCover(int,bool)), Qt::DirectConnection); //added by junam 2013.03.18 for coverflow refresh
    }
    // } added by junam

    if (m_pTrackerExtractor)
    {
        connect(m_pTrackerExtractor, SIGNAL(minerFinished(bool, QString)), this, SLOT(onMinerFinished(bool, QString)));
        connect(m_pTrackerExtractor, SIGNAL(minerInProgress(int)), this, SLOT(onMinerInProgress(int)));
    }
    
    if (m_pTrackerAbstractor != NULL)
    {
        connect(m_pTrackerAbstractor, SIGNAL(RequesteComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, uint)),
                                this,   SLOT(HandleRequestComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, uint)));

        connect(m_pTrackerAbstractor, SIGNAL(RequesteComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, ETrackerAbstarctor_VolumeType, uint)),
                                this,   SLOT(HandleRequestCompleteWithVolumeType(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, ETrackerAbstarctor_VolumeType, uint)));

        m_pTrackerAbstractor->setDeviceId(eJukeBox, NULL, "/usr_data/media");
        m_pTrackerAbstractor->SetCountryVariant(AppEngineBase::GetCountryVariant()); // added by Dmitry 28.03.15
    }

// modified by edo.lee 2013.10.12 pop noise
    m_pLongBtnTimer = new QTimer(this);
    m_pLongBtnTimer->setSingleShot( true );
    m_pLongBtnTimer->setInterval(300);
    connect( m_pLongBtnTimer, SIGNAL(timeout()), this, SLOT(onLongBtnTimerExpired()) );

    //{ added by jaehwan.lee 2014.02.27 to fix ITS 227260
    m_pScanTimer = new QTimer(this);
    m_pScanTimer->setSingleShot( true );
    m_pScanTimer->setInterval(700);
    connect( m_pScanTimer, SIGNAL(timeout()), this, SLOT(onScanTimerExpired()) );
    //} added by jaehwan.lee

    m_tuneMetaInfoTimer.setSingleShot( true );
    connect( &m_tuneMetaInfoTimer, SIGNAL(timeout()), this, SLOT(onTuneMetaInfoTimeOver()) );

    m_isDVDinfoRestored = false;
    m_tempDVDStatus.groupOrTitleNum = 0;
    m_tempDVDStatus.trackOrChapterNo = 0;
    m_tempDVDStatus.playbackTime = 0;

    m_bIsBasicView = true; // added by eugeny.novikov 2012.10.25 for CR 14047
    //m_bIsLoadingScreen = false; // removed by junam 2013.12.10 for code clean
    m_EventId = 0; // added by eugene 2012.11.26 for ISV #59906
    m_NewState = 0; // added by eugene 2012.11.26 for ISV #59906
    // { added by wspark 2013.01.23
    m_lastPlayedIndex = -1;
    m_lastPlayedPos = 0;
    // } added by wspark
    // { added by wspark 2013.02.07 for ISV 72168
    m_pBtResponseTimer = new QTimer(this);
    m_pBtResponseTimer->setSingleShot( true );
    m_pBtResponseTimer->setInterval(2000);
    connect(m_pBtResponseTimer, SIGNAL(timeout()), this, SLOT(onBtResponseTimerExpired()));
    // { added by cychoi 2014.06.03 for Get Info - 1. on TrackChanged 2. delayed 0.5sec from PlayStarted
    m_pBtDelayedGetInfoTimer = new QTimer(this);
    m_pBtDelayedGetInfoTimer->setSingleShot( true );
    m_pBtDelayedGetInfoTimer->setInterval(500);
    connect(m_pBtDelayedGetInfoTimer, SIGNAL(timeout()), this, SLOT(onBtDelayedGetInfoTimerExpired()));
    // } added by cychoi 2014.06.03
    // { added by cychoi 2014.06.05 for Get Info - every 10sec after PlayStarted (some phones don't send TrackChanged)
    m_pBtPeriodicGetInfoTimer = new QTimer(this);
    m_pBtPeriodicGetInfoTimer->setSingleShot( true );
    m_pBtPeriodicGetInfoTimer->setInterval(10000);
    connect(m_pBtPeriodicGetInfoTimer, SIGNAL(timeout()), this, SLOT(onBtPeriodicGetInfoTimerExpired()));
    // } added by cychoi 2014.06.05
    //{ added by yongkyun.lee 20130426 for : BT sync 
    m_pBtRequestTimer = new QTimer(this);
    m_pBtRequestTimer->setSingleShot( true ); // modified by yongkyun.lee 2013-08-28 for : BT OSD sync 
    m_pBtRequestTimer->setInterval(2000);  // modified by yongkyun.lee 2013-08-28 for : BT OSD sync 
    connect(m_pBtRequestTimer, SIGNAL(timeout()), this, SLOT(onBtRequestTimerExpired()));
    //} added by yongkyun.lee 20130426
    // { modified by cychoi 2014.05.12 for duplicated code (added by hyochang.ryu 20130627 for BT delayed play, 20131031 for ITS 205112)
    m_pBtDelayedPlayTimer = new QTimer(this);
    m_pBtDelayedPlayTimer->setSingleShot( true );
    m_pBtDelayedPlayTimer->setInterval(2000);
    connect(m_pBtDelayedPlayTimer, SIGNAL(timeout()), this, SLOT(onBtDelayedPlayTimerExpired()));
    //m_pBtDelayedPlayTimer1 = new QTimer(this);
    //m_pBtDelayedPlayTimer1->setSingleShot( true );
    //m_pBtDelayedPlayTimer1->setInterval(2000);//{ modified by yongkyun.lee 2014-03-11 for : BT Stream Mute
    //connect(m_pBtDelayedPlayTimer1, SIGNAL(timeout()), this, SLOT(onBtDelayedPlayTimer1Expired()));
    // } modified by cychoi 2014.05.12
    //{ added by hyochang.ryu 20130913 for ITS189483
    m_pBtDelayedMuteTimer = new QTimer(this);
    m_pBtDelayedMuteTimer->setSingleShot( true );
    m_pBtDelayedMuteTimer->setInterval(2000);
    connect(m_pBtDelayedMuteTimer, SIGNAL(timeout()), this, SLOT(onBtDelayedMuteTimerExpired()));
    //} added by hyochang.ryu 20130913 for ITS189483
    // } added by wspark

    //{ modified by yongkyun.lee 2013-12-13 for : ITS 213657
    m_iPodMountTimer = new QTimer(this);
    m_iPodMountTimer->setSingleShot( true );
    m_iPodMountTimer->setInterval(3000);
    connect(m_iPodMountTimer, SIGNAL(timeout()), this, SLOT(oniPodMountTimerExpired()));
    //} modified by yongkyun.lee 2013-12-13 
    m_bBTDelayedPlayOnIPodMount = false; // added by cychoi 2014.04.03 for ITS 233324

    // { commented by cychoi 2014.05.12 for deprecated code
    //m_pMuteTimer = new QTimer(this);
    //m_pMuteTimer->setSingleShot( true );
    //m_pMuteTimer->setInterval(300);
    //connect(m_pMuteTimer, SIGNAL(timeout()), this, SLOT(onMuteTimerExpired()));
    // } commented by cychoi 2014.05.12
	
    tempHistory.playList.list = new QList<QString>; // added by eugene.seo 2013.04.10

    //{added by junam 2013.04.13 for AutoStart
    m_pIpod1Controller =  GetControllers().GetIPod1Controller();
    if(m_pIpod1Controller)
        ConnectIpod1Signal();
    m_pIpod2Controller =  GetControllers().GetIPod2Controller();
    if(m_pIpod2Controller)
        ConnectIpod2Signal();
    //} added by junam

    m_eDiscMuteState = DISC_UNMUTE; // added by cychoi 2013.06.02 for sound noise when dragged progress bar

    // {added by wonseok.heo 2013.07.04 disc in out test
    m_DiscLoadRequest = new QTimer(this); 
    m_DiscLoadRequest->setSingleShot( true);
    m_DiscLoadRequest->setInterval(5000);
    connect(m_DiscLoadRequest, SIGNAL(timeout()), this, SLOT(onDiscloadRequest())); 
    // }added by wonseok.heo 2013.07.04 disc in out test

    // { added by kihyung 2013.07.04 for ISV 85455
    m_nLastErrorCode  = -1; // added by kihyung 2013.12.17 for ITS 0214009 
    m_pAudioSinkTimer = new QTimer(this);
    m_pAudioSinkTimer->setInterval(AUDIO_NORMAL_ERROR_INTERVAL);
    connect(m_pAudioSinkTimer, SIGNAL(timeout()), this, SLOT(onAudioSinkError()));
    // } added by kihyung 2013.07.04 for ISV 85455

    m_bBasicControlEnableStatus =true;//added by junam 2013.07.12 for music app

    m_eID3TagState = ID3TAG_NONE; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error 
    m_nID3TagTrackIdx = -1; // added by cychoi 2014.01.07 for keep MP3 ID3 Tag if same song
    m_nID3TagRxCount = 0; // added by cychoi 2013.12.17 for ITS 216033

    m_discScanPlayingMode = 0; // added by wonseok.heo for ITS 191295 2013.09.26
    m_jukeboxUpdate = false; //added by junam 2013.10.03 for ITS_KOR_192927
    m_autoTrackChangeIndex = -1;

    MP_LOG<<"END" << LOG_ENDL;
}
// } modified by eugeny.novikov 2012.12.10

AppMediaPlayer_Controller_Audio::~AppMediaPlayer_Controller_Audio()
{
    for (int i = 0; i < MP::ALL_DEVICES; i++)
    {
        delete m_history[i].playList.list;
    }
    
    if ( m_pTrackerAbstractor != NULL )
    {
        disconnect(m_pTrackerAbstractor, 0, 0, 0);
    }
    delete m_pTrackerAbstractor;
    // delete rpmClientObj; // removed by kihyung 2012.11.03 for booting disable
    delete m_pPathViewModel;
    delete m_pTrackerExtractor;
    delete m_pTreeModel;
    //delete m_pSearchEngine;  //ys-20131010 delete seach engine
    delete m_pListViewModel;
    delete m_pAudioSinkTimer; // added by kihyung 2013.07.04 for ISV 85455
}

IMediaInterface*
AppMediaPlayer_Controller_Audio::GetMediaInterface()
{
    return m_pIMedia;
}

PathViewModel*
AppMediaPlayer_Controller_Audio::GetPathViewModel()
{
    return m_pPathViewModel;
}

CTrackerExtractorClient*
AppMediaPlayer_Controller_Audio::GetTrackerExtractor()
{
    return m_pTrackerExtractor;
}

CListModel*
AppMediaPlayer_Controller_Audio::GetListViewModel()
{
    return m_pListViewModel;
}

QString
AppMediaPlayer_Controller_Audio::DeviceName(int type)
{
    // Simple device names for database-keys and logging:
    return type == MP::JUKEBOX ? "Jukebox" :
           type == MP::USB1 ? "USB1" :
           type == MP::USB2 ? "USB2" :
           type == MP::DISC ? "Disc" :
           type == MP::IPOD1 ? "iPod1" :
           type == MP::IPOD2 ? "iPod2" :
           type == MP::BLUETOOTH ? "Bluetooth" :
           type == MP::AUX ? "AUX" :
           type == MP::UNDEFINED ? "Undefined" : "Wrong_device_index";
}

void
AppMediaPlayer_Controller_Audio::ipod1Authorised(bool status)
{    
    MP_HIGH << "IPodSignal " << status  << LOG_ENDL;

    //{changed by junam 2013.03.30
    //if(m_bIsApple1Ready)
    //{
    //    MP_LOG << "iPod is ready state already " << LOG_ENDL;
    //    return;
    //}    
    if(m_pIpod1Controller == NULL)
    {
        m_pIpod1Controller =  GetControllers().GetIPod1Controller();
    }
    //}changed by junam 2013.03.30

    m_bIsApple1Ready = status;

    if (m_bIsApple1Ready)
    {
        QString ipodId = m_pIpod1Controller->getiPodSerialNumber();
        //{added by junam 2013.03.30
        if(ipodId == NULL)
        {
            MP_LOG << "getiPodSerialNumber is NULL " << LOG_ENDL;
            return;
        }
        //}added by junam 2013.03.30
        m_history[MP::IPOD1].MountPath = "/" + ipodId;
        MP_LOG << "Updated mountPath =" << m_history[MP::IPOD1].MountPath << LOG_ENDL;

        setDeviceId(MP::IPOD1, ipodId, m_history[MP::IPOD1].MountPath);


        m_pSharedAudioData->SetIPodSerialNumber(DATA_TYPE_IPOD1_SERIAL_NUMBER, ipodId);  // added by kihyung 2013.2.20
        MP_LOG << "SDP" << m_pSharedAudioData->GetIPodSerialNumber(DATA_TYPE_IPOD1_SERIAL_NUMBER) << LOG_ENDL;
		// add by yongkyun.lee 2012.07.17 for CR 11963
		emit setStatusDataByIndex( MP::IPOD1, DATA_TYPE_IPOD1_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);		
    }
     // { modified by yongkyun.lee@lge.com  2012.08.28 for :CR13323 ,VR Create START/END
    // { modified by  yongkyun.lee 2012.11.21 for  xSampa check.
    //emit deviceMinerFinished(true, MP::IPOD1);    
    emit deviceMinerFinished(true,(int)VR_BIT_TE, MP::IPOD1);    
    // { modified by  yongkyun.lee 
    //m_TEtoVRsource = MP::IPOD1;
    //emit deviceMinerFinished(true);
    // } modified by yongkyun.lee@lge.com 

    // { added by kihyung 2012.08.04 for CR 11725
    emit mediaSyncfinished(m_bIsApple1Ready);
    // } added by kihyung
}

void
AppMediaPlayer_Controller_Audio::ipod2Authorised(bool status)
{
    MP_LOG << "IPodSignal " << status << LOG_ENDL;

    //{changed by junam 2013.03.30
    //if(m_bIsApple2Ready)
    //{
    //    MP_LOG << "iPod is ready state already " << LOG_ENDL;
    //    return;
    //}
    if(m_pIpod2Controller == NULL)
    {
        m_pIpod2Controller =  GetControllers().GetIPod2Controller();
    }
    //}changed by junam 2013.03.30

    m_bIsApple2Ready = status;

    if (m_bIsApple2Ready)
    {
        QString ipodId = m_pIpod2Controller->getiPodSerialNumber();
        //{added by junam 2013.03.30
        if(ipodId == NULL)
        {
            MP_LOG << "getiPodSerialNumber is NULL " << LOG_ENDL;
            return;
        }
        //}added by junam 2013.03.30
        m_history[MP::IPOD2].MountPath = "/" + ipodId;
        MP_LOG << "Updated mountPath =" << m_history[MP::IPOD2].MountPath << LOG_ENDL;

        setDeviceId(MP::IPOD2, ipodId, m_history[MP::IPOD2].MountPath);

        m_pSharedAudioData->SetIPodSerialNumber(DATA_TYPE_IPOD2_SERIAL_NUMBER, ipodId); // added by kihyung 2013.2.20
        MP_LOG << "SDP" << m_pSharedAudioData->GetIPodSerialNumber(DATA_TYPE_IPOD2_SERIAL_NUMBER) << LOG_ENDL;
        // add by yongkyun.lee 2012.07.17 for CR 11963
        emit setStatusDataByIndex( MP::IPOD2, DATA_TYPE_IPOD2_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
    }
     // { modified  by yongkyun.lee@lge.com  2012.08.24  for :CR13323
     
    // { modified by  yongkyun.lee 2012.11.21 for  xSampa check.
    //emit deviceMinerFinished(true,MP::IPOD2);  
    emit deviceMinerFinished(true,(int)VR_BIT_TE,MP::IPOD2);       
    // { modified by  yongkyun.lee  
    //m_TEtoVRsource = MP::IPOD2;
    //emit deviceMinerFinished(true);
    // } modified  by yongkyun.lee@lge.com 

    // { added by kihyung 2012.08.04 for CR 11725
    emit mediaSyncfinished(m_bIsApple2Ready);
    // } added by kihyung
}

//{added by junam 2013.05.02
void AppMediaPlayer_Controller_Audio::onIPodReady(bool status)
{
    MP::DEVICE_T source = GetSignalSource();
    
    MP_HIGH << "source  = " << source << status << LOG_ENDL;
    
    if(source == MP::IPOD1)
    {
        m_bIsDatabase1Ready = status; //moved by junam 2013.06.06 for iPod playlist disable
        ipod1Authorised(status);
        //m_bIsDatabase1Ready = status; //moved by junam 2013.06.06 for iPod playlist disable
    }
    else  if(source == MP::IPOD2)
    {
        m_bIsDatabase2Ready = status;  //moved by junam 2013.06.06 for iPod playlist disable
        ipod2Authorised(status);
        //m_bIsDatabase2Ready = status; //moved by junam 2013.06.06 for iPod playlist disable
    }

    //{added by junam 2013.06.12 for hold coverloading at indexing
    if( getDevice() == source && m_pListViewModel && isListView())
        m_pListViewModel->requestCoverTimerStart();
    //}added by junam
}

void AppMediaPlayer_Controller_Audio::onAutoPlayReady(bool status)
{
    MP::DEVICE_T source = GetSignalSource();
    MP_LOG << "source  = " << source << LOG_ENDL;
    if(source == MP::IPOD1)
    {
        ipod1Authorised(status);
    }
    else  if(source == MP::IPOD2)
    {
        ipod2Authorised(status);
    }
}
//} added by junam
void
AppMediaPlayer_Controller_Audio::setPlayerMode (const int &mode)
{
    // { modified by kihyung 2013.7.25 for ITS 0181470 
    MP_HIGH << "START" << DeviceName(getDevice()) << "->" << DeviceName(mode) << "m_isChangeToSame" << m_isChangeToSame<< LOG_ENDL;

	m_isChangeToSame = DeviceName(getDevice()) == DeviceName(mode) && !GetEngineMain().getIsFromOtherAV(); //added by edo.lee 2013.05.18
	
    //MP_LOG << "m_isChangeToSame " << m_isChangeToSame << LOG_ENDL;
    
	//added by edo.lee 2013.04.20 for OSD			
	DeviceHistory* infoHistory = GetDeviceHistory(mode);
	if(infoHistory && !m_isChangeToSame ) //modified by edo.lee 2013.05.27
	{
        // { modified by cychoi 2013.05.26 for Cluster IOT #125
        if((MP::DEVICE_T)mode == MP::DISC)
        {
            if(m_discType < MEDIA_SOURCE_MAX)
                setMediaInfoToHistory(m_discType,infoHistory->Title,infoHistory->Folder,infoHistory->Position);
        }
        else if(!GetEngineMain().getOnBoot())//modified by edo.lee 2013.05.27
        {
            //{ added by yungi 2013.08.16 for ITS 0184297
            if((MP::DEVICE_T)mode != MP::AUX)
            {
                setMediaInfoToHistory(AudioUtils::DeviceType((MP::DEVICE_T)mode),infoHistory->Title,infoHistory->Folder,infoHistory->Position);
            }
        }
		//modified by edo.lee 2013.05.09
		//setMediaInfoToHistory(AudioUtils::DeviceType((MP::DEVICE_T)mode),infoHistory->Title,infoHistory->Folder,infoHistory->Position);
        // } modified by cychoi 2013.05.26
        //GetEngineMain().GetNotifier()->setHistoryInfo(AudioUtils::DeviceType((MP::DEVICE_T)mode),infoHistory->Title,infoHistory->Folder,infoHistory->Position);
        //GetEngineMain().GetNotifier()->onSetDisplayOSD(true);	
		//GetEngineMain().GetNotifier()->displayOSD(	OSD_EVENT_AUDIO_TRACK_INFO );
	}
	//added by edo.lee 2013.04.20
	
    m_tSetSrcMutex.lock();
    SetIsSourceChanged(false);
    setSource((MP::DEVICE_T)mode);
    SetIsSourceChanged(true);
    m_tSetSrcMutex.unlock();   
    MP_MEDIUM << "END" << LOG_ENDL;
    // } modified by kihyung 2013.7.25 for ITS 0181470 

    emit isPlayerModeChanged(); // added by wspark 2013.03.23 for ISV 76947
}

// { added by kihyung 2013.2.12 for MEDIA HK Crash
void AppMediaPlayer_Controller_Audio::SetIsSourceChanged(const bool isChanged)
{
    MP_LOG << isChanged << LOG_ENDL;
    
    m_isSourceChanged = isChanged;
}
// } added by kihyung 2013.2.12 for MEDIA HK Crash

// { added by eugene 2012.11.26 for ISV #59906
void
AppMediaPlayer_Controller_Audio::setEventId (const int eventID)
{
    m_EventId = eventID;
}

void
AppMediaPlayer_Controller_Audio::setNewState (const int state)
{
    m_NewState = state;
}
// } added by eugene 2012.11.26 for ISV #59906


MP::DEVICE_T
AppMediaPlayer_Controller_Audio::GetMediaMode()
{
    return getDevice();
}

PLAYER_STATE
AppMediaPlayer_Controller_Audio::GetPlayerState()
{
    return m_PlayerState;
}

//{ modified by yongkyun.lee 2013-08-15 for : ITS 183677
bool
AppMediaPlayer_Controller_Audio::GetFfRewState()
{
    switch(  m_PlayerState)
    {
        case PLAYER_FASTFORWARD:
        case PLAYER_REWIND:
        case PLAYER_FASTFORWARD_X4:
        case PLAYER_FASTFORWARD_X16:
        case PLAYER_FASTFORWARD_X20:
        case PLAYER_REWIND_X4:
        case PLAYER_REWIND_X16:
        case PLAYER_REWIND_X20:
            return true;
        default :
            return false;
    }
    return false;
}
//} modified by yongkyun.lee 2013-08-09 


int
AppMediaPlayer_Controller_Audio::GetMediaNum()
{
    return (getDevice() < MP::ALL_DEVICES) ? m_history[getDevice()].Quantity : 0;
}

// { add by yongkyun.lee@lge.com  2012.10.05 for CR 13484 (New UX: music(LGE) # MP3 File info POPUP
QString
AppMediaPlayer_Controller_Audio::GetCurrentFolderName()
{
    return (getDevice() < MP::ALL_DEVICES) ? m_history[getDevice()].Folder : "Unknown";
}

QString
AppMediaPlayer_Controller_Audio::GetCurrentBitRate()
{
    MediaInfo pInfo;
    GetCurrentMediaInfo( &pInfo);	

	// { modified by eugene.seo 2012.12.06 for bitrate fix
	//return (pInfo.bitrate!="") ? pInfo.bitrate : "Unknown";
    if(pInfo.bitrate!="")
    {
		double tmpNum = pInfo.bitrate.toDouble();
		if (tmpNum > 1000000)
		{
			tmpNum = tmpNum / 1000000;
			pInfo.bitrate = pInfo.bitrate.number((int)tmpNum) + "mbps"; //modified by jeeeun 2012-12-29 for Bitrate displays floating number 
		}
		else if (tmpNum > 1000 && tmpNum < 1000000)
		{
			tmpNum = tmpNum / 1000;
			pInfo.bitrate = pInfo.bitrate.number((int)tmpNum) + "kbps"; // modified by jeeeun 2012-12-29 for Bitrate displays floating number 
		}
		else
			pInfo.bitrate = pInfo.bitrate.number((int)tmpNum) + "bps";  //modified by jeeeun 2012-12-29 for Bitrate displays floating number 
			
	    return pInfo.bitrate;
    }	
	else 
	    return "Unknown";
	// } modified by eugene.seo 2012.12.06 for bitrate fix
}

QString
AppMediaPlayer_Controller_Audio::GetCurrentCreateDate()
{
    MediaInfo pInfo;
    GetCurrentMediaInfo( &pInfo);
    // { modified eugene.seo 2012.12.10 for enabling file createdate
    //return (pInfo.createdate!="") ? pInfo.createdate : "Unknown";
	
	pInfo.fileName.remove("file://");
	QFileInfo m_fileInfo(pInfo.fileName); 
	QDateTime m_createdTime = m_fileInfo.created();

	QString m_createdTimeStr = "";
	m_createdTimeStr = m_createdTime.toString("yyyy.MM.dd AP hh:mm:ss");
	
	return (m_createdTimeStr!="") ? m_createdTimeStr : "Unknown";
    // } modified eugene.seo 2012.12.10 for enabling file createdate
}

// delete eugene.seo 2012.12.10 for enabling file createdate


QString
AppMediaPlayer_Controller_Audio::GetCurrentFormat()
{
    QString fFormat;
    static QString ret;
    fFormat = GetCurrentFileName();
    ret = fFormat.right(3); 
	ret[0] = ret[0].toUpper(); // modified by eugene.seo 2012.12.06 for format fix

    if(getDevice() == MP::IPOD1 ||getDevice() == MP::IPOD2)
        ret = "AAC";
    else if(getDevice() ==  MP::DISC && m_discType == MEDIA_SOURCE_CDDA )
        ret = "CDA";
    
    return (ret!="") ? ret : "Unknown";  
}
// } add by yongkyun.lee@lge.com 

QString
AppMediaPlayer_Controller_Audio::GetCurrentFilePath()
{
    return (getDevice() < MP::ALL_DEVICES) ? m_history[getDevice()].Path : "";
}

QString
AppMediaPlayer_Controller_Audio::GetCurrentSongName()
{
    return (getDevice() < MP::ALL_DEVICES) ? m_history[getDevice()].Title : "Unknown title";
}

QString
AppMediaPlayer_Controller_Audio::GetCurrentArtist()
{
    return (getDevice() < MP::ALL_DEVICES) ? m_history[getDevice()].Artist : "";
}

QString
AppMediaPlayer_Controller_Audio::GetCurrentGenre()
{
    return (getDevice() < MP::ALL_DEVICES) ? m_history[getDevice()].Genre : "";
}
//{added by junam 2013.11.11 for ITS_NA_208040
QString AppMediaPlayer_Controller_Audio::GetCurrentCover()
{
    return (getDevice() < MP::ALL_DEVICES) ? m_history[getDevice()].Cover : "";
}
//}added by junam
QString
AppMediaPlayer_Controller_Audio::GetMountPathFromUSB(MP::DEVICE_T source )
{
    if(source == MP::USB1)
        return m_history[MP::USB1].MountPath.isEmpty() ? \
               m_history[MP::USB2].MountPath : m_history[MP::USB1].MountPath;;
    if(source == MP::USB2)
        return m_history[MP::USB2].MountPath.isEmpty() ? \
               m_history[MP::USB1].MountPath : m_history[MP::USB2].MountPath;
    // { modified by  yongkyun.lee 2012.11.21 for  xSampa check.
    //return "";
    return !m_history[MP::USB1].MountPath.isEmpty() ? m_history[MP::USB1].MountPath :\
         ( !m_history[MP::USB2].MountPath.isEmpty() ? m_history[MP::USB2].MountPath :  "" );
    // { modified by  yongkyun.lee  
}   
 
QString
AppMediaPlayer_Controller_Audio::GetMountPathFromIpod(MP::DEVICE_T source )
{
    if(source == MP::IPOD1)
        return m_history[MP::IPOD1].MountPath.isEmpty() ? \
               m_history[MP::IPOD2].MountPath : m_history[MP::IPOD1].MountPath;;
    if(source == MP::IPOD2)
        return m_history[MP::IPOD2].MountPath.isEmpty() ? \
               m_history[MP::IPOD1].MountPath : m_history[MP::IPOD2].MountPath;
    // { modified by  yongkyun.lee 2012.11.21 for  xSampa check.
    //return "";
    return !m_history[MP::IPOD1].MountPath.isEmpty() ? m_history[MP::IPOD1].MountPath :\
         ( !m_history[MP::IPOD2].MountPath.isEmpty() ? m_history[MP::IPOD2].MountPath :  "" );
    // { modified by  yongkyun.lee  
}    
// } modified by  yongkyun.lee@lge.com  

// { added by cychoi 2014.02.05 for MP3 file name
QString
AppMediaPlayer_Controller_Audio::GetCurrentMP3FileName()
{
    if(getDevice() != MP::DISC || m_discType != MEDIA_SOURCE_MP3)
    {
        return QString();
    }

    QString strTmp1 = "";

    if(getTuneTrackNamesFromCATree())
    {
        int trackIndex = m_pIMedia->GetCurrentIndex();
        if(m_tuneTrackIDs.isEmpty())
        {
            if(trackIndex >= 0 && trackIndex < m_tuneTrackNames.size())
            {
                strTmp1 = m_tuneTrackNames.at(trackIndex);
            }
        }
        else
        {
            int trackID = m_tuneTrackIDs.indexOf(trackIndex + 1);
            if(trackID >= 0 && trackID < m_tuneTrackNames.size())
            {
                strTmp1 = m_tuneTrackNames.at(trackID);
            }
        }
    }

    if(strTmp1.isEmpty())
    {
        strTmp1 = m_pIMedia->GetCurrentFileName();
    }

    return strTmp1;
}
// } added by cychoi 2014.02.05

QString
AppMediaPlayer_Controller_Audio::GetCurrentFileName()
{
    if(m_pIMedia == 0) return "";

    QString strTmp1 = "";
    QString strTmp2 = "";

    //{ modified by cychoi 2013.10.19 for "Track" display by Language Setting
    // { wonseok.heo for NOCR get history title 2014.02.04
    if(getDevice() == MP::DISC)
    {
        if(m_discType == MEDIA_SOURCE_CDDA)
        {
            strTmp1 = QString::number(m_pIMedia->GetCurrentIndex() + 1); // added by wonseok.heo for ITS 197433 2013.10.28
            //strTmp1 = QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_pIMedia->GetCurrentIndex() + 1);
        }
        else //if(MEDIA_SOURCE_MP3)
        {
            if(m_history[MP::DISC].Title == ""){
                strTmp1 = GetCurrentMP3FileName(); // modified by cychoi 2014.02.05 for MP3 file name
            }else{
                strTmp1 = m_history[MP::DISC].Title;
            }
        }
    }
    // } wonseok.heo for NOCR get history title 2014.02.04
    else
    {
        strTmp1 = m_pIMedia->GetCurrentFileName();
    }
    //} modified by cychoi 2013.10.19

    if(!strTmp1.isEmpty())
    {
        int nIndex = strTmp1.lastIndexOf('/');
        strTmp2 = strTmp1.remove(0, nIndex + 1);
        return strTmp2;
    }

    return "Unknown file";
}

QString
AppMediaPlayer_Controller_Audio::GetCurrentAlbumName()
{
    if (getDevice() == MP::JUKEBOX ||
        getDevice() == MP::USB1 || getDevice() == MP::USB2 ||
        getDevice() == MP::IPOD1 || getDevice() == MP::IPOD2)
    {
        if (m_pIMedia == 0) return "";

        MediaInfo tMediaInfo;
        m_pIMedia->GetCurrentMediaInfo(&tMediaInfo);

        return encodeString (tMediaInfo.Album);
    }

    return "";
}

//{Added by junam for CR9528 
bool
AppMediaPlayer_Controller_Audio::GetCurrentMediaInfo(MediaInfo *pInfo)
{
    if(!m_pIMedia)
        return false;
    m_pIMedia->GetCurrentMediaInfo(pInfo);

    /*For iPod : iPod send empty file name*/
    if (pInfo->fileName.isEmpty())
    {
        //{ modified by cychoi 2013.10.19 for "Track" display by Language Setting
        // { modified by cychoi 2014.02.05 for MP3 file name
        if(getDevice() == MP::DISC)
        {
            if(m_discType == MEDIA_SOURCE_CDDA)
            {
                pInfo->fileName = QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_pIMedia->GetCurrentIndex() + 1);
            }
            else //if(MEDIA_SOURCE_MP3)
            {
                pInfo->fileName = GetCurrentMP3FileName();
            }
        }
        // } modified by cychoi 2014.02.05
        else
        {
            pInfo->fileName = m_pIMedia->GetCurrentFileName();
        }
        //} modified by cychoi 2013.10.19
    }
   // { changed by junam 2012.08.16 for CR12072
//  if(!pInfo->fileName.startsWith("file://"))
//      pInfo->fileName.prepend("file://");
    if (pInfo->fileName.compare("No Tracks Playing") == 0)
        pInfo->fileName.clear();
    if( pInfo->fileName.isEmpty() == false && !pInfo->fileName.startsWith("file://"))
        pInfo->fileName.prepend("file://");
 // { changed by junam

    return true;
}
//}Added by junam

void
AppMediaPlayer_Controller_Audio::PauseBT()
{
   MP_LOG << LOG_ENDL;

    //{ modified by hyochang.ryu 20130517
   sendToBTCommand(EVT_BT_MUSIC_PAUSE);
    //} modified by hyochang.ryu 20130517
}

void
AppMediaPlayer_Controller_Audio::Play()
{

    MP::DEVICE_T device = getDevice();
    MP_HIGH << DeviceName(device)<<m_btSending<<m_bIsBTPlaying << LOG_ENDL;
    m_NoOSD = false; //modified by yongkyun.lee@lge.com 2012.07.14  CR 11922
    if (!isPlayerActive())
    {
        MP_LOG << "Player is not active." << LOG_ENDL;
        return;
    }

    // { added by wonseok.heo for DIST IN OUT TEST
    if(device == MP::JUKEBOX && GetEngineMain().getInOutTestMode() == true )
        return;
    // } added by wonseok.heo for DIST IN OUT TEST

    //{ modified by yongkyun.lee 2013-12-13 for : ITS 213657
    if( device == MP::BLUETOOTH && m_iPodMountTimer->isActive() )
    {
        MP_LOG << "m_iPodMountTimer active." << LOG_ENDL;
        m_bBTDelayedPlayOnIPodMount = true; // added by cychoi 2014.04.03 for ITS 233324
        return;      
    }
    else
    //} modified by yongkyun.lee 2013-12-13 
        setAudioPathRPM ();

    //GetEngineMain().setUnMuteState(true); //remove by edo.lee 2013.04.20  for Mute
    GetEngineMain().resetKeepPlayState();//added by edo.lee 2013.08.01 bt call on pwroff

    //{ modified by yongkyun.lee 2013-12-13 for : ITS 213657
    if((device == MP::IPOD1 || device == MP::IPOD2))
    {
        MP_MEDIUM << "setting unmute for ipod when playing" << LOG_ENDL;
        iPodMountedTimerStop();
        m_bBTDelayedPlayOnIPodMount = false; // added by cychoi 2014.04.03 for ITS 233324
        if( GetEngineMain().GetMuteState())
            GetEngineMain().setUnMuteState(true);
    }
    //} modified by yongkyun.lee 2013-12-13 

    if( device == MP::BLUETOOTH )
    {
        // { added by sangmin.seol For ITS 0232871 maintain pause state when BT Call started in power off state
        if( GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->isMaintainPause()==false)
        {
            if(GetEngineMain().GetMuteState())
                GetEngineMain().setUnMuteState(true);
            emit resetPlayIconBT();
            //deleted by yongkyun.lee 2013-10-10 for : ITS 191703 BT getinfo 0.5sec
            //{ modified by yongkyun.lee 2014-03-13 for : BT streaming Mute-play control
            if(GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->getModeStateHistoryFMode())
            {
                GetEngineMain().onBTPlayUnmuteTimerStart();
                GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setModeStateHistoryFMode(false);
            }
            //} modified by yongkyun.lee 2014-03-13

            sendToBTCommand(EVT_BT_MUSIC_PLAY);
        }
        // } added by sangmin.seol For ITS 0232871 maintain pause state when BT Call started in power off state

        //onBtRequestTimerStart();//{ modified by yongkyun.lee 2013-10-10 for : ITS 191703 BT getinfo 0.5sec
    }
    else if (device == MP::DISC && m_pIMedia)
    {
        if(GetEngineMain().getInOutTestMode() == true )
        {
            MP_LOG << "DiscTestPlay" << LOG_ENDL;

            //{added by wonseok.heo 2013.07.04 disc in out test
            if(m_InOutStepCnt ==100)
            {
                //m_pIMedia->Stop();
                m_InOutStepCnt =0;
                return;
            }

            m_pDeckController = GetControllers().GetDeckController();

            DeckEngineerMode  *pDeckEngine = GetEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();

           // m_pIMedia->Stop();
            m_pIMedia->SetCurrentMediaPosition(0);
            m_pIMedia->SetCurrentIndex( 0 );
            //m_pIMedia->DiscTestPlay ();
            pDeckEngine->DiscInOutTest();
            m_history[device].Index    = 0;
            m_history[device].Position = 0;

        }
        else
        {
            //}added by wonseok.heo 2013.07.04 disc in out test
            if( GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->isMaintainPause()==false)  // added by sangmin.seol For ITS 0232871 maintain pause state when BT Call started in power off state
            {
                m_pIMedia->Play();
            }
        } //added by wonseok.heo 2013.07.04 disc in out test
    }
    else if (m_pIMedia && device < MP::ALL_DEVICES)
    {
        // { modified by kihyung 2013.2.7
        PLAYER_STATE state = (PLAYER_STATE)m_pIMedia->GetCurrentMediaState();
        int          index = m_history[device].Index;
        switch(state)
        {
            case PLAYER_SEARCH: //added by junam 2012.10.24 for CR14772
            case PLAYER_PLAY:
            {
                // { modified by kihyung 2013.1.2 for ISV 68243
                if(index != m_pIMedia->GetCurrentIndex()) {
                    MP_LOG << "invoke m_pIMedia->Stop()" << LOG_ENDL;
                    m_pIMedia->Stop();
                }
                // } modified by kihyung 
                // break;
            }
            case PLAYER_STOP:
            {
                int nPosition = 0;
                //{changed by junam 2013.10.03 for iPod select same song
                //if(index == m_pIMedia->GetCurrentIndex())
                if(index == m_pIMedia->GetCurrentIndex() && device != MP::IPOD1 && device != MP::IPOD2) //}changed by junam
                {
                    nPosition = m_history[device].Position;
                    m_pIMedia->SetCurrentMediaPosition(nPosition);
                }
                //emit clearTpMessage(); // added by wspark 2013.04.09 for cluster IOT #37 //deleted by wspark 2013.04.17
                m_pIMedia->SetCurrentIndex(index);
                MP_LOG << "index:" << index << "position:" << nPosition << LOG_ENDL;
            }
            case PLAYER_PAUSE:
            {
                bool isMaintainPause = GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->isMaintainPause();
                bool isBTcalling =  GetEngineMain().getisBTCall();
				
                MP_LOG << "invoke m_pIMedia->Play()"<<isMaintainPause<<isBTcalling << LOG_ENDL;
                if( GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->isMaintainPause()==false /*||GetEngineMain().getisBTCall()==false*/)
                {
                    m_pIMedia->Play();
                }
                break;
            }
            default:
                MP_LOG << "Ignored for state" << state << LOG_ENDL;
                break;
        }
        // } modified by kihyung 2013.2.7        
    }
}

void
AppMediaPlayer_Controller_Audio::Continue()
{
    MP_LOG << DeviceName(getDevice()) << LOG_ENDL; // added by kihyung 2012.08.01 for CR 12268
    m_NoOSD = false; //modified by yongkyun.lee@lge.com 2012.07.14  CR 11922
    if (!IsPlaying())
    {
        Play();
    }
}

void
AppMediaPlayer_Controller_Audio::Pause()
{
    MP::DEVICE_T device = getDevice();
    MP_HIGH << DeviceName(device) << LOG_ENDL; // added by kihyung 2012.08.01 for CR 12268
    m_NoOSD = false; //modified by yongkyun.lee@lge.com 2012.07.14  CR 11922
    if( device == MP::BLUETOOTH )
    {
        // { added by cychoi 2014.04.03 for ITS 233324
        if(m_iPodMountTimer->isActive())
        {
            MP_LOG << "m_iPodMountTimer active." << LOG_ENDL;
            m_bBTDelayedPlayOnIPodMount = false;
        }
        // } added by cychoi 2014.04.03
        PauseBT();
    }
    else if (m_pIMedia)
    {
        m_pIMedia->Pause();
    }
}

void
AppMediaPlayer_Controller_Audio::Stop()
{
    // { added by cychoi 2013.10.11 for ITS 193329 AV Mode Change Failure
    MP::DEVICE_T device = getDevice();
    MP_HIGH << DeviceName(device) << LOG_ENDL; // added by kihyung 2012.08.01 for CR 12268
    if(device >= MP::ALL_DEVICES)
    {
        // If current AV mode is Undefined, just return.
        return;
    }
    // } added by cychoi 2013.10.11

    m_pAudioSinkTimer->stop(); // added by kihyung 2013.07.04 for ISV 85455
    
    // { added by eugene 2012.11.26 for ISV #59906
	if(m_EventId == EVT_FB_OVERLAY_BLEND_OFF){
		emit resetCurrentIndex();
	}
    // } added by eugene 2012.11.26 for ISV #59906
    m_NoOSD = false; //modified by yongkyun.lee@lge.com 2012.07.14  CR 11922
    //{ added by eunkoo 2012.07.26 for CR11898
    if( device == MP::BLUETOOTH )
    {    
        onBtPeriodicGetInfoTimerStop(); // added by cychoi 2014.06.05 for Get Info - every 10sec after PlayStarted (some phones don't send TrackChanged)
        //{ modified by hyochang.ryu 20130517
        sendToBTCommand(EVT_BT_MUSIC_STOP);
        //} modified by hyochang.ryu 20130517
    } 
	//}added by eunkoo 
    else if (m_pIMedia)
    {
        m_pIMedia->Stop();
    }
}

//{ added by hyochang.ryu 20130517
void
AppMediaPlayer_Controller_Audio::Toggle()
{
    //{ modified by yongkyun.lee 2013-10-10 for : ITS 193552
    if(GetMediaMode() == MP::BLUETOOTH )
    {
        if(GetEngineMain().getisBTCall()==true)
        {
            showBTCallPopup();
            return;
        }
    }
    //} modified by yongkyun.lee 2013-10-10 

    setAudioPathRPM();
    MP_HIGH << GetEngineMain().GetMuteState() << GetEngineMain().GetActiveMode() << m_bIsBTPlaying << LOG_ENDL;
    
    if(GetEngineMain().GetMuteState() && !m_bIsBTPlaying) //modified by yongkyun.lee 2013-12-11 for : ITS 214631
        DelayedUnmute4BT();
    
    emit CommandOnBT(EVT_BT_MUSIC_STOP);
}
//} added by hyochang.ryu 20130517

//} added by hyochang.ryu 20130705
void
AppMediaPlayer_Controller_Audio::ResumeBT()
{
    MP_LOG << LOG_ENDL;
    if (m_bBTAvrcpAbove13) Play();
}
//} added by hyochang.ryu 20130705

void
AppMediaPlayer_Controller_Audio::functionStop()
{
    // { added by cychoi 2013.10.11 for ITS 193329 AV Mode Change Failure
    MP::DEVICE_T device = getDevice();
    MP_HIGH << DeviceName(device) << LOG_ENDL;
    if(device >= MP::ALL_DEVICES)
    {
        // If current AV mode is Undefined, just return.
        return;
    }
    // } added by cychoi 2013.10.11

    m_pAudioSinkTimer->stop(); // added by kihyung 2013.07.04 for ISV 85455
    
    m_NoOSD = false;

    if( device == MP::BLUETOOTH )
    {    
        onBtPeriodicGetInfoTimerStop(); // added by cychoi 2014.06.05 for Get Info - every 10sec after PlayStarted (some phones don't send TrackChanged)
        // { modified by lssanh 2013.04.06 ISV69184 BT paused by changesource
        //emit CommandOnBT(EVT_BT_MUSIC_STOP);
        //if (IsPlaying()) // modified by yongkyun.lee 2014-03-21 for : NOCR : BT controller 
        //{
            Pause();

            m_history[device].State = PLAYER_PAUSE;     // } added by sangmin.seol For ITS 0232871 maintain pause state when BT Call started in power off state
        //}
        // } modified by lssanh 2013.04.06 ISV69184 BT paused by changesource
        //m_bIsBTPlaying = false; //deleted by yongkyun.lee 20130426 for : BT sync
    } 
	else if (m_pIMedia)
    {
        // { modified by kihyung 2013.2.8
        if( device == MP::IPOD1 ||  device == MP::IPOD2)
        {
            //{added by junam 2013.09.03 for ITS_KOR_187097
            if(m_isLongPressed)
            {
                setIsLongPress(0);
                emit clearSeekMode();
            }
            //}added by junam

            iPodDeviceOff(); // moved before Pause() by junam 2012.12.13

            //changed by junam 2013.09.06 for ITS_KOR_188268
            //if (IsPlaying())
            if (m_pIMedia->GetCurrentMediaState() != PLAYER_PAUSE) //}changed by junam
                Pause();
        }
        else {
            m_pIMedia->FunctionStop();

            // added by sangmin.seol For ITS 0232871 maintain pause state when BT Call started in power off state
            if (device == MP::DISC)
            {
                m_history[device].State = PLAYER_STOP;
            }
        }
        // } modified by kihyung 2013.2.8        
    }
}

void
AppMediaPlayer_Controller_Audio::UpdateClusterTrackIndex(uint current, uint total){
	emit trackClusterChanged(current, total);
}

//added by edo.lee 2013.05.09
void
AppMediaPlayer_Controller_Audio::RequestTrackInfo(bool bPrev, int trackIndex)
{
    MP::DEVICE_T device = getDevice();

    // { modified by cychoi 2013.06.18 for ISV 85460 DVD OSD mismatch
	if (  device == MP::UNDEFINED ) //modified by edo.lee 2013.05.19
	//if (  m_device == MP::UNDEFINED || m_device == MP::DISC) //modified by edo.lee 2013.05.19
    // } modified by cychoi 2013.06.18
    {
        MP_LOG << "return m_device = " << getDevice() << LOG_ENDL;
        return;
    }

    MP_LOG << bPrev << ", "<< device << ", " << trackIndex << LOG_ENDL;  
	
	//m_moveTrackIndex += track;
    /*
    if(m_moveTrackIndex == trackIndex) //added by edo.lee 2013.05.25
    {
        MP_LOG<<"same track index...skip" << LOG_ENDL;
        //added by edo.lee 2013.10.12 ITS 0195214
        if ( device == MP::JUKEBOX || device == MP::USB1 || device == MP::USB2 )
            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->showCurrentOSD();
		
        return;
    }*/ //removed by suilyou 20131106 ITS 0206842
    
        m_moveTrackIndex = trackIndex;        
    /*if( m_history[m_device].Quantity == m_moveTrackIndex)
		m_moveTrackIndex = 0;
	else if (m_moveTrackIndex == -1)
		m_moveTrackIndex = m_history[m_device].Quantity-1 ;
	 */
	if ( device == MP::DISC )
    {    	
    	if( trackIndex == -1)
            return;
        //{ added by yongkyun.lee 20130703 for : ITS 177742
        //if(m_discType == MEDIA_SOURCE_MP3 || m_discType == MEDIA_SOURCE_CDDA)
        //{
        //    return;
        //}
        //} added by yongkyun.lee 20130703 

        MP_MEDIUM<<"DISC" << LOG_ENDL;
        //{ modified by wonseok.heo for NO CR delete unknown 2013.08.20
        QString artist = "";
        QString song = "";
        QString album = "";
        QString folder = "";
        // } modified by wonseok.heo for NO CR delete unknown 2013.08.20
        QString coverart = "";

        //{modifyed by wonseok.heo for ITS 175732 175697 2013.06.23
        if(m_discType == MEDIA_SOURCE_MP3 )
        {
            if(getTuneTrackNamesFromCATree())
            {
                if (m_tuneTrackIDs.isEmpty())
                {
                    if( m_moveTrackIndex >= 0 && m_moveTrackIndex < m_tuneTrackNames.size() )
                    {
                        song = m_tuneTrackNames.at(m_moveTrackIndex);
                    }
                }
                else
                {
                    int idxID=m_tuneTrackIDs.indexOf(m_moveTrackIndex + 1);
                    if(idxID >= 0 && idxID < m_tuneTrackNames.size())
                    {
                        song = m_tuneTrackNames.at(idxID);
                        m_history[device].Title = song; // added by wonseok.heo for NOCR 2014.02.06
                    }
                    // { added by cychoi 2013.10.07 for ISV 92215 Folder Display on Track Change
                    if(idxID >= 0 && idxID < m_pathsTune.size())
                    {
                        folder = m_pathsTune.at(idxID);
                    }
                    // } added by cychoi 2013.10.07


                    m_pTreeModel->FolderCheck(folder, idxID,trackIndex + 1); //added by wonseok.heo for NOCR List Focus of Disc 2013.12.30

                }

                if( song.isEmpty())
                    song = QString(""); //modified by wonseok.heo for NO CR delete unknown 2013.08.20
                //{ modified by yongkyun.lee 2013-08-24 for : NO CR : Folder is null
                //folder = m_pTreeModel->getCurrentFolderName(); // commented by cychoi 2013.10.07 for ISV 92215 Folder Display on Track Change
                //} modified by yongkyun.lee 2013-08-24 
            }
            EmitOnMediaInfoTempChanged(device, song, album, artist, "", "", "", folder, ""); // added by wonseok.heo for ITS 197433 2013.10.28

        }
        else //if( m_discType == MEDIA_SOURCE_CDDA)
        {
			if (m_moveTrackIndex > -1 && m_listCDContent.count() > m_moveTrackIndex )
			{
				CD_META_INFO cdInfo = m_listCDContent.at(m_moveTrackIndex);
				song =  cdInfo.trackTitle;
				artist = cdInfo.artist;
				album =cdInfo.albumTitle;
				coverart = cdInfo.cover;
                emit mediaInfoClusterChanged(song); // added by wonseok.heo for ITS 197433 2013.10.28
                EmitOnMediaInfoTempChanged(device, song, album, artist, "", "", "", folder, ""); // added by wonseok.heo for ITS 197433 2013.10.28
			}
			else
			{
                //{ modified by cychoi 2013.10.19 for "Track" display by Language Setting
                song = QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_moveTrackIndex + 1);
                emit mediaInfoClusterChanged(song); // added by wonseok.heo for ITS 197433 2013.10.28
                EmitOnMediaInfoTempChanged(device, QString::number(m_moveTrackIndex + 1), album, artist, "", "", "", folder, ""); // added by wonseok.heo for ITS 197433 2013.10.28
                //song = QString("Track %1").arg( m_moveTrackIndex+1);                  
                //} modified by cychoi 2013.10.19
			}
		}
        // }modifyed by wonseok.heo for ITS 175732 175697 2013.06.23

        //EmitOnMediaInfoTempChanged(device, song, album, artist, "", "", "", folder, ""); // modified by kihyung 2013.08.27 for ITS 0186708

        // { modified by wonseok.heo for NO CR count issue 2013.08.20
        if(m_discType == MEDIA_SOURCE_MP3 )
        {
            if(m_nFolderSize > 0)//{ modified by yongkyun.lee 2013-11-06 for : smoke test mp3cd
            {
                if(GetEngineMain().middleEast())
                    emit tuneCounter(QString::number(m_nFolderSize) + '/' + QString::number(m_seekIndex + 1));
                else
                    emit tuneCounter(QString::number(m_seekIndex + 1) + '/' + QString::number(m_nFolderSize));
            }
        }else{
            emit tuneCounter(GetFilesCountInfo(m_seekIndex, m_nFolderSize));
        }
        // } modified by wonseok.heo for NO CR count issue 2013.08.20

        // removed by cychoi 2013.08.17 for unused code
        // { modified by wonseok.heo for ITS 197433 2013.10.28
        // { added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
        if( m_discType == MEDIA_SOURCE_CDDA)
        {
            emit trackClusterChanged(m_seekIndex + 1, m_history[device].Quantity);
        }else{
            emit mediaInfoClusterChanged(song);
            emit trackClusterChanged(m_seekIndex + 1, m_history[device].Quantity);
        }
        // } added by cychoi 2013.08.17
        // } modified by wonseok.heo for ITS 197433 2013.10.28

		GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
                        song, m_randomPlayMode, m_repeatPlayMode, m_scanMode, false); // modified by cychoi 2013.11.12 for OSD Title Translation
		//setMediaInfoToHistory(AudioUtils::DeviceType(m_device),song,folder,0);
		//GetEngineMain().GetNotifier()->onSetDisplayOSD(true);
	}
    else 
	{
		QString filePath="";
		if(m_moveTrackIndex > -1 && m_history[device].playList.list != NULL && m_history[device].playList.list->count() > m_moveTrackIndex ){

			filePath=m_history[device].playList.list->at(m_moveTrackIndex);
            if (!filePath.startsWith("file://"))
			    filePath.prepend("file://");
		}
	
		if ( device == MP::IPOD1 || device == MP::IPOD2){
            MP_MEDIUM<<"IPOD" << LOG_ENDL;
			//modified by edo.lee 2013.05.25
			GetEngineMain().setReceivedTrackInfo(1);
			/*CIpodController *pController = GetIPodController(m_device);
			int tag = bPrev ? REQUID_PREV_TRACK :  REQUID_NEXT_TRACK;
			tag += m_moveTrackIndex;//added by edo.lee 2013.05.18
			if(pController)
			{
			    if(!pController->requestData( eGetMusicSimpleMetaDataQuery_Id, GET_TA_REQUID(tag), m_moveTrackIndex, 1))
			    {
			    	MP_LOG<<"here" << LOG_ENDL;
			        m_pTrackerAbstractor->RequestData( filePath, eGetMusicSimpleMetaDataQuery_Id, GET_TA_REQUID(tag));
			    }
			}*/
		}
		else
		{	
            MP_MEDIUM<<"JB USB" << LOG_ENDL;
			if(0 <= m_moveTrackIndex && m_history[device].playList.list->count() > m_moveTrackIndex)
			{ 
                MP_LOG<<"RequestData success "<<m_moveTrackIndex << LOG_ENDL;
				int tag =bPrev? REQUID_PREV_TRACK :  REQUID_NEXT_TRACK;
				tag += m_moveTrackIndex;//added by edo.lee 2013.05.18
                // { modified by sungha.choi 2013.07.01
                //if ( (device == MP::JUKEBOX && !isMinerFinished()) || (device == MP::USB1 && !isUSB1MinerFinished()) || (device == MP::USB2 && !isUSB2MinerFinished()) )
                if ( device == MP::JUKEBOX || device == MP::USB1 || device == MP::USB2 )    // modified by sungha.choi 2013.08.13 for P1-pretest always file name will be showed
                {
                    //m_pTrackerAbstractor->RequestData( filePath, eTrackerExtractMetadataQuery_Id, GET_TA_REQUID(tag));    // deleted by sungha.choi 2013.07.13
                    // { added by sungha.choi 2013.07.13
                    MP_MEDIUM << "show file name instead of rquesting to TA eGetMusicSimpleMetaDataQuery_Id" << LOG_ENDL;
                    QString fileName = filePath;
                    //{added by junam 2013.12.19 for ITS_NA_216396
                    fileName = fileName.remove( "file://" );
                    if( m_history[device].Quantity == 1 && m_history[device].Index == m_seekIndex && m_history[device].Path == fileName)
                    {
                        MP_LOG<<"Same track will be played. No need update Temp media info" << LOG_ENDL;

                        fileName = m_history[device].Title;     // added by sangmin.seol 2013.12.19 for ITS_0223599 If there is no change of the song, display song title to the OSD
                    }
                    else //}added by junam
                    {
                        m_history[device].Index = m_seekIndex;
                        m_history[device].Path  = fileName;
                        fileName.remove(0, fileName.lastIndexOf('/') + 1);
                        EmitOnMediaInfoTempChanged(device, fileName, "", "", "", "", "", getFolderName(filePath), ""); // modified by kihyung 2013.08.27 for ITS 0186708
                    }
                    // modified by Dmitry 15.08.13 for ITS0184365
                    if (GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->getTuneEnabled())
                    {
                        if(GetEngineMain().middleEast())
                            emit tuneCounter( QString::number(m_nFolderSize) + '/' + QString::number(m_seekIndex + 1));//added by edo.lee 2013.05.30
                        else
                            emit tuneCounter( QString::number(m_seekIndex + 1) + '/' + QString::number(m_nFolderSize));//added by edo.lee 2013.05.30
                    }
                    // emit positionChanged(0);//added by edo.lee 2013.05.24 // removed by kihyung 2013.07.23 for ITS 0181331

                    setMediaInfoToHistory(AudioUtils::DeviceType(device),fileName,getFolderName(filePath),0);

                    // { added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
                    emit mediaInfoClusterChanged(fileName);
                    emit trackClusterChanged(m_seekIndex + 1, m_history[device].Quantity);
                    // } added by cychoi 2013.08.17

                    GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData( (OSD_DEVICE)getOSDevice(device),
                    fileName, m_randomPlayMode, m_repeatPlayMode, m_scanMode, false);
					
                    // GetEngineMain().GetNotifier()->onSetDisplayOSD(true);
                    // } added by sungha.choi 2013.07.13
                }
                else
                {
                    MP_LOG << "request to TA eGetMusicSimpleMetaDataQuery_Id" << LOG_ENDL;
                    m_pTrackerAbstractor->RequestData( filePath, eGetMusicSimpleMetaDataQuery_Id, GET_TA_REQUID(tag));
                }
                // } modified by sungha.choi 2013.07.01
             }
		    	
		}
		//GetEngineMain().setReceivedTrackInfo(1);
		//MP_LOG<<"EDO "<< GetEngineMain().getReceivedTrackInfo()  << LOG_ENDL;
	}
}
void
AppMediaPlayer_Controller_Audio::setMediaInfoToHistory(MEDIA_SOURCE_T device, QString filename, QString foldername, int position, int random, int repeat)//modified by edo.lee 2013.07.05
{
    GetEngineMain().GetNotifier()->setHistoryInfo(device,filename,foldername,position);
    GetEngineMain().GetNotifier()->setHistoryPlayMode(random, repeat);//added by edo.lee 2013.07.05		
    GetEngineMain().GetNotifier()->onSetDisplayOSD(true);	//added by hyochang.ryu 20130817 for ITS184947
        //if(GetEngineMain().GetNotifier()->continueDisplayOSD(true))//added by edo.lee 2013.07.08
        //	GetEngineMain().GetNotifier()->displayOSD(	OSD_EVENT_AUDIO_TRACK_INFO );
}
//added by edo.lee 2013.05.09

void
AppMediaPlayer_Controller_Audio::NextTrack(int isUnsurpported)  // modified by sangmin.seol 2013-12-13 for : ITS 0215489
{ 
    MP::DEVICE_T device = getDevice();
    if((device >= MP::ALL_DEVICES) || (device == MP::AUX)) return; // modified by oseong.kwon 2014.01.14 for ITS 219671
 
    // if(GetEngineMain().getisBTCall() == true) return; // commented by Sergey 17.05.2013
    //{ modified by yongkyun.lee 2013-10-10 for : ITS 193552
    if(GetMediaMode() == MP::BLUETOOTH && GetEngineMain().getisBTCall()==true)
    {
        //{ modified by yongkyun.lee 2013-11-12 for : ITS 208614           
        if(isForeground ())
            showBTCallPopup();
        else
            NotSupportBTOSD();
        return;
        //} modified by yongkyun.lee 2013-11-12 
     }
    //} modified by yongkyun.lee 2013-10-10 

    MP_HIGH << "Device = " << device << LOG_ENDL;
    emit tuneTextColorChanged(false);
    // {  added by wonseok.heo for Disc In out Test 2014.01.07
    if(GetEngineMain().getInOutTestMode() == true && getDevice() == MP::DISC )
        return;
    // }  added by wonseok.heo for Disc In out Test 2014.01.07

    // SUILYOU sound MUTE
    if(m_pLongBtnTimer->isActive())
    {
        GetEngineMain().setUnMuteState(true);
        m_pLongBtnTimer->stop(); // added by Dmitry 13.10.13
    }
    m_pAudioSinkTimer->stop(); // added by kihyung 2013.07.04 for ISV 85455
    
    m_NoOSD = true; //modified by yongkyun.lee@lge.com 2012.07.14  CR 11922

    // removed by kihyung 2013.09.13 for ISV 89552
    // if(device != MP::IPOD1 && device != MP::IPOD2) //added by junam 2013.08.18 for iPod next meta data
    //    ResetTrackInfo(device);

    //Added if condition to exclude ipod for mute setting by Tan 2013.09.02
    if( (device != MP::BLUETOOTH && device != MP::IPOD1 && device != MP::IPOD2)
            && !m_skipTrack) //added by edo.lee 2013.08.30
    {
        GetEngineMain().setMuteForSeek(true); //modified by edo.lee 2013.04.21  for Mute
    }

    //Added unmute flag by Tan 2013.09.02
    if(device == MP::IPOD1 || device == MP::IPOD2)
    {
        if(GetEngineMain().GetMuteState())
        {
            MP_LOG << "IPOD setting m_WillUnMute true"  << LOG_ENDL;
            m_WillUnMute = true;
        }
        MP_LOG << "IPOD sendVRCancelSignal"  << LOG_ENDL;
        GetEngineMain().sendVRCancelSignal();
    }

    // modified by sangmin.seol 2013-12-13 for : ITS 0215489
    if(device != MP::BLUETOOTH && !isUnsurpported)// modified by yongkyun.lee 2013-10-14 for : ITS 195269
        GetEngineMain().setIsTermOnTrackChange(true);
	m_skipTrack = false;
    //if(getDiscType() != MP::AUDIO_CD) //remove by edo.lee 2013.08.28
    //    onPositionChanged(0);//remove by edo.lee 2013.08.28
    if(getScanMode() != MP::SCANOFF)
        setScanMode(MP::SCANOFF);
	//RequestTrackInfo(NEXT_TRACK_INFO);//remove by edo.lee 2013.05.14
    /* If current state is not PLAYER_PLAY, need to set audioPath */
    if (!IsPlaying() || (device == MP::BLUETOOTH) )	//modifieded by hyochang.ryu 20130523 for BT Mute side effet
    {
        setAudioPathRPM ();
    }

    if( device == MP::BLUETOOTH )
    {
       //{ modified by yongkyun.lee 2013-08-28 for : BT OSD sync
       MP_MEDIUM << "send EVT_BT_MUSIC_NEXT_TRACK" << LOG_ENDL;
        //muteBeforePlay();	//deleted by hyochang.ryu 20130906 for ISV 90761
        sendToBTCommand(EVT_BT_MUSIC_NEXT_TRACK);
        //{ modified by yongkyun.lee 2013-08-28 for : BT OSD sync 
        //onBtRequestTimerStart();
        if(m_iTrackInfoReqCount == 0)
            m_iTrackInfoReqCount = m_iTrackInfoReqCount + 1;
        m_iTrackInfoReqCount = m_iTrackInfoReqCount + 1;
        //} modified by yongkyun.lee 2013-08-28 
        if(m_bBTAvrcpAbove13)
        {
            // { modified by oseong.kwon 2014.08.21 for ITS 246092, ITS 246095, ITS 246096 // { modified by cychoi 2014.05.28 for AVP only send "Next Track" command and BT Controller call Play if needed
            MP_LOG << "No EVT_BT_MUSIC_PLAY on Next Track." << LOG_ENDL;
            // Play(); 
            if(GetEngineMain().GetMuteState())
                GetEngineMain().setUnMuteState(true);
            emit resetPlayIconBT();
            // } modified by oseong.kwon 2014.08.21 // } modified by cychoi 2014.05.28
        }
        else 
        {
            //{ modified by yongkyun.lee 2013-10-24 for : ITS 196546
            if(GetEngineMain().GetMuteState()) 
            {
                GetEngineMain().setUnMuteState(true); 
                emit resetPlayIconBT();
            }                
            //} modified by yongkyun.lee 2013-10-24 
           //deleted  by yongkyun.lee 2013-10-10 for : ITS 191703 BT getinfo 0.5sec
             GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData
             (BLUETOOTH_MUSIC, "Next", MP::RANDOMOFF, MP::REPEATOFF, MP::SCANOFF, false);
        }
       //} modified by yongkyun.lee 2013-08-28 
    }
    else if( device !=  MP::DISC )
    {
        if(m_pIMedia == NULL || m_history[device].Quantity == 0) {
            return;
        }
        else
        {
            MP_LOG << "Call m_pIMedia->Next" << LOG_ENDL;

            // { add by changjin 2012.08.09 : for CR 12479
            if( m_enableErrorPopupState == ERROR_POPUP_READY )
                m_enableErrorPopupState = ERROR_POPUP_ENABLE;
            // } add by changjin

            //emit clearTpMessage(); // commented by cychoi 2013.08.17 for No clear TP message from HMC request // added by wspark 2013.04.17
            m_isForwardDirection = true; // added by wspark 2012.09.10 for CR12707
			// commented by ravikanth 25-04-13 for ITS 0175705
//            if (m_isListView)
//                m_pIMedia->Prev(1,1); // added by Michael.Kim 20130410 for New UX
//            else
                m_pIMedia->Next(1);
        }
        // } modified by kihyung
    }
    else if (m_pIMedia)
    {
        // { add by changjin 2012.08.09 : for CR 12479
        if( m_enableErrorPopupState == ERROR_POPUP_READY )
            m_enableErrorPopupState = ERROR_POPUP_ENABLE;
        // } add by changjin

        m_isForwardDirection = true; // added by wspark 2012.09.10 for CR12707

        // { removed by kihyung 2013.1.4 for ISV 68141
        /*
        // { add by lssanh 2012.09.10 for CR9362
        if (m_discType == MEDIA_SOURCE_MP3)
        {
            emit showPopupLoadingData();
        }
        // } add by lssanh 2012.09.10 for CR9362
        */
        // { removed by kihyung 2013.1.4 for ISV 68141
        //emit clearTpMessage(); // commented by cychoi 2013.08.17 for No clear TP message from HMC request // added by wspark 2013.04.17
		// commented by ravikanth 25-04-13 for ITS 0175705
//        if (m_isListView)
//            m_pIMedia->Prev(1,1); // added by Michael.Kim 20130410 for New UX
//        else
            m_pIMedia->Next(1);
    }

    //emit positionCleared(); // added by wspark 2013.03.30 for ISV 77359 // deleted by wspark 2013.04.09
    // { added by kihyung 2012.09.04 for 3PDMS 0136421 
    // emit positionChanged(0); // removed by kihyung 2013.3.28 for continuos next input.
    // } added by kihyung

    //{ modified by yongkyun.lee 2013-08-27 for : ISV 89645  : ITS 181278 delete
    //{ modified by yongkyun.lee 2013-07-30 for : ITS 181278
    //QEventLoop* loop = new QEventLoop(this);
    //loop->processEvents(QEventLoop::ExcludeUserInputEvents, 100); // added by Sergey 24.07.2013
    //} modified by yongkyun.lee 2013-07-30 
    //} modified by yongkyun.lee 2013-08-27 
    //emit clearTpMessage(); // added by wspark 2013.04.09 for cluster IOT #37 // deleted by wspark 2013.04.17
}

void
// { modified by lssanh 2012.09.18 for No35 car DQA#79
//AppMediaPlayer_Controller_Audio::PrevTrack(bool isVRPrev)
AppMediaPlayer_Controller_Audio::PrevTrack(bool isDirectPrev)
// } modified by lssanh 2012.09.18 for No35 car DQA#79
{
//    if(GetEngineMain().getisBTCall() == true) return; // commented by Sergey 17.05.2013

    MP::DEVICE_T device = getDevice();
    if((device >= MP::ALL_DEVICES) || (device == MP::AUX)) return; // modified by oseong.kwon 2014.01.14 for ITS 219671
    
    MP_HIGH << "Device = " << device << LOG_ENDL;
    emit tuneTextColorChanged(false);
    // {  added by wonseok.heo for Disc In out Test 2014.01.07
    if(GetEngineMain().getInOutTestMode() == true && getDevice() == MP::DISC )
        return;
    // }  added by wonseok.heo for Disc In out Test 2014.01.07

    //{ modified by yongkyun.lee 2013-10-10 for : ITS 193552
    if(GetMediaMode() == MP::BLUETOOTH && GetEngineMain().getisBTCall()==true)
    {
        //{ modified by yongkyun.lee 2013-11-12 for : ITS 208614
        if(isForeground ())
            showBTCallPopup();
        else
            NotSupportBTOSD();
        return;
        //} modified by yongkyun.lee 2013-11-12 
    }
    //} modified by yongkyun.lee 2013-10-10 

    // SUILYOU sound MUTE
    if(m_pLongBtnTimer->isActive())
    {
        GetEngineMain().setUnMuteState(true);
        m_pLongBtnTimer->stop(); // added by Dmitry 13.10.13
    }
    m_pAudioSinkTimer->stop(); // added by kihyung 2013.07.04 for ISV 85455
    
    m_NoOSD = true;  //modified by yongkyun.lee@lge.com 2012.07.14  CR 11922

    //{ removed by kihyung 2013.11.12 for ITS 0205241 
    /*
    //{ Added by by Eunhye 2013.01.24 for ISV 70720
    if (m_isListView){
        isDirectPrev = true;
		//onPositionChanged(0);//remove by edo.lee 2013.08.28
    }
    //} by Eunhye 2013.01.24    
    */
    //} removed by kihyung 2013.11.12 for ITS 0205241     

    //Added if condition to exclude ipod for mute setting by Tan 2013.09.02
    if( (device != MP::BLUETOOTH && device != MP::IPOD1 && device != MP::IPOD2)
            && !m_skipTrack) // added by edo.lee 2013.08.30
    {
        GetEngineMain().setMuteForSeek(true); //added by edo.lee 2013.04.21 for Mute
    }

    //Added unmute flag by Tan 2013.09.02
    if(device == MP::IPOD1 || device == MP::IPOD2)
    {
        if(GetEngineMain().GetMuteState())
        {
            MP_LOG<< "IPOD setting m_WillUnMute true" <<LOG_ENDL ;
            m_WillUnMute = true;
        }
        MP_LOG<< "IPOD sendVRCancelSignal()" <<LOG_ENDL ;
        GetEngineMain().sendVRCancelSignal();
    }

    m_skipTrack = false;

    //Modified setIsTermOnTrackChange for iPod by Tan 2013.09.25
    if((device != MP::IPOD1 && device != MP::IPOD2) && (device != MP::BLUETOOTH ))// modified by yongkyun.lee 2013-10-14 for : ITS 195269
    {
        GetEngineMain().setIsTermOnTrackChange(true);
    }
    //GetEngineMain().setIsTermOnTrackChange(true);

	if(getScanMode() != MP::SCANOFF)
		setScanMode(MP::SCANOFF);
    //{Modified by Radhakrushna 20121220 cr 16578
    //If current track duration is less than 3 sec , then reset track info.
    //Otherwise same track will be played from beginning.
    // removed by kihyung 2013.09.13 for ISV 89552    
    /*
    if(m_pIMedia && m_pIMedia->GetCurrentMediaPosition() < 3999 && (device != MP::IPOD1 && device != MP::IPOD2)) //add iPod condition by junam 2013.08.18
    {//3000){	modified by edo.lee 2013.06.07
        ResetTrackInfo(device);
		//RequestTrackInfo(PREV_TRACK_INFO);//remove by edo.lee 2013.05.14
    }
    */
    //}Modified by Radhakrushna 20121220 cr 16578
    
    // { added by eunhye 2013.02.22 for ISV 70720
    //if(!isDirectPrev && m_pIMedia && m_pIMedia->GetCurrentMediaPosition() > 3999)//3000) modified by edo.lee 2013.06.07
    //    emit updateOSD(OSD_EVENT_AUDIO_TRACK_INFO);
    // } added by eunhye 2013.02.22
    // { added by hyochang.ryu 20130711 for BT
    
    if(!isDirectPrev && m_pIMedia && m_pIMedia->GetCurrentMediaPosition() > 2999) // modified by wonseok.heo for 3999 -> 2999 ITS 211578 2013.11.29 || (device == MP::BLUETOOTH))
    {
        if(device == MP::BLUETOOTH) 
        {
            if(m_bBTAvrcpAbove13)
                showCurrentBTOSD();
        }
        else
        {
            //{ modified by shkim 2013-11-19 for : ITS 209908
            QString tmpTitle;
            if(device == MP::DISC && m_discType == MEDIA_SOURCE_CDDA && GetCDDAGraceNote() != GRACENOTE_INDEXING_DONE )
            {
                tmpTitle = QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_history[device].Title);
            }
            else
            {
                tmpTitle = m_history[device].Title;
            }
            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
              tmpTitle, m_randomPlayMode, m_repeatPlayMode, m_scanMode, false);
            //} modified by shkim 2013-11-19 for : ITS 209908
        }
    }
    // } added by hyochang.ryu 20130711 for BT

    /* If current state is not PLAYER_PLAY, need to set audioPath */
    if (!IsPlaying() || (device == MP::BLUETOOTH) )	//modifieded by hyochang.ryu 20130523 for BT Mute side effect
    {
        setAudioPathRPM ();
    }

    if( device ==  MP::BLUETOOTH )
    {
        //{ modified by yongkyun.lee 2013-08-28 for : BT OSD sync
        MP_MEDIUM << "send EVT_BT_MUSIC_PREV_TRACK" << LOG_ENDL;
        //deleted  by yongkyun.lee 2013-10-10 for :  ITS 191703 BT getinfo 0.5sec
        sendToBTCommand(EVT_BT_MUSIC_PREV_TRACK);
        //{ modified by yongkyun.lee 2013-08-28 for : BT OSD sync 
        //onBtRequestTimerStart(); 
        if(m_iTrackInfoReqCount == 0)
            m_iTrackInfoReqCount = m_iTrackInfoReqCount + 1;

        m_iTrackInfoReqCount = m_iTrackInfoReqCount + 1;
        //} modified by yongkyun.lee 2013-08-28 
        if(m_bBTAvrcpAbove13)
        {
            // { modified by oseong.kwon 2014.08.21 for ITS 246092, ITS 246095, ITS 246096 // { modified by cychoi 2014.05.28 for AVP only send "Prev Track" command and BT Controller call Play if needed
            MP_LOG << "No EVT_BT_MUSIC_PLAY on Prev Track." << LOG_ENDL;
            // Play(); 
            if(GetEngineMain().GetMuteState())
                GetEngineMain().setUnMuteState(true);
            emit resetPlayIconBT();
            // } modified by oseong.kwon 2014.08.21 // } modified by cychoi 2014.05.28
        }
        else
        {
            //{ modified by yongkyun.lee 2013-10-24 for : ITS 196546
            if(GetEngineMain().GetMuteState()) 
            {
                GetEngineMain().setUnMuteState(true); 
                emit resetPlayIconBT();
            }                
            //} modified by yongkyun.lee 2013-10-24         
             GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData
             (BLUETOOTH_MUSIC, "Previous", MP::RANDOMOFF, MP::REPEATOFF, MP::SCANOFF, false);
        }
       //} modified by yongkyun.lee 2013-08-28 
    }
    else if( device !=  MP::DISC )
    {
        // { modified by kihyung 2012.08.30 for 3PDMS 0137905
        /*
        if (m_pIMedia == NULL || m_history[m_device].Quantity == 0)
        {
            return;
        }

        if ( m_pIMedia->GetCurrentIndex() == 0 && ( m_pIMedia->GetCurrentMediaPosition() < 4 * 1000 || isVRPrev ) )
        {
            setCurrentIndex(GetMediaNum() - 1);
        }
        else
        {
            MP_LOG << "Call m_pIMedia->Prev" << LOG_ENDL;
            if (isVRPrev)
            {
               m_pIMedia->Prev(1,1);
            }
            else
            {
                // { add by changjin 2012.08.09 : for CR 12479
                if( m_enableErrorPopupState == ERROR_POPUP_READY )
                    m_enableErrorPopupState = ERROR_POPUP_ENABLE;
                // } add by changjin

                m_pIMedia->Prev(1);
            }
        }
        */
        if(m_pIMedia == NULL || m_history[device].Quantity == 0)
        {
            return;
        }
        else
        {
            MP_LOG << "Call m_pIMedia->Prev" << LOG_ENDL;
            // { commented by cychoi 2013.08.17 for No clear TP message from HMC request
            // { added by wspark 2013.04.09 for cluster IOT #37
            //if(m_pIMedia && m_pIMedia->GetCurrentMediaPosition() < 3000)
            //    emit clearTpMessage();
            // } added by wspark
            // } commented by cychoi 2013.08.17
            m_isForwardDirection = false; // added by wspark 2012.09.10 for CR12707
            
            // { modified by lssanh 2012.07.07 for No35 car DQA#79
            //if (isVRPrev)
            if (isDirectPrev)
            // } modified by lssanh 2012.07.07 for No35 car DQA#79
            {
			// commented by ravikanth 25-04-13 for ITS 0175705
//                if (m_isListView)
//                    m_pIMedia->Next(1); // added by Michael.Kim 20130410 for New UX
//                else
                    m_pIMedia->Prev(1,1);
            }
            else
            {
                // { add by changjin 2012.08.09 : for CR 12479
                if( m_enableErrorPopupState == ERROR_POPUP_READY )
                    m_enableErrorPopupState = ERROR_POPUP_ENABLE;
                // } add by changjin
				// commented by ravikanth 25-04-13 for ITS 0175705
//                if (m_isListView)
//                    m_pIMedia->Next(1); // added by Michael.Kim 20130410 for New UX
//                else
                    m_pIMedia->Prev(1);
            }
        }
        // } modified by kihyung
    }
    else if (m_pIMedia)
    {
        MP_LOG << "emit CommandOnVideoDec(DEC_PREV)" << LOG_ENDL;
        if (m_pIMedia == NULL )
        {
            return;
        }

        // { added by wspark 2013.04.09 for cluster IOT #37
        // { commented by cychoi 2013.08.17 for No clear TP message from HMC request
        //if(m_pIMedia && m_pIMedia->GetCurrentMediaPosition() < 3000)
        //    emit clearTpMessage();
        // } added by wspark
        // } commented by cychoi 2013.08.17
        m_isForwardDirection = false; // added by wspark 2012.09.10 for CR12707
        // { modified by lssanh 2012.09.18 for No35 car DQA#79        
        //if (isVRPrev)
        if (isDirectPrev)
        // } modified by lssanh 2012.09.18 for No35 car DQA#79
        {
		// commented by ravikanth 25-04-13 for ITS 0175705
//            if (m_isListView)
//                m_pIMedia->Next(1); // added by Michael.Kim 20130410 for New UX
//            else
                m_pIMedia->Prev(1,1);
        }
        else
        {
            // { add by changjin 2012.08.09 : for CR 12479
            if( m_enableErrorPopupState == ERROR_POPUP_READY )
                m_enableErrorPopupState = ERROR_POPUP_ENABLE;
            // } add by changjin

            // { modified by kihyung 2012.11.22
            // int nCurrTime = m_pIMedia->GetCurrentMediaPosition();
			// commented by ravikanth 25-04-13 for ITS 0175705
//            if (m_isListView)
//                m_pIMedia->Next(1); // added by Michael.Kim 20130410 for New UX
//            else
                m_pIMedia->Prev(1);

            // { removed by kihyung 2013.1.4 for ISV 68141
            /*
            // { add by lssanh 2012.09.10 for CR9362
            if (m_discType == MEDIA_SOURCE_MP3 && nCurrTime < 3000)
            {
                emit showPopupLoadingData();
            }
            // } add by lssanh 2012.09.10 for CR9362
            */
            // { removed by kihyung 2013.1.4 for ISV 68141
            // } modified by kihyung
    
        }
    }
    //{ modified by yongkyun.lee 2013-08-27 for : : ISV 89645  : ITS 181278 delete
    // //{ modified by yongkyun.lee 2013-07-30 for : ITS 181278
    // QEventLoop* loop = new QEventLoop(this);
    // loop->processEvents(QEventLoop::ExcludeUserInputEvents, 100); // added by Sergey 24.07.2013
    // //} modified by yongkyun.lee 2013-07-30 
    //} modified by yongkyun.lee 2013-08-27 
    //emit positionCleared(); // added by wspark 2013.03.30 for ISV 77359 // deleted by wspark 2013.04.09

    // { added by kihyung 2012.09.04 for 3PDMS 0136421 
    // emit positionChanged(0); // removed by kihyung 2013.3.28 for continuos next input.
    // } added by kihyung
}

void
AppMediaPlayer_Controller_Audio::setPlayingPosition( int seconds )
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    MP_LOG << seconds << LOG_ENDL;

    // { modified by richard 2012.10.25 for navi slowness fix
    //if (m_pIMedia && (m_pIMedia->GetCurrentMediaDuration() > seconds * 1000))
    if (m_pIMedia && m_pIMedia->GetCurrentMediaDuration() > seconds)
    {
        MP_LOG << "set position =" << seconds << LOG_ENDL;
        //added by edo.lee 2013.05.21
        if(device == MP::IPOD1 || device == MP::IPOD2)
        {
            GetEngineMain().setUnMuteState(true);

            //{added by junam 2013.08.14 time ceiling
            if(seconds % 1000)
                seconds = seconds - (seconds % 1000) + 1000;
            //}added by junam
        }
		//{ added by edo.lee 2013.09.02 drag mute
		/*else if(device ==MP::JUKEBOX || device ==MP::USB1 ||device ==MP::USB2)
		{
		    GetEngineMain().setMuteForSeek(true);
		}*/
		//} added by edo.lee 2013.09.02
        //m_pIMedia->AudioDragStart();//remove by edo.lee 2013.06.29
        
        //added by edo.lee 2013.05.21
        //m_pIMedia->SetCurrentMediaPosition( seconds * 1000);
        GetEngineMain().setIsTermOnTrackChange(true);//added by edo.lee 2013.10.07 ITS 186267
        m_pIMedia->SetCurrentMediaPosition( seconds );
        m_history[device].Position = seconds; // added by wspark 2013.03.19 for ISV 75982
        //{added by edo.lee 2013.04.25
        if(device == MP::IPOD1 || device == MP::IPOD2) {
            m_iPodChangingPosition = seconds;
        }
        //}added by edo.lee
        if(isPaused()) //added by edo.lee 2013.06.01
            Play();
    }
    // } modified by richard 2012.10.25 for navi slowness fix
}
// { modified by eugeny.novikov 2012.12.04 CR 16199


// { added by cychoi 2013.06.02 for sound noise when dragged progress bar
void
AppMediaPlayer_Controller_Audio::setDiscMuteState(bool bDiscMuteState)
{
    MP_LOG << LOG_ENDL;

    // { modified by cychoi 2013.06.28 for preventing twice mute function call
    if(bDiscMuteState)
    {
        switch(m_eDiscMuteState)
        {
        case DISC_UNMUTE:
            GetEngineMain().setMuteForSeek(true);
        case DISC_UNMUTE_READY:
            m_eDiscMuteState = DISC_MUTE;
            break;
        case DISC_MUTE:
        default:
            // Do nothing
            break;
        }
    }
    // } modified by cychoi 2013.06.28
    else
    {
        //GetEngineMain().setUnMuteState(true);
        m_eDiscMuteState = DISC_UNMUTE_READY;
    }
}
// } added by cychoi 2013.06.02

//added by edo.lee 2013.06.13 for progress bar drag mute
void 
AppMediaPlayer_Controller_Audio::setDragMuteState(bool isMute)
{
    if(m_pIMedia) 
    {
    	MP_LOG <<"Start : "<<isMute << LOG_ENDL;
        
    	if(isMute)
    	{
    		m_pIMedia->AudioDragStart(); //modified by edo.lee 2013.06.26
    	}
        else
    	{
    		m_pIMedia->AudioDragEnd();
    	}
    }
}
//added by edo.lee 2013.06.13

bool
AppMediaPlayer_Controller_Audio::isPaused() const
{
    // PLAYER_STOP should be removed
    return getDevice() == MP::BLUETOOTH ? !m_bIsBTPlaying :	//modified by hyochang.ryu 20131109 for ITS 207604	//false : 			//added by hyochang.ryu 20130709
    ( m_PlayerState == PLAYER_PAUSE || m_PlayerState == PLAYER_STOP );
}

//{ added by cychoi 2013.10.10 for ITS 194757 Pause on FF/REW
bool AppMediaPlayer_Controller_Audio::IsDiscPlaying() const
{
    bool bPlayed = false;

    switch(m_PlayerState)
    {
    case PLAYER_PLAY:
    case PLAYER_FASTFORWARD:
    case PLAYER_REWIND:
    case PLAYER_SEARCH:
    case PLAYER_FASTFORWARD_X4:
    case PLAYER_FASTFORWARD_X16:
    case PLAYER_FASTFORWARD_X20:
    case PLAYER_REWIND_X4:
    case PLAYER_REWIND_X16:
    case PLAYER_REWIND_X20:
        bPlayed = true;
        break;
    default: // make compiler happy
        break;
    }

    return bPlayed;
}
//} modified by cychoi 2013.10.10
// { added by wonseok.heo for ITS 220389 2014.01.16
bool AppMediaPlayer_Controller_Audio::IsDiscNormalPlaying() const
{
    bool bPlayed = false;

    switch(m_PlayerState)
    {
    case PLAYER_PLAY:
    case PLAYER_SEARCH:
        bPlayed = true;
        break;
    default: // make compiler happy
        break;
    }

    return bPlayed;
}
// } added by wonseok.heo for ITS 220389 2014.01.16
bool AppMediaPlayer_Controller_Audio::IsPlaying() const
{
    //{ modified by cychoi 2013.07.29 for ITS 181865
    if(getDevice() == MP::DISC)
    {
        // Users should think PLAYER_SEARCH as playing status.
        // 1. Controller's Command -> Search -> Play/FF/REW 2. Random Play -> Search -> Random Play
        //{ modified by cychoi 2013.10.10 for ITS 194757 Pause on FF/REW
        return IsDiscPlaying();
        //return (m_PlayerState == PLAYER_PLAY || m_PlayerState == PLAYER_SEARCH);
        //} modified by cychoi 2013.10.10
    }
    else
    {
        //{ modified by hyochang.ryu 20130517
        return m_device == MP::BLUETOOTH ? m_bIsBTPlaying : (m_PlayerState == PLAYER_PLAY);	//added by hyochang.ryu 20131002
        //return getDevice() == MP::BLUETOOTH ? true : (m_PlayerState == PLAYER_PLAY);
        //} modified by hyochang.ryu 20130517
    }
    //} modified by cychoi 2013.07.29
}

bool
AppMediaPlayer_Controller_Audio::isStopped() const
{
    return ( m_PlayerState == PLAYER_STOP );
}

void
AppMediaPlayer_Controller_Audio::toggleRepeatMode()
{
    MP_LOG << LOG_ENDL;
    int repeatMode = -1;

    // Folder content:  RepeatAll -> RepeatFolder -> RepeatOne -> RepeatAll...
    // Default content: RepeatAll -> RepeatOne -> RepeatAll...
    if (m_repeatPlayMode == REPEATALL)
    {
        //{ modified by cychoi 2013.11.07 for SmokeTest Repeat Folder on Toggle in CDDA
        if(getDevice() == MP::DISC && m_discType == MEDIA_SOURCE_CDDA)
        {
            repeatMode = REPEATFILE;
        }
        else
        {
            repeatMode = (m_history[getDevice()].ContentPlayed == MP::FOLDER) ? REPEATFOLDER
                                                                           : REPEATFILE;
        }
        //} modified by cychoi 2013.11.07
    }
    else
    {
        repeatMode = (m_repeatPlayMode == REPEATFOLDER) ? REPEATFILE : REPEATALL;
    }
    setRepeatRandomMode(repeatMode, -1);
}

void
AppMediaPlayer_Controller_Audio::toggleRandomMode()
{
    MP_LOG << LOG_ENDL;
    setRepeatRandomMode(-1, (m_randomPlayMode == RANDOMOFF) ? RANDOMFILE : RANDOMOFF);
}

void
AppMediaPlayer_Controller_Audio::resetRepeatOne()
{
    if (m_repeatPlayMode == REPEATFILE)
    {
        MP_LOG<<"Reset RepeatOne state" << LOG_ENDL;
        setRepeatRandomMode(REPEATALL, -1);
    }
}

// added by kihyung.lee 2013.08.31 for ITS 0187529
void 
AppMediaPlayer_Controller_Audio::updateRepeatRandomIcon(int nRepeatMode, int nRandomMode)
{
    int repeatIconState = (nRepeatMode == REPEATFILE) ? 2 : (nRepeatMode == REPEATFOLDER) ? 3 : 1;
    int randomIconState = (nRandomMode == RANDOMOFF)  ? 0 : 1;
    
    MP_LOG << "repeatIcon" << repeatIconState << "randomIcon" << randomIconState << LOG_ENDL;
    
    emit setRandomRepeatIcons(repeatIconState, randomIconState);
}

void
AppMediaPlayer_Controller_Audio::setRepeatRandomMode(int nRepeatState, int nRandomState)
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    MP_HIGH << "nRepeatState" << nRepeatState << "nRandomState" << nRandomState << LOG_ENDL;

    if (!m_pIMedia)
        return;

    int nSDPRandom = (nRandomState == RANDOMOFF) ? STATUS_OFF : STATUS_ON;
    int nSDPRepeat = (nRepeatState == REPEATFILE) ? STATUS_ON : STATUS_OFF;

    if (m_repeatPlayMode == nRepeatState && m_randomPlayMode == nRandomState) // modified by Dmitry Bykov 11.04.2012 for ISV79685
    {
        MP_LOG << "Equal to current state, return" << LOG_ENDL;
        
        if(m_pSharedAudioData->GetStatusDateByIndex(DATA_TYPE_RANDOM) != nSDPRandom) {
            emit setStatusDataByIndex(device, DATA_TYPE_RANDOM, nSDPRandom);
            MP_MEDIUM << "DATA_TYPE_RANDOM " << nSDPRandom << LOG_ENDL;
        }
        if(m_pSharedAudioData->GetStatusDateByIndex(DATA_TYPE_REPEAT) != nSDPRepeat) {
            emit setStatusDataByIndex(device, DATA_TYPE_REPEAT, nSDPRepeat); 
            MP_MEDIUM << "DATA_TYPE_REPEAT " << nSDPRepeat << LOG_ENDL;
        }
        
        return;
    }

    bool isRepeatChanged = false;
    bool isRandomChanged = false;

    if (m_repeatPlayMode != nRepeatState && nRepeatState != -1)
    {
        m_repeatPlayMode = nRepeatState;
        isRepeatChanged = true;

        if (m_repeatPlayMode == REPEATFILE && m_randomPlayMode != RANDOMOFF)
        {
            m_randomPlayMode = RANDOMOFF;
            isRandomChanged = true;
        }
    }

    if (m_randomPlayMode != nRandomState && nRandomState != -1)
    {
        m_randomPlayMode = nRandomState;
        isRandomChanged = true;

        if (m_randomPlayMode != RANDOMOFF && m_repeatPlayMode == REPEATFILE)
        {
            m_repeatPlayMode = REPEATALL;
            isRepeatChanged = true;
        }
    }
    
    MP_LOG << "isRepeatChanged" << isRepeatChanged << "isRandomChanged" << isRandomChanged << LOG_ENDL;
    
    if (isRepeatChanged || isRandomChanged)
    {
        MP_LOG << "m_repeatPlayMode" << m_repeatPlayMode << "m_randomPlayMode" << m_randomPlayMode << LOG_ENDL;

        if (isRepeatChanged)
        {
            MP_MEDIUM << "Invoke m_pIMedia->RepeatPlay()" << m_repeatPlayMode << LOG_ENDL;
            m_pIMedia->RepeatPlay(m_repeatPlayMode);
        }

        if (isRandomChanged)
        {
            MP_MEDIUM << "Invoke m_pIMedia->RandomPlay()" << m_randomPlayMode << LOG_ENDL;
            //{ modified by shkim 2013.11.13 for CDDA Scan Off failure Smoke Test Issue
            if(getDevice() == MP::DISC && m_discType == MEDIA_SOURCE_CDDA)
            {
                CDDAStatusResponse prevCDStatus, newCDStatus;
                m_pIAudio->GetCDStatus(&prevCDStatus);
                m_pIMedia->RandomPlay(m_randomPlayMode);
                m_pIAudio->GetCDStatus(&newCDStatus);
                if((m_randomPlayMode == RANDOMFILE) &&
                   (prevCDStatus.scanModeStatus == 0x01 || prevCDStatus.scanModeStatus == 0x04) &&
                   (newCDStatus.scanModeStatus == 0xFF))
                {
                    emit stopScan();
                }
            }
            else
            {
                m_pIMedia->RandomPlay(m_randomPlayMode);
            }
            //} modified by shkim 2013.11.13
        }

        // added ITS 234802 234804
        if((device == MP::IPOD1 || device == MP::IPOD2) && GetEngineMain().GetMuteState())
        {
            //MP_LOG<< "IPOD setting m_WillUnMute true" <<LOG_ENDL;
            m_WillUnMute = true;
        }

        nSDPRandom = (m_randomPlayMode == RANDOMOFF) ? STATUS_OFF : STATUS_ON;
        nSDPRepeat = (m_repeatPlayMode == REPEATFILE) ? STATUS_ON : STATUS_OFF;
        if(m_pSharedAudioData->GetStatusDateByIndex(DATA_TYPE_RANDOM) != nSDPRandom) {
            emit setStatusDataByIndex(device, DATA_TYPE_RANDOM, nSDPRandom);
        }
        if(m_pSharedAudioData->GetStatusDateByIndex(DATA_TYPE_REPEAT) != nSDPRepeat) {
            emit setStatusDataByIndex(device, DATA_TYPE_REPEAT, nSDPRepeat); 
        }

        updateRepeatRandomIcon(m_repeatPlayMode, m_randomPlayMode); // modified by kihyung 2013.08.31 for ITS 0187529
        emit playingModeChanged(m_randomPlayMode, m_repeatPlayMode, m_scanMode, m_NoOSD);
    }

    if(device != MP::DISC)
    {
        // { modified by kihyung 2013.06.28
        bool bChanged = false;
        if(m_history[device].repeatPlayMode != m_repeatPlayMode) {
            bChanged = true;
            m_history[device].repeatPlayMode = m_repeatPlayMode;
        }
        if(m_history[device].randomPlayMode != m_randomPlayMode) {
            bChanged = true;
            m_history[device].randomPlayMode = m_randomPlayMode;
        }
        
        if(bChanged == true) {
            AppMediaPlayer_SettingsStorage::GetInstance()->SaveAudioSettings(this);
        }
        // } modified by kihyung 2013.06.28        
    }	

	GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->updatePlayingMode((OSD_DEVICE)getOSDevice(device),
		 m_randomPlayMode, m_repeatPlayMode, m_scanMode);
}

void
AppMediaPlayer_Controller_Audio::setScanMode(int scanMode)
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    MP_HIGH << scanMode << LOG_ENDL;
    //{ added by yongkyun.lee 20130531 for : ITS 167819
    int fileIndex = -1;
    int fileCount = -1;
    //} added by yongkyun.lee 20130531 
    MP_MEDIUM << scanMode << LOG_ENDL;

    if (!m_pIMedia)
        return;

    if (m_scanMode == scanMode)
    {
        MP_LOG << "Equal to current scan mode, return" << LOG_ENDL;
        return;
    }

    //{ added by yongkyun.lee 20130531 for : ITS 167819
    if(device == MP::DISC)
    {
        if(m_discType == MEDIA_SOURCE_CDDA)
            m_pIMedia->GetCurrentFileFolderIndex(&fileIndex, &fileCount);
        else if(m_discType == MEDIA_SOURCE_MP3)
        {
            if (scanMode == SCANALL )
            {
                TotalFileFolderCount count = {0, 0, 0};
                m_pIAudio->GetTotalFileFolder(&count);
                fileCount = count.totalFiles;
            }// { modified by wonseok.heo for ITS 191295 2013.09.26
            else if(scanMode == SCANFILE ){
                m_pIMedia->GetCurrentFileFolderIndex(&fileIndex, &fileCount);
                m_discScanPlayingMode = SCANFILE; //added by wonseok.heo for ITS 191295 2013.09.26
            }
            else{
                fileCount = 0xff;
            }// } modified by wonseok.heo for ITS 191295 2013.09.26
        }
        //MP_LOG << "fileCount = " << fileCount << LOG_ENDL;

        if( fileCount <=1)
            return;
    }
    //} added by yongkyun.lee 20130531 
    
    if (scanMode == SCANOFF)
    {
        m_pIMedia->Scan(SCANOFF);
        // removed by Dmitry 23.04.13 for ISV81429
        emit stopScan(); // added by kihyung 2013.07.26 for ITS 0181817 
        m_scanMode = scanMode;//{ modified by yongkyun.lee 2013-07-26 for : smoke test  cd scan osd 
    }
    else
    {
    	GetEngineMain().GetNotifier()->setIsScanMode(true);//added by edo.lee 2013.04.05
        // m_currentRepeatmode = m_repeatPlayMode; // added by eunhye 2013.03.30 for ISV 78066 // removed by kihyung 2013.05.27 for ITS 148293
        emit setStatusDataByIndex(device, DATA_TYPE_RANDOM, STATUS_OFF);
        emit setStatusDataByIndex(device, DATA_TYPE_REPEAT, STATUS_OFF);

        //Added by tan, to unmute when scan is performed.
        if(device == MP::IPOD1 || device == MP::IPOD2)
        {
            if(GetEngineMain().GetMuteState())
            {
                MP_LOG<< "IPOD setting m_WillUnMute true"<<LOG_ENDL ;
                m_WillUnMute = true;
            }
            MP_LOG<< "IPOD sendVRCancelSignal()"<<LOG_ENDL ;
            GetEngineMain().sendVRCancelSignal();
        }


        //{ modified by yongkyun.lee 20130426 for :  scan cancel
        //m_pIMedia->Scan(m_scanMode);
        m_pIMedia->Scan(scanMode);
        //} modified by yongkyun.lee 20130426 

        if (isPaused())
        {
            //{added by junam 2013.06.25 
            if(device == MP::IPOD1 || device == MP::IPOD2)
                m_pIMedia->Play();
            else //}added by junam
                Play();
        }
        m_scanMode = scanMode; //added by wonseok.heo 2013.05.08
        emit startScan();
        //{ modified by yongkyun.lee 20130426 for :  scan cancel
        //emit playingModeChanged(RANDOMOFF, REPEATOFF, m_scanMode, m_NoOSD);
        emit playingModeChanged(RANDOMOFF, REPEATOFF, scanMode, m_NoOSD);        
        //} modified by yongkyun.lee 20130426 
        //{ modified by yongkyun.lee 2013-09-23 for : ISV 91169
        if( device == MP::DISC )
            return;
        //} modified by yongkyun.lee 2013-09-23 
        
    }

    // Modified by jonghwan.cho@lge.com 2013.10.22 for ISV 91170
    if(scanMode != SCANOFF)
    {
        GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->updatePlayingMode((OSD_DEVICE)getOSDevice(device),
                        RANDOMOFF, REPEATOFF, m_scanMode);
    }
}

// { modified by eugene.seo 2012.10.23 for Function_USB_0190
void
AppMediaPlayer_Controller_Audio::setListMode(bool listMode)
{
    // { added by wonseok.heo 2013.08.15 for ITS 184020
    if(listMode==false && m_discType == MEDIA_SOURCE_MP3){
        m_pTreeModel->listExit();
    }
    // } added by wonseok.heo 2013.08.15 for ITS 184020

    if(listMode)  m_iPodSortingOrder = -1;//added by junam 2013.08.21 for iPod sorting

    m_isListView = listMode;
}
// } modified by eugene.seo 2012.10.23 for Function_USB_0190

int // added by eugene.seo 2013.02.07 for unsupported audio play in each repeat mode
AppMediaPlayer_Controller_Audio::getCurrentCategory(int deviceID)
{
    //if(deviceID < MP::JUKEBOX || deviceID >= MP::UNDEFINED)
    if(deviceID < MP::JUKEBOX || deviceID >= MP::ALL_DEVICES)   // modified by sungha.choi 2013.07.15 for static test
        return MP::DEFAULT;
    else
        return m_history[deviceID].ContentPlayed;
}

//by sam
void
AppMediaPlayer_Controller_Audio::saveCategoryTab(QString currentCategoryTab)
{
/*
    switch(m_history[device].ContentPlayed)
    {
    case MP::FOLDER:
        m_history[device].listReqData.tabId = "Folder";
        break;
    case MP::SONG:
        m_history[device].listReqData.tabId = "Song";
        break;
    case MP::ALBUM:
        m_history[device].listReqData.tabId = "Album";
        break;
    case MP::ARTIST:
        m_history[device].listReqData.tabId = "Artist";
        break;
    case MP::GENRE:
        m_history[device].listReqData.tabId = "Genre";
        break;
    }
*/
    MP_LOG<<"category: "<<currentCategoryTab<<" cur device:" << getDevice() << LOG_ENDL;

    if(getDevice() != MP::UNDEFINED)
        m_history[getDevice()].listReqData.tabId = currentCategoryTab;
    SaveAudioData();
}

QString
AppMediaPlayer_Controller_Audio::getCategoryTab(QString currentCategory)
{
    //{changed by junam 2013.12.20 for LIST_ENTRY_POINT
    MP::DEVICE_T device = getDevice();

    MP_LOG << "currentCategory = " << currentCategory << ", device = " << device << LOG_ENDL;

    if(currentCategory == "jukebox" && device == MP::JUKEBOX)
    {
        MP_MEDIUM<<"****sam*** JUKEBOX category: "<<m_history[MP::JUKEBOX].listReqData.tabId << LOG_ENDL;
        return m_history[MP::JUKEBOX].listReqData.tabId;
    }
    else if(currentCategory == "usb" && (device == MP::USB1 || device == MP::USB2))
    {
        MP_MEDIUM<<"****sam*** USB category: "<<m_history[device].listReqData.tabId << LOG_ENDL;
        return m_history[device].listReqData.tabId;
    }//}changed by junam
    //{ added 2013.10.30
    else if(currentCategory == "ipod")
    {
        //Tan 2013.12.11 EU_214076 edit mode crash, make sure getDevice is not UNDEFINED
        if(getDevice() != MP::UNDEFINED)
        {
            MP_MEDIUM<<"****sam*** IPOD category: "<<m_history[getDevice()].listReqData.tabId << LOG_ENDL;
            return m_history[getDevice()].listReqData.tabId;
        }
        else
        {
            MP_HIGH<<"getCategoryTab when getDevice is MP::UNDEFINED" << LOG_ENDL;
            return "";
        }
    }
    //} added 2013.10.30
    MP_MEDIUM<<"****sam*** category: ERROR..." << LOG_ENDL;
    return "";
}

void
AppMediaPlayer_Controller_Audio::clearPlayMode()
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    MP_LOG << LOG_ENDL;
	//{modified by edo.lee 2013.09.07 ITS 0185752 
    //setRepeatRandomMode(REPEATALL, RANDOMOFF); //deleted by yungi 2013.03.28 for ITS 160237
    // { modified by cychoi 2013.05.26 for Cluster IOT #125
    if(device == MP::DISC)
    {
        if(m_discType < MEDIA_SOURCE_MAX)
         GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->updatePlayingMode((OSD_DEVICE)getOSDevice( AudioUtils::DeviceType(m_discType)),
            m_randomPlayMode, m_repeatPlayMode, SCANOFF);
            //GetEngineMain().GetNotifier()->resetScanMode(m_discType);
    }
    else
    {
         GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->updatePlayingMode((OSD_DEVICE)getOSDevice(device),
            m_randomPlayMode, m_repeatPlayMode, SCANOFF);
       // GetEngineMain().GetNotifier()->resetScanMode( AudioUtils::DeviceType(device));//added by edo.lee 2013.05.16
    }
    //GetEngineMain().GetNotifier()->resetScanMode( AudioUtils::DeviceType(m_device));//added by edo.lee 2013.05.16
    // } modified by cychoi 2013.05.26
    //} modified by edo.lee 2013.09.07  ITS 0185752 
    setScanMode(SCANOFF);
}
// } modified by eugeny.novikov 2012.12.04


// modified by kihyung 2013.08.28 for ITS 0186131
void 
AppMediaPlayer_Controller_Audio::SetDefaultPlayMode(MP::PLAYBACK_CONTENT_T category, bool bSameFile)
{
    MP_LOG << category << bSameFile << LOG_ENDL;
    
    if(getRepeatMode() == REPEATFILE && bSameFile == false) {
        if(m_pIMedia) m_pIMedia->RepeatPlay(REPEATALL);
    }
    else if(getRepeatMode() == REPEATFOLDER && category != MP::FOLDER) {
        if(m_pIMedia) m_pIMedia->RepeatPlay(REPEATALL);
    }
}

void
AppMediaPlayer_Controller_Audio::HandleMediaStatusChanged(int mediaStatus)
{
    MP_LOG << "mediaStatus =" << mediaStatus << LOG_ENDL;

    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;

    MP::DEVICE_T source = GetSignalSource();

    switch (mediaStatus)
    {
        case PLAYER_EndOfMedia:
        {
            m_history[device].Position = 0;

            if (m_pIMedia == NULL || source != device)
                break;

            m_isForwardDirection = true; // added by wspark 2012.09.10 for CR12707

            // removed by kihyung 2013.7.25  
            break;
        }
        default:
        {
            break;
        }
    }
}

// { added by junam 2012.10.08 for CR14264
void
AppMediaPlayer_Controller_Audio::onMediaStatusChanged(int mediaStatus)
{
    MP_HIGH << "mediaStatus =" << mediaStatus  << LOG_ENDL;

    MP::DEVICE_T source = GetSignalSource();

    if (m_pIMedia == NULL || source != getDevice())
        return;

    switch (mediaStatus)
    {
        case PLAYER_EndOfMedia:
        {
            if (m_repeatPlayMode == REPEATFILE)// &&  // {  deleted by jungae 2012.12.13 for SANITY_CM_AI027
//               (m_isLongPressed == 1 || m_isLongPressed == -1)) // } deleted by jungae
            {
                setRepeatRandomMode(REPEATALL, RANDOMOFF);
            }
            break;
        }
        default:
            break;
    }
}
// } added by junam

void
AppMediaPlayer_Controller_Audio::onNormalPlay()
{
    ((AppMediaPlayer_EngineAudio*)GetEngineMain().GetEngine(APP_MEDIA_MODE_AUDIO))->setRewReachedFirst(true);
    // {added by Michael.Kim 2013.06.17 for repeat cancel at head of first track
    if(GetMediaMode() == MP::DISC && (m_discType == MEDIA_SOURCE_MP3 || m_discType == MEDIA_SOURCE_CDDA)) 
    {
        GetEngineMain().setUnMuteState(true);//{ modified by yongkyun.lee 2013-08-17 for : ITS 184914

        if((getRepeatMode() == REPEATFILE))
            setRepeatRandomMode(REPEATALL, -1);
        // { modified by cychoi 2013.11.07 for SmokeTest DISC cancel REW
        if(isLongPress())
        {
            if(isLongPress()== -1 ) //getPlayState() >= PLAYER_REWIND_X4 || getPlayState() == PLAYER_REWIND)
                emit changeFastFowardRewind(false , AppEngineQMLConstants::KEY_STATUS_CANCELED ,AppEngineQMLConstants::JOG_LEFT ); // modified by sangmin.seol 2014.01.15 SMOKE cancel REW when at head of first track
            else if(isLongPress()== 1)
                emit changeFastFowardRewind(false , AppEngineQMLConstants::KEY_STATUS_CANCELED ,AppEngineQMLConstants::JOG_RIGHT ); // modified by sangmin.seol 2014.01.15 SMOKE cancel REW when at head of first track
        }
        // } modified by cychoi 2013.11.07
    } else
    {
        // { modified by sangmin.seol 2014.01.15 SMOKE cancel REW when at head of first track
        if(isLongPress())
        {
            if(isLongPress()== -1 ) //getPlayState() >= PLAYER_REWIND_X4 || getPlayState() == PLAYER_REWIND)
                emit changeFastFowardRewind(false , AppEngineQMLConstants::KEY_STATUS_CANCELED ,AppEngineQMLConstants::JOG_LEFT );
            else if(isLongPress()== 1)
                emit changeFastFowardRewind(false , AppEngineQMLConstants::KEY_STATUS_CANCELED ,AppEngineQMLConstants::JOG_RIGHT );
        }
        // } modified by sangmin.seol 2014.01.15 SMOKE cancel REW when at head of first track
    }
    // }added by Michael.Kim 2013.06.17 for repeat cancel at head of first track
} // added by Sergey 28.05.2013

void
AppMediaPlayer_Controller_Audio::onStateChanged(int state)
{
    MP::DEVICE_T device = getDevice();

    //Suryanto Tan 2014.03.17 Issue: Ipod failed to resume play after power off.
    //The onStateChanged was ignored when device is UNDEFINED.
    //if(device >= MP::ALL_DEVICES) return;
    if(device >= MP::ALL_DEVICES)
    {
        MP::DEVICE_T source = GetSignalSource();
        PLAYER_STATE newState = (PLAYER_STATE)state;
        if(source == MP::IPOD1 || source == MP::IPOD2)
        {
            MP_HIGH<<"Save newState on PowerOff: "<< newState << LOG_ENDL;
            m_history[source].State = newState;
            if(newState == PLAYER_PLAY)
            {
                //TAN 2014.09.01 ITS 247200
                //We are already in other mode, however, the iPod report that
                //its state is Player play. thus, we should pause it.
                CIpodController* controller = GetIPodController(source);
                if (controller)
                {
                    MP_HIGH<<"device is taboff, should be paused" << LOG_ENDL;
                    controller->Pause();
                }
            }
        }
        return;
    }
    
    PLAYER_STATE newState = (PLAYER_STATE)state;
    MP::DEVICE_T source = GetSignalSource();
    bool bStateChanged = (m_history[source].State == newState) ? false : true; //added by junam 2013.09.06 for ITS_KOR_188389
    PLAYER_STATE oldState = m_history[source].State; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error 

    MP_LOG << DeviceName(source) << ", " << m_history[source].State << "->" << newState << LOG_ENDL;
    //MP_LOG << m_history[source].State << "->" << newState << LOG_ENDL;

    //if(IsNoSourceByDeviceid(source)) return;//deleted by edo.lee 2012.11.28  for ISV 64487 
    m_history[source].State = newState;

    // iPod autoplay before 'authorization' signal:
    if ((source == MP::IPOD1 || source == MP::IPOD2) && newState == PLAYER_PLAY && bStateChanged) //add bStateChanged by junam 2013.09.06 for ITS_KOR_188389
    {
        CIpodController* controller = GetIPodController(source);
        if (controller)
        {
            if (device != source)
            {
                controller->Pause();
            }

            /*
            //added by edo.lee 2013.06.14 for Mute
            if(m_pMuteTimer->isActive())
                m_pMuteTimer->stop();
            m_pMuteTimer->start();


            GetEngineMain().setUnMuteState(true); //added by junam 2013.08.25 for iPod unmute
            */
        }

        //added by jhcho 2012.02.20
        switch( source ) {
        case MP::IPOD1 :
            emit iPodPlaying( eUSB_PORT1, "", true );
            break;
        case MP::IPOD2 :
            emit iPodPlaying( eUSB_PORT2, "", true );
            break;
        default :
            MP_MEDIUM << "The source of this event is invalid" << LOG_ENDL;
            break;
        }
    }
    //added by edo.lee 2013.05.23
    if(newState == PLAYER_PLAY)
    {
        // { removed by kihyung 2013.06.09
        /*
  if(m_pMuteTimer->isActive())
      m_pMuteTimer->stop();
  m_pMuteTimer->start();
  */
        // } removed by kihyung

        // { added by cychoi 2013.06.02 for sound noise when dragged progress bar
        if(m_eDiscMuteState == DISC_UNMUTE_READY && device == MP::DISC)
        {
            GetEngineMain().setUnMuteState(true);
            m_eDiscMuteState = DISC_UNMUTE;
        }
        //}  added by cychoi 2013.06.02
    }

    if (source == device && newState != m_PlayerState)
    {
        // { modified by kihyung 2013.1.4 for ISV 68141
        if(device == MP::DISC && m_discType == MEDIA_SOURCE_MP3) 
        {
            //if (newState == PLAYER_SEARCH && m_pIAudio) 
            //{
                //emit showPopupLoadingData(); // deleted by lssanh 2013.04.07 delete mp3 popup loading
            //}

            // { modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error 
            if (m_pIAudio && m_eID3TagState != ID3TAG_RECEIVED) // modified by cychoi 2013.12.17 for ITS 216033
            {
                switch(newState)
                {
                case PLAYER_PLAY:
                case PLAYER_FASTFORWARD:
                case PLAYER_REWIND:
                case PLAYER_SCAN: // modified by cychoi 2013.10.03 for SmokeTest Infinite ID3 Tag Req error
                //case PLAYER_FASTFORWARD_X4:
                //case PLAYER_FASTFORWARD_X16:
                //case PLAYER_FASTFORWARD_X20:
                //case PLAYER_REWIND_X4:
                //case PLAYER_REWIND_X16:
                //case PLAYER_REWIND_X20:
                    if(m_eID3TagState == ID3TAG_NONE) // modified by cychoi 2013.12.17 for ITS 216033
                    {
                        m_pIAudio->RequestAllTagInfo();
                        m_eID3TagState = ID3TAG_REQUESTED;
                    }
                    break;
                // { added by cychoi 2013.12.17 for ITS 216033
                case PLAYER_PAUSE:
                    if(m_eID3TagState != ID3TAG_RECEIVED)
                    {
                        m_pIAudio->RequestAllTagInfo();
                        m_eID3TagState = ID3TAG_REQUESTED;
                    }
                    break;
                // } added by cychoi 2013.12.17
                default: // make compiler happy
                    break;
                }
            }
            // } modified by cychoi 2013.09.22
        }
        // } modified by kihyung 2013.1.4

        m_PlayerState = newState;

        // { added by eugeny.novikov 2012.10.25 for CR 14047
        if (m_PlayerState == PLAYER_PLAY)
        {
            MP_MEDIUM << "Playback started, enable mode area" << LOG_ENDL;
			//remove by edo.lee 2013.05.23
			//if(m_pMuteTimer->isActive())
			//	m_pMuteTimer->stop();
			//m_pMuteTimer->start();

            emit enableModeArea();
            //setIsLoadingScreen(false); // removed by junam 2013.12.10 for code clean
            //m_bFirstEntry[device] = false;//removed by junam 2014.01.17 deprecated code
            emit UpdateStateTrackOSD(false);//added by edo.lee 2013.06.22

           //deleted by yongkyun.lee 2013-11-22 for : EU - Tune   
        }
        // } added by eugeny.novikov

        // { modified by kihyung 2012.08.25 for Issue Fix(In jukebox unknown info is displayed if DVDAudio exist in deck).
        // { added by junggil 2012.08.17 for CR11646
        if (device == MP::DISC && m_discType == MEDIA_SOURCE_DVDAUDIO /*|| m_discType == MEDIA_SOURCE_CDDA)*/ && newState == PLAYER_PLAY) // modified by Michael.Kim 2013.04.29 for ISV #81062
        {
            GetDiscInfo();

            m_deckInfo.coverarturl = ""/*DEFALUT_COVER*/; //modified by Michael.Kim 2013.04.09 for New UX
            m_deckInfo.Artist = "Unknown";
            m_deckInfo.Title = (GetCurrentFileName() == "-1") ? "1" : GetCurrentFileName();
            m_deckInfo.Album = "Unknown";

            onMediaInfoChanged(&m_deckInfo);
        }

        /*
        else if (m_device == MP::DISC && m_discType == MEDIA_SOURCE_CDDA) // added by Michael.Kim 2013.04.26 for ISV #81062
        {
             //GetDiscInfo(); deleted by Michael.Kim 2013.05.18 for ISV Issue #83202
             //onMediaInfoChanged(&m_deckInfo);
        }
        */ //deleted by Michael.Kim 2013.05.22 for ITS Issue #0168209

        // } added by junggil
        // } modified by kihyung
        
        //{ added by yongkyun.lee 20130313 for : NOCR- Play/Pause Icon
        // if (m_bRestoring)
        // {
        //     MP_LOG << "Restoring in progress..." << LOG_ENDL;
        //     //return; // deleted by changjin 2012.08.31 : Disc cue icon update issue
        //     //{ added by changjin 2012.08.31
        //     if ( (m_device == MP::DISC) && (newState == PLAYER_PLAY) ) {
        //         MP_LOG << "Disc CUE icon update" << LOG_ENDL;
        //     }
        //     else {
        //         return;
        //     }
        //     // } added by changjin 2012.08.31
        // }
        //} added by yongkyun.lee 20130313 

        switch (newState)
        {
        case PLAYER_STOP:
           emit setStatusDataByIndex( device, DATA_TYPE_PLAY_STATUS, PLAY_STATUS_PAUSE );
           if(!m_isLongPressed && !m_tuneState.m_nTuneStart) //added "m_nTuneStart" by junam 2012.10.5 for CR14119 
               emit playbackStopped();
           break;

        case PLAYER_PLAY:
            // { removed by junam 2013.10.29 for ITS_EU_196049
            //if(m_isLongPressed)
            //{
            //    emit clearSeekMode();
            //}
            // } removed by junam

           emit setStatusDataByIndex( device, DATA_TYPE_PLAY_STATUS, PLAY_STATUS_PLAY );
           GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setMaintainPause(false);
           if(!m_tuneState.m_nTuneStart) //added by junam 2012.10.5 for CR14119 
           {
               // { added by wonseok.heo for ITS 212895 2013.12.05
               // { modified by wonseok.heo for smoke Test countInfo issue 2013.12.07
               emit playbackStarted();
               // } modified by wonseok.heo for smoke Test countInfo issue 2013.12.07
               // } added by wonseok.heo for ITS 212895 2013.12.05
           }
           if (device == MP::DISC)
           {
                emit setStatusDataByIndex(device ,DATA_TYPE_MAX_TRACK_NUM, m_history[device].Quantity );
                emit setStatusDataByIndex(device ,DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END );
           }
           break;

        // { modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error 
        case PLAYER_SEARCH:
            if(device == MP::DISC && (m_discType == MEDIA_SOURCE_MP3 || m_discType == MEDIA_SOURCE_CDDA))
            {
                switch(oldState)
                {
                case PLAYER_PLAY:
                case PLAYER_REPEATPLAY:
                case PLAYER_RANDOMPLAY:
                case PLAYER_PAUSE:
                case PLAYER_REPEATPAUSE:
                case PLAYER_RANDOMPAUSE:
                case PLAYER_STOP:
                //{ modified by yongkyun.lee 2013-11-10 for : ITS 208024
                {
                    bool isTuneTimer = GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->isTuneTimer();
                    if(!isTuneTimer){
                        // { added by wonseok.heo for ITS 212895 2013.12.05
                        // { modified by wonseok.heo for smoke Test countInfo issue 2013.12.07
                        emit playbackStarted();
                        // } modified by wonseok.heo for smoke Test countInfo issue 2013.12.07
                    } // } added by wonseok.heo for ITS 212895 2013.12.05
                }
                //} modified by yongkyun.lee 2013-11-10 
                    break;
                default : // make compiler happy
                    break;
                }
            }
            break;
        // } modified by cychoi 2013.09.22

        case PLAYER_PAUSE:
            emit setStatusDataByIndex( device, DATA_TYPE_PLAY_STATUS, PLAY_STATUS_PAUSE );
            if(!m_isLongPressed && !m_tuneState.m_nTuneStart) //added "m_nTuneStart" by junam 2012.10.5 for CR14119
                emit playbackPaused();
            break;

        //{added by junam 2013.07.24 for iPOD FF/RW icon
        case PLAYER_FASTFORWARD:
        case PLAYER_REWIND:
            if(device == MP::IPOD1 || device == MP::IPOD2)
            {
                emit playbackForcedStart();
            }
            // { added by cychoi 2014.03.14 for ITS 229427 Play OSD on Disc FF/REW
            else if(device == MP::DISC)
            {
                GetEngineMain().setIsTermOnTrackChange(false);
            }
            // } added by cychoi 2014.03.14
            break;
        //}added by junam
        //{ added by edo.lee 2013.10.14 ITS 0194917
        case PLAYER_CHANGE_PLAY_ICON:
            emit playbackStarted();
            break;
        
        case PLAYER_CHANGE_PAUSE_ICON:
            emit playbackPaused();
            break;
        //}added by edo.lee 2013.10.14
        default:
            MP_LOG << "Unhandled state =" << state << LOG_ENDL;
            break;
        }
        emit stateChanged(newState);
    }
}

void
AppMediaPlayer_Controller_Audio::HandleError(int errorCode, QString filename)
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;

    m_nLastErrorCode = errorCode; // added by kihyung 2013.12.17 for ITS 0214009

    MP_HIGH << "code =" << errorCode << LOG_ENDL;
    // removed by kihyung 2013.09.13
    
    switch(errorCode)
    {
        // { modified by kihyung 2013.09.12 for ISV 90605
        case eMP_ERR_STREAM:
        case eMP_ERR_UNSUPPORTED_FILE:
        case eMP_ERR_FORMAT_ERROR:
        //case eMP_ERR_RESOURCE:       // moved by sangmin.seol ITS_0218617
        case eMP_ERR_OTHERDATAERROR: // added by kihyung 2013.08.31 for ITS 0187672 
        {
            MP_MEDIUM << "showPopupError(false)" << LOG_ENDL;
            emit showPopupError(false); 
            break;
        }

        // { removed by sangmin.seol 2014.05.27 change error handle
        // Modified by sangmin.seol ITS_0218617 Add Cancel FF,REW Mode for eMP_ERR_RESOURCE Error
        /*case eMP_ERR_RESOURCE:
        {
            cancelFFRW();
            emit showPopupError(false);
            break;
        }*/
        // Modified by sangmin.seol ITS_0218617 Add Cancel FF,REW Mode for eMP_ERR_RESOURCE Error
        // } removed by sangmin.seol 2014.05.27 change error handle

        case eMP_ERR_UNSUPPORTED_ALL_FILE:
        {
            MP_LOG << "getCurrentCategory:" << getCurrentCategory(device) << LOG_ENDL;
             
            if( getCurrentCategory(device) == MP::FOLDER) // folder
            {
                if( m_repeatPlayMode == REPEATALL )
                {
                    // { modified by kihyung 2013.10.30 for ITS 0204910 
                    if(isForeground() == true) 
                    {
                        emit showPopupError(true); // modified by kihyung 2013.09.12 for ISV 90605
                    }
                    else
                    {
                        GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->SetNotifyAllUnsupported();
                    }
                    // } modified by kihyung 2013.10.30                      
                }
                else if( m_repeatPlayMode == REPEATFOLDER )
                {
                    if (m_pIMedia)
                    {
                        MP_LOG << "next folder" << LOG_ENDL;
                        m_pIMedia->NextFolder(NULL);
                    }
                }
            }
            // Modified by sangmin.seol 2013.11.15 for ITS-0209339 play first item of folder if eMP_ERR_UNSUPPORTED_ALL_FILE case in ALBUM, ARTIST, GENRE
            else if ( getCurrentCategory(device) == MP::ALBUM || getCurrentCategory(device) == MP::ARTIST ||  getCurrentCategory(device) == MP::GENRE) // album
            {
                if( m_repeatPlayMode == REPEATALL )
                {
                    //ETrackerAbstarctor_VolumeType volumeType = AudioUtils::VolumeType(device);
                    //m_pTrackerAbstractor->RequestData(volumeType, eGetAllAlbumsWithUrlFromVolumeQuery_Id, GET_TA_REQUID(REQUID_NEXT_ALBUM));;

                    // { Added by sangmin.seol 2013.11.15 for ITS-0209339 play first item of folder if eMP_ERR_UNSUPPORTED_ALL_FILE case in ALBUM, ARTIST, GENRE
                    if(device == MP::JUKEBOX)
                        requestAllPlaylist(device, true);
                    else
                        requestAllPlaylist(device);
                    // } Added by sangmin.seol 2013.11.15 for ITS-0209339 play first item of folder if eMP_ERR_UNSUPPORTED_ALL_FILE case in ALBUM, ARTIST, GENRE
                }
            }
            else
            {
                if( m_repeatPlayMode == REPEATALL )
                {
                    // { modified by kihyung 2013.10.30 for ITS 0204910 
                    if(isForeground() == true) 
                    {
                        emit showPopupError(true); // modified by kihyung 2013.09.12 for ISV 90605
                    }
                    else
                    {
                        GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->SetNotifyAllUnsupported();
                    }
                    // } modified by kihyung 2013.10.30   
                }
            }
            break;
        }
        // } modified by kihyung 2013.09.12 for ISV 90605

        // { added by sungkim for CR9616
        case eMP_CORRUPT_FILE_COPY_ERROR:        // corrupt file copy error
        {
            m_pListViewModel->clearPopup();
            m_pListViewModel->showPopupByExt(LVEnums::POPUP_TYPE_CORRUPT_FILE, false, false);
    		break; //  modified by eugene 2012.08.22 for static analysis defect #7577
        }
        case eMP_UNSUPPORTED_FORMAT_COPY_ERROR:  // unsupported format copy error
        {
            if( GetListViewModel()->getPopType() != (int)LVEnums::POPUP_TYPE_CAPACITY_ERROR ) // added by eugene.seo 2013.02.05 for no capacity error popup
            {
                m_pListViewModel->clearPopup();
                if(!isListView()) // modified by ravikanth 09-07-13 for Smoke Test Fix. If its list let List handle the event
                {
                    // { modified by dongjin 2013.02.18 for ISV 72965
                    //m_pListViewModel->showPopupByExt(LVEnums::POPUP_TYPE_UNAVAILABLE_FILE, false, false);
                    m_pListViewModel->showPopupByExt(LVEnums::POPUP_TYPE_CORRUPT_FILE, false, false);
                    // } modified by dongjin
                }
                else
                {
                    m_pListViewModel->HandleError(errorCode);
                }
            }
            break; //  modified by eugene 2012.08.22 for static analysis defect #7577
        }
        // } added by sungkim for CR9616
        //{added by aettie.ji 2013.01.29 for ISV 71343
    	case eMP_USER_CANCEL_COPY_ERROR:
    	{
    	    m_pListViewModel->clearPopup();
            m_pListViewModel->showPopupByExt(LVEnums::POPUP_TYPE_COPY_CANCELED, false, false);
    		m_pListViewModel->cancelFileOperation(); // added by eugene.seo 2013.06.10
            break;
        } //}added by aettie.ji 2013.01.29 for ISV 71343
        // added by ruindmby 2012.09.26 for CR#11543
        case eMP_LESS_SPACE_ERROR:
        {
            m_pListViewModel->clearPopup();
            break;
        }
        // added by ruindmby 2012.09.26 for CR#11543

        // { added by kihyung 2012.12.04 for ISV 62683 
        case PLAYER_COMMANDNOTSUPPORTED:
        {
            emit showInhibitionIcon(true);
            break;
        }
        // } added by kihyung
        
        // { added by eugene.seo 2013.04.03
        case eMP_ERR_AUTOAUDIOSINK_FAIL:
        case eMP_ERR_RESOURCE:      // added by sangmin.seol 2014.05.27 eMP_ERR_RESOURCE error handle same with eMP_ERR_AUTOAUDIOSINK_FAIL
        {
            if(m_pIMedia) 
            {
                //m_pIMedia->Stop(); //removed by sangmin.seol 2014.05.27 change error handle //added for ITS 227056 2014.02.27
                GSTAudioRouterDBus *pDBus = GetEngineMain().GetNotifier()->getGSTAudioRouterDBus();

                if(pDBus) {
                    MP_MEDIUM << "GSTAudioRouterDBus.ReleaseAudioSource" << LOG_ENDL;
                    pDBus->ReleaseAudioSource(EAllSource); // added by kihyung 2013.07.04 for ISV 85455
                }
                else
                {
                    MP_HIGH << "GSTAudioRouterDBus is not valid" << LOG_ENDL;
                }

                // { modified by kihyung 2013.12.17 for ITS 0214009 
                // m_pAudioSinkTimer->start();
                m_pAudioSinkTimer->stop();
                m_pAudioSinkTimer->setInterval(AUDIO_SINK_ERROR_INTERVAL);
                m_pAudioSinkTimer->start();
                m_skipTrack = true;     // added by sangmin.seol 2014.05.27 skip when autotrack change.
                // } modified by kihyung 2013.12.17 for ITS 0214009                 
            }
            break;
        }
        // } added by eugene.seo 2013.04.03
        
        // { added by kihyung 2013.4.7 for IPOD
        case 1000:
        {
            MP_LOG << "IPODERR. " << filename << LOG_ENDL;
            
            emit changeIPodCategory(2);   
            
            ETrackerAbstarctor_VolumeType volumeType;
            if(device == MP::IPOD1) {
                volumeType = eIPOD_FRONT;
                m_nIPOD1ErrIndex = 0;
            }
            else if(device == MP::IPOD2) {
                volumeType = eIPOD_REAR;
                m_nIPOD2ErrIndex = 0;
            }
            else
                return;

            Stop();

            if(filename.isEmpty() == false) {
                m_pTrackerAbstractor->RequestData(filename, volumeType, eGetSongIndexFromVolumeQuery_Id, GET_TA_REQUID(REQUID_IPOD_ERR_SONG_IDX));
            }
            else {
                m_pTrackerAbstractor->RequestData(volumeType, eGetAllSongsFromVolumeQuery_Id, GET_TA_REQUID(REQUID_IPOD_ERR_ALL_SONGS));
            }
        }
        // } added by kihyung 2013.4.7 for IPOD        
        default:
            break;
    }
}
// { added by jungae 2012.10.09, for CR14252, iPod reset when device name is same with BT
void AppMediaPlayer_Controller_Audio::StartiPodReset()
{
    MP_LOG << "m_device" << getDevice() << LOG_ENDL;
    if(m_pIMedia) {
        m_pIMedia->SetSystemState(SYSTEM_IPODDEVICE_RESET_ON);
    }
}
// } added by jungae
//added by edo.lee 2013.06.22
void AppMediaPlayer_Controller_Audio::SetSystemMute()
{
    MP_LOG << LOG_ENDL;
    if(m_pIMedia) {
        //m_pIMedia->SetSystemState(SYSTEM_REQUEST_MUTE);
    }
}
//added by edo.lee 2013.06.22

void
AppMediaPlayer_Controller_Audio::ConnectJukeboxSignal()
{
    disconnectControllers();

    connect(m_pJukeboxController, SIGNAL(OnCurrentMediaStatusChanged(int)), this, SLOT(HandleMediaStatusChanged(int)));
    connect(m_pJukeboxController, SIGNAL(OnCurrentMediaInfo(MediaInfo*)), this, SLOT(onMediaInfoChanged(MediaInfo*)));
    connect(m_pJukeboxController, SIGNAL(OnPositionChanged(int)), this, SLOT(onPositionChanged(int)));
    connect(m_pJukeboxController, SIGNAL(OnDurationChanged(int)), this, SLOT(onDurationChanged(int)));
    connect(m_pJukeboxController, SIGNAL(OnStateChanged(int)),    this, SLOT(onStateChanged(int)));
    connect(m_pJukeboxController, SIGNAL(OnNormalPlay()),         this, SLOT(onNormalPlay())); // added by Sergey 28.05.2013
    connect(m_pJukeboxController, SIGNAL(OnError(int)),           this, SIGNAL(Error()));
    connect(m_pJukeboxController, SIGNAL(OnError(int, QString)),  this, SLOT(HandleError(int, QString)));
    connect(m_pJukeboxController, SIGNAL(OnAutoTrackChanged(int)),    this, SLOT(onAutoTrackChanged(int)));
    connect(m_pJukeboxController, SIGNAL(OnTrackChanged(int)),    this, SLOT(onTrackChanged(int)));
    connect(m_pJukeboxController, SIGNAL(OnScanCompleted()),      this, SLOT(ScanComplete()));
    connect(m_pJukeboxController, SIGNAL(OnModeChanged(int)),     this, SLOT(onModeChanged(int))); // added by kihyung 2012.07.06 for HMC_DH__SANITY_DISC282, HMC_DH__SANITY_DISC283
    connect(m_pJukeboxController, SIGNAL(OnRealSoundChanged()),this, SLOT(onRealSoundChanged())); //added by edo.lee 20103.04.21
    connect(m_pJukeboxController, SIGNAL(OnTrackBufferred(int, int, int, int)),     this, SLOT(OnTrackBufferred(int, int, int, int))); //added by edo.lee 2013.05.14
}

void
AppMediaPlayer_Controller_Audio::ConnectUSBSignals()
{
    disconnectControllers();

    connect(m_pUsbController, SIGNAL(OnCurrentMediaStatusChanged(int)), this, SLOT(HandleMediaStatusChanged(int)));
    connect(m_pUsbController, SIGNAL(OnCurrentMediaInfo(MediaInfo*)), this, SLOT(onMediaInfoChanged(MediaInfo*)));
    connect(m_pUsbController, SIGNAL(OnPositionChanged(int)), this, SLOT(onPositionChanged(int)));
    connect(m_pUsbController, SIGNAL(OnDurationChanged(int)), this, SLOT(onDurationChanged(int)));
    connect(m_pUsbController, SIGNAL(OnStateChanged(int)),    this, SLOT(onStateChanged(int)));
    connect(m_pUsbController, SIGNAL(OnNormalPlay()),         this, SLOT(onNormalPlay()));// added by Sergey 28.05.2013
    connect(m_pUsbController, SIGNAL(OnError(int)),           this, SIGNAL(Error()));
    connect(m_pUsbController, SIGNAL(OnError(int, QString)),  this, SLOT(HandleError(int, QString)));
    connect(m_pUsbController, SIGNAL(OnAutoTrackChanged(int)),    this, SLOT(onAutoTrackChanged(int)));
    connect(m_pUsbController, SIGNAL(OnTrackChanged(int)),    this, SLOT(onTrackChanged(int)));
    connect(m_pUsbController, SIGNAL(OnScanCompleted()),      this, SLOT(ScanComplete()));
    connect(m_pUsbController, SIGNAL(OnModeChanged(int)),     this, SLOT(onModeChanged(int))); // added by kihyung 2012.07.06 for HMC_DH__SANITY_DISC282, HMC_DH__SANITY_DISC283
    connect(m_pUsbController, SIGNAL(OnRealSoundChanged()),     this, SLOT(onRealSoundChanged())); //added by edo.lee 20103.04.21
    connect(m_pUsbController, SIGNAL(OnTrackBufferred(int, int, int, int)),     this, SLOT(OnTrackBufferred(int, int, int, int))); //added by edo.lee 2013.05.14
}

void
AppMediaPlayer_Controller_Audio::ConnectDeckSignals()
{
    MP_LOG << LOG_ENDL;
    disconnectControllers();

    connect(m_pDeckController, SIGNAL(OnSearchCompleted(int, int, int)), this, SLOT(OnSearchCompleted(int, int, int)));
    connect(m_pDeckController, SIGNAL(OnPositionChanged(int)), this, SLOT(onPositionChanged(int)));
    connect(m_pDeckController, SIGNAL(OnDurationChanged(int)), this, SLOT(onDurationChanged(int)));
    connect(m_pDeckController, SIGNAL(OnStateChanged(int)),    this, SLOT(onStateChanged(int)));
    connect(m_pDeckController, SIGNAL(OnNormalPlay()),         this, SLOT(onNormalPlay()));// added by Sergey 28.05.2013    
    connect(m_pDeckController, SIGNAL(OnError(int)),           this, SIGNAL(Error()));
    connect(m_pDeckController, SIGNAL(OnGraceNoteTOCReceived(int,void*)), this, SLOT(onGraceNoteTOCReceived(int, void*)));
    connect(m_pDeckController, SIGNAL(OnTrackChanged(int)),    this, SLOT(onDeckTrackChanged(int)));
    connect(m_pDeckController, SIGNAL(OnTrackUpdated(int)),    this, SLOT(onDeckTrackUpdated(int))); // added by cychoi 2013.11.17 for ITS 208698 MP3 Play Info on Cluster 
    connect(m_pDeckController, SIGNAL(OnModeChanged(int)),     this, SLOT(onModeChanged(int))); // added by kihyung 2012.07.06 for HMC_DH__SANITY_DISC282, HMC_DH__SANITY_DISC283
    connect(m_pDeckController, SIGNAL(OnScanCompleted()),      this, SLOT(ScanComplete()));
    connect(m_pDeckController, SIGNAL(OnTagReceived(DvdcTagInfo *)),      this, SLOT(onTagReceived(DvdcTagInfo *)));
    connect(m_pDeckController, SIGNAL(OnAllTagReceived(DvdcAllTagInfo *)),      this, SLOT(onAllTagReceived(DvdcAllTagInfo *)));
    connect(m_pDeckController, SIGNAL(OnError(int, QString)),  this, SLOT(HandleError(int, QString))); // added by kihyung 2012.12.04 for ISV 62683 
    connect(m_pDeckController, SIGNAL(OnRealSoundChanged()),   this, SLOT(onRealSoundChanged())); //added by cychoi 20103.06.11
    connect(m_pDeckController, SIGNAL(OnTrackBufferred(int, int, int, int)),     this, SLOT(OnTrackBufferred(int, int, int ,int))); //added by edo.lee 2013.05.14    
	connect(m_pDeckController, SIGNAL(OnUserTrackChanged(int)),     this, SLOT(onUserTrackChanged(int))); //added by edo.lee 2013.05.14
    connect(m_pDeckController, SIGNAL(OnAutoTrackChanged(int)),     this, SLOT(onAutoTrackChanged(int))); // added by cychoi 2014.04.06 for ITS 233500
}

void
AppMediaPlayer_Controller_Audio::ConnectIpod1Signal()
{
    disconnect( m_pIpod1Controller, 0, 0, 0 );

    connect(m_pIpod1Controller, SIGNAL(OnCurrentMediaStatusChanged(int)), this, SLOT(onMediaStatusChanged(int))); //added by junam 2012.10.08 for CR14264
    connect(m_pIpod1Controller, SIGNAL(iPodReady(bool)),        this, SLOT(onIPodReady(bool)));
    connect(m_pIpod1Controller, SIGNAL(autoPlayReady(bool)),        this, SLOT(onAutoPlayReady(bool)));
    connect(m_pIpod1Controller, SIGNAL(OnCurrentMediaInfo(MediaInfo*)), this, SLOT(onMediaInfoChanged(MediaInfo*)));
    connect(m_pIpod1Controller, SIGNAL(OnPositionChanged(int)), this, SLOT(onPositionChanged(int)));
    connect(m_pIpod1Controller, SIGNAL(OnDurationChanged(int)), this, SLOT(onDurationChanged(int)));
    connect(m_pIpod1Controller, SIGNAL(OnStateChanged(int)),    this, SLOT(onStateChanged(int)));
    //connect(m_pIpod1Controller, SIGNAL(OnNormalPlay()),         this, SLOT(onNormalPlay())); // removed by junam 2013.06.09 for prev key
    connect(m_pIpod1Controller, SIGNAL(OnError(int)),           this, SIGNAL(Error()));
    connect(m_pIpod1Controller, SIGNAL(OnError(int, QString)),  this, SLOT(HandleError(int, QString)));
    connect(m_pIpod1Controller, SIGNAL(OnTrackChanged(int)),    this, SLOT(onTrackChanged(int)));
    connect(m_pIpod1Controller, SIGNAL(OnAutoTrackChanged(int)),    this, SLOT(onAutoTrackChanged(int)));

    connect(m_pIpod1Controller, SIGNAL(IPODDisConnected(QString, int)), this, SLOT(ipodDisconnectedSlot(QString, int)));
    connect(m_pIpod1Controller, SIGNAL(OnScanCompleted()),      this, SLOT(ScanComplete()));
    //connect(m_pIpod1Controller, SIGNAL(IPODConnected(QString,int,int,int,int)), this, SLOT(ipodConnectedSlot(QString,int,int,int,int)));//removed by junam 2013.07.10 for calcel ff
    connect(m_pIpod1Controller, SIGNAL(IPodPopUp(EIPodPopUp)), this, SLOT(displayiPodTaggingOSD(EIPodPopUp)));//revert by junam 2013.03.26 for ISV75917
    connect(m_pIpod1Controller, SIGNAL(IPodTotalNumTracks(int)),this, SLOT(onIPodTotalNumTracks(int)) );
    //connect(m_pIpod1Controller, SIGNAL(OnModeChanged(int)),     this, SLOT(onModeChanged(int))); // removed by kihyung 2013.06.02
    connect(m_pIpod1Controller, SIGNAL(OnGetDeviceName(QString)),     this, SLOT(onGetDeviceName1(QString))); // added by jungae 2012.09.03 for CR 13162    
    connect(m_pIpod1Controller, SIGNAL(SystemStateStatus(int)),     this, SLOT(onSystemStateStatus(int))); // added by jungae 2012.10.09, for CR14252, iPod reset when device name is same with BT
    connect(m_pIpod1Controller, SIGNAL(IPodPlayableSongCount(int, int)),     this, SIGNAL(iPodPlayableSongCount(int, int)));// added by jungae 2012.10.10 for CR 13753  
    connect(m_pIpod1Controller, SIGNAL(onIPodConnectedSignal(QString, QString, EIPodPorts, bool)), this, SLOT(onIPodDeviceConnectedSlot( QString, QString, EIPodPorts, bool )));


    //{added by junam 2013.04.13 for AutoStart
    connect(m_pIpod1Controller, SIGNAL(RequestComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, int, uint)),
            this,   SLOT(HandleRequestComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, int, uint)));
    //}added by junam

    //{changed by junam 2013.05.01 for database update
    connect(m_pIpod1Controller, SIGNAL(OnCurrentPlayingTrackCount(int)),this, SLOT(onCurrentTrackCountChanged(int)) ); //added by junam 2013.04.23 for index DB
    connect(m_pIpod1Controller, SIGNAL(OnCurrentTrackCountChanged(int)),this, SLOT(onDatabaseUpdate(int)) );
    //}changed by junam
    connect(m_pIpod1Controller, SIGNAL(OnTrackBuffered(int, int, int , int)),     this, SLOT(OnTrackBufferred(int, int, int, int))); //added by edo.lee 2013.05.25
    connect(m_pIpod1Controller, SIGNAL(OnBasicControlEnableStatusChange(bool,QString)), this, SLOT(onBasicControlEnableStatusChanged(bool,QString)));//added by junam 2013.07.12 for music app
#ifdef SIRI_TIMEOUT_TEST
    connect(m_pIpod1Controller, SIGNAL(LaunchedDefaultMusicApp(bool, QString)), this, SLOT(onLaunchedDefaultMusicApp(bool, QString)));
    connect(m_pIpod1Controller, SIGNAL(NotifyPlayStateForSiri()),               this, SLOT(onNotifyPlayStateForSiri()));
#endif
}

void
AppMediaPlayer_Controller_Audio::ConnectIpod2Signal()
{
    disconnect( m_pIpod2Controller, 0, 0, 0 );

    connect(m_pIpod2Controller, SIGNAL(OnCurrentMediaStatusChanged(int)), this, SLOT(onMediaStatusChanged(int))); //added by junam 2012.10.08 for CR14264
    connect(m_pIpod2Controller, SIGNAL(iPodReady(bool)),        this, SLOT(onIPodReady(bool)));
    connect(m_pIpod2Controller, SIGNAL(autoPlayReady(bool)),        this, SLOT(onAutoPlayReady(bool)));
    connect(m_pIpod2Controller, SIGNAL(OnCurrentMediaInfo(MediaInfo*)), this, SLOT(onMediaInfoChanged(MediaInfo*)));
    connect(m_pIpod2Controller, SIGNAL(OnPositionChanged(int)), this, SLOT(onPositionChanged(int)));
    connect(m_pIpod2Controller, SIGNAL(OnDurationChanged(int)), this, SLOT(onDurationChanged(int)));
    connect(m_pIpod2Controller, SIGNAL(OnStateChanged(int)),    this, SLOT(onStateChanged(int)));
    //connect(m_pIpod2Controller, SIGNAL(OnNormalPlay()),         this, SLOT(onNormalPlay())); // removed by junam 2013.06.09 for prev key
    connect(m_pIpod2Controller, SIGNAL(OnError(int)),           this, SIGNAL(Error()));
    connect(m_pIpod2Controller, SIGNAL(OnError(int, QString)),  this, SLOT(HandleError(int, QString)));
    connect(m_pIpod2Controller, SIGNAL(OnTrackChanged(int)),    this, SLOT(onTrackChanged(int)));
    connect(m_pIpod2Controller, SIGNAL(OnAutoTrackChanged(int)),    this, SLOT(onAutoTrackChanged(int)));
    connect(m_pIpod2Controller, SIGNAL(IPODDisConnected(QString, int)), this, SLOT(ipodDisconnectedSlot(QString, int)));
    connect(m_pIpod2Controller, SIGNAL(OnScanCompleted()),      this, SLOT(ScanComplete()));
    //connect(m_pIpod2Controller, SIGNAL(IPODConnected(QString,int,int,int,int)), this, SLOT(ipodConnectedSlot(QString,int,int,int,int)));//removed by junam 2013.07.10 for calcel ff
    connect(m_pIpod2Controller, SIGNAL(IPodPopUp(EIPodPopUp)), this, SLOT(displayiPodTaggingOSD(EIPodPopUp))); //revert by junam 2013.03.26 for ISV75917
    connect(m_pIpod2Controller, SIGNAL(IPodTotalNumTracks(int)),this, SLOT(onIPodTotalNumTracks(int)) );
    //connect(m_pIpod2Controller, SIGNAL(OnModeChanged(int)),     this, SLOT(onModeChanged(int))); // removed by kihyung 2013.06.02
    connect(m_pIpod2Controller, SIGNAL(OnGetDeviceName(QString)),     this, SLOT(onGetDeviceName2(QString))); // added by jungae 2012.09.03 for CR 13162
    connect(m_pIpod2Controller, SIGNAL(SystemStateStatus(int)),     this, SLOT(onSystemStateStatus(int))); // added by jungae 2012.10.09, for CR14252, iPod reset when device name is same with BT
    connect(m_pIpod2Controller, SIGNAL(IPodPlayableSongCount(int, int)),     this, SIGNAL(iPodPlayableSongCount(int, int))); // added by jungae 2012.10.10 for CR 13753  
    connect(m_pIpod2Controller, SIGNAL(onIPodConnectedSignal(QString, QString, EIPodPorts, bool)), this, SLOT(onIPodDeviceConnectedSlot( QString, QString, EIPodPorts, bool )));

    //{added by junam 2013.04.13 for AutoStart
    connect(m_pIpod2Controller, SIGNAL(RequestComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, int, uint)),
            this,   SLOT(HandleRequestComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, int, uint)));
    //}added by juanm

    //{changed by junam 2013.05.01 for database update
    connect(m_pIpod2Controller, SIGNAL(OnCurrentPlayingTrackCount(int)),this, SLOT(onCurrentTrackCountChanged(int)) );//added by junam 2013.04.23 for index DB
    connect(m_pIpod2Controller, SIGNAL(OnCurrentTrackCountChanged(int)),this, SLOT(onDatabaseUpdate(int)) );
    //}changed by junam
    connect(m_pIpod2Controller, SIGNAL(OnTrackBuffered(int, int, int, int)),     this, SLOT(OnTrackBufferred(int, int, int, int))); //added by edo.lee 2013.05.25
    connect(m_pIpod2Controller, SIGNAL(OnBasicControlEnableStatusChange(bool,QString)), this, SLOT(onBasicControlEnableStatusChanged(bool,QString)));//added by junam 2013.07.12 for music app
#ifdef SIRI_TIMEOUT_TEST
    connect(m_pIpod2Controller, SIGNAL(LaunchedDefaultMusicApp(bool, QString)), this, SLOT(onLaunchedDefaultMusicApp(bool, QString)));
    connect(m_pIpod2Controller, SIGNAL(NotifyPlayStateForSiri()),               this, SLOT(onNotifyPlayStateForSiri()));
#endif
}

void
AppMediaPlayer_Controller_Audio::ConnectIpod1SignalInRuntime()
{
    connect(m_pIpod1Controller, SIGNAL(OnModeChanged(int)), this, SLOT(onModeChanged(int)));
}

void
AppMediaPlayer_Controller_Audio::ConnectIpod2SignalInRuntime()
{
    connect(m_pIpod2Controller, SIGNAL(OnModeChanged(int)), this, SLOT(onModeChanged(int)));
}

void
AppMediaPlayer_Controller_Audio::disconnectControllers()
{
    MP_LOG << LOG_ENDL;

    // { modified by eugeny.novikov 2012.10.10 for CR 14229
    if (m_pJukeboxController)
    {
        disconnect(m_pJukeboxController, 0, this, 0);
    }

    if (m_pUsbController)
    {
        disconnect(m_pUsbController, 0, this, 0);

        //need to keep connection anytime
        connect(m_pUsbController, SIGNAL(OnFormatFinished(int)), m_pListViewModel, SLOT(FormattingComleteHandler(int)), Qt::UniqueConnection); //added by junam 2013.10.04 for ITS_KOR_193667
    }
    // } modified by eugeny.novikov

    // { added by kihyung 2013.06.02
    if (m_pIpod1Controller) 
    {
        disconnect(m_pIpod1Controller, SIGNAL(OnModeChanged(int)), this, SLOT(onModeChanged(int)));
    }

    if (m_pIpod2Controller) 
    {
        disconnect(m_pIpod2Controller, SIGNAL(OnModeChanged(int)), this, SLOT(onModeChanged(int)));
    }    
    // } added by kihyung 2013.06.02    

    if (m_pDeckController)
    {
        //Moved by yongkyun.lee 2013-08-06 for : ISV 88607
        disconnect(m_pDeckController, SIGNAL(OnSearchCompleted(int, int, int)), this, SLOT(OnSearchCompleted(int, int, int)));
        disconnect(m_pDeckController, SIGNAL(OnPositionChanged(int)), this, SLOT(onPositionChanged(int)));
        disconnect(m_pDeckController, SIGNAL(OnDurationChanged(int)), this, SLOT(onDurationChanged(int)));
        disconnect(m_pDeckController, SIGNAL(OnStateChanged(int)),    this, SLOT(onStateChanged(int)));
        disconnect(m_pDeckController, SIGNAL(OnNormalPlay()),         this, SLOT(onNormalPlay()));// added by Sergey 28.05.2013    
        disconnect(m_pDeckController, SIGNAL(OnError(int)),           this, SIGNAL(Error()));
        disconnect(m_pDeckController, SIGNAL(OnGraceNoteTOCReceived(int,void*)), this, SLOT(onGraceNoteTOCReceived(int, void*)));
        disconnect(m_pDeckController, SIGNAL(OnTrackChanged(int)),    this, SLOT(onDeckTrackChanged(int)));
        disconnect(m_pDeckController, SIGNAL(OnTrackUpdated(int)),    this, SLOT(onDeckTrackUpdated(int))); // added by cychoi 2013.11.17 for ITS 208698 MP3 Play Info on Cluster 
        disconnect(m_pDeckController, SIGNAL(OnModeChanged(int)), this, SLOT(onModeChanged(int))); // added by kihyung 2012.07.06 for HMC_DH__SANITY_DISC282, HMC_DH__SANITY_DISC283
        disconnect(m_pDeckController, SIGNAL(OnScanCompleted()),      this, SLOT(ScanComplete()));
        disconnect(m_pDeckController, SIGNAL(OnTagReceived(DvdcTagInfo *)), this, SLOT(onTagReceived(DvdcTagInfo *)));
        disconnect(m_pDeckController, SIGNAL(OnAllTagReceived(DvdcAllTagInfo *)), this, SLOT(onAllTagReceived(DvdcAllTagInfo *)));
        disconnect(m_pDeckController, SIGNAL(OnError(int, QString)),      this, SLOT(HandleError(int, QString))); // added by kihyung 2012.12.04 for ISV 62683 
        disconnect(m_pDeckController, SIGNAL(OnRealSoundChanged()),   this, SLOT(onRealSoundChanged())); //added by cychoi 20103.06.11
        disconnect(m_pDeckController, SIGNAL(OnTrackBufferred(int, int, int, int)),     this, SLOT(OnTrackBufferred(int, int, int, int))); //added by edo.lee 2013.05.14
        disconnect(m_pDeckController, SIGNAL(OnUserTrackChanged(int)),     this, SLOT(onUserTrackChanged(int))); //added by edo.lee 2013.05.14
        disconnect(m_pDeckController, SIGNAL(OnAutoTrackChanged(int)),     this, SLOT(onAutoTrackChanged(int))); // added by cychoi 2014.04.06 for ITS 233500
    }
}

void
AppMediaPlayer_Controller_Audio::setFolder (const QUrl &folder)
{
    Q_UNUSED(folder);

    // { removed by kihyung 2013.2.17
    /*
    if (m_device != MP::USB1 && m_device != MP::USB2 && m_device != MP::JUKEBOX)
    {
        return;
    }

    if (folder == m_folderPath)
    {
        return;
    }

    QString localFile = folder.toLocalFile();
    m_folderPath = folder;

    if ( m_pTrackerAbstractor != 0 )
    {
        MP_LOG << "Request data from " << localFile << LOG_ENDL;
        if ( localFile != "/" )
        {
            m_pTrackerAbstractor->RequestData( localFile, eSongsByFolderQuery_Id );
        }
    }
    */
    // } removed by kihyung 2013.2.17    
}

QUrl
AppMediaPlayer_Controller_Audio::folder() const
{
    return m_folderPath;
}

QString
AppMediaPlayer_Controller_Audio::album() const
{
    return m_albumName;
}

void
AppMediaPlayer_Controller_Audio::setAlbumThumbnail (QUrl thumbnail)
{
    m_albumThumbnail = thumbnail;
//    MP_LOG << "m_albumThumbnail =" << m_albumThumbnail << LOG_ENDL; // removed by eugeny.novikov 2012.10.25 for CR 14047
}

void
AppMediaPlayer_Controller_Audio::setAlbum (const QString &album)
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    if (device != MP::JUKEBOX  &&
        device != MP::USB1 && device != MP::USB2 &&
        device != MP::IPOD1 && device != MP::IPOD2)
    {
        MP_LOG<<"Invalid device " << device << LOG_ENDL; //added by junam 2013.06.21 for log
        return;
    }

    //Already have song list
    if ( m_albumNameTemp.isEmpty() &&  m_albumName == album &&
        ! m_tuneState.m_nTuneRequest)
    {
        MP_LOG<<"Already have song list" << "album = "<< album << LOG_ENDL; //added by junam 2013.06.21 for log
        //MP_LOG<<"album = "<< album << LOG_ENDL;
        return;
    }

    //Already request song list
    if ( album != NONAME_ALBUM && m_albumNameTemp == album ) // modified by eugene.seo 2013.01.31 for unsupported audio play in each repeat mode
    {
        MP_LOG<<"Already request song list" << LOG_ENDL; //added by junam 2013.06.21 for log
        return;
    }
	
    clearTemps();
    m_albumNameTemp = album;

    if ( m_pTrackerAbstractor != NULL)
    {
        MP_LOG << "Request songs from album" << album << LOG_ENDL;

        ETrackerAbstarctor_VolumeType volumeType = AudioUtils::VolumeType(device);

        if (volumeType == eDefaultVolume)
            return;

        if ((device == MP::IPOD1 || device == MP::IPOD2) && !getAppleDeviceStatus(device))
            return;

        //{ changed by junam 2012.11.12 for CR15447
        m_pPathViewModel->requestCoverEnable(true);
        if (album == NONAME_ALBUM)
        {
            MP_LOG << "send request eGetAllSongsWithoutAlbumFromVolumeQuery_Id from volume " << volumeType << LOG_ENDL;
            m_pTrackerAbstractor->RequestData( volumeType, eGetAllSongsWithoutAlbumFromVolumeQuery_Id, GET_TA_REQUID(REQUID_ALBUM_CHANGE));
        }
        else if ( album != "" )
        {
            MP_LOG << "send request eGetAllSongsByAlbumFromVolumeQuery_Id from volume " << volumeType << LOG_ENDL;
            m_pTrackerAbstractor->RequestData( album, volumeType, eGetAllSongsByAlbumFromVolumeQuery_Id, GET_TA_REQUID(REQUID_ALBUM_CHANGE));
        }
        // } changed by junam
    }
}

// { added by kihyung 2013.08.08 for ITS 0181967 
void AppMediaPlayer_Controller_Audio::updateFSFolderList(MP::DEVICE_T device)
{
    if(device >= MP::ALL_DEVICES) return;

    MP_LOG << device << m_history[device].UpdatedFSInfo << LOG_ENDL;
    
    if(m_pIMediaEx && m_history[device].UpdatedFSInfo == false) 
    {
        QList<FSFolderInfo> tFolderInfo;
        m_pTrackerAbstractor->GetFSFolderInfo(tFolderInfo, AudioUtils::VolumeType(device), eFSAudio);
        m_pIMediaEx->SetFSFolderInfo(tFolderInfo);
        m_history[device].UpdatedFSInfo = true;
    }
}
// } added by kihyung 2013.08.08 for ITS 0181967
 
// { modified by kihyung 2013.09.09 for ISV 90758 
int AppMediaPlayer_Controller_Audio::updateDevicePlayList( MP::DEVICE_T device, QStringList &filePaths, bool bJukeboxCopied)		// modified by sangmin.seol 2013.10.22 JUKEBOX PlayerView Counter Update after copy from USB
{
    MP_HIGH << device << LOG_ENDL;

    int            nRetVal      = 0;
    int            nNewIndex    = -1;
    int            nNewPosition = 0;
    QString        strCurrFile  = QString("");
    QList<QString> tNewList;

    ETrackerAbstarctor_VolumeType volumeType = AudioUtils::VolumeType(device);            

    if(m_history[device].Index >= 0 && m_history[device].Index < m_history[device].playList.list->count()) 
    {
        strCurrFile = m_history[device].playList.list->at(m_history[device].Index);
    }

	// modified by sangmin.seol 2013.10.22 JUKEBOX PlayerView Counter Update after copy from USB
    if(bJukeboxCopied || (m_history[device].ContentPlayed == MP::FOLDER || m_history[device].ContentPlayed == MP::DEFAULT))
    {
        foreach(QString track, filePaths)
        {
            tNewList.append(track);
            if(nNewIndex == -1) {
                if(track == strCurrFile) {
                    nNewIndex = tNewList.size() - 1;
                }
            }
        }

        // added by sangmin.seol 2013.10.22 JUKEBOX PlayerView Counter Update after copy from USB, Change jukebox history tab_id to Folder.
        if(bJukeboxCopied)
        {
            m_history[MP::JUKEBOX].listReqData.tabId = "Folder";

            // added by sangmin.seol 2013.11.13 added by sangmin.seol 2013.11.13 For ITS-0189462 for Category disable if audio file copied to jukebox
            m_minerState.isMinerFinished = false;
            if(m_pListViewModel)
                 m_pListViewModel->setCategoryTabAvailable(MP::JUKEBOX, false); 
            // added by sangmin.seol 2013.11.13 added by sangmin.seol 2013.11.13 For ITS-0189462 for Category disable if audio file copied to jukebox            
        }
    }
    else
    {
        bool bAllPlayListExist = true; //added by junam 2014.01.03 for ITS_KOR_196056
        foreach(QString track, *m_history[device].playList.list)
        {
            if(m_pTrackerAbstractor->FileExists(track, volumeType) == true)
            {
                tNewList.append(track);
                if(nNewIndex == -1) {
                    if(track == strCurrFile) {
                        nNewIndex = tNewList.size() - 1;
                    }
                }
            }
            else
            {
                bAllPlayListExist = false; //added by junam 2014.01.03 for ITS_KOR_196056
            }
        }
        //{added by junam 2014.01.03 for ITS_KOR_196056
        if(bAllPlayListExist)
        {
            MP_LOG<<"All song of playlist did not changed" << LOG_ENDL;
            return 3;
        }
        //}added by junam
    }
    
    if(tNewList.size() > 0) 
    {
        if(nNewIndex == -1) 
        {
            nNewIndex = 0;
            nRetVal   = 2;
        }
        else
        {
            nNewPosition = m_history[device].Position;
            nRetVal      = 1;
        }
    
        MP_LOG << "tNewList.size(): " << tNewList.size() << ", nNewIndex : " << nNewIndex << ", nNewPosition = " << nNewPosition << LOG_ENDL;
		//MP_HIGH << "tNewList.size(): " << tNewList.size() << LOG_ENDL;
        //MP_HIGH << "nNewIndex      : " << nNewIndex << LOG_ENDL;
        //MP_HIGH << "nNewPosition   : " << nNewPosition << LOG_ENDL;
    
        m_history[device].playList.list->clear();
        *m_history[device].playList.list = tNewList;
        m_history[device].Position       = nNewPosition;
        m_history[device].Index          = nNewIndex;
        m_history[device].UpdatedFSInfo  = false;

        if(device == getDevice())      // added by sangmin.seol 2014.03.24 To Fix tune wheel skip after music copy in USB Mode
            m_moveTrackIndex = nNewIndex;   // modified by sangmin.seol To Fix Tune Wheel Skip After Music Delete
    }
    // { added by kihyung 2013.11.06 for ITS 0206863 
    else
    {
        ResetPlaylistInfo(device);
        m_history[device].repeatPlayMode = REPEATALL; 
        m_history[device].randomPlayMode = RANDOMOFF;
    }
    // } added by kihyung 2013.11.06 for ITS 0206863 

    return nRetVal;
}
// } modified by kihyung 2013.09.09 for ISV 90758  

// { modified by kihyung 2013.08.02 for ITS 0180215
bool AppMediaPlayer_Controller_Audio::createDevicePlayList( MP::DEVICE_T device, QStringList &filePaths, MP::PLAYBACK_CONTENT_T contentPlayed, bool bRescan, int nStartIdx )
{
    Q_UNUSED(bRescan);

    if(device >= MP::ALL_DEVICES) return false;
    
    if ( !filePaths.isEmpty() && !isFilePathValid( filePaths[0], device ) ) // modified by kihyung 2013.08.06 for ITS 0182926
    {
        MP_HIGH << "error" << LOG_ENDL;
        return false;
    }

    if(nStartIdx < 0) 
        nStartIdx = 0;
    else if(nStartIdx >= filePaths.size()) 
        nStartIdx = filePaths.size() - 1;

    QString curFile = "";
    if(m_history[device].playList.list)
    {
        // { modified by kihyung 2013.07.20 for ITS 0181069
        /*
        if(m_history[device].playList.index >= 0 && m_history[device].playList.index < m_history[device].playList.list->count())
            curFile = m_history[device].playList.list->at(m_history[device].playList.index);
        */
        if(m_history[device].Index >= 0 && m_history[device].Index < m_history[device].playList.list->count())
            curFile = m_history[device].playList.list->at(m_history[device].Index);
        // } modified by kihyung 2013.07.20 for ITS 0181069
    }
    
    ResetPlaylistInfo(device);
    
    // m_history[device].Position = nPosition; // modified by kihyung 2013.08.29 for ITS 0187228
    m_history[device].ContentPlayed = contentPlayed;
    m_history[device].UpdatedFSInfo = false;

    // { modified by kihyung 2013.08.02 for ITS 0181857 
    int i = 0;
    int bFound = false;
    foreach (QString file, filePaths)
    {
        m_history[device].playList.list->append(file);
        if(bFound == false) {
            if (file == curFile) {
                bFound = true;
                nStartIdx = i;
            }
        }
        i++;
    }
    if(bFound == false) nStartIdx = 0;
    // } modified by kihyung 2013.08.02 for ITS 0181857 
    
    MP_HIGH << bFound << m_history[device].playList.list->count() << nStartIdx << LOG_ENDL;

    SetPlaylist(device, nStartIdx);

    SetDefaultPlayMode(m_history[device].ContentPlayed);
        
    //emit updateSongsCount();    // modified by Dmitry 28.05.13  // removed & moved by sungha.choi 2013.09.06 for ISV 90758 ITS 0188052

    return bFound;
}
// } modified by kihyung 2013.08.02 for ITS 0180215

/**
 * Create PlayList from selected album folder and start play it.
 */
void
AppMediaPlayer_Controller_Audio::createPlayList(QStringList filePaths, MP::PLAYBACK_CONTENT_T contentPlayed)
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    MP_LOG << LOG_ENDL;

    if ( !m_songNameListTemp.isEmpty() )
    {
        m_songNameList.clear();
        m_albumName = m_albumNameTemp;
        m_songNameList << m_songNameListTemp;

        clearTemps();

        MP_LOG << "filePaths =" << filePaths << ", m_albumName  =" << m_albumName << ", m_songNameList = " << m_songNameList << LOG_ENDL;
        //MP_LOG << "m_albumName  =" << m_albumName << LOG_ENDL;
        //MP_LOG << "m_songNameList  =" << m_songNameList << LOG_ENDL;
    }

    if ( !filePaths.isEmpty() && !isFilePathValid( filePaths[0], device ) ) // modified by kihyung 2013.08.06 for ITS 0182926
    {
        return;
    }

    ResetPlaylistInfo(device);
    m_history[device].ContentPlayed = contentPlayed;

    foreach (QString file, filePaths)
    {
        file.remove( "file://" );
        m_history[device].playList.list->append(file);
    }

    if (m_pIMedia)
    {
        if (m_PlayerState != PLAYER_STOP)
        {
            m_pIMedia->Stop();
        }

        SetPlaylist(device);

        // { modified by kihyung 2013.08.28
        SetDefaultPlayMode(m_history[device].ContentPlayed);        
        emit playListReadyAutoStart();
        m_isCreatNewList = true;
        
        /*
        if ( m_isPrev )
        {
            setCurrentIndex(  m_songNameList.count() - 1 );
            m_isPrev = false;
        }

        if (!m_lastSong.isEmpty())
        {
            int m_songIndex = m_history[device].playList.list->indexOf(m_lastSong); //in new play list
            if (m_rndFlag == NONE)
            {
                //must work if repeat after random if end of file
                // { modified by kihyung 2012.07.07
                // if (m_repeatPlayMode == REPEATFOLDER &&
                //     m_songIndex+1 == m_history[m_device].playList.list->count())
                if (m_repeatPlayMode != REPEATFILE &&
                    m_songIndex+1 == m_history[device].playList.list->count())
                // } modified by kihyung
                {
                    setCurrentIndex(0);
                    return;
                }
                //must work if repeat after random if end of file
                else if (m_repeatPlayMode == REPEATFILE)
                {
                    setCurrentIndex(m_songIndex);
                    return;
                }
            }
            //must work if next pressed after random all & after end file after randon off
            else if (m_songIndex+1 == m_history[device].playList.list->count())
            {
                // for correct move by Next button if random changed to repeat
                setCurrentIndex(m_songIndex);
                m_lastSong = "";
                NextTrack();
                return;
            }

            setCurrentIndex(m_songIndex+1);
            m_lastSong = "";
        }
        else
        {
            emit playListReadyAutoStart();
        }
        */
        // } removed by kihyung 2013.08.28        
    }
	// m_isCreatNewList = true;  // removed by kihyung 2013.08.28        
    // m_rndFlag = REQ_NEW_LIST; // removed by kihyung 2013.08.28
}

// { added by junam for CR13012 2012.09.21
void AppMediaPlayer_Controller_Audio::getDepthOrderList(QString rootDir, QStringList &playList)
{
    Q_UNUSED(rootDir); Q_UNUSED(playList);
    // { removed by kihyung 2013.2.17
    /*
    QDir dir(rootDir);

    dir.setSorting( QDir::Name | QDir::IgnoreCase );
    dir.setFilter(QDir::Files | QDir::NoSymLinks);

    foreach(QFileInfo fileInfo, dir.entryInfoList(SUPPORTED_MIME_TYPE))
    {
        playList.append(fileInfo.absoluteFilePath());
    }

    dir.setFilter( QDir::Dirs | QDir::NoSymLinks);
    foreach(QFileInfo fileInfo, dir.entryInfoList())
    {
        if(fileInfo.fileName().startsWith('.'))
            continue;

        getDepthOrderList(fileInfo.absoluteFilePath(), playList);
    }
    */
    // } removed by kihyung 2013.2.17
}


void AppMediaPlayer_Controller_Audio::createPlaylistFromFolderTab(QStringList &filePaths, int nStartIdx)
{
    MP_LOG << "curFilePath =" << nStartIdx << LOG_ENDL;

    createPlayListFromList(filePaths, nStartIdx, MP::FOLDER); // added by kihyung 2013.3.12 for ISV 75638
}
// } added by junam
/* //20131010 delete search engine
void AppMediaPlayer_Controller_Audio::startPlayFromSearch(int searchCategory, QString url,
                                                          QString song, QString album, QString artist)
{
    MP_LOG << LOG_ENDL;

    QString keyword = m_pSearchEngine->getSearchString();
    QString searchItem = "";

    MP::DEVICE_T newSource = (MP::DEVICE_T)m_pSearchEngine->getVolumeId(url);
    ETrackerAbstarctor_VolumeType volumeType = AudioUtils::VolumeType(newSource);
    ETrackerAbstarctor_Query_Ids queryId = eDefault_Id;
    MP_LOG << "newSource =" << DeviceName(newSource) << LOG_ENDL;
    //{  add by yongkyun.lee@lge.com  2012.10.30 : : CR 13733  : Sound Path
    if( newSource != getDevice() )
        emit serch_TabClicked();
    //}  add by yongkyun.lee@lge.com 
    emit activateTab(newSource, true, true); // added by minho 20120821 for NEW UX: Added active tab on media
    emit signalCloseSearch();

    if (newSource == MP::UNDEFINED || volumeType == eDefaultVolume ||
       ((newSource == MP::IPOD1 || newSource == MP::IPOD2) && !getAppleDeviceStatus(newSource)))
    {
        MP_LOG << "Warning! Incorrect search data." << LOG_ENDL;
        return;
    }

    SetMediaMode(newSource);
    ResetPlaylistInfo(newSource);
    m_songNameList.clear();
    m_searchData.trackUrl = url;
    m_searchData.bIsSearch = true;

    m_searchData.matchField = (searchCategory != LVEnums::CATEGORY_ALL) ? -1 :
                               song.contains(keyword, Qt::CaseInsensitive) ? eTitle :
                               album.contains(keyword, Qt::CaseInsensitive) ? eAlbum :
                               artist.contains(keyword, Qt::CaseInsensitive) ? eArtist : -1;

    MP_LOG << "keyword =" << keyword << LOG_ENDL;
    MP_LOG << "matchField =" << m_searchData.matchField << LOG_ENDL;

    if (searchCategory == LVEnums::CATEGORY_SONG ||
       (searchCategory == LVEnums::CATEGORY_ALL && m_searchData.matchField == eTitle))
    {
        createPlayListFromList(QStringList(url), 0, MP::DEFAULT);
        m_searchData.bIsSearch = false;
        return;
    }
    else if (searchCategory == LVEnums::CATEGORY_ALBUM ||
            (searchCategory == LVEnums::CATEGORY_ALL && m_searchData.matchField == eAlbum))
    {
        searchItem = album;
        queryId = eGetAllSongsByAlbumFromVolumeQuery_Id;
    }
    else if (searchCategory == LVEnums::CATEGORY_ARTIST ||
            (searchCategory == LVEnums::CATEGORY_ALL && m_searchData.matchField == eArtist))
    {
        searchItem = artist;
        queryId = eGetAllSongsByArtistFromVolumeQuery_Id;
    }

    MP_LOG << "searchItem =" << searchItem << LOG_ENDL;
    MP_LOG << "queryId =" << MAP_QUERY_IDS.key(queryId) << LOG_ENDL;

    if (queryId != eDefault_Id && !searchItem.isEmpty())
    {
        m_pTrackerAbstractor->RequestData(searchItem, volumeType, queryId, GET_TA_REQUID(REQUID_DEVICE_ID));
    }
}
*/

void
AppMediaPlayer_Controller_Audio::createPlayListFromVR(QString SearchWord,QString SearchURL ,int isAlbum)
{
    Q_UNUSED(SearchWord); Q_UNUSED(SearchURL); Q_UNUSED(isAlbum);
    // { removed by kihyung 2013.2.17
    /*
    MP::DEVICE_T deviceT = (MP::DEVICE_T)m_pSearchEngine->getVolumeId (m_searchData.trackUrl );
    volumeT = AudioUtils::VolumeType(deviceT );// added by  yongkyun.lee 2012.11.21 for  xSampa check.

    MP_LOG << volumeT << deviceT << LOG_ENDL;
    
    ResetPlaylistInfo(deviceT);
    if (deviceT != m_device)
    {
        SetMediaMode(deviceT);
    }   
    m_songNameList.clear();
    m_albumName ="";
    m_enableErrorPopupState = ERROR_POPUP_READY;

    MP_LOG << "SearchWord =" << SearchWord << LOG_ENDL;
    MP_LOG << "SearchURL  =" << SearchURL << LOG_ENDL;
    MP_LOG << "isAlbum    =" << isAlbum << LOG_ENDL;
    MP_LOG << "volumeT    =" << volumeT << LOG_ENDL;
    
    if (m_pTrackerAbstractor != NULL)
    {
        if( isAlbum == eAlbum )
            m_pTrackerAbstractor->RequestData( SearchWord , volumeT , eGetAllSongsByAlbumFromVolumeQuery_Id, GET_TA_REQUID(REQUID_DEVICE_ID));
        else
            m_pTrackerAbstractor->RequestData( SearchWord , volumeT , eGetAllSongsByArtistFromVolumeQuery_Id, GET_TA_REQUID(REQUID_DEVICE_ID));
    }
    // } modified by yongkyun.lee@lge.com 
    */
    // } removed by kihyung 2013.2.17
}

// { add by yongkyun.lee@lge.com  2012.08.14  for : CR 11187
void
AppMediaPlayer_Controller_Audio::ID3InfoFromURL( QString SearchURL ,int isAlbum)
{
    Q_UNUSED(SearchURL); Q_UNUSED(isAlbum);
    // { removed by kihyung 2013.2.17
    /*
    MP::DEVICE_T deviceT;

    m_searchData.matchField = (isAlbum == 0x02) ? eAlbum : eArtist ; 
    m_searchData.trackUrl = SearchURL;
    m_searchData.bIsSearch = true;

    deviceT = (MP::DEVICE_T)m_pSearchEngine->getVolumeId (m_searchData.trackUrl );
    volumeT = AudioUtils::VolumeType(deviceT );

    MP_LOG << "SearchURL     =" << SearchURL << volumeT << LOG_ENDL;
    m_pTrackerAbstractor->RequestData( SearchURL, eGetMusicMetaDataQuery_Id, GET_TA_REQUID(REQUID_DEVICE_ID));
    */
    // } removed by kihyung 2013.2.17    
}
// } add by yongkyun.lee@lge.com 

// { added by eugene.seo 2013.04.10
void 
AppMediaPlayer_Controller_Audio::setLastPlayList()
{
	if(tempHistory.playList.list == NULL)
	{
		tempHistory.playList.list = new QList<QString>;
	}

	CopyHistory(tempHistory, m_history[getDevice()]);
}
// } added by eugene.seo 2013.04.10

/**
 * Create PlayList from selected album folder and start play it.
 */
// modified by Dmitry 23.04.13
void
AppMediaPlayer_Controller_Audio::createPlayListFromList(QStringList fileNameList, int index, MP::PLAYBACK_CONTENT_T contentPlayed)
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    MP_LOG << "TASCAN -> fileNameList.count() =" << fileNameList.count() << "index =" << index << "contentPlayed =" << contentPlayed << "device = "<< device <<LOG_ENDL;
	 
    bool isSameSong = false;
    if(fileNameList.count() == 0) // modified by ravikanth 24-04-13 to avoid delete all crash
    {
        return; // return if no playlist items
    }
    if(device == MP::IPOD1 || device == MP::IPOD2)
    {
       emit playListChanged();
       GetEngineMain().sendVRCancelSignal();
    }
    else if(fileNameList.size() > 0 && index < fileNameList.size())
    {
        if(fileNameList.at(index) == GetCurrentFilePath())
        {
            isSameSong = true;
        }
        else  if (m_device == MP::USB1 || m_device == MP::USB2 || m_device == MP::JUKEBOX)// moved by junam 2013.10.22 for same song selection
        {
            GetEngineMain().setMuteForSeek(true);
        }
    }

    // { added by eugene.seo 2013.04.10
    if (m_pIMedia && (device == MP::JUKEBOX || device == MP::USB1 || device == MP::USB2))
    {
        setLastPlayedIndex(m_pIMedia->GetCurrentIndex() );
        setLastPlayList();
    }
    // } added by eugene.seo 2013.04.10

    // { added by kihyung 2013.08.28 for ITS 0186131 
    bool bKeepRepeatMode = false;
    if(m_history[device].ContentPlayed == contentPlayed && isSameSong == true) 
    {
        bKeepRepeatMode = true; 
    }
    MP_LOG << m_history[device].ContentPlayed << contentPlayed << isSameSong << LOG_ENDL;
    // } added by kihyung 2013.08.28 for ITS 0186131 
    
    if(device == MP::IPOD1 || device == MP::IPOD2) //added by junam 2013.08.17 for iPod empty data
        ResetPlaylistInfo(device, false);
    else
        ResetPlaylistInfo(device, isSameSong == false); // changed by junam 2013.08.29 for ITS_KOR_187291

    m_songNameList.clear();
    m_albumName = "";
    m_enableErrorPopupState = ERROR_POPUP_READY;
    
    m_history[device].ContentPlayed = contentPlayed;

    foreach (QString file, fileNameList)
    {
        file.remove( "file://" );
        m_history[device].playList.list->append( file );
    }
    if (m_pIMedia)
    {
        if (!isSameSong && m_PlayerState != PLAYER_STOP)
        {
            // { modified by edo.lee 2013.01.23
            if(device == MP::DISC && getDiscType() == MP::AUDIO_CD) //added by Michael.Kim 2013.05.22 for New OSD Implementation
            {
                MP_LOG<<"no action because of Disc" << LOG_ENDL;
            }else
            {               
                m_pIMedia->Stop();
            }
			//}  modified by edo.lee 2013.01.23

        }

        // m_rndFlag = REQ_NEW_LIST; // removed by kihyung 2013.08.28

        SetPlaylist(device, index);
        if (isSameSong)
        {
            emit updateSongsCount();

            //{added by junam 2013.11.20 for ITS_NA_209916
            if (m_pIMedia && m_history[device].ContentPlayed == MP::FOLDER)
            {
                int fileIndex = -1, fileCount = -1;
                m_pIMedia->GetCurrentFileFolderIndex(&fileIndex, &fileCount);
                emit trackClusterChanged(fileIndex + 1, m_history[device].Quantity, false); //changed by junam 2013.12.14 for ITS_NA_215663
            }
            else
            {
                emit trackClusterChanged(index + 1, m_history[device].Quantity, false); //changed by junam 2013.12.14 for ITS_NA_215663
            }
            //}added by junam
            m_moveTrackIndex = index; //added by junam 2013.12.17 for ITS_EU_215850
        }

        SetDefaultPlayMode(m_history[device].ContentPlayed, bKeepRepeatMode); // modified by kihyung 2013.08.28 for ITS 0186131 

        if(!isSameSong && m_history[device].playList.list->size() > 0)
        {
            emit playListReadyAutoStart();
        }
        else if (isSameSong && m_history[device].playList.list->size() > 0)
        {
            //{added by junam 2013.08.18 for iPod normal play
            if(device == MP::IPOD1 || device == MP::IPOD2)
            {
                if (!cancelFFRW())
                {
                    setRepeatRandomMode(REPEATALL, RANDOMOFF); //added by junam 2013.09.04 for ITS_KOR_186005
                    NormalPlay();
                }
            }
            else //}added by junam
            {
            // { modified by kihyung 2013.08.14 to remove zero position display issue.
            // if (!cancelFFRW()) NormalPlay();
            cancelFFRW();
            // { modified by kihyung 2013.08.14 to remove zero position display issue.
        }
    }
    }

    // added by edo.lee 2013.06.09
    if(isSameSong)
    {
        MediaInfo pInfo;
        GetCurrentMediaInfo(&pInfo);
        m_history[device].Title = pInfo.Title;
        GetEngineMain().GetNotifier()->setHistoryInfo(AudioUtils::DeviceType(device),m_history[device].Title,m_history[device].Folder,m_history[device].Position);
    }
    else {
    	m_isCreatNewList = true;	
        // { modified by kihyung 2013.10.22 for HMC P-P2-4-15
        if(m_pIMedia && device != MP::IPOD1 && device != MP::IPOD2) 
        {
            QString curFilePath    = m_pIMedia->GetCurrentFileName();
            int     lastSlashIndex = curFilePath.lastIndexOf('/');
            if(lastSlashIndex < 0) {
                lastSlashIndex = 0;
            }
            m_history[device].Title  = curFilePath.mid(lastSlashIndex + 1);
            m_history[device].Folder = getFolderName(curFilePath);
        }
        EmitOnMediaInfoChanged(device, false);
        // } modified by kihyung 2013.10.22 for HMC P-P2-4-15
    }
    // added by edo.lee 2013.06.09    
}
//{changed by junam 2013.07.22 for ONDEMAND_PLAYLIST
void AppMediaPlayer_Controller_Audio::createEmptyPlayList(MP::PLAYBACK_CONTENT_T contentPlayed, int index,  int index0)
{
    MP_LOG << "contentPlayed = "<<contentPlayed <<  "index =" << index << " index0 = "<<index0 << LOG_ENDL;

    MP::DEVICE_T device = getDevice();

    if(device != MP::IPOD1 && device != MP::IPOD2)
    {
        MP_LOG<<"This function is only for iPod" << LOG_ENDL;
        return;
    }
    
    GetEngineMain().sendVRCancelSignal();

    emit playListChanged(); // { added by wspark 2013.04.21 for cluster IOT

    //emit clearTpMessage(); // commented by cychoi 2013.08.17 for No clear TP message from HMC request // added by wspark 2013.04.17

    ResetPlaylistInfo(device, false); //add false by junam 2013.08.17 for iPod empty data
    m_songNameList.clear();
    m_albumName = "";
    m_enableErrorPopupState = ERROR_POPUP_READY;

    m_history[device].ContentPlayed = contentPlayed;

    if (m_pIMedia)
    {
        if (m_PlayerState != PLAYER_STOP)
        {
            m_pIMedia->Stop();
        }

        // m_rndFlag = REQ_NEW_LIST; // removed by kihyung 2013.08.28

        m_history[device].Index    = index;
        m_history[device].Quantity = m_history[device].playList.list->count();
        if ( m_enableErrorPopupState == ERROR_POPUP_READY )
            m_enableErrorPopupState = ERROR_POPUP_ENABLE;

        m_history[device].playList.category = contentPlayed;
        m_history[device].playList.index    = index; // added by junggil 2012.08.13
        m_history[device].playList.mode = index0;
        m_history[device].playList.isMLT    = false;
        m_history[device].playList.position = m_history[device].Position; // added by kihyung 2013.2.12
        
        m_pIMedia->SetPlayList(&m_history[device].playList);
        m_pIMedia->SetCurrentIndex(index);

        AppMediaPlayer_SettingsStorage::GetInstance()->SavePlaylist(this); // added by kihyung 2013.05.22
        emit playListReadyAutoStart();
    }
}
//}changed by junam

//Tan 2013.11.12 added resumeNormalPlay parameter to cancelFFRW functionality. All other logic remain the same.
bool AppMediaPlayer_Controller_Audio::cancelFFRW(bool resumeNormalPlay)
{
    MP_LOG << LOG_ENDL;
    if(isLongPress())
    {
        //{ modified by yongkyun.lee 2013-08-31 for : 
        if(isLongPress()== -1 ) //getPlayState() >= PLAYER_REWIND_X4 || getPlayState() == PLAYER_REWIND)
            emit changeFastFowardRewind(false , AppEngineQMLConstants::KEY_STATUS_CANCELED ,AppEngineQMLConstants::JOG_LEFT ); // modified by sangmin.seol 2014.01.28 cancelFFRW
        else if(isLongPress()== 1)
            emit changeFastFowardRewind(false , AppEngineQMLConstants::KEY_STATUS_CANCELED ,AppEngineQMLConstants::JOG_RIGHT ); // modified by sangmin.seol 2014.01.28 cancelFFRW
        //} modified by yongkyun.lee 2013-08-31
// removed by Dmitry 13.10.13
        clearAllJog(); // added by suilyou ITS 0206832
        if(resumeNormalPlay)
        {
            NormalPlay();
        }
        return true;
    }
    return false;
}

void AppMediaPlayer_Controller_Audio::setFFRW(bool isFFRew, int status, int arrow)
{
    MP_LOG << LOG_ENDL;
   emit changeFastFowardRewind(isFFRew, status, arrow);
}
// modified by Dmitry 23.04.13

void
AppMediaPlayer_Controller_Audio::SetPlaylist(MP::DEVICE_T device, int startIndex)
{
    MP_HIGH << device << startIndex << LOG_ENDL;

    if(device >= MP::ALL_DEVICES) return;  // added by kihyung 2013.08.08 for ITS 0181967 
    
    if (m_pIMedia)
    {
        m_history[device].Index    = startIndex;
		//m_moveTrackIndex = startIndex;//added by edo.lee 2013.05.09
        m_history[device].Quantity = m_history[device].playList.list->count();
        if( startIndex < m_history[device].Quantity) //added by junam 2013.05.02
        {
            m_history[device].Path = m_history[device].playList.list->at(startIndex); // added by Dmitry 20.04.13 for ISV77651
        }
        if ( m_enableErrorPopupState == ERROR_POPUP_READY )
            m_enableErrorPopupState = ERROR_POPUP_ENABLE;

        if(m_history[device].ContentPlayed == MP::FOLDER) {
            m_history[device].playList.category = 1;
        }
        else {
            m_history[device].playList.category = 0;
        }
        m_history[device].playList.index    = startIndex; // added by junggil 2012.08.13
        // m_history[device].playList.isMLT = m_isPlayFromMLT; //added by jeeeun 2012.12.08 for MLT initializing // removed by kihyung 2013.07.18 for ITS 0180268
        m_history[device].playList.position = m_history[device].Position; // added by kihyung 2013.2.12
        if(getDevice() == device) { // modified by ravikanth 11-05-13
            updateFSFolderList(device);  // added by kihyung 2013.08.08 for ITS 0181967 
            m_pIMedia->SetPlayList(&m_history[device].playList);
        }

        // { modified by kihyung 2013.4.15. It is unnecessary for USB, DISC.
        if(device == MP::IPOD1 || device == MP::IPOD2) {
            // { modified by jungae, 2013.02.06 for ITS154401
            //if (startIndex > 0 && startIndex < m_history[device].Quantity)
    		if (startIndex >= 0 && startIndex < m_history[device].Quantity) // } modified by jungae
            {
                if(getDevice() == device) {// modified by ravikanth 11-05-13
                    m_pIMedia->SetCurrentIndex(startIndex);
                }
            }
        }
        // } modified by kihyung 2013.4.15      

        AppMediaPlayer_SettingsStorage::GetInstance()->SavePlaylist(this); // added by kihyung 2013.05.22
    }
    else
    {
        MP_HIGH << "m_pIMedia is NULL" << LOG_ENDL;
    }
}

void
AppMediaPlayer_Controller_Audio::FastForward(MEDIA_FAST_SPEED speed)
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    MP_LOG << speed << LOG_ENDL;
    //{ modified by hyochang.ryu 20130620
    if( device == MP::BLUETOOTH )
    {
        setIsLongPress(0);
        cancelFFRW();
        return;	//added by hyochang.ryu 20130523 for Protecting Side Effect
    }
    //if( m_device != MP::BLUETOOTH )	//added by hyochang.ryu 20130520 for BT Mute
    //} modified by hyochang.ryu 20130620

    //Added if condition for mute setting by Tan 2013.09.02
    if( device != MP::IPOD1 && device != MP::IPOD2)
    {
        GetEngineMain().setMuteForSeek(true); //added by edo.lee 2013.04.21 for Mute
    }
    //m_isLongPressed = 1; // removed by kihyung 2013.2.5 for ISV 72327

    //Added unmute flag by Tan 2013.09.02
    if((device == MP::IPOD1 || device == MP::IPOD2) && GetEngineMain().GetMuteState())
    {
        MP_LOG<< "IPOD setting m_WillUnMute true"  << LOG_ENDL;
        m_WillUnMute = true;
    }


    setIsLongPress(1); // added by dongjin 2013.03.20 for ISV 74906
    setScanMode(SCANOFF); // changed by eugeny - 12-09-15

    if (m_pIMedia)
    {
// modified by edo.lee 2013.10.12 pop noise
        if((m_device == MP::USB1 || m_device == MP::USB2 || m_device == MP::JUKEBOX) && speed == FAST_SPEED_X4)
    	{
    	    m_bFF = true;
		    startLongBtnStart();
        }
        // { modified by wonseok.heo for ITS 197433 2013.10.28
        else if(m_device == MP::DISC && m_discType == MEDIA_SOURCE_CDDA && GetCDDAGraceNote() != GRACENOTE_INDEXING_DONE)
        {
            MP::DEVICE_T source = GetSignalSource();
            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->updateCDDAOSDMetaData((OSD_DEVICE)getOSDevice(device), QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_history[source].Title));
            m_pIMedia->FastForward(speed);
            emit UpdateStateTrackOSD(true);
        }
        else
        {
            m_pIMedia->FastForward(speed);
            emit UpdateStateTrackOSD(true); // added by yungi 2012.11.14 for ISV 62667,ISV 62596
        }
        // } modified by wonseok.heo for ITS 197433 2013.10.28
    }

    //{added junam 2013.07.24 for iPOD FF/RW icon
    if( device == MP::IPOD1 || device == MP::IPOD2 )
    {
        MP_LOG<<"IPOD dose not change speed icon" << LOG_ENDL;
    }
    else //}added by junam
    {
        emit setSpeedIcon(speed);
    }
}

void
AppMediaPlayer_Controller_Audio::Rewind(MEDIA_FAST_SPEED speed)
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    MP_LOG << speed << LOG_ENDL;
    //{ modified by hyochang.ryu 20130620
    if( device == MP::BLUETOOTH )
    {
        setIsLongPress(0);
        cancelFFRW();
        return;	//added by hyochang.ryu 20130523 for Protecting Side Effect
    }
    //if( m_device != MP::BLUETOOTH )	//added by hyochang.ryu 20130520 for BT Mute
    //} modified by hyochang.ryu 20130620

    //Added if condition for mute setting by Tan 2013.09.02
    if(device != MP::IPOD1 && device != MP::IPOD2)
    {
        if( speed == FAST_SPEED_X4 )// modified by yongkyun.lee 2013-09-05 for : ITS 187389
            GetEngineMain().setMuteForSeek(true); //added by edo.lee 2013.04.21 for Mute
    }

    //Added unmute flag by Tan 2013.09.02
    if((device == MP::IPOD1 || device == MP::IPOD2) && GetEngineMain().GetMuteState())
    {
        MP_LOG<< "IPOD setting m_WillUnMute true"  << LOG_ENDL;
        m_WillUnMute = true;
    }


    // m_isLongPressed = -1; // removed by kihyung 2013.2.5 for ISV 72327
    setIsLongPress(-1); // added by dongjin 2013.03.20 for ISV 74906
    setScanMode(SCANOFF); // changed by eugeny - 12-09-15

    if (m_pIMedia)
    {
// modified by edo.lee 2013.10.12 pop noise
        if((m_device == MP::USB1 || m_device == MP::USB2 || m_device == MP::JUKEBOX) && speed == FAST_SPEED_X4)
        {
            m_bFF= false; // modified by Dmitry 13.10.13
            startLongBtnStart();
        }
        // { modified by wonseok.heo for ITS 197433 2013.10.28
        else if(m_device == MP::DISC && m_discType == MEDIA_SOURCE_CDDA && GetCDDAGraceNote() != GRACENOTE_INDEXING_DONE)
        {
            MP::DEVICE_T source = GetSignalSource();
            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->updateCDDAOSDMetaData((OSD_DEVICE)getOSDevice(device), QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_history[source].Title));
            m_pIMedia->Rewind(speed);
            emit UpdateStateTrackOSD(true);
        }
        else
        {
            m_pIMedia->Rewind(speed);
            emit UpdateStateTrackOSD(true); // added by yungi 2012.11.14 for ISV 62667,ISV 62596
        }
        // } modified by wonseok.heo for ITS 197433 2013.10.28
    }

    //{added junam 2013.07.24 for iPOD FF/RW icon
    if( device == MP::IPOD1 || device == MP::IPOD2 )
    {
        MP_LOG<<"IPOD dose not change speed icon" << LOG_ENDL;
    }
    else //}added by junam
    {
        emit setSpeedIcon(speed);
    }
}

void AppMediaPlayer_Controller_Audio::PlayiPodFiles()
{

    MP::DEVICE_T device = getDevice();


    MP_HIGH<<"11PlayiPodFiles"<< device<<LOG_ENDL;
    if(device != MP::IPOD1 && device != MP::IPOD2)
    {
        MP_HIGH<<"22PlayiPodFiles"<< device<<LOG_ENDL;
        return;
    }
    MP_HIGH<<"33PlayiPodFiles"<< device<<LOG_ENDL;
    CIpodController* controller = GetIPodController(device);
    if(controller)
    {
        MP_HIGH<<"44PlayiPodFiles"<< device<<LOG_ENDL;
        controller->PlayiPodFiles();
    }
}

void
AppMediaPlayer_Controller_Audio::NormalPlay()
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;

    // moved by sangmin.seol 2014.04.23 ITS 0235538, 0235541 setIsLongPress(0) when AV_OFF & Power_OFF
    setIsLongPress(0);

    // SUILYOU sound MUTE
    if(m_pLongBtnTimer->isActive())
    {
        GetEngineMain().setUnMuteState(true);
        m_pLongBtnTimer->stop(); // added by Dmitry 13.10.13
    }
    // { added by kihyung 2013.08.20 for ITS 0183782 
    if(/*GetEngineMain().isACCOff() == true ||*/    //{ modified by yongkyun.lee 2013-09-12 for : ITS 189380
       GetEngineMain().isAVOff() == true  ||
       GetEngineMain().isPowerOff() == true) 
    {
        MP_HIGH << GetEngineMain().isACCOff() << GetEngineMain().isAVOff() << GetEngineMain().isPowerOff() << LOG_ENDL;
        return;
    }
    // } added by kihyung 2013.08.20 for ITS 0183782 
    
    MP_LOG << "device = " << device <<LOG_ENDL; // added by kihyung 2012.08.04
    //{added by junam 2013.12.13 for ITS_EU_214770
    if(device == MP::USB1 || device == MP::USB2)
    {
        if(GetEngineMain().GetMediaManager()->GetID(AudioUtils::DeviceType(device)).isEmpty())
        {
            MP_HIGH << "USB device " << device << "is unplugged..." << LOG_ENDL;
            return;
        }
    }
    //}added by junam

    //setIsLongPress(0); // added by dongjin 2013.03.20 for ISV 74906 //modified by Michael.Kim 2013.05.10 for ITS Issue #0167251 // restored by Sergey 14.05.2013 for cancel 20x when jump to next track
    // { modified by kihyung 2012.08.04 
    // for CR 12585 - [3PDMS 132371] [Domestic] Toggle error with BT music Play /Pause
    /*
    if( m_device == MP::BLUETOOTH )
       return;
    */
    if( device == MP::BLUETOOTH ) {
        MP_MEDIUM << " emit resetPlayIconBT()" << LOG_ENDL;
        emit resetPlayIconBT();
        return;
    }
    // } modified by kihyung 

    if (m_pIMedia)
    {
        m_pIMedia->RepeatPlay(m_repeatPlayMode);
        //{ modified by shkim 2013.11.13 for CDDA Scan Off failure Smoke Test Issue
        if(getDevice() == MP::DISC && m_discType == MEDIA_SOURCE_CDDA)
        {
            CDDAStatusResponse prevCDStatus, newCDStatus;
            m_pIAudio->GetCDStatus(&prevCDStatus);
            m_pIMedia->RandomPlay(m_randomPlayMode);
            m_pIAudio->GetCDStatus(&newCDStatus);
            if((m_randomPlayMode == RANDOMFILE) &&
               (prevCDStatus.scanModeStatus == 0x01 || prevCDStatus.scanModeStatus == 0x04) &&
               (newCDStatus.scanModeStatus == 0xFF))
            {
                emit stopScan();
            }
        }
        else
        {
            m_pIMedia->RandomPlay(m_randomPlayMode);
        }
        //} modified by shkim 2013.11.13
        m_pIMedia->Play();
        
        if( device != MP::IPOD1 && device != MP::IPOD2 )//added by junam 2013.05.18 for ISV_KR81848
        {
            emit UpdateStateTrackOSD(false); // added by yungi 2012.11.14 for ISV 62667,ISV 62596
        }

        // { added by eugeny.novikov 2012.10.04 for CR 14121
        if (!isPlayerActive())
        {
            m_pIMedia->Pause();
        }
        // } added by eugeny.novikov

        emit resetSpeedIcon();
    }
}

void
AppMediaPlayer_Controller_Audio::setCurrentIndex( int index )
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;

    MP_LOG << index << GetMediaNum() << LOG_ENDL;

    // { added by wonseok.heo 2013.07.04 disc in out test
    // { modified by cychoi 2013-07-15 for Prevent fix
    if(m_pIMedia && GetEngineMain().getInOutTestMode() == true && device == MP::DISC){
    //if(GetEngineMain().getInOutTestMode() == true && m_device == MP::DISC){
    // } modified by cychoi 2013-07-15
        //m_pIMedia->Stop();
        DeckEngineerMode  *pDeckEngine = GetEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();

        m_pDeckController = GetControllers().GetDeckController();
        m_pIMedia->SetCurrentMediaPosition(0);
        m_pIMedia->SetCurrentIndex( index );
        pDeckEngine->DiscInOutTest();
        //m_pIMedia->DiscTestPlay ();
        m_history[device].Index    = index;
        m_history[device].Position = 0;
        return;
    }

    // { modified by kihyung 2013.2.13
    // { modified by cychoi 2013-07-15 for Prevent fix
    if (m_pIMedia && index >= 0 && (index < GetMediaNum() || (device == MP::DISC && m_discType == MEDIA_SOURCE_CDDA))) //modified by Michael.Kim 2013.05.24 for Song Index Count Error
    //if (m_pIMedia && index >= 0 && index < GetMediaNum() || device == MP::DISC && m_discType == MEDIA_SOURCE_CDDA) //modified by Michael.Kim 2013.05.24 for Song Index Count Error
    // } modified by cychoi 2013-07-15
    {
        if(device != MP::DISC) //added by Michael.Kim 2013.05.04 for ISV Issue #78429
        {
            m_pIMedia->Stop();
            if(m_history[device].Index != index
                    && device != MP::IPOD1 && device != MP::IPOD2)  //Added by junam 2013.04.19 for iPod
            {
                m_pIMedia->SetCurrentMediaPosition(0);
                emit positionChanged(0); // added by kihyung 2013.09.10 for ITS 0188383 
            }
        }
        // emit clearTpMessage(); // added by wspark 2013.04.09 for cluster IOT #37 // deleted by wspark 2013.04.17
        if(m_pIMedia)
        {
            m_pIMedia->SetCurrentIndex( index );
            m_pIMedia->Play ();
        }

        m_history[device].Index    = index;		
        m_history[device].Position = 0; 
		//m_moveTrackIndex = index;//added by edo.lee 2013.05.09

        // { added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
        //if(m_pIMedia != NULL)//Added By Taihyun.Ahn 2013.10.05
        //{
        //    if (m_pIMedia && m_history[device].ContentPlayed == MP::FOLDER)
        //    {
        //        int fileIndex = -1;
        //        int fileCount = -1;
        //        m_pIMedia->GetCurrentFileFolderIndex(&fileIndex, &fileCount);
        //        //{ modified by cychoi 2013.08.29 for HMC QC IPOD cluster display on Bootup
        //        emit mediaInfoClusterChanged(m_history[device].Title);
        //        emit trackClusterChanged(fileIndex + 1, m_history[device].Quantity);
                //} modified by cychoi 2013.08.29
        //    }
        //    else
        //    {
        //        int trackIndex = GetCurrentTrackNumber();
                //{ modifiedby wonseok.heo for ITS 197433 2013.10.28
                //{ modified by cychoi 2013.08.29 for HMC QC IPOD cluster display on Bootup                
        //        if(GetCDDAGraceNote() == GRACENOTE_INDEXING_DONE){
        //            emit mediaInfoClusterChanged(m_history[device].Title);
        //            emit trackClusterChanged(trackIndex, m_history[device].Quantity);
        //        }else{
        //            emit mediaInfoClusterChanged(QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_history[device].Title));
        //            emit trackClusterChanged(trackIndex, m_history[device].Quantity);
        //        }
                //} modified by cychoi 2013.08.29
                //} modified by wonseok.heo for ITS 197433 2013.10.28
        //    }
        //}
        // } added by cychoi 2013.08.17
    }
    // } modified by kihyung 2013.2.13    
}

/*
//{added by aettie.ji 2012.12.18 for new ux
void
AppMediaPlayer_Controller_Audio::setCurrentCategory( QString tabId )
{
	m_history[m_device].curCat = tabId;
}
//}added by aettie.ji 2012.12.18 for new ux
*/
void
AppMediaPlayer_Controller_Audio::HandleRequestComplete (QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID) // add reqUID by junam 2012.10.25 for CR14451
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    MP_LOG << LOG_ENDL;
    if (device == MP::JUKEBOX || device == MP::USB1 || device == MP::USB2 ||
        device == MP::DISC || device == MP::IPOD1 || device == MP::IPOD2)
    {
        RequestComplete(fileList, queryId, reqUID);// add reqUID by junam 2012.10.25 for CR14451
    }
	// { modified by ravikanth 20-04-13
    if(device == MP::JUKEBOX) // modified by ravikanth 28-04-13
    {
        GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->ResetProgressLoading();
    }
	// } modified by ravikanth 20-04-13
}

//{added by junam 2013.04.13 for AutoStart
void AppMediaPlayer_Controller_Audio::HandleRequestComplete( QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId, int offset, uint reqUID )
{ 
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    MP_HIGH << "FORIPOD reqUID = " << reqUID << LOG_ENDL;
    //{ changed by junam 2013.05.20 for OnDemandPlayList
    //if ( m_device == MP::IPOD1 || m_device == MP::IPOD2 )
    //    RequestComplete(dataList, queryId, reqUID);
    
    if(device != MP::IPOD1 && device != MP::IPOD2)
        return;

    //{added by junam 2013.07.23 for ONDEMAND_PLAYLIST
    switch(queryId)
    {
    case eGetAllSongsbyPlaylistQuery_Id:
    case eGetlistofPlaylistsQuery_Id:
        m_pListViewModel->partialRequestComplete(dataList, queryId, offset, reqUID);
        return;
    default:
    {
    	// 
    }
    }
    //}added by junam


    if( reqUID >= 3000 ) //REQUID_LIST_DEVICE_ID is 3000
    {
        MP_MEDIUM << "FOR LIST" << LOG_ENDL;
        m_pListViewModel->ListRequestComplete(dataList, queryId, reqUID);
    }
    else
    {
        MP_MEDIUM << "FOR BASIC" << LOG_ENDL;
        RequestComplete(dataList, queryId, reqUID);
    }
    //}changed by junam
}
//}addded by junam

void
AppMediaPlayer_Controller_Audio::HandleRequestCompleteWithVolumeType (QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType, uint reqUID)
{   
    MEDIA_SOURCE_T source = AudioUtils::DeviceType(volumeType);
    MP::DEVICE_T device = AudioUtils::DeviceType(source);

    MP_LOG << DeviceName(device) << fileList.count() << LOG_ENDL;

    switch (queryId)
    {
    case eGetMusicLimitFileListDir_Id:
    {
        MP_LOG << "TASCAN -> eGetMusicLimitFileListDir_Id" << reqUID << LOG_ENDL;

        //{added by junam 2013.11.16 for coverflow click
        if(reqUID == REQUID_COVERFLOW_CLICK )
        {
            if(fileList.isEmpty())
            {
                MP_HIGH<<"fileList is empty" << LOG_ENDL;
            }
            else
            {
                QStringList fileNameList;
                foreach (QStringList file, fileList)
                {
                    foreach(QString item, file)
                    {
                        fileNameList.append(item);
                    }
                }

                if(m_coverflowClickedItem.startsWith("file://"))
                    m_coverflowClickedItem.remove(0, 7);
                int iIndex = fileNameList.indexOf(m_coverflowClickedItem);
                m_coverflowClickedItem.clear();                
                MP_LOG<<"iIndex = "<<iIndex << " of "<<fileNameList.size()  << LOG_ENDL;
                if(iIndex >= 0)
                {
                    saveCategoryTab("Folder");
                    createPlayListFromList( fileNameList, iIndex, MP::FOLDER );
                    emit showPlayerView();//added by junam 2013.11.28 for ITS_NA_211655
                }
                else
                {
                    MP_HIGH<<"cannot find current play song" << LOG_ENDL;
                }
            }
            break;
        }
        //}added by junam
        
        if(reqUID == REQUID_ALL_FILELIST ||
           reqUID == REQUID_ALL_FILELIST_RESCAN ||  // { added by kihyung 2013.07.09 for ITS 0177427
           reqUID == REQUID_ALL_FILELIST_RESCAN_COPY ||	// added by sangmin.seol 2013.10.22 JUKEBOX PlayerView Counter Update after copy from USB
           reqUID == REQUID_ALL_FILELIST_HKMC) 
        {
            QStringList songList;

            foreach (QStringList file, fileList)
            {
                for (int i = 0; i < file.count(); i++)
                {
                    // MP_LOG << file[i] << LOG_ENDL;
                    songList.append(file[i]);
                }
            }

            MP_LOG << "TASCAN -> eGetMusicLimitFileListDir_Id playlist size:" << songList.count() << LOG_ENDL;

            // { modified by kihyung 2013.11.06 for ITS 0206863.
            MP::PLAYBACK_CONTENT_T contentPlayed;
            #if USE_FILE_INDEX_IN_FOLDER
            if(reqUID == REQUID_ALL_FILELIST_HKMC) {
                contentPlayed = MP::DEFAULT;
            } else {
                contentPlayed = MP::FOLDER;
            }
            #else
                contentPlayed = MP::FOLDER;
            #endif

            bool bRescan = false;
            bool bResetPlaylist = true;
            bool bPlayAfterStop = false;
			// { modified by sangmin.seol 2013.10.22 JUKEBOX PlayerView Counter Update after copy from USB
            if(reqUID == REQUID_ALL_FILELIST_RESCAN || reqUID == REQUID_ALL_FILELIST_RESCAN_COPY) {
                bRescan = true;
                int nRetVal;

                if(reqUID == REQUID_ALL_FILELIST_RESCAN)
                {
                    nRetVal = updateDevicePlayList(device, songList);
                }
                else
                {
                    nRetVal = updateDevicePlayList(device, songList, true);
                }
                // } modified by sangmin.seol 2013.10.22 JUKEBOX PlayerView Counter Update after copy from USB

                //{added by junam 2014.01.03 for ITS_KOR_196056
                if(nRetVal == 3)
                    break;
                //}added by junam

                if(nRetVal != 0) { // 0 is no playlist.
                    bResetPlaylist = false;
                }
                if(nRetVal == 2) { // 2 is no playing file.
                    bPlayAfterStop = true;
                }
            }

            if (!songList.isEmpty())
            {
                MP_HIGH << "bResetPlaylist:" << bResetPlaylist << "bPlayAfterStop:" << bPlayAfterStop << LOG_ENDL;
                if(bResetPlaylist == true) {
                    if(createDevicePlayList(device, songList, contentPlayed, bRescan, 0) == false) {
                        bPlayAfterStop = true;
                    }
                }
                
                if(getDevice() == device) {
                    RestoreFromHistoryAfterTAScan(volumeType, device, bRescan, bPlayAfterStop);
                    emit updateSongsCount(); // added by sungha.choi 2013.09.06 for ISV 90758 ITS 0188052
                }
            }
            // } modified by kihyung 2013.11.06 for ITS 0206863.
        }
        break;
    }
    
    default:
        break;
    }
	// { modified by ravikanth 20-04-13
    if(volumeType == eJukeBox)
    {
        GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->ResetProgressLoading();
    }
	// } modified by ravikanth 20-04-13
}

void
AppMediaPlayer_Controller_Audio::RequestComplete( QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID)// add reqUID by junam 2012.10.25 for CR14451
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    //MP_LOG << "=================================================" << LOG_ENDL;
    MP_HIGH << "queryStr =" << MAP_QUERY_IDS.key(queryId) << ", queryId =" << queryId << ", reqUID = " << reqUID << "m_device = " << DeviceName(device) << "fileList.count =" << fileList.count() << LOG_ENDL;
    //MP_HIGH << "m_device = " << DeviceName(device) << "fileList.count =" << fileList.count() << LOG_ENDL;

    if (!fileList.isEmpty())
    {
        switch ( queryId )
        {
            case eGetAllsongsByKeywordFromVolumeQuery_Id:
            case eGetAllArtistsByKeywordFromVolumeQuery_Id:
            case eGetAllAlbumsByKeywordFromVolumeQuery_Id:
            {
                QString  FindSong;

                m_songNameListTemp.clear();
                m_pathsTemp.clear();
                {
                    foreach (QStringList file, fileList )
                    {
                        FindSong = encodeString(file[4]);
                        m_songNameListTemp.append(FindSong);
                        m_pathsTemp.append(FindSong);
                    }
                }

                if(m_searchData.bIsSearch == true && \
                    ( queryId == eGetAllArtistsByKeywordFromVolumeQuery_Id ||  queryId == eGetAllAlbumsByKeywordFromVolumeQuery_Id ))
                {
                    int plIndex = (m_searchData.matchField == eAlbum) ? m_pathsTemp.indexOf(m_searchData.trackUrl) : 0;
                    createPlayListFromList( m_pathsTemp, plIndex, MP::DEFAULT );
                    m_searchData.bIsSearch = false;
                }
                else
                {
                    createPlayList(m_pathsTemp, MP::DEFAULT);
                }
                break;
            }
            //{changed by junam 2013.03.26 for ISV72425
            //case eGetMusicMetaDataQuery_Id:
            //case eTrackerExtractMetadataQuery_Id:   // added by sungha.choi 2013.07.01    // deleted by sungha.choi 2013.07.13
            case eGetMusicSimpleMetaDataQuery_Id:
            //}changed by junam
            {
                if (fileList[0].isEmpty()) return;

                if(reqUID == GET_TA_REQUID(REQUID_ROTATE_TUNE))
                {
                    //{changed by junam 2013.04.13 for AutoStart
                    //if((m_tuneState.idx_tune < 0) || (m_tuneState.idx_tune >= m_history[m_device].playList.list->size()))
                    if((m_tuneState.idx_tune < 0) || m_tuneState.idx_tune >= m_history[device].Quantity)
                        break;
                    //}changed by junam
                    //{added by junam 2013.03.10 for ISV73458
                    m_tuneMetaInfoTimer.stop();
                    //m_tuneCoverArtTimer.stop();//removed by junam 2013.10.17 for deprecated code
                    //}added by junam

                    //{added by junam 2013.06.28 for ISV86231
                    if(m_tuneState.m_nTuneStart == false)
                    {
                        MP_LOG<<"tune search already stopped" << LOG_ENDL;
                        break;
                    }
                    //}added by junam
                    m_tuneTitle =  fileList[0][0];
                    emit tuneSameFileNoti(m_tuneState.idx_tune == m_history[device].Index);

                    EmitOnTuneMetaData( (fileList[0][1] != "") ? fileList[0][1] :"Unknown", //artist
                                      m_tuneTitle, //song,
                                      (fileList[0][2] != "")? fileList[0][2] : "Unknown", //album,
                                      getFolderName(fileList[0][3]), //folder
                                      m_tuneState.idx_tune != m_history[device].Index ); // modified by kihyung 2013.08.27 for ITS 0186708

                    // removed by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
					
					GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData(
								(OSD_DEVICE)getOSDevice(device), m_tuneTitle, m_randomPlayMode, m_repeatPlayMode, m_scanMode, true);
					
                    if(m_history[device].Quantity >0)//{ modified by yongkyun.lee 2013-11-06 for : smoke test mp3cd
                        emit tuneCounter(GetFilesCountInfo( m_tuneState.idx_tune, m_history[device].Quantity));//added by junam 2013.04.05 for count sync

                    //{changed by junam 2013.03.10 for ISV73458
                    if (m_bIsBasicView)
                    {
                        if(m_tuneState.idx_tune == m_nPendingMedia)
                        {
                            m_tuneState.coverRequestIndex = m_tuneState.idx_tune;
                            m_tuneState.coverRequestAlbum = fileList[0][2];
                            m_tuneState.coverRequestArtist = fileList[0][1];
                            //{changed by junam 2013.03.26 for ISV72425
                            //m_tuneState.coverRequestURI = fileList[0][4];
                            m_tuneState.coverRequestURI = fileList[0][3];
                            //}changed by junam
                            //m_tuneCoverArtTimer.start(TUNE_TIME_COVERART_REQUEST); //removed by junam 2013.03.28 diable requet cover for tune.
                        }
                        else
                        {
                            m_tuneMetaInfoTimer.start(TUNE_TIME_METAINFO_REQUEST);
                        }
                    }// { added by junam 2013.03.13 for update pending tune
                    else
                    {
                        m_tuneMetaInfoTimer.start(TUNE_TIME_METAINFO_REQUEST);
                    }
                    // } added by junam
                    MP_LOG << "Request Complete of TA eGetMusicSimpleMetaDataQuery_Id" << LOG_ENDL;
                    break;
                }
				else if (reqUID >= GET_TA_REQUID(REQUID_PREV_TRACK) || reqUID >= GET_TA_REQUID(REQUID_NEXT_TRACK))
				{
					MP_LOG<< "received track info " << fileList[0][0] << LOG_ENDL;
					int playindex = -1;
					if( reqUID >= GET_TA_REQUID(REQUID_PREV_TRACK) )
					{
						playindex = reqUID - REQUID_PREV_TRACK;
						MP_LOG<<"playindex , m_moveTrackIndex"<<playindex <<" "<<m_moveTrackIndex << LOG_ENDL;
						//if(playindex < m_moveTrackIndex)	return;
					}
                    else 
					{
						playindex = reqUID - REQUID_NEXT_TRACK;
						//if(playindex < m_moveTrackIndex)	return;
					}
					
                    m_history[device].Index = m_seekIndex;
                    m_history[device].Path  = fileList[0][3]; // added by kihyung 2013.09.13 for ISV 89552
                     
					// setCurrentIndex(m_seekIndex);//remove by edo.lee 2013.06.29 
					//if(GetEngineMain().getReceivedTrackInfo() > 0)
					{
						EmitOnMediaInfoTempChanged(device, fileList[0][0],
                               (fileList[0][2] != "")? fileList[0][2] : "Unknown",
                               (fileList[0][1] != "") ? fileList[0][1] :"Unknown",
                               "",
                               "",
                               "",
                               getFolderName(fileList[0][3]),
                               ""); // modified by kihyung 2013.08.27 for ITS 0186708 
                        
                                                //emit tuneCounter( GetFilesCountInfo( playindex,m_history[m_device].Quantity)); //added by edo.lee 2013.05.27
                        if(m_nFolderSize > 0)//{ modified by yongkyun.lee 2013-11-06 for : smoke test mp3cd
                        {
                            if(GetEngineMain().middleEast())
                                emit tuneCounter( QString::number(m_nFolderSize) + '/' + QString::number(m_seekIndex + 1));//added by edo.lee 2013.05.30
                            else
                                 emit tuneCounter( QString::number(m_seekIndex + 1) + '/' + QString::number(m_nFolderSize));//added by edo.lee 2013.05.30
                        }
			 // emit positionChanged(0);//added by edo.lee 2013.05.24 // removed by kihyung 2013.07.23 for ITS 0181331

                        // { added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
                        emit mediaInfoClusterChanged(fileList[0][0]);
                        emit trackClusterChanged(m_seekIndex + 1, m_history[device].Quantity);
                        // } added by cychoi 2013.08.17

                        // { modified by cychoi 2013.05.26 for Cluster IOT #125
                        if(device == MP::DISC)
                        {
                            if(m_discType < MEDIA_SOURCE_MAX)
                            {
                                setMediaInfoToHistory(m_discType,fileList[0][0],getFolderName(fileList[0][3]),0);
								
								GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
                                                fileList[0][0], m_randomPlayMode, m_repeatPlayMode, m_scanMode, false); // modified by cychoi 2013.11.12 for OSD Title Translation
                            }
                        }
                        else
                        {
                            setMediaInfoToHistory(AudioUtils::DeviceType(device),fileList[0][0],getFolderName(fileList[0][3]),0);
							
							GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData( (OSD_DEVICE)getOSDevice(device),
					             fileList[0][0], m_randomPlayMode, m_repeatPlayMode, m_scanMode, false);
                        }
						//setMediaInfoToHistory(AudioUtils::DeviceType(m_device),fileList[0][0],getFolderName(fileList[0][3]),0);
                        // } modified by cychoi 2013.05.26
						//GetEngineMain().GetNotifier()->onSetDisplayOSD(true);
						
						//GetEngineMain().setReceivedTrackInfo(-1);
					}
                                }
                MP_LOG << "Request Complete of TA eGetMusicSimpleMetaDataQuery_Id" << LOG_ENDL;
                break;
            }
            
            case eGetAllSongsByAlbumFromVolumeQuery_Id:
            case eGetAllSongsWithoutAlbumFromVolumeQuery_Id:
            {   
                if (!fileList[0].isEmpty()) 
                {
                    if(reqUID == GET_TA_REQUID(REQUID_ALBUM_CHANGE) || reqUID == GET_TA_REQUID(REQUID_DEVICE_ID))
                    {
                        m_songNameListTemp.clear();
                        m_pathsTemp.clear();
                        //{changed by junam 2013.05.27 for cannot select album after tune
                        //if (m_tuneState.m_nTuneFF || m_tuneState.m_nTuneBack)
                        if ((m_tuneState.m_nTuneFF || m_tuneState.m_nTuneBack) && (reqUID != GET_TA_REQUID(REQUID_ALBUM_CHANGE)))
                        { //}changed by junam
                            MP_LOG << "Start update m_tuneTrackNames" << LOG_ENDL;
                            m_tuneTrackNames.clear();

                            foreach (QStringList file, fileList)
                            {
                                m_tuneTrackNames.append(encodeString(file[0]));
                                m_pathsTune.append(file[1]);
                            }

                            if ((m_tuneState.m_nTuneBack) && (m_nPendingMedia < 0))
                            {
                                m_nPendingMedia = m_tuneTrackNames.count() - 1;
                            }

                            // updateCDTuneInfo(); // removed by kihyung 2013.05.21 to remove crash
                            break;
                        }
                        else
                        {
                            foreach (QStringList file, fileList)
                            {
                                if (queryId == eGetAllSongsByAlbumFromVolumeQuery_Id)
                                {
                                    m_songNameListTemp.append(encodeString(file[0]));
                                    m_pathsTemp.append(file[1]);
                                }
                                else
                                {
                                    m_songNameListTemp.append(encodeString(file[1]) );
                                    m_pathsTemp.append( file[0] );
                                }
                            }
                        }
     
                        if (m_searchData.bIsSearch)
                        {
                            MP_MEDIUM << "eGetAllSongsByAlbumFromVolumeQuery_Id - RequestComplete - m_bIsSearch" << LOG_ENDL;

                            int plIndex = (m_searchData.matchField == eAlbum) ?
                                              m_pathsTemp.indexOf(m_searchData.trackUrl) : 0;

                            MP_LOG << "eGetAllSongsByAlbumFromVolumeQuery_Id: plIndex =" << plIndex << LOG_ENDL;
                            createPlayListFromList(m_pathsTemp, plIndex, MP::DEFAULT);
                            m_searchData.bIsSearch = false;
                            break;
                        }
                    }
                }
                else 
                {
                    m_songNameListTemp.clear();
                }

                //{ added by junam 2012.11.12 for CR15447
                if(reqUID == GET_TA_REQUID(REQUID_ALBUM_CHANGE))
                {
                    //{added by junam 2013.09.08 for ITS_KOR_188412
                    if(IsAlbumSupported(m_pathsTemp) == false) //change name by junam 2013.10.17 for ITS_KOR_196128
                    {
                        MP_LOG<<"All song in album is unsupported file" << LOG_ENDL;
                        clearTemps();
                        m_pListViewModel->clearPopup();
                        m_pListViewModel->showPopupByExt(LVEnums::POPUP_TYPE_PLAY_UNAVAILABLE_FILE, false, false);
                        break;
                    }
                    //}added by junam
                    if ( m_albumNameTemp.isEmpty() || m_pathsTemp.isEmpty() ||
                            isFilePathValid( m_pathsTemp.at(0), getDevice()) == false) // modified by kihyung 2013.08.06 for ITS 0182926
                    {
                        m_pPathViewModel->requestCoverEnable(false);
                    }
                    else
                    {
                        int albumIndex = m_pPathViewModel->getAlbumIndex(m_albumNameTemp);
                        if(albumIndex >= 0)
                        {
                            emit setHighlightCover(albumIndex, false);
                        }
                        m_pPathViewModel->requestCoverEnable(true);
                    }
                }
                // } added by junam

                //{changed by junam 2013.07.20 for ITS_180723
                //createPlayList(m_pathsTemp, MP::DEFAULT);
                createPlayList(m_pathsTemp, MP::ALBUM);
                //}changed by junam

                if (m_tuneState.m_nTuneRequest)
                {
                    setCurrentIndex(m_nPendingMedia);
                    m_tuneState.m_nTuneRequest = false;
                    m_nPendingMedia = -1;
                }
                break;
            }

            case eGetAllAlbumsWithUrlFromVolumeQuery_Id:
            {
                if(reqUID == GET_TA_REQUID(REQUID_NEXT_ALBUM))
                {
                    if(fileList.size() == 1) // no name album
                    {
                        MP_LOG<<"no name album " << LOG_ENDL;
                        ETrackerAbstarctor_VolumeType volumeType = AudioUtils::VolumeType(device);
                        m_pTrackerAbstractor->RequestData(volumeType, eIsThereAnySongWithoutAlbumFromVolumeQuery_Id, reqUID );
                    }
                    else
                    {
                        if(fileList[0].isEmpty())
                        {
                            MP_LOG<<"signalNoContent" << LOG_ENDL;
                            emit signalNoContent();
                        }

                        //MP_LOG<<"fileList exist" << LOG_ENDL;
                        QString albumName = GetCurrentAlbumName();
                        MP_LOG<<"albumName "<<albumName << LOG_ENDL;
                        int i;
                        for(i=0; i < fileList.size();i++ )
                        {
                            if(albumName == fileList.at(i).at(0))
                            {
                                break;
                            }
                        }
                        if(i >= fileList.size()-1)
                        {
                            setAlbum(fileList.at(0).at(0));
                        }
                        else
                        {
                            setAlbum(fileList.at(i+1).at(0));
                        }
                    }
                }
                else if(reqUID == GET_TA_REQUID(REQUID_DEVICE_ID))
                {
                    getMusicAlbum(fileList);
                }
                break;
            }

            case eRetrieveCoverArtOnDemand_Id:
            {
                if(reqUID == GET_TA_REQUID(REQUID_ROTATE_TUNE))
                {                   
                    //{changed by junam 2013.03.10 for ISV73458
                    m_tuneMetaInfoTimer.stop();
                    //m_tuneCoverArtTimer.stop();//removed by junam 2013.10.17 for deprecated code

                    // { added by lssanh 2013.02.17 tune select timer
                    //if (GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->IsTuneSelectTimerOn() == false)
                    //{
                    //    emit setTuneRequestTime(50);
                    //    m_tuneCoverDelayTimer->stop();
                    //    break;
                    //}
                    // } added by lssanh 2013.02.17 tune select timer
                    //}changed by junam
                    if( fileList.isEmpty() ||
                        fileList.at(0).at(1).isEmpty() ||
                        fileList.at(0).at(1).contains("No CoverArt Found"))
                    {
                        //MP_LOG<<"Invalid cover uri" << LOG_ENDL;
                    }
                    else
                    {
                        //{changed by junam 2013.03.10 for ISV73458
                        // { modified by lssanh 2013.01.24 ISV70714
                        // emit tuneCoverart( fileList.at(0).at(1) );
                        //if (m_device == MP::IPOD1 || m_device == MP::IPOD2)
                        //{
                        //    if ( m_tuneCoverDelayTimer->isActive() )
                        //    {
                        //        m_tuneCoverDelayTimer->stop();
                        //    }
                        //    m_tuneCoverDelayTimer->start();

                        //    m_DelayCoverArt = fileList.at(0).at(1);
                        //}
                        //else
                        //{
                        emit tuneCoverart( fileList.at(0).at(1) );
                        //}
                        // } modified by lssanh 2013.01.24 ISV70714
                    }
                    //emit setTuneRequestTime(50);
                    m_tuneMetaInfoTimer.start(TUNE_TIME_METAINFO_REQUEST);
                    //}changed by junam
                    break;
                }
                else if(reqUID == GET_TA_REQUID(REQUID_DEVICE_ID) || 
                        reqUID == GET_TA_REQUID(REQUID_REQUST_COVER_FLOW_VIEW) || 
                        reqUID == GET_TA_REQUID(REQUID_REQUST_COVER_LIST_VIEW))
                {
                    if (device == MP::DISC && m_discType == MEDIA_SOURCE_MP3 && fileList.count() == 1 && fileList.at(0).count() == 2 )
                    {
                        MP_LOG << "ROWS CASE 2" << LOG_ENDL;
                        if (fileList.at(0).at(1).contains("No CoverArt Found"))
                            m_deckInfo.coverarturl = ""/*DEFALUT_COVER*/; //modified by Michael.Kim 2013.04.09 for New UX
                        else
                            m_deckInfo.coverarturl = fileList.at(0).at(1);

                        onMediaInfoChanged(&m_deckInfo);
                        break;
                    }
                    // { added by kihyung 2012.07.9 for CR 11165
                    else if((device == MP::JUKEBOX || 
                             device == MP::USB1    ||
                             device == MP::USB2    ||
                             device == MP::IPOD1   ||
                             device == MP::IPOD2) && m_bRequestCoverOnBasicView == true)
                    {
                        MP_LOG << "ROWS CASE 3" << LOG_ENDL;
                        m_bRequestCoverOnBasicView = false;
                        QString coverart =  (fileList.at(0).at(1).isEmpty() || fileList.at(0).at(1).contains("No CoverArt Found")) ?
                                    ""/*DEFALUT_COVER*/ /*modified by Michael.Kim 2013.04.09 for New UX*/ : fileList.at(0).at(1);
                        emit tuneCoverart( coverart );
                        break;
                    }
                    // } added by kihyung

                    getMusicCoverArt(fileList, reqUID);// add reqUID by junam 2012.10.25 for CR14451
                }
                break;
            }

            case eGetMusicAlbumCoverArt_Id:
            {
                getMusicAlbumCoverArtComplete(fileList);
                break;
            }

            case eGetMusicFromCDQuery_Id:
            {
                MP_LOG << "eGetMusicFromCDQuery_Id" << LOG_ENDL;

                QStringList artists;
                QStringList albums;
                m_listCDContent.clear();

                foreach (QStringList list, fileList)
                {
                    MP_LOG << "\nSIZE: " << list.count() << LOG_ENDL;
                    if (list.count() >= 8) 
                    {

                        CD_META_INFO song;
                        song.trackTitle = list[0];
                        song.albumTitle = list[1];
                        song.artist = list[2];
                        song.genre = list[3];
						if(list.count()>=8)
						{
                            song.composer = list[7]; 
						}
                        song.cover = "";
                        m_listCDContent.append(song);

                        artists.append(list[6]);
                        albums.append(song.albumTitle);
                    }
                }

                SetCDDAGraceNote( GRACENOTE_INDEXING_DONE );// added by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
                //{ added by yongkyun.lee 20130702 for : NO CR - MP3 CD song info update
                DisplayCDInfo(GetCurrentTrackIndex(), false);
                //DisplayCDInfo(GetCurrentTrackIndex());
                //} added by yongkyun.lee 20130702 
                m_pTrackerAbstractor->RequestData( albums, artists, eGetMusicAlbumCoverArt_Id );
                break;
            }

            case eIsThereAnySongWithoutAlbumFromVolumeQuery_Id:
            {
                MP_LOG << "eIsThereAnySongWithoutAlbumFromVolumeQuery_Id" << LOG_ENDL;
				// { added by edo.lee 2012.12.21
				if(reqUID == GET_TA_REQUID(REQUID_NEXT_ALBUM))
				{
					if(!fileList[0].isEmpty()&& fileList[0][0]=="1")
					{
                        // { removed by kihyung 2013.2.17
                        /*
                        // added by eugene.seo 2013.01.18 for 25000 over USB
						if ( m_requestFirstSongTimer->isActive() )	
						    m_requestFirstSongTimer->stop();
						// added by eugene.seo 2013.01.18 for 25000 over USB
						*/
						// } removed by kihyung 2013.2.17
						setAlbum(NONAME_ALBUM);
					}
					else
					{
						MP_LOG<<"signalNoContent" << LOG_ENDL;
						emit signalNoContent();
					}
				}
				else if(reqUID == GET_TA_REQUID(REQUID_DEVICE_ID))
				// } added by edo.lee 
				{
	                if (!fileList[0].isEmpty())
                        {
                            //{removed by junam 2013.11.06 for coverflow update
	                    //if (isCoverCleanRequired())
	                    //{
	                    //    setIsCoverCleanRequired(false);
	                    //    //m_pPathViewModel->clearModelData();
                            //}
                            //}removed by junam
	                }
				}				
                break;
            }

            case eGetAllSongsByArtistFromVolumeQuery_Id:
            {
                MP_LOG << "eGetAllSongsByArtistFromVolumeQueryId" << LOG_ENDL;

                if (!fileList[0].isEmpty() && m_searchData.bIsSearch)
                {
                    QStringList filePaths;
                    foreach (QStringList file, fileList)
                    {
                        m_songNameList.append(encodeString(file[0]));
                        filePaths.append(file[1]);
                    }

                    int plIndex = (m_searchData.matchField == eArtist) ? filePaths.indexOf(m_searchData.trackUrl) : 0;

                    MP_LOG << "eGetAllSongsByArtistFromVolumeQuery_Id: plIndex =" << plIndex << LOG_ENDL;
                    createPlayListFromList(filePaths, plIndex, MP::DEFAULT);
                    m_searchData.bIsSearch = false;
                }
                else
                {
                    m_songNameList.clear();
                }
                break;
            }
            case eGetXSampaId3NameQuery_Id:
            {
                // { removed by kihyung 2013.07.19
                /*
                MP_LOG << "RequestComplete m_pMoreLikeThis->call eGetXSampaId3NameQuery_Id" << fileList.at(0).at(0) << LOG_ENDL;
                m_pMoreLikeThis->call("CreateMoreLikeThis", "album", fileList.at(0).at(0));
                */
                // } removed by kihyung 2013.07.19                
                break;
            }

            // { added by kihyung 2013.4.7 for IPOD
            case eGetSongIndexFromVolumeQuery_Id:
            {
                MP_LOG << "IPODERR. eGetSongIndexFromVolumeQuery_Id " << fileList[0][0].toInt() << LOG_ENDL;
                
                if(GET_TA_REQUID(REQUID_IPOD_ERR_SONG_IDX) != reqUID) break;

                ETrackerAbstarctor_VolumeType volumeType;
                if(device == MP::IPOD1) {
                    m_nIPOD1ErrIndex = fileList[0][0].toInt();
                    volumeType       = eIPOD_FRONT;
                }
                else if(device == MP::IPOD2) {
                    m_nIPOD2ErrIndex = fileList[0][0].toInt();
                    volumeType       = eIPOD_REAR;
                }
                else {
                    break;
                }

                m_pTrackerAbstractor->RequestData(volumeType, eGetAllSongsFromVolumeQuery_Id, GET_TA_REQUID(REQUID_IPOD_ERR_ALL_SONGS));
                
                break;
            }
            
            case eGetAllSongsFromVolumeQuery_Id:
            {
                MP_LOG << "IPODERR. eGetAllSongsFromVolumeQuery_Id" << LOG_ENDL;
                
                if(GET_TA_REQUID(REQUID_IPOD_ERR_ALL_SONGS) != reqUID) break;

                if(fileList[0].isEmpty() == false)
                {
                    int nStartIdx = 0;
                    if(device == MP::IPOD1) {
                        nStartIdx = m_nIPOD1ErrIndex;
                    }
                    else if(device == MP::IPOD2) {
                        nStartIdx = m_nIPOD2ErrIndex;
                    }
                    else {
                        MP_LOG << "eGetAllSongsFromVolumeQuery_Id error... m_device " << device << LOG_ENDL;
                    }
                    
                    QString     strPrefix("file://");
                    QStringList filePaths;
                    foreach (QStringList list, fileList)
                    {
                        list[0].remove(strPrefix);
                        filePaths.append(list[0]);
                    }
                    createPlayListFromList(filePaths, nStartIdx, MP::SONG);
                }
                break;
            }
            // } added by kihyung 2013.4.7 
       
            default:
            {
                break;
            }
        }
    }
    else
    {
        // { modified by eugene.seo 2013.01.18 for 25000 over USB
		switch ( queryId )
		{
	        case eGetAllAlbumsWithUrlFromVolumeQuery_Id:
	        {
                // { added by eugene.seo 2013.01.31 for all unsupported audio play
                if(reqUID == GET_TA_REQUID(REQUID_NEXT_ALBUM))
                {		
                    MP_LOG<<" NoContent OUT!!!" << LOG_ENDL;	
                    if( device == MP::JUKEBOX || device == MP::USB1 || device == MP::USB2 ) {
                        // { modified by kihyung 2013.10.30 for ITS 0204910 
                        if(isForeground() == true) 
                        {
                            emit showPopupError(true); // modified by kihyung 2013.09.12 for ISV 90605
                        }
                        else
                        {
                            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->SetNotifyAllUnsupported();
                        }
                        // } modified by kihyung 2013.10.30  
                    }
                }
                // } added by eugene.seo 2013.01.31 for all unsupported audio play
                // { removed by kihyung 2013.2.17
                /*
                else if(reqUID == GET_TA_REQUID(REQUID_DEVICE_ID))
                {
    	            if (m_minerState.isMinerCleanRequired || m_device == MP::DISC )
    	            {
    	                // m_pPathViewModel->clearModelData();
    	            }			
    				if(m_device == MP::USB1 || m_device == MP::USB2)
    				{						
    					if ( !m_requestFirstSongTimer->isActive() )
    					{
    						m_requestFirstSongTimer->start();
    					}					
    				}
                    
    	            else if (m_device == MP::JUKEBOX)
    	            {
    	                requestContentSize(MP::JUKEBOX);
    	            }
                }
                */
                // } removed by kihyung 2013.2.17                
                break;
	        }
	        case eGetXSampaId3NameQuery_Id:
	        {
                // { removed by kihyung 2013.07.19
                /*
	            MP_LOG << "RequestComplete fileList is NULL m_pMoreLikeThis->call XSampaDB doesn't exist" << m_albumName << LOG_ENDL;
	            m_pMoreLikeThis->call("CreateMoreLikeThis", "album", m_albumName);
	            */
                // } removed by kihyung 2013.07.19
                break;
	        }
            
            //{ modified by yongkyun.lee 2013-07-06 for : ITS 178505
            case eGetMusicFromCDQuery_Id:
            {
                SetCDDAGraceNote( GRACENOTE_INDEXING_NO_INFO );// added by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
                break;
            }
            //} modified by yongkyun.lee 2013-07-06 
            
			default:
            {
			    break;
			}
		}
        // } modified by eugene.seo 2013.01.18 for 25000 over USB
        /* If we recieve empty list, we should reset MirenCounter, to avoid waiting for minerFinished*/
        m_minerState.minerInProgress = 0;
        m_tuneState.m_nTuneFF = false;
        m_tuneState.m_nTuneBack = false;
    }

    if (queryId == eGetAllMusicByKeywordQuery_Id)
    {
        MP_LOG << "eGetAllMusicByKeywordQuery_Id" << LOG_ENDL;
        // m_pSearchEngine->returnSearchResult(fileList, queryId);  //20131010 delete search engine
    }
}

//{ modified by yongkyun.lee 2013-08-22 for :31.6 Ver. Smoke Test 
void
AppMediaPlayer_Controller_Audio::clearMP3Info( )
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    m_history[device].Cover = "";// modified by yongkyun.lee 2013-11-10 for : NOCR 
    m_history[device].Artist = "";
    m_history[device].Album  = "";

}
//} modified by yongkyun.lee 2013-08-22 

//{ added by wonseok.heo 2013.05.17
void
AppMediaPlayer_Controller_Audio::updateMP3Info(QString mp3Name, int index) // modified by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
{
    Q_UNUSED(index);
    
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;

    MP_LOG << LOG_ENDL;

    emit clearRepeatRandom(); //added by wonseok.heo for ITS 172869 2013.06.08

    // { modified by cychoi 2013-07-15 for Prevent fix
    if ( m_discType != MEDIA_SOURCE_MP3)
    //if ( !m_discType == MEDIA_SOURCE_MP3)
    // } modified by cychoi 2013-07-15
        return;

    //{ modified by yongkyun.lee 2013-08-15 for :   NO CR  MP3 ID3-tag update
    QString coverart = m_history[device].Cover  == "" ? "": m_history[device].Cover;
        // { modified by wonseok.heo for NO CR delete unknown 2013.08.20
    QString artist   = m_history[device].Artist == "" ? "" : m_history[device].Artist;
    QString song     = m_history[device].Title  == mp3Name ? "" : m_history[device].Title;
    QString album    = m_history[device].Album  == "" ? "" : m_history[device].Album;
        // } modified by wonseok.heo for NO CR delete unknown 2013.08.20
    //} modified by yongkyun.lee 2013-08-15 

    //modified by aettie 2013 08 05 for ISV NA 85625
    //QString folder = "/..";
    // modified by aettie 20130812 for ITS 183630
    //QString folder = "Root";
    QString folder = "STR_MEDIA_ROOT_FOLDER"; // modified by Dmitry 05.10.13 for ITS0193742
    QString countInfo = "";
    emit mp3fileInfo(coverart, artist, song, album, folder, countInfo);
    // { modified by cychoi 2013.05.26 for Cluster IOT #125
    
    m_strUpdatedMediaInfo = ""; // modified by yongkyun.lee 2013-10-02 for : ITS 192608
    
    if(device == MP::DISC)
    {
        if(m_discType < MEDIA_SOURCE_MAX)
            setMediaInfoToHistory(m_discType,song,folder,0); 
    }
    else
    {
        setMediaInfoToHistory(AudioUtils::DeviceType(device),song,folder,0); // added by wonseok.heo 2013.05.17
    }

    // { added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
    if (m_pIMedia && m_history[device].ContentPlayed == MP::FOLDER)
    {
        int fileIndex = -1;
        int fileCount = -1;
        m_pIMedia->GetCurrentFileFolderIndex(&fileIndex, &fileCount);
        //{ modified by cychoi 2013.08.29 for HMC QC IPOD cluster display on Bootup
        emit mediaInfoClusterChanged(song);
        emit trackClusterChanged(fileIndex + 1, m_history[device].Quantity);
        //} modified by cychoi 2013.08.29
    }
    else
    {
        int trackIndex = GetCurrentTrackNumber();
        //{ modified by cychoi 2013.08.29 for HMC QC IPOD cluster display on Bootup
        emit mediaInfoClusterChanged(song);
        emit trackClusterChanged(trackIndex, m_history[device].Quantity);
        //} modified by cychoi 2013.08.29
    }
    // } added by cychoi 2013.08.17

    GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData(DISC_MP3_MUSIC,
                    song, m_randomPlayMode, m_repeatPlayMode, m_scanMode, false); // modified by cychoi 2013.11.12 for OSD Title Translation
    //setMediaInfoToHistory(AudioUtils::DeviceType(m_device),song,folder,0); // added by wonseok.heo 2013.05.17
    // } modified by cychoi 2013.05.26
    //GetEngineMain().GetNotifier()->onSetDisplayOSD(true); // added by wonseok.heo 2013.05.17
}
//} added by wonseok.heo 2013.05.17

void
AppMediaPlayer_Controller_Audio::updateCDTuneInfo()
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    MP_LOG << LOG_ENDL;

    if (m_tuneTrackNames.count() == 0)
    {
        MP_LOG << "m_tuneTrackNames.count() == 0" << LOG_ENDL;
        tuneReset();
        return;
    }

    MP_MEDIUM << "Update Info" << LOG_ENDL;

    if ( m_nPendingMedia < 0 )
        return;

    QString coverart = ""/*DEFALUT_COVER*/; //modified by Michael.Kim 2013.04.09 for New UX // { modified by junggil 2012.08.10 for CR12745
    // { modified by wonseok.heo for NO CR delete unknown 2013.08.20
    QString artist = "";
    QString song = "";
    QString album = "";
    QString folder = "";
    QString genre = "";
    QString composer = "";
    // } modified by wonseok.heo for NO CR delete unknown 2013.08.20
    QString countInfo = "";
	

    int indexListCDContent = 0;

    if(m_isPlayFromMLT)
    {
        if(m_nPendingMedia >= m_mltFilePathesList.size())
        {
            MP_LOG<<"ERROR m_nPendingMedia is out of bound "<<m_nPendingMedia<<"/"<<m_mltFilePathesList.size() << LOG_ENDL;
            return;
        }

        indexListCDContent = m_mltFilePathesList.at(m_nPendingMedia).toInt();

        if(indexListCDContent >= m_listCDContent.size())
        {
            MP_LOG<<"ERROR indexListCDContent is out of bound "<<indexListCDContent<<"/"<<m_listCDContent.size() << LOG_ENDL;
            return;
        }
    }
    else
    {
        indexListCDContent = m_nPendingMedia;
    }
    
    if (m_listCDContent.count() > 0)
    {
        if(indexListCDContent >= 0 && indexListCDContent < m_listCDContent.size()) {
            CD_META_INFO track = m_listCDContent.at(indexListCDContent);
            artist   = track.artist;
            album    = track.albumTitle;
            genre    = track.genre;
            coverart = track.cover;
        }

        if(indexListCDContent >= 0 && indexListCDContent < m_tuneTrackNames.size()) {
            song = (m_tuneTrackNames.at(indexListCDContent) != "") ? m_tuneTrackNames.at(indexListCDContent) : ""; // modified by wonseok.heo for NO CR delete unknown 2013.08.20
        }

        if(m_isPlayFromMLT)
            countInfo = GetFilesCountInfo( m_nPendingMedia, m_mltFilePathesList.count());
        else
            countInfo = GetFilesCountInfo( m_nPendingMedia, m_listCDContent.count());        
    }
    else
    {
        if (m_tuneTrackIDs.isEmpty())
        {
            if( m_nPendingMedia >= 0 && m_nPendingMedia < m_tuneTrackNames.size() )
            {
                song = m_tuneTrackNames.at(m_nPendingMedia);
            }

			if( m_nPendingMedia >= 0 && m_nPendingMedia < m_pathsTune.size() )
			{
			    folder = m_pathsTune.at(m_nPendingMedia);
			}
        }
        else
        {
            int idxID=m_tuneTrackIDs.indexOf(m_nPendingMedia + 1);
            if(idxID >= 0 && idxID < m_tuneTrackNames.size())
            {
                song = m_tuneTrackNames.at(idxID);
            }

			if(idxID >= 0 && idxID < m_pathsTune.size())
			{
			    folder = m_pathsTune.at(idxID);
			}
        }

        if( song.isEmpty())
            song = QString(""); //  modified by wonseok.heo for NO CR delete unknown 2013.08.20

        if( folder.isEmpty())
            folder = QString(""); //  modified by wonseok.heo for NO CR delete unknown 2013.08.20

        countInfo = GetFilesCountInfo( m_nPendingMedia, m_history[MP::DISC].Quantity);
    }

    m_tuneState.m_nTuneFF = false;
    m_tuneState.m_nTuneBack = false;

    bool tuneSameFile=(m_history[device].Index== m_nPendingMedia);
    emit tuneSameFileNoti(tuneSameFile);

    EmitOnTuneSearch(coverart, artist, song, album, folder, countInfo, tuneSameFile); // modified by kihyung 2013.08.27 for ITS 0186708 
    // { added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
    //emit trackClusterChanged(m_nPendingMedia, m_history[device].Quantity);
    //emit mediaInfoClusterChanged(song);
    // } added by cychoi 2013.08.17
    // { modified by cychoi 2013.11.12 for OSD Title Translation
    // { modified by wonseok.heo for ITS 197433 2013.10.28
    if(tuneSameFile)
    {
        GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
                                                                                          song, m_randomPlayMode, m_repeatPlayMode, m_scanMode, false);
    }
    else
    {
        if((device == MP::DISC) && (m_discType == MEDIA_SOURCE_CDDA) && GetCDDAGraceNote() != GRACENOTE_INDEXING_DONE)
        {
            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
                                                                                              QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(song), m_randomPlayMode, m_repeatPlayMode, m_scanMode, true);
        }
        else
        {
            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
                                                                                              song, m_randomPlayMode, m_repeatPlayMode, m_scanMode, true);
        }
    }
    // } modified by wonseok.heo for ITS 197433 2013.10.28
    // } modified by cychoi 2013.11.12
}


int
AppMediaPlayer_Controller_Audio::getPlayingPosition() const
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return 0;
    
    MP_LOG << m_history[device].Position << LOG_ENDL;
    return m_history[device].Position;
}

int
AppMediaPlayer_Controller_Audio::getPlayingDuration() const
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return 0;
    
    MP_LOG << m_history[device].Duration << LOG_ENDL;
    return m_history[device].Duration;
}

/**
 * To request list of covers, we should get list of all albums from Volume.
 */
void
AppMediaPlayer_Controller_Audio::requestCovers()//bool bCleanup) //changed by junam 2013.11.06 for coverflow update
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;

    MP_LOG << DeviceName(device) << LOG_ENDL;

    //m_minerState.isMinerCleanRequired = bCleanup; //removed by junam 2013.11.06 for coverflow update

    ETrackerAbstarctor_VolumeType volumeType = AudioUtils::VolumeType(device);

    if (volumeType == eDefaultVolume ||
       (volumeType == eIPOD_FRONT && !m_bIsApple1Ready) ||
       (volumeType == eIPOD_REAR && !m_bIsApple2Ready))
    {
        MP_LOG << "Skip requesting covers!" << LOG_ENDL;
        m_minerState.minerInProgress = 0;
        return;
    }

    //{removed by junam 2013.11.06 for coverflow update
    //if(m_bIsBasicView)
    //{
    //    MP_LOG<<"Current state is basic view : no need request cover" << LOG_ENDL;
    //    return;
    //}
    //}removed by junam

    if (m_pTrackerAbstractor != NULL)
    {
        MP_HIGH << "Request all albums from VolumeType" << volumeType << LOG_ENDL;
        //m_pPathViewModel->clearModelData(); //removed by junam 2013.11.06 for coverflow update
        m_pTrackerAbstractor->RequestData(volumeType, eGetAllAlbumsWithUrlFromVolumeQuery_Id, GET_TA_REQUID(REQUID_DEVICE_ID));

        //{removed by junam 2013.10.31 for not using code
        //if (device != MP::IPOD1 && device != MP::IPOD2)
        //{
        //    m_pTrackerAbstractor->RequestData(volumeType, eIsThereAnySongWithoutAlbumFromVolumeQuery_Id, GET_TA_REQUID(REQUID_DEVICE_ID));
        //}
        //}removed by junam
    }
}


//{ added by yongkyun.lee 20130219 for : ISV 73129
int
AppMediaPlayer_Controller_Audio::GetCurrentMltTrackIndex()
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return 0;
    
    if(m_isPlayFromMLT && (device == MP::DISC) &&  ( m_discType = MEDIA_SOURCE_CDDA) )
    { 
        return m_mltFilePathesList.at(m_history[device].Index).toInt();
    }
    else
    {
        return m_history[device].Index;
    }
}
//} added by yongkyun.lee 20130219 

int
AppMediaPlayer_Controller_Audio::GetCurrentTrackIndex()
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return 0;
        
    return m_history[device].Index;
}

int
AppMediaPlayer_Controller_Audio::GetCurrentTrackNumber()
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return 1;
        
    return m_history[device].Index + 1;
}

// { removed by kihyung 2012.11.03 for booting disable
/*
void
AppMediaPlayer_Controller_Audio::requestAudioModeChange(TAudioMode eAudioMode, TAudioSink eAudioSink)
{ 
    MP_LOG << "eAudioMode=" << eAudioMode << LOG_ENDL;
    if (m_currAudioMode != eAudioMode)
    {
        rpmClientObj->RPMClientAcquire(eAudioMode, eAudioSink); 
        m_currAudioMode = eAudioMode;
        MP_LOG << "eAudioMode=" << eAudioMode << " acquired." << LOG_ENDL;
    }
}
*/
// } removed by kihyung 2012.11.03 for booting disable

// { removed by kihyung 2012.11.03 for booting disable
/*
void
AppMediaPlayer_Controller_Audio::resetAudioMode()
{
    m_currAudioMode = eAVOff; 
}
*/
// } removed by kihyung 2012.11.03 for booting disable

void
AppMediaPlayer_Controller_Audio::setAudioPathRPM ()
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    MP_LOG << DeviceName (device) << LOG_ENDL;
      //{ modified by yongkyun.lee 2013-12-13 for : ITS 213657
    if( device == MP::BLUETOOTH && m_iPodMountTimer->isActive() )
    {
        MP_LOG << "m_iPodMountTimer active." << LOG_ENDL;
        return;      
    }
    //} modified by yongkyun.lee 2013-12-13 

    // if (rpmClientObj != NULL) // removed by kihyung 2012.11.03 for booting disable
    {
        switch ( device )
        {
            case MP::JUKEBOX:
                // requestAudioModeChange(eJukeBoxAudioSPDIF, eMain); // removed by kihyung 2012.11.03 for booting disable
                GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setLastAVMode(MODE_STATE_JUKEBOX_AUDIO); // added by ruindmby 2012.08.20
                break;
            case MP::USB1:
                // requestAudioModeChange(eUSB1AudioSPDIF, eMain); // removed by kihyung 2012.11.03 for booting disable
                GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setLastAVMode(MODE_STATE_USB1_AUDIO); // added by ruindmby 2012.08.20
                break;
            case MP::USB2:
                // requestAudioModeChange(eUSB2AudioSPDIF, eMain); // removed by kihyung 2012.11.03 for booting disable
                GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setLastAVMode(MODE_STATE_USB2_AUDIO); // added by ruindmby 2012.08.20
                break;
            case MP::DISC:
                // requestAudioModeChange(eDeckAudioSPDIF, eMain); // removed by kihyung 2012.11.03 for booting disable
                GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setLastAVMode(MODE_STATE_DISC_AUDIO); // added by ruindmby 2012.08.20
                break;
            case MP::IPOD1:
                // requestAudioModeChange(eIPOD1SPDIF, eMain); // removed by kihyung 2012.11.03 for booting disable
                GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setLastAVMode(MODE_STATE_IPOD1_AUDIO); // added by ruindmby 2012.08.20
                break;
            case MP::IPOD2:
                // requestAudioModeChange(eIPOD2SPDIF, eMain); // removed by kihyung 2012.11.03 for booting disable
                GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setLastAVMode(MODE_STATE_IPOD2_AUDIO); // added by ruindmby 2012.08.20
                break;
            case MP::BLUETOOTH:
                // requestAudioModeChange(eBTAudio, eMain); // removed by kihyung 2012.11.03 for booting disable
                GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setLastAVMode(MODE_STATE_BT_AUDIO); // added by ruindmby 2012.08.20
                break;
            case MP::AUX:
                // requestAudioModeChange(eAUX1Audio, eMain); // removed by kihyung 2012.11.03 for booting disable
                GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setLastAVMode(MODE_STATE_AUX1_AUDIO); // added by ruindmby 2012.08.20
                break;
            default:
                MP_MEDIUM << "Unhadled mode !!" << LOG_ENDL;
                break;
        }
    }
}
/*
//{added by aettie.ji 2012.12.18 for new ux
QString
AppMediaPlayer_Controller_Audio::GetCurrentCategoryId()
{
	 if (m_history[m_device].Quantity > 0)
	 {
	 	return m_history[m_device].curCat;

	 }
}
//}added by aettie.ji 2012.12.18 for new ux
*/
QString
AppMediaPlayer_Controller_Audio::GetFilesCountInfo()
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return QString("");
    
    // { changed by eugeny - 12-09-15
    
    QString result = "";
    
    if(device == MP::UNDEFINED) {
        MP_HIGH << " ERROR m_device" << device << LOG_ENDL;
        return result;
    }
    //{added by junam 2013.07.23 for ITS_KOR_181304
    if(device == MP::IPOD1 || device == MP::IPOD2)
    {
        if( m_bBasicControlEnableStatus == false)
        {
            MP_LOG<<"Empty string m_bBasicControlEnableStatus is FALSE" << LOG_ENDL;
            return QString("");
        }
    }
    //}added by junam

    if (m_history[device].Quantity > 0)
    {
        if (m_pIMedia && m_history[device].ContentPlayed == MP::FOLDER)
        {
            int fileIndex = -1;
            int fileCount = -1;
            m_pIMedia->GetCurrentFileFolderIndex(&fileIndex, &fileCount);

            //MP_LOG << fileIndex << fileCount << LOG_ENDL; // modified by eugeny.novikov 2012.10.25 for CR 14047

            if (fileIndex >= 0 && fileCount > 0)
            {
                if(GetEngineMain().middleEast())
                    result = QString::number(fileCount) + '/' + QString::number(fileIndex + 1);
                else
                    result = QString::number(fileIndex + 1) + '/' + QString::number(fileCount);

                // { modified by wspark 2013.03.27 for ISV 77371
                //emit updateMP3CDTrackInfo(fileIndex + 1); // added by wspark 2012.11.07 for updating mp3cd track info correctly.
                emit updateTrackInfo(fileIndex + 1);
                // } modified by wspark
            }
        }
        else
        {
            if(device == MP::DISC && getDiscType() == MP::AUDIO_CD) {
                if(m_isPlayFromMLT)
                {
                    if(GetEngineMain().middleEast())
                     	result = QString::number(m_mltFilePathesList.count()) + '/' + QString::number(GetCurrentTrackNumber()); //added by Michael.Kim 2013.05.24 for Song Index Count Error
                    else
                        result = QString::number(GetCurrentTrackNumber()) + '/' + QString::number(m_mltFilePathesList.count()); //added by Michael.Kim 2013.05.24 for Song Index Count Error
                }else{                    
                    if(m_listCDContent.count() > 0)
                    {
                        if(GetEngineMain().middleEast())
                            result = QString::number(m_listCDContent.count()) + '/' + QString::number(GetCurrentTrackNumber()); //added by Michael.Kim 2013.05.24 for Song Index Count Error
                        else
                            result = QString::number(GetCurrentTrackNumber()) + '/' + QString::number(m_listCDContent.count()); //added by Michael.Kim 2013.05.24 for Song Index Count Error
                    }
                    else
                    {
                        if(GetEngineMain().middleEast())
                            result = QString::number(m_history[device].Quantity) + '/' + QString::number(GetCurrentTrackNumber()); //added by Michael.Kim 2013.05.25 if listCDContent is empty
                        else
                            result = QString::number(GetCurrentTrackNumber()) + '/' + QString::number(m_history[device].Quantity); //added by Michael.Kim 2013.05.25 if listCDContent is empty
                    }
                }
            } else {
                // { modified by wonseok.heo 2013-19-20 for ITS 197065
                if(device == MP::DISC && getDiscType() == MP::MP3_CD)
                    return "";
                // } modified by wonseok.heo 2013-19-20 for ITS 197065
                if(GetEngineMain().middleEast())
                    result = QString::number(m_history[device].Quantity) + '/' + QString::number(GetCurrentTrackNumber());
                else
                    result = QString::number(GetCurrentTrackNumber()) + '/' + QString::number(m_history[device].Quantity);
                // { modified by wspark 2013.03.27 for ISV 77371
                //emit updateMP3CDTrackInfo(GetCurrentTrackNumber()); // added by wspark 2012.11.07 for updating mp3cd track info correctly.
            }
            emit updateTrackInfo(GetCurrentTrackNumber());
            // } modified by wspark
        }
    }
	// { added by lssanh 2012.11.14 for ISV 59882
	// { modified by jungae, 2013.02.07 for ITS155162 
	// else if(m_history[m_device].playList.list)
	else if(m_history[device].playList.list && m_history[device].playList.list->empty() == false) // } modified by jungae
    {
        if(GetEngineMain().middleEast())
            result = QString::number(m_history[device].playList.list->count()) + '/' + QString::number(GetCurrentTrackNumber());
        else
            result = QString::number(GetCurrentTrackNumber()) + '/' + QString::number(m_history[device].playList.list->count());
        // { modified by wspark 2013.03.27 for ISV 77371
        //emit updateMP3CDTrackInfo(GetCurrentTrackNumber());
        emit updateTrackInfo(GetCurrentTrackNumber());
        // } modified by wspark
    }
    else 
    {
        MP_LOG << "ERROR" << LOG_ENDL;
    }
	// } added by lssanh 2012.11.14 for ISV 59882
    MP_LOG << result << LOG_ENDL;
    return result;
// } changed by eugeny - 12-09-15
}

//{added by junam 2013.08.30 for ITS_KOR_186080
QString AppMediaPlayer_Controller_Audio::GetFilesCountInfo(int device)
{
    if(device >= MP::ALL_DEVICES || device == getDevice())
        return GetFilesCountInfo();

    //{removed by junam 2013.10.08 for ITS_NA_192033
    //if (m_history[device].ContentPlayed == MP::FOLDER)
    //{
    //    //TODO - send correct folder index
    //    return QString("");
    //}
    //}removed by junam

    if (m_history[device].Index >= 0 && m_history[device].Quantity > 0)
    {
        if(GetEngineMain().middleEast())
            return QString::number(m_history[device].Quantity) + '/' + QString::number(m_history[device].Index + 1);
        else
            return QString::number(m_history[device].Index + 1) + '/' + QString::number(m_history[device].Quantity);
    }
    return QString("");
}
//}added by junam

// { added by junam 2012.10.06 for folder tune
QString
AppMediaPlayer_Controller_Audio::GetFilesCountInfo(int iIndex, int nCount)
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return QString("");
    
    if ((m_pIMedia && m_history[device].ContentPlayed == MP::FOLDER) &&
        (device != MP::DISC || m_discType != MEDIA_SOURCE_CDDA)) // modfiied by oseong.kwon 2014.01.15 for ITS 219600
    {
        int fileIndex = -1, fileCount = -1;
        m_pIMedia->GetCurrentFileFolderIndex(&fileIndex, &fileCount);
        fileIndex = iIndex - m_history[device].Index + fileIndex;
        MP_LOG<<"fileIndex ="<<fileIndex<<", fileCount ="<<fileCount << LOG_ENDL;
        if (fileIndex >= 0 && fileCount > 0)
        {
            if(GetEngineMain().middleEast()) //modifed by nhj 2013.10.07 for middleeast
                return QString::number(fileCount) + '/' + QString::number(fileIndex + 1);
            else
                return QString::number(fileIndex + 1) + '/' + QString::number(fileCount);
        } //{ modified by wonseok.heo for ISV 92439 2013.10.11
        else{
            return QString("");
        } // } modified by wonseok.heo for ISV 92439 2013.10.11
    }
    if (iIndex >= 0 && nCount > 0)
    {
        if(GetEngineMain().middleEast()) //modifed by nhj 2013.10.07 for middleeast
            return QString::number(nCount) + '/' + QString::number(iIndex + 1);
        else
           return QString::number(iIndex + 1) + '/' + QString::number(nCount);
    }
    return QString("");
}
// } added by junam

QStringList
AppMediaPlayer_Controller_Audio::getPathsTemp() const
{
    return m_pathsTemp;
}

//{removed by junam 2013.11.06 for coverflow update
//bool
//AppMediaPlayer_Controller_Audio::isCoverCleanRequired() const
//{
//    return m_minerState.isMinerCleanRequired;
//}

//void
//AppMediaPlayer_Controller_Audio::setIsCoverCleanRequired( bool b_isCleanRequired )
//{
//    MP_LOG << b_isCleanRequired << LOG_ENDL;
//    m_minerState.isMinerCleanRequired = b_isCleanRequired;
//}
//} removed by junam 2013.11.06 for coverflow update

void
AppMediaPlayer_Controller_Audio::OnSearchCompleted(int nTitleNo, int nChapterNo, int nChapterSize )
{
    MP_LOG << nTitleNo << LOG_ENDL;
	// { added by ravikanth - 12-09-26
    if(m_isDVDinfoRestored)
    {
        if (m_pIMedia)
        {
            MP_LOG << "set position =" << m_tempDVDStatus.playbackTime << LOG_ENDL;
            m_pIMedia->SetCurrentMediaPosition(m_tempDVDStatus.playbackTime);
        }
        m_isDVDinfoRestored = false;
        m_tempDVDStatus.playbackTime = 0;
    }
	// } added by ravikanth - 12-09-26
    emit dvdInfochanged(nTitleNo, nChapterNo, nChapterSize, 1);
}

void
AppMediaPlayer_Controller_Audio::ScanComplete()
{
    MP_LOG << LOG_ENDL;

    MP::DEVICE_T source = GetSignalSource();

    if (getDevice() == source)
    {
        // { modified by wonseok.heo 2013.09.25 for HMC new spec
        if(source == MP::DISC)
        {
            if(m_discScanPlayingMode == SCANFILE) // modified by wonseok.heo for ITS 191295 2013.09.26
            {
                //setRepeatRandomMode(REPEATFOLDER, RANDOMOFF); //modified by wonseok.heo for ITS 207040 206815
                m_discScanPlayingMode = SCANOFF; // added by wonseok.heo for ITS 191295 2013.09.26
            }
            //{ modified by shkim 2013.11.12 for CDDA Scan Off failure Smoke Test Issue
            if(m_discType == MEDIA_SOURCE_CDDA)
            {
                emit stopScan();
            }
            //} modified by shkim 2013.11.12
        }
        else
        {
            setScanMode(SCANOFF); // changed by eugeny - 12-09-15
        }
        // } modified by wonseok.heo 2013.09.25
    }
}

void
AppMediaPlayer_Controller_Audio::requestMLTList()
{
    // { removed by kihyung 2013.07.19
    /*
    MP_LOG << LOG_ENDL;
    m_albumName = GetCurrentAlbumName();

    switch (m_device)
    {
        case MP::JUKEBOX:
            emit signalSetGracenoteVolumeType(0, NULL, NULL);
            break;
        case MP::USB1:
            emit signalSetGracenoteVolumeType(1, m_history[MP::USB1].DeviceID,
                                              m_history[MP::USB1].MountPath);
            break;
        case MP::USB2:
            emit signalSetGracenoteVolumeType(2, m_history[MP::USB2].DeviceID,
                                              m_history[MP::USB2].MountPath);
            break;
        case MP::IPOD1:
        case MP::IPOD2:
            break;
        case MP::DISC:
            m_albumName = m_history[m_device].Album;
            emit signalSetGracenoteVolumeType(3, NULL, NULL);
            break;
        default:
            break;
    }

    MP_LOG << "m_albumName after" << m_albumName << LOG_ENDL;
    QString XSampaDBPath("/app/data/gracenote/XSampaDB"); //modified by aettie.ji 2013.05.01 for Gracenote DB location change
    QFile dbpath(XSampaDBPath);
    if(dbpath.exists())
    {
        QString utf8;
        utf8.clear();
        utf8 = encodeString(m_albumName);
        if(!m_pTrackerAbstractor->RequestData(utf8, eGetXSampaId3NameQuery_Id))
        {
            MP_LOG << "m_pMoreLikeThis->call RequestData failed" << m_albumName << LOG_ENDL;
            m_pMoreLikeThis->call("CreateMoreLikeThis", "album", m_albumName);
        }
    }
    else
    {
        MP_LOG << "m_pMoreLikeThis->call XSampaDB doesn't exist" << m_albumName << LOG_ENDL;
        m_pMoreLikeThis->call("CreateMoreLikeThis", "album", m_albumName);
    }
    */
    // } removed by kihyung 2013.07.19
}

void
AppMediaPlayer_Controller_Audio::PlaylistDataReceived( PlaylistResults results )
{
   MP_LOG << LOG_ENDL;

   m_mltFilePathesList.clear();

   if (results.count <= 1) //modified by aettie 20130607 for ITS 171323
   {
      MP_LOG << "results.count <= 1 : only one song same to itself" << LOG_ENDL;
      emit showNoFoundPopup();
   }
   else
   {
      emit moreLikeThisCluster();
      m_moreLikeThisList.clear();
      m_moreLikeThisList_artist.clear(); //added by aettie.ji 2013.02.18 for ux optimization
      for (int i=0; i<results.count;i++)
      {
          QString filePath = results.playlistData.at(i).filePath;
          if(QFile::exists(filePath) || (getDevice() ==  MP::DISC) ||
             (getDevice() == MP::IPOD1) || (getDevice() == MP::IPOD2))
          {
              // { modified by kihyung 2012.11.29
              if(getDevice() != MP::DISC) m_mltFilePathesList.append(filePath);
              // } modified by kihyung 2012.11.29              
              m_moreLikeThisList.append(results.playlistData.at(i).title);
              MP_LOG << "res.playlistData.at(i).fileName: " << results.playlistData.at(i).title << LOG_ENDL;
			  m_moreLikeThisList_artist.append(results.playlistData.at(i).artist);     //added by aettie.ji 2013.02.18 for ux optimization	
			  MP_LOG << "res.playlistData.at(i).fileName: " << results.playlistData.at(i).artist << LOG_ENDL;
          }
       }
      //{changed by junam 2013.07.01 for mlt play icon
      //emit sendMoreLikeThisList( m_moreLikeThisList, m_moreLikeThisList_artist );     //modified by aettie.ji 2013.02.18 for ux optimization
      emit sendMoreLikeThisList( m_moreLikeThisList, m_moreLikeThisList_artist, m_mltFilePathesList );
      //}changed by junam
   }
   MP_LOG << "Exit" << LOG_ENDL;
}


void
AppMediaPlayer_Controller_Audio::playFromMLTList(int index)
{
    Q_UNUSED(index);
    // { removed by kihyung 2013.07.18 for ITS 0180268
    /*
    MP_LOG << "index" << index << LOG_ENDL;

    // { modified by kihyung 2012.11.29
    if(m_device == MP::DISC)
    {
        m_mltFilePathesList.clear();
        
        int nTrackIndex = -1;
        for(int indexMLT = 0; indexMLT < m_moreLikeThisList.count(); indexMLT++)
        {
            for(int indexCD = 0; indexCD < m_listCDContent.count(); indexCD++)
            {
                if(QString::compare(m_listCDContent.at(indexCD).trackTitle,
                                    m_moreLikeThisList.at(indexMLT)) == 0)
                {
                    m_mltFilePathesList.append(QString("%1").arg(indexCD));
                    if(nTrackIndex < 0 && indexMLT == index)
                        nTrackIndex = indexCD;
                }
            }
        }
        // } changed by junam
    }
    emit showPathView(true); // added by yongkyun.lee 2012.12.11 for CR 16366 No display cover flow on MLT list.
    setIsPlayFromMLT(true); // modified by Dmitry 27.04.13

    // { modified by kihyung 2013.06.07
    if(m_history[m_device].playList.isMLT == false) {
        m_history[m_device].playList.isMLT = true; //added by jeeeun for MLT play in FF/REW 2012.12.05
        CopyHistory(m_preMLTHistory[m_device], m_history[m_device]); // modified by kihyung 2013.4.3 for ISV 78555
    }else{
        MP_LOG << "m_history[m_device].playList.isMLT == true" << LOG_ENDL;
    }
    // } modified by kihyung 2013.06.07

    //{changed by junam 2013.06.11 for ITS173073
    //createPlayListFromList(m_mltFilePathesList, index, MP::DEFAULT);
    createPlayListFromList(m_mltFilePathesList, index, MP::MORELIKETHIS);
    //}changed by junam

    setRepeatRandomMode(REPEATALL, RANDOMOFF); // modified by kihyung 2013.05.27 for ITS 0167284
    emit setStatusDataByIndex(m_device, DATA_TYPE_RANDOM_ENABLE, DISABLE); //added by edo.lee 2013.05.06 for VR random disable
    // } modified by kihyung 2012.11.29
    */
    // } removed by kihyung 2013.07.18 for ITS 0180268
}

int
AppMediaPlayer_Controller_Audio::getSongNameListTempCount()
{
    if ( !m_songNameListTemp.isEmpty())
    {
        return m_songNameListTemp.count();
    }

    return -1;
}

void
AppMediaPlayer_Controller_Audio::clearTemps()
{
    m_pathsTemp.clear();
    m_songNameListTemp.clear();
    m_albumNameTemp.clear();
}

bool
AppMediaPlayer_Controller_Audio::isMinerHandleRequired ( int percentage)
{
    MP_LOG << "percentage =" << percentage << LOG_ENDL;

    if ( percentage > m_minerState.minerInProgress * m_minerState.minerInProgressPercentageStep )
    {
        if ( percentage < 100 )
        {
            m_minerState.minerInProgress++;
        }
        return true;
    }
    return false;
}

// { added by kihyung 2012.07.24 for CR 9673
bool
AppMediaPlayer_Controller_Audio::isMinerFinished()
{
    return m_minerState.isMinerFinished; // modified by eugene.seo 2013.04.15
}
// } added by kihyung

// { added by eugene.seo 2013.03.16
bool
AppMediaPlayer_Controller_Audio::isUSB1MinerFinished() // modified by eugene.seo 2013.04.15
{
    return m_minerState.isUSB1MinerFinished; // modified by eugene.seo 2013.04.15
}
// } added by eugene.seo 2013.03.16

// { added by eugene.seo 2013.04.15
bool
AppMediaPlayer_Controller_Audio::isUSB2MinerFinished()
{
    return m_minerState.isUSB2MinerFinished;
}
// } added by eugene.seo 2013.04.15

//{changed by junam 2013.11.06 for coverflow update
void AppMediaPlayer_Controller_Audio::getMusicAlbum(QVector<QStringList> &fileList)
{
    MP_LOG << "minerInProgress = " << m_minerState.minerInProgress << LOG_ENDL;

//    QStringList newAlbums;
//    QStringList newArtists;
//    QStringList newFiles;

 //   m_pPathViewModel->firstEntrySetter(false); //added by junam 2013.10.31 for ITS_KOR_198571

//    foreach ( QStringList list, fileList )
//    {
//        /* avoid to add albums with the same name.It happens if album name is the same, but artist is diff */
//        if (!newAlbums.contains(list[0]) && !list[0].isEmpty() && !list[1].isEmpty() && !list[2].isEmpty())
//        {
//            newAlbums << list[0];
//            newArtists << list[1];
//            newFiles << list[2];
//        }
//    }

//    if (newAlbums.empty())
//    {
//        m_minerState.minerInProgress = 0;
//    }
//    else if ( m_minerState.isMinerCleanRequired )
//    {
//        setIsCoverCleanRequired( false );
//        m_pPathViewModel->fillModelData( newAlbums, newArtists, newFiles );
//        //emit updatePathView( true );//removed by junam for 2012.12.06 flow view focus
//    }
//    else
//    {
//        if ( !newAlbums.isEmpty() )
//        {
//           m_pPathViewModel->appendModelData( newAlbums, newArtists, newFiles );
//        }
//    }
    emit clearCoverflowIndex(fileList.size());
    m_pPathViewModel->replaceModelData( fileList);
    if ( m_pIMedia )
    {
        MediaInfo info;
        m_pIMedia->GetCurrentMediaInfo(&info);
        emit setCoverflowIndex(m_pPathViewModel->getAlbumIndex(info.Album));
    }
}
//{ changed by junam

void
AppMediaPlayer_Controller_Audio::getMusicCoverArt(QVector<QStringList> fileList, uint reqUID)// add reqUID by junam 2012.10.25 for CR14451
{
    MP_LOG << "response eRetrieveCoverArtOnDemand_Id" << LOG_ENDL;

    QStringList newAlbums, newCovers;

    // m_coverartTemp.clear(); // deleted by junggil 2012.09.04 Not need

    foreach ( QStringList list, fileList )
    {
        if ( list.size() < 2 )
        {
            MP_LOG << "[ERROR] List size is too small" << LOG_ENDL;
            return;
        }
        /* avoid to add albums with the same name.It happens if album name is the same, but artist is diff */
        if ( !newAlbums.contains(list[0]) && !list[0].isEmpty() && !list[1].isEmpty())
        {
            newAlbums << list[0];
            newCovers << list[1];
            // { modified by junggil 2012.09.04 for Update info
            //m_coverartTemp.append(list[1]); // added by junggil 2012.07.31 for CR12069
            //m_coverartTemp = list[1]; //removed by junam 2012.11.02 for unusing code.
            // } modified by junggil
        }
    }

    if (!newAlbums.isEmpty())
    {
        if (reqUID == GET_TA_REQUID(REQUID_REQUST_COVER_LIST_VIEW)) {
            m_pListViewModel->fillAlbumCover (newAlbums, newCovers); 
        }
        else if(reqUID == GET_TA_REQUID(REQUID_REQUST_COVER_FLOW_VIEW)) {
            m_pPathViewModel->fillAlbumCover (newAlbums, newCovers);
        }
        else {
            MP_LOG<<"ERROR:unhanded request uid = "<< reqUID << LOG_ENDL;
        }
        // } changed by junam
    }
}

void
AppMediaPlayer_Controller_Audio::getMusicAlbumCoverArtComplete(QVector<QStringList> fileList)
{
    MP_LOG << LOG_ENDL;

    QStringList newCovers;
    foreach ( QStringList list, fileList )
    {
        if (list.count() > 0)
        {
            // { commented by kihyung 2012.10.10
            // MP_LOG << "Cover URI: " << list[0] << LOG_ENDL;
            // } commented by kihyung
            newCovers << list[0];
        }
    }

    for (int n = 0; n < m_listCDContent.count(); n++)
    {
        CD_META_INFO track = m_listCDContent.at(n);

        track.cover = newCovers.at(0);
        track.cover.remove( "file://" );

        m_listCDContent.replace(n, track);
    }
    //{ added by yongkyun.lee 20130702 for : NO CR - MP3 CD song info update
    DisplayCDInfo(GetCurrentTrackIndex(), false);
    //DisplayCDInfo(GetCurrentTrackIndex());
    //} added by yongkyun.lee 20130702 
}

void
AppMediaPlayer_Controller_Audio::LoadGracenoteData()
{
    MP_LOG << LOG_ENDL;
    m_pTrackerAbstractor->RequestData(m_history[MP::DISC].DeviceID, eGetMusicFromCDQuery_Id);
}

void
//{ added by yongkyun.lee 20130702 for : NO CR - MP3 CD song info update
AppMediaPlayer_Controller_Audio::DisplayCDInfo( int trackIndex , bool updateTitle )
//AppMediaPlayer_Controller_Audio::DisplayCDInfo( int trackIndex  )
//} added by yongkyun.lee 20130702 
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    MP_LOG << "index =" << trackIndex << LOG_ENDL;

    if (trackIndex < m_listCDContent.count() && device == MP::DISC)
    {
        CD_META_INFO track;

        if(m_isPlayFromMLT == false) {
            track = m_listCDContent.at(trackIndex);
        }
        else {
            CDDAStatusResponse tCDInfo;
            m_pIAudio->GetCDStatus(&tCDInfo);

            int nNewTrackIdx = tCDInfo.trackNo - tCDInfo.startTrackNo;
            if(nNewTrackIdx < 0 || nNewTrackIdx >= m_listCDContent.count()) {
                return;
            }
            MP_LOG << "NEW index =" << nNewTrackIdx << LOG_ENDL;
            track = m_listCDContent.at(nNewTrackIdx);
        }

        MediaInfo info;
        info.Title = track.trackTitle;
        info.Album = track.albumTitle;
        info.Artist = track.artist;
        info.coverarturl = track.cover;
		info.Genre = track.genre; //added by lyg  2012.08.30  for 3DPMS_136434 Audio Cd File info
        info.Composer = track.composer; //added by lyg  2012.08.30  for 3DPMS_136434 Audio Cd File info

        // { added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
        if (m_pIMedia && m_history[device].ContentPlayed == MP::FOLDER)
        {
            int fileIndex = -1;
            int fileCount = -1;
            m_pIMedia->GetCurrentFileFolderIndex(&fileIndex, &fileCount);
            //{ modified by cychoi 2013.08.29 for HMC QC IPOD cluster display on Bootup
            emit mediaInfoClusterChanged(info.Title);
            emit trackClusterChanged(fileIndex + 1, m_history[device].Quantity);
            //} modified by cychoi 2013.08.29
        }
        else
        {
            int trackIndex = GetCurrentTrackNumber();
            //{ modified by cychoi 2013.08.29 for HMC QC IPOD cluster display on Bootup
            emit mediaInfoClusterChanged(info.Title);
            emit trackClusterChanged(trackIndex, m_history[device].Quantity);
            //} modified by cychoi 2013.08.29
        }
        // } added by cychoi 2013.08.17

        onMediaInfoChanged(&info);
    }
	//{ Added by by eunhye 2013.02.22 for ISV 70720
    else
    {
        // { modified by wonseok.heo for ITS 188212 2013.09.04
        //QString trackName(GetCurrentFileName());
        QString trackName;
        if(device == MP::DISC && m_discType == MEDIA_SOURCE_MP3){
            trackName = m_history[device].Title;
        }else{
            trackName = GetCurrentFileName();
        }
        // } modified by wonseok.heo for ITS 188212 2013.09.04

        if (!trackName.isEmpty())
        {
            MediaInfo info;
            // { added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
            if (m_pIMedia && m_history[device].ContentPlayed == MP::FOLDER)
            {
                int fileIndex = -1;
                int fileCount = -1;
                m_pIMedia->GetCurrentFileFolderIndex(&fileIndex, &fileCount);
                if (fileIndex >= 0)
                {
                    // { modified by wonseok.heo for ITS 188212 2013.09.04
//                    if(updateTitle && ( device == MP::DISC && m_discType == MEDIA_SOURCE_MP3 ))
//                    {
//                        emit mediaInfoClusterChanged(m_history[device].Path);
//                    }
//                    else
//                    {
//                        emit mediaInfoClusterChanged(trackName);
//                    }
                    // { modified by yungi 2013.11.15 for Cluster AudioCD - Track NO displayed
                    if( m_discType == MEDIA_SOURCE_CDDA )
                        emit mediaInfoClusterChanged(QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_pIMedia->GetCurrentIndex() + 1));
                    else
                        emit mediaInfoClusterChanged(trackName);
                    // } modified by yungi
                    // } modified by wonseok.heo for ITS 188212 2013.09.04
                    emit trackClusterChanged(fileIndex + 1, m_history[device].Quantity);
                }
            }
            else
            {
                int trackIndex = GetCurrentTrackNumber();
                if(trackIndex > 0)
                {
                    // { modified by wonseok.heo for ITS 188212 2013.09.04
//                    if(updateTitle && ( device == MP::DISC && m_discType == MEDIA_SOURCE_MP3 ))
//                    {
//                        emit mediaInfoClusterChanged(m_history[device].Path);
//                    }
//                    else
//                    {
//                        emit mediaInfoClusterChanged(trackName);
//                    }
                    // { modified by yungi 2013.11.15 for Cluster AudioCD - Track NO displayed
                    if( m_discType == MEDIA_SOURCE_CDDA )
                    {
                    	if(m_pIMedia != NULL)
                        	emit mediaInfoClusterChanged(QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_pIMedia->GetCurrentIndex() + 1));
                    }
                    else
                        emit mediaInfoClusterChanged(trackName);
                    // } modified by yungi
                    // } modified by wonseok.heo for ITS 188212 2013.09.04
                    emit trackClusterChanged(trackIndex, m_history[device].Quantity);
                }
            }
            // } added by cychoi 2013.08.17
            //{ added by yongkyun.lee 20130702 for : NO CR - MP3 CD song info update
            if(updateTitle && ( device == MP::DISC && m_discType == MEDIA_SOURCE_MP3 ))
            {
                MP_LOG << "Folder ::" << m_history[device].Folder << LOG_ENDL;

                //{ modified by yongkyun.lee 2013-08-15 for :  NO CR  MP3 ID3-tag update
                QString coverart = m_history[device].Cover ;
                QString artist   = m_history[device].Artist == "" ? "" : m_history[device].Artist;// modified by wonseok.heo for NO CR delete unknown 2013.08.20
                QString song     = trackName;
                QString album    = m_history[device].Album  == "" ? "" : m_history[device].Album;// modified by wonseok.heo for NO CR delete unknown 2013.08.20
               
                emit mp3fileInfo(coverart, artist, song, album, m_history[device].Folder, ""); //modified by wonseok.heo for ITS 188212 2013.09.04
                //} modified by yongkyun.lee 2013-08-15 
                m_strUpdatedMediaInfo = ""; // modified by yongkyun.lee 2013-10-02 for : ITS 192608
            }
            else           
            //} added by yongkyun.lee 20130702 
            {
                info.Title = trackName;
                onMediaInfoChanged(&info);
            }
        }
    }
	//} Added by by eunhye 2013.02.22
}

void AppMediaPlayer_Controller_Audio::onCDEjected()
{

    MP::DEVICE_T device = getDevice(); //added by wonseok.heo for ITS 185648 2013.08.29

    m_listCDContent.clear();
    ResetDeviceInfo(MP::DISC);
    // { added by lssanh 2013.04.28 play mode
    //m_bFirstEntry[MP::DISC]= false; //removed by junam 2014.01.17 deprecated code
    //{ added by yongkyun.lee 20130429 for : NO CR play mode 
    //m_history[m_device].repeatPlayMode = REPEATALL; 
    //m_history[m_device].randomPlayMode = RANDOMOFF;
    m_history[MP::DISC].repeatPlayMode = REPEATALL; 
    m_history[MP::DISC].randomPlayMode = RANDOMOFF;
    //} added by yongkyun.lee 20130429 
    // } added by lssanh 2013.04.28 play mode

    //{ modified by cychoi 2013.11.14 for ITS 209153 CDDA Scan Off failure on CDEjected
    //{ modified by yongkyun.lee 2013-08-06 for : ISV 88607
    if(m_scanMode != SCANOFF && device ==  MP::DISC )  // modified by wonseok.heo for ITS 185648 2013.08.29
    {
        if(m_discType == MEDIA_SOURCE_CDDA)
        {
            emit stopScan();
        }
        onModeChanged(SCANOFF);
    }
    //} modified by yongkyun.lee 2013-08-06 
    //} modified by cychoi 2013.11.14

    m_history[MP::DISC].Folder = "";// modified by yongkyun.lee 2013-07-29 for : NO CR MP3 folder name
    m_discType = MEDIA_SOURCE_MAX;
    m_nID3TagTrackIdx = -1; // added by cychoi 2014.01.07 for keep MP3 ID3 Tag if same song 

	// { modified by wonseok.heo for ITS 205774 2013.11.01
	//     if (m_pTreeModel)
	//     {
	//         m_pTreeModel->releaseModelData();
	//     }
	// } modified by wonseok.heo for ITS 205774 2013.11.01
    //emit activateTab(MP::DISC, false, false); // commented by cychoi 2013.07.25 for SmokeTest Disc screen is not disappear // added by minho 20120821 for NEW UX: Added active tab on media
    disconnect(m_pDeckController, 0, this, 0); //move by changjin 2012.07.26 for CR11681

    //{ commented by cychoi 2013.07.25 for SmokeTest Disc screen is not disappear
    //if (m_device == MP::DISC )
    //{
    //    setPlayerMode(MP::UNDEFINED);
        //emit showDiscEjectedPopup(); //removed by junam 2012.10.16 for CR14562
    //}
    //{ commented by cychoi 2013.07.25
    //{ added by yongkyun.lee 20130619 for : NO CR play mode 
    AppMediaPlayer_SettingsStorage::GetInstance()->SaveAudioSettings(this);
    //} added by yongkyun.lee 20130617 
}

void AppMediaPlayer_Controller_Audio::commandFromBT(int state)
{
   MP_HIGH << "state =" << state << LOG_ENDL;
   //{ deleted by yongkyun.lee 2013-10-10 for :  ITS 191703 BT getinfo 0.5sec

    switch(state)
    {
    //{ modified by yongkyun.lee 2013-12-02 for : ISV 95731
    case EVT_BT_MUSIC_STREAMING_STARTED:     
        m_isBTStreaming = true;
        emit bluetoothPlaybackStarted();
        break;
    case EVT_BT_MUSIC_STREAMING_PAUSED:     
        m_isBTStreaming = false;
        emit bluetoothPlaybackPaused();
        break;
    //} modified by yongkyun.lee 2013-12-02 
    case EVT_BT_MUSIC_PLAYBACK_STARTED:     
        m_bIsBTPlaying = true;
        break;
    case EVT_BT_MUSIC_STOP:     
    case EVT_BT_MUSIC_PLAYBACK_PAUSED:     
        m_bIsBTPlaying = false;
        break;
   }
}

//{ added by hyochang.ryu 20130601
void AppMediaPlayer_Controller_Audio::onBTFunc(bool _pauseplay)
{
	(_pauseplay) ? bluetoothPlaybackStarted() : bluetoothPlaybackPaused();
}
//} added by hyochang.ryu 20130601

//{ added by hyochang.ryu 20130731 for ITS181088
void AppMediaPlayer_Controller_Audio::showBTCallPopup()
{
    MP_LOG << LOG_ENDL;
    emit sigShowBTCallPopup();//{ modified by yongkyun.lee 2013-10-10 for : ITS 193552
}
//} added by hyochang.ryu 20130731 for ITS181088

//{ added by hyochang.ryu 20130913 for ITS190345
void AppMediaPlayer_Controller_Audio::closeBTCallPopup()
{
    emit sigCloseBTCallPopup();
}
//} added by hyochang.ryu 20130913 for ITS190345

// { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
void AppMediaPlayer_Controller_Audio::showDeckHighTempPopup()
{
    MP_LOG << LOG_ENDL;
    emit sigShowDeckHighTempPopup();
}

void AppMediaPlayer_Controller_Audio::closeDeckHighTempPopup()
{
    MP_LOG << LOG_ENDL;
    emit sigCloseDeckHighTempPopup();
}
// } added by cychoi 2014.04.09

void AppMediaPlayer_Controller_Audio::BTPluggedOUT()
{
   MP_LOG << LOG_ENDL;
   //{ modified by yongkyun.lee 2013-10-30 for : NOCR BT Streaming stop
   m_isBTStreaming = false;
   emit bluetoothPlaybackPaused();
   //} modified by yongkyun.lee 2013-10-30 
   //{ modified by yongkyun.lee 2013-10-16 for : ITS 195932
   m_history[MP::BLUETOOTH].Title="";
   m_history[MP::BLUETOOTH].Artist="";
   m_history[MP::BLUETOOTH].Album="";

    if (getDevice() == MP::BLUETOOTH) //added by junam 2013.11.04 for ITS_KOR_206325
        GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->hideBTMusic();
   //} modified by yongkyun.lee 2013-10-16
}

bool
AppMediaPlayer_Controller_Audio::getTuneTrackNamesFromCATree()
{
    QList<TuneTrackInfo> trackInfoList;
    int tmpIndex = -1;
    trackInfoList = m_pTreeModel->getTrackInfoListFromCATree();

    if ( m_nPendingMedia != -1 &&
         m_nPendingMedia < m_tuneTrackIDs.count() )
    {
        tmpIndex = m_tuneTrackIDs[m_nPendingMedia];
    }

    if ( !trackInfoList.isEmpty() )
    {
        m_tuneTrackNames.clear();
        m_tuneTrackIDs.clear();
        m_pathsTune.clear();//added by junam 2012.12.14 for CR16128

        foreach ( TuneTrackInfo trackInfo, trackInfoList )
        {
            m_tuneTrackNames << trackInfo.name;
            m_tuneTrackIDs << trackInfo.id;
            m_pathsTune << trackInfo.folder;//added by junam 2012.12.14 for CR16128
        }

        //deleted by yongkyun.lee 2013-09-02 for : tune error

        return true;
    }

    return false;
}

bool AppMediaPlayer_Controller_Audio::tuneWheel(bool bTuneForward)
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return false;
    

    if(device == MP::IPOD1 || device == MP::IPOD2)
    {
        CIpodController *pController = GetIPodController(device);
        if(pController)
        {
            if(pController->isAcceptingTuneDial() == false)
                return false;
        }
    }//{added by junam 2013.11.29 for tune
    else if (device == MP::JUKEBOX ||  device == MP::USB1  || device == MP::USB2 || device == MP::DISC)
    {
        // { modified by sangmin.seol 2014.03.24 ITS 0230803 RollBack tune wheel skip conditon for scanmode autotrackchange case
        if ( m_moveTrackIndex >= 0 && m_moveTrackIndex != m_pIMedia->GetCurrentIndex())
        {
            MP_LOG<<"Track is changing from "<<m_pIMedia->GetCurrentIndex()<<" to "<<m_moveTrackIndex << LOG_ENDL;
            return false;
        }
        // } modified by sangmin.seol 2014.03.24 ITS 0230803 RollBack tune wheel skip conditon for scanmode autotrackchange case
    }
//}added by junam

    // { added by junam 2012.8.29 for CR12745
    if (device == MP::JUKEBOX ||
        device == MP::USB1  || device == MP::USB2 ||
        device == MP::IPOD1 || device == MP::IPOD2)
    {
        //m_tuneCoverArtTimer.stop();//removed by junam 2013.10.17 for deprecated code

        if (!m_pIMedia)
            return false;

        //added by edo.lee 2013.06.29
        if(m_pIMedia->GetCurrentIndex() != m_history[device].Index
             && device != MP::IPOD1 && device != MP::IPOD2) //Tan, exclude ipod from this logic.
            setCurrentIndex(m_history[device].Index);
        //added by edo.lee 2013.06.29

        //added by edo.lee 2013.06.07 for reset scan mode when tune
        //{removed by junam 2013.06.10 for apply iPod also
        //if ( m_device == MP::JUKEBOX ||
        //        m_device == MP::USB1 || m_device == MP::USB2 )
        //}removed by junam
        setScanMode(SCANOFF);

        int iStart=0, iEnd=0;
        if (m_history[device].ContentPlayed == MP::FOLDER)
        {
            m_pIMedia->GetCurrentFileFolderIndex(&iStart, &iEnd);
            iStart = m_pIMedia->GetCurrentIndex() - iStart;
            iEnd = iStart + iEnd - 1;
        }
        else
        {
            iStart = 0;
            iEnd = m_history[device].Quantity - 1;
        }

        if (!m_tuneState.m_nTuneStart)
        {
            m_tuneState.m_nTuneStart = true;
            m_tuneState.idx_tune = -1; // added by junam 2012.11.9 for tune speed

            //{changed by junam 2013.06.12 for tune
            //m_nPendingMedia =  m_pIMedia->GetCurrentIndex();
            m_nPendingMedia = m_history[device].Index;
            //}changed by junam

            if (m_nPendingMedia < 0)
                m_nPendingMedia = 0;
        }

        if (bTuneForward)
        {
            m_tuneState.m_nTuneFF = true;
            m_nPendingMedia++;

            // { changed by junam 2012.10.06 for folder tune
            //if(m_history[m_device].Quantity <=  m_nPendingMedia)
            //    m_nPendingMedia = 0;
            if( iEnd <  m_nPendingMedia)
                m_nPendingMedia = iStart;
            // } changed by junam
        }
        else
        {
            m_tuneState.m_nTuneBack = true;

            // { changed by junam 2012.10.06 for folder tune
            //if(m_nPendingMedia > 0)
            //    m_nPendingMedia--;
            //else
            //    m_nPendingMedia = m_history[m_device].Quantity - 1;
            if(m_nPendingMedia > iStart)
                m_nPendingMedia--;
            else
                m_nPendingMedia = iEnd;
            // } changed by junam
        }
        //resetScan(); // modified by wspark 2012.09.04 for 3PDMS #136569

        // { added by lssanh 2013.03.09 ISV72498 tune center
        if (m_history[device].Index== m_nPendingMedia)
        {        	
           GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->onTuneTimerStop();
		   GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->showCurrentOSD();
           //GetEngineMain().GetNotifier()->onSetDisplayOSD(true);
           //emit updateOSD( OSD_EVENT_AUDIO_TRACK_INFO );

           m_tuneState.m_nTuneStart = false; //added by junam 2013.06.26
           return false;
        }
        // } added by lssanh 2013.03.09 ISV72498 tune center
		
        emit playbackTuned(); // // added by wspark 2012.09.04 for 3PDMS #136569
        //emit tuneCounter(GetFilesCountInfo( m_nPendingMedia, m_history[m_device].Quantity));//removed by junam 2013.04.05 for count sync

        //{added by junam 2013.03.10 for ISV73458
        if(m_tuneMetaInfoTimer.isActive() == FALSE || m_tuneMetaInfoTimer.interval() != TUNE_TIME_METAINFO_RESPONSE)
        {
        	 MP_LOG<<" USB1, USB2 mining :  "<<  isUSB1MinerFinished()<< isUSB2MinerFinished() << LOG_ENDL;
        	//modified by edo.lee 2013.05.23
        	//if((m_device != MP::USB1 && m_device != MP::USB2) || //added by edo.lee 2013.05.20
			//	(m_device == MP::USB1 && isUSB1MinerFinished())||(m_device == MP::USB2 && isUSB2MinerFinished())) 
			//{
			//	m_tuneMetaInfoTimer.start(TUNE_TIME_METAINFO_REQUEST); 
			//}    		  
			//else
			//{
			//	 MP_LOG<<" tune Search  "<< m_nPendingMedia << LOG_ENDL;
			//	 emit tuneCounter(GetFilesCountInfo( m_nPendingMedia, m_history[m_device].Quantity));
			//}
			//if((device == MP::USB1 || device == MP::USB2) || 
			//	(device == MP::USB1 && !isUSB1MinerFinished())||(device == MP::USB2 && !isUSB2MinerFinished())) 
                        if((device == MP::USB1 && !isUSB1MinerFinished())||(device == MP::USB2 && !isUSB2MinerFinished()))// Modify by Taihyun.ahn 2013.10.05
				{
	           		emit tuneCounter(GetFilesCountInfo( m_nPendingMedia, m_history[device].Quantity));
				}
			m_tuneMetaInfoTimer.start(TUNE_TIME_METAINFO_REQUEST); 
			//modified by edo.lee 2013.05.23
		}
        else
            MP_LOG<<"Already requested... ignore." << LOG_ENDL;
        //}added by junam
        return true;
    }
    // } added by junam

    if ( device == MP::UNDEFINED || 
         m_tuneState.m_nTuneFF || m_tuneState.m_nTuneBack || 
         (m_pIMedia == NULL && device != MP::DISC) ) 
    {
        MP_LOG << "Ignoring" << LOG_ENDL;
        //{ modified by yongkyun.lee 2013-07-26 for : NOCR tune error
        if (device == MP::DISC && (m_discType == MEDIA_SOURCE_CDDA || m_discType == MEDIA_SOURCE_MP3))
        {
            tuneReset();
        }
        //} modified by yongkyun.lee 2013-07-26 
        return false;
    }

    //emit playbackTuned(); // modified by yongkyun.lee 2013-08-19 for : song count  1 is no tune

    m_tuneState.m_nTuneStart = true; //added by junam 2012.10.11 for CR14346

    if (bTuneForward) m_tuneState.m_nTuneFF = true;
    else m_tuneState.m_nTuneBack = true;

    if (m_nPendingMedia == -1)
    {
        m_artistTune = m_history[device].Artist;
        m_titleTune = m_history[device].Title;
        
        // { added by junggil 2012.07.31 for CR12069
        m_albumTune = m_history[device].Album;
        m_folderTuneTemp = m_history[device].Path;
		
		//{modified by aettie.ji 2013.01.31 for ISV 70943
       // if( m_device == MP::DISC)  // junggil - Temporary code!!! If you want to change for dynamic folder name It need a member of folder on MediaInfo struct!!!
           // m_folderTune = "Music";
        m_folderTune = m_history[device].Folder; 
		//}modified by aettie.ji 2013.01.31 for ISV 70943
        m_genreTune = m_history[device].Genre;
        m_composerTune = m_history[device].Composer;
        m_coverartTune = m_history[device].Cover;
        // } added by junggil

        if ( device == MP::DISC )
        {
            m_tuneTrackNames.clear();
            m_tuneState.idx_tune = -1;

            if (m_discType == MEDIA_SOURCE_MP3 ||
                m_discType == MEDIA_SOURCE_DVDAUDIO ||
                m_discType == MEDIA_SOURCE_CDDA) // Modify by changjin 2012.06.29 for CR 8042, 9444
            {
                if ( m_listCDContent.count() > 0 )
                {
                    for ( int i = 0; i < m_listCDContent.count(); i ++ )
                    {
                        m_tuneTrackNames << m_listCDContent[i].trackTitle;
                    }
                }
                else
                {
                    for ( int i = 0; i < m_history[MP::DISC].Quantity; i++ )
                    {
                        //{ modified by cychoi 2013.10.19 for "Track" display by Language Setting
                        m_tuneTrackNames << QString::number(i + 1); // modified by wonseok.heo for ITS 197433 2013.10.28 //QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(i + 1);
                        //m_tuneTrackNames << QString("Track %1").arg( i + 1 );
                        //} modified by cychoi 2013.10.19
                    }
                }

                if ( !m_tuneTrackNames.isEmpty() )
                {
                    m_nPendingMedia = m_history[MP::DISC].Index;
                }
                setScanMode(SCANOFF); //added by Michael.Kim 2013.06.20 for ITS Issue #0175120
            }
            else
            {
                getTuneTrackNamesFromCATree();
            }
        }
    }

    if ( device == MP::DISC )
    {
        bool isTrackNamesAvailable = true;

        if (m_discType == MEDIA_SOURCE_MP3)
        {
            isTrackNamesAvailable = getTuneTrackNamesFromCATree();
        }

        if ( isTrackNamesAvailable )
        {
            // { changed by junam 2012.10.06 for folder tune
            int iStart=0, iEnd=0;
            if (m_history[device].ContentPlayed == MP::FOLDER)
            {
                if(m_pIMedia != NULL)//Added by Taihyun.ahn 2013.10.05
                {
                    m_pIMedia->GetCurrentFileFolderIndex(&iStart, &iEnd);
                    iStart = m_pIMedia->GetCurrentIndex() - iStart;
                    iEnd = iStart + iEnd - 1;
                }

            } // { added by junam 2012.12.05 for CD MLT tune
            else if(m_isPlayFromMLT)
            {
                iStart = 0;
                iEnd = m_mltFilePathesList.count() - 1;
            } // } added by junam
            else
            {
                iStart = 0;
                iEnd = m_tuneTrackNames.count() - 1;
            }

            m_nPendingMedia += bTuneForward ? (+1) : (-1);
            if (m_nPendingMedia < iStart)
                m_nPendingMedia = iEnd;
            else if (m_nPendingMedia > iEnd)
                m_nPendingMedia = iStart;

           // if (m_nPendingMedia < 0)
           // {
           //    m_nPendingMedia = m_tuneTrackNames.count() - 1;
           // }
           // else if (m_nPendingMedia >= m_tuneTrackNames.count())
           // {
           //    m_nPendingMedia = 0;
           // }
            // } changed by junam
            // { added by lssanh 2013.03.09 ISV72498 tune center
            if (m_history[device].Index== m_nPendingMedia)
            {
               GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->onTuneTimerStop();
               // { modified by cychoi 2014.03.26 for ITS 213525, 213526, 213527 CDDA OSD on language change
               showCurrentDiscOSD();
			   //GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->showCurrentOSD();
               // } modified by cychoi 2014.03.26
               GetEngineMain().GetNotifier()->onSetDisplayOSD(true);
               m_tuneState.m_nTuneStart = false; //modified by yongkyun.lee 2013-09-02 for : tune error
               emit updateOSD( OSD_EVENT_AUDIO_TRACK_INFO );
               return false;
            }
            // } added by lssanh 2013.03.09 ISV72498 tune center
            // { modified by wonseok.heo for ISV 92439 2013.10.11
            QString tuneCount = GetFilesCountInfo( m_nPendingMedia, m_history[MP::DISC].Quantity);
            if (tuneCount == ""){
                //tuneReset();
                return false;
            } // } modified by wonseok.heo for ISV 92439 2013.10.11
            updateCDTuneInfo();
            emit playbackTuned(); // modified by yongkyun.lee 2013-08-19 for : song count  1 is no tune
        }
    }
    return true;
}

//{chagned by junam 2013.03.10 for ISV73458.
// modified by Dmitry 13.10.13
void AppMediaPlayer_Controller_Audio::onLongBtnTimerExpired()
{
   if(m_pIMedia)
   {
       if(m_bFF)
           m_pIMedia->FastForward(FAST_SPEED_X4);
       else
           m_pIMedia->Rewind(FAST_SPEED_X4);
   }
}
//{ added by jaehwan.lee 2014.02.27 to fix ITS 227260
void AppMediaPlayer_Controller_Audio::onScanTimerExpired()
{
    MP_HIGH << getDevice() << LOG_ENDL;

    if ( getDevice() == MP::USB1 || getDevice() == MP::USB2)
    {
        // scan next file
        m_pUsbController->ContinueScan();
        //m_pUsbController->Scan(m_scanMode);
    }
    else if( getDevice() == MP::JUKEBOX)
    {
        m_pJukeboxController->ContinueScan();
        //m_pUsbController->Scan(m_scanMode);
    }
}
//added by jaehwan.lee

//void AppMediaPlayer_Controller_Audio::onTuneRequestTimer(void)
void AppMediaPlayer_Controller_Audio::onTuneMetaInfoTimeOver(void)
{//chagned by junam

    MP::DEVICE_T device = getDevice();

    if ( device == MP::DISC || device == MP::UNDEFINED)
    {
        MP_LOG << "return m_device = "<<device << LOG_ENDL;
        return;
    }

    //{changed by junam 2013.04.13 for AutoStart
    //if((m_nPendingMedia < 0) || (m_nPendingMedia >= m_history[m_device].playList.list->size()))
    //{
    //    MP_LOG<<"m_nPendingMedia out of bound = "<<m_nPendingMedia <<"/"<< m_history[m_device].playList.list->size() << LOG_ENDL;
    //    return;
    //}
    if((m_nPendingMedia < 0) || (m_nPendingMedia >= m_history[device].Quantity))
    {
        MP_LOG<<"m_nPendingMedia out of bound = "<<m_nPendingMedia <<"/"<< m_history[device].Quantity << LOG_ENDL;
        return;
    }
    //}changed by junam

    //{ reverted by junam  2012.11.9 for tune speed
    if(m_tuneState.idx_tune == m_nPendingMedia)
    {
        //MP_LOG<<"already handling... skipped : "<<m_nPendingMedia << LOG_ENDL;
        return;
    }    	
    //}reverted by junam
    m_tuneState.idx_tune = m_nPendingMedia;
    //m_tuneCoverArtTimer.stop(); //removed by junam 2013.10.17 for deprecated code

//{changed by junam 2013.04.13 for AutoStart
    QString filePath;
    if(m_history[device].playList.list != NULL && m_tuneState.idx_tune < m_history[device].playList.list->count() )
    {
        filePath=m_history[device].playList.list->at(m_tuneState.idx_tune);
        if (!filePath.startsWith("file://"))
            filePath.prepend("file://");
    }

    m_tuneMetaInfoTimer.start(TUNE_TIME_METAINFO_RESPONSE);

    MP_LOG<<"call eGetMusicSimpleMetaDataQuery_Id "<< m_history[device].playList.list->count() << LOG_ENDL;

    if ( device == MP::IPOD1 || device == MP::IPOD2)
    {
        CIpodController *pController = GetIPodController(device);
        if(pController)
        {
            MP_LOG<<"call here  " << LOG_ENDL;
            if(!pController->requestData( eGetMusicSimpleMetaDataQuery_Id, GET_TA_REQUID(REQUID_ROTATE_TUNE), m_tuneState.idx_tune, 1))
            {
                MP_LOG<<"call here  2" << LOG_ENDL;
                m_pTrackerAbstractor->RequestData( filePath, eGetMusicSimpleMetaDataQuery_Id, GET_TA_REQUID(REQUID_ROTATE_TUNE));
            }
        }
    }
    else
    {
        // { modified by sungha.choi 2013.07.01
        //if ( (device == MP::JUKEBOX && !isMinerFinished()) || (device == MP::USB1 && !isUSB1MinerFinished()) || (device == MP::USB2 && !isUSB2MinerFinished()) )
        if ( device == MP::JUKEBOX || device == MP::USB1 || device == MP::USB2 )    // modified by sungha.choi 2013.08.13 for P1-pretest always file name will be showed
        {
            //m_pTrackerAbstractor->RequestData( filePath, eTrackerExtractMetadataQuery_Id, GET_TA_REQUID(REQUID_ROTATE_TUNE)); // deleted by sungha.choi 2013.07.13
            // { added by sungha.choi 2013.07.13
            MP_LOG << "show file name instead of rquesting to TA eGetMusicSimpleMetaDataQuery_Id" << LOG_ENDL;
            QString fileName = filePath;
            fileName.remove(0, fileName.lastIndexOf('/') + 1);

            if((m_tuneState.idx_tune < 0) || m_tuneState.idx_tune >= m_history[device].Quantity)
                return;

            m_tuneMetaInfoTimer.stop();
            //m_tuneCoverArtTimer.stop();//removed by junam 2013.10.17 for deprecated code

            if(m_tuneState.m_nTuneStart == false)
            {
                MP_LOG<<"tune search already stopped" << LOG_ENDL;
                return;
            }

            emit tuneSameFileNoti(m_tuneState.idx_tune == m_history[device].Index);

            EmitOnTuneMetaData( "", //artist    // modified by sungha.choi 2013.08.16 for requesting HMC
                               fileName, //song,
                               "", //album,    // modified by sungha.choi 2013.08.16 for requesting HMC
                               getFolderName(filePath), //folder
                               m_tuneState.idx_tune != m_history[device].Index ); // modified by kihyung 2013.08.27 for ITS 0186708

            // { added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
            //emit mediaInfoClusterChanged(fileName);
            //emit trackClusterChanged(m_tuneState.idx_tune, m_history[device].Quantity);
            // } added by cychoi 2013.08.17

		    GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData(
                (OSD_DEVICE)getOSDevice(device), fileName, m_randomPlayMode, m_repeatPlayMode, m_scanMode, true);
			m_tuneTitle = fileName;

            emit tuneCounter(GetFilesCountInfo( m_tuneState.idx_tune, m_history[device].Quantity));//added by junam 2013.04.05 for count sync

            if (m_bIsBasicView)
            {
                if(m_tuneState.idx_tune == m_nPendingMedia)
                {
                    m_tuneState.coverRequestIndex = m_tuneState.idx_tune;
                    m_tuneState.coverRequestAlbum = "";
                    m_tuneState.coverRequestArtist = "";
                    m_tuneState.coverRequestURI = "";
                }
                else
                {
                    m_tuneMetaInfoTimer.start(TUNE_TIME_METAINFO_REQUEST);
                }
            }
            else
            {
                m_tuneMetaInfoTimer.start(TUNE_TIME_METAINFO_REQUEST);
            }
            // } added by sungha.choi 2013.07.13
        }
        else
        {
            MP_LOG << "request to TA eGetMusicSimpleMetaDataQuery_Id" << LOG_ENDL;
            m_pTrackerAbstractor->RequestData( filePath, eGetMusicSimpleMetaDataQuery_Id, GET_TA_REQUID(REQUID_ROTATE_TUNE));
        }
        // } modified by sungha.choi 2013.07.01
    }
//}changed by junam

}
//added by junam

//{removed by junam 2013.10.17 for deprecated code
//void AppMediaPlayer_Controller_Audio::onTuneCoverArtTimeOver(void)
//{
//    MP::DEVICE_T device = getDevice();
    
//    if ( device == MP::DISC || device == MP::UNDEFINED)
//    {
//        MP_LOG << "return m_device = "<<device << LOG_ENDL;
//        return;
//    }

//    if (m_bIsBasicView && m_tuneState.coverRequestIndex == m_nPendingMedia)
//    {
//        //emit setTuneRequestTime(5000);
//        //m_tuneMetaInfoTimer.start(TUNE_TIME_METAINFO_RESPONSE);
//        MP_LOG << "m_bIsBasicView eRetrieveCoverArtOnDemand_Id. m_nPendingMedia:" << m_nPendingMedia << LOG_ENDL;
//        m_pTrackerAbstractor->RequestData(QStringList(m_tuneState.coverRequestAlbum),
//                                          QStringList(m_tuneState.coverRequestArtist),
//                                          QStringList(m_tuneState.coverRequestURI),
//                                          eRetrieveCoverArtOnDemand_Id,  GET_TA_REQUID(REQUID_ROTATE_TUNE));
//    }

//    m_tuneMetaInfoTimer.start(TUNE_TIME_METAINFO_REQUEST);
//    //emit setTuneRequestTime(200);
//}
//}removed by junam

void AppMediaPlayer_Controller_Audio::switchToTuned()
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    // { deleted by edo.lee 2012.11.28  for ISV 64487 
    //if(IsNoSourceByDeviceid(m_device))
    //        return ;
    // } deleted by edo.lee

    if ( m_nPendingMedia == -1 )
    {
        /* Workaround UISH issue: it looks to send the keypress multiple
         * times: ideally it should be aware of key repeat timeout, but
         * it does not. So place a hack here.
         *
         * Suppose one day, when the wheel events will come from the
         * hardware but not the keyboard, this will work better. */
        MP_LOG << "Ignored event.." << LOG_ENDL;

        if ( device < MP::ALL_DEVICES && !m_history[device].Title.isEmpty())
        {
        	
	            //{changed by junam 2013.03.26 for ISV72425
	            //emit tuneSearch( "", "", m_history[m_device].Title, "", "", "", "", "" , true );
                //EmitOnTuneSearch( "", "", m_history[device].Title, "", "", "" , true ); // modified by kihyung 2013.08.27 for ITS 0186708
	            //}changed by junam

                //{ changed by jaehwan 2013.10.30 for ITS 199032
                QString countInfo = QString::number(m_history[device].Index) + '/' + QString::number(m_history[device].Quantity);

                EmitOnTuneSearch( m_history[device].Cover, m_history[device].Artist,
                                 m_history[device].Title, m_history[device].Album,
                                 m_history[device].Folder, countInfo , true );
                //} changed by jaehwan

                GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
                                m_history[device].Title, m_randomPlayMode, m_repeatPlayMode, m_scanMode, false); // modified by cychoi 2013.11.12 for OSD Title Translation
        }
        return;
    }

    /* As for tuneWheel() function we should aware that playlist was
     * changed before the actual switch comes. In this case we should not
     * start anything.
     *  */

    MP_LOG << "m_nPendingMedia" << m_nPendingMedia << LOG_ENDL;
    m_enableErrorPopupState = ERROR_POPUP_ENABLE; //added by yungi 2012.11.01 for SANITY_CM_AK025
    setIsRunFromFileManager(false); // added by dongjin 2012.12.07 for SANITY_CM_AK025
    saveLastPlayedInfo(); // added by wspark 2013.01.23
    //emit clearTpMessage(); // commented by cychoi 2013.08.17 for No clear TP message from HMC request // added by wspark 2013.04.17

    //Added if condition for mute setting by Tan 2013.09.02
    if(device != MP::IPOD1 && device != MP::IPOD2)
    {
        //added by edo.lee 2013.05.08 for selecting song using tune
        GetEngineMain().setMuteForSeek(true);
    }

    //Added unmute flag by Tan 2013.09.02
    if(device == MP::IPOD1 || device == MP::IPOD2)
    {
        if(GetEngineMain().GetMuteState())
        {
            MP_LOG<< "IPOD setting m_WillUnMute true" <<LOG_ENDL;
            m_WillUnMute = true;
        }
        MP_LOG<< "IPOD sendVRCancelSignal()"<<LOG_ENDL ;
        GetEngineMain().sendVRCancelSignal();
    }


    GetEngineMain().setIsTermOnTrackChange(true);

    // { modified by wonseok.heo for ITS 186906 2013.08.30
    if(getRepeatMode() == REPEATFILE){
        if(device == MP::DISC && m_discType == MEDIA_SOURCE_MP3 ){
            setRepeatRandomMode(REPEATALL, RANDOMOFF);
        }else{
            onModeChanged(REPEATALL);//added by edo.lee 2013.05.25
        }
    }
    // } modified by wonseok.heo for ITS 186906 2013.08.30
    	
	//GetEngineMain().GetNotifier()->setHistoryTuneInfo();
	//GetEngineMain().GetNotifier()->onSetDisplayOSD(true);	
	//GetEngineMain().GetNotifier()->displayOSD(	OSD_EVENT_AUDIO_TRACK_INFO );
	if(device != MP::IPOD1 && device != MP::IPOD2)
    {
		GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
		m_tuneTitle, m_randomPlayMode, m_repeatPlayMode, m_scanMode, false);
	}

	m_tuneTitle= "";
	
	//added by edo.lee 2013.05.08 
	
    if ( device == MP::DISC )
    {
        //{ modified by yongkyun.lee 2013-07-12 for : NO CR - tune album info
        if((device == MP::DISC) && m_discType == MEDIA_SOURCE_MP3 )
        {
            m_history[MP::DISC].Cover  = "";
            m_history[MP::DISC].Artist = "";
            m_history[MP::DISC].Path   = "";
            m_history[MP::DISC].Album  = "";
        }
        //} modified by yongkyun.lee 2013-07-12 
        
        //{ added   by yongkyun.lee  2012.10.17  for Current file is white color
        if(m_nPendingMedia == m_history[device].Index)
            emit tuneTextColorChanged(false);            
        else
            emit tuneTextColorChanged(true);
        //} added   by yongkyun.lee  
        setCurrentIndex( m_nPendingMedia );
        tuneReset();
        //emit tuneTextColorChanged(true);//{ Removed by yongkyun.lee  2012.10.17  for Current file is white color
    }
    else
    {
        //{added by junam 2013.03.10 for ISV73458.
        m_tuneMetaInfoTimer.stop();
        //m_tuneCoverArtTimer.stop();//removed by junam 2013.10.17 for deprecated code
        //}added by junam
        m_tuneState.m_nTuneFF = false;
        m_tuneState.m_nTuneBack = false;
        m_tuneState.m_nTuneRequest = true;
        // { modified by junggil 2012.08.10 for CR12745
        //setAlbum( m_pPathViewModel->getAlbumName(m_tuneState.idx_tune));
        //if (m_device == MP::IPOD1 || m_device == MP::IPOD2)
        //{
        //  createPlayListFromList( m_pathsTune, m_nPendingMedia );
        //}		

        // { changed by junam 2012.09.12 for CR13632 
        setCurrentIndex( m_nPendingMedia );

        //m_history[m_device].Index = m_tuneState.idx_tune;

        //emit playListReadyAutoStart();
        m_tuneState.idx_tune = -1;
        // } changed by junam

        m_nPendingMedia = -1;
        MP_LOG << "switchToTuned.." << LOG_ENDL;
        m_tuneState.m_nTuneStart = false;
        //m_lastTuneSelectIndex = m_dynamicTemp.toInt();
        // } modified by junggil

        //clearPlayMode();// remove by yongkyun.lee@lge.com 2012.07.16 for CR 11464
    }
    //Deleted by yongkyun.lee 2012.10.11  for : Function_Jukebox_0635
    //clearPlayMode();// add by yongkyun.lee@lge.com 2012.07.16 for CR 11464

}

void
AppMediaPlayer_Controller_Audio::tuneReset()
{
    //{added by junam 2013.03.10 for ISV73458
    m_tuneMetaInfoTimer.stop();
    //m_tuneCoverArtTimer.stop();//removed by junam 2013.10.17 for deprecated code
    //}added by junam
    m_nPendingMedia = -1;
    m_tuneTrackNames.clear();
    m_tuneTrackIDs.clear();
    m_tuneState.m_nTuneStart = false;
    m_tuneState.m_nTuneFF = false;
    m_tuneState.m_nTuneBack = false;
    m_artistTune = "";
    m_titleTune = "";
    // { added by junggil 2012.07.31 for CR12069
    m_albumTune = "";
    m_folderTune = "";
    m_folderTuneTemp = "";
    m_genreTune = "";
    m_composerTune = "";
    m_coverartTune = "";
    // } added by junggil
}

void
AppMediaPlayer_Controller_Audio::setAppleDeviceStatus( int source, bool status )
{
    if (source == MP::IPOD1)
    {
        m_bIsApple1Ready = status;
    }
    else if (source == MP::IPOD2)
    {
        m_bIsApple2Ready = status;
    }
}

bool
AppMediaPlayer_Controller_Audio::getAppleDeviceStatus (int source)
{
    return (source == MP::IPOD1) ? m_bIsApple1Ready :
           (source == MP::IPOD2) ? m_bIsApple2Ready : false;
}

// } added by kihyung 2012.09.01 for Arabic string
QString AppMediaPlayer_Controller_Audio::DeckStringToUnicode(int nType, char *pString)
{
    if(nType == 1 || nType == 2) { // 1: UTF-16(BIG Endian), 2: UTF-16(LITTLE Endian)
        return QString(QString::fromUtf16((unsigned short*)pString));
    }
    else if(nType == 3) { // 3: UTF8
        return QString(QString::fromUtf8(pString));
    }
    else { // 0: NOT UNICODE 
        QTextCodec *codec = QTextCodec::codecForName("EUC-KR");
        QString     result;
        QTextStream in(pString);

        in.setAutoDetectUnicode(false);
        in.setCodec(codec);

        result = in.readAll ();

        return result;
    }
}
// } added by kihyung

QString
AppMediaPlayer_Controller_Audio::encodeString(QString inputString)
{
    // { added by ruinivbe 2012.08.30 CR 12924
    // There is no means to detect encoding properly this moment.
    return inputString;
    // } added by ruinivbe

    // { removed by ruinivbe 2012.08.30 CR 12924
    /*
    if ( isUtf8( inputString.toAscii() ) )
    {
        return inputString;
    }

    QTextCodec *codec = QTextCodec::codecForName("EUC-KR");
    QString result;

    //MP_LOG << "input String =" << inputString << LOG_ENDL; //remove by junam 08.22 for CR9771

    QTextStream in(inputString.toAscii());

    in.setAutoDetectUnicode(false);
    in.setCodec(codec);

    result = in.readAll ();

    //MP_LOG << "output String =" << result << LOG_ENDL;//remove by junam 08.22 for CR9771
    return result;
    */
    // } removed by ruinivbe
}

// { removed by ruinivbe 2012.08.30 CR 12924
/*
bool
AppMediaPlayer_Controller_Audio::isUtf8(const char * str )
{
    int str_bytes = 0;
    if( str == NULL )
        return false;

    str_bytes = strlen(str);
    bool isUtf8(true);
    int index = 0;

    while( index < str_bytes && isUtf8 )
    {
        char achar = str[index];
        int supplemental_bytes = 0;
        if( (achar & 0x80) == 0 )
        {
            // 0xxxxxxx
            ++index;
        }
        // Deleted by minho 20120727
        // { for CR7625 Multi language support in HMI
        //else if( (achar & 0xFE) == 0xFC )
        //{
            //++index;
            //supplemental_bytes = 5;
        //}
        //else if( (achar & 0xFC) == 0xF8 )
        //{
            //++index;
            //supplemental_bytes = 4;
        //}
        // } Deleted by minho
        // modified by minho 20120727
        // { for CR7625 Multi language support in HMI
        //else if( (achar &0xF8) == 0xF0 )
        else if(( (achar &0xF8) == 0xF0 ) || ( (achar & 0xFC) == 0xF8 ))
        // } modified by minho
        {
            // 11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
            ++index;
            supplemental_bytes = 3;
        }
        // modified by minho 20120727
        // { for CR7625 Multi language support in HMI
        //else if( (achar &0xE0) == 0xE0 )
        //else if( ((achar &0xF0) == 0xE0 ) || ((achar &0xA0) == 0xA0) )
        else if( (achar &0xF0) == 0xE0 ) 
        // } modified by minho
        {
            // 1110zzzz 10yyyyyy 10xxxxxx
            ++index;
            supplemental_bytes = 2;
        }
        else if( (achar &0xE0) == 0xC0 )
        {
            // 10yyyyyy 10xxxxxx
            ++index;
            supplemental_bytes = 1;
        }
        else
        {
            isUtf8 = false;
        }

        while( isUtf8 && supplemental_bytes-- )
        {
            if( index >= str_bytes )
            {
                isUtf8 = false;
            }
            // modified by minho 20120725
            // { for CR7625 Multi language support in HMI
            //else if( (str[index++] & 0xC0) != 0x80 )
            else if( (str[index] & 0xC0) == 0x80 )
            // } modified by minho
            {
                // 10uuzzzz
                isUtf8 = false;
            }
        }
    }

    return isUtf8;
}
*/
// } removed by ruinivbe

// { removed by eugeny.novikov 2012.08.15 for CR 12578
//void
//AppMediaPlayer_Controller_Audio::setAll()
//{
//    MP_LOG << LOG_ENDL;

//    if (m_device == MP::JUKEBOX ||
//        m_device == MP::USB1 || m_device == MP::USB2 ||
//        m_device == MP::IPOD1 || m_device == MP::IPOD2)
//    {
//        if (m_pTrackerAbstractor)
//        {
//            m_pTrackerAbstractor->RequestData(AudioUtils::VolumeType(m_device),
//                                              eGetAllSongsFromVolumeQuery_Id);
//        }
//    }
//}
// } removed by eugeny.novikov

void AppMediaPlayer_Controller_Audio::ResetAllHistory()
{
    for (int i = 0; i < MP::ALL_DEVICES; i++)
    {
        ResetDeviceInfo(i);
    }

    ResetExtendDeviceInfo();
}

void AppMediaPlayer_Controller_Audio::ResetDeviceInfo(int deviceId)
{
    MP_MEDIUM << DeviceName(deviceId) << LOG_ENDL;

    if (deviceId != MP::UNDEFINED)
    {
        m_history[deviceId].bShowInfo      = false; // added by junam 2012.9.10 for CR13244
        m_history[deviceId].DeviceID       = "";
        m_history[deviceId].MountPath      = "";
		m_history[deviceId].mediaType      = MEDIA_TYPE_MAX;
        m_history[deviceId].repeatPlayMode = REPEATALL;   // added by kihyung 2013.08.08 for ITS 0183321
	    m_history[deviceId].randomPlayMode = RANDOMOFF;  // added by kihyung 2013.08.08 for ITS 0183321
	    m_history[deviceId].UpdatedFSInfo = false;  // added by kihyung 2013.08.08 for ITS 0181967 
        ResetPlaylistInfo(deviceId);
    }
}

void AppMediaPlayer_Controller_Audio::ResetExtendDeviceInfo()
{
    if(m_usbDevicesHistory.isEmpty())
    {
        for(int i = 0; i < DEVICE_HISTORY_MAX_COUNT - 1; i++)
        {
            m_usbDevicesHistory.append(DeviceHistory());
        }
    }

    for(int i = 0; i < m_usbDevicesHistory.count(); i++)
    {
        if(m_usbDevicesHistory[i].playList.list)
        {
            m_usbDevicesHistory[i].playList.list->clear();
        }
        else
        {
            m_usbDevicesHistory[i].playList.list = new QList<QString>;
        }
        m_usbDevicesHistory[i].DeviceID = "";
    }
}

void AppMediaPlayer_Controller_Audio::ResetPlaylistInfo(int deviceId, bool bTrack) //add bTrack by junam 2013.08.17 for iPod empty data
{
    MP_MEDIUM << deviceId << LOG_ENDL;
    
    if (deviceId != MP::UNDEFINED)
    {
        m_history[deviceId].Index = 0;
        m_history[deviceId].Quantity = 0;

        if (deviceId == MP::IPOD1 || deviceId == MP::IPOD2) //added by junam 2013.11.07 for iPod
            m_history[deviceId].ContentPlayed = MP::DEFAULT;
        else
            m_history[deviceId].ContentPlayed = MP::FOLDER; // modified by kihyung 2013.11.06 for ITS 0206863

        m_history[deviceId].playList.PlaylistName = "";
        m_history[deviceId].playList.mode         = 0;
        m_history[deviceId].playList.category     = 0;
        m_history[deviceId].playList.index        = 0;
        m_history[deviceId].playList.position     = 0.0;
        // m_history[deviceId].playList.isMLT        = m_isPlayFromMLT; // added by jeeeun 2012.12.08 for MLT initializing // removed by kihyung 2013.07.18 for ITS 0180268

		//m_moveTrackIndex = 0;//added by edo.lee 2013.05.09

        if (m_history[deviceId].playList.list)
        {
            m_history[deviceId].playList.list->clear();
        }
        else
        {
            m_history[deviceId].playList.list = new QList<QString>;
        }
        if(bTrack) //add bTrack by junam 2013.08.17 for iPod empty data
        {
            ResetTrackInfo(deviceId);
        }
}
}

void AppMediaPlayer_Controller_Audio::ResetTrackInfo(int deviceId)
{
    MP_MEDIUM << deviceId << LOG_ENDL;
    
    if (deviceId != MP::UNDEFINED)
    {
        m_history[deviceId].Path = "";
        // m_history[deviceId].State = PLAYER_STOP; // removed by kihyung 2013.08.21 for SMOKE TEST 31.4
        m_history[deviceId].Title = "";
        m_history[deviceId].Artist = "";
        m_history[deviceId].Album = "";
        m_history[deviceId].Genre = "";
        m_history[deviceId].Cover = ""/*DEFALUT_COVER*/; //modified by Michael.Kim 2013.04.09 for New UX
        m_history[deviceId].Position = 0;
        m_history[deviceId].Duration = 0;
        m_history[deviceId].Composer = ""; // add by junggil 2012.06.29 for CR08958 : Composer info is not displayed on activating more info in Basic play back screen
    }
}

/**
 * To check processed data is concerned actual MP state
 */
bool AppMediaPlayer_Controller_Audio::isFilePathValid( QString filePath, MP::DEVICE_T device)
{
    // MP::DEVICE_T device = getDevice(); // removed by kihyung 2013.08.06 for ITS 0182926
    if(device >= MP::ALL_DEVICES) { 
        MP_HIGH << "device is UNDEFINED -" << filePath << device << LOG_ENDL;
        return false;
    }
    
    if(filePath.isEmpty()) {
        MP_HIGH << "filePath is empty -" << filePath << device << LOG_ENDL;
        return false;
    }

    if(device == MP::JUKEBOX && (filePath.startsWith("file:///usr_data/media") || filePath.startsWith("/usr_data/media"))) {
        return true;
    }
    else if((device == MP::USB1 || device == MP::USB2) && (filePath.startsWith("file:///media") || filePath.startsWith("/media"))) {
        return true;
    }
    else if((device == MP::USB1 || device == MP::USB2) && (filePath.startsWith("file:///rw_data/media") || filePath.startsWith("/rw_data/media"))) {
        return true;
    }
    else if((device == MP::IPOD1 || device == MP::IPOD2) && filePath.startsWith("file:///ipod")) {
        return true;
    }
    else {
        MP_HIGH << "filePath is incorrect -" << filePath << device << LOG_ENDL;
        return false;
    }
}

/**
*   GetSignalSource() - allows to identify a signal sender and determine destimation mode.
*
*   Note: MusicAppEngine calls some slots directly (desn't emit a signal), so sender is null.
*/
MP::DEVICE_T
AppMediaPlayer_Controller_Audio::GetSignalSource()
{
    QObject* source = this->sender();

    //NOTE: m_pUsbController is one for usb1 and usb2 cases.
    return source == m_pJukeboxController && m_pJukeboxController ? MP::JUKEBOX :
           source == m_pUsbController && m_pUsbController && (getDevice() == MP::USB1) ? MP::USB1 :
           source == m_pUsbController && m_pUsbController && (getDevice() == MP::USB2) ? MP::USB2 :
           source == m_pIpod1Controller && m_pIpod1Controller ? MP::IPOD1 :
           source == m_pIpod2Controller  && m_pIpod2Controller? MP::IPOD2 : MP::DISC;
}

void AppMediaPlayer_Controller_Audio::onPositionChanged(int position)
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;

    // { added by kihyung 2013.08.20 for ITS 0183782
    MP::DEVICE_T source = GetSignalSource();
    if(m_history[source].State == PLAYER_STOP && (source == MP::USB1 || source == MP::USB2 || source == MP::JUKEBOX)) 
    {
        MP_LOG << "position =" << position << "source =" << source << "state =" << m_history[source].State << LOG_ENDL;
        return;
    }
    // } added by kihyung 2013.08.20 for ITS 0183782
    
    // {added by wonseok.heo 2013.07.04 disc in out test
    if (GetEngineMain().getInOutTestMode() == true && device == MP::DISC){

        //m_pIMedia->Stop();

        if ( GetCurrentTrackNumber() == 1 && position <2000){
            MP_LOG<< " Count Status num = " << QString::number(m_InOutTestCnt) << LOG_ENDL;


            emit sendPlayStatus(1 , 1);
            m_InOutTestCnt++;
            emit sendPositon(m_InOutTestCnt);


        }else if (m_history[device].Quantity == GetCurrentTrackNumber() && position <2000){


            emit sendPlayStatus(2 , m_history[device].Quantity );
            MP_LOG<< " Count Status num = " << QString::number(m_InOutTestCnt) << LOG_ENDL;
        }

        if (m_InOutStepCnt == 0 &&  GetCurrentTrackNumber() == 1 && position >10000){

            // { modified by cychoi 2014.03.07 for ITS 228996 CDDA list flickering on item selection
            m_pListViewModel->onItemClick(m_history[device].Quantity -1);
            //m_pTreeModel->onItemClickCD(m_history[device].Quantity -1);
            // } modified by cychoi 2014.03.07

        }else if (m_history[device].Quantity == GetCurrentTrackNumber() && position >10000){

            m_InOutStepCnt ++;

            if(m_InOutTestCnt == 10000 ){
                emit sendPositon(m_InOutTestCnt);
                emit sendPlayStatus(4, 4);
                m_pIMedia->Stop();
                m_InOutTestCnt=1;
                m_InOutStepCnt=0;


            }else{

                emit sendPlayStatus(3, 3);
                m_InOutStepCnt = 0;
                MP_LOG<<" @@@@@@@@ EJECT @@@@ " << LOG_ENDL;
                //m_pIMedia->Stop();
                GetEngineMain().InOutTestEject();
                m_DiscLoadRequest->start();
            }
        }
        else
        {

            m_InOutStepCnt = 0;
            emit positionChanged(position);
        }
        return;

    }
    // }added by wonseok.heo 2013.07.04 disc in out test


    //{added by junam 2013.07.23 for ITS_EU_179614
    if(position < 0)
        return;
    //}added by junam

    //{added by edo.lee 2013.04.25
    // modified by edo.lee 2013.04.29 temp code for ipod mute
    if(device == MP::IPOD1 || device == MP::IPOD2)
    {
        //Added by wooju.lee 2013.09.26 for ITS_KOR_189034
       GetEngineMain().setIsTermOnTrackChange(false);

        //added by edo.lee 2013.05.21 ipod mute
        //Modified by Tan for IPod mute unmute 2013.09.02
        //if(GetEngineMain().GetMuteForSeek())// || GetEngineMain().GetMuteState())
        if(GetEngineMain().GetMuteState() && m_WillUnMute)
        {
            GetEngineMain().setKeepSystemMute(false);
            GetEngineMain().setUnMuteState(true);
            MP_LOG<< "IPOD setting m_WillUnMute false"  << LOG_ENDL;
            m_WillUnMute = false;
            //GetEngineMain().setMuteForSeek(true);
            //onStateChanged((PLAYER_STATE)m_pIMedia->GetCurrentMediaState() ); //removed by juanm 2013.08.30
        }

        // modified by edo.lee 2013.04.29
        if(m_iPodChangingPosition !=0 )
        {
            //{ changed by junam 2013.05.21 for ITS167351
            int jumpTime = position - m_iPodChangingPosition;
            m_iPodChangingPosition = 0;
            //{change by junam 2013.08.14 for progress bar moving back
            //if( jumpTime > 1000 || jumpTime < -1000 )
            if( jumpTime > 1000 || jumpTime < 0)
            {
                MP_LOG<<"ipod time jump" << LOG_ENDL;
                //m_iPodChangingPosition = 0;
                return;
            }
            //}changed by junam
        }
    }
    //}added by edo.lee 
    // Apply the data only when the restoring process is finished:
    //{ modified by yongkyun.lee 2013-07-26 for : ITS 181871
    if (m_bRestoring == false || device == MP::DISC)
        //} modified by yongkyun.lee 2013-07-26
    {
        // MP::DEVICE_T source = GetSignalSource(); // removed by kihyung 2013.08.20 for ITS 0183782
        m_history[source].Position = position;

        if (source == device)
        {
            emit positionChanged(position);

            if(GetEngineMain().getIsTermOnTrackChange() == false)
            {
                //MP_LOG<<"isLongPress "<<isLongPress() << LOG_ENDL; //remove too many log !!!
                //remove by edo.lee 2013.07.30 scan OSD
                /*if(m_scanMode != MP::SCANOFF)
    {
      GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
       m_history[device].Title, MP::RANDOMOFF, MP::REPEATOFF, m_scanMode, false);
    }else*/
                GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->updatePosition((OSD_DEVICE)getOSDevice(device), position , isLongPress() != 0 );
            }

            // { removed by junam 2013.12.10 for code clean
            //if (position > 1000)
            //{
                //setIsLoadingScreen(false);
            //    m_bFirstEntry[source] = false;//added by junam 2012.04.07 for ISV_KR77488
            //}
            // } removed by junam
        }
    }

    // { removed by junam 2013.12.10 for code clean
    //if (isLoadingScreen() == true && position > 1000) {
    //    setIsLoadingScreen(false);
    //}
    // } removed by junam

    // { modified by kihyung 2013.08.06 for ITS 0181069 
    // if (device == MP::DISC && m_bRestoring)
    if (device == MP::DISC || m_bRestoring)
    // } modified by kihyung 2013.08.06 for ITS 0181069         
    {
        //setPlayingPosition(m_history[MP::DISC].Position); // modified by yongkyun.lee 2013-07-26 for : ITS 181871
        m_bRestoring = false;
    }

    // { added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error 
    if(device == MP::DISC && m_discType == MEDIA_SOURCE_MP3) 
    {
        if (m_pIAudio && m_eID3TagState != ID3TAG_RECEIVED)
        {
            switch(m_PlayerState)
            {
            case PLAYER_PLAY:
            case PLAYER_FASTFORWARD:
            case PLAYER_REWIND:
            case PLAYER_SCAN: // modified by cychoi 2013.10.03 for SmokeTest Infinite ID3 Tag Req error
                // { modified by wonseok.heo for ITS 217651 2014.01.05
                if(!m_tuneState.m_nTuneStart){
                    m_pIAudio->RequestAllTagInfo();
                    m_eID3TagState = ID3TAG_REQUESTED;
                }// } modified by wonseok.heo for ITS 217651 2014.01.05
                break;
            default: // make compiler happy
                break;
            }
        }
    }
    // } added by cychoi 2013.09.22

    AppMediaPlayer_SettingsStorage::GetInstance()->SaveAudioDataForcedSpec(this); // added by kihyung 2013.07.30 for ITS 0181638
}

void AppMediaPlayer_Controller_Audio::onDurationChanged(int duration)
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES)
    {
        MP::DEVICE_T source = GetSignalSource();
        if(source == MP::IPOD1 || source == MP::IPOD2)
        {
            m_history[source].Duration = duration;
        }
        return;
    }
    
    MP_LOG << duration << LOG_ENDL;

    MP::DEVICE_T source = GetSignalSource();
    m_history[source].Duration = duration;

    if (source == device)
    {
        GetFilesCountInfo();
        emit durationChanged(duration);
        
        m_bRestoring = false; 	
    }

    if(duration > 0) {
        AppMediaPlayer_SettingsStorage::GetInstance()->SaveAudioSettings(this);
    }
}

//{ added by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
void AppMediaPlayer_Controller_Audio::updateMediaInfo()
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;

    if(device != MP::DISC) return;   // added by kihyung 2013.08.08 for ITS 0183321
    
    MediaInfo pInfo;
    if(GetCurrentMediaInfo(&pInfo))
    {
    	// { modified by kihyung 2013.08.08 for ITS 0183321
        m_history[device].Path = pInfo.fileName;
        EmitOnMediaInfoChanged(device);
        // } modified by kihyung 2013.08.08 for ITS 0183321
    }
    return;
}
//} added by yongkyun.lee 20130416 

void AppMediaPlayer_Controller_Audio::onMediaInfoChanged(MediaInfo* track)
{
    onMediaInfoChangedEx(track); // modified by oseong.kwon 2014.03.03 for ITS 227602
}

// { added by oseong.kwon 2014.03.03 for ITS 227602
void AppMediaPlayer_Controller_Audio::onMediaInfoChangedEx(MediaInfo* track, bool bEmitEnabled)
{
	// { modified by kihyung 2013.08.08 for ITS 0183321
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    // m_tMediaInfoMutex.lock();
    
    if (track != NULL && m_pIMedia != NULL)
    {
        MP::DEVICE_T source = GetSignalSource();
        
        MP_HIGH << source << track->Title << track->Album << track->Artist << track->coverarturl << LOG_ENDL;

        if(bEmitEnabled == true)
        {
            //added by sangmin.seol 2013.12.20 for SmokeTest fix tuneWheel problem after the song change.
            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->onTuneTimerStop();
        }

        if ((track->coverarturl == "No CoverArt Found")  // added by junam 2013.06.20 for ITS0174628
                 || (false == m_pPathViewModel->isCoverFileSizeValid(track->coverarturl))) //added by junam 2013.10.08 for empty coverflow
        {
            track->coverarturl = "";
        }

        //if (track->Album == "") {
        //    track->Album = ""; // modified by wonseok.heo for ITS 197433 2013.10.28
        //}
        //{ modified by yongkyun.lee 2013-08-23 for : ITS 186235
        //if (track->Artist == "") {
        //    track->Artist = ""; // modified by wonseok.heo for ITS 197433 2013.10.28
        //}
        //} modified by yongkyun.lee 2013-08-23 

        if (track->Title != "") {
            m_history[source].Title = (device == MP::DISC && getDiscType() == MP::DVD_AUDIO) ? "Track " + encodeString(track->Title) : 
                                                                                               encodeString(track->Title);
            GetEngineMain().GetNotifier()->SetCDDAMataInfo(true);// added by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
        }
        else {
            m_history[source].Title = "Unknown";
            m_bShowOSD = false;
            GetEngineMain().GetNotifier()->SetCDDAMataInfo(false);
        }

        // Added by jonghwan.cho@lge.com 2013.10.24.
        //QString strPrevSongPath = m_history[source].Path;
        //int     nPrevSongIndex  = m_history[source].Index;
        
        m_history[source].Album    = encodeString(track->Album);
        m_history[source].Artist   = encodeString(track->Artist);
        m_history[source].Genre    = encodeString(track->Genre);
        m_history[source].Path     = track->fileName.remove("file://");
        m_history[source].Cover    = track->coverarturl;
        m_history[source].Composer = encodeString(track->Composer);

        m_artistTune     = m_history[source].Artist;
        m_titleTune      = m_history[source].Title;
        m_albumTune      = m_history[source].Album;
        m_genreTune      = m_history[source].Genre;
        m_composerTune   = m_history[source].Composer;
        m_coverartTune   = m_history[source].Cover;
        m_folderTuneTemp = m_history[source].Path;
        
        if( device != MP::DISC) {
            m_folderTune = getFolderName( m_folderTuneTemp );
        }
        else { // added by yongkyun.lee 20130603 for : Folder name 
            if(m_pTreeModel && m_discType == MEDIA_SOURCE_MP3) {
                m_folderTune = m_pTreeModel->getCurrentFolderName();
            }
        }

        m_history[source].Folder = m_folderTune;

        if (source == device)
        {
            bool isOSD = true;

            // Added by jonghwan.cho@lge.com 2013.10.24.
            //QString strCurFilePath = track->fileName.remove("file://");

            //MP_LOG << "Cluster strCurFilePath = " << strCurFilePath << ", strPrevSongPath = " << strPrevSongPath << LOG_ENDL;
            //if(m_pIMedia != NULL)
            //{
            //    MP_LOG << "Cluster m_pIMedia->GetCurrentIndex() = " << m_pIMedia->GetCurrentIndex() << ", nPrevSongIndex = " << nPrevSongIndex << LOG_ENDL;
            //}
            m_albumName = track->Album;

            // { added by cychoi 2013.08.18 for HMC request - Cluster display = OSD display
            if(m_pIMedia != NULL)
            {
                if (m_pIMedia && m_history[device].ContentPlayed == MP::FOLDER)
                {
                    int fileIndex = -1;
                    int fileCount = -1;
                    m_pIMedia->GetCurrentFileFolderIndex(&fileIndex, &fileCount);
                    //{ modified by cychoi 2013.08.29 for HMC QC IPOD cluster display on Bootup
                    // { modified by yungi 2013.11.15 for Cluster AudioCD - Track NO displayed
                    if( device == MP::DISC && m_discType == MEDIA_SOURCE_CDDA && GetCDDAGraceNote() != GRACENOTE_INDEXING_DONE)
                    {
                        emit mediaInfoClusterChanged(QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_history[source].Title));
                    }
                    else
                    {
                        emit mediaInfoClusterChanged(m_history[source].Title);
                    }
                    // } modified by yungi
                    emit trackClusterChanged(fileIndex + 1, m_history[device].Quantity);
                    // { Added by jonghwan.cho@lge.com 2013.10.24.
                    //if( device == MP::IPOD1 || device == MP::IPOD2 )
                    //{
                    //    if( strCurFilePath.compare(strPrevSongPath) != 0 || nPrevSongIndex != m_pIMedia->GetCurrentIndex() )
                    //    {
                    //        emit trackClusterChanged(fileIndex + 1, m_history[device].Quantity);
                    //    }
                    //}
                    //else
                    //{
                    //    emit trackClusterChanged(fileIndex + 1, m_history[device].Quantity);
                    //}
                    // } Added by jonghwan.cho@lge.com 2013.10.24.
                    //} modified by cychoi 2013.08.29
                }
                else
                {
                    int trackIndex = GetCurrentTrackNumber();
                    //{ modified by cychoi 2013.08.29 for HMC QC IPOD cluster display on Bootup
                    // { added by wonseok.heo for ITS 197433 2013.10.28
                    if(device == MP::DISC && m_discType == MEDIA_SOURCE_CDDA && GetCDDAGraceNote() != GRACENOTE_INDEXING_DONE)
                    {
                        emit mediaInfoClusterChanged(QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_history[source].Title));
                    }
                    else
                    {
                        emit mediaInfoClusterChanged(m_history[source].Title);
                    }
                    // } added by wonseok.heo for ITS 197433 2013.10.28
                    emit trackClusterChanged(trackIndex, m_history[device].Quantity);
                    // { Added by jonghwan.cho@lge.com 2013.10.24.
                    //if( device == MP::IPOD1 || device == MP::IPOD2 )
                    //{
                    //    if( strCurFilePath.compare(strPrevSongPath) != 0 || nPrevSongIndex != m_pIMedia->GetCurrentIndex()  )
                    //    {
                    //        emit trackClusterChanged(trackIndex, m_history[device].Quantity);
                    //    }
                    //}
                    //else
                    //{
                    //    emit trackClusterChanged(trackIndex, m_history[device].Quantity);
                    //}
                    // } Added by jonghwan.cho@lge.com 2013.10.24.
                    //} modified by cychoi 2013.08.29
                }
            }
            // } added by cychoi 2013.08.18

            //Tan Modified for IPOD OSD blink [ISV][KOR][91170]
            if (m_scanMode != MP::SCANOFF && device != MP::IPOD1 && device != MP::IPOD2) 
            {
                // { modified by oseong.kwon 2014.02.04 for OSD update on TrackChange
                if(device == MP::DISC)
                {
                    if(m_bFirstDiscOSD == false)
                    {
                        //{ modified by yongkyun.lee 2013-11-04 for : NOCR SmokeTest - track OSD
                        if(m_discType == MEDIA_SOURCE_CDDA && GetCDDAGraceNote() != GRACENOTE_INDEXING_DONE)
                        {
                            QString tmpTitle = QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_pIMedia->GetCurrentIndex() + 1);
                            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->updateOSDMetaData((OSD_DEVICE)getOSDevice(device),
                                tmpTitle);
                        }
                        //} modified by yongkyun.lee 2013-11-04 
                        else
                        {
                            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->updateOSDMetaData((OSD_DEVICE)getOSDevice(device),
                                m_history[device].Title);
                        }
                    }
                    else
                    {
                        //{ modified by yongkyun.lee 2013-11-04 for : NOCR SmokeTest - track OSD
                        if(m_discType == MEDIA_SOURCE_CDDA && GetCDDAGraceNote() != GRACENOTE_INDEXING_DONE)
                        {
                            QString tmpTitle = QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_pIMedia->GetCurrentIndex() + 1);
                            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
                                tmpTitle , MP::RANDOMOFF, MP::REPEATOFF, m_scanMode, false);
                        }
                        else
                        //} modified by yongkyun.lee 2013-11-04 
                        {
                            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
                                m_history[device].Title, MP::RANDOMOFF, MP::REPEATOFF, m_scanMode, false);
                        }
                        m_bFirstDiscOSD = false;
                    }
                }
                else
                {
                    GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
                        m_history[device].Title, MP::RANDOMOFF, MP::REPEATOFF, m_scanMode, false);
                }
                // } modified by oseong.kwon 2014.02.04
            }
            else if(m_scanMode != MP::SCANOFF)//this is ipod only.
            {
                if(GetEngineMain().getReceivedTrackInfo() > 0)
                {
                    GetEngineMain().setReceivedTrackInfo(-1);
                    GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
                        m_history[device].Title, MP::RANDOMOFF, MP::REPEATOFF, m_scanMode, false);
                }
            }//end of Tan Modified for IPOD OSD blink 
            else if (m_isCreatNewList)
            {
                GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
                    m_history[source].Title, m_randomPlayMode, m_repeatPlayMode, m_scanMode, false);
                m_isCreatNewList = false;
                // { added by oseong.kwon 2014.02.04 for OSD update on TrackChange
                if(device == MP::DISC)
                {
                    m_bFirstDiscOSD = false;
                }
                // } added by oseong.kwon 2014.02.04
            }
            else if (m_isChangedDevice)
            {
                // { added by wonseok.heo for ITS 197433 2013.10.28
                if(device == MP::DISC && m_discType == MEDIA_SOURCE_CDDA && GetCDDAGraceNote() != GRACENOTE_INDEXING_DONE)
                {
                    GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
                                                                                                      QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_history[source].Title), m_randomPlayMode, m_repeatPlayMode, m_scanMode, false);
                }
                else
                // } added by wonseok.heo for ITS 197433 2013.10.28
                {
                    GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
                        m_history[source].Title, m_randomPlayMode, m_repeatPlayMode, m_scanMode, false);
                }
                m_isChangedDevice = false;
                // { added by oseong.kwon 2014.02.04 for OSD update on TrackChange
                if(device == MP::DISC)
                {
                    m_bFirstDiscOSD = false;
                }
                // } added by oseong.kwon 2014.02.04
            }
            else if (m_scanMode == MP::SCANOFF && GetEngineMain().getReceivedTrackInfo() > 0 && (device == MP::IPOD1 || device == MP::IPOD2))
            {
                GetEngineMain().setReceivedTrackInfo(-1);
                GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData(IPOD_MUSIC,
                    m_history[source].Title, m_randomPlayMode, m_repeatPlayMode, m_scanMode, false);
            } 
            else
            {
                // { added by wonseok.heo for ITS 197433 2013.10.28
                if(device == MP::DISC && m_discType == MEDIA_SOURCE_CDDA && GetCDDAGraceNote() != GRACENOTE_INDEXING_DONE)
                {
                    GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->updateOSDMetaData((OSD_DEVICE)getOSDevice(device),
                        QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_history[source].Title));
                }
                // } added by wonseok.heo for ITS 197433 2013.10.28
                else
                {
                    GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->updateOSDMetaData((OSD_DEVICE)getOSDevice(device), m_history[source].Title);
                }
            }
			
            if(bEmitEnabled == true)
            {
                if (device == MP::DISC && m_discType == MEDIA_SOURCE_CDDA)
                {
                    if (GetCDDAGraceNote() >= GRACENOTE_INDEXING_DONE)
                    {
                        EmitOnMediaInfoChanged(source);
                    }
                    else {
                        isOSD = false;
                    }
                    
                    if(GetCDDAGraceNote() == GRACENOTE_INDEXING_READY) {
                        m_CDDAGraceNote = GRACENOTE_INDEXING_NO_INFO; //added by Michael.Kim 2013.06.02 Exception Handling for Deaded Gracnote
                    }
                }
                else
                {
                    EmitOnMediaInfoChanged(source);
                }
                emit tuneTextColorChanged(false);// modified by yongkyun.lee 2013-11-21 for : EU - Tune 
            }

            if (m_bShowOSD && isOSD)
            {
                emit updateOSD( OSD_EVENT_AUDIO_TRACK_INFO );
                m_bShowOSD = isLongPress();
            }
            
            // Highlight the album:
            int albumIndex = m_pPathViewModel->getAlbumIndex(track->Album);
            if (albumIndex != -1)
            {
                emit setHighlightCover(albumIndex, true);
            }
            m_pPathViewModel->requestCoverEnable(false);

            // Sometimes iPod duration change signal emitted too early.
            // Need to update again if duration is zero.
            if( source == MP::IPOD1 || source == MP::IPOD2 )
            {
                if( m_history[source].Duration == 0 )
                {
       				m_history[source].Duration = m_pIMedia->GetCurrentMediaDuration();
                    MP_LOG<<"Duration update again = "<<m_history[source].Duration << LOG_ENDL;
                    if( m_history[source].Duration )
                    {
                        emit durationChanged(m_history[source].Duration);
                    }
                }
            }
	        SaveAudioData(); //added by edo.lee 2013.08.28
        }
    }
    else
    {
        MP_LOG << "Error. Invalid data." << LOG_ENDL;
    }

    // m_tMediaInfoMutex.unlock();
    // } modified by kihyung 2013.08.08 for ITS 0183321
}
// } added by cychoi 2014.03.03

// { commented by cychoi 2014.05.12 for deprecated code //{ added by hyochang.ryu 20130525
//void AppMediaPlayer_Controller_Audio::muteBeforePlay()
//{
//	if(m_pMuteTimer->isActive())
//	m_pMuteTimer->stop();
//	m_pMuteTimer->start();	
//}
// } commented by cychoi 2014.05.12 //} added by hyochang.ryu 20130525

//{ added by hyochang.ryu 20130807 for ITS179637
QString AppMediaPlayer_Controller_Audio::GetMarqueeString( const QString &fullString
                                                      /*, const QString &fontFamily,int pixelSize, int maxWidth*/ )
{
    QString str;
    if(fullString.length() > 57)
    {
        str = fullString.mid(0,57);
        str += "...";
    } 
    else 
    {
        str = fullString;
    }
    return str;
}
//} added by hyochang.ryu 20130807 for ITS179637

// { added by eunkoo 2012.07.26 for CR11898
void AppMediaPlayer_Controller_Audio::BTMusicInfoChanged(QString title, QString album, QString artist, QString count )
{
    MP::DEVICE_T device = getDevice();
    //{ deleted by yongkyun.lee 2013-10-18 for : NOCR BT song info
    MP_HIGH<<title<<album<<artist << LOG_ENDL;

    //{ deleted by hyochang.ryu 20131122 for ITS 210531
    /*
    title = GetMarqueeString(title);
    artist = GetMarqueeString(artist);
    album = GetMarqueeString(album);
    */
    //} deleted by hyochang.ryu 20131122 for ITS 210531
    
    DeviceHistory* infoHistory = GetDeviceHistory(MP::BLUETOOTH);
    infoHistory->Title = title;
    infoHistory->Artist = artist;
    infoHistory->Album = album;

    // { modified by cychoi 2014.09.12 for SmokeTest track info is not changed when user change track on Phone
    // { modified by cychoi 2014.09.02 for should send TP message to cluster only if changed song
    // { modified by oseong.kwon 2014.06.17 for ITS 240595, ITS 240597
    DISPLAY_T display = GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->getCurrentScreen();
    if(m_strUpdatedMediaInfo != title + album + artist + QString::number(display))
    {
        MP_LOG << " Title = " << title << LOG_ENDL;
        if(m_BTSetMediaMode || m_iTrackInfoReqCount)
        {
            showCurrentBTOSD();
        }
        emit btMusicInfoChanged( title, album, artist,count);
        m_strUpdatedMediaInfo = title + album + artist + QString::number(display);
    }
    else 
    {
        MP_LOG << " Title = " << title << " NOT UPDATED..." << LOG_ENDL;
    }
    // } modified by oseong.kwon 2014.06.17

    m_BTSetMediaMode = (GetBTAvrcpAbove13() && (title=="" || title=="Unknown")) ? true : false;
    // } modified by cychoi 2014.09.02
    // } modified by cychoi 2014.09.12
}
// } added by eunkoo

//{ added by hyochang.ryu 20130902 for ITS187310
void AppMediaPlayer_Controller_Audio::NotSupportBTOSD()
{
    OSDManager::instance()->displayOSD(OSD_EVENT_BT_UNSUPPORTED);
}
//} added by hyochang.ryu 20130902 for ITS187310

void AppMediaPlayer_Controller_Audio::onTrackChanged(int index)
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES)
    {
        //Tan. Play iPod music, go to pandora, play music from device,
        //defect: the track index is incorrect can show 35/13.
        MP::DEVICE_T source = GetSignalSource();
        if(source == MP::IPOD1 || source == MP::IPOD2)
        {
            m_history[source].Index = index;
        }
        return;
    }
    
    MP_HIGH << index << LOG_ENDL;

    MP::DEVICE_T source = GetSignalSource();

    //Tan, add conditional check so that we only stop the timer if active device is equal to signal source.
    //The timer should keep running so that it can restart the active device
    //1 sec after receiving audioSinkError.
    if(device == source)
    {
        // { modified by sangmin.seol ITS_0217688 If onTrackchanged for regular file After setskiptrack, do not skiptrack
        if(!m_skipTrack || IsFileSupported(index))
        {
            m_skipTrack = false;
            m_pAudioSinkTimer->stop(); // added by kihyung 2013.07.04 for ISV 85455
        }
        // } modified by sangmin.seol ITS_0217688 If onTrackchanged for regular file After setskiptrack, do not skiptrack
    }

    if ( m_pIMedia == NULL )
    {
        MP_LOG << "m_pIMedia is NULL " << LOG_ENDL;
        return;
    }
    
    if( GetListViewModel()->getPopType() == (int)LVEnums::POPUP_TYPE_DETAIL_FILE_INFO ) {
        GetListViewModel()->FileInfoPopUp();
    }        
	//remove by edo.lee 2013.06.13
	// if(GetEngineMain().getisBTCall() == true) 
	//{
	// 	Pause();
	//}
	 //added by edo.lee 2013.06.13

    m_history[source].Index = index;
    if(source != MP::IPOD1 && source != MP::IPOD2) //added by junam 2013.08.17 for iPod empty data
    {
        //{ modified by cychoi 2013.10.19 for "Track" display by Language Setting
        // { modified by cychoi 2014.02.05 for MP3 file name
        if(source == MP::DISC)
        {
            if(m_discType == MEDIA_SOURCE_CDDA)
            {
                m_history[source].Path = QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_pIMedia->GetCurrentIndex() + 1);
            }
            else //if(MEDIA_SOURCE_MP3)
            {
                m_history[source].Path = GetCurrentMP3FileName();
            }
        }
        // } modified by cychoi 2014.02.05
        else
        {
            m_history[source].Path = m_pIMedia->GetCurrentFileName();
        }
        //} modified by cychoi 2013.10.19
    }

    // Apply the data only when the restoring process is finished:
    if (source == device && m_bRestoring == false)
    {
        if( m_enableErrorPopupState == ERROR_POPUP_READY )
            m_enableErrorPopupState = ERROR_POPUP_ENABLE;
        else
            m_enableErrorPopupState = ERROR_POPUP_DISABLE;

		//remove by edo.lee 2013.03.19
        /*if (source == MP::DISC && getDiscType() == MP::AUDIO_CD)
        {
            DisplayCDInfo(index);
        }*/
    }
    
    if(source == device)
    {
        // { added by eugene.seo 2013.04.10
		if(index != lastPlayedIndex())
		{
			setLastPlayedIndex(-1);
		}
		// } added by eugene.seo 2013.04.10

        //{added by junam 2013.09.04 for ITS_KOR_188344
        if(source == MP::IPOD1 || source == MP::IPOD2)
            GetEngineMain().setIsTermOnTrackChange(false);
        //}added by junam
		
        // { modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error 
        //{ modified by yongkyun.lee 2013-07-05 for : NO CR - MP3 File name display
        if( ( device == MP::DISC && m_discType == MEDIA_SOURCE_MP3 ) )
        {
            // { modified by cychoi 2014.01.07 for keep MP3 ID3 Tag if same song 
            if(m_nID3TagTrackIdx != index ||
               (m_eID3TagState != ID3TAG_RECEIVED && m_nID3TagRxCount == 0))
            {
                m_eID3TagState = ID3TAG_NONE;
                m_nID3TagTrackIdx = index;
                m_nID3TagRxCount = 0; // added by cychoi 2013.12.17 for ITS 216033
                if( ( m_repeatPlayMode != REPEATFILE && m_scanMode == SCANOFF ) )
                {
                    DisplayCDInfo(index , true );
                }
            }
            // } modified by cychoi 2014.01.07
            // { modified by cychoi 2014.07.22 for should send TP Message to cluster although same song on ModeChanged
            else
            {
                emit mediaInfoClusterChanged(m_history[source].Title);
                emit trackClusterChanged(index + 1, m_history[source].Quantity);
            }
            // } modified by cychoi 2014.07.22
            updateCurrentMP3FolderName(); // added by cychoi 2013.10.07 for ISV 92215 Folder Display on Track Change
        }
        //} modified by yongkyun.lee 2013-07-05 
        // } modified by cychoi 2013.09.22
        
        GetFilesCountInfo();
        emit trackChanged(index, m_history[source].Quantity);
        //{ added by Michael.Kim 2013.08.13 for Hyundai Quality Assurance Issue
        if(device == MP::DISC)
        {
            // History duration & position is not accurate if DISC
            // So delay to update duration & position until receive it from Deck
            // { modified by cychoi 2013.10.08 for ITS 190742 CDDA duration
            m_history[device].Duration = m_pIMedia->GetCurrentMediaDuration();
            emit durationChanged(m_history[device].Duration);
            // } modified by cychoi 2013.10.08
            emit positionChanged(0);
        }
        //} added by Michael.Kim 2013.08.13 for Hyundai Quality Assurance Issue
    }

    // { removed by kihyung 2013.06.24 for ISV 79687
    /*
    if(m_isLongPressed != 0 && m_isPlayFromMLT == true) {
        emit disableHardKey(false);
    }
    */
    // } removed by kihyung 2013.06.24 for ISV 79687    
    //added by suilyou 20140312 ITS 0224783
    if(device >= MP::JUKEBOX && device <= MP::USB2)
    {
        if(m_autoTrackChangeIndex == -1)
            GetEngineMain().sendVRCancelSignal();
    }
}

bool
AppMediaPlayer_Controller_Audio::isForeground ()
{
    return m_isForeground;
}

void
AppMediaPlayer_Controller_Audio::setForeground (bool m_Foreground)
{
    m_isForeground = m_Foreground;
    emit isForegroundChanged(); // added by Dmitry 16.05.13

    // { added by kihyung 2012.12.04 for ISV 62683 
    if(m_Foreground == false) {
        emit showInhibitionIcon(false);
    }
    // } added by kihyung
}

void AppMediaPlayer_Controller_Audio::onGraceNoteTOCReceived(int nSize, void *pData)
{
    MP_LOG << LOG_ENDL;

    if (pData && nSize > 0)
    {
        QString strToc;
//        int     nBlocks  = nSize; // changed by eugeny - 12-09-15
        int    *pTocData = (int*)pData;

        for(int i = 0; i < nSize; i++)
        {
            strToc.append(QString::number(pTocData[i]));
            if(i < nSize - 1) {
                strToc.append(" ");
            }
        }

        m_history[MP::DISC].DeviceID = strToc;
        m_history[MP::DISC].Quantity = nSize - 1; // first block is not a song

        // CDDA Disc loading end.
        emit contentSize(AudioUtils::DeviceType(getDevice()),  MEDIA_TYPE_MUSIC,  m_history[MP::DISC].Quantity );
        emit tocReceived(strToc);
    }
    // } modified by kihyung
    
}

void AppMediaPlayer_Controller_Audio::onDeckTrackChanged(int index)
{
    MP_LOG << "index =" << index << LOG_ENDL;
    //added by wonseok.heo for NOCR List Focus of Disc 2013.12.30
    if(m_discType == MEDIA_SOURCE_MP3 && m_eID3TagState == ID3TAG_RECEIVED && m_moveTrackIndex == -1 )
    {
        m_pTreeModel->FolderCheck( "", -1, index + 1 );
    } //added by wonseok.heo for NOCR List Focus of Disc 2013.12.30

    onTrackChanged(index);//added by Eunhye 2013.01.24 for ISV 70720
    m_bFirstDiscOSD = (m_scanMode != SCANOFF); // added by oseong.kwon 2014.02.04 for OSD update on TrackChange

    // { modified by kihyung 2013.2.28
    /*
    if (m_discType == MEDIA_SOURCE_MP3 )
    {
        if (m_repeatPlayMode == REPEATFILE)
            return;
            
        m_deckInfo.Title       = QString(" ");
        m_deckInfo.Artist      = QString(" ");
        m_deckInfo.Album       = QString(" ");
        m_deckInfo.Genre       = QString();
        m_deckInfo.coverarturl = DEFALUT_COVER;
        m_folderTune           = " ";
        onMediaInfoChanged(&m_deckInfo);
    }
    else
    */
    // } modified by kihyung 2013.2.28
    if (m_discType != MEDIA_SOURCE_MP3 )
	{
        if (index < m_listCDContent.count() )
        {
            QString trackName = "";
            if(m_isPlayFromMLT && (m_discType ==  MEDIA_SOURCE_CDDA))
            {
                 int idx = m_mltFilePathesList.at(index).toInt();
                 trackName =  m_listCDContent.at(idx).trackTitle;
                 if (!trackName.isEmpty())
                 {
                     m_deckInfo.Album  = m_listCDContent.at(idx).albumTitle;
                     m_deckInfo.Artist = m_listCDContent.at(idx).artist;
                 
                     if(!m_listCDContent.at(idx).cover.isNull())
                         m_deckInfo.coverarturl = m_listCDContent.at(idx).cover;
                 
                     m_deckInfo.Title = trackName;
                     onMediaInfoChanged(&m_deckInfo);
                 }
            }
            else
            {
                if(!m_listCDContent.at(index).albumTitle.isEmpty())
                    trackName = m_listCDContent.at(index).trackTitle;
                else
                    trackName = GetCurrentFileName();

                if (!trackName.isEmpty())
                {
                    m_deckInfo.Album = m_listCDContent.at(index).albumTitle;
                    m_deckInfo.Artist = m_listCDContent.at(index).artist;

                    if(!m_listCDContent.at(index).cover.isNull())
                        m_deckInfo.coverarturl = m_listCDContent.at(index).cover;

                    m_deckInfo.Title = trackName;
                    onMediaInfoChanged(&m_deckInfo);
                }
            }
       }
       else {
           QString trackName(GetCurrentFileName());
           if (!trackName.isEmpty())
           {
               m_deckInfo.Title = trackName;
               onMediaInfoChanged(&m_deckInfo);
           }
       }
    }

    if (m_discType == MEDIA_SOURCE_DVDAUDIO )
    {
        GetDiscInfo();

        m_deckInfo.coverarturl = ""/*DEFALUT_COVER*/; //modified by Michael.Kim 2013.04.09 for New UX
        m_deckInfo.Artist = "Unknown";
        m_deckInfo.Title = (GetCurrentFileName() == "-1") ? "1" : GetCurrentFileName();
        m_deckInfo.Album = "Unknown";

        onMediaInfoChanged(&m_deckInfo);
    }
}

// { added by cychoi 2013.11.17 for ITS 208698 MP3 Play Info on Cluster 
void AppMediaPlayer_Controller_Audio::onDeckTrackUpdated(int index)
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;

    MP_LOG << "index =" << index << LOG_ENDL;

    if (m_pIMedia && m_history[device].ContentPlayed == MP::FOLDER)
    {
        int fileIndex = -1;
        int fileCount = -1;
        m_pIMedia->GetCurrentFileFolderIndex(&fileIndex, &fileCount);
        emit trackClusterChanged(fileIndex + 1, m_history[device].Quantity);
    }
    else
    {
        int trackIndex = GetCurrentTrackNumber();
        emit trackClusterChanged(trackIndex, m_history[device].Quantity);
    }
}
// } added by cychoi 2013.11.17

void AppMediaPlayer_Controller_Audio::onTagReceived(DvdcTagInfo *pTag)
{
    if(pTag == NULL) return;
    
    MP_LOG << "unicode_info: " << pTag->unicode_info << LOG_ENDL;

    char pTitle[32]  = {0, };
    char pAlbum[32]  = {0, };
    char pArtist[32] = {0, };
    
    // removed by cychoi 2013-10-03 for Unused code
    memcpy(pTitle, pTag->title, sizeof(char)*30);
    memcpy(pAlbum, pTag->album, sizeof(char)*30);
    memcpy(pArtist, pTag->artist, sizeof(char)*30);

    m_deckInfo.Title = AppMediaPlayer_Controller_Audio::DeckStringToUnicode(pTag->unicode_info, pTitle);
    if(m_deckInfo.Title.isEmpty()) {
        m_deckInfo.Title = GetCurrentFileName();
    }
    m_deckInfo.Artist = AppMediaPlayer_Controller_Audio::DeckStringToUnicode(pTag->unicode_info, pArtist);
    m_deckInfo.Album  = AppMediaPlayer_Controller_Audio::DeckStringToUnicode(pTag->unicode_info, pAlbum);
    
    if(pTag->genre < 0 || pTag->genre >= ID3_TAG_GENRE_MAX)
        m_deckInfo.Genre = "";
    else
        m_deckInfo.Genre = ID3_Tag_Genre[pTag->genre];

    QStringList reqAlbums, reqArtists, reqFiles;
    reqAlbums.append(m_deckInfo.Album);
    reqArtists.append(m_deckInfo.Artist);
    //{ modified by cychoi 2013.10.19 for "Track" display by Language Setting
    // { modified by cychoi 2014.02.05 for MP3 file name
    if(getDevice() == MP::DISC)
    {
        if(m_discType == MEDIA_SOURCE_CDDA)
        {
            reqFiles.append(QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_pIMedia->GetCurrentIndex() + 1));
        }
        else //if(MEDIA_SOURCE_MP3)
        {
            reqFiles.append(GetCurrentMP3FileName());
        }
    }
    // } modified by cychoi 2014.02.05
    else
    {
        reqFiles.append(m_pIMedia->GetCurrentFileName());
    }
    //} modified by cychoi 2013.10.19

    requestCurrentFolderName();

    onMediaInfoChanged(&m_deckInfo);
    m_pTrackerAbstractor->RequestData(reqAlbums, reqArtists, reqFiles, eRetrieveCoverArtOnDemand_Id, GET_TA_REQUID(REQUID_DEVICE_ID));
}

void AppMediaPlayer_Controller_Audio::onAllTagReceived(DvdcAllTagInfo *pTag)
{
    if(pTag == NULL || !pTag->valid) return; // modified by cychoi 2013.10.03 for SmokeTest Infinite ID3 Tag Req error // modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error

    MP::DEVICE_T device = getDevice();
    //if(device == MP::DISC && m_tuneState.m_nTuneStart) return; // commented by oseong.kwon 2014.03.03 for ITS 227602 //modified by wonseok.heo for ITS 217651 2014.01.05

    m_deckInfo.Title = pTag->title;
    // { modified by wspark 2013.04.12 for cluster IOT #38
    /*
    if(m_deckInfo.Title.isEmpty()) {
        m_deckInfo.Title = GetCurrentFileName();
    }
    */
    QString tmpTitle;
    tmpTitle = m_deckInfo.Title;
    tmpTitle.remove(QChar(' '));

    if(m_deckInfo.Title.isEmpty() || tmpTitle.isEmpty())
    {
        m_deckInfo.Title = GetCurrentFileName();
    }
    // } modified by wspark
    m_deckInfo.Artist      = pTag->artist;
    m_deckInfo.Album       = pTag->album;
    m_deckInfo.Genre       = pTag->genre;
    m_deckInfo.coverarturl = ""/*DEFALUT_COVER*/; //modified by Michael.Kim 2013.04.09 for New UX
    m_deckInfo.fileName    = GetCurrentFileName();
    m_folderTune           = " ";

    MP_HIGH << m_deckInfo.Artist << m_deckInfo.Album << m_deckInfo.Genre << LOG_ENDL;
    
    QStringList reqAlbums, reqArtists, reqFiles;
    reqAlbums.append(m_deckInfo.Album);
    reqArtists.append(m_deckInfo.Artist);
    //{ modified by cychoi 2013.10.19 for "Track" display by Language Setting
    // { modified by cychoi 2014.02.05 for MP3 file name
    if(device == MP::DISC)
    {
        if(m_discType == MEDIA_SOURCE_CDDA)
        {
            reqFiles.append(QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_pIMedia->GetCurrentIndex() + 1));
        }
        else //if(MEDIA_SOURCE_MP3)
        {
            reqFiles.append(GetCurrentMP3FileName());
        }
    }
    // } modified by cychoi 2014.02.05
    else
    {
        reqFiles.append(m_pIMedia->GetCurrentFileName());
    }
    //} modified by cychoi 2013.10.19

    // { added by oseong.kwon 2014.03.03 for ITS 227602
    if(device == MP::DISC && m_tuneState.m_nTuneStart)
    {
        onMediaInfoChangedEx(&m_deckInfo, false);
        return;
    }
    // } added by oseong.kwon 2014.03.03

    requestCurrentFolderName();

    onMediaInfoChanged(&m_deckInfo);

    emit allTagReceived();
    //emit updateOSD( OSD_EVENT_AUDIO_TRACK_INFO ); remove by edo.lee 2013.03.18

    // { modified by cychoi 2013.12.17 for ITS 216033
    if(pTag->exist == 1)
    {
        m_eID3TagState = ID3TAG_RECEIVED; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error 
        m_nID3TagRxCount = 0;
    }
    else
    {
        m_nID3TagRxCount++;
        if(m_nID3TagRxCount > 3)
        {
            m_eID3TagState = ID3TAG_RECEIVED; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error 
            m_nID3TagRxCount = 0;
        }
    }
    // } modified by cychoi 2013.12.17

    m_pTrackerAbstractor->RequestData(reqAlbums, reqArtists, reqFiles, eRetrieveCoverArtOnDemand_Id, GET_TA_REQUID(REQUID_DEVICE_ID));
}
//added by edo.lee 2013.04.21 for Mute
void AppMediaPlayer_Controller_Audio::onRealSoundChanged()
{
	MP_HIGH << LOG_ENDL;
	GetEngineMain().setIsTermOnTrackChange(false);//added by edo.lee 2013.09.26 ITS 0183932
    GetEngineMain().setUnMuteState(true);
}
void AppMediaPlayer_Controller_Audio::OnTrackBufferred(int bPrev, int nTotalIdx, int nIdxInFolder, int nFolderSize)//(int bPrev, int index)
{
	MP_HIGH<<nTotalIdx<<" " <<nIdxInFolder<< " "<<nFolderSize  << LOG_ENDL;

    //{added by junam 2013.09.04 for ITS_KOR_186481
    MP::DEVICE_T source   = GetSignalSource();
    if( source == MP::IPOD1 || source == MP::IPOD2 )
    {
        if(m_seekIndex == nIdxInFolder && m_nFolderSize == nFolderSize)
        {
            MP_LOG<<"same track bufferred signal" << LOG_ENDL;
            return;
        }
    }
    //}added by junam

	m_seekIndex = nIdxInFolder;
	m_nFolderSize = nFolderSize;
    //emit clearTpMessage(); // commented by cychoi 2013.08.17 for clear TP message on track change from HMC request
    RequestTrackInfo(bPrev, nTotalIdx);

    // { added by cychoi 2013.10.08 for ITS 190742 CDDA duration
    MP::DEVICE_T device = getDevice();
    if(device == source && device == MP::DISC)
    {
        if(m_pIMedia)
        {
            m_history[device].Duration = m_pIMedia->GetCurrentMediaDuration();
            emit durationChanged(m_history[device].Duration);
        }
    }
    // } added by cychoi 2013.10.08

    // { added for enhancing HU seek speed in listview by honggi.shin 2013.11.21
    if( source == MP::JUKEBOX || source == MP::USB1 || source == MP::USB2 )
        emit currPlayingItemPosChanged("", "", "");
    // } added by honggi.shin 2013.11.21 END
    // { added by wonseok.heo for NOCR List Focus of Disc 2013.12.30
    else if(source == MP::DISC && m_discType == MEDIA_SOURCE_CDDA)
    {
        if(nTotalIdx != -1)
        {
            m_history[source].Index = nTotalIdx;
            emit currPlayingItemPosChanged("", "", "");
        }
    } // } added by wonseok.heo for NOCR List Focus of Disc 2013.12.30
}

void AppMediaPlayer_Controller_Audio::onModeChanged(int nMode)
{
    MP_HIGH << "nMode:" << nMode << LOG_ENDL;

    bool         bChanged = false;
    MP::DEVICE_T device   = GetSignalSource();

    // Added by jonghwan.cho@lge.com 2013.11.15. for ITS_NA_0209523
    if( m_device != device )
    {
        //suryanto.tan save the shuffle state
        MP::DEVICE_T source = GetSignalSource();
        if(source == MP::IPOD1 || source == MP::IPOD2)
        {
            if(nMode == RANDOMOFF || nMode == RANDOMFILE || nMode == RANDOMFOLDER)
            {
                m_history[source].randomPlayMode = nMode;

            }
            else if (nMode == REPEATOFF || nMode == REPEATFILE || nMode == REPEATFOLDER || nMode == REPEATALL)
            {
                m_history[source].repeatPlayMode = nMode;
            }
        }
        return;
    }

    // {  added by wonseok.heo for Disc In out Test 2014.01.07
    if(GetEngineMain().getInOutTestMode() == true && getDevice() == MP::DISC )
        return;
    // }  added by wonseok.heo for Disc In out Test 2014.01.07

    switch (nMode)
    {
        case SCANOFF:
            //{ modified by shkim 2013.11.12 for CDDA Scan Off failure Smoke Test Issue
            //{ modified by shkim 2013.11.04 for CDDA Play/Pause Icon Smoke Test Issue
            if(m_device != MP::DISC || m_discType != MEDIA_SOURCE_CDDA)
            {
                emit stopScan();
            }
            //emit stopScan();
            //} modified by shkim 2013.11.04
            //} modified by shkim 2013.11.12

            GetEngineMain().GetNotifier()->setIsScanMode(false);
            // { modified by kihyung 2013.05.27 for ITS 148293
            /*
            setRepeatRandomMode(m_currentRepeatmode, RANDOMOFF);
            emit playingModeChanged(m_randomPlayMode, m_currentRepeatmode, m_scanMode, m_NoOSD);
            */
            // { added by kihyung 2013.09.17 for HMC new spec
            // { removed by kihyung 2013.07.17 for ITS 0180420 
            if(m_device != MP::DISC && ( m_device != MP::IPOD1 && m_device != MP::IPOD2 )) //added by Michael.Kim 2013.05.27 for ITS Issue #0146176
                setRepeatRandomMode(m_repeatPlayMode, RANDOMOFF);
            // } removed by kihyung 2013.07.17 for ITS 0180420             
            // } added by kihyung 2013.09.17 for HMC new spec            
            emit playingModeChanged(m_randomPlayMode, m_repeatPlayMode, m_scanMode, m_NoOSD);
            // } modified by kihyung 2013.05.27            
        case SCANFILE:
        case SCANFOLDER:
        case SCANALL:
            m_scanMode = nMode;
            break;

        case RANDOMOFF:
        case RANDOMFILE:
        case RANDOMFOLDER:
            m_randomPlayMode = nMode;
            if(m_history[device].randomPlayMode != m_randomPlayMode) {
                m_history[device].randomPlayMode = m_randomPlayMode;
                bChanged = true;
            }
            break;

        case REPEATOFF:
        case REPEATFILE:
        case REPEATFOLDER:
        case REPEATALL:
            m_repeatPlayMode = nMode;
            if(m_history[device].repeatPlayMode != m_repeatPlayMode) {
                m_history[device].repeatPlayMode = m_repeatPlayMode;
                bChanged = true;
            }
            break;

        default:
            MP_LOG << "Unhandled mode!" << LOG_ENDL;
            return;
    }

    updateRepeatRandomIcon(m_repeatPlayMode, m_randomPlayMode); // added by kihyung.lee 2013.08.31 for ITS 0187529
    
    emit playingModeChanged(m_randomPlayMode, m_repeatPlayMode, m_scanMode, m_NoOSD);

    //{ modified by yongkyun.lee 2013-09-17 for : ISV 89598
    if(( device == MP::DISC ) && bChanged && ( m_scanMode != SCANOFF ))// modified by yongkyun.lee 2013-09-22 for : Smoke test 
        m_scanMode = SCANOFF;
    //} modified by yongkyun.lee 2013-09-17 
    
    // modified by yongkyun.lee 2013-09-30 for : ISV 91169
    if(( device == MP::DISC ) && ( m_scanMode != SCANOFF ))
    {
        emit startScan(); // added by cychoi 2014.02.03 for play mode on SW Reset
        GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->updatePlayingMode((OSD_DEVICE)getOSDevice(device),
    	     RANDOMOFF, REPEATOFF, m_scanMode);
    }
    else if(( device == MP::IPOD1 || device == MP::IPOD2 ) && m_scanMode == SCANALL )  // added 2013.11.06 for ISV 91170
    {
        GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->updatePlayingMode((OSD_DEVICE)getOSDevice(device),
                 RANDOMOFF, REPEATOFF, m_scanMode);
    }
    else
    {
        GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->updatePlayingMode((OSD_DEVICE)getOSDevice(device),
                 m_randomPlayMode, m_repeatPlayMode, m_scanMode);
    }

    if(bChanged == true || device == MP::DISC ) 
    {
        AppMediaPlayer_SettingsStorage::GetInstance()->SaveAudioSettings(this);
    }
}

void AppMediaPlayer_Controller_Audio::parseReceivedDirTree( DvdcDirTree *dirTree )
{
    MP_LOG << LOG_ENDL;

    if ( !dirTree )
        return;

    if ( m_pTreeModel )
    {
        m_pTreeModel->appendChunkToModel( dirTree );
    }

    // { added by kihyung 2013.4.9 for ISV 77100
    if(m_bUpdateFileFolderIndex == false) 
    {
        int fileIndex = -1;
        int fileCount = -1;
        //{ added by yongkyun.lee 20130413 for : NO CR MP3 List 
        if(m_pIMedia == NULL)
            return;
        //} added by yongkyun.lee 20130413 
        m_pIMedia->GetCurrentFileFolderIndex(&fileIndex, &fileCount);
        MP_LOG << fileIndex << fileCount << LOG_ENDL;

        if(fileIndex != -1 && fileCount != -1) {
            m_bUpdateFileFolderIndex = true;
            emit updateMP3CDFileCount();
        }
    }
    // } added by kihyung 2013.4.9 for ISV 77100
}

// { modified by kihyung 2013.10.08 for ITS 0193747
void AppMediaPlayer_Controller_Audio::onMinerFinished(bool isFinished, QString usbMiningPath)
{
    MP_HIGH << usbMiningPath << isFinished << LOG_ENDL;

    // { modified by eugene.seo 2013.04.16
    if( true == isFinished )
    {
        if(false == onMinerInProgress( 200 )) {   // if do not check category available
            if(m_pListViewModel) {   // { added by sungha.choi 2013.08.12 for ITS 182674_3rd
                m_pListViewModel->onCategoryTabAvailable(isUSBMounted(MP::USB1), isUSBMounted(MP::USB2));
            }   // } added by sungha.choi 2013.08.12 for ITS 182674_3rd
        }

        m_minerState.minerInProgress = 0;
        m_minerState.isMinerFinished = true;

        if(isUSBMounted(MP::USB1)) {    // modified by sungha.choi 2013.08.12
            m_minerState.isUSB1MinerFinished = true;
        }else{
            m_minerState.isUSB1MinerFinished = false;
        }
        if(isUSBMounted(MP::USB2)) {    // modified by sungha.choi 2013.08.12
            m_minerState.isUSB2MinerFinished = true;
        }else{
            m_minerState.isUSB2MinerFinished = false;
        }
        // } modified by kihyung 2013.08.02 for ISV 88324

        if(m_jukeboxUpdate)
        {
            m_jukeboxUpdate = false;
            //{changed by junam 2013.10.31 for ITS_KOR_198284
            if(getDevice() == MP::JUKEBOX)
            {
                if(m_isListView == false && m_bIsBasicView == false )
                {
                    requestCovers();
                }
                //else
                //{
                //    m_pPathViewModel->firstEntrySetter(true); //removed by junam 2013.11.06 for coverflow update
                //}
            }
            //}changed by junam
        }
    }
    else if(usbMiningPath.contains("/rw_data/media/front_usb/") || usbMiningPath.contains("/media/front_usb/"))
    {
        //m_minerState.isUSB1MinerFinished = false; //removed by junam for 2013.11.05
        m_USB1MiningPath = usbMiningPath.replace("%20"," ");
        MP_MEDIUM << "isUSB1MinerFinished = " << m_minerState.isUSB1MinerFinished << LOG_ENDL;
        MP_MEDIUM << "m_USB1MiningPath = " << m_USB1MiningPath << LOG_ENDL;
    }
    else if(usbMiningPath.contains("/rw_data/media/rear_usb/") || usbMiningPath.contains("/media/rear_usb/"))
    {
        //m_minerState.isUSB2MinerFinished = false; //removed by junam for 2013.11.05
        m_USB2MiningPath = usbMiningPath.replace("%20"," ");
        MP_MEDIUM << "isUSB2MinerFinished = " << m_minerState.isUSB2MinerFinished << LOG_ENDL;
        MP_MEDIUM << "m_USB2MiningPath = " << m_USB2MiningPath << LOG_ENDL;
    } 
    else  
    {
        //m_minerState.isMinerFinished = false; //removed by junam for 2013.11.05
    }
    // } modified by eugene.seo 2013.04.16
  
    if(m_pListViewModel)
    {
        m_pListViewModel->getOperationCompleteforTA();

        //{added by kihyung 2013.10.08 for ITS 0193747 
        if( isFinished == false ) 
        {
            m_pListViewModel->setCopySpeed(SYSTEM_SLOW_COPY_SPEED);
        }
        else
        {
            m_pListViewModel->setCopySpeed(SYSTEM_NORMAL_COPY_SPEED);
        }
        //}added by kihyung 2013.10.08 for ITS 
    }

    //{removed by junam 2013.10.03 for ITS_KOR_192927
    //if(m_isListView == false && m_bIsBasicView == false && m_minerState.isMinerFinished == false && getDevice() == MP::JUKEBOX)
    //{
    //    m_pPathViewModel->firstEntrySetter(true);
    //    emit showPathView(true);
    //}
    //}removed by junam

    //{added by junam 2013.06.12 for hold coverloading at indexing
    if( m_pListViewModel && isListView() )
        m_pListViewModel->requestCoverTimerStart();
    //}added by junam
}

bool AppMediaPlayer_Controller_Audio::onMinerInProgress( int percentage )   // modified by sungha.choi 2013.08.12 for ITS 182674_3rd
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return false;
    MP_LOG << LOG_ENDL;

    if ( isMinerHandleRequired( percentage ) )
    {
        if(m_pListViewModel) {   // modified by sungha.choi 2013.08.12 for ITS 182674_3rd
            m_pListViewModel->onCategoryTabAvailable(isUSBMounted(MP::USB1), isUSBMounted(MP::USB2));
        }   // } modified by sungha.choi 2013.08.12 for ITS 182674_3rd

        //{changed by junam 2013.11.06 for coverflow update
        if(m_isListView)
        {
            if(m_pListViewModel)//Added by Taihyun.ahn 2013.10.05
            {
                //{removed by junam 2013.12.2 for list update
                //if((device == MP::USB1 && m_minerState.isUSB1MinerFinished) ||
                //        (device == MP::USB2 && m_minerState.isUSB2MinerFinished) ||
                //        (m_pListViewModel->getCategoryState() == FOLDER_DEPTH_0)   ||
                //        (m_pListViewModel->getCategoryState() == FOLDER_DEPTH_1))
                //{
                //    MP_LOG<<"USB update only once" << LOG_ENDL;
                //    return true;   // modified by sungha.choi 2013.08.12 for ITS 182674_3rd
                //}
                //}removed by junam

                if( m_pListViewModel->operation() == LVEnums::OPERATION_NONE)
                {
                    MP_LOG<<"Update list view" << LOG_ENDL;
                    m_pListViewModel->onDatabaseUpdate(percentage);
                }
            }
        }
        else if(m_bIsBasicView == false )
        {
            MP_LOG<<"Update coverflow view" << LOG_ENDL;
            requestCovers();
        }
    } //}changed by junam
    else    
    {
        return false;
    }
    return true;
}

void AppMediaPlayer_Controller_Audio::handleJukeboxDefault()
{
    MP_HIGH << LOG_ENDL;
    
    MP::DEVICE_T nDevice = MP::JUKEBOX;

    // History:
    if(m_history[nDevice].Quantity == 0 || IsPlaylistValid(nDevice) == false)
    {
        MP_HIGH << "m_history[nDevice].Quantity:" << m_history[nDevice].Quantity << LOG_ENDL; // removed by kihyung 2013.11.22 for ITS 0210669
        ResetDeviceInfo(nDevice);
    }
    m_history[nDevice].MountPath = "/usr_data/media/Music";

    AppMediaPlayer_EngineAudio *pAEngine = (AppMediaPlayer_EngineAudio*)GetEngineMain().GetEngine(APP_MEDIA_MODE_AUDIO); // added by kihyung 2013.07.20 for ITS 0179388

    // Before Indexing:
    if (m_history[nDevice].playList.list->empty())
    {
        RequestBeforeIndexingPlaylist(m_history[nDevice].MountPath);
    }
    // { removed by kihyung 2013.07.20 for ITS 0179388
    // If the below condition is true, it is logic error
    else if(pAEngine != NULL) 
    {
        if(getDevice() == MP::JUKEBOX) {
            if(pAEngine->getLastAVMode() == MODE_STATE_JUKEBOX_AUDIO) {
                MP_HIGH << "LOGIC ERROR CASE 1 !!!" << getDevice() << LOG_ENDL;
                RestoreFromHistoryAfterTAScan(eJukeBox, nDevice);
            }
            else {
                MP_HIGH << "LOGIC ERROR CASE 2 !!!" << getDevice() << LOG_ENDL;
                setDevice(MP::UNDEFINED);
            }
        }
    }
    // } removed by kihyung 2013.07.20 for ITS 0179388    
}

void AppMediaPlayer_Controller_Audio::handleUSBPluggedIn( QString path, bool isAutoPlay, USB_PORT_T usbPort, QString UUID )
{
    MP::DEVICE_T newUSBDevice     = (usbPort == eUSB_PORT1) ? MP::USB1 : MP::USB2;
    MP::DEVICE_T anotherUSBDevice = (newUSBDevice == MP::USB1) ? MP::USB2 : MP::USB1;

    MP_HIGH << "Path =" << path << ", Port =" << usbPort << ", ID =" << UUID << ", Auto = " << isAutoPlay << ", m_history[newUSBDevice].DeviceID " << m_history[newUSBDevice].DeviceID << LOG_ENDL;

    if(usbPort == eUSB_PORT1)
    {
        if(m_USB1MiningPath != path)
        {
            m_minerState.isUSB1MinerFinished = false;
            //{moved by junam 2013.11.22 for ITS_KOR_210689
            //if(m_pListViewModel)   // added by sungha.choi 2013.08.05 for ITS 182674
            //    m_pListViewModel->setCategoryTabAvailable(MP::USB1, false);   // added by sungha.choi 2013.08.05 for ITS 182674
            //}moved by junam
        }

        //{moved by junam 2013.11.22 for ITS_KOR_210689
        if(m_pListViewModel)
            m_pListViewModel->setCategoryTabAvailable(MP::USB1, false);
        //}moved by junam

        if(UUID != m_history[MP::USB1].DeviceID)
        {
            m_bNewUSB1Connected = true; //added by edo.lee 2013.06.10
        }
    }
    else if(usbPort == eUSB_PORT2)
    {
        if(m_USB2MiningPath != path)
        {
            m_minerState.isUSB2MinerFinished = false;

            //{moved by junam 2013.11.22 for ITS_KOR_210689
            //if(m_pListViewModel)   // added by sungha.choi 2013.08.05 for ITS 182674
            //    m_pListViewModel->setCategoryTabAvailable(MP::USB2, false);   // added by sungha.choi 2013.08.05 for ITS 182674
            //}moved by junam
        }

        //{moved by junam 2013.11.22 for ITS_KOR_210689
        if(m_pListViewModel)
            m_pListViewModel->setCategoryTabAvailable(MP::USB2, false);
        //}moved by junam
	
        if(UUID != m_history[MP::USB2].DeviceID)
        {
            m_bNewUSB2Connected = true; //added by edo.lee 2013.06.10
        }
    }
    else
    {
        MP_LOG << "usbPort is invalid!" << LOG_ENDL;
        return;
    }	

    //MP::DEVICE_T newUSBDevice     = (usbPort == eUSB_PORT1) ? MP::USB1 : MP::USB2;
    //MP::DEVICE_T anotherUSBDevice = (newUSBDevice == MP::USB1) ? MP::USB2 : MP::USB1;
    
    setDeviceId(newUSBDevice, UUID, path);
	
    bool bIsUSBHistoryAvailable = (m_history[newUSBDevice].DeviceID == UUID);


    MP_MEDIUM << "newUSBDevice " << newUSBDevice << "anotherUSBDevice " << anotherUSBDevice << LOG_ENDL;
    MP_MEDIUM << "m_history[newUSBDevice].DeviceID " << m_history[newUSBDevice].DeviceID << LOG_ENDL;
    MP_MEDIUM << "bIsUSBHistoryAvailable " << bIsUSBHistoryAvailable << LOG_ENDL;

    if(UUID.isEmpty() == false && m_history[newUSBDevice].DeviceID != UUID)
    {
        for(int idx = 0; idx < m_usbDevicesHistory.count(); idx++)
        {
            MP_HIGH << "usbDevicesHistory(" << idx << ") : " << m_usbDevicesHistory.at(idx).DeviceID << LOG_ENDL;
            
            if(m_usbDevicesHistory.at(idx).DeviceID == UUID)
            {
                MP_HIGH << "USB device" << UUID << "is in history at index" << idx << LOG_ENDL;
                bIsUSBHistoryAvailable = true;

                DeviceHistory tempHistory;
                tempHistory.playList.list = new QList<QString>;
                if(m_history[newUSBDevice].DeviceID.isEmpty() == false) {
                    CopyHistory(tempHistory, m_history[newUSBDevice]);
                }
                CopyHistoryFromUBSExtended(m_history[newUSBDevice], m_usbDevicesHistory.at(idx), path);
                
                MP_MEDIUM << "Pushing to history stack USB" << tempHistory.DeviceID << LOG_ENDL;

                m_usbDevicesHistory.move(idx, 0); // move found on list top
                if(tempHistory.DeviceID.isEmpty() == false) {
                    CopyHistoryToUSBExtended(m_usbDevicesHistory[0], tempHistory); // replace top with backed up m_history
                }
                
                tempHistory.playList.list->clear();
                delete tempHistory.playList.list;

                AppMediaPlayer_SettingsStorage::GetInstance()->SaveUSBAudioExtended(this); // modified by kihyung 2013.07.20 for ITS 0179774
                
                break;
            }
        }
    }	

    if(bIsUSBHistoryAvailable == true) {
        // { modified 2 lines below by honggi.shin 2013.12.03 for avoid empty folder
        MEDIA_SOURCE_T ANOTHER_USB_SRC = anotherUSBDevice == MP::USB1 ? MEDIA_SOURCE_USB1 : MEDIA_SOURCE_USB2;
        if(GetEngineMain().GetMediaManager()->GetID(ANOTHER_USB_SRC).isEmpty() && m_history[newUSBDevice].DeviceID == m_history[anotherUSBDevice].DeviceID)
        {
            MP_HIGH << "USB1 and USB2 has same device ID. remove anotherUSBDevice" << anotherUSBDevice << LOG_ENDL;
            ResetDeviceInfo(anotherUSBDevice);
        }
    }

    bool bResetHistory = false;
    if (!bIsUSBHistoryAvailable || !IsPlaylistValid(newUSBDevice))
    {
        MP_HIGH << "RESET CASE 1" << LOG_ENDL;
        bResetHistory = true;
        ResetDeviceInfo(newUSBDevice);
    }

    // removed by kihyung 2013.09.13
    
    if(bIsUSBHistoryAvailable == true) {
        // { modified by kihyung 2013.08.25 for ITS 0185730 
        if(m_history[newUSBDevice].ContentPlayed == MP::FOLDER || m_history[newUSBDevice].ContentPlayed == MP::DEFAULT) 
        {
            int nTotalAudioCount = GetEngineMain().GetTrackerAbs()->GetAudioFileNum(AudioUtils::VolumeType(newUSBDevice));
            if(m_history[newUSBDevice].playList.list->count() != nTotalAudioCount) {
                MP_HIGH << "RESET CASE 2" << m_history[newUSBDevice].playList.list->count() << GetEngineMain().getnAcount() << LOG_ENDL;
                bResetHistory = true;
                ResetDeviceInfo(newUSBDevice);
            }
        }
        // } modified by kihyung 2013.08.25 for ITS 0185730         
    }

    m_history[newUSBDevice].DeviceID = UUID;
    m_history[newUSBDevice].MountPath = path;

    AppMediaPlayer_EngineAudio *pAEngine = (AppMediaPlayer_EngineAudio*)GetEngineMain().GetEngine(APP_MEDIA_MODE_AUDIO); // added by kihyung 2013.07.20 for ITS 0179388

    // Before Indexing:
    if(bResetHistory == true)
    {
        RequestBeforeIndexingPlaylist(path);
    }
    // { modified by kihyung 2013.07.20 for ITS 0179388
    // If the below condition is true, it is logic error
    else if(pAEngine != NULL) {
        if(getDevice() == MP::USB1 && usbPort == eUSB_PORT1) {
            if(pAEngine->getLastAVMode() == MODE_STATE_USB1_AUDIO) {
                MP_HIGH << "LOGIC ERROR CASE 1 !!!" << getDevice() << LOG_ENDL;
                RestoreFromHistoryAfterTAScan(eUSB_FRONT, newUSBDevice);
            }
            else {
                MP_HIGH << "LOGIC ERROR CASE 2 !!!" << getDevice() << LOG_ENDL;
                setDevice(MP::UNDEFINED);
            }
        }
        else if(getDevice() == MP::USB2 && usbPort == eUSB_PORT2) {
            if(pAEngine->getLastAVMode() == MODE_STATE_USB2_AUDIO) {
                MP_HIGH << "LOGIC ERROR CASE 1 !!!" << getDevice() << LOG_ENDL;
                RestoreFromHistoryAfterTAScan(eUSB_REAR, newUSBDevice);
            }
            else {
                MP_HIGH << "LOGIC ERROR CASE 2 !!!" << getDevice() << LOG_ENDL;
                setDevice(MP::UNDEFINED);
            }
        }
    }
    // } modified by kihyung 2013.07.20 for ITS 0179388

    if(newUSBDevice == MP::USB1) {
        emit setStatusDataByIndex( newUSBDevice, DATA_TYPE_USB1_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);	
    }
    else {
        emit setStatusDataByIndex( newUSBDevice, DATA_TYPE_USB2_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);	
    }

    // emit activateTab(newUSBDevice, true, false); // removed by kihyung 2013.07.08 for ITS 0178696 
    //emit signalGracenoteDeviceStateChanged(newUSBDevice, UUID, path, 0, 0, 0, true); //deleted by aettie 20131010 unused code

    if(m_pListViewModel) {   // added by sungha.choi 2013.09.05 for tracker-miner crash/error
        m_pListViewModel->onCategoryTabAvailable(isUSBMounted(MP::USB1), isUSBMounted(MP::USB2));
    }   // added by sungha.choi 2013.09.05 for tracker-miner crash/error
}

void AppMediaPlayer_Controller_Audio::handleUSBPluggedOut(MP::DEVICE_T deviceToRemove)
{
    MP_HIGH << LOG_ENDL;
    if (deviceToRemove != MP::USB1 && deviceToRemove != MP::USB2)
    {
        MP_HIGH << "invalid device:" << deviceToRemove << LOG_ENDL;
        return;
    }    

    if (getDevice() == deviceToRemove)
    {
        Stop();
    }

    MP_HIGH << m_history[deviceToRemove].DeviceID << m_history[deviceToRemove].MountPath << LOG_ENDL;

    // { added by kihyung 2013 for ITS 0176932 
    if(m_history[deviceToRemove].DeviceID.isEmpty() == true && 
       m_history[deviceToRemove].MountPath.isEmpty() == true)
    {
        MP_HIGH << "return..." << LOG_ENDL;
        return;
    }
    // } added by kihyung 2013 for ITS 0176932     

    // { added by kihyung 2013.06.14
    m_history[deviceToRemove].repeatPlayMode = REPEATALL; 
    m_history[deviceToRemove].randomPlayMode = RANDOMOFF;
    // } added by kihyung 2013.06.14
    
    if(m_history[deviceToRemove].DeviceID.isEmpty() == false) 
    {
        bool bIsUSBHistoryAvailable = false;
        
        for(int idx = 0; idx < m_usbDevicesHistory.count(); idx++)
        {
            MP_LOG << "usbDevicesHistory(" << idx << ") : " << m_usbDevicesHistory.at(idx).DeviceID << LOG_ENDL;    // modified by sangmin.seol 2014.07.07 Reduce Log.
            
            if(m_usbDevicesHistory.at(idx).DeviceID == m_history[deviceToRemove].DeviceID)
            {
                MP_HIGH << "USB device" << m_history[deviceToRemove].DeviceID << "is in history at index" << idx << LOG_ENDL;
                
                m_usbDevicesHistory.move(idx, 0); // move found on list top
                
                CopyHistoryToUSBExtended(m_usbDevicesHistory[0], m_history[deviceToRemove]);
                
                AppMediaPlayer_SettingsStorage::GetInstance()->SaveUSBAudioExtended(this); // modified by kihyung 2013.07.20 for ITS 0179774

                bIsUSBHistoryAvailable = true;
                
                break;
            }
        }

        if(bIsUSBHistoryAvailable == false)
        {
            m_usbDevicesHistory.move(m_usbDevicesHistory.count() - 1, 0);
            CopyHistoryToUSBExtended(m_usbDevicesHistory[0], m_history[deviceToRemove]);

            AppMediaPlayer_SettingsStorage::GetInstance()->SaveUSBAudioExtended(this); // modified by kihyung 2013.07.20 for ITS 0179774
        }
    }
//deleted by aettie 20131010 unused code
    //emit signalGracenoteDeviceStateChanged(deviceToRemove, m_history[deviceToRemove].DeviceID, m_history[deviceToRemove].MountPath, 0, 0, 0, false);
    // emit activateTab( deviceToRemove, false, false );  // removed by kihyung 2013.07.08 for ITS 0178696 

    // { modified by eugene.seo 2013.04.16
    if(deviceToRemove == MP::USB1) {
        emit setStatusDataByIndex( deviceToRemove, DATA_TYPE_USB1_MEDIA_LOADING, MEDIA_DEVICE_UNAVAILABLE);
		m_minerState.isUSB1MinerFinished = false;		
                if(m_pListViewModel)   // added by sungha.choi 2013.08.05 for ITS 182674
                    m_pListViewModel->setCategoryTabAvailable(MP::USB1, false);   // added by sungha.choi 2013.08.05 for ITS 182674
		m_USB1MiningPath = "";	
        MP_MEDIUM << "isUSB1MinerFinished = " << m_minerState.isUSB1MinerFinished << LOG_ENDL;
        MP_MEDIUM << "m_USB1MiningPath = " << m_USB1MiningPath << LOG_ENDL;
    }
    else {
        emit setStatusDataByIndex( deviceToRemove, DATA_TYPE_USB2_MEDIA_LOADING, MEDIA_DEVICE_UNAVAILABLE);
		m_minerState.isUSB2MinerFinished = false;
                if(m_pListViewModel)   // added by sungha.choi 2013.08.05 for ITS 182674
                    m_pListViewModel->setCategoryTabAvailable(MP::USB2, false);   // added by sungha.choi 2013.08.05 for ITS 182674
		m_USB2MiningPath = "";	
        MP_MEDIUM << "isUSB2MinerFinished = " << m_minerState.isUSB2MinerFinished << LOG_ENDL;
        MP_MEDIUM << "m_USB2MiningPath = " << m_USB2MiningPath << LOG_ENDL;
    }
	// } modified by eugene.seo 2013.04.16
    resumeMiner(); // resume miner on usb remove
}

void AppMediaPlayer_Controller_Audio::handleIPODPluggedIn( QString path, bool isAutoPlay, USB_PORT_T usbPort, QString id )
{
    MP_HIGH << "TASCAN -> Path =" << path << "Port =" << usbPort << "ID =" << id << "Auto = " << isAutoPlay << LOG_ENDL;

    if (usbPort != eUSB_PORT1 && usbPort != eUSB_PORT2)
    {
        MP_MEDIUM << "usbPort is invalid!" << LOG_ENDL;
        return;
    }

    MP::DEVICE_T newIPODDevice = (usbPort == eUSB_PORT1) ? MP::IPOD1 : MP::IPOD2;

    if(newIPODDevice == MP::IPOD1)
    {
        m_iPod1ContentSizeReady = false;
    }
    if(newIPODDevice == MP::IPOD2)
    {
        m_iPod2ContentSizeReady = false;
    }

    // History:
    if (m_history[newIPODDevice].DeviceID != id)
    {
        //ResetDeviceInfo(newIPODDevice); //removed by junam 2013.04.13 for AutoStart
        m_history[newIPODDevice].DeviceID = id;
    }
    m_history[newIPODDevice].MountPath = path;

    //{added by junam 2013.08.06 for reset random repeat
    m_history[newIPODDevice].repeatPlayMode = REPEATALL;
    m_history[newIPODDevice].randomPlayMode = RANDOMOFF;
    //}added by junam

    // emit activateTab(newIPODDevice, true, false);  // removed by kihyung 2013.07.08 for ITS 0178696 
}

void AppMediaPlayer_Controller_Audio::handleIPODPluggedOut(MP::DEVICE_T deviceToRemove)
{
    MP_HIGH << DeviceName(deviceToRemove) << LOG_ENDL;

    //emit setStatusDataByIndex ( deviceToRemove, DATA_TYPE_DEVICE_STATUS, CONNECT_STATUS_NOT_CONNECTED );

    if (getDevice() == deviceToRemove)
    {
        Stop();
    }

    if(deviceToRemove == MP::IPOD1)
    {
        m_iPod1ContentSizeReady = false;
    }
    if(deviceToRemove == MP::IPOD2)
    {
        m_iPod2ContentSizeReady = false;
    }

    if (deviceToRemove == MP::IPOD1)
    {
        m_bIsApple1Ready = false;
        m_bIsDatabase1Ready = false;//added by junam 2013.05.02
        // { added by jungae 2012.12.26, for send the connection information to AppBtPhone
        MP_LOG << "Out Port=1, Name=" << m_iPod1DeviceName << "Status= " << m_bIsApple1Ready << LOG_ENDL;
        emit iPodDeviceConnectInfo(1, m_iPod1DeviceName, m_bIsApple1Ready);
        m_iPod1DeviceName = "Unknown";
        // } added by jungae
     }
    else
    {
        m_bIsApple2Ready = false;
        m_bIsDatabase2Ready = false;//added by junam 2013.05.02
        // { added by jungae 2012.12.26, for send the connection information to AppBtPhone
        MP_LOG << "Out Port=2, Name=" <<m_iPod2DeviceName << "Status= " << m_bIsApple2Ready << LOG_ENDL;
        emit iPodDeviceConnectInfo(2, m_iPod2DeviceName, m_bIsApple2Ready);
        m_iPod2DeviceName = "Unknown";
        // } added by jungae
    }

    m_bIsAppleConnectError = false; // added by kihyung 2012.07.09 for CR 10343

    QString mountPath = m_history[deviceToRemove].MountPath;
    QString iPodID = mountPath.mid(mountPath.lastIndexOf("/"));

    m_pTrackerAbstractor->RemoveDeviceId(iPodID);
    ResetDeviceInfo(deviceToRemove);

    // emit activateTab(deviceToRemove, false, false); // removed by kihyung 2013.07.08 for ITS 0178696 
}

void AppMediaPlayer_Controller_Audio::handleCDPluggedIn(MEDIA_SOURCE_T discType)
{
    MP_HIGH << LOG_ENDL;

    // { added by wonseok.heo 2013.07.04 disc in out test
    if( GetEngineMain().getInOutTestMode() == true && discType != MEDIA_SOURCE_CDDA){
        GetEngineMain().InOutTestEject();
        emit wrongDisc();
        return;
    }
    // } added by wonseok.heo 2013.07.04 disc in out test

    m_bUpdateFileFolderIndex = false;
    
    m_discType = discType;

    if (discType == MEDIA_SOURCE_MP3)
    {
        m_pDeckController = GetControllers().GetDeckController();
        connect(m_pDeckController, SIGNAL(OnDirTreeReceived(DvdcDirTree*)), this, SLOT(parseReceivedDirTree(DvdcDirTree*)));
        connect(m_pDeckController, SIGNAL(OnFolderNameReceived(DvdcFolderName *)), this, SLOT(receivedCurrentFolderName(DvdcFolderName*)));
    }

    // { commented by cychoi 2013.10.18 for ITS 194809
    //if( GetEngineMain().GetMediaManager()->isDiscReload() == false )//{ modified by yongkyun.lee 2013-10-08 for : ISV 92155
    //emit activateTab(MP::DISC, true, false);
    // } commented by cychoi 2013.10.18 for ITS 194809
    emit updateUIForDisc();

    if (discType == MEDIA_SOURCE_MP3)
    {
        sendFolderChunkRequestOnDec(0, 0);
    }

    if (m_bDiscPostponeRestore)
    {
        m_bDiscPostponeRestore = false;
        GetEngineMain().GetEngine(APP_MEDIA_MODE_AUDIO)->Restore(MODE_STATE_DISC_AUDIO);
    }

    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;

    emit setStatusDataByIndex(device ,DATA_TYPE_MAX_TRACK_NUM, m_history[device].Quantity );
    emit setStatusDataByIndex(device ,DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END );
}

//{ added by hyochang.ryu 20130802 for ISV88443
void AppMediaPlayer_Controller_Audio::handleAUXPluggedIn()
{
    MP_HIGH << LOG_ENDL;

    emit activateTab(MP::AUX, true, true);	//added by hyochang.ryu 20130821 for screen blink	//false, false);
}
//} added by hyochang.ryu 20130802 for ISV88443

void AppMediaPlayer_Controller_Audio::handleAUXPluggedOut(MP::DEVICE_T deviceToRemove)
{
    MP_HIGH << DeviceName(deviceToRemove) << LOG_ENDL;

    if (getDevice() == deviceToRemove)
    {
        Stop();
    }

    ResetDeviceInfo(deviceToRemove);
    //OSDManager::instance()->clearOSDPub(DISPLAY_FRONT_REAR);	//commented by yungi 2013.10.01 for SmokeTest AUX OSD //added by hyochang.ryu 20130821 for ITS183306/184161
    OSDManager::instance()->setAUXDisconnect(false);//{ modified by yongkyun.lee 2013-09-09 for : AUX video->audio OSD
    //emit activateTab(deviceToRemove, false, false); // commented by cychoi 2013.10.18 for ITS 194809
}

bool
AppMediaPlayer_Controller_Audio::isPlayerActive()
{
    return m_bPlayerActive;
}

void
AppMediaPlayer_Controller_Audio::setPlayerActive(bool m_bPlAct)
{
    m_bPlayerActive = m_bPlAct;
}

// { added by kihyung 2013.07.20 for ITS 0179388 
void AppMediaPlayer_Controller_Audio::setDevice(MP::DEVICE_T device)
{
    MP_LOG << device << LOG_ENDL;
    m_device = device;
}

MP::DEVICE_T AppMediaPlayer_Controller_Audio::getDevice() const
{
    return m_device;
}
// } added by kihyung 2013.07.20 for ITS 0178895

void AppMediaPlayer_Controller_Audio::sendFolderChunkRequestOnDec( int folder, int block )
{
    if (m_pDeckController)
    {
        IAudioInterface* pAudio = m_pDeckController->GetAudioInstance();

        if (pAudio)
        {
            int res = pAudio->RequestDirTree(0, folder, block);
            MP_LOG << res << LOG_ENDL;
        }
    }
}

CTrackerAbstractor*
AppMediaPlayer_Controller_Audio::getTrackerAbstractor()
{
    return m_pTrackerAbstractor;
}

// modified by ruindmby 2012.08.06 for CR12391
bool
AppMediaPlayer_Controller_Audio::isMultipleUSBDevice()
{
    bool bIsMultiple = (m_history[MP::USB1].MountPath != "" && m_history[MP::USB2].MountPath != "");
    MP_LOG << bIsMultiple << LOG_ENDL;
    return bIsMultiple;
}

// { added by kihyung 2012.08.09 for CR 12741
bool AppMediaPlayer_Controller_Audio::isMediaSyncfinished()
{
    MP_LOG << "m_device: " << getDevice() << LOG_ENDL;
    MP_LOG << "m_bIsApple1Ready: " << m_bIsApple1Ready << LOG_ENDL;
    MP_LOG << "m_bIsApple2Ready: " << m_bIsApple2Ready << LOG_ENDL;
    
    if(getDevice() == MP::IPOD1) 
        return m_bIsApple1Ready;
    else if(getDevice() == MP::IPOD2) 
        return m_bIsApple2Ready;
    else
        return false;
}
// } added by kihyung

//{added by junam 2013.05.02
bool AppMediaPlayer_Controller_Audio::isDatabaseReady()
{
    if(getDevice() == MP::IPOD1) {
        MP_HIGH << "m_device: " << getDevice() << m_bIsDatabase1Ready << LOG_ENDL;
        return m_bIsDatabase1Ready;
    }
    else if(getDevice() == MP::IPOD2) {
        MP_HIGH << "m_device: " << getDevice() << m_bIsDatabase2Ready << LOG_ENDL;
        return m_bIsDatabase2Ready;
    }
    else {
        MP_HIGH << "m_device: " << getDevice() << LOG_ENDL;
        return false;
    }
}
//}added by junam

//{ added by junam 2013.05.03 for start mode selection
bool AppMediaPlayer_Controller_Audio::isFlowViewEnable()
{
    return isDatabaseReady();
}
//}added by junam

// { added by kihyung 2013.3.11
bool AppMediaPlayer_Controller_Audio::isTAScanComplete(int nDevice)
{
    if(nDevice == MP::JUKEBOX) {
        return GetEngineMain().isTAScanComplete(eJukeBox);
    }
    else if(nDevice == MP::USB1) {
        return GetEngineMain().isTAScanComplete(eUSB_FRONT);
    }
    else if(nDevice == MP::USB2) {
        return GetEngineMain().isTAScanComplete(eUSB_REAR);
    }
    else {
        return true;
    }
}
// } added by kihyung 2013.3.11


bool
AppMediaPlayer_Controller_Audio::isMultipleiPodDevice()
{
   bool bIsMultiple = (m_history[MP::IPOD1].MountPath != "" && m_history[MP::IPOD2].MountPath != "");
   MP_LOG << bIsMultiple << LOG_ENDL;
   return bIsMultiple;
}
// modified by ruindmby 2012.08.06 for CR12391
void 
AppMediaPlayer_Controller_Audio::showCurrentBTOSD()
{
    if(m_bBTAvrcpAbove13 == false)
    {
        GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData(BLUETOOTH_MUSIC,
            "Phone Music", MP::RANDOMOFF,MP::REPEATOFF,MP::SCANOFF, false);
    }
    else
    {
        DeviceHistory* infoHistory = GetDeviceHistory(MP::BLUETOOTH);
        MP_LOG << infoHistory->Title << LOG_ENDL;
        GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData(BLUETOOTH_MUSIC,
            (infoHistory->Title=="" ? "Unknown": infoHistory->Title), MP::RANDOMOFF,MP::REPEATOFF,MP::SCANOFF, false);	//modified by hyochang.ryu 20130913 for ITS189478
        //GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->showCurrentOSD();
    }
}

// { added by cychoi 2014.03.26 for ITS 213525, 213526, 213527 CDDA OSD on language change
void 
AppMediaPlayer_Controller_Audio::showCurrentDiscOSD()
{
    MP::DEVICE_T device = getDevice();
    if(device != MP::DISC) return;

    if(device == MP::DISC && m_discType == MEDIA_SOURCE_CDDA && GetCDDAGraceNote() != GRACENOTE_INDEXING_DONE)
    {
        GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->updateCDDAOSDMetaData((OSD_DEVICE)getOSDevice(device),
                        QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_history[device].Title));
    }
    GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->showCurrentOSD();
}
// } added by cychoi 2014.03.26


//added by WJL 2014.04.15 for ITS 229118
void
AppMediaPlayer_Controller_Audio::showCurrentIPOD3rdAppOSD()
{
    MP::DEVICE_T device = getDevice();
    GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData(IPOD_MUSIC,
             m_history[device].Title, MP::RANDOMOFF,MP::REPEATOFF,MP::SCANOFF, false);
}

void
AppMediaPlayer_Controller_Audio::SetMediaMode(MP::DEVICE_T mode)
{
    MP_LOG << mode << LOG_ENDL;

    MP_LOG<<"GetEngineMain().getIsFromOtherAV() "<<GetEngineMain().getIsFromOtherAV() <<"m_isChangedDevice"<<m_isChangedDevice << LOG_ENDL;

    //deleted by hyochang.ryu 20131024 for NoCR Aux V->A
    
	//GetEngineMain().setIsTermOnTrackChange(false); //remove by edo.lee 2013.09.24 ITS 0190407 
    
    if (mode != MP::UNDEFINED)
    {       
        if(mode==MP::BLUETOOTH && mode != getDevice()) 
        {
            //{ modified by yongkyun.lee 2013-10-18 for : NOCR BT song info
            //showCurrentBTOSD();
            if(m_bBTAvrcpAbove13 == false)
            {
               GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData(BLUETOOTH_MUSIC,
                    "Phone Music", MP::RANDOMOFF,MP::REPEATOFF,MP::SCANOFF, false);
            }
            //} modified by yongkyun.lee 2013-10-18 
            if(!GetBTAvrcpAbove13()) 
                Play();

            m_BTSetMediaMode = true;//{ modified by yongkyun.lee 2013-10-18 for : NOCR BT song info
            m_strUpdatedMediaInfo = ""; // added by oseong.kwon 2014.06.17 for ITS 240595, ITS 240597
        }
        else if(mode==MP::AUX && mode != getDevice())
        {
            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->showCurrentOSD();
        }
        else if(mode != getDevice() || (GetEngineMain().getIsFromOtherAV() && !m_isChangedDevice)) // modified by kihyung 2013.09.13
        {
            if(mode != MP::IPOD1 && mode != MP::IPOD2)
            {
                DeviceHistory* infoHistory = GetDeviceHistory(mode);
                if(infoHistory && !m_isChangeToSame ) //modified by edo.lee 2013.05.27
                {
                    if(infoHistory->Title != "")
                    {					
                        //{ modified by yongkyun.lee 2013-11-14 for : ITS 209097
                        QString tmpTitle; 
                        if(mode == MP::DISC && m_discType == MEDIA_SOURCE_CDDA && GetCDDAGraceNote() != GRACENOTE_INDEXING_DONE )
                        {
                            tmpTitle = QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(infoHistory->Title);
                        }
                        else
                        {
                            tmpTitle = infoHistory->Title;                    
                        }
                        //} modified by yongkyun.lee 2013-11-14 
                        
                        // removed by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
                        //{changed by junam 2013.08.12 for ISV_KOR_88702
                        GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice((MP::DEVICE_T)mode),
                              tmpTitle, infoHistory->randomPlayMode, infoHistory->repeatPlayMode,
                              (getDevice() == MP::DISC || getDevice() == MP::IPOD1 || getDevice() == MP::IPOD2) ? SCANOFF : m_scanMode, false);//{changed by yongkyun.lee 2013.09.30 for ISV_KOR_88702 - disc add
                        //}changed by junam
                    }
                    else
                    {
                        m_isChangedDevice = true;
                    }
                }
            }
            else
            {
                m_isChangedDevice = true;
            }
        }
	
        if (m_isResourcesOwned)
        {
            emit activateTab(mode, true, true);
        }
        else
        {
            m_pendingSource = mode;
            GetControllers().RequestResources(MEDIAPLAYER_RESOURCE_AUDIO, this);
        }
        if(m_pListViewModel)
        {
            m_pListViewModel->SetFileOperationModeType(); // modified for ITS 0212431
        }
    }
}

void AppMediaPlayer_Controller_Audio::onResourceGranted()
{
    MP_LOG << LOG_ENDL;
    m_isResourcesOwned = true;
	//added by edo.lee 2013.04.20			
	DeviceHistory* infoHistory = GetDeviceHistory(m_pendingSource);
	if(infoHistory && !GetEngineMain().getOnBoot()) //modified by edo.lee 2013.05.27
	{
        // { modified by cychoi 2013.05.26 for Cluster IOT #125
        if(getDevice() == MP::DISC)
        {
            if(m_discType < MEDIA_SOURCE_MAX)
                setMediaInfoToHistory(m_discType,infoHistory->Title,infoHistory->Folder,infoHistory->Position,
                infoHistory->randomPlayMode , infoHistory->repeatPlayMode);//modified by edo.lee 2013.07.05
        }
        else
        {
            //{ added by yungi 2013.08.16 for ITS 0184297
            if(m_pendingSource != MP::AUX)
            {
                setMediaInfoToHistory(AudioUtils::DeviceType(m_pendingSource),infoHistory->Title,infoHistory->Folder,infoHistory->Position,
                    infoHistory->randomPlayMode ,infoHistory->repeatPlayMode);//modified by edo.lee 2013.07.05
            }
            //} added by yungi 2013.08.16 for ITS 0184297
        }
		//added by edo.lee 2013.05.09
		//setMediaInfoToHistory(AudioUtils::DeviceType(m_pendingSource),infoHistory->Title,infoHistory->Folder,infoHistory->Position);
        // } modified by cychoi 2013.05.26
		//GetEngineMain().GetNotifier()->setHistoryInfo(AudioUtils::DeviceType(m_pendingSource),infoHistory->Title,infoHistory->Folder,infoHistory->Position);
	    //GetEngineMain().GetNotifier()->onSetDisplayOSD(true);
		//GetEngineMain().GetNotifier()->displayOSD(	OSD_EVENT_AUDIO_TRACK_INFO );
	}
	//added by edo.lee 2013.04.20
    emit activateTab(m_pendingSource, true, true); // added by minho 20120821 for NEW UX: Added active tab on media
}

//{ modified by yongkyun.lee 2013-09-05 for : ITS 188359
void AppMediaPlayer_Controller_Audio::setDiscSource()
{
    setSource(MP::DISC);
}
//} modified by yongkyun.lee 2013-09-05 

void AppMediaPlayer_Controller_Audio::setSource(MP::DEVICE_T eMode)
{
    // { modified by kihyung 2013.7.25 for ITS 0181470 
    MP_HIGH << "START" << DeviceName(eMode) << LOG_ENDL;

    AppMediaPlayer_EngineAudio *pAEngine = (AppMediaPlayer_EngineAudio*)GetEngineMain().GetEngine(APP_MEDIA_MODE_AUDIO);    // added by sangmin.seol For ITS 0232871 maintain pause state when BT Call started in power off state

    MP::DEVICE_T nOldDevice = getDevice(); // added by jungae 2012.10.09, for CR14252, iPod reset when device name is same with BT

    if(m_pAudioSinkTimer) m_pAudioSinkTimer->stop(); // added by kihyung 2013.07.04 for ISV 85455

    if(m_pIMedia)
    {
        // { modified by kihyung 2013.08.24 for ITS 0186423. Maintain scan mode if old device and new device is same.
        if(nOldDevice != eMode || getScanMode() == SCANOFF) 
        {
            clearPlayMode();
        }
        // To update scan/random/repeat icon in different display(front or rear)
        else if(getScanMode() != SCANOFF)
        {
            MP_LOG << "start scan mode" << AppEngineBase::GetDisplayType() << LOG_ENDL;
            emit startScan();
        }
        // } modified by kihyung 2013.08.24 for ITS 0186423 
        
        // removed by kihyung 2013.07.26 for ITS 0181817 
        //{ added by junam 2013.05.03 for ISV_NA81503
        // clearPlayMode call stop scan but it's result cannot received sometime
        // because of mode switching. adding stop scan manually.
        // emit stopScan();
        //}added by junam
    }

    GetEngineMain().GetNotifier()->cancelFileOperation();
    GetEngineMain().GetNotifier()->onSetDisplayOSD(true);	//added by hyochang.ryu 20130817 for ITS184947		//added by edo.lee 2013.04.01
    if (nOldDevice != eMode)
    {
        if (nOldDevice == MP::IPOD1 || nOldDevice == MP::IPOD2 )
        {
            iPodDeviceOff();
            Pause();
        }
        else//{ deleted by yongkyun.lee 2013-08-30 for : BT sound mute
        {
            //{ modified by yongkyun.lee 2013-09-04 for : BT Mode key - pause
            if( (m_deviceValid == MP::BLUETOOTH) && (eMode != MP::BLUETOOTH ) && pAEngine->getModeStateAVP() )
            //} modified by yongkyun.lee 2013-09-04 
            {
                PauseBT();
            }
            else if(((eMode == MP::BLUETOOTH ) && (m_deviceValid == MP::IPOD1 || m_deviceValid == MP::IPOD2) ) )
            {
                Pause();
            }
            else
            {
                Stop();
            }
        }
    }
    // { added by cychoi 2014.05.28 for ITS 238650
    else
    {
        if(eMode == MP::BLUETOOTH)
        {
            sendToBTCommand(EVT_BT_MUSIC_GET_INFO);
            // should not call Play function if BT Audio is already playing
            MP_LOG << "Source is already set. Ignoring." << LOG_ENDL;
            return;
        }
    }
    // } added by cychoi 2014.05.28

    setDevice(eMode);

    m_imageMap.clear();//added by junam 2013.10.28 for 2nd depth coverart

    MP::DEVICE_T nNewDevice = getDevice();
    
    m_pIMedia           = NULL;
    m_pIMediaEx         = NULL;  // added by kihyung 2013.08.08 for ITS 0181967 
    m_PlayerState       = nNewDevice < MP::ALL_DEVICES ? m_history[nNewDevice].State : PLAYER_STOP;
    m_bRestoring        = false;
    m_albumName         = "";
    m_albumThumbnail    = "";

    //Modified by Tan, when playing 3rd party, go to aux, the 3rd party text should be hidden.
    //if(!m_bBasicControlEnableStatus && nNewDevice!=MP::AUX)	//added by hyochang.ryu 20130814 for ITS184463
    if(!m_bBasicControlEnableStatus)
    {
        //2014.10.21 Tan, ITS 0250594.
        if(nNewDevice != MP::AUX)
        {
            m_bBasicControlEnableStatus = true;
            emit basicControlEnableStatusChanged(true, "");
        }
    }
    
    emit setAppState(eMode);

    setGracenoteHighlightedVolume(nNewDevice);
    
    switch (nNewDevice)
    {
        case MP::JUKEBOX:
        {
            m_pJukeboxController =  GetControllers().GetJukeBoxController();
            m_pIMedia = m_pJukeboxController->GetMediaInstance();
            m_pIMediaEx = (IUSBExMediaInterface*)m_pJukeboxController;  // added by kihyung 2013.08.08 for ITS 0181967 
            m_pIMedia->ChangeMediaMode(MP_MODE_JUKEBOX);
            ConnectJukeboxSignal();
            emit deviceChanged(MEDIA_SOURCE_JUKEBOX);
            m_history[nNewDevice].mediaType = MEDIA_TYPE_MUSIC;
            break;
        }

        case MP::USB1:
        {
            m_pUsbController =  GetControllers().GetUSBController();
            m_pIMedia = m_pUsbController->GetMediaInstance();
            m_pIMediaEx = (IUSBExMediaInterface*)m_pUsbController;  // added by kihyung 2013.08.08 for ITS 0181967 
            m_pIMedia->ChangeMediaMode(MP_MODE_USB);
            ConnectUSBSignals();
            emit deviceChanged(MEDIA_SOURCE_USB1);
            m_history[nNewDevice].mediaType = MEDIA_TYPE_MUSIC;
            break;
        }

        case MP::USB2:
        {
            m_pUsbController =  GetControllers().GetUSBController();
            m_pIMedia = m_pUsbController->GetMediaInstance();
            m_pIMediaEx = (IUSBExMediaInterface*)m_pUsbController;  // added by kihyung 2013.08.08 for ITS 0181967 
            m_pIMedia->ChangeMediaMode(MP_MODE_USB);
            ConnectUSBSignals();
            emit deviceChanged(MEDIA_SOURCE_USB2);
            m_history[nNewDevice].mediaType = MEDIA_TYPE_MUSIC;
            break;
        }

        case MP::DISC:
        {
            m_pDeckController = GetControllers().GetDeckController();
            m_pIMedia = m_pDeckController->GetMediaInstance();
            m_pIAudio = m_pDeckController->GetAudioInstance();
            m_pIVideo = m_pDeckController->GetVideoInstance();
            m_pIMedia->ChangeMediaMode(MP_MODE_DISC);
            ConnectDeckSignals();
            GetDiscInfo();

            if ( m_discType != MEDIA_SOURCE_MAX )
            {
                emit deviceChanged (m_discType);
                if(nOldDevice != eMode)
                {
                    m_deckInfo.Artist       = QString();
                    m_deckInfo.Album        = QString();
                    m_deckInfo.Genre        = QString();
                    m_deckInfo.Composer     = QString();
                    m_deckInfo.coverarturl  = ""/*DEFALUT_COVER*/; //modified by Michael.Kim 2013.04.09 for New UX
                    if (m_discType == MEDIA_SOURCE_MP3 )
                    {
                        m_deckInfo.fileName = QString();
                        m_deckInfo.Title    = QString();
                    }
                    else
                    {
                        if(m_discType == MEDIA_SOURCE_DVDAUDIO) {
                            m_pVideoWidget =  m_pIMedia->GetVideoWidget(); 
                        }
                        //{ modified by cychoi 2013.10.19 for "Track" display by Language Setting
                        if(m_discType == MEDIA_SOURCE_CDDA)
                        {
                            m_deckInfo.fileName = QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_pIMedia->GetCurrentIndex() + 1);
                        }
                        else
                        {
                            m_deckInfo.fileName = m_pIMedia->GetCurrentFileName();
                        }
                        //} modified by cychoi 2013.10.19
                        m_deckInfo.Title = (GetCurrentFileName() == "-1") ? "1" : GetCurrentFileName(); // added by junggil 2012.08.17 for CR11646
                        // onMediaInfoChanged(&m_deckInfo);//{ deleted by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
                    }

                }
                else if (m_discType == MEDIA_SOURCE_MP3 ) {
                    m_deckInfo.Artist       = m_history[nNewDevice].Artist;
                    m_deckInfo.Album        = m_history[nNewDevice].Album;
                    m_deckInfo.Genre        = m_history[nNewDevice].Genre;
                    m_deckInfo.Composer     = m_history[nNewDevice].Composer;
                    m_deckInfo.coverarturl  = m_history[nNewDevice].Cover;
                    m_deckInfo.fileName     = m_history[nNewDevice].Path;
                    m_deckInfo.Title        = m_history[nNewDevice].Title;
                    onMediaInfoChanged(&m_deckInfo);//added by edo.lee 2013.03.18
                }
                //onMediaInfoChanged(&m_deckInfo); //{ deleted by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
            }
            m_history[nNewDevice].mediaType = MEDIA_TYPE_MUSIC;
            m_autoTrackChangeIndex = -1; // added by cychoi 2014.04.04 for ITS 233500
            break;
        }

        case MP::IPOD1:
        {
            m_pIpod1Controller =  GetControllers().GetIPod1Controller();
            if (m_pIpod1Controller)
            {
                m_pIpod1Controller->setTrackerAbsObject(m_pTrackerAbstractor);
                m_pTrackerAbstractor->setActiveIPod(eIPOD_FRONT);//Tan 2014.04.17
                m_pIMedia = m_pIpod1Controller;
                m_pIMedia->ChangeMediaMode(MP_MODE_iPOD);
                ConnectIpod1SignalInRuntime();
                emit deviceChanged(MEDIA_SOURCE_IPOD1);
                m_history[nNewDevice].mediaType = MEDIA_TYPE_MUSIC;
                pAEngine->setPlayAfterBTCalliPod(true); // ITS 240742_240746
            }
            m_autoTrackChangeIndex = -1;
            break;
        }

        case MP::IPOD2:
        {
            m_pIpod2Controller =  GetControllers().GetIPod2Controller();
            if (m_pIpod2Controller)
            {
                m_pIpod2Controller->setTrackerAbsObject(m_pTrackerAbstractor);
                m_pTrackerAbstractor->setActiveIPod(eIPOD_REAR);//Tan 2014.04.17
                m_pIMedia = m_pIpod2Controller;
                m_pIMedia->ChangeMediaMode(MP_MODE_iPOD);
                ConnectIpod2SignalInRuntime();
                emit deviceChanged(MEDIA_SOURCE_IPOD2);
                m_history[nNewDevice].mediaType = MEDIA_TYPE_MUSIC;
                pAEngine->setPlayAfterBTCalliPod(true); // ITS 240742_240746
            }
            m_autoTrackChangeIndex = -1;
            break;
        }

        case MP::BLUETOOTH:
        {
            //{ modified by hyochang.ryu 20130627
            //Play();	
            // { modified by cychoi 2014.05.12 for duplicated code (added by hyochang.ryu 20130627 for BT delayed play, 20131031 for ITS 205112)
            if( !pAEngine->getModeStateAVP() )
            {
                if(!pAEngine->isMaintainPause())    // added by sangmin.seol For ITS 0232871 maintain pause state when BT Call started in power off state
                {
                    m_pBtDelayedPlayTimer->start();
                }
            }
            else
            // } modified by cychoi 2014.05.12
            if(m_deviceValid == MP::IPOD1 || m_deviceValid == MP::IPOD2 )//{ modified by yongkyun.lee 2013-08-30 for : BT sound mute
            {
                m_pBtDelayedPlayTimer->start();
            }  
            else
            {
                // { modified by edo.lee 2013.07.30 bt call play state
                if (GetEngineMain().getKeepPlayState())
                {
                    if(IsPlaying())
                    {
                        Play();
                        GetEngineMain().resetKeepPlayState();//added by edo.lee 2013.08.01 bt call on pwroff
                    }
                    else
                        Pause();

                    //GetEngineMain().resetKeepPlayState();//remove by edo.lee 2013.08.01 bt call on pwroff
                }else
                    Play();
            }
            //{ modified by hyochang.ryu 20130627
            emit deviceChanged( MEDIA_SOURCE_BLUETOOTH );
            break;
        }
        
        case MP::AUX:
            emit deviceChanged( MEDIA_SOURCE_AUX1 );
            //{ modified by shkim for ITS 0208482 Aux OSD Issue 2013.11.12
            MP_LOG <<"[check aux disconnect] Aux Video->Aux Audio :  "<< OSDManager::instance()->getAUXDisconnect() << LOG_ENDL;
            if(!OSDManager::instance()->getAUXDisconnect())
            {
                OSDManager::instance()->checkAuxMode(); //{ added by shkim for ITS 205553 Aux OSD Issue
            }
            else if(OSDManager::instance()->getAUXDisconnect())
            {
                OSDManager::instance()->setAUXDisconnect(false);
            }
            //} modified by shkim for ITS 0208482 Aux OSD Issue 2013.11.12
            break;

        default:
            MP_LOG << "Player Mode is not supported" << LOG_ENDL;
            break;
    }

    // { modified by cychoi 2014.05.12 for duplicated code (added by hyochang.ryu 20130627 for BT delayed play, 20131031 for ITS 205112)
    if(nOldDevice == MP::BLUETOOTH && nNewDevice != MP::BLUETOOTH )//{ modified by yongkyun.lee 2013-11-12 for : ITS 208596
    {
        m_pBtDelayedPlayTimer->stop();
        //m_pBtDelayedPlayTimer1->stop();
    }
    // } modified by cychoi 2014.05.12
    
    if(nOldDevice == MP::BLUETOOTH && (eMode == MP::IPOD1 || eMode == MP::IPOD2) )
    {
        if(m_bSameDName)
        {
            MP_LOG << DeviceName(nOldDevice) << "->" << DeviceName(eMode) << "## iPod reset mode function call" << LOG_ENDL;
            StartiPodReset();
        }
    }

    if(nNewDevice < MP::ALL_DEVICES) {
        //{ added by yongkyun.lee 20130619 for : 
        setRepeatRandomMode( m_history[nNewDevice].repeatPlayMode, m_history[nNewDevice].randomPlayMode);
        m_history[nNewDevice].UpdatedFSInfo = false;  // added by kihyung 2013.08.08 for ITS 0181967 
        m_deviceValid= nNewDevice;// modified by yongkyun.lee 2013-08-30 for : BT sound mute
    }

    RestoreFromHistory(nNewDevice);

    // added by sangmin.seol For ITS 0232871 maintain pause state when BT Call started in power off state
    pAEngine->setMaintainPause(false);

    MP_HIGH << "END" << LOG_ENDL;
}

void AppMediaPlayer_Controller_Audio::onReleaseResources( MEDIAPLAYER_RESOURCES_T resources )
{
    if (resources & MEDIAPLAYER_RESOURCE_AUDIO)
    {
        MP_HIGH << "START" << LOG_ENDL;
        
        m_isResourcesOwned = false;
        setPlayerActive(false);

        // { added by junam 2012.10.10 for clear seek mode
        if(m_isLongPressed)
        {
            setIsLongPress(0); // added by kihyung 2013.2.5 for ISV 72327
            emit clearSeekMode();
        }
        // } added by junam

        // { removed by kihyung 2012.11.03 for booting disable
        /*
        if (IsPlaying())
        {
            resetAudioMode();
        }
        */
        // } removed by kihyung 2012.11.03 for booting disable
        
        // { added by junam 2012.9.26 for CR12669
        if ( getDevice() == MP::IPOD1 || getDevice() == MP::IPOD2 )
        {
            iPodDeviceOff();// moved before Pause() by junam 2012.12.13
            if (IsPlaying())
                Pause();
            //iPodDeviceOff();// moved before Pause() by junam 2012.12.13
        }
        // { added by lssanh 2013.04.06 ISV69184 BT paused by changesource
        else if (getDevice() == MP::BLUETOOTH)
        {
            if (IsPlaying())
                Pause();

            // added by kihyung 2013.5.2
            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->hideBTMusic(); // added by kihyung 2013.5.2
        }
        // } added by lssanh 2013.04.06 ISV69184 BT paused by changesource
        else // } added by junam
        {
            Stop();
        }
        setDevice(MP::UNDEFINED);
        m_pIMedia = NULL;

        GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setLastAVMode(MODE_STATE_NONE); // added by ruindmby 2012.09.11 for CR#13584
        disconnectControllers();
        GetControllers().onResourceReleased( MEDIAPLAYER_RESOURCE_AUDIO );

        MP_LOG << "END" << LOG_ENDL;    // modified by sangmin.seol 2014.07.07 Reduce Log.
    }
}

void AppMediaPlayer_Controller_Audio::InitiateJukeBoxFormat(void)
{
   MP_LOG << "TASCAN" << LOG_ENDL;

   ResetDeviceInfo(MP::JUKEBOX);

   if (m_pIMedia && getDevice() == MP::JUKEBOX)
   {
       m_pIMedia->SetPlayList(&m_history[MP::JUKEBOX].playList);

       m_pPathViewModel->clearModelData();
       EmitOnMediaInfoChanged(MP::JUKEBOX); // modified by kihyung 2013.08.08 for ITS 0183321
   }

   // modified by sangmin.seol 2013.11.14 for fix freeze in copy popup showing state when formated jukebox
   //m_pTrackerAbstractor->RequestData(eJukeBox, eRemoveAllMediaDataFromVolumeQuery_Id);
   QTimer::singleShot(100, this, SLOT(RequestTrackerJukeboxData()));
   // modified by sangmin.seol 2013.11.14 for fix freeze in copy popup showing state when formated jukebox

   GetEngineMain().stopTrackerAbsScan(true); // modified by kihyung 2013.07.25 for ITS 0180896 
}

// { added by sangmin.seol 2013.11.14 for fix freeze in copy popup showing state when formated jukebox
void AppMediaPlayer_Controller_Audio::RequestTrackerJukeboxData()
{
    if (m_pTrackerExtractor)
    {
        MP_HIGH << "RequestTrackerJukeboxData " << LOG_ENDL;

        m_pTrackerAbstractor->RequestData(eJukeBox, eRemoveAllMediaDataFromVolumeQuery_Id);;
    }
}
// } added by sangmin.seol 2013.11.14 for fix freeze in copy popup showing state when formated jukebox


void AppMediaPlayer_Controller_Audio::resumeMiner()
{
    if (m_pTrackerExtractor)
    {
        m_pTrackerExtractor->resumeMiner();
    }
}

void AppMediaPlayer_Controller_Audio::pauseMiner()
{
    if (m_pTrackerExtractor)
    {
        m_pTrackerExtractor->pauseMiner();
    }
}

// { added by eugene 2012.11.14 for jukebox miner status
bool AppMediaPlayer_Controller_Audio::isJukeboxMiningFinished()
{
	bool is_finished = false;
    if (m_pTrackerExtractor)
    {	   
        is_finished = m_pTrackerExtractor->getJukeboxStatus();     
    }
	return is_finished;
}
// } added by eugene 2012.11.14 for jukebox miner status

void AppMediaPlayer_Controller_Audio::setDeviceId(MP::DEVICE_T source, QString id, QString mountPath)
{
    if (source == MP::USB1 || source == MP::USB2 ||
        source == MP::IPOD1 || source == MP::IPOD2)
    {

        CTrackerAbstractor* listTA = NULL;//Added by Taihyun.ahn 2013.10.05
        if(m_pListViewModel != NULL)
        {
            listTA = m_pListViewModel->getTrackerAbstactor();
        }

        ETrackerAbstarctor_VolumeType volume = AudioUtils::VolumeType(source);

        m_pTrackerAbstractor->setDeviceId(volume, id, mountPath);

        if (listTA)
        {
            listTA->setDeviceId(volume, id, mountPath);
        }
    }
}

void AppMediaPlayer_Controller_Audio::selectTitleChapter(int title, int chapter)
{
	// modified by ravikanth - 12-09-26
    if(m_pIVideo)
    {
        m_pIVideo->SearchTitleChapter( title, chapter );
    }
}

void AppMediaPlayer_Controller_Audio::GetDiscInfo()
{
    MP_LOG << LOG_ENDL;
    
    int status = -1;

    // { removed by kihyung 2013.4.6
    // if(!GetEngineMain().getOnBoot()) //added by yungi 2013.04.02 for ISV 77744
    //    GetEngineMain().GetNotifier()->onSetDisplayOSD(false);//added by edo.lee 2013.03.25
    // } removed by kihyung 2013.4.6
    
    switch (m_discType)
    {
        case MEDIA_SOURCE_CDDA:
        {
            //{ Modified by Radhakrushna Mohanty CR#12784 dated 2012.08.23
            if (m_listCDContent.isEmpty() && /*m_history[MP::DISC].DeviceID.isEmpty() &&*/ m_pIAudio)
            //} Modified by Radhakrushna Mohanty CR#12784 dated 2012.08.23
            {
                status = m_pIAudio->RequestGraceNoteTOC();
            }
            break;
        }
        case MEDIA_SOURCE_MP3:
        {
            TotalFileFolderCount count = {0, 0, 0};
            status = m_pIAudio->GetTotalFileFolder(&count);

            m_history[MP::DISC].Quantity = count.totalFiles;
            break;
        }
        case MEDIA_SOURCE_DVDAUDIO:
        {
            if (m_pIVideo)
            {
                DVDStatusResponse DVDInfo; // changed by eugeny - 12-09-15
                status = m_pIVideo->GetDVDStatus(&DVDInfo);

                // { modifed by kihyung 2012.08.30 for DVD-Audio
                // if(DVDInfo.trackOrChapterNo > -1) // added by junggil 2012.08.17 for CR11646
                
                if(DVDInfo.trackOrChapterNo >= 0 && DVDInfo.trackOrChapterNo <= DVDInfo.totalChaptorOrTrack)
                {       
                	//GetEngineMain().GetNotifier()->onSetDisplayOSD(true);//added by edo.lee 2013.03.25
                    //m_strTrackNumber = QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(DVDInfo.trackOrChapterNo);
                    m_strTrackNumber = QString(QApplication::translate("main", "STR_MEDIA_CHAPTER") + " %1").arg(DVDInfo.trackOrChapterNo); //  modified by aettie.ji 2012.11.5 for New UX
                    m_strGroupNumber = QString(QApplication::translate("main", "STR_MEDIA_TITLE") + " %1").arg(DVDInfo.groupOrTitleNum);
                    emit trackNumberChanged();
                    
                    m_history[MP::DISC].Quantity = DVDInfo.totalChaptorOrTrack;
                    emit getdvdInfo(DVDInfo.groupOrTitleNum, DVDInfo.trackOrChapterNo, DVDInfo.totalChaptorOrTrack, DVDInfo.totalGroupOrTitle); // modified by ravikanth 12-09-24
                }

                MP_LOG << m_strTrackNumber << ":" << m_strGroupNumber << LOG_ENDL;
                // } modified by kihyung
            }
            break;
        }
        default:
            break;
    }
    MP_LOG << m_discType << status << LOG_ENDL;
}

// { added by ravikanth - 12-09-26
void AppMediaPlayer_Controller_Audio::storeDVDdata()
{
     MP_LOG << LOG_ENDL;
    int status = -1;
    if (m_pIVideo && m_pIMedia)
    {
        DVDStatusResponse DVDInfo;
        status = m_pIVideo->GetDVDStatus(&DVDInfo);
        m_tempDVDStatus.groupOrTitleNum = DVDInfo.groupOrTitleNum;
        m_tempDVDStatus.trackOrChapterNo = DVDInfo.trackOrChapterNo;
        m_tempDVDStatus.playbackTime = m_pIMedia->GetCurrentMediaPosition();
        MP_LOG << m_tempDVDStatus.groupOrTitleNum << m_tempDVDStatus.trackOrChapterNo << m_tempDVDStatus.playbackTime << LOG_ENDL;
        MP_LOG << "status" << status << LOG_ENDL;
        m_isDVDinfoRestored = false;
    }
}

void AppMediaPlayer_Controller_Audio::restoreDVDdata()
{
    MP_LOG << "title:" <<  m_tempDVDStatus.groupOrTitleNum << " chapter:" << m_tempDVDStatus.trackOrChapterNo <<" position:"<< m_tempDVDStatus.playbackTime << LOG_ENDL;
    selectTitleChapter(m_tempDVDStatus.groupOrTitleNum, m_tempDVDStatus.trackOrChapterNo);
    m_isDVDinfoRestored = true;
    m_tempDVDStatus.groupOrTitleNum = 0;
    m_tempDVDStatus.trackOrChapterNo = 0;
}
// } added by ravikanth - 12-09-26

// { added by eugeny.novikov@teleca.com 2012.08.09 for CR 12186
int AppMediaPlayer_Controller_Audio::getDiscType() const
{
    return (m_discType == MEDIA_SOURCE_CDDA)     ? MP::AUDIO_CD :
           (m_discType == MEDIA_SOURCE_MP3)      ? MP::MP3_CD :
           (m_discType == MEDIA_SOURCE_DVDAUDIO) ? MP::DVD_AUDIO : MP::DISC_NONE;
}
// } added by eugeny.novikov

void AppMediaPlayer_Controller_Audio::setGracenoteHighlightedVolume(MP::DEVICE_T device)
{
    switch (device)
    {
        case MP::JUKEBOX:
            emit signalGracenoteHighlightedVolume(0, NULL, NULL);
            break;
        case MP::USB1:
            emit signalGracenoteHighlightedVolume(1, m_history[MP::USB1].DeviceID,
                                                  m_history[MP::USB2].MountPath);
            break;
        case MP::USB2:
            emit signalGracenoteHighlightedVolume(2, m_history[MP::USB2].DeviceID,
                                                  m_history[MP::USB2].MountPath);
            break;
        case MP::IPOD1:
            break;
        case MP::DISC:
            emit signalGracenoteHighlightedVolume(3, NULL, NULL);
            break;
        default:
            break;
    }
}

// { added by junam 2012.10.25 for CR14451
void
AppMediaPlayer_Controller_Audio::onRequestCoverArtForFlowView(void)
{
    MP::DEVICE_T device = getDevice();
    
    if ( ( device == MP::UNDEFINED) || ( m_pPathViewModel == NULL) ||
            ( m_pTrackerAbstractor == NULL))
    {
        return;
    }

    //{added by junam 2013.04.29 for not showing flow viw
    if(m_bIsBasicView)
        return;
    //}added by junam

    QStringList reqAlbums, reqArtists, reqFiles;
    if (m_pPathViewModel->getOnDemandCoverArt( reqAlbums, reqArtists, reqFiles,
                                              (device == MP::IPOD1 || device == MP::IPOD2) ? 1 : 2))
    {
        MP_LOG << "ROWS" << LOG_ENDL;
        m_pTrackerAbstractor->RequestData(reqAlbums, reqArtists, reqFiles,
                                          eRetrieveCoverArtOnDemand_Id, GET_TA_REQUID(REQUID_REQUST_COVER_FLOW_VIEW));
    }
}
// } added by junam


// { added by junam 2012.10.12 for CR14040
void
AppMediaPlayer_Controller_Audio::onRequestCoverArtForListView()
{
    MP::DEVICE_T device = getDevice();
    
    if ( ( device == MP::UNDEFINED) || ( m_pListViewModel == NULL) ||
            ( m_pTrackerAbstractor == NULL))
    {
        return;
    }

    QStringList reqAlbums, reqArtists, reqFiles;
    if (m_pListViewModel->getOnDemandCoverArt( reqAlbums, reqArtists, reqFiles, 1)) //modified by jaehwan.lee 2013.12.14 for ISV 96345
    {
        MP_LOG << "ROWS" << LOG_ENDL;
        m_pTrackerAbstractor->RequestData(reqAlbums, reqArtists, reqFiles,
                                          eRetrieveCoverArtOnDemand_Id, GET_TA_REQUID(REQUID_REQUST_COVER_LIST_VIEW));
    }
}
// } added by junam

// { added by eugene.seo 2013.01.18 for 25000 over USB
void 
AppMediaPlayer_Controller_Audio::onRequestFirstSong()
{
        requestCovers();//false); //changed by junam 2013.11.06 for coverflow update
}

DeviceHistory*
AppMediaPlayer_Controller_Audio::GetDeviceHistory(int device)
{
    //MP_LOG << DeviceName(device) << LOG_ENDL;
    return (device < MP::ALL_DEVICES) ? &m_history[device] : NULL;
}

// { added by eugeny.novikov 2012.08.15 for CR 12578
QList<DeviceHistory*>
AppMediaPlayer_Controller_Audio::GetExtendDeviceHistory()
{
    QList<DeviceHistory*> retList;
    
    for (int i = 0; i < m_usbDevicesHistory.count(); i++)
    {
        retList.append(&(m_usbDevicesHistory[i]));
    }
    
    return retList;
}
// } added by eugeny.novikov

void
AppMediaPlayer_Controller_Audio::RestoreFromHistory(MP::DEVICE_T device)
{
    // { modified by kihyung 2013.07.25 for ITS 0181470   
    MP_HIGH << "START" << DeviceName(device) << LOG_ENDL;

	if(m_pIMedia == NULL){ 
        MP_HIGH << "no m_pIMedia." << LOG_ENDL;
        return;
	}

    if(device >= MP::ALL_DEVICES) {
        MP_HIGH << "no device." << LOG_ENDL;
        return;
    }
    
    PLAYER_STATE currentState = (PLAYER_STATE)m_pIMedia->GetCurrentMediaState();

    MP_HIGH << "TanDebug currentState: " <<currentState << LOG_ENDL;

    // Temporary: m_albumName is moved from previous realization,
    // it should be reworked or not used.
    m_albumName = m_history[device].Album;

	GetEngineMain().setIsRestored(true); //added by edo.lee 2013.03.16

	// { modified by kihyung 2013.08.08 for ITS 0183321
    /*
	//added by edo.lee 2013.06.10
	if(m_bNewUSB1Connected && device == MP::USB1) 
	{
		m_bNewUSB1Connected = false;
        // RestoreEmptyUI(); //added by junam 2013.06.27 for ITS0159083 // removed by kihyung 2013.07.18 for ITS 0179389
	}
	else if(m_bNewUSB2Connected && device == MP::USB2)
	{
		m_bNewUSB2Connected = false;
        // RestoreEmptyUI(); //added by junam 2013.06.27 for ITS0159083 // removed by kihyung 2013.07.18 for ITS 0179389
	}
    else
	{
	    RestoreUI(device);
	}
	//added by edo.lee 2013.06.10
	*/
	
	RestoreUI(device);
	// } modified by kihyung 2013.08.08 for ITS 0183321

    // { modified by kihyung 2013.3.9
    if (currentState != PLAYER_PLAY)
    {
        switch (device)
        {
            case MP::JUKEBOX:
            {
                if(GetEngineMain().isTAScanComplete(eJukeBox) == true && IsPlaylistEmpty() == false) {
                    MP_HIGH << "isTAScanComplete(eJukeBox) == true m_isChangeToSame :"<<m_isChangeToSame <<"currentState "<<currentState << LOG_ENDL;
                    m_bJukeboxDelayedPlay = false;
                    if (currentState == PLAYER_STOP)
                    {
                        m_bRestoring = true;
                        SetPlaylist(device, m_history[device].Index);
                    }
                    //if(GetEngineMain().getisBTCall() == false) // commented by Sergey 17.05.2013
					if (GetEngineMain().getKeepPlayState()) 
					{
						IsPlaying() ? Play() : Pause();
					}else if(!m_isChangeToSame ){
                        Play();
					}
                }else{
                    if(IsPlaylistEmpty()) {
                        MP_HIGH << "Clear existing playlist"  << LOG_ENDL;
                        requestAllPlaylist(device);
                    }
                    if(GetEngineMain().isTAScanComplete(eJukeBox) == false) {
                        MP_HIGH << "isTAScanComplete(eJukeBox) == false" << LOG_ENDL;
                        m_bJukeboxDelayedPlay = true;    
                    }
                    else 
					{
						if (GetEngineMain().getKeepPlayState()) 
						{
							IsPlaying() ? Play() : Pause();
						}else if(!m_isChangeToSame ){
	                        Play();
						}
					}
                }
                break;
            }
            case MP::USB1:
            {
                if(GetEngineMain().isTAScanComplete(eUSB_FRONT) == true && IsPlaylistEmpty() == false) {
                    MP_HIGH << "isTAScanComplete(eUSB_FRONT) == true" << LOG_ENDL;
                    m_bUSB1DelayedPlay = false;
                    m_bRestoring = true;
                    Stop();
                    SetPlaylist(device, m_history[device].Index);
                    // if(GetEngineMain().getisBTCall() == false) // commented by Sergey 17.05.2013
                    if (GetEngineMain().getKeepPlayState()) 
					{
						IsPlaying() ? Play() : Pause();
					}else if(!m_isChangeToSame ){
                        Play();
					}
                }else{
                    if(IsPlaylistEmpty()) {
                        MP_HIGH << "Clear existing playlist"  << LOG_ENDL;
                        requestAllPlaylist(device);
                    }
                    if(GetEngineMain().isTAScanComplete(eUSB_FRONT) == false) {
                        MP_HIGH << "isTAScanComplete(eUSB_FRONT) == false" << LOG_ENDL;
                        m_bUSB1DelayedPlay = true;
                    }
                    else {
                        if (GetEngineMain().getKeepPlayState()) 
						{
							IsPlaying() ? Play() : Pause();
						}else if(!m_isChangeToSame ){
	                        Play();
						}
                    }
                }
                break;
            }
            case MP::USB2:
            {
                if(GetEngineMain().isTAScanComplete(eUSB_REAR) == true && IsPlaylistEmpty() == false) {
                    MP_HIGH << "isTAScanComplete(eUSB_REAR) == true" << LOG_ENDL;
                    m_bUSB2DelayedPlay = false;
                    m_bRestoring = true;
                    Stop();
                    SetPlaylist(device, m_history[device].Index);
                    // if(GetEngineMain().getisBTCall() == false) // commented by Sergey 17.05.2013
                    if (GetEngineMain().getKeepPlayState()) 
					{
						IsPlaying() ? Play() : Pause();
					}else if(!m_isChangeToSame ){
                        Play();
					}
                }else{
                    if(IsPlaylistEmpty()) {
                        MP_HIGH << "Clear existing playlist"  << LOG_ENDL;
                        requestAllPlaylist(device);
                    }
                    if(GetEngineMain().isTAScanComplete(eUSB_REAR) == true) {
                        MP_HIGH << "isTAScanComplete(eUSB_REAR) == false" << LOG_ENDL;
                        m_bUSB2DelayedPlay = true;
                    }
                    else {
                        if (GetEngineMain().getKeepPlayState()) 
						{
							IsPlaying() ? Play() : Pause();
						}else if(!m_isChangeToSame ){
	                        Play();
						}
                    }
                }
                break;
            }
            case MP::DISC:
            {
                m_bRestoring = true;
                if (m_discType == MEDIA_SOURCE_MP3)
                    m_history[device].ContentPlayed = MP::FOLDER;
                m_pIMedia->SetCurrentIndex(m_history[device].Index);
                // if(GetEngineMain().getisBTCall() == false) // commented by Sergey 17.05.2013
                if (GetEngineMain().getKeepPlayState()) 
				{
					IsPlaying() ? Play() : Pause();
				}else if(!m_isChangeToSame ){
                    Play();
				}
                break;
            }
            case MP::IPOD1:
            case MP::IPOD2:
            {
                //{added by junam 2013.07.12 for music app
                if(m_pIMedia && GetEngineMain().getisBTCall())
                {
                    MP_HIGH << "TanDebug IPODDEVICES_TABON" << LOG_ENDL;
                    m_pIMedia->SetSystemState( SYSTEM_IPODDEVICES_TABON);
                    //Tan 2014.09.01 with ITS Issue KH 0246889 and VI 0246890
                    OSDManager::instance()->displayOSD(OSD_EVENT_IPOD_EMPTY_TITLE);
                    break;
                }
                //}added by junam

                // if(GetEngineMain().getisBTCall() == false) // commented by Sergey 17.05.2013
				if (GetEngineMain().getKeepPlayState())
				{
                    MP_HIGH << "TanDebug KeepPlayState true" << LOG_ENDL;
					IsPlaying() ? Play() : Pause();
				}else if(!m_isChangeToSame ){
                    MP_HIGH << "TanDebug changetosame false" << LOG_ENDL;
					Play();
				}
                else
                {
                    MP_HIGH << "TanDebug not handled" << LOG_ENDL;
                }
                break;
            }
            case MP::BLUETOOTH:
            case MP::AUX:
            default:
            {
                MP_LOG << "Unsupported device" << LOG_ENDL;
                break;
            }
        }
    }
    else
    {
        //Suryanto Tan 2014.09.04 ITS 0247861
        //if user do view Playlist --> AV OFF --> Media Key fast,
        //then the current play state can be playing (but actually it will be paused)
        //To resume playing, we should send play (even though the playstate is already play)
        MP_HIGH<<"Exception case: playState already playing" << LOG_ENDL;
        switch(device)
        {
        case MP::IPOD1:
        case MP::IPOD2:
            if (GetEngineMain().getKeepPlayState())
            {
                MP_HIGH << "TanDebug 2 KeepPlayState true" << LOG_ENDL;
                IsPlaying() ? Play() : Pause();
            }else if(!m_isChangeToSame ){
                MP_HIGH << "TanDebug 2 changetosame false" << LOG_ENDL;
                Play();
            }
            else
            {
                MP_HIGH << "TanDebug 2 not handled" << LOG_ENDL;
            }
            break;
        default:
            break;
        }

    }

	//GetEngineMain().resetKeepPlayState();//remove by edo.lee 2013.08.01 bt call on pwroff
	   // } modified by edo.lee 2013.07.30 bt call play state
    // } modified by kihyung 2013.3.9
    
    MP_LOG << "END" << LOG_ENDL;
    // } modified by kihyung 2013.07.25 for ITS 0181470  
}

// { modified by kihyung 2013.08.02 for ITS 0180215
void AppMediaPlayer_Controller_Audio::RestoreFromHistoryAfterTAScan(ETrackerAbstarctor_VolumeType volumeType, MP::DEVICE_T device, bool bRescan, bool bPlayAfterStop)
{
    MP_HIGH << volumeType << device << bRescan << bPlayAfterStop << LOG_ENDL;
    
    if(m_pIMedia == NULL){ 
        MP_LOG << "no m_pIMedia." << LOG_ENDL;
        return;
	}

    if(device >= MP::ALL_DEVICES) {
        MP_HIGH << "no device." << LOG_ENDL;
        return;
    }
    
    switch (device)
    {
        case MP::JUKEBOX:
        {
            if(volumeType != eJukeBox) break;
            // { modified by kihyung 2013.08.02 for ITS 0180215
            if(m_bJukeboxDelayedPlay == true || bRescan == true) 
            {
                MP_LOG << "TASCAN m_bJukeboxDelayedPlay == true" << LOG_ENDL;
                m_bRestoring = true;
                SetPlaylist(device, m_history[device].Index);
                if(bPlayAfterStop == true) {
                    Stop();
                    Play();
                }
                else if(bRescan == false) {
                    Play();
                }
            }
            // } modified by kihyung 2013.08.02 for ITS 0180215            
            break;
        }
        case MP::USB1:
        {
            if(volumeType != eUSB_FRONT) break;
            if(m_bUSB1DelayedPlay == true) {
                MP_LOG << "TASCAN m_bUSB1DelayedPlay == true" << LOG_ENDL;
                m_bRestoring = true;
                Stop();
                SetPlaylist(device, m_history[device].Index);
                Play();
            }
            break;
        }
        case MP::USB2:
        {
            if(volumeType != eUSB_REAR) break;
            if(m_bUSB2DelayedPlay == true) {
                MP_LOG << "TASCAN m_bUSB2DelayedPlay == true" << LOG_ENDL;
                m_bRestoring = true;
                Stop();
                SetPlaylist(device, m_history[device].Index);
                Play();
            }
            break;
        }
        default:
        {
            MP_LOG << "Unsupported device" << LOG_ENDL;
            break;
        }
    }
}
// } modified by kihyung 2013.08.02 for ITS 0180215

void
AppMediaPlayer_Controller_Audio::RestoreUI(MP::DEVICE_T device)
{
    if (device < MP::ALL_DEVICES)
    {
        MP_HIGH << "START" << LOG_ENDL;

        // { modified by kihyung 2013.08.08 for ITS 0183321
        /*
        if(m_history[device].Title == "" && m_history[device].Album == "")
        {
            MediaInfo pInfo;
            GetCurrentMediaInfo(&pInfo);
            m_history[device].Title = pInfo.Title;
            m_history[device].Album = pInfo.Album;
            m_history[device].Artist = pInfo.Artist;
            if(m_history[device].repeatPlayMode == 0 && m_history[device].randomPlayMode == 0 )
            {
	            m_history[device].repeatPlayMode = REPEATALL;
	            m_history[device].randomPlayMode = RANDOMOFF;
            }
        }
        
        MP_LOG << m_history[device].Title << m_history[device].Album << LOG_ENDL;
        */
        
        EmitOnMediaInfoChanged(device, true, false);  // modified by kihyung 2013.08.21 for ITS 0185499 
        // } modified by kihyung 2013.08.08 for ITS 0183321
		
        if(m_history[device].Duration == 0)
        {
            if(m_pIMedia)
            {
                m_history[device].Duration = m_pIMedia->GetCurrentMediaDuration();
            }
        }
        
        //{ modified by cychoi 2013.07.25 for ISV 87752
        if(device == MP::DISC) 
        {
            // History duration & position is not accurate if DISC
            // So delay to update duration & position until receive it from Deck
            emit durationChanged(m_history[device].Duration); // modified by cychoi 2013.09.24 for ITS 190742 CDDA duration
            emit positionChanged(0);
        }
        else
        {
            emit durationChanged(m_history[device].Duration);
            emit positionChanged(m_history[device].Position);
        }
        //} modified by cychoi 2013.07.25

        if (m_pIMedia)
        {
            if(device != MP::IPOD1 && device != MP::IPOD2) 
            {
                // { added by kihyung 2013.08.24 for ITS 0186423. Maintain scan mode if old device and new device is same. 
                if(getScanMode() == SCANOFF) 
                {
                    m_pIMedia->RandomPlay(m_history[device].randomPlayMode, !m_isChangeToSame); //modified by edo.lee 2013.05.18
                    m_pIMedia->RepeatPlay(m_history[device].repeatPlayMode, !m_isChangeToSame); //modified by edo.lee 2013.05.18
                    
                    emit setStatusDataByIndex(device, DATA_TYPE_RANDOM, (m_history[device].randomPlayMode == RANDOMOFF) ? STATUS_OFF : STATUS_ON);
                    emit setStatusDataByIndex(device, DATA_TYPE_REPEAT, (m_history[device].repeatPlayMode == REPEATFILE) ? STATUS_ON : STATUS_OFF);

                    updateRepeatRandomIcon(m_history[device].repeatPlayMode, m_history[device].randomPlayMode); // added by kihyung.lee 2013.08.31 for ITS 0187529
                }
                // } added by kihyung 2013.08.24 for ITS 0186423. Maintain scan mode if old device and new device is same. 
            }
        }

        MP_LOG << "END" << LOG_ENDL;    // removed by sangmin.seol 2014.07.07 Chnage Log Level
    }
    else
    {
        MP_HIGH << "ERROR device is" << device << LOG_ENDL;
    }
}

// { modified by kihyung 2013.08.08 for ITS 0183321
void AppMediaPlayer_Controller_Audio::RestoreEmptyUI(MP::DEVICE_T device)
{
    EmitOnMediaInfoChanged(device, FALSE);
}
// } modified by kihyung 2013.08.08 for ITS 0183321

// {added by edo.lee 2012.11.28  for ISV 64487 
bool
AppMediaPlayer_Controller_Audio::IsNoSourceByDeviceid(int deviceid)
{
	
	if(m_history[deviceid].Quantity > 0)
	{
		MP_LOG <<" m_history["<<deviceid<<"].Quantity : "<<m_history[deviceid].Quantity << LOG_ENDL;		
		return false;
	}
	
	return true;
}
bool
AppMediaPlayer_Controller_Audio::IsNoSourceBySourceid(int sourceid)
{

	MP::DEVICE_T device = AudioUtils::DeviceType((MEDIA_SOURCE_T)sourceid);
	return IsNoSourceByDeviceid(device);
}
// }added by edo.lee


bool
AppMediaPlayer_Controller_Audio::IsPlaylistEmpty()
{
    return (getDevice() == MP::UNDEFINED ||
            m_history[getDevice()].playList.list == NULL ||
            m_history[getDevice()].playList.list->empty());
}

bool
AppMediaPlayer_Controller_Audio::IsPlaylistValid(MP::DEVICE_T device)
{
    if (device < MP::ALL_DEVICES)
    {
        if(device == MP::JUKEBOX || device == MP::USB1 || device == MP::USB2)   // { addded by sungha.choi 2013.07.31 for scandir performance
        {
            ETrackerAbstarctor_VolumeType volumeType = AudioUtils::VolumeType(device);
            int count = 0;
            foreach (QString track, *m_history[device].playList.list)
            {
                count ++;
                //if (QFile::exists(track) == false)
                if(m_pTrackerAbstractor && (m_pTrackerAbstractor->FileExists(track, volumeType) == false))
                {
                    MP_HIGH << "Error : playlist is broken -" << track << LOG_ENDL;
                    return false;
                }
            }
        }   // } addded by sungha.choi 2013.07.31 for scandir performance
        else
        {
            foreach (QString track, *m_history[device].playList.list)
            {
                if (QFile::exists(track) == false)
                {
                    MP_HIGH << "Error: playlist is broken -" << track << LOG_ENDL;
                    return false;
                }
            }
        }
    }
    MP_LOG << "Playlist is valid" << LOG_ENDL;
    return true;
}

void
AppMediaPlayer_Controller_Audio::RequestBeforeIndexingPlaylist(QString mountPath)
{
    MP::DEVICE_T source; 

    if(mountPath == m_history[MP::USB1].MountPath) 
        source = MP::USB1;
    else if(mountPath == m_history[MP::USB2].MountPath)
        source = MP::USB2; 
    else if(mountPath == m_history[MP::JUKEBOX].MountPath) 
        source = MP::JUKEBOX;
    else 
        source = MP::UNDEFINED;

    MP_HIGH << DeviceName(source) << mountPath << LOG_ENDL;

    if (m_pTrackerAbstractor && source != MP::UNDEFINED)
    {
        MP_LOG << "TASCAN -> request eGetMusicLimitFileListDir_Id" << LOG_ENDL;
        m_pTrackerAbstractor->RequestData(AudioUtils::VolumeType(source), eGetMusicLimitFileListDir_Id, REQUID_ALL_FILELIST_HKMC);
    }
}

// { removed by eugeny.novikov@teleca.com 2012.08.01 for CR 11079
//void AppMediaPlayer_Controller_Audio::onNeedToUseQDir(QString mountPath)
//{
//    MP_LOG << mountPath << LOG_ENDL;

//    m_beforeIndexingDevices << mountPath;
//    RequestBeforeIndexingPlaylist(mountPath);
//}
// } removed by eugeny.novikov

/* It's used only to pass data from IPodController to Gracenote.
 * No MP logic or handling.
 */
void AppMediaPlayer_Controller_Audio::ipodDisconnectedSlot(QString deviceId, int volume)
{
    MP_HIGH << "     IPodSignal " << "volume:" << volume << LOG_ENDL;
    //MP_HIGH << "     Gracenote on IPOD funcionality is removed." << LOG_ENDL; // removed by sangmin.seol 2014.07.07 Reduce Log. //added by aettie.ji 2013.02.04 for ISV 61842,61749

    emit mediaSyncfinished(false);
    m_pPathViewModel->stopRequestCoverTimer(); // added by Dmitry 02.05.13

    EIPodPorts ePortNum;
	if(volume == 1) {
        ePortNum = eIPOD_PORT_1;
    }
    else if(volume == 2) {
        ePortNum = eIPOD_PORT_2;
    }
    else {
        ePortNum = eIPOD_PORT_NONE;
    }

	GetEngineMain().IPodDeviceDisconnected( deviceId, ePortNum );
#ifdef DUAL_KEY
   MP::DEVICE_T device = getDevice();
   if(device==MP::IPOD1||device==MP::IPOD2)
#endif
    cancelFFRW();//added by edo.lee 2013.07.03 ITS 0177339
}

//{removed by junam 2013.07.10 for calcel ff
/* It's used only to pass data from IPodController to Gracenote.
 * No MP logic or handling.
 */
//void AppMediaPlayer_Controller_Audio::ipodConnectedSlot(QString deviceId, int startIndex, int endIndex,
//                                                        int volume, int syncType)
//{
//	//{removed by junam -- too many useless log
//    //MP_LOG << "IPodSignal " << "volume:" << volume << "syncType:" << syncType << LOG_ENDL;
//    //MP_LOG << "Gracenote on IPOD funcionality is removed." << LOG_ENDL; //added by aettie.ji 2013.02.04 for ISV 61842,61749
//	//}removed by junam
//    // { commented by kihyung 2012.08.04 for CR 11725
//    // { added by kihyung 2012.08.01 for CR 11725
//    // emit iPodSyncStep(syncType);
//    // } added by kihyung
//    // } commented by kihyung
//	//deleted by aettie.ji 2013.02.04 for ISV 61842,61749
//    /*if (volume == 1)
//    {
//        emit signalGracenoteDeviceStateChanged(MP::IPOD1, deviceId, "", startIndex, endIndex, syncType, true);
//    }
//    else if (volume == 2)
//    {
//        emit signalGracenoteDeviceStateChanged(MP::IPOD2, deviceId, "", startIndex, endIndex, syncType, true);
//    }*/
//	//deleted by aettie.ji 2013.02.04 for ISV 61842,61749
//        cancelFFRW();//added by edo.lee 2013.07.03 ITS 0177339
//}
//}removed by junam

void AppMediaPlayer_Controller_Audio::onGetDeviceName1(QString name)
{
    // modified by kihyung 2013.07.08
    MP_HIGH << LOG_ENDL;
    m_iPod1DeviceName = name;
    emit iPodDeviceConnectInfo(1, m_iPod1DeviceName, true);
}

void AppMediaPlayer_Controller_Audio::onGetDeviceName2(QString name)
{
    // modified by kihyung 2013.07.08
    MP_HIGH << LOG_ENDL;
    m_iPod2DeviceName = name;
    emit iPodDeviceConnectInfo(2, m_iPod2DeviceName, true);
}


// { added by jungae 2012.10.09, for CR14252, iPod reset when device name is same with BT
void AppMediaPlayer_Controller_Audio::onSystemStateStatus(int status)
{
    if(status == SYSTEM_IPODDEVICE_RESET_OFF)
    {
        MP_HIGH << "ResetMode :" << status << LOG_ENDL;
        Play();
    }
}
// } added by jungae 2012.10.09, for CR14252, iPod reset when device name is same with BT

void AppMediaPlayer_Controller_Audio::displayiPodTaggingOSD(EIPodPopUp nMessageId)
{
   MP_LOG << "message id :" << nMessageId << LOG_ENDL;
   switch( nMessageId )
   {
       case iPodiTunes_SENDINGTAGS :
       {
			OSDManager::instance()->displayOSD(OSD_EVENT_IPOD_SENDINGTAGS);
			break;
       }
       case iPodiTunes_MEMORYFULL :
       {
			OSDManager::instance()->displayOSD(OSD_EVENT_IPOD_MEMORYFULL);
			break;
       }
       case iPodiTunes_SUCCESS :
       {
			OSDManager::instance()->displayOSD(OSD_EVENT_IPOD_SUCCESS);
			break;
       }
       case iPodiTunes_FAILED :
       {
			OSDManager::instance()->displayOSD(OSD_EVENT_IPOD_FAILED);
			break;
       }
       // { added by kihyung 2012.07.09 for CR 10343
       case IPOD_POPUP_UNSUPPORTED :
       {
			m_bIsAppleConnectError = true;
			OSDManager::instance()->displayOSD(OSD_EVENT_IPOD_UNSUPPORTED);
			break;
       }
       case IPOD_POPUP_CONNECTION_ERROR : 
       {
			m_bIsAppleConnectError = true;
			OSDManager::instance()->displayOSD(OSD_EVENT_IPOD_CONNECTION_ERROR);
			break;
       }
       // } added by kihyung
       default :
       {
           MP_LOG << "undefined message id" << LOG_ENDL;
           break;
       }
   }
}

bool AppMediaPlayer_Controller_Audio::AutoplayAllowed()
{
    // Complex condition to make a decision autoplay or not
    // when albums data are received from tracker:
    return (m_bRestoring == false &&
            m_PlayerState == PLAYER_STOP &&
            IsPlaylistEmpty());
}

bool
AppMediaPlayer_Controller_Audio::isEnableErrorPopup() const
{
    return (m_enableErrorPopupState == ERROR_POPUP_ENABLE);
}

void
AppMediaPlayer_Controller_Audio::setIsEnableErrorPopup( bool bEnable )
{
    if (bEnable)
    {
        if( m_enableErrorPopupState != ERROR_POPUP_ENABLE)
            m_enableErrorPopupState = ERROR_POPUP_READY;
    }
    else
    {
        m_enableErrorPopupState = ERROR_POPUP_DISABLE;
    }
}

void AppMediaPlayer_Controller_Audio::onIPodTotalNumTracks(int total)
{
    //{changed by junam 2013.04.17 for empty iPod
    MP::DEVICE_T source = GetSignalSource();
    //emit contentSize(AudioUtils::DeviceType(m_device), MEDIA_TYPE_MUSIC, total);
    if(source == getDevice() )
    {
        emit contentSize(AudioUtils::DeviceType(source), MEDIA_TYPE_MUSIC, total);
    }
    //}changed by junam

    //{removed by junam 2013.04.17 for total count
    //    m_history[source].Index = 0;
    //    m_history[source].Quantity = total;
    //}removed by junam

    //Added by Alexey Edelev. 2012.09.08 CR13600, CR13610
    //{
    //{changed by junam 2013.04.17 for empty iPod
    //if(m_device == MP::IPOD1)
    if(source == MP::IPOD1)
    {//}changed by junam
        if(total < 1)  // added by sungha.choi. 2013.03.10 this "if" condition need to be confirmed. (Temporary fix)
        {
            m_iPod1ContentSizeReady = true;
        }
    }//{changed by junam 2013.04.17 for empty iPod
    //else if(m_device == MP::IPOD2)
    else if(source == MP::IPOD2)
    {//}changed by junam
        if(total < 1)  // added by sungha.choi. 2013.03.10 this "if" condition need to be confirmed. (Temporary fix)
        {
            m_iPod2ContentSizeReady = true;
        }
    }
    //}
    //Added by Alexey Edelev. 2012.09.08 CR13600, CR13610

    //{added by junam 2013.04.17 for empty iPod
    GetEngineMain().GetMediaManager()->SetSourceAvailable(AudioUtils::DeviceType(source), MEDIA_TYPE_MUSIC, total > 0);
    GetEngineMain().GetMediaManager()->EmitDeviceAttached(AudioUtils::DeviceType(source), MEDIA_TYPE_MUSIC, false);
    //}added by junam

    GetEngineMain().GetNotifier()->UpdateSDPMediaCount(AudioUtils::DeviceType(source), MEDIA_TYPE_MUSIC, total);

    if (total < 1)
    {
        //{changed by junam 2013.08.27 for no media osd
        //GetEngineMain().GetNotifier()->displayNoMediaFileOSD(AudioUtils::DeviceType(source));
        OSDManager::instance()->displayNoMediaFileOSD(AudioUtils::DeviceType(source));
        //}changed by junam
    }
}

CIpodController*
AppMediaPlayer_Controller_Audio::GetIPodController(MP::DEVICE_T type)
{
    CIpodController* controller = NULL;

    if (getDevice() == type)
    {
        controller = dynamic_cast<CIpodController*>(m_pIMedia);
    }
    else if (type == MP::IPOD1)
    {
        controller = GetControllers().GetIPod1Controller();
    }
    else if (type == MP::IPOD2)
    {
        controller = GetControllers().GetIPod2Controller();
    }
    return controller;
}

void AppMediaPlayer_Controller_Audio::ExitFromMLT()
{
}

void AppMediaPlayer_Controller_Audio::sendUpdateOSD(unsigned int nEventType)
{
	// { modified by eugene.seo 2012.10.23 for Function_USB_0190
    //if( !isForeground() )
    if(  !isForeground() || (!m_isListView && nEventType == OSD_EVENT_COPY_COMPLETED)) 
    {
	// } modified by eugene.seo 2012.10.23 for Function_USB_0190
		OSDManager::instance()->displayOSD(nEventType);
    }
}

void AppMediaPlayer_Controller_Audio::CopyHistory(DeviceHistory &dst, const DeviceHistory &src) // modified by eugeny.novikov 2012.08.15 for CR 12578
{
    MP_LOG << LOG_ENDL;
    QList<QString>* pList = dst.playList.list;
    pList->clear();
    *pList << *src.playList.list;
    dst = src;
    dst.playList.list = pList;
}

void AppMediaPlayer_Controller_Audio::CopyHistoryFromUBSExtended(DeviceHistory &dst, const DeviceHistory &src, QString mountPath)
{
    MP_HIGH << "from " << src.DeviceID << mountPath << LOG_ENDL;
    
    QList<QString> *pDstList = dst.playList.list;
    QList<QString> *pSrcList = src.playList.list;
    
    pDstList->clear();
    
    for(int i = 0; i < pSrcList->size(); i++)
    {
        pDstList->append(mountPath + pSrcList->at(i));
    }

    dst = src;
    dst.playList.list = pDstList;
}

void AppMediaPlayer_Controller_Audio::CopyHistoryToUSBExtended(DeviceHistory &dst, const DeviceHistory &src)
{
    MP_HIGH << src.DeviceID << "to USBExtended" << LOG_ENDL;
    
    QList<QString> *pDstList = dst.playList.list;
    QList<QString> *pSrcList = src.playList.list;
    
    pDstList->clear();
   
    int nIndex = src.MountPath.size();

    if(nIndex <= 1) {
        MP_HIGH << nIndex << "return..." << LOG_ENDL;
        dst = src; 
        dst.playList.list = pDstList;  
        return;
    }
    
    for(int i = 0; i < pSrcList->size(); i++)
    {
        QString strFile = pSrcList->at(i);
        pDstList->append(strFile.mid(nIndex));
    }
    
    dst = src; 
    dst.playList.list = pDstList;  
}

// { Add by Naeo 2012.07.09 for CR 10970
QStringList
AppMediaPlayer_Controller_Audio::getDiscInfoList()
{
    MP_LOG << LOG_ENDL;
    // { modified by eugeny.novikov 2012.08.15 for CR 12578
//    QStringList returnList = m_listDiscTitle;
    QStringList returnList;

    foreach (CD_META_INFO cdInfo, m_listCDContent)
        returnList.append(cdInfo.trackTitle);
    // } modified by eugeny.novikov

    return returnList;
}

// { added by jaden1.lee 2012.08.13 CR 11655
void
AppMediaPlayer_Controller_Audio::receivedCurrentFolderName(DvdcFolderName *pFolderName )
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
    
    MP_LOG << LOG_ENDL;
    
    // { modified by cychoi 2013.10.07 for Prevent Fix. Should return if NULL
    //{ modified by yongkyun.lee 2013-07-22 for : ITS 180714 =>2013-07-29 for : NO CR MP3 folder name
    if(pFolderName == NULL)//Added by Taihyun.ahn 2013.10.05
    {
        return;
    }
    if( (pFolderName->name[0] == 0x00 && pFolderName->name[1] == 0x00) )
    {
        MP_LOG << pFolderName->valid << LOG_ENDL;
        return;
    }
    //} modified by yongkyun.lee 2013-07-22 
    // } modified by cychoi 2013.10.07

    QString m_folderTuneTmp = charToUnicode(pFolderName->name, pFolderName->unicode);
    
    if (m_folderTuneTmp == "ROOT FOLDER" && pFolderName->folder_no == 0) 
    {
	//modified by aettie 2013 08 05 for ISV NA 85625
//        m_folderTune = "/..";
// modified by aettie 20130812 for ITS 183630
       // m_folderTune = "Root";
        m_folderTune = "STR_MEDIA_ROOT_FOLDER"; // modified by Dmitry 05.10.13 for ITS0193742
    }
    else {
        m_folderTune = m_folderTuneTmp;
    }
    
    m_history[device].Folder = m_folderTune;
    
    emit updateFolderName( m_folderTune );
}

// { added by cychoi 2013.10.07 for ISV 92215 Folder Display on Track Change
void
AppMediaPlayer_Controller_Audio::updateCurrentMP3FolderName()
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;

    MP_LOG << LOG_ENDL;

    QString currentFolderName = m_pTreeModel->getCurrentFolderName();

    if(currentFolderName == "")
    {
        // Tree is not fetched fully, request current folder name to Deck then just return.
        MP_LOG << "Call requestCurrentFolderName()" << LOG_ENDL;
        requestCurrentFolderName();
        return;
    }
 
    if (currentFolderName == "ROOT FOLDER") 
    {
        m_folderTune = "STR_MEDIA_ROOT_FOLDER"; // modified by Dmitry 05.10.13 for ITS0193742
    }
    else
    {
        m_folderTune = currentFolderName;
    }
    
    m_history[device].Folder = m_folderTune;

    emit updateFolderName( m_folderTune );
}
// } added by cychoi 2013.10.07

QString 
AppMediaPlayer_Controller_Audio::charToUnicode( char *pIn, int bUnicode )
{
    if ( !pIn )
        return "";

    int readLength;
    QChar qChar[DIR_TREE_FILE_NAME_SIZE];
    QString resultStr;

    if( bUnicode == 1 )
    {
        unsigned char   p[2];
        unsigned short  *q = (unsigned short*)p;
        int     i, j = 0;

        for( i = 0; i < DIR_TREE_FILE_NAME_SIZE - 1; i++, j = j + 2 )
        {
            p[0] = pIn[j+1];
            p[1] = pIn[j];
            qChar[i] = *q;

            if( qChar[i] == 0 ) {
                break;
            }
        }

        qChar[DIR_TREE_FILE_NAME_SIZE - 1] = 0;
        readLength = i;
    }
    else
    {
        int i;
        for( i = 0; i < DIR_TREE_FILE_NAME_SIZE - 1; i++ )
        {
            qChar[i] = pIn[i];
            if( qChar[i] == 0 ) {
                break;
            }
        }
        qChar[i] = 0;

        readLength = i;
    }
    MP_LOG << "CATreeItem::charToUnicode(): readLength =" << readLength << LOG_ENDL;
    resultStr.setUnicode( qChar, readLength );

    return resultStr;
}

// { added by kihyung 2012.08.30 for DVD-Audio
void AppMediaPlayer_Controller_Audio::titleMenu()
{
    MP_LOG << LOG_ENDL;
    if(m_pIVideo && m_pIMedia) {
        if(m_pIMedia->GetCurrentMediaState() == PLAYER_PAUSE)
		{
			GetEngineMain().setUnMuteState(true); //added by edo.lee 2013.05.19
			m_pIMedia->Play();
        }
        m_pIVideo->Titlemenu(1);
    }
}

void AppMediaPlayer_Controller_Audio::topMenu()
{
    MP_LOG << LOG_ENDL;
    if(m_pIVideo && m_pIMedia) {
        if(m_pIMedia->GetCurrentMediaState() == PLAYER_PAUSE){
			GetEngineMain().setUnMuteState(true); //added by edo.lee 2013.05.19
			m_pIMedia->Play();
    	}
        m_pIVideo->TopMenu(1);
    }
}

// { modified by kihyung 2013.2.5
void AppMediaPlayer_Controller_Audio::showVideo(DISPLAY_T aDisplay)
{
    MP_LOG << "display =" << aDisplay << LOG_ENDL;

    if((aDisplay != DISPLAY_FRONT) && (aDisplay != DISPLAY_REAR))
    {
        MP_LOG << "Unknown display. Return." << LOG_ENDL;
        return;
    }

    //moveWidget(aDisplay); // removed by kihyung 2013.2.14
    clearDisplay(false); 
}

void AppMediaPlayer_Controller_Audio::hideVideo()
{
    clearDisplay(true);
}

void AppMediaPlayer_Controller_Audio::moveWidget(DISPLAY_T aDisplay)
{
    MP_LOG << LOG_ENDL;
    if((aDisplay != DISPLAY_FRONT) && (aDisplay != DISPLAY_REAR))
    {
        MP_LOG << "Unknown display. Return." << LOG_ENDL;
        return;
    }

    QRect widgetSize(0, 0, 1280, 720);

    if(aDisplay == DISPLAY_REAR)
    {
        //QRectangle magic: when you change the Y coordinate,
        // value of height changes  from 'height' to 'height-Y'
        widgetSize.setHeight( widgetSize.height() + 720 );
        widgetSize.setY(720);
    }

    SetVideoWidgetSize(widgetSize.x(), widgetSize.y(), widgetSize.width(), widgetSize.height());
}

void AppMediaPlayer_Controller_Audio::SetVideoWidgetSize(int ax, int ay, int aw, int ah)
{
    if(m_pVideoWidget)
    {
        MP_LOG << "widget =" << m_pVideoWidget << LOG_ENDL; 
		MP_LOG << "x : " << ax << "y : " << ay << "w : " << aw << "h : " << ah << LOG_ENDL;
        m_pVideoWidget->setGeometry(ax, ay, aw, ah);
    }
    else {
        MP_LOG << "m_pVideoWidget is NULL" << LOG_ENDL;
    }
}

void AppMediaPlayer_Controller_Audio::SetVideoSizeWrtDisplay(int ax, int ay, int aw, int ah)
{
    MP_LOG << LOG_ENDL;
    if(GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->getCurrentScreen() == DISPLAY_FRONT) // modified by Sergey for CR#16815
    {
        SetVideoWidgetSize(ax, ay, aw, ah);
    }
    else
    {
        SetVideoWidgetSize(ax, ay + 720, aw, ah);
    }
}

void AppMediaPlayer_Controller_Audio::clearDisplay(bool isBg)
{
    MP_LOG << "isBg =" << isBg << LOG_ENDL;
    
    if(m_pIMedia)
    {
        m_pIMedia->ClearDisplay(isBg);
    }
}
// } modified by kihyung 2013.2.5

void
AppMediaPlayer_Controller_Audio::requestCurrentFolderName( void )
{
    MP_LOG << LOG_ENDL;
    if (m_pDeckController)
    {
        IAudioInterface* pAudio = m_pDeckController->GetAudioInstance();

        if (pAudio)
        {
            CAStatusResponse tCAStatus;
            int res = pAudio->GetCAStatus( &tCAStatus );
            pAudio->RequestFolderName( tCAStatus.currentFolderNo );
            MP_LOG << res << LOG_ENDL;
        }
    }

}

// } added by jaden1.lee

// { added by junam 2012.8.29 for CR12745
//{modified by aettie.ji 2013.01.31 for ISV 70943
QString AppMediaPlayer_Controller_Audio::getFolderName( QString filePath)
{

   if (filePath.startsWith("file://")){filePath.remove("file://");}
   QString tmpStr = filePath;
   QString folderTmp = "";

   MP_LOG << "aettie check: filePath =" << filePath << LOG_ENDL;

   if(!tmpStr.isEmpty())
   {
       folderTmp = tmpStr.section('/', -2, -2);
   }
//modified by aettie 20130522 folder name for NP
//   if((folderTmp == "Music" && tmpStr.count('/') == 4)||((tmpStr.section('/',-3,-3).contains("usb"))&&tmpStr.count('/') == 4))
   if(  (folderTmp == "Music" && tmpStr.count('/') == 4)||
        (tmpStr.section('/',-3,-3).contains("front_usb"))||
        (tmpStr.section('/',-3,-3).contains("rear_usb"))    )
   {
   //modified by aettie 2013 08 05 for ISV NA 85625
//	folderTmp = "/..";
// modified by aettie 20130812 for ITS 183630
	//folderTmp = "Root";
      folderTmp = "STR_MEDIA_ROOT_FOLDER"; // modified by Dmitry 05.10.13 for ITS0193742
   }
   return folderTmp;
//}modified by aettie.ji 2013.01.31 for ISV 70943
}
// } added by junam

// { modified by kihyung 2013.12.17 for ITS 0214009 
void AppMediaPlayer_Controller_Audio::setSkipTrack()
{
    MP_LOG << LOG_ENDL;
    m_skipTrack = true;
    setIsEnableErrorPopup(false);

    m_pAudioSinkTimer->stop();
    m_pAudioSinkTimer->setInterval(AUDIO_NORMAL_ERROR_INTERVAL);
    m_pAudioSinkTimer->start();
}
// } modified by kihyung 2013.12.17 for ITS 0214009 

// { added by eugeny.novikov 2012.10.25 for CR 14047
void AppMediaPlayer_Controller_Audio::setBasicView(bool bBasicView)
{
    m_bIsBasicView = bBasicView;
    emit isBasicViewChanged();
}
// } added by eugeny.novikov

// { removed by junam 2013.12.10 for code clean
//void AppMediaPlayer_Controller_Audio::setIsLoadingScreen(bool isVisible)
//{
//    Q_UNUSED(isVisible);
//}
// } removed by junam

void AppMediaPlayer_Controller_Audio::sendOpStateForNotifier(int deviceId)
{
    // comment by cychoi 2013.05.26 We can't get proper Source Type using Device Type.
    // Currently, we don't use this function, but we should fix it if we will use this function.
    emit deviceChanged(AudioUtils::DeviceType(MP::DEVICE_T(deviceId)));
}
// } modified by wspark
// } added by wspark

//{ added by yongkyun.lee 2012.12.19 for CR 14219 ,POPUP/list close
void AppMediaPlayer_Controller_Audio::SetAccMode(bool accval)
{
    m_isAccOn = accval;
    return;
}

void AppMediaPlayer_Controller_Audio::SetFrontLcdMode(bool flcdval)
{
    m_isFrontLCD = flcdval;
    return;
}

// { removed by kihyung 2013.1.8
/*
void AppMediaPlayer_Controller_Audio::SetCamMode(bool camval)
{
    m_bCamInFG = camval;
    return;
}
*/
// } removed by kihyung 2013.1.8

bool AppMediaPlayer_Controller_Audio::getAccMode() const
{
     return m_isAccOn;
}

bool AppMediaPlayer_Controller_Audio::getFrontLcdMode() const
{
    return m_isFrontLCD;
}

bool AppMediaPlayer_Controller_Audio::getCamMode()
{
    return GetEngineMain().getCamInFG();
}

//} added by yongkyun.lee

// { added by changjin 2012.12.26 for ISV 61843
bool AppMediaPlayer_Controller_Audio::isSeekableMedia()
{
    if(m_pIMedia)
        return m_pIMedia->SeekableMedia();
    else
        return false;		
}
// } added by changjin 2012.12.26
// { added by wspark 2013.01.23
void AppMediaPlayer_Controller_Audio::restoreLastPlay()
{
	MP_LOG<<"last index : "<<lastPlayedIndex() << LOG_ENDL;
    if(lastPlayedIndex() != -1)
    {
    	//modified by edo.lee 2013.05.07
    	if(isRunFromFileManager())
            playLastSavedFile(lastPlayedIndex(), lastPlayedPos());
        else
            setCurrentIndex(lastPlayedIndex());
        //modified by edo.lee 2013.05.07
    }
    else if(IsPlaying()) //added by junam 2013.10.16 for ITS_KOR_194951
    {
        MP_LOG<<"already playing... no need change track" << LOG_ENDL;
    }
    else
    {
        if (m_isForwardDirection)
        {
            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->NextTrack();
        }
        else
        {
            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->PrevTrack(false);
        }
    }
}

void AppMediaPlayer_Controller_Audio::saveLastPlayedInfo()
{
    //{ modified by yongkyun.lee 2013-11-12 for : ITS 208675
    if(m_pIMedia== NULL)
        return;
    //} modified by yongkyun.lee 2013-11-12 
    setLastPlayedIndex(m_pIMedia->GetCurrentIndex() );
    setLastPlayedPos(m_pIMedia->GetCurrentMediaPosition());
}

void AppMediaPlayer_Controller_Audio::playLastSavedFile(int index, int position)
{
    MP_HIGH << LOG_ENDL;
    
    if(!m_pIMedia)
    {
        MP_LOG << "m_pIMedia is NULL. Return." << LOG_ENDL;
        return;
    }

    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;
	
	CopyHistory(m_history[device], tempHistory); // added by eugene.seo 2013.04.10
    m_history[device].playList.position = position;
    SetPlaylist(device, index);
    setCurrentIndex(index);
}
// } added by wspark

//{ modified by hyochang.ryu 20130611
#define BT_REQ_INFO_COUNT	1

//{ modified by yongkyun.lee 2013-08-28 for : BT OSD sync
void AppMediaPlayer_Controller_Audio::onBtRequestDecCount()
{
    if(m_iTrackInfoReqCount > 0)
        m_iTrackInfoReqCount = m_iTrackInfoReqCount - 1;
    else if(m_iTrackInfoReqCount <= 0)
      m_iTrackInfoReqCount = 0;
}
void AppMediaPlayer_Controller_Audio::onBtRequestIncCount()
{
    m_iTrackInfoReqCount = m_iTrackInfoReqCount + 1;
}

void AppMediaPlayer_Controller_Audio::onBtRequestTimerStart()
{
    onBtRequestTimerStop();
    m_pBtRequestTimer->start();
    
    sendToBTCommand(EVT_BT_MUSIC_GET_INFO );
}

void AppMediaPlayer_Controller_Audio::onBtRequestTimerStop()
{
    if (m_pBtRequestTimer->isActive())
    {
        m_pBtRequestTimer->stop();
    }
}

void AppMediaPlayer_Controller_Audio::onBtRequestTimerExpired()
{
      m_iTrackInfoReqCount = 0;
      m_pBtRequestTimer->stop();
}
//} modified by yongkyun.lee 2013-08-28

// { modified by cychoi 2014.05.12 for duplicated code (added by hyochang.ryu 20130627 for BT delayed play, 20131031 for ITS 205112)
void
AppMediaPlayer_Controller_Audio::onBtDelayedPlayTimerExpired()
{
    if (m_pBtDelayedPlayTimer->isActive())
    {
        m_pBtDelayedPlayTimer->stop();
    }
    MP::DEVICE_T device = getDevice();
    MP_HIGH << device << LOG_ENDL;
    if(device == MP::BLUETOOTH)
    {
        Play();
    }
}
//void
//AppMediaPlayer_Controller_Audio::onBtDelayedPlayTimer1Expired()
//{
//    if (m_pBtDelayedPlayTimer1->isActive())
//    {
//        m_pBtDelayedPlayTimer1->stop();
//    }
//    MP::DEVICE_T device = getDevice();
//    MP_HIGH << device << LOG_ENDL;
//    if(device == MP::BLUETOOTH)
//    {
//        Play();
//    }
//}
// } modified by cychoi 2014.05.12

//{ added by hyochang.ryu 2013913 for ITS189483
void
AppMediaPlayer_Controller_Audio::onBtDelayedMuteTimerExpired()
{
    if (m_pBtDelayedMuteTimer->isActive())
    {
        m_pBtDelayedMuteTimer->stop();
    }
    MP_HIGH << LOG_ENDL;
    GetEngineMain().setUnMuteState(true);
}

void
AppMediaPlayer_Controller_Audio::DelayedUnmute4BT()
{
    MP_HIGH << LOG_ENDL;
    //GetEngineMain().setUnMuteState(false); // commented by cychoi 2014.05.12 for meaningless code 
    m_pBtDelayedMuteTimer->start();
}
//} added by hyochang.ryu 2013913 for ITS189483

//deleted by yongkyun.lee 2013-11-25 for : ITS 210541 rollback

//{ modified by yongkyun.lee 2013-10-10 for :  ITS 191703 BT getinfo 0.5sec
void
AppMediaPlayer_Controller_Audio::onBtResponseTimerStop()
{
    if (m_pBtResponseTimer->isActive())
    {
        m_pBtResponseTimer->stop();
    }
}
//} modified by yongkyun.lee 2013-10-10 

// { added by wspark 2013.02.07 for ISV 72168
void
AppMediaPlayer_Controller_Audio::onBtResponseTimerExpired()
{
    if (m_pBtResponseTimer->isActive())
    {
        m_pBtResponseTimer->stop();
    }
    m_btSending = false;
    onBtRequestTimerStart(); //{ modified by yongkyun.lee 2013-10-10 for : ITS 191703 BT getinfo 0.5sec
}
// } added by wspark

// { added by cychoi 2014.06.03 for Get Info - 1. on TrackChanged 2. delayed 0.5sec from PlayStarted
void
AppMediaPlayer_Controller_Audio::onBtDelayedGetInfoTimerExpired()
{
    MP_LOG << LOG_ENDL;

    MP::DEVICE_T device = getDevice();
    if(device == MP::BLUETOOTH)
    {
        onBtRequestTimerStart();
    }
}

void
AppMediaPlayer_Controller_Audio::onBtDelayedGetInfoTimerStop()
{
    MP_LOG << LOG_ENDL;

    if (m_pBtDelayedGetInfoTimer->isActive())
    {
        m_pBtDelayedGetInfoTimer->stop();
    }
}

void
AppMediaPlayer_Controller_Audio::onBtDelayedGetInfoTimerStart()
{
    MP_LOG << LOG_ENDL;

    if (m_pBtDelayedGetInfoTimer->isActive())
    {
        m_pBtDelayedGetInfoTimer->stop();
    }

    m_pBtDelayedGetInfoTimer->start();
}
// } added by cychoi 2014.06.03

// { added by cychoi 2014.06.05 for Get Info - every 10sec after PlayStarted (some phones don't send TrackChanged)
void
AppMediaPlayer_Controller_Audio::onBtPeriodicGetInfoTimerExpired()
{
    MP_LOG << LOG_ENDL;

    MP::DEVICE_T device = getDevice();
    if(device == MP::BLUETOOTH)
    {
        if(GetEngineMain().getisBTCall() == false)
        {
            sendToBTCommand(EVT_BT_MUSIC_GET_INFO );
        }

        m_pBtPeriodicGetInfoTimer->start();
    }
}

void
AppMediaPlayer_Controller_Audio::onBtPeriodicGetInfoTimerStop()
{
    MP_LOG << LOG_ENDL;

    if (m_pBtPeriodicGetInfoTimer->isActive())
    {
        m_pBtPeriodicGetInfoTimer->stop();
    }
}

void
AppMediaPlayer_Controller_Audio::onBtPeriodicGetInfoTimerStart()
{
    MP_LOG << LOG_ENDL;

    if (m_pBtPeriodicGetInfoTimer->isActive())
    {
        m_pBtPeriodicGetInfoTimer->stop();
    }

    m_pBtPeriodicGetInfoTimer->start();
}
// } added by cychoi 2014.06.05

/*//{ added by cychoi 2013.08.25 for BT Music OSD display. Need more investigation later.
void AppMediaPlayer_Controller_Audio::BTOSDDisplayTimerStart()
{
    if (m_pBTOSDDisplayTimer->isActive())
    {
        m_pBTOSDDisplayTimer->stop();
    }

    m_pBTOSDDisplayTimer->start();
    m_bBTOSDDisplayEnabled = true;
}

void AppMediaPlayer_Controller_Audio::BTOSDDisplayTimerStop()
{
    if (m_pBTOSDDisplayTimer->isActive())
    {
        m_pBTOSDDisplayTimer->stop();
        m_bBTOSDDisplayEnabled = false;
    }
}

void AppMediaPlayer_Controller_Audio::onBTOSDDisplayTimerExpired()
{
    m_bBTOSDDisplayEnabled = false;
}
//} added by cychoi 2013.08.25*/

// { commented by cychoi 2014.05.12 for deprecated code //added by edo.lee 2013.04.21 for Mute
//void
//AppMediaPlayer_Controller_Audio::onMuteTimerExpired()
//{
//    if (m_pMuteTimer->isActive())
//    {
//        m_pMuteTimer->stop();
//    }
    //GetEngineMain().setUnMuteState(true);		//deleted by hyochang.ryu 20130824 for ITS186276 rollback  
//}
// } commented by cychoi 2014.05.12 //added by edo.lee 2013.04.21

void 
AppMediaPlayer_Controller_Audio::onIPodDeviceConnectedSlot(QString strNodePath, QString strSerial, EIPodPorts ePortNum, bool isFirst)
{
    // { modified by sangmin.seol 2014.07.04 Reduce Log.
    MP_HIGH << "NEWIPOD IN strNodePath = " << strNodePath << ", strSerial = " << strSerial << ", ePortNum = " << ePortNum << LOG_ENDL;
    //MP_HIGH << "NEWIPOD IN" << LOG_ENDL;
    //MP_HIGH << "NEWIPOD strNodePath = " << strNodePath << LOG_ENDL;
    //MP_HIGH << "NEWIPOD strSerial   = " << strSerial << LOG_ENDL;
    //MP_HIGH << "NEWIPOD ePortNum    = " << ePortNum << LOG_ENDL;
    // } modified by sangmin.seol 2014.07.04 Reduce Log.

    //{added by junam 2013.07.24 for disable rear iPod port
    MP::DEVICE_T source = GetSignalSource();

    if(source == MP::IPOD1 || source == MP::IPOD2)
    {
        m_history[source].listReqData.tabId = "Song";
        ResetTrackInfo(source);
    }

    if(GetEngineMain().GetVariantRearUSB() == false && source == MP::IPOD2)
    {
        MP_HIGH <<"Rear USB port is off state - ignore" << LOG_ENDL;
        return;
    }
    //}added by junam

    GetEngineMain().IPodDeviceConnected( strNodePath, strSerial, ePortNum, isFirst);

    MP_LOG << "NEWIPOD OUT" << LOG_ENDL;
}

QString AppMediaPlayer_Controller_Audio::getIPODDeviceName(MEDIA_SOURCE_T source)
{
	//2015.01.05 Suryanto Tan: Device name Issue use QString.
    if(source == MEDIA_SOURCE_IPOD1)
    {
        return m_iPod1DeviceName;
    }
    else if(source == MEDIA_SOURCE_IPOD2)
    {
        return m_iPod2DeviceName;
    }
    else
    {
        MP_HIGH << "ERROR: else getIPODDeviceName" << LOG_ENDL;
        QString str("");
        return str;
    }
}

// { modified by kihyung 2013.07.09 for ITS 0177427
void
AppMediaPlayer_Controller_Audio::requestAllPlaylist(int nDevice, bool bRescan, bool bJukeboxCopied)	// modified by sangmin.seol 2013.10.22 JUKEBOX PlayerView Counter Update after copy from USB
{
    MP_LOG << nDevice << bRescan << LOG_ENDL;

    if ( m_pTrackerAbstractor )
    {
        MP_LOG << "request eGetMusicLimitFileListDir_Id" << LOG_ENDL;
        if(bRescan == false) 
            m_pTrackerAbstractor->RequestData(AudioUtils::VolumeType((MP::DEVICE_T)nDevice), eGetMusicLimitFileListDir_Id, REQUID_ALL_FILELIST);
        else if(bJukeboxCopied == true)	// added by sangmin.seol 2013.10.22 JUKEBOX PlayerView Counter Update after copy from USB
            m_pTrackerAbstractor->RequestData(AudioUtils::VolumeType((MP::DEVICE_T)nDevice), eGetMusicLimitFileListDir_Id, REQUID_ALL_FILELIST_RESCAN_COPY);
        else
            m_pTrackerAbstractor->RequestData(AudioUtils::VolumeType((MP::DEVICE_T)nDevice), eGetMusicLimitFileListDir_Id, REQUID_ALL_FILELIST_RESCAN);
    }
    else {
        MP_LOG << "m_pTrackerAbstractor is NULL" << LOG_ENDL;
    }
}
// } modified by kihyung 2013.07.09 for ITS 0177427

// { modified by wspark 2013.04.30 for cluster IOT
/*
// { added by wspark 2013.04.09 for cluster IOT #34
void
AppMediaPlayer_Controller_Audio::resumeTpMessage()
{
    //emit clearTpMessage(); // commented by cychoi 2013.08.17 for No clear TP message from HMC request
    MP_LOG << "wspark title = " << m_history[m_device].Title << LOG_ENDL;
    emit sendTpMessage(m_history[m_device].Title);
}
// } added by wspark
*/
void
AppMediaPlayer_Controller_Audio::resumeTpMessage(QString tp, int deviceId)
{
    MP_LOG << "title = " << tp << " deviceId = " << deviceId << LOG_ENDL;
    // comment by cychoi 2013.05.26 We can't get proper Source Type using Device Type.
    // Currently, we don't use this function, but we should fix it if we will use this function.
    emit sendTpMessage(tp, AudioUtils::DeviceType(MP::DEVICE_T(deviceId)));
}
// } modified by wspark

//{ added by yongkyun.lee 20130413 for : NO CR MP3 List 
void
AppMediaPlayer_Controller_Audio::setMP3ListReadState(bool listRead)
{
    MP_LOG << listRead << LOG_ENDL;
    GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setMP3ListReadState(listRead);
    emit dirTreeFetched(listRead); // added by cychoi 2013.11.18 for ITS 208698 MP3 Play Info on Cluster 
}
//} added by yongkyun.lee 20130413 

// { added by wonseok.heo for ITS 197433 2013.10.28
int AppMediaPlayer_Controller_Audio::GetCDDAGraceNoteStatus(){
    MP_LOG << "CDDA STATUS = "<< m_CDDAGraceNote << LOG_ENDL;
    int cddaStatus = m_CDDAGraceNote;
    return cddaStatus;
}
// } added by wonseok.heo for ITS 197433 2013.10.28

//{ added by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
void AppMediaPlayer_Controller_Audio::SetCDDAGraceNote(GNIndexing val )
{
    m_CDDAGraceNote = val; 
    if(val == GRACENOTE_INDEXING_NO_INFO)
    {
        updateMediaInfo();
    }
}
//} added by yongkyun.lee 20130416

//{ added by junam 2013.04.23 for index DB
void AppMediaPlayer_Controller_Audio::onCurrentTrackCountChanged(int count)
{
    MP::DEVICE_T source = GetSignalSource();
    if(source == MP::IPOD1 || source == MP::IPOD2)
    {
        MP_LOG<<"count = "<<count << LOG_ENDL;
        m_history[source].Quantity = count;
        //{changed by junam 2013.09.03 for ITS_KOR_186711
        //if (source == getDevice())
        if (source == getDevice() && !m_tuneState.m_nTuneStart) //}changed by junam
        {
            emit trackChanged(m_history[source].Index, m_history[source].Quantity);
        }
    }
}
//}added by junam

//{added by junam 2013.05.01 for database update
void AppMediaPlayer_Controller_Audio::onDatabaseUpdate(int count)
{
    MP::DEVICE_T source = GetSignalSource();
    MP_LOG<<" source = "<<source << LOG_ENDL;
    if(source == getDevice())
    {
        //{changed by junam 2013.08.21 for iPod sorting
        getIPodSortingOrder();
        //if(getDevice() == MP::IPOD1 || getDevice() == MP::IPOD2)
        //{
        //    CIpodController *pController = GetIPodController(getDevice());
        //    if(pController)
        //    {
        //        if(pController->getSortingOrder() && m_iPodSortingOrder == 0 )
        //        {
        //            MP_LOG<<" emit iPodSortingOrderChanged()" << LOG_ENDL;
        //            m_iPodSortingOrder = pController->getSortingOrder();
        //            emit iPodSortingOrderChanged();
        //        }
        //        else
        //        {
        //            m_iPodSortingOrder = pController->getSortingOrder();
        //        }
        //    }
        //}
        //}changed by junam

        //{added by junam 2013.05.15 for disbable flow view during re-sync
        if(count == 0 && m_bIsBasicView == false)
        {
            emit showPlayerView();
        }
        //}added by junam

        if( m_pListViewModel && isListView())
        {
            MP_LOG<<" for List count = "<<count << LOG_ENDL;
            m_pListViewModel->resetRequestData(); //added by junam 2013.05.03 for not showing list
            //{changed by junam 2013.05.14 for iPod re-sync
            //m_pListViewModel->onDatabaseUpdate();
            m_pListViewModel->onDatabaseUpdate(count);
            //}changed by junam
        }
        else if(m_bIsBasicView)
        {
            MP_LOG<<" for basic count = "<<count << LOG_ENDL;
        }
        else
        {
            MP_LOG<<" for flow count = "<<count << LOG_ENDL;
            requestCovers();//false); //changed by junam 2013.11.06 for coverflow update
        }
    }

    //{added by junam 2013.05.15 for disbable flow view during re-sync
    if(count == 0 )
    {
        if(source == MP::IPOD1)
            m_bIsDatabase1Ready = false;
        else  if(source == MP::IPOD2)
            m_bIsDatabase2Ready = false;
    }
    //}added by junam
}
//}added by junam

// added by Dmitry 27.04.13
void AppMediaPlayer_Controller_Audio::setIsPlayFromMLT(bool isPlayFromMLT)
{
    Q_UNUSED(isPlayFromMLT);
    // { removed by kihyung 2013.07.19
    /*
    m_isPlayFromMLT = isPlayFromMLT;
    emit isPlayFromMLTChanged();
    */
    // } removed by kihyung 2013.07.19
}
// added by Dmitry 27.04.13

// { added by yungi 2014.02.25 for ITS 195248
bool
AppMediaPlayer_Controller_Audio::isBTStreaming() const
{
    MP_LOG << "BT m_isBTStreaming event:" << m_isBTStreaming << LOG_ENDL;
    return m_isBTStreaming;
}
// } added by yungi 2014.02.25

// { modified by yungi 2014.02.25 for ITS 195248 //{ modified by yongkyun.lee 2013-10-14 for :  ITS 195248
void AppMediaPlayer_Controller_Audio::setIsBTStreaming(bool isBTStreaming)
{
    Q_UNUSED(isBTStreaming);
}
// } modified by yungi 2014.02.25 //} modified by yongkyun.lee 2013-10-14

//{added by aettie 20130605 for Gracenote restart
bool
AppMediaPlayer_Controller_Audio::StartProcessWithQProcess( QString &pPath, QStringList &pFirstArg )
{
    MP_LOG<<"GracenoteKilled StartProcessWithQProcess ---> "<<pPath << LOG_ENDL;

    if ( QProcess::startDetached( pPath, pFirstArg ) )
    {
        MP_LOG<<"GracenoteKilled StartProcessWithQProcess --->   startDetached true" << LOG_ENDL;
        return true;
    }
    MP_LOG<<"GracenoteKilled StartProcessWithQProcess ---> false"<<pPath << LOG_ENDL;
    return false;
}
//}added by aettie 20130605 for Gracenote restart

// { added by wonseok.heo 2013.07.04 disc in out test
void AppMediaPlayer_Controller_Audio::onDiscloadRequest()
{
    GetEngineMain().InOutTestLoad();
}

// { added by kihyung 2013.07.04 for ISV 85455
void AppMediaPlayer_Controller_Audio::onAudioSinkError()
{
    MP_HIGH << "m_nLastErrorCode = " << m_nLastErrorCode <<"   m_scanMode = " <<  m_scanMode << LOG_ENDL;
    
    if(m_pIMedia && getDevice() < MP::ALL_DEVICES)
    {
        // { removed by sangmin.seol 2014.05.27 skip in eMP_ERR_AUTOAUDIOSINK_FAIL & eMP_ERR_RESOURCE
        // { modified by kihyung 2013.12.17 for ITS 0214009
        /*if(m_nLastErrorCode == eMP_ERR_AUTOAUDIOSINK_FAIL)
        {
            GetEngineMain().setUnMuteForSeek(true);
            m_pIMedia->SetCurrentMediaPosition(m_history[getDevice()].Position); // added by kihyung 2013.09.10
            m_pIMedia->Play();
        }
        else
        {*/
        // } removed by sangmin.seol 2014.05.27 skip in eMP_ERR_AUTOAUDIOSINK_FAIL & eMP_ERR_RESOURCE
            bool tmpIsRunFromFM = isRunFromFileManager();
            //{ added by jaehwan.lee 2014.02.27 to fix ITS 227260
            if (m_scanMode == SCANFILE || m_scanMode == SCANFOLDER || m_scanMode == SCANALL  )
            {
                MP_MEDIUM <<"SCANNING.... show current file info. and try scan the next file." << LOG_ENDL; // modified by sangmin.seol 2014.07.07 Reduce Log.
                int idx = m_pIMedia->GetCurrentIndex(); //modified by jaehwan.lee to fix ITS 247239
                RequestTrackInfo(false, idx);
                m_pScanTimer->start();
                m_pAudioSinkTimer->stop();
                return;
            }
            //} added by jaehwan
            if (isForwardDirection())
            {
                GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->NextTrack(true);
            }
            else
            {
                GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->PrevTrack(false);
            }
            setIsRunFromFileManager(tmpIsRunFromFM);
        //} // removed by sangmin.seol 2014.05.27 skip in eMP_ERR_AUTOAUDIOSINK_FAIL & eMP_ERR_RESOURCE
        // } modified by kihyung 2013.12.17 for ITS 0214009
    }

    m_pAudioSinkTimer->stop();
}
// } added by kihyung 2013.07.04 for ISV 85455
void AppMediaPlayer_Controller_Audio::onUserTrackChanged(int trackIndex)
{
	MP_LOG<<"Track index : "<<trackIndex << LOG_ENDL;

    MP::DEVICE_T device = getDevice();
    MP::DEVICE_T source = GetSignalSource();
	if(device != source || device != MP::DISC ||
	   GetEngineMain().GetLastPlayerMode() != APP_MEDIA_MODE_AUDIO) // modified by cychoi 2014.04.21 for ITS 235392
	{
		MP_LOG<<"mode is not matched" << LOG_ENDL;
		return;
	}
	QString song = "";
	if(m_discType == MEDIA_SOURCE_MP3 )
	{
        // removed by cychoi 2014.04.06 for ITS 233500
		/*QList<TuneTrackInfo> trackInfoList = m_pTreeModel->getTrackInfoListFromCATree();
		MP_LOG<<"MP3 " <<trackInfoList.count() << LOG_ENDL;
		 if(trackIndex > -1 && trackInfoList.count() > trackIndex)
	 	{
			 TuneTrackInfo trackInfo = trackInfoList.at(trackIndex);
			 song = trackInfo.name;
	 	}*/
	 	
		if(getTuneTrackNamesFromCATree())
	 	{				
		    if (m_tuneTrackIDs.isEmpty())
		    {
		        if( trackIndex >= 0 && trackIndex < m_tuneTrackNames.size() )
		        {
		            song = m_tuneTrackNames.at(trackIndex);
		        }
		    }
		    else
		    {
		        int idxID=m_tuneTrackIDs.indexOf(trackIndex + 1);
		        if(idxID >= 0 && idxID < m_tuneTrackNames.size())
		        {
		            song = m_tuneTrackNames.at(idxID);
		        }
		    }

            // { modified by cychoi 2014.01.07 for keep MP3 ID3 Tag if same song 
            if(m_nID3TagTrackIdx != trackIndex ||
               (m_eID3TagState != ID3TAG_RECEIVED && m_nID3TagRxCount == 0))
            {
                // { modified by wonseok.heo for ITS 188212 2013.09.04
                ResetTrackInfo(device);//{ modified by yongkyun.lee 2013-09-25 for :NO CR - id3tag is not clear when user track changed 
                if( song.isEmpty()){
                    song = m_pIMedia->GetCurrentFileName();
                    //song = QString("Unknown");
                }else{
                    m_history[device].Title = song;
                }
                // } modified by wonseok.heo for ITS 188212 2013.09.04
            }
            // } modified by cychoi 2014.01.07
            // { added by cychoi 2014.04.08 for ISV 99212
            else
            {
                // use history title (if not empty) if same song
                if(!m_history[device].Title.isEmpty())
                    song = m_history[device].Title;
            }
            // } added by cychoi 2014.04.08
		}
        // { modified by cychoi 2014.03.25 for ITS 231034 MP3 song name on Boot-Up (Agree)
        else
        {
            song = m_history[device].Title;
            if(song.isEmpty())
            {
                song = m_pIMedia->GetCurrentFileName();
                //song = QString("Unknown");
            }
        }
        // } modified by cychoi 2014.03.25
        //m_nPendingMedia = -1; // commented by oseong.kwon 2014.03.03 for ITS 227602 //  modified by yongkyun.lee 2013-08-18 for : NOCR Pending init
	}
    // { modified by cychoi 2013.10.10 for CDDA song name on Boot-Up
    else //if(m_discType == MEDIA_SOURCE_CDDA )
    {
        if (trackIndex > -1 && m_listCDContent.count() > trackIndex )
        {
            MP_LOG<<"CDDA "<< m_listCDContent.count() << LOG_ENDL;
            CD_META_INFO cdInfo = m_listCDContent.at(trackIndex);
            song =  cdInfo.trackTitle;
        }
        else
        {
            // { commented by cychoi 2013.10.10 for ITS 204977
            //if(GetEngineMain().getOnBoot())
            //{
            //    song = m_history[device].Title;
            //}
            //else
            // } commented by cychoi 2013.10.30
            {
                //{ modified by cychoi 2013.10.19 for "Track" display by Language Setting
                song = QString::number(trackIndex + 1); // modified by wonseok.heo for ITS 197433 2013.10.28 //QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(trackIndex + 1);
                //song = QString("Track %1").arg( trackIndex+1);                  
                //} modified by cychoi 2013.10.19
            }
        }
    }
    // } modified by cychoi 2013.10.10

    // { modified by cychoi 2014.04.21 for ITS 235392
    m_autoTrackChangeIndex = -1;
    //onAutoTrackChanged(-1); // added by cychoi 2014.04.06 for ITS 233500
    // } modified by cychoi 2014.04.21
	
    //{ modified by yongkyun.lee 2013-09-23 for : ISV 91169
    if( device == MP::DISC && m_scanMode != SCANOFF )
        return;
    //} modified by yongkyun.lee 2013-09-23
    // { added by wonseok.heo for ITS 197433 2013.10.28
    if(m_discType == MEDIA_SOURCE_CDDA && GetCDDAGraceNote() != GRACENOTE_INDEXING_DONE)
    {
        // { modified by cychoi 2013.11.12 for OSD Title Translation
        // { commented by cychoi 2013.10.10 for ITS 204977
        //if(GetEngineMain().getOnBoot())
        //{
        //    GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
        //                                                                                      song, m_randomPlayMode, m_repeatPlayMode, m_scanMode, false);
        //}else{
        // } commented by cychoi 2013.10.10
            GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
                                                                                              QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(song), m_randomPlayMode, m_repeatPlayMode, m_scanMode, false);
        //} // commented by cychoi 2013.10.10
        // } modified by cychoi 2013.11.12
    }
    else
    {
        GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->setOSDMetaData((OSD_DEVICE)getOSDevice(device),
                                                                                          song, m_randomPlayMode, m_repeatPlayMode, m_scanMode, false); // modified by cychoi 2013.11.12 for OSD Title Translation
    }
    // } added by wonseok.heo for ITS 197433 2013.10.28
    m_isChangedDevice = false; // added by oseong.kwon 2014.02.04 for OSD update on TrackChange
    GetEngineMain().sendVRCancelSignal(); // added by cychoi 2014.03.12 for ITS 224783 VR cancel on onUserTrackChanged
}
void AppMediaPlayer_Controller_Audio::setInOuteStepCnt(int cnt)
{
    m_InOutStepCnt = cnt;
}

void AppMediaPlayer_Controller_Audio::setInOutTestCnt(int cnt)
{
    m_InOutTestCnt = cnt;
}
// } added by wonseok.heo 2013.07.04 disc in out test

int AppMediaPlayer_Controller_Audio::getOSDevice(MP::DEVICE_T device)
{
	OSD_DEVICE returnVal = DEVICE_MAX;
	switch(device)
	{
		case MP::JUKEBOX:
			returnVal = JUKEBOX_MUSIC;
			break;
        case MP::USB1:
        case MP::USB2:
			returnVal = USB_MUSIC;
			break;
        case MP::DISC:
			returnVal = m_discType == MEDIA_SOURCE_CDDA ? DISC_CDDA_MUSIC : DISC_MP3_MUSIC; // modified by cychoi 2013.11.12 for OSD Title Translation
			break;
        case MP::IPOD1:
        case MP::IPOD2:
			returnVal = IPOD_MUSIC;
			break;
        case MP::BLUETOOTH:
			returnVal = BLUETOOTH_MUSIC;
			break;
        case MP::AUX:
			returnVal = AUX_MUSIC;
			break;
        default: // added by kihyung 2013.09.13
            returnVal = DEVICE_MAX;
            break;
	}
	return returnVal;
}

// { modified by kihyung 2013.08.21 for ITS 0185499
bool AppMediaPlayer_Controller_Audio::EmitOnMediaInfoChanged(MP::DEVICE_T device, bool bChangeUnknown, bool bUpdateCoverFlow)
{
    // { added by wonseok.heo for ISV 97055 2014.01.28
    if(device == MP::DISC && m_discType == MEDIA_SOURCE_MP3  && m_eID3TagState == ID3TAG_NONE ){
        return false;
    }
    // } added by wonseok.heo for ISV 97055 2014.01.28
    QString title;
    QString album;
    QString artist;
    QString genre;
    QString cover;
    QString composer;
    QString folder;
    QString file;
    
    if(bChangeUnknown == true) 
    {
        if(device == MP::DISC) {
            title    = m_history[device].Title    == "" ? "Unknown" : m_history[device].Title;
            album    = m_history[device].Album    == "" ? "Unknown" : m_history[device].Album;
            artist   = m_history[device].Artist   == "" ? "Unknown" : m_history[device].Artist;
            genre    = m_history[device].Genre    == "" ? "Unknown" : m_history[device].Genre;
            cover    = m_history[device].Cover;
            composer = m_history[device].Composer == "" ? "Unknown" : m_history[device].Composer;
            folder   = m_history[device].Folder;
            file     = m_history[device].Path;
        }
        else {
            title    = m_history[device].Title    == "" ? "Unknown Title" : m_history[device].Title;
            album    = m_history[device].Album    == "" ? " " : m_history[device].Album;
            artist   = m_history[device].Artist   == "" ? " " : m_history[device].Artist;
            genre    = m_history[device].Genre    == "" ? "Unknown" : m_history[device].Genre;
            cover    = m_history[device].Cover;
            composer = m_history[device].Composer == "" ? "Unknown" : m_history[device].Composer;
            folder   = m_history[device].Folder;
            file     = m_history[device].Path;
        }
    }
    else
    {
        title    = m_history[device].Title;
        album    = m_history[device].Album;
        artist   = m_history[device].Artist;
        genre    = m_history[device].Genre;
        cover    = m_history[device].Cover;
        composer = m_history[device].Composer;
        folder   = m_history[device].Folder;
        file     = m_history[device].Path;
    }
    m_moveTrackIndex = m_history[device].Index; // added by sungha.choi 2013.08.16 for P1, m_moveTrackIndex should same as current media info index.

    DISPLAY_T display = GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->getCurrentScreen();

    if(m_strUpdatedMediaInfo != title + album + artist + file + cover + QString::number(display)) 
    {
        MP_HIGH << device << bChangeUnknown << bUpdateCoverFlow << LOG_ENDL;
        //{ modified by yongkyun.lee 2013-10-18 for : NOCR BT song info
        if(device == MP::BLUETOOTH)
            BTMusicInfoChanged( title,  album,  artist,  0 );
        else             
        //} modified by yongkyun.lee 2013-10-18 
        emit mediaInfoChanged(device, title, album, artist, genre, cover, composer, folder, file);
        // { added by cychoi 2014.05.13 for ITS 237351
        if(device == MP::DISC && m_discType == MEDIA_SOURCE_CDDA && GetCDDAGraceNote() != GRACENOTE_INDEXING_DONE)
        {
            emit mediaInfoClusterChanged(QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_history[device].Title));
        }
        // } added by cychoi 2014.05.13
        
        if(bUpdateCoverFlow == true) 
        {
            emit coverFlowMediaInfoChanged(device, title, album, artist, cover);
        }

        m_strUpdatedMediaInfo = title + album + artist + file + cover + QString::number(display);

        emit currPlayingItemPosChanged(album, artist, file); // added by kihyung 2013.09.13 for ISV 89552

        return true;
    }
    else 
    {
        MP_HIGH << "NOT UPDATED..." << LOG_ENDL;    
        return false;
    }
}
// } modified by kihyung 2013.08.21 for ITS 0185499

// { modified by kihyung 2013.08.27 for ITS 0186708 
bool AppMediaPlayer_Controller_Audio::EmitOnMediaInfoTempChanged(MP::DEVICE_T device, QString title, QString album, QString artist, QString genre, QString cover, QString composer, QString folder, QString file)
{
    if(device >= MP::ALL_DEVICES) return false;
    
    DISPLAY_T display = GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->getCurrentScreen();

    if(m_strUpdatedMediaInfo != title + album + artist + file + cover + QString::number(display)) 
    {
        MP_HIGH << device << LOG_ENDL;
        //{ modified by yongkyun.lee 2013-10-18 for : NOCR BT song info
        if(device == MP::BLUETOOTH)
            BTMusicInfoChanged( title,  album,  artist,  0 );
        else
        //} modified by yongkyun.lee 2013-10-18 
            emit mediaInfoTempChanged(title, album, artist, genre, cover, composer, folder, file);

        m_strUpdatedMediaInfo = title + album + artist + file + cover + QString::number(display);

        return true;
    }
    else 
    {
        MP_HIGH << "NOT UPDATED..." << LOG_ENDL;    
        return false;
    }
}

bool AppMediaPlayer_Controller_Audio::EmitOnTuneSearch(QString coverart, QString artist, QString song, QString album, QString folder, QString countInfo, bool samefile)
{
    MP_LOG << LOG_ENDL;
    
    DISPLAY_T display = GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->getCurrentScreen();

    if(m_strUpdatedMediaInfo != coverart + artist + song + album + folder + countInfo + QString::number(display)) 
    {
        MP_HIGH << LOG_ENDL;
        emit tuneSearch(coverart, artist, song, album, folder, countInfo, samefile);
        m_strUpdatedMediaInfo = coverart + artist + song + album + folder + countInfo + QString::number(display);
        return true;
    }
    else 
    {
        MP_HIGH << "NOT UPDATED..." << LOG_ENDL;    
        return false;
    }
}

bool AppMediaPlayer_Controller_Audio::EmitOnTuneMetaData(QString artist, QString song, QString album, QString folder, bool isColor)
{
    MP_LOG << LOG_ENDL;
    
    DISPLAY_T display = GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->getCurrentScreen();

    //Suryanto Tan. 2014.03.12 ITS 229047
    //When tuning between the song with exact same artist song album folder
    //it should be colored.
    //if(m_strUpdatedMediaInfo != artist + song + album + folder + QString::number(display))
    if(m_strUpdatedMediaInfo != artist + song + album + folder + QString::number(display)||isColor)
    {
        MP_HIGH << LOG_ENDL;
        emit tuneMetaData(artist, song, album, folder, isColor);
        m_strUpdatedMediaInfo = artist + song + album + folder + QString::number(display);
        return true;
    }
    else 
    {
        MP_HIGH << "NOT UPDATED..." << LOG_ENDL;    
        return false;
    }
}

// } modified by kihyung 2013.08.27 for ITS 0186708 

void AppMediaPlayer_Controller_Audio::setstrUpdatedMediaInfo(QString value)    // added by puneet 2013.08.29
{
    DISPLAY_T display = GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->getCurrentScreen();

    m_strUpdatedMediaInfo = (value + QString::number(display));
}

// modified by kihyung 2013.08.21 for ITS 0185499 
bool AppMediaPlayer_Controller_Audio::EmitOnCoverFlowMediaInfoChanged(MP::DEVICE_T device, bool bChangeUnknown, bool bEmpty)
{
    MP_LOG << device << bChangeUnknown << LOG_ENDL;
    
    QString title  = "";
    QString album  = "";
    QString artist = "";
    QString cover  = "";

    if(bEmpty == false) 
    {
        if(bChangeUnknown == true) 
        {
            title    = m_history[device].Title    == "" ? " " : m_history[device].Title;
            album    = m_history[device].Album    == "" ? " " : m_history[device].Album;
            artist   = m_history[device].Artist   == "" ? " " : m_history[device].Artist;
            cover    = m_history[device].Cover;
        }
        else
        {
            title    = m_history[device].Title;
            album    = m_history[device].Album;
            artist   = m_history[device].Artist;
            cover    = m_history[device].Cover;
        }
    }

    emit coverFlowMediaInfoChanged(device, title, album, artist, cover);

    return true;
}

//{added by wonseok.heo 2013.06.12 for factory test
void AppMediaPlayer_Controller_Audio::setTrackChange(int trackNum)
{
    if(getDevice() == MP::DISC)
    {
        if(m_discType == MEDIA_SOURCE_CDDA){

           if(m_history[getDevice()].Quantity < trackNum)
                return;

            DeckEngineerMode  *pDeckEngine = GetEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();
            m_pDeckController = GetControllers().GetDeckController();
            m_pIMedia->SetCurrentMediaPosition(0);
            m_pIMedia->SetCurrentIndex( trackNum );
            pDeckEngine->DiscInOutTest();
            m_history[getDevice()].Index    = trackNum;
            m_history[getDevice()].Position = 0;

        }
    }
}
//}added by wonseok.heo 2013.06.12 for factory test

//{added by junam 2013.07.04 for ITS172937
void AppMediaPlayer_Controller_Audio::SaveAudioData()
{
    AppMediaPlayer_SettingsStorage::GetInstance()->SaveAudioSettings(this);
}
//}added by junam


//{changed by junam 2013.07.12 for music app
bool AppMediaPlayer_Controller_Audio::isControllerDisable(int ctrl, bool bSendOSD)
{
    if(getDevice() == MP::IPOD1 || getDevice() == MP::IPOD2)
    {
        //{changed by junam 2013.08.27
        if(ctrl == MP::CTRL_DISABLE_FLOWVIEW)
            return true;

        //{removed by junam 2013.09.16 for enable iPod playlist
        //if(ctrl == MP::CTRL_DISABLE_LIST_PLAYLIST && QFile::exists("/home/meego/playlist") ==false)
        //    return true;
        //}removed by junam

        if(GetEngineMain().getisBTCall())
        {
            if(bSendOSD)
            {
                if (isForeground()) // added for ITS 196266
                {
                    showBTCallPopup();
                    OSDManager::instance()->displayOSD(OSD_EVENT_IPOD_UNSUPPORTED_ACTION_FG); // added for ITS 219160
                }
                else
                {
                    OSDManager::instance()->displayOSD(OSD_EVENT_IPOD_UNSUPPORTED_ACTION);
                }
            }
            return true;
        }
        else if(m_bBasicControlEnableStatus == false)
        {
            //2014.10.16: iPod 3rd Party app.
            //if((ctrl == MP::CTRL_DISABLE_TUNE)) || ctrl == MP::CTRL_DISABLE_PLAYQUE))
            if(ctrl == MP::CTRL_DISABLE_TUNE)
            {
                if(bSendOSD)
                {
                    OSDManager::instance()->displayOSD(OSD_EVENT_IPOD_UNSUPPORTED_ACTION);
                }
                return true;
            }
            else if(ctrl == MP::CTRL_DISABLE_LIST)
            {
                //2014.10.16: Modified by Tan: 3rd party Music App, should not show list
                return true;
            }
        }
        //}changed by junam

        if(isDatabaseReady() == false)
        {
            if(ctrl == MP::CTRL_DISABLE_FLOWVIEW /* ||  ctrl == MP::CTRL_DISABLE_LIST_PLAYLIST*/)//removed by junam 2013.07.22 for ONDEMAND_PLAYLIST
            {
                MP_LOG<<"DB indexing is not finished" << LOG_ENDL;
                return true;
            }
        }
    }
    //{ added by cychoi 2013.12.29 for ITS 217565
    else if(getDevice() == MP::BLUETOOTH)
    {
        //{ added by 2014.04.01 oseong.kwon for ITS 232921
        if(ctrl == MP::CTRL_DISABLE_MENU || ctrl == MP::CTRL_DISABLE_LIST)
        {
            return false;
        }
        //} added by 2014.04.01 oseong.kwon

        if(GetEngineMain().getisBTCall())
        {
            if(bSendOSD)
            {
                if (isForeground())
                {
                    showBTCallPopup();
                }
                else
                {
                    NotSupportBTOSD();
                }
            }
            return true;
        }

        if(m_bBasicControlEnableStatus == false)
        {
            if((ctrl == MP::CTRL_DISABLE_TUNE || ctrl == MP::CTRL_DISABLE_PLAYQUE))
            {
                if(bSendOSD)
                {
                    NotSupportBTOSD();
                }
                return true;
            }
        }
    }
    //} added by cychoi 2013.12.29
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    else if(getDevice() == MP::DISC)
    {
        return GetEngineMain().IsDeckBlocked();
    }
    // } added by cychoi 2014.04.09
    return false;
}

// added by kihyung 2013.08.21 for ITS 0185499 
void AppMediaPlayer_Controller_Audio::updateCoverFlowMediaInfo(int device, bool bChangeUnknown, bool bEmpty)
{
    MP_LOG << device << LOG_ENDL;
    //{ modified by yongkyun.lee 2013-10-18 for : NOCR BT song info
    if(device == MP::BLUETOOTH)
        return;
    //} modified by yongkyun.lee 2013-10-18 
    EmitOnCoverFlowMediaInfoChanged((MP::DEVICE_T)device, bChangeUnknown, bEmpty);
}

 void AppMediaPlayer_Controller_Audio::onBasicControlEnableStatusChanged(bool enable, QString AppName)
 {
     MP_LOG<<"enable = "<<enable <<" AppName = "<<AppName << LOG_ENDL;
     MP::DEVICE_T source = GetSignalSource();

     GetEngineMain().GetNotifier()->onChangeBasicControlEnable(enable); //HMC 2014.08.20

     if(m_bBasicControlEnableStatus == enable)
         return;

     //Tan: 2014.10.25
     //ITS 0251052, when in FM and receiving this signal,
     //do not set this value so that when reentering iPod
     //and calling setsource it can be set.
     //m_bBasicControlEnableStatus = enable;

     if(source == getDevice())
     {
         m_bBasicControlEnableStatus = enable;
         //emit basicControlEnableStatusChanged(enable, AppName);//moved by junam 2013.07.23 for ITS_KOR_181304
         if(enable == false)
         {
             emit showPlayerView();
         }
         emit basicControlEnableStatusChanged(enable, AppName);//moved by junam 2013.07.23 for ITS_KOR_181304
     }
 }
//}changed by junam
#ifdef SIRI_TIMEOUT_TEST
void AppMediaPlayer_Controller_Audio::onLaunchedDefaultMusicApp(bool bEnable, QString stAppName)
{
    MP_LOG << " bEnable = " << bEnable << ", AppName = " << stAppName << LOG_ENDL;
    MP::DEVICE_T source = GetSignalSource();
    if( source == MP::IPOD1 )
    {
         GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->SendToBTDefaultMusicApp(source, bEnable, stAppName);
    }
    else if( source == MP::IPOD2 )
    {
         GET_MODE_ENGINE(AppMediaPlayer_EngineAudio, APP_MEDIA_MODE_AUDIO)->SendToBTDefaultMusicApp(source, bEnable, stAppName);
    }
}

void AppMediaPlayer_Controller_Audio::onNotifyPlayStateForSiri()
{
    MP_LOG << LOG_ENDL;
    MP::DEVICE_T source = GetSignalSource();

    switch( source )
    {
        case MP::IPOD1 :
            emit iPodPlaying( eUSB_PORT1, "", true );
            break;
        case MP::IPOD2 :
            emit iPodPlaying( eUSB_PORT2, "", true );
            break;
        default :
            MP_LOG << "The source of this event is invalid" << LOG_ENDL;
            break;
    }
}
#endif


//{added by junam 2013.08.21 for iPod sorting
 int AppMediaPlayer_Controller_Audio::getIPodSortingOrder()
 {
     MP::DEVICE_T device = getDevice();
     if(device == MP::IPOD1 || device == MP::IPOD2)
     {
         CIpodController* pController = NULL;
         if (device == MP::IPOD1)
             pController = GetControllers().GetIPod1Controller();
         else if (device == MP::IPOD2)
             pController = GetControllers().GetIPod2Controller();

         if(pController)
         {
             if(m_iPodSortingOrder != pController->getSortingOrder())
             {
                 m_iPodSortingOrder = pController->getSortingOrder();
                 emit iPodSortingOrderChanged();
             }
             CTrackerAbstractor::SetSortingOrder(m_iPodSortingOrder);
             return m_iPodSortingOrder;
         }
     }
     return 0;
 }
//}added by junam

void AppMediaPlayer_Controller_Audio::setJukeBoxCategoryTabAvailable()   // { added by sungha.choi 2013.08.12 for ITS 182674_2nd
{
    if(m_pListViewModel)
        m_pListViewModel->onCategoryTabAvailable(false, false); // modified by sungha.choi 2013.08.12 for ITS 182674_3rd
}   // } added by sungha.choi 2013.08.12 for ITS 182674_2nd

bool AppMediaPlayer_Controller_Audio::isUSBMounted(MP::DEVICE_T device)   // added by sungha.choi 2013.08.12 for ITS 182674_3rd
{
    struct dirent **items = NULL; 
    QString mountPath;
    if(device == MP::USB1)
        mountPath = "/media/front_usb";    // modified by sungha.choi 2013.09.02
    else if(device == MP::USB2)
        mountPath = "/media/rear_usb";    // modified by sungha.choi 2013.09.02
    else
        return false;

    int nitems = scandir(mountPath.toUtf8().data(), &items, NULL, NULL);

    if(nitems >= 0) {   // added by sungha.choi 2013.08.17 for AVP crash (sometimes scandir return -1 (error), some system always return -1 if usb unmount)
        for(int i = 0; i < nitems ; i ++){
            
			if(items[i])  //null check before free  added by edo.lee 2013.11.06 
			{
				free(items[i]);
				items[i] = NULL;  //set null after fress
			}
            
    }
        
		if(items)  //null check before free  added by edo.lee 2013.11.06 
		{
			free(items);
			items = NULL;  //set null after fress
		}
        
    } else {
        MP_LOG<< "scandir return error" << LOG_ENDL;
    }   // this code is refered on Linux man page.

    if(nitems > 2) {
        return true;
    }
    return false;
}

//{changed by junam 2013.10.17 for ITS_KOR_196128
bool AppMediaPlayer_Controller_Audio::IsAlbumSupported(QStringList &filePaths)
{
    MP::DEVICE_T device = getDevice();

    if(device == MP::JUKEBOX || device == MP::USB1 || device == MP::USB2)
    {
        foreach(QString playingFile, filePaths)
        {
            if(playingFile.startsWith("file://"))
                playingFile.remove(0, 7);

            if( !playingFile.isEmpty() && QFile::exists(playingFile) && m_pGPlayerEngine && m_pGPlayerEngine->IsFileSupported(playingFile))
                return true;
        }
    }
    return false;
}
//}changed by junam

bool AppMediaPlayer_Controller_Audio::IsFileSupported(QString filePath)
{
    MP::DEVICE_T device = getDevice();

    if(device == MP::JUKEBOX || device == MP::USB1 || device == MP::USB2)
    {
        if(filePath.startsWith("file://"))
            filePath.remove(0, 7);

        if( !filePath.isEmpty() && QFile::exists(filePath) && m_pGPlayerEngine )
        {
            return m_pGPlayerEngine->IsFileSupported(filePath);
        }
        return false;
    }
    return true;
}

//{added by junam 2013.11.28 for ITS_CHN_211039
bool AppMediaPlayer_Controller_Audio::IsFileSupported(int index)
{
    MP::DEVICE_T device = getDevice();
    if(device == MP::JUKEBOX || device == MP::USB1 || device == MP::USB2)
    {
        if( index < 0 || index >= m_history[device].playList.list->size())
            return false;

        QString filePath = m_history[device].playList.list->at(index);
        if(filePath.startsWith("file://"))
            filePath.remove(0, 7);

        if( !filePath.isEmpty() && QFile::exists(filePath) && m_pGPlayerEngine )
        {
            return m_pGPlayerEngine->IsFileSupported(filePath);
        }
        return false;
    }
    return true;
}
//}added by junam

//{ modified by yongkyun.lee 2013-09-09 for : BT CMD LOG
void AppMediaPlayer_Controller_Audio::sendToBTCommand(int btcmd )
{
    MP_HIGH<<"send to BT =" << btcmd << LOG_ENDL;
    // { modified by oseong.kwon 2014.06.17 for ITS 240595, ITS 240597 // { modified by cychoi 2014.06.03 for Get Info - 1. on TrackChanged 2. delayed 0.5sec from PlayStarted
    //{ modified by yongkyun.lee 2013-10-11 for : ITS 191703 BT getinfo 0.5sec
    if(btcmd == EVT_BT_MUSIC_PLAY)
    {
        onBtRequestTimerStop(); // added by cychoi 2014.12.09 for KH CHN HMC QC - OSD is not updated on TrackChanged
        m_pBtResponseTimer->setInterval(2000);
        m_pBtResponseTimer->start();
    }
    else if(btcmd == EVT_BT_MUSIC_PREV_TRACK ||
            btcmd == EVT_BT_MUSIC_NEXT_TRACK)
    {
        m_strUpdatedMediaInfo = "";
        onBtRequestTimerStop(); // added by cychoi 2014.12.09 for KH CHN HMC QC - OSD is not updated on TrackChanged
        m_pBtResponseTimer->setInterval(2000);
        m_pBtResponseTimer->start();
    }
    //} modified by yongkyun.lee 2013-10-11 
    // } modified by oseong.kwon 2014.06.17 // } modified by cychoi 2014.06.03
    emit CommandOnBT(btcmd );
}
//} modified by yongkyun.lee 2013-09-09 

void AppMediaPlayer_Controller_Audio::onAutoTrackChanged(int index)
{
    MP::DEVICE_T device = getDevice();
    if(device >= MP::ALL_DEVICES) return;

    MP_LOG<<"Track index : "<<index << LOG_ENDL;

    // { added by cychoi 2014.04.21 for ITS 235392
    MP::DEVICE_T source = GetSignalSource();
    if(device != source || GetEngineMain().GetLastPlayerMode() != APP_MEDIA_MODE_AUDIO)
    {
        MP_LOG<<"mode is not matched" << LOG_ENDL;
        return;
    }
    // } added by cychoi 2014.04.21

    m_autoTrackChangeIndex = index;

    // { added by cychoi 2014.04.08 for ISV 99213
    if(device == MP::DISC && m_discType == MEDIA_SOURCE_MP3)
    {
        if(m_autoTrackChangeIndex == -1)
        {
            return;
        }

        if(getTuneTrackNamesFromCATree())
        {
            QString song = "";
            if (m_tuneTrackIDs.isEmpty())
            {
                if( index >= 0 && index < m_tuneTrackNames.size() )
                {
                    song = m_tuneTrackNames.at(index);
                }
            }
            else
            {
                int idxID=m_tuneTrackIDs.indexOf(index + 1);
                if(idxID >= 0 && idxID < m_tuneTrackNames.size())
                {
                    song = m_tuneTrackNames.at(idxID);
                }
            }

            ResetTrackInfo(device);
            if(!song.isEmpty())
            {
                // should update title on AutoTrackChanged
                m_history[device].Title = song;
            }
        }
    }
    // } added by cychoi 2014.04.08
}

// { added by cychoi 2013.10.08 for ITS 190742 CDDA duration
int AppMediaPlayer_Controller_Audio::GetMediaDuration(int device)
{
    int nDuration = 0;

    MP_LOG <<  "Get Media Duration:device=" << device << LOG_ENDL;
    switch (device)
    {
    case MP::JUKEBOX:
    case MP::USB1:
    case MP::USB2:
    case MP::DISC:
    case MP::IPOD1:
    case MP::IPOD2:
        nDuration = m_history[device].Duration;
        if(m_pIMedia && nDuration == 0)
        {
            nDuration = m_pIMedia->GetCurrentMediaDuration();
        }
        break;
    case MP::AUX:
    case MP::BLUETOOTH:
    default:
        MP_LOG << "Warning! Unsupported id" << LOG_ENDL;
        break;
    }

    return nDuration;
}
// } added by cychoi 2013.10.08

// { added by wonseok.heo for ITS 205774 2013.11.01
void AppMediaPlayer_Controller_Audio::catreeRelease()
{
    if (m_pTreeModel  )
    {
        m_pTreeModel->releaseModelData();
    }
}
// } added by wonseok.heo for ITS 205774 2013.11.01

//added by junam 2013.11.16 for coverflow click
 void AppMediaPlayer_Controller_Audio::coverflowItemClick(QString filePath)
 {
     MP_LOG<<"filePath =  "<<filePath << LOG_ENDL;
     m_coverflowClickedItem = filePath;
     m_pTrackerAbstractor->RequestData(AudioUtils::VolumeType((MP::DEVICE_T)getDevice()), eGetMusicLimitFileListDir_Id, REQUID_COVERFLOW_CLICK);
 }
//}added by junam


//{ modified by yongkyun.lee 2013-12-13 for : ITS 213657
void AppMediaPlayer_Controller_Audio::iPodMountedTimer()
{
    iPodMountedTimerStart();
}

void AppMediaPlayer_Controller_Audio::iPodMountedTimerStart()
{
    MP::DEVICE_T device = getDevice();
    // { modified by cychoi 2014.04.03 for ITS 233324
    if(device == MP::BLUETOOTH)
    {
        m_iPodMountTimer->start();
    }
    // } modified by cychoi 2014.04.03
}

void AppMediaPlayer_Controller_Audio::iPodMountedTimerStop()
{
    if (m_iPodMountTimer->isActive())
    {
        m_iPodMountTimer->stop();
    }
}

void AppMediaPlayer_Controller_Audio::oniPodMountTimerExpired()
{
    MP::DEVICE_T device = getDevice();
    MP_HIGH << device << LOG_ENDL;

    // { modified by cychoi 2014.04.03 for ITS 233324
    if(device == MP::BLUETOOTH && m_bBTDelayedPlayOnIPodMount == true)
    {
        Play();
        m_bBTDelayedPlayOnIPodMount = false;
    }
    // } modified by cychoi 2014.04.03
}
//} modified by yongkyun.lee 2013-12-13 
