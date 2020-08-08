/****************************************************************************
** Meta object code from reading C++ file 'updatelauncher.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/updatelauncher.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'updatelauncher.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UpdateLauncher[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      58,   16,   15,   15, 0x05,
     107,   16,   15,   15, 0x05,
     162,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     182,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UpdateLauncher[] = {
    "UpdateLauncher\0\0"
    "name,description,progress,weight_progress\0"
    "updateProgressChanged(QString,QString,uint,uint)\0"
    "updateProgressChangedDirect(QString,QString,uint,uint)\0"
    "updateAllFinished()\0statusChanged(int)\0"
};

void UpdateLauncher::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UpdateLauncher *_t = static_cast<UpdateLauncher *>(_o);
        switch (_id) {
        case 0: _t->updateProgressChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 1: _t->updateProgressChangedDirect((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4]))); break;
        case 2: _t->updateAllFinished(); break;
        case 3: _t->statusChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UpdateLauncher::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UpdateLauncher::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UpdateLauncher,
      qt_meta_data_UpdateLauncher, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UpdateLauncher::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UpdateLauncher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UpdateLauncher::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UpdateLauncher))
        return static_cast<void*>(const_cast< UpdateLauncher*>(this));
    return QObject::qt_metacast(_clname);
}

int UpdateLauncher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void UpdateLauncher::updateProgressChanged(const QString & _t1, const QString & _t2, unsigned int _t3, unsigned int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UpdateLauncher::updateProgressChangedDirect(const QString & _t1, const QString & _t2, unsigned int _t3, unsigned int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UpdateLauncher::updateAllFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
