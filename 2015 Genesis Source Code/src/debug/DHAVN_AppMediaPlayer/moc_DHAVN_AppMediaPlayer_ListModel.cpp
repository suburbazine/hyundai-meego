/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_ListModel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/util/audio/DHAVN_AppMediaPlayer_ListModel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_ListModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LVEnums[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       4,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       8, 0x0,   61,   30,
      27, 0x0,    8,  152,
      44, 0x0,    4,  168,
      65, 0x0,    9,  176,

 // enum data: key, value
      83, uint(LVEnums::POPUP_TYPE_UNDEFINED),
     104, uint(LVEnums::POPUP_TYPE_COPY),
     120, uint(LVEnums::POPUP_TYPE_COPY_ALL),
     140, uint(LVEnums::POPUP_TYPE_COPYING),
     159, uint(LVEnums::POPUP_TYPE_COPY_COMPLETE),
     184, uint(LVEnums::POPUP_TYPE_CANCEL_COPY_QUESTION),
     216, uint(LVEnums::POPUP_TYPE_COPY_CANCELED),
     241, uint(LVEnums::POPUP_TYPE_MOVE),
     257, uint(LVEnums::POPUP_TYPE_MOVE_ALL),
     277, uint(LVEnums::POPUP_TYPE_MOVING),
     295, uint(LVEnums::POPUP_TYPE_MOVE_COMPLETE),
     320, uint(LVEnums::POPUP_TYPE_MOVE_CANCELED),
     345, uint(LVEnums::POPUP_TYPE_DELETE),
     363, uint(LVEnums::POPUP_TYPE_DELETING),
     383, uint(LVEnums::POPUP_TYPE_DELETE_COMPLETE),
     410, uint(LVEnums::POPUP_TYPE_EDIT_MODE),
     431, uint(LVEnums::POPUP_TYPE_PL_FIRST_TIME),
     456, uint(LVEnums::POPUP_TYPE_PL_CREATE_NEW),
     481, uint(LVEnums::POPUP_TYPE_PL_ADD_ALL_FILES),
     509, uint(LVEnums::POPUP_TYPE_PL_CHOOSE_PLAYLIST),
     539, uint(LVEnums::POPUP_TYPE_PL_ADD_COMPLETE),
     566, uint(LVEnums::POPUP_CREATE_FOLDER_INCORRECT_CHARACTER),
     606, uint(LVEnums::POPUP_CREATE_FOLDER_EMPTY_NAME),
     637, uint(LVEnums::POPUP_CREATE_FOLDER_NAME_IS_TOO_LONG),
     674, uint(LVEnums::POPUP_FOLDER_ALREADY_EXIST),
     701, uint(LVEnums::POPUP_FILE_ALREADY_EXIST),
     726, uint(LVEnums::POPUP_TYPE_RENAME),
     744, uint(LVEnums::POPUP_TYPE_REPLACE),
     763, uint(LVEnums::POPUP_TYPE_CAPACITY_ERROR),
     789, uint(LVEnums::POPUP_TYPE_UNAVAILABLE_FILE),
     817, uint(LVEnums::POPUP_TYPE_CORRUPT_FILE),
     841, uint(LVEnums::POPUP_TYPE_JUKEBOX_ERROR),
     866, uint(LVEnums::POPUP_TYPE_CAPACITY_VIEW),
     891, uint(LVEnums::POPUP_TYPE_CAPACITY_ERROR_MANAGE),
     924, uint(LVEnums::POPUP_QUICK_FOLDER_DELETE),
     950, uint(LVEnums::POPUP_QUICK_FILE_DELETE),
     974, uint(LVEnums::POPUP_QUICK_PLAYLIST_DELETE),
    1002, uint(LVEnums::POPUP_QUICK_PLAYLIST_FILE_DELETE),
    1035, uint(LVEnums::POPUP_TYPE_TIP_ADD_PLAYLIST),
    1063, uint(LVEnums::POPUP_TYPE_CLEAR_JUKEBOX),
    1088, uint(LVEnums::POPUP_TYPE_FILE_IS_USED),
    1112, uint(LVEnums::POPUP_TYPE_FOLDER_IS_USED),
    1138, uint(LVEnums::POPUP_TYPE_CANCEL_MOVE_QUESTION),
    1170, uint(LVEnums::POPUP_TYPE_GRACENOTE_INDEXING),
    1200, uint(LVEnums::POPUP_TYPE_IPOD_INDEXING),
    1225, uint(LVEnums::POPUP_TYPE_NO_MUSIC_FILES),
    1251, uint(LVEnums::POPUP_TYPE_LOADING_DATA),
    1275, uint(LVEnums::POPUP_TYPE_FORMATING),
    1296, uint(LVEnums::POPUP_TYPE_FORMAT_COMPLETE),
    1323, uint(LVEnums::POPUP_TYPE_DETAIL_FILE_INFO),
    1351, uint(LVEnums::POPUP_TYPE_IPOD_AVAILABLE_FILES_INFO),
    1388, uint(LVEnums::POPUP_TYPE_MLT_NO_MATCH_FOUND),
    1418, uint(LVEnums::POPUP_TYPE_COPY_CANCEL_INFO),
    1446, uint(LVEnums::POPUP_TYPE_COPY_TO_JUKEBOX_CONFIRM),
    1481, uint(LVEnums::POPUP_TYPE_FILE_CANNOT_DELETE),
    1511, uint(LVEnums::POPUP_TYPE_DELETE_INCOMPLETE),
    1540, uint(LVEnums::POPUP_TYPE_CANCEL_COPY_FOR_DELETE_CONFIRM),
    1582, uint(LVEnums::POPUP_TYPE_CANCEL_COPY_FOR_CLEAR_JUKEBOX),
    1623, uint(LVEnums::POPUP_TYPE_BT_DURING_CALL),
    1649, uint(LVEnums::POPUP_TYPE_PLAY_UNAVAILABLE_FILE),
    1682, uint(LVEnums::POPUP_TYPE_HIGH_TEMPERATURE),
    1710, uint(LVEnums::OPERATION_NONE),
    1725, uint(LVEnums::OPERATION_COPY),
    1740, uint(LVEnums::OPERATION_MOVE),
    1755, uint(LVEnums::OPERATION_RENAME),
    1772, uint(LVEnums::OPERATION_DELETE),
    1789, uint(LVEnums::OPERATION_CLEAR),
    1805, uint(LVEnums::OPERATION_CREATE_FOLDER),
    1829, uint(LVEnums::OPERATION_ADD_TO_PLAYLIST),
    1855, uint(LVEnums::CATEGORY_ALL),
    1868, uint(LVEnums::CATEGORY_ARTIST),
    1884, uint(LVEnums::CATEGORY_SONG),
    1898, uint(LVEnums::CATEGORY_ALBUM),
    1913, uint(LVEnums::FOCUS_NONE),
    1924, uint(LVEnums::FOCUS_STATUS_BAR),
    1941, uint(LVEnums::FOCUS_MODE_AREA),
    1957, uint(LVEnums::FOCUS_CONTENT),
    1971, uint(LVEnums::FOCUS_BOTTOM_MENU),
    1989, uint(LVEnums::FOCUS_POPUP),
    2001, uint(LVEnums::FOCUS_OPTION_MENU),
    2019, uint(LVEnums::FOCUS_PLAYBACK_CONTROL),
    2042, uint(LVEnums::FOCUS_KEYPAD),

       0        // eod
};

static const char qt_meta_stringdata_LVEnums[] = {
    "LVEnums\0POPUP_TYPE_STATE_T\0OPERATION_TYPE_T\0"
    "SEARCH_CATEGORY_ID_T\0FOCUSED_ELEMENT_T\0"
    "POPUP_TYPE_UNDEFINED\0POPUP_TYPE_COPY\0"
    "POPUP_TYPE_COPY_ALL\0POPUP_TYPE_COPYING\0"
    "POPUP_TYPE_COPY_COMPLETE\0"
    "POPUP_TYPE_CANCEL_COPY_QUESTION\0"
    "POPUP_TYPE_COPY_CANCELED\0POPUP_TYPE_MOVE\0"
    "POPUP_TYPE_MOVE_ALL\0POPUP_TYPE_MOVING\0"
    "POPUP_TYPE_MOVE_COMPLETE\0"
    "POPUP_TYPE_MOVE_CANCELED\0POPUP_TYPE_DELETE\0"
    "POPUP_TYPE_DELETING\0POPUP_TYPE_DELETE_COMPLETE\0"
    "POPUP_TYPE_EDIT_MODE\0POPUP_TYPE_PL_FIRST_TIME\0"
    "POPUP_TYPE_PL_CREATE_NEW\0"
    "POPUP_TYPE_PL_ADD_ALL_FILES\0"
    "POPUP_TYPE_PL_CHOOSE_PLAYLIST\0"
    "POPUP_TYPE_PL_ADD_COMPLETE\0"
    "POPUP_CREATE_FOLDER_INCORRECT_CHARACTER\0"
    "POPUP_CREATE_FOLDER_EMPTY_NAME\0"
    "POPUP_CREATE_FOLDER_NAME_IS_TOO_LONG\0"
    "POPUP_FOLDER_ALREADY_EXIST\0"
    "POPUP_FILE_ALREADY_EXIST\0POPUP_TYPE_RENAME\0"
    "POPUP_TYPE_REPLACE\0POPUP_TYPE_CAPACITY_ERROR\0"
    "POPUP_TYPE_UNAVAILABLE_FILE\0"
    "POPUP_TYPE_CORRUPT_FILE\0"
    "POPUP_TYPE_JUKEBOX_ERROR\0"
    "POPUP_TYPE_CAPACITY_VIEW\0"
    "POPUP_TYPE_CAPACITY_ERROR_MANAGE\0"
    "POPUP_QUICK_FOLDER_DELETE\0"
    "POPUP_QUICK_FILE_DELETE\0"
    "POPUP_QUICK_PLAYLIST_DELETE\0"
    "POPUP_QUICK_PLAYLIST_FILE_DELETE\0"
    "POPUP_TYPE_TIP_ADD_PLAYLIST\0"
    "POPUP_TYPE_CLEAR_JUKEBOX\0"
    "POPUP_TYPE_FILE_IS_USED\0"
    "POPUP_TYPE_FOLDER_IS_USED\0"
    "POPUP_TYPE_CANCEL_MOVE_QUESTION\0"
    "POPUP_TYPE_GRACENOTE_INDEXING\0"
    "POPUP_TYPE_IPOD_INDEXING\0"
    "POPUP_TYPE_NO_MUSIC_FILES\0"
    "POPUP_TYPE_LOADING_DATA\0POPUP_TYPE_FORMATING\0"
    "POPUP_TYPE_FORMAT_COMPLETE\0"
    "POPUP_TYPE_DETAIL_FILE_INFO\0"
    "POPUP_TYPE_IPOD_AVAILABLE_FILES_INFO\0"
    "POPUP_TYPE_MLT_NO_MATCH_FOUND\0"
    "POPUP_TYPE_COPY_CANCEL_INFO\0"
    "POPUP_TYPE_COPY_TO_JUKEBOX_CONFIRM\0"
    "POPUP_TYPE_FILE_CANNOT_DELETE\0"
    "POPUP_TYPE_DELETE_INCOMPLETE\0"
    "POPUP_TYPE_CANCEL_COPY_FOR_DELETE_CONFIRM\0"
    "POPUP_TYPE_CANCEL_COPY_FOR_CLEAR_JUKEBOX\0"
    "POPUP_TYPE_BT_DURING_CALL\0"
    "POPUP_TYPE_PLAY_UNAVAILABLE_FILE\0"
    "POPUP_TYPE_HIGH_TEMPERATURE\0OPERATION_NONE\0"
    "OPERATION_COPY\0OPERATION_MOVE\0"
    "OPERATION_RENAME\0OPERATION_DELETE\0"
    "OPERATION_CLEAR\0OPERATION_CREATE_FOLDER\0"
    "OPERATION_ADD_TO_PLAYLIST\0CATEGORY_ALL\0"
    "CATEGORY_ARTIST\0CATEGORY_SONG\0"
    "CATEGORY_ALBUM\0FOCUS_NONE\0FOCUS_STATUS_BAR\0"
    "FOCUS_MODE_AREA\0FOCUS_CONTENT\0"
    "FOCUS_BOTTOM_MENU\0FOCUS_POPUP\0"
    "FOCUS_OPTION_MENU\0FOCUS_PLAYBACK_CONTROL\0"
    "FOCUS_KEYPAD\0"
};

void LVEnums::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData LVEnums::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LVEnums::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LVEnums,
      qt_meta_data_LVEnums, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LVEnums::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LVEnums::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LVEnums::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LVEnums))
        return static_cast<void*>(const_cast< LVEnums*>(this));
    return QObject::qt_metacast(_clname);
}

int LVEnums::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_CListModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
     139,   14, // methods
       9,  709, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      36,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      24,   11,   11,   11, 0x05,
      41,   11,   11,   11, 0x05,
      57,   11,   11,   11, 0x05,
      74,   11,   11,   11, 0x05,
     101,   91,   11,   11, 0x05,
     124,   11,   11,   11, 0x05,
     156,   11,   11,   11, 0x05,
     202,  178,   11,   11, 0x05,
     291,  241,   11,   11, 0x05,
     360,  352,   11,   11, 0x05,
     394,  376,   11,   11, 0x05,
     464,  441,   11,   11, 0x05,
     493,   11,   11,   11, 0x05,
     511,   11,   11,   11, 0x05,
     532,   11,   11,   11, 0x05,
     551,   11,   11,   11, 0x05,
     568,   11,   11,   11, 0x05,
     596,   11,   11,   11, 0x05,
     617,   11,   11,   11, 0x05,
     651,  645,   11,   11, 0x05,
     671,   11,   11,   11, 0x05,
     703,  684,   11,   11, 0x05,
     735,   11,   11,   11, 0x05,
     759,   11,   11,   11, 0x05,
     781,  770,   11,   11, 0x05,
     818,  808,   11,   11, 0x05,
     873,  846,   11,   11, 0x05,
     900,   11,   11,   11, 0x05,
     946,  928,   11,   11, 0x05,
     991,   11,   11,   11, 0x05,
    1009,   11,   11,   11, 0x05,
    1026,   11,   11,   11, 0x05,
    1056, 1048,   11,   11, 0x05,
    1081,   11,   11,   11, 0x05,
    1113, 1100,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
    1160, 1136,   11,   11, 0x0a,
    1253, 1236,   11,   11, 0x2a,
    1355, 1324,   11,   11, 0x0a,
    1462, 1438,   11,   11, 0x2a,
    1540, 1236,   11,   11, 0x2a,
    1649, 1614,   11,   11, 0x0a,
    1797, 1769,   11,   11, 0x2a,
    1935, 1912,   11,   11, 0x0a,
    1967,   11,   11,   11, 0x0a,
    2014, 1989,   11,   11, 0x0a,
    2062, 2043,   11,   11, 0x0a,
    2098, 2088,   11,   11, 0x0a,
    2115,   11,   11,   11, 0x0a,
    2172, 2136,   11,   11, 0x0a,
    2213, 2204,   11,   11, 0x0a,
    2238,   11, 2234,   11, 0x0a,
    2278, 2256,   11,   11, 0x0a,
    2352, 2333, 2325,   11, 0x0a,
    2386, 2333, 2381,   11, 0x0a,
    2440, 2415, 2381,   11, 0x0a,
    2478, 2415, 2381,   11, 0x0a,
    2520, 2513,   11,   11, 0x0a,
    2550,   11,   11,   11, 0x0a,
    2577, 2572,   11,   11, 0x0a,
    2607,   11,   11,   11, 0x0a,
    2629,   11,   11,   11, 0x0a,
    2653,   11,   11,   11, 0x08,
    2686,   11,   11,   11, 0x08,

 // methods: signature, parameters, type, tag, flags
    2710,   11,   11,   11, 0x02,
    2790, 2718,   11,   11, 0x02,
    2883, 2833,   11,   11, 0x22,
    2950, 2921,   11,   11, 0x22,
    3005, 2983, 2234,   11, 0x02,
    3033,   11, 2234,   11, 0x22,
    3057,   11,   11,   11, 0x02,
    3076,   11, 2381,   11, 0x02,
    3092,   11,   11,   11, 0x02,
    3103,   11,   11,   11, 0x02,
    3132, 3118,   11,   11, 0x02,
    3169, 3159,   11,   11, 0x22,
    3210, 3192,   11,   11, 0x02,
    3259, 3246,   11,   11, 0x22,
    3294, 3286,   11,   11, 0x22,
    3322, 3312,   11,   11, 0x02,
    3356, 3346,   11,   11, 0x02,
    3382,   11,   11,   11, 0x02,
    3398,   11, 2381,   11, 0x02,
    3415,   11,   11,   11, 0x02,
    3456, 3440,   11,   11, 0x02,
    3501, 3492,   11,   11, 0x22,
    3529,   11,   11,   11, 0x22,
    3550,   11,   11,   11, 0x02,
    3573,   11,   11,   11, 0x02,
    3607, 3599,   11,   11, 0x02,
    3647, 3628,   11,   11, 0x02,
    3688, 3682,   11,   11, 0x02,
    3705,   11,   11,   11, 0x02,
    3729,   11, 2381,   11, 0x02,
    3748,   11,   11,   11, 0x02,
    3789, 3766,   11,   11, 0x02,
    3827, 3682, 2325,   11, 0x02,
    3839, 3682, 2381,   11, 0x02,
    3857,   11,   11,   11, 0x02,
    3896, 3884,   11,   11, 0x02,
    3931, 3159,   11,   11, 0x02,
    3946,   11, 2381,   11, 0x02,
    3964, 3958,   11,   11, 0x02,
    3981,   11,   11,   11, 0x02,
    4001,   11,   11,   11, 0x02,
    4025,   11, 2234,   11, 0x02,
    4045,   11, 2234,   11, 0x02,
    4065,   11, 2234,   11, 0x02,
    4079,   11, 2234,   11, 0x02,
    4104,   11, 2234,   11, 0x02,
    4128, 4122,   11,   11, 0x02,
    4154, 4149,   11,   11, 0x02,
    4205, 4181, 2325,   11, 0x02,
    4228, 4181, 2325,   11, 0x02,
    4256,   11, 2325,   11, 0x02,
    4272,   11, 2325,   11, 0x02,
    4289, 4181, 2325,   11, 0x02,
    4321, 4317,   11,   11, 0x02,
    4347, 4317,   11,   11, 0x02,
    4375,   11, 2234,   11, 0x02,
    4394,   11,   11,   11, 0x02,
    4416,   11, 2381,   11, 0x02,
    4439, 3958,   11,   11, 0x02,
    4466,   11, 2234,   11, 0x02,
    4485,   11, 2234,   11, 0x02,
    4504,   11, 2234,   11, 0x02,
    4523, 3682, 2381,   11, 0x02,
    4545,   11, 2234,   11, 0x02,
    4571,   11, 2381,   11, 0x02,
    4592,   11, 2381,   11, 0x02,
    4620, 4616,   11,   11, 0x02,
    4648,   11, 2381,   11, 0x02,
    4696, 4686, 4673,   11, 0x02,
    4719, 4686, 4673,   11, 0x02,
    4755, 4748, 2381,   11, 0x02,
    4783, 4776, 2325,   11, 0x02,
    4815,   11, 2381,   11, 0x02,
    4843,   11,   11,   11, 0x02,
    4877, 3682, 2325,   11, 0x02,

 // properties: name, type, flags
    4894, 2381, 0x01095003,
    4908, 2234, 0x02095103,
    4918, 2325, 0x0a095001,
    4931, 2234, 0x02095103,
    4943, 2234, 0x02495103,
    4962, 2234, 0x02095103,
    4981, 2234, 0x02095103,
    5000, 2381, 0x01095003,
    5007, 2381, 0x01095003,

 // properties: notify_signal_id
       0,
       0,
       0,
       0,
      28,
       0,
       0,
       0,
       0,

       0        // eod
};

static const char qt_meta_stringdata_CListModel[] = {
    "CListModel\0\0closeList()\0disableListBtn()\0"
    "enableListBtn()\0finishEditMode()\0"
    "cancelEditMode()\0isVisible\0"
    "showJBoxEmptyStr(bool)\0"
    "setCurrentPlayingItemPosition()\0"
    "resetSelectAllItems()\0type,extParam,extParam2\0"
    "signalShowPopup(int,QVariant,QVariant)\0"
    "folderName,fileName,bitRate,fileFormat,createDate\0"
    "signalFileInfo(QVariant,QVariant,QVariant,QVariant,QVariant)\0"
    "restore\0hidePopup(bool)\0plNames,plSources\0"
    "signalUpdatePopupList(QStringList,QStringList)\0"
    "index,total,percentage\0"
    "progressToPopup(int,int,int)\0"
    "editPopupClosed()\0copyingPopupClosed()\0"
    "copyAllConfirmed()\0copyCancelInfo()\0"
    "contentRequestAllComplete()\0"
    "deleteAllConfirmed()\0restartTimerForListUpdate()\0"
    "title\0showKeyPad(QString)\0hideKeyPad()\0"
    "title,historyStack\0updateTextItemView(QString,int)\0"
    "signalUpdateCountInfo()\0manageJB()\0"
    "alphaIndex\0setSelectedAlphaOnTop(int)\0"
    "quickInfo\0signalQuickScrollInfo(bool)\0"
    "tabId,isVisible,isSelected\0"
    "activateTab(int,bool,bool)\0"
    "currentPlayingItemChanged()\0"
    "source,type,count\0"
    "contentSize(MEDIA_SOURCE_T,MEDIA_TYPE_T,int)\0"
    "resetEnteredStr()\0clearSelection()\0"
    "moveTopHistoryStack()\0updated\0"
    "categoryTabUpdated(bool)\0listItemsUpdated()\0"
    "historyStack\0emptyItemViewList(int)\0"
    "fileList,queryId,reqUID\0"
    "ListRequestComplete(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,"
    "uint)\0"
    "fileList,queryId\0"
    "ListRequestComplete(QVector<QStringList>,ETrackerAbstarctor_Query_Ids)\0"
    "fileList,queryId,offset,reqUID\0"
    "partialRequestComplete(QVector<QStringList>,ETrackerAbstarctor_Query_I"
    "ds,int,uint)\0"
    "fileList,queryId,offset\0"
    "partialRequestComplete(QVector<QStringList>,ETrackerAbstarctor_Query_I"
    "ds,int)\0"
    "partialRequestComplete(QVector<QStringList>,ETrackerAbstarctor_Query_I"
    "ds)\0"
    "fileList,queryId,volumeType,reqUID\0"
    "ListRequestCompleteWithVolumeType(QVector<QStringList>,ETrackerAbstarc"
    "tor_Query_Ids,ETrackerAbstarctor_VolumeType,uint)\0"
    "fileList,queryId,volumeType\0"
    "ListRequestCompleteWithVolumeType(QVector<QStringList>,ETrackerAbstarc"
    "tor_Query_Ids,ETrackerAbstarctor_VolumeType)\0"
    "file,totalFilesForCopy\0"
    "replaceFileRequest(QString,int)\0"
    "onOperationComplete()\0skipCount,copyFilesCount\0"
    "onOperationComplete(int,int)\0"
    "fileCount,fileSize\0capacityError(int,qint64)\0"
    "errorCode\0HandleError(int)\0"
    "startJukeBoxFormat()\0"
    "size,totalFiles,percent,fileDeleted\0"
    "onProgress(int,int,int,QString)\0"
    "fileName\0onFileMoved(QString)\0int\0"
    "setPlaybackIcon()\0album,artist,filename\0"
    "setCurrPlayingItemPos(QString,QString,QString)\0"
    "QString\0itemIndex,itemList\0"
    "getMetaInfo(int,QStringList)\0bool\0"
    "getMetaInfo(int,QList<bool>)\0"
    "itemIndex,itemList,value\0"
    "setMetaInfo(int,QStringList&,QString)\0"
    "setMetaInfo(int,QList<bool>&,bool)\0"
    "status\0FormattingComleteHandler(int)\0"
    "onFileSystemChanged()\0disp\0"
    "onOperationDisplay(DISPLAY_T)\0"
    "onCopyOpertaionExit()\0onOperationIncomplete()\0"
    "onCategoryTabAvailableTimeOver()\0"
    "showCopyCompletePopup()\0clear()\0"
    "tabId,itemIndex,historyStack,isRunFromFileManager,fromCategoryOrBackke"
    "y\0"
    "requestListData(QString,int,int,bool,bool)\0"
    "tabId,itemIndex,historyStack,isRunFromFileManager\0"
    "requestListData(QString,int,int,bool)\0"
    "tabId,itemIndex,historyStack\0"
    "requestListData(QString,int,int)\0"
    "fromCategoryOrBackkey\0requestUpdateListData(bool)\0"
    "requestUpdateListData()\0resetRequestData()\0"
    "showEditPopup()\0tipPopup()\0capacityView()\0"
    "popupType,msg\0popupEventHandler(int,int)\0"
    "popupType\0popupEventHandler(int)\0"
    "message,ext1,ext2\0ErrorMessage(int,QVariant,QVariant)\0"
    "message,ext1\0ErrorMessage(int,QVariant)\0"
    "message\0ErrorMessage(int)\0itemIndex\0"
    "toggleItemCheckbox(int)\0isEnabled\0"
    "enableAllCheckBoxes(bool)\0FileInfoPopUp()\0"
    "isAnyoneMarked()\0resetFileOperationData()\0"
    "destPath,plPath\0startFileOperation(QString,QString)\0"
    "destPath\0startFileOperation(QString)\0"
    "startFileOperation()\0handleSelectAllItems()\0"
    "backupFileOperationData()\0itemIdx\0"
    "onItemLongPress(int)\0enteredStr,curPath\0"
    "handleTextEntered(QString,QString)\0"
    "index\0onQuickEdit(int)\0onNewFolderBtnClicked()\0"
    "isPlaylistsExist()\0updatePopupList()\0"
    "index,operation,plName\0"
    "editPlayListsCatalog(int,int,QString)\0"
    "getUrl(int)\0isSelectable(int)\0"
    "copyPlayingSongToJukebox()\0playlistStr\0"
    "addPlayingTrackToPlaylist(QString)\0"
    "isPopupID(int)\0isCopyAll()\0value\0"
    "setCopyAll(bool)\0clearHistoryStack()\0"
    "resetPartialFetchData()\0getTotalItemCount()\0"
    "GetCountryVariant()\0GetLanguage()\0"
    "getCurrentRequestCount()\0getFileURLCount()\0"
    "count\0setFileURLCount(int)\0flag\0"
    "setIsCreatingUrlList(bool)\0"
    "currentIndex,isSongList\0getCountInfo(int,bool)\0"
    "getCountInfoFirst(int,bool)\0getListStatus()\0"
    "getListFolders()\0getCompareFolders(int,bool)\0"
    "idx\0onComponentCompleted(int)\0"
    "onComponentDestruction(int)\0"
    "RemainedCapacity()\0cancelFileOperation()\0"
    "getCopyCompletedMode()\0"
    "setCopyCompletedMode(bool)\0"
    "getCategoryState()\0getSkipFileCount()\0"
    "getCopyFileCount()\0isCheckBoxMarked(int)\0"
    "currentCopyReplaceCount()\0"
    "isQuickViewVisible()\0getCopyFromMainPlayer()\0"
    "val\0setCopyFromMainPlayer(bool)\0"
    "isCategoryTabAvailable()\0QVariantList\0"
    "sortOrder\0getAlphabeticList(int)\0"
    "getHiddenAlphabeticList(int)\0iIndex\0"
    "IsFileSupported(int)\0letter\0"
    "getListIndexWithLetter(QString)\0"
    "isFileOperationInProgress()\0"
    "RequestListDataOnContentChanged()\0"
    "onItemClick(int)\0isFocusChange\0operation\0"
    "JUKEBOX_ROOT\0displayMode\0currentPlayingItem\0"
    "folderHistoryStack\0currentFocuseIndex\0"
    "moving\0isCopyMode\0"
};

void CListModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CListModel *_t = static_cast<CListModel *>(_o);
        switch (_id) {
        case 0: _t->closeList(); break;
        case 1: _t->disableListBtn(); break;
        case 2: _t->enableListBtn(); break;
        case 3: _t->finishEditMode(); break;
        case 4: _t->cancelEditMode(); break;
        case 5: _t->showJBoxEmptyStr((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setCurrentPlayingItemPosition(); break;
        case 7: _t->resetSelectAllItems(); break;
        case 8: _t->signalShowPopup((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        case 9: _t->signalFileInfo((*reinterpret_cast< QVariant(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3])),(*reinterpret_cast< QVariant(*)>(_a[4])),(*reinterpret_cast< QVariant(*)>(_a[5]))); break;
        case 10: _t->hidePopup((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->signalUpdatePopupList((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 12: _t->progressToPopup((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 13: _t->editPopupClosed(); break;
        case 14: _t->copyingPopupClosed(); break;
        case 15: _t->copyAllConfirmed(); break;
        case 16: _t->copyCancelInfo(); break;
        case 17: _t->contentRequestAllComplete(); break;
        case 18: _t->deleteAllConfirmed(); break;
        case 19: _t->restartTimerForListUpdate(); break;
        case 20: _t->showKeyPad((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: _t->hideKeyPad(); break;
        case 22: _t->updateTextItemView((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 23: _t->signalUpdateCountInfo(); break;
        case 24: _t->manageJB(); break;
        case 25: _t->setSelectedAlphaOnTop((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->signalQuickScrollInfo((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 27: _t->activateTab((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 28: _t->currentPlayingItemChanged(); break;
        case 29: _t->contentSize((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1])),(*reinterpret_cast< MEDIA_TYPE_T(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 30: _t->resetEnteredStr(); break;
        case 31: _t->clearSelection(); break;
        case 32: _t->moveTopHistoryStack(); break;
        case 33: _t->categoryTabUpdated((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 34: _t->listItemsUpdated(); break;
        case 35: _t->emptyItemViewList((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->ListRequestComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        case 37: _t->ListRequestComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2]))); break;
        case 38: _t->partialRequestComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 39: _t->partialRequestComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 40: _t->partialRequestComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2]))); break;
        case 41: _t->ListRequestCompleteWithVolumeType((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 42: _t->ListRequestCompleteWithVolumeType((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[3]))); break;
        case 43: _t->replaceFileRequest((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 44: _t->onOperationComplete(); break;
        case 45: _t->onOperationComplete((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 46: _t->capacityError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 47: _t->HandleError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 48: _t->startJukeBoxFormat(); break;
        case 49: _t->onProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 50: _t->onFileMoved((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 51: { int _r = _t->setPlaybackIcon();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 52: _t->setCurrPlayingItemPos((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 53: { QString _r = _t->getMetaInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 54: { bool _r = _t->getMetaInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QList<bool>(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 55: { bool _r = _t->setMetaInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 56: { bool _r = _t->setMetaInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QList<bool>(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 57: _t->FormattingComleteHandler((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 58: _t->onFileSystemChanged(); break;
        case 59: _t->onOperationDisplay((*reinterpret_cast< DISPLAY_T(*)>(_a[1]))); break;
        case 60: _t->onCopyOpertaionExit(); break;
        case 61: _t->onOperationIncomplete(); break;
        case 62: _t->onCategoryTabAvailableTimeOver(); break;
        case 63: _t->showCopyCompletePopup(); break;
        case 64: _t->clear(); break;
        case 65: _t->requestListData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 66: _t->requestListData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 67: _t->requestListData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 68: { int _r = _t->requestUpdateListData((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 69: { int _r = _t->requestUpdateListData();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 70: _t->resetRequestData(); break;
        case 71: { bool _r = _t->showEditPopup();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 72: _t->tipPopup(); break;
        case 73: _t->capacityView(); break;
        case 74: _t->popupEventHandler((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 75: _t->popupEventHandler((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 76: _t->ErrorMessage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        case 77: _t->ErrorMessage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 78: _t->ErrorMessage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 79: _t->toggleItemCheckbox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 80: _t->enableAllCheckBoxes((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 81: _t->FileInfoPopUp(); break;
        case 82: { bool _r = _t->isAnyoneMarked();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 83: _t->resetFileOperationData(); break;
        case 84: _t->startFileOperation((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 85: _t->startFileOperation((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 86: _t->startFileOperation(); break;
        case 87: _t->handleSelectAllItems(); break;
        case 88: _t->backupFileOperationData(); break;
        case 89: _t->onItemLongPress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 90: _t->handleTextEntered((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 91: _t->onQuickEdit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 92: _t->onNewFolderBtnClicked(); break;
        case 93: { bool _r = _t->isPlaylistsExist();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 94: _t->updatePopupList(); break;
        case 95: _t->editPlayListsCatalog((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 96: { QString _r = _t->getUrl((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 97: { bool _r = _t->isSelectable((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 98: _t->copyPlayingSongToJukebox(); break;
        case 99: _t->addPlayingTrackToPlaylist((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 100: _t->isPopupID((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 101: { bool _r = _t->isCopyAll();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 102: _t->setCopyAll((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 103: _t->clearHistoryStack(); break;
        case 104: _t->resetPartialFetchData(); break;
        case 105: { int _r = _t->getTotalItemCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 106: { int _r = _t->GetCountryVariant();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 107: { int _r = _t->GetLanguage();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 108: { int _r = _t->getCurrentRequestCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 109: { int _r = _t->getFileURLCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 110: _t->setFileURLCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 111: _t->setIsCreatingUrlList((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 112: { QString _r = _t->getCountInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 113: { QString _r = _t->getCountInfoFirst((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 114: { QString _r = _t->getListStatus();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 115: { QString _r = _t->getListFolders();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 116: { QString _r = _t->getCompareFolders((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 117: _t->onComponentCompleted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 118: _t->onComponentDestruction((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 119: { int _r = _t->RemainedCapacity();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 120: _t->cancelFileOperation(); break;
        case 121: { bool _r = _t->getCopyCompletedMode();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 122: _t->setCopyCompletedMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 123: { int _r = _t->getCategoryState();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 124: { int _r = _t->getSkipFileCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 125: { int _r = _t->getCopyFileCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 126: { bool _r = _t->isCheckBoxMarked((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 127: { int _r = _t->currentCopyReplaceCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 128: { bool _r = _t->isQuickViewVisible();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 129: { bool _r = _t->getCopyFromMainPlayer();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 130: _t->setCopyFromMainPlayer((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 131: { bool _r = _t->isCategoryTabAvailable();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 132: { QVariantList _r = _t->getAlphabeticList((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = _r; }  break;
        case 133: { QVariantList _r = _t->getHiddenAlphabeticList((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = _r; }  break;
        case 134: { bool _r = _t->IsFileSupported((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 135: { QString _r = _t->getListIndexWithLetter((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 136: { bool _r = _t->isFileOperationInProgress();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 137: _t->RequestListDataOnContentChanged(); break;
        case 138: { QString _r = _t->onItemClick((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CListModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CListModel::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_CListModel,
      qt_meta_data_CListModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CListModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CListModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CListModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CListModel))
        return static_cast<void*>(const_cast< CListModel*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int CListModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 139)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 139;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = getFocusChange(); break;
        case 1: *reinterpret_cast< int*>(_v) = operation(); break;
        case 2: *reinterpret_cast< QString*>(_v) = jukeboxRoot(); break;
        case 3: *reinterpret_cast< int*>(_v) = getDisplayMode(); break;
        case 4: *reinterpret_cast< int*>(_v) = getCurrentPlayingItem(); break;
        case 5: *reinterpret_cast< int*>(_v) = getFolderHistoryStack(); break;
        case 6: *reinterpret_cast< int*>(_v) = getCurrentFocuseIndex(); break;
        case 7: *reinterpret_cast< bool*>(_v) = moving(); break;
        case 8: *reinterpret_cast< bool*>(_v) = getCopyMode(); break;
        }
        _id -= 9;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setFocusChange(*reinterpret_cast< bool*>(_v)); break;
        case 1: setOperation(*reinterpret_cast< int*>(_v)); break;
        case 3: setDisplayMode(*reinterpret_cast< int*>(_v)); break;
        case 4: setCurrentPlayingItem(*reinterpret_cast< int*>(_v)); break;
        case 5: setFolderHistoryStack(*reinterpret_cast< int*>(_v)); break;
        case 6: setCurrentFocuseIndex(*reinterpret_cast< int*>(_v)); break;
        case 7: movingSetter(*reinterpret_cast< bool*>(_v)); break;
        case 8: setCopyMde(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 9;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 9;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void CListModel::closeList()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CListModel::disableListBtn()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CListModel::enableListBtn()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void CListModel::finishEditMode()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CListModel::cancelEditMode()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void CListModel::showJBoxEmptyStr(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CListModel::setCurrentPlayingItemPosition()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void CListModel::resetSelectAllItems()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void CListModel::signalShowPopup(int _t1, QVariant _t2, QVariant _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CListModel::signalFileInfo(QVariant _t1, QVariant _t2, QVariant _t3, QVariant _t4, QVariant _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CListModel::hidePopup(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CListModel::signalUpdatePopupList(QStringList _t1, QStringList _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CListModel::progressToPopup(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CListModel::editPopupClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void CListModel::copyingPopupClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 14, 0);
}

// SIGNAL 15
void CListModel::copyAllConfirmed()
{
    QMetaObject::activate(this, &staticMetaObject, 15, 0);
}

// SIGNAL 16
void CListModel::copyCancelInfo()
{
    QMetaObject::activate(this, &staticMetaObject, 16, 0);
}

// SIGNAL 17
void CListModel::contentRequestAllComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 17, 0);
}

// SIGNAL 18
void CListModel::deleteAllConfirmed()
{
    QMetaObject::activate(this, &staticMetaObject, 18, 0);
}

// SIGNAL 19
void CListModel::restartTimerForListUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 19, 0);
}

// SIGNAL 20
void CListModel::showKeyPad(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void CListModel::hideKeyPad()
{
    QMetaObject::activate(this, &staticMetaObject, 21, 0);
}

// SIGNAL 22
void CListModel::updateTextItemView(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void CListModel::signalUpdateCountInfo()
{
    QMetaObject::activate(this, &staticMetaObject, 23, 0);
}

// SIGNAL 24
void CListModel::manageJB()
{
    QMetaObject::activate(this, &staticMetaObject, 24, 0);
}

// SIGNAL 25
void CListModel::setSelectedAlphaOnTop(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void CListModel::signalQuickScrollInfo(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void CListModel::activateTab(int _t1, bool _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 27, _a);
}

// SIGNAL 28
void CListModel::currentPlayingItemChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 28, 0);
}

// SIGNAL 29
void CListModel::contentSize(MEDIA_SOURCE_T _t1, MEDIA_TYPE_T _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 29, _a);
}

// SIGNAL 30
void CListModel::resetEnteredStr()
{
    QMetaObject::activate(this, &staticMetaObject, 30, 0);
}

// SIGNAL 31
void CListModel::clearSelection()
{
    QMetaObject::activate(this, &staticMetaObject, 31, 0);
}

// SIGNAL 32
void CListModel::moveTopHistoryStack()
{
    QMetaObject::activate(this, &staticMetaObject, 32, 0);
}

// SIGNAL 33
void CListModel::categoryTabUpdated(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 33, _a);
}

// SIGNAL 34
void CListModel::listItemsUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 34, 0);
}

// SIGNAL 35
void CListModel::emptyItemViewList(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 35, _a);
}
QT_END_MOC_NAMESPACE
