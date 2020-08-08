#include "mediamanager.h"
#include "jukeboxcontroller.h"
#include "usbcontroller.h"
#include <QFileInfo>
#include "util.h"

MediaManager* MediaManager::m_pInstance = 0;

MediaManager* MediaManager::GetInstance()
{
    if(m_pInstance == 0) {
        m_pInstance = new MediaManager();
    }

    return m_pInstance;
}

MediaManager::~MediaManager()
{
    MC_LOG << "~MediaManager()" << LOG_ENDL;
}

int MediaManager::SetMediaMode(MEDIA_PLAYER_MODE eFromMedia, MEDIA_PLAYER_MODE eToMode)
{
    Q_UNUSED(eFromMedia);

    #if USE_USB_SINGLE_INSTANCE
    if(eToMode == MP_MODE_USB) {
        JukeboxController::GetInstance()->DisconnectSignal();
        USBController::GetInstance()->ConnectSignal();
    }
    else if(eToMode == MP_MODE_JUKEBOX) {
        USBController::GetInstance()->DisconnectSignal();
        JukeboxController::GetInstance()->ConnectSignal();
    }
    #endif
    
    m_ePrevMode = m_eCurrMode;
    m_eCurrMode = eToMode;

    return m_ePrevMode;
}

MEDIA_PLAYER_MODE MediaManager::GetMediaMode()
{
    return m_eCurrMode;
}

MEDIA_PLAYER_MODE MediaManager::GetPrevMediaMode()
{
    return m_ePrevMode;
}

QString MediaManager::GetFrontUSBPath()
{
    if(m_strUSB1Path.isEmpty()) {
        QFileInfo tFileInfo("/rw_data");
        if(tFileInfo.exists()) {
            m_strUSB1Path = "/rw_data/media/front_usb";
        }
        else {
            m_strUSB1Path = "/media/front_usb";
        }
    }
    return m_strUSB1Path;
}

QString MediaManager::GetRearUSBPath()
{
    if(m_strUSB2Path.isEmpty()) {
        QFileInfo tFileInfo("/rw_data"); // modified by kihyung 2013.11.25
        if(tFileInfo.exists()) {
            m_strUSB2Path = "/rw_data/media/rear_usb";
        }
        else {
            m_strUSB2Path = "/media/rear_usb";
        }
    }
    return m_strUSB2Path;
}

MediaManager::MediaManager()
{
    m_eCurrMode = MP_MODE_NONE;
    m_ePrevMode = MP_MODE_NONE;

    m_strUSB1Path.clear();
    m_strUSB2Path.clear();
}
