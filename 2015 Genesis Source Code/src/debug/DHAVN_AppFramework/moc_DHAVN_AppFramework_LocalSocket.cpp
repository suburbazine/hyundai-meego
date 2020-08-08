/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFramework_LocalSocket.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFramework_LocalSocket.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFramework_LocalSocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LocalSocketIpcClient[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      30,   22,   21,   21, 0x0a,
      44,   22,   21,   21, 0x0a,
      61,   21,   21,   21, 0x0a,
      80,   21,   21,   21, 0x0a,
     102,   21,   21,   21, 0x0a,
     121,   21,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LocalSocketIpcClient[] = {
    "LocalSocketIpcClient\0\0message\0"
    "send(QString)\0send(QByteArray)\0"
    "socket_connected()\0socket_disconnected()\0"
    "socket_readReady()\0"
    "socket_error(QLocalSocket::LocalSocketError)\0"
};

void LocalSocketIpcClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LocalSocketIpcClient *_t = static_cast<LocalSocketIpcClient *>(_o);
        switch (_id) {
        case 0: _t->send((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->send((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->socket_connected(); break;
        case 3: _t->socket_disconnected(); break;
        case 4: _t->socket_readReady(); break;
        case 5: _t->socket_error((*reinterpret_cast< QLocalSocket::LocalSocketError(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LocalSocketIpcClient::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LocalSocketIpcClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LocalSocketIpcClient,
      qt_meta_data_LocalSocketIpcClient, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LocalSocketIpcClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LocalSocketIpcClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LocalSocketIpcClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LocalSocketIpcClient))
        return static_cast<void*>(const_cast< LocalSocketIpcClient*>(this));
    return QObject::qt_metacast(_clname);
}

int LocalSocketIpcClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
