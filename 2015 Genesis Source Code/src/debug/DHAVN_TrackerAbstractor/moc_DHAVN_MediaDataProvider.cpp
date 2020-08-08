/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_MediaDataProvider.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_MediaDataProvider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_MediaDataProvider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CMediaDataProvider[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      37,   20,   19,   19, 0x05,
     124,  100,   19,   19, 0x05,
     196,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
     207,   19,   19,   19, 0x08,
     220,   19,   19,   19, 0x08,
     238,   19,   19,   19, 0x08,
     261,   19,   19,   19, 0x08,
     279,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CMediaDataProvider[] = {
    "CMediaDataProvider\0\0dataList,queryId\0"
    "DataFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids)\0"
    "dataList,queryId,offset\0"
    "DataLimitFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,int"
    ")\0"
    "Error(int)\0OnFinished()\0OnLimitFinished()\0"
    "OnLimitCacheFinished()\0OnIndexFinished()\0"
    "OnIndexWithCountFinished()\0"
};

void CMediaDataProvider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CMediaDataProvider *_t = static_cast<CMediaDataProvider *>(_o);
        switch (_id) {
        case 0: _t->DataFetched((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2]))); break;
        case 1: _t->DataLimitFetched((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->Error((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->OnFinished(); break;
        case 4: _t->OnLimitFinished(); break;
        case 5: _t->OnLimitCacheFinished(); break;
        case 6: _t->OnIndexFinished(); break;
        case 7: _t->OnIndexWithCountFinished(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CMediaDataProvider::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CMediaDataProvider::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CMediaDataProvider,
      qt_meta_data_CMediaDataProvider, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CMediaDataProvider::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CMediaDataProvider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CMediaDataProvider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CMediaDataProvider))
        return static_cast<void*>(const_cast< CMediaDataProvider*>(this));
    return QObject::qt_metacast(_clname);
}

int CMediaDataProvider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CMediaDataProvider::DataFetched(QVector<QStringList> _t1, ETrackerAbstarctor_Query_Ids _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CMediaDataProvider::DataLimitFetched(QVector<QStringList> _t1, ETrackerAbstarctor_Query_Ids _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CMediaDataProvider::Error(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
