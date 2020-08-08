#include <QDeclarativeContext>
#include <QtGui/QDesktopWidget>
#include <QGLWidget>
#include <QtDeclarative>
#include <QElapsedTimer>

#include <DHAVN_AppIBox_AppEngine.h>
#include "AppGLWidget.h"
#include "DHAVN_AppIBox_LogUtils.h"

const char *setTranslationRes (LANGUAGE_T langId)
{
    switch (langId)
    {
    case LANGUAGE_EN_US:
    case LANGUAGE_EN:
    {
        return "/app/share/AppIBox/localization/AppIBox_en";
    }
    case LANGUAGE_KO:
    {
        return "/app/share/AppIBox/localization/AppIBox_ko";
    }
    case LANGUAGE_ZH_CMN:
    case LANGUAGE_ZH_YUE:
    {
        return "/app/share/AppIBox/localization/AppIBox_zh-cmn";
    }
    case LANGUAGE_PT:
    {
        return "/app/share/AppIBox/localization/AppIBox_pt";
    }
    case LANGUAGE_EN_GB:
    {
        return "/app/share/AppIBox/localization/AppIBox_en-gb";
    }
    case LANGUAGE_FR:
    {
        return "/app/share/AppIBox/localization/AppIBox_fr-eu";
    }
    case LANGUAGE_IT:
    {
        return "/app/share/AppIBox/localization/AppIBox_it";
    }
    case LANGUAGE_DE:
    {
        return "/app/share/AppIBox/localization/AppIBox_ge";
    }
    case LANGUAGE_ES:
    {
        return "/app/share/AppIBox/localization/AppIBox_es-eu";
    }
    case LANGUAGE_RU:
    {
        return "/app/share/AppIBox/localization/AppIBox_ru";
    }
    case LANGUAGE_NL:
    {
        return "/app/share/AppIBox/localization/AppIBox_nl";
    }
    case LANGUAGE_SV:
    {
        return "/app/share/AppIBox/localization/AppIBox_sv";
    }
    case LANGUAGE_PL:
    {
        return "/app/share/AppIBox/localization/AppIBox_pl";
    }
    case LANGUAGE_TR:
    {
        return "/app/share/AppIBox/localization/AppIBox_tr";
    }
    case LANGUAGE_CS:
    {
        return "/app/share/AppIBox/localization/AppIBox_cs";
    }
    case LANGUAGE_DA:
    {
        return "/app/share/AppIBox/localization/AppIBox_da";
    }
    case LANGUAGE_SK:
    {
        return "/app/share/AppIBox/localization/AppIBox_sk";
    }
    case LANGUAGE_AR:
    {
        return "/app/share/AppIBox/localization/AppIBox_ar";
    }
    case LANGUAGE_FR_NA:
    {
        return "/app/share/AppIBox/localization/AppIBox_fr_na";
    }
    case LANGUAGE_ES_NA:
    {
        return "/app/share/AppIBox/localization/AppIBox_es_na";
    }
    default:
    {
        return "/app/share/AppIBox/localization/AppIBox_en";
    }
    }
}

IBoxAppEngine* IBoxAppEngine::SigM = NULL;

int main( int argc, char *argv[] )
{
    LW_LOG("Start loading AppIBox...");
    QApplication::setGraphicsSystem("raster");
    AppEngineBase::SetAppArgs( argc, argv );
    QApplication *pApp = NULL;
    QApplication app( argc, argv );
    pApp = &app;

    LANGUAGE_T languageId = AppEngineBase::GetLanguage();
    QTranslator translator;
    const char *translationRes = setTranslationRes (languageId);
    translator.load( translationRes );
    pApp->installTranslator(&translator);
    LocalizeTrigger locTrigger;

    QStringList fontList1;
    fontList1 << "NewHDB_CN" << "Amiri" << "DFHeiW5-A";
    QFont::insertSubstitutions(QString("DH_HDB"), fontList1);

    QStringList fontList2;
    fontList2 << "NewHDR_CN" << "Amiri" << "DFHeiW5-A";
    QFont::insertSubstitutions(QString("DH_HDR"), fontList2);

    DECLARE_META_EVENT();

    IBoxAppEngine uiListener( &translator, languageId /*&timer*/ );

    QDeclarativeContext *ctxtFront = uiListener.GetScreenContext(AppEngineQMLConstants::APP_FRONT_SCREEN);
    ctxtFront->setContextProperty("EngineListener", &uiListener);
    ctxtFront->setContextProperty("LocTrigger", &locTrigger);
    ctxtFront->setContextProperty("ViewWidgetController", &uiListener);

    QDeclarativeContext *ctxtRear = uiListener.GetScreenContext(AppEngineQMLConstants::APP_REAR_SCREEN);
    ctxtRear->setContextProperty("EngineListener", &uiListener);
    ctxtRear->setContextProperty("LocTrigger", &locTrigger);
    ctxtRear->setContextProperty("ViewWidgetController", &uiListener);

    uiListener.SetSource( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, "/app/share/AppIBox/qml/AppIBox/main.qml" );
    uiListener.SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_TranslucentBackground );
    uiListener.SetStyleSheet( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, "background:transparent;" );
    uiListener.SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_NoSystemBackground );
    uiListener.SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_OpaquePaintEvent );

    uiListener.ConnectToUISH();

    APP_ARGS_PROCESSING( uiListener );

    HS_LOG("Finish loading AppIBox...");

    return pApp->exec();
}
