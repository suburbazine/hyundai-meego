#ifndef POWERSTATUSLISTENER_H
#define POWERSTATUSLISTENER_H

#include <QObject>
#include <QDBusMessage>

/** MICOM Manager QtDBus Service name */
#define MICOM_MANAGER_SERVICE_NAME          "com.lge.car.micom"
#define MICOM_MANAGER_POWERMANAGEMENT_PATH  "/PowerManagement"
#define MICOM_MANAGER_POWERMANAGEMENT_INTERFACE "com.lge.car.micom.powermanagement"
#define MICOM_MANAGER_SIGNAL_POWER_KEY_EVENT "LgeDBus_EventPowerKey"

class PowerStatusListener : public QObject{
    Q_OBJECT


public:
    static PowerStatusListener * instance(){
        if (powerStatusListener == NULL)
            powerStatusListener = new PowerStatusListener;

        return powerStatusListener;
    }

    bool getPowerOn();

private:
    PowerStatusListener(QObject * obj = 0);

    static PowerStatusListener * powerStatusListener;

    bool m_isPowerOn;

private slots:
    void PowerStatusEvent(QDBusMessage message);

signals:
    void powerStatusChanged();
};

#endif // POWERSTATUSLISTENER_H
