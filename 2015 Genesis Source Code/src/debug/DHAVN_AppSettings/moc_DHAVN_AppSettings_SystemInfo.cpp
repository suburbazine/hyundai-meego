/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppSettings_SystemInfo.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppSettings_SystemInfo.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppSettings_SystemInfo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SystemInfo[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   14, // methods
      10,  169, // properties
       1,  209, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      26,   11,   11,   11, 0x05,
      44,   11,   11,   11, 0x05,
      66,   11,   11,   11, 0x05,
      85,   11,   11,   11, 0x05,
     104,   11,   11,   11, 0x05,
     124,   11,   11,   11, 0x05,
     144,   11,   11,   11, 0x05,
     172,   11,   11,   11, 0x05,
     199,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     224,  220,   11,   11, 0x0a,
     242,  220,   11,   11, 0x0a,
     264,  220,   11,   11, 0x0a,
     290,  220,   11,   11, 0x0a,
     313,  220,   11,   11, 0x0a,
     336,  220,   11,   11, 0x0a,
     366,  220,   11,   11, 0x0a,
     398,   11,  390,   11, 0x0a,
     420,   11,  390,   11, 0x0a,
     441,  220,   11,   11, 0x0a,
     479,  473,   11,   11, 0x0a,
     510,   11,  390,   11, 0x0a,
     525,  473,   11,   11, 0x0a,
     550,   11,   11,   11, 0x0a,
     568,   11,   11,   11, 0x0a,
     589,  585,   11,   11, 0x0a,
     609,  585,   11,   11, 0x0a,
     628,   11,   11,   11, 0x0a,
     653,  645,   11,   11, 0x08,
     685,   11,   11,   11, 0x08,

 // methods: signature, parameters, type, tag, flags
     701,   11,  390,   11, 0x02,

 // properties: name, type, flags
     712,  390, 0x0a495103,
     718,  390, 0x0a495103,
     728,  390, 0x0a495103,
     742,  390, 0x0a495103,
     753,  390, 0x0a495103,
     764,  390, 0x0a495003,
     776,  390, 0x0a495103,
     788,  390, 0x0a495103,
     808,  390, 0x0a495103,
     827,  390, 0x0a495103,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       5,
       6,
       7,
       8,
       9,

 // enums: name, flags, count, data
     840, 0x0,    7,  213,

 // enum data: key, value
     858, uint(SystemInfo::SYSTEM_INFO_MODEL),
     876, uint(SystemInfo::SYSTEM_INFO_SW_VERSION),
     899, uint(SystemInfo::SYSTEM_INFO_KERNEL_VERSION),
     926, uint(SystemInfo::SYSTEM_INFO_MICOM_VERSION),
     952, uint(SystemInfo::SYSTEM_INFO_MAP_VERSION),
     976, uint(SystemInfo::SYSTEM_INFO_ASSOCIATION_NUM),
    1004, uint(SystemInfo::SYSTEM_INFO_IBOX_VERSION),

       0        // eod
};

static const char qt_meta_stringdata_SystemInfo[] = {
    "SystemInfo\0\0notifyModel()\0notifySWVersion()\0"
    "notifyKernelVersion()\0notifyMapVersion()\0"
    "notifyAppVersion()\0notifyAssociation()\0"
    "notifyIBoxVersion()\0notifyGraceNoteSDKVersion()\0"
    "notifyGraceNoteDBVersion()\0"
    "notifyBTMacAddress()\0str\0setModel(QString)\0"
    "setSWVersion(QString)\0setKernelVersion(QString)\0"
    "setMapVersion(QString)\0setAppVersion(QString)\0"
    "setAssociationNumber(QString)\0"
    "setIBoxVersion(QString)\0QString\0"
    "GraceNoteSDKVersion()\0GraceNoteDBVersion()\0"
    "setGraceNoteSDKVersion(QString)\0value\0"
    "setGraceNoteDBVersion(QString)\0"
    "BTMacAddress()\0setBTMacAddress(QString)\0"
    "RequestGNSDKVer()\0RequestGNDBVer()\0"
    "ver\0onGNSDKVer(QString)\0onGNDBVer(QString)\0"
    "updateNaviInfo()\0version\0"
    "iboxVersionUpdated(QStringList)\0"
    "clearNaviInfo()\0GetModel()\0Model\0"
    "SWVersion\0KernelVersion\0MapVersion\0"
    "AppVersion\0Association\0IBoxVersion\0"
    "GraceNoteSDKVersion\0GraceNoteDBVersion\0"
    "BTMacAddress\0SYSTEM_INFO_TYPES\0"
    "SYSTEM_INFO_MODEL\0SYSTEM_INFO_SW_VERSION\0"
    "SYSTEM_INFO_KERNEL_VERSION\0"
    "SYSTEM_INFO_MICOM_VERSION\0"
    "SYSTEM_INFO_MAP_VERSION\0"
    "SYSTEM_INFO_ASSOCIATION_NUM\0"
    "SYSTEM_INFO_IBOX_VERSION\0"
};

void SystemInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SystemInfo *_t = static_cast<SystemInfo *>(_o);
        switch (_id) {
        case 0: _t->notifyModel(); break;
        case 1: _t->notifySWVersion(); break;
        case 2: _t->notifyKernelVersion(); break;
        case 3: _t->notifyMapVersion(); break;
        case 4: _t->notifyAppVersion(); break;
        case 5: _t->notifyAssociation(); break;
        case 6: _t->notifyIBoxVersion(); break;
        case 7: _t->notifyGraceNoteSDKVersion(); break;
        case 8: _t->notifyGraceNoteDBVersion(); break;
        case 9: _t->notifyBTMacAddress(); break;
        case 10: _t->setModel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->setSWVersion((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->setKernelVersion((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->setMapVersion((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: _t->setAppVersion((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->setAssociationNumber((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->setIBoxVersion((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: { QString _r = _t->GraceNoteSDKVersion();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 18: { QString _r = _t->GraceNoteDBVersion();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 19: _t->setGraceNoteSDKVersion((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: _t->setGraceNoteDBVersion((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: { QString _r = _t->BTMacAddress();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 22: _t->setBTMacAddress((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->RequestGNSDKVer(); break;
        case 24: _t->RequestGNDBVer(); break;
        case 25: _t->onGNSDKVer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 26: _t->onGNDBVer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 27: _t->updateNaviInfo(); break;
        case 28: _t->iboxVersionUpdated((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 29: _t->clearNaviInfo(); break;
        case 30: { QString _r = _t->GetModel();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SystemInfo::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SystemInfo::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SystemInfo,
      qt_meta_data_SystemInfo, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SystemInfo::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SystemInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SystemInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SystemInfo))
        return static_cast<void*>(const_cast< SystemInfo*>(this));
    return QObject::qt_metacast(_clname);
}

int SystemInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = GetModel(); break;
        case 1: *reinterpret_cast< QString*>(_v) = GetSWVersion(); break;
        case 2: *reinterpret_cast< QString*>(_v) = GetKernelVersion(); break;
        case 3: *reinterpret_cast< QString*>(_v) = GetMapVersion(); break;
        case 4: *reinterpret_cast< QString*>(_v) = GetAppVersion(); break;
        case 5: *reinterpret_cast< QString*>(_v) = GetAssociationNumber(); break;
        case 6: *reinterpret_cast< QString*>(_v) = GetIBoxVersion(); break;
        case 7: *reinterpret_cast< QString*>(_v) = GraceNoteSDKVersion(); break;
        case 8: *reinterpret_cast< QString*>(_v) = GraceNoteDBVersion(); break;
        case 9: *reinterpret_cast< QString*>(_v) = BTMacAddress(); break;
        }
        _id -= 10;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setModel(*reinterpret_cast< QString*>(_v)); break;
        case 1: setSWVersion(*reinterpret_cast< QString*>(_v)); break;
        case 2: setKernelVersion(*reinterpret_cast< QString*>(_v)); break;
        case 3: setMapVersion(*reinterpret_cast< QString*>(_v)); break;
        case 4: setAppVersion(*reinterpret_cast< QString*>(_v)); break;
        case 5: setAssociationNumber(*reinterpret_cast< QString*>(_v)); break;
        case 6: setIBoxVersion(*reinterpret_cast< QString*>(_v)); break;
        case 7: setGraceNoteSDKVersion(*reinterpret_cast< QString*>(_v)); break;
        case 8: setGraceNoteDBVersion(*reinterpret_cast< QString*>(_v)); break;
        case 9: setBTMacAddress(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 10;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 10;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void SystemInfo::notifyModel()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void SystemInfo::notifySWVersion()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void SystemInfo::notifyKernelVersion()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void SystemInfo::notifyMapVersion()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void SystemInfo::notifyAppVersion()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void SystemInfo::notifyAssociation()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void SystemInfo::notifyIBoxVersion()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void SystemInfo::notifyGraceNoteSDKVersion()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void SystemInfo::notifyGraceNoteDBVersion()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void SystemInfo::notifyBTMacAddress()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}
QT_END_MOC_NAMESPACE
