/*
    File:       DataDecoder.cpp
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

#include "DataDecoder.h"
#include "iAp2ServiceLog.h"

namespace IAP2
{

DataDecoder::DataDecoder()
{
}

DataDecoder::~DataDecoder()
{
}

int DataDecoder::getMsgId(uint8_t* data, int dataLen)
{
    int nMsgId = -1;
    if(isValidData(data, dataLen)) {
        nMsgId = getUintFromBytes(data, kIndexControlMsgMsgIdMsb, 2);
    }
    return nMsgId;
}

int DataDecoder::getMsgLength(uint8_t* data, int dataLen)
{
    int nMsgLength = -1;
    if(isValidData(data, dataLen))
    {
        nMsgLength = getUintFromBytes(data, kIndexControlMsgLenMsb, 2);
    }
    return nMsgLength;
}

/*
this function return ByteArray containing nParamIndex-th parameter data
*/
ByteArray DataDecoder::getParamData(uint8_t* data, int dataLen, int paramIdx)
{
    int nPreviousParamLength;
    if(paramIdx == 0) {
        nPreviousParamLength = 0;
    }
    else {
        LOGD("ERROR must calculate proper length");
        nPreviousParamLength = 0;
    }

    int nThisParamDataPosition = kIndexControlMsgMinimumLength + 2 + 2 + nPreviousParamLength;

    ByteArray array;
    array.clear();
    if(isValidData(data, dataLen)) {
        array.append((char*)&data[nThisParamDataPosition], dataLen - nThisParamDataPosition);
    }
    return array;
}


bool DataDecoder::isValidData(uint8_t* data, int dataLen)
{
    bool bValidData = false;
    //header should be valid, len should be valid
    if(dataLen < kIndexControlMsgMinimumLength) {
        LOGE("ERROR : Data is too short!");
    }
    else if(!(data[kIndexControlMsgStartMsb] == kControlSessionMsgStartMsb
              && data[kIndexControlMsgStartLsb] == kControlSessionMsgStartLsb)) {
        LOGE("ERROR : Header does not match!");
    }
    else {
        bValidData = true;
    }
    return bValidData;
}

int DataDecoder::getUintFromBytes(uint8_t* data, int nStartIdx, int nLength)
{
    int i;
    int intValue = 0;
    for(i = nStartIdx; i < nStartIdx + nLength; i++) {
        intValue = intValue << 8;
        intValue = intValue | data[i];
    }
    return intValue;
}

uint64_t DataDecoder::getUint64FromBytes(uint8_t* data, int nStartIdx, int nLength)
{
    int i;
    uint64_t uIntValue = 0;
    for(i = nStartIdx; i < nStartIdx + nLength; i++) {
        uIntValue = uIntValue << 8;
        uIntValue = uIntValue | data[i];
    }
    return uIntValue;
}


} // namespace IAP2
