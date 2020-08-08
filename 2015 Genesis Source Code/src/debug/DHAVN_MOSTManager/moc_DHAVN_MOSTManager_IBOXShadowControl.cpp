/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_MOSTManager_IBOXShadowControl.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_MOSTManager_IBOXShadowControl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_MOSTManager_IBOXShadowControl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CIBOXShadowControl[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      18,       // signalCount

 // signals: signature, parameters, type, tag, flags
      38,   20,   19,   19, 0x05,
      80,   58,   19,   19, 0x05,
     122,  108,   19,   19, 0x05,
     173,  153,   19,   19, 0x05,
     225,  206,   19,   19, 0x05,
     280,  263,   19,   19, 0x05,
     332,  311,   19,   19, 0x05,
     388,  367,   19,   19, 0x05,
     443,  424,   19,   19, 0x05,
     487,  466,   19,   19, 0x05,
     522,  424,   19,   19, 0x05,
     592,  550,   19,   19, 0x05,
     646,  639,   19,   19, 0x05,
     700,  679,   19,   19, 0x05,
     737,  153,   19,   19, 0x05,
     797,  769,   19,   19, 0x05,
     865,  827,   19,   19, 0x05,
     936,  918,   19,   19, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CIBOXShadowControl[] = {
    "CIBOXShadowControl\0\0mode,clientStatus\0"
    "WifiMode(uint,uint)\0appStatus,appProgress\0"
    "AppInstallStatus(uint,uint)\0uri,available\0"
    "GISDataAvailable(QString,uint)\0"
    "errorCode,errorInfo\0"
    "GISDataAvailableError(uint,uint)\0"
    "senderHandle,state\0"
    "OpenBrowserRequestResult(ushort,uint)\0"
    "gisRequestResult\0GISRequestResult(QDBusVariant)\0"
    "gisDataRequestResult\0"
    "GISDataRequestResult(QDBusVariant)\0"
    "target,available,uri\0"
    "POIDataAvailable(uint,uint,QString)\0"
    "available,poiCount\0ReceivedPOI(uint,uint)\0"
    "poiDataRequestResult\0"
    "POIDataRequestResult(QDBusVariant)\0"
    "POFDataAvailable(uint,uint)\0"
    "state,senderHandle,blockType,blockCounter\0"
    "VoiceSearchOnlineStatus(uint,ushort,uint,uint)\0"
    "status\0IBOXFunctionBlockInvisible(bool)\0"
    "optype,uri,available\0"
    "LSDataAvailable(uchar,QString,uchar)\0"
    "LSDataAvailableError(uint,uint)\0"
    "optype,senderHandle,success\0"
    "LSRequest(uchar,ushort,uchar)\0"
    "optype,senderHandle,target,uri,lsdata\0"
    "LSDataRequest(uchar,ushort,uchar,QString,QByteArray)\0"
    "ACNActivatedValue\0ACNActivated(bool)\0"
};

void CIBOXShadowControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CIBOXShadowControl *_t = static_cast<CIBOXShadowControl *>(_o);
        switch (_id) {
        case 0: _t->WifiMode((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 1: _t->AppInstallStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 2: _t->GISDataAvailable((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 3: _t->GISDataAvailableError((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 4: _t->OpenBrowserRequestResult((*reinterpret_cast< const ushort(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 5: _t->GISRequestResult((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 6: _t->GISDataRequestResult((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 7: _t->POIDataAvailable((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 8: _t->ReceivedPOI((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 9: _t->POIDataRequestResult((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 10: _t->POFDataAvailable((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 11: _t->VoiceSearchOnlineStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])),(*reinterpret_cast< const uint(*)>(_a[4]))); break;
        case 12: _t->IBOXFunctionBlockInvisible((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 13: _t->LSDataAvailable((*reinterpret_cast< const uchar(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const uchar(*)>(_a[3]))); break;
        case 14: _t->LSDataAvailableError((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 15: _t->LSRequest((*reinterpret_cast< const uchar(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const uchar(*)>(_a[3]))); break;
        case 16: _t->LSDataRequest((*reinterpret_cast< const uchar(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const uchar(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QByteArray(*)>(_a[5]))); break;
        case 17: _t->ACNActivated((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CIBOXShadowControl::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CIBOXShadowControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CIBOXShadowControl,
      qt_meta_data_CIBOXShadowControl, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CIBOXShadowControl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CIBOXShadowControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CIBOXShadowControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CIBOXShadowControl))
        return static_cast<void*>(const_cast< CIBOXShadowControl*>(this));
    if (!strcmp(_clname, "CControlThreadBase"))
        return static_cast< CControlThreadBase*>(const_cast< CIBOXShadowControl*>(this));
    return QObject::qt_metacast(_clname);
}

int CIBOXShadowControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void CIBOXShadowControl::WifiMode(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CIBOXShadowControl::AppInstallStatus(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CIBOXShadowControl::GISDataAvailable(const QString _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CIBOXShadowControl::GISDataAvailableError(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CIBOXShadowControl::OpenBrowserRequestResult(const ushort _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CIBOXShadowControl::GISRequestResult(const QDBusVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CIBOXShadowControl::GISDataRequestResult(const QDBusVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CIBOXShadowControl::POIDataAvailable(const uint _t1, const uint _t2, const QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CIBOXShadowControl::ReceivedPOI(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CIBOXShadowControl::POIDataRequestResult(const QDBusVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CIBOXShadowControl::POFDataAvailable(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CIBOXShadowControl::VoiceSearchOnlineStatus(const uint _t1, const ushort _t2, const uint _t3, const uint _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CIBOXShadowControl::IBOXFunctionBlockInvisible(const bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CIBOXShadowControl::LSDataAvailable(const uchar _t1, const QString _t2, const uchar _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CIBOXShadowControl::LSDataAvailableError(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CIBOXShadowControl::LSRequest(const uchar _t1, const ushort _t2, const uchar _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void CIBOXShadowControl::LSDataRequest(const uchar _t1, const ushort _t2, const uchar _t3, const QString _t4, const QByteArray _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void CIBOXShadowControl::ACNActivated(const bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}
QT_END_MOC_NAMESPACE
