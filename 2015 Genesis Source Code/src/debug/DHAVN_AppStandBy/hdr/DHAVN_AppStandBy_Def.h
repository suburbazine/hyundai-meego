#ifndef DHAVN_APPSTANDBY_DEF_H
#define DHAVN_APPSTANDBY_DEF_H

#include <QObject>
#include <uistate_manager_def.h>
#include <DHAVN_AppSettings_Shared.h>


class SettingsDef : public QObject
{
    Q_OBJECT

    Q_ENUMS(
            SETTINGS_LANGUAGE_T
            )

public:

    enum SETTINGS_LANGUAGE_T
    {
        SETTINGS_LANGUAGE_UNKNOWN = LANGUAGE_UNKNOWN,
        SETTINGS_LANGUAGE_EN      = LANGUAGE_EN,     //< English
        SETTINGS_LANGUAGE_KO      = LANGUAGE_KO,     //< Korean
        SETTINGS_LANGUAGE_ZH_MA   = LANGUAGE_ZH_CN,   //< Chinese Mandarin
        SETTINGS_LANGUAGE_ZH_CN, //= LANGUAGE_ZH_YUE, //< Chinese mainland
        SETTINGS_LANGUAGE_PT, //=  LANGUAGE_PT,    //< Europeen Portuguese
        SETTINGS_LANGUAGE_EN_UK, //=  LANGUAGE_EN_UK, //< English (UK)
        SETTINGS_LANGUAGE_EN_US, //=  LANGUAGE_EN_US, //< English (US)
        SETTINGS_LANGUAGE_FR,   //=  LANGUAGE_FR,    //< European French
        SETTINGS_LANGUAGE_IT, //=  LANGUAGE_IT,    //< Italian
        SETTINGS_LANGUAGE_DE, //=  LANGUAGE_DE,    //< German
        SETTINGS_LANGUAGE_ES, //=  LANGUAGE_ES,    //< European Spanish
        SETTINGS_LANGUAGE_RU, //=  LANGUAGE_RU,    //< Russian
        SETTINGS_LANGUAGE_NL, //=  LANGUAGE_NL,    //< Dutch
        SETTINGS_LANGUAGE_SV, //=  LANGUAGE_SV,    //< Swedish
        SETTINGS_LANGUAGE_PL, //=  LANGUAGE_PL,    //< Polish
        SETTINGS_LANGUAGE_TR, //=  LANGUAGE_TR,    //< Turkish
        SETTINGS_LANGUAGE_CS, //=  LANGUAGE_CS,    //< Czech
        SETTINGS_LANGUAGE_DA, //=  LANGUAGE_DA,    //< Danish
        SETTINGS_LANGUAGE_SK, //=  LANGUAGE_SK,    //< Slovakia
        SETTINGS_LANGUAGE_AR, //=  LANGUAGE_AR    //< Arabic
        SETTINGS_LANGUAGE_FR_NA,
        SETTINGS_LANGUAGE_ES_NA
    };
};

#endif // DHAVN_APPSTANDBY_DEF_H
