/*
 ******************************************************************
 *        COPYRIGHT © 2011, LG Electronics.  
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :   USB Frame Tx Rx                                  *
 * PROGRAMMER :   Dayasudhan Kuruva                                   *
 * DATE       :   30/01/2011                                      *
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
 *  07/12/2011  0.1 Satyanarayana B Cpp file created
 *  30/01/2011  0.2 Dayasudhan -> IPodAppController init and send logic implemented
 ******************************************************************
 */

#include "DHAVN_PndrUSBFrameTxRx.h"
#include <QDebug>

const QString   c_AppId                     = "com.pandora.link.v1";
const QString   c_BundleSeedId              = "6KVQ6HHK5F";
const quint32   c_Max_Payload_Length_USB    = 2000;
const int       gPandoraProtocolIndex       = 0x01; //added by esjang 2013.01.08 for AHA & new iPOD Controller


/**
 *******************************************************************
 * Default constructor
 *
 * @param   None
 *
 * @return  Instance of CPndrUSBFrameTxRx
 *
 *******************************************************************
 */
CPndrUSBFrameTxRx::CPndrUSBFrameTxRx()
   : m_eInitialized(INIT_NOT_COMPLETED) //esjang - Static Test 6496, uninit_member 2012.11.01
   , m_pIPodAppController(NULL) // added by esjang 2012.11.12 for Static Test CID 13141
{
    m_eConnectedDeviceType = DEVICE_TYPE_NONE;
    m_bAppLaunchRequested = false;

    LOG_INIT_V2("Pandora");
}

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
CPndrUSBFrameTxRx::~CPndrUSBFrameTxRx()
{
    LOG_TRACE<< "CPndrUSBFrameTxRx::~CPndrUSBFrameTxR:Entry" <<LOG_ENDL;
    DeInit();
    LOG_TRACE<< "CPndrUSBFrameTxRx::~CPndrUSBFrameTxR:Exit" <<LOG_ENDL;

}


 void CPndrUSBFrameTxRx::ReleaseConnection()
 {
     if(m_pIPodAppController)
     {
          disconnect(m_pIPodAppController,0,this,0);

          EIPodAppErrors err = m_pIPodAppController->D_Init();

          LOG_INFO<<" m_pIPodAppController->D_Init() -> "<<err<<LOG_ENDL;
     }
 }

/**
 *******************************************************************
 * Initializes the USB transport layers
 *
 * @param   None
 *
 * @return  EPndrError
 *
 *******************************************************************
 */
EPndrError CPndrUSBFrameTxRx::Init()
{
    LOG_TRACE<< "CPndrUSBFrameTxRx::Init:Entry" <<LOG_ENDL;

    EPndrError ret = E_SUCCESS;
    m_eInitialized = INIT_NOT_COMPLETED;

#ifdef DHAVN_PANDORA_USB_ENABLED
    ret = E_INIT_FAILED;

if(m_pIPodAppController == NULL){
    m_pIPodAppController = new CIPodAppController();
}
    LOG_HIGH<< "CPndrUSBFrameTxRx::Init:DHAVN_PANDORA_USB_ENABLED" <<LOG_ENDL;

    if(m_pIPodAppController)
    {
        //qDebug()<<"CPndrUSBFrameTxRx::Init() ipod instance : "<<m_pIPodAppController;

        LOG_TRACE<< "CPndrUSBFrameTxRx::Init m_pIPodAppController" <<LOG_ENDL;
        /*bool connVal =*/  connect(m_pIPodAppController, SIGNAL(InitComplete()), this, SLOT(onIPodAppControllerInitCompletion()));
        /*bool connVal1 =*/ connect(m_pIPodAppController, SIGNAL(DataReceived(QByteArray)), this , SLOT(onDataReceived(QByteArray)));
        /*bool connVal2 =*/ connect(m_pIPodAppController, SIGNAL(DeviceDisconnected(int)), this, SLOT(onDeviceDisconnected(int)));
        /*bool connVal3 =*/ connect(m_pIPodAppController, SIGNAL(SessionClosed(EIPodAppErrors)), this , SLOT(onSessionClosed(EIPodAppErrors)));
        /*bool connVal4 =*/ connect(m_pIPodAppController, SIGNAL(Acknowledgement(EIPodAppErrors)), this ,SLOT(onAcknowledgement(EIPodAppErrors)));
        /*bool connVal5 =*/ connect(m_pIPodAppController, SIGNAL(NotifySessionClosed()), this ,SLOT(onNotifySessionClosed())); //added by esjang 2013.06.27 for ISV #71393, ITS #167008
                            connect(m_pIPodAppController, SIGNAL(AudioSampleRateChanged(int)), this ,SIGNAL(AudioSamplingChanged(int)));

       // LOG_TRACE<< "CPndrUSBFrameTxRx::Init connect" << connVal << connVal1  <<connVal2 << connVal3 << connVal4 << connVal5<< LOG_ENDL;

        ret = E_SUCCESS;
    }
    else
    {
        LOG_CRITICAL << "m_pIPodAppController mem allocation Failed."<< LOG_ENDL;
        ret = E_INIT_FAILED;
    }
#endif
    LOG_TRACE<< "CPndrUSBFrameTxRx::Init:EXIT RET" << ret <<LOG_ENDL;

    return ret;
}

/**
 *******************************************************************
 *  DeInit Function for releasing memory
 *
 *
 * @param      None
 *
 * @return     None
 *
 *******************************************************************
 */

void CPndrUSBFrameTxRx::DeInit()
{
    LOG_TRACE<< "CPndrUSBFrameTxRx::DeInit:Entry" <<LOG_ENDL;

    m_eInitialized = INIT_NOT_COMPLETED;

#ifdef DHAVN_PANDORA_USB_ENABLED
    if(m_pIPodAppController)
    {
        LOG_TRACE<< "m_pIPodAppController::DeInit called " <<LOG_ENDL;
        EIPodAppErrors err = m_pIPodAppController->D_Init();
        LOG_TRACE<< "m_pIPodAppController::DeInit err " << err << LOG_ENDL;

        delete m_pIPodAppController;
        m_pIPodAppController =  NULL;
    }
#endif
    LOG_TRACE<< "CPndrUSBFrameTxRx::DeInit:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Connect IPod USB
 *
 * @param      inDeviceType The Device type whether USB Front or Rear
 * @return     true on success else false
 *
 *******************************************************************
 */
bool CPndrUSBFrameTxRx::Connect(const EDeviceType inDeviceType)
{
    LOG_TRACE<< "CPndrUSBFrameTxRx::Connect:Entry" <<LOG_ENDL;

    bool ret = false;
#ifdef DHAVN_PANDORA_USB_ENABLED
    EIPodAppErrors err = IPOD_APPSUCCESS;


    LOG_TRACE<< "CPndrUSBFrameTxRx::Connect: m_pIPodAppController" << "inDeviceType= " << inDeviceType<< LOG_ENDL;

    EIPodPorts eIPodPort = eIPOD_PORT_NONE;
    if(DEVICE_TYPE_IPHONE_USB_FRONT == inDeviceType)
    {
        eIPodPort = eIPOD_PORT_1 ;
    }
    else if(DEVICE_TYPE_IPHONE_USB_REAR == inDeviceType)
    {
        eIPodPort = eIPOD_PORT_2 ;
    }

    // { modified by esjang 2013.01.08 for adding pandrora protocol index to init iPOD Controller
    //err = m_pIPodAppController->Init(c_AppId,c_BundleSeedId,eIPodPort);
    err = m_pIPodAppController->Init(c_AppId,c_BundleSeedId,eIPodPort, gPandoraProtocolIndex);
    // } modified by esjang 2013.01.08 for adding pandrora protocol index to init iPOD Controller

    LOG_INFO<< "CPndrUSBFrameTxRx::Connect: err =" << err <<LOG_ENDL;


    if(IPOD_APPSUCCESS == err)
    {
        m_eConnectedDeviceType = inDeviceType;
        ret = true;
    }
    else
    {
        LOG_CRITICAL<<"Ipod Init failed. "<<LOG_ENDL;
        m_eInitialized = INIT_NOT_COMPLETED;
        emit USBDeviceStatusUpdated(inDeviceType,false);
    }


#endif
    LOG_TRACE<< "CPndrUSBFrameTxRx::Connect:Exit ret = " << ret <<LOG_ENDL;
    return ret;
}

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
void CPndrUSBFrameTxRx::Send(const QByteArray& inData)
{
   LOG_TRACE<< "CPndrUSBFrameTxRx::Send:Entry" <<LOG_ENDL;
   LogCommandID(inData, false);


#ifdef DHAVN_PANDORA_USB_ENABLED

    if(m_eInitialized == INIT_COMPLETED)
    {
        //LOG_INFO<< "CPndrUSBFrameTxRx::Send:m_eInitialized" << m_eInitialized <<LOG_ENDL;


        //LOG_TRACE<< "CPndrUSBFrameTxRx::Send:m_pIPodAppController"<<LOG_ENDL;

        EIPodAppErrors err = m_pIPodAppController->Send(inData);
        //LOG_INFO<< "CPndrUSBFrameTxRx::Send to Device status = " << err <<LOG_ENDL;

        switch(err)
        {
        case IPOD_APPSUCCESS:
            //LOG_INFO<<"No error "<<LOG_ENDL;
            break;
        case IPOD_APPERROR_TIMEOUT:
            LOG_CRITICAL<<"CPndrUSBFrameTxRx::Command is timeout "<<LOG_ENDL;
            break;
        case IPOD_APPERROR_NOTINITIALIZED:
            LOG_CRITICAL<<"CPndrUSBFrameTxRx::The controller is not initialized "<<LOG_ENDL;
            break;
        case IPOD_APPERROR_NOTRESPONDING:
            LOG_CRITICAL<<"CPndrUSBFrameTxRx::The controller is not responding "<<LOG_ENDL;
            break;
        case IPOD_APPERROR_NOTCONNECTED:
            LOG_CRITICAL<<"CPndrUSBFrameTxRx::Device is not connected "<<LOG_ENDL;
            break;
        case IPOD_APPERROR_APPNOTOPENED:
            LOG_CRITICAL<<"CPndrUSBFrameTxRx::Aplication is not opened "<<LOG_ENDL;
            break;
       case IPOD_APPERROR_BUSY:
            LOG_CRITICAL<<"CPndrUSBFrameTxRx::Controller is already busy. Hence apend to queue. "<<LOG_ENDL;
            //Append the req to a queue so that in case of error req can be resend
            //Do not append to USB queue if already its the current req being processed
            //i.e, when On timeout in validator, it is resending the same request
            if(!m_FailedReqArray.contains(inData))
            {
                m_FailedReqArray.append(inData);
                LOG_CRITICAL<<"CPndrUSBFrameTxRx::APPENDED to queue frame = " << inData.toHex().left(32)<<LOG_ENDL;
            }
            LOG_CRITICAL<<"CPndrUSBFrameTxRx::m_FailedReqArray.count = " << m_FailedReqArray.count()<<LOG_ENDL;

            break;
        case IPOD_APPERROR_SESSIONCLOSED:
            LOG_CRITICAL<<"CPndrUSBFrameTxRx::Session closed"<<LOG_ENDL;
            break;
        default:
            LOG_CRITICAL<<"CPndrUSBFrameTxRx::Other Error "<<LOG_ENDL;
            break;
        }
//        if(IPOD_APPSUCCESS != err && IPOD_APPERROR_BUSY != err)
//        {
//            LOG_CRITICAL<<"CPndrUSBFrameTxRx::The command is not sent"<<LOG_ENDL;

//            //m_eInitialized = INIT_NOT_COMPLETED;
//            //emit USBDeviceStatusUpdated(m_eConnectedDeviceType,false);
//            //m_eConnectedDeviceType = DEVICE_TYPE_NONE;
//        }

    }
    else
    {
        LOG_CRITICAL<<"CPndrUSBFrameTxRx::IPodAppController not intialized status = " << m_eInitialized <<LOG_ENDL;
    }
#endif
   // LOG_TRACE<< "CPndrUSBFrameTxRx::Send:Exit" <<LOG_ENDL;

}

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
void CPndrUSBFrameTxRx::Disconnect()
{
    LOG_TRACE<< "CPndrUSBFrameTxRx::Disconnect:Entry" <<LOG_ENDL;
#ifdef DHAVN_PANDORA_USB_ENABLED

    /* De-Initialize the library and disconnect from IPOD App*/
    m_pIPodAppController->D_Init();
#endif
    m_eInitialized = INIT_NOT_COMPLETED;
    LOG_TRACE<< "CPndrUSBFrameTxRx::Disconnect:Exit" <<LOG_ENDL;

}


/**
 *******************************************************************
 * Function for returning the max payload length supported by the
 * USB transport
 *
 * @param[in]   None
 *
 * @return     quint32
 *
 *******************************************************************
 */
quint32 CPndrUSBFrameTxRx::GetMaxPayloadLength()
{
  //  LOG_TRACE<< "Inside CPndrUSBFrameTxRx::onIPodAppControllerInitCompletion" <<LOG_ENDL;

    return c_Max_Payload_Length_USB ;
}

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
void CPndrUSBFrameTxRx::LaunchApplication(QString appBundleID)
{
    LOG_TRACE<< "CPndrUSBFrameTxRx::LaunchApplication start"<<LOG_ENDL;
    EIPodAppErrors err = m_pIPodAppController->LaunchApplication(appBundleID);
    if(IPOD_APPSUCCESS == err)
    {
        m_bAppLaunchRequested = true;
    }

    LOG_TRACE<< "CPndrUSBFrameTxRx::LaunchApplication end"<<LOG_ENDL;
}

#ifdef DHAVN_PANDORA_USB_ENABLED

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
void CPndrUSBFrameTxRx::onIPodAppControllerInitCompletion()
{
    LOG_SLOT<< "CPndrUSBFrameTxRx::onIPodAppControllerInitCompletion:Entry" << "m_eConnectedDeviceType = "<< m_eConnectedDeviceType <<LOG_ENDL;
    m_eInitialized = INIT_COMPLETED;
    emit USBDeviceStatusUpdated(m_eConnectedDeviceType,true);
    LOG_SLOT<< "CPndrUSBFrameTxRx::onIPodAppControllerInitCompletion:Exit" <<LOG_ENDL;

}

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
void CPndrUSBFrameTxRx::onDataReceived(QByteArray dataArray)
{
    LOG_SLOT<<"CPndrUSBFrameTxRx::onDataReceived:Entry"<<LOG_ENDL;
    if(dataArray[0] ==  0x7e)
    {
        LogCommandID(dataArray, false);            
    }
    emit USBDataReceived(dataArray);
    //LOG_SLOT<<"CPndrUSBFrameTxRx::onDataReceived :Exit"<<LOG_ENDL;
}

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
void CPndrUSBFrameTxRx::onAcknowledgement(EIPodAppErrors status)
{
    //LOG_SLOT<<"CPndrUSBFrameTxRx::onAcknowledgement :Entry "<< status<<LOG_ENDL;

    if(m_bAppLaunchRequested)
    {
        LOG_HIGH<<"CPndrUSBFrameTxRx::onAcknowledgement App launch status = "<< status << LOG_ENDL;
    }

    if(IPOD_APPSUCCESS/*0*/ == status)
    {
        //LOG_INFO<<"command is successful"<< LOG_ENDL;

        if(m_bAppLaunchRequested)
        {
            //LOG_SIGNAL << "ApplicationLaunched emitted to FrameTxRx"<< LOG_ENDL;
            emit ApplicationLaunched(E_SUCCESS);
            m_bAppLaunchRequested = false;
            LOG_SIGNAL << "Application Launched emitted with Failed Array.count: "<< m_FailedReqArray.count()<< LOG_ENDL;
        }
        if(m_FailedReqArray.count())
        {
            LOG_CRITICAL<<"Sending request from FailedReqArray = " << m_FailedReqArray.at(0).toHex().left(32)<<LOG_ENDL;
            Send (m_FailedReqArray.at(0));
            m_FailedReqArray.removeAt(0);
        }
    }
    else
    {
        if(m_bAppLaunchRequested)
        {
            LOG_CRITICAL << "ApplicationLaunch failed emitted to FrameTxRx"<< LOG_ENDL;

            emit ApplicationLaunched(E_CHECK_PNDR_APP_ON_DEVICE);
            m_bAppLaunchRequested = false;
        }
        //LOG_CRITICAL<<"command was not successful"<<LOG_ENDL;
    }
   // LOG_SLOT<<"CPndrUSBFrameTxRx::onAcknowledgement :Exit"<<LOG_ENDL;

}

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

void CPndrUSBFrameTxRx::onSessionClosed(EIPodAppErrors error)
{
    LOG_CRITICAL<<"CPndrUSBFrameTxRx::onSessionClosed : "<<error<<LOG_ENDL;

    m_eInitialized = INIT_NOT_COMPLETED;
    emit USBDeviceStatusUpdated(m_eConnectedDeviceType,false);
    m_eConnectedDeviceType = DEVICE_TYPE_NONE;

    //LOG_SLOT<<"CPndrUSBFrameTxRx::onSessionClosed :Exit"<<error<<LOG_ENDL;
}

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

void CPndrUSBFrameTxRx::onDeviceDisconnected(int ipodNumber)
{
    LOG_SLOT<<"CPndrUSBFrameTxRx::onDeviceDisconnected:Entry"<<ipodNumber<<LOG_ENDL;
    m_eInitialized = INIT_NOT_COMPLETED;

    emit USBDeviceStatusUpdated(m_eConnectedDeviceType,false);

    m_eConnectedDeviceType = DEVICE_TYPE_NONE;

    LOG_SLOT<< "CPndrUSBFrameTxRx::onDeviceDisconnected:Exit" <<LOG_ENDL;
}

/**
*******************************************************************
*   This slot will be called when the Session between
*   Library and the IPOD app is closed by some reason.
*   D_Init() will be called internally and the
*   Lib will be under non initialize mode
*
* @param      none
*
* @return     none
*
*******************************************************************
*/
void CPndrUSBFrameTxRx::onNotifySessionClosed() //added by esjang 2013.06.27 for ISV #71393, ITS #167008

{
    LOG_SLOT<<"CPndrUSBFrameTxRx::onNotifySessionClosed" << LOG_ENDL;
    emit PndrSessionClosed();
//    return 0;
}

#endif

//End Of File
