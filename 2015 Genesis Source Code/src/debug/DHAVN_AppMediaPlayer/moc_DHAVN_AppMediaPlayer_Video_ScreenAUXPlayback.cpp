/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_ScreenAUXPlayback.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenAUXPlayback.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_ScreenAUXPlayback.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_ScreenAUXPlayback[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      47,   40,   39,   39, 0x05,
      75,   65,   39,   39, 0x05,

 // slots: signature, parameters, type, tag, flags
      90,   39,   39,   39, 0x0a,
     103,   99,   39,   39, 0x0a,
     120,   39,   39,   39, 0x0a,
     135,   39,   39,   39, 0x0a,
     159,  154,   39,   39, 0x0a,
     225,  185,   39,   39, 0x0a,
     271,   39,   39,   39, 0x0a,
     288,   39,   39,   39, 0x0a,
     313,  306,   39,   39, 0x0a,
     337,  334,   39,   39, 0x0a,
     357,   39,   39,   39, 0x0a,
     373,   39,   39,   39, 0x08,
     390,  334,   39,   39, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_ScreenAUXPlayback[] = {
    "AppMediaPlayer_Video_ScreenAUXPlayback\0"
    "\0onShow\0showLockout(bool)\0direction\0"
    "lostFocus(int)\0onHide()\0arg\0"
    "onShow(QVariant)\0onDeactivate()\0"
    "onMenuKeyPressed()\0item\0"
    "onOptionsItemClicked(int)\0"
    "srcScreenName,resScreenName,eventId,arg\0"
    "onEventReceived(QString,QString,int,QVariant)\0"
    "onMousePressed()\0onMouseReleased()\0"
    "pbArea\0onMouseClicked(bool)\0on\0"
    "onLockoutMode(bool)\0onPlayAuxOnFG()\0"
    "showFullScreen()\0onFgLoading(bool)\0"
};

void AppMediaPlayer_Video_ScreenAUXPlayback::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_ScreenAUXPlayback *_t = static_cast<AppMediaPlayer_Video_ScreenAUXPlayback *>(_o);
        switch (_id) {
        case 0: _t->showLockout((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->lostFocus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onHide(); break;
        case 3: _t->onShow((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 4: _t->onDeactivate(); break;
        case 5: _t->onMenuKeyPressed(); break;
        case 6: _t->onOptionsItemClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->onEventReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 8: _t->onMousePressed(); break;
        case 9: _t->onMouseReleased(); break;
        case 10: _t->onMouseClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->onLockoutMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->onPlayAuxOnFG(); break;
        case 13: _t->showFullScreen(); break;
        case 14: _t->onFgLoading((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_ScreenAUXPlayback::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_ScreenAUXPlayback::staticMetaObject = {
    { &AppMediaPlayer_Video_Screen::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_ScreenAUXPlayback,
      qt_meta_data_AppMediaPlayer_Video_ScreenAUXPlayback, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_ScreenAUXPlayback::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_ScreenAUXPlayback::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_ScreenAUXPlayback::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_ScreenAUXPlayback))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_ScreenAUXPlayback*>(this));
    return AppMediaPlayer_Video_Screen::qt_metacast(_clname);
}

int AppMediaPlayer_Video_ScreenAUXPlayback::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppMediaPlayer_Video_Screen::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_ScreenAUXPlayback::showLockout(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_ScreenAUXPlayback::lostFocus(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
