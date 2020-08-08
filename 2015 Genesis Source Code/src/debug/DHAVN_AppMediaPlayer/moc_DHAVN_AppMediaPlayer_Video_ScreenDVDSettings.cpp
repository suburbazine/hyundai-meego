/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_ScreenDVDSettings.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenDVDSettings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_ScreenDVDSettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_ScreenDVDSettings[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      46,   40,   39,   39, 0x05,
      71,   39,   39,   39, 0x05,

 // slots: signature, parameters, type, tag, flags
      89,   39,   39,   39, 0x0a,
     102,   98,   39,   39, 0x0a,
     119,   40,   39,   39, 0x0a,
     142,   40,   39,   39, 0x0a,
     166,  161,   39,   39, 0x08,
     188,   39,   39,   39, 0x08,

 // methods: signature, parameters, type, tag, flags
     206,   39,   39,   39, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_ScreenDVDSettings[] = {
    "AppMediaPlayer_Video_ScreenDVDSettings\0"
    "\0index\0updateSelectedIndex(int)\0"
    "setDVDSettingUI()\0onHide()\0arg\0"
    "onShow(QVariant)\0radioItemSelected(int)\0"
    "onTabSelected(int)\0item\0handleVCItem(VCItem*)\0"
    "onRetranslateUI()\0onSoftkeyBack()\0"
};

void AppMediaPlayer_Video_ScreenDVDSettings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_ScreenDVDSettings *_t = static_cast<AppMediaPlayer_Video_ScreenDVDSettings *>(_o);
        switch (_id) {
        case 0: _t->updateSelectedIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setDVDSettingUI(); break;
        case 2: _t->onHide(); break;
        case 3: _t->onShow((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 4: _t->radioItemSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onTabSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->handleVCItem((*reinterpret_cast< VCItem*(*)>(_a[1]))); break;
        case 7: _t->onRetranslateUI(); break;
        case 8: _t->onSoftkeyBack(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_ScreenDVDSettings::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_ScreenDVDSettings::staticMetaObject = {
    { &AppMediaPlayer_Video_Screen::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_ScreenDVDSettings,
      qt_meta_data_AppMediaPlayer_Video_ScreenDVDSettings, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_ScreenDVDSettings::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_ScreenDVDSettings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_ScreenDVDSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_ScreenDVDSettings))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_ScreenDVDSettings*>(this));
    return AppMediaPlayer_Video_Screen::qt_metacast(_clname);
}

int AppMediaPlayer_Video_ScreenDVDSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppMediaPlayer_Video_Screen::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_ScreenDVDSettings::updateSelectedIndex(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_ScreenDVDSettings::setDVDSettingUI()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
