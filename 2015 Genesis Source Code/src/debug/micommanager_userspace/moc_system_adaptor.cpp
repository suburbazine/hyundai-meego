/****************************************************************************
** Meta object code from reading C++ file 'system_adaptor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/adaptors/system_adaptor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'system_adaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SystemAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       2,   14, // classinfo
      20,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // classinfo: key, value
      39,   14,
    2569,   55,

 // signals: signature, parameters, type, tag, flags
    2594, 2590, 2589, 2589, 0x05,
    2630, 2590, 2589, 2589, 0x05,

 // slots: signature, parameters, type, tag, flags
    2666, 2589, 2589, 2589, 0x0a,
    2694, 2590, 2589, 2589, 0x0a,
    2736, 2589, 2725, 2589, 0x0a,
    2757, 2589, 2589, 2589, 0x0a,
    2789, 2589, 2589, 2589, 0x0a,
    2820, 2589, 2589, 2589, 0x0a,
    2849, 2590, 2725, 2589, 0x0a,
    2880, 2590, 2589, 2589, 0x0a,
    2912, 2590, 2589, 2589, 0x0a,
    2945, 2590, 2589, 2589, 0x0a,
    2977, 2589, 2589, 2589, 0x0a,
    3005, 2590, 2589, 2589, 0x0a,
    3043, 2590, 2589, 2589, 0x0a,
    3079, 2590, 2589, 2589, 0x0a,
    3108, 2590, 2589, 2589, 0x0a,
    3140, 2590, 2589, 2589, 0x0a,
    3174, 2589, 2589, 2589, 0x0a,
    3198, 2589, 2725, 2589, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SystemAdaptor[] = {
    "SystemAdaptor\0com.lge.car.micom.system\0"
    "D-Bus Interface\0"
    "  <interface name=\"com.lge.car.micom.system\">\n    <signal name=\"Lg"
    "eDBus_EventFrontSpecialKey\">\n      <!-- 0x8375 from micom v0.8 avaia"
    "ble(out:1byte)-->\n      <arg direction=\"out\" type=\"y\"/>\n    </si"
    "gnal>\n    <signal name=\"LgeDBus_FactoryResetComplete\">\n      <!-- "
    "0x8360 (out:1byte)-->\n      <arg direction=\"out\" type=\"y\"/>\n    "
    "</signal>\n    <method name=\"LgeDBus_SetFanControl\">\n      <!-- 0x0"
    "163, v1.3_20111003(in:1byte) -->\n      <arg direction=\"in\" type=\"y"
    "\"/>\n    </method>\n    <method name=\"LgeDBus_ResetHWPeripheral\">\n"
    "      <!-- 0x0164, (in:2byte) -->\n      <arg direction=\"in\" type=\""
    "ay\"/>\n    </method>\n    <method name=\"LgeDBus_SetPhoneMicControl\""
    ">\n      <!-- 0x0165, v1.3_20111003(in:1byte) -->\n      <arg directio"
    "n=\"in\" type=\"y\"/>\n    </method>\n    <method name=\"LgeDBus_SetDi"
    "scIndicationLed\">\n      <!-- 0x0168, v_1.4_20111010(in:1byte) -->\n "
    "     <arg direction=\"in\" type=\"y\"/>\n    </method>\n    <method na"
    "me=\"LgeDBus_SetHwRTCSet\">\n      <!-- 0x0166, v_1.3_20111003(in:7byt"
    "es) -->\n      <arg direction=\"in\" type=\"ay\"/>\n    </method>\n   "
    " <method name=\"LgeDBus_AskRTCInfo\">\n      <!-- 0x0291, Reponse:0x82"
    "91, v_1.3_20111003(out:7bytes) -->\n      <arg direction=\"out\" type="
    "\"ay\"/>\n    </method>\n    <method name=\"LgeDBus_EEPROMWrite\">\n  "
    "    <!-- 0x0195, (in:variable) -->\n      <arg direction=\"in\" type=\""
    "ay\"/>\n    </method>\n    <method name=\"LgeDBus_EEPROMRead\">\n     "
    " <!-- 0x0295, Reponse:0x8295, (in, out:variable) -->\n      <arg direc"
    "tion=\"in\" type=\"ay\"/>\n      <arg direction=\"out\" type=\"ay\"/>\n"
    "    </method>\n    <method name=\"LgeDBus_AppBootComplete\">\n      <!"
    "-- 0x0191, (in:1byte) -->\n      <arg direction=\"in\" type=\"y\"/>\n "
    "   </method>\n    <method name=\"LgeDBus_ETCFactorySetting\">\n      <"
    "!-- 0x0192, (in:1byte) -->\n      <arg direction=\"in\" type=\"y\"/>\n"
    "    </method>\n    <method name=\"LgeDBus_MainImageUpgrade\">\n      <"
    "!-- 0x0198, (in:1byte) -->\n      <arg direction=\"in\" type=\"y\"/>\n"
    "    </method>\n    <method name=\"LgeDBus_WDTCountInfo\">\n      <!-- "
    "0x02A0, (out:4byte) -->\n      <arg direction=\"out\" type=\"ay\"/>\n "
    "   </method>\n    <method name=\"LgeDBus_DATCInitialRequest\"/>\n    <"
    "!-- 0x0180 20120207 -->\n    <method name=\"LgeDBus_CarSetInitialReque"
    "st\"/>\n    <!-- 0x0181 20120207 -->\n    <method name=\"LgeDBus_CarIn"
    "foInitialRequest\"/>\n    <!-- 0x0185 20120207 -->\n    <method name=\""
    "LgeDBus_PGSInitialRequest\"/>\n    <!-- 0x0182 20120207 -->\n    <meth"
    "od name=\"LgeDBus_AVMInitialRequest\"/>\n    <!-- 0x0183 20120207 -->\n"
    "    <method name=\"LgeDBus_WDTCountClear\"/>\n    <!-- 0x01A0 20140104"
    " -->\n  </interface>\n\0"
    "D-Bus Introspection\0\0in0\0"
    "LgeDBus_EventFrontSpecialKey(uchar)\0"
    "LgeDBus_FactoryResetComplete(uchar)\0"
    "LgeDBus_AVMInitialRequest()\0"
    "LgeDBus_AppBootComplete(uchar)\0"
    "QByteArray\0LgeDBus_AskRTCInfo()\0"
    "LgeDBus_CarInfoInitialRequest()\0"
    "LgeDBus_CarSetInitialRequest()\0"
    "LgeDBus_DATCInitialRequest()\0"
    "LgeDBus_EEPROMRead(QByteArray)\0"
    "LgeDBus_EEPROMWrite(QByteArray)\0"
    "LgeDBus_ETCFactorySetting(uchar)\0"
    "LgeDBus_MainImageUpgrade(uchar)\0"
    "LgeDBus_PGSInitialRequest()\0"
    "LgeDBus_ResetHWPeripheral(QByteArray)\0"
    "LgeDBus_SetDiscIndicationLed(uchar)\0"
    "LgeDBus_SetFanControl(uchar)\0"
    "LgeDBus_SetHwRTCSet(QByteArray)\0"
    "LgeDBus_SetPhoneMicControl(uchar)\0"
    "LgeDBus_WDTCountClear()\0LgeDBus_WDTCountInfo()\0"
};

void SystemAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SystemAdaptor *_t = static_cast<SystemAdaptor *>(_o);
        switch (_id) {
        case 0: _t->LgeDBus_EventFrontSpecialKey((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 1: _t->LgeDBus_FactoryResetComplete((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 2: _t->LgeDBus_AVMInitialRequest(); break;
        case 3: _t->LgeDBus_AppBootComplete((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 4: { QByteArray _r = _t->LgeDBus_AskRTCInfo();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 5: _t->LgeDBus_CarInfoInitialRequest(); break;
        case 6: _t->LgeDBus_CarSetInitialRequest(); break;
        case 7: _t->LgeDBus_DATCInitialRequest(); break;
        case 8: { QByteArray _r = _t->LgeDBus_EEPROMRead((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 9: _t->LgeDBus_EEPROMWrite((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 10: _t->LgeDBus_ETCFactorySetting((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 11: _t->LgeDBus_MainImageUpgrade((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 12: _t->LgeDBus_PGSInitialRequest(); break;
        case 13: _t->LgeDBus_ResetHWPeripheral((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 14: _t->LgeDBus_SetDiscIndicationLed((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 15: _t->LgeDBus_SetFanControl((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 16: _t->LgeDBus_SetHwRTCSet((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 17: _t->LgeDBus_SetPhoneMicControl((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 18: _t->LgeDBus_WDTCountClear(); break;
        case 19: { QByteArray _r = _t->LgeDBus_WDTCountInfo();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SystemAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SystemAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_SystemAdaptor,
      qt_meta_data_SystemAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SystemAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SystemAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SystemAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SystemAdaptor))
        return static_cast<void*>(const_cast< SystemAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int SystemAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void SystemAdaptor::LgeDBus_EventFrontSpecialKey(uchar _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SystemAdaptor::LgeDBus_FactoryResetComplete(uchar _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
