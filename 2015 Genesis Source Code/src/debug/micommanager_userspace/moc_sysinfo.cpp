/****************************************************************************
** Meta object code from reading C++ file 'sysinfo.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/objects/sysinfo.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sysinfo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SysInfo[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,    9,    8,    8, 0x05,
      59,   55,    8,    8, 0x05,
      94,   55,    8,    8, 0x05,
     125,   55,    8,    8, 0x05,
     160,   55,    8,    8, 0x05,
     196,   55,    8,    8, 0x05,
     230,   55,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
     277,    8,  266,    8, 0x0a,
     308,    8,  266,    8, 0x0a,
     333,    8,  266,    8, 0x0a,
     360,    8,  266,    8, 0x0a,
     383,    8,  266,    8, 0x0a,
     412,    8,  266,    8, 0x0a,
     440,    8,  266,    8, 0x0a,
     470,    8,  266,    8, 0x0a,
     494,    8,  266,    8, 0x0a,
     517,    8,  266,    8, 0x0a,
     541,    8,    8,    8, 0x0a,
     563,   55,    8,    8, 0x0a,
     589,   55,    8,    8, 0x0a,
     620,   55,    8,    8, 0x0a,
     653,   55,    8,    8, 0x0a,
     695,   55,    8,    8, 0x0a,
     737,   55,    8,    8, 0x0a,
     770,   55,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SysInfo[] = {
    "SysInfo\0\0in0,in1\0LgeDBus_EventCarIFStatus(uchar,uchar)\0"
    "in0\0LgeDBus_EventDayNightStatus(uchar)\0"
    "LgeDBus_PopUpEvent(QByteArray)\0"
    "LgeDBus_AudioMixCHCtrl(QByteArray)\0"
    "LgeDBus_AudioMainCHCtrl(QByteArray)\0"
    "LgeDBus_AudioMuteCtrl(QByteArray)\0"
    "LgeDBus_AudioLastMemory(QByteArray)\0"
    "QByteArray\0LgeDBus_AskLanguageInfoValue()\0"
    "LgeDBus_AskSysBootType()\0"
    "LgeDBus_AskSysLastStatus()\0"
    "LgeDBus_AskSysOption()\0"
    "LgeDBus_AskSettingValueMsg()\0"
    "LgeDBus_AskHDRadioVersion()\0"
    "LgeDBus_AskMOST_INICVersion()\0"
    "LgeDBus_AskSysVersion()\0LgeDBus_AskBTMACAddr()\0"
    "LgeDBus_AskAMPVersion()\0LgeDBus_CPUAliveMsg()\0"
    "LgeDBus_EtcGUIMode(uchar)\0"
    "LgeDBus_SetLanguageInfo(uchar)\0"
    "LgeDBus_SetSysOption(QByteArray)\0"
    "LgeDBus_EmergencyConditionMsg(QByteArray)\0"
    "LgeDBus_SettingValueStatusMsg(QByteArray)\0"
    "LgeDBus_SwVersionMsg(QByteArray)\0"
    "LgeDBus_BTMACAddrStore(QByteArray)\0"
};

void SysInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SysInfo *_t = static_cast<SysInfo *>(_o);
        switch (_id) {
        case 0: _t->LgeDBus_EventCarIFStatus((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2]))); break;
        case 1: _t->LgeDBus_EventDayNightStatus((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 2: _t->LgeDBus_PopUpEvent((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->LgeDBus_AudioMixCHCtrl((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: _t->LgeDBus_AudioMainCHCtrl((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 5: _t->LgeDBus_AudioMuteCtrl((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 6: _t->LgeDBus_AudioLastMemory((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 7: { QByteArray _r = _t->LgeDBus_AskLanguageInfoValue();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 8: { QByteArray _r = _t->LgeDBus_AskSysBootType();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 9: { QByteArray _r = _t->LgeDBus_AskSysLastStatus();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 10: { QByteArray _r = _t->LgeDBus_AskSysOption();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 11: { QByteArray _r = _t->LgeDBus_AskSettingValueMsg();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 12: { QByteArray _r = _t->LgeDBus_AskHDRadioVersion();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 13: { QByteArray _r = _t->LgeDBus_AskMOST_INICVersion();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 14: { QByteArray _r = _t->LgeDBus_AskSysVersion();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 15: { QByteArray _r = _t->LgeDBus_AskBTMACAddr();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 16: { QByteArray _r = _t->LgeDBus_AskAMPVersion();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 17: _t->LgeDBus_CPUAliveMsg(); break;
        case 18: _t->LgeDBus_EtcGUIMode((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 19: _t->LgeDBus_SetLanguageInfo((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 20: _t->LgeDBus_SetSysOption((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 21: _t->LgeDBus_EmergencyConditionMsg((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 22: _t->LgeDBus_SettingValueStatusMsg((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 23: _t->LgeDBus_SwVersionMsg((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 24: _t->LgeDBus_BTMACAddrStore((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SysInfo::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SysInfo::staticMetaObject = {
    { &ObjectAbstractInterface::staticMetaObject, qt_meta_stringdata_SysInfo,
      qt_meta_data_SysInfo, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SysInfo::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SysInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SysInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SysInfo))
        return static_cast<void*>(const_cast< SysInfo*>(this));
    if (!strcmp(_clname, "QDBusContext"))
        return static_cast< QDBusContext*>(const_cast< SysInfo*>(this));
    return ObjectAbstractInterface::qt_metacast(_clname);
}

int SysInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ObjectAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void SysInfo::LgeDBus_EventCarIFStatus(uchar _t1, uchar _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SysInfo::LgeDBus_EventDayNightStatus(uchar _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SysInfo::LgeDBus_PopUpEvent(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SysInfo::LgeDBus_AudioMixCHCtrl(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SysInfo::LgeDBus_AudioMainCHCtrl(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SysInfo::LgeDBus_AudioMuteCtrl(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void SysInfo::LgeDBus_AudioLastMemory(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
