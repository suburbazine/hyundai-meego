/****************************************************************************
** Meta object code from reading C++ file 'dhavnwm.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/dhavnwm.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dhavnwm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DHAVNWm[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,
      26,    8,    8,    8, 0x05,
      35,    8,    8,    8, 0x05,
      51,   49,    8,    8, 0x05,
      79,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      99,    8,    8,    8, 0x08,
     118,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DHAVNWm[] = {
    "DHAVNWm\0\0firstActivated()\0agreed()\0"
    "backToAgree()\0,\0screenChanged(bool,QString)\0"
    "checkAgree(QString)\0registApp(QString)\0"
    "checkAgreeApp(QString)\0"
};

void DHAVNWm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DHAVNWm *_t = static_cast<DHAVNWm *>(_o);
        switch (_id) {
        case 0: _t->firstActivated(); break;
        case 1: _t->agreed(); break;
        case 2: _t->backToAgree(); break;
        case 3: _t->screenChanged((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->checkAgree((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->registApp((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->checkAgreeApp((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DHAVNWm::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DHAVNWm::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_DHAVNWm,
      qt_meta_data_DHAVNWm, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DHAVNWm::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DHAVNWm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DHAVNWm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DHAVNWm))
        return static_cast<void*>(const_cast< DHAVNWm*>(this));
    return QThread::qt_metacast(_clname);
}

int DHAVNWm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void DHAVNWm::firstActivated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DHAVNWm::agreed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void DHAVNWm::backToAgree()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void DHAVNWm::screenChanged(bool _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DHAVNWm::checkAgree(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
