#include <QDeclarativeContext>
#include <QLocalSocket>
#include <QLocalServer>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <QSignalMapper>
#include "AppGLWidget.h"

#include <DHAVN_AppIBox_AppEngine.h>
#include <DHAVN_AppFramework_Event_Def.h>
#include "DHAVN_AppIBox_LogUtils.h"
#include <QTime>
#include <QtGstQmlSink/qmlgstvideoitem.h>
#include <QtGstQmlSink/qmlpaintervideosurface.h>
#include <QGLWidget>

#include "emgd.h"

#ifndef WM_SOCK_PATH
#define WM_SOCK_PATH	"/tmp/dhavn_wm_srv"
#endif
#define WM_STH	'@'

#define SEND_BACKKEYON_TO_MOSTMANAGER

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
IBoxAppEngine::IBoxAppEngine( QTranslator * pTranslator, LANGUAGE_T language, /*QElapsedTimer* pTimer,*/ QObject *pParent )
    : AppVextEngineQML( false, "AppIBox", pParent ),
      m_currentScreenID( AppEngineQMLConstants::APP_ALL_HIDE ),
      m_pTranslator(pTranslator),
      m_currentLanguage(language),
      //m_pDualDisplayHelper(new DualDisplayHelper(GetLogger())),
      //m_pDualDisplayHelper(0),
      m_pSettings(new Settings(GetLogger())),
      m_bIsWidgetShown(false),
      m_eResponseFG(RF_INVALID),
      m_iStatusBarFD(-1),
      m_bDCSwapped(false),
      m_bFG_F(false),
      m_bFG_R(false),
      m_bFG_Navi(false),
      m_bLocalSwap(false),
      m_pVideoSurface(NULL),
      m_bShowRSSI(false),
      m_eLastEntryPoint_F(eAPP_NONE),
      m_eLastEntryPoint_R(eAPP_NONE),
      m_pIBoxView(NULL),
      m_eCloneStatus(CLONE_OFF)
{
    // not use auto response
    m_AppHandlesResponseFg = true;

    this->setNeedToShowStatusBarFlag( true );

    //Logger::Instance()->Log(QString("[Boot Time - iBox] Create GST pipeline start = %1").arg(pTimer->elapsed()) ;

    m_pIBoxView = new IBoxView(GetLogger(), this, this, m_pSettings);


    QDeclarativeView *pView = NULL;
    pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);

//    AppGLWidget *glWidget = AppGLWidget::instance();
//    pView->setViewport(glWidget);
    pView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    m_pVideoSurface = new QGst::Ui::GraphicsVideoSurface(pView);
    pView->rootContext()->setContextProperty(QLatin1String("videoSurface"), m_pVideoSurface);
    QGst::init();

    if (m_pIBoxView)
    {
        m_pIBoxView->setVideoSink(m_pVideoSurface->videoSink(), pView);

        // skip for FG event (ITS 0221042)
        //m_pIBoxView->init();
    }

    //Logger::Instance()->Log(QString("[Boot Time - iBox] Create GST pipeline end = %1").arg(pTimer->elapsed()) ;

    //m_pViewWidget = new QGst::Ui::VideoWidget();

    //m_pViewWidget->hide();
    //m_pViewWidget->lower();
    //m_pViewWidget->setStyleSheet("background-color:black");
    //m_pViewWidget->setAttribute(Qt::WA_DontShowOnScreen, false);
    //hideViewWidget();

    //QGst::init();

    connectToStatusBar();

    //Logger::Instance()->Log(QString("[Boot Time - iBox] Create Can Handler start = %1").arg(pTimer->elapsed()) ;
    m_pCanMessageSender = new CANMessageSender(GetLogger(), this);
    //Logger::Instance()->Log(QString("[Boot Time - iBox] Create Can Handler end = %1").arg(pTimer->elapsed()) ;

    //Logger::Instance()->Log(QString("[Boot Time - iBox] Create MOST Handler start = %1").arg(pTimer->elapsed()) ;
    m_pMostMessageHandler = new MOSTMessageHandler(GetLogger(), this, this, m_pCanMessageSender, this);
    //Logger::Instance()->Log(QString("[Boot Time - iBox] Create MOST Handler end = %1").arg(pTimer->elapsed()) ;

    // for SW WDT
    signalConnection();
    setSignalHandlerObject(this);

    m_pMostMessageHandler->requestStatusBarIndicatorInfo();

    emgd_init();

    m_bIsDrsPopUpShown = false;


    m_bIsVideoInFront = true;

    m_activeScreen = IBOX_FRONT_SCREEN;
    m_DRSPopUpScreen = IBOX_NONE_SCREEN;

    m_bIsNeedResponseFG_F = false;
    m_bIsNeedResponseFG_R = false;
    m_iResponseFGTime = 250;
}


//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
IBoxAppEngine::~IBoxAppEngine()
{
    ReleaseDisplay();

    if (m_pSettings) { delete m_pSettings; }

    //if (m_pDualDisplayHelper) { delete m_pDualDisplayHelper; }

    if (m_pVideoSurface) { delete m_pVideoSurface; }
    //if( m_pViewWidget ) { delete m_pViewWidget; }

    if (m_pIBoxView) { delete m_pIBoxView; }

    if (m_pCanMessageSender) { delete m_pCanMessageSender; }

    if (m_pMostMessageHandler) { delete m_pMostMessageHandler; }

    if ( m_iStatusBarFD >= 0) { ::close(m_iStatusBarFD); }

    gst_deinit();
}

void IBoxAppEngine::reInit()
{
    LW_LOG(" --> ");

    m_pIBoxView->release();

    if (m_pVideoSurface)
    {
        delete m_pVideoSurface;
        m_pVideoSurface = NULL;
    }


    QDeclarativeView *pView = NULL;
    pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);

    AppGLWidget *glWidget = AppGLWidget::instance();
    pView->setViewport(glWidget);

    m_pVideoSurface = new QGst::Ui::GraphicsVideoSurface(pView);
    pView->rootContext()->setContextProperty(QLatin1String("videoSurface"), m_pVideoSurface);


    m_pIBoxView->setVideoSink(m_pVideoSurface->videoSink(), pView);
    m_pIBoxView->init();

    if (m_bFG_F || m_bFG_R)
        m_pIBoxView->playPipeLine();


    LW_LOG(" <-- ");
}

void IBoxAppEngine::ControlDRSOnOffLocal(bool isForFrontFG, bool isForDRSOn)
{
    HS_LOG("4FrontFG:" << isForFrontFG << ", 4DRSOn:" << isForDRSOn << ", FG_F:" << m_bFG_F << ", FG_R:" << m_bFG_R << ", DCSwapped:" << m_bDCSwapped << ", LocalSwap:" << m_bLocalSwap << ", IsDrsShown:" << m_bIsDrsPopUpShown << ", act_Scr:" << m_activeScreen << ", DRS_Scr:" << m_DRSPopUpScreen);

    QDeclarativeView* pView =  NULL;

    if (isForFrontFG) {
        if (m_bDCSwapped) {
            if (isForDRSOn) {
                LW_LOG("[DRS on - SwapDC, FGToFront] 1. Do Nothing.");
            }
            else {
                if (m_DRSPopUpScreen==IBOX_FRONT_SCREEN) {
                    SwapScreenContents();
                    m_bLocalSwap = !m_bLocalSwap;
                    m_activeScreen = IBOX_FRONT_SCREEN;

                    pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
                    if (pView) pView->show();

                    //m_bIsVideoInFront = true;
                    hideDRSPopUp(DISPLAY_REAR);
                    m_DRSPopUpScreen = IBOX_NONE_SCREEN;
                    LW_LOG("[DRS off - SwapDC, FGToFront] 2. DRS PopUp is shown. Do hide DRS in front and Swap locally.");
                }
                else {
                    if (m_bDCSwapped) {

                        if (m_activeScreen!=IBOX_FRONT_SCREEN) {

                            if (m_DRSPopUpScreen==IBOX_REAR_SCREEN) {
                                showDRSPopUp(DISPLAY_FRONT);
                                LW_LOG("[DRS off - SwapDC, FGToFront] 3.00 show DRS popup in front.");
                            }

                            SwapScreenContents(true);
                            m_bLocalSwap = !m_bLocalSwap;
                            m_activeScreen = IBOX_FRONT_SCREEN;

                            if (m_DRSPopUpScreen==IBOX_REAR_SCREEN) {
                                hideDRSPopUp(DISPLAY_FRONT);
                                LW_LOG("[DRS off - SwapDC, FGToFront] 3.01 hide DRS popup in front.");
                            }

                            pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
                            if (pView) pView->show();

                            LW_LOG("[DRS off - SwapDC, FGToFront] 3.02 Hide DRS in rear and Swap locally and show DRS in rear.");
                        }
                        else {
                            LW_LOG("[DRS off - SwapDC, FGToFront] 3A. Do Nothing.");
                        }
                    }
                    else {
                        LW_LOG("[DRS off - SwapDC, FGToFront] 3B. Do Nothing.");
                    }
                }
            }
        }
        else { //No SwapDC
            if (isForDRSOn) {
                if (m_activeScreen==IBOX_FRONT_SCREEN) {

                    if (m_DRSPopUpScreen==IBOX_NONE_SCREEN) {
                        showDRSPopUp(DISPLAY_FRONT);
                        m_DRSPopUpScreen=IBOX_FRONT_SCREEN;

                        LW_LOG("[DRS on - noSwapDC, FGToFront] 4-00. DRS PopUp is not shown. Do show DRS in Front.");
                    }
                    else if (m_DRSPopUpScreen==IBOX_REAR_SCREEN) {
                        showDRSPopUp(DISPLAY_FRONT);
                        m_DRSPopUpScreen=IBOX_FRONT_SCREEN;
                        hideDRSPopUp(DISPLAY_REAR);

                        LW_LOG("[DRS on - noSwapDC, FGToFront] 4-01. DRS PopUp is shown in rear. Do show DRS in Front and hide rear DRS.");
                    }
                    else { //DRS is already shown in front.
                        LW_LOG("[DRS on - noSwapDC, FGToFront] 4. DRS is already shown in front. Do Nothing.");
                    }

                }
                else { // m_activeScreen == IBOX_REAR_SCREEN;
                    if (m_bFG_R) {
                        showDRSPopUp(DISPLAY_FRONT);
                        m_DRSPopUpScreen=IBOX_FRONT_SCREEN;
                        m_iResponseFGTime = 10;
                        LW_LOG("[DRS on - noSwapDC, FGToFront] 4A. Show DRS in front.") ;
                    }
                    else {
                        if (m_DRSPopUpScreen==IBOX_NONE_SCREEN) {
                            showDRSPopUp(DISPLAY_FRONT);
                            m_DRSPopUpScreen=IBOX_FRONT_SCREEN;
                            LW_LOG("[DRS on - noSwapDC, FGToFront] 5A. Show DRS in front.");
                        }
                        else LW_LOG("[DRS on - noSwapDC, FGToFront] 5B. Do Nothing.");
                    }
                }
            }
            else { //DRS off
                if (m_DRSPopUpScreen==IBOX_FRONT_SCREEN) {

                    if (m_bFG_R) {
                        pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
                        if (pView) pView->hide();
                        LW_LOG("[DRS off - noSwapDC, FGToFront] 60. Hide front now.") ;
                        hideDRSPopUp(DISPLAY_FRONT);
                        m_DRSPopUpScreen = IBOX_NONE_SCREEN;

                        pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
                        if (pView) pView->show();

                        SwapScreenContents();
                        m_bLocalSwap = !m_bLocalSwap;
                        m_activeScreen = IBOX_FRONT_SCREEN;
                    }
                    else {


                        if (m_activeScreen!=IBOX_FRONT_SCREEN) {
                            SwapScreenContents();
                            m_bLocalSwap = !m_bLocalSwap;
                            m_activeScreen = IBOX_FRONT_SCREEN;

                            pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
                            if (pView) pView->show();

                            hideDRSPopUp(DISPLAY_REAR);
                            m_DRSPopUpScreen = IBOX_NONE_SCREEN;

                            LW_LOG("[DRS off - noSwapDC, FGToFront] 6A. DRS PopUp is shown. Do hide DRS in front. IBox screen is located in rear. Swap locally.") ;
                        }
                        else {
                            hideDRSPopUp(DISPLAY_FRONT);
                            m_DRSPopUpScreen = IBOX_NONE_SCREEN;
                            LW_LOG("[DRS off - noSwapDC, FGToFront] 6B. DRS PopUp is shown. Do hide DRS in front.") ;
                        }

                    }

                }
                else {
                    if (m_activeScreen == IBOX_REAR_SCREEN) {
                        if (m_bIsDrsPopUpShown) { //[2014-07-03] Fixed DRSPopUp Not Hide issue
                            hideDRSPopUp(DISPLAY_REAR);
                            m_DRSPopUpScreen = IBOX_NONE_SCREEN;
                            LW_LOG("[DRS off - SwapDC, FGToRear] 7.0. Hide DRSPopUp in Rear.") ;
                        }
                        SwapScreenContents();
                        m_bLocalSwap = !m_bLocalSwap;
                        m_activeScreen = IBOX_FRONT_SCREEN;

                        pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
                        if (pView) pView->show();
                        LW_LOG("[DRS off - noSwapDC, FGToFront] 7A. IBox screen is located in rear. Swap locally.") ;
                    }
                    else {
                        if (m_bIsDrsPopUpShown) { //[2014-07-03] Fixed DRSPopUp Not Hide issue
                            hideDRSPopUp(DISPLAY_REAR);
                            m_DRSPopUpScreen = IBOX_NONE_SCREEN;
                            LW_LOG("[DRS off - SwapDC, FGToRear] 7B. Hide DRSPopUp in Rear.") ;
                        }
                        else
                            LW_LOG("[DRS off - noSwapDC, FGToFront] 7C. Do Nothing.") ;
                    }
                }
            }
        }
    }
    else { //For Rear FG
        if (m_bDCSwapped) {
            if (isForDRSOn) {
                if (m_activeScreen==IBOX_REAR_SCREEN) {

                    if (m_DRSPopUpScreen==IBOX_NONE_SCREEN) {
                        showDRSPopUp(DISPLAY_REAR);
                        m_DRSPopUpScreen=IBOX_REAR_SCREEN;

                        LW_LOG("[DRS on - SwapDC, FGToRear] 8-00. DRS PopUp is shown in rear. Show DRS in rear.") ;;
                    }
                    else if (m_DRSPopUpScreen==IBOX_FRONT_SCREEN) {

                        showDRSPopUp(DISPLAY_REAR);
                        m_DRSPopUpScreen=IBOX_REAR_SCREEN;
                        hideDRSPopUp(DISPLAY_FRONT);

                        LW_LOG("[DRS on - SwapDC, FGToRear] 8-01. DRS PopUp is shown in front. Show DRS in front and hide in rear.") ;
                    }
                    else { // DRS is Rear screen
                        LW_LOG("[DRS on - SwapDC, FGToRear] 8-02. DRS PopUp is already shown in rear. Do nothing.") ;
                    }
                }
                else {
                    if (m_bFG_F) {
                        //if (m_DRSPopUpScreen != IBOX_REAR_SCREEN) {
                            showDRSPopUp(DISPLAY_REAR);
                            m_DRSPopUpScreen=IBOX_REAR_SCREEN;
                            LW_LOG("[DRS on - SwapDC, FGToRear] 8A. Show DRS in rear.") ;
                        //}
                        //else {
                        //    LW_LOG("[DRS on - SwapDC, FGToRear] 8. Do Nothing.") ;
                        //}
                        m_iResponseFGTime = 10;
                    }
                    else {
                        if (m_DRSPopUpScreen==IBOX_NONE_SCREEN) {
                            showDRSPopUp(DISPLAY_REAR);
                            m_DRSPopUpScreen=IBOX_REAR_SCREEN;
                            LW_LOG("[DRS on - SwapDC, FGToRear] 9A. Show DRS in rear.");
                        }
                        else LW_LOG("[DRS on - SwapDC, FGToRear] 9B. Do Nothing.");
                    }
                }
            }
            else { //DRS off
                if (m_DRSPopUpScreen==IBOX_REAR_SCREEN) {

                    if (m_activeScreen==IBOX_REAR_SCREEN) {
                        hideDRSPopUp(DISPLAY_REAR);
                        m_DRSPopUpScreen = IBOX_NONE_SCREEN;
                        LW_LOG("[DRS off - SwapDC, FGToRear] 10. DRS PopUp is shown. Do hide DRS in rear.") ;
                    }
                    else {
                        SwapScreenContents();
                        m_bLocalSwap = !m_bLocalSwap;
                        m_activeScreen = IBOX_REAR_SCREEN;

                        pView = GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
                        if (pView) pView->show();

                        hideDRSPopUp(DISPLAY_FRONT);
                        m_DRSPopUpScreen = IBOX_NONE_SCREEN;
                        LW_LOG("[DRS off - SwapDC, FGToRear] 10A. DRS PopUp is shown. Swap locally and Do hide DRS in rear.") ;
                    }
                }
                else {
                    if (m_activeScreen==IBOX_FRONT_SCREEN) {
                        if (m_bIsDrsPopUpShown) { //[2014-07-03] Fixed DRSPopUp Not Hide issue
                            hideDRSPopUp(DISPLAY_FRONT);
                            m_DRSPopUpScreen = IBOX_NONE_SCREEN;
                            LW_LOG("[DRS off - SwapDC, FGToRear] 11.0. Hide DRSPopUp in front.") ;
                        }
                        SwapScreenContents();
                        m_bLocalSwap = !m_bLocalSwap;
                        m_activeScreen = IBOX_REAR_SCREEN;

                        pView = GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
                        if (pView) pView->show();

                        LW_LOG("[DRS off - SwapDC, FGToRear] 11A. IBox screen is located in front. Swap locally.") ;
                    }
                    else {
                        if (m_bIsDrsPopUpShown) { //[2014-07-03] Fixed DRSPopUp Not Hide issue
                            hideDRSPopUp(DISPLAY_FRONT);
                            m_DRSPopUpScreen = IBOX_NONE_SCREEN;
                            LW_LOG("[DRS off - SwapDC, FGToRear] 11B. Hide DRSPopUp in front.") ;
                        }
                        else
                            LW_LOG("[DRS off - SwapDC, FGToRear] 11C. Do Nothing.") ;
                    }
                }
            }
        }
        else { //No SwapDC
            if (isForDRSOn) {
                LW_LOG("[DRS on - NoSwapDC, FGToRear] 12. Do Nothing.") ;
            }
            else {
                if (m_DRSPopUpScreen==IBOX_REAR_SCREEN) {
                    SwapScreenContents();
                    m_bLocalSwap = !m_bLocalSwap;
                    m_activeScreen = IBOX_REAR_SCREEN;

                    pView = GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
                    if (pView) pView->show();

                    //m_bIsVideoInFront = true;
                    hideDRSPopUp(DISPLAY_FRONT);
                    m_DRSPopUpScreen = IBOX_NONE_SCREEN;
                    LW_LOG("[DRS off - NoSwapDC, FGToRear] 13. DRS PopUp is shown. Do hide DRS in front and Swap locally.") ;
                }
                else {
                    if (m_activeScreen == IBOX_FRONT_SCREEN) {

                        if (m_DRSPopUpScreen==IBOX_FRONT_SCREEN) {
                            showDRSPopUp(DISPLAY_REAR);
                            LW_LOG("[DRS off - NoSwapDC, FGToRear] 14.00 show DRS popup in rear.") ;
                        }

                        SwapScreenContents(true);
                        m_bLocalSwap = !m_bLocalSwap;
                        m_activeScreen = IBOX_REAR_SCREEN;

                        pView = GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
                        if (pView) pView->show();

                        LW_LOG("[DRS off - NoSwapDC, FGToRear] 14.01 Swap locally.") ;

                        if (m_DRSPopUpScreen==IBOX_FRONT_SCREEN) {
                            hideDRSPopUp(DISPLAY_REAR);
                            LW_LOG("[DRS off - NoSwapDC, FGToRear] 14.02 hide DRS popup in rear.") ;
                        }


                    }
                    else {
                        LW_LOG("[DRS off - NoSwapDC, FGToRear] 14. Do Nothing.") ;
                    }
                }

            }
        }
    }

    LW_LOG("< ----------------------------------- ") ;
}

//------------------------------------------------------------------------------
// Notify slot for UISH Front display events
//------------------------------------------------------------------------------
void
IBoxAppEngine::NotifyFromUIStateManager( Event aEvent )
{
    switch(aEvent.GetEventId())
    {
    case EVT_REQUEST_PRE_FG:
    {
        HS_LOG ( "EVT_REQUEST_FG ----------> " << currentTime());

        m_eResponseFG = RF_FRONT;
        m_bIsNeedResponseFG_F = true;

        if (!m_bFG_R && !m_bFG_F)
        {
            AcquireDisplay( AppEngineQMLConstants::APP_FRONT_SCREEN );
        }

        // process entry point
        APP_ENTRY_POINT_DATA_T* pEntryPointData = NULL;
        pEntryPointData = (APP_ENTRY_POINT_DATA_T*)aEvent.Data().data();

        if (pEntryPointData)
        {
            HS_LOG ("m_eLastEntryPoint_F:" << m_eLastEntryPoint_F << ", currentEntryPoint:" << pEntryPointData->sAppEntryPoint << ", bTemporalMode:" << pEntryPointData->sHeader.bTemporalMode);
            m_eLastEntryPoint_F = pEntryPointData->sAppEntryPoint;
        }

        m_bFG_F = true;

        ControlDRSOnOffLocal(true, (m_eLastEntryPoint_F == (APP_ENTRY_POINT_T)EIBOX_MODE_IBOX_DRS)? true : false);
        QTimer::singleShot(m_iResponseFGTime, this, SLOT(onTimeoutResponseFGFrontOnly()));

        int result = do_wait_on_2dcopy_blt(1);
        if (result == 0) {
            LW_LOG ( "Wait on 2D Copy Blt Success");
        }
        else {
            CR_LOG ( "Wait on 2D Copy Blt failed");
        }

        QDeclarativeView* pView =  GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN );
        if (pView) pView->repaint();

        break;
    }
    case EVT_REQUEST_BG:
    {
        HS_LOG ( "EVT_REQUEST_BG ------------> " << currentTime());

        m_bFG_F = false;

        if (!m_bFG_F && !m_bFG_R)
        {
            PauseDisplay();

            int result = do_wait_on_2dcopy_blt(0);
            if (result == 0) {
                LW_LOG ( "Wait on 2D Copy Blt Success");
            }
            else {
                CR_LOG ( "Wait on 2D Copy Blt failed");
            }
        }

        if (m_DRSPopUpScreen == IBOX_FRONT_SCREEN) {
            hideDRSPopUp(DISPLAY_FRONT);
            m_DRSPopUpScreen = IBOX_NONE_SCREEN;
        }

        break;
    }

    case EVT_SHOW_IBOX_RSSI_BAR:
    {
        LW_LOG ( "EVT_SHOW_IBOX_RSSI_BAR");

        m_bShowRSSI = true;

        if (!m_strIBoxRSSI.isEmpty())
            sendEventToStatusBar(m_strIBoxRSSI);
    }
        break;

    case EVT_HIDE_IBOX_RSSI_BAR:
    {
        LW_LOG ( "EVT_HIDE_IBOX_RSSI_BAR");

        m_bShowRSSI = false;

        QString pStr = "@STA^HSDPA_RSSI^0^0";
        sendEventToStatusBar(pStr);
    }
        break;

//    default:
//    {
//        NotifyFromUIStateManagerGeneric( aEvent, AppEngineQMLConstants::APP_FRONT_SCREEN );
//        break;
//    }
    }

}

//------------------------------------------------------------------------------
// Notify slot for UISH Rear display events
//------------------------------------------------------------------------------
void
IBoxAppEngine::NotifyFromUIStateManagerRear( Event aEvent )
{
    switch(aEvent.GetEventId())
    {
    case EVT_REQUEST_PRE_FG:
    {
        HS_LOG ( "EVT_REQUEST_FG ----------> " << currentTime());

        m_eResponseFG = RF_REAR;
        m_bIsNeedResponseFG_R = true;

        if (!m_bFG_R && !m_bFG_F)
        {
            AcquireDisplay( AppEngineQMLConstants::APP_REAR_SCREEN );
        }

        // process entry point
        APP_ENTRY_POINT_DATA_T* pEntryPointData = NULL;
        pEntryPointData = (APP_ENTRY_POINT_DATA_T*)aEvent.Data().data();

        if (pEntryPointData)
        {
            HS_LOG ("m_eLastEntryPoint_R:" << m_eLastEntryPoint_R << ", currentEntryPoint:" << pEntryPointData->sAppEntryPoint << ", bTemporalMode:" << pEntryPointData->sHeader.bTemporalMode);
            m_eLastEntryPoint_R = pEntryPointData->sAppEntryPoint;

        }

        m_bFG_R = true;

        ControlDRSOnOffLocal(false, (m_eLastEntryPoint_R == (APP_ENTRY_POINT_T)EIBOX_MODE_IBOX_DRS)? true : false);
        QTimer::singleShot(m_iResponseFGTime, this, SLOT(onTimeoutResponseFGRearOnly()));

        int result = do_wait_on_2dcopy_blt(1);
        if (result == 0) {
            LW_LOG ( "Wait on 2D Copy Blt Success");
        }
        else {
            CR_LOG ( "Wait on 2D Copy Blt failed");
        }

        QDeclarativeView* pView =  GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN );
        if (pView) pView->repaint();

        break;
    }
    case EVT_REQUEST_BG:
    {
        HS_LOG ( "EVT_REQUEST_BG ------------> " << currentTime());

        m_bFG_R = false;

        if (!m_bFG_F && !m_bFG_R)
        {
            PauseDisplay();

            int result = do_wait_on_2dcopy_blt(0);
            if (result == 0) {
                LW_LOG ( "Wait on 2D Copy Blt Success");
            }
            else {
                CR_LOG ( "Wait on 2D Copy Blt failed");
            }
        }

        if (m_DRSPopUpScreen == IBOX_REAR_SCREEN) {
            hideDRSPopUp(DISPLAY_REAR);
            m_DRSPopUpScreen = IBOX_NONE_SCREEN;
        }

        break;
    }

    case EVT_SHOW_IBOX_RSSI_BAR:
    {
        LW_LOG ( "EVT_SHOW_IBOX_RSSI_BAR");

        m_bShowRSSI = true;

        if (!m_strIBoxRSSI.isEmpty())
            sendEventToStatusBar(m_strIBoxRSSI);
    }
        break;

    case EVT_HIDE_IBOX_RSSI_BAR:
    {
        LW_LOG ( "EVT_HIDE_IBOX_RSSI_BAR");

        m_bShowRSSI = false;

        QString pStr = "@STA^HSDPA_RSSI^0^0";
        sendEventToStatusBar(pStr);
    }
        break;


//    default:
//    {
//        NotifyFromUIStateManagerGeneric( aEvent, AppEngineQMLConstants::APP_REAR_SCREEN );
//        break;
//    }
    }

}

//------------------------------------------------------------------------------
// Notify slot for UISH Common events
//------------------------------------------------------------------------------
void IBoxAppEngine::NotifyFromUIStateManagerCommon(Event aEvent)
{
    switch( aEvent.GetEventId() )
    {
    case EVT_FACTORY_RESET_REQUEST:
    {
        LW_LOG("EngineerAppEngine::Request Factory Reset Message From UISH") ;

        Event event(EVT_FACTORY_RESET_RESPONSE, GetThisAppId(), APP_UI_MANAGER);
        if(! NotifyUISHCommon(event))
        {
            CR_LOG( "Factory Reset Response Failed") ;
        }
        break;
    }

    case EVT_LANGUAGE_CHANGED:
    {
        EVENT_LANGUAGE_CHANGED_DATA_T *pLanguageData = (EVENT_LANGUAGE_CHANGED_DATA_T *)aEvent.Data().data();
        if ( pLanguageData != NULL )
        {
            SetLocalization(pLanguageData->languageId);
        }
    }
        break;

    case EVT_SWAP_DC_ENABLED:
    {
        LW_LOG ( "EVT_SWAP_DC_ENABLED");
        m_bDCSwapped = true;
        break;
    }

    case EVT_SWAP_DC_DISABLED:
    {
        LW_LOG ( "EVT_SWAP_DC_DISABLED");
        m_bDCSwapped = false;
        break;
    }

    case EVT_CLONE_NOTIFICATION:
    {
        LW_LOG ( "EVT_CLONE_NOTIFICATION" );

        EVENT_CLONE_NOTIFICATION_DATA_T* pDate = (EVENT_CLONE_NOTIFICATION_DATA_T *)aEvent.Data().data();
        if (pDate)
        {
            m_eCloneStatus = pDate->sCloneStatus;
        }

    }
        break;

    case EVT_SHOW_IBOX_RSSI_BAR:
    {
        LW_LOG ( "EVT_SHOW_IBOX_RSSI_BAR" );

        m_bShowRSSI = true;

        if (!m_strIBoxRSSI.isEmpty())
            sendEventToStatusBar(m_strIBoxRSSI);
    }
        break;

    case EVT_HIDE_IBOX_RSSI_BAR:
    {
        LW_LOG ( "EVT_HIDE_IBOX_RSSI_BAR" );

        m_bShowRSSI = false;

        QString pStr = "@STA^HSDPA_RSSI^0";
        sendEventToStatusBar(pStr);
    }
        break;

//    case EVT_APP_NAVI_FG:
//    {
//        LW_LOG ( "EVT_APP_NAVI_FG" );
//        if ( m_bFG_F || m_bFG_R )
//        {
//            int result = do_wait_on_2dcopy_blt(1);

//            if (result == 0)
//                LW_LOG ( "Wait on 2D Copy Blt Success" );
//            else
//                LW_LOG ( "Wait on 2D Copy Blt failed" );
//        }

//        m_bFG_Navi = true;
//    }
//        break;

//    case EVT_APP_NAVI_BG:
//    {
//        LW_LOG ( "EVT_APP_NAVI_BG" );
//        if ( m_bFG_F || m_bFG_R )
//        {
//            int result = do_wait_on_2dcopy_blt(0);

//            if (result == 0)
//                LW_LOG ( "Wait on 2D Copy Blt Success" );
//            else
//                LW_LOG ( "Wait on 2D Copy Blt failed" );
//        }

//        m_bFG_Navi = false;
//    }
//        break;

    case EVT_KEY_CCP_BACK:
    {
        if (m_bIsDrsPopUpShown) {
            if (!m_bDCSwapped) {
                HS_LOG ( "[Common] EVT_KEY_CCP_BACK with DRS PopUp" );
                KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
                if (message->keyStatus == KEY_STATUS_PRESSED) SendDRSHardBackKey();
            }
            else HS_LOG ( "[Common] EVT_KEY_CCP_BACK with SwapDC, do nothing!!");
        }
        else HS_LOG ( "[Common] EVT_KEY_CCP_BACK without DRS PopUp, do nothing!!");
        break;
    }
    case EVT_KEY_RRC_BACK:
    {
        if (m_bIsDrsPopUpShown) {
            if (m_bDCSwapped) {
                HS_LOG ( "[Common] EVT_KEY_RRC_BACK with DRS PopUp for SwapDC" );
                KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
                if (message->keyStatus == KEY_STATUS_PRESSED) SendDRSHardBackKey();
            }
            else HS_LOG ( "[Common] EVT_KEY_RRC_BACK, do nothing!!" );
        }
        else HS_LOG ( "[Common] EVT_KEY_RRC_BACK without DRS PopUp, do nothing!!" );
        break;
    }

    default:
        break;
    }
}

void IBoxAppEngine::NotifyFromUIStateManagerGeneric( Event &aEvent,
                                                     AppEngineQMLConstants::EAPP_SCREEN_USAGE_T targetScreen )
{
    switch( aEvent.GetEventId() )
    {
    default:
        break;
    }
}

void IBoxAppEngine::SetLocalization( LANGUAGE_T languageId )
{
    if (m_currentLanguage == languageId) // verify on current laguage
        return;

    m_currentLanguage = languageId;

    if ( m_pTranslator == NULL )
    {
        m_pTranslator = new QTranslator( this );
    }

    switch ( languageId )
    {
    case LANGUAGE_KO:
        m_pTranslator->load( "/app/share/AppIBox/localization/AppIBox_ko" );
        break;

    case LANGUAGE_ZH_CMN:
    case LANGUAGE_ZH_YUE:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_zh-cmn");
        break;

    case LANGUAGE_PT:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_pt");
        break;

    case LANGUAGE_EN_GB:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_en-gb");
        break;

    case LANGUAGE_FR:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_fr-eu");
        break;

    case LANGUAGE_IT:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_it");
        break;

    case LANGUAGE_DE:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_ge");
        break;

    case LANGUAGE_ES:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_es-eu");
        break;

    case LANGUAGE_RU:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_ru");
        break;

    case LANGUAGE_NL:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_nl");
        break;

    case LANGUAGE_SV:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_sv");
        break;

    case LANGUAGE_PL:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_pl");
        break;

    case LANGUAGE_TR:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_tr");
        break;

    case LANGUAGE_CS:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_cs");
        break;

    case LANGUAGE_DA:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_da");
        break;

    case LANGUAGE_SK:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_sk");
        break;

    case LANGUAGE_AR:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_ar");
        break;

    case LANGUAGE_FR_NA:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_fa_na");
        break;

    case LANGUAGE_ES_NA:
        m_pTranslator->load("/app/share/AppIBox/localization/AppIBox_es_na");
        break;

    case LANGUAGE_EN_US:
    case LANGUAGE_EN:
    default:
        m_pTranslator->load( "/app/share/AppIBox/localization/AppIBox_en" );
        break;
    }

    qApp->installTranslator( m_pTranslator );
    emit retranslateUi( languageId );
}

bool IBoxAppEngine::connectToStatusBar()
{
    bool bResult = false;
    int iResult;
    struct sockaddr_un servaddr;

    m_iStatusBarFD = socket(AF_LOCAL, SOCK_STREAM, 0);

    if (m_iStatusBarFD != -1)
    {
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sun_family = AF_LOCAL;

        strncpy(servaddr.sun_path, WM_SOCK_PATH, 108);

        iResult = ::connect(m_iStatusBarFD, (struct sockaddr *)&servaddr, sizeof(servaddr));

        if ( iResult == 0)
        {
            bResult = true;
        }


    }
    else
    {
        CR_LOG ( "socket error");
        bResult = false;
    }

    return bResult;

}

void IBoxAppEngine::printCurrentTime()
{
#ifdef PRINT_CURRENT_TIME_IBOX
    QDateTime dateTime = QDateTime::currentDateTime();
    LW_LOG(" time:" << QString(dateTime.toString("hh:mm:ss.zzz")));
#endif
}

QString IBoxAppEngine::currentTime()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    return QString(dateTime.toString("hh:mm:ss.zzz"));
}

/*
bool IBoxAppEngine::addStatusBarArgFromDisplay( QString &string )
{
    bool bResult = true;

    switch (m_currentScreenID)
    {
    case AppEngineQMLConstants::APP_FRONT_SCREEN:
    {
        string.append("F");
        break;
    }

    case AppEngineQMLConstants::APP_REAR_SCREEN:
    {
        string.append("R");
        break;
    }

    case APP_SCREEN_ALL:
    {
        string.append("FR");
        break;
    }

    default:
        bResult = false;
        break;
    }

    return bResult;
}
*/

void IBoxAppEngine::startResponseFGTimer()
{
    QTimer::singleShot(m_pSettings->getInt(KEY_WIDGET_SEND_RESPONSE_FG_DELAY_MS), this, SLOT(onTimeoutResponseFG()));
}

void IBoxAppEngine::sendResponseFG()
{   
    switch( m_eResponseFG )
    {
        case RF_FRONT:
        {
            if (m_bIsNeedResponseFG_F) {
                LW_LOG ( "RF_FRONT >>>>>>>>>>>>>>>>>>>>>>>>>>>") ;

                Event response_event ( EVT_RESPONSE_FG, APP_IBOX, APP_UI_MANAGER );
                NotifyUISH( response_event, DISPLAY_FRONT );
                m_bIsNeedResponseFG_F = false;
                m_iResponseFGTime = 250;
            }
            else {
                LW_LOG ( "EVT_RESPONSE_FG is already sent. (RF_FRONT)");
            }
            m_eResponseFG = RF_INVALID;
            break;
        }

        case RF_REAR:
        {
            if (m_bIsNeedResponseFG_R) {
                LW_LOG ( "RF_REAR >>>>>>>>>>>>>>>>>>>>>>>>>>>" );

                Event response_event ( EVT_RESPONSE_FG, APP_IBOX, APP_UI_MANAGER );
                NotifyUISH( response_event, DISPLAY_REAR );
                m_bIsNeedResponseFG_R = false;
                m_iResponseFGTime = 250;
            }
            else {
                LW_LOG ( "EVT_RESPONSE_FG is already sent. (RF_REAR)" );
            }
            m_eResponseFG = RF_INVALID;
            break;
        }

        default:
            break;
    }

}

//------------------------------------------------------------------------------
// Acquires and creates pipeline for display
//------------------------------------------------------------------------------
void
IBoxAppEngine::AcquireDisplay( AppEngineQMLConstants::EAPP_SCREEN_USAGE_T screen )
{
    LW_LOG ( "---->" );

    m_currentScreenID = screen;
    if (m_pIBoxView) m_pIBoxView->acquire( m_currentScreenID );

    //QDeclarativeView* pDeclarativeView = GetDeclarativeView(screen);
    //pDeclarativeView->setWindowTitle("@OP^SHOW");

    LW_LOG ( "<-----" );

}

//------------------------------------------------------------------------------
// Releases the pipelines of the display
//------------------------------------------------------------------------------
void
IBoxAppEngine::ReleaseDisplay()
{
    LW_LOG ( "" );

    //SetVisibleScreens(m_currentScreenID, false);
    m_currentScreenID = AppEngineQMLConstants::APP_ALL_HIDE;

    if (m_pIBoxView)
        m_pIBoxView->release();

}

void IBoxAppEngine::PauseDisplay()
{
    LW_LOG ( "---->" );

    //SetVisibleScreens(m_currentScreenID, false);
    m_currentScreenID = AppEngineQMLConstants::APP_ALL_HIDE;

    if (m_pIBoxView) m_pIBoxView->pause();

    LW_LOG ( "<----" );
}

void IBoxAppEngine::hideViewWidget()
{
    /*
    LW_LOG ( "" );

    if (m_pViewWidget->isVisible() == true)
    {
        LW_LOG( "View widget is visible. Hide view widget" );
        setWidgetShownFlag(false);
        m_pViewWidget->hide();
    }
    */

    //emit sigHideVideo();
}

void IBoxAppEngine::showViewWidget()
{
    LW_LOG ( "---->" );

    /*
    if (m_pViewWidget->isVisible() == false)
    {
        LW_LOG( "View widget is not visible. Show view widget" );
        setWidgetShownFlag(true);
        m_pViewWidget->show();
        m_pViewWidget->raise();

        if ( this->getNeedToShowStatusBarFlag() == true )
        {
            showStatusBar();
            LW_LOG("Show status bar");
        }
        else
        {
            hideStatusBar();
            LW_LOG("Hide status bar");
        }
    }
    */

    //emit sigShowVideo();


    if ( this->getNeedToShowStatusBarFlag() == true )
    {
        showStatusBar();
        LW_LOG("Show status bar");
    }
    else
    {
        hideStatusBar();
        LW_LOG("Hide status bar");
    }

    printCurrentTime();


    if ( m_pSettings->getInt(KEY_WIDGET_SEND_RESPONSE_FG_DELAY_MS) > 0 )
    {
        startResponseFGTimer();
    }
    else
    {
        sendResponseFG();
    }

    LW_LOG ( "<----" );

}

WId IBoxAppEngine::winId()
{
    //return m_pViewWidget->winId();
    return 0;
}

/*
void IBoxAppEngine::setGeometry(int ax, int ay, int aw, int ah)
{
    //m_pViewWidget->setGeometry(ax, ay, aw, ah);
}
*/

void IBoxAppEngine::showStatusBar()
{
    LW_LOG ( "" );

    /* for new statusbar ===========>> */
    //QDeclarativeView* pDeclarativeView = GetDeclarativeView(m_currentScreenID);
    //pDeclarativeView->setWindowTitle("@OP^SHOW");

    emit sigStatusBar(true);

    setNeedToShowStatusBarFlag( true );
}

void IBoxAppEngine::hideStatusBar()
{
    LW_LOG ( "showViewWidget" );

    /* for new statusbar ===========>> */
    //QDeclarativeView* pDeclarativeView = GetDeclarativeView(m_currentScreenID);
    //pDeclarativeView->setWindowTitle("@OP^HIDE");

    emit sigStatusBar(false);

    setNeedToShowStatusBarFlag( false );
}

void IBoxAppEngine::showOSD(QString &osdCommad)
{
    LW_LOG ( "" );

    sendEventToStatusBar(osdCommad);
}

bool IBoxAppEngine::swapped()
{
    return m_bDCSwapped;
}

bool IBoxAppEngine::sendEventToUISH( Event &aEvent )
{
    return NotifyUISH( aEvent, GetDisplayType() );
}

void IBoxAppEngine::sendEventToStatusBar( QString &string )
{

    if ( m_iStatusBarFD < 0)
    {
        CR_LOG( "Sending event to status bar is failed" );
    }
    else
    {
        QByteArray array( (const char *) (string.utf16()), string.size() * 2);

        write(m_iStatusBarFD, array.data(), array.count());
    }
}

void IBoxAppEngine::setStatusBarModemState( QString &str )
{
    m_strIBoxRSSI = str;

    if (m_bShowRSSI)
        sendEventToStatusBar(m_strIBoxRSSI);
}

/*
DISPLAY_T IBoxAppEngine::getDisplayType()
{
    switch (m_currentScreenID)
    {
    case APP_SCREEN_FRONT: return DISPLAY_FRONT;
    case APP_SCREEN_REAR: return DISPLAY_REAR;
    case APP_SCREEN_ALL: return DISPLAY_FRONT_REAR;
    case APP_SCREEN_NONE: return DISPLAY_NONE;
    }

    return DISPLAY_NONE;
}
*/

void IBoxAppEngine::showDRSPopUp(DISPLAY_T display)
{
    LW_LOG ( "display:" << display);

    QDeclarativeView *pView = NULL;

    if (display == DISPLAY_FRONT)
        pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
    else
        pView = GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);

    if (pView)
    {
        QMetaObject::invokeMethod((QObject*)pView->rootObject(), "loadDRSPopUp_DownloadApps", Qt::QueuedConnection);
        setDeckStatus( m_pMostMessageHandler->getCurDeckStatus() );
    }

    m_bIsDrsPopUpShown = true;

}

void IBoxAppEngine::hideDRSPopUp(DISPLAY_T display)
{
    LW_LOG ( "" );

    QDeclarativeView *pView = NULL;

    if (display == DISPLAY_FRONT)
        pView = GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
    else
        pView = GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);

    if (pView)
    {
        QMetaObject::invokeMethod((QObject*)pView->rootObject(), "unloadDRSPopUp", Qt::QueuedConnection);
    }

    if ( this->getNeedToShowStatusBarFlag() == true )
    {
        showStatusBar();
    }

    m_bIsDrsPopUpShown = false;

    if (display == DISPLAY_FRONT && m_bFG_F)
    {
        QTimer::singleShot(m_iResponseFGTime, this, SLOT(onTimeoutResponseFGFrontOnly()));
    }

    if (display == DISPLAY_REAR && m_bFG_R)
    {
        QTimer::singleShot(m_iResponseFGTime, this, SLOT(onTimeoutResponseFGRearOnly()));
    }
}

void IBoxAppEngine::backFromDRSPopup(int screen)
{

    Event event( EVT_KEY_SOFT_BACK, GetThisAppId(), APP_UI_MANAGER);

    if ( !NotifyUISH(event, (DISPLAY_T)screen) )
    {
        CR_LOG ("backFromDRSPopup: Event notification failed" );
    }
}

void IBoxAppEngine::showDLNAList()
{
    LW_LOG ( "" );

    Event event( EVT_KEY_SOFT_IBOX_APP_DRS_LISTING_CLOSE, GetThisAppId(), APP_UI_MANAGER);

    if ( !NotifyUISH(event,  DISPLAY_FRONT))
    {
        CR_LOG("showDLNAList: Event notification failed" );
    }
}

void IBoxAppEngine::setIncrementTrackPosition()
{
    m_pMostMessageHandler->sendIncrementTrackPosition();
}

void IBoxAppEngine::setDecrementTrackPosition()
{
    m_pMostMessageHandler->sendDecrementTrackPosition();
}

void IBoxAppEngine::setPlayPause( bool isPlay )
{
    m_pMostMessageHandler->sendPlayPause( isPlay );
}

void IBoxAppEngine::onTimeoutResponseFG()
{
    sendResponseFG();
}

void IBoxAppEngine::onTimeoutResponseFGFrontOnly()
{
    if (m_bIsNeedResponseFG_F) {
        LW_LOG ( "Only FRONT >>>>>>>>>>>>>>>>>>>>>>>>>>>" );

        Event response_event ( EVT_RESPONSE_FG, APP_IBOX, APP_UI_MANAGER );
        NotifyUISH( response_event, DISPLAY_FRONT );
        m_bIsNeedResponseFG_F = false;
        m_iResponseFGTime = 250;
    }
    else {
        LW_LOG ( "EVT_RESPONSE_FG is already sent. (Only FRONT)" );
    }
}

void IBoxAppEngine::onTimeoutResponseFGRearOnly()
{
    if (m_bIsNeedResponseFG_R) {
        LW_LOG ( "Only REAR >>>>>>>>>>>>>>>>>>>>>>>>>>>" );

        Event response_event ( EVT_RESPONSE_FG, APP_IBOX, APP_UI_MANAGER );
        NotifyUISH( response_event, DISPLAY_REAR );
        m_bIsNeedResponseFG_R = false;
        m_iResponseFGTime = 250;
    }
    else {
        LW_LOG ( "EVT_RESPONSE_FG is already sent. (Only REAR)" );
    }
}

void IBoxAppEngine::setWidgetShownFlag( bool isShown )
{
    m_bIsWidgetShown = isShown;
}

bool IBoxAppEngine::getWidgetShownFlag()
{
    return m_bIsWidgetShown;
}

void IBoxAppEngine::setNeedToShowStatusBarFlag( bool isNeed )
{
    this->m_bNeedToShowStatusBar = isNeed;
}

/*
void IBoxAppEngine::setVideoSink( QGst::ElementPtr aVideoSink )
{
    if( aVideoSink && surface )
    {
        if( !surface->videoSink() )
        {
            LW_LOG( "video sink is set ");
            surface->setVideoSink( aVideoSink );
        }
    }
}
*/

/*
void IBoxAppEngine::releaseVideoSink()
{
    if( surface && surface->videoSink() )
    {
        LW_LOG( "video sink is released ");
        surface->releaseVideoSink();
    }
}
*/

void IBoxAppEngine::EnterDRSSoftBackKey()
{
    LW_LOG( "EnterDRSSoftBackKey..." );
#ifdef SEND_BACKKEYON_TO_MOSTMANAGER
    m_pMostMessageHandler->sendBackKeyOn(true);
#endif
    //hideDRSPopUp((m_eResponseFG==RF_FRONT)? DISPLAY_FRONT : DISPLAY_REAR);
}

void IBoxAppEngine::SendDRSHardBackKey()
{
    LW_LOG( "SendDRSHardBackKey..." );
#ifdef SEND_BACKKEYON_TO_MOSTMANAGER
    m_pMostMessageHandler->sendBackKeyOn(false);
#endif
}

// for SW WDT -->
void IBoxAppEngine::signalConnection()
{
    struct sigaction act;
    memset(&act, 0x00, sizeof(struct sigaction));

    act.sa_flags = 0;
    act.sa_flags |= SA_RESETHAND;
    act.sa_handler = IBoxAppEngine::signalHandler;

    for ( int signumber = SIGHUP; signumber < SIGUNUSED; signumber++ )
    {
        if ( signumber == SIGKILL || signumber == SIGSTOP ) continue;

        if ( sigaction(signumber, &act, (struct sigaction*)NULL) < 0 )
        {
            exit(1);
        }
    }
}

void IBoxAppEngine::signalHandler (int signum)
{
    CR_LOG("signum: " << signum) ;

    SigM->restart(signum);

    raise(signum);
}


void IBoxAppEngine::setSignalHandlerObject (IBoxAppEngine* m)
{
    SigM = m;
}

void IBoxAppEngine::restart(int signum)
{
    CR_LOG ( "" );

    Event event(EVT_REQUEST_ABNORMAL_TERMINATION, GetThisAppId(), APP_UI_MANAGER, (EVT_COMMAND_T)signum);
    NotifyUISH (event, DISPLAY_FRONT);
    //exit(0);
}
// <--
