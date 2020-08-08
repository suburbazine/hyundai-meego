/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_IPodController_playEngineTaskHandler.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TaskHandlers/playEngineTaskHandler/include/DHAVN_IPodController_playEngineTaskHandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_IPodController_playEngineTaskHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPlayEngineTaskHandler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      36,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      19,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   24,   23,   23, 0x05,
      52,   23,   23,   23, 0x05,
      67,   23,   23,   23, 0x05,
      90,   23,   23,   23, 0x05,
     110,   23,   23,   23, 0x05,
     123,   23,   23,   23, 0x05,
     147,  141,   23,   23, 0x05,
     167,  141,   23,   23, 0x05,
     195,  186,   23,   23, 0x05,
     223,  218,   23,   23, 0x05,
     285,  254,   23,   23, 0x05,
     354,  333,   23,   23, 0x25,
     428,  397,   23,   23, 0x05,
     528,  504,   23,   23, 0x05,
     600,   23,   23,   23, 0x05,
     632,   23,   23,   23, 0x05,
     663,   23,   23,   23, 0x05,
     699,  693,   23,   23, 0x05,
     730,   23,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
     777,  755,   23,   23, 0x0a,
     813,   23,   23,   23, 0x0a,
     841,   23,   23,   23, 0x0a,
     864,  857,   23,   23, 0x0a,
     886,   23,   23,   23, 0x0a,
     901,   23,   23,   23, 0x0a,
     920,   23,   23,   23, 0x0a,
     937,  931,   23,   23, 0x0a,
     973,   23,   23,   23, 0x0a,
    1008,   23,   23,   23, 0x0a,
    1042,   23,   23,   23, 0x0a,
    1057,   23, 1053,   23, 0x0a,
    1068,   23,   23,   23, 0x0a,
    1094,   23,   23,   23, 0x0a,
    1122,   23,   23,   23, 0x0a,
    1152,   23,   23,   23, 0x0a,
    1183,   23,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CPlayEngineTaskHandler[] = {
    "CPlayEngineTaskHandler\0\0nExitCode\0"
    "mainLoopQuit(int)\0mainLoopQuit()\0"
    "OnPositionChanged(int)\0OnStateChanged(int)\0"
    "OnError(int)\0OnScanCompleted()\0index\0"
    "OnTrackChanged(int)\0OnModeChanged(int)\0"
    "mseconds\0OnDurationChanged(int)\0info\0"
    "OnCurrentMediaInfo(MediaInfo*)\0"
    "albumName,artistName,forceFlag\0"
    "fetchCoversForBasicPlayer(QString,QString,bool)\0"
    "albumName,artistName\0"
    "fetchCoversForBasicPlayer(QString,QString)\0"
    "dataList,queryId,offset,reqUID\0"
    "RequestComplete(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,int,"
    "uint)\0"
    "dataList,queryId,reqUID\0"
    "RequestComplete(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,uint"
    ")\0"
    "OnCurrentPlayingTrackCount(int)\0"
    "CurrentUIDMediaInfoRetrieved()\0"
    "CurrentPBMediaInfoRetrieved()\0pause\0"
    "OnChangeDBIndexingStatus(bool)\0"
    "dbChangedAfterDeletion()\0lingoID,commandID,buf\0"
    "packetAvailable(int,int,QByteArray)\0"
    "authorisationFinished(bool)\0startPipeline()\0"
    "source\0setPcmSource(QString)\0"
    "scanPlaylist()\0getPlayStatusATS()\0"
    "Notified()\0count\0onCurrentTrackCountChangedSlot(int)\0"
    "CurrentUIDMediaInfoRetrievedSlot()\0"
    "CurrentPBMediaInfoRetrievedSlot()\0"
    "stopPipe()\0int\0playNext()\0"
    "handlePlayIpodFilesSlot()\0"
    "handlePlaybackStoppedSlot()\0"
    "delayMetadataIndexQuerySlot()\0"
    "delayGetOnDemandMetaInfoSlot()\0"
    "PBTrackInfoTimeOutSlot()\0"
};

void CPlayEngineTaskHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CPlayEngineTaskHandler *_t = static_cast<CPlayEngineTaskHandler *>(_o);
        switch (_id) {
        case 0: _t->mainLoopQuit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->mainLoopQuit(); break;
        case 2: _t->OnPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->OnStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->OnError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->OnScanCompleted(); break;
        case 6: _t->OnTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->OnModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->OnDurationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->OnCurrentMediaInfo((*reinterpret_cast< MediaInfo*(*)>(_a[1]))); break;
        case 10: _t->fetchCoversForBasicPlayer((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 11: _t->fetchCoversForBasicPlayer((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 12: _t->RequestComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 13: _t->RequestComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        case 14: _t->OnCurrentPlayingTrackCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->CurrentUIDMediaInfoRetrieved(); break;
        case 16: _t->CurrentPBMediaInfoRetrieved(); break;
        case 17: _t->OnChangeDBIndexingStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: _t->dbChangedAfterDeletion(); break;
        case 19: _t->packetAvailable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 20: _t->authorisationFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 21: _t->startPipeline(); break;
        case 22: _t->setPcmSource((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->scanPlaylist(); break;
        case 24: _t->getPlayStatusATS(); break;
        case 25: _t->Notified(); break;
        case 26: _t->onCurrentTrackCountChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->CurrentUIDMediaInfoRetrievedSlot(); break;
        case 28: _t->CurrentPBMediaInfoRetrievedSlot(); break;
        case 29: _t->stopPipe(); break;
        case 30: { int _r = _t->playNext();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 31: _t->handlePlayIpodFilesSlot(); break;
        case 32: _t->handlePlaybackStoppedSlot(); break;
        case 33: _t->delayMetadataIndexQuerySlot(); break;
        case 34: _t->delayGetOnDemandMetaInfoSlot(); break;
        case 35: _t->PBTrackInfoTimeOutSlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CPlayEngineTaskHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CPlayEngineTaskHandler::staticMetaObject = {
    { &CTaskHandler::staticMetaObject, qt_meta_stringdata_CPlayEngineTaskHandler,
      qt_meta_data_CPlayEngineTaskHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CPlayEngineTaskHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CPlayEngineTaskHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CPlayEngineTaskHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPlayEngineTaskHandler))
        return static_cast<void*>(const_cast< CPlayEngineTaskHandler*>(this));
    return CTaskHandler::qt_metacast(_clname);
}

int CPlayEngineTaskHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CTaskHandler::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 36)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 36;
    }
    return _id;
}

// SIGNAL 0
void CPlayEngineTaskHandler::mainLoopQuit(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CPlayEngineTaskHandler::mainLoopQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CPlayEngineTaskHandler::OnPositionChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CPlayEngineTaskHandler::OnStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CPlayEngineTaskHandler::OnError(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CPlayEngineTaskHandler::OnScanCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void CPlayEngineTaskHandler::OnTrackChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CPlayEngineTaskHandler::OnModeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CPlayEngineTaskHandler::OnDurationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CPlayEngineTaskHandler::OnCurrentMediaInfo(MediaInfo * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CPlayEngineTaskHandler::fetchCoversForBasicPlayer(QString _t1, QString _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 12
void CPlayEngineTaskHandler::RequestComplete(QVector<QStringList> _t1, ETrackerAbstarctor_Query_Ids _t2, int _t3, uint _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CPlayEngineTaskHandler::RequestComplete(QVector<QStringList> _t1, ETrackerAbstarctor_Query_Ids _t2, uint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CPlayEngineTaskHandler::OnCurrentPlayingTrackCount(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CPlayEngineTaskHandler::CurrentUIDMediaInfoRetrieved()
{
    QMetaObject::activate(this, &staticMetaObject, 15, 0);
}

// SIGNAL 16
void CPlayEngineTaskHandler::CurrentPBMediaInfoRetrieved()
{
    QMetaObject::activate(this, &staticMetaObject, 16, 0);
}

// SIGNAL 17
void CPlayEngineTaskHandler::OnChangeDBIndexingStatus(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void CPlayEngineTaskHandler::dbChangedAfterDeletion()
{
    QMetaObject::activate(this, &staticMetaObject, 18, 0);
}
QT_END_MOC_NAMESPACE
