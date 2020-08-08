/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_iPodDBFetchTaskHandler.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TaskHandlers/iPodDBFetchTaskHandler/include/DHAVN_IPodController_iPodDBFetchTaskHandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_iPodDBFetchTaskHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CIPodDBFetchTaskHandler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      56,   25,   24,   24, 0x05,
      95,   24,   24,   24, 0x05,
     120,  110,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
     160,  138,   24,   24, 0x0a,
     196,   24,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CIPodDBFetchTaskHandler[] = {
    "CIPodDBFetchTaskHandler\0\0"
    "namesOfCategoryContents,length\0"
    "getListComplete(QMap<int,QString>,int)\0"
    "mainLoopQuit()\0nExitCode\0mainLoopQuit(int)\0"
    "lingoID,commandID,buf\0"
    "packetAvailable(int,int,QByteArray)\0"
    "getListTimeOut()\0"
};

void CIPodDBFetchTaskHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CIPodDBFetchTaskHandler *_t = static_cast<CIPodDBFetchTaskHandler *>(_o);
        switch (_id) {
        case 0: _t->getListComplete((*reinterpret_cast< QMap<int,QString>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->mainLoopQuit(); break;
        case 2: _t->mainLoopQuit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->packetAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 4: _t->getListTimeOut(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CIPodDBFetchTaskHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CIPodDBFetchTaskHandler::staticMetaObject = {
    { &CTaskHandler::staticMetaObject, qt_meta_stringdata_CIPodDBFetchTaskHandler,
      qt_meta_data_CIPodDBFetchTaskHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CIPodDBFetchTaskHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CIPodDBFetchTaskHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CIPodDBFetchTaskHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CIPodDBFetchTaskHandler))
        return static_cast<void*>(const_cast< CIPodDBFetchTaskHandler*>(this));
    return CTaskHandler::qt_metacast(_clname);
}

int CIPodDBFetchTaskHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CIPodDBFetchTaskHandler::getListComplete(QMap<int,QString> _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CIPodDBFetchTaskHandler::mainLoopQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CIPodDBFetchTaskHandler::mainLoopQuit(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
