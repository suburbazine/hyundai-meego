/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFramework_AppEngineBase.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFramework_AppEngineBase.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFramework_AppEngineBase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppEngineBase[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      38,   14,   14,   14, 0x05,
      62,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      84,   14,   14,   14, 0x08,
     109,   14,   14,   14, 0x08,
     133,   14,   14,   14, 0x08,
     166,  159,   14,   14, 0x0a,
     198,  159,   14,   14, 0x0a,
     234,  159,   14,   14, 0x0a,
     289,  272,   14,   14, 0x0a,
     338,  330,   14,   14, 0x09,

 // methods: signature, parameters, type, tag, flags
     376,  359,   14,   14, 0x02,
     410,   14,  405,   14, 0x02,
     443,  428,   14,   14, 0x02,
     479,   14,  475,   14, 0x02,
     506,   14,  475,   14, 0x02,
     533,   14,  475,   14, 0x02,
     554,   14,  475,   14, 0x02,
     577,  573,   14,   14, 0x02,
     599,   14,  475,   14, 0x02,
     619,  573,   14,   14, 0x02,
     642,   14,  405,   14, 0x02,
     674,  660,   14,   14, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_AppEngineBase[] = {
    "AppEngineBase\0\0changedJogKeyPressed()\0"
    "changedJogKeyReleased()\0changedTouchPressed()\0"
    "CheckFrontGstPipelines()\0"
    "CheckRearGstPipelines()\0"
    "CheckCameraGstPipelines()\0aEvent\0"
    "NotifyFromUIStateManager(Event)\0"
    "NotifyFromUIStateManagerRear(Event)\0"
    "NotifyFromUIStateManagerCommon(Event)\0"
    "aEvent,aScreenId\0"
    "NotifyFromUIStateManagerBase(Event,uint)\0"
    "nScreen\0SendFGResponse(uint)\0"
    "x,y,width,height\0SetGeometry(int,int,int,int)\0"
    "bool\0IsSwapDCEnabled()\0nCMD,nDispType\0"
    "StatusBarPostEventSend(int,int)\0int\0"
    "GetCountryVariantFromQML()\0"
    "GetVehicleVariantFromQML()\0"
    "GetLanguageFromQML()\0getJogKeyPressed()\0"
    "key\0setJogKeyPressed(int)\0getJogKeyReleased()\0"
    "setJogKeyReleased(int)\0getTouchPressed()\0"
    "bTouchPressed\0setTouchPressed(bool)\0"
};

void AppEngineBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppEngineBase *_t = static_cast<AppEngineBase *>(_o);
        switch (_id) {
        case 0: _t->changedJogKeyPressed(); break;
        case 1: _t->changedJogKeyReleased(); break;
        case 2: _t->changedTouchPressed(); break;
        case 3: _t->CheckFrontGstPipelines(); break;
        case 4: _t->CheckRearGstPipelines(); break;
        case 5: _t->CheckCameraGstPipelines(); break;
        case 6: _t->NotifyFromUIStateManager((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 7: _t->NotifyFromUIStateManagerRear((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 8: _t->NotifyFromUIStateManagerCommon((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 9: _t->NotifyFromUIStateManagerBase((*reinterpret_cast< Event(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 10: _t->SendFGResponse((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 11: _t->SetGeometry((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 12: { bool _r = _t->IsSwapDCEnabled();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 13: _t->StatusBarPostEventSend((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: { int _r = _t->GetCountryVariantFromQML();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 15: { int _r = _t->GetVehicleVariantFromQML();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 16: { int _r = _t->GetLanguageFromQML();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 17: { int _r = _t->getJogKeyPressed();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 18: _t->setJogKeyPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: { int _r = _t->getJogKeyReleased();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 20: _t->setJogKeyReleased((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: { bool _r = _t->getTouchPressed();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 22: _t->setTouchPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppEngineBase::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppEngineBase::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppEngineBase,
      qt_meta_data_AppEngineBase, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppEngineBase::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppEngineBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppEngineBase::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppEngineBase))
        return static_cast<void*>(const_cast< AppEngineBase*>(this));
    return QObject::qt_metacast(_clname);
}

int AppEngineBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void AppEngineBase::changedJogKeyPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void AppEngineBase::changedJogKeyReleased()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void AppEngineBase::changedTouchPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
