#ifndef DHAVN_APPIBOX_IBOXVIEW_H
#define DHAVN_APPIBOX_IBOXVIEW_H

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QMutex>
#include <QDeclarativeView>

#include <gst/gst.h>
#include <QGst/Element>
#include <gstreamer-0.10/gst/interfaces/xoverlay.h>
#include <gst/interfaces/xoverlay.h>

#include "uistate_manager_def.h"
#include "DHAVN_AppFramework_QML_Def.h"

#include "DHAVN_AppIBox_Def.h"
#include "DHAVN_AppIBox_IViewWidgetController.h"
#include "DHAVN_AppIBox_IStatusBarController.h"
#include "DHAVN_AppIBox_Settings.h"
#include "DHAVN_AppIBox_LogUtils.h"

class IBoxView : public QObject
{
    Q_OBJECT

public:
    explicit IBoxView(Logger &pLogger, IViewWidgetController *pController, IStatusBarController *pSBController, Settings *pSettings, QObject * parent = 0);
    ~IBoxView();

    void init();
    void reInit();
    void acquire( AppEngineQMLConstants::EAPP_SCREEN_USAGE_T screenId );
    void release();
    void pause();

    static gboolean onBusMessage(GstBus * /*bus*/, GstMessage * msg, gpointer data);

    Settings * getSettings();

    enum State
    {
        eNotStarted,     // Next states: eCreatePipeLine
        eCreatePipeLine, // Next states: eWaitForPlaying, eWaitForTimeOut, eShowing
        eWaitForPlaying, // Next states: eWaitForTimeOut, eShowing
        eWaitForTimeOut, // Next states: eShowing
        eShowing,        // Next states: eReleasePipeLine
        eReleasePipeLine,// Next states: eNotStarted
        ePausePipeLine,
        ePlayPipeLine
    };

    void changeState(State state);
    void printCurrentTime();
    QString currentTime();

    void setVideoSink(QGst::ElementPtr sink, QDeclarativeView *pView);

    DEFINE_LOGGER

signals:
    void stateChanged(State oldState, State newState);

public slots:
    void onStateChanged(State oldState, State newState);
    void onTimeoutShowWidget();

    void playPipeLine();
    void pausePipeLine();

private:
    bool m_bPipelineCreated;
    IViewWidgetController *m_pViewWidgetController;
    IStatusBarController *m_pStatusBarController;
    AppEngineQMLConstants::EAPP_SCREEN_USAGE_T m_screenId;

    /*!
     * Gst Elements
     */
    GstElement *m_display_pipeline;
    GstElement *m_video_src;
    GstElement *m_mqueue;
    GstElement *m_capsfilter;
    GstElement *m_mainvideosink;

    QMutex m_mutex_gst_pipeline_state;

    Settings *m_pSettings;

    void showWidget();

    State m_state;

    void createPipeLine();
    void releasePipeLine();


    void startShowWidgetTimer();

    void improveFPS(bool bEnable);

    void clearDisplay(bool bEnable);
};

#endif // DHAVN_APPIBOX_IBOXVIEW_H
