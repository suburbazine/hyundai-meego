// { modified by Sergey for CR16250
#include "controller/DHAVN_AppMediaPlayer_Controllers.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerFS.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h"
#include "util/video/DHAVN_AppMediaPlayer_Caption_ImageProvider.h" // modified by ravikanth 04-04-13
#include "Qt/qdeclarativeview.h" // added by edo.lee 2012.12.08 for statusbar
#include "util/audio/DHAVN_AppMediaPlayer_Utils.h" // added by kihyung 2013.2.17
#include <DHAVN_AudioSourceType.h>
#include <DHAVN_GSTAudioRouterInterface.h>
#include "util/osd/OSD_Manager.h"

extern SharedDataProvider *m_pSharedVideoData; // modified by kihyung 2013.1.9 for ISV 69137

AppMediaPlayer_Video_ControllerFS::
AppMediaPlayer_Video_ControllerFS(AppMediaPlayer_Controllers *controllers,
                                  AppMediaPlayer_EngineVideo *engine,
                                  QString mediaRoot,
                                  MEDIA_SOURCE_T mediaSorce,
                                  QObject *parent)
    :QObject(parent),
      m_pIMediaEx(NULL),  // added by kihyung 2013.08.08 for ITS 0181967 
      m_pModel(new AppMediaPlayer_Video_CModel_FS(this)),
      m_pControllers(controllers),
      m_pEngineVideo(engine),
//      m_pUSBController(NULL), // removed by eugeny.novikov 2012.12.10 for CR 16564 fastboot
      m_pJukeboxController(NULL),
      m_pFileOperationHelper(NULL),
      m_mediaSource(mediaSorce),
      m_mediaRoot(mediaRoot),
      m_tempTuneIndex(-1),
      m_isForwardDirection(true),
      m_isSubTitle(true),
      m_isSubTitleString(QString("")),
      m_tuneFile(QString("")), // modified by Sergey for CR#15771
      m_nameFilter(m_pControllers->getExtList(MEDIA_TYPE_VIDEO)),
      m_filesCount(0),
      m_totalFilesSize(0),
      m_attachedJukebox(false), // added by jeeeun 2013.01.05 for device dettached checking 
      m_attachedUSB(false), // added by jeeeun 2013.01.05 for device dettached checking
      m_pBitmapBufAddr(NULL), // modified by ravikanth 04-04-13
      m_currentSkip(false),// modified by Dmitry 13.10.13
      m_pLongBtnTimer(new QTimer(this)), // added by kihyung 2013.12.10 for ITS 0214009 
      m_pSkipTimer(new QTimer(this)), // added by kihyung 2013.12.10 for ITS 0214009 
      m_bFF(false), // added by Dmitry 13.10.13
      m_bIsAutoTrackChanged(false),// added by taihyun.ahn 13.12.04
      m_isSourceActivation(true) // added by Sergey 08.01.2014 for ITS#211412
{
    switch(mediaSorce)
    {
        case MEDIA_SOURCE_JUKEBOX:
        {
            m_pJukeboxController = controllers->GetJukeBoxController();
            break;
        }
        case MEDIA_SOURCE_USB1:
        case MEDIA_SOURCE_USB2:
        {
//            m_pUSBController = controllers->GetUSBController(); // removed by eugeny.novikov 2012.12.10 for CR 16564 fastboot
            //{ added by edo.lee 2012.12.08 for statusbar
            m_pFileOperationHelper = new FileOperationsHelper(new QDeclarativeView());
            //} added by edo.lee
            QTranslator& translator = m_pEngineVideo->getEngineMain().GetTranslator();
            m_pFileOperationHelper->setTranslator(&translator);
            break;
        }
        default:
        {  // make compiler happy
            break;
        }
    }
    m_pIMedia = NULL;
    m_playlist.list = new QList<QString>;
    clearPlaylist();
	m_searchTitle = "";
    // setupFSExplorer(); // removed by kihyung 2013.2.17

	// removed by Sergey 07.09.2013 for ITS#185543
    
    connect(m_pEngineVideo->getEngineMain().GetMediaManager(),
            SIGNAL(sourceAvailableChanged(MEDIA_SOURCE_T,MEDIA_TYPE_T,bool)),
            this,
            SLOT(onSourceAvailableChanged(MEDIA_SOURCE_T,MEDIA_TYPE_T,bool)));

    // { added by kihyung 2013.07.04 for ISV 85455
    m_pAudioSinkTimer = new QTimer(this);
    m_pAudioSinkTimer->setInterval(1000);
    connect(m_pAudioSinkTimer, SIGNAL(timeout()), this, SLOT(onAudioSinkError()));
    // } added by kihyung 2013.07.04 for ISV 85455
// modified by edo.lee 2013.10.12 pop noise
    // m_pLongBtnTimer = new QTimer(this); // removed by kihyung 2013.12.10 for ITS 0214009 
    m_pLongBtnTimer->setSingleShot( true ); // added by Dmitry 13.10.13
    m_pLongBtnTimer->setInterval(400); // modified by Sergey 26.10.2013 for ITS#198709
    connect( m_pLongBtnTimer, SIGNAL(timeout()), this, SLOT(onLongBtnTimerExpired()));

    // { added by Sergey 08.10.2013 for ISV#92307
    // m_pSkipTimer = new QTimer(this); // removed by kihyung 2013.12.10 for ITS 0214009 
    m_pSkipTimer->setInterval(2000);
    connect(m_pSkipTimer, SIGNAL(timeout()), this, SLOT(skip()));
    // } added by Sergey 08.10.2013 for ISV#92307
}

AppMediaPlayer_Video_ControllerFS::~AppMediaPlayer_Video_ControllerFS()
{
    // { removed by kihyung 2013.2.17
    /*
    m_pFSExplorer->deleteLater();
    m_pFSExplorerThread->exit();
    if(!m_pFSExplorerThread->wait(1000)) // give this thread 1 second to finish its processing
        m_pFSExplorerThread->terminate();
    
    delete m_pFSExplorerThread;
    */
    // } removed by kihyung 2013.2.17    
    
    delete m_playlist.list;
    delete m_pFileOperationHelper;
    delete m_pModel;
	delete m_pBitmapBufAddr; // modified by ravikanth 04-04-13
	delete m_pAudioSinkTimer; // added by kihyung 2013.07.04 for ISV 85455
}


void AppMediaPlayer_Video_ControllerFS::dropControllers()
{
    if(m_pAudioSinkTimer) m_pAudioSinkTimer->stop(); // added by kihyung 2013.07.04 for ISV 85455
    
    if ( m_mediaSource != MEDIA_SOURCE_JUKEBOX )
    {
        // { modified by eugeny.novikov 2012.12.10 for CR 16564 fastboot
        if ( GetUSBController() == NULL )
        {
            MP_LOG << "Inconsistent state." << LOG_ENDL;
            return;
        }

        disconnect( GetUSBController(), SIGNAL(OnDurationChanged(int)),
                   this, SLOT(onDurationChanged(int)));
        disconnect( GetUSBController(), SIGNAL(OnAutoTrackChanged(int)),
                this, SLOT(onAutoTrackChanged(int)));
        disconnect( GetUSBController(), SIGNAL(OnTrackChanged(int)),
                   this, SLOT(onTrackChanged(int)));
        disconnect( GetUSBController(), SIGNAL(OnSubtitleChanged(QString)),
                   this, SLOT(onSubtitleChanged(QString)));
        disconnect( GetUSBController(), SIGNAL(HandleXSubtitleChanged(uchar*, int)), //XSubtitle added by jeeeun 2013.04.04
            this, SLOT(onHandleXSubtitleChanged(uchar*, int))); 
        disconnect( GetUSBController(), SIGNAL(OnError(int)),
                   this, SLOT(onError(int)));
        disconnect( GetUSBController(), SIGNAL(OnDRMStatusUpdated(int,int,int)),
                   this, SLOT(onDRMInfo(int,int,int))); // added by Sergey for CR#16015
        disconnect(GetUSBController(), SIGNAL(OnVideoInfo(int,int,int,int,float,float,QString)),
                this, SLOT(onVideoInfo(int,int,int,int,float,float,QString))); // modified for ITS 224766 2014.02.12
        // } modified by eugeny.novikov 2012.12.10

        // { added by kihyung 2013.1.25
        // disconnect(GetUSBController(), SIGNAL(OnStateChanged(int)),
        //        this, SLOT(onStateChanged(int)));
        // } added by kihyung 2013.1.25          
        disconnect(GetUSBController(), SIGNAL(OnTrackBufferred(int, int, int, int)),
            this, SLOT(OnTrackBufferred(int, int, int, int))); //added by edo.lee 2013.08.23
        disconnect( GetUSBController(), SIGNAL(OnRealSoundChanged()), //added by edo.lee 2013.08.26
            this, SLOT(onRealSoundChanged()));

		// removed by Sergey for ITS#181872 30.07.2013
        if ( m_pFileOperationHelper != NULL )
        {
            disconnect( m_pFileOperationHelper, SIGNAL(operationCopyComplete(int,int)), // modified by ravikanth 22-04-13
                       this, SIGNAL(operationCopyComplete()));
            disconnect( m_pFileOperationHelper, SIGNAL(copyError(int)),
                       this, SIGNAL(operationCopyError(int)));
            disconnect( m_pFileOperationHelper, SIGNAL(progress(int,int,int)),
                       this, SLOT(onProgress(int,int,int)));
            // { added by wspark 2012.11.20 for CR15380
            disconnect( m_pFileOperationHelper, SIGNAL(copyRequest(QString,int)),
                       this, SLOT(replaceFileRequest(QString,int))); // modified by ravikanth 07-07-13 for ITS 0178184
            disconnect( m_pFileOperationHelper, SIGNAL(capacityError(int,qint64)),
                       this, SLOT(onCapacityError(int,qint64)));
            disconnect( m_pFileOperationHelper, SIGNAL(fileSystemChanged()), // added by kihyung 2013.2.17
                       this, SLOT(onFileSystemChanged()));
        }
    }
    else
    {
        if ( m_pJukeboxController == NULL )
        {
            MP_LOG << "Inconsistent state." << LOG_ENDL;
            return;
        }

        disconnect( m_pJukeboxController, SIGNAL(OnDurationChanged(int)),
                   this, SLOT(onDurationChanged(int)));
        disconnect( m_pJukeboxController, SIGNAL(OnAutoTrackChanged(int)),
                this, SLOT(onAutoTrackChanged(int)));
        disconnect( m_pJukeboxController, SIGNAL(OnTrackChanged(int)),
                   this, SLOT(onTrackChanged(int)));
        disconnect( m_pJukeboxController, SIGNAL(OnTotalAudioLangReceived(QStringList,int*,int)),
                this, SLOT(onAudioLanguagesReceived(QStringList,int*,int)));  // added by Sergey 20.08.2013 for ITS#184640 
        disconnect( m_pJukeboxController, SIGNAL(OnTotalTextLangReceived(QStringList,int*,int)),
                this, SLOT(onTotalTextLangReceived(QStringList,int*,int)));  // added by Sergey 20.08.2013 for ITS#184640 
        disconnect( m_pJukeboxController, SIGNAL(OnSubtitleChanged(QString)),
                   this, SLOT(onSubtitleChanged(QString)));
        disconnect( m_pJukeboxController, SIGNAL(HandleXSubtitleChanged(uchar*, int)), //XSubtitle added by jeeeun 2013.04.04
            this, SLOT(onHandleXSubtitleChanged(uchar*, int)));
        
        disconnect( m_pJukeboxController, SIGNAL(OnError(int)),
                   this, SLOT(onError(int)));
        disconnect( m_pJukeboxController, SIGNAL(OnDRMStatusUpdated(int,int,int)),
                   this, SLOT(onDRMInfo(int,int,int))); // added by Sergey for CR#16015
        disconnect( m_pJukeboxController, SIGNAL(OnVideoInfo(int,int,int,int,float,float,QString)),
                this, SLOT(onVideoInfo(int,int,int,int,float,float,QString))); // modified for ITS 224766 2014.02.12
        // { added by kihyung 2013.1.25
        // disconnect( m_pJukeboxController, SIGNAL(OnStateChanged(int)),
        //         this, SLOT(onStateChanged(int)));
        // } added by kihyung 2013.1.25                  
         disconnect(m_pJukeboxController, SIGNAL(OnTrackBufferred(int, int, int, int)),
            this, SLOT(OnTrackBufferred(int, int, int, int))); //added by edo.lee 2013.08.23
        disconnect( m_pJukeboxController, SIGNAL(OnRealSoundChanged()), //added by edo.lee 2013.08.26
            this, SLOT(onRealSoundChanged()));

		// removed by Sergey for ITS#181872 30.07.2013
    }
    disconnect( m_pEngineVideo, SIGNAL(fgBgStateChanged(bool)),
               this, SLOT(onFgBgStateChanged(bool)) );
	
    m_pIMedia = NULL;
    m_pIMediaEx = NULL;
    // removed by Sergey for CR#15775
}

// { added by eugeny.novikov 2012.12.10 for CR 16564 fastboot
USBController* AppMediaPlayer_Video_ControllerFS::GetUSBController()
{
    return m_pControllers ? m_pControllers->GetUSBController() : NULL;
}
// } added by eugeny.novikov 2012.12.10

void AppMediaPlayer_Video_ControllerFS::onAudioLanguagesReceived(QStringList langCodeList, int *pStreamIdList, int nCurrStreamId )
{
    MP_LOG << "list =" << langCodeList << LOG_ENDL;
    
    m_pModel->setVoiceLangEnabled(true); // added by Sergey 09.10.2013 for ITS#192050
    m_pModel->setLanguageCode(langCodeList);
    m_pModel->setCurrentStreamId(nCurrStreamId);
}


 // { added by Sergey 09.10.2013 for ITS#184640, 187939 
void AppMediaPlayer_Video_ControllerFS::onTotalTextLangReceived(QStringList langCodeList, int *pStreamIdList, int nCurrStreamId)
{
    MP_LOG << "codes =" << langCodeList << "curStream =" << nCurrStreamId << LOG_ENDL;

    m_pModel->setCaptionLangList(langCodeList);

    // { modified by Sergey 08.01.2014 for ITS#211412
    if(nCurrStreamId < 0)
    {
        if(!m_isSourceActivation)
        {
            m_pModel->setCaptionCurId(0);
        }
    }
    // } modified by Sergey 08.01.2014 for ITS#211412

    m_pModel->setMultipleCaptions(true);
    m_pModel->setSmiCaptions(false);
}
 // } added by Sergey 09.10.2013 for ITS#184640, 187939 


// { added by Sergey for CR#11607
void AppMediaPlayer_Video_ControllerFS::onVideoInfo(int type, int status, int width, int height, float par, float dar, QString fileName)// modified for ITS 224766 2014.02.12
{
    m_pModel->setMultipleCaptions(false); // added by Sergey 09.10.2013 for ITS#187939
	// moved by Sergey 07.01.2014 for ITS#206433
    m_pModel->setVoiceLangEnabled(false); // added by Sergey 09.10.2013 for ITS#192050
    if(!m_isSourceActivation) // added by Sergey 08.01.2014 for ITS#211412
        m_pModel->setCurrentStreamId(0); // added by Sergey 30.10.2013 for ITS#195185

    // { added by kihyung 2013.2.14
    if(m_pEngineVideo->getEngineMain().getCamInFG() == true && m_pEngineVideo->isVideoInFG() == false) {
        MP_LOG << "CAMS. returned in camera mode." << LOG_ENDL;
        return;
    }
    else {
        MP_LOG << "CAMS. continued in non-camera mode." << LOG_ENDL;
    }
    // } added by kihyung 2013.2.14
    
    // { added by jeeeun 2013.01.04 for PAR implementation  
    ASPECT_RATIO_T currAspectRatio; 
    currAspectRatio = m_pEngineVideo->GetVideoController()->getAspectRatio();
    if(type == VIDEOINFO_TYPE_AVI)
    {
        if(status <= 5)
            m_pModel->setReadingFile(true);
        else if(status == 100)
            m_pModel->setReadingFile(false);
    }
    MP_LOG << "par : "<< par << "dar : " << dar <<"currAspectRate : "<< currAspectRatio << LOG_ENDL;
    MP_LOG << "width : "<< width << "height" << height << LOG_ENDL;
    m_pEngineVideo->GetVideoController()->SetPAR(par);
    m_pEngineVideo->GetVideoController()->SetDAR(dar);
    m_pEngineVideo->GetVideoController()->SetActualRect(QRect(0,0, width, height));

	// { modified by Sergey 07.01.2014 for ITS#206433
    //{ modified by edo.lee 2013.12.13 ITS 0215643
    QString divxTemp = ".divx";
    QString * divx = NULL;
    divx = &divxTemp;
    bool isDivxFile = (fileName.endsWith(divx,Qt::CaseInsensitive) || type == VIDEOINFO_TYPE_DIVX);// modified for ITS 224766 2014.02.12
    m_pModel->setSmiCaptions(!isDivxFile);
	// } modified by Sergey 07.01.2014 for ITS#206433
	
    if(currAspectRatio == ASPECT_RATIO_16_9)
    {
        if(isDivxFile) // modified by Sergey 07.01.2014 for ITS#206433
        {
            m_pEngineVideo->GetVideoController()->updateAspectRatioTable(ASPECT_RATIO_16_9, m_pEngineVideo->GetVideoController()->getRectByPAR(par, dar, width, height));
        }
        else
            m_pEngineVideo->GetVideoController()->updateAspectRatioTable(ASPECT_RATIO_16_9,QRect( 0, 0, 1280, 720 ));
    }
    else
    {
        m_pEngineVideo->GetVideoController()->updateAspectRatioTable(ASPECT_RATIO_4_3,QRect( 160, 0, 960, 720 ));
    }

    // { modified by Sergey 23.12.2013 for ITS#216921
    DISPLAY_T widgetDisp = m_pEngineVideo->getCurrentScreen();
    if(m_pEngineVideo->getEngineMain().getCamInFG())
        widgetDisp = m_pEngineVideo->getEngineMain().IsSwapDCEnabled() ? DISPLAY_FRONT : DISPLAY_REAR;

    MP_HIGH << "widgetDisp =" << widgetDisp << LOG_ENDL;

    m_pEngineVideo->GetVideoController()->moveWidget(widgetDisp);
	//    m_pEngineVideo->GetVideoController()->moveWidget(m_pEngineVideo->getCurrentScreen());
	// } modified by Sergey 23.12.2013 for ITS#216921

    //} modified by edo.lee 2013.12.13 ITS 0215643 
	// } added by jeeeun 2013.01.04 for PAR implementation
}
// } added by Sergey for CR#11607

// removed by Sergey 18.05.2013

AppMediaPlayer_Video_CModel_Base *AppMediaPlayer_Video_ControllerFS::getModel()
{
    return qobject_cast<AppMediaPlayer_Video_CModel_Base*>(m_pModel);
}

void AppMediaPlayer_Video_ControllerFS::playFile(QString filepath, bool bFromFM, int position)
{
    if(!m_pIMedia)
    {
        MP_LOG << "m_pIMedia is NULL. Return." << LOG_ENDL;
        return;
    }

    MP_LOG << "file =" << filepath << LOG_ENDL;

    if(!m_playlist.list->contains(filepath))
        addToPlaylist(filepath);

    m_playlist.category = 1;
    // m_playlist.isMLT    = false; //added by jeeeun 2012.12.08 for MLT initializing // removed by kihyung 2013.07.18 for ITS 0180268
    m_playlist.index    = m_playlist.list->indexOf(filepath);
    m_playlist.position = position;

    // If source is active then reset playlist and play new one.
    if(m_mediaSource == m_pEngineVideo->GetVideoController()->getSource())
    {
        stop();
        setPlaylist(m_playlist);

        if( m_pModel && bFromFM == true && m_pModel->repeatMode() == AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_FILE) {
            m_pIMedia->RepeatPlay( REPEATALL );
        }
        
        if(bFromFM == false) {
            play();
        }
        else {
            MP_LOG  << "play when received EVT_REQUEST_FG" << LOG_ENDL;
        }
    }
}

void AppMediaPlayer_Video_ControllerFS::copyFileToJukebox()
{
    MP_LOG << LOG_ENDL;
    // { added by sungha.choi 2013.07.15 for static test
    if(m_pFileOperationHelper == NULL){
        return;
    }
    // } added by sungha.choi 2013.07.15 for static test
	
	// { added by edo.lee 2012.12.08 for statusbar
        m_pFileOperationHelper->setDeclarativeView(m_pEngineVideo->getEngineMain()
		.GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN));
	// } added by edo.lee
	
    m_pFileOperationHelper->setDisplay(m_pEngineVideo->getCurrentScreen()); // modified by eugene.seo 2012.10.31 for Function_USB_0863
    if ( m_pIMedia && m_pFileOperationHelper )
    {
        QList<QString> list;
        list.clear();
        list.append(m_pIMedia->GetCurrentFileName());
        MP_LOG << " list.count() = " << list.count() << LOG_ENDL;
        m_pFileOperationHelper->copy(list,"/usr_data/media/Video");
    }
}

// { added by wspark 2012.08.21 for DQA #47
void AppMediaPlayer_Video_ControllerFS::overwriteCopyOperation() 
{
    MP_LOG << LOG_ENDL;
    if ( m_pIMedia && m_pFileOperationHelper )
    {
        m_pFileOperationHelper->setOverwriteMode(OVERWRITE_ONE_EXIST_FILE_MODE);
    }
}
// } added by wspark 2012.08.21 for DQA #47

void AppMediaPlayer_Video_ControllerFS::cancelCopyOperation()
{
    MP_LOG << LOG_ENDL;

    if ( m_pIMedia && m_pFileOperationHelper )
    {
        m_pFileOperationHelper->setOverwriteMode(CANCEL_COPY_OPERATION_MODE); // added by wspark 2012.08.21 for DQA #47
        m_pFileOperationHelper->cancelOperation();
    }
}

void AppMediaPlayer_Video_ControllerFS::onDurationChanged( int duration )
{
    MP_LOG << duration << LOG_ENDL;

    // removed by kihyung 2013.09.11
    
    m_pModel->setDuration( duration );
    checkRestoreData(duration);
    emit durationChanged( duration );
    m_pModel->setFileFromFM(""); // added by Sergey 15.09.2013 for ITS#185233
    m_pEngineVideo->setPlayRequester(PLAY_REQUSTER_MAX); // added by Sergey 21.09.2013 for ITS#190305

    // { added by Sergey 02.10.2013 for ITS#192855
    MediaInfo mediaInfo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    m_pIMedia->GetCurrentMediaInfo( &mediaInfo );

    /* Check if folder got changed. If changed request files of new folder. */
    int lastSlashIndex = mediaInfo.fileName.lastIndexOf('/');
    if(lastSlashIndex < 0)
        lastSlashIndex = 0;

    QString fullFolderName = mediaInfo.fileName.left(lastSlashIndex);
    if(fullFolderName != m_pModel->fullfoldername())
    {
        m_pModel->setFullFolderName(fullFolderName);
        // requestCurrFolderList(fullFolderName); // removed by kihyung 2013.10.08 for ITS 0194414 
    }
    // } added by Sergey 02.10.2013 for ITS#192855

    if(m_pModel->captionEnable())
        m_pIMedia->EnableSubTitle(true); // added by Sergey 26.10.2013 for ITS#196693
}

void AppMediaPlayer_Video_ControllerFS::onTrackChanged( int trackNo )
{
    MP_LOG << "trackNo =" << trackNo << LOG_ENDL;

    m_pEngineVideo->setShowOSDFromBUfferedTrack(false); //added by Puneet 2013.07.03 ITS# 0177010
    // { modified by kihyung 2013.06.26
    MediaInfo mediaInfo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    if(m_pIMedia)
    {
        m_pAudioSinkTimer->stop(); // added by kihyung 2013.07.04 for ISV 85455
    
        setCheckForPlayableFile(true); 
        m_pIMedia->GetCurrentMediaInfo( &mediaInfo );

        // { added by kihyung 2013.07.22 for ITS 0179477
        if(mediaInfo.fileName.isEmpty() == true) {
            MP_HIGH << "mediaInfo.fileName.isEmpty() == true" << LOG_ENDL;
            return;
        }
        // } added by kihyung 2013.07.22 for ITS 0179477
        
        m_pModel->setWidth( mediaInfo.width );
        m_pModel->setHeight( mediaInfo.height );
        m_pModel->setPlayingUrl(mediaInfo.fileName);
        m_pModel->setLastPlayedUrl("");

        // removed by Sergey 21.09.2013 for ITS#190305

        int lastSlashIndex = mediaInfo.fileName.lastIndexOf('/');
        if(lastSlashIndex < 0) {
            MP_HIGH << "error lastSlashIndex " << lastSlashIndex << LOG_ENDL;
            MP_HIGH << "error filepath " << mediaInfo.fileName << LOG_ENDL;
            lastSlashIndex = 0;
        }
        
		// removed by Sergey 02.10.2013 for ITS#192855
		
		if(m_mediaSource == MEDIA_SOURCE_JUKEBOX)
			m_pEngineVideo->updateOSDMetaData(JUKEBOX_VIDEO, mediaInfo.fileName.mid(lastSlashIndex + 1) );
		else
			m_pEngineVideo->updateOSDMetaData(USB_VIDEO, mediaInfo.fileName.mid(lastSlashIndex + 1) );

        m_pModel->setFolderName(m_pModel->getFolderNameStr(mediaInfo.fileName)); //modified by aettie.ji 2013.01.31 for ISV 70943
        m_pModel->setFilename( mediaInfo.fileName.mid(lastSlashIndex + 1) );
        m_pModel->setPlayingFilename( mediaInfo.fileName.mid(lastSlashIndex + 1) );
        m_pModel->setSubtitleStr("");

        int index = 0;
        int count = 0; // modified by Sergey 25.05.2013
        m_pIMedia->GetCurrentFileFolderIndex(&index, &count);

        // { added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
        m_pEngineVideo->GetVideoController()->sendMediaInfoClusterChanged(mediaInfo.fileName.mid(lastSlashIndex + 1));
        m_pEngineVideo->GetVideoController()->sendTrackClusterChanged(index + 1, count);
        // } added by cychoi 2013.08.17

        // { modified by kihyung 2013.09.11 for ITS 0189474
        m_pModel->setDisplayPlaylistLength(count);
        m_pModel->setDisplayPlaylistPos(index);
        m_pModel->setPlaylistPosition(trackNo);
        m_pModel->setPosition(m_pIMedia->GetCurrentMediaPosition());

        ASPECT_RATIO_T currAspectRatio;
        currAspectRatio = m_pEngineVideo->GetVideoController()->getAspectRatio();

        QString divxTemp = ".divx";
        QString * divx = NULL;
        divx = &divxTemp;
        bool isDivxFile = (m_pModel->playingUrl().endsWith(divx,Qt::CaseInsensitive));

        if(isDivxFile)
        {
            if(currAspectRatio == ASPECT_RATIO_16_9)
            {

                    m_pEngineVideo->GetVideoController()->updateAspectRatioTable(ASPECT_RATIO_16_9, m_pEngineVideo->GetVideoController()->getRectByPAR(m_pEngineVideo->GetVideoController()->getPAR(),
                                                                                                                                                   m_pEngineVideo->GetVideoController()->getDAR(), m_pEngineVideo->GetVideoController()->getActualRect().width(),
                                                                                                                                                   m_pEngineVideo->GetVideoController()->getActualRect().height()));
            }
            else
            {
                m_pEngineVideo->GetVideoController()->updateAspectRatioTable(ASPECT_RATIO_4_3,QRect( 160, 0, 960, 720 ));
            }

            DISPLAY_T widgetDisp = m_pEngineVideo->getCurrentScreen();
            if(m_pEngineVideo->getEngineMain().getCamInFG())
                widgetDisp = m_pEngineVideo->getEngineMain().IsSwapDCEnabled() ? DISPLAY_FRONT : DISPLAY_REAR;

            MP_HIGH << "widgetDisp =" << widgetDisp << LOG_ENDL;

            m_pEngineVideo->GetVideoController()->moveWidget(widgetDisp);
        }


        // } modified by kihyung 2013.09.11 for ITS 0189474
    }
    
   
    // { deleted by lssanh 2013.06.26 bt call 
    /*
    if(m_pEngineVideo->getEngineMain().getisBTCall() == true) 
    {
	    pause();
        // m_pModel->setPosition(0); // removed by kihyung 2013.06.26
    }
    */
	// } deleted by lssanh 2013.06.26 bt call 
    //added by edo.lee 2013.06.11

    setTitleEnabled();

    if(mediaInfo.fileduration != 0) {
        AppMediaPlayer_SettingsStorage::GetInstance()->SaveVideoSettings(m_pEngineVideo->GetVideoController()); // modified by kihyung 2013.07.20 for ITS 0181075
    }
    // } modified by kihyung

    if(!m_bIsAutoTrackChanged)
        m_pEngineVideo->getEngineMain().sendVRCancelSignal(); // added by youngsim.jo 2014.03.13 for ITS 224783,224780 VR cancel

}

void AppMediaPlayer_Video_ControllerFS::enableSubTitle( bool bEnabledSubtitle )
{
    MP_LOG << " bEnabledSubtitle = " << bEnabledSubtitle << LOG_ENDL;

    if (m_pIMedia)
    {
        m_pEngineVideo->GetVideoController()->updateEnableSubtitleInModels(bEnabledSubtitle); // added by Sergey 17.10.2013 for ITS#195182
        m_pIMedia->EnableSubTitle( bEnabledSubtitle );

        //Just kludge for clean up subtitle string on playnback screen
        if ( !bEnabledSubtitle )
            m_pModel->setSubtitleStr("");
    }

    AppMediaPlayer_SettingsStorage::GetInstance()->SaveVideoSettings(m_pEngineVideo->GetVideoController()); // modified by kihyung 2013.07.20 for ITS 0179774
}

void AppMediaPlayer_Video_ControllerFS::setFontSize( AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_T nFontSize )
{
    MP_LOG << " nFontSize = " << nFontSize << LOG_ENDL;

    m_pEngineVideo->GetVideoController()->updateCaptionSizeInModels(nFontSize); // added by Sergey 17.10.2013 for ITS#195182

    if( m_pIMedia )
    {
    	// removed by Sergey 28.09.2013 for ITS#192050
    
        switch (nFontSize)
        {
        case AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_SMALL:
        {
            m_pIMedia->SetFontSize( eMP_SUBTITLEFONT_SMALL );
            break;
        }

        case AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_NORMAL:
        {
            m_pIMedia->SetFontSize( eMP_SUBTITLEFONT_NORMAL );
            break;
        }

        case AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_LARGE:
        {
            m_pIMedia->SetFontSize( eMP_SUBTITLEFONT_LARGE );
            break;
        }

        default:
        {
            //ignore;
            MP_LOG << " unknow size of subtitles font " << LOG_ENDL;
            break;
        }
        }

        AppMediaPlayer_SettingsStorage::GetInstance()->SaveVideoSettings(m_pEngineVideo->GetVideoController()); // modified by kihyung 2013.07.20 for ITS 0179774
    }
}

QString AppMediaPlayer_Video_ControllerFS::getRootPath()
{
    return m_mediaRoot;
}

void AppMediaPlayer_Video_ControllerFS::onProgress( int index, int total, int copyPercentage )
{
    MP_LOG << "index: " << index << " total: " << total << " copyPercentage: " << copyPercentage << LOG_ENDL;
    m_pModel->setCopyPercentage( copyPercentage );
}

// { added by wspark 2012.08.21 for DQA #47
void AppMediaPlayer_Video_ControllerFS::replaceFileRequest(QString file,int totalFilesForCopy) // modified by ravikanth 07-07-13 for ITS 0178184
{
    QString fileName = file.mid(file.lastIndexOf("/") + 1);

    setCopyFileName(fileName);
    m_pEngineVideo->getScreenManager(VIDEO_DISPLAY_FRONT)->closeProgressPopup();//popScreen(AppMediaPlayer_Video_Screens::SCREEN_POP_PROGRESS);
	m_pEngineVideo->getScreenManager(VIDEO_DISPLAY_REAR)->closeProgressPopup();
    // { modified by wspark 2012.11.20 for CR15380
    //emit showPopup();
    emit showPopup(POPUP_TXT_BTN_COPY_EXISTED_FILE);
    // } modified by wspark
}
// } added by wspark 2012.08.21 for DQA #47

// { modified by Sergey for CR#16015
void AppMediaPlayer_Video_ControllerFS::onDRMInfo(int status, int useLimit, int useCount)
{
    MP_LOG << "status =" << status << "useLimit =" << useLimit << "useCount =" << useCount << LOG_ENDL;

    m_pModel->setLoadingMode(false); // added by Sergey 30.07.2013
    m_pModel->backupForUnsupported(); // added by Sergey 01.11.2013 for ITS#197782

    // added by kihyung 2013.10.25
    if(m_pEngineVideo->playRequester() == PLAY_REQUSTER_TUNE) //added by edo.lee 2013.10.01 ITS 0177248
        m_pEngineVideo->setPlayRequester(PLAY_REQUSTER_MAX);

    // { added by Sergey 29.11.2013 for ITS#211869
    QString curFilePath    = m_pIMedia->GetCurrentFileName();
    int     lastSlashIndex = curFilePath.lastIndexOf('/');
    if(lastSlashIndex < 0) 
        lastSlashIndex = 0;
    
    QString fullFolderName = curFilePath.left(lastSlashIndex);
    if(fullFolderName != m_pModel->fullfoldername()) 
        m_pModel->setFullFolderName(fullFolderName);
    

    m_pModel->setFolderName(m_pModel->getFolderNameStr(curFilePath)); // Update folder
    m_pModel->setFilename(curFilePath.mid(lastSlashIndex + 1) ); // Update file name
    MP_HIGH << "folder =" << m_pModel->foldername() << "file =" << m_pModel->filename() << LOG_ENDL;
     // } added by Sergey 29.11.2013 for ITS#211869

    switch(status)
    {
    case eMP_DRM_RENTAL_STATUS:
    {
        MP_LOG << "file manager file =" << m_pModel->fileFromFM() << LOG_ENDL;

        // { added by Sergey 15.09.2013 for ITS#185233
        if(!m_pModel->fileFromFM().isEmpty())
        {
            confirmDRMPlay();
            return;
        }
        // } added by Sergey 15.09.2013 for ITS#185233
		
        m_pModel->setDrmUseCount(useCount);
        m_pModel->setDrmUseLimit(useLimit);
        emit fsPlaybackError(FSERROR_TYPE_DRM_STATUS);
        break;
    }
    case eMP_DRM_RENTAL_EXPIRED:
    {
        m_pModel->setDrmUseLimit(useLimit);
        emit fsPlaybackError(FSERROR_TYPE_DRM_EXPIRED);
        break;
    }
    case eMP_DRM_NOT_AUTHORIZED:
    {
        emit fsPlaybackError(FSERROR_TYPE_DRM_NOT_AUTH);
        break;
    }
    default:
    {
        MP_LOG << "Unsupported value." << LOG_ENDL;
        break;
    }
    }
}

void AppMediaPlayer_Video_ControllerFS::onError(int error)
{
    MP_LOG << "error =" << error << LOG_ENDL;

    switch(error)
    {
    case eMP_ERR_UNSUPPORTED_FILE:
    case eMP_ERR_FORMAT_ERROR:
    case eMP_ERR_STREAM: //added by jeeeun 2013-01-16 for media stream issue
    case eMP_ERR_OTHERDATAERROR:  // added by kihyung 2013.08.31 for ITS 0187672
    case eMP_ERR_DIVX_AUDIO_NOT_SUPPORTED:
    case eMP_ERR_DIVX_VIDEO_NOT_SUPPORTED: // added by Sergey 10.11.2013 new DivX popups
    {
        //}moved for ITS 229131
        if(error == eMP_ERR_UNSUPPORTED_FILE){
            if(m_pModel->playbackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PAUSED)
            {
                m_pSkipTimer->stop();
                MP_HIGH << "PLAYBACK_STATUS_PAUSED... return" << LOG_ENDL;
                return;
            }
        }
        //}moved for ITS 229131
        if(!(m_pEngineVideo->isVideoInFG())){
            skip();
            break;
        }
        m_pModel->backupForUnsupported(); // added by kihyung 2013.06.18
        if(m_pEngineVideo->playRequester() == PLAY_REQUSTER_TUNE)//added by edo.lee 2013.10.01 ITS 0177248
            m_pEngineVideo->setPlayRequester(PLAY_REQUSTER_MAX);
        
		// { modified by kihyung 2013.07.22 for ITS 0179477
        QString curFilePath    = m_pIMedia->GetCurrentFileName();
        int     lastSlashIndex = curFilePath.lastIndexOf('/');
        if(lastSlashIndex < 0) {
            lastSlashIndex = 0;
        }
        
        QString fullFolderName = curFilePath.left(lastSlashIndex);
        if(fullFolderName != m_pModel->fullfoldername()) {
            m_pModel->setFullFolderName(fullFolderName);            
        }
        m_pModel->setFolderName(m_pModel->getFolderNameStr(curFilePath)); // Update folder
        m_pModel->setFilename(curFilePath.mid(lastSlashIndex + 1) ); // Update file name
		// } modified by kihyung 2013.07.22 for ITS 0179477

		// { modified by Sergey 25.05.2013
        int index = 0;
        int count = 0;
        m_pIMedia->GetCurrentFileFolderIndex(&index, &count);
        // { modified by kihyung 2013.09.11 for ITS 0189474
        m_pModel->setDisplayPlaylistLength(count);
        m_pModel->setDisplayPlaylistPos(index);
        m_pModel->setPlaylistPosition(m_pIMedia->GetCurrentIndex());
        // } modified by kihyung 2013.09.11 for ITS 0189474
		// } modified by Sergey 25.05.2013

		// { modified  by Sergey 10.11.2013 new DivX popups
		FSERROR_TYPE_T fsError = FSERROR_TYPE_MAX;
		switch(error)
		{
			case eMP_ERR_UNSUPPORTED_FILE: fsError = FSERROR_TYPE_UNSUPPORTED; break;
			case eMP_ERR_DIVX_AUDIO_NOT_SUPPORTED: fsError = FSERROR_TYPE_AUDIO_FORMAT_UNSUPPORTED; break;
			case eMP_ERR_DIVX_VIDEO_NOT_SUPPORTED: fsError = FSERROR_TYPE_RESOLUTION_UNSUPPORTED; break;
			default: fsError = FSERROR_TYPE_UNSUPPORTED; break;
		}

		if(fsError != FSERROR_TYPE_MAX)
			emit fsPlaybackError(fsError);
		// } modified by Sergey 10.11.2013 new DivX popups
			
        break;
    }
    // { added by Sergey 17.10.2013 for ITS#194953
    case eMP_ERR_UNSUPPORTED_ALL_FILES_IN_FOLDER:
    {
        repeatPlayAll();
        skip();
        repeatPlayFolder();

        break;
    }
    // } added by Sergey 17.10.2013 for ITS#194953
    case eMP_ERR_UNSUPPORTED_ALL_FILE:
    {
        m_pSkipTimer->stop(); // added by kihyung 2013.12.10 for ITS 0214009 
        
		// removed by Sergey 09.09.2013 to maintain Loading popup
        detachDrmSignals();

        // { modified by kihyung 2013.10.30 for ITS 0204910 
        if(m_pEngineVideo->isVideoInFG())
        {
            emit fsPlaybackError(FSERROR_TYPE_UNSUPPORTED_ALL);
        }
        else
        {
            m_pEngineVideo->getEngineMain().GetMediaManager()->SetSourceAvailable(m_mediaSource, MEDIA_TYPE_VIDEO, false);
    		m_pEngineVideo->getEngineMain().GetNotifier()->UpdateSDPMediaCount(m_mediaSource, MEDIA_TYPE_VIDEO, 0);
    		m_pEngineVideo->getEngineMain().GetNotifier()->setEnableMedia(m_mediaSource, MEDIA_TYPE_MAX);
        }
        // } modified by kihyung 2013.10.30 for ITS 0204910         
        
        break;
    }
    case eMP_ERR_SUBTITLE_FILE_NOTFOUND:
    {
        m_isSubTitle = false;
        m_isSubTitleString = m_pIMedia->GetCurrentFileName();
		m_pEngineVideo->closeCaptionScreen(); //added by edo.lee 2014.1.8 ITS 0218077 
        break;
    }
    case eMP_ERR_RESOURCE:
    {
		m_pEngineVideo->CancelAllJogKey(); //added by edo.lee 2014.01.08 EDO_ITS_218617
                skip();// added for G-Streamer Error Handling 2014.05.27
        // { removed by kihyung 2013.2.17
        /*
        if ( ( m_mediaSource == MEDIA_SOURCE_USB1 ) || ( m_mediaSource == MEDIA_SOURCE_USB2 ) )
            m_pFSExplorer->stopMonitoring();
        */
        // } removed by kihyung 2013.2.17        
        break;
    }
    case eMP_ERR_AUTOAUDIOSINK_FAIL:
    {
        //{removed for G-Streamer Error Handling 2014.05.27
        //m_pSkipTimer->stop(); // added by kihyung 2013.12.10 for ITS 0214009
        
        //if(m_pIMedia)//added for ITS 227056 2014.02.27
        //{
            //m_pIMedia->Stop();//added for ITS 227056 2014.02.27
        //}
        //}removed for G-Streamer Error Handling 2014.05.27

        // { modified by kihyung 2013.07.04 for ISV 85455
        GSTAudioRouterDBus *pDBus = m_pEngineVideo->GetVideoController()->GetEngineMain().GetNotifier()->getGSTAudioRouterDBus();
        if(pDBus) {
            MP_HIGH << "GSTAudioRouterDBus.ReleaseAudioSource" << LOG_ENDL;
            pDBus->ReleaseAudioSource(EAllSource);
        }
        
        /*
	    usleep(400000);
	    if(m_pIMedia){
			m_pEngineVideo->getEngineMain().setUnMuteForSeek(false); //added by edo.lee 2013.05.19
			m_pIMedia->Play();
	    }
	    */
            //m_pAudioSinkTimer->start();
        skip();// added for G-Streamer Error Handling 2014.05.27
        // } modified by kihyung 2013.07.04 for ISV 85455
    	break;
    }
    default: // make compiler happy
        break;
    }
}
// } modified by Sergey for CR#16015.

void AppMediaPlayer_Video_ControllerFS::detachDrmSignals()
{
    if ( m_mediaSource != MEDIA_SOURCE_JUKEBOX )
    {
        if ( GetUSBController() == NULL )
        {
            MP_LOG << "Inconsistent state." << LOG_ENDL;
            return;
        }
        disconnect( GetUSBController(), SIGNAL(OnDRMStatusUpdated(int,int,int)),
                   this, SLOT(onDRMInfo(int,int,int)));
    }
    else
    {
        if ( m_pJukeboxController == NULL )
        {
            MP_LOG << "Inconsistent state." << LOG_ENDL;
            return;
        }
        disconnect( m_pJukeboxController, SIGNAL(OnDRMStatusUpdated(int,int,int)),
                   this, SLOT(onDRMInfo(int,int,int)));
    }
}

void AppMediaPlayer_Video_ControllerFS::onSubtitleChanged( QString strCaption )
{
    MP_LOG << "subtitle string:" << strCaption << LOG_ENDL;
    m_pModel->setSubtitleStr( strCaption );
}


void AppMediaPlayer_Video_ControllerFS::setAudioPath()
{
    MP_LOG << "m_mediaSource:" << m_mediaSource << LOG_ENDL;

    switch(m_mediaSource)
    {
        case MEDIA_SOURCE_USB1:
        {
            m_pEngineVideo->NofityAudioPathChanged( MODE_STATE_USB1_VIDEO); // added by lssanh 2012.08.24 for audio path
            break;
        }
        case MEDIA_SOURCE_USB2:
        {
            m_pEngineVideo->NofityAudioPathChanged( MODE_STATE_USB2_VIDEO); // added by lssanh 2012.08.24 for audio path
            break;
        }
        case MEDIA_SOURCE_JUKEBOX:
        {
            m_pEngineVideo->NofityAudioPathChanged( MODE_STATE_JUKEBOX_VIDEO); // added by lssanh 2012.08.24 for audio path
            break;
        }
        default:
        {
            MP_LOG << "Default case not handled" << LOG_ENDL;
            break;
        }
    }
}

void AppMediaPlayer_Video_ControllerFS::fastForward()
{
    if( m_pIMedia )
    {
        getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_4X );// added by taihyun.ahn 2013.10.31
    	m_pEngineVideo->getEngineMain().setMuteForSeek(false); //modified by edo.lee 2013.04.22 for Mute
        m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false);//added by edo.lee 2013.09.02 ITS 185417
// modified by edo.lee 2013.10.12 pop noise
        m_bFF = true;
		startLongBtnTimer();
		getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_4X );
        //m_pIMedia->FastForward(FAST_SPEED_X4);
    }
}

void AppMediaPlayer_Video_ControllerFS::fastForwardCritical()
{
    if( m_pIMedia )
    {
        m_pIMedia->FastForward(FAST_SPEED_X20);
    }
}


// { added by Sergey 15.10.2013 for ITS#195593
void AppMediaPlayer_Video_ControllerFS::cancel_ff_rew()
{    
    MP_LOG << LOG_ENDL;

    stopFFRewTimer(); // added by Sergey13.10.13

    getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING );// added by taihyun.ahn 2013.10.31
 
    if(getModel()->playbackStatus() != AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_20X
            && getModel()->playbackStatus() != AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_4X)
    {
        m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false);
        m_pEngineVideo->getEngineMain().setUnMuteState(false);//taihyun.ahn 2013.11.29 for ITS 212015
    }

    if ( m_pIMedia )
    {
        m_pIMedia->Play();
    }
}
// } added by Sergey 15.10.2013 for ITS#195593


void AppMediaPlayer_Video_ControllerFS::rewind()
{
    if( m_pIMedia )
    {
        getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_4X );// added by taihyun.ahn 2013.10.31
    	m_pEngineVideo->getEngineMain().setMuteForSeek(false); //modified by edo.lee 2013.04.22 for Mute
        m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false);//added by edo.lee 2013.09.02 ITS 185417
// modified by Dmitry 13.10.13
        m_bFF = false;
        startLongBtnTimer();
		getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_4X );
        //m_pIMedia->Rewind(FAST_SPEED_X4);
    }
}

void AppMediaPlayer_Video_ControllerFS::rewindCritical()
{
    if( m_pIMedia )
    {
        m_pIMedia->Rewind(FAST_SPEED_X20);
    }
}

void AppMediaPlayer_Video_ControllerFS::next()
{
	m_pModel->setSubtitleStr(" "); // added by lssanh 2013.03.21 ITS159696
    if(m_pIMedia)
    {
        // { modified by kihyung 2013.09.12 for ISV 90634
        if(m_pEngineVideo->IsStopped())//m_pModel->loadingMode() == true)
        {
            if(m_currentSkip == false && !m_pModel->isUnsupported()) // modified by Sergey 07.10.2013 for ITS#193743
            {
                MP_LOG << "m_pModel->loadingMode()... return..." << LOG_ENDL;
                return; 
            }
        }
        else {
//            m_pModel->setLoadingMode(true);
//            m_pEngineVideo->unblockUI(); // commented by Sergey 08.10.2013 for ISV#92307
        }
        // } modified by kihyung 2013.09.12 for ISV 90634
        
        m_pAudioSinkTimer->stop(); // added by kihyung 2013.07.04 for ISV 85455
        
        // { added by wspark 2013.04.17
        AppMediaPlayer_Video_Controller *pController = m_pEngineVideo->GetVideoController();
        if(pController)
        {
            //pController->sendClearTpMessage(); // commented by cychoi 2013.08.17 for No clear TP message from HMC request
        }
        // } added by wspark

        
        //added by edo.lee 2013.06.23
        if(m_currentSkip)//modified by taihyun.ahn for reverse ITS 208734
        {
            //m_currentSkip = false;
        }
        else//modified by taihyun.ahn 2013.12.11 for ITS 214649
        {
            m_pEngineVideo->getEngineMain().setMuteForSeek(false); //modified by edo.lee 2013.04.22 for Mute
            m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(true);
        }

	        
        m_isForwardDirection = true;

        // { added by Sergey 31.10.2013 for ITS#205257
        m_pModel->setLastPlayedUrl("");
        m_pModel->setLastPlayedPos(0); 
		// } added by Sergey 31.10.2013 for ITS#205257

        m_pSkipTimer->stop(); // added by Sergey 08.10.2013 for ISV#92307

        //{added for TEXTPOPUP at prev or next
        if(m_pEngineVideo->isUnsupportedPopup(m_pEngineVideo->getCurrentScreen()))
        {
            m_pEngineVideo->popScreenFromBoth(AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON);
        }
        //}added for TEXTPOPUP at prev or next
        // { modified by kihyung 2013.06.07 for ITS 0172699
        if(m_pEngineVideo->isVideoListView() == VIDEO_DISPLAY_MAX)
        {
            if(m_currentSkip)//removed by taihyun.ahn for reverse ITS 208734
            {
                m_pIMedia->Next(1,1);
                m_currentSkip = false;
            }
            else
                m_pIMedia->Next(1);
        }
        else
        {
            if(m_currentSkip)//removed by taihyun.ahn for reverse ITS 208734
            {
                m_pIMedia->Prev(1, 1, 1);
                m_currentSkip = false;
            }
            else
                m_pIMedia->Prev(1, 1);
        }
        // } modified by kihyung 2013.06.07

        // { commented by Sergey 10.10.2013 for ITS#194442 as API is ready
//        m_pModel->setLoadingMode(true);
//        m_pEngineVideo->unblockUI(); // added by Sergey 08.10.2013 for ISV#92307
			// } commented by Sergey 10.10.2013 for ITS#194442
    }
}

// moved by kihyung 2013.09.11

void AppMediaPlayer_Video_ControllerFS::prev(bool bForce) // modified by Sergey 28.05.2013
{
	MP_LOG << LOG_ENDL;
	m_pModel->setSubtitleStr(" "); // added by lssanh 2013.03.21 ITS159696
    if(m_pIMedia)
    {
        // { modified by kihyung 2013.09.12 for ISV 90634
        if(m_pEngineVideo->IsStopped())//m_pModel->loadingMode() == true)
        {
            if(m_currentSkip == false && !m_pModel->isUnsupported()) // modified by Sergey 07.10.2013 for ITS#193743
            {
                MP_LOG << "m_pModel->loadingMode()... return..." << LOG_ENDL;
                return; 
            }
        }
        else {
//            m_pModel->setLoadingMode(true);
//            m_pEngineVideo->unblockUI(); // commented by Sergey 08.10.2013 for ISV#92307
        }
        // } modified by kihyung 2013.09.12 for ISV 90634
        
        m_pAudioSinkTimer->stop(); // added by kihyung 2013.07.04 for ISV 85455
        
        // { added by wspark 2013.04.17
        AppMediaPlayer_Video_Controller *pController = m_pEngineVideo->GetVideoController();
		
		if(pController && (pController->PlayPosition() < 3000))
        {
            //pController->sendClearTpMessage(); // commented by cychoi 2013.08.17 for No clear TP message from HMC request
        } 	
        else
        {
    		ShowCurrentModelToOSD();//m_pEngineVideo->showCurrentOSD();
        }
        
        // } added by wspark
        
        //added by edo.lee 2013.06.23
        if(m_currentSkip)
        {
            //m_currentSkip = false;
        }
        else{
	        m_pEngineVideo->getEngineMain().setMuteForSeek(false);//modified by edo.lee 2013.04.22 for Mute
	        m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(true);
        }
	        
        m_isForwardDirection = false;
        
        // { added by Sergey 31.10.2013 for ITS#205257
        m_pModel->setLastPlayedUrl("");
        m_pModel->setLastPlayedPos(0); 
		// } added by Sergey 31.10.2013 for ITS#205257

		m_pSkipTimer->stop(); // added by Sergey 08.10.2013 for ISV#92307

        //{added for TEXTPOPUP at prev or next
        if(m_pEngineVideo->isUnsupportedPopup(m_pEngineVideo->getCurrentScreen()))
        {
            m_pEngineVideo->popScreenFromBoth(AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON);
        }
        //}added for TEXTPOPUP at prev or next
        // { modified by kihyung 2013.06.07 for ITS 0172699
        if(m_pEngineVideo->isVideoListView() == VIDEO_DISPLAY_MAX)
            if(m_currentSkip)//removed by taihyun.ahn for reverse ITS 208734
            {
                m_pIMedia->Prev(1, 1, bForce);
                m_currentSkip = false;
            }
            else
                m_pIMedia->Prev(1, bForce);
        else
            if(m_currentSkip)//removed by taihyun.ahn for reverse ITS 208734
            {
                m_pIMedia->Next(1,1);
                m_currentSkip = false;
            }
            else
                m_pIMedia->Next(1);
        // } modified by kihyung 2013.06.07  

        // { commented by Sergey 10.10.2013 for ITS#194442 as API is ready
//        m_pModel->setLoadingMode(true);
//        m_pEngineVideo->unblockUI(); // added by Sergey 08.10.2013 for ISV#92307
		// } commented by Sergey 10.10.2013 for ITS#194442
    }
}

void AppMediaPlayer_Video_ControllerFS::flickNext()
{
    if( m_pIMedia &&  m_playlist.list->count() > 1 )
    {
    	// m_pModel->setPosition(0);//added by edo.lee 2013.06.21 // removed by kihyung 2013.07.11 for P1 384
        next();
    }
}

void AppMediaPlayer_Video_ControllerFS::flickPrev()
{
    if( m_pIMedia &&  m_playlist.list->count() > 1 )
    {
    	// m_pModel->setPosition(0);//added by edo.lee 2013.06.21 // removed by kihyung 2013.07.11 for P1 384
        prev(1); // modified by Sergey 28.05.2013
    }
}

// { modified by Sergey for CR#15771
bool AppMediaPlayer_Video_ControllerFS::tuneWheel(bool bTuneForward)
{
    // { added by kihyung 2013.07.22 for ITS 0181137
    if(m_pModel->loadingMode() == true && m_pEngineVideo->IsStopped() == true) { // modified by kihyung 2013.10.25 for ITS 0195855 
        MP_LOG << "m_pModel->loadingMode()... return..." << LOG_ENDL;
        return false; 
    }
    // } added by kihyung 2013.07.22 for ITS 0181137

    // { modified by kihyung 2013.09.11 for ITS 0189474
    //[KOR][ITS][181679][minor](aettie.ji)
    if(m_pIMedia == NULL || m_playlist.list->empty() || m_pModel->displayPlaylistLength() <= 1)
    {
        MP_LOG << "m_pIMedia is NULL or m_playlist is empty" << LOG_ENDL;
        MP_LOG << "or m_pModel->displayPlaylistLength() is 1, no list for tune searching" << LOG_ENDL;

		// { modified by ywpark 2013.09.16 for ITS 0189925
		if ( m_pModel->displayPlaylistLength() == 1)	m_pEngineVideo->showCurrentOSD();
		// } modified by ywpark 2013.09.16 for ITS 0189925

        return false;
    }

    MP_LOG << "tune forward =" << bTuneForward << LOG_ENDL;
    
    emit cancelFfRew(); // added by yongkyun 2013.01.19 for ITS 150703

    // removed by Sergey 23.09.2013 for ITS#188498

    if ( m_tempTuneIndex == -1 )
        m_tempTuneIndex = m_pModel->displayPlaylistPos();
	
    if ( bTuneForward )
    {
        // If end of list switch to first.
        if ( m_tempTuneIndex >= m_pModel->displayPlaylistLength() - 1 )
            m_tempTuneIndex = 0;
        else
            m_tempTuneIndex++;
    }
    else
    {
        // If first file in list switch to last.
        if ( m_tempTuneIndex == 0 )
            m_tempTuneIndex = m_pModel->displayPlaylistLength() - 1;
        else
            m_tempTuneIndex--;
    }

    // { modiifed by kihyung 2013.10.08 for ITS 0194414
    /*
    if( m_tempTuneIndex >= 0 && m_tempTuneIndex < m_curDirContent.size()) 
    {	    
	    m_pModel->setDisplayPlaylistPos(m_tempTuneIndex);

	    if(!m_curDirContent.isEmpty())
	    {
	        m_tuneFile = m_curDirContent.at(m_tempTuneIndex); // crash point
	        m_searchTitle = m_tuneFile.mid(m_tuneFile.lastIndexOf('/') + 1);

            if(m_tuneFile == m_pModel->playingUrl()) //modified by edo.lee 2013.07.06
	        {
                m_pEngineVideo->SetTuneSameFile(true);//added by edo.lee 2013.03.22
                m_pEngineVideo->showCurrentOSD();
	        }
	        else {
	            m_pEngineVideo->SetTuneSameFile(false);
				
				if(m_mediaSource == MEDIA_SOURCE_JUKEBOX)
					m_pEngineVideo->setOSDMetaData(
				    JUKEBOX_VIDEO,m_searchTitle, m_pModel->randomMode(), m_pModel->repeatMode(), 0, true);
				else
					m_pEngineVideo->setOSDMetaData(
		   			USB_VIDEO,m_searchTitle, m_pModel->randomMode(), m_pModel->repeatMode(), 0, true);
	        }

            if(!m_tuneFile.isEmpty()) {
                m_pModel->setFilename(m_searchTitle, false); // modified by kihyung 2013.06.18
            }
	    }
    }	
	*/
	if( m_tempTuneIndex >= 0 && m_tempTuneIndex < m_playlist.list->size()) 
    {	    
	    m_pModel->setDisplayPlaylistPos(m_tempTuneIndex);

        m_tuneFile    = m_playlist.list->at(m_tempTuneIndex);
        m_searchTitle = m_tuneFile.mid(m_tuneFile.lastIndexOf('/') + 1);

        if(m_tuneFile == m_pModel->playingUrl())
        {
            m_pEngineVideo->SetTuneSameFile(true);
            m_pEngineVideo->showCurrentOSD();
        }
        else 
        {
            m_pEngineVideo->SetTuneSameFile(false);
			
			if(m_mediaSource == MEDIA_SOURCE_JUKEBOX)
				m_pEngineVideo->setOSDMetaData(JUKEBOX_VIDEO,m_searchTitle, m_pModel->randomMode(), m_pModel->repeatMode(), 0, true);
			else
				m_pEngineVideo->setOSDMetaData(USB_VIDEO,m_searchTitle, m_pModel->randomMode(), m_pModel->repeatMode(), 0, true);
        }
        
        m_pModel->setFilename(m_searchTitle, false);
        m_pModel->setFolderName(m_pModel->getFolderNameStr(m_tuneFile));
    }
	// } modified by kihyung 2013.09.11 for ITS 0189474
	
	return true;	

}

bool AppMediaPlayer_Video_ControllerFS::selectTuned()
{
    MP_LOG << "temp index =" << m_tempTuneIndex << LOG_ENDL;

    if(m_pIMedia == NULL || m_playlist.list->isEmpty() || (m_tempTuneIndex == -1))
    {
        MP_LOG << "m_pIMedia is NULL or m_playlist is null or empty or incorrect index" << LOG_ENDL;
        return false;
    }

    // { modified by kihyung 2012.08.03 for CR 12313
    // modified by kihyung 2013.09.11 for ITS 0189474
    if(m_pModel->displayPlaylistLength() <= m_tempTuneIndex) // in the scope of playlist?
    {
        m_tempTuneIndex = -1;
        return false;
    }

    // added by kihyung 2013.10.25 for ITS 0195855
    if(m_pEngineVideo->isUnsupportedPopup(DISPLAY_FRONT)) 
    {
        m_pEngineVideo->popScreenFromBoth(AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON); 
    }
    
    // { added by wspark 2013.04.17
    AppMediaPlayer_Video_Controller *pController = m_pEngineVideo->GetVideoController();
    if(pController)
    {
        //pController->sendClearTpMessage(); // commented by cychoi 2013.08.17 for No clear TP message from HMC request
    }
    // } added by wspark
    
    m_pEngineVideo->getEngineMain().setMuteForSeek(false); //added by edo.lee 2013.11.23 select tune showing control cue
    m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(true);//added by edo.lee 2013.11.23

	if(m_mediaSource == MEDIA_SOURCE_JUKEBOX)
		m_pEngineVideo->setOSDMetaData(
	    JUKEBOX_VIDEO,m_searchTitle, m_pModel->randomMode(), m_pModel->repeatMode(), 0, false);
	else
		m_pEngineVideo->setOSDMetaData(
			USB_VIDEO,m_searchTitle, m_pModel->randomMode(), m_pModel->repeatMode(), 0, false);

	m_searchTitle = "";
    m_pEngineVideo->setPlayRequester(PLAY_REQUSTER_TUNE);
    saveLastPlayedInfo();
    m_pIMedia->Stop();
    m_pIMedia->SetCurrentIndex(m_playlist.list->indexOf(m_tuneFile));
	m_pModel->setPlayingUrl(m_tuneFile); //added by edo.lee 2013.07.06 check same file
	m_pModel->setPosition(0); //added by kihyung 2013.09.10 for ITS 0188383 
	m_pEngineVideo->getEngineMain().setUnMuteForSeek(false); //added by edo.lee 2013.05.19
    m_pIMedia->Play();
    m_pEngineVideo->getEngineMain().setSelectTuned(true); //Added by taihyun.ahn 2013.10.02  for ITS 188100
    m_tempTuneIndex = -1;
    return true;
}

void AppMediaPlayer_Video_ControllerFS::cancelTune()
{
    if(m_tempTuneIndex < 0) return; // added by kihyung 2013.1.30 for ISV 70211
    
    MP_LOG << LOG_ENDL;
    
    m_tempTuneIndex = -1;

    // { modified by kihyung 2013.10.08 for ITS 0194414
    // m_pModel->setDisplayPlaylistPos(m_curDirContent.indexOf(m_pModel->playingUrl()));
    // m_pModel->setFilename(m_pModel->playingUrl().mid(m_pModel->playingUrl().lastIndexOf('/') + 1));
    if(m_playlist.list) 
    {
        m_pModel->setDisplayPlaylistPos(m_playlist.list->indexOf(m_pModel->playingUrl()));
        m_pModel->setFilename(m_pModel->playingUrl().mid(m_pModel->playingUrl().lastIndexOf('/') + 1));
        m_pModel->setFolderName(m_pModel->getFolderNameStr(m_pModel->playingUrl()));
    }
    // } modified by kihyung 2013.10.08 for ITS 0194414
}

// { modified by Sergey for DRM 30.07.2013
void AppMediaPlayer_Video_ControllerFS::restoreLastPlay()
{
    if(m_pModel->lastPlayedUrl() != "")
    {
        QString url = m_pModel->lastPlayedUrl();
        int pos = m_pModel->lastPlayedPos();

        m_pModel->setLastPlayedUrl("");
        m_pModel->setLastPlayedPos(0);

        playFile(url, false, pos);
    }
    else
        skip();
} 
// } modified by Sergey for DRM 30.07.2013

void AppMediaPlayer_Video_ControllerFS::saveLastPlayedInfo()
{
    m_pModel->setLastPlayedUrl(m_pModel->playingUrl());
    m_pModel->setLastPlayedPos(m_pModel->position());
}
// } modified by Sergey for CR#15771


// removed by Sergey for DRM 30.07.2013. Confirm DRM is implemented in other place.

void AppMediaPlayer_Video_ControllerFS::saveFileInfoForRestore(const QString & newFile)
{
    QString restFile = m_pModel->fullfoldername() +"/"+ m_pModel->filename();

    // removed by Sergey 11.08.2013
	
    m_pModel->setRestoreFile( restFile );
    m_pModel->setRestorePosition( m_pModel->position() );
    m_pModel->setFileFromFM( newFile );
}

void AppMediaPlayer_Video_ControllerFS::clearRestoreData()
{
    MP_LOG << LOG_ENDL;
    m_pModel->setFileFromFM("");
    m_pModel->setRestoreFile("");
    m_pModel->setRestorePosition(-1);
}

void AppMediaPlayer_Video_ControllerFS::checkRestoreData(int value)
{
    MP_LOG<<" value = "<<value << LOG_ENDL;
    if (m_pModel->fileFromFM()== "" && m_pModel->restoreFile()!= "" && value >= 1)
    {
        clearRestoreData();
    }
}

void AppMediaPlayer_Video_ControllerFS::onFgBgStateChanged(bool value)
{
    MP_LOG<<" value = "<<value << LOG_ENDL;
    if (!value)
    {
        clearRestoreData();
    }
}


// { modified by Sergey 07.09.2013 for ITS#185543
void AppMediaPlayer_Video_ControllerFS::setAudioLanguage(int item)
{
    if(m_mediaSource== MEDIA_SOURCE_JUKEBOX )
    {
        m_pJukeboxController->SetAudioLanguage(item);
    }
    else if(m_mediaSource == MEDIA_SOURCE_USB1 || m_mediaSource == MEDIA_SOURCE_USB2)
    {
        GetUSBController()->SetAudioLanguage(item);
    }
}
// } modified by Sergey 07.09.2013 for ITS#185543


// { added by Sergey 09.10.2013 for ITS#187939
void AppMediaPlayer_Video_ControllerFS::setSubtitleLanguage(int item)
{
    if(m_mediaSource== MEDIA_SOURCE_JUKEBOX )
    {
        m_pJukeboxController->setSubtitleLanguage(item);
    }
    else if(m_mediaSource == MEDIA_SOURCE_USB1 || m_mediaSource == MEDIA_SOURCE_USB2)
    {
        GetUSBController()->setSubtitleLanguage(item);
    }
}
// } added by Sergey 09.10.2013 for ITS#187939


// { added by junggil 2012.08.22 for NEW UX 5.1.8.1 Caption
bool AppMediaPlayer_Video_ControllerFS::subTitle()
{
    return m_isSubTitle;
}
// } added by junggil


// modified by kihyung 2013.07.10 for ITS 0173538 
bool AppMediaPlayer_Video_ControllerFS::showSubTitle(bool bShow)
{
    MP_LOG << bShow << LOG_ENDL;
    if(m_pModel->captionEnable() == true) {
        m_pModel->showCaption(bShow);
        return true;
    }
    else {
        return false;
    }
}

// moved by kihyung 2013.09.11
void AppMediaPlayer_Video_ControllerFS::ShowCurrentModelToOSD()
{
	MP_LOG << LOG_ENDL;
	if(m_mediaSource == MEDIA_SOURCE_JUKEBOX)
		m_pEngineVideo->setOSDMetaData(JUKEBOX_VIDEO, m_pModel->filename(),
		m_pModel->randomMode(),m_pModel->repeatMode(),0, false);
	else
		m_pEngineVideo->setOSDMetaData(USB_VIDEO, m_pModel->filename(),
		m_pModel->randomMode(),m_pModel->repeatMode(),0, false);
}

// { added by wspark 2013.01.25 for ISV 66482
void AppMediaPlayer_Video_ControllerFS::setTitleEnabled()
{
    bool result = false;

    switch(m_mediaSource)
    {
        case MEDIA_SOURCE_JUKEBOX:
        {
            result = m_pJukeboxController->getIsSubtitleEnable();
            break;
        }
        case MEDIA_SOURCE_USB1:
        case MEDIA_SOURCE_USB2:
        {
            result = GetUSBController()->getIsSubtitleEnable();
            break;
        }
        default:
        {
            MP_LOG << "Source not initialized. Return FALSE." << LOG_ENDL;
            return; // modified by kihyung 2013.08.22 for ITS 0185761 
        }
    }
    
    m_isSubTitle = result;

    // added by kihyung 2013.08.22 for ITS 0185761 
    if(m_pModel) {
        emit m_pModel->setCurrentSubtitleChanged((int)m_mediaSource);
    }
}
// } added by wspark

void AppMediaPlayer_Video_ControllerFS::requestAllPlaylist(int nReqID)
{
    MP_LOG << LOG_ENDL;
    
    AppMediaPlayer_Video_Controller *pController = m_pEngineVideo->GetVideoController();
    if(pController) {
        CTrackerAbstractor *pTrackerAbs = pController->GetTrackerAbstractor();
        if(pTrackerAbs) {
            MP_LOG << "eGetVideoLimitFileListDir_Id" << LOG_ENDL;
            pTrackerAbs->RequestData(AudioUtils::VolumeType(AudioUtils::DeviceType(m_mediaSource)), eGetVideoLimitFileListDir_Id, nReqID + m_mediaSource);
        }
    }
    else {
        MP_LOG << "pController is NULL" << LOG_ENDL;
    }
}

void AppMediaPlayer_Video_ControllerFS::requestCurrFolderList(QString strFolderName)
{
    AppMediaPlayer_Video_Controller *pController = m_pEngineVideo->GetVideoController();
    if(pController) {
        CTrackerAbstractor *pTrackerAbs = pController->GetTrackerAbstractor();
        if(pTrackerAbs) {
            QString strModifiedPath;
            if(strFolderName.startsWith("file://"))
                strModifiedPath = strFolderName.mid(7);
            else
                strModifiedPath = strFolderName;
            MP_LOG << "eGetVideoFolderTabList_Id" << strModifiedPath << LOG_ENDL;
            pTrackerAbs->RequestData(strModifiedPath, eGetVideoFolderTabList_Id, REQUID_VIDEO_ID + m_mediaSource);
        }   
    }
    else {
        MP_LOG << "pController is NULL" << LOG_ENDL;
    }
}

// modified by kihyung 2013.08.16 for ITS 0184724
void AppMediaPlayer_Video_ControllerFS::UpdateAllPlaylistAfterScan(QStringList &fileList, bool bRescan)
{
    bool bPlayRestart = false; // added by kihyung 2013.08.24 for ITS 0173453 
    
    MP_LOG << fileList.count() << bRescan << LOG_ENDL;

    m_playlist.list->clear();
    if(!fileList.isEmpty())
    {
        m_playlist.list->append(fileList);
    }

    if(m_pModel) 
    {
        // { added by kihyung 2013.11.06 for ITS 0206863 
        if(m_playlist.list->size() <= 0) 
        {
            m_pModel->setRepeatMode(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_OFF);
            m_pModel->setRandomMode(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_OFF);
            return;
        }
        // } added by kihyung 2013.11.06 for ITS 0206863
        
        QString filepath = m_pModel->fullfoldername() + "/" + m_pModel->filename();
        QFile   file(filepath);

        // { modified by kihyung 2013.07.09 for ITS 0179048 (debug)
        if( ( (m_pModel->playlistPosition() < m_playlist.list->count()) || bRescan ) && file.exists() == true)  // modified by sungha.choi 2013.09.24 for ITS 0190303
        {
            MP_LOG << "last memory ok " << filepath << LOG_ENDL;
            
            m_playlist.index    = m_playlist.list->indexOf(filepath);
            m_playlist.position = m_pModel->position();

            if(m_playlist.index < 0) {
                MP_LOG << "error m_playlist.index " << m_playlist.index << LOG_ENDL;
                MP_LOG << "error filepath " << filepath << LOG_ENDL;
                m_playlist.index = 0;    
                m_playlist.position = 0;

                // { added by Sergey 24.10.2014 for ITS#198379
                m_pModel->setFilename(fileNameFromPath(m_playlist.list->at(m_playlist.index)));
                m_pModel->setFolderName(folderNameFromPath(m_playlist.list->at(m_playlist.index)));
                m_pModel->setPosition(0);
                m_pModel->setDuration(0);
                // } added by Sergey 24.10.2014 for ITS#198379

                bPlayRestart = true; // added by kihyung 2013.08.24 for ITS 0173453 
            }
        }
        else
        {
            MP_HIGH << "last memory error " << filepath << LOG_ENDL;
            MP_HIGH << m_playlist.list->count() << "," << m_pModel->playlistPosition() << LOG_ENDL;
            MP_HIGH << file.exists() << LOG_ENDL;
            
            m_playlist.index    = 0;
            m_playlist.position = 0;

            // { added by Sergey 24.10.2014 for ITS#198379
            m_pModel->setFilename(fileNameFromPath(m_playlist.list->at(m_playlist.index)));
            m_pModel->setFolderName(folderNameFromPath(m_playlist.list->at(m_playlist.index)));
            m_pModel->setPosition(0);
            m_pModel->setDuration(0);
			// } added by Sergey 24.10.2014 for ITS#198379
            bPlayRestart = true; // added by kihyung 2013.08.24 for ITS 0173453            
        }
        // } modified by kihyung 2013.07.09 for ITS 0179048 
        
        if(m_playlist.index < m_playlist.list->size() && m_playlist.list->size() > 0) 
        {
            QString strFileName = m_playlist.list->at(m_playlist.index);
            int lastSlashIndex = strFileName.lastIndexOf('/');
            QString fullFolderName = strFileName.left(lastSlashIndex);
            
            m_pModel->setFullFolderName(fullFolderName);            
            // requestCurrFolderList(fullFolderName); // removed by kihyung 2013.10.08 for ITS 0194414  
        }
        // { removed by kihyung 2013.10.08 for ITS 0194414 
        /*
        else {
            m_curDirContent.clear();
        }
        */
        // } removed by kihyung 2013.10.08 for ITS 0194414 

        // { added by kihyung 2013.08.16 for ITS 0184724
        if(bRescan == true) {
            m_playlist.position = 0;
        }
        // } added by kihyung 2013.08.16 for ITS 0184724

        // { modified by kihyung 2013.07.10 for ITS 0179130 
        int nCount = 0;
        int nIndex = 1;
        if(m_pIMedia) {
            // { added by kihyung 2013.08.24 for ITS 0173453 
            if(bPlayRestart)
            {
                stop();
            }
            // } added by kihyung 2013.08.24 for ITS 0173453 
            updateFSFolderList();  // added by kihyung 2013.08.08 for ITS 0181967 
            setPlaylist(m_playlist);
            m_pIMedia->GetCurrentFileFolderIndex(&nIndex, &nCount);            
            MP_LOG << "CASE 1" << nIndex << nCount << LOG_ENDL;
        }
        // { modified by kihyung 2013.10.08 for ITS 0194414 
        /*
        // { added by kihyung 2013.07.18 for ITS 0180443 
        else if(m_curDirContent.size() > 0) {
            nCount = m_curDirContent.size();
            nIndex = m_curDirContent.indexOf(filepath);
            if(nIndex < 0) nIndex = 0;
            MP_LOG << "CASE 2" << nIndex << nCount << LOG_ENDL;
        }
        // } added by kihyung 2013.07.18 for ITS 0180443
        */
        else if(m_playlist.list->size() > 0) {
            nCount = m_playlist.list->size();
            nIndex = m_playlist.list->indexOf(filepath);
            if(nIndex < 0) nIndex = 0;
            MP_LOG << "CASE 2" << nIndex << nCount << LOG_ENDL;
        }
        // } modified by kihyung 2013.10.08 for ITS 0194414 

        // { modified by kihyung 2013.09.11 for ITS 0189474
        m_pModel->setDisplayPlaylistLength(nCount);
        m_pModel->setDisplayPlaylistPos(nIndex);
        m_pModel->setPlaylistPosition(m_playlist.index);
        // } modified by kihyung 2013.09.11 for ITS 0189474
        // } modified by kihyung

        // { added by kihyung 2013.08.24 for ITS 0173453 
        if(bPlayRestart) 
        {
        	// { modified by Sergey 08.01.2014 for ITS#211412
            m_isSourceActivation = true; 
            play();
            setAudioLanguage(m_pModel->getCurrentStreamId()); // added by Sergey 07.09.2013 for ITS#185543
            enableSubTitle(m_pModel->captionEnable());
            setSubtitleLanguage(m_pModel->captionCurId());
            m_isSourceActivation = false;
            // } modified by Sergey 08.01.2014 for ITS#211412
        }
        // } added by kihyung 2013.08.24 for ITS 0173453         
    }
    else 
    {
        m_playlist.index    = 0;
        m_playlist.position = 0;
        // requestCurrFolderList(getRootPath()); // removed by kihyung 2013.10.08 for ITS 0194414 
    }
}

void AppMediaPlayer_Video_ControllerFS::UpdateCurrFolderList(QStringList &fileList)
{
    MP_LOG << "TASCAN -> " << fileList.count() << LOG_ENDL;
    // removed by kihyung 2013.10.08 for ITS 0194414
    // m_curDirContent.clear();
    // m_curDirContent.append(fileList);
}

// { modified by Sergey for CR#16015
void AppMediaPlayer_Video_ControllerFS::skip()
{
    // { added by kihyung 2013.12.05 for 0212864
    //{moved for ITS 229131
    /*if(m_pModel->playbackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PAUSED)
    {
        m_pSkipTimer->stop();
        MP_HIGH << "PLAYBACK_STATUS_PAUSED... return" << LOG_ENDL;
        return;
    }*/
    //}moved for ITS 229131
    // } added by kihyung 2013.12.05 for 0212864         
            
    // { modified by kihyung 2013.08.21 for ITS 0185471
    /*
    m_currentSkip = true;//added by edo.lee 2013.06.23
    if(m_isForwardDirection)
        next();
    else
        prev(true); // modified by Sergey 15.08.2013 for ISV#88948
    */
    MP_LOG << LOG_ENDL;

    m_currentSkip = true;//added by edo.lee 2013.06.23
    /*if(m_isForwardDirection)// removed by taihyun.ahn 13.12.04
    {
        next();
        m_isForwardDirection = false;
    }
    else
    {*/

    //{modified by taihyun.ahn 2013.12.04
    if(m_pEngineVideo->isVideoListView() != VIDEO_DISPLAY_MAX && m_bIsAutoTrackChanged)
    {
        prev(true);
    }
    else
    {
        if(m_isForwardDirection)
        {
            next();
             // modified by Sergey 15.08.2013 for ISV#88948
        }
        else
        {
            prev(true);
        }
    }


    //}modified by taihyun.ahn 2013.12.04
    // } modified by kihyung 2013.08.21 for ITS 0185471    
}
// } modified by Sergey for CR#16015.

// removed by Sergey for CR#15771

// { modified by Sergey for CR#14878
bool AppMediaPlayer_Video_ControllerFS::isFilePlayable(QString filepath)
{
    bool result = false;

    switch(m_mediaSource)
    {
    case MEDIA_SOURCE_JUKEBOX:
    {
        result = m_pJukeboxController->IsFileSupported(filepath);
        break;
    }
    case MEDIA_SOURCE_USB1:
    case MEDIA_SOURCE_USB2:
    {
        result = GetUSBController()->IsFileSupported(filepath); // modified by eugeny.novikov 2012.12.10 for CR 16564 fastboot
        break;
    }
    default:
    {
        MP_LOG << "Source not initialized. Return FALSE." << LOG_ENDL;
        break;
    }
    }
    return result;
}
// } modified by Sergey for CR#14878.

// { added by wspark 2012.11.20 for CR15380
void AppMediaPlayer_Video_ControllerFS::onCapacityError(int filesCount, qint64 filesSize)
{
    m_filesCount = filesCount;
    m_totalFilesSize = filesSize / (1024 * 1024);

    m_pEngineVideo->getScreenManager(VIDEO_DISPLAY_FRONT)->popScreen(AppMediaPlayer_Video_Screens::SCREEN_POP_PROGRESS);
	m_pEngineVideo->getScreenManager(VIDEO_DISPLAY_REAR)->popScreen(AppMediaPlayer_Video_Screens::SCREEN_POP_PROGRESS);
	
    emit showPopup(POPUP_TXT_BTN_CAPACITY_ERROR);
}
// } added by wspark

void AppMediaPlayer_Video_ControllerFS::connectJBSignals()
{
    connect( m_pJukeboxController, SIGNAL(OnDurationChanged(int)),
            this, SLOT(onDurationChanged(int)));
    connect( m_pJukeboxController, SIGNAL(OnAutoTrackChanged(int)),
            this, SLOT(onAutoTrackChanged(int)));
    connect( m_pJukeboxController, SIGNAL(OnTrackChanged(int)),
            this, SLOT(onTrackChanged(int)));
    connect( m_pJukeboxController, SIGNAL(OnSubtitleChanged(QString)),
            this, SLOT(onSubtitleChanged(QString)));
    connect( m_pJukeboxController, SIGNAL(OnError(int)),
            this, SLOT(onError(int)));
    connect( m_pJukeboxController, SIGNAL(OnTotalAudioLangReceived(QStringList,int*,int)),
            this, SLOT(onAudioLanguagesReceived(QStringList,int*,int)));
    connect( m_pJukeboxController, SIGNAL(OnTotalTextLangReceived(QStringList,int*,int)),
            this, SLOT(onTotalTextLangReceived(QStringList,int*,int)));  // added by Sergey 20.08.2013 for ITS#184640 
    connect( m_pJukeboxController, SIGNAL(OnVideoInfo(int,int,int,int,float,float,QString)),
            this, SLOT(onVideoInfo(int,int,int,int,float,float,QString))); // modified for ITS 224766 2014.02.12

    // { added by kihyung 2013.1.25
    // connect(m_pJukeboxController, SIGNAL(OnStateChanged(int)),
    //        this, SLOT(onStateChanged(int)));
    // } added by kihyung 2013.1.25                

    connect( m_pJukeboxController, SIGNAL(OnNormalPlay()),
            this, SLOT(onNormalPlay())); // added by Sergey 28.05.2013

	// removed by Sergey for ITS#181872 30.07.2013
    connect( m_pJukeboxController, SIGNAL(HandleXSubtitleChanged(uchar*, int)), //XSubtitle added by jeeeun 2013.04.04
            this, SLOT(onHandleXSubtitleChanged(uchar*, int)));

	connect( m_pJukeboxController, SIGNAL(OnRealSoundChanged()), //added by edo.lee 2013.04.21
            this, SLOT(onRealSoundChanged()));
    connect( m_pJukeboxController,SIGNAL(OnTrackBufferred(int, int, int, int)),
                    this, SLOT(OnTrackBufferred(int, int, int, int)));//added on 2013.07.03 ITS# 0177010
    connect( m_pJukeboxController, SIGNAL(OnDRMStatusUpdated(int,int,int)),
               this, SLOT(onDRMInfo(int,int,int))); // added by Sergey for DRM 30.07.2013
}

void AppMediaPlayer_Video_ControllerFS::connectUSBSignals()
{
    // { modified by eugeny.novikov 2012.12.10 for CR 16564 fastboot
    connect( GetUSBController(), SIGNAL(OnDurationChanged(int)),
            this, SLOT(onDurationChanged(int)));
    connect( GetUSBController(), SIGNAL(OnAutoTrackChanged(int)),
            this, SLOT(onAutoTrackChanged(int)));
    connect( GetUSBController(), SIGNAL(OnTrackChanged(int)),
            this, SLOT(onTrackChanged(int)));
    connect( GetUSBController(), SIGNAL(OnSubtitleChanged(QString)),
            this, SLOT(onSubtitleChanged(QString)));
    connect( GetUSBController(), SIGNAL(OnError(int)),
            this, SLOT(onError(int)));
    connect(GetUSBController(), SIGNAL(OnTotalAudioLangReceived(QStringList,int*,int)),
            this, SLOT(onAudioLanguagesReceived(QStringList,int*,int)));
    connect(GetUSBController(), SIGNAL(OnTotalTextLangReceived(QStringList,int*,int)),
            this, SLOT(onTotalTextLangReceived(QStringList,int*,int)));  // added by Sergey 20.08.2013 for ITS#184640 
    connect(GetUSBController(), SIGNAL(OnVideoInfo(int,int,int,int,float,float,QString)),
            this, SLOT(onVideoInfo(int,int,int,int,float,float,QString))); // modified for ITS 224766 2014.02.12
    // } modified by eugeny.novikov 2012.12.10

    // { added by kihyung 2013.1.25
    // connect(GetUSBController(), SIGNAL(OnStateChanged(int)),
    //        this, SLOT(onStateChanged(int)));
    // } added by kihyung 2013.1.25    

    connect(GetUSBController(), SIGNAL(OnNormalPlay()),
            this, SLOT(onNormalPlay())); // added by Sergey 28.05.2013
	// removed by Sergey for ITS#181872 30.07.2013

    connect(GetUSBController(), SIGNAL(HandleXSubtitleChanged(uchar*, int)), //XSubtitle added by jeeeun 2013.04.04
            this, SLOT(onHandleXSubtitleChanged(uchar*, int)));

        connect( GetUSBController(), SIGNAL(OnRealSoundChanged()), //added by edo.lee 2013.04.21
            this, SLOT(onRealSoundChanged()));

    connect( GetUSBController(),SIGNAL(OnTrackBufferred(int, int, int, int)),
                    this, SLOT(OnTrackBufferred(int, int, int, int))); //added on 2013.07.03 ITS# 0177010

    connect( GetUSBController(), SIGNAL(OnDRMStatusUpdated(int,int,int)),
               this, SLOT(onDRMInfo(int,int,int))); // added by Sergey for DRM 30.07.2013

    if(m_pFileOperationHelper != NULL)
    {
        connect(m_pFileOperationHelper, SIGNAL(operationCopyComplete(int,int)), // modified by ravikanth 22-04-13
                this, SIGNAL(operationCopyComplete()));
        connect(m_pFileOperationHelper, SIGNAL(copyError(int)),
                this, SIGNAL(operationCopyError(int)));
        connect(m_pFileOperationHelper, SIGNAL(progress(int,int,int)),
                this, SLOT(onProgress(int,int,int)));
        connect(m_pFileOperationHelper, SIGNAL(copyRequest(QString,int)),
                this, SLOT(replaceFileRequest(QString,int))); // modified by ravikanth 07-07-13 for ITS 0178184
        connect(m_pFileOperationHelper, SIGNAL(capacityError(int,qint64)),
                this, SLOT(onCapacityError(int,qint64)));
        connect(m_pFileOperationHelper, SIGNAL(fileSystemChanged()), // added by kihyung 2013.2.17
                this, SLOT(onFileSystemChanged()));
    }
}

//added on 2013.07.03 ITS# 0177010
void AppMediaPlayer_Video_ControllerFS::OnTrackBufferred(int bPrev, int nTotalIdx, int nIdxInFolder, int nFolderSize)
{
    // { modified by kihyung 2013.09.11 for ITS 0189474
    QString trackame;
    if( nTotalIdx >= 0 && nTotalIdx < m_playlist.list->size())
    {
        m_pModel->setDisplayPlaylistLength(nFolderSize);
        m_pModel->setDisplayPlaylistPos(nIdxInFolder);

        if(!m_playlist.list->isEmpty())
        {
            trackame = m_playlist.list->at(nTotalIdx);

            MP_LOG << "Puneet Get FIlename = " <<trackame << LOG_ENDL;

            if(!trackame.isEmpty()) 
            {
                // { added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
                m_pEngineVideo->GetVideoController()->sendMediaInfoClusterChanged(trackame.mid(trackame.lastIndexOf('/') + 1));
                m_pEngineVideo->GetVideoController()->sendTrackClusterChanged(nIdxInFolder + 1, 0);
                // } added by cychoi 2013.08.17
                
				m_pEngineVideo->setShowOSDFromBUfferedTrack(true); //added by edo.lee 2013.07.14
               
				if(m_mediaSource == MEDIA_SOURCE_JUKEBOX) {
					m_pEngineVideo->setOSDMetaData(JUKEBOX_VIDEO, trackame.mid(trackame.lastIndexOf('/') + 1), m_pModel->randomMode(), m_pModel->repeatMode(), 0, false);
				}
				else {
					m_pEngineVideo->setOSDMetaData(USB_VIDEO, trackame.mid(trackame.lastIndexOf('/') + 1), m_pModel->randomMode(), m_pModel->repeatMode(), 0, false);
				}
                
                m_pModel->setPlayingUrl(trackame); //moved by Sergey 24.10.2013 for ITS#198060
                m_pModel->setFilename(trackame.mid(trackame.lastIndexOf('/') + 1));
				//m_pModel->setPlayingUrl(trackame); //added by edo.lee 2013.07.06 check same file //moved by Sergey 24.10.2013 for ITS#198060
                m_pModel->setFolderName(m_pModel->getFolderNameStr(trackame));
				// removed by Sergey 02.10.2013 for ITS#192855
                m_pModel->setPlaylistPosition(nTotalIdx);
                //m_pEngineVideo->setShowOSDFromBUfferedTrack(true);//remove by edo.lee 2013.07.14
            }
        }
    }
    // } modified by kihyung 2013.09.11 for ITS 0189474
}

// { added by kihyung 2013.07.04 for ISV 85455
void AppMediaPlayer_Video_ControllerFS::onAudioSinkError()
{
    MP_HIGH << LOG_ENDL;
    if(m_pIMedia){
		m_pEngineVideo->getEngineMain().setUnMuteForSeek(false); // added by edo.lee 2013.05.19
		m_pIMedia->SetCurrentMediaPosition(m_pModel->position()); // added by kihyung 2013.09.10
		m_pIMedia->Play();
    }

    m_pAudioSinkTimer->stop();
}
// } added by kihyung 2013.07.04 for ISV 85455
// modified by Dmitry 13.10.13
void AppMediaPlayer_Video_ControllerFS::onLongBtnTimerExpired()
{
   MP_LOG << LOG_ENDL;
   if(m_pIMedia)
   {
       AppMediaPlayer_Video_Controller *pController = m_pEngineVideo->GetVideoController();//added by taihyun.ahn 2013.11.06 for ITS 206843
       if(pController) {
           if(!(pController->getLoadingStatus()))
           {
               if(m_bFF)
                   m_pIMedia->FastForward(FAST_SPEED_X4);
               else
                   m_pIMedia->Rewind(FAST_SPEED_X4);
           }
           else
           {
               startLongBtnTimer();
           }
       }
   }
}

void AppMediaPlayer_Video_ControllerFS::onDeviceAttached()
{
    MP_LOG << LOG_ENDL;

    m_pModel->setRemovingMode(false);
    
    // For Jukebox media root is permanent.
    if(m_mediaSource != MEDIA_SOURCE_JUKEBOX)
    {
        m_mediaRoot = m_pEngineVideo->GetUSBPath(m_mediaSource);
		m_attachedUSB = true; // added by jeeeun 2013.01.05 for device dettached checking
    }
    else 
    {
        m_attachedJukebox = true; // added by jeeeun 2013.01.05 for device dettached checking
    }

    requestAllPlaylist(REQUID_VIDEO_ID);
}

void AppMediaPlayer_Video_ControllerFS::onDeviceRemoved()
{
    m_pSkipTimer->stop(); // added by kihyung 2013.12.10 for ITS 0214009 
    
    clearPlaylist();

    if(m_pModel) 
    {
        m_pModel->restoreForUnsupported();
        m_pModel->setRepeatMode(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_OFF);
        m_pModel->setRandomMode(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_OFF);
    }
}

void AppMediaPlayer_Video_ControllerFS::attachControllers()
{
    MP_LOG << "m_mediaSource =" << m_mediaSource << LOG_ENDL;


    if((m_mediaSource != MEDIA_SOURCE_JUKEBOX) && (GetUSBController() != NULL)) // modified by eugeny.novikov 2012.12.10 for CR 16564 fastboot
    {
        m_pIMedia = GetUSBController()->GetMediaInstance(); // modified by eugeny.novikov 2012.12.10 for CR 16564 fastboot
        m_pIMediaEx = (IUSBExMediaInterface*)GetUSBController();  // added by kihyung 2013.08.08 for ITS 0181967 
        connectUSBSignals();
        m_pIMedia->ChangeMediaMode(MP_MODE_USB);
    }
    else if((m_mediaSource == MEDIA_SOURCE_JUKEBOX) && (m_pJukeboxController != NULL))
    {
        m_pIMedia = m_pJukeboxController->GetMediaInstance();
        m_pIMediaEx = (IUSBExMediaInterface*)m_pJukeboxController;  // added by kihyung 2013.08.08 for ITS 0181967 
        connectJBSignals();
        m_pIMedia->ChangeMediaMode( MP_MODE_JUKEBOX );
    }
    connect(m_pEngineVideo, SIGNAL(fgBgStateChanged(bool)), this, SLOT(onFgBgStateChanged(bool)));    

    m_pIMedia->SetPlayerEngine( GPLAYER_ENGINE );

    if(!m_playlist.list->isEmpty())
    {
        /* If playlist is not ready activateSource() will be
         * called later, once playlist is ready.
         */
        activateSource();
    }
    // removed by Sergey 26.07.2013 for ITS#181096
}
//added by edo.lee 2013.04.21 for Mute
void AppMediaPlayer_Video_ControllerFS::onRealSoundChanged()
{
	MP_LOG << LOG_ENDL;	
	m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(false);
	m_pEngineVideo->getEngineMain().setUnMuteState(false);
}


void AppMediaPlayer_Video_ControllerFS::activateSource()
{
    // { modified by kihyung 2013.07.10 for ITS 0179130 
    if(!m_pIMedia || m_mediaSource != m_pEngineVideo->GetVideoController()->getSource())
    {
        MP_LOG << "Current source is NOT active. Return." << LOG_ENDL;
        return;
    }

    // added by honggi.shin 2013.10.25, fix static test problem
    if( m_pModel == NULL )
    {
        MP_LOG << "m_pModel is NULL. Return." << LOG_ENDL;
        return;
    }

    QString filepath = m_pModel->fullfoldername() + "/" + m_pModel->filename();
    QFile file(filepath);

    int nTrackIdx = -1;
    int nPosition = 0;

    if(file.exists() == true)
    {
        nTrackIdx = m_playlist.list->indexOf(filepath);
        nPosition = m_pModel->position();
    }
    else
    {
        MP_HIGH << "there is no file" << filepath << LOG_ENDL;
    }

    if(nTrackIdx >= 0) 
    {
        m_playlist.index    = nTrackIdx;
        m_playlist.position = nPosition;
    }
    else if(m_playlist.list->size() > 0)
    {
        m_playlist.index    = 0;
        m_playlist.position = 0;

        QString strFileName = m_playlist.list->at(m_playlist.index);
        int lastSlashIndex = strFileName.lastIndexOf('/');
        QString fullFolderName = strFileName.left(lastSlashIndex);
        
        m_pModel->setFullFolderName(fullFolderName);

        // { added by Sergey 24.10.2014 for ITS#198379
        m_pModel->setFilename(fileNameFromPath(strFileName));
        m_pModel->setFolderName(folderNameFromPath(strFileName));
        m_pModel->setPosition(0);
        m_pModel->setDuration(0);
        // } added by Sergey 24.10.2014 for ITS#198379
    }
    else 
    {
        MP_HIGH << "m_playlist.list is empty" << LOG_ENDL;  
    }
    
    // requestCurrFolderList(m_pModel->fullfoldername()); // removed by kihyung 2013.10.08 for ITS 0194414
    
	if(m_attachedJukebox == false && m_attachedUSB == false)
    {
        // added by eugene.seo 2013.01.23 for usb play issue							
        if( getModel()->playbackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING )
        	setCheckForPlayableFile(false); // added by Sergey for ScreenTransition
    }
    else if(m_attachedJukebox == true)
    {
        m_attachedJukebox = false;
    }
    else if(m_attachedUSB == true)
    {
        m_attachedUSB = false;
    }

    updateFSFolderList();  // added by kihyung 2013.08.08 for ITS 0181967 
    setPlaylist(m_playlist);

    int nRepeatMode;
    int nRandomMode;
    switch(m_pModel->repeatMode())
    {
    case AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_OFF:     nRepeatMode = REPEATALL;    break;
    case AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_FOLDER:  nRepeatMode = REPEATFOLDER; break;
    case AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_FILE:    nRepeatMode = REPEATFILE;   break;
    default:                                                    nRepeatMode = REPEATALL;    break;
    }
    switch(m_pModel->randomMode())
    {
    case AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_OFF:     nRandomMode = RANDOMOFF;    break;
    case AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_FOLDER:  nRandomMode = RANDOMFILE;   break;
    case AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_FILE:    nRandomMode = RANDOMFILE;   break;
    default:                                                    nRandomMode = RANDOMOFF;    break;
    }
    m_pIMedia->RepeatPlay(nRepeatMode);
    m_pIMedia->RandomPlay(nRandomMode);

    MP_LOG << "repeat, random: " << nRepeatMode << nRandomMode << LOG_ENDL;
    
    setAudioPath();

    m_isSourceActivation = true; // added by Sergey 08.01.2014 for ITS#211412
    // { modified by Sergey 16.10.2013 for ITS#195185
        MP_HIGH << "isMaintainPause =" << m_pEngineVideo->isMaintainPause() << "powerOffBt =" << m_pEngineVideo->getEngineMain().getPowerOffBTCall() << "isBtCall =" << m_pEngineVideo->getEngineMain().getisBTCall() << LOG_ENDL; // added by Sergey 11.10.2013 for ITS#194942

		if(m_pEngineVideo->isMaintainPause() && m_pEngineVideo->getEngineMain().getisBTCall())
		{
			m_pIMedia->SetSystemState(SYSTEM_BT_CALL_START);
			play();
			m_pIMedia->SetSystemState(SYSTEM_BT_CALL_END);
		}
		else
		{
			play();
			m_pEngineVideo->getEngineMain().resetKeepPlayState();
			m_pEngineVideo->getEngineMain().setUnMuteForSeek(false);//added by edo.lee 2013.05.19
		}

		enableSubTitle(m_pModel->captionEnable());
		setSubtitleLanguage(m_pModel->captionCurId()); // added by Sergey 08.01.2014 for ITS#211412
        setAudioLanguage(m_pModel->getCurrentStreamId()); // added by Sergey 07.09.2013 for ITS#185543
    // } modified by Sergey 16.10.2013 for ITS#195185

    //if(m_pModel) //comment this line due to duplicated null checking "m_pModel", honggi.shin 2013.10.25
    {
        int nCount = 0;
        int nIndex = 0;
        m_pIMedia->GetCurrentFileFolderIndex(&nIndex, &nCount);
        // { modified by kihyung 2013.09.11 for ITS 0189474
        m_pModel->setDisplayPlaylistLength(nCount);
        m_pModel->setDisplayPlaylistPos(nIndex);
        m_pModel->setPlaylistPosition(m_playlist.index);
        // } modified by kihyung 2013.09.11 for ITS 0189474
    }
    // } modified by kihyung 2013.07.10 for ITS 0179130
    m_isSourceActivation = false; // added by Sergey 08.01.2014 for ITS#211412
}


// { modified by Sergey 23.10.2013 for ITS#197285
/* For working with video content and playlist */
void AppMediaPlayer_Video_ControllerFS::onSourceAvailableChanged(MEDIA_SOURCE_T source,
                                                                 MEDIA_TYPE_T type,
                                                                 bool status)
{
    // Only Jukebox file system can be changed.
    if(type != MEDIA_TYPE_VIDEO || source != MEDIA_SOURCE_JUKEBOX || m_mediaSource != source)
        return;

    MP_HIGH << "Jukebox status =" << status << LOG_ENDL;

    if(status == false)
    {
        clearPlaylist();
        m_pModel->setFilename("");
        m_pModel->setFolderName("");
        m_pModel->setPosition(0);
        m_pModel->setDuration(0);
    }
}
// } modified by Sergey 23.10.2013 for ITS#197285


void AppMediaPlayer_Video_ControllerFS::updatePlaylist(QStringList &fileList)
{
    MP_LOG << LOG_ENDL;
}

void AppMediaPlayer_Video_ControllerFS::clearPlaylist()
{
    MP_LOG << LOG_ENDL;
    
    m_playlist.list->clear();
    // m_playlist.isMLT    = false; // removed by kihyung 2013.07.18 for ITS 0180268
    m_playlist.category = 1;
    m_playlist.index    = 0;
    m_playlist.mode     = 0;
    m_playlist.position = 0;
}

void AppMediaPlayer_Video_ControllerFS::setSourceAvailable(bool result)
{
    MP_LOG << "result =" << result << LOG_ENDL;

    m_pEngineVideo->getEngineMain().GetMediaManager()->SetSourceAvailable(m_mediaSource,
                                                                          MEDIA_TYPE_VIDEO,
                                                                          result);

    // { modified by kihyung 2013.1.9 for ISV 69137
    if(m_mediaSource == MEDIA_SOURCE_JUKEBOX)
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_JUKEBOX_VIDEO_COUNT, int(result));
    else if(m_mediaSource == MEDIA_SOURCE_USB1)
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_USB1_VIDEO_COUNT, int(result));        
    else if(m_mediaSource == MEDIA_SOURCE_USB2)
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_USB2_VIDEO_COUNT, int(result));        
    // } modified by kihyung 2013.1.9 for ISV 69137    
}

// { removed by kihyung 2013.2.17    
/*
void AppMediaPlayer_Video_ControllerFS::setupFSExplorer()
{
    m_pFSExplorerThread = new QThread(this);
    m_pFSExplorer = new AppMediaPlayer_FSExplorer();
    m_pFSExplorer->moveToThread(m_pFSExplorerThread);

    // Connect requests to FSExplorer
    connect(this, SIGNAL(setContentFilter(const QStringList)),
            m_pFSExplorer, SLOT(setContentFilter(const QStringList)));
    connect(this, SIGNAL(monitorDirContent(QString)),
            m_pFSExplorer, SLOT(monitorDirContent(QString)));
    connect(this, SIGNAL(activateSensor(QString)),
            m_pFSExplorer, SLOT(activateSensor(QString)));
    connect(this, SIGNAL(stopMonitoring()),
            m_pFSExplorer, SLOT(stopMonitoring()));
    connect(this, SIGNAL(requestCheckForContent(QString)),
            m_pFSExplorer, SLOT(requestCheckForContent(QString)));
    connect(this, SIGNAL(requestFirstContent(QString)),
            m_pFSExplorer, SLOT(requestFirstContent(QString)));
    connect(this, SIGNAL(requestContentInDir(QString)),
            m_pFSExplorer, SLOT(requestContentInDir(QString)));
    connect(this, SIGNAL(requestAllContent(QString)),
            m_pFSExplorer, SLOT(requestAllContent(QString)));
    connect(this, SIGNAL(requestContentDirs(QString)),
            m_pFSExplorer, SLOT(requestContentDirs(QString)));

    // Connect responses from FSExplorer
    connect(m_pFSExplorer, SIGNAL(contentChanged(QString)),
            this, SLOT(onPlayingContentChanged()));
    connect(m_pFSExplorer, SIGNAL(contentDirsReport(QStringList)),
            this, SLOT(onContentDirsReport(QStringList)));
    connect(m_pFSExplorer, SIGNAL(firstContentReport(QStringList)),
            this, SLOT(onFirstContentReport(QStringList)));
    connect(m_pFSExplorer, SIGNAL(contentInDirReport(QStringList)),
            this, SLOT(onContentInDirReport(QStringList)));
    connect(m_pFSExplorer, SIGNAL(allContentReport(QStringList)),
            this, SLOT(onAllContentReport(QStringList)));
    connect(m_pFSExplorer, SIGNAL(checkForContentReport(bool)),
            this, SLOT(onCheckForContentReport(bool)));
    connect(m_pFSExplorer, SIGNAL(sensorDetection(bool)),
            this, SLOT(onSensorDetection(bool)));

    m_pFSExplorerThread->start();
    emit setContentFilter(m_nameFilter);
}
*/
// { removed by kihyung 2013.2.17    

/*
void AppMediaPlayer_Video_ControllerFS::onSensorDetection(bool aIsContent)
{
    MP_LOG << "is empty =" << aIsContent << LOG_ENDL;

    setSourceAvailable(aIsContent);
}
*/

/*
void AppMediaPlayer_Video_ControllerFS::onCheckForContentReport(bool aResult)
{
    MP_LOG << "result =" << aResult << LOG_ENDL;

    setSourceAvailable(aResult);
}
*/

/*
void AppMediaPlayer_Video_ControllerFS::onFirstContentReport(QStringList aFiles)
{
    Q_UNUSED(aFiles);
}
*/

/*
void AppMediaPlayer_Video_ControllerFS::onContentInDirReport(QStringList aFiles)
{
    MP_LOG << LOG_ENDL;
    MP_LOG << "playlistik oncontentindir=" << aFiles << LOG_ENDL;
    m_curDirContent.clear();
    m_curDirContent.append(aFiles);
}
*/

/*
void AppMediaPlayer_Video_ControllerFS::onAllContentReport(QStringList aFiles)
{
    if(aFiles.empty())
    {
        MP_LOG << "No files found. Return." << LOG_ENDL;
        return;
    }

    MP_LOG << LOG_ENDL;
	
    updatePlaylist(aFiles);
    activateSource();
}
*/

/*
void AppMediaPlayer_Video_ControllerFS::onContentDirsReport(QStringList aDirs)
{
}
*/

void AppMediaPlayer_Video_ControllerFS::onPlaylistChanged(bool bFromFM)
{
}

/*
void AppMediaPlayer_Video_ControllerFS::onPlayingContentChanged()
{
    MP_LOG << "m_mediaSource =" << m_mediaSource << LOG_ENDL;

    requestAllContent(m_mediaRoot);
}
*/

void AppMediaPlayer_Video_ControllerFS::setPlaylist(PlayList aPlaylist)
{
    m_isForwardDirection = true; // added by kihyung 2013.09.11 for ITS 0188626 
    
    if(m_pIMedia)
    {
        m_pIMedia->SetPlayList(&aPlaylist);
    }
}

void AppMediaPlayer_Video_ControllerFS::addToPlaylist(QString aFile)
{
    if(aFile.isEmpty())
    {
        MP_LOG << "No input file. Return." << LOG_ENDL;
    }

    m_playlist.list->append(aFile);
}
// } modified by Sergey for CR#15771

// { added by kihyung 2013.08.08 for ITS 0181967 
void AppMediaPlayer_Video_ControllerFS::updateFSFolderList()
{
    if(m_pIMediaEx) 
    {
        AppMediaPlayer_Video_Controller *pController = m_pEngineVideo->GetVideoController();
        if(pController) {
            CTrackerAbstractor *pTrackerAbs = pController->GetTrackerAbstractor();
            if(pTrackerAbs) {
                QList<FSFolderInfo> tFolderInfo;
                pTrackerAbs->GetFSFolderInfo(tFolderInfo, AudioUtils::VolumeType(AudioUtils::DeviceType(m_mediaSource)), eFSVideo);
                m_pIMediaEx->SetFSFolderInfo(tFolderInfo);
            }
        }
    }
}
// } added by kihyung 2013.08.08 for ITS 0181967
 
// { added by Sergey for ScreenTransition
void AppMediaPlayer_Video_ControllerFS::setCheckForPlayableFile(bool value)
{
    switch(m_mediaSource)
    {
        case MEDIA_SOURCE_JUKEBOX:
        {
            if(value != m_pJukeboxController->getIsFileSupportedCheck())
                m_pJukeboxController->setIsFileSupportedCheck(value);
            break;
        }
        case MEDIA_SOURCE_USB1:
        case MEDIA_SOURCE_USB2:
        {
            if(value != GetUSBController()->getIsFileSupportedCheck())
                GetUSBController()->setIsFileSupportedCheck(value);
            break;
        }
        default:
        {
            MP_LOG << "Source not initialized. Return." << LOG_ENDL;
            break;
        }
    }
}
// } added by Sergey for ScreenTransition
// { added by jeeeun 2013.01.14 for 68428
bool AppMediaPlayer_Video_ControllerFS::checkIsFileSupported(const QString &filePath)
{
    MP_LOG << "checkIsFileSupported() filePath : "<< filePath << LOG_ENDL;
    switch(m_mediaSource)
    {
        case MEDIA_SOURCE_JUKEBOX:
        {
			return m_pJukeboxController->IsFileSupported(filePath);
            break;
        }
        case MEDIA_SOURCE_USB1:
        case MEDIA_SOURCE_USB2:
        {
            return GetUSBController()->IsFileSupported(filePath);
            break;
        }
        default:
        {
            MP_LOG << "Source not initialized. Return." << LOG_ENDL;
            break;
        }
    }
    return false; // added by sungha.choi 2013.07.15 for static test
}
// } added by jeeeun 2013.01.14 for 68428

// removed by Sergey for ITS#181872 30.07.2013

void AppMediaPlayer_Video_ControllerFS::onFileSystemChanged()
{
    MP_LOG << LOG_ENDL;
    m_pEngineVideo->getEngineMain().requestTrackerAbsRescan(eJukeBox);
	//Added by Puneet for not showing loading Popup on front video/Audio while copying photo in rear. 2013.06.02
    m_pEngineVideo->setRescanProgressLoading();
}

// { XSubtitle added by jeeeun 2013.04.04
void AppMediaPlayer_Video_ControllerFS::onHandleXSubtitleChanged(uchar* buf, int bufSize)
{
   MP_LOG <<"buffer addres : "<< buf <<" size: "<< bufSize << LOG_ENDL;

   if(m_pBitmapBufAddr != NULL) // modified by ravikanth 04-04-13
   {
       delete m_pBitmapBufAddr;
       m_pBitmapBufAddr = NULL;
   }
   if (bufSize > 0)
   {
       m_pBitmapBufAddr = (uchar*)malloc(bufSize);
       memset((void *)m_pBitmapBufAddr, 0, bufSize);
       memcpy((void *)m_pBitmapBufAddr, (void *)buf, bufSize);
       QImage image = QImage::fromData(m_pBitmapBufAddr, bufSize, "BMP");
       QPixmap pixmap = QPixmap::fromImage(image);
       m_pEngineVideo->videoCaptionProvider()->setPixmap(pixmap);
   	}
    m_pModel->setXSubCaptionVisibility(bufSize > 0);
}
// } XSubtitle added by jeeeun 2013.04.04

void AppMediaPlayer_Video_ControllerFS::onNormalPlay()
{
    MP_LOG << LOG_ENDL;
    m_pEngineVideo->GetVideoController()->cancel_ff_rew();//added for ITS 224800 2014.02.17
    m_pEngineVideo->setRewReachedFirst(true);
} // added by Sergey 28.05.2013

// { added by Sergey for DRM 30.07.2013
void AppMediaPlayer_Video_ControllerFS::confirmDRMPlay()
{
    switch(m_mediaSource)
    {
        case MEDIA_SOURCE_JUKEBOX:
        {
            m_pJukeboxController->DrmConfirmPlay();
            break;
        }
        case MEDIA_SOURCE_USB1:
        case MEDIA_SOURCE_USB2:
        {
            GetUSBController()->DrmConfirmPlay();
            break;
        }
        default:
        {
            MP_LOG << "Source not initialized. Return." << LOG_ENDL;
            break;
        }
    }
}
// } added by Sergey for DRM 30.07.2013


// { added by Sergey 11.08.2013
void AppMediaPlayer_Video_ControllerFS::playFromFileManager()
{
    MP_LOG << m_pModel->fileFromFM() << LOG_ENDL;
    
    if (m_pModel->fileFromFM()!= "" && m_pModel->restoreFile()!= "")
    {
        //{modified by edo.lee 2013.09.03 edo.lee ITS 0188035
        QString newFileName = m_pModel->fileFromFM();

        // { added by Sergey 26.10.2013 for ITS#198812 
        m_pModel->setPlayingUrl(newFileName);
        m_pModel->setFilename(fileNameFromPath(newFileName));
        m_pModel->setFolderName(folderNameFromPath(newFileName));
        m_pModel->setDisplayPlaylistPos(fileIndexFromPath(newFileName));
        m_pModel->setPosition(0); // added by kihyung 2013.09.10 for ITS 0213549  
		// } added by Sergey 26.10.2013 for ITS#198812 

        playFile(m_pModel->fileFromFM(), false); // modified by Sergey 18.09.2013 for ITS#190328

        if(m_pModel->repeatMode() == AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_FILE)
           repeatPlayAll(); // added by Sergey 26.10.2013 for ITS#198785

        saveFileInfoForRestore(m_pModel->fileFromFM());
        m_pModel->setRestoreFile(m_pModel->fileFromFM());
        
        // Update OSD
        OSD_DEVICE device = DEVICE_MAX;

        if(m_mediaSource == MEDIA_SOURCE_JUKEBOX)
            device = JUKEBOX_VIDEO;
        else
            device = USB_VIDEO;
        int lastSlashIndex = newFileName.lastIndexOf('/');
        // { modified by Sergey 21.09.2013 for ITS#189296
        newFileName = newFileName.mid(lastSlashIndex + 1);
        m_pModel->setFilename(newFileName);
        m_pEngineVideo->setOSDMetaData(device, newFileName, m_pModel->randomMode(), m_pModel->repeatMode(), 0, false);
        // } modified by Sergey 21.09.2013 for ITS#189296
        //}modified by edo.lee 2013.09.03 edo.lee ITS 0188035
    }
    else if (m_pModel->fileFromFM()== "" && m_pModel->restoreFile()!= "")
    {
       playFile(m_pModel->restoreFile(), false, m_pModel->restorePosition());
    }
}
// } added by Sergey 11.08.2013


// { added by Sergey 31.10.2013 for ITS#205257
void AppMediaPlayer_Video_ControllerFS::onAutoTrackChanged(int index)
{
    MP_HIGH << index << LOG_ENDL;
    
    if(index >= 0)
    {
        m_isForwardDirection = true; // added by Sergey 16.10.2013 for ITS#195740
        m_bIsAutoTrackChanged = true;
    }
    else
        m_bIsAutoTrackChanged = false;

    m_pEngineVideo->GetVideoController()->GetEngineMain().GetNotifier()->setAutoChangeFileIndex(index);
}
// } added by Sergey 31.10.2013 for ITS#205257


// { added by Sergey 13.10.13
void AppMediaPlayer_Video_ControllerFS::stopFFRewTimer()
{
    MP_HIGH << LOG_ENDL;

    if(m_pLongBtnTimer->isActive())
    {
        m_pLongBtnTimer->stop();
    }
}
// } added by Sergey 13.10.13


 // { added by Sergey 19.10.2013 for ITS#196896
QString AppMediaPlayer_Video_ControllerFS::fileNameFromPath(QString filePath)
{
    QString result = "";

    if(!filePath.isEmpty())
        result = filePath.mid(filePath.lastIndexOf('/') + 1);

    return result;
}


QString AppMediaPlayer_Video_ControllerFS::folderNameFromPath(QString filePath)
{
    QString result = "";

    if(!filePath.isEmpty())
        m_pModel->getFolderNameStr(filePath);

    return result;
}


int AppMediaPlayer_Video_ControllerFS::fileIndexFromPath(QString filePath)
{
    int result = 0;

    if(!m_playlist.list->isEmpty())
    {
        result = m_playlist.list->indexOf(filePath);
    }

    return result;
}
 // } added by Sergey 19.10.2013 for ITS#196896
