#ifndef APPVEXTENGINEQML_H
#define APPVEXTENGINEQML_H
#include "DHAVN_AppFramework_AppEngineQML_Def.h"
#include "DHAVN_AppFramework_QML_Def.h"

class AppVextEngineQMLPrivate;

class AppVextEngineQML : public AppEngineQML
{

   Q_OBJECT

public:

    Q_INVOKABLE void SetVisibleScreens(int, bool);

    /** Constructor */
    AppVextEngineQML(  bool bEnableOpenGl, bool bEnableHandleTouch = false, QObject *pParent = NULL  );

    /** Constructor */
    AppVextEngineQML(  bool bEnableOpenGl, QObject *pParent = NULL );

	/** Constructor */
	AppVextEngineQML(  bool bEnableOpenGl, QString aLogFileName, bool bEnableHandleTouch = false, QObject *pParent = NULL );

    /** Constructor */
    AppVextEngineQML(  bool bEnableOpenGl, QString aLogFileName, QObject *pParent = NULL );

    /** Destructor */
    ~AppVextEngineQML();

    /**
     * Returns current screen visibility by sreen id
     */

    bool GetSceensUsage(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreen);

    /**
     * Load qml files for both screens
     */
    void SetSource( AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes, QString sFrontScreen );

    void SetSource( QString sFrontScreen, QString sRearScreen );

    /**
     * Return context for qml definitions by screen id
     */
    QDeclarativeContext * GetScreenContext(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes = AppEngineQMLConstants::APP_FRONT_SCREEN );

    /**
     * Applications need to oveerride this and provide their application ID which
     * must be defined in @ref APP_ID_T.
     */
    virtual APP_ID_T GetThisAppId() const = 0;

    /**
     * Call show funtion for both qml trees
     */
    void show();
    /**
     * Call show funtion for both qml trees
     */
    void hide();

    /**
     * Functions to set views properties
     */
    void SetAttribute(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes, Qt::WidgetAttribute attribute, bool on = true);
    void SetStyleSheet(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes, const QString & styleSheet );

    /**
     * Functions get declarative view - ONLY TO MEDIA PLAYER
     */
    QDeclarativeView* GetDeclarativeView(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T nScreenTypes );

    /**
     * Some applications may require to resize it's window size.
     */
    void ResizeMainWindow(int width, int height);

    /**
     * Swap screen contents between front and rear
     */
	void SwapScreenContents(bool isRearFGIbox = false);

    void HandleAutoVisibility(uint nScreen);

private slots:

    /**
     * Rear and Front popup responce handles
     */
    void HandlePopupResponseRear( int target, QVariant response );
    void HandlePopupResponseFront( int target, QVariant response );
    /**
     * FG_RESPONCE call
     */
    void SlotCallFgFrontResponce();
    void SlotCallFgRearResponce();

    void DoFrameBufferTransitionFront();
    void DoFrameBufferTransitionRear();

    void HandleTouchViewFrontPort( QEvent *aEvent );
    void HandleTouchViewRearPort( QEvent *aEvent );
private:
    /**
     * Used to handle events. This is used for internal handling and shall not
     * be available for applications.
     */
    virtual bool HandleEvent( Event &aEvent );
    virtual bool HandleEventRear( Event &aEvent );

    /**
     * Enable connections with both qml trees
     */
    void MakeConnections();

    void DoFrameBufferTransition(uint nScreen);

private:
    // future enhancement
    virtual void Reserved_1();
    virtual void Reserved_2();
signals:
    void SendTouchport(QEvent *event);

private:
    AppVextEngineQMLPrivate * d_ptr;
    Q_DECLARE_PRIVATE(AppVextEngineQML)

public:
    void handleDumyPopupResponseFront();
    void handleDumyPopupResponseRear();

};

#endif // APPVEXTENGINEQML_H
