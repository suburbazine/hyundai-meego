/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_EngineBase.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/engine/DHAVN_AppMediaPlayer_EngineBase.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_EngineBase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_EngineBase[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: signature, parameters, type, tag, flags
      35,   27,   26,   26, 0x05,
      71,   56,   26,   26, 0x05,
     109,  103,   26,   26, 0x05,
     131,  125,   26,   26, 0x05,
     193,  190,   26,   26, 0x05,
     240,  236,   26,   26, 0x05,
     289,   26,   26,   26, 0x05,
     315,  313,   26,   26, 0x05,
     360,  313,   26,   26, 0x05,

 // methods: signature, parameters, type, tag, flags
     409,   56,   26,   26, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_EngineBase[] = {
    "AppMediaPlayer_EngineBase\0\0isFront\0"
    "contextChanged(bool)\0nCMD,nDispType\0"
    "statusBarPostEventSend(int,int)\0nType\0"
    "updateOSD(uint)\0,,,,,\0"
    "setOSDMetaDataChanged(OSD_DEVICE,QString,int,int,int,bool)\0"
    ",,\0updatePositionChanged(OSD_DEVICE,int,bool)\0"
    ",,,\0updatePlayingModeChanged(OSD_DEVICE,int,int,int)\0"
    "showCurrentOSDChanged()\0,\0"
    "updateOSDMetaDataChanged(OSD_DEVICE,QString)\0"
    "updateCDDAOSDMetaDataChanged(OSD_DEVICE,QString)\0"
    "StatusBarPostEventSend(int,int)\0"
};

void AppMediaPlayer_EngineBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_EngineBase *_t = static_cast<AppMediaPlayer_EngineBase *>(_o);
        switch (_id) {
        case 0: _t->contextChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->statusBarPostEventSend((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->updateOSD((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 3: _t->setOSDMetaDataChanged((*reinterpret_cast< OSD_DEVICE(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6]))); break;
        case 4: _t->updatePositionChanged((*reinterpret_cast< OSD_DEVICE(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 5: _t->updatePlayingModeChanged((*reinterpret_cast< OSD_DEVICE(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 6: _t->showCurrentOSDChanged(); break;
        case 7: _t->updateOSDMetaDataChanged((*reinterpret_cast< OSD_DEVICE(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 8: _t->updateCDDAOSDMetaDataChanged((*reinterpret_cast< OSD_DEVICE(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 9: _t->StatusBarPostEventSend((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_EngineBase::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_EngineBase::staticMetaObject = {
    { &QDeclarativeItem::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_EngineBase,
      qt_meta_data_AppMediaPlayer_EngineBase, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_EngineBase::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_EngineBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_EngineBase::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_EngineBase))
        return static_cast<void*>(const_cast< AppMediaPlayer_EngineBase*>(this));
    return QDeclarativeItem::qt_metacast(_clname);
}

int AppMediaPlayer_EngineBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDeclarativeItem::qt_metacall(_c, _id, _a);
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
void AppMediaPlayer_EngineBase::contextChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_EngineBase::statusBarPostEventSend(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_EngineBase::updateOSD(unsigned int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppMediaPlayer_EngineBase::setOSDMetaDataChanged(OSD_DEVICE _t1, QString _t2, int _t3, int _t4, int _t5, bool _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AppMediaPlayer_EngineBase::updatePositionChanged(OSD_DEVICE _t1, int _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AppMediaPlayer_EngineBase::updatePlayingModeChanged(OSD_DEVICE _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void AppMediaPlayer_EngineBase::showCurrentOSDChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void AppMediaPlayer_EngineBase::updateOSDMetaDataChanged(OSD_DEVICE _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void AppMediaPlayer_EngineBase::updateCDDAOSDMetaDataChanged(OSD_DEVICE _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
