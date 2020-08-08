
/*
 *    @file        <RxThread.c>
 *    @brief       TCP RX thread for debug
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */

#define LOG_TAG "RxThread"
#define LOG_NDEBUG 0
#ifdef __ANDROID_API__
#include "utils/Log.h"
#endif
#include "tseFilterDebug.h"

#define iALLOW_ONLY_ONE_SOCKET_CONNECTION 1

/** @brief TCP packet RX thread
 *
 *  @param pvArg passing argument from parents thread.
 *  @return NULL.
 *  @remark 주의사항 없음
 */
void * rxThread(void *pvArg)
{
    ALOGD("start rxThread");

    int iError;
    int iBytesReceived;
    int iBytesRingbufferInserted;
    int iLoopCounter = 0;
    char pcRxSocketBuffer[iRX_SOCKET_BUFFER_MAXSIZE];
    tsShared * psShared = (tsShared*)pvArg;

    if (NULL == psShared) {
        ALOGE("rxThread() : psShared null pointer\n");
        return NULL;
    }

    while (iFALSE == psShared->iExitRxThread) {
        if (iFALSE == psShared->iClientConnected) {
        /* FIXME: More then one socket connection is still possible. Why? */
            iError = DbgSocketListen(psShared->hServerSocket, iALLOW_ONLY_ONE_SOCKET_CONNECTION);

            if (iError != 0) {
                ALOGE("rxThread() : listen() failed\n");
                psShared->iExitRxThread = iTRUE;
            }

            if (iFALSE == psShared->iExitRxThread) {
                ALOGI("rxThread() : Waiting for connection on port %i\n", TCP_SOCKET_PORT_NUM);

                /* accept(...) is blocking until the client connects */
                psShared->hSocket = DbgSocketAccept(psShared->hServerSocket, NULL, NULL);

                if (psShared->hSocket == iDBG_INVALID_SOCKET) {
                    ALOGE("rxThread() : accept() failed\n");
                    psShared->iExitRxThread = iTRUE;
                } else    {
                    ALOGI("rxThread() : Connection established on port %i\n", TCP_SOCKET_PORT_NUM);
                    psShared->iClientConnected = iTRUE;
                }
            }
        }

        if (psShared->iClientConnected == iTRUE) {
            /* recv(...) is blocking until some data from the client is available */
            iBytesReceived = DbgSocketRecv(psShared->hSocket, pcRxSocketBuffer, iRX_SOCKET_BUFFER_MAXSIZE, 0);

            if (0 >= iBytesReceived) {
                ALOGW("rxThread() : recv() failed. Try new connection\n");
                psShared->iClientConnected = iFALSE;
            }
            else
            {
                ALOGI("rxThread() : [%i] Got %i bytes\n", iLoopCounter, iBytesReceived);

                iBytesRingbufferInserted = insertIntoRingbuffer(psShared->psRxRingbuffer, pcRxSocketBuffer, iBytesReceived);

                if (iBytesReceived != iBytesRingbufferInserted) {
                    ALOGW("rxThread() : [%i] rx ringbuffer full. Droping %i bytes\n", iLoopCounter, iBytesReceived - iBytesRingbufferInserted);
                }

                iLoopCounter++;
            }
        }
    }

    ALOGD("rxThread() : exit\n");

    return NULL;
}

