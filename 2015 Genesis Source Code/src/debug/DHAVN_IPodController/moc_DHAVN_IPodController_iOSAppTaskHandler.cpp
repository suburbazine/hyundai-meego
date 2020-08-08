/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_iOSAppTaskHandler.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TaskHandlers/iOSAppTaskHandler/include/DHAVN_IPodController_iOSAppTaskHandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_iOSAppTaskHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CiOSAppTaskHandler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,
      45,   35,   19,   19, 0x05,
      65,   63,   19,   19, 0x05,
     112,   19,   19,   19, 0x05,
     132,   63,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
     192,  170,   19,   19, 0x0a,
     228,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CiOSAppTaskHandler[] = {
    "CiOSAppTaskHandler\0\0mainLoopQuit()\0"
    "nExitCode\0mainLoopQuit(int)\0,\0"
    "OnBasicControlEnableStatusChange(bool,QString)\0"
    "OnTrackChanged(int)\0"
    "LaunchedDefaultMusicApp(bool,QString)\0"
    "lingoID,commandID,buf\0"
    "packetAvailable(int,int,QByteArray)\0"
    "delayBasicControlEnableSlot()\0"
};

void CiOSAppTaskHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CiOSAppTaskHandler *_t = static_cast<CiOSAppTaskHandler *>(_o);
        switch (_id) {
        case 0: _t->mainLoopQuit(); break;
        case 1: _t->mainLoopQuit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->OnBasicControlEnableStatusChange((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->OnTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->LaunchedDefaultMusicApp((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->packetAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 6: _t->delayBasicControlEnableSlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CiOSAppTaskHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CiOSAppTaskHandler::staticMetaObject = {
    { &CTaskHandler::staticMetaObject, qt_meta_stringdata_CiOSAppTaskHandler,
      qt_meta_data_CiOSAppTaskHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CiOSAppTaskHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CiOSAppTaskHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CiOSAppTaskHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CiOSAppTaskHandler))
        return static_cast<void*>(const_cast< CiOSAppTaskHandler*>(this));
    return CTaskHandler::qt_metacast(_clname);
}

int CiOSAppTaskHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CTaskHandler::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CiOSAppTaskHandler::mainLoopQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CiOSAppTaskHandler::mainLoopQuit(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CiOSAppTaskHandler::OnBasicControlEnableStatusChange(bool _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CiOSAppTaskHandler::OnTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CiOSAppTaskHandler::LaunchedDefaultMusicApp(bool _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
