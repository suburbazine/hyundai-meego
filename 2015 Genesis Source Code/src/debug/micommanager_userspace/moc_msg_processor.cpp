/****************************************************************************
** Meta object code from reading C++ file 'msg_processor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/msg_processor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'msg_processor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CMsgProcessor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      48,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CMsgProcessor[] = {
    "CMsgProcessor\0\0parseIncomingSpiData(QByteArray)\0"
    "outputDebug(QByteArray)\0"
};

void CMsgProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CMsgProcessor *_t = static_cast<CMsgProcessor *>(_o);
        switch (_id) {
        case 0: _t->parseIncomingSpiData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->outputDebug((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CMsgProcessor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CMsgProcessor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CMsgProcessor,
      qt_meta_data_CMsgProcessor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CMsgProcessor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CMsgProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CMsgProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CMsgProcessor))
        return static_cast<void*>(const_cast< CMsgProcessor*>(this));
    return QObject::qt_metacast(_clname);
}

int CMsgProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
