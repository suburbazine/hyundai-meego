/****************************************************************************
** Meta object code from reading C++ file 'ahaAbsFrameTxRx.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ahaAbsFrameTxRx.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ahaAbsFrameTxRx.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CAhaAbsFrameTxRx[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      42,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      38,       // signalCount

 // signals: signature, parameters, type, tag, flags
      35,   18,   17,   17, 0x05,
      89,   69,   17,   17, 0x05,
     146,  126,   17,   17, 0x05,
     202,  183,   17,   17, 0x05,
     258,  238,   17,   17, 0x05,
     313,  295,   17,   17, 0x05,
     348,   17,   17,   17, 0x05,
     370,   17,   17,   17, 0x05,
     390,   17,   17,   17, 0x05,
     407,   17,   17,   17, 0x05,
     438,  426,   17,   17, 0x05,
     464,   17,   17,   17, 0x05,
     483,   17,   17,   17, 0x05,
     512,  501,   17,   17, 0x05,
     546,  501,   17,   17, 0x05,
     577,  501,   17,   17, 0x05,
     614,  501,   17,   17, 0x05,
     686,  650,   17,   17, 0x05,
     726,  501,   17,   17, 0x05,
     792,  762,   17,   17, 0x05,
     830,  501,   17,   17, 0x05,
     861,  501,   17,   17, 0x05,
     895,  501,   17,   17, 0x05,
     929,  501,   17,   17, 0x05,
     962,  501,   17,   17, 0x05,
     996,  501,   17,   17, 0x05,
    1028,  501,   17,   17, 0x05,
    1065,  501,   17,   17, 0x05,
    1103,  501,   17,   17, 0x05,
    1137,  501,   17,   17, 0x05,
    1168,  501,   17,   17, 0x05,
    1210,  501,   17,   17, 0x05,
    1252,  501,   17,   17, 0x05,
    1288,  501,   17,   17, 0x05,
    1332,  501,   17,   17, 0x05,
    1401, 1379,   17,   17, 0x05,
    1453,  501,   17,   17, 0x05,
    1518, 1491,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
    1569,   17,   17,   17, 0x0a,
    1590,   17,   17,   17, 0x0a,
    1615,   17,   17,   17, 0x0a,
    1649, 1639,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CAhaAbsFrameTxRx[] = {
    "CAhaAbsFrameTxRx\0\0albumArtFilePath\0"
    "albumArtProcessFinished(QString&)\0"
    "stationLogoFilePath\0"
    "stationLogoProcessFinished(QString&)\0"
    "partnerLogoFilePath\0"
    "partnerLogoProcessFinished(QString&)\0"
    "presetLogoFilePath\0"
    "presetLogoProcessFinished(QString&)\0"
    "ratingImageFilePath\0"
    "ratingImageProcessFinished(QString&)\0"
    "likeImageFilePath\0likeImageProcessFinished(QString&)\0"
    "ahaAppInitCompleted()\0ahaAppInitTimeOut()\0"
    "ahaPingTimeOut()\0ahaSessionClosed()\0"
    "nSampleRate\0ahaSampleRateChanged(int)\0"
    "ahaSessionOpened()\0ahaAppNotOpened()\0"
    "nErrorCode\0startPeerSessionDecoded(uint16_t)\0"
    "queryStationsDecoded(uint16_t)\0"
    "queryStationContentDecoded(uint16_t)\0"
    "activeStationParamDecoded(uint16_t)\0"
    "nErrorCode,nStartPresetStationIndex\0"
    "presetStationParamDecoded(uint16_t,int)\0"
    "activeContentParamDecoded(uint16_t)\0"
    "nErrorCode,nStartContentIndex\0"
    "listContentParamDecoded(uint16_t,int)\0"
    "queryAlbumArtDecoded(uint16_t)\0"
    "queryStationLogoDecoded(uint16_t)\0"
    "queryPartnerLogoDecoded(uint16_t)\0"
    "queryPresetLogoDecoded(uint16_t)\0"
    "queryRatingImageDecoded(uint16_t)\0"
    "queryLikeImageDecoded(uint16_t)\0"
    "queryCurrentStationDecoded(uint16_t)\0"
    "changeClientPlaybackDecoded(uint16_t)\0"
    "timeShiftContentDecoded(uint16_t)\0"
    "performActionDecoded(uint16_t)\0"
    "selectStationForPlaybackDecoded(uint16_t)\0"
    "selectContentForPlaybackDecoded(uint16_t)\0"
    "currentContentInfoDecoded(uint16_t)\0"
    "notifyPlaybackOfNewStationDecoded(uint16_t)\0"
    "notifyNewContentPlaybackStateDecoded(uint16_t)\0"
    "nErrorCode,nStationID\0"
    "notifyNewContentAvailableDecoded(uint16_t,uint64_t)\0"
    "notifyContentDeletedDecoded(uint16_t)\0"
    "nErrorCode,nConnStatusCode\0"
    "notifyClientStatusUpdateDecoded(uint16_t,uint32_t)\0"
    "onReplyWaitTimeOut()\0onPingCountdownTimeOut()\0"
    "onPingIntervalTimeOut()\0dataArray\0"
    "onDataReceived(QByteArray)\0"
};

void CAhaAbsFrameTxRx::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CAhaAbsFrameTxRx *_t = static_cast<CAhaAbsFrameTxRx *>(_o);
        switch (_id) {
        case 0: _t->albumArtProcessFinished((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->stationLogoProcessFinished((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->partnerLogoProcessFinished((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->presetLogoProcessFinished((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->ratingImageProcessFinished((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->likeImageProcessFinished((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->ahaAppInitCompleted(); break;
        case 7: _t->ahaAppInitTimeOut(); break;
        case 8: _t->ahaPingTimeOut(); break;
        case 9: _t->ahaSessionClosed(); break;
        case 10: _t->ahaSampleRateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->ahaSessionOpened(); break;
        case 12: _t->ahaAppNotOpened(); break;
        case 13: _t->startPeerSessionDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 14: _t->queryStationsDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 15: _t->queryStationContentDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 16: _t->activeStationParamDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 17: _t->presetStationParamDecoded((*reinterpret_cast< uint16_t(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->activeContentParamDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 19: _t->listContentParamDecoded((*reinterpret_cast< uint16_t(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 20: _t->queryAlbumArtDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 21: _t->queryStationLogoDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 22: _t->queryPartnerLogoDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 23: _t->queryPresetLogoDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 24: _t->queryRatingImageDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 25: _t->queryLikeImageDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 26: _t->queryCurrentStationDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 27: _t->changeClientPlaybackDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 28: _t->timeShiftContentDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 29: _t->performActionDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 30: _t->selectStationForPlaybackDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 31: _t->selectContentForPlaybackDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 32: _t->currentContentInfoDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 33: _t->notifyPlaybackOfNewStationDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 34: _t->notifyNewContentPlaybackStateDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 35: _t->notifyNewContentAvailableDecoded((*reinterpret_cast< uint16_t(*)>(_a[1])),(*reinterpret_cast< uint64_t(*)>(_a[2]))); break;
        case 36: _t->notifyContentDeletedDecoded((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 37: _t->notifyClientStatusUpdateDecoded((*reinterpret_cast< uint16_t(*)>(_a[1])),(*reinterpret_cast< uint32_t(*)>(_a[2]))); break;
        case 38: _t->onReplyWaitTimeOut(); break;
        case 39: _t->onPingCountdownTimeOut(); break;
        case 40: _t->onPingIntervalTimeOut(); break;
        case 41: _t->onDataReceived((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CAhaAbsFrameTxRx::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAhaAbsFrameTxRx::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CAhaAbsFrameTxRx,
      qt_meta_data_CAhaAbsFrameTxRx, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAhaAbsFrameTxRx::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAhaAbsFrameTxRx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAhaAbsFrameTxRx::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAhaAbsFrameTxRx))
        return static_cast<void*>(const_cast< CAhaAbsFrameTxRx*>(this));
    return QObject::qt_metacast(_clname);
}

int CAhaAbsFrameTxRx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CAhaAbsFrameTxRx::albumArtProcessFinished(QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CAhaAbsFrameTxRx::stationLogoProcessFinished(QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CAhaAbsFrameTxRx::partnerLogoProcessFinished(QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CAhaAbsFrameTxRx::presetLogoProcessFinished(QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CAhaAbsFrameTxRx::ratingImageProcessFinished(QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CAhaAbsFrameTxRx::likeImageProcessFinished(QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CAhaAbsFrameTxRx::ahaAppInitCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void CAhaAbsFrameTxRx::ahaAppInitTimeOut()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void CAhaAbsFrameTxRx::ahaPingTimeOut()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void CAhaAbsFrameTxRx::ahaSessionClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void CAhaAbsFrameTxRx::ahaSampleRateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CAhaAbsFrameTxRx::ahaSessionOpened()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void CAhaAbsFrameTxRx::ahaAppNotOpened()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void CAhaAbsFrameTxRx::startPeerSessionDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CAhaAbsFrameTxRx::queryStationsDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CAhaAbsFrameTxRx::queryStationContentDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void CAhaAbsFrameTxRx::activeStationParamDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void CAhaAbsFrameTxRx::presetStationParamDecoded(uint16_t _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void CAhaAbsFrameTxRx::activeContentParamDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void CAhaAbsFrameTxRx::listContentParamDecoded(uint16_t _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void CAhaAbsFrameTxRx::queryAlbumArtDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void CAhaAbsFrameTxRx::queryStationLogoDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void CAhaAbsFrameTxRx::queryPartnerLogoDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void CAhaAbsFrameTxRx::queryPresetLogoDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void CAhaAbsFrameTxRx::queryRatingImageDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void CAhaAbsFrameTxRx::queryLikeImageDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void CAhaAbsFrameTxRx::queryCurrentStationDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void CAhaAbsFrameTxRx::changeClientPlaybackDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 27, _a);
}

// SIGNAL 28
void CAhaAbsFrameTxRx::timeShiftContentDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void CAhaAbsFrameTxRx::performActionDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 29, _a);
}

// SIGNAL 30
void CAhaAbsFrameTxRx::selectStationForPlaybackDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 30, _a);
}

// SIGNAL 31
void CAhaAbsFrameTxRx::selectContentForPlaybackDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 31, _a);
}

// SIGNAL 32
void CAhaAbsFrameTxRx::currentContentInfoDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 32, _a);
}

// SIGNAL 33
void CAhaAbsFrameTxRx::notifyPlaybackOfNewStationDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 33, _a);
}

// SIGNAL 34
void CAhaAbsFrameTxRx::notifyNewContentPlaybackStateDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 34, _a);
}

// SIGNAL 35
void CAhaAbsFrameTxRx::notifyNewContentAvailableDecoded(uint16_t _t1, uint64_t _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 35, _a);
}

// SIGNAL 36
void CAhaAbsFrameTxRx::notifyContentDeletedDecoded(uint16_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 36, _a);
}

// SIGNAL 37
void CAhaAbsFrameTxRx::notifyClientStatusUpdateDecoded(uint16_t _t1, uint32_t _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 37, _a);
}
QT_END_MOC_NAMESPACE
