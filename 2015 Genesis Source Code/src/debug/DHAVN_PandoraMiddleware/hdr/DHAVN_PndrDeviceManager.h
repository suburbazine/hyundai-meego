/*
 ******************************************************************
 *        COPYRIGHT © 2011, LG Electronics.  
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     : Device Connector                                  *
 * PROGRAMMER : Vandana Ahuja                                     *
 * DATE       : 03/12/2011                                        *
 * VERSION    : 0.1                                               *
 *----------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module takes care of connecting/disconnecting device
 *  and performing minimal operations which are mandatory after
 *  initializing the connection
 *  It will use Frame encoder class to encode the data call
 *  send function of FrameValidator for sending request.
 *
 *  State check for validity of request will not be performed in 
 *  this module
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  03/12/2011  0.1 Vandana Ahuja   header file created           *
 *  08/12/2011  0.2 Wasim Shaikh    header file modified          *
 *  01/03/2013  0.3 Radhakrushna Mohanty Imlemented to pandora device
 *                at same time, and launching mechanism on iOS.   *
 ******************************************************************
 */


#ifndef DHAVN_PNDR_DEVICE_MANAGER_H
#define DHAVN_PNDR_DEVICE_MANAGER_H

#include <QByteArray>
#include <QObject>
#include <QTimer>
#include "DHAVN_Pndr_Defs.h"
#include "DHAVN_LogUtil.h"

//Forward Declarations
class CPndrFrameEncoder;
class CPndrFrameValidator;
class CPndrFrameTxRx;

/**
* Class to maintain device connection and minimal commands sent to 
* server for initialization purpose
* No slots required here. All will be listened by State Handler
*/
class CPndrDeviceManager  : public QObject
{
    Q_OBJECT

    USE_LOG_UTIL;
public:

    /**
     *******************************************************************
     * Constructor:
     *  Setting Device Type to NULL. creating instance of singleton class
     *
     *
     *
     * @param      None
     *
     * @return     None
     *
     *******************************************************************
     */
    CPndrDeviceManager();

    /**
     *******************************************************************
     *  Set FrameTxRX instace
     * @param      inFrameTxRx , Instance of CPndrFrameTxRx
     * @return     None
     *******************************************************************
     */
    void SetFrameTxRX(CPndrFrameTxRx* inFrameTxRx);

    /**
     *******************************************************************
     * Set Encoder instace
     * @param      inFrameEncoder , Instance of CPndrFrameEncoder
     * @return     None
     *******************************************************************
     */
    void SetEncoder(CPndrFrameEncoder* inFrameEncoder);

    /**
     *******************************************************************
     * Set Validator instace
     * @param      inFrameValidator , Instance of CPndrFrameValidator
     * @return     None
     *******************************************************************
     */
    void SetValidator(CPndrFrameValidator* inFrameValidator);

    /**
     *******************************************************************
     * Destructor:
     *  Setting Device Type to NULL. Calling release to Free instance of
     *  Classes
     *
     *
     *
     * @param      None
     *
     * @return     None
     *
     *******************************************************************
     */
    ~CPndrDeviceManager();


    /**
     *******************************************************************
     * Get currently connected device, if this function is
     *              called by StateHandler that means that device is 
     *              already connected.
     *
     * @param None
     *
     * @return     EDeviceType: Device that is currently connected
     *
     *******************************************************************
     */
     EDeviceType  GetConnectedDevice();

    /**
     *******************************************************************
     * Request to connected to device type
     *
     * @param[in]    inDeviceType: One of the predefined device type
     *
     * @return     None
     *
     *******************************************************************
     */
     EPndrError ConnectDevice(const EDeviceType inDeviceType);

    /**
     *******************************************************************
     * Request to connected to Pandora through already connected device
     * This function will also send request for sending elapsed time 
     * continuously
     *
     *  @param[in]  inAlbumArtDimension The album art size to be provided
     *                              for all Albums
     *  @param[in]  inImgType    The album art img type to be provided
     *                              for all Albums
     * @param[in]   inStationArtDimension size of station art or genre station art image
     *
     * @return     None
     *
     *******************************************************************
     */
     void ConnectPandora(const quint16 inAlbumArtDimension,
                               const EImageType inImgType,
                               const quint16 inStationArtDimension);
    /**
     *******************************************************************
     * Request to disconnect from device. 
     *
     * @param[in]    inDeviceType: One of the predefined device type
     *
     * @return     None
     *
     *******************************************************************
     */
     void Disconnect();

    /**
     *******************************************************************
     * Informaion from State Handler that current active device is 
     * disconnected. 
     *
     * @param    None
     *
     * @return   None
     *
     *******************************************************************
     */
     void Disconnected();
    
    //{ added by esjang 2013.05.15 for certification issue, sessionterminate.

    /**
     *******************************************************************
     * Informaion from State Handler that current active device is 
     * SessionTerminate. 
     *
     * @param    None
     *
     * @return   None
     *
     *******************************************************************
     */
     void SessionTerminate();
    //} added by esjang 2013.05.15 for certification issue, sessionterminate.
    
     /**
      *******************************************************************
      * Requests the current applicate state of the Mobile app
      *
      *
      * @param    None
      *
      * @return   None
      *
      *******************************************************************
      */
     void GetPandoraStatus();
public slots:
     /**
      *******************************************************************
      * Signal for informing that Pandora app is launched, if was not
      * launched already on ipod
      *
      * @return     None
      *
      *******************************************************************
      */
     void onApplicationLaunched(EPndrError success);

     /**
      *******************************************************************
      * This function will be called from timeout signal of app launch.
      * @return     None
      *
      *******************************************************************
      */
     void onTimeout();


     //void onSessionStartAcked(); // added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
     void sessionTimerStart();  // added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack  
     void sessionTimerStop();   // added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
     void onSetMaxFrameDrops(bool inMaxFrameDrops){IsMaxFrameDrops = inMaxFrameDrops;} // added by esjang 2013.05.14 for certification #2.3 11 frame drops. Disable session termination.
     void onDeviceStatusUpdated(const EDeviceType /*inType*/,bool inStatus); // Stop Recovery manager
private slots:

     /**
      *******************************************************************
      * its a function that send the SESSION START message to pandora.
      * This function can be used in Timer::singleShot() api to start session
      * after some delay
      * @return     None
      *
      *******************************************************************
      */
     void sendSessionStartAfterDelay();
     void connectionTimedOut(); /// Reconnect , Recovery manager

signals:
     void ApplicationLaunched(EPndrError success);

private:
    /** Currently connected device */
    EDeviceType m_DeviceType;  

    /** Instance of Encoder class */
    CPndrFrameEncoder* m_pFrameEncoder;
    CPndrFrameValidator* m_pFrameValidator;
    CPndrFrameTxRx* m_pFrameTxRx;
    quint16 m_nAlbumArtDimension;
    quint16 m_nStationArtDimension;
    EImageType m_ImageType;
    QTimer m_Timer;
    QTimer m_Session; // added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
    int m_nSessionCount; // added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
    bool IsMaxFrameDrops; // added by esjang 2013.05.14 for certification #2.3 11 frame drops. Disable session termination.
    /** Recovery timer to reconnect the device */
    QTimer m_ConnectTimer;
    /** Thre times rtetrying */
    int m_nCounter;
};


#endif //DHAVN_PNDR_DEVICE_MANAGER_H
