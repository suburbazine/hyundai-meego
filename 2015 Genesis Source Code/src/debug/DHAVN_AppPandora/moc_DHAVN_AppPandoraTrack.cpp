/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppPandoraTrack.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppPandoraTrack.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppPandoraTrack.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CAppPandoraTrack[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      45,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      15,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,
      32,   17,   17,   17, 0x05,
      44,   17,   17,   17, 0x05,
      70,   57,   17,   17, 0x05,
      92,   17,   17,   17, 0x05,
     112,   17,   17,   17, 0x05,
     143,  133,   17,   17, 0x05,
     173,   17,   17,   17, 0x05,
     190,   17,   17,   17, 0x05,
     223,  209,   17,   17, 0x05,
     259,  246,   17,   17, 0x05,
     282,  273,   17,   17, 0x05,
     343,  308,   17,   17, 0x05,
     383,  375,   17,   17, 0x05,
     414,  404,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     445,   17,   17,   17, 0x0a,
     459,   17,   17,   17, 0x0a,
     502,  476,   17,   17, 0x0a,
     546,   57,   17,   17, 0x0a,
     570,  209,   17,   17, 0x0a,
     614,  599,   17,   17, 0x0a,
     650,  638,   17,   17, 0x0a,
     694,  683,   17,   17, 0x0a,
     737,  722,   17,   17, 0x0a,
     763,   17,   17,   17, 0x0a,
     776,  722,   17,   17, 0x0a,

 // methods: signature, parameters, type, tag, flags
     802,   17,   17,   17, 0x02,
     809,   17,   17,   17, 0x02,
     817,   17,   17,   17, 0x02,
     824,   17,   17,   17, 0x02,
     834,   17,   17,   17, 0x02,
     846,   17,   17,   17, 0x02,
     861,   17,   17,   17, 0x02,
     888,  878,   17,   17, 0x02,
     917,   17,   17,   17, 0x02,
     939,   17,   17,   17, 0x02,
     965,   17,  958,   17, 0x02,
     979,   17,   17,   17, 0x02,
     996,   17,   17,   17, 0x02,
    1024,   17, 1019,   17, 0x02,
    1044,   17, 1036,   17, 0x02,
    1066,   17,   17,   17, 0x02,
    1087,   17, 1079,   17, 0x02,
    1112,   17,   17,   17, 0x02,
    1129,   17, 1019,   17, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_CAppPandoraTrack[] = {
    "CAppPandoraTrack\0\0playStarted()\0"
    "pauseDone()\0skipFailed()\0inTrackToken\0"
    "trackUpdated(quint32)\0trackInfoReceived()\0"
    "trackShardReceived()\0inExplain\0"
    "trackExplainReceived(QString)\0"
    "songBookmarked()\0artistBookmarked()\0"
    "inElapsedTime\0trackElapsedTime(uint)\0"
    "inUserRating\0rated(quint8)\0filePath\0"
    "albumArtReceived(QString)\0"
    "inStationArtIndex,inStationArtPath\0"
    "stationArtReceived(int,QString)\0inError\0"
    "handleError(quint16)\0isPlaying\0"
    "clearScreenonTrackNone(qint16)\0"
    "onPlayTimer()\0startPlayTimer()\0"
    "inRequestType,inPndrError\0"
    "OnRequestCompleted(ERequestType,EPndrError)\0"
    "OnTrackUpdated(quint32)\0"
    "OnUpdateElapsedTime(quint16)\0"
    "inTrackExplain\0OnTrackExplain(QString)\0"
    "inTrackInfo\0OnTrackInfoReceived(TSTrackInfo)\0"
    "inAlbumArt\0OnAlbumArtReceived(QString)\0"
    "inStationToken\0OnStationUpdated(quint32)\0"
    "devChanged()\0OnStationDeleted(quint32)\0"
    "Play()\0Pause()\0Skip()\0ThumbUp()\0"
    "ThumbDown()\0BookmarkSong()\0BookmarkArtist()\0"
    "inEnabled\0SetTrackElapsedPolling(bool)\0"
    "RequestExplaination()\0RequestTrackInfo()\0"
    "qint16\0TrackStatus()\0ClearTrackInfo()\0"
    "onBrandingImageStart()\0bool\0IsPlaying()\0"
    "QString\0IsStationArtPresent()\0"
    "ClearCache()\0quint32\0GetCurrentStationToken()\0"
    "CheckTrackInfo()\0IsNullTrackInof()\0"
};

void CAppPandoraTrack::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CAppPandoraTrack *_t = static_cast<CAppPandoraTrack *>(_o);
        switch (_id) {
        case 0: _t->playStarted(); break;
        case 1: _t->pauseDone(); break;
        case 2: _t->skipFailed(); break;
        case 3: _t->trackUpdated((*reinterpret_cast< const quint32(*)>(_a[1]))); break;
        case 4: _t->trackInfoReceived(); break;
        case 5: _t->trackShardReceived(); break;
        case 6: _t->trackExplainReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->songBookmarked(); break;
        case 8: _t->artistBookmarked(); break;
        case 9: _t->trackElapsedTime((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 10: _t->rated((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 11: _t->albumArtReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->stationArtReceived((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 13: _t->handleError((*reinterpret_cast< const quint16(*)>(_a[1]))); break;
        case 14: _t->clearScreenonTrackNone((*reinterpret_cast< qint16(*)>(_a[1]))); break;
        case 15: _t->onPlayTimer(); break;
        case 16: _t->startPlayTimer(); break;
        case 17: _t->OnRequestCompleted((*reinterpret_cast< const ERequestType(*)>(_a[1])),(*reinterpret_cast< const EPndrError(*)>(_a[2]))); break;
        case 18: _t->OnTrackUpdated((*reinterpret_cast< const quint32(*)>(_a[1]))); break;
        case 19: _t->OnUpdateElapsedTime((*reinterpret_cast< const quint16(*)>(_a[1]))); break;
        case 20: _t->OnTrackExplain((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 21: _t->OnTrackInfoReceived((*reinterpret_cast< const TSTrackInfo(*)>(_a[1]))); break;
        case 22: _t->OnAlbumArtReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 23: _t->OnStationUpdated((*reinterpret_cast< const quint32(*)>(_a[1]))); break;
        case 24: _t->devChanged(); break;
        case 25: _t->OnStationDeleted((*reinterpret_cast< const quint32(*)>(_a[1]))); break;
        case 26: _t->Play(); break;
        case 27: _t->Pause(); break;
        case 28: _t->Skip(); break;
        case 29: _t->ThumbUp(); break;
        case 30: _t->ThumbDown(); break;
        case 31: _t->BookmarkSong(); break;
        case 32: _t->BookmarkArtist(); break;
        case 33: _t->SetTrackElapsedPolling((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 34: _t->RequestExplaination(); break;
        case 35: _t->RequestTrackInfo(); break;
        case 36: { qint16 _r = _t->TrackStatus();
            if (_a[0]) *reinterpret_cast< qint16*>(_a[0]) = _r; }  break;
        case 37: _t->ClearTrackInfo(); break;
        case 38: _t->onBrandingImageStart(); break;
        case 39: { bool _r = _t->IsPlaying();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 40: { QString _r = _t->IsStationArtPresent();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 41: _t->ClearCache(); break;
        case 42: { quint32 _r = _t->GetCurrentStationToken();
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = _r; }  break;
        case 43: _t->CheckTrackInfo(); break;
        case 44: { bool _r = _t->IsNullTrackInof();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CAppPandoraTrack::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAppPandoraTrack::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CAppPandoraTrack,
      qt_meta_data_CAppPandoraTrack, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAppPandoraTrack::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAppPandoraTrack::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAppPandoraTrack::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAppPandoraTrack))
        return static_cast<void*>(const_cast< CAppPandoraTrack*>(this));
    return QObject::qt_metacast(_clname);
}

int CAppPandoraTrack::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 45)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 45;
    }
    return _id;
}

// SIGNAL 0
void CAppPandoraTrack::playStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CAppPandoraTrack::pauseDone()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CAppPandoraTrack::skipFailed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void CAppPandoraTrack::trackUpdated(const quint32 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CAppPandoraTrack::trackInfoReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void CAppPandoraTrack::trackShardReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void CAppPandoraTrack::trackExplainReceived(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CAppPandoraTrack::songBookmarked()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void CAppPandoraTrack::artistBookmarked()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void CAppPandoraTrack::trackElapsedTime(const unsigned int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CAppPandoraTrack::rated(quint8 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CAppPandoraTrack::albumArtReceived(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CAppPandoraTrack::stationArtReceived(const int _t1, const QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CAppPandoraTrack::handleError(const quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CAppPandoraTrack::clearScreenonTrackNone(qint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}
QT_END_MOC_NAMESPACE
