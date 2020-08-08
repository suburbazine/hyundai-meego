/*
    File:       FileTransferManage.cpp
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

#include "FileTransferManage.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include <stdlib.h>
#include "LinkController.h"
#define MAX_USBPATH 2

namespace IAP2
{

FileTransferManage::FileTransferManage()
{
    LOGD("FileTransferManage()");
    pFileTransferManageQHead = NULL;
    pFileTransferManageQTail = NULL;     
    queueInit(&pFileTransferManageQHead, &pFileTransferManageQTail);    
    pthread_mutex_init(&FileTransferManageMutex, NULL);
}

FileTransferManage::~FileTransferManage()
{
    LOGD("~FileTransferManage()");
    queueFinal(pFileTransferManageQHead, pFileTransferManageQTail, TRUE);
    pthread_mutex_destroy(&FileTransferManageMutex);
}


int FileTransferManage::RegisterFileTransferInfo(stFileTransferInform* mstFileTransferInform)
{
    LOGD("RegisterFileTransferInfo()mstFileTransferInform : [%p]", mstFileTransferInform);
    int result = kIap2NoErr;

    if(mstFileTransferInform == NULL) {
        LOGE("ERROR : mstFileTransferInform  is NULL");
        return kIap2ParamErr;
    }
    
    pthread_mutex_lock(&FileTransferManageMutex);    
    if(queuePut(pFileTransferManageQHead, pFileTransferManageQTail, mstFileTransferInform) == FALSE)
    {
        LOGE("Fail queuePut!!");
        LOGE("RegisterFileTransferInfo Failed!!");
        pthread_mutex_unlock(&FileTransferManageMutex);
        return kIap2NotWritableErr;
    }
    pthread_mutex_unlock(&FileTransferManageMutex);
    LOGD("RegisterFileTransferInfo Success!!");
    return result;

}

int FileTransferManage::UnRegisterFileTransferInfo(stFileTransferInform* mstFileTransferInform)
{
    dnode_t* deleteNode = NULL;
    int result = kIap2NoErr;
    if( mstFileTransferInform == NULL ) {
        LOGE("UnRegisterFileTransferInfo() mstFileTransferInform is NULL");
        return kIap2ValueErr;
    }
        
    LOGD("UnRegisterFileTransferInfo () mstFileTransferInform->m_piAP2FileTransfer : %p", mstFileTransferInform->m_piAP2FileTransfer);                
    if(mstFileTransferInform->m_piAP2FileTransfer != NULL) {
        LOGD("FileTransfer Memory Confirm Delete m_piAP2FileTransfer : %p", mstFileTransferInform->m_piAP2FileTransfer);
        free(mstFileTransferInform->m_piAP2FileTransfer);
    }
    
    deleteNode = listFindNodeWithData(pFileTransferManageQHead,  pFileTransferManageQTail, mstFileTransferInform);
    if(mstFileTransferInform != NULL) {
        LOGD("FileTransfer Memory Confirm Delete mstFileTransferInform : %p", mstFileTransferInform);
        free(mstFileTransferInform);
    }
    
    if(FALSE == listDeleteNode(pFileTransferManageQHead, pFileTransferManageQTail, deleteNode))
    {
        LOGE("Can't find node!!");
        result = kIap2NotFoundErr;
    }
    else
    {        
        LOGD("Node DeleteSuccess!!");
    }
    return result;
}

stFileTransferInform* FileTransferManage::searchDatafromiAP2FileTransfer(iAP2FileTransfer_t* piAP2FileTransfer)
{
    LOGD("searchDatafrombufferID() piAP2FileTransfer : %p", piAP2FileTransfer);
    stFileTransferInform* mstFileTransferInform = NULL;
    dnode_t* nextNode = NULL;

    //pthread_mutex_lock(&UsbManageMutex);
    nextNode = pFileTransferManageQHead->next;    
    mstFileTransferInform = (stFileTransferInform*) listGetNodeAfterNode(pFileTransferManageQHead, pFileTransferManageQTail, pFileTransferManageQHead);
    
    while (mstFileTransferInform) {
        if( mstFileTransferInform->m_piAP2FileTransfer == piAP2FileTransfer )
        {
            //pthread_mutex_unlock(&UsbManageMutex);
            return mstFileTransferInform;
        }
        else
        {    
            mstFileTransferInform = (stFileTransferInform*) listGetNodeAfterNode(pFileTransferManageQHead, pFileTransferManageQTail, nextNode);
            nextNode = nextNode->next;            
        }
    }        

    //pthread_mutex_unlock(&UsbManageMutex);
    LOGE("Dont Find matching buuferID's data!!()");
    return NULL;

}

stFileTransferInform* FileTransferManage::searchDatafrombufferID(uint8_t buuferID)
{
    LOGD("searchDatafrombufferID() buuferID : %u", buuferID);
    stFileTransferInform* mstFileTransferInform = NULL;
    dnode_t* nextNode = NULL;

    //pthread_mutex_lock(&UsbManageMutex);
    nextNode = pFileTransferManageQHead->next;    
    mstFileTransferInform = (stFileTransferInform*) listGetNodeAfterNode(pFileTransferManageQHead, pFileTransferManageQTail, pFileTransferManageQHead);
    
    while (mstFileTransferInform) {
        if( mstFileTransferInform->bufferID == buuferID )
        {
            //pthread_mutex_unlock(&UsbManageMutex);
            return mstFileTransferInform;
        }
        else
        {    
            mstFileTransferInform = (stFileTransferInform*) listGetNodeAfterNode(pFileTransferManageQHead, pFileTransferManageQTail, nextNode);
            nextNode = nextNode->next;            
        }
    }        

    //pthread_mutex_unlock(&UsbManageMutex);
    LOGE("Dont Find matching buuferID's data!!()");
    return NULL;

}


int FileTransferManage::searchDatafromConnPathAndUnRegisterFileTransfer(ConnectionPath connPath)
{
    LOGD("searchDatafromConnPathAndUnRegister() connPath : %u", connPath);
    int result = kIap2NoErr;
    int searchCount = 0;
    stFileTransferInform* mstFileTransferInform = NULL;
    dnode_t* nextNode = NULL;

    pthread_mutex_lock(&FileTransferManageMutex);    
    nextNode = pFileTransferManageQHead->next;    
    mstFileTransferInform = (stFileTransferInform*) listGetNodeAfterNode(pFileTransferManageQHead, pFileTransferManageQTail, pFileTransferManageQHead);
    
    while (mstFileTransferInform) {
        if( mstFileTransferInform->connPath == connPath )
        {
            nextNode = nextNode->next;    //move next node before delete node
            result  = UnRegisterFileTransferInfo(mstFileTransferInform);
                
            if(result != kIap2NoErr)
                LOGE("Fail FileTransfer Unregister!!  : %u ", result);
        }    
        mstFileTransferInform = (stFileTransferInform*) listGetNodeAfterNode(pFileTransferManageQHead, pFileTransferManageQTail, nextNode);
        nextNode = nextNode->next;    
        searchCount++;
    }
    pthread_mutex_unlock(&FileTransferManageMutex);    
    LOGD(" searchCount : %u", searchCount);
    return result;

}

} // namespace IAP2


