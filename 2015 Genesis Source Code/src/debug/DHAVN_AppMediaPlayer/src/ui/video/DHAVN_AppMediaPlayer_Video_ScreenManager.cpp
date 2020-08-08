#include <QDebug>
#include <QDeclarativeComponent>
#include <QDeclarativeView>
#include <QDeclarativeItem>
//#include <QDeclarativeEngine>
#include <QDeclarativeContext>

// removed by Sergey 24.08.2013 for ITS#185556 

#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h"
#include "util/DHAVN_AppMediaPlayer_ViewLoader.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenDVDPlayback.h" // added by kihyung 2012.07.12 for CR 9867

AppMediaPlayer_Video_ScreenManager::AppMediaPlayer_Video_ScreenManager( QDeclarativeView *view,
                                                                       VPEnum::VDISP_T disp,	// added by Sergey 28.08.2013 for ITS#186507 
                                                                       AppMediaPlayer_EngineVideo* pEngineVideo, //added by Sergey 27.09.2013 for ITS#189958
                                                                       QObject *parent  )
    :QObject(parent),
     m_pView(NULL),
     m_pRootItem(NULL),
     m_pStatusBar(NULL),
     m_focusIndex(-1),
     m_isFocusVisible(false),
     m_isFocusActive(false),
// removed by Dmitry 18.07.13 for ISV15579
     m_pJogSource(NULL),
// removed by cychoi 2013.07.27 for ISV 88393
     m_isLockoutMode(false),//added by edo.lee 2013.02.25
     m_disp(disp),	// added by Sergey 28.08.2013 for ITS#186507
     m_pEngineVideo(pEngineVideo), //added by Sergey 27.09.2013 for ITS#189958
     m_focusMode(FOCUS_MODE_MAX), // added by sjhyun 2013.11.08 for ITS 207579
     m_isRRC(false)
{
    m_pEngine = view->engine();//added by edo.lee 2013.02.07
    m_pViewLoader = new AppMediaPlayer_ViewLoader( view, this );
    connect( m_pViewLoader,
             SIGNAL(viewLoaded(const QString&, QDeclarativeItem*,QDeclarativeContext*)),
             this,
             SLOT(onViewLoaded(const QString&,QDeclarativeItem*,QDeclarativeContext*)));

    m_listPlaybackScreens.clear();
    m_listPlaybackScreens << AppMediaPlayer_Video_Screens::SCREEN_JUKEBOX_PLAYBACK <<
                             AppMediaPlayer_Video_Screens::SCREEN_USB_PLAYBACK <<
                             AppMediaPlayer_Video_Screens::SCREEN_USB2_PLAYBACK <<
                             AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK <<
                             AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK <<
                             AppMediaPlayer_Video_Screens::SCREEN_FULL;
                             // removed by Sergey 10.08.2013 for 182892

    m_isMenuHardKeyPressed = false; // added by sungha.choi 2013.07.15 for static test
    m_pEmptyView = NULL;     // added by sungha.choi 2013.07.15 for static test
    m_stackAction = STACK_PUSH; // added by sungha.choi 2013.07.15 for static test
    // removed by Sergey 31.08.2013 for ITS#187830
}


/** @copydoc AppMediaPlayer_Video_ScreenManager::registerScreen() */
void AppMediaPlayer_Video_ScreenManager::registerScreen( const QString &screenName,
                                                        AppMediaPlayer_Video_Screen *screen )
{
    if ( m_screens.contains( screenName ) )
    {
        MP_LOG << "Screen" << screenName << "is registered already." << LOG_ENDL;
        return;
    }

    if ( screen == NULL )
    {
        MP_LOG << "Screen" << screenName << "has not controller. Are you sure about what are you doing?" << LOG_ENDL;
        return;
    }

    m_screens.insert( screenName, screen );
}


/** @copydoc AppMediaPlayer_Video_ScreenManager::preloadView() */
void AppMediaPlayer_Video_ScreenManager::preloadView( const QString &viewName )
{
    m_pViewLoader->loadView(viewName);
}


/** @copydoc AppMediaPlayer_Video_ScreenManager::changeScreen() */
void AppMediaPlayer_Video_ScreenManager::changeScreen( const QString &screenName, QVariant arg )
{
    //MP_LOG << "screenName =" << screenName << LOG_ENDL;

    if ( !m_screens.contains(screenName)
         || ( !m_screenStack.empty() && (m_screenStack.top() == screenName ) ) )
    {
        MP_LOG << "Unknown screen" << screenName << LOG_ENDL;
        return;
    }

    MP_LOG << "screenName =" << screenName << LOG_ENDL;

    m_stackAction = STACK_CHANGE;
    m_pendingScreen = screenName;
    // removed by cychoi 2013.07.27 for ISV 88393
    m_screenStartupArgument = arg;

    m_pViewLoader->loadView( m_screens[screenName]->viewName() );
}


/** @copydoc AppMediaPlayer_Video_ScreenManager::pushScreen() */
void AppMediaPlayer_Video_ScreenManager::pushScreen( const QString &screenName, QVariant arg )
{
    //MP_LOG << LOG_ENDL;

    if ( !m_screens.contains(screenName)
         || ( !m_screenStack.empty() && (m_screenStack.top() == screenName ) ) )
    {
        MP_LOG << "Unknown screen" << screenName << LOG_ENDL;
        return;
    }

    MP_LOG << LOG_ENDL;

    // { moved by cychoi 2013.10.04 for ITS 193777
    // { added by shkim 2013.08.27 for ITS 186402
    if(m_screens[screenName]->viewName()=="screen_vcd/DHAVN_VP_VCD_PBC_Menu"  )
    {
        if( m_isLockoutMode == true)
        {
            // Should not display PBC Menu on front if DRS mode
            return;
        }
    }
    // } added by shkim 2013.08.27
    // } moved by cychoi 2013.10.04

    m_stackAction = STACK_PUSH;
    m_pendingScreen = screenName;

    m_screenStartupArgument = arg;

    m_pViewLoader->loadView( m_screens[screenName]->viewName() );
}


// removed by Sergey 10.08.2013 for 182892


/** @copydoc AppMediaPlayer_Video_ScreenManager::popScreen() */
void AppMediaPlayer_Video_ScreenManager::popScreen( QVariant arg ) // modified by Sergey for CR#11607
{
    //MP_LOG << arg << LOG_ENDL;
    if ( m_screenStack.empty() )
    {
        MP_LOG << "What's going on? The screen stack is empty." << LOG_ENDL;
        return;
    }

// removed by Dmitry 18.07.13 for ISV15579

	// } Added by minho
    MP_LOG << "arg =" << arg << " stack count =" << m_screenStack.count() << LOG_ENDL;

    for(int i=0; i<m_screenStack.count(); i++)
    {
       MP_LOG << "stack item =" << m_screenStack.at(i) << LOG_ENDL;
    }

    // The last item in the stack:
    if ( m_screenStack.count() == 1 )
    {
        m_screens[m_screenStack.top()]->hide();
        m_screenStack.clear();
        emit hideUI();
    }
    else // not the last, working as a normal stack pop
    {
        // { added by kihyung 2012.07.12 for CR 9867
        /*
        m_stackAction = STACK_CHANGE;
        m_pendingScreen = m_screenStack.at( m_screenStack.count() - 2 );
        MP_LOG << "pend screen =" << m_pendingScreen << LOG_ENDL;
        m_screenStartupArgument = arg;
        m_pViewLoader->loadView( m_screens[m_pendingScreen]->viewName() );
        */
        
        bool bBackToHome = false;

        // { commented by cychoi 2014.06.27 for removed too many logs... // DVD->Title or Disc menu->back->home video screen.
        //if(m_screenStack.count() == 2 && m_screenStack.top() == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU) {
        //    AppMediaPlayer_Video_ScreenDVDPlayback *pScreen = (AppMediaPlayer_Video_ScreenDVDPlayback*)m_screens[AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU];
        //    if(pScreen->getSelectByTitleMenu() == false) {
        //        bBackToHome = true;
        //    }else{
        //        pScreen->setSelectByTitleMenu(false);
        //    }
        //}
        // } commented by cychoi 2014.06.27
           
        if(bBackToHome == true)
        {
            MP_LOG << "back to home screen" << LOG_ENDL;
            emit hideUI();
        }
        else 
        {
            //MP_LOG << "go to previous screen" << LOG_ENDL;
            m_stackAction = STACK_CHANGE;
			//{ added by eunhye 2013.05.09 ITS 163782 
            if(m_screenStack.count() == 3 && m_screenStack.top() == AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU)
                m_pendingScreen = m_screenStack.at( m_screenStack.count() - 3 );
            else
			// } added by eunhye 2013.05.09 ITS 163782 
                m_pendingScreen = m_screenStack.at( m_screenStack.count() - 2 );
            MP_LOG << "go to previous screen. pend screen =" << m_pendingScreen << LOG_ENDL;
            m_screenStartupArgument = arg;
            m_pViewLoader->loadView( m_screens[m_pendingScreen]->viewName() );    
        }
        // } added by kihyung
    }
}

/** @copydoc AppMediaPlayer_Video_ScreenManager::topScreen() */
AppMediaPlayer_Video_Screen *AppMediaPlayer_Video_ScreenManager::topScreen()
{
    //MP_LOG << LOG_ENDL;

    if ( m_screenStack.empty() )
    {
        MP_LOG << "Looks like there are no screens available." << LOG_ENDL;
        return NULL;
    }

    MP_LOG << LOG_ENDL;

    return m_screens[m_screenStack.top()];
}

void AppMediaPlayer_Video_ScreenManager::checkPrevScreen(QString screen)
{
    MP_LOG << "screen = " << screen << LOG_ENDL;
}

AppMediaPlayer_Video_Screen *AppMediaPlayer_Video_ScreenManager::previousScreen()
{
   //MP_LOG << LOG_ENDL;

   if ( m_screenStack.empty() )
   {
       MP_LOG << "Looks like there are no screens available." << LOG_ENDL;
       return NULL;
   }

   MP_LOG << LOG_ENDL;

   int previousScreenIndex = m_screenStack.size()-2;
   if ( previousScreenIndex < 0 )
   {
      return NULL;
   }

   QString prevScreenName = m_screenStack.at( previousScreenIndex );
   checkPrevScreen(prevScreenName);

   if(!m_screens.contains(prevScreenName))
   {
       return NULL;
   }

//   AppMediaPlayer_Video_Screen *result = m_screens[m_screenStack.at( previousScreenIndex )];

//    QString className = result->metaObject()->superClass()->className();

//    MP_LOG << "AAA class =" << className << LOG_ENDL;

   return m_screens[prevScreenName];
}

// { added by lssanh 2013.04.19 ITS158191
AppMediaPlayer_Video_Screen *AppMediaPlayer_Video_ScreenManager::getPreviousScreen(int step)
{
   //MP_LOG << LOG_ENDL;

   if ( m_screenStack.empty() || (m_screenStack.size() <= step) )
   {
       MP_LOG << "Looks like there are no screens available." << LOG_ENDL;
       return NULL;
   }

   MP_LOG << LOG_ENDL;

   int previousScreenIndex = m_screenStack.size()-(step+1);
   if ( previousScreenIndex < 0 )
   {
      return NULL;
   }

   return m_screens[m_screenStack.at( previousScreenIndex )];
}
// } added by lssanh 2013.04.19 ITS158191

/** @copydoc AppMediaPlayer_Video_ScreenManager::revertScreen() */
bool AppMediaPlayer_Video_ScreenManager::revertScreen()
{
    if ( m_screenHistory.empty() ) {
        MP_LOG << "Screen history is empty. Doing nothing." << LOG_ENDL;
        return false;
    }

    // The last screen in the history is likely to be the same as
    // the current one, so a little magic here:
    QString historyEntry = m_screenHistory.last();
    m_screenHistory.removeLast();
    if ( historyEntry == m_screenStack.first() ) {
        historyEntry = m_screenHistory.last();
        m_screenHistory.removeLast();
    }
    
    changeScreen( historyEntry );
    return true;
}

void AppMediaPlayer_Video_ScreenManager::replaceScreen( const QString &screenName,
                                                            QVariant arg )
{
   if ( !m_screens.contains(screenName) )
   {
       MP_LOG << "Unknown screen" << screenName << LOG_ENDL;
       return;
   }
   if ( m_screenStack.isEmpty() )
   {
      MP_LOG << "Have no screen to replace!" << LOG_ENDL;
      return;
   }

   m_stackAction = STACK_REPLACE;
   m_pendingScreen = screenName;

   m_screenStartupArgument = arg;

   m_pViewLoader->loadView( m_screens[screenName]->viewName() );
}

// removed by cychoi 2013.08.05 for unused code

// { modified by Sergey 04.08.2013
void AppMediaPlayer_Video_ScreenManager::replacePrevScreen( const QString &screenName,int step,
                                                            QVariant arg,
                                                            bool bHide) // added by yungi 2013.09.08 for ITS 186704
{
    Q_UNUSED(arg);

    //MP_LOG  << "step =" << step << LOG_ENDL;

    step ++;
    if (m_screenStack.empty() || m_screenStack.count() < step)
    {
        MP_LOG << "step =" << step << " Screen history is empty. Doing nothing." << LOG_ENDL;
        return;
    }

    int replacedScreenIndex = m_screenStack.count()-step;
    QString replacedScreenName = m_screenStack.value(replacedScreenIndex);
    MP_LOG << "step =" << step << " newScreen =" << screenName << " oldScreen =" << replacedScreenName << LOG_ENDL;

    // change previous screen history and stack with requested screen
    m_screenStack.replace(replacedScreenIndex, screenName);

    if(m_screenHistory.indexOf(replacedScreenName) == replacedScreenIndex) // Sometimes screen history is not updated properly so need this condition before update
        m_screenHistory.replace(replacedScreenIndex, screenName);

    // { modified by yungi 2013.09.08 for ITS 186704
    if(bHide)
        m_screens[replacedScreenName]->hide();
    // } modified by yungi 2013.09.08
}
// } modified by Sergey 04.08.2013

void AppMediaPlayer_Video_ScreenManager::setItemProperty( const QString &itemName,
                                                          const char *property,
                                                          const QVariant &value )
{
    if ( m_screenStack.isEmpty() )
    {
       return;
    }

       // Search for an item in the tree of child items of the current view.
       QObject *item = m_screens[m_screenStack.top()]->getChildItem(itemName);

       if ( item != NULL )
       {
          item->setProperty( property, value );
       }
       else // Item not found.
       {   // Search for an item in the tree of child items of the base view.
          if ( m_pRootItem != NULL)
             item = m_pRootItem->findChild<QObject *>(itemName);

          if ( item != NULL )
          {
             item->setProperty( property, value );
          }
          else // Item not found.
          {   // Search for an item in the list of the overlays.
             foreach(AppMediaPlayer_Video_Screen *overlay, m_overlays.values())
             {
                 item = overlay->getChildItem( itemName );
                 if ( item != NULL )
                 {
                    item->setProperty( property, value );
                    break;
                 }
             }
          }
       }
}

void AppMediaPlayer_Video_ScreenManager::onHideUI()
{
   MP_LOG << LOG_ENDL;
   emit hideUI();
}

void AppMediaPlayer_Video_ScreenManager::addOverlay( const QString &screenName )
{
    if ( !m_screens.contains(screenName) )
    {
        MP_LOG << "Unknown screen" << screenName << LOG_ENDL;
        return;
    }

    if ( m_overlays.contains(screenName) )
    {
        MP_LOG << "Overlay" << screenName << "is already on screen. Ignoring." << LOG_ENDL;
        return;
    }

    m_overlaysPending << screenName;
    m_pViewLoader->loadView( m_screens[screenName]->viewName() );
}

void AppMediaPlayer_Video_ScreenManager::removeOverlay( const QString &overlayName )
{
    if ( m_overlays.contains( overlayName ) )
    {
        m_overlays.remove( overlayName );
        bool wantDeactivate = false;
        // Here we need to check if overlay should retain visible.
        foreach( QString activeScreen, m_screenStack )
        {
            if ( m_screens.contains(activeScreen)
                 && m_screens.value(activeScreen) != NULL )
            {
                AppMediaPlayer_Video_Screen *screen = m_screens[activeScreen];
                if ( screen->m_overlays.contains(overlayName) )
                {
                    wantDeactivate = true;
                }
            }
        }

        if ( wantDeactivate )
        {
            deactivateScreen( overlayName );
        }
        else
        {
            hideScreen( overlayName );
        }
    }
    else
    {
        MP_LOG << "Overlay" << overlayName << "is not visible. Ignoring." << LOG_ENDL;
    }
}


/** @brief Slot invoked by the view loader.
 * While we should not assume if view is got immidiately from the cache
 * of the view loader or loaded for a while, all stack and visibility
 * manipulations are implemented here.
 *
 * The stack manipulations are ruled by the two members of the screen manger:
 * the m_pendingScreen and m_stackAction. The idea is that m_screenAction
 * switches the stack manipulation kind, and the #m_pendingScreen describes
 * what to be pushed or changed.
 *
 * There is no pop operation. It is by design. Please look through the
 * changeScreen method documentation (it is in header file, not here). The
 * screen change operation is designed to be flexible enough to be used as
 * total screen stack change and as trivial pop.
 * */
void AppMediaPlayer_Video_ScreenManager::onViewLoaded( const QString &viewName,
                                                      QDeclarativeItem *pView,
                                                      QDeclarativeContext *pContext )
{
    MP_LOG << viewName <<pContext << LOG_ENDL;

    foreach( QString overlay, m_overlaysPending )
    {
        if ( m_screens[overlay]->viewName() == viewName )
        {            
            m_overlaysPending.removeAll(overlay);
            
            // Make overlay overlap everything
            pView->setProperty( "z", 1000 );
            showScreen( overlay, pView, pContext, true );
            return;
        }
    }
    
    // { modified by cychoi 2013.10.04 for ITS 193777
    if ( m_screens.contains( m_pendingScreen ) )
    {
        // While it is possible that many screens to be loaded a time,
        // we need to check if we've got the requested one:
        if ( m_screens[m_pendingScreen]->viewName() != viewName )
        {
            return;
        }
    }
    // } modified by cychoi 2013.10.04

    // Do the stack manipulations:
    if ( m_stackAction == STACK_PUSH )
    {
        // First of all, disable the top-level screen
        if ( !m_screenStack.isEmpty() )
        {
            deactivateScreen( m_screenStack.top() );
        }

        m_screenStack.push( m_pendingScreen );
        showScreen( m_pendingScreen, pView, pContext, false );
    }
    else if ( m_stackAction == STACK_REPLACE )
    {
       changeLastScreen(m_pendingScreen, pView, pContext);
    }
    else
    {
        // Need to save screen history before manipulating the screen
        // stack, as the screen will be unavailable after this stuff.
        updateScreenHistory();
        
        // if we have a pending screen in the stack then unwind it
        unwindScreenStack( m_pendingScreen );
        
        // Oops, we were moving to screen out of the screen stack,
        if ( m_screenStack.isEmpty() )
        {
            m_screenStack.push( m_pendingScreen );
        }

        showScreen( m_pendingScreen, pView, pContext, false );
    }

    m_pendingScreen = "";
}


void AppMediaPlayer_Video_ScreenManager::unwindScreenStack( const QString &screenName )
{
    while ( !m_screenStack.isEmpty() &&
            m_screenStack.top() != screenName )
    {
        QString top = m_screenStack.pop();
        MP_LOG << "remove top screen" << top << " stack count" << m_screenStack.count() << LOG_ENDL;
        hideScreen(top);
    }
}

/* This function just calls onShow slot of top screen */
void AppMediaPlayer_Video_ScreenManager::activateTopScreen(QVariant arg)
{
    topScreen()->onShow(arg);
} // added by Sergey 24.07.2013


// { modified by Sergey 25.07.2013 for ITS#181724
void AppMediaPlayer_Video_ScreenManager::showScreen( const QString &screenName,
                                                     QDeclarativeItem *pView,
                                                     QDeclarativeContext *pContext,
                                                     bool bOverlay )
{
    //MP_LOG << "screenName:" << screenName << LOG_ENDL;
    AppMediaPlayer_Video_Screen *screen;

    // { modified by cychoi 2013.10.04 for ITS 193777
    if ( !m_screens.contains(screenName) )
    {
        MP_LOG << "Unknow screen name" << LOG_ENDL;
        return;
    }
    // } modified by cychoi 2013.10.04

    MP_LOG << "screenName:" << screenName << LOG_ENDL;

    screen = m_screens[screenName];

    if ( pView != NULL )
    {
       m_pView = pView;
    }

    if ( m_pRootItem == NULL )
    {
        findRootItem();
    }

    connect( screen, SIGNAL(changeScreen(const QString&, QVariant)),
             this, SLOT(changeScreen(const QString &, QVariant)));
    connect( screen, SIGNAL(pushScreen(const QString &, QVariant)),
             this, SLOT(pushScreen(const QString &, QVariant)));
    connect( screen, SIGNAL(popScreen(QVariant)),
             this, SLOT(popScreen(QVariant)));
    connect( screen, SIGNAL(replaceScreen(QString,QVariant)),
            this, SLOT(replaceScreen(QString,QVariant)));
    connect( screen, SIGNAL(setItemProperty(QString,const char*,QVariant)),
            this, SLOT(setItemProperty(QString,const char*,QVariant)));
    connect( screen, SIGNAL(hideUI()),
             this, SLOT(onHideUI()));
    connect( screen, SIGNAL(notifyScreenManager(const QString &, const QString &,
                                                const int, QVariant )),
             this, SLOT(onNotifyScreenManager(const QString &, const QString &,
                                              const int, QVariant )));
    connect( screen, SIGNAL(hideAllFocus()),
             this, SLOT(hideAllFocus()) );

    //added by shkim for ITS 181234
    if(screenName == "VCD_DirectAcccess")
        m_isFocusVisible = true;

    //added by shkim for ITS 181234
    //bool aIsFocusVisible = m_isFocusVisible; // added by wspark for CR12020
    if ( bOverlay )
    {
        m_overlays.insert( screenName, screen );
        screen->show( pView, pContext, QVariant() );
    }
    else
    {
        // Add necessary overlays.
        updateOverlays( screen );
        updateZOrder( pView, bOverlay ); // moved by Sergey 20.08.2013 for ITS#178437

        screen->show( pView, pContext, m_screenStartupArgument );
        m_screenStartupArgument = QVariant();

        updateFocusedSequence( screenName );

        MP_MEDIUM << "focusSequense =" << m_focusSequence << LOG_ENDL; // added by Sergey 30.10.2013 for focus in AUX

        // { modified by Sergey 10.01.2014 for ITS#218955
        // If focused sequence is not empty - we just showing focus on the screen.
        if ( !m_focusSequence.isEmpty() )
        {
            if(screen->getIsModeAreaFocused())
                m_focusIndex = 0;
            else
                m_focusIndex = m_focusSequence.length() - 1;
        }
        // }  modified by Sergey 10.01.2014 for ITS#218955
        else
        {
            // { modified by Sergey 06.11.2013 for ITS#207002
            MP_HIGH << "focus sequence is empty. Show focus on screen and return." << LOG_ENDL;

            m_focusIndex = -1;
            QMetaObject::invokeMethod( pView, "showFocus" );
            return;
            // } modified by Sergey 06.11.2013 for ITS#207002
        }

        MP_HIGH << "focusIndex =" << m_focusIndex << LOG_ENDL; // added by Sergey 30.10.2013 for focus in AUX

            QMetaObject::invokeMethod( m_focusSequence[m_focusIndex], "showFocus" ); // added by Sergey 30.10.2013 for focus in AUX
    }

    // updateZOrder( pView, bOverlay ); // moved by Sergey 20.08.2013 for ITS#178437
    // { added by wspark 2012.08.02 for CR12020
    if(m_isMenuHardKeyPressed == true && screenName == "Options" /*&& m_isFocusVisible == false*/) { // modified by dongjin 2012.11.23
        handleJogEvent(AppEngineQMLConstants::JOG_WHEEL_RIGHT, AppEngineQMLConstants::KEY_STATUS_PRESSED); // modified by Dmitry 15.05.13
        setFocusActive(false); // added by dongjin 2012.11.30
    }
    // } added by wspark
}
// } modified by Sergey 25.07.2013 for ITS#181724


void AppMediaPlayer_Video_ScreenManager::deactivateScreen( const QString &screenName )
{
    AppMediaPlayer_Video_Screen *screen;

    // { modified by cychoi 2013.10.04 for ITS 193777
    if ( !m_screens.contains(screenName) )
    {
        MP_LOG << "Unknow screen name" << LOG_ENDL;
        return;
    }
    // } modified by cychoi 2013.10.04

    screen = m_screens[screenName];
    screen->deactivate();

    hideFocus(); // modified by Sergey 24.08.2013 for ITS#185556 

    m_focusIndex = -1;

    // Don't forget to disconnect focus stuff:
    foreach( QDeclarativeItem *item, m_focusSequence ) {
       disconnect( item, SIGNAL(lostFocus(int,int)), this, SLOT(handleFocusLost(int,int)) );
    }
    disconnect( screen, 0, this, 0 );
}


void AppMediaPlayer_Video_ScreenManager::hideScreen( const QString &screenName )
{
    AppMediaPlayer_Video_Screen *screen;

    // { modified by cychoi 2013.10.04 for ITS 193777
    if ( !m_screens.contains(screenName) )
    {
        MP_LOG << "Unknow screen name" << LOG_ENDL;
        return;
    }
    // } modified by cychoi 2013.10.04

    setFocusActive( false );// added by yongkyun.lee 20130628 for : its 177139

    screen = m_screens[screenName];

    screen->hide();

    deactivateScreen( screenName );
}

void AppMediaPlayer_Video_ScreenManager::updateScreenHistory()
{
    if ( m_screenStack.empty() )
    {
        MP_LOG << "Screen stack is empty. Ignoring." << LOG_ENDL;
        return;
    }
    
    QString currentScreen = m_screenStack.first();
    if ( m_screenHistory.contains( currentScreen ) )
    {
        // If current screen is not the last in the history move it here
        if( m_screenHistory.last() != currentScreen )
        {
            m_screenHistory.removeOne( currentScreen );
            m_screenHistory.append( currentScreen );
        }
    }
    else
    {
        m_screenHistory.append( currentScreen );
    }
}

void AppMediaPlayer_Video_ScreenManager::changeLastScreen(  const QString &screenName,
                                                             QDeclarativeItem *pView,
                                                             QDeclarativeContext *pContext )
{
   if ( m_screenStack.isEmpty() )
      return;

   QString top = m_screenStack.pop();
   MP_LOG << "remove top screen" << top << " stack count" << m_screenStack.count() << LOG_ENDL;
   hideScreen(top);

   m_screenStack.push( screenName );
   showScreen( screenName, pView, pContext, false );
}

void AppMediaPlayer_Video_ScreenManager::updateOverlays( AppMediaPlayer_Video_Screen *screen )
{
    QStringList removeList;
    QStringList addList;
    QString overlay;

    foreach(overlay, m_overlays.keys() )
    {
        if ( screen->m_overlays.indexOf(overlay) == -1 )
        {
            removeList << overlay;
        }
    }
    
    foreach(overlay, screen->m_overlays )
    {
        if ( !m_overlays.contains( overlay ) )
        {
            addList << overlay;
        }
    }

    foreach(overlay, removeList )
    {
        removeOverlay( overlay );
        MP_LOG << "Removing " << overlay << "overlay." << LOG_ENDL;
    }

    foreach(overlay, addList )
    {
        addOverlay( overlay );
        MP_LOG << "Adding" << overlay << "overlay." << LOG_ENDL;
    }

    // It is possible that we're able to reuse some of the overlays.
    // The overlays that could be reused have not been removed from
    // the m_overlays hash, so we need to update z-order of these
    // overlays to make the match the screen stack:
    foreach( overlay, m_overlays.keys() )
    {
        if ( m_overlays.value(overlay)->m_pView != NULL )
        {
            updateZOrder( m_overlays.value(overlay)->m_pView, true );
        }
    }
}

void AppMediaPlayer_Video_ScreenManager::updateZOrder( QDeclarativeItem *pView,
                                                       bool isOverlay )
{
    // We will reserve 2 z-planes for each screen. The generic offset of
    // 100 is placed here to make sure that no element from the root will not
    // overlap our screens. The base z-plane is reserved for the screen
    // itself, base z-plane + 1 is reserved for the overlays.
    int baseZOrder = m_screenStack.size() * 2 + 100;

    if ( isOverlay )
    {
        pView->setProperty("z", baseZOrder + 1);
    }
    else
    {
        pView->setProperty( "z", baseZOrder );
    }
}

void AppMediaPlayer_Video_ScreenManager::findRootItem()
{
    QDeclarativeItem *root = m_pView;
    while( root->parentItem() != NULL )
    {
        root = root->parentItem();
    }

    m_pRootItem = root;
}


void AppMediaPlayer_Video_ScreenManager::onPrevKeyPressed()
{
    AppMediaPlayer_Video_Screen *screen = topScreen();
    if ( screen != NULL )
    {
        screen->onPrevKeyPressed();
    }

    foreach(AppMediaPlayer_Video_Screen *overlay, m_overlays.values())
    {
        overlay->onPrevKeyPressed();
    }
}

void AppMediaPlayer_Video_ScreenManager::onNextKeyPressed()
{
    AppMediaPlayer_Video_Screen *screen = topScreen();
    if ( screen != NULL )
    {
        screen->onNextKeyPressed();
    }
    foreach(AppMediaPlayer_Video_Screen *overlay, m_overlays.values())
    {
        overlay->onNextKeyPressed();
    }    
}

void AppMediaPlayer_Video_ScreenManager::onBackKeyPressed()
{
    AppMediaPlayer_Video_Screen *screen = topScreen();
    if ( screen != NULL )
    {
        screen->onBackKeyPressed();
    }
    foreach(AppMediaPlayer_Video_Screen *overlay, m_overlays.values())
    {
        overlay->onBackKeyPressed();
    }    
}

void AppMediaPlayer_Video_ScreenManager::onMenuKeyPressed(int nKeyType) //wspark
{
    // { added by wspark 2012.08.02 for CR12020
    if( nKeyType == EVT_KEY_SOFT_MENU)
    {
    	MP_LOG<<"hide focus" << LOG_ENDL;
        m_isMenuHardKeyPressed = false;
        //emit hideAllFocus(); //deleted by aettie 2013.03.27 for Touch focus rule
    }
    else if (nKeyType == EVT_KEY_CCP_MENU || nKeyType == EVT_KEY_HU_MENU || nKeyType == EVT_KEY_RRC_MENU )
    {
        m_isMenuHardKeyPressed = true;
    }
    // } added by wspark

    AppMediaPlayer_Video_Screen *screen = topScreen();
    if ( screen != NULL )
    {
        screen->onMenuKeyPressed();
    }
    foreach(AppMediaPlayer_Video_Screen *overlay, m_overlays.values())
    {
        overlay->onMenuKeyPressed();
    }    
}

void AppMediaPlayer_Video_ScreenManager::onNotifyScreenManager(const QString &srcScreenName,
                                                               const QString &resScreenName,
                                                               const int eventId,
                                                               QVariant arg)
{
	// removed by Sergey 07.09.2013 for PRE_FG supoort

   if (!m_screens.contains(resScreenName))
   {
      return;
   }

   AppMediaPlayer_Video_Screen *resScreen = m_screens[resScreenName];

   resScreen->onEventReceived( srcScreenName, resScreenName, eventId, arg );
}

void AppMediaPlayer_Video_ScreenManager::hideAllFocus()
{
    if ( m_isFocusVisible )
    {
        hideFocus(); // modified by Sergey 24.08.2013 for ITS#185556 

        m_focusIndex = -1;
        setFocusVisible( false );
    }
}

void AppMediaPlayer_Video_ScreenManager::setContext( QDeclarativeContext *context )
{
   m_pViewLoader->setContext( context );
}

void AppMediaPlayer_Video_ScreenManager::setRootItem( QObject *root )
{
   MP_LOG << LOG_ENDL;
   
   m_pViewLoader->setRootItem( root );
   if ( m_pStatusBar == NULL ) {
      m_pStatusBar = root->findChild<QDeclarativeItem*>("statusBar");
      if ( m_pStatusBar == NULL ) {
         MP_LOG << "Unable to find status bar. " << "Root child objects:" << LOG_ENDL;

         // QObjectList children = root->children();
         // for ( int i = 0; i < children.length(); ++i ) {
         //    MP_LOG << "   " << children.at(i)->objectName() << LOG_ENDL;
         // }
      }
   }
}

/** @copydoc AppMediaPlayer_Video_ScreenManager::topScreenName() */
QString AppMediaPlayer_Video_ScreenManager::topScreenName()
{
	// removed by Sergey 01.05.2013 for Tune Functionality

    if ( m_screenStack.empty() )
    {
    	// removed by Sergey. This function is called too often and junks log file.
        return QString();
    }

    return m_screenStack.top();
}

// { added by lssanh 2013.04.19 ITS158191
QString AppMediaPlayer_Video_ScreenManager::getPreviousScreenName(int preIdx)
{
	//MP_LOG << LOG_ENDL;
	
	if ( m_screenStack.empty() || (m_screenStack.size() <= preIdx) )
	{
		MP_LOG << "Looks like there are no screens available." << LOG_ENDL;
		return NULL;
	}
	
	int previousScreenIndex = m_screenStack.size()-(preIdx+1);
	if ( previousScreenIndex < 0 )
	{
       MP_LOG << LOG_ENDL;
	   return NULL;
	}
	MP_LOG<< "previous screen : " <<m_screenStack.at( previousScreenIndex ) << LOG_ENDL;
	return m_screenStack.at( previousScreenIndex );
}
// } added by lssanh 2013.04.19 ITS158191

void AppMediaPlayer_Video_ScreenManager::clearStack()
{
    MP_LOG << LOG_ENDL;

    // removed by cychoi 2013.07.27 for ISV 88393

    while(!m_screenStack.isEmpty())
    {
        QString top = m_screenStack.pop();
        MP_LOG << "remove top screen" << top << " stack count" << m_screenStack.count() << LOG_ENDL;
        hideScreen(top);
    }
}
// added by edo.lee 2013.02.05
void AppMediaPlayer_Video_ScreenManager::lockoutMode(bool onShow)//MEDIA_SOURCE_T source)
{
	MP_LOG<<"onShow "<<onShow << LOG_ENDL;
	if(m_screenStack.empty()) return; //added by edo.lee 2013.03.08
	emit loadLockoutTab(onShow); //added by edo.lee 2013.03.12
	// removed by Sergey 31.07.2013 for ITS#180929

	if( m_screenStack.top() == AppMediaPlayer_Video_Screens::SCREEN_JUKEBOX_PLAYBACK
        ||  m_screenStack.top() == AppMediaPlayer_Video_Screens::SCREEN_USB_PLAYBACK
        ||  m_screenStack.top() == AppMediaPlayer_Video_Screens::SCREEN_USB2_PLAYBACK
        ||  m_screenStack.top() == AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK
        ||  m_screenStack.top() == AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK
        ||  m_screenStack.top() == AppMediaPlayer_Video_Screens::SCREEN_AUX1_PLAYBACK //added by edo.lee 2013.03.16
        ||  m_screenStack.top() == AppMediaPlayer_Video_Screens::SCREEN_AUX2_PLAYBACK//added by edo.lee 2013.03.16
        ||  m_screenStack.top() == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU
        ||  m_screenStack.top() == AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU
        ||  m_screenStack.top() == AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS  //added by yungi 2013.09.05 for ITS 187311
        ||  m_screenStack.top() == AppMediaPlayer_Video_Screens::SCREEN_FULL
        ||  m_screenStack.top() == AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON
        ||  m_screenStack.top() == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)//added by edo.lee 2013.03.21
	{
		MP_LOG << "screen onLockoutMode" << LOG_ENDL;	
		m_screens[m_screenStack.top() ]->onLockoutMode(onShow);
	}

}

// removed by Sergey 07.09.2013 for PRE_FG supoort

void AppMediaPlayer_Video_ScreenManager::fgBgStateChange(bool isFG)
{
	emit fgBgStateChanged(isFG);
}

void AppMediaPlayer_Video_ScreenManager::closeProgressPopup()
{
	if(topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_POP_PROGRESS)
		popScreen(AppMediaPlayer_Video_Screens::SCREEN_POP_PROGRESS);
}
void AppMediaPlayer_Video_ScreenManager::closeOptionPopup()
{
	if(topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)
		popScreen(AppMediaPlayer_Video_Screens::SCREEN_OPTIONS);
}
//added by edo.lee 2013.02.25
void AppMediaPlayer_Video_ScreenManager::closeFullScreen()
{
	if(topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_FULL)
		popScreen(AppMediaPlayer_Video_Screens::SCREEN_FULL);
}
//added by edo.lee

// added by edo.lee

// Up to this moment we need to recall, that we must support dual display.
// While separate screens have another set of jogs (CCP and RRC) we need
// an interface that would send them. We're lucky enough, and AppFramework
// provides such interface, all we need is to use it.
void AppMediaPlayer_Video_ScreenManager::setJogSource( QObject *pUIListener )
{
   if (m_pJogSource != NULL)
   {
      disconnect( m_pJogSource, 0, this, 0 );
      m_pJogSource = NULL;
   }

   if (pUIListener != NULL)
   {
      m_pJogSource = pUIListener;
      connect( m_pJogSource, SIGNAL(signalJogNavigation(uint,int,bool,int)), // modified by edo.lee 2013.11.14
               this, SLOT(handleJogEvent(uint,int,bool)));
   }//modified by aettie 20130620 for back key event
}

// { added by kihyung 2013.2.6 for ISV 70642   
void AppMediaPlayer_Video_ScreenManager::hideTopscreen()
{
    MP_LOG << LOG_ENDL;

    if(m_screenStack.isEmpty() == false)
    {
        QString top = m_screenStack.pop();
        MP_LOG << "remove top screen" << top << " stack count" << m_screenStack.count() << LOG_ENDL;
        hideScreen(top);
    }
}
// } added by kihyung 2013.2.6 for ISV 70642   

// Slot for lostFocus() signal emitted from QML UI. All independent QML screens
// that are visible at the moment are connected to this slot. Independent QML screens
// here stands for overlays (ModeArea), screen's root QML, and status bar component.
void AppMediaPlayer_Video_ScreenManager::handleFocusLost( int direction, int focusId )
{
   Q_UNUSED(focusId);
   // We should ignore lostFocuses from active screen which is indicated
   // by m_focusIndex == m_focusSequence.length() - 1, but need to exept UP event for moving to mode area

 /*	 //modified by taihyun.ahn for ITS 218397 2014.01.08
    if(m_pEngineVideo->getEngineMain().getCloneState() == CLONE_OFF &&
        (m_disp == VPEnum::VDISP_FRONT && m_isRRC) ||
            (m_disp == VPEnum::VDISP_REAR && !m_isRRC))
    {
        return;
    }
     //modified by taihyun.ahn for ITS 218397 2014.01.08*/

   if ( m_focusSequence.isEmpty() || (m_focusSequence.length() == 1) )
   {
      MP_LOG << "Screen has not focusable items or lostFocus should be ignored." << LOG_ENDL;
      return;
   }

   if ( !m_isFocusVisible ) 
   {
      MP_LOG << "Initial focus" << direction << LOG_ENDL;
      if ( direction == AppEngineQMLConstants::JOG_UP ||
           direction == AppEngineQMLConstants::JOG_WHEEL_LEFT )
      {
         m_focusIndex = 0;
      }

      if ( direction == AppEngineQMLConstants::JOG_DOWN ||
           direction == AppEngineQMLConstants::JOG_WHEEL_RIGHT )
      {
         m_focusIndex = m_focusSequence.length() - 1;
      }
      setFocusVisible( true );
   }
   else // m_isFocusVisible == true
   {

        hideFocus(); // modified by Sergey 24.08.2013 for ITS#185556

        if ( direction == AppEngineQMLConstants::JOG_UP )
        {
           m_focusIndex--;
        }

        if ( direction == AppEngineQMLConstants::JOG_DOWN )
        {
           m_focusIndex++;
        }

        if ( m_focusIndex < 0 )
        {
            m_focusIndex = 0;
        }
        else if ( m_focusIndex >= m_focusSequence.length() )
        {
            m_focusIndex = m_focusSequence.length() -1;
        }

   }

   // { modified by Sergey 24.08.2013 for ITS#185556 
   if ( m_isFocusVisible )
   {
       updateIsModeAreaFocused(); // added by Sergey 10.01.2014 for ITS#218955

       invokeSetDefaultFocus(m_focusSequence[m_focusIndex], direction);
       showFocus();
   }
   // } modified by Sergey 24.08.2013 for ITS#185556 
}

//{ added by yongkyun.lee 20130225 for :  ISV 73823
void AppMediaPlayer_Video_ScreenManager::SendJogEvent( uint direction, int status )
{
    handleJogEvent(  direction,  status );
    return;
}
//} added by yongkyun.lee 20130225 
//modified by aettie 20130620 for back key event
void AppMediaPlayer_Video_ScreenManager::handleJogEvent( uint direction, int status, bool bRRC )
{
    Q_UNUSED(bRRC);
    m_isRRC = bRRC;//added by taihyun.ahn for ITS 218397 2014.01.08

    //MP_LOG << "direction =" << direction << "status =" << status << "bRRC =" << bRRC << LOG_ENDL;// added by Sergey for CR#13466

    // { modified by kihyung 2013.08.06 for ITS 0182906
    if(m_screenStack.empty() == true) // modified by Sergey 31.08.2013 for ITS#187830
	{
		MP_LOG<<"video is not active" << LOG_ENDL;
		return;//added by edo.lee 2013.02.06
    }
    // } modified by kihyung 2013.08.06 for ITS 0182906
    
    // { modified by Sergey 27.09.2013 for ITS#189958, 191976

    //modified by taihyun.ahn for ITS 221363 2014.01.21
    MP_LOG << "direction =" << direction << " status =" << status << " bRRC =" << bRRC << " pbOwner" << m_pEngineVideo->pbOwner() << " DISP = " << m_disp << m_pEngineVideo->seekHKPressed() << LOG_ENDL;// added by Sergey for CR#13466
    if(!m_pEngineVideo->seekHKPressed())
    {
        if(((m_pEngineVideo->getEngineMain().getCloneState() == CLONE_OFF &&
            (m_disp == VPEnum::VDISP_FRONT && bRRC) ||
            (m_disp == VPEnum::VDISP_REAR && !bRRC)) && m_pEngineVideo->getEngineMain().isDRSShow()))
        {
            return;
        }
    }
    else
    {
        if(status == AppEngineQMLConstants::KEY_STATUS_RELEASED
                || status == AppEngineQMLConstants::KEY_STATUS_CANCELED)
        {
            m_pEngineVideo->setSeekHKPressed(false);
        }
    }
    //modified by taihyun.ahn for ITS 221363 2014.01.21
     
    if(m_pEngineVideo->isPlaybackScreen(m_screenStack.top()))
    {
    	// { modified by Sergey 03.10.2013 for ITS#193201
        if(status == AppEngineQMLConstants::KEY_STATUS_RELEASED
                || status == AppEngineQMLConstants::KEY_STATUS_CANCELED)
        {
            //added by suilyou 20131007 ITS 0192027
            switch(direction)
            {
                case AppEngineQMLConstants::JOG_LEFT:
                {
                    if(!bRRC)
                        m_pEngineVideo->getEngineMain().setJogKeyReleased(CCP_JOG_LEFT_PRESSED);
                    else
                        m_pEngineVideo->getEngineMain().setJogKeyReleased(RRC_JOG_LEFT_PRESSED);
                        break;
                }
                case AppEngineQMLConstants::JOG_RIGHT:
                {
                    if(!bRRC)
                        m_pEngineVideo->getEngineMain().setJogKeyReleased(CCP_JOG_RIGHT_PRESSED);
                    else
                        m_pEngineVideo->getEngineMain().setJogKeyReleased(RRC_JOG_RIGHT_PRESSED);
                        break;
                }
                case AppEngineQMLConstants::JOG_CENTER:
                {
                    if(!bRRC)
                        m_pEngineVideo->getEngineMain().setJogKeyReleased(CCP_JOG_CENTER_PRESSED);
                    else
                        m_pEngineVideo->getEngineMain().setJogKeyReleased(RRC_JOG_CENTER_PRESSED);
                        break;
                }
                default:
                {
                    break;
                }
            }

            onNotifyScreenManager("", m_screenStack.top(), EVENTS_RESTART_FULLSCREEN_TIMER);
        }
        // } modified by Sergey 03.10.2013 for ITS#193201

        if(m_pEngineVideo->isTuneMode() && m_focusIndex == 1
            && (direction == AppEngineQMLConstants::JOG_RIGHT || direction == AppEngineQMLConstants::JOG_LEFT))
        {
            MP_LOG << "Ignore SEEK event as TUNE mode is on. Return." << LOG_ENDL;
            return;
        }
    }
    // } modified by Sergey 27.09.2013 for ITS#189958, 191976

    // { added by Sergey 20.04.2013
    // Should not remove full screen if Next/Prev but remove it if FF/REW.
    if( ( m_screenStack.top() == AppMediaPlayer_Video_Screens::SCREEN_FULL )
        &&
        (
          ( ( direction == AppEngineQMLConstants::JOG_RIGHT || direction == AppEngineQMLConstants::JOG_LEFT )
            && status == AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED ) // modified by Dmitry 15.05.13
          ||
          ( //( direction == AppEngineQMLConstants::JOG_CENTER && status == AppEngineQMLConstants::KEY_STATUS_RELEASED ) || // modified by Dmitry 26.05.13
            direction == AppEngineQMLConstants::JOG_UP || direction == AppEngineQMLConstants::JOG_DOWN ||
            direction == AppEngineQMLConstants::JOG_TOP_RIGHT || direction == AppEngineQMLConstants::JOG_TOP_LEFT ||
            direction == AppEngineQMLConstants::JOG_BOTTOM_RIGHT || direction == AppEngineQMLConstants::JOG_BOTTOM_LEFT
          )
        )
      )
    {
        //emit popFullScreen();
         //modified by taihyun.ahn for ITS 218397 2014.01.08

            popScreen(AppMediaPlayer_Video_Screens::SCREEN_FULL); //added by edo.lee 2013.09.09
            if(direction == AppEngineQMLConstants::JOG_UP)
            {
                return;// added for ITS 228218 2014.03.06
            }

         //modified by taihyun.ahn for ITS 218397 2014.01.08
    }
    // } added by Sergey 20.04.2013

	// removed by Sergey 01.05.2013 for Tune Functionality    

   if ( status == AppEngineQMLConstants::KEY_STATUS_PRESSED )
   {
      setFocusActive( true );
   }
   else if ( status == AppEngineQMLConstants::KEY_STATUS_RELEASED )
   {
      setFocusActive( false );
   }

   if ( !m_isFocusVisible )
   {
       initializeFocus( direction, status );
       //return; // deleted by lssanh 2013.06.27 ITS174032
   }

   if ( m_isFocusVisible )
   {
      if ( m_focusIndex < 0 || m_focusIndex >= m_focusSequence.length() )
      {
         MP_LOG << "Focus index is out of bounds" << LOG_ENDL;
         return;
      }
      
      if ( direction == AppEngineQMLConstants::JOG_UP ||
            direction == AppEngineQMLConstants::JOG_RIGHT ||
           direction == AppEngineQMLConstants::JOG_DOWN ||
           direction == AppEngineQMLConstants::JOG_LEFT ||
           // { added by yungi 2014.02.14 for ITS 225174
           direction == AppEngineQMLConstants::JOG_TOP_RIGHT ||
           direction == AppEngineQMLConstants::JOG_BOTTOM_RIGHT ||
           direction == AppEngineQMLConstants::JOG_TOP_LEFT ||
           direction == AppEngineQMLConstants::JOG_BOTTOM_LEFT ||
           // } added by yungi 2014.02.14
           direction == AppEngineQMLConstants::JOG_CENTER )
      {
            //modified by taihyun.ahn for ITS 218397 2014.01.08

                forwardJogEvent( m_focusSequence[m_focusIndex], direction, status );

             //modified by taihyun.ahn for ITS 218397 2014.01.08
      }

      if ( direction == AppEngineQMLConstants::JOG_WHEEL_RIGHT ||
           direction == AppEngineQMLConstants::JOG_WHEEL_LEFT )
      {
          if(status == AppEngineQMLConstants::KEY_STATUS_PRESSED)  // modified by Dmitry 15.05.13
              forwardJogEvent( m_focusSequence[m_focusIndex], direction,
                              AppEngineQMLConstants::KEY_STATUS_PRESSED ); // modified by Dmitry 15.05.13
      }
   }
}


// { modified by Sergey 15.08.2013 for ITS#184382
void AppMediaPlayer_Video_ScreenManager::showFocus()
{
    if ( m_focusIndex >= 0 && m_focusIndex < m_focusSequence.length() )
        invokeShowFocus(m_focusSequence[m_focusIndex]);
}


void AppMediaPlayer_Video_ScreenManager::hideFocus()
{
    if ( m_focusIndex >= 0 && m_focusIndex < m_focusSequence.length() )
        invokeHideFocus(m_focusSequence[m_focusIndex]);
}

void AppMediaPlayer_Video_ScreenManager::setDefaultFocus(int arrow)
{
    if(m_focusSequence.isEmpty() || m_focusIndex < 0 || m_focusIndex >= m_focusSequence.length())
        return;

    /*
     *  m_focusSequence may contain ModeArea and Screen or just Screen.
     *  Default focus always inside Screen.
     *  So set focus for Screen which is always last element in m_focusSequence.
     */

    if (m_focusIndex != m_focusSequence.length() - 1)
    {
        // If focus on ModeArea hide it.
        hideFocus();

        // Set index of Screen.
        m_focusIndex = m_focusSequence.length() - 1;
    }

    invokeSetDefaultFocus(m_focusSequence[m_focusIndex], arrow);
}


// { added by Sergey 23.10.2013 for ITS#196877
void AppMediaPlayer_Video_ScreenManager::setFocusFromModeAreaToScreen()
{
    if(m_focusSequence.length() > 1 && m_focusIndex == 0)
    {
        invokeHideFocus(m_focusSequence[m_focusIndex]);
        m_focusIndex = 1;

        updateIsModeAreaFocused(); // added by Sergey 10.01.2014 for ITS#218955
    }
}
// } added by Sergey 23.10.2013 for ITS#196877


void AppMediaPlayer_Video_ScreenManager::invokeShowFocus( QObject *item )
{
    if(item)
   QMetaObject::invokeMethod( item, "showFocus" );
}


void AppMediaPlayer_Video_ScreenManager::invokeHideFocus( QObject *item )
{
    if(item)
   QMetaObject::invokeMethod( item, "hideFocus" );
}


void AppMediaPlayer_Video_ScreenManager::invokeSetDefaultFocus( QObject *item, uint arrow )
{
    if(item)
        QMetaObject::invokeMethod( item, "setDefaultFocus", Q_ARG(QVariant, QVariant(arrow)));
}


void AppMediaPlayer_Video_ScreenManager::forwardJogEvent( QObject *item, uint arrow, int status )
{
    if(item)
   QMetaObject::invokeMethod( item, "handleJogEvent",
                              Q_ARG(QVariant, QVariant(arrow)),
                              Q_ARG(QVariant, QVariant(status)));
}
// } modified by Sergey 15.08.2013 for ITS#184382


void AppMediaPlayer_Video_ScreenManager::initializeFocus( uint direction, int status )
{
    //MP_LOG << LOG_ENDL; // added by Sergey for CR#13466

    if ( m_screenStack.empty() )
    {
        MP_LOG << "Looks like there are no screens available." << LOG_ENDL;
        return;
    }

	 //modified by taihyun.ahn for ITS 221363 2014.01.21
    MP_LOG<< "pbOwner = " << m_pEngineVideo->pbOwner() << "DISP = " << m_disp << LOG_ENDL;
    if(!m_pEngineVideo->seekHKPressed())
    {
        if((m_pEngineVideo->getEngineMain().getCloneState() == CLONE_OFF &&
            (m_disp == VPEnum::VDISP_FRONT && m_isRRC) ||
            (m_disp == VPEnum::VDISP_REAR && !m_isRRC)) && m_pEngineVideo->getEngineMain().isDRSShow())
        {
            return;
        }
    }
    else
    {
        if(status == AppEngineQMLConstants::KEY_STATUS_RELEASED
                || status == AppEngineQMLConstants::KEY_STATUS_CANCELED)
        {
            m_pEngineVideo->setSeekHKPressed(false);
        }
    }
     //modified by taihyun.ahn for ITS 221363 2014.01.21

// modified by Dmitry 15.05.13
    if ( status == AppEngineQMLConstants::KEY_STATUS_PRESSED || status == AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED ||
        (status == AppEngineQMLConstants::KEY_STATUS_RELEASED &&
          ( direction == AppEngineQMLConstants::JOG_UP    || 
            direction == AppEngineQMLConstants::JOG_LEFT  || 
            direction == AppEngineQMLConstants::JOG_RIGHT   )&&
         m_listPlaybackScreens.contains(m_screenStack.top())))
    { // It's just  initializes the focus, in this case focus is shown on the screen.

// removed by Dmitry 18.07.13 for ISV15579

        updateFocusedSequence( m_screenStack.top() );

        if ( m_focusSequence.isEmpty() )
        {
            return;
        }

        m_focusIndex = m_focusSequence.length() - 1;
        setFocusVisible( true );

// removed by Dmitry 16.05.13

        if ( ( m_listPlaybackScreens.contains(m_screenStack.top())
             && ( (direction == AppEngineQMLConstants::JOG_CENTER && status == AppEngineQMLConstants::KEY_STATUS_PRESSED) ||
             (direction == AppEngineQMLConstants::JOG_CENTER && status == AppEngineQMLConstants::KEY_STATUS_RELEASED)||
                  direction == AppEngineQMLConstants::JOG_UP ||  
                  direction == AppEngineQMLConstants::JOG_WHEEL_LEFT  ||
                  direction == AppEngineQMLConstants::JOG_WHEEL_RIGHT ||
                  direction == AppEngineQMLConstants::JOG_LEFT   ||
                  direction == AppEngineQMLConstants::JOG_RIGHT )) )
        { 
            forwardJogEvent( m_focusSequence[m_focusIndex], direction,status);
        }
// modified by Dmitry 15.05.13
    }

}

void AppMediaPlayer_Video_ScreenManager::setFocusVisible( bool value )
{
   if ( m_isFocusVisible != value )
   {
      m_isFocusVisible = value;
      setFocusActive( value );
      emit isFocusVisibleChanged( value );
   }
}

void AppMediaPlayer_Video_ScreenManager::setFocusActive( bool value )
{
   if ( value != m_isFocusActive )
   {
      m_isFocusActive = value;
      emit focusActivityChanged( value );
   }
}

// { added by changjin 2012.12.06 for ISV 64510
void AppMediaPlayer_Video_ScreenManager::setFocusActivefromEngineVideo(bool value)
{
    setFocusActive( value );
}
// } added by changjin 2012.12.06


// A little focus magic: in worst case we have several independent
// screens that need focus behavior (StatusBar, overlay aka ModeArea,
// and the screen itself). In order to move focuses, we save a list
// of the items that should have focuses and expect each of them to
// implement AppCommon focus API. At least showFocus(), hideFocus(),
// setDefaultFocus(), and handleJogEvent() functions are required.
// Once the focus leaves the screen it's QML UI is expected to send
// lostFocus() signal.
bool AppMediaPlayer_Video_ScreenManager::updateFocusedSequence( const QString &screenName )
{   // clear old sequence
    m_focusSequence.clear();

    if ( !m_screens.contains(screenName) )
    {
        MP_LOG << "Unknow screen name" << LOG_ENDL;
        return false;
    }

    // add overlay to focused sequence
    foreach( AppMediaPlayer_Video_Screen *overlay, m_overlays.values() )
    {
       if ( overlay->m_pView != NULL )
       {
          m_focusSequence << overlay->m_pView;
       }
    }

    //MP_HIGH << "screen =" << screenName << "lockout =" << m_isLockoutMode << LOG_ENDL; // added by sjhyun 2013.11.08 for ITS 207579

    //add "screen" view to the sequence of the focus
    // { modified by sjhyun 2013.11.08 for ITS 207579
    if ( m_screens[screenName]->m_pView != NULL)
    {
        if(screenName == AppMediaPlayer_Video_Screens::SCREEN_AUX1_PLAYBACK
                || (screenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU && m_isLockoutMode))
        {
            MP_HIGH << "no screen in focus sequence." << LOG_ENDL;

            m_focusMode = FOCUS_MODE_MODEAREA;

            return true;
        }


       m_focusSequence << m_screens[screenName]->m_pView;
       MP_HIGH << "add screen = " << screenName << " to the sequence" << " lockout =" << m_isLockoutMode << LOG_ENDL; // modified by sjhyun 2013.11.08 for ITS 207579
    }
    // } modified by sjhyun
    else
    {
       MP_HIGH << "screen =" << screenName << " lockout =" << m_isLockoutMode << LOG_ENDL; // added by sjhyun 2013.11.08 for ITS 207579
    }

    m_focusMode = FOCUS_MODE_ALL; // added by sjhyun 2013.11.08 for ITS 207579

    if ( m_focusSequence.isEmpty() )
    {
        MP_LOG << "Sequence of the focus is empty!" << LOG_ENDL;
        return false;
    }
    // Connect to the qml focused item signals
    foreach( QDeclarativeItem *item, m_focusSequence ) {
       connect( item, SIGNAL(lostFocus(int,int)), this, SLOT(handleFocusLost(int,int)), Qt::UniqueConnection ); // modified by Sergey 24.08.2013 for ITS#185556 
    }

    return true;
}

// { added by changjin 2012.08.17 : for CR 12449
int AppMediaPlayer_Video_ScreenManager::getControllerFocusIndex()
{
    MP_LOG << "m_focusIndex = " << m_focusIndex << LOG_ENDL;
    return m_focusIndex; 
};
// } added by changjin

// removed by Sergey 27.08.2013 for ITS#186787

// { added by sjhyun 2013.11.08 for ITS 207579
void AppMediaPlayer_Video_ScreenManager::setupFocusSequence(FOCUS_MODE_T mode, QString screenName)
{
    if(screenName != topScreenName() && mode != m_focusMode)
        return;

    //MP_HIGH << "mode =" << mode << "screen =" << screenName << LOG_ENDL;

    // step 1: destroy old focus state
    if(!m_focusSequence.isEmpty() && m_focusIndex != -1)
    {
        MP_HIGH << "mode =" << mode << " screen =" << screenName << " old m_focusSequence =" << m_focusSequence << LOG_ENDL;

        invokeHideFocus(m_focusSequence[m_focusIndex]);
        disconnect(this, SLOT(handleFocusLost(int,int)));
    }
    else
    {
        MP_HIGH << "mode =" << mode << " screen =" << screenName << LOG_ENDL;
    }

    m_focusSequence.clear();


    // step 2: generate new focus state

    // add Mode Area to focused sequence
    foreach(AppMediaPlayer_Video_Screen *overlay, m_overlays.values())
    {
       if(overlay->m_pView != NULL)
       {
          m_focusSequence << overlay->m_pView;
       }
    }

    if(mode == FOCUS_MODE_ALL)
    {
        //add "screen" view to the sequence of the focus
        if(m_screens[screenName]->m_pView != NULL)
        {
           m_focusSequence << m_screens[screenName]->m_pView;
        }

    }

    if(!m_focusSequence.isEmpty())
    {
        MP_HIGH << "new m_focusSequence =" << m_focusSequence << LOG_ENDL;

        // Connect to the qml focused item signals
        foreach(QDeclarativeItem *item, m_focusSequence)
            connect(item, SIGNAL(lostFocus(int,int)), this, SLOT(handleFocusLost(int,int)), Qt::UniqueConnection); // modified by Sergey 24.08.2013 for ITS#185556

        m_focusIndex = m_focusSequence.length() - 1;

        if(m_focusSequence[m_focusIndex] != NULL)
        {
            invokeSetDefaultFocus(m_focusSequence[m_focusIndex]);
            invokeShowFocus(m_focusSequence[m_focusIndex]);
        }
    }

    m_focusMode = mode;
}
// } added by sjhyun


// { added by Sergey 10.01.2014 for ITS#218955
void AppMediaPlayer_Video_ScreenManager::updateIsModeAreaFocused()
{
    if(!m_screenStack.isEmpty() && m_focusSequence.count() > 1)
    {
        AppMediaPlayer_Video_Screen* topScreen = m_screens.value(m_screenStack.top());
        if(topScreen)
        {
            topScreen->setIsModeAreaFocused(m_focusIndex == 0);
        }
    }
}
// } added by Sergey 10.01.2014 for ITS#218955
