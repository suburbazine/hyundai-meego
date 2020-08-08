#ifndef DHAVN_GSTAUDIOROUTER_H
#define DHAVN_GSTAUDIOROUTER_H
#include <QObject>
#include <gst/gst.h>
#include <glib.h>
#include <DHAVN_LogUtil.h>
#include <QDBusConnection>

const char * const AUDIOROUTER_DBUS_NAME           = "com.lg.qt.audiorouter";
const char * const AUDIOROUTER_DBUS_PATH           = "/audiorouter";
const char * const AUDIOROUTER_DBUS_INTERFACE_NAME = "com.lg.qt.interface.audiorouter";
const char * const AUDIOROUTER_DBUS_METHOD         = "control";

class  CGSTAudioRouter : public QObject
{
    Q_OBJECT
public:
    enum EPipeState
    {
        eIdle = 0,
        ePlay,
        ePause,
        eNull
    };

    USE_LOG_UTIL
public:
    CGSTAudioRouter();
    ~CGSTAudioRouter();

    void setSource(QString src);
    void setSink(QString sink="DEFAULT");
    void setSyncType(bool sync = false);
    void setSampleRate(QString string);

    // Added by jonghwan.cho@lge.com 2013.11.07.
    QString GetCardSink() { return m_CardSink; }
    QString GetCardSrc()  { return m_CardSrc;  }
    QString GetRate()     { return m_Rate;     }
    EPipeState GetPipeState() { return m_PipeState; }

    bool start();
    bool stop();
    bool pause();
    bool play();

    void AudioFadeIn();
    void AudioFadeOut();

private:
    static gboolean gst_Bus_callback (GstBus *bus, GstMessage *message, gpointer    data);
//    static void     loggernn( GstDebugCategory * category,
//                              GstDebugLevel level, const gchar * file, const gchar * function,
//                              gint line, GObject * object, GstDebugMessage * message, gpointer unused);
    void InitializeDBusConnect();

//    static void queue_messages_underrun(GstElement * , gpointer );
//    static void queue_messages_overrun(GstElemen * , gpointer );
//    static void queue_messages_running(GstElement * , gpointer );

public Q_SLOTS:
    void AudioFadeOutSlot();
    void AudioFadeInSlot();

private:
    QString     m_CardSink;  // eg - hw:0,1,0
    QString     m_CardSrc;   // eg - hw:2,0
    bool        m_SyncType;

    GstElement* m_AudioPipeline;
    GstElement* m_audioSource, *m_audioconvert, *m_volume;
    GstElement* m_AudioSink, *m_CapsElement, *m_audioqueue, *m_sinkCapsElement;
    GstElement* m_resamplesrc;
    GstElement* m_resamplesink;

    guint       m_GSTBus_Watch_Id;
    EPipeState  m_PipeState;
    QString     m_Rate;

    GstBus      *m_GSTBus;
    GstCaps     *m_Caps;
};

#endif // DHAVN_GSTAUDIOROUTER_H
