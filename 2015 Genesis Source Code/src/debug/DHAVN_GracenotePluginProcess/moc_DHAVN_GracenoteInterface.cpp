/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_GracenoteInterface.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_GracenoteInterface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_GracenoteInterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CGracenoteInterface[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   21,   20,   20, 0x05,
      75,   58,   20,   20, 0x05,
     121,  107,   20,   20, 0x05,
     170,  153,   20,   20, 0x05,
     204,   20,   20,   20, 0x05,
     226,  222,   20,   20, 0x05,
     244,  222,   20,   20, 0x05,
     257,  222,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
     280,   58,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CGracenoteInterface[] = {
    "CGracenoteInterface\0\0status\0"
    "OnMusicIDCDDataReceived(bool)\0"
    "status,imagePath\0CoverArtRetrieved(bool,QString)\0"
    "imagePathList\0CoverArtsRetrieved(QStringList)\0"
    "start,deviceType\0GracenoteIndexingStatus(bool,int)\0"
    "GracenoteKilled()\0ver\0GNSDKVer(QString)\0"
    "GNDBVer(int)\0GNDBRegionVer(QString)\0"
    "onCoverArtRetrieved(bool,QString)\0"
};

void CGracenoteInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CGracenoteInterface *_t = static_cast<CGracenoteInterface *>(_o);
        switch (_id) {
        case 0: _t->OnMusicIDCDDataReceived((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->CoverArtRetrieved((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->CoverArtsRetrieved((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 3: _t->GracenoteIndexingStatus((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->GracenoteKilled(); break;
        case 5: _t->GNSDKVer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->GNDBVer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->GNDBRegionVer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->onCoverArtRetrieved((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CGracenoteInterface::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CGracenoteInterface::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CGracenoteInterface,
      qt_meta_data_CGracenoteInterface, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CGracenoteInterface::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CGracenoteInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CGracenoteInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CGracenoteInterface))
        return static_cast<void*>(const_cast< CGracenoteInterface*>(this));
    return QObject::qt_metacast(_clname);
}

int CGracenoteInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void CGracenoteInterface::OnMusicIDCDDataReceived(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CGracenoteInterface::CoverArtRetrieved(bool _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CGracenoteInterface::CoverArtsRetrieved(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CGracenoteInterface::GracenoteIndexingStatus(bool _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CGracenoteInterface::GracenoteKilled()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void CGracenoteInterface::GNSDKVer(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CGracenoteInterface::GNDBVer(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CGracenoteInterface::GNDBRegionVer(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
