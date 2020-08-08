/****************************************************************************
** Meta object code from reading C++ file 'powermanagement_adaptor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/adaptors/powermanagement_adaptor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'powermanagement_adaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PowerManagementAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       2,   14, // classinfo
      16,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // classinfo: key, value
      57,   23,
    1931,   73,

 // signals: signature, parameters, type, tag, flags
    1956, 1952, 1951, 1951, 0x05,
    1985, 1952, 1951, 1951, 0x05,
    2018, 1952, 1951, 1951, 0x05,
    2058, 1951, 1951, 1951, 0x05,

 // slots: signature, parameters, type, tag, flags
    2099, 1951, 2088, 1951, 0x0a,
    2126, 1952, 2088, 1951, 0x0a,
    2160, 1952, 1951, 1951, 0x0a,
    2201, 1952, 1951, 1951, 0x0a,
    2231, 1952, 1951, 1951, 0x0a,
    2267, 1952, 1951, 1951, 0x0a,
    2305, 1952, 1951, 1951, 0x0a,
    2344, 1952, 1951, 1951, 0x0a,
    2384, 1952, 2088, 1951, 0x0a,
    2426, 1952, 1951, 1951, 0x0a,
    2468, 1952, 2088, 1951, 0x0a,
    2502, 1951, 1951, 1951, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PowerManagementAdaptor[] = {
    "PowerManagementAdaptor\0"
    "com.lge.car.micom.powermanagement\0"
    "D-Bus Interface\0"
    "  <interface name=\"com.lge.car.micom.powermanagement\">\n    <signal "
    "name=\"LgeDBus_EventRequestSleep\">\n      <!-- 0x8394 v1.3_20111003(o"
    "ut:1byte)-->\n      <arg direction=\"out\" type=\"y\"/>\n    </signal>"
    "\n    <signal name=\"LgeDBus_EventPowerKey\">\n      <!-- 0x8395 v1.3_"
    "20111003(out:1byte)-->\n      <arg direction=\"out\" type=\"y\"/>\n   "
    " </signal>\n    <signal name=\"LgeDBus_MPAutoModeTestEvent\">\n      <"
    "arg direction=\"out\" type=\"ay\"/>\n    </signal>\n    <signal name=\""
    "LgeDBus_MPAutoModeTestStart\"/>\n    <method name=\"LgeDBus_SetSleepCo"
    "nfirm\"/>\n    <!-- 0x0194 -->\n    <method name=\"LgeDBus_SetHardware"
    "PowerKeyControl\">\n      <!-- 0x0160 v1.3_20111003(in:1byte)-->\n    "
    "  <arg direction=\"in\" type=\"y\"/>\n    </method>\n    <method name="
    "\"LgeDBus_BTPowerControl\">\n      <!-- 0x0169 20150110(in:1byte)-->\n"
    "      <arg direction=\"in\" type=\"y\"/>\n    </method>\n    <method n"
    "ame=\"LgeDBus_SetMPModeInfo\">\n      <arg direction=\"in\" type=\"ay\""
    "/>\n      <arg direction=\"out\" type=\"ay\"/>\n    </method>\n    <me"
    "thod name=\"LgeDBus_AskMPModeInfo\">\n      <arg direction=\"in\" type"
    "=\"ay\"/>\n      <arg direction=\"out\" type=\"ay\"/>\n    </method>\n"
    "    <method name=\"LgeDBus_MPAutoTestModeRequest\">\n      <arg direct"
    "ion=\"in\" type=\"ay\"/>\n      <arg direction=\"out\" type=\"ay\"/>\n"
    "    </method>\n    <method name=\"LgeDBus_MPAutoTestModeCtrl\">\n     "
    " <arg direction=\"in\" type=\"ay\"/>\n    </method>\n    <method name="
    "\"LgeDBus_MPAutoTestModeEvent\">\n      <arg direction=\"in\" type=\"a"
    "y\"/>\n    </method>\n    <method name=\"LgeDBus_AutoTestWIPIDControl\""
    ">\n      <arg direction=\"in\" type=\"ay\"/>\n    </method>\n    <meth"
    "od name=\"LgeDBus_AskAutoTestWIPID\">\n      <arg direction=\"out\" ty"
    "pe=\"ay\"/>\n    </method>\n    <method name=\"LgeDBus_HWCameraPowerCo"
    "ntrol\">\n      <arg direction=\"in\" type=\"y\"/>\n    </method>\n   "
    " <method name=\"LgeDBus_MPAutoModeTestMsg\">\n      <arg direction=\"i"
    "n\" type=\"ay\"/>\n    </method>\n  </interface>\n\0"
    "D-Bus Introspection\0\0in0\0"
    "LgeDBus_EventPowerKey(uchar)\0"
    "LgeDBus_EventRequestSleep(uchar)\0"
    "LgeDBus_MPAutoModeTestEvent(QByteArray)\0"
    "LgeDBus_MPAutoModeTestStart()\0QByteArray\0"
    "LgeDBus_AskAutoTestWIPID()\0"
    "LgeDBus_AskMPModeInfo(QByteArray)\0"
    "LgeDBus_AutoTestWIPIDControl(QByteArray)\0"
    "LgeDBus_BTPowerControl(uchar)\0"
    "LgeDBus_HWCameraPowerControl(uchar)\0"
    "LgeDBus_MPAutoModeTestMsg(QByteArray)\0"
    "LgeDBus_MPAutoTestModeCtrl(QByteArray)\0"
    "LgeDBus_MPAutoTestModeEvent(QByteArray)\0"
    "LgeDBus_MPAutoTestModeRequest(QByteArray)\0"
    "LgeDBus_SetHardwarePowerKeyControl(uchar)\0"
    "LgeDBus_SetMPModeInfo(QByteArray)\0"
    "LgeDBus_SetSleepConfirm()\0"
};

void PowerManagementAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PowerManagementAdaptor *_t = static_cast<PowerManagementAdaptor *>(_o);
        switch (_id) {
        case 0: _t->LgeDBus_EventPowerKey((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 1: _t->LgeDBus_EventRequestSleep((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 2: _t->LgeDBus_MPAutoModeTestEvent((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->LgeDBus_MPAutoModeTestStart(); break;
        case 4: { QByteArray _r = _t->LgeDBus_AskAutoTestWIPID();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 5: { QByteArray _r = _t->LgeDBus_AskMPModeInfo((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 6: _t->LgeDBus_AutoTestWIPIDControl((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 7: _t->LgeDBus_BTPowerControl((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 8: _t->LgeDBus_HWCameraPowerControl((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 9: _t->LgeDBus_MPAutoModeTestMsg((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 10: _t->LgeDBus_MPAutoTestModeCtrl((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 11: _t->LgeDBus_MPAutoTestModeEvent((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 12: { QByteArray _r = _t->LgeDBus_MPAutoTestModeRequest((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 13: _t->LgeDBus_SetHardwarePowerKeyControl((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 14: { QByteArray _r = _t->LgeDBus_SetMPModeInfo((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 15: _t->LgeDBus_SetSleepConfirm(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PowerManagementAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PowerManagementAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_PowerManagementAdaptor,
      qt_meta_data_PowerManagementAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PowerManagementAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PowerManagementAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PowerManagementAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PowerManagementAdaptor))
        return static_cast<void*>(const_cast< PowerManagementAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int PowerManagementAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void PowerManagementAdaptor::LgeDBus_EventPowerKey(uchar _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PowerManagementAdaptor::LgeDBus_EventRequestSleep(uchar _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PowerManagementAdaptor::LgeDBus_MPAutoModeTestEvent(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void PowerManagementAdaptor::LgeDBus_MPAutoModeTestStart()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
