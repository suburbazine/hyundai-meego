#include "DHAVN_AppIBox_IBoxView.h"
#include <QTime>
#include <QThread>

class Thread : public QThread
{
public:
    static void msleep(int ms)
    {
        QThread::msleep(ms);
    }
};


IBoxView::IBoxView(Logger &pLogger, IViewWidgetController *pController, IStatusBarController *pSBController, Settings *pSettings, QObject * parent) :
    QObject(parent),
    m_pLogger(pLogger),
    m_pViewWidgetController(pController),
    m_pStatusBarController( pSBController ),
    m_screenId(AppEngineQMLConstants::APP_INVALIDE_STATE),
    m_pSettings(pSettings),
    m_state(eNotStarted),
    m_display_pipeline(0),
    m_video_src(0),
    m_mqueue(0),
    m_capsfilter(0),
    m_mainvideosink(0),
    m_bPipelineCreated(false),
    m_mutex_gst_pipeline_state(QMutex::NonRecursive)

{

    QObject::connect(this, SIGNAL(stateChanged(State,State)), this, SLOT(onStateChanged(State,State)));
}

IBoxView::~IBoxView()
{
    QObject::disconnect(this, SIGNAL(stateChanged(State,State)), this, SLOT(onStateChanged(State,State)));
}


void IBoxView::setVideoSink(QGst::ElementPtr sink, QDeclarativeView *pView)
{
    LW_LOG ( "---->" );
    m_mainvideosink = sink;

    if (pView == NULL)
    {
        LW_LOG ( "pView == NULL");
        return;
    }

    WId xwinid = pView->winId();
    gst_x_overlay_set_window_handle (GST_X_OVERLAY (m_mainvideosink), xwinid);
    LW_LOG ( "<----" );
}

void IBoxView::init()
{
    LW_LOG ( "---->" );
    changeState(eCreatePipeLine);
    m_bPipelineCreated = true;
    changeState(ePausePipeLine);
    LW_LOG ( "<----" );
}

void IBoxView::acquire( AppEngineQMLConstants::EAPP_SCREEN_USAGE_T screenId )
{
    LW_LOG ( "===---->" );

    m_screenId = screenId;

    if (!m_bPipelineCreated)
    {
        LW_LOG ( ", !m_bPipelineCreated -> eCreatePipeLine -> ePlayPipeLine" );
        changeState(eCreatePipeLine);
        m_bPipelineCreated = true;
        changeState(ePlayPipeLine);
    }
    else
    {
        LW_LOG ( "m_bPipelineCreated -> ePlayPipeLine" );
        changeState(ePlayPipeLine);
    }

    LW_LOG ( "<----===" );
}

void IBoxView::reInit()
{
    m_pViewWidgetController->reInit();
}

void IBoxView::release()
{
    LW_LOG( "-->" );

    changeState(eReleasePipeLine);

    LW_LOG( "<--" );
}

void IBoxView::pause()
{
    LW_LOG( "-->" );

    changeState(ePausePipeLine);

    LW_LOG( "<--" );
}

gboolean IBoxView::IBoxView::onBusMessage(GstBus *, GstMessage *msg, gpointer data)
{
    IBoxView *view = (IBoxView *) data;

    view->GetLogger().Log( QString("GST Message: %1").arg(GST_MESSAGE_TYPE (msg)));
    //view->GetLogger().Log( view->currentTime() ;

    switch (GST_MESSAGE_TYPE (msg)) {
    case GST_MESSAGE_EOS:{
        view->GetLogger().Log( QString("End-of-stream") );
        break;
    }
    case GST_MESSAGE_ERROR:{
        gchar *debug = 0;
        GError *err = 0;

        gst_message_parse_error (msg, &err, &debug);

        view->GetLogger().Log( QString("[GST_MESSAGE_ERROR] ERROR from element: %1").arg(GST_OBJECT_NAME(msg->src)), Logger::PRIORITY_CRITICAL);
        view->GetLogger().Log( QString("[GST_MESSAGE_ERROR] Error message: %1").arg(err->message), Logger::PRIORITY_CRITICAL);
        if (debug) view->GetLogger().Log( QString("[GST_MESSAGE_ERROR] Debug: %1").arg(debug), Logger::PRIORITY_CRITICAL);

        g_free (debug);
        g_error_free (err);
        break;
    }

    case GST_MESSAGE_WARNING:{
        gchar *debug = 0;
        GError *err = 0;

        gst_message_parse_warning (msg, &err, &debug);

        view->GetLogger().Log( QString("[GST_MESSAGE_WARNING] Warning from element: %1").arg(GST_OBJECT_NAME(msg->src)), Logger::PRIORITY_CRITICAL);
        view->GetLogger().Log( QString("[GST_MESSAGE_WARNING] Warning message: %1").arg(err->message), Logger::PRIORITY_CRITICAL);
        if (debug) view->GetLogger().Log( QString("[GST_MESSAGE_WARNING] Debug: %1").arg(debug), Logger::PRIORITY_CRITICAL);

        if (QString(GST_OBJECT_NAME(msg->src)).contains("qtglvideosink", Qt::CaseInsensitive)) // check if this is emitted only by qtglvideosink
        {
            if(!strcmp(err->message , "Neither GLSL nor ARB Fragment Program are supported for painting. Did you forget to set a gl context?"))  // Compare the message string to known type.
            {
                view->GetLogger().Log( QString("Warning message for qtglvideosink element related to context not valid."), Logger::PRIORITY_CRITICAL); // And here you can do what is needed.
                view->reInit();
            }
        }

        g_free (debug);
        g_error_free (err);
        break;
    }

    case GST_MESSAGE_STATE_CHANGED:{
        GstState old_state, new_state;

        gst_message_parse_state_changed (msg, &old_state, &new_state, NULL);
//        view->GetLogger().Log( QString("Element %1 changed state from %2 to %3").
//                               arg(GST_OBJECT_NAME(msg->src)).
//                               arg(gst_element_state_get_name(old_state)).
//                               arg(gst_element_state_get_name(new_state)) ;

        if (QString(GST_OBJECT_NAME(msg->src)).compare(view->getSettings()->getChars(KEY_PIPELINE_NAME)) == 0
                && new_state == GST_STATE_PLAYING)
        {
            if (view->getSettings()->getInt(KEY_WIDGET_SHOW_DELAY_MS) > 0)
            {
                view->changeState(IBoxView::eWaitForTimeOut);
            }
            else
            {
                view->changeState(IBoxView::eShowing);
            }
        }

        break;
    }
    default:
        break;
    }

    /* we want to be notified again the next time there is a message
     * on the bus, so returning TRUE (FALSE means we want to stop watching
     * for messages on the bus and our callback should not be called again)
     */
    return true;
}

void IBoxView::showWidget()
{
    LW_LOG("showWidget");

    /*
    switch (m_screenId)
    {
    case AppEngineQMLConstants::APP_REAR_SCREEN:
        LOG("USING REAR DISPLAY");
        m_pViewWidgetController->setGeometry(m_pSettings->getInt(KEY_WIDGET_REAR_X), m_pSettings->getInt(KEY_WIDGET_REAR_Y),
                                             m_pSettings->getInt(KEY_WIDGET_REAR_W), m_pSettings->getInt(KEY_WIDGET_REAR_H));
        break;

    default:
        LOG("USING FRONT DISPLAY");
        m_pViewWidgetController->setGeometry(m_pSettings->getInt(KEY_WIDGET_FRONT_X), m_pSettings->getInt(KEY_WIDGET_FRONT_Y),
                                             m_pSettings->getInt(KEY_WIDGET_FRONT_W), m_pSettings->getInt(KEY_WIDGET_FRONT_H));
        break;
    }
    */

    m_pViewWidgetController->showViewWidget();
}

Settings * IBoxView::getSettings()
{
    return m_pSettings;
}

void IBoxView::changeState(IBoxView::State state)
{
    if (m_state != state)
    {
        emit stateChanged(m_state, state);
        m_state = state;
    }
}

void IBoxView::onStateChanged(IBoxView::State oldState, IBoxView::State newState)
{
    HS_LOG("old:" << oldState << ", new:" << newState << ", Mutex Lock");

    m_mutex_gst_pipeline_state.lock();

    switch (newState)
    {
    case eCreatePipeLine:
        createPipeLine();
        break;

    case eWaitForPlaying:
        // Do nothing
        break;

    case eWaitForTimeOut:
        startShowWidgetTimer();
        break;

    case eShowing:
        showWidget();
        break;

    case eReleasePipeLine:
        releasePipeLine();
        break;

    case ePausePipeLine:
        pausePipeLine();
        break;

    case eNotStarted:
        break;

    case ePlayPipeLine:
        playPipeLine();
        break;
    }

    m_mutex_gst_pipeline_state.unlock();
    HS_LOG("Mutex Unlock") ;
}

void IBoxView::createPipeLine()
{
    LW_LOG ( "---->" );

    GValue display = { 0, { { 0 } } };
    g_value_init (&display, G_TYPE_STRING);

    m_display_pipeline = gst_pipeline_new(m_pSettings->getChars(KEY_PIPELINE_NAME));

    if (m_pSettings->getBool(KEY_WIDGET_WATCH_BUS) == true)
    {
        GstBus* bus = gst_element_get_bus (m_display_pipeline);
        gst_bus_add_watch (bus, onBusMessage, this);
        g_object_unref (bus);
    }

    m_capsfilter = gst_element_factory_make("capsfilter", NULL);
    m_video_src = gst_element_factory_make("v4l2src", "sourcev4l2");

    if (m_pSettings->getInt(KEY_PIPELINE_EMGD_MAPPER) > -1)
    {
        setenv("IEGD_VIDEO_OUT_TEXTURE_STREAM", "1", m_pSettings->getInt(KEY_PIPELINE_EMGD_MAPPER));
        setenv("IEGD_EMGD_MAPPER", "FORCE", m_pSettings->getInt(KEY_PIPELINE_EMGD_MAPPER));
        setenv("IEGD_VIDEO_OUTPUT_TEXTURE_STREAM", "FORCE", m_pSettings->getInt(KEY_PIPELINE_EMGD_MAPPER));
    }
    g_object_set(m_video_src, "always-copy", m_pSettings->getInt(KEY_PIPELINE_ALWAYS_COPY), (void *)NULL);
    g_object_set(m_video_src, "queue-size", m_pSettings->getInt(KEY_PIPELINE_QUEUE_SIZE), (void *)NULL);
    g_object_set(G_OBJECT(m_video_src), "device", m_pSettings->getChars(KEY_PIPELINE_DEVICE), (void *)NULL);
    g_object_set(G_OBJECT(m_capsfilter), "caps", gst_caps_from_string(m_pSettings->getChars(KEY_PIPELINE_CAPS)), NULL);


    m_mqueue = gst_element_factory_make("queue", "queue");

    if( m_mainvideosink == NULL )
    {
        CR_LOG(" m_mainvideosink is NULL " );
    }


    gst_bin_add(GST_BIN(m_display_pipeline), m_video_src);
    gst_bin_add(GST_BIN(m_display_pipeline), m_capsfilter);
    gst_bin_add(GST_BIN(m_display_pipeline), m_mainvideosink);

    gst_element_link_many(m_video_src, m_capsfilter, m_mainvideosink, (void *)NULL);

    improveFPS(true);

    /*
    if ( m_pViewWidgetController )
    {
        GstStateChangeReturn sret = GST_STATE_CHANGE_SUCCESS;

        for (int i = 0; i < 3; i++)
        {
            sret = gst_element_set_state (m_display_pipeline, GST_STATE_PAUSED);

            if (sret == GST_STATE_CHANGE_FAILURE)
            {
                CR_LOG("GST_STATE_CHANGE_FAILURE.. retry set state GST_STATE_PLAYING");
                Thread::msleep(100);
            }

            else
            {
                LW_LOG("GST_STATE_CHANGE_SUCCESS");
                break;
            }
        }
    }
    */

    LW_LOG ( currentTime() );
}

void IBoxView::releasePipeLine()
{
    /*
    if( m_pViewWidgetController )
    {
        m_pViewWidgetController->hideViewWidget();
    }
    */

    if (m_display_pipeline != NULL) {
        gst_element_set_state(GST_ELEMENT(m_display_pipeline), GST_STATE_NULL);

        m_video_src = NULL;
        m_mqueue = NULL;
        m_capsfilter = NULL;
        m_mainvideosink = NULL;
        gst_object_unref( m_display_pipeline );
        m_display_pipeline = NULL;
    }

    //changeState(eNotStarted);
}

void IBoxView::pausePipeLine()
{

    LW_LOG ( "---->" );

    improveFPS(false);

//    if( m_pViewWidgetController )
//    {
//        m_pViewWidgetController->hideViewWidget();
//    }

    if (m_display_pipeline != NULL)
    {
        GstStateChangeReturn sret = GST_STATE_CHANGE_SUCCESS;

        for (int i = 0; i < 3; i++)
        {
            gst_element_set_state(m_video_src, GST_STATE_PAUSED);
            sret = gst_element_set_state (m_display_pipeline, GST_STATE_PAUSED);

            if (sret == GST_STATE_CHANGE_FAILURE)
            {
                CR_LOG("GST_STATE_CHANGE_FAILURE.. retry set state GST_STATE_PAUSED");
                Thread::msleep(100);
            }

            else
            {
                HS_LOG("GST_STATE_CHANGE_SUCCESS");
                break;
            }
        }
    }

//    if( m_pViewWidgetController )
//    {
//        m_pViewWidgetController->hideViewWidget();
//    }


    LW_LOG ( "<----" );
}

void IBoxView::playPipeLine()
{
    LW_LOG ( "------------------------->" );

    improveFPS(true);

    //clearDisplay(false);

    if (m_display_pipeline != NULL)
    {
        GstStateChangeReturn sret = GST_STATE_CHANGE_SUCCESS;

        for (int i = 0; i < 3; i++)
        {
            sret = gst_element_set_state (m_display_pipeline, GST_STATE_PLAYING);

            if (sret == GST_STATE_CHANGE_FAILURE)
            {
                CR_LOG("GST_STATE_CHANGE_FAILURE.. retry set state GST_STATE_PLAYING");
                Thread::msleep(100);
            }

            else
            {
                LW_LOG("GST_STATE_CHANGE_SUCCESS");
                break;
            }
        }
    }

    LW_LOG ( "<-----------------------------" );
}

void IBoxView::startShowWidgetTimer()
{
    QTimer::singleShot(m_pSettings->getInt(KEY_WIDGET_SHOW_DELAY_MS), this, SLOT(onTimeoutShowWidget()));
}

void IBoxView::onTimeoutShowWidget()
{
    changeState(eShowing);
}

void IBoxView::printCurrentTime()
{
#ifdef PRINT_CURRENT_TIME_IBOX
    QDateTime dateTime = QDateTime::currentDateTime();
    LW_LOG(" time:" << dateTime.toString("hh:mm:ss.zzz"));
#endif
}

QString IBoxView::currentTime()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    return QString(dateTime.toString("hh:mm:ss.zzz"));
}

void IBoxView::improveFPS(bool bEnable)
{
    //LW_LOG ( "---->" );

    g_object_set (G_OBJECT(m_mainvideosink), "dhavn-enable-sleep", bEnable, NULL);

    //LW_LOG ( "<----" );
}

void IBoxView::clearDisplay(bool bEnable)
{
    //LW_LOG ( "----->" );

    if(bEnable)
    {
        g_object_set (G_OBJECT(m_mainvideosink), "clear-drawable", true, NULL);
        g_object_set (G_OBJECT(m_mainvideosink), "stop-rendering", true, NULL);
        g_object_set (G_OBJECT(m_mainvideosink), "clear-drawable", true, NULL);
    }

    else
    {
        g_object_set (G_OBJECT(m_mainvideosink), "clear-drawable", false, NULL);
        g_object_set (G_OBJECT(m_mainvideosink), "stop-rendering", false, NULL);
    }

    Thread::msleep(33);

    //LW_LOG ( "<-----" );
}
