/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_authorisor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TaskHandlers/authorisor/include/DHAVN_IPodController_authorisor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_authorisor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CAuthorisorTaskHandler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   24,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
      73,   51,   23,   23, 0x0a,
     116,  109,   23,   23, 0x0a,
     144,   23,   23,   23, 0x0a,
     167,   23,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CAuthorisorTaskHandler[] = {
    "CAuthorisorTaskHandler\0\0sucessful\0"
    "authorised(bool)\0lingoID,commandID,buf\0"
    "packetAvailable(int,int,QByteArray)\0"
    "status\0RemoteUiModeActivated(bool)\0"
    "retDevAuthentication()\0Authorisor_restart_slot()\0"
};

void CAuthorisorTaskHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CAuthorisorTaskHandler *_t = static_cast<CAuthorisorTaskHandler *>(_o);
        switch (_id) {
        case 0: _t->authorised((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->packetAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 2: _t->RemoteUiModeActivated((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->retDevAuthentication(); break;
        case 4: _t->Authorisor_restart_slot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CAuthorisorTaskHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAuthorisorTaskHandler::staticMetaObject = {
    { &CTaskHandler::staticMetaObject, qt_meta_stringdata_CAuthorisorTaskHandler,
      qt_meta_data_CAuthorisorTaskHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAuthorisorTaskHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAuthorisorTaskHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAuthorisorTaskHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAuthorisorTaskHandler))
        return static_cast<void*>(const_cast< CAuthorisorTaskHandler*>(this));
    return CTaskHandler::qt_metacast(_clname);
}

int CAuthorisorTaskHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CTaskHandler::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CAuthorisorTaskHandler::authorised(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
