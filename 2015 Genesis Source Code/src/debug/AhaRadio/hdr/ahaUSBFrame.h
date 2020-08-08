#ifndef AHAUSBFRAME_H
#define AHAUSBFRAME_H

#include <QVector>
#include <QtGlobal>//han
#include <DHAVN_IPodAppController_CommonVars.h>
#include <DHAVN_IPodAppController_Def.h>

#include <stdio.h>
#include <unistd.h>
#include <QTimer>
#include <DHAVN_LogUtil.h>
#include <QTime>

#include "ahaCommonUtility.h"
#include "ahaImageUtility.h"
#include "ahaencoder.h"
#include "ahadecoder.h"
#include "ahapredefinedvalues.h"
#include "ahaAbsFrameTxRx.h"

//#include <QElapsedTimer>    //wsuk.kim 130716 downloading_time


class CAhaUSBFrameTxRx : public CAhaAbsFrameTxRx
{
    Q_OBJECT
public:
	
	CAhaUSBFrameTxRx();	
	~CAhaUSBFrameTxRx();

    virtual EAhaError Connect(EDeviceType inDeviceType); //connect to ipod by calling init ipod function
    virtual void Disconnect();
    virtual void requestApplicationLaunch();
    virtual void sendData(const QByteArray data);

    EAhaError GetIpodAppControllerInstance(EDeviceType inDeviceType);
    CIPodAppController* CreateNewIpodAppControllerInstance(EIPodPorts eIPodPort);
    void DestroyIpodAppControllerInstance(EDeviceType inDeviceType);
    void ConnectIPodAppControllerSignal(CIPodAppController* pIpodAppInstance);
    void DisconnectIPodAppControllerSignal(CIPodAppController* pIpodAppInstance);

    //member variable
    CIPodAppController* m_pIPodAppController;

public slots:
    int onIPodAppControllerInitCompletion();
    //int onDataReceived(QByteArray dataArray);
    int onDeviceDisconnected(int ipodNumber);
    int onNotifySessionClosed();
    int onAudioSampleRateChanged(int nSampleRate);
    int onSessionClosed(EIPodAppErrors err);
    int onSessionOpened(int protocolIndex);
    int onAcknowledgement(EIPodAppErrors status);
    //void onInitCompletionWaitTimeOut();
private:
    /** Logger instance */
    USE_LOG_UTIL
};
#endif //AHAUSBFRAME_H
