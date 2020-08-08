/**
 * DHAVN_AppFramework_AppEngineBase.h
 *
 * Declaration of the class AppEngineBase
 */

#ifndef APP_ENGINE_BASE_H
#define APP_ENGINE_BASE_H

#include "DHAVN_AppFramework_Def.h"
#include "DHAVN_AppFramework_Event_Def.h"
#include "DHAVN_AppFramework_Logger_Def.h"
#include <DHAVN_AppFramework_LocalSocket.h>
//#include "DHAVN_AppFramework_StatusAdapter.h"
#include <QApplication>
#include <QWidget>

#define REMOVE_APPTOPWIDGET

#define WINDOW_MANAGER_RAISE
//#define RAISE_WINDOW
#define REMOVE_TRANSITION

#define REMOVE_AUTOVISIBILITY

//Private class declaration
class AppEngineBasePrivate;

#include <QDeclarativeView>

#define DD_VEXT_SUPPORTED
/**
 * Abstract base class for application engines based on the AppFramework. They
 * are two kinds of engines derived from this:
 * <ul>
 * <li>AppEngineQML - For QML-based applications</li>
 * <li>AppEngineQT - For applications using standard Qt widgets</li>
 * </ul>
 * This class and its derived classes provide basic application management. It does
 * the communication with the UI State Manager via D-Bus implicitly.
 */

//class StatusAdapter;

class APP_FRAMEWORK_EXPORT AppEngineBase: public QObject
{
    Q_OBJECT

public:

    enum APP_ENGINE_TYPE
    {
       APP_BASE = 0,
       APP_QML,
       APP_QML_VEXT,
       APP_QT
    };

    /**
     * List of GStreamer Pipelines Type
     */
    enum GST_PIPELINES_T
    {
        GP_DISPLAY_FRONT = DISPLAY_FRONT,
        GP_DISPLAY_REAR = DISPLAY_REAR,
        GP_DISPLAY_CAMERA = 11,
    };

    /**
      * List of Display Setting Type
      */
    enum APP_DISPLAY_SETTING_T {
        ADS_CAMERA = 0,
        ADS_MOBILETV,
        ADS_AVP_JUKEBOX,
        ADS_AVP_USB,
        ADS_AVP_DISC,
        ADS_AVP_AUX,
        ADS_QUICKGUIDE,
		ADS_CONNECTED_CAR,
        ADS_MAX
    };

    /** Constructor */
    AppEngineBase( QWidget *pQAppWidget, QObject *pParent = NULL, APP_ENGINE_TYPE nType = APP_BASE );

    /** Constructor */
    AppEngineBase( QWidget *pQAppWidget, QString aModuleName, QObject *pParent = NULL, APP_ENGINE_TYPE nType = APP_BASE );

    /** Destructor */
    ~AppEngineBase();

    /**
    * Applications need to oveerride this and provide their application ID which
    * must be defined in @ref APP_ID_T.
    */
    virtual APP_ID_T GetThisAppId() const = 0;

    /**
    * Method used to communicate with the UI State Manager via D-Bus. Applications
    * should never use direct D-Bus communication with the UI State Manager but always
    * post events using NotifyUISH().
    * All the events are defined in @ref EVT_PUBLIC_ID_T. If new events are requires
    * the enumeration needs to be extended.
    *
    * @return False if call was successful. This does not give any hint whether or
    *     not the event itself could be successfully handled.
    */
    bool NotifyUISH( Event &aEvent, DISPLAY_T aDisplayType = DISPLAY_FRONT );
    bool NotifyUISHCommon( Event &aEvent );

    /**
    * Reference to logger instance for this application instance.
    */
    Logger & GetLogger();

    /**
    * Used to retrieve an instance of the UI State Manager.
    * Note that application should never communicate directly with the UI State
    * Manager but always use @ref NotifyUISH.
    */
    //QObject * GetUIStateManager() const;

    /**
    * Returns the widget which holds the application view.
    */
    QWidget * GetAppView() const;

    /**
    * Returns active display for ran application
    */
    static ECountryVariant GetCountryVariant() { return m_CountryVariant; }

    static EVehicleVariants GetVehicleVariant() { return m_VehicleVariant; }

    /**
    * Returns active display for ran application
    */
    static LANGUAGE_T GetLanguage() { return m_Language; }

    /**
    * Returns active display for ran application
    */
    static DISPLAY_T GetDisplayType() { return m_DisplayType; }

    /**
    * Returns system eviroment for ran application
    */
    static uint GetSystemVariant() { return m_SystemVariant; }

    /**
    * Connects to the UI State Manager with arguments.
    * The arguments are the command line parameters the application received
    * in its main function.
    */
    void ConnectToUISH( int& argc, char** argv );

    /**
    * Connects to the UI State Manager without arguments.
    * Command line parameter given by SetAppArgs(...) function
    * internally used to detect connection to front or
    * rear state engine.
    */
    void ConnectToUISH();

    /**
    * set the command line arguments to the AFW
    * for later usage. cleans up already allocated
    * memory
    */
    static void SetAppArgs( int argc, char **pArgv );

    /**
    * getter for stored ARGC command line paramter
    */
   static int GetAppArgc() { return m_Argc; }

   /**
    * getter for stored ARGV command line paramter
    */
    static char** GetAppArgv() { return m_pArgv; }

public:

    /**
    * Connects to the UI State Manager.
    *
    * This needs as precondition parsed arguments. Therefore use ConnectToUISH()
    * which parses the arguments first.
    */
    void ConnectToUIStateManager();

    /**
    * by default AFW handles the EVT_RESPONSE_FG, if UI app
    * uses this function it is able to handle it by its own
    * This expansion is done due to the black screens on media app
    * at switch on process
    * set parameter true in case that app handle the event as follows:
    *    switch ( aEvent.GetEventId() )
    *    {
    *       case EVT_REQUEST_FG:
    *
    *          Event event( EVT_RESPONSE_FG, GetThisAppId(), APP_UI_MANAGER );
    *
    *          NotifyUISH( event );
    */
    void SetAppHandlesResponseFg( bool handleResponseFg ){ m_AppHandlesResponseFg = handleResponseFg; };
    void SetBlendingOnOf(bool HandleBlendingOnOff){m_AppHandlesBlendingFg = HandleBlendingOnOff;};
    /**
    * get back the current state of switch for delayed UI
    *
    */
    bool GetAppHandlesResponseFg( void ){ return m_AppHandlesResponseFg; };
    bool GetBlendingOnOf(void){return m_AppHandlesBlendingFg;};
    /**
    * get back the current instance StatusAdapterI
    */

    //StatusAdapter* GetStatusAdapter(){ return m_pStatusAdapter; };

    /**
    * provides an opportunity to change geometry of widget
    */
    Q_INVOKABLE void SetGeometry( int x, int y, int width, int height );

    void SetAutoScreenVisibility( bool state = true );

    void SetTouchLock( bool state );

    void SetHUSeekLock(bool state);

    void SetRRCSeekLock(bool State);

    void SetSWRCSeekLock(bool State);

    bool IsSeekValid(Event aEvent, EVT_ID_T typeA, EVT_ID_T typeB);
    /**
    * Play BEEP sound.
    */
    void PlayBeep(uint beepType = 0);
	
     /**
	 * Send To 0.0 Touch Point To Kernel
	 */
	void SendTouchCleanUpForApps();
	
     /**
     * get the swap dc enable/disable status
     */
    Q_INVOKABLE bool IsSwapDCEnabled();

    /**
    * provides an opportunity for sending command to AppStatusBar
    */
    Q_INVOKABLE void StatusBarPostEventSend( int nCMD, int nDispType );

    /**
    * provides an opportunity for sending OSD clear update event to AppStatusBar
    */
    //Q_INVOKABLE void StatusBarClearOSD( int nDispType );

    /**
    * Returns country variant for ran application
    */
    Q_INVOKABLE static int GetCountryVariantFromQML() { return (int)m_CountryVariant; }

    Q_INVOKABLE static int GetVehicleVariantFromQML() { return (int)m_VehicleVariant; }

    Q_INVOKABLE static int GetLanguageFromQML() { return (int)m_Language; }

    /**
     * notify when other application realases GStreamer Pipelines.
     * use force if not released after 5 secs.
     * you must override HandleGstPipelinesAvailable method
     * @see HandleGstPipelinesAvailable
     *
     * @param nGpType Type of GStreamer Pipelines ( @see GST_PIPELINES_T )
     */
    void NotifyWhenGstPipelinesAvailable(int nGpType);

    /**
     * get GStreamer Piplines Status
     *
     * @param nGpType Type of GStreamer Pipelines ( @see GST_PIPELINES_T )
     * @return true if using, false if not using
     */
    bool GetGstPipelinesStatus(int nGpType);

    /**
     * set GStreamer Piplines Status
     *
     * @param nGpType Type of GStreamer Pipelines ( @see GST_PIPELINES_T )
     * @param bUse true if using, false if not using
     */
    void SetGstPipelinesStatus(int nGpType, bool bUse);

    /**
     * get the owner of GStreamer Piplines
     *
     * @param nGpType Type of GStreamer Pipelines ( @see GST_PIPELINES_T )
     * @param application ID ( @see APP_ID_T )
     */
    int GetGstPipelinesOwner(int nGpType);

   /**
     * set a display setting of the video application
     *
     * @param type Type of Display Setting ( @see APP_DISPLAY_SETTING_T )
     * @param brightness value of brightness
     * @param contlast value of brightness
     * @param tint value of brightness
     * @param hue value of brightness
     */ 
    void SetAppDisplaySetting( APP_DISPLAY_SETTING_T type, unsigned int brightness, unsigned int contrast, unsigned int tint, unsigned int hue );
    unsigned int GetDisplayPlane(APP_ID_T appId);

    void SetFrontFullScreen(bool bFullScreen);
    bool GetFrontFullScreen();
    void SetRearFullScreen(bool bFullScreen);
    bool GetRearFullScreen();
    
signals:
   void changedJogKeyPressed();
   void changedJogKeyReleased();
   void changedTouchPressed();
   
private slots:
    void CheckFrontGstPipelines();
    void CheckRearGstPipelines();
    void CheckCameraGstPipelines();

protected:

    /**
    * The derived classes AppEngineQT and AppEngineQML implement this.
    */
   virtual bool HandleEvent( Event &aEvent ) = 0;

   virtual bool HandleEventRear( Event &aEvent ) = 0;

   virtual bool eventFilter( QObject *pFromObj, QEvent *pEvent );

   /**
    * return true if ignore
    */
   bool IgnoreKeyWhenTouched(Event &aEvent, DISPLAY_T Display);
   void CheckHWKeyStatus(Event &aEvent);
   bool IsIgnorableKeyEvent(int eventId);
   bool IsIgnorableKeyWhenSystemPopup( DISPLAY_T aScreen, EVT_ID_T nEventID );
   int GetCloneMode();
   void SendTouchCleanUp();
   void CancelHwKey(DISPLAY_T aScreen, bool WithOutSeek = false);

   enum FB_TR_REASON
   {
      FB_TR_NONE = 0,
      FB_TR_DO_CLONE,
      FB_TR_DO_FRONT_CLONE,
      FB_TR_DO_REAR_CLONE,
      FB_TR_DO_SWAP_DC,
      FB_TR_CLONED,
      FB_TR_NORMAL,
      FB_TR_MAX,
   };
   void SetFrontApp(int app);
   void SetRearApp(int app);
   void SetFrontGuiApp(int app);
   void SetRearGuiApp(int app);
   void SetFrontEntry(int entry);
   void SetRearEntry(int entry);
   void SetFrontRenderAppInBG(int app);
   void SetRearRenderAppInBG(int app);
   void SetFrontRenderEntryInBG(int entry);
   void SetRearRenderEntryInBG(int entry);
   void SetHwKeyPressed(bool Value, Event &aEvent, EVT_ID_T eid);

   int GetFrontApp();
   int GetRearApp();
   int GetFrontGuiApp();
   int GetRearGuiApp();
   int GetFrontEntry();
   int GetRearEntry();
   int GetFrontRenderAppInBG();
   int GetRearRenderAppInBG();
   int GetFrontRenderEntryInBG();
   int GetRearRenderEntryInBG();

   int GetRealAppId(DISPLAY_T display);

   bool IsVideoApp(int app);
   bool IsDisplaySetting(int entry);
   bool IsDRS();
   bool IsCameraEnabled();
   bool isUsedCancelKey(Event &aEvent);
      
   void DoFrontFBTransition(FB_TR_REASON reason);
   void DoRearFBTransition(FB_TR_REASON reason);

   void UpdateStatusbarOnPopupEvent(Event &aEvent, bool bFullScreen);

public slots:

    /**
    * Slot for applications to receive messages/events from the UI State Manager
    */
    virtual void NotifyFromUIStateManager( Event aEvent ) = 0;

    /**
    * Slot for applications to receive messages/events from the UI State Manager
    */
    virtual void NotifyFromUIStateManagerRear( Event aEvent ) = 0;

    /**
    * Slot for applications to receive messages/events from the UI State Manager
    */
    virtual void NotifyFromUIStateManagerCommon( Event aEvent ) = 0;

   /**
    * Slot for the AppEngine uses to receive events from the UI State Manager.
    */
   void NotifyFromUIStateManagerBase( Event aEvent, uint aScreenId  );

private:
    /**
    * The arguments are the command line parameters tghe application received
    * in its main function.
    */
    static void ParseAppArguments( int &argc, char **argv );
    /**
    * clean up allocated memory for command line parameter
    */
    static void DestroyAppArgs();
    /**
    * check event whether event is jog event.
    */
    bool IsJogEvent( EVT_ID_T nEventID );
    bool IsCCPEvent( EVT_ID_T nEventID );
    bool IsRRCEvent( EVT_ID_T nEventID );
    bool IsRRCKey(int key);

    /**
    * receive messages/events from the UI State Manager
    */
    void HandleNotifyFromUIStateManager( DISPLAY_T nScreenID, Event aEvent );
    void HandleNotifyFromUIStateManagerRear( DISPLAY_T nScreenID, Event aEvent );
    void HandleNotifyFromUIStateManagerCommon( DISPLAY_T nScreenID, Event aEvent );

protected slots:

    void SendFGResponse(uint nScreen);
    /**
    * Slot for the AppEngine uses to receive events from the StatusAdapter.
    */

    //void StatusControlAdapterListener( STATUS_CONTROL_EVENT sEvent );
    //void StatusControlAdapterListener( STATUS_CONTROL_EVENT sEvent, DISPLAY_T nDisplay );

private:
    void SetGstPipelinesStatusInternal(int nGpType, bool bUse);
    void HandleAutoVisibility(uint nScreen);

    bool IsOverlayUse( int uiGuiEntryPoint );
    bool IsVideoPlayerEntry(int entry);
    bool IsPhotoEntry(int entry); 
    int GetRealEntry(DISPLAY_T display);
    APP_DISPLAY_SETTING_T TranslateToDisplayAppID(int appId, int entryId);
    bool GetAppDisplaySetting( APP_DISPLAY_SETTING_T type, unsigned int *brightness, unsigned int *constrast, unsigned int *tint, unsigned int *hue );

    void SetDisplayPlane(APP_ID_T appId, unsigned int displayPlane);
    void CheckDRS(Event &evt, uint nScreen);
    
    void BlendOverlayFb(int aSet, int aDisplayTarget, bool needSwap = false);
    void BindOverlayFb(int aSet, int aDisplayTarget);
    void ChangeOverlay(uint nScreen, int displayPlane);
    
public:
    /**
     * handler for NotifyWhenGstPipelinesAvailable
     * must override HandleGstPipelinesAvailable if call NotifyWhenGstPipelinesAvailable.
     * @see NotifyWhenGstPipelinesAvailable
     *
     * @param nGpType Type of GStreamer Pipelines ( @see GST_PIPELINES_T )
     */
    virtual void HandleGstPipelinesAvailable(int nGpType);

protected:
    static char **m_pArgv;

    static int m_Argc;

    static LANGUAGE_T m_Language;

    static DISPLAY_T m_DisplayType;

    static ECountryVariant m_CountryVariant;

    static EVehicleVariants m_VehicleVariant;

    bool m_bAutoVisibility;

    static uint m_SystemVariant;

    bool m_AppHandlesResponseFg;

    bool m_AppHandlesBlendingFg;
    /**
    * get/set popup displayed state.
    */
    bool GetIsDisplayedPopup(DISPLAY_T nDispType);
    void SetIsDisplayedPopup(DISPLAY_T nDispType, bool bMode);

    bool m_bIsDisplayedPopupOnFront;
    bool m_bIsDisplayedPopupOnRear;

private:
    // future enhancement
    virtual void Reserved_1();
    virtual void Reserved_2();

public:
	Q_INVOKABLE int getJogKeyPressed();
	Q_INVOKABLE void setJogKeyPressed(int key);
	Q_INVOKABLE int getJogKeyReleased();
	Q_INVOKABLE void setJogKeyReleased(int key);
	Q_INVOKABLE bool getTouchPressed();
	Q_INVOKABLE void setTouchPressed(bool bTouchPressed);

    // 버전정보를 질의하기 위한 API 
    // Return Value : 
    //   success : 1  
    //   fail : 0 ( 0인 경우 strVersionInfo 에는 오류에 대한 문자열 포함 )  
    int GetVersion( QString &strArgVersionInfo );

private:
    //Private class object for AppEngineBase
    AppEngineBasePrivate * d_ptr;
    Q_DECLARE_PRIVATE(AppEngineBase)
};

#endif // APP_ENGINE_BASE_H
