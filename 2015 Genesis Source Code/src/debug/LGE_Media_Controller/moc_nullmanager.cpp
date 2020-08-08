/****************************************************************************
** Meta object code from reading C++ file 'nullmanager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/deck/nullmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'nullmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NullManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      20,   12,   12,   12, 0x0a,
      28,   12,   12,   12, 0x0a,
      43,   12,   12,   12, 0x0a,
      61,   50,   12,   12, 0x0a,
      90,   71,   12,   12, 0x0a,
     104,   50,   12,   12, 0x2a,
     120,  114,   12,   12, 0x0a,
     142,  130,   12,   12, 0x0a,
     162,  114,   12,   12, 0x2a,
     178,  130,   12,   12, 0x0a,
     198,  114,   12,   12, 0x2a,
     220,  214,   12,   12, 0x0a,
     237,  214,   12,   12, 0x0a,
     259,  249,   12,   12, 0x0a,
     281,  249,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_NullManager[] = {
    "NullManager\0\0Play()\0Pause()\0FunctionStop()\0"
    "Stop()\0nSkipLevel\0Next(int)\0"
    "nSkipLevel,nForced\0Prev(int,int)\0"
    "Prev(int)\0nMode\0Scan(int)\0nMode,bPlay\0"
    "RandomPlay(int,int)\0RandomPlay(int)\0"
    "RepeatPlay(int,int)\0RepeatPlay(int)\0"
    "nRate\0FastForward(int)\0Rewind(int)\0"
    "pPlayList\0NextFolder(PlayList*)\0"
    "PrevFolder(PlayList*)\0"
};

void NullManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        NullManager *_t = static_cast<NullManager *>(_o);
        switch (_id) {
        case 0: _t->Play(); break;
        case 1: _t->Pause(); break;
        case 2: _t->FunctionStop(); break;
        case 3: _t->Stop(); break;
        case 4: _t->Next((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->Prev((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->Prev((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->Scan((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->RandomPlay((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->RandomPlay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->RepeatPlay((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->RepeatPlay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->FastForward((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->Rewind((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->NextFolder((*reinterpret_cast< PlayList*(*)>(_a[1]))); break;
        case 15: _t->PrevFolder((*reinterpret_cast< PlayList*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData NullManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject NullManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NullManager,
      qt_meta_data_NullManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &NullManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *NullManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *NullManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NullManager))
        return static_cast<void*>(const_cast< NullManager*>(this));
    if (!strcmp(_clname, "ICDInterface"))
        return static_cast< ICDInterface*>(const_cast< NullManager*>(this));
    if (!strcmp(_clname, "IMediaInterface"))
        return static_cast< IMediaInterface*>(const_cast< NullManager*>(this));
    if (!strcmp(_clname, "IAudioInterface"))
        return static_cast< IAudioInterface*>(const_cast< NullManager*>(this));
    return QObject::qt_metacast(_clname);
}

int NullManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
