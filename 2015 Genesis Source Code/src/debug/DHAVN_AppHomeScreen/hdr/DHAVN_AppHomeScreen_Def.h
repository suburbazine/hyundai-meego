#ifndef DHAVN_APPHOMESCREEN_GLOBAL_H
#define DHAVN_APPHOMESCREEN_GLOBAL_H

#include <QObject>

#define MAX_PATH_LENGHT 255
#define MEDIA_STRING_LENGHT 255

#define VIDEO_PALYER_USE_OLD_AV_INTERFACE


class HomeScreenDef: public QObject
{
   Q_OBJECT

   Q_ENUMS( MEDIA_TYPE_T );

public:
    /** avaliable MEDIA types */
    typedef enum
    {
        MEDIA_TYPE_INVALID = 0,
        MEDIA_TYPE_AUDIO,
        MEDIA_TYPE_VIDEO,
        MEDIA_TYPE_RADIO,
        MEDIA_TYPE_DMB,
        MEDIA_TYPE_XM,
        MEDIA_TYPE_DAB,
        MEDIA_TYPE_PANDORA,
        MEDIA_TYPE_AHA,
        MEDIA_TYPE_MAX
    }MEDIA_TYPE_T;

    typedef enum
    {
        SET_PLAYING_MEDIA,        /** call to change current playing file data - HOME_MEDIA_T*/
        UPDATE_PLAY_POSITION,     /** call to change current playing position*/
        UPDATE_BTSTATE_INDICATOR, /** call to update display BT state indicator */
        UPDATE_BT_MESSAGE_COUNT,  /** call to update message and call count*/
        UPDATE_CONTENT_STATUS,    /** call to notify about available content*/
        DEVICE_DISCONNECTED       /** call to notify about disconnected device*/
    }HOME_COMMANDS_T;


    /** avaliable MEDIA source targets */
    typedef enum
    {
        MEDIA_SOURCE_INVALID = 0,
        MEDIA_SOURCE_JBOX,
        MEDIA_SOURCE_DLNA,
        MEDIA_SOURCE_BLUETOOTH,
        MEDIA_SOURCE_DMB1,
        MEDIA_SOURCE_DMB2,
        MEDIA_SOURCE_FM1,
        MEDIA_SOURCE_FM2,
        MEDIA_SOURCE_AM,
        MEDIA_SOURCE_AUX,
        MEDIA_SOURCE_VCD,   // 10
        MEDIA_SOURCE_CD_AUDIO,
        MEDIA_SOURCE_CD_DATA,
        MEDIA_SOURCE_DAB1,
        MEDIA_SOURCE_DAB2,
        MEDIA_SOURCE_USB1,
        MEDIA_SOURCE_USB2,
        MEDIA_SOURCE_IPOD1,
        MEDIA_SOURCE_IPOD2,
        MEDIA_SOURCE_DVD_AUDIO,
        MEDIA_SOURCE_DVD_VIDEO,  // 20

        MEDIA_SOURCE_MAX
    }MEDIA_SOURCE_T;

   typedef enum
   {
      PRESSED_KEY_NONE,
      PRESSED_KEY_SEEK,
      PRESSED_KEY_TUNE,
      PRESSED_KEY_TUNE_DIAL
   } PRESSED_KEY_T;

   typedef enum
   {
      HOME_BT_INDICATOR_ICON_NONE,
      HOME_BT_INDICATOR_ICON_INCOMMING_CALL = 1,
      HOME_BT_INDICATOR_ICON_OUTGOING_CALL,
      HOME_BT_INDICATOR_ICON_CONNECTED,
      HOME_BT_INDICATOR_ICON_ENDING,
      HOME_BT_INDICATOR_ICON_REJECT_CALL,
      HOME_BT_INDICATOR_ICON_SWITCH_TO_CP,
      HOME_BT_INDICATOR_ICON_MIC_VOL_0,
      HOME_BT_INDICATOR_ICON_3_WAY_CONNECTING,
      HOME_BT_INDICATOR_ICON_3_WAY_CONNECTED,
      HOME_BT_INDICATOR_ICON_3_WAY_CALL_ENDING,
      HOME_BT_INDICATOR_ICON_PHONE_CONNECTED,
      HOME_BT_INDICATOR_ICON_NO_REGISTERED_PHONE,
      HOME_BT_INDICATOR_ICON_NO_CONNECTED_PHONE,
      HOME_BT_INDICATOR_ICON_CONNETING,
      HOME_BT_INDICATOR_ICON_DISCONNECTING,
      HOME_BT_INDICATOR_ICON_INITIALIZING,
      HOME_BT_INDICATOR_ICON_DOWNLOADING_PHONEBOOK,
      HOME_BT_INDICATOR_ICON_INCOMING_SMS,
      HOME_BT_INDICATOR_ICON_LISTENING
   }HOME_BT_INDICATOR_ICON_T;
};

typedef struct
{
   wchar_t sFrequency[MEDIA_STRING_LENGHT];
   wchar_t sChannel[MEDIA_STRING_LENGHT];
   bool bScanning; /** Will be removed */
   bool bHD;  /** Will be removed */
   wchar_t sIndicatorText[MEDIA_STRING_LENGHT];  /** Will be removed */
   HomeScreenDef::MEDIA_SOURCE_T nSource;  /** Will be removed */
   HomeScreenDef::PRESSED_KEY_T nPressedKey;  /** Will be removed */
   bool bShowOsd; /** Will be removed */
}HOME_MEDIA_RADIO_T;

typedef struct
{
   wchar_t sCoverPath[MAX_PATH_LENGHT];  /** Will be removed */
   wchar_t sChannel[MEDIA_STRING_LENGHT];
   wchar_t sSong[MEDIA_STRING_LENGHT];
   bool bScanning;  /** Will be removed */
   bool bHD;  /** Will be removed */
   wchar_t sIndicatorText[MEDIA_STRING_LENGHT];  /** Will be removed */
   HomeScreenDef::PRESSED_KEY_T nPressedKey;  /** Will be removed */
   bool bShowOsd;  /** Will be removed */
}HOME_MEDIA_RADIO_XM_T;

typedef struct
{
   wchar_t sServiceName[MEDIA_STRING_LENGHT];
   wchar_t sEnsembleName[MEDIA_STRING_LENGHT];
   bool bScanning;  /** Will be removed */
   bool bHD;  /** Will be removed */
   wchar_t sIndicatorText[MEDIA_STRING_LENGHT];  /** Will be removed */
   HomeScreenDef::PRESSED_KEY_T nPressedKey;  /** Will be removed */
   bool bShowOsd;  /** Will be removed */
   HomeScreenDef::MEDIA_SOURCE_T nSource;
}HOME_MEDIA_RADIO_DAB_T;

typedef struct
{
   wchar_t sCoverPath[ MAX_PATH_LENGHT ];  /** Will be removed */
   wchar_t sChannel[MEDIA_STRING_LENGHT];
   bool bScanning;  /** Will be removed */
   wchar_t sIndicatorText[MEDIA_STRING_LENGHT];  /** Will be removed */
   bool bShowOsd;  /** Will be removed */
   HomeScreenDef::MEDIA_SOURCE_T nSource;
}HOME_MEDIA_DMB_T;

/** this struct contains description of played MEDIA items */
typedef struct
{
   HomeScreenDef::MEDIA_TYPE_T nType;
   HomeScreenDef::MEDIA_SOURCE_T nSource;
   wchar_t sMedia1[ MEDIA_STRING_LENGHT ];
   wchar_t sMedia2[ MEDIA_STRING_LENGHT ];

   union MediaData_U
   {
      HOME_MEDIA_RADIO_T sRadio;
      HOME_MEDIA_RADIO_XM_T sRadioXM;
      HOME_MEDIA_RADIO_DAB_T sRadioDAB;
      HOME_MEDIA_DMB_T sDMB;
   }uData;
}HOME_MEDIA_T;

/** Media content status */
typedef struct
{
    HomeScreenDef::MEDIA_SOURCE_T device;
    bool bAudio;
    bool bVideo;
    bool bPhoto;

} HOME_MEDIA_CONTENT_T;

typedef struct
{
   HomeScreenDef::HOME_BT_INDICATOR_ICON_T eIcon;
   wchar_t sText[MEDIA_STRING_LENGHT];

}HOME_BT_INDICATOR_STATE_DATA_T;

typedef struct
{
   int nMessageCount;
   int nCallsCount;

}HOME_BT_MESSAGE_COUNT_DATA_T;

#endif // DHAVN_APPHOMESCREEN_GLOBAL_H
