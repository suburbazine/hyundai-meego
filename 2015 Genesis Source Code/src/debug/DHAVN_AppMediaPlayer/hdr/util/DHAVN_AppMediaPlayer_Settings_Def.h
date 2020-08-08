
#ifndef APP_MEDIA_PLAYER_SETTINGS_DEF
#define APP_MEDIA_PLAYER_SETTINGS_DEF

#include <QObject>
#include <QString>
#include <QSettings>

struct SETTINGS_KEYS_T;
class AppMediaPlayer_EngineMain;

// { modified by kihyung 2013.07.20 for ITS 0179774
class SettingsDef : public QObject
{
   Q_OBJECT

   Q_ENUMS( QSETTINGS_KEY_T MediaPlayerSettingId )

public:

   enum QSETTINGS_KEY_T
   {
      // Music Player
      KEY_AUDIO_MEDIA_DEVICE_ID,
      KEY_AUDIO_MEDIA_MOUNT_PATH,
      KEY_AUDIO_TRACK_INDEX,
      KEY_AUDIO_TRACK_POSITION,
      KEY_AUDIO_CURRENT_PLAYLIST,
      KEY_AUDIO_DISPLAY_TIP,
      KEY_AUDIO_REPEAT_PLAYMODE,
      KEY_AUDIO_RANDOM_PLAYMODE,
      KEY_AUDIO_VIDEO_ACTIVE_MODE,
      KEY_AUDIO_VIDEO_LAST_ENTRY,
      KEY_AUDIO_TRACK_TITLE,
      KEY_AUDIO_TRACK_ARTIST,
      KEY_AUDIO_TRACK_ALBUM,
      KEY_AUDIO_LIST_TABID,
      KEY_AUDIO_LIST_QUERY,
      KEY_AUDIO_LIST_ITEM_INDEX,
      KEY_AUDIO_LIST_DEPTH,
       KEY_AUDIO_LIST_CATEGORY, //added by junam 2013.07.04 for ITS172937

      // Video Player
      KEY_VIDEO_ACTIVE_SOURCE,
      KEY_VIDEO_USB_DEVICE_ID,	
      KEY_VIDEO_USB_MOUNT_PATH,      
      KEY_VIDEO_USB_FOLDER,
      KEY_VIDEO_USB_FULLFOLDER,
      KEY_VIDEO_USB_FILE,
      KEY_VIDEO_USB_POSITION,
      KEY_VIDEO_USB_INDEX,
      KEY_VIDEO_USB_PAUSED,
      KEY_VIDEO_USB_RANDOM,
      KEY_VIDEO_USB_REPEAT,
      KEY_VIDEO_USB_AUDIO_LANG, // added by Sergey 07.09.2013 for ITS#185543
      KEY_VIDEO_USB2_DEVICE_ID,
      KEY_VIDEO_USB2_MOUNT_PATH,      
      KEY_VIDEO_USB2_FOLDER,
      KEY_VIDEO_USB2_FULLFOLDER,
      KEY_VIDEO_USB2_FILE,
      KEY_VIDEO_USB2_POSITION,
      KEY_VIDEO_USB2_INDEX,
      KEY_VIDEO_USB2_PAUSED,
      KEY_VIDEO_USB2_RANDOM,
      KEY_VIDEO_USB2_REPEAT,
      KEY_VIDEO_USB2_AUDIO_LANG, // added by Sergey 07.09.2013 for ITS#185543
      KEY_VIDEO_JUKEBOX_FOLDER,
      KEY_VIDEO_JUKEBOX_FULLFOLDER,
      KEY_VIDEO_JUKEBOX_FILE,
      KEY_VIDEO_JUKEBOX_POSITION,
      KEY_VIDEO_JUKEBOX_INDEX,
      KEY_VIDEO_JUKEBOX_PAUSED,
      KEY_VIDEO_JUKEBOX_RANDOM,
      KEY_VIDEO_JUKEBOX_REPEAT,
      KEY_VIDEO_JUKEBOX_AUDIO_LANG, // added by Sergey 07.09.2013 for ITS#185543
      KEY_VIDEO_VCD_PAUSED,
      KEY_VIDEO_VCD_TRACK,
      KEY_VIDEO_VCD_REPEAT,
      KEY_VIDEO_VCD_RANDOM,
      KEY_VIDEO_DIVX_SCREEN_MODE, // added by Sergey 25.10.2013 for new DivX
      
      // Photo Player
      KEY_PHOTO_JUKEBOX_FILE_FRONT,
      KEY_PHOTO_USB1_FILE_FRONT,
      KEY_PHOTO_USB2_FILE_FRONT,
      KEY_PHOTO_SLIDESHOW_DURATION_FRONT,
      KEY_PHOTO_JUKEBOX_FILE_REAR,
      KEY_PHOTO_USB1_FILE_REAR,
      KEY_PHOTO_USB2_FILE_REAR,
      KEY_PHOTO_SLIDESHOW_DURATION_REAR,
      
      // ETC
      KEY_VIDEO_USB_SUBTITLE,
      KEY_VIDEO_USB2_SUBTITLE,
      KEY_VIDEO_JUKEBOX_SUBTITLE,
      KEY_VIDEO_USB_SUBTITLE_SIZE,
      KEY_VIDEO_USB2_SUBTITLE_SIZE,
      KEY_VIDEO_JUKEBOX_SUBTITLE_SIZE,
      KEY_VIDEO_USB_SUBTITLE_CURRENT, 
      KEY_VIDEO_USB2_SUBTITLE_CURRENT,
      KEY_VIDEO_JUKEBOX_SUBTITLE_CURRENT, // added by Sergey 08.01.2014 for ITS#211412
      KEY_AUDIO_PLAYED_CONTENTS,

      // Common
      KEY_AUDIO_JUKEBOX_INDEXING_END,
      KEY_AUDIO_JUKEBOX_SCANNING_END, // added by kihyung 2013.07.25 for ITS 0180896 
      KEY_USB1_LAST_UUID,
      KEY_USB1_LAST_AV_MODE,
      KEY_USB2_LAST_UUID,
      KEY_USB2_LAST_AV_MODE,

      KEY_AUDIO_COUNTRY_VARIANT,
      KEY_VIDEO_COUNTRY_VARIANT,
      KEY_PHOTO_COUNTRY_VARIANT
   };
};

struct QSETTINGS_KEYS_T
{
   SettingsDef::QSETTINGS_KEY_T keyId;
   const char *keyString;
};


static const QSETTINGS_KEYS_T QSETTINGS_KEYS[] =
{
   // Audio
   { SettingsDef::KEY_AUDIO_MEDIA_DEVICE_ID,          "audio_media_device_id"      },
   { SettingsDef::KEY_AUDIO_MEDIA_MOUNT_PATH,         "audio_media_mount_path"     },    
   { SettingsDef::KEY_AUDIO_TRACK_INDEX,              "audio_track_index"          },
   { SettingsDef::KEY_AUDIO_TRACK_POSITION,           "audio_track_position"       },
   { SettingsDef::KEY_AUDIO_CURRENT_PLAYLIST,         "audio_current_playlist"     },
   { SettingsDef::KEY_AUDIO_DISPLAY_TIP,              "audio_display_tip"          },
   { SettingsDef::KEY_AUDIO_REPEAT_PLAYMODE,          "audio_repeat_playmode"      },
   { SettingsDef::KEY_AUDIO_RANDOM_PLAYMODE,          "audio_random_playmode"      },
   { SettingsDef::KEY_AUDIO_VIDEO_ACTIVE_MODE,        "audio_video_active_mode"    },
   { SettingsDef::KEY_AUDIO_VIDEO_LAST_ENTRY,         "audio_video_last_entry"     },
   { SettingsDef::KEY_AUDIO_TRACK_TITLE,              "audio_track_title"          },
   { SettingsDef::KEY_AUDIO_TRACK_ARTIST,             "audio_track_artist"         },
   { SettingsDef::KEY_AUDIO_TRACK_ALBUM,              "audio_track_album"          },

   { SettingsDef::KEY_AUDIO_LIST_TABID,               "audio_list_tabid"           },   
   { SettingsDef::KEY_AUDIO_LIST_QUERY,               "audio_list_query"           },   
   { SettingsDef::KEY_AUDIO_LIST_ITEM_INDEX,          "audio_list_item_index"      },       
   { SettingsDef::KEY_AUDIO_LIST_DEPTH,               "audio_list_depth"           },
   { SettingsDef::KEY_AUDIO_LIST_CATEGORY,            "audio_list_category"        },        //added by junam 2013.07.04 for ITS172937
      

   // Video
   { SettingsDef::KEY_VIDEO_ACTIVE_SOURCE,            "video_active_source"        },
   { SettingsDef::KEY_VIDEO_USB_DEVICE_ID,            "video_usb_device_id"        }, // modified by kihyung 2013.08.06
   { SettingsDef::KEY_VIDEO_USB_MOUNT_PATH,           "video_usb_mount_path"       }, // modified by kihyung 2013.08.06   
   { SettingsDef::KEY_VIDEO_USB_FOLDER,               "video_usb_folder"           },
   { SettingsDef::KEY_VIDEO_USB_FULLFOLDER,           "video_usb_fullfolder"       },
   { SettingsDef::KEY_VIDEO_USB_FILE,                 "video_usb_file"             },
   { SettingsDef::KEY_VIDEO_USB_POSITION,             "video_usb_position"         },
   { SettingsDef::KEY_VIDEO_USB_INDEX,                "video_usb_index"            },
   { SettingsDef::KEY_VIDEO_USB_PAUSED,               "video_usb_paused"           },
   { SettingsDef::KEY_VIDEO_USB_RANDOM,               "video_usb_random"           },
   { SettingsDef::KEY_VIDEO_USB_REPEAT,               "video_usb_repeat"           },
   { SettingsDef::KEY_VIDEO_USB_AUDIO_LANG,           "video_usb_audio_lang"       }, // added by Sergey 07.09.2013 for ITS#185543
   { SettingsDef::KEY_VIDEO_USB2_DEVICE_ID,           "video_usb2_device_id"       }, // modified by kihyung 2013.08.06
   { SettingsDef::KEY_VIDEO_USB2_MOUNT_PATH,          "video_usb2_mount_path"      }, // modified by kihyung 2013.08.06   
   { SettingsDef::KEY_VIDEO_USB2_FOLDER,              "video_usb2_folder"          },
   { SettingsDef::KEY_VIDEO_USB2_FULLFOLDER,          "video_usb2_fullfolder"      },
   { SettingsDef::KEY_VIDEO_USB2_FILE,                "video_usb2_file"            },
   { SettingsDef::KEY_VIDEO_USB2_POSITION,            "video_usb2_position"        },
   { SettingsDef::KEY_VIDEO_USB2_INDEX,               "video_usb2_index"           },
   { SettingsDef::KEY_VIDEO_USB2_PAUSED,              "video_usb2_paused"          },
   { SettingsDef::KEY_VIDEO_USB2_RANDOM,              "video_usb2_random"          },
   { SettingsDef::KEY_VIDEO_USB2_REPEAT,              "video_usb2_repeat"          },
   { SettingsDef::KEY_VIDEO_USB2_AUDIO_LANG,          "video_usb2_audio_lang"      }, // added by Sergey 07.09.2013 for ITS#185543
   { SettingsDef::KEY_VIDEO_JUKEBOX_FOLDER,           "video_jukebox_folder"       },
   { SettingsDef::KEY_VIDEO_JUKEBOX_FULLFOLDER,       "video_jukebox_fullfolder"   },
   { SettingsDef::KEY_VIDEO_JUKEBOX_FILE,             "video_jukebox_file"         },
   { SettingsDef::KEY_VIDEO_JUKEBOX_POSITION,         "video_jukebox_position"     },
   { SettingsDef::KEY_VIDEO_JUKEBOX_INDEX,            "video_jukebox_index"        },
   { SettingsDef::KEY_VIDEO_JUKEBOX_PAUSED,           "video_jukebox_paused"       },
   { SettingsDef::KEY_VIDEO_JUKEBOX_RANDOM,           "video_jukebox_random"       },
   { SettingsDef::KEY_VIDEO_JUKEBOX_REPEAT,           "video_jukebox_repeat"       },
   { SettingsDef::KEY_VIDEO_JUKEBOX_AUDIO_LANG,       "video_jukebox_audio_lang"   }, // added by Sergey 07.09.2013 for ITS#185543
   { SettingsDef::KEY_VIDEO_VCD_PAUSED,               "video_vcd_paused"           },
   { SettingsDef::KEY_VIDEO_VCD_TRACK,                "video_vcd_track"            },
   { SettingsDef::KEY_VIDEO_VCD_REPEAT,               "video_vcd_repeat"           },
   { SettingsDef::KEY_VIDEO_VCD_RANDOM,               "video_vcd_random"           },
   { SettingsDef::KEY_VIDEO_DIVX_SCREEN_MODE,         "video_divx_screen_mode"     }, // added by Sergey 25.10.2013 for new DivX
   
   // Photo
   { SettingsDef::KEY_PHOTO_JUKEBOX_FILE_FRONT,       "photo_jukebox_file_front"   },
   { SettingsDef::KEY_PHOTO_USB1_FILE_FRONT,          "photo_usb1_file_front"      },
   { SettingsDef::KEY_PHOTO_USB2_FILE_FRONT,          "photo_usb2_file_front"      },
   { SettingsDef::KEY_PHOTO_SLIDESHOW_DURATION_FRONT, "slideshow_duration_front"   },
   { SettingsDef::KEY_PHOTO_JUKEBOX_FILE_REAR,        "photo_jukebox_file_rear"    },
   { SettingsDef::KEY_PHOTO_USB1_FILE_REAR,           "photo_usb1_file_rear"       },
   { SettingsDef::KEY_PHOTO_USB2_FILE_REAR,           "photo_usb2_file_rear"       },
   { SettingsDef::KEY_PHOTO_SLIDESHOW_DURATION_REAR,  "slideshow_duration_rear"    },
    
   // ETC
   { SettingsDef::KEY_VIDEO_USB_SUBTITLE,             "video_usb_subtitle"         },
   { SettingsDef::KEY_VIDEO_USB2_SUBTITLE,            "video_usb2_subtitle"        },
   { SettingsDef::KEY_VIDEO_JUKEBOX_SUBTITLE,         "video_jukebox_subtitle"     },
   { SettingsDef::KEY_VIDEO_USB_SUBTITLE_SIZE,        "video_usb_subtitle_size"    },
   { SettingsDef::KEY_VIDEO_USB2_SUBTITLE_SIZE,       "video_usb2_subtitle_size"   },
   { SettingsDef::KEY_VIDEO_JUKEBOX_SUBTITLE_SIZE,    "video_jukebox_subtitle_size"},
   { SettingsDef::KEY_VIDEO_USB_SUBTITLE_CURRENT,     "video_usb_subtitle_current" },
   { SettingsDef::KEY_VIDEO_USB2_SUBTITLE_CURRENT,    "video_usb2_subtitle_current"},
   { SettingsDef::KEY_VIDEO_JUKEBOX_SUBTITLE_CURRENT, "video_jukebox_subtitle_current" }, // added by Sergey 08.01.2014 for ITS#211412
   { SettingsDef::KEY_AUDIO_PLAYED_CONTENTS,   	      "audio_played_contents"      },

   // Common
   { SettingsDef::KEY_AUDIO_JUKEBOX_INDEXING_END,     "audio_jukebox_indexing_end" },
   { SettingsDef::KEY_AUDIO_JUKEBOX_SCANNING_END,     "audio_jukebox_scanning_end" }, // added by kihyung 2013.07.25 for ITS 0180896 
   { SettingsDef::KEY_USB1_LAST_UUID,                 "usb1_last_uuid"             },
   { SettingsDef::KEY_USB1_LAST_AV_MODE,              "usb1_last_av_mode"          },
   { SettingsDef::KEY_USB2_LAST_UUID,                 "usb2_last_uuid"             },
   { SettingsDef::KEY_USB2_LAST_AV_MODE,              "usb2_last_av_mode"          },

   { SettingsDef::KEY_AUDIO_COUNTRY_VARIANT,          "audio_country_variant"      },    
   { SettingsDef::KEY_VIDEO_COUNTRY_VARIANT,          "video_country_variant"      },    
   { SettingsDef::KEY_PHOTO_COUNTRY_VARIANT,          "photo_country_variant"      }    
};
// } modified by kihyung 2013.07.20 for ITS 0179774

#endif // SETTINGS_APP_STORAGE
