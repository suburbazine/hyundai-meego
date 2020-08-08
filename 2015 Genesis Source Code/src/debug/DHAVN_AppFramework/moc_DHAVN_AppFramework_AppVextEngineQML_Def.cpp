/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFramework_AppVextEngineQML_Def.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFramework_AppVextEngineQML_Def.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFramework_AppVextEngineQML_Def.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppVextEngineQML[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      63,   47,   17,   17, 0x08,
     101,   47,   17,   17, 0x08,
     140,   17,   17,   17, 0x08,
     166,   17,   17,   17, 0x08,
     191,   17,   17,   17, 0x08,
     222,   17,   17,   17, 0x08,
     259,  252,   17,   17, 0x08,
     293,  252,   17,   17, 0x08,

 // methods: signature, parameters, type, tag, flags
     328,  326,   17,   17, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_AppVextEngineQML[] = {
    "AppVextEngineQML\0\0event\0SendTouchport(QEvent*)\0"
    "target,response\0HandlePopupResponseRear(int,QVariant)\0"
    "HandlePopupResponseFront(int,QVariant)\0"
    "SlotCallFgFrontResponce()\0"
    "SlotCallFgRearResponce()\0"
    "DoFrameBufferTransitionFront()\0"
    "DoFrameBufferTransitionRear()\0aEvent\0"
    "HandleTouchViewFrontPort(QEvent*)\0"
    "HandleTouchViewRearPort(QEvent*)\0,\0"
    "SetVisibleScreens(int,bool)\0"
};

void AppVextEngineQML::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppVextEngineQML *_t = static_cast<AppVextEngineQML *>(_o);
        switch (_id) {
        case 0: _t->SendTouchport((*reinterpret_cast< QEvent*(*)>(_a[1]))); break;
        case 1: _t->HandlePopupResponseRear((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 2: _t->HandlePopupResponseFront((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 3: _t->SlotCallFgFrontResponce(); break;
        case 4: _t->SlotCallFgRearResponce(); break;
        case 5: _t->DoFrameBufferTransitionFront(); break;
        case 6: _t->DoFrameBufferTransitionRear(); break;
        case 7: _t->HandleTouchViewFrontPort((*reinterpret_cast< QEvent*(*)>(_a[1]))); break;
        case 8: _t->HandleTouchViewRearPort((*reinterpret_cast< QEvent*(*)>(_a[1]))); break;
        case 9: _t->SetVisibleScreens((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppVextEngineQML::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppVextEngineQML::staticMetaObject = {
    { &AppEngineQML::staticMetaObject, qt_meta_stringdata_AppVextEngineQML,
      qt_meta_data_AppVextEngineQML, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppVextEngineQML::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppVextEngineQML::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppVextEngineQML::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppVextEngineQML))
        return static_cast<void*>(const_cast< AppVextEngineQML*>(this));
    return AppEngineQML::qt_metacast(_clname);
}

int AppVextEngineQML::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppEngineQML::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void AppVextEngineQML::SendTouchport(QEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
