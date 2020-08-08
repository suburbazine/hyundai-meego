//#include <QGLWidget>
#include <QSizePolicy>
#include <QDeclarativeItem>
#include "DHAVN_AppFramework_AppScreenView.h"

#define REMOVE_CLICK

AppScreenView::AppScreenView(AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T nCurrentScreen, bool bEnableOpenGl, int nYOffset, QWidget *parent, bool bEnableHandleTouch) :
    QDeclarativeView( parent ),
    m_bIsLoaded(false),
    m_bIsDisplayedPopup(false),
    m_nKeyPrev(AppEngineQMLConstants::JOG_NONE),
    m_nKeyStatePrev(AppEngineQMLConstants::KEY_STATUS_NONE),
    m_bRRCPrev(false),
	m_bEnableHandleTouch( bEnableHandleTouch ),
    m_pPopupObj(NULL),
    m_nCurrentScreen(AppEngineQMLConstants::SCREEN_INVALIDE),
    m_nSetScreen(nCurrentScreen),
    m_nYOffset(0)
{
   Logger::Instance()->Log( QString( "AppScreenView create type %1 %2" ).arg(m_nSetScreen).arg(bEnableOpenGl) );

   setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

 /*  if( true == bEnableOpenGl )
   {
      QGLFormat format = QGLFormat::defaultFormat();
      format.setSampleBuffers( true );

      QGLWidget *glWidget = new QGLWidget(format);
      if( glWidget )
      {
         glWidget->setAutoFillBackground(false);
         setViewport(glWidget);  // ownership of glWidget is taken. Comment it for OpenGL disabling
      }
   }
   setParent(parent);*/
   m_nYOffset = nYOffset;
   setWindowFlags(Qt::Widget | Qt::WindowStaysOnTopHint);
   setResizeMode( QDeclarativeView::SizeRootObjectToView );
   UpdateGeometry();
   QPalette p(palette());
   p.setColor(QPalette::Background, Qt::black);
   setPalette(p);
}

AppScreenView::~AppScreenView()
{
   Logger::Instance()->Log( QString( "~AppScreenView create type %1" ).arg(m_nSetScreen) );
   if(m_pPopupObj != NULL)
   {
      delete m_pPopupObj;
   }
}

bool
AppScreenView::IsLoaded()
{
   return m_bIsLoaded;
}


void
AppScreenView::SetOffset(int nOffset)
{
   m_nYOffset = nOffset;
}

void
AppScreenView::SetSource(QString sSource)
{
   setSource(sSource);
   Logger::Instance()->Log( QString( "AppScreenView::SetSource %1" ).arg(sSource.toUtf8().data()) );
   m_bIsLoaded = true;
}

int AppScreenView::getCurrentScreen()
{
   Logger::Instance()->Log( QString( "AppScreenView::getCurrentScreen %1" ).arg(m_nSetScreen) );
   return (int)m_nSetScreen;
}

void AppScreenView::ChangeSetScreen( AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T nSetScreen )
{
   Logger::Instance()->Log( QString( "AppScreenView::ChangeSetScreen %1" ).arg(nSetScreen) );
   m_nSetScreen = nSetScreen;
}

void AppScreenView::SetCurrentScreen( AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T nCurrentScreen )
{
   m_nCurrentScreen = nCurrentScreen;
   UpdateGeometry();
}

void AppScreenView::UpdateGeometry()
{
   Logger::Instance()->Log( QString( "AppScreenView::getCurrentScreen %1 %2" ).arg( m_nCurrentScreen ).arg( m_nSetScreen ) );
   switch( m_nCurrentScreen )
   {

      case AppEngineQMLConstants::SCREEN_FRONT:
         Logger::Instance()->Log( QString( "AppScreenView::UpdateGeometry SCREEN_FRONT screen %1" ).arg( m_nCurrentScreen ) );
         setGeometry( DH_X_FRONT, DH_Y_FRONT + m_nYOffset, DH_SCREEN_WIDTH, DH_SCREEN_HEIGHT - m_nYOffset );
         break;

      case AppEngineQMLConstants::SCREEN_FRONT_EXTENDED:
         Logger::Instance()->Log( QString( "AppScreenView::UpdateGeometry SCREEN_FRONT_EXTENDED screen %1" ).arg(m_nCurrentScreen) );
         setGeometry( DH_X_FRONT, DH_Y_FRONT + m_nYOffset, DH_SCREEN_WIDTH, 2*(DH_SCREEN_HEIGHT + DH_STATUS_BAR) );
         break;

      case AppEngineQMLConstants::SCREEN_REAR:
         Logger::Instance()->Log( QString( "AppScreenView::UpdateGeometry SCREEN_REAR screen %1" ).arg(m_nCurrentScreen) );
         setGeometry( DH_X_REAR, DH_Y_REAR + m_nYOffset, DH_SCREEN_WIDTH, DH_SCREEN_HEIGHT - m_nYOffset );
         break;

      default:
         Logger::Instance()->Log( QString( "AppScreenView::UpdateGeometry default screen %1" ).arg(m_nCurrentScreen) );
         setGeometry( -1, -1, 1,1);
         break;
   }
}

void AppScreenView::HideSystemPopup(bool bPressedButton)
{
   if ( m_pPopupObj )
   {
      emit signalHideSystemPopup();
      QMetaObject::invokeMethod( m_pPopupObj, "hidePopUpInContext", Q_ARG(QVariant, (int)bPressedButton) );
   }
   m_bIsDisplayedPopup = false;
}

void AppScreenView::ShowSystemPopup(Event &aEvent, APP_ID_T nAppID)
{
   if ( m_pPopupObj == NULL )
   {
      QObject *pRootObject = dynamic_cast< QObject * >( rootObject() );
      QDeclarativeItem *rootItem = qobject_cast<QDeclarativeItem *>( pRootObject );
      QDeclarativeComponent component( engine(), QUrl::fromLocalFile( POPUP_QML_PLUGIN_PATH ) );

      m_pPopupObj = component.create();

      QDeclarativeItem *item = qobject_cast<QDeclarativeItem *>( m_pPopupObj );
      item->setParentItem( rootItem );

      connect( m_pPopupObj, SIGNAL( popup_Response( int, QVariant ) ),this, SIGNAL( HandlePopupResponse( int, QVariant ) ) );
   }

   if ( m_pPopupObj )
   {
      CancelKey();
      emit signalShowSystemPopup();
      QMetaObject::invokeMethod( m_pPopupObj, "showPopUpInContext", Q_ARG(QVariant, aEvent.Data()), Q_ARG(QVariant, (int)nAppID) );
   }

   m_bIsDisplayedPopup = true;
}

void
AppScreenView::SendFG()
{
   emit fgReceived();
}

void
AppScreenView::SendBG()
{
   emit bgReceived();
}

void
AppScreenView::CancelKey()
{
	int nRepeat = 1;
    Logger::Instance()->Log("AppScreenView::CancelKey()");
    if( m_nKeyPrev == AppEngineQMLConstants::JOG_CENTER &&
        (m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_PRESSED ||
         m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED ||
         m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_CRITICAL_PRESSED) ) {
        if( m_bIsDisplayedPopup )
           emit signalPopupJogNavigation(m_nKeyPrev, AppEngineQMLConstants::KEY_STATUS_CANCELED, m_bRRCPrev);
        else
           emit signalJogNavigation(m_nKeyPrev, AppEngineQMLConstants::KEY_STATUS_CANCELED, m_bRRCPrev, nRepeat );
        Logger::Instance()->Log( QString( "[KEY_STATE] Cancel key pressed [%1]" ).arg(m_nKeyPrev) );
        m_nKeyPrev = AppEngineQMLConstants::JOG_NONE;
        m_nKeyStatePrev = AppEngineQMLConstants::KEY_STATUS_NONE;
    }
    else if( (m_nKeyPrev >= AppEngineQMLConstants::JOG_UP &&
              m_nKeyPrev <= AppEngineQMLConstants::JOG_BOTTOM_LEFT) &&
             (m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_PRESSED ||
              m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED ||
              m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_CRITICAL_PRESSED) ) {
        if( m_bIsDisplayedPopup )
           emit signalPopupJogNavigation(m_nKeyPrev, AppEngineQMLConstants::KEY_STATUS_CANCELED, m_bRRCPrev);
        else
           emit signalJogNavigation(m_nKeyPrev, AppEngineQMLConstants::KEY_STATUS_CANCELED, m_bRRCPrev, nRepeat);
        Logger::Instance()->Log( QString( "[KEY_STATE] Cancel key pressed [%1]" ).arg(m_nKeyPrev) );
        m_nKeyPrev = AppEngineQMLConstants::JOG_NONE;
        m_nKeyStatePrev = AppEngineQMLConstants::KEY_STATUS_NONE;
    }
}

void
AppScreenView::SendLanguageChanged(int language)
{
    Logger::Instance()->Log("AppScreenView::SendLanguageChanged");
    emit signalLanguageChanged(language);
}

void
AppScreenView::SendJogEvent( AppEngineQMLConstants::EJOG_EVENTS_T key, int keyStatus, bool bRRC )
{
   int nRepeat = keyStatus;

   Logger::Instance()->Log( QString( "AppScreenView::SendJogEvent key=%1 keyStatus=%2 bRRC=%3" ).arg(key).arg(keyStatus).arg(bRRC) );
   Logger::Instance()->Log( QString( "AppScreenView::SendJogEvent m_bIsDisplayedPopup(%1)" ).arg(m_bIsDisplayedPopup) );

   if ( key == AppEngineQMLConstants::JOG_WHEEL_LEFT ||
        key == AppEngineQMLConstants::JOG_WHEEL_RIGHT )
   {
      /*
      for( int index = 1; index <= keyStatus - 1; index++ )
      {
         Logger::Instance()->Log("AppScreenView::SendJogEvent p1" );
         if( m_bIsDisplayedPopup )
            emit signalPopupJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_CLICKED, bRRC );
         else
            emit signalJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_CLICKED, bRRC, nRepeat);
      }
      */
      keyStatus = AppEngineQMLConstants::KEY_STATUS_CLICKED;
   }
   else if ( key == AppEngineQMLConstants::JOG_CENTER )
   {
      switch ( keyStatus )
      {
         case KEY_STATUS_PRESSED:
            keyStatus = AppEngineQMLConstants::KEY_STATUS_PRESSED;
            if( m_bIsDisplayedPopup )
               emit signalPopupJogCenterPressed(bRRC);
            else
               emit signalJogCenterPressed(bRRC);
            break;

         case KEY_STATUS_RELEASED:
            keyStatus = AppEngineQMLConstants::KEY_STATUS_RELEASED;

            if( m_nKeyPrev != key ) {
                Logger::Instance()->Log( "key cancel : release without press " );
                return;
            }

            if ( m_nKeyPrev == key &&
                 m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_PRESSED )
            {
               if( m_bIsDisplayedPopup )
               {
                  emit signalPopupJogCenterClicked(bRRC);
                  emit signalPopupJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_CLICKED, bRRC );
               }
               else
               {
                  emit signalJogCenterClicked(bRRC);
                  emit signalJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_CLICKED, bRRC, nRepeat );
               }
            }

            if( m_bIsDisplayedPopup )
               emit signalPopupJogCenterReleased(bRRC);
            else
               emit signalJogCenterReleased(bRRC);
            break;

         case KEY_STATUS_PRESSED_LONG:
            keyStatus = AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED;
            if( m_bIsDisplayedPopup )
               emit signalPopupJogCenterLongPressed(bRRC);
            else
               emit signalJogCenterLongPressed(bRRC);
            break;

         case KEY_STATUS_PRESSED_CRITICAL:
            keyStatus = AppEngineQMLConstants::KEY_STATUS_CRITICAL_PRESSED;
            if( m_bIsDisplayedPopup )
               emit signalPopupJogCenterCriticalPressed(bRRC);
            else
               emit signalJogCenterCriticalPressed(bRRC);
            break;
         case KEY_STATUS_CANCEL:
            CancelKey();
            break;
         default:
            key = AppEngineQMLConstants::JOG_NONE;
            keyStatus = AppEngineQMLConstants::KEY_STATUS_NONE;
      }
   }
   else
   {
      if ( m_nKeyPrev == key &&
           keyStatus == KEY_STATUS_RELEASED &&
           m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_PRESSED )
      {
         if( m_bIsDisplayedPopup )
            emit signalPopupJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_CLICKED, bRRC );
         else
            emit signalJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_CLICKED, bRRC, nRepeat );
      }      
      keyStatus += AppEngineQMLConstants::KEY_STATUS_PRESSED;
   }

   Logger::Instance()->Log( QString( "AppScreenView::SendJogEvent screen %1" ).arg(m_nSetScreen) );

#ifdef REMOVE_CLICK
   if( (key == AppEngineQMLConstants::JOG_WHEEL_LEFT || key == AppEngineQMLConstants::JOG_WHEEL_RIGHT) &&
        keyStatus == AppEngineQMLConstants::KEY_STATUS_CLICKED ) {
       if( m_bIsDisplayedPopup )
          emit signalPopupJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_PRESSED, bRRC );
       else
          emit signalJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_PRESSED, bRRC, nRepeat );
   }
#endif

   if( m_bIsDisplayedPopup )
      emit signalPopupJogNavigation( key, keyStatus, bRRC );
   else
   {
       if(m_nKeyStatePrev == AppEngineQMLConstants::KEY_STATUS_NONE && keyStatus != AppEngineQMLConstants::KEY_STATUS_PRESSED )
       {
       		if ( (key == AppEngineQMLConstants::JOG_WHEEL_LEFT || key == AppEngineQMLConstants::JOG_WHEEL_RIGHT) == false )
	           return;
       }
       else
			emit signalJogNavigation( key, keyStatus, bRRC, nRepeat );
   }

   m_nKeyPrev = key;
   m_nKeyStatePrev = keyStatus;
   m_bRRCPrev = bRRC;

#ifdef REMOVE_CLICK
   if( (key == AppEngineQMLConstants::JOG_WHEEL_LEFT || key == AppEngineQMLConstants::JOG_WHEEL_RIGHT) &&
        keyStatus == AppEngineQMLConstants::KEY_STATUS_CLICKED ) {
       if( m_bIsDisplayedPopup )
          emit signalPopupJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_RELEASED, bRRC );
       else
          emit signalJogNavigation( key, AppEngineQMLConstants::KEY_STATUS_RELEASED, bRRC, nRepeat );
   }
#endif
}

void AppScreenView::ManualBeep()
{
    emit signalManualBeep();
}

bool  AppScreenView::viewportEvent ( QEvent * event )
{
	switch(event->type())
	{
		case QEvent::TouchBegin:
		case QEvent::TouchUpdate:
		case QEvent::TouchEnd:
		{
			if ( m_bEnableHandleTouch == true )
			{
				emit HandleTouchEvent( event );
				return true; // It means that this event has been handled.
			}
			break;
		}
		break;

		default:
		break;
	}
	return QDeclarativeView::viewportEvent(event);
}

