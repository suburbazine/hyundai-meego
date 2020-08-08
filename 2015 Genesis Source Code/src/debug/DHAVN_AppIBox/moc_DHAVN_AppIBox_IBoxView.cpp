/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppIBox_IBoxView.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppIBox_IBoxView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppIBox_IBoxView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_IBoxView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   10,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      54,   10,    9,    9, 0x0a,
      82,    9,    9,    9, 0x0a,
     104,    9,    9,    9, 0x0a,
     119,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_IBoxView[] = {
    "IBoxView\0\0oldState,newState\0"
    "stateChanged(State,State)\0"
    "onStateChanged(State,State)\0"
    "onTimeoutShowWidget()\0playPipeLine()\0"
    "pausePipeLine()\0"
};

void IBoxView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        IBoxView *_t = static_cast<IBoxView *>(_o);
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast< State(*)>(_a[1])),(*reinterpret_cast< State(*)>(_a[2]))); break;
        case 1: _t->onStateChanged((*reinterpret_cast< State(*)>(_a[1])),(*reinterpret_cast< State(*)>(_a[2]))); break;
        case 2: _t->onTimeoutShowWidget(); break;
        case 3: _t->playPipeLine(); break;
        case 4: _t->pausePipeLine(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData IBoxView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject IBoxView::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IBoxView,
      qt_meta_data_IBoxView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &IBoxView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *IBoxView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *IBoxView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IBoxView))
        return static_cast<void*>(const_cast< IBoxView*>(this));
    return QObject::qt_metacast(_clname);
}

int IBoxView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void IBoxView::stateChanged(State _t1, State _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
