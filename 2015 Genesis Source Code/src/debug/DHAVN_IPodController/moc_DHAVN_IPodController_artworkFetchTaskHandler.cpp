/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_artworkFetchTaskHandler.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TaskHandlers/artworkFetchTaskHandler/include/DHAVN_IPodController_artworkFetchTaskHandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_artworkFetchTaskHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CArtworkFetchTaskHandler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      36,   26,   25,   25, 0x05,
      54,   25,   25,   25, 0x05,

 // slots: signature, parameters, type, tag, flags
      91,   69,   25,   25, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CArtworkFetchTaskHandler[] = {
    "CArtworkFetchTaskHandler\0\0nExitCode\0"
    "mainLoopQuit(int)\0mainLoopQuit()\0"
    "lingoID,commandID,buf\0"
    "packetAvailable(int,int,QByteArray)\0"
};

void CArtworkFetchTaskHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CArtworkFetchTaskHandler *_t = static_cast<CArtworkFetchTaskHandler *>(_o);
        switch (_id) {
        case 0: _t->mainLoopQuit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->mainLoopQuit(); break;
        case 2: _t->packetAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CArtworkFetchTaskHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CArtworkFetchTaskHandler::staticMetaObject = {
    { &CTaskHandler::staticMetaObject, qt_meta_stringdata_CArtworkFetchTaskHandler,
      qt_meta_data_CArtworkFetchTaskHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CArtworkFetchTaskHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CArtworkFetchTaskHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CArtworkFetchTaskHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CArtworkFetchTaskHandler))
        return static_cast<void*>(const_cast< CArtworkFetchTaskHandler*>(this));
    return CTaskHandler::qt_metacast(_clname);
}

int CArtworkFetchTaskHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CTaskHandler::qt_metacall(_c, _id, _a);
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
void CArtworkFetchTaskHandler::mainLoopQuit(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CArtworkFetchTaskHandler::mainLoopQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
