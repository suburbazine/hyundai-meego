/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_EventLoopTimerExitCode.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TaskHandlers/include/DHAVN_EventLoopTimerExitCode.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_EventLoopTimerExitCode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CEventloopTimerExitCode[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   24,   24,   24, 0x0a,
      62,   60,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CEventloopTimerExitCode[] = {
    "CEventloopTimerExitCode\0\0eventloopTimeout()\0"
    "wakeUpAndEmit()\0,\0getListComplete(QVariantMap,int)\0"
};

void CEventloopTimerExitCode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CEventloopTimerExitCode *_t = static_cast<CEventloopTimerExitCode *>(_o);
        switch (_id) {
        case 0: _t->eventloopTimeout(); break;
        case 1: _t->wakeUpAndEmit(); break;
        case 2: _t->getListComplete((*reinterpret_cast< QVariantMap(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CEventloopTimerExitCode::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CEventloopTimerExitCode::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CEventloopTimerExitCode,
      qt_meta_data_CEventloopTimerExitCode, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CEventloopTimerExitCode::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CEventloopTimerExitCode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CEventloopTimerExitCode::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CEventloopTimerExitCode))
        return static_cast<void*>(const_cast< CEventloopTimerExitCode*>(this));
    return QObject::qt_metacast(_clname);
}

int CEventloopTimerExitCode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CEventloopTimerExitCode::eventloopTimeout()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
