/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_QwertyKeypad_ChineseHWR.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_QwertyKeypad_ChineseHWR.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_QwertyKeypad_ChineseHWR.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QwertyKeypadChineseHWR[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      37,   24,   23,   23, 0x05,
      59,   23,   23,   23, 0x05,

 // methods: signature, parameters, type, tag, flags
      93,   86,   78,   23, 0x02,
     115,   23,  111,   23, 0x02,
     134,   23,   23,   23, 0x02,
     152,   23,  111,   23, 0x02,
     171,   23,  111,   23, 0x02,
     207,  196,  191,   23, 0x02,
     234,   24,   23,   23, 0x02,
     261,   24,   23,   23, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_QwertyKeypadChineseHWR[] = {
    "QwertyKeypadChineseHWR\0\0isPrediction\0"
    "candidateUpdate(bool)\0predictionUpdate()\0"
    "QString\0nIndex\0getCandidate(int)\0int\0"
    "getCandidateSize()\0deleteCandidate()\0"
    "getTotalPageSize()\0getCurrentPageNum()\0"
    "bool\0searchWord\0getDKBDPrediction(QString)\0"
    "incrementCurrentPage(bool)\0"
    "decrementCurrentPage(bool)\0"
};

void QwertyKeypadChineseHWR::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QwertyKeypadChineseHWR *_t = static_cast<QwertyKeypadChineseHWR *>(_o);
        switch (_id) {
        case 0: _t->candidateUpdate((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->predictionUpdate(); break;
        case 2: { QString _r = _t->getCandidate((*reinterpret_cast< const int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->getCandidateSize();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: _t->deleteCandidate(); break;
        case 5: { int _r = _t->getTotalPageSize();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 6: { int _r = _t->getCurrentPageNum();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 7: { bool _r = _t->getDKBDPrediction((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: _t->incrementCurrentPage((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->decrementCurrentPage((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QwertyKeypadChineseHWR::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QwertyKeypadChineseHWR::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QwertyKeypadChineseHWR,
      qt_meta_data_QwertyKeypadChineseHWR, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QwertyKeypadChineseHWR::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QwertyKeypadChineseHWR::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QwertyKeypadChineseHWR::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwertyKeypadChineseHWR))
        return static_cast<void*>(const_cast< QwertyKeypadChineseHWR*>(this));
    return QObject::qt_metacast(_clname);
}

int QwertyKeypadChineseHWR::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void QwertyKeypadChineseHWR::candidateUpdate(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QwertyKeypadChineseHWR::predictionUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
