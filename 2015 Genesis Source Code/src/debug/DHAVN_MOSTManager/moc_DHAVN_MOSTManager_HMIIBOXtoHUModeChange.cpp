/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_MOSTManager_HMIIBOXtoHUModeChange.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_MOSTManager_HMIIBOXtoHUModeChange.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_MOSTManager_HMIIBOXtoHUModeChange.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CModeStatusCustomClass[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      36,   24,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
      66,   24,   23,   23, 0x0a,
     153,  100,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CModeStatusCustomClass[] = {
    "CModeStatusCustomClass\0\0target,mode\0"
    "ModeStatusfromIBOX(uint,uint)\0"
    "ModeStatusfromIBOXSlot(uint,uint)\0"
    "flags,messageID,sourceDeviceID,payloadLength,payload\0"
    "SetOperation(uint32_t,uint32_t,DeviceID_t,uint32_t,const uint8_t*)\0"
};

void CModeStatusCustomClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CModeStatusCustomClass *_t = static_cast<CModeStatusCustomClass *>(_o);
        switch (_id) {
        case 0: _t->ModeStatusfromIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 1: _t->ModeStatusfromIBOXSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 2: _t->SetOperation((*reinterpret_cast< uint32_t(*)>(_a[1])),(*reinterpret_cast< uint32_t(*)>(_a[2])),(*reinterpret_cast< DeviceID_t(*)>(_a[3])),(*reinterpret_cast< uint32_t(*)>(_a[4])),(*reinterpret_cast< const uint8_t*(*)>(_a[5]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CModeStatusCustomClass::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CModeStatusCustomClass::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CModeStatusCustomClass,
      qt_meta_data_CModeStatusCustomClass, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CModeStatusCustomClass::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CModeStatusCustomClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CModeStatusCustomClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CModeStatusCustomClass))
        return static_cast<void*>(const_cast< CModeStatusCustomClass*>(this));
    if (!strcmp(_clname, "k2l::acs::protocols::most::CMOSTFunctionClassArray"))
        return static_cast< k2l::acs::protocols::most::CMOSTFunctionClassArray*>(const_cast< CModeStatusCustomClass*>(this));
    return QObject::qt_metacast(_clname);
}

int CModeStatusCustomClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CModeStatusCustomClass::ModeStatusfromIBOX(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_CSystemVisibilityCustomClass[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      44,   30,   29,   29, 0x05,

 // slots: signature, parameters, type, tag, flags
      80,   30,   29,   29, 0x0a,
     173,  120,   29,   29, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CSystemVisibilityCustomClass[] = {
    "CSystemVisibilityCustomClass\0\0"
    "target,source\0SystemVisibilityfromIBOX(uint,uint)\0"
    "SystemVisibilityfromIBOXSlot(uint,uint)\0"
    "flags,messageID,sourceDeviceID,payloadLength,payload\0"
    "SetOperation(uint32_t,uint32_t,DeviceID_t,uint32_t,const uint8_t*)\0"
};

void CSystemVisibilityCustomClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CSystemVisibilityCustomClass *_t = static_cast<CSystemVisibilityCustomClass *>(_o);
        switch (_id) {
        case 0: _t->SystemVisibilityfromIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 1: _t->SystemVisibilityfromIBOXSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 2: _t->SetOperation((*reinterpret_cast< uint32_t(*)>(_a[1])),(*reinterpret_cast< uint32_t(*)>(_a[2])),(*reinterpret_cast< DeviceID_t(*)>(_a[3])),(*reinterpret_cast< uint32_t(*)>(_a[4])),(*reinterpret_cast< const uint8_t*(*)>(_a[5]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CSystemVisibilityCustomClass::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CSystemVisibilityCustomClass::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CSystemVisibilityCustomClass,
      qt_meta_data_CSystemVisibilityCustomClass, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CSystemVisibilityCustomClass::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CSystemVisibilityCustomClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CSystemVisibilityCustomClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSystemVisibilityCustomClass))
        return static_cast<void*>(const_cast< CSystemVisibilityCustomClass*>(this));
    if (!strcmp(_clname, "k2l::acs::protocols::most::CMOSTFunctionClassArray"))
        return static_cast< k2l::acs::protocols::most::CMOSTFunctionClassArray*>(const_cast< CSystemVisibilityCustomClass*>(this));
    return QObject::qt_metacast(_clname);
}

int CSystemVisibilityCustomClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CSystemVisibilityCustomClass::SystemVisibilityfromIBOX(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_CSystemAudibilityCustomClass[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      44,   30,   29,   29, 0x05,

 // slots: signature, parameters, type, tag, flags
      80,   30,   29,   29, 0x0a,
     173,  120,   29,   29, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CSystemAudibilityCustomClass[] = {
    "CSystemAudibilityCustomClass\0\0"
    "target,source\0SystemAudibilityfromIBOX(uint,uint)\0"
    "SystemAudibilityfromIBOXSlot(uint,uint)\0"
    "flags,messageID,sourceDeviceID,payloadLength,payload\0"
    "SetOperation(uint32_t,uint32_t,DeviceID_t,uint32_t,const uint8_t*)\0"
};

void CSystemAudibilityCustomClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CSystemAudibilityCustomClass *_t = static_cast<CSystemAudibilityCustomClass *>(_o);
        switch (_id) {
        case 0: _t->SystemAudibilityfromIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 1: _t->SystemAudibilityfromIBOXSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 2: _t->SetOperation((*reinterpret_cast< uint32_t(*)>(_a[1])),(*reinterpret_cast< uint32_t(*)>(_a[2])),(*reinterpret_cast< DeviceID_t(*)>(_a[3])),(*reinterpret_cast< uint32_t(*)>(_a[4])),(*reinterpret_cast< const uint8_t*(*)>(_a[5]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CSystemAudibilityCustomClass::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CSystemAudibilityCustomClass::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CSystemAudibilityCustomClass,
      qt_meta_data_CSystemAudibilityCustomClass, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CSystemAudibilityCustomClass::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CSystemAudibilityCustomClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CSystemAudibilityCustomClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSystemAudibilityCustomClass))
        return static_cast<void*>(const_cast< CSystemAudibilityCustomClass*>(this));
    if (!strcmp(_clname, "k2l::acs::protocols::most::CMOSTFunctionClassArray"))
        return static_cast< k2l::acs::protocols::most::CMOSTFunctionClassArray*>(const_cast< CSystemAudibilityCustomClass*>(this));
    return QObject::qt_metacast(_clname);
}

int CSystemAudibilityCustomClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CSystemAudibilityCustomClass::SystemAudibilityfromIBOX(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
