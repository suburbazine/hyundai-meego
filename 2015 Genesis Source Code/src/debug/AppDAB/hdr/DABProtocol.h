#ifndef DABPROTOCOL_H
#define DABPROTOCOL_H

#define DAB_FRAME_START_SYMBOL              0xA5
#define DAB_CONTINUITY_INTERMEDIATEA_PACKET (0x00)
#define DAB_CONTINUITY_LAST_PACKET          (0x01<<2)
#define DAB_CONTINUITY_FIRST_PACKET         (0x02<<2)
#define DAB_CONTINUITY_ONE_N_ONLY_PACKET    (0x03<<2)
#define DAB_DEVICE_UART0                    0x00
#define DAB_DEVICE_UART1                    0x01
#define DAB_DEVICE_SPI0                     0x02
#define DAB_DEVICE_SPI1                     0x03
#define DAB_PACKET_TYPE_REQUEST             0x01
#define DAB_PACKET_TYPE_RESPONSE            0x02
#define DAB_PACKET_TYPE_EVENT               0x04

#include <QTime>

enum eDAB_COMMAND_TYPE
{
    DAB_REQUEST_COMMAND_TYPE            = 0x01,
    DAB_RESPONSE_COMMAND_TYPE           = 0x02,
    DAB_EVENT_COMMAND_TYPE              = 0x04,
    DAB_SPI_EVENT_COMMAND_TYPE          = 0x08,
};

//Command
enum eREQEUST_COMMAND
{
    REQ_GET_VERSION                     = 0x0000,
    REQ_SET_FULL_SCAN                   = 0x0001,
    REQ_SET_SINGLE_SCAN                 = 0x0002,
    REQ_SET_SEEK_SERVICE                = 0x0003,
    REQ_SET_SELECT_SERVICE              = 0x0004,
    REQ_SET_ANNOUNCEMENT_FLAG           = 0x0005,
    REQ_GET_SERVICE_LIST                = 0x0006,
    REQ_GET_DLS                         = 0x0007,
    REQ_GET_SLS                         = 0x0008,
    REQ_GET_EPG                         = 0x0009,
    REQ_SET_MODULE_MODE                 = 0x000A,
    REQ_DOWNLOAD_IMAGE                  = 0x000B,
    REQ_GET_ANNOUNCEMENT_FLAG           = 0x000C,
    REQ_SET_EVENT_SEND_TIME             = 0x000D,
    REQ_SET_DRC_FLAG                    = 0x000E,
    REQ_SET_MUTE                        = 0x000F,
    REQ_SPI_TEST_DATA                   = 0x0011,
    REQ_SET_SEEK_STOP                   = 0x0011,
    REQ_SET_SELECT_STOP                 = 0x0012,
    REQ_SET_SERVICE_FOLLOWING_FLAG      = 0x0013,
    REQ_SET_SELECT_ANNOUNCEMENT_SERVICE = 0x0014,
    REQ_SET_AUTO_SCAN                   = 0x0015,
    REQ_SET_FM_SERVICE                  = 0x0016,
    REQ_SET_BROADCASTING                = 0x0017,
    REQ_GET_AUDIO_INFO                  = 0x0018,
    REQ_SET_BAND_SELECTION              = 0x0019,
    REQ_SET_SERVICE_INFO                = 0x0020,
    REQ_SET_FIG_DATA_STATUS             = 0x0021,
    REQ_SET_ANNOUNCE_DELAY              = 0x0022,
    REQ_SET_ANNOUNCE_TIMEOUT            = 0x0023,
    REQ_SET_SERVLINK_MUTE_TIMEOUT       = 0x0024,
    REQ_SET_SERVLINK_CER_VALUE          = 0x0025,
    REQ_SET_PINKNOISE_CER_VALUE         = 0x0026,
    REQ_SET_SIGSTATUS_CER_VALUE         = 0x0027,
    REQ_SET_ANNOUNCEMENT_WEAK_TIMEOUT   = 0x0028,
    REQ_SET_ANTENNA_STATUS              = 0x0029
};

enum eRESPONSE_COMMAND
{
    RSP_GET_VERSION                     = 0x0000,
    RSP_SET_FULL_SCAN                   = 0x0001,
    RSP_SET_SINGLE_SCAN                 = 0x0002,
    RSP_SET_SEEK_SERVICE                = 0x0003,
    RSP_SET_SELECT_SERVICE              = 0x0004,
    RSP_SET_ANNOUNCEMENT_FLAG           = 0x0005,
    RSP_GET_SERVICE_LIST                = 0x0006,
    RSP_GET_DLS                         = 0x0007,
    RSP_GET_SLS                         = 0x0008,
    RSP_GET_EPG                         = 0x0009,
    RSP_SET_MODULE_MODE                 = 0x000A,
    RSP_DOWNLOAD_IMAGE                  = 0x000B,
    RSP_GET_ANNOUNCEMENT_FLAG           = 0x000C,
    RSP_SET_EVENT_SEND_TIME             = 0x000D,
    RSP_SET_DRC_FLAG                    = 0x000E,
    RSP_SET_MUTE                        = 0x000F,
    RSP_SET_SEEK_STOP                   = 0x0011,
    RSP_SET_SELECT_STOP                 = 0x0012,
    RSP_SET_SERVICE_FOLLOWING_FLAG      = 0x0013,
    RSP_SET_SELECT_ANNOUNCEMENT_SERVICE = 0x0014,
    RSP_SET_AUTO_SCAN                   = 0x0015,
    RSP_SET_FM_SERVICE                  = 0x0016,
    RSP_SET_BROADCASTING                = 0x0017,
    RSP_GET_AUDIO_INFO                  = 0x0018,
    RSP_SET_BAND_SELECTION              = 0x0019,
    RSP_SET_SEL_SERVICE_INFO            = 0x0020,
    RSP_SET_FIG_DATA_STATUS             = 0x0021,
    RSP_SET_ANNOUNCE_DELAY              = 0x0022,
    RSP_SET_ANNOUNCE_TIMEOUT            = 0x0023,
    RSP_SET_SERVLINK_MUTE_TIMEOUT       = 0x0024,
    RSP_SET_SERVLINK_CER_VALUE          = 0x0025,
    RSP_SET_PINKNOISE_CER_VALUE         = 0x0026,
    RSP_SET_SIGSTATUS_CER_VALUE         = 0x0027,
    RSP_SET_ANNOUNCEMENT_WEAK_TIMEOUT   = 0x0028,
    RSP_SET_ANTENNA_STATUS              = 0x0029,
    RSP_GET_ANNOUNCEMENT_STATUS         = 0x0030,
    RSP_GET_PINK_STATUS                 = 0x0031
};

enum eEVENT_COMMAND
{
    EVT_SEND_MODULE_STATUS              = 0x0000,
    EVT_SEND_SCAN_STATUS                = 0x0001,
    EVT_SEND_CHANGE_SERVICE             = 0x0003,
    EVT_SEND_ANNOUNCEMENT               = 0x0004,
    EVT_SEND_TIME                       = 0x0005,
    EVT_SEND_QOS                        = 0x0006,
    EVT_UPDATE_DLS                      = 0x0007,
    EVT_UPDATE_SLS                      = 0x0008,
    EVT_UPDATE_EPG                      = 0x0009,
    EVT_DOWNLOAD_STATUS                 = 0x000A,
    EVT_DATA_INFO                       = 0x000B,
    EVT_SERVICE_LINKING_INFOMATION      = 0x000C,
    EVT_SEND_AUTO_SCAN_INFO             = 0x000D,
    EVT_SEND_SIGNAL_INFO                = 0x000E,
    EVT_SERVICE_FOLLOWING_STATE         = 0x000F,
    EVT_SEND_PI_ARRAY                   = 0x0010,
    EVT_SEND_DTC_UART                   = 0x0011,
    EVT_SEND_LINK_ARRAY                 = 0x0012,
    EVT_SEND_COUNTRY_INFO               = 0x0013,
    EVT_SEND_FIG_DATA                   = 0x0014,
    EVT_SEND_PINK_STATUS                = 0x0015,
    EVT_SEND_USER_APP_TYPE              = 0x0016,
    EVT_SEND_ANNOUNCEMENT_TA_INVALID    = 0x001A,
};

enum eSPI_EVENT_COMMAND
{
    CMD_EVENT_SPI_JPEG          = 0x0000,
    CMD_EVENT_SPI_PNG           = 0x0001,
    CMD_EVENT_SPI_BWS           = 0x0003,
    CMD_EVENT_SPI_TPEG          = 0x0004,
    CMD_EVENT_SPI_EPG           = 0x0005,
    CMD_EVENT_SPI_STATION_LOGO  = 0x0006
};

enum eSPI_PACKET_TYPE
{
    SPI_INTERMEDIATE_PACKET = 0,
    SPI_LAST_PACKET         = 1,
    SPI_FIRST_PACKET        = 2,
    SPI_ONE_AND_ONLY_PACKET = 3
};

#pragma pack(1)
////////////////////////////////////////////////////////////////////////////////////////////////
// Response
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _stRSP_GET_VERSION {
    quint8  BootloaderMajorVersion;
    quint8  BootloaderMinorVersion;
    quint8  KernelMajorVersion;
    quint8  KernelMinorVersion;
    quint8  ApplicationMajorVersion;
    quint8  ApplicationMinorVersion;
    quint8  ApplicationMicroVersion;
    quint8  BuildTime[30]; // size: 30, Jan 10 2011 (String Type)
    quint8  BootCount;
}stRSP_GET_VERSION;


typedef struct _stRSP_SET_FULL_SCAN {
    quint8  Band;
    quint8  Action;
}stRSP_SET_FULL_SCAN;

enum eREQ_SET_FULL_SCAN_BAND {
    eREQ_SET_FULL_SCAN_BAND_KOREAN = 0x00,
    eREQ_SET_FULL_SCAN_BAND_EUROPE = 0x01,
    eREQ_SET_FULL_SCAN_BAND_BANDIII= 0x02,  //Spec 1.5
    eREQ_SET_FULL_SCAN_BAND_L_BAND = 0x03   //Spec 1.5
};

enum eREQ_SET_FULL_SCAN_ACTION {
    eREQ_SET_FULL_SCAN_ACTION_START = 0x00, //Start Scan
    eREQ_SET_FULL_SCAN_ACTION_CANCEL = 0x01 //Cancel Scan
};

enum eREQ_SET_AUTO_SCAN_ACTION {
    eREQ_SET_AUTO_SCAN_ACTION_START = 0x00, //Start Scan
    eREQ_SET_AUTO_SCAN_ACTION_CANCEL = 0x01 //Cancel Scan
};

enum eREQ_SET_AUTO_SCAN_DEBUG {
    eREQ_SET_AUTO_SCAN_DEBUG_ON      = 0x00, //Debug On
    eREQ_SET_AUTO_SCAN_DEBUG_OFF     = 0x01  //Debug Off
};

enum eREQ_SET_BROADCASTING {
    eREQ_SET_BROADCASTING_DAB_ONLY = 0x00, //DAB Only
    eREQ_SET_BROADCASTING_DAB_DMB  = 0x01, //DAB + DMB
};

typedef struct _stRSP_SET_SINGLE_SCAN {
    quint8  Band;
    quint32 Frequency; //Khz
} stRSP_SET_SINGLE_SCAN;

enum eREQ_SET_SINGLE_SCAN_BAND {
    eREQ_SET_SINGLE_SCAN_BAND_KOREAN = 0x00,
    eREQ_SET_SINGLE_SCAN_BAND_EUROPE = 0x01
};

enum eREQ_SET_SEEK_SERVICE_DIRECTION {
    eREQ_SET_SEEK_SERVICE_DIRECTION_UPWARD,
    eREQ_SET_SEEK_SERVICE_DIRECTION_DOWNWARD
};
enum eREQ_SET_SEEK_SERVICE_FILTERTYPE {
    eREQ_SET_SEEK_SERVICE_FILTERTYPE_NONE   = 0x00,
    eREQ_SET_SEEK_SERVICE_FILTERTYPE_PTY    = 0x01,
    eREQ_SET_SEEK_SERVICE_FILTERTYPE_LANG   = 0x02,
};

enum DAB_SET_SCAN_TYPE{
    DAB_SET_FULL_SCAN         = 0,
    DAB_SET_SINGLE_SCAN,
    DAB_SET_BSM,
    DAB_SET_SCAN_CANCEL
};

typedef struct _stRSP_SET_SEEK_SERVICE {
    quint8  Band;
    quint8  Direction;
    quint32 Frequency;      //Spec v1.2
    quint32 ServiceID;      //Spec v1.2
    quint8  SubChId;        //Spec v1.2
    quint8  FilterType;
    quint8  FilterValueLen;
    quint8  FilterValue;
} stRSP_SET_SEEK_SERVICE;

typedef struct _stRSP_SET_SELECT_SERVICE {
    quint32 Frequency;              //size:4
    quint8  ServiceType;            //size:1
    quint32 ServiceID;              //size:4
    quint8  SubChId;                //size:1
    quint16 Bitrate;                //size:2
    quint8  SubCHSize;              //size:1
    quint16 Address;                //size:2
    quint8  ps;                     //size:1
    quint8  Label[16];              //size:16
    quint8  PICodeCount;            //size:1
    quint16 PICodeValue[12];        //size:24
}stRSP_SET_SELECT_SERVICE;

typedef struct _stREQ_SET_SELECT_SERVICE {
    quint8      startSymbol;
    quint8      continuity;
    quint16     frameLength;
    //stPacketHeader packetHeader;
    quint8      packetType;
    quint16     packetCmd;
    quint8      packetReserved;
    quint16     packetLength;
    stRSP_SET_SELECT_SERVICE packetData;
    //quint8      packetPadding;
    quint16     frameCRC;
}stREQ_SET_SELECT_SERVICE;

typedef struct _stSET_SERVICE_INFO {
    quint32     Frequency;              //size:4
    quint8      ServiceType;            //size:1
    quint32     ServiceID;              //size:4
    quint8      SubChId;                //size:1
    quint16     Bitrate;                //size:2
    quint8      Label[16];              //size:16
    quint8      labelCharset;           //size:1
    quint8      PICodeCount;            //size:1
    quint16     PICodeValue[12];        //size:24
}stSET_SERVICE_INFO;

typedef struct _stREQ_SET_SERVICE_INFO {
    quint8      startSymbol;
    quint8      continuity;
    quint16     frameLength;
    quint8      packetType;
    quint16     packetCmd;
    quint8      packetReserved;
    quint16     packetLength;
    stSET_SERVICE_INFO packetData;
    quint16     frameCRC;
}stREQ_SET_SERVICE_INFO;

typedef struct _stRSP_SET_SELECT_ANNOUNCEMENT_SERVICE {
    quint32 Frequency;          //size:4
    quint8  ServiceType;        //size:1
    quint32 ServiceID;          //size:4
    quint8  SubChId;            //size:1
    quint16 Bitrate;            //size:2
    quint8  SubCHSize;          //size:1
    quint16 Address;            //size:2
    quint8  ps;                 //size:1
    quint8  Label[16];          //size:16
    quint8  PICodeCount;        //size:1
    quint16 PICodeValue[12];    //size:24
    quint16 Flags;              //size:2
    quint8  Status;             //size:1
}stRSP_SET_SELECT_ANNOUNCEMENT_SERVICE;

typedef struct _stREQ_SET_SELECT_ANNOUNCEMENT_SERVICE {
    quint8      startSymbol;
    quint8      continuity;
    quint16     frameLength;
    quint8      packetType;
    quint16     packetCmd;
    quint8      packetReserved;
    quint16     packetLength;
    stRSP_SET_SELECT_ANNOUNCEMENT_SERVICE packetData;
    quint8      padding;
    quint16     frameCRC;
}stREQ_SET_SELECT_ANNOUNCEMENT_SERVICE;


typedef struct _stRSP_SET_ANNOUNCEMENT_FLAG {
    quint16 Flag;
}stRSP_SET_ANNOUNCEMENT_FLAG;

typedef struct _stRSP_GET_ENSEMBLE_LIST{
    quint32     Frequency;
    quint16     EnsembleID;
    quint8      InternationalTableID; //v1.0
    quint8      EnsembleLabel[16];
    quint8      EnsembleCharset;
    quint16     EnsembleCharFlag;
    quint8      ServiceCount;
}stRSP_GET_ENSEMBLE_LIST;

typedef struct _stRSP_GET_AUDIO_INFO{
    quint32     Frequency;
    quint8      Label[16];
    quint8      ServiceType;
    quint8      Bitrate;
    quint8      DACSamplingRate;
    quint8      ChannelConfig;
    quint8      Sbr;
    quint8      ProtectType;
    quint8      ProtectLevel;
    quint8      ProtectOption;
}stRSP_GET_AUDIO_INFO;

typedef struct _stRSP_GET_SERVICE_LIST {
    quint32     ServiceID;              //size:4
    quint8      ServiceType;            //size:1
    quint8      SubChId;                //size:1
    quint16     Bitrate;                //size:2
    quint8      PS;                     //size:1
    quint8      Charset;                //size:1
    quint16     CharsetFlag;            //size:1
    quint8      ProgrammeType;          // size:1
    quint8      Language;               //size:1
    quint8      Label[16];              //size:16
    quint16     ASUFlags;               //size:2
    quint8      ASUClusterCount;        //size:1
    quint8      ASUClusterIDs[23];      //size:23
    quint8      PICodeCount;            //size:1
    quint8      PICodeType[12];         //size:12
    quint16     PICodeValue[12];        //size:24
    quint16     PICodeValidation;       //size:2
    quint8      SubCHSize;              //size:1
    quint16     Address;                //size:2
}stRSP_GET_SERVICE_LIST;

typedef struct _stASU_INFO {
    quint16     ASUFlags;               //size:2
    quint8      ASUClusterCount;        //size:1
    quint8      ASUClusterIDs[23];      //size:23
}stASU_INFO;

typedef struct _stPICODE_INFO {
    quint8      PICodeCount;            //size:1
    quint8      PICodeType[12];         //size:12
    quint16     PICodeValue[12];        //size:24
    quint16     PICodeValidation;       //size:2
}stPICODE_INFO;

typedef struct _stRSP_SET_MUTE_FLAG {
    quint8      audioFlag;      //size:1
    quint8      videoFlag;      //size:1
}stRSP_SET_MUTE_FLAG;

typedef struct _stRSP_SET_AUTO_SCAN {
    quint8      band;      //size:1
    quint8      action;    //size:1
}stRSP_SET_AUTO_SCAN;

typedef struct _stRSP_SET_FM_SERVICE {
    quint8      band;      //size:1
    quint16     PICode;    //size:2
}stRSP_SET_FM_SERVICE;

typedef struct _stEVT_SEND_AUTO_SCAN {
    quint8      status;         //size:1
    quint32     frequency;      //size:4
    quint8      lock;           //size:1
}stEVT_SEND_AUTO_SCAN;

typedef struct _stEVT_SEND_COUNTRY_INFO {
    quint8      ecc;            //size:1
    quint8      countryID;      //size:1
    quint8      ltoSense;       //size:1
    quint8      lto;
    quint8      interTableID;
}stEVT_SEND_COUNTRY_INFO;

typedef struct _stEVT_SEND_TA_INVALID_INFO {
    quint16     preFlag;        //size:2
    quint16     currentFlag;    //size:2
}stEVT_SEND_TA_INVALID_INFO;

typedef struct _stEVT_DATA_INFO {
    quint16 userApplicationType;
    quint8 subChannelID;
    quint32 frequency;
    quint16 bitrate;
    quint8 label[16];
}stEVT_DATA_INFO;
////////////////////////////////////////////////////////////////////////////////////////////////
// EVENT
////////////////////////////////////////////////////////////////////////////////////////////////
//EVT_SEND_MODULE_STATUS
enum eEVT_SEND_MODULE_STATUS
{
    EVT_SEND_MODULE_STATUS_BOOT = 0,    //Module Booting status
    EVT_SEND_MODULE_STATUS_IDLE,        //Module Booting Completed. request connection to HOST
    EVT_SEND_MODULE_STATUS_READY,       //Connection Completed. Ready to receive Command
    EVT_SEND_MODULE_STATUS_WAITING,     //Broadcasting Waiting( Service State : None )
    EVT_SEND_MODULE_STATUS_SERVICE,     //Broadcasting Service( Service State : Good )
    EVT_SEND_MODULE_STATUS_SCANNING,    //Scan Processing(Service State : None )
    EVT_SEND_MODULE_STATUS_SCANSTOP,    //Scan User Stop(Service State : None )
    EVT_SEND_MODULE_STATUS_SCANEND,      //Scan Completed(Service State : None )
    EVT_SEND_MODULE_STATUS_SEEKEND,      //not find select channel(Service State : None )
    EVT_SEND_MODULE_STATUS_SEEKSTOP      //Seek Service Stop
};
enum eEVT_SEND_SERVICE_STATUS
{
    EVT_SEND_SERVICE_STATUS_NONE, // None selection for DAB
    EVT_SEND_SERVICE_STATUS_GOOD, // Playing Status
    EVT_SEND_SERVICE_STATUS_WEAK  //
};
typedef struct _stEVT_SEND_MODULE_STATUS {
    quint8  ModuleState;        //size:1
    quint8  ServiceState;       //size:1
} stEVT_SEND_MODULE_STATUS;


//EVT_SEND_SCAN_STATUS
enum eEVT_SEND_SCAN_STATUS
{
    EVT_SEND_SCAN_STATUS_UNLOCK,    //UnLock
    EVT_SEND_SCAN_STATUS_LOCK       //Lock
};

typedef struct _stEVT_SEND_SCAN_STATUS {
    quint32  Frequency;             //size:4
    quint8  FrequencyIndex;         //size:1
    quint8  TotalFrequencyCount;    //size:1
    quint8  LockStatus;             //size:1
    quint8  TotalDMBCount;          //size:1
    quint8  TotalDABCount;          //size:1
} stEVT_SEND_SCAN_STATUS;

enum eEVT_SEND_CHANGE_SERVICE_EVENT
{
    EVT_SEND_CHANGE_SERVICE_EVENT_SELECT_SERVICE = 0x00,
    EVT_SEND_CHANGE_SERVICE_EVENT_SEEK,
    EVT_SEND_CHANGE_SERVICE_EVENT_ANNOUNCEMENT,
    EVT_SEND_CHANGE_SERVICE_EVENT_RECONFIGURATION_CHANGE_SERVICE,
    EVT_SEND_CHANGE_SERVICE_EVENT_RECONFIGURATION_NOT_CHANGE_SERVICE,
    EVT_SEND_CHANGE_SERVICE_EVENT_DAB_TO_DAB,
    EVT_SEND_CHANGE_SERVICE_EVENT_PLAY_PREVIOUS_SERVICE,
    EVT_SEND_CHANGE_SERVICE_EVENT_FM_TO_DAB,
    EVT_SEND_CHANGE_SERVICE_EVENT_NORMAL
};

typedef struct _stEVT_SEND_CHANGE_SERVICE {
    quint32 Frequency;              //size:4
    quint8  ServiceType;            //size:1
    quint32 ServiceID;              //size:4
    quint8  SubChId;                //size:1
    quint16 Bitrate;                //size:2
    quint8  Pty;                    //size:1
    quint8  EnsembleLabel[16];      //size:16
    quint8  EnsembleCharset;        //size:1
    quint8  Label[16];              //size:16
    quint8  LabelCharset;           //size:1
    quint8  Event;                  //size:1
}stEVT_SEND_CHANGE_SERVICE;

enum eEVT_SEND_ANNOUNCEMENT_STATUS {
    EVT_SEND_ANNOUNCEMENT_FINISH,
    EVT_SEND_ANNOUNCEMENT_START
};

enum eEVT_SEND_TIME_STATUS {
    EVT_SEND_TIME_STOP     = 0x00,
    EVT_SEND_TIME_START
};

enum eEVT_SET_AUDIO_MUTE_FLAG {
    EVT_SET_AUDIO_OFF     = 0x00,
    EVT_SET_AUDIO_ON
};

enum eEVT_SET_VIDEO_MUTE_FLAG {
    EVT_SET_VIDEO_OFF     = 0x00,
    EVT_SET_VIDEO_ON
};

typedef struct _stEVT_SEND_ANNOUNCEMENT_SERVICE {
    quint32 Frequency;              //size:4
    quint8  ServiceType;            //size:1
    quint32 ServiceID;              //size:4
    quint8  SubChId;                //size:1
    quint16 Bitrate;                //size:2
    quint8  SubCHSize;              //size:1
    quint16 Address;                //size:2
    quint8  ps;                     //size:1
    quint8  Label[16];              //size:16
    quint8  PICodeCount;            //size:1
    quint16 PICodeValue[12];        //size:24
}stEVT_SEND_ANNOUNCEMENT_SERVICE;


typedef struct _stEVT_SEND_ANNOUNCEMENT {
    stEVT_SEND_ANNOUNCEMENT_SERVICE taService;
    quint16 Flag;
    quint8  Status;
}stEVT_SEND_ANNOUNCEMENT;

typedef struct _stEVT_SEND_TIME {
    quint16 Year;               //size:2
    quint8  Month;              //size:1
    quint8  Day;                //size:1
    quint8  Hour;               //size:1
    quint8  Minute;             //size:1
    quint8  Second;             //size:1
    quint32 MJD;                //size:4
    quint8  UTC_Flag;           //size:1
    quint8  UTC_Hour;           //size:1
    quint8  UTC_Minute;         //size:1
    quint8  UTC_Second;         //size:1
    quint16 UTC_Milisecond;     //size:2
}stEVT_SEND_TIME;

typedef struct _stEVT_SEND_QOS {
    quint16 CER;       //size:2 // Channel Decoder Error - [Little Endian]
    quint8  SNR;       //size:1 // Signal to noise rate
    quint32 RSSI;      //size:4 // Received Signal Strength Indicate - INT type [Little Endian]

    quint16 CER_sub;       //size:2 // Channel Decoder Error - [Little Endian]
    quint8  SNR_sub;       //size:1 // Signal to noise rate
    quint32 RSSI_sub;      //size:4 // Received Signal Strength Indicate - INT type [Little Endian]
}stEVT_SEND_QOS;

typedef struct _stEVT_SERVICE_LINKING_INFOMATION {
    quint32     ServiceID;      //size:4
    quint8      R_M;            //size:1
    quint16     ensembleID;     //size:2
    quint8      frequencyCnt;   //size:1
}_stEVT_SERVICE_LINKING_INFOMATION;

typedef struct _stEVT_EPG_SAVE_ITEM {
    int sHour;
    int sMinute;
    int sSecond;
    int duration;
    char programTitle[128];
    char description[256];
    quint32 serviceID;
    quint32 frequencyID;
    quint32 mjd;
}stEVT_EPG_SAVE_ITEM;

typedef struct _stEVT_EPG_RESERVE_ITEM {
    int hour;
    int minute;
    int second;
    int duration;
    QTime startTime;
    char programTitle[128];
    char description[256];
    quint32 serviceID;
    quint32 mjd;
    quint32 frequency;
    quint8  serviceType;
    quint8  subChId;
    quint16 bitrate;
    quint16 asuFlags;
    quint8  asuClusterCount;
    quint8  asuClusterIDs[23];
    quint8  picodeCount;
    quint8  picodeType[12];
    quint16 picodeValue[12];
    quint16 picodeValidation;
    quint8  subCHSize;
    quint16 address;
    char  sLabel[16];
    quint8  pty;
    char sEnsembleLabel[16];
    int  presetIndex;
}stEVT_EPG_RESERVE_ITEM;

typedef struct _stEVT_SETTING_ITEM {
    int m_iServiceFollowing;
    int m_iBandSelection;
    quint16 m_iAnnouncementFlag;
    bool m_bSlideShowOn;
}stEVT_SETTING_ITEM;

typedef struct _stRSP_SET_SERVLINK_CER_VALUE {
    quint32     dab_worst;
    quint32     dab_bad;
    quint32     dab_nogood;
    quint32     dab_good;
    quint32     dabPlus_worst;
    quint32     dabPlus_bad;
    quint32     dabPlus_nogood;
    quint32     dabPlus_good;
}stRSP_SET_SERVLINK_CER_VALUE;

typedef struct _stREQ_SET_SERVLINK_CER_VALUE {
    quint8      startSymbol;
    quint8      continuity;
    quint16     frameLength;
    quint8      packetType;
    quint16     packetCmd;
    quint8      packetReserved;
    quint16     packetLength;
    stRSP_SET_SERVLINK_CER_VALUE packetdata;
    quint16     frameCRC;
}stREQ_SET_SERVLINK_CER_VALUE;

typedef struct _stRSP_SET_SIG_STATUS_CER_VALUE {
    quint8      status_count;
    quint32     dab_bad;
    quint32     dab_nogood;
    quint32     dab_good;
    quint32     dabPlus_bad;
    quint32     dabPlus_nogood;
    quint32     dabPlus_good;
}stRSP_SET_SIG_STATUS_CER_VALUE;

typedef struct _stREQ_SET_SIG_STATUS_CER_VALUE {
    quint8      startSymbol;
    quint8      continuity;
    quint16     frameLength;
    quint8      packetType;
    quint16     packetCmd;
    quint8      packetReserved;
    quint16     packetLength;
    stRSP_SET_SIG_STATUS_CER_VALUE packetdata;
    quint16     frameCRC;
}stREQ_SET_SIG_STATUS_CER_VALUE;

typedef struct _stRSP_SET_PINKNOISE_CER_VALUE {
    quint8      unmute_status_count;
    quint8      mute_status_count;
    quint32     dab_bad;
    quint32     dab_good;
    quint32     dabPlus_bad;
    quint32     dabPlus_good;
}stRSP_SET_PINKNOISE_CER_VALUE;


typedef struct _stREQ_PINKNOISE_CER_VALUE {
    quint8      startSymbol;
    quint8      continuity;
    quint16     frameLength;
    quint8      packetType;
    quint16     packetCmd;
    quint8      packetReserved;
    quint16     packetLength;
    stRSP_SET_PINKNOISE_CER_VALUE packetdata;
    quint16     frameCRC;
}stREQ_PINKNOISE_CER_VALUE;

enum ePTyCodeTable {
    PTY_NO_PROGRAMME_TYPE = 0x00,
    PTY_NEWS,
    PTY_CURRENT_AFFAIRS,
    PTY_INFORMATION,
    PTY_SPORTS,
    PTY_EDUCATION,
    PTY_DRAMA,
    PTY_CULTURE,
    PTY_SCIENCE,
    PTY_VARIED,
    PTY_POP_MUSIC,
    PTY_ROCK_MUSIC,
    PTY_EASY_LISTENING_MUSIC,
    PTY_LIGHT_CLASSICAL,
    PTY_SERIOUS_CLASSICAL,
    PTY_OTHER_MUSIC,
    PTY_WEATHER,
    PTY_FINANCE,
    PTY_CHILDREN_PROGRAMMES,
    PTY_SOCIAL_AFFAIRS,
    PTY_RELIGION,
    PTY_PHONE_IN,
    PTY_TRAVEL,
    PTY_LEISURE,
    PTY_JAZZ_MUSIC,
    PTY_COUNTRY_MUSIC,
    PTY_NATIONAL_MUSIC,
    PTY_OLDIES_MUSIC,
    PTY_FOLK_MUSIC,
    PTY_DOCUMENTARY = 0x1D,
    PTY_NOT_USED
};

#define MAX_PTY_INDEX PTY_NOT_USED

enum eLanguageCodeTable {
    LANGUAGE_UNKNOWN_    = 0x00,
    LANGUAGE_ALBANIAN   = 0x01,
    LANGUAGE_BRETON     = 0x02,
    LANGUAGE_CATALAN    = 0x03,
    LANGUAGE_CROATIAN   = 0x04,
    LANGUAGE_WELSH      = 0x05,
    LANGUAGE_CZECH      = 0x06,
    LANGUAGE_DANISH     = 0x07,
    LANGUAGE_GERMAN     = 0x08,
    LANGUAGE_ENGLISH    = 0x09,
    LANGUAGE_SPANISH    = 0x0A,
    LANGUAGE_ESPERANTO  = 0x0B,
    LANGUAGE_ESTONIAN   = 0x0C,
    LANGUAGE_BASQUE     = 0x0D,
    LANGUAGE_FAROESE    = 0x0E,
    LANGUAGE_FRENCH     = 0x0F,
    LANGUAGE_FRISIAN    = 0x10,
    LANGUAGE_IRISH      = 0x11,
    LANGUAGE_GAELIC     = 0x12,
    LANGUAGE_GALICIAN   = 0x13,
    LANGUAGE_ICELANDIC  = 0x14,
    LANGUAGE_ITALIAN    = 0x15,
    LANGUAGE_LAPPISH    = 0x16,
    LANGUAGE_LATIN      = 0x17,
    LANGUAGE_LATVIAN    = 0x18,
    LANGUAGE_LUXEMBOURGIAN = 0x19,
    LANGUAGE_LITHUANIAN = 0x1A,
    LANGUAGE_HUNGARIAN  = 0x1B,
    LANGUAGE_MALTESE    = 0x1C,
    LANGUAGE_DUTCH      = 0x1D,
    LANGUAGE_NORWEGIAN  = 0x1E,
    LANGUAGE_OCCITAN    = 0x1F,
    LANGUAGE_POLISH     = 0x20,
    LANGUAGE_PORTUGUESE = 0x21,
    LANGUAGE_ROMANIAN   = 0x22,
    LANGUAGE_ROMANSH    = 0x23,
    LANGUAGE_SERBIAN    = 0x24,
    LANGUAGE_SLOVAK     = 0x25,
    LANGUAGE_SLOVENE    = 0x26,
    LANGUAGE_FINNISH    = 0x27,
    LANGUAGE_SWEDISH    = 0x28,
    LANGUAGE_TURKISH    = 0x29,
    LANGUAGE_FLEMISH    = 0x2A,
    LANGUAGE_WALLOON    = 0x2B,
    LANGUAGE_RFU1       = 0x2C,
    LANGUAGE_RFU2       = 0x2D,
    LANGUAGE_RFU3       = 0x2E,
    LANGUAGE_RFU4       = 0x2F
};

enum DAB_SERVICE_STATUS
{
    DAB_SERVICE_IDLE = 0,
    DAB_SERVICE_READY,
    DAB_SERVICE_FULL_SCAN,
    DAB_SERVICE_SINGLE_SCAN,
    DAB_SERVICE_LIST_SCAN,
    DAB_SERVICE_SELECT,
    DAB_SERVICE_LONG_SEEK,
    DAB_SERVICE_SHORT_SEEK,
    DAB_SERVICE_ANNOUNCEMENT,
    DAB_SERVICE_HANDOVER,
    DAB_SERVICE_RECONFIGURATION,
    DAB_SERVICE_AUTO_SCAN,
    DAB_SERVICE_TUNE_SEEK,
    DAB_SERVICE_END
};

enum DAB_SERVICE_FOLLOWING
{    
    DAB_SERVICE_FOLLOWING_OFF           = 0x00,
    DAB_SERVICE_FOLLOWING_DAB_DAB       = 0x01,
    DAB_SERVICE_FOLLOWING_DAB_FM        = 0x02,
    DAB_SERVICE_FOLLOWING_ON            = 0x03
};

enum DAB_SERVICE_APPLICATION_TYPE
{
    DAB_APPLICATION_BWS                 = 0x0003,
    DAB_APPLICATION_EPG                 = 0x0007
};

#if 1 // 2013.10.31 일단 보류하고 추가로 김경민책임님 메일이 오면 그때 적용할 예정
enum DAB_CLUSTER_OP_STATE_TYPE
{
    DAB_CLUSTER_NOMAR_STATE             = 0x36,
    DAB_CLUSTER_RECEVING_STATE          = 0x3B,
    DAB_CLUSTER_SIGNAL_WEAK_STATE       = 0x3C,
    DAB_CLUSTER_SIGNAL_LOST_STATE       = 0x3D
};
#else
enum DAB_CLUSTER_OP_STATE_TYPE
{
    DAB_CLUSTER_NOMAR_STATE             = 0x36,
    DAB_CLUSTER_RECEVING_STATE          = 0x3E, //0x3B, 2013.10.31 update
    DAB_CLUSTER_SIGNAL_WEAK_STATE       = 0x3F, //0x3C, 2013.10.31 update
    DAB_CLUSTER_SIGNAL_LOST_STATE       = 0x3D
};
#endif

enum DAB_CHANNEL_TYPE
{
    DAB_CHANNEL_DAB_TO_DAB = 0x00,
    DAB_CHANNEL_DAB_TO_FM  = 0x01,
    DAB_CHANNEL_NO_SIGNAL  = 0x02,
    DAB_CHANNEL_ICON_CLEAR = 0x03
};

enum eDAB_EVENT_TYPE
{
    DAB_EVENT_NONE              = 0x00,
    DAB_EVENT_FG                = 0x01,
    DAB_EVENT_BG                = 0x02,
    DAB_EVENT_AV_OFF            = 0x03,
    DAB_EVENT_AV_ON             = 0x04,
    DAB_EVENT_LONG_SEEK         = 0x05,
    DAB_EVENT_SHORT_SEEK        = 0x06,
    DAB_EVENT_PRESET_SCAN       = 0x07,
    DAB_EVENT_LIST_SCAN         = 0x08,
    DAB_EVENT_TUNE_UP           = 0x09,
    DAB_EVENT_TUNE_DOWN         = 0x0A,
    DAB_EVENT_SELECT_SERVICE    = 0x0B,
    DAB_EVENT_AV_MODE_NONE      = 0x0C,
    DAB_EVENT_FM_WEAK           = 0x0D,
    DAB_EVENT_DAB_FM_OFF        = 0x0E,
    DAB_EVENT_AV_MODE_FG        = 0x0F,
    DAB_EVENT_NOTIFY_AV_MODE    = 0x10,
    DAB_EVENT_DISPLAY_OFF       = 0x11,
    DAB_EVENT_ANNOUNCEMENT_STOP = 0x12,
    DAB_EVENT_BT_CALL_START     = 0x13,
    DAB_EVENT_BT_CALL_END       = 0x14,
    DAB_EVENT_UNMUTE            = 0x15,
    DAB_EVENT_MUTE              = 0x16,
    DAB_EVENT_POWER_LOGIC_OFF   = 0x17,
    DAB_EVENT_POWER_NORMAL_ON   = 0x18,
    DAB_EVENT_SOUND_SETTING     = 0x19,
    DAB_EVENT_NOTIFY_FROM_VR    = 0x1A,
    DAB_EVENT_FM_CO_CHANNEL     = 0x1B,
    DAB_EVENT_CLOSE_OPTION_MENU = 0x1C,
    DAB_EVENT_TUNE_PRESSED      = 0x1D,
    DAB_EVENT_VR_START          = 0x1E,
    DAB_EVENT_VR_END          = 0x1F
};

enum eDAB_SERVICE_FOLLOWING_STATE
{
    DAB_SERVICE_FOLLOWING_STATE_READY         = 0x00,
    DAB_SERVICE_FOLLOWING_STATE_SWAP          = 0x01,
    DAB_SERVICE_FOLLOWING_STATE_STOP          = 0x02
};

enum eDAB_OSD_OP_COMMAND
{
    DAB_OP_TYPE_AVOFF = 0x00,
    DAB_OP_TYPE_SEEK,
    DAB_OP_TYPE_TA,
    DAB_OP_TYPE_EPG,
    DAB_OP_TYPE_EPG_STARTED,
    DAB_OP_TYPE_SCAN,
    DAB_OP_TYPE_SELECT,
    DAB_OP_TYPE_TUNE_SEEK,
    DAB_OP_TYPE_INFO,
    DAB_OP_TYPE_CLEAR,
    DAB_OP_TYPE_SCANING,
    DAB_OP_TYPE_NOSIGNAL,
    DAB_OP_TYPE_SEARCHING
};

enum eDAB_EPG_RESERVE_DUP_CHCEK
{
    DAB_EPG_RESERVE_DUP_STARTTIME_SERVICE_ID = 0x00,
    DAB_EPG_RESERVE_DUP_NO
};

enum eDAB_STATUS_BAR_TYPE
{
    DAB_STATUS_BAR_TRAFFIC = 0,
    DAB_STATUS_BAR_NEWS,
    DAB_STATUS_BAR_ALL
};

enum eDAB_ANNOUNCEMENT_STATUS_TYPE
{
    DAB_ANNOUNCEMENT_NONE = 0,
    DAB_ANNOUNCEMENT_TRAFFIC_ON,
    DAB_ANNOUNCEMENT_TRAFFIC_OFF,
    DAB_ANNOUNCEMENT_NEWS_ON,
    DAB_ANNOUNCEMENT_NEWS_OFF
};

enum eDAB_STATION_SORT_TYPE
{
    DAB_STATION_SORT_ENSEMBLE = 0,
    DAB_STATION_SORT_ALPHABET,
    DAB_STATION_SORT_PTY
};

enum eDAB_SERVICE_TYPE
{
    SERVICE_TYPE_DAB             = 0x00,
    SERVICE_TYPE_DAB_PLUS        = 0x01,
    SERVICE_TYPE_DMB             = 0x02
};

#define MAX_LANGUAGE_INDEX LANGUAGE_RFU1

#pragma pack()
#endif // DABPROTOCOL_H
