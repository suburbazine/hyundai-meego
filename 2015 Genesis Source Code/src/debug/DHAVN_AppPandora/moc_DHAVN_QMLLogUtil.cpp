/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_QMLLogUtil.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_QMLLogUtil.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_QMLLogUtil.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CQMLLogUtil[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       1,   19, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      31,   13,   12,   12, 0x02,

 // enums: name, flags, count, data
      51, 0x0,    9,   23,

 // enum data: key, value
      60, uint(CQMLLogUtil::LOW_LOG),
      68, uint(CQMLLogUtil::TRAC_LOG),
      77, uint(CQMLLogUtil::MEDI_LOG),
      86, uint(CQMLLogUtil::INFO_LOG),
      95, uint(CQMLLogUtil::HIGH_LOG),
     104, uint(CQMLLogUtil::SIGN_LOG),
     113, uint(CQMLLogUtil::SLOT_LOG),
     122, uint(CQMLLogUtil::TRAN_LOG),
     131, uint(CQMLLogUtil::CRIT_LOG),

       0        // eod
};

static const char qt_meta_stringdata_CQMLLogUtil[] = {
    "CQMLLogUtil\0\0inMessage,inLevel\0"
    "log(QString,quint8)\0LogSysID\0LOW_LOG\0"
    "TRAC_LOG\0MEDI_LOG\0INFO_LOG\0HIGH_LOG\0"
    "SIGN_LOG\0SLOT_LOG\0TRAN_LOG\0CRIT_LOG\0"
};

void CQMLLogUtil::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CQMLLogUtil *_t = static_cast<CQMLLogUtil *>(_o);
        switch (_id) {
        case 0: _t->log((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CQMLLogUtil::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CQMLLogUtil::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CQMLLogUtil,
      qt_meta_data_CQMLLogUtil, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CQMLLogUtil::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CQMLLogUtil::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CQMLLogUtil::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQMLLogUtil))
        return static_cast<void*>(const_cast< CQMLLogUtil*>(this));
    return QObject::qt_metacast(_clname);
}

int CQMLLogUtil::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
