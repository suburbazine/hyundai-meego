#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_CMODEL_VCD_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_CMODEL_VCD_H

#include "DHAVN_AppMediaPlayer_Video_CModel_Base.h"

enum VCD_VERSION
{
    VCD_VERSION_1_1         = 0x00,
    VCD_VERSION_2_0         = 0x01,
    VCD_VERSION_SVCD        = 0x02,
    VCD_VERSION_UNDEFINED   = 0x03
};
// VCD 1.1 = 0x00, VCD 2.0 = 0x01, SVCD or Other = 0x02, UNDEFINED = 0x03

class AppMediaPlayer_Video_CModel_VCD : public AppMediaPlayer_Video_CModel_Base
{
    Q_OBJECT
public:
    AppMediaPlayer_Video_CModel_VCD( QObject *parent );
    virtual ~AppMediaPlayer_Video_CModel_VCD();

    /** @property trackCount
     * The number of tracks on the VCD disc. Property should be considered to be
     * read-only.
     */
    int trackCount() { return m_trackCount; }
    void setTrackCount( int value );

    /** @property currentTrack
     * The currently playing track number. Property should be considered to be
     * read-only.
     */
    int currentTrack() { return m_currentTrack; }
    void setCurrentTrack( int value );

    /** @property isPbcMenuDisplayed
     * Is Pbc Menu displayed. Property should be considered to be
     * read-only.
     */
    bool isPbcMenuDisplayed() { return m_isPbcMenuDisplayed; }
    bool isPbcMenuShown() { return m_isPbcMenuShown; } // added by cychoi 2013.12.19 for ITS 215825 Default focus
    void setIsPbcMenuDisplayed( bool value );

    // { added by Sergey for CR#14692
    /** @property isPbcMode
     * Is Pbc mode is on. Property should be considered to be
     * read-only.
     */
    bool isPbcMode() { return m_isPbcMode; }
    void setIsPbcMode(bool value);

    /** @property vcdVersion
     * Stores version of inserted vcd disc.
     * Property should be considered to be read-only.
     */
     // } added by Sergey for CR#14692.
    int vcdVersion() { return m_nVcdVersion; }
    void setVcdVersion( int value );
	
	    /** @property resumePosition
     * resumePosition. Property should be considered to be
     * read-only.
     */
    int resumePosition() {return m_PlayResumePosition;}
    void setResumePosition(int value);

    // { added by kihyung 2012.07.17 for CR 8042
    int tuneWheelTrack() { return m_tuneWheelTrack; }
    void setTuneWheelTrack( int value );
    // } added by kihyung 2012.07.17
    
    // { added by Sergey Vetugov for CR#13620
    /** @property isTrackLoading
     * isTrackLoading. True from the time when play() was called
     * till got signal that it is really playing.
     */
        bool isTrackLoading() { return m_isTrackLoading; }
        void setIsTrackLoading(bool value);
    // } added by Sergey Vetugov for CR#13620.
    
signals:
    void trackCountChanged( int value );
    void currentTrackChanged( int value );
    void isPbcMenuDisplayedChanged( bool value );
    void tuneWheelTrackChanged( int value ); // added by kihyung 2012.07.17 for CR 8042
    void isTrackLoadingChanged( bool value ); // added by Sergey Vetugov for CR#13620
    void isPbcModeChanged( bool value ); // { added by Sergey for CR#14692

private:
    int m_trackCount;
    int m_currentTrack;
    bool m_isPbcMenuDisplayed;
    bool m_isPbcMenuShown; // added by cychoi 2013.12.19 for ITS 215825 Default focus
    bool m_isPbcMode; // { added by Sergey for CR#14692
    int m_nVcdVersion;
    int m_PlayResumePosition;
    int m_tuneWheelTrack; // added by kihyung 2012.07.17 for CR 8042
    bool m_isTrackLoading; // added by Sergey Vetugov for CR#13620
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_CMODEL_VCD_H
