/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IpodDataProvider.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_IpodDataProvider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IpodDataProvider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CIpodDataProvider[] = {

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
      36,   19,   18,   18, 0x05,
      99,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     110,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CIpodDataProvider[] = {
    "CIpodDataProvider\0\0dataList,queryId\0"
    "DataFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids)\0"
    "Error(int)\0OnFinished()\0"
};

void CIpodDataProvider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CIpodDataProvider *_t = static_cast<CIpodDataProvider *>(_o);
        switch (_id) {
        case 0: _t->DataFetched((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2]))); break;
        case 1: _t->Error((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->OnFinished(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CIpodDataProvider::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CIpodDataProvider::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CIpodDataProvider,
      qt_meta_data_CIpodDataProvider, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CIpodDataProvider::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CIpodDataProvider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CIpodDataProvider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CIpodDataProvider))
        return static_cast<void*>(const_cast< CIpodDataProvider*>(this));
    return QObject::qt_metacast(_clname);
}

int CIpodDataProvider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CIpodDataProvider::DataFetched(QVector<QStringList> _t1, ETrackerAbstarctor_Query_Ids _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CIpodDataProvider::Error(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
