/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_ControllerFS.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/controller/video/DHAVN_AppMediaPlayer_Video_ControllerFS.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_ControllerFS.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_ControllerFS[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      30,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      35,   34,   34,   34, 0x05,
      64,   59,   34,   34, 0x05,
      94,   88,   34,   34, 0x05,
     170,  161,   34,   34, 0x05,
     191,   34,   34,   34, 0x05,
     205,   59,   34,   34, 0x05,

 // slots: signature, parameters, type, tag, flags
     237,   34,   34,   34, 0x0a,
     244,  161,   34,   34, 0x08,
     275,  267,   34,   34, 0x08,
     322,  295,   34,   34, 0x08,
     346,   88,   34,   34, 0x08,
     370,  359,   34,   34, 0x08,
     438,  397,   34,   34, 0x08,
     485,  397,   34,   34, 0x08,
     573,  531,   34,   34, 0x08,
     645,  622,   34,   34, 0x08,
     680,  677,   34,   34, 0x08,
     724,  703,   34,   34, 0x08,
     758,  752,   34,   34, 0x08,
     783,  677,   34,   34, 0x08,
     842,   34,   34,   34, 0x08,
     879,  867,   34,   34, 0x08,
     916,   34,   34,   34, 0x08,
     938,   34,   34,   34, 0x08,
     959,   34,   34,   34, 0x08,
    1015,  974,   34,   34, 0x08,
    1049,   34,   34,   34, 0x08,
    1074, 1068,   34,   34, 0x08,
    1098,   34,   34,   34, 0x08,
    1122,   34,   34,   34, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_ControllerFS[] = {
    "AppMediaPlayer_Video_ControllerFS\0\0"
    "operationCopyComplete()\0type\0"
    "operationCopyError(int)\0error\0"
    "fsPlaybackError(AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_T)\0"
    "duration\0durationChanged(int)\0"
    "cancelFfRew()\0showPopup(POPUP_TXT_BTN_TYPE_T)\0"
    "skip()\0onDurationChanged(int)\0trackNo\0"
    "onTrackChanged(int)\0index,total,copyPercentage\0"
    "onProgress(int,int,int)\0onError(int)\0"
    "strCaption\0onSubtitleChanged(QString)\0"
    "langCodeList,pStreamIdList,nCurrStreamId\0"
    "onAudioLanguagesReceived(QStringList,int*,int)\0"
    "onTotalTextLangReceived(QStringList,int*,int)\0"
    "type,status,width,height,par,dar,fileName\0"
    "onVideoInfo(int,int,int,int,float,float,QString)\0"
    "file,totalFilesForCopy\0"
    "replaceFileRequest(QString,int)\0,,\0"
    "onDRMInfo(int,int,int)\0filesCount,filesSize\0"
    "onCapacityError(int,qint64)\0value\0"
    "onFgBgStateChanged(bool)\0"
    "onSourceAvailableChanged(MEDIA_SOURCE_T,MEDIA_TYPE_T,bool)\0"
    "setSourceAvailable(bool)\0buf,bufSize\0"
    "onHandleXSubtitleChanged(uchar*,int)\0"
    "onFileSystemChanged()\0onRealSoundChanged()\0"
    "onNormalPlay()\0bPrev,nTotalIdx,nIdxInFolder,nFolderSize\0"
    "OnTrackBufferred(int,int,int,int)\0"
    "onAudioSinkError()\0index\0"
    "onAutoTrackChanged(int)\0onLongBtnTimerExpired()\0"
    "stopFFRewTimer()\0"
};

void AppMediaPlayer_Video_ControllerFS::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_ControllerFS *_t = static_cast<AppMediaPlayer_Video_ControllerFS *>(_o);
        switch (_id) {
        case 0: _t->operationCopyComplete(); break;
        case 1: _t->operationCopyError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->fsPlaybackError((*reinterpret_cast< AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_T(*)>(_a[1]))); break;
        case 3: _t->durationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->cancelFfRew(); break;
        case 5: _t->showPopup((*reinterpret_cast< POPUP_TXT_BTN_TYPE_T(*)>(_a[1]))); break;
        case 6: _t->skip(); break;
        case 7: _t->onDurationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->onTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->onProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 10: _t->onError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->onSubtitleChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->onAudioLanguagesReceived((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 13: _t->onTotalTextLangReceived((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 14: _t->onVideoInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5])),(*reinterpret_cast< float(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7]))); break;
        case 15: _t->replaceFileRequest((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 16: _t->onDRMInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 17: _t->onCapacityError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 18: _t->onFgBgStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: _t->onSourceAvailableChanged((*reinterpret_cast< MEDIA_SOURCE_T(*)>(_a[1])),(*reinterpret_cast< MEDIA_TYPE_T(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 20: _t->setSourceAvailable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 21: _t->onHandleXSubtitleChanged((*reinterpret_cast< uchar*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 22: _t->onFileSystemChanged(); break;
        case 23: _t->onRealSoundChanged(); break;
        case 24: _t->onNormalPlay(); break;
        case 25: _t->OnTrackBufferred((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 26: _t->onAudioSinkError(); break;
        case 27: _t->onAutoTrackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: _t->onLongBtnTimerExpired(); break;
        case 29: _t->stopFFRewTimer(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_ControllerFS::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_ControllerFS::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_ControllerFS,
      qt_meta_data_AppMediaPlayer_Video_ControllerFS, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_ControllerFS::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_ControllerFS::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_ControllerFS::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_ControllerFS))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_ControllerFS*>(this));
    if (!strcmp(_clname, "AppMediaPlayer_Video_ControllerBase"))
        return static_cast< AppMediaPlayer_Video_ControllerBase*>(const_cast< AppMediaPlayer_Video_ControllerFS*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_Video_ControllerFS::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 30)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 30;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Video_ControllerFS::operationCopyComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void AppMediaPlayer_Video_ControllerFS::operationCopyError(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Video_ControllerFS::fsPlaybackError(AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_T _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AppMediaPlayer_Video_ControllerFS::durationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AppMediaPlayer_Video_ControllerFS::cancelFfRew()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void AppMediaPlayer_Video_ControllerFS::showPopup(POPUP_TXT_BTN_TYPE_T _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
