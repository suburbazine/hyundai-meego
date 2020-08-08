/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppAhaTrack.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppAhaTrack.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppAhaTrack.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CAppAhaTrack[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      42,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      16,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      28,   13,   13,   13, 0x05,
      40,   13,   13,   13, 0x05,
      53,   13,   13,   13, 0x05,
      68,   13,   13,   13, 0x05,
      98,   88,   13,   13, 0x05,
     184,  128,   13,   13, 0x05,
     252,  243,   13,   13, 0x05,
     278,  243,   13,   13, 0x05,
     307,  243,   13,   13, 0x05,
     336,  243,   13,   13, 0x05,
     365,   13,   13,   13, 0x05,
     428,  393,   13,   13, 0x05,
     468,  460,   13,   13, 0x05,
     497,  491,   13,   13, 0x05,
     525,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     567,  542,   13,   13, 0x0a,
     623,  610,   13,   13, 0x0a,
     659,  647,   13,   13, 0x0a,
     706,  695,   13,   13, 0x0a,
     749,  734,   13,   13, 0x0a,
     795,  780,   13,   13, 0x0a,
     841,  826,   13,   13, 0x0a,
     900,  872,   13,   13, 0x0a,
     955,  933,   13,   13, 0x0a,
    1009,  996,   13,   13, 0x0a,
    1055,   13, 1047,   13, 0x0a,

 // methods: signature, parameters, type, tag, flags
    1083,   13,   13,   13, 0x02,
    1090,   13,   13,   13, 0x02,
    1098,   13,   13,   13, 0x02,
    1110,   13,   13,   13, 0x02,
    1117,   13,   13,   13, 0x02,
    1128,   13,   13,   13, 0x02,
    1140,   13,   13,   13, 0x02,
    1151,   13,   13,   13, 0x02,
    1171,   13,   13,   13, 0x02,
    1194,   13,   13,   13, 0x02,
    1213,   13,   13,   13, 0x02,
    1241,   13,   13,   13, 0x02,
    1274,   13, 1267,   13, 0x02,
    1288,   13,   13,   13, 0x02,
    1305,   13,   13,   13, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_CAppAhaTrack[] = {
    "CAppAhaTrack\0\0playStarted()\0pauseDone()\0"
    "skipFailed()\0trackUpdated()\0"
    "trackInfoReceived()\0inExplain\0"
    "trackExplainReceived(QString)\0"
    "currLikeIcon,altLikeIcon,currDislikeIcon,altDislikeIcon\0"
    "updateLikeDislikeFilePath(QString,QString,QString,QString)\0"
    "filePath\0albumArtReceived(QString)\0"
    "partnerLogoReceived(QString)\0"
    "ratingImageReceived(QString)\0"
    "stationLogoReceived(QString)\0"
    "receivingStationShowPopup()\0"
    "inStationArtIndex,inStationArtPath\0"
    "stationArtReceived(int,QString)\0inError\0"
    "handleError(EAhaError)\0title\0"
    "trackTitleReceived(QString)\0"
    "clearTpMessage()\0inRequestType,inAhaError\0"
    "OnRequestCompleted(ERequestType,EAhaError)\0"
    "inTrackToken\0OnTrackUpdated(quint32)\0"
    "inTrackInfo\0OnTrackInfoReceived(AhaTSTrackInfo)\0"
    "inAlbumArt\0OnAlbumArtReceived(QString)\0"
    "strPartnerLogo\0OnPartnerLogoReceived(QString)\0"
    "strRatingImage\0OnRatingImageReceived(QString)\0"
    "strStationLogo\0OnStationLogoReceived(QString)\0"
    "nIndex,strLikeImageFilePath\0"
    "OnLikeImageReceived(int,QString)\0"
    "nRateState,nCanUnvote\0"
    "OnUpdateLikeDislikeIcon(uint8_t,uint8_t)\0"
    "bRepeatState\0OnRepeatPressedSkipBackReceived(bool)\0"
    "QString\0GetCurrentStationFilePath()\0"
    "Play()\0Pause()\0PlayPause()\0Skip()\0"
    "SkipBack()\0Forward30()\0Rewind15()\0"
    "LikeButtonClicked()\0DislikeButtonClicked()\0"
    "RequestTrackInfo()\0DisplayCurrentLikeDislike()\0"
    "ClearCurrentLikeDislike()\0qint16\0"
    "TrackStatus()\0ClearTrackInfo()\0"
    "ignoreBufferingByKey()\0"
};

void CAppAhaTrack::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CAppAhaTrack *_t = static_cast<CAppAhaTrack *>(_o);
        switch (_id) {
        case 0: _t->playStarted(); break;
        case 1: _t->pauseDone(); break;
        case 2: _t->skipFailed(); break;
        case 3: _t->trackUpdated(); break;
        case 4: _t->trackInfoReceived(); break;
        case 5: _t->trackExplainReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->updateLikeDislikeFilePath((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 7: _t->albumArtReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->partnerLogoReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->ratingImageReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->stationLogoReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->receivingStationShowPopup(); break;
        case 12: _t->stationArtReceived((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 13: _t->handleError((*reinterpret_cast< const EAhaError(*)>(_a[1]))); break;
        case 14: _t->trackTitleReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->clearTpMessage(); break;
        case 16: _t->OnRequestCompleted((*reinterpret_cast< const ERequestType(*)>(_a[1])),(*reinterpret_cast< const EAhaError(*)>(_a[2]))); break;
        case 17: _t->OnTrackUpdated((*reinterpret_cast< const quint32(*)>(_a[1]))); break;
        case 18: _t->OnTrackInfoReceived((*reinterpret_cast< const AhaTSTrackInfo(*)>(_a[1]))); break;
        case 19: _t->OnAlbumArtReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 20: _t->OnPartnerLogoReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 21: _t->OnRatingImageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 22: _t->OnStationLogoReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 23: _t->OnLikeImageReceived((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 24: _t->OnUpdateLikeDislikeIcon((*reinterpret_cast< const uint8_t(*)>(_a[1])),(*reinterpret_cast< const uint8_t(*)>(_a[2]))); break;
        case 25: _t->OnRepeatPressedSkipBackReceived((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 26: { QString _r = _t->GetCurrentStationFilePath();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 27: _t->Play(); break;
        case 28: _t->Pause(); break;
        case 29: _t->PlayPause(); break;
        case 30: _t->Skip(); break;
        case 31: _t->SkipBack(); break;
        case 32: _t->Forward30(); break;
        case 33: _t->Rewind15(); break;
        case 34: _t->LikeButtonClicked(); break;
        case 35: _t->DislikeButtonClicked(); break;
        case 36: _t->RequestTrackInfo(); break;
        case 37: _t->DisplayCurrentLikeDislike(); break;
        case 38: _t->ClearCurrentLikeDislike(); break;
        case 39: { qint16 _r = _t->TrackStatus();
            if (_a[0]) *reinterpret_cast< qint16*>(_a[0]) = _r; }  break;
        case 40: _t->ClearTrackInfo(); break;
        case 41: _t->ignoreBufferingByKey(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CAppAhaTrack::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAppAhaTrack::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CAppAhaTrack,
      qt_meta_data_CAppAhaTrack, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAppAhaTrack::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAppAhaTrack::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAppAhaTrack::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAppAhaTrack))
        return static_cast<void*>(const_cast< CAppAhaTrack*>(this));
    return QObject::qt_metacast(_clname);
}

int CAppAhaTrack::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 42)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 42;
    }
    return _id;
}

// SIGNAL 0
void CAppAhaTrack::playStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CAppAhaTrack::pauseDone()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CAppAhaTrack::skipFailed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void CAppAhaTrack::trackUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CAppAhaTrack::trackInfoReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void CAppAhaTrack::trackExplainReceived(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CAppAhaTrack::updateLikeDislikeFilePath(QString _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CAppAhaTrack::albumArtReceived(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CAppAhaTrack::partnerLogoReceived(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CAppAhaTrack::ratingImageReceived(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CAppAhaTrack::stationLogoReceived(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CAppAhaTrack::receivingStationShowPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void CAppAhaTrack::stationArtReceived(const int _t1, const QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CAppAhaTrack::handleError(const EAhaError _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CAppAhaTrack::trackTitleReceived(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CAppAhaTrack::clearTpMessage()
{
    QMetaObject::activate(this, &staticMetaObject, 15, 0);
}
QT_END_MOC_NAMESPACE
