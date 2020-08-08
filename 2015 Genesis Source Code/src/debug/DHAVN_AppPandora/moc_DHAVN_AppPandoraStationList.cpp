/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppPandoraStationList.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppPandoraStationList.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppPandoraStationList.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PandoraListReqType[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
      19, 0x0,    5,   18,

 // enum data: key, value
      43, uint(PandoraListReqType::ENONELIST),
      53, uint(PandoraListReqType::EFIRSTLIST),
      64, uint(PandoraListReqType::ENEXTLIST),
      74, uint(PandoraListReqType::EPREVLIST),
      84, uint(PandoraListReqType::EQUICKLIST),

       0        // eod
};

static const char qt_meta_stringdata_PandoraListReqType[] = {
    "PandoraListReqType\0EStationListRequestType\0"
    "ENONELIST\0EFIRSTLIST\0ENEXTLIST\0EPREVLIST\0"
    "EQUICKLIST\0"
};

void PandoraListReqType::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData PandoraListReqType::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PandoraListReqType::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PandoraListReqType,
      qt_meta_data_PandoraListReqType, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PandoraListReqType::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PandoraListReqType::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PandoraListReqType::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PandoraListReqType))
        return static_cast<void*>(const_cast< PandoraListReqType*>(this));
    return QObject::qt_metacast(_clname);
}

int PandoraListReqType::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_CAppPandoraStationList[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      55,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      15,       // signalCount

 // signals: signature, parameters, type, tag, flags
      36,   24,   23,   23, 0x05,
     108,   54,   23,   23, 0x05,
     165,  157,   23,   23, 0x05,
     207,  185,   23,   23, 0x05,
     271,  236,   23,   23, 0x05,
     303,   23,   23,   23, 0x05,
     340,  323,   23,   23, 0x05,
     370,  359,   23,   23, 0x05,
     397,  389,   23,   23, 0x05,
     445,  418,   23,   23, 0x05,
     480,   23,   23,   23, 0x05,
     494,   23,   23,   23, 0x05,
     517,   23,   23,   23, 0x05,
     557,  536,   23,   23, 0x05,
     585,   23,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
     622,  604,   23,   23, 0x0a,
     689,  662,   23,   23, 0x0a,
     736,  728,   23,   23, 0x0a,
     792,  762,   23,   23, 0x0a,
     836,  829,   23,   23, 0x0a,
     866,  728,   23,   23, 0x0a,
     911,  896,   23,   23, 0x0a,
     949,  937,   23,   23, 0x0a,

 // methods: signature, parameters, type, tag, flags
    1015,  983,   23,   23, 0x02,
    1062, 1052,   23,   23, 0x02,
    1116, 1086,   23,   23, 0x02,
    1154,   23, 1150,   23, 0x02,
    1178,   23, 1150,   23, 0x02,
    1201,   23, 1150,   23, 0x02,
    1222,   23, 1150,   23, 0x02,
    1252, 1246,   23,   23, 0x02,
    1283, 1271,   23,   23, 0x02,
    1306,   23,   23,   23, 0x02,
    1332,   23,   23,   23, 0x02,
    1345,   23,   23,   23, 0x02,
    1381, 1367, 1150,   23, 0x02,
    1415,   23,   23,   23, 0x02,
    1434,   23, 1429,   23, 0x02,
    1452,   23, 1429,   23, 0x02,
    1470,   23,   23,   23, 0x02,
    1503, 1489,   23,   23, 0x02,
    1535, 1246, 1527,   23, 0x02,
    1560,  728, 1527,   23, 0x02,
    1597, 1587,   23,   23, 0x02,
    1618,   23,   23,   23, 0x02,
    1628,   23,   23,   23, 0x02,
    1641,   23, 1429,   23, 0x02,
    1660,   23,   23,   23, 0x02,
    1678, 1246, 1527,   23, 0x02,
    1695,   23, 1150,   23, 0x02,
    1714,   23, 1150,   23, 0x02,
    1747, 1736, 1527,   23, 0x02,
    1765, 1246, 1429,   23, 0x02,
    1786,   23, 1527,   23, 0x02,
    1807,   23, 1429,   23, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_CAppPandoraStationList[] = {
    "CAppPandoraStationList\0\0inListCount\0"
    "stationToken(int)\0"
    "inStartIndex,inCount,inType,inCurrPlayIndex,inReqType\0"
    "stationListReceived(int,int,quint16,qint8,qint8)\0"
    "inIndex\0stationDeleted(int)\0"
    "inIndex,inStationName\0"
    "stationInserted(int,QString)\0"
    "inStationArtIndex,inStationArtPath\0"
    "stationArtReceived(int,QString)\0"
    "toastLoadingPopup()\0inStationArtPath\0"
    "currStArt(QString)\0playStatus\0"
    "setPlayState(bool)\0inError\0"
    "handleError(quint16)\0token,inStationName,isCurr\0"
    "tunedStation(quint32,QString,bool)\0"
    "requestFail()\0preFetchingCompleted()\0"
    "preFetchingStart()\0inActiveStationToken\0"
    "activeStationToken(quint32)\0"
    "noStationPresent()\0inStationInfoList\0"
    "OnStationInfoList(QList<TSStationInfo>)\0"
    "inStationCount,inTokenList\0"
    "OnStationTokens(quint8,QList<quint32>)\0"
    "inToken\0OnStationDeleted(quint32)\0"
    "inIndexLocation,inStationInfo\0"
    "OnStationAdded(quint8,TSStationInfo)\0"
    "inType\0OnStationSortOrder(ESortType)\0"
    "OnStationArtReceived(quint32)\0"
    "inStationToken\0OnStationUpdated(quint32)\0"
    "inTrackInfo\0OnCurrentStationInfo(TSTrackInfo)\0"
    "inMaxCount,inActiveStationToken\0"
    "GetFirstStationListSlot(int,quint32)\0"
    "inReqType\0GetNextStationList(int)\0"
    "sortType,inActiveStationToken\0"
    "GetSortedStationList(int,quint32)\0int\0"
    "GetTotalStationsCount()\0GetCurrentStartIndex()\0"
    "GetCurrentEndIndex()\0GetCurrentActiveIndex()\0"
    "index\0SelectStation(int)\0stationName\0"
    "SelectStation(QString)\0CancelStationArtRequest()\0"
    "ClearCache()\0ClearCurrentRequest()\0"
    "alphabet,isBT\0GetIndexForAlphabet(QString,bool)\0"
    "PlayShuffle()\0bool\0IsNextItemExist()\0"
    "IsPrevItemExist()\0ResetListReqType()\0"
    "inListReqType\0ResetListReqType(qint8)\0"
    "QString\0IsStationArtPresent(int)\0"
    "StationArtPresent(quint32)\0isForward\0"
    "GetStationName(bool)\0TuneOff()\0"
    "TuneSelect()\0IsPendingRequest()\0"
    "FetchStationArt()\0StationName(int)\0"
    "StationListCount()\0StationListSortType()\0"
    "inStr,isBT\0Key(QString,bool)\0"
    "IsSharedStation(int)\0CurrentStationName()\0"
    "IsPreFetchingStations()\0"
};

void CAppPandoraStationList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CAppPandoraStationList *_t = static_cast<CAppPandoraStationList *>(_o);
        switch (_id) {
        case 0: _t->stationToken((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 1: _t->stationListReceived((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const quint16(*)>(_a[3])),(*reinterpret_cast< qint8(*)>(_a[4])),(*reinterpret_cast< qint8(*)>(_a[5]))); break;
        case 2: _t->stationDeleted((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 3: _t->stationInserted((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->stationArtReceived((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->toastLoadingPopup(); break;
        case 6: _t->currStArt((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->setPlayState((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 8: _t->handleError((*reinterpret_cast< const quint16(*)>(_a[1]))); break;
        case 9: _t->tunedStation((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 10: _t->requestFail(); break;
        case 11: _t->preFetchingCompleted(); break;
        case 12: _t->preFetchingStart(); break;
        case 13: _t->activeStationToken((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 14: _t->noStationPresent(); break;
        case 15: _t->OnStationInfoList((*reinterpret_cast< const QList<TSStationInfo>(*)>(_a[1]))); break;
        case 16: _t->OnStationTokens((*reinterpret_cast< const quint8(*)>(_a[1])),(*reinterpret_cast< const QList<quint32>(*)>(_a[2]))); break;
        case 17: _t->OnStationDeleted((*reinterpret_cast< const quint32(*)>(_a[1]))); break;
        case 18: _t->OnStationAdded((*reinterpret_cast< const quint8(*)>(_a[1])),(*reinterpret_cast< const TSStationInfo(*)>(_a[2]))); break;
        case 19: _t->OnStationSortOrder((*reinterpret_cast< const ESortType(*)>(_a[1]))); break;
        case 20: _t->OnStationArtReceived((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 21: _t->OnStationUpdated((*reinterpret_cast< const quint32(*)>(_a[1]))); break;
        case 22: _t->OnCurrentStationInfo((*reinterpret_cast< const TSTrackInfo(*)>(_a[1]))); break;
        case 23: _t->GetFirstStationListSlot((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2]))); break;
        case 24: _t->GetNextStationList((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->GetSortedStationList((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2]))); break;
        case 26: { int _r = _t->GetTotalStationsCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 27: { int _r = _t->GetCurrentStartIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 28: { int _r = _t->GetCurrentEndIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 29: { int _r = _t->GetCurrentActiveIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 30: _t->SelectStation((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 31: _t->SelectStation((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 32: _t->CancelStationArtRequest(); break;
        case 33: _t->ClearCache(); break;
        case 34: _t->ClearCurrentRequest(); break;
        case 35: { int _r = _t->GetIndexForAlphabet((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 36: _t->PlayShuffle(); break;
        case 37: { bool _r = _t->IsNextItemExist();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 38: { bool _r = _t->IsPrevItemExist();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 39: _t->ResetListReqType(); break;
        case 40: _t->ResetListReqType((*reinterpret_cast< qint8(*)>(_a[1]))); break;
        case 41: { QString _r = _t->IsStationArtPresent((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 42: { QString _r = _t->StationArtPresent((*reinterpret_cast< quint32(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 43: _t->GetStationName((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 44: _t->TuneOff(); break;
        case 45: _t->TuneSelect(); break;
        case 46: { bool _r = _t->IsPendingRequest();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 47: _t->FetchStationArt(); break;
        case 48: { QString _r = _t->StationName((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 49: { int _r = _t->StationListCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 50: { int _r = _t->StationListSortType();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 51: { QString _r = _t->Key((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 52: { bool _r = _t->IsSharedStation((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 53: { QString _r = _t->CurrentStationName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 54: { bool _r = _t->IsPreFetchingStations();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CAppPandoraStationList::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAppPandoraStationList::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CAppPandoraStationList,
      qt_meta_data_CAppPandoraStationList, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAppPandoraStationList::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAppPandoraStationList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAppPandoraStationList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAppPandoraStationList))
        return static_cast<void*>(const_cast< CAppPandoraStationList*>(this));
    return QObject::qt_metacast(_clname);
}

int CAppPandoraStationList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 55)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 55;
    }
    return _id;
}

// SIGNAL 0
void CAppPandoraStationList::stationToken(const int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CAppPandoraStationList::stationListReceived(const int _t1, int _t2, const quint16 _t3, qint8 _t4, qint8 _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CAppPandoraStationList::stationDeleted(const int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CAppPandoraStationList::stationInserted(const int _t1, const QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CAppPandoraStationList::stationArtReceived(const int _t1, const QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CAppPandoraStationList::toastLoadingPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void CAppPandoraStationList::currStArt(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CAppPandoraStationList::setPlayState(const bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CAppPandoraStationList::handleError(const quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CAppPandoraStationList::tunedStation(quint32 _t1, QString _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CAppPandoraStationList::requestFail()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}

// SIGNAL 11
void CAppPandoraStationList::preFetchingCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void CAppPandoraStationList::preFetchingStart()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void CAppPandoraStationList::activeStationToken(quint32 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CAppPandoraStationList::noStationPresent()
{
    QMetaObject::activate(this, &staticMetaObject, 14, 0);
}
QT_END_MOC_NAMESPACE
