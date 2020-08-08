/****************************************************************************
** Meta object code from reading C++ file 'msc_identification.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/modules/most/msc/msc_identification.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'msc_identification.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MscIdentification[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   19,   18,   18, 0x08,
      52,   19,   18,   18, 0x08,
      78,   19,   18,   18, 0x08,
     106,   19,   18,   18, 0x08,
     142,   19,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MscIdentification[] = {
    "MscIdentification\0\0,\0"
    "ReadHWIdentifier(uint,QString)\0"
    "ReadDevName(uint,QString)\0"
    "ReadHWVersion(uint,QString)\0"
    "ReadNumberofModule(uint,QByteArray)\0"
    "ReadModuleVersion(uint,QDBusVariant)\0"
};

void MscIdentification::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MscIdentification *_t = static_cast<MscIdentification *>(_o);
        switch (_id) {
        case 0: _t->ReadHWIdentifier((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->ReadDevName((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->ReadHWVersion((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->ReadNumberofModule((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 4: _t->ReadModuleVersion((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QDBusVariant(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MscIdentification::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MscIdentification::staticMetaObject = {
    { &MscAbstract::staticMetaObject, qt_meta_stringdata_MscIdentification,
      qt_meta_data_MscIdentification, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MscIdentification::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MscIdentification::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MscIdentification::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MscIdentification))
        return static_cast<void*>(const_cast< MscIdentification*>(this));
    return MscAbstract::qt_metacast(_clname);
}

int MscIdentification::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MscAbstract::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
