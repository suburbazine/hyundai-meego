/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_SModel_FS_FileInfoPopup.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_FS_FileInfoPopup.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_SModel_FS_FileInfoPopup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_SModel_FS_FileInfoPopup[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       3,   54, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      46,   45,   45,   45, 0x05,
      71,   45,   45,   45, 0x05,
      96,   45,   45,   45, 0x05,

 // slots: signature, parameters, type, tag, flags
     123,   45,   45,   45, 0x0a,
     144,   45,   45,   45, 0x0a,
     165,   45,   45,   45, 0x0a,
     188,   45,   45,   45, 0x08,
     238,   45,   45,   45, 0x08,

 // properties: name, type, flags
     297,  289, 0x0a495001,
     306,  289, 0x0a495001,
     315,  289, 0x0a495001,

 // properties: notify_signal_id
       0,
       1,
       2,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_SModel_FS_FileInfoPopup[] = {
    "AppMediaPlayer_Video_SModel_FS_FileInfoPopup\0"
    "\0filenameChanged(QString)\0"
    "durationChanged(QString)\0"
    "resolutionChanged(QString)\0"
    "setFilename(QString)\0setDuration(QString)\0"
    "setResolution(QString)\0"
    "onAppMediaPlayer_Video_CModel_FSwidthChanged(int)\0"
    "onAppMediaPlayer_Video_CModel_FSheightChanged(int)\0"
    "QString\0filename\0duration\0resolution\0"
};

void AppMediaPlayer_Video_SModel_FS_FileInfoPopup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_SModel_FS_FileInfoPopup *_t = static_cast<AppMediaPlayer_Video_SModel_FS_FileInfoPopup *>(_o);
        switch (_id) {
        case 0: _t->filenameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->durationChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->resolutionChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->setFilename((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->setDuration((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->setResolution((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->onAppMediaPlayer_Video_CModel_FSwidthChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->onAppMediaPlayer_Video_CModel_FSheightChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_SModel_FS_FileInfoPopup::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_SModel_FS_FileInfoPopup::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_SModel_FS_FileInfoPopup,
      qt_meta_data_AppMediaPlayer_Video_SModel_FS_FileInfoPopup, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_SModel_FS_FileInfoPopup::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_SModel_FS_FileInfoPopup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_SModel_FS_FileInfoPopup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_SModel_FS_FileInfoPopup))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_SModel_FS_FileInfoPopup*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_Video_SModel_FS_FileInfoPopup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = filename(); break;
        case 1: *reinterpret_cast< QString*>(_v) = duration(); break;
        case 2: *reinterpret_cast< QString*>(_v) = resolution(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_SModel_FS_FileInfoPopup::filenameChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_SModel_FS_FileInfoPopup::durationChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Video_SModel_FS_FileInfoPopup::resolutionChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
