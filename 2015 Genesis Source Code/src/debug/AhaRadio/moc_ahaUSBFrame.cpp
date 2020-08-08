/****************************************************************************
** Meta object code from reading C++ file 'ahaUSBFrame.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ahaUSBFrame.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ahaUSBFrame.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CAhaUSBFrameTxRx[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   17,   18,   17, 0x0a,
      69,   58,   18,   17, 0x0a,
      95,   17,   18,   17, 0x0a,
     131,  119,   18,   17, 0x0a,
     165,  161,   18,   17, 0x0a,
     211,  197,   18,   17, 0x0a,
     239,  232,   18,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CAhaUSBFrameTxRx[] = {
    "CAhaUSBFrameTxRx\0\0int\0"
    "onIPodAppControllerInitCompletion()\0"
    "ipodNumber\0onDeviceDisconnected(int)\0"
    "onNotifySessionClosed()\0nSampleRate\0"
    "onAudioSampleRateChanged(int)\0err\0"
    "onSessionClosed(EIPodAppErrors)\0"
    "protocolIndex\0onSessionOpened(int)\0"
    "status\0onAcknowledgement(EIPodAppErrors)\0"
};

void CAhaUSBFrameTxRx::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CAhaUSBFrameTxRx *_t = static_cast<CAhaUSBFrameTxRx *>(_o);
        switch (_id) {
        case 0: { int _r = _t->onIPodAppControllerInitCompletion();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: { int _r = _t->onDeviceDisconnected((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: { int _r = _t->onNotifySessionClosed();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->onAudioSampleRateChanged((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { int _r = _t->onSessionClosed((*reinterpret_cast< EIPodAppErrors(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 5: { int _r = _t->onSessionOpened((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 6: { int _r = _t->onAcknowledgement((*reinterpret_cast< EIPodAppErrors(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CAhaUSBFrameTxRx::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAhaUSBFrameTxRx::staticMetaObject = {
    { &CAhaAbsFrameTxRx::staticMetaObject, qt_meta_stringdata_CAhaUSBFrameTxRx,
      qt_meta_data_CAhaUSBFrameTxRx, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAhaUSBFrameTxRx::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAhaUSBFrameTxRx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAhaUSBFrameTxRx::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAhaUSBFrameTxRx))
        return static_cast<void*>(const_cast< CAhaUSBFrameTxRx*>(this));
    return CAhaAbsFrameTxRx::qt_metacast(_clname);
}

int CAhaUSBFrameTxRx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CAhaAbsFrameTxRx::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
