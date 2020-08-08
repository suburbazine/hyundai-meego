
/*
 *    @file        <TxThread.c>
 *    @brief       TCP TX thread for debug
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */

#define LOG_TAG "TxThread"
#define LOG_NDEBUG 0
#ifdef __ANDROID_API__
#include "utils/Log.h"
#endif
#include "tseFilterDebug.h"

/** @brief TCP packet TX thread
 *
 *  @param pvArg passing argument from parents thread.
 *  @return NULL.
 *  @remark 주의사항 없음
 */
void *txThread(void *pvArg)
{
    ALOGD("start txThread \r\n");
    int iError;
    int iLoopCounter = 0;
    tsShared * psShared = (tsShared*)pvArg;
    char pcTxSocketBuffer[iTX_SOCKET_BUFFER_MAXSIZE];
    int iBytesTransmitted;
    int iBytesRingbufferExtracted;


    if (psShared == NULL) {
        ALOGE("txThread() : psShared null pointer\n");
        return NULL;
    }

    if (psShared->phTxSema == NULL) {
        ALOGE("txThread() : null pointer\n");
        return NULL;
    }

    while (iFALSE == psShared->iExitTxThread) {
        iError = DbgSemaWait(psShared->phTxSema);

        if (iError != 0) {
            ALOGE("txThread() : sem_wait() failed\n");
            psShared->iExitTxThread = iTRUE;
        } else {
            ALOGI("txThread() : execute loop %i\n", iLoopCounter++);
        /* thread work */
            if (psShared->iClientConnected == iTRUE) {
                iBytesRingbufferExtracted = extractFromRingbuffer(psShared->psTxRingbuffer, pcTxSocketBuffer, iTX_SOCKET_BUFFER_MAXSIZE);

                if (0 < iBytesRingbufferExtracted) {
                    iBytesTransmitted = DbgSocketSend(psShared->hSocket, pcTxSocketBuffer, iBytesRingbufferExtracted, 0);

                    if (iBytesTransmitted == -1) {
                        ALOGE("txThread() : send() failed\n");
                        psShared->iExitTxThread = iTRUE;
                    } else if (iBytesTransmitted < iBytesRingbufferExtracted) {
                        ALOGE("txThread() : send() dropping %i bytes\n", iBytesRingbufferExtracted - iBytesTransmitted);
                    }
                }
            }
        }
    }

    ALOGI("txThread() : exit\n");

    return NULL;
}
