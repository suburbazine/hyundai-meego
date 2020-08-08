#include <QtCore>
#include <QApplication>
#include <QtDeclarative>
#include <QDeclarativeView>
#include <QDebug>
#include <QDeclarativeContext>
#include <DHAVN_AppUserManual_Log_Def.h>

#include "DHAVN_AppUserManual_Engine.h"
#include "DHAVN_AppUserManual_PDFController.h"
#include "DHAVN_AppUserManual_Def.h"

#define HWTYPE_DH              0
#define HWTYPE_15MY          1
#define HWTYPE_DHPE         2


AppUserManual_Engine* AppUserManual_Engine::SigM = NULL;

const QString KLanguageId = QString("-l:");

LANGUAGE_T getLanguageId (int argccp, char **argvcp)
{
    for (int i=0; i<argccp; i++)
    {
       if( NULL != strstr( argvcp[i], KLanguageId.toUtf8().data() ) )
       {
          return (LANGUAGE_T)atoi( (char *)&argvcp[i][KLanguageId.length()] );
       }
    }
    return LANGUAGE_UNKNOWN;
}

const char* setTranslationResource( LANGUAGE_T langId )
{
    switch( langId )
    {
    case LANGUAGE_KO:
     return "/app/share/AppUserManual/localization/UserManual_ko";
    case LANGUAGE_ES:
       return "/app/share/AppUserManual/localization/UserManual_es-eu";
    case LANGUAGE_ES_NA:
       return "/app/share/AppUserManual/localization/UserManual_es-na";
    case LANGUAGE_FR:
       return "/app/share/AppUserManual/localization/UserManual_fr-eu";
    case LANGUAGE_FR_NA:
       return "/app/share/AppUserManual/localization/UserManual_fr-na";
    case LANGUAGE_AR:
       return "/app/share/AppUserManual/localization/UserManual_ar";
    case LANGUAGE_CS:
       return "/app/share/AppUserManual/localization/UserManual_cs";
    case LANGUAGE_DA:
       return "/app/share/AppUserManual/localization/UserManual_da";
    case LANGUAGE_DE:
       return "/app/share/AppUserManual/localization/UserManual_de";
    case LANGUAGE_IT:
       return "/app/share/AppUserManual/localization/UserManual_it";
    case LANGUAGE_NL:
       return "/app/share/AppUserManual/localization/UserManual_nl";
    case LANGUAGE_PL:
       return "/app/share/AppUserManual/localization/UserManual_pl";
    case LANGUAGE_PT:
       return "/app/share/AppUserManual/localization/UserManual_pt";
    case LANGUAGE_RU:
       return "/app/share/AppUserManual/localization/UserManual_ru";
    case LANGUAGE_SK:
       return "/app/share/AppUserManual/localization/UserManual_sk";
    case LANGUAGE_SV:
       return "/app/share/AppUserManual/localization/UserManual_sv";
    case LANGUAGE_TR:
       return "/app/share/AppUserManual/localization/UserManual_tr";
    case LANGUAGE_ZH_CN:
    case LANGUAGE_ZH_YUE:
       return "/app/share/AppUserManual/localization/UserManual_zh-cmn";
    case LANGUAGE_EN_GB:
       return "/app/share/AppUserManual/localization/UserManual_en-gb";
     case LANGUAGE_EN_US:
      default:
        return "/app/share/AppUserManual/localization/UserManual_en";
    }
}

int main( int argc, char* argv[] )
{
    QTime time;
    time.start();
    QDateTime	dateTime = QDateTime::currentDateTime();
    LW_LOG( "[TIME] main start time :  " << dateTime.toString("hh:mm:ss.zzz") );

//    QApplication :: setGraphicsSystem( "native" );
//    QApplication :: setGraphicsSystem("opengl" );
    dateTime = QDateTime::currentDateTime();
    QApplication :: setGraphicsSystem("raster" );
    dateTime = QDateTime::currentDateTime();
    LW_LOG( "[TIME] after setGraphicsSystem time :  " << dateTime.toString("hh:mm:ss.zzz") );
    dateTime = QDateTime::currentDateTime();
    AppEngineBase :: SetAppArgs( argc, argv );
    QApplication userManual( argc, argv );

    int hwType = -1;

    for ( int i = 0; i < argc; i++)
    {
       QString appParam( argv[i] );

       if( appParam == "-evv:0" ) hwType = HWTYPE_DH;
       else if( appParam == "-evv:1" ) hwType = HWTYPE_15MY;
       else if( appParam == "-evv:2" ) { //case DHPE     // -evv:0  DH       -evv:1  15MY        -evv:2  DHPE
           hwType = HWTYPE_DHPE;
       }
    }
    HS_LOG("-evv: " << hwType );

    char **argvcp = AppEngineBase::GetAppArgv();
    int argccp = AppEngineBase::GetAppArgc();

    LANGUAGE_T languageId = getLanguageId( AppEngineBase::GetAppArgc(), AppEngineBase::GetAppArgv() );
    QTranslator translator;
    const char *translatorResource = setTranslationResource( languageId );
    translator.load( translatorResource );
    userManual.installTranslator( &translator );

    QStringList fontListB;
    fontListB << "NewHDB_CN" << "Amiri" << "DFHeiW5-A";
    QFont::insertSubstitutions(QString("DH_HDB"), fontListB);

    QStringList fontListR;
    fontListR << "NewHDR_CN" << "Amiri" << "DFHeiW5-A";
    QFont::insertSubstitutions(QString("DH_HDR"), fontListR);

    LocalizeTrigger LocTrigger;
    dateTime = QDateTime::currentDateTime();
    LW_LOG( "[TIME] after languageId time :  " << dateTime.toString("hh:mm:ss.zzz") );

//    qmlRegisterType<OptionMenuModel>( "OptionMenuModel", 1, 0, "OptionMenuModel" );
    qmlRegisterUncreatableType<OptionMenuModel>( "OptionMenuModel", 1, 0, "OptionMenuModel", "OptionMenuModel Can not be created from qml" );
    qmlRegisterType<MANUALMENU>( "ManualMenuEnums", 1, 0, "MANUALMENU" );
    qmlRegisterType<AppUserManual_PDFController>("AppUserManual_PDFController", 1, 0, "PdfViewerItem");

    DECLARE_META_EVENT();

    AppUserManual_Engine userManual_Engine( argccp, argvcp, languageId, translator , hwType );
    LW_LOG( "after app engine time.elapsed(): " <<  time.elapsed() );

    QDeclarativeContext* fs_Context = userManual_Engine.GetScreenContext(AppEngineQMLConstants::APP_FRONT_SCREEN);
    fs_Context->setContextProperty("EngineListener", &userManual_Engine);
    fs_Context->setContextProperty("LocTrigger", &LocTrigger);

    /*QDeclarativeContext**/ fs_Context = userManual_Engine.GetScreenContext(AppEngineQMLConstants::APP_REAR_SCREEN);
    fs_Context->setContextProperty("EngineListener", &userManual_Engine);
    fs_Context->setContextProperty("LocTrigger", &LocTrigger);
    dateTime = QDateTime::currentDateTime();
    LW_LOG( "[TIME] after QDeclarativeContext time :  " << dateTime.toString("hh:mm:ss.zzz") );

    userManual_Engine.SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_NoSystemBackground);
    userManual_Engine.SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_OpaquePaintEvent);
    userManual_Engine.SetAttribute( AppEngineQMLConstants::APP_DOUBLE_SCREEN, Qt::WA_TranslucentBackground);
    dateTime = QDateTime::currentDateTime();
    LW_LOG( "[TIME] after SetAttribute time :  " << dateTime.toString("hh:mm:ss.zzz") );

    dateTime = QDateTime::currentDateTime();
    LW_LOG( "[TIME] before setViewportUpdateMode time :  " << dateTime.toString("hh:mm:ss.zzz") );
    QDeclarativeView * front_view = userManual_Engine.GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
    front_view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
//    front_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    QDeclarativeView * rear_view = userManual_Engine.GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
    rear_view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
//    rear_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    dateTime = QDateTime::currentDateTime();
    LW_LOG( "[TIME] after setViewportUpdateMode time :  " << dateTime.toString("hh:mm:ss.zzz") );

//    userManual_Engine.SetStyleSheet( AppEngineQMLConstants::APP_DOUBLE_SCREEN, "background:transparent;");

//    userManual_Engine.SetSource( "/app/share/AppUserManual/qml/DHAVN_AppUserManual_Main.qml", "/app/share/AppUserManual/qml/DHAVN_AppUserManual_Main.qml" );
    userManual_Engine.SetSource(AppEngineQMLConstants::APP_DUPLICATE_SCEENS,  "/app/share/AppUserManual/qml/DHAVN_AppUserManual_Main.qml" );
    dateTime = QDateTime::currentDateTime();
    LW_LOG( "[TIME] after SetSource time :  " << dateTime.toString("hh:mm:ss.zzz") );

    userManual_Engine.RetranslateUI();

    userManual_Engine.ConnectToUISH();
    dateTime = QDateTime::currentDateTime();
    LW_LOG( "[TIME] after ConnectToUISH time :  " << dateTime.toString("hh:mm:ss.zzz") );
    APP_ARGS_PROCESSING( userManual_Engine );

    dateTime = QDateTime::currentDateTime();
    LW_LOG( "[TIME] main exit time :  " << dateTime.toString("hh:mm:ss.zzz") );
    return userManual.exec();
}
