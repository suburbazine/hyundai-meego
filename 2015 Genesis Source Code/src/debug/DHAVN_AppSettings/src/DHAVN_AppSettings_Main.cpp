#include <QDeclarativeContext>
#include <QApplication>
#include <QtGui/QDesktopWidget>
#include <QtDeclarative>

#ifdef QML_BOOSTER_ENABLED
#include <../applauncherd/mdeclarativecache.h>
#define Q_BOOSTER_EXPORT Q_DECL_EXPORT
#else
#define Q_BOOSTER_EXPORT
#endif

#include <DHAVN_AppSettings_SaveSettings.h>
#include <DHAVN_AppSettings_SystemInfo.h>

#include <DHAVN_AppSettings_AppEngine.h>
#include <DHAVN_AppSettings_Def.h>
#include "DHAVN_AppSettings_System_JukeBoxInfo.h"
#include <DHAVN_AppSettings_Shared.h>
#include "DHAVN_AppSettings_TimeInfo.h"

#include "DHAVN_AppSettings_TransparencyPainter.h"

#include "DHAVN_QCANControllerListener.h"

//const QString KLanguageId = QString("-l:");
//const QString KCountryVariantId = QString("-cv:");

// Watch Dog
SettingsAppEngine* SettingsAppEngine::SigSettingsAppEngine = NULL;


const char *setTranslationRes (LANGUAGE_T langId)
{
    switch (langId)
    {
    case LANGUAGE_EN:
    case LANGUAGE_EN_US:
    {
        return "/app/share/AppSettings/localization/AppSettings_en";
    }
    case LANGUAGE_KO:
    {
        return "/app/share/AppSettings/localization/AppSettings_ko";
    }
    case LANGUAGE_ZH_YUE:
    case LANGUAGE_ZH_CMN:
    {
        return "/app/share/AppSettings/localization/AppSettings_zh-cmn";
    }
    case LANGUAGE_PT:
    {
        return "/app/share/AppSettings/localization/AppSettings_pt";
    }
    case LANGUAGE_EN_GB:
    {
        return "/app/share/AppSettings/localization/AppSettings_en-gb";
    }
    case LANGUAGE_FR:
    {
        return "/app/share/AppSettings/localization/AppSettings_fr-eu";
    }
    case LANGUAGE_IT:
    {
        return "/app/share/AppSettings/localization/AppSettings_it";
    }
    case LANGUAGE_DE:
    {
        return "/app/share/AppSettings/localization/AppSettings_ge";
    }
    case LANGUAGE_ES:
    {
        return "/app/share/AppSettings/localization/AppSettings_es-eu";
    }
    case LANGUAGE_RU:
    {
        return "/app/share/AppSettings/localization/AppSettings_ru";
    }
    case LANGUAGE_NL:
    {
        return "/app/share/AppSettings/localization/AppSettings_nl";
    }
    case LANGUAGE_SV:
    {
        return "/app/share/AppSettings/localization/AppSettings_sv";
    }
    case LANGUAGE_PL:
    {
        return "/app/share/AppSettings/localization/AppSettings_pl";
    }
    case LANGUAGE_TR:
    {
        return "/app/share/AppSettings/localization/AppSettings_tr";
    }
    case LANGUAGE_CS:
    {
        return "/app/share/AppSettings/localization/AppSettings_cs";
    }
    case LANGUAGE_DA:
    {
        return "/app/share/AppSettings/localization/AppSettings_da";
    }
    case LANGUAGE_SK:
    {
        return "/app/share/AppSettings/localization/AppSettings_sk";
    }
    case LANGUAGE_AR:
    {
        return "/app/share/AppSettings/localization/AppSettings_ar";
    }
    case LANGUAGE_FR_NA:
    {
        return "/app/share/AppSettings/localization/AppSettings_fr-na";
    }
    case LANGUAGE_ES_NA:
    {
        return "/app/share/AppSettings/localization/AppSettings_es-na";
    }
    default:
    {
        return "/app/share/AppSettings/localization/AppSettings_en";
    }
    }
}

bool JukeboxEnable;
int glo_balance; // add for ISV 100925
int glo_fade; // add for ISV 100925
int glo_high; // add for ISV 100925
int glo_mid; // add for ISV 100925
int glo_low; // add for ISV 100925
int glo_front_bright; // add for ISV 100925
int glo_rear_bright; // add for ISV 100925
bool factory = false; // add for its 243706

Q_BOOSTER_EXPORT int main( int argc, char *argv[] )
{
    Logger::Instance()->Log(QString("step 1"), Logger::PRIORITY_HI);

    AppEngineBase::SetAppArgs( argc, argv );

    //Logger::Instance()->Log(QString("step 2"), Logger::PRIORITY_LOW);

    QApplication app( argc, argv );

    //Logger::Instance()->Log(QString("step 3"), Logger::PRIORITY_LOW);

    ECountryVariant variant = AppEngineBase::GetCountryVariant();
    LANGUAGE_T languageId = AppEngineBase::GetLanguage();

    //Logger::Instance()->Log(QString("[AppSettings] main function step 4"), Logger::PRIORITY_LOW);

    QTranslator translator;
    const char *translationRes = setTranslationRes (languageId);
    translator.load( translationRes );
    app.installTranslator(&translator);

    //Logger::Instance()->Log(QString("step 5"), Logger::PRIORITY_LOW);

    DECLARE_META_EVENT();

    QStringList fontList1;
    fontList1 << "NewHDB_CN" << "Amiri" << "DFHeiW5-A";
    QFont::insertSubstitutions(QString("DH_HDB"), fontList1);

    QStringList fontList2;
    fontList2 << "NewHDR_CN" << "Amiri" << "DFHeiW5-A";
    QFont::insertSubstitutions(QString("DH_HDR"), fontList2);

    Logger::Instance()->Log(QString("step 2"), Logger::PRIORITY_HI);

    SettingsAppEngine settingsAppEngine( variant, &translator, languageId);
    SettingsStorage appSaveSettings(&settingsAppEngine);
    SystemInfo systemInfo(&settingsAppEngine, variant);
    AppSettings_System_JukeBoxInfo jukeBoxInfo(&settingsAppEngine);
    TimeInfo timeInfo;

    bool bNaviSdcard = false;
    for ( int i = 0; i < argc; i++)
    {
       QString appParam( argv[i] );

       //Logger::Instance()->Log(QString("argv = %1").arg(appParam), Logger::PRIORITY_HI);

       if ( appParam == "-mmcvalid" )
       {
            bNaviSdcard = true;
       }
    }

    if (bNaviSdcard == false)
    {
        appSaveSettings.removedNaviSDCard();
    }

    QCANControllerListener uiQcanControllerListner(&settingsAppEngine,&appSaveSettings);

    LocalizeTrigger locTrigger;

    qmlRegisterType<SettingsDef>("com.settings.variables", 1, 0, "Settings");
    qmlRegisterType<AppSettingsDef>("com.settings.defines", 1, 0, "SettingsDef");
    qmlRegisterType<SystemInfo>("com.settings.systeminfo", 1, 0, "SystemInfoDef");
    qmlRegisterType<TransparencyPainter>("Transparency", 1, 0, "TransparencyPainter");

    QDeclarativeContext *ctxt = settingsAppEngine.GetScreenContext();
    ctxt->setContextProperty( "EngineListener", &settingsAppEngine );
    ctxt->setContextProperty( "SettingsStorage", &appSaveSettings );
    ctxt->setContextProperty( "SystemInfo", &systemInfo );
    ctxt->setContextProperty( "JukeBoxInfo", &jukeBoxInfo );
    ctxt->setContextProperty( "LocTrigger", &locTrigger);
    ctxt->setContextProperty( "TimeInfo", &timeInfo);

    ctxt = settingsAppEngine.GetScreenContext( AppEngineQMLConstants::APP_REAR_SCREEN );
    ctxt->setContextProperty( "EngineListener", &settingsAppEngine );
    ctxt->setContextProperty( "SettingsStorage", &appSaveSettings );
    ctxt->setContextProperty( "SystemInfo", &systemInfo );
    ctxt->setContextProperty( "JukeBoxInfo", &jukeBoxInfo );
    ctxt->setContextProperty( "LocTrigger", &locTrigger);
    ctxt->setContextProperty( "TimeInfo", &timeInfo);

    settingsAppEngine.SetSource( AppEngineQMLConstants::APP_DUPLICATE_SCEENS,
                                 "/app/share/AppSettings/qml/DHAVN_AppSettings_Main.qml" );

    settingsAppEngine.SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_TranslucentBackground );
    settingsAppEngine.SetStyleSheet( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, "background:transparent;" );

    settingsAppEngine.ConnectToUISH();

    appSaveSettings.checkResetComplete();

    //if ( settingsAppEngine.getGPSTimeSetting() == true )
    //{
    //  settingsAppEngine.requestGPSTimeToNavi();
    //}

    appSaveSettings.checkDBAndDelete(); //added for DB write Fail Issue

    APP_ARGS_PROCESSING( settingsAppEngine );
    Logger::Instance()->Log(QString("step 3"), Logger::PRIORITY_HI);

    return app.exec();
}
