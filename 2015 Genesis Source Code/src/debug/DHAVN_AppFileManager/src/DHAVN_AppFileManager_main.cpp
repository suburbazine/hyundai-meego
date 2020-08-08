#include <QDeclarativeContext>
#include <DHAVN_AppFileManager_Engine.h>
#include <QDebug>
#include <QTextCodec>

#include <QtGui/QDesktopWidget>
#include <QtDeclarative>
// removed by Dmitry 17.08.13

#ifdef QML_BOOSTER_ENABLED
   #include <applauncherd/mdeclarativecache.h>
   #define Q_BOOSTER_EXPORT Q_DECL_EXPORT
#else
   #define Q_BOOSTER_EXPORT
#endif

#include <DHAVN_AppFileManager_AudioListModel.h>
#include <DHAVN_AppFileManager_VideoListModel.h>
#include <DHAVN_AppFileManager_PictureListModel.h>
#include "DHAVN_AppFileManager_CStateManager.h"
#include "DHAVN_AppFileManager_CTrackerExtractorClient.h"
#include <DHAVN_AppFileManager_UI_main.h>

#include <DHAVN_AppFileManager_CommonDef.h>
#include "DHAVN_AppMediaPlayer_EventsEmulator.h"//Added by Alexey Edelev 2012.10.15
#include "../hdr/AppGLWidget.h"

const QString KCountryVariantId = QString("-cv:");

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
//add country variant LANGUAGE_EN_US / LANGUAGE_ES_NA / LANGUAGE_FR_NA
const char *setTranslationRes (LANGUAGE_T langId)
{
    switch (langId)
    {
    case LANGUAGE_EN_US:
    {
        return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_en";
    }
    case LANGUAGE_KO:
    {
        return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_ko";
    }
    case LANGUAGE_ES:
    {
        return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_es-eu";
    }
    case LANGUAGE_FR:
    {
        return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_fr-eu";
    }
    case LANGUAGE_ES_NA:
    {
        return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_es-na";
    }
    case LANGUAGE_FR_NA:
    {
        return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_fr-na";
    }
    case LANGUAGE_AR:
       return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_ar";

    case LANGUAGE_CS:
       return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_cs";

    case LANGUAGE_DA:
       return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_da";

    case LANGUAGE_EN_GB:
       return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_en-gb";

    case LANGUAGE_DE:
       return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_ge";

    case LANGUAGE_IT:
       return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_it";

    case LANGUAGE_NL:
       return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_nl";

    case LANGUAGE_PL:
       return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_pl";

    case LANGUAGE_PT:
       return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_pt";

    case LANGUAGE_RU:
       return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_ru";

    case LANGUAGE_SK:
       return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_sk";

    case LANGUAGE_SV:
       return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_sv";

    case LANGUAGE_TR:
       return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_tr";

    case LANGUAGE_ZH_CN:
       return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_zh-cmn";

    case LANGUAGE_ZH_YUE:
       return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_zh-yue";
    default:
    {
        return "/app/share/AppFileManager/localization/DHAVN_AppFileManager_en";
    }
    }
}

Q_BOOSTER_EXPORT int main(int argc, char *argv[])
{
    QApplication::setGraphicsSystem("raster");

    AppEngineBase::SetAppArgs( argc, argv );
    char **argvcp = AppEngineBase::GetAppArgv();
    int argccp = AppEngineBase::GetAppArgc();

    QApplication     *pApp  = NULL;
// removed by Dmitry 17.08.13

#ifdef QML_BOOSTER_ENABLED
    pApp  = MDeclarativeCache::qApplication( argc, argv );
#else
    QApplication app( argc, argv );
    pApp = &app;

    QDeclarativeView view;
#endif

    QStringList fontList1;
    fontList1 << "NewHDB_CN" << "Amiri" << "DFHeiW5-A"; // modified by Michael.Kim 2014.02.05 for ITS 223717
//modified by aettie.ji for ME font 20131015
    QFont::insertSubstitutions(QString("DH_HDB"), fontList1);

    QStringList fontList2;
    fontList2 << "NewHDR_CN" << "Amiri" << "DFHeiW5-A"; // modified by Michael.Kim 2014.02.05 for ITS 223717
    QFont::insertSubstitutions(QString("DH_HDR"), fontList2);
    //modified by hyejin 2013.09.11 for adding armini font 

    
    LANGUAGE_T languageId = getLanguageId (argccp, argvcp);
    ECountryVariant countryVariant = getCountryVariantId(argccp, argvcp);

    QTranslator translator;
    const char *translationRes = setTranslationRes (languageId);
    translator.load( translationRes );
    pApp->installTranslator(&translator);
    QFont appFont( "DH_HDR" );
    pApp->setFont( appFont );

    qmlRegisterType<CommonDef>("com.filemanager.defines", 1, 0, "CommonDef");
    qmlRegisterType<UIMainDef>("com.filemanager.uicontrol", 1, 0, "UIDef"); // modified by Dmitry 17.08.13

    DECLARE_META_EVENT();
    FileManagerEngine EngineListener( argccp, argvcp, languageId , countryVariant);

    LocalizeTrigger locTrigger;
    QDeclarativeContext *ctxt = EngineListener.GetScreenContext(AppEngineQMLConstants::APP_FRONT_SCREEN);

    // MP_LOG << "AppFileManager  CStatemanager start" << LOG_ENDL;
    CStateManager stateManagerFront(NULL, true, eInvalidState, /*USBController::GetFirstInstance(),*/ &EngineListener, ctxt, &translator);
    // MP_LOG << "AppFileManager  CStatemanager end" << LOG_ENDL;
    UI_Main *uiControl = stateManagerFront.getUIControl();
    EventsEmulator eventSender;//Added by Alexey Edelev 2012.10.15

    //{changed by junam 2013.09.30 for change raster
    //AppGLWidget *glWidget = AppGLWidget::instanceFront();
    QDeclarativeView *view = EngineListener.GetDeclarativeView( AppEngineQMLConstants::APP_FRONT_SCREEN );
    //view->setViewport( glWidget );
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    //glWidget = AppGLWidget::instanceRear();
    view = EngineListener.GetDeclarativeView( AppEngineQMLConstants::APP_REAR_SCREEN );
    //view->setViewport( glWidget );
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //}changed by junam

    ctxt->setContextProperty( "EngineListener", &EngineListener );
    ctxt->setContextProperty( "StateManager", &stateManagerFront );
    ctxt->setContextProperty( "LocTrigger", &locTrigger);
    ctxt->setContextProperty( "UIControl", uiControl );
    ctxt->setContextProperty( "TitleProvider", uiControl->getTitleProvider() );
    ctxt->setContextProperty( "FmMenu", uiControl->getOptionMenu() );
    ctxt->setContextProperty( "EventsEmulator", &eventSender );//Added by Alexey Edelev 2012.10.15

    QDeclarativeContext *ctxt2 = EngineListener.GetScreenContext(AppEngineQMLConstants::APP_REAR_SCREEN);
    CStateManager stateManagerRear(NULL, false, eInvalidState, /*USBController::GetSecondInstance(),*/ &EngineListener, ctxt2, &translator);
    uiControl = stateManagerRear.getUIControl();

    ctxt2->setContextProperty( "EngineListener", &EngineListener );
    ctxt2->setContextProperty( "StateManager", &stateManagerRear );
    ctxt2->setContextProperty( "LocTrigger", &locTrigger);
    ctxt2->setContextProperty( "UIControl", uiControl );
    ctxt2->setContextProperty( "TitleProvider", uiControl->getTitleProvider() );
    ctxt2->setContextProperty( "FmMenu", uiControl->getOptionMenu() );
    ctxt2->setContextProperty( "EventsEmulator", &eventSender );//Added by Alexey Edelev 2012.10.15

    EngineListener.SetSource( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, "/app/share/AppFileManager/qml/DHAVN_AppFileManager_root.qml" ); // modified by Sergey for boot time
    //EngineListener.SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_TranslucentBackground); remove by edo.lee 2013.06.10
    EngineListener.SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_NoSystemBackground );
    EngineListener.SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_OpaquePaintEvent );   
    EngineListener.SetStyleSheet( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, "background:transparent;");//added by edo.lee 2013.06.10
    EngineListener.storeSM(&stateManagerFront, &stateManagerRear); // added by Dmitry 09.10.13

    EngineListener.ConnectToUISH();
    APP_ARGS_PROCESSING( EngineListener );

// modified by Dmitry 17.08.13
    bool ret = pApp->exec();
    return ret;
}
