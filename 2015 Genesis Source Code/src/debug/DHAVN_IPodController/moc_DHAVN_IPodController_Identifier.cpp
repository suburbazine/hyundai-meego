/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_Identifier.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TaskHandlers/Identifier/include/DHAVN_IPodController_Identifier.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_Identifier.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CIdentifierTaskHandler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      35,   24,   23,   23, 0x05,
      68,   52,   23,   23, 0x05,
     111,  101,   23,   23, 0x05,
     136,   23,   23,   23, 0x05,
     158,  151,   23,   23, 0x05,
     183,   23,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
     220,  198,   23,   23, 0x0a,
     256,   23,   23,   23, 0x0a,
     268,   23,   23,   23, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CIdentifierTaskHandler[] = {
    "CIdentifierTaskHandler\0\0successful\0"
    "identified(bool)\0AppId,sessionId\0"
    "AppSessionOpened(QString,qint16)\0"
    "sessionId\0AppSessionClosed(qint16)\0"
    "mainLoopQuit()\0isIDPS\0identificationType(bool)\0"
    "reconnection()\0lingoID,commandID,buf\0"
    "packetAvailable(int,int,QByteArray)\0"
    "StartIDPS()\0StartIDPS_timeout_slot()\0"
};

void CIdentifierTaskHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CIdentifierTaskHandler *_t = static_cast<CIdentifierTaskHandler *>(_o);
        switch (_id) {
        case 0: _t->identified((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->AppSessionOpened((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< qint16(*)>(_a[2]))); break;
        case 2: _t->AppSessionClosed((*reinterpret_cast< qint16(*)>(_a[1]))); break;
        case 3: _t->mainLoopQuit(); break;
        case 4: _t->identificationType((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->reconnection(); break;
        case 6: _t->packetAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 7: _t->StartIDPS(); break;
        case 8: _t->StartIDPS_timeout_slot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CIdentifierTaskHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CIdentifierTaskHandler::staticMetaObject = {
    { &CTaskHandler::staticMetaObject, qt_meta_stringdata_CIdentifierTaskHandler,
      qt_meta_data_CIdentifierTaskHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CIdentifierTaskHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CIdentifierTaskHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CIdentifierTaskHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CIdentifierTaskHandler))
        return static_cast<void*>(const_cast< CIdentifierTaskHandler*>(this));
    return CTaskHandler::qt_metacast(_clname);
}

int CIdentifierTaskHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CTaskHandler::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CIdentifierTaskHandler::identified(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CIdentifierTaskHandler::AppSessionOpened(const QString & _t1, qint16 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CIdentifierTaskHandler::AppSessionClosed(qint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CIdentifierTaskHandler::mainLoopQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CIdentifierTaskHandler::identificationType(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CIdentifierTaskHandler::reconnection()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
