/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_SModel_Caption.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_Caption.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_SModel_Caption.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_SModel_Caption[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       5,   64, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      37,   36,   36,   36, 0x05,
      64,   36,   36,   36, 0x05,
      88,   36,   36,   36, 0x05,
     112,   36,   36,   36, 0x05,
     142,   36,   36,   36, 0x05,

 // slots: signature, parameters, type, tag, flags
     167,   36,   36,   36, 0x0a,
     190,   36,   36,   36, 0x0a,
     210,   36,   36,   36, 0x0a,
     230,   36,   36,   36, 0x0a,
     256,   36,   36,   36, 0x0a,

 // properties: name, type, flags
     282,  277, 0x01495001,
     300,  296, 0x02495001,
     320,  312, 0x0a495103,
     328,  277, 0x01495103,
     345,  277, 0x01495103,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_SModel_Caption[] = {
    "AppMediaPlayer_Video_SModel_Caption\0"
    "\0captionEnableChanged(bool)\0"
    "captionSizeChanged(int)\0uiStateChanged(QString)\0"
    "multipleCaptionsChanged(bool)\0"
    "smiCaptionsChanged(bool)\0"
    "setCaptionEnable(bool)\0setCaptionSize(int)\0"
    "setUiState(QString)\0setMultipleCaptions(bool)\0"
    "setSmiCaptions(bool)\0bool\0captionEnable\0"
    "int\0captionSize\0QString\0uiState\0"
    "multipleCaptions\0smiCaptions\0"
};

void AppMediaPlayer_Video_SModel_Caption::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_SModel_Caption *_t = static_cast<AppMediaPlayer_Video_SModel_Caption *>(_o);
        switch (_id) {
        case 0: _t->captionEnableChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->captionSizeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->uiStateChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->multipleCaptionsChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->smiCaptionsChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->setCaptionEnable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setCaptionSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->setUiState((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->setMultipleCaptions((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->setSmiCaptions((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_SModel_Caption::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_SModel_Caption::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_SModel_Caption,
      qt_meta_data_AppMediaPlayer_Video_SModel_Caption, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_SModel_Caption::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_SModel_Caption::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_SModel_Caption::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_SModel_Caption))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_SModel_Caption*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_Video_SModel_Caption::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = captionEnable(); break;
        case 1: *reinterpret_cast< int*>(_v) = captionSize(); break;
        case 2: *reinterpret_cast< QString*>(_v) = uiState(); break;
        case 3: *reinterpret_cast< bool*>(_v) = multipleCaptions(); break;
        case 4: *reinterpret_cast< bool*>(_v) = smiCaptions(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 2: setUiState(*reinterpret_cast< QString*>(_v)); break;
        case 3: setMultipleCaptions(*reinterpret_cast< bool*>(_v)); break;
        case 4: setSmiCaptions(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_SModel_Caption::captionEnableChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_SModel_Caption::captionSizeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Video_SModel_Caption::uiStateChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppMediaPlayer_Video_SModel_Caption::multipleCaptionsChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AppMediaPlayer_Video_SModel_Caption::smiCaptionsChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
