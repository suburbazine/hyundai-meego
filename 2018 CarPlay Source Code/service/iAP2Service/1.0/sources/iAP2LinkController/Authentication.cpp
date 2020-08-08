/*
    File:       Authentication.cpp
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

#include <vector>

#include "Authentication.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include <stdlib.h>

namespace IAP2
{

Authentication::Authentication()
{
    LOGD("Authentication()");
}

Authentication::~Authentication()
{
    LOGD("~Authentication()");
}

void Authentication::sendAuthenticationCertificate(LinkController* pLnkCtrler, struct iAP2Link_st* link, uint8_t session)
{
    LOGD("sendAuthenticationCertificate()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }
    uint8_t retryAuthenticationCount = 5;
    uint32_t nEncodedDataLength;
    ByteArray getCertificate;
    uint8_t* encodedData = NULL;
    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    for( int curruntCount =0 ; curruntCount < retryAuthenticationCount; curruntCount++)
    {    
        
        getCertificate = mAuthCp.getCertificate();
        if(getCertificate.length() > 0)
            break;
        else
            LOGE("    ERROR : Fail to get getCertificate curruntCount :%d", curruntCount + 1);
        
    }        
    
    if(getCertificate.length() <= 0) { // Send Invalid Certificate to Apple device (236P at Accessro Interface Specification R19)
        LOGE("    ERROR : Send Invalid AuthenticationCertificate");
    
        
        uint8_t invalidData = 0;
        nParamIdList.push_back(0);
        paramDataList.push_back((const uint8_t*)&invalidData);
        nParamDataLenList.push_back(sizeof(invalidData));

        // jungyeon.kim It's message encoding
        nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdAuthenticationCertificate,
                              nParamIdList, paramDataList, nParamDataLenList, &encodedData);    

        ByteArray encodedDaraArr((char*)encodedData, nEncodedDataLength);

        // encodedDaraArr means Payload, such as control session message structure(p519) of iAP2 session(Accessory Authentication)
        pLnkCtrler->processOutBoundTraffic(link, session, encodedDaraArr);
        delete [] encodedData;
        
        return;
    }


    // jungyeon.Kim Parameter ID 0x0000 => AuthenticationCertificate p533
    nParamIdList.push_back(0);
    paramDataList.push_back((const uint8_t*)getCertificate.constData());
    nParamDataLenList.push_back(getCertificate.length());

    // jungyeon.kim It's message encoding
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdAuthenticationCertificate,
                          nParamIdList, paramDataList, nParamDataLenList, &encodedData);


    LOGD("    getCertificate length[%d] nEncodedDataLength[%d]", getCertificate.length(), nEncodedDataLength);

    ByteArray encodedDaraArr((char*)encodedData, nEncodedDataLength);

    // encodedDaraArr means Payload, such as control session message structure(p519) of iAP2 session(Accessory Authentication)
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDaraArr);

    delete [] encodedData;
}

void Authentication::sendAuthenticationChallengeResponse(LinkController* pLnkCtrler, struct iAP2Link_st* link, uint8_t session, ByteArray challenge)
{
    LOGD("sendAuthenticationChallengeResponse()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }
    
    uint8_t retryAuthenticationCount = 5;
    uint32_t nEncodedDataLength;
    ByteArray challengeResponse;
    uint8_t* encodedData = NULL;
    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    for( int curruntCount =0 ; curruntCount < retryAuthenticationCount; curruntCount++)
    {    
        
        challengeResponse = mAuthCp.getDigitalSign(challenge);
        if(challengeResponse.length() > 0)
            break;
        else
            LOGE("    ERROR : Fail to get getResponse curruntCount :%d", curruntCount + 1);
        
    }        
    
    if(challengeResponse.length() <= 0) { // Send Invalid Response to Apple device (236P at Accessro Interface Specification R19)
        LOGE("    ERROR : Send Invalid AuthenticationResponse");
    
        
        uint8_t invalidData = 0;
        nParamIdList.push_back(0);
        paramDataList.push_back((const uint8_t*)&invalidData);
        nParamDataLenList.push_back(sizeof(invalidData));

        // jungyeon.kim It's message encoding
        nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdAuthenticationResponse,
                              nParamIdList, paramDataList, nParamDataLenList, &encodedData);    

        ByteArray encodedDaraArr((char*)encodedData, nEncodedDataLength);

        // encodedDaraArr means Payload, such as control session message structure(p519) of iAP2 session(Accessory Authentication)
        pLnkCtrler->processOutBoundTraffic(link, session, encodedDaraArr);
        delete [] encodedData;
        
        return;
    }




    nParamIdList.push_back(0);
    paramDataList.push_back((const uint8_t*)challengeResponse.constData());
    nParamDataLenList.push_back(challengeResponse.length());

    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdAuthenticationResponse,
                          nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedData_arr((char*)encodedData, nEncodedDataLength);
    pLnkCtrler->processOutBoundTraffic(link, session, encodedData_arr);
    
    delete [] encodedData;
}

int Authentication::handleAuthenticationFailedUpdateEvent(LinkController* pLnkCtrler)
{
    int result = kIap2NoErr;

    //LOGD("handleAuthenticationFailedUpdateEvent()");

    uint32_t byteNum = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(sizeof(EventHeader) + sizeof(uint32_t)*1);
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, sizeof(EventHeader) + sizeof(uint32_t)*1, sizeof(EventHeader));

    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt = kEvtAuthenticationFailed;
    mEventHeader->dataSize = byteNum;    

    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(/*sessinID*/0, pIpcData, byteNum);
    }
    else {
        LOGD("    ERROR : mpSendEventToApp is NULL");
    }

    if (result != kIap2NoErr) {
        LOGW("    WARNING : send event fail. result[%d]", result);
    }

    free(pIpcData);
    return result;
}

} // namespace IAP2
