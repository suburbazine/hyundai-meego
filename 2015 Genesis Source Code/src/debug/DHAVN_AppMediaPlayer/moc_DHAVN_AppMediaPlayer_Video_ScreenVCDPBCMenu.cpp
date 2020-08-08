/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_ScreenVCDPBCMenu.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenVCDPBCMenu.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_ScreenVCDPBCMenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_ScreenVCDPBCMenu[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      50,   39,   38,   38, 0x05,
      82,   75,   38,   38, 0x05,
     111,  100,   38,   38, 0x05,

 // slots: signature, parameters, type, tag, flags
     130,   38,   38,   38, 0x0a,
     143,  139,   38,   38, 0x0a,
     160,   38,   38,   38, 0x0a,
     181,  175,   38,   38, 0x0a,
     199,   38,   38,   38, 0x0a,
     212,   38,   38,   38, 0x0a,
     226,   38,   38,   38, 0x0a,
     242,   38,   38,   38, 0x0a,
     253,   38,   38,   38, 0x0a,
     281,  275,   38,   38, 0x0a,
     318,  315,   38,   38, 0x0a,
     338,   38,   38,   38, 0x0a,
     357,   38,   38,   38, 0x0a,
     373,   38,   38,   38, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_ScreenVCDPBCMenu[] = {
    "AppMediaPlayer_Video_ScreenVCDPBCMenu\0"
    "\0tab,isShow\0updateModeArea(int,bool)\0"
    "onShow\0showLockout(bool)\0onTempMode\0"
    "setPBCMenuUI(bool)\0onHide()\0arg\0"
    "onShow(QVariant)\0onDeactivate()\0index\0"
    "onKeyEntered(int)\0onEnterKey()\0"
    "onReturnKey()\0onSoftkeyBack()\0onPbcOff()\0"
    "setDefaultInputText()\0value\0"
    "onIsPbcMenuDisplayedChanged(bool)\0on\0"
    "onLockoutMode(bool)\0onBackKeyPressed()\0"
    "setupModeArea()\0onRetranslateUI()\0"
};

void AppMediaPlayer_Video_ScreenVCDPBCMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_ScreenVCDPBCMenu *_t = static_cast<AppMediaPlayer_Video_ScreenVCDPBCMenu *>(_o);
        switch (_id) {
        case 0: _t->updateModeArea((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->showLockout((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->setPBCMenuUI((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onHide(); break;
        case 4: _t->onShow((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 5: _t->onDeactivate(); break;
        case 6: _t->onKeyEntered((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->onEnterKey(); break;
        case 8: _t->onReturnKey(); break;
        case 9: _t->onSoftkeyBack(); break;
        case 10: _t->onPbcOff(); break;
        case 11: _t->setDefaultInputText(); break;
        case 12: _t->onIsPbcMenuDisplayedChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->onLockoutMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->onBackKeyPressed(); break;
        case 15: _t->setupModeArea(); break;
        case 16: _t->onRetranslateUI(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_ScreenVCDPBCMenu::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_ScreenVCDPBCMenu::staticMetaObject = {
    { &AppMediaPlayer_Video_Screen::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_ScreenVCDPBCMenu,
      qt_meta_data_AppMediaPlayer_Video_ScreenVCDPBCMenu, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_ScreenVCDPBCMenu::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_ScreenVCDPBCMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_ScreenVCDPBCMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_ScreenVCDPBCMenu))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_ScreenVCDPBCMenu*>(this));
    return AppMediaPlayer_Video_Screen::qt_metacast(_clname);
}

int AppMediaPlayer_Video_ScreenVCDPBCMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void AppMediaPlayer_Video_ScreenVCDPBCMenu::updateModeArea(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_ScreenVCDPBCMenu::showLockout(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Video_ScreenVCDPBCMenu::setPBCMenuUI(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
