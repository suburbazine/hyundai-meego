#ifndef DABAPPLICATION_DEF_H
#define DABAPPLICATION_DEF_H
/**
 * FileName: DABApplication_Def.h
 * Author: David.Bae
 * Time: 2011-12-04 16:24
 */

#if defined(__UNIX_DESKTOP__)

#define DAB_IMAGE_ROOT              "/home/meego/Hyunge_Work/DAB_Finality_Linux/AppDAB/qml/images/"
#define DAB_DEFAULT_SOURCE          "/home/meego/Hyunge_Work/DAB_Finality_Linux/AppDAB/qml/subapp/DHDABMain.qml"
#define DAB_DATA_ROOT               "/home/meego/Hyunge_Work/DAB_Finality_Linux/AppDAB/data"
#define DAB_APP_ENGINE_LOG_FILE     "/app/share/AppDAB/log/DABAppEngine_Log.txt"
#define DAB_LOCALIZATION_PATH       "/home/meego/Hyunge_Work/DAB_Finality_Linux/AppDAB/translation"
#define DAB_SPI_LOG_FILE            "/app/share/AppDAB/log/DAB_SPI_Log.txt"
#define DAB_UART_LOG_FILE           "/app/share/AppDAB/log/DAB_UART_Log.txt"
#define DAB_MODE1_DATA_FILE         "/home/meego/Hyunge_Work/DAB_Finality_Linux/AppDAB-build-desktop/DABMode1Data.txt"
#define DAB_SETTING_FILE            "/app/share/AppDAB/Data/DABSettings.txt"
#define DAB_MODULE_VERSION_FILE     "/app/data/AppDAB/Data/DABModuleVersion.txt"
#define SETTING_LOCALIZATION_PATH   "/app/share/AppSettings/localization"
#define DAB_STATION_LOGO_ROOT       "/home/meego/Hyunge_Work/DAB_Finality_Linux/AppDAB-build-desktop/d315.png"
#define DAB_STATION_EPG_ROOT        "/home/meego/Hyunge_Work/DAB_Finality_Linux/AppDAB-build-desktop/EPG/"
#define DAB_EPG_RESERVATION         "/home/meego/Hyunge_Work/DAB_Finality_Linux/AppDAB-build-desktop/Reservation.dat"
#else

#define DAB_IMAGE_ROOT              "/app/share/images/AppDAB/"
#define DAB_DEFAULT_SOURCE          "/app/share/AppDAB/qml/subapp/DHDABMain.qml"
#define DAB_LOCALIZATION_PATH       "/app/share/AppDAB/localization"
#define DAB_SPI_LOG_FILE            "/app/data/AppDAB/log/DAB_SPI_Log.txt"
#define DAB_UART_LOG_FILE           "/app/data/AppDAB/log/DAB_UART_Log.txt"
#define DAB_APP_ENGINE_LOG_FILE     "/app/data/AppDAB/log/DABAppEngine_Log.txt"
#define DAB_APP_LOG_FILE            "/app/data/AppDAB/log/DAB_Log.txt"
#define DAB_UART_LOG_FILE           "/app/data/AppDAB/log/DAB_UART_Log.txt"
#define DAB_DATA_ROOT               "/app/data/AppDAB/Data/"
#define DAB_STATION_LOGO_ROOT       "/app/data/AppDAB/Data/StationLogo/"
#define DAB_STATION_EPG_ROOT        "/app/data/AppDAB/Data/EPG/"
#define DAB_EPG_RESERVATION         "/app/data/AppDAB/Data/Reservation.dat"
#define DAB_MODE1_DATA_FILE         "/app/data/AppDAB/Data/DABMode1Data.txt"
#define DAB_MODE2_DATA_FILE         "/app/data/AppDAB/Data/DABMode2Data.txt"
#define DAB_MODE3_DATA_FILE         "/app/data/AppDAB/Data/DABMode3Data.txt"
#define DAB_SETTING_FILE            "/app/data/AppDAB/Data/DABSettings.ini"
#define DAB_MODULE_VERSION_FILE     "/app/data/AppDAB/Data/DABModuleVersion.txt"
#endif

/* ==================== DAB Setting Value ============================*/
#define ANN_TRAFFIC_ALARM_ON                        0x0003
#define ANN_ALARM_ON                                0x0001

#define SERVICE_FOLLOWING_OFF                       0x00
#define SERVICE_FOLLOWING_ON                        0x03

#define BAND_SELECTION_EUROPE                       0x01
#define BAND_SELECTION_BANDIII                      0x02


/**
  2013.06.30 Mobilus DaehyungE
  채널 Select후 30초이후 TA를 체크해서 없을경우 TA 팝업 표시함.
*/
#define TA_SEARCH_INTERVAL                              30000   // 30 second

/**
  2013.06.30 Mobilus DaehyungE
  DAB 방송에서 Time값이 내려오면 그 Time을 기준으로 1초단위로 App에서 time을 계산한다.
  이유는 GPS 및 Target 시간이 틀어질수도 있기때문이다.
  EPG을 예약하면 이 시간을 기준으로 EPG 예약 팝업을 뛰운다.
*/
#define BROADCASTING_TIME_INTERVAL                      1000    //  1 second

/**
  2013.06.30 Mobilus DaehyungE
  Seek 선택후 5초간 채널을 찾지 못할경우 Seek popup창을 보여준다.
*/
#define SEEK_INTERVAL                                   5000    //  5 second

/**
  2013.06.30 Mobilus DaehyungE
  seek popup을 닫기 위해서는 Module Seek end가 내려와야 하는데 간혈적으로 내려오지 않는 현상이 발생함.
  App에서 30초간 내려오지 않을경우 자동으로 seek popup을 닫는다.
*/
#define SEEK_POPUP_INTERVAL                             30      // 30 second

/**
  2013.06.30 Mobilus DaehyungE
  SLS / DLS / DL+가 2분동안 내려오지 않을경우 해당 값들을 초기화한다.
*/
#define DLS_TIME_INTERVAL                               120     //  2 minute
#define SLS_TIME_INTERVAL                               120     //  2 minute
#define DL_PLUS_TIME_INTERVAL                           120     //  2 minute

/**
  2013.06.30 Mobilus DaehyungE
  Module에서 Weak Signal이 내려오면 그때부터 10초이후에 No Signal 아이콘을 표시한다.
  이유는 HMETC Muller 요청사항
*/
#define WEAK_TIME_INTERVAL                              10000   // 10 second


#define SIGNAL_IGNORE_COUNT                             3 // 5 -> 3

/**
  2013.06.30 Mobilus DaehyungE
  Current 채널이 신호가 좋으면 해당 Frequency에 DAB-FM/DAB-DAB/NO_SIGNAL 아이콘 삭제,
  Good status가 연속으로 4번들어오면 신호가 좋은것으로 판단다.
*/
#define CHANNEL_STATUS_CLEAR_TIME                       4       //  2 second

/**
  2013.06.30 Mobilus DaehyungE
  HMETC Announcement spec 적용
  DAB-FM / AV_MODE 전환시 / 채널 변경시 5초간 TA 무시한다
  2013.10.07 5초에서 1초로 변경
*/
#define DEACTIVE_TA_INTERVAL                            1000    //  5 second -> 1 second

/**
  2013.06.30 Mobilus DaehyungE
  DAB-FM linking interval: 16s->30s->60s
*/
#define DAB_TO_FM_FIRST_INTERVAL                        80        //   16  second
#define DAB_TO_FM_SECOND_INTERVAL                       150       //   30  second
#define DAB_TO_FM_THIRD_INTERVAL                        300       //  60  second

/**
  2013.06.30 Mobilus DaehyungE
  Good이 연속으로 2min 들어오면 DAB 신호가 좋은것으로 판단하고 DAB-FM 전환시간을 First interval로 초기화한다.
*/
#define DAB_TO_FM_INIT_INTERVAL                         600       //  120  second

/**
  2013.06.30 Mobilus DaehyungE
  DAB-FM 상태가 1분이상 유지하면 Good Count를 1초로 초기화한다.
*/
#define FM_LINKING_TIME_INTERVAL                        60000    //   1  minute

/**
  2013.06.30 Mobilus DaehyungE
  DAB-FM 상태에서 FM Weak Signal이 발생하면 10초간 DAB-FM 전환을 막는다..
  2013.09.16 MJY request 10초에서 30초로 변경한다.
  2013.11.07 HMETC request 30 second -> 1 second
*/
#define FM_WEAK_TIME_INTERVAL                           1    //  10  second -> 1 second

#define DAB_SCAN_INTERVAL                               8000
#define DAB_PLUS_SCAN_INTERVAL                          7000
#define DMB_SCAN_INTERVAL                               7500

/* ==================== Module Control Time Value ===================*/
#define ANNOUNCEMENT_DELAY_TIME                         2       // 2 second
#define ANNOUNCEMENT_TIMEOUT                            5       // 5 minute
#define SERVCE_LINK_MUTE_TIMEOUT                        5       // 5 second


#define EPG_NO_LABEL                        "no_label"
#define EPG_NO_DESCRIPTOR                   "no_descriptor"

/* ==================== Enum Type ==================================*/
enum DAB_SIGNAL_INFO
{
    DAB_SIGNAL_GOOD                     = 0x00,
    DAB_SIGNAL_NO_GOOD                  = 0x01,
    DAB_SIGNAL_BAD                      = 0x02,
    DAB_SIGNAL_NONE                     = 0x03
};

enum MICOM_STATUS_INFO
{
    MICOM_STATUS_FM_NO_AVAILABLE           = 0x00,
    MICOM_STATUS_FM_AVAILABLE              = 0x01,
    MICOM_STATUS_FM_AUDIO_ACTIVATE         = 0x02,
    MICOM_STATUS_FM_WEAK                   = 0x03,
    MICOM_STATUS_FM_CO_CHANNEL             = 0x04
};

enum FM_DAB_STATE_INFO
{
    FM_DAB_STATE_FIRST                   = 0x00,
    FM_DAB_STATE_SECOND                  = 0x01,
    FM_DAB_STATE_THIRD                   = 0x02,
    FM_DAB_STATE_FOURTH                  = 0x03
};

enum eDAB_SYSTEM_POPUP_TYPE
{
    DAB_POPUP_NONE                       = 0,
    DAB_POPUP_TA_TYPE,
    DAB_POPUP_ALARM_TYPE,
    DAB_POPUP_PRESERVE_TYPE,
    DAB_POPUP_TA_SEARCH_TYPE,
    DAB_POPUP_TA_NO_SEARCH_TYPE
};

enum eDAB_SERVICE_FOLLOWING_STATE_INFO
{
    DAB_SERVICE_FOLLOWING_STATE_AUDIO_DAB           = 0x00,
    DAB_SERVICE_FOLLOWING_STATE_AUDIO_FM            = 0x01,
    DAB_SERVICE_FOLLOWING_STATE_AUDIO_FM_CHANGEING  = 0x02,
    DAB_SERVICE_FOLLOWING_STATE_AUDIO_DAB_CHANGEING = 0x03
};

enum eTA_STATUS
{
    TA_STATUS_INVAILD = 0,
    TA_STATUS_OFF     = 1,
    TA_STATUS_ON      = 2
};

#endif // DABAPPLICATION_DEF_H
