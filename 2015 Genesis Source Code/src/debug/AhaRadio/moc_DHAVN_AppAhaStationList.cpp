/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppAhaStationList.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppAhaStationList.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppAhaStationList.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CAppAhaStationList[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      47,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      46,   20,   19,   19, 0x05,
      72,   20,   19,   19, 0x05,
     121,   95,   19,   19, 0x05,
     148,   95,   19,   19, 0x05,
     183,  176,   19,   19, 0x05,
     228,  215,   19,   19, 0x05,
     253,   19,   19,   19, 0x05,
     296,  281,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
     337,  320,   19,   19, 0x0a,
     407,  390,   19,   19, 0x0a,
     466,  460,   19,   19, 0x0a,
     521,  503,   19,   19, 0x0a,
     595,  572,   19,   19, 0x0a,
     645,   19,   19,   19, 0x0a,
     682,  668,   19,   19, 0x0a,
     704,   19,   19,   19, 0x0a,
     725,   19,   19,   19, 0x0a,
     746,   19,   19,   19, 0x0a,
     774,   19,   19,   19, 0x0a,
     788,   19,   19,   19, 0x0a,
     802,  460,   19,   19, 0x0a,
     835,  829,   19,   19, 0x0a,
     858,  460,   19,   19, 0x0a,
     882,  829,   19,   19, 0x0a,
     902,  460,   19,   19, 0x0a,
     930,  829,   19,   19, 0x0a,
     962,   19,  954,   19, 0x0a,
    1008,  987,  954,   19, 0x0a,
    1047, 1037,  954,   19, 0x0a,
    1074,   19, 1070,   19, 0x0a,
    1093,   19, 1070,   19, 0x0a,
    1132,   19, 1070,   19, 0x0a,
    1153,   19, 1070,   19, 0x0a,
    1181, 1037, 1176,   19, 0x0a,
    1222, 1205,   19,   19, 0x0a,
    1252,   19, 1070,   19, 0x0a,
    1294, 1279,   19,   19, 0x0a,
    1321,   19, 1070,   19, 0x0a,
    1362, 1345,   19,   19, 0x0a,
    1391,   19, 1070,   19, 0x0a,
    1417,   19, 1070,   19, 0x0a,
    1434,   19, 1070,   19, 0x0a,
    1448,   19, 1070,   19, 0x0a,
    1466,   19, 1070,   19, 0x0a,
    1483,   19, 1070,   19, 0x0a,
    1497,   19, 1070,   19, 0x0a,

 // methods: signature, parameters, type, tag, flags
    1515,   19,   19,   19, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_CAppAhaStationList[] = {
    "CAppAhaStationList\0\0model_index,nStationCount\0"
    "modelPresetReady(int,int)\0"
    "modelLBSReady(int,int)\0model_index,nContentCount\0"
    "modelContentReady(int,int)\0"
    "modelContentScroll(int,int)\0isPlay\0"
    "modelStationNowListenning(bool)\0"
    "bRepeatState\0Content1stSkipBack(bool)\0"
    "receivingStationHidePopup()\0playBackStatus\0"
    "setBufferingStatus(int)\0qVectStationInfo\0"
    "OnStationListLoaded(QVector<AhaStationBriefInfo_T>&)\0"
    "qVectContentInfo\0"
    "OnContentListLoaded(QVector<AhaContentBriefInfo_T>&)\0"
    "appID\0OnNotifyContentIDChanged(qulonglong)\0"
    "appID,play_status\0"
    "OnNotifyPlaybackStateChangedSlot(uint64_t,uint8_t)\0"
    "nIsLBS,appID,imagePath\0"
    "OnStationLogoReceived(uint8_t,qulonglong,QString)\0"
    "OnTimeout_DelayEvent()\0interval,type\0"
    "requestTimer(int,int)\0requestStationList()\0"
    "requestContentList()\0requestStationListInTrack()\0"
    "playStation()\0playContent()\0"
    "selectPresetID(qulonglong)\0index\0"
    "selectPresetIndex(int)\0selectLBSID(qulonglong)\0"
    "selectLBSIndex(int)\0selectContentID(qulonglong)\0"
    "selectContentIndex(int)\0QString\0"
    "getLastSelectedStation()\0tuneIndex,bTrackView\0"
    "getStationNameTune(int,bool)\0tuneIndex\0"
    "getStationArtTune(int)\0int\0"
    "getStationIdTune()\0"
    "getStationIndexUsedfromStationIdTune()\0"
    "getStationIdTuneUp()\0getStationIdTuneDown()\0"
    "bool\0checkSameStationId(int)\0"
    "stationFocusInex\0setStationListFocusIndex(int)\0"
    "getStationListFocusIndex()\0trackTuneIndex\0"
    "setTrackViewTuneIndex(int)\0"
    "getTrackViewTuneIndex()\0contentTuneIndex\0"
    "setContentListTuneIndex(int)\0"
    "getContentListTuneIndex()\0getPresetIndex()\0"
    "getLBSIndex()\0getContentIndex()\0"
    "getPresetCount()\0getLBSCount()\0"
    "getContentCount()\0modelClear()\0"
};

void CAppAhaStationList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CAppAhaStationList *_t = static_cast<CAppAhaStationList *>(_o);
        switch (_id) {
        case 0: _t->modelPresetReady((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->modelLBSReady((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->modelContentReady((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->modelContentScroll((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->modelStationNowListenning((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->Content1stSkipBack((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 6: _t->receivingStationHidePopup(); break;
        case 7: _t->setBufferingStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->OnStationListLoaded((*reinterpret_cast< QVector<AhaStationBriefInfo_T>(*)>(_a[1]))); break;
        case 9: _t->OnContentListLoaded((*reinterpret_cast< QVector<AhaContentBriefInfo_T>(*)>(_a[1]))); break;
        case 10: _t->OnNotifyContentIDChanged((*reinterpret_cast< const qulonglong(*)>(_a[1]))); break;
        case 11: _t->OnNotifyPlaybackStateChangedSlot((*reinterpret_cast< uint64_t(*)>(_a[1])),(*reinterpret_cast< uint8_t(*)>(_a[2]))); break;
        case 12: _t->OnStationLogoReceived((*reinterpret_cast< const uint8_t(*)>(_a[1])),(*reinterpret_cast< const qulonglong(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 13: _t->OnTimeout_DelayEvent(); break;
        case 14: _t->requestTimer((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->requestStationList(); break;
        case 16: _t->requestContentList(); break;
        case 17: _t->requestStationListInTrack(); break;
        case 18: _t->playStation(); break;
        case 19: _t->playContent(); break;
        case 20: _t->selectPresetID((*reinterpret_cast< qulonglong(*)>(_a[1]))); break;
        case 21: _t->selectPresetIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->selectLBSID((*reinterpret_cast< qulonglong(*)>(_a[1]))); break;
        case 23: _t->selectLBSIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->selectContentID((*reinterpret_cast< qulonglong(*)>(_a[1]))); break;
        case 25: _t->selectContentIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: { QString _r = _t->getLastSelectedStation();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 27: { QString _r = _t->getStationNameTune((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 28: { QString _r = _t->getStationArtTune((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 29: { int _r = _t->getStationIdTune();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 30: { int _r = _t->getStationIndexUsedfromStationIdTune();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 31: { int _r = _t->getStationIdTuneUp();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 32: { int _r = _t->getStationIdTuneDown();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 33: { bool _r = _t->checkSameStationId((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 34: _t->setStationListFocusIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: { int _r = _t->getStationListFocusIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 36: _t->setTrackViewTuneIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: { int _r = _t->getTrackViewTuneIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 38: _t->setContentListTuneIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 39: { int _r = _t->getContentListTuneIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 40: { int _r = _t->getPresetIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 41: { int _r = _t->getLBSIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 42: { int _r = _t->getContentIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 43: { int _r = _t->getPresetCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 44: { int _r = _t->getLBSCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 45: { int _r = _t->getContentCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 46: _t->modelClear(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CAppAhaStationList::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAppAhaStationList::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CAppAhaStationList,
      qt_meta_data_CAppAhaStationList, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAppAhaStationList::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAppAhaStationList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAppAhaStationList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAppAhaStationList))
        return static_cast<void*>(const_cast< CAppAhaStationList*>(this));
    return QObject::qt_metacast(_clname);
}

int CAppAhaStationList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CAppAhaStationList::modelPresetReady(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CAppAhaStationList::modelLBSReady(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CAppAhaStationList::modelContentReady(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CAppAhaStationList::modelContentScroll(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CAppAhaStationList::modelStationNowListenning(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CAppAhaStationList::Content1stSkipBack(const bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CAppAhaStationList::receivingStationHidePopup()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void CAppAhaStationList::setBufferingStatus(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
