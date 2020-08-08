#include "util/DHAVN_AppMediaPlayer_Settings.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"

#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerFS.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerVCD.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_VCD.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h"
#include "engine/DHAVN_AppMediaPlayer_EngineAudio.h"
#include "engine/DHAVN_AppMediaPlayer_EnginePhoto.h"
#include "mediaplayer_def.h"

// { added by kihyung 2013.08.09 for ITS 0183077
#define SETTINGS_FILEPATH_AUDIO         "/app/data/AppMedia/CombinedMediaAudio.conf"
#define SETTINGS_FILEPATH_AUDIO_BACKUP  "/app/data/AppMedia/CombinedMediaAudioBackup.conf" //added by jaehwan.lee 2014.05.23 to fix "data load fail"
#define SETTINGS_FILEPATH_VIDEO         "/app/data/AppMedia/CombinedMediaVideo.conf"
#define SETTINGS_FILEPATH_VIDEO_BACKUP  "/app/data/AppMedia/CombinedMediaVideoBackup.conf" //added by jaehwan.lee 2014.05.23 to fix "data load fail"
#define SETTINGS_FILEPATH_PHOTO         "/app/data/AppMedia/CombinedMediaPhoto.conf"
#define SETTINGS_FILEPATH_PHOTO_BACKUP  "/app/data/AppMedia/CombinedMediaPhotoBackup.conf" //added by jaehwan.lee 2014.05.23 to fix "data load fail"
#define SETTINGS_FILEPATH_COMMON        "/app/data/AppMedia/CombinedMediaCommon.conf"
#define SETTINGS_FILEPATH_COMMON_BACKUP  "/app/data/AppMedia/CombinedMediaCommonBackup.conf" //added by sangmin.seol 2014.09.24 to fix "data load fail"
#define SETTINGS_FILEPATH_PLAYLIST      "/app/data/AppMedia/CombinedMediaPlaylist.conf"
#define SETTINGS_FILEPATH_USBEXT        "/app/data/AppMedia/CombinedMediaUSBExt.conf"
#define SETTINGS_FILEPATH_USB_VIDEO_EXT "/app/data/AppMedia/CombinedMediaUSBVideoExt.conf"
#define SETTINGS_FILEPATH_USBPLAYLIST   "/app/data/AppMedia/CombinedMediaUSBPlaylist.conf"

#define VIDEO_GROUP         "video"
#define PHOTO_GROUP         "photo"
#define AUDIO_GROUP         "audio"
#define LAST_AV_EP_GROUP    "lastAVEP"
#define PLAYLIST_GROUP      "playlist"
#define USBEXT_GROUP        "usbext"
#define USBVIDEOEXT_GROUP   "usbvideoext"
#define USBPLAYLIST_GROUP   "usbplaylist"

#define USBEXT_NAME         "USBExtended"

AppMediaPlayer_SettingsStorage* AppMediaPlayer_SettingsStorage::m_pInstance = 0;

AppMediaPlayer_SettingsStorage::AppMediaPlayer_SettingsStorage( QObject* ):
   audio(SETTINGS_FILEPATH_AUDIO, QSettings::IniFormat),
   audioBackup(SETTINGS_FILEPATH_AUDIO_BACKUP, QSettings::IniFormat), //added by jaehwan.lee 2014.05.23 to fix "data load fail"
   video(SETTINGS_FILEPATH_VIDEO, QSettings::IniFormat),
   videoBackup(SETTINGS_FILEPATH_VIDEO_BACKUP, QSettings::IniFormat), //added by jaehwan.lee 2014.05.23 to fix "data load fail"
   photo(SETTINGS_FILEPATH_PHOTO, QSettings::IniFormat),
   photoBackup(SETTINGS_FILEPATH_PHOTO_BACKUP, QSettings::IniFormat), //added by jaehwan.lee 2014.05.23 to fix "data load fail"
   common(SETTINGS_FILEPATH_COMMON, QSettings::IniFormat),
   commonBackup(SETTINGS_FILEPATH_COMMON_BACKUP, QSettings::IniFormat), //added by jaehwan.lee 2014.05.23 to fix "data load fail"
   playlist(SETTINGS_FILEPATH_PLAYLIST, QSettings::IniFormat),
   usbextended(SETTINGS_FILEPATH_USBEXT, QSettings::IniFormat), 
   usbplaylist(SETTINGS_FILEPATH_USBPLAYLIST, QSettings::IniFormat), 
   usbvideoextended(SETTINGS_FILEPATH_USB_VIDEO_EXT, QSettings::IniFormat), 
   m_bJukeboxIndexing(false),
   m_bJukeboxScanning(false), // added by kihyung 2013.07.25 for ITS 0180896 
   // { added by cychoi 2013-07-15 for Prevent fix
   m_nLastUSB1AVMode(APP_MEDIA_MODE_MAX),
   m_nLastUSB2AVMode(APP_MEDIA_MODE_MAX),
   // } added by cychoi 2013-07-15
   m_nLastAVEntry(0),
   m_nLoadLastAVEntry(0), // added by kihyung 2013.07.16 for STATIC 22926
   m_nLastAudioPos(-1), // added by kihyung 2013.07.30 for ITS 0181638
   m_nLastVideoPos(-1), // added by kihyung 2013.07.30 for ITS 0181638
   m_nBootUSBPort(0), // added by jaehwan.lee 2013.10.25 fix for static test issue
   m_controllerVideo(NULL),
   m_controllerAudio(NULL),
   m_EngineMain(NULL)   //added by sangmin.seol 2014.09.24 to fix "data load fail"
{
  // { added by jaehwan.lee 2014.05.23 to fix "data load fail"
   m_videoBackupTimer.setSingleShot(true);
   m_videoBackupTimer.setInterval(2500);
   connect(&m_videoBackupTimer, SIGNAL(timeout()), this, SLOT(OnVideoBackupTimerExpired()) );

   m_audioBackupTimer.setSingleShot(true);
   m_audioBackupTimer.setInterval(2500);
   connect(&m_audioBackupTimer, SIGNAL(timeout()), this, SLOT(OnAudioBackupTimerExpired()) );

   m_photoBackupTimer.setSingleShot(true);
   m_photoBackupTimer.setInterval(2500);
   connect(&m_photoBackupTimer, SIGNAL(timeout()), this, SLOT(OnPhotoBackupTimerExpired()) );
   //} added by jaehwan.lee 2014.05.23 to fix "data load fail"

   //{ added by sangmin.seol 2014.09.24 to fix "data load fail"
   m_commonBackupTimer.setSingleShot(true);
   m_commonBackupTimer.setInterval(2500);
   connect(&m_commonBackupTimer, SIGNAL(timeout()), this, SLOT(OnCommonBackupTimerExpired()) );
   //} added by sangmin.seol 2014.09.24 to fix "data load fail"

   m_photoDisplay =PHOTO_DISPLAY_NONE;
}

// { added by jaehwan.lee 2014.05.23 to fix "data load fail"
void
AppMediaPlayer_SettingsStorage::OnVideoBackupTimerExpired()
{
    MP_MEDIUM <<"" << LOG_ENDL;
    SaveVideoData(m_controllerVideo, true);
}

void
AppMediaPlayer_SettingsStorage::OnAudioBackupTimerExpired()
{
    MP_MEDIUM <<"" << LOG_ENDL;
    SaveAudioData(m_controllerAudio, true);
}

void
AppMediaPlayer_SettingsStorage::OnPhotoBackupTimerExpired()
{
    MP_MEDIUM <<"" << LOG_ENDL;
    SavePhotoData(m_photoLastState, m_photoDisplay, true);
}
// } added by jaehwan.lee 2014.05.23 to fix "data load fail"

//{ added by sangmin.seol 2014.09.24 to fix "data load fail"
void
AppMediaPlayer_SettingsStorage::OnCommonBackupTimerExpired()
{
    MP_MEDIUM <<"" << LOG_ENDL;
    SaveCommon(m_EngineMain, true);
}
//} added by sangmin.seol 2014.09.24 to fix "data load fail"

void
AppMediaPlayer_SettingsStorage::LoadAudioData(AppMediaPlayer_Controller_Audio *controllerAudio)
{
    MP_MEDIUM << "START" << LOG_ENDL;
    if (controllerAudio)
    {
        QSettings *settings = &audio;

        settings->beginGroup(AUDIO_GROUP);

        int countryVar = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_COUNTRY_VARIANT].keyString, 10000).toInt();

        if (countryVar == 10000 )
        {
            settings->endGroup();
            MP_HIGH << "LOAD FAIL, TRY WITH BACKUP" << LOG_ENDL;

            //retry with backup
            settings = &audioBackup;
            settings->beginGroup(AUDIO_GROUP);

            countryVar = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_COUNTRY_VARIANT].keyString, 10000).toInt();

            if (countryVar == 10000 )
            {
                MP_HIGH << "BACKUP LOAD FAIL" << LOG_ENDL;
            }
        }

        if(countryVar != AppEngineBase::GetCountryVariant()) 
        {
            settings->endGroup();
            
            MP_HIGH << "my version: " << countryVar << ", system version: " << AppEngineBase::GetCountryVariant() << LOG_ENDL; // added by kihyung 2013.07.24 for ITS 0181561
            
            ResetAudioData(controllerAudio);
            SaveAudioData(controllerAudio);
            return;
        }
        
        for (int device = MP::JUKEBOX; device < MP::ALL_DEVICES; device++)
        {
            controllerAudio->ResetDeviceInfo(device);
            
            DeviceHistory *history = controllerAudio->GetDeviceHistory(device);

            if (history)
            {
                settings->beginGroup(controllerAudio->DeviceName(device));
                history->DeviceID               = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_MEDIA_DEVICE_ID].keyString, "").toString();
                history->MountPath              = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_MEDIA_MOUNT_PATH].keyString, "").toString();
                history->Index                  = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_INDEX].keyString, 0).toInt();
                history->Position               = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_POSITION].keyString, 0).toInt();
                history->ContentPlayed          = (MP::PLAYBACK_CONTENT_T)settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_PLAYED_CONTENTS].keyString, 0).toInt();
                history->repeatPlayMode         = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_REPEAT_PLAYMODE].keyString, REPEATALL).toInt();
                history->randomPlayMode         = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_RANDOM_PLAYMODE].keyString, RANDOMOFF).toInt();
                history->mediaType              = (MEDIA_TYPE_T)settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_VIDEO_ACTIVE_MODE].keyString, 0).toInt();
                history->Title                  = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_TITLE].keyString, "").toString();
                history->Artist                 = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_ARTIST].keyString, "").toString();
                history->Album                  = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_ALBUM].keyString, "").toString();
                history->listReqData.tabId      = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_TABID].keyString, "").toString();
                history->listReqData.queryStr   = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_QUERY].keyString, "").toString();
                history->listReqData.itemIndex  = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_ITEM_INDEX].keyString, 0).toInt();
                history->listReqData.depth      = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_DEPTH].keyString, 0).toInt();
                settings->endGroup();

                history->playList.index  = 0;
                history->Quantity        = 0;

                //{added by junam 2013.12.4 for ISV_95045
                if ( device == MP::JUKEBOX || device == MP::USB1 || device == MP::USB2 )
                    history->Cover = CTrackerAbstractor::GetMediaArt( history->Artist, history->Album);
                //}added by junam

                MP_MEDIUM << "device = " << device << " history->Index = " << history->Index << "," << history->Position << LOG_ENDL;
            }
        }

        bool bTip = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_DISPLAY_TIP].keyString, false).toBool();
        controllerAudio->GetListViewModel()->setDisplayTip(bTip);

        //{added by junam 2013.07.04 for ITS172937
        QVariantList categoryDefault, categoryValue;
        for(int idx = 0; idx< 9; idx++)
        {
            categoryDefault.append(QVariant::fromValue(idx));
        }
        categoryValue = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_CATEGORY].keyString,
                                    QVariant::fromValue(categoryDefault)).toList();
        foreach(QVariant var, categoryDefault)
        {
            int iFirst = categoryValue.indexOf(var);
            int iLast = categoryValue.lastIndexOf(var);
            if(iFirst == -1 || iFirst != iLast)
            {
                categoryValue = categoryDefault;
                MP_HIGH << "Setting is broken : use default value instead" << LOG_ENDL;
                break;
            }
        }
        controllerAudio->categoryListSetter(categoryValue);
        //}added by junam

        settings->endGroup();
    }
    MP_MEDIUM << "END" << LOG_ENDL;

    LoadPlaylist(controllerAudio);
}

// modified by kihyung 2013.07.26 for ITS 0181599 
void
AppMediaPlayer_SettingsStorage::LoadPhotoData(PHOTO_LAST_STATE_T* lastState, PHOTO_DISPLAY display)
{
    MP_MEDIUM << "START" << LOG_ENDL;

    if (lastState)
    {
        QSettings *settings = &photo;

        settings->beginGroup(PHOTO_GROUP);

        int countryVar = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_COUNTRY_VARIANT].keyString, 10000).toInt();

        if (countryVar == 10000 )
        {
            settings->endGroup();
            MP_HIGH << "LOAD FAIL, TRY WITH BACKUP" << LOG_ENDL;

            //retry with backup
            settings = &photoBackup;
            settings->beginGroup(PHOTO_GROUP);

            countryVar = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_COUNTRY_VARIANT].keyString, 10000).toInt();

            if (countryVar == 10000 )
            {
                MP_HIGH << "BACKUP LOAD FAIL" << LOG_ENDL;
            }
        }

        if(countryVar != AppEngineBase::GetCountryVariant()) 
        {
            settings->endGroup();

            MP_HIGH << "my version: " << countryVar << ", system version: " << AppEngineBase::GetCountryVariant() << LOG_ENDL; // added by kihyung 2013.07.24 for ITS 0181561
            
            ResetPhotoData(lastState, display);
            SavePhotoData(lastState, display); // added by kihyung 2013.07.24 for ITS 0181561
            return;
        }
		
		// modified by ravikanth 27-07-13 for ITS 181678
        if(display == PHOTO_DISPLAY_FRONT || display == PHOTO_DISPLAY_REAR) {
            for(int i=0;i<PHOTO_SOURCE_MAX;i++)
            {
                lastState[PHOTO_DISPLAY_FRONT].currentFile[i].clear();
                lastState[PHOTO_DISPLAY_REAR].currentFile[i].clear();
            }

            lastState[PHOTO_DISPLAY_FRONT].currentFile[PHOTO_SOURCE_JUKEBOX].append(settings->value(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_JUKEBOX_FILE_FRONT].keyString, "").toString());
            lastState[PHOTO_DISPLAY_FRONT].currentFile[PHOTO_SOURCE_USB_1].append(settings->value(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_USB1_FILE_FRONT].keyString, "").toString());
            lastState[PHOTO_DISPLAY_FRONT].currentFile[PHOTO_SOURCE_USB_2].append(settings->value(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_USB2_FILE_FRONT].keyString, "").toString());
            lastState[PHOTO_DISPLAY_FRONT].slideshowDelay = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_SLIDESHOW_DURATION_FRONT].keyString, 5).toInt();

            lastState[PHOTO_DISPLAY_REAR].currentFile[PHOTO_SOURCE_JUKEBOX].append(settings->value(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_JUKEBOX_FILE_FRONT].keyString, "").toString());
            lastState[PHOTO_DISPLAY_REAR].currentFile[PHOTO_SOURCE_USB_1].append(settings->value(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_USB1_FILE_FRONT].keyString, "").toString());
            lastState[PHOTO_DISPLAY_REAR].currentFile[PHOTO_SOURCE_USB_2].append(settings->value(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_USB2_FILE_FRONT].keyString, "").toString());
            lastState[PHOTO_DISPLAY_REAR].slideshowDelay = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_SLIDESHOW_DURATION_FRONT].keyString, 5).toInt();

        }

        settings->endGroup();
    }

    MP_MEDIUM << "END" << LOG_ENDL;
}

void
AppMediaPlayer_SettingsStorage::LoadVideoData(AppMediaPlayer_Video_Controller *controllerVideo)
{
    MP_MEDIUM << "START" << LOG_ENDL;
 
    QSettings *settings;

    settings = &video;

    settings->beginGroup(VIDEO_GROUP);

    int countryVar = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_COUNTRY_VARIANT].keyString, 10000).toInt();

    if (countryVar == 10000 )
    {
        settings->endGroup();
        MP_HIGH << "LOAD FAIL, TRY WITH BACKUP" << LOG_ENDL;
        //retry with backup

        settings = &videoBackup;
        settings->beginGroup(VIDEO_GROUP);

        countryVar = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_COUNTRY_VARIANT].keyString, 10000).toInt();

        if (countryVar == 10000 )
        {
            MP_HIGH << "BACKUP LOAD FAIL" << LOG_ENDL;
        }
    }

    if(countryVar != AppEngineBase::GetCountryVariant()) 
    {
        MP_HIGH << "ALL LOAD FAIL, Reset Video Data" << LOG_ENDL;
        settings->endGroup();

        MP_HIGH << "my version: " << countryVar << ", system version: " << AppEngineBase::GetCountryVariant() << LOG_ENDL; // added by kihyung 2013.07.24 for ITS 0181561
        
        ResetVideoData(controllerVideo);
        SaveVideoData(controllerVideo); // added by kihyung 2013.07.24 for ITS 0181561
        return;
    }
  
    AppMediaPlayer_Video_CModel_FS  *jukeboxData;
    AppMediaPlayer_Video_CModel_FS  *usbData;
    AppMediaPlayer_Video_CModel_FS  *usb2Data;
 
    jukeboxData = static_cast<AppMediaPlayer_Video_CModel_FS*>(controllerVideo->getController(MEDIA_SOURCE_JUKEBOX)->getModel());
    usbData     = static_cast<AppMediaPlayer_Video_CModel_FS*>(controllerVideo->getController(MEDIA_SOURCE_USB1)->getModel());
    usb2Data    = static_cast<AppMediaPlayer_Video_CModel_FS*>(controllerVideo->getController(MEDIA_SOURCE_USB2)->getModel());
 
    // usb..
    if(settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_FILE].keyString, "").toString() !="")
    {
        usbData->setDeviceID( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_DEVICE_ID].keyString, "").toString());
        usbData->setMountPath( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_MOUNT_PATH].keyString, "").toString());
        usbData->setFolderName( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_FOLDER].keyString, "").toString());
        usbData->setFullFolderName( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_FULLFOLDER].keyString, "").toString());
        usbData->setFilename( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_FILE].keyString, "").toString() );
        usbData->setPosition( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_POSITION].keyString, 0).toInt() );
        usbData->setPlaylistPosition( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_INDEX].keyString, 0).toInt() );
        usbData->setCaptionEnable(settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_SUBTITLE].keyString, false).toBool() );
        usbData->setCaptionSize( (AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_T)settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_SUBTITLE_SIZE].keyString, 1).toInt() );
        usbData->setCaptionCurId( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_SUBTITLE_CURRENT].keyString, 0).toInt() ); // added by Sergey 08.01.2014 for ITS#211412
        usbData->setRandomMode( (AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T)settings->value( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_RANDOM].keyString, (int)AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_OFF).toInt() );
        usbData->setRepeatMode( (AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T)settings->value( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_REPEAT].keyString, (int)AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_OFF).toInt() );
        usbData->setCurrentStreamId( settings->value( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_AUDIO_LANG].keyString, 0).toInt() ); // added by Sergey 07.09.2013 for ITS#185543
        usbData->setDirty(true);
    }
 
    // usb2..
    if(settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_FILE].keyString, "").toString() !="")
    {
        usb2Data->setDeviceID( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_DEVICE_ID].keyString, "").toString());
        usb2Data->setMountPath( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_MOUNT_PATH].keyString, "").toString());
        usb2Data->setFolderName( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_FOLDER].keyString, "").toString());
        usb2Data->setFullFolderName( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_FULLFOLDER].keyString, "").toString());
        usb2Data->setFilename( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_FILE].keyString, "").toString() );
        usb2Data->setPosition( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_POSITION].keyString, 0).toInt() );
        usb2Data->setPlaylistPosition( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_INDEX].keyString, 0).toInt() );
        usb2Data->setCaptionEnable( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_SUBTITLE].keyString, false).toBool() ); // modified by lssanh 2013.02.20 subtitle
        usb2Data->setCaptionSize( (AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_T)settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_SUBTITLE_SIZE].keyString, 1).toInt());
        usb2Data->setCaptionCurId( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_SUBTITLE_CURRENT].keyString, 0).toInt() ); // added by Sergey 08.01.2014 for ITS#211412
        usb2Data->setRandomMode( (AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T)settings->value( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_RANDOM].keyString, (int)AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_OFF).toInt() );
        usb2Data->setRepeatMode( (AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T)settings->value( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_REPEAT].keyString, (int)AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_OFF).toInt() );
        usb2Data->setCurrentStreamId( settings->value( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_AUDIO_LANG].keyString, 0).toInt() ); // added by Sergey 07.09.2013 for ITS#185543
        usb2Data->setDirty(true);
    }
    
    // jukebox...
    if(settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_FILE].keyString, "").toString() !="")
    {
        jukeboxData->setFolderName( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_FOLDER].keyString, "").toString() );
        jukeboxData->setFullFolderName( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_FULLFOLDER].keyString, "").toString() );
        jukeboxData->setFilename( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_FILE].keyString, "").toString() );
        jukeboxData->setPosition( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_POSITION].keyString, 0).toInt() );
        jukeboxData->setPlaylistPosition( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_INDEX].keyString, 0).toInt() );
        jukeboxData->setCaptionEnable( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_SUBTITLE].keyString, false).toBool() );
        jukeboxData->setCaptionSize( (AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_T)settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_SUBTITLE_SIZE].keyString, 1).toInt() );
        jukeboxData->setCaptionCurId( settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_SUBTITLE_CURRENT].keyString, 0).toInt() ); // added by Sergey 08.01.2014 for ITS#211412
        jukeboxData->setRandomMode( (AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T)settings->value( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_RANDOM].keyString, (int)AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_OFF).toInt() );
        jukeboxData->setRepeatMode( (AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T)settings->value( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_REPEAT].keyString, (int)AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_OFF).toInt() );
        jukeboxData->setCurrentStreamId( settings->value( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_AUDIO_LANG].keyString, 0).toInt() ); // added by Sergey 07.09.2013 for ITS#185543
        controllerVideo->loadDivxScreenMode( (DIVX_STATE)settings->value( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_DIVX_SCREEN_MODE].keyString, 0).toInt() ); // added by Sergey 25.10.2013 for new DivX
        jukeboxData->setDirty(true);
 
    }

    MP_MEDIUM << "USB1" << usbData->deviceID() << LOG_ENDL;
    MP_MEDIUM << "USB2" << usb2Data->deviceID() << LOG_ENDL;
    
    settings->endGroup();

    MP_MEDIUM << "END" << LOG_ENDL;
}

void 
AppMediaPlayer_SettingsStorage::LoadCommon()
{
    //{ modified by sangmin.seol 2014.09.24 to fix "data load fail"
    QSettings *settings;

    settings = &common;

    settings->beginGroup(LAST_AV_EP_GROUP);

    int countryVar = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_COUNTRY_VARIANT].keyString, 10000).toInt();

    if (countryVar == 10000 )
    {
        settings->endGroup();
        MP_HIGH << "LOAD FAIL, TRY WITH BACKUP" << LOG_ENDL;
        //retry with backup

        settings = &commonBackup;
        settings->beginGroup(LAST_AV_EP_GROUP);

        countryVar = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_COUNTRY_VARIANT].keyString, 10000).toInt();

        if (countryVar == 10000 )
        {
            MP_HIGH << "BACKUP LOAD FAIL" << LOG_ENDL;
        }
    }

    if(countryVar != AppEngineBase::GetCountryVariant())
    {
        settings->endGroup();
        MP_HIGH << "my version: " << countryVar << ", system version: " << AppEngineBase::GetCountryVariant() << LOG_ENDL; // added by kihyung 2013.07.24 for ITS 0181561

        ResetCommon(m_EngineMain);
        SaveCommon(m_EngineMain); // added by kihyung 2013.07.24 for ITS 0181561
        return;
    }

    m_nLastAVEntry     = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_VIDEO_LAST_ENTRY].keyString, 0).toInt();
    m_bJukeboxIndexing = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_JUKEBOX_INDEXING_END].keyString, false).toBool();
    m_bJukeboxScanning = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_JUKEBOX_SCANNING_END].keyString, false).toBool(); // added by kihyung 2013.07.25 for ITS 0180896
    m_strLastUSB1ID    = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_USB1_LAST_UUID].keyString, "").toString();
    m_nLastUSB1AVMode  = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_USB1_LAST_AV_MODE].keyString, APP_MEDIA_MODE_MAX).toInt();
    m_strLastUSB2ID    = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_USB2_LAST_UUID].keyString, "").toString();
    m_nLastUSB2AVMode  = settings->value(QSETTINGS_KEYS[SettingsDef::KEY_USB2_LAST_AV_MODE].keyString, APP_MEDIA_MODE_MAX).toInt();
    settings->endGroup();
	//} modified by sangmin.seol 2014.09.24 to fix "data load fail"

    MP_MEDIUM << "m_nLastAVEntry = " << m_nLastAVEntry << " m_bJukeboxIndexing = " << m_bJukeboxIndexing << " m_bJukeboxScanning = " << m_bJukeboxScanning << LOG_ENDL;
}

void 
AppMediaPlayer_SettingsStorage::LoadUSBAudioExtended(AppMediaPlayer_Controller_Audio *controllerAudio)
{
    MP_MEDIUM << "START" << LOG_ENDL;
    if (controllerAudio)
    {
        usbextended.beginGroup(USBEXT_GROUP);
        
        controllerAudio->ResetExtendDeviceInfo();
        
        QList<DeviceHistory*> history = controllerAudio->GetExtendDeviceHistory();

        for (int i = 0; i < history.count(); i++)
        {
            if (history[i])
            {
                usbextended.beginGroup(QString(USBEXT_NAME) + i);
                history[i]->DeviceID                = usbextended.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_MEDIA_DEVICE_ID].keyString, "").toString();
                history[i]->MountPath               = usbextended.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_MEDIA_MOUNT_PATH].keyString, "").toString();
                history[i]->Index                   = usbextended.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_INDEX].keyString, 0).toInt();
                history[i]->Position                = usbextended.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_POSITION].keyString, 0).toInt();
                history[i]->ContentPlayed           = (MP::PLAYBACK_CONTENT_T)usbextended.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_PLAYED_CONTENTS].keyString, 0).toInt();
                history[i]->repeatPlayMode          = usbextended.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_REPEAT_PLAYMODE].keyString, REPEATALL).toInt();
                history[i]->randomPlayMode          = usbextended.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_RANDOM_PLAYMODE].keyString, RANDOMOFF).toInt();
                history[i]->mediaType               = (MEDIA_TYPE_T)usbextended.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_VIDEO_ACTIVE_MODE].keyString, 0).toInt();
                history[i]->Title                   = usbextended.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_TITLE].keyString, "").toString();
                history[i]->Artist                  = usbextended.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_ARTIST].keyString, "").toString();
                history[i]->Album                   = usbextended.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_ALBUM].keyString, "").toString();
                history[i]->listReqData.tabId       = usbextended.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_TABID].keyString, "").toString();
                history[i]->listReqData.queryStr    = usbextended.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_QUERY].keyString, "").toString();
                history[i]->listReqData.itemIndex   = usbextended.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_ITEM_INDEX].keyString, 0).toInt();
                history[i]->listReqData.depth       = usbextended.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_DEPTH].keyString, 0).toInt();                
                usbextended.endGroup();
                
                history[i]->playList.index  = 0;
                history[i]->Quantity        = 0;
                
                MP_MEDIUM << i << history[i]->DeviceID << LOG_ENDL;
            }
        }
        usbextended.endGroup();
    }
    MP_MEDIUM << "END" << LOG_ENDL;

    LoadUSBPlaylist(controllerAudio);
}

void 
AppMediaPlayer_SettingsStorage::LoadUSBVideoExtended(AppMediaPlayer_Video_Controller *controllerVideo)
{
    MP_MEDIUM << "START" << LOG_ENDL;
    if (controllerVideo)
    {
        usbvideoextended.beginGroup(USBVIDEOEXT_GROUP);
        
        controllerVideo->ResetExtendDeviceInfo();
        
        QList<AppMediaPlayer_Video_CModel_FS*> history = controllerVideo->GetExtendDeviceHistory();

        for (int i = 0; i < history.count(); i++)
        {
            if (history[i])
            {
                usbvideoextended.beginGroup(QString(USBEXT_NAME) + i);
                if(usbvideoextended.value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_FILE].keyString, "").toString() !="")
                {
                    history[i]->setDeviceID( usbvideoextended.value( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_DEVICE_ID].keyString, "").toString());
                    history[i]->setMountPath( usbvideoextended.value( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_MOUNT_PATH].keyString, "").toString());
                    history[i]->setFolderName( usbvideoextended.value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_FOLDER].keyString, "").toString());
                    history[i]->setFullFolderName( usbvideoextended.value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_FULLFOLDER].keyString, "").toString());
                    history[i]->setFilename( usbvideoextended.value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_FILE].keyString, "").toString() );
                    history[i]->setPosition( usbvideoextended.value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_POSITION].keyString, 0).toInt() );
                    history[i]->setPlaylistPosition( usbvideoextended.value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_INDEX].keyString, 0).toInt() );
                    history[i]->setCaptionEnable(usbvideoextended.value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_SUBTITLE].keyString, false).toBool() );
                    history[i]->setCaptionSize( (AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_T)usbvideoextended.value(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_SUBTITLE_SIZE].keyString, 1).toInt() );
                    history[i]->setRandomMode( (AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T)usbvideoextended.value( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_RANDOM].keyString, (int)AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_OFF).toInt() );
                    history[i]->setRepeatMode( (AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T)usbvideoextended.value( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_REPEAT].keyString, (int)AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_OFF).toInt() );
                    history[i]->setDirty(true);
                }
                usbvideoextended.endGroup();

                MP_MEDIUM << i << " " << history[i]->deviceID() << LOG_ENDL;
            }
        }
        usbvideoextended.endGroup();
    }
    MP_MEDIUM << "END" << LOG_ENDL;
}
    
void
AppMediaPlayer_SettingsStorage::SaveAudioData(AppMediaPlayer_Controller_Audio* controllerAudio, bool bBackup)
{
    if (controllerAudio)
    {
        MP_MEDIUM << "START bBackup = " << bBackup << LOG_ENDL;

        QSettings *settings;
        if (bBackup)
            settings = &audioBackup;
        else
            settings = &audio;


        settings->beginGroup(AUDIO_GROUP);

        for (int device = MP::JUKEBOX; device < MP::ALL_DEVICES; device++)
        {
            DeviceHistory *history = controllerAudio->GetDeviceHistory(device);

            if (history)
            {
                settings->beginGroup(controllerAudio->DeviceName(device));
                settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_MEDIA_DEVICE_ID].keyString, history->DeviceID);
                settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_MEDIA_MOUNT_PATH].keyString, history->MountPath);
                settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_INDEX].keyString, history->Index);
                settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_POSITION].keyString, history->Position);
                settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_PLAYED_CONTENTS].keyString, (int)history->ContentPlayed);
                settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_REPEAT_PLAYMODE].keyString, history->repeatPlayMode);
                settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_RANDOM_PLAYMODE].keyString, history->randomPlayMode);
                settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_VIDEO_ACTIVE_MODE].keyString, (int)history->mediaType);
                settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_TITLE].keyString, history->Title);
                settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_ARTIST].keyString, history->Artist);
                settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_ALBUM].keyString, history->Album);
                settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_TABID].keyString, history->listReqData.tabId);
                settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_QUERY].keyString, history->listReqData.queryStr);
                settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_ITEM_INDEX].keyString, history->listReqData.itemIndex);
                settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_DEPTH].keyString, history->listReqData.depth);
                settings->endGroup();

                MP_MEDIUM << "device=" << device << "history->Index=" << history->Index << "," << history->Position << LOG_ENDL;
            }
        }
        
        settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_DISPLAY_TIP].keyString, controllerAudio->GetListViewModel()->isDisplayTip());
        //{added by junam 2013.07.04 for ITS172937
        settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_CATEGORY].keyString, QVariant::fromValue( controllerAudio->categoryList()));
        //}added by junam
        settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_COUNTRY_VARIANT].keyString, (int)AppEngineBase::GetCountryVariant());
        settings->endGroup();

        MP_MEDIUM << "END" << LOG_ENDL;
    }
    
    // if(bSync) audio.sync(); // removed by kihyung 2013.11.05 for ITS 0206507 
}

void
AppMediaPlayer_SettingsStorage::SavePlaylist(AppMediaPlayer_Controller_Audio* controllerAudio, bool bSync)
{
    if (controllerAudio)
    {
        MP_MEDIUM << "START" << bSync << LOG_ENDL;
        
        playlist.beginGroup(PLAYLIST_GROUP);

        for (int device = MP::JUKEBOX; device < MP::ALL_DEVICES; device++)
        {
            DeviceHistory *history = controllerAudio->GetDeviceHistory(device);

            //{removed by kihyung 2013.07.18 for ITS 0180268
            //{added by junam 2013.06.11 for ITS173073
            //if(history->ContentPlayed == MP::MORELIKETHIS)
            //    continue;
            //}added by junam
            //}removed by kihyung 2013.07.18 for ITS 0180268

            QList<QVariant> list;
            foreach (QString filePath, *(history->playList.list))
            {
                list.append(filePath);
            }

            playlist.beginGroup(controllerAudio->DeviceName(device));
            playlist.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_CURRENT_PLAYLIST].keyString, list);                    
            playlist.endGroup();
        }
        
        playlist.endGroup();

        MP_MEDIUM << "END" << LOG_ENDL;

        // if(bSync) playlist.sync(); // removed by kihyung 2013.11.05 for ITS 0206507 
    }
}

// { added by jaehwan.lee 2014.05.23 to fix "data load fail"
void
AppMediaPlayer_SettingsStorage::SavePhotoData(PHOTO_LAST_STATE_T* lastState, PHOTO_DISPLAY display, bool bBackup)
{
    if (lastState)
    {
        MP_MEDIUM << "START bBackup = " << bBackup << LOG_ENDL;

        QSettings *settings;
        if (bBackup)
            settings = &photoBackup;
        else
            settings = &photo;

        settings->beginGroup(PHOTO_GROUP);
  
        // MP_LOG << " ======save data for Front display====== " << LOG_ENDL;
        // MP_LOG<<"[JukeboxFilePath]"<< lastState[PHOTO_DISPLAY_FRONT].currentFile[PHOTO_SOURCE_JUKEBOX] << LOG_ENDL;
        // MP_LOG<<"[USB1FilePath]"<< lastState[PHOTO_DISPLAY_FRONT].currentFile[PHOTO_SOURCE_USB_1] << LOG_ENDL;
        // MP_LOG<<"[USB2FilePath]"<< lastState[PHOTO_DISPLAY_FRONT].currentFile[PHOTO_SOURCE_USB_2] << LOG_ENDL;
        // MP_LOG<<"[SlideshowDelay]" << lastState[PHOTO_DISPLAY_FRONT].slideshowDelay << LOG_ENDL;
  
        if(display == PHOTO_DISPLAY_FRONT) {
            settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_JUKEBOX_FILE_FRONT].keyString, lastState[PHOTO_DISPLAY_FRONT].currentFile[PHOTO_SOURCE_JUKEBOX]);
            settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_USB1_FILE_FRONT].keyString, lastState[PHOTO_DISPLAY_FRONT].currentFile[PHOTO_SOURCE_USB_1]);
            settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_USB2_FILE_FRONT].keyString, lastState[PHOTO_DISPLAY_FRONT].currentFile[PHOTO_SOURCE_USB_2]);
            settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_SLIDESHOW_DURATION_FRONT].keyString, lastState[PHOTO_DISPLAY_FRONT].slideshowDelay);
        }
  
        // MP_LOG << " ======save data for Rear display====== " << LOG_ENDL;
        // MP_LOG<<"[JukeboxFilePath]"<< lastState[PHOTO_DISPLAY_REAR].currentFile[PHOTO_SOURCE_JUKEBOX] << LOG_ENDL;
        // MP_LOG<<"[USB1FilePath]"<< lastState[PHOTO_DISPLAY_REAR].currentFile[PHOTO_SOURCE_USB_1] << LOG_ENDL;
        // MP_LOG<<"[USB2FilePath]"<< lastState[PHOTO_DISPLAY_REAR].currentFile[PHOTO_SOURCE_USB_2] << LOG_ENDL;
        // MP_LOG<<"[SlideshowDelay]" << lastState[PHOTO_DISPLAY_REAR].slideshowDelay << LOG_ENDL;
  
        if(display == PHOTO_DISPLAY_REAR) {
      	  // modified by ravikanth 27-07-13 for ITS 181678
            settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_JUKEBOX_FILE_FRONT].keyString, lastState[PHOTO_DISPLAY_REAR].currentFile[PHOTO_SOURCE_JUKEBOX]);
            settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_USB1_FILE_FRONT].keyString, lastState[PHOTO_DISPLAY_REAR].currentFile[PHOTO_SOURCE_USB_1]);
            settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_USB2_FILE_FRONT].keyString, lastState[PHOTO_DISPLAY_REAR].currentFile[PHOTO_SOURCE_USB_2]);
            settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_SLIDESHOW_DURATION_FRONT].keyString, lastState[PHOTO_DISPLAY_REAR].slideshowDelay);
        }
        
        settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_PHOTO_COUNTRY_VARIANT].keyString, (int)AppEngineBase::GetCountryVariant());
        settings->endGroup();
    }
}


void
AppMediaPlayer_SettingsStorage::SaveVideoSettings(AppMediaPlayer_Video_Controller *controllerVideo)
{
    m_controllerVideo = controllerVideo;
    m_videoBackupTimer.start();

    SaveVideoData(controllerVideo, false);
}

void
AppMediaPlayer_SettingsStorage::SaveAudioSettings(AppMediaPlayer_Controller_Audio *controllerAudio)
{
    m_controllerAudio = controllerAudio;
    m_audioBackupTimer.start();

    SaveAudioData(controllerAudio, false);
}

void
AppMediaPlayer_SettingsStorage::SavePhotoSettings(PHOTO_LAST_STATE_T* lastState, PHOTO_DISPLAY display)
{
    //copy last state
    if (lastState)
    {
        for(int i=0; i < PHOTO_SOURCE_MAX; i++)
        {
            m_photoLastState[display].currentFile[i] = lastState[display].currentFile[i];
            m_photoLastState[display].curFolder[i] = lastState[display].curFolder[i];
            m_photoLastState[display].filesInCurFolder[i] = lastState[display].filesInCurFolder[i];
        }
        m_photoLastState[display].slideshowDelay = lastState[display].slideshowDelay;
    }

    //save display
    m_photoDisplay = display;

    m_photoBackupTimer.start();

    SavePhotoData(lastState, display, false);
}
// } added by jaehwan.lee 2014.05.23 to fix "data load fail"

//{ added by sangmin.seol 2014.09.24 to fix "data load fail"
void
AppMediaPlayer_SettingsStorage::SaveCommonSettings(AppMediaPlayer_EngineMain *pEngineMain)
{
    m_EngineMain = pEngineMain;

    if(m_commonBackupTimer.isActive())
        m_commonBackupTimer.stop();

    m_commonBackupTimer.start();

    SaveCommon(pEngineMain, false);
}
//} added by sangmin.seol 2014.09.24 to fix "data load fail"

// modified by kihyung 2013.07.20 for 0181075 
void
AppMediaPlayer_SettingsStorage::SaveVideoData(AppMediaPlayer_Video_Controller *controllerVideo, bool bBackup)
{
    if (controllerVideo == NULL)
    {
        MP_HIGH << "Invalid arg." << LOG_ENDL;
        return;
    }

    MP_MEDIUM << "START BACKUP=" << bBackup << LOG_ENDL;
    QSettings *settings;

    if (bBackup)
        settings = &videoBackup;
    else
        settings = &video;

 
    settings->beginGroup(VIDEO_GROUP);
 
    AppMediaPlayer_Video_CModel_FS  *jukeboxData;
    AppMediaPlayer_Video_CModel_FS  *usbData;
    AppMediaPlayer_Video_CModel_FS  *usb2Data;
 
    jukeboxData = static_cast<AppMediaPlayer_Video_CModel_FS*>(controllerVideo->getController(MEDIA_SOURCE_JUKEBOX)->getModel());
    usbData     = static_cast<AppMediaPlayer_Video_CModel_FS*>(controllerVideo->getController(MEDIA_SOURCE_USB1)->getModel());
    usb2Data    = static_cast<AppMediaPlayer_Video_CModel_FS*>(controllerVideo->getController(MEDIA_SOURCE_USB2)->getModel());    

    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_COUNTRY_VARIANT].keyString, (int)AppEngineBase::GetCountryVariant());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_ACTIVE_SOURCE].keyString, (int)controllerVideo->getSource());
    // usb...
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_DEVICE_ID].keyString, usbData->deviceID());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_MOUNT_PATH].keyString, usbData->mountPath());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_FOLDER].keyString, usbData->foldername());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_FULLFOLDER].keyString, usbData->fullfoldername());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_FILE].keyString, usbData->filename());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_POSITION].keyString, usbData->position());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_INDEX].keyString, usbData->playlistPosition());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_SUBTITLE].keyString, usbData->captionEnable());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_SUBTITLE_SIZE].keyString, usbData->captionSize());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_SUBTITLE_CURRENT].keyString, usbData->captionCurId()); // added by Sergey 08.01.2014 for ITS#211412
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_RANDOM].keyString, usbData->randomMode());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_REPEAT].keyString, usbData->repeatMode());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_AUDIO_LANG].keyString, usbData->getCurrentStreamId()); // added by Sergey 07.09.2013 for ITS#185543
 
    // usb2...
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_DEVICE_ID].keyString, usb2Data->deviceID());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_MOUNT_PATH].keyString, usb2Data->mountPath());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_FOLDER].keyString, usb2Data->foldername());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_FULLFOLDER].keyString, usb2Data->fullfoldername());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_FILE].keyString, usb2Data->filename());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_POSITION].keyString, usb2Data->position());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_INDEX].keyString, usb2Data->playlistPosition());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_SUBTITLE].keyString, usb2Data->captionEnable());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_SUBTITLE_SIZE].keyString, usb2Data->captionSize()); // modified by Sergey for 70345
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_SUBTITLE_CURRENT].keyString, usb2Data->captionCurId()); // added by Sergey 08.01.2014 for ITS#211412
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_RANDOM].keyString, usb2Data->randomMode());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_REPEAT].keyString, usb2Data->repeatMode());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_AUDIO_LANG].keyString, usb2Data->getCurrentStreamId()); // added by Sergey 07.09.2013 for ITS#185543
 
    // jukebox..
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_FOLDER].keyString, jukeboxData->foldername());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_FULLFOLDER].keyString, jukeboxData->fullfoldername());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_FILE].keyString, jukeboxData->filename());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_POSITION].keyString, jukeboxData->position());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_INDEX].keyString, jukeboxData->playlistPosition());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_SUBTITLE].keyString, jukeboxData->captionEnable());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_SUBTITLE_SIZE].keyString, jukeboxData->captionSize());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_SUBTITLE_CURRENT].keyString, jukeboxData->captionCurId()); // added by Sergey 08.01.2014 for ITS#211412
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_RANDOM].keyString, jukeboxData->randomMode());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_REPEAT].keyString, jukeboxData->repeatMode());
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_AUDIO_LANG].keyString, jukeboxData->getCurrentStreamId()); // added by Sergey 07.09.2013 for ITS#185543
    settings->setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_DIVX_SCREEN_MODE].keyString, controllerVideo->divxScreenMode()); // added by Sergey 25.10.2013 for new DivX
 
    settings->endGroup();

    MP_MEDIUM << "JUKEBOX" << jukeboxData->deviceID() << jukeboxData->filename() << LOG_ENDL;
    MP_MEDIUM << "USB1" << usbData->deviceID() << usbData->filename() << LOG_ENDL;
    MP_MEDIUM << "USB2" << usb2Data->deviceID() << usb2Data->filename() << LOG_ENDL;

    MP_MEDIUM << "END" << LOG_ENDL;
}

// { modified by sangmin.seol 2014.09.24 to fix "data load fail"
void AppMediaPlayer_SettingsStorage::SaveCommon(AppMediaPlayer_EngineMain *pEngineMain, bool bBackup)
{
    if (pEngineMain == NULL)
    {
        MP_HIGH << "Invalid arg." << LOG_ENDL;
        return;
    }

    QSettings *settings;

    if (bBackup)
        settings = &commonBackup;
    else
        settings = &common;
// } modified by sangmin.seol 2014.09.24 to fix "data load fail"

    AppMediaPlayer_EngineAudio      *pEngineAudio = (AppMediaPlayer_EngineAudio*)pEngineMain->GetEngine(APP_MEDIA_MODE_AUDIO);
    AppMediaPlayer_Controller_Audio *pController  = (AppMediaPlayer_Controller_Audio*)pEngineAudio->GetController();
    
    int  nLastEntry       = pEngineMain->GetLastAVEntryPoint(false);
    bool bJukeboxIndexing = pController->isMinerFinished();
    bool bJukeboxScanning = (pEngineMain->isCopyInProgress() == true || 
                             pEngineMain->isTAScanComplete(eJukeBox) == false || 
                             pEngineMain->isRequestedJukeboxRescan() == true) ? false : true; // modified by kihyung 2013.11.16 to remove sync error after copy/delete.

    // { modified by sangmin.seol 2014.09.24 to fix "data load fail"
    settings->beginGroup(LAST_AV_EP_GROUP);
    settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_COUNTRY_VARIANT].keyString, (int)AppEngineBase::GetCountryVariant());
    settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_VIDEO_LAST_ENTRY].keyString, nLastEntry);
    settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_JUKEBOX_INDEXING_END].keyString, bJukeboxIndexing);
    settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_JUKEBOX_SCANNING_END].keyString, bJukeboxScanning); // added by kihyung 2013.07.25 for ITS 0180896
    settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_USB1_LAST_UUID].keyString, m_strLastUSB1ID);
    settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_USB1_LAST_AV_MODE].keyString, m_nLastUSB1AVMode);
    settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_USB2_LAST_UUID].keyString, m_strLastUSB2ID);
    settings->setValue(QSETTINGS_KEYS[SettingsDef::KEY_USB2_LAST_AV_MODE].keyString, m_nLastUSB2AVMode);
    settings->endGroup();
    // } modified by sangmin.seol 2014.09.24 to fix "data load fail"

    // if(bSync) common.sync(); // removed by kihyung 2013.11.05 for ITS 0206507 

    MP_MEDIUM << "nLastEntry =" << nLastEntry << "bJukeboxIndexing =" << bJukeboxIndexing << "bJukeboxScanning =" << bJukeboxScanning << m_strLastUSB1ID << m_strLastUSB2ID << LOG_ENDL;
}

void AppMediaPlayer_SettingsStorage::SaveUSBAudioExtended(AppMediaPlayer_Controller_Audio *controllerAudio, bool bSync)
{
    if (controllerAudio)
    {
        MP_MEDIUM << "START" << bSync << LOG_ENDL;
        
        usbextended.beginGroup(USBEXT_GROUP);

        QList<DeviceHistory*> history = controllerAudio->GetExtendDeviceHistory();

        for (int i = 0; i < history.count(); i++)
        {
            if (history[i])
            {
                usbextended.beginGroup(QString(USBEXT_NAME) + i);

                usbextended.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_MEDIA_DEVICE_ID].keyString, history[i]->DeviceID);
                usbextended.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_MEDIA_MOUNT_PATH].keyString, history[i]->MountPath);
                usbextended.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_INDEX].keyString, history[i]->Index);
                usbextended.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_POSITION].keyString, history[i]->Position);
                usbextended.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_PLAYED_CONTENTS].keyString, (int)history[i]->ContentPlayed);
                usbextended.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_REPEAT_PLAYMODE].keyString, history[i]->repeatPlayMode);
                usbextended.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_RANDOM_PLAYMODE].keyString, history[i]->randomPlayMode);
                usbextended.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_VIDEO_ACTIVE_MODE].keyString, (int)history[i]->mediaType);
                usbextended.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_TITLE].keyString, history[i]->Title);
                usbextended.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_ARTIST].keyString, history[i]->Artist);
                usbextended.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_ALBUM].keyString, history[i]->Album);
                usbextended.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_TABID].keyString, history[i]->listReqData.tabId);
                usbextended.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_QUERY].keyString, history[i]->listReqData.queryStr);
                usbextended.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_ITEM_INDEX].keyString, history[i]->listReqData.itemIndex);
                usbextended.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_LIST_DEPTH].keyString, history[i]->listReqData.depth); 
                
                usbextended.endGroup();
            }
        }
       
        usbextended.endGroup();

        // if(bSync) usbextended.sync(); // removed by kihyung 2013.11.05 for ITS 0206507 

        MP_MEDIUM << "END" << LOG_ENDL;
    }

    SaveUSBPlaylist(controllerAudio, bSync);
}

void AppMediaPlayer_SettingsStorage::SaveUSBVideoExtended(AppMediaPlayer_Video_Controller *controllerVideo, bool bSync)
{
    if (controllerVideo)
    {
        MP_MEDIUM << "START" << LOG_ENDL;
        
        usbvideoextended.beginGroup(USBVIDEOEXT_GROUP);

        QList<AppMediaPlayer_Video_CModel_FS*> history = controllerVideo->GetExtendDeviceHistory();

        for (int i = 0; i < history.count(); i++)
        {
            if (history[i])
            {
                usbvideoextended.beginGroup(QString(USBEXT_NAME) + i);

                usbvideoextended.setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_DEVICE_ID].keyString, history[i]->deviceID());
                usbvideoextended.setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_MOUNT_PATH].keyString, history[i]->mountPath());
                usbvideoextended.setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_FOLDER].keyString, history[i]->foldername());
                usbvideoextended.setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_FULLFOLDER].keyString, history[i]->fullfoldername());
                usbvideoextended.setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_FILE].keyString, history[i]->filename());
                usbvideoextended.setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_POSITION].keyString, history[i]->position());
                usbvideoextended.setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_INDEX].keyString, history[i]->playlistPosition());
                usbvideoextended.setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_SUBTITLE].keyString, history[i]->captionEnable());
                usbvideoextended.setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_SUBTITLE_SIZE].keyString, history[i]->captionSize());
                usbvideoextended.setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_RANDOM].keyString, history[i]->randomMode());
                usbvideoextended.setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_REPEAT].keyString, history[i]->repeatMode());
                
                usbvideoextended.endGroup();

                MP_MEDIUM << i << history[i]->deviceID() << LOG_ENDL;
            }
        }
       
        usbvideoextended.endGroup();

        // if(bSync) usbvideoextended.sync(); // removed by kihyung 2013.11.05 for ITS 0206507 

        MP_MEDIUM << "END" << LOG_ENDL;
    }
}

// { added by kihyung 2013.07.30 for ITS 0181638
void AppMediaPlayer_SettingsStorage::SaveAudioDataForcedSpec(AppMediaPlayer_Controller_Audio *controllerAudio)
{
    if (controllerAudio)
    {
        int device = controllerAudio->getDevice();

        if(device != MP::JUKEBOX && device != MP::USB1 && device != MP::USB2) {
            return;
        }
        
        DeviceHistory *history = controllerAudio->GetDeviceHistory(device);
        if(history == NULL) {
            return;
        }

        int nNewPos = (history->Position / 1000) % 5;
        if(nNewPos != 4) {
            m_nLastAudioPos = nNewPos;
            return;
        }

        if(nNewPos == m_nLastAudioPos) {
            return;
        }
        m_nLastAudioPos = nNewPos;

        MP_MEDIUM << history->Position << LOG_ENDL;
        
        audio.beginGroup(AUDIO_GROUP);
        audio.beginGroup(controllerAudio->DeviceName(device));
        
        audio.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_INDEX].keyString, history->Index);
        audio.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_TRACK_POSITION].keyString, history->Position);
        
        audio.endGroup();
        audio.endGroup();
    }
}

void AppMediaPlayer_SettingsStorage::SaveVideoDataForcedSpec(AppMediaPlayer_Video_Controller *controllerVideo)
{
    if (controllerVideo)
    {
        MEDIA_SOURCE_T  nSource = controllerVideo->getSource();
        if(nSource != MEDIA_SOURCE_JUKEBOX &&
           nSource != MEDIA_SOURCE_USB1   &&
           nSource != MEDIA_SOURCE_USB2)
        {
            return;
        }

        AppMediaPlayer_Video_CModel_FS  *pModel = static_cast<AppMediaPlayer_Video_CModel_FS*>(controllerVideo->getController(nSource)->getModel());
        if(pModel == NULL) {
            return;
        }

        int nNewPos = (pModel->position() / 1000) % 5;
        if(nNewPos != 4) {
            m_nLastVideoPos = nNewPos;
            return;
        }

        if(nNewPos == m_nLastVideoPos) {
            return;
        }
        m_nLastVideoPos = nNewPos;

        MP_MEDIUM << pModel->position() << LOG_ENDL;
        
        video.beginGroup(VIDEO_GROUP);
     
        if(nSource == MEDIA_SOURCE_USB1) {
            video.setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB_POSITION].keyString, pModel->position());
        }
        else if(nSource == MEDIA_SOURCE_USB2) {
            video.setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_USB2_POSITION].keyString, pModel->position());
        }
        else if(nSource == MEDIA_SOURCE_JUKEBOX) {
            video.setValue( QSETTINGS_KEYS[SettingsDef::KEY_VIDEO_JUKEBOX_POSITION].keyString, pModel->position());
        }
        
        video.endGroup();
    }
}
// } added by kihyung 2013.07.30 for ITS 0181638

int AppMediaPlayer_SettingsStorage::GetLastAVEntryPoint()
{
    MP_LOG << "m_nLastAVEntry = " << m_nLastAVEntry << LOG_ENDL;
    return m_nLastAVEntry;
}

bool AppMediaPlayer_SettingsStorage::GetJukeboxIndexingFinished()
{
    MP_LOG << "m_bJukeboxIndexing = " << m_bJukeboxIndexing << LOG_ENDL;
    return m_bJukeboxIndexing;
}

// added by kihyung 2013.07.25 for ITS 0180896 
bool AppMediaPlayer_SettingsStorage::GetJukebxoScanningFinished()
{
    MP_LOG << "m_bJukeboxScanning = " << m_bJukeboxScanning << LOG_ENDL;
    return m_bJukeboxScanning;
}

// { added by kihyung.lee 2013.07.05 for ISV 84474
int AppMediaPlayer_SettingsStorage::GetLoadLastAVEntryPoint()
{
    MP_LOG << "m_nLoadLastAVEntry = " << m_nLoadLastAVEntry << LOG_ENDL;
    return m_nLoadLastAVEntry;
}

void AppMediaPlayer_SettingsStorage::SetLoadLastAVEntryPoint(int nAVEntry)
{
    MP_LOG << "nAVEntry = " << nAVEntry << LOG_ENDL;
    m_nLoadLastAVEntry = nAVEntry;
}
// } modified by kihyung.lee 2013.07.05 for ISV 84474

// { added by kihyung 2013.07.31 for ISV 88437
int AppMediaPlayer_SettingsStorage::GetBootUSBPort()
{
    return m_nBootUSBPort;
}

QString AppMediaPlayer_SettingsStorage::GetBootUSBUUID()
{
    return m_strBootUSBUUID;
}

void AppMediaPlayer_SettingsStorage::SetBootUSBPort(int nPort)
{
    m_nBootUSBPort = nPort;
}

void AppMediaPlayer_SettingsStorage::SetBootUSBUUID(QString strUUID)
{
    m_strBootUSBUUID = strUUID;
}
// } added by kihyung 2013.07.31 for ISV 88437

void AppMediaPlayer_SettingsStorage::SetLastUSB1ID(QString strID, int nAVMode)
{
    m_strLastUSB1ID   = strID;
    m_nLastUSB1AVMode = nAVMode;
}

void AppMediaPlayer_SettingsStorage::SetLastUSB2ID(QString strID, int nAVMode)
{
    m_strLastUSB2ID   = strID;    
    m_nLastUSB2AVMode = nAVMode;
}

int AppMediaPlayer_SettingsStorage::GetLastUSB1AVMode()
{
    return (int)m_nLastUSB1AVMode;
}

int AppMediaPlayer_SettingsStorage::GetLastUSB2AVMode()
{
    return (int)m_nLastUSB2AVMode;
}

QString AppMediaPlayer_SettingsStorage::GetLastUSB1ID()
{
    return m_strLastUSB1ID;
}

QString AppMediaPlayer_SettingsStorage::GetLastUSB2ID()
{
    return m_strLastUSB2ID;    
}

AppMediaPlayer_SettingsStorage*
AppMediaPlayer_SettingsStorage::GetInstance()
{
    if ( m_pInstance == NULL )
    {
        m_pInstance = new AppMediaPlayer_SettingsStorage();
    }
    return m_pInstance;
}

// removed by Dmitry 08.05.13

void AppMediaPlayer_SettingsStorage::SyncAll()
{
    audio.sync();
    playlist.sync();
    photo.sync();
    video.sync();
    common.sync();
    usbextended.sync();
    usbvideoextended.sync();
    usbplaylist.sync();
}

void AppMediaPlayer_SettingsStorage::ResetAudioData(AppMediaPlayer_Controller_Audio *controllerAudio)
{
    MP_MEDIUM << "START" << LOG_ENDL;
    
    for (int device = MP::JUKEBOX; device < MP::ALL_DEVICES; device++)
    {
        controllerAudio->ResetDeviceInfo(device);
        
        DeviceHistory *history = controllerAudio->GetDeviceHistory(device);

        if (history)
        {
            history->DeviceID        = "";
            history->MountPath       = "";
            history->Index           = 0;
            history->Position        = 0;
            history->ContentPlayed   = MP::DEFAULT;
            history->repeatPlayMode  = (int)REPEATALL;
            history->randomPlayMode  = (int)RANDOMOFF;
            history->mediaType       = MEDIA_TYPE_MAX;
            history->Title           = "";
            history->Artist          = "";
            history->Album           = "";
            history->playList.index  = 0;
            history->Quantity        = 0;
            history->listReqData.tabId      = "";
            history->listReqData.queryStr   = "";
            history->listReqData.itemIndex  = 0;
            history->listReqData.depth      = 0;
        }
    }

    //{added by junam 2013.07.04 for ITS172937
    QVariantList categoryDefault;
    for(int idx = 0; idx< 9; idx++)
    {
        categoryDefault.append(QVariant::fromValue(idx));
    }
    controllerAudio->categoryListSetter(categoryDefault);
    //}added by junam

    MP_MEDIUM << "END" << LOG_ENDL;
}

void AppMediaPlayer_SettingsStorage::ResetVideoData(AppMediaPlayer_Video_Controller *controllerVideo)
{
    MP_MEDIUM << LOG_ENDL;
  
    AppMediaPlayer_Video_CModel_FS  *jukeboxData;
    AppMediaPlayer_Video_CModel_FS  *usbData;
    AppMediaPlayer_Video_CModel_FS  *usb2Data;
 
    jukeboxData = static_cast<AppMediaPlayer_Video_CModel_FS*>(controllerVideo->getController(MEDIA_SOURCE_JUKEBOX)->getModel());
    usbData     = static_cast<AppMediaPlayer_Video_CModel_FS*>(controllerVideo->getController(MEDIA_SOURCE_USB1)->getModel());
    usb2Data    = static_cast<AppMediaPlayer_Video_CModel_FS*>(controllerVideo->getController(MEDIA_SOURCE_USB2)->getModel());

    usbData->setDeviceID("");
    usbData->setMountPath("");    
    usbData->setFolderName("");
    usbData->setFullFolderName("");
    usbData->setFilename("");
    usbData->setPosition(0);
    usbData->setPlaylistPosition(0);
    usbData->setCaptionEnable(false);
    usbData->setCaptionSize(AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_NORMAL);
    usbData->setRandomMode(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_OFF);
    usbData->setRepeatMode(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_OFF);

    usb2Data->setDeviceID("");
    usb2Data->setMountPath("");    
    usb2Data->setFolderName("");
    usb2Data->setFullFolderName("");
    usb2Data->setFilename("");
    usb2Data->setPosition(0);
    usb2Data->setPlaylistPosition(0);
    usb2Data->setCaptionEnable(false);
    usb2Data->setCaptionSize(AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_NORMAL);
    usb2Data->setRandomMode(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_OFF);
    usb2Data->setRepeatMode(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_OFF);

    jukeboxData->setDeviceID("");
    jukeboxData->setMountPath("");    
    jukeboxData->setFolderName("");
    jukeboxData->setFullFolderName("");
    jukeboxData->setFilename("");
    jukeboxData->setPosition(0);
    jukeboxData->setPlaylistPosition(0);
    jukeboxData->setCaptionEnable(false);
    jukeboxData->setCaptionSize(AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_NORMAL);
    jukeboxData->setRandomMode(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_OFF);
    jukeboxData->setRepeatMode(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_OFF);
}

// modified by kihyung 2013.07.26 for ITS 0181599 
void AppMediaPlayer_SettingsStorage::ResetPhotoData(PHOTO_LAST_STATE_T* lastState, PHOTO_DISPLAY display)
{
    MP_MEDIUM << LOG_ENDL;
    
    for(int i=0;i<PHOTO_SOURCE_MAX;i++)
    {
        lastState[PHOTO_DISPLAY_FRONT].currentFile[i].clear();
        lastState[PHOTO_DISPLAY_REAR].currentFile[i].clear();
    }

    if(display == PHOTO_DISPLAY_FRONT) {
        lastState[PHOTO_DISPLAY_FRONT].currentFile[PHOTO_SOURCE_JUKEBOX].append("");
        lastState[PHOTO_DISPLAY_FRONT].currentFile[PHOTO_SOURCE_USB_1].append("");
        lastState[PHOTO_DISPLAY_FRONT].currentFile[PHOTO_SOURCE_USB_2].append("");
        lastState[PHOTO_DISPLAY_FRONT].slideshowDelay = 5;
    }

    if(display == PHOTO_DISPLAY_REAR) {
        lastState[PHOTO_DISPLAY_REAR].currentFile[PHOTO_SOURCE_JUKEBOX].append("");
        lastState[PHOTO_DISPLAY_REAR].currentFile[PHOTO_SOURCE_USB_1].append("");
        lastState[PHOTO_DISPLAY_REAR].currentFile[PHOTO_SOURCE_USB_2].append("");
        lastState[PHOTO_DISPLAY_REAR].slideshowDelay = 5;
    }
}

void AppMediaPlayer_SettingsStorage::ResetCommon(AppMediaPlayer_EngineMain *pEngineMain)
{
    MP_MEDIUM << LOG_ENDL;
    
    m_nLastAVEntry     = 0;
    m_bJukeboxIndexing = false;
    m_bJukeboxScanning = false;
    m_strLastUSB1ID    = "";
    m_nLastUSB1AVMode  = APP_MEDIA_MODE_MAX;
    m_strLastUSB2ID    = "";
    m_nLastUSB2AVMode  = APP_MEDIA_MODE_MAX;
}

void AppMediaPlayer_SettingsStorage::ResetUSBAudioExtended(AppMediaPlayer_Controller_Audio *controllerAudio)
{
    if(controllerAudio)
    {
        MP_MEDIUM << LOG_ENDL;
        controllerAudio->ResetExtendDeviceInfo();
    }
}

void AppMediaPlayer_SettingsStorage::ResetUSBVideoExtended(AppMediaPlayer_Video_Controller *controllerVideo)
{
    if(controllerVideo)
    {
        MP_MEDIUM << LOG_ENDL;
        controllerVideo->ResetExtendDeviceInfo();
    }
}

void AppMediaPlayer_SettingsStorage::ResetData(AppMediaPlayer_EngineMain *pEngineMain)
{
    MP_MEDIUM << LOG_ENDL;

    AppMediaPlayer_EnginePhoto      *pEnginePhotoF    = (AppMediaPlayer_EnginePhoto*)pEngineMain->GetEngine(APP_MEDIA_MODE_PHOTO_FRONT);
    PHOTO_LAST_STATE_T              *lastPhotoStateF  = pEnginePhotoF->getLastState();

    AppMediaPlayer_EnginePhoto      *pEnginePhotoR    = (AppMediaPlayer_EnginePhoto*)pEngineMain->GetEngine(APP_MEDIA_MODE_PHOTO_REAR);
    PHOTO_LAST_STATE_T              *lastPhotoStateR  = pEnginePhotoR->getLastState();

    // { modified by kihyung 2013.08.20 for ITS 0181599 
    // lastPhotoStateF[PHOTO_DISPLAY_FRONT].slideshowDelay = 5;
    // lastPhotoStateR[PHOTO_DISPLAY_REAR].slideshowDelay = 5;
    pEnginePhotoF->setCurrentDisplaySlideShowDelay(5);
    pEnginePhotoR->setCurrentDisplaySlideShowDelay(5);
    // } modified by kihyung 2013.08.20 for ITS 0181599 
        
    SavePhotoData(lastPhotoStateF, PHOTO_DISPLAY_FRONT);
    SavePhotoData(lastPhotoStateR, PHOTO_DISPLAY_REAR);
}

// modified by kihyung 2013.07.26 for ITS 0181599 
void AppMediaPlayer_SettingsStorage::FactoryReset(AppMediaPlayer_EngineMain *pEngineMain)
{
    MP_MEDIUM << LOG_ENDL;

    AppMediaPlayer_EngineAudio      *pEngineAudio     = (AppMediaPlayer_EngineAudio*)pEngineMain->GetEngine(APP_MEDIA_MODE_AUDIO);
    AppMediaPlayer_Controller_Audio *controllerAudio  = (AppMediaPlayer_Controller_Audio*)pEngineAudio->GetController();

    AppMediaPlayer_EngineVideo      *pEngineVideo     = (AppMediaPlayer_EngineVideo*)pEngineMain->GetEngine(APP_MEDIA_MODE_VIDEO);
    AppMediaPlayer_Video_Controller *controllerVideo  = (AppMediaPlayer_Video_Controller*)pEngineVideo->GetController();

    AppMediaPlayer_EnginePhoto      *pEnginePhotoF    = (AppMediaPlayer_EnginePhoto*)pEngineMain->GetEngine(APP_MEDIA_MODE_PHOTO_FRONT);
    PHOTO_LAST_STATE_T              *lastPhotoStateF  = pEnginePhotoF->getLastState();

    AppMediaPlayer_EnginePhoto      *pEnginePhotoR    = (AppMediaPlayer_EnginePhoto*)pEngineMain->GetEngine(APP_MEDIA_MODE_PHOTO_REAR);
    PHOTO_LAST_STATE_T              *lastPhotoStateR  = pEnginePhotoR->getLastState();
    
    ResetAudioData(controllerAudio);
    ResetVideoData(controllerVideo);
    ResetPhotoData(lastPhotoStateF, PHOTO_DISPLAY_FRONT);
    ResetPhotoData(lastPhotoStateR, PHOTO_DISPLAY_REAR);
    ResetCommon(pEngineMain);
    ResetUSBAudioExtended(controllerAudio);
    ResetUSBVideoExtended(controllerVideo);

    SaveAudioData(controllerAudio);
    SaveVideoData(controllerVideo);
    SavePhotoData(lastPhotoStateF, PHOTO_DISPLAY_FRONT);
    SavePhotoData(lastPhotoStateR, PHOTO_DISPLAY_REAR);
    SaveCommon(pEngineMain);
    SaveUSBAudioExtended(controllerAudio);
    SaveUSBVideoExtended(controllerVideo);
}

void
AppMediaPlayer_SettingsStorage::LoadPlaylist(AppMediaPlayer_Controller_Audio *controllerAudio)
{
    if (controllerAudio)
    {
        MP_MEDIUM << "START" << LOG_ENDL;
        playlist.beginGroup(PLAYLIST_GROUP);
        for (int device = MP::JUKEBOX; device < MP::ALL_DEVICES; device++)
        {
            DeviceHistory *history = controllerAudio->GetDeviceHistory(device);

            if (history)
            {
                QString         path;
                QList<QVariant> list;

                playlist.beginGroup(controllerAudio->DeviceName(device));
                list = playlist.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_CURRENT_PLAYLIST].keyString, 0).toList();
                playlist.endGroup();

                foreach (QVariant trackPath, list)
                {
                    path = trackPath.toString();
                    history->playList.list->append(path);
                }
                
                history->playList.index  = history->Index;
                history->Quantity        = list.count();
            }
        }
        
        playlist.endGroup();
        MP_MEDIUM << "END" << LOG_ENDL;
    }
}

void 
AppMediaPlayer_SettingsStorage::LoadUSBPlaylist(AppMediaPlayer_Controller_Audio *controllerAudio)
{
    if (controllerAudio)
    {
        MP_MEDIUM << "START" << LOG_ENDL;
        usbplaylist.beginGroup(USBPLAYLIST_GROUP);
              
        QList<DeviceHistory*> history = controllerAudio->GetExtendDeviceHistory();

        for (int i = 0; i < history.count(); i++)
        {
            if (history[i])
            {
                QString         path;
                QList<QVariant> list;

                usbplaylist.beginGroup(QString(USBEXT_NAME) + i);
                list = usbplaylist.value(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_CURRENT_PLAYLIST].keyString, 0).toList();
                usbplaylist.endGroup();

                foreach (QVariant trackPath, list)
                {
                    path = trackPath.toString();
                    history[i]->playList.list->append(path);
                }
                
                history[i]->playList.index  = history[i]->Index;
                history[i]->Quantity        = list.count();
            }
        }
        usbplaylist.endGroup();
        MP_MEDIUM << "END" << LOG_ENDL;
    }
}

void AppMediaPlayer_SettingsStorage::SaveUSBPlaylist(AppMediaPlayer_Controller_Audio *controllerAudio, bool bSync)
{
    if (controllerAudio)
    {
        MP_MEDIUM << "START" << LOG_ENDL;
        
        usbplaylist.beginGroup(USBPLAYLIST_GROUP);

        QList<DeviceHistory*> history = controllerAudio->GetExtendDeviceHistory();
        
        for (int i = 0; i < history.count(); i++)
        {
            if (history[i])
            {               
                QList<QVariant> list;
                foreach (QString filePath, *(history[i]->playList.list))
                {
                    list.append(filePath);
                }

                usbplaylist.beginGroup(QString(USBEXT_NAME) + i);
                usbplaylist.setValue(QSETTINGS_KEYS[SettingsDef::KEY_AUDIO_CURRENT_PLAYLIST].keyString, list);                    
                usbplaylist.endGroup();
            }
        }
       
        usbplaylist.endGroup();

        // if(bSync) usbplaylist.sync(); // removed by kihyung 2013.11.05 for ITS 0206507 

        MP_MEDIUM << "END" << LOG_ENDL;
    }
}
// } added by kihyung 2013.08.09 for ITS 0183077

