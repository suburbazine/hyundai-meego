/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_ScreenFull.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenFull.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_ScreenFull.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_ScreenFull[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      40,   33,   32,   32, 0x05,

 // slots: signature, parameters, type, tag, flags
      58,   32,   32,   32, 0x0a,
      71,   67,   32,   32, 0x0a,
      88,   32,   32,   32, 0x0a,
     107,   32,   32,   32, 0x0a,
     129,  126,   32,   32, 0x0a,
     149,   32,   32,   32, 0x0a,
     204,  164,   32,   32, 0x0a,
     250,  126,   32,   32, 0x0a,
     270,   32,   32,   32, 0x0a,
     288,   32,   32,   32, 0x0a,
     305,   32,   32,   32, 0x0a,
     317,   32,   32,   32, 0x0a,
     329,   32,   32,   32, 0x0a,
     344,   32,   32,   32, 0x0a,
     359,   32,   32,   32, 0x0a,
     375,   32,   32,   32, 0x0a,
     399,   32,   32,   32, 0x0a,
     415,   32,   32,   32, 0x0a,
     439,   32,   32,   32, 0x0a,
     457,   32,   32,   32, 0x0a,
     478,   32,   32,   32, 0x0a,
     496,   32,   32,   32, 0x0a,
     518,  512,   32,   32, 0x0a,
     552,   32,   32,   32, 0x0a,

 // methods: signature, parameters, type, tag, flags
     577,   32,  572,   32, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_ScreenFull[] = {
    "AppMediaPlayer_Video_ScreenFull\0\0"
    "onShow\0showLockout(bool)\0onHide()\0arg\0"
    "onShow(QVariant)\0onBackKeyPressed()\0"
    "onMenuKeyPressed()\0on\0onSetTuneMode(bool)\0"
    "onDeactivate()\0srcScreenName,resScreenName,eventId,arg\0"
    "onEventReceived(QString,QString,int,QVariant)\0"
    "onLockoutMode(bool)\0onMouseReleased()\0"
    "onMouseClicked()\0onNextBtn()\0onPrevBtn()\0"
    "onFlick_next()\0onFlick_prev()\0"
    "onPrevBtnHold()\0onPrevBtnHoldCritical()\0"
    "onNextBtnHold()\0onNextBtnHoldCritical()\0"
    "onCancel_ff_rew()\0onLongPressPlayBtn()\0"
    "onTogglePlayBtn()\0onSelectTuned()\0"
    "value\0onIsPbcMenuDisplayedChanged(bool)\0"
    "onFMCopyCompleted()\0bool\0isFullScreenMode()\0"
};

void AppMediaPlayer_Video_ScreenFull::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_ScreenFull *_t = static_cast<AppMediaPlayer_Video_ScreenFull *>(_o);
        switch (_id) {
        case 0: _t->showLockout((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->onHide(); break;
        case 2: _t->onShow((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 3: _t->onBackKeyPressed(); break;
        case 4: _t->onMenuKeyPressed(); break;
        case 5: _t->onSetTuneMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->onDeactivate(); break;
        case 7: _t->onEventReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 8: _t->onLockoutMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->onMouseReleased(); break;
        case 10: _t->onMouseClicked(); break;
        case 11: _t->onNextBtn(); break;
        case 12: _t->onPrevBtn(); break;
        case 13: _t->onFlick_next(); break;
        case 14: _t->onFlick_prev(); break;
        case 15: _t->onPrevBtnHold(); break;
        case 16: _t->onPrevBtnHoldCritical(); break;
        case 17: _t->onNextBtnHold(); break;
        case 18: _t->onNextBtnHoldCritical(); break;
        case 19: _t->onCancel_ff_rew(); break;
        case 20: _t->onLongPressPlayBtn(); break;
        case 21: _t->onTogglePlayBtn(); break;
        case 22: _t->onSelectTuned(); break;
        case 23: _t->onIsPbcMenuDisplayedChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 24: _t->onFMCopyCompleted(); break;
        case 25: { bool _r = _t->isFullScreenMode();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_ScreenFull::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_ScreenFull::staticMetaObject = {
    { &AppMediaPlayer_Video_Screen::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_ScreenFull,
      qt_meta_data_AppMediaPlayer_Video_ScreenFull, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_ScreenFull::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_ScreenFull::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_ScreenFull::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_ScreenFull))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_ScreenFull*>(this));
    return AppMediaPlayer_Video_Screen::qt_metacast(_clname);
}

int AppMediaPlayer_Video_ScreenFull::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppMediaPlayer_Video_Screen::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_ScreenFull::showLockout(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
