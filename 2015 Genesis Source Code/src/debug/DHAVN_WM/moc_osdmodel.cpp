/****************************************************************************
** Meta object code from reading C++ file 'osdmodel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/statusbar/osdmodel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'osdmodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OSDModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      25,   21,    9,    9, 0x08,
      67,    9,    9,    9, 0x08,
      80,    9,    9,    9, 0x08,
      96,   94,    9,    9, 0x08,
     115,    9,    9,    9, 0x08,
     126,   94,    9,    9, 0x08,
     152,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_OSDModel[] = {
    "OSDModel\0\0osdEmpty()\0,,,\0"
    "processOSD(int,QString,QStringList,uchar)\0"
    "osdTimeout()\0activateOSD()\0,\0"
    "lockOSD(bool,bool)\0clearAll()\0"
    "checkScreen(bool,QString)\0clearById(uchar)\0"
};

void OSDModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OSDModel *_t = static_cast<OSDModel *>(_o);
        switch (_id) {
        case 0: _t->osdEmpty(); break;
        case 1: _t->processOSD((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3])),(*reinterpret_cast< uchar(*)>(_a[4]))); break;
        case 2: _t->osdTimeout(); break;
        case 3: _t->activateOSD(); break;
        case 4: _t->lockOSD((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->clearAll(); break;
        case 6: _t->checkScreen((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: _t->clearById((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData OSDModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OSDModel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_OSDModel,
      qt_meta_data_OSDModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OSDModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OSDModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OSDModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OSDModel))
        return static_cast<void*>(const_cast< OSDModel*>(this));
    return QObject::qt_metacast(_clname);
}

int OSDModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void OSDModel::osdEmpty()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
