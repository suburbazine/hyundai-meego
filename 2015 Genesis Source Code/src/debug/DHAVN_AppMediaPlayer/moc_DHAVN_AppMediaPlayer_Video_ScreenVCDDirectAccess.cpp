/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_ScreenVCDDirectAccess.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenVCDDirectAccess.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_ScreenVCDDirectAccess.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_ScreenVCDDirectAccess[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      51,   44,   43,   43, 0x05,
      80,   69,   43,   43, 0x05,

 // slots: signature, parameters, type, tag, flags
     104,   43,   43,   43, 0x0a,
     119,   43,   43,   43, 0x0a,
     132,  128,   43,   43, 0x0a,
     189,  149,   43,   43, 0x0a,
     238,  235,   43,   43, 0x0a,
     258,   43,   43,   43, 0x0a,
     277,   43,   43,   43, 0x0a,
     297,  293,   43,   43, 0x0a,
     327,   43,   43,   43, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_ScreenVCDDirectAccess[] = {
    "AppMediaPlayer_Video_ScreenVCDDirectAccess\0"
    "\0onShow\0showLockout(bool)\0onTempMode\0"
    "setDirectAccessUI(bool)\0onDeactivate()\0"
    "onHide()\0arg\0onShow(QVariant)\0"
    "srcScreenName,resScreenName,eventId,arg\0"
    "onEventReceived(QString,QString,int,QVariant)\0"
    "on\0onLockoutMode(bool)\0onBackKeyPressed()\0"
    "onSoftkeyBack()\0str\0onVCDMenuItemPressed(QString)\0"
    "setDefaultInputText()\0"
};

void AppMediaPlayer_Video_ScreenVCDDirectAccess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_ScreenVCDDirectAccess *_t = static_cast<AppMediaPlayer_Video_ScreenVCDDirectAccess *>(_o);
        switch (_id) {
        case 0: _t->showLockout((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setDirectAccessUI((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->onDeactivate(); break;
        case 3: _t->onHide(); break;
        case 4: _t->onShow((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 5: _t->onEventReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 6: _t->onLockoutMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->onBackKeyPressed(); break;
        case 8: _t->onSoftkeyBack(); break;
        case 9: _t->onVCDMenuItemPressed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->setDefaultInputText(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_ScreenVCDDirectAccess::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_ScreenVCDDirectAccess::staticMetaObject = {
    { &AppMediaPlayer_Video_Screen::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_ScreenVCDDirectAccess,
      qt_meta_data_AppMediaPlayer_Video_ScreenVCDDirectAccess, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_ScreenVCDDirectAccess::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_ScreenVCDDirectAccess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_ScreenVCDDirectAccess::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_ScreenVCDDirectAccess))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_ScreenVCDDirectAccess*>(this));
    return AppMediaPlayer_Video_Screen::qt_metacast(_clname);
}

int AppMediaPlayer_Video_ScreenVCDDirectAccess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppMediaPlayer_Video_Screen::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_ScreenVCDDirectAccess::showLockout(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_ScreenVCDDirectAccess::setDirectAccessUI(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
