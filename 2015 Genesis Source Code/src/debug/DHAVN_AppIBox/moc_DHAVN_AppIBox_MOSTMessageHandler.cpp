/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppIBox_MOSTMessageHandler.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppIBox_MOSTMessageHandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppIBox_MOSTMessageHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MOSTMessageHandler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      49,   20,   19,   19, 0x05,
      82,   75,   19,   19, 0x05,
     129,  103,   19,   19, 0x05,
     198,  170,   19,   19, 0x05,
     254,  244,   19,   19, 0x05,
     309,  299,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
     329,   20,   19,   19, 0x0a,
     357,  103,   19,   19, 0x0a,
     400,  170,   19,   19, 0x0a,
     459,  448,   19,   19, 0x0a,
     478,  299,   19,   19, 0x0a,
     511,  500,   19,   19, 0x0a,
     573,  538,   19,   19, 0x0a,
     647,  613,   19,   19, 0x0a,
     701,  688,   19,   19, 0x0a,
     740,  729,   19,   19, 0x0a,
     779,  766,   19,   19, 0x0a,
     840,  810,   19,   19, 0x0a,
     884,   75,   19,   19, 0x0a,
     926,  908,   19,   19, 0x0a,
     959,  948,   19,   19, 0x0a,
     994,  978,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MOSTMessageHandler[] = {
    "MOSTMessageHandler\0\0diskTime,trackTime,titleTime\0"
    "TimePosition(int,int,int)\0status\0"
    "DLNAConnStatus(uint)\0playtime,trackno,songinfo\0"
    "CurrentSongInfo(uint,ushort,QStringList)\0"
    "vtitle,time,track,vfilename\0"
    "CurrentVideoInfo(QString,uint,ushort,QString)\0"
    "mediaInfo\0SendDLNAMediaInfoToHome(const HOME_MEDIA_T*)\0"
    "statusbar\0ShowStatusBar(uint)\0"
    "onTimePosition(int,int,int)\0"
    "onCurrentSongInfo(uint,ushort,QStringList)\0"
    "onCurrentVideoInfo(QString,uint,ushort,QString)\0"
    "deckStatus\0onDeckStatus(uint)\0"
    "onShowStatusBar(uint)\0displayOSD\0"
    "OnDisplayOSD(QDBusVariant)\0"
    "brightness,saturation,contrast,hue\0"
    "onAdjustScreenFromIBOX(int,int,int,int)\0"
    "target,lcdFront,lcdRear1,lcdRear2\0"
    "onLCDBrightnessFromIBOX(int,int,int,int)\0"
    "illumination\0onIlluminationFromIBOX(int)\0"
    "videoRatio\0onVideoRatioFromIBOX(int)\0"
    "displayOnOff\0onDisplayControlFromIBOX(uint)\0"
    "deviceID,senderHandle,sTarget\0"
    "onVideoDefaultRequestIBOX(uint,ushort,uint)\0"
    "onModemDataStatus(uint)\0mode,clientStatus\0"
    "onWifiMode(uint,uint)\0modemLevel\0"
    "onModemLevel(uint)\0userMode,micOff\0"
    "onUserMode(uint,bool)\0"
};

void MOSTMessageHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MOSTMessageHandler *_t = static_cast<MOSTMessageHandler *>(_o);
        switch (_id) {
        case 0: _t->TimePosition((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 1: _t->DLNAConnStatus((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 2: _t->CurrentSongInfo((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3]))); break;
        case 3: _t->CurrentVideoInfo((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const ushort(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 4: _t->SendDLNAMediaInfoToHome((*reinterpret_cast< const HOME_MEDIA_T*(*)>(_a[1]))); break;
        case 5: _t->ShowStatusBar((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 6: _t->onTimePosition((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 7: _t->onCurrentSongInfo((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3]))); break;
        case 8: _t->onCurrentVideoInfo((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const ushort(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 9: _t->onDeckStatus((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 10: _t->onShowStatusBar((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 11: _t->OnDisplayOSD((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 12: _t->onAdjustScreenFromIBOX((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 13: _t->onLCDBrightnessFromIBOX((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 14: _t->onIlluminationFromIBOX((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 15: _t->onVideoRatioFromIBOX((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 16: _t->onDisplayControlFromIBOX((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 17: _t->onVideoDefaultRequestIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 18: _t->onModemDataStatus((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 19: _t->onWifiMode((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 20: _t->onModemLevel((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 21: _t->onUserMode((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MOSTMessageHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MOSTMessageHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MOSTMessageHandler,
      qt_meta_data_MOSTMessageHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MOSTMessageHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MOSTMessageHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MOSTMessageHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MOSTMessageHandler))
        return static_cast<void*>(const_cast< MOSTMessageHandler*>(this));
    return QObject::qt_metacast(_clname);
}

int MOSTMessageHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void MOSTMessageHandler::TimePosition(const int _t1, const int _t2, const int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MOSTMessageHandler::DLNAConnStatus(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MOSTMessageHandler::CurrentSongInfo(const uint _t1, const ushort _t2, const QStringList _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MOSTMessageHandler::CurrentVideoInfo(const QString _t1, const uint _t2, const ushort _t3, const QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MOSTMessageHandler::SendDLNAMediaInfoToHome(const HOME_MEDIA_T * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MOSTMessageHandler::ShowStatusBar(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
