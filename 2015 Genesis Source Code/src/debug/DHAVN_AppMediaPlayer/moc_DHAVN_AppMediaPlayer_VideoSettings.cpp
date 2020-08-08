/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_VideoSettings.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/util/video/DHAVN_AppMediaPlayer_VideoSettings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_VideoSettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VideoSettingsDef[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       2,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
      17, 0x0,   27,   22,
      41, 0x0,    2,   76,

 // enum data: key, value
      63, uint(VideoSettingsDef::DB_KEY_DIVX_REG_STATE),
      85, uint(VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_BRIGHTNESS),
     117, uint(VideoSettingsDef::DB_KEY_VIDEO_USB_BRIGHTNESS),
     145, uint(VideoSettingsDef::DB_KEY_VIDEO_USB2_BRIGHTNESS),
     174, uint(VideoSettingsDef::DB_KEY_VIDEO_DVD_BRIGHTNESS),
     202, uint(VideoSettingsDef::DB_KEY_VIDEO_VCD_BRIGHTNESS),
     230, uint(VideoSettingsDef::DB_KEY_VIDEO_AUX_BRIGHTNESS),
     258, uint(VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_SATURATION),
     290, uint(VideoSettingsDef::DB_KEY_VIDEO_USB_SATURATION),
     318, uint(VideoSettingsDef::DB_KEY_VIDEO_USB2_SATURATION),
     347, uint(VideoSettingsDef::DB_KEY_VIDEO_DVD_SATURATION),
     375, uint(VideoSettingsDef::DB_KEY_VIDEO_VCD_SATURATION),
     403, uint(VideoSettingsDef::DB_KEY_VIDEO_AUX_SATURATION),
     431, uint(VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_HUE),
     456, uint(VideoSettingsDef::DB_KEY_VIDEO_USB_HUE),
     477, uint(VideoSettingsDef::DB_KEY_VIDEO_USB2_HUE),
     499, uint(VideoSettingsDef::DB_KEY_VIDEO_DVD_HUE),
     520, uint(VideoSettingsDef::DB_KEY_VIDEO_VCD_HUE),
     541, uint(VideoSettingsDef::DB_KEY_VIDEO_AUX_HUE),
     562, uint(VideoSettingsDef::DB_KEY_VIDEO_JUKEBOX_CONTRAST),
     592, uint(VideoSettingsDef::DB_KEY_VIDEO_USB_CONTRAST),
     618, uint(VideoSettingsDef::DB_KEY_VIDEO_USB2_CONTRAST),
     645, uint(VideoSettingsDef::DB_KEY_VIDEO_DVD_CONTRAST),
     671, uint(VideoSettingsDef::DB_KEY_VIDEO_VCD_CONTRAST),
     697, uint(VideoSettingsDef::DB_KEY_VIDEO_AUX_CONTRAST),
     723, uint(VideoSettingsDef::DB_KEY_LAST_ASPECT_RATIO),
     748, uint(VideoSettingsDef::DB_KEY_MAX),
     759, uint(VideoSettingsDef::SETTINGS_DIVX_REG),
     777, uint(VideoSettingsDef::SETTINGS_DIVX_DEREG),

       0        // eod
};

static const char qt_meta_stringdata_VideoSettingsDef[] = {
    "VideoSettingsDef\0VIDEO_DB_SETTINGS_KEY_T\0"
    "VIDEO_SETTINGS_DIVX_T\0DB_KEY_DIVX_REG_STATE\0"
    "DB_KEY_VIDEO_JUKEBOX_BRIGHTNESS\0"
    "DB_KEY_VIDEO_USB_BRIGHTNESS\0"
    "DB_KEY_VIDEO_USB2_BRIGHTNESS\0"
    "DB_KEY_VIDEO_DVD_BRIGHTNESS\0"
    "DB_KEY_VIDEO_VCD_BRIGHTNESS\0"
    "DB_KEY_VIDEO_AUX_BRIGHTNESS\0"
    "DB_KEY_VIDEO_JUKEBOX_SATURATION\0"
    "DB_KEY_VIDEO_USB_SATURATION\0"
    "DB_KEY_VIDEO_USB2_SATURATION\0"
    "DB_KEY_VIDEO_DVD_SATURATION\0"
    "DB_KEY_VIDEO_VCD_SATURATION\0"
    "DB_KEY_VIDEO_AUX_SATURATION\0"
    "DB_KEY_VIDEO_JUKEBOX_HUE\0DB_KEY_VIDEO_USB_HUE\0"
    "DB_KEY_VIDEO_USB2_HUE\0DB_KEY_VIDEO_DVD_HUE\0"
    "DB_KEY_VIDEO_VCD_HUE\0DB_KEY_VIDEO_AUX_HUE\0"
    "DB_KEY_VIDEO_JUKEBOX_CONTRAST\0"
    "DB_KEY_VIDEO_USB_CONTRAST\0"
    "DB_KEY_VIDEO_USB2_CONTRAST\0"
    "DB_KEY_VIDEO_DVD_CONTRAST\0"
    "DB_KEY_VIDEO_VCD_CONTRAST\0"
    "DB_KEY_VIDEO_AUX_CONTRAST\0"
    "DB_KEY_LAST_ASPECT_RATIO\0DB_KEY_MAX\0"
    "SETTINGS_DIVX_REG\0SETTINGS_DIVX_DEREG\0"
};

void VideoSettingsDef::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData VideoSettingsDef::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject VideoSettingsDef::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_VideoSettingsDef,
      qt_meta_data_VideoSettingsDef, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VideoSettingsDef::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VideoSettingsDef::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VideoSettingsDef::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VideoSettingsDef))
        return static_cast<void*>(const_cast< VideoSettingsDef*>(this));
    return QObject::qt_metacast(_clname);
}

int VideoSettingsDef::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_AppMediaPlayer_VideoSettings[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_VideoSettings[] = {
    "AppMediaPlayer_VideoSettings\0"
};

void AppMediaPlayer_VideoSettings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData AppMediaPlayer_VideoSettings::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_VideoSettings::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_VideoSettings,
      qt_meta_data_AppMediaPlayer_VideoSettings, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_VideoSettings::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_VideoSettings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_VideoSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_VideoSettings))
        return static_cast<void*>(const_cast< AppMediaPlayer_VideoSettings*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_VideoSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
