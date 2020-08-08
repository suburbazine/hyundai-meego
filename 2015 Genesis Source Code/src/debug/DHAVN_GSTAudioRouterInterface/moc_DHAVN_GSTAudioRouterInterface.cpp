/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_GSTAudioRouterInterface.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_GSTAudioRouterInterface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_GSTAudioRouterInterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GSTAudioRouterDBus[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   20,   19,   19, 0x05,
      66,   20,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
     139,  119,   99,   19, 0x0a,
     194,  187,   99,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GSTAudioRouterDBus[] = {
    "GSTAudioRouterDBus\0\0status,inType\0"
    "AquireStatus(bool,EAudioSource)\0"
    "ReleaseStatus(bool,EAudioSource)\0"
    "QDBusPendingReply<>\0inSrc,inRate,inType\0"
    "AquireAudioSource(QString,QString,EAudioSource)\0"
    "inType\0ReleaseAudioSource(EAudioSource)\0"
};

void GSTAudioRouterDBus::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GSTAudioRouterDBus *_t = static_cast<GSTAudioRouterDBus *>(_o);
        switch (_id) {
        case 0: _t->AquireStatus((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const EAudioSource(*)>(_a[2]))); break;
        case 1: _t->ReleaseStatus((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const EAudioSource(*)>(_a[2]))); break;
        case 2: { QDBusPendingReply<> _r = _t->AquireAudioSource((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const EAudioSource(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 3: { QDBusPendingReply<> _r = _t->ReleaseAudioSource((*reinterpret_cast< const EAudioSource(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GSTAudioRouterDBus::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GSTAudioRouterDBus::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_GSTAudioRouterDBus,
      qt_meta_data_GSTAudioRouterDBus, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GSTAudioRouterDBus::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GSTAudioRouterDBus::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GSTAudioRouterDBus::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GSTAudioRouterDBus))
        return static_cast<void*>(const_cast< GSTAudioRouterDBus*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int GSTAudioRouterDBus::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
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
void GSTAudioRouterDBus::AquireStatus(bool _t1, const EAudioSource & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GSTAudioRouterDBus::ReleaseStatus(bool _t1, const EAudioSource & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
