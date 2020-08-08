/****************************************************************************
** Meta object code from reading C++ file 'sysinfo_adaptor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/adaptors/sysinfo_adaptor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sysinfo_adaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SysInfoAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       2,   14, // classinfo
      25,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // classinfo: key, value
      41,   15,
    3665,   57,

 // signals: signature, parameters, type, tag, flags
    3690, 3686, 3685, 3685, 0x05,
    3726, 3686, 3685, 3685, 0x05,
    3762, 3686, 3685, 3685, 0x05,
    3797, 3686, 3685, 3685, 0x05,
    3839, 3831, 3685, 3685, 0x05,
    3877, 3686, 3685, 3685, 0x05,
    3912, 3686, 3685, 3685, 0x05,

 // slots: signature, parameters, type, tag, flags
    3954, 3685, 3943, 3685, 0x0a,
    3978, 3685, 3943, 3685, 0x0a,
    4001, 3685, 3943, 3685, 0x0a,
    4029, 3685, 3943, 3685, 0x0a,
    4060, 3685, 3943, 3685, 0x0a,
    4090, 3685, 3943, 3685, 0x0a,
    4119, 3685, 3943, 3685, 0x0a,
    4144, 3685, 3943, 3685, 0x0a,
    4171, 3685, 3943, 3685, 0x0a,
    4194, 3685, 3943, 3685, 0x0a,
    4218, 3686, 3685, 3685, 0x0a,
    4253, 3685, 3685, 3685, 0x0a,
    4275, 3686, 3685, 3685, 0x0a,
    4317, 3686, 3685, 3685, 0x0a,
    4343, 3686, 3685, 3685, 0x0a,
    4374, 3686, 3685, 3685, 0x0a,
    4407, 3686, 3685, 3685, 0x0a,
    4449, 3686, 3685, 3685, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SysInfoAdaptor[] = {
    "SysInfoAdaptor\0com.lge.car.micom.sysinfo\0"
    "D-Bus Interface\0"
    "  <interface name=\"com.lge.car.micom.sysinfo\">\n    <method name=\"L"
    "geDBus_AskSysBootType\">\n      <!-- 0x201, Reponse:0x8201, v1.3_20111"
    "003(out:4bytes) -->\n      <arg direction=\"out\" type=\"ay\"/>\n    <"
    "/method>\n    <method name=\"LgeDBus_AskSysLastStatus\">\n      <!-- 0"
    "x0202, Response:0x8202, v1.3_20111003(out:3bytes) -->\n      <arg dire"
    "ction=\"out\" type=\"ay\"/>\n    </method>\n    <method name=\"LgeDBus"
    "_SetSysOption\">\n      <!-- 0x0193, v1.3_20111003(in:9bytes) -->\n   "
    "   <arg direction=\"in\" type=\"ay\"/>\n    </method>\n    <method nam"
    "e=\"LgeDBus_EtcGUIMode\">\n      <!-- 0x0190, (in:1bytes) -->\n      <"
    "arg direction=\"in\" type=\"y\"/>\n    </method>\n    <method name=\"L"
    "geDBus_AskSysVersion\">\n      <!-- 0x0203, Response:0x8203, v1.3_2011"
    "1003(out:16bytes) -->\n      <arg direction=\"out\" type=\"ay\"/>\n   "
    " </method>\n    <method name=\"LgeDBus_SetLanguageInfo\">\n      <!-- "
    "0x0197, v?.?_20120919 -->\n      <arg direction=\"in\" type=\"y\"/>\n "
    "   </method>\n    <method name=\"LgeDBus_AskLanguageInfoValue\">\n    "
    "  <!-- 0x0293, Response:0x8293, v?.?_20120919(out:1ytes) -->\n      <a"
    "rg direction=\"out\" type=\"ay\"/>\n    </method>\n    <method name=\""
    "LgeDBus_AskHDRadioVersion\">\n      <!-- 0x0205, Response:0x8205, v?.?"
    "_20120312(out:40bytes) -->\n      <arg direction=\"out\" type=\"ay\"/>"
    "\n    </method>\n    <method name=\"LgeDBus_AskMOST_INICVersion\">\n  "
    "    <!-- 0x0260, Response:0x8260, v?.?_20120423(out:12bytes) -->\n    "
    "  <arg direction=\"out\" type=\"ay\"/>\n    </method>\n    <method nam"
    "e=\"LgeDBus_AskSysOption\">\n      <!-- 0x0204, Response:0x8204, v1.3_"
    "20111003(out:9bytes) -->\n      <arg direction=\"out\" type=\"ay\"/>\n"
    "    </method>\n    <method name=\"LgeDBus_CPUAliveMsg\"/>\n    <!-- 0x"
    "0198 -->\n    <method name=\"LgeDBus_EmergencyConditionMsg\">\n      <"
    "!-- 0x019C, (in:2bytes) -->\n      <arg direction=\"in\" type=\"ay\"/>"
    "\n    </method>\n    <method name=\"LgeDBus_SettingValueStatusMsg\">\n"
    "      <!-- 0x019D, (in:2bytes) -->\n      <arg direction=\"in\" type=\""
    "ay\"/>\n    </method>\n    <method name=\"LgeDBus_AskSettingValueMsg\""
    ">\n      <!-- 0x029D, (out:2bytes) -->\n      <arg direction=\"out\" t"
    "ype=\"ay\"/>\n    </method>\n    <method name=\"LgeDBus_SwVersionMsg\""
    ">\n      <!-- 0x019E (in:3bytes)-->\n      <arg direction=\"in\" type="
    "\"ay\"/>\n    </method>\n    <method name=\"LgeDBus_BTMACAddrStore\">\n"
    "      <!-- 0x019F (in:4bytes)-->\n      <arg direction=\"in\" type=\"a"
    "y\"/>\n    </method>\n    <method name=\"LgeDBus_AskBTMACAddr\">\n    "
    "  <!-- 0x029F, Response:0x829F (out:4bytes)-->\n      <arg direction=\""
    "out\" type=\"ay\"/>\n    </method>\n    <method name=\"LgeDBus_AskAMPV"
    "ersion\">\n      <!-- 0x0206, Response:0x8206 (out:5bytes)-->\n      <"
    "arg direction=\"out\" type=\"ay\"/>\n    </method>\n    <signal name=\""
    "LgeDBus_EventCarIFStatus\">\n      <!-- 0x8390, v1.3_20111003(out:2byt"
    "es) -->\n      <arg direction=\"out\" type=\"y\"/>\n      <arg directi"
    "on=\"out\" type=\"y\"/>\n    </signal>\n    <signal name=\"LgeDBus_Eve"
    "ntDayNightStatus\">\n      <!-- 0x8391, v1.3_20111003(out:1byte) -->\n"
    "      <arg direction=\"out\" type=\"y\"/>\n    </signal>\n    <signal "
    "name=\"LgeDBus_PopUpEvent\">\n      <!-- 0x8392, (out:3byte) -->\n    "
    "  <arg direction=\"out\" type=\"ay\"/>\n    </signal>\n    <signal nam"
    "e=\"LgeDBus_AudioMixCHCtrl\">\n      <!-- 0x8112, (out:2byte) -->\n   "
    "   <arg direction=\"out\" type=\"ay\"/>\n    </signal>\n    <signal na"
    "me=\"LgeDBus_AudioMainCHCtrl\">\n      <!-- 0x8110, (out:1byte) -->\n "
    "     <arg direction=\"out\" type=\"ay\"/>\n    </signal>\n    <signal "
    "name=\"LgeDBus_AudioLastMemory\">\n      <!-- 0x8119, (out:2byte) -->\n"
    "      <arg direction=\"out\" type=\"ay\"/>\n    </signal>\n    <signal"
    " name=\"LgeDBus_AudioMuteCtrl\">\n      <!-- 0x8116, (out:2byte) -->\n"
    "      <arg direction=\"out\" type=\"ay\"/>\n    </signal>\n  </interfa"
    "ce>\n\0"
    "D-Bus Introspection\0\0in0\0"
    "LgeDBus_AudioLastMemory(QByteArray)\0"
    "LgeDBus_AudioMainCHCtrl(QByteArray)\0"
    "LgeDBus_AudioMixCHCtrl(QByteArray)\0"
    "LgeDBus_AudioMuteCtrl(QByteArray)\0"
    "in0,in1\0LgeDBus_EventCarIFStatus(uchar,uchar)\0"
    "LgeDBus_EventDayNightStatus(uchar)\0"
    "LgeDBus_PopUpEvent(QByteArray)\0"
    "QByteArray\0LgeDBus_AskAMPVersion()\0"
    "LgeDBus_AskBTMACAddr()\0"
    "LgeDBus_AskHDRadioVersion()\0"
    "LgeDBus_AskLanguageInfoValue()\0"
    "LgeDBus_AskMOST_INICVersion()\0"
    "LgeDBus_AskSettingValueMsg()\0"
    "LgeDBus_AskSysBootType()\0"
    "LgeDBus_AskSysLastStatus()\0"
    "LgeDBus_AskSysOption()\0LgeDBus_AskSysVersion()\0"
    "LgeDBus_BTMACAddrStore(QByteArray)\0"
    "LgeDBus_CPUAliveMsg()\0"
    "LgeDBus_EmergencyConditionMsg(QByteArray)\0"
    "LgeDBus_EtcGUIMode(uchar)\0"
    "LgeDBus_SetLanguageInfo(uchar)\0"
    "LgeDBus_SetSysOption(QByteArray)\0"
    "LgeDBus_SettingValueStatusMsg(QByteArray)\0"
    "LgeDBus_SwVersionMsg(QByteArray)\0"
};

void SysInfoAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SysInfoAdaptor *_t = static_cast<SysInfoAdaptor *>(_o);
        switch (_id) {
        case 0: _t->LgeDBus_AudioLastMemory((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->LgeDBus_AudioMainCHCtrl((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->LgeDBus_AudioMixCHCtrl((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->LgeDBus_AudioMuteCtrl((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: _t->LgeDBus_EventCarIFStatus((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2]))); break;
        case 5: _t->LgeDBus_EventDayNightStatus((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 6: _t->LgeDBus_PopUpEvent((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 7: { QByteArray _r = _t->LgeDBus_AskAMPVersion();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 8: { QByteArray _r = _t->LgeDBus_AskBTMACAddr();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 9: { QByteArray _r = _t->LgeDBus_AskHDRadioVersion();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 10: { QByteArray _r = _t->LgeDBus_AskLanguageInfoValue();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 11: { QByteArray _r = _t->LgeDBus_AskMOST_INICVersion();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 12: { QByteArray _r = _t->LgeDBus_AskSettingValueMsg();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 13: { QByteArray _r = _t->LgeDBus_AskSysBootType();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 14: { QByteArray _r = _t->LgeDBus_AskSysLastStatus();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 15: { QByteArray _r = _t->LgeDBus_AskSysOption();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 16: { QByteArray _r = _t->LgeDBus_AskSysVersion();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 17: _t->LgeDBus_BTMACAddrStore((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 18: _t->LgeDBus_CPUAliveMsg(); break;
        case 19: _t->LgeDBus_EmergencyConditionMsg((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 20: _t->LgeDBus_EtcGUIMode((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 21: _t->LgeDBus_SetLanguageInfo((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 22: _t->LgeDBus_SetSysOption((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 23: _t->LgeDBus_SettingValueStatusMsg((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 24: _t->LgeDBus_SwVersionMsg((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SysInfoAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SysInfoAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_SysInfoAdaptor,
      qt_meta_data_SysInfoAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SysInfoAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SysInfoAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SysInfoAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SysInfoAdaptor))
        return static_cast<void*>(const_cast< SysInfoAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int SysInfoAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
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
void SysInfoAdaptor::LgeDBus_AudioLastMemory(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SysInfoAdaptor::LgeDBus_AudioMainCHCtrl(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SysInfoAdaptor::LgeDBus_AudioMixCHCtrl(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SysInfoAdaptor::LgeDBus_AudioMuteCtrl(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SysInfoAdaptor::LgeDBus_EventCarIFStatus(uchar _t1, uchar _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SysInfoAdaptor::LgeDBus_EventDayNightStatus(uchar _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void SysInfoAdaptor::LgeDBus_PopUpEvent(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
