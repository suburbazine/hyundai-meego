/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_PathViewModel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/util/audio/DHAVN_AppMediaPlayer_PathViewModel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_PathViewModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PathViewModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      50,   30,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      78,   14,   14,   14, 0x08,

 // methods: signature, parameters, type, tag, flags
      97,   14,   14,   14, 0x02,
     128,  122,  114,   14, 0x02,
     146,  122,  114,   14, 0x02,
     165,  122,  114,   14, 0x02,
     186,  182,   14,   14, 0x02,
     212,  182,   14,   14, 0x02,
     245,   14,  240,   14, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_PathViewModel[] = {
    "PathViewModel\0\0requestCover()\0"
    "coverIndex,isScroll\0setHighlightCover(int,bool)\0"
    "requestCoverSlot()\0clearModelData()\0"
    "QString\0index\0getAlbumName(int)\0"
    "getArtistName(int)\0getFilePath(int)\0"
    "idx\0onComponentCompleted(int)\0"
    "onComponentDestruction(int)\0bool\0"
    "isBusy()\0"
};

void PathViewModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PathViewModel *_t = static_cast<PathViewModel *>(_o);
        switch (_id) {
        case 0: _t->requestCover(); break;
        case 1: _t->setHighlightCover((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->requestCoverSlot(); break;
        case 3: _t->clearModelData(); break;
        case 4: { QString _r = _t->getAlbumName((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: { QString _r = _t->getArtistName((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 6: { QString _r = _t->getFilePath((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 7: _t->onComponentCompleted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->onComponentDestruction((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: { bool _r = _t->isBusy();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PathViewModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PathViewModel::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_PathViewModel,
      qt_meta_data_PathViewModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PathViewModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PathViewModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PathViewModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PathViewModel))
        return static_cast<void*>(const_cast< PathViewModel*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int PathViewModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void PathViewModel::requestCover()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void PathViewModel::setHighlightCover(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
