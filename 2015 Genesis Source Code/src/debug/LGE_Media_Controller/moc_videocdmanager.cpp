/****************************************************************************
** Meta object code from reading C++ file 'videocdmanager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/deck/videocdmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videocdmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VideoCDManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      35,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      16,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   16,   15,   15, 0x05,
      59,   49,   15,   15, 0x05,
      89,   82,   15,   15, 0x05,
     120,  109,   15,   15, 0x05,
     157,  144,   15,   15, 0x05,
     190,  183,   15,   15, 0x05,
     203,   15,   15,   15, 0x05,
     233,  221,   15,   15, 0x05,
     259,  253,   15,   15, 0x05,
     278,   15,   15,   15, 0x05,
     352,  311,   15,   15, 0x05,
     386,   15,   15,   15, 0x05,
     401,   15,   15,   15, 0x05,
     422,  221,   15,   15, 0x05,
     451,  446,   15,   15, 0x05,
     481,  475,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     507,   15,   15,   15, 0x0a,
     514,   15,   15,   15, 0x0a,
     522,   15,   15,   15, 0x0a,
     537,   15,   15,   15, 0x0a,
     555,  544,   15,   15, 0x0a,
     584,  565,   15,   15, 0x0a,
     598,  544,   15,   15, 0x2a,
     608,  253,   15,   15, 0x0a,
     630,  618,   15,   15, 0x0a,
     650,  253,   15,   15, 0x2a,
     666,  618,   15,   15, 0x0a,
     686,  253,   15,   15, 0x2a,
     708,  702,   15,   15, 0x0a,
     725,  702,   15,   15, 0x0a,
     747,  737,   15,   15, 0x0a,
     769,  737,   15,   15, 0x0a,
     791,   15,   15,   15, 0x0a,
     812,   15,   15,   15, 0x0a,
     834,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_VideoCDManager[] = {
    "VideoCDManager\0\0nPosition\0"
    "OnPositionChanged(int)\0nDuration\0"
    "OnDurationChanged(int)\0nState\0"
    "OnStateChanged(int)\0nDiscState\0"
    "OnDiscStateChanged(int)\0nScreenState\0"
    "OnScreenStateChanged(int)\0nError\0"
    "OnError(int)\0OnScanCompleted()\0"
    "nTrackIndex\0OnTrackChanged(int)\0nMode\0"
    "OnModeChanged(int)\0OnCurrentMediaStatusChanged(int)\0"
    "bPrev,nTotalIdx,nIdxInFolder,nFolderSize\0"
    "OnTrackBufferred(int,int,int,int)\0"
    "OnNormalPlay()\0OnRealSoundChanged()\0"
    "OnUserTrackChanged(int)\0pToc\0"
    "OnTOCReceived(DvdcTOC*)\0pToc2\0"
    "OnTOC2Received(DvdcTOC2*)\0Play()\0"
    "Pause()\0FunctionStop()\0Stop()\0nSkipLevel\0"
    "Next(int)\0nSkipLevel,nForced\0Prev(int,int)\0"
    "Prev(int)\0Scan(int)\0nMode,bPlay\0"
    "RandomPlay(int,int)\0RandomPlay(int)\0"
    "RepeatPlay(int,int)\0RepeatPlay(int)\0"
    "nRate\0FastForward(int)\0Rewind(int)\0"
    "pPlayList\0NextFolder(PlayList*)\0"
    "PrevFolder(PlayList*)\0HandleRetryTocInfo()\0"
    "HandleRetryToc2Info()\0HandleRetryPlayback()\0"
};

void VideoCDManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        VideoCDManager *_t = static_cast<VideoCDManager *>(_o);
        switch (_id) {
        case 0: _t->OnPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->OnDurationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->OnStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->OnDiscStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->OnScreenStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->OnError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->OnScanCompleted(); break;
        case 7: _t->OnTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->OnModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->OnCurrentMediaStatusChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->OnTrackBufferred((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 11: _t->OnNormalPlay(); break;
        case 12: _t->OnRealSoundChanged(); break;
        case 13: _t->OnUserTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->OnTOCReceived((*reinterpret_cast< DvdcTOC*(*)>(_a[1]))); break;
        case 15: _t->OnTOC2Received((*reinterpret_cast< DvdcTOC2*(*)>(_a[1]))); break;
        case 16: _t->Play(); break;
        case 17: _t->Pause(); break;
        case 18: _t->FunctionStop(); break;
        case 19: _t->Stop(); break;
        case 20: _t->Next((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->Prev((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 22: _t->Prev((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->Scan((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->RandomPlay((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 25: _t->RandomPlay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->RepeatPlay((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 27: _t->RepeatPlay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: _t->FastForward((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->Rewind((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->NextFolder((*reinterpret_cast< PlayList*(*)>(_a[1]))); break;
        case 31: _t->PrevFolder((*reinterpret_cast< PlayList*(*)>(_a[1]))); break;
        case 32: _t->HandleRetryTocInfo(); break;
        case 33: _t->HandleRetryToc2Info(); break;
        case 34: _t->HandleRetryPlayback(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData VideoCDManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject VideoCDManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_VideoCDManager,
      qt_meta_data_VideoCDManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VideoCDManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VideoCDManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VideoCDManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VideoCDManager))
        return static_cast<void*>(const_cast< VideoCDManager*>(this));
    if (!strcmp(_clname, "ICDInterface"))
        return static_cast< ICDInterface*>(const_cast< VideoCDManager*>(this));
    if (!strcmp(_clname, "IMediaInterface"))
        return static_cast< IMediaInterface*>(const_cast< VideoCDManager*>(this));
    if (!strcmp(_clname, "IVideoInterface"))
        return static_cast< IVideoInterface*>(const_cast< VideoCDManager*>(this));
    return QObject::qt_metacast(_clname);
}

int VideoCDManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    }
    return _id;
}

// SIGNAL 0
void VideoCDManager::OnPositionChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VideoCDManager::OnDurationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VideoCDManager::OnStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void VideoCDManager::OnDiscStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void VideoCDManager::OnScreenStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void VideoCDManager::OnError(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void VideoCDManager::OnScanCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void VideoCDManager::OnTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void VideoCDManager::OnModeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void VideoCDManager::OnCurrentMediaStatusChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void VideoCDManager::OnTrackBufferred(int _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void VideoCDManager::OnNormalPlay()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void VideoCDManager::OnRealSoundChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void VideoCDManager::OnUserTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void VideoCDManager::OnTOCReceived(DvdcTOC * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void VideoCDManager::OnTOC2Received(DvdcTOC2 * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}
QT_END_MOC_NAMESPACE
