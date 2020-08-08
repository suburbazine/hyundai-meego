/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFileManager_Logger.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFileManager_Logger.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFileManager_Logger.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LoggerEx[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       1,   24, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      29,   10,    9,    9, 0x02,
      64,   55,    9,    9, 0x22,

 // enums: name, flags, count, data
      77, 0x0,   10,   28,

 // enum data: key, value
      90, uint(LoggerEx::PRIORITY_LOW),
     103, uint(LoggerEx::PRIORITY_MEDIUM),
     119, uint(LoggerEx::PRIORITY_HI),
     131, uint(LoggerEx::PRIORITY_CRITICAL),
     149, uint(LoggerEx::PRIORITY_ASSERT),
     165, uint(LoggerEx::PRIORITY_TRACE),
     180, uint(LoggerEx::PRIORITY_INFO),
     194, uint(LoggerEx::PRIORITY_SIGNAL),
     210, uint(LoggerEx::PRIORITY_SLOT),
     224, uint(LoggerEx::PRIORITY_TRANSITION),

       0        // eod
};

static const char qt_meta_stringdata_LoggerEx[] = {
    "LoggerEx\0\0aMessage,aPriority\0"
    "Log(QString,LOG_PRIORITY)\0aMessage\0"
    "Log(QString)\0LOG_PRIORITY\0PRIORITY_LOW\0"
    "PRIORITY_MEDIUM\0PRIORITY_HI\0"
    "PRIORITY_CRITICAL\0PRIORITY_ASSERT\0"
    "PRIORITY_TRACE\0PRIORITY_INFO\0"
    "PRIORITY_SIGNAL\0PRIORITY_SLOT\0"
    "PRIORITY_TRANSITION\0"
};

void LoggerEx::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LoggerEx *_t = static_cast<LoggerEx *>(_o);
        switch (_id) {
        case 0: _t->Log((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< LOG_PRIORITY(*)>(_a[2]))); break;
        case 1: _t->Log((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LoggerEx::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LoggerEx::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LoggerEx,
      qt_meta_data_LoggerEx, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LoggerEx::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LoggerEx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LoggerEx::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LoggerEx))
        return static_cast<void*>(const_cast< LoggerEx*>(this));
    return QObject::qt_metacast(_clname);
}

int LoggerEx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
