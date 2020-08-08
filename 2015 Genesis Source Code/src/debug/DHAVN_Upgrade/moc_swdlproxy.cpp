/****************************************************************************
** Meta object code from reading C++ file 'swdlproxy.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/modules/most/proxy/swdlproxy.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'swdlproxy.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SWDLInterface[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      34,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      17,       // signalCount

 // signals: signature, parameters, type, tag, flags
      35,   15,   14,   14, 0x05,
      81,   69,   14,   14, 0x05,
     118,  107,   14,   14, 0x05,
     169,  149,   14,   14, 0x05,
     221,  203,   14,   14, 0x05,
     267,  247,   14,   14, 0x05,
     315,  298,   14,   14, 0x05,
     364,  343,   14,   14, 0x05,
     420,  401,   14,   14, 0x05,
     474,  456,   14,   14, 0x05,
     522,  506,   14,   14, 0x05,
     572,  552,   14,   14, 0x05,
     637,  606,   14,   14, 0x05,
     686,  670,   14,   14, 0x05,
     732,  716,   14,   14, 0x05,
     782,  762,   14,   14, 0x05,
     821,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     873,   15,  849,   14, 0x0a,
     910,   69,  849,   14, 0x0a,
     939,  107,  849,   14, 0x0a,
     973,  149,  849,   14, 0x0a,
    1019, 1010,  849,   14, 0x0a,
    1060,  203,  849,   14, 0x0a,
    1115, 1089,  849,   14, 0x0a,
    1154,  298,  849,   14, 0x0a,
    1185,  343,  849,   14, 0x0a,
    1225,  401,  849,   14, 0x0a,
    1264,  456,  849,   14, 0x0a,
    1299,  506,  849,   14, 0x0a,
    1332,  552,  849,   14, 0x0a,
    1393, 1369,  849,   14, 0x0a,
    1424,  670,  849,   14, 0x0a,
    1457,  716,  849,   14, 0x0a,
    1503, 1490,  849,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SWDLInterface[] = {
    "SWDLInterface\0\0checkactivatemodule\0"
    "CheckActivateModule(QDBusVariant)\0"
    "eraseMemory\0EraseMemory(QDBusVariant)\0"
    "initUpdate\0InitUpdateResult(QDBusVariant)\0"
    "PrepareFileTransfer\0"
    "PrepareFileTransfer(QDBusVariant)\0"
    "swdlid,deviceName\0ReadDevName(uint,QString)\0"
    "swdlid,hwidentifier\0ReadHWIdentifier(uint,QString)\0"
    "swdlid,hwversion\0ReadHWVersion(uint,QString)\0"
    "swdlid,moduleversion\0"
    "ReadModuleVersion(uint,QDBusVariant)\0"
    "swdlid,modulevalue\0"
    "ReadNumberofModule(uint,QByteArray)\0"
    "reprogrammingmode\0ReprogrammingMode(QDBusVariant)\0"
    "requestdownload\0RequestDownload(QDBusVariant)\0"
    "requesttransferexit\0"
    "RequestTransferExit(QDBusVariant)\0"
    "swdlid,swdlOpMode,swdlProgress\0"
    "SWDLProgressInfo(uint,uint,uint)\0"
    "transferdataCMS\0TransferDataAMS(QDBusVariant)\0"
    "transferdataAMS\0TransferDataCMS(QDBusVariant)\0"
    "swdlid,updateStatus\0"
    "UpdateProgressingInfoStatus(uint,uint)\0"
    "iBoxSWDLFBlockVisible(bool)\0"
    "QDBusPendingReply<bool>\0"
    "SetCheckActivateModule(QDBusVariant)\0"
    "SetEraseMemory(QDBusVariant)\0"
    "SetInitUpdateResult(QDBusVariant)\0"
    "SetPrepareFileTransfer(QDBusVariant)\0"
    "fileInfo\0SetPrepareFileTransferIBOX(QDBusVariant)\0"
    "SetReadDevName(uint,QString)\0"
    "swdlid,state,hwidentifier\0"
    "SetReadHWIdentifier(uint,uint,QString)\0"
    "SetReadHWVersion(uint,QString)\0"
    "SetReadModuleVersion(uint,QDBusVariant)\0"
    "SetReadNumberofModule(uint,QByteArray)\0"
    "SetReprogrammingMode(QDBusVariant)\0"
    "SetRequestDownload(QDBusVariant)\0"
    "SetRequestTransferExit(QDBusVariant)\0"
    "swdlOpMode,swdlProgress\0"
    "SetSWDLProgressInfo(uint,uint)\0"
    "SetTransferDataAMS(QDBusVariant)\0"
    "SetTransferDataCMS(QDBusVariant)\0"
    "updateStatus\0SetUpdateProgressingInfo(uint)\0"
};

void SWDLInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SWDLInterface *_t = static_cast<SWDLInterface *>(_o);
        switch (_id) {
        case 0: _t->CheckActivateModule((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 1: _t->EraseMemory((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 2: _t->InitUpdateResult((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 3: _t->PrepareFileTransfer((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 4: _t->ReadDevName((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->ReadHWIdentifier((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: _t->ReadHWVersion((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: _t->ReadModuleVersion((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QDBusVariant(*)>(_a[2]))); break;
        case 8: _t->ReadNumberofModule((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 9: _t->ReprogrammingMode((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 10: _t->RequestDownload((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 11: _t->RequestTransferExit((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 12: _t->SWDLProgressInfo((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        case 13: _t->TransferDataAMS((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 14: _t->TransferDataCMS((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 15: _t->UpdateProgressingInfoStatus((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 16: _t->iBoxSWDLFBlockVisible((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 17: { QDBusPendingReply<bool> _r = _t->SetCheckActivateModule((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 18: { QDBusPendingReply<bool> _r = _t->SetEraseMemory((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 19: { QDBusPendingReply<bool> _r = _t->SetInitUpdateResult((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 20: { QDBusPendingReply<bool> _r = _t->SetPrepareFileTransfer((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 21: { QDBusPendingReply<bool> _r = _t->SetPrepareFileTransferIBOX((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 22: { QDBusPendingReply<bool> _r = _t->SetReadDevName((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 23: { QDBusPendingReply<bool> _r = _t->SetReadHWIdentifier((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 24: { QDBusPendingReply<bool> _r = _t->SetReadHWVersion((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 25: { QDBusPendingReply<bool> _r = _t->SetReadModuleVersion((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QDBusVariant(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 26: { QDBusPendingReply<bool> _r = _t->SetReadNumberofModule((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 27: { QDBusPendingReply<bool> _r = _t->SetReprogrammingMode((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 28: { QDBusPendingReply<bool> _r = _t->SetRequestDownload((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 29: { QDBusPendingReply<bool> _r = _t->SetRequestTransferExit((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 30: { QDBusPendingReply<bool> _r = _t->SetSWDLProgressInfo((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 31: { QDBusPendingReply<bool> _r = _t->SetTransferDataAMS((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 32: { QDBusPendingReply<bool> _r = _t->SetTransferDataCMS((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 33: { QDBusPendingReply<bool> _r = _t->SetUpdateProgressingInfo((*reinterpret_cast< uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SWDLInterface::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SWDLInterface::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_SWDLInterface,
      qt_meta_data_SWDLInterface, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SWDLInterface::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SWDLInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SWDLInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SWDLInterface))
        return static_cast<void*>(const_cast< SWDLInterface*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int SWDLInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 34)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 34;
    }
    return _id;
}

// SIGNAL 0
void SWDLInterface::CheckActivateModule(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SWDLInterface::EraseMemory(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SWDLInterface::InitUpdateResult(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SWDLInterface::PrepareFileTransfer(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SWDLInterface::ReadDevName(uint _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SWDLInterface::ReadHWIdentifier(uint _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void SWDLInterface::ReadHWVersion(uint _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void SWDLInterface::ReadModuleVersion(uint _t1, const QDBusVariant & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void SWDLInterface::ReadNumberofModule(uint _t1, const QByteArray & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void SWDLInterface::ReprogrammingMode(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void SWDLInterface::RequestDownload(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void SWDLInterface::RequestTransferExit(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void SWDLInterface::SWDLProgressInfo(uint _t1, uint _t2, uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void SWDLInterface::TransferDataAMS(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void SWDLInterface::TransferDataCMS(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void SWDLInterface::UpdateProgressingInfoStatus(uint _t1, uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void SWDLInterface::iBoxSWDLFBlockVisible(const bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}
QT_END_MOC_NAMESPACE
