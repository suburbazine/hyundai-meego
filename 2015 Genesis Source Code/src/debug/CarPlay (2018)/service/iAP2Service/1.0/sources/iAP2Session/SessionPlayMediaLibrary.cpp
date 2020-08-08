/*
    File:       SessionPlayMediaLibrary.cpp
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
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#include "SessionPlayMediaLibrary.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
#include "SessionDefine.h"

namespace IAP2
{

SessionPlayMediaLibrary::SessionPlayMediaLibrary()
{
    LOGD("SessionPlayMediaLibrary()");
}

SessionPlayMediaLibrary::~SessionPlayMediaLibrary()
{
    LOGD("SessionPlayMediaLibrary()");
}

int SessionPlayMediaLibrary::handlePlayMediaLibraryCmd(uint32_t cmd, uint32_t handle, uint8_t* pRxData, LinkController* pLnkCtrler)
{
    int result = kIap2NoErr;
    LOGD("handlePlayMediaLibraryCmd() cmd[%u] handle[0x%x] pRxData[%p] pLnkCtrler[%p]", cmd, handle, pRxData, pLnkCtrler);

    if (pRxData == NULL) {
        LOGE("    ERROR : pRxData is NULL");
        return kIap2ParamErr;
    }

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR : pLnkCtrler is NULL");
        return kIap2ParamErr;
    }

    switch(cmd) {
        case kCmdPlayMediaLibraryCurrentSelection : {
            PlayMediaLibraryCurrentSelectionArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            arg.MediaLibraryUniqueIdentifier_CurrentSelection.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.MediaLibraryUniqueIdentifier_CurrentSelection.size);
            sendPlayMediaLibraryCurrentSelection(pLnkCtrler, &arg);
            break;
        }

        case kCmdPlayMediaLibraryItems: {
            PlayMediaLibraryItemsArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            arg.ItemsPersistentIdentifier.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.ItemsPersistentIdentifier.size);
            arg.ItemStartingIndex.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);            
            if(arg.ItemStartingIndex.bPresent == TRUE)
            {
                arg.ItemStartingIndex.value = iAp2MarshalGetUint32(&iAp2Marshal);
               }            
            arg.MediaLibraryUniqueIdentifier_Item.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.MediaLibraryUniqueIdentifier_Item.size);            
            sendPlayMediaLibraryItems(pLnkCtrler, &arg);
            break;
        }
        case kCmdPlayMediaLibraryCollection : {
            PlayMediaLibraryCollectionArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            arg.CollectionPersistentIdentifier = iAp2MarshalGetUint64(&iAp2Marshal);
            arg.CollectionType = iAp2MarshalGetUint32(&iAp2Marshal);
            arg.CollectionStartingIndex.bPresent = iAp2MarshalGetUint32(&iAp2Marshal);
            if(arg.CollectionStartingIndex.bPresent == TRUE) 
            {
                arg.CollectionStartingIndex.value = iAp2MarshalGetUint32(&iAp2Marshal);
               }
            arg.MediaLibraryUniqueIdentifier_Collection.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.MediaLibraryUniqueIdentifier_Collection.size);     
            LOGD("arg.CollectionPersistentIdentifier : %llu", arg.CollectionPersistentIdentifier);
            sendPlayMediaLibraryCollection(pLnkCtrler, &arg);
            break;
        }
        case kCmdPlayMediaLibrarySpecial : {
            PlayMediaLibrarySpecialArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            arg.MediaLibraryUniqueIdentifier_Special.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.MediaLibraryUniqueIdentifier_Special.size); 
            arg.AllSongs = iAp2MarshalGetUint32(&iAp2Marshal);
            
            LOGD("arg.MediaLibraryUniqueIdentifier_Special value : %s", arg.MediaLibraryUniqueIdentifier_Special.value);
            LOGD("arg.AllSongs: %u", arg.AllSongs);
            
            sendPlayMediaLibrarySpecial(pLnkCtrler, &arg);
            break;
        }
        default :
            LOGE("    ERROR : unknown cmd[%d]", kIap2ParamErr);
            result = kIap2ParamErr;
            break;
    }
    
    return result;
}

void SessionPlayMediaLibrary::sendPlayMediaLibraryCurrentSelection(LinkController * pLnkCtrler, PlayMediaLibraryCurrentSelectionArg* pArg)
{
    LOGD("sendPlayMediaLibraryCurrentSelection()");

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

    createPlayMediaLibraryCurrentSelectionParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdPlayMediaLibraryCurrentSelection, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }        
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}

void SessionPlayMediaLibrary::sendPlayMediaLibraryItems(LinkController * pLnkCtrler, PlayMediaLibraryItemsArg* pArg)
{
    LOGD("sendPlayMediaLibraryItems()");

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

    createPlayMediaLibraryItemsParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdPlayMediaLibraryItems, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}


void SessionPlayMediaLibrary::sendPlayMediaLibraryCollection(LinkController * pLnkCtrler, PlayMediaLibraryCollectionArg* pArg)
{
    LOGD("sendPlayMediaLibraryCollection()");

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

    createPlayMediaLibraryCollectionParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdPlayMediaLibraryCollection, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}


void SessionPlayMediaLibrary::sendPlayMediaLibrarySpecial(LinkController * pLnkCtrler, PlayMediaLibrarySpecialArg* pArg)
{
    LOGD("sendPlayMediaLibrarySpecial()");

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

    createPlayMediaLibrarySpecialParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdPlayMediaLibrarySpecial, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}

void SessionPlayMediaLibrary::createPlayMediaLibraryCurrentSelectionParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, PlayMediaLibraryCurrentSelectionArg* pArg)
{
    LOGD("createPlayMediaLibraryCurrentSelectionParamList()");

    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    LOGD("pArg->MediaLibraryUniqueIdentifier_CurrentSelection.value : %s", pArg->MediaLibraryUniqueIdentifier_CurrentSelection.value);
    LOGD("pArg->MediaLibraryUniqueIdentifier_CurrentSelection.size : %d", pArg->MediaLibraryUniqueIdentifier_CurrentSelection.size);
    DataEncoder::appendBytesParam(pArg->MediaLibraryUniqueIdentifier_CurrentSelection.value,pArg->MediaLibraryUniqueIdentifier_CurrentSelection.size, kParamMediaLibraryUniqueIdentifier_CurrentSelection, nParamIdList, paramDataList, nParamDataLenList);

}
void SessionPlayMediaLibrary::createPlayMediaLibraryItemsParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, PlayMediaLibraryItemsArg* pArg)
{
    LOGD("createPlayMediaLibraryItemsParamList()");

    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    LOGD("pArg->ItemsPersistentIdentifier.value : %s", pArg->ItemsPersistentIdentifier.value);
    LOGD("pArg->ItemsPersistentIdentifier.size : %d", pArg->ItemsPersistentIdentifier.size);
    LOGD("pArg->ItemStartingIndex.bPresent : %d",pArg->ItemStartingIndex.bPresent);
    LOGD("pArg->ItemStartingIndex.value : %d", pArg->ItemStartingIndex.value);
    LOGD("pArg->MediaLibraryUniqueIdentifier_Item.value : %s", pArg->MediaLibraryUniqueIdentifier_Item.value);
    LOGD("pArg->MediaLibraryUniqueIdentifier_Item.size : %d", pArg->MediaLibraryUniqueIdentifier_Item.size);
    DataEncoder::appendBytesParam(pArg->ItemsPersistentIdentifier.value,pArg->ItemsPersistentIdentifier.size, kParamItemsPersistentIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    if (pArg->ItemStartingIndex.bPresent == TRUE)   {
        DataEncoder::appendNumericParam(pArg->ItemStartingIndex.value,4, kParamItemsStartingIndex, nParamIdList, paramDataList, nParamDataLenList);
    }
    DataEncoder::appendBytesParam(pArg->MediaLibraryUniqueIdentifier_Item.value,pArg->MediaLibraryUniqueIdentifier_Item.size, kParamMediaLibraryUniqueIdentifier_Items, nParamIdList, paramDataList, nParamDataLenList);
}

void SessionPlayMediaLibrary::createPlayMediaLibraryCollectionParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, PlayMediaLibraryCollectionArg* pArg)
{
    LOGD("createPlayMediaLibraryCollectionParamList()");

    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();
     LOGD("pArg->CollectionPersistentIdentifier : %llu", pArg->CollectionPersistentIdentifier);
    DataEncoder::append64bitNumericParam(pArg->CollectionPersistentIdentifier, 8, kParamCollectionPersistentIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNumericParam(pArg->CollectionType, 1, kParamCollectionType, nParamIdList, paramDataList, nParamDataLenList);
    if (pArg->CollectionStartingIndex.bPresent == TRUE)   {
        DataEncoder::appendNumericParam(pArg->CollectionStartingIndex.value,4, kParamCollectionStartingIndex, nParamIdList, paramDataList, nParamDataLenList);
    }
    DataEncoder::appendBytesParam(pArg->MediaLibraryUniqueIdentifier_Collection.value,pArg->MediaLibraryUniqueIdentifier_Collection.size,  kParamMediaLibraryUniqueIdentifier_Collection, nParamIdList, paramDataList, nParamDataLenList);
}

void SessionPlayMediaLibrary::createPlayMediaLibrarySpecialParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, PlayMediaLibrarySpecialArg* pArg)
{
    LOGD("createPlayMediaLibrarySpecialParamList()");

    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();
    DataEncoder::appendBytesParam(pArg->MediaLibraryUniqueIdentifier_Special.value,pArg->MediaLibraryUniqueIdentifier_Special.size,  kParamMediaLibraryUniqueIdentifier_Special, nParamIdList, paramDataList, nParamDataLenList);
    if(pArg->AllSongs == TRUE)    {
        DataEncoder::appendNoneParam(kParamSpecialAllSongs, nParamIdList, paramDataList, nParamDataLenList);
    }
}

}// namespace IAP2
