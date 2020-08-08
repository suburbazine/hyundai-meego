/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_CATreeModel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/util/audio/DHAVN_AppMediaPlayer_CATreeModel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_CATreeModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CATreeModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       2,  119, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      25,   12,   12,   12, 0x05,
      53,   12,   12,   12, 0x05,
      79,   12,   12,   12, 0x05,
     111,   12,   12,   12, 0x05,
     135,   12,   12,   12, 0x05,
     173,  154,   12,   12, 0x05,
     205,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     222,   12,   12,   12, 0x0a,

 // methods: signature, parameters, type, tag, flags
     283,  256,  248,   12, 0x02,
     306,   12,  248,   12, 0x02,
     329,  256,  248,   12, 0x02,
     357,   12,  248,   12, 0x02,
     373,   12,  248,   12, 0x02,
     390,  256,  248,   12, 0x02,
     424,  418,  248,   12, 0x02,
     446,   12,  441,   12, 0x02,
     459,   12,   12,   12, 0x02,
     467,   12,   12,   12, 0x02,
     479,   12,  475,   12, 0x02,
     492,   12,  441,   12, 0x02,

 // properties: name, type, flags
     509,  475, 0x02495001,
     528,  475, 0x02495001,

 // properties: notify_signal_id
       1,
       2,

       0        // eod
};

static const char qt_meta_stringdata_CATreeModel[] = {
    "CATreeModel\0\0closeList()\0"
    "currentPlayingItemChanged()\0"
    "currentFocusItemChanged()\0"
    "setCurrentPlayingItemPosition()\0"
    "signalUpdateCountInfo()\0listDefaultFocus()\0"
    "title,historyStack\0updateTextItemView(QString,int)\0"
    "clearSelection()\0onMp3CdMediaInfoChanged()\0"
    "QString\0currentIndex,isListFocused\0"
    "getCountInfo(int,bool)\0getCurrentFolderName()\0"
    "getCountInfoFirst(int,bool)\0getListStatus()\0"
    "getListFolders()\0getCompareFolders(int,bool)\0"
    "index\0onItemClick(int)\0bool\0openParent()\0"
    "clear()\0reset()\0int\0getTreeDep()\0"
    "isCurrItemRoot()\0currentPlayingItem\0"
    "currentFocusItem\0"
};

void CATreeModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CATreeModel *_t = static_cast<CATreeModel *>(_o);
        switch (_id) {
        case 0: _t->closeList(); break;
        case 1: _t->currentPlayingItemChanged(); break;
        case 2: _t->currentFocusItemChanged(); break;
        case 3: _t->setCurrentPlayingItemPosition(); break;
        case 4: _t->signalUpdateCountInfo(); break;
        case 5: _t->listDefaultFocus(); break;
        case 6: _t->updateTextItemView((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->clearSelection(); break;
        case 8: _t->onMp3CdMediaInfoChanged(); break;
        case 9: { QString _r = _t->getCountInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 10: { QString _r = _t->getCurrentFolderName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 11: { QString _r = _t->getCountInfoFirst((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 12: { QString _r = _t->getListStatus();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 13: { QString _r = _t->getListFolders();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 14: { QString _r = _t->getCompareFolders((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 15: { QString _r = _t->onItemClick((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 16: { bool _r = _t->openParent();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 17: _t->clear(); break;
        case 18: _t->reset(); break;
        case 19: { int _r = _t->getTreeDep();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 20: { bool _r = _t->isCurrItemRoot();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CATreeModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CATreeModel::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_CATreeModel,
      qt_meta_data_CATreeModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CATreeModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CATreeModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CATreeModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CATreeModel))
        return static_cast<void*>(const_cast< CATreeModel*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int CATreeModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = getCurrentPlayingItem(); break;
        case 1: *reinterpret_cast< int*>(_v) = getCurrentFocusItem(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void CATreeModel::closeList()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CATreeModel::currentPlayingItemChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CATreeModel::currentFocusItemChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void CATreeModel::setCurrentPlayingItemPosition()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CATreeModel::signalUpdateCountInfo()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void CATreeModel::listDefaultFocus()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void CATreeModel::updateTextItemView(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CATreeModel::clearSelection()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}
QT_END_MOC_NAMESPACE
