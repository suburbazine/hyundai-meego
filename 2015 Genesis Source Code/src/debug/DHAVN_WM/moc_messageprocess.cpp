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
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   16,   15,   15, 0x05,
      56,   52,   15,   15, 0x05,
      98,   15,   15,   15, 0x05,
     105,   16,   15,   15, 0x05,
     124,   15,   15,   15, 0x05,
     141,   15,   15,   15, 0x05,
     158,   15,   15,   15, 0x05,
     179,   15,   15,   15, 0x05,
     210,   15,   15,   15, 0x05,
     236,   15,   15,   15, 0x05,
     250,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     266,   15,   15,   15, 0x0a,
     283,   15,   15,   15, 0x0a,
     304,   15,   15,   15, 0x0a,
     320,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MessageProcess[] = {
    "MessageProcess\0\0,\0operationCommand(int,QStringList)\0"
    ",,,\0osdCommand(int,QString,QStringList,uchar)\0"
    "beep()\0osdLock(bool,bool)\0screenSwap(bool)\0"
    "screenClone(int)\0languageChanged(int)\0"
    "applicationRegistered(QString)\0"
    "broadcastCommand(QString)\0clearOSDAll()\0"
    "clearFrontOSD()\0process(QString)\0"
    "sendFrontHomeClick()\0statusBarHide()\0"
    "runListener()\0"
};

void MessageProcess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MessageProcess *_t = static_cast<MessageProcess *>(_o);
        switch (_id) {
        case 0: _t->operationCommand((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2]))); break;
        case 1: _t->osdCommand((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3])),(*reinterpret_cast< uchar(*)>(_a[4]))); break;
        case 2: _t->beep(); break;
        case 3: _t->osdLock((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->screenSwap((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->screenClone((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->languageChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->applicationRegistered((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->broadcastCommand((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->clearOSDAll(); break;
        case 10: _t->clearFrontOSD(); break;
        case 11: _t->process((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->sendFrontHomeClick(); break;
        case 13: _t->statusBarHide(); break;
        case 14: _t->runListener(); break;
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
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void MessageProcess::operationCommand(int _t1, const QStringList & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MessageProcess::osdCommand(int _t1, const QString & _t2, const QStringList & _t3, uchar _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MessageProcess::beep()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void MessageProcess::osdLock(bool _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MessageProcess::screenSwap(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MessageProcess::screenClone(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MessageProcess::languageChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MessageProcess::applicationRegistered(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void MessageProcess::broadcastCommand(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void MessageProcess::clearOSDAll()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void MessageProcess::clearFrontOSD()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}
QT_END_MOC_NAMESPACE
