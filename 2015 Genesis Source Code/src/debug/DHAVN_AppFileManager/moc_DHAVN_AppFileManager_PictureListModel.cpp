/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFileManager_PictureListModel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFileManager_PictureListModel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFileManager_PictureListModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PictureListModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       1,   39, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   18,   17,   17, 0x05,
      71,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     116,   92,   17,   17, 0x08,
     208,  190,   17,   17, 0x08,
     264,  229,   17,   17, 0x08,

 // properties: name, type, flags
     309,  304, 0x01495103,

 // properties: notify_signal_id
       1,

       0        // eod
};

static const char qt_meta_stringdata_PictureListModel[] = {
    "PictureListModel\0\0dataList\0"
    "getFirstImageAndCount(QVector<QStringList>)\0"
    "layoutReadyChanged()\0dataList,queryId,reqUID\0"
    "onRequestFinished(QVector<QStringList>,ETrackerAbstarctor_Query_Ids,ui"
    "nt)\0"
    "error,errorString\0onError(int,QString)\0"
    "thumbnail,unsupported,index,bFront\0"
    "onThumbnailReady(QString,bool,int,bool)\0"
    "bool\0layoutReady\0"
};

void PictureListModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PictureListModel *_t = static_cast<PictureListModel *>(_o);
        switch (_id) {
        case 0: _t->getFirstImageAndCount((*reinterpret_cast< QVector<QStringList>(*)>(_a[1]))); break;
        case 1: _t->layoutReadyChanged(); break;
        case 2: _t->onRequestFinished((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        case 3: _t->onError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->onThumbnailReady((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PictureListModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PictureListModel::staticMetaObject = {
    { &BaseListModel::staticMetaObject, qt_meta_stringdata_PictureListModel,
      qt_meta_data_PictureListModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PictureListModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PictureListModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PictureListModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PictureListModel))
        return static_cast<void*>(const_cast< PictureListModel*>(this));
    return BaseListModel::qt_metacast(_clname);
}

int PictureListModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
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
void PictureListModel::getFirstImageAndCount(QVector<QStringList> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PictureListModel::layoutReadyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
