/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_MOSTManager_NavigationFBlockControl.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_MOSTManager_NavigationFBlockControl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_MOSTManager_NavigationFBlockControl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CNavigationFblockControl[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      59,   26,   25,   25, 0x05,
     132,  100,   25,   25, 0x05,
     216,  176,   25,   25, 0x05,
     266,  254,   25,   25, 0x05,
     300,  292,   25,   25, 0x05,
     341,  322,   25,   25, 0x05,
     402,  374,   25,   25, 0x05,
     450,  433,   25,   25, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CNavigationFblockControl[] = {
    "CNavigationFblockControl\0\0"
    "optype,senderHandle,inputKeyword\0"
    "RequestLocalSearch(uchar,ushort,QString)\0"
    "optype,senderHandle,target,text\0"
    "OpenLocalSearch(uchar,ushort,uchar,QString)\0"
    "optype,senderHandle,target,inputKeyword\0"
    "SearchPOI(uchar,ushort,uchar,QString)\0"
    "searchRoute\0SearchRoute(QDBusVariant)\0"
    "showMap\0ShowMap(QDBusVariant)\0"
    "registerMyLocation\0RegisterMyLocation(QDBusVariant)\0"
    "senderHandle,target,poiKind\0"
    "FindNearBy(ushort,uchar,uchar)\0"
    "checkGPSBoundary\0CheckGPSBoundary(QDBusVariant)\0"
};

void CNavigationFblockControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CNavigationFblockControl *_t = static_cast<CNavigationFblockControl *>(_o);
        switch (_id) {
        case 0: _t->RequestLocalSearch((*reinterpret_cast< const uchar(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: _t->OpenLocalSearch((*reinterpret_cast< const uchar(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const uchar(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 2: _t->SearchPOI((*reinterpret_cast< const uchar(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const uchar(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 3: _t->SearchRoute((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 4: _t->ShowMap((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 5: _t->RegisterMyLocation((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 6: _t->FindNearBy((*reinterpret_cast< const ushort(*)>(_a[1])),(*reinterpret_cast< const uchar(*)>(_a[2])),(*reinterpret_cast< const uchar(*)>(_a[3]))); break;
        case 7: _t->CheckGPSBoundary((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CNavigationFblockControl::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CNavigationFblockControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CNavigationFblockControl,
      qt_meta_data_CNavigationFblockControl, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CNavigationFblockControl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CNavigationFblockControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CNavigationFblockControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CNavigationFblockControl))
        return static_cast<void*>(const_cast< CNavigationFblockControl*>(this));
    if (!strcmp(_clname, "CControlThreadBase"))
        return static_cast< CControlThreadBase*>(const_cast< CNavigationFblockControl*>(this));
    return QObject::qt_metacast(_clname);
}

int CNavigationFblockControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CNavigationFblockControl::RequestLocalSearch(const uchar _t1, const ushort _t2, const QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CNavigationFblockControl::OpenLocalSearch(const uchar _t1, const ushort _t2, const uchar _t3, const QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CNavigationFblockControl::SearchPOI(const uchar _t1, const ushort _t2, const uchar _t3, const QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CNavigationFblockControl::SearchRoute(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CNavigationFblockControl::ShowMap(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CNavigationFblockControl::RegisterMyLocation(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CNavigationFblockControl::FindNearBy(const ushort _t1, const uchar _t2, const uchar _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CNavigationFblockControl::CheckGPSBoundary(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
