/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFramework_AppEngineQML_Def.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFramework_AppEngineQML_Def.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFramework_AppEngineQML_Def.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppEngineQML[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      17,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   14,   13,   13, 0x05,
      48,   14,   13,   13, 0x05,
      77,   14,   13,   13, 0x05,
     107,   14,   13,   13, 0x05,
     140,   14,   13,   13, 0x05,
     203,  177,   13,   13, 0x05,
     242,   14,   13,   13, 0x05,
     276,   14,   13,   13, 0x05,
     310,   14,   13,   13, 0x05,
     345,   14,   13,   13, 0x05,
     383,   14,   13,   13, 0x05,
     443,  425,   13,   13, 0x05,
     483,   13,   13,   13, 0x05,
     496,   13,   13,   13, 0x05,
     509,   13,   13,   13, 0x05,
     533,   13,   13,   13, 0x05,
     562,  557,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     605,  589,   13,   13, 0x0a,
     639,   13,   13,   13, 0x08,
     660,   13,   13,   13, 0x08,

 // methods: signature, parameters, type, tag, flags
     693,  680,   13,   13, 0x02,
     721,   13,   13,   13, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_AppEngineQML[] = {
    "AppEngineQML\0\0bRRC\0signalJogCenterClicked(bool)\0"
    "signalJogCenterPressed(bool)\0"
    "signalJogCenterReleased(bool)\0"
    "signalJogCenterLongPressed(bool)\0"
    "signalJogCenterCriticalPressed(bool)\0"
    "arrow,status,bRRC,nRepeat\0"
    "signalJogNavigation(uint,int,bool,int)\0"
    "signalPopupJogCenterClicked(bool)\0"
    "signalPopupJogCenterPressed(bool)\0"
    "signalPopupJogCenterReleased(bool)\0"
    "signalPopupJogCenterLongPressed(bool)\0"
    "signalPopupJogCenterCriticalPressed(bool)\0"
    "arrow,status,bRRC\0"
    "signalPopupJogNavigation(uint,int,bool)\0"
    "fgReceived()\0bgReceived()\0"
    "signalShowSystemPopup()\0signalHideSystemPopup()\0"
    "lang\0signalLanguageChanged(int)\0"
    "target,response\0handlePopupResponse(int,QVariant)\0"
    "SlotCallFgResponse()\0DoTimedTransition()\0"
    "arrow,status\0testKeyClickEvents(int,int)\0"
    "ManualBeep()\0"
};

void AppEngineQML::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppEngineQML *_t = static_cast<AppEngineQML *>(_o);
        switch (_id) {
        case 0: _t->signalJogCenterClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->signalJogCenterPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->signalJogCenterReleased((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->signalJogCenterLongPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->signalJogCenterCriticalPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->signalJogNavigation((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 6: _t->signalPopupJogCenterClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->signalPopupJogCenterPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->signalPopupJogCenterReleased((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->signalPopupJogCenterLongPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->signalPopupJogCenterCriticalPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->signalPopupJogNavigation((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 12: _t->fgReceived(); break;
        case 13: _t->bgReceived(); break;
        case 14: _t->signalShowSystemPopup(); break;
        case 15: _t->signalHideSystemPopup(); break;
        case 16: _t->signalLanguageChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->handlePopupResponse((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 18: _t->SlotCallFgResponse(); break;
        case 19: _t->DoTimedTransition(); break;
        case 20: _t->testKeyClickEvents((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 21: _t->ManualBeep(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppEngineQML::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppEngineQML::staticMetaObject = {
    { &AppEngineBase::staticMetaObject, qt_meta_stringdata_AppEngineQML,
      qt_meta_data_AppEngineQML, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppEngineQML::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppEngineQML::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppEngineQML::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppEngineQML))
        return static_cast<void*>(const_cast< AppEngineQML*>(this));
    return AppEngineBase::qt_metacast(_clname);
}

int AppEngineQML::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppEngineBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void AppEngineQML::signalJogCenterClicked(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppEngineQML::signalJogCenterPressed(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppEngineQML::signalJogCenterReleased(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppEngineQML::signalJogCenterLongPressed(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AppEngineQML::signalJogCenterCriticalPressed(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AppEngineQML::signalJogNavigation(uint _t1, int _t2, bool _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void AppEngineQML::signalPopupJogCenterClicked(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void AppEngineQML::signalPopupJogCenterPressed(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void AppEngineQML::signalPopupJogCenterReleased(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void AppEngineQML::signalPopupJogCenterLongPressed(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void AppEngineQML::signalPopupJogCenterCriticalPressed(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void AppEngineQML::signalPopupJogNavigation(uint _t1, int _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void AppEngineQML::fgReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void AppEngineQML::bgReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void AppEngineQML::signalShowSystemPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 14, 0);
}

// SIGNAL 15
void AppEngineQML::signalHideSystemPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 15, 0);
}

// SIGNAL 16
void AppEngineQML::signalLanguageChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}
QT_END_MOC_NAMESPACE
