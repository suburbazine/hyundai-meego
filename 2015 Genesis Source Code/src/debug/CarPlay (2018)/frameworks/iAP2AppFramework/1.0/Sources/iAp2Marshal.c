/*
    File:       iAp2Marshal.c
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

#include "iAp2Marshal.h"
#include "iAp2AppFrameworkExternal.h"
#include "iAp2AppFrameworkExternal_command.h"
#include "iAp2AppFrameworkExternal_event.h"
#include "iAp2Log.h"

//===========================================================================================================================
//    Definitions
//===========================================================================================================================
#define MIO_SHARED    0x01  /* needs to be buffer freed */
#define MIO_OVERFLOW    0x02  /* ran out of space */

//===========================================================================================================================
//    Declarations
//===========================================================================================================================


//===========================================================================================================================
//    iAp2MarshalInit
//===========================================================================================================================
void iAp2MarshalInit(MarshalIo* mio, void* data, uint32_t maxdata, uint32_t headerSize)
{
    if (headerSize > maxdata) {
        mio->flags = MIO_OVERFLOW;
        mio->dataAvail = 0;
        mio->headerSize = 0;
        LOGE("iAp2MarshalInit() buffer over flow. headerSize[%u] maxdata[%u]", headerSize, maxdata);
        return;
    }
    
    mio->data = mio->data0 = (char *) data + headerSize;
    mio->headerSize = headerSize;
    mio->dataAvail = maxdata - headerSize;
    mio->flags = 0;

}

//===========================================================================================================================
//    iAp2MarshalInitFromTxn
//===========================================================================================================================
void iAp2MarshalInitFromTxn(MarshalIo* mio, void* rx)
{
    CommandHeader* pRx = rx;
    mio->data = mio->data0 = (char*)pRx + sizeof(CommandHeader);
    mio->dataAvail = pRx->dataSize;
    mio->flags = MIO_SHARED;
}

//===========================================================================================================================
//    iAp2MarshalAlloc
//===========================================================================================================================
void* iAp2MarshalAlloc(MarshalIo* mio, uint32_t size)
{
    size = (size + 3) & (~3);
    if (size > mio->dataAvail) {
        mio->flags |= MIO_OVERFLOW;
        LOGE("ERROR : iAp2MarshalAlloc() overflow size[%u] dataAvail[%u]", size, mio->dataAvail);
        return 0;
    } else {
        void *ptr = mio->data;
        mio->data += size;
        mio->dataAvail -= size;
        return ptr;
    }
}

//===========================================================================================================================
//    iAp2MarshalPutUint32
//===========================================================================================================================
void iAp2MarshalPutUint32(MarshalIo *mio, uint32_t n)
{
    uint32_t* ptr = (uint32_t*)iAp2MarshalAlloc(mio, sizeof(n));
    if (ptr)
        *ptr = n;
}

//===========================================================================================================================
//    iAp2MarshalPutUint64
//===========================================================================================================================
void iAp2MarshalPutUint64(MarshalIo *mio, uint64_t n)
{
    uint64_t* ptr = (uint64_t*) iAp2MarshalAlloc(mio, sizeof(n));
    if (ptr)
        *ptr = n;
}

//===========================================================================================================================
//    iAp2MarshalPutBytes
//===========================================================================================================================
void iAp2MarshalPutBytes(MarshalIo* mio, const uint8_t* bytes, uint32_t length)
{
#if 1
    uint8_t* ptr;
    //uint32_t size = (length + 3) & (~3);

    if (bytes == NULL) {
        LOGE("    ERROR : iAp2MarshalPutBytes() bytes is NULL");
        return;
    }

    iAp2MarshalPutUint32(mio, length);
    ptr = (uint8_t*) iAp2MarshalAlloc(mio, length);
    if (ptr) {
        // initialize last 4bytes;
        //memset(&ptr[size-4], 0x00, 4);
        memset(ptr, 0x00, length);
        memcpy(ptr, bytes, length);
    }
#else
    uint8_t* ptr;

    if (bytes == NULL) {
        LOGE("    ERROR : iAp2MarshalPutBytes() bytes is NULL");
        return;
    }

    iAp2MarshalPutUint32(mio, length);
    ptr = (uint8_t*) iAp2MarshalAlloc(mio, length);
    if (ptr)
        memcpy(ptr, bytes, length);
#endif

}


//===========================================================================================================================
//    iAp2MarshalGet
//===========================================================================================================================
static void* iAp2MarshalGet(MarshalIo* mio, uint32_t size)
{
    size = (size + 3) & (~3);

    if (mio->dataAvail < size){
        mio->dataAvail = 0;
        mio->flags |= MIO_OVERFLOW;
        LOGE("ERROR : iAp2MarshalGet() Buffer overflow. dataAvail. mio[%u] size[%u]", mio->dataAvail, size);
        return 0;
    }  else {
        void *ptr = mio->data;
        mio->data += size;
        mio->dataAvail -= size;
        return ptr;
    }
}

//===========================================================================================================================
//    iAp2MarshalGetUint32
//===========================================================================================================================
uint32_t iAp2MarshalGetUint32(MarshalIo* mio)
{
    //uint32_t *ptr = iAp2MarshalGet(mio, sizeof(*ptr));
    uint32_t* ptr = (uint32_t*)iAp2MarshalGet(mio, sizeof(uint32_t));
    return ptr ? *ptr : 0;
}

//===========================================================================================================================
//    iAp2MarshalGetUint64
//===========================================================================================================================
uint64_t iAp2MarshalGetUint64(MarshalIo* mio)
{
    uint64_t* ptr =(uint64_t*) iAp2MarshalGet(mio, sizeof(uint64_t));
    return ptr ? *ptr : 0;
}

//===========================================================================================================================
//    iAp2MarshalGetBytes
//===========================================================================================================================
uint8_t* iAp2MarshalGetBytes(MarshalIo* mio, uint32_t* size)
{
    if (mio == NULL) {
        LOGE("ERROR : iAp2MarshalGetByte() mio is NULL");
        return NULL;
    }

    if (size == NULL) {
        LOGE("ERROR : iAp2MarshalGetByte() size is NULL");
        return NULL;
    }
    
    *size = iAp2MarshalGetUint32(mio);
    return iAp2MarshalGet(mio, *size);
}

//===========================================================================================================================
//    iAp2MarshalGetUint32
//===========================================================================================================================
uint32_t iAp2MarshalGetUsedSize(MarshalIo* mio)
{
    uint32_t usedSize =0;

    if (mio == NULL) {
        LOGE("ERROR : iAp2MarshalGetUsedSize() mio is NULL");
        return 0;
    }

    usedSize = mio->data - mio->data0 + mio->headerSize;
    return usedSize;
}

