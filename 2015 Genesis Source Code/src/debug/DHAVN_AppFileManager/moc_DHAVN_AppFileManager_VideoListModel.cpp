/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFileManager_VideoListModel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFileManager_VideoListModel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFileManager_VideoListModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VideoListModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       1,   59, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   16,   15,   15, 0x05,
      79,   68,   15,   15, 0x05,
     129,  114,   15,   15, 0x05,
     167,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     206,  188,   15,   15, 0x0a,
     238,  227,   15,   15, 0x0a,
     286,  271,   15,   15, 0x0a,

 // methods: signature, parameters, type, tag, flags
     336,   15,   15,   15, 0x02,
     354,  344,   15,   15, 0x02,

 // properties: name, type, flags
     385,  380, 0x01495103,

 // properties: notify_signal_id
       3,

       0        // eod
};

static const char qt_meta_stringdata_VideoListModel[] = {
    "VideoListModel\0\0,list\0"
    "append(VideoListModel*,QList<VideoModelItem>)\0"
    "path,reqId\0requestThumbnails(QStringList,int)\0"
    "path,uid,reqId\0requestFolderContent(QString,int,int)\0"
    "layoutReadyChanged()\0error,errorString\0"
    "onError(int,QString)\0index,path\0"
    "onThumbnailReceived(int,QString)\0"
    "dataList,reqId\0"
    "onFolderContentReceived(QVector<QStringList>,int)\0"
    "clear()\0start,end\0updateThumbnails(int,int)\0"
    "bool\0layoutReady\0"
};

void VideoListModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        VideoListModel *_t = static_cast<VideoListModel *>(_o);
        switch (_id) {
        case 0: _t->append((*reinterpret_cast< VideoListModel*(*)>(_a[1])),(*reinterpret_cast< const QList<VideoModelItem>(*)>(_a[2]))); break;
        case 1: _t->requestThumbnails((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->requestFolderContent((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->layoutReadyChanged(); break;
        case 4: _t->onError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->onThumbnailReceived((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->onFolderContentReceived((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->clear(); break;
        case 8: _t->updateThumbnails((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData VideoListModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject VideoListModel::staticMetaObject = {
    { &BaseListModel::staticMetaObject, qt_meta_stringdata_VideoListModel,
      qt_meta_data_VideoListModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VideoListModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VideoListModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VideoListModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VideoListModel))
        return static_cast<void*>(const_cast< VideoListModel*>(this));
    return BaseListModel::qt_metacast(_clname);
}

int VideoListModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = getLayoutReady(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setLayoutReady(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void VideoListModel::append(VideoListModel * _t1, const QList<VideoModelItem> & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VideoListModel::requestThumbnails(QStringList _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VideoListModel::requestFolderContent(QString _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void VideoListModel::layoutReadyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
static const uint qt_meta_data_QTrackerProcessor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   19,   18,   18, 0x05,
      65,   52,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     137,  113,   18,   18, 0x0a,
     222,  211,   18,   18, 0x0a,
     274,  259,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QTrackerProcessor[] = {
    "QTrackerProcessor\0\0,\0"
    "thumbnailReceived(int,QString)\0"
    "dataList,req\0"
    "folderContentReceived(QVector<QStringList>,int)\0"
    "dataList,queryId,reqUID\0"
    "onRequestFinished(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,ui"
    "nt)\0"
    "path,reqId\0onRequestThumbnails(QStringList,int)\0"
    "path,uid,reqId\0onRequestFolderContent(QString,int,int)\0"
};

void QTrackerProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QTrackerProcessor *_t = static_cast<QTrackerProcessor *>(_o);
        switch (_id) {
        case 0: _t->thumbnailReceived((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->folderContentReceived((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->onRequestFinished((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        case 3: _t->onRequestThumbnails((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->onRequestFolderContent((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QTrackerProcessor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QTrackerProcessor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QTrackerProcessor,
      qt_meta_data_QTrackerProcessor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QTrackerProcessor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QTrackerProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QTrackerProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTrackerProcessor))
        return static_cast<void*>(const_cast< QTrackerProcessor*>(this));
    return QObject::qt_metacast(_clname);
}

int QTrackerProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void QTrackerProcessor::thumbnailReceived(int _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QTrackerProcessor::folderContentReceived(QVector<QStringList> _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
