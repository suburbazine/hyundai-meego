/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_QwertyKeypad_ChineseHWR_Canvas.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_QwertyKeypad_ChineseHWR_Canvas.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_QwertyKeypad_ChineseHWR_Canvas.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QwertyKeypadChineseHWRCanvas[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      43,   30,   29,   29, 0x05,

 // slots: signature, parameters, type, tag, flags
      66,   30,   29,   29, 0x0a,

 // methods: signature, parameters, type, tag, flags
      88,   29,   29,   29, 0x02,
      98,   29,   29,   29, 0x02,
     111,   29,   29,   29, 0x02,
     139,  123,   29,   29, 0x02,
     162,  123,   29,   29, 0x02,
     182,   29,   29,   29, 0x02,
     198,   29,   29,   29, 0x02,
     211,  123,   29,   29, 0x02,
     252,  245,  237,   29, 0x02,
     274,   29,  270,   29, 0x02,
     293,   29,   29,   29, 0x02,
     311,   29,   29,   29, 0x02,
     338,  327,  322,   29, 0x02,
     365,   29,  270,   29, 0x02,
     384,   29,  270,   29, 0x02,
     404,   30,   29,   29, 0x02,
     431,   30,   29,   29, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_QwertyKeypadChineseHWRCanvas[] = {
    "QwertyKeypadChineseHWRCanvas\0\0"
    "isPrediction\0candidateUpdated(bool)\0"
    "updateCandidate(bool)\0initHWR()\0"
    "releaseHWR()\0initBtHWR()\0nMouseX,nMouseY\0"
    "sendLastPoint(int,int)\0drawLineTo(int,int)\0"
    "canvasRelease()\0clearImage()\0"
    "getDrawingPoints(int,int)\0QString\0"
    "nIndex\0getCandidate(int)\0int\0"
    "getCandidateSize()\0deleteCandidate()\0"
    "startHWR()\0bool\0searchWord\0"
    "getDKBDPrediction(QString)\0"
    "getTotalPageSize()\0getCurrentPageNum()\0"
    "incrementCurrentPage(bool)\0"
    "decrementCurrentPage(bool)\0"
};

void QwertyKeypadChineseHWRCanvas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QwertyKeypadChineseHWRCanvas *_t = static_cast<QwertyKeypadChineseHWRCanvas *>(_o);
        switch (_id) {
        case 0: _t->candidateUpdated((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->updateCandidate((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->initHWR(); break;
        case 3: _t->releaseHWR(); break;
        case 4: _t->initBtHWR(); break;
        case 5: _t->sendLastPoint((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->drawLineTo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->canvasRelease(); break;
        case 8: _t->clearImage(); break;
        case 9: _t->getDrawingPoints((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: { QString _r = _t->getCandidate((*reinterpret_cast< const int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 11: { int _r = _t->getCandidateSize();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 12: _t->deleteCandidate(); break;
        case 13: _t->startHWR(); break;
        case 14: { bool _r = _t->getDKBDPrediction((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 15: { int _r = _t->getTotalPageSize();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 16: { int _r = _t->getCurrentPageNum();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 17: _t->incrementCurrentPage((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: _t->decrementCurrentPage((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QwertyKeypadChineseHWRCanvas::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QwertyKeypadChineseHWRCanvas::staticMetaObject = {
    { &QDeclarativeItem::staticMetaObject, qt_meta_stringdata_QwertyKeypadChineseHWRCanvas,
      qt_meta_data_QwertyKeypadChineseHWRCanvas, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QwertyKeypadChineseHWRCanvas::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QwertyKeypadChineseHWRCanvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QwertyKeypadChineseHWRCanvas::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwertyKeypadChineseHWRCanvas))
        return static_cast<void*>(const_cast< QwertyKeypadChineseHWRCanvas*>(this));
    return QDeclarativeItem::qt_metacast(_clname);
}

int QwertyKeypadChineseHWRCanvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDeclarativeItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void QwertyKeypadChineseHWRCanvas::candidateUpdated(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
