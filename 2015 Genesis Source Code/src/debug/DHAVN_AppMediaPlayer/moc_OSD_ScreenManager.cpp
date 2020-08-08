/****************************************************************************
** Meta object code from reading C++ file 'OSD_ScreenManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/util/osd/OSD_ScreenManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OSD_ScreenManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OSDScreenManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,
      35,   17,   17,   17, 0x05,
      53,   17,   17,   17, 0x05,
      70,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      89,   17,   17,   17, 0x08,
     106,   17,   17,   17, 0x08,
     122,   17,   17,   17, 0x08,
     148,   17,   17,   17, 0x08,
     173,   17,   17,   17, 0x08,
     195,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_OSDScreenManager[] = {
    "OSDScreenManager\0\0clearAvOSD(uint)\0"
    "agreeScreenShow()\0lastIncomedOSD()\0"
    "clearConnectInfo()\0enterSubScreen()\0"
    "exitSubScreen()\0enterVideoSubScreen(bool)\0"
    "exitVideoSubScreen(bool)\0enterFullScreen(bool)\0"
    "exitFullScreen(bool)\0"
};

void OSDScreenManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OSDScreenManager *_t = static_cast<OSDScreenManager *>(_o);
        switch (_id) {
        case 0: _t->clearAvOSD((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 1: _t->agreeScreenShow(); break;
        case 2: _t->lastIncomedOSD(); break;
        case 3: _t->clearConnectInfo(); break;
        case 4: _t->enterSubScreen(); break;
        case 5: _t->exitSubScreen(); break;
        case 6: _t->enterVideoSubScreen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->exitVideoSubScreen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->enterFullScreen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->exitFullScreen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData OSDScreenManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OSDScreenManager::staticMetaObject = {
    { &OSDAbstractManager::staticMetaObject, qt_meta_stringdata_OSDScreenManager,
      qt_meta_data_OSDScreenManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OSDScreenManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OSDScreenManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OSDScreenManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OSDScreenManager))
        return static_cast<void*>(const_cast< OSDScreenManager*>(this));
    return OSDAbstractManager::qt_metacast(_clname);
}

int OSDScreenManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = OSDAbstractManager::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void OSDScreenManager::clearAvOSD(unsigned int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OSDScreenManager::agreeScreenShow()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void OSDScreenManager::lastIncomedOSD()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void OSDScreenManager::clearConnectInfo()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
