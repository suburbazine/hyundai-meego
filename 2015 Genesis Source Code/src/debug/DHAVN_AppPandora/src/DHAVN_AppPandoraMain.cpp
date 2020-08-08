#include <QDeclarativeContext>
#include<QtDeclarative>
#include <QDeclarativeView>
#include <QApplication>
#include <QTranslator>
#include <QGLWidget>
#include "DHAVN_AppPandoraBase.h"
#include <DHAVN_AppPandoraMenus_def.h>
#include <DHAVN_AppPandoraSortTypes_def.h>
#include <DHAVN_AppPandoraStationList.h>
#include <DHAVN_AppPandoraDeviceListInfo.h>

#include <QDeclarativeEngine>
#include <QDeclarativeComponent>
#include <qdeclarativeprivate.h>
#include "DHAVN_AppPandora_Def.h"
#include "DHAVN_QMLLogUtil.h"

#ifdef FRAMEWORK_ENABLED
#ifdef QML_BOOSTER_ENABLED
   #include <applauncherd/MDeclarativeCache>
   #define Q_BOOSTER_EXPORT Q_DECL_EXPORT
#else
   #define Q_BOOSTER_EXPORT
#endif
#endif

#ifdef FRAMEWORK_ENABLED
const QString KLanguageId = QString("-l:");
const QString KCountryVariantId = QString("-cv:");
#endif

CAppPandoraBase* CAppPandoraBase::SigM= NULL; // added by esjang 2013.03.18 for Preventing Abnormal termination(SW WDT)

// function to store initial argc and argv value because QApplication call
// removes some parameters from them
void DeleteArgv (char **argv)
{
   for (char** i = argv; *i != NULL; ++i)
   {
       free(*i);
   }
   free(argv);
}
// {  add vehicle type for DH, 16MY, DH PE wonseok.heo 2014.12.29
const QString KVehicleType = QString("-evv:");

int getVehicleType(int argccp, char **argvcp)
{
    for (int i=0; i<argccp; i++)
    {
       if( NULL != strstr( argvcp[i], KVehicleType.toUtf8().data() ) )
       {
          return atoi( (char *)&argvcp[i][KVehicleType.length()] );
       }
    }
    return -1;
}// add vehicle type for DH, 16MY, DH PE wonseok.heo 2014.12.29

char** CopyArgv(int argc, char **argv)
{
   char **argvcp = (char**)malloc((argc + 1) * sizeof (char *));

   for (int i = 0; i < argc; i++)
   {
       int len = strlen(argv[i]);
       // { modified by esjang 2012.11.13 for Static Test CID 13019
       //argvcp[i] = (char*)malloc(sizeof(char*) * (len + 1));
       argvcp[i] = (char*)malloc(sizeof(size_t) * (len + 1));
       // } modified by esjang
       strncpy(argvcp[i], argv[i],len);
   }
   argvcp[argc] = NULL;
   return argvcp;
}


// functions to select application startup language from command line parameter
LANGUAGE_T getLanguageId (int argccp, char **argvcp)
{
#ifdef FRAMEWORK_ENABLED
    for (int i=0; i<argccp; i++)
    {
       if( NULL != strstr( argvcp[i], KLanguageId.toUtf8().data() ) )
       {
          return (LANGUAGE_T)atoi( (char *)&argvcp[i][KLanguageId.length()] );
       }
    }
    return LANGUAGE_UNKNOWN;
#else
    Q_UNUSED(argccp)
    Q_UNUSED(argvcp)
    return 0;
#endif
}

#ifdef FRAMEWORK_ENABLED
ECountryVariant getCountryVariantId (int argccp, char **argvcp)
{
    for (int i=0; i<argccp; i++)
    {
       if( NULL != strstr( argvcp[i], KCountryVariantId.toUtf8().data() ) )
       {
          return (ECountryVariant)atoi( (char *)&argvcp[i][KCountryVariantId.length()] );
       }
    }
    return eCVKorea;
}
#endif

const char *setTranslationRes (LANGUAGE_T langId, int vehicleType)
{
#ifdef FRAMEWORK_ENABLED
    switch (langId)
    {
    case LANGUAGE_EN_US: // modified by esjang 2013.05.28 for Language
    {
        if (vehicleType ==2 ){
            return "/app/share/AppPandora/localization/PandoraRadioPE_en";
        }else{
            return "/app/share/AppPandora/localization/PandoraRadio_en";
        }

    }
    case LANGUAGE_KO:
    {
        if (vehicleType ==2 ){
            return "/app/share/AppPandora/localization/PandoraRadioPE_ko"; // modified by esjang 2013.05.28 for Language
        }else{
            return "/app/share/AppPandora/localization/PandoraRadio_ko"; // modified by esjang 2013.05.28 for Language
        }

    }
    case LANGUAGE_FR_NA: // modified by esjang 2013.07.27 for Language
    {
        if (vehicleType ==2 ){
            return "/app/share/AppPandora/localization/PandoraRadioPE_fr-na"; // modified by esjang 2013.05.28 for Language
        }else{
            return "/app/share/AppPandora/localization/PandoraRadio_fr-na"; // modified by esjang 2013.05.28 for Language
        }
        //though we don't have Chinese translation resources English is loaded

    }
    case LANGUAGE_ES_NA: //modified by esjang 2013.07.27 for Language
    {
        if (vehicleType ==2 ){
            return "/app/share/AppPandora/localization/PandoraRadioPE_es-na"; // modified by esjang 2013.05.28 for Language
        }else{
            return "/app/share/AppPandora/localization/PandoraRadio_es-na"; // modified by esjang 2013.05.28 for Language
        }
        //though we don't have Chinese translation resources English is loaded

    }

    default:
    {
        if (vehicleType ==2 ){
            return "/app/share/AppPandora/localization/PandoraRadioPE_en";
        }else{
            return "/app/share/AppPandora/localization/PandoraRadio_en";
        }

    }
    }
#else
    Q_UNUSED(langId)
    return NULL;
#endif
}

#ifndef FRAMEWORK_ENABLED
int main( int argc, char *argv[] )
{
   char **argvcp = CopyArgv( argc, argv );
   int argccp = argc;

   QApplication::setGraphicsSystem( "opengl" );
   QApplication app( argc, argv );

   // Supporting translation
   LANGUAGE_T languageId = getLanguageId (argccp, argvcp);
   QTranslator translator;
   const char *translationRes = setTranslationRes (languageId);
   translator.load( translationRes ); // modified by esjang 2013.05.28 for Language
   Q_UNUSED(translationRes)

   app.installTranslator(&translator);

   QDeclarativeView view;
  //{ modified by esjang 2013.03.21 for QT Attribute
  //view.viewport()->setAttribute(Qt::WA_TranslucentBackground);
  //view.viewport()->setAttribute(Qt::WA_NoSystemBackground);
  //view.viewport()->setAttribute(Qt::WA_OpaquePaintEvent);

  view.viewport()->setAttribute(AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_NoSystemBackground);
  view.viewport()->setAttribute(AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_OpaquePaintEvent);

  //} modified by esjang 2013.03.21 for QT Attribute
  
  qRegisterMetaType<ERequestType>("ERequestType");
  qRegisterMetaType<EPndrError>("EPndrError");
  qRegisterMetaType<QString>("QString");
  qRegisterMetaType<TSTrackInfo>("TSTrackInfo");

  qmlRegisterType<OptionMenuModel>("OptionMenuModel", 1, 0, "OptionMenuModel");
  qmlRegisterType<PandoraSortTypes>("PandoraSortTypes", 1, 0, "SortType");
  qmlRegisterType<PandoraListReqType>("PandoraListReqType", 1, 0, "ListReqType");
  qmlRegisterType<DeviceInfo>("DeviceInfo", 1, 0, "DeviceInfo");
  //{ modified by yongkyun.lee 2014-02-18 for : 
  qmlRegisterType<POPUPEnums>("POPUPEnums", 1, 0, "PopupIDPnd");
  //} modified by yongkyun.lee 2014-02-18 
  qmlRegisterType<CQMLLogUtil>("CQMLLogUtil", 1, 0, "LogSysID");
  
  qmlRegisterUncreatableType<PandoraMenuItems>("PandoraMenuItems", 1, 0, "MenuItems","this exports enum");

   // Creating application specific class based on Application Framework Lib
  CAppPandoraBase appPandoraBase( view, languageId );

   // Getting QML root context and registering application specific class
   // in QML for invoking C++ methods from QML side
   QDeclarativeContext *ctxt = view.rootContext();
   ctxt->setContextProperty("EngineListener", &appPandoraBase);
   ctxt->setContextProperty("UIListener", &appPandoraBase);
   ctxt->setContextProperty("pndrStationList", &appPandoraBase.GetStationListRef());
   ctxt->setContextProperty("pndrSearch", &appPandoraBase.GetSearchRef());
   ctxt->setContextProperty("pndrTrack", &appPandoraBase.GetTrackRef());
   ctxt->setContextProperty("pandoraMenus", &appPandoraBase.GetPandoraMenus());
   ctxt->setContextProperty("pndrNotifier", &appPandoraBase.GetPandoraNotifier());// added by esjang 2013.04.26 for Clear TP Message
//{ modified by yongkyun.lee 2014-02-18 for : 
   ctxt->setContextProperty("POPUPEnums", &appPandoraBase);
//} modified by yongkyun.lee 2014-02-18 

   //create the logger instance
   CQMLLogUtil logUtil;
   ctxt->setContextProperty("logUtil", &logUtil);

   // TODO need to check this why required. copied from MediaPlayerApp
   LocalizeTrigger locTrigger;
   ctxt->setContextProperty( "LocTrigger", &locTrigger );

   // Registering start QML file. This is entry point for UI
   view.setSource( QUrl( "qml/DHAVN_PandoraRadio.qml" ) );

   view.show();

   int result = app.exec();

   return result;
}

#else
Q_BOOSTER_EXPORT int main( int argc, char *argv[] )
{
    CAppPandoraBase::SetAppArgs(argc, argv);

    QApplication     *pApp  = NULL;
    QDeclarativeView *pView = NULL;

#ifdef QML_BOOSTER_ENABLED
    pApp  = MDeclarativeCache::qApplication( argc, argv );
    pView = MDeclarativeCache::qDeclarativeView();
#else
    QApplication app( argc, argv );
    pApp = &app;

    QDeclarativeView view;
    pView = &view;
#endif

    QApplication::setGraphicsSystem( "raster" ); // set "opengl" for GLES2.X
    QGLWidget *glWidget = new QGLWidget();
    pView->setViewport( glWidget );

    QFont appFont( "NewHDR" );
    app.setFont( appFont );
    QStringList fontList1;
    fontList1 << "NewHDB" ;
    QFont::insertSubstitutions(QString("DH_HDB"), fontList1);
    QStringList fontList2;
    fontList2 << "NewHDR";
    QFont::insertSubstitutions(QString("DH_HDR"), fontList2);

    int nVehicleType = getVehicleType(argc, argv); // add vehicle type for DH, 16MY, DH PE wonseok.heo 2014.12.29

    ECountryVariant variant = getCountryVariantId(CAppPandoraBase::GetAppArgc(), CAppPandoraBase::GetAppArgv());
    LANGUAGE_T languageId = getLanguageId (CAppPandoraBase::GetAppArgc(), CAppPandoraBase::GetAppArgv());
    QTranslator translator;
    const char *translationRes = setTranslationRes (languageId, nVehicleType);
    translator.load( translationRes );
    pApp->installTranslator(&translator);

    DECLARE_META_EVENT();
	/** register meta type before creating instance of base . */
    qRegisterMetaType<ERequestType>("ERequestType");
    qRegisterMetaType<EPndrError>("EPndrError");
    qRegisterMetaType<QString>("QString");
    qRegisterMetaType<TSTrackInfo>("TSTrackInfo");

    //qmlRegisterType<OptionMenuModel>("OptionMenuModel", 1, 0, "OptionMenuModel" , "OptionMenuModel Can not be created from qml");
    qmlRegisterUncreatableType<OptionMenuModel>( "OptionMenuModel", 1, 0, "OptionMenuModel", "OptionMenuModel Can not be created from qml" );


    qmlRegisterType<PandoraSortTypes>("PandoraSortTypes", 1, 0, "SortType");
    qmlRegisterType<PandoraListReqType>("PandoraListReqType", 1, 0, "ListReqType");
    //{ modified by yongkyun.lee 2014-02-18 for : 
    qmlRegisterType<POPUPEnums>("POPUPEnums", 1, 0, "PopupIDPnd");
    //} modified by yongkyun.lee 2014-02-18 
    //qmlRegisterType<DeviceInfo>("DeviceInfo", 1, 0, "DeviceInfo");
    qmlRegisterType<CQMLLogUtil>("CQMLLogUtil", 1, 0, "LogSysID");

    qmlRegisterUncreatableType<PandoraMenuItems>("PandoraMenuItems", 1, 0, "MenuItems","this exports enum");
    CAppPandoraBase appPandoraBase( *pView, variant, languageId ,nVehicleType ); // add vehicle type for DH, 16MY, DH PE wonseok.heo 2014.12.29
    qDebug() << QString( "-----PandoraAfterBase--------%1.%2 secs" ).arg( QTime::currentTime().second() ).arg( QTime::currentTime().msec() );

    LocalizeTrigger locTrigger;

    QDeclarativeContext *ctxt = pView->rootContext();   
    ctxt->setContextProperty( "LocTrigger", &locTrigger );
    ctxt->setContextProperty("EngineListener", &appPandoraBase);
    ctxt->setContextProperty("UIListener", &appPandoraBase);	
    ctxt->setContextProperty("pndrStationList", &appPandoraBase.GetStationListRef());
    ctxt->setContextProperty("pndrSearch", &appPandoraBase.GetSearchRef());
    ctxt->setContextProperty("pndrTrack", &appPandoraBase.GetTrackRef());
    ctxt->setContextProperty("pandoraMenus", &appPandoraBase.GetPandoraMenus());
    ctxt->setContextProperty("pndrNotifier", (QObject*)&appPandoraBase.GetPandoraNotifier());// added by esjang 2013.04.26 for Clear TP Message
    //{ modified by yongkyun.lee 2014-02-18 for : 
    ctxt->setContextProperty("POPUPEnums", &appPandoraBase);
    //} modified by yongkyun.lee 2014-02-18 

//create the logger instance
    CQMLLogUtil logUtil;
    ctxt->setContextProperty("logUtil", &logUtil);

  //{ modified by esjang 2013.08.12 for QT Attribute
#ifndef QT_ATTRIBUTE    
    pView->setAttribute(Qt::WA_TranslucentBackground);
#else
    pView->setAttribute(Qt::WA_NoSystemBackground);
    pView->setAttribute(Qt::WA_OpaquePaintEvent);
#endif
  //} modified by esjang 2013.08.12 for QT Attribute
  
    pView->setStyleSheet("background:transparent;");

#if defined (Q_WS_WIN)
    pView->setSource( QUrl( "qml/DHAVN_PandoraRadio.qml" ) );
#else
    qDebug() << QString( "-----Before--------%1.%2 secs" ).arg( QTime::currentTime().second() ).arg( QTime::currentTime().msec() );
    pView->setSource( QUrl( "/app/share/AppPandora/qml/DHAVN_PandoraRadio.qml" ) );
    qDebug() << QString( "-----After--------%1.%2 secs" ).arg( QTime::currentTime().second() ).arg( QTime::currentTime().msec() );
#endif // Q_WS_WIN

    appPandoraBase.ConnectToUISH();
    appPandoraBase.RetranslateUi();

    APP_ARGS_PROCESSING( (*pView) );

    int result = pApp->exec();

    return result;
}
#endif
