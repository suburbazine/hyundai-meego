/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_ScreenTitleDiscMenu.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenTitleDiscMenu.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_ScreenTitleDiscMenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_ScreenTitleDiscMenu[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      49,   42,   41,   41, 0x05,
      74,   67,   41,   41, 0x05,
     108,   98,   41,   41, 0x05,
     133,   98,   41,   41, 0x05,
     167,  160,   41,   41, 0x05,

 // slots: signature, parameters, type, tag, flags
     201,   41,   41,   41, 0x0a,
     214,  210,   41,   41, 0x0a,
     231,   41,   41,   41, 0x0a,
     255,  250,   41,   41, 0x0a,
     321,  281,   41,   41, 0x0a,
     370,  367,   41,   41, 0x0a,
     390,   41,   41,   41, 0x0a,
     408,   41,   41,   41, 0x0a,
     438,  428,   41,   41, 0x0a,
     453,   41,   41,   41, 0x0a,
     465,   41,   41,   41, 0x0a,
     481,  474,   41,   41, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_ScreenTitleDiscMenu[] = {
    "AppMediaPlayer_Video_ScreenTitleDiscMenu\0"
    "\0onShow\0showLockout(bool)\0onHide\0"
    "hideTitleDiscMenu(bool)\0onLoading\0"
    "loadingModeChanged(bool)\0"
    "fgLoadingModeChanged(bool)\0m_disp\0"
    "titleDiscLoadingDisplayCheck(int)\0"
    "onHide()\0arg\0onShow(QVariant)\0"
    "onMenuKeyPressed()\0item\0"
    "onOptionsItemClicked(int)\0"
    "srcScreenName,resScreenName,eventId,arg\0"
    "onEventReceived(QString,QString,int,QVariant)\0"
    "on\0onLockoutMode(bool)\0onRetranslateUI()\0"
    "listButtonHandler()\0direction\0"
    "cursorKey(int)\0titleMenu()\0select()\0"
    "nState\0onScreenStateChanged(int)\0"
};

void AppMediaPlayer_Video_ScreenTitleDiscMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_ScreenTitleDiscMenu *_t = static_cast<AppMediaPlayer_Video_ScreenTitleDiscMenu *>(_o);
        switch (_id) {
        case 0: _t->showLockout((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->hideTitleDiscMenu((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->loadingModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->fgLoadingModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->titleDiscLoadingDisplayCheck((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onHide(); break;
        case 6: _t->onShow((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 7: _t->onMenuKeyPressed(); break;
        case 8: _t->onOptionsItemClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->onEventReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 10: _t->onLockoutMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->onRetranslateUI(); break;
        case 12: _t->listButtonHandler(); break;
        case 13: _t->cursorKey((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->titleMenu(); break;
        case 15: _t->select(); break;
        case 16: _t->onScreenStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_ScreenTitleDiscMenu::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_ScreenTitleDiscMenu::staticMetaObject = {
    { &AppMediaPlayer_Video_Screen::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_ScreenTitleDiscMenu,
      qt_meta_data_AppMediaPlayer_Video_ScreenTitleDiscMenu, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_ScreenTitleDiscMenu::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_ScreenTitleDiscMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_ScreenTitleDiscMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_ScreenTitleDiscMenu))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_ScreenTitleDiscMenu*>(this));
    return AppMediaPlayer_Video_Screen::qt_metacast(_clname);
}

int AppMediaPlayer_Video_ScreenTitleDiscMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppMediaPlayer_Video_Screen::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_ScreenTitleDiscMenu::showLockout(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_ScreenTitleDiscMenu::hideTitleDiscMenu(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Video_ScreenTitleDiscMenu::loadingModeChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppMediaPlayer_Video_ScreenTitleDiscMenu::fgLoadingModeChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AppMediaPlayer_Video_ScreenTitleDiscMenu::titleDiscLoadingDisplayCheck(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
