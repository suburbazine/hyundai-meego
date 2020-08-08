/*
    File:       DataEncoder.h
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

#ifndef DATAENCODER_H
#define DATAENCODER_H

//#include <QObject>
#include <iAP2Packet.h>
#include <IAP2PredefinedValues.h>
#include <vector>
#include <iAp2AppFrameworkExternal.h>
#include <iAp2AppFrameworkExternal_command.h>
#include <iAp2AppFrameworkExternal_event.h>


namespace IAP2
{

class DataEncoder
{

    public:
        DataEncoder();

        static uint32_t encodeMsg(int sessionType, int nMsgId,
                                   std::vector<int> nParamIdList,
                                   std::vector<const uint8_t*> paramDataList,
                                   std::vector<uint32_t> nParamDataLenList,
                                   uint8_t** ppEncodedDataOutput);

        static void encodeParam(uint8_t* encodedData, std::vector<int> nParamIdList, std::vector<const uint8_t*> paramDataList,
                                std::vector<uint32_t> nParamDataLenList);

        static uint32_t encodeControlTypeMsg(int nMsgId,
                                                 std::vector<int> nParamIdList,
                                                 std::vector<const uint8_t*> paramDataList,
                                                 std::vector<uint32_t> nParamDataLenList,
                                                 uint8_t** ppEncodedDataOutput);
        static void appendStringParam(const char* strParam, int nParamID,
                                          std::vector<int> &nParamIdList,
                                          std::vector<const uint8_t *> &paramDataList,
                                          std::vector<uint32_t> &nParamDataLenList);
        static void appendNumericParam(int nValue, int nNumOfBytes, int nParamID,
                                           std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList,
                                           std::vector<uint32_t> &nParamDataLenList );

        static void append64bitNumericParam(uint64_t nValue, int nNumOfBytes, int nParamID, 
                                            std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList, 
                                            std::vector<uint32_t> &nParamDataLenList );

        static void appendNoneParam(int nParamID,
                                 std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList,
                                 std::vector<uint32_t> &nParamDataLenList );

        static void appendBytesParam(uint8_t* bytes, int nNumOfBytes, int nParamID,
                                         std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList,
                                         std::vector<uint32_t> &nParamDataLenList );

        static int setBytes(uint8_t* data, int nStartPosition, int nByteLength, int nValue);


        static uint8_t getByteFromIntegerValue(int nInteger, int nBytePosition, int nNumberOfBytes);

        static uint8_t getByteFromUint64Value(uint64_t nUint64Value, int nBytePosition);

        static void createAccessoryHIDReportParamList(int nValue, std::vector<int>& nParamIdList,
                                                                 std::vector<const uint8_t *>& paramDataList,
                                                                 std::vector<uint32_t>& nParamDataLenList);

        static void createStartHIDParamList(std::vector<int>& nParamIdList,
                                                                 std::vector<const uint8_t *>& paramDataList,
                                                                 std::vector<uint32_t>& nParamDataLenList);

        static void createStopHIDParamList(std::vector<int>& nParamIdList,
                                                                 std::vector<const uint8_t *>& paramDataList,
                                                                 std::vector<uint32_t>& nParamDataLenList);
        static void deleteParamList(std::vector<int>& nParamIdList,
                                        std::vector<const uint8_t *>& paramDataList,
                                        std::vector<uint32_t>& nParamDataLenList);
};

} // namespace IAP2

#endif // DATAENCODER_H
