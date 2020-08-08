/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFileManager_CTrackerExtractorClient.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFileManager_CTrackerExtractorClient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFileManager_CTrackerExtractorClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CTrackerExtractorClient[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x05,
      43,   24,   24,   24, 0x05,
      66,   61,   24,   24, 0x05,
     108,   24,   24,   24, 0x05,
     130,   24,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
     173,  151,   24,   24, 0x08,
     244,  204,   24,   24, 0x08,
     323,  290,   24,   24, 0x08,
     367,   24,   24,   24, 0x08,
     379,   24,   24,   24, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CTrackerExtractorClient[] = {
    "CTrackerExtractorClient\0\0showNotifyPopup()\0"
    "hideNotifyPopup()\0list\0"
    "updateUIData(const QVector<QStringList>*)\0"
    "MinerStatusFinished()\0ThumbNailGenerated()\0"
    "minerType,minerStatus\0"
    "onMinerStatus(QString,QString)\0"
    "ChangeStatus,ChangedType,UpdtaedUrlList\0"
    "onChangeNotify(int,bool,QVector<QStringList>)\0"
    "minerType,minerStatus,percentage\0"
    "onMiningStatusInDetail(QString,QString,int)\0"
    "onTimeout()\0onThumbnailsGenerated()\0"
};

void CTrackerExtractorClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CTrackerExtractorClient *_t = static_cast<CTrackerExtractorClient *>(_o);
        switch (_id) {
        case 0: _t->showNotifyPopup(); break;
        case 1: _t->hideNotifyPopup(); break;
        case 2: _t->updateUIData((*reinterpret_cast< const QVector<QStringList>*(*)>(_a[1]))); break;
        case 3: _t->MinerStatusFinished(); break;
        case 4: _t->ThumbNailGenerated(); break;
        case 5: _t->onMinerStatus((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->onChangeNotify((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< QVector<QStringList>(*)>(_a[3]))); break;
        case 7: _t->onMiningStatusInDetail((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 8: _t->onTimeout(); break;
        case 9: _t->onThumbnailsGenerated(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CTrackerExtractorClient::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CTrackerExtractorClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CTrackerExtractorClient,
      qt_meta_data_CTrackerExtractorClient, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CTrackerExtractorClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CTrackerExtractorClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CTrackerExtractorClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTrackerExtractorClient))
        return static_cast<void*>(const_cast< CTrackerExtractorClient*>(this));
    return QObject::qt_metacast(_clname);
}

int CTrackerExtractorClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void CTrackerExtractorClient::showNotifyPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CTrackerExtractorClient::hideNotifyPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CTrackerExtractorClient::updateUIData(const QVector<QStringList> * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CTrackerExtractorClient::MinerStatusFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CTrackerExtractorClient::ThumbNailGenerated()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
