/****************************************************************************
** Meta object code from reading C++ file 'alertdialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/ui/alertdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'alertdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AlertDialog[] = {

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
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      29,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AlertDialog[] = {
    "AlertDialog\0\0exitConfirmed()\0"
    "jogDialEvent(int)\0"
};

void AlertDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AlertDialog *_t = static_cast<AlertDialog *>(_o);
        switch (_id) {
        case 0: _t->exitConfirmed(); break;
        case 1: _t->jogDialEvent((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AlertDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AlertDialog::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AlertDialog,
      qt_meta_data_AlertDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AlertDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AlertDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AlertDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AlertDialog))
        return static_cast<void*>(const_cast< AlertDialog*>(this));
    return QWidget::qt_metacast(_clname);
}

int AlertDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void AlertDialog::exitConfirmed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
