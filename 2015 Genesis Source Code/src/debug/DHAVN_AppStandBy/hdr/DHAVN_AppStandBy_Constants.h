#ifndef DHAVN_APPSTANDBY_CONSTANTS_H
#define DHAVN_APPSTANDBY_CONSTANTS_H

#include <QObject>
#include <uistate_manager_def.h>
#include <DHAVN_AppSettings_Shared.h>
#define RES_STR_LENGHT 256

class StandByAppConstants : public QObject
{
   Q_OBJECT

   Q_ENUMS( INT_CONSTANTS_T UI_CONSTANTS STRING_CONSTANTS )
   Q_ENUMS( SETTINGS_LANGUAGE_T )

public:
    enum INT_CONSTANTS_T
    {
       BUTTON_AGREE = 0,
        BUTTON_LANGUAGE ,    // AppID Last
        BUTTON_USER_GUIDE,   // AppID user guide
        BUTTON_CLOCK,        // AppID clock
        BUTTON_AGREEMENT    // AppID agreement screen
    };

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
       SETTINGS_LANGUAGE_AR //=  LANGUAGE_AR    //< Arabic

   };

    static QString getStr(int id);
private:
   StandByAppConstants();
};

#endif // DHAVN_APPSTANDBY_CONSTANTS_H
