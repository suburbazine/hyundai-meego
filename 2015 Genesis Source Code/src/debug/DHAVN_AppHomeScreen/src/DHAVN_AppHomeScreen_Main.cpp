#include <QDeclarativeContext>
#include <QApplication>
#include <QtGui/QDesktopWidget>
#include <QGLWidget>
#include <QtDeclarative>
#include <QElapsedTimer>

#include <DHAVN_AppHomeScreen_InitData.h>
#include <DHAVN_AppHomeScreen_AppEngine.h>
#include <DHAVN_AppHomeScreen_LocalizeTrigger.h>
#include <DHAVN_AppHomeScreen_AppData.h>

int Command_Field;
QString ViewID;


AppEngine* AppEngine::SigM = NULL;

int main( int argc, char *argv[] )
{
    AppEngineBase::SetAppArgs( argc, argv );

    QApplication app( argc, argv );

    DECLARE_META_EVENT();

    AppData appData;
    InitData initData( &appData );
    LocalizeTrigger LocTrigger( AppEngineBase::GetLanguage() );
    HS_LOG( "GetLanguage = " << AppEngineBase::GetLanguage() );

    AppEngine ApplicationEngine( &initData, &LocTrigger );

    QStringList fontList1;
    fontList1 << "NewHDB_CN" << "Amiri" << "DFHeiW5-A";
    QFont::insertSubstitutions(QString("DH_HDB"), fontList1);

    QStringList fontList2;
    fontList2 << "NewHDR_CN" << "Amiri" << "DFHeiW5-A";
    QFont::insertSubstitutions(QString("DH_HDR"), fontList2);

    QDeclarativeContext *ctxt = ApplicationEngine.GetScreenContext();
    ctxt->setContextProperty( "AppData", &appData );
    ctxt->setContextProperty( "InitData", &initData );
    ctxt->setContextProperty( "EngineListener", &ApplicationEngine );
    ctxt->setContextProperty( "LocTrigger", &LocTrigger );

    QDeclarativeContext *ctxt2 = ApplicationEngine.GetScreenContext(AppEngineQMLConstants::APP_REAR_SCREEN);
    ctxt2->setContextProperty( "AppData", &appData );
    ctxt2->setContextProperty( "InitData", &initData );
    ctxt2->setContextProperty( "EngineListener", &ApplicationEngine );
    ctxt2->setContextProperty( "LocTrigger", &LocTrigger );


    qmlRegisterType<HomeScreenDef>( "QmlHomeScreenDef", 1, 0, "EHSDef" );
    qmlRegisterType<HSDefP>( "QmlHomeScreenDefPrivate", 1, 0, "EHSDefP" );

    ApplicationEngine.SetSource( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, "/app/share/AppHomeMenu/qml/DHAVN_AppHomeScreen_Main.qml" );
    ApplicationEngine.SetAttribute( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, Qt::WA_NoSystemBackground );

    ApplicationEngine.ConnectToUISH();
    APP_ARGS_PROCESSING( ApplicationEngine );
    int result = app.exec();

    return result;
}
