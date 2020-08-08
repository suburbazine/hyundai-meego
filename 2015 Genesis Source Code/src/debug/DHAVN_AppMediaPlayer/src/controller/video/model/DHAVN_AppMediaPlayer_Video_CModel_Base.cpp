#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include  <QtGui>

AppMediaPlayer_Video_CModel_Base::AppMediaPlayer_Video_CModel_Base( QObject *parent )
    : QObject(parent)
{
    m_playbackStatus = PLAYBACK_STATUS_STOPPED;
    m_randomMode = RANDOM_MODE_OFF;
    m_repeatMode = REPEAT_MODE_OFF; // modified by ravikanth - 12-09-19
    m_scanMode   = SCAN_MODE_OFF; // added by kihyung 2012.07.26 for CR 11894
    m_position = 0;
    m_isDirty = false;
    m_isDragging = false;
    m_loadingMode = false; // added by Sergey for Loading popup
    m_removingMode = false; // added by kihyung 2013.09.15 for ISV 91113
}

AppMediaPlayer_Video_CModel_Base::~AppMediaPlayer_Video_CModel_Base()
{
}

void AppMediaPlayer_Video_CModel_Base::setPlaybackStatus(PLAYBACK_STATUS_T value)
{
    m_playbackStatus = value;
    emit playbackStatusChanged( value );
}


void AppMediaPlayer_Video_CModel_Base::setRandomMode(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T value)
{
    m_randomMode = value;
    emit randomModeChanged( value );
}


void AppMediaPlayer_Video_CModel_Base::setRepeatMode(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T value)
{
    m_repeatMode = value;
    emit repeatModeChanged( value );
}

// { added by kihyung 2012.07.26 for CR 11894
void AppMediaPlayer_Video_CModel_Base::setScanMode(AppMediaPlayer_Video_CModel_Base::SCAN_MODE_T value)
{
    m_scanMode = value;
    emit scanModeChanged( value );
}
// } added by kihyung

void AppMediaPlayer_Video_CModel_Base::setPosition( int value )
{
    m_position = value;
    emit positionChanged( value );
}

void AppMediaPlayer_Video_CModel_Base::setDirty( bool value )
{
    MP_LOG << "value =" << value << LOG_ENDL;
    m_isDirty = value;
    emit dirtyChanged( value );
}

void AppMediaPlayer_Video_CModel_Base::setDragging( bool value )
{
    MP_LOG << "value =" << value << LOG_ENDL;
    m_isDragging = value;
    emit draggingChanged( value );
}

void AppMediaPlayer_Video_CModel_Base::setLoadingMode( bool value )
{
    if(value == true) 
    {
        setRemovingMode(false);
    }
        
    m_loadingMode = value;
    emit loadingModeChanged(value);
} // modified by Sergey 18.05.2013

void AppMediaPlayer_Video_CModel_Base::setRemovingMode( bool value ) 
{
    m_removingMode = value;
    emit removingModeChanged(value);
}

