/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_Identifier_NonIDPS.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TaskHandlers/Identifier/include/DHAVN_IPodController_Identifier_NonIDPS.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_Identifier_NonIDPS.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CIdentifierNonIDPSTaskHandler[] = {

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
      53,   31,   30,   30, 0x0a,
      89,   30,   30,   30, 0x0a,
     113,   30,   30,   30, 0x0a,
     140,   30,   30,   30, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CIdentifierNonIDPSTaskHandler[] = {
    "CIdentifierNonIDPSTaskHandler\0\0"
    "lingoID,commandID,buf\0"
    "packetAvailable(int,int,QByteArray)\0"
    "identify_restart_slot()\0"
    "identifyDeviceLingoReset()\0"
    "identifyDeviceLingo()\0"
};

void CIdentifierNonIDPSTaskHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CIdentifierNonIDPSTaskHandler *_t = static_cast<CIdentifierNonIDPSTaskHandler *>(_o);
        switch (_id) {
        case 0: _t->packetAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 1: _t->identify_restart_slot(); break;
        case 2: _t->identifyDeviceLingoReset(); break;
        case 3: _t->identifyDeviceLingo(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CIdentifierNonIDPSTaskHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CIdentifierNonIDPSTaskHandler::staticMetaObject = {
    { &CIdentifierTaskHandler::staticMetaObject, qt_meta_stringdata_CIdentifierNonIDPSTaskHandler,
      qt_meta_data_CIdentifierNonIDPSTaskHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CIdentifierNonIDPSTaskHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CIdentifierNonIDPSTaskHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CIdentifierNonIDPSTaskHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CIdentifierNonIDPSTaskHandler))
        return static_cast<void*>(const_cast< CIdentifierNonIDPSTaskHandler*>(this));
    return CIdentifierTaskHandler::qt_metacast(_clname);
}

int CIdentifierNonIDPSTaskHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CIdentifierTaskHandler::qt_metacall(_c, _id, _a);
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
