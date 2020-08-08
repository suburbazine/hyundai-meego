/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_ScreenCaption.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenCaption.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_ScreenCaption.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_ScreenCaption[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      36,   35,   35,   35, 0x05,

 // slots: signature, parameters, type, tag, flags
      49,   35,   35,   35, 0x0a,
      62,   58,   35,   35, 0x0a,
     119,   79,   35,   35, 0x0a,
     165,   35,   35,   35, 0x0a,
     188,   35,   35,   35, 0x0a,

 // methods: signature, parameters, type, tag, flags
     230,  215,   35,   35, 0x02,
     261,  256,   35,   35, 0x02,
     278,   35,   35,   35, 0x02,
     294,  256,   35,   35, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_ScreenCaption[] = {
    "AppMediaPlayer_Video_ScreenCaption\0\0"
    "initScreen()\0onHide()\0arg\0onShow(QVariant)\0"
    "srcScreenName,resScreenName,eventId,arg\0"
    "onEventReceived(QString,QString,int,QVariant)\0"
    "onDurationChanged(int)\0"
    "onCaptionCurIdChanged(int)\0enabledCaption\0"
    "onSetEnabledCaption(bool)\0item\0"
    "onRadioItem(int)\0onSoftkeyBack()\0"
    "onMenuItem(int)\0"
};

void AppMediaPlayer_Video_ScreenCaption::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_ScreenCaption *_t = static_cast<AppMediaPlayer_Video_ScreenCaption *>(_o);
        switch (_id) {
        case 0: _t->initScreen(); break;
        case 1: _t->onHide(); break;
        case 2: _t->onShow((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 3: _t->onEventReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 4: _t->onDurationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onCaptionCurIdChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onSetEnabledCaption((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->onRadioItem((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->onSoftkeyBack(); break;
        case 9: _t->onMenuItem((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_ScreenCaption::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_ScreenCaption::staticMetaObject = {
    { &AppMediaPlayer_Video_Screen::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_ScreenCaption,
      qt_meta_data_AppMediaPlayer_Video_ScreenCaption, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_ScreenCaption::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_ScreenCaption::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_ScreenCaption::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_ScreenCaption))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_ScreenCaption*>(this));
    return AppMediaPlayer_Video_Screen::qt_metacast(_clname);
}

int AppMediaPlayer_Video_ScreenCaption::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void AppMediaPlayer_Video_ScreenCaption::initScreen()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
