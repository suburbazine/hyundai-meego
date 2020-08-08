/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppUserManual_Def.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppUserManual_Def.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppUserManual_Def.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MANUALMENU[] = {

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
      11, 0x0,    5,   18,

 // enum data: key, value
      26, uint(MANUALMENU::OPTION_MANUAL_MENU),
      45, uint(MANUALMENU::OPTION_MANUAL_SEARCH),
      66, uint(MANUALMENU::OPTION_MANUAL_GOTO),
      85, uint(MANUALMENU::OPTION_MANUAL_EXIT_SEARCH),
     111, uint(MANUALMENU::OPTION_MANUAL_EXIT_ZOOM),

       0        // eod
};

static const char qt_meta_stringdata_MANUALMENU[] = {
    "MANUALMENU\0MANUAL_OPTIONS\0OPTION_MANUAL_MENU\0"
    "OPTION_MANUAL_SEARCH\0OPTION_MANUAL_GOTO\0"
    "OPTION_MANUAL_EXIT_SEARCH\0"
    "OPTION_MANUAL_EXIT_ZOOM\0"
};

void MANUALMENU::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MANUALMENU::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MANUALMENU::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MANUALMENU,
      qt_meta_data_MANUALMENU, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MANUALMENU::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MANUALMENU::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MANUALMENU::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MANUALMENU))
        return static_cast<void*>(const_cast< MANUALMENU*>(this));
    return QObject::qt_metacast(_clname);
}

int MANUALMENU::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
