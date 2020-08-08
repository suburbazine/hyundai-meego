/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodAppController_Def.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../IPodAppController/include/DHAVN_IPodAppController_Def.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodAppController_Def.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CIPodAppController[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,
      45,   35,   19,   19, 0x05,
      77,   70,   19,   19, 0x05,
     115,  109,   19,   19, 0x05,
     159,  145,   19,   19, 0x05,
     178,   19,   19,   19, 0x05,
     212,  200,   19,   19, 0x05,
     251,  240,   19,   19, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CIPodAppController[] = {
    "CIPodAppController\0\0InitComplete()\0"
    "dataArray\0DataReceived(QByteArray)\0"
    "status\0Acknowledgement(EIPodAppErrors)\0"
    "error\0SessionClosed(EIPodAppErrors)\0"
    "protocolIndex\0SessionOpened(int)\0"
    "NotifySessionClosed()\0nSampleRate\0"
    "AudioSampleRateChanged(int)\0ipodNumber\0"
    "DeviceDisconnected(int)\0"
};

void CIPodAppController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CIPodAppController *_t = static_cast<CIPodAppController *>(_o);
        switch (_id) {
        case 0: _t->InitComplete(); break;
        case 1: _t->DataReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->Acknowledgement((*reinterpret_cast< EIPodAppErrors(*)>(_a[1]))); break;
        case 3: _t->SessionClosed((*reinterpret_cast< EIPodAppErrors(*)>(_a[1]))); break;
        case 4: _t->SessionOpened((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->NotifySessionClosed(); break;
        case 6: _t->AudioSampleRateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->DeviceDisconnected((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CIPodAppController::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CIPodAppController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CIPodAppController,
      qt_meta_data_CIPodAppController, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CIPodAppController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CIPodAppController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CIPodAppController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CIPodAppController))
        return static_cast<void*>(const_cast< CIPodAppController*>(this));
    return QObject::qt_metacast(_clname);
}

int CIPodAppController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CIPodAppController::InitComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CIPodAppController::DataReceived(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CIPodAppController::Acknowledgement(EIPodAppErrors _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CIPodAppController::SessionClosed(EIPodAppErrors _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CIPodAppController::SessionOpened(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CIPodAppController::NotifySessionClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void CIPodAppController::AudioSampleRateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CIPodAppController::DeviceDisconnected(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
