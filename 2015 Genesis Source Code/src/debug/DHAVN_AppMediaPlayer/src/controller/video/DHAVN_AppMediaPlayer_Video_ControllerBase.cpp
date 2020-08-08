#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerBase.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h"

#include <usbcontroller.h>
#include <jukeboxcontroller.h>

class AppMediaPlayer_Video_CModel_Base;

void AppMediaPlayer_Video_ControllerBase::play()
{
    if(m_pIMedia)
    {
        m_pIMedia->Play();
    }
}

void AppMediaPlayer_Video_ControllerBase::pause()
{
    if(m_pIMedia)
    {
        m_pIMedia->Pause();
    }
}

void AppMediaPlayer_Video_ControllerBase::togglePlay()
{
    if ( getModel()->playbackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING )
    {
        pause();
    }
    else
    {
        play();
    }
}

void AppMediaPlayer_Video_ControllerBase::toggleRepeat()
{
    switch(getModel()->repeatMode())
    {
    // { modified by kihyung 2012.06.28
    /*
    case AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_OFF:
    case AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_FILE:
    {
        repeatPlayFolder();
        break;
    }

    case AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_FOLDER:
    {
        repeatPlayFile();
        break;
    }

    default:
    {
        //ignore
        break;
    }
    */
    // { modified by ravikanth - 12-09-19
    case AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_OFF:
        repeatPlayFolder();
        break;
    case AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_FOLDER:
        repeatPlayFile();
        break;
    case AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_FILE:
        repeatPlayAll();
        break;
    // } modified by ravikanth - 12-09-19
    default:
        //ignore
        break;
    // } modified by kihyung 
    }
}


void AppMediaPlayer_Video_ControllerBase::toggleRandom()
{
    switch( getModel()->randomMode() )
    {
    // { modified by kihyung 2012.06.28
    /*
    case AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_OFF:
    {
    randomPlayFolder();
    break;
    }

    case AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_FOLDER:
    case AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_FILE:
    {
    randomPlayOff();
    break;
    }

    default:
    {
    //ignore
    break;
    }
    */
    case AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_OFF:
        randomPlayFile();
        break;
    case AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_FILE:
    case AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_FOLDER:    
        randomPlayOff();
        break;
    default:
        //ignore
        break;
    // } modified by kihyung
    }
}


void AppMediaPlayer_Video_ControllerBase::stop()
{
    if(m_pIMedia)
    {
        m_pIMedia->Stop();
    }
}

// { added by kihyung 2012.07.31 for C 11949
void AppMediaPlayer_Video_ControllerBase::functionStop()
{
    if(m_pIMedia)
    {
        m_pIMedia->FunctionStop();
    }
}
// } added by kihyung

void AppMediaPlayer_Video_ControllerBase::next()
{
    if(m_pIMedia)
    {
        m_pIMedia->Next(1);
    }
}
void AppMediaPlayer_Video_ControllerBase::prev(bool bForce ) //modified by edo.lee 2013.06.11
{
    if(m_pIMedia)
    {
        m_pIMedia->Prev(1, bForce);
    }
}


void AppMediaPlayer_Video_ControllerBase::setPosition( int position )
{
    // getModel()->setPosition( position ); // removed by Sergey 15.05.2013
    if(m_pIMedia)
    {
        m_pIMedia->SetCurrentMediaPosition(position);
    }
}


void AppMediaPlayer_Video_ControllerBase::randomPlayOff()
{
    if ( m_pIMedia )
    {
        m_pIMedia->RandomPlay( RANDOMOFF );
    }
}


void AppMediaPlayer_Video_ControllerBase::randomPlayFile()
{
    if ( m_pIMedia )
    {
        m_pIMedia->RandomPlay( RANDOMFILE );
    }
}

void AppMediaPlayer_Video_ControllerBase::randomPlayFolder()
{
    if( m_pIMedia )
    {
        m_pIMedia->RandomPlay( RANDOMFOLDER );
    }
}

void AppMediaPlayer_Video_ControllerBase::repeatPlayOff()
{
    if ( m_pIMedia )
    {
        m_pIMedia->RepeatPlay( REPEATOFF );
    }
}

void AppMediaPlayer_Video_ControllerBase::repeatPlayFile()
{
    if( m_pIMedia )
    {
       m_pIMedia->RepeatPlay( REPEATFILE );
    }
}

void AppMediaPlayer_Video_ControllerBase::repeatPlayFolder()
{
    if ( m_pIMedia )
    {
        m_pIMedia->RepeatPlay( REPEATFOLDER );
    }
}

// { added by ravikanth - 12-09-19
void AppMediaPlayer_Video_ControllerBase::repeatPlayAll()
{
    if ( m_pIMedia )
    {
        m_pIMedia->RepeatPlay( REPEATALL );
    }
}
// } added by ravikanth - 12-09-19

void AppMediaPlayer_Video_ControllerBase::cancel_ff_rew()
{
    if ( m_pIMedia )
    {
        m_pIMedia->Play();
    }
}
