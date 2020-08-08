/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_ScreenTextButtonPopup.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenTextButtonPopup.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_ScreenTextButtonPopup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_ScreenTextButtonPopup[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      54,   44,   43,   43, 0x05,
      89,   43,   43,   43, 0x05,
     112,  105,   43,   43, 0x05,

 // slots: signature, parameters, type, tag, flags
     130,   43,   43,   43, 0x0a,
     143,  139,   43,   43, 0x0a,
     160,   43,   43,   43, 0x0a,
     181,  174,   43,   43, 0x0a,
     203,   43,   43,   43, 0x0a,
     225,  222,   43,   43, 0x0a,
     245,   43,   43,   43, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_ScreenTextButtonPopup[] = {
    "AppMediaPlayer_Video_ScreenTextButtonPopup\0"
    "\0btn1,btn2\0updateButtonModel(QString,QString)\0"
    "popUpTimerOut()\0onShow\0showLockout(bool)\0"
    "onHide()\0arg\0onShow(QVariant)\0"
    "closeScreen()\0button\0onSoftkeyBtn(QString)\0"
    "onBackKeyPressed()\0on\0onLockoutMode(bool)\0"
    "onRetranslateUI()\0"
};

void AppMediaPlayer_Video_ScreenTextButtonPopup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_ScreenTextButtonPopup *_t = static_cast<AppMediaPlayer_Video_ScreenTextButtonPopup *>(_o);
        switch (_id) {
        case 0: _t->updateButtonModel((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->popUpTimerOut(); break;
        case 2: _t->showLockout((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onHide(); break;
        case 4: _t->onShow((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 5: _t->closeScreen(); break;
        case 6: _t->onSoftkeyBtn((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->onBackKeyPressed(); break;
        case 8: _t->onLockoutMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->onRetranslateUI(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_ScreenTextButtonPopup::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_ScreenTextButtonPopup::staticMetaObject = {
    { &AppMediaPlayer_Video_Screen::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_ScreenTextButtonPopup,
      qt_meta_data_AppMediaPlayer_Video_ScreenTextButtonPopup, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_ScreenTextButtonPopup::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_ScreenTextButtonPopup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_ScreenTextButtonPopup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_ScreenTextButtonPopup))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_ScreenTextButtonPopup*>(this));
    return AppMediaPlayer_Video_Screen::qt_metacast(_clname);
}

int AppMediaPlayer_Video_ScreenTextButtonPopup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppMediaPlayer_Video_Screen::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_ScreenTextButtonPopup::updateButtonModel(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_ScreenTextButtonPopup::popUpTimerOut()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void AppMediaPlayer_Video_ScreenTextButtonPopup::showLockout(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
