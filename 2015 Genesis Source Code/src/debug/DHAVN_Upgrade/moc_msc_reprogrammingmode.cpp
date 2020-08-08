/****************************************************************************
** Meta object code from reading C++ file 'msc_reprogrammingmode.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/modules/most/msc/msc_reprogrammingmode.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'msc_reprogrammingmode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MscReprogrammingMode[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x08,
      54,   21,   21,   21, 0x08,
      76,   21,   21,   21, 0x08,
      94,   92,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MscReprogrammingMode[] = {
    "MscReprogrammingMode\0\0"
    "ReprogrammingMode(QDBusVariant)\0"
    "waitEnterBootloader()\0notReceiveAck()\0"
    ",\0ReadHWIdentifier(uint,QString)\0"
};

void MscReprogrammingMode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MscReprogrammingMode *_t = static_cast<MscReprogrammingMode *>(_o);
        switch (_id) {
        case 0: _t->ReprogrammingMode((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 1: _t->waitEnterBootloader(); break;
        case 2: _t->notReceiveAck(); break;
        case 3: _t->ReadHWIdentifier((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MscReprogrammingMode::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MscReprogrammingMode::staticMetaObject = {
    { &MscAbstract::staticMetaObject, qt_meta_stringdata_MscReprogrammingMode,
      qt_meta_data_MscReprogrammingMode, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MscReprogrammingMode::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MscReprogrammingMode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MscReprogrammingMode::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MscReprogrammingMode))
        return static_cast<void*>(const_cast< MscReprogrammingMode*>(this));
    return MscAbstract::qt_metacast(_clname);
}

int MscReprogrammingMode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MscAbstract::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
