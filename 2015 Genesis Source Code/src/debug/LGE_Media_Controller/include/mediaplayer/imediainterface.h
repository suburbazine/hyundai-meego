#ifndef IMEDIAINTERFACE_H
#define IMEDIAINTERFACE_H

#include "mediaplayer_global.h"
#include "mediaplayer_def.h"

class MEDIAPLAYERSHARED_EXPORT IMediaInterface 
{    
public:
    virtual int         ChangeMediaMode(MEDIA_PLAYER_MODE eMode)= 0;
    virtual QString     GetCurrentFileName()                    = 0;
    virtual int         GetCurrentIndex()                       = 0;
    virtual void        SetCurrentIndex(int nIndex)             = 0;
    virtual int         GetCurrentMediaState()                  = 0;
    virtual int         GetCurrentMediaDuration()               = 0;
    virtual int         GetCurrentMediaPosition()               = 0;
    virtual void        SetCurrentMediaPosition(int position)   = 0;
    virtual int         GetDeviceState()                        = 0;    // return DEVICE_STATE

    virtual void        GetCurrentMediaInfo(MediaInfo *pMediaInfo) = 0;
    virtual void        GetDeviceInfo(DeviceInfo *pDeviceInfo)     = 0;
    virtual void        GetPlaylist(PlayList *pPlaylist)           = 0;

    virtual void        SetPlayList(PlayList *pPlaylist)        = 0;
    virtual bool        SeekableMedia()                         = 0;
    virtual void        SetBrightness(double value)             = 0;
    virtual void        SetContrast(double value)               = 0;
    virtual void        SetTint(double value)                   = 0;
    virtual void        SetBrightness(MEDIA_DISPLAY displayPlane, double value) = 0;
    virtual void        SetContrast(MEDIA_DISPLAY displayPlane, double value)   = 0;
    virtual void        SetTint(MEDIA_DISPLAY displayPlane, double value)       = 0;
    virtual double      GetBrightness()                         = 0;
    virtual double      GetContrast()                           = 0;
    virtual double      GetTint()                               = 0;
    virtual void        SetSystemState(int)                     = 0;
    virtual QWidget*    GetVideoWidget()                        = 0;
    virtual void        EnableSubTitle(bool bEnable)            = 0;
    virtual void        SetPlayerEngine(PLAYER_ENGINE_TYPE eType) = 0;
    virtual void        SetFontSize(const int fontSize)         = 0;
    virtual bool        ClearDisplay(bool isBackground)         = 0;

    virtual void        SetHue(double value)                             = 0;
    virtual void        SetHue(MEDIA_DISPLAY displayPlane, double value) = 0;
    virtual double      GetHue()                                         = 0;
    
    virtual void        GetCurrentFileFolderIndex(int *pFileIdx, int *pTotalFiles) = 0;

    virtual void        AudioDragStart()                        = 0;
    virtual void        AudioDragEnd()                          = 0;
    
public slots:
    virtual void        Play()                                  = 0;
    virtual void        Pause()                                 = 0;
    virtual void        FunctionStop()                          = 0;
    virtual void        Stop()                                  = 0;
    virtual void        Next(int nSkipLevel)                    = 0;
    virtual void        Next(int nSkipLevel, int nAutoSkip) {Q_UNUSED(nSkipLevel); Q_UNUSED(nAutoSkip);} // added by kihyung 2013.11.13 for ITS 0208734 
    virtual void        Prev(int nSkipLevel, int nForced = 0)   = 0;
    virtual void        Prev(int nSkipLevel, int nAutoSkip, int nForced) {Q_UNUSED(nSkipLevel); Q_UNUSED(nAutoSkip); Q_UNUSED(nForced);} // added by kihyung 2013.11.13 for ITS 0208734 
    virtual void        Scan(int mode)                          = 0;
    virtual void        RandomPlay(int mode, int bPlay = true)  = 0;
    virtual void        RepeatPlay(int mode, int bPlay = true)  = 0;
    virtual void        FastForward(int rate)                   = 0;
    virtual void        Rewind(int rate)                        = 0;
    virtual void        NextFolder(PlayList *pPlayList)         = 0;
    virtual void        PrevFolder(PlayList *pPlayList)         = 0;
    
};

#endif // IMEDIAINTERFACE_H
