#include <QtCore>
#include <QDebug>
#include <QDeclarativeContext>
#include <QtGstQmlSink/qmlgstvideoitem.h>

#include <DHAVN_AppFramework_QML_Def.h>

#include "engine/DHAVN_AppMediaPlayer_EngineMain.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"
#include "engine/DHAVN_AppMediaPlayer_EngineAudio.h"
#include "engine/DHAVN_AppMediaPlayer_EnginePhoto.h"
//#include "util/audio/DHAVN_AppMediaPlayer_SingleRowCarousel.h"
//#include "util/audio/DHAVN_AppMediaPlayer_SingleRowItem.h"
//#include "util/audio/DHAVN_AppMediaPlayer_AppGLWidget.h"
//#include "util/video/DHAVN_AppMediaPlayer_TransparencyPainter.h"
#include "util/DHAVN_AppMediaPlayer_EventsEmulator.h"//Added by Alexey Edelev 2012.10.15
#include "util/osd/OSD_Manager.h"

#ifdef AVP_BACKTRACE
#include <signal.h>
#include <execinfo.h>
#include <time.h> // added by Michael.Kim 2014. 07.17 for timer log
#endif

#define MAIN_LOG( logText ) \
{\
    QString sLogString;\
    QTextStream tsLogStream( &sLogString );\
    tsLogStream << __FUNCTION__ << " :: " << logText;\
    Logger::Instance()->Log( sLogString, Logger::PRIORITY_HI );\
}

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

const char *setTranslationRes (LANGUAGE_T langId)
{
   switch (langId)
   {
   case LANGUAGE_EN:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_en";
   case LANGUAGE_KO:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_ko";
   case LANGUAGE_ES:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_es-eu";
   case LANGUAGE_FR:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_fr-eu";
   case LANGUAGE_ZH_CN:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_zh-cmn";
   case LANGUAGE_ZH_YUE:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_zh-yue";
   case LANGUAGE_AR:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_ar";
   case LANGUAGE_IT:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_it";
   case LANGUAGE_RU:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_ru";
   case LANGUAGE_DE:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_ge";
   case LANGUAGE_PT:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_pt";
   case LANGUAGE_EN_GB:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_en-gb";
   case LANGUAGE_PL:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_pl";
   case LANGUAGE_CS:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_cs";
   case LANGUAGE_DA:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_da";
   case LANGUAGE_NL:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_nl";
   case LANGUAGE_SK:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_sk";
   case LANGUAGE_SV:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_sv";
   case LANGUAGE_TR:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_tr";
   case LANGUAGE_ES_NA:
       return "/app/share/AppMediaPlayer/localization/MediaPlayer_es-na";
   case LANGUAGE_FR_NA:
       return "/app/share/AppMediaPlayer/localization/MediaPlayer_fr-na";
   default:
      return "/app/share/AppMediaPlayer/localization/MediaPlayer_en";
   }
}

static void injectMode(int argc, char *argv[], AppMediaPlayer_EngineMain *eng)
{
    APP_ENTRY_POINT_DATA_T evt;
    evt.sAppEntryPoint = eAPP_NONE;
    
    for ( int i=0; i < argc; i++ ) {
        if ( argv[i] == QString("video") ) {
            evt.sAppEntryPoint = eAPP_AVP_VIDEO;
        }

        if ( argv[i] == QString("audio") ) {
            evt.sAppEntryPoint = eAPP_AVP_AUDIO;
        }

        if ( argv[i] == QString("photo") ) {
            evt.sAppEntryPoint = eAPP_AVP_PHOTO;
        }
    }

    if (evt.sAppEntryPoint != eAPP_NONE)
    {
        QByteArray aData( (const char *)&evt, sizeof(APP_ENTRY_POINT_DATA_T) );
        Event fg( EVT_REQUEST_FG, APP_AVP, APP_AVP, aData );
        eng->NotifyFromUIStateManager( fg );
    }
}

// { modified by Sergey for CR#16565
#ifdef AVP_BACKTRACE
void sighandler(int sig)
{
    void *array[50];
    size_t size;
    char **strings;
    size_t i;

    QString fileName = AVP_BACKTRACE_LOG_FILE;
    QFileInfo tFileInfo(fileName);
    FILE *fp;

    if(tFileInfo.size() < 1048576)  // append during filesize below 1MB
        fp = fopen (AVP_BACKTRACE_LOG_FILE, "a+");
    else
        fp = fopen (AVP_BACKTRACE_LOG_FILE, "w+");

    fprintf(fp,"Before AppEngine created AVP Backtrace Log Signal %d\n", sig);
    size = backtrace(array, 50);
    strings = backtrace_symbols(array, size);

    // {added by Michael.Kim 2014. 07.17 for timer log
    struct tm *t;
    time_t timer;
    timer = time(NULL);
    t = localtime(&timer);
    // }added by Michael.Kim 2014. 07.17 for timer log

    for(i = 2; i < size; i++){
        fprintf(fp, "%d: %04d-%02d-%02d %02d:%02d:%02d %s\n", i - 2,  t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, strings[i]);
        char buffer[256];
        char strBuffer[256];
        memset(buffer, 0x0, 256);
        memset(strBuffer, 0x0, 256);
        char *start = strstr(strings[i],"[");
        char *end = strstr(strings[i],"]");
        memcpy(strBuffer,start+1,end - start-1);
        fprintf(fp,"%s\n", strBuffer);
        sprintf(buffer,"addr2line -fe /app/bin/AppMediaPlayer %s\n", strBuffer);
        memset(strBuffer, 0x0, 256);
        FILE *addr2line = popen(buffer, "r");
        if(addr2line == NULL)
        {
            perror("error : ");
        }
        else{
            while(fgets(strBuffer, 256, addr2line)!=NULL)
                fprintf(fp,"%s", strBuffer);
            pclose(addr2line);
        }
    }

    free(strings);
    fclose(fp);
}

#endif
int main( int argc, char *argv[] )
{

#ifdef AVP_BACKTRACE
    signal(SIGABRT, sighandler);
    signal(SIGSEGV, sighandler);
#endif
    
    MAIN_LOG("AVP_START main() start: " << QDateTime::currentDateTime().toString("hh:mm:ss.zzz"));

    QApplication::setGraphicsSystem( "raster" ); // set "opengl" for GLES2.X

    AppEngineBase::SetAppArgs(argc, argv);

    QApplication app( argc, argv );
    gst_init( &argc, &argv );

    QFont appFont( "DH_HDR" );
    app.setFont( appFont );
    //{ modified by wonseok.heo for add amiri font 2013.09.02
    QStringList fontList1;
    fontList1 << "NewHDB_CN" << "Amiri" << "DFHeiW5-A"; // modified by cychoi 2013.12.14 for NewHDB_CN & NewDHR_CN font 
    QFont::insertSubstitutions(QString("DH_HDB"), fontList1);
    QStringList fontList2;
    fontList2 << "NewHDR_CN" << "Amiri" << "DFHeiW5-A"; // modified by cychoi 2013.12.14 for NewHDB_CN & NewDHR_CN font 
    QFont::insertSubstitutions(QString("DH_HDR"), fontList2);
    // } modified by wonseok.heo for add amiri font 2013.09.02

    //Suryanto Tan.
    int nVehicleType = getVehicleType(argc, argv);

    LANGUAGE_T languageId = getLanguageId( AppEngineBase::GetAppArgc(), AppEngineBase::GetAppArgv() );
    QTranslator translator;
    const char *translationRes = setTranslationRes (languageId);
    translator.load( translationRes );
    app.installTranslator(&translator);

    DECLARE_META_EVENT();

    AppMediaPlayer_EngineMain engineListener( languageId, translator, nVehicleType );

    //engineListener.CheckMediaLastMode(); //add by junam 2013.10.02
#ifdef BOOT_LOADING_SCREEN
    engineListener.ConnectAVP2UISH();
#endif
    LocalizeTrigger locTrigger;

    //{changed by junam 2013.09.30 for change raster
    //AppGLWidget *glWidget = AppGLWidget::instanceFront();
    QDeclarativeView *view = engineListener.GetDeclarativeView( AppEngineQMLConstants::APP_FRONT_SCREEN );
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //view->setViewport( glWidget );

    //glWidget = AppGLWidget::instanceRear();
    view = engineListener.GetDeclarativeView( AppEngineQMLConstants::APP_REAR_SCREEN );
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //view->setViewport( glWidget );
    //}changed by junam
    EventsEmulator eventSender;//Added by Alexey Edelev 2012.10.15

    QDeclarativeContext *ctxt = engineListener.GetScreenContext( AppEngineQMLConstants::APP_FRONT_SCREEN );
    ctxt->setContextProperty("EngineListenerMain", &engineListener);
    ctxt->setContextProperty("LocTrigger", &locTrigger);
    ctxt->setContextProperty("EventsEmulator", &eventSender);//Added by Alexey Edelev 2012.10.15
    // added by edo.lee 2013.02.04
    ctxt = engineListener.GetScreenContext( AppEngineQMLConstants::APP_REAR_SCREEN );
    ctxt->setContextProperty("EngineListenerMain", &engineListener);
    ctxt->setContextProperty("LocTrigger", &locTrigger);
    ctxt->setContextProperty("EventsEmulator", &eventSender);//Added by Alexey Edelev 2012.10.15
    //{removed by junam 2013.09.30 for change raster
    //qmlRegisterType<SingleRowCarousel>("carousel", 1, 0, "SingleRowCarouselView");
    //qmlRegisterType<SingleRowItem>("carousel", 1, 0, "SingleRowCarouselItem");
    //qmlRegisterType<TransparencyPainter>("Transparency", 1, 0, "TransparencyPainter");
    //}removed by junam

    engineListener.SetSource( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, "/app/share/AppMediaPlayer/qml/DHAVN_MP_Main.qml" );
    //engineListener.SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_TranslucentBackground); remove by edo.lee  2013.06.10

    // restored by Sergey 22.04.2013 to fix white screen blink when video on rear, photo on front and launch Navi.
    engineListener.SetStyleSheet( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, "background:transparent;"); //removed by junam 2013.04.03 for ISV77703

    // { added by lssanh 2013.02.21 QT attribute
    engineListener.SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_TranslucentBackground ); //changed by junam 2013.09.30 for change raster
    engineListener.SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_OpaquePaintEvent );
    // } added by lssanh 2013.02.21 QT attribute
    // modified by edo.lee  
      
    engineListener.SetAppHandlesResponseFg(false); // modified by Sergey 07.04.2013 // modified by sungha.choi 2013.09.26 for using auto response fg of AFW
#ifdef BOOT_LOADING_SCREEN
    if(argc >= 11)
    {
        QString appParam( argv[10] );
        engineListener.setAVPBootMode(appParam);
    }
#endif
    engineListener.CheckMediaLastMode(); //removed by junam 2013.10.02 //ys-20130607 send EVT_APP_IS_READY_TO_PLAY

    OSDManager::instance(&engineListener);

    APP_ARGS_PROCESSING( engineListener );
    injectMode( argc, argv, &engineListener );

    int result = app.exec();

    return result;
}
// } modified by Sergey for CR#16565
