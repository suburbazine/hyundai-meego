/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_DirListProvider.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_DirListProvider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_DirListProvider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CDirListProvider[] = {

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
      20,   18,   17,   17, 0x05,
      83,   17,   17,   17, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CDirListProvider[] = {
    "CDirListProvider\0\0,\0"
    "DataFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids)\0"
    "Error(int)\0"
};

void CDirListProvider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CDirListProvider *_t = static_cast<CDirListProvider *>(_o);
        switch (_id) {
        case 0: _t->DataFetched((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2]))); break;
        case 1: _t->Error((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CDirListProvider::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CDirListProvider::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CDirListProvider,
      qt_meta_data_CDirListProvider, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CDirListProvider::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CDirListProvider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CDirListProvider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDirListProvider))
        return static_cast<void*>(const_cast< CDirListProvider*>(this));
    return QThread::qt_metacast(_clname);
}

int CDirListProvider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
void CDirListProvider::DataFetched(QVector<QStringList> _t1, ETrackerAbstarctor_Query_Ids _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CDirListProvider::Error(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
