/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppUserManual_DocumentWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppUserManual_DocumentWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppUserManual_DocumentWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppUserManual_DocumentWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      35,   29,   30,   29, 0x0a,
      54,   49,   29,   29, 0x0a,
      67,   29,   29,   29, 0x2a,
      84,   77,   29,   29, 0x0a,
      97,   77,   29,   29, 0x0a,
     116,   77,   29,   29, 0x0a,
     145,  139,   29,   29, 0x0a,
     161,   29,   29,   29, 0x0a,
     175,   29,   29,   29, 0x0a,
     200,  189,   29,   29, 0x0a,
     232,  214,   30,   29, 0x0a,
     272,  261,   30,   29, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AppUserManual_DocumentWidget[] = {
    "AppUserManual_DocumentWidget\0\0bool\0"
    "setDocument()\0page\0setpage(int)\0"
    "setpage()\0status\0setDrs(bool)\0"
    "isFullScreen(bool)\0setNumericKeypad(bool)\0"
    "scale\0setScale(qreal)\0setNextPage()\0"
    "setPrevPage()\0pageNumber\0gotoPage(int)\0"
    "searchText,bFirst\0searchForward(QString&,bool)\0"
    "searchText\0searchBackward(QString&)\0"
};

void AppUserManual_DocumentWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppUserManual_DocumentWidget *_t = static_cast<AppUserManual_DocumentWidget *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->setDocument();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: _t->setpage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setpage(); break;
        case 3: _t->setDrs((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->isFullScreen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->setNumericKeypad((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setScale((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 7: _t->setNextPage(); break;
        case 8: _t->setPrevPage(); break;
        case 9: _t->gotoPage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: { bool _r = _t->searchForward((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: { bool _r = _t->searchBackward((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppUserManual_DocumentWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppUserManual_DocumentWidget::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_AppUserManual_DocumentWidget,
      qt_meta_data_AppUserManual_DocumentWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppUserManual_DocumentWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppUserManual_DocumentWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppUserManual_DocumentWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppUserManual_DocumentWidget))
        return static_cast<void*>(const_cast< AppUserManual_DocumentWidget*>(this));
    return QLabel::qt_metacast(_clname);
}

int AppUserManual_DocumentWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
