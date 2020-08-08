/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_PndrFrameDecoder.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_PndrFrameDecoder.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_PndrFrameDecoder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPndrFrameDecoder[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      16,       // signalCount

 // signals: signature, parameters, type, tag, flags
      36,   19,   18,   18, 0x05,
      69,   19,   18,   18, 0x05,
     101,   19,   18,   18, 0x05,
     135,   19,   18,   18, 0x05,
     203,  169,   18,   18, 0x05,
     283,  257,   18,   18, 0x05,
     342,  324,   18,   18, 0x05,
     376,   19,   18,   18, 0x05,
     414,  257,   18,   18, 0x05,
     463,  257,   18,   18, 0x05,
     535,  511,   18,   18, 0x05,
     628,  582,   18,   18, 0x05,
     747,  687,   18,   18, 0x05,
     814,   18,   18,   18, 0x05,
     831,   18,   18,   18, 0x05,
     860,  850,   18,   18, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CPndrFrameDecoder[] = {
    "CPndrFrameDecoder\0\0inRequest,inData\0"
    "ReplyReceived(EPndrEvent,quint8)\0"
    "ReplyReceived(EPndrEvent,qint8)\0"
    "ReplyReceived(EPndrEvent,quint16)\0"
    "ReplyReceived(EPndrEvent,QString)\0"
    "inRequest,inSearchId,inSearchList\0"
    "ReplyReceived(EPndrEvent,quint32,QList<TSSearchInfo>)\0"
    "inRequest,inData1,inData2\0"
    "ReplyReceived(EPndrEvent,quint32,quint8)\0"
    "inRequest,inData1\0ReplyReceived(EPndrEvent,quint32)\0"
    "ReplyReceived(EPndrEvent,TSTrackInfo)\0"
    "ReplyReceived(EPndrEvent,quint32,QList<quint32>)\0"
    "ReplyReceived(EPndrEvent,quint8,QList<quint32>)\0"
    "inRequest,inStationList\0"
    "ReplyReceived(EPndrEvent,QList<TSStationInfo>)\0"
    "inRequest,inTrackToken,segIndex,totalSeg,data\0"
    "ReplyReceived(EPndrEvent,quint32,quint8,quint8,QByteArray)\0"
    "inRequest,inStationToken,inArtLength,segIndex,totalSeg,data\0"
    "ReplyReceived(EPndrEvent,quint32,quint32,quint8,quint8,QByteArray)\0"
    "ReceivedStatus()\0ReceivedBranding()\0"
    "inErrCode\0ErrorReceived(EPndrError)\0"
};

void CPndrFrameDecoder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CPndrFrameDecoder *_t = static_cast<CPndrFrameDecoder *>(_o);
        switch (_id) {
        case 0: _t->ReplyReceived((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint8(*)>(_a[2]))); break;
        case 1: _t->ReplyReceived((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const qint8(*)>(_a[2]))); break;
        case 2: _t->ReplyReceived((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint16(*)>(_a[2]))); break;
        case 3: _t->ReplyReceived((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->ReplyReceived((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2])),(*reinterpret_cast< const QList<TSSearchInfo>(*)>(_a[3]))); break;
        case 5: _t->ReplyReceived((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2])),(*reinterpret_cast< const quint8(*)>(_a[3]))); break;
        case 6: _t->ReplyReceived((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2]))); break;
        case 7: _t->ReplyReceived((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const TSTrackInfo(*)>(_a[2]))); break;
        case 8: _t->ReplyReceived((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2])),(*reinterpret_cast< const QList<quint32>(*)>(_a[3]))); break;
        case 9: _t->ReplyReceived((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint8(*)>(_a[2])),(*reinterpret_cast< const QList<quint32>(*)>(_a[3]))); break;
        case 10: _t->ReplyReceived((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const QList<TSStationInfo>(*)>(_a[2]))); break;
        case 11: _t->ReplyReceived((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2])),(*reinterpret_cast< const quint8(*)>(_a[3])),(*reinterpret_cast< const quint8(*)>(_a[4])),(*reinterpret_cast< const QByteArray(*)>(_a[5]))); break;
        case 12: _t->ReplyReceived((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2])),(*reinterpret_cast< const quint32(*)>(_a[3])),(*reinterpret_cast< const quint8(*)>(_a[4])),(*reinterpret_cast< const quint8(*)>(_a[5])),(*reinterpret_cast< const QByteArray(*)>(_a[6]))); break;
        case 13: _t->ReceivedStatus(); break;
        case 14: _t->ReceivedBranding(); break;
        case 15: _t->ErrorReceived((*reinterpret_cast< const EPndrError(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CPndrFrameDecoder::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CPndrFrameDecoder::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CPndrFrameDecoder,
      qt_meta_data_CPndrFrameDecoder, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CPndrFrameDecoder::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CPndrFrameDecoder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CPndrFrameDecoder::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPndrFrameDecoder))
        return static_cast<void*>(const_cast< CPndrFrameDecoder*>(this));
    return QObject::qt_metacast(_clname);
}

int CPndrFrameDecoder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void CPndrFrameDecoder::ReplyReceived(const EPndrEvent _t1, const quint8 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CPndrFrameDecoder::ReplyReceived(const EPndrEvent _t1, const qint8 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CPndrFrameDecoder::ReplyReceived(const EPndrEvent _t1, const quint16 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CPndrFrameDecoder::ReplyReceived(const EPndrEvent _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CPndrFrameDecoder::ReplyReceived(const EPndrEvent _t1, const quint32 _t2, const QList<TSSearchInfo> & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CPndrFrameDecoder::ReplyReceived(const EPndrEvent _t1, const quint32 _t2, const quint8 _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CPndrFrameDecoder::ReplyReceived(const EPndrEvent _t1, const quint32 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CPndrFrameDecoder::ReplyReceived(const EPndrEvent _t1, const TSTrackInfo & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CPndrFrameDecoder::ReplyReceived(const EPndrEvent _t1, const quint32 _t2, const QList<quint32> & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CPndrFrameDecoder::ReplyReceived(const EPndrEvent _t1, const quint8 _t2, const QList<quint32> & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CPndrFrameDecoder::ReplyReceived(const EPndrEvent _t1, const QList<TSStationInfo> & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CPndrFrameDecoder::ReplyReceived(const EPndrEvent _t1, const quint32 _t2, const quint8 _t3, const quint8 _t4, const QByteArray & _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CPndrFrameDecoder::ReplyReceived(const EPndrEvent _t1, const quint32 _t2, const quint32 _t3, const quint8 _t4, const quint8 _t5, const QByteArray & _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CPndrFrameDecoder::ReceivedStatus()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void CPndrFrameDecoder::ReceivedBranding()
{
    QMetaObject::activate(this, &staticMetaObject, 14, 0);
}

// SIGNAL 15
void CPndrFrameDecoder::ErrorReceived(const EPndrError _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}
QT_END_MOC_NAMESPACE
