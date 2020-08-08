#include <QtDeclarative>
#include <QDeclarativeContext>
#include <QApplication>
#include <QtGui/QDesktopWidget>

#ifdef QML_BOOSTER_ENABLED
   #include <applauncherd/MDeclarativeCache>
   #define Q_BOOSTER_EXPORT Q_DECL_EXPORT
#else
   #define Q_BOOSTER_EXPORT
#endif

#include <DHAVN_AppStandBy_AppEngine.h>
#include <uistate_manager_def.h>
#include <DHAVN_AppStandBy_Constants.h>
//#include <DHAVN_LogUtil.h>
#include <QTime>
//#include <DHAVN_AppStandBy_Def.h>

/* Clock */
#include <DHAVN_AppStandBy_UpdateClock.h>
#include "DHAVN_QCANControllerListener.h"

#define HWTYPE_DH               0
#define HWTYPE_15MY             1
#define HWTYPE_17MY             3

const QString KDisplay = QString("-display:0");
const QString KFrontDisplayId = QString(":0.0");
const QString KRearDisplayId = QString(":0.1");

const char * parseLanguageRes ( LANGUAGE_T languageId, ECountryVariant countryVariant )
{
    switch( languageId )
    {
    case LANGUAGE_KO:
        return "/app/share/AppStandBy/localization/StandBy_ko";

    case LANGUAGE_FR:
        return "/app/share/AppStandBy/localization/StandBy_fr-eu";

    case LANGUAGE_FR_NA:
            return "/app/share/AppStandBy/localization/StandBy_fr-na";

    case LANGUAGE_ES:
            return "/app/share/AppStandBy/localization/StandBy_es-eu";

    case LANGUAGE_ES_NA:
            return "/app/share/AppStandBy/localization/StandBy_es-na";

    case LANGUAGE_AR:
        return "/app/share/AppStandBy/localization/StandBy_ar";

    case LANGUAGE_CS:
        return "/app/share/AppStandBy/localization/StandBy_cs";

    case LANGUAGE_DA:
        return "/app/share/AppStandBy/localization/StandBy_da";

    case LANGUAGE_EN_GB:
        return "/app/share/AppStandBy/localization/StandBy_en-gb";

    case LANGUAGE_DE:
        return "/app/share/AppStandBy/localization/StandBy_de";

    case LANGUAGE_IT:
        return "/app/share/AppStandBy/localization/StandBy_it";

    case LANGUAGE_NL:
        return "/app/share/AppStandBy/localization/StandBy_nl";

    case LANGUAGE_PL:
        return "/app/share/AppStandBy/localization/StandBy_pl";

    case LANGUAGE_PT:
        return "/app/share/AppStandBy/localization/StandBy_pt";

    case LANGUAGE_RU:
        return "/app/share/AppStandBy/localization/StandBy_ru";

    case LANGUAGE_SK:
        return "/app/share/AppStandBy/localization/StandBy_sk";

    case LANGUAGE_SV:
        return "/app/share/AppStandBy/localization/StandBy_sv";

    case LANGUAGE_TR:
        return "/app/share/AppStandBy/localization/StandBy_tr";

    case LANGUAGE_ZH_CN:
        return "/app/share/AppStandBy/localization/StandBy_zh-cmn";

//    case LANGUAGE_ZH_YUE:
//        return "/app/share/AppStandBy/localization/StandBy_zh-yue";

    case LANGUAGE_EN_US:
    default:
        return "/app/share/AppStandBy/localization/StandBy_en";
    }
}

//Parse and return the power state argument.
int ParsePstFromArg( int &argc, char** argv )
{
    int iPowerState = 1;
    for ( int index = 0; index < argc; ++index )
    {
        if ( strstr( argv[index], "-pst:" ) != NULL )
        {
           QStringList cvParams = QString( argv[ index ] ).split( ":" );

           if ( cvParams.count() == 2 )
           {
              iPowerState = cvParams.at( 1 ).toUInt();
           }
        }
    }

    return iPowerState;
}

// WDT //
StandByAppEngine* StandByAppEngine::SigM = NULL;

Q_BOOSTER_EXPORT int main( int argc, char *argv[] )
{
    QApplication :: setGraphicsSystem("raster" );
    LW_LOG(  ">>>>>> AppStandBy main start " );
   AppEngineBase::SetAppArgs( argc, argv );
   qmlRegisterType<SettingsDef>("com.settings.variables", 1, 0, "Settings");

   char **argvcp = AppEngineBase::GetAppArgv();
   int argccp = AppEngineBase::GetAppArgc();

   QApplication     *pApp  = NULL;
   QApplication app( argc, argv );
   pApp = &app;
   DECLARE_META_EVENT();

   StandByAppEngine appEngine(  argccp, argvcp);
   StandByApp clockUpdate( &appEngine);
   QCANControllerListener uiQcanControllerListner(&appEngine);

   int hwType = -1;

   for ( int i = 0; i < argc; i++)
   {
      QString appParam( argv[i] );

      if( appParam == "-evv:0" ) hwType = HWTYPE_DH;
      else if( appParam == "-evv:1" ) hwType = HWTYPE_15MY;
      else if( appParam == "-evv:17" ) {
          hwType = HWTYPE_17MY;
      }
   }
   HS_LOG("-evv: " << hwType );
   appEngine.setEvv( hwType );

   ECountryVariant countryVariant = AppEngineBase::GetCountryVariant();
   LANGUAGE_T languageId = AppEngineBase::GetLanguage();
   appEngine.m_pTranslator.load( parseLanguageRes (languageId, countryVariant) );

   pApp->installTranslator( &appEngine.m_pTranslator );

   QStringList fontListB;
   fontListB << "NewHDB_CN" << "Amiri" << "DFHeiW5-A";
   QFont::insertSubstitutions(QString("DH_HDB"), fontListB);

   QStringList fontListR;
   fontListR << "NewHDR_CN" << "Amiri" << "DFHeiW5-A";
   QFont::insertSubstitutions(QString("DH_HDR"), fontListR);

   qmlRegisterUncreatableType< StandByAppConstants >("StandByAppConstants", 1, 0, "CONST", "StandByAppConstants cannot be created from QML context");
   qmlRegisterType<StandByApp>("QmlClockAppClockEnum", 1, 0, "EnumUpdateClock");
   QDeclarativeContext* fs_Context = appEngine.GetScreenContext(AppEngineQMLConstants::APP_FRONT_SCREEN);
   fs_Context->setContextProperty("EngineListener", &appEngine);
   fs_Context->setContextProperty("ClockUpdate", &clockUpdate);
   fs_Context->setContextProperty("LocTrigger", &appEngine);
   fs_Context->setContextProperty("cppToqml", &appEngine.engineToQml);

   /*QDeclarativeContext**/ fs_Context = appEngine.GetScreenContext(AppEngineQMLConstants::APP_REAR_SCREEN);
   fs_Context->setContextProperty("EngineListener", &appEngine);
   fs_Context->setContextProperty("ClockUpdate", &clockUpdate);
   fs_Context->setContextProperty("LocTrigger", &appEngine);
   fs_Context->setContextProperty("cppToqml", &appEngine.engineToQml);

//   appEngine.GetScreenContext()->setContextProperty( "EngineListener", &appEngine );
//   appEngine.GetScreenContext()->setContextProperty( "LocTrigger", &appEngine );

   QDeclarativeView *pView = new QDeclarativeView();
   pView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
   LW_LOG(  ">>>>>> setSource ");
   appEngine.SetSource( AppEngineQMLConstants::APP_DUPLICATE_SCEENS , "/app/share/AppStandBy/qml/DHAVN_AppStandByClock_Main.qml" );
//   appEngine.SetSource( AppEngineQMLConstants::APP_DUPLICATE_SCEENS , "/app/share/AppStandBy/qml/DHAVN_AppStandByClock_Main.qml" );
   LW_LOG(  "<<<<<<<< setSource ");
//   appEngine.SetAttribute( AppEngineQMLConstants::APP_FRONT_SCREEN, Qt::WA_TranslucentBackground );
//   appEngine.SetStyleSheet( AppEngineQMLConstants::APP_FRONT_SCREEN, "background:transparent;" );
   appEngine.SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_NoSystemBackground);
   appEngine.SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_OpaquePaintEvent);


   APP_ARGS_PROCESSING( (appEngine) );
   LW_LOG(  ">>>>>> ConnectToUISH() ");
   appEngine.ConnectToUISH();
   LW_LOG(  "<<<<<<<< ConnectToUISH() ");
   appEngine.getSettingsFromDB();
   appEngine.getSharedMemoryFromUISH();
   //For black screen of PWR OFF in boot time.
   if (ParsePstFromArg(argc, argv)==1) { //DEF_NORMAL_POWER_ON  = 0x01,
        appEngine.setIsPowerStateOn(true);
   }
   else {
       appEngine.setIsPowerStateOn(false);
   }
   appEngine.callFunctionAfterBoot();
   return pApp->exec();
}
