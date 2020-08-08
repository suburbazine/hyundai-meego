/****************************************************************************
** Meta object code from reading C++ file 'cddamanager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/deck/cddamanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cddamanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CDDAManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      46,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      25,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   13,   12,   12, 0x05,
      56,   46,   12,   12, 0x05,
      86,   79,   12,   12, 0x05,
     117,  106,   12,   12, 0x05,
     154,  141,   12,   12, 0x05,
     187,  180,   12,   12, 0x05,
     200,   12,   12,   12, 0x05,
     230,  218,   12,   12, 0x05,
     256,  250,   12,   12, 0x05,
     275,   12,   12,   12, 0x05,
     349,  308,   12,   12, 0x05,
     383,   12,   12,   12, 0x05,
     398,   12,   12,   12, 0x05,
     419,  218,   12,   12, 0x05,
     443,  218,   12,   12, 0x05,
     472,  467,   12,   12, 0x05,
     502,  496,   12,   12, 0x05,
     536,  528,   12,   12, 0x05,
     578,  566,   12,   12, 0x05,
     625,  616,   12,   12, 0x05,
     662,  657,   12,   12, 0x05,
     696,  690,   12,   12, 0x05,
     736,  726,   12,   12, 0x05,
     782,  770,   12,   12, 0x05,
     835,  820,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     869,   12,   12,   12, 0x0a,
     876,   12,   12,   12, 0x0a,
     884,   12,   12,   12, 0x0a,
     899,   12,   12,   12, 0x0a,
     917,  906,   12,   12, 0x0a,
     946,  927,   12,   12, 0x0a,
     960,  906,   12,   12, 0x2a,
     970,  250,   12,   12, 0x0a,
     992,  980,   12,   12, 0x0a,
    1012,  250,   12,   12, 0x2a,
    1028,  980,   12,   12, 0x0a,
    1048,  250,   12,   12, 0x2a,
    1070, 1064,   12,   12, 0x0a,
    1087, 1064,   12,   12, 0x0a,
    1109, 1099,   12,   12, 0x0a,
    1131, 1099,   12,   12, 0x0a,
    1153,   12,   12,   12, 0x0a,
    1176,   12,   12,   12, 0x0a,
    1197,   12,   12,   12, 0x0a,
    1219,   12,   12,   12, 0x0a,
    1243,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CDDAManager[] = {
    "CDDAManager\0\0nPosition\0OnPositionChanged(int)\0"
    "nDuration\0OnDurationChanged(int)\0"
    "nState\0OnStateChanged(int)\0nDiscState\0"
    "OnDiscStateChanged(int)\0nScreenState\0"
    "OnScreenStateChanged(int)\0nError\0"
    "OnError(int)\0OnScanCompleted()\0"
    "nTrackIndex\0OnTrackChanged(int)\0nMode\0"
    "OnModeChanged(int)\0OnCurrentMediaStatusChanged(int)\0"
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
    "OnTag2Received(DvdcTag2Info*)\0pFileName\0"
    "OnFileNameReceived(DvdcFileName*)\0"
    "pFolderName\0OnFolderNameReceived(DvdcFolderName*)\0"
    "nSize,pTocData\0OnGraceNoteTOCReceived(int,void*)\0"
    "Play()\0Pause()\0FunctionStop()\0Stop()\0"
    "nSkipLevel\0Next(int)\0nSkipLevel,nForced\0"
    "Prev(int,int)\0Prev(int)\0Scan(int)\0"
    "nMode,bPlay\0RandomPlay(int,int)\0"
    "RandomPlay(int)\0RepeatPlay(int,int)\0"
    "RepeatPlay(int)\0nRate\0FastForward(int)\0"
    "Rewind(int)\0pPlayList\0NextFolder(PlayList*)\0"
    "PrevFolder(PlayList*)\0HandleRetryGNTocInfo()\0"
    "HandleRetryTocInfo()\0HandleRetryToc2Info()\0"
    "HandleRetryCDTextInfo()\0HandleRetryPlayback()\0"
};

void CDDAManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CDDAManager *_t = static_cast<CDDAManager *>(_o);
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
        case 14: _t->OnAutoTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->OnTOCReceived((*reinterpret_cast< DvdcTOC*(*)>(_a[1]))); break;
        case 16: _t->OnTOC2Received((*reinterpret_cast< DvdcTOC2*(*)>(_a[1]))); break;
        case 17: _t->OnCDTextReceived((*reinterpret_cast< DvdcCDText*(*)>(_a[1]))); break;
        case 18: _t->OnCDTextInfoReceived((*reinterpret_cast< DvdcCdTextInfo*(*)>(_a[1]))); break;
        case 19: _t->OnDirTreeReceived((*reinterpret_cast< DvdcDirTree*(*)>(_a[1]))); break;
        case 20: _t->OnTagReceived((*reinterpret_cast< DvdcTagInfo*(*)>(_a[1]))); break;
        case 21: _t->OnTag2Received((*reinterpret_cast< DvdcTag2Info*(*)>(_a[1]))); break;
        case 22: _t->OnFileNameReceived((*reinterpret_cast< DvdcFileName*(*)>(_a[1]))); break;
        case 23: _t->OnFolderNameReceived((*reinterpret_cast< DvdcFolderName*(*)>(_a[1]))); break;
        case 24: _t->OnGraceNoteTOCReceived((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2]))); break;
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
        case 41: _t->HandleRetryGNTocInfo(); break;
        case 42: _t->HandleRetryTocInfo(); break;
        case 43: _t->HandleRetryToc2Info(); break;
        case 44: _t->HandleRetryCDTextInfo(); break;
        case 45: _t->HandleRetryPlayback(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CDDAManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CDDAManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CDDAManager,
      qt_meta_data_CDDAManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CDDAManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CDDAManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CDDAManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDDAManager))
        return static_cast<void*>(const_cast< CDDAManager*>(this));
    if (!strcmp(_clname, "ICDInterface"))
        return static_cast< ICDInterface*>(const_cast< CDDAManager*>(this));
    if (!strcmp(_clname, "IMediaInterface"))
        return static_cast< IMediaInterface*>(const_cast< CDDAManager*>(this));
    if (!strcmp(_clname, "IAudioInterface"))
        return static_cast< IAudioInterface*>(const_cast< CDDAManager*>(this));
    return QObject::qt_metacast(_clname);
}

int CDDAManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 46)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 46;
    }
    return _id;
}

// SIGNAL 0
void CDDAManager::OnPositionChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CDDAManager::OnDurationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CDDAManager::OnStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CDDAManager::OnDiscStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CDDAManager::OnScreenStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CDDAManager::OnError(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CDDAManager::OnScanCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void CDDAManager::OnTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CDDAManager::OnModeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CDDAManager::OnCurrentMediaStatusChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CDDAManager::OnTrackBufferred(int _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CDDAManager::OnNormalPlay()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void CDDAManager::OnRealSoundChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void CDDAManager::OnUserTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CDDAManager::OnAutoTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CDDAManager::OnTOCReceived(DvdcTOC * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void CDDAManager::OnTOC2Received(DvdcTOC2 * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void CDDAManager::OnCDTextReceived(DvdcCDText * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void CDDAManager::OnCDTextInfoReceived(DvdcCdTextInfo * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void CDDAManager::OnDirTreeReceived(DvdcDirTree * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void CDDAManager::OnTagReceived(DvdcTagInfo * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void CDDAManager::OnTag2Received(DvdcTag2Info * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void CDDAManager::OnFileNameReceived(DvdcFileName * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void CDDAManager::OnFolderNameReceived(DvdcFolderName * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void CDDAManager::OnGraceNoteTOCReceived(int _t1, void * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}
QT_END_MOC_NAMESPACE
