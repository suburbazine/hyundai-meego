/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_QwertyKeypad_ChinesePinyin_Item.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_QwertyKeypad_ChinesePinyin_Item.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_QwertyKeypad_ChinesePinyin_Item.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PinyinListModelItem[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      29,   20,   21,   20, 0x02,
      47,   20,   43,   20, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_PinyinListModelItem[] = {
    "PinyinListModelItem\0\0QString\0getItemName()\0"
    "int\0getItemSize()\0"
};

void PinyinListModelItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PinyinListModelItem *_t = static_cast<PinyinListModelItem *>(_o);
        switch (_id) {
        case 0: { QString _r = _t->getItemName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 1: { int _r = _t->getItemSize();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PinyinListModelItem::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PinyinListModelItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PinyinListModelItem,
      qt_meta_data_PinyinListModelItem, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PinyinListModelItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PinyinListModelItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PinyinListModelItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PinyinListModelItem))
        return static_cast<void*>(const_cast< PinyinListModelItem*>(this));
    return QObject::qt_metacast(_clname);
}

int PinyinListModelItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
