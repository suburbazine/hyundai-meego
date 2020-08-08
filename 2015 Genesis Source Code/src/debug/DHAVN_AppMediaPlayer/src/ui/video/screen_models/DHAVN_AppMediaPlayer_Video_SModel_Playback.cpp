
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_Playback.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include  <QtGui>

#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_Playback.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_DVD.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_VCD.h"

AppMediaPlayer_Video_SModel_Playback::AppMediaPlayer_Video_SModel_Playback( QObject *parent )
    : QObject(parent),
// { modified by eugene 2012.08.22 for static analysis defect #6952
	// removed by Sergey 01.05.2013 for Tune Functionality
    m_bindedAppMediaPlayer_Video_CModel_FS(NULL),
    m_bindedAppMediaPlayer_Video_CModel_DVD(NULL),
    m_bindedAppMediaPlayer_Video_CModel_VCD(NULL)
// } modified by eugene 2012.08.22 for static analysis defect #6952
{
    m_filename = QString("");
    m_filename2 = QString("");
    m_subtitleStr = QString("");
    m_position = 0;
    m_duration = 0;
    m_countInfo = QString("");
    m_repeatMode = 0;
    m_randomMode = 0;
    m_scanMode   = 0; // added by kihyung 2012.07.26 for CR 11894
    m_progressBarMode = QString("");
    m_playbackStatus = 0;
    m_lockoutMode = false;
    m_loadingMode = false; // added by Sergey for CR16250
    m_captionSize = 0;
    m_captionEnable = false;  // added by kihyung 2013.06.14
    m_tuneMode = false;
    m_focusVisible = false;
    m_focusActivity = false;
    m_camMode = false; // added by wspark 2013.02.21 for ISV 73305
    m_divxCaptionVisible = false; // modified by ravikanth 04-04-13
    m_pbcMode = false; // added by wspark 2013.04.15 for ITS 162431
    m_tuneSameFile = false; // added by sungha.choi 2013.07.15 for static test
    m_fgLoadingMode = false; // added by jaewhan.lee 2013.10.25 for static test
    m_removingMode = false; // added by wspark 2014.02.24 for ITS 226667
}

void AppMediaPlayer_Video_SModel_Playback::bindWith( AppMediaPlayer_Video_CModel_FS *target )
{
    m_bindedAppMediaPlayer_Video_CModel_FS = target;
    connect( target, SIGNAL(subtileStrChanged(QString)),
             this, SLOT(setSubtitleStr(QString)));
    setSubtitleStr(target->subtileStr());
    connect( target, SIGNAL(captionSizeChanged(int)),
             this, SLOT(setCaptionSize(int)));
    setCaptionSize(target->captionSize());
    connect( target, SIGNAL(captionEnableChanged(bool)),
             this, SLOT(setCaptionEnable(bool)));  // added by kihyung 2013.06.14
    setCaptionEnable(target->captionEnable());  // added by kihyung 2013.06.14
    connect( target, SIGNAL(playlistLengthChanged(int)),
             this, SLOT(onAppMediaPlayer_Video_CModel_FSplaylistLengthChanged(int)));
    connect( target, SIGNAL(playlistPositionChanged(int)),
             this, SLOT(onAppMediaPlayer_Video_CModel_FSplaylistPositionChanged(int)));
    setCountInfo( QString("%1/%2").arg(m_bindedAppMediaPlayer_Video_CModel_FS->displayPlaylistPos()).arg(m_bindedAppMediaPlayer_Video_CModel_FS->displayPlaylistLength()) );  // modified by kihyung 2013.09.11 for ITS 0189474
    connect( target, SIGNAL(positionChanged(int)),
             this, SLOT(onAppMediaPlayer_Video_CModel_FSpositionChanged(int)));
	// modified by richard 2012.10.25 for navi slowness fix		 
    //setPosition( (m_bindedAppMediaPlayer_Video_CModel_FS->position()/1000) );
	setPosition( m_bindedAppMediaPlayer_Video_CModel_FS->position() );
    connect( target, SIGNAL(durationChanged(int)),
             this, SLOT(onAppMediaPlayer_Video_CModel_FSdurationChanged(int)));

    connect( target, SIGNAL(tuneSameFileChanged(bool)),this, SLOT(setTuneSameFile(bool)));// added by yongkyun.lee  2012.10.17  for Current file is white color
	// modified by richard 2012.10.25 for navi slowness fix		 
    //setDuration( (m_bindedAppMediaPlayer_Video_CModel_FS->duration()/1000) );
	setDuration( m_bindedAppMediaPlayer_Video_CModel_FS->duration() );
    connect( target, SIGNAL(playbackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T)),
             this, SLOT(onAppMediaPlayer_Video_CModel_FSplaybackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T)));
    setPlaybackStatus( (m_bindedAppMediaPlayer_Video_CModel_FS->playbackStatus()) );
    connect( target, SIGNAL(repeatModeChanged(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T)),
             this, SLOT(onAppMediaPlayer_Video_CModel_FSrepeatModeChanged(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T)));
    setRepeatMode( (m_bindedAppMediaPlayer_Video_CModel_FS->repeatMode()) );
    connect( target, SIGNAL(randomModeChanged(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T)),
             this, SLOT(onAppMediaPlayer_Video_CModel_FSrandomModeChanged(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T)));
    setRandomMode( (m_bindedAppMediaPlayer_Video_CModel_FS->randomMode()) );
    // { added by kihyung 2012.07.26 for CR 11894
    connect( target, SIGNAL(scanModeChanged(AppMediaPlayer_Video_CModel_Base::SCAN_MODE_T)),
             this, SLOT(onAppMediaPlayer_Video_CModel_FSscanModeChanged(AppMediaPlayer_Video_CModel_Base::SCAN_MODE_T)));
    setScanMode( (m_bindedAppMediaPlayer_Video_CModel_FS->scanMode()) );
    // } added by kihyung
    connect( target, SIGNAL(loadingModeChanged(bool)),
             this, SLOT(onAppMediaPlayer_Video_CModel_FSloadingModeChanged(bool)));
    setLoadingMode( (m_bindedAppMediaPlayer_Video_CModel_FS->loadingMode()) ); // added by Sergey for CR16250

    // added by kihyung 2013.09.15 for ISV 91113
    connect( target, SIGNAL(removingModeChanged(bool)),
             this, SLOT(onAppMediaPlayer_Video_CModel_FSremovingModeChanged(bool)));
    setRemovingMode( (m_bindedAppMediaPlayer_Video_CModel_FS->removingMode()) ); 

    connect( target, SIGNAL(xsubCaptionVisibilityChanged(bool)),
             this, SLOT(onXSubCaptionVisibilityChanged(bool))); // modified by ravikanth 04-04-13
}

void AppMediaPlayer_Video_SModel_Playback::bindWith( AppMediaPlayer_Video_CModel_DVD *target )
{
    m_bindedAppMediaPlayer_Video_CModel_DVD = target;
    connect( target, SIGNAL(loadingModeChanged(bool)),
             this, SLOT(onAppMediaPlayer_Video_CModel_DVDloadingModeChanged(bool)));
    setLoadingMode( (m_bindedAppMediaPlayer_Video_CModel_DVD->loadingMode()) ); // added by Sergey for Loading popup
    // added by kihyung 2013.09.15 for ISV 91113
    connect( target, SIGNAL(removingModeChanged(bool)),
             this, SLOT(onAppMediaPlayer_Video_CModel_DVDremovingModeChanged(bool)));
    setRemovingMode( (m_bindedAppMediaPlayer_Video_CModel_DVD->removingMode()) ); 
    connect( target, SIGNAL(titleChanged(int)),
             this, SLOT(onAppMediaPlayer_Video_CModel_DVDtitleChanged(int)));
    // { modified by cychoi 2013.09.24 for ITS 191333
    if(m_bindedAppMediaPlayer_Video_CModel_DVD->title() == 0)
    {
        setFilename( QString(" ") );
    }
    else
    {
        setFilename( QString(QApplication::translate("main", "STR_MEDIA_TITLE") + " %1").arg(m_bindedAppMediaPlayer_Video_CModel_DVD->title()) );
    }
    // } modified by cychoi 2013.09.24
    connect( target, SIGNAL(chapterChanged(int)),
             this, SLOT(onAppMediaPlayer_Video_CModel_DVDchapterChanged(int)));
    // { modified by cychoi 2013.09.24 for ITS 191333
    if(m_bindedAppMediaPlayer_Video_CModel_DVD->chapter() == 0)
    {
        setFilename2( QString(" ") );
    }
    else
    {
        setFilename2( QString(QApplication::translate("main", "STR_MEDIA_SEARCH_CHAPTER") + " %1").arg(m_bindedAppMediaPlayer_Video_CModel_DVD->chapter()) );
    }
    // } modified by cychoi 2013.09.24
    connect( target, SIGNAL(positionChanged(int)),
             this, SLOT(onAppMediaPlayer_Video_CModel_DVDpositionChanged(int)));
	// modified by richard 2012.10.25 for navi slowness fix	
    //setPosition( (m_bindedAppMediaPlayer_Video_CModel_DVD->position()/1000) );
	setPosition( m_bindedAppMediaPlayer_Video_CModel_DVD->position() );
    connect( target, SIGNAL(playbackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T)),
             this, SLOT(onAppMediaPlayer_Video_CModel_DVDplaybackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T)));
    setPlaybackStatus( (m_bindedAppMediaPlayer_Video_CModel_DVD->playbackStatus()) );
}
void AppMediaPlayer_Video_SModel_Playback::bindWith( AppMediaPlayer_Video_CModel_VCD *target )
{
    m_bindedAppMediaPlayer_Video_CModel_VCD = target;
    connect( target, SIGNAL(loadingModeChanged(bool)),
             this, SLOT(onAppMediaPlayer_Video_CModel_VCDloadingModeChanged(bool)));
    setLoadingMode( (m_bindedAppMediaPlayer_Video_CModel_VCD->loadingMode()) ); // added by Sergey for Loading popup
    // added by kihyung 2013.09.15 for ISV 91113
    connect( target, SIGNAL(removingModeChanged(bool)),
             this, SLOT(onAppMediaPlayer_Video_CModel_VCDremovingModeChanged(bool)));
    setRemovingMode( (m_bindedAppMediaPlayer_Video_CModel_VCD->removingMode()) ); 
    connect( target, SIGNAL(repeatModeChanged(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T)),
             this, SLOT(onAppMediaPlayer_Video_CModel_VCDrepeatModeChanged(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T)));
    setRepeatMode( (m_bindedAppMediaPlayer_Video_CModel_VCD->repeatMode()) );
    connect( target, SIGNAL(randomModeChanged(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T)),
             this, SLOT(onAppMediaPlayer_Video_CModel_VCDrandomModeChanged(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T)));
    setRandomMode( (m_bindedAppMediaPlayer_Video_CModel_VCD->randomMode()) );
    // { added by kihyung 2012.07.26 for CR 11894
    connect( target, SIGNAL(scanModeChanged(AppMediaPlayer_Video_CModel_Base::SCAN_MODE_T)),
             this, SLOT(onAppMediaPlayer_Video_CModel_VCDscanModeChanged(AppMediaPlayer_Video_CModel_Base::SCAN_MODE_T)));
    setScanMode( (m_bindedAppMediaPlayer_Video_CModel_VCD->scanMode()) );
    // } added by kihyung
    connect( target, SIGNAL(playbackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T)),
             this, SLOT(onAppMediaPlayer_Video_CModel_VCDplaybackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T)));
    setPlaybackStatus( (m_bindedAppMediaPlayer_Video_CModel_VCD->playbackStatus()) );
    connect( target, SIGNAL(currentTrackChanged(int)),
             this, SLOT(onAppMediaPlayer_Video_CModel_VCDcurrentTrackChanged(int)));
    // { added by kihyung 2012.07.17 for CR 8042
    connect( target, SIGNAL(tuneWheelTrackChanged(int)),
             this, SLOT(onAppMediaPlayer_Video_CModel_VCDtuneWheelTrackChanged(int)));
    // } added by kihyung 2012.07.17    
    // { modified by cychoi 2013.09.24 for ITS 191333
    if(m_bindedAppMediaPlayer_Video_CModel_VCD->currentTrack() == 0)
    {
        setFilename( QString(" ") );
    }
    else
    {
        setFilename( QString(QApplication::translate("main", "STR_MEDIA_CHAPTER") + " %1").arg(m_bindedAppMediaPlayer_Video_CModel_VCD->currentTrack()) );
    }
    // } modified by cychoi 2013.09.24
    connect( target, SIGNAL(positionChanged(int)),
             this, SLOT(onAppMediaPlayer_Video_CModel_VCDpositionChanged(int)));
    // { added by wspark 2013.04.15 for ITS 162431
    connect( target, SIGNAL(isPbcModeChanged(bool)),
             this, SLOT(setPbcMode(bool)));
    // } added by wspark
 	// modified by richard 2012.10.25 for navi slowness fix	
    //setPosition( (m_bindedAppMediaPlayer_Video_CModel_VCD->position()/1000) );
	setPosition( m_bindedAppMediaPlayer_Video_CModel_VCD->position() );
}

void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_FSplaylistLengthChanged(int)
{
    setCountInfo( QString("%1/%2").arg(m_bindedAppMediaPlayer_Video_CModel_FS->displayPlaylistPos()).arg(m_bindedAppMediaPlayer_Video_CModel_FS->displayPlaylistLength()) ); // modified by kihyung 2013.09.11 for ITS 0189474
}
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_FSplaylistPositionChanged(int)
{
    setCountInfo( QString("%1/%2").arg(m_bindedAppMediaPlayer_Video_CModel_FS->displayPlaylistPos()).arg(m_bindedAppMediaPlayer_Video_CModel_FS->displayPlaylistLength()) ); // modified by kihyung 2013.09.11 for ITS 0189474
}
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_FSpositionChanged(int)
{
    // modified by richard 2012.10.25 for navi slowness fix	
    // setPosition( (m_bindedAppMediaPlayer_Video_CModel_FS->position()/1000) );
	setPosition( m_bindedAppMediaPlayer_Video_CModel_FS->position() );
}
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_FSdurationChanged(int)
{
    // modified by richard 2012.10.25 for navi slowness fix	
    // setDuration( (m_bindedAppMediaPlayer_Video_CModel_FS->duration()/1000) );
	setDuration( m_bindedAppMediaPlayer_Video_CModel_FS->duration() );
}
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_FSplaybackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T)
{
    setPlaybackStatus( (m_bindedAppMediaPlayer_Video_CModel_FS->playbackStatus()) );
}
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_FSrepeatModeChanged(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T)
{
    setRepeatMode( (m_bindedAppMediaPlayer_Video_CModel_FS->repeatMode()) );
}
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_FSrandomModeChanged(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T)
{
    setRandomMode( (m_bindedAppMediaPlayer_Video_CModel_FS->randomMode()) );
}
// { added by kihyung 2012.07.26 for CR 11894
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_FSscanModeChanged(AppMediaPlayer_Video_CModel_Base::SCAN_MODE_T)
{
    setScanMode( (m_bindedAppMediaPlayer_Video_CModel_FS->scanMode()) );
}
// } added by kihyung
// { added by Sergey for CR16250
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_FSloadingModeChanged(bool)
{
    setLoadingMode( m_bindedAppMediaPlayer_Video_CModel_FS->loadingMode() );
}
// }added by Sergey for CR16250
// { added by kihyung 2013.09.15 for ISV 91113
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_FSremovingModeChanged(bool)
{
    setRemovingMode(m_bindedAppMediaPlayer_Video_CModel_FS->removingMode());
}    
// } added by kihyung 2013.09.15 for ISV 91113
// { added by Sergey for Loading popup
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_DVDloadingModeChanged(bool)
{
    setLoadingMode( m_bindedAppMediaPlayer_Video_CModel_DVD->loadingMode() );
}
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_VCDloadingModeChanged(bool)
{
    setLoadingMode( m_bindedAppMediaPlayer_Video_CModel_VCD->loadingMode() );
}
// } added by Sergey for Loading popup
// { added by kihyung 2013.09.15 for ISV 91113                
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_DVDremovingModeChanged(bool)
{
    setLoadingMode( m_bindedAppMediaPlayer_Video_CModel_DVD->removingMode() );
}
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_VCDremovingModeChanged(bool)
{
setLoadingMode( m_bindedAppMediaPlayer_Video_CModel_VCD->removingMode() );
}
// } added by kihyung 2013.09.15 for ISV 91113                
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_DVDtitleChanged(int)
{
    // { modified by cychoi 2013.09.24 for ITS 191333
    if(m_bindedAppMediaPlayer_Video_CModel_DVD->title() == 0)
    {
        setFilename( QString(" ") );
    }
    else
    {
        setFilename( QString(QApplication::translate("main", "STR_MEDIA_TITLE") + " %1").arg(m_bindedAppMediaPlayer_Video_CModel_DVD->title()) );
    }
    // } modified by cychoi 2013.09.24
}
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_DVDchapterChanged(int)
{
    // { modified by cychoi 2013.09.24 for ITS 191333
    if(m_bindedAppMediaPlayer_Video_CModel_DVD->chapter() == 0)
    {
        setFilename2( QString(" ") );
    }
    else
    {
        setFilename2( QString(QApplication::translate("main", "STR_MEDIA_SEARCH_CHAPTER") + " %1").arg(m_bindedAppMediaPlayer_Video_CModel_DVD->chapter()) );
    }
    // } modified by cychoi 2013.09.24
}
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_DVDpositionChanged(int)
{
    // modified by richard 2012.10.25 for navi slowness fix
    //setPosition( (m_bindedAppMediaPlayer_Video_CModel_DVD->position()/1000) );
	setPosition( m_bindedAppMediaPlayer_Video_CModel_DVD->position() );
}
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_DVDplaybackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T)
{
    setPlaybackStatus( (m_bindedAppMediaPlayer_Video_CModel_DVD->playbackStatus()) );
}
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_VCDrepeatModeChanged(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T)
{
    setRepeatMode( (m_bindedAppMediaPlayer_Video_CModel_VCD->repeatMode()) );
}
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_VCDrandomModeChanged(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T)
{
    setRandomMode( (m_bindedAppMediaPlayer_Video_CModel_VCD->randomMode()) );
}
// { added by kihyung 2012.07.26 for CR 11894
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_VCDscanModeChanged(AppMediaPlayer_Video_CModel_Base::SCAN_MODE_T)
{
    setScanMode( (m_bindedAppMediaPlayer_Video_CModel_VCD->scanMode()) );
}
// } added by kihyung
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_VCDplaybackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T)
{
    setPlaybackStatus( (m_bindedAppMediaPlayer_Video_CModel_VCD->playbackStatus()) );
}
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_VCDcurrentTrackChanged(int)
{
    // { modified by cychoi 2013.09.24 for ITS 191333
    if(m_bindedAppMediaPlayer_Video_CModel_VCD->currentTrack() == 0)
    {
        setFilename( QString(" ") );
    }
    else
    {
        // modified by minho 20120627 for changed Track number instead Chapter
        setFilename( QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_bindedAppMediaPlayer_Video_CModel_VCD->currentTrack()) ); // KMH2012
    }
    // } modified by cychoi 2013.09.24
}
// { added by kihyung 2012.07.17 for CR 8042
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_VCDtuneWheelTrackChanged(int)
{
    setFilename( QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_bindedAppMediaPlayer_Video_CModel_VCD->tuneWheelTrack()) );
}
// } added by kihyung 2012.07.17
void AppMediaPlayer_Video_SModel_Playback::onAppMediaPlayer_Video_CModel_VCDpositionChanged(int)
{
    // modified by richard 2012.10.25 for navi slowness fix
    //setPosition( (m_bindedAppMediaPlayer_Video_CModel_VCD->position()/1000) );
	setPosition( m_bindedAppMediaPlayer_Video_CModel_VCD->position() );
}

void AppMediaPlayer_Video_SModel_Playback::setFilename(QString value)
{
    m_filename = value;
    emit filenameChanged( m_filename );
}
void AppMediaPlayer_Video_SModel_Playback::setFilename2(QString value)
{
    m_filename2 = value;
    emit filename2Changed( m_filename2 );
}
void AppMediaPlayer_Video_SModel_Playback::setSubtitleStr(QString value)
{
    m_subtitleStr = value;
    emit subtitleStrChanged( m_subtitleStr );
}
void AppMediaPlayer_Video_SModel_Playback::setPosition(int value)
{
    m_position = value;
    emit positionChanged( m_position );
}
void AppMediaPlayer_Video_SModel_Playback::setDuration(int value)
{
    m_duration = value;
    emit durationChanged( m_duration );
}
void AppMediaPlayer_Video_SModel_Playback::setCountInfo(QString value)
{
    m_countInfo = value;
    emit countInfoChanged( m_countInfo );
}
void AppMediaPlayer_Video_SModel_Playback::setRepeatMode(int value)
{
    m_repeatMode = value;
    emit repeatModeChanged( m_repeatMode );
}
void AppMediaPlayer_Video_SModel_Playback::setRandomMode(int value)
{
    m_randomMode = value;
    emit randomModeChanged( m_randomMode );
}
// { added by kihyung 2012.07.26 for CR 11894
void AppMediaPlayer_Video_SModel_Playback::setScanMode(int value)
{
    m_scanMode = value;
    emit scanModeChanged( m_scanMode );
}
// } added by kihyung
void AppMediaPlayer_Video_SModel_Playback::setProgressBarMode(QString value)
{
    m_progressBarMode = value;
    emit progressBarModeChanged( m_progressBarMode );
}
void AppMediaPlayer_Video_SModel_Playback::setPlaybackStatus(int value)
{
    m_playbackStatus = value;
    emit playbackStatusChanged( m_playbackStatus );

    if(m_playbackStatus == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_STOPPED)
        setPosition(0); // modified by Sergey for 78517
}
void AppMediaPlayer_Video_SModel_Playback::setLockoutMode(bool value)
{
    m_lockoutMode = value;
    emit lockoutModeChanged( m_lockoutMode );
}

void AppMediaPlayer_Video_SModel_Playback::setLoadingMode(bool value)
{
    m_loadingMode = value;
    emit loadingModeChanged( m_loadingMode );
} // modified by Sergey 18.05.2013

// modified by Sergey 18.05.2013
void AppMediaPlayer_Video_SModel_Playback::setRemovingMode(bool value)
{
    m_removingMode = value;
    emit removingModeChanged( m_removingMode );
} 

void AppMediaPlayer_Video_SModel_Playback::setCaptionSize(int value)
{
    m_captionSize = value;
    emit captionSizeChanged( m_captionSize );
}

// added by kihyung 2013.06.14
void AppMediaPlayer_Video_SModel_Playback::setCaptionEnable(bool enable)
{
    MP_LOG << enable << LOG_ENDL;
    m_captionEnable = enable;
    emit captionEnableChanged(m_captionEnable);
}

void AppMediaPlayer_Video_SModel_Playback::setTuneMode(bool value)
{
    // rollback by cychoi 2013.11.10 for ITS 207831 Control Cue Icon on Tune Mode
    m_tuneMode = value;
    emit tuneModeChanged( m_tuneMode );
}

// { added by yongkyun.lee  2012.10.17  for Current file is white color
void AppMediaPlayer_Video_SModel_Playback::setTuneSameFile(bool value)
{
    m_tuneSameFile = value;
    emit tuneSameFileChanged( m_tuneSameFile );
}
// } added by  yongkyun.lee 

// removed by Sergey 01.05.2013 for Tune Functionality

void AppMediaPlayer_Video_SModel_Playback::setFocusVisible(bool value)
{
    m_focusVisible = value;
    emit focusVisibleChanged( m_focusVisible );
}
void AppMediaPlayer_Video_SModel_Playback::setFocusActivity(bool value)
{
    m_focusActivity = value;
    emit focusActivityChanged( m_focusActivity );
}

// { added by wspark 2013.02.21 for ISV 73305
void AppMediaPlayer_Video_SModel_Playback::setCamMode(bool value)
{
    m_camMode = value;
    emit camModeChanged( m_camMode );
}
// } added by wspark

// { modified by ravikanth 04-04-13
void AppMediaPlayer_Video_SModel_Playback::onXSubCaptionVisibilityChanged(bool value)
{
    m_divxCaptionVisible = value;
    emit divxCaptionVisibleChanged( m_divxCaptionVisible );
}
// } modified by ravikanth 04-04-13

// { added by wspark 2013.04.15 for ITS 162431
void AppMediaPlayer_Video_SModel_Playback::setPbcMode(bool value)
{
    m_pbcMode = value;
    emit pbcModeChanged( m_pbcMode );
}
// } added by wspark

// { added by Sergey 07.09.2013 for PRE_FG supoort
void AppMediaPlayer_Video_SModel_Playback::setFgLoadingMode(bool value)
{
    if(value == true)
        setRemovingMode(false); // added by Sergey 24.10.2013 for ITS#87134

    m_fgLoadingMode = value;
    emit fgLoadingModeChanged( m_fgLoadingMode );
}
// } added by Sergey 07.09.2013 for PRE_FG supoort

// { added by cychoi 2013.09.03 for ITS 187585
void AppMediaPlayer_Video_SModel_Playback::retranslateUI(VIDEO_SOURCE_T source)
{
    if(source == VIDEO_SOURCE_DVD)
    {
        setFilename( QString(QApplication::translate("main", "STR_MEDIA_TITLE") + " %1").arg(m_bindedAppMediaPlayer_Video_CModel_DVD->title()) );
        setFilename2( QString(QApplication::translate("main", "STR_MEDIA_SEARCH_CHAPTER") + " %1").arg(m_bindedAppMediaPlayer_Video_CModel_DVD->chapter()) );
    }
    else if(source == VIDEO_SOURCE_VCD)
    {
        setFilename( QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(m_bindedAppMediaPlayer_Video_CModel_VCD->currentTrack()) );
    }
    // { added by Sergey 26.11.2013 for Root folder translation
    else if(source == VIDEO_SOURCE_FS && m_bindedAppMediaPlayer_Video_CModel_FS->isRootFolder())
    {
        setFilename2(QApplication::translate("main", QT_TR_NOOP("STR_MEDIA_ROOT_FOLDER")));
    }
    // } added by Sergey 26.11.2013 for Root folder translation
}
// } added by cychoi 2013.09.03

