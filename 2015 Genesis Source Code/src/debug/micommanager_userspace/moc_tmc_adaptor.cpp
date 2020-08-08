/****************************************************************************
** Meta object code from reading C++ file 'tmc_adaptor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/adaptors/tmc_adaptor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tmc_adaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TMCAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       2,   14, // classinfo
       7,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // classinfo: key, value
      33,   11,
    1163,   49,

 // signals: signature, parameters, type, tag, flags
    1188, 1184, 1183, 1183, 0x05,
    1224, 1184, 1183, 1183, 0x05,

 // slots: signature, parameters, type, tag, flags
    1274, 1183, 1263, 1183, 0x0a,
    1299, 1183, 1263, 1183, 0x0a,
    1324, 1183, 1263, 1183, 0x0a,
    1356, 1184, 1183, 1183, 0x0a,
    1414, 1406, 1183, 1183, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TMCAdaptor[] = {
    "TMCAdaptor\0com.lge.car.micom.tmc\0"
    "D-Bus Interface\0"
    "  <interface name=\"com.lge.car.micom.tmc\">\n    <signal name=\"LgeDB"
    "us_EventInfoUpdate\">\n      <arg direction=\"out\" type=\"ay\"/>\n   "
    "   <!-- 0x8340, 20111228(out:8bytes) -->\n    </signal>\n    <signal n"
    "ame=\"LgeDBus_EventRdsFreqUpdate\">\n      <arg direction=\"out\" type"
    "=\"ay\"/>\n      <!-- 0x8340, 20111228(out:8bytes) -->\n    </signal>\n"
    "    <method name=\"LgeDBus_SetManualFreqChange\">\n      <!-- 0x0140 2"
    "0111228(in:2byte) -->\n      <arg direction=\"in\" type=\"y\"/>\n     "
    " <arg direction=\"in\" type=\"y\"/>\n    </method>\n    <method name=\""
    "LgeDBus_AskCurrentFreq\">\n      <!-- 0x0240, Response:0x8240 20111228"
    "(out:2bytes) -->\n      <arg direction=\"out\" type=\"ay\"/>\n    </me"
    "thod>\n    <method name=\"LgeDBus_AskStationSensitivity\">\n      <!--"
    " 0x0241, Reponse:0x8241, 20111228(out:1bytes) -->\n      <arg directio"
    "n=\"out\" type=\"ay\"/>\n    </method>\n    <method name=\"LgeDBus_Ask"
    "StationList\">\n      <!-- 0x0242, Reponse:0x8242, 20111228(out:variab"
    "le bytes) -->\n      <arg direction=\"out\" type=\"ay\"/>\n    </metho"
    "d>\n    <method name=\"LgeDBus_SetCountryCodeAndDecodeStatus\">\n     "
    " <arg direction=\"in\" type=\"ay\"/>\n    </method>\n  </interface>\n\0"
    "D-Bus Introspection\0\0in0\0"
    "LgeDBus_EventInfoUpdate(QByteArray)\0"
    "LgeDBus_EventRdsFreqUpdate(QByteArray)\0"
    "QByteArray\0LgeDBus_AskCurrentFreq()\0"
    "LgeDBus_AskStationList()\0"
    "LgeDBus_AskStationSensitivity()\0"
    "LgeDBus_SetCountryCodeAndDecodeStatus(QByteArray)\0"
    "in0,in1\0LgeDBus_SetManualFreqChange(uchar,uchar)\0"
};

void TMCAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TMCAdaptor *_t = static_cast<TMCAdaptor *>(_o);
        switch (_id) {
        case 0: _t->LgeDBus_EventInfoUpdate((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->LgeDBus_EventRdsFreqUpdate((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: { QByteArray _r = _t->LgeDBus_AskCurrentFreq();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 3: { QByteArray _r = _t->LgeDBus_AskStationList();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 4: { QByteArray _r = _t->LgeDBus_AskStationSensitivity();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 5: _t->LgeDBus_SetCountryCodeAndDecodeStatus((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 6: _t->LgeDBus_SetManualFreqChange((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TMCAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TMCAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_TMCAdaptor,
      qt_meta_data_TMCAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TMCAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TMCAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TMCAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TMCAdaptor))
        return static_cast<void*>(const_cast< TMCAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int TMCAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void TMCAdaptor::LgeDBus_EventInfoUpdate(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TMCAdaptor::LgeDBus_EventRdsFreqUpdate(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
