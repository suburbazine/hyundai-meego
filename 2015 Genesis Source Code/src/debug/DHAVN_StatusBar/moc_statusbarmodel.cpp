/****************************************************************************
** Meta object code from reading C++ file 'statusbarmodel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/statusbarmodel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'statusbarmodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StatusBarModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      33,   15,   15,   15, 0x05,
      72,   52,   15,   15, 0x05,
     101,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     122,   15,   15,   15, 0x0a,
     149,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_StatusBarModel[] = {
    "StatusBarModel\0\0updateIconView()\0"
    "updateEuropeInfo()\0chour,cminute,cAMPM\0"
    "timeChanged(int,int,QString)\0"
    "powerStatusChanged()\0statusChanged(QStringList)\0"
    "eventChanged(QStringList)\0"
};

void StatusBarModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        StatusBarModel *_t = static_cast<StatusBarModel *>(_o);
        switch (_id) {
        case 0: _t->updateIconView(); break;
        case 1: _t->updateEuropeInfo(); break;
        case 2: _t->timeChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 3: _t->powerStatusChanged(); break;
        case 4: _t->statusChanged((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 5: _t->eventChanged((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData StatusBarModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject StatusBarModel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_StatusBarModel,
      qt_meta_data_StatusBarModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StatusBarModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StatusBarModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StatusBarModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StatusBarModel))
        return static_cast<void*>(const_cast< StatusBarModel*>(this));
    return QObject::qt_metacast(_clname);
}

int StatusBarModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void StatusBarModel::updateIconView()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void StatusBarModel::updateEuropeInfo()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void StatusBarModel::timeChanged(int _t1, int _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void StatusBarModel::powerStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
