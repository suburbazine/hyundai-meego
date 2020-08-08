#include <DHAVN_AppFramework_Event_Def.h>
#include <uistate_manager_def.h>
#include "util/DHAVN_AppMediaPlayer_MediaManager.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "engine/DHAVN_AppMediaPlayer_EngineMain.h"
#include "engine/DHAVN_AppMediaPlayer_EngineAudio.h"
#include "controller/DHAVN_AppMediaPlayer_Controllers.h"
#include "util/audio/DHAVN_AppMediaPlayer_Utils.h" //added by junam 2013.04.07 ISV_KR77488
#include "util/osd/OSD_Manager.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h" // modified by yongkyun.lee 2013-09-05 for : ITS 188359


// { added by kihyung 2013.1.9 for ISV 69132
extern SharedDataProvider *m_pSharedAudioData;
extern SharedDataProvider *m_pSharedPhotoData;
extern SharedDataProvider *m_pSharedVideoData;
// } added by kihyung 2013.1.9 for ISV 69132    

// { modified by eugeny.novikov 2012.12.10 for CR 16564 fastboot
AppMediaPlayer_MediaManager::AppMediaPlayer_MediaManager(AppMediaPlayer_EngineMain &aEngineMain,
                                                         QObject *pParent):
     QObject(pParent),
     m_EngineMain(aEngineMain),
     m_eDiscState(-1), // added by kihyung 2012.09.15
     m_bSendDiscStateToUISH(false),
     m_bSendDiscTypeToUISH(false),
     m_bGstVideoMode(false) // added by kihyung 2013.2.3
{
    initializeDeviceInfo();

    m_pDeckController = aEngineMain.GetControllers().GetDeckController();

    connect(m_pDeckController, SIGNAL(OnDiscStateChanged(int)), this, SLOT(onDeckScanCompleted(int)));
    connect(m_pDeckController, SIGNAL(OnError(int)),            this, SLOT(onDeckError(int)));
    connect(m_pDeckController, SIGNAL(OnStateChanged(int)),     this, SLOT(onGstPipelineState(int)));

    connect(aEngineMain.GetControllers().GetUSBController(), SIGNAL(OnStateChanged(int)),     this, SLOT(onGstPipelineState(int)));
    connect(aEngineMain.GetControllers().GetJukeBoxController(), SIGNAL(OnStateChanged(int)),     this, SLOT(onGstPipelineState(int)));

    for (int i = 0; i < MEDIA_SOURCE_MAX; i++)
        for (int j = 0; j < MEDIA_TYPE_MAX; j++)
            m_bSourceAvailability[i][j] = false;

    m_isReloaded = false;
    m_bEjectingReceived = false; // added by cychoi 2013.06.02 for DQA issue - OSD is not displayed if eject Disc on Disc loading.

    DEVICE_INFO _deviceInfo = { "/usr_data/media/Picture", 0 };
    m_tableDevInfo[MEDIA_SOURCE_JUKEBOX] = _deviceInfo;
}
// } modified by eugeny.novikov 2012.12.10

bool AppMediaPlayer_MediaManager::IsSourceAvailable( MEDIA_SOURCE_T aSource, MEDIA_TYPE_T aType )
{
    //{ modified by yongkyun.lee 2013-10-14 for : ITS 195269
    if( aSource == MEDIA_SOURCE_BLUETOOTH )
        return true;		//added by hyochang.ryu 20131015 for ITS 195269
    else
    //} modified by yongkyun.lee 2013-10-14 
    return m_bSourceAvailability[aSource][aType];
}

void AppMediaPlayer_MediaManager::SetSourceAvailable(MEDIA_SOURCE_T source, MEDIA_TYPE_T type, bool status)
{
    MP_LOG << "TASCAN -> " << source << type << status << LOG_ENDL;
    
    if (source < MEDIA_SOURCE_MAX && type < MEDIA_TYPE_MAX)
    {
        m_bSourceAvailability[source][type] = status;
    }
    emit sourceAvailableChanged(source, type, status); // added by Sergey for #16250
}

QString AppMediaPlayer_MediaManager::GetMountPath( MEDIA_SOURCE_T aSource )
{
   return m_tableDevInfo[ aSource ].mountPath;
}

/** @copydoc AppMediaPlayer_Controllers::GetUSBUUID(MEDIA_SOURCE_T aSource) */
QString AppMediaPlayer_MediaManager::GetID(MEDIA_SOURCE_T aSource)
{
   return m_tableDevInfo[ aSource ].id;
}

void AppMediaPlayer_MediaManager::onDeckScanCompleted(int param)
{
    MP_HIGH << "[param] =" << param << LOG_ENDL;
    
    EVENT_DISC_STATUS_T discStatus;
    int                 nPrevDiscState = m_eDiscState;
    MEDIA_SOURCE_T      eMediaSource   = MEDIA_SOURCE_MAX;

    discStatus.sDiscStatus = m_isReloaded ? eDISC_STATE_RELOADED : eDISC_STATE_LOADED;
    discStatus.sDiscType = eDISC_TYPE_INVALID; // added by cychoi 2013.06.03 for setting default to INVALID by UISH request


    // { added by wonseok.heo 2013.07.04 disc in out test
    if( m_EngineMain.getInOutTestMode() == true){
        m_EngineMain.sendDiscStatusChk(param);

//        if(m_eDiscState ==8 && param == 5 ){
//            onDeckScanCompleted(6);
//            onDeckScanCompleted(7);
//        }
    }
	// } added by wonseok.heo 2013.07.04 disc in out test

    m_eDiscState = param;

    switch( param )
    {
    case PLAYER_DISCEJECTING:
    {
        // { added by wonseok.heo 2013.09.23 disc in out test
        if( m_EngineMain.getInOutTestMode() == true){
            break;
        }
        // } added by wonseok.heo 2013.09.23 disc in out test
        handleDiscEject();
        // { added by cychoi 2013.10.11 for Notify Disc eject state to VR
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_EMPTY);
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_EMPTY);
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_UNAVAILABLE);
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_UNAVAILABLE);
        // } added by cychoi 2013.10.11
        // { modified by cychoi 2013.07.27 for ITS 181737 Home AVP icon on Disc Eject
        if(m_EngineMain.IsConnectAVP2UISH() == true) {
            m_bSendDiscStateToUISH = true;
            discStatus.sDiscStatus = eDISC_STATE_EJECTING;
            sendDiscStatusToUISH(discStatus);
        }
        // } modified by cychoi 2013.07.27
        m_isReloaded = false;
        m_bEjectingReceived = true; // added by cychoi 2013.06.02 for DQA issue - OSD is not displayed if eject Disc on Disc loading.
        //m_EngineMain.GetNotifier()->displayOSD(OSD_EVENT_ADISC_EJECTED);// added by yongkyun.lee 20130503 for : DISC OSD 
        m_EngineMain.GetNotifier()->GetEjectDisplay();// added by yongkyun.lee 20130612 for : ITS 173417-70953
        break;
    }    
    case PLAYER_DISCEJECTED:
        // { modified by cychoi 2013.08.16 for ITS 184690 Eject OSD
        // ITS 181737 Home AVP icon on Disc Eject. ITS 173950 Not to notify Disc Status when disc is ejecting
        // DQA issue - OSD is not displayed if eject Disc on Disc loading. To avoid the below OSD display on auto reload after user ejection.

        // { added by wonseok.heo 2013.09.23 disc in out test
        if( m_EngineMain.getInOutTestMode() == true){
            break;
        }
        // } added by wonseok.heo 2013.09.23 disc in out test
        emit catreedeviceRemoved( ); //added by wonseok.heo for ITS 205774 2013.11.01
        handleDiscEject();
        if(m_bEjectingReceived == true || nPrevDiscState == PLAYER_DISCLOADING || nPrevDiscState == PLAYER_DISCRELOADING)
        {
            OSDManager::instance()->displayOSD(OSD_EVENT_ADISC_EJECTED);
        }
        if(m_EngineMain.IsConnectAVP2UISH() == true)
        {
            m_bSendDiscStateToUISH = true;
            discStatus.sDiscStatus = eDISC_STATE_EJECTED;
            sendDiscStatusToUISH(discStatus);
        }
        // } modified by cychoi 2013.08.16

        // removed by cychoi 2013.06.17 for code arrangement

        m_tableDevInfo[MEDIA_SOURCE_DVDVIDEO].id.clear();
        m_tableDevInfo[MEDIA_SOURCE_DVDVIDEO].mountPath.clear();
        m_tableDevInfo[MEDIA_SOURCE_VCD].id.clear();
        m_tableDevInfo[MEDIA_SOURCE_VCD].mountPath.clear();
        m_tableDevInfo[MEDIA_SOURCE_MP3].id.clear();
        m_tableDevInfo[MEDIA_SOURCE_MP3].mountPath.clear();
        m_tableDevInfo[MEDIA_SOURCE_CDDA].id.clear();
        m_tableDevInfo[MEDIA_SOURCE_CDDA].mountPath.clear();
        m_tableDevInfo[MEDIA_SOURCE_DVDAUDIO].id.clear();
        m_tableDevInfo[MEDIA_SOURCE_DVDAUDIO].mountPath.clear();
        m_isReloaded = false;
        m_bEjectingReceived = false; // added by cychoi 2013.06.02 for DQA issue - OSD is not displayed if eject Disc on Disc loading.
        break;

    case PLAYER_DVDVIDEOINSERTED:
        // { added by lssanh 2013.04.25 reload local code error disc 
        if(m_pDeckController->GetMediaInstance()->GetDeviceState() == DiscError &&
           m_EngineMain.getDeckLastError() != PLAYER_HIGHTEMPERATURE) // modified by cychoi 2014.04.25 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
        {
            m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_UNAVAILABLE);
            m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_UNAVAILABLE);

            m_bSourceAvailability[MEDIA_SOURCE_DVDVIDEO][MEDIA_TYPE_VIDEO]  = false;
            m_bSourceAvailability[MEDIA_SOURCE_VCD][MEDIA_TYPE_VIDEO]       = false;
            m_bSourceAvailability[MEDIA_SOURCE_MP3][MEDIA_TYPE_MUSIC]       = false;
            m_bSourceAvailability[MEDIA_SOURCE_CDDA][MEDIA_TYPE_MUSIC]      = false;
            m_bSourceAvailability[MEDIA_SOURCE_DVDAUDIO][MEDIA_TYPE_MUSIC]  = false;
            
            if(m_EngineMain.IsConnectAVP2UISH() == true) {
                m_bSendDiscTypeToUISH = true;
                discStatus.sDiscStatus = eDISC_STATE_RELOADED;
                discStatus.sDiscType   = eDISC_TYPE_UNKNOWN_CD;
                sendDiscStatusToUISH(discStatus);
            }
            m_isReloaded = true;
        }
        else
        {
        // } added by lssanh 2013.04.25 reload local code error disc 
            m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_DVDVIDEO);
            m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_DVDVIDEO);
            // { added by cychoi 2013.06.05 for ISV 84281
            m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
            m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
            // } added by cychoi 2013.06.05
                
            m_bSourceAvailability[MEDIA_SOURCE_DVDVIDEO][MEDIA_TYPE_VIDEO] = true;
            emit deviceAttached( MEDIA_SOURCE_DVDVIDEO, MEDIA_TYPE_VIDEO, true );
            if(m_EngineMain.IsConnectAVP2UISH() == true) {
                m_bSendDiscTypeToUISH = true;
                discStatus.sDiscType = eDISC_TYPE_VIDEO_DVD;
                sendDiscStatusToUISH(discStatus);
#ifdef BOOT_LOADING_SCREEN
                if(m_EngineMain.m_tBootInfo.bisReadyToPlay == false)
                {
                    MP_HIGH<<"ConnectAVP2UISHReadyToPlay - isReadytoPlay"<<discStatus.sDiscType << LOG_ENDL;
                    emit isReadytoPlay();
                }
#endif
            }
            eMediaSource = MEDIA_SOURCE_DVDVIDEO;
        } // added by lssanh 2013.04.25 reload local code error disc 
        
        //{ modified by yongkyun.lee 2013-09-05 for : ITS 188359
        if( m_EngineMain.GetLastAVEntryPoint(false) == eAPP_AVP_VIDEO_MODE_DISC)
        {
            AppMediaPlayer_EngineVideo *pVEngine = (AppMediaPlayer_EngineVideo*)m_EngineMain.GetEngine(APP_MEDIA_MODE_VIDEO);
            if(!pVEngine->GetVideoController()->IsPlaying())
                pVEngine->GetVideoController()->setDiscSource(MEDIA_SOURCE_DVDVIDEO);
        }        
        //} modified by yongkyun.lee 2013-09-05         
        break;

    case PLAYER_VCDINSERTED:
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_VCD);
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_VCD);
        // { added by cychoi 2013.06.05 for ISV 84281
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
        // } added by cychoi 2013.06.05
            
        m_bSourceAvailability[MEDIA_SOURCE_VCD][MEDIA_TYPE_VIDEO] = true;
        emit deviceAttached( MEDIA_SOURCE_VCD, MEDIA_TYPE_VIDEO, true );

        if(m_EngineMain.IsConnectAVP2UISH() == true) {
            m_bSendDiscTypeToUISH = true;
            discStatus.sDiscType = eDISC_TYPE_VIDEO_CD;
            sendDiscStatusToUISH(discStatus);
#ifdef BOOT_LOADING_SCREEN
            if(m_EngineMain.m_tBootInfo.bisReadyToPlay == false)
            {
                MP_HIGH<<"ConnectAVP2UISHReadyToPlay - isReadytoPlay"<<discStatus.sDiscType << LOG_ENDL;
                emit isReadytoPlay();
            }
#endif
        }
        eMediaSource = MEDIA_SOURCE_VCD;
        //{ modified by yongkyun.lee 2013-09-05 for : ITS 188359
        if( m_EngineMain.GetLastAVEntryPoint(false) == eAPP_AVP_VIDEO_MODE_DISC)
        {
            AppMediaPlayer_EngineVideo *pVEngine = (AppMediaPlayer_EngineVideo*)m_EngineMain.GetEngine(APP_MEDIA_MODE_VIDEO);
            AppMediaPlayer_Video_Controller *pController = (AppMediaPlayer_Video_Controller*)pVEngine->GetVideoController();
            if(!pController->IsPlaying())
                pVEngine->GetVideoController()->setDiscSource(MEDIA_SOURCE_VCD);
        }        
        //} modified by yongkyun.lee 2013-09-05         
        break;

    case PLAYER_CAINSERTED:
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_CA);
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_CA);
        // { added by cychoi 2013.06.05 for ISV 84281
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
        // } added by cychoi 2013.06.05
        
        m_bSourceAvailability[MEDIA_SOURCE_MP3][MEDIA_TYPE_MUSIC] = true;
        emit deviceAttached( MEDIA_SOURCE_MP3, MEDIA_TYPE_MUSIC, true );

        if(m_EngineMain.IsConnectAVP2UISH() == true) {
            m_bSendDiscTypeToUISH = true;
            discStatus.sDiscType = eDISC_TYPE_MP3_CD;
            sendDiscStatusToUISH(discStatus);
#ifdef BOOT_LOADING_SCREEN
            if(m_EngineMain.m_tBootInfo.bisReadyToPlay == false)
            {
                MP_HIGH<<"ConnectAVP2UISHReadyToPlay - isReadytoPlay"<<discStatus.sDiscType << LOG_ENDL;
                emit isReadytoPlay();
            }
#endif
        }
        eMediaSource = MEDIA_SOURCE_MP3;
        //{ modified by yongkyun.lee 2013-09-05 for : ITS 188359
        if( m_EngineMain.GetLastAVEntryPoint(false) == eAPP_AVP_AUDIO_MODE_DISC)
        {
            AppMediaPlayer_EngineAudio *pEngine = (AppMediaPlayer_EngineAudio*)m_EngineMain.GetEngine(APP_MEDIA_MODE_AUDIO);
            AppMediaPlayer_Controller_Audio *pController = (AppMediaPlayer_Controller_Audio*)pEngine->GetController();
            if( !pController->IsPlaying() )
            {
                pEngine->Restore(MODE_STATE_DISC_AUDIO);
            }
        }
        //} modified by yongkyun.lee 2013-09-05 
        break;

    case PLAYER_CDDAINSERTED:
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_CDDA);
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_CDDA);
        // { added by cychoi 2013.06.05 for ISV 84281
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
        // } added by cychoi 2013.06.05
        
        m_bSourceAvailability[MEDIA_SOURCE_CDDA][MEDIA_TYPE_MUSIC] = true;
        emit deviceAttached( MEDIA_SOURCE_CDDA, MEDIA_TYPE_MUSIC, true );

        if(m_EngineMain.IsConnectAVP2UISH() == true) {
            m_bSendDiscTypeToUISH = true;
            discStatus.sDiscType = eDISC_TYPE_AUDIO_CD;
            sendDiscStatusToUISH(discStatus);
#ifdef BOOT_LOADING_SCREEN
            if(m_EngineMain.m_tBootInfo.bisReadyToPlay == false)
            {
                MP_HIGH<<"ConnectAVP2UISHReadyToPlay - isReadytoPlay"<<discStatus.sDiscType << LOG_ENDL;
                emit isReadytoPlay();
            }
#endif
        }
        eMediaSource = MEDIA_SOURCE_CDDA;
        //{ modified by yongkyun.lee 2013-09-05 for : ITS 188359
        if( m_EngineMain.GetLastAVEntryPoint(false) == eAPP_AVP_AUDIO_MODE_DISC)
        {
            AppMediaPlayer_EngineAudio *pEngine = (AppMediaPlayer_EngineAudio*)m_EngineMain.GetEngine(APP_MEDIA_MODE_AUDIO);
            AppMediaPlayer_Controller_Audio *pController = (AppMediaPlayer_Controller_Audio*)pEngine->GetController();
            if( !pController->IsPlaying() )
            {
                pEngine->Restore(MODE_STATE_DISC_AUDIO);
            }
        }        
        //} modified by yongkyun.lee 2013-09-05 
        break;

    case PLAYER_DVDAUDIOINSERTED:
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_DVDAUDIO);
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, INSERTED_DISC_TYPE_DVDAUDIO);
        // { added by cychoi 2013.06.05 for ISV 84281
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
        // } added by cychoi 2013.06.05
        
        m_bSourceAvailability[MEDIA_SOURCE_DVDAUDIO][MEDIA_TYPE_MUSIC] = true;
        emit deviceAttached( MEDIA_SOURCE_DVDAUDIO, MEDIA_TYPE_MUSIC, true );

        if(m_EngineMain.IsConnectAVP2UISH() == false) {
            m_bSendDiscTypeToUISH = true;
            discStatus.sDiscType = eDISC_TYPE_AUDIO_DVD;
            sendDiscStatusToUISH(discStatus);
#ifdef BOOT_LOADING_SCREEN
            if(m_EngineMain.m_tBootInfo.bisReadyToPlay == false)
            {
                MP_HIGH<<"ConnectAVP2UISHReadyToPlay - isReadytoPlay"<<discStatus.sDiscType << LOG_ENDL;
                emit isReadytoPlay();
            }
#endif
        }
        eMediaSource = MEDIA_SOURCE_DVDAUDIO;
        //{ modified by yongkyun.lee 2013-09-05 for : ITS 188359
        if( m_EngineMain.GetLastAVEntryPoint(false) == eAPP_AVP_VIDEO_MODE_DISC)
        {
            AppMediaPlayer_EngineVideo *pVEngine = (AppMediaPlayer_EngineVideo*)m_EngineMain.GetEngine(APP_MEDIA_MODE_VIDEO);
            AppMediaPlayer_Video_Controller *pController = (AppMediaPlayer_Video_Controller*)pVEngine->GetVideoController();
            if(!pController->IsPlaying())
                pVEngine->GetVideoController()->setDiscSource(MEDIA_SOURCE_DVDAUDIO);
        }        
        //} modified by yongkyun.lee 2013-09-05         
        break;

    case PLAYER_DISCLOADED:
        emit discLoaded(false);
        if(nPrevDiscState < 0) {
            if(m_EngineMain.IsConnectAVP2UISH() == true) {
                m_bSendDiscStateToUISH = true;
                discStatus.sDiscStatus = eDISC_STATE_LOADED_BOOTING;
                sendDiscStatusToUISH(discStatus);
            }
        }
        // { added by cychoi 2013.06.05 for ISV 84281
        //m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
        //m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
        // } added by cychoi 2013.06.05
        // { added by cychoi 2014.03.06 for HMC Request OSD after Loaded
        if(nPrevDiscState == PLAYER_DISCLOADING)
        {
            emit discLoading();
        }
        // } added by cychoi 2014.03.06
        break;

    case PLAYER_DISCLOADING:
        if(m_EngineMain.IsConnectAVP2UISH() == true) {
            m_bSendDiscStateToUISH = true;
            discStatus.sDiscStatus = eDISC_STATE_LOADING;
            sendDiscStatusToUISH(discStatus);
        }
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCANNING);
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCANNING);
        //emit discLoading(); // commented by cychoi 2014.03.06 for HMC Request OSD after Loaded
        break;

    case PLAYER_DISCRELOADING:
        if(m_EngineMain.IsConnectAVP2UISH() == true) {
            m_bSendDiscStateToUISH = true;
            discStatus.sDiscStatus = eDISC_STATE_RELOADING;
            sendDiscStatusToUISH(discStatus);
        }
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCANNING);
        m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCANNING);
        m_isReloaded = true;
        break;		  

    case PLAYER_DISCRELOADED:

        // { added by wonseok.heo 2013.07.04 disc in out test
        if(m_EngineMain.getInOutTestMode() == true){

            if(m_EngineMain.IsConnectAVP2UISH() == true) {
                m_bSendDiscStateToUISH = true;
                discStatus.sDiscStatus = eDISC_STATE_LOADING;
                sendDiscStatusToUISH(discStatus);
            }
            m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCANNING);
            m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCANNING);
            emit discLoading();

        }else{
      //}added by wonseok.heo 2013.07.04 disc in out test
        if(m_pDeckController->GetMediaInstance()->GetDeviceState() == DiscError &&
           m_EngineMain.getDeckLastError() != PLAYER_HIGHTEMPERATURE) // modified by cychoi 2014.04.25 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
        {
            m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_UNAVAILABLE);
            m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_UNAVAILABLE);

            m_bSourceAvailability[MEDIA_SOURCE_DVDVIDEO][MEDIA_TYPE_VIDEO]  = false;
            m_bSourceAvailability[MEDIA_SOURCE_VCD][MEDIA_TYPE_VIDEO]       = false;
            m_bSourceAvailability[MEDIA_SOURCE_MP3][MEDIA_TYPE_MUSIC]       = false;
            m_bSourceAvailability[MEDIA_SOURCE_CDDA][MEDIA_TYPE_MUSIC]      = false;
            m_bSourceAvailability[MEDIA_SOURCE_DVDAUDIO][MEDIA_TYPE_MUSIC]  = false;

            if(m_EngineMain.IsConnectAVP2UISH() == true) {
                m_bSendDiscStateToUISH = true;
                discStatus.sDiscStatus = eDISC_STATE_RELOADED;
                discStatus.sDiscType   = eDISC_TYPE_UNKNOWN_CD;
                sendDiscStatusToUISH(discStatus);
            }
        }
        // { modifed by cychoi 2013.06.05 for ISV 84281
        //else
        //{
        //    m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
        //    m_pSharedVideoData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING, MEDIA_DEVICE_SCAN_END);
        //}
        // } modifed by cychoi 2013.06.05
        m_isReloaded = true;
        }  //added by wonseok.heo 2013.07.04 disc in out test
        break;

    case PLAYER_DISCREMOVED:
        m_isReloaded = false;
        m_bEjectingReceived = false; // added by cychoi 2013.06.02 for DQA issue - OSD is not displayed if eject Disc on Disc loading.
        if(nPrevDiscState < 0) {
            if(m_EngineMain.IsConnectAVP2UISH() == true) {
                m_bSendDiscStateToUISH = true;
                discStatus.sDiscStatus = eDISC_STATE_NO_DISC_BOOTING;
                sendDiscStatusToUISH(discStatus);
            }
        }
        // { modified by cychoi 2013.10.25 for ISV 93002 Should send "Ejected" if "Ejected" is not sent yet
        // { added by cychoi 2013.07.11 for ITS 178539 No Disc LED On. It's workaround. I will find good solution later.
        else if(nPrevDiscState != eDISC_STATE_EJECTED) {
            if(m_EngineMain.IsConnectAVP2UISH() == true) {
                m_bSendDiscStateToUISH = true;
                discStatus.sDiscStatus = eDISC_STATE_EJECTED;
                sendDiscStatusToUISH(discStatus);
            }
        }
        // } added by cychoi 2013.07.11
        // } modified by cychoi 2013.10.25
#ifdef BOOT_LOADING_SCREEN
        if(m_EngineMain.m_tBootInfo.bisReadyToPlay == false)
        {
            MP_HIGH<<"ConnectAVP2UISHReadyToPlay - isReadytoPlay" << LOG_ENDL;
            emit isReadytoPlay();
        }
#endif
        break;
    default:
        break;
    }

    if(eMediaSource != MEDIA_SOURCE_MAX) {
        DEVICE_INFO tDeviceInfo = {"/dev/video/dvd", 0};
        m_tableDevInfo[eMediaSource] = tDeviceInfo;
    }
}

void AppMediaPlayer_MediaManager::sendDiscStatusToUISH( EVENT_DISC_STATUS_T discStatus )
{
    MP_HIGH << "discStatus =" << discStatus.sDiscStatus << LOG_ENDL;

    Event uishEvent( EVT_DISC_STATUS, APP_AVP, APP_UI_MANAGER, QByteArray( (char*)&discStatus, sizeof( discStatus ) ) ); // modified by lssanh 2013.02.21 IPC payload
    m_EngineMain.notifyUISH( uishEvent ); // modified by Sergey for SwapDC
}

void AppMediaPlayer_MediaManager::initializeDeviceInfo()
{
}

//{removed by junam 2013.03.26 for ISV75917
//void AppMediaPlayer_MediaManager::onIPod1Authorized(bool status)
//{
//    MP_LOG << status << LOG_ENDL;

//    emit notifyIPodReady(MEDIA_SOURCE_IPOD1);

//    // { added by kihyung 2013.1.31
//    SetSourceAvailable(MEDIA_SOURCE_IPOD1, MEDIA_TYPE_MUSIC, true);
//    EmitDeviceAttached(MEDIA_SOURCE_IPOD1, MEDIA_TYPE_MUSIC, true);
//    // } added by kihyung 2013.1.31

//    AppMediaPlayer_EngineAudio *pEngine = (AppMediaPlayer_EngineAudio*)m_EngineMain.GetEngine(APP_MEDIA_MODE_AUDIO);
//    if(pEngine) {
//        AppMediaPlayer_Controller_Audio *pController = (AppMediaPlayer_Controller_Audio*)pEngine->GetController();
//        if(pController) {
//            // { added by kihyung 2013.2.20
//            CIpodController *pIPod = pController->GetControllers().GetIPod1Controller();
//            if(pIPod) {
//                QString iPodID = pIPod->getiPodSerialNumber();
//                m_pSharedAudioData->SetIPodSerialNumber(DATA_TYPE_IPOD1_SERIAL_NUMBER, iPodID);
//            }
//            // } added by kihyung 2013.2.20
//            //pController->setAppleDeviceStatus(MP::IPOD1, status);//removed by junam 2013.3.22 for remove multiple request
//        }
//    }
//}

//void AppMediaPlayer_MediaManager::onIPod2Authorized(bool status)
//{
//    MP_LOG << status << LOG_ENDL;

//    emit notifyIPodReady(MEDIA_SOURCE_IPOD2);

//    // { added by kihyung 2013.1.31
//    SetSourceAvailable(MEDIA_SOURCE_IPOD2, MEDIA_TYPE_MUSIC, true);
//    EmitDeviceAttached(MEDIA_SOURCE_IPOD2, MEDIA_TYPE_MUSIC, true);
//    // } added by kihyung 2013.1.31

//    AppMediaPlayer_EngineAudio *pEngine = (AppMediaPlayer_EngineAudio*)m_EngineMain.GetEngine(APP_MEDIA_MODE_AUDIO);
//    if(pEngine) {
//        AppMediaPlayer_Controller_Audio *pController = (AppMediaPlayer_Controller_Audio*)pEngine->GetController();
//        if(pController) {
//            // { added by kihyung 2013.2.20
//            CIpodController *pIPod = pController->GetControllers().GetIPod2Controller();
//            if(pIPod) {
//                QString iPodID = pIPod->getiPodSerialNumber();
//                m_pSharedAudioData->SetIPodSerialNumber(DATA_TYPE_IPOD2_SERIAL_NUMBER, iPodID);
//            }
//            // } added by kihyung 2013.2.20
//            //pController->setAppleDeviceStatus(MP::IPOD2, status);//removed by junam 2013.3.22 for remove multiple request
//        }
//    }
//}
//{removed by junam 2013.03.26 for ISV75917

/** @copydoc AppMediaPlayer_Controllers::onUSBDevConnected() */
// { modified by kihyung.lee 2013.07.05 for ISV 84474
bool
AppMediaPlayer_MediaManager::onUSBDevConnected( USB_PORT_T portNumber, const QString UUID, const QString mountPath)
{
    MP_HIGH << "portNumber = " << portNumber << "UUID = " << UUID << "mountPath = " << mountPath << LOG_ENDL;
    //MP_HIGH << "UUID = " << UUID << LOG_ENDL;
    //MP_HIGH << "mountPath = " << mountPath << LOG_ENDL;
    
    if(mountPath.isEmpty() == true) {
        MP_HIGH << "mountPath is null. return..." << LOG_ENDL;
        return false;
    }

    MEDIA_SOURCE_T mediaSource = MEDIA_SOURCE_MAX;

    if ( portNumber == eUSB_PORT1 )
    {
        mediaSource = MEDIA_SOURCE_USB1;
        m_EngineMain.startTrackerAbsScan(eUSB_FRONT);
        m_EngineMain.GetControllers().GetUSBController()->SetSystemState(SYSTEM_USB1_MOUNTED);
    }
    else if ( portNumber == eUSB_PORT2 )
    {
        mediaSource = MEDIA_SOURCE_USB2;
        m_EngineMain.startTrackerAbsScan(eUSB_REAR);
        m_EngineMain.GetControllers().GetUSBController()->SetSystemState(SYSTEM_USB2_MOUNTED);
    }
    else
    {
	   	MP_LOG << "Invalid USB Port" << LOG_ENDL;
		return false;
    }

    DEVICE_INFO _deviceInfo = { mountPath, UUID };

    m_tableDevInfo[mediaSource] = _deviceInfo;

    //{removed by junam 2014.01.17 deprecated code
    //((AppMediaPlayer_Controller_Audio*)m_EngineMain.GetEngine(APP_MEDIA_MODE_AUDIO)->GetController())->setFirstEntry(AudioUtils::DeviceType(mediaSource), true);
    //}removed by junam

    return true;
}
// } modified by kihyung.lee 2013.07.05 for ISV 84474

/** @copydoc AppMediaPlayer_Controllers::onUSBDevDisconnected() */
void AppMediaPlayer_MediaManager::onUSBDevDisconnected( USB_PORT_T portNumber )
{
    // { modified by kihyung 2013.07.31 for ISV 88437
    MP_HIGH << "usbPort = " << portNumber << LOG_ENDL;

    MEDIA_SOURCE_T  mediaSource  = MEDIA_SOURCE_MAX;
    USB_PORT_T      nBootUSBPort = (USB_PORT_T)AppMediaPlayer_SettingsStorage::GetInstance()->GetBootUSBPort();

    if ( portNumber == eUSB_PORT1 )
    {
        mediaSource = MEDIA_SOURCE_USB1;
        m_EngineMain.GetControllers().GetUSBController()->SetSystemState(SYSTEM_USB1_REMOVED);
    }
    else if ( portNumber == eUSB_PORT2 )
    {
        mediaSource = MEDIA_SOURCE_USB2;
        m_EngineMain.GetControllers().GetUSBController()->SetSystemState(SYSTEM_USB2_REMOVED);
    }
    else
    {
        MP_HIGH << "Invalid USB Port" << LOG_ENDL;
        return;
    }

    if(nBootUSBPort == portNumber) 
    {
        MP_HIGH << "IN BOOT, USB REMOVED..." << LOG_ENDL;
        AppMediaPlayer_SettingsStorage::GetInstance()->SetBootUSBPort((int)eUSB_PORT_INVALID);
        AppMediaPlayer_SettingsStorage::GetInstance()->SetBootUSBUUID("");
    }
    // } modified by kihyung 2013.07.31 for ISV 88437

    bool bEmitRemoved = true;
    if(m_tableDevInfo[mediaSource].mountPath.isEmpty() == true) {
        MP_HIGH << "bEmitRemoved = " << bEmitRemoved << LOG_ENDL;
        bEmitRemoved = false;
    }

    m_tableDevInfo[mediaSource].id.clear();
    m_tableDevInfo[mediaSource].mountPath.clear();

    m_EngineMain.stopTrackerAbsScan(false, portNumber); // modified by kihyung 2013.07.25 for ITS 0180896 

    for ( int i=0; i<MEDIA_TYPE_MAX; i++ )
    {
        m_bSourceAvailability[mediaSource][i] = false;
        if(bEmitRemoved == true) {
            emit deviceRemoved( mediaSource, (MEDIA_TYPE_T)i );
        }
    }
}

/** @copydoc AppMediaPlayer_Controllers::onIPODConnected() */
void
AppMediaPlayer_MediaManager::onIPODConnected( USB_PORT_T portNumber, const QString UUID, const QString mountPath, USB_DATA_CONTENT_MASK_T content )
{
    // { modified by kihyung 2013.1.26
    MP_LOG << LOG_ENDL;

    MP_LOG << "mountPath = " << mountPath << LOG_ENDL;
    MP_LOG << "mask = " << content << LOG_ENDL;
    MP_LOG << "usbPort = " << portNumber << LOG_ENDL;
    MP_LOG << "UUID = " << UUID << LOG_ENDL;

    MEDIA_SOURCE_T mediaSource = MEDIA_SOURCE_MAX;
    DEVICE_INFO _deviceInfo = { mountPath, UUID };
    
    if (portNumber == eUSB_PORT1)
    {
        mediaSource = MEDIA_SOURCE_IPOD1;
        m_tableDevInfo[ mediaSource ]= _deviceInfo;
    }
    else if (portNumber == eUSB_PORT2)
    {
        mediaSource = MEDIA_SOURCE_IPOD2;
        m_tableDevInfo[ mediaSource ]= _deviceInfo;
    }
    // { modified by eugene 2012.08.23 for static analysis defect #6905
    else
    {
	   	MP_LOG << "Invalid USB Port" << LOG_ENDL;
		return;
    }
    // } modified by eugene 2012.08.23 for static analysis defect #6905 

    //DEVICE_INFO _deviceInfo = { mountPath, UUID };
    //m_tableDevInfo[ mediaSource ]= _deviceInfo;

    //m_bSourceAvailability[mediaSource][MEDIA_TYPE_MUSIC] = true;
    //emit deviceAttached(mediaSource, MEDIA_TYPE_MUSIC, true);
    // } modified by kihyung 2013.1.26
}

/** @copydoc AppMediaPlayer_Controllers::onIPODDisconnected() */
void AppMediaPlayer_MediaManager::onIPODDisconnected(USB_PORT_T portNumber)
{
    MP_LOG << "usbPort = " << portNumber << LOG_ENDL;

    MEDIA_SOURCE_T mediaSource = MEDIA_SOURCE_MAX;

    if (portNumber == eUSB_PORT1)
    {
        mediaSource = MEDIA_SOURCE_IPOD1;
    }
    else if (portNumber == eUSB_PORT2)
    {
        mediaSource = MEDIA_SOURCE_IPOD2;
    }
	// { modified by eugene 2012.08.23 for static analysis defect #6907
    else
    {
	   	MP_LOG << "Invalid USB Port" << LOG_ENDL;
		return;
    }
    // } modified by eugene 2012.08.23 for static analysis defect #6907 

    m_tableDevInfo[mediaSource].id.clear();
    m_tableDevInfo[mediaSource].mountPath.clear();

    m_bSourceAvailability[mediaSource][MEDIA_TYPE_MUSIC] = false;
    emit deviceRemoved(mediaSource, MEDIA_TYPE_MUSIC);
}

/** @copydoc AppMediaPlayer_Controllers::onAUXConnected() */
void AppMediaPlayer_MediaManager::onAUXConnected(MEDIA_TYPE_T type)
{
    MP_LOG << LOG_ENDL;

    m_bSourceAvailability[MEDIA_SOURCE_AUX1][type] = true;
    emit deviceAttached(MEDIA_SOURCE_AUX1, type, true);
}

void AppMediaPlayer_MediaManager::onAUX2Connected(MEDIA_TYPE_T type)
{
    MP_LOG << LOG_ENDL;

    m_bSourceAvailability[MEDIA_SOURCE_AUX2][type] = true;
    emit deviceAttached( MEDIA_SOURCE_AUX2, type, true);
}

/** @copydoc AppMediaPlayer_Controllers::onAUXDisconnected() */
void AppMediaPlayer_MediaManager::onAUXDisconnected()
{
    MP_LOG << LOG_ENDL;

    //AUX is not applicable for Photo
    for ( int i = 0; i < MEDIA_TYPE_PHOTO; i++ )
    {
        m_bSourceAvailability[MEDIA_SOURCE_AUX1][i] = false;
        emit deviceRemoved( MEDIA_SOURCE_AUX1, (MEDIA_TYPE_T)i );
    }
}

void AppMediaPlayer_MediaManager::onAUX2Disconnected()
{
    MP_LOG << LOG_ENDL;

    //AUX is not applicable for Photo
    for ( int i = 0; i < MEDIA_TYPE_PHOTO; i++ )
    {
        m_bSourceAvailability[MEDIA_SOURCE_AUX2][i] = false;
        emit deviceRemoved( MEDIA_SOURCE_AUX2, (MEDIA_TYPE_T)i );
    }
}

void AppMediaPlayer_MediaManager::handleDiscEject()
{
    MP_LOG << LOG_ENDL;
    
    if ( m_bSourceAvailability[MEDIA_SOURCE_DVDVIDEO][MEDIA_TYPE_VIDEO] )
    {
        m_bSourceAvailability[MEDIA_SOURCE_DVDVIDEO][MEDIA_TYPE_VIDEO] = false;
        emit deviceRemoved( MEDIA_SOURCE_DVDVIDEO, MEDIA_TYPE_VIDEO );
    }

    if ( m_bSourceAvailability[MEDIA_SOURCE_VCD][MEDIA_TYPE_VIDEO] )
    {
        m_bSourceAvailability[MEDIA_SOURCE_VCD][MEDIA_TYPE_VIDEO] = false;
        emit deviceRemoved( MEDIA_SOURCE_VCD, MEDIA_TYPE_VIDEO );
    }

    if ( m_bSourceAvailability[MEDIA_SOURCE_MP3][MEDIA_TYPE_MUSIC] )
    {
        m_bSourceAvailability[MEDIA_SOURCE_MP3][MEDIA_TYPE_MUSIC] = false;
        emit deviceRemoved( MEDIA_SOURCE_MP3, MEDIA_TYPE_MUSIC );
    }

    if ( m_bSourceAvailability[MEDIA_SOURCE_CDDA][MEDIA_TYPE_MUSIC] )
    {
        m_bSourceAvailability[MEDIA_SOURCE_CDDA][MEDIA_TYPE_MUSIC] = false;
        emit deviceRemoved( MEDIA_SOURCE_CDDA, MEDIA_TYPE_MUSIC );
    }

    if ( m_bSourceAvailability[MEDIA_SOURCE_DVDAUDIO][MEDIA_TYPE_MUSIC] )
    {
        m_bSourceAvailability[MEDIA_SOURCE_DVDAUDIO][MEDIA_TYPE_MUSIC] = false;
        emit deviceRemoved( MEDIA_SOURCE_DVDAUDIO, MEDIA_TYPE_MUSIC );
    }
}

/** @copydoc AppMediaPlayer_Controllers::isAutoplay() */
bool AppMediaPlayer_MediaManager::isAutoplay( MEDIA_TYPE_T player, USB_DATA_CONTENT_MASK_T content )
{
    switch( player )
    {
    case MEDIA_TYPE_MUSIC:
    {
        if ( content & USB_DATA_CONTENT_AUDIO )
            return true;
        else
            return false;
    }
    case MEDIA_TYPE_VIDEO:
    {
        if ( ( content & USB_DATA_CONTENT_VIDEO ) && !( content & USB_DATA_CONTENT_MEDIA ) )
        {
            return true;
        }
        else
        {
            return false;
        }
        return true;
    }
    case MEDIA_TYPE_PHOTO:
    {
        if ( content & USB_DATA_CONTENT_PHOTO
                && !( content & USB_DATA_CONTENT_AUDIO ) && !( content & USB_DATA_CONTENT_VIDEO ) )
            return true;
        else
            return false;
    }
    default:
    {
        MP_LOG << "Unknown player" << LOG_ENDL;
        return false;
    }
    }
}

/** @copydoc AppMediaPlayer_Controllers::ejectDisc( bool isForce ) */
void AppMediaPlayer_MediaManager::ejectDisc( bool isForce )
{
    // { modified by cychoi 2013.08.07 for ITS 183155 Repeat mode init error on Disc Eject
    // TODO: Investigate whether Load & Eject is moved to IMediaInterface or not
    switch(m_eDiscState)
    {
        case PLAYER_DVDVIDEOINSERTED:
        case PLAYER_DVDAUDIOINSERTED:
        case PLAYER_VCDINSERTED:
        {
            // { modified by kihyung 2013.3.16 for 76260
            IVideoInterface *pVideo = m_pDeckController->GetVideoInstance();
            if( pVideo )
            {
                MP_LOG << "video" << LOG_ENDL;
                pVideo->Eject( (uint)isForce );
                // if(!m_EngineMain.GetNotifier()->GetAVOffMode() || !pEngine->IsAvOFF()) {
                //     m_EngineMain.GetNotifier()->displayOSD(OSD_EVENT_ADISC_EJECTED); // added by edo.lee 2013.02.27
                // }
            }
            else
            {
                IAudioInterface *pAudio = m_pDeckController->GetAudioInstance();
                if( pAudio )
                {
                    MP_LOG << "audio" << LOG_ENDL;
                    pAudio->Eject ( (uint)isForce );
                    // if(!m_EngineMain.GetNotifier()->GetAVOffMode() || !pEngine->IsAvOFF()) {
                    //     m_EngineMain.GetNotifier()->displayOSD(OSD_EVENT_ADISC_EJECTED); // added by edo.lee 2013.02.27
                    // }
                }
            }
            // } modified by kihyung 2013.3.16 for 76260    
            break;
        }
        case PLAYER_CDDAINSERTED:
        case PLAYER_CAINSERTED:
        default:
        {
            // { modified by kihyung 2013.3.16 for 76260
            IAudioInterface *pAudio = m_pDeckController->GetAudioInstance();
            
            if( pAudio )
            {
                MP_LOG << "audio" << LOG_ENDL;
                pAudio->Eject ( (uint)isForce );
                // if(!m_EngineMain.GetNotifier()->GetAVOffMode() || !pEngine->IsAvOFF()) {
                //     m_EngineMain.GetNotifier()->displayOSD(OSD_EVENT_ADISC_EJECTED); // added by edo.lee 2013.02.27
                // }
            }
            else
            {
                IVideoInterface *pVideo = m_pDeckController->GetVideoInstance();
                if( pVideo )
                {
                    MP_LOG << "video" << LOG_ENDL;
                    pVideo->Eject( (uint)isForce );
                    // if(!m_EngineMain.GetNotifier()->GetAVOffMode() || !pEngine->IsAvOFF()) {
                    //     m_EngineMain.GetNotifier()->displayOSD(OSD_EVENT_ADISC_EJECTED); // added by edo.lee 2013.02.27
                    // }
                }
            }
            // } modified by kihyung 2013.3.16 for 76260    
            break;
        }
    }
    // } modified by cychoi 2013.08.07
}

// { added by kihyung 2012.07.27 for ITS 0136589
bool AppMediaPlayer_MediaManager::IsDiscEjected()
{
    switch(m_eDiscState) 
    {
    case PLAYER_DVDVIDEOINSERTED:
    case PLAYER_DVDAUDIOINSERTED:
    case PLAYER_VCDINSERTED:
    case PLAYER_CDDAINSERTED:
    case PLAYER_CAINSERTED:
    case PLAYER_DISCLOADED:
    case PLAYER_DISCRELOADED:
    case PLAYER_DISCLOADING:
    case PLAYER_DISCRELOADING:
    case PLAYER_DISCEJECTING:
        return false;
    case PLAYER_DISCEJECTED:
    case PLAYER_DISCREMOVED:
    default:
        return true;
    }
}
// } added by kihyung

void AppMediaPlayer_MediaManager::onDeckError(int param)
{
   MP_LOG << "error code =" << param << LOG_ENDL;

   m_EngineMain.deckErrorHandler(param);
}

// { added by kihyung 2013.2.3
void AppMediaPlayer_MediaManager::onGstPipelineState(int nState)
{
    //MP_LOG << nState << LOG_ENDL;
    // { modified by kihyung 2013.2.7    
    /*
    MP_LOG << nState << m_bGstVideoMode << LOG_ENDL;

    if(m_bGstVideoMode == true) {
        if(nState == PLAYER_STOP)
        {
            m_bGstVideoMode = false;
            m_EngineMain.setGstPipelineStatus(true);
        }
        else if(nState == PLAYER_PLAY)
        {
            m_bGstVideoMode = false;
            m_EngineMain.setGstPipelineStatus(false);
        }
    }
    */
    /* remove by edo.lee 2013.03.26
    if(m_EngineMain.GetActiveMode() == APP_MEDIA_MODE_VIDEO) //added by edo.lee 2013.03.26
   	{
	    if(nState == PLAYER_STOP)
	    {
	       m_EngineMain.setGstPipelineStatus(true);
	    }
	    else if(nState == PLAYER_PLAY)
	    {
	        m_EngineMain.setGstPipelineStatus(false);
	    }
    }*/
    // } modified by kihyung 2013.2.7    
}
// } added by kihyung 2013.2.3

// removed by kihyung 2013.07.08

void AppMediaPlayer_MediaManager::StartDiscControllers()
{
    MP_HIGH << m_eDiscState << m_bSendDiscStateToUISH << m_bSendDiscTypeToUISH << LOG_ENDL;
    
    EVENT_DISC_STATUS_T discStatus;

    // { added by cychoi 2013.06.03 for setting default to INVALID by UISH request
    // { modifed by cychoi 2013.06.05 for ISV 84281
    discStatus.sDiscStatus = m_isReloaded ? eDISC_STATE_RELOADED : eDISC_STATE_LOADED;
    //discStatus.sDiscStatus = eDISC_STATE_INVALID;
    // { modifed by cychoi 2013.06.05 for ISV 84281
    discStatus.sDiscType = eDISC_TYPE_INVALID;
    // } added by cychoi 2013.06.03
    
    switch( m_eDiscState )
    {
    case PLAYER_DISCEJECTING:
        // { modified by cychoi 2013.07.27 for ITS 181737 Home AVP icon on Disc Eject
        if(m_bSendDiscStateToUISH == false) {
            discStatus.sDiscStatus = eDISC_STATE_EJECTING;
            sendDiscStatusToUISH(discStatus);
        }
        break; // added by cychoi 2013.06.17 for ITS 173950 Not to notify Disc Status when disc is ejecting
        // } modified by cychoi 2013.07.27
    case PLAYER_DISCEJECTED:
        if(m_bSendDiscStateToUISH == false) {
            discStatus.sDiscStatus = eDISC_STATE_EJECTED; // modified by cychoi 2013.07.27 for ITS 181737 Home AVP icon on Disc Eject
            sendDiscStatusToUISH(discStatus);
        }
        break;
        
    case PLAYER_DISCLOADED:
        if(m_bSendDiscStateToUISH == false) {
            discStatus.sDiscStatus = eDISC_STATE_LOADED_BOOTING;
            sendDiscStatusToUISH(discStatus);
        }
        break;

    case PLAYER_DISCLOADING:
        if(m_bSendDiscStateToUISH == false) {
            discStatus.sDiscStatus = eDISC_STATE_LOADING;
            sendDiscStatusToUISH(discStatus);
        }
        break;

    case PLAYER_DISCRELOADING:
        if(m_bSendDiscStateToUISH == false) {
            discStatus.sDiscStatus = eDISC_STATE_RELOADING;
            sendDiscStatusToUISH(discStatus);
        }
        break;		  

    case PLAYER_DISCRELOADED:
        if(m_pDeckController->GetMediaInstance()->GetDeviceState() == DiscError &&
           m_EngineMain.getDeckLastError() != PLAYER_HIGHTEMPERATURE) { // modified by cychoi 2014.04.25 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
            if(m_bSendDiscStateToUISH == false) {
                discStatus.sDiscStatus = eDISC_STATE_RELOADED;
                discStatus.sDiscType   = eDISC_TYPE_UNKNOWN_CD;
                sendDiscStatusToUISH(discStatus);
            }
        }
        break;

    case PLAYER_DISCREMOVED:
        if(m_bSendDiscStateToUISH == false) {
            discStatus.sDiscStatus = eDISC_STATE_NO_DISC_BOOTING;
            sendDiscStatusToUISH(discStatus);
        }
        break;
            
    case PLAYER_DVDVIDEOINSERTED:
        if(m_bSendDiscTypeToUISH == false) {
            discStatus.sDiscType = eDISC_TYPE_VIDEO_DVD;
            sendDiscStatusToUISH(discStatus);
        }
        break;

    case PLAYER_VCDINSERTED:
        if(m_bSendDiscTypeToUISH == false) {
            discStatus.sDiscType = eDISC_TYPE_VIDEO_CD;
            sendDiscStatusToUISH(discStatus);
        }
        break;

    case PLAYER_CAINSERTED:
        if(m_bSendDiscTypeToUISH == false) {
            discStatus.sDiscType = eDISC_TYPE_MP3_CD;
            sendDiscStatusToUISH(discStatus);
        }
        break;

    case PLAYER_CDDAINSERTED:
        if(m_bSendDiscTypeToUISH == false) {
            discStatus.sDiscType = eDISC_TYPE_AUDIO_CD;
            sendDiscStatusToUISH(discStatus);
        }
        break;

    case PLAYER_DVDAUDIOINSERTED:
        if(m_bSendDiscTypeToUISH == false) {
            discStatus.sDiscType = eDISC_TYPE_AUDIO_DVD;
            sendDiscStatusToUISH(discStatus);
        }
        break;
        
    default:
        break;
    }
}

void AppMediaPlayer_MediaManager::StartJukeboxControllers()
{
}

void AppMediaPlayer_MediaManager::StartUSBControllers()
{
}

// { added by changjin 2012.12.14 for CR 16130
int AppMediaPlayer_MediaManager::getDiscState()
{
    return m_eDiscState;
}

void AppMediaPlayer_MediaManager::loadDisc()
{
    // { modified by cychoi 2013.08.07 for ITS 183155 Repeat mode init error on Disc Eject
    // TODO: Investigate whether Load & Eject is moved to IMediaInterface or not
    switch(m_eDiscState)
    {
        case PLAYER_DVDVIDEOINSERTED:
        case PLAYER_DVDAUDIOINSERTED:
        case PLAYER_VCDINSERTED:
        {
            IAudioInterface *pAudio = m_pDeckController->GetAudioInstance();
            if( pAudio )
            {
                MP_LOG << "audio" << LOG_ENDL;
                pAudio->Load();
            }
            else
            {
                IVideoInterface *pVideo = m_pDeckController->GetVideoInstance();
                if( pVideo )
                {
                    MP_LOG << "video" << LOG_ENDL;
                    pVideo->Load();
                }
            }
            break;
        }
        case PLAYER_CDDAINSERTED:
        case PLAYER_CAINSERTED:
        default:
        {
            IVideoInterface *pVideo = m_pDeckController->GetVideoInstance();
            if( pVideo )
            {
                MP_LOG << "video" << LOG_ENDL;
                pVideo->Load();
            }
            else
            {
                IAudioInterface *pAudio = m_pDeckController->GetAudioInstance();
                if( pAudio )
                {
                    MP_LOG << "audio" << LOG_ENDL;
                    pAudio->Load();
                }
            }
            break;
        }
    }
    // } modified by cychoi 2013.08.07
}
// } added by changjin 2012.12.14

// { modified by kihyung 2013.1.26
void AppMediaPlayer_MediaManager::EmitDeviceAttached(MEDIA_SOURCE_T aMediaSource, MEDIA_TYPE_T aType, bool autoplay)
{
    MP_LOG << "TASCAN -> " << aMediaSource << aType << autoplay << LOG_ENDL;
    
    emit deviceAttached(aMediaSource, aType, autoplay);
}
// } modified by kihyung 2013.1.26

// { added by kihyung 2013.2.3
void AppMediaPlayer_MediaManager::setGstVideoMode(int mode)
{
    MP_LOG << LOG_ENDL;
    m_bGstVideoMode = mode;
}
// } added by kihyung 2013.2.3

