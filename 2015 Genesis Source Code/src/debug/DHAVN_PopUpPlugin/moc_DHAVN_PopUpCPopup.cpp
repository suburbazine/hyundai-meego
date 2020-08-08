/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_PopUpCPopup.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_PopUpCPopup.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_PopUpCPopup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPopUp[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,    8,    7,    7, 0x05,
      60,   55,    7,    7, 0x05,
      85,   55,    7,    7, 0x05,
     114,   55,    7,    7, 0x05,
     145,   55,    7,    7, 0x05,
     173,   55,    7,    7, 0x05,
     198,    7,    7,    7, 0x05,
     228,  217,    7,    7, 0x05,
     265,  257,    7,    7, 0x05,
     286,    7,    7,    7, 0x05,
     311,  304,    7,    7, 0x05,
     336,   55,    7,    7, 0x05,
     362,   55,    7,    7, 0x05,

 // methods: signature, parameters, type, tag, flags
     390,    7,    7,    7, 0x02,
     410,    7,    7,    7, 0x02,
     441,  434,  430,    7, 0x02,
     468,  463,    7,    7, 0x02,
     490,    7,  430,    7, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_CPopUp[] = {
    "CPopUp\0\0size,message\0"
    "showPopup_MessageBox(int,QString)\0"
    "args\0showPopup_Text(QVariant)\0"
    "showNaviPopup_Text(QVariant)\0"
    "showPopup_CustomText(QVariant)\0"
    "showPopup_Warning(QVariant)\0"
    "showPopup_List(QVariant)\0closeSystemPopup()\0"
    "target,res\0popup_Response(int,QVariant)\0"
    "timeout\0startPopupTimer(int)\0"
    "showSystemPopup()\0ignore\0"
    "setignoreAutoClose(bool)\0"
    "showPopup_Toast(QVariant)\0"
    "showPopup_XMAlert(QVariant)\0"
    "showPopUp(QVariant)\0responseButton(int)\0"
    "int\0nAppID\0getScreenOffsetY(int)\0data\0"
    "setSharedMemData(int)\0getSharedMemData()\0"
};

void CPopUp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CPopUp *_t = static_cast<CPopUp *>(_o);
        switch (_id) {
        case 0: _t->showPopup_MessageBox((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->showPopup_Text((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 2: _t->showNaviPopup_Text((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 3: _t->showPopup_CustomText((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 4: _t->showPopup_Warning((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 5: _t->showPopup_List((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 6: _t->closeSystemPopup(); break;
        case 7: _t->popup_Response((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 8: _t->startPopupTimer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->showSystemPopup(); break;
        case 10: _t->setignoreAutoClose((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->showPopup_Toast((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 12: _t->showPopup_XMAlert((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 13: _t->showPopUp((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 14: _t->responseButton((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: { int _r = _t->getScreenOffsetY((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 16: _t->setSharedMemData((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: { int _r = _t->getSharedMemData();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CPopUp::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CPopUp::staticMetaObject = {
    { &QDeclarativeItem::staticMetaObject, qt_meta_stringdata_CPopUp,
      qt_meta_data_CPopUp, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CPopUp::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CPopUp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CPopUp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPopUp))
        return static_cast<void*>(const_cast< CPopUp*>(this));
    return QDeclarativeItem::qt_metacast(_clname);
}

int CPopUp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDeclarativeItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void CPopUp::showPopup_MessageBox(int _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CPopUp::showPopup_Text(QVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CPopUp::showNaviPopup_Text(QVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CPopUp::showPopup_CustomText(QVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CPopUp::showPopup_Warning(QVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CPopUp::showPopup_List(QVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CPopUp::closeSystemPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void CPopUp::popup_Response(int _t1, QVariant _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CPopUp::startPopupTimer(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CPopUp::showSystemPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void CPopUp::setignoreAutoClose(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CPopUp::showPopup_Toast(QVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CPopUp::showPopup_XMAlert(QVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}
QT_END_MOC_NAMESPACE
