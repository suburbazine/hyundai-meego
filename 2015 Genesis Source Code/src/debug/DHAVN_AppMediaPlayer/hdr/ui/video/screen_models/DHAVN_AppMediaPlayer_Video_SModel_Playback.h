
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_Playback.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_PLAYBACK_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_PLAYBACK_H

#include <QObject>


#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_DVD.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_VCD.h"

class AppMediaPlayer_Video_CModel_FS;
class AppMediaPlayer_Video_CModel_DVD;
class AppMediaPlayer_Video_CModel_VCD;
// { added by cychoi 2013.09.03 for ITS 187585
enum VIDEO_SOURCE_T
{
    VIDEO_SOURCE_FS, // modified by Sergey 26.11.2013 for Root folder translation
    VIDEO_SOURCE_DVD,
    VIDEO_SOURCE_VCD,
    VIDEO_SOURCE_AUX1, // Not used
    VIDEO_SOURCE_AUX2, // Not used

    VIDEO_SOURCE_MAX
};
// } added by cychoi 2013.09.03
class AppMediaPlayer_Video_SModel_Playback : public QObject
{
    Q_OBJECT

    /** @property filename
     * Reflects the playing file's name
     */
    Q_PROPERTY( QString filename
                READ filename
                NOTIFY filenameChanged)
    /** @property filename2
     * Reflects the playing file's name
     */
    Q_PROPERTY( QString filename2
                READ filename2
                NOTIFY filename2Changed)
    /** @property subtitleStr
     * subtile string for displaying
     */
    Q_PROPERTY( QString subtitleStr
                READ subtitleStr
                NOTIFY subtitleStrChanged)
    /** @property position
     * Reflects the position of the currently played media in seconds.
     */
    Q_PROPERTY( int position
                READ position
                NOTIFY positionChanged)
    /** @property duration
     * Reflects the duration of the currently played media in seconds.
     */
    Q_PROPERTY( int duration
                READ duration
                NOTIFY durationChanged)
    /** @property countInfo
     * Provides a string in a XX/XX form of the currently played video.
     */
    Q_PROPERTY( QString countInfo
                READ countInfo
                NOTIFY countInfoChanged)
    /** @property repeatMode
     * 0 -- repeat off, 1 -- repeat file, 2 -- repeat folder
     */
    Q_PROPERTY( int repeatMode
                READ repeatMode
                NOTIFY repeatModeChanged)
    /** @property randomMode
     * 0 -- random off, 1 -- random file, 2 -- random folder
     */
    Q_PROPERTY( int randomMode
                READ randomMode
                NOTIFY randomModeChanged)

    // { added by kihyung 2012.07.26 for CR 11894
    /** @property scanMode
     * 0 -- scan off, 1 -- scan file, 2 -- scan folder
     */
    Q_PROPERTY( int scanMode
                READ scanMode
                NOTIFY scanModeChanged)
    // } added by kihyung                
    
    /** @property progressBarMode
     * Progress bar component style "jukebox" and "deck" are supported.
     */
    Q_PROPERTY( QString progressBarMode
                READ progressBarMode
                NOTIFY progressBarModeChanged)
    /** @property playbackStatus
     * Reflects the playback state (playing, paused, stopped).
     */
    Q_PROPERTY( int playbackStatus
                READ playbackStatus
                NOTIFY playbackStatusChanged)
    /** @property lockoutMode
     * When turned on dims video playback.
     */
    Q_PROPERTY( bool lockoutMode
                READ lockoutMode
                NOTIFY lockoutModeChanged)
    /** @property loadingMode
     * When turned on show loading popup.
     */
    Q_PROPERTY( bool loadingMode
                READ loadingMode
                NOTIFY loadingModeChanged) // added by Sergey for CR16250
    /** @property removingMode
     * When removed device show empty bg.
     */
    Q_PROPERTY( bool removingMode
                READ removingMode
                NOTIFY removingModeChanged) // added by kihyung 2013.09.15 for ISV 91113            
    /** @property captionSize
     * Size of the caption
     */
    Q_PROPERTY( int captionSize
                READ captionSize
                NOTIFY captionSizeChanged)

    /** @property captionEnable
     * Enabled or disabled caption
     */
    Q_PROPERTY( bool captionEnable
                READ captionEnable
                NOTIFY captionEnableChanged) // added by kihyung 2013.06.14
                
    // { added by yongkyun.lee  2012.10.17  for Current file is white color
    Q_PROPERTY( bool tuneSameFile
                READ tuneSameFile
                NOTIFY tuneSameFileChanged)
    // } added by yongkyun.lee 
    /** @property tuneMode
     * Is player in tune mode
     */
    Q_PROPERTY( bool tuneMode
                READ tuneMode
                NOTIFY tuneModeChanged)
    // { added by kihyung 2012.08.03 for CR 12313
	// removed by Sergey 01.05.2013 for Tune Functionality
    /** @property focusVisible
     * Is focus visible
     */
    Q_PROPERTY( bool focusVisible
                READ focusVisible
                NOTIFY focusVisibleChanged)
    /** @property focusActivity
     * Is focus activ
     */
    Q_PROPERTY( bool focusActivity
                READ focusActivity
                NOTIFY focusActivityChanged)

    // { added by wspark 2013.02.21 for ISV 73305
    Q_PROPERTY( bool camMode
                READ camMode
                NOTIFY camModeChanged)
    // } added by wspark

    Q_PROPERTY( bool divxCaptionVisible
                READ divxCaptionVisible
                NOTIFY divxCaptionVisibleChanged) // modified by ravikanth 04-04-13

    // { added by wspark 2013.04.15 for ITS 162431
    Q_PROPERTY( bool pbcMode
                READ pbcMode
                NOTIFY pbcModeChanged)
    // } added by wspark

    
    Q_PROPERTY( bool fgLoadingMode
                READ fgLoadingMode
                WRITE setFgLoadingMode
                NOTIFY fgLoadingModeChanged)  // added by Sergey 07.09.2013 for PRE_FG supoort

public:
    AppMediaPlayer_Video_SModel_Playback( QObject *parent = NULL );

    void bindWith( AppMediaPlayer_Video_CModel_FS * );
    void bindWith( AppMediaPlayer_Video_CModel_DVD * );
    void bindWith( AppMediaPlayer_Video_CModel_VCD * );

    QString filename() { return m_filename; }
    QString filename2() { return m_filename2; }
    QString subtitleStr() { return m_subtitleStr; }
    int position() { return m_position; }
    int duration() { return m_duration; }
    QString countInfo() { return m_countInfo; }
    int repeatMode() { return m_repeatMode; }
    int randomMode() { return m_randomMode; }
    int scanMode() { return m_scanMode; } // added by kihyung 2012.07.26 for CR 11894
    QString progressBarMode() { return m_progressBarMode; }
    int playbackStatus() { return m_playbackStatus; }
    bool lockoutMode() { return m_lockoutMode; }
    bool loadingMode() { return m_loadingMode; } // added by Sergey for CR16250
    bool removingMode() { return m_removingMode; } // added by kihyung 2013.09.15 for ISV 91113            
    int captionSize() { return m_captionSize; }
    bool captionEnable() { return m_captionEnable; } // added by kihyung 2013.06.14
    bool tuneMode() { return m_tuneMode; }
    bool tuneSameFile() { return m_tuneSameFile; } // added by yongkyun.lee  2012.10.17  for Current file is white color
	// removed by Sergey 01.05.2013 for Tune Functionality
    bool focusVisible() { return m_focusVisible; }
    bool focusActivity() { return m_focusActivity; }
    bool camMode() { return m_camMode; } // added by wspark 2013.02.21 for ISV 73305
    bool divxCaptionVisible() { return m_divxCaptionVisible; } // modified by ravikanth 04-04-13
    bool pbcMode() { return m_pbcMode; } // added by wspark 2013.04.15 for ITS 162431
    bool fgLoadingMode() { return m_fgLoadingMode; } // added by Sergey 07.09.2013 for PRE_FG supoort

    void retranslateUI(VIDEO_SOURCE_T); // added by cychoi 2013.09.03 for ITS 187585

public slots:
    void setTuneSameFile(bool);   // added by yongkyun.lee  2012.10.17  for Current file is white color
    void setFilename(QString);
    void setFilename2(QString);
    void setSubtitleStr(QString);
    void setPosition(int);
    void setDuration(int);
    void setCountInfo(QString);
    void setRepeatMode(int);
    void setRandomMode(int);
    void setScanMode(int); // added by kihyung 2012.07.26 for CR 11894
    void setProgressBarMode(QString);
    void setPlaybackStatus(int);
    void setLockoutMode(bool);
    void setLoadingMode(bool); // added by Sergey for CR16250
    void setRemovingMode(bool); // added by kihyung 2013.09.15 for ISV 91113            
    void setCaptionSize(int);
    void setCaptionEnable(bool); // added by kihyung 2013.06.14
    void setTuneMode(bool);
	// removed by Sergey 01.05.2013 for Tune Functionality
    void setFocusVisible(bool);
    void setFocusActivity(bool);
    void setCamMode(bool); // added by wspark 2013.02.21 for ISV 73305
    void setPbcMode(bool); // added by wspark 2013.04.15 for ITS 162431
    void setFgLoadingMode(bool); // added by Sergey 07.09.2013 for PRE_FG supoort

signals:
    void tuneSameFileChanged( bool  );   // added by yongkyun.lee  2012.10.17 for Current file is white color
    void filenameChanged( QString );
    void filename2Changed( QString );
    void subtitleStrChanged( QString );
    void captionEnableChanged( bool ); // added by kihyung 2013.06.14
    void positionChanged( int );
    void durationChanged( int );
    void countInfoChanged( QString );
    void repeatModeChanged( int );
    void randomModeChanged( int );
    void scanModeChanged( int ); // added by kihyung 2012.07.26 for CR 11894
    void progressBarModeChanged( QString );
    void playbackStatusChanged( int );
    void lockoutModeChanged( bool );
    void loadingModeChanged( bool ); // added by Sergey for CR16250
    void removingModeChanged( bool ); // added by kihyung 2013.09.15 for ISV 91113 
    void captionSizeChanged( int );
    void tuneModeChanged( bool );
	// removed by Sergey 01.05.2013 for Tune Functionality
    void focusVisibleChanged( bool );
    void focusActivityChanged( bool );
    void camModeChanged(bool); // added by wspark 2013.02.21 for ISV 73305
    void divxCaptionVisibleChanged(bool); // modified by ravikanth 04-04-13
    void pbcModeChanged(bool); // added by wspark 2013.04.15 for ITS 162431
    void fgLoadingModeChanged(bool); // added by Sergey 07.09.2013 for PRE_FG supoort
    void tuneModeChanged_2nd( bool ); // added by sjhyun 2013.11.09 for ITS 207831


private slots:
    void onAppMediaPlayer_Video_CModel_FSplaylistLengthChanged(int);
    void onAppMediaPlayer_Video_CModel_FSplaylistPositionChanged(int);
    void onAppMediaPlayer_Video_CModel_FSpositionChanged(int);
    void onAppMediaPlayer_Video_CModel_FSdurationChanged(int);
    void onAppMediaPlayer_Video_CModel_FSplaybackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T);
    void onAppMediaPlayer_Video_CModel_FSrepeatModeChanged(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T);
    void onAppMediaPlayer_Video_CModel_FSrandomModeChanged(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T);
    void onAppMediaPlayer_Video_CModel_FSscanModeChanged(AppMediaPlayer_Video_CModel_Base::SCAN_MODE_T); // added by kihyung 2012.07.26 for CR 11894
    void onAppMediaPlayer_Video_CModel_FSloadingModeChanged(bool); // added by Sergey for CR16250
    void onAppMediaPlayer_Video_CModel_FSremovingModeChanged(bool); // added by kihyung 2013.09.15 for ISV 91113            
    void onAppMediaPlayer_Video_CModel_DVDloadingModeChanged(bool); // added by Sergey for Loading popup
    void onAppMediaPlayer_Video_CModel_DVDremovingModeChanged(bool); // added by kihyung 2013.09.15 for ISV 91113                
    void onAppMediaPlayer_Video_CModel_DVDtitleChanged(int);
    void onAppMediaPlayer_Video_CModel_DVDchapterChanged(int);
    void onAppMediaPlayer_Video_CModel_DVDpositionChanged(int);
    void onAppMediaPlayer_Video_CModel_DVDplaybackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T);
    void onAppMediaPlayer_Video_CModel_VCDloadingModeChanged(bool); // added by Sergey for Loading popup
    void onAppMediaPlayer_Video_CModel_VCDremovingModeChanged(bool); // added by kihyung 2013.09.15 for ISV 91113                
    void onAppMediaPlayer_Video_CModel_VCDrepeatModeChanged(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T);
    void onAppMediaPlayer_Video_CModel_VCDrandomModeChanged(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T);
    void onAppMediaPlayer_Video_CModel_VCDscanModeChanged(AppMediaPlayer_Video_CModel_Base::SCAN_MODE_T); // added by kihyung 2012.07.26 for CR 11894
    void onAppMediaPlayer_Video_CModel_VCDplaybackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T);
    void onAppMediaPlayer_Video_CModel_VCDcurrentTrackChanged(int);
    void onAppMediaPlayer_Video_CModel_VCDtuneWheelTrackChanged(int); // added by kihyung 2012.07.17 for CR 8042
    void onAppMediaPlayer_Video_CModel_VCDpositionChanged(int);
    void onXSubCaptionVisibilityChanged(bool); // modified by ravikanth 04-04-13



private:
    QString m_filename;    
    QString m_filename2;    
    QString m_subtitleStr;    
    int m_position;    
    int m_duration;    
    QString m_countInfo;    
    int m_repeatMode;    
    int m_randomMode;    
    int m_scanMode; // added by kihyung 2012.07.26 for CR 11894
    QString m_progressBarMode;    
    int m_playbackStatus;    
    bool m_lockoutMode;    
    bool m_loadingMode; // added by Sergey for CR16250
    bool m_removingMode; // added by kihyung 2013.09.15 for ISV 91113            
    int m_captionSize;    
    bool m_captionEnable; // added by kihyung 2013.06.14
    bool m_tuneMode;    
    bool m_tuneSameFile; // added by yongkyun.lee  2012.10.17  for Current file is white color   
	// removed by Sergey 01.05.2013 for Tune Functionality
    bool m_focusVisible;    
    bool m_focusActivity;    
    bool m_camMode; // added by wspark 2013.02.21 for ISV 73305
    bool m_divxCaptionVisible; // modified by ravikanth 04-04-13
    bool m_pbcMode; // added by wspark 2013.04.15 for ITS 162431
    bool m_fgLoadingMode; // added by Sergey 07.09.2013 for PRE_FG supoort

    AppMediaPlayer_Video_CModel_FS *m_bindedAppMediaPlayer_Video_CModel_FS;
    AppMediaPlayer_Video_CModel_DVD *m_bindedAppMediaPlayer_Video_CModel_DVD;
    AppMediaPlayer_Video_CModel_VCD *m_bindedAppMediaPlayer_Video_CModel_VCD;
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_PLAYBACK_H

