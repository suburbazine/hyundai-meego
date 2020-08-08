#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREEN_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREEN_H

#include <QObject>
#include <QVariant>
#include <QStringList>

enum OPTIONS_T
{
   OPTIONS_MENU_TITLE,
   OPTIONS_LIST,
   OPTIONS_RANDOM,
   OPTIONS_RANDOM_ON,
   OPTIONS_RANDOM_OFF,
   OPTIONS_REPEAT,
   OPTIONS_REPEAT_ALL,
   OPTIONS_REPEAT_ONE,
   OPTIONS_REPEAT_OFF,
   OPTIONS_REPEAT_FOLDER,    // added by ravikanth - 12-09-19
   OPTIONS_CAPTIONS,
   OPTIONS_LANGUAGE,
   OPTIONS_COPY,
   OPTIONS_FILE_INFO,
   OPTIONS_FULL_SCREEN,
   OPTIONS_DISPLAY_SET,
   OPTIONS_SOUND_SET,
   OPTIONS_TITLE_MENU,
   OPTIONS_DISC_MENU,
   OPTIONS_PBC_ONOFF,
   OPTIONS_PBC_MENU,
   OPTIONS_DIRECT_ACCESS,
   OPTIONS_SETTINGS,
   OPTIONS_DIVX_SETTINGS,
   // { modified by ravikanth CR 15994 12-11-22
//   OPTIONS_AUDIO_LANGUAGE_SETTINGS, //{ deleted by yongkyun.lee 20130205 for : ISV 70932
   OPTIONS_VIDEO_CAPTION,
   OPTIONS_VIDEO_SWITCH_CAPTION,
   OPTIONS_VIDEO_VOICE,
   OPTIONS_VIDEO_SWITCH_VOICE,
   OPTIONS_VIDEO_SWITCH_VOICE2,  // added by dongjin 2013.02.27 for ISV 71500
   OPTIONS_VIDEO_ANGLE,
   OPTIONS_VIDEO_SWITCH_ANGLE,
   OPTIONS_VIDEO_TITLE,
//{ added by yongkyun.lee 20130205 for : ISV 70932
   OPTIONS_AUDIO_LANGUAGE_SETTINGS,
   OPTIONS_MENU_MAX
//} added by yongkyun.lee 20130205 
   // } modified by ravikanth CR 15994 12-11-22
};

// { modified by Sergey 16.05.2013
enum EVENTS_T
{
    EVENTS_SHOW_PLAYBACK_ONLY,
    EVENTS_SELECT_PBC,
    EVENTS_PBC_MENU,
    // Mode Area
    EVENTS_SET_COUNTER_VISIBLE,
    EVENTS_SET_CUR_NUMBER,
    EVENTS_SET_TOTAL_NUMBER,
    EVENTS_SET_ICON_VISIBLE,
    EVENTS_SET_ICON,
    EVENTS_SET_RTEXT_VISIBLE,
    EVENTS_SET_RTEXT,
    // 10 
    EVENTS_SET_LIST_BTN_VISIBLE,
    EVENTS_SET_MENU_BTN_VISIBLE,
    EVENTS_SET_TEXT,
    EVENTS_SET_NORMAL,//added by edo.lee 2013.05.20
    EVENTS_SET_SETTING,//added by edo.lee 2013.05.20
    EVENTS_NO_FULLSCREEN_ANIM, // added by Sergey 03.08.2013 for ITS#0180899
    EVENTS_MOUSE_PRESSED,
    EVENTS_MOUSE_RELEASED, // added by Sergey 04.08.2013 for ITS#179312
    EVENTS_RESTART_FULLSCREEN_TIMER, // added by Sergey 31.08.2013 for ITS#186799
    EVENTS_SET_FG_LOADING_MODE, // added by Sergey 07.09.2013 for PRE_FG supoort
    //20
    EVENTS_SET_DVD_ACTIVE_TAP, // added by sjhyun 2013.09.11 for ITS 189090
    EVENTS_RESET_PENDING_POPUP, // added by Sergey 22.09.2013 for ITS#186731
    EVENTS_INIT_SCREEN,	// added by Sergey 23.10.2013 for ITS#196877
    EVENTS_DRS_MODE,  // added by Sergey 27.10.2013 for ITS#198669
    EVENTS_UNDO_SELECT_PBC, // added by cychoi 2014.02.20 for (HMC Request)

    EVENTS_MAX
};
// } modified by Sergey 16.05.2013

// { modified by Sergey for CR#15744, 16015
enum POPUP_TXT_BTN_TYPE_T
{
    POPUP_TXT_BTN_UNSUPPORTED_FILE,
    POPUP_TXT_BTN_NO_VIDEO_ON_USB,
    POPUP_TXT_BTN_NO_VIDEO_ON_JUKEBOX,
    POPUP_TXT_BTN_UNAVAILABLE_TRACK,
    POPUP_TXT_BTN_COPY_EXISTED_FILE,
    POPUP_TXT_BTN_DRM_CONFIRM,
    POPUP_TXT_BTN_DRM_EXPIRED,
    POPUP_TXT_BTN_DRM_NOT_AUTH,
    POPUP_TXT_BTN_CAPACITY_ERROR, // added by wspark 2012.11.20 for CR15380
    POPUP_TXT_BTN_UNAVAILABLE_LANG_CODE, // modified by ravikanth 13-03-13
    POPUP_TXT_BTN_COPY_INPROGRESS, // modified by ravikanth 16-04-13
    POPUP_TXT_BTN_UNSUPPORTED_ALL, // added by Sergey 25.05.2013
    POPUP_TXT_BTN_AUDIO_FORMAT_UNSUPPORTED,
    POPUP_TXT_BTN_RESOLUTION_UNSUPPORTED, // added by Sergey 10.11.2013 new DivX popups
    POPUP_TXT_BTN_DRIVING_REGULATION, // added by cychoi 2014.02.20 for (HMC Request)
    POPUP_TXT_BTN_HIGH_TEMPERATURE, // added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    POPUP_TXT_BTN_MAX
};
// } modified by Sergey for CR#15744, 16015

// { added by Sergey for CR#11607
enum POPUP_TOAST_TYPE_T
{
    POPUP_TOAST_READING_FILE,
    POPUP_TOAST_COPY_COMPLETED,
    POPUP_TOAST_COPY_CANCELED,
    POPUP_TOAST_MAX
};
// } added by Sergey for CR#11607

#define FULL_SCREEN_INTERVAL 5000

class QDeclarativeItem;
class QDeclarativeContext;
class AppMediaPlayer_Video_ScreenManager;

/** @class AppMediaPlayer_Video_Screen
 * @brief Base class for each of the video mode screens.
 *
 * */
class AppMediaPlayer_Video_Screen : public QObject
{
    Q_OBJECT
public:
    AppMediaPlayer_Video_Screen( QObject *parent = NULL );
    virtual ~AppMediaPlayer_Video_Screen();

    /** @brief Method that should return the view name.
     * The only pure virtual method here: each screen must provide the
     * name of a view it wants to operate with.
     * */
    virtual QString viewName() = 0;

    // A few helpers to make implementor's life less difficult
    /** @brief Helper method to show an item in the UI.
     * This method should be used to show an arbitrary item in the UI.
     * The itemName refers to the objectName property of the QML item
     * and works throughout the UI. This method will be able to show
     * statusBar for example defined not event under the video root.
     * */
    void showItem( const QString &itemName );

    /** @brief Helper method to hide an item in the UI.
     * This method should be used to hide an arbitrary item in the UI.
     * The itemName refers to the objectName property of the QML item
     * and works throught the UI. This method will be able to hide the
     * statusBar defined not even under the video root.
     * */
    void hideItem( const QString &itemName );

    /** @brief Helper method to find the child item.
     * This method should be used to get access to the item under the
     * view's hierarchy. The itemName argument refers to the objectName
     * property of the QML item.
     * If item is not found this method will return NULL.
     * */
    QDeclarativeItem *getChildItem( const QString &itemName );

    void setmIsVisible(const bool visible); // added by yungi 2013.06.04 for ITS 167159

public slots:
    /** @brief Method that will be called when screen should be hidden.
     * Note that method is not virtual. To do the screen-specific actions
     * implementation needs to override onHide() slot.
     * */
    void hide();

    /** @brief Method that will be called when screen should be shown.
     * This method will not call anything overridable and meant to be
     * internal for the ScreenManager <-> Screen flow.
     * Note that method is not virtual. To do the screen-specific actions
     * implementation needs to override onShow() slot.
     * @param item The screen view.
     * @param context The view context.
     * */
    void show( QDeclarativeItem *item,
                   QDeclarativeContext *context,
                   QVariant arg );

    /** @brief Method that will be called when screen should be suspended.
     * This method will not call anything overridable and meant to be
     * internal for the ScreenManager <-> Screen flow.
     * */
    void deactivate();
    
    /** @brief Slot called when screen is requested to hide itself.
     * It is garanteed, that by the time this slot called, the protected
     * m_pView and m_pContext fields are valid. However, once the method
     * finishes, they will be set to NULL.
     *
     * Note however, if screen pushes something on top of itself, the
     * screen will neither be hidden nor shown once the screens on top
     * will gone.
     * */
    virtual void onHide() {};

    /** @brief Slot called when screen is requested to show itself.
     * It is garanteed, that by the time this slot called, the protected
     * m_pView and m_pContext fields are valid. Moreover, they are
     * garanteed to be valid till the call of the onHide() method.
     *
     * When the screen becomes visible due to dismiss of the screen
     * on top of this one, the onShow will not be called.
     * */
    virtual void onShow( QVariant arg ) { Q_UNUSED(arg); }

    virtual void onDeactivate() {};

    virtual void onPrevKeyPressed() {};
    virtual void onNextKeyPressed() {};
    virtual void onMenuKeyPressed() {};
    virtual void onBackKeyPressed();
    virtual void onMousePressed();
    virtual void onOptionsItemClicked(int item) { Q_UNUSED(item); };
    virtual void onOptionsCheckBoxSelect ( int item, bool flag ) { Q_UNUSED(item); Q_UNUSED(flag); };

    virtual void onEventReceived(const QString &srcScreenName, const QString &resScreenName,
                                 const int eventId, const QVariant arg)
                                 { Q_UNUSED(srcScreenName); Q_UNUSED(resScreenName);
                                   Q_UNUSED(eventId); Q_UNUSED(arg)}

	void lockoutMode(bool on); //added by edo.lee 2013.02.05
	virtual void onLockoutMode(bool on){};//added by edo.lee 2013.02.05

signals:
    /** @brief Signal emitted by the controller to switch screen.
     * The controller implementation should emit this signal when it
     * wants to transit to another screen.
     *
     * @see AppMediaPlayer_Video_ScreenManager::changeScreen()
     * */
    void changeScreen( const QString &screenName, QVariant arg = QVariant() );

    /** @brief Signal emitted by the controller to push the screen.
     * The controller implementation should emit this signal when it
     * wants another screen to be pushed on top of this one.
     *
     * @see AppMediaPlayer_Video_ScreenManager::pushScreen()
     * */
    void pushScreen( const QString &screenName, QVariant arg = QVariant() );

    /** @brief Signal emitted by the controller to pop the screen.
     * The controller implementation should emit this signal when it
     * wants to remove the screen from the screen stack.
     *
     * @see AppMediaPlayer_Video_ScreenManager::popScreen()
     * */
    void popScreen( QVariant arg = QVariant() );

    /** @brief Signal emitted by the controller to replace the last screen on the desired screen.
     * The controller implementation should emit this signal when it
     * wants to remove the top screen from the screen stack and put the desired screen in its place.
     *
     * @see AppMediaPlayer_Video_ScreenManager::replaceScreen()
     * */
    void replaceScreen( const QString &screenName, QVariant arg = QVariant() );

    /** @brief Helper signal to set an item's property.
     * Signal emitted by the controller to set an item's property.
     * The controller implementation should emit this ssignal when it
     * wants to set item's property
     *
     * @see AppMediaPlayer_Video_ScreenManager::setItemProperty()
     * */
    void setItemProperty( const QString &itemName, const char *property, const QVariant &value );

    /** @brief Signal emitted by the controller to hide the UI of App.
     * The controller implementation should emit this signal when it
     * wants to hide all UI of app.
     *
     * @see AppMediaPlayer_Video_ScreenManager::onHideUI()
     * */
    void hideUI();

    /** @brief Method to send event to another screen via screen manager.
     *
     * @see AppMediaPlayer_Video_ScreenManager::onNotifyScreenManager()
     * */
    void notifyScreenManager(const QString &srcScreenName, const QString &resScreenName,
                             const int eventId, QVariant arg = QVariant());

    /** @brief Signal emitted by the controller to hide all focus from the screen.
     * The controller implementation should emit this signal when it
     * wants to hide all focus from the screen.
     *
     * @see AppMediaPlayer_Video_ScreenManager::hideAllFocus()
     * */
    void hideAllFocus();

    /** @brief Signal emitted when view is loaded and got all
     * necessary contexts. Getting this signal qml can be sure that
     * it has an access to SModel and screen controller.
     * */
    void loadingCompleted();

    void setDefaultFocus(); // added by Sergey 25.10.2013 for new DivX

protected:
    /** @brief Method to set the model to the view.
     * This method should be called once in the controller constructor.
     * The implementation garantees, that that model will be assigned to
     * the view once the screen goes on top.
     * */
    void setModel( QObject *pModel );

    /** @brief Method to set the model to the view.
     * By default property name is 'video_model'.
     * This method allows to set additional models and set names for them.
     * CAUTION: you can set this model when screen is visible only.
     * Otherwise it will not be binded to QML. Be sure that you call
     * setTempModel() when screen goes on top. E.g. inside onShow() func.
     * */
    void setTempModel( QObject *pModel, QString name );

    void showTxtBtnPopUp( POPUP_TXT_BTN_TYPE_T typePopUp );

    void pushToastPopUp( POPUP_TOAST_TYPE_T typePopUp ); // added by Sergey for CR#11607

    void invokeQmlMethod( const QString method, QVariant arg ); // added by Sergey 27.10.2013 for ITS#198669

    // { added by Sergey 10.01.2014 for ITS#218955
    virtual void setIsModeAreaFocused(bool isFocused) { Q_UNUSED(isFocused); }
    bool getIsModeAreaFocused() { return m_isModeAreaFocused; }
    // } added by Sergey 10.01.2014 for ITS#218955

    QDeclarativeItem    *m_pView;
    QDeclarativeContext *m_pContext;

    /** The list of overlays the screen requires.
     * */
    QStringList          m_overlays;
    QString              m_myName; // added by Sergey 31.08.2013 for ITS#186799
    bool                 m_isModeAreaFocused; // added by Sergey 10.01.2014 for ITS#218955

    friend class AppMediaPlayer_Video_ScreenManager;
private:
    QObject             *m_pModel;
    bool                 m_isVisible;


};

Q_DECLARE_METATYPE(POPUP_TXT_BTN_TYPE_T);
Q_DECLARE_METATYPE(POPUP_TOAST_TYPE_T); // added by Sergey for CR#11607

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREEN_H
