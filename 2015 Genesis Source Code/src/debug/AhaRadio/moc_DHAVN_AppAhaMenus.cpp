/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppAhaMenus.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppAhaMenus.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppAhaMenus.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AhaMenus[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       2,   39, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      52,   32,    9,    9, 0x05,

 // methods: signature, parameters, type, tag, flags
     128,   93,    9,    9, 0x02,
     242,  180,    9,    9, 0x02,
     344,  293,    9,    9, 0x22,

 // properties: name, type, flags
     407,  390, 0x0049500b,
     425,  390, 0x0049500b,

 // properties: notify_signal_id
       0,
       0,

       0        // eod
};

static const char qt_meta_stringdata_AhaMenus[] = {
    "AhaMenus\0\0optMenuModelChanged()\0"
    "topLeft,bottomRight\0"
    "menuDataChanged(QModelIndex,QModelIndex)\0"
    "strCurrLikeIcon,strCurrDislikeIcon\0"
    "UpdateLikeDislikeIconInOptionsMenu(QString,QString)\0"
    "bAllowLike,bAllowDislike,bAllowCall,bAllowNavigate,bNoNetwork\0"
    "UpdateAllowInOptionsMenu(bool,bool,bool,bool,bool)\0"
    "bAllowLike,bAllowDislike,bAllowCall,bAllowNavigate\0"
    "UpdateAllowInOptionsMenu(bool,bool,bool,bool)\0"
    "OptionMenuModel*\0optTrackMenuModel\0"
    "optListMenuModel\0"
};

void AhaMenus::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AhaMenus *_t = static_cast<AhaMenus *>(_o);
        switch (_id) {
        case 0: _t->optMenuModelChanged(); break;
        case 1: _t->menuDataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 2: _t->UpdateLikeDislikeIconInOptionsMenu((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->UpdateAllowInOptionsMenu((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 4: _t->UpdateAllowInOptionsMenu((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AhaMenus::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AhaMenus::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AhaMenus,
      qt_meta_data_AhaMenus, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AhaMenus::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AhaMenus::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AhaMenus::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AhaMenus))
        return static_cast<void*>(const_cast< AhaMenus*>(this));
    return QObject::qt_metacast(_clname);
}

int AhaMenus::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< OptionMenuModel**>(_v) = GetOptTrackMenuModel(); break;
        case 1: *reinterpret_cast< OptionMenuModel**>(_v) = GetOptListMenuModel(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: SetOptTrackMenuModel(*reinterpret_cast< OptionMenuModel**>(_v)); break;
        case 1: SetOptListMenuModel(*reinterpret_cast< OptionMenuModel**>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void AhaMenus::optMenuModelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void AhaMenus::menuDataChanged(const QModelIndex & _t1, const QModelIndex & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
