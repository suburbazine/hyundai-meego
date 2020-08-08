/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppHomeScreen_IconsModel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppHomeScreen_IconsModel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppHomeScreen_IconsModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_IconsModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       5,   49, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      26,   11,   11,   11, 0x05,
      45,   11,   11,   11, 0x05,
      65,   11,   11,   11, 0x05,
      87,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     113,  108,   11,   11, 0x0a,
     158,   11,   11,   11, 0x08,

 // properties: name, type, flags
     181,  177, 0x02495003,
     201,  193, 0x0a495001,
     214,  177, 0x02495001,
     229,  177, 0x02495001,
     243,  177, 0x02095401,

 // properties: notify_signal_id
       1,
       2,
       3,
       4,
       0,

       0        // eod
};

static const char qt_meta_stringdata_IconsModel[] = {
    "IconsModel\0\0ReloadModel()\0updateFocusIndex()\0"
    "UpdateFocusedText()\0UpdateFocusedViewId()\0"
    "UpdateFocusedAppId()\0nID,\0"
    "UpdateDataField(HSDefP::APP_DATA_T,QVariant)\0"
    "handleItemChange()\0int\0nFocusIndex\0"
    "QString\0sFocusedText\0nFocusedViewId\0"
    "nFocusedAppId\0nCountItems\0"
};

void IconsModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        IconsModel *_t = static_cast<IconsModel *>(_o);
        switch (_id) {
        case 0: _t->ReloadModel(); break;
        case 1: _t->updateFocusIndex(); break;
        case 2: _t->UpdateFocusedText(); break;
        case 3: _t->UpdateFocusedViewId(); break;
        case 4: _t->UpdateFocusedAppId(); break;
        case 5: _t->UpdateDataField((*reinterpret_cast< HSDefP::APP_DATA_T(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 6: _t->handleItemChange(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData IconsModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject IconsModel::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_IconsModel,
      qt_meta_data_IconsModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &IconsModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *IconsModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *IconsModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IconsModel))
        return static_cast<void*>(const_cast< IconsModel*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int IconsModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = GetFocusIndex(); break;
        case 1: *reinterpret_cast< QString*>(_v) = GetFocusedText(); break;
        case 2: *reinterpret_cast< int*>(_v) = GetFocusedViewId(); break;
        case 3: *reinterpret_cast< int*>(_v) = GetFocusedAppId(); break;
        case 4: *reinterpret_cast< int*>(_v) = GetCountItems(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: SetFocusIndex(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void IconsModel::ReloadModel()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void IconsModel::updateFocusIndex()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void IconsModel::UpdateFocusedText()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void IconsModel::UpdateFocusedViewId()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void IconsModel::UpdateFocusedAppId()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
