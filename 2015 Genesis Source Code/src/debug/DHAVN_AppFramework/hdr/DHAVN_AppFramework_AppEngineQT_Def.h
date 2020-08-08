/**
 * DHAVN_AppFramework_AppEngineQT.h
 *
 * Declaration of the class AppEngineQT
 */

#ifndef APPENGINEQT_H
#define APPENGINEQT_H

#include "DHAVN_AppFramework_AppEngineBase.h"

class AppEngineQTPrivate;
/**
 * Abstract base class for standard widget-based Qt applications.
 */
class APP_FRAMEWORK_EXPORT AppEngineQT : public AppEngineBase
{
   Q_OBJECT

public:
   /** Constructor */
   AppEngineQT( QWidget *pQAppWidget, QObject *pParent = NULL );

   /** Constructor */
   AppEngineQT( QWidget *pQAppWidget, QString aModuleName, QObject *pParent = NULL );

   /** Destructor */
   ~AppEngineQT();

   /**
    * Returns the current application view widget
    */
   QWidget * GetCurrentView();

   /**
    * Applications need to oveerride this and provide their application ID which
    * must be defined in @ref APP_ID_T.
    */
   virtual APP_ID_T GetThisAppId() const = 0;

protected:
   /**
    * Used to handle events. This is used for internal handling and shall not
    * be available for applications.
    */
   virtual bool HandleEvent( Event &aEvent );
   virtual bool HandleEventRear( Event &aEvent );

private:
   // future enhancement
   virtual void Reserved_1();
   virtual void Reserved_2();

private:
    //Private class object for AppEngineQT
    AppEngineQTPrivate * d_ptr;
    Q_DECLARE_PRIVATE(AppEngineQT)
};


#endif // APPENGINEQT_H
