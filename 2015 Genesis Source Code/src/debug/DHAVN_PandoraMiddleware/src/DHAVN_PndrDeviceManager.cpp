/*
 ******************************************************************
 *        COPYRIGHT  2011, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     : Device Connector                                  *
 * PROGRAMMER : Wasim Shaikh                                      *
 * DATE       : 08/12/2011                                        *
 * VERSION    : 0.1                                               *
 *---------------------------------------------------------------*
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
 * MODIFICATION RECORDS
 *  08/12/2011  0.1 Wasim Shaikh   Implementation file created
 *  01/03/2013  0.3 Radhakrushna Mohanty Imlemented for two ipod support
 *                  pandora launching mechanism on iOS.
 *
 ******************************************************************
 */
#include "DHAVN_PndrDeviceManager.h"
#include "DHAVN_PndrLinkConstants.h"
#include "DHAVN_PndrFrameEncoder.h"
#include "DHAVN_PndrFrameValidator.h"
#include "DHAVN_PndrFrameTxRx.h"
#include <QTimer>

const QString c_BundleAppId = "com.pandora";
// modified by esjang 2013.03.28 for Pandora Certification #1.1
const QString c_ACCESSORY_ID = "5699D813"; //modified by wonseok.heo for DH Accessory ID 2015.01.16
const quint8 c_FLAGS_BITMASK = 0; // modified by esjang 2013.05.09 for Pandora Certification

//const QString c_ACCESSORY_ID ="APITOOL0";

const int TIMEOUT_FOR_SESSION_START = 15000;

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

CPndrDeviceManager::CPndrDeviceManager()
    : m_nAlbumArtDimension(0) // added by esjang 2012.11.12 for Static Test CID 13137
    , m_nStationArtDimension(0) // added by esjang 2012.11.12 for Static Test CID 13137
    , m_ImageType(IMG_TYPE_NONE) // added by esjang 2012.11.12 for Static Test CID 13137
    , m_pFrameTxRx(NULL)
    , IsMaxFrameDrops(FALSE) // added by esjang 2013.05.14 for certification #2.3 11 frame drops. Disable session termination.
    , m_nCounter(0)
 , m_nSessionCount(0)// added by esjang 2013.07.08 for Static Test CID 13137
{
    LOG_INIT_V2("Pandora");
    m_DeviceType = DEVICE_TYPE_NONE;
    m_pFrameEncoder =  NULL;
    m_pFrameValidator = NULL;
    //LOG_TRACE << "CPndrDeviceManager:: CPndrDeviceManager() constructed"<< LOG_ENDL;
    connect(&m_Timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
    m_Timer.setSingleShot(true);

    //{ added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
   // LOG_INFO << "esjang 0510 CPndrDeviceManager() is start <- "<< LOG_ENDL;
    connect(&m_Session,SIGNAL(timeout()),this,SLOT(sendSessionStartAfterDelay()));
    m_Session.setSingleShot(true);


    connect(&m_ConnectTimer,SIGNAL(timeout()),this,SLOT(connectionTimedOut()));
    m_ConnectTimer.setSingleShot(true);
    //connect(m_pFrameValidator, SIGNAL(SessionStartAcked()), this, SLOT(sessionTimerStart()));
    //connect(m_pFrameValidator, SIGNAL(ReceivedStatus()), this, SLOT(sessionTimerStop()) );
    //LOG_INFO << "esjang 0510 CPndrDeviceManager() is start ->"<< LOG_ENDL;

    //} added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
    
}

/**
 *******************************************************************
 *  Set FrameTxRX instace
 * @param      inFrameTxRx , Instance of CPndrFrameTxRx
 * @return     None
 *******************************************************************
 */
void CPndrDeviceManager::SetFrameTxRX(CPndrFrameTxRx* inFrameTxRx)
{
    m_pFrameTxRx =inFrameTxRx;
    connect(m_pFrameTxRx,SIGNAL(ApplicationLaunched(EPndrError)),this,SLOT(onApplicationLaunched(EPndrError)));

    connect(m_pFrameTxRx,SIGNAL(DeviceStatusUpdated(const EDeviceType,bool)),this,SLOT(onDeviceStatusUpdated(const EDeviceType,bool)));

}

/**
 *******************************************************************
 * Set Encoder instace
 * @param      inFrameEncoder , Instance of CPndrFrameEncoder
 * @return     None
 *******************************************************************
 */
void CPndrDeviceManager::SetEncoder(CPndrFrameEncoder* inFrameEncoder)
{
    m_pFrameEncoder = inFrameEncoder;
}

/**
 *******************************************************************
 * Set Validator instace
 * @param      inFrameValidator , Instance of CPndrFrameValidator
 * @return     None
 *******************************************************************
 */
 void CPndrDeviceManager::SetValidator(CPndrFrameValidator* inFrameValidator)
 {
    m_pFrameValidator = inFrameValidator;

    //{ added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
    //LOG_INFO <<"esjang 0510 set validator and connection for session timer"<<LOG_ENDL;
    connect(m_pFrameValidator, SIGNAL(SessionStartAcked()), this, SLOT(sessionTimerStart()));
    //connect(m_pFrameValidator, SIGNAL(SessionTerminateAcked()), this, SLOT(onDisconnect()));// added by esjang 2013.05.15 for certification 
    connect(m_pFrameValidator, SIGNAL(ReceivedStatus()), this, SLOT(sessionTimerStop()) );
    connect(m_pFrameValidator, SIGNAL(MaxFrameDrops(bool)), this, SLOT(onSetMaxFrameDrops(bool)) ); // added by esjang 2013.05.14 for certification #2.3 11 frame drops. Disable session termination.   
    //LOG_INFO <<"esjang 0510 set validator and connection for session timer" << LOG_ENDL;

    //} added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack

 }

/**
 *******************************************************************
 * Destructor:
 *  Setting Device Type to NULL. Calling release to Free instance of
 *  Classes
 *
 * @param      None
 *
 * @return     None
 *
 *******************************************************************
 */

CPndrDeviceManager::~CPndrDeviceManager()
{
    LOG_TRACE << "In CPndrDeviceManager:: ~CPndrDeviceManager() "<< LOG_ENDL;
    m_pFrameEncoder = NULL;
    m_pFrameValidator = NULL;
    m_pFrameTxRx = NULL;
    //{ added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
    if(m_Session.isActive()){
        m_Session.stop();
    }
    //} added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
    
    LOG_TRACE << "Exiting CPndrDeviceManager:: ~CPndrDeviceManager()"<< LOG_ENDL;
}


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
EDeviceType CPndrDeviceManager:: GetConnectedDevice()
{
    LOG_INFO << "CPndrDeviceManager:: GetConnectedDevice() Device is  "
             << m_DeviceType << LOG_ENDL;
    return m_DeviceType;
}

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
EPndrError CPndrDeviceManager::ConnectDevice(const EDeviceType inDeviceType)
{
    //LOG_TRACE << "In CPndrDeviceManager:: ConnectDevice() "<< LOG_ENDL;
    LOG_HIGH << "DeviceManager::ConnectDevice inDeviceType: " << inDeviceType << LOG_ENDL;

    m_DeviceType = inDeviceType;
    EPndrError rtnErrorCode = E_SUCCESS; //m_pFrameTxRx->Init();
  //  if(E_SUCCESS == rtnErrorCode)
    {
        rtnErrorCode = m_pFrameTxRx->SetTransport(m_DeviceType);

        if(rtnErrorCode == E_SUCCESS)
        {
            m_ConnectTimer.start(3000); // 3 sec timer to get device connect successfully , Recovery manager
        }
    }
    //LOG_TRACE << "Exiting CPndrDeviceManager:: ConnectDevice()"<< LOG_ENDL;

    return rtnErrorCode;

}

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
 *  @param[in]   inStationArtDimension size of station art or genre station art image
 * @return     None
 *
 *******************************************************************
 */
void  CPndrDeviceManager::ConnectPandora(const quint16 inAlbumArtDimension,
                                         const EImageType inImgType,
                                         const quint16 inStationArtDimension)

{
    if(m_ConnectTimer.isActive())m_ConnectTimer.stop();

   // LOG_TRACE << "In CPndrDeviceManager:: ConnectPandora() "<< LOG_ENDL;

    LOG_INFO << " DeviceManager ConnectPandora param inDeviceType = "
             << m_DeviceType << "inAlbumArtDimension=" << inAlbumArtDimension
             << "inImgType = "<< inImgType << LOG_ENDL;

    m_nAlbumArtDimension = inAlbumArtDimension;
    m_nStationArtDimension = inStationArtDimension;
    m_ImageType = inImgType;

    if(DEVICE_TYPE_SMARTPHONE == m_DeviceType)
    {
    /** *if pauseOnConnect value of true, in which case the Mobile
      * App will pause audio playback during its session initialization.
      */

        LOG_HIGH << "Device is Smartphone. Hence Connect directly" <<LOG_ENDL;
          
        m_pFrameEncoder->EncodeAndSend(PNDR_SESSION_START,PNDR_API_VERSION_CURRENT,
                                   c_ACCESSORY_ID, inAlbumArtDimension,
                                   inImgType,c_FLAGS_BITMASK,inStationArtDimension, TRUE);
        
    }
    else
    {
        LOG_HIGH << "Device is Ipod. Hence Request app Launch." <<LOG_ENDL;
        m_pFrameTxRx->LaunchApplication(c_BundleAppId);

        m_Timer.start(7500);// 5sec , No where mentioned in spec , Its just //modifieded esjang
        //assumption.

    }

    //LOG_TRACE << "Exiting CPndrDeviceManager:: ConnectPandora()"<< LOG_ENDL;
}

/**
 *******************************************************************
 * Request to disconnect/terminate the Pandora Session
 *
 * @param[in]    None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrDeviceManager::Disconnect()
{
    LOG_TRACE << "In CPndrDeviceManager:: Disconnect() "<< LOG_ENDL;

    //m_DeviceType = DEVICE_TYPE_NONE; /** Clear device connected to Pandora Application*/
    //m_pFrameTxRx->SetTransport(m_DeviceType);

    //Physical Bluetooth SPP/IAP connection is valid
    //Pandora session is terminated
   // m_pFrameEncoder->EncodeAndSend(PNDR_SESSION_TERMINATE);

    IsMaxFrameDrops = FALSE;

    //LOG_TRACE << "Exiting CPndrDeviceManager:: Disconnect()"<< LOG_ENDL;
}

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
void CPndrDeviceManager::Disconnected()
{
    LOG_TRACE << "CPndrDeviceManager:: Disconnected() "<< LOG_ENDL;
    Disconnect();
    //LOG_TRACE << "Exiting CPndrDeviceManager:: Disconnected()"<< LOG_ENDL;
}



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
void CPndrDeviceManager::SessionTerminate()
{
    LOG_TRACE << "In CPndrDeviceManager:: SessionTerminate() "<< LOG_ENDL;
    if(IsMaxFrameDrops == FALSE)
    {
        LOG_TRACE << "In CPndrDeviceManager:: SessionTerminate(): Send session termination "<< LOG_ENDL;
        //sleep(10);//esjang test 
        
        m_pFrameEncoder->EncodeAndSend(PNDR_SESSION_TERMINATE);
        //m_pFrameEncoder->EncodeAndSend(PNDR_GET_STATION_ACTIVE);

    }
    else
    {
        LOG_TRACE << "In CPndrDeviceManager:: SessionTerminate(): Max Frame drops is TRUE, No session termination "<< LOG_ENDL;
        IsMaxFrameDrops = FALSE;
        LOG_TRACE << "In CPndrDeviceManager:: SessionTerminate(): Set Max Frame drops to FALSE"<< LOG_ENDL;        
    }

    LOG_TRACE << "Exiting CPndrDeviceManager:: SessionTerminate()"<< LOG_ENDL;
}

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
void CPndrDeviceManager::GetPandoraStatus()
{
    m_pFrameEncoder->EncodeAndSend(PNDR_GET_STATUS);
}

/**
 *******************************************************************
 * Signal for informing that Pandora app is launched, if was not
 * launched already on ipod
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrDeviceManager::onApplicationLaunched(EPndrError status)
{
    //LOG_TRACE << "In CPndrDeviceManager:: onApplicationLaunched() "<< LOG_ENDL;
    //LOG_INFO << "esjang 0510 init session count"<< LOG_ENDL;
    m_nSessionCount = 0; // added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack

    if(m_ConnectTimer.isActive())m_ConnectTimer.stop();

    if(m_Timer.isActive()){
        disconnect(&m_Timer,0,this,0);
        m_Timer.stop();
    }
    if(E_SUCCESS == status)
    {
        LOG_HIGH << "Pandora is launched on Ipod. Hence Connect Now"<< LOG_ENDL;
        /*
        m_pFrameEncoder->EncodeAndSend(PNDR_SESSION_START,PNDR_API_VERSION_CURRENT,
                                       c_ACCESSORY_ID, m_nAlbumArtDimension,
                                       m_ImageType,PNDR_SESSION_START,m_nStationArtDimension,false);
        */
        // modification 
        m_pFrameEncoder->EncodeAndSend(PNDR_SESSION_START,PNDR_API_VERSION_CURRENT,
                                       c_ACCESSORY_ID, m_nAlbumArtDimension,
                                       m_ImageType,c_FLAGS_BITMASK,m_nStationArtDimension /*, false*/);

      //Single shot timer should not start , which we dnt have control to stop.

      //QTimer::singleShot(TIMEOUT_FOR_SESSION_START, this, SLOT(sendSessionStartAfterDelay()));
    }
    else
    {
        LOG_HIGH << "Pandora is not lunched status : " << status<< LOG_ENDL;
        emit ApplicationLaunched(status);

    }
    //LOG_TRACE << "Exiting CPndrDeviceManager:: onApplicationLaunched()"<< LOG_ENDL;
}

/**
 *******************************************************************
 * its a slot that send the SESSION START message to pandora.
 * This function can be used in Timer::singleShot() api to start session
 * after some delay
 * @return     None
 *
 *******************************************************************
 */
void CPndrDeviceManager::sendSessionStartAfterDelay()
{
    //LOG_INFO << "esjang 0510 CPndrDeviceManager::sendSessionStartAfterDelay: <-"<< LOG_ENDL;

    if(m_Session.isActive()){
        m_Session.stop();
       // LOG_INFO << "esjang 0510 OnSessionTimerStop mPandora->State() stop timer ==============>>>>>>>>>>>>>>>>>>>>>>>"<< LOG_ENDL;
    }

    if(m_nSessionCount < 3)
    {
        
        LOG_CRITICAL<<"************sendSessionStartAfterDelay******* retry count :  "<<m_nSessionCount <<LOG_ENDL;
        //LOG_INFO << "esjang 0510 Encode and send Session Start Again !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: session count: <-"<< m_nSessionCount << LOG_ENDL;
        m_nSessionCount++;
        //LOG_INFO << "esjang 0510 increasing session count: "<< m_nSessionCount << LOG_ENDL;

        m_pFrameEncoder->EncodeAndSend(PNDR_SESSION_START,PNDR_API_VERSION_CURRENT,
                                               c_ACCESSORY_ID, m_nAlbumArtDimension,
                                               m_ImageType,c_FLAGS_BITMASK,m_nStationArtDimension,true);//modified by esjang 2013.05.09 for changing sequece num
        //LOG_INFO << "esjang 0510 Encode and send Session Start Again !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: ->"<< LOG_ENDL;
    }
    //{ added by cheolhwan 2014-02-27. To prevent displaying connecting view indefinitely.
    else
    {
        LOG_CRITICAL << "************sendSessionStartAfterDelay******* retry count[MAX] : " << m_nSessionCount << LOG_ENDL;
        emit ApplicationLaunched(E_NETWORK_FAILED);
    }
    //}  added by cheolhwan 2014-02-27. To prevent displaying connecting view indefinitely.
   // LOG_INFO << "esjang 0510 CPndrDeviceManager::sendSessionStartAfterDelay: ->"<< LOG_ENDL;
}

void CPndrDeviceManager::sessionTimerStart()
{
    LOG_HIGH<< "******session start acked****"<< LOG_ENDL;

    if(m_Session.isActive()){
        m_Session.stop();
        //LOG_INFO << "esjang 0510 OnSessionTimerStop mPandora->State() stop timer ==============>>>>>>>>>>>>>>>>>>>>>>>"<< LOG_ENDL;
    }
    m_Session.start(5000); //

    //LOG_INFO << "esjang 0510 OnSessionTimerStop mPandora->State() start timer <<<<<<<<<<<<<<<<<<<<<=============="<< LOG_ENDL;
}
//{ added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
void CPndrDeviceManager::sessionTimerStop()
{
    //LOG_INFO << "esjang 0510 OnSessionTimerStop mPandora->State() stop timer <-"<< LOG_ENDL;

    if(m_Session.isActive())
    {
        LOG_HIGH<< "******session started ****"<< LOG_ENDL;
        m_Session.stop();
       // LOG_INFO << "esjang 0510 OnSessionTimerStop mPandora->State() stop timer ==============>>>>>>>>>>>>>>>>>>>>>>>"<< LOG_ENDL;
    }
    //LOG_INFO << "esjang 0510 OnSessionTimerStop mPandora->State() stop timer ->"<< LOG_ENDL;
}
//} added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack

/**
 *******************************************************************
 * This function will be called from timeout signal of app launch.
 * @return     None
 *
 *******************************************************************
 */
void CPndrDeviceManager::onTimeout()
{
    LOG_CRITICAL<<"*******Could not launch app on iOS Device******* "<<LOG_ENDL;
    emit ApplicationLaunched(E_CHECK_PNDR_APP_ON_DEVICE);
}

//{ added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
//void CPndrDeviceManager::onSessionStartAcked()
//{
//    LOG_INFO <<"esjang 0510 CPndrDeviceManager::onSessionStartAcked <-"<<LOG_ENDL;

//    if(m_Session.isActive()){
//        m_Session.stop();
//        LOG_INFO << "esjang 0510 OnSessionTimerStop mPandora->State() stop timer ==============>>>>>>>>>>>>>>>>>>>>>>>"<< LOG_ENDL;
//    }
//    else
//    {
//        LOG_INFO << "esjang 0510 CAppPandoraBase::OnSessionTimer is not active"<< LOG_ENDL;

//    }
//    m_Session.start(5000);
//    LOG_INFO << "esjang 0510 OnSessionTimerStop mPandora->State() start timer <<<<<<<<<<<<<<<<<<<<<<<<<<<=============="<< LOG_ENDL;

//    LOG_INFO<<"esjang 0510 CPndrDeviceManager::onSessionStartAcked ->"<<LOG_ENDL;
    
//}
//} added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack

void CPndrDeviceManager::connectionTimedOut()
{
    LOG_CRITICAL<<"CPndrDeviceManager::connectionTimedOut() " << LOG_ENDL;

    if(m_nCounter < 3)
    {
        m_nCounter++;
        m_pFrameTxRx->Disconnect();
        this->ConnectDevice(m_DeviceType);
    }
    else{
        m_nCounter = 0;
        //m_pFrameTxRx->Disconnect(); // added by jyjeon 2014-03-27 for iPod deInit  //removed by cheolhwan 2014-03-29. Disconnectting the device should move from MiddleWare to PandoraApp.
    }
}

void CPndrDeviceManager::onDeviceStatusUpdated(const EDeviceType /*inType*/,bool inStatus)
{
    LOG_CRITICAL<<"CPndrDeviceManager::OnDeviceStatusUpdated : " << inStatus << LOG_ENDL;
    m_nCounter = 0;
    if(m_ConnectTimer.isActive()){
        m_ConnectTimer.stop();
    }
}



//End Of File
