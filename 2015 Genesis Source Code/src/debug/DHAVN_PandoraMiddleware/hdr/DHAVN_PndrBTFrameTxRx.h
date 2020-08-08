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
 *  This module will connect to BT device. Send and receive data 
 *  over BT connection
 *
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  05/12/2011  0.1 Vandana Ahuja   header file created
 ******************************************************************
 */


#ifndef DHAVN_PNDR_BTFRAME__TXRX_H
#define DHAVN_PNDR_BTFRAME__TXRX_H

#include <QObject>
#include <QMutex>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include "DHAVN_PndrFrameTxRx.h"
#include "DHAVN_LogUtil.h"

//Forward Declarations
class CPandoraBTDBusInterface;

class CPndrBTFrameTxRx : public CPndrFrameTxRx
{
    Q_OBJECT
    USE_LOG_UTIL;
    /**
    * This enumaration will be used for checking BTController
    * initialization completed or not.
    */
    typedef enum
    {
        INIT_COMPLETED = 0,   /** No active device */
        INIT_NOT_COMPLETED = 1           /** Max number of devices possible */
    } EInitialized;

    /**
    * This enumaration will be used for checking error codes
    * during SPPDataWrite
    */
    typedef enum
    {
        SPP_DATA_WRITE_SUCCESS = 0,        /** SPPDataWrite Success */
        ERR_SPP_DISCONNECTED = 1,          /** SPPDataWrite error. SPPDisconnected */
        ERR_SPP_INCORRECT_PACKET_SIZE = 2, /** SPPDataWrite error. Wrong packet size */
        ERR_SPP_INVALID_STATUS = 3         /** SPPDataWrite error. Invalid status */
    } ESPPDataWriteError;

public:
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
     CPndrBTFrameTxRx();

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
     ~CPndrBTFrameTxRx();

     /** Initializes the transport layer */
     EPndrError Init();

     /** Releases the memory allocated */
     void DeInit();

    /**
     *******************************************************************
     * Connect BT
     *
     * @param      None
     * @return     true on success else false
     * 
     *******************************************************************
     */
    bool Connect();

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
    void BTDataReceived(const QByteArray& inFrame);

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
    void BTDeviceStatusUpdated(const EDeviceType inDeviceType, bool inIsConnected);

public Q_SLOTS:
    /**
     *******************************************************************
     * onSPPConnected
     *
     * @param      None
     *
     * @return     None
     *
     *******************************************************************
     */
    void onSPPConnected(bool inConnect);

    /**
     *******************************************************************
     * onSPPDisconnected
     *
     * @param      None
     *
     * @return     None
     *
     *******************************************************************
     */
    void onSPPDisconnected(bool inDisconnect);

    /**
     *******************************************************************
     * onSPPDataReadComplete
     *
     * @param      QDBusVariant
     *
     * @return     None
     *
     *******************************************************************
     */
    void onSPPDataReadComplete(const QByteArray);

    /**
     *******************************************************************
     * onSPPDataWriteToBTStatus
     *
     * @param      error - Errorcode if any during SPPDataWrite
     *              0- Success, 1- SPPDisconnected, 2- Wrong Packet Size, 3- Invalid Status
     *
     * @return     None
     *
     *******************************************************************
     */
    void onSPPDataWriteToBTStatus(const int error);


private:
    /** Handle to the Dbus session*/
    CPandoraBTDBusInterface* m_pDBusInterface;

    /** to check ipodcontroller  initialization completed or not.*/
    EInitialized m_eInitialized;

    /** List of requests that has failed */
    QList <QByteArray> m_FailedReqArray;

};

#endif //DHAVN_PNDR_BTFRAME__TXRX_H
