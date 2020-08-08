/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_QRPMClient.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_QRPMClient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_QRPMClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QRPMClient[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: signature, parameters, type, tag, flags
      37,   12,   11,   11, 0x05,
     100,   78,   11,   11, 0x05,
     165,  148,   11,   11, 0x05,
     226,  210,   11,   11, 0x05,
     289,  274,   11,   11, 0x05,
     339,   11,   11,   11, 0x05,
     361,   11,   11,   11, 0x05,
     388,   11,   11,   11, 0x05,
     414,   11,   11,   11, 0x05,
     435,   78,   11,   11, 0x05,
     504,  482,   11,   11, 0x05,
     566,  548,   11,   11, 0x05,
     605,  148,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     652,  646,   11,   11, 0x0a,
     686,  646,   11,   11, 0x0a,
     720,  646,   11,   11, 0x0a,
     753,  646,   11,   11, 0x0a,
     787,  646,   11,   11, 0x0a,
     826,   11,   11,   11, 0x0a,
     858,   11,   11,   11, 0x0a,
     889,   11,   11,   11, 0x0a,
     920,   11,   11,   11, 0x0a,
     953,   11,   11,   11, 0x0a,
     985,   11,   11,   11, 0x0a,
    1024, 1017,   11,   11, 0x0a,
    1061,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QRPMClient[] = {
    "QRPMClient\0\0eVolumeMode,nVolumeValue\0"
    "MICOMVolumeStepSignal(TVolumeMode,qint8)\0"
    "eAudioMode,eAudioSink\0"
    "RPMClientAcquireResponse(TAudioMode,TAudioSink)\0"
    "sAudioVolumeInfo\0"
    "RPMClientGetVolumeResponse(SAudioVolumeInfo)\0"
    "sAudioSetupInfo\0"
    "RPMClientGetAudioSetupResponse(SAudioSetupInfo)\0"
    "sLastAudioList\0"
    "RPMClientGetLastAudioListResponse(SLastAudioList)\0"
    "ASMModeChangeSignal()\0ASMMixingInfoSignal(qint8)\0"
    "ASMCallInProgressSignal()\0"
    "VRNotFesibleSignal()\0"
    "RPMClientAvModeResponse(TAudioMode,TAudioSink)\0"
    "eAudioChannel,bResult\0"
    "RPMClientMixResponse(TAudioMixChannel,bool)\0"
    "eMuteMode,bResult\0"
    "RPMClientMuteResponse(EAudioSink,bool)\0"
    "RPMClientGetVolumeInfo(SAudioVolumeInfo)\0"
    "reply\0MuteAudioResponseSlot(QByteArray)\0"
    "MainAudioResponseSlot(QByteArray)\0"
    "MixAudioResponseSlot(QByteArray)\0"
    "LastAudioResponseSlot(QByteArray)\0"
    "DBusMicomResponseHandler(QDBusMessage)\0"
    "ErrorResponseHandlerMainAudio()\0"
    "ErrorResponseHandlerSubAudio()\0"
    "ErrorResponseHandlerMixAudio()\0"
    "ErrorResponseHandlerVolumeInfo()\0"
    "ErrorResponseHandlerSetupInfo()\0"
    "ErrorResponseHandlerLastAudio()\0signal\0"
    "DBusMicomSignalHandler(QDBusMessage)\0"
    "DBusASMSignalHandler(QDBusMessage)\0"
};

void QRPMClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QRPMClient *_t = static_cast<QRPMClient *>(_o);
        switch (_id) {
        case 0: _t->MICOMVolumeStepSignal((*reinterpret_cast< TVolumeMode(*)>(_a[1])),(*reinterpret_cast< qint8(*)>(_a[2]))); break;
        case 1: _t->RPMClientAcquireResponse((*reinterpret_cast< TAudioMode(*)>(_a[1])),(*reinterpret_cast< TAudioSink(*)>(_a[2]))); break;
        case 2: _t->RPMClientGetVolumeResponse((*reinterpret_cast< SAudioVolumeInfo(*)>(_a[1]))); break;
        case 3: _t->RPMClientGetAudioSetupResponse((*reinterpret_cast< SAudioSetupInfo(*)>(_a[1]))); break;
        case 4: _t->RPMClientGetLastAudioListResponse((*reinterpret_cast< SLastAudioList(*)>(_a[1]))); break;
        case 5: _t->ASMModeChangeSignal(); break;
        case 6: _t->ASMMixingInfoSignal((*reinterpret_cast< qint8(*)>(_a[1]))); break;
        case 7: _t->ASMCallInProgressSignal(); break;
        case 8: _t->VRNotFesibleSignal(); break;
        case 9: _t->RPMClientAvModeResponse((*reinterpret_cast< TAudioMode(*)>(_a[1])),(*reinterpret_cast< TAudioSink(*)>(_a[2]))); break;
        case 10: _t->RPMClientMixResponse((*reinterpret_cast< TAudioMixChannel(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 11: _t->RPMClientMuteResponse((*reinterpret_cast< EAudioSink(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 12: _t->RPMClientGetVolumeInfo((*reinterpret_cast< SAudioVolumeInfo(*)>(_a[1]))); break;
        case 13: _t->MuteAudioResponseSlot((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 14: _t->MainAudioResponseSlot((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 15: _t->MixAudioResponseSlot((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 16: _t->LastAudioResponseSlot((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 17: _t->DBusMicomResponseHandler((*reinterpret_cast< QDBusMessage(*)>(_a[1]))); break;
        case 18: _t->ErrorResponseHandlerMainAudio(); break;
        case 19: _t->ErrorResponseHandlerSubAudio(); break;
        case 20: _t->ErrorResponseHandlerMixAudio(); break;
        case 21: _t->ErrorResponseHandlerVolumeInfo(); break;
        case 22: _t->ErrorResponseHandlerSetupInfo(); break;
        case 23: _t->ErrorResponseHandlerLastAudio(); break;
        case 24: _t->DBusMicomSignalHandler((*reinterpret_cast< QDBusMessage(*)>(_a[1]))); break;
        case 25: _t->DBusASMSignalHandler((*reinterpret_cast< QDBusMessage(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QRPMClient::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QRPMClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QRPMClient,
      qt_meta_data_QRPMClient, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QRPMClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QRPMClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QRPMClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QRPMClient))
        return static_cast<void*>(const_cast< QRPMClient*>(this));
    return QObject::qt_metacast(_clname);
}

int QRPMClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void QRPMClient::MICOMVolumeStepSignal(TVolumeMode _t1, qint8 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QRPMClient::RPMClientAcquireResponse(TAudioMode _t1, TAudioSink _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QRPMClient::RPMClientGetVolumeResponse(SAudioVolumeInfo _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QRPMClient::RPMClientGetAudioSetupResponse(SAudioSetupInfo _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QRPMClient::RPMClientGetLastAudioListResponse(SLastAudioList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QRPMClient::ASMModeChangeSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void QRPMClient::ASMMixingInfoSignal(qint8 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QRPMClient::ASMCallInProgressSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void QRPMClient::VRNotFesibleSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void QRPMClient::RPMClientAvModeResponse(TAudioMode _t1, TAudioSink _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void QRPMClient::RPMClientMixResponse(TAudioMixChannel _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void QRPMClient::RPMClientMuteResponse(EAudioSink _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void QRPMClient::RPMClientGetVolumeInfo(SAudioVolumeInfo _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}
QT_END_MOC_NAMESPACE
