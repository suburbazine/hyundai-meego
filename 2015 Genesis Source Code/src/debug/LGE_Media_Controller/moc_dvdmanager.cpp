/****************************************************************************
** Meta object code from reading C++ file 'dvdmanager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/deck/dvdmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dvdmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DVDManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      43,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      25,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   12,   11,   11, 0x05,
      55,   45,   11,   11, 0x05,
      85,   78,   11,   11, 0x05,
     116,  105,   11,   11, 0x05,
     153,  140,   11,   11, 0x05,
     186,  179,   11,   11, 0x05,
     199,   11,   11,   11, 0x05,
     229,  217,   11,   11, 0x05,
     255,  249,   11,   11, 0x05,
     274,   11,   11,   11, 0x05,
     322,  307,   11,   11, 0x05,
     360,  347,   11,   11, 0x05,
     398,  388,   11,   11, 0x05,
     433,  423,   11,   11, 0x05,
     499,  458,   11,   11, 0x05,
     533,   11,   11,   11, 0x05,
     548,   11,   11,   11, 0x05,
     569,  217,   11,   11, 0x05,
     652,  593,   11,   11, 0x05,
     732,  697,   11,   11, 0x05,
     770,  388,   11,   11, 0x05,
     808,  794,   11,   11, 0x05,
     835,  423,   11,   11, 0x05,
     888,  855,   11,   11, 0x05,
     928,  919,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     952,   11,   11,   11, 0x0a,
     959,   11,   11,   11, 0x0a,
     967,   11,   11,   11, 0x0a,
     982,   11,   11,   11, 0x0a,
    1000,  989,   11,   11, 0x0a,
    1029, 1010,   11,   11, 0x0a,
    1043,  989,   11,   11, 0x2a,
    1053,  249,   11,   11, 0x0a,
    1075, 1063,   11,   11, 0x0a,
    1095,  249,   11,   11, 0x2a,
    1111, 1063,   11,   11, 0x0a,
    1131,  249,   11,   11, 0x2a,
    1153, 1147,   11,   11, 0x0a,
    1170, 1147,   11,   11, 0x0a,
    1192, 1182,   11,   11, 0x0a,
    1214, 1182,   11,   11, 0x0a,
    1236,   11,   11,   11, 0x0a,
    1256,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DVDManager[] = {
    "DVDManager\0\0nPosition\0OnPositionChanged(int)\0"
    "nDuration\0OnDurationChanged(int)\0"
    "nState\0OnStateChanged(int)\0nDiscState\0"
    "OnDiscStateChanged(int)\0nScreenState\0"
    "OnScreenStateChanged(int)\0nError\0"
    "OnError(int)\0OnScanCompleted()\0"
    "nTrackIndex\0OnTrackChanged(int)\0nMode\0"
    "OnModeChanged(int)\0OnCurrentMediaStatusChanged(int)\0"
    "nTitleGrpIndex\0OnTitleGroupChanged(int)\0"
    "nSubtitleNum\0OnTotalSubtitleChanged(int)\0"
    "nAudioNum\0OnTotalAudioChanged(int)\0"
    "nAngleNum\0OnTotalAngleChanged(int)\0"
    "bPrev,nTotalIdx,nIdxInFolder,nFolderSize\0"
    "OnTrackBufferred(int,int,int,int)\0"
    "OnNormalPlay()\0OnRealSoundChanged()\0"
    "OnUserTrackChanged(int)\0"
    "pAudioCodeList,pAudioAttrList,pAudioChnlList,nAudioCodeNum\0"
    "OnTotalAudioLangReceived(int*,int*,int*,int)\0"
    "pSubtitleCodeList,nSubtitleCodeNum\0"
    "OnTotalSubtitleLangReceived(int*,int)\0"
    "OnAudioLangChanged(int)\0nSubtitleCode\0"
    "OnSubtitleLangChanged(int)\0"
    "OnAngleChanged(int)\0"
    "nTitleNo,nChapterNo,nChapterSize\0"
    "OnSearchCompleted(int,int,int)\0bSuccess\0"
    "OnDVDInitCompleted(int)\0Play()\0Pause()\0"
    "FunctionStop()\0Stop()\0nSkipLevel\0"
    "Next(int)\0nSkipLevel,nForced\0Prev(int,int)\0"
    "Prev(int)\0Scan(int)\0nMode,bPlay\0"
    "RandomPlay(int,int)\0RandomPlay(int)\0"
    "RepeatPlay(int,int)\0RepeatPlay(int)\0"
    "nRate\0FastForward(int)\0Rewind(int)\0"
    "pPlayList\0NextFolder(PlayList*)\0"
    "PrevFolder(PlayList*)\0HandleRetryVRInfo()\0"
    "HandleRetryPlayback()\0"
};

void DVDManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DVDManager *_t = static_cast<DVDManager *>(_o);
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
        case 10: _t->OnTitleGroupChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->OnTotalSubtitleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->OnTotalAudioChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->OnTotalAngleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->OnTrackBufferred((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 15: _t->OnNormalPlay(); break;
        case 16: _t->OnRealSoundChanged(); break;
        case 17: _t->OnUserTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->OnTotalAudioLangReceived((*reinterpret_cast< int*(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2])),(*reinterpret_cast< int*(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 19: _t->OnTotalSubtitleLangReceived((*reinterpret_cast< int*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 20: _t->OnAudioLangChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->OnSubtitleLangChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->OnAngleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->OnSearchCompleted((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 24: _t->OnDVDInitCompleted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->Play(); break;
        case 26: _t->Pause(); break;
        case 27: _t->FunctionStop(); break;
        case 28: _t->Stop(); break;
        case 29: _t->Next((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->Prev((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 31: _t->Prev((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->Scan((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->RandomPlay((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 34: _t->RandomPlay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: _t->RepeatPlay((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 36: _t->RepeatPlay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: _t->FastForward((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 38: _t->Rewind((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 39: _t->NextFolder((*reinterpret_cast< PlayList*(*)>(_a[1]))); break;
        case 40: _t->PrevFolder((*reinterpret_cast< PlayList*(*)>(_a[1]))); break;
        case 41: _t->HandleRetryVRInfo(); break;
        case 42: _t->HandleRetryPlayback(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DVDManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DVDManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DVDManager,
      qt_meta_data_DVDManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DVDManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DVDManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DVDManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DVDManager))
        return static_cast<void*>(const_cast< DVDManager*>(this));
    if (!strcmp(_clname, "ICDInterface"))
        return static_cast< ICDInterface*>(const_cast< DVDManager*>(this));
    if (!strcmp(_clname, "IMediaInterface"))
        return static_cast< IMediaInterface*>(const_cast< DVDManager*>(this));
    if (!strcmp(_clname, "IVideoInterface"))
        return static_cast< IVideoInterface*>(const_cast< DVDManager*>(this));
    return QObject::qt_metacast(_clname);
}

int DVDManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 43)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 43;
    }
    return _id;
}

// SIGNAL 0
void DVDManager::OnPositionChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DVDManager::OnDurationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DVDManager::OnStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DVDManager::OnDiscStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DVDManager::OnScreenStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DVDManager::OnError(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DVDManager::OnScanCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void DVDManager::OnTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void DVDManager::OnModeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void DVDManager::OnCurrentMediaStatusChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void DVDManager::OnTitleGroupChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void DVDManager::OnTotalSubtitleChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void DVDManager::OnTotalAudioChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void DVDManager::OnTotalAngleChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void DVDManager::OnTrackBufferred(int _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void DVDManager::OnNormalPlay()
{
    QMetaObject::activate(this, &staticMetaObject, 15, 0);
}

// SIGNAL 16
void DVDManager::OnRealSoundChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 16, 0);
}

// SIGNAL 17
void DVDManager::OnUserTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void DVDManager::OnTotalAudioLangReceived(int * _t1, int * _t2, int * _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void DVDManager::OnTotalSubtitleLangReceived(int * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void DVDManager::OnAudioLangChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void DVDManager::OnSubtitleLangChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void DVDManager::OnAngleChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void DVDManager::OnSearchCompleted(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void DVDManager::OnDVDInitCompleted(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}
QT_END_MOC_NAMESPACE
