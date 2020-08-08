/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_MOSTManager_DLNAShadowControl.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_MOSTManager_DLNAShadowControl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_MOSTManager_DLNAShadowControl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CDLNAShadowControl[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      49,   20,   19,   19, 0x05,
      82,   75,   19,   19, 0x05,
     129,  103,   19,   19, 0x05,
     198,  170,   19,   19, 0x05,
     255,  244,   19,   19, 0x05,
     286,  272,   19,   19, 0x05,
     346,  306,   19,   19, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CDLNAShadowControl[] = {
    "CDLNAShadowControl\0\0diskTime,trackTime,titleTime\0"
    "TimePosition(int,int,int)\0status\0"
    "DLNAConnStatus(uint)\0playtime,trackno,songinfo\0"
    "CurrentSongInfo(uint,ushort,QStringList)\0"
    "vtitle,time,track,vfilename\0"
    "CurrentVideoInfo(QString,uint,ushort,QString)\0"
    "deckStatus\0Deckstatus(uint)\0trackPosition\0"
    "TrackPosition(uint)\0"
    "currentNumTrack,currentRelativeTrackPos\0"
    "TrackInformation(uint,uint)\0"
};

void CDLNAShadowControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CDLNAShadowControl *_t = static_cast<CDLNAShadowControl *>(_o);
        switch (_id) {
        case 0: _t->TimePosition((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 1: _t->DLNAConnStatus((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 2: _t->CurrentSongInfo((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3]))); break;
        case 3: _t->CurrentVideoInfo((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])),(*reinterpret_cast< const ushort(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 4: _t->Deckstatus((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 5: _t->TrackPosition((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 6: _t->TrackInformation((*reinterpret_cast< const uint(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CDLNAShadowControl::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CDLNAShadowControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CDLNAShadowControl,
      qt_meta_data_CDLNAShadowControl, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CDLNAShadowControl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CDLNAShadowControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CDLNAShadowControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDLNAShadowControl))
        return static_cast<void*>(const_cast< CDLNAShadowControl*>(this));
    if (!strcmp(_clname, "CControlThreadBase"))
        return static_cast< CControlThreadBase*>(const_cast< CDLNAShadowControl*>(this));
    return QObject::qt_metacast(_clname);
}

int CDLNAShadowControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CDLNAShadowControl::TimePosition(const int _t1, const int _t2, const int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CDLNAShadowControl::DLNAConnStatus(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CDLNAShadowControl::CurrentSongInfo(const uint _t1, const ushort _t2, const QStringList _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CDLNAShadowControl::CurrentVideoInfo(const QString _t1, const uint _t2, const ushort _t3, const QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CDLNAShadowControl::Deckstatus(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CDLNAShadowControl::TrackPosition(const uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CDLNAShadowControl::TrackInformation(const uint _t1, const uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
