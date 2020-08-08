

#ifndef FILTER_DEBUG_H_
#define FILTER_DEBUG_H_

/*
 *    @file        <tseFilterDebug.h>
 *    @brief       Filter Debug API header of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
/* --------------------------------------------------------------- */

//#include "MvoiceFilter_priv.h"

#include "dbg_sema.h"
#include "dbg_mutex.h"
#include "dbg_thread.h"
#include "dbg_socket.h"
#include "tse.h"
#include "ringbuffer.h"
#ifdef __ANDROID_API__
#include "utils/Log.h"
#endif
#define iIPPORT 2012
#ifndef TCP_SOCKET_PORT_NUM
#define TCP_SOCKET_PORT_NUM iIPPORT
#endif

#define iRX_SOCKET_BUFFER_MAXSIZE (1024*3) //128
#define iRX_RINGBUFFER_SIZE (32 * iRX_SOCKET_BUFFER_MAXSIZE)
#define iDEBUG_BUFFER_IN_MAXSIZE iRX_SOCKET_BUFFER_MAXSIZE

#define iTX_SOCKET_BUFFER_MAXSIZE (1024*6)//512
#define iTX_RINGBUFFER_SIZE (8 * iTX_SOCKET_BUFFER_MAXSIZE)
#define iDEBUG_BUFFER_OUT_MAXSIZE iTX_SOCKET_BUFFER_MAXSIZE

//#define iPROCESS_MILLISECOND_SLEEP 2000

//#define iCHARCODE_EXIT 101 /* this is the 'e' character */

#define iFALSE 0
#define iTRUE 1

#ifdef __ANDROID_API__
	#ifndef ALOGE
	#define ALOGE LOGE
	#endif
	#ifndef ALOGD
	#define ALOGD LOGD
	#endif
	#ifndef ALOGI
	#define ALOGI LOGI
	#endif
	#ifndef ALOGW
	#define ALOGW LOGW
	#endif
#define fprintf(a,args...) ALOGE(args)
#define TSE_LOG_PRINTF(fmt,...) ALOGE(fmt, ##__VA_ARGS__)

#else
#define TSE_LOG_PRINTF_ERR(fmt,...) tseFilter_log_printf(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define TSE_LOG_PRINTF_WARN(fmt,...) tseFilter_log_printf(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define TSE_LOG_PRINTF_DBG(fmt,...) tseFilter_log_printf(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define TSE_LOG_PRINTF_INFO(fmt,...) tseFilter_log_printf(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define ALOGE  TSE_LOG_PRINTF_ERR
#define ALOGW  TSE_LOG_PRINTF_WARN
#define ALOGD  TSE_LOG_PRINTF_DBG
#define ALOGI  TSE_LOG_PRINTF_INFO
#endif

void * rxThread( void * pvArg );
void * txThread( void * pvArg );

struct _sShared
{
   tsRingbuffer * psRxRingbuffer;
   tsRingbuffer * psTxRingbuffer;

   tDbgSema * phTxSema;

   int iExitRxThread;
   int iExitTxThread;
   int iExitProcessThread;

   tDBG_SOCKET hServerSocket;
   tDBG_SOCKET hSocket;
   int iClientConnected;
};

typedef struct _sShared tsShared;

typedef struct _Debugger {
    tsShared *sShared;
    tDbgThread *hRxThread;
    tDbgThread *hTxThread;
    ttseDebug *sTseDebugBuffer;
} Debugger;

typedef void (*LGTSE_logCB)(char *msg);

//int setupDebugMode(FilterContext *ctx);
//void unsetDebugMode(FilterContext *ctx);
void tseFilter_log_printf(const char *, const int, char *, ...); 
int tseFilter_log_init(LGTSE_logCB userCB);
void tseFilter_log_deinit();

/* --------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif /* C++ */

#endif /* FILTER_DEBUG_H_ */
