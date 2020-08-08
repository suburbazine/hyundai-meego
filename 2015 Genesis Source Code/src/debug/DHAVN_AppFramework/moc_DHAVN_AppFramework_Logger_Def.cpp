/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFramework_Logger_Def.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFramework_Logger_Def.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFramework_Logger_Def.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Logger[] = {

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
      27,    8,    7,    7, 0x02,
      62,   53,    7,    7, 0x22,

 // enums: name, flags, count, data
      75, 0x0,   10,   28,

 // enum data: key, value
      88, uint(Logger::PRIORITY_LOW),
     101, uint(Logger::PRIORITY_MEDIUM),
     117, uint(Logger::PRIORITY_HI),
     129, uint(Logger::PRIORITY_CRITICAL),
     147, uint(Logger::PRIORITY_ASSERT),
     163, uint(Logger::PRIORITY_TRACE),
     178, uint(Logger::PRIORITY_INFO),
     192, uint(Logger::PRIORITY_SIGNAL),
     208, uint(Logger::PRIORITY_SLOT),
     222, uint(Logger::PRIORITY_TRANSITION),

       0        // eod
};

static const char qt_meta_stringdata_Logger[] = {
    "Logger\0\0aMessage,aPriority\0"
    "Log(QString,LOG_PRIORITY)\0aMessage\0"
    "Log(QString)\0LOG_PRIORITY\0PRIORITY_LOW\0"
    "PRIORITY_MEDIUM\0PRIORITY_HI\0"
    "PRIORITY_CRITICAL\0PRIORITY_ASSERT\0"
    "PRIORITY_TRACE\0PRIORITY_INFO\0"
    "PRIORITY_SIGNAL\0PRIORITY_SLOT\0"
    "PRIORITY_TRANSITION\0"
};

void Logger::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Logger *_t = static_cast<Logger *>(_o);
        switch (_id) {
        case 0: _t->Log((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< LOG_PRIORITY(*)>(_a[2]))); break;
        case 1: _t->Log((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Logger::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Logger::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Logger,
      qt_meta_data_Logger, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Logger::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Logger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Logger::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Logger))
        return static_cast<void*>(const_cast< Logger*>(this));
    return QObject::qt_metacast(_clname);
}

int Logger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
