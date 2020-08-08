#ifndef IBOX_APP_ENGINE_H
#define IBOX_APP_ENGINE_H

// for SW WDT
#include <signal.h>
#include <bits/siginfo.h>

// Qt Header
#include <QDeclarativeView>
#include <QTranslator>
#include <QApplication>
#include <QObject>
#include <QTimer>
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QGst/Ui/VideoWidget>
#include <QGst/Ui/GraphicsVideoSurface>
#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>
#include <QGst/Init>


// DH Header
#include <uistate_manager_def.h>
#include <DHAVN_AppFramework_AppEngineQML_Def.h>
#include <DHAVN_AppFramework_AppVextEngineQML_Def.h>

// Local Heaser
#include "DHAVN_AppIBox_Def.h"
#include "DHAVN_AppIBox_IBoxView.h"
//#include "DHAVN_AppIBox_DualDisplayHelper.h"
#include "DHAVN_AppIBox_IViewWidgetController.h"
#include "DHAVN_AppIBox_CANMessageSender.h"
#include "DHAVN_AppIBox_MOSTMessageHandler.h"
#include "DHAVN_AppIBox_IStatusBarController.h"
#include "DHAVN_AppIBox_Settings.h"
#include "DHAVN_AppIBox_IEventSender.h"
#include "DHAVN_AppIBox_LogUtils.h"

class LocalizeTrigger : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString empty READ empty NOTIFY retrigger);

public:
    QString empty() { return QString(); }

signals:
    void retrigger();
};

/*!
 *  Declaration of @ref IBoxAppEngine.
 *
 * This class handles the acquiring and releasing of the ibox gst pipelines
 * for both front and rear displays. Also used for event notifications
 * to and from UISH.
 */
class IBoxAppEngine: public AppVextEngineQML, public IViewWidgetController, public IStatusBarController, public IEventSender
{
    Q_OBJECT

public:

    /*!
     * Constructor
     * @param aQMLView   Reference to QDeclarativeView object
     * @param aParent    Pointer to parent
     */
    IBoxAppEngine( QTranslator * pTranslator,
                  LANGUAGE_T language,
                   //QElapsedTimer* pTimer,
                  QObject *aParent = NULL );

    /*!
     * Constructor
     * @param aParent    Pointer to parent
     */
    IBoxAppEngine(QObject *parent = 0);

    /*!
     * Destructor
     */
    ~IBoxAppEngine();

    void showStatusBar();
    void hideStatusBar();
    void showOSD(QString &osdCommad);
    bool swapped();
    CLONE_STATUS_T cloneStatus() { return m_eCloneStatus; }

    bool sendEventToUISH( Event &aEvent );
    void sendEventToStatusBar( QString &string );
    void setStatusBarModemState( QString &str );

    void setWidgetShownFlag( bool isShown );
    bool getWidgetShownFlag();
    void setNeedToShowStatusBarFlag( bool isNeed );

    void hideViewWidget();
    void showViewWidget();


    //void setVideoSink( QGst::ElementPtr aVideoSink );
    //void releaseVideoSink();

    Q_INVOKABLE void showDRSPopUp(DISPLAY_T display);
    Q_INVOKABLE void hideDRSPopUp(DISPLAY_T display);
    Q_INVOKABLE void backFromDRSPopup(int screen);
    Q_INVOKABLE void showDLNAList();

    Q_INVOKABLE void setIncrementTrackPosition();
    Q_INVOKABLE void setDecrementTrackPosition();
    Q_INVOKABLE void setPlayPause( bool isPlay );

    Q_INVOKABLE void EnterDRSSoftBackKey();
    void SendDRSHardBackKey();

    // for SW WDT -->
    static void signalHandler (int signum);
    void signalConnection();
    static void setSignalHandlerObject (IBoxAppEngine* m);
    static IBoxAppEngine* SigM;
    void restart(int);

    IBoxView* iBoxView() { return m_pIBoxView; }


protected:

    /*!
    * Returns the Application ID for this APP IBOX
    * @return APP_ID_T Application ID
    */
    APP_ID_T GetThisAppId() const { return APP_IBOX; }

    WId winId();
    //void setGeometry(int ax, int ay, int aw, int ah);

private:

    /*!
    * Acquires the display Gst pipleline for IBox video display
    */
    void AcquireDisplay(AppEngineQMLConstants::EAPP_SCREEN_USAGE_T screen);

    /*!
    * Releases the display Gst pipleline used for IBox video display
    */
    void ReleaseDisplay();
    void PauseDisplay();

    void SetLocalization( LANGUAGE_T languageId );
    bool connectToStatusBar();

    void printCurrentTime();
    QString currentTime();
    //bool addStatusBarArgFromDisplay( QString &string );

    void startResponseFGTimer();
    void sendResponseFG();

    void ControlDRSOnOffLocal(bool isForFrontFG, bool isForDRSOn);

public slots:
    void NotifyFromUIStateManager( Event aEvent );
    void NotifyFromUIStateManagerRear( Event aEvent );
    void NotifyFromUIStateManagerCommon ( Event aEvent );
    void NotifyFromUIStateManagerGeneric( Event &aEvent, AppEngineQMLConstants::EAPP_SCREEN_USAGE_T targetScreen );

    void reInit();

    void onTimeoutResponseFG();
    void onTimeoutResponseFGFrontOnly();
    void onTimeoutResponseFGRearOnly();

Q_SIGNALS:
    void setDeckStatus( uint deckStatus );
    void retranslateUi(int langId);
//    void sigShowVideo();
//    void sigHideVideo();
    void sigStatusBar(bool bShow);

private:

    IBoxView *m_pIBoxView;

    /*!
    * Widget for video sink pipeline
    */
    //QGst::Ui::VideoWidget *m_pViewWidget;
    QGst::Ui::GraphicsVideoSurface *m_pVideoSurface;


    /*!
    * Stores the screen id in which the IBox is launched.
    */
    AppEngineQMLConstants::EAPP_SCREEN_USAGE_T m_currentScreenID;

    //DISPLAY_T getDisplayType();

    bool m_isStatusBarIsVisible;

    CANMessageSender *m_pCanMessageSender;
    MOSTMessageHandler *m_pMostMessageHandler;

    QTranslator *m_pTranslator;
    LANGUAGE_T m_currentLanguage;

    bool m_bIsDrsPopUpShown;

    //DualDisplayHelper *m_pDualDisplayHelper;
    Settings *m_pSettings;

    bool m_bIsWidgetShown;
    RESPONSE_FG_T m_eResponseFG;

    int m_iStatusBarFD;

    bool m_bDCSwapped;

    CLONE_STATUS_T m_eCloneStatus;

    bool m_bFG_F;
    bool m_bFG_R;
    bool m_bFG_Navi;

    APP_ENTRY_POINT_T m_eLastEntryPoint_F;
    APP_ENTRY_POINT_T m_eLastEntryPoint_R;

    bool m_bLocalSwap;

    QString m_strIBoxRSSI;
    bool m_bShowRSSI;

    bool m_bIsVideoInFront;

    IBOX_SCREEN_ID_T m_activeScreen;
    IBOX_SCREEN_ID_T m_DRSPopUpScreen;

    bool m_bIsNeedResponseFG_F;
    bool m_bIsNeedResponseFG_R;
    int m_iResponseFGTime;

};

#endif // IBOX_APP_ENGINE_H
