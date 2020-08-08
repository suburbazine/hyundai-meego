/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Thumbnailer.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/util/photo/DHAVN_AppMediaPlayer_Thumbnailer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Thumbnailer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PhotoThumbnailer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   18,   17,   17, 0x05,
      98,   66,   17,   17, 0x05,
     142,  136,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     202,  168,   17,   17, 0x0a,
     264,  244,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PhotoThumbnailer[] = {
    "PhotoThumbnailer\0\0image,thumbnail\0"
    "thumbnailReady(QString,QString)\0"
    "image,thumbnailName,mediaSource\0"
    "processThumbnail(QString,QString,int)\0"
    "image\0fileNotSupported(QString)\0"
    "image,thumbnailPath,thumbnailName\0"
    "onThumbnailReady(QString,QString,QString)\0"
    "image,thumbnailName\0"
    "onFileNotSupported(QString,QString)\0"
};

void PhotoThumbnailer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PhotoThumbnailer *_t = static_cast<PhotoThumbnailer *>(_o);
        switch (_id) {
        case 0: _t->thumbnailReady((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->processThumbnail((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->fileNotSupported((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->onThumbnailReady((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 4: _t->onFileNotSupported((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PhotoThumbnailer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PhotoThumbnailer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PhotoThumbnailer,
      qt_meta_data_PhotoThumbnailer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PhotoThumbnailer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PhotoThumbnailer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PhotoThumbnailer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PhotoThumbnailer))
        return static_cast<void*>(const_cast< PhotoThumbnailer*>(this));
    return QObject::qt_metacast(_clname);
}

int PhotoThumbnailer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void PhotoThumbnailer::thumbnailReady(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PhotoThumbnailer::processThumbnail(const QString & _t1, const QString & _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PhotoThumbnailer::fileNotSupported(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
static const uint qt_meta_data_PhotoThumbnailProcessor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      59,   25,   24,   24, 0x05,
     119,   99,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
     189,  153,   24,   24, 0x0a,
     227,   24,   24,   24, 0x0a,
     250,   24,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PhotoThumbnailProcessor[] = {
    "PhotoThumbnailProcessor\0\0"
    "image,thumbnailPath,thumbnailName\0"
    "thumbnailReady(QString,QString,QString)\0"
    "image,thumbnailName\0"
    "fileNotSupported(QString,QString)\0"
    "image,thumbnailFileName,mediaSource\0"
    "processThumbnail(QString,QString,int)\0"
    "fillThumbnailerCache()\0createDirIfFormatted()\0"
};

void PhotoThumbnailProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PhotoThumbnailProcessor *_t = static_cast<PhotoThumbnailProcessor *>(_o);
        switch (_id) {
        case 0: _t->thumbnailReady((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: _t->fileNotSupported((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->processThumbnail((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->fillThumbnailerCache(); break;
        case 4: _t->createDirIfFormatted(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PhotoThumbnailProcessor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PhotoThumbnailProcessor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PhotoThumbnailProcessor,
      qt_meta_data_PhotoThumbnailProcessor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PhotoThumbnailProcessor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PhotoThumbnailProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PhotoThumbnailProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PhotoThumbnailProcessor))
        return static_cast<void*>(const_cast< PhotoThumbnailProcessor*>(this));
    return QObject::qt_metacast(_clname);
}

int PhotoThumbnailProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void PhotoThumbnailProcessor::thumbnailReady(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PhotoThumbnailProcessor::fileNotSupported(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
