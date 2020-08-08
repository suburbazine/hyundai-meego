/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_CoverArtDataProvider.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_CoverArtDataProvider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_CoverArtDataProvider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCoverArtDataprovider[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      40,   23,   22,   22, 0x05,
     103,   22,   22,   22, 0x05,
     132,  114,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
     169,   22,   22,   22, 0x0a,
     179,   22,   22,   22, 0x0a,
     195,  193,   22,   22, 0x0a,
     219,  216,   22,   22, 0x0a,
     244,   22,   22,   22, 0x0a,
     269,  259,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CCoverArtDataprovider[] = {
    "CCoverArtDataprovider\0\0dataList,queryId\0"
    "DataFetched(QVector<QStringList>,ETrackerAbstarctor_Query_Ids)\0"
    "Error(int)\0albumList,urlList\0"
    "getCoverArt(QStringList,QStringList)\0"
    "Started()\0Finished(int)\0,\0"
    "Thumbnail(QUrl,QUrl)\0,,\0"
    "Error(QString,QUrl,QUrl)\0Dequeued(QUrl)\0"
    "coverArts\0onCoverArtsRetrived(QStringList)\0"
};

void CCoverArtDataprovider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCoverArtDataprovider *_t = static_cast<CCoverArtDataprovider *>(_o);
        switch (_id) {
        case 0: _t->DataFetched((*reinterpret_cast< QVector<QStringList>(*)>(_a[1])),(*reinterpret_cast< ETrackerAbstarctor_Query_Ids(*)>(_a[2]))); break;
        case 1: _t->Error((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->getCoverArt((*reinterpret_cast< QStringList(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 3: _t->Started(); break;
        case 4: _t->Finished((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->Thumbnail((*reinterpret_cast< QUrl(*)>(_a[1])),(*reinterpret_cast< QUrl(*)>(_a[2]))); break;
        case 6: _t->Error((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QUrl(*)>(_a[2])),(*reinterpret_cast< QUrl(*)>(_a[3]))); break;
        case 7: _t->Dequeued((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 8: _t->onCoverArtsRetrived((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCoverArtDataprovider::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCoverArtDataprovider::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CCoverArtDataprovider,
      qt_meta_data_CCoverArtDataprovider, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCoverArtDataprovider::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCoverArtDataprovider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCoverArtDataprovider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCoverArtDataprovider))
        return static_cast<void*>(const_cast< CCoverArtDataprovider*>(this));
    return QObject::qt_metacast(_clname);
}

int CCoverArtDataprovider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CCoverArtDataprovider::DataFetched(QVector<QStringList> _t1, ETrackerAbstarctor_Query_Ids _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CCoverArtDataprovider::Error(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CCoverArtDataprovider::getCoverArt(QStringList _t1, QStringList _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
