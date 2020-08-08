/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppStandBy_UpdateClock.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppStandBy_UpdateClock.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppStandBy_UpdateClock.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StandByApp[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       4,   94, // properties
       1,  110, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      32,   12,   11,   11, 0x05,
      70,   54,   11,   11, 0x05,
     129,   88,   11,   11, 0x05,
     168,  159,   11,   11, 0x05,
     194,   11,   11,   11, 0x05,
     218,   11,   11,   11, 0x05,
     240,   11,   11,   11, 0x05,
     258,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     285,   11,   11,   11, 0x0a,
     309,  305,   11,   11, 0x0a,
     327,   11,   11,   11, 0x0a,
     339,   11,   11,   11, 0x0a,
     356,   11,   11,   11, 0x0a,
     384,  376,   11,   11, 0x0a,
     437,   11,   11,   11, 0x0a,

 // methods: signature, parameters, type, tag, flags
     452,   11,   11,   11, 0x02,

 // properties: name, type, flags
     474,  466, 0x0a495001,
     487,  483, 0x02495001,
     502,  483, 0x02495001,
     520,  466, 0x0a495001,

 // properties: notify_signal_id
       6,
       4,
       7,
       5,

 // enums: name, flags, count, data
     533, 0x0,    9,  114,

 // enum data: key, value
     552, uint(StandByApp::CLOCK_DATEFORMAT_YYYYMMDD_DASH),
     583, uint(StandByApp::CLOCK_DATEFORMAT_MMDDYYYY_DASH),
     614, uint(StandByApp::CLOCK_DATEFORMAT_YYYYMMDD_SLASH),
     646, uint(StandByApp::CLOCK_DATEFORMAT_MMDDYYYY_SLASH),
     678, uint(StandByApp::CLOCK_DATEFORMAT_YYYYMMDD_DOT),
     708, uint(StandByApp::CLOCK_DATEFORMAT_MMDDYYYY_DOT),
     738, uint(StandByApp::CLOCK_DATEFORMAT_DDMMYYYY_DASH),
     769, uint(StandByApp::CLOCK_DATEFORMAT_DDMMYYYY_SLASH),
     801, uint(StandByApp::CLOCK_DATEFORMAT_DDMMYYYY_DOT),

       0        // eod
};

static const char qt_meta_stringdata_StandByApp[] = {
    "StandByApp\0\0cur_hour,cur_minute\0"
    "dataTimeSync(int,int)\0m_pKeyPathImage\0"
    "setImage(QString)\0"
    "cur_day,cur_dayOfWeek,cur_month,cur_year\0"
    "dataDateSync(int,int,int,int)\0cur_area\0"
    "dataLocationSync(QString)\0"
    "updatedKeyType12hTime()\0updatedKeyPathImage()\0"
    "updatedFileLoad()\0updatedKeyTypeDateFormat()\0"
    "updateTimeAndDate()\0val\0updateTimer(bool)\0"
    "afterBoot()\0setGPSTime(bool)\0"
    "getSystemSettings()\0message\0"
    "updateSystemSettings(EVENT_SETTINGS_CHANGED_DATA_T*)\0"
    "fileLoad(bool)\0TapOnScreen()\0QString\0"
    "fileLoad\0int\0keyType12hTime\0"
    "keyTypeDateFormat\0keyPathImage\0"
    "CLOCK_DATEFORMAT_T\0CLOCK_DATEFORMAT_YYYYMMDD_DASH\0"
    "CLOCK_DATEFORMAT_MMDDYYYY_DASH\0"
    "CLOCK_DATEFORMAT_YYYYMMDD_SLASH\0"
    "CLOCK_DATEFORMAT_MMDDYYYY_SLASH\0"
    "CLOCK_DATEFORMAT_YYYYMMDD_DOT\0"
    "CLOCK_DATEFORMAT_MMDDYYYY_DOT\0"
    "CLOCK_DATEFORMAT_DDMMYYYY_DASH\0"
    "CLOCK_DATEFORMAT_DDMMYYYY_SLASH\0"
    "CLOCK_DATEFORMAT_DDMMYYYY_DOT\0"
};

void StandByApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        StandByApp *_t = static_cast<StandByApp *>(_o);
        switch (_id) {
        case 0: _t->dataTimeSync((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->setImage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->dataDateSync((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 3: _t->dataLocationSync((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->updatedKeyType12hTime(); break;
        case 5: _t->updatedKeyPathImage(); break;
        case 6: _t->updatedFileLoad(); break;
        case 7: _t->updatedKeyTypeDateFormat(); break;
        case 8: _t->updateTimeAndDate(); break;
        case 9: _t->updateTimer((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->afterBoot(); break;
        case 11: _t->setGPSTime((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->getSystemSettings(); break;
        case 13: _t->updateSystemSettings((*reinterpret_cast< EVENT_SETTINGS_CHANGED_DATA_T*(*)>(_a[1]))); break;
        case 14: _t->fileLoad((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->TapOnScreen(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData StandByApp::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject StandByApp::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_StandByApp,
      qt_meta_data_StandByApp, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StandByApp::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StandByApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StandByApp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StandByApp))
        return static_cast<void*>(const_cast< StandByApp*>(this));
    return QObject::qt_metacast(_clname);
}

int StandByApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = getFileLoad(); break;
        case 1: *reinterpret_cast< int*>(_v) = getKeyType12hTime(); break;
        case 2: *reinterpret_cast< int*>(_v) = getKeyTypeDateFormat(); break;
        case 3: *reinterpret_cast< QString*>(_v) = getKeyPathImage(); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void StandByApp::dataTimeSync(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void StandByApp::setImage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void StandByApp::dataDateSync(int _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void StandByApp::dataLocationSync(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void StandByApp::updatedKeyType12hTime()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void StandByApp::updatedKeyPathImage()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void StandByApp::updatedFileLoad()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void StandByApp::updatedKeyTypeDateFormat()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}
QT_END_MOC_NAMESPACE
