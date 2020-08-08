/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_ViewLoader.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/util/DHAVN_AppMediaPlayer_ViewLoader.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_ViewLoader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_ViewLoader[] = {

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
      51,   27,   26,   26, 0x05,

 // slots: signature, parameters, type, tag, flags
     110,   26,   26,   26, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_ViewLoader[] = {
    "AppMediaPlayer_ViewLoader\0\0"
    "viewName,pView,pContext\0"
    "viewLoaded(QString,QDeclarativeItem*,QDeclarativeContext*)\0"
    "loadComplete()\0"
};

void AppMediaPlayer_ViewLoader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_ViewLoader *_t = static_cast<AppMediaPlayer_ViewLoader *>(_o);
        switch (_id) {
        case 0: _t->viewLoaded((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QDeclarativeItem*(*)>(_a[2])),(*reinterpret_cast< QDeclarativeContext*(*)>(_a[3]))); break;
        case 1: _t->loadComplete(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_ViewLoader::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_ViewLoader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_ViewLoader,
      qt_meta_data_AppMediaPlayer_ViewLoader, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_ViewLoader::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_ViewLoader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_ViewLoader::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_ViewLoader))
        return static_cast<void*>(const_cast< AppMediaPlayer_ViewLoader*>(this));
    return QObject::qt_metacast(_clname);
}

int AppMediaPlayer_ViewLoader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void AppMediaPlayer_ViewLoader::viewLoaded(const QString & _t1, QDeclarativeItem * _t2, QDeclarativeContext * _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
