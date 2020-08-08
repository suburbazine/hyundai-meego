/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_CModel_FS.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_CModel_FS.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_CModel_FS[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      27,       // signalCount

 // signals: signature, parameters, type, tag, flags
      38,   32,   31,   31, 0x05,
      63,   32,   31,   31, 0x05,
      95,   32,   31,   31, 0x05,
     122,   32,   31,   31, 0x05,
     149,   32,   31,   31, 0x05,
     180,   32,   31,   31, 0x05,
     198,   32,   31,   31, 0x05,
     217,   32,   31,   31, 0x05,
     243,   32,   31,   31, 0x05,
     264,   32,   31,   31, 0x05,
     291,   32,   31,   31, 0x05,
     320,   32,   31,   31, 0x05,
     344,   32,   31,   31, 0x05,
     371,   32,   31,   31, 0x05,
     407,   32,   31,   31, 0x05,
     432,   31,   31,   31, 0x05,
     462,   31,   31,   31, 0x05,
     487,   32,   31,   31, 0x05,
     514,   31,   31,   31, 0x05,
     538,   31,   31,   31, 0x05,
     562,   31,   31,   31, 0x05,
     593,   31,   31,   31, 0x05,
     619,   31,   31,   31, 0x05,
     646,   31,   31,   31, 0x05,
     671,   31,   31,   31, 0x05,
     706,   31,   31,   31, 0x05,
     741,   31,   31,   31, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_CModel_FS[] = {
    "AppMediaPlayer_Video_CModel_FS\0\0value\0"
    "filenameChanged(QString)\0"
    "playingFilenameChanged(QString)\0"
    "foldernameChanged(QString)\0"
    "subtileStrChanged(QString)\0"
    "fullfoldernameChanged(QString)\0"
    "widthChanged(int)\0heightChanged(int)\0"
    "tuneSameFileChanged(bool)\0"
    "durationChanged(int)\0playlistLengthChanged(int)\0"
    "playlistPositionChanged(int)\0"
    "captionSizeChanged(int)\0"
    "captionEnableChanged(bool)\0"
    "captionLangListChanged(QStringList)\0"
    "captionCurIdChanged(int)\0"
    "multipleCaptionsChanged(bool)\0"
    "smiCaptionsChanged(bool)\0"
    "copyPercentageChanged(int)\0"
    "drmUseCountChanged(int)\0drmUseLimitChanged(int)\0"
    "lastPlayedFileChanged(QString)\0"
    "lastPlayedPosChanged(int)\0"
    "playingUrlChanged(QString)\0"
    "readingFileChanged(bool)\0"
    "xsubCaptionVisibilityChanged(bool)\0"
    "currentSubtitleSettingChanged(int)\0"
    "voiceLangEnabledChanged(bool)\0"
};

void AppMediaPlayer_Video_CModel_FS::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_CModel_FS *_t = static_cast<AppMediaPlayer_Video_CModel_FS *>(_o);
        switch (_id) {
        case 0: _t->filenameChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->playingFilenameChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->foldernameChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->subtileStrChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->fullfoldernameChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->widthChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->heightChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->tuneSameFileChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->durationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->playlistLengthChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->playlistPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->captionSizeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->captionEnableChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->captionLangListChanged((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 14: _t->captionCurIdChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->multipleCaptionsChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->smiCaptionsChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: _t->copyPercentageChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->drmUseCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->drmUseLimitChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->lastPlayedFileChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: _t->lastPlayedPosChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->playingUrlChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->readingFileChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 24: _t->xsubCaptionVisibilityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: _t->currentSubtitleSettingChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->voiceLangEnabledChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_CModel_FS::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_CModel_FS::staticMetaObject = {
    { &AppMediaPlayer_Video_CModel_Base::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_CModel_FS,
      qt_meta_data_AppMediaPlayer_Video_CModel_FS, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_CModel_FS::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_CModel_FS::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_CModel_FS::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_CModel_FS))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_CModel_FS*>(this));
    return AppMediaPlayer_Video_CModel_Base::qt_metacast(_clname);
}

int AppMediaPlayer_Video_CModel_FS::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppMediaPlayer_Video_CModel_Base::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_CModel_FS::filenameChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_CModel_FS::playingFilenameChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Video_CModel_FS::foldernameChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppMediaPlayer_Video_CModel_FS::subtileStrChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AppMediaPlayer_Video_CModel_FS::fullfoldernameChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AppMediaPlayer_Video_CModel_FS::widthChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void AppMediaPlayer_Video_CModel_FS::heightChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void AppMediaPlayer_Video_CModel_FS::tuneSameFileChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void AppMediaPlayer_Video_CModel_FS::durationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void AppMediaPlayer_Video_CModel_FS::playlistLengthChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void AppMediaPlayer_Video_CModel_FS::playlistPositionChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void AppMediaPlayer_Video_CModel_FS::captionSizeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void AppMediaPlayer_Video_CModel_FS::captionEnableChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void AppMediaPlayer_Video_CModel_FS::captionLangListChanged(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void AppMediaPlayer_Video_CModel_FS::captionCurIdChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void AppMediaPlayer_Video_CModel_FS::multipleCaptionsChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void AppMediaPlayer_Video_CModel_FS::smiCaptionsChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void AppMediaPlayer_Video_CModel_FS::copyPercentageChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void AppMediaPlayer_Video_CModel_FS::drmUseCountChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void AppMediaPlayer_Video_CModel_FS::drmUseLimitChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void AppMediaPlayer_Video_CModel_FS::lastPlayedFileChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void AppMediaPlayer_Video_CModel_FS::lastPlayedPosChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void AppMediaPlayer_Video_CModel_FS::playingUrlChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void AppMediaPlayer_Video_CModel_FS::readingFileChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void AppMediaPlayer_Video_CModel_FS::xsubCaptionVisibilityChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void AppMediaPlayer_Video_CModel_FS::currentSubtitleSettingChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void AppMediaPlayer_Video_CModel_FS::voiceLangEnabledChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}
QT_END_MOC_NAMESPACE
