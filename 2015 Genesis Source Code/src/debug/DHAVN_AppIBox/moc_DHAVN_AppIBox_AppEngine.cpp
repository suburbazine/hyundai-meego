/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppIBox_AppEngine.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppIBox_AppEngine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppIBox_AppEngine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LocalizeTrigger[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       1,   19, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // properties: name, type, flags
      37,   29, 0x0a495001,

 // properties: notify_signal_id
       0,

       0        // eod
};

static const char qt_meta_stringdata_LocalizeTrigger[] = {
    "LocalizeTrigger\0\0retrigger()\0QString\0"
    "empty\0"
};

void LocalizeTrigger::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LocalizeTrigger *_t = static_cast<LocalizeTrigger *>(_o);
        switch (_id) {
        case 0: _t->retrigger(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData LocalizeTrigger::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LocalizeTrigger::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LocalizeTrigger,
      qt_meta_data_LocalizeTrigger, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LocalizeTrigger::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LocalizeTrigger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LocalizeTrigger::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LocalizeTrigger))
        return static_cast<void*>(const_cast< LocalizeTrigger*>(this));
    return QObject::qt_metacast(_clname);
}

int LocalizeTrigger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
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
void LocalizeTrigger::retrigger()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_IBoxAppEngine[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   15,   14,   14, 0x05,
      53,   46,   14,   14, 0x05,
      78,   72,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     104,   97,   14,   14, 0x0a,
     136,   97,   14,   14, 0x0a,
     172,   97,   14,   14, 0x0a,
     230,  210,   14,   14, 0x0a,
     313,   14,   14,   14, 0x0a,
     322,   14,   14,   14, 0x0a,
     344,   14,   14,   14, 0x0a,
     375,   14,   14,   14, 0x0a,

 // methods: signature, parameters, type, tag, flags
     413,  405,   14,   14, 0x02,
     437,  405,   14,   14, 0x02,
     468,  461,   14,   14, 0x02,
     490,   14,   14,   14, 0x02,
     505,   14,   14,   14, 0x02,
     533,   14,   14,   14, 0x02,
     568,  561,   14,   14, 0x02,
     587,   14,   14,   14, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_IBoxAppEngine[] = {
    "IBoxAppEngine\0\0deckStatus\0setDeckStatus(uint)\0"
    "langId\0retranslateUi(int)\0bShow\0"
    "sigStatusBar(bool)\0aEvent\0"
    "NotifyFromUIStateManager(Event)\0"
    "NotifyFromUIStateManagerRear(Event)\0"
    "NotifyFromUIStateManagerCommon(Event)\0"
    "aEvent,targetScreen\0"
    "NotifyFromUIStateManagerGeneric(Event&,AppEngineQMLConstants::EAPP_SCR"
    "EEN_USAGE_T)\0"
    "reInit()\0onTimeoutResponseFG()\0"
    "onTimeoutResponseFGFrontOnly()\0"
    "onTimeoutResponseFGRearOnly()\0display\0"
    "showDRSPopUp(DISPLAY_T)\0hideDRSPopUp(DISPLAY_T)\0"
    "screen\0backFromDRSPopup(int)\0"
    "showDLNAList()\0setIncrementTrackPosition()\0"
    "setDecrementTrackPosition()\0isPlay\0"
    "setPlayPause(bool)\0EnterDRSSoftBackKey()\0"
};

void IBoxAppEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        IBoxAppEngine *_t = static_cast<IBoxAppEngine *>(_o);
        switch (_id) {
        case 0: _t->setDeckStatus((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 1: _t->retranslateUi((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->sigStatusBar((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->NotifyFromUIStateManager((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 4: _t->NotifyFromUIStateManagerRear((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 5: _t->NotifyFromUIStateManagerCommon((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 6: _t->NotifyFromUIStateManagerGeneric((*reinterpret_cast< Event(*)>(_a[1])),(*reinterpret_cast< AppEngineQMLConstants::EAPP_SCREEN_USAGE_T(*)>(_a[2]))); break;
        case 7: _t->reInit(); break;
        case 8: _t->onTimeoutResponseFG(); break;
        case 9: _t->onTimeoutResponseFGFrontOnly(); break;
        case 10: _t->onTimeoutResponseFGRearOnly(); break;
        case 11: _t->showDRSPopUp((*reinterpret_cast< DISPLAY_T(*)>(_a[1]))); break;
        case 12: _t->hideDRSPopUp((*reinterpret_cast< DISPLAY_T(*)>(_a[1]))); break;
        case 13: _t->backFromDRSPopup((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->showDLNAList(); break;
        case 15: _t->setIncrementTrackPosition(); break;
        case 16: _t->setDecrementTrackPosition(); break;
        case 17: _t->setPlayPause((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: _t->EnterDRSSoftBackKey(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData IBoxAppEngine::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject IBoxAppEngine::staticMetaObject = {
    { &AppVextEngineQML::staticMetaObject, qt_meta_stringdata_IBoxAppEngine,
      qt_meta_data_IBoxAppEngine, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &IBoxAppEngine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *IBoxAppEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *IBoxAppEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IBoxAppEngine))
        return static_cast<void*>(const_cast< IBoxAppEngine*>(this));
    if (!strcmp(_clname, "IViewWidgetController"))
        return static_cast< IViewWidgetController*>(const_cast< IBoxAppEngine*>(this));
    if (!strcmp(_clname, "IStatusBarController"))
        return static_cast< IStatusBarController*>(const_cast< IBoxAppEngine*>(this));
    if (!strcmp(_clname, "IEventSender"))
        return static_cast< IEventSender*>(const_cast< IBoxAppEngine*>(this));
    return AppVextEngineQML::qt_metacast(_clname);
}

int IBoxAppEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppVextEngineQML::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void IBoxAppEngine::setDeckStatus(uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IBoxAppEngine::retranslateUi(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void IBoxAppEngine::sigStatusBar(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
