/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_PndrDeviceManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_PndrDeviceManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_PndrDeviceManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPndrDeviceManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   20,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      60,   20,   19,   19, 0x0a,
      94,   19,   19,   19, 0x0a,
     106,   19,   19,   19, 0x0a,
     126,   19,   19,   19, 0x0a,
     161,  145,   19,   19, 0x0a,
     196,  186,   19,   19, 0x0a,
     236,   19,   19,   19, 0x08,
     265,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CPndrDeviceManager[] = {
    "CPndrDeviceManager\0\0success\0"
    "ApplicationLaunched(EPndrError)\0"
    "onApplicationLaunched(EPndrError)\0"
    "onTimeout()\0sessionTimerStart()\0"
    "sessionTimerStop()\0inMaxFrameDrops\0"
    "onSetMaxFrameDrops(bool)\0,inStatus\0"
    "onDeviceStatusUpdated(EDeviceType,bool)\0"
    "sendSessionStartAfterDelay()\0"
    "connectionTimedOut()\0"
};

void CPndrDeviceManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CPndrDeviceManager *_t = static_cast<CPndrDeviceManager *>(_o);
        switch (_id) {
        case 0: _t->ApplicationLaunched((*reinterpret_cast< EPndrError(*)>(_a[1]))); break;
        case 1: _t->onApplicationLaunched((*reinterpret_cast< EPndrError(*)>(_a[1]))); break;
        case 2: _t->onTimeout(); break;
        case 3: _t->sessionTimerStart(); break;
        case 4: _t->sessionTimerStop(); break;
        case 5: _t->onSetMaxFrameDrops((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->onDeviceStatusUpdated((*reinterpret_cast< const EDeviceType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 7: _t->sendSessionStartAfterDelay(); break;
        case 8: _t->connectionTimedOut(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CPndrDeviceManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CPndrDeviceManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CPndrDeviceManager,
      qt_meta_data_CPndrDeviceManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CPndrDeviceManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CPndrDeviceManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CPndrDeviceManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPndrDeviceManager))
        return static_cast<void*>(const_cast< CPndrDeviceManager*>(this));
    return QObject::qt_metacast(_clname);
}

int CPndrDeviceManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CPndrDeviceManager::ApplicationLaunched(EPndrError _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
