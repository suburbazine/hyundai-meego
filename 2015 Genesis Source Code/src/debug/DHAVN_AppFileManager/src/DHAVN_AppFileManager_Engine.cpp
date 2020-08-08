#include <DHAVN_AppFileManager_Engine.h>
#include <DHAVN_AppFramework_Event_Def.h>
#include <uistate_manager_def.h>
#include "DHAVN_AppFileManager_CStateManager.h"
#include <DHAVN_AppFileManager_Shared.h>
#include "DHAVN_AppSettings_Shared.h"
#include "DHAVN_AppMediaPlayer_Shared.h"
#include <QDebug>
#include <QString>
// removed by Dmitry 17.08.13
#include "DHAVN_AppFileManager_UI_main.h" // added by Sergey for CR#15468
#include <MediaInfo/MediaInfo.h> // added by eugene.seo 2013.03.09
#include <signal.h>
#include <QmlStatusBar/osdwriter.h>
#include "DHAVN_AppFileManager_Thumbnailer.h" //2014.05.23

FileManagerEngine* FileManagerEngine::m_pInstance = NULL;

// { added by eugene.seo 2013.03.09
#define PHOTO_MAX_SUPPORTED_SIZE 5 //mega bytes
#define PHOTO_MAX_SUPPORTED_WIDTH 4000
#define PHOTO_MAX_SUPPORTED_HEIGHT 3000
// } added by eugene.seo 2013.03.09


/* Test mapping H/W keys */
struct XKEYS_MAP_T
{
    // Key string
    int keyCode;
    // Display to handle key
    CommonDef::FILEMANAGER_DISPLAY_T display;
    // Actual hardware key
    EVT_PUBLIC_ID_T event;
};


// Mapping table for keybord key events to actual key
XKEYS_MAP_T XKEYS_MAP[] =
{
   /** Head unit (Front panel) keys */
   { Qt::Key_Backspace,  CommonDef::FILEMANAGER_DISPLAY_FRONT,   EVT_KEY_HU_BACK     },

   /** CCP keys */
   { Qt::Key_J,          CommonDef::FILEMANAGER_DISPLAY_FRONT,   EVT_KEY_CCP_BACK    },

   /** RRC keys */
   { Qt::Key_Comma,      CommonDef::FILEMANAGER_DISPLAY_REAR,    EVT_KEY_RRC_BACK    },

   { Qt::Key_L,      CommonDef::FILEMANAGER_DISPLAY_FRONT,    EVT_KEY_CCP_MENU    },
   { Qt::Key_Slash,      CommonDef::FILEMANAGER_DISPLAY_REAR,    EVT_KEY_RRC_MENU    },
   { Qt::Key_I,      CommonDef::FILEMANAGER_DISPLAY_NONE,    EVT_KEY_HU_MENU    },

   { Qt::Key_unknown,    CommonDef::FILEMANAGER_DISPLAY_NONE,    EVT_MAX            }
};
/* End test mapping H/W keys */


const QString KFrontDisplayId = QString(":0.0");
const QString KRearDisplayId = QString(":0.1");

// removed by kihyung 2013.12.09 for ITS 0213549 
//const QString USBFront = QString("/media/front_usb"); // modified by Dmitry 22.05.13
//const QString USBRear = QString("/media/rear_usb"); // modified by Dmitry 22.05.13

FileManagerEngine::FileManagerEngine( int &argc, char **argv,
                                     LANGUAGE_T nLanguage, ECountryVariant cv, QObject *pParent )
: AppVextEngineQML( true, "AppFileManager", pParent ),//changed by Alexey Edelev 2012.09.28. CR14010
  //appId(APP_NONE),//modified by edo.lee 2013.05.27
  appId_front(APP_NONE),
  appId_rear(APP_NONE),
  m_pTranslator(NULL),
  m_nLanguage(nLanguage),
  m_isUsbAttached(false),
  m_nDisplay( CommonDef::FILEMANAGER_DISPLAY_FRONT ),
  m_variantCounry(cv),
  m_currentStateTraking(eInvalidState),
  m_IsFrontVisible( false ),
  m_IsRearVisible( false ),
  //added by edo.lee 2013.06.11
  //currendDeviceIsJuke(true),
  currendDeviceIsJuke_front(false), //added by edo.lee 2013.06.18
  currendDeviceIsJuke_rear(false),//added by edo.lee 2013.06.18
  //added by edo.lee 2013.06.11
  m_isMove(true)// add by eunhye 2012.09.21 for CR 9243
  ,m_bScrollingTicker( true) //added by aettie for ticker stop when DRS on
  ,m_pUSBController(NULL) // added by kihyung 2013.07.08 for ITS 0178816
  ,m_popupType(false) // add by eunhye 2013.02.06 for ISV70222/70319, ITS150138/149862
  ,m_pTrackerAbs(NULL) // modified by Dmitry Bykov 04.04.2013
  ,m_pExtractor(new CTrackerExtractorClient) // added by Dmitry 14.08.13 for ITS0183845
  ,m_bfileCopyOperationStatus(false) // modified by ravikanth 16-04-13 
  ,m_bMiddleEast(false) // added by Dmitry 03.05.13
  ,m_bVideoPlaying(false) // added by Dmitry 12.07.13 for ISV85006
  ,copyFromMainPlayer(false) //[KOR][ITS][179014][comment](aettie.ji)
  ,m_isFrontLCD(true) // added by Sergey 02.08.2103 for ITS#181512
  ,m_bisPowerOff(false) // added by Dmitry 10.10.13 for ITS0194726
// modified by Dmitry 03.10.13 for ITS0187937
  ,m_pSharedVideoData(new SharedDataProvider(VIDEOPLAYER_SHARED_MEMORY_KEY))
  ,m_pSharedPictureData(new SharedDataProvider(PHOTOVIEWER_SHARED_MEMORY_KEY))
  ,m_pSMFront(NULL) // modified by Dmitry 09.10.13
  ,m_pSMRear(NULL) // modified by Dmitry 09.10.13
  ,m_jogDirectionKeyPressed(0)
  ,m_jogDirectionPressedStatus(false)
  ,m_isFrontVideoType(false) //added by hyejin 20141210 for ITS#0254279
  ,m_isRearVideoType(false) //added by hyejin 20141210 for ITS#0254279
{
   // FM_LOG_INIT_V2("AppFileManager");
   for( int i = 0; i < argc; i++ )
   {
      if( QString(( argv[i] )).contains(KRearDisplayId) )
      {
         /** Application launches at rear display */
         m_nDisplay = CommonDef::FILEMANAGER_DISPLAY_REAR;
      }
   }

   memset(&this->frontScreenData, 0, sizeof(LocalScreenData));
   memset(&this->rearScreenData, 0, sizeof(LocalScreenData));
   setMiddleEast(GetLanguage() == LANGUAGE_AR); // modified by Dmitry 11.05.13

   this->frontScreenData.m_isBgState = true;
   this->rearScreenData.m_isBgState = true;

   m_pDeletedFiles = (DeletedFiles*)malloc(sizeof(DeletedFiles)); // modified by eugene 2012.08.27 for static analysis defect #6893
   m_bUSB1Attached = false;
   m_bUSB2Attached = false;
   m_bUSB1TAReadComplete = false;
   m_bUSB2TAReadComplete = false;   
   m_bJukeboxTAReadComplete = false;

   // added by kihyung 2013.12.09 for ITS 0213549 
   m_strUSB1Path.clear(); 
   m_strUSB2Path.clear();

   m_eCloneStatus = CLONE_OFF;

   setSignalHandlerObject(this);
   signalConnection();
   m_filePath[CommonDef::FILEMANAGER_DISPLAY_FRONT] = ""; // front
   m_filePath[CommonDef::FILEMANAGER_DISPLAY_REAR] = ""; // rear
}

FileManagerEngine::~FileManagerEngine()
{
    if (NULL != m_pTranslator)
    {
       delete m_pTranslator;
    }

    if (m_pExtractor)// added by Dmitry 14.08.13 for ITS0183845
       delete m_pExtractor; // added by Dmitry 14.08.13 for ITS0183845
    
    free(m_pDeletedFiles); 
}

// added by Dmitry Bykov 04.04.2013
CTrackerAbstractor* FileManagerEngine::getTrackerInstance()
{
   if (!m_pTrackerAbs)
   {
      m_pTrackerAbs = new CTrackerAbstractor();
      if(m_pTrackerAbs)
      {
          m_pTrackerAbs->SetFileSystemScanMode(false);
          connect(m_pTrackerAbs, SIGNAL(ReadCompleted(ETrackerAbstarctor_VolumeType, int)), this, SLOT(TAReadCompleted(ETrackerAbstarctor_VolumeType, int)));
          m_pTrackerAbs->SetCountryVariant(AppEngineBase::GetCountryVariant()); // added by Dmitry 28.05.13
          m_pTrackerAbs->SetAVNSWVersion(GetSWVersion());
      }
   }
   return m_pTrackerAbs;
}
// added by Dmitry Bykov 04.04.2013

QString FileManagerEngine::GetSWVersion()
{
    if(m_strSWVersion.isEmpty() == true) 
    {
        QFile inFile("/etc/version.txt");
        if(inFile.open(QIODevice::ReadOnly) == false) {
            MP_LOG << "file not open" << LOG_ENDL;
            return ""; // modified by Dmitry 17.08.13
        }

        QTextStream inFileStream(&inFile);
        QString     strVersion;
        
        inFileStream.skipWhiteSpace();
        inFileStream >> strVersion;

        inFile.close();

        if(strVersion.isEmpty() == true) {
            m_strSWVersion = "unknown";
        }
        else {
            m_strSWVersion = strVersion;
        }
    }

    MP_LOG << m_strSWVersion << LOG_ENDL;
    
    return m_strSWVersion;
}

void FileManagerEngine::signalHandelr(int signum)
{
    MP_LOG << "Signum = " << signum << LOG_ENDL;
    m_pInstance->restart(signum);
    raise(signum);
}

void FileManagerEngine::setSignalHandlerObject(FileManagerEngine *m)
{
    m_pInstance = m;
}

// Watch Dog
void FileManagerEngine::signalConnection()
{
    struct sigaction act;
    act.sa_flags    = 0;
    act.sa_flags   |= SA_RESETHAND; //SA_RESTART; modified  by Hyejin.Noh 2014.03.28 for ITS 0231401
    act.sa_handler  = FileManagerEngine::signalHandelr;
    sigemptyset(&act.sa_mask);  //added by aettie 2013.04.03 for static defect 20375


    for( int signumber = SIGHUP ; signumber<SIGUNUSED ; signumber++)
    {
        if( signumber == SIGKILL || signumber == SIGSTOP || signumber == SIGCHLD ) continue; //modified  by Hyejin.Noh 2014.03.28 for ITS 0231401
        if(sigaction(signumber, &act, (struct sigaction *)NULL)<0)
        {
            GetLogger().Log(QString("Sigaction failnum = %1").arg(signumber));
            exit(1);
        }
    }
}

void FileManagerEngine::restart(int signum)
{
    Event event( EVT_REQUEST_ABNORMAL_TERMINATION, GetThisAppId(), APP_UI_MANAGER, (EVT_COMMAND_T)signum);//DISPLAY_FRONT); modified  by Hyejin.Noh 2014.03.28 for ITS 0231401
    if(!NotifyUISH(event, DISPLAY_FRONT))
    {
        MP_LOG << "FileManagerEngine::restart: Event notification failed" << LOG_ENDL;
    }
    //exit(0); removed  by Hyejin.Noh 2014.03.28 for ITS 0231401
}

void FileManagerEngine::EventFromUIStateManager( Event & aEvent, bool isFront){
    GetLogger().Log( QString( "FileManagerEngine::EventFromUIStateManager(%2) Start with [%1]").arg( aEvent.GetEventId() ).arg( (isFront?"Front":"Rear")) );
    if ( (aEvent.GetTarget() != GetThisAppId()) && (aEvent.GetTarget() != APP_ALL) )
    {
        return;
    }

    AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T screenGeometry = (isFront ? AppEngineQMLConstants::SCREEN_FRONT : AppEngineQMLConstants::SCREEN_REAR);
    DISPLAY_T display = (isFront ? DISPLAY_FRONT : DISPLAY_REAR);

    switch ( aEvent.GetEventId())
    {
        case EVT_REQUEST_FG:
        {
//            Event evt(EVT_RESPONSE_FG, APP_AVP, APP_UI_MANAGER);
//            NotifyUISH(evt, display);
            break;
        }
        case EVT_REQUEST_PRE_FG:
        {
           if ( !aEvent.Data().isEmpty() )
           {
              if (aEvent.GetCommand())
              {
                 APP_ENTRY_POINT_DATA_T *pEventData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();

                 if ( pEventData )
                 {
                    MP_LOG << "ZZZ pEventData->sAppEntryPoint = " << pEventData->sAppEntryPoint << LOG_ENDL;
                    APP_ID_T appid = APP_AVP;
                    CStateManager * stateManager = (isFront? m_pSMFront : m_pSMRear);
                    AppFileManagerStartParameter fmParams;
                    fmParams.copyState = false;
                    fmParams.isCopyFileUnsupported = false;
                    fmParams.startState = eStartInvalidSState;
                    QString playingFile("");

                    bool bUpdate = false;
                    bool clearAvOSD = false;

                    switch (pEventData->sAppEntryPoint)
                    {
                       case eAPP_FILEMANAGER_JUKEBOX_VIDEO:
                           bUpdate = (stateManager->GetCurrentState() != eJukeVideo);
                           fmParams.startState = bUpdate ? eStartJukeVideo : eVideoFileChanged;
                           playingFile = m_pSharedVideoData->getStringData(STRING_DATA_TYPE_PLAYING_FILE);
                           clearAvOSD = true;
                           break;
                       case eAPP_FILEMANAGER_JUKEBOX_PHOTO:
                           bUpdate = (stateManager->GetCurrentState() != eJukePictureShow);
                           fmParams.startState = eStartJukePicture;
                           playingFile = m_pSharedPictureData->getStringData(STRING_DATA_TYPE_PLAYING_FILE);
                           break;
                       case eAPP_FILEMANAGER_USB1_VIDEO:
                       case eAPP_FILEMANAGER_USB2_VIDEO:
                           playingFile = m_pSharedVideoData->getStringData(STRING_DATA_TYPE_PLAYING_FILE);
                           // modified by Dmitry 03.11.13 for ITS0206227
                           bUpdate = (stateManager->GetCurrentState() != eUsbVideo && stateManager->GetCurrentState() != eJukeVideoCopy) ||
                                    ((stateManager->GetCurrentState() == eUsbVideo) &&
                                    ((playingFile.contains("front_usb") && stateManager->getCurrentPath().contains("rear_usb")) ||
                                     (playingFile.contains("rear_usb") && stateManager->getCurrentPath().contains("front_usb"))));
                           fmParams.startState = bUpdate ? eStartUsbVideo : eVideoFileChanged;
                           clearAvOSD = true;
                           break;
                       case eAPP_FILEMANAGER_USB1_PHOTO:
                       case eAPP_FILEMANAGER_USB2_PHOTO:
                           //playingFile = m_pSharedPictureData->getStringData(STRING_DATA_TYPE_PLAYING_FILE); // modified for ITS 0217689
                           if(pEventData->sAppEntryPoint == eAPP_FILEMANAGER_USB1_PHOTO)
                               playingFile = m_pSharedPictureData->getStringData(STRING_DATA_TYPE_PLAYING_FILE_USB1);
                           else
                               playingFile = m_pSharedPictureData->getStringData(STRING_DATA_TYPE_PLAYING_FILE_USB2);
                           // modified by Dmitry 03.11.13 for ITS0206227
                           bUpdate = (stateManager->GetCurrentState() != eUsbPictureShow && stateManager->GetCurrentState() != eJukePictureCopy) ||
                                    ((stateManager->GetCurrentState() == eUsbPictureShow) &&
                                    ((playingFile.contains("front_usb") && stateManager->getCurrentPath().contains("rear_usb")) ||
                                     (playingFile.contains("rear_usb") && stateManager->getCurrentPath().contains("front_usb"))));
                           fmParams.startState = eStartUsbPicture;
                           break;
// modified by Dmitry 25.10.13 for ITS0198269
                       case eAPP_FILEMANAGER_SETTINGS:
                           fmParams.startState = eStartJukeListPicture;
                           appid = APP_SETTINGS;
                           break;
                       default:
                          break;
                    }

                    // modified for ITS 0210817
                    if(fmParams.startState == eVideoFileChanged)
                    {
                        QString currentPlayFile = stateManager->GetCurrentPlayFolder().append("/").append(stateManager->GetCurrentPlayName());
                        if(currentPlayFile != playingFile)
                        {
                            bUpdate = true;
                        }
                    }

                    if (bUpdate)
                    {
                       MP_LOG << "SHM playingFile = " << playingFile << LOG_ENDL;
                       snprintf(fmParams.currentPlayFileName, sizeof(fmParams.currentPlayFileName), playingFile.mid(playingFile.lastIndexOf("/") + 1).toUtf8().constData());
                       playingFile.truncate(playingFile.lastIndexOf("/"));
                       snprintf(fmParams.folderPath, sizeof(fmParams.folderPath), playingFile.toUtf8().constData());

                       Event launch( EVT_POST_EVENT,
                                    appid,
                                    APP_FMGR,
                                    EVT_FM_LAUNCH_STATE,
                                    QByteArray((char *)&fmParams, sizeof(fmParams)));

                       //emit NotifyUISH(launch ,  display);
                       EventHandlerPostEvent(launch, isFront);  // added by Michael.Kim 2014.06.27 for Copy List Issue in smoke test

                       //{ modified for ITS 0210817
                       if(fmParams.startState == eVideoFileChanged)
                       {
                           emit showContent( screenGeometry, true);
                       }
                       //} modified for ITS 0210817
                    }
                    else
                       emit showContent( screenGeometry, true);

                    if ( clearAvOSD ) {
                        if ( isFront)  OSDWriter::SendOSD("@OSD:F:CLEAR_AV");
                        else            OSDWriter::SendOSD("@OSD:R:CLEAR_AV");
                    }

                    //{added for ITS 231147 2014.03.26
                    MP_LOG << "bTemporalMode =" << pEventData->sHeader.bTemporalMode << LOG_ENDL;
                    if(!(pEventData->sHeader.bTemporalMode))
                    {
                        emit closePopupOnFG();
                    }
                    //}added for ITS 231147 2014.03.26
                  }
               }
               // modified by Dmitry 15.10.13 for ITS0195265 ITS0195628
               else
               {
                  emit hideMenu( screenGeometry );
                  emit hideOnFg( screenGeometry );
               }
            }

            LocalScreenData * localScreenData = (isFront ? &frontScreenData : &rearScreenData); 
            emit loadMainQml(screenGeometry); // modified by Sergey 25.04.2013
            emit showBottomBarOnCopy(); // modified by ravikanth 06-04-13
            localScreenData->m_isBgState = false;
            setDisplayVisible( display, true ); 
            emit signalSetDisplay( (int)getDisplayVisible() );
            emit defaultFocus(); //added by junam 2013.04.02 for Not moving jog

            APP_EVENT_DATA_BASE_T *pEventData = (APP_EVENT_DATA_BASE_T *)aEvent.Data().data();

            if( pEventData->sIsJogEnabled )
                emit focusReceived( pEventData->sIsJogEnabled );

            // removed by Dmitry 13.11.13

            break;
        }

        case EVT_REQUEST_BG:
        {

            LocalScreenData * localScreenData = (isFront ? &frontScreenData : &rearScreenData); 
            localScreenData->m_isBgState = true;
            setDisplayVisible( display, false ); 
            emit signalSetDisplay( (int)getDisplayVisible() );
            emit fileSelectedReset(); // added by Michael.Kim 2014.11.14 for ITS 252495
            clearALLJogKey();

            if( isFront && m_isFrontVideoType)
            {
                Event postevent( EVT_POST_EVENT, GetThisAppId(), APP_AVP, EVT_FM_VIDEO_HIDE );
                sendEventToUISH(postevent, m_IsFrontVisible);
                m_isFrontVideoType = false;
            }
            else if ( !isFront && m_isRearVideoType )
            {
                Event postevent( EVT_POST_EVENT, GetThisAppId(), APP_AVP, EVT_FM_VIDEO_HIDE );
                sendEventToUISH(postevent, m_IsFrontVisible);
                m_isRearVideoType = false;
            }
            
            localScreenData->m_isListFromMP = false;
            localScreenData->m_showFocusOnFg = false;

            // ITS 0194416
            emit showContent(screenGeometry, false); 

            if( isFront && (m_isFrontLCD || m_bisPowerOff)) { // ITS 0194726
                emit hideMenu(screenGeometry); // ITS 0194416
            }
            else if ( !isFront && IsSwapDCEnabled()){
                if ( m_isFrontLCD || m_bisPowerOff)
                    emit hideMenu(screenGeometry); // ITS 0194416
            }

            //{added for ITS 232574 2014.03.28
            setJogKeyReleased(CCP_JOG_CENTER_PRESSED);
            signalJogNavigation( AppEngineQMLConstants::JOG_CENTER, AppEngineQMLConstants::KEY_STATUS_CANCELED );
            //}added for ITS 232574 2014.03.28
            break;
        }

        // modified by Michael.Kim 2014.06.27 for Copy List Issue in smoke test
         case EVT_POST_EVENT:
         {
             EventHandlerPostEvent(aEvent, isFront);
             break;
         }
         // modified by Michael.Kim 2014.06.27 for Copy List Issue in smoke test

        #if (USE_AFW_SWAP_DC == 0)
        case EVT_KEY_SOFT_MENU:
        {
            emit keyEvent( (int)CommonDef::FILEMANAGER_EVT_KEY_MENU, (int)CommonDef::FILEMANAGER_EVT_KEY_STATUS_PRESSED, screenGeometry);
            break;
        }
        #endif

        case EVT_REQUEST_APP_EXIT:
        {
            GetLogger().Log( "FileManagerEngine::NotifyFromUIStateManager: Processing Event::EVT_REQUEST_APP_EXIT" );
            qApp->quit();
            break;
        }

// removed by Dmitry 16.08.13 for ITS0184683

        default:
        {
            #if USE_AFW_SWAP_DC
            NotifyFromUIStateManagerCommon( aEvent );
            #endif
            break;
        }
    }
}

// modified by Michael.Kim 2014.06.27 for Copy List Issue in smoke test
void
FileManagerEngine::EventHandlerPostEvent(Event & aEvent, bool isFront)
{
    MP_CRITICAL << "EVT_POST_EVENT source = " << aEvent.GetSource() << LOG_ENDL;
    AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T screenGeometry = (isFront ? AppEngineQMLConstants::SCREEN_FRONT : AppEngineQMLConstants::SCREEN_REAR);
    if (aEvent.GetCommand() == EVT_FM_LAUNCH_STATE &&
       (aEvent.GetSource() == APP_AVP || aEvent.GetSource() == APP_SETTINGS))
    {
        bool * isVideoType = (isFront ? &m_isFrontVideoType : &m_isRearVideoType);
        getTrackerInstance();

        *isVideoType = false;

        int state = eStartInvalidSState;
        bool shouldStateChanged = true;

        AppFileManagerStartParameter eventData = {eStartInvalidSState, {0}, {0}, false, false}; //fix coverity issue  // modified by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list

        EAppFileManagerStartState startState;
        if (sizeof(AppFileManagerStartParameter) == aEvent.Data().size())
        {
           memcpy(&eventData, aEvent.Data().data(), aEvent.Data().size());
           startState = eventData.startState;

           switch (startState)
           {
              case eStartUsbMusicTitle:
                 state = eUsbMusicTitle;
                 break;
              case eStartUsbMusicArtist:
                 state = eUsbMusicArtist;
                 break;
              case eStartUsbMusicAlbum:
                 state = eUsbMusicAlbom;
                 break;
              case eStartUsbVideo:
                 state = eUsbVideo;
                 *isVideoType = true;
                 break;
              case eStartUsbPicture:
                 state = eUsbPictureShow;
                 break;
              case eSatrtJukeMusicTitle:
                 state = eJukeMusicTitle;
                 break;
              case eStartJukeMusicArtist:
                 state = eJukeMusicArtist;
                 break;
              case eStartJukeMusicAlbum:
                 state = eJukeMusicAlbom;
                 break;
              case eStartJukeVideo:
                 state = eJukeVideo;
                 *isVideoType = true;
                 break;
              case eStartJukePicture:
                 state = eJukePictureShow;
                 break;
              case eStartJukeListVideo:
                 state = eJukeListVideo;
                 break;
              case eStartUsbListVideo:
                 state = eUsbListVideo;
                 break;
              case eStartJukeListPicture:
                 state = eJukeListPicture;
                 break;
              case eStartUsbListPicture:
                 state = eUsbListPicture;
                 break;
              case eVideoFileChanged:
              case eVideoFileAutoChanged: //added by hyejin 20141210 for ITS#0254279
                 shouldStateChanged = false;
                 *isVideoType = true;
                 break;
              default:
                 shouldStateChanged = false;
           }

           if (shouldStateChanged == true)
           {
               appId_front = aEvent.GetSource();

               QString currentPlayFileName = QString::fromUtf8(eventData.currentPlayFileName);
               QString folderPath = QString::fromUtf8(eventData.folderPath);
               if (aEvent.GetSource() == APP_SETTINGS)
                   eventData.copyState = false;

               if(!eventData.copyState || !(state == eUsbPictureShow)) // modified for ITS 0209355
               {
                    eventData.isCopyFileUnsupported = false;
               }

               emit showCopy(!eventData.copyState, eventData.isCopyFileUnsupported);

               // ITS 0171117
               if(state == eJukeListPicture && aEvent.GetSource() == APP_SETTINGS) {
                   MP_LOG <<"FROM SETTING..." << LOG_ENDL;
                   folderPath = "/usr_data/media/Picture"; // modified by ravikanth 10.06.13 fif for Waiting Display > find image > show empty
               }

               emit postEventToStart(state,folderPath,currentPlayFileName, screenGeometry);
           }
           else if (eventData.startState == eAudioFileChanged || eventData.startState == eVideoFileChanged || eventData.startState == eVideoFileAutoChanged)
           {
               QString currentPlayFileName(QString::fromUtf8(eventData.currentPlayFileName));
               QString folderPath(QString::fromUtf8(eventData.folderPath));

               emit resetFMLaunchBrowseState(screenGeometry);
               emit postEventPlayFileChanged(folderPath.append("/").append(currentPlayFileName),
                                             eventData.startState,
                                             screenGeometry);
           }
        }
    }
    // ITS0187937
    else if(aEvent.GetCommand() == EVT_FILEOPERATION_STOP)
    {
        GetLogger().Log("FileManagerEngine::NotifyFromUIStateManager EVT_POST_EVENT EVT_FILEOPERATION_STOP ");
        if(aEvent.GetSource() == APP_AVP) // modified by ravikanth 16-04-13
        {
            if ( !aEvent.Data().isEmpty() ) // modified for ISV 93629
            {
                // modified for ITS 0214738
                AppFileManagerFileOperationParameter eventData = {false, eStartInvalidSState};

                EAppFileManagerStartState startState;
                if (sizeof(AppFileManagerFileOperationParameter) == aEvent.Data().size())
                {
                    memcpy(&eventData, aEvent.Data().data(), aEvent.Data().size());
                    startState = eventData.mediaType;
                    //bool isFront = eventData.isFront;;
                    setOSDDisplay(eventData.isFront);
                    emit setFileOperationStopMedia(int(startState), eventData.isFront); // modified for ITS 0210799
                }
            }
            emit cancelFileOperation();
            emit setFileOperationStopMedia(int(eStartInvalidSState),true);
        }
        emit closeCopyCancelConfirmPopup(); // modified for ITS 0217509
    }
    else if(aEvent.GetCommand() == EVT_JUKEBOX_COPY_START)
    {
        GetLogger().Log("FileManagerEngine::NotifyFromUIStateManager EVT_POST_EVENT EVT_JUKEBOX_COPY_START ");
        if(aEvent.GetSource() == APP_AVP)
            m_bfileCopyOperationStatus = true;
    }
    else if(aEvent.GetCommand() == EVT_JUKEBOX_COPY_END)
    {
        GetLogger().Log("FileManagerEngine::NotifyFromUIStateManager EVT_POST_EVENT EVT_JUKEBOX_COPY_END ");
        if(aEvent.GetSource() == APP_AVP)
            m_bfileCopyOperationStatus = false;
        emit closeCopyCancelConfirmPopup(); // modified for ITS 0217509
    }
    // } modified by ravikanth 16-04-13
    else if(aEvent.GetCommand() == EVT_FM_MEDIA_SCAN_READ_START && aEvent.GetSource() == APP_AVP)
    {
        // { added by Sergey 25.04.2013
        // For fast screen transition to FileManager
        emit loadMainQml(AppEngineQMLConstants::SCREEN_FRONT);
        emit loadMainQml(AppEngineQMLConstants::SCREEN_REAR);
        // } added by Sergey 25.04.2013

        if(getTrackerInstance()) {
            int *pData = (int*)aEvent.Data().data();
            ETrackerAbstarctor_VolumeType volumeType = (ETrackerAbstarctor_VolumeType)(*pData);
            getTrackerInstance()->StartReadScanResult(volumeType);
        }
    }
    // added by Dmitry 12.07.13 for ISV85006
    else if (aEvent.GetCommand() == EVT_FM_VIDEO_STATE_CHANGED && (aEvent.GetSource() == APP_AVP))
    {
       //GetLogger().Log(QString("ZZZ *(int*)aEvent.Data().data() = %1").arg(*(int*)aEvent.Data().data()));
       setVideoPlaying(!(*(int*)aEvent.Data().data()));
    }
    // added by Dmitry 12.07.13 for ISV85006
    // mofdified for ISV 89536
    else if ( (aEvent.GetCommand() == EVT_KEY_HU_SEEK_PREV) || (aEvent.GetCommand() ==  EVT_KEY_SWRC_SEEK_PREV)
             || (aEvent.GetCommand() ==  EVT_KEY_HU_SEEK_NEXT) || (aEvent.GetCommand() ==  EVT_KEY_SWRC_SEEK_NEXT)
             || (aEvent.GetCommand() ==  EVT_KEY_RRC_SEEK_PREV) || (aEvent.GetCommand() ==  EVT_KEY_RRC_SEEK_NEXT) )
    {
        emit hideVideoMenu();
    }
}
// modified by Michael.Kim 2014.06.27 for Copy List Issue in smoke test

// { modified by kihyung 2012.12.23 for SwapDC
void FileManagerEngine::NotifyFromUIStateManager( Event aEvent )
{
    EventFromUIStateManager(aEvent, true);
}

void FileManagerEngine::NotifyFromUIStateManagerRear( Event aEvent )
{
    EventFromUIStateManager(aEvent, false);
}

void FileManagerEngine::processJogDirectionKey(Event & aEvent){
    switch(aEvent.GetEventId()){
    case EVT_KEY_CCP_JOG_DIAL_UP:
    case EVT_KEY_CCP_JOG_DIAL_DOWN:
    case EVT_KEY_RRC_JOG_DIAL_UP:
    case EVT_KEY_RRC_JOG_DIAL_DOWN:
    {
        KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
        if ( eventData->keyStatus == KEY_STATUS_RELEASED || eventData->keyStatus == KEY_STATUS_CANCEL){
            m_jogDirectionPressedStatus = false;
        }
        else{
            if ( aEvent.GetEventId() == EVT_KEY_RRC_JOG_DIAL_DOWN || aEvent.GetEventId() == EVT_KEY_CCP_JOG_DIAL_DOWN)
                m_jogDirectionKeyPressed = AppEngineQMLConstants::JOG_DOWN;
            else
                m_jogDirectionKeyPressed = AppEngineQMLConstants::JOG_UP;

            m_jogDirectionPressedStatus = true;
        }
    }
    break;

    default: break;
    }
}

void FileManagerEngine::cancelJogDirectionKey(){
    if ( m_jogDirectionPressedStatus ){
        signalJogNavigation(m_jogDirectionKeyPressed, AppEngineQMLConstants::KEY_STATUS_CANCELED); 
        m_jogDirectionPressedStatus = false;
    }
}

void FileManagerEngine::NotifyFromUIStateManagerCommon ( Event aEvent )
{
    GetLogger().Log( QString( "FileManagerEngine::NotifyFromUIStateManagerCommon Start with [%1]").arg( aEvent.GetEventId() ) );
    
    if ( (aEvent.GetTarget() != GetThisAppId()) && (aEvent.GetTarget() != APP_ALL) )
    {
       return;
    }
 
    processJogDirectionKey(aEvent);

    switch ( aEvent.GetEventId() )
    {
        case EVT_LANGUAGE_CHANGED:
        {
           GetLogger().Log( "FileManagerEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED" );
     
           EVENT_LANGUAGE_CHANGED_DATA_T *pLanguageData = (EVENT_LANGUAGE_CHANGED_DATA_T *)aEvent.Data().data();
     
           if ( pLanguageData != NULL )
           {
              if ( m_pTranslator == NULL )
              {
                 m_pTranslator = new QTranslator( this );
              }
              setMiddleEast(false); // added by Dmitry 11.05.13
     //aettie added country varient LANGUAGE_ES_NA / LANGUAGE_FR_NA / LANGUAGE_EN_US
              if ( m_pTranslator )
              {
                 switch ( pLanguageData->languageId )
                 {
                 case LANGUAGE_KO:
                    GetLogger().Log( "FileManagerEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::KO" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_ko" );
                    break;
                 case LANGUAGE_ES_NA:
                    GetLogger().Log( "FileManagerEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::ES EU" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_es-na" );
                    break;
     
                 case LANGUAGE_FR_NA:
                    GetLogger().Log( "FileManagerEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR EU" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_fr-na" );
                    break;
                 case LANGUAGE_ES:
                    GetLogger().Log( "FileManagerEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::ES EU" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_es-eu" );
                    break;
     
                 case LANGUAGE_FR:
                    GetLogger().Log( "FileManagerEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR EU" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_fr-eu" );
                    break;
     
                 case LANGUAGE_AR:
                 {
                    GetLogger().Log( "FileManagerAppEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_ar" );
                    setMiddleEast(true); // added by Dmitry 11.05.13
                    break;
                 }
     
                 case LANGUAGE_CS:
                 {
                    GetLogger().Log( "FileManagerAppEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_cs" );
                    break;
                 }
     
                 case LANGUAGE_DA:
                 {
                    GetLogger().Log( "FileManagerAppEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_da" );
                    break;
                 }
     
                 case LANGUAGE_EN_GB:
                 {
                    GetLogger().Log( "FileManagerAppEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_en-gb" );
                    break;
                 }
     
                 case LANGUAGE_DE:
                 {
                    GetLogger().Log( "FileManagerAppEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_ge" );
                    break;
                 }
     
                 case LANGUAGE_IT:
                 {
                    GetLogger().Log( "FileManagerAppEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_it" );
                    break;
                 }
     
                 case LANGUAGE_NL:
                 {
                    GetLogger().Log( "FileManagerAppEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_nl" );
                    break;
                 }
     
                 case LANGUAGE_PL:
                 {
                    GetLogger().Log( "FileManagerAppEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_pl" );
                    break;
                 }
     
                 case LANGUAGE_PT:
                 {
                    GetLogger().Log( "FileManagerAppEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_pt" );
                    break;
                 }
     
                 case LANGUAGE_RU:
                 {
                    GetLogger().Log( "FileManagerAppEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_ru" );
                    break;
                 }
     
                 case LANGUAGE_SK:
                 {
                    GetLogger().Log( "FileManagerAppEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_sk" );
                    break;
                 }
     
                 case LANGUAGE_SV:
                 {
                    GetLogger().Log( "FileManagerAppEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_sv" );
                    break;
                 }
     
                 case LANGUAGE_TR:
                 {
                    GetLogger().Log( "FileManagerAppEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_tr" );
                    break;
                 }
     
                 case LANGUAGE_ZH_CN:
                 {
                    GetLogger().Log( "FileManagerAppEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_zh-cmn" );
                    break;
                 }
     
                 case LANGUAGE_ZH_YUE:
                 {
                    GetLogger().Log( "FileManagerAppEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::FR" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_zh-yue" );
                    break;
                 }
                 case LANGUAGE_EN_US :
                 default:
                    GetLogger().Log( "FileManagerEngine::NotifyFromUIStateManagerCommon:Processing Event::EVT_LANGUAGE_CHANGED::DEF" );
                    m_pTranslator->load( "/app/share/AppFileManager/localization/DHAVN_AppFileManager_en" );
                    break;
                 }
     
                 qApp->installTranslator( m_pTranslator );
     
                 emit retranslateUi((int)pLanguageData->languageId);
              }
           }
           break;
        }
        //added by aettie for ticker stop when DRS on
        case EVT_DRS_SHOW:
        {
            m_bScrollingTicker = false;
            emit tickerChanged(m_bScrollingTicker);
            break;
        }
        case EVT_DRS_HIDE:
        {
            m_bScrollingTicker = true;
            emit tickerChanged(m_bScrollingTicker);

            break;
        }
        case EVT_USB_MOUNTED:
        {
            APP_EVENT_USB_DATA_T eventData;
            if (sizeof(APP_EVENT_USB_DATA_T) != aEvent.Data().size())
            {
                return;
            }

            memcpy(&eventData, aEvent.Data().data(), sizeof(APP_EVENT_USB_DATA_T));
            emit signalUSBPluggedIn(eventData.mountPath, (int)eventData.usbPort);
            if(eventData.usbPort == eUSB_PORT1)
                m_bUSB1Attached = true;
            else if(eventData.usbPort == eUSB_PORT2)
                m_bUSB2Attached = true;

            m_isUsbAttached = true;
            emit updateUsbAttached();
            break;
        }
     
        case EVT_USB_REMOVED:
        {
            APP_EVENT_USB_DATA_T eventData;
            if (sizeof(APP_EVENT_USB_DATA_T) != aEvent.Data().size())
            {
                return;
            }

            memcpy(&eventData, aEvent.Data().data(), sizeof(APP_EVENT_USB_DATA_T));
            emit signalUSBPluggedOut((int)eventData.usbPort);
            if(eventData.usbPort == eUSB_PORT1)
            {
                m_bUSB1TAReadComplete = false;
                m_bUSB1Attached = false;
            }
            else if(eventData.usbPort == eUSB_PORT2) 
            {
                m_bUSB2TAReadComplete = false;
                m_bUSB2Attached = false;
            }

            if(!m_bUSB1Attached && !m_bUSB2Attached)
                m_isUsbAttached = false;

            emit updateUsbAttached();
            break;
        }
     
        case EVT_KEY_HU_BACK:
        case EVT_KEY_CCP_BACK:
        {
            KeyEventHandler( CommonDef::FILEMANAGER_EVT_KEY_BACK, aEvent,  GetViewR(DISPLAY_FRONT));//AppEngineQMLConstants::SCREEN_FRONT);//modified by edo.lee 2013.04.02
            break; 
        }
        
        case EVT_KEY_RRC_BACK:
        {
            KeyEventHandler( CommonDef::FILEMANAGER_EVT_KEY_BACK, aEvent,  GetViewR(DISPLAY_REAR));//AppEngineQMLConstants::SCREEN_REAR);//modified by edo.lee 2013.04.02
            break;
        }
     
        case EVT_KEY_HU_MENU:
        case EVT_KEY_CCP_MENU:
        {
            //added by suilyou 20131014 ITS 0195205 START
            if((m_eCloneStatus != CLONE_OFF) && ( getJogKeyPressed() != 0))
            {
                MP_CRITICAL << "EVT_KEY_HU_MENU Menu not processed in clone mode and jog already pressed" << LOG_ENDL;
                break;
            }

            //added by suilyou 20131014 ITS 0195205 END
            KeyEventHandler( CommonDef::FILEMANAGER_EVT_KEY_MENU, aEvent, GetViewR(DISPLAY_FRONT));//AppEngineQMLConstants::SCREEN_FRONT);//modified by edo.lee 2013.03.19
            break;
        }
        
        case EVT_KEY_RRC_MENU:
        {
            //added by suilyou 20131014 ITS 0195205 START
            // @todo - UISH to fix menu press on clone, remove below code once fixed in UISH
            if((m_eCloneStatus != CLONE_OFF) && ( getJogKeyPressed() != 0))
            {
                MP_CRITICAL << "EVT_KEY_HU_MENU Menu not processed in clone mode and jog already pressed" << LOG_ENDL;
                break;
            }
            //added by suilyou 20131014 ITS 0195205 END
            KeyEventHandler( CommonDef::FILEMANAGER_EVT_KEY_MENU, aEvent, GetViewR(DISPLAY_REAR));//AppEngineQMLConstants::SCREEN_REAR);//modified by edo.lee 2013.03.19
            break;
        }

        #if USE_AFW_SWAP_DC
        case EVT_KEY_SOFT_MENU:
        {
//{ added by aettie 2013.08.01 for ITS 181900
            // @todo - UISH to fix menu press on clone, remove below code once fixed in UISH
            if((m_eCloneStatus != CLONE_OFF) && ( getJogKeyPressed() != 0))
            {
                MP_CRITICAL << "EVT_KEY_HU_MENU Menu not processed in clone mode and jog already pressed" << LOG_ENDL;
                break;
            }


            if ( m_jogDirectionPressedStatus ){
                signalJogNavigation(m_jogDirectionKeyPressed, AppEngineQMLConstants::KEY_STATUS_CANCELED); 
                m_jogDirectionPressedStatus = false;
            }

            emit keyEvent( (int)CommonDef::FILEMANAGER_EVT_KEY_MENU, (int)CommonDef::FILEMANAGER_EVT_KEY_STATUS_PRESSED, AppEngineQMLConstants::SCREEN_FRONT );
            break;
        }
        #endif
     
        case EVT_KEY_CCP_JOG_DIAL_UP:
        {
            JogEventHandle( CommonDef::JOG_DIRECTION_UP, aEvent );
            break;
        }
     
        case EVT_KEY_CCP_JOG_DIAL_RIGHT:
        {
        //added by suilyou 20130924 for key/touch blocking START
        KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
        switch(eventData->keyStatus)
        {
            case KEY_STATUS_PRESSED:
            {
                setJogKeyPressed(CCP_JOG_RIGHT_PRESSED );
//                setSharedMemData(getJogKeyPressed());
                GetLogger().Log(QString("FileManagerEngine::EVT_KEY_CCP_JOG_DIAL_RIGHT:setJogKeyPressed key status: %1").arg(getJogKeyPressed()));
                break;
            }
            case KEY_STATUS_RELEASED:
            {
                setJogKeyReleased(CCP_JOG_RIGHT_PRESSED );
//                setSharedMemData(getJogKeyPressed());
                GetLogger().Log(QString("FileManagerEngine::EVT_KEY_CCP_JOG_DIAL_RIGHT:setJogKeyReleased key status: %1").arg(getJogKeyPressed()));
                break;
            }
            default:
            {
                break;
            }
        }
        //added by suilyou 20130924 for key/touch blocking END
            JogEventHandle( CommonDef::JOG_DIRECTION_RIGHT, aEvent );
            break;
        }
     
        case EVT_KEY_CCP_JOG_DIAL_DOWN:
        {
            JogEventHandle( CommonDef::JOG_DIRECTION_DOWN, aEvent );
            break;
        }
     
        case EVT_KEY_CCP_JOG_DIAL_LEFT:
        {
            //added by suilyou 20130924 for key/touch blocking START
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
            switch(eventData->keyStatus)
            {
                case KEY_STATUS_PRESSED:
                {
                    setJogKeyPressed(CCP_JOG_LEFT_PRESSED );
                    //setSharedMemData(getJogKeyPressed());
                    GetLogger().Log(QString("FileManagerEngine::EVT_KEY_CCP_JOG_DIAL_LEFT:setJogKeyPressed key status: %1").arg(getJogKeyPressed()));
                    break;
                }
                case KEY_STATUS_RELEASED:
                {
                    setJogKeyReleased(CCP_JOG_LEFT_PRESSED );
                    //setSharedMemData(getJogKeyPressed());
                    GetLogger().Log(QString("FileManagerEngine::EVT_KEY_CCP_JOG_DIAL_LEFT:setJogKeyReleased key status: %1").arg(getJogKeyPressed()));
                    break;
                }
                default:
                {
                    break;
                }
            }
            //added by suilyou 20130924 for key/touch blocking END
            JogEventHandle( CommonDef::JOG_DIRECTION_LEFT, aEvent );
            break;
        }
        case EVT_KEY_CCP_JOG_DIAL_TOP_RIGHT:
        case EVT_KEY_CCP_JOG_DIAL_DOWN_LEFT:
        {
            JogEventHandle( CommonDef::JOG_DIRECTION_SCROLL_RIGHT, aEvent );
            break;
        }
     
        case EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT:
        case EVT_KEY_CCP_JOG_DIAL_TOP_LEFT:
        {
            JogEventHandle( CommonDef::JOG_DIRECTION_SCROLL_LEFT, aEvent );
            break;
        }
     
        case EVT_KEY_CCP_JOG_DIAL_CENTER:
        {
            //added by suilyou 20130924 for key/touch blocking START
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
            switch(eventData->keyStatus)
            {
                case KEY_STATUS_PRESSED:
                {
                    setJogKeyPressed(CCP_JOG_CENTER_PRESSED );
                    //setSharedMemData(getJogKeyPressed());
                    MP_LOG << "EVT_KEY_CCP_JOG_DIAL_CENTER:setJogKeyPressed key status" << getJogKeyPressed() << LOG_ENDL;
                    break;
                }
                case KEY_STATUS_RELEASED:
                {
                    setJogKeyReleased(CCP_JOG_CENTER_PRESSED );
                    //setSharedMemData(getJogKeyPressed());
                     MP_LOG << "EVT_KEY_CCP_JOG_DIAL_CENTER:setJogKeyReleased key status" << getJogKeyPressed() << LOG_ENDL;
                    break;
                }
                case KEY_STATUS_CANCEL:
                {
                    setJogKeyReleased(CCP_JOG_CENTER_PRESSED );
                    emit ClearSharedData();
                    break;
                }
                default:
                {
                    break;
                }
            }
        //added by suilyou 20130924 for key/touch blocking END
            JogEventHandle( CommonDef::JOG_DIRECTION_CENTER, aEvent );
            break;
        }
        //added by suilyou 20130924 for key/touch blocking START
        case EVT_KEY_RRC_JOG_DIAL_CENTER:
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
            switch(eventData->keyStatus)
            {
                case KEY_STATUS_PRESSED:
                {
                    setJogKeyPressed(RRC_JOG_CENTER_PRESSED );
                    //setSharedMemData(getJogKeyPressed());
                    GetLogger().Log(QString("FileManagerEngine::EVT_KEY_RRC_JOG_DIAL_CENTER:setJogKeyPressed key status: %1").arg(getJogKeyPressed()));
                    break;
                }
                case KEY_STATUS_RELEASED:
                {
                    setJogKeyReleased(RRC_JOG_CENTER_PRESSED );
                    //setSharedMemData(getJogKeyPressed());
                    GetLogger().Log(QString("FileManagerEngine::EVT_KEY_RRC_JOG_DIAL_CENTER:setJogKeyReleased key status: %1").arg(getJogKeyPressed()));
                    break;
                }

// added by Dmitry 08.11.13 for ITS0207568
                case KEY_STATUS_CANCEL:
                {
                   setJogKeyReleased(RRC_JOG_CENTER_PRESSED );
                   emit ClearSharedData();
                   break;
                }

                default:
                {
                    break;
                }
            }
            //added by suilyou 20130924 for key/touch blocking END
            break;
        }
        case EVT_KEY_RRC_JOG_DIAL_LEFT:
        {
            //added by suilyou 20130924 for key/touch blocking START
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
            switch(eventData->keyStatus)
            {
                case KEY_STATUS_PRESSED:
                {
                    setJogKeyPressed(RRC_JOG_LEFT_PRESSED );
                    //setSharedMemData(getJogKeyPressed());
                    GetLogger().Log(QString("FileManagerEngine::EVT_KEY_RRC_JOG_DIAL_LEFT:setJogKeyPressed key status: %1").arg(getJogKeyPressed()));
                    break;
                }
                case KEY_STATUS_RELEASED:
                {
                    setJogKeyReleased(RRC_JOG_LEFT_PRESSED );
                    //setSharedMemData(getJogKeyPressed());
                    GetLogger().Log(QString("FileManagerEngine::EVT_KEY_RRC_JOG_DIAL_LEFT:setJogKeyReleased key status: %1").arg(getJogKeyPressed()));
                    break;
                }
                default:
                {
                    break;
                }
            }
            //added by suilyou 20130924 for key/touch blocking END
            break;
        }
        case EVT_KEY_RRC_JOG_DIAL_RIGHT:
        {
            //added by suilyou 20130924 for key/touch blocking START
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
            switch(eventData->keyStatus)
            {
                case KEY_STATUS_PRESSED:
                {
                    setJogKeyPressed(RRC_JOG_RIGHT_PRESSED );
                    //setSharedMemData(getJogKeyPressed());
                    GetLogger().Log(QString("FileManagerEngine::EVT_KEY_RRC_JOG_DIAL_RIGHT:setJogKeyPressed key status: %1").arg(getJogKeyPressed()));
                    break;
                }
                case KEY_STATUS_RELEASED:
                {
                    setJogKeyReleased(RRC_JOG_RIGHT_PRESSED );
                    //setSharedMemData(getJogKeyPressed());
                    GetLogger().Log(QString("FileManagerEngine::EVT_KEY_RRC_JOG_DIAL_RIGHT:setJogKeyReleased key status: %1").arg(getJogKeyPressed()));
                    break;
                }
                default:
                {
                    break;
                }
            }
            //added by suilyou 20130924 for key/touch blocking END
            break;
        }
        case EVT_SETTINGS_CHANGED:
        {
            if ( aEvent.Data().isEmpty() )
            {
                break;
            }
     
            EVENT_SETTINGS_CHANGED_DATA_T *eventData = (EVENT_SETTINGS_CHANGED_DATA_T*)aEvent.Data().data();
            handleSettingsCommand( eventData );
            break;
        }
        
//        case EVT_ACC_CHANGED:
//        {
//            // modified by ravikanth 14.06.2013 for ITS 0173814
//            EVENT_EVT_CAR_IF_STATUS_T *paccstatus = (EVENT_EVT_CAR_IF_STATUS_T *)aEvent.Data().data();
//            if(paccstatus->status == 0)
//            {
//                GetLogger().Log(QString("ACC off"));
//                cancelFileCopyOperation();
//                emit cancelJukeboxFormat();
//                // emit hideOnBg(); // modified by Dmitry Bykov 13.04.13 no need to hide focus
//            }
//            break;
//        }
// { add by eunhye 2013.02.06 for ISV70222/70319, ITS150138/149862

        case EVT_KEY_HU_JOG_DIAL_ENCODER:
        {

            if(m_popupType)
            {
                //remove by edo.lee 2013.07.15 skip tune on popup
                QString param(aEvent.GetCommand()); //modified by edo.lee 2013.10.01 ITS 0183932 
                Event event( EVT_KEY_HU_JOG_DIAL_ENCODER, GetThisAppId(), APP_AVP, aEvent.GetCommand()); // modified by lssanh 2013.02.21 IPC payload
                sendEventToUISH( event, m_IsFrontVisible );
                //remove by edo.lee 2013.07.15
            }
            else
            {
                //added by suilyou 20130924 START
                if(getJogKeyPressed()!=0)
                    return;
                switch(m_eCloneStatus)
                {
                    case CLONE_OFF:
                    {
                        switch(IsSwapDCEnabled()) // modified by Dmitry 10.03.13 for ITS0187937
                        {
                            case true:
                            {
                                if(getJogKeyPressed()!=0)
                                {
                                    GetLogger().Log(QString("EVT_KEY_HU_JOG_DIAL_ENCODER: JogKeyPressed key status = %1").arg(getJogKeyPressed()));
                                    return;
                                }
                                break;
                            }
                            case false:
                            {
                                if(getJogKeyPressed()!=0)
                                {
                                    GetLogger().Log(QString("EVT_KEY_HU_JOG_DIAL_ENCODER: JogKeyPressed key status = %1").arg(getJogKeyPressed()));
                                    return;
                                }
                                break;
                            }
                        }
                        break;
                    }
                    default:
                    {
                        if(getJogKeyPressed()!=0)
                        {
                            GetLogger().Log(QString("EVT_KEY_HU_JOG_DIAL_ENCODER: JogKeyPressed key status = %1").arg(getJogKeyPressed()));
                            return;
                        }
                        break;
                    }
                }
                if((int)aEvent.GetCommand() > 0  )
                    signalJogNavigation( AppEngineQMLConstants::JOG_WHEEL_RIGHT, AppEngineQMLConstants::KEY_STATUS_PRESSED);
                else
                    signalJogNavigation( AppEngineQMLConstants::JOG_WHEEL_LEFT, AppEngineQMLConstants::KEY_STATUS_PRESSED);
            }
            break;
        }
        case EVT_KEY_HU_TUNE_PRESSED:
        {
            KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
            if ( message->keyStatus == KEY_STATUS_RELEASED)
            {
                setJogKeyReleased(CCP_JOG_CENTER_PRESSED);
                if(m_popupType)
                {
                    //remove by edo.lee 2013.07.15 skip tune on popup
                    KEY_EVENT_DATA_T keyData;
                    keyData.keyStatus = KEY_STATUS_RELEASED;
                    Event event( EVT_KEY_HU_TUNE_PRESSED, GetThisAppId(), APP_AVP, aEvent.GetCommand(), QByteArray( (const char *)&keyData, sizeof(KEY_EVENT_DATA_T))); // modified by lssanh 2013.02.21 IPC payload
                    sendEventToUISH( event, m_IsFrontVisible );
                }
                else
                {
                    signalJogNavigation( AppEngineQMLConstants::JOG_CENTER, AppEngineQMLConstants::KEY_STATUS_RELEASED );
                }
            }
            else if ( message->keyStatus == KEY_STATUS_PRESSED)
            {
                if ( m_popupType ) {
                    KEY_EVENT_DATA_T keyData;
                    keyData.keyStatus = KEY_STATUS_PRESSED;
                    Event event( EVT_KEY_HU_TUNE_PRESSED, GetThisAppId(), APP_AVP, aEvent.GetCommand(), QByteArray( (const char *)&keyData, sizeof(KEY_EVENT_DATA_T))); // modified by lssanh 2013.02.21 IPC payload
                    sendEventToUISH( event, m_IsFrontVisible );
                }
                else{
                    //added by suilyou 20131010 ITS 0194943
                    setJogKeyPressed(CCP_JOG_CENTER_PRESSED);
                    signalJogNavigation( AppEngineQMLConstants::JOG_CENTER, AppEngineQMLConstants::KEY_STATUS_PRESSED );
                }
            }
            break;
        }
        case EVT_CLONE_NOTIFICATION:
        {
            if(!aEvent.Data().isEmpty())
            {
                EVENT_CLONE_NOTIFICATION_DATA_T *pEventData = (EVENT_CLONE_NOTIFICATION_DATA_T *)aEvent.Data().data();

                if(pEventData)
                {
                    m_eCloneStatus = pEventData->sCloneStatus;
// modified by Dmitry 10.03.13 for ITS0187937
                }
            }
            break;
        }
// modified by Dmitry 08.10.13 for ITS0194416

        case EVT_FRONT_LCD_ON:
        {
           m_isFrontLCD = true;
           break;
        }

        case EVT_FRONT_LCD_OFF:
        {
           m_isFrontLCD = false;
           break;
        }
        
        // modified for ITS 0212002
        case EVT_POWER_STATE_LOGIC_OFF:
        {
           // modified by ravikanth 14.06.2013 for ITS 0173814
           cancelFileCopyOperation();
           emit cancelJukeboxFormat();
           break;
        }
// added by Dmitry 10.10.13 for ITS0194726
        case EVT_POWER_STATE_NORMAL_OFF:
        {
           m_bisPowerOff = true;
           // modified for ITS 0210855 reversed
           //cancelFileCopyOperation();
           //emit cancelJukeboxFormat();
           break;
        }

        case EVT_POWER_STATE_NORMAL_ON:
        {
           m_bisPowerOff = false;
           break;
        }

        // { added by honggi.shin, 2014.05.23, for finishing thumbnail operation.
        case EVT_REQUEST_SAVE_UNSAVED_DATA:
        {
            MP_CRITICAL << "EVT_REQUEST_SAVE_UNSAVED_DATA" << LOG_ENDL;

            // 1. need to stop videoThumbnail operation
            QList<QVariant> arg1;
            arg1 << "MSG:STOP";
            QDBusMessage message = QDBusMessage::createMethodCall("org.lge.TrackerExtractor", "/", "org.lge.TrackerExtractor", "generateTNFromPath");
            message.setArguments(arg1);
            m_pExtractor->getConnection().asyncCall(message);


            // 2. need to stop photoThumbnail operation
            // it implemented in AVP

            // 3. need to stop pic_list_thumbnail operation
            Thumbnailer::setReadyToPWOFF(true);
            MP_CRITICAL << "setReadyToPWOFF is true. pic list operation stoped." << LOG_ENDL;

            // 4. sleep for stop
            usleep(1000 * 1000);

            // 5. send response
            Event event( EVT_RESPONSE_SAVE_UNSAVED_DATA, GetThisAppId(), APP_UI_MANAGER );
            sendEventToUISH( event, true );

            break;
        }
        // } added by honggi.shin, 2014.05.23, END


// added by Dmitry 10.10.13 for ITS0194726
// added by Dmitry 06.11.13 for ISV94041
        case EVT_FORMAT_JUKEBOX_START:
        {
           emit formatting(true);
           break;
        }

        case EVT_FORMAT_JUKEBOX_END:
        {
           emit formatting(false);
           break;
        }

         //{added for ITS 232574 2014.03.28
        case EVT_SWAP_DC_ENABLED:
        {
            setJogKeyReleased(CCP_JOG_CENTER_PRESSED);
            signalJogNavigation( AppEngineQMLConstants::JOG_CENTER, AppEngineQMLConstants::KEY_STATUS_CANCELED );
            break;
        }
         //}added for ITS 232574 2014.03.28

// added by Dmitry 06.11.13 for ISV94041
    }
}
//added by edo.lee 2013.03.19
AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T 
FileManagerEngine::GetViewR(DISPLAY_T display) 
{
    AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T ret = AppEngineQMLConstants::SCREEN_FRONT;
    if(m_eCloneStatus == CLONE_ON_FRONT)
    {
        ret = AppEngineQMLConstants::SCREEN_FRONT;
    }
    else if(m_eCloneStatus == CLONE_ON_REAR)
    {
        ret = AppEngineQMLConstants::SCREEN_REAR;
    }
    else if(m_eCloneStatus == CLONE_OFF)
    {
        if(display == DISPLAY_FRONT)
            ret = AppEngineQMLConstants::SCREEN_FRONT;
        else
            ret = AppEngineQMLConstants::SCREEN_REAR;
    }   
    return ret;
}
//added by edo.lee 2013.03.19

// } modified by kihyung 2012.12.23 for SwapDC

void FileManagerEngine::TAReadCompleted(ETrackerAbstarctor_VolumeType volumeType, int bSuccess)
{   
    GetLogger().Log(QString("FileManagerEngine::TAReadCompleted: volumeType: %1, bSuccess: %2").arg(volumeType).arg(bSuccess));
    
    if(volumeType == eJukeBox) 
    {
        m_bJukeboxTAReadComplete = true;        
    }
    else if(volumeType == eUSB_FRONT) 
    {
        m_bUSB1TAReadComplete = true;
    }
    else if(volumeType == eUSB_REAR) 
    {
        m_bUSB2TAReadComplete = false;            
    }
    else 
    {
        return;
    }

    Event tPostevent(EVT_POST_EVENT, GetThisAppId(), APP_AVP, EVT_FM_MEDIA_SCAN_READ_END, QByteArray((char*)&volumeType, sizeof(volumeType)));
    sendEventToUISH(tPostevent, true);
}


void FileManagerEngine::HandleBackKey(bool isFront)
{
   GetLogger().Log( "FileManagerEngine::HandleBackKey: Start" );

   Event event( EVT_KEY_SOFT_BACK, GetThisAppId(), APP_NONE );
   sendEventToUISH(event, isFront);
}

void FileManagerEngine::HandleHomeKey(bool isFront)
{
   GetLogger().Log( "FileManagerEngine::HandleHomeKey: Start" );

   Event event( EVT_KEY_SOFT_HOME, GetThisAppId(), APP_NONE );
   sendEventToUISH(event, isFront);
}

void FileManagerEngine::OnUSBTest()
{
   GetLogger().Log( "FileManagerEngine::OnUSBTest - Passed" );
}
   
void FileManagerEngine::PlayMusicFile(const QString &filePath, bool isFront)
{
   // FM_LOG << "FileManagerEngine::PlayMusicFile  filePath = " << filePath  << LOG_ENDL;
   QString param(filePath);
   // Post event
   Event postevent( EVT_POST_EVENT, GetThisAppId(), APP_AUDIO, param.toUtf8() ); // modified by lssanh 2013.02.21 IPC payload

   sendEventToUISH(postevent, isFront);

   Event event( EVT_KEY_SOFT_MP, GetThisAppId(), APP_UI_MANAGER );

   sendEventToUISH(event, isFront);
}


// { modified by Sergey 15.09.2013 for ITS#185233
void FileManagerEngine::PlayVideoFile(const QString &filePath, bool isFront)
{
    // { modified by kihyung 2013.07.08 for ITS 0178816 
    /*
    QString param(filePath);
    Event postevent( EVT_POST_EVENT, GetThisAppId(), APP_AVP, EVT_PLAY_VIDEO, param.toUtf8() );
    sendEventToUISH(postevent, isFront);
    */
    // { modified by edo.lee 2013.08.30 ITS 0186482
    if((this->frontScreenData.m_isBgState && isFront)
            ||( this->rearScreenData.m_isBgState && !isFront))
    {
        MP_CRITICAL << "FileManagerEngine::FM is BG ...  does not work PlayVideoFile()" << LOG_ENDL;
        return;
    }
    // { modified by edo.lee 2013.08.30

    QString checkFile = filePath;
    checkFile.remove( "file://" );

    // { modified by Sergey 10.11.2013 new DivX popups
    int unsupportedType = GetUSBController()->IsFileSupportedAdvanced(checkFile);

    m_drmFile.isDRM = GetUSBController()->isDivXDRM(&m_drmFile.type, &m_drmFile.limitCount, &m_drmFile.usedCount);

    MP_LOG << "isDRM =" << m_drmFile.isDRM << "type =" << m_drmFile.type << "limit =" << m_drmFile.limitCount << "used =" << m_drmFile.usedCount << "isSupported =" << unsupportedType << LOG_ENDL;

    if(unsupportedType != eMP_MEDIA_SUPPORTED)
    {
        UIMainDef::POPUP_TYPE popupType = UIMainDef::POPUP_TYPE_MAX;

        switch(unsupportedType)
        {
            case eMP_MEDIA_UNSUPPORTED_GENERAL: popupType = UIMainDef::POPUP_TYPE_UNSUPPORTED_FILE; break;
            case eMP_MEDIA_UNSUPPORTED_AUDIO: popupType = UIMainDef::POPUP_TYPE_AUDIO_FORMAT_UNSUPPORTED; break;
            case eMP_MEDIA_UNSUPPORTED_VIDEO: popupType = UIMainDef::POPUP_TYPE_RESOLUTION_UNSUPPORTED; break;
            default: popupType = UIMainDef::POPUP_TYPE_UNSUPPORTED_FILE; break;
        }

        // { modified by kihyung 2013.12.09 for ITS 0213549 
        if(popupType != UIMainDef::POPUP_TYPE_MAX)
        {
            if(popupType == UIMainDef::POPUP_TYPE_UNSUPPORTED_FILE)
            {
                QString strUSB1Path = GetFrontUSBPath();
                QString strUSB2Path = GetRearUSBPath();

                if(checkFile.indexOf(strUSB1Path) == 0) 
                {
                    if(m_bUSB1Attached == false)
                    {
                        MP_CRITICAL << "m_bUSB1Attached == false. return...";
                        return;
                    }
                    else if(checkFile.indexOf(strUSB1Path) == 0 && checkFile.count('/') >= 3) 
                    {
                        QString strUSBMount;
                        if(checkFile.indexOf("/rw_data/media") == 0)
                            strUSBMount = "/" + checkFile.section('/', 1, 4);
                        else
                            strUSBMount = "/" + checkFile.section('/', 1, 3);

                        usleep(100000);
                        if(access(strUSBMount.toAscii().constData(), F_OK) != 0) {
                            MP_CRITICAL << strUSBMount << "maybe disconnected. return...";
                            return;
                        }
                        MP_CRITICAL << strUSBMount << LOG_ENDL;
                    }
                }
                else if(checkFile.indexOf(strUSB2Path) == 0) 
                {
                    if(m_bUSB2Attached == false)
                    {
                        MP_CRITICAL << "m_bUSB2Attached == false. return..." << LOG_ENDL;
                        return;
                    }
                    else if(checkFile.indexOf(strUSB2Path) == 0 && checkFile.count('/') >= 3)
                    {
                        QString strUSBMount;
                        if(checkFile.indexOf("/rw_data/media") == 0)
                            strUSBMount = "/" + checkFile.section('/', 1, 4);
                        else
                            strUSBMount = "/" + checkFile.section('/', 1, 3);
                        
                        usleep(100000);
                        if(access(strUSBMount.toAscii().constData(), F_OK) != 0) {
                            MP_CRITICAL << strUSBMount << "maybe disconnected. return..." << LOG_ENDL;
                            return;
                        }
                        MP_CRITICAL << strUSBMount << LOG_ENDL;
                    }
                }
            }
            emit showPopup(popupType, isFront); // modified by Dmitry 17.08.13
        }
        // } modified by kihyung 2013.12.09 for ITS 0213549 
        return;
    }
    // } modified by Sergey 10.11.2013 new DivX popups
    else if(m_drmFile.isDRM)
    {
        switch(m_drmFile.type)
        {
            case eMP_DRM_RENTAL_STATUS:
            {
                if(m_drmFile.limitCount == 0 && m_drmFile.usedCount == 0)
                {
                    handlePlayVideo(filePath, isFront);
                    break;
                }

                m_drmFile.path = filePath;
                emit showPopup(UIMainDef::POPUP_TYPE_DRM_CONFIRM, isFront);

                break;
            }
            case eMP_DRM_RENTAL_EXPIRED:
            {
                emit showPopup(UIMainDef::POPUP_TYPE_DRM_EXPIRED, isFront);
                break;
            }
            case eMP_DRM_NOT_AUTHORIZED:
            {
                emit showPopup(UIMainDef::POPUP_TYPE_DRM_NOT_AUTH, isFront);
                break;
            }
            default:
                break;
        }

        return;
    }
    
    handlePlayVideo(filePath, isFront);
}
// } modified by Sergey 15.09.2013 for ITS#185233


void FileManagerEngine::ShowPictureFile( const QString & filePath, bool isFront , bool isSupported)
{
    // FM_LOG << "FileManagerEngine::ShowPictureFile  filePath = " << filePath << LOG_ENDL;
    // modified for ITS 0206895
    if(isSupported || isFileSupported(filePath)) // modified by ravikanth 27-04-13
    {
        QString param(filePath);
            Event postevent( EVT_POST_EVENT, GetThisAppId(), APP_AVP, EVT_SHOW_PHOTO, param.toUtf8() );
        sendEventToUISH(postevent, isFront);

        if (currendDeviceIsJuke_front && isFront)
        {
            Event event( EVT_KEY_SOFT_AVP, GetThisAppId(), APP_UI_MANAGER, eAPP_AVP_PHOTO_MODE_JUKEBOX );
            sendEventToUISH(event, isFront);
        }
        else if (currendDeviceIsJuke_rear && !isFront)
        {
            Event event( EVT_KEY_SOFT_AVP, GetThisAppId(), APP_UI_MANAGER, eAPP_AVP_PHOTO_MODE_JUKEBOX );
            sendEventToUISH(event, isFront);
        }
        //modified by edo.lee 2013.06.11
        // { added by eugene.seo 2013.03.26
        // else if(filePath.contains(USBFront))
        else if(filePath.indexOf(GetFrontUSBPath()) == 0) // modified by kihyung 2013.12.09 for ITS 0213549 
        {
            Event event( EVT_KEY_SOFT_AVP, GetThisAppId(), APP_UI_MANAGER, eAPP_AVP_PHOTO_MODE_USB1 );
            sendEventToUISH(event, isFront);
        }
        // else if(filePath.contains(USBRear))
        else if(filePath.indexOf(GetRearUSBPath()) == 0) // modified by kihyung 2013.12.09 for ITS 0213549 
        {
            Event event( EVT_KEY_SOFT_AVP, GetThisAppId(), APP_UI_MANAGER, eAPP_AVP_PHOTO_MODE_USB2 );
            sendEventToUISH(event, isFront);
        }
        // } added by eugene.seo 2013.03.26
        else
        {
            Event event( EVT_KEY_SOFT_GALLERY, GetThisAppId(), APP_UI_MANAGER );
            sendEventToUISH(event, isFront);
        }
        // { modified by ravikanth 27-04-13
    }
    else
    {
        emit showPopup(UIMainDef::POPUP_TYPE_UNSUPPORTED_FILE, isFront); // modified by Dmitry 17.08.13
    }
    // } modified by ravikanth 27-04-13
}

void FileManagerEngine::JogEventHandle( CommonDef::JOG_DIRECTIONS direction, Event &event )
{
   KEY_EVENT_DATA_T eventData;

   if ( sizeof( KEY_EVENT_DATA_T ) == event.Data().size() )
   {
      memcpy( &eventData, event.Data().data(), sizeof( KEY_EVENT_DATA_T ) );
      if ( eventData.keyStatus == KEY_STATUS_RELEASED )
      {
         emit jogEvent( direction );
      }
   }
}

void FileManagerEngine::FileSelected(const QString &filePath, int fileType, bool isFront, bool isSupported)
{
   switch(fileType)
   {
      case CommonDef::MusicFileType:
      {
         PlayMusicFile(filePath, isFront);
      }
      break;
      case CommonDef::VideoFileType:
      {
         PlayVideoFile(filePath, isFront);
      }
      break;
      case CommonDef::PictureFileType:
      {
         if (isFront? appId_front == APP_SETTINGS : appId_rear == APP_SETTINGS ) //modified by edo.lee 2013.0527
         {
            LaunchSettingsPhotoFrame(filePath, isFront);
         }
         else
         {
            ShowPictureFile(filePath, isFront, isSupported); // modified for ITS 0206895
         }
      }
      break;
   }
}

void FileManagerEngine::LaunchSettingsPhotoFrame(QString filePath, bool isFront)
{
   GetLogger().Log( "FileManagerEngine::LaunchSettingsPhFr: Start" );

   // { modified by eugene.seo 2013.03.09
   if(isFileSupported(filePath))
   {
       // { modified by eugene.seo 2013.03.25
       //Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_SETTINGS, filePath.toAscii() ); 
       Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_SETTINGS, filePath.toUtf8() );
       // } modified by eugene.seo 2013.03.25
       sendEventToUISH( postEvent, isFront );

       Event event( EVT_KEY_SOFT_BACK, GetThisAppId(), APP_UI_MANAGER );
       sendEventToUISH( event, isFront );
   }
   else
   {
           emit showPopup(UIMainDef::POPUP_TYPE_UNSUPPORTED_FILE, isFront); // modified by Dmitry 17.08.13
   } 
   // } modified by eugene.seo 2013.03.09
}

// { added by eugene.seo 2013.03.09
bool  
FileManagerEngine::isFileSupported(QString file_path) 
{    
    MP_LOG << "isFileSupported: file_path = " << file_path << LOG_ENDL;

    if(file_path.startsWith("file://"))
        file_path = file_path.remove(0, 7);

    QFile file(file_path);
    double m_currentLoadingFileSize = 0.0;
    if (file.open(QIODevice::ReadOnly))
    {
        m_currentLoadingFileSize = (double)file.size()/1048576;
        MP_LOG << "file size: " << m_currentLoadingFileSize << LOG_ENDL;
        file.close();
    }
    else {
        MP_LOG << "file open error" <<  file_path << LOG_ENDL;
        return false;
    }
    
    if( m_currentLoadingFileSize <= PHOTO_MAX_SUPPORTED_SIZE ) // supported size in megabytes
    {
        MediaInfoLib::MediaInfo MI;
        MI.Open(file_path.toStdWString());
        m_imageInfoStruct.i_ImageWidth = QString::fromStdWString(MI.Get(MediaInfoLib::Stream_Image, 0, _T("Height"))).toInt();
        m_imageInfoStruct.i_ImageHeight = QString::fromStdWString(MI.Get(MediaInfoLib::Stream_Image, 0, _T("Width"))).toInt();
        MI.Close();
        // { modified by ravikanth 27-04-13
        if(m_imageInfoStruct.i_ImageWidth * m_imageInfoStruct.i_ImageHeight > PHOTO_MAX_SUPPORTED_WIDTH * PHOTO_MAX_SUPPORTED_HEIGHT )
        {
            return false;
        }
        else if(m_imageInfoStruct.i_ImageWidth == 0 || m_imageInfoStruct.i_ImageHeight == 0)
        {
            // some times for a valid image header info width and height is 0
            // use header info for performance for valid data if info is not valid
            // query data from image
            QImage image(file_path);
            double imageWidth = image.width();
            double imageHeight = image.height();
            if( imageWidth == 0 || imageHeight == 0 ||
                    imageWidth * imageHeight > PHOTO_MAX_SUPPORTED_WIDTH * PHOTO_MAX_SUPPORTED_HEIGHT )
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        else
        {
            return true;
        }
        // } modified by ravikanth 27-04-13
    }
    return false; // unsupported value greater than 5
}
// } added by eugene.seo 2013.03.09

// { added by kihyung 2013.07.08 for ITS 0178816
USBController* 
FileManagerEngine::GetUSBController()
{
    if(m_pUSBController == 0) {
        m_pUSBController = USBController::GetThirdInstance();
    }
    return m_pUSBController;
}
// } added by kihyung 2013.07.08 for ITS 0178816

void FileManagerEngine::LaunchSettingsKeypad(bool isFront)
{
   GetLogger().Log( "FileManagerEngine::LaunchSettingsKeypad: Start" );

   AppSettingsStartParameter startState;
   startState.startState = AppSettingsDef::eStart_Mode_General_Keyboard;
   Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_SETTINGS,
                   QByteArray((char *) &startState, sizeof(AppSettingsStartParameter)));

   sendEventToUISH( postEvent, isFront );

   Event event( EVT_KEY_SOFT_SETTINGS, GetThisAppId(), APP_UI_MANAGER );
   if ( !sendEventToUISH( event, isFront ) )
   {
      // MP_LOG << "FileManagerEngine::LaunchSettingsKeypad: Event notification failed" << LOG_ENDL;
   }
}

void FileManagerEngine::LaunchMusicList(bool isFront)
{
   GetLogger().Log( "FileManagerEngine::LaunchMusicList: Start" );

   Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_AUDIO );
   sendEventToUISH(postEvent, isFront);

   bool result = (isFront) ? this->frontScreenData.m_isListFromMP :
                             this->rearScreenData.m_isListFromMP;
   if ( result )
   {
      Event event( EVT_KEY_SOFT_BACK, GetThisAppId(), APP_UI_MANAGER );
      sendEventToUISH(event, isFront);
   }
   else
   {
      Event event( EVT_KEY_SOFT_MP, GetThisAppId(), APP_UI_MANAGER );
      sendEventToUISH(event, isFront);
   }
}

void FileManagerEngine::RetranslateUi()
{
    emit retranslateUi((int)m_nLanguage);
}

EVT_ID_T
FileManagerEngine::GetEvent( const int nKeyCode )
{
    unsigned int index = 0;

    while ( XKEYS_MAP[ index ].event != EVT_MAX )
    {
        if ( XKEYS_MAP[ index ].keyCode == nKeyCode &&
             XKEYS_MAP[ index ].display == m_nDisplay )
        {
            return XKEYS_MAP[ index ].event;
        }

        ++index;
    }

    return EVT_NONE;
}

void FileManagerEngine::KeyEventHandler( CommonDef::FILEMANAGER_EVT_KEY_T nEventKey, Event aEvent,
                                   int screen)
{
   KEY_EVENT_DATA_T *message = NULL;

   if ( !aEvent.Data().isEmpty() )
   {
      message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
   }

   if ( message )
   {
      emit keyEvent( (int)nEventKey, (int)message->keyStatus, screen );
   }
}

void FileManagerEngine::QmlKeyHandler( int nKeyCode, int nStatus, bool isFront )
{
   KEY_EVENT_DATA_T message;

   memset( &message, 0, sizeof(message) );

   message.keyStatus = (KEY_STATUS_T)nStatus;

   Event event( GetEvent( nKeyCode ),
                APP_FMGR, APP_FMGR,
                QByteArray( (char *)&message, sizeof( message ) ) ); // modified by lssanh 2013.02.21 IPC payload

   if (isFront)
   {
       NotifyFromUIStateManager( event );
   }
   else
   {
       NotifyFromUIStateManagerRear(event);
   }
}

void FileManagerEngine::setCountryChanged(ECountryVariant value)
{
    this->m_variantCounry = value;
    emit variantCountryChanged(this->m_variantCounry);
}

void FileManagerEngine::notifyDeleteVideoPlayer(bool isJukeBox, const QString &path, bool isFront)
{
    m_pDeletedFiles->typeSource = isJukeBox;
    const char *str = path.toAscii().data();

    snprintf(m_pDeletedFiles->mPathToDeletedFiles, sizeof(m_pDeletedFiles->mPathToDeletedFiles), str); 
    QByteArray messageData((char *) m_pDeletedFiles, sizeof(DeletedFiles));

    Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_ALL, TRACKS_DELETED, messageData );
    sendEventToUISH(postEvent, isFront);
}

bool FileManagerEngine::isBgState(bool isFront)
{
    if (isFront)
    {
        return this->frontScreenData.m_isBgState;
    }

    return this->rearScreenData.m_isBgState;
}

void FileManagerEngine::setPhotoFrame(const QString path, bool isFront)
{
    GetLogger().Log( "FileManagerEngine::setPhotoFrame: Start" );

    // { modified by eugene.seo 2013.03.25
    //Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_SETTINGS, path.toAscii() );
    Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_SETTINGS, path.toUtf8() );
    // } modified by eugene.seo 2013.03.25
    sendEventToUISH(postEvent, isFront);
}

void FileManagerEngine::launchPhotoGallery(bool isFront)
{
    GetLogger().Log( "FileManagerEngine::launchPhotoGallery: Start photo gallery" );

    Event event( EVT_KEY_SOFT_GALLERY, GetThisAppId(), APP_UI_MANAGER );
    sendEventToUISH(event, isFront);
}

void FileManagerEngine::launchVideoPlayer(bool isFront)
{
    GetLogger().Log( "FileManagerEngine::launchVideoPlayer: Start video player" );
    QString filePath = isFront ? m_filePath[CommonDef::FILEMANAGER_DISPLAY_FRONT] : m_filePath[CommonDef::FILEMANAGER_DISPLAY_REAR];

    //{ modified by jeeeun for not play video when video option menu pressed 
    //Event event( EVT_KEY_SOFT_VP, GetThisAppId(), APP_UI_MANAGER ); 
    //sendEventToUISH(event, isFront);
    //modified by edo.lee 2013.06.11
    if (currendDeviceIsJuke_front && isFront)
    {
        Event event( EVT_KEY_SOFT_AVP, GetThisAppId(), APP_UI_MANAGER, eAPP_AVP_VIDEO_MODE_JUKEBOX );
        sendEventToUISH(event, isFront);
    }
    else  if (currendDeviceIsJuke_rear && !isFront)
    {
        Event event( EVT_KEY_SOFT_AVP, GetThisAppId(), APP_UI_MANAGER, eAPP_AVP_VIDEO_MODE_JUKEBOX );
        sendEventToUISH(event, isFront);
    }
    //modified by edo.lee 2013.06.11
    else
    {
        // { modified by jeeeun 2013.01.17 for front rear USB video play
        // if(filePath.contains(USBFront)) // modified by Dmitry 22.05.13
        if(filePath.indexOf(GetFrontUSBPath()) == 0) // modified by kihyung 2013.12.09 for ITS 0213549 
        {
            Event event( EVT_KEY_SOFT_VP, GetThisAppId(), APP_UI_MANAGER, eAPP_AVP_VIDEO_MODE_USB1);
            sendEventToUISH(event, isFront);
        }
        // else if(filePath.contains(USBRear)) // modified by Dmitry 22.05.13
        else if(filePath.indexOf(GetRearUSBPath()) == 0) // modified by kihyung 2013.12.09 for ITS 0213549 
        {
            Event event( EVT_KEY_SOFT_VP, GetThisAppId(), APP_UI_MANAGER, eAPP_AVP_VIDEO_MODE_USB2);
            sendEventToUISH(event, isFront);
        }
        else
        {
            MP_CRITICAL << "FileManagerEngine::launchVideoPlayer() error " << LOG_ENDL;
        }
        // } modified by jeeeun 2013.01.17 for front rear USB video play
    }
    // } modified by jeeeun for not play video when video option menu pressed
}

void FileManagerEngine::checkFocusVisibility(bool isFront, bool visible)
{
    GetLogger().Log( "FileManagerEngine::checkFocusVisibility: check focus" );

    if (isFront)
    {
        this->frontScreenData.m_showFocusOnFg = visible;
    }
    else
    {
       this->rearScreenData.m_showFocusOnFg = visible;
    }

    GetLogger().Log( QString(
                       "FileManagerEngine::checkFocusVisibility: focus visibility is %1").arg(
                       visible)
                   );
}

void FileManagerEngine::renewFocus(bool isFront)
{
    bool result = (isFront) ? this->frontScreenData.m_showFocusOnFg :
                              this->rearScreenData.m_showFocusOnFg;

    if (result)
    {
       emit defaultFocus();
    }
}

bool FileManagerEngine::sendEventToUISH(Event &aEvent, bool isFront)
{
    bool result = false;
// removed by Dmitry 17.08.13

    #if USE_AFW_SWAP_DC
    if(isCommonTypeEvent(aEvent.GetEventId())) 
    {
        result = NotifyUISHCommon(aEvent);
    }
    else
    {
    #endif
        if (isFront)
        {
            result = NotifyUISH(aEvent, DISPLAY_FRONT);
        }
        else
        {
            result = NotifyUISH(aEvent, DISPLAY_REAR);
        }
    #if USE_AFW_SWAP_DC    
    }
    #endif

    return result;
}


bool 
FileManagerEngine::isCommonTypeEvent(EVT_ID_T aEventID)
{
    switch(aEventID)
    {
    case EVT_USB_MOUNTED:
    case EVT_USB_REMOVED:
    case EVT_APP_LAUNCH:
    case EVT_LANGUAGE_CHANGED:
    case EVT_DVD_MEDIA_INSERT:
    case EVT_RESOURCE_GRANTED:
    case EVT_RESOURCE_DENIED:
    case EVT_RESOURCE_RELEASED:
    case EVT_RESOURCE_RELEASED_BY_MANAGER:
    case EVT_RESOURCE_LOST:
    case EVT_RESOURCE_ERROR:
    case EVT_RESOURCE_AVAILABLE:
    case EVT_KEY_HU_BACK:
    case EVT_KEY_HU_MENU:
    case EVT_KEY_HU_TUNE:
    case EVT_KEY_HU_SEEK_NEXT:
    case EVT_KEY_HU_SEEK_PREV:
    case EVT_KEY_CCP_JOG_DIAL:
    case EVT_KEY_RRC_BACK:
    case EVT_KEY_RRC_MENU:
    case EVT_KEY_RRC_VOL_UP:
    case EVT_KEY_RRC_VOL_DOWN:
    case EVT_KEY_RRC_SEEK_NEXT:
    case EVT_KEY_RRC_SEEK_PREV:
    case EVT_KEY_RRC_MUTE:
    case EVT_KEY_RRC_JOG_DIAL:
    case EVT_KEY_SWRC_PTT:
    case EVT_KEY_SWRC_CALL_SEND:
    case EVT_KEY_SWRC_CALL_END:
    case EVT_KEY_SWRC_MUTE:
    case EVT_KEY_SWRC_VOL_UP:
    case EVT_KEY_SWRC_VOL_DOWN:
    case EVT_KEY_SWRC_SEEK_NEXT:
    case EVT_KEY_SWRC_SEEK_PREV:
    case EVT_KEY_SOFT_MENU:
    case EVT_KEY_HU_EJECT:
    case EVT_KEY_HU_TUNE_LEFT:
    case EVT_KEY_HU_TUNE_RIGHT:
    case EVT_KEY_HU_TUNE_PRESSED:
    case EVT_KEY_CCP_BACK:
    case EVT_KEY_CCP_MENU:
    case EVT_KEY_CCP_JOG_DIAL_TOP_LEFT:
    case EVT_KEY_CCP_JOG_DIAL_LEFT:
    case EVT_KEY_CCP_JOG_DIAL_DOWN_LEFT:
    case EVT_KEY_CCP_JOG_DIAL_UP:
    case EVT_KEY_CCP_JOG_DIAL_CENTER:
    case EVT_KEY_CCP_JOG_DIAL_DOWN:
    case EVT_KEY_CCP_JOG_DIAL_TOP_RIGHT:
    case EVT_KEY_CCP_JOG_DIAL_RIGHT:
    case EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT:
    case EVT_KEY_RRC_JOG_DIAL_TOP_LEFT:
    case EVT_KEY_RRC_JOG_DIAL_LEFT:
    case EVT_KEY_RRC_JOG_DIAL_DOWN_LEFT:
    case EVT_KEY_RRC_JOG_DIAL_UP:
    case EVT_KEY_RRC_JOG_DIAL_CENTER:
    case EVT_KEY_RRC_JOG_DIAL_DOWN:
    case EVT_KEY_RRC_JOG_DIAL_TOP_RIGHT:
    case EVT_KEY_RRC_JOG_DIAL_RIGHT:
    case EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT:
    case EVT_KEY_HU_JOG_DIAL_ENCODER:
    case EVT_KEY_CCP_JOG_DIAL_ENCODER:
    case EVT_KEY_RRC_JOG_DIAL_ENCODER:
    case EVT_DISC_STATUS:
    case EVT_NAVI_GPS_TIME_SET:
    case EVT_CLOCK_TIME_SET:
    case EVT_BT_INCOMING_CALL:
    case EVT_BT_CALL_ACCEPTED:
    case EVT_BT_CALL_REJECTED:
    case EVT_CAMERA_CLOSE:
    case EVT_KEY_SWRC_LONG_CALL_SEND:
    case EVT_START_AUDIO_PLAY_IN_BG:
    case EVT_BT_CALL_STATUS:
    case EVT_BT_CALL_ENDED:
    case EVT_BT_MUSIC_PLAY:
    case EVT_BT_MUSIC_STOP:
    case EVT_BT_MUSIC_PAUSE:
    case EVT_BT_MUSIC_PREV_TRACK:
    case EVT_BT_MUSIC_NEXT_TRACK:
    case EVT_BT_MUSIC_REPEAT_ALL_ON:
    case EVT_BT_MUSIC_REPEAT_ALL_OFF:
    case EVT_BT_MUSIC_REPEAT_ONE_ON:
    case EVT_BT_MUSIC_REPEAT_ONE_OFF:
    case EVT_BT_MUSIC_SHUFFLE_ON:
    case EVT_BT_MUSIC_SHUFFLE_OFF:
    case EVT_BT_MUSIC_SET_INFO:
    case EVT_BT_MUSIC_GET_MODE:
    case EVT_BT_MUSIC_CONN_CONFIG:
    case EVT_BT_MUSIC_ENABLED:
    case EVT_BT_MUSIC_DISABLED:
    case EVT_BT_MUSIC_PLAYBACK_STARTED:
    case EVT_BT_MUSIC_PLAYBACK_PAUSED:
    case EVT_TA_RDS_START:
    case EVT_TA_RDS_STOP:
    case EVT_DAB_REQUEST_RADIO:
    case EVT_RADIO_RESPONSE_DAB:
    case EVT_RADIO_END_REQUEST:
    case EVT_VIDEOPLAY_STATE:
    case EVT_ASPECTRATIO_CHANGE:
    case EVT_INFO_DRIVING_MODE:
    case EVT_INFO_VEHICLE_HEIGHT:
    case EVT_TA_DAB_START:
    case EVT_TA_DAB_END:
    case EVT_UPDATE_OSD_ON_TA:
    case EVT_DAB_CURRENT_PLAY:
    case EVT_DAB_STATUS_REQUEST:
    case EVT_RADIO_STATUS_RESPONSE:
    case EVT_KEY_AGREE_PRESSED:
    case EVT_KEY_HOME_MENU_LOCK:
    case EVT_KEY_HOME_MENU_UNLOCK:
    case EVT_BLUELINK_CALL_ONGOING:
    case EVT_BLUELINK_CALL_IDLE:
    case EVT_ENGINEERING_SETTING_DRIVINGRESTRICTION:
    case EVT_ENGINEERING_SETTING_REARCAMERASIGNAL:
    case EVT_KEY_RRC_CH_FW:
    case EVT_KEY_RRC_CH_BK:
    case EVT_EPG_DAB_START:
    case EVT_AV_MODE_CHANGE:
    case EVT_REQEUST_MUTE:
    case EVT_REQEUST_UNMUTE:
    case EVT_STATUSBAR_INFO_CHANGED:
    case EVT_SETTINGS_CHANGED:
    case EVT_ACC_OFF:
    case EVT_POPUP_HISTORY_RESPONSE:
    case EVT_REQUEST_VRTTS_VERSION:
    case EVT_RESPONSE_VRTTS_VERSION:
    case EVT_BT_PAIRING_STATUS:
    case EVT_GLOBAL_SEARCH_EVENT:
    case EVT_ALT_ON:
    case EVT_ALT_OFF:
    case EVT_REQ_ENG_MODE_ENTER:
    case EVT_TERMINATE_ENG_MODE:
    case EVT_DIAGNOSTIC_TEST_START:
    case EVT_DIAGNOSTIC_TEST_END:
    case EVT_FACTORY_RESET_REQUEST:
    case EVT_FACTORY_RESET_RESPONSE:
    case EVT_INITIAL_OK:
    case EVT_POWER_STATE_LOGIC_ON:
    case EVT_POWER_STATE_NORMAL_OFF:
    case EVT_MEDIA_ENABLED:
    case EVT_JUKEBOX_ENABLED:
    case EVT_USB1_ENABLED:
    case EVT_USB2_ENABLED:
    case EVT_IPOD1_ENABLED:
    case EVT_IPOD2_ENABLED:
    case EVT_MIRROR_LINK_ENABLED:
    case EVT_NAVI_INIT_GPS:
    case EVT_NAVI_INITIALIZE_REQUEST:
    case EVT_NAVI_INITIALIZE_RESPONSE:
    case EVT_NAVI_NOTIFY_CHANGE_ADMIN_DISTRICT:
    case EVT_NAVI_USER_FUEL_INFO:
    case EVT_NAVI_CPU_RESTRICTED:
    case EVT_NAVI_CPU_NORMAL:
    case EVT_REQUEST_NAVI_DISPLAY_STATUS:
    case EVT_RESPONSE_NAVI_DISPLAY_STATUS:
    case EVT_NAVI_SHARED_IMAGE_INITIALIZE:
    case EVT_NAVI_SHARED_IMAGE_UPDATE:
    case EVT_NAVI_GLOBAL_SEARCH_START:
    case EVT_NAVI_GLOBAL_SEARCH_RESPONSE:
    case EVT_NAVI_POI:
    case EVT_NAVI_SETTTING_UNIT:
    case EVT_NAVI_IBOX_POI:
    case EVT_NAVI_COMMAND:
    case EVT_NAVI_FULL_PATH:
    case EVT_NAVI_FULL_PATH_TPEG:
    case EVT_NAVI_OPEN_BROWSER_URL:
    case EVT_NAVI_SETTTING_PHONE_PRIORITY:
    case EVT_XM_REQUEST_NAVI:
    case EVT_NAVI_MANUAL_INPUT:
    case EVT_BT_HOST_BD_ADDRESS:
    case EVT_BT_DEV_BD_ADDRESS:
    case EVT_BT_CONNECTION_STATUS:
    case EVT_ML_REQUEST_BT_CONNECTION:
    case EVT_ML_AUDIO_REQUEST:
    case EVT_ML_AUDIO_START:
    case EVT_ML_AUDIO_STOP:
    case EVT_ML_STATUS_NOTIFY:
    case EVT_SUBMODE_CHANGED:
    case EVT_SWAP_DC_ENABLED:
    case EVT_SWAP_DC_DISABLED:
    case EVT_VR_INITIALIZE:
    case EVT_VR_INITIAL_OK:
    case EVT_VR_LIST_CREATE:
    case EVT_VR_END:
    case EVT_VR_COMMAND:
    case EVT_VR_MODE_CHANGED:
    case EVT_LIST_CREATE_START:
    case EVT_LIST_CREATE_END:
    case EVT_MIX_PLAY_START:
    case EVT_MIX_PLAY_STOP:
    case EVT_MIX_PLAY_END:
    case EVT_IPOD_MOUNTED:
    case EVT_IPOD_REMOVED:
    case EVT_AUX_PLUGEDIN:
    case EVT_AUX_REMOVED:
    case EVT_CDDVD_MOUNTED:
    case EVT_CDDVD_REMOVED:
    case EVT_IPOD_UDEV_DETECTED:
    case EVT_IPOD_UDEV_REMOVED:
    case EVT_IPHONE_UDEV_DETECTED:
    case EVT_IPHONE_UDEV_REMOVED:
    case EVT_CALL_START:
    case EVT_CALL_END:
    case EVT_REQUEST_UPGRADE:
    case EVT_RESPONSE_UPGRADE_CANCEL:
    case EVT_SYSTEM_OPTIONS:
    case EVT_SYSTEM_OPTIONS_SET:
    case EVT_NAVI_ENTRY_POINT_SETTINGS:
    case EVT_ENG_REQUEST_CONNECTIVITY:
    case EVT_ENG_RESPONSE_CONNECTIVITY:
    case EVT_REQUEST_SAVE_UNSAVED_DATA:
    case EVT_RESPONSE_SAVE_UNSAVED_DATA:
    case EVT_ALL_SLEEP_CANCEL_ACTIVATE:
    case EVT_RADIO_FREQUENCY_CHANGE:
    case EVT_DRS_ON:
    case EVT_DRS_OFF:
    case EVT_DWS_ON:
    case EVT_DWS_OFF:
    case EVT_REAR_GEAR_ENGAGED:
    case EVT_REAR_GEAR_REMOVED:
    case EVT_RESTORE_LAST_AV_MODE:
    case EVT_IGN_CHANGED:
    case EVT_ACC_CHANGED:
    case EVT_POWER_STATE_LOGIC_OFF:
    case EVT_POWER_STATE_NORMAL_ON:
    case EVT_REQUEST_ACC_STATUS:
    case EVT_REQUEST_IGN_STATUS:
    case EVT_RESPONSE_ACC_STATUS:
    case EVT_RESPONSE_IGN_STATUS:
    case EVT_NAVI_GPS_TIME_GET:
    case EVT_XSAMPA_AVAILABLE:
    case EVT_FORMAT_JUKEBOX_START:
    case EVT_FORMAT_JUKEBOX_END:
    case EVT_RSE_LOCK_STATUS:
    case EVT_BEEP_PLAY:
    case EVT_FM_LAUNCH_STATE:
    case EVT_PLAY_VIDEO:
    case EVT_SHOW_PHOTO:
    case EVT_AUTO_PLAY_IN_BG:
    case EVT_INITIALIZATION_REQUEST:
    case EVT_REQUEST_AUDIO_PATH:
    case EVT_ENABLE_DVDSETTINGS:
    case EVT_DISABLE_DVDSETTINGS:
    case EVT_REQUEST_DVD_INFO:
    case EVT_SHOW_DESKTOP_COMPLETE:
    case EVT_USB_PHOTO_STATUS:
    case EVT_VIDEO_MOVE_START:
    case EVT_VIDEO_MOVE_END:
    case EVT_SOUND_STATE_IBOX:
    case EVT_CAMERA_OFF_EVENT:
    case EVT_PANDORA_STATUS:
    case EVT_REQUEST_DIVX_REG_INFO:
    case EVT_RESPONSE_DIVX_REG_INFO:
    case EVT_VIDEO_FILE_SUPPORT: // added by jeeeun 2013.01.14 for 68428    
        return true;
    default:
        return false;
    }
}

void FileManagerEngine::notifyFilesChanged()
{
    // FM_LOG << "FileManagerEngine::notifyFilesChanged" << LOG_ENDL;

    Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_ALL, TRACKS_CHANGED);

    NotifyUISH( postEvent );
}
//added by edo.lee 2013.05.16
void FileManagerEngine::setOSDDisplay(bool isFront)
{
    emit signalSetDisplay( isFront ? DISPLAY_REAR : DISPLAY_FRONT);
}

// modified by Dmitry 17.08.13
void FileManagerEngine::notifyFormatJukeboxBegin()
{
    //MP_LOG << LOG_ENDL;
    Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_AVP, EVT_FORMAT_JUKEBOX_START);

    #if USE_AFW_SWAP_DC
    NotifyUISHCommon ( postEvent );
    #else
    NotifyUISH(postEvent);
    #endif
}
// modified by Dmitry 17.08.13

void FileManagerEngine::notifyFormatJukeboxEnd()
{
    // FM_LOG << "FileManagerEngine::notifyFormatJukeboxEnd"  << LOG_ENDL;
    //{modified by edo.lee 2013.02.15
    Event event(EVT_FORMAT_JUKEBOX_END, GetThisAppId(), APP_ALL);
    
    #if USE_AFW_SWAP_DC
    NotifyUISHCommon ( event );
    #else
    NotifyUISH(event);
    #endif

    //added by edo.lee 2013.02.18 for update main menu
    Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_AVP, EVT_FORMAT_JUKEBOX_END);

    #if USE_AFW_SWAP_DC
    NotifyUISHCommon ( postEvent );
    #else
    NotifyUISH(postEvent);
    #endif
    
    //}modified by edo.lee
}
// } added by lssanh 2012.09.17 for CR13482

bool FileManagerEngine::checkInUsbState()
{
    return m_currentStateTraking == eUsbVideo ||
            m_currentStateTraking == eUsbMusicTitle ||
            m_currentStateTraking == eUsbMusicArtist ||
            m_currentStateTraking == eUsbMusicAlbom ||
            m_currentStateTraking == eUsbVideo ||
            m_currentStateTraking == eUsbVideoCopy ||
            m_currentStateTraking == eUsbPictureShow ||
            m_currentStateTraking == eUsbPictureCopy ||
            m_currentStateTraking == eUsbListVideo ||
            m_currentStateTraking == eUsbListPicture ||
            m_currentStateTraking == eUsbMusicList;
}

QString FileManagerEngine::GetValidFilePath( QString originalFileName )
{
    //This functon is necessary when tracker returns urls not in percent encoding
    originalFileName.replace("%", "%25");
    originalFileName.replace("#", "%23");
    return QUrl(originalFileName).path().replace("%23", "#").replace("%25", "%");
}
// modified by edo.lee 2013.03.22
DISPLAY_T FileManagerEngine::getDisplayVisible()
{
   DISPLAY_T result = DISPLAY_NONE;

   if ( m_IsFrontVisible && m_IsRearVisible )
   {
      //result = DISPLAY_FRONT_REAR;      
   }
   else if ( m_IsFrontVisible )
   {
      //result = DISPLAY_FRONT;
      result = DISPLAY_REAR;
   }
    // { modified - ravikanth 05-03-13
   else if ( m_IsRearVisible )
   {
      //result = DISPLAY_REAR;
      result = DISPLAY_FRONT;
   }
   else
   {
       result = DISPLAY_FRONT_REAR;  
   }
    // } modified - ravikanth 05-03-13
   return result;
}
// modified by edo.lee 2013.03.22

void FileManagerEngine::setDisplayVisible( DISPLAY_T display, bool value )
{
   switch ( display )
   {
      case DISPLAY_FRONT:
      {
         m_IsFrontVisible = value;
      }
      break;

      case DISPLAY_REAR:
      {
         m_IsRearVisible = value;
      }
      break;

      case DISPLAY_FRONT_REAR:
      {
         m_IsFrontVisible = value;
         m_IsRearVisible = value;
      }
      break;
// modified by Dmitry 17.08.13
      default:
         break;
   }
}
//modified by edo.lee 2013.06.11
void FileManagerEngine::setJukeboxDevice(bool value, bool isFront) 
{
    MP_LOG << "setJukeboxDevice :: " << value << " " << isFront << LOG_ENDL;
    if(isFront)
        currendDeviceIsJuke_front = value;
    else
        currendDeviceIsJuke_rear = value;
}
//modified by edo.lee 2013.06.11
//{ add by eunhye 2012.09.21 for CR 9243
void FileManagerEngine::setMoveStatus(bool status)
{
   m_isMove = status;
   bool isFront = getDisplayVisible();
   if(m_isMove)
   {
      Event postevent( EVT_POST_EVENT, GetThisAppId(), APP_AVP, EVT_VIDEO_MOVE_END);
      sendEventToUISH(postevent, isFront);
   }
   else
   {
      Event postevent( EVT_POST_EVENT, GetThisAppId(), APP_AVP, EVT_VIDEO_MOVE_START);
      sendEventToUISH(postevent, isFront);
   }
}
//} add by eunhye 2012.09.21  CR 9243

// { added by junggil 2012.10.04 for CR13505 Media Apps - delta UIS 2.15 to 2.17 
void FileManagerEngine::handleSettingsCommand( EVENT_SETTINGS_CHANGED_DATA_T *eventData )
{
    // FM_LOG << ", sValue = " << eventData->sValue << LOG_ENDL;        // 1
    // FM_LOG << ", sKey = " << eventData->sKey << LOG_ENDL;                  // 68 DB_KEY_SCROLL_TICKER
    //{Changed by Alexey Edelev 2012.10.06 fix text scrolling
    if( eventData->sKey == AppSettingsDef::DB_KEY_SCROLL_TICKER ) {//Changed by Alexey Edelev 2012.10.10 fix for OBS build issues
        m_bScrollingTicker = (eventData->sValue == 1);
        //Added by Alexey Edelev 2012.10.06 fix text scrolling
        emit scrollingTickerChanged();
        //Added by Alexey Edelev 2012.10.06 fix text scrolling
    }
    //}Changed by Alexey Edelev 2012.10.06 fix text scrolling
}
// } added by junggil 


// { modified by Sergey 15.09.2013 for ITS#185233
void FileManagerEngine::drmConfirm(bool isFront)
{
    handlePlayVideo(m_drmFile.path, isFront);

    m_drmFile.isDRM = false;
    m_drmFile.type = -1;
    m_drmFile.limitCount = 0;
    m_drmFile.usedCount = 0;
    m_drmFile.path = "";
}


void FileManagerEngine::drmReset()
{
    m_drmFile.isDRM = false;
    m_drmFile.type = -1;
    m_drmFile.limitCount = 0;
    m_drmFile.usedCount = 0;
    m_drmFile.path = "";
}


void FileManagerEngine::handlePlayVideo(QString filePath, bool isFront)
{
    QString param(filePath);
    Event postevent(EVT_POST_EVENT, GetThisAppId(), APP_AVP, EVT_PLAY_VIDEO, param.toUtf8());
    sendEventToUISH(postevent, isFront);

    launchVideoPlayer(isFront);
}
// } modified by Sergey 15.09.2013 for ITS#185233


bool FileManagerEngine::isTAReadComplete(ETrackerAbstarctor_VolumeType volumeType)
{
    if(volumeType == eJukeBox) 
    {
        return m_bJukeboxTAReadComplete;
    }
    else if(volumeType == eUSB_FRONT)
    {
        return m_bUSB1TAReadComplete;
    }
    else if(volumeType == eUSB_REAR)
    {
        return m_bUSB2TAReadComplete;
    }
    return false; //added by aettie 2013.03.28 for Static defect 19484

}

// { modified by ravikanth 16-04-13
void FileManagerEngine::notifyCopyToJukeboxBegin(bool isFront, bool isVideo) // modified by eugene.seo 2013.05.29
{   
    setOSDDisplay( isFront);//added by edo.lee 2013.05.16
    Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_AVP, EVT_JUKEBOX_COPY_START,  QByteArray((char *)&isVideo, sizeof(isVideo))); // modified by eugene.seo 2013.05.29
    #if USE_AFW_SWAP_DC
    NotifyUISHCommon ( postEvent );
    #else
    NotifyUISH(postEvent);
    #endif
    setFileCopyOperationStatus(true);
    
    // modified by ravikanth 11-07-13 for ITS 0177070
    // event to UISH
    Event event( EVT_JUKEBOX_COPY_START, GetThisAppId(), APP_UI_MANAGER );
    #if USE_AFW_SWAP_DC
    NotifyUISHCommon ( event );
    #else
    NotifyUISH(event);
    #endif
}

// modified by ravikanth 12-07-13 for ITS 0174301
void FileManagerEngine::notifyCopyToJukeboxEnd(bool isVideo)
{
    Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_AVP, EVT_JUKEBOX_COPY_END, QByteArray((char *)&isVideo, sizeof(isVideo)));
    #if USE_AFW_SWAP_DC
    NotifyUISHCommon ( postEvent );
    #else
    NotifyUISH(postEvent);
    #endif
    setFileCopyOperationStatus(false);

    // modified by ravikanth 11-07-13 for ITS 0177070
    // event to UISH
    Event event( EVT_JUKEBOX_COPY_END, GetThisAppId(), APP_UI_MANAGER );
    #if USE_AFW_SWAP_DC
    NotifyUISHCommon ( event );
    #else
    NotifyUISH(event);
    #endif

    emit closeCopyCancelConfirmPopup(); // modified for ITS 0217509
}

void FileManagerEngine::CancelCopyJukebox()
{
    Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_AVP, EVT_FILEOPERATION_STOP);
    #if USE_AFW_SWAP_DC
    NotifyUISHCommon ( postEvent );
    #else
    NotifyUISH(postEvent);
    #endif
}

void FileManagerEngine::setFileCopyOperationStatus(bool isInProgress) // modified by kihyung 2013.07.15 for STATIC 20893
{
    m_bfileCopyOperationStatus = isInProgress;
}

void FileManagerEngine::cancelFileCopyOperation()
{
    setFileCopyOperationStatus(false);
    emit cancelFileOperation();
}
// } modified by ravikanth 16-04-13 

// added by Dmitry 11.05.13
void FileManagerEngine::setMiddleEast(bool east)
{
    if (m_bMiddleEast != east)
    {
       m_bMiddleEast = east;
       emit middleEastChanged();
    }
}
// added by Dmitry 11.05.13

// modified by ravikanth 29-06-13 for ITS 0176909
void FileManagerEngine::cancelCopyOnInstance(bool screenInstance)
{
    setFileCopyOperationStatus(false);
    emit cancelFileOperationOnInstance(screenInstance);
}

// added by Dmitry 21.08.13
void FileManagerEngine::connectReadComplete(bool bConnect)
{
   if (m_pTrackerAbs)
   {
      bConnect ? connect(m_pTrackerAbs, SIGNAL(ReadCompleted(ETrackerAbstarctor_VolumeType, int)), this, SIGNAL(readCompleted(ETrackerAbstarctor_VolumeType, int))) :
                 disconnect(m_pTrackerAbs, SIGNAL(ReadCompleted(ETrackerAbstarctor_VolumeType, int)), this, SIGNAL(readCompleted(ETrackerAbstarctor_VolumeType, int)));
   }
}
// added by Dmitry 21.08.13

void FileManagerEngine::setFilePath(const QString& filepath, bool isFront)
{
    if(isFront)
        m_filePath[CommonDef::FILEMANAGER_DISPLAY_FRONT] = filepath;
    else
        m_filePath[CommonDef::FILEMANAGER_DISPLAY_REAR] = filepath;
}

void FileManagerEngine::sendTouchCleanUpForApps()
{
    GetLogger().Log( " sendTouchCleanUpForApps()");
    SendTouchCleanUpForApps ();
}

void FileManagerEngine::clearALLJogKey()
{
    setJogKeyReleased(0xFFFFFFFF);
    emit ClearSharedData();
    GetLogger().Log("clearALLJogKey() key status " +getJogKeyPressed());
}

// { added by kihyung 2013.12.09 for ITS 0213549 
QString FileManagerEngine::GetFrontUSBPath()
{
    if(m_strUSB1Path.isEmpty()) {
        QFileInfo tFileInfo("/rw_data");
        if(tFileInfo.exists()) {
            m_strUSB1Path = "/rw_data/media/front_usb";
        }
        else {
            m_strUSB1Path = "/media/front_usb";
        }
    }
    return m_strUSB1Path;   
}

QString FileManagerEngine::GetRearUSBPath()
{
    if(m_strUSB2Path.isEmpty()) {
        QFileInfo tFileInfo("/rw_data");
        if(tFileInfo.exists()) {
            m_strUSB2Path = "/rw_data/media/rear_usb";
        }
        else {
            m_strUSB2Path = "/media/rear_usb";
        }
    }
    return m_strUSB2Path;
}
// } added by kihyung 2013.12.09 for ITS 0213549

void FileManagerEngine::MBeep()
{
    MP_LOG << LOG_ENDL;
    ManualBeep();
}
