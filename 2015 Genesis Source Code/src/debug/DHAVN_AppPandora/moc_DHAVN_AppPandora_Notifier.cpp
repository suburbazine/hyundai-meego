/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppPandora_Notifier.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppPandora_Notifier.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppPandora_Notifier.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CExternalNotifier[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      72,   54,   18,   18, 0x0a,

 // methods: signature, parameters, type, tag, flags
     109,   18,   18,   18, 0x02,
     124,   18,   18,   18, 0x02,
     150,  144,   18,   18, 0x02,
     176,   18,   18,   18, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_CExternalNotifier[] = {
    "CExternalNotifier\0\0canframe\0"
    "Can_data_send(SQCanFrame)\0,inStation,isCurr\0"
    "OnTunedStation(quint32,QString,bool)\0"
    "ClearCluster()\0SendNullToCluster()\0"
    "title\0UpdateTrackTitle(QString)\0"
    "UpdateOSDOnCallingState()\0"
};

void CExternalNotifier::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CExternalNotifier *_t = static_cast<CExternalNotifier *>(_o);
        switch (_id) {
        case 0: _t->Can_data_send((*reinterpret_cast< const SQCanFrame(*)>(_a[1]))); break;
        case 1: _t->OnTunedStation((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 2: _t->ClearCluster(); break;
        case 3: _t->SendNullToCluster(); break;
        case 4: _t->UpdateTrackTitle((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->UpdateOSDOnCallingState(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CExternalNotifier::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CExternalNotifier::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CExternalNotifier,
      qt_meta_data_CExternalNotifier, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CExternalNotifier::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CExternalNotifier::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CExternalNotifier::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CExternalNotifier))
        return static_cast<void*>(const_cast< CExternalNotifier*>(this));
    return QObject::qt_metacast(_clname);
}

int CExternalNotifier::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CExternalNotifier::Can_data_send(const SQCanFrame & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
