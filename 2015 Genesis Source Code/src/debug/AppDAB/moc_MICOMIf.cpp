/****************************************************************************
** Meta object code from reading C++ file 'MICOMIf.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/MICOMIf.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MICOMIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MICOMIf[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      43,   39,    9,    8, 0x0a,
      76,   39,    9,    8, 0x0a,
     107,   39,    9,    8, 0x0a,
     157,   39,  137,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MICOMIf[] = {
    "MICOMIf\0\0QDBusPendingReply<QByteArray>\0"
    "in0\0LgeDBus_CANTPSendMsg(QByteArray)\0"
    "LgeDBus_DTCClearRequest(uchar)\0"
    "LgeDBus_DTCReadRequest(uchar)\0"
    "QDBusPendingReply<>\0LgeDBus_DTCSendMsg(QByteArray)\0"
};

void MICOMIf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MICOMIf *_t = static_cast<MICOMIf *>(_o);
        switch (_id) {
        case 0: { QDBusPendingReply<QByteArray> _r = _t->LgeDBus_CANTPSendMsg((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<QByteArray>*>(_a[0]) = _r; }  break;
        case 1: { QDBusPendingReply<QByteArray> _r = _t->LgeDBus_DTCClearRequest((*reinterpret_cast< uchar(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<QByteArray>*>(_a[0]) = _r; }  break;
        case 2: { QDBusPendingReply<QByteArray> _r = _t->LgeDBus_DTCReadRequest((*reinterpret_cast< uchar(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<QByteArray>*>(_a[0]) = _r; }  break;
        case 3: { QDBusPendingReply<> _r = _t->LgeDBus_DTCSendMsg((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MICOMIf::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MICOMIf::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_MICOMIf,
      qt_meta_data_MICOMIf, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MICOMIf::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MICOMIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MICOMIf::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MICOMIf))
        return static_cast<void*>(const_cast< MICOMIf*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int MICOMIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
