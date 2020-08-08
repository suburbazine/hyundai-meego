/*
 ******************************************************************
 *        COPYRIGHT  2011, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :   Pandora Middleware state handler                *
 * PROGRAMMER :   Snigdha Satapathy                               *
 * DATE       :   07/12/2011                                      *
 * VERSION    :   0.1                                             *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module is responsible for handling the state machine for
 *  Pandora middleware
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 * 07/12/2011   0.1 Snigdha Satapathy   cpp file created
 * 01/03/2013   0.2 Radhakrushna Mohanty modifified for 2 Ipod
                    support moved single ton pattern from class
                    level to instance owner level .
 ******************************************************************
 */

#include <QMetaType>

#include "DHAVN_PndrStateHandler.h"
#include "DHAVN_PndrDeviceManager.h"
#include "DHAVN_PndrStationController.h"
#include "DHAVN_PndrTrackController.h"
#include "DHAVN_PndrTrackProperties.h"
#include "DHAVN_PndrFrameEncoder.h"
#include "DHAVN_PndrFrameDecoder.h"
#include "DHAVN_PndrFrameTxRx.h"
#include "DHAVN_PndrFrameEncoder.h"
#include "DHAVN_PndrFrameValidator.h"
#include "DHAVN_PndrCache.h"
#include <stdlib.h>
#include <string.h>
#include <QDir>

#define LOG_TAG_STATEHANDLER "StateHandler"

//TO DO: check this timeout value from Doc
const int cTimeoutInterval = 30000;
const QString c_BundleAppId = "com.pandora";

const char* port1 = "Front: ";
const char* port2 = "Rear: ";

//{ modified by yongkyun.lee 2014-10-07 for : Branding - Middleware
#define BANDIMAG_SEND_SIZE_2 0x200 // android
#define BANDIMAG_SEND_SIZE   0x80  // ipad
#define BANDIMAG_SEND_MAX_SIZE 0x9000
char    inData[BANDIMAG_SEND_SIZE_2+2]={0x00};
//} modified by yongkyun.lee 2014-10-07 

/**
 *******************************************************************
 * Default class constructor .
 *
 * @param None
 *
 *******************************************************************
 */
CPndrStateHandler::CPndrStateHandler():
    m_pDeviceManager(0)
    , m_pStationCtrl(0)
    , m_pTrackCtrl(0)
    , m_pTrackProperties(0)
    , m_pFrameDecoder(0)
    , m_pFrameEncoder(0)
    , m_pFrameValidator(0)
    , m_pFrameTxRx(0)
    , m_pStCache(0)
    , m_nAlbumArtDimension(0) // added by esjang 2012.11.12 for Static Test CID 13138
    , m_nStationArtDimension(0) // added by esjang 2012.11.12 for Static Test CID 13138
    , m_eAlbumArtImgType(IMG_TYPE_NONE) // added by esjang 2012.11.12 for Static Test CID 13138
    , m_pReplyWaitTimer(NULL) // added by esjang 2012.11.12 for Static Test CID 13138
    , m_eStatusOnSessionStart(STATE_NONE)
    , m_bFetchStationArt(false)
    , m_PreFetchingStations(false) //added by cheolhwan 2014-04-17. To fix the unlimited receiving station. (ITS 238629)
    , m_BandingImageTimer( new QTimer(this))// modified by yongkyun.lee 2014-08-25 for : Branding Image
    , m_LastStationToken(0) // added by cheolhwan 2014-04-25. ITS 234067. To ignore a duplicate of the message with an incorrect sequence number.
{
    LOG_INIT_V2("Pandora");
    //{ modified by yongkyun.lee 2014-10-07 for : Branding - Middleware
    m_BrandingImageSegIndex = 0;
    m_Brandingfile = NULL;
    m_BrandingBuf  = NULL;
    m_BrandingfileSize = 0;
    //} modified by yongkyun.lee 2014-10-07 
    
    m_Port = (char*) malloc(3);            
    memcpy(m_Port , "::" , sizeof("::")); // modified by esjang 2013.05.22 for static test # 20895, 20896, 20898

    InitializeStates();
     //{ modified by yongkyun.lee 2014-10-07 for : Branding - Middleware
     if(m_BandingImageTimer == NULL )
     {
         m_BandingImageTimer = new QTimer(this);
     }    
     m_BandingImageTimer->setSingleShot( true );
     m_BandingImageTimer->setInterval( 10 ); //  5 sec
     connect( m_BandingImageTimer, SIGNAL(timeout()), this, SLOT(onBrandingImageTimer()) );  
     //} modified by yongkyun.lee 2014-10-07 

}

/**
 *******************************************************************
 * Initialize memory .
 *
 * @param None
 *
 *******************************************************************
 */
EPndrError CPndrStateHandler::Init()
{
    LOG_TRACE<< "CPndrStateHandler::Init:Entry" <<LOG_ENDL;

    EPndrError ret = E_SUCCESS;

    LOG_HIGH<<" m_ePriState : "<< m_ePriState<<LOG_ENDL;
    m_BrandingImageSegIndex = 0;//{ modified by yongkyun.lee 2014-08-25 for : Branding Image

    if(PRI_STATE_NONE == m_ePriState /*&& PRI_STATE_INIT != m_ePriState*/)
    {
        ret = E_INIT_FAILED;
        //Create the reply wait timer
        m_pReplyWaitTimer = new QTimer(this);
        if(m_pReplyWaitTimer == NULL)
        {
            goto InitFailed;
        }
        m_pReplyWaitTimer->setInterval(cTimeoutInterval);


        //m_pFrameTxRx = CPndrFrameTxRx::GetInstance();
        if(m_pFrameTxRx == NULL){
        m_pFrameTxRx = new CPndrFrameTxRx();
        }

        if(m_pFrameTxRx == NULL)
        {
            goto InitFailed;
        }

        m_pFrameDecoder = new CPndrFrameDecoder();//CPndrFrameDecoder::GetInstance();
        if(m_pFrameDecoder == NULL)
        {
            goto InitFailed;
        }

        m_pFrameValidator = new CPndrFrameValidator() ; //CPndrFrameValidator::GetInstance();
        if(m_pFrameValidator == NULL)
        {
            goto InitFailed;
        }
        m_pFrameValidator->SetFrameTxRX(m_pFrameTxRx);
        m_pFrameValidator->SetDecoder(m_pFrameDecoder);


        m_pFrameEncoder = new CPndrFrameEncoder() ; //CPndrFrameEncoder::GetInstance();
        if(m_pFrameEncoder == NULL)
        {
            goto InitFailed;
        }
        m_pFrameEncoder->SetValidator(m_pFrameValidator);

        m_pDeviceManager = new CPndrDeviceManager();
        if(m_pDeviceManager == NULL)
        {
            goto InitFailed;
        }
        m_pDeviceManager->SetFrameTxRX(m_pFrameTxRx);
        m_pDeviceManager->SetEncoder(m_pFrameEncoder);
        m_pDeviceManager->SetValidator(m_pFrameValidator);

        m_pStationCtrl = new CPndrStationController();
        if(m_pStationCtrl == NULL)
        {
            goto InitFailed;
        }
        m_pStationCtrl->SetFrameTxRX(m_pFrameTxRx);
        m_pStationCtrl->SetEncoder(m_pFrameEncoder);
        m_pStationCtrl->SetDecoder(m_pFrameDecoder);

        m_pTrackCtrl = new CPndrTrackController();
        if(m_pTrackCtrl == NULL)
        {
            goto InitFailed;
        }
        m_pTrackCtrl->SetEncoder(m_pFrameEncoder);

        m_pTrackProperties = new CPndrTrackProperties();
        if(m_pTrackProperties == NULL)
        {
            goto InitFailed;
        }
        m_pTrackProperties->SetFrameTxRX(m_pFrameTxRx);
        m_pTrackProperties->SetEncoder(m_pFrameEncoder);
        m_pTrackProperties->SetDecoder(m_pFrameDecoder);

        m_pStCache = new CPndrCache();
        if(m_pStCache == NULL)
        {
            goto InitFailed;
        }

        connect(m_pReplyWaitTimer,SIGNAL(timeout()),this,SLOT(OnTimeout()));

        connect(m_pFrameTxRx,SIGNAL(DeviceStatusUpdated(EDeviceType , bool)),
                this,SLOT(OnDeviceStatusUpdated(EDeviceType , bool)));

        connect(m_pTrackProperties,SIGNAL(RequestCompleted(const EPndrEvent,const QString&)),
                this,SLOT(OnRequestCompletion(const EPndrEvent,const QString&)));

        connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent , const quint8 )),
                this,SLOT(OnRequestCompletion(const EPndrEvent , const quint8 )));

        connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent , const quint16 )),
                this,SLOT(OnRequestCompletion(const EPndrEvent , const quint16 )));

        connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent , const QString& )),
                this,SLOT(OnRequestCompletion(const EPndrEvent , const QString& )));

        connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent , const quint32,const QList<TSSearchInfo>& )),
               this,SLOT(OnRequestCompletion(const EPndrEvent , const quint32,const QList<TSSearchInfo>& )));

        connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent , const quint32 ,const quint8 )),
                this,SLOT(OnRequestCompletion(const EPndrEvent , const quint32 , const quint8 )));

        connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent , const quint32 )),
                this,SLOT(OnRequestCompletion(const EPndrEvent , const quint32 )));

        connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent , const TSTrackInfo& )),
                this,SLOT(OnRequestCompletion(const EPndrEvent , const TSTrackInfo& )));

        connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent , const quint8 ,const QList<quint32>& )),
                this,SLOT(OnRequestCompletion(const EPndrEvent , const quint8 , const QList<quint32>& )));

        connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent , const quint32 ,const QList<quint32>& )),
                this,SLOT(OnRequestCompletion(const EPndrEvent , const quint32 , const QList<quint32>& )));

        connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent , const QList<TSStationInfo>& )),
                this,SLOT(OnRequestCompletion(const EPndrEvent , const QList<TSStationInfo>& )));

        connect(m_pFrameDecoder,SIGNAL(ErrorReceived(EPndrError)),this,SLOT(OnErrorReceived(EPndrError)));

        connect(m_pDeviceManager,SIGNAL(ApplicationLaunched(EPndrError)), this,SLOT(OnErrorReceived(EPndrError)));

        qRegisterMetaType<EPndrEvent>("EPndrEvent");
        connect(m_pStationCtrl,SIGNAL(ReplyReceived(const EPndrEvent , const quint8 )),
                this,SLOT(OnRequestCompletion(const EPndrEvent , const quint8 )),Qt::QueuedConnection);

        //{ modified by yongkyun.lee 2014-08-25 for : Branding Image
        connect(m_pStationCtrl,SIGNAL(getBrandingImage( )),
                this,SLOT(OnRequestBrandingImage(  )),Qt::QueuedConnection);
        connect(m_pFrameValidator,SIGNAL(NextBrandingImage()), this, SLOT(OnRequestBrandingImage()),Qt::QueuedConnection);
        //} modified by yongkyun.lee 2014-08-14 
        connect(m_pStationCtrl,SIGNAL(StationArtReceived(quint32 )),this,SIGNAL(StationArtReceived(quint32)));
        connect(m_pFrameValidator,SIGNAL(SessionTerminateAcked()), this, SIGNAL(SessionTerminateAcked()));// added by esjang 2013.05.15

        connect(m_pFrameTxRx, SIGNAL(PndrSessionClosed()), this, SLOT(onPndrSessionClosed())); //added by esjang 2013.06.27 for ISV #71393, ITS #167008

        connect(m_pFrameTxRx, SIGNAL(AudioSamplingChanged(int)), this, SIGNAL(AudioSamplingChanged(int)));
        ret = E_SUCCESS;
        m_ePriState =  PRI_STATE_INIT ;
        m_bFetchStationArt = false;

        InitFailed:
        if(E_INIT_FAILED == ret)
        {
            LOG_CRITICAL << "CRITICAL:CPndrStateHandler::Init Failed" <<LOG_ENDL;
            DeInit();
        }
    }
    LOG_TRACE<< "CPndrStateHandler::Init:Exit => ret:" << ret <<LOG_ENDL;

    return ret;

}

/**
 *******************************************************************
 * destructor
 *
 *******************************************************************
 */
CPndrStateHandler::~CPndrStateHandler()
{
    LOG_TRACE<< "CPndrStateHandler::~CPndrStateHandler:Entry" <<LOG_ENDL;
    DeInit();
    if(m_Port)
    {
        free(m_Port);
        m_Port = NULL;
    }
    //{ modified by yongkyun.lee 2014-10-07 for : Branding - Middleware
    if(m_BrandingBuf)
        delete[] m_BrandingBuf;
    m_BrandingBuf=NULL;
    if(m_Brandingfile)
        delete m_Brandingfile;
    m_Brandingfile=NULL;
    //} modified by yongkyun.lee 2014-10-07 
    LOG_TRACE<< "CPndrStateHandler::~CPndrStateHandler:Exit" <<LOG_ENDL;
}

/**
 *******************************************************************
 * Release all memory
 *
 *******************************************************************
 */
void CPndrStateHandler::DeInit()
{
    LOG_TRACE<< "CPndrStateHandler::DeInit:Entry" <<LOG_ENDL;    
    //if(PRI_STATE_NONE != m_ePriState)
    {
        if(m_pReplyWaitTimer)
        {
            LOG_INFO<< "CPndrStateHandler::DeInit:m_pReplyWaitTimer stopped" <<LOG_ENDL;
            m_pReplyWaitTimer->stop();
            disconnect(m_pReplyWaitTimer,SIGNAL(timeout()),this,SLOT(OnTimeout()));
            delete m_pReplyWaitTimer;
            m_pReplyWaitTimer = NULL;
        }

        if(m_pDeviceManager)
        {
            delete m_pDeviceManager;
            m_pDeviceManager =  NULL;
        }

        if(m_pStationCtrl)
        {
           m_pStationCtrl->ClearStationArtFolder();
           disconnect(m_pStationCtrl,SIGNAL(ReplyReceived(const EPndrEvent , const quint8 )),
                    this,SLOT(OnRequestCompletion(const EPndrEvent , const quint8 )));

           disconnect(m_pStationCtrl,SIGNAL(StationArtReceived(quint32)),this,SIGNAL(StationArtReceived(quint32)));

            delete m_pStationCtrl;
            m_pStationCtrl =  NULL;
        }

        if(m_pTrackCtrl)
        {
            delete m_pTrackCtrl ;
            m_pTrackCtrl = NULL;
        }

        if(m_pTrackProperties)
        {
            delete m_pTrackProperties;
            m_pTrackProperties =  NULL;
        }

        if(m_pFrameDecoder)
        {
            delete m_pFrameDecoder;
            m_pFrameDecoder = NULL;
        }

        if(m_pFrameEncoder)
        {
            delete m_pFrameEncoder;
            m_pFrameEncoder = NULL;
        }

        if(m_pFrameValidator)
        {
            delete m_pFrameValidator;
            m_pFrameValidator = NULL;
        }

        if(m_pFrameTxRx)
        {
            delete m_pFrameTxRx;
            m_pFrameTxRx = NULL;
        }
        if(m_pStCache)
        {
            delete m_pStCache;
            m_pStCache = NULL;
        }
    }

    InitializeStates();

    LOG_TRACE<< "CPndrStateHandler::DeInit:Exit" <<LOG_ENDL;

}


/**
 *******************************************************************
 * Initialize all states to default value,
 *
 * @param      None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStateHandler::InitializeStates()
{
    LOG_TRACE<< "CPndrStateHandler::InitializeStates:Entry" <<LOG_ENDL;

    m_ePriState = PRI_STATE_NONE;
    m_eSecState = SEC_STATE_NONE;
    m_eSecPreviousState = SEC_STATE_NONE;
    m_eTrackInfoState = TRACK_INFO_NONE ;
    m_bFetchStationArt = false;
    LOG_TRACE<< "CPndrStateHandler::InitializeStates:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Check if requested operation is possible in current state or not
 * To be called by Interface
 *
 * @param None
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If request can be made in current state
 *                E_INVALID_STATE: The requested operation
 *                  not possible in current state
 *
 *******************************************************************
 */
EPndrError CPndrStateHandler::CanProceedWithRequest(const ERequestType inReqType)
{
    LOG_TRACE<< "CPndrStateHandler::CanProceedWithRequest:Entry" <<LOG_ENDL;

    LOG_INFO << LOG_TAG_STATEHANDLER << " Request type :"<<inReqType
              << "m_ePriState:["<<m_ePriState <<"] m_eSecState:["<<m_eSecState<<"]"
              << LOG_ENDL;

    EPndrError retStatus = E_INVALID_STATE;

    switch(inReqType)
    {
        case REQ_TYPE_CONNECT:
        {
            if(PRI_STATE_INIT == m_ePriState || PRI_STATE_DEVICE_CONNECTED  == m_ePriState ) //{ modified by yongkyun.lee 2014-09-18 for : ITS 248606
            {
                retStatus = E_SUCCESS ;
            }
            break;
        }

        //{ modified by yongkyun.lee 2014-08-25 for : Branding Image 
        case REQ_TYPE_BRANDING_IMAGE:
        {
            retStatus = E_SUCCESS ;
            break;
        }
        case REQ_TYPE_BRANDING_SEGMENT:
        {
            retStatus = E_SUCCESS ;
            break;
        }            
        //} modified by yongkyun.lee 2014-08-04 
        
        case REQ_TYPE_TRACK_INFO:
        case REQ_TYPE_STATION_SELECT:
        case REQ_TYPE_SEARCH_SELECT:
        case REQ_TYPE_EXPLAIN:
        case REQ_TYPE_STATION_COUNT:
        {
//            if(SEC_STATE_NONE == m_eSecState &&
//                    (PRI_STATE_NONE != m_ePriState &&
//                     PRI_STATE_INIT != m_ePriState &&
//                     PRI_STATE_DEVICE_CONNECTED != m_ePriState))
        if(m_ePriState >= PRI_STATE_PANDORA_CONNECTED)
            {
                retStatus = E_SUCCESS ;
            }
            break;
        }
        case REQ_TYPE_SEARCH:
        case REQ_TYPE_SEARCH_AUTO_COMPLETE:
        {
            if((SEC_STATE_NONE == m_eSecState ||
                SEC_STATE_AUTO_COMPLETE_SEARCH_REQUESTED == m_eSecState) &&
                    (PRI_STATE_NONE != m_ePriState &&
                     PRI_STATE_INIT != m_ePriState &&
                     PRI_STATE_DEVICE_CONNECTED != m_ePriState))
            {
                retStatus = E_SUCCESS ;
            }
            break;
        }

        case REQ_TYPE_STATION_LIST:
        case REQ_TYPE_SORT:
        {
//           if(/*(SEC_STATE_STATION_LIST_REQUESTED == m_eSecState ||
//               SEC_STATE_NONE== m_eSecState) &&*/
//               PRI_STATE_NONE != m_ePriState &&
//                PRI_STATE_INIT != m_ePriState &&
//                PRI_STATE_DEVICE_CONNECTED != m_ePriState)
//            {
//                retStatus = E_SUCCESS ;
//            }

            if( m_ePriState >= PRI_STATE_PANDORA_CONNECTED )
            {
                retStatus = E_SUCCESS ;
            }
            break;
        }

        case REQ_TYPE_PLAY:
        {
            LOG_HIGH<< "CanProceedWithRequest [REQ_TYPE_PLAY] m_ePriState:"<< m_ePriState <<LOG_ENDL;
            if(PRI_STATE_PANDORA_CONNECTED == m_ePriState ||
                    PRI_STATE_PAUSED == m_ePriState || PRI_STATE_PAUSE_REQUESTED == m_ePriState
                    ||PRI_STATE_PLAY_REQUESTED == m_ePriState) // modified by esjang for ITS # 195131 ,Send play request after phone call end.(without bt connection)
            {
                retStatus = E_SUCCESS ;
            }
            break;
        }
        case REQ_TYPE_PAUSE:
        {
            LOG_HIGH<< "CanProceedWithRequest [REQ_TYPE_PAUSE] m_ePriState:"<< m_ePriState <<LOG_ENDL;
            if(PRI_STATE_PANDORA_CONNECTED == m_ePriState ||
                    PRI_STATE_PLAYING == m_ePriState ||  PRI_STATE_PLAY_REQUESTED == m_ePriState)
            {
                retStatus = E_SUCCESS ;
            }
            break;
        }
        case REQ_TYPE_SKIP:
        case REQ_TYPE_RATE:
        case REQ_TYPE_BOOKMARK_SONG:
        case REQ_TYPE_BOOKMARK_ARTIST:
        {
            if(m_ePriState >= PRI_STATE_PANDORA_CONNECTED)
            {
                retStatus = E_SUCCESS ;
            }
            break;
        }
        case REQ_TYPE_DISCONNECT:
        {
            if(PRI_STATE_NONE != m_ePriState)
            {
                retStatus = E_SUCCESS ;
            }
            break;
        }
        default:
        {
            LOG_CRITICAL << LOG_TAG_STATEHANDLER <<"CRITICAL:Req Not handled "<<inReqType<<LOG_ENDL;
            break;
        }
    }

    if(E_SUCCESS == retStatus)
    {
        if(m_pReplyWaitTimer == NULL) 
        {
            LOG_TRACE<< "CPndrStateHandler::CanProceedWithRequest:m_pReplyWaitTimer is NULL" <<LOG_ENDL;
            return retStatus;
        }
        //Start the 30 sec timer
        //LOG_INFO << LOG_TAG_STATEHANDLER <<"[m_pReplyWaitTimer->start] Request Timer started "<<inReqType<<LOG_ENDL;
        if(!m_pReplyWaitTimer->isActive())
        m_pReplyWaitTimer->start();
    }
    else
    {
        LOG_CRITICAL << LOG_TAG_STATEHANDLER<< ":CanProceedWithRequest Failed for req "<<inReqType<<LOG_ENDL;
        LOG_CRITICAL <<LOG_TAG_STATEHANDLER<< " Failed reason m_ePriState:"<<m_ePriState<<" , m_eSecState:"
                     << m_eSecState <<LOG_ENDL;
    }

   // LOG_INFO << LOG_TAG_STATEHANDLER <<"Return status : "<<retStatus<<LOG_ENDL;

    LOG_TRACE<< "CPndrStateHandler::CanProceedWithRequest:Exit" <<LOG_ENDL;

    return retStatus;
}

/**
 *******************************************************************
 * This function will connect to device and initiate
 *  the pandora connection
 *
 *    TODO: Currently its not possible to get the device ID/name
 *            by which it can be identified that which device to
 *            connect if there are more than one device is
 *            connected through usb
 *
 * @param[in]    inDeviceType: This can be iPhone or Smart Phone
 *                    By default if iPhone is EDeviceType then USB
 *                    will be tried for connection & if smart device
 *                    is used as EDeviceType then BT will be tried
 *                    for connection.
 *                  inAlbumArtDimension: The expected size of album art for
 *                      each music
 *                  inAlbumArtImgType: The expected image type of album
 *                      art for each music, if its value is IMG_TYPE_NONE,
 *                      then default value as IMG_TYPE_JPEG will be used.
 *                  inStationArtDimension size of station art or genre station
 *                       art image
 *
 * @return     EPndrError
 *
 *******************************************************************
 */
EPndrError CPndrStateHandler::Connect(const EDeviceType inDeviceType,
                               const quint16 inAlbumArtDimension ,
                               const EImageType inAlbumArtImgType,
                               const quint16 inStationArtDimension)
{

    if(m_Port){
        free(m_Port);
        m_Port = NULL;
    }
    m_Port = (char*) malloc(5);  // Modified by es.jang 2014-02-12. To prevent memory overflow.
    if(inDeviceType == DEVICE_TYPE_IPHONE_USB_FRONT)
    {
        memcpy(m_Port , ":F:" , sizeof(":F:"));// added by esjang 2013.05.23 for static test # 20897
    }
    else if (inDeviceType == DEVICE_TYPE_IPHONE_USB_REAR){
        memcpy(m_Port , ":R:" , sizeof(":R:"));// added by esjang 2013.05.23 for static test # 20897
    }
    else if (inDeviceType == DEVICE_TYPE_SMARTPHONE){
        memcpy(m_Port , ":BT:" , sizeof(":BT:"));
    }

    LOG_TRACE<<m_Port<< "CPndrStateHandler::Connect:Entry" <<LOG_ENDL;

    //Save the dimension and Image type
    //to be used while connecting Pandora
    m_nAlbumArtDimension = inAlbumArtDimension;
    m_nStationArtDimension = inStationArtDimension;
    m_eAlbumArtImgType = inAlbumArtImgType;
    EPndrError err = m_pDeviceManager->ConnectDevice(inDeviceType);
    if(E_SUCCESS != err)
    {
        LOG_CRITICAL <<m_Port<< "CRITICAL:Connect Failed with err " <<err <<LOG_ENDL;
        emit Result(REQ_TYPE_CONNECT , err);
        m_ePriState = PRI_STATE_INIT ;
    }

    LOG_TRACE<<m_Port<< "CPndrStateHandler::Connect:Exit" <<LOG_ENDL;
    return err;

}

/**
 *******************************************************************
 * Disconnect any existing connection
 *
 * @param None
 *
 * @return  None
 *
 *******************************************************************
 */
void CPndrStateHandler::Disconnect()
{
    LOG_HIGH<<m_Port<<"CPndrStateHandler::Disconnect:Entry => m_ePriState["<<m_ePriState<<"]" <<LOG_ENDL;
    //LOG_CRITICAL<<"CRITICAL:Device got disconnected"<<LOG_ENDL;
    if(PRI_STATE_NONE != m_ePriState )
    {
        m_pDeviceManager->Disconnect();
        InitializeStates();
        m_ePriState = PRI_STATE_INIT; //Init State is maintained since all physical connections are valid
    }
    LOG_TRACE<<m_Port<<"CPndrStateHandler::Disconnect:Exit" <<LOG_ENDL;

}

//{ added by esjang 2013.05.15 for certification issue, sessionterminate.
/**
 *******************************************************************
 * SessionTerminate any session
 *
 * @param None
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is not connected
 *
 * @see:    Result
 *******************************************************************
 */
void CPndrStateHandler::SessionTerminate()
{
    LOG_TRACE<<m_Port<<"CPndrStateHandler::SessionTerminate() <-"<<LOG_ENDL;
    if(m_pDeviceManager != NULL)
        m_pDeviceManager->SessionTerminate();
    LOG_TRACE<<m_Port<<" CPndrStateHandler::SessionTerminate() ->" <<LOG_ENDL;

}
//} added by esjang 2013.05.15 for certification issue, sessionterminate.
/**
 *******************************************************************
 * This function will provide the Information
 * about current track being played/paused
 *
 * @param None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStateHandler::RequestTrackInfo()
{
    LOG_TRACE<<m_Port<< "CPndrStateHandler::RequestTrackInfo:Entry" <<LOG_ENDL;
    m_eSecState = SEC_STATE_TRACK_INFO_REQUESTED;
    m_eTrackInfoState = TRACK_INFO_NONE;

    m_pTrackProperties->RequestTrackPartInfo();


    if(m_pStationCtrl->IsStInfoAvail(m_sTrackInfo))
    {
        LOG_TRACE<<m_Port<< "m_pStationCtrl->IsStInfoAvail : " << m_sTrackInfo.CurrentStation << LOG_ENDL;
        m_eTrackInfoState= (ETrackInfoState)(m_eTrackInfoState|TRACK_STATION_INFO);
    }
    else
    {
        //Request Active Station Info
        LOG_TRACE<<m_Port<< "Request Active Station Info" <<LOG_ENDL;
        m_pStationCtrl->RequestCurrentStationInfo(m_eAlbumArtImgType);
    }

    LOG_TRACE<<m_Port<< "CPndrStateHandler::RequestTrackInfo:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Request for explanation for currently played/paused song
 *
 * @param None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStateHandler::RequestExplanation()
{
    LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestExplanation:Entry" <<LOG_ENDL;
    m_eSecState = SEC_STATE_TRACK_EXPLAIN_REQUESTED;
    m_pTrackProperties->RequestExplain();
    LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestExplanation:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Request to Play the track. This will be possible ONLY
 * if track was paused before
 *
 * @param None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStateHandler::Play()
{
    LOG_HIGH<<m_Port<< "SH::Play Request" <<LOG_ENDL;
    m_ePriState = PRI_STATE_PLAY_REQUESTED ;
    m_pTrackCtrl->Play();
    //LOG_TRACE<<m_Port<< "CPndrStateHandler::Play:Exit" <<LOG_ENDL;

}


/**
 *******************************************************************
 * Request to Pause the current track being played.
 *
 * @param None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStateHandler::Pause()
{
    LOG_HIGH<<m_Port<< "CPndrStateHandler::Pause:Entry" <<LOG_ENDL;
    m_ePriState = PRI_STATE_PAUSE_REQUESTED ;
    m_pTrackCtrl->Pause();
    //LOG_TRACE<<m_Port<< "CPndrStateHandler::Pause:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Request to Skip the song
 *
 * @param None
 *
 * @return     EPandoraError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is Not connected OR
 *                                    No track is in Play/Pause status
 *
 * @see:    TrackUpdated
 *******************************************************************
 */
void CPndrStateHandler::Skip()
{
    LOG_TRACE<<m_Port<<"CPndrStateHandler::Skip:Entry" <<LOG_ENDL;
    if(m_eSecState != SEC_STATE_SKIP_REQUESTED && m_eSecState != SEC_STATE_TRACK_INFO_REQUESTED)
    {
        m_eSecState = SEC_STATE_SKIP_REQUESTED ;
        m_pTrackCtrl->Skip();
    }
    LOG_TRACE<<m_Port<<"CPndrStateHandler::Skip:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Request to Rate the song
 *
 * @param[in]    inRate    The Rating that user would like to give to
 *                        current track
 *                        1:    ThumbUp
 *                        2:    ThumbDown
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStateHandler::Rate(const quint8 inRate)
{
    LOG_TRACE<<m_Port<<"CPndrStateHandler::Rate:Entry" <<LOG_ENDL;

    if(inRate == 1)
    {
        m_eSecState = SEC_STATE_THUMB_UP_REQUESTED ;
        m_pTrackProperties->RequestRateUp();

    }
    else
    {
        m_eSecState = SEC_STATE_THUMB_DOWN_REQUESTED;
        m_pTrackProperties->RequestRateDown();
    }
    LOG_TRACE<<m_Port<<"CPndrStateHandler::Rate:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Request to Bookmark current Track
 *
 * @param None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStateHandler::BookmarkSong()
{
    LOG_TRACE<<m_Port<<"CPndrStateHandler::BookmarkSong:Entry" <<LOG_ENDL;
    if( m_sTrackInfo.bTrackBookmarked ){
          m_pReplyWaitTimer->stop();
        emit  Result(REQ_TYPE_BOOKMARK_SONG,E_SUCCESS );
    }
    else
    {
        m_eSecState = SEC_STATE_BOOKMARK_SONG_REQUESTED ;
        m_pTrackProperties->RequestBookmarkSong();
    }
    LOG_TRACE<<m_Port<<"CPndrStateHandler::BookmarkSong:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Bookmark Artist of current Track
 *
 * @param None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStateHandler::BookmarkArtist()
{
    LOG_TRACE<<m_Port<<"CPndrStateHandler::BookmarkArtist:Entry" <<LOG_ENDL;
    if( m_sTrackInfo.bArtistBookmarked ){
        m_pReplyWaitTimer->stop();
        emit  Result(REQ_TYPE_BOOKMARK_ARTIST,E_SUCCESS );
    }
    else
    {
        m_eSecState = SEC_STATE_BOOKMARK_ARTIST_REQUESTED ;
        m_pTrackProperties->RequestBookmarkArtist();
    }

    LOG_TRACE<<m_Port<<"CPndrStateHandler::BookmarkArtist:Exit" <<LOG_ENDL;

}


/**
 *******************************************************************
 * Request to set the station sort type.
 *
 * @param[in]    inSortType    What is expected order for station List
 *
 * @return     None
 *
 *******************************************************************
 */


void CPndrStateHandler::RequestStationSort(const ESortType inSortType)
{
    LOG_HIGH<<m_Port<<"StateHandler RequestStationSort " <<LOG_ENDL;
    //To Do: check the states for this request
    m_eSecState = SEC_STATE_STATION_LIST_REQUESTED ;
    m_pStationCtrl->RequestStationSort(inSortType);
    //LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestStationSort:Exit" <<LOG_ENDL;
}

/**
 *******************************************************************
 * Request to get the station sort type.
 *
 * @param[in]    None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStateHandler::RequestStationSortOrder()
{
   // LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestStationSortOrder:Entry" <<LOG_ENDL;
    LOG_HIGH<<m_Port<<"StateHandler RequestStationSortOrder" <<LOG_ENDL;
    m_eSecState = SEC_STATE_STATION_LIST_REQUESTED ;
    m_pStationCtrl->RequestStationSortOrder();
    //LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestStationSortOrder:Exit" <<LOG_ENDL;
}

/**
 *******************************************************************
 * Request to get the station count.
 *
 * @param[in]    None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStateHandler::RequestStationCount()
{
    //LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestStationCount:Entry" <<LOG_ENDL;

    QList<quint32> tokenList;
    ESortType eSortType = m_pStationCtrl->CurrentSortOrder();
    
    LOG_HIGH<<m_Port<<" RequestStationCount() <- eSortType=" << eSortType <<LOG_ENDL;
    if(m_pStCache->StationCount(eSortType , tokenList))
    {
        LOG_HIGH<<m_Port<<" StateHandler , station tokens from cache -> m_eSecState:"<< m_eSecState <<LOG_ENDL;

        if(m_eSecState == SEC_STATE_STATION_LIST_REQUESTED)
        {
            m_eSecState = SEC_STATE_NONE;
        }
        m_pReplyWaitTimer->stop();
        emit StationTokens(tokenList.count(),tokenList);
    }
    else
    {
        LOG_HIGH<<m_Port<<" StateHandler , request station count" <<LOG_ENDL;
        m_eSecState = SEC_STATE_STATION_LIST_REQUESTED ;
        m_pStationCtrl->RequestStationCount();
    }
    //LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestStationCount:Exit" <<LOG_ENDL;
}

/**
 *******************************************************************
 * Request to Get the station list.
 *
 * @param[in]    inStartIndex    Which is the start index for getting
 *                      the station list
 *                  inEndIndex      Which is the end index for getting
 *                      the station list
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATION_REQ_RANGE: Invalid range
 *
 *
 *******************************************************************
 */
EPndrError CPndrStateHandler::RequestStationList(const qint8 inStartIndex,
                                                 const qint8 inEndIndex)
{
   //LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestStationList:Entry" <<LOG_ENDL;

   EPndrError error = E_SUCCESS ;

   QList<TSStationInfo> stInfoList;

   //LOG_CRITICAL<<m_Port<<" SH :: Fetch station art , m_bFetchStationArt : "<< m_bFetchStationArt<<LOG_ENDL;

   if(m_pStCache->AllStationAvailable(m_pStationCtrl->CurrentSortOrder() , stInfoList))
   {
       LOG_HIGH<<m_Port<<" StateHandler All station available "<<LOG_ENDL;
       if(m_eSecState == SEC_STATE_STATION_LIST_REQUESTED)
       {
           m_eSecState = SEC_STATE_NONE;
       }
       m_pReplyWaitTimer->stop();
       emit StationInfoList(stInfoList);

       if(m_bFetchStationArt)
       {
            LOG_CRITICAL<<m_Port<<" SH :: Fetch station art , after read from cache"<<LOG_ENDL;
            m_pStationCtrl->FetchStationArt(inStartIndex, inEndIndex);
       }
   }
   else if(m_pStCache->IsStationsAvailable(m_pStationCtrl->CurrentSortOrder() ,
                                   inStartIndex , inEndIndex , stInfoList))
   {
       LOG_HIGH<<m_Port<<" StateHandler requested station range available "<<LOG_ENDL;
       if(m_eSecState == SEC_STATE_STATION_LIST_REQUESTED)
       {
           m_eSecState = SEC_STATE_NONE;
       }
       m_pReplyWaitTimer->stop();
       emit StationInfoList(stInfoList);

       if(m_bFetchStationArt)
       {
            LOG_CRITICAL<<m_Port<<" SH :: Fetch station art , after read from cache"<<LOG_ENDL;
            m_pStationCtrl->FetchStationArt(inStartIndex, inEndIndex);
       }
   }
   else
   {

       LOG_HIGH<<m_Port<<" StateHandler stations not avilable  , Req to device "<<LOG_ENDL;
       error = m_pStationCtrl->RequestStationInfo(inStartIndex, inEndIndex);

       if(E_SUCCESS == error)
       {
           m_eSecState = SEC_STATE_STATION_LIST_REQUESTED ;
           if(m_bFetchStationArt)
           {
                LOG_CRITICAL<<m_Port<<" SH :: Fetch station art , no cache present "<<LOG_ENDL;
                m_pStationCtrl->FetchStationArt(inStartIndex, inEndIndex);
           }
       }
       else
       {
           LOG_CRITICAL<<m_Port<<"SIGNAL: Station List Error Emitted" << LOG_ENDL;
           m_eSecState = SEC_STATE_NONE ;
           //stop the timer
          // LOG_INFO<<m_Port<<"CPndrStateHandler::Request Station List error:m_pReplyWaitTimer stopped" <<LOG_ENDL;

           m_pReplyWaitTimer->stop();
           //LOG_SIGNAL <<m_Port<<"SIGNAL: Station List Error Emitted" << LOG_ENDL;
           emit Result(REQ_TYPE_STATION_LIST,E_INVALID_STATION_REQ_RANGE);
       }
   }
   //LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestStationList:Exit" <<LOG_ENDL;

   return error;
}

/**
 *******************************************************************
 * Request to select a station for start playing
 *
 * @param[in]    inStationToken    Token station which is selected
 *                                    to be played
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStateHandler::RequestStationSelect(const quint32 inStationToken)
{
    LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestStationSelect:Entry" <<LOG_ENDL;
    if(m_eSecState == SEC_STATE_AUTO_COMPLETE_SEARCH_REQUESTED)
        m_eSecPreviousState = SEC_STATE_AUTO_COMPLETE_SEARCH_REQUESTED;
    else
        CancelStationArtRequest();
    m_eSecState = SEC_STATE_STATION_SELECT_REQUESTED ;
    m_pStationCtrl->RequestStationSelect(inStationToken);
    LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestStationSelect:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Request for auto search complete given a word
 *
 * @param[in]    inWordToSearch    The word to be searched
 *                                    max length: 248
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStateHandler::RequestSearchAutoComplete(const QString& inWordToSearch)
{
    LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestSearchAutoComplete:Entry" <<LOG_ENDL;
    //TO DO: Merge the state with Search_Requested later
    if(m_pStationCtrl != NULL )
    {
    m_eSecState = SEC_STATE_AUTO_COMPLETE_SEARCH_REQUESTED ;
    m_pStationCtrl->RequestAutoCompleteSearch(inWordToSearch);
    }
    else
        LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestSearchAutoComplete: m_pStationCtrl == NULL" <<LOG_ENDL;        
    
    LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestSearchAutoComplete:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Request for extended search for a given word
 *
 * @param[in]    inWordToSearch    The word to be searched
 *                                    max length: 248
 *
 * @return  None
 *
 *******************************************************************
 */
void CPndrStateHandler::RequestSearch(const QString& inWordToSearch)
{
    LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestSearch:Entry" <<LOG_ENDL;
    m_eSecState = SEC_STATE_SEARCH_REQUESTED ;
    m_pStationCtrl->RequestExtendedSearch(inWordToSearch) ;
    LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestSearch:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Request to select the item from search
 * @param[in]    inSearchID    The searchId
 * @param[in]    inToken    The token to identify the track
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStateHandler::RequestSearchSelect(const quint32 inToken)
{
    LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestSearchSelect:Entry" <<LOG_ENDL;
    m_eSecState = SEC_STATE_SEARCH_SELECT_REQUESTED ;
    m_pStationCtrl->RequestSearchSelect(inToken);
    LOG_TRACE<<m_Port<<"CPndrStateHandler::RequestSearchSelect:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * The below methods are requested , when we receive session already started.
 * @param      None    
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStateHandler::RequestStatus()
{
    m_pDeviceManager->GetPandoraStatus();
}

void CPndrStateHandler::RequestActiveStation()
{
    m_pStationCtrl->GetActiveStationToken();
}

void CPndrStateHandler::RequestTrackBasic()
{
    m_pTrackProperties->RequestTrackInfo();
}


//{ modified by yongkyun.lee 2014-08-25 for : Branding Image
void CPndrStateHandler::RequestBrandingImage(QString BIPath ,QString BIName)
{
    QString BIFullPath;
    LOG_HIGH<<m_Port<<"StateHandler RequestBrandingImage " <<LOG_ENDL;
    m_BrandingImagePath = BIPath ;
    m_BrandingImageName = BIName;
    BIFullPath  = BIPath+BIName;
      QFile file(BIFullPath);
      if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      {
          LOG_HIGH<<"CPndrStateHandler::onBrandingImageStart file Open error"   <<LOG_ENDL;                 
          return;
      }

      LOG_INFO<<"CPndrStateHandler::onBrandingImageStart() <-"<<LOG_ENDL;
    
    m_pStationCtrl->RequestBrandingImage( file.size());
    return; 
}

void CPndrStateHandler::RequestBrandingSegment(const quint8 SegIndex,const quint8 totalSeg, quint8* inData,  int fzise)
{
     LOG_TRACE<<m_Port<<"StateHandler RequestBrandingSegment " <<LOG_ENDL;
}

//} modified by yongkyun.lee 2014-08-04 


/**
* Following signals will be emitted by decoder and listened here
*/

/**
* Used for capturing signal for following
* PNDR_UPDATE_STATUS
* PNDR_UPDATE_TRACK_BOOKMARK_SONG
* PNDR_UPDATE_TRACK_BOOKMARK_ARTIST
* PNDR_RETURN_STATION_COUNT
* PNDR_UPDATE_STATIONS_ORDER
* PNDR_RETURN_STATIONS_ORDER
* PNDR_UPDATE_NOTICE
* PNDR_UPDATE_TRACK_RATING
*/
void CPndrStateHandler::OnRequestCompletion(const EPndrEvent inRequest, const quint8 inData)
{

    LOG_SLOT<<m_Port<<"SH::OnRequestCompletion Req ID = "<<inRequest <<" Data = " << inData
               <<" m_ePriState:"<< m_ePriState<< " , m_eSecState:"<< m_eSecState << LOG_ENDL;

    //stop the timer
    //LOG_INFO<<m_Port<< "CPndrStateHandler::Status/Bookmark/Count/Order/rating/notice :m_pReplyWaitTimer stopped" <<LOG_ENDL;

    m_pReplyWaitTimer->stop();

    if( SESSION_ALREADY_STARTED == (SESSION_ALREADY_STARTED & m_eStatusOnSessionStart) )
    {
        LOG_HIGH<<m_Port<< "CPndrStateHandler:: [m_pReplyWaitTimer->start]" <<LOG_ENDL;
        m_eStatusOnSessionStart = (EStatusOnAlreadyStartedSession)(m_eStatusOnSessionStart|GET_STATUS );
        RequestActiveStation();
        m_pReplyWaitTimer->start();
    }

    switch(inRequest)
    {
        case EVENT_PLAY:
        {
            m_ePriState = PRI_STATE_PLAYING;
            LOG_SIGNAL <<m_Port<<"SIGNAL: Play Emitted" << LOG_ENDL;
            emit Result(REQ_TYPE_PLAY , E_SUCCESS);
            break;
        }
        case EVENT_PAUSE:
        {
            LOG_SIGNAL <<m_Port<<"SIGNAL: Pause Emitted" << LOG_ENDL;
            if(m_ePriState != PRI_STATE_PAUSE_REQUESTED  && m_ePriState != PRI_STATE_DEVICE_CONNECTED)
            {
                LOG_CRITICAL <<m_Port<<"Wrong state , auto pause came" << LOG_ENDL;
            }
            m_ePriState = PRI_STATE_PAUSED;
            emit Result(REQ_TYPE_PAUSE , E_SUCCESS);
            break;
        }       
        case EVENT_BOOKMARK_SONG:
        case EVENT_BOOKMARK_ARTIST:
        {
            ERequestType reqType = REQ_TYPE_BOOKMARK_SONG;
            if(EVENT_BOOKMARK_ARTIST == inRequest)
            {
                reqType = REQ_TYPE_BOOKMARK_ARTIST;
            }

            if(SEC_STATE_BOOKMARK_SONG_REQUESTED == m_eSecState ||
                SEC_STATE_BOOKMARK_ARTIST_REQUESTED == m_eSecState )
            {
                m_eSecState =   SEC_STATE_NONE ;
                // modified by esjang for certificate bookmark 
                /*
                if( inData)
                {
                    LOG_SIGNAL<<m_Port<<"SIGNAL: Bookmark Emitted" << LOG_ENDL;
                    emit  Result(reqType,E_SUCCESS );
                }
                else
                {
                    LOG_CRITICAL<<m_Port<<"CRITICAL:Bookmark Failed" << LOG_ENDL;
                    emit Result(reqType, E_BOOKMARK_FAILED);
                }
                */
            }
            if( inData)
            {
               // LOG_SIGNAL<<m_Port<<"SIGNAL: Bookmark Emitted" << LOG_ENDL;

                if(reqType == REQ_TYPE_BOOKMARK_ARTIST)
                    m_sTrackInfo.bArtistBookmarked = true;
                else
                    m_sTrackInfo.bTrackBookmarked = true;

                emit  Result(reqType,E_SUCCESS );
            }
            else
            {
                LOG_CRITICAL<<m_Port<<"CRITICAL:Bookmark Failed" << LOG_ENDL;
                emit Result(reqType, E_BOOKMARK_FAILED);
            }
            break;
        }
        case EVENT_SORT:
        case EVENT_SORT_ORDER:
        {
            //LOG_INFO <<m_Port<<"Sort Order received " << LOG_ENDL;
           // LOG_INFO <<m_Port<<"Sort Order received SecState = " << m_eSecState << LOG_ENDL;

            ESortType sortType = SORT_TYPE_DATE;
            if(inData == 1 )
            {
                sortType = SORT_TYPE_ALPHA;
            }
            //LOG_SIGNAL<<m_Port<<"SIGNAL: StationSortOrder Emitted" << LOG_ENDL;

            emit StationSortOrder(sortType);
            if(SEC_STATE_STATION_LIST_REQUESTED == m_eSecState)
            {
                m_eSecState = SEC_STATE_NONE;
            }
//No need of requesting station count on sort order request
//            if(SEC_STATE_STATION_LIST_REQUESTED == m_eSecState)
//            {
//                //Issue request for getting the station count
//                LOG_INFO <<m_Port<<"RequestStationCount " << LOG_ENDL;
//                m_pStationCtrl->RequestStationCount();
//            }
            break;
        }
        case EVENT_STATION_COUNT:
        {
            //LOG_CRITICAL <<m_Port <<"Station Count received : "<< inData << LOG_ENDL;

            if(SEC_STATE_STATION_LIST_REQUESTED == m_eSecState)
            {
                //Request for All Station Tokens in the sorted order
                //LOG_INFO <<m_Port<<"RequestAllStationTokens " << LOG_ENDL;

                m_pStationCtrl->RequestAllStationTokens();
            }           
            break;
        }

        case EVENT_RATE_POS:
        case EVENT_RATE_NEG:
        case EVENT_UPDATE_RATE:
        {
            if (inRequest == EVENT_UPDATE_RATE) 
            {
                LOG_CRITICAL <<m_Port<< LOG_TAG_STATEHANDLER <<"CRITICAL:Ratings is handled "<<LOG_ENDL;                
                //LOG_SIGNAL <<m_Port<<"SIGNAL: Rate Update Emitted" << LOG_ENDL;

                if( 0 == inData ) // Rating None
                    emit Result(REQ_TYPE_RATE_NONE, E_SUCCESS);
                else if(1 == inData) //Rating Positive
                    emit Result(REQ_TYPE_RATE_POS, E_SUCCESS);
                else if(2 == inData) //Rating Negative
                    emit Result(REQ_TYPE_RATE_NEG, E_SUCCESS);
                // TODO: Compare track tokens with the last track info -esjang

            }            
            if(SEC_STATE_THUMB_UP_REQUESTED == m_eSecState ||
              SEC_STATE_THUMB_DOWN_REQUESTED == m_eSecState)
            {
                if(0 != inData)
                {
                    m_eSecState =   SEC_STATE_NONE ;
                    // This signal is emitted by decoder only for Success case
                    // So inData validation not required.
                    //LOG_SIGNAL <<m_Port<<"SIGNAL: Rate track Emitted" << LOG_ENDL;
                    //emit Result(REQ_TYPE_RATE,E_SUCCESS );
                }

            }
            
            break;
        }
        default:
            {
            LOG_CRITICAL <<m_Port<< LOG_TAG_STATEHANDLER <<"CRITICAL:OnRequestCompletion Not handled "<<LOG_ENDL;
            break;
            }
    }

   // LOG_INFO<<m_Port<< "CPndrStateHandler::OnRequestCompletion:Exit" <<LOG_ENDL;

}

/**
* Used for capturing signal for following
* PNDR_UPDATE_TRACK_ELAPSED
*/
void CPndrStateHandler::OnRequestCompletion(const EPndrEvent /*inRequest*/, const quint16 inData)
{
    //LOG_SLOT<<m_Port<< "CPndrStateHandler::OnRequestCompletion:Entry" <<LOG_ENDL;
    //LOG_SLOT<<m_Port<<"Request id :"<<inRequest <<"in Data : " << inData << LOG_ENDL;

    //LOG_INFO<< "CPndrStateHandler::Track elapsed:m_pReplyWaitTimer stopped" <<LOG_ENDL;

    //stop the timer
    m_pReplyWaitTimer->stop();

    //Set the Track Info elapsed time if TrackInfo req was made
    if(SEC_STATE_TRACK_INFO_REQUESTED == m_eSecState)
    {
        m_sTrackInfo.nElapsed = inData;
    }
    //For TrackInfo req/ Play req, emit the elapsed time
    //LOG_INFO <<m_Port<<"SIGNAL: UpdateElapsedTime Emitted" << LOG_ENDL;
    emit UpdateElapsedTime(inData);

    if (( SEC_STATE_TRACK_INFO_REQUESTED == m_eSecState)
            && (TRACK_INFO_ALL_RECEIVED == m_eTrackInfoState))
    {
        m_eTrackInfoState = TRACK_INFO_NONE;
        m_eSecState = SEC_STATE_NONE ;
        LOG_SIGNAL <<"SIGNAL: TrackInfo Emitted" << LOG_ENDL;
        emit TrackInfo(m_sTrackInfo);
    }

    //LOG_INFO<<m_Port<< "CPndrStateHandler::OnRequestCompletion:Exit" <<LOG_ENDL;
}

/**
* Used for capturing signal for following
* PNDR_RETURN_TRACK_ALBUM_ART_SEGMENT // emitted by track controller
* PNDR_RETURN_TRACK_EXPLAIN_SEGMENT // emitted by track controller
*/
void CPndrStateHandler::OnRequestCompletion(const EPndrEvent inRequest, const QString& inData)
{
    LOG_SLOT<<m_Port<< "CPndrStateHandler::OnRequestCompletion Req ID = "<<inRequest <<LOG_ENDL;

    //stop the timer
   // LOG_INFO<<m_Port<< "CPndrStateHandler::Art/Explain:m_pReplyWaitTimer stopped" <<LOG_ENDL;

    m_pReplyWaitTimer->stop();

    switch(inRequest)
    {
        case EVENT_GET_ALBUM_ART:
        {
            LOG_SIGNAL<<m_Port<< "Album art url emitted is: " << inData << LOG_ENDL;
            
            emit AlbumArt(inData);

            break;
        }
        case EVENT_GET_EXPLAIN:
        {
            if(SEC_STATE_TRACK_EXPLAIN_REQUESTED == m_eSecState)
            {
                m_eSecState = SEC_STATE_NONE;              
                emit TrackExplain(inData);
            }
            break;
        }
    default:
        break;

    }

   // LOG_INFO<<m_Port<< "CPndrStateHandler::OnRequestCompletion:Exit" <<LOG_ENDL;
}


/**
* Used for capturing signal for following
*   PNDR_RETURN_SEARCH_RESULT_INFO
*/
void CPndrStateHandler::OnRequestCompletion(const EPndrEvent inRequest,const quint32 inSearchId,
                                            const QList<TSSearchInfo>& inSearchList)
{
    Q_UNUSED(inRequest)
    //LOG_SLOT<<m_Port<< "CPndrStateHandler::OnRequestCompletion:Entry" <<LOG_ENDL;
   // LOG_INFO<<m_Port<<"Search Result Info obtained for Search Id = " << inSearchId << LOG_ENDL;

    //stop the timer
    //LOG_INFO<< "CPndrStateHandler::Search Result Info:m_pReplyWaitTimer stopped" <<LOG_ENDL;

    m_pReplyWaitTimer->stop();

    if(SEC_STATE_SEARCH_REQUESTED == m_eSecState
            || SEC_STATE_AUTO_COMPLETE_SEARCH_REQUESTED == m_eSecState ||
            SEC_STATE_AUTO_COMPLETE_SEARCH_REQUESTED == m_eSecPreviousState)
    {

        m_eSecState = SEC_STATE_NONE ;
        m_eSecPreviousState = SEC_STATE_NONE;
        emit SearchComplete(inSearchList) ;
    }
    else{
        LOG_SIGNAL <<m_Port<<"StateHandler :inSearchId["<<inSearchId<<"] getLastSearchId:"<<m_pStationCtrl->getLastSearchId()<<LOG_ENDL;
        //{ added by cheolhwan 2014-03-08. To modified the screen update error in SearchView during the continuous inputting.
        if(inSearchId <= m_pStationCtrl->getLastSearchId())
        {
            LOG_SIGNAL <<m_Port<<" StateHandler : SearchComplete()" << LOG_ENDL;
            m_eSecState = SEC_STATE_NONE ;
            m_eSecPreviousState = SEC_STATE_NONE;
            emit SearchComplete(inSearchList) ;
        }
        //} added by cheolhwan 2014-03-08. To modified the screen update error in SearchView during the continuous inputting.
    }

  //  LOG_SLOT<<m_Port<< "CPndrStateHandler::OnRequestCompletion:Exit" <<LOG_ENDL;
}

/**
* Used for capturing signal for following
*   PNDR_UPDATE_STATION_ADDED
*/
void CPndrStateHandler::OnRequestCompletion(const EPndrEvent inRequest,
                                            const quint32 inToken, const quint8 inIndex)
{

    //LOG_SLOT<<m_Port<< "CPndrStateHandler::OnRequestCompletion:Entry" <<LOG_ENDL;
    LOG_SLOT<<m_Port<<"CPndrStateHandler::Station Added Req ID = "<<inRequest <<" Token = " << inToken
            << " Index = " << inIndex << LOG_ENDL;

   // LOG_INFO<<m_Port<< "CPndrStateHandler::Station Added:m_pReplyWaitTimer stopped" <<LOG_ENDL;

    //stop the timer
    m_pReplyWaitTimer->stop();

    if(SORT_TYPE_NONE != m_pStationCtrl->CurrentSortOrder() ) // No use of sending this info
    {
        TSStationInfo inStationInfo;
        inStationInfo.nStationToken = inToken;
        //{ modified by cheolhwan 2014-04-25. ITS 234067. To ignore a duplicate of the message with an incorrect sequence number.
        if(m_LastStationToken == inToken && inToken != 0)
        {
            LOG_CRITICAL<<m_Port<< "CPndrStateHandler::OnRequestCompletion  m_LastStationToken["<<m_LastStationToken<<"] , inToken["<<inToken<<"]"<<LOG_ENDL;
            m_LastStationToken = inToken;
            return;
        }
        m_LastStationToken = inToken;
        //} modified by cheolhwan 2014-04-25. ITS 234067. To ignore a duplicate of the message with an incorrect sequence number.
        
        //TO DO: Get the station Label here before emitting
        LOG_SIGNAL <<"SIGNAL: Station Added Emitted" << LOG_ENDL;

        m_pStCache->InsertToken(m_pStationCtrl->CurrentSortOrder() ,inIndex, inToken);

        emit StationAdded(inIndex, inStationInfo);
    }
   // LOG_INFO<<m_Port<< "CPndrStateHandler::OnRequestCompletion:Exit" <<LOG_ENDL;

}

/**
* Used for capturing signal for following
* PNDR_UPDATE_TRACK
* PNDR_UPDATE_TRACK_ALBUM_ART
* PNDR_UPDATE_TRACK_EXPLAIN // emitted by track controller
* PNDR_UPDATE_STATION_DELETED
* PNDR_UPDATE_STATION_ACTIVE
*/
void CPndrStateHandler::OnRequestCompletion(const EPndrEvent inRequest, const quint32 inData)
{
    LOG_SLOT<<m_Port<< "CPndrStateHandler::OnRequestCompletion Req ID = " << inRequest
            << " Data = " << inData<<LOG_ENDL;
   // LOG_SLOT<<m_Port<<"Request id : "<<inRequest <<" inData : " << inData << LOG_ENDL;

    //stop the timer
   // LOG_INFO<<m_Port<< "CPndrStateHandler::Track/Album Art/Explain/Station Deleted/Active:m_pReplyWaitTimer stopped" <<LOG_ENDL;

    m_pReplyWaitTimer->stop();

    switch(inRequest)
    {
        case EVENT_TRACK_UPDATED:        
        {
            LOG_SLOT<<m_Port<<"Track Updates, m_ePriState:"<<m_ePriState << LOG_ENDL;

            // Handle updateTrack received after Connect
            if( PRI_STATE_DEVICE_CONNECTED == m_ePriState)
            {
                if(inData)
                {
                    //Currently HMI is not handling this state
                    // No screen transitions
                    m_ePriState = PRI_STATE_PANDORA_CONNECTED;
                    LOG_SIGNAL <<m_Port<<"SIGNAL: Connect Success Emitted" << LOG_ENDL;
                    emit Result(REQ_TYPE_CONNECT , E_SUCCESS);
                }
                else
                {
                   LOG_CRITICAL <<m_Port<< "CRITICAL:Connect Failed. Check Device." << LOG_ENDL;
                   emit Result(REQ_TYPE_CONNECT , E_CHECK_PNDR_APP_ON_DEVICE);
                }
            }

            //For any Secondary State, if track has been updated notify HMI

            //Once UpdateTrack is called, HMI should take care of
            // calling getTrackInfo for updated track
            //LOG_SIGNAL <<m_Port<<"SIGNAL: Track Updated Emitted" << LOG_ENDL;
           // emit TrackUpdated(inData);

            //If Skip or Search Select was requested, the state should be reset
            if(SEC_STATE_SKIP_REQUESTED == m_eSecState||
               SEC_STATE_SEARCH_SELECT_REQUESTED == m_eSecState ||
                    SEC_STATE_STATION_SELECT_REQUESTED == m_eSecState)
            {
                m_eSecState = SEC_STATE_NONE ;
            }

           // LOG_SIGNAL <<m_Port<<"SIGNAL: Track Updated Emitted" << LOG_ENDL;
			//{ added by Radha. added to start 30sec timer when received station teken is "0".
            //This is an exception probably for low network scenario .
            //If we dnt receive valid track id to request track info and wait for some time to be timed out.
            if(inData == 0)
            {
                 LOG_HIGH <<m_Port<<"Track Updated with Id : 0 ,Wait Timer Started " << LOG_ENDL;
                 m_pReplyWaitTimer->start();
            }
			//} added by Radha. added to start 30sec timer when received station teken is "0".
            emit TrackUpdated(inData);

            break;
        }
        case EVENT_TRACK_COMPLETED:
        {
            //Not handled currently
            break;
        }
        case EVENT_ALBUM_ART:
        {
            // TO DO: Emit the length if required , Currently Not required
            m_sTrackInfo.nAlbumArtLength = inData;
            if(m_sTrackInfo.nAlbumArtLength > 0 && IMG_TYPE_NONE != m_eAlbumArtImgType)
            {
                //LOG_INFO<<" Album art has been updated , Plz download"<<LOG_ENDL;
                m_pTrackProperties->RequestAlbumArt(m_eAlbumArtImgType);
            }
            break;
        }
        case EVENT_EXPLAIN:
        {
            // TO DO: Emit the length if required
            break;
        }
        case EVENT_STATION_DELETE:
        {
            //If a station was deleted, reset the state and emit the signal
            // HMI should take care of requesting StationList update after this
            if(SEC_STATE_STATION_DELETE_REQUESTED == m_eSecState)
            {
                m_eSecState = SEC_STATE_NONE;
            }
                LOG_SIGNAL <<m_Port<<"SIGNAL: Station Deleted Emitted" << LOG_ENDL;

                m_pStCache->RemoveToken(m_pStationCtrl->CurrentSortOrder(), inData);

                emit StationDeleted(inData);

            break;
        }
        case EVENT_STATION_ACTIVE:
        {
            //If Station select was done, only state is reset.
            // UpdateTrack will be emitted when PNDR_UPDATE_TRACK comes
            if(SEC_STATE_STATION_SELECT_REQUESTED == m_eSecState)
            {
                m_eSecState = SEC_STATE_NONE;
            }

            
            //LOG_SIGNAL << m_Port << "esjang 0612 m_ePriState: " << m_ePriState << LOG_ENDL;
            
            if(PRI_STATE_DEVICE_CONNECTED == m_ePriState ) // modified by esjang 2013.06.12 for ITS # 172690, 172599, 172697
            {
                //LOG_SIGNAL << m_Port << "esjang 0612 m_PriState changed to Connected" << LOG_ENDL;
                m_ePriState = PRI_STATE_PANDORA_CONNECTED;
            } 

            LOG_SIGNAL <<m_Port<<"SIGNAL: Station Updated Emitted with stationToken " <<inData << LOG_ENDL;
            emit StationUpdated(inData);

            if(GET_STATUS == (GET_STATUS & m_eStatusOnSessionStart))
            {
                m_eStatusOnSessionStart = (EStatusOnAlreadyStartedSession)( m_eStatusOnSessionStart|GET_ACTIVE_STATION) ;
                RequestTrackBasic();
                m_pReplyWaitTimer->start();
            }
            break;
        }
        default:
        {
           break;
        }
    }

    //LOG_INFO<<m_Port<< "CPndrStateHandler::OnRequestCompletion:Exit" <<LOG_ENDL;
}

/**
* Used for capturing signal for following
* PNDR_RETURN_TRACK_INFO_EXTENDED
*/
void CPndrStateHandler::OnRequestCompletion(const EPndrEvent inRequest,
                                            const TSTrackInfo& inData)
{
//    LOG_INFO<<m_Port<< "CPndrStateHandler::OnRequestCompletion:Entry" <<LOG_ENDL;
//    LOG_INFO<<m_Port<<"Request id : Return Track Info Extended" << LOG_ENDL;

    //stop the timer
    //LOG_INFO<< "CPndrStateHandler::Track Info:m_pReplyWaitTimer stopped" <<LOG_ENDL;

    m_pReplyWaitTimer->stop();

    m_eTrackInfoState = (ETrackInfoState)(m_eTrackInfoState|TRACK_INFO_TRK_INFO); // track  part info received

//    if(EVENT_EXTENDED_TRACK_INFO == inRequest && SEC_STATE_TRACK_INFO_REQUESTED == m_eSecState )
//    {

        m_sTrackInfo = inData;
        m_pStationCtrl->IsStInfoAvail(m_sTrackInfo);
   // }

    LOG_HIGH <<m_Port<<"SH::OnRequestCompletion Trackinfo recv Sec st. =  = " << m_eSecState << " TrackInfo st. = " << m_eTrackInfoState << LOG_ENDL;


    if ( SEC_STATE_TRACK_INFO_REQUESTED == m_eSecState)
    {
        //m_eTrackInfoState = TRACK_INFO_NONE;
        m_eSecState = SEC_STATE_NONE ;        
    }   

    if(TRACK_INFO_ALL_RECEIVED == m_eTrackInfoState)
    {
        m_eTrackInfoState = TRACK_INFO_NONE;
    }

    if(m_sTrackInfo.nAlbumArtLength > 0 && IMG_TYPE_NONE != m_eAlbumArtImgType)
    {
         m_pTrackProperties->RequestAlbumArt(m_eAlbumArtImgType);
    }

    if( GET_ACTIVE_STATION == (GET_ACTIVE_STATION & m_eStatusOnSessionStart) )
    {
        LOG_HIGH<<" SH::Session already started , Track Info recived : " << inData.nTrackToken<<LOG_ENDL;
        m_eStatusOnSessionStart = (EStatusOnAlreadyStartedSession)( m_eStatusOnSessionStart|GET_TRACK_INFO);
        m_eStatusOnSessionStart = STATE_NONE;

        m_pTrackProperties->UpodateTrackToken(inData.nTrackToken);
        emit TrackUpdated(inData.nTrackToken);
    }
    else{
        //LOG_INFO <<m_Port<<"SIGNAL: TrackInfo Emitted" << LOG_ENDL;
        emit TrackInfo(m_sTrackInfo);
    }

   //LOG_INFO<<m_Port<<"CPndrStateHandler::OnRequestCompletion:Exit" <<LOG_ENDL;

}

/**
* Used for capturing signal for following
*   PNDR_RETURN_STATION_INFO
*/
void CPndrStateHandler::OnRequestCompletion(const EPndrEvent inRequest, const QList<TSStationInfo>& inData)
{
    Q_UNUSED(inRequest)
    LOG_INFO<<m_Port<<"CPndrStateHandler::OnRequestCompletion: Station Info Received Entry" <<LOG_ENDL;

    //stop the timer
   // LOG_INFO<<m_Port<<"CPndrStateHandler::Station info:m_pReplyWaitTimer stopped" <<LOG_ENDL;

    m_pReplyWaitTimer->stop();

    LOG_HIGH <<m_Port<<"SH:: St.Info recv Sec st.["<<m_eSecState<<"], m_eSecPreviousState["<<m_eSecPreviousState<<"], Trackinfo st.["<<m_eTrackInfoState<<"] m_PreFetchingStations["<<m_PreFetchingStations<<"]"<<LOG_ENDL;

    //If track info was requested, Append the current station name to the Track Info

    bool emitStationInfo = true;
    //{ modified by cheolhwan 2014-04-17. To fix the unlimited receiving station. Added the condition "m_PreFetchingStations". (ITS 238629)
    if((SEC_STATE_STATION_LIST_REQUESTED != m_eSecState && !m_PreFetchingStations) && inData.count())
    {
        m_eTrackInfoState= (ETrackInfoState)(m_eTrackInfoState|TRACK_STATION_INFO);
        m_sTrackInfo.CurrentStation = inData.at(0).StationLabel;
        m_sTrackInfo.StationToken = inData.at(0).nStationToken;
        m_sTrackInfo.bShared = inData.at(0).bShared;
        m_pStationCtrl->SetActiveStation(m_sTrackInfo);
        emitStationInfo = false;
        m_pStCache->SaveStationInfo(inData);
    }
    else {
        if (SEC_STATE_STATION_LIST_REQUESTED != m_eSecState && !m_PreFetchingStations){
            m_eTrackInfoState= (ETrackInfoState)(m_eTrackInfoState|TRACK_STATION_INFO);
            emitStationInfo = false;
        }
    }

    //If all track info has been received, emit the TrackInfo
    if ((SEC_STATE_STATION_LIST_REQUESTED != m_eSecState && !m_PreFetchingStations)
            && (TRACK_INFO_ALL_RECEIVED == m_eTrackInfoState))
    {
        emitStationInfo = false;
        m_eTrackInfoState = TRACK_INFO_NONE;
        if( SEC_STATE_TRACK_INFO_REQUESTED == m_eSecState)
            m_eSecState = SEC_STATE_NONE ;
        LOG_INFO <<m_Port<<"SIGNAL: Track Info Emitted" << LOG_ENDL;

        m_pStationCtrl->RequestCurrentStationArt();
        emit currentStationInfo(m_sTrackInfo);

        emit TrackInfo(m_sTrackInfo);
    }

    if(SEC_STATE_STATION_LIST_REQUESTED == m_eSecState || m_PreFetchingStations)
    {
        m_eSecState = SEC_STATE_NONE ;
    }
    //} modified by cheolhwan 2014-04-17. To fix the unlimited receiving station. Added the condition "m_PreFetchingStations".

    if(emitStationInfo)
    {
        m_pStCache->SaveStationInfo(inData);
        LOG_HIGH <<m_Port<<"SIGNAL: StationInfo List Emitted" << LOG_ENDL;
        emit StationInfoList(inData);
    }

    //LOG_INFO<<m_Port<<"CPndrStateHandler::OnRequestCompletion:Station Info Received Exit" <<LOG_ENDL;

}

/**
* Used for capturing signal for following
*   PNDR_RETURN_STATION_TOKENS
*/
void CPndrStateHandler::OnRequestCompletion(const EPndrEvent inRequest, const quint8 inStart_index,
                                            const QList<quint32>& inTokenList)
{
    //LOG_SLOT<<m_Port<<"CPndrStateHandler::OnRequestCompletion: Station Tokens Received:Entry" <<LOG_ENDL;
    LOG_SLOT<<m_Port<<" SH::St Token recv Req ID ="<<inRequest <<" Start Index = " << inStart_index << " Sec St:" << m_eSecState << LOG_ENDL;

    //LOG_INFO<<m_Port<< "CPndrStateHandler::Station Tokens:m_pReplyWaitTimer stopped" <<LOG_ENDL;

    //stop the timer
    m_pReplyWaitTimer->stop();

    switch(inRequest)
    {
        case EVENT_STATION_TOKENS:
        {
            if(inTokenList.count() > 1) {// Expecting station token count must be > 1
                m_pStCache->SaveStationToken(m_pStationCtrl->CurrentSortOrder() , inTokenList);
                m_pStationCtrl->SaveStationTokens(inTokenList);
            }
            else
                LOG_CRITICAL<<m_Port<<"****Station Token count==> "<< inTokenList.count()<< LOG_ENDL;

            if(SEC_STATE_STATION_LIST_REQUESTED == m_eSecState)
            {
                //Once the sort order and station count is received, emit StationTokens
               // LOG_INFO <<m_Port<<"SIGNAL: StationTokens Emitted" << LOG_ENDL;
                emit StationTokens(inTokenList.count(),inTokenList);
            }
            break;
        }
        default:
            break;
    }

   // LOG_INFO<<m_Port<<"CPndrStateHandler::OnRequestCompletion:Station Tokens Received:Exit" <<LOG_ENDL;
}

/**
* Used for capturing signal for following
*   PNDR_UPDATE_SEARCH
*/
void CPndrStateHandler::OnRequestCompletion(const EPndrEvent inRequest, const quint32 inData1,
                                            const QList<quint32>& inData2)
{
//    LOG_SLOT<<m_Port<<"CPndrStateHandler::OnRequestCompletion:Entry" <<LOG_ENDL;
//    LOG_INFO<<m_Port<<"Search Update received for searchId " << inData1 << LOG_ENDL;
    //stop the timer
    //LOG_INFO<< "CPndrStateHandler::Update Search:m_pReplyWaitTimer stopped" <<LOG_ENDL;

    m_pReplyWaitTimer->stop();

    switch(inRequest)
    {
        case EVENT_SEARCH:
        {
            if(SEC_STATE_SEARCH_REQUESTED == m_eSecState
                || SEC_STATE_AUTO_COMPLETE_SEARCH_REQUESTED == m_eSecState
                || SEC_STATE_AUTO_COMPLETE_SEARCH_REQUESTED == m_eSecPreviousState )
            {
                //Update Search is obtained
                //Issue request to get the search result info
                //LOG_INFO<<m_Port<<"Request Search result Info for search Id " << inData1<<LOG_ENDL;
                //{ Modified by Radha. Does not send request when no music tokens.
                //m_pReplyWaitTimer->start();
                //m_pStationCtrl->RequestSearchResultInfo(inData1,inData2);
                if(inData2.count() > 0) {
                    m_pReplyWaitTimer->start();
                    m_pStationCtrl->RequestSearchResultInfo(inData1,inData2);
                }
                else{
                    LOG_HIGH<<m_Port<<"No MusicTokens available,Emited SearchComplete with no result" <<LOG_ENDL;
                    m_eSecState = SEC_STATE_NONE ;
                    m_eSecPreviousState = SEC_STATE_NONE;
                    QList<TSSearchInfo> inSearchList;
                    emit SearchComplete(inSearchList) ;
                }
                //} Modified by Radha. Does not send request when no music tokens.
            }
            else{
                LOG_HIGH <<m_Port<<"StateHandler :: UPDATE Search -> SearchId:"<<inData1<<" getLastSearchId:"<<m_pStationCtrl->getLastSearchId()<< LOG_ENDL;
                //{ added by cheolhwan 2014-03-08. To modified the screen update error in SearchView during the continuous inputting.
                if(inData1 <= m_pStationCtrl->getLastSearchId())
                {
                    if(inData2.count() > 0) {
                        LOG_HIGH <<m_Port<<"StateHandler :: RequestSearchResultInfo"<< LOG_ENDL;
                        m_pReplyWaitTimer->start();
                        m_pStationCtrl->RequestSearchResultInfo(inData1,inData2);
                    }
                }
                //} added by cheolhwan 2014-03-08. To modified the screen update error in SearchView during the continuous inputting.
            }
            break;
        }
        default:
            break;
    }

//    LOG_SLOT<<m_Port<<"CPndrStateHandler::OnRequestCompletion:Exit" <<LOG_ENDL;
}


//{ modified by yongkyun.lee 2014-08-25 for : Branding Image 
void CPndrStateHandler::OnRequestBrandingImage()
{
    LOG_SLOT<<m_Port<<"CPndrStateHandler::OnRequestBrandingImage  m_BandingImageTimer start" << LOG_ENDL;
    m_BandingImageTimer->start();        

}
//} modified by yongkyun.lee 2014-08-04 

/** 
* Device connection slot
*/
void CPndrStateHandler::OnDeviceStatusUpdated(const EDeviceType inDeviceType, bool inIsConnected)
{    
    //LOG_SLOT<<m_Port<<"CPndrStateHandler::OnDeviceStatusUpdated:Entry" <<LOG_ENDL;

    LOG_SLOT<<m_Port<<"SH::OnDeviceStatusUpdated Dev = " << inDeviceType << " status = " << inIsConnected
            << " m_ePriState:" << m_ePriState<< LOG_ENDL;

    //stop the timer
    //LOG_INFO<<m_Port<<"CPndrStateHandler::Device Status Updated:m_pReplyWaitTimer stopped" <<LOG_ENDL;

    m_pReplyWaitTimer->stop();

    if(inIsConnected)
    {
        if( PRI_STATE_INIT == m_ePriState )
        {
             m_ePriState = PRI_STATE_DEVICE_CONNECTED ;
             m_pDeviceManager->ConnectPandora(m_nAlbumArtDimension , m_eAlbumArtImgType,
                                              m_nStationArtDimension);            
        }       
    }
    else
    {        
        LOG_CRITICAL <<m_Port<<"CRITICAL:Connect Failed. Device Not found" << LOG_ENDL;
        emit Result(REQ_TYPE_CONNECT , E_DEVICE_NOT_FOUND);
    }

   // LOG_SLOT<<m_Port<<"CPndrStateHandler::OnDeviceStatusUpdated:Exit" <<LOG_ENDL;

}

/**
* On Error
* This slot is ovoked whehever some error is emitted by decoder
* @param[in]    inErrCode    The errorcode
*
* @return     None
*/
void CPndrStateHandler::OnErrorReceived(EPndrError inErrCode)
{
   // LOG_SLOT<<m_Port<<"CPndrStateHandler::OnErrorReceived:Entry" <<LOG_ENDL;
    LOG_CRITICAL <<m_Port<<" SH::OnErrorReceived errCode = " <<inErrCode
                 << " m_ePriState:"<<m_ePriState << " m_eSecState:"<<m_eSecState<< LOG_ENDL;

   // LOG_CRITICAL <<m_Port<<"OnErrorReceived when m_ePriState ="<< m_ePriState << "m_eSecState =" << m_eSecState << LOG_ENDL;
    //stop the timer
   // LOG_INFO<<m_Port<<"CPndrStateHandler::Onerror :m_pReplyWaitTimer stopped" <<LOG_ENDL;

    if (m_pReplyWaitTimer)
    {
    	m_pReplyWaitTimer->stop();
    }

    m_eStatusOnSessionStart = STATE_NONE;    
    //{ added by Radha. To prevent that receiving station popup is displayed indefinitely.
    if( m_ePriState >= PRI_STATE_PANDORA_CONNECTED  && E_SESSION_ALREADY_STARTED == inErrCode)
    {
        LOG_HIGH<<"***IGNORE SESSION_ALREADY_STARTED error while State is INVALID"<<LOG_ENDL;
        //{ modified by yongkyun.lee 2014-11-27 for : ITS  253427
        //if(m_eSecState == SEC_STATE_STATION_LIST_REQUESTED)
            m_eSecState = SEC_STATE_NONE ;
        //} modified by yongkyun.lee 2014-11-27 
        return;
    }
    //} added by Radha. To prevent that receiving station popup is displayed indefinitely.
    switch(m_eSecState)
    {
        case SEC_STATE_NONE:
        {
            if( m_ePriState > PRI_STATE_NONE )
            {
                switch(inErrCode)
                {
                    //If No Station or No Active Station or Station Already started
                    //is received, Pandora Connect has been successful in that case.
                    case E_NO_ACTIVE_STATIONS:
                    case E_NO_STATIONS:
                    case E_SESSION_ALREADY_STARTED:
                    case E_STATION_DOES_NOT_EXIST:
                    {
                        if( m_ePriState > PRI_STATE_INIT /*PRI_STATE_DEVICE_CONNECTED == m_ePriState*/ )
                        {
                            //LOG_INFO << m_Port<<"Pandora State set to Connected "<< LOG_ENDL;
                            m_ePriState = PRI_STATE_PANDORA_CONNECTED;
                            if(inErrCode == E_SESSION_ALREADY_STARTED)
                            {
                                //LOG_INFO <<m_Port<< "Pandora State set to Connected "<< LOG_ENDL;
                                m_eStatusOnSessionStart = SESSION_ALREADY_STARTED ;
                                RequestStatus();
                                m_pReplyWaitTimer->start();
                            }                            
                        }
                        else
                        {
                             m_ePriState = PRI_STATE_INIT;
                        }

//                        if(inErrCode == E_SESSION_ALREADY_STARTED)
//                        {
//                            LOG_INFO <<m_Port<< "Pandora State set to Connected "<< LOG_ENDL;
//                            m_eStatusOnSessionStart = SESSION_ALREADY_STARTED ;
//                            RequestStatus();
//                        }
                        break;
                    }                    
                    case E_INSUFFICIENT_CONNECTIVITY:
                    {

                        if(m_ePriState < PRI_STATE_PANDORA_CONNECTED)
                        {
                            m_ePriState = PRI_STATE_INIT;
                            inErrCode = E_NETWORK_FAILED;
                        }
                        break;
                    }
                    case E_TRACK_RATING_FAILED:
                    {
                        LOG_CRITICAL <<m_Port<<"TRACK RATING FAILED - Comes while sec state is None"<< LOG_ENDL;
                        break;
                    }
                    //{ added by esjang 2013.08.12 for ITS #183727
                    case E_SKIP_LIMIT_REACHED:
                    {
                        LOG_CRITICAL <<m_Port<<"E_SKIP_LIMIT_REACHED - Comes while sec state is None"<< LOG_ENDL;
                        break;
                    }
                    //} added by esjang 2013.08.12 for ITS #183727
                    /*
                    case E_BOOKMARK_FAILED:
                    {
                        LOG_CRITICAL <<m_Port<<"TRACK BookMark FAILED - Comes while sec state is None"<< LOG_ENDL;
                        break;
                    }
                    */
//                    E_LICENSING_RESTRICTIONS = 31,
//                    E_INVALID_LOGIN = 32,
//                    E_NOTICE_ERROR_MAINTENANCE = 33
                    default:
                    {
                        LOG_CRITICAL <<m_Port<<" CRITICAL:Connect Failed"<< LOG_ENDL;
                        m_ePriState = PRI_STATE_INIT;
                        break;
                    }
                }

                LOG_SIGNAL <<m_Port<<"SH::Emit error to HMI error = "<< inErrCode << LOG_ENDL;
                emit Result(REQ_TYPE_CONNECT,inErrCode);

            }
            break;
        }
        case SEC_STATE_SKIP_REQUESTED:
        {
            LOG_CRITICAL <<m_Port<<" CRITICAL:Skip Failed"<< LOG_ENDL;
            emit Result(REQ_TYPE_SKIP,inErrCode);
            break;
        }
        case SEC_STATE_THUMB_UP_REQUESTED:
        case SEC_STATE_THUMB_DOWN_REQUESTED:
        {
            LOG_CRITICAL <<m_Port<<" CRITICAL:Track Rating Failed"<< LOG_ENDL;
            emit  Result(REQ_TYPE_RATE,inErrCode );
            break;
        }
        case SEC_STATE_BOOKMARK_SONG_REQUESTED:
        {
            LOG_CRITICAL <<m_Port<<" CRITICAL:Bookmark Song Failed"<< LOG_ENDL;
            emit  Result(REQ_TYPE_BOOKMARK_SONG,inErrCode );
            break;
        }
        case SEC_STATE_BOOKMARK_ARTIST_REQUESTED:
        {
            LOG_CRITICAL <<m_Port<<" CRITICAL:Bookmark Artist Failed"<< LOG_ENDL;
            emit  Result(REQ_TYPE_BOOKMARK_ARTIST,inErrCode );
            break;
        }
        case SEC_STATE_STATION_SELECT_REQUESTED:
        {
            LOG_CRITICAL <<m_Port<<" CRITICAL:Station Select Failed"<< LOG_ENDL;
            emit  Result(REQ_TYPE_STATION_SELECT,inErrCode );
            break;
        }
        case SEC_STATE_SEARCH_SELECT_REQUESTED:
        {
            // First station created
            if(E_NO_ACTIVE_STATIONS == inErrCode)
            {
                // Protocol issue , As per protocol it should not come here .
                // If coming , Please ignore status
                LOG_CRITICAL <<m_Port<<" First Station created "<< LOG_ENDL;
            }
            else
            {
                LOG_CRITICAL <<m_Port<<" CRITICAL:Search Select Failed"<< LOG_ENDL;
                emit  Result(REQ_TYPE_SEARCH_SELECT,inErrCode );
            }
            break;
        }
        case SEC_STATE_AUTO_COMPLETE_SEARCH_REQUESTED:
        {
            LOG_CRITICAL <<m_Port<<" CRITICAL:SearchAutoComplete Failed"<< LOG_ENDL;
            m_eSecPreviousState = SEC_STATE_NONE;
            emit  Result(REQ_TYPE_SEARCH_AUTO_COMPLETE,inErrCode );
            break;
        }
        case SEC_STATE_SEARCH_REQUESTED:
        {
            LOG_CRITICAL <<m_Port<<" CRITICAL:Extended Search Failed"<< LOG_ENDL;
            emit  Result(REQ_TYPE_SEARCH,inErrCode );
            break;
        }
        case SEC_STATE_STATION_DELETE_REQUESTED:
        {
            LOG_CRITICAL <<m_Port<<" CRITICAL:Station Delete Failed"<< LOG_ENDL;
            emit  Result(REQ_TYPE_DELETE_STATION,inErrCode );
            break;
        }
        case SEC_STATE_TRACK_EXPLAIN_REQUESTED:
        {
            LOG_CRITICAL <<m_Port<<" CRITICAL:Track Explanation Req Failed"<< LOG_ENDL;
            emit  Result(REQ_TYPE_EXPLAIN,inErrCode );
            break;
        }
        case SEC_STATE_STATION_LIST_REQUESTED:
        {
            //E_NO_ACTIVE_STATIONS notification comes multiple times
            // So after 1st notification has come and List request has been
            // sent, block this notification else it goes to loop
            if(E_NO_ACTIVE_STATIONS != inErrCode)
            {
                LOG_CRITICAL <<m_Port<<" CRITICAL:Station List Req Failed"<< LOG_ENDL;
                emit  Result(REQ_TYPE_STATION_LIST,inErrCode );
            }
            break;
        }
        case SEC_STATE_TRACK_INFO_REQUESTED:
        {
            LOG_CRITICAL <<m_Port<<" CRITICAL:Track Info Req Failed"<< LOG_ENDL;
            emit  Result(REQ_TYPE_TRACK_INFO,inErrCode );
            break;
        }
        default:
        {
            LOG_CRITICAL <<m_Port<<"CRITICAL:Error not handled errCode =" <<inErrCode << LOG_ENDL;
            break;
        }
    }

    m_eSecState = SEC_STATE_NONE ;

    if( m_ePriState >= PRI_STATE_PANDORA_CONNECTED && inErrCode == E_NO_STATIONS)
    {
        LOG_HIGH<<m_Port<<"OnErrorReceived() -> ClearInfo()" <<LOG_ENDL;
        m_pStCache->ClearInfo(); // Clear all previous data.
        m_pStationCtrl->ClearData();
        m_pTrackProperties->ClearData();
    }

    switch(m_eSecPreviousState)
    {
        case SEC_STATE_AUTO_COMPLETE_SEARCH_REQUESTED:
        {
            m_eSecPreviousState = SEC_STATE_NONE;
            emit  Result(REQ_TYPE_SEARCH_AUTO_COMPLETE,inErrCode );
            break;
        }
        default:
        break;
    }

    LOG_SLOT<<m_Port<<"CPndrStateHandler::OnErrorReceived:Exit" <<LOG_ENDL;
}


/**
 * Timer slot to be listened to
 * This slot is invoked when no response is received for given time
 */
void CPndrStateHandler::OnTimeout()
{
    //LOG_TRACE<< "CPndrStateHandler::OnTimeout:Entry" <<LOG_ENDL;

    //If timeout happens, emit request timeout error for the last request
    LOG_CRITICAL <<m_Port<< " CPndrStateHandler:Request Timeout Error."<< LOG_ENDL;

    OnErrorReceived(E_REQUEST_TIMEOUT);

    //LOG_TRACE<<m_Port<<"CPndrStateHandler::OnTimeout:Exit" <<LOG_ENDL;
}

/**
 * This slot is invoked when any Station Art is received
 * Sends the same station Art info to app
 */
//void CPndrStateHandler::onStationArtReceived(const TSStationArt& inStationArt)
//{
//    LOG_SLOT<<m_Port<<"CPndrStateHandler::onStationArtReceived Entry"<<LOG_ENDL;
//    emit StationArtReceived( inStationArt);
//    LOG_SLOT<<m_Port<<"CPndrStateHandler::onStationArtReceived Exit"<<LOG_ENDL;
//}

/**
 *******************************************************************
 * Cancels any outstanding request of station Art when user goes back
 * from List view
 *
 * @return  void
 *
 *******************************************************************
 */
void CPndrStateHandler::CancelStationArtRequest()
{
    LOG_TRACE<<m_Port<<"CPndrStateHandler::CancelStationArtRequest Entry"<<LOG_ENDL;

    if(m_bFetchStationArt){
        m_bFetchStationArt = false;
        m_pStationCtrl->CancelStationArtRequest();
    }

    LOG_TRACE<<m_Port<<"CPndrStateHandler::CancelStationArtRequest Exit"<<LOG_ENDL;
}

/**
 *******************************************************************
 * Kill pandora application on ipod
 * 
 *
 * @return  void
 *
 *******************************************************************
 */
void CPndrStateHandler::onPndrSessionClosed() //added by esjang 2013.06.27 for ISV #71393, ITS #167008

{
    LOG_TRACE<<m_Port<<"CPndrStateHandler::onPndrSessionClosed Entry"<<LOG_ENDL;

    m_ePriState = PRI_STATE_NONE;
    emit Result(REQ_TYPE_CONNECT, E_CHECK_PNDR_APP_ON_DEVICE);

    LOG_TRACE<<m_Port<<"CPndrStateHandler::onPndrSessionClosed Exit"<<LOG_ENDL;

}


/**
 *******************************************************************
 * Request to set the elapsed time polling
 * @param  true elapsed time polling is enabled
 *         false elapsed time polling is disabled
 * @return  void
 *
 *******************************************************************
 */
void CPndrStateHandler::SetTrackElapsedPolling(bool inEnabled)
{
    //LOG_SLOT<<m_Port<<"CPndrStateHandler::SetTrackElapsedPolling:Entry" <<LOG_ENDL;
    m_pTrackProperties->SetTrackElapsedPolling(inEnabled);
    //LOG_SLOT<<m_Port<<"CPndrStateHandler::SetTrackElapsedPolling:Exit" <<LOG_ENDL;
}

/**
 *******************************************************************
 * Returns the current sort order
 *
 * @return  ESortType the current sort type
 *
 *******************************************************************
 */
ESortType CPndrStateHandler::CurrentSortOrder()
{
   LOG_TRACE<<m_Port<<"In CPndrStateHandler::CurrentSortOrder "<<LOG_ENDL;

   return m_pStationCtrl->CurrentSortOrder();

}

/**
 *******************************************************************
 * Fetch Station Art on List Request is on
 *
 * @return  void
 *
 *******************************************************************
 */
void CPndrStateHandler::FetchStationArt()
{
    m_bFetchStationArt = true;
}

//{ added by cheolhwan 2014-04-09.
/**
 *******************************************************************
 * Returns the Primary state of Pandora
 *
 * @return  EPriState
 *
 *******************************************************************
 */
EPriState CPndrStateHandler::GetPriState()
{
    LOG_HIGH<<m_Port<<"In CPndrStateHandler::GetPriState() m_ePriState = "<<m_ePriState<<LOG_ENDL;
    return m_ePriState;
}

/**
 *******************************************************************
 * Returns the Secondary state of Pandora
 *
 * @return	ESecState
 *
 *******************************************************************
 */
ESecState CPndrStateHandler::GetSecState()
{
    LOG_HIGH<<m_Port<<"In CPndrStateHandler::GetSecState() m_eSecState = "<<m_eSecState<<LOG_ENDL;
    return m_eSecState;
}

/**
 *******************************************************************
 * Returns the Previous Secondary state of Pandora
 *
 * @return	ESecState
 *
 *******************************************************************
 */
ESecState CPndrStateHandler::GetSecPreviousState()
{
    LOG_HIGH<<m_Port<<"In CPndrStateHandler::GetSecPreviousState() = "<<m_eSecPreviousState<<LOG_ENDL;
    return m_eSecPreviousState;
}
//} added by cheolhwan 2014-04-09.


//{ added by cheolhwan 2014-04-17. To fix the unlimited receiving station. (ITS 238629)
/**
 *******************************************************************
 * Request to set the preFetchingStations flag
 *
 * @return	ESecState
 *
 *******************************************************************
 */
void CPndrStateHandler::setPreFetchingStations(bool inPreFetchingStations)
{
    LOG_HIGH<<m_Port<<"In CPndrStateHandler::setPreFetchingStations() = "<<inPreFetchingStations<<LOG_ENDL;
    m_PreFetchingStations = inPreFetchingStations;
}
//} added by cheolhwan 2014-04-17. To fix the unlimited receiving station.


//{ modified by yongkyun.lee 2014-10-07 for : Branding - Middleware
void CPndrStateHandler::onBrandingImageTimer()
{
        int readloop =0;            
        int inloop =0;
        int startPtr = 0;
        int brandingImgBufSize;
        static int tmpcnt =0;

        LOG_INFO<<"CPndrStateHandler::onBrandingImageTimer start .."   <<LOG_ENDL;
        m_BandingImageTimer->stop();

         //if(m_pMainEngine->getModeStateHistoryCurr() == MODE_STATE_BT_PANDORA)
             brandingImgBufSize = BANDIMAG_SEND_SIZE_2;
         //else
         //    brandingImgBufSize = BANDIMAG_SEND_SIZE;
         
         if(m_Brandingfile == NULL)
         {
             m_Brandingfile =  new QFile();
             QDir::setCurrent(m_BrandingImagePath);
             m_Brandingfile->setFileName(m_BrandingImageName);

         }
         if( m_BrandingfileSize < 0 )
         {
             LOG_HIGH<< "CPndrStateHandler::onBrandingImageTimer() -> Error = " <<  m_BrandingfileSize <<LOG_ENDL;        
             return;
         }
         
         if(!m_Brandingfile->isOpen())
         {
             if( !m_Brandingfile->exists() )
             {
                 LOG_HIGH<< "The file " << m_Brandingfile->fileName() << " does not exist." <<LOG_ENDL;        
                 m_BrandingfileSize = -2;
                 return ;
             }
             if (!m_Brandingfile->open( QIODevice::ReadOnly ))
             {
                 LOG_HIGH<<"CPndrStateHandler::onBrandingImageTimer file Open error"   <<LOG_ENDL;                 
                 m_BrandingfileSize = -1;
                 return;
             }
             m_BrandingfileSize = m_Brandingfile->size() ;
             if(m_BrandingBuf!= NULL)
             {
               delete[] m_BrandingBuf;  // modified by wonseok.heo static test issue 2014.10.01
               //delete m_Brandingfile;
             }
             m_BrandingBuf = new char [m_BrandingfileSize];
             //this->m_Brandingfile->read(m_BrandingBuf,m_BrandingfileSize); // modified by wonseok.heo static test issue 2014.10.01
             m_Brandingfile->read(m_BrandingBuf,m_BrandingfileSize);
        }

        startPtr = (m_BrandingImageSegIndex*brandingImgBufSize);

        if((m_BrandingfileSize < startPtr) || 
           (m_BrandingImageSegIndex > (m_BrandingfileSize/brandingImgBufSize)+1) )
        {
            LOG_HIGH<<"CPndrStateHandler::onBrandingImage END.... Success!!:: brandingImageSize =" << brandingImgBufSize <<", tmpcnt=" << tmpcnt <<LOG_ENDL;                 
            m_Brandingfile->close () ;
            delete[] m_BrandingBuf;
            m_BrandingBuf = NULL;
            delete m_Brandingfile;
            m_Brandingfile = NULL;
            return;
        }

        memset(inData,0x00,brandingImgBufSize+2);
        for(readloop= startPtr ; (readloop < (startPtr + brandingImgBufSize ) ) ; readloop++)
        {
             if(m_BrandingfileSize <= readloop  )
                break;
             inData[inloop]= m_BrandingBuf[readloop];
             inloop++;
        } 

        LOG_INFO<<"m_BrandingImageSegIndex=" << m_BrandingImageSegIndex << ", total =" << (m_BrandingfileSize/brandingImgBufSize) <<  " , file size =" << m_BrandingfileSize << " , Send byte =" << inloop <<LOG_ENDL;
        //m_PndrInterface->RequestBrandingSegment((quint8)m_BrandingImageSegIndex , (m_BrandingfileSize/brandingImgBufSize)+1 ,(quint8*)inData , inloop );        
        m_pTrackProperties->RequestBrandingSegment((quint8)m_BrandingImageSegIndex , (m_BrandingfileSize/brandingImgBufSize)+1 ,(quint8*)inData , inloop);
        m_BrandingImageSegIndex ++;
        tmpcnt++;
    return;
}
//{ modified by yongkyun.lee 2014-08-25


//} modified by yongkyun.lee 2014-10-07 

//End Of File

