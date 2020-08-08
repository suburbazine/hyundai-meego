/** @file DHAVN_AppMediaPlayer_Video_Controller.cpp
 * Implementation details.
 *
 *
 * */

#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h"

#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerFS.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerDVD.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerVCD.h"

#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "engine/DHAVN_AppMediaPlayer_EngineMain.h"
#include "util/video/DHAVN_AppMediaPlayer_VideoSettings.h" // modified by ravikanth 06-03-13
#include "util/osd/OSD_Manager.h"	//added by hyochang.ryu 20131024 for ITS 192354

#include <gplayerengine.h>

extern "C" {
#include <gst_divx_api.h>
} // added by Sergey 30.07.2013


#define DEVICE_HISTORY_MAX_COUNT 5 // added by kihyung 2013.07.19 for ITS 0179774

AppMediaPlayer_Video_Controller::
AppMediaPlayer_Video_Controller( AppMediaPlayer_EngineMain &engineMain,
                                 AppMediaPlayer_EngineVideo *videoEngine )
    : AppMediaPlayer_Controller_Base( engineMain ),
      m_mediaSource(MEDIA_SOURCE_MAX),
      m_LastMediaSource(MEDIA_SOURCE_MAX),
      m_pEngineVideo( videoEngine ),
      m_pGPlayerEngine(new GPlayerEngine()),
      m_currAspectRatio(ASPECT_RATIO_16_9),
      m_smallWidgetRect(0,0,0,0), //added by Sergey for CR#16002
      m_actualRect(0,0,0,0), // added by jeeeun for PAR implementation
      m_par(1),    // added by jeeeun for PAR implementation
      m_dar(1),    // added by jeeeun for PAR implementation
      m_pTrackerAbs(0),
      m_TuneInProgress(false), // modified by Sergey 08.04.2013
      m_bClearDisplay(true), // added by Sergey 15.10.2013 for ITS#194932
      m_divxScreenMode(DIVX_STATE_REGISTRATION_CODE) // added by Sergey 25.10.2013 for new DivX
{
    m_pIMedia = NULL;
    m_pVideoWidget = NULL;
    m_isResourcesOwned = false;
    m_isCopying = false; // added by wspark 2012.08.21 for DQA #47

    m_tableOfControllers[MEDIA_SOURCE_JUKEBOX] = new AppMediaPlayer_Video_ControllerFS( &GetControllers(),
                                                                                        m_pEngineVideo,
                                                                                        "/usr_data/media/Video",
                                                                                        MEDIA_SOURCE_JUKEBOX,
                                                                                        this);

    m_tableOfControllers[MEDIA_SOURCE_USB1] = new AppMediaPlayer_Video_ControllerFS( &GetControllers(),
                                                                                     m_pEngineVideo,
                                                                                     "",
                                                                                     MEDIA_SOURCE_USB1,
                                                                                     this );

    m_tableOfControllers[MEDIA_SOURCE_USB2] = new AppMediaPlayer_Video_ControllerFS( &GetControllers(),
                                                                                     m_pEngineVideo,
                                                                                     "",
                                                                                     MEDIA_SOURCE_USB2,
                                                                                     this );

    m_tableOfControllers[MEDIA_SOURCE_VCD] = new AppMediaPlayer_Video_ControllerVCD( &GetControllers(),
                                                                                     m_pEngineVideo, // added by kihyung 2012.07.12
                                                                                     this);

    m_tableOfControllers[MEDIA_SOURCE_DVDVIDEO] = new AppMediaPlayer_Video_ControllerDVD( &GetControllers(),
                                                                                          m_pEngineVideo, //lssanh
                                                                                          this);

    m_pControllerCurrent = NULL;

    connectCModels();
    //static_cast <AppMediaPlayer_Video_ControllerFS*> (getController(MEDIA_SOURCE_JUKEBOX))->onDeviceAttached(); // added by Sergey for CR16250

    connect(m_pEngineVideo, SIGNAL(deviceAttached(MEDIA_SOURCE_T,bool)),
            this, SLOT(onDeviceAttached(MEDIA_SOURCE_T,bool)));
    connect(m_pEngineVideo, SIGNAL(deviceRemoved(MEDIA_SOURCE_T)),
            this, SLOT(onDeviceRemoved(MEDIA_SOURCE_T)));

	ResetExtendDeviceInfo(); // added by kihyung 2013.07.19 for ITS 0179774 
	
	// { modified by ravikanth 06-03-13
     m_pSettingsStorage = AppMediaPlayer_VideoSettings::GetInstance();
     m_currAspectRatio = (ASPECT_RATIO_T)m_pSettingsStorage->LoadSetting(VideoSettingsDef::DB_KEY_LAST_ASPECT_RATIO);
	 // } modified by ravikanth 06-03-13

     //Filling a known aspect ratios in the table (For front display).
     m_tableAspectRatio[ASPECT_RATIO_FULL] = QRect( 0, 0, 1280, 720 );
     m_tableAspectRatio[ASPECT_RATIO_16_9] = QRect( 0, 0, 1280, 720 );
     m_tableAspectRatio[ASPECT_RATIO_4_3] =  QRect( 160, 0, 960, 720 );

	//added by edo.lee 2013.04.21  for Mute
    m_pMuteTimer = new QTimer(this);
    m_pMuteTimer->setSingleShot( true );
    m_pMuteTimer->setInterval(300);
    connect(m_pMuteTimer, SIGNAL(timeout()), this, SLOT(onMuteTimerExpired()));

    connect(m_pGPlayerEngine, SIGNAL(StateChanged(int)), this, SLOT(HandleAuxStateChanged(int)));
    connect(&GetEngineMain(), SIGNAL(overplaneChanged()), this, SLOT(onOverplaneChanged())); // added by Sergey 15.10.2013 for ITS#194932

    // { added by Sergey 17.10.2013 for ITS#195182
    m_pJBCtrl = static_cast<AppMediaPlayer_Video_ControllerFS*>(getController(MEDIA_SOURCE_JUKEBOX)); 
    m_pUSB1Ctrl = static_cast<AppMediaPlayer_Video_ControllerFS*>(getController(MEDIA_SOURCE_USB1)); 
    m_pUSB2Ctrl = static_cast<AppMediaPlayer_Video_ControllerFS*>(getController(MEDIA_SOURCE_USB2));
    // } added by Sergey 17.10.2013 for ITS#195182
}


/** @copydoc AppMediaPlayer_Video_Controller::~AppMediaPlayer_Video_Controller() */
AppMediaPlayer_Video_Controller::~AppMediaPlayer_Video_Controller()
{
   AppMediaPlayer_VideoSettings::FreeInstance(); // modified by ravikanth 06-03-13
   delete  m_pTrackerAbs;
   delete  m_pGPlayerEngine;
}

// { added by jeeeun for PAR implementation
void AppMediaPlayer_Video_Controller::updateAspectRatioTable(ASPECT_RATIO_T aType, QRect aRect)
{
    m_tableAspectRatio[aType] = aRect;
}
// } added by jeeeun for PAR implementation

// { added by jeeeun 2013.01.14 for 68428
bool AppMediaPlayer_Video_Controller::checkCurrentFileSupported(const QString &filePath)
{
    MEDIA_SOURCE_T eCurrMedia = getSource();
    MP_LOG << "eCurrMedia "<< eCurrMedia << LOG_ENDL;
    if(eCurrMedia == MEDIA_SOURCE_JUKEBOX ||
           eCurrMedia == MEDIA_SOURCE_USB1 ||
           eCurrMedia == MEDIA_SOURCE_USB2) 
    {
            return static_cast<AppMediaPlayer_Video_ControllerFS*>(getController(eCurrMedia))->checkIsFileSupported(filePath);
    }
    return false; // added by sungha.choi 2013.07.15 for static test
}
// } added by jeeeun 2013.01.14 for 68428

//added by edo.lee 2013.06.23
void AppMediaPlayer_Video_Controller::SetSystemMute()
{
	 MP_LOG << LOG_ENDL;
    if(m_pIMedia) {
        //m_pIMedia->SetSystemState(SYSTEM_REQUEST_MUTE);
    }
}
//added by edo.lee 2013.06.23

// { added by kihyung 2013.07.19 for ITS 0179774
void AppMediaPlayer_Video_Controller::handleUSBPluggedIn(QString path, bool isAutoPlay, USB_PORT_T usbPort, QString UUID)
{
    if(usbPort != eUSB_PORT1 && usbPort != eUSB_PORT2)
	{
        MP_HIGH << "usbPort is invalid!" << LOG_ENDL;
        return;
    }	

    MP_HIGH << "Path =" << path << "Port =" << usbPort << "ID =" << UUID << "Auto = " << isAutoPlay << LOG_ENDL;

    AppMediaPlayer_Video_CModel_FS *pModel1;
    AppMediaPlayer_Video_CModel_FS *pModel2;

    pModel1 = static_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_USB1)->getModel());
    pModel2 = static_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_USB2)->getModel());

    AppMediaPlayer_Video_CModel_FS *newUSBDevice     = (usbPort == eUSB_PORT1) ? pModel1 : pModel2;
    AppMediaPlayer_Video_CModel_FS *anotherUSBDevice = (usbPort == eUSB_PORT1) ? pModel2 : pModel1;

    bool bIsUSBHistoryAvailable = (newUSBDevice->deviceID() == UUID);

    MP_HIGH << "m_history[newUSBDevice].DeviceID " << newUSBDevice->deviceID() << LOG_ENDL;
    MP_HIGH << "bIsUSBHistoryAvailable " << bIsUSBHistoryAvailable << LOG_ENDL;

    if(UUID.isEmpty() == false && newUSBDevice->deviceID() != UUID)
    {
        for(int idx = 0; idx < m_usbDevicesHistory.count(); idx++)
        {
            MP_HIGH << idx << m_usbDevicesHistory.at(idx)->deviceID() << LOG_ENDL;
            
            if(m_usbDevicesHistory.at(idx)->deviceID() == UUID)
            {
                MP_HIGH << "USB device" << UUID << "is in history at index" << idx << LOG_ENDL;
                bIsUSBHistoryAvailable = true;

                AppMediaPlayer_Video_CModel_FS tempHistory;
                if(newUSBDevice->deviceID().isEmpty() == false) {
                    CopyHistory(&tempHistory, newUSBDevice);
                }
                CopyHistoryFromUBSExtended(newUSBDevice, m_usbDevicesHistory.at(idx), path);
                
                MP_LOG << "Pushing to history stack USB" << tempHistory.deviceID() << LOG_ENDL;

                m_usbDevicesHistory.move(idx, 0);
                if(tempHistory.deviceID().isEmpty() == false) {
                    CopyHistoryToUSBExtended(m_usbDevicesHistory[0], &tempHistory);
                }

                AppMediaPlayer_SettingsStorage::GetInstance()->SaveUSBVideoExtended(this);
                
                break;
            }
        }
    }	

    if(bIsUSBHistoryAvailable == true) {
        // { modified 2 lines below by honggi.shin 2013.12.03 for avoid empty folder
        MEDIA_SOURCE_T ANOTHER_USB_SRC = anotherUSBDevice == pModel1 ? MEDIA_SOURCE_USB1 : MEDIA_SOURCE_USB2;
        if(GetEngineMain().GetMediaManager()->GetID(ANOTHER_USB_SRC).isEmpty() && newUSBDevice->deviceID() == anotherUSBDevice->deviceID())
        {
            MP_HIGH << "USB1 and USB2 has same device ID. remove anotherUSBDevice" << LOG_ENDL;
            anotherUSBDevice->initModel();
        }
    }

    if (bIsUSBHistoryAvailable == false)
    {
        MP_HIGH << "RESET CASE: newUSBDevice->initModel()" << LOG_ENDL;
        newUSBDevice->initModel();
    }
    
    newUSBDevice->setDeviceID(UUID);
    newUSBDevice->setMountPath(path);
}

void AppMediaPlayer_Video_Controller::handleUSBPluggedOut(MEDIA_SOURCE_T removedSource)
{
    MP_HIGH << LOG_ENDL;
    if (removedSource != MEDIA_SOURCE_USB1 && removedSource != MEDIA_SOURCE_USB2)
    {
        MP_HIGH << "invalid device:" << removedSource << LOG_ENDL;
        return;
    }    

    if (m_mediaSource == removedSource)
    {
        stop();
    }

    AppMediaPlayer_Video_CModel_FS *history;
    history = static_cast<AppMediaPlayer_Video_CModel_FS*>(getController(removedSource)->getModel());

    MP_HIGH << history->deviceID() << history->mountPath() << LOG_ENDL;

    if(history->deviceID().isEmpty() == true && history->mountPath().isEmpty() == true)
    {
        MP_HIGH << "return..." << LOG_ENDL;
        return;
    }
    
    if(history->deviceID().isEmpty() == false) 
    {
        bool bIsUSBHistoryAvailable = false;
        
        for(int idx = 0; idx < m_usbDevicesHistory.count(); idx++)
        {
            MP_HIGH << idx << m_usbDevicesHistory.at(idx)->deviceID() << LOG_ENDL;
            
            if(m_usbDevicesHistory.at(idx)->deviceID() == history->deviceID())
            {
                MP_HIGH << "USB device" << history->deviceID() << "is in history at index" << idx << LOG_ENDL;
                
                m_usbDevicesHistory.move(idx, 0); // move found on list top
                
                CopyHistoryToUSBExtended(m_usbDevicesHistory[0], history);
                
                AppMediaPlayer_SettingsStorage::GetInstance()->SaveUSBVideoExtended(this);

                bIsUSBHistoryAvailable = true;
                
                break;
            }
        }

        if(bIsUSBHistoryAvailable == false)
        {
            m_usbDevicesHistory.move(m_usbDevicesHistory.count() - 1, 0);
            CopyHistoryToUSBExtended(m_usbDevicesHistory[0], history);

            AppMediaPlayer_SettingsStorage::GetInstance()->SaveUSBVideoExtended(this);
        }
    }
}
// } added by kihyung 2013.07.19 for ITS 0179774

/** @copydoc AppMediaPlayer_Video_Controller::SetMediaSource() */
/* 1. If we're not the owner of the required resources, we need to acquire them
 * 2. If we're the owner of the resources, we need to switch between
 *    controllers.
 * 3.
 */
void AppMediaPlayer_Video_Controller::setSource( MEDIA_SOURCE_T source,
                                                 QString path )
{
    MP_LOG << "source =" << source << "m_isResourcesOwned =" << m_isResourcesOwned << LOG_ENDL;


    if ( source == MEDIA_SOURCE_MAX )
    {
        MP_LOG << "Invalid source passed. Ignoring." << LOG_ENDL;
        return;
    }

    if ( source == m_mediaSource )
    {
        MP_LOG << "Source is already set. Ignoring." << LOG_ENDL;
        // { added by cychoi 2013.12.17 for ITS 215865
        if(m_pEngineVideo->getIsFgReceived() == true)
        {
            // should call ClearDisplay on overplaneChanged signal when received FG
            m_bClearDisplay = false;
        }
        // } added by cychoi 2013.12.17

        return;
    }

    if(m_mediaSource != MEDIA_SOURCE_MAX)
    {
        m_pEngineVideo->setMaintainPause(false); // added by Sergey 11.10.2013 for ITS#194942
    }

    if(getController(source) && 
       (GetEngineMain().getPowerOffBTCall()==false ||
        source == MEDIA_SOURCE_DVDVIDEO ||
        source == MEDIA_SOURCE_DVDAUDIO ||
        source == MEDIA_SOURCE_VCD)) // modified by yungi 2013.10.15 for ITS 195758 //modified by edo.lee 2013.08.01
        getController(source)->getModel()->setLoadingMode(true); // added by Sergey 26.07.2013 for ITS#181096

    // { added by lssanh 2013.02.07 2USB
    GetEngineMain().GetNotifier()->cancelFileOperation();
    // } added by lssanh 2013.02.07 2USB

    // removed by Sergey for CR16250

    if ( !m_isResourcesOwned )
    {
       m_pendingSource = source;

       GetControllers().RequestResources( MEDIAPLAYER_RESOURCE_AUDIO |
                                         MEDIAPLAYER_RESOURCE_GRAPHICS,
                                         this);
    }
    else
    {
        doSetSource( source );
    }
}

void AppMediaPlayer_Video_Controller::stop()
{
    MP_LOG << LOG_ENDL;

    if(m_pControllerCurrent)
    {
        // m_pEngineVideo->getEngineMain().GetMediaManager()->setGstVideoMode(true);  // removed by kihyung 2013.2.7
        
        // { modified by kihyung 2012.07.31 for C 11949
        /*
        m_pControllerCurrent->stop();
        */
        MEDIA_SOURCE_T eCurrMedia = getSource();
        if(eCurrMedia == MEDIA_SOURCE_DVDVIDEO ||
           eCurrMedia == MEDIA_SOURCE_DVDAUDIO ||
           eCurrMedia == MEDIA_SOURCE_VCD) 
        {
            m_pControllerCurrent->functionStop();
        }
        else
        {
            m_pControllerCurrent->stop();
        }
        // { modified by kihyung
    }
}

// { added by kihyung 2013.2.3
void AppMediaPlayer_Video_Controller::stopAux()
{
    MP_LOG << LOG_ENDL;
    //m_pEngineVideo->getEngineMain().GetMediaManager()->setGstVideoMode(true);  // removed by kihyung 2013.2.7
    m_pGPlayerEngine->setIsAuxVideoMode(false); //added by shkim for PV2 JB Video_Aux Video Flickering Issue 2013.11.06
    m_pGPlayerEngine->Stop();
    
}
// } added by kihyung 2013.2.3

void AppMediaPlayer_Video_Controller::pause()
{
    MP_LOG << LOG_ENDL;

    if(m_pControllerCurrent)
    {
        m_pControllerCurrent->pause();
        //{ added by yongkyun.lee 20130310 for : NO CR Play and Puase
        if(m_pEngineVideo)
            m_pEngineVideo->setPassPlay (true);
        //} added by yongkyun.lee 20130310 
    }
}

void AppMediaPlayer_Video_Controller::next()
{
    MP_LOG << LOG_ENDL;

    if(m_pControllerCurrent)
    {
		// removed by Sergey 07.10.2013 for ITS#193743
        m_pControllerCurrent->next();
    }
}

void AppMediaPlayer_Video_Controller::prev()
{
    MP_LOG<<"play position : "<<PlayPosition()  << LOG_ENDL;

    if(m_pControllerCurrent)
    {
		// removed by Sergey 07.10.2013 for ITS#193743
        m_pControllerCurrent->prev();
    }
}

void AppMediaPlayer_Video_Controller::flickNext()
{
    MP_LOG << LOG_ENDL;

    if(m_pControllerCurrent)
    {
       //emit clearTpMessage(); // added by wspark 2013.04.09 for cluster IOT #37 // deleted by wspark 2013.04.17
       m_pControllerCurrent->flickNext();
    }
}

void AppMediaPlayer_Video_Controller::flickPrev()
{
    MP_LOG << LOG_ENDL;

    if(m_pControllerCurrent)
    {
        //emit clearTpMessage(); // added by wspark 2013.04.09 for cluster IOT #37 // deleted by wspark 2013.04.17
       m_pControllerCurrent->flickPrev();
    }
}

//{ added by yongkyun.lee 20130320 for : NO CR ff/rew OSD
bool AppMediaPlayer_Video_Controller::PrevNextOSD(bool vOSD)
{
    MP_MEDIUM << vOSD << LOG_ENDL;
    emit UpdateStateTrackOSD(vOSD); 
	m_isLongPress = vOSD;
	return true;//added by edo.lee 2013.07.15 STATIC TEST
}
//} added by yongkyun.lee 20130320 

void AppMediaPlayer_Video_Controller::fastForward()
{
   MP_LOG << LOG_ENDL;

   if(m_pControllerCurrent)
   {
       // { commented by cychoi 2013.09.09 for ITS 188943 Each FS/DVD/VCD controller do mute & set OSD flag 
       //GetEngineMain().setMuteForSeek(false); //added by edo.lee 2013.04.21 for Mute
       // } commented by cychoi 2013.09.09
       m_pControllerCurrent->fastForward();
       emit UpdateStateTrackOSD(true); // added by yungi 2012.11.14 for ISV 62667,ISV 62596
       m_isLongPress = true;
   }
}

void AppMediaPlayer_Video_Controller::fastForwardCritical()
{
   MP_LOG << LOG_ENDL;

   if(m_pControllerCurrent)
   {
       m_pControllerCurrent->fastForwardCritical();
       emit UpdateStateTrackOSD(true); // added by yungi 2012.11.14 for ISV 62667,ISV 62596
       m_isLongPress = true;
   }
}

void AppMediaPlayer_Video_Controller::rewind()
{
    MP_LOG << LOG_ENDL;

    if(m_pControllerCurrent)
    {
        // { commented by cychoi 2013.09.09 for ITS 188943 Each FS/DVD/VCD controller do mute & set OSD flag 
        //GetEngineMain().setMuteForSeek(false); //added by edo.lee 2013.04.21 for Mute
        // } commented by cychoi 2013.09.09
        m_pControllerCurrent->rewind();
        emit UpdateStateTrackOSD(true); // added by yungi 2012.11.14 for ISV 62667,ISV 62596
        m_isLongPress = true;
    }
}

void AppMediaPlayer_Video_Controller::rewindCritical()
{
    MP_LOG << LOG_ENDL;

    if(m_pControllerCurrent)
    {
       m_pControllerCurrent->rewindCritical();
       emit UpdateStateTrackOSD(true); // added by yungi 2012.11.14 for ISV 62667,ISV 62596
       m_isLongPress = true;
    }
}

void AppMediaPlayer_Video_Controller::cancel_ff_rew()
{
    MP_LOG << LOG_ENDL;
    if ( m_pControllerCurrent )
    {
       //m_pEngineVideo->getEngineMain().setUnMuteState(false); //added by edo.lee 2013.05.19
	   m_pControllerCurrent->cancel_ff_rew();
	   //if(m_pEngineVideo->getEngineMain().getisBTCall()) // remove by edo.lee 2013.09.03 ITS 0188120
	   //    m_pControllerCurrent->pause(); // remove by edo.lee 2013.09.03 ITS 0188120
	   
       emit UpdateStateTrackOSD(false); // added by yungi 2012.11.14 for ISV 62667,ISV 62596
       m_isLongPress = false;
    }
}

void AppMediaPlayer_Video_Controller::play()
{
    // { modified by kihyung 2013.07.18 for ITS 0180456
    // { added by wonseok.heo for  ITS 178767 2013.07.10
    /*remoe by edo.lee 2013.07.31
    if(GetEngineMain().getPowerOffBTCall() == true) {
        MP_LOG << "GetEngineMain().getPowerOffBTCall() == true" << LOG_ENDL;
        GetEngineMain().setPowerOffBTCall(false);
        // removed by Sergey 26.07.2013 for ITS#181096
        return;
    }*/ 
    // } added by wonseok.heo for  ITS 178767 2013.07.10

    if ( m_pControllerCurrent )
    {
        MP_HIGH << "powerOffBt =" << GetEngineMain().getPowerOffBTCall() << "isBtCall =" << GetEngineMain().getisBTCall() << LOG_ENDL; // modified by Sergey 16.10.2013 for ITS#195185
        
        // emit clearTpMessage(); // added by wspark 2013.04.09 for cluster IOT #37 // deleted by wspark 2013.04.17
        // m_pEngineVideo->getEngineMain().GetMediaManager()->setGstVideoMode(true);   // removed by kihyung 2013.2.7
        // m_pEngineVideo->getEngineMain().setUnMuteState(false);//added by edo.lee 2013.04.20
        // modified by edo.lee 2013.04.22
        /*if(m_pMuteTimer->isActive())
            m_pMuteTimer->stop();
        m_pMuteTimer->start();*/
		//modified by edo.lee 2013.08.01 bt call on pwroff

		// { modified by Sergey 11.10.2013 for ITS#194942
		if(isFSSource(m_mediaSource))
		{
			m_pControllerCurrent->play(); // modified by Sergey 16.10.2013 for ITS#195185
		}
		else // DVD, VCD
		{
		if(GetEngineMain().getPowerOffBTCall() && GetEngineMain().getisBTCall())//modified by edo.lee 2013.08.21
			pauseByCallOnPowerOff();		
		else{
	        m_pControllerCurrent->play();				
			GetEngineMain().resetKeepPlayState();
		}
		//modified by edo.lee 2013.08.01 bt call on pwroff
    }
    }
    // } modified by kihyung 2013.07.18 for ITS 0180456
    // } modified by Sergey 11.10.2013 for ITS#194942
}
void
AppMediaPlayer_Video_Controller::pauseByCallOnPowerOff()
{
	MP_LOG << LOG_ENDL;
	if(m_pIMedia)
	{
		m_pIMedia->SetSystemState(SYSTEM_BT_CALL_START);
		m_pControllerCurrent->play();
		m_pIMedia->SetSystemState(SYSTEM_BT_CALL_END);

		GetEngineMain().setPowerOffBTCall(false);
	}
}

// { added by kihyung 2013.2.3
void AppMediaPlayer_Video_Controller::playAux()
{
    MP_LOG << LOG_ENDL;

    // m_pEngineVideo->getEngineMain().GetMediaManager()->setGstVideoMode(true);   // removed by kihyung 2013.2.7
    // { added by Jeeeun 2013.02.26 for SANITY_CM_AF017
    ASPECT_RATIO_T currAspectRatio;
    QRect currRect;
    currAspectRatio = getAspectRatio();
    // } added by Jeeeun 2013.02.26 for SANITY_CM_AF017
    m_pGPlayerEngine->SetURI("/dev/video/aux");
    m_pGPlayerEngine->setIsAuxVideoMode(true); //added by shkim for PV2 JB Video_Aux Video Flickering Issue 2013.11.06
    m_pGPlayerEngine->Play();
    m_pVideoWidget =  m_pGPlayerEngine->m_pVideoWidget;
    if( m_pVideoWidget )
    {
        MP_LOG<< "resize video widget" << LOG_ENDL;
        // { added by Jeeeun 2013.02.26 for SANITY_CM_AF017  
        currRect = m_tableAspectRatio[currAspectRatio];
        //{ added by hyochang.ryu 20130807 for ITS_181152
        if(currRect.width() > 1280 || currRect.height() > 720)
        { 
            float w, h, r;
            if(currRect.width() > 1280) 
            {
                r = currRect.height() / currRect.width();
                h = r * 1280;
                currRect.setWidth(1280);
                currRect.setHeight((int)h);
            }
            if(currRect.height() > 720) 
            {
                r = currRect.width() / currRect.height();
                w = r * 720;
                currRect.setWidth((int)w);
                currRect.setHeight(720);
            }
        }
        //} added by hyochang.ryu 20130807 for ITS_181152
        m_pVideoWidget->resize(currRect.width(), currRect.height());
        // } added by Jeeeun 2013.02.26 for SANITY_CM_AF017
        //m_pVideoWidget->resize(1280, 720); // removed by Jeeeun 2013.02.26 for SANITY_CM_AF017

        emit playAuxOnFG(); // added by radhakrushna 2013.03.11 for ISV 74997
    }    
}
// } added by kihyung 2013.2.3

//{ added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD       
int AppMediaPlayer_Video_Controller::PlayPosition()
{
    if(m_pIMedia != NULL)
    {
       return m_pIMedia->GetCurrentMediaPosition();
    }

    return 0;
}
// }added by  yongkyun.lee

//{ modified by yongkyun.lee 2013-09-05 for : ITS 188359
bool AppMediaPlayer_Video_Controller::IsPlaying()
{
    if ( m_pControllerCurrent )
    {
       if(m_pControllerCurrent->getModel()->playbackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING )
       {
            return true;
       }
       return false;
    }
    return false;
}
//} modified by yongkyun.lee 2013-09-05 

PLAYBACK_STATUS_T AppMediaPlayer_Video_Controller::getPlaybackStatus()
{
    if ( m_pControllerCurrent )
    {
       return m_pControllerCurrent->getModel()->playbackStatus();
    }
    return AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_INVALID;
}

// { added by lssanh 2013.07.02 ITS176637
bool AppMediaPlayer_Video_Controller::getLoadingStatus()
{
	MP_LOG << LOG_ENDL;

    if ( m_pControllerCurrent )
    {
    MP_LOG << "loading mode =" << m_pControllerCurrent->getModel()->loadingMode() << LOG_ENDL;
       return m_pControllerCurrent->getModel()->loadingMode();
    }
    return false;
}
// } added by lssanh 2013.07.02 ITS176637


// { modified by ravikanth 27-12-12
void AppMediaPlayer_Video_Controller::setDarkness( DISPLAY_PLANES_T display, int value )
{
    Q_UNUSED(display);
    emit contrastChanged(value);
}

void AppMediaPlayer_Video_Controller::setColor( DISPLAY_PLANES_T display, int value )
{
    Q_UNUSED(display);
    emit hueChanged(value);
}

void AppMediaPlayer_Video_Controller::setSaturation( DISPLAY_PLANES_T display, int value )
{
    Q_UNUSED(display);
    emit saturationChanged(value);
}

void AppMediaPlayer_Video_Controller::setBrightness( DISPLAY_PLANES_T display, int value )
{
    Q_UNUSED(display);
    emit brightnessChanged(value);
}
// } modified by ravikanth 27-12-12

// { modified by Sergey for ITS#181872 30.07.2013
void AppMediaPlayer_Video_Controller::showVideo(DISPLAY_T aDisplay)
{
    MP_HIGH << "display =" << aDisplay << LOG_ENDL; // added by Sergey 05.11.2013 for ITS#212560

    if((aDisplay != DISPLAY_FRONT) && (aDisplay != DISPLAY_REAR))
    {
        MP_LOG << "Unknown display. Return." << LOG_ENDL;
        return;
    }

    // removed by Sergey 24.09.2013 for ITS#191438

    moveWidget(aDisplay);

    // removed by Sergey 24.09.2013 for ITS#191438

	// { addeed by edo.lee 2013.08.15 for new entry for subscreen
	if( GetEngineMain().getLaunchSubscreen())
	{
		MP_LOG<<"subscreen in AVP is loaded - for clear flickering" << LOG_ENDL;
		clearDisplay(true);
	}
	// } addeed by edo.lee 2013.08.15 for new entry for subscreen
    else
    {
        clearDisplay(false);
    }
    //added by shkim for ITS 178085
}
// } modified by Sergey for ITS#181872 30.07.2013

// removed by Sergey 31.07.2013

// { added by Sergey 26.05.2013
void AppMediaPlayer_Video_Controller::changeWidgetDisplay(DISPLAY_T aDisplay)
{
    if((aDisplay != DISPLAY_FRONT) && (aDisplay != DISPLAY_REAR) || m_pVideoWidget == NULL)
    {
        MP_LOG << "Unknown display. Return." << LOG_ENDL;
        return;
    }

    QRect rect = m_pVideoWidget->geometry();
    int curHeight = rect.height();

    MP_HIGH << "curHeight =" << curHeight << LOG_ENDL; // added by Sergey 05.11.2013 for ITS#212560

    if(rect.y() >= 720 && aDisplay == DISPLAY_FRONT)
        rect.setY(rect.y() - 720);
    else if(rect.y() < 720 && aDisplay == DISPLAY_REAR)
        rect.setY(rect.y() + 720);

    MP_HIGH << "curHeight after moving =" << curHeight << LOG_ENDL; // added by Sergey 05.11.2013 for ITS#212560
    //QRect bug: when you change the Y coordinate,
    // value of height changes  from 'height' to 'height-Y'
    if(rect.height() <= 0) // modified by Sergey 05.11.2013 for ITS#212560
        rect.setHeight(curHeight);

    SetVideoWidgetSize(rect.x(), rect.y(), rect.width(), rect.height());
    // removed by cychoi 2013.07.03 for ITS 177711 - Camera and DVD/VCD Video is mixed.
}
// } added by Sergey 26.05.2013

void AppMediaPlayer_Video_Controller::moveWidget(DISPLAY_T aDisplay)
{    
    if((aDisplay != DISPLAY_FRONT) && (aDisplay != DISPLAY_REAR))
    {
        MP_LOG << "Unknown display. Return." << LOG_ENDL;
        return;
    }

    QRect widgetSize = m_tableAspectRatio.value(ASPECT_RATIO_T(m_currAspectRatio), QRect(0, 0, 1280, 720));

	if( m_pEngineVideo->getIsShowLockoutView() == VIDEO_DISPLAY_FRONT)
	{
		aDisplay = DISPLAY_REAR;
	}else if ( m_pEngineVideo->getIsShowLockoutView() == VIDEO_DISPLAY_REAR)
	{
		aDisplay = DISPLAY_FRONT;
	}
	
    if(aDisplay == DISPLAY_REAR )//added by edo.lee 2013.02.18
    {        
        //QRectangle magic: when you change the Y coordinate,
        // value of height changes  from 'height' to 'height-Y'
	
        //widgetSize.setHeight( widgetSize.height() + 720 );
		widgetSize.setY(widgetSize.y()+720);
		widgetSize.setHeight( widgetSize.height() + 720 );

    }else
	{
		if(widgetSize.y() > 720){
            widgetSize.setY(widgetSize.y() - 720);
        }
	}

	

    MP_HIGH << "aDisplay:" << aDisplay << LOG_ENDL;
    SetVideoWidgetSize(widgetSize.x(), widgetSize.y(), widgetSize.width(), widgetSize.height());
    m_pEngineVideo->SetVideoSize(aDisplay); // modified by cychoi 2013.07.03 for ITS 177711 //added by yungi 2013.05.23 for ITS 167826
}

void AppMediaPlayer_Video_Controller::SetVideoWidgetSize(int ax, int ay, int aw, int ah)
{
    static QWidget *pVideoWidget = 0;
    static int      x = -1;
    static int      y = -1;
    static int      w = -1;
    static int      h = -1;
    
    if( m_pVideoWidget )
    {
        if(pVideoWidget != m_pVideoWidget || x != ax || y != ay || w != aw || h != ah) 
        {
            pVideoWidget = m_pVideoWidget;
            x = ax;
            y = ay;
            w = aw;
            h = ah;
                
            MP_HIGH << "widget = " << m_pVideoWidget << "x : " << ax << "y : " << ay << "w : " << aw << "h : " << ah << "swap =" << m_pEngineVideo->getEngineMain().IsSwapDCEnabled() << LOG_ENDL; // modified by Sergey 23.12.2013 for ITS#216921
            m_pVideoWidget->setGeometry(ax, ay, aw, ah);
        }
    }
}
// } modified by Sergey for CR#15775

void AppMediaPlayer_Video_Controller::SetVideoSizeWrtDisplay(int ax, int ay, int aw, int ah)
{
    MP_LOG << LOG_ENDL;
    if(m_pEngineVideo->getCurrentScreen() == DISPLAY_FRONT) // modified by Sergey for CR#16815
    {
        SetVideoWidgetSize(ax, ay, aw, ah);
    }
    else
    {
        SetVideoWidgetSize(ax, ay + 720, aw, ah);
    }
}

void AppMediaPlayer_Video_Controller::SetAspectRatio( ASPECT_RATIO_T aspectRatio )
{
    MP_LOG << "aspectRatio = " << aspectRatio << LOG_ENDL;
    m_currAspectRatio = aspectRatio;
    m_pSettingsStorage->SaveSetting(int(aspectRatio),VideoSettingsDef::DB_KEY_LAST_ASPECT_RATIO); // modified by ravikanth 06-03-13
}
// { added by jeeeun for PAR implementation
ASPECT_RATIO_T AppMediaPlayer_Video_Controller::getAspectRatio()
{
    MP_LOG << "aspectRatio = " << m_currAspectRatio << LOG_ENDL;
    return m_currAspectRatio;
}

void AppMediaPlayer_Video_Controller::SetActualRect(QRect aRect)
{
    m_actualRect = aRect;
}

void AppMediaPlayer_Video_Controller::SetPAR(float aPar)
{
    m_par = aPar;
}

void AppMediaPlayer_Video_Controller::SetDAR(float aDar)
{
    m_dar = aDar;
}
// } added by jeeeun for PAR implementation

AppMediaPlayer_Video_ControllerBase* AppMediaPlayer_Video_Controller::getController( MEDIA_SOURCE_T mediaSource )
{
   // MP_LOG << " mediaSource: " << mediaSource << LOG_ENDL;
   return m_tableOfControllers.value( mediaSource, NULL );
}

void AppMediaPlayer_Video_Controller::onError(int errCode)
{
    MP_LOG << errCode << LOG_ENDL;
}

void AppMediaPlayer_Video_Controller::onPositionChanged(int position)
{
    if ( m_pControllerCurrent != NULL )
    {
        // { added by cychoi 2013.08.27 for ITS 184304 Loading screen close
        // Unfortunately the only case to know that track really started playing is
        // to catch onPositionChanged signal. Catching onTrackChanged or OnStateChanged
        // can't be used as they come too early while Deck is still not ready
        // to get new requests.
        if(m_pControllerCurrent->getModel()->loadingMode() && position >= 1000)
        {
            m_pControllerCurrent->getModel()->setLoadingMode(false);
        }
        // } added by cychoi 2013.08.27

        m_pControllerCurrent->getModel()->setPosition( position );
    }

	emit positionChanged(position);

	if(m_pEngineVideo->getEngineMain().getIsTermOnTrackChange() == false) {
        // { modified by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
        if(m_mediaSource == MEDIA_SOURCE_DVDVIDEO ||
           m_mediaSource == MEDIA_SOURCE_DVDAUDIO ||
           m_mediaSource == MEDIA_SOURCE_VCD)
        {
            m_pEngineVideo->updatePosition((OSD_DEVICE)getOSDDevice(m_mediaSource), position ,IsFFRewState());
        }
        else
        {
            m_pEngineVideo->updatePosition((OSD_DEVICE)getOSDDevice(m_mediaSource), position ,m_isLongPress);
        }
        // } modified by cychoi 20103.10.16
	}

    AppMediaPlayer_SettingsStorage::GetInstance()->SaveVideoDataForcedSpec(this); // added by kihyung 2013.07.30 for ITS 0181638
} 

// modified by Sergey 18.05.2013
int AppMediaPlayer_Video_Controller::getOSDDevice(MEDIA_SOURCE_T media)
{
    OSD_DEVICE returnval = DEVICE_MAX;
    switch(media)
    {
    case MEDIA_SOURCE_JUKEBOX:
        returnval = JUKEBOX_VIDEO;
        break;
    case MEDIA_SOURCE_USB1:
    case MEDIA_SOURCE_USB2:
        returnval = USB_VIDEO;
        break;
    // { modified by cychoi 2013.11.12 for OSD Title Translation
    case MEDIA_SOURCE_DVDVIDEO:
        returnval = DISC_DVD_VIDEO;
        break;
    case MEDIA_SOURCE_VCD:
        returnval = DISC_VCD_VIDEO;
        break;
    // } modified by cychoi 2013.11.12
    case MEDIA_SOURCE_AUX1:
    case MEDIA_SOURCE_AUX2:
        returnval = AUX_VIDEO;
        break;
    default:
        //
        returnval = DEVICE_MAX;
    }
    return returnval;
}

void AppMediaPlayer_Video_Controller::onDurationChanged(int duration)
{
    MP_LOG << "duration = " << duration << LOG_ENDL;
    
    // modified by richard 2012.10.25 for navi slowness fix
    // emit durationChanged(duration / 1000);
	emit durationChanged(duration);
}

// { modified by Sergey for CR#15775
void AppMediaPlayer_Video_Controller::onTrackChanged(int index)
{
    MP_LOG << "index = " << index << LOG_ENDL;
    GetEngineMain().setIsFROnTune(false); //added by edo.lee 2013.03.25
    emit tuneTimerExpired(); //added ITS 188647
    //emit trackChanged(index, 0); //commented by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
}
// } modified by Sergey for CR#15775

/* Here we need to track the currently running state change command.
 * We have a defined expectation of the command final state, and if
 * we match it, we must handle the next command in the queue.
 * */
void AppMediaPlayer_Video_Controller::onStateChanged(int state)
{
    if ( m_pControllerCurrent == NULL )
    {
        return;
    }

    MP_HIGH << "State = " << state << LOG_ENDL; // modified by Sergey 10.10.2013 for ITS#194442

    AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T eOldPlayback = m_pControllerCurrent->getModel()->playbackStatus();
    

    // Update the model:
    switch( state )
    {
    case PLAYER_PLAY:
    case PLAYER_REPEATPLAY:
    case PLAYER_RANDOMPLAY:
        {
            m_pControllerCurrent->getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING );
            // { modified by cychoi 2013.08.27 for ITS 184304 Loading screen close
            // { modified by cychoi 2013.12.19 for ITS 216591 close VCD loading screen when changed play position
            if(m_mediaSource == MEDIA_SOURCE_VCD)
            {
                if(m_pEngineVideo->isPBCOff())
                {
                    if(m_pControllerCurrent->getModel()->loadingMode())
                        m_pControllerCurrent->getModel()->setLoadingMode(false); // added by wspark 2013.01.16
                }
            }
            // } modified by cychoi 2013.12.19
            else
            {
                if(m_pControllerCurrent->getModel()->loadingMode())
                    m_pControllerCurrent->getModel()->setLoadingMode(false); // added by wspark 2013.01.16
            }
            // } modified by cychoi 2013.08.27

            m_pEngineVideo->setMaintainPause(false); // added by Sergey 11.10.2013 for ITS#194942
        }
        break;

    //{ added by cychoi 2013.08.13 for ITS 183912 Sound mute on toggle play
    case PLAYER_SEARCH:
        {
            if(m_mediaSource == MEDIA_SOURCE_DVDVIDEO || m_mediaSource == MEDIA_SOURCE_VCD)
            {
                // { modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error 
                // Users should think PLAYER_SEARCH as playing status.
                // 1. Controller's Command -> Search -> Play/FF/REW 2. Random Play -> Search -> Random Play
                if(eOldPlayback < AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_4X)
                {
                    m_pControllerCurrent->getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING );
                }
                // } modified by cychoi 2013.09.22
                // { commented by cychoi 2013.08.27 for ITS 184304 Loading screen close
                //if(m_pControllerCurrent->getModel()->loadingMode())
                //     m_pControllerCurrent->getModel()->setLoadingMode(false);
                // } commented by cychoi 2013.08.27
            }
            // { added by Sergey 10.10.2013 for ITS#194442
            else if(m_mediaSource == MEDIA_SOURCE_JUKEBOX || m_mediaSource == MEDIA_SOURCE_USB1 || m_mediaSource == MEDIA_SOURCE_USB2)
            {
                m_pControllerCurrent->getModel()->setLoadingMode(true);
                m_pEngineVideo->repaintUI(); // modified by Sergey 18.10.2013 for safe ui update
            }
            // } added by Sergey 10.10.2013 for ITS#194442
        }
        break;
    //} added by cychoi 2013.08.13

    case PLAYER_PAUSE:
    case PLAYER_REPEATPAUSE:
    case PLAYER_RANDOMPAUSE:
        if(m_pControllerCurrent->getModel()->dragging() == false)
        {
            m_pControllerCurrent->getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PAUSED );
            // { modified by cychoi 2013.08.27 for ITS 184304 Loading screen close
            if(m_pControllerCurrent->getModel()->loadingMode())
                m_pControllerCurrent->getModel()->setLoadingMode(false); // added by lssanh 2013.07.02 ITS176637 
            // } modified by cychoi 2013.08.27
        }
        break;

    case PLAYER_STOP:
        m_pControllerCurrent->getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_STOPPED );
        break;
    case PLAYER_FASTFORWARD_X4:
        m_pControllerCurrent->getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_4X );

        break;
    case PLAYER_FASTFORWARD_X16:
        m_pControllerCurrent->getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_16X);

        break;
    case PLAYER_FASTFORWARD_X20:
        m_pControllerCurrent->getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_20X);

        break;
    case PLAYER_REWIND_X4:
        m_pControllerCurrent->getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_4X );

        break;
    case PLAYER_REWIND_X16:
        m_pControllerCurrent->getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_16X );

        break;
    case PLAYER_REWIND_X20:
        m_pControllerCurrent->getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_20X );

        break;

    case PLAYER_NEXTTRACK:
    case PLAYER_PREVTRACK:
    case PLAYER_SCAN:
        break;

    // { added by kihyung 2013.09.12 for ISV 90634
    case PLAYER_CHANGE_PLAY_ICON:
        m_pControllerCurrent->getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING );
        break;
        
    case PLAYER_CHANGE_PAUSE_ICON:
        m_pControllerCurrent->getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PAUSED );
        break;
    // } added by kihyung 2013.09.12 for ISV 90634
    
    default:
        break;
    }

    // { added by Sergey 13.10.2013 for ITS#195262
    // if(m_pControllerCurrent != NULL && m_pControllerCurrent->getModel()) //comment this line due to duplicated null checking "m_pControllerCurrent", honggi.shin 2013.10.25
    if( m_pControllerCurrent->getModel() )  //added by honggi.shin 2013.10.25, fix static test problem
    {
        if(eOldPlayback != m_pControllerCurrent->getModel()->playbackStatus()) {
            AppMediaPlayer_SettingsStorage::GetInstance()->SaveVideoSettings(this);
        }
    }
    // } added by Sergey 13.10.2013 for ITS#195262

    // Inform Notifier (maybe some one else) about state change.
    emit stateChanged(state);
}

void AppMediaPlayer_Video_Controller::onModeChanged(int mode )
{
    MP_LOG << "Mode = " << mode << LOG_ENDL;

    if ( m_pControllerCurrent == NULL )
    {
        return;
    }

    bool bChanged = false;
    
    AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T eOldRandomMode = m_pControllerCurrent->getModel()->randomMode();
    AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T eOldRepeatMode = m_pControllerCurrent->getModel()->repeatMode();

    switch( mode )
    {
    case RANDOMOFF:
        m_pControllerCurrent->getModel()->setRandomMode( AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_OFF );
        break;

    case RANDOMFILE:
        m_pControllerCurrent->getModel()->setRandomMode( AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_FILE );
        break;

    case RANDOMFOLDER:
        m_pControllerCurrent->getModel()->setRandomMode( AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_FOLDER );
        break;

    case REPEATFILE:
        m_pControllerCurrent->getModel()->setRepeatMode( AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_FILE );
        break;
		
    case REPEATOFF:
    case REPEATALL:
        m_pControllerCurrent->getModel()->setRepeatMode( AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_OFF );
        break;

    case REPEATFOLDER:
        m_pControllerCurrent->getModel()->setRepeatMode( AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_FOLDER );
        break;
    }

    if(eOldRandomMode != m_pControllerCurrent->getModel()->randomMode() || 
       eOldRepeatMode != m_pControllerCurrent->getModel()->repeatMode())
    {
        bChanged = true;
    }

    // Inform Notifier (maybe some one else) about mode change.
    emit playingModeChanged((MEDIA_MODE)mode);
	m_pEngineVideo->updatePlayingMode((OSD_DEVICE)getOSDDevice(m_mediaSource), 
	m_pControllerCurrent->getModel()->randomMode() ,
	m_pControllerCurrent->getModel()->repeatMode(),
	0);
    if(bChanged == true) {
        AppMediaPlayer_SettingsStorage::GetInstance()->SaveVideoSettings(this);
    }
}


/* DD notes:
 * Currently we drop doing anything, when we're requested to drop the
 * resources. However, it might be possible that only graphics resources
 * are release (photo is on top), so we might want not to stop the media.
 * */
void AppMediaPlayer_Video_Controller::onReleaseResources( MEDIAPLAYER_RESOURCES_T resources )
{
    Q_UNUSED(resources);
    m_isResourcesOwned = false;
    MP_LOG << LOG_ENDL;
    // stop();  // commented by Sergey for CR#16771
    //m_pIMedia = NULL; // removed by kihyung 2013.5.17

    releaseSource();

    QMetaObject::invokeMethod( &GetControllers(),
                              "onResourceReleased",
                              Q_ARG(MEDIAPLAYER_RESOURCES_T, resources) );
}


/* A little bit untrivial stuff needs to happen here.
 * We might not having graphics output stuff up to the moment just in case
 * the controller is intialized before the QML UI is loaded. In this case
 * we cannot do anything clever -- we must wait till UI will be ready.
 *
 * However, there is an ugly looking background playback startup: the
 * playback is started as soon as possible (before the QML is loaded
 * completely).
 * */
void AppMediaPlayer_Video_Controller::onResourceGranted()
{
   MP_LOG << LOG_ENDL;
   
   doSetSource( m_pendingSource );

   m_isResourcesOwned = true;
}

CTrackerAbstractor* AppMediaPlayer_Video_Controller::GetTrackerAbstractor()
{ 
    if(m_pTrackerAbs == NULL) 
    {
        // m_pTrackerAbs = new CTrackerAbstractor();
        m_pTrackerAbs = GetEngineMain().GetTrackerAbs();
        if(m_pTrackerAbs != NULL) {
            connect(m_pTrackerAbs, SIGNAL(RequesteComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, uint)),
                                    this,   SLOT(HandleReqComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, uint)));
            connect(m_pTrackerAbs, SIGNAL(RequesteComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, ETrackerAbstarctor_VolumeType, uint)),
                                    this,   SLOT(HandleReqComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, ETrackerAbstarctor_VolumeType, uint)));
            m_pTrackerAbs->setDeviceId(eJukeBox, NULL, "/usr_data/media");
        }
    }
    
    return m_pTrackerAbs;
}

void AppMediaPlayer_Video_Controller::releaseSource()
{
    MP_HIGH << LOG_ENDL;
    // { added by junam 2012.10.18 for multiple seek command
    if ( isSeekMode() )
    {
        cancel_ff_rew();
    }
    // } added by junam

    // { added by Sergey for CR#16771
    /* Stub:CR#15575. When stop DVD/VCD video widget blinks with green patch.
     * To hide it we overlap widget with black rectangle before the stop.
     */
     
	//GetEngineMain().setGstPipelineStatus(true);	//moved by hyochang.ryu 20130618 after stop for AUX/DVD	//added by edo.lee 2013.03.26
    // removed by cychoi 2013.08.30 for ITS 187026 DVD Title/Disc Menu Loading Screen
    // { added by kihyung 2013.1.30 for ISV 70603
    if(m_mediaSource == MEDIA_SOURCE_VCD) {
        m_pEngineVideo->setPBCOff();
        // { added by cychoi 2014.03.28 for ITS 232560
        m_pEngineVideo->initDirectAccessDisplayed(VIDEO_DISPLAY_FRONT, false);
        m_pEngineVideo->initDirectAccessDisplayed(VIDEO_DISPLAY_REAR, false);
        // } added by cychoi 2014.03.28
    }
    // } added by kihyung
    // added by eugene.seo 2013.01.28 for video switch issue from usb to jukebox when usb is removed
    //{ deleted by yongkyun.lee 20130206 for : ISV 70913
    //    if(m_mediaSource == MEDIA_SOURCE_DVDVIDEO || m_mediaSource == MEDIA_SOURCE_VCD) 
    //   {
    //        QEventLoop* loop = new QEventLoop(this);   
    //        loop->processEvents(QEventLoop::ExcludeUserInputEvents, 100); // modified by eugene.seo 2013.01.30 for rollback to origin value 1 -> 100
    //        // } added by Sergey for CR#16771
    //    }
    //} deleted by yongkyun.lee 20130206 

    // { modified by kihyung 2013.2.3
    MP_LOG << "Aux m_mediaSource:" << m_mediaSource << LOG_ENDL;
    GetEngineMain().setReleaseGstFromVideo(true);
    if(m_mediaSource == MEDIA_SOURCE_AUX1 || m_mediaSource == MEDIA_SOURCE_AUX2) 
    {
        stopAux();
        GetEngineMain().setGstPipelineStatus(true); // moved by kihyung 2013.06.27
    }
    else if(m_pControllerCurrent != NULL)
    {
        // Set the model dirty flag, as once released model data does not
        // match the controller state.
        MP_LOG<<"m_TuneInProgress : " <<m_TuneInProgress << LOG_ENDL;
        if(m_TuneInProgress)// added by Radhakrushna for CR#14111
        	m_pControllerCurrent->cancelTune();
        m_pControllerCurrent->getModel()->setDirty(true);
        stop();
        m_pControllerCurrent->dropControllers();
        m_pControllerCurrent->getModel()->setPlaybackStatus( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_STOPPED );
    }
    else
    {
        // let's try to stop anyway.
        stop();
    }
    // } modified by kihyung 2013.2.3

    MP_LOG << "m_mediaSource" << m_mediaSource << LOG_ENDL;

    // GetEngineMain().setGstPipelineStatus(true);      // moved by hyochang.ryu 20130601	//added by edo.lee 2013.03.26

    ReleaseMediaSource( m_mediaSource );
    m_LastMediaSource = m_mediaSource;
    m_mediaSource = MEDIA_SOURCE_MAX;
    m_pIMedia = NULL;
    m_pControllerCurrent = NULL;
    m_pEngineVideo->setLastAVMode(MODE_STATE_NONE); // added by ruindmby 2012.09.11 for CR#13584

    emit sourceReleased(); // added by Sergey 14.08.2013 for ITS#184081
}

//{ modified by yongkyun.lee 2013-09-05 for : ITS 188359
void AppMediaPlayer_Video_Controller::setDiscSource( MEDIA_SOURCE_T source)
{
    doSetSource(source);
}
//} modified by yongkyun.lee 2013-09-05 

void AppMediaPlayer_Video_Controller::doSetSource( MEDIA_SOURCE_T source)
{
    MP_LOG << "[source]" << source << LOG_ENDL;
    
    releaseSource();

    m_mediaSource = source;

    m_pControllerCurrent = getController(m_mediaSource);

    m_pEngineVideo->DisplayOSD(true); //added by edo.lee 2013.04.01    

    if( m_pControllerCurrent ) // added by Sergey 04.10.2013 for ITS#193849
    {
        switch( m_mediaSource )
        {
        case MEDIA_SOURCE_JUKEBOX:
            m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(true);//modified by edo.lee 2013.08.10 ITS 0182918
            m_pEngineVideo->setOSDMetaData(JUKEBOX_VIDEO, ((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->filename(),
                                           ((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->randomMode(),
                                           ((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->repeatMode(),0, false);
            m_pEngineVideo->getEngineMain().GetNotifier()->setHistoryInfo(m_mediaSource,((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->filename(),
                                                                          ((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->foldername(),
                                                                          ((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->position());
            break;
        case MEDIA_SOURCE_USB1:
            m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(true);//modified by edo.lee 2013.08.10 ITS 0182918
            m_pEngineVideo->setOSDMetaData(USB_VIDEO, ((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->filename(),
                                           ((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->randomMode(),
                                           ((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->repeatMode(),0, false);
            m_pEngineVideo->getEngineMain().GetNotifier()->setHistoryInfo(m_mediaSource,((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->filename(),
                                                                          ((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->foldername(),
                                                                          ((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->position());

            break;
        case MEDIA_SOURCE_USB2:
            m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(true);//modified by edo.lee 2013.08.10 ITS 0182918
            m_pEngineVideo->setOSDMetaData(USB_VIDEO, ((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->filename(),
                                           ((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->randomMode(),
                                           ((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->repeatMode(),0, false);
            m_pEngineVideo->getEngineMain().GetNotifier()->setHistoryInfo(m_mediaSource,((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->filename(),
                                                                          ((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->foldername(),
                                                                          ((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->position());

            break;
        case MEDIA_SOURCE_DVDVIDEO:
            ((AppMediaPlayer_Video_ControllerDVD*)m_pControllerCurrent)->showOSD(true);
            m_pEngineVideo->getEngineMain().GetNotifier()->setHistoryInfo(m_mediaSource,QString::number(((AppMediaPlayer_Video_CModel_DVD*)m_pControllerCurrent->getModel())->chapter()),
                                                                          QString::number(((AppMediaPlayer_Video_CModel_DVD*)m_pControllerCurrent->getModel())->title()),
                                                                          ((AppMediaPlayer_Video_CModel_DVD*)m_pControllerCurrent->getModel())->position());
            break;
        case MEDIA_SOURCE_VCD:
            ((AppMediaPlayer_Video_ControllerVCD*)m_pControllerCurrent)->showOSD(true);
            m_pEngineVideo->getEngineMain().GetNotifier()->setHistoryInfo(m_mediaSource,QString::number(((AppMediaPlayer_Video_CModel_VCD*)m_pControllerCurrent->getModel())->currentTrack()),
                                                                          "",
                                                                          ((AppMediaPlayer_Video_CModel_VCD*)m_pControllerCurrent->getModel())->position());

            break;
        default:
            // make compiler happy
            break;
        }
    } // added by Sergey 04.10.2013 for ITS#193849
    //{ modified by hyochang.ryu 20131024 for ITS 192354
    else
    {
        switch( m_mediaSource )
        {
        case MEDIA_SOURCE_AUX1:
        case MEDIA_SOURCE_AUX2:
            //not use
            /*
            m_pVideoWidget =  m_pGPlayerEngine->m_pVideoWidget;//added by edo.lee 2013.03.08 ISV 74842
            m_pEngineVideo->NofityAudioPathChanged(MODE_STATE_AUX1_VIDEO);
            setAUXSource(source);
            m_pEngineVideo->getEngineMain().GetNotifier()->setHistoryInfo(m_mediaSource,"","",0);		//added by hyochang.ryu 20130717 for ITS179249
            */
            //emit checkAuxMode();
            OSDManager::instance()->checkAuxMode();	//modified by hyochang.ryu 20131105 
            break;
        default: // make complier happy
            break;
        }
    }
    //} modified by hyochang.ryu 20131024 for ITS 192354

    AcquireMediaSource( source );

    if( m_pControllerCurrent )
    {
        m_pControllerCurrent->attachControllers();
        m_pControllerCurrent->getModel()->setDirty(false);
        m_pIMedia = m_pControllerCurrent->getIMedia();
        m_pVideoWidget =  m_pIMedia->GetVideoWidget();
        MP_LOG << "widget =" << m_pVideoWidget << LOG_ENDL;
    }
    else
    {
        m_pIMedia = NULL;
    }

    emit deviceChanged(m_mediaSource);
}

void AppMediaPlayer_Video_Controller::setAUXSource(MEDIA_SOURCE_T source)
{
    // { modified by kihyung 2013.2.3   
    Q_UNUSED(source);
    MP_LOG << LOG_ENDL;
    if(m_pEngineVideo->getIsFgReceived() == false) {
        playAux();
    }
    // } modified by kihyung 2013.2.3    
}

// { modified by Sergey for CR16250
void AppMediaPlayer_Video_Controller::onDeviceAttached( MEDIA_SOURCE_T source,
                                                        bool autoplay )
{
    Q_UNUSED(autoplay);

    if ( (source == MEDIA_SOURCE_USB1) || ( source == MEDIA_SOURCE_USB2 ) || ( source == MEDIA_SOURCE_JUKEBOX ))
        static_cast <AppMediaPlayer_Video_ControllerFS*>(getController(source))->onDeviceAttached();
}

void AppMediaPlayer_Video_Controller::onDeviceRemoved(MEDIA_SOURCE_T source)
{
    if(source == MEDIA_SOURCE_USB1 || source == MEDIA_SOURCE_USB2 || source == MEDIA_SOURCE_JUKEBOX)
    {
        // { modified by Sergey 24.09.2013 for ITS#191450
        if(source == m_mediaSource)
        {
            m_pControllerCurrent->getModel()->setRemovingMode(true);
            m_pEngineVideo->repaintUI(); // modified by Sergey 18.10.2013 for safe ui update
        }

        if(source == m_mediaSource) // This additional condition MUST be here!
            releaseSource();
        // } modified by Sergey 24.09.2013 for ITS#191450

        static_cast <AppMediaPlayer_Video_ControllerFS*>(getController(source))->onDeviceRemoved();
    }
    else if(source == MEDIA_SOURCE_DVDVIDEO || source == MEDIA_SOURCE_DVDAUDIO)
    {
        if(source == m_mediaSource)
            releaseSource();

        static_cast <AppMediaPlayer_Video_ControllerDVD*>(getController(source))->onDeviceRemoved();
    }
    else if(source == MEDIA_SOURCE_VCD)
    {
        if(source == m_mediaSource)
            releaseSource();

        static_cast <AppMediaPlayer_Video_ControllerVCD*>(getController(source))->onDeviceRemoved();
    }
}
// } modified by Sergey for CR16250

// removed by Sergey 30.04.2013 for camera delay issue

void AppMediaPlayer_Video_Controller::HandleReqComplete(QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    MP_LOG << "TASCAN -> queryStr =" << MAP_QUERY_IDS.key(queryId) << ", queryId =" << queryId << ", reqUID = " << reqUID << LOG_ENDL;
    MP_LOG << "TASCAN -> fileList.count =" << fileList.count() << LOG_ENDL;

    switch(queryId)
    {
    case eGetVideoFolderTabList_Id:
    {
        QStringList songList;
        
        foreach (QStringList list, fileList)
        {
            if(list.at(1) == "true") continue; // folder
            songList.append(list.at(0));       // file
        }

        if(!songList.isEmpty())
        {
            MEDIA_SOURCE_T mediaSource;
            switch(reqUID)
            {
            case REQUID_VIDEO_ID + MEDIA_SOURCE_JUKEBOX:    mediaSource = MEDIA_SOURCE_JUKEBOX; break;
            case REQUID_VIDEO_ID + MEDIA_SOURCE_USB1:       mediaSource = MEDIA_SOURCE_USB1;    break;
            case REQUID_VIDEO_ID + MEDIA_SOURCE_USB2:       mediaSource = MEDIA_SOURCE_USB2;    break;
            default:
                MP_LOG << "reqUID is invalid" << reqUID << LOG_ENDL;
                return;
            }

            AppMediaPlayer_Video_ControllerFS *pController = (AppMediaPlayer_Video_ControllerFS*)getController(mediaSource);
            if(pController) {
                pController->UpdateCurrFolderList(songList);
            }else{
                MP_LOG << "TASCAN -> " << "pController is NULL" << LOG_ENDL;
            }
        }
        break;
    }        
    default:
    {
        break;
    }
    }
	// { modified by ravikanth 20-04-13
    if(reqUID == (REQUID_VIDEO_ID + MEDIA_SOURCE_JUKEBOX))
    {
        m_pEngineVideo->ResetProgressLoading();
    }
	// } modified by ravikanth 20-04-13
}

void AppMediaPlayer_Video_Controller::HandleReqComplete(QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType, uint reqUID)
{
    MP_LOG << "TASCAN -> queryStr =" << MAP_QUERY_IDS.key(queryId) << ", queryId =" << queryId << ", reqUID = " << reqUID << LOG_ENDL;
    MP_LOG << "TASCAN -> fileList.count =" << fileList.count() << LOG_ENDL;
    
    switch(queryId) 
    {
    case eGetVideoLimitFileListDir_Id:
    {
        MP_LOG << "TASCAN -> eGetVideoLimitFileListDir_Id" << reqUID << LOG_ENDL;
        
        QStringList songList;

        foreach (QStringList file, fileList)
        {
            for (int i = 0; i < file.count(); i++)
            {
                songList.append(file[i]);
            }
        }

        MP_LOG << "TASCAN -> " << "songList:" << songList.size() << LOG_ENDL;

        // { modified by kihyung 2013.11.06 for ITS 0206863 
        MEDIA_SOURCE_T mediaSource;
        switch(volumeType)
        {
        case eJukeBox:      mediaSource = MEDIA_SOURCE_JUKEBOX; break;
        case eUSB_FRONT:    mediaSource = MEDIA_SOURCE_USB1;    break;
        case eUSB_REAR:     mediaSource = MEDIA_SOURCE_USB2;    break;
        default:
            MP_LOG << "volumeType is invalid" << volumeType << LOG_ENDL;
            return;
        }

        AppMediaPlayer_Video_ControllerFS *pController = (AppMediaPlayer_Video_ControllerFS*)getController(mediaSource);
        if(pController) {
            // modified by kihyung 2013.08.16 for ITS 0184724 
            bool bRescan = (reqUID == REQUID_RESCAN_ID + mediaSource) ? true : false; // added by kihyung 2013.08.25 for ITS 0185459 
            pController->UpdateAllPlaylistAfterScan(songList, bRescan);
        }else{
            MP_LOG << "TASCAN -> " << "pController is NULL" << LOG_ENDL;
        }
        // } added by kihyung 2013.11.06 for ITS 0206863 
        break;
    }
    
    default: 
    {
        break;
    }
    }
	// { modified by ravikanth 20-04-13
    if(volumeType == eJukeBox)
    {
        m_pEngineVideo->ResetProgressLoading();
    }
	// } modified by ravikanth 20-04-13
}

// { added by kihyung 2013.07.19 for ITS 0179774
void AppMediaPlayer_Video_Controller::ResetExtendDeviceInfo()
{
    if(m_usbDevicesHistory.isEmpty())
    {
        for(int i = 0; i < DEVICE_HISTORY_MAX_COUNT - 1; i++)
        {
            m_usbDevicesHistory.append(new AppMediaPlayer_Video_CModel_FS(this));
        }
    }

    for(int i = 0; i < m_usbDevicesHistory.count(); i++)
    {
        m_usbDevicesHistory[i]->initModel();
    }
}

QList<AppMediaPlayer_Video_CModel_FS*> AppMediaPlayer_Video_Controller::GetExtendDeviceHistory()
{
    QList<AppMediaPlayer_Video_CModel_FS*> retList;
    
    for (int i = 0; i < m_usbDevicesHistory.count(); i++)
    {
        retList.append(m_usbDevicesHistory[i]);
    }
    
    return retList;
}

void AppMediaPlayer_Video_Controller::CopyHistory(AppMediaPlayer_Video_CModel_FS *pDst, AppMediaPlayer_Video_CModel_FS *pSrc)
{
    MP_HIGH << LOG_ENDL;
    pDst->copyHistory(pSrc);
}

void AppMediaPlayer_Video_Controller::CopyHistoryFromUBSExtended(AppMediaPlayer_Video_CModel_FS *pDst, AppMediaPlayer_Video_CModel_FS *pSrc, QString mountPath)
{
    MP_HIGH << "from " << pSrc->deviceID() << mountPath << LOG_ENDL;
    pDst->copyHistoryFromExtended(pSrc, mountPath);
}

void AppMediaPlayer_Video_Controller::CopyHistoryToUSBExtended(AppMediaPlayer_Video_CModel_FS *pDst, AppMediaPlayer_Video_CModel_FS *pSrc)
{
    MP_HIGH << pSrc->deviceID() << "to USBExtended" << LOG_ENDL;
    pDst->copyHistoryToExtended(pSrc);
}
// } added by kihyung 2013.07.19 for ITS 0179774

/* This part of code is kindly implemented to help ExternalNotifier.
 * It shouldn't be here but this is the best place.
 * Well, life is full of paradoxes.
 */

/* Catching signals from Models */

/* Come here when File name is changed in FS model */
void AppMediaPlayer_Video_Controller::onFilenameChanged(QString filename)
{
    MP_LOG << "filename =" << filename << LOG_ENDL;
    if(!filename.isEmpty())
    {
        emit filenameChanged(filename);
    }
}

void AppMediaPlayer_Video_Controller::onFoldernameChanged(QString foldername)
{
    if(!foldername.isEmpty())
    {
        emit foldernameChanged(foldername);
    }
}

void AppMediaPlayer_Video_Controller::onFullFolderNameChanged(QString fullfolderename)
{
    if(!fullfolderename.isEmpty())
    {
        emit fullfoldernameChanged(fullfolderename);
    }
}

void AppMediaPlayer_Video_Controller::onPlaylistLengthChanged(int count)
{
    MP_LOG << count << LOG_ENDL;

    if(count > 0)
    {
        emit playlistLengthChanged(count);
    }

    if( QObject::sender() ==
        qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_JUKEBOX)->getModel()) )
    {
        emit contentSize(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_VIDEO, (count>0));
    }
    else if( QObject::sender() ==
             qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_USB1)->getModel()) )
    {
        emit contentSize(MEDIA_SOURCE_USB1, MEDIA_TYPE_VIDEO, (count>0));
    }
    else if( QObject::sender() ==
             qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_USB2)->getModel()) )
    {
        emit contentSize(MEDIA_SOURCE_USB2, MEDIA_TYPE_VIDEO, (count>0));
    }
}

void AppMediaPlayer_Video_Controller::onPlaylistPositionChanged(int count)
{
    if(count >= 0)
    {
        emit playlistPositionChanged(count);
    }
}

/* Come here when Chapter name is changed in DVD or VCD model */
void AppMediaPlayer_Video_Controller::onChapterChanged(int chapter)
{
    if(chapter > 0)
    {
        onFilenameChanged(QString::number(chapter));
    }
}

void AppMediaPlayer_Video_Controller::onTitleChanged(int title)
{
    MP_LOG << title << LOG_ENDL;
    if(title > 0)
    {
        //MP_LOG << QString::number(title) << LOG_ENDL;
        onFoldernameChanged(QString::number(title));
    }
}

// { remvoed by kihyung 2012.11.26. removed spec.
/*
// { added by Sergey Vetugov for CR#13601
void AppMediaPlayer_Video_Controller::onDVDScreenChanged(int state)
{
    MP_LOG << "state =" << state << LOG_ENDL;

    emit dvdScreenChanged(state);
}
// } added by Sergey Vetugov for CR#13601
*/
// } remvoed by kihyung 2012.11.26. removed spec.

// Connect CModels.
void AppMediaPlayer_Video_Controller::connectCModels()
{
    // FS File name.
    connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_JUKEBOX)->getModel()),
            SIGNAL(filenameChanged(QString)),
            this,
            SLOT(onFilenameChanged(QString)));

    connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_USB1)->getModel()),
            SIGNAL(filenameChanged(QString)),
            this,
            SLOT(onFilenameChanged(QString)));

    connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_USB2)->getModel()),
            SIGNAL(filenameChanged(QString)),
            this,
            SLOT(onFilenameChanged(QString)));

    // FS Folder name.
    connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_JUKEBOX)->getModel()),
            SIGNAL(foldernameChanged(QString)),
            this,
            SLOT(onFoldernameChanged(QString)));

    connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_USB1)->getModel()),
            SIGNAL(foldernameChanged(QString)),
            this,
            SLOT(onFoldernameChanged(QString)));

    connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_USB2)->getModel()),
            SIGNAL(foldernameChanged(QString)),
            this,
            SLOT(onFoldernameChanged(QString)));

    // FS Full folder name.
    connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_JUKEBOX)->getModel()),
            SIGNAL(fullfoldernameChanged(QString)),
            this,
            SLOT(onFullFolderNameChanged(QString)));

    connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_USB1)->getModel()),
            SIGNAL(fullfoldernameChanged(QString)),
            this,
            SLOT(onFullFolderNameChanged(QString)));

    connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_USB2)->getModel()),
            SIGNAL(fullfoldernameChanged(QString)),
            this,
            SLOT(onFullFolderNameChanged(QString)));

    // FS Tracks quantity.
    connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_JUKEBOX)->getModel()),
            SIGNAL(playlistLengthChanged(int)),
            this,
            SLOT(onPlaylistLengthChanged(int)));

    connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_USB1)->getModel()),
            SIGNAL(playlistLengthChanged(int)),
            this,
            SLOT(onPlaylistLengthChanged(int)));

    connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_USB2)->getModel()),
            SIGNAL(playlistLengthChanged(int)),
            this,
            SLOT(onPlaylistLengthChanged(int)));

    // FS Current track.
    connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_JUKEBOX)->getModel()),
            SIGNAL(playlistPositionChanged(int)),
            this,
            SLOT(onPlaylistPositionChanged(int)));

    connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_USB1)->getModel()),
            SIGNAL(playlistPositionChanged(int)),
            this,
            SLOT(onPlaylistPositionChanged(int)));

    connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(getController(MEDIA_SOURCE_USB2)->getModel()),
            SIGNAL(playlistPositionChanged(int)),
            this,
            SLOT(onPlaylistPositionChanged(int)));

    // DVD Current chapter.
    connect(qobject_cast<AppMediaPlayer_Video_CModel_DVD*>(getController(MEDIA_SOURCE_DVDVIDEO)->getModel()),
            SIGNAL(titleChanged(int)),
            this,
            SLOT(onTitleChanged(int)));

    connect(qobject_cast<AppMediaPlayer_Video_CModel_DVD*>(getController(MEDIA_SOURCE_DVDVIDEO)->getModel()),
            SIGNAL(chapterChanged(int)),
            this,
            SLOT(onChapterChanged(int)));

    //{ added by yongkyun.lee 20130626 for :  disc OSD
    connect(qobject_cast<AppMediaPlayer_Video_CModel_DVD*>(getController(MEDIA_SOURCE_DVDVIDEO)->getModel()),
            SIGNAL(trackCountChanged(int)),
            this,
            SLOT(onPlaylistLengthChanged(int)));
    //} added by yongkyun.lee 20130626 


    // { remvoed by kihyung 2012.11.26. removed spec.
    /*
    // { added by Sergey Vetugov for CR#13601
    connect(qobject_cast<AppMediaPlayer_Video_CModel_DVD*>(getController(MEDIA_SOURCE_DVDVIDEO)->getModel()),
            SIGNAL(playerScreenStateChanged(int)),
            this,
            SLOT(onDVDScreenChanged(int)));
    // } added by Sergey Vetugov for CR#13601
    */
    // } remvoed by kihyung 2012.11.26. removed spec.

    // VCD Current chapter.
    connect(qobject_cast<AppMediaPlayer_Video_CModel_VCD*>(getController(MEDIA_SOURCE_VCD)->getModel()),
            SIGNAL(currentTrackChanged(int)),
            this,
            SLOT(onChapterChanged(int)));

    // { added by kihyung 2012.07.17 for CR 8042
    connect(qobject_cast<AppMediaPlayer_Video_CModel_VCD*>(getController(MEDIA_SOURCE_VCD)->getModel()),
            SIGNAL(tuneWheelTrackChanged(int)),
            this,
            SLOT(onChapterChanged(int)));
    // } added by kihyung 2012.07.17
	
	// removed by Sergey for CR#15771
	
    //{ added by yongkyun.lee 20130626 for :  disc OSD
    connect(qobject_cast<AppMediaPlayer_Video_CModel_VCD*>(getController(MEDIA_SOURCE_VCD)->getModel()),
            SIGNAL(trackCountChanged(int)),
            this,
            SLOT(onPlaylistLengthChanged(int)));
    //} added by yongkyun.lee 20130626 
    
}


void AppMediaPlayer_Video_Controller::HandleAuxStateChanged(int nState)
{
    switch(nState)
    {
        case 1:
            emit onAuxStateChanged(PLAYER_PLAY);
            break;
        case 2:
            emit onAuxStateChanged(PLAYER_PAUSE);            
            break;
        case 0:
            emit onAuxStateChanged(PLAYER_STOP);            
            break;
        default:
            break;
    }
}

/** @brief Updates the size and position of the video widget.
 * If you recall the VideoWidgetProps structure, you will see that it specifies
 * the rectangle, where the video stream should fit into, and an aspect ration
 * enumeration. The supported aspect ratios for the momen are 16x9 and 4x3. The
 * full aspect ratio is synonim for the 16x9 which is exactly the aspect ratio
 * of the display.
 *
 * We have no restrictions on the size of the intended display width here. This
 * makes us to be responsible for aligning the widget inside the display area,
 * when the display rect aspect rate does not match the requested one.
 *
 * For the rest of the details, you will need to read the code.
 * */
void AppMediaPlayer_Video_Controller::updateAspectRatio( VideoWidgetProps *widgetProps )
{

    MP_LOG << "Aspect: " << (int)widgetProps->aspectRatio << LOG_ENDL;
    MP_LOG << "Requested position: x = " << widgetProps->displayX << ", y = " << widgetProps->displayY << LOG_ENDL;
    MP_LOG << "Requested size: width = " << widgetProps->displayWidth << ", height = " << widgetProps->displayHeight << LOG_ENDL;
    MP_LOG << "Request on front display: " << widgetProps->isFrontDisplay  << LOG_ENDL;

    // At this point we have no idea about proportions of the display width and
    // height we were requested to output video to. So we need to take into
    // account that actual video widget size might be limited by the display
    // width as well as by display height.
    QRect actualRect( widgetProps->displayX, widgetProps->displayY,
                      widgetProps->displayWidth, widgetProps->displayHeight );


    switch( widgetProps->aspectRatio )
    {
    // { added by jeeeun for PAR implementation 
    case ASPECT_RATIO_FULL:
    {
        //{ modified by cychoi 2013.12.04 for ITS 212759
        //{ added by yongkyun.lee 20130302 for : ISV 70577
        if(m_mediaSource == MEDIA_SOURCE_DVDVIDEO || m_mediaSource == MEDIA_SOURCE_VCD ||
           m_mediaSource == MEDIA_SOURCE_AUX1 || m_mediaSource == MEDIA_SOURCE_AUX2)
        {
             //m_tableAspectRatio.value(ASPECT_RATIO_FULL , QRect(0, 0, 1280, 720));
             updateAspectRatioTable(widgetProps->aspectRatio, QRect(0, 0, 1280, 720));
        }
        else
        //} added by yongkyun.lee 20130228 
        {
            actualRect = getRectByPAR(m_par, m_dar, m_actualRect.width(), m_actualRect.height());		
            updateAspectRatioTable(widgetProps->aspectRatio, actualRect);
        }
        //} modified by cychoi 2013.12.04
        break;
    }
	// } added by jeeeun for PAR implementation
    case ASPECT_RATIO_16_9:
    {
        if(m_mediaSource == MEDIA_SOURCE_JUKEBOX || m_mediaSource == MEDIA_SOURCE_USB1 ||
           m_mediaSource == MEDIA_SOURCE_USB2)
        {
            QString divxTemp = ".divx";
            QString * divx = NULL;
            divx = &divxTemp;

            if(widgetProps->aspectRatio == ASPECT_RATIO_16_9  && ((AppMediaPlayer_Video_CModel_FS*)m_pControllerCurrent->getModel())->playingUrl().endsWith(divx,Qt::CaseInsensitive))
            {
                actualRect = getRectByPAR(m_par, m_dar, m_actualRect.width(), m_actualRect.height());
                updateAspectRatioTable(ASPECT_RATIO_16_9, actualRect);
             break;
            }
        }
        if( (widgetProps->displayWidth * 1000 / 16) > (widgetProps->displayHeight * 1000 / 9) )
        {
            actualRect.setHeight( widgetProps->displayHeight );
            actualRect.setWidth( actualRect.height() / 9 * 16 );
        }
        else
        {
            actualRect.setWidth( widgetProps->displayWidth );
            actualRect.setHeight( actualRect.width() / 16 * 9 );
        }
        break;
    }

    case ASPECT_RATIO_4_3:
    {
        if ( (widgetProps->displayWidth * 1000 / 4) > (widgetProps->displayHeight * 1000 / 3) )
        {
            actualRect.setHeight( widgetProps->displayHeight );
            actualRect.setWidth( actualRect.height() / 3 * 4 );
        }
        else
        {
            actualRect.setWidth( widgetProps->displayWidth );
            actualRect.setHeight( actualRect.width() / 4 * 3 );
        }
        break;
    }

    default:
    {
        //Ignore
        break;
    }

    }

    // the following two should print true in both cases
    MP_LOG << "Assertion: width " << (actualRect.width() <= widgetProps->displayWidth) << LOG_ENDL;
    MP_LOG << "Assertion: height" << (actualRect.height() <= widgetProps->displayHeight) << LOG_ENDL;

    // If no alignment is required, (actual width) / 2 - displayWidth / 2 will
    // give the zero, so no alignment will happen. Same applies for the
    // vertical position.
    if ( actualRect.width() < widgetProps->displayWidth )
        actualRect.moveLeft(widgetProps->displayX +
                           ((widgetProps->displayWidth - actualRect.width()) / 2 ) );

    if ( actualRect.height() < widgetProps->displayHeight )
        actualRect.moveTop( widgetProps->displayY +
                          (widgetProps->displayHeight - actualRect.height() ) / 2);
	
    if(m_pEngineVideo->getCurrentScreen() == DISPLAY_REAR) // modified by Sergey 08.04.2013
    {
       //QRectangle magic: when you change the Y coordinate,
       // value of  height is changes  from 'height' to 'height-Y'
       actualRect.setHeight( actualRect.height() + 720 );
       actualRect.setTop( actualRect.y() + 720 );
    }

    MP_LOG << "Adjusting geometry to: "
           << actualRect.left() << actualRect.top()
           << actualRect.width() << actualRect.height() << LOG_ENDL;

    SetAspectRatio( widgetProps->aspectRatio );

    m_smallWidgetRect = actualRect; //added by Sergey for CR#16002
}
// { added by jeeeun for PAR implementation
QRect AppMediaPlayer_Video_Controller::getRectByPAR(float aPar, float aDar, int aWidth, int aHeight)
{
    QRect result = QRect(0,0,0,0);
    int bWidth = 0;
	int bHeight = 0;
	int px = 0;
	int py = 0;
    float tmpWidth = 0;
	float tmpHeight = 0;
	MP_LOG << " aPar : "<< aPar << "aDar : " << aDar << LOG_ENDL;
	
	if(aPar > 3.076 && aPar < 3.078)
    {
	    MP_LOG << " PAR : 40:33" << LOG_ENDL;
		aPar = 1.212;
		if(aHeight != 0)
        {
		      aDar = aPar * (float)aWidth/(float)aHeight;
        }
    }
	
	bWidth = aWidth;
    // width standard  
    // width / tmpHeight = DAR / 1
	// tmpHeight = width / DAR
	if(aDar != 0)
    {
	    bHeight = (int)(bWidth / aDar);
    }
    // in case of width = 1280
	// width : height = 1280 : x
	// x = height / width * 1280 
	tmpHeight = (float)bHeight / (float)bWidth;
	tmpHeight = tmpHeight * 1280;
	tmpWidth = 1280;

    // Height is more than 720
    // width : height = y : 720
	// y = width / height * 720
	// Dar = width / height
	// y = Dar * 720
	if(tmpHeight > 720)
    {	   
		tmpWidth = aDar * 720;
		tmpHeight = 720;
    }
    
	bWidth = (int)tmpWidth;
	bHeight = (int)tmpHeight;

    //black bar size 
    px = (1280 - bWidth)/2;
	py = (720 - bHeight)/2;
	MP_LOG << "bWidth : "<< bWidth << "bHeight : " << bHeight << LOG_ENDL;
	result = QRect(px,py,bWidth, bHeight);
    return result;
}
// } added by jeeeun for PAR implementation

// { added by kihyung 2013.2.14
void AppMediaPlayer_Video_Controller::requestVideoInfo()
{
    MP_LOG << LOG_ENDL;
    if(m_pIMedia) {
        m_pIMedia->SetSystemState(SYSTEM_CLEAR_DISPLAY_FG);
    }
}
// } added by kihyung 2013.2.14
//added by edo.lee 2013.04.21 for Mute
void
AppMediaPlayer_Video_Controller::onMuteTimerExpired()
{
    MP_LOG << LOG_ENDL;
    if (m_pMuteTimer->isActive())
    {
        m_pMuteTimer->stop();
    }
	GetEngineMain().setUnMuteState(false);
}
//added by edo.lee 2013.04.21

// { modified by Sergey for ITS#181872 30.07.2013
void AppMediaPlayer_Video_Controller::clearDisplay(bool aOn)
{   
    MP_HIGH << "on =" << aOn << "swap =" << m_pEngineVideo->getEngineMain().IsSwapDCEnabled() << LOG_ENDL; // modified by Sergey 23.12.2013 for ITS#216921

    if(m_pIMedia)
    {
        m_pIMedia->ClearDisplay(aOn);
        m_bClearDisplay = aOn; // added by Sergey 15.10.2013 for ITS#194932
    }
    else if(m_pGPlayerEngine) // no controller for AUX
    {
        m_pGPlayerEngine->ClearDisplay(aOn);
        m_bClearDisplay = aOn; // added by Sergey 15.10.2013 for ITS#194932
    }
    else
    {
        MP_LOG << "No access to the API." << LOG_ENDL;
    }
}
// } modified by Sergey for ITS#181872 30.07.2013


// { added by junam 2012.10.18 for multiple seek command
bool AppMediaPlayer_Video_Controller::isSeekMode(void)
{
    PLAYBACK_STATUS_T status = getPlaybackStatus();
    switch (status)
    {
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_4X:
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_16X:
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_20X:
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_4X:
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_16X:
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_20X:
        return true;
    default:
        break;
    }
    return false;
}
// } added by junam

// removed by Sergey for CR#15775

// { added by Sergey for CR#16002
void AppMediaPlayer_Video_Controller::showSmallWidget()
{
    SetVideoWidgetSize(m_smallWidgetRect.x(),
                       m_smallWidgetRect.y(),
                       m_smallWidgetRect.width(),
                       m_smallWidgetRect.height());
}
// } added by Sergey for CR#16002.

//{ added by Radhakrushna for CR#14111
// { modified by cychoi 2013.06.29 for ITS 175967 No tune wheel in PBC menu
bool AppMediaPlayer_Video_Controller::tuneWheel(bool direction)
//void AppMediaPlayer_Video_Controller::tuneWheel(bool direction)
{
    m_TuneInProgress = false;
    if(m_pControllerCurrent)
    {
        // { added by kihyung 2013.07.22 for ITS 0181137
        AppMediaPlayer_Video_CModel_Base *pCModel = m_pControllerCurrent->getModel();
        if(pCModel) {
            if(pCModel->loadingMode() == true && m_pEngineVideo->IsStopped() == true) { // modified by kihyung 2013.10.25 for ITS 0195855 
                MP_LOG << "pCModel->loadingMode()... return..." << LOG_ENDL;
                return m_TuneInProgress;
            }
        }
        // } added by kihyung 2013.07.22 for ITS 0181137
    
        m_TuneInProgress = m_pControllerCurrent->tuneWheel(direction);
    }

    return m_TuneInProgress;
}
// } modified by cychoi 2013.06.29

void AppMediaPlayer_Video_Controller::cancelTune()
{
    m_TuneInProgress = false;
    if(m_pControllerCurrent)
    {
        m_pControllerCurrent->cancelTune();
    }
}

bool AppMediaPlayer_Video_Controller::selectTuned()
{
    m_TuneInProgress = false;
    bool ret = false;
    if(m_pControllerCurrent)
    {
        ret = m_pControllerCurrent->selectTuned();
    }
    return ret;
}
//} added by Radhakrushna for CR#14111

// { added by Sergey 30.07.2013
// =================== DRM REGISTRATION ============================================

registrationState AppMediaPlayer_Video_Controller::drmRegistrationStatus()
{
    int temp;
    registrationState divxState = SETTINGS_DRM_NEVER_REGISTERED;

    /** IsDeviceActivated()
      *     Return unsigned int;
      *         DRM_SUCCESS = 0 : registered.
      *         DRM_NOT_REGISTERED = 2 : not registered. But the device is registered once.
      *         DRM_GENERAL_ERROR = 4 : error (Failure)
      *         DRM_NEVER_REGISTERED = 5 : Never registered.
      */
    temp = IsDeviceActivated();
    switch(temp)
    {
        case 0:
            divxState = SETTINGS_DRM_SUCCESS;
            MP_LOG << "DRM_SUCCESS" << LOG_ENDL;
            break;
        case 2:
            divxState = SETTINGS_DRM_NOT_REGISTERED;
            MP_LOG << "DRM_NOT_REGISTERED" << LOG_ENDL;
            break;
        case 4:
            divxState = SETTINGS_DRM_GENERAL_ERROR;
            MP_LOG << "DRM_GENERAL_ERROR" << LOG_ENDL;
            break;
        case 5:
            divxState = SETTINGS_DRM_NEVER_REGISTERED;
            MP_LOG << "DRM_NEVER_REGISTERED" << LOG_ENDL;
            break;
        default:
            MP_LOG << "unsupported state =" << temp << LOG_ENDL;
            divxState = SETTINGS_DRM_NEVER_REGISTERED;
            break;
    }

    return divxState;
}

void AppMediaPlayer_Video_Controller::recieveRegistrationCode()
{
    if(drmRegistrationStatus() == SETTINGS_DRM_SUCCESS)
    {
        MP_LOG << "Already registered. Return" << LOG_ENDL;
        return;
    }

    char regCode[11];

    if(GetRegistrationCode(regCode) == 0) // DRM_SUCCESS
    {
        MP_LOG << "DRM_SUCCESS regCode = " << regCode << LOG_ENDL;
        m_drmRegistrationCode = QString(regCode);
    }
    else // DRM_NOT_AUTHORIZED
    {
        MP_LOG << "Unable to register. Get error DRM_NOT_AUTHORIZED." << LOG_ENDL;
    }
}

void AppMediaPlayer_Video_Controller::recieveDeregistrationCode()
{
   //comment by edo.lee 2013.11.16 because does not get deregistration value when B+ off/on
   /* registrationState regState = drmRegistrationStatus();

    if(regState == SETTINGS_DRM_NOT_REGISTERED || regState == SETTINGS_DRM_NEVER_REGISTERED)
    {
        MP_LOG << "Already NOT registered. Return." << LOG_ENDL;
        return;
    }*/

    char deRegCode[11];

    if(GetDeregistrationCode(deRegCode) == 0) // "DRM_SUCCESS"
    {
        MP_LOG << "DRM_SUCCESS deregCode = " << deRegCode << LOG_ENDL;
        m_drmDeregistrationCode = QString(deRegCode);
    }
    else  // DRM_NOT_ATHORIZED
    {
        MP_LOG << "Unable to Deregister. Get error DRM_NOT_AUTHORIZED." << LOG_ENDL;
    }
}

// =================== END OF DRM REGISTRATION ============================================
// } added by Sergey 30.07.2013


// { added by Sergey 11.08.2013
bool AppMediaPlayer_Video_Controller::isFSSource(MEDIA_SOURCE_T source)
{
    bool result = false;

    if(source == MEDIA_SOURCE_JUKEBOX || source == MEDIA_SOURCE_USB1 || source == MEDIA_SOURCE_USB2)
        result = true;

    return result;
};


void AppMediaPlayer_Video_Controller::playFromFileManager()
{
    if(isFSSource(m_mediaSource))
    {
        AppMediaPlayer_Video_ControllerFS *pFSCtrl = dynamic_cast<AppMediaPlayer_Video_ControllerFS*>(getCurrentController());

        if(pFSCtrl)
            pFSCtrl->playFromFileManager();
    }
}
// } modified by Sergey 11.08.2013


// { added by Sergey 15.10.2013 for ITS#194932
void AppMediaPlayer_Video_Controller::onOverplaneChanged()
{
    clearDisplay(m_bClearDisplay);
}
// } added by Sergey 15.10.2013 for ITS#194932

// { added by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
bool AppMediaPlayer_Video_Controller::IsFFRewState()
{
    AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T ePlaybackStatus = m_pControllerCurrent->getModel()->playbackStatus();

    switch( ePlaybackStatus )
    {
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_4X:
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_16X:
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_20X:
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_4X:
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_16X:
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_20X:
        return true;
    default : // make compiler happy
        return false;
    }
    return false;
}
// } added by cychoi 20103.10.16

// { added by Sergey 17.10.2013 for ITS#195182
void AppMediaPlayer_Video_Controller::updateCaptionSizeInModels(AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_T nFontSize)
{
    m_pJBCtrl->getFSModel()->setCaptionSize(nFontSize);
    m_pUSB1Ctrl->getFSModel()->setCaptionSize(nFontSize);
    m_pUSB2Ctrl->getFSModel()->setCaptionSize(nFontSize);
}


void AppMediaPlayer_Video_Controller::updateEnableSubtitleInModels(bool bOn)
{
    m_pJBCtrl->getFSModel()->setCaptionEnable(bOn);
    m_pUSB1Ctrl->getFSModel()->setCaptionEnable(bOn);
    m_pUSB2Ctrl->getFSModel()->setCaptionEnable(bOn);
}
// } added by Sergey 17.10.2013 for ITS#195182


// { added by Sergey 24.10.2013 for ITS#87134
void AppMediaPlayer_Video_Controller::setBackground(bool on)
{
    if(m_pControllerCurrent && (m_mediaSource == MEDIA_SOURCE_USB1 || m_mediaSource == MEDIA_SOURCE_USB2 || m_mediaSource == MEDIA_SOURCE_JUKEBOX))
    {
        m_pControllerCurrent->getModel()->setRemovingMode(on);
    }
}
// } added by Sergey 24.10.2013 for ITS#87134

// { added by Sergey 25.10.2013 for new DivX
void AppMediaPlayer_Video_Controller::setDivxScreenMode(DIVX_STATE mode)
{
    MP_HIGH << mode << LOG_ENDL;
    
    m_divxScreenMode = mode;
    AppMediaPlayer_SettingsStorage::GetInstance()->SaveVideoSettings(this);
}


void AppMediaPlayer_Video_Controller::loadDivxScreenMode(DIVX_STATE mode)
{
    MP_HIGH << mode << LOG_ENDL;
    
    if(mode >= DIVX_STATE_REGISTRATION_CODE && mode < DIVX_STATE_MAX)
        m_divxScreenMode = mode;
    else
        m_divxScreenMode = DIVX_STATE_REGISTRATION_CODE;
}
// } added by Sergey 25.10.2013 for new DivX
