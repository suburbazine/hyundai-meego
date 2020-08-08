/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_QwertyKeypad_ChinesePinyin.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_QwertyKeypad_ChinesePinyin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_QwertyKeypad_ChinesePinyin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QwertyKeypadChinesePinyin[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       3,  129, // properties
       1,  141, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   26,   26,   26, 0x05,
      48,   26,   26,   26, 0x05,
      72,   26,   26,   26, 0x05,
      96,   26,   26,   26, 0x05,

 // methods: signature, parameters, type, tag, flags
     148,  142,  121,   26, 0x02,
     161,  142,  157,   26, 0x02,
     186,  142,  178,   26, 0x02,
     203,  142,  178,   26, 0x02,
     266,  233,   26,   26, 0x02,
     306,  298,   26,   26, 0x02,
     326,   26,   26,   26, 0x02,
     340,   26,  157,   26, 0x02,
     363,   26,  351,   26, 0x02,
     407,  396,  391,   26, 0x02,
     434,   26,   26,   26, 0x02,
     457,   26,   26,   26, 0x02,
     480,   26,  178,   26, 0x02,
     499,   26,  157,   26, 0x02,
     537,  528,   26,   26, 0x02,
     571,  528,   26,   26, 0x02,
     599,   26,   26,   26, 0x02,
     632,   26,  157,   26, 0x02,
     657,   26,   26,   26, 0x02,

 // properties: name, type, flags
     679,  157, 0x02495001,
     691,  157, 0x02495001,
     706,  157, 0x02495001,

 // properties: notify_signal_id
       0,
       2,
       3,

 // enums: name, flags, count, data
     722, 0x0,    5,  145,

 // enum data: key, value
     729, uint(QwertyKeypadChinesePinyin::LayoutMandarinPinyin),
     750, uint(QwertyKeypadChinesePinyin::LayoutMandarinHandWriting),
     776, uint(QwertyKeypadChinesePinyin::LayoutCantonesePinyin),
     798, uint(QwertyKeypadChinesePinyin::LayoutCantoneseHandWriting),
     825, uint(QwertyKeypadChinesePinyin::LayoutMax),

       0        // eod
};

static const char qt_meta_stringdata_QwertyKeypadChinesePinyin[] = {
    "QwertyKeypadChinesePinyin\0\0"
    "pinyinCountChanged()\0phoneBookDBOpenFailed()\0"
    "currentPageNumChanged()\0"
    "pinyinTotalPageChanged()\0PinyinListModelItem*\0"
    "index\0get(int)\0int\0getItemSize(int)\0"
    "QString\0getItemName(int)\0"
    "getNameByItemIndexOfPage(int)\0"
    "nType,bPhoneBook,phoneBookDBPath\0"
    "setKeypadType(int,bool,QString)\0keyWord\0"
    "setKeyWord(QString)\0clearBuffer()\0"
    "getCount()\0QStringList\0"
    "getConsonantsNextCharList()\0bool\0"
    "searchWord\0getDKBDPrediction(QString)\0"
    "incrementCurrentPage()\0decrementCurrentPage()\0"
    "getPhoneticSpell()\0getPrevPhoneticSpellLength()\0"
    "inputTxt\0addPrevPhoneticInputText(QString)\0"
    "setUpdateInputText(QString)\0"
    "setPrevPhoneticInputTextByBack()\0"
    "getPhoneticCountResult()\0clearPhoneticBuffer()\0"
    "pinyinCount\0currentPageNum\0pinyinTotalPage\0"
    "Layout\0LayoutMandarinPinyin\0"
    "LayoutMandarinHandWriting\0"
    "LayoutCantonesePinyin\0LayoutCantoneseHandWriting\0"
    "LayoutMax\0"
};

void QwertyKeypadChinesePinyin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QwertyKeypadChinesePinyin *_t = static_cast<QwertyKeypadChinesePinyin *>(_o);
        switch (_id) {
        case 0: _t->pinyinCountChanged(); break;
        case 1: _t->phoneBookDBOpenFailed(); break;
        case 2: _t->currentPageNumChanged(); break;
        case 3: _t->pinyinTotalPageChanged(); break;
        case 4: { PinyinListModelItem* _r = _t->get((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< PinyinListModelItem**>(_a[0]) = _r; }  break;
        case 5: { int _r = _t->getItemSize((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 6: { QString _r = _t->getItemName((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 7: { QString _r = _t->getNameByItemIndexOfPage((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 8: _t->setKeypadType((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 9: _t->setKeyWord((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->clearBuffer(); break;
        case 11: { int _r = _t->getCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 12: { QStringList _r = _t->getConsonantsNextCharList();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = _r; }  break;
        case 13: { bool _r = _t->getDKBDPrediction((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 14: _t->incrementCurrentPage(); break;
        case 15: _t->decrementCurrentPage(); break;
        case 16: { QString _r = _t->getPhoneticSpell();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 17: { int _r = _t->getPrevPhoneticSpellLength();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 18: _t->addPrevPhoneticInputText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 19: _t->setUpdateInputText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: _t->setPrevPhoneticInputTextByBack(); break;
        case 21: { int _r = _t->getPhoneticCountResult();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 22: _t->clearPhoneticBuffer(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QwertyKeypadChinesePinyin::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QwertyKeypadChinesePinyin::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_QwertyKeypadChinesePinyin,
      qt_meta_data_QwertyKeypadChinesePinyin, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QwertyKeypadChinesePinyin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QwertyKeypadChinesePinyin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QwertyKeypadChinesePinyin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwertyKeypadChinesePinyin))
        return static_cast<void*>(const_cast< QwertyKeypadChinesePinyin*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int QwertyKeypadChinesePinyin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = getPinyinCount(); break;
        case 1: *reinterpret_cast< int*>(_v) = getCurrentPageNum(); break;
        case 2: *reinterpret_cast< int*>(_v) = getPinyinTotalPage(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QwertyKeypadChinesePinyin::pinyinCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QwertyKeypadChinesePinyin::phoneBookDBOpenFailed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QwertyKeypadChinesePinyin::currentPageNumChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void QwertyKeypadChinesePinyin::pinyinTotalPageChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
