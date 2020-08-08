/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_Controller.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_Controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_Controller[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      39,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      15,       // signalCount

 // signals: signature, parameters, type, tag, flags
      33,   32,   32,   32, 0x05,
      63,   58,   32,   32, 0x05,
      90,   32,   32,   32, 0x05,
     127,  121,   32,   32, 0x05,
     154,  121,   32,   32, 0x05,
     183,   32,   32,   32, 0x05,
     211,  205,   32,   32, 0x05,
     242,  236,   32,   32, 0x05,
     263,  236,   32,   32, 0x05,
     279,  236,   32,   32, 0x05,
     302,  236,   32,   32, 0x05,
     325,   32,   32,   32, 0x05,
     339,   32,   32,   32, 0x05,
     362,   32,   32,   32, 0x05,
     381,   32,   32,   32, 0x05,

 // slots: signature, parameters, type, tag, flags
     408,  398,   32,   32, 0x0a,
     452,   32,   32,   32, 0x0a,
     481,  472,   32,   32, 0x0a,
     519,  508,   32,   32, 0x0a,
     564,  548,   32,   32, 0x0a,
     605,  597,   32,   32, 0x0a,
     633,  627,   32,   32, 0x0a,
     653,  121,   32,   32, 0x0a,
     682,  121,   32,   32, 0x0a,
     723,  717,  713,   32, 0x0a,
     752,   32,   32,   32, 0x08,
     765,   32,   32,   32, 0x08,
     797,  788,   32,   32, 0x08,
     826,  820,   32,   32, 0x08,
     846,   32,   32,   32, 0x08,
     866,   32,   32,   32, 0x08,
     885,   32,   32,   32, 0x08,
     909,  902,   32,   32, 0x08,
     938,  936,   32,   32, 0x08,
     976,   32,   32,   32, 0x08,
    1008,   32,   32,   32, 0x08,
    1053, 1029,   32,   32, 0x08,
    1162, 1127,   32,   32, 0x08,
    1266,   32,   32,   32, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_Controller[] = {
    "AppMediaPlayer_Video_Controller\0\0"
    "filenameChanged(QString)\0name\0"
    "foldernameChanged(QString)\0"
    "fullfoldernameChanged(QString)\0count\0"
    "playlistLengthChanged(int)\0"
    "playlistPositionChanged(int)\0"
    "dvdScreenChanged(int)\0speed\0"
    "UpdateStateTrackOSD(int)\0value\0"
    "contrastChanged(int)\0hueChanged(int)\0"
    "saturationChanged(int)\0brightnessChanged(int)\0"
    "playAuxOnFG()\0onAuxStateChanged(int)\0"
    "tuneTimerExpired()\0sourceReleased()\0"
    "resources\0onReleaseResources(MEDIAPLAYER_RESOURCES_T)\0"
    "onResourceGranted()\0filename\0"
    "onFilenameChanged(QString)\0foldername\0"
    "onFoldernameChanged(QString)\0"
    "fullfolderename\0onFullFolderNameChanged(QString)\0"
    "chapter\0onChapterChanged(int)\0title\0"
    "onTitleChanged(int)\0onPlaylistLengthChanged(int)\0"
    "onPlaylistPositionChanged(int)\0int\0"
    "media\0getOSDDevice(MEDIA_SOURCE_T)\0"
    "onError(int)\0onPositionChanged(int)\0"
    "duration\0onDurationChanged(int)\0index\0"
    "onTrackChanged(int)\0onStateChanged(int)\0"
    "onModeChanged(int)\0connectCModels()\0"
    "nState\0HandleAuxStateChanged(int)\0,\0"
    "onDeviceAttached(MEDIA_SOURCE_T,bool)\0"
    "onDeviceRemoved(MEDIA_SOURCE_T)\0"
    "onMuteTimerExpired()\0fileList,queryId,reqUID\0"
    "HandleReqComplete(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,ui"
    "nt)\0"
    "fileList,queryId,volumeType,reqUID\0"
    "HandleReqComplete(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,ET"
    "rackerAbstarctor_VolumeType,uint)\0"
    "onOverplaneChanged()\0"
};

void AppMediaPlayer_Video_Controller::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_Controller *_t = static_cast<AppMediaPlayer_Video_Controller *>(_o);
        switch (_id) {
        case 0: _t->filenameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->foldernameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->fullfoldernameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->playlistLengthChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->playlistPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->dvdScreenChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->UpdateStateTrackOSD((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->contrastChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->hueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->saturationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->brightnessChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->playAuxOnFG(); break;
        case 12: _t->onAuxStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->tuneTimerExpired(); break;
        case 14: _t->sourceReleased(); break;
        case 15: _t->onReleaseResources((*reinterpret_cast< MEDIAPLAYER_RESOURCES_T(*)>(_a[1]))); break;
        case 16: _t->onResourceGranted(); break;
        case 17: _t->onFilenameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: _t->onFoldernameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 19: _t->onFullFolderNameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: _t->onChapterChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->onTitleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->onPlaylistLengthChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->onPlaylistPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: { int _r = _t->getOSDDevice((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 25: _t->onError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->onPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->onDurationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: _t->onTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->onStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->onModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->connectCModels(); break;
        case 32: _t->HandleAuxStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->onDeviceAttached((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 34: _t->onDeviceRemoved((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1]))); break;
        case 35: _t->onMuteTimerExpired(); break;
        case 36: _t->HandleReqComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        case 37: _t->HandleReqComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 38: _t->onOverplaneChanged(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_Controller::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_Controller::staticMetaObject = {
    { &AppMediaPlayer_Controller_Base::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_Controller,
      qt_meta_data_AppMediaPlayer_Video_Controller, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_Controller::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_Controller::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_Controller::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_Controller))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_Controller*>(this));
    return AppMediaPlayer_Controller_Base::qt_metacast(_clname);
}

int AppMediaPlayer_Video_Controller::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppMediaPlayer_Controller_Base::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 39)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 39;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_Controller::filenameChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AppMediaPlayer_Video_Controller::foldernameChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Video_Controller::fullfoldernameChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppMediaPlayer_Video_Controller::playlistLengthChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AppMediaPlayer_Video_Controller::playlistPositionChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AppMediaPlayer_Video_Controller::dvdScreenChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void AppMediaPlayer_Video_Controller::UpdateStateTrackOSD(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void AppMediaPlayer_Video_Controller::contrastChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void AppMediaPlayer_Video_Controller::hueChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void AppMediaPlayer_Video_Controller::saturationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void AppMediaPlayer_Video_Controller::brightnessChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void AppMediaPlayer_Video_Controller::playAuxOnFG()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void AppMediaPlayer_Video_Controller::onAuxStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void AppMediaPlayer_Video_Controller::tuneTimerExpired()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void AppMediaPlayer_Video_Controller::sourceReleased()
{
    QMetaObject::activate(this, &staticMetaObject, 14, 0);
}
QT_END_MOC_NAMESPACE
