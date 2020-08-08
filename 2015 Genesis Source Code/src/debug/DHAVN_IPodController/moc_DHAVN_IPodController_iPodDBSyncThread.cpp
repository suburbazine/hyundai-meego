/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_iPodDBSyncThread.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TrackerConnector/include/DHAVN_IPodController_iPodDBSyncThread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_iPodDBSyncThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CiPodDBSyncThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      36,   33,   18,   18, 0x05,
      77,   18,   18,   18, 0x05,
      97,   91,   18,   18, 0x05,
     129,   91,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     160,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CiPodDBSyncThread[] = {
    "CiPodDBSyncThread\0\0startDBSync()\0,,\0"
    "getArtWork(QStringList,QStringList,bool)\0"
    "requestQuit()\0pause\0OnDBIndexingStatusChanged(bool)\0"
    "OnChangeDBIndexingStatus(bool)\0"
    "requestDBSync()\0"
};

void CiPodDBSyncThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CiPodDBSyncThread *_t = static_cast<CiPodDBSyncThread *>(_o);
        switch (_id) {
        case 0: _t->startDBSync(); break;
        case 1: _t->getArtWork((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 2: _t->requestQuit(); break;
        case 3: _t->OnDBIndexingStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->OnChangeDBIndexingStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->requestDBSync(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CiPodDBSyncThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CiPodDBSyncThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CiPodDBSyncThread,
      qt_meta_data_CiPodDBSyncThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CiPodDBSyncThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CiPodDBSyncThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CiPodDBSyncThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CiPodDBSyncThread))
        return static_cast<void*>(const_cast< CiPodDBSyncThread*>(this));
    return QThread::qt_metacast(_clname);
}

int CiPodDBSyncThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CiPodDBSyncThread::startDBSync()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CiPodDBSyncThread::getArtWork(QStringList _t1, QStringList _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CiPodDBSyncThread::requestQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void CiPodDBSyncThread::OnDBIndexingStatusChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CiPodDBSyncThread::OnChangeDBIndexingStatus(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
