/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_SModel_DVDSettings.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_DVDSettings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_SModel_DVDSettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DVDSettingsSModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       4,   89, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   19,   18,   18, 0x05,
      67,   19,   18,   18, 0x05,
     106,   19,   18,   18, 0x05,
     147,  141,   18,   18, 0x05,
     172,  141,   18,   18, 0x05,
     201,  195,   18,   18, 0x05,
     227,  195,   18,   18, 0x05,
     255,  195,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     282,  195,   18,   18, 0x0a,
     304,  195,   18,   18, 0x0a,
     328,  195,   18,   18, 0x0a,
     348,  195,   18,   18, 0x0a,
     371,  141,   18,   18, 0x0a,
     392,  141,   18,   18, 0x0a,
     449,  410,   18,   18, 0x0a,

 // properties: name, type, flags
     488,  484, 0x02495001,
     498,  484, 0x02495001,
     512,  484, 0x02495001,
     528,  484, 0x02495001,

 // properties: notify_signal_id
       4,
       5,
       6,
       7,

       0        // eod
};

static const char qt_meta_stringdata_DVDSettingsSModel[] = {
    "DVDSettingsSModel\0\0list,index\0"
    "captionDataChanged(QStringList*,int)\0"
    "audioLangDataChanged(QStringList*,int)\0"
    "angleDataChanged(QStringList*,int)\0"
    "value\0isDVDDimmedChanged(bool)\0"
    "activeTabChanged(bool)\0index\0"
    "activeCaptionChanged(int)\0"
    "activeAudioLangChanged(int)\0"
    "activeMenuLangChanged(int)\0"
    "setActiveCaption(int)\0setActiveAudioLang(int)\0"
    "setActiveAngle(int)\0setActiveMenuLang(int)\0"
    "setIsDVDDimmed(bool)\0setActiveTab(int)\0"
    "captionCode,audioLangCode,menuLangCode\0"
    "activeLangCodeChanged(int,int,int)\0"
    "int\0activeTab\0activeCaption\0activeAudioLang\0"
    "activeMenuLang\0"
};

void DVDSettingsSModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DVDSettingsSModel *_t = static_cast<DVDSettingsSModel *>(_o);
        switch (_id) {
        case 0: _t->captionDataChanged((*reinterpret_cast< QStringList*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->audioLangDataChanged((*reinterpret_cast< QStringList*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->angleDataChanged((*reinterpret_cast< QStringList*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->isDVDDimmedChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->activeTabChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->activeCaptionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->activeAudioLangChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->activeMenuLangChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->setActiveCaption((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->setActiveAudioLang((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->setActiveAngle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->setActiveMenuLang((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->setIsDVDDimmed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->setActiveTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->activeLangCodeChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DVDSettingsSModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DVDSettingsSModel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DVDSettingsSModel,
      qt_meta_data_DVDSettingsSModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DVDSettingsSModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DVDSettingsSModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DVDSettingsSModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DVDSettingsSModel))
        return static_cast<void*>(const_cast< DVDSettingsSModel*>(this));
    return QObject::qt_metacast(_clname);
}

int DVDSettingsSModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = activeTab(); break;
        case 1: *reinterpret_cast< int*>(_v) = activeCaption(); break;
        case 2: *reinterpret_cast< int*>(_v) = activeAudioLang(); break;
        case 3: *reinterpret_cast< int*>(_v) = activeMenuLang(); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DVDSettingsSModel::captionDataChanged(QStringList * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DVDSettingsSModel::audioLangDataChanged(QStringList * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DVDSettingsSModel::angleDataChanged(QStringList * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DVDSettingsSModel::isDVDDimmedChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DVDSettingsSModel::activeTabChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DVDSettingsSModel::activeCaptionChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DVDSettingsSModel::activeAudioLangChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void DVDSettingsSModel::activeMenuLangChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
