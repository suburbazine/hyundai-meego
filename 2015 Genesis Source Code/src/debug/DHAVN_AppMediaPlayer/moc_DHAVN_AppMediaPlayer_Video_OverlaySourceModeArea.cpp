/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppMediaPlayer_Video_OverlaySourceModeArea.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_OverlaySourceModeArea.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppMediaPlayer_Video_OverlaySourceModeArea.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppMediaPlayer_Video_OverlaySourceModeArea[] = {

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
      44,   43,   43,   43, 0x0a,
      57,   53,   43,   43, 0x0a,
      74,   43,   43,   43, 0x0a,
     133,   93,   43,   43, 0x0a,
     179,   43,   43,   43, 0x0a,
     199,   43,   43,   43, 0x0a,
     216,   43,   43,   43, 0x0a,
     234,   43,   43,   43, 0x0a,
     250,   43,   43,   43, 0x0a,
     283,  270,   43,   43, 0x0a,
     311,  303,   43,   43, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AppMediaPlayer_Video_OverlaySourceModeArea[] = {
    "AppMediaPlayer_Video_OverlaySourceModeArea\0"
    "\0onHide()\0arg\0onShow(QVariant)\0"
    "onBackKeyPressed()\0"
    "srcScreenName,resScreenName,eventId,arg\0"
    "onEventReceived(QString,QString,int,QVariant)\0"
    "menuButtonHandler()\0onMousePressed()\0"
    "onMouseReleased()\0onSoftkeyBack()\0"
    "listButtonHandler()\0arrow,status\0"
    "onJogEvent(int,int)\0nNumber\0"
    "onSetFileCountInfo(int)\0"
};

void AppMediaPlayer_Video_OverlaySourceModeArea::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppMediaPlayer_Video_OverlaySourceModeArea *_t = static_cast<AppMediaPlayer_Video_OverlaySourceModeArea *>(_o);
        switch (_id) {
        case 0: _t->onHide(); break;
        case 1: _t->onShow((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 2: _t->onBackKeyPressed(); break;
        case 3: _t->onEventReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 4: _t->menuButtonHandler(); break;
        case 5: _t->onMousePressed(); break;
        case 6: _t->onMouseReleased(); break;
        case 7: _t->onSoftkeyBack(); break;
        case 8: _t->listButtonHandler(); break;
        case 9: _t->onJogEvent((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->onSetFileCountInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppMediaPlayer_Video_OverlaySourceModeArea::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppMediaPlayer_Video_OverlaySourceModeArea::staticMetaObject = {
    { &AppMediaPlayer_Video_Screen::staticMetaObject, qt_meta_stringdata_AppMediaPlayer_Video_OverlaySourceModeArea,
      qt_meta_data_AppMediaPlayer_Video_OverlaySourceModeArea, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppMediaPlayer_Video_OverlaySourceModeArea::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppMediaPlayer_Video_OverlaySourceModeArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppMediaPlayer_Video_OverlaySourceModeArea::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppMediaPlayer_Video_OverlaySourceModeArea))
        return static_cast<void*>(const_cast< AppMediaPlayer_Video_OverlaySourceModeArea*>(this));
    return AppMediaPlayer_Video_Screen::qt_metacast(_clname);
}

int AppMediaPlayer_Video_OverlaySourceModeArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
