/*
    File:       iAp2Connection.c
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

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

#include "iAp2AppFramework.h"
#include "iAp2Marshal.h"
#include "iAp2AppFrameworkCommon.h"
#include "iAp2AppFrameworkPrivate.h"
#include "iAp2AppFrameworkExternal.h"
#include "iAp2AppFrameworkExternal_command.h"
#include "iAp2AppFrameworkExternal_event.h"
#include "iAp2Log.h"
#include "iAp2Connection.h"
#include "iAp2AppFrameworkPower.h"
#include "iAp2AppFrameworkNowPlaying.h"
#include "iAp2AppFrameworkVehicleStatus.h"
#include "iAp2AppFrameworkDeviceNotification.h"
#include "iAp2AppFrameworkBTConnection.h"
#include "iAp2AppFrameworkLocation.h"
#include "iAp2AppFrameworkAssistiveTouch.h"
#include "iAp2AppFrameworkHID.h"
#include "iAp2AppFrameworkMediaLibraryInformation.h"
#include "iAp2AppFrameworkExternalAccessoryProtocol.h"
#include "iAp2AppFrameworkUSBDeviceModeAudio.h"
#include "iAp2AppFrameworkVoiceOver.h"
#include "iAp2AppFrameworkWiFiInformation.h"
#include "iAp2AppFrameworkCommunications.h"
#include "iAp2AppFrameworkFileTransfer.h"
//===========================================================================================================================
//    Definitions
//===========================================================================================================================


//===========================================================================================================================
//    Declarations
//===========================================================================================================================
static void* iAp2EventHandleThread(void* data);
static int iAp2ConectionHandleServerEvent(iAp2AppRef inRef, uint32_t evt, unsigned char* pRxData);

// TODO : internal use only. let's hide from app later.
//extern int iAp2ConectionHandleEventPowerUpdate(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventNowPlayingUpdate(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventStartVehicleStatusUpdate(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventStopVehicleStatusUpdate(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventDeviceInformationUpdate(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventDeviceLanguageUpdate(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventTelephonyCallStateInformation(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventTelephonyUpdate(iAp2AppRef inRef, unsigned char* pRxData);

//extern int iAp2ConectionHandleEventBTConnectionUpdate(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventStartLocationInformation(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventStopLocationInformation(iAp2AppRef inRef, unsigned char* pRxData);

//ADD Yongil 2014.10.23
//extern int iAp2ConectionHandleEventAssistiveTouchInformation(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventDeviceHIDReport(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventMeidaLibraryInformation(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventMediaLibraryUpdates(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventStartExternalAccessoryProtocolSession(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventStopExternalAccessoryProtocolSession(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventUSBDeviceModeAudioInformation(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventVoiceOverUpdate(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventVoiceOverCursorUpdate(iAp2AppRef inRef, unsigned char* pRxData);
//extern int iAp2ConectionHandleEventWiFiInformation(iAp2AppRef inRef, unsigned char* pRxData);
//END Yongil


//===========================================================================================================================
//    iAp2ConnectionConnectToService
//===========================================================================================================================
int iAp2ConnectionConnectToService(iAp2AppRef inRef)
{
    #define RETRY_INTERVAL (50*1000) // 50 ms
    #define RETRY_TIME (3000*1000) // 3 sec
    
    int len;
    struct sockaddr_un address;
    int result;
    int retryCount = 0;
    int maxRetryCount = 0;

    //LOGD("iAp2ConnectionConnectToService() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    inRef->sockFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (inRef->sockFd == -1) {
        LOGE("    ERROR : socket open failed. inRef->sockFd[%d] error[%d][%s]", inRef->sockFd, errno, strerror(errno));
        return kIap2OpenErr; 
    }

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_FILE_PATH);
    len = sizeof(address);

    maxRetryCount = RETRY_TIME/RETRY_INTERVAL;

    while (1) {
        result = connect(inRef->sockFd, (struct sockaddr*) &address, len);
        if (result == -1 && retryCount < maxRetryCount) {
            usleep(RETRY_INTERVAL); // 50 msec
            retryCount++;
        }
        else if(result >= 0) {
            LOGI("connection retryCount[%d]", retryCount);
            LOGI("connection succes fd : [%u] " , inRef->sockFd);
            return kIap2NoErr;
        }
        else {
            LOGE("    ERROR : socket connect failed. inRef->sockFd[%d] error[%d][%s] retryCount[%d]", inRef->sockFd, errno, strerror(errno), retryCount);
            return kIap2NotPreparedErr;
        }
    }

    return kIap2NoErr;
}

//===========================================================================================================================
//    iAp2ConnectionConnectToService
//===========================================================================================================================
int iAp2ConnectionDisconnectToService(iAp2AppRef inRef)
{
    int result;

    LOGD("iAp2ConnectionDisconnectToService() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (inRef->sockFd != -1) {
        result = close(inRef->sockFd);

        if (result == -1) {
            LOGE("    ERROR : socket close failed. inRef->sockFd[%d] error[%d][%s]", inRef->sockFd, errno, strerror(errno));
            return kIap2OpenErr;
        }
        inRef->sockFd = -1;
    }
    LOGD("iAp2ConnectionDisconnectToService() inRef[%p] END", inRef);
    return kIap2NoErr;
}


//===========================================================================================================================
//    iAp2ConnectionSendCmd
//===========================================================================================================================
int iAp2ConnectionSendCmd(iAp2AppRef inRef, void* inData, unsigned int inLength)
{
    int retValue = kIap2NoErr;
    int sendSize = 0;
    struct iAp2AppPrivate* handle = inRef;

    //LOGD("iAp2ConnectionSendCmd() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (inData == NULL) {
        LOGE("ERROR : inArg is NULL");
        return kIap2ParamErr;
    }

    if (inLength == 0) {
        LOGE("ERROR : inLength is 0");
        return kIap2ParamErr;
    }

    if (handle->sockFd == -1) {
        LOGE("ERROR : sockFd is invalid");
        return kIap2ConnectionErr;
    }
    if (handle->connState < CS_POWER_UPDATED) {
        LOGE("ERROR : connState[%d] is not CS_POWER_UPDATED", handle->connState);
        return kIap2StateErr;
    }

    sendSize = send(handle->sockFd, (char*) inData, inLength, MSG_DONTWAIT);
    
    //LOGD("inData[%p] inLength[%d] sendSize[%p]", inData, inLength, sendSize);
    if (sendSize == -1) {
        LOGE("    ERROR : socket close failed. sockFd[%d] error[%d][%s]", handle->sockFd, errno, strerror(errno));
    }

    return retValue;
}

//===========================================================================================================================
//    iAp2ConnectionSettingCmd
//===========================================================================================================================
int iAp2ConnectionSettingCmd(iAp2AppRef inRef, void* inData, unsigned int inLength)
{
    int retValue = kIap2NoErr;
    int sendSize = 0;
    struct iAp2AppPrivate* handle = inRef;

    //LOGD("iAp2ConnectionSettingCmd() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (inData == NULL) {
        LOGE("ERROR : inArg is NULL");
        return kIap2ParamErr;
    }

    if (inLength == 0) {
        LOGE("ERROR : inLength is 0");
        return kIap2ParamErr;
    }

    if (handle->sockFd == -1) {
        LOGE("ERROR : sockFd is invalid");
        return kIap2ConnectionErr;
    }

    sendSize = send(handle->sockFd, (char*) inData, inLength, MSG_DONTWAIT);
    
    //LOGD("inData[%p] inLength[%d] sendSize[%p]", inData, inLength, sendSize);
    if (sendSize == -1) {
        LOGE("    ERROR : socket close failed. sockFd[%d] error[%d][%s]", handle->sockFd, errno, strerror(errno));
    }

    return retValue;
}

//===========================================================================================================================
//    iAp2ConnectionGetInitConnectionData
//===========================================================================================================================
int iAp2ConnectionGetInitConnectionData(iAp2AppRef inRef, InitConnectionData* outInitConnData)
{
    int retValue = kIap2NoErr;
    int sendSize = 0;
    int rcvSize = 0;
        
    struct iAp2AppPrivate* handle = inRef;

    //LOGD("iAp2ConnectionGetInitConnectionData() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (outInitConnData == NULL) {
        LOGE("ERROR : outInitConnData is NULL");
        return kIap2ParamErr;
    }

    if (handle->sockFd == -1) {
        LOGE("ERROR : sockFd is invalid");
        return kIap2ConnectionErr;
    }

    uint32_t byteNum = 0;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    
    uint32_t ipcData[16/4];
    uint32_t dummyValue = 0xFFFFFFFF;

    iAp2MarshalInit(&iAp2Marshal, ipcData, 16, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, dummyValue);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    ipcData[0] = kCmdGetInitConnectionData;
    ipcData[1] = (uint32_t)inRef;
    ipcData[2] = byteNum;

    //LOGD("cmd[%u] appHandle[%p] dataSize[%u] dummyValue[0x%x]", ipcData[0], ipcData[1], ipcData[2], ipcData[3]);

    sendSize = send(handle->sockFd, (char*) ipcData, byteNum, MSG_DONTWAIT);
    if (sendSize == -1) {
        LOGE("    ERROR : socket send failed. sockFd[%d] error[%d][%s]", handle->sockFd, errno, strerror(errno));
    }

    EventHeader header;
    rcvSize = recv(handle->sockFd, &header, sizeof(header), MSG_WAITALL);
    if (rcvSize == -1) {
        LOGE("    ERROR : socket recv failed. sockFd[%d] error[%d][%s]", handle->sockFd, errno, strerror(errno));
    }
    else {
        //LOGD("    evt[%u] appHandle[%p] dataSize[%u]", header.evt, header.appHandle, header.dataSize);
        
        unsigned char* pRxData = (unsigned char*)malloc(header.dataSize);
        if (pRxData == NULL){
            LOGE("    ERROR : pRxData is NULL");
            return kIap2NoMemoryErr;
        }
        // copy header
        memcpy(pRxData, &header, sizeof(EventHeader));

        rcvSize = recv(handle->sockFd, pRxData+sizeof(EventHeader), header.dataSize - sizeof(EventHeader), MSG_WAITALL);
        if (rcvSize <= 0) {
            retValue = close(handle->sockFd);
            if (retValue == -1) {
                LOGE("    ERROR : socket close failed. sockFd[%d] error[%d][%s]", handle->sockFd, errno, strerror(errno));
            }
        }
        else {
            iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);
            outInitConnData->connState = (ConnectionState)iAp2MarshalGetUint32(&iAp2Marshal);
            outInitConnData->connPath = (ConnectionPath)iAp2MarshalGetUint32(&iAp2Marshal);            
            outInitConnData->connMode = (ConnectionMode)iAp2MarshalGetUint32(&iAp2Marshal);
            outInitConnData->carplaySupport = (ConnectionCarPlaySupport)iAp2MarshalGetUint32(&iAp2Marshal);
            outInitConnData->iap2Support = (ConnectionIap2Support)iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("    connState[%u] connMode[%u] carplaySupport[%u] iap2Support[%u]", outInitConnData->connState, outInitConnData->connMode, outInitConnData->carplaySupport, outInitConnData->iap2Support);
        }

        free(pRxData);
    }    

    return retValue;
}


//===========================================================================================================================
//    iAp2ConnectionStartEventHandling
//===========================================================================================================================
int iAp2ConnectionStartEventHandling(iAp2AppRef inRef)
{
    struct iAp2AppPrivate* handle = inRef;
    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }
    
    int result = pthread_create(&handle->iAp2EvtHandleThread, NULL, iAp2EventHandleThread, (void*)inRef);
    if (result != 0) {
        LOGE("    ERROR : pthread_create failed. result[%d]", result);
        return kIap2UnexpectedErr;
    }

    return kIap2NoErr;
}

//===========================================================================================================================
//    iAp2ConnectionStopEventHandling
//===========================================================================================================================
int iAp2ConnectionStopEventHandling(iAp2AppRef inRef)
{
    LOGD("    iAp2ConnectionStopEventHandling");
    int result = 0;
    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    struct iAp2AppPrivate* handle = inRef;
    
    handle->mIsTerminated = TRUE;

    result = pthread_join(handle->iAp2EvtHandleThread, NULL);
    if (result != 0) {
        LOGE("    ERROR : pthread_Stop failed. result[%d]", result);
        return kIap2UnexpectedErr;
    }
    LOGD("    iAp2ConnectionStopEventHandling END");
    return kIap2NoErr;
}

//===========================================================================================================================
//    iAp2EventHandleThread
//===========================================================================================================================
static void* iAp2EventHandleThread(void* data)
{
    fd_set fds;
    struct timeval tv;
    int ret;

    int result = kIap2NoErr;
    int rcvSize = 0;
    EventHeader header;
    struct iAp2AppPrivate* handle = (struct iAp2AppPrivate*)data;

    if (handle == NULL) {
        LOGE("ERROR : handle is NULL");
        return NULL;
    }

    LOGD("iAp2EventHandleThread() handle[%p]", handle);

    while (!handle->mIsTerminated) {
        FD_ZERO(&fds);
        FD_SET(handle->sockFd, &fds);
        tv.tv_sec = 0;
        tv.tv_usec = 100*1000; // 100 msec
        //LOGD("    iAp2 Server waiting !!! : %u ", handle->mIsTerminated);
        ret = select(handle->sockFd+1, &fds, NULL, NULL, &tv);
        if (ret > 0 && FD_ISSET(handle->sockFd, &fds)) {
            // let's read data from server
            rcvSize = recv(handle->sockFd, &header, sizeof(header), MSG_WAITALL);

            if (rcvSize <= 0) {
                // These calls return the number of bytes received, or -1 if an error occurred. The return value will be 0 when the peer has performed an orderly shutdown
                LOGE("    ==> close sockFd[%d]", handle->sockFd);
                LOGE("    ==> rcvSize[%d]", rcvSize);

                if ((rcvSize < 0) && (errno == EINTR)) 
                { 
                    LOGE("EINTR continue"); 
                    continue; 
                }
                
                result = close(handle->sockFd);
                if (result == -1) {
                    LOGE("    ERROR : socket close failed. sockFd[%d] error[%d][%s]", handle->sockFd, handle->sockFd, strerror(errno));
                }
                
                handle->mIsTerminated = TRUE;
            }
            else {
                //LOGD("    evt[%u] appHandle[%p] dataSize[%u]", header.evt, header.appHandle, header.dataSize);

                unsigned char* pRxData = (unsigned char*)malloc(header.dataSize);
                if (pRxData == NULL){
                    LOGE("    ERROR : pRxData is NULL");
                    return NULL;
                }
                // copy header
                memcpy(pRxData, &header, sizeof(EventHeader));

                if (header.dataSize > sizeof(EventHeader)) {
                    rcvSize = recv(handle->sockFd, pRxData+sizeof(EventHeader), header.dataSize - sizeof(EventHeader), MSG_WAITALL);
                    if (rcvSize <= 0) {
                        LOGE("    ==> close sockFd[%d]", handle->sockFd);
                        LOGE("    ==> rcvSize[%d]", rcvSize);
                        
                        if ((rcvSize < 0) && (errno == EINTR)) 
                        { 
                            LOGE("EINTR continue"); 
                            continue; 
                        }
                        
                        result = close(handle->sockFd);
                        if (result == -1) {
                            LOGE("    ERROR : socket close failed. sockFd[%d] error[%d][%s]", handle->sockFd, errno, strerror(errno));
                        }
                        
                        handle->mIsTerminated = TRUE;
                    }
                    else {
                        // TODO : multi-application handling.
                        if (1/*handle == header.appHandle*/) {
                            
                            result = iAp2ConectionHandleServerEvent(handle, header.evt, pRxData);
                            if (result != kIap2NoErr) {
                                LOGE("    ERROR : iAp2ConectionHandleServerEvent() result[%d]", result);
                            }
                        }
                        else {
                            LOGE("    ERROR : Unkonwn header.appHandle[%p] handle[%p]", header.appHandle, handle);
                        }
                    }
                }
                else {
                    // TODO : multi-application handling.
                    if (1/*handle == header.appHandle*/) {
                        
                        result = iAp2ConectionHandleServerEvent(handle, header.evt, pRxData);
                        if (result != kIap2NoErr) {
                            LOGE("    ERROR : iAp2ConectionHandleServerEvent() result[%d]", result);
                        }
                    }
                    else {
                        LOGE("    ERROR : Unkonwn header.appHandle[%p] handle[%p]", header.appHandle, handle);
                    }
                }

                free(pRxData);
            }
        }
    }
    //iAp2ConectionHandleSocketClose(handle);
    LOGI("iAp2EventHandleThread() handle[%p] END", handle);
    return NULL;
}

//===========================================================================================================================
//    iAp2ConectionHandleServerEvent
//===========================================================================================================================
static int iAp2ConectionHandleServerEvent(iAp2AppRef inRef, uint32_t evt, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    //LOGI("iAp2ConectionHandleServerEvent() inRef[0x%x] evt[%u] pRxData[%p]", inRef, evt, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    switch(evt) {
        case kEvtConnectionInfoUpdate :
            result = iAp2ConectionHandleEventConnectionInfoUpdate(inRef, pRxData);
            break;
            
        case kEvtConnectionStateUpdate :
            result = iAp2ConectionHandleEventConnectionStateUpdate(inRef, pRxData);
            break;
            
        case kEvtConnectionStateAndPathUpdate :
            result = iAp2ConectionHandleEventConnectionStateAndPathUpdate(inRef, pRxData);
            break;

        case kEvtConnectionModeUpdate :
            result = iAp2ConectionHandleEventConnectionModeUpdate(inRef, pRxData);
            break;

        case kEvtConnectionCarplaySupportUpdate :
            result = iAp2ConectionHandleEventCarplaySupportUpdate(inRef, pRxData);
            break;

        case kEvtConnectionIap2SupportUpdate :
            result = iAp2ConectionHandleEventIap2SupportUpdate(inRef, pRxData);
            break;
#ifdef ANDROID
        case kEvtConnectionIap2DeviceInfoUpdate :
            result = iAp2ConectionHandleEventDeviceInfoUpdateUpdate(inRef, pRxData);
            break;
#endif
            
        case kEvtPowerUpdate :
            result = iAp2ConectionHandleEventPowerUpdate(inRef, pRxData);
            break;

        case kEvtNowPlayingUpdate :
            result = iAp2ConectionHandleEventNowPlayingUpdate(inRef, pRxData);
            break;

// #ifdef iAP2SERVICE_FILETRANSFER
        //case kEvtNowPlayingAlbumArtworkUpdate:
        //    result = iAp2ConectionHandleEventNowPlayingAlbumArtwork(&inRef, pRxData);
        //    break;
// #endif
            
        case kEvtStartVehicleStatusUpdates :
            result = iAp2ConectionHandleEventStartVehicleStatusUpdate(inRef, pRxData);
            break;

        case kEvtStopVehicleStatusUpdates :
            result = iAp2ConectionHandleEventStopVehicleStatusUpdate(inRef, pRxData);
            break;

        case kEvtDeviceInformationUpdate :
            result = iAp2ConectionHandleEventDeviceInformationUpdate(inRef, pRxData);
            break;

        case kEvtDeviceLanguageUpdate : 
            result = iAp2ConectionHandleEventDeviceLanguageUpdate(inRef, pRxData);
            break;
            
        case kEvtTelephonyCallStateInformation :
            result = iAp2ConectionHandleEventTelephonyCallStateInformation(inRef, pRxData);
            break;

        case kEvtTelephonyUpdate :
            result = iAp2ConectionHandleEventTelephonyUpdate(inRef, pRxData);
            break;

        case kEvtBTConnectionUpdate :
            result  = iAp2ConectionHandleEventBTConnectionUpdate(inRef, pRxData);
            break;

        case kEvtStartLocationInformation :
            result = iAp2ConectionHandleEventStartLocationInformation(inRef, pRxData);
            break;

        case kEvtStopLocationInformation :
            result = iAp2ConectionHandleEventStopLocationInformation(inRef, pRxData);
            break;

// ADD Yongil 2014.10.23
        case kEvtDeviceHIDReport : 
            result = iAp2ConectionHandleEventDeviceHIDReport(inRef, pRxData);
            break;        
            
        case kEvtMediaLibraryInformation : 
            result = iAp2ConectionHandleEventMeidaLibraryInformation(inRef, pRxData);
            break;
            
        case kEvtMediaLibraryUpdates : 
            result = iAp2ConectionHandleEventMediaLibraryUpdates(inRef, pRxData);
            break;
            
        case kEvtAssistiveTouchInformation : 
            result = iAp2ConectionHandleEventAssistiveTouchInformation(inRef, pRxData);
            break;    
            
        case kEvtStartExternalAccessoryProtcolSession : 
            result = iAp2ConectionHandleEventStartExternalAccessoryProtocolSession(inRef, pRxData);
            break;    
            
        case kEvtStopExternalAccessoryProtcolSession : 
            result = iAp2ConectionHandleEventStopExternalAccessoryProtocolSession(inRef, pRxData);
            break;    

        case kEvtExternalAccessoryProtcolData :
            result = iAp2ConectionHandleExternalAccessoryProtocolData(inRef, pRxData);
            break;    
            
        case kEvtUSBDeviceModeAudioInformation: 
            result = iAp2ConectionHandleEventUSBDeviceModeAudioInformation(inRef, pRxData);
            break;    
            
        case kEvtVoiceOverUpdates:
            result = iAp2ConectionHandleEventVoiceOverUpdate(inRef, pRxData);
            break;

        case kEvtVoiceOverCursorUpdates:
            result = iAp2ConectionHandleEventVoiceOverCursorUpdate(inRef, pRxData);
            break;

        case kEvtWiFiInformation:
            result = iAp2ConectionHandleEventWiFiInformation(inRef, pRxData);
            break;

        case kEvtCallStateUpdate:
            result = iAp2ConectionHandleEventCallStateUpdates(inRef, pRxData);
            break;    

        case kEvtCommunicationsUpdate:
            result = iAp2ConectionHandleEventCommunicationsUpdates(inRef, pRxData);
            break;        

        case kEvtListUpdateUpdate:
            result = iAp2ConectionHandleEventListUpdate(inRef, pRxData);
            break;    
           	   
        case kEvtFileTransferUpdate : 
            result = iAp2ConectionHandleEventFileTransferUpdate(inRef, pRxData);
            break;

        case kEvtAuthenticationFailed:
            result = iAp2ConectionHandleEventAuthenticaitonFailed(inRef, pRxData);
            break;
            
        case kEvtIdentificationFailed:
            result = iAp2ConectionHandleEventIdentificationFailed(inRef, pRxData);
            break;
//END Yongil            
        default :
            LOGE("    ERROR : unknown evt[%d]", evt);
            result = kIap2ParamErr;
            break;
    }
    
    return result;

}

