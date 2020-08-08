/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_QCANController_CANTxRx.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_QCANController_CANTxRx.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_QCANController_CANTxRx.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CQCANController_CANTxRx[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      32,   25,   24,   24, 0x05,
      66,   24,   24,   24, 0x05,
      82,   24,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
     103,   25,   24,   24, 0x0a,
     147,  129,   24,   24, 0x0a,
     202,  191,   24,   24, 0x0a,
     234,   25,   24,   24, 0x0a,
     267,   24,   24,   24, 0x0a,
     280,   24,   24,   24, 0x0a,
     312,  298,   24,   24, 0x0a,
     343,  191,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CQCANController_CANTxRx[] = {
    "CQCANController_CANTxRx\0\0c_data\0"
    "Can_data_received(SQCanFrameList)\0"
    "Filter_can_id()\0Filter_can_bcm_msg()\0"
    "Can_send_data(SQCanFrame)\0c_bcm_data,c_data\0"
    "Can_send_bcm_data(SQBCMHead,SQCanFrameList)\0"
    "c_bcm_data\0Can_receive_bcm_data(SQBCMHead)\0"
    "Can_receive_data(SQCanFrameList)\0"
    "Get_can_id()\0Get_can_bcm_msg()\0"
    "c_can_id_list\0Can_read_data_raw(SQCanIdList)\0"
    "Can_read_bcm_data_raw(SQBCMHead)\0"
};

void CQCANController_CANTxRx::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CQCANController_CANTxRx *_t = static_cast<CQCANController_CANTxRx *>(_o);
        switch (_id) {
        case 0: _t->Can_data_received((*reinterpret_cast< const SQCanFrameList(*)>(_a[1]))); break;
        case 1: _t->Filter_can_id(); break;
        case 2: _t->Filter_can_bcm_msg(); break;
        case 3: _t->Can_send_data((*reinterpret_cast< const SQCanFrame(*)>(_a[1]))); break;
        case 4: _t->Can_send_bcm_data((*reinterpret_cast< const SQBCMHead(*)>(_a[1])),(*reinterpret_cast< const SQCanFrameList(*)>(_a[2]))); break;
        case 5: _t->Can_receive_bcm_data((*reinterpret_cast< const SQBCMHead(*)>(_a[1]))); break;
        case 6: _t->Can_receive_data((*reinterpret_cast< const SQCanFrameList(*)>(_a[1]))); break;
        case 7: _t->Get_can_id(); break;
        case 8: _t->Get_can_bcm_msg(); break;
        case 9: _t->Can_read_data_raw((*reinterpret_cast< const SQCanIdList(*)>(_a[1]))); break;
        case 10: _t->Can_read_bcm_data_raw((*reinterpret_cast< const SQBCMHead(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CQCANController_CANTxRx::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CQCANController_CANTxRx::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CQCANController_CANTxRx,
      qt_meta_data_CQCANController_CANTxRx, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CQCANController_CANTxRx::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CQCANController_CANTxRx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CQCANController_CANTxRx::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQCANController_CANTxRx))
        return static_cast<void*>(const_cast< CQCANController_CANTxRx*>(this));
    return QObject::qt_metacast(_clname);
}

int CQCANController_CANTxRx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CQCANController_CANTxRx::Can_data_received(const SQCanFrameList & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CQCANController_CANTxRx::Filter_can_id()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CQCANController_CANTxRx::Filter_can_bcm_msg()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
