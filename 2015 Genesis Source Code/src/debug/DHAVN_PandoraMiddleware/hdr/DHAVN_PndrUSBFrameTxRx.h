/*
 ******************************************************************
 *        COPYRIGHT © 2011, LG Electronics.  
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :   BT Frame Tx Rx                                  *
 * PROGRAMMER :   Vandana Ahuja                                   *
 * DATE       :   05/12/2011                                      *
 * VERSION    :   0.1                                             *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module will connect to USB device. Send and receive data 
 *  over USB connection
 *
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  05/12/2011  0.1 Vandana Ahuja   header file created
 ******************************************************************
 */

#ifndef DHAVN_PNDR_USBFRAME_TXRX_H
#define DHAVN_PNDR_USBFRAME_TXRX_H

#include <QByteArray>

#ifdef DHAVN_PANDORA_USB_ENABLED
#include <DHAVN_IPodAppController_CommonVars.h>
#include <DHAVN_IPodAppController_Def.h>
#endif

#include "DHAVN_PndrFrameTxRx.h"
#include "DHAVN_LogUtil.h"

class CPndrUSBFrameTxRx : public CPndrFrameTxRx
{
    Q_OBJECT
    USE_LOG_UTIL;
    /**
    * This enumaration will be used for checking ipodcontroller
    * initialization completed or not.
    */
    typedef enum
    {
        INIT_COMPLETED,   /** No active device */
        INIT_NOT_COMPLETED           /** Max number of devices possible */
    } EInitialized;
public:

     void ReleaseConnection();

    /**
     *******************************************************************
     * Default constructor
     *
     * @param   None
     *
     * @return  Instance of CPndrFrameTxRx
     *
     *******************************************************************
     */
     CPndrUSBFrameTxRx();

     /**
      *******************************************************************
      * Destructor
      *
      * @param   None
      *
      * @return  None
      *
      *******************************************************************
      */
     ~CPndrUSBFrameTxRx();

     /** Initializes the transport layer */
     EPndrError Init();

     /** Releases the memory allocated */
     void DeInit();

    /**
     *******************************************************************
     * Connect BT
     *
     * @param      EDeviceType The USB Device Type: Front or Rear
     * @return     true on success else false
     * 
     *******************************************************************
     */
    bool Connect(const EDeviceType inDeviceType);

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
    void Send(const QByteArray& inData);


    /**
     *******************************************************************
     * Disconnect this device
     *
     * @param      None
     *
     * @return     None
     * 
     *******************************************************************
     */
    void Disconnect();

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
     quint32 GetMaxPayloadLength();

     /**
       * API to be used to launch an application
       * on an iOS device.
       *
       *parameter - QString is the application bundle identifier.
       *example for Pandora is com.pandora
       *
       * Return value as IPOD_APPSUCCESS means the
       * RequestApplicationLaunch command sent successfully
       * to an iOS device.
       * One must wait on the Acknowledgement() signal for the
       * status of the command execution.
       */
     void LaunchApplication(QString appBundleID);

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
    void USBDataReceived(const QByteArray& inFrame);

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
    void USBDeviceStatusUpdated(const EDeviceType inDeviceType, bool inIsConnected);

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
     * Signal for informing that Pandora app new audio sample rate
     * @param sampleRate, Sample rate
     * @return     None
     *
     *******************************************************************
     */
    void AudioSamplingChanged(int sampleRate);

#ifdef DHAVN_PANDORA_USB_ENABLED
 public slots:
    /**
    *******************************************************************
    *   This slot will be called when the initialization of the
    *   m_pIPodAppController is completed and its ready to be used.
    *
    * @param      None
    *
    * @return     None
    *
    *******************************************************************
    */
    void onIPodAppControllerInitCompletion();

    /**
    *******************************************************************
    *  This slot will be called when the data is received
    *  from the IPOD app.
    *
    * @param      const QByteArray& dataArra
    *
    * @return     None
    *
    *******************************************************************
    */
    void onDataReceived(QByteArray dataArray);

    /**
    *******************************************************************
    *  This slot will be called when the acknowledgement of
    *  any issued command is received.
    *
    *
    *  @param     status = true - command is successful
    *             status = false - command was unsuccessful
    *  @return    none
    *
    *******************************************************************
    */
    void onAcknowledgement(EIPodAppErrors status);

    /**
    *******************************************************************
    *   This slot will be called when the Session between
    *   Library and the IPOD app is closed by some reason.
    *   D_Init() will be called internally and the
    *   Lib will be under non initialize mode
    *
    * @param      EIPodAppErrors error
    *
    * @return     none
    *
    *******************************************************************
    */
    void onSessionClosed(EIPodAppErrors error);

    /**
    *******************************************************************
    * This slot will be called when the
    * device will be disconnected from the HU
    * D_Init() will be called internally and the
    * Lib will be under non initialize mode
    *
    * @param      int ipodNumber
    *
    * @return     None
    *
    *******************************************************************
    */
    void onDeviceDisconnected(int ipodNumber);

    /**
    *******************************************************************
    *   This slot will be called when the Session between
    *   Library and the IPOD app is closed by some reason.
    *   D_Init() will be called internally and the
    *   Lib will be under non initialize mode
    *
    * @param      EIPodAppErrors error
    *
    * @return     none
    *
    *******************************************************************
    */
    void onNotifySessionClosed(); //added by esjang 2013.06.27 for ISV #71393, ITS #167008

private:

     /** Ipod App controller instance */
    CIPodAppController * m_pIPodAppController;

     /** List of requests that has failed */
    QList <QByteArray> m_FailedReqArray;
#endif

private:
    /** to check ipodcontroller  initialization completed or not.*/
    EInitialized m_eInitialized;

    EDeviceType m_eConnectedDeviceType;

    bool m_bAppLaunchRequested;
};

#endif //DHAVN_PNDR_USBFRAME_TXRX_H
