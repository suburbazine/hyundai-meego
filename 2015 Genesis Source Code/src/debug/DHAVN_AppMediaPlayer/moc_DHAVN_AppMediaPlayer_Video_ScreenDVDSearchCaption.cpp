/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_ScreenDVDSearchCaption.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenDVDSearchCaption.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_ScreenDVDSearchCaption.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ScreenDVDSearchCaption[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   23,   23,   23, 0x0a,
      37,   33,   23,   23, 0x0a,
      58,   54,   23,   23, 0x0a,
      93,   80,   23,   23, 0x0a,
     120,   23,  112,   23, 0x0a,
     137,   23,   23,   23, 0x0a,
     158,   23,   23,   23, 0x0a,
     174,   23,   23,   23, 0x0a,
     196,   23,   23,   23, 0x0a,
     217,   23,   23,   23, 0x0a,
     276,  236,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ScreenDVDSearchCaption[] = {
    "ScreenDVDSearchCaption\0\0onHide()\0arg\0"
    "onShow(QVariant)\0key\0onKeyEntered(QString)\0"
    "languageCode\0onCodeEntered(int)\0QString\0"
    "getCurrentCode()\0refreshCaptionList()\0"
    "onSoftkeyBack()\0setDefaultInputText()\0"
    "deleteAllInputText()\0onBackKeyPressed()\0"
    "srcScreenName,resScreenName,eventId,arg\0"
    "onEventReceived(QString,QString,int,QVariant)\0"
};

void ScreenDVDSearchCaption::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ScreenDVDSearchCaption *_t = static_cast<ScreenDVDSearchCaption *>(_o);
        switch (_id) {
        case 0: _t->onHide(); break;
        case 1: _t->onShow((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 2: _t->onKeyEntered((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->onCodeEntered((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: { QString _r = _t->getCurrentCode();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: _t->refreshCaptionList(); break;
        case 6: _t->onSoftkeyBack(); break;
        case 7: _t->setDefaultInputText(); break;
        case 8: _t->deleteAllInputText(); break;
        case 9: _t->onBackKeyPressed(); break;
        case 10: _t->onEventReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ScreenDVDSearchCaption::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ScreenDVDSearchCaption::staticMetaObject = {
    { &AppMediaPlayer_Video_Screen::staticMetaObject, qt_meta_stringdata_ScreenDVDSearchCaption,
      qt_meta_data_ScreenDVDSearchCaption, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ScreenDVDSearchCaption::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ScreenDVDSearchCaption::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ScreenDVDSearchCaption::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ScreenDVDSearchCaption))
        return static_cast<void*>(const_cast< ScreenDVDSearchCaption*>(this));
    return AppMediaPlayer_Video_Screen::qt_metacast(_clname);
}

int ScreenDVDSearchCaption::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppMediaPlayer_Video_Screen::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
