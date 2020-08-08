/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_MOSTManager_TelephoneShadowControl.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_MOSTManager_TelephoneShadowControl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_MOSTManager_TelephoneShadowControl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CTelephoneShadowControl[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      16,       // signalCount

 // signals: signature, parameters, type, tag, flags
      35,   25,   24,   24, 0x05,
      72,   58,   24,   24, 0x05,
     113,   96,   24,   24, 0x05,
     151,  140,   24,   24, 0x05,
     181,  168,   24,   24, 0x05,
     236,  209,   24,   24, 0x05,
     280,  264,   24,   24, 0x05,
     312,  300,   24,   24, 0x05,
     350,  334,   24,   24, 0x05,
     395,  383,   24,   24, 0x05,
     431,  415,   24,   24, 0x05,
     473,  461,   24,   24, 0x05,
     524,  504,   24,   24, 0x05,
     552,  504,   24,   24, 0x05,
     600,  580,   24,   24, 0x05,
     634,  627,   24,   24, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CTelephoneShadowControl[] = {
    "CTelephoneShadowControl\0\0telNumber\0"
    "CurrentNumber(QString)\0callStateData\0"
    "CallState(QDBusVariant)\0callDurationData\0"
    "CallDuration(QDBusVariant)\0modemlevel\0"
    "ModemLevel(uint)\0contact,name\0"
    "RecipientInfo(uint,QString)\0"
    "msdCallStatus,msdCallCount\0"
    "MissedCallStatus(uint,uint)\0usermode,micOff\0"
    "UserMode(uint,bool)\0modemstatus\0"
    "ModemDataStatus(uint)\0pBookDataResult\0"
    "CollectPBookResult(QDBusVariant)\0"
    "pBookStatus\0PBookStatus(ushort)\0"
    "callHistoryInfo\0CallHistoryInfo(QDBusVariant)\0"
    "dialNumData\0DialNumberResult(QDBusVariant)\0"
    "senderHandle,callId\0AcceptCallResult(uint,uint)\0"
    "HangupCallResult(uint,uint)\0"
    "tmStatus,provStatus\0TMServiceStatus(uint,uint)\0"
    "status\0CallHistoryStatus(uint)\0"
};

void CTelephoneShadowControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CTelephoneShadowControl *_t = static_cast<CTelephoneShadowControl *>(_o);
        switch (_id) {
        case 0: _t->CurrentNumber((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->CallState((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 2: _t->CallDuration((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 3: _t->ModemLevel((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 4: _t->RecipientInfo((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->MissedCallStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 6: _t->UserMode((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        case 7: _t->ModemDataStatus((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 8: _t->CollectPBookResult((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 9: _t->PBookStatus((*reinterpret_cast< const ushort(*)>(_a[1]))); break;
        case 10: _t->CallHistoryInfo((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 11: _t->DialNumberResult((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        case 12: _t->AcceptCallResult((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 13: _t->HangupCallResult((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 14: _t->TMServiceStatus((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 15: _t->CallHistoryStatus((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CTelephoneShadowControl::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CTelephoneShadowControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CTelephoneShadowControl,
      qt_meta_data_CTelephoneShadowControl, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CTelephoneShadowControl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CTelephoneShadowControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CTelephoneShadowControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTelephoneShadowControl))
        return static_cast<void*>(const_cast< CTelephoneShadowControl*>(this));
    if (!strcmp(_clname, "CControlThreadBase"))
        return static_cast< CControlThreadBase*>(const_cast< CTelephoneShadowControl*>(this));
    return QObject::qt_metacast(_clname);
}

int CTelephoneShadowControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void CTelephoneShadowControl::CurrentNumber(const QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CTelephoneShadowControl::CallState(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CTelephoneShadowControl::CallDuration(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CTelephoneShadowControl::ModemLevel(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CTelephoneShadowControl::RecipientInfo(const uint _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CTelephoneShadowControl::MissedCallStatus(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CTelephoneShadowControl::UserMode(const uint _t1, const bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CTelephoneShadowControl::ModemDataStatus(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CTelephoneShadowControl::CollectPBookResult(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CTelephoneShadowControl::PBookStatus(const ushort _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CTelephoneShadowControl::CallHistoryInfo(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CTelephoneShadowControl::DialNumberResult(const QDBusVariant & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CTelephoneShadowControl::AcceptCallResult(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CTelephoneShadowControl::HangupCallResult(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CTelephoneShadowControl::TMServiceStatus(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CTelephoneShadowControl::CallHistoryStatus(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}
QT_END_MOC_NAMESPACE
