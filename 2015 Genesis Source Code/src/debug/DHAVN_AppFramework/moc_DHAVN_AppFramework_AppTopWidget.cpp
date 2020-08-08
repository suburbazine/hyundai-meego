/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppFramework_AppTopWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppFramework_AppTopWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppFramework_AppTopWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppTopWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      46,   37,   13,   13, 0x0a,
      76,   13,   13,   13, 0x2a,
      97,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AppTopWidget[] = {
    "AppTopWidget\0\0SignalDrawFinish(uint)\0"
    "nDisplay\0startEventHandling(DISPLAY_T)\0"
    "startEventHandling()\0SlotEmitFinish()\0"
};

void AppTopWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppTopWidget *_t = static_cast<AppTopWidget *>(_o);
        switch (_id) {
        case 0: _t->SignalDrawFinish((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 1: _t->startEventHandling((*reinterpret_cast< DISPLAY_T(*)>(_a[1]))); break;
        case 2: _t->startEventHandling(); break;
        case 3: _t->SlotEmitFinish(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppTopWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppTopWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AppTopWidget,
      qt_meta_data_AppTopWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppTopWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppTopWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppTopWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppTopWidget))
        return static_cast<void*>(const_cast< AppTopWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int AppTopWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void AppTopWidget::SignalDrawFinish(uint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
