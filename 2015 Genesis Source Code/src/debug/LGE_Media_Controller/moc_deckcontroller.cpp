/****************************************************************************
** Meta object code from reading C++ file 'deckcontroller.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/controller/deckcontroller.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'deckcontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DeckController[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      41,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      40,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      39,   15,   15,   15, 0x05,
      62,   15,   15,   15, 0x05,
      82,   15,   15,   15, 0x05,
     106,   15,   15,   15, 0x05,
     132,   15,   15,   15, 0x05,
     145,   15,   15,   15, 0x05,
     169,  163,   15,   15, 0x05,
     189,  163,   15,   15, 0x05,
     209,  163,   15,   15, 0x05,
     228,   15,   15,   15, 0x05,
     261,  163,   15,   15, 0x05,
     299,  286,   15,   15, 0x05,
     337,  327,   15,   15, 0x05,
     372,  362,   15,   15, 0x05,
     438,  397,   15,   15, 0x05,
     472,   15,   15,   15, 0x05,
     487,   15,   15,   15, 0x05,
     520,  508,   15,   15, 0x05,
     544,  508,   15,   15, 0x05,
     573,  568,   15,   15, 0x05,
     603,  597,   15,   15, 0x05,
     637,  629,   15,   15, 0x05,
     679,  667,   15,   15, 0x05,
     726,  717,   15,   15, 0x05,
     763,  758,   15,   15, 0x05,
     797,  791,   15,   15, 0x05,
     835,  827,   15,   15, 0x05,
     879,  869,   15,   15, 0x05,
     925,  913,   15,   15, 0x05,
    1022,  963,   15,   15, 0x05,
    1102, 1067,   15,   15, 0x05,
    1140,  327,   15,   15, 0x05,
    1178, 1164,   15,   15, 0x05,
    1205,  362,   15,   15, 0x05,
    1258, 1225,   15,   15, 0x05,
    1298, 1289,   15,   15, 0x05,
    1337, 1322,   15,   15, 0x05,
    1371,   15,   15,   15, 0x05,
    1399,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
    1426, 1420,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DeckController[] = {
    "DeckController\0\0OnPositionChanged(int)\0"
    "OnDurationChanged(int)\0OnStateChanged(int)\0"
    "OnDiscStateChanged(int)\0"
    "OnScreenStateChanged(int)\0OnError(int)\0"
    "OnScanCompleted()\0index\0OnTrackChanged(int)\0"
    "OnTrackUpdated(int)\0OnModeChanged(int)\0"
    "OnCurrentMediaStatusChanged(int)\0"
    "OnTitleGroupChanged(int)\0nSubtitleNum\0"
    "OnTotalSubtitleChanged(int)\0nAudioNum\0"
    "OnTotalAudioChanged(int)\0nAngleNum\0"
    "OnTotalAngleChanged(int)\0"
    "bPrev,nTotalIdx,nIdxInFolder,nFolderSize\0"
    "OnTrackBufferred(int,int,int,int)\0"
    "OnNormalPlay()\0OnRealSoundChanged()\0"
    "nTrackIndex\0OnUserTrackChanged(int)\0"
    "OnAutoTrackChanged(int)\0pTOC\0"
    "OnTOCReceived(DvdcTOC*)\0pTOC2\0"
    "OnTOC2Received(DvdcTOC2*)\0pCDText\0"
    "OnCDTextReceived(DvdcCDText*)\0pCDTextInfo\0"
    "OnCDTextInfoReceived(DvdcCdTextInfo*)\0"
    "pDirTree\0OnDirTreeReceived(DvdcDirTree*)\0"
    "pTag\0OnTagReceived(DvdcTagInfo*)\0pTag2\0"
    "OnTag2Received(DvdcTag2Info*)\0pAllTag\0"
    "OnAllTagReceived(DvdcAllTagInfo*)\0"
    "pFileName\0OnFileNameReceived(DvdcFileName*)\0"
    "pFolderName\0OnFolderNameReceived(DvdcFolderName*)\0"
    "pAudioCodeList,pAudioAttrList,pAudioChnlList,nAudioCodeNum\0"
    "OnTotalAudioLangReceived(int*,int*,int*,int)\0"
    "pSubtitleCodeList,nSubtitleCodeNum\0"
    "OnTotalSubtitleLangReceived(int*,int)\0"
    "OnAudioLangChanged(int)\0nSubtitleCode\0"
    "OnSubtitleLangChanged(int)\0"
    "OnAngleChanged(int)\0"
    "nTitleNo,nChapterNo,nChapterSize\0"
    "OnSearchCompleted(int,int,int)\0bSuccess\0"
    "OnDVDInitCompleted(int)\0nSize,pTocData\0"
    "OnGraceNoteTOCReceived(int,void*)\0"
    "OnDeckPriorityLangChanged()\0"
    "OnDirTreeCompleted()\0nMode\0"
    "HandleModeChanged(int)\0"
};

void DeckController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DeckController *_t = static_cast<DeckController *>(_o);
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
        case 11: _t->OnTitleGroupChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->OnTotalSubtitleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->OnTotalAudioChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->OnTotalAngleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->OnTrackBufferred((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 16: _t->OnNormalPlay(); break;
        case 17: _t->OnRealSoundChanged(); break;
        case 18: _t->OnUserTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->OnAutoTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->OnTOCReceived((*reinterpret_cast< DvdcTOC*(*)>(_a[1]))); break;
        case 21: _t->OnTOC2Received((*reinterpret_cast< DvdcTOC2*(*)>(_a[1]))); break;
        case 22: _t->OnCDTextReceived((*reinterpret_cast< DvdcCDText*(*)>(_a[1]))); break;
        case 23: _t->OnCDTextInfoReceived((*reinterpret_cast< DvdcCdTextInfo*(*)>(_a[1]))); break;
        case 24: _t->OnDirTreeReceived((*reinterpret_cast< DvdcDirTree*(*)>(_a[1]))); break;
        case 25: _t->OnTagReceived((*reinterpret_cast< DvdcTagInfo*(*)>(_a[1]))); break;
        case 26: _t->OnTag2Received((*reinterpret_cast< DvdcTag2Info*(*)>(_a[1]))); break;
        case 27: _t->OnAllTagReceived((*reinterpret_cast< DvdcAllTagInfo*(*)>(_a[1]))); break;
        case 28: _t->OnFileNameReceived((*reinterpret_cast< DvdcFileName*(*)>(_a[1]))); break;
        case 29: _t->OnFolderNameReceived((*reinterpret_cast< DvdcFolderName*(*)>(_a[1]))); break;
        case 30: _t->OnTotalAudioLangReceived((*reinterpret_cast< int*(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2])),(*reinterpret_cast< int*(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 31: _t->OnTotalSubtitleLangReceived((*reinterpret_cast< int*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 32: _t->OnAudioLangChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->OnSubtitleLangChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: _t->OnAngleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: _t->OnSearchCompleted((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 36: _t->OnDVDInitCompleted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: _t->OnGraceNoteTOCReceived((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2]))); break;
        case 38: _t->OnDeckPriorityLangChanged(); break;
        case 39: _t->OnDirTreeCompleted(); break;
        case 40: _t->HandleModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DeckController::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DeckController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DeckController,
      qt_meta_data_DeckController, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DeckController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DeckController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DeckController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DeckController))
        return static_cast<void*>(const_cast< DeckController*>(this));
    return QObject::qt_metacast(_clname);
}

int DeckController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 41)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 41;
    }
    return _id;
}

// SIGNAL 0
void DeckController::OnPositionChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DeckController::OnDurationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DeckController::OnStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DeckController::OnDiscStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DeckController::OnScreenStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DeckController::OnError(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DeckController::OnScanCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void DeckController::OnTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void DeckController::OnTrackUpdated(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void DeckController::OnModeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void DeckController::OnCurrentMediaStatusChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void DeckController::OnTitleGroupChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void DeckController::OnTotalSubtitleChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void DeckController::OnTotalAudioChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void DeckController::OnTotalAngleChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void DeckController::OnTrackBufferred(int _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void DeckController::OnNormalPlay()
{
    QMetaObject::activate(this, &staticMetaObject, 16, 0);
}

// SIGNAL 17
void DeckController::OnRealSoundChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 17, 0);
}

// SIGNAL 18
void DeckController::OnUserTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void DeckController::OnAutoTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void DeckController::OnTOCReceived(DvdcTOC * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void DeckController::OnTOC2Received(DvdcTOC2 * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void DeckController::OnCDTextReceived(DvdcCDText * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void DeckController::OnCDTextInfoReceived(DvdcCdTextInfo * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void DeckController::OnDirTreeReceived(DvdcDirTree * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void DeckController::OnTagReceived(DvdcTagInfo * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void DeckController::OnTag2Received(DvdcTag2Info * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void DeckController::OnAllTagReceived(DvdcAllTagInfo * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 27, _a);
}

// SIGNAL 28
void DeckController::OnFileNameReceived(DvdcFileName * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void DeckController::OnFolderNameReceived(DvdcFolderName * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 29, _a);
}

// SIGNAL 30
void DeckController::OnTotalAudioLangReceived(int * _t1, int * _t2, int * _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 30, _a);
}

// SIGNAL 31
void DeckController::OnTotalSubtitleLangReceived(int * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 31, _a);
}

// SIGNAL 32
void DeckController::OnAudioLangChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 32, _a);
}

// SIGNAL 33
void DeckController::OnSubtitleLangChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 33, _a);
}

// SIGNAL 34
void DeckController::OnAngleChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 34, _a);
}

// SIGNAL 35
void DeckController::OnSearchCompleted(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 35, _a);
}

// SIGNAL 36
void DeckController::OnDVDInitCompleted(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 36, _a);
}

// SIGNAL 37
void DeckController::OnGraceNoteTOCReceived(int _t1, void * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 37, _a);
}

// SIGNAL 38
void DeckController::OnDeckPriorityLangChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 38, 0);
}

// SIGNAL 39
void DeckController::OnDirTreeCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 39, 0);
}
QT_END_MOC_NAMESPACE
