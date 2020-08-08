#include <QDeclarativeContext>
#include<QtDeclarative>
#include <QDeclarativeView>
#include <QApplication>
#include <QTranslator>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>
#include <qdeclarativeprivate.h>
#include <QtOpenGL/QGLWidget>

#include "DHAVN_AppAha_Def.h"
#include "DHAVN_AppAhaMenus_def.h"
#include "DHAVN_AppAhaStationList.h"
#include "DHAVN_AppAhaDeviceListInfo.h"
#include "DHAVN_AppAhaBase.h"
#include "DHAVN_AppAhaTrack.h"
#include "DHAVN_AppAha_Def.h"
#include "DHAVN_AppAha_ModelChild.h"
#include "DHAVN_AppAha_QmlProperty.h"

#ifdef FRAMEWORK_ENABLED
#ifdef QML_BOOSTER_ENABLED
   #include <applauncherd/mdeclarativecache.h>
   #define Q_BOOSTER_EXPORT Q_DECL_EXPORT
#else
   #define Q_BOOSTER_EXPORT
#endif
#endif

#ifdef FRAMEWORK_ENABLED
const QString KLanguageId = QString("-l:");
const QString KCountryVariantId = QString("-cv:");
#endif

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
// added by hs.ryu 2013-04-03
const char *setTranslationRes (LANGUAGE_T langId)
{
#ifdef FRAMEWORK_ENABLED
    switch (langId)
    {
    case LANGUAGE_EN_US:    // modified by Ryu for language 20130823
    {
        return "/app/share/AppAhaRadio/localization/AhaRadio_en";
    }
    case LANGUAGE_KO:       // modified by Ryu for language 20130823
    {
        return "/app/share/AppAhaRadio/localization/AhaRadio_ko";
    }
    case LANGUAGE_FR_NA:    // modified by Ryu for language 20130823
    {
        return "/app/share/AppAhaRadio/localization/AhaRadio_fr-na";
    }
    case LANGUAGE_ES_NA:    // modified by Ryu for language 20130823
    {
        return "/app/share/AppAhaRadio/localization/AhaRadio_es-na";
    }
    default:
    {
        return "/app/share/AppAhaRadio/localization/AhaRadio_en";
    }
    }
#else
    Q_UNUSED(langId)
    return NULL;
#endif
}

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
}



CAppAhaBase* CAppAhaBase::SigM = NULL;



int main( int argc, char *argv[] )
{

    int nVehicleType = getVehicleType(argc, argv);

    CAppAhaBase::SetAppArgs(argc, argv);

    QApplication     *pApp  = NULL;
    QDeclarativeView *pView = NULL;

    //QApplication::setGraphicsSystem("opengl");
    QApplication::setGraphicsSystem("raster");

#ifdef QML_BOOSTER_ENABLED
    pApp  = MDeclarativeCache::qApplication( argc, argv );
    pView = MDeclarativeCache::qDeclarativeView();
#else
    QApplication app( argc, argv );
    pApp = &app;

    QDeclarativeView view;
    pView = &view;
#endif




//wsuk.kim 130916 font
//    QFont appFont( "NewHDR" );
//    app.setFont( appFont );
    QStringList fontList1;
    fontList1 << "NewHDB" ;
    QFont::insertSubstitutions(QString("DH_HDB"), fontList1);
    QStringList fontList2;
    fontList2 << "NewHDR";
    QFont::insertSubstitutions(QString("DH_HDR"), fontList2);
//wsuk.kim 130916 font

    ECountryVariant variant = getCountryVariantId(CAppAhaBase::GetAppArgc(), CAppAhaBase::GetAppArgv());
    LANGUAGE_T languageId = getLanguageId (CAppAhaBase::GetAppArgc(), CAppAhaBase::GetAppArgv());
    QTranslator translator;
    const char *translationRes = setTranslationRes (languageId);
    translator.load( translationRes );
    pApp->installTranslator(&translator);

    DECLARE_META_EVENT();
	/** register meta type before creating instance of base . */
    qRegisterMetaType<ERequestType>("ERequestType");
    qRegisterMetaType<EAhaError>("EAhaError");
    qRegisterMetaType<AhaTSTrackInfo>("AhaTSTrackInfo");

    //qmlRegisterType<OptionMenuModel>("OptionMenuModel", 1, 0, "OptionMenuModel");
    qmlRegisterUncreatableType<OptionMenuModel>( "OptionMenuModel", 1, 0, "OptionMenuModel", "OptionMenuModel Can not be created from qml" );
    qmlRegisterType<DeviceInfo>("DeviceInfo", 1, 0, "DeviceInfo");
    qmlRegisterUncreatableType<AhaMenuItems>("AhaMenuItems", 1, 0, "MenuItems","this exports enum");
    CAppAhaBase appAhaBase( *pView, variant, languageId, nVehicleType );
    LocalizeTrigger locTrigger;

    CAppAha_ModelPreset* modelPreset = new CAppAha_ModelPreset();
    CAppAha_ModelLBS*     modelLBS = new CAppAha_ModelLBS();
    CAppAha_ModelContent* modelContent = new CAppAha_ModelContent();
    CAppAhaQmlProperty* qmlProperty = new CAppAhaQmlProperty();

    QDeclarativeContext *ctxt = pView->rootContext();   
    ctxt->setContextProperty("LocTrigger", &locTrigger );
    ctxt->setContextProperty("UIListener", &appAhaBase);
    ctxt->setContextProperty("ahaTrack", CAppAhaTrack::getInstance());
    ctxt->setContextProperty("InTrackInfo", CAppAhaTrack::getInstance()->getTrackInfo());
    ctxt->setContextProperty("ahaStationList", CAppAhaStationList::getInstance());
    ctxt->setContextProperty("modelPreset", modelPreset);
    ctxt->setContextProperty("modelLBS", modelLBS);
    ctxt->setContextProperty("modelContent", modelContent);
    ctxt->setContextProperty("ahaMenus", &appAhaBase.GetAhaMenus());
    ctxt->setContextProperty("qmlProperty", qmlProperty);

    QGLFormat format = QGLFormat::defaultFormat();
    format.setSampleBuffers(true);
    QGLWidget *glWidget = new QGLWidget(format);
    glWidget->setAutoFillBackground(false);
    pView->setViewport(glWidget);

#ifdef __ATTRIBUTE__    //wsuk.kim 13061change_attribute
    pView->setAttribute(Qt::WA_TranslucentBackground);
#else
    pView->setAttribute(Qt::WA_NoSystemBackground);
    pView->setAttribute(Qt::WA_OpaquePaintEvent);
#endif

    pView->setStyleSheet("background:transparent;");
    pView->setSource( QUrl( "/app/share/AppAhaRadio/qml/DHAVN_AhaRadio.qml" ) );

    appAhaBase.ConnectToUISH();
    appAhaBase.RetranslateUi();

    APP_ARGS_PROCESSING( (*pView) );
    return pApp->exec();
}
