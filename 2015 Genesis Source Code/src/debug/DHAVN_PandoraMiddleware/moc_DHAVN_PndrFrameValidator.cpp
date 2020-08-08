/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_PndrFrameValidator.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_PndrFrameValidator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_PndrFrameValidator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPndrFrameValidator[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      29,   21,   20,   20, 0x05,
      55,   20,   20,   20, 0x05,
      75,   20,   20,   20, 0x05,
      99,   20,   20,   20, 0x05,
     119,   20,   20,   20, 0x05,
     138,   20,   20,   20, 0x05,
     171,  155,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
     198,  191,   20,   20, 0x0a,
     225,  191,   20,   20, 0x0a,
     253,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CPndrFrameValidator[] = {
    "CPndrFrameValidator\0\0inFrame\0"
    "FrameReceived(QByteArray)\0SessionStartAcked()\0"
    "SessionTerminateAcked()\0NextBrandingImage()\0"
    "ReceivedBranding()\0ReceivedStatus()\0"
    "inMaxFrameDrops\0MaxFrameDrops(bool)\0"
    "inData\0OnDataReceived(QByteArray)\0"
    "OnFrameReceived(QByteArray)\0OnTimeout()\0"
};

void CPndrFrameValidator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CPndrFrameValidator *_t = static_cast<CPndrFrameValidator *>(_o);
        switch (_id) {
        case 0: _t->FrameReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->SessionStartAcked(); break;
        case 2: _t->SessionTerminateAcked(); break;
        case 3: _t->NextBrandingImage(); break;
        case 4: _t->ReceivedBranding(); break;
        case 5: _t->ReceivedStatus(); break;
        case 6: _t->MaxFrameDrops((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->OnDataReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 8: _t->OnFrameReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 9: _t->OnTimeout(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CPndrFrameValidator::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CPndrFrameValidator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CPndrFrameValidator,
      qt_meta_data_CPndrFrameValidator, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CPndrFrameValidator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CPndrFrameValidator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CPndrFrameValidator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPndrFrameValidator))
        return static_cast<void*>(const_cast< CPndrFrameValidator*>(this));
    return QObject::qt_metacast(_clname);
}

int CPndrFrameValidator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void CPndrFrameValidator::FrameReceived(const QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CPndrFrameValidator::SessionStartAcked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CPndrFrameValidator::SessionTerminateAcked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void CPndrFrameValidator::NextBrandingImage()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CPndrFrameValidator::ReceivedBranding()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void CPndrFrameValidator::ReceivedStatus()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void CPndrFrameValidator::MaxFrameDrops(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
