/****************************************************************************
** Meta object code from reading C++ file 'update_widget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/ui/update_widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'update_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UpdateWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      28,   13,   13,   13, 0x05,
      41,   13,   13,   13, 0x05,
      63,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      83,   81,   13,   13, 0x08,
     109,   13,   13,   13, 0x08,
     133,  129,   13,   13, 0x08,
     176,  129,   13,   13, 0x08,
     225,   13,   13,   13, 0x08,
     247,   81,   13,   13, 0x08,
     276,   13,   13,   13, 0x08,
     293,   13,   13,   13, 0x08,
     307,   13,   13,   13, 0x08,
     320,   13,   13,   13, 0x08,
     342,  338,   13,   13, 0x0a,
     364,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_UpdateWidget[] = {
    "UpdateWidget\0\0startReboot()\0stopUpdate()\0"
    "startRebootFailCase()\0jogDialEvent(int)\0"
    ",\0availableUpdate(int,uint)\0"
    "goToProgressPages()\0,,,\0"
    "progressChanged(QString,QString,uint,uint)\0"
    "progressChangedDirect(QString,QString,uint,uint)\0"
    "imageValidateFailed()\0"
    "updateFinished(bool,QString)\0"
    "goToVerifyPage()\0usbDetached()\0"
    "setUser(int)\0checkModeChange()\0msg\0"
    "failToUpdate(QString)\0goToFailPage()\0"
};

void UpdateWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UpdateWidget *_t = static_cast<UpdateWidget *>(_o);
        switch (_id) {
        case 0: _t->startReboot(); break;
        case 1: _t->stopUpdate(); break;
        case 2: _t->startRebootFailCase(); break;
        case 3: _t->jogDialEvent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->availableUpdate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 5: _t->goToProgressPages(); break;
        case 6: _t->progressChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 7: _t->progressChangedDirect((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 8: _t->imageValidateFailed(); break;
        case 9: _t->updateFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 10: _t->goToVerifyPage(); break;
        case 11: _t->usbDetached(); break;
        case 12: _t->setUser((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->checkModeChange(); break;
        case 14: _t->failToUpdate((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: _t->goToFailPage(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UpdateWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UpdateWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UpdateWidget,
      qt_meta_data_UpdateWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UpdateWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UpdateWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UpdateWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UpdateWidget))
        return static_cast<void*>(const_cast< UpdateWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int UpdateWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void UpdateWidget::startReboot()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void UpdateWidget::stopUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void UpdateWidget::startRebootFailCase()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void UpdateWidget::jogDialEvent(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
