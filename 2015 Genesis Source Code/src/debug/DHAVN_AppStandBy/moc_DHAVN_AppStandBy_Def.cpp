/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppStandBy_Def.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppStandBy_Def.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppStandBy_Def.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SettingsDef[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
      12, 0x0,   23,   18,

 // enum data: key, value
      32, uint(SettingsDef::SETTINGS_LANGUAGE_UNKNOWN),
      58, uint(SettingsDef::SETTINGS_LANGUAGE_EN),
      79, uint(SettingsDef::SETTINGS_LANGUAGE_KO),
     100, uint(SettingsDef::SETTINGS_LANGUAGE_ZH_MA),
     124, uint(SettingsDef::SETTINGS_LANGUAGE_ZH_CN),
     148, uint(SettingsDef::SETTINGS_LANGUAGE_PT),
     169, uint(SettingsDef::SETTINGS_LANGUAGE_EN_UK),
     193, uint(SettingsDef::SETTINGS_LANGUAGE_EN_US),
     217, uint(SettingsDef::SETTINGS_LANGUAGE_FR),
     238, uint(SettingsDef::SETTINGS_LANGUAGE_IT),
     259, uint(SettingsDef::SETTINGS_LANGUAGE_DE),
     280, uint(SettingsDef::SETTINGS_LANGUAGE_ES),
     301, uint(SettingsDef::SETTINGS_LANGUAGE_RU),
     322, uint(SettingsDef::SETTINGS_LANGUAGE_NL),
     343, uint(SettingsDef::SETTINGS_LANGUAGE_SV),
     364, uint(SettingsDef::SETTINGS_LANGUAGE_PL),
     385, uint(SettingsDef::SETTINGS_LANGUAGE_TR),
     406, uint(SettingsDef::SETTINGS_LANGUAGE_CS),
     427, uint(SettingsDef::SETTINGS_LANGUAGE_DA),
     448, uint(SettingsDef::SETTINGS_LANGUAGE_SK),
     469, uint(SettingsDef::SETTINGS_LANGUAGE_AR),
     490, uint(SettingsDef::SETTINGS_LANGUAGE_FR_NA),
     514, uint(SettingsDef::SETTINGS_LANGUAGE_ES_NA),

       0        // eod
};

static const char qt_meta_stringdata_SettingsDef[] = {
    "SettingsDef\0SETTINGS_LANGUAGE_T\0"
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
    "SETTINGS_LANGUAGE_FR_NA\0SETTINGS_LANGUAGE_ES_NA\0"
};

void SettingsDef::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData SettingsDef::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SettingsDef::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SettingsDef,
      qt_meta_data_SettingsDef, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SettingsDef::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SettingsDef::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SettingsDef::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SettingsDef))
        return static_cast<void*>(const_cast< SettingsDef*>(this));
    return QObject::qt_metacast(_clname);
}

int SettingsDef::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
