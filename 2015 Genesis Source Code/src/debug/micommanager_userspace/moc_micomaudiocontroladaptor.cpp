/****************************************************************************
** Meta object code from reading C++ file 'micomaudiocontroladaptor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/adaptors/micomaudiocontroladaptor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'micomaudiocontroladaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MicomAudioControlAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       1,   14, // classinfo
       1,   16, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // classinfo: key, value
      51,   25,

 // slots: signature, parameters, type, tag, flags
      72,   68,   67,   67, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MicomAudioControlAdaptor[] = {
    "MicomAudioControlAdaptor\0"
    "com.lge.car.micom.audioif\0D-Bus Interface\0"
    "\0msg\0ProcessAudioControlMessage(QDBusMessage)\0"
};

void MicomAudioControlAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MicomAudioControlAdaptor *_t = static_cast<MicomAudioControlAdaptor *>(_o);
        switch (_id) {
        case 0: _t->ProcessAudioControlMessage((*reinterpret_cast< const QDBusMessage(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MicomAudioControlAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MicomAudioControlAdaptor::staticMetaObject = {
    { &QDBusAbstractAdaptor::staticMetaObject, qt_meta_stringdata_MicomAudioControlAdaptor,
      qt_meta_data_MicomAudioControlAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MicomAudioControlAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MicomAudioControlAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MicomAudioControlAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MicomAudioControlAdaptor))
        return static_cast<void*>(const_cast< MicomAudioControlAdaptor*>(this));
    return QDBusAbstractAdaptor::qt_metacast(_clname);
}

int MicomAudioControlAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractAdaptor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
