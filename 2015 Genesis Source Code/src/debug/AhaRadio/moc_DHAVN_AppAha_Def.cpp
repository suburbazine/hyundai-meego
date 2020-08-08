/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppAha_Def.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppAha_Def.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppAha_Def.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TrackInfo[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
      20,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      17,   10, 0x82095001,
      26,   10, 0x82095001,
      41,   34, 0x86095001,
      59,   54, 0x01095001,
      71,   54, 0x01095001,
      81,   54, 0x01095001,
      94,   54, 0x01095001,
     104,   54, 0x01095001,
     118,   54, 0x01095001,
     133,   54, 0x01095001,
     143,   54, 0x01095001,
     157,   54, 0x01095001,
     171,   54, 0x01095001,
     192,  184, 0x0a095001,
     198,  184, 0x0a095001,
     205,  184, 0x0a095001,
     211,  184, 0x0a095001,
     223,  184, 0x0a095001,
     231,  184, 0x0a095103,
     250,  242, 0x03095001,

       0        // eod
};

static const char qt_meta_stringdata_TrackInfo[] = {
    "TrackInfo\0qint16\0duration\0elapsed\0"
    "quint8\0ratingStatus\0bool\0allowRating\0"
    "allowLike\0allowDislike\0allowSkip\0"
    "allowSkipBack\0allowTimeShift\0allowCall\0"
    "allowNavigate\0allowBookmark\0allowExplain\0"
    "QString\0title\0artist\0album\0albumArtUrl\0"
    "station\0stationArt\0quint32\0stationToken\0"
};

void TrackInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData TrackInfo::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TrackInfo::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TrackInfo,
      qt_meta_data_TrackInfo, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TrackInfo::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TrackInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TrackInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TrackInfo))
        return static_cast<void*>(const_cast< TrackInfo*>(this));
    return QObject::qt_metacast(_clname);
}

int TrackInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< qint16*>(_v) = getDuration(); break;
        case 1: *reinterpret_cast< qint16*>(_v) = getElapsed(); break;
        case 2: *reinterpret_cast< quint8*>(_v) = getRating(); break;
        case 3: *reinterpret_cast< bool*>(_v) = getAllowRating(); break;
        case 4: *reinterpret_cast< bool*>(_v) = getAllowLike(); break;
        case 5: *reinterpret_cast< bool*>(_v) = getAllowDislike(); break;
        case 6: *reinterpret_cast< bool*>(_v) = getAllowSkip(); break;
        case 7: *reinterpret_cast< bool*>(_v) = getAllowSkipBack(); break;
        case 8: *reinterpret_cast< bool*>(_v) = getAllowTimeShift(); break;
        case 9: *reinterpret_cast< bool*>(_v) = getAllowCall(); break;
        case 10: *reinterpret_cast< bool*>(_v) = getAllowNavigate(); break;
        case 11: *reinterpret_cast< bool*>(_v) = getAllowBookmark(); break;
        case 12: *reinterpret_cast< bool*>(_v) = getAllowExplain(); break;
        case 13: *reinterpret_cast< QString*>(_v) = getTitle(); break;
        case 14: *reinterpret_cast< QString*>(_v) = getArtist(); break;
        case 15: *reinterpret_cast< QString*>(_v) = getAlbum(); break;
        case 16: *reinterpret_cast< QString*>(_v) = getAlbumArtUrl(); break;
        case 17: *reinterpret_cast< QString*>(_v) = getStationName(); break;
        case 18: *reinterpret_cast< QString*>(_v) = getStationArt(); break;
        case 19: *reinterpret_cast< quint32*>(_v) = getStationToken(); break;
        }
        _id -= 20;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 18: setStationArt(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 20;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 20;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 20;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 20;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 20;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 20;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 20;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
