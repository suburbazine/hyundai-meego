/****************************************************************************
** Meta object code from reading C++ file 'eventlistener.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/statusbar/listener/eventlistener.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'eventlistener.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EventListener[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,   14,   14,   14, 0x08,
      72,   14,   14,   14, 0x08,
      81,   79,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_EventListener[] = {
    "EventListener\0\0eventTriggered(QString)\0"
    "handleAudioMessage(QDBusMessage)\0"
    "beep()\0,\0handleCarIFMessage(uchar,uchar)\0"
};

void EventListener::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        EventListener *_t = static_cast<EventListener *>(_o);
        switch (_id) {
        case 0: _t->eventTriggered((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->handleAudioMessage((*reinterpret_cast< QDBusMessage(*)>(_a[1]))); break;
        case 2: _t->beep(); break;
        case 3: _t->handleCarIFMessage((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData EventListener::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject EventListener::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_EventListener,
      qt_meta_data_EventListener, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EventListener::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EventListener::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EventListener::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EventListener))
        return static_cast<void*>(const_cast< EventListener*>(this));
    return QObject::qt_metacast(_clname);
}

int EventListener::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void EventListener::eventTriggered(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
