/****************************************************************************
** Meta object code from reading C++ file 'most_updater.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/modules/most/most_updater.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'most_updater.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MOSTUpdater[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   13,   12,   12, 0x08,
      52,   12,   12,   12, 0x08,
      78,   76,   12,   12, 0x08,
     111,   12,   12,   12, 0x08,
     146,  137,   12,   12, 0x08,
     188,  180,   12,   12, 0x28,
     214,   12,   12,   12, 0x28,

       0        // eod
};

static const char qt_meta_stringdata_MOSTUpdater[] = {
    "MOSTUpdater\0\0,,,\0statusChanged(int,int,int,QString)\0"
    "cannotReceiveResponse()\0,\0"
    "blockTransfered(quint64,quint64)\0"
    "processIBoxWaitCount(int)\0isError,\0"
    "finishCurrentUpdate(bool,QString)\0"
    "isError\0finishCurrentUpdate(bool)\0"
    "finishCurrentUpdate()\0"
};

void MOSTUpdater::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MOSTUpdater *_t = static_cast<MOSTUpdater *>(_o);
        switch (_id) {
        case 0: _t->statusChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 1: _t->cannotReceiveResponse(); break;
        case 2: _t->blockTransfered((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2]))); break;
        case 3: _t->processIBoxWaitCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->finishCurrentUpdate((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->finishCurrentUpdate((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->finishCurrentUpdate(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MOSTUpdater::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MOSTUpdater::staticMetaObject = {
    { &AbstractUpdater::staticMetaObject, qt_meta_stringdata_MOSTUpdater,
      qt_meta_data_MOSTUpdater, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MOSTUpdater::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MOSTUpdater::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MOSTUpdater::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MOSTUpdater))
        return static_cast<void*>(const_cast< MOSTUpdater*>(this));
    return AbstractUpdater::qt_metacast(_clname);
}

int MOSTUpdater::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractUpdater::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
