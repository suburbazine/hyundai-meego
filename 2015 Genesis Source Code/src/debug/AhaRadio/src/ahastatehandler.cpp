//Suryanto Tan: suryanto.tan@lge.com
//this functions will take care of all transition logic.
//it decides what to do when

#include "ahastatehandler.h"
#include "ahaImageUtility.h"
#include "DHAVN_AppAha_QmlProperty.h"
#include "DHAVN_AppAhaTrack.h"

CAhaStateHandler::CAhaStateHandler()
{
    LOG_INIT_V2("Aha");

    m_PriState = AHA_PRI_NONE_STATE;
    m_nRequestedContentID = 0;

    //timer
    m_AppLaunchWaitTimer = NULL;
    m_StartPeerDelayTimer = NULL;
    m_StartPeerResponseTimer = NULL;
    m_QueryResponseTimer = NULL;
//wsuk.kim 130904 ITS_0182092 repeat buffering
    m_BufferingintervalTimer = NULL;
    m_DelayedUnmuteTimer = NULL;

    m_nBufferingCount = 0;
    m_nNetworkStatus = 0/*AHA_NOTIFY_RESUME_NORMAL*/;
//wsuk.kim 130904 ITS_0182092 repeat buffering

//    m_AhaUSBFrameTxRx = new CAhaUSBFrameTxRx;

    m_nDeviceType = DEVICE_TYPE_NONE;

    m_AhaBTFrameTxRx = NULL;
    m_AhaUSBFrameTxRx = NULL;
    m_AhaActiveFrameTxRx = NULL;

//    m_RetryStartPeerSessionTimer = NULL;
    m_nLikeSelected = 0;
    m_nDislikeSelected = 0;
}

void CAhaStateHandler::connectActiveTxRxSlots()
{
    disconnectActiveTxRxSlots();
    connect(m_AhaActiveFrameTxRx, SIGNAL(ahaAppInitCompleted()),
            this, SLOT(ahaAppInitCompletedSlot()));
    connect(m_AhaActiveFrameTxRx, SIGNAL(ahaAppInitTimeOut()),
            this, SLOT(ahaAppInitTimeOutSlot()));
    connect(m_AhaActiveFrameTxRx, SIGNAL(ahaPingTimeOut()),
            this, SLOT(ahaPingTimeOutSlot()));
    connect(m_AhaActiveFrameTxRx, SIGNAL(ahaSessionClosed()),
            this, SLOT(ahaSessionClosedSlot()));
    connect(m_AhaActiveFrameTxRx, SIGNAL(ahaSessionOpened()),
            this, SLOT(ahaSessionOpenedSlot()));
    connect(m_AhaActiveFrameTxRx, SIGNAL(ahaAppNotOpened()),
            this, SLOT(ahaAppNotOpenedSlot()));
    connect(m_AhaActiveFrameTxRx, SIGNAL(ahaSampleRateChanged(int)),
            this, SIGNAL(ahaSampleRateChanged(int)));
}

void CAhaStateHandler::disconnectActiveTxRxSlots()
{
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(ahaAppInitCompleted()),
            this, SLOT(ahaAppInitCompletedSlot()));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(ahaAppInitTimeOut()),
            this, SLOT(ahaAppInitTimeOutSlot()));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(ahaPingTimeOut()),
            this, SLOT(ahaPingTimeOutSlot()));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(ahaSessionClosed()),
            this, SLOT(ahaSessionClosedSlot()));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(ahaSessionOpened()),
            this, SLOT(ahaSessionOpenedSlot()));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(ahaAppNotOpened()),
            this, SLOT(ahaAppNotOpenedSlot()));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(ahaSampleRateChanged(int)),
            this, SIGNAL(ahaSampleRateChanged(int)));
}


void CAhaStateHandler::connectImageRelatedSlots()
{
    disconnectImageRelatedSlots();
    connect(m_AhaActiveFrameTxRx, SIGNAL(albumArtProcessFinished(QString&)),
            this, SLOT(onAlbumArtProcessFinishedSlot(QString&)));
    connect(this, SIGNAL(albumArtProcessFinished(QString&)),
            this, SLOT(onAlbumArtProcessFinishedSlot(QString&)));

    connect(m_AhaActiveFrameTxRx, SIGNAL(stationLogoProcessFinished(QString&)),
            this, SLOT(onStationLogoProcessFinishedSlot(QString&)));
    connect(this, SIGNAL(stationLogoProcessFinished(QString&)),
            this, SLOT(onStationLogoProcessFinishedSlot(QString&)));

    connect(m_AhaActiveFrameTxRx, SIGNAL(partnerLogoProcessFinished(QString&)),
            this, SLOT(onPartnerLogoProcessFinishedSlot(QString&)));
    connect(this, SIGNAL(partnerLogoProcessFinished(QString&)),
            this, SLOT(onPartnerLogoProcessFinishedSlot(QString&)));

    connect(m_AhaActiveFrameTxRx, SIGNAL(presetLogoProcessFinished(QString&)),
            this, SLOT(onPresetLogoProcessFinishedSlot(QString&)));
    connect(this, SIGNAL(presetLogoProcessFinished(QString&)),
            this, SLOT(onPresetLogoProcessFinishedSlot(QString&)));

    connect(m_AhaActiveFrameTxRx, SIGNAL(ratingImageProcessFinished(QString&)),
            this, SLOT(onRatingImageProcessFinishedSlot(QString&)));
    connect(this, SIGNAL(ratingImageProcessFinished(QString&)),
            this, SLOT(onRatingImageProcessFinishedSlot(QString&)));

    connect(m_AhaActiveFrameTxRx, SIGNAL(likeImageProcessFinished(QString&)),
            this, SLOT(onLikeImageProcessFinishedSlot(QString&)));
    connect(this, SIGNAL(likeImageProcessFinished(QString&)),
            this, SLOT(onLikeImageProcessFinishedSlot(QString&)));
}

void CAhaStateHandler::disconnectImageRelatedSlots()
{
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(albumArtProcessFinished(QString&)),
            this, SLOT(onAlbumArtProcessFinishedSlot(QString&)));
    disconnect(this, SIGNAL(albumArtProcessFinished(QString&)),
            this, SLOT(onAlbumArtProcessFinishedSlot(QString&)));

    disconnect(m_AhaActiveFrameTxRx, SIGNAL(stationLogoProcessFinished(QString&)),
            this, SLOT(onStationLogoProcessFinishedSlot(QString&)));
    disconnect(this, SIGNAL(stationLogoProcessFinished(QString&)),
            this, SLOT(onStationLogoProcessFinishedSlot(QString&)));

    disconnect(m_AhaActiveFrameTxRx, SIGNAL(partnerLogoProcessFinished(QString&)),
            this, SLOT(onPartnerLogoProcessFinishedSlot(QString&)));
    disconnect(this, SIGNAL(partnerLogoProcessFinished(QString&)),
            this, SLOT(onPartnerLogoProcessFinishedSlot(QString&)));

    disconnect(m_AhaActiveFrameTxRx, SIGNAL(presetLogoProcessFinished(QString&)),
            this, SLOT(onPresetLogoProcessFinishedSlot(QString&)));
    disconnect(this, SIGNAL(presetLogoProcessFinished(QString&)),
            this, SLOT(onPresetLogoProcessFinishedSlot(QString&)));

    disconnect(m_AhaActiveFrameTxRx, SIGNAL(ratingImageProcessFinished(QString&)),
            this, SLOT(onRatingImageProcessFinishedSlot(QString&)));
    disconnect(this, SIGNAL(ratingImageProcessFinished(QString&)),
            this, SLOT(onRatingImageProcessFinishedSlot(QString&)));

    disconnect(m_AhaActiveFrameTxRx, SIGNAL(likeImageProcessFinished(QString&)),
            this, SLOT(onLikeImageProcessFinishedSlot(QString&)));
    disconnect(this, SIGNAL(likeImageProcessFinished(QString&)),
            this, SLOT(onLikeImageProcessFinishedSlot(QString&)));
}

void CAhaStateHandler::connectDecodedSlots()
{
    disconnectDecodedSlots();
    connect(m_AhaActiveFrameTxRx, SIGNAL(startPeerSessionDecoded(uint16_t)),
            this, SLOT(startPeerSessionDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(queryStationsDecoded(uint16_t)),
            this, SLOT(queryStationsDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(queryStationContentDecoded(uint16_t)),
            this, SLOT(queryStationContentDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(activeStationParamDecoded(uint16_t)),
            this, SLOT(activeStationParamDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(presetStationParamDecoded(uint16_t, int)),
            this, SLOT(presetStationParamDecodedSlot(uint16_t, int)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(activeContentParamDecoded(uint16_t)),
            this, SLOT(activeContentParamDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(listContentParamDecoded(uint16_t, int)),
            this, SLOT(listContentParamDecodedSlot(uint16_t, int)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(queryAlbumArtDecoded(uint16_t)),
            this, SLOT(queryAlbumArtDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(queryStationLogoDecoded(uint16_t)),
            this, SLOT(queryStationLogoDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(queryPartnerLogoDecoded(uint16_t)),
            this, SLOT(queryPartnerLogoDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(queryPresetLogoDecoded(uint16_t)),
            this, SLOT(queryPresetLogoDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(queryRatingImageDecoded(uint16_t)),
            this, SLOT(queryRatingImageDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(queryLikeImageDecoded(uint16_t)),
            this, SLOT(queryLikeImageDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(queryCurrentStationDecoded(uint16_t)),
            this, SLOT(queryCurrentStationDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(changeClientPlaybackDecoded(uint16_t)),
            this, SLOT(changeClientPlaybackDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(timeShiftContentDecoded(uint16_t)),
            this, SLOT(timeShiftContentDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(performActionDecoded(uint16_t)),
            this, SLOT(performActionDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(selectStationForPlaybackDecoded(uint16_t)),
            this, SLOT(selectStationForPlaybackDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(selectContentForPlaybackDecoded(uint16_t)),
            this, SLOT(selectContentForPlaybackDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(currentContentInfoDecoded(uint16_t)),
            this, SLOT(currentContentInfoDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(notifyPlaybackOfNewStationDecoded(uint16_t)),
            this, SLOT(notifyPlaybackOfNewStationDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(notifyNewContentPlaybackStateDecoded(uint16_t)),
            this, SLOT(notifyNewContentPlaybackStateDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(notifyNewContentAvailableDecoded(uint16_t, uint64_t)),
            this, SLOT(notifyNewContentAvailableDecodedSlot(uint16_t, uint64_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(notifyContentDeletedDecoded(uint16_t)),
            this, SLOT(notifyContentDeletedDecodedSlot(uint16_t)));
    connect(m_AhaActiveFrameTxRx, SIGNAL(notifyClientStatusUpdateDecoded(uint16_t, uint32_t)),
            this, SLOT(notifyClientStatusUpdateDecodedSlot(uint16_t, uint32_t)));
}

void CAhaStateHandler::disconnectDecodedSlots()
{
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(startPeerSessionDecoded(uint16_t)),
            this, SLOT(startPeerSessionDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(queryStationsDecoded(uint16_t)),
            this, SLOT(queryStationsDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(queryStationContentDecoded(uint16_t)),
            this, SLOT(queryStationContentDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(activeStationParamDecoded(uint16_t)),
            this, SLOT(activeStationParamDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(presetStationParamDecoded(uint16_t, int)),
            this, SLOT(presetStationParamDecodedSlot(uint16_t, int)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(activeContentParamDecoded(uint16_t)),
            this, SLOT(activeContentParamDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(listContentParamDecoded(uint16_t, int)),
            this, SLOT(listContentParamDecodedSlot(uint16_t, int)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(queryAlbumArtDecoded(uint16_t)),
            this, SLOT(queryAlbumArtDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(queryStationLogoDecoded(uint16_t)),
            this, SLOT(queryStationLogoDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(queryPartnerLogoDecoded(uint16_t)),
            this, SLOT(queryPartnerLogoDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(queryPresetLogoDecoded(uint16_t)),
            this, SLOT(queryPresetLogoDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(queryRatingImageDecoded(uint16_t)),
            this, SLOT(queryRatingImageDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(queryLikeImageDecoded(uint16_t)),
            this, SLOT(queryLikeImageDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(queryCurrentStationDecoded(uint16_t)),
            this, SLOT(queryCurrentStationDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(changeClientPlaybackDecoded(uint16_t)),
            this, SLOT(changeClientPlaybackDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(timeShiftContentDecoded(uint16_t)),
            this, SLOT(timeShiftContentDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(performActionDecoded(uint16_t)),
            this, SLOT(performActionDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(selectStationForPlaybackDecoded(uint16_t)),
            this, SLOT(selectStationForPlaybackDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(selectContentForPlaybackDecoded(uint16_t)),
            this, SLOT(selectContentForPlaybackDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(currentContentInfoDecoded(uint16_t)),
            this, SLOT(currentContentInfoDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(notifyPlaybackOfNewStationDecoded(uint16_t)),
            this, SLOT(notifyPlaybackOfNewStationDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(notifyNewContentPlaybackStateDecoded(uint16_t)),
            this, SLOT(notifyNewContentPlaybackStateDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(notifyNewContentAvailableDecoded(uint16_t, uint64_t)),
            this, SLOT(notifyNewContentAvailableDecodedSlot(uint16_t, uint64_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(notifyContentDeletedDecoded(uint16_t)),
            this, SLOT(notifyContentDeletedDecodedSlot(uint16_t)));
    disconnect(m_AhaActiveFrameTxRx, SIGNAL(notifyClientStatusUpdateDecoded(uint16_t, uint32_t)),
            this, SLOT(notifyClientStatusUpdateDecodedSlot(uint16_t, uint32_t)));
}

CAhaStateHandler::~CAhaStateHandler()
{
    LOG_HIGH<<"[ CAhaStateHandler::~CAhaStateHandler() ]"<<LOG_ENDL;
    Disconnect();

    m_AhaActiveFrameTxRx = NULL;

    if(m_AhaUSBFrameTxRx)
        delete m_AhaUSBFrameTxRx;
    m_AhaUSBFrameTxRx = NULL;

    if(m_AhaBTFrameTxRx)
        delete m_AhaBTFrameTxRx;
    m_AhaBTFrameTxRx = NULL;

    if(m_AppLaunchWaitTimer)
    {
        delete m_AppLaunchWaitTimer;
        m_AppLaunchWaitTimer = NULL;
    }
    if(m_StartPeerResponseTimer)
    {
        delete m_StartPeerResponseTimer;
        m_StartPeerResponseTimer = NULL;
    }
    if(m_StartPeerDelayTimer)
    {
        delete m_StartPeerDelayTimer;
        m_StartPeerDelayTimer = NULL;
    }
    if(m_BufferingintervalTimer)    //wsuk.kim 130904 ITS_0182092 repeat buffering
    {
        delete m_BufferingintervalTimer;
        m_BufferingintervalTimer = NULL;
    }
    if(m_QueryResponseTimer)
    {
        delete m_QueryResponseTimer;
        m_QueryResponseTimer = NULL;
    }
    if(m_DelayedUnmuteTimer)
    {
        delete m_DelayedUnmuteTimer;
        m_DelayedUnmuteTimer = NULL;
    }

    m_PriState = AHA_PRI_NONE_STATE;
}

void CAhaStateHandler::createTimers()
{
    //We wait a few seconds for application to launch,
    //if it does not launch, we should handle it.
    if(m_AppLaunchWaitTimer)
        delete m_AppLaunchWaitTimer;
    m_AppLaunchWaitTimer = new QTimer(this);
    if(m_AppLaunchWaitTimer)
    {
        m_AppLaunchWaitTimer->setSingleShot(true);
        connect(m_AppLaunchWaitTimer, SIGNAL(timeout()), this, SLOT(appLaunchTimeOutSlot()));
    }

    //once we ask to startPeerSession, we expect a response.
    //if no response come, then there is a problem in the connection.
    if(m_StartPeerResponseTimer)
        delete m_StartPeerResponseTimer;
    m_StartPeerResponseTimer = new QTimer(this);
    if(m_StartPeerResponseTimer)
    {
        m_StartPeerResponseTimer->setSingleShot(true);
        connect(m_StartPeerResponseTimer, SIGNAL(timeout()), this, SLOT(startPeerResponseTimeOutSlot()));
    }

    //We wait a few seconds before calling the start peer session.
    //this is to give a space between start peer retry.
    if(m_StartPeerDelayTimer)
        delete m_StartPeerDelayTimer;
    m_StartPeerDelayTimer = new QTimer(this);
    if(m_StartPeerDelayTimer)
    {
        m_StartPeerDelayTimer->setSingleShot(true);
        connect(m_StartPeerDelayTimer, SIGNAL(timeout()), this, SLOT(peerDelayTimeOutSlot()));
    }

//wsuk.kim 130904 ITS_0182092 repeat buffering
    if(m_BufferingintervalTimer)
        delete m_BufferingintervalTimer;

    m_BufferingintervalTimer = new QTimer(this);
    if(m_BufferingintervalTimer)
    {
        m_BufferingintervalTimer->setSingleShot(true);
        connect(m_BufferingintervalTimer, SIGNAL(timeout()), this, SLOT(bufferIntervalTimeOutSlot()));
    }
//wsuk.kim 130904 ITS_0182092 repeat buffering

    if(m_QueryResponseTimer)
            delete m_QueryResponseTimer;
    m_QueryResponseTimer = new QTimer(this);
    if(m_QueryResponseTimer)
    {
        m_QueryResponseTimer->setSingleShot(true);
        connect(m_QueryResponseTimer, SIGNAL(timeout()), this, SLOT(queryResponseTimeOutSlot()));
    }

    if(m_DelayedUnmuteTimer)
        delete m_DelayedUnmuteTimer;
    m_DelayedUnmuteTimer = new QTimer(this);
    if(m_DelayedUnmuteTimer)
    {
        m_DelayedUnmuteTimer->setSingleShot(true);
        connect(m_DelayedUnmuteTimer, SIGNAL(timeout()), this, SLOT(delayUnmuteTimeOutSlot()) );
    }
}


void CAhaStateHandler::destroyTimers()
{
    if(m_AppLaunchWaitTimer){
        disconnect(m_AppLaunchWaitTimer, SIGNAL(timeout()), this, SLOT(appLaunchTimeOutSlot()));
        delete m_AppLaunchWaitTimer;
        m_AppLaunchWaitTimer = NULL;
    }
    if(m_StartPeerResponseTimer){
        disconnect(m_StartPeerResponseTimer, SIGNAL(timeout()), this, SLOT(startPeerResponseTimeOutSlot()));
        delete m_StartPeerResponseTimer;
        m_StartPeerResponseTimer = NULL;
    }
    if(m_StartPeerDelayTimer){
        disconnect(m_StartPeerDelayTimer, SIGNAL(timeout()), this, SLOT(peerDelayTimeOutSlot()));
        delete m_StartPeerDelayTimer;
        m_StartPeerDelayTimer = NULL;
    }
    if(m_BufferingintervalTimer){
        disconnect(m_BufferingintervalTimer, SIGNAL(timeout()), this, SLOT(bufferIntervalTimeOutSlot()));
        delete m_BufferingintervalTimer;
        m_BufferingintervalTimer = NULL;
    }
    if(m_QueryResponseTimer){
        disconnect(m_QueryResponseTimer, SIGNAL(timeout()), this, SLOT(queryResponseTimeOutSlot()));
        delete m_QueryResponseTimer;
        m_QueryResponseTimer = NULL;
    }
    if(m_DelayedUnmuteTimer)
    {
        disconnect(m_DelayedUnmuteTimer, SIGNAL(timeout()), this, SLOT(delayUnmuteTimeOutSlot()) );
        delete m_DelayedUnmuteTimer;
        m_DelayedUnmuteTimer = NULL;
    }
}

EAhaError CAhaStateHandler::setDeviceType(EDeviceType inDeviceType)
{
    LOG_HIGH << "CAhaStateHandler::setDeviceType :" << inDeviceType <<LOG_ENDL;

    EAhaError nAhaError = E_INVALID_STATE;
    switch(inDeviceType)
    {
    case DEVICE_TYPE_IPHONE_USB_FRONT:
    case DEVICE_TYPE_IPHONE_USB_REAR:

        LOG_HIGH << "CAhaStateHandler::setDeviceType 11111:" << inDeviceType <<LOG_ENDL;
        if(m_AhaUSBFrameTxRx != NULL)
        {
            LOG_HIGH << "CAhaStateHandler::setDeviceType 2222222:" << inDeviceType <<LOG_ENDL;
            m_AhaActiveFrameTxRx = m_AhaUSBFrameTxRx;
            nAhaError = m_AhaUSBFrameTxRx->Init();
            break;
        }

        LOG_HIGH << "CAhaStateHandler::setDeviceType 33333333333:" << inDeviceType <<LOG_ENDL;
        m_AhaUSBFrameTxRx = new CAhaUSBFrameTxRx();
        if(m_AhaUSBFrameTxRx == NULL)
        {
            LOG_HIGH << "CAhaStateHandler::setDeviceType m_AhaUSBFrameTxRx == NULL:" << inDeviceType <<LOG_ENDL;
           break;
        }
        LOG_HIGH << "CAhaStateHandler::setDeviceType xxxxxxxxxxx:" << inDeviceType <<LOG_ENDL;
        nAhaError = m_AhaUSBFrameTxRx->Init();
        LOG_HIGH << "CAhaStateHandler::setDeviceType 555555 :" << inDeviceType <<LOG_ENDL;
        m_AhaActiveFrameTxRx = m_AhaUSBFrameTxRx;
        LOG_HIGH << "CAhaStateHandler::setDeviceType :" << inDeviceType <<LOG_ENDL;

        m_nDeviceType = inDeviceType;
        break;
    case DEVICE_TYPE_SMARTPHONE:
        LOG_HIGH << "CAhaStateHandler::setDeviceType DEVICE_TYPE_SMARTPHONE 11111111111111"  <<LOG_ENDL;
        if(m_AhaBTFrameTxRx != NULL)
        {
            LOG_HIGH << "CAhaStateHandler::setDeviceType DEVICE_TYPE_SMARTPHONE not null..."  <<LOG_ENDL;
            m_AhaActiveFrameTxRx = m_AhaBTFrameTxRx;
            nAhaError = E_SUCCESS;
            break;
        }
        LOG_HIGH << "CAhaStateHandler::setDeviceType 2222222222222222"  <<LOG_ENDL;

        m_AhaBTFrameTxRx = new CAhaBTFrameTxRx();
        if(m_AhaBTFrameTxRx == NULL)
        {
            LOG_HIGH << "CAhaStateHandler::setDeviceType 3333333333333"  <<LOG_ENDL;
            break;
        }

        //BT Phone is connected. So IniT BT.
        nAhaError = m_AhaBTFrameTxRx->Init();
        LOG_INFO<< "CAhaStateHandler::Connect BT... 11111" <<LOG_ENDL;

        m_AhaActiveFrameTxRx = m_AhaBTFrameTxRx;
        LOG_HIGH << "CAhaStateHandler::setDeviceType :" << inDeviceType <<LOG_ENDL;

        m_nDeviceType = inDeviceType;
        break;
    default:
        nAhaError = E_DEVICE_NOT_FOUND;
        break;
    }

    return nAhaError;
}

EAhaError CAhaStateHandler::Connect(EDeviceType inDeviceType)
{
    LOG_HIGH << "CAhaStateHandler::Connect inDeviceType = " << inDeviceType << LOG_ENDL;
    createTimers();

    EAhaError nAhaError = E_INVALID_STATE;

    m_PriState = AHA_PRI_NONE_STATE;
    m_nLikeSelected = 0;
    m_nDislikeSelected = 0;

    nAhaError = setDeviceType(inDeviceType);

    if(E_SUCCESS == nAhaError)
    {
        LOG_HIGH<<"E_SUCCESS == nAhaError connect..."<<LOG_ENDL;
        connectDecodedSlots();
        connectImageRelatedSlots();
        connectActiveTxRxSlots();

        nAhaError = m_AhaActiveFrameTxRx->Connect(inDeviceType);
    }

    return nAhaError;
}

int CAhaStateHandler::Disconnect()
{    
    LOG_HIGH << "[ CAhaStateHandler::Disconnect ] %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << LOG_ENDL;
    //if(USB)
    m_PriState = AHA_PRI_NONE_STATE;
    m_nLikeSelected = 0;
    m_nDislikeSelected = 0;
    m_nBufferingCount = 0;  //wsuk.kim 130904 ITS_0182092 repeat buffering
    m_nNetworkStatus = 0 /*AHA_NOTIFY_RESUME_NORMAL*/;  //wsuk.kim 130904 ITS_0182092 repeat buffering

//hsryu_0612_update_optimize
    m_PrevTrackInfo.CurrentStation.clear();
    m_PrevTrackInfo.Artist.clear();
    m_PrevTrackInfo.Album.clear();
    m_PrevTrackInfo.Title.clear();

    destroyTimers();

    if(m_AhaActiveFrameTxRx)
    {
        disconnectDecodedSlots();
        disconnectImageRelatedSlots();
        disconnectActiveTxRxSlots();

        m_AhaActiveFrameTxRx->Disconnect();
        m_AhaActiveFrameTxRx->DeInit();
        if(DEVICE_TYPE_SMARTPHONE == getDeviceType())
        {
            delete m_AhaBTFrameTxRx;
            m_AhaBTFrameTxRx = NULL;
            LOG_HIGH<<"@@@@@@@@@@@@@@@@@@@@@@@@@@"<<LOG_ENDL;
            m_nDeviceType = DEVICE_TYPE_NONE;
            m_AhaActiveFrameTxRx = NULL;
        }
    }

    m_bPeerConnected = false;

    LOG_HIGH << "[ CAhaStateHandler::Disconnect ] %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% out.." << LOG_ENDL;
    return 0;
}

void CAhaStateHandler::startPeerSession(bool bResetCount, int nDelayTime)
{
    LOG_HIGH << "CAhaStateHandler::startPeerSession" << "  nDelayTime = " << nDelayTime << LOG_ENDL;
    //we always delay for 4 second before we attempt anything
    static int nPeerRetryCount = 0;
    if(bResetCount)
    {
        nPeerRetryCount = 0;
    }
    if(nPeerRetryCount < MAX_PEER_RETRY )
    {
        nPeerRetryCount++;
        LOG_HIGH <<"nPeerRetryCount %d\n"<< nPeerRetryCount << LOG_ENDL;
        m_StartPeerDelayTimer->start(nDelayTime);//start peer session 4 seconds later
    }
    else
    {
        LOG_HIGH <<"maximum trial reached, reset the controller.\n"<< LOG_ENDL;
        emit Result(REQ_TYPE_CONNECT, E_AHA_APP_NOT_OPENED);
    }
}

void CAhaStateHandler::queryingPresetStations()
{
    LOG_HIGH << "CAhaStateHandler::queryingPresetStations" << LOG_ENDL;
    DEBUG_STATEHANDLER("Tan: queryingPresetStations \n");

    //only if the preset has been loaded.
    m_PriState = AHA_PRI_PRESET_QUERYING_STATE;
    m_AhaActiveFrameTxRx->queryPresetStationIDs();
}

void CAhaStateHandler::queryingContentList()
{
    LOG_HIGH << "CAhaStateHandler::queryingContentList" << LOG_ENDL;
    LOG_HIGH <<"Tan: queryingContentList \n" << LOG_ENDL;
    //only if the preset has been loaded.
    m_AhaActiveFrameTxRx->queryStationContentIDs();
}

void CAhaStateHandler::queryingActiveStation()
{
    LOG_HIGH << "CAhaStateHandler::queryingActiveStation" << LOG_ENDL;
    LOG_HIGH <<"Tan: queryingActiveStation \n" << LOG_ENDL;

    m_AhaActiveFrameTxRx->queryActiveStationParam();
}

void CAhaStateHandler::selectContentToPlay(uint64_t nContentID)
{
    LOG_HIGH << "CAhaStateHandler::selectContentToPlay" << "  nContentID = " << nContentID << LOG_ENDL;
    m_AhaActiveFrameTxRx->querySelectContentForPlayback(nContentID);
}

void CAhaStateHandler::selectStationToPlay(uint64_t nStationID)
{
    LOG_HIGH << "CAhaStateHandler::selectStationToPlay" << "  nStationID = " << nStationID << LOG_ENDL;
    //hsryu_0612_update_optimize
    if(m_AhaActiveFrameTxRx->m_TopProcessPlayStation == true) //do not receive station info
    {
        //so clear
        m_PrevTrackInfo.CurrentStation.clear();
        m_PrevTrackInfo.Artist.clear();
        m_PrevTrackInfo.Album.clear();
        m_PrevTrackInfo.Title.clear();
    }
    m_PriState = AHA_PRI_STATION_CHANGING_STATE;
    m_AhaActiveFrameTxRx->querySelectStationForPlayback(nStationID);
}

void CAhaStateHandler::queryingActiveContent(bool bClearTrackDisplay)
{
    LOG_HIGH << "CAhaStateHandler::queryingActiveContent" << "  bClearTrackDisplay = " << bClearTrackDisplay << LOG_ENDL;
    AhaClientInfo_T* pAhaClientInfo;
    AhaStationInfo_T* pAhaStationInfo;
    uint64_t nActiveContentID;

    //LOG_HIGH <<"Statehandler: queryingActiveContent  \n" << LOG_ENDL;

    pAhaClientInfo = m_AhaActiveFrameTxRx->getAhaClientInfo();
    pAhaStationInfo = m_AhaActiveFrameTxRx->getAhaStationInfo();
    nActiveContentID = pAhaClientInfo->nActiveContentID;

    m_PriState = AHA_PRI_NONE_STATE;

    //cancel any image download if any
    cancelTrackViewImageDownload();

    if(nActiveContentID !=0)
    {
        LOG_HIGH <<"Statehandler: we have active content \n" << LOG_ENDL;
        m_AhaActiveFrameTxRx->queryCurrentContentDisplayInfo();
    }
    else
    {
        LOG_HIGH <<"Statehandler: waiting for activeContent  \n" << LOG_ENDL;
        //I will clear up every track info
        emit TrackUpdated(1);
        LOG_HIGH << "CAhaStateHandler::queryingActiveContent " << "  TrackUpdated " << LOG_ENDL;
        clearCurrentTrackInfo();
        clearContentImagesDisplay();
        m_CurrentTrackInfo.CurrentStation = pAhaStationInfo->strStationFullName;
        emit TrackInfo(m_CurrentTrackInfo);
    }
}

void CAhaStateHandler::cancelTrackViewImageDownload()
{
    LOG_HIGH << "CAhaStateHandler::cancelTrackViewImageDownload" << LOG_ENDL;
    m_AhaActiveFrameTxRx->cancelBinaryImageDownload(QUERY_MODE_ALBUM_ART);
    m_AhaActiveFrameTxRx->cancelBinaryImageDownload(QUERY_MODE_PARTNER_LOGO);
    m_AhaActiveFrameTxRx->cancelBinaryImageDownload(QUERY_MODE_STATION_LOGO);
    m_AhaActiveFrameTxRx->cancelBinaryImageDownload(QUERY_MODE_LIKE_IMAGE);
    m_AhaActiveFrameTxRx->cancelBinaryImageDownload(QUERY_MODE_RATING_IMAGE);
}

void CAhaStateHandler::clearTrackViewDisplay()
{
    LOG_HIGH << "CAhaStateHandler::clearTrackViewDisplay" << LOG_ENDL;
    //partner logo, station logo, station title
    QString strTemp;
    strTemp.clear();
    clearCurrentTrackInfo();
    emit StationLogo(strTemp);
    emit PartnerLogo(strTemp);
    emit TrackInfo(m_CurrentTrackInfo);
    clearContentImagesDisplay();
}

void CAhaStateHandler::clearContentImagesDisplay()
{
    LOG_HIGH << "CAhaStateHandler::clearContentImagesDisplay" << LOG_ENDL;
    QString strTemp;
    strTemp.clear();
    emit AlbumArt(strTemp);
    emit RatingImage(strTemp);
    emit LikeImage(AHA_IMAGE_LIKE_S, strTemp);
    emit LikeImage(AHA_IMAGE_LIKE_U, strTemp);
    emit LikeImage(AHA_IMAGE_DISLIKE_S, strTemp);
    emit LikeImage(AHA_IMAGE_DISLIKE_U, strTemp);
    ShowCurrentLikeDislikeIcon();
}

void CAhaStateHandler::clearCurrentTrackInfo()
{
    LOG_HIGH << "CAhaStateHandler::clearCurrentTrackInfo" << LOG_ENDL;
    m_CurrentTrackInfo.Artist.clear();
    m_CurrentTrackInfo.Album.clear();
    m_CurrentTrackInfo.Title.clear();
    m_CurrentTrackInfo.CurrentStation.clear();
    m_CurrentTrackInfo.bAllowSkip = 0;
    m_CurrentTrackInfo.bAllowSkipBack = 0;
    m_CurrentTrackInfo.bAllowTimeShift = 0;
    m_CurrentTrackInfo.bAllowLike = 0;
    m_CurrentTrackInfo.bAllowDislike = 0;
    m_CurrentTrackInfo.bAllowNavigate = 0;
    m_CurrentTrackInfo.bAllowCall = 0;
}

//hsryu_0502_factory_reset
void CAhaStateHandler::ReadyForClearCache()
{
    LOG_HIGH << "CAhaStateHandler::ReadyForClearCache" << LOG_ENDL;
    if(m_AhaActiveFrameTxRx != NULL)
    {
        LOG_HIGH << "CAhaStateHandler::ReadyForClearCache m_AhaActiveFrameTxRx != NULL" << LOG_ENDL;
        m_AhaActiveFrameTxRx->cancelBinaryImageDownload(QUERY_MODE_ALBUM_ART);
        m_AhaActiveFrameTxRx->cancelBinaryImageDownload(QUERY_MODE_PARTNER_LOGO);
        m_AhaActiveFrameTxRx->cancelBinaryImageDownload(QUERY_MODE_STATION_LOGO);
        m_AhaActiveFrameTxRx->cancelBinaryImageDownload(QUERY_MODE_PRESET_LOGO);
        m_AhaActiveFrameTxRx->cancelBinaryImageDownload(QUERY_MODE_RATING_IMAGE);
        m_AhaActiveFrameTxRx->cancelBinaryImageDownload(QUERY_MODE_LIKE_IMAGE);
    }
    else
    {
        LOG_HIGH << "CAhaStateHandler::ReadyForClearCache m_AhaActiveFrameTxRx == NULL" << LOG_ENDL;
    }
}

void CAhaStateHandler::ClearCacheData()
{
    LOG_HIGH << "CAhaStateHandler::ClearCacheData" << LOG_ENDL;
    clearPresetLogoDirectory();
    clearStationLogoDirectory();
    clearContentImagesDirectory();
    clearPartnerLogoDirectory();
    clearAlbumArtDirectory();
}
//hsryu_0502_factory_reset

void CAhaStateHandler::requestApplicationLaunch()
{
    LOG_HIGH << "CAhaStateHandler::requestApplicationLaunch" << LOG_ENDL;
    TAN_PRINTF("requestApplicationLaunch\n");
    m_AhaActiveFrameTxRx->requestApplicationLaunch();
}

void CAhaStateHandler::ahaAppInitCompletedSlot()
{
    LOG_HIGH << "CAhaStateHandler::ahaAppInitCompletedSlot" << LOG_ENDL;
    LOG_HIGH <<"ahaAppInitCompletedSlot" << LOG_ENDL;
    startPeerSession(true, TIME_PEERDELAY_SHORT);
}

void CAhaStateHandler::ahaAppInitTimeOutSlot()
{
    LOG_HIGH << "CAhaStateHandler::iPodInitTimeOutSlot" << LOG_ENDL;
    LOG_HIGH <<"Damn! iPodInitTimeOutSlot\n" << LOG_ENDL;
    emit Result(REQ_TYPE_CONNECT, E_DEVICE_NOT_FOUND);
}
void CAhaStateHandler::ahaPingTimeOutSlot()
{
    LOG_HIGH << "CAhaStateHandler::ahaPingTimeOutSlot" << LOG_ENDL;
    LOG_HIGH <<"Aha ping time out!!\n" << LOG_ENDL;
    emit Result(REQ_TYPE_CONNECT, E_AHA_SESSION_CLOSED);
}

void CAhaStateHandler::ahaSessionClosedSlot()
{
    LOG_HIGH << "CAhaStateHandler::ahaSessionClosedSlot" << LOG_ENDL;
    LOG_HIGH <<"ahaSessionClosedSlot!!\n" << LOG_ENDL;
    m_PriState = AHA_PRI_NONE_STATE;
    emit Result(REQ_TYPE_CONNECT, E_AHA_SESSION_CLOSED);
}

void CAhaStateHandler::ahaSessionOpenedSlot()
{
    LOG_HIGH << "CAhaStateHandler::ahaSessionOpenedSlot" << LOG_ENDL;
    //good, now we can try to peer.
    LOG_HIGH <<"ahaSessionOpenedSlot!!\n" << LOG_ENDL;
    m_AppLaunchWaitTimer->stop();
    startPeerSession(true, TIME_PEERDELAY_SHORT);
}

void CAhaStateHandler::peerDelayTimeOutSlot()
{
    LOG_HIGH << "CAhaStateHandler::peerDelayTimeOutSlot" << LOG_ENDL;
    TAN_PRINTF("peerDelayTimeOutSlot\n");
    //startpeersession expect startpeersession decoded signal.

    if(m_nDeviceType == DEVICE_TYPE_SMARTPHONE)
    {
        startPeerSession(false, TIME_RETRY_PEER_SESSION);   //send startPeerSession siganl every 4s.
    }
    else{
        m_StartPeerResponseTimer->start(TIME_PEER_RESPONSE_WAIT);
    }

    m_PriState = AHA_PRI_CONNECT_SYNCING_STATE;
    m_AhaActiveFrameTxRx->startPeerSession();
}

void CAhaStateHandler::ahaAppNotOpenedSlot()
{
    LOG_HIGH << "CAhaStateHandler::ahaAppNotOpenedSlot" << LOG_ENDL;
    m_PriState = AHA_PRI_NONE_STATE;
    requestApplicationLaunch();
    //start timer, if we do not
    m_AppLaunchWaitTimer->start(TIME_APPLAUNCH);
}

void CAhaStateHandler::appLaunchTimeOutSlot()
{
    LOG_HIGH << "CAhaStateHandler::appLaunchTimeOutSlot" << LOG_ENDL;
    m_PriState = AHA_PRI_NONE_STATE;
    emit Result(REQ_TYPE_CONNECT, E_AHA_APP_NOT_OPENED);
}

void CAhaStateHandler::startPeerResponseTimeOutSlot()
{
    LOG_HIGH << "CAhaStateHandler::startPeerResponseTimeOutSlot" << LOG_ENDL;
    TAN_PRINTF("startPeerResponseTimeOutSlot\n");
    m_PriState = AHA_PRI_NONE_STATE;
    emit Result(REQ_TYPE_CONNECT, E_AHA_APP_NOT_OPENED);
}

void CAhaStateHandler::queryResponseTimeOutSlot()
{
    LOG_HIGH << "CAhaStateHandler::queryResponseTimeOutSlot" << LOG_ENDL;
    TAN_PRINTF("queryResponseTimeOutSlot");
    m_PriState = AHA_PRI_NONE_STATE;
    emit Result(REQ_TYPE_CONNECT, E_AHA_APP_NOT_OPENED);
}

void CAhaStateHandler::delayUnmuteTimeOutSlot()
{
    LOG_HIGH << "CAhaStateHandler::queryResponseTimeOutSlot" << LOG_ENDL;
    emit unmute();
}

void CAhaStateHandler::startPeerSessionDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::startPeerSessionDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;
    TAN_PRINTF("startPeerSessionDecodedSlot\n");
    m_StartPeerResponseTimer->stop();

    if(m_StartPeerDelayTimer){
        m_StartPeerDelayTimer->stop();
    }
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        if(true == m_bPeerConnected)
        {
            LOG_HIGH << "CAhaStateHandler::startPeerSessionDecodedSlot already connected..." << LOG_ENDL;
            return;
        }
        m_bPeerConnected = true;

        m_AhaActiveFrameTxRx->printAhaClientInfo();
        LOG_HIGH <<"emit Result(REQ_TYPE_CONNECT, E_SUCCESS) from startPeerSessionDecodedSlot\n" << LOG_ENDL;
        emit Result(REQ_TYPE_CONNECT, E_SUCCESS);
        //transient to next stage
        m_QueryResponseTimer->start(TIME_QUERY_RESPONSE_WAIT);      // added by Ryu 20131022
        TAN_PRINTF("m_QueryResponseTimer->start \n");
        LOG_HIGH << "m_QueryResponseTimer->start" << LOG_ENDL;
        m_AhaActiveFrameTxRx->queryCurrentStation();
        break;
    case AHA_ERR_LOGIN_FAILED:
        LOG_HIGH <<"Login failed: happens when user not logged in!\n" << LOG_ENDL;
        emit Result(REQ_TYPE_CONNECT, E_AHA_PLEASE_LOGIN);
        break;
    case AHA_ERR_PROT_NOT_SUPPORTED:
        LOG_HIGH <<"AHA_ERR_PROT_NOT_SUPPORTED! \n" << LOG_ENDL;
        emit Result(REQ_TYPE_CONNECT, E_AHA_PROT_NOT_SUPPORTED);
        break;
    case AHA_ERR_SESSION_REJECTED:
        LOG_HIGH <<"AHA_ERR_SESSION_REJECTED \n" << LOG_ENDL;
        emit Result(REQ_TYPE_CONNECT, E_AHA_SESSION_REJECTED);
        break;
    case AHA_ERR_NOT_LOGGED_IN:
        LOG_HIGH <<"AHA_ERR_NOT_LOGGED_IN: retry connect (different from login failed)!\n" << LOG_ENDL;
        //startPeerSession(false, TIME_PEERDELAY_LONG);
        emit Result(REQ_TYPE_CONNECT, E_NETWORK_FAILED);    // add by Ryu 20140315
        break;
    default:
        LOG_HIGH <<"Should retry connection!\n" << LOG_ENDL;
        startPeerSession(false, TIME_PEERDELAY_LONG);
        break;
    }
}

void CAhaStateHandler::queryStationsDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::queryStationsDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;

    //querying station load the list of presets.
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        m_AhaActiveFrameTxRx->printAhaClientInfo();

        //transient to next stage;
        m_AhaActiveFrameTxRx->queryPresetStationsParam(0);
        break;
    default:
        break;
    }
}

void CAhaStateHandler::queryStationContentDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::queryStationContentDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;

    //querying station load the list of presets.
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        m_AhaActiveFrameTxRx->printAhaClientInfo();

        //transient to next stage;
        m_AhaActiveFrameTxRx->queryContentListParam(0);
        break;
    default:
        break;
    }
}

void CAhaStateHandler::activeStationParamDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::activeStationParamDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;

    AhaStationInfo_T* pAhaStationInfo;
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        m_AhaActiveFrameTxRx->printActiveStationInfo();

        pAhaStationInfo = m_AhaActiveFrameTxRx->getAhaStationInfo();
        m_CurrentTrackInfo.CurrentStation = pAhaStationInfo->strStationFullName;
        CAppAhaTrack::getInstance()->setStationName(m_CurrentTrackInfo.CurrentStation);

        //transient to next stage
        m_AhaActiveFrameTxRx->startStationLogoDownload();
        break;
    default:
        break;
    }
}

void CAhaStateHandler::presetStationParamDecodedSlot(uint16_t nErrorCode, int nStartPresetStationIndex)
{
    LOG_HIGH << "CAhaStateHandler::presetStationParamDecodedSlot" << "  nErrorCode = " << nErrorCode << "  nStartPresetStationIndex = " << nStartPresetStationIndex << LOG_ENDL;

    int ret;
    if(nErrorCode == AHA_ERR_NO_ERROR) // wewill
    {
        ret = m_AhaActiveFrameTxRx->queryPresetStationsParam(nStartPresetStationIndex + MAX_PRESET_STATION_PER_QUERY);
        if(ret == -1) // we already have everything
        {
            m_AhaActiveFrameTxRx->printPresetStationsInfo();
            emit stationListLoaded(m_AhaActiveFrameTxRx->m_PresetStationsInfo);

            m_PresetQueryIndex = 0;
            m_AhaActiveFrameTxRx->startPresetLogoDownload(m_PresetQueryIndex);
        }
    }
}

void CAhaStateHandler::activeContentParamDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::activeContentParamDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;

    AhaContentInfo_T* pAhaContentInfo;
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        static bool bTempTrueFalse = true;

        m_QueryResponseTimer->stop();           // added by Ryu 20131209   error reported by NAQC
        LOG_HIGH << "m_QueryResponseTimer->stop" << LOG_ENDL;

        m_AhaActiveFrameTxRx->printActiveContentInfo();
        pAhaContentInfo = m_AhaActiveFrameTxRx->getAhaContentInfo();
        m_CurrentTrackInfo.bAllowSkip = pAhaContentInfo->nCanNext;
        m_CurrentTrackInfo.bAllowSkipBack = pAhaContentInfo->nCanBack;
        m_CurrentTrackInfo.bAllowTimeShift = pAhaContentInfo->nCanTimeShift;
        m_CurrentTrackInfo.bAllowLike = pAhaContentInfo->nCanLike;
        m_CurrentTrackInfo.bAllowDislike = pAhaContentInfo->nCanDislike;

        m_CurrentTrackInfo.bAllowNavigate = !((doubleIsZero(pAhaContentInfo->dbLat) && doubleIsZero(pAhaContentInfo->dbLon)));
        m_CurrentTrackInfo.bAllowCall = (pAhaContentInfo->strPhone.length() != 0);

        //Album art needs to update.
        m_CurrentTrackInfo.AlbumArtUrl = pAhaContentInfo->strImageURL;

        if(bTempTrueFalse) bTempTrueFalse =false;
        else bTempTrueFalse = true;

        m_nLikeSelected = pAhaContentInfo->nLiked;
        m_nDislikeSelected = pAhaContentInfo->nDisliked;
        //TAN_PRINTF("activeContentParamDecodedSlot: %s \n", m_CurrentTrackInfo.CurrentStation.toAscii().data());
        //TAN_PRINTF("activeContentParamDecodedSlot: %s \n", m_CurrentTrackInfo.Artist.toAscii().data());
        //TAN_PRINTF("activeContentParamDecodedSlot: %s \n", m_CurrentTrackInfo.Album.toAscii().data());
        //TAN_PRINTF("activeContentParamDecodedSlot: %s \n", m_CurrentTrackInfo.Title.toAscii().data());
//hsryu_0612_update_optimize
        if(m_PrevTrackInfo.CurrentStation.compare(m_CurrentTrackInfo.CurrentStation) == 0 &&
           m_PrevTrackInfo.Artist.compare(m_CurrentTrackInfo.Artist) == 0 &&
           m_PrevTrackInfo.Album.compare(m_CurrentTrackInfo.Album) == 0 &&
           m_PrevTrackInfo.Title.compare(m_CurrentTrackInfo.Title) == 0 &&
           m_PrevTrackInfo.AlbumArtUrl.compare(m_CurrentTrackInfo.AlbumArtUrl) == 0)
        {
            TAN_PRINTF("\nBlock Duaplicate Info \n\n");
            LOG_HIGH << "CAhaStateHandler::activeContentParamDecodedSlot  11111" <<LOG_ENDL;
            break;
        }
        else
        {
            m_PrevTrackInfo.CurrentStation = m_CurrentTrackInfo.CurrentStation;
            m_PrevTrackInfo.Artist = m_CurrentTrackInfo.Artist;
            m_PrevTrackInfo.Album = m_CurrentTrackInfo.Album;
            m_PrevTrackInfo.Title = m_CurrentTrackInfo.Title;
            m_PrevTrackInfo.AlbumArtUrl = m_CurrentTrackInfo.AlbumArtUrl;

            emit TrackUpdated(1);
            emit TrackInfo(m_CurrentTrackInfo);
            m_AhaActiveFrameTxRx->startRatingImageDownload();
        }
        break;
    default:
        break;
    }

}

void CAhaStateHandler::listContentParamDecodedSlot(uint16_t nErrorCode, int nStartContentIndex)
{
    LOG_HIGH << "CAhaStateHandler::listContentParamDecodedSlot" << "  nErrorCode = " << nErrorCode << "  nStartContentIndex = " << nStartContentIndex << LOG_ENDL;

    int ret;
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        ret = m_AhaActiveFrameTxRx->queryContentListParam(nStartContentIndex + MAX_CONTENT_PER_QUERY);
        if(ret == -1) //we already have everything
        {
            m_AhaActiveFrameTxRx->printContentListInfo();
            emit contentListLoaded(m_AhaActiveFrameTxRx->m_ContentListsInfo);
        }
        break;
    default:
        break;
    }
}

void CAhaStateHandler::onAlbumArtProcessFinishedSlot(QString &filePath)
{
    LOG_HIGH << "CAhaStateHandler::onAlbumArtProcessFinishedSlot" << "  filePath = " << filePath <<  LOG_ENDL;

    emit AlbumArt(filePath);
}

void CAhaStateHandler::onStationLogoProcessFinishedSlot(QString &filePath)
{
    LOG_HIGH << "CAhaStateHandler::onStationLogoProcessFinishedSlot" << "  filePath = " << filePath << LOG_ENDL;

    emit StationLogo(filePath);

    //transient to next step
    m_AhaActiveFrameTxRx->startPartnerLogoDownload();
}

void CAhaStateHandler::onPartnerLogoProcessFinishedSlot(QString &filePath)
{
    LOG_HIGH << "CAhaStateHandler::onPartnerLogoProcessFinishedSlot" << "  filePath = " << filePath << LOG_ENDL;

    emit PartnerLogo(filePath);
    // AhaContentInfo_T* pAhaContentInfo = m_AhaActiveFrameTxRx->getAhaContentInfo(); // not used variable now - by hsryu 2013.05.23
    // AhaClientInfo_T* pAhaClientInfo = m_AhaActiveFrameTxRx->getAhaClientInfo();         // not used variable now - by hsryu 2013.05.23
    //transient to next step

    queryingActiveContent(true);
}

void CAhaStateHandler::onPresetLogoProcessFinishedSlot(QString &filePath)
{
    LOG_HIGH << "CAhaStateHandler::onPresetLogoProcessFinishedSlot" << "  filePath = " << filePath << LOG_ENDL;

    qulonglong appID = m_AhaActiveFrameTxRx->getPresetStationID(m_PresetQueryIndex);
    uint8_t nIsLBS = m_AhaActiveFrameTxRx->getLBSFlag(m_PresetQueryIndex);
    emit StationLogo(nIsLBS, appID, filePath); // wewill

    int nPresetCount = m_AhaActiveFrameTxRx->m_PresetStationsInfo.count();

    //transient to next step
    m_PresetQueryIndex++;
    if(m_PresetQueryIndex < nPresetCount)
    {
        m_AhaActiveFrameTxRx->startPresetLogoDownload(m_PresetQueryIndex);
    }
    else
    {
        //we have successfully finished downloading all logo.
        if(m_PriState == AHA_PRI_PRESET_QUERYING_STATE)
        {
            m_PriState = AHA_PRI_NONE_STATE;
        }
    }
}

void CAhaStateHandler::onRatingImageProcessFinishedSlot(QString &filePath)
{
    LOG_HIGH << "CAhaStateHandler::onRatingImageProcessFinishedSlot" << "  filePath = " << filePath << LOG_ENDL;

    emit RatingImage(filePath);
    m_LikeQueryIndex = 0;
    m_AhaActiveFrameTxRx->startLikeImageDownload(m_LikeQueryIndex);
}


void CAhaStateHandler::onLikeImageProcessFinishedSlot(QString &filePath)
{
    LOG_HIGH << "CAhaStateHandler::onLikeImageProcessFinishedSlot" << "  filePath = " << filePath << LOG_ENDL;

    emit LikeImage(m_LikeQueryIndex, filePath);
    m_LikeQueryIndex++;
    if(m_LikeQueryIndex < AHA_IMAGE_LIKE_MAX_COUNT)
    {
        m_AhaActiveFrameTxRx->startLikeImageDownload(m_LikeQueryIndex);
    }
    else
    {
        //we have downloaded all the necessary image, let us show it to GUI
        ShowCurrentLikeDislikeIcon();

        //transient to next step
        m_AhaActiveFrameTxRx->startAlbumArtDownload();
    }
}

void CAhaStateHandler::queryAlbumArtDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::queryAlbumArtDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;

    AhaImageInfo_T* pAhaImageInfo;
    AhaContentInfo_T* pAhaContentInfo;
    QString filePath;
    bool bSaveSuccess;

    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        pAhaImageInfo = m_AhaActiveFrameTxRx->getAlbumArtInfo();
        if(pAhaImageInfo->nIsDownloadComplete)
        {
            pAhaContentInfo = m_AhaActiveFrameTxRx->getAhaContentInfo();
            filePath.append(AHA_ALBUM_ART_PATH);
            bSaveSuccess = saveImageToFile(false, pAhaImageInfo->imageDataByte,
                            pAhaContentInfo->strImageURL, filePath);
            pAhaImageInfo->imageDataByte.clear();
            if(!bSaveSuccess)
                filePath.clear();
            emit albumArtProcessFinished(filePath);
        }
        else
        {
            m_AhaActiveFrameTxRx->continueAlbumArtDownload();
        }
        break;
    case AHA_ERR_UNAVAILABLE_RESOURCE:
        LOG_HIGH <<"Unabailable resource AHA_SEC_QUERY_ALBUM_ART_STATE \n" << LOG_ENDL;
        filePath.clear();
        emit albumArtProcessFinished(filePath);
        break;
    case AHA_ERR_INVALID_RESOURCE:
        LOG_HIGH <<"Invalid resource AHA_SEC_QUERY_ALBUM_ART_STATE \n" << LOG_ENDL;
        filePath.clear();
        emit albumArtProcessFinished(filePath);
        break;
    default:
        break;
    }
}

void CAhaStateHandler::queryStationLogoDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::queryStationLogoDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;

    AhaImageInfo_T* pAhaImageInfo;
    AhaStationInfo_T* pAhaStationInfo;
    QString filePath;
    bool bSaveSuccess;

    LOG_HIGH <<"Tan: queryStationLogoDecodedSlot \n" << LOG_ENDL;

    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        pAhaImageInfo = m_AhaActiveFrameTxRx->getStationLogoInfo();
        if(pAhaImageInfo->nIsDownloadComplete)
        {
            //save to file
            pAhaStationInfo = m_AhaActiveFrameTxRx->getAhaStationInfo();
            filePath.append(AHA_STATION_LOGO_PATH);
            bSaveSuccess = saveImageToFile(false, pAhaImageInfo->imageDataByte,
                            pAhaStationInfo->strStationIconURL, filePath);
            pAhaImageInfo->imageDataByte.clear();
            if(!bSaveSuccess)
                filePath.clear();
            emit stationLogoProcessFinished(filePath);
        }
        else
        {
            m_AhaActiveFrameTxRx->continueStationLogoDownload();
        }
        break;
    case AHA_ERR_UNAVAILABLE_RESOURCE:
        LOG_HIGH <<"Unavailable resource AHA_SEC_QUERY_STATION_LOGO_STATE \n" << LOG_ENDL;
        filePath.clear();
        emit stationLogoProcessFinished(filePath);
        break;
    case AHA_ERR_INVALID_RESOURCE:
        LOG_HIGH <<"Invalid resource AHA_SEC_QUERY_STATION_LOGO_STATE \n" << LOG_ENDL;
        filePath.clear();
        emit stationLogoProcessFinished(filePath);
        break;
    default:
        break;
    }
}

void CAhaStateHandler::queryPartnerLogoDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::queryPartnerLogoDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;

    AhaImageInfo_T* pAhaImageInfo;
    AhaStationInfo_T* pAhaStationInfo;
    QString filePath;
    bool bSaveSuccess;

    LOG_HIGH <<"Tan: queryPartnerLogoDecodedSlot \n" << LOG_ENDL;
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        pAhaImageInfo = m_AhaActiveFrameTxRx->getPartnerLogoInfo();
        if(pAhaImageInfo->nIsDownloadComplete)
        {
            //save to file
            pAhaStationInfo = m_AhaActiveFrameTxRx->getAhaStationInfo();
            filePath.append(AHA_PARTNER_LOGO_PATH);
            bSaveSuccess = saveImageToFile(false, pAhaImageInfo->imageDataByte,
                            pAhaStationInfo->strPartnerURL, filePath);
            pAhaImageInfo->imageDataByte.clear();
            if(!bSaveSuccess)
                filePath.clear();
            emit partnerLogoProcessFinished(filePath);
        }
        else
        {
            m_AhaActiveFrameTxRx->continuePartnerLogoDownload();
        }
        break;
    case AHA_ERR_UNAVAILABLE_RESOURCE:
        LOG_HIGH <<"Unavailable resource AHA_SEC_QUERY_PARTNER_LOGO_STATE \n" << LOG_ENDL;
        filePath.clear();
        emit partnerLogoProcessFinished(filePath);
        break;
    case AHA_ERR_INVALID_RESOURCE:
        LOG_HIGH <<"Invalid resource AHA_SEC_QUERY_PARTNER_LOGO_STATE \n" << LOG_ENDL;
        filePath.clear();
        emit partnerLogoProcessFinished(filePath);
        break;
    default:
        break;
    }
}


void CAhaStateHandler::queryPresetLogoDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::queryPresetLogoDecodedSlot" << " nErrorCode = " << nErrorCode << LOG_ENDL;

    AhaImageInfo_T* pAhaImageInfo;
    AhaStationBriefInfo_T* pAhaStationBriefInfo;
    QString filePath;
    bool bSaveSuccess;

    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        pAhaImageInfo = m_AhaActiveFrameTxRx->getPresetLogoInfo();
        if(pAhaImageInfo->nIsDownloadComplete)
        {
            //save to file
            pAhaStationBriefInfo = &(m_AhaActiveFrameTxRx->m_PresetStationsInfo[m_PresetQueryIndex]);
            filePath.append(AHA_STATION_LOGO_PATH);
            bSaveSuccess = saveImageToFile(false, pAhaImageInfo->imageDataByte,
                            pAhaStationBriefInfo->strStationIconURL, filePath);
            pAhaImageInfo->imageDataByte.clear();
            if(!bSaveSuccess)
                filePath.clear();
            emit presetLogoProcessFinished(filePath);
        }
        else
        {
            m_AhaActiveFrameTxRx->continuePresetLogoDownload(m_PresetQueryIndex);
        }
        break;
    case AHA_ERR_UNAVAILABLE_RESOURCE:
        LOG_HIGH <<"Unavailable resource AHA_SEC_QUERY_PRESET_LOGO_STATE \n" << LOG_ENDL;
        filePath.clear();
        emit presetLogoProcessFinished(filePath);
        break;
    case AHA_ERR_INVALID_RESOURCE:
        LOG_HIGH <<"Invalid resource AHA_SEC_QUERY_PRESET_LOGO_STATE \n" << LOG_ENDL;
        filePath.clear();
        emit presetLogoProcessFinished(filePath);
        break;
    default:
        break;
    }
}

void CAhaStateHandler::queryRatingImageDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::queryRatingImageDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;

    AhaImageInfo_T* pAhaImageInfo;
    AhaContentInfo_T* pAhaContentInfo;
    QString filePath;
    bool bSaveSuccess;

    LOG_HIGH <<"Tan: queryRatingImageDecodedSlot \n" << LOG_ENDL;

    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        pAhaImageInfo = m_AhaActiveFrameTxRx->getRatingImageInfo();
        if(pAhaImageInfo->nIsDownloadComplete)
        {
            pAhaContentInfo = m_AhaActiveFrameTxRx->getAhaContentInfo();
            filePath.append(AHA_CONTENT_IMAGES_PATH);
            bSaveSuccess = saveImageToFile(false, pAhaImageInfo->imageDataByte,
                            pAhaContentInfo->strRatingImageURL, filePath);
            pAhaImageInfo->imageDataByte.clear();
            if(!bSaveSuccess)
                filePath.clear();
            emit ratingImageProcessFinished(filePath);
        }
        else
        {
            m_AhaActiveFrameTxRx->continueRatingImageDownload();
        }
        break;
    case AHA_ERR_UNAVAILABLE_RESOURCE:
        LOG_HIGH <<"Unabailable resource AHA_SEC_QUERY_RATING_IMAGE_STATE \n" << LOG_ENDL;
        filePath.clear();
        emit ratingImageProcessFinished(filePath);
        break;
    case AHA_ERR_INVALID_RESOURCE:
        LOG_HIGH <<"Invalid resource AHA_SEC_QUERY_RATING_IMAGE_STATE \n" << LOG_ENDL;
        filePath.clear();
        emit ratingImageProcessFinished(filePath);
        break;
    default:
        break;
    }
}

void CAhaStateHandler::queryLikeImageDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::queryLikeImageDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;

    AhaImageInfo_T* pAhaImageInfo;
    QString filePath, strImageUrl;
    bool bSaveSuccess;

    LOG_HIGH <<"Tan: queryLikeImageDecodedSlot \n" << LOG_ENDL;

    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        pAhaImageInfo = m_AhaActiveFrameTxRx->getLikeImageInfo();
        if(pAhaImageInfo->nIsDownloadComplete)
        {
            //save to file
            filePath.append(AHA_CONTENT_IMAGES_PATH);
            strImageUrl = m_AhaActiveFrameTxRx->getLikeImageFilePath(m_LikeQueryIndex);
            bSaveSuccess = saveImageToFile(false, pAhaImageInfo->imageDataByte,
                           strImageUrl , filePath);

            pAhaImageInfo->imageDataByte.clear();
            if(!bSaveSuccess)
                filePath.clear();
            emit likeImageProcessFinished(filePath);
        }
        else
        {
            m_AhaActiveFrameTxRx->continueLikeImageDownload(m_LikeQueryIndex);
        }
        break;
    case AHA_ERR_UNAVAILABLE_RESOURCE:
        LOG_HIGH <<"Unabailable resource AHA_SEC_QUERY_LIKE_IMAGE_STATE \n" << LOG_ENDL;
        filePath.clear();
        emit likeImageProcessFinished(filePath);
        break;
    case AHA_ERR_INVALID_RESOURCE:
        LOG_HIGH <<"Invalid resource AHA_SEC_QUERY_LIKE_IMAGE_STATE \n" << LOG_ENDL;
        filePath.clear();
        emit likeImageProcessFinished(filePath);
        break;
    default:
        break;
    }
}

void CAhaStateHandler::queryCurrentStationDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::queryCurrentStationDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;

    uint64_t nActiveStationID;
    uint64_t nActiveContentID;
    AhaClientInfo_T* pAhaClientInfo;

    if(m_nNetworkStatus != AHA_NOTIFY_RESUME_NORMAL)    //wsuk.kim 131120 airplane mode on, first connect to display 'no activate station' popup on track view.
    {
        m_QueryResponseTimer->stop();
        return;
    }

    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        m_AhaActiveFrameTxRx->printAhaClientInfo();
        pAhaClientInfo = m_AhaActiveFrameTxRx->getAhaClientInfo();
        nActiveStationID = pAhaClientInfo->nActiveStationID;
        nActiveContentID = pAhaClientInfo->nActiveContentID;

        //TAN_PRINTF("queryCurrentStationDecodedSlot\n");
        //TAN_PRINTF("nActiveStationID: %llu\n", nActiveStationID);
        //TAN_PRINTF("m_PriState: %d\n", m_PriState);

        //if we are in the connnect and syncing state
        if(m_PriState == AHA_PRI_CONNECT_SYNCING_STATE)
        {
            //go to list view if either content id or station id is zero
            if(nActiveStationID == 0)
            {
                m_QueryResponseTimer->stop();         // added by Ryu 20131022
                LOG_HIGH << "m_QueryResponseTimer->stop" << LOG_ENDL;


                emit TrackUpdated(1);
                LOG_HIGH << "CAhaStateHandler::queryCurrentStationDecodedSlot " << "AHA_PRI_CONNECT_SYNCING_STATE > TrackUpdated" << LOG_ENDL;
                clearTrackViewDisplay();
                cancelTrackViewImageDownload();
                processRequest(AHA_PAUSE_REQUEST); // ITS 227354
                emit NoActiveStation();
            }
            else if(nActiveContentID == 0)
            {
                m_PriState = AHA_PRI_NONE_STATE;
                //ITS_0230534
                CAppAhaQmlProperty::getInstance()->setFirstTrackEntrance(true); //wsuk.kim TUNE_TRACK
                selectStationToPlay(nActiveStationID);
            }
            else
            {
                //0417    queryingPresetStations();
                CAppAhaQmlProperty::getInstance()->setFirstTrackEntrance(true); //wsuk.kim TUNE_TRACK
                queryingActiveStation();
            }
        }
        else
        {
            if(nActiveStationID == 0)
            {
                emit TrackUpdated(1);
                LOG_HIGH << "CAhaStateHandler::queryCurrentStationDecodedSlot " << "  TrackUpdated " << LOG_ENDL;
                clearTrackViewDisplay();
                cancelTrackViewImageDownload();
            }
            else if(nActiveContentID == 0)
            {
                m_PriState = AHA_PRI_NONE_STATE;
                selectStationToPlay(nActiveStationID);
            }
            else
            {
                queryingActiveStation();
            }
        }
        break;
    default:
        break;
    }
}

void CAhaStateHandler::changeClientPlaybackDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::changeClientPlaybackDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;

    //AhaClientInfo_T* pAhaClientInfo;
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        m_AhaActiveFrameTxRx->printAhaClientInfo(); //after user, play or pause,
        switch(m_AhaActiveFrameTxRx->m_AhaClientInfo.nPlaybackState)
        {
        case AHA_PLAYBACK_PLAY:
            emit Result(REQ_TYPE_PLAY, E_SUCCESS);
            break;
        case AHA_PLAYBACK_PAUSE:
        case AHA_PLAYBACK_STOP:
            emit Result(REQ_TYPE_PAUSE, E_SUCCESS);
            break;
        default:
            break;
        }
        //pAhaClientInfo = m_AhaActiveFrameTxRx->getAhaClientInfo();
        break;
    default:
        break;
    }
}

void CAhaStateHandler::timeShiftContentDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::timeShiftContentDecodedSlot" << " nErrorCode = " << nErrorCode << LOG_ENDL;

    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        LOG_HIGH <<"timeShiftContentDecodedSlot: AHA_ERR_NO_ERROR \n" << LOG_ENDL;
        break;
    case AHA_ERR_CANNOT_TIMESHIFT:
        LOG_HIGH <<"timeShiftContentDecodedSlot: AHA_ERR_CANNOT_TIMESHIFT \n" << LOG_ENDL;
        break;
    default:
        LOG_HIGH <<"timeShiftContentDecodedSlot: default \n" << LOG_ENDL;
        break;
    }
}

void CAhaStateHandler::performActionDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::performActionDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        LOG_HIGH <<"performActionDecodedSlot: AHA_ERR_NO_ERROR \n" << LOG_ENDL;
        break;
    case AHA_ERR_INVALID_CONTENT_ID:
        LOG_HIGH <<"performActionDecodedSlot: AHA_ERR_INVALID_CONTENT_ID \n" << LOG_ENDL;
        break;
    case AHA_ERR_INVALID_ACTION:
        LOG_HIGH <<"performActionDecodedSlot: AHA_ERR_INVALID_ACTION \n" << LOG_ENDL;
        break;
    case AHA_ERR_NEED_MYFEEDS_STATION:
        LOG_HIGH <<"performActionDecodedSlot: AHA_ERR_NEED_MYFEEDS_STATION \n" << LOG_ENDL;
        break;
    default:
        LOG_HIGH <<"performActionDecodedSlot: default \n" << LOG_ENDL;
        break;
    }
}

void CAhaStateHandler::selectStationForPlaybackDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::selectStationForPlaybackDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;
    if(nErrorCode == AHA_ERR_NO_ERROR)
    {
        m_PriState = AHA_PRI_NONE_STATE;
        LOG_HIGH <<"selectStationForPlaybackDecodedSlot No error\n" << LOG_ENDL;
    }
}

void CAhaStateHandler::selectContentForPlaybackDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::selectContentForPlaybackDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;
    if(nErrorCode == AHA_ERR_NO_ERROR)
    {
        LOG_HIGH <<"selectContentForPlaybackDecodedSlot No error\n" << LOG_ENDL;
        emit RepeatPressedSkipBack(true);  //wsuk.kim ISV_77919 repeat_key
    }
}

void CAhaStateHandler::currentContentInfoDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::currentContentInfoDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;
    //hsryu_0612_update_optimize
    AhaStationInfo_T* pAhaStationInfo;
    AhaContentInfo_T* pAhaContentInfo;
    int nContentInfoCount;
    //hsryu_0228 remove LF,space at front,end
    QString tempStr = "";
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        m_AhaActiveFrameTxRx->printActiveContentInfo();

        //hsryu_0612_update_optimize
        pAhaStationInfo = m_AhaActiveFrameTxRx->getAhaStationInfo();
        m_CurrentTrackInfo.CurrentStation = pAhaStationInfo->strStationFullName;

        pAhaContentInfo = m_AhaActiveFrameTxRx->getAhaContentInfo();
        m_CurrentTrackInfo.Artist.clear();
        m_CurrentTrackInfo.Album.clear();
        m_CurrentTrackInfo.Title.clear();

        nContentInfoCount = pAhaContentInfo->strQVectContentInfoString.count();

        switch(nContentInfoCount)
        {
        case 3:
            m_CurrentTrackInfo.Album = pAhaContentInfo->strQVectContentInfoString[2];
        case 2:
            m_CurrentTrackInfo.Title = pAhaContentInfo->strQVectContentInfoString[1];
        case 1:
            m_CurrentTrackInfo.Artist = pAhaContentInfo->strQVectContentInfoString[0];
            break;
        default:
            if(nContentInfoCount > 3)
            {
                m_CurrentTrackInfo.Album = pAhaContentInfo->strQVectContentInfoString[2];
                m_CurrentTrackInfo.Title = pAhaContentInfo->strQVectContentInfoString[1];
                m_CurrentTrackInfo.Artist = pAhaContentInfo->strQVectContentInfoString[0];
            }
            break;
        }
        //hsryu_0228 remove LF,space at front,end
        tempStr = m_CurrentTrackInfo.Artist;
        m_CurrentTrackInfo.Artist.clear();
        m_CurrentTrackInfo.Artist = tempStr.trimmed();

        LOG_HIGH << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << LOG_ENDL;
        LOG_HIGH << "cur:"<<pAhaContentInfo->nContentID<<",before " <<m_AhaActiveFrameTxRx->getAhaClientInfo()->nActiveContentID<< LOG_ENDL;
        LOG_HIGH << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << LOG_ENDL;

        //ITS_0226203
        if(pAhaContentInfo->nContentID != m_AhaActiveFrameTxRx->getAhaClientInfo()->nActiveContentID)
        {
            emit OSDInfoReady(m_CurrentTrackInfo.Artist);
        }
        //transient to next stage
        m_AhaActiveFrameTxRx->queryActiveContentParam();

        break;
    default:
        break;
    }
}

void CAhaStateHandler::notifyPlaybackOfNewStationDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::notifyPlaybackOfNewStationDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;
    TAN_PRINTF("CAhaStateHandler::notifyPlaybackOfNewStationDecodedSlot  nErrorCode = %d \n", nErrorCode);
    //AhaClientInfo_T* pAhaClientInfo;      // not used variable now - by hsryu 2013.05.23
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        if(m_PriState != AHA_PRI_CONNECT_SYNCING_STATE)
        {
            m_AhaActiveFrameTxRx->cancelRecievingInterruptQuery(QUERY_MODE_ACTIVE_CONTENT_PARAM); //wsuk.kim ISV_75730 station_remain
            //m_PriState = AHA_PRI_STATION_CHANGING_STATE;
            //pAhaClientInfo = m_AhaActiveFrameTxRx->getAhaClientInfo();   // not used variable now - by hsryu 2013.05.23
            m_AhaActiveFrameTxRx->printAhaClientInfo();
            //transient to next stage
            //m_AhaActiveFrameTxRx->queryCurrentStation(true);
            m_AhaActiveFrameTxRx->queryActiveStationParam();
        }
        break;
    default:
        break;
    }
}

void CAhaStateHandler::notifyNewContentPlaybackStateDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::notifyNewContentPlaybackStateDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;
    TAN_PRINTF("CAhaStateHandler::notifyNewContentPlaybackStateDecodedSlot  nErrorCode = %d \n", nErrorCode);

    AhaContentInfo_T* pAhaContentInfo;
    AhaClientInfo_T* pAhaClientInfo;
    AhaStationInfo_T* pAhaStationInfo; //hsryu_0530_same_contentID_refresh

    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        pAhaContentInfo = m_AhaActiveFrameTxRx->getAhaContentInfo();
        pAhaClientInfo = m_AhaActiveFrameTxRx->getAhaClientInfo();
        pAhaStationInfo = m_AhaActiveFrameTxRx->getAhaStationInfo(); //hsryu_0530_same_contentID_refresh
        emit notifyPlaybackStateChanged(pAhaClientInfo->nActiveContentID, pAhaClientInfo->nPlaybackState);

        if(m_PriState == AHA_PRI_CONNECT_SYNCING_STATE ||
           //m_PriState == AHA_PRI_PRESET_QUERYING_STATE ||
           m_PriState == AHA_PRI_STATION_CHANGING_STATE)
        {
            //For now, we just ignore the notification but we should
            //save the notification and then act on it!
            LOG_HIGH <<"Ignoring notification! When PriState is busy doing stuff! \n" << LOG_ENDL;
            return;
        }

        if(m_nNetworkStatus == 0/*AHA_NOTIFY_RESUME_NORMAL*/)   //wsuk.kim 130904 ITS_0182092 repeat buffering
            bufferingIntervalCheckStart(pAhaClientInfo->nPlaybackState);

        if(pAhaClientInfo->nPlaybackState == AHA_PLAYBACK_PAUSE )
        {
            TAN_PRINTF("Playback State from phone --> AHA_PLAYBACK_PAUSE \n");
            LOG_HIGH << "Playback State from phone --> AHA_PLAYBACK_PAUSE" << LOG_ENDL;
            emit Result(REQ_TYPE_PAUSE, E_SUCCESS);
        }
        else if(pAhaClientInfo->nPlaybackState == AHA_PLAYBACK_STOP)
        {
            TAN_PRINTF("Playback State from phone --> AHA_PLAYBACK_STOP \n");
            LOG_HIGH << "Playback State from phone --> AHA_PLAYBACK_STOP" << LOG_ENDL;
            emit Result(REQ_TYPE_PAUSE, E_SUCCESS);
        }
        else if(pAhaClientInfo->nPlaybackState == AHA_PLAYBACK_PLAY)
        {
            TAN_PRINTF("Playback State from phone --> AHA_PLAYBACK_PLAY \n");
            LOG_HIGH << "Playback State from phone --> AHA_PLAYBACK_PLAY" << LOG_ENDL;
            emit Result(REQ_TYPE_PLAY, E_SUCCESS);
        }
        else if(pAhaClientInfo->nPlaybackState == AHA_PLAYBACK_BUFFERING)
        {
            TAN_PRINTF("Playback State from phone --> AHA_PLAYBACK_BUFFERING \n");
            LOG_HIGH << "Playback State from phone --> AHA_PLAYBACK_BUFFERING" << LOG_ENDL;
            emit Result(REQ_TYPE_PLAY, E_SUCCESS);
        }
        /*
        if(pAhaClientInfo->nPlaybackState == AHA_PLAYBACK_PAUSE ||
                pAhaClientInfo->nPlaybackState == AHA_PLAYBACK_STOP)
        {
            TAN_PRINTF("Playback State from phone ---> emit Result(REQ_TYPE_PAUSE, E_SUCCESS) \n");
            emit Result(REQ_TYPE_PAUSE, E_SUCCESS);
        }
        else if (pAhaClientInfo->nPlaybackState == AHA_PLAYBACK_PLAY ||
                 pAhaClientInfo->nPlaybackState == AHA_PLAYBACK_BUFFERING)
        {
            TAN_PRINTF("Playback State from phone ---> emit Result(REQ_TYPE_PLAY, E_SUCCESS) \n");
            emit Result(REQ_TYPE_PLAY, E_SUCCESS);
        }
        */

        //if the info we have about the content is not the same with the active content:
        //hsryu_0530_same_contentID_refresh
        if(pAhaContentInfo->nContentID != pAhaClientInfo->nActiveContentID)
        {
            LOG_HIGH <<"Refreshing content \n" << LOG_ENDL;
            //see if we already requested, if so we should wait instead of flooding the
            //server with too many request.
            if(m_nRequestedContentID != pAhaClientInfo->nActiveContentID)
            {
                LOG_HIGH <<"Request refreshing content\n" << LOG_ENDL;
                TAN_PRINTF("===hsryu Request refreshing content once===\n");
                m_nRequestedContentID = pAhaClientInfo->nActiveContentID;
                queryingActiveContent();
                //Tan: 2013.01.17: emit nActiveContentID to content list to move cursor
                emit notifyActiveContentIDChanged(pAhaClientInfo->nActiveContentID);
            }
        }
        else //hsryu_0530_same_contentID_refresh
        {
            if(m_AhaActiveFrameTxRx->getAhaContentInfo()->nCanTimeShift == 0 &&
                 pAhaStationInfo->nIsLBS == 0  &&
                 pAhaStationInfo->nIsFacebook == 0 &&
                 pAhaStationInfo->nIsTwitter == 0)
            {
                TAN_PRINTF("hsryu Request refreshing content at same content ID \n");
                m_AhaActiveFrameTxRx->queryCurrentContentDisplayInfo();
            }
        }
        break;
    default:
        break;
    }
}

void CAhaStateHandler::notifyNewContentAvailableDecodedSlot(uint16_t nErrorCode, uint64_t nStationID)
{
    LOG_HIGH << "CAhaStateHandler::notifyNewContentAvailableDecodedSlot" << "  nErrorCode = " << nErrorCode << "  nStationID = " << nStationID << LOG_ENDL;
    AhaClientInfo_T *pAhaClientInfo;
    CAppAhaQmlProperty* qmlPropertyInstance = NULL;


    qmlPropertyInstance = CAppAhaQmlProperty::getInstance();
    pAhaClientInfo = m_AhaActiveFrameTxRx->getAhaClientInfo();

    LOG_HIGH <<"notifyNewContentAvailableDecodedSlot returns nErrorCode: %d.\n" << nErrorCode << LOG_ENDL;
    //LOG_HIGH <<"Check if nActiveStationID equal to this nStationID: %llu, %llu \n",
    //       pAhaClientInfo->nActiveStationID, nStationID << LOG_ENDL;
    //request content list only if the new content belong to the active station id
    //and only if we are in the content list view.
    if(qmlPropertyInstance !=NULL && nStationID == pAhaClientInfo->nActiveStationID)
    {
        //if(qmlPropertyInstance->getFocusContentList())
        {
            queryingContentList();
        }
    }
}

void CAhaStateHandler::notifyContentDeletedDecodedSlot(uint16_t nErrorCode)
{
    LOG_HIGH << "CAhaStateHandler::notifyContentDeletedDecodedSlot" << "  nErrorCode = " << nErrorCode << LOG_ENDL;
    CAppAhaQmlProperty* qmlPropertyInstance = NULL;

    LOG_HIGH <<"notifyContentDeletedDecodedSlot returns nErrorCode: %d.\n"<< nErrorCode  << LOG_ENDL;

    qmlPropertyInstance = CAppAhaQmlProperty::getInstance();
    if(qmlPropertyInstance !=NULL)
    {
        if(qmlPropertyInstance->getFocusContentList())
        {
            emit contentListLoaded(m_AhaActiveFrameTxRx->m_ContentListsInfo);
        }
    }
}

void CAhaStateHandler::notifyClientStatusUpdateDecodedSlot(uint16_t nErrorCode, uint32_t nConnStatusCode)
{
    LOG_HIGH << "CAhaStateHandler::notifyClientStatusUpdateDecodedSlot" << "  nErrorCode = " << nErrorCode << "  nConnStatusCode = " << nConnStatusCode << LOG_ENDL;
    //LOG_HIGH <<"notifyClientStatusUpdateDecodedSlot returns nErrorCode: %d, nConnStatusCode: %d.\n",
    //                   nErrorCode, nConnStatusCode);
    m_nNetworkStatus = nConnStatusCode; //wsuk.kim 130904 ITS_0182092 repeat buffering
    switch(nConnStatusCode)
    {
#ifndef NO_NETWORK//wsuk.kim no_network
    case AHA_NOTIFY_SERVER_LOST:
        LOG_HIGH << "CAhaStateHandler::notifyClientStatusUpdateDecodedSlot AHA_NOTIFY_SERVER_LOST" << LOG_ENDL;
        TAN_PRINTF("CAhaStateHandler::notifyClientStatusUpdateDecodedSlot  AHA_NOTIFY_SERVER_LOST \n");
        if(m_PriState == AHA_PRI_CONNECT_SYNCING_STATE)
        {
            emit NetworkStatus(3/*No_Network_SYNCING_STATE*/);
            emit Result(REQ_TYPE_CONNECT, E_NETWORK_FAILED);
        }
        else
        {
            emit NetworkStatus(AHA_NOTIFY_SERVER_LOST);
            emit Result(/*REQ_TYPE_CONNECT wsuk.kim 0614*/ REQ_TYPE_NONE, E_NETWORK_FAILED);
        }
        break;
    case AHA_NOTIFY_SERVER_DOWN:
        LOG_HIGH<<"[ CAhaStateHandler::notifyClientStatusUpdateDecodedSlot() ] AHA_NOTIFY_SERVER_DOWN,, to disconnect.. "<<LOG_ENDL;
        TAN_PRINTF("CAhaStateHandler::notifyClientStatusUpdateDecodedSlot  AHA_NOTIFY_SERVER_DOWN \n");
        emit NetworkStatus(AHA_NOTIFY_SERVER_DOWN);
        Disconnect();
        break;
    case AHA_NOTIFY_RESUME_NORMAL:
        LOG_HIGH << "CAhaStateHandler::notifyClientStatusUpdateDecodedSlot AHA_NOTIFY_RESUME_NORMAL" << LOG_ENDL;
        TAN_PRINTF("CAhaStateHandler::notifyClientStatusUpdateDecodedSlot  AHA_NOTIFY_RESUME_NORMAL \n");
        emit NetworkStatus(AHA_NOTIFY_RESUME_NORMAL);
        emit NetworkResume();     // no network by Ryu
        break;
    default:
        break;
#else
    case AHA_NOTIFY_SERVER_LOST:
    case AHA_NOTIFY_SERVER_DOWN:
        //emit Result(REQ_TYPE_CONNECT, E_AHA_APP_NOT_OPENED);
        break;
    case AHA_NOTIFY_RESUME_NORMAL:
    default:
        //do nothing
        break;
#endif
    }
}

void CAhaStateHandler::processRequest(AHA_REQUEST_TYPE_T nRequestAction)
{
    LOG_HIGH << "CAhaStateHandler::processRequest nRequestAction = " << nRequestAction << LOG_ENDL;
    uint8_t nPlaybackState, nCanPause, nCanNext, nCanBack,nCanTimeShift;
    uint8_t nCanLike, nCanDislike, nCanUnvote;

    if(NULL == m_AhaActiveFrameTxRx)
    {
        LOG_HIGH << "CAhaStateHandler::processRequest NULL == m_AhaActiveFrameTxRx " << LOG_ENDL;
        return;
    }

    switch (nRequestAction)
    {
    case AHA_PLAY_REQUEST:
        LOG_HIGH << "CAhaStateHandler::processRequest AHA_PLAY_REQUEST play...!!!" << LOG_ENDL;
        nPlaybackState = m_AhaActiveFrameTxRx->getAhaClientInfo()->nPlaybackState;

        // below code will be done on changeClientPlaybackDecodedSlot
        //emit Result(REQ_TYPE_PLAY, E_SUCCESS);
        if(nPlaybackState != AHA_PLAYBACK_PLAY)
        {
            LOG_HIGH <<"Resume playing!\n" << LOG_ENDL;
            m_AhaActiveFrameTxRx->play();
        }
        else
        {
            LOG_HIGH <<"Already playing!\n" << LOG_ENDL;
            m_DelayedUnmuteTimer->start(500);
        }
        break;
    case AHA_PAUSE_REQUEST:
        LOG_HIGH << "CAhaStateHandler::processRequest AHA_PAUSE_REQUEST" << LOG_ENDL;
        nCanPause = m_AhaActiveFrameTxRx->getAhaContentInfo()->nCanPause;
        LOG_HIGH << "CAhaStateHandler::processRequest 222222222222 :" <<nCanPause<< LOG_ENDL;

        // below code will be done on changeClientPlaybackDecodedSlot
        //emit Result(REQ_TYPE_PAUSE, E_SUCCESS);
        if(nCanPause)
        {
            LOG_HIGH <<"Pausing!\n" << LOG_ENDL;
            m_AhaActiveFrameTxRx->pause();
        }
        else
        {
            LOG_HIGH <<"Stopping!\n" << LOG_ENDL;
            m_AhaActiveFrameTxRx->stop();
        }
        break;
    case AHA_BACK_REQUEST:
        nCanBack = m_AhaActiveFrameTxRx->getAhaContentInfo()->nCanBack;

        if(nCanBack)
        {
            LOG_HIGH <<"Back!\n" << LOG_ENDL;
            m_AhaActiveFrameTxRx->back();
        }
        else
        {
            LOG_HIGH <<"Cannot back!\n" << LOG_ENDL;
        }
        break;
    case AHA_NEXT_REQUEST:
        nCanNext = m_AhaActiveFrameTxRx->getAhaContentInfo()->nCanNext;

        if(nCanNext)
        {
            LOG_HIGH <<"Next!\n" << LOG_ENDL;
            m_AhaActiveFrameTxRx->next();
        }
        else
        {
            LOG_HIGH <<"Cannot next!\n" << LOG_ENDL;
        }
        break;
    case AHA_REWIND15_REQUEST:
        nCanTimeShift = m_AhaActiveFrameTxRx->getAhaContentInfo()->nCanTimeShift;

        if(nCanTimeShift)
        {
            LOG_HIGH <<"Rewind15! \n" << LOG_ENDL;
            m_AhaActiveFrameTxRx->rewind15();
        }
        else
        {
            LOG_HIGH <<"Cannot timeshift \n" << LOG_ENDL;
        }
        break;
    case AHA_FORWARD30_REQUEST:
        nCanTimeShift = m_AhaActiveFrameTxRx->getAhaContentInfo()->nCanTimeShift;
        LOG_HIGH <<"AHA_FORWARD30_REQUEST: \n" << LOG_ENDL;
        if(nCanTimeShift)
        {
            LOG_HIGH <<"Forward30! \n" << LOG_ENDL;
            m_AhaActiveFrameTxRx->forward30();

        }
        else
        {
            LOG_HIGH <<"Cannot timeshift \n" << LOG_ENDL;
        }
        break;
    case AHA_LIKE_VOTE_UNVOTE_REQUEST:
        if(m_nLikeSelected)
        {
            //like was selected, so, this is a request to unvote.
            nCanUnvote = m_AhaActiveFrameTxRx->getAhaContentInfo()->nCanUnvote;
            if(nCanUnvote)
            {
                m_nLikeSelected = 0;    //unvoting
                LOG_HIGH <<"Unvote like! \n" << LOG_ENDL;
                m_AhaActiveFrameTxRx->unvote();
            }
            else
            {
                LOG_HIGH <<"Cannot unvote like \n" << LOG_ENDL;
            }
        }
        else
        {
            //like was unselected, voting like.
            nCanLike = m_AhaActiveFrameTxRx->getAhaContentInfo()->nCanLike;
            if(nCanLike)
            {
                m_nLikeSelected = 1;
                m_nDislikeSelected = 0;
                LOG_HIGH <<"Vote like! \n" << LOG_ENDL;
                m_AhaActiveFrameTxRx->voteLike();
            }
            else
            {
                LOG_HIGH <<"Cannot vote like \n" << LOG_ENDL;
            }
        }
        ShowCurrentLikeDislikeIcon();
        break;
    case AHA_DISLIKE_VOTE_UNVOTE_REQUEST:
        if(m_nDislikeSelected)
        {
            //dislike was selected, so, this is a request to unvote dislike.
            nCanUnvote = m_AhaActiveFrameTxRx->getAhaContentInfo()->nCanUnvote;
            if(nCanUnvote)
            {
                m_nDislikeSelected = 0;    //unvoting
                LOG_HIGH <<"Unvote dislike! \n" << LOG_ENDL;
                m_AhaActiveFrameTxRx->unvote();
            }
            else
            {
                LOG_HIGH <<"Cannot unvote dislike \n" << LOG_ENDL;
            }
        }
        else
        {
            //dislike was unselected, voting dislike.
            nCanDislike = m_AhaActiveFrameTxRx->getAhaContentInfo()->nCanDislike;
            if(nCanDislike)
            {
                m_nDislikeSelected = 1;
                m_nLikeSelected = 0;
                LOG_HIGH <<"Vote dislike! \n" << LOG_ENDL;
                m_AhaActiveFrameTxRx->voteDislike();
            }
            else
            {
                LOG_HIGH <<"Cannot vote dislike \n" << LOG_ENDL;
            }
        }
        ShowCurrentLikeDislikeIcon();
        break;
    case AHA_LOG_CALL_REQUEST:
        m_AhaActiveFrameTxRx->logCall();
        break;
    case AHA_LOG_NAVIGATE_REQUEST:
        m_AhaActiveFrameTxRx->logNavigate();
        break;
    default:
        break;
    }
}

uint8_t CAhaStateHandler::getCurrentRateState(uint8_t nLikeSelected, uint8_t nDislikeSelected)
{
    LOG_HIGH << "CAhaStateHandler::getCurrentRateState" << "  nLikeSelected = " << nLikeSelected << "  nDislikeSelected = " << nDislikeSelected << LOG_ENDL;
    uint8_t nRateState = AHA_RATE_NONE_SELECTED;

    if(nLikeSelected && nDislikeSelected)
    {
        nRateState = AHA_RATE_BOTH_SELECTED;
    }
    else if(nLikeSelected)
    {
        nRateState = AHA_RATE_LIKE_SELECTED;
    }
    else if(nDislikeSelected)
    {
        nRateState = AHA_RATE_DISLIKE_SELECTED;
    }
    else
    {
        nRateState = AHA_RATE_NONE_SELECTED;
    }
    return nRateState;
}

void CAhaStateHandler::ShowCurrentLikeDislikeIcon()
{
    LOG_HIGH << "CAhaStateHandler::ShowCurrentLikeDislikeIcon" << LOG_ENDL;
    AhaContentInfo_T* pAhaContentInfo;
    uint8_t nCanUnvote, nRateState;

    pAhaContentInfo = m_AhaActiveFrameTxRx->getAhaContentInfo();
    nCanUnvote = pAhaContentInfo->nCanUnvote;
    nRateState = getCurrentRateState(m_nLikeSelected, m_nDislikeSelected);
    emit UpdateLikeDislikeIcon(nRateState, nCanUnvote);

    // if BAN icon is clicked
    if(nCanUnvote == false && m_nDislikeSelected == true)
    {
        nRateState = getCurrentRateState(m_nLikeSelected, m_nDislikeSelected);
        QTimer::singleShot(500, this, SLOT(OnTimeout_BanRefreshEvent()));
    }
}

void CAhaStateHandler::OnTimeout_BanRefreshEvent()
{
    LOG_HIGH << "CAhaStateHandler::OnTimeout_BanRefreshEvent" << LOG_ENDL;
    uint8_t nCanUnvote = false;
    uint8_t nRateState = false;

    m_nDislikeSelected = 0;
    nRateState = getCurrentRateState(m_nLikeSelected, m_nDislikeSelected);
    emit UpdateLikeDislikeIcon(nRateState, nCanUnvote);
}

//wsuk.kim 130904 ITS_0182092 repeat buffering
void CAhaStateHandler::bufferingIntervalCheckStart(uint8_t play_status)
{
    LOG_HIGH << "CAhaStateHandler::bufferingIntervalCheckStart" << LOG_ENDL;

    if(play_status != AHA_PLAYBACK_BUFFERING/*AHA_PLAYBACK_PLAY*/)  //play 2,   buffering 3
        return;

    if(m_nBufferingCount == 1)
    {
        m_BufferingintervalTimer->start(TIME_BUFFERING_INTERVAL_WAIT);
    }

    if(m_nBufferingCount < 11)
    {
        m_nBufferingCount++;
    }
    else //if(5 <= m_nBufferingCount)
    {
        bufferingTimerstop();   //wsuk.kim 130926 ITS_191339 repeat press play/pause key, occur repeat buffering popup.
//        m_BufferingintervalTimer->stop();
//        bufferIntervalTimeOutSlot();
    }
}

void CAhaStateHandler::bufferIntervalTimeOutSlot()
{
    if(11 <= m_nBufferingCount)
    {
        emit Result(REQ_TYPE_NONE, E_REPEAT_BUFFERING);
    }
    m_nBufferingCount = 0;
}

void CAhaStateHandler::bufferingTimerstop() //wsuk.kim 130926 ITS_191339 repeat press play/pause key, occur repeat buffering popup.
{
    m_BufferingintervalTimer->stop();
    bufferIntervalTimeOutSlot();
}
//wsuk.kim 130904 ITS_0182092 repeat buffering

void CAhaStateHandler::stopTimers()
{
    LOG_HIGH << "CAhaStateHandler::stopTimers()" << LOG_ENDL;
    TAN_PRINTF("CAhaStateHandler::stopTimers() \n");

    m_AppLaunchWaitTimer->stop();
    m_StartPeerDelayTimer->stop();
    m_StartPeerResponseTimer->stop();  // added by Ryu : stop timers at Trackview : ITS0224000
    m_QueryResponseTimer->stop();

    //m_BufferingintervalTimer->stop();
}
