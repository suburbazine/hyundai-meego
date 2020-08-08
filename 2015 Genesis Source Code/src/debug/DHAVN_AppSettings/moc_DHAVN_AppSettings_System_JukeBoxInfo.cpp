/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppSettings_System_JukeBoxInfo.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppSettings_System_JukeBoxInfo.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppSettings_System_JukeBoxInfo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppSettings_System_JukeBoxInfo[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      55,   32,   31,   31, 0x05,
     119,   97,   31,   31, 0x05,
     164,  157,   31,   31, 0x05,
     187,   31,   31,   31, 0x05,

 // slots: signature, parameters, type, tag, flags
     211,  204,   31,   31, 0x0a,
     241,   31,   31,   31, 0x0a,
     262,   31,   31,   31, 0x0a,
     284,  275,   31,   31, 0x0a,

 // methods: signature, parameters, type, tag, flags
     310,   31,  306,   31, 0x02,
     335,   31,  306,   31, 0x02,
     357,   31,  306,   31, 0x02,
     383,  157,   31,   31, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_AppSettings_System_JukeBoxInfo[] = {
    "AppSettings_System_JukeBoxInfo\0\0"
    "maxMb,usedMb,usedScale\0"
    "jukeBoxInfoUpdated(QVariant,QVariant,int)\0"
    "currentSize,usedScale\0"
    "jukeBoxInfoUpdatedString(QString,int)\0"
    "screen\0formattingStarted(int)\0"
    "formatComplete()\0status\0"
    "FormattingComleteHandler(int)\0"
    "StartJukeBoxFormat()\0UpdateInfo()\0"
    "useTimer\0startUpdateInfo(bool)\0int\0"
    "GetJukeBoxMaxScaleSize()\0GetJukeBoxMaxMBSize()\0"
    "GetUsedJukeBoxScaleSize()\0"
    "InitiateJukeBoxFormat(int)\0"
};

void AppSettings_System_JukeBoxInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppSettings_System_JukeBoxInfo *_t = static_cast<AppSettings_System_JukeBoxInfo *>(_o);
        switch (_id) {
        case 0: _t->jukeBoxInfoUpdated((*reinterpret_cast< QVariant(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->jukeBoxInfoUpdatedString((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->formattingStarted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->formatComplete(); break;
        case 4: _t->FormattingComleteHandler((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->StartJukeBoxFormat(); break;
        case 6: _t->UpdateInfo(); break;
        case 7: _t->startUpdateInfo((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: { int _r = _t->GetJukeBoxMaxScaleSize();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: { int _r = _t->GetJukeBoxMaxMBSize();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 10: { int _r = _t->GetUsedJukeBoxScaleSize();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 11: _t->InitiateJukeBoxFormat((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppSettings_System_JukeBoxInfo::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppSettings_System_JukeBoxInfo::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppSettings_System_JukeBoxInfo,
      qt_meta_data_AppSettings_System_JukeBoxInfo, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppSettings_System_JukeBoxInfo::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppSettings_System_JukeBoxInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppSettings_System_JukeBoxInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppSettings_System_JukeBoxInfo))
        return static_cast<void*>(const_cast< AppSettings_System_JukeBoxInfo*>(this));
    return QObject::qt_metacast(_clname);
}

int AppSettings_System_JukeBoxInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void AppSettings_System_JukeBoxInfo::jukeBoxInfoUpdated(QVariant _t1, QVariant _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppSettings_System_JukeBoxInfo::jukeBoxInfoUpdatedString(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppSettings_System_JukeBoxInfo::formattingStarted(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppSettings_System_JukeBoxInfo::formatComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
