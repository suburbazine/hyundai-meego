/****************************************************************************
** Meta object code from reading C++ file 'ask_upgrade_page.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/ui/ask_upgrade_page.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ask_upgrade_page.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AskUpgradePage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      33,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      49,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AskUpgradePage[] = {
    "AskUpgradePage\0\0upgradeConfirm()\0"
    "upgradeCancel()\0jogDialEvent(int)\0"
};

void AskUpgradePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AskUpgradePage *_t = static_cast<AskUpgradePage *>(_o);
        switch (_id) {
        case 0: _t->upgradeConfirm(); break;
        case 1: _t->upgradeCancel(); break;
        case 2: _t->jogDialEvent((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AskUpgradePage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AskUpgradePage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AskUpgradePage,
      qt_meta_data_AskUpgradePage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AskUpgradePage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AskUpgradePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AskUpgradePage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AskUpgradePage))
        return static_cast<void*>(const_cast< AskUpgradePage*>(this));
    return QWidget::qt_metacast(_clname);
}

int AskUpgradePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void AskUpgradePage::upgradeConfirm()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void AskUpgradePage::upgradeCancel()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
