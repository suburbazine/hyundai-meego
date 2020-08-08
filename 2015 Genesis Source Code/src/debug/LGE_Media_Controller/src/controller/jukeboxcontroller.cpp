#include "jukeboxcontroller.h"
#include "performance.h"
#include "util.h"
#include <QVariant>

#if USE_USB_SINGLE_INSTANCE
JukeboxController* JukeboxController::m_pInstance = 0;
#else
JukeboxController* JukeboxController::m_pFirstInstance = 0;
JukeboxController* JukeboxController::m_pSecondInstance = 0;
JukeboxController* JukeboxController::m_pThirdInstance = 0;
#endif

#if USE_USB_SINGLE_INSTANCE
JukeboxController* JukeboxController::GetInstance()
{
    if(m_pInstance == 0) {
        PT_START_TIME(PT_OBJ_NAME_1, 0);
        m_pInstance = new JukeboxController(MP_MODE_JUKEBOX);
        m_pInstance->SetMyMediaMode(MP_MODE_JUKEBOX);
        PT_END_TIME(PT_OBJ_NAME_1, "JukeboxController::GetInstance()");
    }

    return m_pInstance;
}
#else
JukeboxController* JukeboxController::GetFirstInstance()
{
    if(m_pFirstInstance == 0) {
        PT_START_TIME(PT_OBJ_NAME_1, 0);
        m_pFirstInstance = new JukeboxController(MP_MODE_JUKEBOX);
        m_pFirstInstance->SetMyMediaMode(MP_MODE_JUKEBOX);
        PT_END_TIME(PT_OBJ_NAME_1, "JukeboxController::GetFirstInstance()");
    }

    return m_pFirstInstance;
}

JukeboxController* JukeboxController::GetSecondInstance()
{
    if(m_pSecondInstance == 0) {
        PT_START_TIME(PT_OBJ_NAME_1, 0);
        m_pSecondInstance = new JukeboxController(MP_MODE_JUKEBOX);
        m_pSecondInstance->SetMyMediaMode(MP_MODE_JUKEBOX);
        PT_END_TIME(PT_OBJ_NAME_1, "JukeboxController::GetSecondInstance()");
    }

    return m_pSecondInstance;
}

JukeboxController* JukeboxController::GetThirdInstance()
{
    if(m_pThirdInstance == 0) {
        PT_START_TIME(PT_OBJ_NAME_1, 0);
        m_pThirdInstance = new JukeboxController(MP_MODE_JUKEBOX);
        m_pThirdInstance->SetMyMediaMode(MP_MODE_JUKEBOX);
        PT_END_TIME(PT_OBJ_NAME_1, "JukeboxController::GetThirdInstance()");
    }

    return m_pThirdInstance;
}
#endif

JukeboxController::~JukeboxController()
{
    MC_LOG << "~JukeboxController()" << LOG_ENDL;
}

JukeboxController::JukeboxController(MEDIA_PLAYER_MODE eMode, QObject *parent) : USBController(eMode, parent)
{
    // add code
}    
