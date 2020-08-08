/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFileManager_UI_main.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFileManager_UI_main.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFileManager_UI_main.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UIMainDef[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
      14,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
      10, 0x0,   10,   70,
      25, 0x0,    8,   90,
      42, 0x0,    2,  106,
      50, 0x0,    3,  110,
      61, 0x0,   58,  116,
      72, 0x0,   18,  232,
      90, 0x0,   53,  268,
     103, 0x0,    2,  374,
     120, 0x0,    1,  378,
     134, 0x0,    3,  380,
     149, 0x0,    3,  386,
     163, 0x0,    3,  392,
     177, 0x0,    5,  398,
     196, 0x0,   13,  408,

 // enum data: key, value
     214, uint(UIMainDef::MODE_AREA_TEXT_DEFAULT),
     237, uint(UIMainDef::MODE_AREA_TEXT_COPY_FROM),
     262, uint(UIMainDef::MODE_AREA_TEXT_COPY_TO),
     285, uint(UIMainDef::MODE_AREA_TEXT_ENTER_FOLDER_NAME),
     318, uint(UIMainDef::MODE_AREA_TEXT_RENAME),
     340, uint(UIMainDef::MODE_AREA_TEXT_MOVE_FROM),
     365, uint(UIMainDef::MODE_AREA_TEXT_MOVE_TO),
     388, uint(UIMainDef::MODE_AREA_TEXT_DELETE),
     410, uint(UIMainDef::MODE_AREA_TEXT_WAITING_DISPLAY),
     441, uint(UIMainDef::MODE_AREA_TEXT_INVALID),
     464, uint(UIMainDef::BOTTOM_BAR_INVISIBLE),
     485, uint(UIMainDef::BOTTOM_BAR_COPY_TO),
     504, uint(UIMainDef::BOTTOM_BAR_COPY_FROM),
     525, uint(UIMainDef::BOTTOM_BAR_SELECT_DESELECT),
     552, uint(UIMainDef::BOTTOM_BAR_MOVE_TO),
     571, uint(UIMainDef::BOTTOM_BAR_DELETE),
     589, uint(UIMainDef::BOTTOM_BAR_MOVE),
     605, uint(UIMainDef::BOTTOM_BAR_DELETE_ALL),
     627, uint(UIMainDef::DEVICE_USB),
     638, uint(UIMainDef::DEVICE_JUKEBOX),
     653, uint(UIMainDef::FILE_TYPE_AUDIO),
     669, uint(UIMainDef::FILE_TYPE_VIDEO),
     685, uint(UIMainDef::FILE_TYPE_PICTURE),
     703, uint(UIMainDef::POPUP_TYPE_START),
     720, uint(UIMainDef::POPUP_TYPE_COPY),
     736, uint(UIMainDef::POPUP_TYPE_COPYING),
     755, uint(UIMainDef::POPUP_TYPE_COPY_COMPLETE),
     780, uint(UIMainDef::POPUP_TYPE_DELETE),
     798, uint(UIMainDef::POPUP_TYPE_DELETE_FOLDER),
     823, uint(UIMainDef::POPUP_TYPE_DELETE_ALL),
     845, uint(UIMainDef::POPUP_TYPE_DELETING),
     865, uint(UIMainDef::POPUP_TYPE_DELETE_COMPLETE),
     892, uint(UIMainDef::POPUP_TYPE_NOUSB),
     909, uint(UIMainDef::POPUP_TYPE_FOLDER_OPERATION),
     937, uint(UIMainDef::POPUP_TYPE_FILE_VIDEO_JUKEBOX_OPERATION),
     977, uint(UIMainDef::POPUP_TYPE_FILE_VIDEO_USB_OPERATION),
    1013, uint(UIMainDef::POPUP_TYPE_FILE_PICTURE_JUKEBOX_OPERATION),
    1055, uint(UIMainDef::POPUP_TYPE_FILE_PICTURE_USB_OPERATION),
    1093, uint(UIMainDef::POPUP_TYPE_SET_AS_FRAME),
    1117, uint(UIMainDef::POPUP_TYPE_FRAME_SAVED),
    1140, uint(UIMainDef::POPUP_TYPE_EDIT_MODE),
    1161, uint(UIMainDef::POPUP_TYPE_FOLDER_NAME_IS_TOO_LONG),
    1196, uint(UIMainDef::POPUP_TYPE_FOLDER_ALREADY_EXIST),
    1228, uint(UIMainDef::POPUP_TYPE_FILE_ALREADY_EXIST),
    1258, uint(UIMainDef::POPUP_TYPE_EMPTY_FOLDER_NAME),
    1287, uint(UIMainDef::POPUP_TYPE_EMPTY_FILE_NAME),
    1314, uint(UIMainDef::POPUP_TYPE_INCORRECT_CHARACTER),
    1345, uint(UIMainDef::POPUP_TYPE_REPLACE_FILE),
    1369, uint(UIMainDef::POPUP_TYPE_COPY_CANCELLED),
    1395, uint(UIMainDef::POPUP_TYPE_CAPACITY_ERROR),
    1421, uint(UIMainDef::POPUP_TYPE_COPY_CANCEL),
    1444, uint(UIMainDef::POPUP_TYPE_MOVE),
    1460, uint(UIMainDef::POPUP_TYPE_MOVING),
    1478, uint(UIMainDef::POPUP_TYPE_MOVE_ALL),
    1498, uint(UIMainDef::POPUP_TYPE_MOVE_CANCEL),
    1521, uint(UIMainDef::POPUP_TYPE_MOVE_CANCELLED),
    1547, uint(UIMainDef::POPUP_TYPE_MOVE_COMPLETE),
    1572, uint(UIMainDef::POPUP_TYPE_JUKEBOX_CAPACITY),
    1600, uint(UIMainDef::POPUP_TYPE_OPTION_MENU_DELETE_ALL),
    1634, uint(UIMainDef::POPUP_TYPE_DELETING_ALL),
    1658, uint(UIMainDef::POPUP_TYPE_CORROPTED_ERROR),
    1685, uint(UIMainDef::POPUP_TYPE_JUKEBOX_ERROR),
    1710, uint(UIMainDef::POPUP_TYPE_FILE_NAME_IS_TOO_LONG),
    1743, uint(UIMainDef::POPUP_TYPE_FOLDER_IS_USED),
    1769, uint(UIMainDef::POPUP_TYPE_FILE_IS_USED),
    1793, uint(UIMainDef::POPUP_TYPE_COPY_ALL_CONFIRMATION),
    1826, uint(UIMainDef::POPUP_TYPE_FORMATTING),
    1848, uint(UIMainDef::POPUP_TYPE_FORMAT_COMPLETE),
    1875, uint(UIMainDef::POPUP_TYPE_UNSUPPORTED_FILE),
    1903, uint(UIMainDef::POPUP_TYPE_COPY_TO_JUKEBOX_CONFIRM),
    1938, uint(UIMainDef::POPUP_TYPE_FILE_CANNOT_DELETE),
    1968, uint(UIMainDef::POPUP_TYPE_REPLACE_FRAME_FILE),
    1998, uint(UIMainDef::POPUP_TYPE_DELETE_INCOMPLETE),
    2027, uint(UIMainDef::POPUP_TYPE_CANCEL_COPY_FOR_DELETE_CONFIRM),
    2069, uint(UIMainDef::POPUP_TYPE_CANCEL_COPY_FOR_CLEAR_JUKEBOX_CONFIRM),
    2118, uint(UIMainDef::POPUP_TYPE_DRM_CONFIRM),
    2141, uint(UIMainDef::POPUP_TYPE_DRM_EXPIRED),
    2164, uint(UIMainDef::POPUP_TYPE_DRM_NOT_AUTH),
    2188, uint(UIMainDef::POPUP_TYPE_AUDIO_FORMAT_UNSUPPORTED),
    2224, uint(UIMainDef::POPUP_TYPE_RESOLUTION_UNSUPPORTED),
    2258, uint(UIMainDef::POPUP_TYPE_MAX),
    2273, uint(UIMainDef::BOTTOM_BAR_EVENT_SELECT_AUDIO_FILES),
    2309, uint(UIMainDef::BOTTOM_BAR_EVENT_SELECT_VIDEO_FILES),
    2345, uint(UIMainDef::BOTTOM_BAR_EVENT_SELECT_PICTURE_FILES),
    2383, uint(UIMainDef::BOTTOM_BAR_EVENT_COPY),
    2405, uint(UIMainDef::BOTTOM_BAR_EVENT_DELETE),
    2429, uint(UIMainDef::BOTTOM_BAR_EVENT_COPY_HERE),
    2456, uint(UIMainDef::BOTTOM_BAR_EVENT_CREATE_FOLDER),
    2487, uint(UIMainDef::BOTTOM_BAR_EVENT_CANCEL),
    2511, uint(UIMainDef::BOTTOM_BAR_EVENT_SELECT_ALL),
    2539, uint(UIMainDef::BOTTOM_BAR_EVENT_DESELECT_ALL),
    2569, uint(UIMainDef::BOTTOM_BAR_EVENT_MOVE),
    2591, uint(UIMainDef::BOTTOM_BAR_EVENT_MOVE_ALL),
    2617, uint(UIMainDef::BOTTOM_BAR_EVENT_MOVE_FILE),
    2644, uint(UIMainDef::BOTTOM_BAR_EVENT_MOVE_HERE),
    2671, uint(UIMainDef::BOTTOM_BAR_EVENT_MOVE_CANCEL),
    2700, uint(UIMainDef::BOTTOM_BAR_EVENT_COPY_ALL),
    2726, uint(UIMainDef::BOTTOM_BAR_EVENT_DELETE_ALL),
    2754, uint(UIMainDef::BOTTOM_BAR_EVENT_CANCEL_ON_FIRST_LEVEL),
    2793, uint(UIMainDef::POPUP_EVENT_COPY_CONFIRMATION),
    2823, uint(UIMainDef::POPUP_EVENT_COPY_COMPLETED),
    2850, uint(UIMainDef::POPUP_EVENT_COPY_CANCEL),
    2874, uint(UIMainDef::POPUP_EVENT_COPY_COMPLETE_POPUP_CLOSED),
    2913, uint(UIMainDef::POPUP_EVENT_DELETE_CONFIRMATION),
    2945, uint(UIMainDef::POPUP_EVENT_DELETE_CANCEL),
    2971, uint(UIMainDef::POPUP_EVENT_DELETE_ALL_CANCEL),
    3001, uint(UIMainDef::POPUP_EVENT_DELETE_COMPLETED),
    3030, uint(UIMainDef::POPUP_EVENT_DELETE_COMPLETE_POPUP_CLOSED),
    3071, uint(UIMainDef::POPUP_EVENT_NOUSB_OK),
    3092, uint(UIMainDef::POPUP_EVENT_FILE_OPERATION_RENAME),
    3126, uint(UIMainDef::POPUP_EVENT_FILE_OPERATION_DELETE),
    3160, uint(UIMainDef::POPUP_EVENT_FOLDER_OPERATION_DELETE),
    3196, uint(UIMainDef::POPUP_EVENT_FILE_OPERATION_CANCEL),
    3230, uint(UIMainDef::POPUP_EVENT_SAVE_FRAME_CANCEL),
    3260, uint(UIMainDef::POPUP_EVENT_SAVE_FRAME_JUKEBOX),
    3291, uint(UIMainDef::POPUP_EVENT_EDIT_MODE_OK),
    3316, uint(UIMainDef::POPUP_EVENT_SAVE_FRAME_REQUEST),
    3347, uint(UIMainDef::POPUP_EVENT_CREATE_FOLDER_EMPTY_NAME),
    3384, uint(UIMainDef::POPUP_EVENT_CREATE_FOLDER_NAME_IS_TOO_LONG),
    3427, uint(UIMainDef::POPUP_EVENT_CREATE_FOLDER_INCORRECT_CHARACTER),
    3473, uint(UIMainDef::POPUP_EVENT_CREATE_FOLDER_FOLDER_ALREADY_EXIST),
    3520, uint(UIMainDef::POPUP_EVENT_REPLACE),
    3540, uint(UIMainDef::POPUP_EVENT_REPLACE_ALL),
    3564, uint(UIMainDef::POPUP_EVENT_NO_REPLACE),
    3587, uint(UIMainDef::POPUP_EVENT_REPLACE_CANCEL),
    3614, uint(UIMainDef::POPUP_EVENT_CAPACITY_ERROR_CONFIRM),
    3649, uint(UIMainDef::POPUP_EVENT_JUKEBOX),
    3669, uint(UIMainDef::POPUP_EVENT_COPY_CONTINUE),
    3695, uint(UIMainDef::POPUP_EVENT_COPY_CANCEL_CONFIRM),
    3727, uint(UIMainDef::POPUP_EVENT_COPY_CANCELED_POPUP_CLOSED),
    3766, uint(UIMainDef::POPUP_EVENT_MOVE_ALL_CANCEL),
    3794, uint(UIMainDef::POPUP_EVENT_MOVE_CANCEL),
    3818, uint(UIMainDef::POPUP_EVENT_MOVE_CANCEL_CONFIRM),
    3850, uint(UIMainDef::POPUP_EVENT_MOVE_CANCELED_POPUP_CLOSED),
    3889, uint(UIMainDef::POPUP_EVENT_MOVE_CONTINUE),
    3915, uint(UIMainDef::POPUP_EVENT_MOVE_COMPLETED),
    3942, uint(UIMainDef::POPUP_EVENT_JUKEBOX_CAPACITY_OK),
    3974, uint(UIMainDef::POPUP_EVENT_FORMATTING_JUKEBOX_CONFIRMATION),
    4018, uint(UIMainDef::POPUP_EVENT_FOLDER_OPERATION_RENAME),
    4054, uint(UIMainDef::POPUP_EVENT_SAVE_FRAME_COMPLETED),
    4087, uint(UIMainDef::POPUP_EVENT_COPY_ALL_CONFIRMATION),
    4121, uint(UIMainDef::POPUP_EVENT_COPY_ALL_CANCEL_CONFIRMATION),
    4162, uint(UIMainDef::POPUP_TYPE_COPY_CANCEL_INFO),
    4190, uint(UIMainDef::POPUP_EVENT_COPY_TO_JUKEBOX_CONFIRM),
    4226, uint(UIMainDef::POPUP_EVENT_JUKEBOX_COPY_CANCEL_CONFIRM),
    4266, uint(UIMainDef::POPUP_EVENT_COPY_PROGRESS_CLOSE),
    4298, uint(UIMainDef::POPUP_EVENT_DELETE_PROGRESS_CANCEL),
    4333, uint(UIMainDef::POPUP_EVENT_DELETE_INCOMPLETE),
    4363, uint(UIMainDef::POPUP_EVENT_FRAME_REPLACE_CANCEL),
    4396, uint(UIMainDef::POPUP_EVENT_CANCEL_COPY_FOR_DELETE_CONFIRM),
    4439, uint(UIMainDef::POPUP_EVENT_CANCEL_COPY_ALL_FOR_COPY_CONFIRM),
    4484, uint(UIMainDef::POPUP_EVENT_CLEAR_STATE_TO_NONE),
    4516, uint(UIMainDef::MODE_AREA_EVENT_BACK_PRESSED),
    4545, uint(UIMainDef::MODE_AREA_EVENT_EDIT_PRESSED),
    4574, uint(UIMainDef::KEYPAD_EVENT_TEXT_ENERED),
    4599, uint(UIMainDef::CONTENT_EVENT_FOLDER_OPERATION),
    4630, uint(UIMainDef::CONTENT_EVENT_FILE_OPERATION),
    4659, uint(UIMainDef::CONTENT_EVENT_FILE_BLOCK_DELETE_OPERATION),
    4701, uint(UIMainDef::CONTENT_MODE_UNDEFINED),
    4724, uint(UIMainDef::CONTENT_MODE_LIST),
    4742, uint(UIMainDef::CONTENT_MODE_FILEMANAGER),
    4767, uint(UIMainDef::MUSIC_FILTER_ARTIST),
    4787, uint(UIMainDef::MUSIC_FILTER_ALBUM),
    4806, uint(UIMainDef::MUSIC_FILTER_SONG),
    4824, uint(UIMainDef::FOLDER_MODE_FOLDER),
    4843, uint(UIMainDef::FOLDER_MODE_FILE_PICTURE_USB),
    4872, uint(UIMainDef::FOLDER_MODE_FILE_PICTURE_JUKEBOX),
    4905, uint(UIMainDef::FOLDER_MODE_FILE_VIDEO_USB),
    4932, uint(UIMainDef::FOLDER_MODE_FILE_VIDEO_JUKEBOX),
    4963, uint(UIMainDef::CURRENT_OPERATION_MOVE),
    4986, uint(UIMainDef::CURRENT_OPERATION_COPY),
    5009, uint(UIMainDef::CURRENT_OPERATION_MOVE_CANCEL),
    5039, uint(UIMainDef::CURRENT_OPERATION_COPY_CANCEL),
    5069, uint(UIMainDef::CURRENT_OPERATION_MOVE_ALL),
    5096, uint(UIMainDef::CURRENT_OPERATION_COPY_ALL),
    5123, uint(UIMainDef::CURRENT_OPERATION_DELETE),
    5148, uint(UIMainDef::CURRENT_OPERATION_DELETE_ALL),
    5177, uint(UIMainDef::CURRENT_OPERATION_INVALID),
    5203, uint(UIMainDef::CURRENT_OPERATION_SAVE_FRAME),
    5232, uint(UIMainDef::CURRENT_OPERATION_COPY_REPLACE_CANCEL),
    5270, uint(UIMainDef::CURRENT_OPERATION_COPY_CANCEL_FOR_COPY),
    5309, uint(UIMainDef::CURRENT_OPERATION_FORMAT_JUKEBOX),

       0        // eod
};

static const char qt_meta_stringdata_UIMainDef[] = {
    "UIMainDef\0MODE_AREA_TEXT\0BOTTOM_BAR_MODES\0"
    "DEVICES\0FILE_TYPES\0POPUP_TYPE\0"
    "BOTTOM_BAR_EVENTS\0POPUP_EVENTS\0"
    "MODE_AREA_EVENTS\0KEYPAD_EVENTS\0"
    "CONTENT_EVENTS\0CONTENT_MODES\0MUSIC_FILTERS\0"
    "FOLDER_POPUP_MODES\0CURRENT_OPERATION\0"
    "MODE_AREA_TEXT_DEFAULT\0MODE_AREA_TEXT_COPY_FROM\0"
    "MODE_AREA_TEXT_COPY_TO\0"
    "MODE_AREA_TEXT_ENTER_FOLDER_NAME\0"
    "MODE_AREA_TEXT_RENAME\0MODE_AREA_TEXT_MOVE_FROM\0"
    "MODE_AREA_TEXT_MOVE_TO\0MODE_AREA_TEXT_DELETE\0"
    "MODE_AREA_TEXT_WAITING_DISPLAY\0"
    "MODE_AREA_TEXT_INVALID\0BOTTOM_BAR_INVISIBLE\0"
    "BOTTOM_BAR_COPY_TO\0BOTTOM_BAR_COPY_FROM\0"
    "BOTTOM_BAR_SELECT_DESELECT\0"
    "BOTTOM_BAR_MOVE_TO\0BOTTOM_BAR_DELETE\0"
    "BOTTOM_BAR_MOVE\0BOTTOM_BAR_DELETE_ALL\0"
    "DEVICE_USB\0DEVICE_JUKEBOX\0FILE_TYPE_AUDIO\0"
    "FILE_TYPE_VIDEO\0FILE_TYPE_PICTURE\0"
    "POPUP_TYPE_START\0POPUP_TYPE_COPY\0"
    "POPUP_TYPE_COPYING\0POPUP_TYPE_COPY_COMPLETE\0"
    "POPUP_TYPE_DELETE\0POPUP_TYPE_DELETE_FOLDER\0"
    "POPUP_TYPE_DELETE_ALL\0POPUP_TYPE_DELETING\0"
    "POPUP_TYPE_DELETE_COMPLETE\0POPUP_TYPE_NOUSB\0"
    "POPUP_TYPE_FOLDER_OPERATION\0"
    "POPUP_TYPE_FILE_VIDEO_JUKEBOX_OPERATION\0"
    "POPUP_TYPE_FILE_VIDEO_USB_OPERATION\0"
    "POPUP_TYPE_FILE_PICTURE_JUKEBOX_OPERATION\0"
    "POPUP_TYPE_FILE_PICTURE_USB_OPERATION\0"
    "POPUP_TYPE_SET_AS_FRAME\0POPUP_TYPE_FRAME_SAVED\0"
    "POPUP_TYPE_EDIT_MODE\0"
    "POPUP_TYPE_FOLDER_NAME_IS_TOO_LONG\0"
    "POPUP_TYPE_FOLDER_ALREADY_EXIST\0"
    "POPUP_TYPE_FILE_ALREADY_EXIST\0"
    "POPUP_TYPE_EMPTY_FOLDER_NAME\0"
    "POPUP_TYPE_EMPTY_FILE_NAME\0"
    "POPUP_TYPE_INCORRECT_CHARACTER\0"
    "POPUP_TYPE_REPLACE_FILE\0"
    "POPUP_TYPE_COPY_CANCELLED\0"
    "POPUP_TYPE_CAPACITY_ERROR\0"
    "POPUP_TYPE_COPY_CANCEL\0POPUP_TYPE_MOVE\0"
    "POPUP_TYPE_MOVING\0POPUP_TYPE_MOVE_ALL\0"
    "POPUP_TYPE_MOVE_CANCEL\0POPUP_TYPE_MOVE_CANCELLED\0"
    "POPUP_TYPE_MOVE_COMPLETE\0"
    "POPUP_TYPE_JUKEBOX_CAPACITY\0"
    "POPUP_TYPE_OPTION_MENU_DELETE_ALL\0"
    "POPUP_TYPE_DELETING_ALL\0"
    "POPUP_TYPE_CORROPTED_ERROR\0"
    "POPUP_TYPE_JUKEBOX_ERROR\0"
    "POPUP_TYPE_FILE_NAME_IS_TOO_LONG\0"
    "POPUP_TYPE_FOLDER_IS_USED\0"
    "POPUP_TYPE_FILE_IS_USED\0"
    "POPUP_TYPE_COPY_ALL_CONFIRMATION\0"
    "POPUP_TYPE_FORMATTING\0POPUP_TYPE_FORMAT_COMPLETE\0"
    "POPUP_TYPE_UNSUPPORTED_FILE\0"
    "POPUP_TYPE_COPY_TO_JUKEBOX_CONFIRM\0"
    "POPUP_TYPE_FILE_CANNOT_DELETE\0"
    "POPUP_TYPE_REPLACE_FRAME_FILE\0"
    "POPUP_TYPE_DELETE_INCOMPLETE\0"
    "POPUP_TYPE_CANCEL_COPY_FOR_DELETE_CONFIRM\0"
    "POPUP_TYPE_CANCEL_COPY_FOR_CLEAR_JUKEBOX_CONFIRM\0"
    "POPUP_TYPE_DRM_CONFIRM\0POPUP_TYPE_DRM_EXPIRED\0"
    "POPUP_TYPE_DRM_NOT_AUTH\0"
    "POPUP_TYPE_AUDIO_FORMAT_UNSUPPORTED\0"
    "POPUP_TYPE_RESOLUTION_UNSUPPORTED\0"
    "POPUP_TYPE_MAX\0BOTTOM_BAR_EVENT_SELECT_AUDIO_FILES\0"
    "BOTTOM_BAR_EVENT_SELECT_VIDEO_FILES\0"
    "BOTTOM_BAR_EVENT_SELECT_PICTURE_FILES\0"
    "BOTTOM_BAR_EVENT_COPY\0BOTTOM_BAR_EVENT_DELETE\0"
    "BOTTOM_BAR_EVENT_COPY_HERE\0"
    "BOTTOM_BAR_EVENT_CREATE_FOLDER\0"
    "BOTTOM_BAR_EVENT_CANCEL\0"
    "BOTTOM_BAR_EVENT_SELECT_ALL\0"
    "BOTTOM_BAR_EVENT_DESELECT_ALL\0"
    "BOTTOM_BAR_EVENT_MOVE\0BOTTOM_BAR_EVENT_MOVE_ALL\0"
    "BOTTOM_BAR_EVENT_MOVE_FILE\0"
    "BOTTOM_BAR_EVENT_MOVE_HERE\0"
    "BOTTOM_BAR_EVENT_MOVE_CANCEL\0"
    "BOTTOM_BAR_EVENT_COPY_ALL\0"
    "BOTTOM_BAR_EVENT_DELETE_ALL\0"
    "BOTTOM_BAR_EVENT_CANCEL_ON_FIRST_LEVEL\0"
    "POPUP_EVENT_COPY_CONFIRMATION\0"
    "POPUP_EVENT_COPY_COMPLETED\0"
    "POPUP_EVENT_COPY_CANCEL\0"
    "POPUP_EVENT_COPY_COMPLETE_POPUP_CLOSED\0"
    "POPUP_EVENT_DELETE_CONFIRMATION\0"
    "POPUP_EVENT_DELETE_CANCEL\0"
    "POPUP_EVENT_DELETE_ALL_CANCEL\0"
    "POPUP_EVENT_DELETE_COMPLETED\0"
    "POPUP_EVENT_DELETE_COMPLETE_POPUP_CLOSED\0"
    "POPUP_EVENT_NOUSB_OK\0"
    "POPUP_EVENT_FILE_OPERATION_RENAME\0"
    "POPUP_EVENT_FILE_OPERATION_DELETE\0"
    "POPUP_EVENT_FOLDER_OPERATION_DELETE\0"
    "POPUP_EVENT_FILE_OPERATION_CANCEL\0"
    "POPUP_EVENT_SAVE_FRAME_CANCEL\0"
    "POPUP_EVENT_SAVE_FRAME_JUKEBOX\0"
    "POPUP_EVENT_EDIT_MODE_OK\0"
    "POPUP_EVENT_SAVE_FRAME_REQUEST\0"
    "POPUP_EVENT_CREATE_FOLDER_EMPTY_NAME\0"
    "POPUP_EVENT_CREATE_FOLDER_NAME_IS_TOO_LONG\0"
    "POPUP_EVENT_CREATE_FOLDER_INCORRECT_CHARACTER\0"
    "POPUP_EVENT_CREATE_FOLDER_FOLDER_ALREADY_EXIST\0"
    "POPUP_EVENT_REPLACE\0POPUP_EVENT_REPLACE_ALL\0"
    "POPUP_EVENT_NO_REPLACE\0"
    "POPUP_EVENT_REPLACE_CANCEL\0"
    "POPUP_EVENT_CAPACITY_ERROR_CONFIRM\0"
    "POPUP_EVENT_JUKEBOX\0POPUP_EVENT_COPY_CONTINUE\0"
    "POPUP_EVENT_COPY_CANCEL_CONFIRM\0"
    "POPUP_EVENT_COPY_CANCELED_POPUP_CLOSED\0"
    "POPUP_EVENT_MOVE_ALL_CANCEL\0"
    "POPUP_EVENT_MOVE_CANCEL\0"
    "POPUP_EVENT_MOVE_CANCEL_CONFIRM\0"
    "POPUP_EVENT_MOVE_CANCELED_POPUP_CLOSED\0"
    "POPUP_EVENT_MOVE_CONTINUE\0"
    "POPUP_EVENT_MOVE_COMPLETED\0"
    "POPUP_EVENT_JUKEBOX_CAPACITY_OK\0"
    "POPUP_EVENT_FORMATTING_JUKEBOX_CONFIRMATION\0"
    "POPUP_EVENT_FOLDER_OPERATION_RENAME\0"
    "POPUP_EVENT_SAVE_FRAME_COMPLETED\0"
    "POPUP_EVENT_COPY_ALL_CONFIRMATION\0"
    "POPUP_EVENT_COPY_ALL_CANCEL_CONFIRMATION\0"
    "POPUP_TYPE_COPY_CANCEL_INFO\0"
    "POPUP_EVENT_COPY_TO_JUKEBOX_CONFIRM\0"
    "POPUP_EVENT_JUKEBOX_COPY_CANCEL_CONFIRM\0"
    "POPUP_EVENT_COPY_PROGRESS_CLOSE\0"
    "POPUP_EVENT_DELETE_PROGRESS_CANCEL\0"
    "POPUP_EVENT_DELETE_INCOMPLETE\0"
    "POPUP_EVENT_FRAME_REPLACE_CANCEL\0"
    "POPUP_EVENT_CANCEL_COPY_FOR_DELETE_CONFIRM\0"
    "POPUP_EVENT_CANCEL_COPY_ALL_FOR_COPY_CONFIRM\0"
    "POPUP_EVENT_CLEAR_STATE_TO_NONE\0"
    "MODE_AREA_EVENT_BACK_PRESSED\0"
    "MODE_AREA_EVENT_EDIT_PRESSED\0"
    "KEYPAD_EVENT_TEXT_ENERED\0"
    "CONTENT_EVENT_FOLDER_OPERATION\0"
    "CONTENT_EVENT_FILE_OPERATION\0"
    "CONTENT_EVENT_FILE_BLOCK_DELETE_OPERATION\0"
    "CONTENT_MODE_UNDEFINED\0CONTENT_MODE_LIST\0"
    "CONTENT_MODE_FILEMANAGER\0MUSIC_FILTER_ARTIST\0"
    "MUSIC_FILTER_ALBUM\0MUSIC_FILTER_SONG\0"
    "FOLDER_MODE_FOLDER\0FOLDER_MODE_FILE_PICTURE_USB\0"
    "FOLDER_MODE_FILE_PICTURE_JUKEBOX\0"
    "FOLDER_MODE_FILE_VIDEO_USB\0"
    "FOLDER_MODE_FILE_VIDEO_JUKEBOX\0"
    "CURRENT_OPERATION_MOVE\0CURRENT_OPERATION_COPY\0"
    "CURRENT_OPERATION_MOVE_CANCEL\0"
    "CURRENT_OPERATION_COPY_CANCEL\0"
    "CURRENT_OPERATION_MOVE_ALL\0"
    "CURRENT_OPERATION_COPY_ALL\0"
    "CURRENT_OPERATION_DELETE\0"
    "CURRENT_OPERATION_DELETE_ALL\0"
    "CURRENT_OPERATION_INVALID\0"
    "CURRENT_OPERATION_SAVE_FRAME\0"
    "CURRENT_OPERATION_COPY_REPLACE_CANCEL\0"
    "CURRENT_OPERATION_COPY_CANCEL_FOR_COPY\0"
    "CURRENT_OPERATION_FORMAT_JUKEBOX\0"
};

void UIMainDef::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData UIMainDef::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UIMainDef::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UIMainDef,
      qt_meta_data_UIMainDef, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UIMainDef::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UIMainDef::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UIMainDef::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UIMainDef))
        return static_cast<void*>(const_cast< UIMainDef*>(this));
    return QObject::qt_metacast(_clname);
}

int UIMainDef::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_UI_Main[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      82,   14, // methods
      23,  424, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      24,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,
      32,    8,    8,    8, 0x05,
      55,    8,    8,    8, 0x05,
      80,    8,    8,    8, 0x05,
     101,   96,    8,    8, 0x05,
     116,    8,    8,    8, 0x05,
     136,  128,    8,    8, 0x05,
     155,    8,    8,    8, 0x05,
     186,  180,    8,    8, 0x05,
     210,    8,    8,    8, 0x05,
     233,    8,    8,    8, 0x05,
     260,    8,    8,    8, 0x05,
     273,    8,    8,    8, 0x05,
     286,    8,    8,    8, 0x05,
     307,    8,    8,    8, 0x05,
     328,    8,    8,    8, 0x05,
     348,  180,    8,    8, 0x05,
     363,    8,    8,    8, 0x05,
     389,  180,    8,    8, 0x05,
     421,    8,    8,    8, 0x05,
     443,    8,    8,    8, 0x05,
     465,    8,    8,    8, 0x05,
     479,    8,    8,    8, 0x05,
     502,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
     526,    8,    8,    8, 0x08,
     555,    8,    8,    8, 0x08,
     574,    8,    8,    8, 0x08,
     593,    8,    8,    8, 0x08,
     617,    8,    8,    8, 0x08,
     638,    8,    8,    8, 0x08,
     678,  665,    8,    8, 0x08,
     708,    8,    8,    8, 0x08,
     719,  128,    8,    8, 0x08,
     733,    8,    8,    8, 0x08,
     762,    8,    8,    8, 0x08,
     813,  788,    8,    8, 0x08,
     837,    8,    8,    8, 0x08,
     863,    8,    8,    8, 0x08,
     875,    8,    8,    8, 0x08,
     896,  889,    8,    8, 0x08,
     928,    8,    8,    8, 0x08,
     952,    8,    8,    8, 0x08,
     982,  976,    8,    8, 0x0a,
    1009,  976,    8,    8, 0x0a,
    1032,  976,    8,    8, 0x0a,
    1058,  976,    8,    8, 0x0a,
    1087, 1076,    8,    8, 0x0a,
    1120,  976,    8,    8, 0x0a,
    1145, 1076,    8,    8, 0x0a,
    1179,    8,    8,    8, 0x0a,
    1197,    8,    8,    8, 0x0a,
    1253, 1216,    8,    8, 0x0a,
    1287,  976,    8,    8, 0x0a,
    1344, 1317,    8,    8, 0x0a,
    1399, 1378,    8,    8, 0x0a,
    1437, 1427,    8,    8, 0x0a,
    1461, 1454,    8,    8, 0x0a,
    1504,    8,    8,    8, 0x0a,
    1525,    8,    8,    8, 0x0a,
    1558, 1552,    8,    8, 0x0a,
    1580, 1454,    8,    8, 0x0a,
    1610,    8,    8,    8, 0x0a,
    1640, 1632,    8,    8, 0x0a,
    1705, 1680,    8,    8, 0x0a,
    1738,    8,    8,    8, 0x0a,

 // methods: signature, parameters, type, tag, flags
    1756,    8,    8,    8, 0x02,
    1780,    8, 1772,    8, 0x02,
    1806,    8, 1799,    8, 0x02,
    1827,    8, 1799,    8, 0x02,
    1852,    8, 1848,    8, 0x02,
    1868,    8, 1848,    8, 0x02,
    1883,  180,    8,    8, 0x02,
    1912, 1906,    8,    8, 0x02,
    1938, 1906,    8,    8, 0x02,
    1960, 1906,    8,    8, 0x02,
    1988,    8, 1983,    8, 0x02,
    2012,    8, 1848,    8, 0x02,
    2042, 2038,    8,    8, 0x02,
    2070,    8, 1848,    8, 0x02,
    2094, 2089,    8,    8, 0x02,
    2116,    8, 1848,    8, 0x02,
    2135,  665,    8,    8, 0x02,

 // properties: name, type, flags
    2159, 1848, 0x02495001,
    2173, 1848, 0x02495001,
    2187, 1848, 0x02495001,
    2203, 1983, 0x01495001,
    2214, 1772, 0x0a495001,
    2228, 1772, 0x0a495001,
    2246, 1848, 0x02495001,
    2258, 1848, 0x02495001,
    2271, 1983, 0x01495001,
    2282, 1983, 0x01095003,
    2297, 1983, 0x01095003,
    2312, 1983, 0x01095003,
    2329, 1983, 0x01495001,
    2346, 1983, 0x01495001,
    2365, 1983, 0x01495001,
    2379, 1772, 0x0a095001,
    2394, 1848, 0x02095103,
    2412, 1983, 0x01095003,
    2423, 1772, 0x0a095001,
    2434, 1983, 0x01495001,
    2452, 1772, 0x0a095001,
    2472, 1848, 0x02495001,
    2493, 1848, 0x02495001,

 // properties: notify_signal_id
       0,
       1,
       2,
       8,
       9,
      10,
      13,
      14,
      15,
       0,
       0,
       0,
      17,
      18,
      19,
       0,
       0,
       0,
       0,
      20,
       0,
      22,
      22,

       0        // eod
};

static const char qt_meta_stringdata_UI_Main[] = {
    "UI_Main\0\0bottomBarModeChanged()\0"
    "currentDeviceChanged()\0currentFileTypeChanged()\0"
    "loadBottomBar()\0type\0showPopup(int)\0"
    "hidePopup()\0display\0hidePopupOnFG(int)\0"
    "closeCopyProgressPopup()\0value\0"
    "editButtonChanged(bool)\0modeAreaTitleChanged()\0"
    "modeAreaFileCountChanged()\0showKeyPad()\0"
    "hideKeyPad()\0contentModeChanged()\0"
    "musicFilterChanged()\0browseOnlyChanged()\0"
    "setDevice(int)\0selectAllEnabledChanged()\0"
    "deselectAllEnabledChanged(bool)\0"
    "notifyHideBottomBar()\0notifyIndexingPopup()\0"
    "menuPressed()\0copySkipCountChanged()\0"
    "resetFocusOnEmtyModel()\0"
    "initializeCompletedHandler()\0"
    "hidePopupHandler()\0showPopupHandler()\0"
    "slotShowPopupDeleting()\0slotDeleteComplete()\0"
    "slotShowPopupAllDeleting()\0type,isFront\0"
    "slotEngineShowPopup(int,bool)\0slotOnBg()\0"
    "slotOnFg(int)\0slotOnShowProgressingPopup()\0"
    "slotCancelFileOperation()\0"
    "m_isCopy,unsupportedFile\0"
    "slotShowCopy(bool,bool)\0"
    "slotShowBottomBarOnCopy()\0startCopy()\0"
    "startDelete()\0screen\0"
    "onResetFMLaunchBrowseState(int)\0"
    "onCancelJukeboxFormat()\0showCopyCompletePopup()\0"
    "event\0bottomBarEventHandler(int)\0"
    "popupEventHandler(int)\0modeAreaEventHandler(int)\0"
    "eventHandler(int)\0event,data\0"
    "keyPadEventHandler(int,QVariant)\0"
    "usbjukeEventHandler(int)\0"
    "contentEventHandler(int,QVariant)\0"
    "usbStateHandler()\0startViewHandler()\0"
    "device,fileType,musicFilter,typeMode\0"
    "initStateHandler(int,int,int,int)\0"
    "musicFileterEventHandler(int)\0"
    "fileName,totalFilesForCopy\0"
    "onReplaceFileRequest(QString,int)\0"
    "filesCount,filesSize\0onCapacityError(int,qint64)\0"
    "typeError\0onCopyError(int)\0status\0"
    "onOperationStatusChanged(EOperationStatus)\0"
    "slotUpdateModeArea()\0slotCurrentFolderChanged()\0"
    "nPort\0slotOnUsbRemoved(int)\0"
    "FormattingComleteHandler(int)\0"
    "onCopyOpertaionExit()\0isFront\0"
    "slotcancelFileOperationOnInstance(bool)\0"
    "skipCount,copyFilesCount\0"
    "onCopyOperationComplete(int,int)\0"
    "clearSharedData()\0handleMenuKey()\0"
    "QString\0getProcessedFile()\0double\0"
    "getJukeboxCapacity()\0getJukeboxUsedSize()\0"
    "int\0getFilesCount()\0getFilesSize()\0"
    "setCopyAllStatus(bool)\0count\0"
    "setModeAreaCount(QString)\0"
    "setCopySkipCount(int)\0setCopyTotalCount(int)\0"
    "bool\0IsCurrentVideoPlaying()\0"
    "currentCopyReplaceCount()\0val\0"
    "setCopyFromMainPlayer(bool)\0"
    "RemainedCapacity()\0data\0setSharedMemData(int)\0"
    "getSharedMemData()\0showPopupType(int,bool)\0"
    "bottomBarMode\0currentDevice\0currentFileType\0"
    "editButton\0modeAreaTitle\0modeAreaFileCount\0"
    "contentMode\0musicFileter\0browseOnly\0"
    "musicIsChecked\0videoIsChecked\0"
    "pictureIsChecked\0selectAllEnabled\0"
    "deselectAllEnabled\0hideBottomBar\0"
    "selectedFodler\0currentPopupEvent\0"
    "is_CopyAll\0keyPadText\0showIndexingPopup\0"
    "selectedPictureName\0currentCopySkipCount\0"
    "currentCopyTotalCount\0"
};

void UI_Main::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UI_Main *_t = static_cast<UI_Main *>(_o);
        switch (_id) {
        case 0: _t->bottomBarModeChanged(); break;
        case 1: _t->currentDeviceChanged(); break;
        case 2: _t->currentFileTypeChanged(); break;
        case 3: _t->loadBottomBar(); break;
        case 4: _t->showPopup((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->hidePopup(); break;
        case 6: _t->hidePopupOnFG((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->closeCopyProgressPopup(); break;
        case 8: _t->editButtonChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->modeAreaTitleChanged(); break;
        case 10: _t->modeAreaFileCountChanged(); break;
        case 11: _t->showKeyPad(); break;
        case 12: _t->hideKeyPad(); break;
        case 13: _t->contentModeChanged(); break;
        case 14: _t->musicFilterChanged(); break;
        case 15: _t->browseOnlyChanged(); break;
        case 16: _t->setDevice((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->selectAllEnabledChanged(); break;
        case 18: _t->deselectAllEnabledChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: _t->notifyHideBottomBar(); break;
        case 20: _t->notifyIndexingPopup(); break;
        case 21: _t->menuPressed(); break;
        case 22: _t->copySkipCountChanged(); break;
        case 23: _t->resetFocusOnEmtyModel(); break;
        case 24: _t->initializeCompletedHandler(); break;
        case 25: _t->hidePopupHandler(); break;
        case 26: _t->showPopupHandler(); break;
        case 27: _t->slotShowPopupDeleting(); break;
        case 28: _t->slotDeleteComplete(); break;
        case 29: _t->slotShowPopupAllDeleting(); break;
        case 30: _t->slotEngineShowPopup((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 31: _t->slotOnBg(); break;
        case 32: _t->slotOnFg((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->slotOnShowProgressingPopup(); break;
        case 34: _t->slotCancelFileOperation(); break;
        case 35: _t->slotShowCopy((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 36: _t->slotShowBottomBarOnCopy(); break;
        case 37: _t->startCopy(); break;
        case 38: _t->startDelete(); break;
        case 39: _t->onResetFMLaunchBrowseState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 40: _t->onCancelJukeboxFormat(); break;
        case 41: _t->showCopyCompletePopup(); break;
        case 42: _t->bottomBarEventHandler((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 43: _t->popupEventHandler((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 44: _t->modeAreaEventHandler((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 45: _t->eventHandler((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 46: _t->keyPadEventHandler((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 47: _t->usbjukeEventHandler((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 48: _t->contentEventHandler((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 49: _t->usbStateHandler(); break;
        case 50: _t->startViewHandler(); break;
        case 51: _t->initStateHandler((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 52: _t->musicFileterEventHandler((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 53: _t->onReplaceFileRequest((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 54: _t->onCapacityError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 55: _t->onCopyError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 56: _t->onOperationStatusChanged((*reinterpret_cast< EOperationStatus(*)>(_a[1]))); break;
        case 57: _t->slotUpdateModeArea(); break;
        case 58: _t->slotCurrentFolderChanged(); break;
        case 59: _t->slotOnUsbRemoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 60: _t->FormattingComleteHandler((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 61: _t->onCopyOpertaionExit(); break;
        case 62: _t->slotcancelFileOperationOnInstance((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 63: _t->onCopyOperationComplete((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 64: _t->clearSharedData(); break;
        case 65: _t->handleMenuKey(); break;
        case 66: { QString _r = _t->getProcessedFile();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 67: { double _r = _t->getJukeboxCapacity();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 68: { double _r = _t->getJukeboxUsedSize();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 69: { int _r = _t->getFilesCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 70: { int _r = _t->getFilesSize();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 71: _t->setCopyAllStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 72: _t->setModeAreaCount((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 73: _t->setCopySkipCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 74: _t->setCopyTotalCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 75: { bool _r = _t->IsCurrentVideoPlaying();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 76: { int _r = _t->currentCopyReplaceCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 77: _t->setCopyFromMainPlayer((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 78: { int _r = _t->RemainedCapacity();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 79: _t->setSharedMemData((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 80: { int _r = _t->getSharedMemData();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 81: _t->showPopupType((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UI_Main::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UI_Main::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UI_Main,
      qt_meta_data_UI_Main, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UI_Main::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UI_Main::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UI_Main::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UI_Main))
        return static_cast<void*>(const_cast< UI_Main*>(this));
    return QObject::qt_metacast(_clname);
}

int UI_Main::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 82)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 82;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = getBottomBarMode(); break;
        case 1: *reinterpret_cast< int*>(_v) = getCurrentDevice(); break;
        case 2: *reinterpret_cast< int*>(_v) = getCurrentFileType(); break;
        case 3: *reinterpret_cast< bool*>(_v) = getEditButton(); break;
        case 4: *reinterpret_cast< QString*>(_v) = getModeAreaTitle(); break;
        case 5: *reinterpret_cast< QString*>(_v) = getModeAreaFileCount(); break;
        case 6: *reinterpret_cast< int*>(_v) = getContentMode(); break;
        case 7: *reinterpret_cast< int*>(_v) = getMusicFilter(); break;
        case 8: *reinterpret_cast< bool*>(_v) = getBrowseOnly(); break;
        case 9: *reinterpret_cast< bool*>(_v) = emptyBoolGetter(); break;
        case 10: *reinterpret_cast< bool*>(_v) = emptyBoolGetter(); break;
        case 11: *reinterpret_cast< bool*>(_v) = emptyBoolGetter(); break;
        case 12: *reinterpret_cast< bool*>(_v) = getSelectAllEnabled(); break;
        case 13: *reinterpret_cast< bool*>(_v) = getDeselectAllEnabled(); break;
        case 14: *reinterpret_cast< bool*>(_v) = getHideBottomBar(); break;
        case 15: *reinterpret_cast< QString*>(_v) = getSelectedFolder(); break;
        case 16: *reinterpret_cast< int*>(_v) = getCurrentPopupEvent(); break;
        case 17: *reinterpret_cast< bool*>(_v) = getCopyAllStatus(); break;
        case 18: *reinterpret_cast< QString*>(_v) = getKeyPadText(); break;
        case 19: *reinterpret_cast< bool*>(_v) = getIndexingPopup(); break;
        case 20: *reinterpret_cast< QString*>(_v) = getSelectedPictureName(); break;
        case 21: *reinterpret_cast< int*>(_v) = currentCopySkipCount(); break;
        case 22: *reinterpret_cast< int*>(_v) = currentCopyTotalCount(); break;
        }
        _id -= 23;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 9: setMusicChecked(*reinterpret_cast< bool*>(_v)); break;
        case 10: setVideoChecked(*reinterpret_cast< bool*>(_v)); break;
        case 11: setPictureChecked(*reinterpret_cast< bool*>(_v)); break;
        case 16: setCurrentPopupEvent(*reinterpret_cast< int*>(_v)); break;
        case 17: setCopyAllStatus(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 23;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 23;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 23;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 23;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 23;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 23;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 23;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void UI_Main::bottomBarModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void UI_Main::currentDeviceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void UI_Main::currentFileTypeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void UI_Main::loadBottomBar()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void UI_Main::showPopup(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void UI_Main::hidePopup()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void UI_Main::hidePopupOnFG(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void UI_Main::closeCopyProgressPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void UI_Main::editButtonChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void UI_Main::modeAreaTitleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void UI_Main::modeAreaFileCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}

// SIGNAL 11
void UI_Main::showKeyPad()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void UI_Main::hideKeyPad()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void UI_Main::contentModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void UI_Main::musicFilterChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 14, 0);
}

// SIGNAL 15
void UI_Main::browseOnlyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, 0);
}

// SIGNAL 16
void UI_Main::setDevice(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void UI_Main::selectAllEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 17, 0);
}

// SIGNAL 18
void UI_Main::deselectAllEnabledChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void UI_Main::notifyHideBottomBar()
{
    QMetaObject::activate(this, &staticMetaObject, 19, 0);
}

// SIGNAL 20
void UI_Main::notifyIndexingPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 20, 0);
}

// SIGNAL 21
void UI_Main::menuPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 21, 0);
}

// SIGNAL 22
void UI_Main::copySkipCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 22, 0);
}

// SIGNAL 23
void UI_Main::resetFocusOnEmtyModel()
{
    QMetaObject::activate(this, &staticMetaObject, 23, 0);
}
QT_END_MOC_NAMESPACE
