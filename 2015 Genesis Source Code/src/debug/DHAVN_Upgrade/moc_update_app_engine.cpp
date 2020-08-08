/****************************************************************************
** Meta object code from reading C++ file 'update_app_engine.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/uish_interface/update_app_engine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'update_app_engine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UpdateAppEngine[] = {

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
      17,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,   35,   16,   16, 0x0a,
      74,   35,   16,   16, 0x0a,
     110,   35,   16,   16, 0x0a,
     148,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UpdateAppEngine[] = {
    "UpdateAppEngine\0\0jogDialEvent(int)\0"
    "aEvent\0NotifyFromUIStateManager(Event)\0"
    "NotifyFromUIStateManagerRear(Event)\0"
    "NotifyFromUIStateManagerCommon(Event)\0"
    "exitApplication()\0"
};

void UpdateAppEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UpdateAppEngine *_t = static_cast<UpdateAppEngine *>(_o);
        switch (_id) {
        case 0: _t->jogDialEvent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->NotifyFromUIStateManager((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 2: _t->NotifyFromUIStateManagerRear((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 3: _t->NotifyFromUIStateManagerCommon((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 4: _t->exitApplication(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UpdateAppEngine::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UpdateAppEngine::staticMetaObject = {
    { &AppEngineQML::staticMetaObject, qt_meta_stringdata_UpdateAppEngine,
      qt_meta_data_UpdateAppEngine, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UpdateAppEngine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UpdateAppEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UpdateAppEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UpdateAppEngine))
        return static_cast<void*>(const_cast< UpdateAppEngine*>(this));
    return AppEngineQML::qt_metacast(_clname);
}

int UpdateAppEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppEngineQML::qt_metacall(_c, _id, _a);
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
void UpdateAppEngine::jogDialEvent(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
