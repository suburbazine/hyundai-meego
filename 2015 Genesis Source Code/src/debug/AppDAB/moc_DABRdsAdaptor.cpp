/****************************************************************************
** Meta object code from reading C++ file 'DABRdsAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DABRdsAdaptor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DABRdsAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DABRdsAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x05,
      43,   15,   14,   14, 0x05,
      68,   15,   14,   14, 0x05,
      93,   15,   14,   14, 0x05,
     131,  120,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     167,   15,   14,   14, 0x0a,
     191,   15,   14,   14, 0x0a,
     218,   14,   14,   14, 0x0a,
     242,   14,  237,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DABRdsAdaptor[] = {
    "DABRdsAdaptor\0\0onOff\0reqNewsDABtoRDS(bool)\0"
    "reqTrafficDABToRDS(bool)\0"
    "cmdTrafficRDStoDAB(bool)\0"
    "cmdReqSetAnnounement(bool)\0type,onOff\0"
    "cmdReqDisplayStatusBar(quint8,bool)\0"
    "onTrafficRDStoDAB(bool)\0"
    "onReqTrafficDABtoRDS(bool)\0"
    "onCheckRDSLanuch()\0bool\0"
    "getAnnouncePopupStatus()\0"
};

void DABRdsAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DABRdsAdaptor *_t = static_cast<DABRdsAdaptor *>(_o);
        switch (_id) {
        case 0: _t->reqNewsDABtoRDS((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->reqTrafficDABToRDS((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->cmdTrafficRDStoDAB((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->cmdReqSetAnnounement((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->cmdReqDisplayStatusBar((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->onTrafficRDStoDAB((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->onReqTrafficDABtoRDS((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->onCheckRDSLanuch(); break;
        case 8: { bool _r = _t->getAnnouncePopupStatus();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DABRdsAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DABRdsAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DABRdsAdaptor,
      qt_meta_data_DABRdsAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DABRdsAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DABRdsAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DABRdsAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DABRdsAdaptor))
        return static_cast<void*>(const_cast< DABRdsAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int DABRdsAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void DABRdsAdaptor::reqNewsDABtoRDS(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DABRdsAdaptor::reqTrafficDABToRDS(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DABRdsAdaptor::cmdTrafficRDStoDAB(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DABRdsAdaptor::cmdReqSetAnnounement(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DABRdsAdaptor::cmdReqDisplayStatusBar(quint8 _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
