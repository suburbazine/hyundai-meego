/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFileManager_Thumbnailer.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFileManager_Thumbnailer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFileManager_Thumbnailer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Thumbnailer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      48,   13,   12,   12, 0x05,
      92,   86,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     150,  118,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Thumbnailer[] = {
    "Thumbnailer\0\0thumbnail,unsupported,index,bFront\0"
    "thumbnailReady(QString,bool,int,bool)\0"
    "image\0processThumbnail(QString)\0"
    "image,thumbnailPath,unsupported\0"
    "onThumbnailReady(QString,QString,bool)\0"
};

void Thumbnailer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Thumbnailer *_t = static_cast<Thumbnailer *>(_o);
        switch (_id) {
        case 0: _t->thumbnailReady((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 1: _t->processThumbnail((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->onThumbnailReady((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Thumbnailer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Thumbnailer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Thumbnailer,
      qt_meta_data_Thumbnailer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Thumbnailer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Thumbnailer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Thumbnailer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Thumbnailer))
        return static_cast<void*>(const_cast< Thumbnailer*>(this));
    return QObject::qt_metacast(_clname);
}

int Thumbnailer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Thumbnailer::thumbnailReady(const QString & _t1, bool _t2, int _t3, bool _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Thumbnailer::processThumbnail(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_ThumbnailProcessor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      52,   20,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      95,   89,   19,   19, 0x0a,
     121,   19,   19,   19, 0x0a,
     144,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ThumbnailProcessor[] = {
    "ThumbnailProcessor\0\0image,thumbnailPath,unsupported\0"
    "thumbnailReady(QString,QString,bool)\0"
    "image\0processThumbnail(QString)\0"
    "fillThumbnailerCache()\0createDirIfFormatted()\0"
};

void ThumbnailProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ThumbnailProcessor *_t = static_cast<ThumbnailProcessor *>(_o);
        switch (_id) {
        case 0: _t->thumbnailReady((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 1: _t->processThumbnail((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->fillThumbnailerCache(); break;
        case 3: _t->createDirIfFormatted(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ThumbnailProcessor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ThumbnailProcessor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ThumbnailProcessor,
      qt_meta_data_ThumbnailProcessor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ThumbnailProcessor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ThumbnailProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ThumbnailProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ThumbnailProcessor))
        return static_cast<void*>(const_cast< ThumbnailProcessor*>(this));
    return QObject::qt_metacast(_clname);
}

int ThumbnailProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void ThumbnailProcessor::thumbnailReady(const QString & _t1, const QString & _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
