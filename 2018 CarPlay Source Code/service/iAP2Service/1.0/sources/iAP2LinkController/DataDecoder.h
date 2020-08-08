/*
    File:       DataDecoder.h
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

#ifndef DATADECODER_H
#define DATADECODER_H

#include <stdint.h>
#include <iAP2Link.h>
#include <IAP2PredefinedValues.h>
#include "ByteArray.h"

namespace IAP2
{
    class DataDecoder
    {
        public:
            DataDecoder();
            ~DataDecoder();
            static int getMsgId(uint8_t* data, int dataLen);
            static int getMsgLength(uint8_t* data, int dataLen);
            static ByteArray getParamData(uint8_t* data, int dataLen, int paramIdx);

            static bool isValidData(uint8_t* data, int dataLen);
            static int getUintFromBytes(uint8_t* data, int nStartIdx, int nLength);
            static uint64_t getUint64FromBytes(uint8_t* data, int nStartIdx, int nLength);

        private:            
    };

} // namespace IAP2

#endif // DATADECODER_H
