/****************************************************************************
** Meta object code from reading C++ file 'OSD_Manager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/util/osd/OSD_Manager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OSD_Manager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OSDManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      52,   12,   11,   11, 0x08,
     118,  100,   11,   11, 0x08,
     181,  154,   11,   11, 0x08,
     231,  223,   11,   11, 0x08,
     266,  223,   11,   11, 0x08,
     308,   11,   11,   11, 0x08,
     334,  325,   11,   11, 0x08,
     351,   11,   11,   11, 0x08,
     370,   11,   11,   11, 0x08,
     388,  386,   11,   11, 0x08,
     433,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_OSDManager[] = {
    "OSDManager\0\0device,title,shuffle,repeat,scan,isTune\0"
    "displayOSD(OSD_DEVICE,QString,int,int,int,bool)\0"
    "device,pos,isLong\0updatePosition(OSD_DEVICE,int,bool)\0"
    "device,shuffle,repeat,scan\0"
    "updatePlayingMode(OSD_DEVICE,int,int,int)\0"
    "device,\0updateMetaData(OSD_DEVICE,QString)\0"
    "updateCDDAOSDMetaData(OSD_DEVICE,QString)\0"
    "showCurrentOSD()\0aDisplay\0clearAvOSD(uint)\0"
    "clearConnectInfo()\0onDiscLoading()\0,\0"
    "onDeviceRemoved(MEDIA_SOURCE_T,MEDIA_TYPE_T)\0"
    "displayAuxMode()\0"
};

void OSDManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OSDManager *_t = static_cast<OSDManager *>(_o);
        switch (_id) {
        case 0: _t->displayOSD((*reinterpret_cast< OSD_DEVICE(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6]))); break;
        case 1: _t->updatePosition((*reinterpret_cast< OSD_DEVICE(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 2: _t->updatePlayingMode((*reinterpret_cast< OSD_DEVICE(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 3: _t->updateMetaData((*reinterpret_cast< OSD_DEVICE(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->updateCDDAOSDMetaData((*reinterpret_cast< OSD_DEVICE(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->showCurrentOSD(); break;
        case 6: _t->clearAvOSD((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 7: _t->clearConnectInfo(); break;
        case 8: _t->onDiscLoading(); break;
        case 9: _t->onDeviceRemoved((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1])),(*reinterpret_cast< MEDIA_TYPE_T(*)>(_a[2]))); break;
        case 10: _t->displayAuxMode(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData OSDManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OSDManager::staticMetaObject = {
    { &OSDAbstractManager::staticMetaObject, qt_meta_stringdata_OSDManager,
      qt_meta_data_OSDManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OSDManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OSDManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OSDManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OSDManager))
        return static_cast<void*>(const_cast< OSDManager*>(this));
    return OSDAbstractManager::qt_metacast(_clname);
}

int OSDManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = OSDAbstractManager::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
