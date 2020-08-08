/*
    File:       iAp2Marshal.h
    Package:    LG iAP2 Application Framework
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

#ifndef IAP2MARSHALL_H_
#define IAP2MARSHALL_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

//===========================================================================================================================
//    Definitions
//===========================================================================================================================
typedef struct 
{
    char* data;                /* pointer to read/write from */
    uint32_t headerSize;    /* header size*/
    uint32_t dataAvail;        /* bytes available in data buffer */
    char* data0;            /* start of data buffer */
    uint32_t flags;
} MarshalIo;

//===========================================================================================================================
//    Declarations
//===========================================================================================================================

void iAp2MarshalInit(MarshalIo* mio, void* data, uint32_t maxdata, uint32_t headerSize);
void iAp2MarshalInitFromTxn(MarshalIo* mio, void* rx);
void* iAp2MarshalAlloc(MarshalIo* mio, uint32_t size);
void iAp2MarshalPutUint32(MarshalIo* mio, uint32_t n);
void iAp2MarshalPutUint64(MarshalIo* mio, uint64_t n);
void iAp2MarshalPutBytes(MarshalIo* mio, const uint8_t* bytes, uint32_t length);
uint32_t iAp2MarshalGetUint32(MarshalIo* mio);
uint64_t iAp2MarshalGetUint64(MarshalIo* mio);
uint8_t* iAp2MarshalGetBytes(MarshalIo* mio, uint32_t* size);
uint32_t iAp2MarshalGetUsedSize(MarshalIo* mio);


#ifdef __cplusplus
}
#endif

#endif /* IAP2MARSHALL_H_ */

