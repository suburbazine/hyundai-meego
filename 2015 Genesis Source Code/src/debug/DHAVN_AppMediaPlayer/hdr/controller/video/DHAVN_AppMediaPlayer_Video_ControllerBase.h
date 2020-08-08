#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_CONTROLLERBASE_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_CONTROLLERBASE_H

#include <QObject>

class IMediaInterface;
class AppMediaPlayer_Video_CModel_Base;

/** @class AppMediaPlayer_Video_ControllerBase
 * Common interface for each one of the source-specific controllers.
 * */
class AppMediaPlayer_Video_ControllerBase
{
public:
    /** @brief Stops doing anything with the controllers.
     * */
    virtual void dropControllers() = 0;

    /** @brief Method notifying that player can do whatever it wants.
     * */
    virtual void attachControllers() = 0;

    /** @brief Method to provide access to controller-specific model.
     */
    virtual AppMediaPlayer_Video_CModel_Base *getModel() = 0;

    /** @brief Every controller should be able to set its audio path
      */
    virtual void setAudioPath() = 0;

    IMediaInterface *getIMedia() { return m_pIMedia; }

    /* Playback functionality */
    virtual void play();
    virtual void togglePlay();
    virtual void toggleRepeat();
    virtual void toggleRandom();
    virtual void stop();
    virtual void functionStop(); // added by kihyung 2012.07.31 for C 11949
    virtual void pause();
    virtual void next();
    virtual void prev(bool bForce = 0); //modified by edo.lee 2013.06.11
    virtual bool tuneWheel(bool bTuneForward) { Q_UNUSED(bTuneForward); return false; }; // modified by kihyung 2012.07.17. add return value.
    virtual bool selectTuned() { return false; }; // modified by junggil 2012.07.05 for CR11261 : Not able to search video files using CCP/RRC jogdial
    virtual void cancelTune() {};
    virtual void fastForward() {};
    virtual void fastForwardCritical() {};
    virtual void rewind() {};
    virtual void rewindCritical() {};
    virtual void cancel_ff_rew();
    virtual void setPosition(int position);
    virtual void randomPlayOff();
    virtual void randomPlayFile();
    virtual void randomPlayFolder();
    virtual void repeatPlayOff();
    virtual void repeatPlayFile();
    virtual void repeatPlayFolder();
    virtual void repeatPlayAll(); // added by ravikanth - 12-09-19
    virtual void flickPrev() {};
    virtual void flickNext() {};
    virtual bool subTitle() { return true; }; // added by junggil 2012.08.22 for NEW UX 5.1.8.1 Caption
    virtual bool showSubTitle(bool bShow) { return false; }; // modified by kihyung 2013.07.10 for ITS 0173538 
    virtual void ShowCurrentModelToOSD(){};
    /* End of playback functionality */

protected:
    IMediaInterface *m_pIMedia;    
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_CONTROLLERBASE_H
