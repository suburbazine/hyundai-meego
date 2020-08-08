/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_ScreenDVDPlayback.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenDVDPlayback.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_ScreenDVDPlayback.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_ScreenDVDPlayback[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      44,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      40,   39,   39,   39, 0x05,
      62,   57,   39,   39, 0x05,
      86,   80,   39,   39, 0x05,
     113,   39,   39,   39, 0x05,
     132,   39,   39,   39, 0x05,
     153,  146,   39,   39, 0x05,

 // slots: signature, parameters, type, tag, flags
     171,   39,   39,   39, 0x0a,
     184,  180,   39,   39, 0x0a,
     201,   39,   39,   39, 0x0a,
     216,   39,   39,   39, 0x0a,
     240,  235,   39,   39, 0x0a,
     306,  266,   39,   39, 0x0a,
     352,   39,   39,   39, 0x0a,
     369,   39,   39,   39, 0x0a,
     394,  387,   39,   39, 0x0a,
     415,   39,   39,   39, 0x0a,
     434,   39,   39,   39, 0x0a,
     455,   39,   39,   39, 0x0a,
     478,  471,   39,   39, 0x0a,
     504,  471,   39,   39, 0x0a,
     538,  471,   39,   39, 0x0a,
     568,   39,   39,   39, 0x0a,
     586,   39,   39,   39, 0x0a,
     607,   39,   39,   39, 0x0a,
     627,   39,   39,   39, 0x0a,
     639,   39,   39,   39, 0x0a,
     651,   39,   39,   39, 0x0a,
     666,   39,   39,   39, 0x0a,
     681,   39,   39,   39, 0x0a,
     697,   39,   39,   39, 0x0a,
     721,   39,   39,   39, 0x0a,
     737,   39,   39,   39, 0x0a,
     761,   39,   39,   39, 0x0a,
     779,   39,   39,   39, 0x0a,
     795,   39,   39,   39, 0x0a,
     875,  872,   39,   39, 0x0a,
     895,  872,   39,   39, 0x0a,
     915,  872,   39,   39, 0x0a,
     942,  936,   39,   39, 0x0a,
     973,  872,   39,   39, 0x0a,
     992,   39,   39,   39, 0x0a,
    1023, 1010,   39,   39, 0x08,
    1049,   39,   39,   39, 0x08,
    1066,  872,   39,   39, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_ScreenDVDPlayback[] = {
    "AppMediaPlayer_Video_ScreenDVDPlayback\0"
    "\0resetSpeedIcon()\0rate\0setSpeedIcon(int)\0"
    "bShow\0showPlaybackControls(bool)\0"
    "releaseLongPress()\0cancelFfRew()\0"
    "onShow\0showLockout(bool)\0onHide()\0arg\0"
    "onShow(QVariant)\0onDeactivate()\0"
    "onMenuKeyPressed()\0item\0"
    "onOptionsItemClicked(int)\0"
    "srcScreenName,resScreenName,eventId,arg\0"
    "onEventReceived(QString,QString,int,QVariant)\0"
    "onMousePressed()\0onMouseReleased()\0"
    "pbArea\0onMouseClicked(bool)\0"
    "onModeAreaTapped()\0onButtonAreaTapped()\0"
    "onSoftkeyBack()\0button\0onSoftkeyBtnHold(QString)\0"
    "onSoftkeyBtnHoldCritical(QString)\0"
    "onSoftkeyBtnReleased(QString)\0"
    "onTogglePlayBtn()\0onLongPressPlayBtn()\0"
    "listButtonHandler()\0onPrevBtn()\0"
    "onNextBtn()\0onFlick_next()\0onFlick_prev()\0"
    "onNextBtnHold()\0onNextBtnHoldCritical()\0"
    "onPrevBtnHold()\0onPrevBtnHoldCritical()\0"
    "onCancel_ff_rew()\0onSelectTuned()\0"
    "onPlaybackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STA"
    "TUS_T)\0"
    "on\0onLockoutMode(bool)\0onSetTuneMode(bool)\0"
    "onTuneSameFile(bool)\0value\0"
    "onFocusVisibilityChanged(bool)\0"
    "onSetCamMode(bool)\0onRetranslateUI()\0"
    "nScreenState\0onScreenStateChanged(int)\0"
    "showFullScreen()\0onFgLoading(bool)\0"
};

void AppMediaPlayer_Video_ScreenDVDPlayback::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_ScreenDVDPlayback *_t = static_cast<AppMediaPlayer_Video_ScreenDVDPlayback *>(_o);
        switch (_id) {
        case 0: _t->resetSpeedIcon(); break;
        case 1: _t->setSpeedIcon((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->showPlaybackControls((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->releaseLongPress(); break;
        case 4: _t->cancelFfRew(); break;
        case 5: _t->showLockout((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->onHide(); break;
        case 7: _t->onShow((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 8: _t->onDeactivate(); break;
        case 9: _t->onMenuKeyPressed(); break;
        case 10: _t->onOptionsItemClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->onEventReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 12: _t->onMousePressed(); break;
        case 13: _t->onMouseReleased(); break;
        case 14: _t->onMouseClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->onModeAreaTapped(); break;
        case 16: _t->onButtonAreaTapped(); break;
        case 17: _t->onSoftkeyBack(); break;
        case 18: _t->onSoftkeyBtnHold((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 19: _t->onSoftkeyBtnHoldCritical((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: _t->onSoftkeyBtnReleased((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: _t->onTogglePlayBtn(); break;
        case 22: _t->onLongPressPlayBtn(); break;
        case 23: _t->listButtonHandler(); break;
        case 24: _t->onPrevBtn(); break;
        case 25: _t->onNextBtn(); break;
        case 26: _t->onFlick_next(); break;
        case 27: _t->onFlick_prev(); break;
        case 28: _t->onNextBtnHold(); break;
        case 29: _t->onNextBtnHoldCritical(); break;
        case 30: _t->onPrevBtnHold(); break;
        case 31: _t->onPrevBtnHoldCritical(); break;
        case 32: _t->onCancel_ff_rew(); break;
        case 33: _t->onSelectTuned(); break;
        case 34: _t->onPlaybackStatusChanged((*reinterpret_cast< AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T(*)>(_a[1]))); break;
        case 35: _t->onLockoutMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 36: _t->onSetTuneMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 37: _t->onTuneSameFile((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 38: _t->onFocusVisibilityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 39: _t->onSetCamMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 40: _t->onRetranslateUI(); break;
        case 41: _t->onScreenStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 42: _t->showFullScreen(); break;
        case 43: _t->onFgLoading((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_ScreenDVDPlayback::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_ScreenDVDPlayback::staticMetaObject = {
    { &AppMediaPlayer_Video_Screen::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_ScreenDVDPlayback,
      qt_meta_data_AppMediaPlayer_Video_ScreenDVDPlayback, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_ScreenDVDPlayback::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_ScreenDVDPlayback::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_ScreenDVDPlayback::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_ScreenDVDPlayback))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_ScreenDVDPlayback*>(this));
    return AppMediaPlayer_Video_Screen::qt_metacast(_clname);
}

int AppMediaPlayer_Video_ScreenDVDPlayback::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppMediaPlayer_Video_Screen::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 44)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 44;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_ScreenDVDPlayback::resetSpeedIcon()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void AppMediaPlayer_Video_ScreenDVDPlayback::setSpeedIcon(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Video_ScreenDVDPlayback::showPlaybackControls(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppMediaPlayer_Video_ScreenDVDPlayback::releaseLongPress()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void AppMediaPlayer_Video_ScreenDVDPlayback::cancelFfRew()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void AppMediaPlayer_Video_ScreenDVDPlayback::showLockout(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
