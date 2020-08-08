/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_MOSTManager_DiagnosisFBlock_ShadowControl.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_MOSTManager_DiagnosisFBlock_ShadowControl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_MOSTManager_DiagnosisFBlock_ShadowControl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CDiagnosisFBlockShadowControl[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      45,   31,   30,   30, 0x05,
      78,   30,   30,   30, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CDiagnosisFBlockShadowControl[] = {
    "CDiagnosisFBlockShadowControl\0\0"
    "mainSWVersion\0MainSWVersionStatus(QStringList)\0"
    "GetVariantCodeStatus()\0"
};

void CDiagnosisFBlockShadowControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CDiagnosisFBlockShadowControl *_t = static_cast<CDiagnosisFBlockShadowControl *>(_o);
        switch (_id) {
        case 0: _t->MainSWVersionStatus((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->GetVariantCodeStatus(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CDiagnosisFBlockShadowControl::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CDiagnosisFBlockShadowControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CDiagnosisFBlockShadowControl,
      qt_meta_data_CDiagnosisFBlockShadowControl, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CDiagnosisFBlockShadowControl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CDiagnosisFBlockShadowControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CDiagnosisFBlockShadowControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDiagnosisFBlockShadowControl))
        return static_cast<void*>(const_cast< CDiagnosisFBlockShadowControl*>(this));
    if (!strcmp(_clname, "CControlThreadBase"))
        return static_cast< CControlThreadBase*>(const_cast< CDiagnosisFBlockShadowControl*>(this));
    return QObject::qt_metacast(_clname);
}

int CDiagnosisFBlockShadowControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CDiagnosisFBlockShadowControl::MainSWVersionStatus(const QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CDiagnosisFBlockShadowControl::GetVariantCodeStatus()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
