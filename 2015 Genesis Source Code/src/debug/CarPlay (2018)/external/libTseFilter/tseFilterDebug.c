/*
 *    @file        <tseFilterDebug.c>
 *    @brief       Filter Debug API of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */

#define LOG_TAG "FilterDebugger"
#define LOG_NDEBUG 0
#ifdef __ANDROID_API__
#include "utils/Log.h"
#endif
#include "tseFilterDebug.h"
#include "tseFilterDebugAPI.h"

#include <stdarg.h> 
#include <stdio.h> 
#include <string.h>

/** @brief free debugger releated memories
 *
 *  @param debugger handle.
 *  @return Void.
 *  @remark 주의사항 없음
 */
static void freeDebugger(Debugger *debugger)
{
    ALOGE("freeDebugger()");
    if (debugger != NULL)
    {
        if (debugger->sShared != NULL)
        {
            if (debugger->sShared->phTxSema != NULL)
            {
                free(debugger->sShared->phTxSema);
            }
            free(debugger->sShared);
        }
        if (debugger->hRxThread != NULL)
        {
            free(debugger->hRxThread);
        }
        if (debugger->hTxThread != NULL)
        {
            free(debugger->hTxThread);
        }
        if (debugger->sTseDebugBuffer != NULL)
        {
            free(debugger->sTseDebugBuffer);
        }
        free(debugger);
    }
}

/** @brief start debugging mode (allocation memory, create ring buffer, create TX/RX thread, open TCP socket)
 *
 *  @param ctx filter context handle.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int setupDebugMode(FilterContext *ctx)
{
    ALOGE("setupDebuggerMode()");

    int iError;
    int iSocketOption;
    struct sDBG_SOCKADDR_IN sIpAddr;
    Debugger * debugger = NULL;
    tsShared *sShared = NULL; //fix 20150421 DefectId=86162 
    
    debugger = (Debugger *)calloc(1, sizeof(Debugger));
    if (debugger == NULL) {
        ALOGE("ERROR : setupDebuggerEnv() : calloc() failed for debugger.");
        goto error;
    }

    sShared = (tsShared *)calloc(1, sizeof(tsShared));
    if (sShared == NULL) {
        ALOGE("ERROR : setupDebuggerEnv() : calloc() failed for tsShared.");
        goto error;
    }
    debugger->sShared = sShared;

    debugger->hRxThread = (tDbgThread *)malloc(sizeof(tDbgThread));
    debugger->hTxThread = (tDbgThread *)malloc(sizeof(tDbgThread));
    debugger->sTseDebugBuffer = (ttseDebug *)calloc(1, sizeof(ttseDebug));

    sShared->iExitRxThread = iFALSE;
    sShared->iExitTxThread = iFALSE;
    sShared->iExitProcessThread = iFALSE;
    sShared->iClientConnected = iFALSE;
    sShared->phTxSema = (tDbgSema *)calloc(1, sizeof(tDbgSema));
    sShared->psRxRingbuffer = NULL;
    sShared->psTxRingbuffer = NULL;

    iError = createRingbuffer(&(sShared->psRxRingbuffer), iRX_RINGBUFFER_SIZE);

    if (iRINGBUFFER_OK != iError) {
        ALOGE("ERROR : setupDebuggerEnv() : createRingbuffer() failed for rx.");
        goto error;
    }

    iError = createRingbuffer(&(sShared->psTxRingbuffer), iTX_RINGBUFFER_SIZE);

    if (iRINGBUFFER_OK != iError) {
        ALOGE("ERROR : setupDebuggerEnv() : createRingbuffer() failed for tx.");
        goto error;
    }

    sIpAddr.sin_family = DBG_AF_INET;
    sIpAddr.sin_addr.s_addr = DBG_INADDR_ANY;
    sIpAddr.sin_port = Dbghtons((short)TCP_SOCKET_PORT_NUM);

    sShared->hServerSocket = DbgSocketCreate(DBG_AF_INET, DBG_SOCK_STREAM, 0);

    if (sShared->hServerSocket == iDBG_INVALID_SOCKET) {
        ALOGE("ERROR : setupDebuggerEnv() : socket() failed" );
        goto error;
    }

    iSocketOption = 1;
    iError = DbgSocketSetSockOpt(sShared->hServerSocket,
        DBG_SOL_SOCKET,
        DBG_SO_REUSEADDR,
        &iSocketOption,
        (tDBG_SOCKLEN)sizeof(int));
    if (iError != 0) {
        ALOGE("ERROR : setupDebuggerEnv() : setsockopt() failed");
        goto error;
    }

    iError = DbgSocketBind( sShared->hServerSocket,
        (struct sDBG_SOCKADDR*)&sIpAddr,
        sizeof(sIpAddr) );

    if (iError != 0) {
        ALOGE("ERROR : setupDebuggerEnv() : bind() failed" );
        goto error;
    }

    /* create tx semaphore.
       Tx thread notification about available TSE debug out data */

    iError = DbgSemaInit(sShared->phTxSema, 0, 0);
    if (iError != 0) {
        ALOGE("ERROR : setupDebuggerEnv() : sem_init() for tx semaphore failed");
        goto error;
    }

    /* create all threads */

    iError = DbgThreadCreate(debugger->hRxThread, rxThread, sShared);
    if (iError != 0) {
        ALOGE("ERROR : setupDebuggerEnv() : pthread_create() for rxThread failed");
        goto error;
    }

    iError = DbgThreadCreate(debugger->hTxThread, txThread, sShared);
    if (iError != 0) {
        ALOGE("ERROR : setupDebuggerEnv() : pthread_create() for txThread failed");
        goto error;
    }

    ctx->debugger = debugger;
    return iTRUE;

error:
    freeDebugger(debugger);
    return iFALSE;
}

/** @brief exit debugging mode (free memory, destroy ring buffer, destroy thread, close TCP socket)
 *
 *  @param ctx filter context handle.
 *  @return Void.
 *  @remark 주의사항 없음
 */
void unsetDebugMode(FilterContext *ctx)
{
    ALOGE("unsetDebuggerMode()");

    Debugger *debugger = ctx->debugger;
    if (debugger == NULL)
        return;

    tsShared *sShared = debugger->sShared;

    /* close server socket and a connected client socket */

    if (sShared->iClientConnected == iTRUE) {
        DbgSocketShutdown(sShared->hSocket, iDBG_SOCKET_SHUT_RDWR);
        DbgSocketClose(sShared->hSocket);
    }

    DbgSocketShutdown(sShared->hServerSocket, iDBG_SOCKET_SHUT_RDWR);
    DbgSocketClose(sShared->hServerSocket);

    /* set exit notification for all threads */

    sShared->iExitProcessThread = iTRUE;
    sShared->iExitRxThread = iTRUE;
    sShared->iExitTxThread = iTRUE;
    DbgSemaPost(sShared->phTxSema);

    /* wait for all threads exiting */

    DbgThreadJoin(*(debugger->hRxThread));
    DbgThreadJoin(*(debugger->hTxThread));

    /* destroy semaphore */

    DbgSemaDestroy(sShared->phTxSema);

    /* destroy all ringbuffers */

    destroyRingbuffer(&(sShared->psRxRingbuffer));
    destroyRingbuffer(&(sShared->psTxRingbuffer) );

    freeDebugger(debugger);

    ctx->debugger = NULL;
}


LGTSE_logCB	_g_log_CB = NULL;

/** @brief set user defined log print callbck
 *
 *  @param userCB log print callback function pointer.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int tseFilter_log_init(LGTSE_logCB userCB)
{
	if(userCB)
		_g_log_CB = userCB;
	return 0;
}

/** @brief unset user defined log print callbck
 *
 *  @return Void.
 *  @remark 주의사항 없음
 */
void tseFilter_log_deinit()
{
	_g_log_CB = NULL;
}

/** @brief make user defined log print string format
 *
 *  @param func call function name.
 *  @param line call line number.
 *  @param fmt format string.
 *  @return Void.
 *  @remark 주의사항 없음
 */
void tseFilter_log_printf(const char *func, const int line, char *fmt, ...) 
{
	va_list ap; 
	va_start(ap, fmt); 
#define MAX_LOG_STR_SIZE	256
	if(_g_log_CB){
		char msg[MAX_LOG_STR_SIZE];
		char msg_tmp[MAX_LOG_STR_SIZE];
#ifdef WIN32
		_snprintf(msg, MAX_LOG_STR_SIZE, "%d:%s] ", line, func); 
#else
		snprintf(msg, MAX_LOG_STR_SIZE, "%d:%s] ", line, func); 
#endif
		vsnprintf(msg_tmp, MAX_LOG_STR_SIZE, fmt, ap); 
		_g_log_CB(strncat(msg, msg_tmp, MAX_LOG_STR_SIZE - strlen(msg) - 1) );
	}else{
		//fprintf(stderr, "%d:%s] ", line, func); 
		vfprintf(stderr, fmt, ap); 
	}
	va_end(ap); 
} 
