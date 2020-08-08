/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFramework_AppScreenView.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFramework_AppScreenView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFramework_AppScreenView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppScreenView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      20,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   15,   14,   14, 0x05,
      49,   15,   14,   14, 0x05,
      78,   15,   14,   14, 0x05,
     108,   15,   14,   14, 0x05,
     141,   15,   14,   14, 0x05,
     204,  178,   14,   14, 0x05,
     243,   15,   14,   14, 0x05,
     277,   15,   14,   14, 0x05,
     311,   15,   14,   14, 0x05,
     346,   15,   14,   14, 0x05,
     384,   15,   14,   14, 0x05,
     444,  426,   14,   14, 0x05,
     484,   14,   14,   14, 0x05,
     497,   14,   14,   14, 0x05,
     516,  510,   14,   14, 0x05,
     544,  542,   14,   14, 0x05,
     578,   14,   14,   14, 0x05,
     597,   14,   14,   14, 0x05,
     621,   14,   14,   14, 0x05,
     650,  645,   14,   14, 0x05,

 // methods: signature, parameters, type, tag, flags
     681,   14,  677,   14, 0x02,
     700,   14,   14,   14, 0x02,
     713,   14,  677,   14, 0x02,
     740,   14,  677,   14, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_AppScreenView[] = {
    "AppScreenView\0\0bRRC\0signalJogCenterClicked(bool)\0"
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
    "fgReceived()\0bgReceived()\0event\0"
    "HandleTouchEvent(QEvent*)\0,\0"
    "HandlePopupResponse(int,QVariant)\0"
    "signalManualBeep()\0signalShowSystemPopup()\0"
    "signalHideSystemPopup()\0lang\0"
    "signalLanguageChanged(int)\0int\0"
    "getCurrentScreen()\0ManualBeep()\0"
    "GetCountryVariantFromQML()\0"
    "GetLanguageFromQML()\0"
};

void AppScreenView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppScreenView *_t = static_cast<AppScreenView *>(_o);
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
        case 14: _t->HandleTouchEvent((*reinterpret_cast< QEvent*(*)>(_a[1]))); break;
        case 15: _t->HandlePopupResponse((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 16: _t->signalManualBeep(); break;
        case 17: _t->signalShowSystemPopup(); break;
        case 18: _t->signalHideSystemPopup(); break;
        case 19: _t->signalLanguageChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: { int _r = _t->getCurrentScreen();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 21: _t->ManualBeep(); break;
        case 22: { int _r = _t->GetCountryVariantFromQML();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 23: { int _r = _t->GetLanguageFromQML();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppScreenView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppScreenView::staticMetaObject = {
    { &QDeclarativeView::staticMetaObject, qt_meta_stringdata_AppScreenView,
      qt_meta_data_AppScreenView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppScreenView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppScreenView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppScreenView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppScreenView))
        return static_cast<void*>(const_cast< AppScreenView*>(this));
    return QDeclarativeView::qt_metacast(_clname);
}

int AppScreenView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDeclarativeView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    return _id;
}

// SIGNAL 0
void AppScreenView::signalJogCenterClicked(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppScreenView::signalJogCenterPressed(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppScreenView::signalJogCenterReleased(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppScreenView::signalJogCenterLongPressed(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AppScreenView::signalJogCenterCriticalPressed(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AppScreenView::signalJogNavigation(uint _t1, int _t2, bool _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void AppScreenView::signalPopupJogCenterClicked(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void AppScreenView::signalPopupJogCenterPressed(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void AppScreenView::signalPopupJogCenterReleased(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void AppScreenView::signalPopupJogCenterLongPressed(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void AppScreenView::signalPopupJogCenterCriticalPressed(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void AppScreenView::signalPopupJogNavigation(uint _t1, int _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void AppScreenView::fgReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void AppScreenView::bgReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void AppScreenView::HandleTouchEvent(QEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void AppScreenView::HandlePopupResponse(int _t1, QVariant _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void AppScreenView::signalManualBeep()
{
    QMetaObject::activate(this, &staticMetaObject, 16, 0);
}

// SIGNAL 17
void AppScreenView::signalShowSystemPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 17, 0);
}

// SIGNAL 18
void AppScreenView::signalHideSystemPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 18, 0);
}

// SIGNAL 19
void AppScreenView::signalLanguageChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}
QT_END_MOC_NAMESPACE
