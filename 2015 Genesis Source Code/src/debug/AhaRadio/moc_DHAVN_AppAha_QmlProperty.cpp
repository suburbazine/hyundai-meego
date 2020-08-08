/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppAha_QmlProperty.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppAha_QmlProperty.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppAha_QmlProperty.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CAppAhaQmlProperty[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   20,   19,   19, 0x0a,
      49,   43,   19,   19, 0x0a,
      73,   19,   69,   19, 0x0a,
      90,   19,   69,   19, 0x0a,
     117,  105,   19,   19, 0x0a,
     163,  143,   19,   19, 0x0a,
     198,   19,  190,   19, 0x0a,
     217,   19,  190,   19, 0x0a,
     241,   19,  190,   19, 0x0a,
     282,  264,   19,   19, 0x0a,
     315,   19,  310,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CAppAhaQmlProperty[] = {
    "CAppAhaQmlProperty\0\0area\0setFocusArea(int)\0"
    "index\0setFocusScreen(int)\0int\0"
    "getFocusScreen()\0getFocusArea()\0"
    "errViewText\0setErrorViewText(QString)\0"
    "errorText2LineIndex\0setErrorViewText2Line(int)\0"
    "QString\0getErrorViewText()\0"
    "getErrorViewText2Line()\0getActiveStationName()\0"
    "isFirstTrackState\0setFirstTrackEntrance(bool)\0"
    "bool\0getFirstTrackEntrance()\0"
};

void CAppAhaQmlProperty::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CAppAhaQmlProperty *_t = static_cast<CAppAhaQmlProperty *>(_o);
        switch (_id) {
        case 0: _t->setFocusArea((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 1: _t->setFocusScreen((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 2: { int _r = _t->getFocusScreen();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->getFocusArea();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: _t->setErrorViewText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->setErrorViewText2Line((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 6: { QString _r = _t->getErrorViewText();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 7: { QString _r = _t->getErrorViewText2Line();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 8: { QString _r = _t->getActiveStationName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 9: _t->setFirstTrackEntrance((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: { bool _r = _t->getFirstTrackEntrance();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CAppAhaQmlProperty::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAppAhaQmlProperty::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CAppAhaQmlProperty,
      qt_meta_data_CAppAhaQmlProperty, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAppAhaQmlProperty::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAppAhaQmlProperty::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAppAhaQmlProperty::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAppAhaQmlProperty))
        return static_cast<void*>(const_cast< CAppAhaQmlProperty*>(this));
    return QObject::qt_metacast(_clname);
}

int CAppAhaQmlProperty::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
