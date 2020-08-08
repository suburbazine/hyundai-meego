/*
    File:       FileTransfer.cpp
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

#include "FileTransfer.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include "SendEventToApp.h"
#include <stdlib.h>

namespace IAP2
{

FileTransfer::FileTransfer()
{
    LOGD("FileTransfer()");
}

FileTransfer::~FileTransfer()
{
    LOGD("~FileTransfer()");
}

void FileTransfer::createIAP2FileTransferCreate(LinkController* pLnkCtrler, uint8_t bufferID, ConnectionPath RecvPath, FILETRANSFERMODE UserInfo )
{

    LOGD("createIAP2FileTransferCreate() : ID (%d)", bufferID);
    int result = 0;
    
    stFileTransferInform* mstFileTransferInform = NULL;
    mstFileTransferInform = (stFileTransferInform*) malloc(sizeof(stFileTransferInform));
    mstFileTransferInform->m_piAP2FileTransfer =  (iAP2FileTransfer_t*)malloc(sizeof(iAP2FileTransfer_t));
    mstFileTransferInform->mFiletransferMode = UserInfo;
    mstFileTransferInform->bufferID = bufferID;
    mstFileTransferInform->connPath = RecvPath;
    LOGD("FileTransfer Memory Confirm Create mstFileTransferInform : %p", mstFileTransferInform);
    LOGD("FileTransfer Memory Confirm Create m_piAP2FileTransfer : %p", mstFileTransferInform->m_piAP2FileTransfer);
    LOGD("mstFileTransferInform->mFiletransferMode : %u", mstFileTransferInform->mFiletransferMode);
    LOGD("iAP2FileTransferGotDataCB_Caller : %p", iAP2FileTransferGotDataCB_Caller);
    LOGD("iAP2FileTransferEndCB_Caller  : %p, bufferID : %u", iAP2FileTransferEndCB_Caller, bufferID);
    iAP2FileTransferCreate( pLnkCtrler->m_piAP2LinkRunLoop->link,
                        0x02,
                        bufferID,
                        iAP2FileTransferGotDataCB_Caller,
                        iAP2FileTransferEndCB_Caller,
                        (void*)&mstFileTransferInform->mFiletransferMode,
                        TRUE,
                        mstFileTransferInform->m_piAP2FileTransfer
                    );

    result = pLnkCtrler->mFileTransferManage.RegisterFileTransferInfo(mstFileTransferInform);
    if(result != kIap2NoErr)
        LOGE("ERROR FileTransfer register failed!!");    


#if 0 // MUlti Source
    LOGD("createIAP2FileTransferCreate() : ID (%d)", bufferID);
    int result = 0;
    stUsbConectionInform* mstUsbConectionInform = NULL;
    mstUsbConectionInform = pLnkCtrler->mUsbManage.searchDatafromConnPath(RecvPath);

    stFileTransferInform* mstFileTransferInform = NULL;
    mstFileTransferInform = (stFileTransferInform*) malloc(sizeof(stFileTransferInform));
    mstFileTransferInform->m_piAP2FileTransfer =  (iAP2FileTransfer_t*)malloc(sizeof(iAP2FileTransfer_t));
    mstFileTransferInform->mFiletransferMode = UserInfo;
    mstFileTransferInform->bufferID = bufferID;
    mstFileTransferInform->connPath = RecvPath;
    LOGD("FileTransfer Memory Confirm Create mstFileTransferInform : %p", mstFileTransferInform);
    LOGD("FileTransfer Memory Confirm Create m_piAP2FileTransfer : %p", mstFileTransferInform->m_piAP2FileTransfer);
    LOGD("mstFileTransferInform->mFiletransferMode : %u", mstUsbConectionInform->mFiletransferMode);
    if(mstUsbConectionInform == NULL)
    {
        LOGE("ERROR mstUsbConectionInform is NULL!!");
        return ;
    }
    pthread_mutex_lock(&pLnkCtrler->mUsbManage.UsbManageMutex);
    iAP2FileTransferCreate(mstUsbConectionInform->linkRunLoop->link,
                        mstUsbConectionInform->mLinkRunLoopCB->m_synParam.sessionInfo[1].id,
                        bufferID,
                        mstUsbConectionInform->mLinkRunLoopCB->iAP2FileTransferGotDataCB_Caller,
                        mstUsbConectionInform->mLinkRunLoopCB->iAP2FileTransferEndCB_Caller,
                        (void*)&mstFileTransferInform->mFiletransferMode,
                        TRUE,
                        mstFileTransferInform->m_piAP2FileTransfer
                    );
    pthread_mutex_unlock(&pLnkCtrler->mUsbManage.UsbManageMutex);

    result = pLnkCtrler->mFileTransferManage.RegisterFileTransferInfo(mstFileTransferInform);
    if(result != kIap2NoErr)
        LOGE("ERROR FileTransfer register failed!!");    
#endif
#if 0
    // Use For Transfer Data ((Accessory -> iOS))
    iAP2FileTransferStart(m_piAP2FileTransfer,
                    m_piAP2FileTransfer->pBuffer,
                    m_piAP2FileTransfer->buffSize,
                    m_piAP2FileTransfer->totalSize,
                    iAP2FileTransferEndCB_Caller,
                    m_piAP2FileTransfer->gotCBUserInfo,
                    m_piAP2FileTransfer->bStream,
                    m_piAP2FileTransfer->bDeleteBuffOnFinish
                    );
#endif
}



BOOL FileTransfer::iAP2FileTransferGotDataCB_Caller(struct iAP2FileTransfer_st* fileXfer, void* userInfo)
{
    LOGD("    iAP2FileTransferGotDataCB_Caller :: ENTER : fileXfer(%p), userinfo(%p), state(%u), sentSize(%u), BufferSentSize(%u), buffSize(%u)", fileXfer, userInfo, fileXfer->state, fileXfer->sentSize, fileXfer->buffSentSize, fileXfer->buffSize);

    if (fileXfer && userInfo) {
        iAP2Link_st* pLink = (iAP2Link_st*)(fileXfer->link);
        LinkController* pInstance = (LinkController*)(LinkController*)((iAP2LinkRunLoop_t*)(pLink->context))->context;

        LOGD(" <<FILEXFER>> :: pInstance = %p LinkController = %p", pInstance, fileXfer->link->context);
        
        pInstance->recvFileTransferDataCB((iAP2FileTransfer_st*)fileXfer, (void*)userInfo);
    }
    return TRUE;
}

BOOL FileTransfer::iAP2FileTransferEndCB_Caller(struct iAP2FileTransfer_st* fileXfer, void* userInfo)
{
    LOGD("    iAP2FileTransferEndCB_Caller :: ENTER");
    
    if (fileXfer && userInfo) {       
       iAP2Link_st* pLink = (iAP2Link_st*)(fileXfer->link);
       LinkController* pInstance = (LinkController*)(LinkController*)((iAP2LinkRunLoop_t*)(pLink->context))->context;
       FILETRANSFERMODE* mFileTransferMode = (FILETRANSFERMODE *)userInfo;
       if(pInstance == NULL)
       {
           LOGE("ERROR    pInstance is NULL");
           return FALSE;
       }
       else
       {    
           if(fileXfer->state != kiAP2FileTransferStateFinishRecv)
               pInstance->mFileTransfer.handleFileTransferUpdate((iAP2FileTransfer_st*)fileXfer, *mFileTransferMode);
           iAP2FileTransferDelete(fileXfer);
           
           stFileTransferInform* mstFileTransferInform = NULL;
           mstFileTransferInform = pInstance->mFileTransferManage.searchDatafrombufferID(fileXfer->bufferID);
           if(mstFileTransferInform == NULL) 
               LOGE("Don't find!! matching bufferID");
           else {
               LOGD("delete mstFileTransferInform : %p", mstFileTransferInform);
               pthread_mutex_lock(&pInstance->mFileTransferManage.FileTransferManageMutex);    
               pInstance->mFileTransferManage.UnRegisterFileTransferInfo(mstFileTransferInform);
               pthread_mutex_unlock(&pInstance->mFileTransferManage.FileTransferManageMutex);    
               LOGD("iAP2FileTransferDelete");
           }
       }       
    }
    return TRUE;
}

void FileTransfer::handleFileTransferUpdate(iAP2FileTransfer_st* fileXfer, FILETRANSFERMODE mFileTransferMode)
{
    int result = kIap2NoErr;
    FileTransferUpdateEvt event;

    if ( fileXfer == NULL) {
        LOGE("    Error : fileXfer is NULL");
        return;
    }
    
    iAP2Link_st* pLink = (iAP2Link_st*)(fileXfer->link);

    if ( pLink == NULL) {
          LOGE("    Error : pLink is NULL");
        return;
    }
    LinkController* pInstance = (LinkController*)(LinkController*)((iAP2LinkRunLoop_t*)(pLink->context))->context;    

    if (pInstance == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }
    
    LOGD("    handleFileTransferUpdate :: ENTER : fileXfer(%p),fileXfer->link(%p) mFileTransferMode(%u), state(%u), sentSize(%u), BufferSentSize(%u), buffSize(%u)", fileXfer, fileXfer->link,mFileTransferMode, fileXfer->state, fileXfer->sentSize, fileXfer->buffSentSize, fileXfer->buffSize);
    ConnectionPath RecvPath = CP_NONE;    
    RecvPath = pInstance->mConnection.mConnPath;
       
    // Confirm State
    LOGD("handleFileTransferUpdate() state[%u]", fileXfer->state);    
    memset(&event, 0x00, sizeof(event));
    decodeFileTransferUpdate(fileXfer, &event);
    event.mFileTransferMode = mFileTransferMode;
    LOGD("handleFileTransferUpdate(), event.mFileTransferMode : %u", event.mFileTransferMode);
    
    result = handleFileTransferUpdateEvent(pInstance,&event, RecvPath);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleFileTransferUpdate(). result[%d]", result);


}

void FileTransfer::decodeFileTransferUpdate(iAP2FileTransfer_st* fileXfer, FileTransferUpdateEvt* pEvt)
{
    LOGD("decodeFileTransferUpdate()");

    if (pEvt == NULL) {
        LOGE("    Error : pEvt is NULL");
        return;
    }
    pEvt->state = (FileTransferState)fileXfer->state;
    pEvt->totSize = fileXfer->totalSize;
    pEvt->buffSize= fileXfer->buffSize;
    if(fileXfer->pBuffer && (pEvt->buffSize > 0)) {
        pEvt->bPresent = TRUE;
        pEvt->value = (uint8_t*) malloc(sizeof(uint64_t) * fileXfer->buffSize);
        memcpy(pEvt->value, fileXfer->pBuffer, fileXfer->buffSize);
    }
    pEvt->bufferID = fileXfer->bufferID;

}

int FileTransfer::handleFileTransferUpdateEvent(LinkController* pLnkCtrler, FileTransferUpdateEvt* pEvent, ConnectionPath RecvPath)
{
    
    int result = kIap2NoErr;

    LOGD("handleFileTransferUpdateEvent() RecvPath : %u", RecvPath);
    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t mallocSize = sizeof(FileTransferUpdateEvt) + pEvent->buffSize + 100;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

       
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(EventHeader));

    LOGD("pEvent->state[%u]", pEvent->state);
    LOGD("pEvent->totSize[%u]", pEvent->totSize);
    LOGD("pEvent->bPresent : %u", pEvent->bPresent);
    LOGD("pEvent->buffSize : %u", pEvent->buffSize);      
    LOGD("pEvent->bufferID [%u]", pEvent->bufferID);
    LOGD("pEvent->mFileTransferMode [%u]", pEvent->mFileTransferMode);
    pEvent->mFileTransferMode = FILETRANSFERMODE_NOWPLAYING_ARTWORK;
    iAp2MarshalPutUint64(&iAp2Marshal, pEvent->state);
    iAp2MarshalPutUint64(&iAp2Marshal, pEvent->totSize);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bPresent);
    if(pEvent->bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->value, (uint32_t)pEvent->buffSize);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bufferID);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mFileTransferMode);  
        
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);    

    mEventHeader->evt = kEvtFileTransferUpdate;
    mEventHeader->dataSize = byteNum;
//    mEventHeader->RecvPath = RecvPath;
    
   if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(0, pIpcData, byteNum);
    }
    else {
        LOGE("    ERROR : mpSessionServer is NULL");
    }

    if (result != kIap2NoErr) {
        LOGE("    ERROR : send event fail. result[%d]", result);
    }
    
    free(pEvent->value);
    free(pIpcData);
    return result;
}

#if 0
int FileTransfer::handleiAP2FileTransferRequestCMD(LinkController* pLnkCtrler, uint8_t bufferID, ConnectionPath SendPath, FileTransferCMD eFileTransferCMD)
{
    int result = kIap2NoErr;
    LOGD("sendiAP2FileTransferCancel() : bufferID (%u), SendPath : %u", bufferID, SendPath);
    stUsbConectionInform* mstUsbConectionInform = NULL;

    if (pLnkCtrler == NULL) {
        LOGE("ERROR pLnkCtrler is NULL!!");
        result = kIap2ParamErr;
        return result;
    }
        
    mstUsbConectionInform = pLnkCtrler->mUsbManage.searchDatafromConnPath(SendPath);
    if(mstUsbConectionInform == NULL)
    {
        LOGE("ERROR mstUsbConectionInform is NULL!!");
        result = kIap2NotFoundErr;
        return result;
    }
    
    
    stFileTransferInform* mstFileTransferInform = NULL;
    mstFileTransferInform = pLnkCtrler->mFileTransferManage.searchDatafrombufferID(bufferID);
    if(mstFileTransferInform == NULL)
    {
        LOGE("ERROR mstFileTransferInform is NULL!!");
        result = kIap2NotFoundErr;
        return result;
    }    
 
    LOGD("FileTransfer Memory Confirm Create mstFileTransferInform : %p", mstFileTransferInform);
    LOGD("FileTransfer Memory Confirm Create m_piAP2FileTransfer : %p", mstFileTransferInform->m_piAP2FileTransfer);
    LOGD("mstFileTransferInform->mFiletransferMode : %u", mstUsbConectionInform->mFiletransferMode);
    
    switch(eFileTransferCMD)
    {        
        case FT_CANCEL:
            LOGD("    FT_CANCEL");
            iAP2FileTransferCancel(mstFileTransferInform->m_piAP2FileTransfer);
            break;

        case FT_PAUSE:
            LOGD("    FT_PAUSE");
            iAP2FileTransferPause(mstFileTransferInform->m_piAP2FileTransfer);
            break;
            
        case FT_FAILURE:
            LOGD("    FT_FAILURE");
            iAP2FileTransferFailure(mstFileTransferInform->m_piAP2FileTransfer);
            break;
            
        case FT_RESUME:
            LOGD("    FT_RESUME");
            iAP2FileTransferResume(mstFileTransferInform->m_piAP2FileTransfer);
            break;
            
        default :
            result = kIap2IDErr;
            LOGD("    Unkonw FileTransfer CMD ID : %u ", eFileTransferCMD);
    }

    return result;    
}
#endif
} // namespace IAP2
