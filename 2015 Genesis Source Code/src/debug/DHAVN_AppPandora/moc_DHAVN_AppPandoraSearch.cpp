/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppPandoraSearch.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppPandoraSearch.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppPandoraSearch.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CAppPandoraSearch[] = {

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
      19,   18,   18,   18, 0x05,
      59,   40,   18,   18, 0x05,
      92,   40,   18,   18, 0x05,
     137,  129,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     178,  161,   18,   18, 0x0a,
     242,  216,   18,   18, 0x0a,

 // methods: signature, parameters, type, tag, flags
     298,  286,   18,   18, 0x02,
     326,  286,   18,   18, 0x02,
     358,  350,   18,   18, 0x02,

 // properties: name, type, flags
     388,  380, 0x0a495003,

 // properties: notify_signal_id
       0,

       0        // eod
};

static const char qt_meta_stringdata_CAppPandoraSearch[] = {
    "CAppPandoraSearch\0\0updateSearchString()\0"
    "inSearchResultList\0autoSearchCompleted(QStringList)\0"
    "extendedSearchCompleted(QStringList)\0"
    "inError\0handleError(EPndrError)\0"
    "inSearchInfoList\0OnSearchComplete(QList<TSSearchInfo>)\0"
    "inRequestType,inPndrError\0"
    "OnRequestCompleted(ERequestType,EPndrError)\0"
    "inSearhWord\0SearchAutoComplete(QString)\0"
    "SearchExtended(QString)\0inIndex\0"
    "SearchSelect(quint32)\0QString\0"
    "sSearchString\0"
};

void CAppPandoraSearch::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CAppPandoraSearch *_t = static_cast<CAppPandoraSearch *>(_o);
        switch (_id) {
        case 0: _t->updateSearchString(); break;
        case 1: _t->autoSearchCompleted((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 2: _t->extendedSearchCompleted((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 3: _t->handleError((*reinterpret_cast< const EPndrError(*)>(_a[1]))); break;
        case 4: _t->OnSearchComplete((*reinterpret_cast< const QList<TSSearchInfo>(*)>(_a[1]))); break;
        case 5: _t->OnRequestCompleted((*reinterpret_cast< const ERequestType(*)>(_a[1])),(*reinterpret_cast< const EPndrError(*)>(_a[2]))); break;
        case 6: _t->SearchAutoComplete((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->SearchExtended((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->SearchSelect((*reinterpret_cast< const quint32(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CAppPandoraSearch::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAppPandoraSearch::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CAppPandoraSearch,
      qt_meta_data_CAppPandoraSearch, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAppPandoraSearch::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAppPandoraSearch::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAppPandoraSearch::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAppPandoraSearch))
        return static_cast<void*>(const_cast< CAppPandoraSearch*>(this));
    return QObject::qt_metacast(_clname);
}

int CAppPandoraSearch::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
        case 0: *reinterpret_cast< QString*>(_v) = getSearchString(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setSearchString(*reinterpret_cast< QString*>(_v)); break;
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
void CAppPandoraSearch::updateSearchString()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CAppPandoraSearch::autoSearchCompleted(const QStringList & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CAppPandoraSearch::extendedSearchCompleted(const QStringList & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CAppPandoraSearch::handleError(const EPndrError _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
