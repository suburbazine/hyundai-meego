/****************************************************************************
** Meta object code from reading C++ file 'deckmanager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/deck/deckmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'deckmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DeckManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      37,   12,   12,   12, 0x05,
      50,   12,   12,   12, 0x05,
      82,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     110,   12,   12,   12, 0x0a,
     133,   12,   12,   12, 0x08,
     148,   12,   12,   12, 0x08,
     169,   12,   12,   12, 0x08,
     198,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DeckManager[] = {
    "DeckManager\0\0OnDiscStateChanged(int)\0"
    "OnError(int)\0OnCommErrorWithDeckChanged(int)\0"
    "OnDeckPriorityLangChanged()\0"
    "HandlePacketReceived()\0HandleReload()\0"
    "HandleCommonStatus()\0HandleDiscDetectLimitation()\0"
    "HandleDiagnostic()\0"
};

void DeckManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DeckManager *_t = static_cast<DeckManager *>(_o);
        switch (_id) {
        case 0: _t->OnDiscStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->OnError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->OnCommErrorWithDeckChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->OnDeckPriorityLangChanged(); break;
        case 4: _t->HandlePacketReceived(); break;
        case 5: _t->HandleReload(); break;
        case 6: _t->HandleCommonStatus(); break;
        case 7: _t->HandleDiscDetectLimitation(); break;
        case 8: _t->HandleDiagnostic(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DeckManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DeckManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DeckManager,
      qt_meta_data_DeckManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DeckManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DeckManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DeckManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DeckManager))
        return static_cast<void*>(const_cast< DeckManager*>(this));
    return QObject::qt_metacast(_clname);
}

int DeckManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void DeckManager::OnDiscStateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DeckManager::OnError(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DeckManager::OnCommErrorWithDeckChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DeckManager::OnDeckPriorityLangChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
