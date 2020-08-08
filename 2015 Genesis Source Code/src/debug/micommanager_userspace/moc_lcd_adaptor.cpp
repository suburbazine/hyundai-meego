/****************************************************************************
** Meta object code from reading C++ file 'lcd_adaptor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/adaptors/lcd_adaptor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lcd_adaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LcdAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       2,   14, // classinfo
       4,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
      33,   11,
     835,   49,

 // signals: signature, parameters, type, tag, flags
     860,  856,  855,  855, 0x05,

 // slots: signature, parameters, type, tag, flags
     909,  855,  898,  855, 0x0a,
     938,  856,  855,  855, 0x0a,
     989,  973,  855,  855, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LcdAdaptor[] = {
    "LcdAdaptor\0com.lge.car.micom.lcd\0"
    "D-Bus Interface\0"
    "  <interface name=\"com.lge.car.micom.lcd\">\n    <method name=\"LgeDB"
    "us_SetBacklightControl\">\n      <!-- 0x0161 v1.3_20111003(in:1byte) -"
    "->\n      <arg direction=\"in\" type=\"y\"/>\n    </method>\n    <meth"
    "od name=\"LgeDBus_SetLcdPWMControl\">\n      <!-- 0x0162 v1.3_20111003"
    "(in:3bytes) -->\n      <!-- 0x0162 20111212(in:4bytes) -->\n      <arg"
    " direction=\"in\" type=\"y\"/>\n      <arg direction=\"in\" type=\"y\""
    "/>\n      <arg direction=\"in\" type=\"y\"/>\n      <arg direction=\"i"
    "n\" type=\"y\"/>\n    </method>\n    <method name=\"LgeDBus_AskLcdSetI"
    "nfoValue\">\n      <!-- 0x0290, Reponse:0x8290, v1.3_20111003(out:4byt"
    "es) -->\n      <arg direction=\"out\" type=\"ay\"/>\n    </method>\n  "
    "  <signal name=\"LgeDBus_EventLCDSetupInfo\">\n      <!-- 0x8397 -->\n"
    "      <arg direction=\"out\" type=\"ay\"/>\n    </signal>\n  </interfa"
    "ce>\n\0"
    "D-Bus Introspection\0\0in0\0"
    "LgeDBus_EventLCDSetupInfo(QByteArray)\0"
    "QByteArray\0LgeDBus_AskLcdSetInfoValue()\0"
    "LgeDBus_SetBacklightControl(uchar)\0"
    "in0,in1,in2,in3\0"
    "LgeDBus_SetLcdPWMControl(uchar,uchar,uchar,uchar)\0"
};

void LcdAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LcdAdaptor *_t = static_cast<LcdAdaptor *>(_o);
        switch (_id) {
        case 0: _t->LgeDBus_EventLCDSetupInfo((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: { QByteArray _r = _t->LgeDBus_AskLcdSetInfoValue();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 2: _t->LgeDBus_SetBacklightControl((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 3: _t->LgeDBus_SetLcdPWMControl((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2])),(*reinterpret_cast< uchar(*)>(_a[3])),(*reinterpret_cast< uchar(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LcdAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LcdAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_LcdAdaptor,
      qt_meta_data_LcdAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LcdAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LcdAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LcdAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LcdAdaptor))
        return static_cast<void*>(const_cast< LcdAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int LcdAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void LcdAdaptor::LgeDBus_EventLCDSetupInfo(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
