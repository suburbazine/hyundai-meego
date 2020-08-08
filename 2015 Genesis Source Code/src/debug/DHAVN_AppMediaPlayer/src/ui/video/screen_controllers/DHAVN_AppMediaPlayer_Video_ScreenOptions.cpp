// { modified by Sergey 01.09.2013 for ITS#186753 (added QT_TR_NOOP only)
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenOptions.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerFS.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerDVD.h" // modified by ravikanth CR 15994 12-11-22

OPTION_MENU_T INIT[] = {
                         {0,0,0,0,0,0,0,0,0}
                       };
// { modified by ravikanth CR 15994 12-11-22
OPTION_MENU_T VIDEO_CAPTION_MENU[] =
{
   { OPTIONS_MENU_TITLE,   "STR_SETTING_DVD_CAPTION_LANG_OPTION", false,  false, false, true, true, NULL, 0 }, // modified by cychoi 2014.02.04 ITS 222651
   { OPTIONS_VIDEO_TITLE,    "STR_SETTING_DVD_CHANGE_CAPTION", false,  false,  false, true, false, NULL, 0 },
   { OPTIONS_VIDEO_SWITCH_CAPTION,   "", false,  false,  false, true, true, NULL, 0 }
};

OPTION_MENU_T VIDEO_VOICE_MENU[] =
{
   { OPTIONS_MENU_TITLE,   "STR_SETTING_DVD_VOICE_LANG", false,  false, false, true, true, NULL, 0 },
   { OPTIONS_VIDEO_TITLE,    "STR_SETTING_DVD_CHANGE_VOICE", false,  false,  false, true, false, NULL, 0 },
   // { modified by dongjin 2013.02.27 for ISV 71500
   //{ OPTIONS_VIDEO_SWITCH_VOICE,   "", false,  false,  false, true, true, NULL, 0 }
   { OPTIONS_VIDEO_SWITCH_VOICE,   "", false,  false,  false, true, true, NULL, 0 },
   { OPTIONS_VIDEO_SWITCH_VOICE2,   "", false,  false,  false, false, true, NULL, 0 }
    // } modified by dongjin
};

OPTION_MENU_T VIDEO_ANGLE_MENU[] =
{
   { OPTIONS_MENU_TITLE,   "STR_SETTING_DVD_ANGLES", false,  false, false, false, true, NULL, 0 },
   { OPTIONS_VIDEO_TITLE,    "STR_SETTING_DVD_CHANGE_ANGLE", false,  false,  false, true, false, NULL, 0 },
   { OPTIONS_VIDEO_SWITCH_ANGLE,   "", false,  false,  false, true, true, NULL, 0 }
};
// } modified by ravikanth CR 15994 12-11-22

OPTION_MENU_T VIDEO_RANDOM_MENU[] =
{
   { OPTIONS_MENU_TITLE,   "",                          false,  false, false, false, true, NULL, 0 },
   { OPTIONS_RANDOM_ON,    "STR_MEDIA_RANDOM_ON", false,  true,  false, true, true, NULL, 0 }, // modified by ravikanth - CR 12958
   { OPTIONS_RANDOM_OFF,   "STR_MEDIA_RANDOM_OFF",      false,  true,  false, true, true, NULL, 0 } // modified by ravikanth - CR 12958
};

OPTION_MENU_T VIDEO_REPEAT_MENU[] =
{
   { OPTIONS_MENU_TITLE,   "",                          false,  false, false, false, true, NULL, 0 },
   // { modified by kihyung 2012.08.20 for 3PDMS 0136482
   /*
   { OPTIONS_REPEAT_ALL,   "STR_MEDIA_REPEAT_FOLDER",   false,  true,  false, true, true, NULL, 0 },
   { OPTIONS_REPEAT_ONE,   "STR_MEDIA_REPEAT_CURRENT",  false,  true,  false, true, true, NULL, 0 }
   */
   { OPTIONS_REPEAT_ALL,   "STR_MEDIA_REPEAT_ALL",          false,  true,  false, true, true, NULL, 0 },
   { OPTIONS_REPEAT_FOLDER,"STR_MEDIA_REPEAT_FOLDER",          false,  true,  false, true, true, NULL, 0 }, // { added by ravikanth - 12-09-19
   { OPTIONS_REPEAT_ONE,   "STR_MEDIA_REPEAT_VIDEO_ONE",    false,  true,  false, true, true, NULL, 0 }
   // } modified by kihyung 
};

// { added by cychoi 2013.06.24 for UX check result by UI Team
OPTION_MENU_T VIDEO_VCD_REPEAT_MENU[] =
{
   { OPTIONS_MENU_TITLE,   "",                          false,  false, false, false, true, NULL, 0 },
   { OPTIONS_REPEAT_ALL,   "STR_MEDIA_REPEAT_ALL",          false,  true,  false, true, true, NULL, 0 },
   { OPTIONS_REPEAT_ONE,   "STR_MEDIA_REPEAT_VCD_ONE",    false,  true,  false, true, true, NULL, 0 } // modified by cychoi 2013.10.24 for UX team REQ
};
// } added by cychoi 2013.06.24

OPTION_MENU_T JUKEBOX_MENU[] =
{
    { OPTIONS_MENU_TITLE,   "",                            false,   false,   false,    false, true, NULL,    0 },
    { OPTIONS_LIST,         "STR_MEDIA_LIST",              false,   false,   false,    true, true, NULL,    0 },
    { OPTIONS_RANDOM,       "STR_MEDIA_SHUFFLE",            false,   false,   true,     true, true, VIDEO_RANDOM_MENU,  sizeof(VIDEO_RANDOM_MENU)/sizeof(OPTION_MENU_T) }, // modified by ravikanth - CR 12958
    { OPTIONS_REPEAT,       "STR_MEDIA_REPEAT",            false,   false,   true,     true, true, VIDEO_REPEAT_MENU,  sizeof(VIDEO_REPEAT_MENU)/sizeof(OPTION_MENU_T) },
    { OPTIONS_CAPTIONS,     "STR_MEDIA_CAPTION_SETTING_OPTION",false,   false,   false,    true, true, NULL,    0 }, // modified by cychoi 2014.02.04 ITS 222651 //added by aettie 20130314 for Dvix Setting
    { OPTIONS_LANGUAGE,     "STR_MEDIA_VOICE_SETTING",  false,   false,   false,    true, true, 0,  0 }, // modified by Sergey 08.08.2013 for Voice Lang in USB/JB.
    //{ OPTIONS_FILE_INFO,    "STR_MEDIA_FILE_INFO",         false,   false,   false,    true, true, NULL,    0 }, //deleted by eunhye 2013.02.17 for UX Scenario 1.Jukebox PlayList
    //{ OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",       false,   false,   false,    true, true, NULL,    0 },
    // { add by Sergey Vetiugov 2012.07.30
    // for CR#11905
    { OPTIONS_DIVX_SETTINGS,"STR_MEDIA_DIVX_SETTINGS",      false,   false,   false,    true,  true, NULL,    0 },
    // } add by Sergey Vetiugov
    { OPTIONS_DISPLAY_SET,  "STR_MEDIA_DISPLAY_SETTINGS",  false,   false,   false,    true, true, NULL,    0 },
    { OPTIONS_SOUND_SET,    "STR_MEDIA_SOUND_SETING",      false,   false,   false,    true, true, NULL,    0 },
    { OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",       false,   false,   false,    true, true, NULL,    0 }//moved by taihyun.ahn 2013.10.25
};

OPTION_MENU_T USB_MENU[] =
{
   { OPTIONS_MENU_TITLE,   "",                            false,   false,   false,    false, true, NULL,    0 },
   { OPTIONS_LIST,         "STR_MEDIA_LIST",              false,   false,   false,    true,  true, NULL,    0 },
   { OPTIONS_RANDOM,       "STR_MEDIA_SHUFFLE",            false,   false,   true,     true,  true, VIDEO_RANDOM_MENU,  sizeof(VIDEO_RANDOM_MENU)/sizeof(OPTION_MENU_T) }, // modified by ravikanth - CR 12958
   { OPTIONS_REPEAT,       "STR_MEDIA_REPEAT",            false,   false,   true,     true,  true, VIDEO_REPEAT_MENU,  sizeof(VIDEO_REPEAT_MENU)/sizeof(OPTION_MENU_T) },
   { OPTIONS_COPY,         "STR_MEDIA_MNG_COPY_TO_JUKEBOX",false,   false,   false,    true,  true, NULL,    0 },//moved by taihyun.ahn 2013.10.28
   { OPTIONS_CAPTIONS,     "STR_MEDIA_CAPTION_SETTING_OPTION",false,   false,   false,    true,  true, NULL,    0 }, // modified by cychoi 2014.02.04 ITS 222651 //added by aettie 20130314 for Dvix Setting
   { OPTIONS_LANGUAGE,     "STR_MEDIA_VOICE_SETTING",  false,   false,   false,    true,  true, 0,  0 }, // modified by Sergey 08.08.2013 for Voice Lang in USB/JB.
   //{ OPTIONS_FILE_INFO,    "STR_MEDIA_FILE_INFO",          false,   false,   false,    true,  true, NULL,    0 }, //deleted by eunhye 2013.02.17 for UX Scenario 1.Jukebox PlayList
   //{ OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",        false,   false,   false,    true,  true, NULL,    0 },
   { OPTIONS_DIVX_SETTINGS,"STR_MEDIA_DIVX_SETTINGS",      false,   false,   false,    true,  true, NULL,    0 },
   { OPTIONS_DISPLAY_SET,  "STR_MEDIA_DISPLAY_SETTINGS",   false,   false,   false,    true,  true, NULL,    0 },
   { OPTIONS_SOUND_SET,    "STR_MEDIA_SOUND_SETING",     false,   false,   false,    true,  true, NULL,    0 },
   { OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",        false,   false,   false,    true,  true, NULL,    0 }//moved by taihyun.ahn 2013.10.25
};

// { modified by cychoi 2013.07.13 for List View spec out
OPTION_MENU_T DVD_MENU[] =
{
   { OPTIONS_MENU_TITLE,   "",                            false,   false,   false,   false, true,  NULL,    0 },
   { OPTIONS_TITLE_MENU,   "STR_MEDIA_TITLE_MENU",        false,   false,   false,    true, true,   NULL,    0 },
   { OPTIONS_DISC_MENU,    "STR_MEDIA_DISC_MENU",         false,   false,   false,   true,  true,  NULL,    0 },
   // { modified by ravikanth CR 15994 12-11-22
   { OPTIONS_VIDEO_CAPTION,"STR_SETTING_DVD_CAPTION_LANG_OPTION", false,   false,   true,     true,  true, VIDEO_CAPTION_MENU,  sizeof(VIDEO_CAPTION_MENU)/sizeof(OPTION_MENU_T) }, // modified by cychoi 2014.02.04 ITS 222651
   { OPTIONS_VIDEO_VOICE,  "STR_SETTING_DVD_VOICE_LANG", false,   false,   true,     true,  true, VIDEO_VOICE_MENU,  sizeof(VIDEO_VOICE_MENU)/sizeof(OPTION_MENU_T) },
   { OPTIONS_VIDEO_ANGLE,  "STR_SETTING_DVD_ANGLES",    false,   false,   true,     true,  true, VIDEO_ANGLE_MENU,  sizeof(VIDEO_ANGLE_MENU)/sizeof(OPTION_MENU_T) },
   // } modified by ravikanth CR 15994 12-11-22
   { OPTIONS_SETTINGS,     "STR_SETTING_DVD_SETTING_OPTION",     false,   false,   false,    true, true,   NULL,    0 }, // modified by cychoi 2014.02.04 ITS 222651
   //{ OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",       false,   false,   false,    true, true,   NULL,    0 },
   { OPTIONS_DISPLAY_SET,  "STR_MEDIA_DISPLAY_SETTINGS",  false,   false,   false,    true, true,   NULL,    0 },
   { OPTIONS_SOUND_SET,    "STR_MEDIA_SOUND_SETING",      false,   false,   false,    true, true,   NULL,    0 },
   { OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",       false,   false,   false,    true, true,   NULL,    0 }
};

// removed by cychoi 2013.07.31 for unused code

OPTION_MENU_T VCD_MENU[] =
{
   { OPTIONS_MENU_TITLE,   "",                            false,   false,   false,   false, true,  NULL,    0 },
   { OPTIONS_REPEAT,       "STR_MEDIA_REPEAT",            false,   false,   false,     true, true,  VIDEO_VCD_REPEAT_MENU/*VIDEO_REPEAT_MENU*/,  sizeof(VIDEO_VCD_REPEAT_MENU/*VIDEO_REPEAT_MENU*/)/sizeof(OPTION_MENU_T) }, // modified by yungi 2013.09.27 ITS 0191791 // modified by cychoi 2013.06.24 for UX check result by UI Team
   { OPTIONS_DIRECT_ACCESS,"STR_MEDIA_SHORTCUT",          false,   false,   false,   true,  true,  NULL,    0 },
   { OPTIONS_PBC_ONOFF,    "STR_MEDIA_PBC",               true,   false,   false,   true,   true,  NULL,    0 },
   { OPTIONS_PBC_MENU,     "STR_MEDIA_PBC_MENU_OPTION",   false,   false,   false,   true,  false,  NULL,    0 }, // modified by cychoi 2014.02.04 ITS 222651
   //{ OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",       false,   false,   false,    true, true,  NULL,    0 },
   { OPTIONS_DISPLAY_SET,  "STR_MEDIA_DISPLAY_SETTINGS",  false,   false,   false,    true, true,  NULL,    0 },
   { OPTIONS_SOUND_SET,    "STR_MEDIA_SOUND_SETING",      false,   false,   false,    true, true,  NULL,    0 },
   { OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",       false,   false,   false,    true, true,  NULL,    0 }
};
// } modified by cychoi 2013.07.13

OPTION_MENU_T AUX_MENU[] =
{
   { OPTIONS_MENU_TITLE,   "",                          false,   false,   false,   false, true,  NULL,    0 },
   //{ OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",     false,   false,   false,   true, true,   NULL,    0 },
   { OPTIONS_DISPLAY_SET,  "STR_MEDIA_DISPLAY_SETTINGS",false,   false,   false,   true, true,   NULL,    0 },
   { OPTIONS_SOUND_SET,    "STR_MEDIA_SOUND_SETING",    false,   false,   false,   true, true,   NULL,    0 },
   { OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",     false,   false,   false,   true, true,   NULL,    0 }
};
// { modified by edo.lee 2013.07.05 ITS 0175753
OPTION_MENU_T JUKEBOX_DRS_MENU[] =
{
    { OPTIONS_MENU_TITLE,   "",                            false,   false,   false,    false, true, NULL,    0 },
    { OPTIONS_LIST,         "STR_MEDIA_LIST",              false,   false,   false,    true, true, NULL,    0 },
    { OPTIONS_RANDOM,       "STR_MEDIA_SHUFFLE",            false,   false,   true,     true, true, VIDEO_RANDOM_MENU,  sizeof(VIDEO_RANDOM_MENU)/sizeof(OPTION_MENU_T) }, // modified by ravikanth - CR 12958
    { OPTIONS_REPEAT,       "STR_MEDIA_REPEAT",            false,   false,   true,     true, true, VIDEO_REPEAT_MENU,  sizeof(VIDEO_REPEAT_MENU)/sizeof(OPTION_MENU_T) },
    { OPTIONS_CAPTIONS,     "STR_MEDIA_CAPTION_SETTING_OPTION",false,   false,   false,    true, true, NULL,    0 }, // modified by cychoi 2014.02.04 ITS 222651 //added by aettie 20130314 for Dvix Setting
    { OPTIONS_LANGUAGE,     "STR_MEDIA_VOICE_SETTING",  false,   false,   false,    true,  true, 0,  0 }, // modified by Sergey 08.08.2013 for Voice Lang in USB/JB.
    //{ OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",       false,   false,   false,    true, true, NULL,    0 },
    { OPTIONS_DIVX_SETTINGS,"STR_MEDIA_DIVX_SETTINGS",      false,   false,   false,    true,  true, NULL,    0 },
    { OPTIONS_DISPLAY_SET,  "STR_MEDIA_DISPLAY_SETTINGS",  false,   false,   false,    true, false, NULL,    0 },
    { OPTIONS_SOUND_SET,    "STR_MEDIA_SOUND_SETING",      false,   false,   false,    true, true, NULL,    0 },
    { OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",       false,   false,   false,    true, true, NULL,    0 }//moved by taihyun.ahn 2013.10.25
};

OPTION_MENU_T USB_DRS_MENU[] =
{
   { OPTIONS_MENU_TITLE,   "",                            false,   false,   false,    false, true, NULL,    0 },
   { OPTIONS_LIST,         "STR_MEDIA_LIST",              false,   false,   false,    true,  true, NULL,    0 },
   { OPTIONS_RANDOM,       "STR_MEDIA_SHUFFLE",            false,   false,   true,     true,  true, VIDEO_RANDOM_MENU,  sizeof(VIDEO_RANDOM_MENU)/sizeof(OPTION_MENU_T) }, // modified by ravikanth - CR 12958
   { OPTIONS_REPEAT,       "STR_MEDIA_REPEAT",            false,   false,   true,     true,  true, VIDEO_REPEAT_MENU,  sizeof(VIDEO_REPEAT_MENU)/sizeof(OPTION_MENU_T) },
   { OPTIONS_COPY,         "STR_MEDIA_MNG_COPY_TO_JUKEBOX",false,   false,   false,    true,  true, NULL,    0 }, // modified by cychoi 2014.02.04 ITS 222651 //moved by taihyun.ahn 2013.10.28
   { OPTIONS_CAPTIONS,     "STR_MEDIA_CAPTION_SETTING_OPTION",false,   false,   false,    true,  true, NULL,    0 }, //added by aettie 20130314 for Dvix Setting   
   { OPTIONS_LANGUAGE,     "STR_MEDIA_VOICE_SETTING",  false,   false,   false,    true,  true, 0,  0 }, // modified by Sergey 08.08.2013 for Voice Lang in USB/JB.
   //{ OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",        false,   false,   false,    true,  true, NULL,    0 },
   { OPTIONS_DIVX_SETTINGS,"STR_MEDIA_DIVX_SETTINGS",      false,   false,   false,    true,  true, NULL,    0 },
   { OPTIONS_DISPLAY_SET,  "STR_MEDIA_DISPLAY_SETTINGS",   false,   false,   false,    true,  false, NULL,    0 },
   { OPTIONS_SOUND_SET,    "STR_MEDIA_SOUND_SETING",     false,   false,   false,    true,  true, NULL,    0 },
   { OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",        false,   false,   false,    true,  true, NULL,    0 }//moved by taihyun.ahn 2013.10.25
};

// { modified by cychoi 2013.07.13 for List View spec out
OPTION_MENU_T DVD_DRS_MENU[] =
{
   { OPTIONS_MENU_TITLE,   "",                            false,   false,   false,   false, true,  NULL,    0 },
   { OPTIONS_TITLE_MENU,   "STR_MEDIA_TITLE_MENU",        false,   false,   false,    true, false,   NULL,    0 },
   { OPTIONS_DISC_MENU,    "STR_MEDIA_DISC_MENU",         false,   false,   false,   true,  false,  NULL,    0 },
   { OPTIONS_VIDEO_CAPTION,"STR_SETTING_DVD_CAPTION_LANG_OPTION", false,   false,   true,     true,  true, VIDEO_CAPTION_MENU,  sizeof(VIDEO_CAPTION_MENU)/sizeof(OPTION_MENU_T) }, // modified by cychoi 2014.02.04 ITS 222651
   { OPTIONS_VIDEO_VOICE,  "STR_SETTING_DVD_VOICE_LANG", false,   false,   true,     true,  true, VIDEO_VOICE_MENU,  sizeof(VIDEO_VOICE_MENU)/sizeof(OPTION_MENU_T) },
   { OPTIONS_VIDEO_ANGLE,  "STR_SETTING_DVD_ANGLES",    false,   false,   true,     true,  true, VIDEO_ANGLE_MENU,  sizeof(VIDEO_ANGLE_MENU)/sizeof(OPTION_MENU_T) },
   { OPTIONS_SETTINGS,     "STR_SETTING_DVD_SETTING_OPTION",     false,   false,   false,    true, true,   NULL,    0 }, // modified by cychoi 2014.02.04 ITS 222651
   //{ OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",       false,   false,   false,    true, true,   NULL,    0 },
   { OPTIONS_DISPLAY_SET,  "STR_MEDIA_DISPLAY_SETTINGS",  false,   false,   false,    true, false,   NULL,    0 },
   { OPTIONS_SOUND_SET,    "STR_MEDIA_SOUND_SETING",      false,   false,   false,    true, true,   NULL,    0 },
   { OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",       false,   false,   false,    true, true,   NULL,    0 }
};

// removed by cychoi 2013.07.31 for unused code

OPTION_MENU_T VCD_DRS_MENU[] =
{
   { OPTIONS_MENU_TITLE,   "",                            false,   false,   false,   false, true,  NULL,    0 },
   { OPTIONS_REPEAT,       "STR_MEDIA_REPEAT",            false,   false,   true,     true, true,  VIDEO_VCD_REPEAT_MENU,  sizeof(VIDEO_VCD_REPEAT_MENU)/sizeof(OPTION_MENU_T) }, 
   { OPTIONS_DIRECT_ACCESS,"STR_MEDIA_SHORTCUT",          false,   false,   false,   true,  true,  NULL,    0 },
   { OPTIONS_PBC_ONOFF,    "STR_MEDIA_PBC",               true,   false,   false,   true,   true,  NULL,    0 },
   { OPTIONS_PBC_MENU,     "STR_MEDIA_PBC_MENU_OPTION",   false,   false,   false,   true,  false,  NULL,    0 },  // modified by cychoi 2014.02.04 ITS 222651
   //{ OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",       false,   false,   false,    true, true,  NULL,    0 },
   { OPTIONS_DISPLAY_SET,  "STR_MEDIA_DISPLAY_SETTINGS",  false,   false,   false,    true, false,  NULL,    0 },
   { OPTIONS_SOUND_SET,    "STR_MEDIA_SOUND_SETING",      false,   false,   false,    true, true,  NULL,    0 },
   { OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",       false,   false,   false,    true, true,  NULL,    0 }
};
// } modified by cychoi 2013.07.13

OPTION_MENU_T AUX_DRS_MENU[] =
{
   { OPTIONS_MENU_TITLE,   "",                          false,   false,   false,   false, true,  NULL,    0 },
   //{ OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",     false,   false,   false,   true, true,   NULL,    0 },
   { OPTIONS_DISPLAY_SET,  "STR_MEDIA_DISPLAY_SETTINGS",false,   false,   false,   true, false,   NULL,    0 },
   { OPTIONS_SOUND_SET,    "STR_MEDIA_SOUND_SETING",    false,   false,   false,   true, true,   NULL,    0 },
   { OPTIONS_FULL_SCREEN,  "STR_MEDIA_FULL_SCREEN",     false,   false,   false,   true, true,   NULL,    0 }
};


AppMediaPlayer_Video_ScreenOptions::
AppMediaPlayer_Video_ScreenOptions( AppMediaPlayer_Video_Controller *pController,
                                    AppMediaPlayer_EngineVideo *pEngineVideo,
                                    int disp,
                                    QObject *parent )
    : AppMediaPlayer_Video_Screen( parent ),
      m_pEngineVideo(pEngineVideo),
      m_pController(pController),
      optionsMenuData(NULL),
      m_pVideoLangugeSettingsMenu(NULL),
      m_disp(disp),
      m_bCloseInBG(false) // modified by Sergey 06.08.2013 to close menu in good time
{
    Q_UNUSED(pController);

   qmlRegisterUncreatableType<OptionMenuModel>("OptionMenuModel", 1, 0, "OptionMenuModel", "OptionMenuModel Can not be created from qml");
   m_pModel = new AppMediaPlayer_Video_SModel_Options(this);
   m_pModel->setOptionMenuModel(new OptionMenuModel( INIT, sizeof(INIT) / sizeof(OPTION_MENU_T) ));
   setModel( m_pModel );
   vcdData = static_cast<AppMediaPlayer_Video_CModel_VCD*>(pController->getController(MEDIA_SOURCE_VCD)->getModel());

   dvdData = static_cast<AppMediaPlayer_Video_CModel_DVD*>(pController->getController(MEDIA_SOURCE_DVDVIDEO)->getModel()); // modified by ravikanth CR 15994 12-11-22

   connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(m_pController->getController(MEDIA_SOURCE_JUKEBOX)->getModel()),
           SIGNAL(playlistPositionChanged(int)),
           this,
           SLOT(closeOptionsIfAudioLangModel()));

   connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(m_pController->getController(MEDIA_SOURCE_USB1)->getModel()),
           SIGNAL(playlistPositionChanged(int)),
           this,
           SLOT(closeOptionsIfAudioLangModel()));

   connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(m_pController->getController(MEDIA_SOURCE_USB2)->getModel()),
           SIGNAL(playlistPositionChanged(int)),
           this,
           SLOT(closeOptionsIfAudioLangModel()));
	// { modified by ravikanth CR 15994 12-11-22
   // { modified by dongjin 2013.02.27 for ISV 71500
   /*connect(qobject_cast<AppMediaPlayer_Video_CModel_DVD*>(m_pController->getController(MEDIA_SOURCE_DVDVIDEO)->getModel()),
           SIGNAL(activeAudioLangStringChanged(QString)),
           this,
           SLOT(onActiveAudioLangChanged(QString)));*/
   connect(qobject_cast<AppMediaPlayer_Video_CModel_DVD*>(m_pController->getController(MEDIA_SOURCE_DVDVIDEO)->getModel()),
              SIGNAL(activeAudioLangStringChanged(QString, QString)),
              this,
              SLOT(onActiveAudioLangChanged(QString, QString)));
   // } modified by dongjin
   connect(qobject_cast<AppMediaPlayer_Video_CModel_DVD*>(m_pController->getController(MEDIA_SOURCE_DVDVIDEO)->getModel()),
           SIGNAL(activeCaptionStringChanged(QString)),
           this,
           SLOT(onActiveCaptionChanged(QString)));
   connect(qobject_cast<AppMediaPlayer_Video_CModel_DVD*>(m_pController->getController(MEDIA_SOURCE_DVDVIDEO)->getModel()),
           SIGNAL(activeAngleChanged(int)),
           this,
           SLOT(onActiveAngleChanged(int)));
   // } modified by ravikanth CR 15994 12-11-22

    // removed by cychoi 2014.03.18 for ITS 229882 No option menu on Title/Disc Menu Screen //{ added by yongkyun.lee 2013-11-05 for ITS 206356
   connect(m_pEngineVideo->getScreenManager(m_disp), SIGNAL(fgBgStateChanged(bool)), this, SLOT(onFgBg(bool))); // added by Sergey 06.08.2013 to close menu in good time

   // { added by kihyung 2013.08.22 for ITS 0185761
   connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(m_pController->getController(MEDIA_SOURCE_JUKEBOX)->getModel()),
           SIGNAL(currentSubtitleSettingChanged(int)),
           this, 
           SLOT(onSubtitleSettingChanged(int)));
   
   connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(m_pController->getController(MEDIA_SOURCE_USB1)->getModel()),
           SIGNAL(currentSubtitleSettingChanged(int)),
           this, 
           SLOT(onSubtitleSettingChanged(int)));
   
   connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(m_pController->getController(MEDIA_SOURCE_USB2)->getModel()),
           SIGNAL(currentSubtitleSettingChanged(int)),
           this, 
           SLOT(onSubtitleSettingChanged(int)));
   // } added by kihyung 2013.08.22 for ITS 0185761

   // { added by Sergey 09.10.2013 for ITS#192050
   connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(m_pController->getController(MEDIA_SOURCE_JUKEBOX)->getModel()),
           SIGNAL(voiceLangEnabledChanged(bool)),
           this,
           SLOT(onVoiceLangEnabled(bool)));

   connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(m_pController->getController(MEDIA_SOURCE_USB1)->getModel()),
           SIGNAL(voiceLangEnabledChanged(bool)),
           this,
           SLOT(onVoiceLangEnabled(bool)));

   connect(qobject_cast<AppMediaPlayer_Video_CModel_FS*>(m_pController->getController(MEDIA_SOURCE_USB2)->getModel()),
           SIGNAL(voiceLangEnabledChanged(bool)),
           this,
           SLOT(onVoiceLangEnabled(bool)));
	// } added by Sergey 09.10.2013 for ITS#192050
           
   //added by edo.lee 2013.07.15 STATIC TEST
   m_pControllerModel = NULL;
   fsData = NULL;
   jukeboxData = NULL;
   usbData = NULL;
   //added by edo.lee 2013.07.15 STATIC TEST   	
   m_isLockoutView = false; // added by cychoi 2013-07-16 for Prevent fix
   m_isLockout = false; //added by jaehwan 2013-10-25 for fix static test issue
}

AppMediaPlayer_Video_ScreenOptions::~AppMediaPlayer_Video_ScreenOptions()
{
}

void AppMediaPlayer_Video_ScreenOptions::onHide()
{
    emit closeOptions(false);  // removed by Sergey 08.08.2013 to fix double OptionsMenu
}

// { modified by Sergey 06.08.2013 to close menu in good time
void AppMediaPlayer_Video_ScreenOptions::onDeactivate()
{
}
// } modified by Sergey 06.08.2013 to close menu in good time

/** @copydoc AppMediaPlayer_Video_Screen::onShow() */
void AppMediaPlayer_Video_ScreenOptions::onShow( QVariant arg )
{
    MP_LOG << arg << m_disp << LOG_ENDL;
    OPTION_MENU_T* optionsMenuData = NULL;
    int optionsMenuSize = 0;
//    m_isLockoutView = m_pEngineVideo->getIsShowLockoutView() == m_disp; //added by edo.lee 2013.05.02
	m_isLockoutView = m_pEngineVideo->getScreenManager(m_disp)->getIsLockoutMode(); // added by Sergey 10.12.2013 for ITS#213582
	// removed by Sergey 14.08.2013 for ITS#184379
	VIDEO_REPEAT_MENU[0].sText = QT_TR_NOOP("STR_MEDIA_REPEAT");
	VIDEO_RANDOM_MENU[0].sText = QT_TR_NOOP("STR_MEDIA_SHUFFLE"); // modified by ravikanth - CR 12958

    VIDEO_REPEAT_MENU[2].bEnabled = true; // added by ravikanth - 12-09-19

	if(!arg.toString().isEmpty())
		m_menuType = arg.toString(); // added by Sergey 16.09.2013 for ITS#185236

	// { modified by Sergey 14.08.2013 for ITS#184379
	if(m_menuType == "USB") // modified by Sergey 16.09.2013 for ITS#185236
    {
        AppMediaPlayer_Video_ControllerBase* pFSCtrl = m_pController->getCurrentController();

        if(pFSCtrl)
        {
            fsData = static_cast<AppMediaPlayer_Video_CModel_FS*>(m_pController->getCurrentController()->getModel());
            setRepeatRandomeModel(fsData->repeatMode(), fsData->randomMode());

            if(fsData->voiceLangEnabled()) // added by Sergey 09.10.2013 for ITS#192050
                setLanguageSettings(fsData->getLanguageCode(), fsData->getCurrentStreamId());  // modified by Sergey 08.08.2013 for Voice Lang in USB/JB

            if(m_isLockoutView)
            {
                USB_DRS_MENU[0].sText = QT_TR_NOOP("STR_SETTING_SYSTEM_DISPLAY");
                USB_DRS_MENU[5].bEnabled = m_pController->getCurrentController()->subTitle();//modified by taihyun.ahn 2013.10.28
                USB_DRS_MENU[6].bEnabled = fsData->voiceLangEnabled(); //modified by taihyun.ahn 2013.10.28
                optionsMenuData = USB_DRS_MENU;
                optionsMenuSize = sizeof(USB_DRS_MENU) / sizeof(OPTION_MENU_T);
            }
            else
            {
                USB_MENU[0].sText = QT_TR_NOOP("STR_SETTING_SYSTEM_DISPLAY");
                USB_MENU[5].bEnabled = m_pController->getCurrentController()->subTitle(); //modified by taihyun.ahn 2013.10.28
                USB_MENU[6].bEnabled = fsData->voiceLangEnabled(); //modified by taihyun.ahn 2013.10.28
                optionsMenuData = USB_MENU;
                optionsMenuSize = sizeof(USB_MENU) / sizeof(OPTION_MENU_T);
            }
        }
    }
    else if(m_menuType == "JUKEBOX") // modified by Sergey 16.09.2013 for ITS#185236
    {
        AppMediaPlayer_Video_ControllerBase* pFSCtrl = m_pController->getCurrentController();

        if(pFSCtrl)
        {
            fsData = static_cast<AppMediaPlayer_Video_CModel_FS*>(m_pController->getCurrentController()->getModel());
            setRepeatRandomeModel(fsData->repeatMode(), fsData->randomMode());

            if(fsData->voiceLangEnabled()) // added by Sergey 09.10.2013 for ITS#192050
                setLanguageSettings(fsData->getLanguageCode(), fsData->getCurrentStreamId()); // modified by Sergey 08.08.2013 for Voice Lang in USB/JB

            if(m_isLockoutView)
            {
                JUKEBOX_DRS_MENU[0].sText = QT_TR_NOOP("STR_SETTING_SYSTEM_DISPLAY");
                JUKEBOX_DRS_MENU[4].bEnabled = m_pController->getCurrentController()->subTitle();
                JUKEBOX_DRS_MENU[5].bEnabled = fsData->voiceLangEnabled(); // added by Sergey 09.10.2013 for ITS#192050
                optionsMenuData = JUKEBOX_DRS_MENU;
                optionsMenuSize = sizeof(JUKEBOX_DRS_MENU) / sizeof(OPTION_MENU_T);
            }
            else
            {
                JUKEBOX_MENU[0].sText = QT_TR_NOOP("STR_SETTING_SYSTEM_DISPLAY");
                JUKEBOX_MENU[4].bEnabled = m_pController->getCurrentController()->subTitle();
                JUKEBOX_MENU[5].bEnabled = fsData->voiceLangEnabled(); // added by Sergey 09.10.2013 for ITS#192050
                optionsMenuData = JUKEBOX_MENU;
                optionsMenuSize = sizeof(JUKEBOX_MENU) / sizeof(OPTION_MENU_T);
            }
        }
    }
    // } modified by Sergey 14.08.2013 for ITS#184379
    else if(m_menuType == "DVD") // modified by Sergey 16.09.2013 for ITS#185236
    {
        // { modified by cychoi 2013.07.13 for List View spec out
        // { modified by cychoi 2013.07.06 for List is disabled on No DRS
        if(m_isLockoutView)
        {
            DVD_DRS_MENU[0].sText = QT_TR_NOOP("STR_SETTING_SYSTEM_DISPLAY");
            // { modified by cychoi 2014.02.19 for (HMC Request)
            DVD_DRS_MENU[1].bEnabled = true; 
            DVD_DRS_MENU[2].bEnabled = true; 
            // } modified by cychoi 2014.02.19
            // { modified by cychoi 2013.10.27 for ITS 198873
            DVD_DRS_MENU[7].bEnabled = false; // OPTIONS_DISPLAY_SET
            DVD_DRS_MENU[9].bEnabled = true; // OPTIONS_FULL_SCREEN
            // } modified by cychoi 2013.10.27
            optionsMenuData = DVD_DRS_MENU;
            optionsMenuSize = sizeof(DVD_DRS_MENU) / sizeof(OPTION_MENU_T);
        }
        else
        {
            DVD_MENU[0].sText = QT_TR_NOOP("STR_SETTING_SYSTEM_DISPLAY");
            // { modified by cychoi 2013.10.27 for ITS 198873
            DVD_MENU[9].bEnabled = true; // OPTIONS_FULL_SCREEN
            // } modified by cychoi 2013.10.27
            optionsMenuData = DVD_MENU;
            optionsMenuSize = sizeof(DVD_MENU) / sizeof(OPTION_MENU_T);
        }
        // } modified by cychoi 2013.07.06
        // } modified by cychoi 2013.07.13
    }
    else if(m_menuType == "DVD_TITLE_MENU") // modified by Sergey 16.09.2013 for ITS#185236
    {
        // { modified by cychoi 2013.07.13 for List View spec out
        // { modified by cychoi 2013.07.06 for code arrangement
        if(m_isLockoutView)
        {
            DVD_DRS_MENU[0].sText = QT_TR_NOOP("STR_SETTING_SYSTEM_DISPLAY");
            // { modified by cychoi 2014.02.19 for (HMC Request)
            DVD_DRS_MENU[1].bEnabled = true; 
            DVD_DRS_MENU[2].bEnabled = true; 
            // } modified by cychoi 2014.02.19
            // { modified by cychoi 2013.10.27 for ITS 198873
            DVD_DRS_MENU[7].bEnabled = false; // OPTIONS_DISPLAY_SET
            DVD_DRS_MENU[9].bEnabled = false; // OPTIONS_FULL_SCREEN
            // } modified by cychoi 2013.10.27
            optionsMenuData = DVD_DRS_MENU;
            optionsMenuSize = sizeof( DVD_DRS_MENU) / sizeof(OPTION_MENU_T);
        }
        else
        {
            DVD_MENU[0].sText = QT_TR_NOOP("STR_SETTING_SYSTEM_DISPLAY");
            // { modified by cychoi 2013.10.27 for ITS 198873
            DVD_MENU[9].bEnabled = false; // OPTIONS_FULL_SCREEN
            // } modified by cychoi 2013.10.27
            optionsMenuData = DVD_MENU;
            optionsMenuSize = sizeof( DVD_MENU) / sizeof(OPTION_MENU_T);
        }            
        // } added by cychoi 2013.06.09
        // } modified by cychoi 2013.07.13
    }
    else if(m_menuType == "VCD") // modified by Sergey 16.09.2013 for ITS#185236
    {
        // { modified by cychoi 2013.07.13 for List View spec out
        // { modified by cychoi 2013.07.06 for VCD repeat menu
        VIDEO_VCD_REPEAT_MENU[0].sText = QT_TR_NOOP("STR_MEDIA_REPEAT"); // added by cychoi 2013.07.06 for VCD repeat menu
        setVCDRepeatModel(vcdData->repeatMode());
        //VIDEO_REPEAT_MENU[2].bEnabled = false; // added by ravikanth - 12-09-19
        //setRepeatRandomeModel(vcdData->repeatMode(), vcdData->randomMode());
        // } modified by cychoi 2013.07.06
        // { modified by cychoi 2013.07.06 for PBC On is disabled on No DRS
        if(m_isLockoutView)
        {
            VCD_DRS_MENU[0].sText = QT_TR_NOOP("STR_SETTING_SYSTEM_DISPLAY");
            // { commented by cychoi 2014.02.19 for (HMC Request)
            //VCD_DRS_MENU[3].bEnabled = false; 
            //VCD_DRS_MENU[4].bEnabled = false; // added by cychoi 2014.01.23 for ITS 221992
            // } commented by cychoi 2014.02.19 for (HMC Request)
            // { modified by cychoi 2013.10.27 for ITS 198873
            VCD_DRS_MENU[5].bEnabled = false; // OPTIONS_DISPLAY_SET
            // } modified by cychoi 2013.10.27
            // { modified by cychoi 2014.02.19 for (HMC Request)
            if(VCD_DRS_MENU[3].bSelected)
            {
                VCD_DRS_MENU[6].bEnabled = false; // OPTIONS_SOUND_SET
            }
            else
            {
                VCD_DRS_MENU[6].bEnabled = true; // OPTIONS_SOUND_SET
            }
            // } modified by cychoi 2014.02.19
            optionsMenuData = VCD_DRS_MENU;
            optionsMenuSize = sizeof(VCD_DRS_MENU) / sizeof(OPTION_MENU_T);	
        }
        else
        {
            VCD_MENU[0].sText = QT_TR_NOOP("STR_SETTING_SYSTEM_DISPLAY");
            // { added by cychoi 2014.01.23 for ITS 221992
            if(VCD_MENU[3].bSelected)
            {
                VCD_MENU[5].bEnabled = false; // OPTIONS_DISPLAY_SET
                VCD_MENU[6].bEnabled = false; // OPTIONS_SOUND_SET
            }
            else
            {
                VCD_MENU[5].bEnabled = true; // OPTIONS_DISPLAY_SET
                VCD_MENU[6].bEnabled = true; // OPTIONS_SOUND_SET
            }
            // } added by cychoi 2014.01.23
            optionsMenuData = VCD_MENU;
            optionsMenuSize = sizeof(VCD_MENU) / sizeof(OPTION_MENU_T);	
        }
        // } modified by cychoi 2013.07.06
        // } modified by cychoi 2013.07.13
    }
    else if (m_menuType == "AUX") // modified by Sergey 16.09.2013 for ITS#185236
    {
        //AUX_MENU[0].sText = QT_TR_NOOP("STR_SETTING_SYSTEM_DISPLAY");
		//AUX_MENU[2].bEnabled = isLockoutView == false;//added by edo.lee 2013.05.24
        if(m_isLockoutView)
    	{
            AUX_DRS_MENU[0].sText = QT_TR_NOOP("STR_SETTING_SYSTEM_DISPLAY");
            //{  modified by cychoi 2013.10.27 for ITS 198873
			AUX_DRS_MENU[1].bEnabled = false; // OPTIONS_DISPLAY_SET
            // } modified by cychoi 2013.10.27
			optionsMenuData = AUX_DRS_MENU;
			optionsMenuSize = sizeof(AUX_DRS_MENU) / sizeof(OPTION_MENU_T);
    	}
		else
		{
			AUX_MENU[0].sText = QT_TR_NOOP("STR_SETTING_SYSTEM_DISPLAY");
			optionsMenuData = AUX_MENU;
			optionsMenuSize = sizeof(AUX_MENU) / sizeof(OPTION_MENU_T);
		}
    }

    // { modified by cychoi 2013.12.05 for ITS 212884 // { modified by ravikanth CR 15994 12-11-22
    VIDEO_CAPTION_MENU[2].sText = dvdData->activeCaption();
    VIDEO_VOICE_MENU[2].sText = dvdData->activeAudioLang();
    VIDEO_VOICE_MENU[3].sText = dvdData->activeAudioLang2();  // added by dongjin 2013.02.27 for ISV 71500
    VIDEO_VOICE_MENU[3].bEnabled = false; // added by cychoi 2013.06.30 for New UX
    VIDEO_ANGLE_MENU[2].sText = QString::number(dvdData->activeAngle());
	// } modified by cychoi 2013.12.05 // } modified by ravikanth CR 15994 12-11-22

    // { added by wspark 2014.01.21 for ITS 221677
    int voiceMenuSize = sizeof(VIDEO_VOICE_MENU)/sizeof(OPTION_MENU_T);
    if(dvdData->activeAudioLangNo() == 0) // No Audio
    {
        // VIDEO_VOICE_MENU
        DVD_MENU[4].nCountItemAttachedMenu = voiceMenuSize - 1;
        DVD_DRS_MENU[4].nCountItemAttachedMenu = voiceMenuSize - 1;
    }
    else
    {
        DVD_MENU[4].nCountItemAttachedMenu = voiceMenuSize;
        DVD_DRS_MENU[4].nCountItemAttachedMenu = voiceMenuSize;
    }
    // } added by wspark

    if(m_pModel->getOptionMenuModel())
    {    
        m_pModel->getOptionMenuModel()->updateModel( optionsMenuData,
                                                     optionsMenuSize );		
    }
    else
    {
        m_pModel->setOptionMenuModel(new OptionMenuModel( optionsMenuData,
                                                          optionsMenuSize ));
    }

	m_bCloseInBG = false; // modified by Sergey 06.08.2013 to close menu in good time
	emit showOptions(); // added by Sergey 02.08.2103 for ITS#181512
}

void AppMediaPlayer_Video_ScreenOptions::onMouseReleased()
{
}


void AppMediaPlayer_Video_ScreenOptions::onMenuKeyPressed()
{
   MP_LOG << LOG_ENDL;
   //popScreen();  // deleted by lssanh 2013.04.15 NoCR slide menu off
   emit closeOptions(true); // modified by Dmitry 26.04.13
}

// { modified by Sergey 08.08.2013 for Voice Lang in USB/JB.
void AppMediaPlayer_Video_ScreenOptions::onItemPressed(int item)
{
    MP_LOG << "item =" << item << LOG_ENDL;

    //{ added by eunhye 2013.04.18 ITS 162583
    if ( item == OPTIONS_VIDEO_SWITCH_VOICE2 )
        return;
    //} added by eunhye 2013.04.18

    switch (item)
    {
        case OPTIONS_VIDEO_SWITCH_CAPTION:
        case OPTIONS_VIDEO_SWITCH_VOICE:
        case OPTIONS_VIDEO_SWITCH_ANGLE:
        case OPTIONS_RANDOM:
        case OPTIONS_REPEAT:
        case OPTIONS_SETTINGS: //added by edo.lee 2013.08.22 keep option by showing subscreen
        case OPTIONS_DIVX_SETTINGS://added by edo.lee 2013.08.22
        case OPTIONS_CAPTIONS://added by edo.lee 2013.08.22
        {
            if(m_pEngineVideo->getScreenManager(m_disp)->previousScreen() != NULL) // added by Sergey 21.11.2013 for ITS#207978
            {
                m_pEngineVideo->getScreenManager(m_disp)->previousScreen()->onOptionsItemClicked(item); // modified by Sergey 06.08.2013 to close menu in good time
                // { removed by taihyun.ahn 2013.12.24 for ITS 217053
                /*if(item == OPTIONS_DIVX_SETTINGS || item == OPTIONS_CAPTIONS)
                {
                    m_pEngineVideo->GetVideoController()->clearDisplay(true);//modified by taihyun.ahn 2013-12-21 
                }*/
                // } removed by taihyun 2013.12.24
            }
            break;
        }

        case OPTIONS_SOUND_SET: // added by Dmitry 21.07.13
        case OPTIONS_LIST:
        case OPTIONS_DISPLAY_SET: // added by Sergey 06.08.2013 for ITS#175510
        {
            if(m_pEngineVideo->getScreenManager(m_disp)->previousScreen() != NULL) // added by Sergey 21.11.2013 for ITS#207978
                m_pEngineVideo->getScreenManager(m_disp)->previousScreen()->onOptionsItemClicked(item);

            m_bCloseInBG = true; // modified by Sergey 06.08.2013 to close menu in good time
            break;
        }

        case OPTIONS_REPEAT_ALL:
        case OPTIONS_REPEAT_ONE:
        case OPTIONS_REPEAT_OFF:
        case OPTIONS_REPEAT_FOLDER:
        case OPTIONS_RANDOM_ON:
        case OPTIONS_RANDOM_OFF:
        {
            if(m_pEngineVideo->getScreenManager(m_disp)->previousScreen() != NULL) // added by Sergey 21.11.2013 for ITS#207978
                m_pEngineVideo->getScreenManager(m_disp)->previousScreen()->onOptionsItemClicked(item);

            emit closeOptions(false); //restored for ITS195746 29.Oct.2013
            break;
        }

        default:
        {
            // { modified by Sergey 07.09.2013 for ITS#185543
            // Voice language switched for USB/JB case
            if(m_pModel->getOptionMenuModel() && //added by edo.lee 2013.11.21
               m_pModel->getOptionMenuModel()->getCurrentMenuHeader() == QT_TR_NOOP("STR_MEDIA_VOICE_SETTING") &&
               item >= OPTIONS_MENU_MAX )
            {
                AppMediaPlayer_Video_ControllerFS* pFSCtrl = dynamic_cast<AppMediaPlayer_Video_ControllerFS*>(m_pController->getCurrentController());

                if(pFSCtrl)
                    pFSCtrl->setAudioLanguage(item - OPTIONS_MENU_MAX - 2); // 0 element is not used, 1st element is Header in OptionsMenu

                emit closeOptions(false);
                break;
            }
            // } modified by Sergey 07.09.2013 for ITS#185543

            emit closeOptions(false);

            // Cases when first must close OptionsMenu then handle item selection.
            if( m_pEngineVideo->getScreenManager(m_disp)->topScreen()) //added by edo.lee 2013.04.06
            {
                m_pEngineVideo->getScreenManager(m_disp)->topScreen()->onOptionsItemClicked(item);
            }
            break;
        }
    }
}
// } modified by Sergey 08.08.2013 for Voice Lang in USB/JB.


void AppMediaPlayer_Video_ScreenOptions::onCheckBoxSelect ( int item, bool flag )
{
   MP_LOG << "item =" << item << " flag =" << flag << LOG_ENDL;
   AppMediaPlayer_Video_Screen * previousScreen = m_pEngineVideo->getScreenManager(m_disp)->previousScreen();
   if ( previousScreen &&
        (m_isLockoutView == false || item != OPTIONS_PBC_ONOFF)) // added by cychoi 2014.02.20 for (HMC Request)
   {
      previousScreen->onOptionsCheckBoxSelect(item, flag);
   }
   closeOptions(false); // modified by Dmitry 26.04.13
   // { added by cychoi 2014.02.20 for (HMC Request)
   if( m_pEngineVideo->getScreenManager(m_disp)->topScreen() &&
       (m_isLockoutView == true && item == OPTIONS_PBC_ONOFF))
   {
       m_pEngineVideo->getScreenManager(m_disp)->topScreen()->onOptionsCheckBoxSelect(item, flag);
   }
   // } added by cychoi 2014.02.20
}

void  AppMediaPlayer_Video_ScreenOptions::onEventReceived(const QString &srcScreenName,
                                                          const QString &resScreenName,
                                                          const int eventId,
                                                          const QVariant arg)
{
    MP_LOG << " srcScreenName = " << srcScreenName << " resScreenName = " << resScreenName << \
             " eventId = " << eventId << " arg = " << arg << LOG_ENDL;

    Q_UNUSED(srcScreenName);

    if ( resScreenName == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS )
    {
        switch( eventId )
        {
            // { added by cychoi 2014.02.20 for (HMC Request)
            case EVENTS_UNDO_SELECT_PBC:
            {
                MP_MEDIUM << "EVENTS_UNDO_SELECT_PBC" << LOG_ENDL;
                if(m_pModel->getOptionMenuModel())
                {
                    VCD_DRS_MENU[3].bSelected = arg.toBool();
                    VCD_MENU[3].bSelected = arg.toBool();
                    m_pModel->getOptionMenuModel()->checkBoxSelected(2, arg.toBool());
                }
                break;
            }
            // } added by cychoi 2014.02.20
            case EVENTS_SELECT_PBC:
            {
                MP_MEDIUM << "EVENTS_ENABLE_PBC_MENU" << LOG_ENDL;
                // { modified by cychoi 2014.01.07 for ITS 207960 we find good solution // { modified by cychoi 2013.07.13 for List View spec out
                VCD_DRS_MENU[1].bEnabled = !arg.toBool();
                VCD_DRS_MENU[2].bEnabled = !arg.toBool();
                VCD_DRS_MENU[3].bSelected = arg.toBool(); // modified by cychoi 2013.07.17 for PBC On Off checkbox setting

                VCD_MENU[1].bEnabled = !arg.toBool();
                VCD_MENU[2].bEnabled = !arg.toBool();
                VCD_MENU[3].bSelected = arg.toBool(); // modified by cychoi 2013.07.17 for PBC On Off checkbox setting
                // } modified by cychoi 2014.01.07 // } modified by cychoi 2013.07.13
                // { added by cychoi 2014.01.23 for ITS 221992
                if(VCD_MENU[3].bSelected)
                {
                    VCD_MENU[5].bEnabled = false; // OPTIONS_DISPLAY_SET
                    VCD_MENU[6].bEnabled = false; // OPTIONS_SOUND_SET
                }
                else
                {
                    VCD_MENU[5].bEnabled = true; // OPTIONS_DISPLAY_SET
                    VCD_MENU[6].bEnabled = true; // OPTIONS_SOUND_SET
                }
                // } added by cychoi 2014.01.23
                // { modified by kihyung 2012.07.12
                // VCD_MENU[6].bEnabled = arg.toBool();
                // if(arg.toBool() == false) {
                    // VCD_MENU[6].bEnabled = arg.toBool();
                // }
                // } mdofied by kihyung

                if(m_pModel->getOptionMenuModel())
                {
                    m_pModel->getOptionMenuModel()->itemEnabled( OPTIONS_DIRECT_ACCESS, !arg.toBool() );
                    //m_pModel->getOptionMenuModel()->itemEnabled( OPTIONS_LIST , !arg.toBool() ); // commented by cychoi 2013.07.13 for List View spec out
                    // m_pModel->getOptionMenuModel()->itemEnabled( OPTIONS_RANDOM, !arg.toBool() ); // Deleted by yungi 2012.07.26 for CR12321 
                    m_pModel->getOptionMenuModel()->itemEnabled( OPTIONS_REPEAT, !arg.toBool() );
                    // { modified by kihyung 2012.07.12
                    // m_pModel->getOptionMenuModel()->itemEnabled( OPTIONS_PBC_MENU , arg.toBool() );
                    // if(arg.toBool() == false) {
                        // m_pModel->getOptionMenuModel()->itemEnabled( OPTIONS_PBC_MENU , arg.toBool() );
                    // }
                    // } modified by kihyung
                }

                break;
            }
// modified by ruindmby 2012.08.29 for CR 11030
            case EVENTS_PBC_MENU:
            {
                // { modified by cychoi 2014.01.07 for ITS 207960 we find good solution // { modified by cychoi 2013.07.13 for List View spec out
                VCD_DRS_MENU[4].bEnabled  = arg.toBool();  // Modified by yungi 2012.07.26 for CR12321

                VCD_MENU[4].bEnabled  = arg.toBool();
                // } modified by cychoi 2014.01.07 // } modified by cychoi 2013.07.13
			
                if(m_pModel->getOptionMenuModel()) 
                {
                    m_pModel->getOptionMenuModel()->itemEnabled( OPTIONS_PBC_MENU , arg.toBool() );
                }
                break;
            }
// modified by ruindmby 2012.08.29 for CR 11030
            default:
                break; // make compiler happy
        }
    }
}

void AppMediaPlayer_Video_ScreenOptions::setRepeatRandomeModel(int repeatMode, int randomMode)
{
    switch(randomMode)
    {
       case 0:   //random off
       {
           VIDEO_RANDOM_MENU[1].bSelected = false;      //on
           VIDEO_RANDOM_MENU[2].bSelected = true;       //off
           break;
       }

       case 1:   //folder
       case 2:   //all
       {
           VIDEO_RANDOM_MENU[1].bSelected = true;
           VIDEO_RANDOM_MENU[2].bSelected = false;
           break;
       }
       default:
           break;
    }

    switch(repeatMode)
    {
       // { modified by ravikanth - 12-09-19
       case 0:  //all
       {
          VIDEO_REPEAT_MENU[1].bSelected = true;   //all
          VIDEO_REPEAT_MENU[2].bSelected = false;  //folder
          VIDEO_REPEAT_MENU[3].bSelected = false;  //one
          break;
       }

       case 1:  //folder
       {
           VIDEO_REPEAT_MENU[1].bSelected = false;  //all
           VIDEO_REPEAT_MENU[2].bSelected = true;   //folder
           VIDEO_REPEAT_MENU[3].bSelected = false;  //one
           break;
       }

       case 2: //one
       {
           VIDEO_REPEAT_MENU[1].bSelected = false;  //all
           VIDEO_REPEAT_MENU[2].bSelected = false;  //folder
           VIDEO_REPEAT_MENU[3].bSelected = true;   //one
           break;
       }
       // { modified by ravikanth - 12-09-19

       default:
           break;
    }
}

// added by cychoi 2013.07.06 for VCD repeat menu
void AppMediaPlayer_Video_ScreenOptions::setVCDRepeatModel(int repeatMode)
{
    switch(repeatMode)
    {
        case 0:  //all
        {
            VIDEO_VCD_REPEAT_MENU[1].bSelected = true;   //all
            VIDEO_VCD_REPEAT_MENU[2].bSelected = false;  //one
            break;
        }

        case 2:  //one
        {
            VIDEO_VCD_REPEAT_MENU[1].bSelected = false;  //all
            VIDEO_VCD_REPEAT_MENU[2].bSelected = true;   //one
            break;
        }

        case 1: //folder
        default:
            break;
    }
}
// } added by cychoi 2013.07.06

void AppMediaPlayer_Video_ScreenOptions::closeOptionsIfAudioLangModel( )
{
   MP_LOG<<"topScreen:"<<m_pEngineVideo->getScreenManager(m_disp)->topScreenName() << LOG_ENDL;
   if ( m_pModel->getOptionMenuModel() &&
        m_pModel->getOptionMenuModel()->getCurrentMenuHeader() == QT_TR_NOOP("STR_MEDIA_VOICE_SETTING") &&
        m_pEngineVideo->getScreenManager(m_disp)->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS &&
        (fsData->displayPlaylistLength() > 1 && fsData->repeatMode() !=  AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_FILE)) // modified by kihyung 2013.09.11 for ITS 0189474
   {
      emit closeOptions(false); // modified by Dmitry 26.04.13
   }
}

// { modified by Sergey 08.08.2013 for Voice Lang in USB/JB.
void AppMediaPlayer_Video_ScreenOptions::setLanguageSettings(QStringList langCodeList, int nCurrStreamId)
{
    int optionMenuCount = langCodeList.length() + 2; // 0 element is not used, 1st element is Header

    if(m_pVideoLangugeSettingsMenu != NULL)
    {
        delete[] m_pVideoLangugeSettingsMenu;
    }

    m_pVideoLangugeSettingsMenu = new OPTION_MENU_T[ optionMenuCount ];
	
    // 0 element is not visible. It must be filled to use API getCurrentMenuHeader() in future.
    int index = 0;
    m_pVideoLangugeSettingsMenu[index].nId = OPTIONS_MENU_TITLE;
    m_pVideoLangugeSettingsMenu[index].sText = QT_TR_NOOP("STR_MEDIA_VOICE_SETTING");
    m_pVideoLangugeSettingsMenu[index].bCheckBox = false;
    m_pVideoLangugeSettingsMenu[index].bRadioBtn = false;
    m_pVideoLangugeSettingsMenu[index].bSelected = false;
    m_pVideoLangugeSettingsMenu[index].bSeparator = true;
    m_pVideoLangugeSettingsMenu[index].bEnabled = false;
    m_pVideoLangugeSettingsMenu[index].vAttachedMenu = NULL;
    m_pVideoLangugeSettingsMenu[index].nCountItemAttachedMenu = 0;

    // 1st element is Header. It is not selectable but just shows sub option title.
    index = 1;
    m_pVideoLangugeSettingsMenu[index].nId = OPTIONS_MENU_TITLE;
    m_pVideoLangugeSettingsMenu[index].sText = QT_TR_NOOP("STR_MEDIA_VOICE_SETTING");
    m_pVideoLangugeSettingsMenu[index].bCheckBox = false;
    m_pVideoLangugeSettingsMenu[index].bRadioBtn = false;
    m_pVideoLangugeSettingsMenu[index].bSelected = false;
    m_pVideoLangugeSettingsMenu[index].bSeparator = true;
    m_pVideoLangugeSettingsMenu[index].bEnabled = false;
    m_pVideoLangugeSettingsMenu[index].vAttachedMenu = NULL;
    m_pVideoLangugeSettingsMenu[index].nCountItemAttachedMenu = 0;

    for(int i = 2; i < optionMenuCount; i++) // 0 element is not used, 1st element is Header hence start from "i = 2"
    {
        m_pVideoLangugeSettingsMenu[i].nId = i + OPTIONS_MENU_MAX;

        // { modified by Sergey 22.08.2013 for ITS#184641
        int langIndex = i - 2; // to get 0
        QString langName = langCodeList.at(langIndex).toLocal8Bit().constData();

        if(langName.isEmpty())
            langName = "Audio " + QString::number(langIndex + 1);

        m_pVideoLangugeSettingsMenu[i].sText = langName;
		// } modified by Sergey 22.08.2013 for ITS#184641
        
        m_pVideoLangugeSettingsMenu[i].bCheckBox = false;
        m_pVideoLangugeSettingsMenu[i].bRadioBtn = true;
        m_pVideoLangugeSettingsMenu[i].bSelected = (nCurrStreamId == i-2)? true:false;
        m_pVideoLangugeSettingsMenu[i].bSeparator = true;
        m_pVideoLangugeSettingsMenu[i].bEnabled = true;
        m_pVideoLangugeSettingsMenu[i].vAttachedMenu = NULL;
        m_pVideoLangugeSettingsMenu[i].nCountItemAttachedMenu = 0;
    }

	if(m_isLockoutView)
	{
		USB_DRS_MENU[6].vAttachedMenu = m_pVideoLangugeSettingsMenu; // modified by Sergey 03.11.2013 for ITS#206033
		USB_DRS_MENU[6].nCountItemAttachedMenu = optionMenuCount; // modified by Sergey 03.11.2013 for ITS#206033

	    JUKEBOX_DRS_MENU[5].vAttachedMenu = m_pVideoLangugeSettingsMenu;
	    JUKEBOX_DRS_MENU[5].nCountItemAttachedMenu = optionMenuCount;
	}
	else
	{
		USB_MENU[6].vAttachedMenu = m_pVideoLangugeSettingsMenu; // modified by Sergey 03.11.2013 for ITS#206033
		USB_MENU[6].nCountItemAttachedMenu = optionMenuCount; // modified by Sergey 03.11.2013 for ITS#206033

	    JUKEBOX_MENU[5].vAttachedMenu = m_pVideoLangugeSettingsMenu;
	    JUKEBOX_MENU[5].nCountItemAttachedMenu = optionMenuCount;
	}
}
// } modified by Sergey 08.08.2013 for Voice Lang in USB/JB.


// modified by Dmitry 26.04.13
void AppMediaPlayer_Video_ScreenOptions::onBackKeyPressed()
{
   if(m_pModel->getOptionMenuModel())
   {
      MP_LOG << LOG_ENDL;
      emit backPressed();
   }
}
// modified by Dmitry 26.04.13

// { modified by ravikanth CR 15994 12-11-22
void AppMediaPlayer_Video_ScreenOptions::onActiveAudioLangChanged(QString text, QString text2)  // modified by dongjin 2013.02.27 for ISV 71500
{
    MP_MEDIUM << text << LOG_ENDL;
    VIDEO_VOICE_MENU[2].sText = text;
    VIDEO_VOICE_MENU[3].sText = text2; // added by dongjin 2013.02.27 for ISV 71500
    VIDEO_VOICE_MENU[3].bEnabled = false; // added by cychoi 2013.06.30 for New UX
	m_pModel->getOptionMenuModel()->itemTextChange(OPTIONS_VIDEO_SWITCH_VOICE,text); //added by lssanh 2012.12.12 for CR16580	
    m_pModel->getOptionMenuModel()->itemTextChange(OPTIONS_VIDEO_SWITCH_VOICE2,text2);  // added by dongjin 2013.02.27 for ISV 71500
}

void AppMediaPlayer_Video_ScreenOptions::onActiveCaptionChanged(QString text)
{
    MP_MEDIUM << text << LOG_ENDL;
    VIDEO_CAPTION_MENU[2].sText = text;
	m_pModel->getOptionMenuModel()->itemTextChange(OPTIONS_VIDEO_SWITCH_CAPTION,text); //added by lssanh 2012.12.12 for CR16580
}

void AppMediaPlayer_Video_ScreenOptions::onActiveAngleChanged(int text)
{
    MP_MEDIUM << text << LOG_ENDL;
    VIDEO_ANGLE_MENU[2].sText = QString::number(text);
	m_pModel->getOptionMenuModel()->itemTextChange(OPTIONS_VIDEO_SWITCH_ANGLE,QString::number(text)); //added by lssanh 2012.12.12 for CR16580	
}
// } modified by ravikanth CR 15994 12-11-22

// added by kihyung 2013.08.22 for ITS 0185761 
void AppMediaPlayer_Video_ScreenOptions::onSubtitleSettingChanged(int mediaSource)
{
    MEDIA_SOURCE_T source = (MEDIA_SOURCE_T)mediaSource;
    // removed by Sergey 09.10.2013 for ITS#192050
    
    if(source != m_pController->getSource()) return;

    if(m_pModel->getOptionMenuModel()) 
    {
        MP_LOG << mediaSource << m_pController->getCurrentController()->subTitle() << LOG_ENDL;
        m_pModel->getOptionMenuModel()->itemEnabled(OPTIONS_CAPTIONS, m_pController->getCurrentController()->subTitle());
    }
}


void AppMediaPlayer_Video_ScreenOptions::onVoiceLangEnabled(bool on)
{
    if(m_pModel->getOptionMenuModel())
    {
        m_pModel->getOptionMenuModel()->itemEnabled(OPTIONS_LANGUAGE, on);
    }
} // added by Sergey 09.10.2013 for ITS#192050


// { modified by Sergey 06.08.2013 to close menu in good time
void AppMediaPlayer_Video_ScreenOptions::onHidden()
{
    if(m_pEngineVideo->getScreenManager(m_disp)->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)  // modified by Sergey 08.08.2013 to fix double OptionsMenu
    {
        popScreen();
    }
}
// added by edo.lee 2013.11.20 ITS 0207978 
void AppMediaPlayer_Video_ScreenOptions::invokeSelectItemMethod(QObject* pObj, QString method, int arg )
{
    MP_LOG << LOG_ENDL;
	 QMetaObject::invokeMethod(pObj, method.toLocal8Bit(), Qt::QueuedConnection, Q_ARG(int, arg)); 
}

void AppMediaPlayer_Video_ScreenOptions::onLockoutMode(bool on)
{
   MP_LOG << "on =" << on << LOG_ENDL;

   //m_pModel->setLockoutMode(on);
   m_isLockout = on;
   emit showLockout(on);
   if(m_pEngineVideo->getEngineMain().isTempMode() == false)  // added by edo.lee 2013.10.07 ITS 194158
	   closeOptions(false); //modified by Sergey 02.10.2013 for ITS#192341
}


void AppMediaPlayer_Video_ScreenOptions::onFgBg(bool isFG)
{
    if(m_pEngineVideo->getScreenManager(m_disp)->topScreenName() != AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)
        return;

    if(!isFG && m_bCloseInBG)
    {
        emit closeOptions(false);
    }
}
// } modified by Sergey 06.08.2013 to close menu in good time

// } modified by Sergey 01.09.2013 for ITS#186753 (added QT_TR_NOOP only)


// removed by cychoi 2014.03.18 for ITS 229882 No option menu on Title/Disc Menu Screen //{ added by yongkyun.lee 2013-11-05 for ITS 206356

