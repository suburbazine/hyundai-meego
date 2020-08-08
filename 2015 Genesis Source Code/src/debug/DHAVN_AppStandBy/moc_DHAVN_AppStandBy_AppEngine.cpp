/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppStandBy_AppEngine.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppStandBy_AppEngine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppStandBy_AppEngine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppEngineEnum[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
      14, 0x0,    3,   18,

 // enum data: key, value
      28, uint(AppEngineEnum::APP_DISPLAY_FRONT),
      46, uint(AppEngineEnum::APP_DISPLAY_REAR),
      63, uint(AppEngineEnum::APP_DISPLAY_NONE),

       0        // eod
};

static const char qt_meta_stringdata_AppEngineEnum[] = {
    "AppEngineEnum\0APP_DISPLAY_T\0"
    "APP_DISPLAY_FRONT\0APP_DISPLAY_REAR\0"
    "APP_DISPLAY_NONE\0"
};

void AppEngineEnum::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData AppEngineEnum::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppEngineEnum::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppEngineEnum,
      qt_meta_data_AppEngineEnum, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppEngineEnum::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppEngineEnum::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppEngineEnum::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppEngineEnum))
        return static_cast<void*>(const_cast< AppEngineEnum*>(this));
    return QObject::qt_metacast(_clname);
}

int AppEngineEnum::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_StandByAppEngine[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      40,   14, // methods
       1,  214, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      24,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,
      26,   17,   17,   17, 0x05,
      44,   40,   17,   17, 0x05,
      56,   17,   17,   17, 0x05,
      83,   17,   17,   17, 0x05,
     112,   17,   17,   17, 0x05,
     137,  129,   17,   17, 0x05,
     185,   17,   17,   17, 0x05,
     201,   17,   17,   17, 0x05,
     242,  226,   17,   17, 0x05,
     277,   17,   17,   17, 0x05,
     308,  297,   17,   17, 0x05,
     347,  332,   17,   17, 0x05,
     379,  370,   17,   17, 0x05,
     411,  401,   17,   17, 0x05,
     430,   17,   17,   17, 0x05,
     454,   17,   17,   17, 0x05,
     480,  465,   17,   17, 0x05,
     515,  498,   17,   17, 0x05,
     544,  537,   17,   17, 0x05,
     558,   17,   17,   17, 0x05,
     592,  570,   17,   17, 0x05,
     625,  615,   17,   17, 0x05,
     657,  648,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     684,  677,   17,   17, 0x0a,
     716,  677,   17,   17, 0x0a,
     752,  677,   17,   17, 0x0a,
     790,   17,   17,   17, 0x0a,
     805,   17,   17,   17, 0x0a,

 // methods: signature, parameters, type, tag, flags
     852,  837,   17,   17, 0x02,
     887,  879,   17,   17, 0x02,
     905,   17,  901,   17, 0x02,
     926,   17,  921,   17, 0x02,
     944,   17,  901,   17, 0x02,
     965,   17,   17,   17, 0x02,
     995,  986,   17,   17, 0x02,
    1015,   17,   17,   17, 0x02,
    1039, 1031,   17,   17, 0x02,
    1060,   17,  901,   17, 0x02,
    1069,   17,   17,   17, 0x02,

 // properties: name, type, flags
    1093, 1085, 0x0a495001,

 // properties: notify_signal_id
       7,

       0        // eod
};

static const char qt_meta_stringdata_StandByAppEngine[] = {
    "StandByAppEngine\0\0setFG()\0setFile(bool)\0"
    "val\0appFG(bool)\0sendTimeoutToUpdateClock()\0"
    "sendTimeoutToExternalClock()\0"
    "setGPSTime(bool)\0message\0"
    "settingsChanged(EVENT_SETTINGS_CHANGED_DATA_T*)\0"
    "retranslateUI()\0signalLoadingCompleted()\0"
    "status,screenId\0signalAutoDisappearTimer(bool,int)\0"
    "signalLangChanged()\0mainScreen\0"
    "signalSetLanguage(bool)\0index,screenId\0"
    "setFocusIndex(int,int)\0bEmergen\0"
    "signalEmergency(bool)\0launchApp\0"
    "setLaunchApp(bool)\0signalSetDefaultFocus()\0"
    "signalFg()\0state,screenId\0qmlLoad(bool,int)\0"
    "setRear,screenId\0setRearLogo(bool,int)\0"
    "status\0clockFG(bool)\0afterBoot()\0"
    "status,arrow,screenId\0signalJog(int,int,int)\0"
    "disappear\0setAutoDisappear(bool)\0"
    "screenId\0backKeyPressed(int)\0aEvent\0"
    "NotifyFromUIStateManager(Event)\0"
    "NotifyFromUIStateManagerRear(Event)\0"
    "NotifyFromUIStateManagerCommon(Event)\0"
    "timeoutTimer()\0ChangedPowerState(QDBusMessage)\0"
    "appId,nDisplay\0LaunchApplication(int,int)\0"
    "pressed\0setTime(bool)\0int\0GetLanguageID()\0"
    "bool\0CheckNaviStatus()\0CheckVehicleStatus()\0"
    "HandleBackLanguage()\0language\0"
    "ChangeLanguage(int)\0playAudioBeep()\0"
    "visible\0showToastPopup(bool)\0getEvv()\0"
    "HandleBackKey()\0QString\0empty\0"
};

void StandByAppEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        StandByAppEngine *_t = static_cast<StandByAppEngine *>(_o);
        switch (_id) {
        case 0: _t->setFG(); break;
        case 1: _t->setFile((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->appFG((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->sendTimeoutToUpdateClock(); break;
        case 4: _t->sendTimeoutToExternalClock(); break;
        case 5: _t->setGPSTime((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->settingsChanged((*reinterpret_cast< EVENT_SETTINGS_CHANGED_DATA_T*(*)>(_a[1]))); break;
        case 7: _t->retranslateUI(); break;
        case 8: _t->signalLoadingCompleted(); break;
        case 9: _t->signalAutoDisappearTimer((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->signalLangChanged(); break;
        case 11: _t->signalSetLanguage((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->setFocusIndex((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: _t->signalEmergency((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->setLaunchApp((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->signalSetDefaultFocus(); break;
        case 16: _t->signalFg(); break;
        case 17: _t->qmlLoad((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->setRearLogo((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 19: _t->clockFG((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: _t->afterBoot(); break;
        case 21: _t->signalJog((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 22: _t->setAutoDisappear((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 23: _t->backKeyPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->NotifyFromUIStateManager((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 25: _t->NotifyFromUIStateManagerRear((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 26: _t->NotifyFromUIStateManagerCommon((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 27: _t->timeoutTimer(); break;
        case 28: _t->ChangedPowerState((*reinterpret_cast< QDBusMessage(*)>(_a[1]))); break;
        case 29: _t->LaunchApplication((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 30: _t->setTime((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 31: { int _r = _t->GetLanguageID();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 32: { bool _r = _t->CheckNaviStatus();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 33: { int _r = _t->CheckVehicleStatus();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 34: _t->HandleBackLanguage(); break;
        case 35: _t->ChangeLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->playAudioBeep(); break;
        case 37: _t->showToastPopup((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 38: { int _r = _t->getEvv();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 39: _t->HandleBackKey(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData StandByAppEngine::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject StandByAppEngine::staticMetaObject = {
    { &AppVextEngineQML::staticMetaObject, qt_meta_stringdata_StandByAppEngine,
      qt_meta_data_StandByAppEngine, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StandByAppEngine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StandByAppEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StandByAppEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StandByAppEngine))
        return static_cast<void*>(const_cast< StandByAppEngine*>(this));
    return AppVextEngineQML::qt_metacast(_clname);
}

int StandByAppEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppVextEngineQML::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = empty(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
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
void StandByAppEngine::setFG()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void StandByAppEngine::setFile(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void StandByAppEngine::appFG(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void StandByAppEngine::sendTimeoutToUpdateClock()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void StandByAppEngine::sendTimeoutToExternalClock()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void StandByAppEngine::setGPSTime(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void StandByAppEngine::settingsChanged(EVENT_SETTINGS_CHANGED_DATA_T * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void StandByAppEngine::retranslateUI()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void StandByAppEngine::signalLoadingCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void StandByAppEngine::signalAutoDisappearTimer(bool _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void StandByAppEngine::signalLangChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}

// SIGNAL 11
void StandByAppEngine::signalSetLanguage(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void StandByAppEngine::setFocusIndex(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void StandByAppEngine::signalEmergency(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void StandByAppEngine::setLaunchApp(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void StandByAppEngine::signalSetDefaultFocus()
{
    QMetaObject::activate(this, &staticMetaObject, 15, 0);
}

// SIGNAL 16
void StandByAppEngine::signalFg()
{
    QMetaObject::activate(this, &staticMetaObject, 16, 0);
}

// SIGNAL 17
void StandByAppEngine::qmlLoad(bool _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void StandByAppEngine::setRearLogo(bool _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void StandByAppEngine::clockFG(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void StandByAppEngine::afterBoot()
{
    QMetaObject::activate(this, &staticMetaObject, 20, 0);
}

// SIGNAL 21
void StandByAppEngine::signalJog(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void StandByAppEngine::setAutoDisappear(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void StandByAppEngine::backKeyPressed(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}
QT_END_MOC_NAMESPACE
