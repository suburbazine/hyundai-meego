#ifndef DHAVN_APPHOMESCREEN_LOCALIZETRIGGER_H
#define DHAVN_APPHOMESCREEN_LOCALIZETRIGGER_H

#include <QObject>
#include <QTranslator>
#include <MGConfItem>
#include <DHAVN_AppFramework_Def.h>
#include <DHAVN_AppHomeScreen_Log_Def.h>

const QString TransRes_EN = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_en";
const QString TransRes_KO = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_ko";
const QString TransRes_ZH_CMN = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_zh-cmn";
const QString TransRes_ZH_YUE = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_zh-cmn";
const QString TransRes_PT = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_pt";
const QString TransRes_EN_GB = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_en-gb";
const QString TransRes_FR = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_fr-eu";
const QString TransRes_IT = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_it";
const QString TransRes_DE = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_ge";
const QString TransRes_ES = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_es-eu";
const QString TransRes_RU = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_ru";
const QString TransRes_NL = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_nl";
const QString TransRes_SV = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_sv";
const QString TransRes_PL = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_pl";
const QString TransRes_TR = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_tr";
const QString TransRes_CS = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_cs";
const QString TransRes_DA = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_da";
const QString TransRes_SK = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_sk";
const QString TransRes_AR = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_ar";
const QString TransRes_FR_NA = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_fr-na";
const QString TransRes_ES_NA = "/app/share/AppHomeMenu/localization/DHAVN_AppHomeScreen_es-na";

class LocalizeTrigger : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString empty READ empty NOTIFY retrigger );
    Q_PROPERTY( bool arab READ arab WRITE setArab NOTIFY arabChanged );

public:
   LocalizeTrigger( LANGUAGE_T nLanguage );
   ~LocalizeTrigger(){};

   void SetLanguage( LANGUAGE_T nLanguage );
   LANGUAGE_T GetLanguage(){ return m_nLanguage; }
   QString Translate( QString sStrId );

signals:
   void retrigger();
   void arabChanged();

private:
   QString empty(){ return QString(); }
   void InitTranslator();
   QString GetTransRes();
   bool arab() { return m_bArab; }
   void setArab (bool flag) { m_bArab = flag; emit arabChanged(); }

private:
   QTranslator *m_pTranslator;
   LANGUAGE_T m_nLanguage;
   bool m_bArab;
};

#endif // DHAVN_APPHOMESCREEN_LOCALIZETRIGGER_H
