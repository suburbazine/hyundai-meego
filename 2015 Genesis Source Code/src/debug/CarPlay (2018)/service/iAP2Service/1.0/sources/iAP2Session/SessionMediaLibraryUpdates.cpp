/*
    File:       SessionMediaLibraryUpdates.cpp
    Package:    LG iAP2 Service
    Author:     Yongil Park (yongil.park@lge.com)
    Copyright © [2014-2016] by LG Electronics Inc.

    This program or software including the accompanying associated documentation
    (“Software”) is the proprietary software of LG Electronics Inc. and or its
    licensors, and may only be used, duplicated, modified or distributed pursuant
    to the terms and conditions of a separate written license agreement between you
    and LG Electronics Inc. (“Authorized License”). Except as set forth in an
    Authorized License, LG Electronics Inc. grants no license (express or implied),
    rights to use, or waiver of any kind with respect to the Software, and LG
    Electronics Inc. expressly reserves all rights in and to the Software and all
    intellectual property therein. If you have no Authorized License, then you have
    no rights to use the Software in any ways, and should immediately notify LG
    Electronics Inc. and discontinue all use of the Software.
*/

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <fcntl.h>
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
//#include "iAp2ServiceCommon.h"
#include "SessionMediaLibraryUpdates.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
#include "SessionDefine.h"


namespace IAP2
{

SessionMeidaLibraryUpdates::SessionMeidaLibraryUpdates()
{
    LOGD("SessionMeidaLibraryUpdates()");
}

SessionMeidaLibraryUpdates::~SessionMeidaLibraryUpdates()
{
    LOGD("SessionMeidaLibraryUpdates()");
}

int SessionMeidaLibraryUpdates::handleMeidaLibraryUpdatesCmd(uint32_t cmd, uint32_t handle, uint8_t* pRxData, LinkController* pLnkCtrler)
{
    int result = kIap2NoErr;
    LOGD("handleMeidaLibraryUpdatesCmd() cmd[%u] handle[0x%x] pRxData[%p] pLnkCtrler[%p]", cmd, handle, pRxData, pLnkCtrler);

    if (pRxData == NULL) {
        LOGE("    ERROR : pRxData is NULL");
        return kIap2ParamErr;
    }

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR : pLnkCtrler is NULL");
        return kIap2ParamErr;
    }

    switch(cmd) {
        case kCmdStartMediaLibraryUpdates : {
            StartMediaLibraryUpdatesArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            LOGD("    rcv kCmdStartMediaLibraryUpdates");

            memset(&arg, 0x00, sizeof(StartMediaLibraryUpdatesArg));
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            
            arg.MediaLibraryUniqueIdentifier_Start.value =  iAp2MarshalGetBytes(&iAp2Marshal, &arg.MediaLibraryUniqueIdentifier_Start.size);

            arg.LastKnownMediaLibraryRevision.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if ( arg.LastKnownMediaLibraryRevision.bPresent == TRUE) {
                arg.LastKnownMediaLibraryRevision.value =  iAp2MarshalGetBytes(&iAp2Marshal, &arg.LastKnownMediaLibraryRevision.size);
            }

            arg.MediaItemProperties.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if ( arg.MediaItemProperties.bPresent == TRUE) {
                ParsingMediaLibraryUpdateCmdMediaItemGroup(&iAp2Marshal, &arg)        ;
            }
            
            arg.MediaPlaylistProperties.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if ( arg.MediaPlaylistProperties.bPresent == TRUE) {
                ParsingMediaLibraryUpdateCmdPlaylistGroup(&iAp2Marshal, &arg);
            }
                
                
            arg.MediaLibraryUpdateProgress = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.MediaLibraryIsHidingRemoteItems = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.PlayAllSongsCapable = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            sendStartMediaLibraryUpdates(pLnkCtrler, &arg);
            break;
        }

        case kCmdStopMediaLibraryUpdates : {
            StopMediaLibraryUpdatesArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            LOGD("    rcv kCmdStopMediaLibraryUpdates");
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            
            arg.MediaLibraryUniqueIdentifier_Stop.value =  iAp2MarshalGetBytes(&iAp2Marshal, &arg.MediaLibraryUniqueIdentifier_Stop.size);
            
            sendStopMediaLibraryUpdates(pLnkCtrler, &arg);
            break;
        }

        default :
            LOGE("    ERROR : unknown cmd[%d]", kIap2ParamErr);
            result = kIap2ParamErr;
            break;
    }
    
    return result;
}

void SessionMeidaLibraryUpdates::sendStartMediaLibraryUpdates(LinkController* pLnkCtrler, StartMediaLibraryUpdatesArg* pArg)
{
    LOGD("sendStartMediaLibraryUpdates()");
    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;
    
    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;
    
    createStartMediaLibraryUpdatesParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);    
    
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStartMediaLibraryUpdates,
                          nParamIdList, paramDataList, nParamDataLenList, &encodedData);
    
    
    
    ByteArray encodedDaraArr((char*)encodedData, nEncodedDataLength);
    
    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDaraArr);        
    
    DataEncoder::deleteParamList(nParamIdList,paramDataList, nParamDataLenList );
    delete [] encodedData;

}

void SessionMeidaLibraryUpdates::sendStopMediaLibraryUpdates(LinkController* pLnkCtrler, StopMediaLibraryUpdatesArg* pArg)
{
    LOGD("sendStopMediaLibraryUpdates()");
    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;
    
    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;
    
    createStopMediaLibraryUpdatesParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);    
    
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStopMediaLibraryUpdates,
                          nParamIdList, paramDataList, nParamDataLenList, &encodedData);
    
    ByteArray encodedDaraArr((char*)encodedData, nEncodedDataLength);
    
    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDaraArr);        
    
    DataEncoder::deleteParamList(nParamIdList,paramDataList, nParamDataLenList );
    delete [] encodedData;

}


void SessionMeidaLibraryUpdates::createStartMediaLibraryUpdatesParamList(std::vector<int>& nParamIdList,
                                                     std::vector<const uint8_t *>& paramDataList,
                                                     std::vector<uint32_t>& nParamDataLenList,
                                                     StartMediaLibraryUpdatesArg* pArg)
{
    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    uint8_t* groupParamBytes = NULL;
    uint32_t nNumOfBytes;

    DataEncoder::appendBytesParam(pArg->MediaLibraryUniqueIdentifier_Start.value,pArg->MediaLibraryUniqueIdentifier_Start.size, kParamIDMediaLibraryUniqueIdentifier_startupdate, nParamIdList, paramDataList, nParamDataLenList);

    if (pArg->LastKnownMediaLibraryRevision.bPresent == TRUE) {
        DataEncoder::appendBytesParam(pArg->LastKnownMediaLibraryRevision.value,pArg->LastKnownMediaLibraryRevision.size, kParamIDLastKnownMediaLibraryRevision, nParamIdList, paramDataList, nParamDataLenList);
    }

    nNumOfBytes = createGroupParamBytesForMediaItemProperties(&groupParamBytes, pArg);

    LOGD("    kParamIDMediaItemProperties groupParamBytes");
    LOGD("    groupParamBytes[%s] nNumOfBytes[%u]", (const char*)groupParamBytes, nNumOfBytes);
    if (pArg->MediaItemProperties.bPresent == TRUE) {
        DataEncoder::appendBytesParam(groupParamBytes, nNumOfBytes, kParamIDMediaItemProperties, nParamIdList, paramDataList, nParamDataLenList);
    }

    if (groupParamBytes != NULL) {
        delete [] groupParamBytes;
        groupParamBytes = NULL;
    }
    
    nNumOfBytes = createGroupParamBytesForMediaPlaylistProperties(&groupParamBytes, pArg);

    LOGD("    kParamIDMediaPlayListProperties groupParamBytes");
    LOGD("    groupParamBytes[%s] nNumOfBytes[%u]", (const char*)groupParamBytes, nNumOfBytes);

    if (pArg->MediaPlaylistProperties.bPresent == TRUE) {
        DataEncoder::appendBytesParam(groupParamBytes, nNumOfBytes, kParamIDMediaPlayListProperties, nParamIdList, paramDataList, nParamDataLenList);
    }
    
    if (pArg->MediaLibraryUpdateProgress == TRUE){
        DataEncoder::appendNoneParam(kParamIDMediaLibraryUpdateProgress_Start, nParamIdList, paramDataList, nParamDataLenList);
    }
    if (pArg->MediaLibraryIsHidingRemoteItems == TRUE) {
        DataEncoder::appendNoneParam(kParamIDMediaLibraryIsHidingRemoteItems_Start, nParamIdList, paramDataList, nParamDataLenList);
    }

    if (groupParamBytes != NULL) {
        delete [] groupParamBytes;
        groupParamBytes = NULL;
    }
    
}

uint32_t SessionMeidaLibraryUpdates::createGroupParamBytesForMediaItemProperties(uint8_t** encodedBytes, StartMediaLibraryUpdatesArg* pArg)
{
    unsigned int i, nTotalLength;
    std::vector<int> nSubParamIdList;
    std::vector<const uint8_t *> subParamDataList;
    std::vector<uint32_t> nSubParamDataLenList;
    uint8_t* encodedDataArr;

    createMediaItemPropertiesParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList, pArg);

    nTotalLength = 0;
    for(i = 0; i < nSubParamIdList.size(); i++) {
        nTotalLength = nTotalLength + 2 + 2 + nSubParamDataLenList.at(i);
    }

    encodedDataArr = new uint8_t[nTotalLength];
    DataEncoder::encodeParam(&encodedDataArr[0], nSubParamIdList, subParamDataList, nSubParamDataLenList);
    DataEncoder::deleteParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);
    *encodedBytes = encodedDataArr;
    return nTotalLength;
}

void SessionMeidaLibraryUpdates::createMediaItemPropertiesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList,
                                                              std::vector<uint32_t> &nParamDataLenList, StartMediaLibraryUpdatesArg* pArg)
{
#ifndef TEST_CODE
        LOGD("createMediaItemPropertiesParamList()");
    
        if (pArg == NULL) {
            LOGE("    ERROR :  pArg is NULL");
            return;
        }
#endif // #ifndef TEST_CODE

    if (pArg->MediaItemProperties.bPresent == TRUE) {
        if (pArg->MediaItemProperties.MediaItemPropertyPersistentIdentifier == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyPersistentIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    
        if (pArg->MediaItemProperties.MediaItemPropertyTitle == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyTitle, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyMediaType == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyMediaType, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyRating == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyRating, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyPlaybackDurationInMilliseconds == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyPlaybackDurationInMilliseconds, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyAlbumPersistentIdentifier == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyAlbumPersistentIdentifier, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyAlbumTitle == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyAlbumTitle, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyAlbumTrackNumber == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyAlbumTrackNumber, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyAlbumTrackCount == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyAlbumTrackCount, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyAlbumDiscNumber == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyAlbumDiscNumber, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyAlbumDiscCount == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyAlbumDiscCount, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyArtistPersistentIdentifier == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyArtistPersistentIdentifier, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyArtist == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyArtist, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyAlbumAritistPersistentIdentifier == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyAlbumAritistPersistentIdentifier, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyAlbumAritist == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyAlbumAritist, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyGenrePersistentIdentifier == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyGenrePersistentIdentifier, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyGenre == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyGenre, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyComposerPersistentIdentifier == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyComposerPersistentIdentifier, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyComposer == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyComposer, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyIsPartOfCompilation == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyIsPartOfCompilation, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaItemProperties.MediaItemPropertyIsResidentOnDevice == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyIsResidentOnDevice, nParamIdList, paramDataList, nParamDataLenList);
                
        if (pArg->MediaItemProperties.MediaItemPropertyChapterCount == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaItemPropertyChapterCount, nParamIdList, paramDataList, nParamDataLenList);
    }

}

uint32_t SessionMeidaLibraryUpdates::createGroupParamBytesForMediaPlaylistProperties(uint8_t** encodedBytes, StartMediaLibraryUpdatesArg* pArg)
{
    unsigned int i, nTotalLength;
    std::vector<int> nSubParamIdList;
    std::vector<const uint8_t *> subParamDataList;
    std::vector<uint32_t> nSubParamDataLenList;
    uint8_t* encodedDataArr;

    createMediaPlaylistPropertiesParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList, pArg);

    nTotalLength = 0;
    for(i = 0; i < nSubParamIdList.size(); i++) {
        nTotalLength = nTotalLength + 2 + 2 + nSubParamDataLenList.at(i);
    }

    encodedDataArr = new uint8_t[nTotalLength];
    DataEncoder::encodeParam(&encodedDataArr[0], nSubParamIdList, subParamDataList, nSubParamDataLenList);
    DataEncoder::deleteParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);
    *encodedBytes = encodedDataArr;
    return nTotalLength;
}

void SessionMeidaLibraryUpdates::createMediaPlaylistPropertiesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList,
                                                              std::vector<uint32_t> &nParamDataLenList, StartMediaLibraryUpdatesArg* pArg)
{
#ifndef TEST_CODE
        LOGD("createMediaPlaylistPropertiesParamList()");
    
        if (pArg == NULL) {
            LOGE("    ERROR :  pArg is NULL");
            return;
        }
#endif // #ifndef TEST_CODE

    if (pArg->MediaPlaylistProperties.bPresent == TRUE) {
        if (pArg->MediaPlaylistProperties.MediaPlaylistPropertyPersistentIdentifier == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaPlaylistPropertyPersistentIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    
        if (pArg->MediaPlaylistProperties.MediaPlaylistPropertyName == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaPlaylistPropertyName, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaPlaylistProperties.MediaPlaylistPropertyParentPersistentIdentifier == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaPlaylistPropertyParentPersistentIdentifier, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaPlaylistProperties.MediaPlaylistPropertyIsGeniusMix == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaPlaylistPropertyIsGeniusMix, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaPlaylistProperties.MediaPlaylistPropertyIsFolder == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaPlaylistPropertyIsFolder, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaPlaylistProperties.MediaPlaylistPropertyContainedMediaItemsFileTransferIdentifier == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaPlaylistPropertyContainedMediaItemsFileTransferIdentifier, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->MediaPlaylistProperties.MediaPlaylistPropertyIsiTunesRadioStation == TRUE)
            DataEncoder::appendNoneParam(kParamIDMediaPlaylistPropertyIsiTunesRadioStation, nParamIdList, paramDataList, nParamDataLenList);
    }

}

void SessionMeidaLibraryUpdates::createStopMediaLibraryUpdatesParamList(std::vector<int>& nParamIdList,
                                                     std::vector<const uint8_t *>& paramDataList,
                                                     std::vector<uint32_t>& nParamDataLenList,
                                                     StopMediaLibraryUpdatesArg* pArg)
{
    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    DataEncoder::appendBytesParam(pArg->MediaLibraryUniqueIdentifier_Stop.value,pArg->MediaLibraryUniqueIdentifier_Stop.size, kParamIDMediaLibraryUniqueIdentifier_stopupdate, nParamIdList, paramDataList, nParamDataLenList);
}




void SessionMeidaLibraryUpdates::ParsingMediaLibraryUpdateCmdMediaItemGroup(MarshalIo* iAp2Marshal, StartMediaLibraryUpdatesArg* arg)
{
    arg->MediaItemProperties.MediaItemPropertyPersistentIdentifier = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyTitle = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyMediaType = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyRating = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyPlaybackDurationInMilliseconds = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyAlbumPersistentIdentifier = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyAlbumTitle = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyAlbumTrackNumber = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyAlbumTrackCount = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyAlbumDiscNumber = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyAlbumDiscCount = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyArtistPersistentIdentifier = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyArtist= (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyAlbumAritistPersistentIdentifier = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyAlbumAritist= (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyGenrePersistentIdentifier = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyGenre = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyComposerPersistentIdentifier = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyComposer= (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyIsPartOfCompilation = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaItemProperties.MediaItemPropertyIsResidentOnDevice = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);        
    arg->MediaItemProperties.MediaItemPropertyChapterCount = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);        
}

void SessionMeidaLibraryUpdates::ParsingMediaLibraryUpdateCmdPlaylistGroup(MarshalIo* iAp2Marshal, StartMediaLibraryUpdatesArg* arg)
{
    arg->MediaPlaylistProperties.MediaPlaylistPropertyPersistentIdentifier = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaPlaylistProperties.MediaPlaylistPropertyName = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaPlaylistProperties.MediaPlaylistPropertyParentPersistentIdentifier = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaPlaylistProperties.MediaPlaylistPropertyIsGeniusMix = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaPlaylistProperties.MediaPlaylistPropertyIsFolder = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaPlaylistProperties.MediaPlaylistPropertyContainedMediaItemsFileTransferIdentifier = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->MediaPlaylistProperties.MediaPlaylistPropertyIsiTunesRadioStation = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
}



}// namespace IAP2
