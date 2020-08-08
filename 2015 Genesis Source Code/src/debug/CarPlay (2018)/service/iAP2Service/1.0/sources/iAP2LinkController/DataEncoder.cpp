/*
    File:       DataEncoder.cpp
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

#include "DataEncoder.h"
#include "iAp2ServiceLog.h"

namespace IAP2
{

DataEncoder::DataEncoder()
{

}

void DataEncoder::createStopHIDParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList)
{
    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    appendNumericParam(0x0000, 2, kParamIdHidHIDComponentIdentifier,
                    nParamIdList, paramDataList, nParamDataLenList);

}

void DataEncoder::createStartHIDParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList)
{
    // jungyeon.kim p416
    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    appendNumericParam(0x0000, 2, kParamIdHidHIDComponentIdentifier,
                    nParamIdList, paramDataList, nParamDataLenList);
    appendNumericParam(0x05AC, 2, kParamIdHidVendorIdentifier,
                    nParamIdList, paramDataList, nParamDataLenList); //LGE Vendor id
    appendNumericParam(0x0001, 2,kParamIdHidProductIdentifier,
                    nParamIdList, paramDataList, nParamDataLenList);
    uint8_t* bytesData = new uint8_t[256];
    int nStartPosition = 0;
    nStartPosition = setBytes(bytesData, nStartPosition, 2, 0x050C);    // USAGE_PAGE (Consumer Devices) jungyeon.kim
    nStartPosition = setBytes(bytesData, nStartPosition, 2, 0x0901);    // USAGE (Consumer Control)
    nStartPosition = setBytes(bytesData, nStartPosition, 2, 0xA101);    // COLLECTION (Application)
    nStartPosition = setBytes(bytesData, nStartPosition, 2, 0x1500);    //       LOGICAL_MINIMUM (0)
    nStartPosition = setBytes(bytesData, nStartPosition, 2, 0x2501);    //       LOGICAL_MAXIMUM (1)
    nStartPosition = setBytes(bytesData, nStartPosition, 2, 0x7501);    //       REPORT_SIZE (1)
    nStartPosition = setBytes(bytesData, nStartPosition, 2, 0x9505);    //       REPORT_COUNT (5)
    nStartPosition = setBytes(bytesData, nStartPosition, 2, 0x09CD);    //        USAGE (Play/Pause)
    nStartPosition = setBytes(bytesData, nStartPosition, 2, 0x09B5);    //       USAGE (SCAN Next Track)
    nStartPosition = setBytes(bytesData, nStartPosition, 2, 0x09B6);    //       USAGE (SCAN Previous Track)
    nStartPosition = setBytes(bytesData, nStartPosition, 2, 0x09E9);    //       USAGE (Volume Increment)
    nStartPosition = setBytes(bytesData, nStartPosition, 2, 0x09EA);    //       USAGE (Volume Decrement)
    nStartPosition = setBytes(bytesData, nStartPosition, 2, 0x8102);    //       INPUT (Data, Var, Abs)
    nStartPosition = setBytes(bytesData, nStartPosition, 2, 0x7503);    //       REPORT_SIZE (3)
    nStartPosition = setBytes(bytesData, nStartPosition, 2, 0x9501);    //       REPORT_COUNT (1)
    nStartPosition = setBytes(bytesData, nStartPosition, 2, 0x8103);    //       INPUT (Cnst, Var, Abs)
    nStartPosition = setBytes(bytesData, nStartPosition, 1, 0xC0);        // END COLLECTION

    appendBytesParam(bytesData, nStartPosition, kParamIdHidHIDReportDescriptor,
                    nParamIdList, paramDataList, nParamDataLenList);
    delete [] bytesData;
}


void DataEncoder::createAccessoryHIDReportParamList(int nValue, std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList)
{

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    appendNumericParam(0x0000, 2, kParamIdHidHIDComponentIdentifier,
                    nParamIdList, paramDataList, nParamDataLenList);
    appendNumericParam(nValue, 1, kParamIdHidHIDReport,
                    nParamIdList, paramDataList, nParamDataLenList);
}

//Example: nByteLength = 2, nStartPosition 5, value 7, then.
//data[5] = 0x00 data [6] = 0x07
int DataEncoder::setBytes(uint8_t* data, int nStartPosition, int nByteLength, int nValue)
{
    int i, j;
    int nNextPosition;
    for(i = nStartPosition, j = nByteLength; i < nStartPosition + nByteLength; i++, j--) {
        data[i] = getByteFromIntegerValue(nValue, j-1 , 4);
    }
    nNextPosition = nStartPosition + nByteLength;
    return nNextPosition;
}

void DataEncoder::deleteParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t *>& paramDataList, std::vector<uint32_t>& nParamDataLenList)
{
    //the only parameter I have to take extra care is const uint8_t*
    unsigned int i;
    for(i = 0; i < nParamIdList.size();i++) {
        //I will delete what the paramDataList point to first!
        delete [] paramDataList.at(i);
    }
    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();
}

void DataEncoder::appendStringParam(const char* strParam, int nParamID, std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList, std::vector<uint32_t> &nParamDataLenList)
{
    //need to call delete later!
    uint8_t* mydata;
    int len = strlen(strParam);
    if ( len < 0 ) {
        len = 0;
    }
    mydata = new uint8_t[len+1];
    memset(mydata, 0, len+1);
    memcpy(mydata, strParam, len);

    nParamIdList.push_back(nParamID);
    paramDataList.push_back((const uint8_t*)mydata);
    nParamDataLenList.push_back(len+1);
}

void DataEncoder::appendNumericParam(int nValue, int nNumOfBytes, int nParamID, std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList, std::vector<uint32_t> &nParamDataLenList )
{
    int i=0;
    uint8_t* mydata = new uint8_t[nNumOfBytes];

    for(i=0; i<nNumOfBytes; i++) {
        mydata[i] = getByteFromIntegerValue(nValue, nNumOfBytes-1-i, 4);
    }
    nParamIdList.push_back(nParamID);
    paramDataList.push_back((const uint8_t*)mydata);
    nParamDataLenList.push_back(nNumOfBytes);

}

void DataEncoder::append64bitNumericParam(uint64_t nValue, int nNumOfBytes, int nParamID, std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList, std::vector<uint32_t> &nParamDataLenList )
{
    int i=0;
    uint8_t* mydata = new uint8_t[nNumOfBytes];

    for(i=0; i<nNumOfBytes; i++) {
        mydata[i] = getByteFromUint64Value(nValue, (nNumOfBytes-1)-i);        
    }
    nParamIdList.push_back(nParamID);
    paramDataList.push_back((const uint8_t*)mydata);
    nParamDataLenList.push_back(nNumOfBytes);

}

void DataEncoder::appendNoneParam(int nParamID, std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList, std::vector<uint32_t> &nParamDataLenList )
{
    //AppendNone should append no data. however, this is std::vector, if I do not append,
    //the paramData will mismatch.
    //thus, append dummy value. the value will not be used anyway because we check datalen.
    uint8_t* mydata = new uint8_t[1];
    mydata[0] = 0xff; //any dummy
    nParamIdList.push_back(nParamID);
    paramDataList.push_back((const uint8_t*)mydata);
    nParamDataLenList.push_back(0);
}



void DataEncoder::appendBytesParam(uint8_t* bytes, int nNumOfBytes, int nParamID, std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList, std::vector<uint32_t> &nParamDataLenList )
{
    uint8_t* mydata = new uint8_t[nNumOfBytes];
    memcpy(mydata, bytes,nNumOfBytes);

    nParamIdList.push_back(nParamID);
    paramDataList.push_back((const uint8_t*)mydata);
    nParamDataLenList.push_back(nNumOfBytes);
}



uint32_t DataEncoder::encodeMsg(int sessionType, int nMsgId, std::vector<int> nParamIdList, std::vector<const uint8_t*> paramDataList, std::vector<uint32_t> nParamDataLenList, uint8_t** ppEncodedDataOutput)
{
    uint32_t nEncodedDataLength = 0;
    switch(sessionType)
    {
        case kIAP2PacketServiceTypeControl:
            nEncodedDataLength = encodeControlTypeMsg(nMsgId, nParamIdList, paramDataList, nParamDataLenList, ppEncodedDataOutput);
            break;
            
        default:
            LOGD("encoding control type default");
            break;
    }
    return nEncodedDataLength;

}

uint32_t DataEncoder::encodeControlTypeMsg(int nMsgId, std::vector<int> nParamIdList, std::vector<const uint8_t*> paramDataList, std::vector<uint32_t> nParamDataLenList, uint8_t** ppEncodedDataOutput)
{
    uint32_t nTotalLength;
    unsigned int i;

    uint8_t* encodedDataArr;

    // junyeon.kim nTotalLength => Msg Total Length
    nTotalLength = kIndexControlMsgMinimumLength;
    for(i = 0; i < nParamIdList.size(); i++) {
        nTotalLength = nTotalLength + 2 + 2 + nParamDataLenList.at(i);
    }

    encodedDataArr = new uint8_t[nTotalLength];

    encodedDataArr[kIndexControlMsgStartMsb] = kControlSessionMsgStartMsb;
    encodedDataArr[kIndexControlMsgStartLsb] = kControlSessionMsgStartLsb;
    encodedDataArr[kIndexControlMsgLenMsb] = getByteFromIntegerValue((int)nTotalLength, 1, 4);
    encodedDataArr[kIndexControlMsgLenLsb] = getByteFromIntegerValue((int)nTotalLength, 0, 4);
    encodedDataArr[kIndexControlMsgMsgIdMsb] = getByteFromIntegerValue(nMsgId,1,4);
    encodedDataArr[kIndexControlMsgMsgIdLsb] = getByteFromIntegerValue(nMsgId,0,4);

    encodeParam(&encodedDataArr[kIndexControlMsgMinimumLength], nParamIdList, paramDataList, nParamDataLenList);
    *ppEncodedDataOutput = encodedDataArr;
    return nTotalLength;
}

void DataEncoder::encodeParam(uint8_t* encodedData, std::vector<int> nParamIdList, std::vector<const uint8_t*> paramDataList, std::vector<uint32_t> nParamDataLenList)
{
    unsigned int i;
    uint32_t nParamOffset = 0;
    for(i = 0; i< nParamIdList.size(); i++) {
        encodedData[nParamOffset + kIndexControlMsgParamLenMsb] = getByteFromIntegerValue((int)(2+2+nParamDataLenList.at(i)), 1, 4);
        encodedData[nParamOffset + kIndexControlMsgParamLenLsb] = getByteFromIntegerValue((int)(2+2+nParamDataLenList.at(i)), 0, 4);
        encodedData[nParamOffset + kIndexControlMsgParamIdMsb] = getByteFromIntegerValue(nParamIdList.at(i), 1, 4);
        encodedData[nParamOffset + kIndexControlMsgParamIdLsb] = getByteFromIntegerValue(nParamIdList.at(i), 0, 4);
        if(nParamDataLenList.at(i) > 0) {
            memcpy(&encodedData[nParamOffset + kIndexControlMsgParamData], paramDataList.at(i),    nParamDataLenList.at(i));
        }
        nParamOffset = nParamOffset + 2 + 2 + nParamDataLenList.at(i);
    }
}

uint8_t DataEncoder::getByteFromIntegerValue(int nInteger, int nBytePosition, int nNumberOfBytes)
{
    uint8_t nByte;
    (void)nNumberOfBytes;

    nByte = (nInteger >> (nBytePosition*8))& 0xFF;
    return nByte;
}

uint8_t DataEncoder::getByteFromUint64Value(uint64_t nUint64Value, int nBytePosition)
{
    uint8_t nByte;

    nByte = (nUint64Value >> (nBytePosition*8))& 0xFF;
    return nByte;
}


} // namespace IAP2

