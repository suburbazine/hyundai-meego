/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_CModel_DVD.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_DVD.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_CModel_DVD.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_CModel_DVD[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      20,       // signalCount

 // signals: signature, parameters, type, tag, flags
      39,   33,   32,   32, 0x05,
      59,   33,   32,   32, 0x05,
      77,   33,   32,   32, 0x05,
     102,   33,   32,   32, 0x05,
     125,   33,   32,   32, 0x05,
     150,   33,   32,   32, 0x05,
     173,   33,   32,   32, 0x05,
     196,   32,   32,   32, 0x05,
     239,  233,   32,   32, 0x05,
     288,  267,   32,   32, 0x05,
     334,   32,   32,   32, 0x05,
     367,  233,   32,   32, 0x05,
     405,  393,   32,   32, 0x05,
     441,   32,   32,   32, 0x05,
     470,  233,   32,   32, 0x05,
     537,  494,   32,   32, 0x05,
     572,   33,   32,   32, 0x05,
     608,   33,   32,   32, 0x05,
     632,   33,   32,   32, 0x05,
     662,   33,   32,   32, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_CModel_DVD[] = {
    "AppMediaPlayer_Video_CModel_DVD\0\0value\0"
    "chapterChanged(int)\0titleChanged(int)\0"
    "focusChapterChanged(int)\0"
    "focusTitleChanged(int)\0chapterCountChanged(int)\0"
    "titleCountChanged(int)\0trackCountChanged(int)\0"
    "audioLangDataChanged(AudioLangData*)\0"
    "index\0activeAudioLangChanged(int)\0"
    "audioLang,audioLang2\0"
    "activeAudioLangStringChanged(QString,QString)\0"
    "captionDataChanged(CaptionData*)\0"
    "activeCaptionChanged(int)\0captionLang\0"
    "activeCaptionStringChanged(QString)\0"
    "angleDataChanged(AngleData*)\0"
    "activeAngleChanged(int)\0"
    "captionLangCode,audioLangCode,menuLangCode\0"
    "activeLangCodeChanged(int,int,int)\0"
    "isStartedBySettingsAppChanged(bool)\0"
    "playerStateChanged(int)\0"
    "playerScreenStateChanged(int)\0"
    "listBtnVisibleChanged(bool)\0"
};

void AppMediaPlayer_Video_CModel_DVD::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_CModel_DVD *_t = static_cast<AppMediaPlayer_Video_CModel_DVD *>(_o);
        switch (_id) {
        case 0: _t->chapterChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->titleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->focusChapterChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->focusTitleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->chapterCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->titleCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->trackCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->audioLangDataChanged((*reinterpret_cast< AudioLangData*(*)>(_a[1]))); break;
        case 8: _t->activeAudioLangChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->activeAudioLangStringChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 10: _t->captionDataChanged((*reinterpret_cast< CaptionData*(*)>(_a[1]))); break;
        case 11: _t->activeCaptionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->activeCaptionStringChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->angleDataChanged((*reinterpret_cast< AngleData*(*)>(_a[1]))); break;
        case 14: _t->activeAngleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->activeLangCodeChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 16: _t->isStartedBySettingsAppChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: _t->playerStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->playerScreenStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->listBtnVisibleChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_CModel_DVD::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_CModel_DVD::staticMetaObject = {
    { &AppMediaPlayer_Video_CModel_Base::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_CModel_DVD,
      qt_meta_data_AppMediaPlayer_Video_CModel_DVD, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_CModel_DVD::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_CModel_DVD::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_CModel_DVD::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_CModel_DVD))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_CModel_DVD*>(this));
    return AppMediaPlayer_Video_CModel_Base::qt_metacast(_clname);
}

int AppMediaPlayer_Video_CModel_DVD::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppMediaPlayer_Video_CModel_Base::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_CModel_DVD::chapterChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_CModel_DVD::titleChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Video_CModel_DVD::focusChapterChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppMediaPlayer_Video_CModel_DVD::focusTitleChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AppMediaPlayer_Video_CModel_DVD::chapterCountChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AppMediaPlayer_Video_CModel_DVD::titleCountChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void AppMediaPlayer_Video_CModel_DVD::trackCountChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void AppMediaPlayer_Video_CModel_DVD::audioLangDataChanged(AudioLangData * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void AppMediaPlayer_Video_CModel_DVD::activeAudioLangChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void AppMediaPlayer_Video_CModel_DVD::activeAudioLangStringChanged(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void AppMediaPlayer_Video_CModel_DVD::captionDataChanged(CaptionData * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void AppMediaPlayer_Video_CModel_DVD::activeCaptionChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void AppMediaPlayer_Video_CModel_DVD::activeCaptionStringChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void AppMediaPlayer_Video_CModel_DVD::angleDataChanged(AngleData * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void AppMediaPlayer_Video_CModel_DVD::activeAngleChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void AppMediaPlayer_Video_CModel_DVD::activeLangCodeChanged(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void AppMediaPlayer_Video_CModel_DVD::isStartedBySettingsAppChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void AppMediaPlayer_Video_CModel_DVD::playerStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void AppMediaPlayer_Video_CModel_DVD::playerScreenStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void AppMediaPlayer_Video_CModel_DVD::listBtnVisibleChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}
QT_END_MOC_NAMESPACE
