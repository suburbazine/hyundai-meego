/*
 ******************************************************************
 *        COPYRIGHT © Teleca AB                                   *
 *----------------------------------------------------------------*
 * MODULE     :  IPodAppController                                *
 *
 * PROGRAMMERS : Pawan Gupta                                      *
 * DATE       :  16 jan 2012                                      *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :

 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef DHAVN_IPODAPPCONTROLLER_PRIVATE_H
#define DHAVN_IPODAPPCONTROLLER_PRIVATE_H

/**
  * includes
  */
#include <qobject.h>
#include <DHAVN_IPodAppController_CommonVars.h>
#include <DHAVN_IPodAppController_global.h>
#include <QTimer>
#include <DHAVN_IPodAppController_DBusInterface.h>
#include <DHAVN_LogUtil.h>

#include <QThread>

#define DOT "."

/**
  * Forward declarations
  */
class CHIDRawPacketHandle;
class CIAPPacketReader;
class CIAPPacketWriter;


struct SResendStructure
{
    QByteArray m_Data;
    int count;
};

class CIPodAppControllerPriv : public QObject
{
    USE_LOG_UTIL
    Q_OBJECT
public:
    CIPodAppControllerPriv(QObject* parent = 0);

    ~CIPodAppControllerPriv();

    EIPodAppErrors Init(const QString& AppId, const QString& BundleSeedI, int ipodNumber, int protocolIndex); // modified by Tan 2013.01.05

    EIPodAppErrors D_Init();

    EIPodAppErrors Send(const QByteArray& dataArray);

    bool IsInitialized() {return m_IsInitialized; }

    EIPodAppErrors RequestForAppLaunch(QString bundleID, bool internal=false);
    qint16 getTransactionId(bool increment=true); //Added for CR 16819: Fix for ATS

    /**
      * Instance of CHIDRawPacketHandle class
      */
    CHIDRawPacketHandle* m_RawPacketHandler;


signals:

    /**
      * This is the signal of the class
      * This will be emitted when the initialization of the class
      * is completed and its ready to be used.
      */
    void InitComplete();

    /**
      * This is the signal of the class
      * This will be emitted when the data is received
      * from the IPOD app.
      * Note: This is not the acknoledgement of any sent command.
      */
    void DataReceived(const QByteArray& dataArray);

    /**
      * This the signal of the class
      * This will be emitted when the acknowledgement of
      * any issued command is received.
      * status = true - command is successful
      * status = false - command was unsuccessful
      */
    void Acknowledgement(EIPodAppErrors status);

    /**
      * This is the signal of the class
      * This will be emitted when the Session between
      * Library and the IPOD app is closed by some reason.
      */
    void SessionClosed(EIPodAppErrors error);
    void SessionOpened(int protocolIndex); // added by Tan 2012.12.15
    void NotifySessionClosed(); // added by Tan 2013.01.05
    void AudioSampleRateChanged(int  nSampleRate); //added by Tan for Aha sampling rate changed,

    void DeviceDisconnected(int ipodNumber);

public slots:
    void packetAvailable(int lingoId, int commandId, QByteArray& iAPPacketData);
    void largePacketAvailable(int lingoId, int commandId, QByteArray& iAPPacketData);
    void SendCommandTimerExpired();
    void OnIPodDisconnected(const QString& ipodNumber);
    void OnIpodAppSessionClosed(const QString& ipodNumber, const QString& appId);
    void AppLaunchAcknowledgementSlot();
private:

    /**
      * Request for the maximum payload size
      * which can be transferred to the ipod
      */
    EIPodAppErrors RequestTransPortMaxPayLoadSize();

    /**
      * set the payload size to the handle and return the transaction id for checking
      */

	// { modified by Tan 2012.12.15
    //void SetPayloadMaxSize(const QByteArray& data);
    int SetPayloadMaxSize(const QByteArray& data); // } modified by Tan 2012.12.15

    /**
      * Deinitialize the class and make it ready
      * for the next initialization
      */
    void Deinitialize();

    /**
      * Get the session ID
      */
    int GetSessionId();

    void EmitSessionClosedSignal(EIPodAppErrors error);

    void ClearResndLogic();

    void StartResendLogic(const QByteArray& data);

    void AcknowledgeIpodDataTransferCommenad(const QByteArray& dataarray);

    QString GetHidPathFromFile();

private:
    QThread* m_RawPacketThread;

    /**
      * Instance of CIAPPacketReader class
      */
    CIAPPacketReader* m_IAPReader;

    /**
      * Instance of CIAPPacketWriter class
      */
    CIAPPacketWriter* m_IAPWriter;

    /**
      * Instance of SIPodAppHandle structure
      */
    SIPodAppHandle m_Handle;

    /**
      * Transaction ID
      */
    int m_TransactionId;

	// { added by Tan 2012.12.15
    /*  Tan: CR_InitComplete_1 add m_ReqTransportTransactId to keep track of what transactionID was used
        when calling requestTransportMaxPayloadSize()

        Explanation:
            After pandora call CIPodAppController->Init(), the ipodAppController will
        send requestTransportMaxPayloadSize to apple device
            The IpodAppController then will wait for the response 0x12 from apple device.
        When receiving the response, it will emit initComplete() signal to Pandora.

        Problem:
            There is no guarantee that the requestTransportMaxPayloadSize() was  requested
        from the Pandora. So emitting initComplete when pandora does not call CIPodAppController->Init
        is logically wrong.

        Solution:
            Check the transactionId in the response to see if it matches the transation id sent in the request.
        If match, then, we can emit initComplete when receiving 0x12. Otherwise, we should ignore it.
        because that response is not for this particular IpodAppController instance.
    */
    int m_ReqTransportTransactId;
	// } added by Tan 
    /**
      * Resend timer
      */
    QTimer* m_SendCommandTimeOutWatcher;

    /**
      * Resend command structure
      */
    SResendStructure m_ResendStruct;

    OrgLgeDbusTestInterface* m_DBusInterface;

    bool m_IsInitialized;
    static qint16 m_TransId; //Added for CR 16819: Fix for ATS    
};

#endif // DHAVN_IPODAPPCONTROLLER_PRIVATE_H
