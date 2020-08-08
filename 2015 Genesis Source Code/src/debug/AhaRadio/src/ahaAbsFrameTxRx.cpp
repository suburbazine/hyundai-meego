
#include "ahaAbsFrameTxRx.h"

CAhaAbsFrameTxRx::CAhaAbsFrameTxRx()
{
    LOG_INIT_V2("Aha");

    m_ReplyWaitTimer = NULL;
    m_PingCountdownTimer = NULL;
    m_PingIntervalTimer = NULL;
    m_bChannelBusy = NULL;
    m_bLaunchRequested = NULL;
    m_bInitCompletionReceived = NULL;
    m_nRequestedStationID = NULL;
    m_nRequestedContentID = NULL;
    m_PresetQueryIndex = 0;
    m_LikeQueryIndex = 0;
    m_TopProcessPlayStation = NULL; //hsryu_0612_update_optimize
    m_nContentIndexToQuery = 0;
    m_nPresetStationIndexToQuery = 0;

    m_bLongReceivedData = false;
    m_nLongReceivedData = 0;
    m_ReceivedData.clear();
}

CAhaAbsFrameTxRx::~CAhaAbsFrameTxRx()
{
    LOG_HIGH<<"[ CAhaAbsFrameTxRx::~CAhaAbsFrameTxRx ] call destructor "<<LOG_ENDL;
    DeInit();
    m_ReplyWaitTimer = NULL;
    m_PingCountdownTimer = NULL;
    m_PingIntervalTimer = NULL;
    m_bChannelBusy = NULL;
    m_bLaunchRequested = NULL;
    m_bInitCompletionReceived = NULL;
    m_nRequestedStationID = NULL;
    m_nRequestedContentID = NULL;
    m_PresetQueryIndex = 0;
    m_LikeQueryIndex = 0;
    m_TopProcessPlayStation = NULL; //hsryu_0612_update_optimize
}

EAhaError CAhaAbsFrameTxRx::Init()
{
    LOG_HIGH << "CAhaAbsFrameTxRx::Init()" <<LOG_ENDL;
    resetMemberVariables();
    createTimers();

    return E_SUCCESS;
}

void CAhaAbsFrameTxRx::DeInit()
{
    LOG_HIGH << "CAhaAbsFrameTxRx::DeInit()" <<LOG_ENDL;
    resetMemberVariables();
    destroyTimers();
}

void CAhaAbsFrameTxRx::processPendingRequest()
{
    uint16_t nOpCode;
    AHA_QUERY_MODE_T nQueryMode;
    //LOG_HIGH<<"[ CAhaAbsFrameTxRx::processPendingRequest ] in..." <<LOG_ENDL;
    //if there is pending transaction to process and if the channel is not occupied.

    if(m_PendingTransactionQueue.count() && !m_bChannelBusy)
    {
        nOpCode = m_PendingTransactionQueue[0].nOpCode;
        nQueryMode = m_PendingTransactionQueue[0].nQueryMode;
        m_PendingTransactionQueue.remove(0);
        LOG_HIGH <<"[ CAhaAbsFrameTxRx::processPendingRequest ] nOpCode: "<< nOpCode<<" nQueryMode: "<< nQueryMode<< LOG_ENDL;

        encodeAndSendCommand(nOpCode, nQueryMode);
    }
    else
    {
        LOG_HIGH <<"[ CAhaAbsFrameTxRx::processPendingRequest ] No Pending Request " << LOG_ENDL;
    }
}

void CAhaAbsFrameTxRx::sendCommandRequest(uint16_t nOpCode, AHA_QUERY_MODE_T nQueryMode)
{
LOG_HIGH<<"[ CAhaAbsFrameTxRx::sendCommandRequest ] in... op:" << nOpCode <<" query : " <<nQueryMode <<LOG_ENDL;
    TransactionRecordTable_T thisRecord;
    thisRecord.nOpCode = nOpCode;
    thisRecord.nQueryMode = nQueryMode;
    m_PendingTransactionQueue.append(thisRecord);
    if(!m_bChannelBusy)
    {
        processPendingRequest();
    }
}

void CAhaAbsFrameTxRx::addToTransactionTable(QByteArray &byteArray, AHA_QUERY_MODE_T nQueryMode)
{
    uint16_t nTransactionID;
    uint16_t nOpCode;

    TransactionRecordTable_T thisTransactionRecord;
    nTransactionID = byteToUint16_t(byteArray, 4);
    nOpCode = byteToUint16_t(byteArray, 6);
    thisTransactionRecord.nTransactionID = nTransactionID;
    thisTransactionRecord.nOpCode = nOpCode;
    thisTransactionRecord.nQueryMode = nQueryMode;

    m_TransactionRecordTable.append(thisTransactionRecord);
}


uint16_t CAhaAbsFrameTxRx::extractOpCodeFromTransactionTable(QByteArray &byteArray,uint16_t &nOpCode,
                                                         AHA_QUERY_MODE_T &nQueryMode)
{
    int i;
    uint16_t nTransactionID;
    bool isResponse = false;
    nOpCode = AHA_OPCODE_UNKNOWN_OPCODE;
    nQueryMode = QUERY_MODE_UNKNOWN_MODE;

    nTransactionID = byteToUint16_t(byteArray, 4);

    LOG_HIGH <<"[ CAhaAbsFrameTxRx::extractOpCodeFromTransactionTable ] trans ID: " << nTransactionID<< LOG_ENDL;

    //check if MSB is 1, if yes, it is a response to our query
    if(nTransactionID & 0x8000)
    {
        //When requesting with ID 1, the server returns 8001, i.e. appends the MSB of ID with 1.
        nTransactionID = 0x7FFF & nTransactionID;
        isResponse = true;
    }
    else
    {
        nTransactionID = AHA_TRANSACTION_ID_LIMIT; //because our value will never reach 32768
    }

    if(isResponse)
    {
        for (i=0; i< m_TransactionRecordTable.size(); i++)
        {
            if(nTransactionID == m_TransactionRecordTable[i].nTransactionID)
            {
                nOpCode = m_TransactionRecordTable[i].nOpCode;
                nQueryMode = m_TransactionRecordTable[i].nQueryMode;
                m_TransactionRecordTable.remove(i);
                break;
            }
        }
    }
    else
    {
        //client initiated command.
        nOpCode = byteToUint16_t(byteArray,6);
        LOG_HIGH <<"Client sends nOpCode: " << nOpCode<< LOG_ENDL;
    }
    return nTransactionID;
}

int CAhaAbsFrameTxRx::processSinglePackage(QByteArray &byteArray)
{
    uint16_t nOpCode, nAhaError, nTransactionID;
    AHA_QUERY_MODE_T nQueryMode;

    //LOG_HIGH <<"Single Package:" <<byteArray.toHex()<< LOG_ENDL;

    printBinaryByteArray(byteArray);

    //validateCheckSum
    if(!verifyChecksum(byteArray))
    {
        LOG_HIGH <<"\nChecking checksum: FAIL" << LOG_ENDL;
        LOG_HIGH <<"\nChecking checksum: FAIL" << LOG_ENDL;
        LOG_HIGH <<"\nChecking checksum: FAIL" << LOG_ENDL;
        return 0;
    }

    //look up the opcode and query mode by using the received transaction id.
    nTransactionID = extractOpCodeFromTransactionTable(byteArray, nOpCode, nQueryMode);

    resetReplyWaitTimer(nTransactionID);

    nAhaError = decodeResponse(byteArray, nOpCode, nQueryMode);
    m_PingCountdownTimer->start(TIME_PING_WAIT);
    m_PingIntervalTimer->stop();

    return 0;
}

void CAhaAbsFrameTxRx::encodeAndSendCommand(uint16_t nOpCode, AHA_QUERY_MODE_T nQueryMode)
{
    QByteArray data;

    LOG_HIGH<< "CAhaAbsFrameTxRx::encodeAndSendCommand op:" << nOpCode << " query mode: " << nQueryMode <<LOG_ENDL;

    switch(nOpCode)
    {
    case AHA_OPCODE_START_PEER_SESSION:
        data = createStartPeerSessionRequest();
        break;
    case AHA_OPCODE_QUERY_CURRENT_STATION:
        data = createQueryCurrentStationRequest();
        break;
    case AHA_OPCODE_QUERY_STATIONS:
        data = createQueryStationsRequest();
        break;
    case AHA_OPCODE_QUERY_STATION_PARAMETERS:
        if(nQueryMode == QUERY_MODE_ACTIVE_STATION_PARAM)
        {
            LOG_HIGH<< " %%%  AHA_OPCODE_QUERY_STATION_PARAMETERS 111 nActiveStationID:" << m_AhaClientInfo.nActiveStationID<<LOG_ENDL;
            data = createQuerySingleStationParametersRequest(m_AhaClientInfo.nActiveStationID);
        }
        else if(nQueryMode == QUERY_MODE_PRESET_STATION_PARAM)
        {
            LOG_HIGH<< " %%%  AHA_OPCODE_QUERY_STATION_PARAMETERS 222 nActiveStationID:" << m_AhaClientInfo.nActiveStationID<<LOG_ENDL;
            data = createQueryMultipleStationParametersRequest(m_AhaClientInfo.nQVectPresetStationID, m_nPresetStationIndexToQuery);
        }
        break;
    case AHA_OPCODE_QUERY_STATION_CONTENT:
        LOG_HIGH<< " %%%  AHA_OPCODE_QUERY_STATION_CONTENT 1111 nActiveStationID:" << m_AhaClientInfo.nActiveStationID<<LOG_ENDL;
        data = createQueryStationContentRequest(m_AhaClientInfo.nActiveStationID);
        break;
    case AHA_OPCODE_QUERY_CONTENT_PARAMETERS:
        if(nQueryMode == QUERY_MODE_ACTIVE_CONTENT_PARAM)
        {
            LOG_HIGH<< " %%% QUERY_MODE_ACTIVE_CONTENT_PARAM contents id:" <<m_AhaClientInfo.nActiveContentID<<LOG_ENDL;
            data = createQuerySingleContentParametersRequest(m_AhaClientInfo.nActiveContentID);
        }
        else if(nQueryMode == QUERY_MODE_LIST_CONTENT_PARAM)
        {
            data = createQueryMultipleContentParametersRequest(m_AhaClientInfo.nQVectListContentID, m_nContentIndexToQuery);
        }
        break;
    case AHA_OPCODE_QUERY_BINARY_DATA:
        if(nQueryMode == QUERY_MODE_ALBUM_ART)
        {
            if(m_ActiveContentInfo.strImageURL != NULL)
                LOG_HIGH <<" %%% Sending QUERY_MODE_ALBUM_ART url:" <<m_ActiveContentInfo.strImageURL<< LOG_ENDL;
            data = createQueryBinaryDataRequest(m_ActiveContentInfo.strImageURL,
                                                m_AlbumArtInfo.nOffset,
                                                ALBUM_ART_WIDTH, ALBUM_ART_HEIGHT);
        }
        else if(nQueryMode == QUERY_MODE_STATION_LOGO)
        {
            if(m_ActiveStationInfo.strStationIconURL != NULL)
                LOG_HIGH <<" %%% Sending QUERY_MODE_STATION_LOGO url" <<m_ActiveStationInfo.strStationIconURL<< LOG_ENDL;
            data = createQueryBinaryDataRequest(m_ActiveStationInfo.strStationIconURL,
                                                m_StationLogoInfo.nOffset,
                                                STATION_LOGO_WIDTH, STATION_LOGO_HEIGHT);
        }
        else if(nQueryMode == QUERY_MODE_PARTNER_LOGO)
        {
            if(m_ActiveStationInfo.strPartnerURL != NULL)
                LOG_HIGH <<" %%% Sending QUERY_MODE_PARTNER_LOGO url:" <<m_ActiveStationInfo.strPartnerURL<< LOG_ENDL;
            data = createQueryBinaryDataRequest(m_ActiveStationInfo.strPartnerURL,
                                                m_PartnerLogoInfo.nOffset,
                                                PARTNER_LOGO_WIDTH, PARTNER_LOGO_HEIGHT);
        }
        else if(nQueryMode == QUERY_MODE_PRESET_LOGO)
        {
            //ITS_0223473
            if(m_PresetQueryIndex >= 0 && m_PresetQueryIndex < m_PresetStationsInfo.size())
            {
                if(m_PresetStationsInfo[m_PresetQueryIndex].strStationIconURL != NULL)
                    LOG_HIGH <<" %%% Sending QUERY_MODE_PRESET_LOGO url:" <<m_PresetStationsInfo[m_PresetQueryIndex].strStationIconURL<< LOG_ENDL;
                data = createQueryBinaryDataRequest(m_PresetStationsInfo[m_PresetQueryIndex].strStationIconURL,
                                                    m_PresetLogoInfo.nOffset,
                                                    STATION_LOGO_WIDTH, STATION_LOGO_HEIGHT);
            }
        }
        else if(nQueryMode == QUERY_MODE_RATING_IMAGE)
        {
            if(m_ActiveContentInfo.strRatingImageURL != NULL)
                LOG_HIGH <<" %%% Sending QUERY_MODE_RATING_IMAGE url:" <<m_ActiveContentInfo.strRatingImageURL<< LOG_ENDL;
            data = createQueryBinaryDataRequest(m_ActiveContentInfo.strRatingImageURL,
                                                m_RatingImageInfo.nOffset,
                                              RATING_IMAGE_WIDTH, RATING_IMAGE_HEIGHT);
        }
        else if(nQueryMode == QUERY_MODE_LIKE_IMAGE)
        {
            if(getLikeImageFilePath(m_LikeQueryIndex) != NULL)
                LOG_HIGH <<" %%% Sending QUERY_MODE_LIKE_IMAGE filepath:" <<getLikeImageFilePath(m_LikeQueryIndex)<< LOG_ENDL;
            data = createQueryBinaryDataRequest(getLikeImageFilePath(m_LikeQueryIndex),
                                                m_LikeImageInfo.nOffset,
                                              LIKE_IMAGE_WIDTH, LIKE_IMAGE_HEIGHT);
        }
        break;
    case AHA_OPCODE_SELECT_STATION_FOR_PLAYBACK:
        //hsryu_0612_update_optimize
        //TAN_PRINTF("AHA_OPCODE_SELECT_STATION_FOR_PLAYBACK \n");
        m_TopProcessPlayStation = true;
        data = createSelectStationForPlaybackRequest(m_nRequestedStationID);
        break;
    case AHA_OPCODE_SELECT_CONTENT_FOR_PLAYBACK:
        if(nQueryMode == QUERY_MODE_BACK)
        {
            data = createSelectContentForPlaybackRequest((uint64_t)AHA_CONTENT_ID_BACK);
        }
        else if(nQueryMode == QUERY_MODE_NEXT)
        {
            data = createSelectContentForPlaybackRequest((uint64_t)AHA_CONTENT_ID_NEXT);
        }
        else
        {
            data = createSelectContentForPlaybackRequest(m_nRequestedContentID);
        }
        break;
    case AHA_OPCODE_CHANGE_CLIENT_PLAYBACK_STATE:
        if(nQueryMode == QUERY_MODE_PLAY)
        {
            data = createChangeClientPlaybackRequest(AHA_PLAYBACK_PLAY);
        }
        else if(nQueryMode == QUERY_MODE_PAUSE)
        {
            data = createChangeClientPlaybackRequest(AHA_PLAYBACK_PAUSE);
        }
        else if(nQueryMode == QUERY_MODE_STOP)
        {
            data = createChangeClientPlaybackRequest(AHA_PLAYBACK_STOP);
        }
        break;
    case AHA_OPCODE_TIME_SHIFT_CONTENT:
        if(nQueryMode == QUERY_MODE_REWIND15)
        {
            data = createTimeShiftContentRequest(-15);
        }
        else if(nQueryMode == QUERY_MODE_FORWARD30)
        {
            data = createTimeShiftContentRequest(30);
        }
        break;
    case AHA_OPCODE_PERFORM_ACTION:
        if(nQueryMode == QUERY_MODE_VOTE_LIKE)
        {
            data = createPerformActionRequest(AHA_ACT_VOTE_LIKE, m_AhaClientInfo.nActiveContentID);
        }
        else if(nQueryMode == QUERY_MODE_VOTE_DISLIKE)
        {
            data = createPerformActionRequest(AHA_ACT_VOTE_DISLIKE, m_AhaClientInfo.nActiveContentID);
        }
        else if(nQueryMode == QUERY_MODE_UNVOTE)
        {
            data = createPerformActionRequest(AHA_ACT_UNVOTE, m_AhaClientInfo.nActiveContentID);
        }
        else if(nQueryMode == QUERY_MODE_LOG_NAVIGATE)
        {
            data = createPerformActionRequest(AHA_ACT_LOG_NAVIGATION, m_AhaClientInfo.nActiveContentID);
        }
        else if(nQueryMode == QUERY_MODE_LOG_CALL)
        {
            data = createPerformActionRequest(AHA_ACT_LOG_CALL, m_AhaClientInfo.nActiveContentID);
        }
        else if(nQueryMode == QUERY_MODE_PING)
        {
            data = createPerformActionRequest(AHA_ACT_PING, m_AhaClientInfo.nActiveContentID);
        }
        break;
    case AHA_OPCODE_REQUEST_CURRENT_CONTENT_INFORMATION:
        data = createRequestCurrentContentInfo();
        break;
    default:
        break;
    }

    //printBinaryByteArray(data);

    LOG_HIGH <<"\n encodeAndSendCommand with nOpCode: " << nOpCode << " trans ID:"<<generateUniqueQueryID(false)<< LOG_ENDL;

    //Add data to transaction table for the purpose of book keeping.
    addToTransactionTable(data, nQueryMode);
    m_bChannelBusy = true;

    sendData(data);

    LOG_HIGH <<"Sending binary data returns: "<< LOG_ENDL;
}

void CAhaAbsFrameTxRx::onPingCountdownTimeOut()
{
    //30 secs has elapsed since we received any notification from Aha
    //let us check if aha client is still alive.
    TAN_PRINTF("onPingCountdownTimeOut\n");
    ping(true); //ping with reset count.
}

void CAhaAbsFrameTxRx::onPingIntervalTimeOut()
{
    //retry ping without restarting the counter.
    TAN_PRINTF("onPingIntervalTimeOut\n");
    ping(false);
}

void CAhaAbsFrameTxRx::onReplyWaitTimeOut()
{
    uint16_t nLastTransactionID = generateUniqueQueryID(false);
    uint16_t nOpCode;
    AHA_QUERY_MODE_T nQueryMode;
    int i;
    LOG_HIGH <<"onReplyWaitTimeOut +" <<nLastTransactionID<< LOG_ENDL;

    for (i=0; i< m_TransactionRecordTable.size(); i++)
    {
        if(nLastTransactionID == m_TransactionRecordTable[i].nTransactionID)
        {
            nOpCode = m_TransactionRecordTable[i].nOpCode;
            nQueryMode = m_TransactionRecordTable[i].nQueryMode;
            m_TransactionRecordTable.remove(i);

            LOG_HIGH <<"Resending binary command after timeout: " << LOG_ENDL;
            m_bChannelBusy = false;
            sendCommandRequest(nOpCode, nQueryMode);
            break;
        }
    }
}

void CAhaAbsFrameTxRx::onDataReceived(QByteArray byteArray)
{
    QByteArray frameStartByte;
    QDataStream frameStartByteDS(&frameStartByte, QIODevice::WriteOnly);
    frameStartByteDS.setByteOrder(QDataStream::LittleEndian);
    frameStartByteDS<< (uint16_t) START_AHA_PACKAGE;

    QByteArray singleByteArray;
    uint16_t nLength, nArraySize;
    int nStartPackageIndex;
    uint16_t nPackageCount;
    LOG_HIGH <<"\n\n onDataReceived data size:" << byteArray.size() << LOG_ENDL;
//    LOG_HIGH<<"onDataReceived:"<<byteArray.toHex()<<LOG_ENDL;
//    printBinaryByteArray(byteArray);

    //sometimes data come in multiple packages.
    //let us cut the data into single packages.
    //1. Read through data until you find the start of package.
    //2. Read the length of data to check if it is at least 10 bytes.
    //3. Extract the package and call processing single package.

    m_ReceivedData.append(byteArray);

    if(m_bLongReceivedData)
    {
        //check teminal of long data.
        //LOG_HIGH<<"##### onDataReceived 111111 size :"<<m_ReceivedData.length()/*<<" data: "<<m_ReceivedData.toHex()*/<<LOG_ENDL;

        if(m_nLongReceivedData != m_ReceivedData.length())
        {
            LOG_HIGH<<"onDataReceived merging data...return..long:"<<m_nLongReceivedData<<" data length:"<<m_ReceivedData.length()<<LOG_ENDL;
            return;
        }
    }

    nArraySize = m_ReceivedData.size();
    nPackageCount = 0;
    nStartPackageIndex = 0;

    while(nStartPackageIndex < nArraySize)
    {
        nStartPackageIndex = m_ReceivedData.indexOf(frameStartByte,nStartPackageIndex);
        if( -1 == nStartPackageIndex)
        {
            LOG_HIGH <<"No start of package found." << LOG_ENDL;
            break;
        }
        LOG_HIGH<<"onDataReceived:"<<byteArray.toHex()<<LOG_ENDL;

        nLength = byteToUint16_t(m_ReceivedData, nStartPackageIndex + 2);

        if(nLength > nArraySize && false == m_bLongReceivedData)
        {
            m_bLongReceivedData = true;
            //m_pReceivedData.append(byteArray);
            m_nLongReceivedData = nLength;
            LOG_HIGH<<"%%%%% onDataReceived long data  size :"<<m_nLongReceivedData<<LOG_ENDL;
            //LOG_HIGH<<"onDataReceived size :"<<m_ReceivedData.length()<<" data: "<<m_ReceivedData.toHex()<<LOG_ENDL;
            return;
        }

        //verify the claimed length
        if((nStartPackageIndex+nLength) > nArraySize || 0 == nLength)
        {
            nStartPackageIndex++;
            LOG_HIGH <<"The nLength claim is invalid" << LOG_ENDL;
            continue;
        }
        singleByteArray.clear();
        singleByteArray = m_ReceivedData.mid(nStartPackageIndex, nLength);
        processSinglePackage(singleByteArray);
        nStartPackageIndex = nStartPackageIndex + nLength;
        nPackageCount++;
    }

    frameStartByte.clear();

    m_bLongReceivedData = false;
    m_ReceivedData.clear();

    LOG_HIGH <<"nPackageCount " << nPackageCount<< LOG_ENDL;
}

void CAhaAbsFrameTxRx::startPeerSession()
{
    TAN_PRINTF("CAhaAbsFrameTxRx::startPeerSession() \n");
    LOG_HIGH<<"CAhaAbsFrameTxRx::startPeerSession " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_START_PEER_SESSION, QUERY_MODE_UNKNOWN_MODE);
}

void CAhaAbsFrameTxRx::play()
{
    TAN_PRINTF("CAhaAbsFrameTxRx::play() \n");
    LOG_HIGH<<"CAhaAbsFrameTxRx::play " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_CHANGE_CLIENT_PLAYBACK_STATE, QUERY_MODE_PLAY);
}

void CAhaAbsFrameTxRx::stop()
{
    TAN_PRINTF("CAhaAbsFrameTxRx::stop() \n");
    LOG_HIGH<<"CAhaAbsFrameTxRx::stop " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_CHANGE_CLIENT_PLAYBACK_STATE, QUERY_MODE_STOP);
}

void CAhaAbsFrameTxRx::pause()
{
    TAN_PRINTF("CAhaAbsFrameTxRx::pause() \n");
    LOG_HIGH<<"CAhaAbsFrameTxRx::pause " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_CHANGE_CLIENT_PLAYBACK_STATE, QUERY_MODE_PAUSE);
}

void CAhaAbsFrameTxRx::back()
{
    TAN_PRINTF("CAhaAbsFrameTxRx::back() \n");
    LOG_HIGH<<"CAhaAbsFrameTxRx::back " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_SELECT_CONTENT_FOR_PLAYBACK, QUERY_MODE_BACK);
}

void CAhaAbsFrameTxRx::next()
{
    TAN_PRINTF("CAhaAbsFrameTxRx::next() \n");
    LOG_HIGH<<"CAhaAbsFrameTxRx::next " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_SELECT_CONTENT_FOR_PLAYBACK, QUERY_MODE_NEXT);
}

void CAhaAbsFrameTxRx::rewind15()
{
    TAN_PRINTF("CAhaAbsFrameTxRx::rewind15() \n");
    LOG_HIGH<<"CAhaAbsFrameTxRx::rewind15 " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_TIME_SHIFT_CONTENT, QUERY_MODE_REWIND15);
}

void CAhaAbsFrameTxRx::voteLike()
{
    TAN_PRINTF("CAhaAbsFrameTxRx::voteLike() \n");
    LOG_HIGH<<"CAhaAbsFrameTxRx::voteLike " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_PERFORM_ACTION, QUERY_MODE_VOTE_LIKE);
}

void CAhaAbsFrameTxRx::voteDislike()
{
    TAN_PRINTF("CAhaAbsFrameTxRx::voteDislike() \n");
    LOG_HIGH<<"CAhaAbsFrameTxRx::voteDislike " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_PERFORM_ACTION, QUERY_MODE_VOTE_DISLIKE);
}

void CAhaAbsFrameTxRx::unvote()
{
    TAN_PRINTF("CAhaAbsFrameTxRx::unvote() \n");
    LOG_HIGH<<"CAhaAbsFrameTxRx::unvote " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_PERFORM_ACTION, QUERY_MODE_UNVOTE);
}

void CAhaAbsFrameTxRx::logCall()
{
    TAN_PRINTF("CAhaAbsFrameTxRx::logCall() \n");
    LOG_HIGH<<"CAhaAbsFrameTxRx::logCall " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_PERFORM_ACTION, QUERY_MODE_LOG_CALL);
}

void CAhaAbsFrameTxRx::logNavigate()
{
    TAN_PRINTF("CAhaAbsFrameTxRx::logNavigate() \n");
    LOG_HIGH<<"CAhaAbsFrameTxRx::logNavigate " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_PERFORM_ACTION, QUERY_MODE_LOG_NAVIGATE);
}

void CAhaAbsFrameTxRx::ping(bool bResetCount)
{
    static int nPingCount = 0;
    if(bResetCount)
    {
        nPingCount = 0;
    }

    if(nPingCount < MAX_PING_RETRY)
    {
        nPingCount++;
        TAN_PRINTF("ping %d \n", nPingCount);
        sendCommandRequest(AHA_OPCODE_PERFORM_ACTION, QUERY_MODE_PING);
        m_PingIntervalTimer->start(TIME_PING_INTERVAL);
    }
    else
    {
        emit ahaPingTimeOut();
    }
}

void CAhaAbsFrameTxRx::forward30()
{
    TAN_PRINTF("CAhaAbsFrameTxRx::forward30() \n");
    LOG_HIGH<<"CAhaAbsFrameTxRx::forward30 " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_TIME_SHIFT_CONTENT, QUERY_MODE_FORWARD30);
}

void CAhaAbsFrameTxRx::queryCurrentContentDisplayInfo()
{
    LOG_HIGH<<"CAhaAbsFrameTxRx::queryCurrentContentDisplayInfo in..." <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_REQUEST_CURRENT_CONTENT_INFORMATION, QUERY_MODE_UNKNOWN_MODE);
}

void CAhaAbsFrameTxRx::querySelectStationForPlayback(uint64_t nStationID)
{
    m_nRequestedStationID = nStationID;
    LOG_HIGH<<"CAhaAbsFrameTxRx::querySelectStationForPlayback " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_SELECT_STATION_FOR_PLAYBACK, QUERY_MODE_UNKNOWN_MODE);
}

void CAhaAbsFrameTxRx::querySelectContentForPlayback(uint64_t nContentID)
{
    m_nRequestedContentID = nContentID;
    LOG_HIGH<<"CAhaAbsFrameTxRx::querySelectContentForPlayback " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_SELECT_CONTENT_FOR_PLAYBACK, QUERY_MODE_UNKNOWN_MODE);
}

void CAhaAbsFrameTxRx::queryCurrentStation(bool ignoreStationID)
{
    LOG_HIGH<<"CAhaAbsFrameTxRx::queryCurrentStation " <<LOG_ENDL;
    if(ignoreStationID)
    {
        sendCommandRequest(AHA_OPCODE_QUERY_CURRENT_STATION, QUERY_MODE_IGNORE_STATION_ID);
    }
    else
    {
        sendCommandRequest(AHA_OPCODE_QUERY_CURRENT_STATION, QUERY_MODE_UNKNOWN_MODE);
    }
}

void CAhaAbsFrameTxRx::queryPresetStationIDs()
{
    LOG_HIGH<<"CAhaAbsFrameTxRx::queryPresetStationIDs " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_QUERY_STATIONS, QUERY_MODE_UNKNOWN_MODE);
}

void CAhaAbsFrameTxRx::queryStationContentIDs()
{
    LOG_HIGH<<"CAhaAbsFrameTxRx::queryStationContentIDs " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_QUERY_STATION_CONTENT, QUERY_MODE_UNKNOWN_MODE);
}

void CAhaAbsFrameTxRx::queryActiveStationParam()
{
    LOG_HIGH<<"CAhaAbsFrameTxRx::queryActiveStationParam " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_QUERY_STATION_PARAMETERS, QUERY_MODE_ACTIVE_STATION_PARAM);
}

int CAhaAbsFrameTxRx::queryPresetStationsParam(int nStartPresetStationIndex)
{
    LOG_HIGH<<"[CAhaAbsFrameTxRx::queryPresetStationsParam] " <<nStartPresetStationIndex<<LOG_ENDL;
    if(nStartPresetStationIndex == 0)
    {
        //first query. clear all the Preset Station list info, we will insert things.
        m_PresetStationsInfo.clear();
    }
    if(nStartPresetStationIndex < m_AhaClientInfo.nQVectPresetStationID.count())
    {
        m_nPresetStationIndexToQuery = nStartPresetStationIndex;
        sendCommandRequest(AHA_OPCODE_QUERY_STATION_PARAMETERS, QUERY_MODE_PRESET_STATION_PARAM);
        return 0;
    }
    else
    {
        return -1; //preset index exceeds maximum preset index .
    }
}


void CAhaAbsFrameTxRx::queryActiveContentParam()
{
    LOG_HIGH<<"CAhaAbsFrameTxRx::queryActiveContentParam " <<LOG_ENDL;
    sendCommandRequest(AHA_OPCODE_QUERY_CONTENT_PARAMETERS, QUERY_MODE_ACTIVE_CONTENT_PARAM);
}

int CAhaAbsFrameTxRx::queryContentListParam(int nStartContentIndex)
{
    LOG_HIGH<<"CAhaAbsFrameTxRx::queryContentListParam " <<LOG_ENDL;
    if(nStartContentIndex == 0)
    {
        //first query. clear all the content list info, we will insert things.
        m_ContentListsInfo.clear();
    }
    if(nStartContentIndex < m_AhaClientInfo.nQVectListContentID.count())
    {
        m_nContentIndexToQuery = nStartContentIndex;
        sendCommandRequest(AHA_OPCODE_QUERY_CONTENT_PARAMETERS, QUERY_MODE_LIST_CONTENT_PARAM);
        return 0;
    }
    else
    {
        return -1; //contentindex exceeds maximum content index .
    }
}



uint16_t CAhaAbsFrameTxRx::decodeResponse(QByteArray &byteArray, uint16_t nOpCode, AHA_QUERY_MODE_T nQueryMode)
{
    LOG_HIGH<<"CAhaAbsFrameTxRx::decodeResponse op: "<< nOpCode<<LOG_ENDL;
    //LOG_HIGH <<"\n Decoding data with nOpcode: "<< nOpCode<< LOG_ENDL;
    //TAN_PRINTF("CAhaAbsFrameTxRx::decodeResponse   nOpCode = 0x%04x\n", nOpCode);

    uint16_t nErrorCode = AHA_ERR_NO_ERROR;
    uint64_t nStationID = 0;
    uint32_t nConnStatusCode = 0;

    //call the appropriate decoder function for each command type
    //hsryu_0612_update_optimize
    if(m_TopProcessPlayStation == true)
    {
        if(nOpCode == AHA_OPCODE_REQUEST_CURRENT_CONTENT_INFORMATION)
        {
            if(m_ActiveStationInfo.nStationID != m_nRequestedStationID)
            {
                return nErrorCode;
            }
            else
            {
                m_TopProcessPlayStation = false;
            }
        }
    }
    switch(nOpCode)
    {
    case AHA_OPCODE_START_PEER_SESSION:
        nErrorCode = decodeStartPeerSession(byteArray, m_AhaClientInfo);
        emit startPeerSessionDecoded(nErrorCode);
        break;
    case AHA_OPCODE_VEHICLE_INFORMATION_UPDATE:
        break;
    case AHA_OPCODE_QUERY_STATIONS:
        nErrorCode = decodeQueryStations(byteArray, m_AhaClientInfo);
        emit queryStationsDecoded(nErrorCode);
        break;
    case AHA_OPCODE_QUERY_STATION_PARAMETERS:
        if(nQueryMode == QUERY_MODE_ACTIVE_STATION_PARAM)
        {
            nErrorCode = decodeQuerySingleStationParameters(byteArray, m_ActiveStationInfo);
            emit activeStationParamDecoded(nErrorCode);
        }
        else if(nQueryMode == QUERY_MODE_PRESET_STATION_PARAM)
        {
            nErrorCode = decodeQueryMultipleStationParameters(byteArray, m_PresetStationsInfo);
            emit presetStationParamDecoded(nErrorCode, m_nPresetStationIndexToQuery);
        }
        break;
    case AHA_OPCODE_QUERY_STATION_CONTENT:
        nErrorCode = decodeQueryStationContent(byteArray, m_AhaClientInfo);
        emit queryStationContentDecoded(nErrorCode);
        break;
    case AHA_OPCODE_QUERY_CONTENT_PARAMETERS:
        if(nQueryMode == QUERY_MODE_ACTIVE_CONTENT_PARAM)
        {
            nErrorCode = decodeQuerySingleContentParameters(byteArray, m_ActiveContentInfo);
            emit activeContentParamDecoded(nErrorCode);
        }
        else if(nQueryMode == QUERY_MODE_LIST_CONTENT_PARAM)
        {
            nErrorCode = decodeQueryMultipleContentParameters(byteArray, m_ContentListsInfo);
            emit listContentParamDecoded(nErrorCode, m_nContentIndexToQuery);
        }
        break;
    case AHA_OPCODE_QUERY_BINARY_DATA:
        if(nQueryMode == QUERY_MODE_ALBUM_ART)
        {
            LOG_HIGH <<"\n ###### QUERY_MODE_ALBUM_ART data: "<< byteArray.size()<< LOG_ENDL;
            nErrorCode = decodeQueryBinaryData(byteArray, m_AlbumArtInfo);
            emit queryAlbumArtDecoded(nErrorCode);
        }
        else if(nQueryMode == QUERY_MODE_STATION_LOGO)
        {
            LOG_HIGH <<"\n ###### QUERY_MODE_STATION_LOGO data: "<< byteArray.size()<< LOG_ENDL;
            nErrorCode = decodeQueryBinaryData(byteArray, m_StationLogoInfo);
            emit queryStationLogoDecoded(nErrorCode);
        }
        else if(nQueryMode == QUERY_MODE_PARTNER_LOGO)
        {
            LOG_HIGH <<"\n ###### QUERY_MODE_PARTNER_LOGO data: "<< byteArray.size()<< LOG_ENDL;
            nErrorCode = decodeQueryBinaryData(byteArray, m_PartnerLogoInfo);
            emit queryPartnerLogoDecoded(nErrorCode);
        }
        else if(nQueryMode == QUERY_MODE_PRESET_LOGO)
        {
            LOG_HIGH <<"\n ###### QUERY_MODE_PRESET_LOGO data: "<< byteArray.size()<< LOG_ENDL;
            nErrorCode = decodeQueryBinaryData(byteArray, m_PresetLogoInfo);
            emit queryPresetLogoDecoded(nErrorCode);
        }
        else if(nQueryMode == QUERY_MODE_RATING_IMAGE)
        {
            LOG_HIGH <<"\n ###### QUERY_MODE_RATING_IMAGE data: "<< byteArray.size()<< LOG_ENDL;
            nErrorCode = decodeQueryBinaryData(byteArray, m_RatingImageInfo);
            emit queryRatingImageDecoded(nErrorCode);
        }
        else if(nQueryMode == QUERY_MODE_LIKE_IMAGE )
        {
            LOG_HIGH <<"\n ###### QUERY_MODE_LIKE_IMAGE data: "<< byteArray.size()<< LOG_ENDL;
            nErrorCode = decodeQueryBinaryData(byteArray, m_LikeImageInfo);
            emit queryLikeImageDecoded(nErrorCode);
        }
        break;
    case AHA_OPCODE_QUERY_CURRENT_STATION:
        if(nQueryMode == QUERY_MODE_IGNORE_STATION_ID )
        {
        LOG_HIGH<<"QUERY_MODE_IGNORE_STATION_ID 1111111 nActiveContentID:"<<m_AhaClientInfo.nActiveContentID<<LOG_ENDL;
            nErrorCode = decodeQueryCurrentStation(byteArray, m_AhaClientInfo, true);
            LOG_HIGH<<"QUERY_MODE_IGNORE_STATION_ID 2222222 nActiveContentID:"<<m_AhaClientInfo.nActiveContentID<<", error:"<<nErrorCode<<LOG_ENDL;
        }
        else
        {
            LOG_HIGH<<"QUERY_MODE_IGNORE_STATION_ID else 111111 nActiveContentID:"<<m_AhaClientInfo.nActiveContentID<<LOG_ENDL;
            nErrorCode = decodeQueryCurrentStation(byteArray, m_AhaClientInfo, false);
            LOG_HIGH<<"QUERY_MODE_IGNORE_STATION_ID else 222222222 nActiveContentID:"<<m_AhaClientInfo.nActiveContentID<<", error:"<<nErrorCode<<LOG_ENDL;
        }
        emit queryCurrentStationDecoded(nErrorCode);
        break;
    case AHA_OPCODE_SELECT_STATION_FOR_PLAYBACK:
        nErrorCode = decodeSelectStationForPlayback(byteArray);
        emit selectStationForPlaybackDecoded(nErrorCode);
        break;
    case AHA_OPCODE_SELECT_CONTENT_FOR_PLAYBACK:
        nErrorCode = decodeSelectContentForPlayback(byteArray);
        emit selectContentForPlaybackDecoded(nErrorCode);
        break;
    case AHA_OPCODE_CHANGE_CLIENT_PLAYBACK_STATE:
        LOG_HIGH<<"AHA_OPCODE_CHANGE_CLIENT_PLAYBACK_STATE 1111111 nActiveContentID:"<<m_AhaClientInfo.nActiveContentID<<LOG_ENDL;
        nErrorCode = decodeChangeClientPlayback(byteArray, m_AhaClientInfo);
        LOG_HIGH<<"AHA_OPCODE_CHANGE_CLIENT_PLAYBACK_STATE 2222222222 nActiveContentID:"<<m_AhaClientInfo.nActiveContentID<<LOG_ENDL;
        emit changeClientPlaybackDecoded(nErrorCode);
        break;
    case AHA_OPCODE_TIME_SHIFT_CONTENT:
        nErrorCode = decodeTimeShiftContent(byteArray);
        emit timeShiftContentDecoded(nErrorCode);
        break;
    case AHA_OPCODE_PERFORM_ACTION:
        nErrorCode = decodePerformAction(byteArray);
        emit performActionDecoded(nErrorCode);
        break;
    case AHA_OPCODE_REQUEST_CURRENT_CONTENT_INFORMATION:
        nErrorCode = decodeRequestCurrentContentInfo(byteArray, m_ActiveContentInfo);
        emit currentContentInfoDecoded(nErrorCode);
        break;
    //client initiated command.
    case AHA_OPCODE_NOTIFY_PLAYBACK_OF_NEW_STATION:
        nErrorCode = decodeNotifyPlaybackOfNewStation(byteArray, m_AhaClientInfo);
        emit notifyPlaybackOfNewStationDecoded(nErrorCode);
        break;
    case AHA_OPCODE_NOTIFY_NEW_CONTENT_PLAYBACK_STATE:
        LOG_HIGH<<"AHA_OPCODE_NOTIFY_NEW_CONTENT_PLAYBACK_STATE nActiveContentID:"<<m_AhaClientInfo.nActiveContentID<<LOG_ENDL;
        nErrorCode = decodeNotifyNewContentPlaybackState(byteArray, m_AhaClientInfo);
        LOG_HIGH<<"AHA_OPCODE_NOTIFY_NEW_CONTENT_PLAYBACK_STATE 2222222222 nActiveContentID:"<<m_AhaClientInfo.nActiveContentID<<LOG_ENDL;
        emit notifyNewContentPlaybackStateDecoded(nErrorCode);
        break;
    case AHA_OPCODE_NOTIFY_NEW_CONTENT_AVAILABLE:
        nErrorCode = decodeNotifyNewContentAvailable(byteArray, nStationID);
        emit notifyNewContentAvailableDecoded(nErrorCode, nStationID);
        break;
    case AHA_OPCODE_NOTIFY_CONTENT_DELETED:
        nErrorCode = decodeNotifyContentDeleted(byteArray, m_ContentListsInfo);
        emit notifyContentDeletedDecoded(nErrorCode);
        break;
    case AHA_OPCODE_NOTIFY_CLIENT_STATUS_UPDATE:
        nErrorCode = decodeNotifyClientStatusUpdate(byteArray, nConnStatusCode);
        emit notifyClientStatusUpdateDecoded(nErrorCode, nConnStatusCode);
        break;
    case AHA_OPCODE_NOTIFY_CLIENT_POSITION_UPDATE:
    //  decodeNotifyClientPositionUpdate(byteArray, AhaClientInfo);
        break;
    case AHA_OPCODE_NOTIFY_3rd_PARTY_CONNECTIVITY_CHANGE:
    //  decodeNotify3rdPartyConnectivityChange(byteArray, AhaClientInfo);
        break;
    case AHA_OPCODE_NOTIFY_SUBSCRIBED_STATION_LIST_CHANGE:
        nErrorCode = decodeNotifySubscribedStationListChange();
        break;
    case AHA_OPCODE_NOTIFY_PENDING_REQUEST:
    //  decodeNotifyPendingRequest(byteArray);
        break;
    default:
        break;
    }
    return nErrorCode;
}

void CAhaAbsFrameTxRx::continueAlbumArtDownload()
{
    if(m_AlbumArtInfo.nIsDownloadComplete == 0)
    {
        //we have to count the number of repetition here to prevent infinite loop
        if(m_AlbumArtInfo.nFragmentCount < N_MAX_DATA_FRAGMENT)
        {
            LOG_HIGH <<"Continue to download.. nFragmentCount: "<< m_AlbumArtInfo.nFragmentCount<< LOG_ENDL;
            sendCommandRequest(AHA_OPCODE_QUERY_BINARY_DATA, QUERY_MODE_ALBUM_ART);
        }
        else
        {
            LOG_HIGH <<"ERROR: too many data fragment or possible infinite loop: "<< m_AlbumArtInfo.nFragmentCount<< LOG_ENDL;
        }
    }
    else
    {
        LOG_HIGH <<"Error, stop requesting binary data" << LOG_ENDL;
    }
}

void CAhaAbsFrameTxRx::continuePartnerLogoDownload()
{
    //we have to count the number of repetition here to prevent infinite loop
    if(m_PartnerLogoInfo.nIsDownloadComplete == 0)
    {
        if(m_PartnerLogoInfo.nFragmentCount < N_MAX_DATA_FRAGMENT)
        {
            LOG_HIGH <<"Continue to download.. nFragmentCount: %d\n"<< m_PartnerLogoInfo.nFragmentCount<< LOG_ENDL;
            sendCommandRequest(AHA_OPCODE_QUERY_BINARY_DATA, QUERY_MODE_PARTNER_LOGO);
        }
        else
        {
            LOG_HIGH <<"ERROR: too many data fragment or possible infinite loop: "<< m_PartnerLogoInfo.nFragmentCount<< LOG_ENDL;
        }
    }
    else //-1 means error occur, should not reattempt.
    {
        LOG_HIGH <<"Error, stop requesting binary data" << LOG_ENDL;
    }
}

void CAhaAbsFrameTxRx::continueStationLogoDownload()
{
    //we have to count the number of repetition here to prevent infinite loop
    if(m_StationLogoInfo.nIsDownloadComplete == 0)
    {
        if(m_StationLogoInfo.nFragmentCount < N_MAX_DATA_FRAGMENT)
        {
            LOG_HIGH <<"Continue to download.. nFragmentCount: "<< m_StationLogoInfo.nFragmentCount<< LOG_ENDL;
            sendCommandRequest(AHA_OPCODE_QUERY_BINARY_DATA, QUERY_MODE_STATION_LOGO);
        }
        else
        {
            LOG_HIGH <<"ERROR: too many data fragment or possible infinite loop: "<< m_StationLogoInfo.nFragmentCount<< LOG_ENDL;
        }
    }
    else //-1 means error occur, should not reattempt.
    {
        LOG_HIGH <<"Error, stop requesting binary data" << LOG_ENDL;
    }
}

void CAhaAbsFrameTxRx::continuePresetLogoDownload(int nIndex)
{
    //we have to count the number of repetition here to prevent infinite loop
    if(m_PresetLogoInfo.nIsDownloadComplete == 0)
    {
        if(m_PresetLogoInfo.nFragmentCount < N_MAX_DATA_FRAGMENT)
        {
            LOG_HIGH <<"Continue to download.. nFragmentCount: "<< m_PresetLogoInfo.nFragmentCount<< LOG_ENDL;
            m_PresetQueryIndex = nIndex;
            sendCommandRequest(AHA_OPCODE_QUERY_BINARY_DATA, QUERY_MODE_PRESET_LOGO);
        }
        else
        {
            LOG_HIGH <<"ERROR: too many data fragment or possible infinite loop: "<< m_PresetLogoInfo.nFragmentCount<< LOG_ENDL;
        }
    }
    else //-1 means error occur, should not reattempt.
    {
        LOG_HIGH <<"Error, stop requesting binary data" << LOG_ENDL;
    }
}

void CAhaAbsFrameTxRx::continueRatingImageDownload()
{
    //we have to count the number of repetition here to prevent infinite loop
    if(m_RatingImageInfo.nIsDownloadComplete == 0)
    {
        if(m_RatingImageInfo.nFragmentCount < N_MAX_DATA_FRAGMENT)
        {
            LOG_HIGH <<"Continue to download.. nFragmentCount: "<< m_RatingImageInfo.nFragmentCount<< LOG_ENDL;
            sendCommandRequest(AHA_OPCODE_QUERY_BINARY_DATA, QUERY_MODE_RATING_IMAGE);
        }
        else
        {
            LOG_HIGH <<"ERROR: too many data fragment or possible infinite loop: "<< m_RatingImageInfo.nFragmentCount<< LOG_ENDL;
        }
    }
    else //-1 means error occur, should not reattempt.
    {
        LOG_HIGH <<"Error, stop requesting binary data" << LOG_ENDL;
    }
}

void CAhaAbsFrameTxRx::continueLikeImageDownload(int nIndex)
{
    //we have to count the number of repetition here to prevent infinite loop
    if(m_LikeImageInfo.nIsDownloadComplete == 0)
    {
        if(m_LikeImageInfo.nFragmentCount < N_MAX_DATA_FRAGMENT)
        {
            LOG_HIGH <<"Continue to download.. nFragmentCount: "<< m_LikeImageInfo.nFragmentCount<< LOG_ENDL;
            m_LikeQueryIndex = nIndex;
            sendCommandRequest(AHA_OPCODE_QUERY_BINARY_DATA, QUERY_MODE_LIKE_IMAGE);
        }
        else
        {
            LOG_HIGH <<"ERROR: too many data fragment or possible infinite loop: %d\n"<< m_LikeImageInfo.nFragmentCount<< LOG_ENDL;
        }
    }
    else //-1 means error occur, should not reattempt.
    {
        LOG_HIGH <<"Error, stop requesting binary data" << LOG_ENDL;
    }
}

void CAhaAbsFrameTxRx::startAlbumArtDownload()
{
    LOG_HIGH<<"[CAhaAbsFrameTxRx::startAlbumArtDownload] in...url:"<<m_ActiveContentInfo.strImageURL<<LOG_ENDL;
    //let us check first if the file already existed.
    QString filePath;
    QFileInfo fileInfo;

    filePath = generateFileName(m_ActiveContentInfo.strImageURL);
    filePath.prepend(AHA_ALBUM_ART_PATH);
    fileInfo = filePath;

    LOG_HIGH<<"[CAhaAbsFrameTxRx::startAlbumArtDownload] filepath:"<<filePath<<LOG_ENDL;

    if(fileInfo.exists() || m_ActiveContentInfo.strImageURL.length() == 0)
    {
        LOG_HIGH <<"album art existed, do not download length:" <<m_ActiveContentInfo.strImageURL.length()<< LOG_ENDL;
        emit albumArtProcessFinished(filePath);
    }
    else
    {
        LOG_HIGH<<"[CAhaAbsFrameTxRx::startAlbumArtDownload] start image download..."<<LOG_ENDL;
        cancelBinaryImageDownload(QUERY_MODE_ALBUM_ART);
        resetAlbumArtInfo();
        sendCommandRequest(AHA_OPCODE_QUERY_BINARY_DATA, QUERY_MODE_ALBUM_ART);
    }
}

void CAhaAbsFrameTxRx::startPartnerLogoDownload()
{
    LOG_HIGH<<"[CAhaAbsFrameTxRx::startPartnerLogoDownload] in..."<<LOG_ENDL;
    QString filePath;
    QFileInfo fileInfo;

    filePath = generateFileName(m_ActiveStationInfo.strPartnerURL);
    filePath.prepend(AHA_PARTNER_LOGO_PATH);
    fileInfo = filePath;

    if(fileInfo.exists() || m_ActiveStationInfo.strPartnerURL.length() == 0)
    {
        LOG_HIGH <<"partnerlogo existed, do not download " << LOG_ENDL;
        emit partnerLogoProcessFinished(filePath);
    }
    else
    {
        cancelBinaryImageDownload(QUERY_MODE_PARTNER_LOGO);
        resetPartnerLogoInfo();
        sendCommandRequest(AHA_OPCODE_QUERY_BINARY_DATA, QUERY_MODE_PARTNER_LOGO);
    }
}


void CAhaAbsFrameTxRx::startStationLogoDownload()
{
    LOG_HIGH<<"[CAhaAbsFrameTxRx::startStationLogoDownload] in..."<<LOG_ENDL;
    QString filePath;
    QFileInfo fileInfo;

    filePath = generateFileName(m_ActiveStationInfo.strStationIconURL);
    filePath.prepend(AHA_STATION_LOGO_PATH);
    fileInfo = filePath;

    if(fileInfo.exists() || m_ActiveStationInfo.strStationIconURL.length() == 0)
    {
        LOG_HIGH <<"stationLogo existed, do not download " << LOG_ENDL;
        emit stationLogoProcessFinished(filePath);
    }
    else
    {
        cancelBinaryImageDownload(QUERY_MODE_STATION_LOGO);
        resetStationLogoInfo();
        sendCommandRequest(AHA_OPCODE_QUERY_BINARY_DATA, QUERY_MODE_STATION_LOGO);
    }
}

qulonglong CAhaAbsFrameTxRx::getPresetStationID(int nIndex)
{
    return m_PresetStationsInfo[nIndex].nStationID;
}

uint8_t CAhaAbsFrameTxRx::getLBSFlag(int nIndex)
{
    return m_PresetStationsInfo[nIndex].nIsLBS;
}

void CAhaAbsFrameTxRx::startPresetLogoDownload(int nIndex)
{
    LOG_HIGH<<"[CAhaAbsFrameTxRx::startPresetLogoDownload] in..."<<LOG_ENDL;
    QString filePath;
    QFileInfo fileInfo;

    filePath = generateFileName(m_PresetStationsInfo[nIndex].strStationIconURL);
    filePath.prepend(AHA_STATION_LOGO_PATH);
    fileInfo = filePath;

    LOG_HIGH <<"startPresetLogoDownload: %d\n"<< nIndex<< LOG_ENDL;
    if(fileInfo.exists() || m_PresetStationsInfo[nIndex].strStationIconURL.length() == 0)
    {
        LOG_HIGH <<"presetLogo[%d] existed, do not download \n"<< nIndex<< LOG_ENDL;
        emit presetLogoProcessFinished(filePath);
    }
    else
    {
        cancelBinaryImageDownload(QUERY_MODE_PRESET_LOGO);
        resetPresetLogoInfo();
        m_PresetQueryIndex = nIndex;
        sendCommandRequest(AHA_OPCODE_QUERY_BINARY_DATA, QUERY_MODE_PRESET_LOGO);
    }
}

void CAhaAbsFrameTxRx::startRatingImageDownload()
{
    QString filePath;
    QFileInfo fileInfo;

    filePath = generateFileName(m_ActiveContentInfo.strRatingImageURL);
    filePath.prepend(AHA_CONTENT_IMAGES_PATH);
    fileInfo = filePath;

    LOG_HIGH <<"[CAhaAbsFrameTxRx::startRatingImageDownload] url:" <<m_ActiveContentInfo.strRatingImageURL<<", filepath:"<<filePath<< LOG_ENDL;
    if(fileInfo.exists() || m_ActiveContentInfo.strRatingImageURL.length() == 0 )
    {
        LOG_HIGH <<"startRatingImageDownload existed or url is null, do not download " << LOG_ENDL;
        emit ratingImageProcessFinished(filePath);
    }
    else
    {
        cancelBinaryImageDownload(QUERY_MODE_RATING_IMAGE);
        resetRatingImageInfo();
        sendCommandRequest(AHA_OPCODE_QUERY_BINARY_DATA, QUERY_MODE_RATING_IMAGE);
    }
}

void CAhaAbsFrameTxRx::startLikeImageDownload(int nIndex)
{
    QString filePath, filePathOrig;
    QFileInfo fileInfo;

    LOG_HIGH <<"startLikeImageDownload: %d\n"<< nIndex<< LOG_ENDL;

    filePathOrig = getLikeImageFilePath(nIndex);
    if(filePathOrig.length() == 0)
    {
        LOG_HIGH <<"filepathOrig.length() is zero " << LOG_ENDL;
        filePathOrig.clear();
        emit likeImageProcessFinished(filePathOrig);
        return;     //wsuk.kim 130718 ITS_0177783 didn`t display selected BAN img
    }

    filePath = generateFileName(filePathOrig);
    filePath.prepend(AHA_CONTENT_IMAGES_PATH);
    fileInfo = filePath;

    if(fileInfo.exists())
    {
        LOG_HIGH <<"startLikeImageDownload existed, do not download " << LOG_ENDL;
        emit likeImageProcessFinished(filePath);
    }
    else
    {
        cancelBinaryImageDownload(QUERY_MODE_LIKE_IMAGE);
        resetLikeImageInfo();
        m_LikeQueryIndex = nIndex;
        sendCommandRequest(AHA_OPCODE_QUERY_BINARY_DATA, QUERY_MODE_LIKE_IMAGE);
    }
}

QString CAhaAbsFrameTxRx::getLikeImageFilePath(int nIndex)
{
    QString filePath;
    uint8_t nCanLike, nCanDislike;

    nCanLike = m_ActiveContentInfo.nCanLike;
    nCanDislike = m_ActiveContentInfo.nCanDislike;

    filePath.clear();
    switch(nIndex)
    {
    case AHA_IMAGE_LIKE_S:
        if(nCanLike)
        {
            filePath = m_ActiveStationInfo.strLikeSelectedURL;
        }
        break;
    case AHA_IMAGE_LIKE_U:
        if(nCanLike)
        {
            filePath = m_ActiveStationInfo.strLikeUnselectedURL;
        }
        break;
    case AHA_IMAGE_DISLIKE_S:
        if(nCanDislike)
        {
            filePath = m_ActiveStationInfo.strDislikeSelectedURL;
        }
        break;
    case AHA_IMAGE_DISLIKE_U:
        if(nCanDislike)
        {
            filePath = m_ActiveStationInfo.strDislikeUnselectedURL;
        }
        break;
    default:
        break;
    }
    return filePath;
}


void CAhaAbsFrameTxRx::cancelBinaryImageDownload(AHA_QUERY_MODE_T nQueryMode)
{
    //LOG_HIGH<<"[CAhaAbsFrameTxRx::cancelBinaryImageDownload] in...mode:"<<nQueryMode<<LOG_ENDL;
    //look for any entry in the queue and in the transaction record.
    //if you delete it from entry, the response will be ignored.
    int i;

    for(i = 0; i< m_PendingTransactionQueue.count(); i++)
    {
        if(m_PendingTransactionQueue[i].nQueryMode == nQueryMode)
            m_PendingTransactionQueue.remove(i);
    }
    for(i = 0; i< m_TransactionRecordTable.count(); i++)
    {
        if(m_TransactionRecordTable[i].nQueryMode == nQueryMode)
            m_TransactionRecordTable.remove(i);
    }
}

//wsuk.kim ISV_75730 station_remain
void CAhaAbsFrameTxRx::cancelRecievingInterruptQuery(AHA_QUERY_MODE_T nQueryMode)
{
    LOG_HIGH<<"[CAhaAbsFrameTxRx::cancelRecievingInterruptQuery] in...mode:"<<nQueryMode<<LOG_ENDL;
    //look for any entry in the queue and in the transaction record.
    //if you delete it from entry, the response will be ignored.
    int i;

    for(i = 0; i< m_PendingTransactionQueue.count(); i++)
    {
        if(m_PendingTransactionQueue[i].nQueryMode == nQueryMode)
            m_PendingTransactionQueue.remove(i);
    }
    for(i = 0; i< m_TransactionRecordTable.count(); i++)
    {
        if(m_TransactionRecordTable[i].nQueryMode == nQueryMode)
            m_TransactionRecordTable.remove(i);
    }
}
//wsuk.kim ISV_75730 station_remain

void CAhaAbsFrameTxRx::printAhaClientInfo()
{
    int nPresetCount, i, nContentCount;

    LOG_HIGH <<"\nprintAhaClientInfo" << LOG_ENDL;

    LOG_HIGH <<"\t m_AhaClientInfo.nGPSAvailable: %d\n"<< m_AhaClientInfo.nGPSAvailable<< LOG_ENDL;
    LOG_HIGH <<"\t m_AhaClientInfo.strAppVersion: %s\n"<< m_AhaClientInfo.strAppVersion.toUtf8().data()<< LOG_ENDL;
    LOG_HIGH <<"\t m_AhaClientInfo.strDevModel: %s\n"<< m_AhaClientInfo.strDevModel.toUtf8().data()<< LOG_ENDL;
    LOG_HIGH <<"\t m_AhaClientInfo.strDevSoftVersion: %s\n"<< m_AhaClientInfo.strDevSoftVersion.toUtf8().data()<< LOG_ENDL;
    LOG_HIGH <<"\t m_AhaClientInfo.nActiveStationID: %llu\n"<< m_AhaClientInfo.nActiveStationID<< LOG_ENDL;
    LOG_HIGH <<"\t m_AhaClientInfo.nActiveContentID: %llu\n"<< m_AhaClientInfo.nActiveContentID<< LOG_ENDL;
    LOG_HIGH <<"\t m_AhaClientInfo.nPlaybackState: %d\n"<< m_AhaClientInfo.nPlaybackState<< LOG_ENDL;
    LOG_HIGH <<"\t m_AhaClientInfo.nElapsedSecs: %d\n"<< m_AhaClientInfo.nElapsedSecs<< LOG_ENDL;

    nPresetCount = m_AhaClientInfo.nQVectPresetStationID.count();
    for(i=0; i< nPresetCount;i++)
    {
        LOG_HIGH <<"\t m_AhaClientInfo.nQVectPresetStationID[%d] i: "<< i<<" id: "<< m_AhaClientInfo.nQVectPresetStationID[i]<< LOG_ENDL;
    }
    nContentCount = m_AhaClientInfo.nQVectListContentID.count();
    for(i=0; i< nContentCount;i++)
    {
        LOG_HIGH <<"\t m_AhaClientInfo.nQVectListContentID[%d] i: "<< i<<" id: "<< m_AhaClientInfo.nQVectListContentID[i]<< LOG_ENDL;
    }
}

void CAhaAbsFrameTxRx::printActiveStationInfo()
{
    LOG_HIGH <<"\nprintActiveStationInfo" << LOG_ENDL;
//#ifdef LATE_ICON
    LOG_HIGH <<"\t m_ActiveStationInfo.nStationID: "<< m_ActiveStationInfo.nStationID<<LOG_ENDL;
    LOG_HIGH <<"\t m_ActiveStationInfo.nLikeAllowed: "<< m_ActiveStationInfo.nLikeAllowed<<LOG_ENDL;
    LOG_HIGH <<"\t m_ActiveStationInfo.nReadOnly: "<< m_ActiveStationInfo.nReadOnly<<LOG_ENDL;
    LOG_HIGH <<"\t m_ActiveStationInfo.nIsTwitter: "<< m_ActiveStationInfo.nIsTwitter<<LOG_ENDL;
    LOG_HIGH <<"\t m_ActiveStationInfo.nIsLBS: "<< m_ActiveStationInfo.nIsLBS<<LOG_ENDL;
    LOG_HIGH <<"\t m_ActiveStationInfo.nCanTimeShift: "<< m_ActiveStationInfo.nCanTimeShift<<LOG_ENDL;
    LOG_HIGH <<"\t m_ActiveStationInfo.nIsDynamic: "<< m_ActiveStationInfo.nIsDynamic<<LOG_ENDL;
    LOG_HIGH <<"\t m_ActiveStationInfo.nIsFacebook: "<< m_ActiveStationInfo.nIsFacebook<<LOG_ENDL;
    LOG_HIGH <<"\t m_ActiveStationInfo.nCanPostText: "<< m_ActiveStationInfo.nCanPostText<<LOG_ENDL;
    LOG_HIGH <<"\t m_ActiveStationInfo.nRecordTime: "<< m_ActiveStationInfo.nRecordTime<<LOG_ENDL;
    LOG_HIGH <<"\t m_ActiveStationInfo.strStationIconURL: "<< m_ActiveStationInfo.strStationIconURL.toUtf8().data()<<LOG_ENDL;
    LOG_HIGH <<"\t m_ActiveStationInfo.strStationFullName: "<< m_ActiveStationInfo.strStationFullName.toUtf8().data()<<LOG_ENDL;
    LOG_HIGH <<"\t m_ActiveStationInfo.strPartnerURL: "<< m_ActiveStationInfo.strPartnerURL.toUtf8().data()<<LOG_ENDL;
    LOG_HIGH <<"\t m_ActiveStationInfo.strDislikeSelectedURL: "<< m_ActiveStationInfo.strDislikeSelectedURL.toUtf8().data()<<LOG_ENDL;
    LOG_HIGH <<"\t m_ActiveStationInfo.strDislikeUnselectedURL: "<< m_ActiveStationInfo.strDislikeUnselectedURL.toUtf8().data()<<LOG_ENDL;
    LOG_HIGH <<"\t m_ActiveStationInfo.strLikeSelectedURL: "<< m_ActiveStationInfo.strLikeSelectedURL.toUtf8().data()<<LOG_ENDL;
    LOG_HIGH <<"\t m_ActiveStationInfo.strLikeUnselectedURL: "<< m_ActiveStationInfo.strLikeUnselectedURL.toUtf8().data()<<LOG_ENDL;
//#endif
}



void CAhaAbsFrameTxRx::printPresetStationsInfo()
{
     LOG_HIGH <<"CAhaAbsFrameTxRx::printPresetStationsInfo" << LOG_ENDL;
     int nPresetCount = m_PresetStationsInfo.size();
//#ifdef LATE_ICON
    for (int i = 0; i < nPresetCount; i++)
    {
        LOG_HIGH <<"\t m_PresetStationsInfo["<<i<<"].nStationID: " <<m_PresetStationsInfo[i].nStationID<<LOG_ENDL;
        LOG_HIGH <<"\t m_PresetStationsInfo[ "<<i<<"].strStationFullName:"<< m_PresetStationsInfo[i].strStationFullName.toUtf8().data()<<LOG_ENDL;
        LOG_HIGH <<"\t m_PresetStationsInfo[ "<<i<<"].strStationIconURL:"<< m_PresetStationsInfo[i].strStationIconURL.toUtf8().data()<<LOG_ENDL;
        LOG_HIGH <<"\t m_PresetStationsInfo[ "<<i<<"].nIsLBS:" <<m_PresetStationsInfo[i].nIsLBS<<LOG_ENDL;
    }
//#endif
}

void CAhaAbsFrameTxRx::printContentListInfo()
{
    uint16_t i, nContentCount;
    nContentCount = m_ContentListsInfo.count();

    LOG_HIGH <<"\n printContentListInfo " << LOG_ENDL;

    for (i = 0; i < nContentCount; i++)
    {
        LOG_HIGH <<"\t m_ContentListsInfo["<<i<<"].nStationID:"<< m_ContentListsInfo[i].nContentID<<LOG_ENDL;
        LOG_HIGH <<"\t m_ContentListsInfo["<<i<<"].strStationFullName:"<< m_ContentListsInfo[i].strTitle.toUtf8().data()<<LOG_ENDL;
    }
}

void CAhaAbsFrameTxRx::printActiveContentInfo()
{
    int nInfoStringCount, i;

    LOG_HIGH <<"\nprintActiveContentInfo" << LOG_ENDL;

    LOG_HIGH <<"\t	m_ActiveContentInfo.nContentID : "<<	m_ActiveContentInfo.nContentID <<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.nLiked : "<<	m_ActiveContentInfo.nLiked	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.nCanPause : "<<	m_ActiveContentInfo.nCanPause	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.nCanNext :	"<<	m_ActiveContentInfo.nCanNext	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.nCanBack :	"<<	m_ActiveContentInfo.nCanBack	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.nCanChange : "<<	m_ActiveContentInfo.nCanChange	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.nCanTimeShift : "<<	m_ActiveContentInfo.nCanTimeShift	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.nCanLike : "<<	m_ActiveContentInfo.nCanLike	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.nCanDislike : "<<	m_ActiveContentInfo.nCanDislike	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.nCanUnvote : "<<	m_ActiveContentInfo.nCanUnvote	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.nDisliked : "<<	m_ActiveContentInfo.nDisliked	<<LOG_ENDL;

    LOG_HIGH <<"\t	m_ActiveContentInfo.dbLon : "<<	m_ActiveContentInfo.dbLon	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.dbLat : "<<	m_ActiveContentInfo.dbLat	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.dbRating : "<<	m_ActiveContentInfo.dbRating	<<LOG_ENDL;

    LOG_HIGH <<"\t	m_ActiveContentInfo.strTitle : "<<	m_ActiveContentInfo.strTitle.toUtf8().data()	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.strImageURL : "<<	m_ActiveContentInfo.strImageURL.toUtf8().data()	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.strZip : "<<	m_ActiveContentInfo.strZip.toUtf8().data()	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.strAddress1 : "<<	m_ActiveContentInfo.strAddress1.toUtf8().data()	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.strState :	"<<	m_ActiveContentInfo.strState.toUtf8().data()	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.strCity : "<<	m_ActiveContentInfo.strCity.toUtf8().data()	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.strPhone :	"<<	m_ActiveContentInfo.strPhone.toUtf8().data()	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.strFullAddress :"<<	m_ActiveContentInfo.strFullAddress.toUtf8().data()	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.strAddress2 : "<<	m_ActiveContentInfo.strAddress2.toUtf8().data()	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.strCountryCode : "<<	m_ActiveContentInfo.strCountryCode.toUtf8().data()	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.strLangCode : "<<	m_ActiveContentInfo.strLangCode.toUtf8().data()	<<LOG_ENDL;
    LOG_HIGH <<"\t	m_ActiveContentInfo.strRatingImageURL : "<<	m_ActiveContentInfo.strRatingImageURL.toUtf8().data()	<<LOG_ENDL;

    nInfoStringCount = m_ActiveContentInfo.strQVectContentInfoString.count();

    for(i=0;i<nInfoStringCount;i++)
    {
        LOG_HIGH <<"\t m_ActiveContentInfo.strQVectContentInfoString["<< i<<"] "<<m_ActiveContentInfo.strQVectContentInfoString[i].toUtf8().data() <<LOG_ENDL;
    }
}

void CAhaAbsFrameTxRx::createTimers()
{
    //create m_ReplyWaitTimer instances
    if(m_ReplyWaitTimer != NULL)
        delete m_ReplyWaitTimer;

    m_ReplyWaitTimer = new QTimer(this);
    if(m_ReplyWaitTimer)
    {
        m_ReplyWaitTimer->setSingleShot(true);
        connect(m_ReplyWaitTimer, SIGNAL(timeout()), this, SLOT(onReplyWaitTimeOut()));
    }

    //create m_PingCountdownTimer instances
    if(m_PingCountdownTimer != NULL)
        delete m_PingCountdownTimer;

    m_PingCountdownTimer = new QTimer(this);
    if(m_PingCountdownTimer)
    {
        m_PingCountdownTimer->setSingleShot(true);
        connect(m_PingCountdownTimer, SIGNAL(timeout()), this, SLOT(onPingCountdownTimeOut()));
    }

    //create m_PingIntervalTimer instances
    if(m_PingIntervalTimer != NULL)
        delete m_PingIntervalTimer;

    m_PingIntervalTimer = new QTimer(this);
    if(m_PingIntervalTimer)
    {
        m_PingIntervalTimer->setSingleShot(true);
        connect(m_PingIntervalTimer, SIGNAL(timeout()), this, SLOT(onPingIntervalTimeOut()));
    }
}

void CAhaAbsFrameTxRx::destroyTimers()
{
    if(m_ReplyWaitTimer!=NULL)
    {
        disconnect(m_ReplyWaitTimer, SIGNAL(timeout()), this, SLOT(onReplyWaitTimeOut()));
        delete m_ReplyWaitTimer;
        m_ReplyWaitTimer = NULL;
    }

    if(m_PingCountdownTimer!=NULL)
    {
        disconnect(m_PingCountdownTimer, SIGNAL(timeout()), this, SLOT(onPingCountdownTimeOut()));
        delete m_PingCountdownTimer;
        m_PingCountdownTimer = NULL;
    }

    if(m_PingIntervalTimer!=NULL)
    {
        disconnect(m_PingIntervalTimer, SIGNAL(timeout()), this, SLOT(onPingIntervalTimeOut()));
        delete m_PingIntervalTimer;
        m_PingIntervalTimer = NULL;
    }
}

void CAhaAbsFrameTxRx::resetReplyWaitTimer(uint16_t nTransactionID)
{
    uint16_t nLastTransactionID = generateUniqueQueryID(false);
//    LOG_HIGH <<"resetReplyWaitTimer " << LOG_ENDL;

    LOG_HIGH <<"nLastTransactionID : "<< nLastTransactionID<<", nTransactionID : "<< nTransactionID<<LOG_ENDL;

//    LOG_HIGH<<"CAhaAbsFrameTxRx::resetReplyWaitTimer id: "<<nTransactionID <<LOG_ENDL;

    if(nTransactionID == nLastTransactionID)
    {
        LOG_HIGH <<"Response received before timeout." << LOG_ENDL;
        m_ReplyWaitTimer->stop();
        m_bChannelBusy = false;
        processPendingRequest();
    }
    else
    {
        LOG_HIGH <<"Irrelevant or old command or Aha client message" << LOG_ENDL;
    }
}

void CAhaAbsFrameTxRx::resetMemberVariables()
{
    m_TransactionRecordTable.clear();
    m_PendingTransactionQueue.clear();
    m_PresetStationsInfo.clear();
    m_ContentListsInfo.clear();

    m_bChannelBusy = false;
    m_bLaunchRequested = false;
    m_bInitCompletionReceived = false;
    m_TopProcessPlayStation = false; //hsryu_0612_update_optimize

    m_nRequestedStationID = 0;
    m_nRequestedContentID = 0;

    m_PresetQueryIndex = 0;
    m_LikeQueryIndex = 0;

    m_AhaClientInfo.clear();
    m_ActiveStationInfo.clear();
    m_ActiveContentInfo.clear();

    //resetAhaClientInfo();
    //resetActiveStationInfo();
    //resetActiveContentInfo();
    resetAlbumArtInfo();
    resetStationLogoInfo();
    resetPartnerLogoInfo();
    resetPresetLogoInfo();
    resetRatingImageInfo();
    resetLikeImageInfo();
}

void CAhaAbsFrameTxRx::resetAlbumArtInfo()
{
    m_AlbumArtInfo.imageDataByte.clear();
    m_AlbumArtInfo.nIsDownloadComplete = 0;
    m_AlbumArtInfo.nFragmentCount = 0;
    m_AlbumArtInfo.nOffset = 0;
}
void CAhaAbsFrameTxRx::resetStationLogoInfo()
{
    m_StationLogoInfo.imageDataByte.clear();
    m_StationLogoInfo.nIsDownloadComplete = 0;
    m_StationLogoInfo.nFragmentCount = 0;
    m_StationLogoInfo.nOffset = 0;
}
void CAhaAbsFrameTxRx::resetPartnerLogoInfo()
{
    m_PartnerLogoInfo.imageDataByte.clear();
    m_PartnerLogoInfo.nIsDownloadComplete = 0;
    m_PartnerLogoInfo.nFragmentCount = 0;
    m_PartnerLogoInfo.nOffset = 0;
}

void CAhaAbsFrameTxRx::resetPresetLogoInfo()
{
    m_PresetLogoInfo.imageDataByte.clear();
    m_PresetLogoInfo.nIsDownloadComplete = 0;
    m_PresetLogoInfo.nFragmentCount = 0;
    m_PresetLogoInfo.nOffset = 0;
}

void CAhaAbsFrameTxRx::resetRatingImageInfo()
{
    m_RatingImageInfo.imageDataByte.clear();
    m_RatingImageInfo.nIsDownloadComplete = 0;
    m_RatingImageInfo.nFragmentCount = 0;
    m_RatingImageInfo.nOffset = 0;
}

void CAhaAbsFrameTxRx::resetLikeImageInfo()
{
    m_LikeImageInfo.imageDataByte.clear();
    m_LikeImageInfo.nIsDownloadComplete = 0;
    m_LikeImageInfo.nFragmentCount = 0;
    m_LikeImageInfo.nOffset = 0;
}


//void CAhaAbsFrameTxRx::resetActiveContentInfo()
//{
//    m_ActiveContentInfo.nLiked	= 0;
//    m_ActiveContentInfo.nCanPause = 0;
//    m_ActiveContentInfo.nCanNext = 0;
//    m_ActiveContentInfo.nCanBack = 0;
//    m_ActiveContentInfo.nCanChange	= 0;
//    m_ActiveContentInfo.nCanTimeShift = 0;
//    m_ActiveContentInfo.nCanLike = 0;
//    m_ActiveContentInfo.nCanDislike = 0;
//    m_ActiveContentInfo.nCanUnvote = 0;
//    m_ActiveContentInfo.nDisliked = 0;

//    m_ActiveContentInfo.dbLon = 0;
//    m_ActiveContentInfo.dbLat = 0;
//    m_ActiveContentInfo.dbRating = 0;

//    m_ActiveContentInfo.strTitle.clear();
//    m_ActiveContentInfo.strImageURL.clear();
//    m_ActiveContentInfo.strZip.clear();
//    m_ActiveContentInfo.strAddress1.clear();
//    m_ActiveContentInfo.strState.clear();
//    m_ActiveContentInfo.strCity.clear();
//    m_ActiveContentInfo.strPhone.clear();
//    m_ActiveContentInfo.strFullAddress.clear();
//    m_ActiveContentInfo.strAddress2.clear();
//    m_ActiveContentInfo.strCountryCode.clear();
//    m_ActiveContentInfo.strLangCode.clear();
//    m_ActiveContentInfo.strRatingImageURL.clear();
//    m_ActiveContentInfo.strQVectContentInfoString.clear();
//}

//void CAhaAbsFrameTxRx::resetActiveStationInfo()
//{
//    m_ActiveStationInfo.nStationID = 0;
//    m_ActiveStationInfo.nLikeAllowed = 0;
//    m_ActiveStationInfo.nReadOnly = 0;
//    m_ActiveStationInfo.nIsTwitter = 0;
//    m_ActiveStationInfo.nIsLBS = 0;
//    m_ActiveStationInfo.nCanTimeShift = 0;
//    m_ActiveStationInfo.nIsDynamic = 0;
//    m_ActiveStationInfo.nIsFacebook = 0;
//    m_ActiveStationInfo.nCanPostText = 0;
//    m_ActiveStationInfo.nRecordTime = 0;
//    m_ActiveStationInfo.strStationIconURL.clear();
//    m_ActiveStationInfo.strStationFullName.clear();
//    m_ActiveStationInfo.strPartnerURL.clear();
//    m_ActiveStationInfo.strLikeSelectedURL.clear();
//    m_ActiveStationInfo.strDislikeSelectedURL.clear();
//    m_ActiveStationInfo.strLikeUnselectedURL.clear();
//    m_ActiveStationInfo.strDislikeUnselectedURL.clear();
//}


//void CAhaAbsFrameTxRx::resetAhaClientInfo()
//{
//    m_AhaClientInfo.nGPSAvailable = 0;
//    m_AhaClientInfo.strAppVersion.clear();
//    m_AhaClientInfo.strDevModel.clear();
//    m_AhaClientInfo.strDevSoftVersion.clear();

//    m_AhaClientInfo.nActiveStationID = 0;
//    m_AhaClientInfo.nActiveContentID = 0;
//    m_AhaClientInfo.nPlaybackState = 0;
//    m_AhaClientInfo.nElapsedSecs = 0;
//    m_AhaClientInfo.nQVectPresetStationID.clear();
//    m_AhaClientInfo.nQVectListContentID.clear();
//}

AhaClientInfo_T* CAhaAbsFrameTxRx::getAhaClientInfo()
{
    return &m_AhaClientInfo;
}

AhaStationInfo_T* CAhaAbsFrameTxRx::getAhaStationInfo()
{
    return &m_ActiveStationInfo;
}

AhaContentInfo_T* CAhaAbsFrameTxRx::getAhaContentInfo()
{
    return &m_ActiveContentInfo;
}

AhaImageInfo_T* CAhaAbsFrameTxRx::getAlbumArtInfo()
{
    return &m_AlbumArtInfo;
}

AhaImageInfo_T* CAhaAbsFrameTxRx::getStationLogoInfo()
{
    return &m_StationLogoInfo;
}
AhaImageInfo_T* CAhaAbsFrameTxRx::getPartnerLogoInfo()
{
    return &m_PartnerLogoInfo;
}
AhaImageInfo_T* CAhaAbsFrameTxRx::getPresetLogoInfo()
{
    return &m_PresetLogoInfo;
}
AhaImageInfo_T* CAhaAbsFrameTxRx::getRatingImageInfo()
{
    return &m_RatingImageInfo;
}

AhaImageInfo_T* CAhaAbsFrameTxRx::getLikeImageInfo()
{
    return &m_LikeImageInfo;
}
