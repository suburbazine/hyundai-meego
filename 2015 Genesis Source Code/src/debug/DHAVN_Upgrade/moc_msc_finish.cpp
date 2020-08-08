/****************************************************************************
** Meta object code from reading C++ file 'msc_finish.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/modules/most/msc/msc_finish.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'msc_finish.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MscFinish[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      28,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      49,   47,   10,   10, 0x08,
      77,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MscFinish[] = {
    "MscFinish\0\0updateFinished()\0"
    "iBoxWaitCount(int)\0,\0UpdateProcessing(uint,uint)\0"
    "checkFinishIBoxUpdate()\0"
};

void MscFinish::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MscFinish *_t = static_cast<MscFinish *>(_o);
        switch (_id) {
        case 0: _t->updateFinished(); break;
        case 1: _t->iBoxWaitCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->UpdateProcessing((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 3: _t->checkFinishIBoxUpdate(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MscFinish::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MscFinish::staticMetaObject = {
    { &MscAbstract::staticMetaObject, qt_meta_stringdata_MscFinish,
      qt_meta_data_MscFinish, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MscFinish::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MscFinish::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MscFinish::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MscFinish))
        return static_cast<void*>(const_cast< MscFinish*>(this));
    return MscAbstract::qt_metacast(_clname);
}

int MscFinish::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MscAbstract::qt_metacall(_c, _id, _a);
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
void MscFinish::updateFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void MscFinish::iBoxWaitCount(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
