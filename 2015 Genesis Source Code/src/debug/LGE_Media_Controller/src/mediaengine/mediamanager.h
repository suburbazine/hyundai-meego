#ifndef _MEDIA_MANAGER_H
#define _MEDIA_MANAGER_H

#include "mediaplayer_def.h"

class MediaManager
{
public: 
    static MediaManager*    GetInstance();
    ~MediaManager();

    int                     SetMediaMode(MEDIA_PLAYER_MODE eFromMedia, MEDIA_PLAYER_MODE eToMode);
    MEDIA_PLAYER_MODE       GetMediaMode();

    MEDIA_PLAYER_MODE       GetPrevMediaMode();

    QString                 GetFrontUSBPath();
    QString                 GetRearUSBPath();
    
private:
    MediaManager();
    static MediaManager     *m_pInstance;

    MEDIA_PLAYER_MODE        m_eCurrMode;
    MEDIA_PLAYER_MODE        m_ePrevMode;

    QString                  m_strUSB1Path;
    QString                  m_strUSB2Path;
};

#endif
