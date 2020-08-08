/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Settings.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/util/DHAVN_AppMediaPlayer_Settings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Settings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_SettingsStorage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      32,   31,   31,   31, 0x08,
      60,   31,   31,   31, 0x08,
      88,   31,   31,   31, 0x08,
     116,   31,   31,   31, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_SettingsStorage[] = {
    "AppMediaPlayer_SettingsStorage\0\0"
    "OnVideoBackupTimerExpired()\0"
    "OnAudioBackupTimerExpired()\0"
    "OnPhotoBackupTimerExpired()\0"
    "OnCommonBackupTimerExpired()\0"
};

void AppMediaPlayer_SettingsStorage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_SettingsStorage *_t = static_cast<AppMediaPlayer_SettingsStorage *>(_o);
        switch (_id) {
        case 0: _t->OnVideoBackupTimerExpired(); break;
        case 1: _t->OnAudioBackupTimerExpired(); break;
        case 2: _t->OnPhotoBackupTimerExpired(); break;
        case 3: _t->OnCommonBackupTimerExpired(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData AppMediaPlayer_SettingsStorage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_SettingsStorage::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_SettingsStorage,
      qt_meta_data_AppMediaPlayer_SettingsStorage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_SettingsStorage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_SettingsStorage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_SettingsStorage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_SettingsStorage))
        return static_cast<void*>(const_cast< AppMediaPlayer_SettingsStorage*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_SettingsStorage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
