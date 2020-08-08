/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFileManager_CommonDef.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFileManager_CommonDef.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFileManager_CommonDef.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CommonDef[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       5,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
      10, 0x0,    3,   34,
      20, 0x0,    7,   40,
      35, 0x0,    2,   54,
      64, 0x0,    3,   58,
      86, 0x0,    2,   64,

 // enum data: key, value
     108, uint(CommonDef::MusicFileType),
     122, uint(CommonDef::VideoFileType),
     136, uint(CommonDef::PictureFileType),
     152, uint(CommonDef::JOG_DIRECTION_UP),
     169, uint(CommonDef::JOG_DIRECTION_RIGHT),
     189, uint(CommonDef::JOG_DIRECTION_DOWN),
     208, uint(CommonDef::JOG_DIRECTION_LEFT),
     227, uint(CommonDef::JOG_DIRECTION_SCROLL_RIGHT),
     254, uint(CommonDef::JOG_DIRECTION_SCROLL_LEFT),
     280, uint(CommonDef::JOG_DIRECTION_CENTER),
     301, uint(CommonDef::FILEMANAGER_EVT_KEY_STATUS_PRESSED),
     336, uint(CommonDef::FILEMANAGER_EVT_KEY_STATUS_RELEASED),
     372, uint(CommonDef::FILEMANAGER_DISPLAY_FRONT),
     398, uint(CommonDef::FILEMANAGER_DISPLAY_REAR),
     423, uint(CommonDef::FILEMANAGER_DISPLAY_NONE),
     448, uint(CommonDef::FILEMANAGER_EVT_KEY_BACK),
     473, uint(CommonDef::FILEMANAGER_EVT_KEY_MENU),

       0        // eod
};

static const char qt_meta_stringdata_CommonDef[] = {
    "CommonDef\0FileTypes\0JOG_DIRECTIONS\0"
    "FILEMANAGER_EVT_KEY_STATUS_T\0"
    "FILEMANAGER_DISPLAY_T\0FILEMANAGER_EVT_KEY_T\0"
    "MusicFileType\0VideoFileType\0PictureFileType\0"
    "JOG_DIRECTION_UP\0JOG_DIRECTION_RIGHT\0"
    "JOG_DIRECTION_DOWN\0JOG_DIRECTION_LEFT\0"
    "JOG_DIRECTION_SCROLL_RIGHT\0"
    "JOG_DIRECTION_SCROLL_LEFT\0"
    "JOG_DIRECTION_CENTER\0"
    "FILEMANAGER_EVT_KEY_STATUS_PRESSED\0"
    "FILEMANAGER_EVT_KEY_STATUS_RELEASED\0"
    "FILEMANAGER_DISPLAY_FRONT\0"
    "FILEMANAGER_DISPLAY_REAR\0"
    "FILEMANAGER_DISPLAY_NONE\0"
    "FILEMANAGER_EVT_KEY_BACK\0"
    "FILEMANAGER_EVT_KEY_MENU\0"
};

void CommonDef::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CommonDef::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CommonDef::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CommonDef,
      qt_meta_data_CommonDef, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CommonDef::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CommonDef::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CommonDef::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CommonDef))
        return static_cast<void*>(const_cast< CommonDef*>(this));
    return QObject::qt_metacast(_clname);
}

int CommonDef::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
