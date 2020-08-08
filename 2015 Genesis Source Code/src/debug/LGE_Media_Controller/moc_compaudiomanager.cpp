/****************************************************************************
** Meta object code from reading C++ file 'compaudiomanager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/deck/compaudiomanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'compaudiomanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CompAudioManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      47,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      27,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   18,   17,   17, 0x05,
      61,   51,   17,   17, 0x05,
      91,   84,   17,   17, 0x05,
     122,  111,   17,   17, 0x05,
     159,  146,   17,   17, 0x05,
     192,  185,   17,   17, 0x05,
     205,   17,   17,   17, 0x05,
     235,  223,   17,   17, 0x05,
     255,  223,   17,   17, 0x05,
     281,  275,   17,   17, 0x05,
     300,   17,   17,   17, 0x05,
     374,  333,   17,   17, 0x05,
     408,   17,   17,   17, 0x05,
     423,   17,   17,   17, 0x05,
     444,  223,   17,   17, 0x05,
     468,  223,   17,   17, 0x05,
     497,  492,   17,   17, 0x05,
     527,  521,   17,   17, 0x05,
     561,  553,   17,   17, 0x05,
     603,  591,   17,   17, 0x05,
     650,  641,   17,   17, 0x05,
     687,  682,   17,   17, 0x05,
     721,  715,   17,   17, 0x05,
     759,  751,   17,   17, 0x05,
     803,  793,   17,   17, 0x05,
     849,  837,   17,   17, 0x05,
     887,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     908,   17,   17,   17, 0x0a,
     915,   17,   17,   17, 0x0a,
     923,   17,   17,   17, 0x0a,
     938,   17,   17,   17, 0x0a,
     956,  945,   17,   17, 0x0a,
     985,  966,   17,   17, 0x0a,
     999,  945,   17,   17, 0x2a,
    1009,  275,   17,   17, 0x0a,
    1031, 1019,   17,   17, 0x0a,
    1051,  275,   17,   17, 0x2a,
    1067, 1019,   17,   17, 0x0a,
    1087,  275,   17,   17, 0x2a,
    1109, 1103,   17,   17, 0x0a,
    1126, 1103,   17,   17, 0x0a,
    1148, 1138,   17,   17, 0x0a,
    1170, 1138,   17,   17, 0x0a,
    1192,   17,   17,   17, 0x0a,
    1213,   17,   17,   17, 0x0a,
    1245,   17,   17,   17, 0x0a,
    1266,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CompAudioManager[] = {
    "CompAudioManager\0\0nPosition\0"
    "OnPositionChanged(int)\0nDuration\0"
    "OnDurationChanged(int)\0nState\0"
    "OnStateChanged(int)\0nDiscState\0"
    "OnDiscStateChanged(int)\0nScreenState\0"
    "OnScreenStateChanged(int)\0nError\0"
    "OnError(int)\0OnScanCompleted()\0"
    "nTrackIndex\0OnTrackChanged(int)\0"
    "OnTrackUpdated(int)\0nMode\0OnModeChanged(int)\0"
    "OnCurrentMediaStatusChanged(int)\0"
    "bPrev,nTotalIdx,nIdxInFolder,nFolderSize\0"
    "OnTrackBufferred(int,int,int,int)\0"
    "OnNormalPlay()\0OnRealSoundChanged()\0"
    "OnUserTrackChanged(int)\0OnAutoTrackChanged(int)\0"
    "pTOC\0OnTOCReceived(DvdcTOC*)\0pTOC2\0"
    "OnTOC2Received(DvdcTOC2*)\0pCDText\0"
    "OnCDTextReceived(DvdcCDText*)\0pCDTextInfo\0"
    "OnCDTextInfoReceived(DvdcCdTextInfo*)\0"
    "pDirTree\0OnDirTreeReceived(DvdcDirTree*)\0"
    "pTag\0OnTagReceived(DvdcTagInfo*)\0pTag2\0"
    "OnTag2Received(DvdcTag2Info*)\0pAllTag\0"
    "OnAllTagReceived(DvdcAllTagInfo*)\0"
    "pFileName\0OnFileNameReceived(DvdcFileName*)\0"
    "pFolderName\0OnFolderNameReceived(DvdcFolderName*)\0"
    "OnDirTreeCompleted()\0Play()\0Pause()\0"
    "FunctionStop()\0Stop()\0nSkipLevel\0"
    "Next(int)\0nSkipLevel,nForced\0Prev(int,int)\0"
    "Prev(int)\0Scan(int)\0nMode,bPlay\0"
    "RandomPlay(int,int)\0RandomPlay(int)\0"
    "RepeatPlay(int,int)\0RepeatPlay(int)\0"
    "nRate\0FastForward(int)\0Rewind(int)\0"
    "pPlayList\0NextFolder(PlayList*)\0"
    "PrevFolder(PlayList*)\0HandleRetryDirTree()\0"
    "HandleRetryFileFolderTotalNum()\0"
    "HandleRetryTagInfo()\0HandleRetryPlayback()\0"
};

void CompAudioManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CompAudioManager *_t = static_cast<CompAudioManager *>(_o);
        switch (_id) {
        case 0: _t->OnPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->OnDurationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->OnStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->OnDiscStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->OnScreenStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->OnError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->OnScanCompleted(); break;
        case 7: _t->OnTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->OnTrackUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->OnModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->OnCurrentMediaStatusChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->OnTrackBufferred((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 12: _t->OnNormalPlay(); break;
        case 13: _t->OnRealSoundChanged(); break;
        case 14: _t->OnUserTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->OnAutoTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->OnTOCReceived((*reinterpret_cast< DvdcTOC*(*)>(_a[1]))); break;
        case 17: _t->OnTOC2Received((*reinterpret_cast< DvdcTOC2*(*)>(_a[1]))); break;
        case 18: _t->OnCDTextReceived((*reinterpret_cast< DvdcCDText*(*)>(_a[1]))); break;
        case 19: _t->OnCDTextInfoReceived((*reinterpret_cast< DvdcCdTextInfo*(*)>(_a[1]))); break;
        case 20: _t->OnDirTreeReceived((*reinterpret_cast< DvdcDirTree*(*)>(_a[1]))); break;
        case 21: _t->OnTagReceived((*reinterpret_cast< DvdcTagInfo*(*)>(_a[1]))); break;
        case 22: _t->OnTag2Received((*reinterpret_cast< DvdcTag2Info*(*)>(_a[1]))); break;
        case 23: _t->OnAllTagReceived((*reinterpret_cast< DvdcAllTagInfo*(*)>(_a[1]))); break;
        case 24: _t->OnFileNameReceived((*reinterpret_cast< DvdcFileName*(*)>(_a[1]))); break;
        case 25: _t->OnFolderNameReceived((*reinterpret_cast< DvdcFolderName*(*)>(_a[1]))); break;
        case 26: _t->OnDirTreeCompleted(); break;
        case 27: _t->Play(); break;
        case 28: _t->Pause(); break;
        case 29: _t->FunctionStop(); break;
        case 30: _t->Stop(); break;
        case 31: _t->Next((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->Prev((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 33: _t->Prev((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: _t->Scan((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: _t->RandomPlay((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 36: _t->RandomPlay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: _t->RepeatPlay((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 38: _t->RepeatPlay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 39: _t->FastForward((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 40: _t->Rewind((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 41: _t->NextFolder((*reinterpret_cast< PlayList*(*)>(_a[1]))); break;
        case 42: _t->PrevFolder((*reinterpret_cast< PlayList*(*)>(_a[1]))); break;
        case 43: _t->HandleRetryDirTree(); break;
        case 44: _t->HandleRetryFileFolderTotalNum(); break;
        case 45: _t->HandleRetryTagInfo(); break;
        case 46: _t->HandleRetryPlayback(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CompAudioManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CompAudioManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CompAudioManager,
      qt_meta_data_CompAudioManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CompAudioManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CompAudioManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CompAudioManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CompAudioManager))
        return static_cast<void*>(const_cast< CompAudioManager*>(this));
    if (!strcmp(_clname, "ICDInterface"))
        return static_cast< ICDInterface*>(const_cast< CompAudioManager*>(this));
    if (!strcmp(_clname, "IMediaInterface"))
        return static_cast< IMediaInterface*>(const_cast< CompAudioManager*>(this));
    if (!strcmp(_clname, "IAudioInterface"))
        return static_cast< IAudioInterface*>(const_cast< CompAudioManager*>(this));
    return QObject::qt_metacast(_clname);
}

int CompAudioManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 47)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 47;
    }
    return _id;
}

// SIGNAL 0
void CompAudioManager::OnPositionChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CompAudioManager::OnDurationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CompAudioManager::OnStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CompAudioManager::OnDiscStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CompAudioManager::OnScreenStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CompAudioManager::OnError(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CompAudioManager::OnScanCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void CompAudioManager::OnTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CompAudioManager::OnTrackUpdated(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CompAudioManager::OnModeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CompAudioManager::OnCurrentMediaStatusChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CompAudioManager::OnTrackBufferred(int _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CompAudioManager::OnNormalPlay()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void CompAudioManager::OnRealSoundChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void CompAudioManager::OnUserTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CompAudioManager::OnAutoTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void CompAudioManager::OnTOCReceived(DvdcTOC * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void CompAudioManager::OnTOC2Received(DvdcTOC2 * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void CompAudioManager::OnCDTextReceived(DvdcCDText * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void CompAudioManager::OnCDTextInfoReceived(DvdcCdTextInfo * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void CompAudioManager::OnDirTreeReceived(DvdcDirTree * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void CompAudioManager::OnTagReceived(DvdcTagInfo * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void CompAudioManager::OnTag2Received(DvdcTag2Info * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void CompAudioManager::OnAllTagReceived(DvdcAllTagInfo * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void CompAudioManager::OnFileNameReceived(DvdcFileName * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void CompAudioManager::OnFolderNameReceived(DvdcFolderName * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void CompAudioManager::OnDirTreeCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 26, 0);
}
QT_END_MOC_NAMESPACE
