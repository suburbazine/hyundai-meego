/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_PndrTrackProperties.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_PndrTrackProperties.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_PndrTrackProperties.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPndrTrackProperties[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      39,   22,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
      76,   21,   21,   21, 0x0a,
      88,   22,   21,   21, 0x0a,
     189,  128,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CPndrTrackProperties[] = {
    "CPndrTrackProperties\0\0inRequest,inData\0"
    "RequestCompleted(EPndrEvent,QString)\0"
    "OnTimeout()\0OnRequestCompletion(EPndrEvent,quint32)\0"
    "inRequest,inTrackToken,inSegmentindex,inTotalsegments,inData\0"
    "OnRequestCompletion(EPndrEvent,quint32,quint8,quint8,QByteArray)\0"
};

void CPndrTrackProperties::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CPndrTrackProperties *_t = static_cast<CPndrTrackProperties *>(_o);
        switch (_id) {
        case 0: _t->RequestCompleted((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->OnTimeout(); break;
        case 2: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2]))); break;
        case 3: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2])),(*reinterpret_cast< const quint8(*)>(_a[3])),(*reinterpret_cast< const quint8(*)>(_a[4])),(*reinterpret_cast< const QByteArray(*)>(_a[5]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CPndrTrackProperties::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CPndrTrackProperties::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CPndrTrackProperties,
      qt_meta_data_CPndrTrackProperties, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CPndrTrackProperties::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CPndrTrackProperties::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CPndrTrackProperties::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPndrTrackProperties))
        return static_cast<void*>(const_cast< CPndrTrackProperties*>(this));
    return QObject::qt_metacast(_clname);
}

int CPndrTrackProperties::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CPndrTrackProperties::RequestCompleted(const EPndrEvent _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
