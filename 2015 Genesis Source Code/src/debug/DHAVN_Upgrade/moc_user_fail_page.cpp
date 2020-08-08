/****************************************************************************
** Meta object code from reading C++ file 'user_fail_page.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/ui/user_fail_page.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'user_fail_page.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UserFailPage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      26,   13,   13,   13, 0x08,
      42,   13,   13,   13, 0x08,
      56,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UserFailPage[] = {
    "UserFailPage\0\0rebootNow()\0rebootClicked()\0"
    "decreaseSec()\0jogDialEvent(int)\0"
};

void UserFailPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UserFailPage *_t = static_cast<UserFailPage *>(_o);
        switch (_id) {
        case 0: _t->rebootNow(); break;
        case 1: _t->rebootClicked(); break;
        case 2: _t->decreaseSec(); break;
        case 3: _t->jogDialEvent((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UserFailPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UserFailPage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UserFailPage,
      qt_meta_data_UserFailPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UserFailPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UserFailPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UserFailPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UserFailPage))
        return static_cast<void*>(const_cast< UserFailPage*>(this));
    return QWidget::qt_metacast(_clname);
}

int UserFailPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void UserFailPage::rebootNow()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
