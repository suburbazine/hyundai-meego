/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_MOSTManager_MOSTManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_MOSTManager_MOSTManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_MOSTManager_MOSTManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CMOSTManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      31,   13,   13,   13, 0x08,
      49,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CMOSTManager[] = {
    "CMOSTManager\0\0onDriverLoaded()\0"
    "onDriverCrashed()\0onMHPCrashed()\0"
};

void CMOSTManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CMOSTManager *_t = static_cast<CMOSTManager *>(_o);
        switch (_id) {
        case 0: _t->onDriverLoaded(); break;
        case 1: _t->onDriverCrashed(); break;
        case 2: _t->onMHPCrashed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CMOSTManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CMOSTManager::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CMOSTManager,
      qt_meta_data_CMOSTManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CMOSTManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CMOSTManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CMOSTManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CMOSTManager))
        return static_cast<void*>(const_cast< CMOSTManager*>(this));
    if (!strcmp(_clname, "CIACSApplicationEvents"))
        return static_cast< CIACSApplicationEvents*>(const_cast< CMOSTManager*>(this));
    return QThread::qt_metacast(_clname);
}

int CMOSTManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
static const uint qt_meta_data_CAdaptorThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_CAdaptorThread[] = {
    "CAdaptorThread\0"
};

void CAdaptorThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CAdaptorThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAdaptorThread::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CAdaptorThread,
      qt_meta_data_CAdaptorThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAdaptorThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAdaptorThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAdaptorThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAdaptorThread))
        return static_cast<void*>(const_cast< CAdaptorThread*>(this));
    return QObject::qt_metacast(_clname);
}

int CAdaptorThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_CMOSTDriver[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      28,   12,   12,   12, 0x05,
      44,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      62,   57,   12,   12, 0x08,
      85,   12,   12,   12, 0x08,
     102,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CMOSTDriver[] = {
    "CMOSTDriver\0\0DriverLoaded()\0DriverCrashed()\0"
    "MHPCrashed()\0path\0onFileChanged(QString)\0"
    "onTimerTimeout()\0onDaemonWatherTimeout()\0"
};

void CMOSTDriver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CMOSTDriver *_t = static_cast<CMOSTDriver *>(_o);
        switch (_id) {
        case 0: _t->DriverLoaded(); break;
        case 1: _t->DriverCrashed(); break;
        case 2: _t->MHPCrashed(); break;
        case 3: _t->onFileChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->onTimerTimeout(); break;
        case 5: _t->onDaemonWatherTimeout(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CMOSTDriver::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CMOSTDriver::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CMOSTDriver,
      qt_meta_data_CMOSTDriver, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CMOSTDriver::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CMOSTDriver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CMOSTDriver::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CMOSTDriver))
        return static_cast<void*>(const_cast< CMOSTDriver*>(this));
    return QObject::qt_metacast(_clname);
}

int CMOSTDriver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CMOSTDriver::DriverLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CMOSTDriver::DriverCrashed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CMOSTDriver::MHPCrashed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
