/****************************************************************************
** Meta object code from reading C++ file 'deckengineermode.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/engineermode/deckengineermode.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'deckengineermode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DeckEngineerMode[] = {

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
      25,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      57,   18,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DeckEngineerMode[] = {
    "DeckEngineerMode\0\0bError\0"
    "OnCommErrorWithDeckChanged(int)\0"
    "HandleCommErrorWithDeckChanged(int)\0"
};

void DeckEngineerMode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DeckEngineerMode *_t = static_cast<DeckEngineerMode *>(_o);
        switch (_id) {
        case 0: _t->OnCommErrorWithDeckChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->HandleCommErrorWithDeckChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DeckEngineerMode::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DeckEngineerMode::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DeckEngineerMode,
      qt_meta_data_DeckEngineerMode, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DeckEngineerMode::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DeckEngineerMode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DeckEngineerMode::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DeckEngineerMode))
        return static_cast<void*>(const_cast< DeckEngineerMode*>(this));
    return QObject::qt_metacast(_clname);
}

int DeckEngineerMode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void DeckEngineerMode::OnCommErrorWithDeckChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
