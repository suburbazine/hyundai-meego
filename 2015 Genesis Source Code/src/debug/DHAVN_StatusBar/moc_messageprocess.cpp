/****************************************************************************
** Meta object code from reading C++ file 'messageprocess.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/messageprocess.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'messageprocess.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MessageProcess[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      51,   15,   15,   15, 0x05,
      85,   15,   15,   15, 0x05,
      94,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     108,   15,   15,   15, 0x08,
     119,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MessageProcess[] = {
    "MessageProcess\0\0statusCommandReceived(QStringList)\0"
    "eventCommandReceived(QStringList)\0"
    "agreed()\0backToAgree()\0readData()\0"
    "checkState(QLocalSocket::LocalSocketState)\0"
};

void MessageProcess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MessageProcess *_t = static_cast<MessageProcess *>(_o);
        switch (_id) {
        case 0: _t->statusCommandReceived((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->eventCommandReceived((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 2: _t->agreed(); break;
        case 3: _t->backToAgree(); break;
        case 4: _t->readData(); break;
        case 5: _t->checkState((*reinterpret_cast< QLocalSocket::LocalSocketState(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MessageProcess::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MessageProcess::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MessageProcess,
      qt_meta_data_MessageProcess, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MessageProcess::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MessageProcess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MessageProcess::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MessageProcess))
        return static_cast<void*>(const_cast< MessageProcess*>(this));
    return QObject::qt_metacast(_clname);
}

int MessageProcess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void MessageProcess::statusCommandReceived(const QStringList & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MessageProcess::eventCommandReceived(const QStringList & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MessageProcess::agreed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void MessageProcess::backToAgree()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
