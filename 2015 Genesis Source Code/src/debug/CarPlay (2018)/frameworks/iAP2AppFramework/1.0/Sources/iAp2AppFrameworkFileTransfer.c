/*
 *     iAp2AppFrameworkFileTrnasfer.c
 *
 *    Created on: 07/22, 2015
 *    Author: yongil.park
 */

#include <stdlib.h>

#include "iAp2AppFrameworkCommon.h"
#include "iAp2AppFrameworkFileTransfer.h"
#include "iAp2AppFrameworkPrivate.h"
#include "iAp2Connection.h"
#include "iAp2Log.h"


//===========================================================================================================================
//    Definitions
//===========================================================================================================================
/*
//===========================================================================================================================
//    Declarations
//===========================================================================================================================
int iAp2AppFileTransferRequestCMD(struct iAp2AppPrivate inRef, FileTransferCMD mFileTransferCMD, uint8_t bufferID)
{
      int retValue = kIap2NoErr;

    LOGD("iAp2AppFileTransferRequest FileTransferCMD : %u , bufferID : %u", mFileTransferCMD, bufferID);

    uint32_t byteNum = 0;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*(100);
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    
    CommandHeader* pCommandHeader = (CommandHeader* )pIpcData;
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, mFileTransferCMD);
    iAp2MarshalPutUint32(&iAp2Marshal, bufferID);

    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pCommandHeader->cmd = kCmdFileTransferRequest;
    pCommandHeader->appHandle = inRef;
    pCommandHeader->dataSize = byteNum;    

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;

}
*/
//===========================================================================================================================
//    iAp2ConectionHandleEventFileTransferUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventFileTransferUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{    
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventFileTransferUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->FileTransferUpdate_f == NULL) {
        LOGW("    WARING : delegate->FileTransferUpdate_f is NULL");
        return kIap2NoErr;
    }

    FileTransferUpdateArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    
    memset(&cbArg, 0x00, sizeof(FileTransferUpdateArg));
    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);    
    cbArg.state = (uint64_t)iAp2MarshalGetUint64(&iAp2Marshal);
    cbArg.totSize = (uint64_t)iAp2MarshalGetUint64(&iAp2Marshal);
    cbArg.bPresent = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);
    if( cbArg.bPresent == TRUE )
        cbArg.value = iAp2MarshalGetBytes(&iAp2Marshal, (uint32_t*)&cbArg.buffSize);
    cbArg.bufferID = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.mFileTransferMode = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);
    
    if ((handle->delegate != NULL) && (handle->delegate->FileTransferUpdate_f != NULL)) {
        result = handle->delegate->FileTransferUpdate_f(handle, &cbArg);
    }
    else {
        LOGW("    WARING : delegate.FileTransferUpdate_f is NULL");
    }
    //free(cbArg.value);
    return result;
}
// #endif
