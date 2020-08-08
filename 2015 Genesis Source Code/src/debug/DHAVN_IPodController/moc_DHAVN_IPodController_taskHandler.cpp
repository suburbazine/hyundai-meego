/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_taskHandler.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TaskHandlers/include/DHAVN_IPodController_taskHandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_taskHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CTaskHandler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      74,   52,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CTaskHandler[] = {
    "CTaskHandler\0\0packetBytes\0"
    "sendiAPPacket(QByteArray)\0"
    "lingoID,commandID,buf\0"
    "packetAvailable(int,int,QByteArray)\0"
};

void CTaskHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CTaskHandler *_t = static_cast<CTaskHandler *>(_o);
        switch (_id) {
        case 0: _t->sendiAPPacket((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 1: _t->packetAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CTaskHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CTaskHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CTaskHandler,
      qt_meta_data_CTaskHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CTaskHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CTaskHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CTaskHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTaskHandler))
        return static_cast<void*>(const_cast< CTaskHandler*>(this));
    return QObject::qt_metacast(_clname);
}

int CTaskHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void CTaskHandler::sendiAPPacket(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
