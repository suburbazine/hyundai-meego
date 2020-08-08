#include <util/DHAVN_AppMediaPlayer_Notifier.h>
#include <DHAVN_MOSTManager_MOSTStructures.h>
#include <DHAVN_AppFramework_AppEngineQML_Def.h>
#include <util/DHAVN_AppMediaPlayer_OSD.h>
#include "engine/DHAVN_AppMediaPlayer_EngineMain.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include <DHAVN_AppFileManager_Shared.h>
#include "util/video/DHAVN_AppMediaPlayer_MainUtils.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"
#include "engine/DHAVN_AppMediaPlayer_EngineAudio.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "DHAVN_StatusControl_Def.h"
#include "controller/DHAVN_AppMediaPlayer_ControllerAudio.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerFS.h"
#include "util/audio/DHAVN_AppMediaPlayer_Utils.h" // added by junam 2012.11.28 for CR14466
#include <QCoreApplication>//added by edo.lee 2013.01.14
#include "util/video/DHAVN_AppMediaPlayer_VideoSettings.h" // modified by ravikanth 27-12-12
#include "DHAVN_DualDisplayHelper.h" // added by kihyung 2013.1.16
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"//added by edo.lee 2013.03.20
#include <QmlStatusBar/osdwriter.h> //modified by edo.lee 2013.04.04
#include "controller/DHAVN_AppMediaPlayer_ControllerPhoto.h"//added by edo.lee 2013.04.11
#include <DHAVN_AudioSourceType.h>
#include <DHAVN_GSTAudioRouterInterface.h>

#define AVP_USE_NONDETAIL_OSD_DISPLAY 0 // added by kihyung 2012.10.21 for R5.20 hotfix    

//added by edo.lee 2013.03.23
#define AVP_ENCODE_OSD_FORMAT_RULE2(strMessage) \
            (strMessage).replace("^", "\\^") \

#define AVP_ENCODE_OSD_FORMAT_RULE1(strMessage) \
            (strMessage).replace("@", "\\@") \
//added by edo.lee 2013.03.23

const wchar_t TUNE_TEXT_COLOR[] = L"#7CBDFF";

// { modified by kihyung 2013.1.9 for ISV 69132
// Shared data provider:
SharedDataProvider *m_pSharedAudioData;
SharedDataProvider *m_pSharedPhotoData;
SharedDataProvider *m_pSharedVideoData;
// } modified by kihyung 2013.1.9 for ISV 69132    

// { added by wspark 2012.09.18 CAN TP Message
const quint16 TPCanID[APP_MEDIA_MODE_MAX][MEDIA_SOURCE_MAX] =
{
    {0x4E2,      // JUKEBOX-music
     0x490,      // USB1-music
     0x490,      // USB2-music
     0xFF,
     0xFF,
     0x48D,      // CD-music
     0x48D,      // CD-DA
     0xFF,       // DVD-A
     0x48F,      // IPOD1
     0x48F,       // IPOD2
     // { added by wspark 2012.12.10 for sending bluetooth opstate
     0xFF,
     0xFF,
     0x485          // BLUETOOTH(TP_HU_HF_CLU)
     // } added by wspark
    },
    {0x4E2,      // JUKEBOX-video
     0x490,      // USB1-video
     0x490,      // USB2-video
     0x48E,      // dvd-v
     0x48D,      // VCD
     0xFF,
     0xFF,
     0xFF,
     0xFF},
    /* APP_MEDIA_MODE_PHOTO */
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};
// } added by wspark

const int clusterStates[APP_MEDIA_MODE_MAX][MEDIA_SOURCE_MAX] =
{
    /* APP_MEDIA_MODE_AUDIO */
//} modify by wonsik 2012.07.10 for CR9361
//    {0x71, 0x64, 0xFF, 0xFF, 0x43, 0x41, 0x51, 0x61, 0x67},
// { modify by wonsik 2012.07.27 for CR9361
// applied CAN DB v0.12.01 data.
//   {0x71,      // JUKEBOX-music
//     0x64,      // USB1-music
//     0x64,      // USB2-music
//     0xFF,
//     0xFF,
//     0x43,      // CD-music
//     0x41,      // CD-DA
//     0x51,      // DVD-A
//     0x61,      // IPOD1
//     0x61       // IPOD2
//    },
    {0x07,      // JUKEBOX-music
     0x65,      // USB1-music
     0x65,      // USB2-music
     0xFF,
     0xFF,
     0x52,      // CD-music
     0x51,      // CD-DA
     0x53,      // DVD-A
     0x61,      // IPOD1
     0x61,       // IPOD2
     // { added by wspark 2012.11.07 for sending aux opstate.
     0x6B,      // AUX1
     0x6B,       // AUX2
     // } added by wspark
     0X08       // BLUETOOTH // added by wspark 2012.12.10 for sending bluetooth opstate.
    },
// } modify by wonsik 2012.07.27 for CR9361
	
//{ modify by wonsik 2012.07.10 for CR9361
    /* APP_MEDIA_MODE_VIDEO */
//{ modify by wonsik 2012.07.10 for CR9361
//    {0x72, 0x65, 0x54, 0x42, 0xFF, 0xFF, 0xFF, 0xFF, 0x68},
// { modify by wonsik 2012.07.27 for CR9361
// applied CAN DB v0.12.01 data.
//    {0x72,      // JUKEBOX-video
//     0x65,      // USB1-video
//     0x65,      // USB2-video
//     0x52,      // dvd-v
//     0x42,      // VCD
//     0xFF,
//     0xFF,
//     0xFF,
//     0x68},
 
    {0x0F,      // JUKEBOX-video
     0x64,      // USB1-video
     0x64,      // USB2-video
     0x54,      // dvd-v
     0x55,      // VCD
     0xFF,
     0xFF,
     0xFF,
     0x68,
     // { added by wspark 2012.11.07 for sending aux opstate.
     0xFF,
     0x6C,
     0x6C
     // } added by wspark

    },
 // } modify by wonsik 2012.07.27 for CR9361
//} modify by wonsik 2012.07.10 for CR9361
    /* APP_MEDIA_MODE_PHOTO */
    {0x73, 0x66, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};

// { modified by eugene.seo 2013.03.26 for display setting
const int brightnessMap[3][21] = 
{
	{		
		    0.00,  3276.75,  6553.50,  9830.25, 13107.00, 
		16383.75, 19660.50, 22937.25, 26214.00, 29490.75,
		32767.50, 
		36044.25, 39321.00, 42597.75, 45874.50,	49151.25, 
		52428.00, 55704.75, 58981.50, 62258.25, 65535.00 
	},
    {		
		    0.00,  2930.00,  5860.00,  8790.00, 11720.00,		
		14650.00, 17580.00, 20510.00, 23440.00, 26370.00,		
		29300.00,		
		32230.00, 35160.00, 38090.00, 41020.00, 43950.00,		
		46880.00, 49810.00, 52740.00, 55670.00, 58600.00 		
    },
	{		
		    0.00,  2930.00,  5860.00,  8790.00, 11720.00,		
		14650.00, 17580.00, 20510.00, 23440.00, 26370.00,		
		29300.00,		
		32230.00, 35160.00, 38090.00, 41020.00, 43950.00,		
		46880.00, 49810.00, 52740.00, 55670.00, 58600.00 		
    }
};

const int contrastMap[3][21] =
{
	{		
		    0.00,  3276.75,  6553.50,  9830.25, 13107.00, 
		16383.75, 19660.50, 22937.25, 26214.00, 29490.75,
		32767.50, 
		36044.25, 39321.00, 42597.75, 45874.50,	49151.25, 
		52428.00, 55704.75, 58981.50, 62258.25, 65535.00 
	},
    {		
		 1065.00,  4288.50,  7512.00,  10735.50, 13959.00,
		17182.50, 20406.00, 23629.50, 26853.00, 30076.50, 
		33300.00, 
		36523.50, 39747.00, 42970.50, 46194.00,	49417.50,
		52641.00, 55864.50, 59088.00, 62311.50, 65535.00		
    },    
	{		         
		   925.00,  4155.50,  7386.00, 10616.50, 13847.00, 
		 17077.50, 20308.00, 23538.50, 26769.00, 29999.50, 
		 33230.00, 
		 36460.50, 39691.00, 42921.50, 46152.00, 49382.50, 
		 52613.00, 55843.50, 59074.00, 62304.50, 65535.00           		 
	}
};

const int tintMap[3][21] =
{
	{		
		    0.00,  3276.75,  6553.50,  9830.25, 13107.00, 
		16383.75, 19660.50, 22937.25, 26214.00, 29490.75,
		32767.50, 
		36044.25, 39321.00, 42597.75, 45874.50,	49151.25, 
		52428.00, 55704.75, 58981.50, 62258.25, 65535.00 
	},
    {			
		 1201.00,  4417.70,  7634.40, 10851.10, 14067.80,
		17284.50, 20501.20, 23717.90, 26934.60, 30151.30,
		33368.00, 
		36584.70, 39801.40, 43018.10, 46234.80, 49451.50, 
		52668.20, 55884.90, 59101.60, 62318.30, 65535.00 
		
    },   
	{	 
		   865.00,  4098.50,  7332.00, 10565.50, 13799.00, 
		 17032.50, 20266.00, 23499.50, 26733.00, 29966.50, 
		 33200.00, 
		 36433.50, 39667.00, 42900.50, 46134.00, 49367.50, 
		 52601.00, 55834.50, 59068.00, 62301.50, 65535.00 		  		 
	}
};
// } modified by eugene.seo 2013.03.26 for display setting

ExternalNotifier::ExternalNotifier( AppMediaPlayer_EngineMain *pEngineMain )
 : m_pVideoCtrl(NULL),
   m_pEngineMain(pEngineMain),
   m_bTuneMode(false),
   m_bFullScreenMode(false),
   m_bPassPlayState(false), // added by sjhyun 2013.09.22 for ITS 190471
   m_bSendingCanProgressMessage(false), // added by wspark 2013.02.19
   m_bBtCallProgressing(false), // added by wspark 2013.03.16
   // { deleted by wspark 2013.04.17
   /*
   m_bAudioTpNeedToSend(false), // added by wspark 2013.03.27 for ISV 77360
   m_bVideoTpNeedToSend(false),  // added by wspark 2013.03.28 for ISV 77376
   */
   // } deleted by wspark
   m_bClusterUpdate(false), // added by wspark 2013.04.10 for cluster update
   m_bFirstUpdateTrack(true), // added by wspark 2013.04.14 for cluster IOT #40
   m_bTpNeedToSend(false), // added by wspark 2013.04.17
   mShmCurrentAVInfo("UISHStatusInformation"), // added by wspark 2013.04.23 for cluster IOT
   m_pIPODAudioRouter(NULL),
   m_gstPipelineDisplay(DISPLAY_NONE), // added by kihyung 2014.01.14 for ITS 0218420
   m_bBasicControlEnable(true)
{
    MP_LOG << LOG_ENDL;

    m_mode = APP_MEDIA_MODE_MAX;
    m_activeMode = APP_MEDIA_MODE_MAX; // added by cychoi 2013.09.03 for cluster update on device changed
    m_modeforPhoto = APP_MEDIA_MODE_MAX; // added by wspark 2013.04.25 for cluster IOT
    m_device = MEDIA_SOURCE_MAX;
	m_activeDevice = MEDIA_SOURCE_MAX; // added by cychoi 2013.09.03 for cluster update on device changed
    m_BackupOSDDevice = MEDIA_SOURCE_MAX; // added by yongkyun.lee 20130626 for : ITS 176114
    m_currentVideoMediaState = APP_STATE_NONE; // modified by ravikanth 27-12-12
    m_displayPlane = OVERLAY_PLANE;
    m_bTuneSameFile = false;// added by yongkyun.lee  2012.10.17  for Current file is white color
    ResetModeData();
    InitilizeDisplayParam(); // modified by ravikanth 27-12-12

    m_pEngines[APP_MEDIA_MODE_AUDIO] = m_pEngineMain->GetEngine(APP_MEDIA_MODE_AUDIO);
    m_pEngines[APP_MEDIA_MODE_VIDEO] = m_pEngineMain->GetEngine(APP_MEDIA_MODE_VIDEO);
    m_pEngines[APP_MEDIA_MODE_PHOTO_FRONT] = m_pEngineMain->GetEngine(APP_MEDIA_MODE_PHOTO_FRONT);
    m_pEngines[APP_MEDIA_MODE_PHOTO_REAR] = m_pEngineMain->GetEngine(APP_MEDIA_MODE_PHOTO_REAR);

    m_bAVOffMode = false; //added by changjin 2012.10.04 for SANITY_CM_AJ338 : false is AV On
    // { removed by junam 2012.11.28 for CR14466
    //m_bDelayOSDUpdate = false; //added by changjin 2012.10.15 for SANITY_CM_AG056
    //m_bCancelUpdateOSD = false; //added by changjin 2012.10.22 for CR 14445
    // } removed by junam
    //m_OSDDisplay = false; //added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD  
    //m_OSDTreckSpeed = false; // added by yungi 2012.11.14 for ISV 62667,ISV 62596
    //m_bAuxForBoot = true; // added by lssanh 2013.01.26 for ISV68519
	m_isScanMode = false;//added by edo.lee 2013.04.05 
    m_CDDAMataInfo = false;// added by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
    m_autoTrackChangeIndex = -1;

    m_bDirTreeFetched = false; // added by cychoi 2013.11.18 for ITS 208698 MP3 Play Info on Cluster 

    //m_pOSDView = new QDeclarativeView(); //delete by ys 2013.09.30 //added by edo.lee 2012.12.04 for new statusbar

    for (int n = 0; n < APP_MEDIA_MODE_MAX; n++)
    {
        if (m_pEngines[n] != NULL)
        {
            connect(m_pEngines[n]->GetController(), SIGNAL(deviceChanged(int)), this, SLOT(onDeviceChanged(int)));
            connect(m_pEngines[n]->GetController(), SIGNAL(stateChanged(int)), this, SLOT(onStateChanged(int)));
            connect(m_pEngines[n]->GetController(), SIGNAL(positionChanged(int)), this, SLOT(onPositionChanged(int)));
            // { modified by wspark 2013.04.09
            //connect(m_pEngines[n]->GetController(), SIGNAL(positionCleared()), this, SLOT(onPositionCleared())); // added by wspark 2013.03.30 for ISV 77359
            connect(m_pEngines[n]->GetController(), SIGNAL(positionCleared(bool)), this, SLOT(onPositionCleared(bool)));
            // } modified by wspark
            connect(m_pEngines[n]->GetController(), SIGNAL(durationChanged(int)), this, SLOT(onDurationChanged(int)));
            connect(m_pEngines[n]->GetController(), SIGNAL(trackChanged(int,int)), this, SLOT(onTrackChanged(int,int)));
            connect(m_pEngines[n]->GetController(), SIGNAL(trackClusterChanged(int,int,bool)), this, SLOT(onTrackClusterChanged(int,int,bool))); // added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
            connect(m_pEngines[n]->GetController(), SIGNAL(mediaInfoChanged(int, QString, QString, QString, QString, QString, QString, QString, QString)),
                                              this, SLOT( onMediaInfoChanged(int, QString, QString, QString, QString, QString, QString, QString, QString))); // modified  by kihyung 2013.08.08 for ITS 0183028
            connect(m_pEngines[n]->GetController(), SIGNAL(mediaInfoClusterChanged(QString)),
                                              this, SLOT( onMediaInfoClusterChanged(QString))); // added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
            connect(m_pEngines[n]->GetController(), SIGNAL(updateOSD(unsigned int)), this, SLOT(displayOSD(unsigned int)));
            // { removed by junam 2012.11.28 for CR14466
            //connect(m_pEngines[n]->GetController(), SIGNAL(cancelUpdateOSD(bool)), this, SLOT(setCancelUpdateOSD(bool))); //added by changjin 2012.10.22 for CR 14445
            // } removed by junam
            connect(m_pEngines[n]->GetController(), SIGNAL(contentSize(MEDIA_SOURCE_T,MEDIA_TYPE_T,int)),
                        this, SLOT(onContentSize(MEDIA_SOURCE_T,MEDIA_TYPE_T,int)));
            connect(m_pEngines[n]->GetController(),
                    SIGNAL(playingModeChanged(MEDIA_MODE)),
                    this,
                    SLOT(onPlayingModeChanged(MEDIA_MODE)));
            //{ modified by yongkyun.lee@lge.com 2012.07.14  CR 11922
            //connect(m_pEngines[n]->GetController(), SIGNAL( playingModeChanged( int, int, int )),
            //       this, SLOT(onPlayingModeChanged( int, int, int )));
             connect(m_pEngines[n]->GetController(), SIGNAL( playingModeChanged( int, int, int, bool )),
                    this, SLOT(onPlayingModeChanged( int, int, int, bool )));
            // } modified by yongkyun.lee@lge.com


             //{changed by junam 2013.03.26 for ISV72425
             //connect(m_pEngines[n]->GetController(), SIGNAL( tuneSearch( QString, QString, QString, QString, QString, QString, QString, QString, bool )),
             //        this, SLOT( onTuneSearchChanged( QString, QString, QString, QString, QString, QString, QString, QString ,bool )));
             connect(m_pEngines[n]->GetController(), SIGNAL( tuneSearch( QString, QString, QString, QString, QString, QString, bool )),
                     this, SLOT( onTuneSearchChanged( QString, QString, QString, QString, QString, QString ,bool )));
             //}changed by junam

            // { added by junam 2012.11.14 for tune update osd
            if( APP_MEDIA_MODE_AUDIO == n ) // added by junam 2012.11.28 for CR14466
            {
                connect(m_pEngines[n]->GetController(), SIGNAL( tuneCounter( QString)),this, SLOT( onTuneCounterChanged( QString)));
                //{changed by junam 2013.03.26 for ISV72425
                //connect(m_pEngines[n]->GetController(), SIGNAL( tuneMetaData( QString, QString, QString, QString, QString, QString, bool )),
                //        this, SLOT( onTuneMetaDataChanged( QString, QString, QString, QString, QString, QString, bool )));
                connect(m_pEngines[n]->GetController(), SIGNAL( tuneMetaData( QString, QString, QString, QString, bool )),
                        this, SLOT( onTuneMetaDataChanged( QString, QString, QString, QString, bool )));
                //}changed by junam
                connect(m_pEngines[n]->GetController(), SIGNAL( dirTreeFetched( bool)),this, SLOT( onDirTreeFetched( bool))); // added by cychoi 2013.11.18 for ITS 208698 MP3 Play Info on Cluster 
            }
            // } added by junam

            connect(m_pEngines[n]->GetController(), SIGNAL(tuneSameFileNoti(bool)), this, SLOT(onTuneSameFile(bool))); 
            connect(m_pEngines[n], SIGNAL(tuneSameFileNoti(bool)), this, SLOT(onTuneSameFile(bool)));
            connect(m_pEngines[n], SIGNAL(setDisplayOSD(bool)), this, SLOT(onSetDisplayOSD(bool)));//added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD  
            //} modified  by yongkyun.lee 
            // } add by junggil
            connect(m_pEngines[n], SIGNAL(updateOSD(unsigned int)), this, SLOT(displayOSD(unsigned int)));
            // { removed by junam 2012.11.28 for CR14466
            //connect(m_pEngines[n], SIGNAL(cancelUpdateOSD(bool)), this, SLOT(setCancelUpdateOSD(bool))); //added by changjin 2012.10.22 for CR 14445
            // } removed by junam
            connect(m_pEngines[n]->GetController(), SIGNAL(moreLikeThisCluster()), this, SLOT(onMoreLikeThisCluster()));
			//added by eunkoo 2012.07.26 for CR11898
			connect(m_pEngines[n]->GetController(), SIGNAL(btMusicInfoChanged(QString, QString, QString, QString)), this, SLOT(onBTMusicInfoChanged(QString, QString, QString, QString)));
			//added by eunkoo 
            // connect(m_pEngines[n]->GetController(), SIGNAL(updateDvdTrackInfo(int)), this, SLOT(onUpdateDvdTrackInfo(int))); // modified by ravikanth - 12-09-25 - cr 13789 // remvoed by kihyung 2012.11.26. removed spec.
            // { modified by wspark 2013.03.27 for ISV 77371
            //connect(m_pEngines[n]->GetController(), SIGNAL(updateMP3CDTrackInfo(int)), this, SLOT(onUpdateMP3CDTrackInfo(int)));  // added by wspark 2012.11.07 for updating mp3cd track info correctly.
            connect(m_pEngines[n]->GetController(), SIGNAL(updateTrackInfo(int, QString)), this, SLOT(onUpdateTrackInfo(int, QString))); // modified by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
            // } modified by wspark
            //connect(m_pEngines[n]->GetController(), SIGNAL(auxAudioActivated()), this, SLOT(onAuxAudioActivated())); // deleted by wspark 2012.12.10 for sending bluetooth opstate
            connect(m_pEngines[n]->GetController(), SIGNAL(UpdateStateTrackOSD(int)), this, SLOT(onUpdateStateTrackOSD(int)));// added by yungi 2012.11.14 for ISV 62667,ISV 62596
            connect(m_pEngines[n]->GetController(), SIGNAL(clearTpMessage()), this, SLOT(onClearTpMessage())); // added by wspark 2013.04.09 for cluster IOT #37
            // { modified by wspark 2013.04.30 for cluster IOT
            //connect(m_pEngines[n]->GetController(), SIGNAL(sendTpMessage(QString)), this, SLOT(onSendTpMessage(QString))); // added by wspark 2013.04.09 for cluster IOT #34
            connect(m_pEngines[n]->GetController(), SIGNAL(sendTpMessage(QString, int)), this, SLOT(onSendTpMessage(QString, int)));
            // } modified by wspark
            connect(m_pEngines[n]->GetController(), SIGNAL(playListChanged()), this, SLOT(onPlayListChanged())); // added by wspark 2013.04.21 for cluster IOT


        }

        // { modified by Sergey Vetugov 2012.08.01 CR#12651
        //m_updatePositionTimer.setSingleShot(true);		
        // } modified by Sergey Vetugov
    }

    // Connect to video stuff.
    AppMediaPlayer_EngineVideo* pVideoEngine =
            dynamic_cast<AppMediaPlayer_EngineVideo*>(m_pEngines[APP_MEDIA_MODE_VIDEO]);

    //bool bIsContentExist = FALSE;
	// { modified by eugene 2012.08.22 for static analysis defect #6903
	if (pVideoEngine)
	{
	    m_pVideoCtrl = pVideoEngine->GetVideoController();

	    connect(pVideoEngine, SIGNAL(setTuneMode(bool)), this, SLOT(setTuneMode(bool)));
	    connect(pVideoEngine, SIGNAL(setFullScreenMode(bool)), this, SLOT(setFullScreenMode(bool)));
	    connect(pVideoEngine, SIGNAL(avModeChanged(MEDIA_SOURCE_T, bool)), this, SLOT(onAvModeChanged(MEDIA_SOURCE_T, bool)));
        connect(pVideoEngine, SIGNAL(setInitDispValue()), this, SLOT(onSetInitialDisplayValue()));//added by shkim for ITS 189354 Display Setting value Issue
        connect(pVideoEngine, SIGNAL(setPassPlayState(bool)), this, SLOT(onSetPassPlayState(bool))); // added by sjhyun 2013.09.22 for ITS 190471
	}
	// } modified by eugene 2012.08.22 for static analysis defect #6903

    connect(m_pVideoCtrl, SIGNAL(filenameChanged(QString)), this, SLOT(onVideoFilenameChanged(QString)));
    connect(m_pVideoCtrl, SIGNAL(foldernameChanged(QString)), this, SLOT(onFoldernameChanged(QString)));
    connect(m_pVideoCtrl, SIGNAL(fullfoldernameChanged(QString)), this, SLOT(onVideoFullFolderNameChanged(QString)));
    connect(m_pVideoCtrl, SIGNAL(playlistLengthChanged(int)), this, SLOT(onPlaylistLenghtChanged(int)));
    connect(m_pVideoCtrl, SIGNAL(playlistPositionChanged(int)), this, SLOT(onPlaylistPositionChanged(int)));
    // { added by Sergey Vetugov for CR#13601
    // connect(m_pVideoCtrl, SIGNAL(dvdScreenChanged(int)), this, SLOT(onDVDScreenChanged(int))); // remvoed by kihyung 2012.11.26. removed spec.
    // } added by Sergey Vetugov for CR#13601
	// { added by ravikanth - 12-09-19
	// { modified by ravikanth 27-12-12
    connect(m_pVideoCtrl, SIGNAL(brightnessChanged(int)), this, SLOT(onBrightnessChanged(int)));
    connect(m_pVideoCtrl, SIGNAL(saturationChanged(int)), this, SLOT(onTintChanged(int)));
    connect(m_pVideoCtrl, SIGNAL(hueChanged(int)), this, SLOT(onHueChanged(int)));
    connect(m_pVideoCtrl, SIGNAL(contrastChanged(int)), this, SLOT(onContrastChanged(int)));
	// } modified by ravikanth 27-12-12
    // removed by Sergey for CR#15771
    // { modified by wspark 2013.04.10 for cluster update
    /*
    // { added by wspark 2013.03.16
    AppMediaPlayer_EngineAudio* pAudioEngine =
            dynamic_cast<AppMediaPlayer_EngineAudio*>(m_pEngines[APP_MEDIA_MODE_AUDIO]);
    if(pAudioEngine)
    {
        connect(pAudioEngine, SIGNAL(notifyBtCallProgressing(bool)), this, SLOT(onNotifyBtCallProgressing(bool)));
    }
    // } added by wspark
    */
    connect(m_pEngineMain, SIGNAL(notifyBtCallProgressing(bool)), this, SLOT(onNotifyBtCallProgressing(bool)));
    // } modified by wspark

    connect(m_pEngineMain, SIGNAL(sendTpMessage()), this, SLOT(onSendTpMessage()));  // added by wspark 2013.04.15 for EVT_UPDATE_CLUSTER

    connect(m_pEngineMain, SIGNAL(retranslateUi(QVariant)), this, SLOT(onRetranslateUI())); // added by cychoi 2013.09.03 for ITS 187585

    // Media Manager:
    m_pMediaManager = m_pEngineMain->GetMediaManager();
    connect(m_pMediaManager, SIGNAL(deviceAttached(MEDIA_SOURCE_T, MEDIA_TYPE_T, bool)), this, SLOT(onDeviceAttached(MEDIA_SOURCE_T,MEDIA_TYPE_T,bool)));
    connect(m_pMediaManager, SIGNAL(deviceRemoved(MEDIA_SOURCE_T, MEDIA_TYPE_T)), this, SLOT(onDeviceRemoved(MEDIA_SOURCE_T,MEDIA_TYPE_T)));
    connect(m_pMediaManager, SIGNAL(discLoading()), this, SLOT(onDiscLoading()));
    connect(m_pMediaManager, SIGNAL(notifyIPodReady(MEDIA_SOURCE_T)), this, SLOT(onNotifyIPodReady(MEDIA_SOURCE_T)));
    // connect(m_pMediaManager, SIGNAL(sourceAvailableChanged(MEDIA_SOURCE_T,MEDIA_TYPE_T,bool)), this, SLOT(onSourceAvailableChanged(MEDIA_SOURCE_T,MEDIA_TYPE_T,bool))); // added by Sergey for #16250 // removed by kihyung 2013.2.17

    // Cluster:
    m_pQCanController = new CQCANController_ApplnTxRx(this);
    m_pQCanController->Init();

    // { changed by junam 2012.10.24 for crash protection.
    //m_pCanTPInterface = new QDBusInterface("com.lge.car.micom", "/Dtc", "com.lge.car.micom.dtc", QDBusConnection::sessionBus()); // added by wspark 2012.09.18 CAN TP Message
    if (QDBusConnection::sessionBus().interface()->isServiceRegistered("com.lge.car.micom"))
    {
        m_pCanTPInterface = new QDBusInterface("com.lge.car.micom", "/Dtc", "com.lge.car.micom.dtc", QDBusConnection::sessionBus());
    }
    else
    {
        m_pCanTPInterface = NULL;
        MP_LOG << "Service com.lge.car.micom is not registered" << LOG_ENDL;
    }
    // } changed by junam

    // IBox:
    // { changed by junam 2012.11.26 for CR15923
    //m_pMostJukeboxAdaptor = new QDBusInterface("com.dhavn.mostmanager.qtdbus", "/jukebox", "", QDBusConnection::sessionBus());
    //m_pMostDiscAdaptor = new QDBusInterface("com.dhavn.mostmanager.qtdbus", "/dvdplayer", "", QDBusConnection::sessionBus());
    //m_pMostAuxInAdaptor = new QDBusInterface("com.dhavn.mostmanager.qtdbus", "/auxin", "", QDBusConnection::sessionBus());	
    m_pMostJukeboxAdaptor = NULL;
    m_pMostDiscAdaptor = NULL;
    m_pMostAuxInAdaptor = NULL;
    // } changed by junam
    
	// { added by edo.lee 2013.01.23
	resetMediaEnabled(MEDIA_SOURCE_JUKEBOX);
	resetMediaEnabled(MEDIA_SOURCE_USB1);
	resetMediaEnabled(MEDIA_SOURCE_USB2	);
	resetMediaEnabled(MEDIA_SOURCE_IPOD1);
	resetMediaEnabled(MEDIA_SOURCE_IPOD2);	
	// } added by edo.lee
	
    //m_pMainUtil = AppMediaPlayer_MainUtils::GetInstance(); //ys-20131007 is not used

    // { removed by kihyung 2013.2.17
    /*
    onContentSize(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_VIDEO,
                  (int)m_pMediaManager->IsSourceAvailable(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_VIDEO)); // added by Sergey for #16250

    int nPhotoCount = static_cast <AppMediaPlayer_Controller_Photo*> (m_pEngines[APP_MEDIA_MODE_PHOTO_FRONT]->GetController())->GetCurrentPlayListCount();
    onContentSize(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_PHOTO, nPhotoCount);
    */
    // } removed by kihyung 2013.2.17
    
    m_pDDHelper = new DualDisplayHelper(); // added by ravikanth 27-12-12
    // { added by wspark 2013.04.23 for cluster IOT
    bool ret = mShmCurrentAVInfo.attach();
    if(ret == false)
    {
        MP_HIGH << QString( "Can't attach to the shared memroy -> error code : %1" ).arg( mShmCurrentAVInfo.error() ) << LOG_ENDL;
    }
    // } added by wspark
}
ExternalNotifier::~ExternalNotifier()
{
    AppMediaPlayer_VideoSettings::FreeInstance(); // added by ravikanth 27-12-12
    for (int n = 0; n < APP_MEDIA_MODE_MAX; n++)
    {
        disconnect(m_pEngines[n]->GetController(), 0,0,0);
        m_pEngines[n] = NULL;
    }
    delete m_pDDHelper; // added by ravikanth 27-12-12
    delete m_pQCanController;
    delete m_pCanTPInterface; // added by wspark 2012.09.18 CAN TP Message
    if(m_pMostJukeboxAdaptor) // added by junam 2012.11.26 for CR15923
        delete m_pMostJukeboxAdaptor;
    if(m_pMostDiscAdaptor) // added by junam 2012.11.26 for CR15923
        delete m_pMostDiscAdaptor;
    if(m_pMostAuxInAdaptor) // added by junam 2012.11.26 for CR15923
        delete m_pMostAuxInAdaptor;
    AppMediaPlayer_MainUtils::FreeInstance();
}
// { added by junam 2012.11.26 for CR15923
QDBusInterface * ExternalNotifier::getMostJukeboxAdaptor()
{
    if ( m_pMostJukeboxAdaptor == NULL &&
            QDBusConnection::sessionBus().interface()->isServiceRegistered("com.dhavn.mostmanager.qtdbus"))
        m_pMostJukeboxAdaptor = new QDBusInterface("com.dhavn.mostmanager.qtdbus", "/jukebox", "", QDBusConnection::sessionBus());
    return m_pMostJukeboxAdaptor;
}

QDBusInterface * ExternalNotifier::getMostDiscAdaptor()
{
    if ( m_pMostDiscAdaptor == NULL &&
            QDBusConnection::sessionBus().interface()->isServiceRegistered("com.dhavn.mostmanager.qtdbus"))
        m_pMostDiscAdaptor = new QDBusInterface("com.dhavn.mostmanager.qtdbus", "/dvdplayer", "", QDBusConnection::sessionBus());
    return m_pMostDiscAdaptor;
}

QDBusInterface * ExternalNotifier::getMostAuxInAdaptor()
{
    if ( m_pMostAuxInAdaptor == NULL &&
            QDBusConnection::sessionBus().interface()->isServiceRegistered("com.dhavn.mostmanager.qtdbus"))
        m_pMostAuxInAdaptor = new QDBusInterface("com.dhavn.mostmanager.qtdbus", "/auxin", "", QDBusConnection::sessionBus());
    return m_pMostAuxInAdaptor;
}
// } added by junam

GSTAudioRouterDBus* ExternalNotifier::getGSTAudioRouterDBus()
{
    if(m_pIPODAudioRouter == NULL)
    {
        m_pIPODAudioRouter = new GSTAudioRouterDBus("org.lge.GSTAudioRouterDBus", "/", QDBusConnection::sessionBus(), 0);
    }
    return m_pIPODAudioRouter;
}

//added by edo.lee 2013.05.16
void ExternalNotifier::resetScanMode(MEDIA_SOURCE_T device)
{
    // { modified by kihyung 2013.07.17 for ISV NA 86690
    if ( device < MEDIA_SOURCE_MAX ) 
    {
        //{ modified by yongkyun.lee 2013-07-15 for : ITS 179776
    	MP_LOG<<device<< m_devices[device].m_playingScanMode << LOG_ENDL;
    	if(m_devices[device].m_playingScanMode <= SCANFOLDER || m_devices[device].m_playingScanMode == SCANALL)
    		m_devices[device].m_playingScanMode = SCANOFF;
        //} modified by yongkyun.lee 2013-07-15 
    }
    // } modified by kihyung 2013.07.17 for ISV NA 86690
}
void ExternalNotifier::ResetModeData()
{
    for (int device = 0; device < MEDIA_SOURCE_MAX; device++)
    {
        ResetDeviceData((MEDIA_SOURCE_T)device);
    }
}

void ExternalNotifier::ResetDeviceData(MEDIA_SOURCE_T device)
{
    m_bSendingCanProgressMessage = false; // added by wspark 2013.02.19 for sending valid track number in CAN message.
    if ( device < MEDIA_SOURCE_MAX )
    {
        m_devices[device].m_state = PLAYER_STOP;
        m_devices[device].m_number = 0;
        m_devices[device].m_quantity = 0;
        m_devices[device].m_videoTpMessage = ""; // added by wspark 2013.03.28 for ISV 77376
        // removed by Sergey for CR#15771
        ResetTrackData(device);
    }
}

void ExternalNotifier::ResetTrackData(MEDIA_SOURCE_T device)
{
    if ( device < MEDIA_SOURCE_MAX )
    {
        m_devices[device].m_position = 0;
        m_devices[device].m_duration = 0;
        m_devices[device].m_title = "";
        m_devices[device].m_album = "";
        m_devices[device].m_artist = "";

        //{ modified by yongkyun.lee 2013-07-15 for : ITS 179776
        m_devices[device].m_playingScanMode = 0;
        m_devices[device].m_playingRandomMode = 0;
        m_devices[device].m_playingRepeatMode = 0;
        //} modified by yongkyun.lee 2013-07-15 
        // removed by Sergey for CR#15771
    }
}

void ExternalNotifier::SetActiveMode(APP_MEDIA_MODE_T mode)
{
    MP_LOG << mode << LOG_ENDL;
    //We should not call Reset here because of there are situations when 
    //SetActiveMode() is called after MediaInfoChanged()
    //ResetModeData();
    // { modified by wspark 2013.04.25 for cluster IOT
    //m_mode = mode;
    m_modeforPhoto = mode;
    if(mode == APP_MEDIA_MODE_AUDIO || mode == APP_MEDIA_MODE_VIDEO)
    {
        // { commented by cychoi 2013.06.24 for cluster IOT
        // { added by wspark 2013.04.28 for cluster IOT
        //if(m_mode != mode)
        //{
        //    sendCanTpMessage(TPCanID[m_mode][m_device], "");
        //}
        // } added by wspark
        // } commented by cychoi 2013.06.24
        m_mode = mode;
    }
    // } modified by wspark
}

// { added by eugene.seo 2013.02.22
MEDIA_TYPE_T ExternalNotifier::GetMediaType(APP_MEDIA_MODE_T mode) 
{
    MP_LOG << mode << LOG_ENDL;

    switch(mode)
    {
    	case APP_MEDIA_MODE_AUDIO:
			return MEDIA_TYPE_MUSIC;
		case APP_MEDIA_MODE_VIDEO:
			return MEDIA_TYPE_VIDEO;
		case APP_MEDIA_MODE_PHOTO_FRONT:
		case APP_MEDIA_MODE_PHOTO_REAR:
			return MEDIA_TYPE_PHOTO;			
		default:
			return MEDIA_TYPE_MAX;
    }
}
// } added by eugene.seo 2013.02.22

// { added by lssanh 2013.02.07 2USB
void ExternalNotifier::cancelFileOperation()
{
   MP_LOG << LOG_ENDL;
   // removed by ravikanth 16-04-13
//   static_cast <AppMediaPlayer_Controller_Audio*> (m_pEngines[APP_MEDIA_MODE_AUDIO]->GetController())->GetListViewModel()->cancelOperationChangeSource();
//   Event fmstate( EVT_POST_EVENT, m_pEngineMain->GetThisAppId(), APP_FMGR, EVT_FILEOPERATION_STOP);
//   m_pEngineMain->notifyUISH( fmstate );
}
// } added by lssanh 2013.02.07 2USB

// { added by changjin 2012.10.04 for SANITY_CM_AJ338
APP_MEDIA_MODE_T ExternalNotifier::GetActiveMode(void)
{
    // { modified by wspark 2013.04.25 for cluster IOT
    //return m_mode;
    return m_modeforPhoto;
    // } modified by wspark
}
// } added by changjin 2012.10.04

void ExternalNotifier::onDeviceChanged(int device)
{
    MP_MEDIUM << "device=" << device << "m_mode=" << m_mode << "m_device=" << m_device << LOG_ENDL;

    //{ modified by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
    if ( (m_device != device || m_activeDevice != device || m_activeMode != m_mode) && // modified by cychoi 2013.09.03 for cluster update on device changed
         device < MEDIA_SOURCE_MAX )
    {
        //sendCanTpMessage(TPCanID[m_mode][m_device], ""); // commented by cychoi 2013.06.18 for cluster IOT 139 // added by wspark 2013.04.28 for cluster IOT

        m_device = (MEDIA_SOURCE_T)device;
        m_activeDevice = (MEDIA_SOURCE_T)device; // added by cychoi 2013.09.03 for cluster update on device changed
        m_activeMode = m_mode; // added by cychoi 2013.09.03 for cluster update on device changed
        ResetDeviceData( m_device );

        if(QObject::sender() == m_pVideoCtrl)
        {
            notifySDPByVideoSource(m_device);
        }

        m_devices[device].m_title = "";
        m_devices[device].m_videoTpMessage = "";
        m_bSendingCanProgressMessage = false;
        quint64 data = GetClusterState();
        SendClusterData_OpStateOnly(data);

        sendCanTpMessage(TPCanID[m_mode][m_device], ""); // modified by cychoi 2013.06.18 for cluster IOT 139 // deleted by wspark 2013.04.20
        m_pEngineMain->handleDeckErrorOnDeviceChanged(); // added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    }
    //} modified by cychoi 2013.08.17
}

void ExternalNotifier::onStateChanged(int state)
{
    MP_MEDIUM << state << LOG_ENDL;

    //{ modified by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
    if ( m_device < MEDIA_SOURCE_MAX )
    {
        m_devices[m_device].m_state = (PLAYER_STATE)state;
        // { modified by sjhyun 2013.09.22 for ITS 190471
        if ((PLAYER_STATE)state == PLAYER_STOP)
        {
            m_devices[m_device].m_position = 0; // added by Dmitry Bykov 08.04.13 for ISV77807
        }
        //{added by edo.lee 2013.06.27 for BT call
        else if ((PLAYER_STATE)state == PLAYER_PLAY)
        {
            m_pEngineMain->setSelectTuned(false);//Added by taihyun.ahn 2013.10.02  for ITS 188100
            if((m_device != MEDIA_SOURCE_DVDVIDEO && m_device != MEDIA_SOURCE_VCD) || m_bPassPlayState == false)
            {
                if(m_pEngineMain->getPlayAfterBTCallState() == false )//Added by taihyun.ahn 2013.10.02  for ITS 188100
                {
                    m_pEngineMain->setPlayOnBTCall();
                }
            }
        }
        //}added by edo.lee 2013.06.27 for BT call
        else if ((PLAYER_STATE)state == PLAYER_PAUSE)
        {
            m_bPassPlayState = false;
            m_pEngineMain->setPlayAfterBTCallState(false);//moved by taihyun.ahn 2013.10.15  for ITS 0195246
        }
        // } modified by sjhyun 2013.09.22

        SendMostData_State();

        if(QObject::sender() == m_pVideoCtrl)
        {
            notifySDPByVideoPlayingState(m_devices[m_device].m_state);
            // added by Dmitry 12.07.13 for ISV85006
            if (m_device == MEDIA_SOURCE_JUKEBOX || m_device == MEDIA_SOURCE_USB1 || m_device == MEDIA_SOURCE_USB2)
                notifyFMVideoPlaybackChanged(m_devices[m_device].m_state);
            // added by Dmitry 12.07.13 for ISV85006
        }
    }
    //} modified by cychoi 2013.08.17
}

void ExternalNotifier::onPositionChanged(int nSeconds)
{
    static int oldSeconds = 0; // added by wspark 2012.11.05 for sending one can message a second.
    // MP_MEDIUM << "m_device=" << m_device << " nSeconds=" << nSeconds << LOG_ENDL;

    if( !m_bBasicControlEnable && ( m_device == MEDIA_SOURCE_IPOD1 ||  m_device == MEDIA_SOURCE_IPOD2 ))
    {
        MP_LOG << "onPositionChanged. but !m_bBasicControlEnable" << LOG_ENDL;
        return;
    }

    //{ modified by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
    if ( m_device < MEDIA_SOURCE_MAX )
    {
        m_devices[m_device].m_position = nSeconds;

        // { modified by wspark 2012.11.05 for sending one can message a second.
        int nCluseterSeconds = nSeconds/1000;
        if(nCluseterSeconds != oldSeconds)
        {
            //if(m_device != MEDIA_SOURCE_BLUETOOTH) // added by wspark 2012.12.10 for sending bluetooth opstate
            if(m_device != MEDIA_SOURCE_BLUETOOTH && m_bSendingCanProgressMessage)
            {
                AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_pEngines[APP_MEDIA_MODE_VIDEO]); // added by yungi 2013.11.05  for Cluster DVD-Video Chapter ,TitleDiscMenu
                // { modified by yungi 2013.11.05  for Cluster DVD-Video Chapter ,TitleDiscMenu
                if(pVideoEngine != NULL)
                {
                    // { modified by cychoi 2013.11.12 for DVD Title/Disc Menu don't have to send Opstate on onPositionChanged
                    if(m_device != MEDIA_SOURCE_DVDVIDEO || pVideoEngine->isDVDTrackPlaying())
                    {
                        SendClusterData_Progress(nCluseterSeconds);
                    }
                    //else
                    //{
                    //    quint64 data = GetClusterState(); // added by yungi 2013.11.05  for Cluster DVD-Video Chapter ,TitleDiscMenu
                    //    SendClusterData_OpStateOnly(data);
                    //}
                    // } modified by cychoi 2013.11.12
                }
                // } modified by yungi
                m_bFirstUpdateTrack = false; // added by wspark 2013.04.14 for cluster IOT #40
            }
        }
        oldSeconds = nCluseterSeconds;
        // } modified by wspark 2012.11.05
    }
    //} modified by cychoi 2013.08.17
}

// { added by wspark 2013.03.30 for ISV 77359
// { modified by wspark 2013.04.09
void ExternalNotifier::onPositionCleared(bool value)
{
    //m_bSendingCanProgressMessage = false;
    m_bSendingCanProgressMessage = value;
}
// } modified by wspark
// } added by wspark

void ExternalNotifier::onDurationChanged(int nSeconds)
{
    MP_MEDIUM << "m_device=" << m_device << " nSeconds=" << nSeconds << LOG_ENDL;

    if ( m_device >= MEDIA_SOURCE_MAX || nSeconds < 1)
    {
        return;
    }

    m_devices[m_device].m_duration = nSeconds;
    m_bSendingCanProgressMessage = true; // added by wspark 2013.04.09 for cluster IOT #37
}

void ExternalNotifier::onTrackChanged(int index, int quantity)
{
    MP_MEDIUM << "m_device=" << m_device << " index=" << index << " quantity=" << quantity << LOG_ENDL;

    // modified by cychoi 2013.10.26 for only using trackClusterChanged signal (onTrackClusterChanged)
}

//{ added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
void ExternalNotifier::onTrackClusterChanged(int index, int quantity, bool bResetPosition)
{
    MP_LOG << "m_device=" << m_device << " index=" << index << " quantity=" << quantity << LOG_ENDL;
    if (m_device == MEDIA_SOURCE_MAX)
    {
        return;
    }

    if(m_bTuneMode)
    {
        return;
    }

    if(QObject::sender() == m_pVideoCtrl)
    {
        m_devices[m_device].m_number = index;
        //m_devices[m_device].m_number = index + 1;

        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_VIDEO_STATUS, m_devices[m_device].m_number);
    }
    else
    {
        m_devices[m_device].m_number = index;
        //m_devices[m_device].m_number = index + 1;
        m_devices[m_device].m_quantity = quantity;
    }

    // { modified by cychoi 2013.09.03 for cluster update on device changed
    if((m_devices[m_device].m_number > 0) ||
       (m_device == MEDIA_SOURCE_MP3 && m_bDirTreeFetched == false)) // added by cychoi 2013.11.18 for ITS 208698 MP3 Play Info on Cluster 
    {
        // { added by cychoi 2013.11.28 for ITS 211634
        if (m_devices[m_device].m_number == 0xffff)
        {
            m_devices[m_device].m_title = "";
            m_devices[m_device].m_videoTpMessage = "";
            
            m_bSendingCanProgressMessage = false; // added by wspark 2013.04.17

            quint64 data = GetClusterState();
            SendClusterData_OpStateOnly(data);
            sendCanTpMessage(TPCanID[m_mode][m_device], "");
            MP_LOG << "TP message was cleared" << LOG_ENDL;
        }
        else if( !m_bBasicControlEnable && ( m_device == MEDIA_SOURCE_IPOD1 ||  m_device == MEDIA_SOURCE_IPOD2 ))
        {
            m_bSendingCanProgressMessage = false;
            quint64 data = GetClusterState();
            SendClusterData_OpStateOnly(data);
            sendCanTpMessage(TPCanID[m_mode][m_device], m_devices[m_device].m_title);
            MP_LOG << "TP message " << m_devices[m_device].m_title << LOG_ENDL;
        }
        else
        // } added by cychoi 2013.11.28
        {
            m_bTpNeedToSend = true;

            if( m_device == MEDIA_SOURCE_IPOD1 || m_device == MEDIA_SOURCE_IPOD2 )
            {
                MP_MEDIUM << "IPOD always dose not clear position manually" << LOG_ENDL;
            }
            else if(bResetPosition) //add if by junam 2013.11.14 for ITS_NA_215663
            {
                m_devices[m_device].m_position = 0; // added by cychoi 2013.06.07 for ITS 150487
            }
            MP_MEDIUM << "m_devices[m_device].m_position = " << m_devices[m_device].m_position << LOG_ENDL;
            // Modified by jonghwan.cho@lge.com 2013.11.14. for ISV KOR 93448
            SendClusterData_Progress(m_devices[m_device].m_position/1000); // modified by cychoi 2013.11.15 for HMC QC Cluser Play Time Error
            //SendClusterData_Progress(0);
            // } added by wspark

            m_bSendingCanProgressMessage = true; // added by wspark 2013.02.19

            //m_bTpNeedToSend = true; // for Cluster IOT issue #95
        }
    }
    // } modified by cychoi 2013.09.03
}
//} added by cychoi 2013.08.17

// modified by kihyung 2013.08.08 for ITS 0183028
void ExternalNotifier::onMediaInfoChanged(int deviceID, QString title, QString album, QString artist, QString, QString, QString, QString, QString fileName ) // modified by junggil 2012.08.02 for CR12564
{
    MP_MEDIUM << "m_device=" << m_device << " deviceID=" << deviceID << " title=" << title << " album=" << album << " artist=" << artist << " fileName=" << fileName << LOG_ENDL;
    
    //{ modified by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
    if ( m_device < MEDIA_SOURCE_MAX )
    {
        // { modified by wspark 2013.03.27 for ISV 77360
        //m_devices[m_device].m_title = title;
        QString mediaTitle = (title == "") ? fileName : title;
        if(m_devices[m_device].m_title != mediaTitle && !m_bTuneMode)
        {
            // { modified by wspark 2013.04.17
            //m_bAudioTpNeedToSend = true;
            m_bTpNeedToSend = true;
            // } modified by wspark
            m_devices[m_device].m_title = mediaTitle;
        }
        // } modified by wspark
        m_devices[m_device].m_album = album;
        m_devices[m_device].m_artist = "";

        if (MEDIA_SOURCE_MP3 == m_device)
        {
            int lastSlashIndex = fileName.lastIndexOf('/');
            if (lastSlashIndex != -1)
            {
                int prevSlashIndex = fileName.lastIndexOf('/',lastSlashIndex-1);
                m_devices[m_device].m_artist = fileName.mid(prevSlashIndex+1,lastSlashIndex - prevSlashIndex-1);
            }
        }
        else
        {
            m_devices[m_device].m_artist = artist;
        }

        // added by edo.lee 2013.03.25
        if( m_pEngineMain->getIsRestored())
        {
            m_pEngineMain->setIsRestored(false);
            return;
        }

        //if(m_isScanMode) m_OSDDisplay = true; //added by edo.lee 2013.04.05

        //if(title.length()>0 && title.compare(" ")!=0 ) //modified by edo.lee 2013.05.10) {
        //{
            //displayOSD(OSD_EVENT_AUDIO_TRACK_INFO);
        //}
    }
    //} modified by cychoi 2013.08.17
}

// { added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
void ExternalNotifier::onMediaInfoClusterChanged(QString title)
{
    MP_LOG << "m_device=" << m_device << " title=" << title << LOG_ENDL;
    
    if ( m_device < MEDIA_SOURCE_MAX )
    {
        if(QObject::sender() == m_pVideoCtrl)
        {
            // { modified by cychoi 2013.09.03 for ITS 187585
            QString videoTpMessage = title;
            if(m_device == MEDIA_SOURCE_VCD)
            {
                QString strTrack = QString(QApplication::translate("main", "STR_MEDIA_TRACK"));
                videoTpMessage.prepend(strTrack);
            }
            else if(m_device == MEDIA_SOURCE_DVDVIDEO)
            {
                QString strTitle = QString(QApplication::translate("main", "STR_MEDIA_TITLE"));
                videoTpMessage.prepend(strTitle);
            }
            if(m_devices[m_device].m_videoTpMessage != videoTpMessage && !m_bTuneMode)
            {
                m_bTpNeedToSend = true;
                m_devices[m_device].m_videoTpMessage = videoTpMessage;
            }
            // } modified by cychoi 2013.09.03
        }
        else
        {
            if(m_devices[m_device].m_title != title && !m_bTuneMode)
            {
                m_bTpNeedToSend = true;
                m_devices[m_device].m_title = title;
            }
        }
    }
}
// } added by cychoi 2013.08.17

// {  added by cychoi 2013.11.18 for ITS 208698 MP3 Play Info on Cluster 
void ExternalNotifier::onDirTreeFetched( bool bFetched)
{
    m_bDirTreeFetched = bFetched;
}
// }  added by cychoi 2013.11.18

//added by eunkoo 2012.07.26 for CR11898
void ExternalNotifier::onBTMusicInfoChanged( QString title, QString album, QString artist ,QString count) 
{
    MP_LOG << "m_device=" << m_device << " title=" << title << " album=" << album << " artist=" << artist << " count=" << count << LOG_ENDL;
    //{ modified by hyochang.ryu 20130520
    if ( m_device == MEDIA_SOURCE_BLUETOOTH) // modified by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
    {
        //{ modified by hyochang.ryu 20131102 for cluster 
        m_devices[MEDIA_SOURCE_BLUETOOTH].m_title = (title == "" || title == "Unknown") ? QString(QApplication::translate("main", "STR_MEDIA_UNKNOWN")) : title; // modified by cychoi 2014.07.11 for AVN & Cluster display different string for Unknown (VI 15MY Bluetooth Compatibility Test)
        m_devices[MEDIA_SOURCE_BLUETOOTH].m_album = album;
        m_devices[MEDIA_SOURCE_BLUETOOTH].m_artist = artist;	
        //} modified by hyochang.ryu 20131102 for cluster 
		
        quint64 data = GetClusterState();
        SendClusterData_OpStateOnly(data);
        // { modified by wspark 2013.04.30 for cluster IOT
        //sendCanTpMessage(TPCanID[m_mode][m_device], title);
        sendCanTpMessage(TPCanID[m_mode][MEDIA_SOURCE_BLUETOOTH], m_devices[MEDIA_SOURCE_BLUETOOTH].m_title);
        // } modified by wspark
    }
    //} modified by hyochang.ryu 20130520
}
//} added by eunkoo



int ExternalNotifier::GetClusterState()
{
    if (m_mode < APP_MEDIA_MODE_MAX && m_device < MEDIA_SOURCE_MAX)
    {
        return clusterStates[m_mode][m_device];
    }
    return 0xFF;
}

void ExternalNotifier::SendClusterData(quint64 data)
{
    // { modified by wspark 2013.04.23 for cluster IOT
    //if(m_bClusterUpdate && !m_bBtCallProgressing)
    if(m_bClusterUpdate && !m_bBtCallProgressing && IsAvModeFromSharedMemory())
    {
    // } modified by wspark
        // { modify by wspark 2012.07.27 for CR9361
        // CAN DB v0.12.01
        //SQCanFrame frame = {0x1E3, 0x08, {0,0,0,0,0,0,0,0}};
        SQCanFrame frame = {0x114, 0x08, {0,0,0,0,0,0,0,0}};
        // } modify by wspark 2012.07.27 for CR9361
        data = qToBigEndian(data);
        memcpy(&frame.uqcandata, &data, sizeof(quint64));

        emit Can_data_send(frame);
    } // added by wspark 2013.03.16
}

// { added by wspark 2012.09.14 for CAN unused signals.
void ExternalNotifier::SendClusterData_OpStateOnly(quint64 opState)
{
    // { deleted by wspark 2013.04.25 for cluster IOT
    /*
    if (m_mode == APP_MEDIA_MODE_PHOTO_FRONT || m_mode == APP_MEDIA_MODE_PHOTO_REAR)
        return;
    */
    // } deleted by wspark

    //{ added by cychoi 2013.12.16 for ISV 96147
    if(opState == 0x00 || opState == 0xFF)
    {
        // If OpState is invalid, just return.
        MP_HIGH << "opState = " << opState << LOG_ENDL;
        return;
    }
    //} added by cychoi 2013.12.16

    MP_LOG << "opState = " << opState << LOG_ENDL;

    quint64 data = 0;

    // HU NAVI ONOFF (first 1bit)
    // HU_OpState (7 bits)
    data = opState;

    // HU_Tuner_Area (3bits)
    data <<= 3;
    data += 0x7;

    // HU_Preset_Number (5bits)
    data <<= 5;
    data += 0x1f;

    // HU_Track_Number (16bits)
    data <<= 16;
    data += 0xffff;

    // HU_Play_time_Sec (6bits)
    data <<= 6;
    data += 0x3f;

    // HU_Play_time_Min (7bits)
    data <<= 7;
    data += 0x7f;

    // HU_Play_time_Hour (6bits)
    data <<= 6;
    data += 0x3f;

    // HU_Frequency (9bits)
    data <<= 9;

    // HU_Disc_select_No (4bits)
    data <<= 4;
    data += 0xf;

    SendClusterData(data);
}
// } added by wspark


// { added by wspark 2012.09.18 CAN TP Message
bool ExternalNotifier::sendCanTpMessage(quint16 id, QString data)
{
    MP_LOG << "TP ID = " << id << "m_bClusterUpdate = " << m_bClusterUpdate << LOG_ENDL;
	// { added by wspark 2013.04.29 for cluster IOT
    if(id == 0x0 || id == 0xFF)
    {
        return false;
    }
    // } added by wspark

    // { modified by wspark 2013.04.28 for cluster IOT
    //if(m_bClusterUpdate && !m_bBtCallProgressing && IsAvModeFromSharedMemory())
    if((m_bClusterUpdate && !m_bBtCallProgressing && IsAvModeFromSharedMemory()) ||
       (m_bClusterUpdate && data == ""))
    // } modified by wspark
    {


        QDBusReply<bool> reply;

        QByteArray buffer;

        // { modified by wspark 2013.05.01
        //buffer.resize(100);
        buffer.resize(258);
        // } modified by wspark

        buffer.clear();
        //QByteArray arrayStationName = data.toLocal8Bit(); // deleted by wspark 2013.05.01
        //quint8 nLength = arrayStationName.length(); // removed by changjin kim 2012.10.05 Not use
        quint8 nLength2 = data.size();

        buffer[0] = (id >> 8);
        buffer[1] = (id & 0xff);
        // { modified by wspark 2013.04.12 for clear TP
        // { modified by wspark 2013.01.24 for IPOD/BT TP message.
        //buffer[2] = nLength2 * 2;
        if(id == 0x48F || id == 0x485) // 0x485 : BT, 0x48F : IPOD
        {
            // { modified by wspark 2013.03.29 for ISV 77368
            //buffer[2] = nLength2 * 2 + 1; // for opcode
            if(data == "")
            {
                // { modified by wspark 2013.04.17
                /*
                buffer[2] = 2; // opcode + clear TP
                buffer[4] = 0x0; // clear TP
                */
                buffer[2] = 3; // opcode(1byte) + clear TP(2byte)
                buffer[4] = 0x0; // clear TP
                buffer[5] = 0x0; // clear TP
                // } modified by wspark
            }
            else
            {
                // { modified by wspark 2013.05.01
                //buffer[2] = nLength2 * 2 + 1; // for opcode
                // { modifed by cychoi 2013.06.11 for TP message 64 bytes limitation
                if(nLength2 * 2 + 1 > 0x41)
                {
                    buffer[2] = 0x41;
                }
                // } modifed by cychoi 2013.06.11
                else
                {
                    buffer[2] = nLength2 * 2 + 1; // for opcode
                }
                // } modified by wspark
            }
            // } modified by wspark
            if(id == 0x48F)
            {
                buffer[3] = 0x1; // opcode : 0x1
            }
            else if(id == 0x485)
            {
                buffer[3] = 0x3; // opcode : 0x3
            }
        }
        else
        {
            // { modified by wspark 2013.03.30 for ISV 77359
            //buffer[2] = nLength2 * 2;
            if(data == "")
            {
                // { modified by wspark 2013.04.17
                /*
                buffer[2] = 1;      // length
                buffer[3] = 0x0;    // clear TP
                */
                buffer[2] = 2;      // length
                buffer[3] = 0x0;    // clear TP
                buffer[4] = 0x0;    // clear TP
                // } modified by wspark
            }
            else
            {
                // { modified by wspark 2013.05.01
                //buffer[2] = nLength2 * 2;
                // { modifed by cychoi 2013.06.11 for TP message 64 bytes limitation
                if(nLength2 * 2 > 0x40)
                {
                    buffer[2] = 0x40;
                }
                // } modifed by cychoi 2013.06.11
                else
                {
                    buffer[2] = nLength2 * 2;
                }
                // } modified by wspark
            }
            // } modified by wspark
        }
        // } modified by wspark
        // } modified by wspark

        // { modified by wspark 2013.03.29 for ISV 77368
        //QByteArray tpbody((const char*) (data.utf16()), data.size() * 2);
        //buffer.append(tpbody);
        if(data != "")
        {
            // { modified by wspark 2013.05.01
            //QByteArray tpbody((const char*) (data.utf16()), data.size() * 2);
            int dataLen = data.size() * 2;
            // { modifed by cychoi 2013.06.11 for TP message 64 bytes limitation
            if(dataLen > 0x40)
            {
                dataLen = 0x40;
            }
            // } modifed by cychoi 2013.06.11
            QByteArray tpbody((const char*) (data.utf16()), dataLen);
            // } modified by wspark

            buffer.append(tpbody);
        }
        // } modified by wspark

        // { added by junam 2012.10.24 for crash protection.
        if(m_pCanTPInterface == NULL)
        {
            if (QDBusConnection::sessionBus().interface()->isServiceRegistered("com.lge.car.micom"))
            {
                m_pCanTPInterface = new QDBusInterface("com.lge.car.micom", "/Dtc", "com.lge.car.micom.dtc", QDBusConnection::sessionBus());
            }
            else
            {
                MP_LOG << "Service com.lge.car.micom is not registered" << LOG_ENDL;
            }
        }

        if( m_pCanTPInterface && m_pCanTPInterface->isValid()) //}added by junam
        {
            reply = m_pCanTPInterface->call("LgeDBus_CANTPSendMsg", buffer);
        }


        return reply;
    } // added by wspark 2013.03.16

    return false; // added by cychoi 2013.06.10 for return
}
// } added by wspark

void ExternalNotifier::SendClusterData_Progress(int position)
{
    if(m_device>=MEDIA_SOURCE_AUX1) return;	// added by hyochang.ryu 20130713 for BT

    //{ added by cychoi 2013.12.16 for ISV 96147
    int id = GetClusterState();

    if(id == 0x00 || id == 0xFF)
    {
        // If OpState is invalid, just return.
        MP_HIGH << "opState = " << id << LOG_ENDL;
        return;
    }
    //} added by cychoi 2013.12.16

    // MP_LOG << position << LOG_ENDL;
    
    //{ modified by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
    quint64 data = 0;

    // { modify by wspark 2012.07.27 for CR9361
    // HU NAVI ONOFF (first 1bit)
    // HU_OpState (7 bits)
    data = GetClusterState();

    // HU_Tuner_Area (3bits)
    data <<= 3;
    data += 0x7; // added by wspark 2012.09.14 for CAN unused signals.

    // HU_Preset_Number (5bits)
    data <<= 5;
    data += 0x1f; // added by wspark 2012.09.14 for CAN unused signals.

    // HU_Track_Number (16bits)
    data <<= 16;
    // { modified by wspark 2012.09.14 for CAN unused signals.
    // data += m_devices[m_device].m_number;
    // MP_LOG << "m_devices[m_device].m_number = " << m_devices[m_device].m_number << " m_device = " << m_device << LOG_ENDL;
    if(m_devices[m_device].m_number > 0 )
        data += m_devices[m_device].m_number;
    else
        data += 0xffff;
    // } modified by wspark

    // HU_Play_time_Sec (6bits)
    data <<= 6;
    data += position % 60;

    // HU_Play_time_Min (7bits)
    data <<= 7;
    data += (position % 3600) / 60;

    // HU_Play_time_Hour (6bits)
    data <<= 6;
    data += position / 3600;

    // HU_Frequency (9bits)
    data <<= 9;
    // { added by wspark 2012.09.18 for unused signal.
    if(m_device == MEDIA_SOURCE_DVDVIDEO)
    // { modified by wspark 2012.11.07 for sending title number for DVD
    //data += m_devices[m_device].m_number;
    {
        bool ret = 0;
        if(!m_devices[m_device].m_folderName.isEmpty())
        {
            int titleNumber = m_devices[m_device].m_folderName.toInt(&ret);
            if(ret)
                data += titleNumber;
        }
    }
    // } modified by wspark
    else
        data += 0x1ff;
	// } added by wspark

    // HU_Disc_select_No (4bits)
    data <<= 4;
    // { modified by wspark 2012.09.14 for CAN unused signals.
    // HU_Disc_select_No : 0xf(invalid)
    data += 0xf;
    // } modified by wspark
    // } modify by wspark 2012.07.27 for CR9361

    SendClusterData(data);

    // { added by cychoi 2013.09.03 for cluster update on device changed
    if(m_bClusterUpdate == false)
    {
        // should not send Tp Message to Cluster.
        return;
    }
    // } added by cychoi 2013.09.03

    if(m_bTpNeedToSend)
    {
        usleep(10000); // Delay

        // { modified by cychoi 2013.08.29 for HMC QC IPOD cluster display on Bootup
        if((m_devices[m_device].m_number > 0) ||
           (m_device == MEDIA_SOURCE_MP3 && m_bDirTreeFetched == false)) // added by cychoi 2013.11.18 for ITS 208698 MP3 Play Info on Cluster 
        {
            m_bTpNeedToSend = false; // moved by cychoi 2013.09.03 for cluster update on device changed
            if(m_mode == APP_MEDIA_MODE_VIDEO && m_devices[m_device].m_videoTpMessage != "")
            {
                // { modified by cychoi 2013.06.28 for HMC request - don't send DVD Tp message
                if(m_device != MEDIA_SOURCE_DVDVIDEO)
                {
                    // { modified by cychoi 2013.09.03 for ITS 187585 // { modified by cychoi 2013.06.24 for ISV 85786
                    MP_LOG << "Video: Track No" << m_devices[m_device].m_number << "TpMessage" << m_devices[m_device].m_videoTpMessage << LOG_ENDL;
                    sendCanTpMessage(TPCanID[m_mode][m_device], m_devices[m_device].m_videoTpMessage);
                    // } modified by cychoi 2013.09.03 // } modified by cychoi 2013.06.24
                }
                else
                {
                    MP_LOG << "DVD: Track No" << m_devices[m_device].m_number << "TpMessage" << m_devices[m_device].m_videoTpMessage << LOG_ENDL;
                }
                // } modified by cychoi 2013.06.28
            }
            else if(m_mode == APP_MEDIA_MODE_AUDIO && m_devices[m_device].m_title != "")
            {
                MP_LOG << "Audio: Track No" << m_devices[m_device].m_number << "TpMessage" << m_devices[m_device].m_title << LOG_ENDL;
                sendCanTpMessage(TPCanID[m_mode][m_device], m_devices[m_device].m_title);
            }
        }
        else //if(m_devices[m_device].m_number <= 0)
        {
            MP_LOG << "Clear TpMessage" << LOG_ENDL;
            sendCanTpMessage(TPCanID[m_mode][m_device], "");
        }
        //} added by cychoi 2013.08.29
    }
    //} modified by cychoi 2013.08.17
}

// removed by cychoi 2013.08.17 for code arrangement

void ExternalNotifier::SendMostData_State()
{
    if(m_device >= MEDIA_SOURCE_MAX) return; // added by kihyung 2013.07.17 for ISV 86690
    
    uint deckStatus = 0x00;

    switch (m_devices[m_device].m_state)
    {
        case PLAYER_PLAY:        deckStatus = 0x00;  break;
        case PLAYER_STOP:        deckStatus = 0x01;  break;
        case PLAYER_PAUSE:       deckStatus = 0x02;  break;
        case PLAYER_FASTFORWARD: deckStatus = 0x07;  break;
        case PLAYER_REWIND:      deckStatus = 0x08;  break;
        default:
            break;
    }

    switch (m_device)
    {
        case MEDIA_SOURCE_JUKEBOX:
        {
            break;
        }
        case MEDIA_SOURCE_DVDAUDIO:
        case MEDIA_SOURCE_DVDVIDEO:
        case MEDIA_SOURCE_CDDA:
        case MEDIA_SOURCE_MP3:
        case MEDIA_SOURCE_VCD:
        {
            // { changed by junam 2012.11.26 for CR15923
            //m_pMostDiscAdaptor->call("SetDeckStatus", deckStatus);
            QDBusInterface *pBusInterface = getMostDiscAdaptor();
            if(pBusInterface)
                pBusInterface->call("SetDeckStatus", deckStatus);
            //} changed by junam
            break;
        }
        default:
        {
            // { changed by junam 2012.11.26 for CR15923
            //m_pMostAuxInAdaptor->call("SetDeckStatus", deckStatus, DEVICE_AUX_NUMBER_ONE);
            QDBusInterface * pBusInterface = getMostAuxInAdaptor();
            if(pBusInterface)
                pBusInterface->call("SetDeckStatus", deckStatus, DEVICE_AUX_NUMBER_ONE);
            //} changed by junam
            break;
        }
    }
}

// removed by cychoi 2013.08.17 for code arrangement

// { added by edo.lee 2012.12.04 for new statusbar
/*
void ExternalNotifier::DisplyChangeTime(int nSeconds,  APP_MEDIA_MODE_T media)
{
 	// MP_LOG << LOG_ENDL;
        //m_pOSDView = m_pEngineMain->GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN); //delete by ys 2013.09.30 //added by edo.lee 2012.12.08 for new statusbar

    QString info =  m_strOSDChangeTime.replace("INFO^","INFO_TIME^",Qt::CaseSensitive);
    info =  m_strOSDChangeTime.replace("INFO_DIAL^","INFO_DIAL_TIME^",Qt::CaseSensitive); // modified by edo.lee 2013.01.11

 	QString timeString;
	if(media == APP_MEDIA_MODE_AUDIO)
		timeString = m_pMainUtil->convertDurationAudio(nSeconds);
	else if (media == APP_MEDIA_MODE_VIDEO)
		timeString = m_pMainUtil->convertDuration(nSeconds);
	
    info += QString("("+timeString+")");
	// MP_LOG<<m_pOSDView << "OSD String : " << info << LOG_ENDL;
        //if(m_pOSDView){//delete by ys 2013.09.30
	    //m_pOSDView->setWindowTitle(info);	
	    //OSDWriter::SendOSD(info); //modified by edo.lee 2013.04.04
		//QCoreApplication::flush(); //added by edo.lee 2013.01.16
        //}
}
*/
// added by edo.lee 2013.04.10
void ExternalNotifier::StopOSDForClone()
{
	//MP_LOG << LOG_ENDL;
	//m_updatePositionTimer.stop();
	//OSDWriter::SendOSD("@OSD:FR:CLEAR_AV");//modified by edo.lee 2013.04.10
}
// added by edo.lee 2013.04.10
/*
void ExternalNotifier::DisplayVideoInfo() //modified by edo.lee 2013.03.04
{
    if(m_device >= MEDIA_SOURCE_MAX) return; // added by kihyung 2013.07.17 for ISV 86690
    
	MP_LOG << LOG_ENDL;
    //{ added by yongkyun.lee 20130702 for :  NO CR - None AV OSD
    //{ modified by yongkyun.lee 2013-07-07 for : NO CR - OSD None AV
    // if(!m_pEngineMain->GetNotifyAVModeState())
    //     return ;
    //} modified by yongkyun.lee 2013-07-07 
    //} added by yongkyun.lee 20130702 
    //added by edo.lee 2013.07.12
    if(IsAvModeFromSharedMemory()== false)
	{
		MP_LOG<< "current is other mode.. return" << LOG_ENDL;
		return;
	}
	//added by edo.lee 2013.07.12
    MP_LOG<<"m_devices[m_device].m_title "<<m_devices[m_device].m_title << LOG_ENDL;
     if((m_device!=MEDIA_SOURCE_AUX1||m_device!=MEDIA_SOURCE_AUX2||m_device!=MEDIA_SOURCE_BLUETOOTH) &&		
	 ( m_devices[m_device].m_title.isEmpty() && m_devices[m_device].m_title.isNull() &&  m_devices[m_device].m_title == " "&&  m_devices[m_device].m_title == ""))
 	{
 		MP_LOG<<"title is null..." << LOG_ENDL;
		return;
 	}
	//added by edo.lee 2013.07.07 
    
    //m_pOSDView = m_pEngineMain->GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN); //delete by ys 2013.09.30 //added by edo.lee 2012.12.08 for new statusbar
   	AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_pEngines[APP_MEDIA_MODE_VIDEO]);   
    if(pVideoEngine == NULL) return; // added by kihyung 2013.07.16 for STATIC 23539
    QString info = "";
	// { modified by edo.lee 2013.03.05
	//if(m_bTuneMode)
	//{
		// { modified by edo.lee 2013.01.03 OSD Issue
		 //modified by edo.lee 2013.03.22 
		if(m_bTuneMode && !m_bTuneSameFile)
		{		
			if(m_pEngineMain->getIsFROnTune() ||
				(( pVideoEngine->isVideoListView() == VIDEO_DISPLAY_FRONT && m_pEngineMain->getIsSwapEnabled())
				||(pVideoEngine->isVideoListView() == VIDEO_DISPLAY_REAR && !m_pEngineMain->getIsSwapEnabled()))) // modified by lssanh 2013.03.24 ISV77034
			{			
				info = "@OSD:FR^MEDIA^INFO_DIAL^";
			}
			else
			{			
				//if( m_pEngineMain->getPlayInfoDisplay() == DISPLAY_FRONT_REAR) //remove by edo.lee 2013.04.25
					info = "@OSD:"+GetOSDDispString()+"^MEDIA^INFO_DIAL^";
			}
		}
		else
		{
			//modified by edo.lee 2013.07.14
			if(m_pEngineMain->getIsFROnTune() || (m_OSDTreckSpeed && pVideoEngine->isVideoListView() != VIDEO_DISPLAY_MAX)) // modified by lssanh 2013.03.24 ISV77034
			{					
				info = "@OSD:FR^MEDIA^INFO^";
			}
			else{				
				//if( m_pEngineMain->getPlayInfoDisplay() == DISPLAY_FRONT_REAR) //added by edo.lee 2013.04.25
					info ="@OSD:"+GetOSDDispString()+"^MEDIA^INFO^" ;
			}
		}
		//modified by edo.lee 2013.03.22 
		//info ="@OSD:"+GetOSDDispString()+"^MEDIA^INFO_DIAL^" ;
		//info = "@OSD:FR^MEDIA^INFO_DIAL^" ;
		// } modified by edo.lee
	//}
	//else
	//{
		// { modified by edo.lee 2012.12.15
		//info = "@OSD:"+GetOSDDispString()+"^MEDIA^INFO^" ;
		//info = "@OSD:FR^MEDIA^INFO^" ; 
		// } modified by edo.lee
	//}
	// } modified by edo.lee

    QString folderName = "";
    QString fileName = "";
    setOSDFileFolder(fileName, folderName);
	fileName = AVP_ENCODE_OSD_FORMAT_RULE1( AVP_ENCODE_OSD_FORMAT_RULE2	(fileName)); //added by edo.lee 2013.03.23	
	switch(m_device)
	{
		case MEDIA_SOURCE_JUKEBOX:
		{
		    info += "JUKEBOX^"; 
			info += "JUKEBOX^VIDEO^";
			//info +=  QString(folderName + "/"+ fileName+"^");
			info += QString(fileName+"^");//modified by edo.lee 2013.02.28
		    break;
		}
		case MEDIA_SOURCE_USB1:
		case MEDIA_SOURCE_USB2:
		{
		    info += "USB^"; 
			info += "USB^VIDEO^";
			//info +=  QString(folderName + "/"+ fileName+"^");
			info += QString(fileName+"^");//modified by edo.lee 2013.02.28
		    break;
		}
//{ modified by yongkyun.lee 20130130 for : ISV 71746
//			case MEDIA_SOURCE_DVDVIDEO:
//			case MEDIA_SOURCE_VCD:
//			{
//			    info += "DISC^";	
//				info += "DISC^VIDEO^";
//				if(m_device == MEDIA_SOURCE_VCD)
//				{
//					info += QString(fileName+"^");
//				}
//				else if(m_device == MEDIA_SOURCE_DVDVIDEO)
//				{
//					info += QString(folderName +" "+ fileName+"^");
//				}
//			    break;
//			}
		case MEDIA_SOURCE_DVDVIDEO:
        {
            info += "DVD^";    
            info += "DVD^VIDEO^";
            //info += QString(folderName +" "+ fileName+"^");
            info += QString(fileName+"^");//modified by edo.lee 2013.02.28
            break;
        }
		case MEDIA_SOURCE_VCD:
		{
		    info += "DISC^";	
			info += "DISC^VIDEO^";
			info += QString(fileName+"^");
		    break;
		}
//} modified by yongkyun.lee 20130130 
	    //{ added by hyochang.ryu 20130522 for AUX OSD
	    case MEDIA_SOURCE_AUX1:
	    case MEDIA_SOURCE_AUX2:		
	    	info += "AUX^"; 
	    	info += "AUX^VIDEO^";
		//OSDWriter::SendOSD(info); return;
	    	break;
	   //} added by hyochang.ryu 20130522 for AUX OSD
		default:
		{
		    MP_LOG << "Unsupported source." << LOG_ENDL;
		    return;
		}
	}

	if(m_bTuneMode)
	{
		// remove by edo.lee 2013.04.18

		
		// { added by kihyung 2013.06.21 for ISV 85801
		if(m_devices[m_device].m_quantity <= 1) {
            MP_LOG << "m_devices[m_device].m_quantity" << m_devices[m_device].m_quantity << LOG_ENDL;
            return;
		}
		// } added by kihyung 2013.06.21
	}else {

         //{ modified by yongkyun.lee 2013-07-15 for : 
         if(m_devices[m_device].m_playingRandomMode == RANDOMFILE || 
            m_devices[m_device].m_playingRandomMode == RANDOMFOLDER )
                info += "(SH)^";

         if(m_devices[m_device].m_playingRepeatMode == REPEATFILE )
                info += "(RP1)^";
         else if(m_devices[m_device].m_playingRepeatMode == REPEATFOLDER )
                info += "(RPF)^";
         //} modified by yongkyun.lee 2013-07-15 

	}	
        
    if((m_bTuneMode == false || m_bTuneSameFile) // modified by Dmitry 30.04.13
        &&(m_device < MEDIA_SOURCE_AUX1)) 			//modified by hyochang.ryu 20130608 for AUX OSD
	{
        AppMediaPlayer_EngineVideo *pVEngine = (AppMediaPlayer_EngineVideo*)m_pEngineMain->GetEngine(APP_MEDIA_MODE_VIDEO);
		m_strOSDChangeTime = info;
	 	QString timeString = m_pMainUtil->convertDuration(pVEngine->GetVideoController()->PlayPosition());
	    info += QString("("+timeString+")");
	}
	else
	{
		m_updatePositionTimer.stop(); //added by edo.lee 2013.03.19
	}

        //MP_LOG <<m_pOSDView<< "OSD String : " << info << LOG_ENDL;
	//remove by edo.lee 2013.07.07
    //if((m_device==MEDIA_SOURCE_AUX1||m_device==MEDIA_SOURCE_AUX2) ||		//added by hyochang.ryu 20130522 for AUX OSD
	//    (m_pOSDView && fileName.isEmpty() == false && fileName.isNull() == false && fileName != " "))
	//{
		//m_pOSDView->setWindowTitle("@OP^CLEAR_AV");//modified by edo.lee 2013.03.14
	    //m_pOSDView->setWindowTitle(info);
	    m_BackupOSDDevice = m_device;// added by yongkyun.lee 20130626 for : ITS 176114
	    //OSDWriter::SendOSD(info); //modified by edo.lee 2013.04.04
		//QCoreApplication::flush(); //added by edo.lee 2013.01.14 
	//}
	if(!m_bTuneMode) //removed by edo.lee 2013.02.05
		m_updatePositionTimer.start(5000);
}
*/
/*
void ExternalNotifier::DisplayAudioTrackOSD()
{
    if(m_device >= MEDIA_SOURCE_MAX) return; // added by kihyung 2013.07.17 for ISV 86690
    
    MP_LOG << "m_device: " << m_device << LOG_ENDL;

	if(IsAvModeFromSharedMemory()== false)
	{
		MP_LOG<< "current is other mode.. return" << LOG_ENDL;
		//return;
	}

    //m_pOSDView = m_pEngineMain->GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN); //delete by ys 2013.09.30 //added by edo.lee 2012.12.08 for new statusbar

    //if ( m_device >= MEDIA_SOURCE_MAX ) // removed by kihyung 2013.07.
    //    return;
	//remove by edo.lee 2013.04.26
	//if(m_pEngineMain->getCloneState() != CLONE_OFF && m_pEngineMain->getPlayInfoDisplay() != DISPLAY_FRONT_REAR) {
    //    return;//added by edo.lee 2013.04.10
	//}
    //{ added by yongkyun.lee 20130702 for :  NO CR - None AV OSD
    //if(!m_pEngineMain->GetNotifyAVModeState())
    //    return ;
    //} added by yongkyun.lee 20130702
    //added by edo.lee 2013.07.07
     if((m_device!=MEDIA_SOURCE_AUX1||m_device!=MEDIA_SOURCE_AUX2||m_device!=MEDIA_SOURCE_BLUETOOTH) &&		
	 ( m_devices[m_device].m_title.isEmpty() && m_devices[m_device].m_title.isNull() &&  m_devices[m_device].m_title == " "&&  m_devices[m_device].m_title == ""))
 	{
 		MP_LOG<<"title is null..." << LOG_ENDL;
		return;
 	}
	//added by edo.lee 2013.07.07

    // modified by Dmitry Bykov 11.04.2013 for ISV79681
    QString info("");
    AppMediaPlayer_EngineAudio* pAudioEngine = dynamic_cast<AppMediaPlayer_EngineAudio*>(m_pEngines[APP_MEDIA_MODE_AUDIO]);
    // { added by cychoi 2013-07-15 for Prevent fix
    if(pAudioEngine == NULL)
        return;
    // } added by cychoi 2013-07-15
	AppMediaPlayer_Controller_Audio* pAudioController = dynamic_cast<AppMediaPlayer_Controller_Audio*>(pAudioEngine->GetController());
	    // { added by cychoi 2013-07-15 for Prevent fix
    if(pAudioController == NULL)
        return;
    // } added by cychoi 2013-07-15
    if ((m_OSDTreckSpeed &&  (pAudioController->isListView() || pAudioEngine->isMltState())))
		//|| pAudioController->isListView())//added by edo.lee 2013.07.12
    {
        info.append("@OSD:FR^MEDIA^INFO^");
    }
    else
    {
        info.append("@OSD:"+GetOSDDispString()+"^MEDIA^INFO^");
    }
    // modified by Dmitry Bykov 11.04.2013 for ISV79681

    switch( m_device )
    {
    case MEDIA_SOURCE_JUKEBOX:		
    	info += "JUKEBOX^"; 
    	info += "JUKEBOX^MUSIC^";
    	break;

    case MEDIA_SOURCE_USB1:
    case MEDIA_SOURCE_USB2:		
    	info += "USB^"; 
    	info += "USB^MUSIC^";
    	break;
    //{ added by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
    //case MEDIA_SOURCE_MP3:
    //case MEDIA_SOURCE_CDDA:
    case MEDIA_SOURCE_CDDA:
        if( !pAudioEngine->getGracenoteIndexingStatus(MP::DISC) && !GetCDDAMataInfo())
            return;
    case MEDIA_SOURCE_MP3:
    //} added by yongkyun.lee 20130416
    case MEDIA_SOURCE_DVDAUDIO:		
    	info += "DISC^";	
    	info += "DISC^MUSIC^";
    	break;

    case MEDIA_SOURCE_IPOD1:
    case MEDIA_SOURCE_IPOD2:		
    	info += "IPOD^";	
    	info += "iPod^MUSIC^"; //modified by edo.lee 2013.04.16
    	break;

        //{ added by hyochang.ryu 20130522 for AUX OSD
    //{deleted by yungi 2012.12.27 for ITS 150749
    case MEDIA_SOURCE_AUX1:
    case MEDIA_SOURCE_AUX2:		
    	info += "AUX^"; 
    	info += "AUX^MUSIC^";
    	break;
     //} added by hyochang.ryu 20130522 for AUX OSD
   //} deleted by yungi 2012.12.27 for ITS 150749

    case MEDIA_SOURCE_BLUETOOTH:		
    	info += "BLUETOOTH^";	
    	info += "BLUETOOTH^MUSIC^";
    	break;

    default:
    	break;
    }

    //{ added by hyochang.ryu 2013.05.09 for DHMM4_#23 ==> ISV 73726 rollback
//       	if(MEDIA_SOURCE_BLUETOOTH == m_device) 
// 		m_devices[m_device].m_title = "Streaming...";
    //} added by hyochang.ryu 2013.05.09 for DHMM4_#23
    //info += QString(m_devices[m_device].m_title+"^");		
	//added by edo.lee 2013.05.27
    QFont mFont;
    mFont.setFamily("NewHDR");
	QFontMetrics fontMetrics(mFont);
    QString sTitle = m_devices[m_device].m_title = fontMetrics.elidedText(m_devices[m_device].m_title, Qt::ElideRight, 300);
	//added by edo.lee 2013.05.27

    //{ added by hyochang.ryu 20130716
    if(m_device==MEDIA_SOURCE_BLUETOOTH&&!pAudioController->GetBTAvrcpAbove13())
        sTitle = "Phone Music";
    //} added by hyochang.ryu 20130716
	info += AVP_ENCODE_OSD_FORMAT_RULE1( AVP_ENCODE_OSD_FORMAT_RULE2( sTitle ))+"^"; //modified by edo.lee 20163.03.23
	
    //{ modified by yongkyun.lee 2013-07-15 for : ITS 179776
    MP_LOG << "playmode: " << m_devices[m_device].m_playingRandomMode <<  m_devices[m_device].m_playingRepeatMode << LOG_ENDL;
    if(m_devices[m_device].m_playingScanMode == SCANFOLDER || 
       m_devices[m_device].m_playingScanMode == SCANFILE )
           info += "(SF)^";
    else if(m_devices[m_device].m_playingScanMode == SCANALL )
               info += "(SE)^";

    if(m_devices[m_device].m_playingRandomMode == RANDOMFILE || 
       m_devices[m_device].m_playingRandomMode == RANDOMFOLDER )
           info += "(SH)^";

    if(m_devices[m_device].m_playingRepeatMode == REPEATFILE )
           info += "(RP1)^";
    else if(m_devices[m_device].m_playingRepeatMode == REPEATFOLDER )
           info += "(RPF)^";
    //} modified by yongkyun.lee 2013-07-15 

    m_strOSDChangeTime = info;

    //removed by junam 2013.07.11 for ITS179327
    //{added by junam 2013.07.01 for ITS176989
    //if(m_device == MEDIA_SOURCE_IPOD1 || m_device == MEDIA_SOURCE_IPOD2)
    //{
        //IPod may not start from 00:00 position.
        //If it happen, m_position is not correct position at this moment.
    //} //}added by junam
    //else
    //}removed by junam
    if(m_device < MEDIA_SOURCE_AUX1) 	//modified by hyochang.ryu 20130522 for AUX OSD
    {
        QString sec;
        sec = m_pMainUtil->convertDurationAudio(m_devices[m_device].m_position);
        info += QString("("+sec+")");
    }
    MP_LOG << "OSD String : "<< info << LOG_ENDL;
    
    //{ modified by yungi 2013.04.30 for ITS 0165494
    //if(m_pOSDView && m_devices[m_device].m_title.isEmpty() == false && m_devices[m_device].m_title.isNull() == false && m_devices[m_device].m_title != " ")
    //if((m_device==MEDIA_SOURCE_AUX1||m_device==MEDIA_SOURCE_AUX2||m_device==MEDIA_SOURCE_BLUETOOTH) ||		//20130612 BT "Unknown" //added by hyochang.ryu 20130522 for AUX OSD
	// ( m_devices[m_device].m_title.isEmpty() == false && m_devices[m_device].m_title.isNull() == false &&  m_devices[m_device].m_title != " "&&  m_devices[m_device].m_title != ""))	//modified by edo.lee 2013.07.06
    //{	
    	//m_pOSDView->setWindowTitle("@OP^CLEAR_AV"); //modified by edo.lee 2013.03.14
        //m_pOSDView->setWindowTitle(info);
        //OSDWriter::SendOSD(info); //modified by edo.lee 2013.04.04
		//QCoreApplication::flush(); //added by edo.lee 2013.01.14 
    //}
	if(MEDIA_SOURCE_BLUETOOTH != m_device )
	    m_updatePositionTimer.start(5000);
}
*/
//{ added by hyochang.ryu 20130716
/*  //ys-20131007 is not used
void ExternalNotifier::DisplayCustomOSD(QString sTitle)
{
    MP_LOG << "m_device: " << m_device << LOG_ENDL;

    if(m_device >= MEDIA_SOURCE_MAX) return; // added by kihyung 2013.07.17 for ISV 86690

    // modified by Dmitry Bykov 11.04.2013 for ISV79681
    QString info("");
    AppMediaPlayer_EngineAudio* pAudioEngine = dynamic_cast<AppMediaPlayer_EngineAudio*>(m_pEngines[APP_MEDIA_MODE_AUDIO]);
    if(pAudioEngine == NULL) return; // added by kihyung 2013.08.01 for static test
    AppMediaPlayer_Controller_Audio* pAudioController = dynamic_cast<AppMediaPlayer_Controller_Audio*>(pAudioEngine->GetController());
    if(pAudioController == NULL) return; // added by kihyung 2013.08.01 for static test    
    //if(!pAudioEngine||!pAudioController)        return; // removed by kihyung 2013.08.01 for static test    
    
    if ((m_OSDTreckSpeed && (pAudioController->isListView() || pAudioEngine->isMltState())))
        info.append("@OSD:FR^MEDIA^INFO^");
    else
        info.append("@OSD:"+GetOSDDispString()+"^MEDIA^INFO^");

    switch( m_device )
    {
    case MEDIA_SOURCE_JUKEBOX:		
    	info += "JUKEBOX^";     	info += "JUKEBOX^MUSIC^";
    	break;

    case MEDIA_SOURCE_USB1:    case MEDIA_SOURCE_USB2:		
    	info += "USB^";     	info += "USB^MUSIC^";
    	break;
    case MEDIA_SOURCE_CDDA:
        if( !pAudioEngine->getGracenoteIndexingStatus(MP::DISC) && !GetCDDAMataInfo())
            return;
    case MEDIA_SOURCE_MP3:    case MEDIA_SOURCE_DVDAUDIO:		
    	info += "DISC^";	    	info += "DISC^MUSIC^";
    	break;

    case MEDIA_SOURCE_IPOD1:    case MEDIA_SOURCE_IPOD2:		
    	info += "IPOD^";	    	info += "iPod^MUSIC^"; 
    	break;

    case MEDIA_SOURCE_AUX1:    case MEDIA_SOURCE_AUX2:		
    	info += "AUX^";     	info += "AUX^MUSIC^";
    	break;

    case MEDIA_SOURCE_BLUETOOTH:		
    	info += "BLUETOOTH^";	   	info += "BLUETOOTH^MUSIC^";
    	break;

    default:
    	break;
    }

    QFont mFont;
    mFont.setFamily("NewHDR");
	QFontMetrics fontMetrics(mFont);
    //QString sTitle = m_devices[m_device].m_title = fontMetrics.elidedText(m_devices[m_device].m_title, Qt::ElideRight, 300);
	info += AVP_ENCODE_OSD_FORMAT_RULE1( AVP_ENCODE_OSD_FORMAT_RULE2( sTitle ))+"^"; //modified by edo.lee 20163.03.23
	
    //{ modified by yongkyun.lee 2013-07-15 for : ITS 179776
    MP_LOG << "playmode: " << m_devices[m_device].m_playingRandomMode <<  m_devices[m_device].m_playingRepeatMode << LOG_ENDL;
    if(m_devices[m_device].m_playingScanMode == SCANFOLDER ||m_devices[m_device].m_playingScanMode == SCANFILE )
           info += "(SF)^";
    else if(m_devices[m_device].m_playingScanMode == SCANALL )
           info += "(SE)^";
    if(m_devices[m_device].m_playingRandomMode == RANDOMFILE || m_devices[m_device].m_playingRandomMode == RANDOMFOLDER )
           info += "(SH)^";

    if(m_devices[m_device].m_playingRepeatMode == REPEATFILE )
           info += "(RP1)^";
    else if(m_devices[m_device].m_playingRepeatMode == REPEATFOLDER )
           info += "(RPF)^";
    //} modified by yongkyun.lee 2013-07-15 

    m_strOSDChangeTime = info;

    if(m_device < MEDIA_SOURCE_AUX1) 	//modified by hyochang.ryu 20130522 for AUX OSD
    {
        QString sec;
        sec = m_pMainUtil->convertDurationAudio(m_devices[m_device].m_position);
        info += QString("("+sec+")");
    }
    MP_LOG << "OSD String : "<< info << LOG_ENDL;
    
        //OSDWriter::SendOSD(info); //modified by edo.lee 2013.04.04

	if(MEDIA_SOURCE_BLUETOOTH != m_device )
	    m_updatePositionTimer.start(5000);
}
*/
//} added by hyochang.ryu 20130716
/*
void ExternalNotifier::DisplayAudioInfo_Tune(QString title, QString number)
{
	MP_LOG << "m_device: " << m_device << LOG_ENDL;
    if ( m_device >= MEDIA_SOURCE_MAX )
        return;
	//added by edo.lee 2013.07.11
    if((m_device!=MEDIA_SOURCE_AUX1||m_device!=MEDIA_SOURCE_AUX2||m_device!=MEDIA_SOURCE_BLUETOOTH) &&		
	( m_devices[m_device].m_title.isEmpty() && m_devices[m_device].m_title.isNull() &&  m_devices[m_device].m_title == " "&&  m_devices[m_device].m_title == ""))
 	{
 		MP_LOG<<"title is null..." << LOG_ENDL;
		return;
 	}
    //added by edo.lee 2013.07.11
	//if(m_pEngineMain->getCloneState() != CLONE_OFF && m_pEngineMain->getPlayInfoDisplay() != DISPLAY_FRONT_REAR) return;//added by edo.lee 2013.04.10
	//remove by edo.lee 2013.04.26
    //_pOSDView = m_pEngineMain->GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN); //delete by ys 2013.09.30 //added by edo.lee 2012.12.08 for new statusbar
	QString info = "";
	//added by edo.lee 2013.07.12 
	AppMediaPlayer_EngineAudio* pAudioEngine = dynamic_cast<AppMediaPlayer_EngineAudio*>(m_pEngines[APP_MEDIA_MODE_AUDIO]);
    if(pAudioEngine == NULL) return; // added by kihyung 2013.07.16 for STATIC 23541
	AppMediaPlayer_Controller_Audio* pAudioController = dynamic_cast<AppMediaPlayer_Controller_Audio*>(pAudioEngine->GetController());
    if(pAudioController == NULL) return; // added by kihyung 2013.07.16 for STATIC 23541
	MP_LOG<<"pAudioController->isListView() "<<pAudioController->isListView() << LOG_ENDL;
    if(pAudioController->isListView()) 
		 info = "@OSD:FR^MEDIA^INFO_DIAL^" ;
	//added by edo.lee 2013.07.12
	else
		  info = "@OSD:"+GetOSDDispString()+"^MEDIA^INFO_DIAL^" ;
		
	switch( m_device )
	{
		case MEDIA_SOURCE_JUKEBOX:		
			info += "JUKEBOX^"; 
			info += "JUKEBOX^MUSIC^";
			break;

		case MEDIA_SOURCE_USB1:
		case MEDIA_SOURCE_USB2:		
			info += "USB^"; 
			info += "USB^MUSIC^";
			break;

		case MEDIA_SOURCE_MP3:
		case MEDIA_SOURCE_CDDA:
		case MEDIA_SOURCE_DVDAUDIO:		
			info += "DISC^";	
			info += "DISC^MUSIC^";
			break;

		case MEDIA_SOURCE_IPOD1:
		case MEDIA_SOURCE_IPOD2:		
			info += "IPOD^";	
			info += "iPod^MUSIC^";//modified by edo.lee 2013.04.16
			break;
		case MEDIA_SOURCE_BLUETOOTH:		
			info += "BLUETOOTH^";	
			info += "BLUETOOTH^MUSIC^";
			break;

		default:
			break;
	}

	//info += title+QString("^");
	//added by edo.lee 2013.05.27
	QFont mFont;
    mFont.setFamily("NewHDR");
	QFontMetrics fontMetrics(mFont);
    title = fontMetrics.elidedText(title, Qt::ElideRight, 300);
	info += AVP_ENCODE_OSD_FORMAT_RULE1( AVP_ENCODE_OSD_FORMAT_RULE2( title ) )+QString("^"); //modified by edo.lee 20163.03.23
	//info += QString("(")+number+QString(")^");// remove by edo.lee 2013.04.18

	m_updatePositionTimer.stop(); //added by edo.lee 2013.03.19
	MP_LOG << "OSD String : "<< info << LOG_ENDL;
        //if(m_pOSDView)//delete by ys 2013.09.30
        //{
	    //m_pOSDView->setWindowTitle(info);
	    //OSDWriter::SendOSD(info); //modified by edo.lee 2013.04.04
		//QCoreApplication::flush(); //added by edo.lee 2013.01.14 
        //}

	//m_updatePositionTimer.start(5000); //remove by edo.lee 2013.03.16
}
*/
/*
void ExternalNotifier::DisplayVideoTrackOSD() // modified by edo.lee 2013.03.04
{
    MP_LOG << LOG_ENDL;
    //if(m_OSDDisplay == false) return; removed by edo.lee 2013.01.03 OSD issue
    if(m_pEngineMain->IsVideoModeEnterd(DISPLAY_FRONT)
		|| m_pEngineMain->IsVideoModeEnterd(DISPLAY_REAR)) //added by edo.lee 2013.06.13
    	{
		    DisplayVideoInfo();
		    m_OSDDisplay = false;
    	}
}
*/
//{ modified by yongkyun.lee 20130705 for : ITS 178308
/*
void ExternalNotifier::DisplayDVDMenuOSD() 
{
    MP_LOG << LOG_ENDL;

    if( m_pEngineMain->IsVideoModeEnterd(DISPLAY_FRONT) || 
	    m_pEngineMain->IsVideoModeEnterd(DISPLAY_REAR)) 
    {
	    QString info = "";	
        //{ modified by yongkyun.lee 2013-07-06 for : ITS 178575
        if(m_bTuneMode)
        {
            info ="@OSD:FR";
            info += "^MEDIA^INFO^" ;	
        }
        else
        {
        	info ="@OSD:"+GetOSDDispString()+"^MEDIA^INFO^" ;	
        }
        //} modified by yongkyun.lee 2013-07-06 
        info += "DISC^";    
        info += "DISC^VIDEO^";
        info += QString( QApplication::translate("main", "STR_MEDIA_SELECT_DISK_MENU")+"^");
        //OSDWriter::SendOSD(info); //modified by edo.lee 2013.04.04
    }
}
*/
//} modified by yongkyun.lee 20130705 

// added by edo.lee 2013.02.18
/* //ys-20131007 is not used
void ExternalNotifier::DisplayVideoOSDonDevice(int device)
{
    if(device >= MEDIA_SOURCE_MAX) return; // added by kihyung 2013.07.17 for ISV 86690
    
	m_OSDDisplay = false;
	QString info = "";	
		if(m_bTuneMode && !m_bTuneSameFile)
		{			
			if(m_pEngineMain->getIsFROnTune())
			{			
				info = "@OSD:FR^MEDIA^INFO_DIAL^";
			}
			else
			{		
				//remove by edo.lee 2013.04.26
				//if(m_pEngineMain->getCloneState() == CLONE_OFF || m_pEngineMain->getPlayInfoDisplay() == DISPLAY_FRONT_REAR)
					info = "@OSD:"+GetOSDDispString()+"^MEDIA^INFO_DIAL^";
			}
		}
		else
		{
			if(m_pEngineMain->getIsFROnTune()) 
			{			
				info = "@OSD:FR^MEDIA^INFO^";
			}
			else{		
				//remove by edo.lee 2013.04.26
				//if(m_pEngineMain->getCloneState() == CLONE_OFF || m_pEngineMain->getPlayInfoDisplay() == DISPLAY_FRONT_REAR) 
					info ="@OSD:"+GetOSDDispString()+"^MEDIA^INFO^" ;
			}
		}		

    QString folderName = m_devices[device].m_folderName;
    QString fileName = m_devices[device].m_title;
	
    //setOSDFileFolder(fileName, folderName);
	fileName = AVP_ENCODE_OSD_FORMAT_RULE1( AVP_ENCODE_OSD_FORMAT_RULE2	(fileName)); 
	switch(device)
	{
		case MEDIA_SOURCE_JUKEBOX:
		{
		    info += "JUKEBOX^"; 
			info += "JUKEBOX^VIDEO^";
			info += QString(fileName+"^");
		    break;
		}
		case MEDIA_SOURCE_USB1:
		case MEDIA_SOURCE_USB2:
		{
		    info += "USB^"; 
			info += "USB^VIDEO^";
			info += QString(fileName+"^");
		    break;
		}
		case MEDIA_SOURCE_DVDVIDEO:
        {
            info += "DVD^";    
            info += "DVD^VIDEO^";
            //info += QString(folderName +" "+ fileName+"^");
            info += QString(QApplication::translate("main", "STR_MEDIA_CHAPTER") + " " +fileName+"^");
            break;
        }
		case MEDIA_SOURCE_VCD:
		{
		    info += "DISC^";	
			info += "DISC^VIDEO^";
			info += QString( QApplication::translate("main", "STR_MEDIA_TRACK")+fileName+"^");
		    break;
		}
		default:
		{
		    MP_LOG << "Unsupported source." << LOG_ENDL;
		    return;
		}
	}

	if(!m_bTuneMode)
	{
         //{ modified by yongkyun.lee 2013-07-15 for : ITS 179776 
         if(m_devices[device].m_playingRepeatMode == REPEATFILE)
				info += "(RP1)^";
         else if(m_devices[device].m_playingRepeatMode == REPEATFOLDER)
				info += "(RPF)^";

         if(m_devices[device].m_playingRepeatMode == RANDOMFILE ||
            m_devices[device].m_playingRepeatMode == RANDOMFOLDER )
				info += "(SH)^";
         //} modified by yongkyun.lee 2013-07-15 
	}	
    if(m_bTuneMode == false) // added by edo.lee 2013.03.16
    {
        AppMediaPlayer_EngineVideo *pVEngine = (AppMediaPlayer_EngineVideo*)m_pEngineMain->GetEngine(APP_MEDIA_MODE_VIDEO);
        m_strOSDChangeTime = info;
        QString timeString = m_pMainUtil->convertDuration(pVEngine->GetVideoController()->PlayPosition());
        info += QString("("+timeString+")");
    }
    else
    {
        m_updatePositionTimer.stop(); //added by edo.lee 2013.03.19
    }

    //MP_LOG <<m_pOSDView<< "OSD String : " << info << LOG_ENDL; //delete by ys 2013.09.30
    //if(m_pOSDView && fileName.isEmpty() == false && fileName.isNull() == false && fileName != " ")
    //{
        //OSDWriter::SendOSD(info); //modified by edo.lee 2013.04.04
    //}
    if(!m_bTuneMode) 
    {
    	m_updatePositionTimer.start(5000);
    }
}
*/

void ExternalNotifier::displayNoMediaFileOSD(MEDIA_SOURCE_T source )
{
	MP_LOG << LOG_ENDL;
         //m_pOSDView = m_pEngineMain->GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);//delete by ys 2013.09.30
	 QString   info  = "@OSD:FR^";
	if(source == MEDIA_SOURCE_IPOD1 
	|| source == MEDIA_SOURCE_IPOD2)
	{
		info += "MEDIA_INFO^WARN^IPOD^" ;
		info += "iPod^";		 //modified by edo.lee 2013.04.16
	//modified by edo.lee 2013.03.05
	}else if(source == MEDIA_SOURCE_USB1 
	|| source == MEDIA_SOURCE_USB2)
	{
		info += "MEDIA_INFO^WARN^USB^" ;
		info += "USB^";
	}else
	{
		info += "MEDIA_INFO^WARN^JUKEBOX^" ;
		info += "JUKEBOX^";
	}
	//modified by edo.lee
	 info += qApp->translate("main","STR_NO_PLAYABLE_FILE");
	 
        //if(m_pOSDView) //delete by ys 2013.09.30
        //{
	    //m_pOSDView->setWindowTitle(info);
	    //OSDWriter::SendOSD(info); //modified by edo.lee 2013.04.04
		//QCoreApplication::flush();
        //}
}

// modified by ravikanth 06--7-13 for ITS 0178087
/*
void ExternalNotifier::displayOSD(unsigned int nEventType)
{
    displayOSD(nEventType, DISPLAY_NONE);
}
// added by edo.lee
void ExternalNotifier::displayOSD(unsigned int nEventType, DISPLAY_T display)
{
   MP_LOG << nEventType << LOG_ENDL;

   //m_pOSDView = m_pEngineMain->GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN); //delete by ys 2013.09.30 //added by edo.lee 2012.12.08 for new statusbar

   QString   info  = "@OSD:FR^";
   DISPLAY_T nDisp = DISPLAY_AUTO;
   if(display == DISPLAY_FRONT) // modified by ravikanth 06--7-13 for ITS 0178087
   {
       info  = "@OSD:F^";
   }
   else if(display == DISPLAY_REAR)
   {
       info  = "@OSD:R^";
   }
   else
   {
       // no implementation
   }


   m_bAuxForBoot = false; // added by lssanh 2013.01.26 for ISV68519

   switch( nEventType )
   {
      case OSD_EVENT_NO_PLAYABLE_FILE:
          info += "MEDIA_INFO^WARN^JUKEBOX^" ;
         //added by edo.lee 2016.03.05
         if(m_device == MEDIA_SOURCE_USB1)
              info += "USB1^";
         else if(m_device == MEDIA_SOURCE_USB2)
             info += "USB2^";
         else
             info += "JUKEBOX^";
         //added by edo.lee 2016.03.05
         info += qApp->translate("main","STR_NO_PLAYABLE_FILE"); //modified by edo.lee 2013.01.25
         break;

      case OSD_EVENT_AUDIO_TRACK_INFO:
         if(!m_OSDDisplay )
            return;
         m_OSDDisplay= false;

         //DisplayAudioTrackOSD();
         nDisp = GetOSDDisplay();
         break;

      case OSD_EVENT_AUX_EXTERNAL_DEVICE_CONNECTED:
                 //{ added by yongkyun.lee 20130424 for : NO CR
                 //info += "MEDIA_INFO^WARN^AUX^" ;
                 info += "CONNECT_INFO^WARN^AUX^" ;
                 //} added by yongkyun.lee 20130424 
		 info += "AUX^";		 
		 info += qApp->translate("main","STR_EXTERNAL_DEVICE_CONNECTED");
         break;

      case OSD_EVENT_UNSUPPORTED_ACTION:
		 info += "MEDIA_INFO^ERR^AUX^" ;
		 info += "AUX^";		 
		 info +=  qApp->translate("main","STR_OPERATION_NOT_SUPPORTED");
         break;

      case OSD_EVENT_IPOD_SENDINGTAGS:
		 info += "MEDIA_INFO^WARN^IPOD^" ;
		 info += "iPod^";		 //modified by edo.lee 2013.04.16
		 info +=  qApp->translate("main","STR_IPOD_TAGGING_SENDING_TAGS_TO_IPOD");
         break;

      case OSD_EVENT_IPOD_MEMORYFULL:
		 info += "MEDIA_INFO^WARN^IPOD^" ;
		 info += "iPod^";		 //modified by edo.lee 2013.04.16
		 info +=  qApp->translate("main","STR_IPOD_TAGGING_MEMORY_FULL");
         break;

      case OSD_EVENT_IPOD_READING:
         //info += "MEDIA_INFO^WARN^IPOD^" ;  //suryanto.tan 2013.3.3: change from warn icon to loading
         info += "CONNECT_INFO^LOADING^IPOD^" ; //modified by edo.lee 2013.07.03
		 info += "iPod^";		 //modified by edo.lee 2013.04.16
		 info +=  qApp->translate("main","STR_IPOD_READING");
         break;
         
      case OSD_EVENT_IPOD_SUCCESS:
		 info += "MEDIA_INFO^WARN^IPOD^" ;
		 info += "iPod^";		//modified by edo.lee 2013.04.16 
		 info +=  qApp->translate("main","STR_IPOD_TAGGING_TAGGING_SUCCESSFUL");
         break;

      case OSD_EVENT_IPOD_FAILED:
		 info += "MEDIA_INFO^WARN^IPOD^" ;
		 info += "iPod^";		//modified by edo.lee 2013.04.16 
		 info +=  qApp->translate("main","STR_IPOD_TAGGING_TAGGING_FAILED");
         break;

      case OSD_EVENT_VIDEO_AV_OFF:
		 info += "COMMON^WARN^" ;		 
		 info += qApp->translate("main","STR_OSD_VIDEO_OFF");		 
         break;
         
      case OSD_EVENT_AUDIO_AV_OFF:
		 info += "COMMON^WARN^" ;		 
		 info +=  qApp->translate("main","STR_OSD_AUDIO_OFF");
         break;
         
      case OSD_EVENT_COPY_COMPLETED:
		 info += "MEDIA_INFO^WARN^JUKEBOX^" ;
		 info += "JUKEBOX^";
		 info +=  qApp->translate("main","STR_MEDIA_MNG_COPY_COMPLETED");
         break;
         
      case OSD_EVENT_DELETE_COMPLETED:
		 info += "MEDIA_INFO^WARN^JUKEBOX^" ;
		 info += "JUKEBOX^";
		 info +=  qApp->translate("main","STR_MEDIA_MNG_DELETE_COMPLETED");
         break;
         
       case OSD_EVENT_MOVE_COMPLETED:
		 info += "MEDIA_INFO^WARN^JUKEBOX^" ;
		 info += "JUKEBOX^";
		 info +=  qApp->translate("main","STR_OSD_MOVING_COMPLITED");
         break;
         
      case OSD_EVENT_IPOD_UNSUPPORTED:
		 info += "MEDIA_INFO^ERR^IPOD^" ;
		 info += "iPod^";//modified by edo.lee 2013.04.16
		 info +=  qApp->translate("main","STR_IPOD_UNSUPPORTED");
         break;
         
      case OSD_EVENT_IPOD_CONNECTION_ERROR:
		 info += "MEDIA_INFO^ERR^IPOD^" ;
		 info += "iPod^";//modified by edo.lee 2013.04.16
		 info +=  qApp->translate("main","STR_IPOD_CONNECTION_ERROR");
         break;
         
      case OSD_EVENT_ADISC_EJECTED:
          //{ modified by yongkyun.lee 2013-07-18 for : ITS 179198
          if(m_DeckOSDStatus == "C")
              info  = "@OSD:"+GetEjectDisplay()+"^"; //added by edo.lee 2013.03.11
          else
          //} modified by yongkyun.lee 2013-07-18 
          info  = "@OSD:"+m_DeckOSDStatus+"^"; //added by edo.lee 2013.03.11
                  info += "CONNECT_INFO^WARN^DISC^" ;
		  info += "DISC^";
		  info +=  qApp->translate("main","STR_OSD_EJECTED_DISC");
	  	  break;

      case OSD_EVENT_NO_INSERTED_DISC:
                  info += "CONNECT_INFO^WARN^DISC^" ;          
		  info += "DISC^";
		  info +=  qApp->translate("main","STR_OSD_NO_INSERTED_DISC");
          break;
      
      case OSD_EVENT_JUKEBOX_READING:
		  info += "MEDIA_INFO^WARN^JUKEBOX^" ;
		  info += "JUKEBOX^";
		  info +=  qApp->translate("main","STR_JUKEBOX_READING");
          break;

      case OSD_EVENT_AUX_VIDEO_INFO:
                //{added by yungi 2012.12.27 for ITS 150749
                  info += "MEDIA^INFO^AUX^";
                  info += "AUX Video^";
                  info += "VIDEO^";
                //}added by yungi 2012.12.27 for ITS 150749
          break;
          
      case OSD_EVENT_AUX_AUDIO_INFO:
                 info += "MEDIA^INFO^AUX^";
                 info += "AUX Audio^";
                 info += "MUSIC^";
          break;
          
      case OSD_EVENT_AUX_DISCONNECTED:
         //{ added by yongkyun.lee 20130520 for : NO CR : AUX OSD
         {
             //{ added by hyochang.ryu 20130724 for OSD repitition
    	      if(m_pEngineMain->GetNotifyAVModeState()) 
                 return;
             //} added by hyochang.ryu 20130724 for OSD repitition
             info.clear ();
             info = "@OSD:";
             bool isfg = static_cast <AppMediaPlayer_Controller_Audio*> (m_pEngines[APP_MEDIA_MODE_AUDIO]->GetController())->isForeground();

             if( m_device != MEDIA_SOURCE_AUX1 && m_device != MEDIA_SOURCE_AUX2 )		// 1 )
             {
                 info+="FR";
             }
             else if( isfg )
             {
                 if( m_pEngineMain->getCurrScreen() == DISPLAY_FRONT && ( m_device == MEDIA_SOURCE_AUX1 || m_device == MEDIA_SOURCE_AUX2) )
                 {
                     info+="F";
                 }
                 if( m_pEngineMain->getCurrScreen() == DISPLAY_REAR  && ( m_device == MEDIA_SOURCE_AUX1 || m_device == MEDIA_SOURCE_AUX2) )
                 {   
                     info+="R";
                 }
             }
             else
             {
                  return;
             }        
         }
         info+="^";
         //} added by yongkyun.lee 20130520 

         //{ added by yongkyun.lee 20130424 for : NO CR
         //info += "MEDIA_INFO^WARN^AUX^" ;
         info += "CONNECT_INFO^WARN^AUX^" ;        
         //} added by yongkyun.lee 20130424 
         info += "AUX^";
         info += qApp->translate("main","STR_AUX_DISCONNECTED");
         break;

      case OSD_EVENT_AUX_CONNECTED:
         //return;		//added by hyochang.ryu 20130617 no more (MODE)
         //{ modified by hyochang.ryu 20130710
         {
             info.clear ();
             info = "@OSD:";
             bool isfg = static_cast <AppMediaPlayer_Controller_Audio*> (m_pEngines[APP_MEDIA_MODE_AUDIO]->GetController())->isForeground();

             if( isfg )
             {
                 if( m_pEngineMain->getCurrScreen() == DISPLAY_FRONT && !GetAVOffMode() )
                 {
                     info+="F";
                 }
                 if( m_pEngineMain->getCurrScreen() == DISPLAY_REAR  && !GetAVOffMode() )
                 {   
                     info+="R";
                 }
             }
             else return;
         }
         info+="^";
         //{ added by yongkyun.lee 20130424 for : NO CR
         //info += "MEDIA_INFO^WARN^AUX^" ;
         info += "CONNECT_INFO^WARN^AUX^" ;
         //} added by yongkyun.lee 20130424 
         info += "AUX^";
         info += qApp->translate("main","STR_AUX_CONNECTED");
         //} modified by hyochang.ryu 20130710
         break;

      //{ added by hyochang.ryu 20130704		 
      case OSD_EVENT_BT_UNSUPPORTED:
		 info += "MEDIA_INFO^ERR^BLUETOOTH^" ;
		 info += "BLUETOOTH^";		 
		 info +=  qApp->translate("main","STR_OPERATION_NOT_SUPPORTED");
         break;		//added by hyochang.ryu 20130617 no more (MODE)
      //} added by hyochang.ryu 20130704		 

      default:
         break;
   }
	
	MP_LOG<<"OSD String:"<<info << LOG_ENDL;
	if(info == "@OSD:FR^")return; //added by edo.lee 2012.12.16
        //if(m_pOSDView) //delete by ys 2013.09.30
        //{
	    //m_pOSDView->setWindowTitle(info);
	    //OSDWriter::SendOSD(info); //modified by edo.lee 2013.04.04
		//QCoreApplication::flush(); //added by edo.lee 2013.01.14 
        //}
}
*/
// removed by cychoi 2013.08.17 for code arrangement

// { added by yongkyun.lee  2012.10.17  for Current file is white color
void ExternalNotifier::onTuneSameFile(bool val)
{
   m_bTuneSameFile = val;
}
// } added by  yongkyun.lee 

// Updates OSD with video playback information.
/*
void ExternalNotifier::updateVideoOSD()
{
}
*/
void ExternalNotifier::updateVideoFileManager()
{
    if(m_device >= MEDIA_SOURCE_MAX) return; // added by kihyung 2013.07.17 for ISV 86690
        
    // { modified by kihyung 2013.07.10 for ITS 0179120
    //AppMediaPlayer_EngineVideo *pVEngine = (AppMediaPlayer_EngineVideo*)m_pEngineMain->GetEngine(APP_MEDIA_MODE_VIDEO); //remove by edo.lee 2014.01.19 STATIC TEST
    // added by Dmitry 10.03.13 for ITS0187937
    AppMediaPlayer_Video_ControllerFS *m_pControllerFS = static_cast<AppMediaPlayer_Video_ControllerFS*>(m_pVideoCtrl->getCurrentController());

 	// { modified by Sergey 04.10.2013 for ITS#193837	
    if(m_pControllerFS != NULL)
    {
        QString filePath = m_pControllerFS->getFSModel()->playingUrl();
        m_devices[m_device].m_fullFolderName = filePath.left(filePath.lastIndexOf('/')); // modified by Sergey 04.10.2013 for ITS#193837
        m_pSharedVideoData->setStringData(STRING_DATA_TYPE_PLAYING_FILE, filePath);
    }
     // } modified by Sergey 04.10.2013 for ITS#193837	

    // { commented by Sergey 23.11.2013 for ITS#210658
//    if(pVEngine) {
//        if(pVEngine->isVideoListView() == VIDEO_DISPLAY_MAX) { // modified by kihyung 2013.07.15 for ITS 0179120
//            MP_LOG << "Skip File Manager Update" << LOG_ENDL;
//            return;
//        }
//    }
    // } commented by Sergey 23.11.2013 for ITS#210658
    
    AppFileManagerStartParameter fmParams; // modified by Dmitry 25.07.13

    MP_LOG<<"current file "<<m_devices[m_device].m_title << LOG_ENDL;
    switch(m_device)
    {
    case MEDIA_SOURCE_JUKEBOX:
    case MEDIA_SOURCE_USB1:
    case MEDIA_SOURCE_USB2:
        fmParams.startState = ( m_autoTrackChangeIndex == -1 ) ? eVideoFileChanged : eVideoFileAutoChanged;
        break;
    default:
        MP_LOG << "Applicable only for JUKEBOX and USB" << LOG_ENDL;
        return;
    }

	 // removed by Sergey 04.10.2013 for ITS#193837	

// modified by Dmitry 25.07.13 
    snprintf(fmParams.currentPlayFileName, sizeof(fmParams.currentPlayFileName), m_devices[m_device].m_title.toUtf8().constData());
    snprintf(fmParams.folderPath, sizeof(fmParams.folderPath), m_devices[m_device].m_fullFolderName.toUtf8().constData());
// modified by Dmitry 25.07.13
    MP_LOG<<"m_devices[m_device].m_title:"<< m_devices[m_device].m_title << LOG_ENDL;
    MP_LOG<<"m_devices[m_device].m_fullFolderName:"<< m_devices[m_device].m_fullFolderName << LOG_ENDL;
    
    Event fmstate( EVT_POST_EVENT, m_pEngineMain->GetThisAppId(), APP_FMGR, EVT_FM_LAUNCH_STATE, QByteArray((char *)&fmParams, sizeof(fmParams)));
    m_pEngineMain->notifyUISH( fmstate );
    // } modified by kihyung 2013.07.10 for ITS 0179120
    m_autoTrackChangeIndex = -1;
}

void ExternalNotifier::updateAudioFileManager()
{
    // removed by cychoi 2013.08.17 for code arrangement
}

// removed by cychoi 2013.08.17 for code arrangement

void ExternalNotifier::onDeviceAttached(MEDIA_SOURCE_T source, MEDIA_TYPE_T type, bool autoplay)
{
    Q_UNUSED(autoplay)
    MP_LOG << source << type << LOG_ENDL;

    // { commented by cychoi 2014.09.24 for DH.EU.SOP issue in Cluster, AUX icon is indicated on iPod mode // { added by wspark 2012.11.07 for sending aux opstate.
    //if((type == MEDIA_TYPE_MUSIC || type == MEDIA_TYPE_VIDEO) && (source == MEDIA_SOURCE_AUX1 || source == MEDIA_SOURCE_AUX2))
    //if((type == MEDIA_TYPE_MUSIC && (source == MEDIA_SOURCE_AUX1 || source == MEDIA_SOURCE_AUX2)) ||
    //   (type == MEDIA_TYPE_VIDEO && (source == MEDIA_SOURCE_AUX1 || source == MEDIA_SOURCE_AUX2)))
    //{
        // { added by yungi 2012.12.14 for CR16059
        // { modified by cychoi 2013.09.03 for cluster update on device changed
        //m_bAuxForBoot = false;
        // } modified by cychoi 2013.09.03
    //    m_device = MEDIA_SOURCE_AUX1;
        //} added by yungi 2012.12.14 for CR16059
    //    quint64 data = clusterStates[type][source];
    //    SendClusterData_OpStateOnly(data);
    //}
    // } commented by cychoi 2014.09.24 // } added by wspark

    if (m_device == MEDIA_SOURCE_MAX && type == MEDIA_TYPE_MUSIC)
    {
        // { modified by cychoi 2013.09.03 for cluster update on device changed
        m_device = source;
        //m_activeDevice = source;
        // } modified by cychoi 2013.09.03
    }

    // {  added by cychoi 2013.11.18 for ITS 208698 MP3 Play Info on Cluster 
    if( source == MEDIA_SOURCE_MP3 || source == MEDIA_SOURCE_CDDA )
    {
        m_bDirTreeFetched = false;
    }
    // }  added by cychoi 2013.11.18

    UpdateSDP_DeviceStatus( source,  type, true );
}

void ExternalNotifier::onDeviceRemoved(MEDIA_SOURCE_T source, MEDIA_TYPE_T type)
{
    MP_LOG << source << type << LOG_ENDL;
/* //ys -20130930 is not used
    // { added by lssanh 2013.03.30 Sanity AF006
    if((type == MEDIA_TYPE_MUSIC || type == MEDIA_TYPE_VIDEO) && (source == MEDIA_SOURCE_AUX1 || source == MEDIA_SOURCE_AUX2))
    //if(//(type == MEDIA_TYPE_MUSIC) && 						//deleted by hyochang.ryu 20130621 for AUX disconn OSD
	//	(source == MEDIA_SOURCE_AUX1 || source == MEDIA_SOURCE_AUX2))
    {
        displayOSD(OSD_EVENT_AUX_DISCONNECTED);
    }
    // } added by lssanh 2013.03.30 Sanity AF006
*/
    // Main menu update:
    HomeScreenDef::MEDIA_SOURCE_T device = TranslateToHomeSource(source);
    if (device < HomeScreenDef::MEDIA_SOURCE_MAX)
    {
        Event event(EVT_POST_EVENT, m_pEngineMain->GetThisAppId(), APP_MAIN_MENU,
                    HomeScreenDef::DEVICE_DISCONNECTED,
                    QByteArray((char*)&device, sizeof(device))); // modified by lssanh 2013.02.21 IPC payload
        m_pEngineMain->notifyUISH(event); // modified by Sergey for SwapDC
    }

    UpdateSDP_DeviceStatus( source,  type, false );
	resetMediaEnabled(source); //added by edo.lee 2013.01.23

    // { added by cychoi 2013.11.18 for ITS 208698 MP3 Play Info on Cluster 
    if( m_device == MEDIA_SOURCE_MP3 || m_device == MEDIA_SOURCE_CDDA )
    {
        m_bDirTreeFetched = false;
        ResetDeviceData( m_device );
    }
    // } added by cychoi 2013.11.18
}

//{changed by junam 2013.03.26 for ISV72425
//void ExternalNotifier::onTuneSearchChanged( QString m_coverart, QString m_artist, QString m_song, QString m_album, QString m_folder, QString m_genre, QString m_composer, QString m_countInfo ,  bool m_samefile )
void ExternalNotifier::onTuneSearchChanged( QString m_coverart, QString m_artist, QString m_song, QString m_album, QString m_folder, QString m_countInfo ,  bool m_samefile )
{//}changed by junam
    MP_LOG << " title: " << m_song << " number: " << m_countInfo << "artist" << m_artist << LOG_ENDL;
    Q_UNUSED(m_coverart)
    Q_UNUSED(m_album)
    Q_UNUSED(m_folder)
    Q_UNUSED(m_samefile)
    
	m_SearchTitle = m_song; //added by edo.lee 2013.05.08
    //DisplayAudioInfo_Tune(m_song, m_countInfo);
}

// { added by junam 2012.11.14 for tune update osd
void ExternalNotifier::onTuneCounterChanged(QString count)
{
    Q_UNUSED(count)
    //MP_LOG << " count: " << count << LOG_ENDL;
    //m_tuneCounter = count;
}

//{changed by junam 2013.03.26 for ISV72425
//void ExternalNotifier::onTuneMetaDataChanged( QString artist, QString song, QString album, QString folder, QString genre, QString composer, bool isColor )
void ExternalNotifier::onTuneMetaDataChanged( QString artist, QString song, QString album, QString folder, bool isColor )
{//}changed by junam
    MP_LOG << " artist: " << artist << " song: " << song << " album: " << album << LOG_ENDL;
    Q_UNUSED(folder)
    Q_UNUSED(isColor)
	m_SearchTitle = song; //added by edo.lee 2013.05.08
        //DisplayAudioInfo_Tune(song, m_tuneCounter);
}
// } added by junam

// { modified by yongkyun.lee@lge.com 2012.07.14  CR 11922
//void ExternalNotifier::onPlayingModeChanged( int nRandom, int nRepeat, int nScan )
void ExternalNotifier::onPlayingModeChanged( int nRandom, int nRepeat, int nScan , bool nNoOSD)
// } modified by yongkyun.lee@lge.com
{
   MP_LOG << "random: " << nRandom << " repeat: " << nRepeat << " scan: " << nScan << LOG_ENDL;
   Q_UNUSED(nNoOSD)
   if ( m_device < MEDIA_SOURCE_MAX )
   {
 //{ Modified by Radhakrushna CR#13517 2012007 
         m_devices[m_device].m_playingRandomMode = nRandom; // modified by yongkyun.lee 2013-07-15 for : ITS 179776
         m_devices[m_device].m_playingRepeatMode = nRepeat; // modified by yongkyun.lee 2013-07-15 for : ITS 179776
         m_devices[m_device].m_playingScanMode   = nScan;   // modified by yongkyun.lee 2013-07-15 for : ITS 179776
 //} Modified by Radhakrushna CR#13517 2012007 
   }
}

// { removed by kihyung 2013.2.17
/*
// { added by Sergey for #16250
void ExternalNotifier::onSourceAvailableChanged(MEDIA_SOURCE_T source, MEDIA_TYPE_T type, bool status)
{
    // Video player
    if(type == MEDIA_TYPE_VIDEO)
    {
    	setVideoEnableMedia( source, status ? MEDIA_ENABLE:MEDIA_DISABLE); //added by edo.lee 2013.02.14
        UpdateMainMenu(source);
    }
}
// } added by Sergey for #16250.
*/
// } removed by kihyung 2013.2.17

//added by edo.lee 2013.01.23
void ExternalNotifier::resetMediaEnabled(MEDIA_SOURCE_T source)
{
	for(int i=0;i<MEDIA_TYPE_MAX;i++)
                m_MediaEnabled[i][source]=MEDIA_INIT;
}
//added by edo.lee 

void ExternalNotifier::onContentSize(MEDIA_SOURCE_T source, MEDIA_TYPE_T type, int count)
{
    // { modified by kihyung 2013.2.17
    MP_LOG << source << type << count << LOG_ENDL;

    /*
	if(type == MEDIA_TYPE_MUSIC)
		setMusicEnableMedia(source, count > 0 ? MEDIA_ENABLE:MEDIA_DISABLE);
	else if(type == MEDIA_TYPE_VIDEO)
		setVideoEnableMedia(source, count > 0 ? MEDIA_ENABLE:MEDIA_DISABLE);
	else if(type == MEDIA_TYPE_PHOTO)
		setPhotoEnableMedia(source, count > 0 ? MEDIA_ENABLE:MEDIA_DISABLE);
    
    UpdateMainMenu(source);
    */
    // } modified by kihyung 2013.2.17    
}

void ExternalNotifier::UpdateSDP_DeviceStatus(MEDIA_SOURCE_T source, MEDIA_TYPE_T type, bool isConnected)
{
    MP_LOG << source << type << isConnected   << LOG_ENDL;

    if(source == MEDIA_SOURCE_JUKEBOX )
    {
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_JUKEBOX_MUSIC_COUNT, m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_MUSIC));
        m_pSharedPhotoData->SetStatusDateByIndex(DATA_TYPE_JUKEBOX_IMAGE_COUNT, m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_PHOTO));
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_JUKEBOX_VIDEO_COUNT, m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_VIDEO));
    }
    else if (source == MEDIA_SOURCE_USB1 )
    {
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_USB1_MUSIC_COUNT,  m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_MUSIC) );
        m_pSharedPhotoData->SetStatusDateByIndex(DATA_TYPE_USB1_IMAGE_COUNT,  m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_PHOTO) );
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_USB1_VIDEO_COUNT,  m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_VIDEO));
    }
    else if ( source == MEDIA_SOURCE_USB2)
    {
         m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_USB2_MUSIC_COUNT, m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_MUSIC));
         m_pSharedPhotoData->SetStatusDateByIndex(DATA_TYPE_USB2_IMAGE_COUNT, m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_PHOTO));
         m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_USB2_VIDEO_COUNT,  m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_VIDEO));
    }
    else if( source == MEDIA_SOURCE_IPOD1)
    {
        if( isConnected == false )
        {
            m_pSharedAudioData->SetStatusDateByIndex( DATA_TYPE_IPOD1_MUSIC_COUNT , 0);
        }//{added by junam 2013.03.26 for ISV75917
        else
        {
            m_pSharedAudioData->SetStatusDateByIndex( DATA_TYPE_IPOD1_MUSIC_COUNT, m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_MUSIC)); //added by junam 2013.03.19 for ISV75917
        }
        //}added by junam 2013.03.26
        m_pSharedAudioData->SetStatusDateByIndex( DATA_TYPE_IPOD1_MEDIA_LOADING, isConnected ? MEDIA_DEVICE_SCANNING : MEDIA_DEVICE_UNAVAILABLE );
        //m_pSharedAudioData->SetStatusDateByIndex( DATA_TYPE_IPOD1_MUSIC_COUNT, m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_MUSIC)); //removed by junam 2013.03.26 for ISV75917
    }
    else if( source == MEDIA_SOURCE_IPOD2)
    {
        if( isConnected == false )
        {
            m_pSharedAudioData->SetStatusDateByIndex( DATA_TYPE_IPOD2_MUSIC_COUNT , 0);
        }//{added by junam 2013.03.26 for ISV75917
        else
        {
            m_pSharedAudioData->SetStatusDateByIndex( DATA_TYPE_IPOD2_MUSIC_COUNT, m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_MUSIC));
        }
        //}added by junam 2013.03.26
        m_pSharedAudioData->SetStatusDateByIndex( DATA_TYPE_IPOD2_MEDIA_LOADING, isConnected ? MEDIA_DEVICE_SCANNING : MEDIA_DEVICE_UNAVAILABLE );
        //m_pSharedAudioData->SetStatusDateByIndex( DATA_TYPE_IPOD2_MUSIC_COUNT, m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_MUSIC));//removed by junam 2013.03.26 for ISV75917
    }
    // { modified by kihyung 2013.1.7 ISV 68214
    else if ( source == MEDIA_SOURCE_MP3 || source == MEDIA_SOURCE_DVDAUDIO ||
              source == MEDIA_SOURCE_VCD || source == MEDIA_SOURCE_DVDVIDEO ||
              source == MEDIA_SOURCE_CDDA )
    {
        if( isConnected == false )
        {
            m_pSharedAudioData->SetStatusDateByIndex( DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_EMPTY );
            m_pSharedVideoData->SetStatusDateByIndex( DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_EMPTY );
            m_pSharedAudioData->SetStatusDateByIndex( DATA_TYPE_MAX_TRACK_NUM, 0 );
            m_pSharedVideoData->SetStatusDateByIndex( DATA_TYPE_MAX_TRACK_NUM, 0 );
        }
        /*
        else
        {
            m_pSharedAudioData->SetStatusDateByIndex ( DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
            m_pSharedVideoData->SetStatusDateByIndex ( DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
        }
        */
    }
    // } modified by kihyung 2013.1.7 ISV 68214   
    
	// { remove by edo.lee 2013.01.23
	//if(source == MEDIA_SOURCE_JUKEBOX || source == MEDIA_SOURCE_USB1 || source == MEDIA_SOURCE_USB2)
		//|| source == MEDIA_SOURCE_IPOD1 || source == MEDIA_SOURCE_IPOD2) // remove by edo.lee 2013.01.23
	//{
	//	m_pEngineMain->notifyEnableMedia(type ,source,  m_pMediaManager->IsSourceAvailable(source, type));
	//}
	//else remove by edo.lee 2013.01.23
	//{
	//	m_pEngineMain->notifyEnableMedia(type ,source, isConnected);
	//}
	// } added by edo.lee
}

// { modified by kihyung 2013.1.7 ISV 68214 
void ExternalNotifier::UpdateSDPMediaCount(MEDIA_SOURCE_T source, MEDIA_TYPE_T type, int count)
{
    DATA_TYPE_T key;
    MP_LOG << source << type << count << LOG_ENDL;

    if( source == MEDIA_SOURCE_IPOD1 || source == MEDIA_SOURCE_IPOD2 )
    {
        if (type == MEDIA_TYPE_MUSIC)
        {
            key = (source == MEDIA_SOURCE_IPOD1) ? DATA_TYPE_IPOD1_MUSIC_COUNT : DATA_TYPE_IPOD2_MUSIC_COUNT;
            m_pSharedAudioData->SetStatusDateByIndex(key, count);
        }
    }
    else if( source == MEDIA_SOURCE_JUKEBOX ||  source == MEDIA_SOURCE_USB1  ||  source == MEDIA_SOURCE_USB2 )
    {
        if (type == MEDIA_TYPE_PHOTO)
        {
           key = ( source == MEDIA_SOURCE_JUKEBOX) ? DATA_TYPE_JUKEBOX_IMAGE_COUNT : \
                 ((source == MEDIA_SOURCE_USB1 )   ? DATA_TYPE_USB1_IMAGE_COUNT : DATA_TYPE_USB2_IMAGE_COUNT);
           m_pSharedPhotoData->SetStatusDateByIndex(key, count);
        }
        else if (type == MEDIA_TYPE_MUSIC)
        {
            key = ( source == MEDIA_SOURCE_JUKEBOX) ? DATA_TYPE_JUKEBOX_MUSIC_COUNT : \
                  ((source == MEDIA_SOURCE_USB1 )   ? DATA_TYPE_USB1_MUSIC_COUNT : DATA_TYPE_USB2_MUSIC_COUNT);
            m_pSharedAudioData->SetStatusDateByIndex(key, count);
        }
        else if (type == MEDIA_TYPE_VIDEO)
        {
            key = ( source == MEDIA_SOURCE_JUKEBOX) ? DATA_TYPE_JUKEBOX_VIDEO_COUNT : \
                  ((source == MEDIA_SOURCE_USB1 )   ? DATA_TYPE_USB1_VIDEO_COUNT : DATA_TYPE_USB2_VIDEO_COUNT);
            m_pSharedVideoData->SetStatusDateByIndex(key, count);
        }
    }
}
// } modified by kihyung 2013.1.7 ISV 68214 

void ExternalNotifier::setEnableMedia(MEDIA_SOURCE_T source, MEDIA_TYPE_T playMediaType)
{
    MP_HIGH << source << playMediaType << LOG_ENDL;
    
    MODE_STATE_T modeState;
    
    if(playMediaType == MEDIA_TYPE_MUSIC) 
    {
        switch(source)
        {    
        case MEDIA_SOURCE_JUKEBOX: modeState = MODE_STATE_JUKEBOX_AUDIO;    break;
        case MEDIA_SOURCE_USB1:    modeState = MODE_STATE_USB1_AUDIO;       break;
        case MEDIA_SOURCE_USB2:    modeState = MODE_STATE_USB2_AUDIO;       break;
        case MEDIA_SOURCE_IPOD1:   modeState = MODE_STATE_IPOD1_AUDIO;      break;
        case MEDIA_SOURCE_IPOD2:   modeState = MODE_STATE_IPOD2_AUDIO;      break;
        default:
            return;
        }
    }
    else if(playMediaType == MEDIA_TYPE_VIDEO) {
        switch(source)
        {    
        case MEDIA_SOURCE_JUKEBOX: modeState = MODE_STATE_JUKEBOX_VIDEO;    break;
        case MEDIA_SOURCE_USB1 :   modeState = MODE_STATE_USB1_VIDEO;       break;
        case MEDIA_SOURCE_USB2:    modeState = MODE_STATE_USB2_VIDEO;       break;
        default:
            return;
        }
    }
    else if(playMediaType == MEDIA_TYPE_PHOTO) {
        switch(source)
        {    
        case MEDIA_SOURCE_JUKEBOX: modeState = MODE_STATE_NONE;             break;
        case MEDIA_SOURCE_USB1 :   modeState = MODE_STATE_NONE;             break;
        case MEDIA_SOURCE_USB2:    modeState = MODE_STATE_NONE;             break;
        default:
            return;
        }
    }
    else {
        modeState = MODE_STATE_NONE;
    }

    MEDIA_ENABLE_T eMEnable = m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_MUSIC)? MEDIA_ENABLE:MEDIA_DISABLE; 
    MEDIA_ENABLE_T eVEnable = m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_VIDEO)? MEDIA_ENABLE:MEDIA_DISABLE; 
	MEDIA_ENABLE_T ePEnable = m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_PHOTO)? MEDIA_ENABLE:MEDIA_DISABLE;
	
	m_pEngineMain->notifyEnableMedia(source, modeState, eMEnable, eVEnable, ePEnable);
    // UpdateSDP_DeviceStatus(source, playMediaType, m_pMediaManager->IsSourceAvailable(source, playMediaType)); // modified by ravikanth 18-04-13 // removed by kihyung 2013.5.6
}

// removed by cychoi 2013.08.17 for code arrangement

void ExternalNotifier::UpdateMainMenu(MEDIA_SOURCE_T source)
{
    Q_UNUSED(source)
    /*
    HOME_MEDIA_CONTENT_T content;

    content.device = TranslateToHomeSource(source);
    content.bAudio = m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_MUSIC);
    content.bVideo = m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_VIDEO);
    content.bPhoto = m_pMediaManager->IsSourceAvailable(source, MEDIA_TYPE_PHOTO);

    if (content.device < HomeScreenDef::MEDIA_SOURCE_MAX)
    {
        MP_LOG << "TASCAN -> UPDATE_CONTENT_STATUS: " <<  content.device << content.bAudio << content.bVideo << content.bPhoto << LOG_ENDL;
        
        QByteArray data((char*)&content, sizeof(content)); // modified by lssanh 2013.02.21 IPC payload

        Event event(EVT_POST_EVENT, m_pEngineMain->GetThisAppId(), APP_MAIN_MENU,
                    HomeScreenDef::UPDATE_CONTENT_STATUS, data);
        m_pEngineMain->NotifyUISH(event);
    }
    */
}

void ExternalNotifier::UpdateMainMenuAfterBoot()
{ 
    MP_LOG << LOG_ENDL;
    QTimer::singleShot(10000, this, SLOT(onUpdateMainMenuAfterBoot()));
}

HomeScreenDef::MEDIA_SOURCE_T
ExternalNotifier::TranslateToHomeSource(MEDIA_SOURCE_T source)
{
    switch (source)
    {
        case MEDIA_SOURCE_JUKEBOX:
            return HomeScreenDef::MEDIA_SOURCE_JBOX;
        case MEDIA_SOURCE_USB1:
            return HomeScreenDef::MEDIA_SOURCE_USB1;
        case MEDIA_SOURCE_USB2:
            return HomeScreenDef::MEDIA_SOURCE_USB2;
        case MEDIA_SOURCE_CDDA:
            return HomeScreenDef::MEDIA_SOURCE_CD_AUDIO;
        case MEDIA_SOURCE_MP3:
            return HomeScreenDef::MEDIA_SOURCE_CD_DATA;
        case MEDIA_SOURCE_DVDAUDIO:
            return HomeScreenDef::MEDIA_SOURCE_DVD_AUDIO;
        case MEDIA_SOURCE_VCD:
            return HomeScreenDef::MEDIA_SOURCE_VCD;
        case MEDIA_SOURCE_DVDVIDEO:
            return HomeScreenDef::MEDIA_SOURCE_DVD_VIDEO;
        case MEDIA_SOURCE_IPOD1:
            return HomeScreenDef::MEDIA_SOURCE_IPOD1;
        case MEDIA_SOURCE_IPOD2:
            return HomeScreenDef::MEDIA_SOURCE_IPOD2;
        case MEDIA_SOURCE_AUX1:
        case MEDIA_SOURCE_AUX2:
            return HomeScreenDef::MEDIA_SOURCE_AUX;
        case MEDIA_SOURCE_BLUETOOTH:
            return HomeScreenDef::MEDIA_SOURCE_BLUETOOTH;
        default:
            return HomeScreenDef::MEDIA_SOURCE_MAX;
    }
}

void ExternalNotifier::updateErrorOSD(int param)
{
    Q_UNUSED(param)
    // { removed by cychoi 2013.07.30 for unused code
	//m_updatePositionTimer.stop();//added by edo.lee 2013.01.07
    // { removed by cychoi 2013.07.30 for unused code
}

// added by edo.lee 2013.03.11
QString ExternalNotifier::GetEjectDisplay()
{
	MP_LOG<< "m_device"<<m_device<< m_pEngineMain->GetFrontView() <<m_pEngineMain->GetRearView() << m_pEngineMain->getIsNoneState() << LOG_ENDL;
	QString OSDDisplay = "FR";

    //{ modified by yongkyun.lee 2013-07-12 for : ITS 179702
    /* None AV : front rear : OSD */
    if(m_pEngineMain->getIsNoneState())
    {
         OSDDisplay="FR";
         m_DeckOSDStatus = OSDDisplay;
         return OSDDisplay;
    }

    /* AV : BG Player : front rear : OSD */    
    if( (m_pEngineMain->GetFrontView()>= APP_MEDIA_MODE_PHOTO_FRONT &&
        m_pEngineMain->GetRearView() >= APP_MEDIA_MODE_PHOTO_FRONT))
    {
       /* disc - play : not display */
       if( m_device == MEDIA_SOURCE_DVDVIDEO || m_device == MEDIA_SOURCE_VCD  ||
           m_device == MEDIA_SOURCE_MP3      || m_device == MEDIA_SOURCE_CDDA || 
           m_device == MEDIA_SOURCE_DVDAUDIO )
       {
           OSDDisplay="C"; // modified by yongkyun.lee 2013-07-18 for : ITS 179198
       }
       else/* juke/usb/ipod etc... - play : OSD display */
       {
            //{ modified by yongkyun.lee 2013-07-18 for :  ITS 179198
            if(m_DeckOSDStatus == "C")
                OSDDisplay="";
            else
            //} modified by yongkyun.lee 2013-07-18 
            OSDDisplay="FR";
       }
       m_DeckOSDStatus = OSDDisplay;
       return OSDDisplay;
    }
        
    /* AV : FG : DISC : play : */
    //{ modified by yongkyun.lee 2013-07-18 for : ITS 179198
    // if( m_device == MEDIA_SOURCE_DVDVIDEO || m_device == MEDIA_SOURCE_VCD     || 
    //     (m_device >= MEDIA_SOURCE_MP3     && m_device <= MEDIA_SOURCE_DVDAUDIO))
    //{ modified by yongkyun.lee 2013-07-18
	{
	    OSDDisplay=""; 
        if ( m_pEngineMain->GetFrontView() == APP_MEDIA_MODE_VIDEO  ||  m_pEngineMain->GetFrontView()== APP_MEDIA_MODE_AUDIO)
		{
			OSDDisplay = "F";
		}

        if( m_pEngineMain->GetRearView()==APP_MEDIA_MODE_VIDEO ||  m_pEngineMain->GetRearView() == APP_MEDIA_MODE_AUDIO)
		{
			OSDDisplay += "R";
        }
    }       
    //} modified by yongkyun.lee 2013-07-12 
 
    //{ added by yongkyun.lee 20130612 for :  ITS 173417-70953
    if( m_device == MEDIA_SOURCE_DVDVIDEO || m_device == MEDIA_SOURCE_VCD  ||
        m_device == MEDIA_SOURCE_MP3      || m_device == MEDIA_SOURCE_CDDA || 
        m_device == MEDIA_SOURCE_DVDAUDIO )
    {
        m_DeckOSDStatus = OSDDisplay;
    }
    else
    {
        m_DeckOSDStatus="FR";
    }
    //} added by yongkyun.lee 20130612 
	return OSDDisplay;
}

DISPLAY_T ExternalNotifier::GetOSDDisplay()
{
	if( m_pEngineMain->getIsCloneTemp() /*|| m_pEngineMain->getReleasingClone()*/) //modified by edo.lee 2013.07.07
	{		MP_LOG<< "set clone or release clone" << LOG_ENDL;

        return DISPLAY_NONE;
	}
    // { added by yungi 2012.11.07 for No CR OSD <Seek Track>
    AppMediaPlayer_EngineAudio* pAudioEngine = dynamic_cast<AppMediaPlayer_EngineAudio*>(m_pEngines[APP_MEDIA_MODE_AUDIO]);   
	AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_pEngines[APP_MEDIA_MODE_VIDEO]);   
    // { added by sungha.choi 2013.07.15 for static test
    if(pAudioEngine == NULL || pVideoEngine == NULL)
    {
        MP_HIGH << "pAudioEngine or pVideoEngine are NULL" << LOG_ENDL;
        return DISPLAY_NONE;
    }
    // } added by sungha.choi 2013.07.15 for static test

    // } added by yungi 2012.11.07 for No CR OSD <Seek Track>
    // { added by lssanh 2012.01.22 ITS150442
    int nPopup = static_cast <AppMediaPlayer_Controller_Audio*> (m_pEngines[APP_MEDIA_MODE_AUDIO]->GetController())->GetListViewModel()->getPopType();
    // } added by lssanh 2012.01.22 ITS150442

	//added by edo.lee 2013.07.15 STATIC TEST
	if(!pAudioEngine )
	{
		MP_LOG<<"audio engien is null" << LOG_ENDL;
	 	 return DISPLAY_NONE;
	}else if (!pVideoEngine)
	{
		MP_LOG<<"video engien is null" << LOG_ENDL;
		 return DISPLAY_NONE;
	}
	//added by edo.lee 2013.07.15 STATIC TEST

    DISPLAY_T nDisp = DISPLAY_FRONT_REAR;
    //{ modified by yungi 2012.11.07 for No CR OSD <Seek Track>

//added on 2013.07.03 ITS# 0177010
    MP_LOG << "getShowOSDFromBUfferedTrack() = " << pVideoEngine->getShowOSDFromBUfferedTrack() << LOG_ENDL;
    MP_LOG << "getScreenManager(VIDEO_DISPLAY_FRONT)->topScreenName() = " << pVideoEngine->getScreenManager(VIDEO_DISPLAY_FRONT)->topScreenName() << LOG_ENDL;
    MP_LOG << "getScreenManager(VIDEO_DISPLAY_REAR)->topScreenName() = " << pVideoEngine->getScreenManager(VIDEO_DISPLAY_REAR)->topScreenName() << LOG_ENDL;


    if(   pAudioEngine->getCategoryEditMode()   //added by junam 2013.06.24 for ISV_KR_85467
            || (nPopup >= 0 && (nPopup != (int)LVEnums::POPUP_TYPE_IPOD_INDEXING
                                        && nPopup != (int)LVEnums::POPUP_TYPE_LOADING_DATA))            
            ||(pVideoEngine->getShowOSDFromBUfferedTrack() &&// pVideoEngine->isVideoListView() == VIDEO_DISPLAY_MAX &&//added by edo.lee 2013.03.18
               (( pVideoEngine->getScreenManager(VIDEO_DISPLAY_FRONT)->topScreenName() ==
                                               AppMediaPlayer_Video_Screens::SCREEN_FULL)
                        || ( pVideoEngine->getScreenManager(VIDEO_DISPLAY_REAR)->topScreenName() ==
                            AppMediaPlayer_Video_Screens::SCREEN_FULL))) //added on 2013.07.03 ITS# 0177010
        )
    {
        nDisp = DISPLAY_FRONT_REAR;
    }
    else
    {
        if (/* m_pEngineMain->GetFrontView() < APP_MEDIA_MODE_PHOTO_FRONT &&
             m_pEngineMain->GetFrontView() == m_pEngineMain->GetFrontMode() &&*/         
             m_pEngineMain->getPlayInfoDisplay() == DISPLAY_REAR &&
             !m_pEngineMain->getOnBoot())
        {
        	//added by edo.lee 2013.04.18
        	/*if(!m_pEngineMain->getCamInFG() )
        	{        		
	            nDisp = DISPLAY_REAR;					
        	}
			else
			{
				//if(m_pEngineMain->getIsSwapEnabled())
				//	nDisp = DISPLAY_FRONT;
				//else
					nDisp = DISPLAY_REAR;
			}*/
			nDisp = DISPLAY_REAR;
			//added by edo.lee 2013.04.18
        }

        else if (/* m_pEngineMain->GetRearView() < APP_MEDIA_MODE_PHOTO_FRONT &&
             m_pEngineMain->GetRearView() == m_pEngineMain->GetRearMode() &&*/
             m_pEngineMain->getPlayInfoDisplay() == DISPLAY_FRONT &&
             !m_pEngineMain->getOnBoot())
        {        	        	
		     nDisp = DISPLAY_FRONT;					
        }

		//modified by edo.lee 2013.03.29
		if (m_pEngineMain->getPlayInfoDisplay() == DISPLAY_REAR &&
			pVideoEngine->getIsShowLockoutView() == VIDEO_DISPLAY_FRONT)
		{
			if((pVideoEngine->getScreenManager(VIDEO_DISPLAY_REAR)->getScreensCount()==0)
			   ||( pVideoEngine->getScreenManager(VIDEO_DISPLAY_REAR)->topScreenName() == 
				AppMediaPlayer_Video_Screens::SCREEN_FULL))				
				nDisp = DISPLAY_FRONT_REAR;
			else
				nDisp = DISPLAY_FRONT;
		}
		else if(m_pEngineMain->getPlayInfoDisplay() == DISPLAY_FRONT &&
			pVideoEngine->getIsShowLockoutView() == VIDEO_DISPLAY_REAR )
		{
			if((pVideoEngine->getScreenManager(VIDEO_DISPLAY_FRONT)->getScreensCount()==0)
				||(pVideoEngine->getScreenManager(VIDEO_DISPLAY_FRONT)->topScreenName() == 
				AppMediaPlayer_Video_Screens::SCREEN_FULL))
				nDisp = DISPLAY_FRONT_REAR;
			else
				nDisp = DISPLAY_REAR;
		}
		//modified by edo.lee 2013.03.29	

    }
    //} modified by yungi 2012.11.07 for No CR OSD <Seek Track>

    return nDisp;
}

// { added by edo.lee 2012.12.07 for statusbar
QString ExternalNotifier::GetOSDDispString()
{
	DISPLAY_T disp = GetOSDDisplay();
   QString s_disp="";
   switch(disp)
   {
   	case DISPLAY_FRONT:
		s_disp = "F";
		break;
	case DISPLAY_REAR:
		s_disp = "R";
		break;
	case DISPLAY_FRONT_REAR:	
	case DISPLAY_AUTO:
		s_disp = "FR";
		break;
	case DISPLAY_NONE: //added by edo.lee 2013.04.22 for OSD
		s_disp = "";
        break;
	default:
		s_disp = "F";
		
   }
	return s_disp;
}
// } added by edo.lee

void ExternalNotifier::notifySDPByVideoSource(MEDIA_SOURCE_T source)
{
    APP_STATE_T mode = APP_STATE_NONE;

    switch(source)
    {
        case MEDIA_SOURCE_JUKEBOX:
        {
            mode = APP_STATE_JUKEBOX;
            break;
        }
        case MEDIA_SOURCE_USB1:
        {
            mode = APP_STATE_USB1;
            break;
        }
        case MEDIA_SOURCE_USB2:
        {
            mode = APP_STATE_USB2;
            break;
        }
        case MEDIA_SOURCE_DVDVIDEO:
        {
            mode = APP_STATE_DVD;
            m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_DVDVIDEO);
            m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_DVDVIDEO);
            break;
        }
        case MEDIA_SOURCE_VCD:
        {
            mode = APP_STATE_VCD;
            m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_VCD);
            m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_VCD);
            break;
        }
        case MEDIA_SOURCE_AUX1:
        case MEDIA_SOURCE_AUX2:
        {
            mode = APP_STATE_AUX;
            break;
        }
        default:
        {
            MP_LOG << "not handled mode" << LOG_ENDL;
            break;
        }
    }

    if(mode != APP_STATE_NONE)
    {
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_DEVICE_STATUS, CONNECT_STATUS_CONNECTED);
    }
}

void ExternalNotifier::notifySDPByVideoPlayingState(PLAYER_STATE state)
{
    switch(state)
    {
        case PLAYER_PLAY:
        {
            m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_PLAY_STATUS, true);
            break;
        }
        case PLAYER_PAUSE:
        case PLAYER_STOP:
        {
            m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_PLAY_STATUS, false);
            break;
        }
        default:
        {
            MP_LOG << "Unsupported state" << LOG_ENDL;
            break;
        }
    }
}

void ExternalNotifier::onPlayingModeChanged(MEDIA_MODE mode)
{
    // PLEASE! Feel free to modify this function for Music and Photo.
    MP_LOG<< mode << LOG_ENDL;
    if(m_device != MEDIA_SOURCE_MAX)
    {
        //{ modified by yongkyun.lee 2013-07-15 for : ITS 179776
        if(mode == SCANOFF || mode == SCANFILE || mode == SCANFOLDER || mode == SCANALL)
        {
            m_devices[m_device].m_playingScanMode = mode;
        }
        if(mode == RANDOMOFF || mode == RANDOMFILE || mode == RANDOMFOLDER)
        {
            m_devices[m_device].m_playingRandomMode = mode;
        }
        if(mode >= REPEATOFF && mode <= REPEATALL )
        {
            m_devices[m_device].m_playingRepeatMode = mode;
        }
        //} modified by yongkyun.lee 2013-07-15 
    }

    if(QObject::sender() != m_pVideoCtrl)
    {
        return;
    }
    // { modified by lssanh 2012.10.25 for SANITY_CM_AJ012
    //		  if ( !m_bTuneMode )
    // { Deleted  by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD   
    //if ( !m_bTuneMode && GetAVOffMode() == false)
	// { modified by lssanh 2012.10.25 for SANITY_CM_AJ012
    //{
    //    updateVideoOSD();
    //}
     // } Deleted  by  yongkyun.lee  
    switch(mode)
    {
        case REPEATFILE:
        case REPEATFOLDER:
        {
            m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_REPEAT, true);
            break;
        }
        case REPEATOFF:
        {
            // { modified by kihyung 2012.07.07
            // m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_REPEAT, false);
            m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_REPEAT, true);
            // } modified by kihyung
            break;
        }
        case RANDOMFILE:
        case RANDOMFOLDER:
        {
            m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_RANDOM, true);
            break;
        }
        case RANDOMOFF:
        {
            m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_RANDOM, false);
            break;
        }
        default:
        {
            MP_LOG << "Unsupported mode" << LOG_ENDL;
        }
    }
}

// Technical method to check SDP update functionality.
void ExternalNotifier::readSDPVideoData()
{
    MP_LOG << "random:" << m_pSharedVideoData->GetStatusDateByIndex(DATA_TYPE_RANDOM) << LOG_ENDL;
    MP_LOG << "repeat:" << m_pSharedVideoData->GetStatusDateByIndex(DATA_TYPE_REPEAT) << LOG_ENDL;
    MP_LOG << "is playing:" << m_pSharedVideoData->GetStatusDateByIndex(DATA_TYPE_PLAY_STATUS) << LOG_ENDL;
    MP_LOG << "is connected:" << m_pSharedVideoData->GetStatusDateByIndex(DATA_TYPE_DEVICE_STATUS) << LOG_ENDL;
    MP_LOG << "type of inserted disc:" << m_pSharedVideoData->GetStatusDateByIndex(DATA_TYPE_INSERTED_DISC) << LOG_ENDL;
}

void ExternalNotifier::setOSDFileFolder(QString &fileName, QString &folderName)
{
    switch(m_device)
    {
        case MEDIA_SOURCE_JUKEBOX:
        case MEDIA_SOURCE_USB1:
        case MEDIA_SOURCE_USB2:
        {
            folderName = m_devices[m_device].m_folderName;
            fileName = m_devices[m_device].m_title;
            break;
        }
        case MEDIA_SOURCE_VCD:
        {
            // modified by minho
            // for CR12068 Display wrong word on OSD after insert VCD
            // fileName = QApplication::translate("main", "STR_MEDIA_CHAPTER") + " " + m_devices[m_device].m_title;
            // { modified by changjin 2012.12.11 for CR 16225
            // fileName = QApplication::translate("main", "STR_MEDIA_TRACK") + " " + m_devices[m_device].m_title;
            // { modified by lssanh 2013.01.29 ISV71402
            // QString strTitle = "";
            // strTitle.sprintf( "%d", m_devices[m_device].m_number); 
            fileName = QApplication::translate("main", "STR_MEDIA_TRACK") + " " + m_devices[m_device].m_title;
            // } modified by lssanh
            // } modified by changjin 2012.12.11
            // modified by minho
            break;
        }
        case MEDIA_SOURCE_DVDVIDEO:
        {
            folderName = QApplication::translate("main", "STR_MEDIA_TITLE") + " " + m_devices[m_device].m_folderName;
            fileName = QApplication::translate("main", "STR_MEDIA_CHAPTER") + " " + m_devices[m_device].m_title;
            break;
        }
        default:
        {
            MP_LOG << "Unsupported source." << LOG_ENDL;
            break;
        }
    }
}

void ExternalNotifier::onVideoFilenameChanged(QString filename)
{
    MP_MEDIUM << "filename =" << filename << LOG_ENDL;

    if( m_device != MEDIA_SOURCE_MAX )
    {
        if(m_mode != APP_MEDIA_MODE_VIDEO) return; //added by yungi 2013.08.28 for HMC QC Cluster wrong track number

        m_devices[m_device].m_title = filename;

        // { modified by cychoi 2013.09.03 for ITS 187585
        QString videoTpMessage = filename;
        if(m_device == MEDIA_SOURCE_VCD)
        {
            QString strTrack = QString(QApplication::translate("main", "STR_MEDIA_TRACK"));
            videoTpMessage.prepend(strTrack);
        }
        // } modified by cychoi 2013.09.03

        // { added by cychoi 2013.06.10 for code arrangement
        if(m_devices[m_device].m_videoTpMessage != videoTpMessage && // modified by cychoi 2013.09.03 for ITS 187585
           m_device != MEDIA_SOURCE_DVDVIDEO && m_mode == APP_MEDIA_MODE_VIDEO &&
           !m_bTuneMode)
        {
            // { modified by cychoi 2013.06.24 for ISV 85786
            m_devices[m_device].m_videoTpMessage = videoTpMessage; // modified by cychoi 2013.09.03 for ITS 187585
            // } modified by cychoi 2013.06.24
            m_bTpNeedToSend = true;
        }
        // } added by cychoi 2013.06.10

        m_SearchTitle = filename; //added by edo.lee 2013.05.08
        
		//m_updatePositionTimer.stop();//added by edo.lee 2013.06.27
        //{ added by yongkyun.lee 20130626 for : ITS 176114
        //if(m_updatePositionTimer.isActive() && 
        //  (m_BackupOSDDevice == m_device ) && 
        //  (m_device == MEDIA_SOURCE_DVDVIDEO || m_device == MEDIA_SOURCE_VCD ) )
        //    m_OSDDisplay = true;
        //} added by yongkyun.lee 20130626 
        //m_updatePositionTimer.stop();//added by edo.lee 2013.06.27
        
        //if(GetAVOffMode() == false && m_device != MEDIA_SOURCE_AUX1 && (m_OSDDisplay || m_bTuneMode ))
        //{        	
	         //DisplayVideoTrackOSD();//modified by edo.lee 2013.03.04            
            // removed by cychoi 2013.06.10 for meaningless code
        //}

        // { modified by Sergey 04.10.2013 for ITS#193837
        MEDIA_SOURCE_T source = m_pVideoCtrl->getSource();
        if(!m_bTuneMode && (source == MEDIA_SOURCE_JUKEBOX || source == MEDIA_SOURCE_USB1 || source == MEDIA_SOURCE_USB2))
        {
            updateVideoFileManager();
        }
        // { modified by Sergey 04.10.2013 for ITS#193837
    }
}

void ExternalNotifier::onVideoFullFolderNameChanged(QString fullfoldername)
{
    MP_MEDIUM << fullfoldername << LOG_ENDL;
    if(m_device != MEDIA_SOURCE_MAX)
    {
        if(m_mode != APP_MEDIA_MODE_VIDEO) return; //added by yungi 2013.08.28 for HMC QC Cluster wrong track number

        m_devices[m_device].m_fullFolderName = fullfoldername;
    }
}

void ExternalNotifier::onPlaylistLenghtChanged(int count)
{
    MP_MEDIUM << count << LOG_ENDL;
    if(m_device != MEDIA_SOURCE_MAX)
    {
        if(m_mode != APP_MEDIA_MODE_VIDEO) return; //added by yungi 2013.08.28 for HMC QC Cluster wrong track number

        m_devices[m_device].m_quantity = count;
    }
}

void ExternalNotifier::onPlaylistPositionChanged(int position)
{
    MP_MEDIUM << position << LOG_ENDL;
    if(m_device != MEDIA_SOURCE_MAX)
    {
        if(m_mode != APP_MEDIA_MODE_VIDEO) return; //added by yungi 2013.08.28 for HMC QC Cluster wrong track number

        // { modified by wspark 2013.04.09 for cluster IOT #37
        //m_devices[m_device].m_number = position + 1;
        if(!m_bTuneMode)
        {
            m_devices[m_device].m_number = position + 1;
        }
        // } modified by wspark
    }
}

void ExternalNotifier::onFoldernameChanged(QString name)
{
    if(m_device != MEDIA_SOURCE_MAX)
    {
        if(m_mode != APP_MEDIA_MODE_VIDEO) return; //added by yungi 2013.08.28 for HMC QC Cluster wrong track number

        // { added by cychoi 2013.06.10 for code arrangement
        m_devices[m_device].m_folderName = name;
        if(m_device == MEDIA_SOURCE_DVDVIDEO)
        {
            // { modified by cychoi 2013.09.03 for ITS 187585
            // { modified by cychoi 2013.06.24 for ISV 85786
            QString videoTpMessage = name;
            QString strTitle = QString(QApplication::translate("main", "STR_MEDIA_TITLE"));
            videoTpMessage.prepend(strTitle);
            // } modified by cychoi 2013.06.24
            // } modified by cychoi 2013.09.03

            if(m_devices[m_device].m_videoTpMessage != videoTpMessage && // modified by cychoi 2013.09.03 for ITS 187585
               !m_bTuneMode) // modified by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
            {
                // { modified by cychoi 2013.06.24 for ISV 85786
                m_devices[m_device].m_videoTpMessage = videoTpMessage; // modified by cychoi 2013.09.03 for ITS 187585
                // } modified by cychoi 2013.06.24
				m_bTpNeedToSend = true;
            }
        }
        // } added by cychoi 2013.06.10
        // removed by cychoi 2013.06.10 for meaningless code
    }
}

// { remvoed by kihyung 2012.11.26. removed spec.
/*
// { added by Sergey Vetugov for CR#13601
void ExternalNotifier::onDVDScreenChanged(int state)
{
    MP_LOG << "state =" << state << LOG_ENDL;

    if(state == PLAYER_TITLEMENU_DISPLAY || state == PLAYER_TOPMENU_DISPLAY
       || state == PLAYER_ROOTMENU_DISPLAY)
    {
        hideVideoAVWidget();
    }
}
// } added by Sergey Vetugov for CR#13601
*/
// } remvoed by kihyung 2012.11.26. removed spec.

void ExternalNotifier::onMoreLikeThisCluster()
{
    // { deleted by wspark 2012.12.10 MLT is deleted in spec.
    /*
// { modify by wspark 2012.07.27 for CR9361
    //quint64 data = 0x76;
    quint64 data = 0x05;
// } modify by wspark 2012.07.27 for CR9361
    // { modified by wspark 2012.09.14 for CAN unused signals.
    //SendClusterData(data << 56);
    SendClusterData_OpStateOnly(data);
    // } modified by wspark
    */
    // } deleted by wspark
}

void ExternalNotifier::onDiscLoading()
{
	//MP_LOG << LOG_ENDL;
    //    m_DeckOSDStatus="";// added by yongkyun.lee 20130612 for :  ITS 173417-70953
	//m_updatePositionTimer.stop();//added by edo.lee 2013.01.07
	//QString info  = "@OSD:FR^MEDIA_INFO^LOADING^DISC^DISC^";
	//info +=  qApp->translate("main","STR_READ_DISC_INFO");
        //m_pOSDView = m_pEngineMain->GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN); //delete by ys 2013.09.30added by edo.lee 2013.02.14
        //if(m_pOSDView){
	    //m_pOSDView->setWindowTitle(info);
	    //OSDWriter::SendOSD(info); //modified by edo.lee 2013.04.04
		//QCoreApplication::flush(); //added by edo.lee 2013.01.16
        //}
}

void ExternalNotifier::onNotifyIPodReady(MEDIA_SOURCE_T source)
{
    MP_LOG << source << LOG_ENDL;
    m_pSharedAudioData->SetStatusDateByIndex( \
                    MEDIA_SOURCE_IPOD1 ? DATA_TYPE_IPOD1_MEDIA_LOADING : DATA_TYPE_IPOD2_MEDIA_LOADING ,\
                    MEDIA_DEVICE_SCAN_END);
}

void ExternalNotifier::onAvModeChanged(MEDIA_SOURCE_T source, bool)
{
    MP_LOG << source << LOG_ENDL;
    
    APP_STATE_T mediaState = APP_STATE_NONE;
    switch( source )
    {
        case MEDIA_SOURCE_JUKEBOX:
        {
            mediaState = APP_STATE_JUKEBOX;
            break;
        }
        case MEDIA_SOURCE_USB1:
        {
            mediaState = APP_STATE_USB1;
            break;
        }
        case MEDIA_SOURCE_USB2:
        {
            mediaState = APP_STATE_USB2;
            break;
        }
        case MEDIA_SOURCE_DVDVIDEO:
        {
            mediaState = APP_STATE_DVD;
            break;
        }
        case MEDIA_SOURCE_VCD:
        {
            mediaState = APP_STATE_VCD;
            break;
        }
        case MEDIA_SOURCE_AUX1:
        case MEDIA_SOURCE_AUX2:
        {
            mediaState = APP_STATE_AUX;
            break;
        }
        default:
        {
            break;
        }
    }
    m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_ACTIVE_MEDIA_SOURCE, (int)mediaState);

    // { modified by kihyung 2013.06.13 for ITS 0173713
    if(mediaState == APP_STATE_USB1 || mediaState == APP_STATE_USB2
            || mediaState == APP_STATE_JUKEBOX || mediaState == APP_STATE_NONE)
        m_currentVideoMediaState = APP_STATE_JUKEBOX;
    else if(mediaState == APP_STATE_DVD  || mediaState == APP_STATE_VCD)
        m_currentVideoMediaState = APP_STATE_DVD;
    else
        m_currentVideoMediaState = mediaState;

    // { modified for ITS 0177757 2013.07.07
    if(!m_pEngineMain->getCamInFG())
        resetDisplayParamWrtState(m_currentVideoMediaState);
}

// { added by  yongkyun.lee  2012.11.05 for  Check Video/audio OSD
void ExternalNotifier::onSetDisplayOSD(bool vOSD)
{
    Q_UNUSED(vOSD)
    //m_OSDDisplay =  vOSD;
}
// } added by  yongkyun.lee

// removed by Sergey for CR#15771

// { remvoed by kihyung 2012.11.26. removed spec.
/*
// { modified by ravikanth - 12-09-25 - cr 13789
void ExternalNotifier::onUpdateDvdTrackInfo(int state)
{
    MP_LOG << "state =" << state << LOG_ENDL;
    if(state == PLAYER_TITLEMENU_DISPLAY || state == PLAYER_TOPMENU_DISPLAY
       || state == PLAYER_ROOTMENU_DISPLAY)
    {
        StopAVWidgets();
    }
}
// } modified by ravikanth - 12-09-25 - cr 13789
*/
// } remvoed by kihyung 2012.11.26. removed spec.

// { added by changjin 2012.10.04 for SANITY_CM_AJ338
void ExternalNotifier::SetAVOffMode(bool mode)
{
	MP_LOG << LOG_ENDL;
    m_bAVOffMode = mode;
	//if(mode)
	//{
	//	m_updatePositionTimer.stop();//added by edo.lee 2013.05.13 ISV 81062
		//OSDWriter::SendOSD("@OSD:FR:CLEAR_AV");//added by edo.lee 2013.05.13 ISV 81062
	//}
}
//added by edo.lee 2013.06.25
void ExternalNotifier::ClearOSD(bool isFront)
{
	MP_LOG<<isFront << LOG_ENDL;
	/* {  moodified by edo.lee 2013.07.09
	QString disp = "FR";
	if(isFront)
		disp = "F";
	else
		disp = "R";
	
	OSDWriter::SendOSD("@OSD:"+disp+":CLEAR_AV");
	*/
	if(!m_pEngineMain->getIsDiscEject())
	{
		//modified by edo.lee 2013.07.09
		if(isFront)
		{
			if(m_pEngineMain->getCamInFG())
			{
				if(m_pEngineMain->getIsSwapEnabled()==true)
				{
					//OSDWriter::SendOSD("@OSD:F:CLEAR_AV");
				}
			}
			else 
			{
				//OSDWriter::SendOSD("@OSD:F:CLEAR_AV");
			}
		}
		else{
			if(m_pEngineMain->getCamInFG())
			{
				if (m_pEngineMain->getIsSwapEnabled()==false)
				{
					//OSDWriter::SendOSD("@OSD:R:CLEAR_AV");
				}
			}
			else{
				//OSDWriter::SendOSD("@OSD:R:CLEAR_AV");
			}
		}	
		//modified by edo.lee 2013.07.09
		
	}
	else
	{
		m_pEngineMain->setIsDiscEject(false);
	}
	// }  moodified by edo.lee 2013.07.09
}
//added by edo.lee 2013.06.25
bool ExternalNotifier::GetAVOffMode(void)
{
    return m_bAVOffMode;
}
// } added by changjin 2012.10.04

// removed by cychoi 2013.08.17 for code arrangement

// { added by wspark 2012.11.07 for updating mp3cd track info correctly.
// { modified by wspark 2013.03.27 for ISV 77371
//void ExternalNotifier::onUpdateMP3CDTrackInfo(int track)
void ExternalNotifier::onUpdateTrackInfo(int track)
// } modified by wspark
{
    MP_LOG << m_device << track << LOG_ENDL;
    if (m_device == MEDIA_SOURCE_MAX)
    {
        return;
    }

    //{ modified by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
    if(m_device == MEDIA_SOURCE_CDDA || m_bTuneMode)
    {
        return;
    }

    // { modified by wspark 2013.03.27 for ISV 77371
    /*
    if(GetClusterState() == 0x52) // MP3CD
        m_devices[m_device].m_number = track;
    */
    m_devices[m_device].m_number = track;
    // } modified by wspark

    if(QObject::sender() == m_pVideoCtrl)
    {
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_VIDEO_STATUS, m_devices[m_device].m_number);
    }

    m_bTpNeedToSend = true; // moved by cychoi 2013.09.03 for cluster update sync

    // { added by wspark 2013.04.14 for cluster IOT #40
    m_devices[m_device].m_position = 0; // added by cychoi 2013.06.07 for ITS 150487
    SendClusterData_Progress(0);
    // } added by wspark

    m_bSendingCanProgressMessage = true; // added by wspark 2013.02.19

    //m_bTpNeedToSend = true; // commented by cychoi 2013.09.03 for cluster update sync // for Cluster IOT issue #95
    //} modified by cychoi 2013.08.17
}
// } added by wspark

// removed by cychoi 2013.08.17 for code arrangement

// { added by yungi 2012.11.14 for ISV 62667,ISV 62596
void ExternalNotifier::onUpdateStateTrackOSD(int speedCheck)
{
    Q_UNUSED(speedCheck)
    //MP_LOG << speedCheck << LOG_ENDL;
    //m_OSDTreckSpeed=speedCheck;
}
// } added by yungi 2012.11.14 for ISV 62667,ISV 62596

// { added by ravikanth 27-12-12
void ExternalNotifier::resetDisplayParamWrtState(APP_STATE_T mediaState)
{
    MP_LOG<<mediaState << LOG_ENDL;
    int brightness;
    int tint;
    int hue;
    int contrast;
    //if(mediaState != APP_STATE_NONE) // delete by eugene.seo 2013.04.22
    //{
        mediaState = APP_STATE_JUKEBOX; // added by eugene.seo 2013.04.22
        brightness = m_displayPlaneData[mediaState].m_brightness;
        setBrightness(brightness);
		
        tint = m_displayPlaneData[mediaState].m_tint;
        setTint(tint);
		
        hue = m_displayPlaneData[mediaState].m_hue;
        setHue(hue);
		
        contrast = m_displayPlaneData[mediaState].m_contrast;
        setContrast(contrast);
		
        MP_LOG<<"JUKEBOX "<<brightness<<" "<<tint<<" "<<hue<<" "<<contrast << LOG_ENDL;
    //}

    m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_BRIGHTNESS, brightness);
    m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_SATURATION, tint);
    m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_HUE, hue);
    m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_CONTRAST, contrast);
}

//added by shkim for ITS 189354 Display Setting value Issue
void ExternalNotifier::onSetInitialDisplayValue()
{
    APP_STATE_T mediaState = APP_STATE_JUKEBOX; // added by eugene.seo 2013.04.22
    int brightness = m_displayPlaneData[mediaState].m_brightness;


    int tint = m_displayPlaneData[mediaState].m_tint;


    int hue = m_displayPlaneData[mediaState].m_hue;


    int contrast = m_displayPlaneData[mediaState].m_contrast;

    MP_LOG << "brightness: " << brightness <<" , tint: " << tint << ", hue:" << hue << ", contrast: "<<contrast << LOG_ENDL;
    m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_PLAY_STATUS, true);
    m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_BRIGHTNESS, brightness);
    m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_SATURATION, tint);
    m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_HUE, hue);
    m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_CONTRAST, contrast);
}
//added by shkim for ITS 189354 Display Setting value Issue
void ExternalNotifier::setBrightness(int brightness)
{
	// removed by Sergey 24.08.2013

    //m_pDDHelper->SetBrightness(brightness); // delete by eugene.seo 2013.02.27 for display setting
	if(brightness > 10 || brightness < -10) return; // added by eugene.seo 2013.04.23
    m_currentBrightness = brightness;

    // added by kihyung 2013.08.27 for ITS 0186511 
    if(m_pVideoCtrl->getSource() == MEDIA_SOURCE_MAX)
        return;

	// { modified by eugene.seo 2013.04.22
	//if( m_currentVideoMediaState == APP_STATE_AUX || m_currentVideoMediaState == APP_STATE_JUKEBOX || m_currentVideoMediaState == APP_STATE_DVD )
	//{ // delete by eugene.seo 2013.04.23
		
	    if(m_currentVideoMediaState == APP_STATE_AUX)
			brightness = brightnessMap[0][brightness+10];	
		else if(m_currentVideoMediaState == APP_STATE_JUKEBOX)
			brightness = brightnessMap[1][brightness+10];	
		else // m_currentVideoMediaState == APP_STATE_DVD
			brightness = brightnessMap[2][brightness+10];

		MP_LOG << brightness << LOG_ENDL;		

        #if (AVP_USE_NEW_BLEND_AFW == 0)
		// { modified by eugene.seo 2013.06.03
		AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_pEngines[APP_MEDIA_MODE_VIDEO]);   
        // { added by cychoi 2013-07-15 for Prevent fix
        if(pVideoEngine == NULL)
            return;
        // } added by cychoi 2013-07-15     

        // { added by kihyung 2013.07.20 for ITS 0180419 
        APP_MEDIA_MODE_T eViewF = m_pEngineMain->GetAVMediaMode(m_pEngineMain->GetAVPEntryPoint(DISPLAY_FRONT), DISPLAY_FRONT);
        APP_MEDIA_MODE_T eViewR = m_pEngineMain->GetAVMediaMode(m_pEngineMain->GetAVPEntryPoint(DISPLAY_REAR), DISPLAY_REAR);
        // } added by kihyung 2013.07.20 for ITS 0180419  

        DualDisplayHelper::MEDIA_DISPLAY eMediaDisplay;
        if(pVideoEngine->IsPlaying() || m_currentVideoMediaState == APP_STATE_AUX)
		{
		    if(m_displayPlane == OVERLAY_PLANE) {
		        eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_B;
		    }
		    else if(m_displayPlane == SPRITEC_PLANE) {
		        eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_A;
		    }
		    else if(m_displayPlane == OVERLAY_SPRITEC) {
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_A_AND_B;
		    }
            else {
                return;
            }

            if(pVideoEngine->getIsShowLockoutView() != VIDEO_DISPLAY_MAX && m_pEngineMain->isDRSShow() == true) {
                MP_LOG << "lockout mode" << LOG_ENDL;	
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_B;
            }
            // { modified by kihyung 2013.07.20 for ITS 0180419 
            else if(eViewF == APP_MEDIA_MODE_PHOTO_FRONT || eViewR == APP_MEDIA_MODE_PHOTO_REAR) 
            {
                MP_LOG << "photo mode" << LOG_ENDL;
                if(m_pEngineMain->GetRearView() == APP_MEDIA_MODE_PHOTO_REAR && m_pEngineMain->getCamInFG() == true) {
                    return;
                }
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_B;
            }
            // } modified by kihyung 2013.07.20 for ITS 0180419 
	    }	
		else
		{
		    if(m_displayPlane == OVERLAY_PLANE) {
		        eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_D;
		    }
		    else if(m_displayPlane == SPRITEC_PLANE) {
		        eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_C;
		    }
		    else if(m_displayPlane == OVERLAY_SPRITEC) {
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_C_AND_D;
		    }
            else {
                return;
            }

            if(pVideoEngine->getIsShowLockoutView() != VIDEO_DISPLAY_MAX && m_pEngineMain->isDRSShow() == true) {
                MP_LOG << "lockout mode" << LOG_ENDL;	
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_D;
            }
            // { modified by kihyung 2013.07.20 for ITS 0180419 
            else if(eViewF == APP_MEDIA_MODE_PHOTO_FRONT || eViewR == APP_MEDIA_MODE_PHOTO_REAR) 
            {
                MP_LOG << "photo mode" << LOG_ENDL;	
                if(m_pEngineMain->GetRearView() == APP_MEDIA_MODE_PHOTO_REAR && m_pEngineMain->getCamInFG() == true) {
                    return;
                }
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_D;
            }
            // } modified by kihyung 2013.07.20 for ITS 0180419 
		}
        #else
        DualDisplayHelper::MEDIA_DISPLAY eMediaDisplay = (DualDisplayHelper::MEDIA_DISPLAY)m_pEngineMain->GetDisplayPlane(APP_AVP);

        // { added by kihyung 2013.08.21 for ITS 0185408
        AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_pEngines[APP_MEDIA_MODE_VIDEO]);   
        if(pVideoEngine == NULL)
            return;
        
        if(pVideoEngine->IsPlaying() == false && m_currentVideoMediaState != APP_STATE_AUX)
        {
            if(eMediaDisplay == DualDisplayHelper::eMP_DISPLAY_B)               eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_D;
		    else if(eMediaDisplay == DualDisplayHelper::eMP_DISPLAY_A)          eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_C;
		    else if(eMediaDisplay == DualDisplayHelper::eMP_DISPLAY_A_AND_B)    eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_C_AND_D;
        }
        // } added by kihyung 2013.08.21 for ITS 0185408        
        #endif

        // { modified for ITS 0177757 2013.07.07
        // if(pVideoEngine->IsStopped() == false || m_currentVideoMediaState == APP_STATE_AUX)
    	//added by edo.lee 2013.11.13 ITS 0208911 
		DualDisplayHelper::MEDIA_DISPLAY eCameraDisplay = (DualDisplayHelper::MEDIA_DISPLAY)m_pEngineMain->GetDisplayPlane(APP_CAMERA);
        if(eCameraDisplay == eMediaDisplay && m_pEngineMain->getCamInFG())
    	{
    		MP_LOG<<"" << LOG_ENDL;
    	}else
	        m_pDDHelper->SetDirectBrightness(brightness, eMediaDisplay);
        // updateAFWScreenSetting(); // added by kihyung 2013.07.29 for ITS 0181531 // removed by kihyung 2013.08.27 for ITS 0186511 
		// } modified by eugene.seo 2013.06.03
	//}
    // } modified by eugene.seo 2013.04.22
}

void ExternalNotifier::setContrast(int contrast)
{
	// removed by Sergey 24.08.2013

    //m_pDDHelper->SetContrast(contrast); // delete by eugene.seo 2013.02.27 for display setting
	if(contrast > 10 || contrast < -10) return; // added by eugene.seo 2013.04.23
    m_currentContrast = contrast;	

    // added by kihyung 2013.08.27 for ITS 0186511 
    if(m_pVideoCtrl->getSource() == MEDIA_SOURCE_MAX)
        return;

	// { modified by eugene.seo 2013.04.22
	//if( m_currentVideoMediaState == APP_STATE_AUX || m_currentVideoMediaState == APP_STATE_JUKEBOX || m_currentVideoMediaState == APP_STATE_DVD )
	//{ // delete by eugene.seo 2013.04.23
		if(m_currentVideoMediaState == APP_STATE_AUX)
			contrast = contrastMap[0][contrast+10];	
		else if(m_currentVideoMediaState == APP_STATE_JUKEBOX)
			contrast = contrastMap[1][contrast+10];	
		else // m_currentVideoMediaState == APP_STATE_DVD
			contrast = contrastMap[2][contrast+10];			

		MP_LOG << contrast << LOG_ENDL;

        #if (AVP_USE_NEW_BLEND_AFW == 0)
		// { modified by eugene.seo 2013.06.03
		AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_pEngines[APP_MEDIA_MODE_VIDEO]);   
        // { added by cychoi 2013-07-15 for Prevent fix
        if(pVideoEngine == NULL)
            return;
        // } added by cychoi 2013-07-15

        // { added by kihyung 2013.07.20 for ITS 0180419 
        APP_MEDIA_MODE_T eViewF = m_pEngineMain->GetAVMediaMode(m_pEngineMain->GetAVPEntryPoint(DISPLAY_FRONT), DISPLAY_FRONT);
        APP_MEDIA_MODE_T eViewR = m_pEngineMain->GetAVMediaMode(m_pEngineMain->GetAVPEntryPoint(DISPLAY_REAR), DISPLAY_REAR);
        // } added by kihyung 2013.07.20 for ITS 0180419  

        DualDisplayHelper::MEDIA_DISPLAY eMediaDisplay;
        if(pVideoEngine->IsPlaying() || m_currentVideoMediaState == APP_STATE_AUX)
		{
		    if(m_displayPlane == OVERLAY_PLANE) {
		        eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_B;
		    }
		    else if(m_displayPlane == SPRITEC_PLANE) {
		        eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_A;
		    }
		    else if(m_displayPlane == OVERLAY_SPRITEC) {
		        eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_A_AND_B;
		    }
            else {
                return;
            }

            if(pVideoEngine->getIsShowLockoutView() != VIDEO_DISPLAY_MAX && m_pEngineMain->isDRSShow() == true) {
                MP_LOG << "lockout mode" << LOG_ENDL;	
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_B;
            }
            // { modified by kihyung 2013.07.20 for ITS 0180419 
            else if(eViewF == APP_MEDIA_MODE_PHOTO_FRONT || eViewR == APP_MEDIA_MODE_PHOTO_REAR) 
            {
                MP_LOG << "photo mode" << LOG_ENDL;	
                if(m_pEngineMain->GetRearView() == APP_MEDIA_MODE_PHOTO_REAR && m_pEngineMain->getCamInFG() == true) {
                    return;
                }
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_B;
            }
            // } modified by kihyung 2013.07.20 for ITS 0180419 
		}
		else
		{
     		if(m_displayPlane == OVERLAY_PLANE) {
		        eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_D;
		    }
		    else if(m_displayPlane == SPRITEC_PLANE) {
		        eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_C;
		    }
		    else if(m_displayPlane == OVERLAY_SPRITEC) {
		        eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_C_AND_D;
		    }
            else {
                return;
            }

            if(pVideoEngine->getIsShowLockoutView() != VIDEO_DISPLAY_MAX && m_pEngineMain->isDRSShow() == true) {
                MP_LOG << "lockout mode" << LOG_ENDL;	
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_D;
            }
            // { modified by kihyung 2013.07.20 for ITS 0180419 
            else if(eViewF == APP_MEDIA_MODE_PHOTO_FRONT || eViewR == APP_MEDIA_MODE_PHOTO_REAR) 
            {
                MP_LOG << "photo mode" << LOG_ENDL;	
                if(m_pEngineMain->GetRearView() == APP_MEDIA_MODE_PHOTO_REAR && m_pEngineMain->getCamInFG() == true) {
                    return;
                }
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_D;
            }
            // } modified by kihyung 2013.07.20 for ITS 0180419 
		}
		#else
        DualDisplayHelper::MEDIA_DISPLAY eMediaDisplay = (DualDisplayHelper::MEDIA_DISPLAY)m_pEngineMain->GetDisplayPlane(APP_AVP);

        // { added by kihyung 2013.08.21 for ITS 0185408
        AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_pEngines[APP_MEDIA_MODE_VIDEO]);   
        if(pVideoEngine == NULL)
            return;
        
        if(pVideoEngine->IsPlaying() == false && m_currentVideoMediaState != APP_STATE_AUX)
        {
            if(eMediaDisplay == DualDisplayHelper::eMP_DISPLAY_B)               eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_D;
		    else if(eMediaDisplay == DualDisplayHelper::eMP_DISPLAY_A)          eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_C;
		    else if(eMediaDisplay == DualDisplayHelper::eMP_DISPLAY_A_AND_B)    eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_C_AND_D;
        }
        // } added by kihyung 2013.08.21 for ITS 0185408 
        #endif
		
        // { modified for ITS 0177757 2013.07.07
        // if(pVideoEngine->IsStopped() == false || m_currentVideoMediaState == APP_STATE_AUX)
        //added by edo.lee 2013.11.13 ITS 0208911 
		DualDisplayHelper::MEDIA_DISPLAY eCameraDisplay = (DualDisplayHelper::MEDIA_DISPLAY)m_pEngineMain->GetDisplayPlane(APP_CAMERA);
        if(eCameraDisplay == eMediaDisplay && m_pEngineMain->getCamInFG())
    	{
    		MP_LOG<<"" << LOG_ENDL;
    	}else
           m_pDDHelper->SetDirectContrast(contrast, eMediaDisplay);
        // updateAFWScreenSetting(); // added by kihyung 2013.07.29 for ITS 0181531 // removed by kihyung 2013.08.27 for ITS 0186511 
		//} modified by eugene.seo 2013.06.03
	//}
	// } modified by eugene.seo 2013.04.22
}

void ExternalNotifier::setTint(int tint)
{
	// removed by Sergey 24.08.2013

	//m_pDDHelper->SetTint(tint); // delete by eugene.seo 2013.02.27 for display setting
	if(tint > 10 || tint < -10) return; // added by eugene.seo 2013.04.23
	m_currentTint = tint;

    // added by kihyung 2013.08.27 for ITS 0186511 
    if(m_pVideoCtrl->getSource() == MEDIA_SOURCE_MAX)
        return;

	// { modified by eugene.seo 2013.04.22
	//if( m_currentVideoMediaState == APP_STATE_AUX || m_currentVideoMediaState == APP_STATE_JUKEBOX || m_currentVideoMediaState == APP_STATE_DVD )
	//{ // delete by eugene.seo 2013.04.23
		if(m_currentVideoMediaState == APP_STATE_AUX)
			tint = tintMap[0][tint+10];	
		else if(m_currentVideoMediaState == APP_STATE_JUKEBOX)
			tint = tintMap[1][tint+10];	
		else
			tint = tintMap[2][tint+10];

		MP_LOG << tint << LOG_ENDL;

        #if (AVP_USE_NEW_BLEND_AFW == 0)
		// { modified by eugene.seo 2013.06.03
		AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_pEngines[APP_MEDIA_MODE_VIDEO]);   
        // { added by cychoi 2013-07-15 for Prevent fix
        if(pVideoEngine == NULL)
            return;
        // } added by cychoi 2013-07-15

        // { added by kihyung 2013.07.20 for ITS 0180419 
        APP_MEDIA_MODE_T eViewF = m_pEngineMain->GetAVMediaMode(m_pEngineMain->GetAVPEntryPoint(DISPLAY_FRONT), DISPLAY_FRONT);
        APP_MEDIA_MODE_T eViewR = m_pEngineMain->GetAVMediaMode(m_pEngineMain->GetAVPEntryPoint(DISPLAY_REAR), DISPLAY_REAR);
        // } added by kihyung 2013.07.20 for ITS 0180419  
		
        DualDisplayHelper::MEDIA_DISPLAY eMediaDisplay;
        if(pVideoEngine->IsPlaying() || m_currentVideoMediaState == APP_STATE_AUX)
        {
            if(m_displayPlane == OVERLAY_PLANE) {
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_B;
            }
            else if(m_displayPlane == SPRITEC_PLANE) {
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_A;
            }
            else if(m_displayPlane == OVERLAY_SPRITEC) {
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_A_AND_B;
            }
            else {
                return;
            }

            if(pVideoEngine->getIsShowLockoutView() != VIDEO_DISPLAY_MAX && m_pEngineMain->isDRSShow() == true) {
                MP_LOG << "lockout mode" << LOG_ENDL;	
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_B;
            }
            // { modified by kihyung 2013.07.20 for ITS 0180419 
            else if(eViewF == APP_MEDIA_MODE_PHOTO_FRONT || eViewR == APP_MEDIA_MODE_PHOTO_REAR) 
            {
                MP_LOG << "photo mode" << LOG_ENDL;	
                if(m_pEngineMain->GetRearView() == APP_MEDIA_MODE_PHOTO_REAR && m_pEngineMain->getCamInFG() == true) {
                    return;
                }
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_B;
            }
            // } modified by kihyung 2013.07.20 for ITS 0180419 
		}
		else
		{
     		if(m_displayPlane == OVERLAY_PLANE) {
		        eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_D;
		    }
		    else if(m_displayPlane == SPRITEC_PLANE) {
		        eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_C;
		    }
		    else if(m_displayPlane == OVERLAY_SPRITEC) {
		        eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_C_AND_D;
		    }	
            else {
                return;
            }

            if(pVideoEngine->getIsShowLockoutView() != VIDEO_DISPLAY_MAX && m_pEngineMain->isDRSShow() == true) {
                MP_LOG << "lockout mode" << LOG_ENDL;	
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_D;
            }
            // { modified by kihyung 2013.07.20 for ITS 0180419 
            else if(eViewF == APP_MEDIA_MODE_PHOTO_FRONT || eViewR == APP_MEDIA_MODE_PHOTO_REAR) 
            {
                MP_LOG << "photo mode" << LOG_ENDL;	
                if(m_pEngineMain->GetRearView() == APP_MEDIA_MODE_PHOTO_REAR && m_pEngineMain->getCamInFG() == true) {
                    return;
                }
                eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_D;
            }
            // } modified by kihyung 2013.07.20 for ITS 0180419 
		}
		#else
        DualDisplayHelper::MEDIA_DISPLAY eMediaDisplay = (DualDisplayHelper::MEDIA_DISPLAY)m_pEngineMain->GetDisplayPlane(APP_AVP);

        // { added by kihyung 2013.08.21 for ITS 0185408
        AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_pEngines[APP_MEDIA_MODE_VIDEO]);   
        if(pVideoEngine == NULL)
            return;
        
        if(pVideoEngine->IsPlaying() == false && m_currentVideoMediaState != APP_STATE_AUX)
        {
            if(eMediaDisplay == DualDisplayHelper::eMP_DISPLAY_B)               eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_D;
		    else if(eMediaDisplay == DualDisplayHelper::eMP_DISPLAY_A)          eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_C;
		    else if(eMediaDisplay == DualDisplayHelper::eMP_DISPLAY_A_AND_B)    eMediaDisplay = DualDisplayHelper::eMP_DISPLAY_C_AND_D;
        }
        // } added by kihyung 2013.08.21 for ITS 0185408 
        #endif
		
        // { modified for ITS 0177757 2013.07.07
        // if(pVideoEngine->IsStopped() == false || m_currentVideoMediaState == APP_STATE_AUX)
        //added by edo.lee 2013.11.13 ITS 0208911 
		DualDisplayHelper::MEDIA_DISPLAY eCameraDisplay = (DualDisplayHelper::MEDIA_DISPLAY)m_pEngineMain->GetDisplayPlane(APP_CAMERA);
        if(eCameraDisplay == eMediaDisplay && m_pEngineMain->getCamInFG())
    	{
    		MP_LOG<<"" << LOG_ENDL;
    	}else
            m_pDDHelper->SetDirectTint(tint, eMediaDisplay);
        // updateAFWScreenSetting(); // added by kihyung 2013.07.29 for ITS 0181531 // removed by kihyung 2013.08.27 for ITS 0186511 
		// } modified by eugene.seo 2013.06.03
	//}
	// } modified by eugene.seo 2013.04.22
}

void ExternalNotifier::setHue(int hue)
{
    Q_UNUSED(hue)
    /*
    MP_LOG << hue << LOG_ENDL;
    
    m_pDDHelper->SetHue(hue);
    m_currentHue = hue;
    */
}

// { modified by Sergey 25.08.2013 for ITS#186166
void ExternalNotifier::onBrightnessChanged(int brightness)
{
    // if(m_pVideoCtrl->getSource() != MEDIA_SOURCE_MAX) // removed by kihyung 2013.08.27 for ITS 0186511
    setBrightness(brightness);

    m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_BRIGHTNESS, brightness);
    if(m_pSettingsStorage && m_displayPlaneData[APP_STATE_JUKEBOX].m_brightness != brightness)
    {
        m_displayPlaneData[APP_STATE_JUKEBOX].m_brightness = brightness;
        m_pSettingsStorage->SaveSetting(brightness, VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_BRIGHTNESS, APP_STATE_JUKEBOX);
    }

    updateAFWScreenSetting(); // added by kihyung 2013.08.27 for ITS 0186511 
}

void ExternalNotifier::onContrastChanged(int contrast)
{
    // if(m_pVideoCtrl->getSource() != MEDIA_SOURCE_MAX) // removed by kihyung 2013.08.27 for ITS 0186511 
	setContrast(contrast);

	m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_CONTRAST, contrast);
	if(m_pSettingsStorage && m_displayPlaneData[APP_STATE_JUKEBOX].m_contrast != contrast)
	{
		m_displayPlaneData[APP_STATE_JUKEBOX].m_contrast = contrast;
		m_pSettingsStorage->SaveSetting(contrast, VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_CONTRAST, APP_STATE_JUKEBOX);
	}

    updateAFWScreenSetting(); // added by kihyung 2013.08.27 for ITS 0186511 
}

void ExternalNotifier::onTintChanged(int tint)
{
    // if(m_pVideoCtrl->getSource() != MEDIA_SOURCE_MAX) // removed by kihyung 2013.08.27 for ITS 0186511 
    setTint(tint);

    m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_SATURATION, tint);
    if(m_pSettingsStorage && m_displayPlaneData[APP_STATE_JUKEBOX].m_tint != tint)
    {
        m_displayPlaneData[APP_STATE_JUKEBOX].m_tint = tint;
        m_pSettingsStorage->SaveSetting(tint, VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_SATURATION, APP_STATE_JUKEBOX);
    }

    updateAFWScreenSetting(); // added by kihyung 2013.08.27 for ITS 0186511 
}
// } modified by Sergey 25.08.2013 for ITS#186166


void ExternalNotifier::onHueChanged(int hue)
{
	// removed by Sergey 24.08.2013
    //if(m_currentVideoMediaState != APP_STATE_NONE) // delete by eugene.seo 2013.04.22
    //{
        setHue(hue);
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_VIDEO_IMAGE_HUE, hue);
        if(m_pSettingsStorage && m_displayPlaneData[APP_STATE_JUKEBOX].m_hue != hue)
        {
            m_displayPlaneData[APP_STATE_JUKEBOX].m_hue = hue;
            m_pSettingsStorage->SaveSetting(hue, VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_HUE, APP_STATE_JUKEBOX);
        }
    //}
}


// { added by kihyung 2013.2.4 for ISV 71353
void ExternalNotifier::onUpdateMainMenuAfterBoot()
{
    MP_LOG << LOG_ENDL;

    // m_pEngineMain->ConnectAVP2UISH(); // removed by kihyung 2013.07.31

    if(m_pEngineMain) 
    {
        if(m_pEngineMain->isTAReadComplete(eJukeBox) == false) {
            m_pEngineMain->requestFMReadTAScan(eJukeBox);
        }

        if(m_pEngineMain->isTAReadComplete(eUSB_FRONT) == false) {
            m_pEngineMain->requestFMReadTAScan(eUSB_FRONT);
        }

        if(m_pEngineMain->isTAReadComplete(eUSB_REAR) == false) {
            m_pEngineMain->requestFMReadTAScan(eUSB_REAR);
        }
    }
}
// added by kihyung 2013.2.4 for ISV 71353

void ExternalNotifier::InitilizeDisplayParam()
{
    MP_LOG << LOG_ENDL;
    m_pSettingsStorage = AppMediaPlayer_VideoSettings::GetInstance();
    //for (int index = (int)APP_STATE_NONE; index < APP_STATE_MAX; index++) // delete by eugene.seo 2013.04.22
    //{
        //APP_STATE_T state = APP_STATE_T(index); // delete by eugene.seo 2013.04.22
        APP_STATE_T state = APP_STATE_JUKEBOX; // added by eugene.seo 2013.04.22
        if(m_pSettingsStorage)
        {
            m_displayPlaneData[state].m_brightness  = m_pSettingsStorage->LoadSetting(VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_BRIGHTNESS, state);
            m_displayPlaneData[state].m_tint        = m_pSettingsStorage->LoadSetting(VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_SATURATION, state);
            m_displayPlaneData[state].m_hue         = m_pSettingsStorage->LoadSetting(VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_HUE, state);
            m_displayPlaneData[state].m_contrast    = m_pSettingsStorage->LoadSetting(VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_CONTRAST, state);

            MP_LOG << m_displayPlaneData[state].m_brightness
                  << m_displayPlaneData[state].m_tint
                  << m_displayPlaneData[state].m_hue
                  << m_displayPlaneData[state].m_contrast << LOG_ENDL;
        }
        else
        {
            m_displayPlaneData[state].m_brightness  = 0;
            m_displayPlaneData[state].m_tint        = 0;
            m_displayPlaneData[state].m_hue         = 0;
            m_displayPlaneData[state].m_contrast    = 0;
        }
    //}

    // { modified by kihyung 2013.07.26 for ITS 0181345
    m_currentBrightness = m_displayPlaneData[state].m_brightness;
    m_currentTint       = m_displayPlaneData[state].m_tint;
    m_currentHue        = m_displayPlaneData[state].m_hue;
    m_currentContrast   = m_displayPlaneData[state].m_contrast;
    // } modified by kihyung 2013.07.26 for ITS 0181345

    updateAFWScreenSetting(); // added by kihyung 2013.07.29 for ITS 0181531 
}
// } added by ravikanth 27-12-12

void ExternalNotifier::setOverlayPlanes(DISPLAY_PLANES_T eDispPlane, bool bForce)
{
// { modified by kihyung 2013.08.13 for ITS 0183749 
#if (AVP_USE_NEW_BLEND_AFW == 0)    
    MP_HIGH << m_displayPlane << eDispPlane << m_currentVideoMediaState << bForce << LOG_ENDL;
    
    if(m_displayPlane == eDispPlane && bForce == false) return;

    m_displayPlane = eDispPlane;

    if(eDispPlane == OVERLAY_PLANE) {
        m_pDDHelper->SetDisplayPlane(DualDisplayHelper::eMP_DISPLAY_B);
    }
    else if(eDispPlane == SPRITEC_PLANE) {
        m_pDDHelper->SetDisplayPlane(DualDisplayHelper::eMP_DISPLAY_A);
    }
    else if(eDispPlane == OVERLAY_SPRITEC) {
        m_pDDHelper->SetDisplayPlane(DualDisplayHelper::eMP_DISPLAY_A_AND_B);
    }

    if(m_currentVideoMediaState > APP_STATE_NONE && m_currentVideoMediaState < APP_STATE_MAX) 
    {
        // { modified by ravikanth 28-03-13
        setBrightness(m_currentBrightness);

        setContrast(m_currentContrast);

        setTint(m_currentTint);

        setHue(m_currentHue);
        // } modified by ravikanth 28-03-13
    }
#else
    Q_UNUSED(eDispPlane)
    Q_UNUSED(bForce)
#endif
// { modified by kihyung 2013.08.13 for ITS 0183749 
}
// { added by wspark 2013.03.16
void ExternalNotifier::onNotifyBtCallProgressing(bool value)
{
    MP_LOG << LOG_ENDL;
    m_bBtCallProgressing = value;
}
// } added by wspark

// { added by wspark 2013.04.10 for cluster update
void ExternalNotifier::setClusterUpdate(bool value)
{
    // { added by wspark 2013.04.28 for cluster IOT
    if(value == false)
    {
        sendCanTpMessage(TPCanID[m_mode][m_device], "");
    }
    // } added by wspark
    m_bClusterUpdate = value;
    MP_LOG << "m_bClusterUpdate = " << m_bClusterUpdate << LOG_ENDL;
} 
// } added by wspark 

// { added by wspark 2013.04.09 for cluster IOT #37
void ExternalNotifier::onClearTpMessage()
{
    if(m_device >= MEDIA_SOURCE_MAX) return; // added by kihyung 2013.07.17 for ISV 86690

    // { modified by wspark 2013.04.21 for cluster IOT
    /*
    m_devices[m_device].m_title = "";
    m_bSendingCanProgressMessage = false; // added by wspark 2013.04.17
    */
    if((m_device == MEDIA_SOURCE_IPOD1 || m_device == MEDIA_SOURCE_IPOD2)
            && m_devices[m_device].m_quantity <= 1 && m_bFirstUpdateTrack == false)
    {
        MP_LOG << "Only one song in ipod playlist, don't clear tp" << LOG_ENDL;
        m_bSendingCanProgressMessage = true;
        m_bTpNeedToSend = true;
    }
    else
    {
        m_devices[m_device].m_title = "";
        m_bSendingCanProgressMessage = false; // added by wspark 2013.04.17
    }
    // } modified by wspark
    if(m_device != MEDIA_SOURCE_DVDVIDEO)
    {
        m_devices[m_device].m_videoTpMessage = "";

        quint64 data = GetClusterState();
        SendClusterData_OpStateOnly(data);
        sendCanTpMessage(TPCanID[m_mode][m_device], "");
    }
    // } modified by wspark
    m_bFirstUpdateTrack = true; // added by wspark 2013.04.14 for cluster IOT #40
}
// } added by wspark

// { modified by wspark 2013.04.30 for cluster IOT
/*
// { added by wspark 2013.04.09 for cluster IOT #34
void ExternalNotifier::onSendTpMessage( QString title)
{
    MP_LOG << "title = " << title << LOG_ENDL;

    if ( m_device < MEDIA_SOURCE_MAX )
    {
        if(m_devices[m_device].m_title != title)
        {
		// { modified by wspark 2013.04.17
            //m_bAudioTpNeedToSend = true;
			m_bTpNeedToSend = true;
			// } modified by wspark
            m_devices[m_device].m_title = title;
        }
    }
}
// } added by wspark
*/
void ExternalNotifier::onSendTpMessage( QString title, int device)
{
    if ( device < MEDIA_SOURCE_MAX )
    {
            m_devices[device].m_title = title;
            if(device == MEDIA_SOURCE_BLUETOOTH)
            {
                quint64 data = clusterStates[m_mode][device];
                SendClusterData_OpStateOnly(data);
                sendCanTpMessage(TPCanID[m_mode][device], m_devices[device].m_title);
            }
            else
            {
                m_bTpNeedToSend = true;
            }
    }
}
// } modified by wspark
// { added by wspark 2013.04.15 for EVT_UPDATE_CLUSTER
void ExternalNotifier::onSendTpMessage()
{
    if ( m_device < MEDIA_SOURCE_MAX )
    {
        // { modified by wspark 2013.04.28 for cluster IOT
        //m_bTpNeedToSend = true;
        // { modified by cychoi 2013.06.10 for code arrangement
        //if(m_device != MEDIA_SOURCE_VCD)
        //{
            m_bTpNeedToSend = true;
        //}
        // } modified by cychoi 2013.06.10
        // } modified by wspark

        MP_LOG << "m_mode=" << m_mode << "m_device=" << m_device << LOG_ENDL;

        if(m_device == MEDIA_SOURCE_BLUETOOTH)
        {
            quint64 data = GetClusterState();
            SendClusterData_OpStateOnly(data);
            sendCanTpMessage(TPCanID[m_mode][m_device], m_devices[m_device].m_title);
        }
        // { modified by yungi 2013.11.05 for Cluster DVD-Video Chapter ,TitleDiscMenu // added by cychoi 2013.09.03 for cluster update on device changed
        else
        {
            // { modified by cychoi 2013.11.15 for HMC QC Cluser Play Time Error
            // { modified by yungi 2013.11.03 for Cluster BT-Call end Not State Changed When DVD TItle Disc Menu
            quint64 data = GetClusterState();
            SendClusterData_OpStateOnly(data); // rollback by yungi 2013.11.04 Cluster BT-Call END

            AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_pEngines[APP_MEDIA_MODE_VIDEO]); // added by yungi 2013.11.03 for Cluster BT-Call end Not State Changed When DVD TItle Disc Menu
            if(pVideoEngine != NULL)
            {
                if(m_device != MEDIA_SOURCE_DVDVIDEO || pVideoEngine->isDVDTrackPlaying())
                {
                    int nCluseterSeconds = m_devices[m_device].m_position/1000;
                    SendClusterData_Progress(nCluseterSeconds);
                }
            }
            // } modified by yungi 2013.11.03 for Cluster BT-Call end Not State Changed When DVD TItle Disc Menu
            // } modified by cychoi 2013.11.15
        }
        // } modified by yungi
    }
}
// } added by wspark

void ExternalNotifier::onChangeBasicControlEnable(bool isBasicControlEnable) //HMC 2014.08.20 iPod app mode clear time
{
    MP_LOG<<"m_bBasicControlEnable " << isBasicControlEnable << LOG_ENDL;
    m_bBasicControlEnable = isBasicControlEnable;

    if( !m_bBasicControlEnable && ( m_device == MEDIA_SOURCE_IPOD1 ||  m_device == MEDIA_SOURCE_IPOD2 ))
    {
        quint64 data = GetClusterState();
        SendClusterData_OpStateOnly(data);
    }
}

// { added by cychoi 2013.09.03 for ITS 187585
void ExternalNotifier::onRetranslateUI()
{
    if(m_device == MEDIA_SOURCE_DVDVIDEO || m_device == MEDIA_SOURCE_DVDAUDIO)
    {
        onFoldernameChanged(QString::number(m_devices[m_device].m_number));
    }
    else if(m_device == MEDIA_SOURCE_VCD)
    {
        onVideoFilenameChanged(QString::number(m_devices[m_device].m_number));
    }
}
// } added by cychoi 2013.09.03

//added by edo.lee 2013.07.05
void ExternalNotifier::setHistoryPlayMode(int random, int repeat)
{
    if(m_device >= MEDIA_SOURCE_MAX) return; // added by kihyung 2013.07.17 for ISV 86690
    
	MP_LOG<<"random " << random<<"repeat "<< repeat << LOG_ENDL;
     m_devices[m_device].m_playingRandomMode = random; // modified by yongkyun.lee 2013-07-15 for : ITS 179776
     m_devices[m_device].m_playingRepeatMode = repeat; // modified by yongkyun.lee 2013-07-15 for : ITS 179776
	//added by edo.lee 2013.07.06
	//if(random != -1 && repeat != -1 &&  m_updatePositionTimer.isActive())
	//{
	//	m_updatePositionTimer.stop();
                //DisplayAudioTrackOSD();
	//}
	//added by edo.lee 2013.07.06	
}
//added by edo.lee 2013.07.05
void ExternalNotifier::setHistoryInfo(MEDIA_SOURCE_T device, QString filename, QString foldername, int position)
{
    if(device >= MEDIA_SOURCE_MAX) return; // added by kihyung 2013.07.17 for ISV 86690

    // { commented by cychoi 2013.06.24 for cluster IOT
    // { modified by wspark 2013.04.30 for cluster IOT
    //sendCanTpMessage(TPCanID[m_mode][m_device], ""); // added by wspark 2013.04.29 for cluster IOT
    //if(device != m_device)
    //{
    //    sendCanTpMessage(TPCanID[m_mode][m_device], "");
    //}
    // } modified by wspark
    // } commented by cychoi 2013.06.24

    m_device = device; // moved by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
    //m_activeDevice = device; // added by cychoi 2013.09.03 for cluster update on device changed

    m_devices[device].m_folderName = foldername;
    m_devices[device].m_position = position;
    m_devices[device].m_title = filename;
}
//added by edo.lee 2013.07.06
/*
bool ExternalNotifier::continueDisplayOSD(bool isAudio)
{
	if(m_updatePositionTimer.isActive())
	{
		m_updatePositionTimer.stop();
		if(isAudio)
			DisplayAudioTrackOSD();
		else
			DisplayVideoInfo();
		return true;
	}	
	return false;
}
*/
//added by edo.lee 2013.07.06
//added by edo.lee 2013.05.08
void ExternalNotifier::setHistoryTuneInfo()
{
	MP_LOG<<"m_SearchTitle "<< m_SearchTitle << LOG_ENDL;
	if(m_SearchTitle != " ")
		setHistoryInfo(m_device, m_SearchTitle, "",0);
	m_SearchTitle = " ";
}
//added by edo.lee 2013.05.08
// { added by wspark 2013.04.21 for cluster IOT
void ExternalNotifier::onPlayListChanged()
{
    m_bFirstUpdateTrack = true;
}

// { added by wspark 2013.04.23 for cluster IOT
bool ExternalNotifier::GetSharedMemoryInfo(int offset, unsigned char * pBuf, int size)
{
    if ( !mShmCurrentAVInfo.isAttached() )
    {
        MP_HIGH << QString( "Lost Connection with the shared memroy -> error code : %1" ).arg( mShmCurrentAVInfo.error() ) << LOG_ENDL;
        return false;
    }

    mShmCurrentAVInfo.lock();
    memcpy( pBuf, ((unsigned char *)mShmCurrentAVInfo.data())+offset, size );
    mShmCurrentAVInfo.unlock();

    return true;
}

bool ExternalNotifier::SetSharedMemoryInfo(int offset, unsigned char * pBuf, int size)
{
    if ( !mShmCurrentAVInfo.isAttached() )
    {
        MP_HIGH << QString( "Lost Connection with the shared memroy -> error code : %1" ).arg( mShmCurrentAVInfo.error() ) << LOG_ENDL;
        return false;
    }

    mShmCurrentAVInfo.lock();
    memcpy( ((unsigned char *)mShmCurrentAVInfo.data())+offset, pBuf, size );
    mShmCurrentAVInfo.unlock();

    return true;
}

// { added by kihyung 2013.07.17 for NEW AFW    
void ExternalNotifier::updateAFWScreenSetting()
{
#if AVP_USE_NEW_BLEND_AFW    
    MP_LOG << m_currentBrightness << m_currentContrast << m_currentTint << LOG_ENDL;
    
    if(m_currentBrightness > 10 || m_currentBrightness < -10) return;
    if(m_currentContrast > 10 || m_currentContrast < -10) return;
    if(m_currentTint > 10 || m_currentTint < -10) return;
    
    m_pEngineMain->SetAppDisplaySetting(AppEngineBase::ADS_AVP_JUKEBOX, 
                                        brightnessMap[1][m_currentBrightness+10],
                                        contrastMap[1][m_currentContrast+10],
                                        tintMap[1][m_currentTint+10],
                                        0);
    m_pEngineMain->SetAppDisplaySetting(AppEngineBase::ADS_AVP_USB, 
                                        brightnessMap[1][m_currentBrightness+10],
                                        contrastMap[1][m_currentContrast+10],
                                        tintMap[1][m_currentTint+10],
                                        0);
    m_pEngineMain->SetAppDisplaySetting(AppEngineBase::ADS_AVP_DISC, 
                                        brightnessMap[2][m_currentBrightness+10],
                                        contrastMap[2][m_currentContrast+10],
                                        tintMap[2][m_currentTint+10],
                                        0);
    m_pEngineMain->SetAppDisplaySetting(AppEngineBase::ADS_AVP_AUX, 
                                        brightnessMap[0][m_currentBrightness+10],
                                        contrastMap[0][m_currentContrast+10],
                                        tintMap[0][m_currentTint+10],
                                        0);    
#endif
}
// } added by kihyung 2013.07.17 for NEW AFW

bool ExternalNotifier::IsAvModeFromSharedMemory()
{
    int currentAV;	
    bool ret = GetSharedMemoryInfo(SHM_OFFSET_CURRENT_AV_MODE, (unsigned char *)&currentAV, SHM_CURRENT_AV_MODE_SIZE );//remove by edo.lee 2013.07.15 STATIC TEST
    bool isAvMode;

    if(ret)
    {
        switch(currentAV)
        {
            case MODE_STATE_AUX1_AUDIO:
            case MODE_STATE_AUX1_VIDEO:
            case MODE_STATE_AUX2_AUDIO:
            case MODE_STATE_AUX2_VIDEO:
            case MODE_STATE_JUKEBOX_AUDIO:
            case MODE_STATE_JUKEBOX_VIDEO:
            case MODE_STATE_BT_AUDIO:
            case MODE_STATE_DISC_AUDIO:
            case MODE_STATE_DISC_VIDEO:
            case MODE_STATE_USB1_AUDIO:
            case MODE_STATE_USB1_VIDEO:
            case MODE_STATE_USB2_AUDIO:
            case MODE_STATE_USB2_VIDEO:
            case MODE_STATE_IPOD1_AUDIO:
            case MODE_STATE_IPOD2_AUDIO:
                isAvMode = true;
                break;
            default:
            MP_LOG << "isAvMode = false!!!!!!!!" << LOG_ENDL;
                isAvMode = false;
                break;
        }
    }
    else
    {
        isAvMode = true;
    }
    return isAvMode;
}

//added by edo.lee 2013.06.27
MODE_STATE_T ExternalNotifier::GetCurrentFromSHM()
{
    int currentAV;
    bool ret = GetSharedMemoryInfo(SHM_OFFSET_CURRENT_AV_MODE, (unsigned char *)&currentAV, SHM_CURRENT_AV_MODE_SIZE);//remove by edo.lee 2013.07.15 STATIC TEST
    //bool isAvMode;
	
    if(ret)
    {
        return (MODE_STATE_T)currentAV;
    }
    return MODE_STATE_MAX;
}
//added by edo.lee 2013.06.27
// } added by wspark

//added by Dmitry 12.07.13 for ISV85006
void ExternalNotifier::notifyFMVideoPlaybackChanged(PLAYER_STATE state)
{
   switch(state)
   {
       case PLAYER_PLAY:
       case PLAYER_PAUSE:
       case PLAYER_STOP:
       case PLAYER_FASTFORWARD_X4:
       case PLAYER_FASTFORWARD_X16:
       case PLAYER_FASTFORWARD_X20:
       case PLAYER_REWIND_X4:
       case PLAYER_REWIND_X16:
       case PLAYER_REWIND_X20:
           break;
       default:
           return;
   }

   MP_LOG << "Sending video state to FM = " << (char*)&state << LOG_ENDL;
   Event event( EVT_POST_EVENT,
                           m_pEngineMain->GetThisAppId(),
                           APP_FMGR,
                           EVT_FM_VIDEO_STATE_CHANGED,
                           QByteArray((char*)&state));
   m_pEngineMain->notifyUISH( event );
   
}
//added by Dmitry 12.07.13 for ISV85006

void ExternalNotifier::setAutoChangeFileIndex(int index)
{
    m_autoTrackChangeIndex = index;
}

// added by kihyung 2014.01.14 for ITS 0218420
void ExternalNotifier::setGstPipelineDisplay(DISPLAY_T display)
{
    MP_HIGH << display << LOG_ENDL;
    m_gstPipelineDisplay = display;
}
