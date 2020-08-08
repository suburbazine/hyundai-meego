#ifndef DHAVN_APPMEDIAPLAYER_VIDEOSETTINGS_H
#define DHAVN_APPMEDIAPLAYER_VIDEOSETTINGS_H


#include <QObject>
#include <QString>
#include <QSettings>
#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include <persistman.h>

// { modified by ravikanth 27-12-12

struct VIDEO_DB_SETTINGS_KEYS_T; 

class VideoSettingsDef : public QObject
{
   Q_OBJECT

   Q_ENUMS(VIDEO_DB_SETTINGS_KEY_T
           VIDEO_SETTINGS_DIVX_T)

public:

   enum VIDEO_DB_SETTINGS_KEY_T
   {
       DB_KEY_DIVX_REG_STATE,
       DB_KEY_VIDEO_JUKEBOX_BRIGHTNESS,                            /**for Brightness*/
       DB_KEY_VIDEO_USB_BRIGHTNESS,
       DB_KEY_VIDEO_USB2_BRIGHTNESS,
       DB_KEY_VIDEO_DVD_BRIGHTNESS,
       DB_KEY_VIDEO_VCD_BRIGHTNESS,
       DB_KEY_VIDEO_AUX_BRIGHTNESS,
       DB_KEY_VIDEO_JUKEBOX_SATURATION,                            /**for Saturation**/
       DB_KEY_VIDEO_USB_SATURATION,
       DB_KEY_VIDEO_USB2_SATURATION,
       DB_KEY_VIDEO_DVD_SATURATION,
       DB_KEY_VIDEO_VCD_SATURATION,
       DB_KEY_VIDEO_AUX_SATURATION,
       DB_KEY_VIDEO_JUKEBOX_HUE,                                   /**for Hue**/
       DB_KEY_VIDEO_USB_HUE,
       DB_KEY_VIDEO_USB2_HUE,
       DB_KEY_VIDEO_DVD_HUE,
       DB_KEY_VIDEO_VCD_HUE,
       DB_KEY_VIDEO_AUX_HUE,
       DB_KEY_VIDEO_JUKEBOX_CONTRAST,                              /**for Contrast**/
       DB_KEY_VIDEO_USB_CONTRAST,
       DB_KEY_VIDEO_USB2_CONTRAST,
       DB_KEY_VIDEO_DVD_CONTRAST,
       DB_KEY_VIDEO_VCD_CONTRAST,
       DB_KEY_VIDEO_AUX_CONTRAST,
       DB_KEY_LAST_ASPECT_RATIO, // modified by ravikanth 06-03-13
       DB_KEY_MAX
   };

   enum VIDEO_SETTINGS_DIVX_T
   {
     SETTINGS_DIVX_REG = 0,
     SETTINGS_DIVX_DEREG = 1
   };

};


struct  VIDEO_DB_SETTINGS_KEYS_T
{
   VideoSettingsDef::VIDEO_DB_SETTINGS_KEY_T keyId;
   const char *keyString;
};

// Table of representation variables
static const  VIDEO_DB_SETTINGS_KEYS_T  VIDEO_SETTINGS_DB_VARIABLES_KEYS[] =
{
      { VideoSettingsDef::DB_KEY_DIVX_REG_STATE,               "mDIVXRegStateKey"           },
      { VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_BRIGHTNESS,     "mVideoBrightness"           },
      { VideoSettingsDef::DB_KEY_VIDEO_USB_BRIGHTNESS,         "mUSBVideoBrightness"        },
      { VideoSettingsDef::DB_KEY_VIDEO_USB2_BRIGHTNESS,        "mUSB2VideoBrightness"       },
      { VideoSettingsDef::DB_KEY_VIDEO_DVD_BRIGHTNESS,         "mDVDVideoBrightness"        },
      { VideoSettingsDef::DB_KEY_VIDEO_VCD_BRIGHTNESS,         "mVCDVideoBrightness"        },
      { VideoSettingsDef::DB_KEY_VIDEO_AUX_BRIGHTNESS,         "mAUXVideoBrightness"        },
      { VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_SATURATION,     "mVideoSaturation"           },
      { VideoSettingsDef::DB_KEY_VIDEO_USB_SATURATION,         "mUSBVideoSaturation"        },
      { VideoSettingsDef::DB_KEY_VIDEO_USB2_SATURATION,        "mUSB2VideoSaturation"       },
      { VideoSettingsDef::DB_KEY_VIDEO_DVD_SATURATION,         "mDVDVideoSaturation"        },
      { VideoSettingsDef::DB_KEY_VIDEO_VCD_SATURATION,         "mVCDVideoSaturation"        },
      { VideoSettingsDef::DB_KEY_VIDEO_AUX_SATURATION,         "mAUXVideoSaturation"        },
      { VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_HUE,            "mVideoHue"                  },
      { VideoSettingsDef::DB_KEY_VIDEO_USB_HUE,                "mUSBVideoHue"               },
      { VideoSettingsDef::DB_KEY_VIDEO_USB2_HUE,               "mUSB2VideoHue"              },
      { VideoSettingsDef::DB_KEY_VIDEO_DVD_HUE,                "mDVDVideoHue"               },
      { VideoSettingsDef::DB_KEY_VIDEO_VCD_HUE,                "mVCDVideoHue"               },
      { VideoSettingsDef::DB_KEY_VIDEO_AUX_HUE,                "mAUXVideoHue"               },
      { VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_CONTRAST,       "mVideoContrast"             },
      { VideoSettingsDef::DB_KEY_VIDEO_USB_CONTRAST,           "mUSBVideoContrast"          },
      { VideoSettingsDef::DB_KEY_VIDEO_USB2_CONTRAST,          "mUSB2VideoContrast"         },
      { VideoSettingsDef::DB_KEY_VIDEO_DVD_CONTRAST,           "mDVDVideoContrast"          },
      { VideoSettingsDef::DB_KEY_VIDEO_VCD_CONTRAST,           "mVCDVideoContrast"          },
      { VideoSettingsDef::DB_KEY_VIDEO_AUX_CONTRAST,           "mAUXVideoContrast"          }, // modified by ravikanth 06-03-13
      { VideoSettingsDef::DB_KEY_LAST_ASPECT_RATIO,            "mPreviousAspectRatio"       } // modified by ravikanth 06-03-13
};

static const int VIDEO_SETTINGS_VARIABLES_DEFAULT_INT[][2]=
{
      { VideoSettingsDef::DB_KEY_DIVX_REG_STATE,             VideoSettingsDef::SETTINGS_DIVX_DEREG },
      { VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_BRIGHTNESS,   0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_USB_BRIGHTNESS,       0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_USB2_BRIGHTNESS,      0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_DVD_BRIGHTNESS,       0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_VCD_BRIGHTNESS,       0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_AUX_BRIGHTNESS,       0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_SATURATION,   0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_USB_SATURATION,       0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_USB2_SATURATION,      0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_DVD_SATURATION,       0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_VCD_SATURATION,       0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_AUX_SATURATION,       0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_HUE,          0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_USB_HUE,              0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_USB2_HUE,             0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_DVD_HUE,              0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_VCD_HUE,              0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_AUX_HUE,              0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_CONTRAST,     0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_USB_CONTRAST,         0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_USB2_CONTRAST,        0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_DVD_CONTRAST,         0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_VCD_CONTRAST,         0                                 },
      { VideoSettingsDef::DB_KEY_VIDEO_AUX_CONTRAST,         0                                 }, // modified by ravikanth 06-03-13
      { VideoSettingsDef::DB_KEY_LAST_ASPECT_RATIO,          0                                 } // modified by ravikanth 06-03-13
      };

class AppMediaPlayer_VideoSettings : public QObject
{
    Q_OBJECT


    AppMediaPlayer_VideoSettings( QObject* parent = NULL );

    ~ AppMediaPlayer_VideoSettings() {}


public:


   int LoadSetting( int key );
   void SaveSetting( int value, int key );
   bool OpenScreenSettingsDb();
   void CloseScreenSettingsDb();
   int  SetDefaultSetting( int key );

   static AppMediaPlayer_VideoSettings * GetInstance();
   static void FreeInstance();

   // loads after key normalization
   int LoadSetting( int key, int type );
   void SaveSetting( int value, int key, int type );

private:

   static AppMediaPlayer_VideoSettings *m_pInstance;
   static int m_count;
   PMHANDLE_T *m_pDBSession;
   int m_nDBState;
};

// } modified by ravikanth 27-12-12
#endif // DHAVN_APPMEDIAPLAYER_VIDEOSETTINGS_H
