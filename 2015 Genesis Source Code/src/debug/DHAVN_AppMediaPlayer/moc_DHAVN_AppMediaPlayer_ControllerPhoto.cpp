/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_ControllerPhoto.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/controller/DHAVN_AppMediaPlayer_ControllerPhoto.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_ControllerPhoto.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Controller_Photo[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      33,   32,   32,   32, 0x05,
      63,   50,   32,   32, 0x05,
     109,   97,   32,   32, 0x05,

 // slots: signature, parameters, type, tag, flags
     150,  140,   32,   32, 0x0a,
     191,  167,   32,   32, 0x0a,
     304,  269,   32,   32, 0x0a,
     421,  412,   32,   32, 0x0a,
     452,  412,   32,   32, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Controller_Photo[] = {
    "AppMediaPlayer_Controller_Photo\0\0"
    "showErrorPopup()\0,mediaSource\0"
    "filesInCurFolder(QStringList,int)\0"
    "mediaSource\0handleAllUnsupportedFiles(int)\0"
    "errorCode\0HandleError(int)\0"
    "fileList,queryId,reqUID\0"
    "HandleRequestComplete(QVector<QStringList>,ETrackerAbstarctor_Query_Id"
    "s,uint)\0"
    "fileList,queryId,volumeType,reqUID\0"
    "HandleRequestComplete(QVector<QStringList>,ETrackerAbstarctor_Query_Id"
    "s,ETrackerAbstarctor_VolumeType,uint)\0"
    "filepath\0updateUnsupportedInfo(QString)\0"
    "handleUnsupportedFile(QString)\0"
};

void AppMediaPlayer_Controller_Photo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Controller_Photo *_t = static_cast<AppMediaPlayer_Controller_Photo *>(_o);
        switch (_id) {
        case 0: _t->showErrorPopup(); break;
        case 1: _t->filesInCurFolder((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->handleAllUnsupportedFiles((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->HandleError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->HandleRequestComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        case 5: _t->HandleRequestComplete((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< ETrackerAbstarctor_VolumeType(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 6: _t->updateUnsupportedInfo((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->handleUnsupportedFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Controller_Photo::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Controller_Photo::staticMetaObject = {
    { &AppMediaPlayer_Controller_Base::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Controller_Photo,
      qt_meta_data_AppMediaPlayer_Controller_Photo, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Controller_Photo::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Controller_Photo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Controller_Photo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Controller_Photo))
        return static_cast<void*>(const_cast< AppMediaPlayer_Controller_Photo*>(this));
    return AppMediaPlayer_Controller_Base::qt_metacast(_clname);
}

int AppMediaPlayer_Controller_Photo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppMediaPlayer_Controller_Base::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void AppMediaPlayer_Controller_Photo::showErrorPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void AppMediaPlayer_Controller_Photo::filesInCurFolder(QStringList _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AppMediaPlayer_Controller_Photo::handleAllUnsupportedFiles(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
