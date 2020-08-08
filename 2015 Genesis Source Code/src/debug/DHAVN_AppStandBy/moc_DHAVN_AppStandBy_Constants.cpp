/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppStandBy_Constants.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppStandBy_Constants.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppStandBy_Constants.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StandByAppConstants[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       2,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
      20, 0x0,    5,   22,
      36, 0x0,   21,   32,

 // enum data: key, value
      56, uint(StandByAppConstants::BUTTON_AGREE),
      69, uint(StandByAppConstants::BUTTON_LANGUAGE),
      85, uint(StandByAppConstants::BUTTON_USER_GUIDE),
     103, uint(StandByAppConstants::BUTTON_CLOCK),
     116, uint(StandByAppConstants::BUTTON_AGREEMENT),
     133, uint(StandByAppConstants::SETTINGS_LANGUAGE_UNKNOWN),
     159, uint(StandByAppConstants::SETTINGS_LANGUAGE_EN),
     180, uint(StandByAppConstants::SETTINGS_LANGUAGE_KO),
     201, uint(StandByAppConstants::SETTINGS_LANGUAGE_ZH_MA),
     225, uint(StandByAppConstants::SETTINGS_LANGUAGE_ZH_CN),
     249, uint(StandByAppConstants::SETTINGS_LANGUAGE_PT),
     270, uint(StandByAppConstants::SETTINGS_LANGUAGE_EN_UK),
     294, uint(StandByAppConstants::SETTINGS_LANGUAGE_EN_US),
     318, uint(StandByAppConstants::SETTINGS_LANGUAGE_FR),
     339, uint(StandByAppConstants::SETTINGS_LANGUAGE_IT),
     360, uint(StandByAppConstants::SETTINGS_LANGUAGE_DE),
     381, uint(StandByAppConstants::SETTINGS_LANGUAGE_ES),
     402, uint(StandByAppConstants::SETTINGS_LANGUAGE_RU),
     423, uint(StandByAppConstants::SETTINGS_LANGUAGE_NL),
     444, uint(StandByAppConstants::SETTINGS_LANGUAGE_SV),
     465, uint(StandByAppConstants::SETTINGS_LANGUAGE_PL),
     486, uint(StandByAppConstants::SETTINGS_LANGUAGE_TR),
     507, uint(StandByAppConstants::SETTINGS_LANGUAGE_CS),
     528, uint(StandByAppConstants::SETTINGS_LANGUAGE_DA),
     549, uint(StandByAppConstants::SETTINGS_LANGUAGE_SK),
     570, uint(StandByAppConstants::SETTINGS_LANGUAGE_AR),

       0        // eod
};

static const char qt_meta_stringdata_StandByAppConstants[] = {
    "StandByAppConstants\0INT_CONSTANTS_T\0"
    "SETTINGS_LANGUAGE_T\0BUTTON_AGREE\0"
    "BUTTON_LANGUAGE\0BUTTON_USER_GUIDE\0"
    "BUTTON_CLOCK\0BUTTON_AGREEMENT\0"
    "SETTINGS_LANGUAGE_UNKNOWN\0"
    "SETTINGS_LANGUAGE_EN\0SETTINGS_LANGUAGE_KO\0"
    "SETTINGS_LANGUAGE_ZH_MA\0SETTINGS_LANGUAGE_ZH_CN\0"
    "SETTINGS_LANGUAGE_PT\0SETTINGS_LANGUAGE_EN_UK\0"
    "SETTINGS_LANGUAGE_EN_US\0SETTINGS_LANGUAGE_FR\0"
    "SETTINGS_LANGUAGE_IT\0SETTINGS_LANGUAGE_DE\0"
    "SETTINGS_LANGUAGE_ES\0SETTINGS_LANGUAGE_RU\0"
    "SETTINGS_LANGUAGE_NL\0SETTINGS_LANGUAGE_SV\0"
    "SETTINGS_LANGUAGE_PL\0SETTINGS_LANGUAGE_TR\0"
    "SETTINGS_LANGUAGE_CS\0SETTINGS_LANGUAGE_DA\0"
    "SETTINGS_LANGUAGE_SK\0SETTINGS_LANGUAGE_AR\0"
};

void StandByAppConstants::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData StandByAppConstants::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject StandByAppConstants::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_StandByAppConstants,
      qt_meta_data_StandByAppConstants, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StandByAppConstants::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StandByAppConstants::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StandByAppConstants::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StandByAppConstants))
        return static_cast<void*>(const_cast< StandByAppConstants*>(this));
    return QObject::qt_metacast(_clname);
}

int StandByAppConstants::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
