#include "engine/DHAVN_AppMediaPlayer_EnginePhoto.h"
#include <DHAVN_AppFramework_Event_Def.h>
#include <DHAVN_AppFramework_AppEngineQML_Def.h>
#include <DHAVN_AppFileManager_Shared.h>
#include <QDeclarativeContext>
#include <QDeclarativeItem>
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "util/DHAVN_AppMediaPlayer_GraphicsOutput.h"
#include <DHAVN_AppSettings_Shared.h>
#include <QmlStatusBar/osdwriter.h> //modified by edo.lee 2013.04.04
#include <QCryptographicHash> // modified by ravikanth 08-04-13
#include "util/osd/OSD_Manager.h"

#define USE_NEW_SETTING_ENTRY_POINT     1  // modified by kihyung 2013.1.26

extern SharedDataProvider *m_pSharedPhotoData;

OPTION_MENU_T SLIDESHOW_MENU[]=
{
   { 200, QT_TR_NOOP("STR_MEDIA_SLIDE_SHOW_SETUP"), false, false, false, false, true, NULL, 0 },
   { 21, QT_TR_NOOP("STR_MEDIA_5SEC"), false, true, false, true, true, NULL, 0 }, // modified by ruindmby 2012.11.30 for CR#15766
   { 22, QT_TR_NOOP("STR_MEDIA_10SEC"), false, true, false, true, true, NULL, 0 },
   { 23, QT_TR_NOOP("STR_MEDIA_20SEC"), false, true, false, true, true, NULL, 0 },
   { 24, QT_TR_NOOP("STR_MEDIA_30SEC"), false, true, false, true, true, NULL, 0 }
};

OPTION_MENU_T PHOTO_MAIN_MENU_USB[]=
{
//modified by aettie 20130828 for ITS 184108
   { 100, "MENU", false, false, false, false, true, NULL, 0 },
   { 1, QT_TR_NOOP("STR_MEDIA_SLIDESHOW"), false, false, false, true, true, NULL, 0 },
   { 2, QT_TR_NOOP("STR_MEDIA_LIST"), false, false, false, true, true, NULL, 0 },
   //{ 3, QT_TR_NOOP("STR_MEDIA_ROTATION"), false, false, false, true, true, NULL, 0 }, //deleted by lssanh 2013.03.17 ISV76425 del rotation
   //{ 4, QT_TR_NOOP("STR_MEDIA_SAVE_FRAME"), false, false, false, true, true, NULL, 0 },
   { 5, QT_TR_NOOP("STR_MEDIA_MNG_COPY_TO_JUKEBOX"), false, false, false, true, true, NULL, 0 },
   //{ 6, QT_TR_NOOP("STR_MEDIA_FULL_SCREEN"), false, false, false, true, true, NULL, 0 },
   //{ 7, QT_TR_NOOP("STR_MEDIA_FILE_INFO"), false, false, false, true, true, NULL, 0 }, //deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox FileInfo deleted
   { 8, QT_TR_NOOP("STR_MEDIA_SLIDE_SHOW_SETUP"), false, false, false, true, true, SLIDESHOW_MENU, sizeof(SLIDESHOW_MENU)/sizeof(OPTION_MENU_T) },
   { 9, QT_TR_NOOP("STR_MEDIA_DISPLAY_SETTINGS"), false, false, false, true, true, NULL, 0 },
   { 6, QT_TR_NOOP("STR_MEDIA_FULL_SCREEN"), false, false, false, true, true, NULL, 0 }//moved by taihyun.ahn 2013.10.25
};

OPTION_MENU_T PHOTO_MAIN_MENU_JUKEBOX[]=
{
//modified by aettie 20130828 for ITS 184108
   { 100, "MENU", false, false, false, false, true, NULL, 0 },
   { 1, QT_TR_NOOP("STR_MEDIA_SLIDESHOW"), false, false, false, true, true, NULL, 0 },
   { 2, QT_TR_NOOP("STR_MEDIA_LIST"), false, false, false, true, true, NULL, 0 },
   //{ 3, QT_TR_NOOP("STR_MEDIA_ROTATION"), false, false, false, true, true, NULL, 0 }, //deleted by lssanh 2013.03.17 ISV76425 del rotation
   //{ 4, QT_TR_NOOP("STR_MEDIA_SAVE_FRAME"), false, false, false, true, true, NULL, 0 },
   //{ 6, QT_TR_NOOP("STR_MEDIA_FULL_SCREEN"), false, false, false, true, true, NULL, 0 },
   //{ 7, QT_TR_NOOP("STR_MEDIA_FILE_INFO"), false, false, false, true, true, NULL, 0 }, //deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox FileInfo deleted
   { 8, QT_TR_NOOP("STR_MEDIA_SLIDE_SHOW_SETUP"), false, false, false, true, true, SLIDESHOW_MENU, sizeof(SLIDESHOW_MENU)/sizeof(OPTION_MENU_T) },
   { 9, QT_TR_NOOP("STR_MEDIA_DISPLAY_SETTINGS"), false, false, false, true, true, NULL, 0 },
   { 6, QT_TR_NOOP("STR_MEDIA_FULL_SCREEN"), false, false, false, true, true, NULL, 0 }//moved by taihyun.ahn 2013.10.25
};

// added by ruindmby 2012.11.30 for CR#15766
OPTION_MENU_T SLIDESHOW_MENU_REAR[]=
{
   { 200, QT_TR_NOOP("STR_MEDIA_SLIDE_SHOW_SETUP"), false, false, false, false, true, NULL, 0 },
   { 21, QT_TR_NOOP("STR_MEDIA_5SEC"), false, true, false, true, true, NULL, 0 },
   { 22, QT_TR_NOOP("STR_MEDIA_10SEC"), false, true, false, true, true, NULL, 0 },
   { 23, QT_TR_NOOP("STR_MEDIA_20SEC"), false, true, false, true, true, NULL, 0 },
   { 24, QT_TR_NOOP("STR_MEDIA_30SEC"), false, true, false, true, true, NULL, 0 }
};

OPTION_MENU_T PHOTO_MAIN_MENU_USB_REAR[]=
{
//modified by aettie 20130828 for ITS 184108
   { 100, "MENU", false, false, false, false, true, NULL, 0 },
   { 1, QT_TR_NOOP("STR_MEDIA_SLIDESHOW"), false, false, false, true, true, NULL, 0 },
   { 2, QT_TR_NOOP("STR_MEDIA_LIST"), false, false, false, true, true, NULL, 0 },
   //{ 3, QT_TR_NOOP("STR_MEDIA_ROTATION"), false, false, false, true, true, NULL, 0 }, //deleted by lssanh 2013.03.17 ISV76425 del rotation
   //{ 4, QT_TR_NOOP("STR_MEDIA_SAVE_FRAME"), false, false, false, true, true, NULL, 0 },
   { 5, QT_TR_NOOP("STR_MEDIA_MNG_COPY_TO_JUKEBOX"), false, false, false, true, true, NULL, 0 },
   //{ 6, QT_TR_NOOP("STR_MEDIA_FULL_SCREEN"), false, false, false, true, true, NULL, 0 },
   //{ 7, QT_TR_NOOP("STR_MEDIA_FILE_INFO"), false, false, false, true, true, NULL, 0 }, //deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox FileInfo deleted
   { 8, QT_TR_NOOP("STR_MEDIA_SLIDE_SHOW_SETUP"), false, false, false, true, true, SLIDESHOW_MENU_REAR, sizeof(SLIDESHOW_MENU_REAR)/sizeof(OPTION_MENU_T) },
   { 9, QT_TR_NOOP("STR_MEDIA_DISPLAY_SETTINGS"), false, false, false, true, true, NULL, 0 },
   { 6, QT_TR_NOOP("STR_MEDIA_FULL_SCREEN"), false, false, false, true, true, NULL, 0 }//moved by taihyun.ahn 2013.10.25
};

OPTION_MENU_T PHOTO_MAIN_MENU_JUKEBOX_REAR[]=
{
//modified by aettie 20130828 for ITS 184108
   { 100, "MENU", false, false, false, false, true, NULL, 0 },
   { 1, QT_TR_NOOP("STR_MEDIA_SLIDESHOW"), false, false, false, true, true, NULL, 0 },
   { 2, QT_TR_NOOP("STR_MEDIA_LIST"), false, false, false, true, true, NULL, 0 },
   //{ 3, QT_TR_NOOP("STR_MEDIA_ROTATION"), false, false, false, true, true, NULL, 0 }, //deleted by lssanh 2013.03.17 ISV76425 del rotation
   //{ 4, QT_TR_NOOP("STR_MEDIA_SAVE_FRAME"), false, false, false, true, true, NULL, 0 },
   //{ 6, QT_TR_NOOP("STR_MEDIA_FULL_SCREEN"), false, false, false, true, true, NULL, 0 },
   //{ 7, QT_TR_NOOP("STR_MEDIA_FILE_INFO"), false, false, false, true, true, NULL, 0 }, //deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox FileInfo deleted
   { 8, QT_TR_NOOP("STR_MEDIA_SLIDE_SHOW_SETUP"), false, false, false, true, true, SLIDESHOW_MENU_REAR, sizeof(SLIDESHOW_MENU_REAR)/sizeof(OPTION_MENU_T) },
   { 9, QT_TR_NOOP("STR_MEDIA_DISPLAY_SETTINGS"), false, false, false, true, true, NULL, 0 },
   { 6, QT_TR_NOOP("STR_MEDIA_FULL_SCREEN"), false, false, false, true, true, NULL, 0 }//moved by taihyun.ahn 2013.10.25
};
// added by ruindmby 2012.11.30 for CR#15766

// modified by ruindmby 2012.11.30 for CR#15766
AppMediaPlayer_EnginePhoto::AppMediaPlayer_EnginePhoto( AppMediaPlayer_EngineMain &aEngineMain, bool isFrontDisplay )
: m_EngineMain( aEngineMain ),
  m_pTranslator( NULL ),
  m_PhotoController( aEngineMain, this ), // added by Sergey 22.05.2013
  m_nHideUI( true ),
  m_isDRSShow (false),/*added by yongkyun.lee 2012.12.03 for photo DRS from VR.*/
  m_pOptMenuModel( NULL ),
  m_unsupportedFile(false), // modified by ravikanth 15-05-13
  //changed by Alexey Edelev. Fix bug 13059. 2012.08.24
  //{
  m_isRunFromFileManager( false )
  //}
  //changed by Alexey Edelev. Fix bug 13059. 2012.08.24
  ,m_statusBarVisible(true),
  m_CurDisplay(PHOTO_DISPLAY_NONE), // added by ruindmby 2012.11.30 for CR#15766
  m_isCopy(false),// added by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list 
  m_isSwitchFromRestore(false), // added by kihyung 2013.3.19 for ISV 67749
  m_trackerUpdateInProgress(false), // modified by ravikanth 20-04-13
  m_bMirrorLayout(false), // modified by ravikanth 04-05-13
  m_bSeekForward(true), // added by Sergey 22.05.2013
  m_bPreviousDRSState(false), // modified by ravikanth 11-07-13 for ITS 0179160
  m_bPhotolistlunch(false), // modified by Dmitry 10.03.13 for ITS0187937
  m_bBackOnUserRequest(false)
{
  m_slideShowTimer.setInterval(5000);
  m_slideShowTimer.setSingleShot(false);
  connect(&m_slideShowTimer, SIGNAL(timeout()),this, SIGNAL(slideShowNext()));
   connect( m_EngineMain.GetMediaManager(),
            SIGNAL (deviceAttached( MEDIA_SOURCE_T, MEDIA_TYPE_T, bool )),
            this,
            SLOT (onDeviceAttached( MEDIA_SOURCE_T, MEDIA_TYPE_T, bool ))
          );
   connect( m_EngineMain.GetMediaManager(),
            SIGNAL (deviceRemoved( MEDIA_SOURCE_T, MEDIA_TYPE_T )),
            this,
            SLOT (onDeviceRemoved( MEDIA_SOURCE_T, MEDIA_TYPE_T ))
          );

   connect( &m_PhotoController, SIGNAL (showErrorPopup() ),
                          this, SIGNAL (showErrorPopup() )
          );

   //connect( &m_PhotoController, SIGNAL (handleAllUnsupportedFiles(int) ),
   //                       this, SIGNAL (handleAllUnsupportedFiles(int) ) // modified by ravikanth 28-04-13
   //       );
   connect( &m_PhotoController, SIGNAL (handleAllUnsupportedFiles(int) ),
                          this, SLOT (onHandleAllUnsupportedFiles(int) )
          );
// { removed by sangmin.seol ITS_0218558 RollBack DUAL_KEY for Photo.
//#ifdef DUAL_KEY
//     connect(this, SIGNAL(signalTransformEVT2QML_KEY(EVT_ID_T )), &m_EngineMain, SLOT(transformEVT2QML_KEY(EVT_ID_T)));
//#endif
// } removed by sangmin.seol ITS_0218558 RollBack DUAL_KEY for Photo.
   m_CurDisplay = isFrontDisplay ? PHOTO_DISPLAY_FRONT : PHOTO_DISPLAY_REAR;
   //memset(m_LastState, 0, PHOTO_DISPLAY_NONE * sizeof(PHOTO_LAST_STATE_T));

   qmlRegisterUncreatableType<OptionMenuModel>("OptionMenuModel", 1, 0, "OptionMenuModel", "OptionMenuModel Can not be created from qml");

//{Added by Alexey Edelev 2012.10.23 CR14638
   if(PHOTO_DISPLAY_FRONT == m_CurDisplay)
   {
       SetOptMenuModel(new OptionMenuModel(PHOTO_MAIN_MENU_JUKEBOX, sizeof(PHOTO_MAIN_MENU_JUKEBOX)/sizeof(OPTION_MENU_T)));
       connect(&m_EngineMain, SIGNAL(frontViewChanged()), this, SIGNAL(isActiveChanged()));
   }
   else
   {
       SetOptMenuModel(new OptionMenuModel(PHOTO_MAIN_MENU_JUKEBOX_REAR, sizeof(PHOTO_MAIN_MENU_JUKEBOX_REAR)/sizeof(OPTION_MENU_T)));
       connect(&m_EngineMain, SIGNAL(rearViewChanged()), this, SIGNAL(isActiveChanged()));
   }
//}Added by Alexey Edelev 2012.10.23 CR14638
//   connect(&m_EngineMain, SIGNAL(jukeboxCopyInProgressChanged(bool)), this, SIGNAL(jukeboxCopyInProgress())); // modified by ravikanth 20-04-13
   // connect( &m_PhotoController, SIGNAL (changePhotoFolder( PHOTO_SOURCES_LIST )), this, SLOT (changePhotoFolder(PHOTO_SOURCES_LIST))); // removed by kihyung 2013.2.17

   LoadSettings();

   // { removed by kihyung 2013.2.17
   // m_PhotoController.SetFolderAndFile(m_LastState[m_CurDisplay].currentFile[m_PhotoController.getCurSourceID()], PHOTO_SOURCE_JUKEBOX);
   // setCurrentSource(m_PhotoController.GetCurrentFileFullPathName());
   // } removed by kihyung 2013.2.17
   
   //OSDview = new QDeclarativeView(); //delete by ys 2013.09.30 //added by edo.lee 2012.12.08 for statusbar
   m_isFrontSoftBackPressed = false; //added by edo.lee 2013.03.30
   m_isRearSoftBackPressed = false; //added by edo.lee 2013.03.30
   
	// modified by ravikanth 28-04-13
   connect(&m_PhotoController, SIGNAL(filesInCurFolder(QStringList,int)), this, SLOT(updateFilesInCurFolder(QStringList,int))); // added by Sergey for ITS#159209 for 27.04.2013
   connect(&m_EngineMain, SIGNAL(retranslateUi(QVariant)), this, SIGNAL(folderNameChanged())); // modified for ITS 0215804
   connect(&m_EngineMain, SIGNAL(closeCopyCancelConfirmPopup()), this, SIGNAL(closeCopyCancelConfirmPopup())); // modified for ITS 0217509
}
// modified by ruindmby 2012.11.30 for CR#15766

// { removed by kihyung 2013.2.17
/*
void
AppMediaPlayer_EnginePhoto::createFSMonitorForSource(PHOTO_SOURCES_LIST source,const QString monitor_path)
{
   // MP_LOG << LOG_ENDL;
   m_pFSMonitor[source] = new AppMediaPlayer_FSMonitor(monitor_path, this);
   m_pFSMonitor[source]->setRecursiveMonitoring(false);

   connect( m_pFSMonitor[source], SIGNAL(contentChanged()),
            this, SLOT(onContentChanged()));

   QStringList nameFilter;
   // { modified by ruinseve for CR#12584
   // nameFilter << "*.jpg" << "*.png" << "*.bmp" << "*.gif"<< "*.tif"<< "*.tiff";
   nameFilter = m_PhotoController.GetControllers().getExtList(MEDIA_TYPE_PHOTO);
   // } modified by ruinseve for CR#12584.

   m_pFSMonitor[source]->setContentFilter(nameFilter);
   m_pFSMonitor[source]->runMonitoring();
}
*/
// } removed by kihyung 2013.2.17

// { removed by kihyung 2013.2.17
/*
void
AppMediaPlayer_EnginePhoto::onContentChanged()
{
   // MP_LOG << LOG_ENDL;
   AppMediaPlayer_FSMonitor *tmp = (AppMediaPlayer_FSMonitor*)sender();
   for (int index=0; index<PHOTO_SOURCE_MAX; index++ )
   {
      if ( tmp == m_pFSMonitor[index] )
      {
         // MP_LOG<<"[source]"<<index << LOG_ENDL;
         QString tmp_folder = m_PhotoController.GetFolderPathFromSource((PHOTO_SOURCES_LIST)index);
         m_PhotoController.SetFolder(tmp_folder ,(PHOTO_SOURCES_LIST)index);
      }
   }
}
*/
// } removed by kihyung 2013.2.17


// modified by ruindmby 2012.11.30 for CR# 16072
void
AppMediaPlayer_EnginePhoto::updateSource( int source_id )
{
   int source_mask = m_PhotoController.getSourceMask();
   int mask = 0;

   switch (source_id)
   {
      case PHOTO_SOURCE_JUKEBOX:
         mask = PHOTO_SOURCE_MASK_JUKEBOX;
         break;
      case PHOTO_SOURCE_USB_1:
         mask = PHOTO_SOURCE_MASK_USB1;
         break;
      case PHOTO_SOURCE_USB_2:
         mask = PHOTO_SOURCE_MASK_USB2;
         break;
      default:
         break;
   }

   // if source is the same but it's not present in source_mask anymore switch to jukebox
   if (source_id == m_PhotoController.getCurSourceID() && !(mask & source_mask))
   {
      source_id = PHOTO_SOURCE_JUKEBOX;
   }

   if (source_id != m_PhotoController.getCurSourceID())
   {
      if ( source_id == PHOTO_SOURCE_USB_1 || source_id == PHOTO_SOURCE_USB_2 )
      {
          NotifyUSBPhotoStatus( true );
      }
      else
      {
          NotifyUSBPhotoStatus( false );
      }

      emit photoUpdateSource( source_id, source_mask );
      m_PhotoController.changeCurSourceID( ( PHOTO_SOURCES_LIST ) source_id );

      // { added by wspark 2012.11.01 for SANITY_CM_AJ559
      if(m_PhotoController.isSourceEmpty((PHOTO_SOURCES_LIST)source_id))
      {
          emit noImageFile(source_id);
          // emit backtoPrev(source_mask); // commented by ravikanth 10-05-13
          return;
      }
      // } added by wspark
   }
   // added by minho 20121205 for removed tab on modearea
   else
   {
      emit photoUpdateSource( source_id, source_mask );
      m_PhotoController.changeCurSourceID( ( PHOTO_SOURCES_LIST ) source_id );

      if(m_PhotoController.isSourceEmpty((PHOTO_SOURCES_LIST)source_id))
      {
          emit noImageFile(source_id);
          // emit backtoPrev(source_mask); // commented by ravikanth 10-05-13
          return;
      }
   }

   // modified by ravikanth 18.06.13 for ITS 0173604 and SANITY_CM_AJ589
   QString strLastFile = m_LastState[m_CurDisplay].currentFile[m_PhotoController.getCurSourceID()];
   if(QFile::exists(strLastFile) != false)
   {
       // MP_LOG<< strLastFile << LOG_ENDL;
	   // modified by ravikanth 27-07-13 for ITS 181678
       bool indexFound = m_PhotoController.setPlaylistIndexWrtFile(strLastFile);
       if(!indexFound)
       {
           // image exist in disc but playlist not valid, get playlist
           QString folder = strLastFile.mid(0, strLastFile.lastIndexOf('/'));
           if(!folder.isEmpty())
           {
               m_LastState[m_CurDisplay].curFolder[m_PhotoController.getCurSourceID()] = folder;
               m_PhotoController.requestFilesInFolder(folder, m_PhotoController.getCurSourceID()); // count info is updated by response
           }
       }
       // added for ITS 190397
       else
       {
           QString folder = strLastFile.mid(0, strLastFile.lastIndexOf('/'));
           if(!folder.isEmpty())
           {
               m_LastState[m_CurDisplay].curFolder[m_PhotoController.getCurSourceID()] = folder;
           }
       }
   }
   // added by minho 

   SetCurModePlaylist(); // added by wspark 2013.03.26 for ISV 77091
}
// modified by ruindmby 2012.11.30 for CR# 16072

void
AppMediaPlayer_EnginePhoto::onDeviceAttached( MEDIA_SOURCE_T aSource, MEDIA_TYPE_T aType, bool autoplay )
{
    static int bFirst = false; // added by kihyung 2013.3.29 for performance
    
    if ( aType != MEDIA_TYPE_PHOTO ) return;

    MP_LOG << "source =" << aSource << "autoplay ="  << autoplay << LOG_ENDL;

    if ( aSource == MEDIA_SOURCE_JUKEBOX || aSource == MEDIA_SOURCE_USB1 || aSource == MEDIA_SOURCE_USB2 )
    {
        PHOTO_SOURCES_LIST source = (PHOTO_SOURCES_LIST)aSource;

        trytoRestoreDevice(source);

        if ( source == PHOTO_SOURCE_USB_1 )
        {
            m_PhotoController.addSource(PHOTO_SOURCE_MASK_USB1);
        }
        else if ( source == PHOTO_SOURCE_USB_2 )
        {
            m_PhotoController.addSource(PHOTO_SOURCE_MASK_USB2);
        }

        // { modified by kihyung 2013.3.29 for performance
        if( aSource != MEDIA_SOURCE_JUKEBOX || bFirst == true ) 
        {
            if( autoplay )
            {
                updateSource( ( int ) source );
            }
            else
            {
                updateSource(m_PhotoController.getCurSourceID());
            }
        }
        // } modified by kihyung 2013.3.29 for performance
    }

    // { modified by kihyung 2013.3.29 for performance
    if(bFirst == false) {
        bFirst = true;
    }
    // } modified by kihyung 2013.3.29 for performance    
}

void
AppMediaPlayer_EnginePhoto::onDeviceRemoved( MEDIA_SOURCE_T aSource, MEDIA_TYPE_T aType )
{
   if ( aType != MEDIA_TYPE_PHOTO ) return;

   // MP_LOG << "source =" << aSource << LOG_ENDL;

   PHOTO_SOURCES_LIST source = ( PHOTO_SOURCES_LIST )aSource;

   if( source < PHOTO_SOURCE_MAX )
   {
       /*
       if(m_pFSMonitor[source] != NULL)//Added by Alexey Edelev 2012.10.03
           m_pFSMonitor[source]->stopMonitoring();
       */
       SaveSettings(); // modified by ruindmby 2012.11.30 for CR#15766
       if((m_PhotoController.getCurSourceID() == source) || !isSlideShow()) // modified by ravikanth for ITS 0187343
       {
           m_PhotoController.ClearPlayList( source );
           m_LastState[m_CurDisplay].curFolder[m_PhotoController.getCurSourceID()].clear(); // added by Sergey 23.05.2013
           m_LastState[m_CurDisplay].filesInCurFolder[(PHOTO_SOURCES_LIST)source].clear(); // modified by ravikanth 10-05-13
           if(m_PhotoController.getCurSourceID() == source)
               m_source = ""; // added by Sergey 23.05.2013
       }
   }

   // this popup is displayed by UISH
   //show "USB removed" popup
    //{ added by yongkyun.lee 20130401 for : ISV 78014
    emit usbPluggedOut((int)source);
    //emit usbPluggedOut(); // added by lssanh 2013.03.17 ISV76423
    //} added by yongkyun.lee 20130401 

   //change source mask and tab
   if ( source == PHOTO_SOURCE_USB_1 )
   {
      m_PhotoController.removeSource( PHOTO_SOURCE_MASK_USB1 );
   }
   else if ( source == PHOTO_SOURCE_USB_2 )
   {
      m_PhotoController.removeSource( PHOTO_SOURCE_MASK_USB2 );
   }
   // { modified by ravikanth 16-03-13 for ISV 71121
//{modified by aettie.ji 2013.01.17 for ISV 68019
//   MP_LOG<<"Photo m_PhotoController.getSourceMask()"<<m_PhotoController.getSourceMask() << LOG_ENDL;
//   if (m_PhotoController.getSourceMask() == PHOTO_SOURCE_MASK_USB1_JUKEBOX)
//   {
//   	updateSource(PHOTO_SOURCE_USB_1);
//   }
//   else if (m_PhotoController.getSourceMask() == PHOTO_SOURCE_MASK_USB2_JUKEBOX)
//   {
//   	updateSource(PHOTO_SOURCE_USB_2);
//   }
//   else updateSource(PHOTO_SOURCE_JUKEBOX);
   //updateSource(PHOTO_SOURCE_JUKEBOX); // added by ruindmby 2012.11.30 for CR#15766
   //}modified by aettie.ji 2013.01.17 for ISV 68019
   // } modified by ravikanth 16-03-13

}

AppMediaPlayer_EnginePhoto::~AppMediaPlayer_EnginePhoto()
{
   delete m_pOptMenuModel;
   SetOptMenuModel(NULL);

   /*
   for ( unsigned index = 0; index < PHOTO_SOURCE_MAX; index++ )
   {
       if( m_pFSMonitor[index] != NULL ) //Added by Alexey Edelev 2012.10.03
           delete m_pFSMonitor[index];
   }
   */
}

void
AppMediaPlayer_EnginePhoto::ShowUI(DISPLAY_T display)
{
   Q_UNUSED(display);
   MP_HIGH << LOG_ENDL;
   //if(OSDview){		//delete by ys 2013.09.30
   	//OSDview->setWindowTitle("@OP^CLEAR_AV"); // modified by edo.lee 2013.03.14
   	//OSDWriter::SendOSD("@OSD:FR:CLEAR_AV"); //remove by edo.lee 2013.05.28
   	//QCoreApplication::flush(); //added by edo.lee 2013.01.16
        //}
   m_isHideScreen = false;//remove by edo.lee 2013.03.14
   setStatusBarVisiblePrivate(true);//added by edo.lee 2013.03.14
   setUIHidden( false );
   SetPhotoEntryPoint( m_EntryPoint );
   // { added by wspark 2012.11.01 for SANITY_CM_AJ559
   if(m_PhotoController.getCurSourceID() == PHOTO_SOURCE_JUKEBOX)
   {
       if(m_PhotoController.isSourceEmpty((PHOTO_SOURCES_LIST)PHOTO_SOURCE_JUKEBOX))
       {
               int source_mask = m_PhotoController.getSourceMask();
               emit noImageFile(PHOTO_SOURCE_JUKEBOX);
               emit backtoPrev(source_mask);
               return;
       }
   }
   // } added by wspark
   m_PhotoController.InitSources();
   emit photoRequestFG(true); // modified by ravikanth 11-07-13 for ITS 0179160

   // check loading on fg, if jukebox is formatted or delete all is called
   // loading is not reset after tracker callback as no valid control for jukebox
   emit jukeboxCopyInProgress(); // modified by ravikanth 30-04-13
   //added by edo.lee 2013.07.03
   QString curFilePath = m_EngineMain.getCurrnetFilePath(m_CurDisplay == PHOTO_DISPLAY_FRONT);
   if( curFilePath != "" && m_EngineMain.isDRSShow()) // modified by ravikanth 17-07-13 for Photo launch from list on clone
           syncImageFRonDRS(curFilePath);
  //added by edo.lee 2013.07.03
}

void
AppMediaPlayer_EnginePhoto::HideUI(DISPLAY_T display)
{
   MP_HIGH << LOG_ENDL;
   
   //if(OSDview){ //delete by ys 2013.09.30
   	//OSDview->setWindowTitle("@OP^CLEAR_AV"); // modified by edo.lee 2013.03.14
   	//OSDWriter::SendOSD("@OSD:FR:CLEAR_AV"); //remove by edo.lee 2013.04.26
   	//QCoreApplication::flush(); //added by edo.lee 2013.01.16
        //}
   // removed by Dmitry 23.07.13
   setStatusBarVisiblePrivate(true);
   m_isHideScreen = true; //added by edo.lee 2013.03.14
   setIsRunFromFileManager(false); //Added by Alexey Edelev 2012.09.15. CR13059
   setIsSlideShow(false); // added by lssanh 2013.03.17 ISV76423
   emit photoDRSModeOFF(); //added by edo.lee 2013.03.04
   setUIHidden( true ); // added by Dmitry 23.07.13
   m_isDRSShow = false; //added by edo.lee 2013.08.16 hotfix can not enter list

   if(m_bPhotolistlunch == true)
   {
        emit closeMenu();
        m_bPhotolistlunch = false;
   }

   if (m_bBackOnUserRequest == true)
   {
       emit defaulControlCue();
       m_bBackOnUserRequest = false;
   }
}

void *
AppMediaPlayer_EnginePhoto::GetUIState()
{
   return NULL;
}

QString
AppMediaPlayer_EnginePhoto::getCurrentFileNameByPath(const QString& fileFullPath)
{
   // MP_LOG << LOG_ENDL;
   QString retStr = "";

   QString strTmp = fileFullPath;
   if(!strTmp.isEmpty())
   {
      int nIndex = strTmp.lastIndexOf('/');
      retStr = strTmp.remove(0, nIndex + 1);
   }
   return retStr;
}
//added by edo.lee 2013.06.09
void
AppMediaPlayer_EnginePhoto::stopOtherViewSlideShow()
{
	MP_LOG << LOG_ENDL;
    AppMediaPlayer_EnginePhoto *pOther =
            (AppMediaPlayer_EnginePhoto*)m_EngineMain.GetEngine(m_CurDisplay == PHOTO_DISPLAY_FRONT ? APP_MEDIA_MODE_PHOTO_REAR:APP_MEDIA_MODE_PHOTO_FRONT);
    if(pOther)
        pOther->stopSlideShowForSource(m_PhotoController.getCurSourceID()); // modified for ITS 0197699
}
//added by edo.lee 2013.06.09
QString
AppMediaPlayer_EnginePhoto::getCurrentFileExtByPath(const QString& fileFullPath)
{
   // MP_LOG << LOG_ENDL;
   QString retStr = "";
   QString strTmp = fileFullPath;
   MP_LOG<<"[file] ="<< strTmp << LOG_ENDL;

   if(!strTmp.isEmpty())
   {
       int nIndex = strTmp.lastIndexOf('.');
       retStr = strTmp.remove(0, nIndex + 1);
   }
   return retStr;
}

QString
AppMediaPlayer_EnginePhoto::getCurrentFolderNameByPath(const QString& fileFullPath)
{
   QString retStr = "";

   QString strTmp = fileFullPath;
   //{added by aettie 2013.04.10 for ISV_KR 70943
    if(strTmp.startsWith("file://")){
        strTmp.remove(0, 7);
    }
   //}added by aettie 2013.04.10 for ISV_KR 70943
   if(!strTmp.isEmpty())
   {
       retStr = strTmp.section('/', -2, -2);
   }
	//{modified by aettie.ji 2013.01.31 for ISV 70943
   // { added by lssanh 2013.01.22 ITS149968
  /* if (retStr == "Picture" && strTmp.count('/') == 4)
   {
      retStr = "/";
   }*/
   // } added by lssanh 2013.01.22 ITS149968
   //modified by aettie 20130522 folder name for NP
	// if((retStr == "Picture" && strTmp.count('/') == 4)||((strTmp.section('/',-3,-3).contains("usb"))&&strTmp.count('/') == 4))
       if(  (retStr == "Picture" && strTmp.count('/') == 4)||
            (strTmp.section('/',-3,-3).contains("front_usb"))||
            (strTmp.section('/',-3,-3).contains("rear_usb"))   )
	   {
	      //retStr = "/";
		  //modified by aettie 2013 08 05 for ISV NA 85625
//	      retStr = "/..";
// modified by aettie 20130812 for ITS 183630
	      //retStr = "Root";
            retStr =QApplication::translate("main", QT_TR_NOOP(  "STR_MEDIA_ROOT_FOLDER"));
	   }
	   return retStr;
}
	//}modified by aettie.ji 2013.01.31 for ISV 70943
QString
AppMediaPlayer_EnginePhoto::getCurrentFilePath(const QString& fileFullPath)
{
   QString retStr = fileFullPath;
   if(!retStr.isEmpty())
   {
       retStr.truncate(retStr.lastIndexOf('/'));
   }
   return retStr;
}

// { added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
QString 
AppMediaPlayer_EnginePhoto::getCurrentFileCreatedDateByPath(const QString& fileFullPath)
{
   QString CreateDate = "";
   CreateDate = m_PhotoController.GetCurrentCreateDate(); 
   return (CreateDate != "") ? CreateDate : "Unknown";
}
// } added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup

// { modified by Sergey for ITS#159209 for 27.04.2013
int
AppMediaPlayer_EnginePhoto::GetCurrentPlayListCount()
{
    return m_LastState[m_CurDisplay].filesInCurFolder[m_PhotoController.getCurSourceID()].length();
}

int
AppMediaPlayer_EnginePhoto::GetCurrentIndex()
{
    QStringList filesInFolder = m_LastState[m_CurDisplay].filesInCurFolder[m_PhotoController.getCurSourceID()];
    QString curFile = m_LastState[m_CurDisplay].currentFile[m_PhotoController.getCurSourceID()];

    return filesInFolder.indexOf(curFile);
}
// } modified by Sergey for ITS#159209 for 27.04.2013

// { added by kihyung 2013.4.14
void 
AppMediaPlayer_EnginePhoto::SetNotifyAllUnsupported(int sourceID)
{
    MEDIA_SOURCE_T source;
    if(sourceID == PHOTO_SOURCE_JUKEBOX) {
        source = MEDIA_SOURCE_JUKEBOX;
    }
    else if(sourceID == PHOTO_SOURCE_USB_1) {
        source = MEDIA_SOURCE_USB1;
    }
    else if(sourceID == PHOTO_SOURCE_USB_2) {// modified by edo.lee 2013.07.15 STATIC_TEST
        source = MEDIA_SOURCE_USB2;
    }
    else {
        return;
    }

    MP_LOG << sourceID << source << LOG_ENDL;
    
    m_EngineMain.GetMediaManager()->SetSourceAvailable(source, MEDIA_TYPE_PHOTO, false);
    m_EngineMain.GetNotifier()->UpdateSDPMediaCount(source, MEDIA_TYPE_PHOTO, 0);
    m_EngineMain.GetNotifier()->setEnableMedia(source, MEDIA_TYPE_MAX);
}
// } added by kihyung 2013.4.14

void
AppMediaPlayer_EnginePhoto::SwitchMode(const int source_id)
{
    m_PhotoController.SwitchMode( (PHOTO_SOURCES_LIST)source_id ); // modified by ravikanth 04-05-13
	// commented by ravikanth 14-05-13
//    if(!m_PhotoController.isFileSupported(m_PhotoController.GetCurrentFileFullPathName()))
//    {
//        m_PhotoController.NextImage();
//    }
   OptionMenuUpdate( (PHOTO_SOURCES_LIST)source_id );
   setCurrentSource(m_PhotoController.GetCurrentFileFullPathName());
   emit playlistCountChanged();
   emit currentIndexChanged();
}

bool
AppMediaPlayer_EnginePhoto::isSwitchFromRestore()
{
    return m_isSwitchFromRestore;
}

void
AppMediaPlayer_EnginePhoto::SetCurModePlaylist()
{
// commented by ravikanth 14-05-13
//    if(!m_PhotoController.isFileSupported(m_PhotoController.GetCurrentFileFullPathName()))
//    {
//        m_PhotoController.NextImage();
//    }
   //m_PhotoController.SetCurModePlaylist(); // removed by kihyung 2013.2.17
   setCurrentSource(m_PhotoController.GetCurrentFileFullPathName(), true); // modified by ravikanth 12-07-13
   SaveSettings(); // added by wspark 2013.01.29 for ITS 151353
   emit playlistCountChanged();
   emit currentIndexChanged();
}

// {modified by Michael.kim 2013.10.11 for ITS 194857
void
AppMediaPlayer_EnginePhoto::HandleBackKey(bool onUserBack)
{

    Event event( EVT_KEY_SOFT_BACK, m_EngineMain.GetThisAppId(), APP_UI_MANAGER );
	//added by edo.lee 2013.03.21
	DISPLAY_T disp = DISPLAY_FRONT;
	if(m_EngineMain.getIsFrontBack() == false)
		disp = DISPLAY_REAR;
	//added by edo.lee 2013.03.21
	//modified by edo.lee 2013.04.07
	if(m_isDRSShow && m_CurDisplay == PHOTO_DISPLAY_FRONT)
		disp = DISPLAY_FRONT;
	else if(m_isDRSShow && m_CurDisplay == PHOTO_DISPLAY_REAR)
		disp = DISPLAY_REAR;
	//modified by edo.lee 2013.04.07
    MP_LOG<<disp << LOG_ENDL;
    emit NotifyUISH (event, disp);

    if( m_EngineMain.getCloneState() == CLONE_OFF )
        m_bBackOnUserRequest = onUserBack;
}

//modified by edo.lee 2013.03.30
//modified by aettie 20130620 for back key event
// }modified by Michael.Kim 2013.10.11 for ITS 194857

void
AppMediaPlayer_EnginePhoto::HandleSoftBackKey(bool isFront, bool bRRC)
{
    MP_LOG << LOG_ENDL;
	DISPLAY_T appDisp = DISPLAY_FRONT;
	if(!isFront)
		appDisp = DISPLAY_REAR;
	/*if(m_isBackPressByJog)
	{
		if(m_isFrontSoftBackPressed)
		{
			appDisp = DISPLAY_FRONT;
			m_isFrontSoftBackPressed = false;
		}else if(m_isRearSoftBackPressed)
		{
			appDisp = DISPLAY_REAR;
			m_isRearSoftBackPressed = false;
		}
		m_isBackPressByJog = false;
	}*/
            MP_LOG<<"Back_key :  m_isBackPressByJog"<<m_isBackPressByJog << LOG_ENDL;
            MP_LOG<<"Back_key :  m_isFrontSoftBackPressed"<<m_isFrontSoftBackPressed << LOG_ENDL;
            MP_LOG<<"Back_key :  m_isRearSoftBackPressed"<<m_isRearSoftBackPressed << LOG_ENDL;
            MP_LOG<<"Back_key :  bRRC"<<bRRC << LOG_ENDL;

    	/*if(m_isBackPressByJog)
	{
		if(m_isFrontSoftBackPressed)
		{
			appDisp = DISPLAY_FRONT;
			m_isFrontSoftBackPressed = false;
                    MP_LOG<<"Back_key : CCP EVT_SOFT_BACK_KEY m_isBackPressByJog"<<appDisp << LOG_ENDL;

		}
            else if(m_isRearSoftBackPressed)
		{
			appDisp = DISPLAY_REAR;
			m_isRearSoftBackPressed = false;
                    MP_LOG<<"Back_key : RRC EVT_SOFT_BACK_KEY m_isBackPressByJog"<<appDisp << LOG_ENDL;

		}*/
	if(m_isBackPressByJog)
	{
		if(!bRRC)
		{
			appDisp = DISPLAY_FRONT;
			m_isFrontSoftBackPressed = false;
			MP_LOG<<"Back_key : CCP EVT_SOFT_BACK_KEY m_isBackPressByJog"<<appDisp << LOG_ENDL;
		}
		else if(bRRC)
		{
			appDisp = DISPLAY_REAR;
			m_isRearSoftBackPressed = false;
			MP_LOG<<"Back_key : RRC EVT_SOFT_BACK_KEY m_isBackPressByJog"<<appDisp << LOG_ENDL;
		}
		m_isBackPressByJog = false;
		
		Event event( EVT_KEY_SOFT_BACK, m_EngineMain.GetThisAppId(), APP_UI_MANAGER );	
		emit NotifyUISH (event, appDisp);
	}
	else
	{
		MP_LOG<<"Back_key : EVT_TOUCH_BACK_KEY m_isBackPressByJog"<<appDisp << LOG_ENDL;
		Event event( EVT_TOUCH_BACK_KEY, m_EngineMain.GetThisAppId(), APP_UI_MANAGER );	
		emit NotifyUISH (event, DISPLAY_FRONT);
	}
}
//modified by aettie 20130620 for back key event
//modified by edo.lee 2013.03.30

bool
AppMediaPlayer_EnginePhoto::EventHandler( Event& aEvent, DISPLAY_T display )
{
	//added by edo.lee 2013.03.08
	if((m_CurDisplay == PHOTO_DISPLAY_FRONT && display == DISPLAY_REAR)
		|| (m_CurDisplay == PHOTO_DISPLAY_REAR && display == DISPLAY_FRONT))
		return false;
	//added by edo.lee
	
    switch( aEvent.GetEventId() )
    {
    case EVT_REQUEST_APP_EXIT:
        MP_LOG << "[EVT_REQUEST_APP_EXIT]" << LOG_ENDL; //Maybe Event process should be removed
        SaveSettings();
        break;

	case EVT_REQUEST_PRE_FG:	// modified by Sergey 07.09.2013 for PRE_FG supoort
	{
		MP_LOG << "[EVT_REQUEST_FG]" << LOG_ENDL;
		//{added by yongkyun.lee 2012.12.03 for photo DRS from VR.
		/*(m_isDRSShow)
		{
		emit photoDRSModeON();
		break;
		} */

		// { added by edo.lee 2013.02.04
         m_bPreviousDRSState = m_bPreviousDRSState ? false : m_isDRSShow; // modified by ravikanth 11-07-13 for ITS 0179160
		//if(display == DISPLAY_FRONT)
		{
			APP_ENTRY_POINT_DATA_T *pEventData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();
			switch (pEventData->sAppEntryPoint)
			{
				case eAPP_AVP_PHOTO_MODE_JUKEBOX_DRS:
					updateSource( (int)PHOTO_SOURCE_JUKEBOX ); //added by edo.lee 2013.06.08
					m_isDRSShow = true; 
					emit photoDRSModeON();
					break;
				case eAPP_AVP_PHOTO_MODE_USB1_DRS:
					updateSource( (int)PHOTO_SOURCE_USB_1 ); //added by edo.lee 2013.06.08
					m_isDRSShow = true; 
					emit photoDRSModeON();
					break;
				case eAPP_APP_PHOTO_MODE_USB2_DRS: 
					updateSource( (int)PHOTO_SOURCE_USB_2 ); //added by edo.lee 2013.06.08
					m_isDRSShow = true; 
					emit photoDRSModeON();
					break;
                 //{ added by yongkyun.lee 20130324 for : ISV 77066
                case eAPP_AVP_PHOTO_MODE_JUKEBOX:        
                    updateSource( (int)PHOTO_SOURCE_JUKEBOX );
                    m_isDRSShow = false;//added by edo.lee 2013.02.25
                    emit photoDRSModeOFF();
                    break;
                case eAPP_AVP_PHOTO_MODE_USB1:
                    updateSource( (int)PHOTO_SOURCE_USB_1 );
                    m_isDRSShow = false;//added by edo.lee 2013.02.25
                    emit photoDRSModeOFF();
                    break;
                case eAPP_AVP_PHOTO_MODE_USB2:   
                    updateSource( (int)PHOTO_SOURCE_USB_2 );
                    m_isDRSShow = false;//added by edo.lee 2013.02.25
                    emit photoDRSModeOFF();
                    break;
                //} added by yongkyun.lee 20130324 
				default:
					m_isDRSShow = false;//added by edo.lee 2013.02.25
			        emit photoDRSModeOFF();
				break;
			}
		}
		// } added by edo.lee 

		//}added by yongkyun.lee
		// modified by ravikanth 11-07-13 for ITS 0179160   
		// not an happy fix.. to remove changes this need to handled in UISH as ui need checks for DRS and normal state     
		if( (m_bPreviousDRSState == true && m_isDRSShow == false))// || (m_isDRSShow == true)) //mdofied by edo.lee 2013.08.20
			emit photoRequestFG(true);
		else
			emit photoRequestFG(false);
		break;
	}

	/*	// { added by edo.lee 2013.02.04
	case EVT_DRS_CHANGE:
	{
		if(display == DISPLAY_FRONT)
		{
			APP_ENTRY_POINT_DATA_T *pEventData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();
			switch (pEventData->sAppEntryPoint)
			{
				case eAPP_AVP_PHOTO_MODE_JUKEBOX_DRS:
				case eAPP_AVP_PHOTO_MODE_USB1_DRS:
				case eAPP_APP_PHOTO_MODE_USB2_DRS: 
					m_isDRSShow = true; //added by edo.lee 2013.02.25
					emit photoDRSModeON();
                     break;
				default:
					m_isDRSShow = false;//added by edo.lee 2013.02.25
                    emit photoDRSModeOFF();
					break;
			}
		}
		break;
	}
	// } added by edo.lee 


    case EVT_DRS_SHOW:
    {
        MP_LOG<<"[EVT_DRS_SHOW]" << LOG_ENDL;
        m_isDRSShow = true;//added by yongkyun.lee 2012.12.03 for photo DRS from VR.
        emit photoDRSModeON();
        break;
    }
    //{added by yongkyun.lee 2012.12.03 for photo DRS from VR.
    case EVT_DRS_HIDE:
    {
        m_isDRSShow = false;
        emit photoDRSModeOFF();
        break;
    }
    */
    //}added by yongkyun.lee 
    case EVT_VR_COMMAND:
        MP_LOG<<"[EVT_VR_COMMAND]" << LOG_ENDL;
        if ( !aEvent.Data().isEmpty() )
        {
            EVENT_EVT_VR_COMMAND_DATA_T *pData = (EVENT_EVT_VR_COMMAND_DATA_T*)aEvent.Data().data();
       
            //{added by yongkyun.lee 2012.12.03 for photo DRS from VR.
            if(m_isDRSShow)
            {
                emit photoDRSModeON();
                break;
            }
            //}added by yongkyun.lee 
            if ( pData->header.appIdReciever == APP_AVP)
            {
                switch(pData->commandId)
                {
                case AV_MODE_CHANGE:
                {
                    // MP_LOG<<"[commandId==AV_MODE CHANGE]" << LOG_ENDL;
                    // { modified by eugene 2012.08.23 for static analysis defect #6904 
                    //switch((int)(pData->sVRPayload.data))
                    switch((int)(pData->sVRPayload.data[0]))
                    // } modified by eugene 2012.08.23 for static analysis defect #6904 
                    {
                    case AV_MODE_USB_IMAGE:
                        updateSource( (int)PHOTO_SOURCE_USB_1 );
                        break;
                    case AV_MODE_JUKEBOX_IMAGE:
                        updateSource( (int)PHOTO_SOURCE_JUKEBOX );
                        break;
                    default:
                        break;
                    }
                    break;
                }
                default:
                    break;
                }
            }
        }
        break;

    case EVT_KEY_HU_MENU:
    case EVT_KEY_CCP_MENU:
    case EVT_KEY_RRC_MENU:
    {
        MP_LOG<<"[MENU_KEY]" << LOG_ENDL;
        KeyEventHandler( PHOTO_EVT_KEY_MENU, aEvent );
        break;
    }

    case EVT_KEY_SOFT_MENU:
    {
        emit photoKeyMenuReleased(false); // modified by dongjin 2012.11.23
        break;
    }

    case EVT_KEY_HU_BACK:
    case EVT_KEY_CCP_BACK:
    case EVT_KEY_RRC_BACK:
    {
        MP_LOG<<"[BACK_KEY]" << LOG_ENDL;
        KeyEventHandler( PHOTO_EVT_KEY_BACK, aEvent );
        break;
    }
	//addd by edo.lee 2013.03.30
//#ifndef DUAL_KEY  // removed by sangmin.seol ITS_0218558 RollBack DUAL_KEY for Photo.
	case EVT_KEY_CCP_JOG_DIAL_CENTER:
		if(m_isBackPressByJog)
			m_isFrontSoftBackPressed = true;
		break;
	case EVT_KEY_RRC_JOG_DIAL_CENTER:
		if(m_isBackPressByJog)
			m_isRearSoftBackPressed = true;
		break;
//#endif // removed by sangmin.seol ITS_0218558 RollBack DUAL_KEY for Photo.
	//addd by edo.lee 2013.03.30
// { removed by sangmin.seol ITS_0218558 RollBack DUAL_KEY for Photo.
//#ifdef DUAL_KEY
    /*case EVT_KEY_CCP_JOG_DIAL_LEFT:
    case EVT_KEY_CCP_JOG_DIAL_RIGHT:
    case EVT_KEY_CCP_JOG_DIAL_CENTER:
    case EVT_KEY_CCP_JOG_DIAL_UP:
    case EVT_KEY_CCP_JOG_DIAL_DOWN:
    {
        MP_LOG << "DUAL_KEY JOG forwarding to QML through signalJogNavigation" << "key = " << signalTransformEVT2QML_KEY(aEvent.GetEventId()) << LOG_ENDL;
        KEY_EVENT_DATA_T *ed = (KEY_EVENT_DATA_T*)aEvent.Data().data();
        emit signalJogNavigation(signalTransformEVT2QML_KEY(aEvent.GetEventId()),
                                  (ed->keyStatus)+1001, false, 1);
        break;
    }
    case EVT_KEY_RRC_JOG_DIAL_LEFT:
    case EVT_KEY_RRC_JOG_DIAL_RIGHT:
    case EVT_KEY_RRC_JOG_DIAL_CENTER:
    case EVT_KEY_RRC_JOG_DIAL_UP:
    case EVT_KEY_RRC_JOG_DIAL_DOWN:
    {
        MP_LOG << "DUAL_KEY RRC JOG forwarding to QML through signalJogNavigation" << "key = " << signalTransformEVT2QML_KEY(aEvent.GetEventId()) << LOG_ENDL;
        KEY_EVENT_DATA_T *ed = (KEY_EVENT_DATA_T*)aEvent.Data().data();
        emit signalJogNavigation(signalTransformEVT2QML_KEY(aEvent.GetEventId()),
                                  (ed->keyStatus)+1001, true, 1);
        break;
    }
    case EVT_KEY_CCP_JOG_DIAL_ENCODER:
    {
        if( (int)aEvent.GetCommand() > 0 )
        {
            emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                                      AppEngineQMLConstants::KEY_STATUS_PRESSED, false, 1);
            emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                                      AppEngineQMLConstants::KEY_STATUS_RELEASED, false, 1);
        }
        else
        {
            emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
                                      AppEngineQMLConstants::KEY_STATUS_PRESSED, false, 1);
            emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
                                      AppEngineQMLConstants::KEY_STATUS_RELEASED, false, 1);
        }
        break;
    }
    case EVT_KEY_RRC_JOG_DIAL_ENCODER:
    {
        if( (int)aEvent.GetCommand() > 0 )
        {
            emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                                      AppEngineQMLConstants::KEY_STATUS_PRESSED, true, 1);
            emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                                      AppEngineQMLConstants::KEY_STATUS_RELEASED, true, 1);
        }
        else
        {
            emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
                                      AppEngineQMLConstants::KEY_STATUS_PRESSED, true, 1);
            emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
                                      AppEngineQMLConstants::KEY_STATUS_RELEASED, true, 1);
        }
        break;
    }*/
//#endif
// } removed by sangmin.seol ITS_0218558 RollBack DUAL_KEY for Photo.
    case EVT_FORMAT_JUKEBOX_START:
    {
        MP_LOG<<"[EVT_FORMAT_JUKEBOX_START]" << LOG_ENDL;
//        if (m_PhotoController.getCurSourceID() == PHOTO_SOURCE_JUKEBOX )
//        {
//            m_PhotoController.Stop();
//        }
        // m_pFSMonitor[PHOTO_SOURCE_JUKEBOX]->stopMonitoring(); // added by eugeny.novikov 2012.11.22 for CR 15773
        break;
    }

    case EVT_FORMAT_JUKEBOX_END:
    {
        MP_LOG<<"[EVT_FORMAT_JUKEBOX_END]" << LOG_ENDL;
        m_PhotoController.ClearPlayList(PHOTO_SOURCE_JUKEBOX);
        break;
    }

    case EVT_POST_EVENT:
    {
        MP_LOG<<"[EVT_POST_EVENT]" << LOG_ENDL;
        if ( ( APP_FMGR == aEvent.GetSource() ) &&
           ( !aEvent.Data().isEmpty() ) &&
           ( aEvent.GetCommand() == EVT_SHOW_PHOTO ) )
        {
            // { modified by wspark 2012.10.30 for not showing image with korean folder/file name.
            //QString filePath( aEvent.Data().data() );
            QString filePath = filePath.fromUtf8(aEvent.Data().data());
            // } modified by wspark
            filePath.remove( "file://" );
            MP_LOG << "filePath" << filePath << LOG_ENDL;

            PlayTrackFromFileManager(filePath);
            return true;
        }
		//added by edo.lee 2013.02.15
		if ( ( APP_FMGR == aEvent.GetSource() ) &&
           ( aEvent.GetCommand() == EVT_FORMAT_JUKEBOX_START ) )
        {
        	MP_LOG<<"[EVT_FORMAT_JUKEBOX_START]" << LOG_ENDL;
        	// m_pFSMonitor[PHOTO_SOURCE_JUKEBOX]->stopMonitoring();
		}
		else if(( APP_FMGR == aEvent.GetSource() ) &&
           ( aEvent.GetCommand() == EVT_FORMAT_JUKEBOX_END ) )
		{
			MP_LOG<<"from FMGR [EVT_FORMAT_JUKEBOX_END]" << LOG_ENDL;
	        m_PhotoController.ClearPlayList(PHOTO_SOURCE_JUKEBOX);
		}
		//added by edo.lee
		// { modified by ravikanth 16-04-13
        else if ( ( APP_FMGR == aEvent.GetSource() ) &&
           ( aEvent.GetCommand() == EVT_JUKEBOX_COPY_START ) )
        {
		    MP_LOG<<"[EVT_JUKEBOX_COPY_START]" << LOG_ENDL;
		    // added by eugene.seo 2013.05.29
            bool isVideo = *(bool *)aEvent.Data().data();
            PHOTO_SOURCES_LIST  nSource = m_PhotoController.getCurSourceID();
            
			if(!isVideo && nSource == PHOTO_SOURCE_USB_1)				
            	m_EngineMain.setCopyInProgress(true, 1);
			else if(!isVideo && nSource == PHOTO_SOURCE_USB_2)
				m_EngineMain.setCopyInProgress(true, 2);
			else
				m_EngineMain.setCopyInProgress(true, 0);
			// added by eugene.seo 2013.05.29
        }
                else if(( APP_FMGR == aEvent.GetSource() ) &&
           ( aEvent.GetCommand() == EVT_JUKEBOX_COPY_END ) )
                {
                MP_LOG<<"from FMGR [EVT_JUKEBOX_COPY_END]" << LOG_ENDL;
				// modified by ravikanth 12-07-13 for ITS 0174301
                bool isVideo = *(bool *)aEvent.Data().data(); 
                if(!isVideo)
                {
                    emit copyCompleted();
                }
                 // m_EngineMain.setCopyInProgress(false, 0); // modified by eugene.seo 2013.05.29
                }
				// } modified by ravikanth 16-04-13
		// { added by kihyung 2013.2.17
		else if(APP_FMGR == aEvent.GetSource() && aEvent.GetCommand() == EVT_FM_MEDIA_SCAN_READ_END)
		{
            int *pData = (int*)aEvent.Data().data();
            ETrackerAbstarctor_VolumeType volumeType = (ETrackerAbstarctor_VolumeType)(*pData);
            m_EngineMain.completedTARead(volumeType, true);
		}
		// } added by kihyung 2013.2.17        
		// { added by kihyung 2013.2.17
		else if(APP_FMGR == aEvent.GetSource() && aEvent.GetCommand() == EVT_MAX + 1002)
		{
            int *pData = (int*)aEvent.Data().data();
            ETrackerAbstarctor_VolumeType volumeType = (ETrackerAbstarctor_VolumeType)(*pData);
            m_EngineMain.requestTrackerAbsRescan(volumeType);
            //Added by Puneet for not showing loading Popup on front video/Audio while copying photo in rear. 2013.06.02
            setRescanProgressLoading();
		}
		// } added by kihyung 2013.2.17        
        break;
    }
    case EVT_KEY_HU_TA:
    {
        MP_LOG<<"[EVT_KEY_HU_TA]" << LOG_ENDL;
        emit defaulControlCue();
        break;
    }

    // { remove by edo.lee 2013.01.30  AV OFF , display OSD
   /* case EVT_AV_MODE_CHANGE:
    {
        MP_LOG<<"[EVT_AV_MODE_CHANGE]" << LOG_ENDL;
    	if ( !aEvent.Data().isEmpty() )
    	{
    		//m_isHideScreen = true;//remove by edo.lee 2013.01.11
    		EVENT_MODE_CHANGED_DATA_T *pData = (EVENT_MODE_CHANGED_DATA_T*)aEvent.Data().data();

    		switch (pData->sMode)
    		{
    			case MODE_STATE_AVOFF:
    			{
    				if(m_EngineMain.GetLastPlayMode() == APP_MEDIA_MODE_VIDEO)
    				{
    					emit updateOSD( OSD_EVENT_VIDEO_AV_OFF );
    				}
    				else
    				{
    					emit updateOSD( OSD_EVENT_AUDIO_AV_OFF );
    				}
    			}
    		}	
    	}
    }*/
    // } added by edo.lee 

    default:
        break;
    }
    
    return false;
}

void
AppMediaPlayer_EnginePhoto::KeyEventHandler( PHOTO_EVT_KEY_T nEventKey, Event aEvent )
{
   // MP_LOG << LOG_ENDL;

   KEY_EVENT_DATA_T *pData = NULL;
   if ( !aEvent.Data().isEmpty() )
   {
      pData = (KEY_EVENT_DATA_T *)aEvent.Data().data();
   }

   if ( pData )
   {
      switch (nEventKey)
      {
         case PHOTO_EVT_KEY_MENU :
         {
//{ added by aettie 2013.08.01 for ITS 181900
           // if ( (KEY_STATUS_T)pData->keyStatus == KEY_STATUS_RELEASED )
            if ( (KEY_STATUS_T)pData->keyStatus == KEY_STATUS_PRESSED )
            {
               // MP_LOG<<"[KeyMenuReleased]" << LOG_ENDL;
               emit photoKeyMenuReleased(true); // modified by dongjin 2012.11.23
            }
            break;
         }
         case PHOTO_EVT_KEY_BACK :
         {
            if ( (KEY_STATUS_T)pData->keyStatus == KEY_STATUS_PRESSED )
            {
               // MP_LOG<<"[KeyBackPressed]" << LOG_ENDL;
               emit photoKeyBackPressed();
            }
            break;
         }
         default:
           break;
      }
   }
}

// modified by Dmitry 25.07.13
void
AppMediaPlayer_EnginePhoto::LaunchFileManager( QString folderPath )
{
   // MP_LOG << "folderPath:" << folderPath << "source_id:" << source_id << LOG_ENDL;

   AppFileManagerStartParameter param;
   param.startState = eStartInvalidSState;

   memcpy((void *)param.currentPlayFileName, (void *)getCurrentFileNameByPath(m_source).toUtf8().constData(), sizeof(param.currentPlayFileName));
   memcpy((void *)param.folderPath, (void *)folderPath.toUtf8().constData(), sizeof(param.folderPath)); 
   param.currentPlayFileName[sizeof(param.currentPlayFileName) - 1] = '\0';
   param.folderPath[sizeof(param.folderPath) - 1] = '\0';
     

   param.copyState = m_isCopy;// by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list 
   param.isCopyFileUnsupported = isUnsupportedFile(); // modified for ITS 0209355

   emit onMenuClosed();//added by edo.lee 2013.05.31

   DISPLAY_T disp = DISPLAY_REAR;
   if(m_CurDisplay == PHOTO_DISPLAY_FRONT)
			disp = DISPLAY_FRONT;

   if( m_CurDisplay == PHOTO_DISPLAY_REAR  && m_isDRSShow &&// added by edo.lee 2013.08.17 hotfix empty list on photo
       m_EngineMain.getIsSameDRSPhoto(m_CurDisplay == PHOTO_DISPLAY_FRONT, m_PhotoController.getCurSourceID()))
   {
      MP_LOG<<"getIsSameDRSPhoto true" << LOG_ENDL;
      disp = DISPLAY_FRONT;
   }

   // removed by Dmitry 03.10.13 for ITS0187937
   Event event( EVT_KEY_SOFT_FMGR, m_EngineMain.GetThisAppId(), APP_NONE );
   emit NotifyUISH(event, disp);

   if ( m_PhotoController.getCurSourceID() == (int)PHOTO_SOURCE_JUKEBOX)//jukebox
   {
       param.startState = eStartJukePicture;
       // MP_LOG<< "[folderPath]:"<< folderPath  << LOG_ENDL;
   }
   else if ( m_PhotoController.getCurSourceID() == (int)PHOTO_SOURCE_USB_1 ||
            m_PhotoController.getCurSourceID() == (int)PHOTO_SOURCE_USB_2  )
   {
       param.startState = eStartUsbPicture;
       // MP_LOG<< "[folderPath]:"<< folderPath  << LOG_ENDL;
   }

   // Post event to FileManager
   Event postEvent( EVT_POST_EVENT,
                    m_EngineMain.GetThisAppId(),
                    APP_FMGR,
                    EVT_FM_LAUNCH_STATE,
                    QByteArray( (char*)&param, sizeof( param ) ) ); // modified by lssanh 2013.02.21 IPC payload
   emit NotifyUISH (postEvent,disp);

   m_isCopy = false;// added by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list  
   m_bPhotolistlunch = true;
   MP_LOG<<disp << LOG_ENDL;
}
// modified by Dmitry 25.07.13

void
AppMediaPlayer_EnginePhoto::PlayTrackFromFileManager( QString strNewFilePath )
{
    MP_LOG << LOG_ENDL;
	// { modified by ravikanth 25-12-12
    setIsRunFromFileManager(true); // added by wspark 2012.09.20 for CR12999
	
    m_PhotoController.setCurrentFileSupported(strNewFilePath); // modified for ITS 0206895
    //if( !m_PhotoController.isFileSupported(strNewFilePath) )
    //{
    //    emit showErrorPopup();
    //    return;
    //}
	// } modified by ravikanth 25-12-12
	
    m_FMPreviouslyPlayedImage = m_PhotoController.GetCurrentFileFullPathName(); //Added by alexey edelev 2012.09.08. Fix bug 13059.

    //Added by Alexey Edelev 2012.09.07. CR13196
    //{
    int newSource = (int)getSourceByPath(strNewFilePath);
	
	// modified by ravikanth 29-06-13 for All playlist to Folder playlist changes
    if( m_FMPreviouslyPlayedImage.mid(0,m_FMPreviouslyPlayedImage.lastIndexOf('/')).remove("file://") != strNewFilePath.mid(0,strNewFilePath.lastIndexOf('/')).remove("file://") )
    {
        m_PhotoController.requestFilesInFolder(strNewFilePath.mid(0,strNewFilePath.lastIndexOf('/')), getSourceByPath(strNewFilePath));
    }

    if(newSource != PHOTO_SOURCE_MAX && newSource != m_PhotoController.getCurSourceID())
    {
        updateSource(newSource);
    }
    //}

    m_PhotoController.setPlaylistIndexWrtFile(strNewFilePath); // modified by Sergey for ITS#159209 for 27.04.2013
	// modified by ravikanth 27-07-13 for ITS 181678
//added by edo.lee 2013.07.03
    SetCurModePlaylist();	
        m_EngineMain.syncIndexFRPhotoEngine(m_CurDisplay == PHOTO_DISPLAY_FRONT , strNewFilePath, true, m_PhotoController.getCurSourceID());
}
void
AppMediaPlayer_EnginePhoto::syncImageFRonDRS(QString path, PHOTO_SOURCES_LIST source)
{
    MP_LOG << LOG_ENDL;
    if(m_PhotoController.getCurSourceID() == source)
    {
        m_FMPreviouslyPlayedImage = m_PhotoController.GetCurrentFileFullPathName();
        if( m_FMPreviouslyPlayedImage.mid(0,m_FMPreviouslyPlayedImage.lastIndexOf('/')).remove("file://") != path.mid(0,path.lastIndexOf('/')).remove("file://") )
        {
            m_PhotoController.requestFilesInFolder(path.mid(0,path.lastIndexOf('/')), getSourceByPath(path));
        }
        m_PhotoController.setPlaylistIndexWrtFile(path);
        SetCurModePlaylist();
    }
    else if(source != PHOTO_SOURCE_MAX)
    {
        //MP_LOG << path << LOG_ENDL;
        m_LastState[m_CurDisplay].currentFile[source] = path;
    }
}
//added by edo.lee 2013.07.03
//Added by alexey edelev 2012.09.08. Fix bug 13059.
//{
bool
AppMediaPlayer_EnginePhoto::restorePreviouslyPlayedImage()
{
    if(!m_FMPreviouslyPlayedImage.isEmpty())
    {
        int     nIndex    = m_FMPreviouslyPlayedImage.lastIndexOf('/');
        QString strFolder = m_FMPreviouslyPlayedImage.mid(0, nIndex);      
        
        // m_PhotoController.requestLastFolder(m_PhotoController.getCurSourceID(), strFolder, m_FMPreviouslyPlayedImage); // removed by Sergey for ITS#159209 for 27.04.2013
        
        SetCurModePlaylist();
        m_FMPreviouslyPlayedImage.clear();
        return true;
    }
    return false;
}
//}
//Added by alexey edelev 2012.09.08. Fix bug 13059.

//Added by Alexey Edelev 2012.09.07. CR13196
//{
PHOTO_SOURCES_LIST
AppMediaPlayer_EnginePhoto::getSourceByPath(const QString& filePath)
{
    if(filePath.startsWith("/usr_data/media/Picture"))
    {
        return PHOTO_SOURCE_JUKEBOX;
    }

    if(!m_EngineMain.GetMediaManager()->GetMountPath((MEDIA_SOURCE_T)PHOTO_SOURCE_USB_1).isEmpty() && filePath.startsWith(m_EngineMain.GetMediaManager()->GetMountPath((MEDIA_SOURCE_T)PHOTO_SOURCE_USB_1))) //changed by Alexey Edelev 2012.09.15. CR13059
    {
        return PHOTO_SOURCE_USB_1;
    }

    if(!m_EngineMain.GetMediaManager()->GetMountPath((MEDIA_SOURCE_T)PHOTO_SOURCE_USB_2).isEmpty() && filePath.startsWith(m_EngineMain.GetMediaManager()->GetMountPath((MEDIA_SOURCE_T)PHOTO_SOURCE_USB_2))) //changed by Alexey Edelev 2012.09.15. CR13059
    {
        return PHOTO_SOURCE_USB_2;
    }

    return PHOTO_SOURCE_MAX;
}
//}
//Added by Alexey Edelev 2012.09.07. CR13196

// { removed by kihyung 2013.2.17
/*
void
AppMediaPlayer_EnginePhoto::trytoRestoreJukebox()//try to Restore Jukebox
{
   if ( false == m_PhotoController.SetFolderAndFile( m_LastState[m_CurDisplay].currentFile[PHOTO_SOURCE_JUKEBOX], PHOTO_SOURCE_JUKEBOX ) ) // modified by ruindmby 2012.11.30 for CR#15766
   {
      // MP_LOG<<"[restore default folder]" << LOG_ENDL;
      m_PhotoController.SetFolder( DEFAULT_JUKEBOX_PATH, PHOTO_SOURCE_JUKEBOX );
   }
}
*/

void
AppMediaPlayer_EnginePhoto::trytoRestoreDevice(const PHOTO_SOURCES_LIST restored_source)
{
    MP_LOG << restored_source << LOG_ENDL;
 
    MEDIA_SOURCE_T media_source = (MEDIA_SOURCE_T)restored_source;
 
    if ( m_EngineMain.GetMediaManager()->IsSourceAvailable(media_source, MEDIA_TYPE_PHOTO) == false )
    {
        MP_LOG << "media source is not available" << media_source << LOG_ENDL;
        return;
    }
 
    QString strLastFile = m_LastState[m_CurDisplay].currentFile[restored_source];

    MP_LOG << "last file:" << strLastFile << LOG_ENDL;

	// { modified by ravikanth 29-06-13 for All playlist to Folder playlist changes
	// { modified by Sergey for ITS#159209 for 27.04.2013
    if(QFile::exists(strLastFile) == false) 
    {
        MP_LOG << "file not found" << LOG_ENDL;
        setIsSwitchFromRestore(true);
        m_PhotoController.requestFirstFolder(restored_source);
    }
    else
    {
        m_PhotoController.requestFilesInFolder(strLastFile.mid(0,strLastFile.lastIndexOf('/')), restored_source);
    }

    //m_PhotoController.requestAllFiles(restored_source);
	// } modified by Sergey for ITS#159209 for 27.04.2013
	// } modified by ravikanth 29-06-13 for All playlist to Folder playlist changes
}

void
AppMediaPlayer_EnginePhoto::LoadSettings()//Load setting from QSettings to Photo Conteroller
{
   // MP_LOG << LOG_ENDL;

   // modified by ruindmby 2012.11.30 for CR#15766
   AppMediaPlayer_SettingsStorage::GetInstance()->LoadPhotoData(m_LastState, m_CurDisplay); // modified by kihyung 2013.07.26 for ITS 0181599 
   if (m_LastState[m_CurDisplay].slideshowDelay <= 5) m_LastState[m_CurDisplay].slideshowDelay = 5; // added by ruindmby 2012.12.12
   // modified by ravikanth 27-07-13 for ITS 181678
   setCurrentDisplaySlideShowDelay(m_LastState[m_CurDisplay].slideshowDelay);

//   OPTION_MENU_T* model;

//   if (PHOTO_DISPLAY_FRONT == m_CurDisplay)
//   {
//      model = SLIDESHOW_MENU;
//   }
//   else
//   {
//      model = SLIDESHOW_MENU_REAR;
//   }
//   switch (m_LastState[m_CurDisplay].slideshowDelay)
//   {
//   case 5:
//      model[1].bSelected = true;
//      break;
//   case 10:
//      model[2].bSelected = true;
//      break;
//   case 20:
//      model[3].bSelected = true;
//      break;
//   case 30:
//      model[4].bSelected = true;
//      break;
//   default:
//      model[1].bSelected = true;
//      break;
//   }
   // modified by ruindmby 2012.11.30 for CR#15766

   // { removed by kihyung 2013.2.17
   /* 
   trytoRestoreJukebox();
   trytoRestoreUSB(PHOTO_SOURCE_USB_1);
   trytoRestoreUSB(PHOTO_SOURCE_USB_2);
   */
   // } removed by kihyung 2013.2.17
}

// modified by kihyung 2013.08.09 for ITS 0183077
void
AppMediaPlayer_EnginePhoto::SaveSettings(bool bSync)
{
 	AppMediaPlayer_SettingsStorage::GetInstance()->SavePhotoSettings(m_LastState, m_CurDisplay);
}

void AppMediaPlayer_EnginePhoto::Restore(MODE_STATE_T mode)
{
    Q_UNUSED(mode);
}

void AppMediaPlayer_EnginePhoto::setUIHidden( bool value )
{
   m_nHideUI = value;
   emit photoUIHidden();
}

//returns false if file already exists
bool AppMediaPlayer_EnginePhoto::SaveAsFrame( const bool replace )
{
    // { modified by kihyung 2013.08.14 for ITS 0184288
    QDir saveFolder(DEFAULT_SAVEFRAME_PATH);
    if(!saveFolder.exists(QString("saveframe")))
    {
        if(!saveFolder.mkdir(QString("saveframe")))
        {
            return true;
        }
    }

    QString strSrcName = m_PhotoController.GetCurrentFileFullPathName();
    QFile srcfile(strSrcName);

    int     nSize       = strSrcName.size();
    int     nIndex      = strSrcName.lastIndexOf(".");
    QString strFormat   = strSrcName.mid(nIndex, nSize - nIndex).toLower();

    QString dstFileName1 = QString(DEFAULT_SAVEFRAME_PATH) + QString("/saveframe/") + QString(DEFAULT_SAVEFRAME_FILE1) + strFormat;
    QString dstFileName2 = QString(DEFAULT_SAVEFRAME_PATH) + QString("/saveframe/") + QString(DEFAULT_SAVEFRAME_FILE2) + strFormat;
    QString dstFileName;

    if(QFile::exists(dstFileName1) == true) {
        QFile::remove(dstFileName1);
        dstFileName = dstFileName2;
    }
    else {
        QFile::remove(dstFileName2);
        dstFileName = dstFileName1;
    }
    
    if(QFile::exists(dstFileName))
    {
        if(!QFile::remove(dstFileName))
        {
            MP_HIGH << "remove error" << LOG_ENDL;
            return false;    
        }
    }

    if(!srcfile.copy(dstFileName))
    {
        MP_HIGH << "copy error" << LOG_ENDL;
        return false;
    }
    
    Event event(EVT_POST_EVENT, m_EngineMain.GetThisAppId(), APP_SETTINGS, dstFileName.toUtf8());
    emit NotifyUISH( event );

	OSDManager::instance()->displayOSD(OSD_EVENT_COPY_COMPLETED, eAPP_AVP_PHOTO_MODE_JUKEBOX);
    return true;
    // } modified by kihyung 2013.08.14 for ITS 0184288    
}

void AppMediaPlayer_EnginePhoto::LaunchScreenSettings()
{
    MP_LOG << LOG_ENDL;
    // { modified by kihyung 2013.1.26
#if USE_NEW_SETTING_ENTRY_POINT   
    APP_EVENT_DATA_BASE_T pEventData;
    pEventData.sIsJogEnabled = false;

    Event softEvent(EVT_KEY_SOFT_SETTINGS, m_EngineMain.GetThisAppId(), APP_UI_MANAGER, eAPP_SETTINGS_SCREEN,
                    QByteArray((char *)&pEventData, sizeof(pEventData))); // modified by lssanh 2013.02.21 IPC payload
    if(m_CurDisplay == PHOTO_DISPLAY_FRONT)
        emit NotifyUISH(softEvent, DISPLAY_FRONT);
    else
        emit NotifyUISH(softEvent, DISPLAY_REAR);
#else
    AppSettingsStartParameter startState;
    startState.startState = AppSettingsDef::eStart_Mode_Screen;
    startState.transparentBG = false;
 
    Event postEvent(EVT_POST_EVENT, m_EngineMain.GetThisAppId(), APP_SETTINGS, QByteArray((char *) &startState, sizeof(startState)));
    emit NotifyUISH(postEvent);
    
    Event event(EVT_KEY_SOFT_SETTINGS, m_EngineMain.GetThisAppId(), APP_UI_MANAGER);
    emit NotifyUISH( event );
#endif   
    // } modified by kihyung 2013.1.26
}

// { modified by wspark 2012.11.20 for CR15521
// bool AppMediaPlayer_EnginePhoto::CopyImageToJukebox()
bool AppMediaPlayer_EnginePhoto::CopyImageToJukebox(bool replace)
// } modified by wspark
{
    MP_LOG << LOG_ENDL;
    QFile cur_file( m_PhotoController.GetCurrentFileFullPathName() );

    //copy current file to the Jukebox root
    QString new_name = DEFAULT_JUKEBOX_PATH;
    new_name.append( "/" );
    new_name.append( getCurrentFileNameByPath(m_source));

    MP_LOG << "new file name = " << new_name << LOG_ENDL;

   // { added by wspark 2012.11.20 for CR15521
   QFile new_file( new_name );
   if ( new_file.exists() )
   {
       if(replace)
       {
           MP_LOG << "remove existing file" << LOG_ENDL;
           if ( !new_file.remove() )
           {
              MP_LOG << "failed to remove file, error = " << new_file.error() << LOG_ENDL;
           }
       }
       else
       {
         MP_LOG << "file already exist!!" << LOG_ENDL;
         return false;
       }
   }
   // } added by wspark
    if ( false == cur_file.copy( new_name ) )
    {
        MP_LOG << "copy failed, error = " << cur_file.error() << LOG_ENDL;
        return false;
    }
    return true;
}

void AppMediaPlayer_EnginePhoto::setStatusBarVisible( bool visible )
{
    if( visible != m_statusBarVisible) {
        m_statusBarVisible = visible;
        setStatusBarVisiblePrivate(visible);
        emit statusBarVisibleChanged();
    }
}

void AppMediaPlayer_EnginePhoto::setStatusBarVisiblePrivate( bool visible )
{
	//modified by edo.lee 2013.04.04
	/*MP_LOG << LOG_ENDL;
   	// { added by edo.lee 2012.12.05 for statusbar	
	#if STATUSBAR_NEW

	// { modified by edo.lee 2013.02.04
	//if(m_EngineMain.GetFrontView() == APP_MEDIA_MODE_PHOTO_FRONT)
	if(m_CurDisplay == PHOTO_DISPLAY_FRONT && m_EngineMain.GetFrontView() == APP_MEDIA_MODE_PHOTO_FRONT)
		OSDview = m_EngineMain.GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
	//else if (m_EngineMain.GetRearView() == APP_MEDIA_MODE_PHOTO_REAR)
	else if(m_CurDisplay == PHOTO_DISPLAY_REAR && m_EngineMain.GetRearView() == APP_MEDIA_MODE_PHOTO_REAR)
		OSDview = m_EngineMain.GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
	// } modified by edo.lee
	
	QString info = "";
	info = visible ? "@OP^SSHOW" : "@OP^SHIDE"; //modified by edo.lee 2013.02.01
	MP_LOG<<OSDview<<"OSD String "<<info << LOG_ENDL;
	if(OSDview){
		//OSDview->setWindowTitle(info);
		OSDWriter::SendOSD("@OP^CLEAR_AV"); //modified by edo.lee 2013.04.04
		//QCoreApplication::flush(); //added by edo.lee 2013.01.16
		}
	
	#else
	// } added by edo.lee
   EVT_ID_T evt_id = ( visible ? EVT_SHOW_STATUS_BAR : EVT_HIDE_STATUS_BAR );
   Event event( evt_id, m_EngineMain.GetThisAppId(), APP_STATUS_BAR );
   emit NotifyUISH( event );
   #endif// added by edo.lee 2012.12.05 for statusbar	
   */
	//modified by edo.lee 2013.04.04
}

void AppMediaPlayer_EnginePhoto::SetPhotoEntryPoint( APP_ENTRY_POINT_T entryPoint )
{
   // MP_LOG << LOG_ENDL;
   switch( entryPoint )
   {
      case eAPP_AVP_PHOTO_MODE_JUKEBOX:
	  case eAPP_AVP_PHOTO_MODE_JUKEBOX_DRS: //added by edo.lee 2013.03.04
         updateSource( ( int ) PHOTO_SOURCE_JUKEBOX );
         break;

      case eAPP_AVP_PHOTO_MODE_USB1:
	  case eAPP_AVP_PHOTO_MODE_USB1_DRS://added by edo.lee 2013.03.04
         updateSource( ( int ) PHOTO_SOURCE_USB_1 );
         break;

      case eAPP_AVP_PHOTO_MODE_USB2:
	  case eAPP_APP_PHOTO_MODE_USB2_DRS://added by edo.lee 2013.03.04
         updateSource( ( int ) PHOTO_SOURCE_USB_2 );
         break;
// added by ruindmby 2012.11.30 for CR# 16072
      case eAPP_MODE_LAST_STATE:
         updateSource( ( int ) m_PhotoController.getCurSourceID() );
         break;
// added by ruindmby 2012.11.30 for CR# 16072
      default:
        break;
   }
}

void AppMediaPlayer_EnginePhoto::SetOptMenuModel( OptionMenuModel *optMenuModel )
{
    if(m_pOptMenuModel != optMenuModel)
    {
        m_pOptMenuModel = optMenuModel;
        emit optMenuModelChanged();
    }
}
void AppMediaPlayer_EnginePhoto::photoMenuUpdate( int source )
{
	OptionMenuUpdate( (PHOTO_SOURCES_LIST) source );
}

// modified by ruindmby 2012.11.30 for CR#15766
void AppMediaPlayer_EnginePhoto::OptionMenuUpdate( PHOTO_SOURCES_LIST source )
{
   // MP_LOG << "source = " << source << LOG_ENDL;

   OPTION_MENU_T *pOptMenuStruct;
   int size = 0;

   if (PHOTO_DISPLAY_FRONT == m_CurDisplay)
   {
      pOptMenuStruct = PHOTO_MAIN_MENU_JUKEBOX;
      size = sizeof(PHOTO_MAIN_MENU_JUKEBOX)/sizeof(OPTION_MENU_T);
   }
   else
   {
      pOptMenuStruct = PHOTO_MAIN_MENU_JUKEBOX_REAR;
      size = sizeof(PHOTO_MAIN_MENU_JUKEBOX_REAR)/sizeof(OPTION_MENU_T);
   }


   switch ( source )
   {
      case PHOTO_SOURCE_USB_1:
      case PHOTO_SOURCE_USB_2:
         if (PHOTO_DISPLAY_FRONT == m_CurDisplay)
         {
            pOptMenuStruct = PHOTO_MAIN_MENU_USB;
            size = sizeof(PHOTO_MAIN_MENU_USB)/sizeof(OPTION_MENU_T);
         }
         else
         {
            pOptMenuStruct = PHOTO_MAIN_MENU_USB_REAR;
            size = sizeof(PHOTO_MAIN_MENU_USB_REAR)/sizeof(OPTION_MENU_T);			
         }		 
		   pOptMenuStruct[1].bEnabled = m_isDRSShow == false ;//added by edo.lee 2013.06.08
		  // pOptMenuStruct[7].bEnabled = m_isDRSShow == false ;//remove by edo.lee 2013.07.25
          //pOptMenuStruct[3].bEnabled = !isUnsupportedFile(); // modified for ITS 0196040
         break;

	case PHOTO_SOURCE_JUKEBOX:		
			pOptMenuStruct[1].bEnabled = m_isDRSShow == false ;//added by edo.lee 2013.06.08
		//	pOptMenuStruct[6].bEnabled = m_isDRSShow == false ;//remove by edo.lee 2013.07.25
			break;
      default:	  	
         break;
   }


   if ( m_pOptMenuModel )
   {
      m_pOptMenuModel->updateModel( pOptMenuStruct, size );
   }
   else
   {
      // MP_LOG << "m_pOptMenuModel is NULL" << LOG_ENDL;
   }
}
// modified by ruindmby 2012.11.30 for CR#15766

// { added and modified by junggil 2012.10.05 for SANITY_CM_AJ497 
QString AppMediaPlayer_EnginePhoto::makeElidedString( const QString &fullString, const QString &fontFamily, int pixelSize, int maxWidth )
{
    QFont mFont;
    mFont.setFamily( fontFamily );
    //mFont.setPixelSize( pixelSize );
    mFont.setPointSize( pixelSize );	//modified by aettie.ji 2012.11.28 for uxlaunch update

    QFontMetrics fontMetrics( mFont );

    return fontMetrics.elidedText( fullString, Qt::ElideRight, maxWidth);
}

int AppMediaPlayer_EnginePhoto::getStringWidth( const QString &fullString,
                                                      const QString &fontFamily,
                                                      int pixelSize )
{
    QFont mFont;
    mFont.setFamily(fontFamily);
    //mFont.setPixelSize(pixelSize);
    mFont.setPointSize(pixelSize);	//modified by aettie.ji 2012.11.28 for uxlaunch update

    QFontMetrics fontMetrics(mFont);

    return fontMetrics.width(fullString, -1);
}

// { added by dongjin 2012.10.17 for CR13581
void AppMediaPlayer_EnginePhoto::NotifyUSBPhotoStatus( bool USBPhotoStatus )
{
    MP_LOG << USBPhotoStatus << LOG_ENDL;
    
    EVENT_USB_PHOTO_STATUS_DATA message = { { APP_ALL, EVT_USB_PHOTO_STATUS, 0, 0 }, USBPhotoStatus };

    message.header.dataSize = sizeof( message );
    Event event( EVT_USB_PHOTO_STATUS, m_EngineMain.GetThisAppId(), APP_UI_MANAGER, QByteArray( (char*)&message, sizeof( message ) ) ); // modified by lssanh 2013.02.21 IPC payload

    emit NotifyUISH( event );
}
// } added by dongjin

// modified by ruindmby 2012.11.30 for CR#15766
bool AppMediaPlayer_EnginePhoto::getIsActive()
{
    if(PHOTO_DISPLAY_FRONT == m_CurDisplay)
    {
        return (m_EngineMain.GetFrontView() == APP_MEDIA_MODE_PHOTO_FRONT);
    }
    else
    {
        return (m_EngineMain.GetRearView() == APP_MEDIA_MODE_PHOTO_REAR);
    }
}
// modified by ruindmby 2012.11.30 for CR#15766

// { added by wspark 2012.11.01 for SANITY_CM_AJ559
void AppMediaPlayer_EnginePhoto::HandleNoImageInJukebox()
{

    if(m_PhotoController.isSourceEmpty((PHOTO_SOURCES_LIST)m_PhotoController.getCurSourceID()))
    {
        HandleBackKey();
    }
}
// } added by wspark

void AppMediaPlayer_EnginePhoto::save(qreal angle)
{
    QString tmpSource = currentSource().path(); // modified by eugene.seo 2013.02.18
    QImage image(tmpSource);
    if(image.isNull()) {
        MP_LOG << " Error: " << tmpSource << LOG_ENDL;
        //error here
        return;
    }
    QTransform rotation;
    rotation.rotate(angle);
    image = image.transformed(rotation);
    image.save(tmpSource, 0, 100);
    //reload image with updated rotation
    setCurrentSource("");
    setCurrentSource(tmpSource);
}

void AppMediaPlayer_EnginePhoto::skip()
{
    if(m_bSeekForward)
        next();
    else
        previous();
} // added by Sergey 22.05.2013

void AppMediaPlayer_EnginePhoto::next()
{
    m_bSeekForward = true; // added by Sergey 22.05.2013
    m_PhotoController.NextImage(); // modified by ravikanth 15-05-13
    setIsRunFromFileManager(false);
    setCurrentSource(m_PhotoController.GetCurrentFileFullPathName());
    SaveSettings(); // added by wspark 2013.01.29 for ITS 151353
    emit currentIndexChanged();
	// modified by ravikanth 27-07-13 for ITS 181678
    //m_EngineMain.syncIndexFRPhotoEngine(m_CurDisplay == PHOTO_DISPLAY_FRONT , m_PhotoController.GetCurrentFileFullPathName(), false);//added by edo.lee 2013.07.03
    m_EngineMain.SetPhotoIndexOnOtherEngine(m_PhotoController.getCurSourceID(), m_PhotoController.GetCurrentIndex(), m_CurDisplay == PHOTO_DISPLAY_FRONT);
}

void AppMediaPlayer_EnginePhoto::previous()
{
    m_bSeekForward = false; // added by Sergey 22.05.2013
    m_PhotoController.PrevImage(); // modified by ravikanth 15-05-13
    setIsRunFromFileManager(false);
    setCurrentSource(m_PhotoController.GetCurrentFileFullPathName());
    SaveSettings(); // added by wspark 2013.01.29 for ITS 151353
    emit currentIndexChanged();
	// modified by ravikanth 27-07-13 for ITS 181678
        //m_EngineMain.syncIndexFRPhotoEngine(m_CurDisplay == PHOTO_DISPLAY_FRONT , m_PhotoController.GetCurrentFileFullPathName(), false);//added by edo.lee 2013.07.03
    m_EngineMain.SetPhotoIndexOnOtherEngine(m_PhotoController.getCurSourceID(), m_PhotoController.GetCurrentIndex(), m_CurDisplay == PHOTO_DISPLAY_FRONT);
}
//added by edo.lee 2013.07.03
QString AppMediaPlayer_EnginePhoto::GetCurrentFilePath()
{
	return m_PhotoController.GetCurrentFileFullPathName();
}
//added by edo.lee 2013.07.03
void AppMediaPlayer_EnginePhoto::setIsSlideShow(bool value)
 {
 	MP_LOG << LOG_ENDL;
	//if(m_isDRSShow) return;//added by edo.lee 2013.02.25 //removed by eunhye 2013.03.26 for ISV 72423 
    setIsRunFromFileManager(false);
    if(m_slideShowTimer.isActive() != value) {
        if(value) {
            m_slideShowTimer.start();
            //emit slideShowNext(); // deleted by lssanh 2013.03.16 ISV76255
        }
        else {
            m_slideShowTimer.stop();
        }
        emit isSlideShowChanged();
    }
}

// added by ruindmby 2012.11.30 for CR#15766
// modified by ravikanth 27-07-13 for ITS 181678
void AppMediaPlayer_EnginePhoto::setSlideShowDelay(int delay)
{
    //MP_LOG << LOG_ENDL;
    m_EngineMain.setSlideShowDelay(delay);
}

void AppMediaPlayer_EnginePhoto::setCurrentDisplaySlideShowDelay(int delay)
{
   //MP_LOG<<delay << LOG_ENDL;
   if(m_slideShowDelay != delay)
   {
      m_slideShowDelay = delay;
      m_slideShowTimer.setInterval(delay * 1000);
      m_LastState[m_CurDisplay].slideshowDelay = delay;
      emit slideShowDelayChanged();
   }
   setSlideShowDelayToOptions();
}

// { added by Sergey for ITS#159209 for 27.04.2013
void AppMediaPlayer_EnginePhoto::updateFilesInCurFolder(QStringList fileList, int mediaSource) // modified by ravikanth 28-04-13
{
    if(fileList.isEmpty())
    {
        MP_LOG << "Emtry file. Return." << LOG_ENDL;
        return;
    }
	// modified by ravikanth 01-05-13
    m_LastState[m_CurDisplay].filesInCurFolder[(PHOTO_SOURCES_LIST)mediaSource].clear();
    m_LastState[m_CurDisplay].filesInCurFolder[(PHOTO_SOURCES_LIST)mediaSource].append(fileList);
	// modified by ravikanth 27-07-13 for ITS 181678
    m_PhotoController.setPlaylistIndexWrtFile(m_LastState[m_CurDisplay].currentFile[(PHOTO_SOURCES_LIST)mediaSource]);
    emit playlistCountChanged();
}
// } added by Sergey for ITS#159209 for 27.04.2013

void AppMediaPlayer_EnginePhoto::setCurrentSource(QString source, bool onPlaylistSet) // modified by ravikanth 12-07-13
{
   if (m_source != source)
   {
      m_source = source;
      STRING_DATA_TYPE_T shDataType = (m_PhotoController.getCurSourceID() == PHOTO_SOURCE_USB_1) ? STRING_DATA_TYPE_PLAYING_FILE_USB1
                : ((m_PhotoController.getCurSourceID() == PHOTO_SOURCE_USB_2) ? STRING_DATA_TYPE_PLAYING_FILE_USB2 : STRING_DATA_TYPE_PLAYING_FILE); // modified for ITS 0217689
      // MP_LOG << "setCurrentSource" << shDataType <<" src: "<< source << LOG_ENDL;
      m_pSharedPhotoData->setStringData(shDataType, source); // modified by Dmitry 10.03.13 for ITS0187937

	  // { added by Sergey for ITS#159209 for 27.04.2013
      // For updating count info
      QString folder = source.mid(0, source.lastIndexOf('/'));

      if((folder != m_LastState[m_CurDisplay].curFolder[m_PhotoController.getCurSourceID()])
              || (GetCurrentPlayListCount() <= 0 )) // modified by ravikanth 14-05-13
      {
          m_LastState[m_CurDisplay].curFolder[m_PhotoController.getCurSourceID()] = folder;
          m_PhotoController.requestFilesInFolder(folder, m_PhotoController.getCurSourceID()); // count info is updated by response
      }
	  // } added by Sergey for ITS#159209 for 27.04.2013

      m_LastState[m_CurDisplay].currentFile[m_PhotoController.getCurSourceID()].clear();
      m_LastState[m_CurDisplay].currentFile[m_PhotoController.getCurSourceID()].append(source);
	  // commented by ravikanth 30-04-13
      // m_unsupportedFile = (m_PhotoController.currentLoadingFileSize() > 5) ? true : false; // changed by ravikanth 07-04-13
	  // { modified by ravikanth 15-05-13
      if(m_PhotoController.isFileSupported(QString("")))
      {
          m_unsupportedFile = false;
          //emit showImageLoading();
          m_PhotoController.stopUnsupportedSkipTimer(); // modified by ravikanth 29-06-13
      }
      else
      {
          m_unsupportedFile = true;
          //emit showImageLoading();
          m_PhotoController.handleUnsupportedFile(source); // added by Sergey 22.05.2013

      }
      emit currentSourceChanged();
	  // } modified by ravikanth 15-05-13
   }
   // modified by ravikanth 12-07-13 for Back screen fix after USB Remove and insert while Unsupporthed image is playing
   else if(onPlaylistSet)
   {
       // on playlist set and current source is not changed, inform qml about unsupported state
       if(m_PhotoController.isFileSupported(QString("")))
       {
           m_unsupportedFile = false;
           m_PhotoController.stopUnsupportedSkipTimer(); // modified by ravikanth 29-06-13
       }
       else
       {
           m_unsupportedFile = true;
           m_PhotoController.handleUnsupportedFile(source); // added by Sergey 22.05.2013
        }
       emit currentSourceChanged();
   }
   if(onPlaylistSet)
       emit folderNameChanged(); // modified for ITS 0215804
   // m_PhotoController.GetPhotoMetaDataInfo(source); // removed by kihyung
}
// added by ruindmby 2012.11.30 for CR#15766

// { modified by ravikanth 02-04-13
 QUrl AppMediaPlayer_EnginePhoto::currentSource()
 { 
 	// @ todo: change this implementation logic to proper place by avoiding m_source to be empty
	// added this check to avoid undesired results when loading image
     if(!m_source.isEmpty())
        return QUrl::fromLocalFile(m_source);
     else
         return QUrl("");
 }
 // } modified by ravikanth 02-04-13

// modified by ravikanth 13.06.2013 for ITS 0172957
 bool AppMediaPlayer_EnginePhoto::isHighResolutionImage()
 {
     return m_PhotoController.isHighResolutionImage();
 }

// { modified by ravikanth 08-04-13
 QString AppMediaPlayer_EnginePhoto::getThumbnailName(const QString &image)
 {
     QString thumbnailName = QString(QCryptographicHash::hash(image.toUtf8(), QCryptographicHash::Md5).toHex())
             + QString(".jpg");
     thumbnailName.prepend(m_PhotoController.relativeThumbnailPath());
     MP_LOG << "image: " << image << " thumbnailName: " << thumbnailName << LOG_ENDL;

     QFile thumbnailFileName(thumbnailName);
     if(QFile::exists(thumbnailName) && thumbnailFileName.size()>0 )
     {
         MP_LOG << "Thumbnail already in cache: " << thumbnailName << LOG_ENDL;
     }
     else
     {
	 	// modified by ravikanth 06-08-13
        // if no thumbnail, load original image
        thumbnailName = currentSource().toString();
     }
     return thumbnailName;
 }
// } modified by ravikanth 08-04-13

// { modified by ravikanth 16-04-13
void AppMediaPlayer_EnginePhoto::CancelCopyJukebox()
{
	// modified for ITS 0214738
    AppFileManagerFileOperationParameter param;
    param.isFront = ( m_CurDisplay == PHOTO_DISPLAY_FRONT ); // modified for ISV 93629
    param.mediaType = eStartInvalidSState;
    if ( m_PhotoController.getCurSourceID() == (int)PHOTO_SOURCE_JUKEBOX)//jukebox
    {
        param.mediaType = eStartJukePicture;
    }
    else if ( m_PhotoController.getCurSourceID() == (int)PHOTO_SOURCE_USB_1 ||
             m_PhotoController.getCurSourceID() == (int)PHOTO_SOURCE_USB_2  )
    {
        param.mediaType = eStartUsbPicture;
    }
    Event postEvent( EVT_POST_EVENT,
                     m_EngineMain.GetThisAppId(),
                     APP_FMGR,
                     EVT_FILEOPERATION_STOP, QByteArray((char *)&param, sizeof(param)));
    emit NotifyUISH (postEvent);
    m_EngineMain.cancelAudioFileCopyOperation();
}
// } modified by ravikanth 16-04-13

// { modified by ravikanth 18-04-13
bool AppMediaPlayer_EnginePhoto::loadingOnCopy()
{
    // removed by kihyung 2013.09.09 for ITS 0186027
    /*
    MP_LOG << m_PhotoController.getCurSourceID() << m_trackerUpdateInProgress << LOG_ENDL;
    if(m_PhotoController.getCurSourceID() == PHOTO_SOURCE_JUKEBOX)
    {
        return m_trackerUpdateInProgress; // modified by ravikanth 20-04-13
    }
    */
    return false;
}
// } modified by ravikanth 18-04-13

// { modified by ravikanth 20-04-13
void AppMediaPlayer_EnginePhoto::setRescanProgressLoading()
{
    // removed by kihyung 2013.09.09 for ITS 0186027
    /*
    MP_LOG << LOG_ENDL;
    m_trackerUpdateInProgress = true;
    emit jukeboxCopyInProgress();
    */
}

void AppMediaPlayer_EnginePhoto::ResetProgressLoading()
{
    // removed by kihyung 2013.09.09 for ITS 0186027
    /*
    MP_LOG << LOG_ENDL;
    m_trackerUpdateInProgress = false;
    emit jukeboxCopyInProgress();
    */
}
// } modified by ravikanth 20-04-13

// { modified by ravikanth 15-05-13
int AppMediaPlayer_EnginePhoto::GetAllPlaylistCurrentIndex()
{
    return m_PhotoController.GetCurrentIndex();
}

int AppMediaPlayer_EnginePhoto::GetAllPlaylistTotalCount()
{
    return m_PhotoController.GetCurrentPlayListCount();
}
// } modified by ravikanth 15-05-13

// modified by ravikanth 27-07-13 for ITS 181678
void AppMediaPlayer_EnginePhoto::SetPhotoIndex(PHOTO_SOURCES_LIST source, int index)
{
    m_PhotoController.SetIndexForPhotoSource(source, index);
    SetPhotoLastState(source);
    SetCurModePlaylist(); //added by edo.lee 2013.08.20
}

void AppMediaPlayer_EnginePhoto::SetPhotoLastState(PHOTO_SOURCES_LIST source)
{
    QString fullFileName = m_PhotoController.GetFileFullPathName(source);
    if(fullFileName != "")
    {
        if(fullFileName.startsWith("file://"))
        {
            fullFileName = fullFileName.remove(0,7);
        }

        QString folder = fullFileName.mid(0, fullFileName.lastIndexOf("/"));

        //modified by ravikanth 15-08-13 for ITS 184362
        //QString fileName = fullFileName.mid(fullFileName.lastIndexOf("/") + 1);
        //MP_LOG << folder << " "<< fullFileName << LOG_ENDL;
        m_LastState[m_CurDisplay].curFolder[source] = folder;
        m_LastState[m_CurDisplay].currentFile[source] = fullFileName;
    }
}

void AppMediaPlayer_EnginePhoto::setSlideShowDelayToOptions()
{
    OPTION_MENU_T* model;

    if (PHOTO_DISPLAY_FRONT == m_CurDisplay)
    {
       model = SLIDESHOW_MENU;
    }
    else
    {
       model = SLIDESHOW_MENU_REAR;
    }

    //reset model
    for(int i=1; i<5; i++)
    {
        model[i].bSelected = false;
    }

    switch (m_LastState[m_CurDisplay].slideshowDelay)
    {
    case 5:
       model[1].bSelected = true;
       break;
    case 10:
       model[2].bSelected = true;
       break;
    case 20:
       model[3].bSelected = true;
       break;
    case 30:
       model[4].bSelected = true;
       break;
    default:
       model[1].bSelected = true;
       break;
    }
}

void AppMediaPlayer_EnginePhoto::stopSlideShowForSource(const PHOTO_SOURCES_LIST source)
{
    MP_LOG << LOG_ENDL;
    if(source == m_PhotoController.getCurSourceID())
        setIsSlideShow(false);
}

void AppMediaPlayer_EnginePhoto::setUnsupportedOnImageLoadingError()
{
    MP_LOG << LOG_ENDL;
    m_unsupportedFile = true;
    m_PhotoController.handleUnsupportedFile(m_source);
    emit currentSourceChanged(); // emit source change to notify qml for changing to unsupported image
}

// modified for icon disable on all unsupported files
void AppMediaPlayer_EnginePhoto::onHandleAllUnsupportedFiles(int source)
{
    MP_LOG << LOG_ENDL;
    SetNotifyAllUnsupported(source);
}

// modified for ITS 0215804
QString AppMediaPlayer_EnginePhoto::folderName()
{
    //MP_LOG << (currentSource().toLocalFile()) << LOG_ENDL;
    return getCurrentFolderNameByPath(currentSource().toLocalFile());
}
