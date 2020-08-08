/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_PndrStationController.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_PndrStationController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_PndrStationController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPndrStationController[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      41,   24,   23,   23, 0x05,
      74,   23,   23,   23, 0x05,
      93,   23,   23,   23, 0x05,
     127,  112,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
     191,  155,   23,   23, 0x0a,
     271,  245,   23,   23, 0x0a,
     336,  318,   23,   23, 0x0a,
     376,   24,   23,   23, 0x0a,
     490,  415,   23,   23, 0x0a,
     563,   23,   23,   23, 0x0a,
     578,   23,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CPndrStationController[] = {
    "CPndrStationController\0\0inRequest,inData\0"
    "ReplyReceived(EPndrEvent,quint8)\0"
    "ReceivedBranding()\0getBrandingImage()\0"
    "inStationToken\0StationArtReceived(quint32)\0"
    "inRequest,inStart_index,inTokenList\0"
    "OnRequestCompletion(EPndrEvent,quint8,QList<quint32>)\0"
    "inRequest,inData1,inData2\0"
    "OnRequestCompletion(EPndrEvent,quint32,quint8)\0"
    "inRequest,inData1\0"
    "OnRequestCompletion(EPndrEvent,quint32)\0"
    "OnRequestCompletion(EPndrEvent,quint8)\0"
    "inRequest,inStationToken,inArtLength,inSegmentindex,inTotalsegments,in"
    "Data\0"
    "OnRequestCompletion(EPndrEvent,quint32,quint32,quint8,quint8,QByteArra"
    "y)\0"
    "RequestStArt()\0OnRequestbranding()\0"
};

void CPndrStationController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CPndrStationController *_t = static_cast<CPndrStationController *>(_o);
        switch (_id) {
        case 0: _t->ReplyReceived((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint8(*)>(_a[2]))); break;
        case 1: _t->ReceivedBranding(); break;
        case 2: _t->getBrandingImage(); break;
        case 3: _t->StationArtReceived((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 4: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint8(*)>(_a[2])),(*reinterpret_cast< const QList<quint32>(*)>(_a[3]))); break;
        case 5: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2])),(*reinterpret_cast< const quint8(*)>(_a[3]))); break;
        case 6: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2]))); break;
        case 7: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint8(*)>(_a[2]))); break;
        case 8: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2])),(*reinterpret_cast< const quint32(*)>(_a[3])),(*reinterpret_cast< const quint8(*)>(_a[4])),(*reinterpret_cast< const quint8(*)>(_a[5])),(*reinterpret_cast< const QByteArray(*)>(_a[6]))); break;
        case 9: _t->RequestStArt(); break;
        case 10: _t->OnRequestbranding(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CPndrStationController::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CPndrStationController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CPndrStationController,
      qt_meta_data_CPndrStationController, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CPndrStationController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CPndrStationController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CPndrStationController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPndrStationController))
        return static_cast<void*>(const_cast< CPndrStationController*>(this));
    return QObject::qt_metacast(_clname);
}

int CPndrStationController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void CPndrStationController::ReplyReceived(const EPndrEvent _t1, const quint8 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CPndrStationController::ReceivedBranding()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CPndrStationController::getBrandingImage()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void CPndrStationController::StationArtReceived(quint32 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
