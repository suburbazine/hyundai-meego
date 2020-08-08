/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_PndrStateHandler.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_PndrStateHandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_PndrStateHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPndrStateHandler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      35,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      19,       // signalCount

 // signals: signature, parameters, type, tag, flags
      31,   19,   18,   18, 0x05,
      69,   54,   18,   18, 0x05,
     102,   91,   18,   18, 0x05,
     146,  120,   18,   18, 0x05,
     178,   18,   18,   18, 0x05,
     220,  202,   18,   18, 0x05,
     271,  258,   18,   18, 0x05,
     308,  293,   18,   18, 0x05,
     349,  332,   18,   18, 0x05,
     415,  385,   18,   18, 0x05,
     458,  450,   18,   18, 0x05,
     496,  482,   18,   18, 0x05,
     550,  523,   18,   18, 0x05,
     599,  587,   18,   18, 0x05,
     627,  293,   18,   18, 0x05,
     668,  655,   18,   18, 0x05,
     694,   19,   18,   18, 0x05,
     726,   18,   18,   18, 0x05,
     746,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     782,  765,   18,   18, 0x0a,
     821,  765,   18,   18, 0x0a,
     861,  765,   18,   18, 0x0a,
     935,  901,   18,   18, 0x0a,
    1021,  995,   18,   18, 0x0a,
    1086, 1068,   18,   18, 0x0a,
    1126,  765,   18,   18, 0x0a,
    1170,  995,   18,   18, 0x0a,
    1224,  995,   18,   18, 0x0a,
    1279,  765,   18,   18, 0x0a,
    1332,   18,   18,   18, 0x0a,
    1357,   18,   18,   18, 0x0a,
    1407, 1380,   18,   18, 0x0a,
    1457, 1447,   18,   18, 0x0a,
    1485,   18,   18,   18, 0x0a,
    1497,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CPndrStateHandler[] = {
    "CPndrStateHandler\0\0inTrackInfo\0"
    "TrackInfo(TSTrackInfo)\0inTrackExplain\0"
    "TrackExplain(QString)\0inAlbumArt\0"
    "AlbumArt(QString)\0inRequestType,inPndrError\0"
    "Result(ERequestType,EPndrError)\0"
    "SessionTerminateAcked()\0inStationInfoList\0"
    "StationInfoList(QList<TSStationInfo>)\0"
    "inTrackToken\0TrackUpdated(quint32)\0"
    "inStationToken\0StationUpdated(quint32)\0"
    "inSearchInfoList\0SearchComplete(QList<TSSearchInfo>)\0"
    "inIndexLocation,inStationInfo\0"
    "StationAdded(quint8,TSStationInfo)\0"
    "inToken\0StationDeleted(quint32)\0"
    "inElapsedTime\0UpdateElapsedTime(quint16)\0"
    "inStationCount,inTokenList\0"
    "StationTokens(quint8,QList<quint32>)\0"
    "inSortOrder\0StationSortOrder(ESortType)\0"
    "StationArtReceived(quint32)\0inSampleRate\0"
    "AudioSamplingChanged(int)\0"
    "currentStationInfo(TSTrackInfo)\0"
    "NextBrandingImage()\0getBrandingImage()\0"
    "inRequest,inData\0"
    "OnRequestCompletion(EPndrEvent,quint8)\0"
    "OnRequestCompletion(EPndrEvent,quint16)\0"
    "OnRequestCompletion(EPndrEvent,QString)\0"
    "inRequest,inSearchId,inSearchList\0"
    "OnRequestCompletion(EPndrEvent,quint32,QList<TSSearchInfo>)\0"
    "inRequest,inData1,inData2\0"
    "OnRequestCompletion(EPndrEvent,quint32,quint8)\0"
    "inRequest,inData1\0"
    "OnRequestCompletion(EPndrEvent,quint32)\0"
    "OnRequestCompletion(EPndrEvent,TSTrackInfo)\0"
    "OnRequestCompletion(EPndrEvent,quint8,QList<quint32>)\0"
    "OnRequestCompletion(EPndrEvent,quint32,QList<quint32>)\0"
    "OnRequestCompletion(EPndrEvent,QList<TSStationInfo>)\0"
    "OnRequestBrandingImage()\0"
    "onBrandingImageTimer()\0"
    "inDeviceType,inIsConnected\0"
    "OnDeviceStatusUpdated(EDeviceType,bool)\0"
    "inErrCode\0OnErrorReceived(EPndrError)\0"
    "OnTimeout()\0onPndrSessionClosed()\0"
};

void CPndrStateHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CPndrStateHandler *_t = static_cast<CPndrStateHandler *>(_o);
        switch (_id) {
        case 0: _t->TrackInfo((*reinterpret_cast< const TSTrackInfo(*)>(_a[1]))); break;
        case 1: _t->TrackExplain((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->AlbumArt((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->Result((*reinterpret_cast< const ERequestType(*)>(_a[1])),(*reinterpret_cast< const EPndrError(*)>(_a[2]))); break;
        case 4: _t->SessionTerminateAcked(); break;
        case 5: _t->StationInfoList((*reinterpret_cast< const QList<TSStationInfo>(*)>(_a[1]))); break;
        case 6: _t->TrackUpdated((*reinterpret_cast< const quint32(*)>(_a[1]))); break;
        case 7: _t->StationUpdated((*reinterpret_cast< const quint32(*)>(_a[1]))); break;
        case 8: _t->SearchComplete((*reinterpret_cast< const QList<TSSearchInfo>(*)>(_a[1]))); break;
        case 9: _t->StationAdded((*reinterpret_cast< const quint8(*)>(_a[1])),(*reinterpret_cast< const TSStationInfo(*)>(_a[2]))); break;
        case 10: _t->StationDeleted((*reinterpret_cast< const quint32(*)>(_a[1]))); break;
        case 11: _t->UpdateElapsedTime((*reinterpret_cast< const quint16(*)>(_a[1]))); break;
        case 12: _t->StationTokens((*reinterpret_cast< const quint8(*)>(_a[1])),(*reinterpret_cast< const QList<quint32>(*)>(_a[2]))); break;
        case 13: _t->StationSortOrder((*reinterpret_cast< ESortType(*)>(_a[1]))); break;
        case 14: _t->StationArtReceived((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 15: _t->AudioSamplingChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->currentStationInfo((*reinterpret_cast< const TSTrackInfo(*)>(_a[1]))); break;
        case 17: _t->NextBrandingImage(); break;
        case 18: _t->getBrandingImage(); break;
        case 19: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint8(*)>(_a[2]))); break;
        case 20: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint16(*)>(_a[2]))); break;
        case 21: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 22: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2])),(*reinterpret_cast< const QList<TSSearchInfo>(*)>(_a[3]))); break;
        case 23: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2])),(*reinterpret_cast< const quint8(*)>(_a[3]))); break;
        case 24: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2]))); break;
        case 25: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const TSTrackInfo(*)>(_a[2]))); break;
        case 26: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint8(*)>(_a[2])),(*reinterpret_cast< const QList<quint32>(*)>(_a[3]))); break;
        case 27: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2])),(*reinterpret_cast< const QList<quint32>(*)>(_a[3]))); break;
        case 28: _t->OnRequestCompletion((*reinterpret_cast< const EPndrEvent(*)>(_a[1])),(*reinterpret_cast< const QList<TSStationInfo>(*)>(_a[2]))); break;
        case 29: _t->OnRequestBrandingImage(); break;
        case 30: _t->onBrandingImageTimer(); break;
        case 31: _t->OnDeviceStatusUpdated((*reinterpret_cast< const EDeviceType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 32: _t->OnErrorReceived((*reinterpret_cast< EPndrError(*)>(_a[1]))); break;
        case 33: _t->OnTimeout(); break;
        case 34: _t->onPndrSessionClosed(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CPndrStateHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CPndrStateHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CPndrStateHandler,
      qt_meta_data_CPndrStateHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CPndrStateHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CPndrStateHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CPndrStateHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPndrStateHandler))
        return static_cast<void*>(const_cast< CPndrStateHandler*>(this));
    return QObject::qt_metacast(_clname);
}

int CPndrStateHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CPndrStateHandler::TrackInfo(const TSTrackInfo & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CPndrStateHandler::TrackExplain(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CPndrStateHandler::AlbumArt(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CPndrStateHandler::Result(const ERequestType _t1, const EPndrError _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CPndrStateHandler::SessionTerminateAcked()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void CPndrStateHandler::StationInfoList(const QList<TSStationInfo> & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CPndrStateHandler::TrackUpdated(const quint32 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CPndrStateHandler::StationUpdated(const quint32 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CPndrStateHandler::SearchComplete(const QList<TSSearchInfo> & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CPndrStateHandler::StationAdded(const quint8 _t1, const TSStationInfo & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CPndrStateHandler::StationDeleted(const quint32 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CPndrStateHandler::UpdateElapsedTime(const quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CPndrStateHandler::StationTokens(const quint8 _t1, const QList<quint32> & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CPndrStateHandler::StationSortOrder(ESortType _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CPndrStateHandler::StationArtReceived(quint32 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CPndrStateHandler::AudioSamplingChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void CPndrStateHandler::currentStationInfo(const TSTrackInfo & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void CPndrStateHandler::NextBrandingImage()
{
    QMetaObject::activate(this, &staticMetaObject, 17, 0);
}

// SIGNAL 18
void CPndrStateHandler::getBrandingImage()
{
    QMetaObject::activate(this, &staticMetaObject, 18, 0);
}
QT_END_MOC_NAMESPACE
