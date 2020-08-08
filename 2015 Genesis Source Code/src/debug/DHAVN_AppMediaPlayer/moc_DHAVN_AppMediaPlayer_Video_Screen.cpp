/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_Screen.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_Screen.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_Screen[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      15,       // signalCount

 // signals: signature, parameters, type, tag, flags
      44,   29,   28,   28, 0x05,
      86,   75,   28,   28, 0x25,
     108,   29,   28,   28, 0x05,
     137,   75,   28,   28, 0x25,
     161,  157,   28,   28, 0x05,
     181,   28,   28,   28, 0x25,
     193,   29,   28,   28, 0x05,
     225,   75,   28,   28, 0x25,
     272,  248,   28,   28, 0x05,
     318,   28,   28,   28, 0x05,
     367,  327,   28,   28, 0x05,
     453,  417,   28,   28, 0x25,
     494,   28,   28,   28, 0x05,
     509,   28,   28,   28, 0x05,
     528,   28,   28,   28, 0x05,

 // slots: signature, parameters, type, tag, flags
     546,   28,   28,   28, 0x0a,
     570,  553,   28,   28, 0x0a,
     624,   28,   28,   28, 0x0a,
     637,   28,   28,   28, 0x0a,
     646,  157,   28,   28, 0x0a,
     663,   28,   28,   28, 0x0a,
     678,   28,   28,   28, 0x0a,
     697,   28,   28,   28, 0x0a,
     716,   28,   28,   28, 0x0a,
     735,   28,   28,   28, 0x0a,
     754,   28,   28,   28, 0x0a,
     776,  771,   28,   28, 0x0a,
     812,  802,   28,   28, 0x0a,
     846,  327,   28,   28, 0x0a,
     895,  892,   28,   28, 0x0a,
     913,  892,   28,   28, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_Screen[] = {
    "AppMediaPlayer_Video_Screen\0\0"
    "screenName,arg\0changeScreen(QString,QVariant)\0"
    "screenName\0changeScreen(QString)\0"
    "pushScreen(QString,QVariant)\0"
    "pushScreen(QString)\0arg\0popScreen(QVariant)\0"
    "popScreen()\0replaceScreen(QString,QVariant)\0"
    "replaceScreen(QString)\0itemName,property,value\0"
    "setItemProperty(QString,const char*,QVariant)\0"
    "hideUI()\0srcScreenName,resScreenName,eventId,arg\0"
    "notifyScreenManager(QString,QString,int,QVariant)\0"
    "srcScreenName,resScreenName,eventId\0"
    "notifyScreenManager(QString,QString,int)\0"
    "hideAllFocus()\0loadingCompleted()\0"
    "setDefaultFocus()\0hide()\0item,context,arg\0"
    "show(QDeclarativeItem*,QDeclarativeContext*,QVariant)\0"
    "deactivate()\0onHide()\0onShow(QVariant)\0"
    "onDeactivate()\0onPrevKeyPressed()\0"
    "onNextKeyPressed()\0onMenuKeyPressed()\0"
    "onBackKeyPressed()\0onMousePressed()\0"
    "item\0onOptionsItemClicked(int)\0item,flag\0"
    "onOptionsCheckBoxSelect(int,bool)\0"
    "onEventReceived(QString,QString,int,QVariant)\0"
    "on\0lockoutMode(bool)\0onLockoutMode(bool)\0"
};

void AppMediaPlayer_Video_Screen::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_Screen *_t = static_cast<AppMediaPlayer_Video_Screen *>(_o);
        switch (_id) {
        case 0: _t->changeScreen((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 1: _t->changeScreen((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->pushScreen((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 3: _t->pushScreen((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->popScreen((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 5: _t->popScreen(); break;
        case 6: _t->replaceScreen((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 7: _t->replaceScreen((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->setItemProperty((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const char*(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 9: _t->hideUI(); break;
        case 10: _t->notifyScreenManager((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< QVariant(*)>(_a[4]))); break;
        case 11: _t->notifyScreenManager((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 12: _t->hideAllFocus(); break;
        case 13: _t->loadingCompleted(); break;
        case 14: _t->setDefaultFocus(); break;
        case 15: _t->hide(); break;
        case 16: _t->show((*reinterpret_cast< QDeclarativeItem*(*)>(_a[1])),(*reinterpret_cast< QDeclarativeContext*(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        case 17: _t->deactivate(); break;
        case 18: _t->onHide(); break;
        case 19: _t->onShow((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 20: _t->onDeactivate(); break;
        case 21: _t->onPrevKeyPressed(); break;
        case 22: _t->onNextKeyPressed(); break;
        case 23: _t->onMenuKeyPressed(); break;
        case 24: _t->onBackKeyPressed(); break;
        case 25: _t->onMousePressed(); break;
        case 26: _t->onOptionsItemClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->onOptionsCheckBoxSelect((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 28: _t->onEventReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 29: _t->lockoutMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 30: _t->onLockoutMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_Screen::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_Screen::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_Screen,
      qt_meta_data_AppMediaPlayer_Video_Screen, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_Screen::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_Screen::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_Screen::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_Screen))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_Screen*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_Video_Screen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_Screen::changeScreen(const QString & _t1, QVariant _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 2
void AppMediaPlayer_Video_Screen::pushScreen(const QString & _t1, QVariant _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 4
void AppMediaPlayer_Video_Screen::popScreen(QVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 6
void AppMediaPlayer_Video_Screen::replaceScreen(const QString & _t1, QVariant _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 8
void AppMediaPlayer_Video_Screen::setItemProperty(const QString & _t1, const char * _t2, const QVariant & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void AppMediaPlayer_Video_Screen::hideUI()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void AppMediaPlayer_Video_Screen::notifyScreenManager(const QString & _t1, const QString & _t2, const int _t3, QVariant _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 12
void AppMediaPlayer_Video_Screen::hideAllFocus()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void AppMediaPlayer_Video_Screen::loadingCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void AppMediaPlayer_Video_Screen::setDefaultFocus()
{
    QMetaObject::activate(this, &staticMetaObject, 14, 0);
}
QT_END_MOC_NAMESPACE
