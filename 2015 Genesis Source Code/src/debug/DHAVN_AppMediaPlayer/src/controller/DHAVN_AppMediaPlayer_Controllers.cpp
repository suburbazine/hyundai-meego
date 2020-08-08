#include "controller/DHAVN_AppMediaPlayer_Controllers.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include <QSettings> // added by kihyung 2012.11.03

// { added by ruinseve for CR#12584
QString fileFormatPath = "/home/meego/.config/PE_config.conf";
QString audioFormatKey = "Supported_Audio_File_Formats";
QString videoFormatKey = "Supported_Video_File_Formats";
QString photoFormatKey = "Supported_Image_File_Formats";
// } added by ruinseve for CR#1258.


AppMediaPlayer_Controllers::AppMediaPlayer_Controllers(int nVehicleType)
    : QObject(NULL),
      m_pUSB1Controller(NULL),
      m_pUSB2Controller(NULL),
      m_pUSB3Controller(NULL),
      m_pJukeBox1Controller(NULL),
      m_pJukeBox2Controller(NULL),
      m_pDeckController(NULL),
      m_pIPod1Controller(NULL),
      m_pIPod2Controller(NULL)
{
    QString appName("AppMediaPlayer");

    // { removed by kihyung 2012.11.03 for booting disable 
    /*
    m_pRPMClient = new QRPMClient(&appName);
    connect( m_pRPMClient, SIGNAL(RPMClientAcquireResponse(TAudioMode,TAudioSink)),
             this, SLOT(onRPMAcquireResponse(TAudioMode, TAudioSink)));
    */
    // } removed by kihyung 2012.11.03 for booting disable

    m_nVehicleType = nVehicleType;

    m_resourceMap[MEDIAPLAYER_RESOURCE_AUDIO] = NULL;
    m_resourceMap[MEDIAPLAYER_RESOURCE_GRAPHICS] = NULL;

    initExtLists(); // { added by ruinseve for CR#12584
}


AppMediaPlayer_Controllers::~AppMediaPlayer_Controllers()
{
    delete m_pUSB1Controller;
    delete m_pUSB2Controller;
    delete m_pUSB3Controller;
    delete m_pJukeBox1Controller;
    delete m_pJukeBox2Controller;
    delete m_pDeckController;
    delete m_pIPod1Controller;
    delete m_pIPod2Controller;
    // delete m_pRPMClient; // removed by kihyung 2012.11.03 for booting disable
}

/** @copydoc AppMediaPlayer_Controller_Base::SetAudioPath() */
// { removed by kihyung 2012.11.03 for booting disable
/*
void AppMediaPlayer_Controllers::SetAudioPath( TAudioMode aMode )
{
    // m_pRPMClient->RPMClientAcquire( aMode, eMain ); 
}
*/
// } removed by kihyung 2012.11.03 for booting disable

// { removed by kihyung 2012.11.03 for booting disable
/*
void AppMediaPlayer_Controllers::onRPMAcquireResponse(TAudioMode aAudioMode, TAudioSink aAudioSink)
{
    MP_LOG << "[MP] AppMediaPlayer_Controllers::" << __func__
             << "mode=" << aAudioMode
             << "synk=" << aAudioSink << LOG_ENDL;
}
*/
// } removed by kihyung 2012.11.03 for booting disable

/* resource management stuff */
/** @copydoc AppMediaPlayer_Controllers::RequestResources() */
void AppMediaPlayer_Controllers::RequestResources( MEDIAPLAYER_RESOURCES_T resources,
                                                   QObject *newOwner )
{
    m_pendingResourceRequest.resources = resources;
    m_pendingResourceRequest.owner = newOwner;
    
    QSet<QObject *> waitList;

    if ( resources & MEDIAPLAYER_RESOURCE_AUDIO
         && m_resourceMap[MEDIAPLAYER_RESOURCE_AUDIO] != NULL
         && m_resourceMap[MEDIAPLAYER_RESOURCE_AUDIO] != newOwner )
    {
        waitList << m_resourceMap[MEDIAPLAYER_RESOURCE_AUDIO];
    }

    if ( resources & MEDIAPLAYER_RESOURCE_GRAPHICS
         && m_resourceMap[MEDIAPLAYER_RESOURCE_GRAPHICS] != NULL
         && m_resourceMap[MEDIAPLAYER_RESOURCE_GRAPHICS] != newOwner)
    {
        waitList << m_resourceMap[MEDIAPLAYER_RESOURCE_GRAPHICS];
    }

    if ( waitList.empty() )
    {
       MP_LOG << "waitList empty" << LOG_ENDL;
       tryGrantResources();
    }
    else
    {
        // Request the current owners to drop the resources.
        QObject *owner;
        foreach( owner, waitList )
        {
            QMetaObject::invokeMethod( owner,
                                       "onReleaseResources",
                                       Q_ARG(MEDIAPLAYER_RESOURCES_T, resources) );
        }
    }
}


void AppMediaPlayer_Controllers::onResourceReleased( MEDIAPLAYER_RESOURCES_T resource )
{
    // It depends on resource request...
    if ( resource & MEDIAPLAYER_RESOURCE_AUDIO ) {
        m_resourceMap[MEDIAPLAYER_RESOURCE_AUDIO] = NULL;
    }

    if ( resource & MEDIAPLAYER_RESOURCE_GRAPHICS ) {
        m_resourceMap[MEDIAPLAYER_RESOURCE_GRAPHICS] = NULL;
    }

    tryGrantResources();
}

void AppMediaPlayer_Controllers::tryGrantResources()
{
    MEDIAPLAYER_RESOURCES_T resources = m_pendingResourceRequest.resources;
    
    if ( m_pendingResourceRequest.resources != 0 &&
         m_pendingResourceRequest.owner != NULL )
    {
        // Check if resources are free:
        if ( resources & MEDIAPLAYER_RESOURCE_AUDIO
             && m_resourceMap[MEDIAPLAYER_RESOURCE_AUDIO] != NULL
             && m_resourceMap[MEDIAPLAYER_RESOURCE_AUDIO] != m_pendingResourceRequest.owner )
        {
            return;
        }
        if ( resources & MEDIAPLAYER_RESOURCE_GRAPHICS
             && m_resourceMap[MEDIAPLAYER_RESOURCE_GRAPHICS] != NULL
             && m_resourceMap[MEDIAPLAYER_RESOURCE_GRAPHICS] != m_pendingResourceRequest.owner )
        {
            return;
        }

        // If still here (all resources are free), grant them:
        if ( resources & MEDIAPLAYER_RESOURCE_AUDIO )
        {
            m_resourceMap[MEDIAPLAYER_RESOURCE_AUDIO] = m_pendingResourceRequest.owner;
        }

        if ( resources & MEDIAPLAYER_RESOURCE_GRAPHICS )
        {
            m_resourceMap[MEDIAPLAYER_RESOURCE_GRAPHICS] = m_pendingResourceRequest.owner;
        }

        // Finally, notify the new resource owner, that resources are
        // available for use
        QMetaObject::invokeMethod( m_pendingResourceRequest.owner,
                                   "onResourceGranted",
                                   Qt::DirectConnection );
        m_pendingResourceRequest.resources = 0;
        m_pendingResourceRequest.owner = NULL;
    }
}

// { added by ruinseve for CR#12584
QStringList AppMediaPlayer_Controllers::getExtList(MEDIA_TYPE_T type)
{
    MP_LOG << "type =" << type << LOG_ENDL;

    QStringList result;

    switch(type)
    {
        case MEDIA_TYPE_MUSIC:
        {
            result = m_audioExtList;
            break;
        }
        case MEDIA_TYPE_VIDEO:
        {
            result = m_videoExtList;
            break;
        }
        case MEDIA_TYPE_PHOTO:
        {
            result = m_photoExtList;
            break;
        }
        default:
        {
            MP_LOG << "Unsupportable type" << LOG_ENDL;
            break;
        }
    }

    return result;
}

// Define which files players are able to play.
void AppMediaPlayer_Controllers::initExtLists()
{
    QSettings settings(fileFormatPath, QSettings::IniFormat);       // open file

    // For audio
    QString filterStr = settings.value(audioFormatKey).toString();   // get string contained extenshions
    filterStr.prepend("*").replace("<<","<<*");     // add '*' to search audios files by extenshion
    m_audioExtList = filterStr.split("<<");           // split string and put formats to name filter

    // For video
    filterStr = settings.value(videoFormatKey).toString();
    filterStr.prepend("*").replace("<<","<<*");
    m_videoExtList = filterStr.split("<<");

    // For photo
    filterStr = settings.value(photoFormatKey).toString();
    filterStr.prepend("*").replace("<<","<<*");
    m_photoExtList = filterStr.split("<<");
}
// } added by ruinseve for CR#12584.
