/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                  Common and global variables defined here      *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef COMMONVARS_H
#define COMMONVARS_H
#include <qstring.h>

#ifndef TAN_DEBUG
    #define TAN_PRINTF(x,...)
#else
    #define TAN_PRINTF printf
#endif

#define TEMP_ARTWORK_LOCATION "/app/data/AppMedia/tmp/iPodData/"
#define PERM_DBPATH           "/app/data/AppMedia/share/iPodDBPath/"
#define TEMP_DBPATH           "/dev/shm/"
#define SAMPLE_RATE_FILEPATH_1 "/app/data/AppMedia/share/sampleRate_1.txt"
#define SAMPLE_RATE_FILEPATH_2 "/app/data/AppMedia/share/sampleRate_2.txt"

#define NO_OF_META_INFO_UID_Track_Info 4
#define NO_OF_META_INFO_PB_Track_Info 8

//Do not change this flag.
//AVP hard coded this.
enum PrevNext_T
{
    OSDKey_None = -1,
    OSDKey_Next,
    OSDKey_Prev,
    OSDKey_Scan,
    OSDKey_SetCurrentIndex
};

static const int BYTESTOBEREAD = 2048;
static const int CONFIGURATION_APPLE = 2;
static const int VENDOR_ID = 0x05ac;
const QString gConfSetter("ConfSetter");
const QString gRawPcktHandler("RawPcktHandler");
const QString gDetector("Detector");
const QString gHidDetector("HidDetector");
const QString gReader("Reader");
const QString gWriter("Writer");
const QString gCoprocessorDataProvider("Coprocessor");
const QString gEnterUiModeTaskHandler ("EnterUiMode");
const QString gAuthorisor ("AuthorisorTaskHandler");
const QString gIdentifier ("IdentifierTaskHandler");
const QString giTunesTaskHandler ("iTunesDBHandler");
const QString gPlayEngineTaskHandler ("iPlayEngineTaskHandler");
const QString gArtworkFetchTaskHandler ("iArtworkFetchTaskHandler");
const QString gAudioSourceDetector ("iAudioSourceDetector");
const QString giTunesTagTaskHandler("iTunesTagTaskHandler");
const QString gControllerPrivateData("ControllerprivateData");
const QString gPandoraTaskHandler ("iPandoraTaskHandler");
const QString giPodControllerLogName_1("Media_IPodController:1");
const QString giPodControllerLogName_2("Media_IPodController:2");
const QString giPodControllerLogName("Media_IPodController");
const QString gControllerLogName("Media_IPodController");

//coprocessor data
const QString gIPodTempPath ("/app/data/AppMedia/tmp/tempIPodData/");
const QString gIPodHidDevicePath ("/app/data/AppMedia/tmp/tempIPodData/ipodhiddev.txt");
const QString gIPodHidDevicePathPort1 ("/app/data/AppMedia/tmp/tempIPodData/ipodhiddevpath1.txt");
const QString gIPodHidDevicePathPort2 ("/app/data/AppMedia/tmp/tempIPodData/ipodhiddevpath2.txt");
const QString gIPodPandoraAppSessionID ("/app/data/AppMedia/tmp/tempIPodData/com.pandora.link.v1");
const QString gIPodPandoraAppID ("com.pandora.link.v1");
const int gPandoraProtocolIndex = 0x01;
const QString gIPodAhaAppSessionID ("/app/data/AppMedia/tmp/tempIPodData/com.ahamobile.link.v1");
const QString gIPodAhaAppID ("com.ahamobile.link.v1");
const int gAhaProtocolIndex = 0x02;

//coprocessor data
#define I2C_DEV_FILE_NAME	"/dev/i2c-0"
#define I2C_CP_SLAVE_ADDRESS 	0X11
#define DEV_VERSION_REG_ADDR	0X0000
#define ACCES_CERTIFICATE_START_PAGE_SIZE	128
#define ACCES_CERTIFICATE_DATA_LEN_REG_ADDR	0X0030
#define ACCES_CERTIFICATE_DATA_LEN_REG_SIZE	2
#define ACCES_CERTIFICATE_NUM_PAGES		15
#define ACCES_CERTIFICATE_START_PAGE_REG_ADDR	0X0031
#define USLEEP_TIME		(10000)
#define CHALLENGE_DATA_LEN_REG_ADDR	0X0020
#define CHALLENGE_DATA_LEN_SIZE		2
#define CHALLENGE_DATA_REG_ADDR		0X0021
#define CHALLENGE_DATA_SIZE_MAX		20
#define AUTH_CONTROL_STATUS_REG_ADDR	0X0010
#define AUTH_CONTROL_STATUS_SIZE	1
#define ERROR_CODE_REG_ADDR	0X0005
#define ERROR_CODE_REG_SIZE	1
#define SIG_DATA_LEN_REG_ADDR           0X0011
#define SIG_DATA_LEN_REG_SIZE           2
#define SIG_DATA_REG_ADDR               0X0012
#define SIG_DATA_REG_SIZE               128

#define SYNC                1
#define RESYNC              2
#define IPOD_TOUCH_1GEN_PROD_ID 0x1291
#define IPOD_APPLICATION_BUNDLESEED_ID "com.apple"
#define IPOD_APP_ID "com.apple"
#define IPOD_MUSICAPP_APP_ID "com.apple.Music"
#define IPOD_MOBILEAPP_APP_ID "com.apple.mobileipod"
#define PANDORA_BUNDLESEED_NAME "com.pandora"
#define IPOD_OTHERAPP_FILEDIFFERENCIATOR "OTHERAPP"

#define NANO_1G_PRODUCT_ID 0x120A
#define NANO_2G_PRODUCT_ID 0x1260
#define NANO_3G_PRODUCT_ID 0x1262
#define NANO_4G_PRODUCT_ID 0x1263
#define NANO_5G_PRODUCT_ID 0x1265
#define NANO_6G_PRODUCT_ID 0x1266
#define NANO_7G_PRODUCT_ID 0x1267
#define CLASSIC_6G_PRODUCT_ID 0x1261
#define IPAD_1G_PRODUCT_ID 0x129A

#define IPAD_2G_WIFI_ID 0x129F
#define IPAD_2G_GSM_ID 0x12A2
#define IPAD_2G_CDMA_ID 0x12A3
#define IPAD_2G_WIFI_RA_ID 0x12A9

#define IPAD_3G_WIFI_ID 0x12A4
#define IPAD_3G_CDMA_ID 0x12A5
#define IPAD_3G_GLOBAL_ID 0x12A6

#define VENDOR_DEFINED_POWER_REQUEST              0x40
#define EXTRA_CURRENT_500mA_IN_SUSPEND_STATE       500
#define EXTRA_CURRENT_500mA_IN_NO_SUSPEND_STATE    500       // recommended value for iPod
#define EXTRA_CURRENT_1600mA_IN_NO_SUSPEND_STATE  1600       // recommended value for iPad

static const int MAXCERLENGTHTOBESENT = 128;
static const int MAXCERLENGTHSMALLPACKET = 240;
static const int MAXCERLENGTHLARGEPACKET = 450;
static const int SMALLPACKETNUMBEROFBYTES = 255;

const QString PLAYLISTCATEGORY = "Playlist";
const QString ARTISTCATEGORY = "Artist";
const QString ALBUMCATEGORY = "Album";
const QString TRACKCATEGORY = "Track";
const QString ITUNEUCATEGORY = "iTunesU";
const QString PODCASTCATEGORY = "Podcast";
const QString AUDIOBOOKCATEGORY = "Audiobook";
const QString COMPOSERCATEGORY = "Composer";
const QString GENRECATEGORY = "Genre";
const QString DHTrackCategory = "DHTrack";
const QString gIPODiTunesUType("iTunes U"); //Type of the info

struct SiPodDBTrackInfo // IPOD track info structure
{
       quint64  m_UID;
       // m_Type values : AllSongs,Audiobook,Podcasts,iTunesU, Playlist
       QString m_Type;
       QString m_Title;
       QString m_Artist;
       QString m_Album;
       QString m_Genre;
       QString m_Composer;
       int m_trackIndex;
       int m_trackDuration;
       qint16 m_albumTrackIndex;
       qint16 m_chaptorCount;
       int m_TrackCount;
       int m_TopLevelIndex;
       QString m_TopLevelName;

       SiPodDBTrackInfo () :
           m_UID(0),
           m_Type(""),
           m_Title(""),
           m_Artist(""),
           m_Album(""),
           m_Genre(""),
           m_Composer(""),
           m_trackIndex(0),
           m_trackDuration(0),
           m_albumTrackIndex(0),
           m_chaptorCount(0),
           m_TrackCount(-1),
           m_TopLevelIndex(-1),
           m_TopLevelName(""){}
};

#define DOT "."

//error codes
const int errDeviceIsAlreadyInHIDMode = -9999;

//token strings
const QString gIdentifyToken("IdentifyToken");
const QString gAccCapsToken("AccCapsToken");
const QString gAccInfoToken("AccInfoToken");
const QString gEAProtocolToken("EAProtocolToken");
const QString gBundleSeedIDPrefToken("BundleSeedIDPrefToken");

//commom strings
const QString gUnknownTitle("Unknown Title");
const QString gUnknownArtist("Unknown Artist");
const QString gUnknownAlbum("Unknown Album");
const QString gUnknownGenre("Unknown");
const QString gUnknownComposer("Unknown");
const QString gUnknown("Unknown");

const QString fVolumePath_IPod = "file:///ipod";
const QString fForwardSlash = "/"; /* ForwardSlash */
const QString fUnderscoreChar = "_"; /* Underscore */

// added by jonghwan.cho@lge.com    2013.04.17.
// for iOS 6.1
enum PlayState {
    ePlayState_STOP   = 0,
    ePlayState_PLAY   = 1,
    ePlayState_PAUSE  = 2,
    ePlayState_FF     = 3,
    ePlayState_REW    = 4
};

enum RepeatState {
    eRepeatState_NONE = 0,
    eRepeatState_ONE  = 1,
    eRepeatState_ALL  = 2
};

enum ShuffleState {
    eShuffleState_Disabled = 0,
    eShuffleState_Enabled  = 1
};
// for iOS 6.1
// added by jonghwan.cho@lge.com    2013.04.17.

// { Added by jonghwan.cho@lge.com  2013.05.21.
enum EPlaybackState {
    ePlaybackState_Reserved_0x00 = 0,
    ePlaybackState_Reserved_0x01,
    ePlaybackState_Stoppped_0x02,
    ePlaybackState_Reserved_0x03,
    ePlaybackState_Reserved_0x04,
    ePlaybackState_FFWSeekStarted_0x05,
    ePlaybackState_REWSeekStarted_0x06,
    ePlaybackState_FFW_REW_SeekStopped_0x07,
    ePlaybackState_Reserved_0x08,
    ePlaybackState_Reserved_0x09,
    ePlaybackState_Playing_0x0A,
    ePlaybackState_Paused_0x0B,
    ePlaybackState_Reserved_0x0C_0xFF
};
// } Added by jonghwan.cho@lge.com  2013.05.21.
#endif // COMMONVARS_H
