/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Notifier.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/util/DHAVN_AppMediaPlayer_Notifier.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Notifier.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ExternalNotifier[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      57,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      59,   53,   17,   17, 0x0a,
      87,   79,   17,   17, 0x0a,
     116,  110,   17,   17, 0x0a,
     140,   79,   17,   17, 0x0a,
     178,  163,   17,   17, 0x0a,
     232,  202,   17,   17, 0x0a,
     333,  268,   17,   17, 0x0a,
     427,  421,   17,   17, 0x0a,
     471,  462,   17,   17, 0x0a,
     519,  494,   17,   17, 0x0a,
     580,  573,   17,   17, 0x0a,
     605,  601,   17,   17, 0x0a,
     644,  626,   17,   17, 0x0a,
     720,  691,   17,   17, 0x0a,
     826,  759,   17,   17, 0x0a,
     906,  900,   17,   17, 0x0a,
     969,  936,   17,   17, 0x0a,
    1034, 1029,   17,   17, 0x0a,
    1067, 1056,   17,   17, 0x0a,
    1099, 1094,   17,   17, 0x0a,
    1132,   17,   17,   17, 0x0a,
    1165, 1156,   17,   17, 0x0a,
    1202, 1197,   17,   17, 0x0a,
    1246, 1231,   17,   17, 0x0a,
    1284,  900,   17,   17, 0x0a,
    1322, 1313,   17,   17, 0x0a,
    1374, 1353,   17,   17, 0x0a,
    1437, 1425,   17,   17, 0x0a,
    1485, 1482,   17,   17, 0x0a,
    1503, 1482,   17,   17, 0x0a,
    1527, 1482,   17,   17, 0x0a,
    1552,   17,   17,   17, 0x0a,
    1579,   17,   17,   17, 0x0a,
    1602, 1595,   17,   17, 0x0a,
    1644, 1636,   17,   17, 0x0a,
    1687, 1681,   17,   17, 0x0a,
    1721, 1710,   17,   17, 0x0a,
    1751, 1746,   17,   17, 0x0a,
    1774, 1770,   17,   17, 0x0a,
    1801, 1792,   17,   17, 0x0a,
    1824,   17,   17,   17, 0x0a,
    1852, 1595,   17,   17, 0x0a,
    1890,  110,   17,   17, 0x0a,
    1922,   17,   17,   17, 0x0a,
    1943, 1941,   17,   17, 0x0a,
    1993, 1972,   17,   17, 0x0a,
    2026,   17,   17,   17, 0x0a,
    2044,   17,   17,   17, 0x0a,
    2064,   17,   17,   17, 0x0a,
    2082,   17,   17,   17, 0x0a,
    2105,   17, 2100,   17, 0x0a,
    2123,  601,   17,   17, 0x0a,
    2165,   17, 2145,   17, 0x0a,
    2206, 2189, 2100,   17, 0x0a,
    2250, 2189, 2100,   17, 0x0a,
    2294,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ExternalNotifier[] = {
    "ExternalNotifier\0\0canframe\0"
    "Can_data_send(SQCanFrame)\0state\0"
    "onStateChanged(int)\0seconds\0"
    "onPositionChanged(int)\0value\0"
    "onPositionCleared(bool)\0onDurationChanged(int)\0"
    "index,quantity\0onTrackChanged(int,int)\0"
    "index,quantity,bResetPosition\0"
    "onTrackClusterChanged(int,int,bool)\0"
    "deviceID,title,album,artist,genre,cover,composer,folder,filename\0"
    "onMediaInfoChanged(int,QString,QString,QString,QString,QString,QString"
    ",QString,QString)\0"
    "title\0onMediaInfoClusterChanged(QString)\0"
    "bFetched\0onDirTreeFetched(bool)\0"
    "title,album,artist,count\0"
    "onBTMusicInfoChanged(QString,QString,QString,QString)\0"
    "device\0onDeviceChanged(int)\0val\0"
    "onTuneSameFile(bool)\0source,type,count\0"
    "onContentSize(MEDIA_SOURCE_T,MEDIA_TYPE_T,int)\0"
    "nRandom,nRepeat,nScan,nNoOSD\0"
    "onPlayingModeChanged(int,int,int,bool)\0"
    "m_coverart,m_artist,m_song,m_album,m_folder,m_countInfo,m_samefile\0"
    "onTuneSearchChanged(QString,QString,QString,QString,QString,QString,bo"
    "ol)\0"
    "count\0onTuneCounterChanged(QString)\0"
    "artist,song,album,folder,isColor\0"
    "onTuneMetaDataChanged(QString,QString,QString,QString,bool)\0"
    "vOSD\0onSetDisplayOSD(bool)\0speedCheck\0"
    "onUpdateStateTrackOSD(int)\0mode\0"
    "onPlayingModeChanged(MEDIA_MODE)\0"
    "onMoreLikeThisCluster()\0filename\0"
    "onVideoFilenameChanged(QString)\0name\0"
    "onFoldernameChanged(QString)\0"
    "fullfoldername\0onVideoFullFolderNameChanged(QString)\0"
    "onPlaylistLenghtChanged(int)\0position\0"
    "onPlaylistPositionChanged(int)\0"
    "source,type,autoplay\0"
    "onDeviceAttached(MEDIA_SOURCE_T,MEDIA_TYPE_T,bool)\0"
    "source,type\0onDeviceRemoved(MEDIA_SOURCE_T,MEDIA_TYPE_T)\0"
    "on\0setTuneMode(bool)\0setFullScreenMode(bool)\0"
    "onSetPassPlayState(bool)\0"
    "onSetInitialDisplayValue()\0onDiscLoading()\0"
    "source\0onNotifyIPodReady(MEDIA_SOURCE_T)\0"
    "source,\0onAvModeChanged(MEDIA_SOURCE_T,bool)\0"
    "track\0onUpdateTrackInfo(int)\0brightness\0"
    "onBrightnessChanged(int)\0tint\0"
    "onTintChanged(int)\0hue\0onHueChanged(int)\0"
    "contrast\0onContrastChanged(int)\0"
    "onUpdateMainMenuAfterBoot()\0"
    "displayNoMediaFileOSD(MEDIA_SOURCE_T)\0"
    "onNotifyBtCallProgressing(bool)\0"
    "onClearTpMessage()\0,\0onSendTpMessage(QString,int)\0"
    "isBasicControlEnable\0"
    "onChangeBasicControlEnable(bool)\0"
    "onRetranslateUI()\0onPlayListChanged()\0"
    "StopOSDForClone()\0onSendTpMessage()\0"
    "bool\0GetCDDAMataInfo()\0SetCDDAMataInfo(bool)\0"
    "GSTAudioRouterDBus*\0getGSTAudioRouterDBus()\0"
    "offset,pBuf,size\0"
    "GetSharedMemoryInfo(int,unsigned char*,int)\0"
    "SetSharedMemoryInfo(int,unsigned char*,int)\0"
    "updateAFWScreenSetting()\0"
};

void ExternalNotifier::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ExternalNotifier *_t = static_cast<ExternalNotifier *>(_o);
        switch (_id) {
        case 0: _t->Can_data_send((*reinterpret_cast< const SQCanFrame(*)>(_a[1]))); break;
        case 1: _t->onStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onPositionCleared((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onDurationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onTrackChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->onTrackClusterChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 7: _t->onMediaInfoChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7])),(*reinterpret_cast< QString(*)>(_a[8])),(*reinterpret_cast< QString(*)>(_a[9]))); break;
        case 8: _t->onMediaInfoClusterChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->onDirTreeFetched((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->onBTMusicInfoChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 11: _t->onDeviceChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->onTuneSameFile((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->onContentSize((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1])),(*reinterpret_cast< MEDIA_TYPE_T(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 14: _t->onPlayingModeChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 15: _t->onTuneSearchChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< bool(*)>(_a[7]))); break;
        case 16: _t->onTuneCounterChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: _t->onTuneMetaDataChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 18: _t->onSetDisplayOSD((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: _t->onUpdateStateTrackOSD((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->onPlayingModeChanged((*reinterpret_cast< MEDIA_MODE(*)>(_a[1]))); break;
        case 21: _t->onMoreLikeThisCluster(); break;
        case 22: _t->onVideoFilenameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->onFoldernameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 24: _t->onVideoFullFolderNameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 25: _t->onPlaylistLenghtChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->onPlaylistPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->onDeviceAttached((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1])),(*reinterpret_cast< MEDIA_TYPE_T(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 28: _t->onDeviceRemoved((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1])),(*reinterpret_cast< MEDIA_TYPE_T(*)>(_a[2]))); break;
        case 29: _t->setTuneMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 30: _t->setFullScreenMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 31: _t->onSetPassPlayState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 32: _t->onSetInitialDisplayValue(); break;
        case 33: _t->onDiscLoading(); break;
        case 34: _t->onNotifyIPodReady((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1]))); break;
        case 35: _t->onAvModeChanged((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 36: _t->onUpdateTrackInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: _t->onBrightnessChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 38: _t->onTintChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 39: _t->onHueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 40: _t->onContrastChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 41: _t->onUpdateMainMenuAfterBoot(); break;
        case 42: _t->displayNoMediaFileOSD((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1]))); break;
        case 43: _t->onNotifyBtCallProgressing((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 44: _t->onClearTpMessage(); break;
        case 45: _t->onSendTpMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 46: _t->onChangeBasicControlEnable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 47: _t->onRetranslateUI(); break;
        case 48: _t->onPlayListChanged(); break;
        case 49: _t->StopOSDForClone(); break;
        case 50: _t->onSendTpMessage(); break;
        case 51: { bool _r = _t->GetCDDAMataInfo();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 52: _t->SetCDDAMataInfo((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 53: { GSTAudioRouterDBus* _r = _t->getGSTAudioRouterDBus();
            if (_a[0]) *reinterpret_cast< GSTAudioRouterDBus**>(_a[0]) = _r; }  break;
        case 54: { bool _r = _t->GetSharedMemoryInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< unsigned char*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 55: { bool _r = _t->SetSharedMemoryInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< unsigned char*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 56: _t->updateAFWScreenSetting(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ExternalNotifier::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ExternalNotifier::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ExternalNotifier,
      qt_meta_data_ExternalNotifier, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ExternalNotifier::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ExternalNotifier::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ExternalNotifier::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ExternalNotifier))
        return static_cast<void*>(const_cast< ExternalNotifier*>(this));
    return QObject::qt_metacast(_clname);
}

int ExternalNotifier::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 57)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 57;
    }
    return _id;
}

// SIGNAL 0
void ExternalNotifier::Can_data_send(const SQCanFrame & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
