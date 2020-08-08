#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_VCD.h"

AppMediaPlayer_Video_CModel_VCD::AppMediaPlayer_Video_CModel_VCD( QObject *parent )
    : AppMediaPlayer_Video_CModel_Base( parent )
{
    m_trackCount = 0;
    m_currentTrack = 0;
    m_isPbcMenuDisplayed = false;
    m_isPbcMenuShown = false; // added by cychoi 2013.12.19 for ITS 215825 Default focus
    m_isPbcMode = false; // { added by Sergey for CR#14692
    m_nVcdVersion = VCD_VERSION_UNDEFINED;
    m_PlayResumePosition = 0;
    m_tuneWheelTrack = 0; // added by kihyung 2012.07.17 for CR 8042
    m_isTrackLoading = false; // modified by eugene 2012.10.29 for static analysis defect #7096
}

AppMediaPlayer_Video_CModel_VCD::~AppMediaPlayer_Video_CModel_VCD()
{
}

void AppMediaPlayer_Video_CModel_VCD::setTrackCount(int value)
{
    MP_MEDIUM << "value =" << value << LOG_ENDL;
    m_trackCount = value;
    emit trackCountChanged( value );
}

void AppMediaPlayer_Video_CModel_VCD::setCurrentTrack(int value)
{
    MP_MEDIUM << "value =" << value << LOG_ENDL;
    m_currentTrack = value;
    emit currentTrackChanged( value );
}

void AppMediaPlayer_Video_CModel_VCD::setIsPbcMenuDisplayed(bool value)
{
    // { modified by kihyung 2013.1.9 for ISV 70642
    MP_MEDIUM << "value =" << value << LOG_ENDL;
    if(m_isPbcMenuDisplayed != value) {
        m_isPbcMenuDisplayed = value;
        emit isPbcMenuDisplayedChanged( value );
        m_isPbcMenuShown = value; // added by cychoi 2013.12.19 for ITS 215825 Default focus
    }
    // } modified by kihyung 2013.1.9 for ISV 70642    
}

// { modified by eugene 2012.08.30
/* 
void AppMediaPlayer_Video_CModel_VCD::setIsPbcMode(bool value)
{
   MP_LOG << "value =" << value << LOG_ENDL;
   m_bIsPbcMode = value;
}
*/
// } modified by eugene 2012.08.30


void AppMediaPlayer_Video_CModel_VCD::setVcdVersion(int value)
{
   MP_MEDIUM << "value =" << value << LOG_ENDL;
   m_nVcdVersion = value;
}

void AppMediaPlayer_Video_CModel_VCD::setResumePosition( int value )
{
    MP_MEDIUM << " value " << value << LOG_ENDL;
    m_PlayResumePosition = value;
}


// { added by kihyung 2012.07.17 for CR 8042
void AppMediaPlayer_Video_CModel_VCD::setTuneWheelTrack( int value )
{
    MP_MEDIUM << "value =" << value << LOG_ENDL;
    m_tuneWheelTrack = value;
    emit tuneWheelTrackChanged(value);
}
// } added by kihyung 2012.07.17

// { added by Sergey Vetugov for CR#13620
void AppMediaPlayer_Video_CModel_VCD::setIsTrackLoading( bool value )
{
    // { added by cychoi 2014.06.27 for removed too many logs...
    if(m_isTrackLoading == value)
    {
        return;
    }
    // } added by cychoi 2014.06.27

    MP_MEDIUM << "value =" << value << LOG_ENDL;

    m_isTrackLoading = value;
    emit isTrackLoadingChanged(value);
}
// } added by Sergey Vetugov for CR#13620.

// { added by Sergey for CR#14692
void AppMediaPlayer_Video_CModel_VCD::setIsPbcMode(bool value)
{
    MP_MEDIUM << "value =" << value << LOG_ENDL;

    m_isPbcMode = value;
    //emit isPbcModeChanged(value); // commented by cychoi 2013.07.03 for ITS 178068
}
// } added by Sergey for CR#14692.
