/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_EngModeNotifier.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/util/DHAVN_AppMediaPlayer_EngModeNotifier.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_EngModeNotifier.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EngModeNotifier[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,
      34,   16,   16,   16, 0x0a,
      60,   56,   16,   16, 0x0a,
      95,   87,   16,   16, 0x0a,
     135,  124,   16,   16, 0x0a,
     187,  171,   16,   16, 0x0a,
     216,  171,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EngModeNotifier[] = {
    "EngModeNotifier\0\0OnAsk_DeckInfo()\0"
    "OnAsk_AudioPathInfo()\0in0\0"
    "OnSet_AudioPathInfo(uchar)\0service\0"
    "OnServiceRegistered(QString)\0aErrorCode\0"
    "HandleCommErrorWithDeckChanged(int)\0"
    "eCountryVariant\0ResetData(EMPCountryVariant)\0"
    "FactoryReset(EMPCountryVariant)\0"
};

void EngModeNotifier::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        EngModeNotifier *_t = static_cast<EngModeNotifier *>(_o);
        switch (_id) {
        case 0: _t->OnAsk_DeckInfo(); break;
        case 1: _t->OnAsk_AudioPathInfo(); break;
        case 2: _t->OnSet_AudioPathInfo((*reinterpret_cast< uchar(*)>(_a[1]))); break;
        case 3: _t->OnServiceRegistered((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->HandleCommErrorWithDeckChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->ResetData((*reinterpret_cast< EMPCountryVariant(*)>(_a[1]))); break;
        case 6: _t->FactoryReset((*reinterpret_cast< EMPCountryVariant(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData EngModeNotifier::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject EngModeNotifier::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_EngModeNotifier,
      qt_meta_data_EngModeNotifier, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EngModeNotifier::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EngModeNotifier::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EngModeNotifier::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EngModeNotifier))
        return static_cast<void*>(const_cast< EngModeNotifier*>(this));
    return QObject::qt_metacast(_clname);
}

int EngModeNotifier::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
