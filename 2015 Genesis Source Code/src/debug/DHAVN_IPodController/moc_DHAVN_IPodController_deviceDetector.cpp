/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_deviceDetector.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "detector/include/DHAVN_IPodController_deviceDetector.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_deviceDetector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CDeviceDetector[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      36,   17,   16,   16, 0x05,
      65,   16,   16,   16, 0x05,
      90,   16,   16,   16, 0x05,
     149,  128,   16,   16, 0x05,
     190,  179,   16,   16, 0x05,
     226,  223,   16,   16, 0x05,
     282,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     303,   16,   16,   16, 0x0a,
     333,  328,   16,   16, 0x0a,
     364,  328,   16,   16, 0x0a,
     397,   16,   16,   16, 0x0a,
     429,   16,   16,   16, 0x0a,
     450,  223,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CDeviceDetector[] = {
    "CDeviceDetector\0\0vendorId,productId\0"
    "iPodDeviceDetetcted(int,int)\0"
    "iPodDeviceDisconnected()\0"
    "IPodConnectStateForBootupSignal(bool)\0"
    "status,hidDevicePath\0iPodHidDetected(bool,QString)\0"
    "deviceName\0iPodAudioDeviceDetected(QString)\0"
    ",,\0onIPodDeviceConnectedSignal(QString,QString,EIPodPorts)\0"
    "destroyTaskHandler()\0iPodDeviceDetectedSlot()\0"
    "path\0hidDeviceDetetcedSlot(QString)\0"
    "audioDeviceDetetcedSlot(QString)\0"
    "startEnumerationAndSetDevices()\0"
    "requestDestroyTask()\0"
    "onIPodAudioDeviceConnectedSLOT(QString,QString,EIPodPorts)\0"
};

void CDeviceDetector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CDeviceDetector *_t = static_cast<CDeviceDetector *>(_o);
        switch (_id) {
        case 0: _t->iPodDeviceDetetcted((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->iPodDeviceDisconnected(); break;
        case 2: _t->IPodConnectStateForBootupSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->iPodHidDetected((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->iPodAudioDeviceDetected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->onIPodDeviceConnectedSignal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< EIPodPorts(*)>(_a[3]))); break;
        case 6: _t->destroyTaskHandler(); break;
        case 7: _t->iPodDeviceDetectedSlot(); break;
        case 8: _t->hidDeviceDetetcedSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->audioDeviceDetetcedSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->startEnumerationAndSetDevices(); break;
        case 11: _t->requestDestroyTask(); break;
        case 12: _t->onIPodAudioDeviceConnectedSLOT((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< EIPodPorts(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CDeviceDetector::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CDeviceDetector::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CDeviceDetector,
      qt_meta_data_CDeviceDetector, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CDeviceDetector::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CDeviceDetector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CDeviceDetector::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDeviceDetector))
        return static_cast<void*>(const_cast< CDeviceDetector*>(this));
    return QObject::qt_metacast(_clname);
}

int CDeviceDetector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void CDeviceDetector::iPodDeviceDetetcted(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CDeviceDetector::iPodDeviceDisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CDeviceDetector::IPodConnectStateForBootupSignal(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CDeviceDetector::iPodHidDetected(bool _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CDeviceDetector::iPodAudioDeviceDetected(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CDeviceDetector::onIPodDeviceConnectedSignal(QString _t1, QString _t2, EIPodPorts _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CDeviceDetector::destroyTaskHandler()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}
QT_END_MOC_NAMESPACE
