#include "DHAVN_GSTAudioRouter_Def.h"
#include <QTimer>
//constants
QString gAlsaSrcName     = "alsasrcname";
QString gAlsaSinkName    = "alsasinkname";
QString gGSTPipelineName = "alsaPipelinename";
//static GstElement* m_AudioPipeline = NULL;

CGSTAudioRouter::CGSTAudioRouter()
{
    LOG_INIT_V2("Media_IPodController");
    //LOG_TRACE<<"CGSTAudioRouter::CGSTAudioRouter() IN" << LOG_ENDL;
    gst_init(NULL, NULL);

    /**open to debug the lib**/
    //setenv("GST_DEBUG","alsa*:5",1);
    //gst_debug_add_log_function (loggernn, NULL);
    /**open to debug end**/

    m_CardSink.clear();
    m_CardSink.append("DEFAULT");
    m_CardSrc.clear();

    m_GSTBus_Watch_Id = 0;
    m_SyncType        = false;
    m_AudioPipeline   = NULL;
    m_audioSource     = NULL;
    m_AudioSink       = NULL;
    m_CapsElement     = NULL;
    m_resamplesrc     = NULL;
    m_resamplesink    = NULL;
    m_sinkCapsElement = NULL;
    m_PipeState       = eIdle;

    InitializeDBusConnect();

    //LOG_TRACE << "CGSTAudioRouter::CGSTAudioRouter() OUT" << LOG_ENDL;
}

CGSTAudioRouter::~CGSTAudioRouter()
{
    //LOG_TRACE<<"CGSTAudioRouter::~CGSTAudioRouter() IN" << LOG_ENDL;

    stop();

    if(m_AudioPipeline)
    {
        //LOG_TRACE << "   killing pipe... gst_object_unref( m_AudioPipleline )" << LOG_ENDL;
        gst_object_unref (m_AudioPipeline);
    }

    if(m_GSTBus_Watch_Id>0)
        g_source_remove (m_GSTBus_Watch_Id);

    m_GSTBus_Watch_Id = 0;
    m_AudioPipeline   = NULL;

    //LOG_TRACE<<"CGSTAudioRouter::~CGSTAudioRouter() OUT" << LOG_ENDL;
}

void 
CGSTAudioRouter::InitializeDBusConnect()
{
    //LOG_TRACE << "CGSTAudioRouter::InitializeDBusConnect() IN" << LOG_ENDL;

    QDBusConnection m_DBusConn = QDBusConnection::sessionBus();

    m_DBusConn.registerObject( AUDIOROUTER_DBUS_PATH,
                               this );
    // Fade Out
    m_DBusConn.connect( QString(),
                       AUDIOROUTER_DBUS_PATH,
                       AUDIOROUTER_DBUS_INTERFACE_NAME,
                       "AudioFadeOutSignal",
                       this,
                       SLOT( AudioFadeOutSlot() ) );

    // Fade In
    m_DBusConn.connect( QString(),
                       AUDIOROUTER_DBUS_PATH,
                       AUDIOROUTER_DBUS_INTERFACE_NAME,
                       "AudioFadeInSignal",
                       this,
                       SLOT( AudioFadeInSlot() ) );


    //LOG_TRACE << "CGSTAudioRouter::InitializeDBusConnect() OUT" << LOG_ENDL;
}

void CGSTAudioRouter::setSource(QString src)
{
    //LOG_TRACE << "CGSTAudioRouter::setSource() IN, src = " << src << LOG_ENDL;
    m_CardSrc.clear();
    m_CardSrc.append(src);
}

void CGSTAudioRouter::setSink(QString sink)
{
    //LOG_TRACE << "CGSTAudioRouter::setSink() IN, sink = " << sink << LOG_ENDL;
    m_CardSink.clear();
    m_CardSink.append(sink);
}

void CGSTAudioRouter::setSyncType(bool sync)
{
    //LOG_TRACE << "CGSTAudioRouter::setSyncType() IN, sync = " << sync << LOG_ENDL;
    m_SyncType = sync;
}

void CGSTAudioRouter::setSampleRate(QString string)
{
    //LOG_TRACE << "CGSTAudioRouter::setSampleRate() In, string = " << string << LOG_ENDL;
    m_Rate.clear();
    m_Rate.append(string);
}

bool CGSTAudioRouter::pause()
{
    return false;
    // Modified by jonghwan.cho@lge.com	2013.03.29.
    // for static test
#if 0
    //LOG_TRACE << "CGSTAudioRouter :: Pause()" << LOG_ENDL;
    bool ret = false;
    if(m_PipeState == ePlay)
    {
        if(GST_STATE_CHANGE_FAILURE == gst_element_set_state(m_AudioPipeline, GST_STATE_READY))
        {
            //LOG_TRACE <<"CGSTAudioRouter pause state not set" << LOG_ENDL;
        }
        else
        {
            gst_element_send_event(m_AudioPipeline , gst_event_new_flush_start ());
            m_PipeState = ePause;
            ret = true;
            //LOG_TRACE <<"CGSTAudioRouter pause state set" << LOG_ENDL;
        }
    }
    return ret;
#endif
}

bool CGSTAudioRouter::play()
{
    //LOG_TRACE << "CGSTAudioRouter::Play() IN" << LOG_ENDL;

    bool ret = false;

    if( m_PipeState == ePause || m_PipeState == eNull )
    {
        if(GST_STATE_CHANGE_FAILURE == gst_element_set_state(m_AudioPipeline, GST_STATE_PLAYING))
        {
            LOG_HIGH << "CGSTAudioRouter::Play(), playSlot state not set, call stop()" << LOG_ENDL;
            stop();
        }
        else
        {
            ret = true;
            m_PipeState = ePlay;
            LOG_HIGH << "CGSTAudioRouter::Play(), playSlot state set, call AudioFadeIn(), m_PipeState = " << m_PipeState << LOG_ENDL;
            AudioFadeIn();
        }
    }
    //LOG_TRACE << "CGSTAudioRouter::Play() OUT" << LOG_ENDL;
    return ret;
}

bool CGSTAudioRouter::start()
{
    //LOG_TRACE << "CGSTAudioRouter::start() IN" << LOG_ENDL;

    bool ret        = false;
    bool use_myrate = false;
    static int rate;

    if( m_AudioPipeline && m_PipeState == eIdle )
    {
        //LOG_TRACE << "   m_AudioPipeline is already exist, so clean the pipe" << LOG_ENDL;
        if(GST_STATE_CHANGE_SUCCESS == gst_element_get_state(m_AudioPipeline,NULL,NULL,-1))
        {
            gst_object_unref (m_AudioPipeline);
        }
        else
        {
            //LOG_TRACE << "   pipeline is not stopped_" << __LINE__ << LOG_ENDL;
        }
        m_AudioPipeline = NULL;
    }

    if(!m_AudioPipeline)
    {
        LOG_HIGH << "CGSTAudioRouter::start(), Creating pipeline" << LOG_ENDL;
        m_audioSource   = gst_element_factory_make("alsasrc",    (gchar*)gAlsaSrcName.toAscii().data());
        m_AudioSink     = gst_element_factory_make("alsasink",   (gchar*)gAlsaSinkName.toAscii().data());
        m_CapsElement   = gst_element_factory_make("capsfilter", "mycaps");
        m_audioqueue    = gst_element_factory_make("queue",      "audio_queue");
        m_volume        = gst_element_factory_make("volume",     "audio_volume");
        m_AudioPipeline = gst_pipeline_new((gchar*)gGSTPipelineName.toAscii().data());

        if( !m_audioSource || !m_AudioSink || !m_AudioPipeline || !m_CapsElement || !m_audioqueue || !m_volume)
        {
            //LOG_TRACE << "CGSTAudioRouter one of the element is not created" << LOG_ENDL;
            return ret;
        }

        GstBus* bus = gst_pipeline_get_bus (GST_PIPELINE (m_AudioPipeline));
        m_GSTBus_Watch_Id = gst_bus_add_watch (bus, gst_Bus_callback, this);
        gst_object_unref (bus);

        gchar* recordDevice((gchar*)m_CardSrc.toAscii().data());
        g_print("CGSTAudioRouter record device is:  %s",  recordDevice);

        //LOG_TRACE << "   m_CardSrc = " << m_CardSrc.toAscii().data() << LOG_ENDL;
        g_object_set(G_OBJECT(m_audioSource), "device", recordDevice, NULL);

        if(!m_SyncType)                   g_object_set(G_OBJECT(m_AudioSink), "sync", m_SyncType, NULL);
        if(m_CardSink.compare("DEFAULT")) g_object_set(G_OBJECT(m_AudioSink), "device", (gchar*)m_CardSink.toAscii().data(), NULL);

        QString capsString("audio/x-raw-int,rate=");
        capsString.append(m_Rate);
        //LOG_TRACE << "   m_Caps    = " << capsString << LOG_ENDL;

        gchar* caps((gchar*)capsString.toAscii().data());
        m_Caps = gst_caps_from_string(caps);

        g_object_set(G_OBJECT(m_CapsElement), "caps", m_Caps, NULL);
        gst_caps_unref(m_Caps);

        int myrate = m_Rate.toInt();

        rate = myrate;
        //LOG_TRACE << "   myrate    = " << myrate <<LOG_ENDL;

        if(myrate < 44100 || myrate > 48000)
        {
            m_resamplesrc     = gst_element_factory_make ("audioconvert", "audio-resamplesrc");
            m_resamplesink    = gst_element_factory_make ("audioresample", "audio-resamplesink");
            m_sinkCapsElement = gst_element_factory_make ("capsfilter" , "sinkcaps");

            m_Caps = gst_caps_from_string("audio/x-raw-int,rate=44100");
            g_object_set(G_OBJECT(m_sinkCapsElement), "caps", m_Caps, NULL);

            gst_caps_unref(m_Caps);
            use_myrate = true;
            //LOG_TRACE << "   modified myrate = " << myrate <<LOG_ENDL;
        }

        //<!-- Added by junmyoung.lee@lge.com 2013.10.19.
        // for over-run
//        g_object_set(G_OBJECT(m_audioqueue), "low-percent",      2,    NULL);
//        g_object_set(G_OBJECT(m_audioqueue), "high-percent",     50,   NULL);
//        g_object_set(G_OBJECT(m_audioqueue), "max-size-buffers", 100,  NULL);
//        g_object_set(G_OBJECT(m_audioqueue), "use-buffering",    true, NULL);

        // for under-run
        g_object_set(G_OBJECT(m_audioqueue), "leaky",                 1,    NULL);
        g_object_set(G_OBJECT(m_audioqueue), "min-threshold-buffers", 30,   NULL);
        g_object_set(G_OBJECT(m_audioqueue), "clear-prebuffering",    true, NULL);
        //--!>

//        g_object_set(G_OBJECT(m_audioqueue), "leaky", 1 , NULL);
//        g_object_set(G_OBJECT(m_audioqueue), "max-size-buffers", 4 * 1024 * 1024, NULL);
//        g_object_set(G_OBJECT(m_audioqueue), "max-size-bytes",   1 * 1024 * 1024, NULL);
//        //g_object_set(G_OBJECT(m_audioqueue), "max-size-time", 500000000, NULL);

//        g_signal_connect(G_OBJECT(m_audioqueue), "underrun", G_CALLBACK(queue_messages_underrun), this);
//        g_signal_connect(G_OBJECT(m_audioqueue), "overrun", G_CALLBACK(queue_messages_overrun), this);
//        g_signal_connect(G_OBJECT(m_audioqueue), "running", G_CALLBACK(queue_messages_running), this);

        LOG_HIGH << "CGSTAudioRouter::start(), use_myrate = " << use_myrate << ", myrate = " << myrate <<LOG_ENDL;
        if( use_myrate )
        {
            gst_bin_add_many( GST_BIN(m_AudioPipeline), m_audioSource,  m_CapsElement, m_audioqueue,
                              m_volume, m_resamplesrc,  m_resamplesink, m_sinkCapsElement, m_AudioSink, NULL);
            gst_element_link_many( m_audioSource, m_CapsElement, m_audioqueue, m_volume, m_resamplesrc, m_resamplesink, m_sinkCapsElement, m_AudioSink, NULL);
        }
        else
        {
            gst_bin_add_many( GST_BIN(m_AudioPipeline), m_audioSource,  m_CapsElement, m_audioqueue, m_volume, m_AudioSink, NULL );
            gst_element_link_many( m_audioSource, m_CapsElement, m_audioqueue, m_volume, m_AudioSink, NULL );
        }
        m_PipeState = eNull;
    }

    //LOG_TRACE << "   call play()" << LOG_ENDL;
    return play();
}

//* open to debug**/
//void CGSTAudioRouter::loggernn(GstDebugCategory * category,
//    GstDebugLevel level, const gchar * file, const gchar * function,
//    gint line, GObject * object, GstDebugMessage * message, gpointer unused)
//{

//    USE_LOG_UTIL
//    LOG_INIT_V2("Media_IPodController");

//    //LOG_TRACE << "MYGST_MESSAGES"<< LOG_ENDL ;
//    const gchar *dbg_msg;

//    dbg_msg = gst_debug_message_get (message);

//    //LOG_TRACE << "MYGST_MESSAGES" << dbg_msg << LOG_ENDL;
//}
///**open to debug*

bool CGSTAudioRouter::stop()
{
    //LOG_TRACE << "CGSTAudioRouter::Stop() IN" << LOG_ENDL;

    if((!m_AudioPipeline) || (m_PipeState == eIdle))
    {
        //LOG_TRACE << "   m_AudioPipeline is false, so return false" << LOG_ENDL;
        return false;
    }

    //Tan remove the AudioFadeOut() when stopping pipeline
    //Stop is called when NOTIFY_AV_MODE comes (e.g.: iPod to Other AV)
    //FadeOut will cause trigger AUDIO SINK_ERROR because AVP will acquire the sound before
    //we release it.
    //AudioFadeOut();

    if(GST_STATE_CHANGE_FAILURE == gst_element_set_state(m_AudioSink, GST_STATE_NULL))
    {
        //LOG_TRACE << "   null Element m_AudioSink state not set" << LOG_ENDL;
    }
    else
    {
        //LOG_TRACE << "   null Element m_AudioSink state set" << LOG_ENDL;
    }

    if(GST_STATE_CHANGE_FAILURE == gst_element_set_state(m_AudioPipeline, GST_STATE_NULL))
    {
        //LOG_TRACE << "   null Element m_AudioPipeline state not set" << LOG_ENDL;
    }
    else
    {
        //LOG_TRACE << "   null Element m_AudioPipeline state set" << LOG_ENDL;
    }

    if( GST_STATE_CHANGE_SUCCESS == gst_element_get_state( m_AudioPipeline, NULL, NULL, -1 ))
    {
        m_PipeState = eIdle;
        //LOG_TRACE << "   get element m_AudioPipeline, m_PipeState = " << m_PipeState << LOG_ENDL;
    }

    if( m_AudioPipeline )
    {
        //LOG_TRACE << "   m_AudioPipeline is exist, so clean the pipe_" << __LINE__ << LOG_ENDL;
        gst_object_unref (m_AudioPipeline);
    }

    if( m_GSTBus_Watch_Id > 0 )
        g_source_remove (m_GSTBus_Watch_Id);

    m_GSTBus_Watch_Id = 0;

    m_audioSource     = NULL;
    m_AudioSink       = NULL;
    m_CapsElement     = NULL;
    m_AudioPipeline   = NULL;
    m_resamplesrc     = NULL;
    m_resamplesink    = NULL;
    m_sinkCapsElement = NULL;

    //LOG_TRACE << "CGSTAudioRouter::Stop() OUT" << LOG_ENDL;
    return true;
}

gboolean CGSTAudioRouter::gst_Bus_callback (GstBus */*bus*/, GstMessage *message, gpointer  data)
{
    USE_LOG_UTIL
    LOG_INIT_V2("Media_IPodController");
    ////LOG_TRACE << "C1GSTAudioRouter in gstbus call back in" << LOG_ENDL;

    CGSTAudioRouter *caller  = (CGSTAudioRouter*)data;

    //LOG_HIGH << "CGSTAudioRouter::gst_Bus_callback(), Got message = " << GST_MESSAGE_TYPE_NAME (message)  << ", message type = " << GST_MESSAGE_TYPE (message)<< LOG_ENDL;
    switch (GST_MESSAGE_TYPE (message))
    {
        case GST_MESSAGE_ERROR:
        {
            GError *err;
            gchar  *debug;
            gst_message_parse_error (message, &err, &debug);
            LOG_HIGH << "CGSTAudioRouter::gst_Bus_callback(), TypeName = " << GST_MESSAGE_TYPE_NAME (message) << ", \"" << debug << "\"" << LOG_ENDL;
            if(caller) caller->stop();

            g_error_free(err);
            g_free (debug);

            break;
        }
        case GST_MESSAGE_EOS:
            ////LOG_TRACE << "in gstbus call back EOS" << LOG_ENDL;
            break;
        case GST_MESSAGE_WARNING:
        {
            ////LOG_TRACE << "in gstbus call back warning" << LOG_ENDL;
            //gst_message_parse_warning(message, &err, &debug);
            //gst_message_parse_error (message, &err, &debug);
            ////LOG_TRACE << "CGSTAudioRouter Warning: " << err->message   << LOG_ENDL;
            //g_error_free (err);
            //g_free (debug);
        }
            break;
        case GST_MESSAGE_STATE_CHANGED:
        {
            GstState oldState, newState, pending;
            gst_message_parse_state_changed (message, &oldState, &newState, &pending);
            //LOG_TRACE << "CGSTAudioRouter::gst_Bus_callback(), TypeName = " << GST_MESSAGE_TYPE_NAME (message) << ", old state = " << oldState << ", new state = " << newState << LOG_ENDL;
        }
            break;
        default:
            /* unhandled message */
            break;
    } // switch
    return true;
}


void CGSTAudioRouter::AudioFadeIn()
{
    //LOG_TRACE<<"CGSTAudioRouter::AudioFadeIn() IN"<<LOG_ENDL;
    g_object_set(G_OBJECT(m_volume),"volume", 0.0, NULL);
    usleep(7000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.1, NULL);
    usleep(7000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.2, NULL);
    usleep(7000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.3, NULL);
    usleep(7000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.4, NULL);
    usleep(7000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.5, NULL);
    usleep(7000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.6, NULL);
    usleep(7000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.7, NULL);
    usleep(7000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.8, NULL);
    usleep(7000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.9, NULL);
    usleep(7000);
    g_object_set(G_OBJECT(m_volume),"volume", 1.0, NULL);
    //usleep(7000);
    //LOG_TRACE<<"CGSTAudioRouter::AudioFadeIn() OUT"<<LOG_ENDL;
}

void CGSTAudioRouter::AudioFadeOut()
{
    //LOG_TRACE<<"CGSTAudioRouter::AudioFadeOut() IN"<<LOG_ENDL;
    g_object_set(G_OBJECT(m_volume),"volume", 1.0, NULL);
    usleep(8000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.9, NULL);
    usleep(8000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.8, NULL);
    usleep(8000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.7, NULL);
    usleep(8000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.6, NULL);
    usleep(8000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.5, NULL);
    usleep(8000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.4, NULL);
    usleep(8000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.3, NULL);
    usleep(8000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.2, NULL);
    usleep(8000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.1, NULL);
    usleep(8000);
    g_object_set(G_OBJECT(m_volume),"volume", 0.0, NULL);
    usleep(8000);
    usleep(380000);
    //LOG_TRACE<<"CGSTAudioRouter::AudioFadeOut() OUT"<<LOG_ENDL;
}

void
CGSTAudioRouter::AudioFadeInSlot()
{
    ////LOG_TRACE << "CGSTAudioRouter::AudioFadeInSlot() IN"  << LOG_ENDL;
    AudioFadeIn();
    ////LOG_TRACE << "CGSTAudioRouter::AudioFadeInSlot() OUT" << LOG_ENDL;
}

void
CGSTAudioRouter::AudioFadeOutSlot()
{
    ////LOG_TRACE << "CGSTAudioRouter::AudioFadeOutSlot IN"  << LOG_ENDL;
    AudioFadeOut();
    ////LOG_TRACE << "CGSTAudioRouter::AudioFadeOutSlot OUT" << LOG_ENDL;
}
