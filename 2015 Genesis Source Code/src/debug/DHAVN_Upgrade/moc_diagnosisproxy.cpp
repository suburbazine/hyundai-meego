/****************************************************************************
** Meta object code from reading C++ file 'diagnosisproxy.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/modules/most/proxy/diagnosisproxy.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'diagnosisproxy.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DiagnosisInterface[] = {

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
      20,   19,   19,   19, 0x05,
      57,   43,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
     114,   19,   90,   19, 0x0a,
     145,  133,   90,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DiagnosisInterface[] = {
    "DiagnosisInterface\0\0GetVariantCodeStatus()\0"
    "mainSWVersion\0MainSWVersionStatus(QStringList)\0"
    "QDBusPendingReply<bool>\0GetMainSWVersion()\0"
    "variantcode\0SetVariantCode(QDBusVariant)\0"
};

void DiagnosisInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DiagnosisInterface *_t = static_cast<DiagnosisInterface *>(_o);
        switch (_id) {
        case 0: _t->GetVariantCodeStatus(); break;
        case 1: _t->MainSWVersionStatus((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 2: { QDBusPendingReply<bool> _r = _t->GetMainSWVersion();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 3: { QDBusPendingReply<bool> _r = _t->SetVariantCode((*reinterpret_cast< const QDBusVariant(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DiagnosisInterface::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DiagnosisInterface::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_DiagnosisInterface,
      qt_meta_data_DiagnosisInterface, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DiagnosisInterface::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DiagnosisInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DiagnosisInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DiagnosisInterface))
        return static_cast<void*>(const_cast< DiagnosisInterface*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int DiagnosisInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
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
void DiagnosisInterface::GetVariantCodeStatus()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DiagnosisInterface::MainSWVersionStatus(const QStringList & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
