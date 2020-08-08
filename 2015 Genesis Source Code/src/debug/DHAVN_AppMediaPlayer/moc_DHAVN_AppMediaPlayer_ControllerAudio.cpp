/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_ControllerAudio.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/controller/DHAVN_AppMediaPlayer_ControllerAudio.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_ControllerAudio.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MP[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       6,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       3, 0x0,   10,   38,
      12, 0x0,   11,   58,
      33, 0x0,   52,   80,
      53, 0x0,    4,  184,
      71, 0x0,   12,  192,
      90, 0x0,   15,  216,

 // enum data: key, value
     111, uint(MP::JUKEBOX),
     119, uint(MP::USB1),
     124, uint(MP::USB2),
     129, uint(MP::DISC),
     134, uint(MP::IPOD1),
     140, uint(MP::IPOD2),
     146, uint(MP::BLUETOOTH),
     156, uint(MP::AUX),
     160, uint(MP::ALL_DEVICES),
     172, uint(MP::UNDEFINED),
     182, uint(MP::SCANOFF),
     190, uint(MP::SCANFILE),
     199, uint(MP::SCANFOLDER),
     210, uint(MP::RANDOMOFF),
     220, uint(MP::RANDOMFILE),
     231, uint(MP::RANDOMFOLDER),
     244, uint(MP::REPEATOFF),
     254, uint(MP::REPEATFILE),
     265, uint(MP::REPEATFOLDER),
     278, uint(MP::REPEATALL),
     288, uint(MP::SCANALL),
     296, uint(MP::OPTION_MENU_JUKEBOX),
     316, uint(MP::OPTION_MENU_JUKEBOX_LIST),
     341, uint(MP::OPTION_MENU_USB),
     357, uint(MP::OPTION_MENU_USB_LIST),
     378, uint(MP::OPTION_MENU_DISC),
     395, uint(MP::OPTION_MENU_DISC_LIST),
     417, uint(MP::OPTION_MENU_IPOD),
     434, uint(MP::OPTION_MENU_IPOD_LIST),
     456, uint(MP::OPTION_MENU_SHUFFLE_MAIN),
     481, uint(MP::OPTION_MENU_REPEAT_MAIN),
     505, uint(MP::OPTION_MENU_SCAN),
     522, uint(MP::OPTION_MENU_SCAN_MAIN),
     544, uint(MP::OPTION_MENU_SCAN_ALL),
     565, uint(MP::OPTION_MENU_SCAN_FOLDER),
     589, uint(MP::OPTION_MENU_LIST),
     606, uint(MP::OPTION_MENU_MORE_LIKE_THIS),
     633, uint(MP::OPTION_MENU_SEARCH),
     652, uint(MP::OPTION_MENU_ADD_TO_PLAYLIST),
     680, uint(MP::OPTION_MENU_MORE_INFO),
     702, uint(MP::OPTION_MENU_SOUND_SETTING),
     728, uint(MP::OPTION_MENU_REPEAT_ALL),
     751, uint(MP::OPTION_MENU_REPEAT_FOLDER),
     777, uint(MP::OPTION_MENU_REPEAT_ONE),
     800, uint(MP::OPTION_MENU_REPEAT_OFF),
     823, uint(MP::OPTION_MENU_SHUFFLE_ON),
     846, uint(MP::OPTION_MENU_SHUFFLE_OFF),
     870, uint(MP::OPTION_MENU_CLEAR_JUKEBOX),
     896, uint(MP::OPTION_MENU_FILE_INFO),
     918, uint(MP::OPTION_MENU_COPY_TO_JUKEBOX),
     946, uint(MP::OPTION_MENU_NOW_PLAYING),
     970, uint(MP::OPTION_MENU_MOVE),
     987, uint(MP::OPTION_MENU_DELETE),
    1006, uint(MP::OPTION_MENU_CANCEL_OPERATION),
    1035, uint(MP::OPTION_MENU_CAPACITY),
    1056, uint(MP::OPTION_MENU_VIEW),
    1073, uint(MP::OPTION_MENU_SHUFFLE),
    1093, uint(MP::OPTION_MENU_REPEAT),
    1112, uint(MP::OPTION_MENU_EDIT_CATEGORY),
    1138, uint(MP::OPTION_MENU_USB_BASIC),
    1160, uint(MP::OPTION_MENU_JUKEBOX_BASIC),
    1186, uint(MP::OPTION_MENU_BASIC_PLAYER),
    1211, uint(MP::OPTION_MENU_TITLE_MENU),
    1234, uint(MP::OPTION_MENU_DISK_MENU),
    1256, uint(MP::OPTION_MENU_DVD_SETTING),
    1280, uint(MP::OPTION_FULL_SCREEN),
    1299, uint(MP::OPTION_MENU_DISPLAY_SETTING),
    1327, uint(MP::OPTION_MENU_EXIT_MORE_LIKE_THIS),
    1359, uint(MP::OPTION_MENU_MLT),
    1375, uint(MP::OPTION_MENU_CONNECTION_SETTING),
    1406, uint(MP::OPTION_MENU_AUX),
    1422, uint(MP::OPTION_MENU_KEYPAD_SETTING),
    1449, uint(MP::OPTION_MENU_IPOD_MUISC_APP),
    1476, uint(MP::DISC_NONE),
    1486, uint(MP::AUDIO_CD),
    1495, uint(MP::MP3_CD),
    1502, uint(MP::DVD_AUDIO),
    1512, uint(MP::DEFAULT),
    1520, uint(MP::ALBUM),
    1526, uint(MP::FOLDER),
    1533, uint(MP::PLAYLIST),
    1542, uint(MP::SONG),
    1547, uint(MP::GENRE),
    1553, uint(MP::ARTIST),
    1560, uint(MP::PODCAST),
    1568, uint(MP::ITUNES),
    1575, uint(MP::AUDIOBOOK),
    1585, uint(MP::COMPOSER),
    1594, uint(MP::MORELIKETHIS),
    1607, uint(MP::CTRL_DISABLE_PRGRESSBAR),
    1631, uint(MP::CTRL_DISABLE_PLAYQUE),
    1652, uint(MP::CTRL_DISABLE_TUNE),
    1670, uint(MP::CTRL_DISABLE_MENU),
    1688, uint(MP::CTRL_DISABLE_FLOWVIEW),
    1710, uint(MP::CTRL_DISABLE_LIST),
    1728, uint(MP::CTRL_DISABLE_LIST_PLAYLIST),
    1755, uint(MP::CTRL_DISABLE_LIST_ARTIST),
    1780, uint(MP::CTRL_DISABLE_LIST_ALBUM),
    1804, uint(MP::CTRL_DISABLE_LIST_SONG),
    1827, uint(MP::CTRL_DISABLE_LIST_ITUNES),
    1852, uint(MP::CTRL_DISABLE_LIST_PODCAST),
    1878, uint(MP::CTRL_DISABLE_LIST_AUDIOBOOK),
    1906, uint(MP::CTRL_DISABLE_LIST_COMPOSER),
    1933, uint(MP::CTRL_DISABLE_LIST_GENRE),

       0        // eod
};

static const char qt_meta_stringdata_MP[] = {
    "MP\0DEVICE_T\0MODE_REPEAT_RANDOM_T\0"
    "AUDIO_OPTION_MODE_T\0AUDIO_DISC_TYPE_T\0"
    "PLAYBACK_CONTENT_T\0CONTROLLER_DISABLE_T\0"
    "JUKEBOX\0USB1\0USB2\0DISC\0IPOD1\0IPOD2\0"
    "BLUETOOTH\0AUX\0ALL_DEVICES\0UNDEFINED\0"
    "SCANOFF\0SCANFILE\0SCANFOLDER\0RANDOMOFF\0"
    "RANDOMFILE\0RANDOMFOLDER\0REPEATOFF\0"
    "REPEATFILE\0REPEATFOLDER\0REPEATALL\0"
    "SCANALL\0OPTION_MENU_JUKEBOX\0"
    "OPTION_MENU_JUKEBOX_LIST\0OPTION_MENU_USB\0"
    "OPTION_MENU_USB_LIST\0OPTION_MENU_DISC\0"
    "OPTION_MENU_DISC_LIST\0OPTION_MENU_IPOD\0"
    "OPTION_MENU_IPOD_LIST\0OPTION_MENU_SHUFFLE_MAIN\0"
    "OPTION_MENU_REPEAT_MAIN\0OPTION_MENU_SCAN\0"
    "OPTION_MENU_SCAN_MAIN\0OPTION_MENU_SCAN_ALL\0"
    "OPTION_MENU_SCAN_FOLDER\0OPTION_MENU_LIST\0"
    "OPTION_MENU_MORE_LIKE_THIS\0"
    "OPTION_MENU_SEARCH\0OPTION_MENU_ADD_TO_PLAYLIST\0"
    "OPTION_MENU_MORE_INFO\0OPTION_MENU_SOUND_SETTING\0"
    "OPTION_MENU_REPEAT_ALL\0OPTION_MENU_REPEAT_FOLDER\0"
    "OPTION_MENU_REPEAT_ONE\0OPTION_MENU_REPEAT_OFF\0"
    "OPTION_MENU_SHUFFLE_ON\0OPTION_MENU_SHUFFLE_OFF\0"
    "OPTION_MENU_CLEAR_JUKEBOX\0"
    "OPTION_MENU_FILE_INFO\0OPTION_MENU_COPY_TO_JUKEBOX\0"
    "OPTION_MENU_NOW_PLAYING\0OPTION_MENU_MOVE\0"
    "OPTION_MENU_DELETE\0OPTION_MENU_CANCEL_OPERATION\0"
    "OPTION_MENU_CAPACITY\0OPTION_MENU_VIEW\0"
    "OPTION_MENU_SHUFFLE\0OPTION_MENU_REPEAT\0"
    "OPTION_MENU_EDIT_CATEGORY\0"
    "OPTION_MENU_USB_BASIC\0OPTION_MENU_JUKEBOX_BASIC\0"
    "OPTION_MENU_BASIC_PLAYER\0"
    "OPTION_MENU_TITLE_MENU\0OPTION_MENU_DISK_MENU\0"
    "OPTION_MENU_DVD_SETTING\0OPTION_FULL_SCREEN\0"
    "OPTION_MENU_DISPLAY_SETTING\0"
    "OPTION_MENU_EXIT_MORE_LIKE_THIS\0"
    "OPTION_MENU_MLT\0OPTION_MENU_CONNECTION_SETTING\0"
    "OPTION_MENU_AUX\0OPTION_MENU_KEYPAD_SETTING\0"
    "OPTION_MENU_IPOD_MUISC_APP\0DISC_NONE\0"
    "AUDIO_CD\0MP3_CD\0DVD_AUDIO\0DEFAULT\0"
    "ALBUM\0FOLDER\0PLAYLIST\0SONG\0GENRE\0"
    "ARTIST\0PODCAST\0ITUNES\0AUDIOBOOK\0"
    "COMPOSER\0MORELIKETHIS\0CTRL_DISABLE_PRGRESSBAR\0"
    "CTRL_DISABLE_PLAYQUE\0CTRL_DISABLE_TUNE\0"
    "CTRL_DISABLE_MENU\0CTRL_DISABLE_FLOWVIEW\0"
    "CTRL_DISABLE_LIST\0CTRL_DISABLE_LIST_PLAYLIST\0"
    "CTRL_DISABLE_LIST_ARTIST\0"
    "CTRL_DISABLE_LIST_ALBUM\0CTRL_DISABLE_LIST_SONG\0"
    "CTRL_DISABLE_LIST_ITUNES\0"
    "CTRL_DISABLE_LIST_PODCAST\0"
    "CTRL_DISABLE_LIST_AUDIOBOOK\0"
    "CTRL_DISABLE_LIST_COMPOSER\0"
    "CTRL_DISABLE_LIST_GENRE\0"
};

void MP::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MP::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MP::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MP,
      qt_meta_data_MP, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MP::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MP::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MP::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MP))
        return static_cast<void*>(const_cast< MP*>(this));
    return QObject::qt_metacast(_clname);
}

int MP::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_AppMediaPlayer_Controller_Audio[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
     223,   14, // methods
      34, 1129, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      90,       // signalCount

 // signals: signature, parameters, type, tag, flags
      43,   33,   32,   32, 0x05,
      64,   32,   32,   32, 0x05,
      97,   87,   32,   32, 0x05,
     147,  120,   32,   32, 0x05,
     174,   32,   32,   32, 0x05,
     193,   32,   32,   32, 0x05,
     211,   32,   32,   32, 0x05,
     274,  230,   32,   32, 0x05,
     306,  230,   32,   32, 0x05,
     334,   32,   32,   32, 0x05,
     352,   32,   32,   32, 0x05,
     372,   32,   32,   32, 0x05,
     390,   32,   32,   32, 0x05,
     412,   32,   32,   32, 0x05,
     429,   32,   32,   32, 0x05,
     445,   32,   32,   32, 0x05,
     472,   32,   32,   32, 0x05,
     498,   32,   32,   32, 0x05,
     523,   32,   32,   32, 0x05,
     547,   32,   32,   32, 0x05,
     570,   32,   32,   32, 0x05,
     589,  587,   32,   32, 0x05,
     628,   32,   32,   32, 0x05,
     643,  636,   32,   32, 0x05,
     684,  662,   32,   32, 0x05,
     714,   32,   32,   32, 0x05,
     731,   32,   32,   32, 0x05,
     754,  749,   32,   32, 0x05,
     772,   32,   32,   32, 0x05,
     784,   32,   32,   32, 0x05,
     814,  795,   32,   32, 0x05,
     871,  865,   32,   32, 0x05,
     917,  897,   32,   32, 0x05,
     975,   32,   32,   32, 0x05,
    1010,  994,   32,   32, 0x05,
    1031,   32,   32,   32, 0x05,
    1052,   32,   32,   32, 0x05,
    1074,   32,   32,   32, 0x05,
    1101,   32,   32,   32, 0x05,
    1143, 1129,   32,   32, 0x05,
    1167,   32,   32,   32, 0x05,
    1190,   32,   32,   32, 0x05,
    1207,   32,   32,   32, 0x05,
    1263, 1230,   32,   32, 0x05,
    1330, 1310,   32,   32, 0x05,
    1364, 1358,   32,   32, 0x05,
    1395, 1389,   32,   32, 0x05,
    1430, 1418,   32,   32, 0x05,
    1472, 1457,   32,   32, 0x05,
    1530, 1526,   32,   32, 0x05,
    1551,   32,   32,   32, 0x05,
    1581, 1571,   32,   32, 0x05,
    1616, 1605,   32,   32, 0x05,
    1645, 1642,   32,   32, 0x05,
    1688,   32,   32,   32, 0x05,
    1709,   32,   32,   32, 0x05,
    1730,   32,   32,   32, 0x05,
    1760,   32,   32,   32, 0x05,
    1805, 1776,   32,   32, 0x05,
    1836,   32,   32,   32, 0x05,
    1857,   32,   32,   32, 0x05,
    1877,   32,   32,   32, 0x05,
    1900, 1894,   32,   32, 0x05,
    1921, 1358,   32,   32, 0x05,
    1975, 1942,   32,   32, 0x05,
    2035, 2026,   32,   32, 0x05,
    2066, 2057,   32,   32, 0x05,
    2114, 2087,   32,   32, 0x05,
    2155, 2144,   32,   32, 0x05,
    2186, 2180,   32,   32, 0x05,
    2211,   32,   32,   32, 0x05,
    2229, 2087,   32,   32, 0x05,
    2269,   32,   32,   32, 0x05,
    2292, 2287,   32,   32, 0x05,
    2320,   32,   32,   32, 0x05,
    2342,   32,   32,   32, 0x05,
    2382, 2361,   32,   32, 0x05,
    2419,   32,   32,   32, 0x05,
    2442,   32,   32,   32, 0x05,
    2515, 2459,   32,   32, 0x05,
    2576,   32,   32,   32, 0x05,
    2598,   32,   32,   32, 0x05,
    2624,   32,   32,   32, 0x05,
    2655, 2644,   32,   32, 0x05,
    2692, 2672,   32,   32, 0x05,
    2716,   32,   32,   32, 0x05,
    2743, 2728,   32,   32, 0x05,
    2789,   32,   32,   32, 0x05,
    2811,   32,   32,   32, 0x05,
    2835,   32,   32,   32, 0x05,

 // slots: signature, parameters, type, tag, flags
    2861, 2849,   32,   32, 0x0a,
    2891, 2849,   32,   32, 0x0a,
    2936, 2917,   32,   32, 0x0a,
    2968, 2961,   32,   32, 0x0a,
    2990, 2961,   32,   32, 0x0a,
    3012, 2961,   32,   32, 0x0a,
    3030, 2961,   32,   32, 0x0a,
    3068, 3052,   32,   32, 0x0a,
    3107, 3102,   32,   32, 0x0a,
    3133, 3102,   32,   32, 0x0a,
    3159, 2961,   32,   32, 0x0a,
    3195, 3184,   32,   32, 0x0a,
    3253, 3229,   32,   32, 0x0a,
    3362, 3331,   32,   32, 0x0a,
    3448, 3444,   32,   32, 0x0a,
    3570,   32,   32,   32, 0x0a,
    3618, 3585,   32,   32, 0x0a,
    3657, 3649,   32,   32, 0x0a,
    3695,   32,   32,   32, 0x0a,
    3721, 3709,   32,   32, 0x0a,
    3763, 3755,   32,   32, 0x0a,
    3798,   32,   32,   32, 0x0a,
    3837, 3825,   32,   32, 0x0a,
    3880,   32,   32,   32, 0x0a,
    3918, 3909,   32,   32, 0x0a,
    3941, 3909,   32,   32, 0x0a,
    3964, 1389,   32,   32, 0x0a,
    3984, 1389,   32,   32, 0x0a,
    4008, 1389,   32,   32, 0x0a,
    4032, 1894,   32,   32, 0x0a,
    4082, 4063,   32,   32, 0x0a,
    4120, 1894,   32,   32, 0x2a,
    4153,  865,   32,   32, 0x0a,
    4173,   32,   32,   32, 0x0a,
    4193, 4188,   32,   32, 0x0a,
    4221, 4188,   32,   32, 0x0a,
    4255,   32,   32,   32, 0x0a,
    4274,   32,   32,   32, 0x0a,
    4336, 4295,   32,   32, 0x0a,
    4397, 4370,   32,   32, 0x0a,
    4443, 4432, 4427,   32, 0x0a,
    4466, 1389,   32,   32, 0x0a,
    4500, 4490,   32,   32, 0x0a,
    4544,   32,   32,   32, 0x0a,
    4570, 4564,   32,   32, 0x0a,
    4596,   32,   32,   32, 0x0a,
    4621,   32,   32,   32, 0x0a,
    4652,   32,   32,   32, 0x0a,
    4683,   32,   32,   32, 0x0a,
    4704,   32,   32,   32, 0x0a,
    4731,   32,   32,   32, 0x0a,
    4755,   32,   32,   32, 0x0a,
    4788,   32,   32,   32, 0x0a,
    4818,   32,   32,   32, 0x0a,
    4849,   32,   32,   32, 0x0a,
    4883,   32,   32,   32, 0x0a,
    4914,   32,   32,   32, 0x0a,
    4946,   32,   32,   32, 0x0a,
    4972,   32,   32,   32, 0x0a,
    5002,   32,   32,   32, 0x0a,
    5032,   32,   32,   32, 0x0a,
    5097, 5058,   32,   32, 0x0a,
    5171, 5164, 5156,   32, 0x0a,
    5216, 5205,   32,   32, 0x0a,
    5231,   32,   32,   32, 0x0a,
    5251,   32,   32,   32, 0x0a,
    5281, 5270,   32,   32, 0x0a,
    5305,   32,   32,   32, 0x0a,
    5329,   32,   32,   32, 0x0a,
    5350,   32,   32,   32, 0x0a,
    5377, 2728,   32,   32, 0x0a,
    5417,   32, 4427,   32, 0x0a,
    5435, 1358,   32,   32, 0x08,
    5467, 1358,   32,   32, 0x08,
    5489, 2728,   32,   32, 0x08,
    5551, 5537,   32,   32, 0x08,

 // methods: signature, parameters, type, tag, flags
    5600, 5586,   32,   32, 0x02,
    5628,   32,   32,   32, 0x02,
    5669, 5645,   32,   32, 0x02,
    5727, 5701,   32,   32, 0x02,
    5765, 5756,   32,   32, 0x02,
    5791, 5782,   32,   32, 0x02,
    5822, 5813, 5809,   32, 0x02,
    5846,   32,   32,   32, 0x02,
    5865,   32,   32,   32, 0x02,
    5884,   32, 5809,   32, 0x02,
    5900,   32, 5809,   32, 0x02,
    5916,   32, 5809,   32, 0x02,
    5937, 5930, 5809,   32, 0x02,
    5978, 5959,   32,   32, 0x02,
    6019, 6003, 5156,   32, 0x02,
    6049, 6043,   32,   32, 0x02,
    6086, 6077, 4427,   32, 0x02,
    6121, 6112, 4427,   32, 0x02,
    6147,   32, 4427,   32, 0x02,
    6165,   32,   32,   32, 0x02,
    6198, 6183,   32,   32, 0x02,
    6228, 6221,   32,   32, 0x02,
    6251,   32,   32,   32, 0x02,
    6281, 6267, 4427,   32, 0x02,
    6316, 6311, 4427,   32, 0x22,
    6370, 6341,   32,   32, 0x02,
    6417, 6410,   32,   32, 0x02,
    6438, 5930, 5809,   32, 0x02,
    6467,   32, 4427,   32, 0x02,
    6483,   32, 4427,   32, 0x02,
    6501,   32, 4427,   32, 0x02,
    6514,   32, 4427,   32, 0x02,
    6536,   32, 4427,   32, 0x02,
    6548,   32, 4427,   32, 0x02,
    6566,   32,   32,   32, 0x02,
    6580,   32, 4427,   32, 0x02,
    6603,   32, 4427,   32, 0x02,
    6625,   32, 4427,   32, 0x02,
    6647,   32, 4427,   32, 0x02,
    6674, 6666, 4427,   32, 0x02,
    6721, 6696,   32,   32, 0x02,
    6773,   32,   32,   32, 0x02,
    6791,   32,   32,   32, 0x02,
    6803,   32,   32,   32, 0x02,
    6813,   32,   32,   32, 0x02,
    6840, 6828,   32,   32, 0x02,
    6888, 6871,   32,   32, 0x02,
    6927, 6871,   32,   32, 0x02,
    6989, 6967,   32,   32, 0x02,
    7050, 7038,   32,   32, 0x02,
    7079,   32, 5809,   32, 0x02,
    7121, 7104, 4427,   32, 0x02,
    7138,   32, 4427,   32, 0x22,
    7160, 7151, 4427,   32, 0x02,
    7185,   32, 4427,   32, 0x02,
    7210, 7206,   32,   32, 0x02,
    7235, 7151,   32,   32, 0x02,

 // properties: name, type, flags
    7263, 4427, 0x01495103,
    7284, 5809, 0x02495103,
    7295, 4427, 0x01095003,
    7311, 5809, 0x02095003,
    7327, 7322, 0x11095103,
    7334, 5156, 0x0a095103,
    7340, 5156, 0x0a095001,
    7345, 5156, 0x0a095001,
    7151, 5156, 0x0a095001,
    2026, 5156, 0x0a095001,
    7352, 5809, 0x02495001,
    7361, 4427, 0x01095103,
    7370, 4427, 0x01495103,
    7384, 4427, 0x01095103,
    7398, 4427, 0x01095001,
    7422, 7410, 0x0b095001,
    7432, 5809, 0x02095001,
    7442, 5809, 0x02095001,
    7455, 5809, 0x02095001,
    7466, 5809, 0x02095103,
    7479, 5809, 0x02095103,
    7491, 4427, 0x01095103,
    7510, 4427, 0x01495003,
    7522, 4427, 0x01095103,
    7538, 4427, 0x01095103,
    7558, 4427, 0x01095103,
    7578, 5156, 0x0a495001,
    7590, 5156, 0x0a495001,
    7602, 4427, 0x01095001,
    7621, 5809, 0x02495001,
    7651, 7638, 0x09095003,
    7664, 4427, 0x01095001,
    7691, 4427, 0x01495003,
    7704, 4427, 0x01095003,

 // properties: notify_signal_id
      56,
      74,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
      60,
       0,
      77,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
      59,
       0,
       0,
       0,
      54,
      55,
       0,
      81,
       0,
       0,
      80,
       0,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Controller_Audio[] = {
    "AppMediaPlayer_Controller_Audio\0\0"
    "isDisable\0disableHardKey(bool)\0"
    "tuneSameFileNoti(bool)\0isVisible\0"
    "showPathView(QVariant)\0"
    "tabId,isVisible,isSelected\0"
    "activateTab(int,bool,bool)\0"
    "serch_TabClicked()\0updateUIForDisc()\0"
    "closePopUpLoader()\0"
    "TitleNum,ChapterNum,totalChapter,totalTitle\0"
    "dvdInfochanged(int,int,int,int)\0"
    "getdvdInfo(int,int,int,int)\0"
    "playbackStopped()\0resetCurrentIndex()\0"
    "playbackStarted()\0playbackForcedStart()\0"
    "playbackPaused()\0playbackTuned()\0"
    "bluetoothPlaybackStarted()\0"
    "bluetoothPlaybackPaused()\0"
    "playListReadyAutoStart()\0"
    "playListStartSameSong()\0CommandOnVideoDec(int)\0"
    "CommandOnBT(int)\0,\0"
    "FastForwardComanndOnVideoDec(bool,int)\0"
    "Error()\0volume\0VolumeChanged(int)\0"
    "repeatIcon,randomIcon\0"
    "setRandomRepeatIcons(int,int)\0"
    "resetSpeedIcon()\0resetPlayIconBT()\0"
    "rate\0setSpeedIcon(int)\0startScan()\0"
    "stopScan()\0state,index,aValue\0"
    "setStatusDataByIndex(MP::DEVICE_T,DATA_TYPE_T,int)\0"
    "state\0setAppState(MP::DEVICE_T)\0"
    "names,artists,files\0"
    "sendMoreLikeThisList(QStringList,QStringList,QStringList)\0"
    "showNoFoundPopup()\0bUnsupportedAll\0"
    "showPopupError(bool)\0sigShowBTCallPopup()\0"
    "sigCloseBTCallPopup()\0sigShowDeckHighTempPopup()\0"
    "sigCloseDeckHighTempPopup()\0categoryIndex\0"
    "changeIPodCategory(int)\0showPopupLoadingData()\0"
    "allTagReceived()\0showIPodRemovedPopup()\0"
    "trackName,trackAlbum,trackArtist\0"
    "updateCDTrackMetadata(QString,QString,QString)\0"
    "coverIndex,isScroll\0setHighlightCover(int,bool)\0"
    "count\0clearCoverflowIndex(int)\0index\0"
    "setCoverflowIndex(int)\0isTuneColor\0"
    "tuneTextColorChanged(bool)\0volume,id,path\0"
    "signalGracenoteHighlightedVolume(int,QString,QString)\0"
    "toc\0tocReceived(QString)\0signalCloseSearch()\0"
    "bFinished\0mediaSyncfinished(bool)\0"
    "folderName\0updateFolderName(QString)\0"
    ",,\0deviceMinerFinished(bool,int,MP::DEVICE_T)\0"
    "trackNumberChanged()\0groupNumberChanged()\0"
    "isRunFromFileManagerChanged()\0"
    "clearSeekMode()\0MasterTableCount,DeviceCount\0"
    "iPodPlayableSongCount(int,int)\0"
    "isBasicViewChanged()\0isDiscTypeChanged()\0"
    "enableModeArea()\0track\0updateTrackInfo(int)\0"
    "tuneCounter(QString)\0"
    "artist,song,album,folder,isColor\0"
    "tuneMetaData(QString,QString,QString,QString,bool)\0"
    "coverart\0tuneCoverart(QString)\0bFetched\0"
    "dirTreeFetched(bool)\0portNum,deviceName,connect\0"
    "iPodPlaying(int,QString,bool)\0speedCheck\0"
    "UpdateStateTrackOSD(int)\0bShow\0"
    "showInhibitionIcon(bool)\0signalNoContent()\0"
    "iPodDeviceConnectInfo(int,QString,bool)\0"
    "releaseResource()\0disp\0"
    "operationDisplay(DISPLAY_T)\0"
    "isPlayerModeChanged()\0updateSongsCount()\0"
    "isFFRew,status,arrow\0"
    "changeFastFowardRewind(bool,int,int)\0"
    "isPlayFromMLTChanged()\0showPlayerView()\0"
    "m_coverart,m_artist,m_song,m_album,m_folder,m_countInfo\0"
    "mp3fileInfo(QString,QString,QString,QString,QString,QString)\0"
    "isForegroundChanged()\0iPodSortingOrderChanged()\0"
    "clearRepeatRandom()\0discStatus\0"
    "sendPositon(int)\0playstatus,trackNum\0"
    "sendPlayStatus(int,int)\0wrongDisc()\0"
    "enable,AppName\0"
    "basicControlEnableStatusChanged(bool,QString)\0"
    "hideiPod3rdPartyTxt()\0restoreIndexCountText()\0"
    "clearAllJog()\0mediaStatus\0"
    "HandleMediaStatusChanged(int)\0"
    "onMediaStatusChanged(int)\0errorCode,filename\0"
    "HandleError(int,QString)\0status\0"
    "ipod1Authorised(bool)\0ipod2Authorised(bool)\0"
    "onIPodReady(bool)\0onAutoPlayReady(bool)\0"
    "deviceId,volume\0ipodDisconnectedSlot(QString,int)\0"
    "name\0onGetDeviceName1(QString)\0"
    "onGetDeviceName2(QString)\0"
    "onSystemStateStatus(int)\0nMessageId\0"
    "displayiPodTaggingOSD(EIPodPopUp)\0"
    "fileList,queryId,reqUID\0"
    "HandleRequestComplete(QVector<QStringList>,ETrackerAbstarctor_Query_Id"
    "s,uint)\0"
    "dataList,queryId,offset,reqUID\0"
    "HandleRequestComplete(QVector<QStringList>,ETrackerAbstarctor_Query_Id"
    "s,int,uint)\0"
    ",,,\0"
    "HandleRequestCompleteWithVolumeType(QVector<QStringList>,ETrackerAbsta"
    "rctor_Query_Ids,ETrackerAbstarctor_VolumeType,uint)\0"
    "ScanComplete()\0nTitleNo,nChapterNo,nChapterSize\0"
    "OnSearchCompleted(int,int,int)\0results\0"
    "PlaylistDataReceived(PlaylistResults)\0"
    "onCDEjected()\0nSize,pData\0"
    "onGraceNoteTOCReceived(int,void*)\0"
    "dirTree\0parseReceivedDirTree(DvdcDirTree*)\0"
    "requestCurrentFolderName()\0pFolderName\0"
    "receivedCurrentFolderName(DvdcFolderName*)\0"
    "updateCurrentMP3FolderName()\0millisec\0"
    "onPositionChanged(int)\0onDurationChanged(int)\0"
    "onTrackChanged(int)\0onDeckTrackChanged(int)\0"
    "onDeckTrackUpdated(int)\0"
    "onMediaInfoChanged(MediaInfo*)\0"
    "track,bEmitEnabled\0"
    "onMediaInfoChangedEx(MediaInfo*,bool)\0"
    "onMediaInfoChangedEx(MediaInfo*)\0"
    "onStateChanged(int)\0onNormalPlay()\0"
    "pTag\0onTagReceived(DvdcTagInfo*)\0"
    "onAllTagReceived(DvdcAllTagInfo*)\0"
    "onModeChanged(int)\0onRealSoundChanged()\0"
    "bPrev,nTotalIdx,nIdxInFolder,nFolderSize\0"
    "OnTrackBufferred(int,int,int,int)\0"
    "isFinished,onMinerFinished\0"
    "onMinerFinished(bool,QString)\0bool\0"
    "percentage\0onMinerInProgress(int)\0"
    "onAutoTrackChanged(int)\0resources\0"
    "onReleaseResources(MEDIAPLAYER_RESOURCES_T)\0"
    "onResourceGranted()\0total\0"
    "onIPodTotalNumTracks(int)\0"
    "onTuneMetaInfoTimeOver()\0"
    "onRequestCoverArtForListView()\0"
    "onRequestCoverArtForFlowView()\0"
    "onRequestFirstSong()\0onBtResponseTimerExpired()\0"
    "onBtResponseTimerStop()\0"
    "onBtDelayedGetInfoTimerExpired()\0"
    "onBtDelayedGetInfoTimerStop()\0"
    "onBtDelayedGetInfoTimerStart()\0"
    "onBtPeriodicGetInfoTimerExpired()\0"
    "onBtPeriodicGetInfoTimerStop()\0"
    "onBtPeriodicGetInfoTimerStart()\0"
    "onBtRequestTimerExpired()\0"
    "onBtDelayedPlayTimerExpired()\0"
    "onBtDelayedMuteTimerExpired()\0"
    "oniPodMountTimerExpired()\0"
    "strNodePath,strSerial,ePortNum,isFirst\0"
    "onIPodDeviceConnectedSlot(QString,QString,EIPodPorts,bool)\0"
    "QString\0source\0getIPODDeviceName(MEDIA_SOURCE_T)\0"
    "_pauseplay\0onBTFunc(bool)\0onDiscloadRequest()\0"
    "onAudioSinkError()\0trackIndex\0"
    "onUserTrackChanged(int)\0onLongBtnTimerExpired()\0"
    "onScanTimerExpired()\0onNotifyPlayStateForSiri()\0"
    "onLaunchedDefaultMusicApp(bool,QString)\0"
    "getIsForeground()\0onCurrentTrackCountChanged(int)\0"
    "onDatabaseUpdate(int)\0"
    "onBasicControlEnableStatusChanged(bool,QString)\0"
    "current,total\0UpdateClusterTrackIndex(uint,uint)\0"
    "title,chapter\0selectTitleChapter(int,int)\0"
    "resetRepeatOne()\0nRepeatMode,nRandomMode\0"
    "updateRepeatRandomIcon(int,int)\0"
    "nRepeatState,nRandomState\0"
    "setRepeatRandomMode(int,int)\0scanMode\0"
    "setScanMode(int)\0listMode\0setListMode(bool)\0"
    "int\0deviceID\0getCurrentCategory(int)\0"
    "toggleRepeatMode()\0toggleRandomMode()\0"
    "getRepeatMode()\0getRandomMode()\0"
    "getScanMode()\0device\0GetMediaDuration(int)\0"
    "currentCategoryTab\0saveCategoryTab(QString)\0"
    "currentCategory\0getCategoryTab(QString)\0"
    "tabId\0sendOpStateForNotifier(int)\0"
    "deviceid\0IsNoSourceByDeviceid(int)\0"
    "sourceid\0IsNoSourceBySourceid(int)\0"
    "isSeekableMedia()\0restoreLastPlay()\0"
    "bDiscMuteState\0setDiscMuteState(bool)\0"
    "isMute\0setDragMuteState(bool)\0"
    "SaveAudioData()\0ctrl,bSendOSD\0"
    "isControllerDisable(int,bool)\0ctrl\0"
    "isControllerDisable(int)\0"
    "device,bChangeUnknown,bEmpty\0"
    "updateCoverFlowMediaInfo(int,bool,bool)\0"
    "isShow\0setIsShowPopup(bool)\0"
    "GetCurrentMediaPosition(int)\0"
    "GetFfRewState()\0getFrontLcdMode()\0"
    "getCamMode()\0IsDiscNormalPlaying()\0"
    "IsPlaying()\0AutoplayAllowed()\0"
    "ExitFromMLT()\0isMultipleiPodDevice()\0"
    "isMultipleUSBDevice()\0isMediaSyncfinished()\0"
    "isFlowViewEnable()\0nDevice\0"
    "isTAScanComplete(int)\0title,album,artist,count\0"
    "BTMusicInfoChanged(QString,QString,QString,QString)\0"
    "NotSupportBTOSD()\0titleMenu()\0topMenu()\0"
    "setSkipTrack()\0pObj,member\0"
    "invokeMethod(QObject*,QString)\0"
    "pObj,member,arg1\0"
    "invokeMethod(QObject*,QString,QString)\0"
    "invokeMethod(QObject*,QString,QVariant)\0"
    "pObj,member,arg1,arg2\0"
    "invokeMethod(QObject*,QString,QVariant,QVariant)\0"
    "tp,deviceId\0resumeTpMessage(QString,int)\0"
    "GetCDDAGraceNoteStatus()\0resumeNormalPlay\0"
    "cancelFFRW(bool)\0cancelFFRW()\0filePath\0"
    "IsFileSupported(QString)\0getCoverFlowEnable()\0"
    "set\0setCoverFlowEnable(bool)\0"
    "coverflowItemClick(QString)\0"
    "isRunFromFileManager\0PlayerMode\0"
    "isSourceChanged\0m_NewState\0QUrl\0folder\0"
    "album\0song\0artist\0DiscType\0isSearch\0"
    "isPlayFromMLT\0isBTStreaming\0isRestoring\0"
    "QStringList\0pathsTemp\0m_isAccOn\0"
    "m_isFrontLCD\0m_bCamInFG\0currentIndex\0"
    "isLongPress\0isEnableErrorPopup\0"
    "isBasicView\0isMinerFinished\0"
    "isUSB1MinerFinished\0isUSB2MinerFinished\0"
    "trackNumber\0groupNumber\0isForwardDirection\0"
    "iPodSortingOrder\0QVariantList\0"
    "categoryList\0isBasicControlEnableStatus\0"
    "isForeground\0bShowInfo\0"
};

void AppMediaPlayer_Controller_Audio::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Controller_Audio *_t = static_cast<AppMediaPlayer_Controller_Audio *>(_o);
        switch (_id) {
        case 0: _t->disableHardKey((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->tuneSameFileNoti((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->showPathView((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 3: _t->activateTab((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 4: _t->serch_TabClicked(); break;
        case 5: _t->updateUIForDisc(); break;
        case 6: _t->closePopUpLoader(); break;
        case 7: _t->dvdInfochanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 8: _t->getdvdInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 9: _t->playbackStopped(); break;
        case 10: _t->resetCurrentIndex(); break;
        case 11: _t->playbackStarted(); break;
        case 12: _t->playbackForcedStart(); break;
        case 13: _t->playbackPaused(); break;
        case 14: _t->playbackTuned(); break;
        case 15: _t->bluetoothPlaybackStarted(); break;
        case 16: _t->bluetoothPlaybackPaused(); break;
        case 17: _t->playListReadyAutoStart(); break;
        case 18: _t->playListStartSameSong(); break;
        case 19: _t->CommandOnVideoDec((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->CommandOnBT((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->FastForwardComanndOnVideoDec((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 22: _t->Error(); break;
        case 23: _t->VolumeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->setRandomRepeatIcons((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 25: _t->resetSpeedIcon(); break;
        case 26: _t->resetPlayIconBT(); break;
        case 27: _t->setSpeedIcon((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: _t->startScan(); break;
        case 29: _t->stopScan(); break;
        case 30: _t->setStatusDataByIndex((*reinterpret_cast< MP::DEVICE_T(*)>(_a[1])),(*reinterpret_cast< DATA_TYPE_T(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 31: _t->setAppState((*reinterpret_cast< MP::DEVICE_T(*)>(_a[1]))); break;
        case 32: _t->sendMoreLikeThisList((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2])),(*reinterpret_cast< QStringList(*)>(_a[3]))); break;
        case 33: _t->showNoFoundPopup(); break;
        case 34: _t->showPopupError((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 35: _t->sigShowBTCallPopup(); break;
        case 36: _t->sigCloseBTCallPopup(); break;
        case 37: _t->sigShowDeckHighTempPopup(); break;
        case 38: _t->sigCloseDeckHighTempPopup(); break;
        case 39: _t->changeIPodCategory((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 40: _t->showPopupLoadingData(); break;
        case 41: _t->allTagReceived(); break;
        case 42: _t->showIPodRemovedPopup(); break;
        case 43: _t->updateCDTrackMetadata((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 44: _t->setHighlightCover((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 45: _t->clearCoverflowIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 46: _t->setCoverflowIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 47: _t->tuneTextColorChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 48: _t->signalGracenoteHighlightedVolume((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 49: _t->tocReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 50: _t->signalCloseSearch(); break;
        case 51: _t->mediaSyncfinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 52: _t->updateFolderName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 53: _t->deviceMinerFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< MP::DEVICE_T(*)>(_a[3]))); break;
        case 54: _t->trackNumberChanged(); break;
        case 55: _t->groupNumberChanged(); break;
        case 56: _t->isRunFromFileManagerChanged(); break;
        case 57: _t->clearSeekMode(); break;
        case 58: _t->iPodPlayableSongCount((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 59: _t->isBasicViewChanged(); break;
        case 60: _t->isDiscTypeChanged(); break;
        case 61: _t->enableModeArea(); break;
        case 62: _t->updateTrackInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 63: _t->tuneCounter((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 64: _t->tuneMetaData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 65: _t->tuneCoverart((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 66: _t->dirTreeFetched((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 67: _t->iPodPlaying((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 68: _t->UpdateStateTrackOSD((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 69: _t->showInhibitionIcon((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 70: _t->signalNoContent(); break;
        case 71: _t->iPodDeviceConnectInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 72: _t->releaseResource(); break;
        case 73: _t->operationDisplay((*reinterpret_cast< DISPLAY_T(*)>(_a[1]))); break;
        case 74: _t->isPlayerModeChanged(); break;
        case 75: _t->updateSongsCount(); break;
        case 76: _t->changeFastFowardRewind((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 77: _t->isPlayFromMLTChanged(); break;
        case 78: _t->showPlayerView(); break;
        case 79: _t->mp3fileInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6]))); break;
        case 80: _t->isForegroundChanged(); break;
        case 81: _t->iPodSortingOrderChanged(); break;
        case 82: _t->clearRepeatRandom(); break;
        case 83: _t->sendPositon((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 84: _t->sendPlayStatus((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 85: _t->wrongDisc(); break;
        case 86: _t->basicControlEnableStatusChanged((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 87: _t->hideiPod3rdPartyTxt(); break;
        case 88: _t->restoreIndexCountText(); break;
        case 89: _t->clearAllJog(); break;
        case 90: _t->HandleMediaStatusChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 91: _t->onMediaStatusChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 92: _t->HandleError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 93: _t->ipod1Authorised((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 94: _t->ipod2Authorised((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 95: _t->onIPodReady((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 96: _t->onAutoPlayReady((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 97: _t->ipodDisconnectedSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 98: _t->onGetDeviceName1((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 99: _t->onGetDeviceName2((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 100: _t->onSystemStateStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 101: _t->displayiPodTaggingOSD((*reinterpret_cast< EIPodPopUp(*)>(_a[1]))); break;
        case 102: _t->HandleRequestComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        case 103: _t->HandleRequestComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 104: _t->HandleRequestCompleteWithVolumeType((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 105: _t->ScanComplete(); break;
        case 106: _t->OnSearchCompleted((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 107: _t->PlaylistDataReceived((*reinterpret_cast< PlaylistResults(*)>(_a[1]))); break;
        case 108: _t->onCDEjected(); break;
        case 109: _t->onGraceNoteTOCReceived((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2]))); break;
        case 110: _t->parseReceivedDirTree((*reinterpret_cast< DvdcDirTree*(*)>(_a[1]))); break;
        case 111: _t->requestCurrentFolderName(); break;
        case 112: _t->receivedCurrentFolderName((*reinterpret_cast< DvdcFolderName*(*)>(_a[1]))); break;
        case 113: _t->updateCurrentMP3FolderName(); break;
        case 114: _t->onPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 115: _t->onDurationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 116: _t->onTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 117: _t->onDeckTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 118: _t->onDeckTrackUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 119: _t->onMediaInfoChanged((*reinterpret_cast< MediaInfo*(*)>(_a[1]))); break;
        case 120: _t->onMediaInfoChangedEx((*reinterpret_cast< MediaInfo*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 121: _t->onMediaInfoChangedEx((*reinterpret_cast< MediaInfo*(*)>(_a[1]))); break;
        case 122: _t->onStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 123: _t->onNormalPlay(); break;
        case 124: _t->onTagReceived((*reinterpret_cast< DvdcTagInfo*(*)>(_a[1]))); break;
        case 125: _t->onAllTagReceived((*reinterpret_cast< DvdcAllTagInfo*(*)>(_a[1]))); break;
        case 126: _t->onModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 127: _t->onRealSoundChanged(); break;
        case 128: _t->OnTrackBufferred((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 129: _t->onMinerFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 130: { bool _r = _t->onMinerInProgress((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 131: _t->onAutoTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 132: _t->onReleaseResources((*reinterpret_cast< MEDIAPLAYER_RESOURCES_T(*)>(_a[1]))); break;
        case 133: _t->onResourceGranted(); break;
        case 134: _t->onIPodTotalNumTracks((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 135: _t->onTuneMetaInfoTimeOver(); break;
        case 136: _t->onRequestCoverArtForListView(); break;
        case 137: _t->onRequestCoverArtForFlowView(); break;
        case 138: _t->onRequestFirstSong(); break;
        case 139: _t->onBtResponseTimerExpired(); break;
        case 140: _t->onBtResponseTimerStop(); break;
        case 141: _t->onBtDelayedGetInfoTimerExpired(); break;
        case 142: _t->onBtDelayedGetInfoTimerStop(); break;
        case 143: _t->onBtDelayedGetInfoTimerStart(); break;
        case 144: _t->onBtPeriodicGetInfoTimerExpired(); break;
        case 145: _t->onBtPeriodicGetInfoTimerStop(); break;
        case 146: _t->onBtPeriodicGetInfoTimerStart(); break;
        case 147: _t->onBtRequestTimerExpired(); break;
        case 148: _t->onBtDelayedPlayTimerExpired(); break;
        case 149: _t->onBtDelayedMuteTimerExpired(); break;
        case 150: _t->oniPodMountTimerExpired(); break;
        case 151: _t->onIPodDeviceConnectedSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< EIPodPorts(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 152: { QString _r = _t->getIPODDeviceName((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 153: _t->onBTFunc((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 154: _t->onDiscloadRequest(); break;
        case 155: _t->onAudioSinkError(); break;
        case 156: _t->onUserTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 157: _t->onLongBtnTimerExpired(); break;
        case 158: _t->onScanTimerExpired(); break;
        case 159: _t->onNotifyPlayStateForSiri(); break;
        case 160: _t->onLaunchedDefaultMusicApp((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 161: { bool _r = _t->getIsForeground();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 162: _t->onCurrentTrackCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 163: _t->onDatabaseUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 164: _t->onBasicControlEnableStatusChanged((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 165: _t->UpdateClusterTrackIndex((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 166: _t->selectTitleChapter((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 167: _t->resetRepeatOne(); break;
        case 168: _t->updateRepeatRandomIcon((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 169: _t->setRepeatRandomMode((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 170: _t->setScanMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 171: _t->setListMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 172: { int _r = _t->getCurrentCategory((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 173: _t->toggleRepeatMode(); break;
        case 174: _t->toggleRandomMode(); break;
        case 175: { int _r = _t->getRepeatMode();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 176: { int _r = _t->getRandomMode();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 177: { int _r = _t->getScanMode();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 178: { int _r = _t->GetMediaDuration((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 179: _t->saveCategoryTab((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 180: { QString _r = _t->getCategoryTab((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 181: _t->sendOpStateForNotifier((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 182: { bool _r = _t->IsNoSourceByDeviceid((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 183: { bool _r = _t->IsNoSourceBySourceid((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 184: { bool _r = _t->isSeekableMedia();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 185: _t->restoreLastPlay(); break;
        case 186: _t->setDiscMuteState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 187: _t->setDragMuteState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 188: _t->SaveAudioData(); break;
        case 189: { bool _r = _t->isControllerDisable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 190: { bool _r = _t->isControllerDisable((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 191: _t->updateCoverFlowMediaInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 192: _t->setIsShowPopup((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 193: { int _r = _t->GetCurrentMediaPosition((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 194: { bool _r = _t->GetFfRewState();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 195: { bool _r = _t->getFrontLcdMode();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 196: { bool _r = _t->getCamMode();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 197: { bool _r = _t->IsDiscNormalPlaying();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 198: { bool _r = _t->IsPlaying();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 199: { bool _r = _t->AutoplayAllowed();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 200: _t->ExitFromMLT(); break;
        case 201: { bool _r = _t->isMultipleiPodDevice();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 202: { bool _r = _t->isMultipleUSBDevice();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 203: { bool _r = _t->isMediaSyncfinished();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 204: { bool _r = _t->isFlowViewEnable();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 205: { bool _r = _t->isTAScanComplete((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 206: _t->BTMusicInfoChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 207: _t->NotSupportBTOSD(); break;
        case 208: _t->titleMenu(); break;
        case 209: _t->topMenu(); break;
        case 210: _t->setSkipTrack(); break;
        case 211: _t->invokeMethod((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 212: _t->invokeMethod((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 213: _t->invokeMethod((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        case 214: _t->invokeMethod((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3])),(*reinterpret_cast< QVariant(*)>(_a[4]))); break;
        case 215: _t->resumeTpMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 216: { int _r = _t->GetCDDAGraceNoteStatus();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 217: { bool _r = _t->cancelFFRW((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 218: { bool _r = _t->cancelFFRW();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 219: { bool _r = _t->IsFileSupported((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 220: { bool _r = _t->getCoverFlowEnable();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 221: _t->setCoverFlowEnable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 222: _t->coverflowItemClick((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Controller_Audio::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Controller_Audio::staticMetaObject = {
    { &AppMediaPlayer_Controller_Base::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Controller_Audio,
      qt_meta_data_AppMediaPlayer_Controller_Audio, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Controller_Audio::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Controller_Audio::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Controller_Audio::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Controller_Audio))
        return static_cast<void*>(const_cast< AppMediaPlayer_Controller_Audio*>(this));
    return AppMediaPlayer_Controller_Base::qt_metacast(_clname);
}

int AppMediaPlayer_Controller_Audio::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppMediaPlayer_Controller_Base::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 223)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 223;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = isRunFromFileManager(); break;
        case 1: *reinterpret_cast< int*>(_v) = playerMode(); break;
        case 2: *reinterpret_cast< bool*>(_v) = isSourceChanged(); break;
        case 3: *reinterpret_cast< int*>(_v) = newState(); break;
        case 4: *reinterpret_cast< QUrl*>(_v) = folder(); break;
        case 5: *reinterpret_cast< QString*>(_v) = album(); break;
        case 6: *reinterpret_cast< QString*>(_v) = GetCurrentSongName(); break;
        case 7: *reinterpret_cast< QString*>(_v) = GetCurrentArtist(); break;
        case 8: *reinterpret_cast< QString*>(_v) = GetCurrentFilePath(); break;
        case 9: *reinterpret_cast< QString*>(_v) = GetCurrentCover(); break;
        case 10: *reinterpret_cast< int*>(_v) = getDiscType(); break;
        case 11: *reinterpret_cast< bool*>(_v) = isSearch(); break;
        case 12: *reinterpret_cast< bool*>(_v) = isPlayFromMLT(); break;
        case 13: *reinterpret_cast< bool*>(_v) = isBTStreaming(); break;
        case 14: *reinterpret_cast< bool*>(_v) = isRestoring(); break;
        case 15: *reinterpret_cast< QStringList*>(_v) = getPathsTemp(); break;
        case 16: *reinterpret_cast< int*>(_v) = getAccMode(); break;
        case 17: *reinterpret_cast< int*>(_v) = getFrontLcdMode(); break;
        case 18: *reinterpret_cast< int*>(_v) = getCamMode(); break;
        case 19: *reinterpret_cast< int*>(_v) = GetCurrentTrackIndex(); break;
        case 20: *reinterpret_cast< int*>(_v) = isLongPress(); break;
        case 21: *reinterpret_cast< bool*>(_v) = isEnableErrorPopup(); break;
        case 22: *reinterpret_cast< bool*>(_v) = isBasicView(); break;
        case 23: *reinterpret_cast< bool*>(_v) = isMinerFinished(); break;
        case 24: *reinterpret_cast< bool*>(_v) = isUSB1MinerFinished(); break;
        case 25: *reinterpret_cast< bool*>(_v) = isUSB2MinerFinished(); break;
        case 26: *reinterpret_cast< QString*>(_v) = trackNumber(); break;
        case 27: *reinterpret_cast< QString*>(_v) = groupNumber(); break;
        case 28: *reinterpret_cast< bool*>(_v) = isForwardDirection(); break;
        case 29: *reinterpret_cast< int*>(_v) = getIPodSortingOrder(); break;
        case 30: *reinterpret_cast< QVariantList*>(_v) = categoryList(); break;
        case 31: *reinterpret_cast< bool*>(_v) = isBasicControlEnableStatus(); break;
        case 32: *reinterpret_cast< bool*>(_v) = isForeground(); break;
        case 33: *reinterpret_cast< bool*>(_v) = getShowInfo(); break;
        }
        _id -= 34;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setIsRunFromFileManager(*reinterpret_cast< bool*>(_v)); break;
        case 1: setPlayerMode(*reinterpret_cast< int*>(_v)); break;
        case 2: SetIsSourceChanged(*reinterpret_cast< bool*>(_v)); break;
        case 3: setNewState(*reinterpret_cast< int*>(_v)); break;
        case 4: setFolder(*reinterpret_cast< QUrl*>(_v)); break;
        case 5: setAlbum(*reinterpret_cast< QString*>(_v)); break;
        case 11: setIsSearch(*reinterpret_cast< bool*>(_v)); break;
        case 12: setIsPlayFromMLT(*reinterpret_cast< bool*>(_v)); break;
        case 13: setIsBTStreaming(*reinterpret_cast< bool*>(_v)); break;
        case 19: setCurrentIndex(*reinterpret_cast< int*>(_v)); break;
        case 20: setIsLongPress(*reinterpret_cast< int*>(_v)); break;
        case 21: setIsEnableErrorPopup(*reinterpret_cast< bool*>(_v)); break;
        case 22: setBasicView(*reinterpret_cast< bool*>(_v)); break;
        case 23: setIsMinerFinished(*reinterpret_cast< bool*>(_v)); break;
        case 24: setIsUSB1MinerFinished(*reinterpret_cast< bool*>(_v)); break;
        case 25: setIsUSB2MinerFinished(*reinterpret_cast< bool*>(_v)); break;
        case 30: categoryListSetter(*reinterpret_cast< QVariantList*>(_v)); break;
        case 32: setForeground(*reinterpret_cast< bool*>(_v)); break;
        case 33: setShowInfo(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 34;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 34;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 34;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 34;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 34;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 34;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 34;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Controller_Audio::disableHardKey(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Controller_Audio::tuneSameFileNoti(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Controller_Audio::showPathView(QVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppMediaPlayer_Controller_Audio::activateTab(int _t1, bool _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AppMediaPlayer_Controller_Audio::serch_TabClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void AppMediaPlayer_Controller_Audio::updateUIForDisc()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void AppMediaPlayer_Controller_Audio::closePopUpLoader()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void AppMediaPlayer_Controller_Audio::dvdInfochanged(int _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void AppMediaPlayer_Controller_Audio::getdvdInfo(int _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void AppMediaPlayer_Controller_Audio::playbackStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void AppMediaPlayer_Controller_Audio::resetCurrentIndex()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}

// SIGNAL 11
void AppMediaPlayer_Controller_Audio::playbackStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void AppMediaPlayer_Controller_Audio::playbackForcedStart()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void AppMediaPlayer_Controller_Audio::playbackPaused()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void AppMediaPlayer_Controller_Audio::playbackTuned()
{
    QMetaObject::activate(this, &staticMetaObject, 14, 0);
}

// SIGNAL 15
void AppMediaPlayer_Controller_Audio::bluetoothPlaybackStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 15, 0);
}

// SIGNAL 16
void AppMediaPlayer_Controller_Audio::bluetoothPlaybackPaused()
{
    QMetaObject::activate(this, &staticMetaObject, 16, 0);
}

// SIGNAL 17
void AppMediaPlayer_Controller_Audio::playListReadyAutoStart()
{
    QMetaObject::activate(this, &staticMetaObject, 17, 0);
}

// SIGNAL 18
void AppMediaPlayer_Controller_Audio::playListStartSameSong()
{
    QMetaObject::activate(this, &staticMetaObject, 18, 0);
}

// SIGNAL 19
void AppMediaPlayer_Controller_Audio::CommandOnVideoDec(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void AppMediaPlayer_Controller_Audio::CommandOnBT(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void AppMediaPlayer_Controller_Audio::FastForwardComanndOnVideoDec(bool _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void AppMediaPlayer_Controller_Audio::Error()
{
    QMetaObject::activate(this, &staticMetaObject, 22, 0);
}

// SIGNAL 23
void AppMediaPlayer_Controller_Audio::VolumeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void AppMediaPlayer_Controller_Audio::setRandomRepeatIcons(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void AppMediaPlayer_Controller_Audio::resetSpeedIcon()
{
    QMetaObject::activate(this, &staticMetaObject, 25, 0);
}

// SIGNAL 26
void AppMediaPlayer_Controller_Audio::resetPlayIconBT()
{
    QMetaObject::activate(this, &staticMetaObject, 26, 0);
}

// SIGNAL 27
void AppMediaPlayer_Controller_Audio::setSpeedIcon(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 27, _a);
}

// SIGNAL 28
void AppMediaPlayer_Controller_Audio::startScan()
{
    QMetaObject::activate(this, &staticMetaObject, 28, 0);
}

// SIGNAL 29
void AppMediaPlayer_Controller_Audio::stopScan()
{
    QMetaObject::activate(this, &staticMetaObject, 29, 0);
}

// SIGNAL 30
void AppMediaPlayer_Controller_Audio::setStatusDataByIndex(MP::DEVICE_T _t1, DATA_TYPE_T _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 30, _a);
}

// SIGNAL 31
void AppMediaPlayer_Controller_Audio::setAppState(MP::DEVICE_T _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 31, _a);
}

// SIGNAL 32
void AppMediaPlayer_Controller_Audio::sendMoreLikeThisList(QStringList _t1, QStringList _t2, QStringList _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 32, _a);
}

// SIGNAL 33
void AppMediaPlayer_Controller_Audio::showNoFoundPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 33, 0);
}

// SIGNAL 34
void AppMediaPlayer_Controller_Audio::showPopupError(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 34, _a);
}

// SIGNAL 35
void AppMediaPlayer_Controller_Audio::sigShowBTCallPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 35, 0);
}

// SIGNAL 36
void AppMediaPlayer_Controller_Audio::sigCloseBTCallPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 36, 0);
}

// SIGNAL 37
void AppMediaPlayer_Controller_Audio::sigShowDeckHighTempPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 37, 0);
}

// SIGNAL 38
void AppMediaPlayer_Controller_Audio::sigCloseDeckHighTempPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 38, 0);
}

// SIGNAL 39
void AppMediaPlayer_Controller_Audio::changeIPodCategory(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 39, _a);
}

// SIGNAL 40
void AppMediaPlayer_Controller_Audio::showPopupLoadingData()
{
    QMetaObject::activate(this, &staticMetaObject, 40, 0);
}

// SIGNAL 41
void AppMediaPlayer_Controller_Audio::allTagReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 41, 0);
}

// SIGNAL 42
void AppMediaPlayer_Controller_Audio::showIPodRemovedPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 42, 0);
}

// SIGNAL 43
void AppMediaPlayer_Controller_Audio::updateCDTrackMetadata(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 43, _a);
}

// SIGNAL 44
void AppMediaPlayer_Controller_Audio::setHighlightCover(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 44, _a);
}

// SIGNAL 45
void AppMediaPlayer_Controller_Audio::clearCoverflowIndex(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 45, _a);
}

// SIGNAL 46
void AppMediaPlayer_Controller_Audio::setCoverflowIndex(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 46, _a);
}

// SIGNAL 47
void AppMediaPlayer_Controller_Audio::tuneTextColorChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 47, _a);
}

// SIGNAL 48
void AppMediaPlayer_Controller_Audio::signalGracenoteHighlightedVolume(int _t1, QString _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 48, _a);
}

// SIGNAL 49
void AppMediaPlayer_Controller_Audio::tocReceived(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 49, _a);
}

// SIGNAL 50
void AppMediaPlayer_Controller_Audio::signalCloseSearch()
{
    QMetaObject::activate(this, &staticMetaObject, 50, 0);
}

// SIGNAL 51
void AppMediaPlayer_Controller_Audio::mediaSyncfinished(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 51, _a);
}

// SIGNAL 52
void AppMediaPlayer_Controller_Audio::updateFolderName(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 52, _a);
}

// SIGNAL 53
void AppMediaPlayer_Controller_Audio::deviceMinerFinished(bool _t1, int _t2, MP::DEVICE_T _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 53, _a);
}

// SIGNAL 54
void AppMediaPlayer_Controller_Audio::trackNumberChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 54, 0);
}

// SIGNAL 55
void AppMediaPlayer_Controller_Audio::groupNumberChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 55, 0);
}

// SIGNAL 56
void AppMediaPlayer_Controller_Audio::isRunFromFileManagerChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 56, 0);
}

// SIGNAL 57
void AppMediaPlayer_Controller_Audio::clearSeekMode()
{
    QMetaObject::activate(this, &staticMetaObject, 57, 0);
}

// SIGNAL 58
void AppMediaPlayer_Controller_Audio::iPodPlayableSongCount(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 58, _a);
}

// SIGNAL 59
void AppMediaPlayer_Controller_Audio::isBasicViewChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 59, 0);
}

// SIGNAL 60
void AppMediaPlayer_Controller_Audio::isDiscTypeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 60, 0);
}

// SIGNAL 61
void AppMediaPlayer_Controller_Audio::enableModeArea()
{
    QMetaObject::activate(this, &staticMetaObject, 61, 0);
}

// SIGNAL 62
void AppMediaPlayer_Controller_Audio::updateTrackInfo(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 62, _a);
}

// SIGNAL 63
void AppMediaPlayer_Controller_Audio::tuneCounter(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 63, _a);
}

// SIGNAL 64
void AppMediaPlayer_Controller_Audio::tuneMetaData(QString _t1, QString _t2, QString _t3, QString _t4, bool _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 64, _a);
}

// SIGNAL 65
void AppMediaPlayer_Controller_Audio::tuneCoverart(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 65, _a);
}

// SIGNAL 66
void AppMediaPlayer_Controller_Audio::dirTreeFetched(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 66, _a);
}

// SIGNAL 67
void AppMediaPlayer_Controller_Audio::iPodPlaying(int _t1, QString _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 67, _a);
}

// SIGNAL 68
void AppMediaPlayer_Controller_Audio::UpdateStateTrackOSD(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 68, _a);
}

// SIGNAL 69
void AppMediaPlayer_Controller_Audio::showInhibitionIcon(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 69, _a);
}

// SIGNAL 70
void AppMediaPlayer_Controller_Audio::signalNoContent()
{
    QMetaObject::activate(this, &staticMetaObject, 70, 0);
}

// SIGNAL 71
void AppMediaPlayer_Controller_Audio::iPodDeviceConnectInfo(int _t1, QString _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 71, _a);
}

// SIGNAL 72
void AppMediaPlayer_Controller_Audio::releaseResource()
{
    QMetaObject::activate(this, &staticMetaObject, 72, 0);
}

// SIGNAL 73
void AppMediaPlayer_Controller_Audio::operationDisplay(DISPLAY_T _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 73, _a);
}

// SIGNAL 74
void AppMediaPlayer_Controller_Audio::isPlayerModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 74, 0);
}

// SIGNAL 75
void AppMediaPlayer_Controller_Audio::updateSongsCount()
{
    QMetaObject::activate(this, &staticMetaObject, 75, 0);
}

// SIGNAL 76
void AppMediaPlayer_Controller_Audio::changeFastFowardRewind(bool _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 76, _a);
}

// SIGNAL 77
void AppMediaPlayer_Controller_Audio::isPlayFromMLTChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 77, 0);
}

// SIGNAL 78
void AppMediaPlayer_Controller_Audio::showPlayerView()
{
    QMetaObject::activate(this, &staticMetaObject, 78, 0);
}

// SIGNAL 79
void AppMediaPlayer_Controller_Audio::mp3fileInfo(QString _t1, QString _t2, QString _t3, QString _t4, QString _t5, QString _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 79, _a);
}

// SIGNAL 80
void AppMediaPlayer_Controller_Audio::isForegroundChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 80, 0);
}

// SIGNAL 81
void AppMediaPlayer_Controller_Audio::iPodSortingOrderChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 81, 0);
}

// SIGNAL 82
void AppMediaPlayer_Controller_Audio::clearRepeatRandom()
{
    QMetaObject::activate(this, &staticMetaObject, 82, 0);
}

// SIGNAL 83
void AppMediaPlayer_Controller_Audio::sendPositon(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 83, _a);
}

// SIGNAL 84
void AppMediaPlayer_Controller_Audio::sendPlayStatus(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 84, _a);
}

// SIGNAL 85
void AppMediaPlayer_Controller_Audio::wrongDisc()
{
    QMetaObject::activate(this, &staticMetaObject, 85, 0);
}

// SIGNAL 86
void AppMediaPlayer_Controller_Audio::basicControlEnableStatusChanged(bool _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 86, _a);
}

// SIGNAL 87
void AppMediaPlayer_Controller_Audio::hideiPod3rdPartyTxt()
{
    QMetaObject::activate(this, &staticMetaObject, 87, 0);
}

// SIGNAL 88
void AppMediaPlayer_Controller_Audio::restoreIndexCountText()
{
    QMetaObject::activate(this, &staticMetaObject, 88, 0);
}

// SIGNAL 89
void AppMediaPlayer_Controller_Audio::clearAllJog()
{
    QMetaObject::activate(this, &staticMetaObject, 89, 0);
}
QT_END_MOC_NAMESPACE
