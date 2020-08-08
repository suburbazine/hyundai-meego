/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_ControllerBase.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/controller/DHAVN_AppMediaPlayer_ControllerBase.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_ControllerBase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Controller_Base[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      24,       // signalCount

 // signals: signature, parameters, type, tag, flags
      41,   32,   31,   31, 0x05,
      71,   62,   31,   31, 0x05,
      98,   92,   31,   31, 0x05,
     120,   31,   31,   31, 0x05,
     137,   31,   31,   31, 0x05,
     170,  155,   31,   31, 0x05,
     222,  192,   31,   31, 0x05,
     256,  155,   31,   31, 0x25,
     348,  285,   31,   31, 0x05,
     466,  434,   31,   31, 0x05,
     585,  529,   31,   31, 0x05,
     689,  671,   31,   31, 0x05,
     746,  740,   31,   31, 0x05,
     804,  779,   31,   31, 0x05,
     865,  856,   31,   31, 0x05,
     890,  884,   31,   31, 0x05,
     919,  908,   31,   31, 0x05,
     953,  935,   31,   31, 0x05,
    1027,  998,   31,   31, 0x05,
    1069, 1064,   31,   31, 0x05,
    1167, 1100,   31,   31, 0x05,
    1232,   31,   31,   31, 0x05,
    1254,   31,   31,   31, 0x05,
    1279, 1277,   31,   31, 0x05,

 // slots: signature, parameters, type, tag, flags
    1306,   31,   31,   31, 0x0a,
    1329,   31,   31,   31, 0x0a,
    1349,   31,   31,   31, 0x0a,
    1369,   31,   31,   31, 0x0a,
    1400,   31,   31,   31, 0x0a,
    1423,   31,   31,   31, 0x0a,
    1442,   31,   31,   31, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Controller_Base[] = {
    "AppMediaPlayer_Controller_Base\0\0"
    "position\0positionChanged(int)\0duration\0"
    "durationChanged(int)\0value\0"
    "positionCleared(bool)\0clearTpMessage()\0"
    "playListChanged()\0index,quantity\0"
    "trackChanged(int,int)\0"
    "index,quantity,bResetPosition\0"
    "trackClusterChanged(int,int,bool)\0"
    "trackClusterChanged(int,int)\0"
    "device,title,album,artist,genre,cover,composer,folder,filename\0"
    "mediaInfoChanged(int,QString,QString,QString,QString,QString,QString,Q"
    "String,QString)\0"
    "device,title,album,artist,cover\0"
    "coverFlowMediaInfoChanged(int,QString,QString,QString,QString)\0"
    "title,album,artist,genre,cover,composer,folder,filename\0"
    "mediaInfoTempChanged(QString,QString,QString,QString,QString,QString,Q"
    "String,QString)\0"
    "album,artist,file\0"
    "currPlayingItemPosChanged(QString,QString,QString)\0"
    "title\0mediaInfoClusterChanged(QString)\0"
    "title,album,artist,count\0"
    "btMusicInfoChanged(QString,QString,QString,QString)\0"
    "deviceId\0deviceChanged(int)\0state\0"
    "stateChanged(int)\0nEventType\0"
    "updateOSD(uint)\0source,type,count\0"
    "contentSize(MEDIA_SOURCE_T,MEDIA_TYPE_T,int)\0"
    "nRandom,nRepeat,nScan,nNoOSD\0"
    "playingModeChanged(int,int,int,bool)\0"
    "mode\0playingModeChanged(MEDIA_MODE)\0"
    "m_coverart,m_artist,m_song,m_album,m_folder,m_countInfo,m_samefile\0"
    "tuneSearch(QString,QString,QString,QString,QString,QString,bool)\0"
    "moreLikeThisCluster()\0updateMP3CDFileCount()\0"
    ",\0sendTpMessage(QString,int)\0"
    "onPositionChanged(int)\0onStateChanged(int)\0"
    "onTrackChanged(int)\0onMediaInfoChanged(MediaInfo*)\0"
    "onDurationChanged(int)\0onModeChanged(int)\0"
    "onError(int)\0"
};

void AppMediaPlayer_Controller_Base::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Controller_Base *_t = static_cast<AppMediaPlayer_Controller_Base *>(_o);
        switch (_id) {
        case 0: _t->positionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->durationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->positionCleared((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->clearTpMessage(); break;
        case 4: _t->playListChanged(); break;
        case 5: _t->trackChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->trackClusterChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 7: _t->trackClusterChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->mediaInfoChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7])),(*reinterpret_cast< QString(*)>(_a[8])),(*reinterpret_cast< QString(*)>(_a[9]))); break;
        case 9: _t->coverFlowMediaInfoChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 10: _t->mediaInfoTempChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7])),(*reinterpret_cast< QString(*)>(_a[8]))); break;
        case 11: _t->currPlayingItemPosChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 12: _t->mediaInfoClusterChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->btMusicInfoChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 14: _t->deviceChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->updateOSD((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 17: _t->contentSize((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1])),(*reinterpret_cast< MEDIA_TYPE_T(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 18: _t->playingModeChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 19: _t->playingModeChanged((*reinterpret_cast< MEDIA_MODE(*)>(_a[1]))); break;
        case 20: _t->tuneSearch((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< bool(*)>(_a[7]))); break;
        case 21: _t->moreLikeThisCluster(); break;
        case 22: _t->updateMP3CDFileCount(); break;
        case 23: _t->sendTpMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 24: _t->onPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->onStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->onTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->onMediaInfoChanged((*reinterpret_cast< MediaInfo*(*)>(_a[1]))); break;
        case 28: _t->onDurationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->onModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->onError((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Controller_Base::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Controller_Base::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Controller_Base,
      qt_meta_data_AppMediaPlayer_Controller_Base, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Controller_Base::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Controller_Base::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Controller_Base::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Controller_Base))
        return static_cast<void*>(const_cast< AppMediaPlayer_Controller_Base*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_Controller_Base::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Controller_Base::positionChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Controller_Base::durationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Controller_Base::positionCleared(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppMediaPlayer_Controller_Base::clearTpMessage()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void AppMediaPlayer_Controller_Base::playListChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void AppMediaPlayer_Controller_Base::trackChanged(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void AppMediaPlayer_Controller_Base::trackClusterChanged(int _t1, int _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 8
void AppMediaPlayer_Controller_Base::mediaInfoChanged(int _t1, QString _t2, QString _t3, QString _t4, QString _t5, QString _t6, QString _t7, QString _t8, QString _t9)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)), const_cast<void*>(reinterpret_cast<const void*>(&_t9)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void AppMediaPlayer_Controller_Base::coverFlowMediaInfoChanged(int _t1, QString _t2, QString _t3, QString _t4, QString _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void AppMediaPlayer_Controller_Base::mediaInfoTempChanged(QString _t1, QString _t2, QString _t3, QString _t4, QString _t5, QString _t6, QString _t7, QString _t8)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void AppMediaPlayer_Controller_Base::currPlayingItemPosChanged(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void AppMediaPlayer_Controller_Base::mediaInfoClusterChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void AppMediaPlayer_Controller_Base::btMusicInfoChanged(QString _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void AppMediaPlayer_Controller_Base::deviceChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void AppMediaPlayer_Controller_Base::stateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void AppMediaPlayer_Controller_Base::updateOSD(unsigned int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void AppMediaPlayer_Controller_Base::contentSize(MEDIA_SOURCE_T _t1, MEDIA_TYPE_T _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void AppMediaPlayer_Controller_Base::playingModeChanged(int _t1, int _t2, int _t3, bool _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void AppMediaPlayer_Controller_Base::playingModeChanged(MEDIA_MODE _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void AppMediaPlayer_Controller_Base::tuneSearch(QString _t1, QString _t2, QString _t3, QString _t4, QString _t5, QString _t6, bool _t7)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void AppMediaPlayer_Controller_Base::moreLikeThisCluster()
{
    QMetaObject::activate(this, &staticMetaObject, 21, 0);
}

// SIGNAL 22
void AppMediaPlayer_Controller_Base::updateMP3CDFileCount()
{
    QMetaObject::activate(this, &staticMetaObject, 22, 0);
}

// SIGNAL 23
void AppMediaPlayer_Controller_Base::sendTpMessage(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}
QT_END_MOC_NAMESPACE
