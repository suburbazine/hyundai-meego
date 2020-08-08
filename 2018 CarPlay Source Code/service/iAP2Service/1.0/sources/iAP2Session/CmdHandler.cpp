/*
    File:       CmdHandler.cpp
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
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#include "CmdHandler.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
#include "SessionDefine.h"
namespace IAP2
{

static void* serverThreadFunc(void* data)
{
    CmdHandler* serverThread = (CmdHandler*) data;
    serverThread->serverThread(data);
    return NULL;
}

static bool compareFd(int first, int second)
{
     // descending order
     return first > second;
}

CmdHandler::CmdHandler(LinkController* pLinkController,std::vector<int>* pSockFdVector) : mTid(0), mIsTerminated(false), mServerSockFd(-1), mpSockFdVector(pSockFdVector), 
                            miAP2LnkCtrler(pLinkController)
                    
{
    LOGD("CmdHandler()");
    pthread_mutex_init(&mMutexSsThSync, NULL);
    pthread_cond_init(&mCondSsThSync, NULL);

    pthread_create(&mThread, NULL, serverThreadFunc, (void*)this);

    pthread_mutex_lock(&mMutexSsThSync);
    pthread_cond_wait(&mCondSsThSync, &mMutexSsThSync);
    pthread_mutex_unlock(&mMutexSsThSync);
}

CmdHandler::~CmdHandler()
{
    unsigned int i = 0;
    int retVal = 0;
    void* pThreadResult = NULL;
    
    LOGD("~CmdHandler()");

    mIsTerminated = true;
    
    retVal = pthread_join(mThread, &pThreadResult);
    if (retVal != 0)
        LOGE("    Thread join failed    error[%d][%s]", errno, strerror(errno));
    else
        LOGI("    serverThread is Terminated!!!");
    
    for (i=0; i<  mpSockFdVector->size(); i++) {
        retVal = close( mpSockFdVector->at(i));
        if (retVal == -1)
            LOGE("    ERROR : fail to close  mSockFdVector[%d]=>[%d] error[%d][%s]", i,  mpSockFdVector->at(i), errno, strerror(errno));
    }

    pthread_cond_destroy(&mCondSsThSync);
    pthread_mutex_destroy(&mMutexSsThSync);        
}

void* CmdHandler::serverThread(void* data)
{
    signal(SIGPIPE, SIG_IGN);
    int result = kIap2NoErr;
    fd_set readFds;
    fd_set testFds;
    struct timeval tv;
    struct sockaddr_un clientAddr;
    int clientAddrlen = 0;
    int clientSockFd = -1;
    int fd = -1;

    // CloseSockFdVector includes socket fds to be closed
    std::vector<int> closeSockFdIndexVector;
    std::vector<int>::iterator it;
    CommandHeader header;
    int rcvSize = 0;
   
    (void) data;

    mTid = getTid();
    LOGD("serverThread() created!!! mTid[%d]", mTid);

    pthread_mutex_lock(&mMutexSsThSync);
    pthread_cond_signal(&mCondSsThSync);
    pthread_mutex_unlock(&mMutexSsThSync);

    result = listenClient();
    if (result < 0) {
        LOGE("    ERROR : listenClient failed");
        return NULL; 
    }

    FD_ZERO(&readFds);
    FD_SET(mServerSockFd, &readFds);

    while (!mIsTerminated) {
        testFds = readFds;
        //LOGD("    iAp2 Server waiting !!!");
        tv.tv_sec = 0;
        tv.tv_usec = 100*1000; // 100 msec
        result = select(mpSockFdVector->at(0)+1, &testFds,  NULL, NULL, &tv);
        if (result == -1) {
            LOGE("    ERROR : select failed. error[%d][%s]", errno, strerror(errno));
            return NULL; 
        }

        //LOGD("    =====> mSockFdVector.size[%d]", mSockFdVector.size());

        // first, process mSockFdVector for client !!!
        for (unsigned int index = 0; index < mpSockFdVector->size(); index++) {
            fd = mpSockFdVector->at(index);
            
            //LOGD("    fd[%d]", fd);
            if (FD_ISSET(fd, &testFds) && fd != mServerSockFd) {
             // LOGD("CommandHeader[%d] fd[%d]", sizeof(CommandHeader), fd);
             // let's read data from client
                rcvSize = recv(fd, &header, sizeof(header), MSG_WAITALL);

                if (rcvSize <= 0) {
                    // These calls return the number of bytes received, or -1 if an error occurred. The return value will be 0 when the peer has performed an orderly shutdown
                    FD_CLR(fd, &readFds);
                    closeSockFdIndexVector.push_back(index);
                    LOGE("    ==> close fd[%d]", fd);

                    if ((rcvSize < 0) && (errno == EINTR)) 
                    { 
                        LOGW("EINTR continue"); 
                        continue; 
                    }
                    
                    result = close(fd);
                    if (result == -1) {
                        LOGE("    ERROR : socket close failed. fd[%d] error[%d][%s]", fd, errno, strerror(errno));
                    }
                }
                else {
                    LOGD("    cmd[%u] appHandle[0x%p]", header.cmd, header.appHandle);
                    
                    unsigned char* pRxData = (unsigned char*)malloc(header.dataSize);
                    if (pRxData == NULL){
                        LOGE("    ERROR : pRxData is NULL");
                        return NULL;
                    }
                    // copy header
                    memcpy(pRxData, &header, sizeof(CommandHeader));

                    if (header.dataSize > sizeof(CommandHeader)) {

                        rcvSize = recv(fd, pRxData+sizeof(CommandHeader), header.dataSize - sizeof(CommandHeader), MSG_WAITALL);
                        if (rcvSize <= 0) {
                            // These calls return the number of bytes received, or -1 if an error occurred. The return value will be 0 when the peer has performed an orderly shutdown
                            closeSockFdIndexVector.push_back(index);
                            LOGE("    ==> close fd[%d]", fd);

                            if ((rcvSize < 0) && (errno == EINTR)) 
                            { 
                                LOGW("EINTR continue"); 
								free(pRxData);
                                continue; 
                            }
                            
                            result = close(fd);
                            if (result == -1) {
                                LOGE("    ERROR : socket close failed. fd[%d] error[%d][%s]", fd, errno, strerror(errno));
                            }
                        }
                        else {                    
                            result = handleClientCmd(fd, header.cmd, header.appHandle, pRxData);
                            if (result != kIap2NoErr) {
                                LOGE("    ERROR : handleClientCmd() result[%d]", result);
                            }
                        }
                    }
                    else {
                        result = handleClientCmd(fd, header.cmd, header.appHandle, pRxData);
                        if (result != kIap2NoErr) {
                            LOGE("    ERROR : handleClientCmd() result[%d]", result);
                        }
                    }
                                            
                    free(pRxData);
                }
            }
        }

        if (closeSockFdIndexVector.empty() == false) {
            for (it = closeSockFdIndexVector.begin(); it != closeSockFdIndexVector.end(); ++it) {
                LOGD("    erase fd[%d] in mSockFdVector", fd);
                mpSockFdVector->erase(mpSockFdVector->begin() + *it);
            }
            std::sort(mpSockFdVector->begin(),  mpSockFdVector->end(), compareFd);
            closeSockFdIndexVector.clear();

            for (unsigned int i = 0; i < mpSockFdVector->size(); i++) {
                LOGD("    == mSockFdVector [%d]->[%d]", i, mpSockFdVector->at(i));
            }
        }

        // second process mServerSockFd  !!!
        if (FD_ISSET(mServerSockFd, &testFds)) {
            LOGD("    mServerSockFd[%d] is selected !!!", mServerSockFd);

            clientAddrlen = sizeof(clientAddr);
            clientSockFd = accept(mServerSockFd, (struct sockaddr*) &clientAddr, (socklen_t*)&clientAddrlen);
            if (clientSockFd == -1) {
                LOGE("    ERROR : socket accept failed mServerSockFd[%d] error[%d][%s]", mServerSockFd, errno, strerror(errno));
                //return NULL;
            }
            else {
                FD_SET(clientSockFd, &readFds);
                mpSockFdVector->push_back(clientSockFd);
                std::sort(mpSockFdVector->begin(),  mpSockFdVector->end(), compareFd);
                LOGI("    added new client. clientSockFd[%d] mSockFdVector size[%d]", clientSockFd,  mpSockFdVector->size());
                //LOGD("    clientAddr.sun_path[%s]->[0x%X][0x%X] clientAddrlen[%d]", clientAddr.sun_path, clientAddr.sun_path[0], clientAddr.sun_path[1], clientAddrlen); 
            
                for (unsigned int i = 0; i <mpSockFdVector->size(); i++) {
                    LOGD("    == mSockFdVector [%d]->[%d]", i, mpSockFdVector->at(i));
                }
            }

        }
    }

    LOGI("serverThread() terminated!!! mTid[%d]", mTid);

    return NULL;
}

int CmdHandler::listenClient()
{
    struct sockaddr_un serverAddr = {0, ""};
    //struct sockaddr_un clientAddr;
    int serverAddrlen = 0;
    //int clientAddrlen = 0;

    int result;
    //fd_set readFds, testFds;

    LOGD("listenClient()");

    unlink(SOCKET_FILE_PATH);
    mServerSockFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (mServerSockFd == -1) {
        LOGE("    ERROR : socket open failed. mServerSockFd[%d] error[%d][%s]", mServerSockFd, errno, strerror(errno));
        return kIap2OpenErr; 
    }

    setReuseAddressOption(mServerSockFd);
    setLingerOption(mServerSockFd);    

    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, SOCKET_FILE_PATH);
    serverAddrlen = sizeof(serverAddr);

    result = bind(mServerSockFd, (struct sockaddr*) &serverAddr, serverAddrlen);

#ifdef KOECN
    char socketpath[100] = "chmod 777 ";
    strcat(socketpath, SOCKET_FILE_PATH);
    LOGD("socketpath[%s]", socketpath);
    system(socketpath);
#endif

    if (result == -1) {
        LOGE("    ERROR : socket bind failed. mServerSockFd[%d] error[%d][%s]", mServerSockFd, errno, strerror(errno));
        return kIap2OpenErr; 
    }

    result = listen(mServerSockFd, MAX_CLIENT);
    if (result == -1) {
        LOGE("    ERROR : socket listen failed. mServerSockFd[%d] error[%d][%s]", mServerSockFd, errno, strerror(errno));
        return kIap2OpenErr; 
    }

    mpSockFdVector->push_back(mServerSockFd);
    miAP2LnkCtrler->mpSendEventToApp->mServerSockFd = mServerSockFd;
    LOGI("    ListenClient Connected mServerSockFd[%d] ", mServerSockFd);

    return kIap2NoErr;
}

void CmdHandler::setReuseAddressOption(int sockFd)
{    
    int flag = 1;
    int retVal = 0;

    retVal = setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, (char *) &flag, sizeof(int));
    if (retVal != 0) {
        LOGE("    sockFd[%d] Error no[%d][%s]", sockFd, errno, strerror(errno));
    }
}

void CmdHandler::setLingerOption(int sockFd)
{    
    int retVal = 0;

    struct linger l;

    l.l_onoff = TRUE; // LINGER ï¿½É¼ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
    l.l_linger = 0;   // LINGER Timout ï¿½ï¿½ï¿½

    retVal = setsockopt(sockFd, SOL_SOCKET, SO_LINGER, (char*)&l, sizeof(struct linger));

    if (retVal != 0) {
        LOGE("    sockFd[%d] Error no[%d][%s]", sockFd, errno, strerror(errno));
    }
}

int CmdHandler::handleClientCmd(int currentFd, uint32_t cmd, uint32_t handle, uint8_t* pRxData)
{
    int result = kIap2NoErr;
    LOGI("handleClientCmd() currentFd[%d] cmd[%u] handle[0x%x] pRxData[%p]", currentFd, cmd, handle, pRxData);

    switch(cmd) {
        case kCmdGetInitConnectionData :
        case kCmdCarplayOnOff:
        case kCmdIdentificationSelect:
        case kCmdUsbModeChange:
        case kCmdCarPlayClientState:
        case kCmdStartCarPlay:
        case kCmdTerminateDaemon:
            result = mSessionConn.handleConnectionCmd(currentFd, cmd, handle, pRxData, miAP2LnkCtrler);
            break;
            
        case kCmdStartPowerUpdates :
        case kCmdStopPowerUpdates :
        case kCmdPowerSourceUpdate :
            result = mSessionPower.handlePowerCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;

        case kCmdStartNowPlayingUpdates :
        case kCmdStopNowPlayingUpdates :
        case kCmdSetNowPlayingInformation:
            result = mSessionNowPlaying.handleNowPlayingCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;
        
        case kCmdStartTelephonyCallStateInformation :
        case kCmdStopTelephonyCallStateInformation :
        case kCmdStartTelephonyUpdates :
        case kCmdStopTelephonyUpdates :
            result = mSessionTelephony.handleTelephonyCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;

        case kCmdLocationInformation :
            result = mSessionLocation.handleLocationCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;

        case kCmdStartMediaLibraryInformaion :
        case kCmdStopMediaLibraryInformaion :
            result = mSessionMeidaLibraryInformation.handleMeidaLibraryInformationCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;

        case kCmdStartMediaLibraryUpdates :
        case kCmdStopMediaLibraryUpdates :
            result = mSessionMeidaLibraryUpdates.handleMeidaLibraryUpdatesCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;

        case kCmdPlayMediaLibraryCurrentSelection :
        case kCmdPlayMediaLibraryItems:
        case kCmdPlayMediaLibraryCollection :
        case kCmdPlayMediaLibrarySpecial:
            result = mSessionPlayMediaLibrary.handlePlayMediaLibraryCmd(cmd, handle, pRxData, miAP2LnkCtrler);
               break;

        case kCmdStartHID :
        case kCmdStopHID :
        case kCmdAccessoryHIDReport :
            result = mSessionHID.handleHIDCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;

        case kCmdVehicleStatusUpdate :
            result = mSessionVehicleStatus.handleVehicleStatusCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;

        case kCmdBTComponentInformation:
        case kCmdStartBTConnectionUpdates:
        case kCmdStopBTConnectionUpdates:
            result = mSessionBTConnection.handleBTConnectionCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;

        case kCmdRequestAppLaunch :
            result = mSessionAppLaunch.handleAppLaunchCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;

        case kCmdStartAssistiveTouch :
        case kCmdStopAssistiveTouch :
        case kCmdStartAssistiveTouchInformation : 
        case kCmdStopAssistiveTouchInformation :
               result = mSessionAssistiveTouch.handleAssistiveTouchCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;

        case kCmdStartUSBDeviceModeAudio :
        case kCmdStopUSBDeviceModeAudio : 
            result = mSessionUSBDeviceModeAudio.handleUSBDeviceModeAudioCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;

        case kCmdStartVoiceOver :
        case kCmdStopVoiceOver : 
        case kCmdRequestVoiceOverMoveCursor : 
        case kCmdRequestVoiceOverActivateCursor : 
        case kCmdRequestVoiceOverScrollPage : 
        case kCmdRequestVoiceOverSpeakText : 
        case kCmdRequestVoiceOverPauseText : 
        case kCmdRequestVoiceOverResumeText : 
        case kCmdStartVoiceOverUpdates : 
        case kCmdStopVoiceOverUpdates : 
        case kCmdRequestVoiceOverConfiguration : 
        case kCmdStartVoiceOverCursorUpdates :     
        case kCmdStopVoiceOverCursorUpdates : 
            result = mSessionVoiceOver.handleVoiceOverCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;    

        case kCmdRequestWiFiInformation :
            result = mSessionWiFiInformation.handleWiFiInformationCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;    

        case kCmdStatusExternalAccessoryProtocolSession :
        case kCmdExternalAccessoryProtocolData :
            result = mSessionExternalAccessoryProtocol.handleExternalAccessoryProtocolCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;    

        case kCmdIdentificationUpdate :
            result = mSessionIdentification.handleIdentificationCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;    

        case kCmdStartCallStateUpdates :
        case kCmdStopCallStateUpdates :
        case kCmdStartCommunicationsUpdates :            
        case kCmdStopCommunicationsUpdates :
        case kCmdInitiateCall :
        case kCmdAcceptCall :
        case kCmdEndCall :    
        case kCmdSwapCalls :    
        case kCmdMergeCalls :        
        case kCmdHoldStatusUpdate :
        case kCmdMuteStatusUpdate :            
        case kCmdSendDTMF :
        case kCmdStartListUpdates :    
        case kCmdStopListUpdates:
            result = mSessionCommunications.handleCommunicationsCmd(cmd, handle, pRxData, miAP2LnkCtrler);
            break;        
           
        default :
            LOGE("    ERROR : unknown cmd[%d]", cmd);
            result = kIap2ParamErr;
            break;
    }
    
    return result;
}

}// namespace IAP2
