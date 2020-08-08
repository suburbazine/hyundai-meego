/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppHomeScreen_ViewController.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppHomeScreen_ViewController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppHomeScreen_ViewController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ViewController[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       5,   79, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      34,   15,   15,   15, 0x05,
      53,   15,   15,   15, 0x05,
      74,   15,   15,   15, 0x05,
      93,   15,   15,   15, 0x05,
     118,  111,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     134,   15,   15,   15, 0x0a,
     161,  155,   15,   15, 0x0a,
     182,   15,   15,   15, 0x08,

 // methods: signature, parameters, type, tag, flags
     211,  196,   15,   15, 0x02,
     246,  230,   15,   15, 0x02,
     282,  273,   15,   15, 0x02,
     308,   15,  304,   15, 0x02,

 // properties: name, type, flags
     329,  321, 0x0a495001,
     340,  304, 0x02495001,
     357,  352, 0x01495003,
     371,  352, 0x01495003,
     383,  304, 0x02495003,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,

       0        // eod
};

static const char qt_meta_stringdata_ViewController[] = {
    "ViewController\0\0UpdateTitleText()\0"
    "UpdateTitleAlign()\0UpdateFocusEnabled()\0"
    "UpdateJogPressed()\0UpdatePopUpType()\0"
    "screen\0closepopup(int)\0onTimeoutShowPopup()\0"
    "title\0UpdateTitle(QString)\0UpdateTitle()\0"
    "nAppId,nViewId\0ShowPopUp(int,int)\0"
    "nViewId,iconIdx\0SetFocusIconIndex(int,int)\0"
    "bEnabled\0SetFocusEnabled(bool)\0int\0"
    "GetDisplay()\0QString\0sTitleText\0"
    "nTitleAlign\0bool\0bFocusEnabled\0"
    "bJogPressed\0nPopUpType\0"
};

void ViewController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ViewController *_t = static_cast<ViewController *>(_o);
        switch (_id) {
        case 0: _t->UpdateTitleText(); break;
        case 1: _t->UpdateTitleAlign(); break;
        case 2: _t->UpdateFocusEnabled(); break;
        case 3: _t->UpdateJogPressed(); break;
        case 4: _t->UpdatePopUpType(); break;
        case 5: _t->closepopup((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onTimeoutShowPopup(); break;
        case 7: _t->UpdateTitle((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->UpdateTitle(); break;
        case 9: _t->ShowPopUp((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->SetFocusIconIndex((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->SetFocusEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: { int _r = _t->GetDisplay();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ViewController::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ViewController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ViewController,
      qt_meta_data_ViewController, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ViewController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ViewController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ViewController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ViewController))
        return static_cast<void*>(const_cast< ViewController*>(this));
    return QObject::qt_metacast(_clname);
}

int ViewController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = GetTitleText(); break;
        case 1: *reinterpret_cast< int*>(_v) = GetTitleAlign(); break;
        case 2: *reinterpret_cast< bool*>(_v) = GetFocusEnabled(); break;
        case 3: *reinterpret_cast< bool*>(_v) = GetJogPressed(); break;
        case 4: *reinterpret_cast< int*>(_v) = GetPopUpType(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 2: SetFocusEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 3: SetJogPressed(*reinterpret_cast< bool*>(_v)); break;
        case 4: SetPopUpType(*reinterpret_cast< int*>(_v)); break;
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
void ViewController::UpdateTitleText()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ViewController::UpdateTitleAlign()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void ViewController::UpdateFocusEnabled()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void ViewController::UpdateJogPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void ViewController::UpdatePopUpType()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void ViewController::closepopup(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
