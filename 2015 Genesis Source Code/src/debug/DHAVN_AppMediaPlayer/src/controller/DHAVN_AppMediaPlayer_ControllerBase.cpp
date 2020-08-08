#include "controller/DHAVN_AppMediaPlayer_ControllerBase.h"
#include "engine/DHAVN_AppMediaPlayer_EngineMain.h"

AppMediaPlayer_Controller_Base::AppMediaPlayer_Controller_Base( AppMediaPlayer_EngineMain &engineMain )
   : m_EngineMain( engineMain ),
     m_Controllers( engineMain.GetControllers() ),
// { modified by eugene 2012.08.22 for static analysis defect #7053
     // m_pRPMClient(NULL), // removed by kihyung 2012.11.03 for booting disable
     m_pDeckController(NULL),
     m_pIPodController(NULL)
// } modified by eugene 2012.08.22 for static analysis defect #7053
{
}

AppMediaPlayer_Controller_Base::~AppMediaPlayer_Controller_Base()
{
}

/** @copydoc AppMediaPlayer_Controller_Base::AcquireMediaSource() */
void AppMediaPlayer_Controller_Base::AcquireMediaSource( MEDIA_SOURCE_T aSource )
{
    // { modified by kihyung 2013.1.26
    QObject *controller = NULL;
    IMediaInterface *pIMedia = NULL;
    
    switch( aSource )
    {
    case MEDIA_SOURCE_JUKEBOX:
        controller = m_Controllers.GetJukeBoxController();
        pIMedia    = m_Controllers.GetJukeBoxController()->GetMediaInstance();
        break;

    case MEDIA_SOURCE_USB1:
    case MEDIA_SOURCE_USB2:
       controller = m_Controllers.GetUSBController();
       pIMedia    = m_Controllers.GetUSBController()->GetMediaInstance();
       break;

    case MEDIA_SOURCE_DVDVIDEO: // fall through
    case MEDIA_SOURCE_VCD:      // fall through
    case MEDIA_SOURCE_MP3:      // fall through
    case MEDIA_SOURCE_DVDAUDIO: // fall through
    case MEDIA_SOURCE_CDDA:
        controller = m_Controllers.GetDeckController();
        pIMedia    = m_Controllers.GetDeckController()->GetMediaInstance();
        break;

    case MEDIA_SOURCE_IPOD1:
        controller = m_Controllers.GetIPod1Controller();
        pIMedia    = (IMediaInterface*)m_Controllers.GetIPod1Controller();
        break;

    case MEDIA_SOURCE_IPOD2:
        controller = m_Controllers.GetIPod2Controller();
        pIMedia    = (IMediaInterface*)m_Controllers.GetIPod2Controller();
        break;

    default:
        break; // make compiler happy
    }

    if ( controller )
    {
        connect( controller, SIGNAL(OnPositionChanged(int)),
                 this, SLOT(onPositionChanged(int)));
        connect( controller, SIGNAL(OnStateChanged(int)),
                 this, SLOT(onStateChanged(int)));
        connect( controller, SIGNAL(OnTrackChanged(int)),
                 this, SLOT(onTrackChanged(int)));
        connect( controller, SIGNAL(OnDurationChanged(int)),
                 this, SLOT(onDurationChanged(int)));
        connect( controller, SIGNAL(OnModeChanged(int)),
                 this, SLOT(onModeChanged(int)));
        connect( controller, SIGNAL(OnError(int)),
                 this, SLOT(onError(int)));

        // { added by kihyung 2013.1.25 for update first controller state
        if(pIMedia) {
            onStateChanged(pIMedia->GetCurrentMediaState());
        }
        // } added by kihyung 2013.1.25
    }
    else
    {
        MP_LOG << "AppMediaPlayer_Controller_Base::AcquireMediaSource()"
                 << "No idea what to do with media source " << aSource << LOG_ENDL;
    }
    // } modified by kihyung 2013.1.26
}

/** @copydoc AppMediaPlayer_Controller_Base::ReleaseMediaSource() */
void AppMediaPlayer_Controller_Base::ReleaseMediaSource( MEDIA_SOURCE_T aSource )
{
    switch( aSource )
    {
    case MEDIA_SOURCE_JUKEBOX:
        disconnect( m_Controllers.GetJukeBoxController(), 0, this, 0 );
        break;

    case MEDIA_SOURCE_USB1:
    case MEDIA_SOURCE_USB2:
        disconnect( m_Controllers.GetUSBController(), 0, this, 0 );
        break;

    case MEDIA_SOURCE_DVDVIDEO: // fall through
    case MEDIA_SOURCE_VCD:      // fall through
    case MEDIA_SOURCE_MP3:      // fall through
    case MEDIA_SOURCE_DVDAUDIO: // fall through
    case MEDIA_SOURCE_CDDA:
        disconnect( m_Controllers.GetDeckController(), 0, this, 0 );
        break;

    case MEDIA_SOURCE_IPOD1:
        disconnect( m_Controllers.GetIPod1Controller(), 0, 0, 0 );
        break;

    case MEDIA_SOURCE_IPOD2:
        disconnect( m_Controllers.GetIPod2Controller(), 0, 0, 0 );
        break;

    default: // make compiler happy
        break;
    }
}
