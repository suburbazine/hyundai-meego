#include <QSizePolicy>
#include <QTimer>
#include "DHAVN_AppFramework_AppMainWidget.h"
#include "DHAVN_AppFramework_AppEngineBase.h"

AppMainWidget::AppMainWidget( bool bEnableOpenGl, bool bEnableHandleTouch) :
    m_bEnableOpenGl(bEnableOpenGl),
    m_bShowFrontScreen(false),
    m_bShowRearScreen(false),
    m_pFrontView(NULL),
    m_pRearView(NULL),
    m_nSkipOffset(false),
	m_bEnableHandleTouch( bEnableHandleTouch ),
    m_bRaise(false),
    m_bLower(false)
{
   //setWindowFlags(Qt::Widget | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
   //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

//   m_pFrontView = new AppScreenView(AppEngineQMLConstants::SCREEN_FRONT, m_bEnableOpenGl, NULL);
//   m_pRearView = new AppScreenView(AppEngineQMLConstants::SCREEN_REAR, m_bEnableOpenGl, NULL);

 //  QPalette p(palette());
  // p.setColor(QPalette::Background, Qt::black);
  // setPalette(p);

  // setAttribute(Qt::WA_TranslucentBackground);
  // setStyleSheet("background:transparent;");
}


void AppMainWidget::CheckScreensLoading(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes)
{
   switch( nScreenTypes )
   {
      case AppEngineQMLConstants::APP_FRONT_SCREEN:
         if( NULL == m_pFrontView )
         {
			m_pFrontView = new AppScreenView(AppEngineQMLConstants::SCREEN_FRONT, false, 0, NULL, m_bEnableHandleTouch);
         }
         break;

      case AppEngineQMLConstants::APP_REAR_SCREEN:
         if( NULL == m_pRearView )
         {
			m_pRearView = new AppScreenView(AppEngineQMLConstants::SCREEN_REAR, false, 0, NULL, m_bEnableHandleTouch);
         }
         break;

      case AppEngineQMLConstants::APP_DOUBLE_SCREEN:
      case AppEngineQMLConstants::APP_DUPLICATE_SCEENS:
         if( NULL == m_pFrontView )
         {
			m_pFrontView = new AppScreenView(AppEngineQMLConstants::SCREEN_FRONT, false, m_nSkipOffset, NULL, m_bEnableHandleTouch);
         }
         if( NULL == m_pRearView )
         {
			m_pRearView = new AppScreenView(AppEngineQMLConstants::SCREEN_REAR, false, m_nSkipOffset, NULL, m_bEnableHandleTouch);
         }
         break;

      default:
         break;
   }
}

AppMainWidget::~AppMainWidget()
{
   if( m_pFrontView != NULL )
   {
      delete m_pFrontView;
      m_pFrontView = NULL;
   }
   if( m_pFrontView != NULL )
   {
      delete m_pRearView;
      m_pRearView = NULL;
   }
}

void
AppMainWidget::SetOffset( int nOffset )
{
      m_nSkipOffset = nOffset;
      if( m_pFrontView != NULL )
      {
         m_pFrontView->SetOffset(nOffset);
      }
      if( m_pRearView != NULL )
      {
         m_pRearView->SetOffset(nOffset);
      }
}


AppScreenView *
AppMainWidget::GetFrontView()
{   
   CheckScreensLoading(AppEngineQMLConstants::APP_FRONT_SCREEN);
   return m_pFrontView;
}

AppScreenView *
AppMainWidget::GetRearView()
{
   CheckScreensLoading(AppEngineQMLConstants::APP_REAR_SCREEN);
   return m_pRearView;
}

bool
AppMainWidget::GetSceensUsage(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreen)
{
   bool bScreenAvalible = false;

   switch( nScreen )
   {
      case AppEngineQMLConstants::APP_FRONT_SCREEN:
         bScreenAvalible =m_bShowFrontScreen;
         break;

      case AppEngineQMLConstants::APP_REAR_SCREEN:
         bScreenAvalible = m_bShowRearScreen;
         break;

      case AppEngineQMLConstants::APP_DOUBLE_SCREEN:
      case AppEngineQMLConstants::APP_DUPLICATE_SCEENS:
         bScreenAvalible = m_bShowFrontScreen && m_bShowRearScreen;
         break;

      case AppEngineQMLConstants::APP_ALL_HIDE:
         bScreenAvalible = !( m_bShowFrontScreen || m_bShowRearScreen );
         break;

      default:
         bScreenAvalible = ( m_bShowFrontScreen || m_bShowRearScreen );
         break;
   }
   return bScreenAvalible;
}

void
AppMainWidget::HandleRequestEvents(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nState, bool bVisible, bool bDelay, bool bRaise, bool bLower )
{
   switch( nState )
   {
      case AppEngineQMLConstants::APP_FRONT_SCREEN:
         Logger::Instance()->Log("AppMainWidget::HandleRequestEvents 1");
         m_bShowFrontScreen = bVisible;
         break;

      case AppEngineQMLConstants::APP_REAR_SCREEN:
         Logger::Instance()->Log("AppMainWidget::HandleRequestEvents 2");
         m_bShowRearScreen = bVisible;
         break;

      case AppEngineQMLConstants::APP_DOUBLE_SCREEN:
      case AppEngineQMLConstants::APP_DUPLICATE_SCEENS:
         Logger::Instance()->Log("AppMainWidget::HandleRequestEvents 3");
         m_bShowFrontScreen = bVisible;
         m_bShowRearScreen = bVisible;
         break;

      default:
         Logger::Instance()->Log("AppMainWidget::HandleRequestEvents default");
         m_bShowFrontScreen = false;
         m_bShowRearScreen = false;
         break;
   }

   m_bRaise = bRaise;
   m_bLower = bLower;
   	
   if( bDelay )
   {
      Logger::Instance()->Log("AppMainWidget::delay process");
//      QTimer::singleShot(250, this, SLOT( UpdateVisibleScreens( )));
   QTimer::singleShot(50, this, SLOT( UpdateVisibleScreens( )));
   }
   else
   {
      UpdateVisibleScreens();
   }
}


void AppMainWidget::hide()
{
   if( m_pFrontView != NULL )
   {
      m_pFrontView->hide();
   }
   if( m_pRearView != NULL )
   {
      m_pRearView->hide();
   }
}

void AppMainWidget::show()
{
   UpdateVisibleScreens( );
}

void
AppMainWidget::UpdateVisibleScreens()
{
  Logger::Instance()->Log( QString( "AppMainWidget::UpdateVisibleScreens [ raise =%1 lower =%2 ]" ).arg(m_bRaise).arg(m_bLower) );

  if(  m_bShowRearScreen == true && m_bShowFrontScreen == true )
  {
     CheckScreensLoading(AppEngineQMLConstants::APP_DOUBLE_SCREEN);
     if( m_pRearView != NULL && m_pFrontView != NULL && m_pRearView->IsLoaded() && m_pFrontView->IsLoaded() )
     {
        m_pFrontView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_FRONT );
        m_pFrontView->ChangeSetScreen( AppEngineQMLConstants::SCREEN_FRONT );
        m_pRearView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_REAR );
        m_pRearView->ChangeSetScreen( AppEngineQMLConstants::SCREEN_REAR );

		m_pRearView->repaint();
        m_pFrontView->repaint();
        m_pRearView->show();
        m_pFrontView->show();
#ifndef WINDOW_MANAGER_RAISE
#ifdef RAISE_WINDOW
        if( m_bRaise )
        {
            m_pRearView->raise();
            m_pFrontView->raise();
        }
		else if ( m_bLower )
		{
			m_pRearView->lower();
            m_pFrontView->lower();
		}
#else
		m_pRearView->lower();
		m_pFrontView->lower();
#endif
#endif
     }
     else if( m_pFrontView != NULL && m_pFrontView->IsLoaded() )
     {
        m_pFrontView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_FRONT_EXTENDED );
        m_pFrontView->ChangeSetScreen( AppEngineQMLConstants::SCREEN_FRONT );

		m_pFrontView->repaint();
		m_pFrontView->show();
#ifndef WINDOW_MANAGER_RAISE	
#ifdef RAISE_WINDOW
        if( m_bRaise )
            m_pFrontView->raise();
		else if ( m_bLower )
			m_pFrontView->lower();
#else
		m_pFrontView->lower();
#endif
#endif
     }
   //  repaint();
    // setGeometry( DH_X_FRONT, DH_Y_FRONT - m_nSkipOffset, DH_SCREEN_WIDTH, 2*DH_SCREEN_HEIGHT + DH_STATUS_BAR + m_nSkipOffset );
     Logger::Instance()->Log("AppMainWidget::SetVisibleScreens AppEngineQMLConstants::APP_DUPLICATE_SCEENS");
  }
  else if( m_bShowFrontScreen == true )
  {
     Logger::Instance()->Log("AppMainWidget::SetVisibleScreens AppEngineQMLConstants::APP_FRONT_SCREEN");

     CheckScreensLoading(AppEngineQMLConstants::APP_FRONT_SCREEN);
     if( m_pRearView != NULL && m_pRearView->IsLoaded() && m_bShowRearScreen == false )
     {
         m_pRearView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_INVALIDE );
         m_pRearView->hide();
     }
     if( m_pFrontView != NULL && m_pFrontView->IsLoaded() )
     {
        m_pFrontView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_FRONT );
        m_pFrontView->ChangeSetScreen( AppEngineQMLConstants::SCREEN_FRONT );

		m_pFrontView->repaint();
		m_pFrontView->show();
#ifndef WINDOW_MANAGER_RAISE		
#ifdef RAISE_WINDOW
        if( m_bRaise )
            m_pFrontView->raise();
		else if ( m_bLower )
			m_pFrontView->lower();
#else
		m_pFrontView->lower();	
#endif
#endif
     }
   //  repaint();
    // setGeometry( DH_X_FRONT, DH_Y_FRONT - m_nSkipOffset, DH_SCREEN_WIDTH, DH_SCREEN_HEIGHT + m_nSkipOffset );
  }
  else if( m_bShowRearScreen == true )
  {
     CheckScreensLoading(AppEngineQMLConstants::APP_REAR_SCREEN);
     Logger::Instance()->Log("AppMainWidget::SetVisibleScreens AppEngineQMLConstants::APP_REAR_SCREEN");
     if( m_pRearView != NULL && m_pRearView->IsLoaded() )
     {
        if( m_pFrontView != NULL && m_pFrontView->IsLoaded() && m_bShowFrontScreen == false )
        {
            m_pFrontView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_INVALIDE );
            m_pFrontView->hide();
        }
        m_pRearView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_REAR );
        m_pRearView->ChangeSetScreen( AppEngineQMLConstants::SCREEN_REAR );

		m_pRearView->repaint();
		m_pRearView->show();
#ifndef WINDOW_MANAGER_RAISE
#ifdef RAISE_WINDOW
        if( m_bRaise )
            m_pRearView->raise();
		else if ( m_bLower )
			m_pRearView->lower();
#else
		m_pRearView->lower();
#endif
#endif
     }
     else if( m_pFrontView != NULL && m_pFrontView->IsLoaded() )
     {
        m_pFrontView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_REAR );
        m_pFrontView->ChangeSetScreen( AppEngineQMLConstants::SCREEN_REAR );

		m_pFrontView->repaint();
		m_pFrontView->show();
#ifndef WINDOW_MANAGER_RAISE
#ifdef RAISE_WINDOW		
        if( m_bRaise )
            m_pFrontView->raise();
		else if ( m_bLower )
			m_pFrontView->lower();
#else
		m_pFrontView->lower();
#endif
#endif
     }
   //  repaint();
   //  setGeometry( DH_X_REAR, DH_Y_REAR - m_nSkipOffset, DH_SCREEN_WIDTH, DH_SCREEN_HEIGHT + m_nSkipOffset );
  }
  else
  {
     Logger::Instance()->Log("AppMainWidget::SetVisibleScreens AppEngineQMLConstants::default");
     if( m_pFrontView != NULL && m_pFrontView->IsLoaded() )
     {
        m_pFrontView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_INVALIDE );
        m_pFrontView->hide();
     }
     if( m_pRearView != NULL && m_pRearView->IsLoaded() )
     {
        m_pRearView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_INVALIDE );
        m_pRearView->hide();
     }
  //   repaint();
  //   setGeometry( 0, DH_SCREEN_HEIGHT*2 , 1 ,1);
  }
}

QDeclarativeContext *
AppMainWidget::GetScreenContext(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes)
{
   QDeclarativeContext* pRes = NULL;

   CheckScreensLoading(nScreenTypes);

   if( m_pFrontView == NULL )
   {
      Logger::Instance()->Log("AppMainWidget::GetScreenContext error m_pFrontView");
   }
   if( m_pRearView == NULL )
   {
      Logger::Instance()->Log("AppMainWidget::GetScreenContext error m_pRearView");
   }

   if( nScreenTypes == AppEngineQMLConstants::APP_REAR_SCREEN )
   {
      pRes = m_pRearView->rootContext();
   }
   else
   {
      pRes = m_pFrontView->rootContext();
   }
   return pRes;
}

bool
AppMainWidget::CheckStartRgsOnFg()
{
   char **argvcp = AppEngineBase::GetAppArgv();
   int argccp = AppEngineBase::GetAppArgc();
   bool bResult = false;

   for ( int index = 0; index < argccp; ++index )
   {
       if ( strstr( argvcp[index], "fg" ) != NULL ) // Language
       {
          bResult = true;
          break;
       }
   }
   return bResult;
}

void
AppMainWidget::SetSource( AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes, QString sFrontScreen, QString sRearScreen )
{

   bool  bFgStartArg = CheckStartRgsOnFg();
   CheckScreensLoading(nScreenTypes);
   switch( nScreenTypes )
   {
      case AppEngineQMLConstants::APP_FRONT_SCREEN:
         Logger::Instance()->Log("AppMainWidget::SetSource APP_FRONT_SCREEN");
         m_pFrontView->rootContext()->setContextProperty( "UIListener", m_pFrontView );
         m_pFrontView->SetSource(sFrontScreen);
         if( true == bFgStartArg )
         {
            m_bShowFrontScreen = true;
         }
         break;

      case AppEngineQMLConstants::APP_REAR_SCREEN:
         Logger::Instance()->Log("AppMainWidget::SetSource APP_REAR_SCREEN");
         m_pRearView->rootContext()->setContextProperty( "UIListener", m_pRearView );
         m_pRearView->SetSource(sFrontScreen);
         if( true == bFgStartArg )
         {
            m_bShowRearScreen = true;
         }
         break;

      case AppEngineQMLConstants::APP_DOUBLE_SCREEN:
         Logger::Instance()->Log("AppMainWidget::SetSource APP_DOUBLE_SCREEN");
         m_pRearView->rootContext()->setContextProperty( "UIListener", m_pRearView );
         m_pFrontView->rootContext()->setContextProperty( "UIListener", m_pFrontView );
         m_pFrontView->SetSource(sFrontScreen);
         m_pRearView->SetSource(sRearScreen);
         if( true == bFgStartArg )
         {
            m_bShowFrontScreen = true;
            m_bShowRearScreen = true;
         }
         break;

      case AppEngineQMLConstants::APP_DUPLICATE_SCEENS:
         Logger::Instance()->Log("AppMainWidget::SetSource APP_DUPLICATE_SCEENS");
         m_pRearView->rootContext()->setContextProperty( "UIListener", m_pRearView );
         m_pFrontView->rootContext()->setContextProperty( "UIListener", m_pFrontView );
         m_pFrontView->SetSource(sFrontScreen);
         m_pRearView->SetSource(sFrontScreen);
         if( true == bFgStartArg )
         {
            m_bShowFrontScreen = true;
            m_bShowRearScreen = true;
         }
         break;

      default:
         m_bShowFrontScreen = false;
         m_bShowRearScreen = false;
         break;
   }
   UpdateVisibleScreens();
   hide();
}

/**
 * Functions to set views properties
 */
void
AppMainWidget::SetAttribute(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes, Qt::WidgetAttribute attribute, bool on )
{
   CheckScreensLoading(nScreenTypes);
   switch( nScreenTypes )
   {
      case AppEngineQMLConstants::APP_FRONT_SCREEN:
         Logger::Instance()->Log("AppMainWidget::SetAttribute APP_FRONT_SCREEN");
         m_pFrontView->setAttribute( attribute, on );
         break;

      case AppEngineQMLConstants::APP_REAR_SCREEN:
         Logger::Instance()->Log("AppMainWidget::SetAttribute APP_REAR_SCREEN");
         m_pRearView->setAttribute( attribute, on );
         break;

      case AppEngineQMLConstants::APP_DOUBLE_SCREEN:
      case AppEngineQMLConstants::APP_DUPLICATE_SCEENS:
         Logger::Instance()->Log("AppMainWidget::SetAttribute APP_DOUBLE_SCREEN");
         m_pFrontView->setAttribute( attribute, on );
         m_pRearView->setAttribute( attribute, on );
         break;

      default:
         Logger::Instance()->Log("AppMainWidget::SetAttribute invalide");
         break;
   }
}

void
AppMainWidget::SetStyleSheet(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes, const QString & styleSheet )
{
   CheckScreensLoading(nScreenTypes);
   switch( nScreenTypes )
   {
      case AppEngineQMLConstants::APP_FRONT_SCREEN:
         Logger::Instance()->Log("AppMainWidget::SetAttribute APP_FRONT_SCREEN");
         m_pFrontView->setStyleSheet( styleSheet );
         break;

      case AppEngineQMLConstants::APP_REAR_SCREEN:
         Logger::Instance()->Log("AppMainWidget::SetAttribute APP_REAR_SCREEN");
         m_pRearView->setStyleSheet( styleSheet );
         break;

      case AppEngineQMLConstants::APP_DOUBLE_SCREEN:
      case AppEngineQMLConstants::APP_DUPLICATE_SCEENS:
         Logger::Instance()->Log("AppMainWidget::SetAttribute APP_DOUBLE_SCREEN");
         m_pFrontView->setStyleSheet( styleSheet );
         m_pRearView->setStyleSheet( styleSheet );
         break;

      default:
         Logger::Instance()->Log("AppMainWidget::SetAttribute invalide");
         break;
   }
}

/**
 * Swap screen view
 */
void
AppMainWidget::SwapScreenView(bool isRearFGIbox)
{
	if ( m_pFrontView && m_pRearView )
	{
		// swap object itself
		AppScreenView* pTemp = m_pFrontView;
		m_pFrontView = m_pRearView;
		m_pRearView = pTemp;
		// set properties for new role

		m_pFrontView->ChangeSetScreen( AppEngineQMLConstants::SCREEN_FRONT );
		m_pRearView->ChangeSetScreen( AppEngineQMLConstants::SCREEN_REAR );

		if (  m_bShowRearScreen == true && m_bShowFrontScreen == true )
		{
			Logger::Instance()->Log("AppMainWidget::SwapScreenView AppEngineQMLConstants::APP_DOUBLE_SCREEN");  
			CheckScreensLoading(AppEngineQMLConstants::APP_DOUBLE_SCREEN);
			if ( m_pRearView != NULL && m_pFrontView != NULL && m_pRearView->IsLoaded() && m_pFrontView->IsLoaded() )
			{
				m_pFrontView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_FRONT );
				m_pFrontView->ChangeSetScreen( AppEngineQMLConstants::SCREEN_FRONT );
				m_pRearView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_REAR );
				m_pRearView->ChangeSetScreen( AppEngineQMLConstants::SCREEN_REAR );
				m_pRearView->repaint();
				m_pFrontView->repaint();
			}
			else if ( m_pFrontView != NULL && m_pFrontView->IsLoaded() )
			{
				m_pFrontView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_FRONT_EXTENDED );
				m_pFrontView->ChangeSetScreen( AppEngineQMLConstants::SCREEN_FRONT );
				m_pFrontView->repaint();
			}
		}
		else if ( m_bShowFrontScreen == true )
		{
			Logger::Instance()->Log("AppMainWidget::SwapScreenView AppEngineQMLConstants::APP_FRONT_SCREEN");
			if (isRearFGIbox) 
			{
				Logger::Instance()->Log("AppMainWidget::SwapScreenView AppEngineQMLConstants::APP_FRONT_SCREEN, isRearFGIbox==true");
				CheckScreensLoading(AppEngineQMLConstants::APP_FRONT_SCREEN);
				if( m_pFrontView != NULL && m_pFrontView->IsLoaded() )
				{
					m_pFrontView->show();
					m_pFrontView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_FRONT );
					m_pFrontView->ChangeSetScreen( AppEngineQMLConstants::SCREEN_FRONT );
					m_pFrontView->repaint();
				}
				if( m_pRearView != NULL && m_pRearView->IsLoaded() && m_bShowRearScreen == false )
				{
					m_pRearView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_INVALIDE );
					m_pRearView->hide();
				}
			}
			else
			{
				Logger::Instance()->Log("AppMainWidget::SwapScreenView AppEngineQMLConstants::APP_FRONT_SCREEN, isRearFGIbox==false");
				CheckScreensLoading(AppEngineQMLConstants::APP_FRONT_SCREEN);
				if( m_pRearView != NULL && m_pRearView->IsLoaded() && m_bShowRearScreen == false )
				{
					m_pRearView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_INVALIDE );
					m_pRearView->hide();
				}
				if( m_pFrontView != NULL && m_pFrontView->IsLoaded() )
				{
					m_pFrontView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_FRONT );
					m_pFrontView->ChangeSetScreen( AppEngineQMLConstants::SCREEN_FRONT );
					m_pFrontView->repaint();
				}
			}
		}
		else if ( m_bShowRearScreen == true )
		{
			if (isRearFGIbox) 
			{
				Logger::Instance()->Log("AppMainWidget::SwapScreenView AppEngineQMLConstants::APP_REAR_SCREEN, isRearFGIbox==true");
			CheckScreensLoading(AppEngineQMLConstants::APP_REAR_SCREEN);
				if( m_pRearView != NULL && m_pRearView->IsLoaded() )
				{
					m_pRearView->show();
					m_pRearView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_REAR );
					m_pRearView->ChangeSetScreen( AppEngineQMLConstants::SCREEN_REAR );
					m_pRearView->repaint();
				}
				if( m_pFrontView != NULL && m_pFrontView->IsLoaded() && m_bShowFrontScreen == false )
				{
					m_pFrontView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_INVALIDE );
					m_pFrontView->hide();
				}
			}
			else
			{
				CheckScreensLoading(AppEngineQMLConstants::APP_REAR_SCREEN);
				Logger::Instance()->Log("AppMainWidget::SwapScreenView AppEngineQMLConstants::APP_REAR_SCREEN, isRearFGIbox==false");
				if ( m_pRearView != NULL && m_pRearView->IsLoaded() )
				{
					if ( m_pFrontView != NULL && m_pFrontView->IsLoaded() && m_bShowFrontScreen == false )
					{
						m_pFrontView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_INVALIDE );
						m_pFrontView->hide();
					}
					m_pRearView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_REAR );
					m_pRearView->ChangeSetScreen( AppEngineQMLConstants::SCREEN_REAR );
					m_pRearView->repaint();
				}
				else if ( m_pFrontView != NULL && m_pFrontView->IsLoaded() )
				{
					m_pFrontView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_REAR );
					m_pFrontView->ChangeSetScreen( AppEngineQMLConstants::SCREEN_REAR );
					m_pFrontView->repaint();
				}
			}
		}
		else
		{
			Logger::Instance()->Log("AppMainWidget::SwapScreenView AppEngineQMLConstants::default");
			if ( m_pFrontView != NULL && m_pFrontView->IsLoaded() )
			{
				m_pFrontView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_INVALIDE );
				m_pFrontView->hide();
			}
			if ( m_pRearView != NULL && m_pRearView->IsLoaded() )
			{
				m_pRearView->SetCurrentScreen( AppEngineQMLConstants::SCREEN_INVALIDE );
				m_pRearView->hide();
			}
		}
	}
}

