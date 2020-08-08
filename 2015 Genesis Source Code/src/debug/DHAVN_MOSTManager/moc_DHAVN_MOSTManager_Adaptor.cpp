/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_MOSTManager_Adaptor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_MOSTManager_Adaptor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_MOSTManager_Adaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CAuxInAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   20,   15,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CAuxInAdaptor[] = {
    "CAuxInAdaptor\0\0bool\0msg\0"
    "SetAuxDeviceInfo(QDBusMessage)\0"
};

void CAuxInAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CAuxInAdaptor *_t = static_cast<CAuxInAdaptor *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->SetAuxDeviceInfo((*reinterpret_cast< const QDBusMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CAuxInAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAuxInAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CAuxInAdaptor,
      qt_meta_data_CAuxInAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAuxInAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAuxInAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAuxInAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAuxInAdaptor))
        return static_cast<void*>(const_cast< CAuxInAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int CAuxInAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_CBlueToothAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      48,   19,   18,   18, 0x05,
     112,   80,   18,   18, 0x05,
     169,  153,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     189,   19,   18,   18, 0x08,
     225,   80,   18,   18, 0x08,
     270,  153,   18,   18, 0x08,
     313,  299,  294,   18, 0x0a,
     340,   19,  294,   18, 0x0a,
     408,  373,  294,   18, 0x0a,
     457,  450,  294,   18, 0x0a,
     475,  153,  294,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CBlueToothAdaptor[] = {
    "CBlueToothAdaptor\0\0senderHandle,callId,deviceID\0"
    "StartHangupCall(uint,uint,uint)\0"
    "senderHandle,telNumber,deviceID\0"
    "DialNumberRequestIBOX(uint,QString,uint)\0"
    "usermode,micOff\0UserMode(uint,bool)\0"
    "StartHangupCallSlot(uint,uint,uint)\0"
    "DialNumberRequestIBOXSlot(uint,QString,uint)\0"
    "UserModeSlot(uint,bool)\0bool\0callStatedata\0"
    "SetCallState(QDBusVariant)\0"
    "HangupCallResult(uint,uint,uint)\0"
    "state,senderHandle,callID,deviceID\0"
    "DialNumberResultIBOX(uint,uint,uint,uint)\0"
    "btMode\0SetBTStatus(uint)\0"
    "SetUserMode(uint,bool)\0"
};

void CBlueToothAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CBlueToothAdaptor *_t = static_cast<CBlueToothAdaptor *>(_o);
        switch (_id) {
        case 0: _t->StartHangupCall((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 1: _t->DialNumberRequestIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 2: _t->UserMode((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        case 3: _t->StartHangupCallSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 4: _t->DialNumberRequestIBOXSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 5: _t->UserModeSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        case 6: { bool _r = _t->SetCallState((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: { bool _r = _t->HangupCallResult((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: { bool _r = _t->DialNumberResultIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])),(*reinterpret_cast< const uint(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: { bool _r = _t->SetBTStatus((*reinterpret_cast< const uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { bool _r = _t->SetUserMode((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CBlueToothAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CBlueToothAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CBlueToothAdaptor,
      qt_meta_data_CBlueToothAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CBlueToothAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CBlueToothAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CBlueToothAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CBlueToothAdaptor))
        return static_cast<void*>(const_cast< CBlueToothAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int CBlueToothAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void CBlueToothAdaptor::StartHangupCall(const uint _t1, const uint _t2, const uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CBlueToothAdaptor::DialNumberRequestIBOX(const uint _t1, const QString _t2, const uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CBlueToothAdaptor::UserMode(const uint _t1, const bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
static const uint qt_meta_data_CDiagnosisAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      33,   19,   18,   18, 0x05,
      66,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      89,   19,   18,   18, 0x08,
     126,   18,   18,   18, 0x08,
     158,   18,  153,   18, 0x0a,
     189,  177,  153,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CDiagnosisAdaptor[] = {
    "CDiagnosisAdaptor\0\0mainSWVersion\0"
    "MainSWVersionStatus(QStringList)\0"
    "GetVariantCodeStatus()\0"
    "MainSWVersionStatusSlot(QStringList)\0"
    "GetVariantCodeStatusSlot()\0bool\0"
    "GetMainSWVersion()\0variantcode\0"
    "SetVariantCode(QDBusVariant)\0"
};

void CDiagnosisAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CDiagnosisAdaptor *_t = static_cast<CDiagnosisAdaptor *>(_o);
        switch (_id) {
        case 0: _t->MainSWVersionStatus((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->GetVariantCodeStatus(); break;
        case 2: _t->MainSWVersionStatusSlot((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 3: _t->GetVariantCodeStatusSlot(); break;
        case 4: { bool _r = _t->GetMainSWVersion();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { bool _r = _t->SetVariantCode((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CDiagnosisAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CDiagnosisAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CDiagnosisAdaptor,
      qt_meta_data_CDiagnosisAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CDiagnosisAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CDiagnosisAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CDiagnosisAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDiagnosisAdaptor))
        return static_cast<void*>(const_cast< CDiagnosisAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int CDiagnosisAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CDiagnosisAdaptor::MainSWVersionStatus(const QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CDiagnosisAdaptor::GetVariantCodeStatus()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_CDLNAAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      43,   14,   13,   13, 0x05,
      76,   69,   13,   13, 0x05,
     123,   97,   13,   13, 0x05,
     192,  164,   13,   13, 0x05,
     249,  238,   13,   13, 0x05,
     280,  266,   13,   13, 0x05,
     340,  300,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     368,   14,   13,   13, 0x08,
     398,   69,   13,   13, 0x08,
     423,   97,   13,   13, 0x08,
     468,  164,   13,   13, 0x08,
     518,  238,   13,   13, 0x08,
     539,  266,   13,   13, 0x08,
     563,  300,   13,   13, 0x08,
     604,  600,  595,   13, 0x0a,
     632,  600,  595,   13, 0x0a,
     670,  663,  595,   13, 0x0a,
     702,  663,  595,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CDLNAAdaptor[] = {
    "CDLNAAdaptor\0\0diskTime,trackTime,titleTime\0"
    "TimePosition(int,int,int)\0status\0"
    "DLNAConnStatus(uint)\0playtime,trackno,songinfo\0"
    "CurrentSongInfo(uint,ushort,QStringList)\0"
    "vtitle,time,track,vfilename\0"
    "CurrentVideoInfo(QString,uint,ushort,QString)\0"
    "deckStatus\0DeckStatus(uint)\0trackPosition\0"
    "TrackPosition(uint)\0"
    "currentNumTrack,currentRelativeTrackPos\0"
    "TrackInformation(uint,uint)\0"
    "TimePositionSlot(int,int,int)\0"
    "DLNAConnStatusSlot(uint)\0"
    "CurrentSongInfoSlot(uint,ushort,QStringList)\0"
    "CurrentVideoInfoSlot(QString,uint,ushort,QString)\0"
    "DeckStatusSlot(uint)\0TrackPositionSlot(uint)\0"
    "TrackInformationSlot(uint,uint)\0bool\0"
    "msg\0SetDeckStatus(QDBusMessage)\0"
    "SetTrackPosition(QDBusMessage)\0nSteps\0"
    "SetIncrementTrackPosition(uint)\0"
    "SetDecrementTrackPosition(uint)\0"
};

void CDLNAAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CDLNAAdaptor *_t = static_cast<CDLNAAdaptor *>(_o);
        switch (_id) {
        case 0: _t->TimePosition((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 1: _t->DLNAConnStatus((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 2: _t->CurrentSongInfo((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3]))); break;
        case 3: _t->CurrentVideoInfo((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const ushort(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 4: _t->DeckStatus((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 5: _t->TrackPosition((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 6: _t->TrackInformation((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 7: _t->TimePositionSlot((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 8: _t->DLNAConnStatusSlot((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 9: _t->CurrentSongInfoSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3]))); break;
        case 10: _t->CurrentVideoInfoSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const ushort(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 11: _t->DeckStatusSlot((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 12: _t->TrackPositionSlot((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 13: _t->TrackInformationSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 14: { bool _r = _t->SetDeckStatus((*reinterpret_cast< const QDBusMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 15: { bool _r = _t->SetTrackPosition((*reinterpret_cast< const QDBusMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 16: { bool _r = _t->SetIncrementTrackPosition((*reinterpret_cast< const uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 17: { bool _r = _t->SetDecrementTrackPosition((*reinterpret_cast< const uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CDLNAAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CDLNAAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CDLNAAdaptor,
      qt_meta_data_CDLNAAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CDLNAAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CDLNAAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CDLNAAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDLNAAdaptor))
        return static_cast<void*>(const_cast< CDLNAAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int CDLNAAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void CDLNAAdaptor::TimePosition(const int _t1, const int _t2, const int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CDLNAAdaptor::DLNAConnStatus(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CDLNAAdaptor::CurrentSongInfo(const uint _t1, const ushort _t2, const QStringList _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CDLNAAdaptor::CurrentVideoInfo(const QString _t1, const uint _t2, const ushort _t3, const QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CDLNAAdaptor::DeckStatus(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CDLNAAdaptor::TrackPosition(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CDLNAAdaptor::TrackInformation(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
static const uint qt_meta_data_CHMIAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      84,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      27,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   13,   12,   12, 0x05,
      71,   51,   12,   12, 0x05,
     108,   13,   12,   12, 0x05,
     148,  132,   12,   12, 0x05,
     194,  178,   12,   12, 0x05,
     237,  224,   12,   12, 0x05,
     274,  267,   12,   12, 0x05,
     309,  292,   12,   12, 0x05,
     353,  340,   12,   12, 0x05,
     397,  381,   12,   12, 0x05,
     442,  431,   12,   12, 0x05,
     477,  467,   12,   12, 0x05,
     513,  497,   12,   12, 0x05,
     544,  539,   12,   12, 0x05,
     610,  575,   12,   12, 0x05,
     683,  648,   12,   12, 0x05,
     735,  722,   12,   12, 0x05,
     767,  761,   12,   12, 0x05,
     805,  791,   12,   12, 0x05,
     864,  834,   12,   12, 0x05,
     923,  904,   12,   12, 0x05,
     969,  957,   12,   12, 0x05,
    1030,  995,   12,   12, 0x05,
    1069, 1063,   12,   12, 0x05,
    1138, 1099,   12,   12, 0x05,
    1187,  904,   12,   12, 0x05,
    1232, 1221,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
    1252,   13,   12,   12, 0x08,
    1282,   51,   12,   12, 0x08,
    1323,   13,   12,   12, 0x08,
    1351,  132,   12,   12, 0x08,
    1385,  178,   12,   12, 0x08,
    1419,  224,   12,   12, 0x08,
    1453,  267,   12,   12, 0x08,
    1475,  292,   12,   12, 0x08,
    1510,  340,   12,   12, 0x08,
    1544,  381,   12,   12, 0x08,
    1582,  431,   12,   12, 0x08,
    1611,  467,   12,   12, 0x08,
    1635,  497,   12,   12, 0x08,
    1665,  539,   12,   12, 0x08,
    1700,  575,   12,   12, 0x08,
    1742,  648,   12,   12, 0x08,
    1785,  722,   12,   12, 0x08,
    1815,  761,   12,   12, 0x08,
    1843,  791,   12,   12, 0x08,
    1876,  834,   12,   12, 0x08,
    1922,  904,   12,   12, 0x08,
    1960,  957,   12,   12, 0x08,
    1990,  995,   12,   12, 0x08,
    2027, 1063,   12,   12, 0x08,
    2061, 1099,   12,   12, 0x08,
    2122, 1221,   12,   12, 0x08,
    2170, 2151, 2146,   12, 0x0a,
    2230, 2191, 2146,   12, 0x0a,
    2280, 2262, 2146,   12, 0x0a,
    2320, 2309, 2146,   12, 0x0a,
    2346, 2342, 2146,   12, 0x0a,
    2378, 2342, 2146,   12, 0x0a,
    2408, 2342, 2146,   12, 0x0a,
    2437, 2342, 2146,   12, 0x0a,
    2473, 2342, 2146,   12, 0x0a,
    2514, 2342, 2146,   12, 0x0a,
    2550, 2342, 2146,   12, 0x0a,
    2591,  575, 2146,   12, 0x0a,
    2618,  648, 2146,   12, 0x0a,
    2652,  722, 2146,   12, 0x0a,
    2673,  761, 2146,   12, 0x0a,
    2692,  224, 2146,   12, 0x0a,
    2718,  267, 2146,   12, 0x0a,
    2739,  791, 2146,   12, 0x0a,
    2776, 2763, 2146,   12, 0x0a,
    2798,  292, 2146,   12, 0x0a,
    2834, 2342, 2146,   12, 0x0a,
    2869,  957, 2146,   12, 0x0a,
    2890,  497, 2146,   12, 0x0a,
    2947, 2919, 2146,   12, 0x0a,
    3010, 2989, 2146,   12, 0x0a,
    3043,  995, 2146,   12, 0x0a,
    3093, 3079, 2146,   12, 0x0a,
    3132, 3116, 2146,   12, 0x0a,
    3163,  340, 2146,   12, 0x0a,
    3189, 2342, 2146,   12, 0x0a,
    3223, 2342, 2146,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CHMIAdaptor[] = {
    "CHMIAdaptor\0\0target,mode\0"
    "ModeStatusIBOX(uint,uint)\0errorCode,errorInfo\0"
    "ModeStatusErrorIBOX(uint,QByteArray)\0"
    "ModeStatusHU(uint,uint)\0vTarget,vSource\0"
    "SystemVisibilityHU(uint,uint)\0"
    "aTarget,aSource\0SystemAudibilityHU(uint,uint)\0"
    "keyBoardData\0KeyBoardSetting(QDBusVariant)\0"
    "status\0Progressbar(uint)\0popUpRequestData\0"
    "PopUpRequestToHU(QDBusVariant)\0"
    "senderHandle\0AbortPopUpRequestToHU(uint)\0"
    "popUpResultData\0PopUpResultFromIBOX(QDBusVariant)\0"
    "displayOSD\0DisplayOSD(QDBusVariant)\0"
    "statusbar\0ShowStatusBar(uint)\0"
    "hardKey,softKey\0KeyInputStatus(uint,uint)\0"
    "need\0DrivingRestrictionStatus(uint)\0"
    "brightness,saturation,contrast,hue\0"
    "AdjustScreenFromIBOX(int,int,int,int)\0"
    "bTarget,lcdFront,lcdRear1,lcdRear2\0"
    "LCDBrightnessFromIBOX(int,int,int,int)\0"
    "illumination\0IlluminationFromIBOX(int)\0"
    "ratio\0VideoRatioFromIBOX(int)\0"
    "displayStatus\0DisplayControlFromIBOX(uint)\0"
    "deviceID,senderHandle,sTarget\0"
    "VideoDefaultRequestIBOX(uint,uint,uint)\0"
    "state,senderHandle\0VideoDefaultResultIBOX(uint,uint)\0"
    "beepSetting\0BeepSettingFromIBOX(uint)\0"
    "emgcState,controllerLock,soundLock\0"
    "EmergencySetIBOX(uint,uint,uint)\0state\0"
    "HMIFunctionBlockVisible(bool)\0"
    "state,senderHandle,errorCode,errorInfo\0"
    "SystemDefaultSetError(uint,uint,uint,QByteArray)\0"
    "SystemDefaultSetResult(uint,uint)\0"
    "muteStatus\0MuteStatusSet(bool)\0"
    "ModeStatusIBOXSlot(uint,uint)\0"
    "ModeStatusErrorIBOXSlot(uint,QByteArray)\0"
    "ModeStatusHUSlot(uint,uint)\0"
    "SystemVisibilityHUSlot(uint,uint)\0"
    "SystemAudibilityHUSlot(uint,uint)\0"
    "KeyBoardSettingSlot(QDBusVariant)\0"
    "ProgressbarSlot(uint)\0"
    "PopUpRequestToHUSlot(QDBusVariant)\0"
    "AbortPopUpRequestToHUSlot(ushort)\0"
    "PopUpResultFromIBOXSlot(QDBusVariant)\0"
    "DisplayOSDSlot(QDBusVariant)\0"
    "ShowStatusBarSlot(uint)\0"
    "KeyInputStatusSlot(uint,uint)\0"
    "DrivingRestrictionStatusSlot(uint)\0"
    "AdjustScreenFromIBOXSlot(int,int,int,int)\0"
    "LCDBrightnessFromIBOXSlot(int,int,int,int)\0"
    "IlluminationFromIBOXSlot(int)\0"
    "VideoRatioFromIBOXSlot(int)\0"
    "DisplayControlFromIBOXSlot(uint)\0"
    "VideoDefaultRequestIBOXSlot(uint,ushort,uint)\0"
    "VideoDefaultResultIBOXSlot(uint,uint)\0"
    "BeepSettingFromIBOXSlot(uint)\0"
    "EmergencySetIBOXSlot(uint,uint,uint)\0"
    "HMIFunctionBlockVisibleSlot(bool)\0"
    "SystemDefaultSetResultSlot(uint,uint16_t,uint8_t,QByteArray)\0"
    "MuteStatusSetSlot(bool)\0bool\0"
    "systemLanguageCode\0SetLanguage(QString)\0"
    "sourceType,defaultVolume,currentVolume\0"
    "SetVolumeStatus(uint,uint,uint)\0"
    "time,timeZone,dst\0SetTime(QByteArray,int,uint)\0"
    "systemDate\0SetDate(QDBusVariant)\0msg\0"
    "SetModeStatusIBOX(QDBusMessage)\0"
    "SetModeStatusHU(QDBusMessage)\0"
    "SetSoundStatus(QDBusMessage)\0"
    "SetSystemVisibilityHU(QDBusMessage)\0"
    "SetSystemVisibilityHUError(QDBusMessage)\0"
    "SetSystemAudibilityHU(QDBusMessage)\0"
    "SetSystemAudibilityHUError(QDBusMessage)\0"
    "SetScreen(int,int,int,int)\0"
    "SetLCDBrightness(int,int,int,int)\0"
    "SetIllumination(int)\0SetVideoRatio(int)\0"
    "SetKeyBoard(QDBusVariant)\0"
    "SetProgressbar(uint)\0SetDisplayControl(uint)\0"
    "distanceUnit\0SetDistanceUnit(uint)\0"
    "SetPopUpRequestToIBOX(QDBusVariant)\0"
    "SetPopUpResultFromHU(QDBusMessage)\0"
    "SetBeepSetting(uint)\0SetKeyInputStatus(uint,uint)\0"
    "state,deviceID,senderHandle\0"
    "SetVideoDefaultResultIBOX(uint,uint,uint)\0"
    "senderHandle,sTarget\0"
    "SetVideoDefaulttoIBOX(uint,uint)\0"
    "SetEmergencySetIBOX(uint,uint,uint)\0"
    "phonePriority\0SetPhonePriority(uint)\0"
    "osdId,osdStatus\0SetDisplayOSDStatus(uint,uint)\0"
    "SetSystemDefaultSet(uint)\0"
    "SetMuteStatusStatus(QDBusMessage)\0"
    "SetMuteStatusError(QDBusMessage)\0"
};

void CHMIAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CHMIAdaptor *_t = static_cast<CHMIAdaptor *>(_o);
        switch (_id) {
        case 0: _t->ModeStatusIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 1: _t->ModeStatusErrorIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 2: _t->ModeStatusHU((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 3: _t->SystemVisibilityHU((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 4: _t->SystemAudibilityHU((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 5: _t->KeyBoardSetting((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 6: _t->Progressbar((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 7: _t->PopUpRequestToHU((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 8: _t->AbortPopUpRequestToHU((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 9: _t->PopUpResultFromIBOX((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 10: _t->DisplayOSD((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 11: _t->ShowStatusBar((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 12: _t->KeyInputStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 13: _t->DrivingRestrictionStatus((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 14: _t->AdjustScreenFromIBOX((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 15: _t->LCDBrightnessFromIBOX((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 16: _t->IlluminationFromIBOX((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 17: _t->VideoRatioFromIBOX((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 18: _t->DisplayControlFromIBOX((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 19: _t->VideoDefaultRequestIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 20: _t->VideoDefaultResultIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 21: _t->BeepSettingFromIBOX((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 22: _t->EmergencySetIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 23: _t->HMIFunctionBlockVisible((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 24: _t->SystemDefaultSetError((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 25: _t->SystemDefaultSetResult((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 26: _t->MuteStatusSet((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 27: _t->ModeStatusIBOXSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 28: _t->ModeStatusErrorIBOXSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 29: _t->ModeStatusHUSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 30: _t->SystemVisibilityHUSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 31: _t->SystemAudibilityHUSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 32: _t->KeyBoardSettingSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 33: _t->ProgressbarSlot((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 34: _t->PopUpRequestToHUSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 35: _t->AbortPopUpRequestToHUSlot((*reinterpret_cast< ushort(*)>(_a[1]))); break;
        case 36: _t->PopUpResultFromIBOXSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 37: _t->DisplayOSDSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 38: _t->ShowStatusBarSlot((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 39: _t->KeyInputStatusSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 40: _t->DrivingRestrictionStatusSlot((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 41: _t->AdjustScreenFromIBOXSlot((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 42: _t->LCDBrightnessFromIBOXSlot((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 43: _t->IlluminationFromIBOXSlot((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 44: _t->VideoRatioFromIBOXSlot((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 45: _t->DisplayControlFromIBOXSlot((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 46: _t->VideoDefaultRequestIBOXSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 47: _t->VideoDefaultResultIBOXSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 48: _t->BeepSettingFromIBOXSlot((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 49: _t->EmergencySetIBOXSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 50: _t->HMIFunctionBlockVisibleSlot((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 51: _t->SystemDefaultSetResultSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint16_t(*)>(_a[2])),(*reinterpret_cast< const uint8_t(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 52: _t->MuteStatusSetSlot((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 53: { bool _r = _t->SetLanguage((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 54: { bool _r = _t->SetVolumeStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 55: { bool _r = _t->SetTime((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 56: { bool _r = _t->SetDate((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 57: { bool _r = _t->SetModeStatusIBOX((*reinterpret_cast< const QDBusMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 58: { bool _r = _t->SetModeStatusHU((*reinterpret_cast< const QDBusMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 59: { bool _r = _t->SetSoundStatus((*reinterpret_cast< const QDBusMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 60: { bool _r = _t->SetSystemVisibilityHU((*reinterpret_cast< const QDBusMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 61: { bool _r = _t->SetSystemVisibilityHUError((*reinterpret_cast< const QDBusMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 62: { bool _r = _t->SetSystemAudibilityHU((*reinterpret_cast< const QDBusMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 63: { bool _r = _t->SetSystemAudibilityHUError((*reinterpret_cast< const QDBusMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 64: { bool _r = _t->SetScreen((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 65: { bool _r = _t->SetLCDBrightness((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 66: { bool _r = _t->SetIllumination((*reinterpret_cast< const int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 67: { bool _r = _t->SetVideoRatio((*reinterpret_cast< const int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 68: { bool _r = _t->SetKeyBoard((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 69: { bool _r = _t->SetProgressbar((*reinterpret_cast< const uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 70: { bool _r = _t->SetDisplayControl((*reinterpret_cast< const uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 71: { bool _r = _t->SetDistanceUnit((*reinterpret_cast< const uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 72: { bool _r = _t->SetPopUpRequestToIBOX((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 73: { bool _r = _t->SetPopUpResultFromHU((*reinterpret_cast< const QDBusMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 74: { bool _r = _t->SetBeepSetting((*reinterpret_cast< const uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 75: { bool _r = _t->SetKeyInputStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 76: { bool _r = _t->SetVideoDefaultResultIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 77: { bool _r = _t->SetVideoDefaulttoIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 78: { bool _r = _t->SetEmergencySetIBOX((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 79: { bool _r = _t->SetPhonePriority((*reinterpret_cast< const uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 80: { bool _r = _t->SetDisplayOSDStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 81: { bool _r = _t->SetSystemDefaultSet((*reinterpret_cast< const uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 82: { bool _r = _t->SetMuteStatusStatus((*reinterpret_cast< const QDBusMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 83: { bool _r = _t->SetMuteStatusError((*reinterpret_cast< const QDBusMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CHMIAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CHMIAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CHMIAdaptor,
      qt_meta_data_CHMIAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CHMIAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CHMIAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CHMIAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CHMIAdaptor))
        return static_cast<void*>(const_cast< CHMIAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int CHMIAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 84)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 84;
    }
    return _id;
}

// SIGNAL 0
void CHMIAdaptor::ModeStatusIBOX(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CHMIAdaptor::ModeStatusErrorIBOX(const uint _t1, const QByteArray _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CHMIAdaptor::ModeStatusHU(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CHMIAdaptor::SystemVisibilityHU(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CHMIAdaptor::SystemAudibilityHU(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CHMIAdaptor::KeyBoardSetting(const QDBusVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CHMIAdaptor::Progressbar(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CHMIAdaptor::PopUpRequestToHU(const QDBusVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CHMIAdaptor::AbortPopUpRequestToHU(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CHMIAdaptor::PopUpResultFromIBOX(const QDBusVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CHMIAdaptor::DisplayOSD(const QDBusVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CHMIAdaptor::ShowStatusBar(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CHMIAdaptor::KeyInputStatus(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CHMIAdaptor::DrivingRestrictionStatus(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CHMIAdaptor::AdjustScreenFromIBOX(const int _t1, const int _t2, const int _t3, const int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CHMIAdaptor::LCDBrightnessFromIBOX(const int _t1, const int _t2, const int _t3, const int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void CHMIAdaptor::IlluminationFromIBOX(const int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void CHMIAdaptor::VideoRatioFromIBOX(const int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void CHMIAdaptor::DisplayControlFromIBOX(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void CHMIAdaptor::VideoDefaultRequestIBOX(const uint _t1, const uint _t2, const uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void CHMIAdaptor::VideoDefaultResultIBOX(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void CHMIAdaptor::BeepSettingFromIBOX(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void CHMIAdaptor::EmergencySetIBOX(const uint _t1, const uint _t2, const uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void CHMIAdaptor::HMIFunctionBlockVisible(const bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void CHMIAdaptor::SystemDefaultSetError(const uint _t1, const uint _t2, const uint _t3, const QByteArray _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void CHMIAdaptor::SystemDefaultSetResult(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void CHMIAdaptor::MuteStatusSet(const bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}
static const uint qt_meta_data_CIBoxAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      44,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      18,       // signalCount

 // signals: signature, parameters, type, tag, flags
      32,   14,   13,   13, 0x05,
      74,   52,   13,   13, 0x05,
     116,  102,   13,   13, 0x05,
     167,  147,   13,   13, 0x05,
     219,  200,   13,   13, 0x05,
     269,  255,   13,   13, 0x05,
     314,  300,   13,   13, 0x05,
     370,  349,   13,   13, 0x05,
     425,  406,   13,   13, 0x05,
     462,  448,   13,   13, 0x05,
     516,  497,   13,   13, 0x05,
     586,  544,   13,   13, 0x05,
     638,  631,   13,   13, 0x05,
     692,  671,   13,   13, 0x05,
     729,  147,   13,   13, 0x05,
     791,  763,   13,   13, 0x05,
     857,  819,   13,   13, 0x05,
     926,  908,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     945,   14,   13,   13, 0x08,
     969,   52,   13,   13, 0x08,
    1001,  102,   13,   13, 0x08,
    1036,  147,   13,   13, 0x08,
    1073,  200,   13,   13, 0x08,
    1115,  255,   13,   13, 0x08,
    1150,  300,   13,   13, 0x08,
    1189,  349,   13,   13, 0x08,
    1229,  406,   13,   13, 0x08,
    1256,  448,   13,   13, 0x08,
    1295,  497,   13,   13, 0x08,
    1327,  544,   13,   13, 0x08,
    1378,  631,   13,   13, 0x08,
    1415,  671,   13,   13, 0x08,
    1456,  147,   13,   13, 0x08,
    1492,  763,   13,   13, 0x08,
    1526,  819,   13,   13, 0x08,
    1583,  908,   13,   13, 0x08,
    1615, 1611, 1606,   13, 0x0a,
    1668, 1653, 1606,   13, 0x0a,
    1717, 1698, 1606,   13, 0x0a,
    1765, 1751, 1606,   13, 0x0a,
    1811, 1799, 1606,   13, 0x0a,
    1838,   13, 1606,   13, 0x0a,
    1883, 1852, 1606,   13, 0x0a,
    1957, 1926, 1606,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CIBoxAdaptor[] = {
    "CIBoxAdaptor\0\0mode,clientStatus\0"
    "WifiMode(uint,uint)\0appStatus,appProgress\0"
    "AppInstallStatus(uint,uint)\0uri,available\0"
    "GISDataAvailable(QString,uint)\0"
    "errorCode,errorInfo\0"
    "GISDataAvailableError(uint,uint)\0"
    "senderHandle,state\0"
    "OpenBrowserRequestResult(uint,uint)\0"
    "gisResultData\0GISRequestResult(QDBusVariant)\0"
    "gisDataResult\0GISDataRequestResult(QDBusVariant)\0"
    "target,available,uri\0"
    "POIDataAvailable(uint,uint,QString)\0"
    "available,poiCount\0ReceivedPOI(uint,uint)\0"
    "poiDataResult\0POIDataRequestResult(QDBusVariant)\0"
    "available,pofCount\0POFDataAvailable(uint,uint)\0"
    "state,senderHandle,blockType,blockCounter\0"
    "VoiceSearchOnlineStatus(uint,uint,uint,uint)\0"
    "status\0IBOXFunctionBlockInvisible(bool)\0"
    "optype,uri,available\0"
    "onLSDataAvailable(uint,QString,uint)\0"
    "onLSDataAvailableError(uint,uint)\0"
    "optype,senderHandle,success\0"
    "onLSRequest(uint,uint,uint)\0"
    "optype,senderHandle,target,uri,lsdata\0"
    "onLSDataRequest(uint,uint,uint,QString,QByteArray)\0"
    "ACNActivatedValue\0ACNActivated(bool)\0"
    "WifiModeSlot(uint,uint)\0"
    "AppInstallStatusSlot(uint,uint)\0"
    "GISDataAvailableSlot(QString,uint)\0"
    "GISDataAvailableErrorSlot(uint,uint)\0"
    "OpenBrowserRequestResultSlot(ushort,uint)\0"
    "GISRequestResultSlot(QDBusVariant)\0"
    "GISDataRequestResultSlot(QDBusVariant)\0"
    "POIDataAvailableSlot(uint,uint,QString)\0"
    "ReceivedPOISlot(uint,uint)\0"
    "POIDataRequestResultSlot(QDBusVariant)\0"
    "POFDataAvailableSlot(uint,uint)\0"
    "VoiceSearchOnlineStatusSlot(uint,ushort,uint,uint)\0"
    "IBOXFunctionBlockInvisibleSlot(bool)\0"
    "LSDataAvailableSlot(uchar,QString,uchar)\0"
    "LSDataAvailableErrorSlot(uint,uint)\0"
    "LSRequestSlot(uchar,ushort,uchar)\0"
    "LSDataRequestSlot(uchar,ushort,uchar,QString,QByteArray)\0"
    "ACNActivatedSlot(bool)\0bool\0msg\0"
    "StartOpenBrowserRequest(QDBusMessage)\0"
    "gisRequestData\0StartGISRequest(QDBusVariant)\0"
    "gisDataRequestData\0StartGISDataRequest(QDBusVariant)\0"
    "poiDataRquest\0StartPOIDataRequest(QDBusVariant)\0"
    "voiceSearch\0SetVoiceSearchOnline(uint)\0"
    "GetWiFiMode()\0optype,senderHandle,uri,lsdata\0"
    "SetLSRequest(uint,uint,QString,QByteArray)\0"
    "optype,senderHandle,target,uri\0"
    "SetLSDataRequest(uint,uint,uint,QString)\0"
};

void CIBoxAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CIBoxAdaptor *_t = static_cast<CIBoxAdaptor *>(_o);
        switch (_id) {
        case 0: _t->WifiMode((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 1: _t->AppInstallStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 2: _t->GISDataAvailable((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 3: _t->GISDataAvailableError((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 4: _t->OpenBrowserRequestResult((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 5: _t->GISRequestResult((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 6: _t->GISDataRequestResult((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 7: _t->POIDataAvailable((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 8: _t->ReceivedPOI((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 9: _t->POIDataRequestResult((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 10: _t->POFDataAvailable((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 11: _t->VoiceSearchOnlineStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])),(*reinterpret_cast< const uint(*)>(_a[4]))); break;
        case 12: _t->IBOXFunctionBlockInvisible((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 13: _t->onLSDataAvailable((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 14: _t->onLSDataAvailableError((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 15: _t->onLSRequest((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 16: _t->onLSDataRequest((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< QByteArray(*)>(_a[5]))); break;
        case 17: _t->ACNActivated((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 18: _t->WifiModeSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 19: _t->AppInstallStatusSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 20: _t->GISDataAvailableSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 21: _t->GISDataAvailableErrorSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 22: _t->OpenBrowserRequestResultSlot((*reinterpret_cast< const ushort(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 23: _t->GISRequestResultSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 24: _t->GISDataRequestResultSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 25: _t->POIDataAvailableSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 26: _t->ReceivedPOISlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 27: _t->POIDataRequestResultSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 28: _t->POFDataAvailableSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 29: _t->VoiceSearchOnlineStatusSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])),(*reinterpret_cast< const uint(*)>(_a[4]))); break;
        case 30: _t->IBOXFunctionBlockInvisibleSlot((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 31: _t->LSDataAvailableSlot((*reinterpret_cast< const uchar(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const uchar(*)>(_a[3]))); break;
        case 32: _t->LSDataAvailableErrorSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 33: _t->LSRequestSlot((*reinterpret_cast< const uchar(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const uchar(*)>(_a[3]))); break;
        case 34: _t->LSDataRequestSlot((*reinterpret_cast< const uchar(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const uchar(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< QByteArray(*)>(_a[5]))); break;
        case 35: _t->ACNActivatedSlot((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 36: { bool _r = _t->StartOpenBrowserRequest((*reinterpret_cast< const QDBusMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 37: { bool _r = _t->StartGISRequest((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 38: { bool _r = _t->StartGISDataRequest((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 39: { bool _r = _t->StartPOIDataRequest((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 40: { bool _r = _t->SetVoiceSearchOnline((*reinterpret_cast< const uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 41: { bool _r = _t->GetWiFiMode();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 42: { bool _r = _t->SetLSRequest((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 43: { bool _r = _t->SetLSDataRequest((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CIBoxAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CIBoxAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CIBoxAdaptor,
      qt_meta_data_CIBoxAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CIBoxAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CIBoxAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CIBoxAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CIBoxAdaptor))
        return static_cast<void*>(const_cast< CIBoxAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int CIBoxAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 44)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 44;
    }
    return _id;
}

// SIGNAL 0
void CIBoxAdaptor::WifiMode(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CIBoxAdaptor::AppInstallStatus(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CIBoxAdaptor::GISDataAvailable(const QString _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CIBoxAdaptor::GISDataAvailableError(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CIBoxAdaptor::OpenBrowserRequestResult(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CIBoxAdaptor::GISRequestResult(const QDBusVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CIBoxAdaptor::GISDataRequestResult(const QDBusVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CIBoxAdaptor::POIDataAvailable(const uint _t1, const uint _t2, const QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CIBoxAdaptor::ReceivedPOI(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CIBoxAdaptor::POIDataRequestResult(const QDBusVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CIBoxAdaptor::POFDataAvailable(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CIBoxAdaptor::VoiceSearchOnlineStatus(const uint _t1, const uint _t2, const uint _t3, const uint _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CIBoxAdaptor::IBOXFunctionBlockInvisible(const bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CIBoxAdaptor::onLSDataAvailable(const uint _t1, const QString _t2, const uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CIBoxAdaptor::onLSDataAvailableError(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CIBoxAdaptor::onLSRequest(const uint _t1, const uint _t2, const uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void CIBoxAdaptor::onLSDataRequest(const uint _t1, const uint _t2, const uint _t3, const QString _t4, QByteArray _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void CIBoxAdaptor::ACNActivated(const bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}
static const uint qt_meta_data_CNavigationAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      53,   20,   19,   19, 0x05,
     125,   93,   19,   19, 0x05,
     207,  167,   19,   19, 0x05,
     255,  243,   19,   19, 0x05,
     291,  283,   19,   19, 0x05,
     334,  315,   19,   19, 0x05,
     397,  369,   19,   19, 0x05,
     447,  430,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
     500,  489,  484,   19, 0x0a,
     545,  528,  484,   19, 0x0a,
     605,  579,  484,   19, 0x0a,
     647,  627,  484,   19, 0x0a,
     680,  627,  484,   19, 0x0a,
     738,  710,  484,   19, 0x0a,
     767,  710,  484,   19, 0x0a,
     798,  710,  484,   19, 0x0a,
     825,  710,  484,   19, 0x0a,
     887,  863,  484,   19, 0x0a,
     930,  710,  484,   19, 0x0a,
     960,  710,  484,   19, 0x0a,
     996,   20,   19,   19, 0x08,
    1041,   93,   19,   19, 0x08,
    1089,  167,   19,   19, 0x08,
    1131,  243,   19,   19, 0x08,
    1161,  283,   19,   19, 0x08,
    1187,  315,   19,   19, 0x08,
    1224,  369,   19,   19, 0x08,
    1259,  430,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CNavigationAdaptor[] = {
    "CNavigationAdaptor\0\0"
    "optype,senderHandle,inputKeyword\0"
    "onRequestLocalSearch(uint,uint,QString)\0"
    "optype,senderHandle,target,text\0"
    "onOpenLocalSearch(uint,uint,uint,QString)\0"
    "optype,senderHandle,target,inputKeyword\0"
    "onSearchPOI(uint,uint,uint,QString)\0"
    "searchRoute\0onSearchRoute(QDBusVariant)\0"
    "showMap\0onShowMap(QDBusVariant)\0"
    "registerMyLocation\0"
    "onRegisterMyLocation(QDBusVariant)\0"
    "senderHandle,target,poiKind\0"
    "onFindNearBySlot(uint,uint,uint)\0"
    "checkGPSBoundary\0onCheckGPSBoundarySlot(QDBusVariant)\0"
    "bool\0gpsRawData\0SetGPSRawData(QDBusVariant)\0"
    "gpsRawDataBessel\0SetGPSRawDataBessel(QDBusVariant)\0"
    "dataAvailable,mapDataType\0"
    "SetMapData(uint,uint)\0optype,senderHandle\0"
    "SetRequestLocalSearch(uint,uint)\0"
    "SetOpenLocalSearch(uint,uint)\0"
    "optype,senderHandle,success\0"
    "SetSearchPOI(uint,uint,uint)\0"
    "SetSearchRoute(uint,uint,uint)\0"
    "SetShowMap(uint,uint,uint)\0"
    "SetRegisterMyLocation(uint,uint,uint)\0"
    "optype,routeSet,poiData\0"
    "SetDestinationInfo(uint,uint,QDBusVariant)\0"
    "SetFindNearBy(uint,uint,uint)\0"
    "SetCheckGPSBoundary(uint,uint,uint)\0"
    "RequestLocalSearchSlot(uchar,ushort,QString)\0"
    "OpenLocalSearchSlot(uchar,ushort,uchar,QString)\0"
    "SearchPOISlot(uchar,ushort,uchar,QString)\0"
    "SearchRouteSlot(QDBusVariant)\0"
    "ShowMapSlot(QDBusVariant)\0"
    "RegisterMyLocationSlot(QDBusVariant)\0"
    "FindNearBySlot(ushort,uchar,uchar)\0"
    "CheckGPSBoundarySlot(QDBusVariant)\0"
};

void CNavigationAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CNavigationAdaptor *_t = static_cast<CNavigationAdaptor *>(_o);
        switch (_id) {
        case 0: _t->onRequestLocalSearch((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: _t->onOpenLocalSearch((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 2: _t->onSearchPOI((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 3: _t->onSearchRoute((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 4: _t->onShowMap((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 5: _t->onRegisterMyLocation((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 6: _t->onFindNearBySlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 7: _t->onCheckGPSBoundarySlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 8: { bool _r = _t->SetGPSRawData((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: { bool _r = _t->SetGPSRawDataBessel((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { bool _r = _t->SetMapData((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: { bool _r = _t->SetRequestLocalSearch((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 12: { bool _r = _t->SetOpenLocalSearch((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 13: { bool _r = _t->SetSearchPOI((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 14: { bool _r = _t->SetSearchRoute((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 15: { bool _r = _t->SetShowMap((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 16: { bool _r = _t->SetRegisterMyLocation((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 17: { bool _r = _t->SetDestinationInfo((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const QDBusVariant(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 18: { bool _r = _t->SetFindNearBy((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 19: { bool _r = _t->SetCheckGPSBoundary((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 20: _t->RequestLocalSearchSlot((*reinterpret_cast< const uchar(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 21: _t->OpenLocalSearchSlot((*reinterpret_cast< const uchar(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const uchar(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 22: _t->SearchPOISlot((*reinterpret_cast< const uchar(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const uchar(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 23: _t->SearchRouteSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 24: _t->ShowMapSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 25: _t->RegisterMyLocationSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 26: _t->FindNearBySlot((*reinterpret_cast< const ushort(*)>(_a[1])),(*reinterpret_cast< const uchar(*)>(_a[2])),(*reinterpret_cast< const uchar(*)>(_a[3]))); break;
        case 27: _t->CheckGPSBoundarySlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CNavigationAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CNavigationAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CNavigationAdaptor,
      qt_meta_data_CNavigationAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CNavigationAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CNavigationAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CNavigationAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CNavigationAdaptor))
        return static_cast<void*>(const_cast< CNavigationAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int CNavigationAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    return _id;
}

// SIGNAL 0
void CNavigationAdaptor::onRequestLocalSearch(const uint _t1, const uint _t2, const QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CNavigationAdaptor::onOpenLocalSearch(const uint _t1, const uint _t2, const uint _t3, const QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CNavigationAdaptor::onSearchPOI(const uint _t1, const uint _t2, const uint _t3, const QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CNavigationAdaptor::onSearchRoute(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CNavigationAdaptor::onShowMap(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CNavigationAdaptor::onRegisterMyLocation(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CNavigationAdaptor::onFindNearBySlot(const uint _t1, const uint _t2, const uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CNavigationAdaptor::onCheckGPSBoundarySlot(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
static const uint qt_meta_data_CSWDLAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      53,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      18,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   14,   13,   13, 0x05,
      83,   65,   13,   13, 0x05,
     126,  109,   13,   13, 0x05,
     173,  154,   13,   13, 0x05,
     230,  209,   13,   13, 0x05,
     285,  267,   13,   13, 0x05,
     328,  317,   13,   13, 0x05,
     371,  359,   13,   13, 0x05,
     413,  397,   13,   13, 0x05,
     463,  443,   13,   13, 0x05,
     513,  497,   13,   13, 0x05,
     559,  543,   13,   13, 0x05,
     616,  589,   13,   13, 0x05,
     680,  660,   13,   13, 0x05,
     734,  714,   13,   13, 0x05,
     799,  768,   13,   13, 0x05,
     852,  832,   13,   13, 0x05,
     897,  891,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     925,   14,   13,   13, 0x08,
     960,   65,   13,   13, 0x08,
     990,  109,   13,   13, 0x08,
    1022,  154,   13,   13, 0x08,
    1062,  209,   13,   13, 0x08,
    1103,  267,   13,   13, 0x08,
    1139,  317,   13,   13, 0x08,
    1174,  359,   13,   13, 0x08,
    1204,  397,   13,   13, 0x08,
    1258, 1238,   13,   13, 0x08,
    1296,  543,   13,   13, 0x08,
    1330,  497,   13,   13, 0x08,
    1364,  589,   13,   13, 0x08,
    1412,  660,   13,   13, 0x08,
    1450,  714,   13,   13, 0x08,
    1488,  768,   13,   13, 0x08,
    1525,  832,   13,   13, 0x08,
    1568,  891,   13,   13, 0x08,
    1631, 1605, 1600,   13, 0x0a,
    1670,   65, 1600,   13, 0x0a,
    1699,  109, 1600,   13, 0x0a,
    1730,  154, 1600,   13, 0x0a,
    1769,  209, 1600,   13, 0x0a,
    1833, 1809, 1600,   13, 0x0a,
    1864,  267, 1600,   13, 0x0a,
    1899,  317, 1600,   13, 0x0a,
    1933,  359, 1600,   13, 0x0a,
    1962,  397, 1600,   13, 0x0a,
    1995,  443, 1600,   13, 0x0a,
    2041, 2032, 1600,   13, 0x0a,
    2082,  497, 1600,   13, 0x0a,
    2115,  543, 1600,   13, 0x0a,
    2148,  660, 1600,   13, 0x0a,
    2185,  714, 1600,   13, 0x0a,
    2235, 2222, 1600,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CSWDLAdaptor[] = {
    "CSWDLAdaptor\0\0swdlid,hwidentifier\0"
    "ReadHWIdentifier(uint,QString)\0"
    "swdlid,deviceName\0ReadDevName(uint,QString)\0"
    "swdlid,hwversion\0ReadHWVersion(uint,QString)\0"
    "swdlid,modulevalue\0"
    "ReadNumberofModule(uint,QByteArray)\0"
    "swdlid,moduleversion\0"
    "ReadModuleVersion(uint,QDBusVariant)\0"
    "reprogrammingmode\0ReprogrammingMode(QDBusVariant)\0"
    "initUpdate\0InitUpdateResult(QDBusVariant)\0"
    "eraseMemory\0EraseMemory(QDBusVariant)\0"
    "requestdownload\0RequestDownload(QDBusVariant)\0"
    "PrepareFileTransfer\0"
    "PrepareFileTransfer(QDBusVariant)\0"
    "transferdataAMS\0TransferDataCMS(QDBusVariant)\0"
    "transferdataCMS\0TransferDataAMS(QDBusVariant)\0"
    "swdlid,errorCode,errorInfo\0"
    "TransferDataAMSError(uint,uchar,QByteArray)\0"
    "checkactivatemodule\0"
    "CheckActivateModule(QDBusVariant)\0"
    "requesttransferexit\0"
    "RequestTransferExit(QDBusVariant)\0"
    "swdlid,swdlOpMode,swdlProgress\0"
    "SWDLProgressInfo(uint,uint,uint)\0"
    "swdlid,updateStatus\0"
    "UpdateProgressingInfoStatus(uint,uint)\0"
    "state\0iBoxSWDLFBlockVisible(bool)\0"
    "ReadHWIdentifierSlot(uint,QString)\0"
    "ReadDevNameSlot(uint,QString)\0"
    "ReadHWVersionSlot(uint,QString)\0"
    "ReadNumberofModuleSlot(uint,QByteArray)\0"
    "ReadModuleVersionSlot(uint,QDBusVariant)\0"
    "ReprogrammingModeSlot(QDBusVariant)\0"
    "InitUpdateResultSlot(QDBusVariant)\0"
    "EraseMemorySlot(QDBusVariant)\0"
    "RequestDownloadSlot(QDBusVariant)\0"
    "preparefiletransfer\0"
    "PrepareFileTransferSlot(QDBusVariant)\0"
    "TransferDataCMSSlot(QDBusVariant)\0"
    "TransferDataAMSSlot(QDBusVariant)\0"
    "TransferDataAMSErrorSlot(uint,uchar,QByteArray)\0"
    "CheckActivateModuleSlot(QDBusVariant)\0"
    "RequestTransferExitSlot(QDBusVariant)\0"
    "SWDLProgressInfoSlot(uint,uint,uint)\0"
    "UpdateProgressingInfoStatusSlot(uint,uint)\0"
    "iBoxSWDLFBlockVisibleSlot(bool)\0bool\0"
    "swdlid,state,hwidentifier\0"
    "SetReadHWIdentifier(uint,uint,QString)\0"
    "SetReadDevName(uint,QString)\0"
    "SetReadHWVersion(uint,QString)\0"
    "SetReadNumberofModule(uint,QByteArray)\0"
    "SetReadModuleVersion(uint,QDBusVariant)\0"
    "swdlOpMode,swdlProgress\0"
    "SetSWDLProgressInfo(uint,uint)\0"
    "SetReprogrammingMode(QDBusVariant)\0"
    "SetInitUpdateResult(QDBusVariant)\0"
    "SetEraseMemory(QDBusVariant)\0"
    "SetRequestDownload(QDBusVariant)\0"
    "SetPrepareFileTransfer(QDBusVariant)\0"
    "fileInfo\0SetPrepareFileTransferIBOX(QDBusVariant)\0"
    "SetTransferDataCMS(QDBusVariant)\0"
    "SetTransferDataAMS(QDBusVariant)\0"
    "SetCheckActivateModule(QDBusVariant)\0"
    "SetRequestTransferExit(QDBusVariant)\0"
    "updateStatus\0SetUpdateProgressingInfo(uint)\0"
};

void CSWDLAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CSWDLAdaptor *_t = static_cast<CSWDLAdaptor *>(_o);
        switch (_id) {
        case 0: _t->ReadHWIdentifier((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->ReadDevName((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->ReadHWVersion((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->ReadNumberofModule((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 4: _t->ReadModuleVersion((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QDBusVariant(*)>(_a[2]))); break;
        case 5: _t->ReprogrammingMode((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 6: _t->InitUpdateResult((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 7: _t->EraseMemory((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 8: _t->RequestDownload((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 9: _t->PrepareFileTransfer((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 10: _t->TransferDataCMS((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 11: _t->TransferDataAMS((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 12: _t->TransferDataAMSError((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uchar(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 13: _t->CheckActivateModule((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 14: _t->RequestTransferExit((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 15: _t->SWDLProgressInfo((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 16: _t->UpdateProgressingInfoStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 17: _t->iBoxSWDLFBlockVisible((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 18: _t->ReadHWIdentifierSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 19: _t->ReadDevNameSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 20: _t->ReadHWVersionSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 21: _t->ReadNumberofModuleSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 22: _t->ReadModuleVersionSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QDBusVariant(*)>(_a[2]))); break;
        case 23: _t->ReprogrammingModeSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 24: _t->InitUpdateResultSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 25: _t->EraseMemorySlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 26: _t->RequestDownloadSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 27: _t->PrepareFileTransferSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 28: _t->TransferDataCMSSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 29: _t->TransferDataAMSSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 30: _t->TransferDataAMSErrorSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uchar(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 31: _t->CheckActivateModuleSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 32: _t->RequestTransferExitSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 33: _t->SWDLProgressInfoSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3]))); break;
        case 34: _t->UpdateProgressingInfoStatusSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 35: _t->iBoxSWDLFBlockVisibleSlot((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 36: { bool _r = _t->SetReadHWIdentifier((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 37: { bool _r = _t->SetReadDevName((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 38: { bool _r = _t->SetReadHWVersion((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 39: { bool _r = _t->SetReadNumberofModule((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 40: { bool _r = _t->SetReadModuleVersion((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QDBusVariant(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 41: { bool _r = _t->SetSWDLProgressInfo((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 42: { bool _r = _t->SetReprogrammingMode((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 43: { bool _r = _t->SetInitUpdateResult((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 44: { bool _r = _t->SetEraseMemory((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 45: { bool _r = _t->SetRequestDownload((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 46: { bool _r = _t->SetPrepareFileTransfer((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 47: { bool _r = _t->SetPrepareFileTransferIBOX((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 48: { bool _r = _t->SetTransferDataCMS((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 49: { bool _r = _t->SetTransferDataAMS((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 50: { bool _r = _t->SetCheckActivateModule((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 51: { bool _r = _t->SetRequestTransferExit((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 52: { bool _r = _t->SetUpdateProgressingInfo((*reinterpret_cast< const uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CSWDLAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CSWDLAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CSWDLAdaptor,
      qt_meta_data_CSWDLAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CSWDLAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CSWDLAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CSWDLAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSWDLAdaptor))
        return static_cast<void*>(const_cast< CSWDLAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int CSWDLAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 53)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 53;
    }
    return _id;
}

// SIGNAL 0
void CSWDLAdaptor::ReadHWIdentifier(const uint _t1, const QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CSWDLAdaptor::ReadDevName(const uint _t1, const QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CSWDLAdaptor::ReadHWVersion(const uint _t1, const QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CSWDLAdaptor::ReadNumberofModule(const uint _t1, const QByteArray _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CSWDLAdaptor::ReadModuleVersion(const uint _t1, const QDBusVariant & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CSWDLAdaptor::ReprogrammingMode(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CSWDLAdaptor::InitUpdateResult(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CSWDLAdaptor::EraseMemory(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CSWDLAdaptor::RequestDownload(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CSWDLAdaptor::PrepareFileTransfer(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CSWDLAdaptor::TransferDataCMS(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CSWDLAdaptor::TransferDataAMS(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CSWDLAdaptor::TransferDataAMSError(const uint _t1, const uchar _t2, const QByteArray _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CSWDLAdaptor::CheckActivateModule(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CSWDLAdaptor::RequestTransferExit(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CSWDLAdaptor::SWDLProgressInfo(const uint _t1, const uint _t2, const uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void CSWDLAdaptor::UpdateProgressingInfoStatus(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void CSWDLAdaptor::iBoxSWDLFBlockVisible(const bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}
static const uint qt_meta_data_CSpeechAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      59,   16,   15,   15, 0x05,
     127,   99,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     147,   16,   15,   15, 0x08,
     191,   99,   15,   15, 0x08,
     260,  220,  215,   15, 0x0a,
     319,  294,  215,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CSpeechAdaptor[] = {
    "CSpeechAdaptor\0\0"
    "sourceDeviceID,senderHandle,length,ttsData\0"
    "StartTTSData(uint,uint,uint,QByteArray)\0"
    "sourceDeviceID,senderHandle\0"
    "AbortTTS(uint,uint)\0"
    "StartTTSDataSlot(uint,uint,uint,QByteArray)\0"
    "AbortTTSSlot(uint,uint)\0bool\0"
    "senderHandle,lengthconst,deviceid,state\0"
    "SetTTSResult(uint,uint,uint,uint)\0"
    "inputTarget,inputKeyWord\0"
    "SetManualInput(uint,QString)\0"
};

void CSpeechAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CSpeechAdaptor *_t = static_cast<CSpeechAdaptor *>(_o);
        switch (_id) {
        case 0: _t->StartTTSData((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 1: _t->AbortTTS((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 2: _t->StartTTSDataSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 3: _t->AbortTTSSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 4: { bool _r = _t->SetTTSResult((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])),(*reinterpret_cast< const uint(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { bool _r = _t->SetManualInput((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CSpeechAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CSpeechAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CSpeechAdaptor,
      qt_meta_data_CSpeechAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CSpeechAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CSpeechAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CSpeechAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSpeechAdaptor))
        return static_cast<void*>(const_cast< CSpeechAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int CSpeechAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CSpeechAdaptor::StartTTSData(const uint _t1, const uint _t2, const uint _t3, const QByteArray _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CSpeechAdaptor::AbortTTS(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_CTelephoneAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      42,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      16,       // signalCount

 // signals: signature, parameters, type, tag, flags
      29,   19,   18,   18, 0x05,
      66,   52,   18,   18, 0x05,
     103,   90,   18,   18, 0x05,
     141,  130,   18,   18, 0x05,
     171,  158,   18,   18, 0x05,
     226,  199,   18,   18, 0x05,
     270,  254,   18,   18, 0x05,
     302,  290,   18,   18, 0x05,
     340,  324,   18,   18, 0x05,
     386,  370,   18,   18, 0x05,
     432,  419,   18,   18, 0x05,
     462,  450,   18,   18, 0x05,
     513,  493,   18,   18, 0x05,
     541,  493,   18,   18, 0x05,
     589,  569,   18,   18, 0x05,
     623,  616,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     647,   19,   18,   18, 0x08,
     674,   52,   18,   18, 0x08,
     702,   90,   18,   18, 0x08,
     733,  130,   18,   18, 0x08,
     754,  158,   18,   18, 0x08,
     786,  199,   18,   18, 0x08,
     818,  254,   18,   18, 0x08,
     842,  290,   18,   18, 0x08,
     868,  324,   18,   18, 0x08,
     902,  370,   18,   18, 0x08,
     939,  419,   18,   18, 0x08,
     963,  450,   18,   18, 0x08,
     998,  493,   18,   18, 0x08,
    1030,  493,   18,   18, 0x08,
    1062,  569,   18,   18, 0x08,
    1093,  616,   18,   18, 0x08,
    1149, 1126, 1121,   18, 0x0a,
    1178,  450, 1121,   18, 0x0a,
    1208,   18, 1121,   18, 0x0a,
    1237, 1233, 1121,   18, 0x0a,
    1267,  493, 1121,   18, 0x0a,
    1294,   18, 1121,   18, 0x0a,
    1315,   18, 1121,   18, 0x0a,
    1331,   18, 1121,   18, 0x0a,
    1348,   18, 1121,   18, 0x0a,
    1363,   18, 1121,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CTelephoneAdaptor[] = {
    "CTelephoneAdaptor\0\0telNumber\0"
    "CurrentNumber(QString)\0callStatedata\0"
    "CallState(QDBusVariant)\0callDuration\0"
    "CallDuration(QDBusVariant)\0modemlevel\0"
    "ModemLevel(uint)\0contact,name\0"
    "RecipientInfo(uint,QString)\0"
    "msdCallStatus,msdCallCount\0"
    "MissedCallStatus(uint,uint)\0usermode,micOff\0"
    "UserMode(uint,bool)\0modemstatus\0"
    "ModemDataStatus(uint)\0callHistoryInfo\0"
    "CallHistoryInfo(QDBusVariant)\0"
    "pBookDataResult\0CollectPBookResult(QDBusVariant)\0"
    "totalPBEntry\0PBookStatus(uint)\0"
    "dialNumData\0DialNumberResult(QDBusVariant)\0"
    "senderHandle,callId\0AcceptCallResult(uint,uint)\0"
    "HangupCallResult(uint,uint)\0"
    "tmStatus,provStatus\0TMServiceStatus(uint,uint)\0"
    "status\0CallHistoryStatus(uint)\0"
    "CurrentNumberSlot(QString)\0"
    "CallStateSlot(QDBusVariant)\0"
    "CallDurationSlot(QDBusVariant)\0"
    "ModemLevelSlot(uint)\0"
    "RecipientInfoSlot(uint,QString)\0"
    "MissedCallStatusSlot(uint,uint)\0"
    "UserModeSlot(uint,bool)\0"
    "ModemDataStatusSlot(uint)\0"
    "CallHistoryInfoSlot(QDBusVariant)\0"
    "CollectPBookResultSlot(QDBusVariant)\0"
    "PBookStatusSlot(ushort)\0"
    "DialNumberResultSlot(QDBusVariant)\0"
    "AcceptCallResultSlot(uint,uint)\0"
    "HangupCallResultSlot(uint,uint)\0"
    "TMServiceStatusSlot(uint,uint)\0"
    "CallHistoryStatusSlot(uint)\0bool\0"
    "senderHandle,pBookType\0"
    "StartCollectPBook(uint,uint)\0"
    "StartDialNumber(QDBusVariant)\0"
    "CallHistoryInfoRequest()\0msg\0"
    "StartAcceptCall(QDBusMessage)\0"
    "StartHangupCall(uint,uint)\0"
    "GetModemDataStatus()\0GetModemLevel()\0"
    "GetPBookStatus()\0GetTMService()\0"
    "GetCallHistoryStatus()\0"
};

void CTelephoneAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CTelephoneAdaptor *_t = static_cast<CTelephoneAdaptor *>(_o);
        switch (_id) {
        case 0: _t->CurrentNumber((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->CallState((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 2: _t->CallDuration((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 3: _t->ModemLevel((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 4: _t->RecipientInfo((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->MissedCallStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 6: _t->UserMode((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        case 7: _t->ModemDataStatus((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 8: _t->CallHistoryInfo((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 9: _t->CollectPBookResult((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 10: _t->PBookStatus((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 11: _t->DialNumberResult((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 12: _t->AcceptCallResult((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 13: _t->HangupCallResult((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 14: _t->TMServiceStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 15: _t->CallHistoryStatus((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 16: _t->CurrentNumberSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->CallStateSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 18: _t->CallDurationSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 19: _t->ModemLevelSlot((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 20: _t->RecipientInfoSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 21: _t->MissedCallStatusSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 22: _t->UserModeSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        case 23: _t->ModemDataStatusSlot((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 24: _t->CallHistoryInfoSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 25: _t->CollectPBookResultSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 26: _t->PBookStatusSlot((*reinterpret_cast< const ushort(*)>(_a[1]))); break;
        case 27: _t->DialNumberResultSlot((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 28: _t->AcceptCallResultSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 29: _t->HangupCallResultSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 30: _t->TMServiceStatusSlot((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 31: _t->CallHistoryStatusSlot((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 32: { bool _r = _t->StartCollectPBook((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 33: { bool _r = _t->StartDialNumber((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 34: { bool _r = _t->CallHistoryInfoRequest();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 35: { bool _r = _t->StartAcceptCall((*reinterpret_cast< const QDBusMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 36: { bool _r = _t->StartHangupCall((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 37: { bool _r = _t->GetModemDataStatus();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 38: { bool _r = _t->GetModemLevel();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 39: { bool _r = _t->GetPBookStatus();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 40: { bool _r = _t->GetTMService();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 41: { bool _r = _t->GetCallHistoryStatus();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CTelephoneAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CTelephoneAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CTelephoneAdaptor,
      qt_meta_data_CTelephoneAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CTelephoneAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CTelephoneAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CTelephoneAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTelephoneAdaptor))
        return static_cast<void*>(const_cast< CTelephoneAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int CTelephoneAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 42)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 42;
    }
    return _id;
}

// SIGNAL 0
void CTelephoneAdaptor::CurrentNumber(const QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CTelephoneAdaptor::CallState(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CTelephoneAdaptor::CallDuration(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CTelephoneAdaptor::ModemLevel(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CTelephoneAdaptor::RecipientInfo(const uint _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CTelephoneAdaptor::MissedCallStatus(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CTelephoneAdaptor::UserMode(const uint _t1, const bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CTelephoneAdaptor::ModemDataStatus(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CTelephoneAdaptor::CallHistoryInfo(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CTelephoneAdaptor::CollectPBookResult(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CTelephoneAdaptor::PBookStatus(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CTelephoneAdaptor::DialNumberResult(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CTelephoneAdaptor::AcceptCallResult(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CTelephoneAdaptor::HangupCallResult(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CTelephoneAdaptor::TMServiceStatus(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CTelephoneAdaptor::CallHistoryStatus(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}
static const uint qt_meta_data_CDMBTunerAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      40,   23,   18,   17, 0x0a,
      72,   63,   18,   17, 0x0a,
     133,   98,   18,   17, 0x0a,
     173,  157,   18,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CDMBTunerAdaptor[] = {
    "CDMBTunerAdaptor\0\0bool\0dmbSignalQuality\0"
    "SetSignalQuality(uint)\0tpegInfo\0"
    "SetTPEGInfo(QDBusVariant)\0"
    "dmbStauts,currentBrowserListLength\0"
    "SetDMBStauts(uint,uint)\0tpegData,length\0"
    "SetTPEGStatus(QByteArray,int)\0"
};

void CDMBTunerAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CDMBTunerAdaptor *_t = static_cast<CDMBTunerAdaptor *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->SetSignalQuality((*reinterpret_cast< const uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { bool _r = _t->SetTPEGInfo((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = _t->SetDMBStauts((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { bool _r = _t->SetTPEGStatus((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CDMBTunerAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CDMBTunerAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CDMBTunerAdaptor,
      qt_meta_data_CDMBTunerAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CDMBTunerAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CDMBTunerAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CDMBTunerAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDMBTunerAdaptor))
        return static_cast<void*>(const_cast< CDMBTunerAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int CDMBTunerAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
static const uint qt_meta_data_CSatRadioAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      31,   18,   17,   17, 0x05,
      74,   58,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     127,  103,   98,   17, 0x0a,
     183,  160,   98,   17, 0x0a,
     251,  213,   98,   17, 0x0a,
     306,  289,   98,   17, 0x0a,
     330,   18,   17,   17, 0x08,
     361,   58,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CSatRadioAdaptor[] = {
    "CSatRadioAdaptor\0\0type,station\0"
    "onStationSelect(uint,uint)\0serviceProvider\0"
    "onServiceProvider(uint)\0bool\0"
    "optype,station,presetNo\0"
    "SetStationSelect(uint,uint,uint)\0"
    "optype,serviceProvider\0"
    "SetServiceProvider(uint,uint)\0"
    "optype,audio,stationList,subscription\0"
    "SetServiceStatus(uint,bool,bool,uint)\0"
    "optype,tmcStatus\0SetTMCStatus(uint,uint)\0"
    "StationSelectSlot(uchar,uchar)\0"
    "ServiceProviderSlot(uchar)\0"
};

void CSatRadioAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CSatRadioAdaptor *_t = static_cast<CSatRadioAdaptor *>(_o);
        switch (_id) {
        case 0: _t->onStationSelect((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 1: _t->onServiceProvider((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 2: { bool _r = _t->SetStationSelect((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const uint(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { bool _r = _t->SetServiceProvider((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: { bool _r = _t->SetServiceStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2])),(*reinterpret_cast< const bool(*)>(_a[3])),(*reinterpret_cast< const uint(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { bool _r = _t->SetTMCStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: _t->StationSelectSlot((*reinterpret_cast< const uchar(*)>(_a[1])),(*reinterpret_cast< const uchar(*)>(_a[2]))); break;
        case 7: _t->ServiceProviderSlot((*reinterpret_cast< const uchar(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CSatRadioAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CSatRadioAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CSatRadioAdaptor,
      qt_meta_data_CSatRadioAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CSatRadioAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CSatRadioAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CSatRadioAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSatRadioAdaptor))
        return static_cast<void*>(const_cast< CSatRadioAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int CSatRadioAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CSatRadioAdaptor::onStationSelect(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CSatRadioAdaptor::onServiceProvider(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_CAppStateAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      27,   23,   18,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CAppStateAdaptor[] = {
    "CAppStateAdaptor\0\0bool\0msg\0"
    "onAppStateChanged(QDBusMessage)\0"
};

void CAppStateAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CAppStateAdaptor *_t = static_cast<CAppStateAdaptor *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->onAppStateChanged((*reinterpret_cast< const QDBusMessage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CAppStateAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAppStateAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CAppStateAdaptor,
      qt_meta_data_CAppStateAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAppStateAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAppStateAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAppStateAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAppStateAdaptor))
        return static_cast<void*>(const_cast< CAppStateAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int CAppStateAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
