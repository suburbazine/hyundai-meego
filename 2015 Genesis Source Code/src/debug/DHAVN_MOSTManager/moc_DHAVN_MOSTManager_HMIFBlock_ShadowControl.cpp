/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_MOSTManager_HMIFBlock_ShadowControl.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_MOSTManager_HMIFBlock_ShadowControl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_MOSTManager_HMIFBlock_ShadowControl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CHMIFblockStateListener[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CHMIFblockStateListener[] = {
    "CHMIFblockStateListener\0\0OnWaitForTimeout()\0"
};

void CHMIFblockStateListener::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CHMIFblockStateListener *_t = static_cast<CHMIFblockStateListener *>(_o);
        switch (_id) {
        case 0: _t->OnWaitForTimeout(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CHMIFblockStateListener::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CHMIFblockStateListener::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CHMIFblockStateListener,
      qt_meta_data_CHMIFblockStateListener, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CHMIFblockStateListener::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CHMIFblockStateListener::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CHMIFblockStateListener::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CHMIFblockStateListener))
        return static_cast<void*>(const_cast< CHMIFblockStateListener*>(this));
    if (!strcmp(_clname, "IACSMOSTEventListener"))
        return static_cast< IACSMOSTEventListener*>(const_cast< CHMIFblockStateListener*>(this));
    return QObject::qt_metacast(_clname);
}

int CHMIFblockStateListener::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_CHMIFBlockShadowControl[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      24,       // signalCount

 // signals: signature, parameters, type, tag, flags
      37,   25,   24,   24, 0x05,
      83,   63,   24,   24, 0x05,
     133,  120,   24,   24, 0x05,
     170,  163,   24,   24, 0x05,
     205,  188,   24,   24, 0x05,
     249,  236,   24,   24, 0x05,
     295,  279,   24,   24, 0x05,
     340,  329,   24,   24, 0x05,
     375,  365,   24,   24, 0x05,
     406,  395,   24,   24, 0x05,
     443,  427,   24,   24, 0x05,
     474,  469,   24,   24, 0x05,
     540,  505,   24,   24, 0x05,
     613,  578,   24,   24, 0x05,
     665,  652,   24,   24, 0x05,
     697,  691,   24,   24, 0x05,
     735,  721,   24,   24, 0x05,
     794,  764,   24,   24, 0x05,
     855,  836,   24,   24, 0x05,
     901,  889,   24,   24, 0x05,
     962,  927,   24,   24, 0x05,
    1001,  995,   24,   24, 0x05,
    1070, 1031,   24,   24, 0x05,
    1127,  163,   24,   24, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CHMIFBlockShadowControl[] = {
    "CHMIFBlockShadowControl\0\0target,mode\0"
    "ModeStatusIBOX(uint,uint)\0errorCode,errorInfo\0"
    "ModeStatusErrorIBOX(uint,QByteArray)\0"
    "keyBoardData\0KeyBoardSetting(QDBusVariant)\0"
    "status\0Progressbar(uint)\0popUpRequestData\0"
    "PopUpRequestToHU(QDBusVariant)\0"
    "senderHandle\0AbortPopUpRequestToHU(ushort)\0"
    "popUpResultData\0PopUpResultFromIBOX(QDBusVariant)\0"
    "displayOSD\0DisplayOSD(QDBusVariant)\0"
    "statusbar\0ShowStatusBar(uint)\0controller\0"
    "LockController(uint)\0hardKey,softKey\0"
    "KeyInputStatus(uint,uint)\0need\0"
    "DrivingRestrictionStatus(uint)\0"
    "brightness,saturation,contrast,hue\0"
    "AdjustScreenFromIBOX(int,int,int,int)\0"
    "bTarget,lcdFront,lcdRear1,lcdRear2\0"
    "LCDBrightnessFromIBOX(int,int,int,int)\0"
    "illumination\0IlluminationFromIBOX(int)\0"
    "ratio\0VideoRatioFromIBOX(int)\0"
    "displayStatus\0DisplayControlFromIBOX(uint)\0"
    "deviceID,senderHandle,sTarget\0"
    "VideoDefaultRequestIBOX(uint,ushort,uint)\0"
    "state,senderHandle\0VideoDefaultResultIBOX(uint,uint)\0"
    "beepSetting\0BeepSettingFromIBOX(uint)\0"
    "emgcState,controllerLock,soundLock\0"
    "EmergencySetIBOX(uint,uint,uint)\0state\0"
    "HMIFunctionBlockVisible(bool)\0"
    "state,senderHandle,errorCode,errorInfo\0"
    "SystemDefaultSetResult(uint,uint16_t,uint8_t,QByteArray)\0"
    "MuteStatusSet(bool)\0"
};

void CHMIFBlockShadowControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CHMIFBlockShadowControl *_t = static_cast<CHMIFBlockShadowControl *>(_o);
        switch (_id) {
        case 0: _t->ModeStatusIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 1: _t->ModeStatusErrorIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 2: _t->KeyBoardSetting((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 3: _t->Progressbar((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 4: _t->PopUpRequestToHU((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 5: _t->AbortPopUpRequestToHU((*reinterpret_cast< ushort(*)>(_a[1]))); break;
        case 6: _t->PopUpResultFromIBOX((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 7: _t->DisplayOSD((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 8: _t->ShowStatusBar((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 9: _t->LockController((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 10: _t->KeyInputStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 11: _t->DrivingRestrictionStatus((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 12: _t->AdjustScreenFromIBOX((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 13: _t->LCDBrightnessFromIBOX((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 14: _t->IlluminationFromIBOX((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 15: _t->VideoRatioFromIBOX((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 16: _t->DisplayControlFromIBOX((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 17: _t->VideoDefaultRequestIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 18: _t->VideoDefaultResultIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 19: _t->BeepSettingFromIBOX((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 20: _t->EmergencySetIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 21: _t->HMIFunctionBlockVisible((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 22: _t->SystemDefaultSetResult((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint16_t(*)>(_a[2])),(*reinterpret_cast< const uint8_t(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 23: _t->MuteStatusSet((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CHMIFBlockShadowControl::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CHMIFBlockShadowControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CHMIFBlockShadowControl,
      qt_meta_data_CHMIFBlockShadowControl, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CHMIFBlockShadowControl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CHMIFBlockShadowControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CHMIFBlockShadowControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CHMIFBlockShadowControl))
        return static_cast<void*>(const_cast< CHMIFBlockShadowControl*>(this));
    if (!strcmp(_clname, "CControlThreadBase"))
        return static_cast< CControlThreadBase*>(const_cast< CHMIFBlockShadowControl*>(this));
    return QObject::qt_metacast(_clname);
}

int CHMIFBlockShadowControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    return _id;
}

// SIGNAL 0
void CHMIFBlockShadowControl::ModeStatusIBOX(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CHMIFBlockShadowControl::ModeStatusErrorIBOX(const uint _t1, const QByteArray _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CHMIFBlockShadowControl::KeyBoardSetting(const QDBusVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CHMIFBlockShadowControl::Progressbar(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CHMIFBlockShadowControl::PopUpRequestToHU(const QDBusVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CHMIFBlockShadowControl::AbortPopUpRequestToHU(ushort _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CHMIFBlockShadowControl::PopUpResultFromIBOX(const QDBusVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CHMIFBlockShadowControl::DisplayOSD(const QDBusVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CHMIFBlockShadowControl::ShowStatusBar(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CHMIFBlockShadowControl::LockController(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CHMIFBlockShadowControl::KeyInputStatus(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CHMIFBlockShadowControl::DrivingRestrictionStatus(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CHMIFBlockShadowControl::AdjustScreenFromIBOX(const int _t1, const int _t2, const int _t3, const int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CHMIFBlockShadowControl::LCDBrightnessFromIBOX(const int _t1, const int _t2, const int _t3, const int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CHMIFBlockShadowControl::IlluminationFromIBOX(const int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CHMIFBlockShadowControl::VideoRatioFromIBOX(const int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void CHMIFBlockShadowControl::DisplayControlFromIBOX(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void CHMIFBlockShadowControl::VideoDefaultRequestIBOX(const uint _t1, const ushort _t2, const uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void CHMIFBlockShadowControl::VideoDefaultResultIBOX(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void CHMIFBlockShadowControl::BeepSettingFromIBOX(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void CHMIFBlockShadowControl::EmergencySetIBOX(const uint _t1, const uint _t2, const uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void CHMIFBlockShadowControl::HMIFunctionBlockVisible(const bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void CHMIFBlockShadowControl::SystemDefaultSetResult(const uint _t1, const uint16_t _t2, const uint8_t _t3, const QByteArray _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void CHMIFBlockShadowControl::MuteStatusSet(const bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}
QT_END_MOC_NAMESPACE
