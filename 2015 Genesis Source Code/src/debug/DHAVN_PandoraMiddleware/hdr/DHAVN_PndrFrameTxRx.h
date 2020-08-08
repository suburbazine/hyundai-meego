/*
 ******************************************************************
 *        COPYRIGHT ?2011, LG Electronics.  
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :   BT/USB Frame Tx Rx                                  *
 * PROGRAMMER :   Vandana Ahuja                                   *
 * DATE       :   05/12/2011                                      *
 * VERSION    :   0.1                                             *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module will connect to active device. Send and receive data 
 *  over active connection
 *
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  05/12/2011  0.1 Vandana Ahuja   header file created
 ******************************************************************
 */


#ifndef DHAVN_PNDR_FRAME_TXRX_H
#define DHAVN_PNDR_FRAME_TXRX_H

#include <QObject>
#include "DHAVN_Pndr_Defs.h"
#include "DHAVN_LogUtil.h"

//Forward Declarations
class CPndrUSBFrameTxRx;
class CPndrBTFrameTxRx;

class CPndrFrameTxRx : public QObject
{
    Q_OBJECT
public:

    virtual ~CPndrFrameTxRx();


    void ReleaseConnection();
    /**
     *******************************************************************
     * Get the instance of this class
     *
     * @param   None
     *
     * @return  Instance of CPndrFrameTxRx
     * 
     *******************************************************************
     */
   // static CPndrFrameTxRx* GetInstance();


    /**
     *******************************************************************
     * Releases instance of CPndrFrameTxRx
     *
     * @param    None
     *
     * @return     None
     *
     *******************************************************************
     */
   // void ReleaseInstance();

    /** Initializes the transport layer */
    EPndrError InitDevice(const EDeviceType inDeviceType);

    /** Releases the memory allocated */
    void DeInit();

    /**
     *******************************************************************
     * Set and connect currently selected transport. If already 
     * connected then return the instance else try to connect
     *
     * @param[in]   inDeviceType: The device to be used for sending and
     *                              receiving information. E_DEVICE_NONE
     *                              if device is disconnected.
     *
     * @return     None
     * 
     *******************************************************************
     */
    EPndrError SetTransport(const EDeviceType inDeviceType);

    /**
     *******************************************************************
     * Send the data over currently active transport
     *
     * @param[in]   inData: The data to be sent 
     *
     * @return     None
     * 
     *******************************************************************
     */
    virtual void Send(const QByteArray& inData);


    /**
     *******************************************************************
     * Disconnect currently active device. This will be called by
     * Frame Validator as part of request from Application OR as
     * part of timeout for any eack expected OR as part of state handler
     * request in absence of an expected reply within given time slot.
     *
     * @param      None
     *
     * @return     None
     * 
     *******************************************************************
     */
    virtual void Disconnect();

    /**
     *******************************************************************
     * Function for returning the max payload length supported by the
     * Active transport
     *
     * @param[in]   None
     *
     * @return     quint32
     *
     *******************************************************************
     */
    virtual quint32 GetMaxPayloadLength();

    /**
      * API to be used to launch an application
      * on an iOS device.
      *
      * parameter - QString is the application bundle identifier.
      * example for Pandora is com.pandora
      *
      */
    void LaunchApplication(QString appBundleID);

    void LogCommandID(QByteArray dataArray , bool isbt);  

public slots:
    /**
     *******************************************************************
     * Function for emitting Signal for informing that data is received
     *
     * @param[in]   inData: The data to be sent 
     *
     * @return     None
     * 
     *******************************************************************
     */
    void EmitDataReceivedSignal(const QByteArray& inFrame);

    /**
     *******************************************************************
     * Function for emitting Signal for device status
     *
     * @param[in]   inDeviceType: The device type for which status 
     *                              needs to be emitted
     * @param[in]   isConnected: The device is connected or disconnected
     *
     * @return     None
     * 
     *******************************************************************
     */
    void EmitDeviceState(const EDeviceType inDeviceType, bool isConnected);

#ifdef DHAVN_PANDORA_TESTFRAMEWORK
public slots:

    /**
     *******************************************************************
     * OnTimeout
     *
     * @param      None
     *
     * @return     None
     *
     *******************************************************************
     */
    void OnTimeout();

#endif //DHAVN_PANDORA_TESTFRAMEWORK

signals:
    /**
     *******************************************************************
     * Signal for informing that frame is received
     *
     * @param[in]   inData: The data to be sent 
     *
     * @return     None
     * 
     *******************************************************************
     */
    void DataReceived(const QByteArray& inFrame);

    /**
     *******************************************************************
     * Signal for informing that device is connected, if was not 
     * connected already 
     *
     * @param[in]   inDeviceType: The device which got connected
     * @param[in]   inIsConnected: The device status
     *
     * @return     None
     * 
     *******************************************************************
     */
    void DeviceStatusUpdated(const EDeviceType inDeviceType, bool inIsConnected);

    /**
     *******************************************************************
     * Signal for informing that Pandora app is launched, if was not
     * launched already on ipod
     *
     * @return     None
     *
     *******************************************************************
     */
    void ApplicationLaunched(EPndrError success);

    /**
     *******************************************************************
     * Signal for informing that Pandora app session is closed, if
     * user kills pandora application on ipod.
     *
     * @return     None
     *
     *******************************************************************
     */    

    void PndrSessionClosed(); //added by esjang 2013.06.27 for ISV #71393, ITS #167008

    /**
     *******************************************************************
     * Signal for informing that audio saple rate is changed
     * @param inSampleRate , New sample rate
     * @return     None
     *
     *******************************************************************
     */
    void AudioSamplingChanged(int inSampleRate);

public:
    /** Constructor as private to make this class as singleton */
    CPndrFrameTxRx();

#ifdef DHAVN_PANDORA_TESTFRAMEWORK
    QTimer m_Timer;
#endif //DHAVN_PANDORA_TESTFRAMEWORK

private:
    /** Usb Transport instance */
    CPndrUSBFrameTxRx*  m_pUsbTxRx;

    /** BT Transport instance */
    CPndrBTFrameTxRx*  m_pBtTxRx;

    /** Active Transport instance */
    CPndrFrameTxRx*  m_pActiveTxRx;

    /** Instance counter */
   // static int m_nInstanceCounter;

    /** Reference to self */
   // static CPndrFrameTxRx* m_pInstance;

#ifdef DHAVN_PANDORA_TESTFRAMEWORK
    QList<QByteArray> reqListArray;
#endif //DHAVN_PANDORA_TESTFRAMEWORK

    USE_LOG_UTIL;

};


#endif //DHAVN_PNDR_FRAME_TXRX_H
