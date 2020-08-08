/****************************************************************************
** Meta object code from reading C++ file 'msc_requestdownload.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/modules/most/msc/msc_requestdownload.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'msc_requestdownload.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MscRequestDownload[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MscRequestDownload[] = {
    "MscRequestDownload\0\0RequestDownload(QDBusVariant)\0"
};

void MscRequestDownload::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MscRequestDownload *_t = static_cast<MscRequestDownload *>(_o);
        switch (_id) {
        case 0: _t->RequestDownload((*reinterpret_cast< const QDBusVariant(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MscRequestDownload::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MscRequestDownload::staticMetaObject = {
    { &MscAbstract::staticMetaObject, qt_meta_stringdata_MscRequestDownload,
      qt_meta_data_MscRequestDownload, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MscRequestDownload::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MscRequestDownload::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MscRequestDownload::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MscRequestDownload))
        return static_cast<void*>(const_cast< MscRequestDownload*>(this));
    return MscAbstract::qt_metacast(_clname);
}

int MscRequestDownload::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MscAbstract::qt_metacall(_c, _id, _a);
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
