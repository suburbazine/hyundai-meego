/****************************************************************************
** Meta object code from reading C++ file 'ahainterface.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ahainterface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ahainterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CAhaInterface[] = {

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
      36,   15,   14,   14, 0x05,
     108,   87,   14,   14, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CAhaInterface[] = {
    "CAhaInterface\0\0qVectContentListInfo\0"
    "contentListLoaded(QVector<AhaContentBriefInfo_T>&)\0"
    "qVectStationListInfo\0"
    "stationListLoaded(QVector<AhaStationBriefInfo_T>&)\0"
};

void CAhaInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CAhaInterface *_t = static_cast<CAhaInterface *>(_o);
        switch (_id) {
        case 0: _t->contentListLoaded((*reinterpret_cast< QVector<AhaContentBriefInfo_T>(*)>(_a[1]))); break;
        case 1: _t->stationListLoaded((*reinterpret_cast< QVector<AhaStationBriefInfo_T>(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CAhaInterface::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAhaInterface::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CAhaInterface,
      qt_meta_data_CAhaInterface, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAhaInterface::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAhaInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAhaInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAhaInterface))
        return static_cast<void*>(const_cast< CAhaInterface*>(this));
    return QObject::qt_metacast(_clname);
}

int CAhaInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CAhaInterface::contentListLoaded(QVector<AhaContentBriefInfo_T> & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CAhaInterface::stationListLoaded(QVector<AhaStationBriefInfo_T> & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
