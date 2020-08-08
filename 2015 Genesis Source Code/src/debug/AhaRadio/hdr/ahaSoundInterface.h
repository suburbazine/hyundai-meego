#ifndef AHASOUNDINTERFACE_H
#define AHASOUNDINTERFACE_H
#include <libudev.h>
#include <QProcess>
#include <QString>
#include <QTimer>
#include <DHAVN_IPodAppController_CommonVars.h>
#include <DHAVN_IPodAppController_Def.h>
#include <DHAVN_AudioSourceType.h>
#include <DHAVN_GSTAudioRouterInterface.h>

#include "ahapredefinedvalues.h"

class CSoundInterface : public QObject
{
    Q_OBJECT
public:

    CSoundInterface();
    ~CSoundInterface();

    void setVehicleType(int nVehicleType);

    void openSoundChannel(EIPodPorts nCurrIPodPortNum);
    void changeSampleRate(EIPodPorts nCurrIPodPortNum, int nSampleRate);
    void resetSampleRate(EIPodPorts nCurrIPodPortNum);
    void updateSampleRate(EIPodPorts nCurrIPodPortNum);

    void closeSoundChannel();
private:

    EAudioSource m_OpenedAudioSource;

    void FindAudioDevice(EIPodPorts nCurrIPodPortNum, QString &strSoundSource);
    EIPodPorts GetUSBPortNumber(QString sysPath);
    int m_nVehicleType;
    EAudioSource PortToAudioSource(EIPodPorts nCurrIPodPortNum);

    EIPodPorts AudioSourceToPort(EAudioSource eAudioSource);

    GSTAudioRouterDBus* m_pAudioRouter;

    int m_nCurrentSampleRate_Front;
    int m_nCurrentSampleRate_Rear;
    int m_nOpenedSampleRate;

    /** Logger instance */
    USE_LOG_UTIL
};
#endif // AHASOUNDINTERFACE_H
