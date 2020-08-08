#ifndef iAP2Link_iAP2LinkRunLoopImp_h
#define iAP2Link_iAP2LinkRunLoopImp_h

#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdint.h>
#include <pthread.h>

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct IAPServiceContext_st
{
	pthread_t hThread;					//  iAP2LinkRunLoopThread handle
	BOOL bThreadTerminate;
	pthread_mutex_t iAP2LinkRLTMutex;
	pthread_cond_t  iAP2LinkRLTCond;

	pthread_mutex_t eventMaskMutex;

	pthread_mutex_t iAP2LinkListMutex; //#LGE_CHAGE Yongil.park Problem : packet value is null in the send-packet list

	// queue that contains recieved packet from device(iPhone)
	dnode_t* pRcvPacketQHead;
	dnode_t* pRcvPacketQTail;

	//pthread_mutex_t timerMutex;

	// expired timer event queue
	dnode_t* pExpiredTimerQHead;
	dnode_t* pExpiredTimerQTail;

	// timer list
	dnode_t* pTimerListHead;
	dnode_t* pTimerListTail;
} IAPServiceContext_t;

void initContext(IAPServiceContext_t* pContext);
void finalContext(IAPServiceContext_t* pContext);
void processTimer(IAPServiceContext_t* pIApContext);
void retreiveRcvPacket(IAPServiceContext_t* pIApContext, void** ppRcvPacket, unsigned int* pnRcvPacket);

#ifdef __cplusplus
}
#endif


#endif // iAP2Link_iAP2LinkRunLoopImp_h

