/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_QCANController_ApplnTxRx.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_QCANController_ApplnTxRx.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_QCANController_ApplnTxRx.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CQCANController_ApplnTxRx[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   27,   26,   26, 0x05,
      65,   27,   26,   26, 0x05,
      95,   27,   26,   26, 0x05,
     143,  121,   26,   26, 0x05,
     198,  187,   26,   26, 0x05,
     230,   27,   26,   26, 0x05,
     264,   26,   26,   26, 0x05,
     280,   26,   26,   26, 0x05,

 // slots: signature, parameters, type, tag, flags
     313,  301,   26,   26, 0x0a,
     346,  121,   26,   26, 0x0a,
     405,  389,   26,   26, 0x0a,
     448,  187,   26,   26, 0x0a,
     481,   26,   26,   26, 0x08,
     494,   26,   26,   26, 0x08,
     524,  512,   26,   26, 0x08,
     577,  560,   26,   26, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CQCANController_ApplnTxRx[] = {
    "CQCANController_ApplnTxRx\0\0c_data\0"
    "Can_filter_msg_id(SQCanIdList)\0"
    "Can_filter_bcm_msg(SQBCMHead)\0"
    "Can_data_send(SQCanFrame)\0"
    "c_bcm_data,c_can_data\0"
    "Can_data_send_bcm(SQBCMHead,SQCanFrameList)\0"
    "c_bcm_data\0Can_data_receive_bcm(SQBCMHead)\0"
    "Can_data_received(SQCanFrameList)\0"
    "Filter_can_id()\0Filter_can_bcm_msg()\0"
    "c_send_data\0Send_data_to_can_raw(SQCanFrame)\0"
    "Send_data_to_bcm(SQBCMHead,SQCanFrameList)\0"
    "c_received_data\0"
    "Received_data_from_can_raw(SQCanFrameList)\0"
    "Receive_data_from_bcm(SQBCMHead)\0"
    "Get_can_id()\0Get_can_bcm_msg()\0"
    "c_filter_id\0Received_can_filter_id(SQCanIdList)\0"
    "c_filter_bcm_msg\0"
    "Received_can_filter_bcm_msg(SQBCMHead)\0"
};

void CQCANController_ApplnTxRx::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CQCANController_ApplnTxRx *_t = static_cast<CQCANController_ApplnTxRx *>(_o);
        switch (_id) {
        case 0: _t->Can_filter_msg_id((*reinterpret_cast< const SQCanIdList(*)>(_a[1]))); break;
        case 1: _t->Can_filter_bcm_msg((*reinterpret_cast< const SQBCMHead(*)>(_a[1]))); break;
        case 2: _t->Can_data_send((*reinterpret_cast< const SQCanFrame(*)>(_a[1]))); break;
        case 3: _t->Can_data_send_bcm((*reinterpret_cast< const SQBCMHead(*)>(_a[1])),(*reinterpret_cast< const SQCanFrameList(*)>(_a[2]))); break;
        case 4: _t->Can_data_receive_bcm((*reinterpret_cast< const SQBCMHead(*)>(_a[1]))); break;
        case 5: _t->Can_data_received((*reinterpret_cast< const SQCanFrameList(*)>(_a[1]))); break;
        case 6: _t->Filter_can_id(); break;
        case 7: _t->Filter_can_bcm_msg(); break;
        case 8: _t->Send_data_to_can_raw((*reinterpret_cast< const SQCanFrame(*)>(_a[1]))); break;
        case 9: _t->Send_data_to_bcm((*reinterpret_cast< const SQBCMHead(*)>(_a[1])),(*reinterpret_cast< const SQCanFrameList(*)>(_a[2]))); break;
        case 10: _t->Received_data_from_can_raw((*reinterpret_cast< const SQCanFrameList(*)>(_a[1]))); break;
        case 11: _t->Receive_data_from_bcm((*reinterpret_cast< const SQBCMHead(*)>(_a[1]))); break;
        case 12: _t->Get_can_id(); break;
        case 13: _t->Get_can_bcm_msg(); break;
        case 14: _t->Received_can_filter_id((*reinterpret_cast< const SQCanIdList(*)>(_a[1]))); break;
        case 15: _t->Received_can_filter_bcm_msg((*reinterpret_cast< const SQBCMHead(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CQCANController_ApplnTxRx::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CQCANController_ApplnTxRx::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CQCANController_ApplnTxRx,
      qt_meta_data_CQCANController_ApplnTxRx, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CQCANController_ApplnTxRx::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CQCANController_ApplnTxRx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CQCANController_ApplnTxRx::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQCANController_ApplnTxRx))
        return static_cast<void*>(const_cast< CQCANController_ApplnTxRx*>(this));
    return QObject::qt_metacast(_clname);
}

int CQCANController_ApplnTxRx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CQCANController_ApplnTxRx::Can_filter_msg_id(const SQCanIdList & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CQCANController_ApplnTxRx::Can_filter_bcm_msg(const SQBCMHead & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CQCANController_ApplnTxRx::Can_data_send(const SQCanFrame & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CQCANController_ApplnTxRx::Can_data_send_bcm(const SQBCMHead & _t1, const SQCanFrameList & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CQCANController_ApplnTxRx::Can_data_receive_bcm(const SQBCMHead & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CQCANController_ApplnTxRx::Can_data_received(const SQCanFrameList & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CQCANController_ApplnTxRx::Filter_can_id()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void CQCANController_ApplnTxRx::Filter_can_bcm_msg()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}
QT_END_MOC_NAMESPACE
