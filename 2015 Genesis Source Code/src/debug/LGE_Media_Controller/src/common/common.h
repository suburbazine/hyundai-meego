#ifndef COMMON_H
#define COMMON_H

#define USE_SYS_CLASS_TAB2_ASYNC        1
#define TAB_DEVICE_PATH                 "/dev/tab2-0"
#define DVD_DEVICE_PATH_FOR_PE          "/dev/video/dvd"
#define MAX_READ_BUFFER_SIZE            512

#define LGE_CONTROLLER_VER              "2013.01.28.01"

#if USE_SYS_CLASS_TAB2_ASYNC
#define DECK_CONTROLLER_VER             "STREAM.ASYNC(1)"
#else
#define DECK_CONTROLLER_VER             "STREAM.ASYNC(0)"
#endif

#define _DECK_DIAGNOSTIC_               1       
#define _USB_DIAGNOSTIC_                1

#include <QDebug>
#include <QDateTime>
#define LOG() qDebug() << "[MC_2]" << metaObject()->className() << "::" << __func__
#define WARNING() qWarning() << "[MC_1]" << metaObject()->className() << "::" << __func__
#define CRITICAL() qCritical() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << " [MC_0]" << metaObject()->className() << "::" << __func__


enum MEDIA_PLAYER_FAST_SPEED
{
    eMP_FAST_SPEED_X1   = 0x00,
    eMP_FAST_SPEED_X4   = 0x01,
    eMP_FAST_SPEED_X20  = 0x02
};

enum MEDIA_PLAYER_RANDOM_MODE
{
    eMP_RANDOM_OFF              = 0x00,

    // FOR CDDA/VIDOE-CD
    eMP_RANDOM_TRACK,

    // FOR USB/JUKEBOX/COMP-AUDIO
    eMP_RANDOM_FILE_IN_FOLDER,
    eMP_RANDOM_FILE_IN_DISC,
    eMP_RANDOM_FOLDER,
    eMP_RANDOM_FILE_BEYOUND_FOLDER,

    // FOR DVD-VIDEO
    eMP_RANDOM_CHAPTER_IN_TITLE,
    eMP_RANDOM_CHAPTER_IN_DISC,
    eMP_RANDOM_TITLE,

    // FOR DVD-AUDIO
    eMP_RANDOM_TRACK_IN_GROUP,
    eMP_RANDOM_TRACK_IN_DISC,
    eMP_RANDOM_GROUP,

    // FOR DVD-VR
    eMP_RANDOM_CHAPTER_IN_PGPL,
    // eMP_RANDOM_CHAPTER_IN_DISC,
    eMP_RANDOM_PGPL
};

enum MEDIA_PLAYER_REPEAT_MODE
{
    eMP_REPEAT_OFF              = 0x00,

    // FOR CDDA/VIDEO-CD
    eMP_REPEAT_TRACK,

    // FOR USB/JUKEBOX/COMP-AUDIO
    eMP_REPEAT_FILE,
    eMP_REPEAT_FOLDER_UNDER_FOLDER,
    eMP_REPEAT_FOLDER_BEYOUND_FOLDER,

    // FOR DVD-VIDEO
    eMP_REPEAT_CHAPTER,
    eMP_REPEAT_TITLE,

    // FOR DVD-AUDIO
    // eMP_REPEAT_TRACK,
    eMP_REPEAT_GROUP,

    // FOR DVD-VR
    // eMP_REPEAT_CHAPTER,
    eMP_REPEAT_PGPL
};


enum MEDIA_PLAYER_SCAN_MODE
{
    eMP_SCAN_OFF                = 0x00,

    // FOR CDDA/VIDEO-CD
    eMP_SCAN_TRACK,

    // FOR USB/JUKEBOX/COMP-AUDIO
    eMP_SCAN_FILE_IN_FOLDER,
    eMP_SCAN_FILE_IN_DISC,
    eMP_SCAN_FOLDER,
    eMP_SCAN_FILE_BEYOUND_FOLDER,

    // FOR DVD-VIDEO
    eMP_SCAN_CHAPTER_IN_TITLE,
    eMP_SCAN_CHAPTER_IN_DISC,
    eMP_SCAN_TITLE,

    // FOR DVD-AUDIO
    eMP_SCAN_TRACK_IN_GROUP,
    eMP_SCAN_TRACK_IN_DISC,
    eMP_SCAN_GROUP,

    // FOR DVD-VR
    eMP_SCAN_CHAPTER_IN_PGPL,
    // eMP_SCAN_CHAPTER_IN_DISC,
    eMP_SCAN_PGPL
};

enum MEDIA_PLAYER_STATE
{
    eMP_STATE_STOPPED           = 0x00,
    eMP_STATE_PLAYING           = 0x01,
    eMP_STATE_PAUSED            = 0x02,
    eMP_STATE_SEARCHING         = 0x03,
    eMP_STATE_FAST_PLAYING      = 0x04,
    eMP_STATE_NONE
};

enum MEDIA_PLAYER_MEDIA_STATUS
{
    eMP_STATUS_MEDIA_UNKNOWN    = 0x00,
    eMP_STATUS_NOMEDIA,
    eMP_STATUS_LOADING_MEDIA,
    eMP_STATUS_LOADED_MEDIA,
    eMP_STATUS_STALLED_MEDIA,
    eMP_STATUS_BUFFERING_MEDIA,
    eMP_STATUS_BUFFERED_MEDIA,
    eMP_STATUS_END_OF_MEDIA,
    eMP_STATUS_INVALID_MEDIA,
    eMP_STATUS_MEDIA_NONE
};


#endif // COMMON_H
