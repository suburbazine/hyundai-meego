/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_remoteUiModeTaskHandler.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TaskHandlers/remoteUiModeTaskHandler/include/DHAVN_IPodController_remoteUiModeTaskHandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_remoteUiModeTaskHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CRemoteUIModeTaskHandler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   25,   25,   25, 0x05,
      48,   41,   25,   25, 0x05,
      75,   70,   25,   25, 0x05,
      98,   25,   25,   25, 0x05,
     124,   25,   25,   25, 0x05,
     150,   25,   25,   25, 0x05,

 // slots: signature, parameters, type, tag, flags
     187,  165,   25,   25, 0x0a,
     223,   25,   25,   25, 0x0a,
     251,   25,   25,   25, 0x0a,
     287,   25,   25,   25, 0x0a,
     308,   25,   25,   25, 0x0a,
     328,   25,   25,   25, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CRemoteUIModeTaskHandler[] = {
    "CRemoteUIModeTaskHandler\0\0mainLoopQuit()\0"
    "status\0EnteredInUiMode(bool)\0name\0"
    "GetDeviceName(QString)\0identify_timeout_signal()\0"
    "identify_restart_signal()\0reconnection()\0"
    "lingoID,commandID,buf\0"
    "packetAvailable(int,int,QByteArray)\0"
    "authorisationFinished(bool)\0"
    "IdentificationProcessFinished(bool)\0"
    "EnterUiModeTimeOut()\0enterRemoteUiMode()\0"
    "identify_timeout_slot()\0"
};

void CRemoteUIModeTaskHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CRemoteUIModeTaskHandler *_t = static_cast<CRemoteUIModeTaskHandler *>(_o);
        switch (_id) {
        case 0: _t->mainLoopQuit(); break;
        case 1: _t->EnteredInUiMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->GetDeviceName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->identify_timeout_signal(); break;
        case 4: _t->identify_restart_signal(); break;
        case 5: _t->reconnection(); break;
        case 6: _t->packetAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 7: _t->authorisationFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->IdentificationProcessFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->EnterUiModeTimeOut(); break;
        case 10: _t->enterRemoteUiMode(); break;
        case 11: _t->identify_timeout_slot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CRemoteUIModeTaskHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CRemoteUIModeTaskHandler::staticMetaObject = {
    { &CTaskHandler::staticMetaObject, qt_meta_stringdata_CRemoteUIModeTaskHandler,
      qt_meta_data_CRemoteUIModeTaskHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CRemoteUIModeTaskHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CRemoteUIModeTaskHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CRemoteUIModeTaskHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CRemoteUIModeTaskHandler))
        return static_cast<void*>(const_cast< CRemoteUIModeTaskHandler*>(this));
    return CTaskHandler::qt_metacast(_clname);
}

int CRemoteUIModeTaskHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CTaskHandler::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void CRemoteUIModeTaskHandler::mainLoopQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CRemoteUIModeTaskHandler::EnteredInUiMode(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CRemoteUIModeTaskHandler::GetDeviceName(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CRemoteUIModeTaskHandler::identify_timeout_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CRemoteUIModeTaskHandler::identify_restart_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void CRemoteUIModeTaskHandler::reconnection()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
