/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_EventsEmulator.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppMediaPlayer_EventsEmulator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_EventsEmulator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EventsEmulator[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      28,   16,   15,   15, 0x02,
      89,   66,   15,   15, 0x02,
     133,  126,   15,   15, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_EventsEmulator[] = {
    "EventsEmulator\0\0object,lock\0"
    "lockScrolling(QDeclarativeItem*,bool)\0"
    "direction,delta,object\0"
    "sendWheel(int,int,QDeclarativeItem*)\0"
    "object\0sendMouseRelease(QDeclarativeItem*)\0"
};

void EventsEmulator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        EventsEmulator *_t = static_cast<EventsEmulator *>(_o);
        switch (_id) {
        case 0: _t->lockScrolling((*reinterpret_cast< QDeclarativeItem*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->sendWheel((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QDeclarativeItem*(*)>(_a[3]))); break;
        case 2: _t->sendMouseRelease((*reinterpret_cast< QDeclarativeItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData EventsEmulator::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject EventsEmulator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_EventsEmulator,
      qt_meta_data_EventsEmulator, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EventsEmulator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EventsEmulator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EventsEmulator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EventsEmulator))
        return static_cast<void*>(const_cast< EventsEmulator*>(this));
    return QObject::qt_metacast(_clname);
}

int EventsEmulator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
