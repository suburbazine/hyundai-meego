/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFileManager_AudioModel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFileManager_AudioModel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFileManager_AudioModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AudioModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      32,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      92,   75,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AudioModel[] = {
    "AudioModel\0\0audioList,errorCode\0"
    "audioListLoaded(QList<AudioModelItem>,int)\0"
    "dataList,queryId\0"
    "onRequestFinished(QVector<QStringList>,ETrackerAbstarctor_Query_Ids)\0"
};

void AudioModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AudioModel *_t = static_cast<AudioModel *>(_o);
        switch (_id) {
        case 0: _t->audioListLoaded((*reinterpret_cast< const QList<AudioModelItem>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->onRequestFinished((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AudioModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AudioModel::staticMetaObject = {
    { &BaseMediaModel::staticMetaObject, qt_meta_stringdata_AudioModel,
      qt_meta_data_AudioModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AudioModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AudioModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AudioModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AudioModel))
        return static_cast<void*>(const_cast< AudioModel*>(this));
    return BaseMediaModel::qt_metacast(_clname);
}

int AudioModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseMediaModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void AudioModel::audioListLoaded(const QList<AudioModelItem> _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
