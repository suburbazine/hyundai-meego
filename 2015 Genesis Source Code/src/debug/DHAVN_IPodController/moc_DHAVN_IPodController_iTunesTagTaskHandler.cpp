/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_iTunesTagTaskHandler.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TaskHandlers/iTunesTagTaskHandler/include/DHAVN_IPodController_iTunesTagTaskHandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_iTunesTagTaskHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CiTunesTagTaskHandler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x05,
      48,   38,   22,   22, 0x05,
      66,   22,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
     110,   88,   22,   22, 0x0a,
     166,  146,   22,   22, 0x0a,
     216,  206,  202,   22, 0x0a,
     240,   22,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CiTunesTagTaskHandler[] = {
    "CiTunesTagTaskHandler\0\0mainLoopQuit()\0"
    "nExitCode\0mainLoopQuit(int)\0"
    "IPodPopUp(EIPodPopUp)\0lingoID,commandID,buf\0"
    "packetAvailable(int,int,QByteArray)\0"
    "pListName,pListPath\0"
    "updateTagListenerE(QString,QString)\0"
    "int\0plistPath\0exportTagFileE(QString)\0"
    "closeFileHandleE(int)\0"
};

void CiTunesTagTaskHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CiTunesTagTaskHandler *_t = static_cast<CiTunesTagTaskHandler *>(_o);
        switch (_id) {
        case 0: _t->mainLoopQuit(); break;
        case 1: _t->mainLoopQuit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->IPodPopUp((*reinterpret_cast< EIPodPopUp(*)>(_a[1]))); break;
        case 3: _t->packetAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 4: _t->updateTagListenerE((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: { int _r = _t->exportTagFileE((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 6: _t->closeFileHandleE((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CiTunesTagTaskHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CiTunesTagTaskHandler::staticMetaObject = {
    { &CTaskHandler::staticMetaObject, qt_meta_stringdata_CiTunesTagTaskHandler,
      qt_meta_data_CiTunesTagTaskHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CiTunesTagTaskHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CiTunesTagTaskHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CiTunesTagTaskHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CiTunesTagTaskHandler))
        return static_cast<void*>(const_cast< CiTunesTagTaskHandler*>(this));
    return CTaskHandler::qt_metacast(_clname);
}

int CiTunesTagTaskHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CiTunesTagTaskHandler::mainLoopQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CiTunesTagTaskHandler::mainLoopQuit(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CiTunesTagTaskHandler::IPodPopUp(EIPodPopUp _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
