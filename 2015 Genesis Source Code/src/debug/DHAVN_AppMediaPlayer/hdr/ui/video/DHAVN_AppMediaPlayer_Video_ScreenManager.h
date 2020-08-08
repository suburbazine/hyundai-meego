#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENMANAGER_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENMANAGER_H

#include <QObject>
#include <QHash>
#include <QStack>
#include <QStringList>
#include <QVariant>
#include <QList>
#include <QDeclarativeEngine>
#include "../../util/DHAVN_AppMediaPlayer_MediaManager.h"
#include <DHAVN_AppFramework_QML_Def.h> // added by Sergey 24.08.2013 for ITS#185556 
#include "util/video/DHAVN_AppMediaPlayer_VP_Enums.h"	// added by Sergey 28.08.2013 for ITS#186507 

class QDeclarativeView;
class QDeclarativeItem;
class QDeclarativeContext;

class AppMediaPlayer_ViewLoader;
class AppMediaPlayer_Video_Screen;
class AppMediaPlayer_EngineVideo; //added by Sergey 27.09.2013 for ITS#189958
 
/** @class AppMediaPlayer_Video_ScreenManager
 *
 * This class is intended to help in implementation of the screen-based UI.
 * We call the triple of controller, model and a view as a screen.  The idea
 * is to have a UI as a set of screens, which form a stack. The only complex
 * issue here is that we want views to be shared across controllers.
 *
 * For example, playback view could cover playback from any source we have,
 * but have a controllers for each source. Moreover, the 'fullscreen' screen
 * shares the same view, but it pushed on top of the playback screens.
 *
 * In details, the screen manager wraps two primary functionalities: it
 * manages the screen stack and implements the view loading.
 * */
class AppMediaPlayer_Video_ScreenManager : public QObject
{
    Q_OBJECT

    // { modified by Sergey 28.08.2013 for ITS#186507 
public:

    Q_PROPERTY(VPEnum::VDISP_T disp READ disp);
    VPEnum::VDISP_T disp() { return (VPEnum::VDISP_T)m_disp; }

    AppMediaPlayer_Video_ScreenManager( QDeclarativeView *view,
                                       VPEnum::VDISP_T disp,
                                       AppMediaPlayer_EngineVideo* pEngineVideo, //added by Sergey 27.09.2013 for ITS#189958
                                       QObject *parent = NULL);
	// } modified by Sergey 28.08.2013 for ITS#186507 
                                       
    /** @brief Registers the screen in the screen manager.
     * While the class responsibility is to manage the screen stack, but not
     * to create them, this method is used.
     *
     * Make shure to call this for each screen before any use of the
     * public slots here.
     * */
    void registerScreen( const QString &screenName,
                         AppMediaPlayer_Video_Screen *screen );


    /** @brief Preloads the view.
     * By design, each view is loaded on demand. When the screen is shown
     * first time it's view will be loaded. While this may be an issue for
     * the first time launch, it is possible to preload the screen(s) at
     * boot time.
     *
     * @note While no one can forbid to load all screens at boot time, think
     * twice if it is good idea to load even one.
     * */
    void preloadView( const QString &viewName );

    /** @brief Returns the top-level screen.
     * If there is no screen on top it returns NULL (for example when called
     * when the video mode is not active).
     * */
    AppMediaPlayer_Video_Screen *topScreen();

    /** @brief Returns the previous-level screen.
     * If there is no screen on top it returns NULL (for example when called
     * when the video mode is not active).
     * */
    AppMediaPlayer_Video_Screen *previousScreen();
    void checkPrevScreen(QString screen);

    AppMediaPlayer_Video_Screen *getPreviousScreen(int step);// added by lssanh 2013.04.19 ITS158191

    void setContext( QDeclarativeContext *context );
    void setRootItem( QObject *root );

    /** @brief Returns the top-level screen's name.
     * If there is no screen on top it returns empty string (for example when called
     * when the video mode is not active).
     * */
    QString topScreenName();
	QString getPreviousScreenName(int preIdx); // added by lssanh 2013.04.19 ITS158191
	QString firstScreen() { return m_screenStack.first(); } // added by Sergey 22.09.2013 for ITS#186731
    void  clearStack();

	// { modified by Sergey 15.08.2013 for ITS#184382
	
    void hideTopscreen(); // added by kihyung 2013.2.6 for ISV 70642

	// removed by Sergey 07.09.2013 for PRE_FG supoort
    
    void activateTopScreen(QVariant arg = 0); // added by Sergey 24.07.2013

    // ============== Focus Public API =================================

    /** @brief When the argument is NOT NULL video starts to catch jog events from UIListener.
     *         When the argument is NULL video stops to catch jog events from UIListener.
     */
    void setJogSource( QObject * );

    /** @brief Shows focus of active element.
     */
    void showFocus();

    /** @brief Hides focus of active element.
     */
    void hideFocus();

    /** @brief Set default focus of active screen.
     */
    Q_INVOKABLE void setDefaultFocus(int arrow = AppEngineQMLConstants::JOG_DOWN);  // modified by Sergey 29.10.2013 for ITS#198413

    Q_INVOKABLE void setFocusFromModeAreaToScreen(); // added by Sergey 23.10.2013 for ITS#196877

    Q_INVOKABLE void setupFocusSequence(FOCUS_MODE_T mode, QString screen); // added by sjhyun 2013.11.08 for ITS 207579


    bool isFocusVisible() { return m_isFocusVisible; }

    void SendJogEvent( uint direction, int status );//{ added by yongkyun.lee 20130225 for :  ISV 73823

    // ============== EOF Focus Public API =================================

	// } modified by Sergey 15.08.2013 for ITS#184382

    
public slots:
    /** @brief Slot to change the current screen.
     * This slot rolls back the screen stack. If the screen name supplied
     * by an argument exists in the stack, it hides the current one and
     * shows the screen requests. Each screen between the requested and
     * top are dismissed from the screen stack.
     *
     * If there is no screenName available in the screen stack, it hides
     * the current screen, shows the requested screen and cleans up the
     * screen stack. In this case screen stack will contain only the
     * screen requested.
     *
     * The QVariant argument is provided to the screen implementation
     * as is. Screen manager makes no assumption on what it contains.
     * */
    void changeScreen( const QString &screenName, QVariant arg = QVariant() );

    /** @brief Slot to push screen on top of current one.
     * This slot hides the current screen, shows the requested screen and
     * pushes the requested screen on the screen stack.
     *
     * The QVariant argument is provided to the screen implementation as is.
     * Screen manager makes no assumption on what it contains.
     * */
    void pushScreen( const QString &screenName, QVariant arg = QVariant() );

    /** @brief Slot to pop screen from the screen stack.
     * If there are more than one screen on the screen stack, it hides the
     * top screen and shows the one below the current one. If there is only
     * one screen available in the screen stack, it emits the hideUI signal.
     * It is expected that the application should hide completely.
     * */
    void popScreen( QVariant arg = QVariant() ); // modified by Sergey for CR#11607


    /** @brief Slot to change the screen to the previous one in history.
     * If screen history is not empty, this just callse changeScreen() with
     * the proper screen name retreived from the screen history. If the
     * screen history is empty, it just does nothing.
     * @return true If it changed the screen, fals if it did not.
     *
     * The screen history is updated in the following way:
     * It records the bottom-most screen from the screen stack, removing
     * the duplicates meanwhile. 
     * */
    bool revertScreen();

    void replaceScreen( const QString &screenName, QVariant arg = QVariant() );
    void replacePrevScreen( const QString &screenName, int step, QVariant arg = QVariant(), bool bHide = true ); // modified by yungi 2013.09.08 for ITS 186704 // modified by lssanh 2013.05.17 ISV81959 // modified by ravikanth 13-03-13

    /** @brief Helper slot to set an item's property.
     * This slot should be used to update item's property. The itemName
     * refers to the objectName property of the QML item and works
     * throughout the UI. This method is able to update property of the
     * status bar defined not even under the video root.
     * */
    void setItemProperty( const QString &itemName, const char *property, const QVariant &value );

    void onHideUI();

    void onPrevKeyPressed();
    void onNextKeyPressed();
    // { modified by wspark 2012.08.02 for CR12020
    //void onMenuKeyPressed();
    void onMenuKeyPressed(int nKeyType);
    // } modified by wspark

    void onBackKeyPressed();

    /** @brief Method to send event to another screen via screen manager.
     * */
    void onNotifyScreenManager(const QString &srcScreenName,
                               const QString &resScreenName,
                               const int eventId,
                               QVariant arg = QVariant()); // modified by Sergey 07.09.2013 for PRE_FG supoort

	// removed by Sergey 15.08.2013 for ITS#184382
	
    int getScreensCount(){return m_screenStack.count(); };

    int getControllerFocusIndex(); // added by changjin 2012.08.17 : for CR 12449
    void setFocusActivefromEngineVideo(bool value); //added by changjin 2012.12.06 for ISV 64510
    void lockoutMode(bool onShow);//MEDIA_SOURCE_T source);//modified by edo.lee 2013.02.27
    // removed by Sergey 07.09.2013 for PRE_FG supoort
	void fgBgStateChange(bool isFG);

	void closeProgressPopup();
	void closeOptionPopup();
	void closeFullScreen();//added by edo.lee 2013.02.25
    void setIsLockoutMode(bool lockout){m_isLockoutMode = lockout;}//added by edo.lee 2013.02.25
	bool getIsLockoutMode(){return m_isLockoutMode;}//added by edo.lee 2013.02.25
    // removed by cychoi 2013.07.27 for ISV 88393
    // removed by Sergey 31.08.2013 for ITS#187830
        bool getIsRRC(){return m_isRRC;} //added by taihyun.ahn for ITS 218397 2014.01.08
	
signals:
    /** @brief Signal emitted when UI should be hidden.
     * This event happens when the last screen poped itself from the screen
     * stack. Typical case is back button handler in bottom-most screen.
     * */
    void hideUI();

    void isFocusVisibleChanged( bool value );

    void focusActivityChanged( bool value );

	// removed by Sergey 07.09.2013 for PRE_FG supoort
	void fgBgStateChanged(bool isFG);
	void loadLockoutTab(bool on); //modified by edo.lee 2013.03.12
	void popFullScreen();//added by edo.lee 2013.03.27
	// removed by Sergey 07.09.2013 for PRE_FG supoort
	
private slots:
    void onViewLoaded( const QString &viewName,
                       QDeclarativeItem *pView,
                       QDeclarativeContext *pContext );

    /** @brief Slot to add an overlay to the UI.
     * Overlay screen does not participate in the screen hierarchy, but
     * instead is placed on top of top-most screen in the stack. 
     * */
    void addOverlay( const QString &screenName );

    /** @brief Method to remove an overlay from the UI.
     * This method removes the overlay.
     * */
    void removeOverlay( const QString &screenName );


    /** Method invoked by the request from the QML side, when the screen
     * have lost the focus.
     * */
    void handleFocusLost( int direction, int focusId );

    void handleJogEvent( uint direction, int status, bool bRRC = false ); //modified by aettie 20130620 for back key event

    void setFocusActive( bool value = false );

private:
//     Shows screen and attaches screen stack change signals of the screenName
    void showScreen( const QString &screenName,
                     QDeclarativeItem *pView,
                     QDeclarativeContext *pContext,
                     bool bOverlay );
    void hideScreen( const QString &screenName );

    
    // Detaches the screen stack change signals of the screenName
    void deactivateScreen( const QString &screenName );

    // Travers the stack down to screenName detaching and hiding each
    // screen.
    void unwindScreenStack( const QString &screenName );

    // Updates the screen history. It is assumed that the screen stack
    // is valid before call to this method.
    void updateScreenHistory();

    void changeLastScreen( const QString &screenName,
                             QDeclarativeItem *pView,
                             QDeclarativeContext *pContext
                            );

    void updateOverlays( AppMediaPlayer_Video_Screen *screen );

    void updateZOrder( QDeclarativeItem *pView, bool isOverlay );

    void findRootItem();

    // { modified by Sergey 15.08.2013 for ITS#184382
    
    // ========================== Focus Utils ===========================================

    /** @brief Just make focus of given QML visible. No any plays with index. Just show.
     */
    void invokeShowFocus( QObject *item );

    /** @brief Just make focus of given QML NOT visible. No any plays with index. Just hide.
     */
    void invokeHideFocus( QObject *item );

    /** @brief Just set focus to default position of screen. Doesn't show focus.
     */
    void invokeSetDefaultFocus( QObject *item, uint arrow = AppEngineQMLConstants::JOG_DOWN); // modified by sjhyun 2013.11.08 for ITS 207579

    void hideAllFocus();
    void forwardJogEvent( QObject *item, uint arrow, int status );
    void initializeFocus( uint direction, int status );
    void setFocusVisible( bool value );
    bool updateFocusedSequence( const QString &screenName );
    void updateIsModeAreaFocused(); // added by Sergey 10.01.2014 for ITS#218955

    // ========================== EOF Focus Utils ===========================================

	// } modified by Sergey 15.08.2013 for ITS#184382


    AppMediaPlayer_ViewLoader                   *m_pViewLoader;
    QHash<QString,AppMediaPlayer_Video_Screen*>  m_screens;
    QStack<QString>                              m_screenStack;
    QStringList                                  m_screenHistory;
    QHash<QString,AppMediaPlayer_Video_Screen*>  m_overlays;

    enum STACK_ACTION_T {
        STACK_PUSH,
        STACK_CHANGE,
        STACK_REPLACE
    };

    // Screen change request stuff:
    // FIXME: In order to enhance code readability we might wrap this stuff
    // into the structure.
    QString         m_pendingScreen;
    STACK_ACTION_T  m_stackAction;

    QStringList     m_overlaysPending;

    QVariant       m_screenStartupArgument;

    QDeclarativeItem *m_pView;
    QDeclarativeItem *m_pRootItem;
    QDeclarativeItem *m_pStatusBar;

    QList<QDeclarativeItem*>  m_focusSequence;
    int                       m_focusIndex;
    bool                      m_isFocusVisible;
    bool                      m_isFocusActive;
// removed by Dmitry 18.07.13 for ISV15579
    bool                      m_isMenuHardKeyPressed; // added by wspark 2012.08.02 for CR12020
    QObject                  *m_pJogSource;
    QStringList               m_listPlaybackScreens;
    QDeclarativeEngine      *m_pEngine;
	QDeclarativeItem* 		  m_pEmptyView;
	bool 						m_isLockoutMode;//added by edo.lee 2013.02.25
    // removed by cychoi 2013.07.27 for ISV 88393
    // removed by Sergey 31.08.2013 for ITS#187830
    VPEnum::VDISP_T           m_disp;	// added by Sergey 28.08.2013 for ITS#186507 
    AppMediaPlayer_EngineVideo* m_pEngineVideo; //added by Sergey 27.09.2013 for ITS#189958
    FOCUS_MODE_T              m_focusMode; // added by sjhyun 2013.11.08 for ITS 207579
    bool                      m_isRRC; //added by taihyun.ahn for ITS 218397 2014.01.08
};	

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENMANAGER_H
