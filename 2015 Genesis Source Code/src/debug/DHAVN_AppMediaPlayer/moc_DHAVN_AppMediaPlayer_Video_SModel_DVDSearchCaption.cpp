/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_SModel_DVDSearchCaption.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_DVDSearchCaption.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_SModel_DVDSearchCaption.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_SModel_DVDSearchCaption[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       2,   34, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      46,   45,   45,   45, 0x05,
      72,   45,   45,   45, 0x05,

 // slots: signature, parameters, type, tag, flags
     110,   45,   45,   45, 0x0a,
     132,   45,   45,   45, 0x0a,

 // properties: name, type, flags
     174,  166, 0x0a495103,
     196,  184, 0x0b495001,

 // properties: notify_signal_id
       0,
       1,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_SModel_DVDSearchCaption[] = {
    "AppMediaPlayer_Video_SModel_DVDSearchCaption\0"
    "\0inputTextChanged(QString)\0"
    "captionStringListChanged(QStringList)\0"
    "setInputText(QString)\0"
    "setCaptionStringList(QStringList)\0"
    "QString\0inputText\0QStringList\0"
    "captionStringList\0"
};

void AppMediaPlayer_Video_SModel_DVDSearchCaption::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_SModel_DVDSearchCaption *_t = static_cast<AppMediaPlayer_Video_SModel_DVDSearchCaption *>(_o);
        switch (_id) {
        case 0: _t->inputTextChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->captionStringListChanged((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 2: _t->setInputText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->setCaptionStringList((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_SModel_DVDSearchCaption::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_SModel_DVDSearchCaption::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_SModel_DVDSearchCaption,
      qt_meta_data_AppMediaPlayer_Video_SModel_DVDSearchCaption, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_SModel_DVDSearchCaption::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_SModel_DVDSearchCaption::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_SModel_DVDSearchCaption::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_SModel_DVDSearchCaption))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_SModel_DVDSearchCaption*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_Video_SModel_DVDSearchCaption::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = inputText(); break;
        case 1: *reinterpret_cast< QStringList*>(_v) = captionStringList(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setInputText(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_SModel_DVDSearchCaption::inputTextChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_SModel_DVDSearchCaption::captionStringListChanged(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
