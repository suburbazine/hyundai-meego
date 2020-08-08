/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppHomeScreen_View.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppHomeScreen_View.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppHomeScreen_View.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_View[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       8,   54, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x05,
      27,    5,    5,    5, 0x05,
      38,    5,    5,    5, 0x05,
      53,    5,    5,    5, 0x05,
      72,    5,    5,    5, 0x05,
      91,    5,    5,    5, 0x05,

 // slots: signature, parameters, type, tag, flags
     108,    5,    5,    5, 0x08,
     120,    5,    5,    5, 0x08,

 // properties: name, type, flags
     143,  135, 0x0a495001,
     147,  135, 0x0a495001,
     159,  155, 0x02495001,
     171,  155, 0x02495001,
     188,  183, 0x01095401,
     204,  183, 0x01095401,
     220,  155, 0x02495003,
     232,  155, 0x02095401,

 // properties: notify_signal_id
       1,
       2,
       3,
       4,
       0,
       0,
       0,
       0,

       0        // eod
};

static const char qt_meta_stringdata_View[] = {
    "View\0\0UpdateFocusedIndex()\0UpdateBg()\0"
    "UpdateBgMask()\0UpdateBgMaskXPos()\0"
    "UpdateBGMaskYPos()\0updateSubModel()\0"
    "LoadModel()\0LoadSubModel()\0QString\0"
    "sBG\0sBGMask\0int\0nBgMaskXPos\0nBgMaskYPos\0"
    "bool\0bTitleAvailable\0bMediaAvailable\0"
    "nFocusIndex\0nViewId\0"
};

void View::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        View *_t = static_cast<View *>(_o);
        switch (_id) {
        case 0: _t->UpdateFocusedIndex(); break;
        case 1: _t->UpdateBg(); break;
        case 2: _t->UpdateBgMask(); break;
        case 3: _t->UpdateBgMaskXPos(); break;
        case 4: _t->UpdateBGMaskYPos(); break;
        case 5: _t->updateSubModel(); break;
        case 6: _t->LoadModel(); break;
        case 7: _t->LoadSubModel(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData View::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject View::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_View,
      qt_meta_data_View, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &View::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *View::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *View::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_View))
        return static_cast<void*>(const_cast< View*>(this));
    return QObject::qt_metacast(_clname);
}

int View::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = GetBg(); break;
        case 1: *reinterpret_cast< QString*>(_v) = GetBgMask(); break;
        case 2: *reinterpret_cast< int*>(_v) = GetBgMaskXPos(); break;
        case 3: *reinterpret_cast< int*>(_v) = GetBgMaskYPos(); break;
        case 4: *reinterpret_cast< bool*>(_v) = GetTitleAvailable(); break;
        case 5: *reinterpret_cast< bool*>(_v) = GetMediaAvailable(); break;
        case 6: *reinterpret_cast< int*>(_v) = GetFocusIndex(); break;
        case 7: *reinterpret_cast< int*>(_v) = GetViewId(); break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 6: SetFocusIndex(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 8;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void View::UpdateFocusedIndex()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void View::UpdateBg()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void View::UpdateBgMask()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void View::UpdateBgMaskXPos()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void View::UpdateBGMaskYPos()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void View::updateSubModel()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
