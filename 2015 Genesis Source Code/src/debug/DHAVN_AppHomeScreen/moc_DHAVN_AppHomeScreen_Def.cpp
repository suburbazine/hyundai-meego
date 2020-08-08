/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppHomeScreen_Def.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppHomeScreen_Def.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppHomeScreen_Def.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HomeScreenDef[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
      14, 0x0,   10,   18,

 // enum data: key, value
      27, uint(HomeScreenDef::MEDIA_TYPE_INVALID),
      46, uint(HomeScreenDef::MEDIA_TYPE_AUDIO),
      63, uint(HomeScreenDef::MEDIA_TYPE_VIDEO),
      80, uint(HomeScreenDef::MEDIA_TYPE_RADIO),
      97, uint(HomeScreenDef::MEDIA_TYPE_DMB),
     112, uint(HomeScreenDef::MEDIA_TYPE_XM),
     126, uint(HomeScreenDef::MEDIA_TYPE_DAB),
     141, uint(HomeScreenDef::MEDIA_TYPE_PANDORA),
     160, uint(HomeScreenDef::MEDIA_TYPE_AHA),
     175, uint(HomeScreenDef::MEDIA_TYPE_MAX),

       0        // eod
};

static const char qt_meta_stringdata_HomeScreenDef[] = {
    "HomeScreenDef\0MEDIA_TYPE_T\0"
    "MEDIA_TYPE_INVALID\0MEDIA_TYPE_AUDIO\0"
    "MEDIA_TYPE_VIDEO\0MEDIA_TYPE_RADIO\0"
    "MEDIA_TYPE_DMB\0MEDIA_TYPE_XM\0"
    "MEDIA_TYPE_DAB\0MEDIA_TYPE_PANDORA\0"
    "MEDIA_TYPE_AHA\0MEDIA_TYPE_MAX\0"
};

void HomeScreenDef::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData HomeScreenDef::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HomeScreenDef::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HomeScreenDef,
      qt_meta_data_HomeScreenDef, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HomeScreenDef::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HomeScreenDef::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HomeScreenDef::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HomeScreenDef))
        return static_cast<void*>(const_cast< HomeScreenDef*>(this));
    return QObject::qt_metacast(_clname);
}

int HomeScreenDef::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
