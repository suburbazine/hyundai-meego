/****************************************************************************
** Meta object code from reading C++ file 'osd_manager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/osd_manager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'osd_manager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OSD_manager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   13,   12,   12, 0x0a,
      50,   42,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_OSD_manager[] = {
    "OSD_manager\0\0osd_message\0sendOSD(QString)\0"
    "display\0OSD_AppsBooting(int)\0"
};

void OSD_manager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OSD_manager *_t = static_cast<OSD_manager *>(_o);
        switch (_id) {
        case 0: _t->sendOSD((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->OSD_AppsBooting((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData OSD_manager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OSD_manager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_OSD_manager,
      qt_meta_data_OSD_manager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OSD_manager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OSD_manager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OSD_manager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OSD_manager))
        return static_cast<void*>(const_cast< OSD_manager*>(this));
    return QObject::qt_metacast(_clname);
}

int OSD_manager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
