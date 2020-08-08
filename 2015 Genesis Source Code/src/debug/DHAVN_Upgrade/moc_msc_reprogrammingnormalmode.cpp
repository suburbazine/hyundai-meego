/****************************************************************************
** Meta object code from reading C++ file 'msc_reprogrammingnormalmode.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/modules/most/msc/msc_reprogrammingnormalmode.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'msc_reprogrammingnormalmode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MscReprogrammingNormalMode[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      28,   27,   27,   27, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MscReprogrammingNormalMode[] = {
    "MscReprogrammingNormalMode\0\0"
    "ReprogrammingMode(QDBusVariant)\0"
};

void MscReprogrammingNormalMode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MscReprogrammingNormalMode *_t = static_cast<MscReprogrammingNormalMode *>(_o);
        switch (_id) {
        case 0: _t->ReprogrammingMode((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MscReprogrammingNormalMode::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MscReprogrammingNormalMode::staticMetaObject = {
    { &MscAbstract::staticMetaObject, qt_meta_stringdata_MscReprogrammingNormalMode,
      qt_meta_data_MscReprogrammingNormalMode, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MscReprogrammingNormalMode::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MscReprogrammingNormalMode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MscReprogrammingNormalMode::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MscReprogrammingNormalMode))
        return static_cast<void*>(const_cast< MscReprogrammingNormalMode*>(this));
    return MscAbstract::qt_metacast(_clname);
}

int MscReprogrammingNormalMode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MscAbstract::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
