/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_GracenoteCDLookupAdaptor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_GracenoteCDLookupAdaptor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_GracenoteCDLookupAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CGracenoteCDLookupDBusAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       2,   14, // classinfo
      24,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // classinfo: key, value
      60,   30,
      77,   76,

 // signals: signature, parameters, type, tag, flags
     104,   97,   76,   76, 0x05,
     151,  134,   76,   76, 0x05,
     197,  183,   76,   76, 0x05,
     247,  229,   76,   76, 0x05,
     281,   76,   76,   76, 0x05,
     303,  299,   76,   76, 0x05,
     321,  299,   76,   76, 0x05,
     334,  299,   76,   76, 0x05,

 // slots: signature, parameters, type, tag, flags
     363,  357,   76,   76, 0x0a,
     407,  401,   76,   76, 0x0a,
     459,  446,   76,   76, 0x0a,
     516,  489,   76,   76, 0x0a,
     583,  556,   76,   76, 0x0a,
     617,   76,   76,   76, 0x0a,
     677,  630,   76,   76, 0x0a,
     743,  716,   76,   76, 0x0a,
     799,  782,   76,   76, 0x0a,
     828,  782,   76,   76, 0x0a,
     858,   76,   76,   76, 0x0a,
     900,  879,   76,   76, 0x0a,
     969,  951,   76,   76, 0x0a,
     999,   76,   76,   76, 0x0a,
    1011,   76,   76,   76, 0x0a,
    1022,   76,   76,   76, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CGracenoteCDLookupDBusAdaptor[] = {
    "CGracenoteCDLookupDBusAdaptor\0"
    "org.lge.GracenoteCDLookupDBus\0"
    "D-Bus Interface\0\0D-Bus Introspection\0"
    "status\0OnMusicIDCDDataReceived(bool)\0"
    "status,imagePath\0CoverArtRetrieved(bool,QString)\0"
    "imagePathList\0CoverArtsRetrieved(QStringList)\0"
    "status,deviceType\0GracenoteIndexingStatus(bool,int)\0"
    "GracenoteKilled()\0ver\0GNSDKVer(QString)\0"
    "GNDBVer(int)\0GNDBRegionVer(QString)\0"
    "cdtoc\0FetchGracenoteMediaDataForCD(QString)\0"
    "cdTOC\0DeleteGracenoteMediaDataForCD(QString)\0"
    "metaDataList\0RetrieveCoverArt(QStringList)\0"
    "volume,SerialNum,mountPath\0"
    "EnableMLTForVolume(int,QString,QString)\0"
    "serialNum,mountPath,volume\0"
    "USBConnected(QString,QString,int)\0"
    "jukeBoxMLT()\0"
    "serialNum,StartIndex,EndIndex,syncState,volume\0"
    "IPODConnected(QString,int,int,int,int)\0"
    "volume,serialNum,mountPath\0"
    "HighLightedVolume(int,QString,QString)\0"
    "serialNum,volume\0USBDisConnected(QString,int)\0"
    "IPODDisConnected(QString,int)\0"
    "CloseCollectionIDs()\0albumList,artistList\0"
    "RetrieveMultipleCoverArts(QStringList,QStringList)\0"
    "DestinationVolume\0ResyncAfterCopyOperation(int)\0"
    "GetSDKVer()\0GetDBVer()\0GetDBRegionVer()\0"
};

void CGracenoteCDLookupDBusAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CGracenoteCDLookupDBusAdaptor *_t = static_cast<CGracenoteCDLookupDBusAdaptor *>(_o);
        switch (_id) {
        case 0: _t->OnMusicIDCDDataReceived((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->CoverArtRetrieved((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->CoverArtsRetrieved((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 3: _t->GracenoteIndexingStatus((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->GracenoteKilled(); break;
        case 5: _t->GNSDKVer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->GNDBVer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->GNDBRegionVer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->FetchGracenoteMediaDataForCD((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->DeleteGracenoteMediaDataForCD((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->RetrieveCoverArt((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 11: _t->EnableMLTForVolume((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 12: _t->USBConnected((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 13: _t->jukeBoxMLT(); break;
        case 14: _t->IPODConnected((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4])),(*reinterpret_cast< const int(*)>(_a[5]))); break;
        case 15: _t->HighLightedVolume((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 16: _t->USBDisConnected((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 17: _t->IPODDisConnected((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 18: _t->CloseCollectionIDs(); break;
        case 19: _t->RetrieveMultipleCoverArts((*reinterpret_cast< const QStringList(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2]))); break;
        case 20: _t->ResyncAfterCopyOperation((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 21: _t->GetSDKVer(); break;
        case 22: _t->GetDBVer(); break;
        case 23: _t->GetDBRegionVer(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CGracenoteCDLookupDBusAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CGracenoteCDLookupDBusAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_CGracenoteCDLookupDBusAdaptor,
      qt_meta_data_CGracenoteCDLookupDBusAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CGracenoteCDLookupDBusAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CGracenoteCDLookupDBusAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CGracenoteCDLookupDBusAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CGracenoteCDLookupDBusAdaptor))
        return static_cast<void*>(const_cast< CGracenoteCDLookupDBusAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int CGracenoteCDLookupDBusAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    return _id;
}

// SIGNAL 0
void CGracenoteCDLookupDBusAdaptor::OnMusicIDCDDataReceived(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CGracenoteCDLookupDBusAdaptor::CoverArtRetrieved(bool _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CGracenoteCDLookupDBusAdaptor::CoverArtsRetrieved(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CGracenoteCDLookupDBusAdaptor::GracenoteIndexingStatus(bool _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CGracenoteCDLookupDBusAdaptor::GracenoteKilled()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void CGracenoteCDLookupDBusAdaptor::GNSDKVer(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CGracenoteCDLookupDBusAdaptor::GNDBVer(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CGracenoteCDLookupDBusAdaptor::GNDBRegionVer(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
