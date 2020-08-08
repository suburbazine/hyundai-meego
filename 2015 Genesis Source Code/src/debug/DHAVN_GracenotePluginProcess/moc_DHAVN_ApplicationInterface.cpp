/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_ApplicationInterface.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_ApplicationInterface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_ApplicationInterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CApplicationInterface[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      49,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      26,       // signalCount

 // signals: signature, parameters, type, tag, flags
      31,   23,   22,   22, 0x05,
      54,   46,   22,   22, 0x05,
      75,   46,   22,   22, 0x05,
     110,   97,   22,   22, 0x05,
     161,  140,   22,   22, 0x05,
     239,  212,   22,   22, 0x05,
     297,  270,   22,   22, 0x05,
     348,  331,   22,   22, 0x05,
     424,  377,   22,   22, 0x05,
     463,  331,   22,   22, 0x05,
     493,  212,   22,   22, 0x05,
     532,   22,   22,   22, 0x05,
     564,  551,   22,   22, 0x05,
     594,   22,   22,   22, 0x05,
     620,  613,   22,   22, 0x05,
     667,  650,   22,   22, 0x05,
     713,  699,   22,   22, 0x05,
     763,  745,   22,   22, 0x05,
     797,   22,   22,   22, 0x05,
     815,  613,   22,   22, 0x05,
     849,  845,   22,   22, 0x05,
     862,  845,   22,   22, 0x05,
     880,  845,   22,   22, 0x05,
     903,   22,   22,   22, 0x05,
     915,   22,   22,   22, 0x05,
     926,   22,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
     943,   46,   22,   22, 0x0a,
     982,   46,   22,   22, 0x0a,
    1012,   97,   22,   22, 0x0a,
    1042,  140,   22,   22, 0x0a,
    1093,  212,   22,   22, 0x0a,
    1133,   22,   22,   22, 0x0a,
    1146,  270,   22,   22, 0x0a,
    1180,  331,   22,   22, 0x0a,
    1209,  377,   22,   22, 0x0a,
    1248,  331,   22,   22, 0x0a,
    1278,  212,   22,   22, 0x0a,
    1317,   22,   22,   22, 0x0a,
    1338,  551,   22,   22, 0x0a,
    1368,   22,   22,   22, 0x0a,
    1385,   22,   22,   22, 0x0a,
    1401,  745,   22,   22, 0x0a,
    1437,   22,   22,   22, 0x0a,
    1457,  845,   22,   22, 0x0a,
    1472,  845,   22,   22, 0x0a,
    1492,  845,   22,   22, 0x0a,
    1517,   22,   22,   22, 0x0a,
    1529,   22,   22,   22, 0x0a,
    1540,   22,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CApplicationInterface[] = {
    "CApplicationInterface\0\0variant\0"
    "start(QString)\0tocInfo\0fetchCDData(QString)\0"
    "deleteCDData(QString)\0metaDataList\0"
    "retrieveCoverArt(QStringList)\0"
    "albumList,artistList\0"
    "retrieveMultipleCoverArts(QStringList,QStringList)\0"
    "volume,serialNum,mountPath\0"
    "enableMLT(int,QString,QString)\0"
    "serialNum,mountPath,volume\0"
    "usbConnected(QString,QString,int)\0"
    "serialNum,volume\0usbDisconnected(QString,int)\0"
    "serialNum,startIndex,endIndex,syncState,volume\0"
    "ipodConnected(QString,int,int,int,int)\0"
    "ipodDisConnected(QString,int)\0"
    "highLightedVolume(int,QString,QString)\0"
    "closeCollections()\0targetVolume\0"
    "resyncAfterCopyOperation(int)\0"
    "jukeBoxMLTSignal()\0status\0"
    "OnMusicIDCDDataReceived(bool)\0"
    "status,imagePath\0CoverArtRetrieved(bool,QString)\0"
    "imagePathList\0CoverArtsRetrieved(QStringList)\0"
    "status,deviceType\0GracenoteIndexingStatus(bool,int)\0"
    "GracenoteKilled()\0GracenoteLaunchComplete(bool)\0"
    "ver\0GNDBVer(int)\0GNSDKVer(QString)\0"
    "GNDBRegionVer(QString)\0getSDKVer()\0"
    "getDBVer()\0getDBRegionVer()\0"
    "FetchMediaDataFromCDTOCLookup(QString)\0"
    "DeleteMediaDataForCD(QString)\0"
    "RetrieveCoverArt(QStringList)\0"
    "RetrieveMultipleCoverArts(QStringList,QStringList)\0"
    "EnableMLTForVolume(int,QString,QString)\0"
    "jukeBoxMLT()\0USBConnected(QString,QString,int)\0"
    "USBDisConnected(QString,int)\0"
    "IPODConnected(QString,int,int,int,int)\0"
    "IPODDisConnected(QString,int)\0"
    "HighLightedVolume(int,QString,QString)\0"
    "CloseCollectionIDs()\0ResyncAfterCopyOperation(int)\0"
    "onChildStarted()\0onExitFromApp()\0"
    "OnGracenoteIndexingStatus(bool,int)\0"
    "onGracenoteKilled()\0onGNDBVer(int)\0"
    "onGNSDKVer(QString)\0onGNDBRegionVer(QString)\0"
    "GetSDKVer()\0GetDBVer()\0GetDBRegionVer()\0"
};

void CApplicationInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CApplicationInterface *_t = static_cast<CApplicationInterface *>(_o);
        switch (_id) {
        case 0: _t->start((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->fetchCDData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->deleteCDData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->retrieveCoverArt((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 4: _t->retrieveMultipleCoverArts((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 5: _t->enableMLT((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 6: _t->usbConnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: _t->usbDisconnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->ipodConnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 9: _t->ipodDisConnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->highLightedVolume((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 11: _t->closeCollections(); break;
        case 12: _t->resyncAfterCopyOperation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->jukeBoxMLTSignal(); break;
        case 14: _t->OnMusicIDCDDataReceived((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->CoverArtRetrieved((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 16: _t->CoverArtsRetrieved((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 17: _t->GracenoteIndexingStatus((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->GracenoteKilled(); break;
        case 19: _t->GracenoteLaunchComplete((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: _t->GNDBVer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->GNSDKVer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 22: _t->GNDBRegionVer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->getSDKVer(); break;
        case 24: _t->getDBVer(); break;
        case 25: _t->getDBRegionVer(); break;
        case 26: _t->FetchMediaDataFromCDTOCLookup((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 27: _t->DeleteMediaDataForCD((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 28: _t->RetrieveCoverArt((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 29: _t->RetrieveMultipleCoverArts((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 30: _t->EnableMLTForVolume((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 31: _t->jukeBoxMLT(); break;
        case 32: _t->USBConnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 33: _t->USBDisConnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 34: _t->IPODConnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 35: _t->IPODDisConnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 36: _t->HighLightedVolume((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 37: _t->CloseCollectionIDs(); break;
        case 38: _t->ResyncAfterCopyOperation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 39: _t->onChildStarted(); break;
        case 40: _t->onExitFromApp(); break;
        case 41: _t->OnGracenoteIndexingStatus((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 42: _t->onGracenoteKilled(); break;
        case 43: _t->onGNDBVer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 44: _t->onGNSDKVer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 45: _t->onGNDBRegionVer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 46: _t->GetSDKVer(); break;
        case 47: _t->GetDBVer(); break;
        case 48: _t->GetDBRegionVer(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CApplicationInterface::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CApplicationInterface::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CApplicationInterface,
      qt_meta_data_CApplicationInterface, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CApplicationInterface::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CApplicationInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CApplicationInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CApplicationInterface))
        return static_cast<void*>(const_cast< CApplicationInterface*>(this));
    return QObject::qt_metacast(_clname);
}

int CApplicationInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 49)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 49;
    }
    return _id;
}

// SIGNAL 0
void CApplicationInterface::start(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CApplicationInterface::fetchCDData(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CApplicationInterface::deleteCDData(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CApplicationInterface::retrieveCoverArt(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CApplicationInterface::retrieveMultipleCoverArts(QStringList _t1, QStringList _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CApplicationInterface::enableMLT(int _t1, QString _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CApplicationInterface::usbConnected(QString _t1, QString _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CApplicationInterface::usbDisconnected(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CApplicationInterface::ipodConnected(QString _t1, int _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CApplicationInterface::ipodDisConnected(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CApplicationInterface::highLightedVolume(int _t1, QString _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CApplicationInterface::closeCollections()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void CApplicationInterface::resyncAfterCopyOperation(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CApplicationInterface::jukeBoxMLTSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void CApplicationInterface::OnMusicIDCDDataReceived(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CApplicationInterface::CoverArtRetrieved(bool _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void CApplicationInterface::CoverArtsRetrieved(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void CApplicationInterface::GracenoteIndexingStatus(bool _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void CApplicationInterface::GracenoteKilled()
{
    QMetaObject::activate(this, &staticMetaObject, 18, 0);
}

// SIGNAL 19
void CApplicationInterface::GracenoteLaunchComplete(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void CApplicationInterface::GNDBVer(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void CApplicationInterface::GNSDKVer(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void CApplicationInterface::GNDBRegionVer(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void CApplicationInterface::getSDKVer()
{
    QMetaObject::activate(this, &staticMetaObject, 23, 0);
}

// SIGNAL 24
void CApplicationInterface::getDBVer()
{
    QMetaObject::activate(this, &staticMetaObject, 24, 0);
}

// SIGNAL 25
void CApplicationInterface::getDBRegionVer()
{
    QMetaObject::activate(this, &staticMetaObject, 25, 0);
}
QT_END_MOC_NAMESPACE
