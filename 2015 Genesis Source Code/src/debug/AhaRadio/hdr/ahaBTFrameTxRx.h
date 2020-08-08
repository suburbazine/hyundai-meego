#ifndef AHABTFRAMETXRX_H
#define AHABTFRAMETXRX_H

#include <QVector>
#include <QtGlobal>//han
#include <DHAVN_IPodAppController_CommonVars.h>
#include <DHAVN_IPodAppController_Def.h>

#include <stdio.h>
#include <unistd.h>
#include <QTimer>
#include "ahaCommonUtility.h"
#include "ahaImageUtility.h"
#include "ahaencoder.h"
#include "ahadecoder.h"
#include "ahapredefinedvalues.h"
#include "ahaAbsFrameTxRx.h"
#include "ahaBTDBusInterface.h"

//#include <QElapsedTimer>    //wsuk.kim 130716 downloading_time

typedef enum
{
    INIT_COMPLETED = 0,   /** No active device */
    INIT_NOT_COMPLETED = 1           /** Max number of devices possible */
} EInitialized;

typedef enum
{
    SPP_DATA_WRITE_SUCCESS = 0,        /** SPPDataWrite Success */
    ERR_SPP_DISCONNECTED = 1,          /** SPPDataWrite error. SPPDisconnected */
    ERR_SPP_INCORRECT_PACKET_SIZE = 2, /** SPPDataWrite error. Wrong packet size */
    ERR_SPP_INVALID_STATUS = 3         /** SPPDataWrite error. Invalid status */
} ESPPDataWriteError;

class CAhaBTFrameTxRx : public CAhaAbsFrameTxRx
{
    Q_OBJECT
public:

        CAhaBTFrameTxRx();
        ~CAhaBTFrameTxRx();

    virtual EAhaError Connect(EDeviceType inDeviceType); //connect to ipod by calling init ipod function
    virtual void Disconnect();
    virtual EAhaError Init();
    virtual void DeInit();
    virtual void requestApplicationLaunch();
    virtual void sendData(const QByteArray data);

    quint32 GetMaxPayloadLength();

signals:

    void DataReceived(const QByteArray& inFrame);
    //void BTDataReceived(const QByteArray& inFrame);
    //void BTDeviceStatusUpdated(const EDeviceType inDeviceType, bool inIsConnected);

public Q_SLOTS:

    void onSPPConnected(bool inConnect);
    void onSPPDisconnected(bool inDisconnect);
    void onSPPDataReadComplete(const QByteArray);
    void onSPPDataWriteToBTStatus(const int error);

public:
    /** Handle to the Dbus session*/
    CAhaBTDBusInterface* m_pDBusInterface;

    /** to check ipodcontroller  initialization completed or not.*/
    EInitialized m_eInitialized;

    /** List of requests that has failed */
    QList <QByteArray> m_FailedReqArray;
public:
    USE_LOG_UTIL
};

#endif //AHABTFRAMETXRX_H
