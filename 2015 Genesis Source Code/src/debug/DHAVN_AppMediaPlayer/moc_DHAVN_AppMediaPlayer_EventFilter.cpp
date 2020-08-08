/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_EventFilter.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/engine/DHAVN_AppMediaPlayer_EventFilter.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_EventFilter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EventFilter[] = {

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
      17,   13,   12,   12, 0x05,
      52,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      69,   13,   12,   12, 0x0a,
      94,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EventFilter[] = {
    "EventFilter\0\0evt\0sendEventForExecute(EVENT_DISPLAY)\0"
    "requestDequeue()\0onEnqueue(EVENT_DISPLAY)\0"
    "onDequeue()\0"
};

void EventFilter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        EventFilter *_t = static_cast<EventFilter *>(_o);
        switch (_id) {
        case 0: _t->sendEventForExecute((*reinterpret_cast< EVENT_DISPLAY(*)>(_a[1]))); break;
        case 1: _t->requestDequeue(); break;
        case 2: _t->onEnqueue((*reinterpret_cast< EVENT_DISPLAY(*)>(_a[1]))); break;
        case 3: _t->onDequeue(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData EventFilter::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject EventFilter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_EventFilter,
      qt_meta_data_EventFilter, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EventFilter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EventFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EventFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EventFilter))
        return static_cast<void*>(const_cast< EventFilter*>(this));
    return QObject::qt_metacast(_clname);
}

int EventFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void EventFilter::sendEventForExecute(EVENT_DISPLAY _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EventFilter::requestDequeue()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
