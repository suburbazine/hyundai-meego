/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodAppController_Private.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../IPodAppController/include/DHAVN_IPodAppController_Private.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodAppController_Private.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CIPodAppControllerPriv[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   23,   23,   23, 0x05,
      49,   39,   23,   23, 0x05,
      81,   74,   23,   23, 0x05,
     119,  113,   23,   23, 0x05,
     163,  149,   23,   23, 0x05,
     182,   23,   23,   23, 0x05,
     216,  204,   23,   23, 0x05,
     255,  244,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
     311,  279,   23,   23, 0x0a,
     348,  279,   23,   23, 0x0a,
     390,   23,   23,   23, 0x0a,
     416,  244,   23,   23, 0x0a,
     461,  444,   23,   23, 0x0a,
     501,   23,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CIPodAppControllerPriv[] = {
    "CIPodAppControllerPriv\0\0InitComplete()\0"
    "dataArray\0DataReceived(QByteArray)\0"
    "status\0Acknowledgement(EIPodAppErrors)\0"
    "error\0SessionClosed(EIPodAppErrors)\0"
    "protocolIndex\0SessionOpened(int)\0"
    "NotifySessionClosed()\0nSampleRate\0"
    "AudioSampleRateChanged(int)\0ipodNumber\0"
    "DeviceDisconnected(int)\0"
    "lingoId,commandId,iAPPacketData\0"
    "packetAvailable(int,int,QByteArray&)\0"
    "largePacketAvailable(int,int,QByteArray&)\0"
    "SendCommandTimerExpired()\0"
    "OnIPodDisconnected(QString)\0"
    "ipodNumber,appId\0"
    "OnIpodAppSessionClosed(QString,QString)\0"
    "AppLaunchAcknowledgementSlot()\0"
};

void CIPodAppControllerPriv::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CIPodAppControllerPriv *_t = static_cast<CIPodAppControllerPriv *>(_o);
        switch (_id) {
        case 0: _t->InitComplete(); break;
        case 1: _t->DataReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->Acknowledgement((*reinterpret_cast< EIPodAppErrors(*)>(_a[1]))); break;
        case 3: _t->SessionClosed((*reinterpret_cast< EIPodAppErrors(*)>(_a[1]))); break;
        case 4: _t->SessionOpened((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->NotifySessionClosed(); break;
        case 6: _t->AudioSampleRateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->DeviceDisconnected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->packetAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 9: _t->largePacketAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 10: _t->SendCommandTimerExpired(); break;
        case 11: _t->OnIPodDisconnected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->OnIpodAppSessionClosed((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 13: _t->AppLaunchAcknowledgementSlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CIPodAppControllerPriv::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CIPodAppControllerPriv::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CIPodAppControllerPriv,
      qt_meta_data_CIPodAppControllerPriv, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CIPodAppControllerPriv::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CIPodAppControllerPriv::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CIPodAppControllerPriv::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CIPodAppControllerPriv))
        return static_cast<void*>(const_cast< CIPodAppControllerPriv*>(this));
    return QObject::qt_metacast(_clname);
}

int CIPodAppControllerPriv::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void CIPodAppControllerPriv::InitComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CIPodAppControllerPriv::DataReceived(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CIPodAppControllerPriv::Acknowledgement(EIPodAppErrors _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CIPodAppControllerPriv::SessionClosed(EIPodAppErrors _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CIPodAppControllerPriv::SessionOpened(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CIPodAppControllerPriv::NotifySessionClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void CIPodAppControllerPriv::AudioSampleRateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CIPodAppControllerPriv::DeviceDisconnected(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
