#include "powerstatuslistener.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include "log_console.h"

#include "uistate_manager_def.h"

PowerStatusListener * PowerStatusListener::powerStatusListener = NULL;

PowerStatusListener::PowerStatusListener(QObject * obj)
: QObject(obj) {

    m_isPowerOn = true;

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.connect(MICOM_MANAGER_SERVICE_NAME, MICOM_MANAGER_POWERMANAGEMENT_PATH, MICOM_MANAGER_POWERMANAGEMENT_INTERFACE, MICOM_MANAGER_SIGNAL_POWER_KEY_EVENT, this, SLOT(PowerStatusEvent(QDBusMessage)));
}

void PowerStatusListener::PowerStatusEvent(QDBusMessage message)
{
    QList<QVariant> list;
//    MICOM_MODE_POWER_KEY_CONTROL_STATE_TYPE_T ucMicomPowerMode;

    // Checking for the message arguments
    list = message.arguments();

    if ( list.isEmpty()  == false) {

        bool curIsPowerOn = m_isPowerOn;

        int mode = list.at( 0 ).toInt();

        switch(mode)
        {
        case DEF_NORMAL_POWER_OFF:
        case DEF_LOGIC_POWER_OFF:
            m_isPowerOn = false;
            break;
        default:
            m_isPowerOn = true;
        }

        if (curIsPowerOn != m_isPowerOn) {
            emit powerStatusChanged();
        }
    }

}

bool PowerStatusListener::getPowerOn()
{
    return m_isPowerOn;
}


