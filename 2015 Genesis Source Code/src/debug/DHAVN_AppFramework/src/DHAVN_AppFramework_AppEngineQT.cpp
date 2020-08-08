#include "DHAVN_AppFramework_AppEngineQT_Def.h"

/** AppEngineQTPrivate class definition */
class AppEngineQTPrivate
{

private:

    /** Constructor */
    AppEngineQTPrivate( AppEngineQT *pParent ):q_ptr(pParent){}

       
    /** Destructor */
    virtual ~AppEngineQTPrivate() {}

public:


  //Q-Pointer to the AppEngineQT class
  AppEngineQT * const q_ptr;
  Q_DECLARE_PUBLIC(AppEngineQT)
};

AppEngineQT::AppEngineQT( QWidget *pQAppWidget, QObject *pParent )
: AppEngineBase( pQAppWidget, pParent, APP_QT )
{
    d_ptr = new AppEngineQTPrivate(this);
}

AppEngineQT::AppEngineQT( QWidget *pQAppWidget, QString aModuleName, QObject *pParent )
: AppEngineBase( pQAppWidget, aModuleName, pParent, APP_QT )
{
    d_ptr = new AppEngineQTPrivate(this);
}

AppEngineQT::~AppEngineQT()
{
    delete d_ptr;
}

QWidget *
AppEngineQT::GetCurrentView()
{
   return GetAppView();
}

bool
AppEngineQT::HandleEvent( Event &aEvent )
{
   bool result = true;

   GetLogger().Log( "AppEngineQT::HandleEvent: Start" );

   switch ( aEvent.GetEventId() )
   {
      // This event could be supported on QML applications only, since POPUP plugin based on QML
      case EVT_SHOW_POPUP:
      {
        // TODO:
        break;
      }
      default:
         result = false;
         break;
   }

   return result;
}
void 
AppEngineQT::Reserved_1()
{
}

void 
AppEngineQT::Reserved_2()
{
}

bool
AppEngineQT::HandleEventRear( Event & )
{
    GetLogger().Log( "<--- AppEngineQT::HandleEventRear()" );

    return false;
}

