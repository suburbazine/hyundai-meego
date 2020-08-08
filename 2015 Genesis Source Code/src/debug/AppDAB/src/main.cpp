#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QApplication>
#include <QTranslator>
#include <QtDeclarative>
#include <QHostInfo>
#include <QHostAddress>
#include <QGLWidget>
#include <QtDebug>

#ifdef QML_BOOSTER_ENABLED
#include <applauncherd/MDeclarativeCache>
#define Q_BOOSTER_EXPORT Q_DECL_EXPORT
#else
#define Q_BOOSTER_EXPORT
#endif

#include "DABAppEngine.h"
#include "DABApplication_Def.h"
#include "DABController.h"
#include "DABUIListener.h"
#include "LocTrigger.h"

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

char** CopyArgv(int argc, char **argv)
{
   char **argvcp = (char**)malloc((argc + 1) * sizeof (char *));

   for (int i = 0; i < argc; i++)
   {
       int len = strlen(argv[i]);
       argvcp[i] = (char*)malloc(sizeof(char*) * (len + 1));
//       strcpy(argvcp[i], argv[i]);
       strncpy(argvcp[i], argv[i], len);
   }
   argvcp[argc] = NULL;
   return argvcp;
}

// functions to select application startup language from command line parameter
LANGUAGE_T getLanguageId (int argccp, char **argvcp)
{
    //qDebug() << "argccp:" << argccp;
    for (int i=0; i<argccp; i++)
    {
        //qDebug() << "argvcp[i]:" << argvcp[i] <<", KLanguageId.toUtf8().data():" << KLanguageId.toUtf8().data();
       if( NULL != strstr( argvcp[i], KLanguageId.toUtf8().data() ) )
       {
           //qDebug() << "KLanguageId.length():" << KLanguageId.length();
           //qDebug() << "argvcp[i][KLanguageId.length()] :" << argvcp[i][KLanguageId.length()];
          return (LANGUAGE_T)atoi( (char *)&argvcp[i][KLanguageId.length()] );
       }
    }
    return LANGUAGE_UNKNOWN;
}

const char *setTranslationRes (LANGUAGE_T langId)
{
    switch (langId)
    {        
        case LANGUAGE_EN:
        case LANGUAGE_EN_US:{ return "/app/share/AppDAB/localization/AppDAB_en_us"; }
        case LANGUAGE_EN_GB:{ return "/app/share/AppDAB/localization/AppDAB_en_gb"; }
//        case LANGUAGE_EN:{ return "/app/share/AppDAB/localization/AppDAB_en"; }
        case LANGUAGE_KO:{ return "/app/share/AppDAB/localization/AppDAB_ko"; }
        case LANGUAGE_PT:{ return "/app/share/AppDAB/localization/AppDAB_pt"; } //< Europeen Portuguese
        case LANGUAGE_FR:{ return "/app/share/AppDAB/localization/AppDAB_fr"; } //< European French
        case LANGUAGE_IT:{ return "/app/share/AppDAB/localization/AppDAB_it"; } //< Italian
        case LANGUAGE_DE:{ return "/app/share/AppDAB/localization/AppDAB_de"; } //< German
        case LANGUAGE_ES:{ return "/app/share/AppDAB/localization/AppDAB_es"; } //< European Spanish
        case LANGUAGE_RU:{ return "/app/share/AppDAB/localization/AppDAB_ru"; } //< Russian
        case LANGUAGE_NL:{ return "/app/share/AppDAB/localization/AppDAB_nl"; } //< Dutch
        case LANGUAGE_SV:{ return "/app/share/AppDAB/localization/AppDAB_sv"; } //< Swedish
        case LANGUAGE_PL:{ return "/app/share/AppDAB/localization/AppDAB_pl"; } //< Polish
        case LANGUAGE_TR:{ return "/app/share/AppDAB/localization/AppDAB_tr"; } //< Turkish
        case LANGUAGE_CS:{ return "/app/share/AppDAB/localization/AppDAB_cs"; } //< Czech
        case LANGUAGE_DA:{ return "/app/share/AppDAB/localization/AppDAB_da"; } //< Danish
        case LANGUAGE_SK:{ return "/app/share/AppDAB/localization/AppDAB_sk"; } //< Slovakia
        case LANGUAGE_AR:    //< Arabic
        case LANGUAGE_ZH_CN:  //< Peoples Republic Of China (Mandarin)
        case LANGUAGE_ZH_YUE: //< Peoples Republic Of China (Cantonese)
        default:{ return "/app/share/AppDAB/localization/AppDAB_en"; }
    }
}

void removeLogFile(void)
{
    QFile outFile1(DAB_APP_LOG_FILE);
    outFile1.remove();
    outFile1.close();

    QFile outFile2(DAB_APP_ENGINE_LOG_FILE);
    outFile2.remove();
    outFile2.close();
}

#ifdef __DAB_DEBUG_MSG_OUTPUT_FILE__
void myMessageOutputFile(QtMsgType type, const char *msg)
{
    QString txt;

    if(type == QtWarningMsg || type == QtCriticalMsg)
        return;

    switch (type)
    {
        case QtDebugMsg:
            txt = QString("Debug : %1").arg(msg);
            break;
        case QtWarningMsg:
//            txt = QString("Warning : %1").arg(msg);
            break;

        case QtCriticalMsg:
//            txt = QString("Critical : %1").arg(msg);
            break;

        case QtFatalMsg:
            txt = QString("Fatal : %1").arg(msg);
            abort();
    }

    QFile outFile(DAB_APP_LOG_FILE);
    if(outFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream ts(&outFile);
        ts << QTime::currentTime().toString("hh:mm:ss.zzz") << " :: " << txt << endl;
    }
}
#endif

Q_BOOSTER_EXPORT int main( int argc, char *argv[] )
{
    removeLogFile();

    QTime startTime;
    QTime endTime;
    startTime = QTime::currentTime();
    startTime.start();

    char **argvcp = CopyArgv( argc, argv );
    int argccp = argc;

#ifndef __UNIX_DESKTOP__

#ifdef __RASTER_ENGINE__
    QApplication::setGraphicsSystem("raster");
#else
    QApplication::setGraphicsSystem("opengl");
#endif

#endif
#ifdef QML_BOOSTER_ENABLED
    QApplication *app = NULL;
    app = MDeclarativeCache::qApplication(argc, argv);
#else
    QApplication app( argc, argv );
#endif

    // Supporting translation
    LANGUAGE_T languageId = getLanguageId (argccp, argvcp);
    QTranslator* translator = new QTranslator();    
    const char *translationRes = setTranslationRes (languageId);
//    qDebug() << " TranslationRes:" << translationRes << LOG_ENDL;
    if(!translator->load( translationRes )){
//        qDebug() << " TranslationRes Load Fail" << LOG_ENDL;
    }else{
//        qDebug() << " TranslationRes Load OK" << LOG_ENDL;
    }
#ifdef QML_BOOSTER_ENABLED
    app->installTranslator(translator);
#else
    app.installTranslator(translator);
#endif

#if 0 // Not Used
    QTranslator* statusBarTranslator = new QTranslator();
    const char *statusBarTranslationRes = setStatusBarTranslationRes(languageId);
    qDebug() << " TranslationRes:" << statusBarTranslationRes;
    if(!statusBarTranslator->load( statusBarTranslationRes )){
        qDebug() << " statusBarTranslationRes Load Fail";
    }else{
        qDebug() << " statusBarTranslationRes Load OK";
    }
#ifdef QML_BOOSTER_ENABLED
    app->installTranslator(statusBarTranslator);
#else
    app.installTranslator(statusBarTranslator);
#endif
#endif

#ifdef QML_BOOSTER_ENABLED
    QDeclarativeView *view = NULL;
    view = MDeclarativeCache::qDeclarativeView();
#else
    QDeclarativeView view;
#endif

    DECLARE_META_EVENT();

    QGLFormat format = QGLFormat::defaultFormat();
    format.setSampleBuffers( true );

#ifdef QML_BOOSTER_ENABLED
    QGLWidget *glWidget = new QGLWidget(format);
    if( glWidget )
    {
        glWidget->setAutoFillBackground(false);
        view->setViewport(glWidget);  // ownership of glWidget is taken. Comment it for OpenGL disabling
    }

    DABAppEngine uiListener( *view, languageId );
#else
    #ifdef __RASTER_ENGINE__
    DABAppEngine uiListener( view, languageId );
    view.setAttribute(Qt::WA_NoSystemBackground);
    view.viewport()->setAttribute(Qt::WA_NoSystemBackground);
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    #else
    QGLWidget *glWidget = new QGLWidget(format);
    if( glWidget )
    {
        glWidget->setAutoFillBackground(false);
#ifndef __UNIX_DESKTOP__
        view.setViewport(glWidget);  // ownership of glWidget is taken. Comment it for OpenGL disabling
#endif
    }

    DABAppEngine uiListener( view, languageId );

    view.setAttribute(Qt::WA_NoSystemBackground);
    view.setAttribute(Qt::WA_OpaquePaintEvent);
    #endif
#endif

    uiListener.Log("[DABAppEngine] ConnectToUISH..");
    uiListener.setTranslator(translator);
    uiListener.setInitLanguage(languageId);
#if 0 //Not Used
    uiListener.setStatusBarTranslator(statusBarTranslator);
#endif

#ifndef __UNIX_DESKTOP__
    uiListener.ConnectToUISH( argc, argv);
    uiListener.RetranslateUi();
#endif

#ifdef QML_BOOSTER_ENABLED
    APP_ARGS_PROCESSING( (*view) );
#else
    APP_ARGS_PROCESSING( view );
#endif

    DeleteArgv( argvcp );

    endTime = QTime::currentTime();
    uiListener.Log(QString("[DAB] Start Time: %1").arg(startTime.toString()));
    uiListener.Log(QString("[DAB]   End Time: %1").arg(endTime.toString()));
    uiListener.Log(QString("[DAB] Start Up  : %1 sec").arg((double)startTime.elapsed()/1000));  

#ifdef __DAB_DEBUG_MSG_OUTPUT_FILE__
    qInstallMsgHandler(myMessageOutputFile);
#endif   
//    uiListener.setConnectUISH();

#ifdef QML_BOOSTER_ENABLED
    return app->exec();
#else
    return app.exec();
#endif
}
