/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppSettings_SaveSettings_ResetThread.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppSettings_SaveSettings_ResetThread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppSettings_SaveSettings_ResetThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ResetSettingThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   20,   19,   19, 0x05,
      62,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      75,   71,   67,   19, 0x0a,
     115,   98,   67,   19, 0x0a,
     149,   19,   19,   19, 0x0a,
     171,   19,   19,   19, 0x0a,
     194,   19,   19,   19, 0x0a,
     213,   71,   67,   19, 0x0a,
     230,   19,   67,   19, 0x0a,
     257,   19,   67,   19, 0x0a,
     285,   19,   67,   19, 0x0a,
     308,   19,   67,   19, 0x0a,
     329,   71,   67,   19, 0x0a,
     352,   20,   19,   19, 0x0a,
     378,   19,  373,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ResetSettingThread[] = {
    "ResetSettingThread\0\0value,key\0"
    "propertyResetCompleted(int,int)\0aa()\0"
    "int\0key\0GetDefaultSetting(int)\0"
    "defaultDispOnOff\0GetDefaultSettingOfDispOnOff(int)\0"
    "resetScreenSettings()\0resetGeneralSettings()\0"
    "GetDefaultKeyPad()\0LoadSetting(int)\0"
    "GetDefaultDateTimeFormat()\0"
    "getDefaultHyundaiKeyboard()\0"
    "GetDefaultSummerTime()\0GetDefaultLanguage()\0"
    "SetDefaultSetting(int)\0SaveSetting(int,int)\0"
    "bool\0close()\0"
};

void ResetSettingThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ResetSettingThread *_t = static_cast<ResetSettingThread *>(_o);
        switch (_id) {
        case 0: _t->propertyResetCompleted((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->aa(); break;
        case 2: { int _r = _t->GetDefaultSetting((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->GetDefaultSettingOfDispOnOff((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: _t->resetScreenSettings(); break;
        case 5: _t->resetGeneralSettings(); break;
        case 6: _t->GetDefaultKeyPad(); break;
        case 7: { int _r = _t->LoadSetting((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 8: { int _r = _t->GetDefaultDateTimeFormat();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: { int _r = _t->getDefaultHyundaiKeyboard();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 10: { int _r = _t->GetDefaultSummerTime();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 11: { int _r = _t->GetDefaultLanguage();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 12: { int _r = _t->SetDefaultSetting((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 13: _t->SaveSetting((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: { bool _r = _t->close();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ResetSettingThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ResetSettingThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ResetSettingThread,
      qt_meta_data_ResetSettingThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ResetSettingThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ResetSettingThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ResetSettingThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ResetSettingThread))
        return static_cast<void*>(const_cast< ResetSettingThread*>(this));
    return QThread::qt_metacast(_clname);
}

int ResetSettingThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void ResetSettingThread::propertyResetCompleted(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ResetSettingThread::aa()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
