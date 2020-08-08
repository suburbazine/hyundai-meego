/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_ScreenOptions.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenOptions.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_ScreenOptions.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_ScreenOptions[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      36,   35,   35,   35, 0x05,
      59,   50,   35,   35, 0x05,
      78,   35,   35,   35, 0x05,
      92,   35,   35,   35, 0x05,

 // slots: signature, parameters, type, tag, flags
     110,   35,   35,   35, 0x0a,
     123,  119,   35,   35, 0x0a,
     140,   35,   35,   35, 0x0a,
     195,  155,   35,   35, 0x0a,
     241,   35,   35,   35, 0x0a,
     260,   35,   35,   35, 0x0a,
     283,  278,   35,   35, 0x0a,
     312,  302,   35,   35, 0x0a,
     339,   35,   35,   35, 0x0a,
     366,  350,   35,   35, 0x0a,
     411,   35,   35,   35, 0x0a,
     442,   35,   35,   35, 0x0a,
     472,  461,   35,   35, 0x0a,
     519,  514,   35,   35, 0x0a,
     551,  514,   35,   35, 0x0a,
     589,  577,   35,   35, 0x0a,
     622,  619,   35,   35, 0x0a,
     647,  619,   35,   35, 0x0a,
     672,  667,   35,   35, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_ScreenOptions[] = {
    "AppMediaPlayer_Video_ScreenOptions\0\0"
    "backPressed()\0animated\0closeOptions(bool)\0"
    "showOptions()\0showLockout(bool)\0"
    "onHide()\0arg\0onShow(QVariant)\0"
    "onDeactivate()\0srcScreenName,resScreenName,eventId,arg\0"
    "onEventReceived(QString,QString,int,QVariant)\0"
    "onMenuKeyPressed()\0onMouseReleased()\0"
    "item\0onItemPressed(int)\0item,flag\0"
    "onCheckBoxSelect(int,bool)\0onHidden()\0"
    "pObj,method,arg\0"
    "invokeSelectItemMethod(QObject*,QString,int)\0"
    "closeOptionsIfAudioLangModel()\0"
    "onBackKeyPressed()\0text,text2\0"
    "onActiveAudioLangChanged(QString,QString)\0"
    "text\0onActiveCaptionChanged(QString)\0"
    "onActiveAngleChanged(int)\0mediaSource\0"
    "onSubtitleSettingChanged(int)\0on\0"
    "onVoiceLangEnabled(bool)\0onLockoutMode(bool)\0"
    "isFG\0onFgBg(bool)\0"
};

void AppMediaPlayer_Video_ScreenOptions::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_ScreenOptions *_t = static_cast<AppMediaPlayer_Video_ScreenOptions *>(_o);
        switch (_id) {
        case 0: _t->backPressed(); break;
        case 1: _t->closeOptions((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->showOptions(); break;
        case 3: _t->showLockout((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onHide(); break;
        case 5: _t->onShow((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 6: _t->onDeactivate(); break;
        case 7: _t->onEventReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 8: _t->onMenuKeyPressed(); break;
        case 9: _t->onMouseReleased(); break;
        case 10: _t->onItemPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->onCheckBoxSelect((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 12: _t->onHidden(); break;
        case 13: _t->invokeSelectItemMethod((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 14: _t->closeOptionsIfAudioLangModel(); break;
        case 15: _t->onBackKeyPressed(); break;
        case 16: _t->onActiveAudioLangChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 17: _t->onActiveCaptionChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: _t->onActiveAngleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->onSubtitleSettingChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->onVoiceLangEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 21: _t->onLockoutMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: _t->onFgBg((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_ScreenOptions::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_ScreenOptions::staticMetaObject = {
    { &AppMediaPlayer_Video_Screen::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_ScreenOptions,
      qt_meta_data_AppMediaPlayer_Video_ScreenOptions, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_ScreenOptions::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_ScreenOptions::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_ScreenOptions::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_ScreenOptions))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_ScreenOptions*>(this));
    return AppMediaPlayer_Video_Screen::qt_metacast(_clname);
}

int AppMediaPlayer_Video_ScreenOptions::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppMediaPlayer_Video_Screen::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_ScreenOptions::backPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void AppMediaPlayer_Video_ScreenOptions::closeOptions(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Video_ScreenOptions::showOptions()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void AppMediaPlayer_Video_ScreenOptions::showLockout(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
