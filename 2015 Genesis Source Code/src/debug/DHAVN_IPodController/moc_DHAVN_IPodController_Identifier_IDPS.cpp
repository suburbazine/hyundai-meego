/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_Identifier_IDPS.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TaskHandlers/Identifier/include/DHAVN_IPodController_Identifier_IDPS.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_Identifier_IDPS.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CIdentifierIDPSTaskHandler[] = {

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
      50,   28,   27,   27, 0x0a,
      86,   27,   27,   27, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CIdentifierIDPSTaskHandler[] = {
    "CIdentifierIDPSTaskHandler\0\0"
    "lingoID,commandID,buf\0"
    "packetAvailable(int,int,QByteArray)\0"
    "identify_restart_slot()\0"
};

void CIdentifierIDPSTaskHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CIdentifierIDPSTaskHandler *_t = static_cast<CIdentifierIDPSTaskHandler *>(_o);
        switch (_id) {
        case 0: _t->packetAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 1: _t->identify_restart_slot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CIdentifierIDPSTaskHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CIdentifierIDPSTaskHandler::staticMetaObject = {
    { &CIdentifierTaskHandler::staticMetaObject, qt_meta_stringdata_CIdentifierIDPSTaskHandler,
      qt_meta_data_CIdentifierIDPSTaskHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CIdentifierIDPSTaskHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CIdentifierIDPSTaskHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CIdentifierIDPSTaskHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CIdentifierIDPSTaskHandler))
        return static_cast<void*>(const_cast< CIdentifierIDPSTaskHandler*>(this));
    return CIdentifierTaskHandler::qt_metacast(_clname);
}

int CIdentifierIDPSTaskHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CIdentifierTaskHandler::qt_metacall(_c, _id, _a);
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
