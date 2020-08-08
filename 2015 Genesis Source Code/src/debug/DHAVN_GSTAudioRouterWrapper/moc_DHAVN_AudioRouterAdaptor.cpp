/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AudioRouterAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DHAVN_AudioRouterAdaptor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AudioRouterAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GSTAudioRouterDBusAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       2,   14, // classinfo
       4,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // classinfo: key, value
      53,   26,
      70,   69,

 // signals: signature, parameters, type, tag, flags
     107,   90,   69,   69, 0x05,
     139,   90,   69,   69, 0x05,

 // slots: signature, parameters, type, tag, flags
     195,  172,   69,   69, 0x0a,
     253,  243,   69,   69, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GSTAudioRouterDBusAdaptor[] = {
    "GSTAudioRouterDBusAdaptor\0"
    "org.lge.GSTAudioRouterDBus\0D-Bus Interface\0"
    "\0D-Bus Introspection\0status,inSrcType\0"
    "AquireStatus(bool,EAudioSource)\0"
    "ReleaseStatus(bool,EAudioSource)\0"
    "inSrc,inRate,inSrcType\0"
    "AquireAudioSource(QString,QString,EAudioSource)\0"
    "inSrcType\0ReleaseAudioSource(EAudioSource)\0"
};

void GSTAudioRouterDBusAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GSTAudioRouterDBusAdaptor *_t = static_cast<GSTAudioRouterDBusAdaptor *>(_o);
        switch (_id) {
        case 0: _t->AquireStatus((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const EAudioSource(*)>(_a[2]))); break;
        case 1: _t->ReleaseStatus((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const EAudioSource(*)>(_a[2]))); break;
        case 2: _t->AquireAudioSource((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const EAudioSource(*)>(_a[3]))); break;
        case 3: _t->ReleaseAudioSource((*reinterpret_cast< const EAudioSource(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GSTAudioRouterDBusAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GSTAudioRouterDBusAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_GSTAudioRouterDBusAdaptor,
      qt_meta_data_GSTAudioRouterDBusAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GSTAudioRouterDBusAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GSTAudioRouterDBusAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GSTAudioRouterDBusAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GSTAudioRouterDBusAdaptor))
        return static_cast<void*>(const_cast< GSTAudioRouterDBusAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int GSTAudioRouterDBusAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void GSTAudioRouterDBusAdaptor::AquireStatus(bool _t1, const EAudioSource & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GSTAudioRouterDBusAdaptor::ReleaseStatus(bool _t1, const EAudioSource & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
