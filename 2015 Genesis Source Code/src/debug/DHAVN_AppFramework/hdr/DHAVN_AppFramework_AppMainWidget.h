#ifndef DHAVN_APPFRAMEWORK_APPMAINWIDGET_H
#define DHAVN_APPFRAMEWORK_APPMAINWIDGET_H

#include "DHAVN_AppFramework_AppScreenView.h"

class AppMainWidget : public QObject
{
    Q_OBJECT
public:
	/** Constructor */
	   AppMainWidget( bool bEnableOpenGl, bool bEnableHandleTouch = false );
    /** Destructor */
   ~AppMainWidget();

   /**
    * Return qml trees for both screens
    */
   AppScreenView * GetFrontView();
   AppScreenView * GetRearView();

   /**
    * Returns current screen visibility by sreen id
    */
   bool GetSceensUsage(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T);

   /**
    * Realize FG/BG logic for showing qml trees
    */
   void HandleRequestEvents(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nState, bool bVisible, bool bDelay = false, bool bRaise = false, bool bLower = false);

   /**
    * Return context for qml definitions by screen id
    */
   QDeclarativeContext * GetScreenContext(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes);

   /**
    * Load qml files for both screens
    */
   void SetSource( AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes, QString sFrontScreen, QString sRearScreen );

   /**
    * Set top offset usage
    */
   void SetOffset( int nOffset );

   /**
    * Functions to set views properties
    */
   void SetAttribute(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes, Qt::WidgetAttribute attribute, bool on );
   void SetStyleSheet(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes, const QString & styleSheet );

   /**
    * Swap screen view
    */
   void SwapScreenView(bool isRearFGIbox = false);

public slots:
   /**
    * Redraw both qml trees in current states
    */
   void UpdateVisibleScreens();
   void hide();
   void show();

private:   
   /** OpenGL usage*/
   bool  m_bEnableOpenGl;
   /**  Front qml tree visibility */
   bool  m_bShowFrontScreen;
   /**  Rear qml tree visibility */
   bool  m_bShowRearScreen;
   /** qml trees */
   AppScreenView* m_pFrontView;
   AppScreenView* m_pRearView;
   /** pixs number to  skip on top*/
   bool m_nSkipOffset;
   bool m_bEnableHandleTouch;
   bool m_bRaise;
   bool m_bLower;

   bool CheckStartRgsOnFg();
   void CheckScreensLoading(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes);

};

#endif // DHAVN_APPFRAMEWORK_APPMAINWIDGET_H
