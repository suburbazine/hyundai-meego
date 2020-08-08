/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_MOSTManager_SoftwareDownloadFBlockShadowControl.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_MOSTManager_SoftwareDownloadFBlockShadowControl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_MOSTManager_SoftwareDownloadFBlockShadowControl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CIBOXSWDLFblockStateListener[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      30,   29,   29,   29, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CIBOXSWDLFblockStateListener[] = {
    "CIBOXSWDLFblockStateListener\0\0"
    "OnWaitForTimeout()\0"
};

void CIBOXSWDLFblockStateListener::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CIBOXSWDLFblockStateListener *_t = static_cast<CIBOXSWDLFblockStateListener *>(_o);
        switch (_id) {
        case 0: _t->OnWaitForTimeout(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CIBOXSWDLFblockStateListener::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CIBOXSWDLFblockStateListener::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CIBOXSWDLFblockStateListener,
      qt_meta_data_CIBOXSWDLFblockStateListener, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CIBOXSWDLFblockStateListener::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CIBOXSWDLFblockStateListener::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CIBOXSWDLFblockStateListener::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CIBOXSWDLFblockStateListener))
        return static_cast<void*>(const_cast< CIBOXSWDLFblockStateListener*>(this));
    if (!strcmp(_clname, "IACSMOSTEventListener"))
        return static_cast< IACSMOSTEventListener*>(const_cast< CIBOXSWDLFblockStateListener*>(this));
    return QObject::qt_metacast(_clname);
}

int CIBOXSWDLFblockStateListener::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_CSWDLFBlockShadowControl[] = {

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
      46,   26,   25,   25, 0x05,
      95,   77,   25,   25, 0x05,
     138,  121,   25,   25, 0x05,
     185,  166,   25,   25, 0x05,
     242,  221,   25,   25, 0x05,
     297,  279,   25,   25, 0x05,
     340,  329,   25,   25, 0x05,
     383,  371,   25,   25, 0x05,
     425,  409,   25,   25, 0x05,
     475,  455,   25,   25, 0x05,
     525,  509,   25,   25, 0x05,
     571,  555,   25,   25, 0x05,
     628,  601,   25,   25, 0x05,
     692,  672,   25,   25, 0x05,
     746,  726,   25,   25, 0x05,
     811,  780,   25,   25, 0x05,
     864,  844,   25,   25, 0x05,
     909,  903,   25,   25, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CSWDLFBlockShadowControl[] = {
    "CSWDLFBlockShadowControl\0\0swdlid,hwidentifier\0"
    "ReadHWIdentifier(uint,QString)\0"
    "swdlid,deviceName\0ReadDevName(uint,QString)\0"
    "swdlid,hwversion\0ReadHWVersion(uint,QString)\0"
    "swdlid,modulevalue\0"
    "ReadNumberofModule(uint,QByteArray)\0"
    "swdlid,moduleversion\0"
    "ReadModuleVersion(uint,QDBusVariant)\0"
    "reprogrammingmode\0ReprogrammingMode(QDBusVariant)\0"
    "initUpdate\0InitUpdateResult(QDBusVariant)\0"
    "eraseMemory\0EraseMemory(QDBusVariant)\0"
    "requestdownload\0RequestDownload(QDBusVariant)\0"
    "PrepareFileTransfer\0"
    "PrepareFileTransfer(QDBusVariant)\0"
    "transferdataAMS\0TransferDataCMS(QDBusVariant)\0"
    "transferdataCMS\0TransferDataAMS(QDBusVariant)\0"
    "swdlid,errorCode,errorInfo\0"
    "TransferDataAMSError(uint,uchar,QByteArray)\0"
    "checkactivatemodule\0"
    "CheckActivateModule(QDBusVariant)\0"
    "requesttransferexit\0"
    "RequestTransferExit(QDBusVariant)\0"
    "swdlid,swdlOpMode,swdlProgress\0"
    "SWDLProgressInfo(uint,uint,uint)\0"
    "swdlid,updateStatus\0"
    "UpdateProgressingInfoStatus(uint,uint)\0"
    "state\0iBoxSWDLFBlockVisible(bool)\0"
};

void CSWDLFBlockShadowControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CSWDLFBlockShadowControl *_t = static_cast<CSWDLFBlockShadowControl *>(_o);
        switch (_id) {
        case 0: _t->ReadHWIdentifier((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->ReadDevName((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->ReadHWVersion((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->ReadNumberofModule((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 4: _t->ReadModuleVersion((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QDBusVariant(*)>(_a[2]))); break;
        case 5: _t->ReprogrammingMode((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 6: _t->InitUpdateResult((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 7: _t->EraseMemory((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 8: _t->RequestDownload((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 9: _t->PrepareFileTransfer((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 10: _t->TransferDataCMS((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 11: _t->TransferDataAMS((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 12: _t->TransferDataAMSError((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uchar(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 13: _t->CheckActivateModule((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 14: _t->RequestTransferExit((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 15: _t->SWDLProgressInfo((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 16: _t->UpdateProgressingInfoStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 17: _t->iBoxSWDLFBlockVisible((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CSWDLFBlockShadowControl::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CSWDLFBlockShadowControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CSWDLFBlockShadowControl,
      qt_meta_data_CSWDLFBlockShadowControl, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CSWDLFBlockShadowControl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CSWDLFBlockShadowControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CSWDLFBlockShadowControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSWDLFBlockShadowControl))
        return static_cast<void*>(const_cast< CSWDLFBlockShadowControl*>(this));
    if (!strcmp(_clname, "CControlThreadBase"))
        return static_cast< CControlThreadBase*>(const_cast< CSWDLFBlockShadowControl*>(this));
    return QObject::qt_metacast(_clname);
}

int CSWDLFBlockShadowControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CSWDLFBlockShadowControl::ReadHWIdentifier(const uint _t1, const QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CSWDLFBlockShadowControl::ReadDevName(const uint _t1, const QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CSWDLFBlockShadowControl::ReadHWVersion(const uint _t1, const QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CSWDLFBlockShadowControl::ReadNumberofModule(const uint _t1, const QByteArray _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CSWDLFBlockShadowControl::ReadModuleVersion(const uint _t1, const QDBusVariant & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CSWDLFBlockShadowControl::ReprogrammingMode(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CSWDLFBlockShadowControl::InitUpdateResult(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CSWDLFBlockShadowControl::EraseMemory(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CSWDLFBlockShadowControl::RequestDownload(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CSWDLFBlockShadowControl::PrepareFileTransfer(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CSWDLFBlockShadowControl::TransferDataCMS(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CSWDLFBlockShadowControl::TransferDataAMS(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CSWDLFBlockShadowControl::TransferDataAMSError(const uint _t1, const uchar _t2, const QByteArray _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CSWDLFBlockShadowControl::CheckActivateModule(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CSWDLFBlockShadowControl::RequestTransferExit(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CSWDLFBlockShadowControl::SWDLProgressInfo(const uint _t1, const uint _t2, const uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void CSWDLFBlockShadowControl::UpdateProgressingInfoStatus(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void CSWDLFBlockShadowControl::iBoxSWDLFBlockVisible(const bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}
QT_END_MOC_NAMESPACE
