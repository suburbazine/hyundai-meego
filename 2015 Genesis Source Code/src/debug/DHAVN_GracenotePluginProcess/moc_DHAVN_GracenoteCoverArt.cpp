/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_GracenoteCoverArt.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_GracenoteCoverArt.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_GracenoteCoverArt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CGracenoteCoverArt[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      37,   20,   19,   19, 0x05,
      76,   69,   19,   19, 0x25,

       0        // eod
};

static const char qt_meta_stringdata_CGracenoteCoverArt[] = {
    "CGracenoteCoverArt\0\0status,imagePath\0"
    "coverArtRetrieved(bool,QString)\0status\0"
    "coverArtRetrieved(bool)\0"
};

void CGracenoteCoverArt::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CGracenoteCoverArt *_t = static_cast<CGracenoteCoverArt *>(_o);
        switch (_id) {
        case 0: _t->coverArtRetrieved((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->coverArtRetrieved((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CGracenoteCoverArt::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CGracenoteCoverArt::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CGracenoteCoverArt,
      qt_meta_data_CGracenoteCoverArt, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CGracenoteCoverArt::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CGracenoteCoverArt::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CGracenoteCoverArt::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CGracenoteCoverArt))
        return static_cast<void*>(const_cast< CGracenoteCoverArt*>(this));
    return QObject::qt_metacast(_clname);
}

int CGracenoteCoverArt::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CGracenoteCoverArt::coverArtRetrieved(bool _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
