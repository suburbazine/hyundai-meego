/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppHomeScreen_AppEngine.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppHomeScreen_AppEngine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppHomeScreen_AppEngine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppEngine[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       1,  134, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   11,   10,   10, 0x05,
      87,   49,   10,   10, 0x05,
     122,  115,   10,   10, 0x05,
     151,  138,   10,   10, 0x05,
     201,  183,   10,   10, 0x05,
     227,  115,   10,   10, 0x05,
     247,  115,   10,   10, 0x05,
     267,   10,   10,   10, 0x05,
     283,  115,   10,   10, 0x05,
     305,  299,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     339,  332,   10,   10, 0x0a,
     371,  332,   10,   10, 0x0a,
     407,  332,   10,   10, 0x0a,
     476,  445,   10,   10, 0x0a,
     577,   10,   10,   10, 0x08,
     602,   10,  598,   10, 0x08,
     619,  299,   10,   10, 0x08,

 // methods: signature, parameters, type, tag, flags
     647,  642,   10,   10, 0x02,
     690,   10,   10,   10, 0x02,
     710,  115,   10,   10, 0x02,
     730,   10,   10,   10, 0x02,
     747,   10,   10,   10, 0x02,
     770,  766,  758,   10, 0x02,
     788,  784,   10,   10, 0x02,

 // properties: name, type, flags
     807,  598, 0x02495103,

 // properties: notify_signal_id
       9,

       0        // eod
};

static const char qt_meta_stringdata_AppEngine[] = {
    "AppEngine\0\0screen,animation\0"
    "goMainMenu(int,bool)\0"
    "screen,animation,subMenuId,focusIndex\0"
    "goSubMenu(int,bool,int,int)\0screen\0"
    "goHelpMenu(int)\0screen,index\0"
    "setMediaMenuFocusIndex(int,int)\0"
    "bLaunching,screen\0sigAppLaunching(bool,int)\0"
    "receiveFGEvent(int)\0receiveBGEvent(int)\0"
    "signalBackKey()\0closepopup(int)\0value\0"
    "countryVariantChanged(int)\0aEvent\0"
    "NotifyFromUIStateManager(Event)\0"
    "NotifyFromUIStateManagerRear(Event)\0"
    "NotifyFromUIStateManagerCommon(Event)\0"
    "aEvent,pViewController,nScreen\0"
    "NotifyFromUIStateManagerGeneric(Event,ViewController*,AppEngineQMLCons"
    "tants::EAPP_SCREEN_GEOMETRY_T)\0"
    "resetAppLaunchData()\0int\0countryVariant()\0"
    "setCountryVariant(int)\0,,,,\0"
    "LaunchApplication(int,int,int,int,QString)\0"
    "BackKeyHandler(int)\0hideLocalPopup(int)\0"
    "setBeepCommand()\0playBeep()\0QString\0"
    "hdb\0getFont(bool)\0log\0logForQML(QString)\0"
    "countryVariant\0"
};

void AppEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppEngine *_t = static_cast<AppEngine *>(_o);
        switch (_id) {
        case 0: _t->goMainMenu((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->goSubMenu((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 2: _t->goHelpMenu((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setMediaMenuFocusIndex((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->sigAppLaunching((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->receiveFGEvent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->receiveBGEvent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->signalBackKey(); break;
        case 8: _t->closepopup((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->countryVariantChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->NotifyFromUIStateManager((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 11: _t->NotifyFromUIStateManagerRear((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 12: _t->NotifyFromUIStateManagerCommon((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 13: _t->NotifyFromUIStateManagerGeneric((*reinterpret_cast< Event(*)>(_a[1])),(*reinterpret_cast< ViewController*(*)>(_a[2])),(*reinterpret_cast< AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T(*)>(_a[3]))); break;
        case 14: _t->resetAppLaunchData(); break;
        case 15: { int _r = _t->countryVariant();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 16: _t->setCountryVariant((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->LaunchApplication((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 18: _t->BackKeyHandler((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->hideLocalPopup((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->setBeepCommand(); break;
        case 21: _t->playBeep(); break;
        case 22: { QString _r = _t->getFont((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 23: _t->logForQML((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppEngine::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppEngine::staticMetaObject = {
    { &AppVextEngineQML::staticMetaObject, qt_meta_stringdata_AppEngine,
      qt_meta_data_AppEngine, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppEngine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppEngine))
        return static_cast<void*>(const_cast< AppEngine*>(this));
    return AppVextEngineQML::qt_metacast(_clname);
}

int AppEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppVextEngineQML::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = countryVariant(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setCountryVariant(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void AppEngine::goMainMenu(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppEngine::goSubMenu(int _t1, bool _t2, int _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppEngine::goHelpMenu(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppEngine::setMediaMenuFocusIndex(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AppEngine::sigAppLaunching(bool _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AppEngine::receiveFGEvent(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void AppEngine::receiveBGEvent(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void AppEngine::signalBackKey()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void AppEngine::closepopup(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void AppEngine::countryVariantChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}
QT_END_MOC_NAMESPACE
