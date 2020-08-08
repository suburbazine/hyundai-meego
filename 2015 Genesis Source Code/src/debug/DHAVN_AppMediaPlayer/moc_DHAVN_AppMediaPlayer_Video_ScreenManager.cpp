/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_ScreenManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_ScreenManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_ScreenManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      49,   14, // methods
       1,  259, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      36,   35,   35,   35, 0x05,
      51,   45,   35,   35, 0x05,
      79,   45,   35,   35, 0x05,
     111,  106,   35,   35, 0x05,
     137,  134,   35,   35, 0x05,
     158,   35,   35,   35, 0x05,

 // slots: signature, parameters, type, tag, flags
     189,  174,   35,   35, 0x0a,
     231,  220,   35,   35, 0x2a,
     253,  174,   35,   35, 0x0a,
     282,  220,   35,   35, 0x2a,
     306,  302,   35,   35, 0x0a,
     326,   35,   35,   35, 0x2a,
     343,   35,  338,   35, 0x0a,
     358,  174,   35,   35, 0x0a,
     390,  220,   35,   35, 0x2a,
     439,  413,   35,   35, 0x0a,
     504,  484,   35,   35, 0x2a,
     560,  544,   35,   35, 0x2a,
     615,  591,   35,   35, 0x0a,
     661,   35,   35,   35, 0x0a,
     672,   35,   35,   35, 0x0a,
     691,   35,   35,   35, 0x0a,
     719,  710,   35,   35, 0x0a,
     741,   35,   35,   35, 0x0a,
     800,  760,   35,   35, 0x0a,
     888,  852,   35,   35, 0x2a,
     935,   35,  931,   35, 0x0a,
     953,   35,  931,   35, 0x0a,
     979,   45,   35,   35, 0x0a,
    1022, 1015,   35,   35, 0x0a,
    1040,  106,   35,   35, 0x0a,
    1062,   35,   35,   35, 0x0a,
    1083,   35,   35,   35, 0x0a,
    1102,   35,   35,   35, 0x0a,
    1128, 1120,   35,   35, 0x0a,
    1151,   35,  338,   35, 0x0a,
    1170,   35,  338,   35, 0x0a,
    1205, 1181,   35,   35, 0x08,
    1266,  220,   35,   35, 0x08,
    1286,  220,   35,   35, 0x08,
    1327, 1309,   35,   35, 0x08,
    1374, 1352,   35,   35, 0x08,
    1421, 1404,   35,   35, 0x28,
    1446,   45,   35,   35, 0x08,
    1467,   35,   35,   35, 0x28,

 // methods: signature, parameters, type, tag, flags
    1490, 1484,   35,   35, 0x02,
    1511,   35,   35,   35, 0x22,
    1529,   35,   35,   35, 0x02,
    1572, 1560,   35,   35, 0x02,

 // properties: name, type, flags
    1629, 1613, 0x00095009,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_ScreenManager[] = {
    "AppMediaPlayer_Video_ScreenManager\0\0"
    "hideUI()\0value\0isFocusVisibleChanged(bool)\0"
    "focusActivityChanged(bool)\0isFG\0"
    "fgBgStateChanged(bool)\0on\0"
    "loadLockoutTab(bool)\0popFullScreen()\0"
    "screenName,arg\0changeScreen(QString,QVariant)\0"
    "screenName\0changeScreen(QString)\0"
    "pushScreen(QString,QVariant)\0"
    "pushScreen(QString)\0arg\0popScreen(QVariant)\0"
    "popScreen()\0bool\0revertScreen()\0"
    "replaceScreen(QString,QVariant)\0"
    "replaceScreen(QString)\0screenName,step,arg,bHide\0"
    "replacePrevScreen(QString,int,QVariant,bool)\0"
    "screenName,step,arg\0"
    "replacePrevScreen(QString,int,QVariant)\0"
    "screenName,step\0replacePrevScreen(QString,int)\0"
    "itemName,property,value\0"
    "setItemProperty(QString,const char*,QVariant)\0"
    "onHideUI()\0onPrevKeyPressed()\0"
    "onNextKeyPressed()\0nKeyType\0"
    "onMenuKeyPressed(int)\0onBackKeyPressed()\0"
    "srcScreenName,resScreenName,eventId,arg\0"
    "onNotifyScreenManager(QString,QString,int,QVariant)\0"
    "srcScreenName,resScreenName,eventId\0"
    "onNotifyScreenManager(QString,QString,int)\0"
    "int\0getScreensCount()\0getControllerFocusIndex()\0"
    "setFocusActivefromEngineVideo(bool)\0"
    "onShow\0lockoutMode(bool)\0fgBgStateChange(bool)\0"
    "closeProgressPopup()\0closeOptionPopup()\0"
    "closeFullScreen()\0lockout\0"
    "setIsLockoutMode(bool)\0getIsLockoutMode()\0"
    "getIsRRC()\0viewName,pView,pContext\0"
    "onViewLoaded(QString,QDeclarativeItem*,QDeclarativeContext*)\0"
    "addOverlay(QString)\0removeOverlay(QString)\0"
    "direction,focusId\0handleFocusLost(int,int)\0"
    "direction,status,bRRC\0"
    "handleJogEvent(uint,int,bool)\0"
    "direction,status\0handleJogEvent(uint,int)\0"
    "setFocusActive(bool)\0setFocusActive()\0"
    "arrow\0setDefaultFocus(int)\0setDefaultFocus()\0"
    "setFocusFromModeAreaToScreen()\0"
    "mode,screen\0setupFocusSequence(FOCUS_MODE_T,QString)\0"
    "VPEnum::VDISP_T\0disp\0"
};

void AppMediaPlayer_Video_ScreenManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_ScreenManager *_t = static_cast<AppMediaPlayer_Video_ScreenManager *>(_o);
        switch (_id) {
        case 0: _t->hideUI(); break;
        case 1: _t->isFocusVisibleChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->focusActivityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->fgBgStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->loadLockoutTab((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->popFullScreen(); break;
        case 6: _t->changeScreen((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 7: _t->changeScreen((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->pushScreen((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 9: _t->pushScreen((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->popScreen((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 11: _t->popScreen(); break;
        case 12: { bool _r = _t->revertScreen();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 13: _t->replaceScreen((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 14: _t->replaceScreen((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: _t->replacePrevScreen((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 16: _t->replacePrevScreen((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        case 17: _t->replacePrevScreen((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->setItemProperty((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const char*(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 19: _t->onHideUI(); break;
        case 20: _t->onPrevKeyPressed(); break;
        case 21: _t->onNextKeyPressed(); break;
        case 22: _t->onMenuKeyPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->onBackKeyPressed(); break;
        case 24: _t->onNotifyScreenManager((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< QVariant(*)>(_a[4]))); break;
        case 25: _t->onNotifyScreenManager((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 26: { int _r = _t->getScreensCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 27: { int _r = _t->getControllerFocusIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 28: _t->setFocusActivefromEngineVideo((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 29: _t->lockoutMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 30: _t->fgBgStateChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 31: _t->closeProgressPopup(); break;
        case 32: _t->closeOptionPopup(); break;
        case 33: _t->closeFullScreen(); break;
        case 34: _t->setIsLockoutMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 35: { bool _r = _t->getIsLockoutMode();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 36: { bool _r = _t->getIsRRC();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 37: _t->onViewLoaded((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QDeclarativeItem*(*)>(_a[2])),(*reinterpret_cast< QDeclarativeContext*(*)>(_a[3]))); break;
        case 38: _t->addOverlay((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 39: _t->removeOverlay((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 40: _t->handleFocusLost((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 41: _t->handleJogEvent((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 42: _t->handleJogEvent((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 43: _t->setFocusActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 44: _t->setFocusActive(); break;
        case 45: _t->setDefaultFocus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 46: _t->setDefaultFocus(); break;
        case 47: _t->setFocusFromModeAreaToScreen(); break;
        case 48: _t->setupFocusSequence((*reinterpret_cast< FOCUS_MODE_T(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

#ifdef Q_NO_DATA_RELOCATION
static const QMetaObjectAccessor qt_meta_extradata_AppMediaPlayer_Video_ScreenManager[] = {
        VPEnum::getStaticMetaObject,
#else
static const QMetaObject *qt_meta_extradata_AppMediaPlayer_Video_ScreenManager[] = {
        &VPEnum::staticMetaObject,
#endif //Q_NO_DATA_RELOCATION
    0
};

const QMetaObjectExtraData AppMediaPlayer_Video_ScreenManager::staticMetaObjectExtraData = {
    qt_meta_extradata_AppMediaPlayer_Video_ScreenManager,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_ScreenManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_ScreenManager,
      qt_meta_data_AppMediaPlayer_Video_ScreenManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_ScreenManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_ScreenManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_ScreenManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_ScreenManager))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_ScreenManager*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_Video_ScreenManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 49)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 49;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< VPEnum::VDISP_T*>(_v) = disp(); break;
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
void AppMediaPlayer_Video_ScreenManager::hideUI()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void AppMediaPlayer_Video_ScreenManager::isFocusVisibleChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Video_ScreenManager::focusActivityChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppMediaPlayer_Video_ScreenManager::fgBgStateChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AppMediaPlayer_Video_ScreenManager::loadLockoutTab(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AppMediaPlayer_Video_ScreenManager::popFullScreen()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
